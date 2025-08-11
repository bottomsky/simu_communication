#ifndef COMMUNICATION_JAMMER_MODEL_H
#define COMMUNICATION_JAMMER_MODEL_H

#include <string>
#include <stdexcept>
#include <vector>
#include <cmath>
#include "CommunicationDistanceModel.h"

// 干扰类型枚举
enum class JammerType {
    GAUSSIAN_NOISE,    // 高斯白噪声干扰
    NARROWBAND,        // 窄带干扰
    SWEEP_FREQUENCY,   // 扫频干扰
    PULSE,            // 脉冲干扰
    BARRAGE,          // 阻塞干扰
    SPOT             // 点频干扰
};

// 干扰策略枚举
enum class JammerStrategy {
    CONTINUOUS,       // 连续干扰
    INTERMITTENT,     // 间歇干扰
    ADAPTIVE,         // 自适应干扰
    RANDOM           // 随机干扰
};

// 干扰效果等级枚举
enum class JammerEffectLevel {
    NO_EFFECT,        // 无效果
    SLIGHT,           // 轻微影响
    MODERATE,         // 中等影响
    SEVERE,           // 严重影响
    COMPLETE_DENIAL   // 完全拒止
};

class CommunicationJammerModel {
private:
    // 干扰机基本参数
    JammerType jammerType;        // 干扰类型
    JammerStrategy strategy;      // 干扰策略
    double jammerTransmitPower_dBm;          // 干扰功率(dBm)
    double jammerFrequency_kHz;      // 干扰中心频率(kHz)
    double jammerBandwidth;      // 干扰带宽(kHz)
    double jammerRange;          // 干扰作用距离(km)
    
    // 目标信号参数
    double targetFrequency;      // 目标信号频率(kHz)
    double targetBandwidth;      // 目标信号带宽(kHz)
    double targetSignalTransmitPower_dBm;          // 目标信号功率(dBm)
    double jammerToTargetDistance; // 目标距离(km)
    
    // 环境参数
    double propagationLoss;      // 传播损耗(dB)
    double atmosphericLoss;      // 大气损耗(dB)
    
    // 干扰效果参数
    double jammerToSignalRatio;  // 干信比(J/S)
    double effectiveJammerPower; // 有效干扰功率(dBm)
    
    // 脉冲干扰特有参数
    double pulseWidth;           // 脉冲宽度(ms)
    double pulseRepetitionRate;  // 脉冲重复频率(Hz)
    double dutyCycle;           // 占空比(0-1)
    
    // 扫频干扰特有参数
    double sweepRate;           // 扫频速率(MHz/s)
    double sweepRange;          // 扫频范围(MHz)
    
    // 参数校验方法
    bool isPowerValid(double power_dBm) const;
    bool isFrequencyValid(double freq_kHz) const;
    bool isBandwidthValid(double bandwidth_kHz) const;
    bool isRangeValid(double range_km) const;
    bool isDutyCycleValid(double duty) const;
    
    // 内部计算方法
    double calculatePropagationLoss(double distance_km, double freq_kHz) const;
    double calculateFrequencyOverlap() const;
    double calculateJammerEffectivePower() const;

public:
    // 构造函数，带默认参数
    CommunicationJammerModel(
        JammerType type = JammerType::GAUSSIAN_NOISE,
        JammerStrategy strat = JammerStrategy::CONTINUOUS,
        double power = 30.0,           // 默认干扰功率30dBm
        double frequency = 10000.0,    // 默认频率10MHz
        double bandwidth = 100.0,      // 默认带宽100kHz
        double range = 50.0           // 默认作用距离50km
    );

    // 干扰机参数设置方法
    void setJammerType(JammerType type);
    void setJammerStrategy(JammerStrategy strat);
    bool setJammerPower(double power_dBm);
    bool setJammerFrequency(double freq_kHz);
    bool setJammerBandwidth(double bandwidth_kHz);
    bool setJammerRange(double range_km);
    
    // 目标信号参数设置方法
    bool setTargetFrequency(double freq_kHz);
    bool setTargetBandwidth(double bandwidth_kHz);
    bool setTargetPower(double power_dBm);
    bool setTargetDistance(double distance_km);
    
    // 脉冲干扰参数设置
    bool setPulseWidth(double width_ms);
    bool setPulseRepetitionRate(double rate_Hz);
    bool setDutyCycle(double duty);
    
    // 扫频干扰参数设置
    bool setSweepRate(double rate_MHz_per_s);
    bool setSweepRange(double range_MHz);
    
    // 环境参数设置
    bool setPropagationLoss(double loss_dB);
    bool setAtmosphericLoss(double loss_dB);

    // 参数获取方法
    JammerType getJammerType() const;
    JammerStrategy getJammerStrategy() const;
    double getJammerPower() const;
    double getJammerFrequency() const;
    double getJammerBandwidth() const;
    double getJammerRange() const;
    double getTargetFrequency() const;
    double getTargetBandwidth() const;
    double getTargetPower() const;
    double getTargetDistance() const;
    double getPulseWidth() const;
    double getPulseRepetitionRate() const;
    double getDutyCycle() const;
    double getSweepRate() const;
    double getSweepRange() const;



    // 核心计算方法
    double calculatePulsePeakPower(double averagePower, double dutyCycle) const;    //计算脉冲干扰的峰值功率
    double calculateJammerToSignalRatio() const;    // 计算干扰干信比(dB)
    double calculateJammerEffectiveness() const;     // 计算干扰有效性(0-1)
    double calculateCommunicationDegradation() const; // 计算通信性能下降率(0-1)
    JammerEffectLevel evaluateJammerEffect() const;  // 评估干扰效果等级
    /// @brief 计算干扰干信比 (J/S)
	/// @details 干信比(dB) = 干扰功率(dBm) - 信号功率(dBm)
	///          其中：
	///          - 干扰功率 = 干扰源发射功率 - 干扰路径损耗 - 干扰频率对应的大气损耗
	///          - 信号功率 = 信号源发射功率 - 信号路径损耗 - 信号频率对应的大气损耗
	/// @return 干信比(dB)，正值表示干扰强于信号，负值表示信号强于干扰
	double CommunicationJammerModel::calculateJammerToSignalRatio(
		double signalToTargetDistance_km,
		double targetSignalFrequency_kHz) const;

    // 特定干扰类型的效果计算
    double calculateGaussianNoiseEffect() const;     // 高斯噪声干扰效果
    double calculateNarrowbandEffect() const;        // 窄带干扰效果
    double calculateSweepFrequencyEffect() const;    // 扫频干扰效果
    double calculatePulseJammerEffect() const;       // 脉冲干扰效果
    double calculateBarrageJammerEffect() const;     // 阻塞干扰效果
    double calculateSpotJammerEffect() const;        // 点频干扰效果
	double CommunicationJammerModel::getAtmosphericLoss(double frequency_kHz) const;
    
    // 干扰覆盖范围计算
    double calculateJammingRange() const;            // 计算干扰有效覆盖范围(km)
    double calculateJammingArea() const;             // 计算干扰覆盖面积(m²)
    double calculateJammerCoverage() const;          // 计算干扰覆盖范围(km²)
    bool isTargetInJammerRange() const;             // 判断目标是否在干扰范围内
    
    // 干扰功率需求计算
    double calculateRequiredJammerPower(double desired_js_ratio) const; // 计算所需干扰功率
    double calculateOptimalJammerFrequency() const;  // 计算最优干扰频率
    
    // 多干扰机协同效果
    double calculateCombinedJammerEffect(const std::vector<CommunicationJammerModel>& jammers) const;
    
    // 获取信息字符串
    std::string getParameterInfo() const;
    std::string getJammerEffectInfo() const;
    std::string getRecommendationInfo() const;
};

#endif // COMMUNICATION_JAMMER_MODEL_H