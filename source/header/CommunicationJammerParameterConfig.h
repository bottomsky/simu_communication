#ifndef COMMUNICATION_JAMMER_PARAMETER_CONFIG_H
#define COMMUNICATION_JAMMER_PARAMETER_CONFIG_H

#include <string>

/**
 * @file CommunicationJammerParameterConfig.h
 * @brief 通信干扰模型参数配置类
 * 
 * 该类提供统一的参数校验和配置管理功能，用于替代硬编码的参数范围检查。
 * 所有参数范围和校验逻辑集中在此类中，便于维护和扩展。
 */
class CommunicationJammerParameterConfig {
public:
    // 干扰功率参数配置
    static constexpr double MIN_JAMMER_POWER = -50.0;    // 最小干扰功率(dBm)
    static constexpr double MAX_JAMMER_POWER = 50.0;     // 最大干扰功率(dBm)
    
    // 频率参数配置
    static constexpr double MIN_FREQUENCY = 1.0;         // 最小频率(kHz)
    static constexpr double MAX_FREQUENCY = 30000000.0;  // 最大频率(kHz) - 30GHz
    
    // 带宽参数配置
    static constexpr double MIN_BANDWIDTH = 0.1;         // 最小带宽(kHz)
    static constexpr double MAX_BANDWIDTH = 10000.0;     // 最大带宽(kHz)
    
    // 距离参数配置
    static constexpr double MIN_RANGE = 0.1;             // 最小作用距离(km)
    static constexpr double MAX_RANGE = 1000.0;          // 最大作用距离(km)
    
    // 目标功率参数配置
    static constexpr double MIN_TARGET_POWER = -150.0;   // 最小目标功率(dBm)
    static constexpr double MAX_TARGET_POWER = 50.0;     // 最大目标功率(dBm)
    
    // 脉冲参数配置
    static constexpr double MIN_PULSE_WIDTH = 0.001;     // 最小脉冲宽度(ms)
    static constexpr double MAX_PULSE_WIDTH = 1000.0;    // 最大脉冲宽度(ms)
    static constexpr double MIN_PULSE_RATE = 0.1;        // 最小脉冲重复频率(Hz)
    static constexpr double MAX_PULSE_RATE = 100000.0;   // 最大脉冲重复频率(Hz)
    static constexpr double MIN_DUTY_CYCLE = 0.0;        // 最小占空比
    static constexpr double MAX_DUTY_CYCLE = 1.0;        // 最大占空比
    
    // 扫频参数配置
    static constexpr double MIN_SWEEP_RATE = 0.001;      // 最小扫频速率(MHz/s)
    static constexpr double MAX_SWEEP_RATE = 1000.0;     // 最大扫频速率(MHz/s)
    static constexpr double MIN_SWEEP_RANGE = 0.001;     // 最小扫频范围(MHz)
    static constexpr double MAX_SWEEP_RANGE = 10000.0;   // 最大扫频范围(MHz)
    
    // 环境损耗参数配置
    static constexpr double MIN_PROPAGATION_LOSS = 0.0;  // 最小传播损耗(dB)
    static constexpr double MAX_PROPAGATION_LOSS = 200.0; // 最大传播损耗(dB)
    static constexpr double MIN_ATMOSPHERIC_LOSS = 0.0;  // 最小大气损耗(dB)
    static constexpr double MAX_ATMOSPHERIC_LOSS = 50.0; // 最大大气损耗(dB)

public:
    /**
     * @brief 校验干扰功率是否在有效范围内
     * @param power_dBm 干扰功率(dBm)
     * @return true 如果功率在有效范围内，否则返回false
     */
    static bool isJammerPowerValid(double power_dBm);
    
    /**
     * @brief 校验频率是否在有效范围内
     * @param freq_kHz 频率(kHz)
     * @return true 如果频率在有效范围内，否则返回false
     */
    static bool isFrequencyValid(double freq_kHz);
    
    /**
     * @brief 校验带宽是否在有效范围内
     * @param bandwidth_kHz 带宽(kHz)
     * @return true 如果带宽在有效范围内，否则返回false
     */
    static bool isBandwidthValid(double bandwidth_kHz);
    
    /**
     * @brief 校验作用距离是否在有效范围内
     * @param range_km 作用距离(km)
     * @return true 如果距离在有效范围内，否则返回false
     */
    static bool isRangeValid(double range_km);
    
    /**
     * @brief 校验目标功率是否在有效范围内
     * @param power_dBm 目标功率(dBm)
     * @return true 如果功率在有效范围内，否则返回false
     */
    static bool isTargetPowerValid(double power_dBm);
    
    /**
     * @brief 校验脉冲宽度是否在有效范围内
     * @param width_ms 脉冲宽度(ms)
     * @return true 如果脉冲宽度在有效范围内，否则返回false
     */
    static bool isPulseWidthValid(double width_ms);
    
    /**
     * @brief 校验脉冲重复频率是否在有效范围内
     * @param rate_Hz 脉冲重复频率(Hz)
     * @return true 如果频率在有效范围内，否则返回false
     */
    static bool isPulseRepetitionRateValid(double rate_Hz);
    
    /**
     * @brief 校验占空比是否在有效范围内
     * @param duty 占空比(0-1)
     * @return true 如果占空比在有效范围内，否则返回false
     */
    static bool isDutyCycleValid(double duty);
    
    /**
     * @brief 校验扫频速率是否在有效范围内
     * @param rate_MHz_per_s 扫频速率(MHz/s)
     * @return true 如果扫频速率在有效范围内，否则返回false
     */
    static bool isSweepRateValid(double rate_MHz_per_s);
    
    /**
     * @brief 校验扫频范围是否在有效范围内
     * @param range_MHz 扫频范围(MHz)
     * @return true 如果扫频范围在有效范围内，否则返回false
     */
    static bool isSweepRangeValid(double range_MHz);
    
    /**
     * @brief 校验传播损耗是否在有效范围内
     * @param loss_dB 传播损耗(dB)
     * @return true 如果损耗在有效范围内，否则返回false
     */
    static bool isPropagationLossValid(double loss_dB);
    
    /**
     * @brief 校验大气损耗是否在有效范围内
     * @param loss_dB 大气损耗(dB)
     * @return true 如果损耗在有效范围内，否则返回false
     */
    static bool isAtmosphericLossValid(double loss_dB);
    
    /**
     * @brief 获取参数范围信息字符串
     * @return 包含所有参数范围的格式化字符串
     */
    static std::string getParameterRangeInfo();
};

#endif // COMMUNICATION_JAMMER_PARAMETER_CONFIG_H