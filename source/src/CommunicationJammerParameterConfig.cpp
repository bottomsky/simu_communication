#include "../header/CommunicationJammerParameterConfig.h"
#include <sstream>
#include <iomanip>
#include <string>

// 干扰功率校验实现
bool CommunicationJammerParameterConfig::isJammerPowerValid(double power_dBm) {
    return power_dBm >= MIN_JAMMER_POWER && power_dBm <= MAX_JAMMER_POWER;
}

// 频率校验实现
bool CommunicationJammerParameterConfig::isFrequencyValid(double freq_kHz) {
    return freq_kHz >= MIN_FREQUENCY && freq_kHz <= MAX_FREQUENCY;
}

// 带宽校验实现
bool CommunicationJammerParameterConfig::isBandwidthValid(double bandwidth_kHz) {
    return bandwidth_kHz >= MIN_BANDWIDTH && bandwidth_kHz <= MAX_BANDWIDTH;
}

// 作用距离校验实现
bool CommunicationJammerParameterConfig::isRangeValid(double range_km) {
    return range_km >= MIN_RANGE && range_km <= MAX_RANGE;
}

// 目标功率校验实现
bool CommunicationJammerParameterConfig::isTargetPowerValid(double power_dBm) {
    return power_dBm >= MIN_TARGET_POWER && power_dBm <= MAX_TARGET_POWER;
}

// 脉冲宽度校验实现
bool CommunicationJammerParameterConfig::isPulseWidthValid(double width_ms) {
    return width_ms >= MIN_PULSE_WIDTH && width_ms <= MAX_PULSE_WIDTH;
}

// 脉冲重复频率校验实现
bool CommunicationJammerParameterConfig::isPulseRepetitionRateValid(double rate_Hz) {
    return rate_Hz >= MIN_PULSE_RATE && rate_Hz <= MAX_PULSE_RATE;
}

// 占空比校验实现
bool CommunicationJammerParameterConfig::isDutyCycleValid(double duty) {
    return duty >= MIN_DUTY_CYCLE && duty <= MAX_DUTY_CYCLE;
}

// 扫频速率校验实现
bool CommunicationJammerParameterConfig::isSweepRateValid(double rate_MHz_per_s) {
    return rate_MHz_per_s >= MIN_SWEEP_RATE && rate_MHz_per_s <= MAX_SWEEP_RATE;
}

// 扫频范围校验实现
bool CommunicationJammerParameterConfig::isSweepRangeValid(double range_MHz) {
    return range_MHz >= MIN_SWEEP_RANGE && range_MHz <= MAX_SWEEP_RANGE;
}

// 传播损耗校验实现
bool CommunicationJammerParameterConfig::isPropagationLossValid(double loss_dB) {
    return loss_dB >= MIN_PROPAGATION_LOSS && loss_dB <= MAX_PROPAGATION_LOSS;
}

// 大气损耗校验实现
bool CommunicationJammerParameterConfig::isAtmosphericLossValid(double loss_dB) {
    return loss_dB >= MIN_ATMOSPHERIC_LOSS && loss_dB <= MAX_ATMOSPHERIC_LOSS;
}

// 获取参数范围信息实现
std::string CommunicationJammerParameterConfig::getParameterRangeInfo() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    
    oss << "=== 通信干扰模型参数范围配置 ===\n";
    oss << "干扰功率范围: " << MIN_JAMMER_POWER << " 到 " << MAX_JAMMER_POWER << " dBm\n";
    oss << "频率范围: " << MIN_FREQUENCY << " 到 " << MAX_FREQUENCY << " kHz\n";
    oss << "带宽范围: " << MIN_BANDWIDTH << " 到 " << MAX_BANDWIDTH << " kHz\n";
    oss << "作用距离范围: " << MIN_RANGE << " 到 " << MAX_RANGE << " km\n";
    oss << "目标功率范围: " << MIN_TARGET_POWER << " 到 " << MAX_TARGET_POWER << " dBm\n";
    oss << "脉冲宽度范围: " << MIN_PULSE_WIDTH << " 到 " << MAX_PULSE_WIDTH << " ms\n";
    oss << "脉冲重复频率范围: " << MIN_PULSE_RATE << " 到 " << MAX_PULSE_RATE << " Hz\n";
    oss << "占空比范围: " << MIN_DUTY_CYCLE << " 到 " << MAX_DUTY_CYCLE << "\n";
    oss << "扫频速率范围: " << MIN_SWEEP_RATE << " 到 " << MAX_SWEEP_RATE << " MHz/s\n";
    oss << "扫频范围: " << MIN_SWEEP_RANGE << " 到 " << MAX_SWEEP_RANGE << " MHz\n";
    oss << "传播损耗范围: " << MIN_PROPAGATION_LOSS << " 到 " << MAX_PROPAGATION_LOSS << " dB\n";
    oss << "大气损耗范围: " << MIN_ATMOSPHERIC_LOSS << " 到 " << MAX_ATMOSPHERIC_LOSS << " dB\n";
    
    return oss.str();
}