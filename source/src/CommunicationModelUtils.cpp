#include "CommunicationModelUtils.h"
#include "CommunicationDistanceModel.h"
#include "EnvironmentLossConfigManager.h"
#include <cmath>
#include <algorithm>
#include <numeric>

// 前向声明的枚举类型定义
enum class CommunicationQuality {
    EXCELLENT = 5,             // 优秀
    GOOD = 4,                  // 良好
    FAIR = 3,                  // 一般
    POOR = 2,                  // 较差
    FAILED = 1                 // 失败
};

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace CommunicationModelUtils {

    // ==================== 快速计算函数 ====================
    
    double quickCalculatePower(double frequency, double range, EnvironmentType env) {
        // 从配置获取环境特性
        const EnvironmentLossConfig& config = EnvironmentLossConfigManager::getConfig(env);
        
        double wavelength = 300.0 / frequency; // MHz to meters
        double freeSpaceRef = 20.0 * std::log10(4.0 * M_PI / wavelength);
        double pathLoss = freeSpaceRef + 10.0 * config.pathLossExponent * std::log10(range * 1000.0);
        
        // 考虑环境损耗和频率因子
        double environmentLoss = config.environmentLoss;
        double frequencyLoss = config.frequencyFactor * std::log10(frequency / 1000.0) * 2.0;
        double totalLoss = environmentLoss + frequencyLoss;
        
        return -100.0 + 10.0 + pathLoss + totalLoss; // 噪声功率 + SNR + 路径损耗 + 环境损耗
    }
    
    CommunicationQuality quickAssessQuality(double snr, double ber) {
        if (snr > 20.0 && ber < 1e-6) return CommunicationQuality::EXCELLENT;
        else if (snr > 10.0 && ber < 1e-4) return CommunicationQuality::GOOD;
        else if (snr > 5.0 && ber < 1e-2) return CommunicationQuality::FAIR;
        else if (snr > 0.0 && ber < 0.1) return CommunicationQuality::POOR;
        else return CommunicationQuality::FAILED;
    }
    
    // ==================== 单位转换函数 ====================
    
    double dBmToWatts(double dBm) {
        return std::pow(10.0, (dBm - 30.0) / 10.0);
    }
    
    double wattsTodBm(double watts) {
        return 10.0 * std::log10(watts) + 30.0;
    }
    
    double dBToLinear(double dB) {
        return std::pow(10.0, dB / 10.0);
    }
    
    double linearTodB(double linear) {
        return 10.0 * std::log10(linear);
    }
    
    double frequencyToWavelength(double frequency) {
        return 300.0 / frequency; // MHz to meters
    }
    
    double wavelengthToFrequency(double wavelength) {
        return 300.0 / wavelength; // meters to MHz
    }
    
    // ==================== 环境建模辅助函数 ====================
    
    double calculateAtmosphericLoss(double frequency, double distance, double humidity) {
        // 基于ITU-R P.676建议的大气损耗模型
        double frequencyGHz = frequency / 1000.0;
        
        // 氧气吸收
        double oxygenLoss = 0.0;
        if (frequencyGHz < 10.0) {
            oxygenLoss = 0.0067 * frequencyGHz * frequencyGHz;
        } else {
            oxygenLoss = 0.067 * frequencyGHz;
        }
        
        // 水蒸气吸收
        double waterVaporLoss = 0.0;
        if (frequencyGHz > 1.0) {
            double waterVaporDensity = humidity * 0.1; // 简化的水蒸气密度计算
            waterVaporLoss = 0.05 * waterVaporDensity * frequencyGHz;
        }
        
        return (oxygenLoss + waterVaporLoss) * distance;
    }
    
    double calculateRainLoss(double frequency, double distance, double rainRate) {
        // 基于ITU-R P.838建议的雨衰模型
        double frequencyGHz = frequency / 1000.0;
        
        // 雨衰系数（简化模型）
        double k, alpha;
        if (frequencyGHz < 1.0) {
            k = 0.0001;
            alpha = 0.5;
        } else if (frequencyGHz < 10.0) {
            k = 0.001 * std::pow(frequencyGHz, 1.5);
            alpha = 1.0;
        } else {
            k = 0.01 * std::pow(frequencyGHz, 0.5);
            alpha = 1.2;
        }
        
        return k * std::pow(rainRate, alpha) * distance;
    }
    
    double calculateFoliageLoss(double frequency, double distance, double foliageDensity) {
        // 基于ITU-R P.833建议的植被损耗模型
        double frequencyGHz = frequency / 1000.0;
        
        // 植被损耗系数
        double specificLoss = 0.2 * std::pow(frequencyGHz, 0.3) * foliageDensity;
        
        return specificLoss * distance;
    }
    
    double calculateUrbanLoss(double frequency, double distance, double buildingDensity) {
        // 基于Okumura-Hata模型的城市环境损耗
        double frequencyMHz = frequency;
        
        // 城市环境修正因子
        double urbanCorrection = 0.0;
        if (frequencyMHz > 150.0) {
            urbanCorrection = 3.2 * std::pow(std::log10(11.75 * 30.0), 2) - 4.97; // 假设30m天线高度
        }
        
        // 建筑密度影响
        double densityFactor = buildingDensity * 10.0; // 简化的密度因子
        
        return (urbanCorrection + densityFactor) * std::log10(distance);
    }
    
    // ==================== 统计分析函数 ====================
    
    double calculateMean(const std::vector<double>& values) {
        if (values.empty()) return 0.0;
        
        double sum = std::accumulate(values.begin(), values.end(), 0.0);
        return sum / values.size();
    }
    
    double calculateStandardDeviation(const std::vector<double>& values) {
        if (values.size() < 2) return 0.0;
        
        double mean = calculateMean(values);
        double sumSquaredDiff = 0.0;
        
        for (double value : values) {
            double diff = value - mean;
            sumSquaredDiff += diff * diff;
        }
        
        return std::sqrt(sumSquaredDiff / (values.size() - 1));
    }
    
    double calculatePercentile(const std::vector<double>& values, double percentile) {
        if (values.empty()) return 0.0;
        
        std::vector<double> sortedValues = values;
        std::sort(sortedValues.begin(), sortedValues.end());
        
        double index = (percentile / 100.0) * (sortedValues.size() - 1);
        int lowerIndex = static_cast<int>(std::floor(index));
        int upperIndex = static_cast<int>(std::ceil(index));
        
        if (lowerIndex == upperIndex) {
            return sortedValues[lowerIndex];
        }
        
        double weight = index - lowerIndex;
        return sortedValues[lowerIndex] * (1.0 - weight) + sortedValues[upperIndex] * weight;
    }
    
    std::pair<double, double> calculateConfidenceInterval(const std::vector<double>& values, double confidence) {
        if (values.size() < 2) return {0.0, 0.0};
        
        double mean = calculateMean(values);
        double stdDev = calculateStandardDeviation(values);
        
        // 使用t分布的近似值（对于大样本，接近正态分布）
        double alpha = 1.0 - confidence;
        double tValue = 1.96; // 95%置信度的近似值
        
        if (confidence > 0.99) tValue = 2.58;
        else if (confidence > 0.95) tValue = 1.96;
        else if (confidence > 0.90) tValue = 1.64;
        else tValue = 1.28;
        
        double margin = tValue * stdDev / std::sqrt(values.size());
        
        return {mean - margin, mean + margin};
    }
    
    // ==================== 兼容性函数 ====================
    
    CommunicationQuality evaluateQuality(double snr, double ber) {
        return quickAssessQuality(snr, ber);
    }
    
    CommunicationQuality quickEvaluateQuality(double snr, double ber) {
        return quickAssessQuality(snr, ber);
    }
}