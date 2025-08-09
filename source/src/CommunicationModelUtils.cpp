#include "CommunicationModelUtils.h"
#include "CommunicationDistanceModel.h"
#include "EnvironmentLossConfigManager.h"
#include "MathConstants.h"
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

namespace CommunicationModelUtils {

    // ==================== 单位转换函数 ====================
    
    double frequencyToWavelength(double frequency) {
        return MathConstants::FREQ_WAVELENGTH_CONSTANT / frequency; // MHz to meters
    }
    
    double wavelengthToFrequency(double wavelength) {
        return MathConstants::FREQ_WAVELENGTH_CONSTANT / wavelength; // meters to MHz
    }

    // ==================== 快速计算函数 ====================
    
    double quickCalculatePower(double frequency, double range, EnvironmentType env) {
        // 从配置获取环境特性
        const EnvironmentLossConfig& config = EnvironmentLossConfigManager::getConfig(env);
        
        double wavelength = frequencyToWavelength(frequency);
        double freeSpaceRef = MathConstants::FSPL_DISTANCE_COEFFICIENT * std::log10(4.0 * MathConstants::PI / wavelength);
        double pathLoss = freeSpaceRef + MathConstants::LINEAR_TO_DB_MULTIPLIER * config.pathLossExponent * std::log10(range * 1000.0);
        
        // 考虑环境损耗和频率因子
        double environmentLoss = config.environmentLoss;
        double frequencyLoss = config.frequencyFactor * std::log10(frequency / 1000.0) * MathConstants::FREQ_FACTOR_MULTIPLIER;
        double totalLoss = environmentLoss + frequencyLoss;
        
        return -100.0 + MathConstants::LINEAR_TO_DB_MULTIPLIER + pathLoss + totalLoss; // 噪声功率 + SNR + 路径损耗 + 环境损耗
    }
    
    CommunicationQuality quickAssessQuality(double snr, double ber) {
        if (snr > MathConstants::EXCELLENT_SNR_THRESHOLD && ber < MathConstants::EXCELLENT_BER_THRESHOLD) return CommunicationQuality::EXCELLENT;
        else if (snr > MathConstants::GOOD_SNR_THRESHOLD && ber < MathConstants::GOOD_BER_THRESHOLD) return CommunicationQuality::GOOD;
        else if (snr > MathConstants::FAIR_SNR_THRESHOLD && ber < MathConstants::FAIR_BER_THRESHOLD) return CommunicationQuality::FAIR;
        else if (snr > 0.0 && ber < MathConstants::POOR_BER_THRESHOLD) return CommunicationQuality::POOR;
        else return CommunicationQuality::FAILED;
    }
    
    // ==================== 其他单位转换函数 ====================
    
    double dBmToWatts(double dBm) {
        return std::pow(MathConstants::LINEAR_TO_DB_MULTIPLIER, (dBm - MathConstants::DBM_TO_WATTS_OFFSET) / MathConstants::DB_TO_LINEAR_DIVISOR);
    }
    
    double wattsTodBm(double watts) {
        return MathConstants::LINEAR_TO_DB_MULTIPLIER * std::log10(watts) + MathConstants::DBM_TO_WATTS_OFFSET;
    }
    
    double dBToLinear(double dB) {
        return std::pow(MathConstants::LINEAR_TO_DB_MULTIPLIER, dB / MathConstants::DB_TO_LINEAR_DIVISOR);
    }
    
    double linearTodB(double linear) {
        return MathConstants::LINEAR_TO_DB_MULTIPLIER * std::log10(linear);
    }
    
    // ==================== 环境建模辅助函数 ====================
    
    double calculateAtmosphericLoss(double frequency, double distance, double humidity) {
        // 基于ITU-R P.676建议的大气损耗模型
        double frequencyGHz = frequency / 1000.0;
        
        // 氧气吸收
        double oxygenLoss = 0.0;
        if (frequencyGHz < MathConstants::LINEAR_TO_DB_MULTIPLIER) {
            oxygenLoss = MathConstants::OXYGEN_ABSORPTION_COEFF_1 * frequencyGHz * frequencyGHz;
        } else {
            oxygenLoss = MathConstants::OXYGEN_ABSORPTION_COEFF_2 * frequencyGHz;
        }
        
        // 水蒸气吸收
        double waterVaporLoss = 0.0;
        if (frequencyGHz > 1.0) {
            double waterVaporDensity = humidity * MathConstants::WATER_VAPOR_DENSITY_FACTOR; // 简化的水蒸气密度计算
            waterVaporLoss = MathConstants::WATER_VAPOR_ABSORPTION_COEFF * waterVaporDensity * frequencyGHz;
        }
        
        return (oxygenLoss + waterVaporLoss) * distance;
    }
    
    double calculateRainLoss(double frequency, double distance, double rainRate) {
        // 基于ITU-R P.838建议的雨衰模型
        double frequencyGHz = frequency / 1000.0;
        
        // 雨衰系数（简化模型）
        double k, alpha;
        if (frequencyGHz < 1.0) {
            k = MathConstants::RAIN_K_LOW_FREQ;
            alpha = MathConstants::RAIN_ALPHA_LOW_FREQ;
        } else if (frequencyGHz < MathConstants::LINEAR_TO_DB_MULTIPLIER) {
            k = MathConstants::RAIN_K_MID_FREQ_BASE * std::pow(frequencyGHz, 1.5);
            alpha = MathConstants::RAIN_ALPHA_MID_FREQ;
        } else {
            k = MathConstants::RAIN_K_HIGH_FREQ_BASE * std::pow(frequencyGHz, 0.5);
            alpha = MathConstants::RAIN_ALPHA_HIGH_FREQ;
        }
        
        return k * std::pow(rainRate, alpha) * distance;
    }
    
    double calculateFoliageLoss(double frequency, double distance, double foliageDensity) {
        // 基于ITU-R P.833建议的植被损耗模型
        double frequencyGHz = frequency / 1000.0;
        
        // 植被损耗系数
        double specificLoss = MathConstants::FOLIAGE_LOSS_BASE_COEFF * std::pow(frequencyGHz, MathConstants::FOLIAGE_FREQ_EXPONENT) * foliageDensity;
        
        return specificLoss * distance;
    }
    
    double calculateUrbanLoss(double frequency, double distance, double buildingDensity) {
        // 基于Okumura-Hata模型的城市环境损耗
        double frequencyMHz = frequency;
        
        // 城市环境修正因子
        double urbanCorrection = 0.0;
        if (frequencyMHz > 150.0) {
            urbanCorrection = MathConstants::URBAN_CORRECTION_COEFF * std::pow(std::log10(MathConstants::URBAN_ANTENNA_HEIGHT_COEFF * MathConstants::URBAN_ANTENNA_HEIGHT), 2) + MathConstants::URBAN_CORRECTION_OFFSET; // 假设30m天线高度
        }
        
        // 建筑密度影响
        double densityFactor = buildingDensity * MathConstants::URBAN_DENSITY_FACTOR; // 简化的密度因子
        
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
        double tValue = MathConstants::CONFIDENCE_T_VALUE_95; // 95%置信度的近似值
        
        if (confidence > 0.99) tValue = MathConstants::CONFIDENCE_T_VALUE_99;
        else if (confidence > 0.95) tValue = MathConstants::CONFIDENCE_T_VALUE_95;
        else if (confidence > 0.90) tValue = MathConstants::CONFIDENCE_T_VALUE_90;
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