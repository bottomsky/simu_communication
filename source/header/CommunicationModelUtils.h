#ifndef COMMUNICATION_MODEL_UTILS_H
#define COMMUNICATION_MODEL_UTILS_H

#include <vector>
#include <utility>
#include "EnvironmentLossConfigManager.h"

// 前向声明
enum class CommunicationQuality;

/**
 * @brief 通信模型工具类
 * 
 * 提供各种便利函数，包括快速计算、单位转换、环境建模辅助函数和统计分析函数
 */
namespace CommunicationModelUtils {
    
    // ==================== 快速计算函数 ====================
    
    /**
     * @brief 快速计算所需功率
     * @param frequency 频率 (MHz)
     * @param range 通信距离 (km)
     * @param env 环境类型
     * @return 所需发射功率 (dBm)
     */
    double quickCalculatePower(double frequency, double range, EnvironmentType env);
    
    /**
     * @brief 快速评估通信质量
     * @param snr 信噪比 (dB)
     * @param ber 误码率
     * @return 通信质量等级
     */
    CommunicationQuality quickAssessQuality(double snr, double ber);
    
    // ==================== 单位转换函数 ====================
    
    /**
     * @brief dBm 转换为瓦特
     * @param dBm dBm 值
     * @return 瓦特值
     */
    double dBmToWatts(double dBm);
    
    /**
     * @brief 瓦特转换为 dBm
     * @param watts 瓦特值
     * @return dBm 值
     */
    double wattsTodBm(double watts);
    
    /**
     * @brief dB 转换为线性值
     * @param dB dB 值
     * @return 线性值
     */
    double dBToLinear(double dB);
    
    /**
     * @brief 线性值转换为 dB
     * @param linear 线性值
     * @return dB 值
     */
    double linearTodB(double linear);
    
    /**
     * @brief 频率转换为波长
     * @param frequency 频率 (MHz)
     * @return 波长 (m)
     */
    double frequencyToWavelength(double frequency);
    
    /**
     * @brief 波长转换为频率
     * @param wavelength 波长 (m)
     * @return 频率 (MHz)
     */
    double wavelengthToFrequency(double wavelength);
    
    // ==================== 环境建模辅助函数 ====================
    
    /**
     * @brief 计算大气损耗
     * @param frequency 频率 (MHz)
     * @param distance 距离 (km)
     * @param humidity 湿度 (%)
     * @return 大气损耗 (dB)
     */
    double calculateAtmosphericLoss(double frequency, double distance, double humidity);
    
    /**
     * @brief 计算雨衰损耗
     * @param frequency 频率 (MHz)
     * @param distance 距离 (km)
     * @param rainRate 降雨率 (mm/h)
     * @return 雨衰损耗 (dB)
     */
    double calculateRainLoss(double frequency, double distance, double rainRate);
    
    /**
     * @brief 计算植被损耗
     * @param frequency 频率 (MHz)
     * @param distance 距离 (km)
     * @param foliageDensity 植被密度 (0-1)
     * @return 植被损耗 (dB)
     */
    double calculateFoliageLoss(double frequency, double distance, double foliageDensity);
    
    /**
     * @brief 计算城市环境损耗
     * @param frequency 频率 (MHz)
     * @param distance 距离 (km)
     * @param buildingDensity 建筑密度 (0-1)
     * @return 城市环境损耗 (dB)
     */
    double calculateUrbanLoss(double frequency, double distance, double buildingDensity);
    
    // ==================== 统计分析函数 ====================
    
    /**
     * @brief 计算平均值
     * @param values 数值向量
     * @return 平均值
     */
    double calculateMean(const std::vector<double>& values);
    
    /**
     * @brief 计算标准差
     * @param values 数值向量
     * @return 标准差
     */
    double calculateStandardDeviation(const std::vector<double>& values);
    
    /**
     * @brief 计算百分位数
     * @param values 数值向量
     * @param percentile 百分位数 (0-100)
     * @return 百分位数值
     */
    double calculatePercentile(const std::vector<double>& values, double percentile);
    
    /**
     * @brief 计算置信区间
     * @param values 数值向量
     * @param confidence 置信度 (0-1)
     * @return 置信区间 (下界, 上界)
     */
    std::pair<double, double> calculateConfidenceInterval(const std::vector<double>& values, double confidence);
    
    // ==================== 兼容性函数 ====================
    
    /**
     * @brief 评估通信质量（兼容性函数）
     * @param snr 信噪比 (dB)
     * @param ber 误码率
     * @return 通信质量等级
     */
    CommunicationQuality evaluateQuality(double snr, double ber);
    
    /**
     * @brief 快速评估通信质量（兼容性函数）
     * @param snr 信噪比 (dB)
     * @param ber 误码率
     * @return 通信质量等级
     */
    CommunicationQuality quickEvaluateQuality(double snr, double ber);
}

#endif // COMMUNICATION_MODEL_UTILS_H