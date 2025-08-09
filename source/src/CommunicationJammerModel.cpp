#include "../header/CommunicationJammerModel.h"
#include "../header/CommunicationJammerParameterConfig.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

// 构造函数实现
CommunicationJammerModel::CommunicationJammerModel(
    JammerType type, JammerStrategy strat, double power,
    double frequency, double bandwidth, double range)
    : jammerType(type), strategy(strat), propagationLoss(0.0), atmosphericLoss(0.0),
      jammerToSignalRatio(0.0), effectiveJammerPower(0.0),
      pulseWidth(1.0), pulseRepetitionRate(1000.0), dutyCycle(0.5),
      sweepRate(1.0), sweepRange(10.0),
      targetFrequency(10000.0), targetBandwidth(25.0), targetPower(-80.0), targetDistance(10.0) {
    
    if (!setJammerPower(power)) {
        throw std::invalid_argument("干扰功率超出有效范围(-50至50dBm)");
    }
    if (!setJammerFrequency(frequency)) {
        throw std::invalid_argument("干扰频率超出有效范围(1至30000000kHz)");
    }
    if (!setJammerBandwidth(bandwidth)) {
        throw std::invalid_argument("干扰带宽超出有效范围(0.1至10000kHz)");
    }
    if (!setJammerRange(range)) {
        throw std::invalid_argument("干扰作用距离超出有效范围(0.1至1000km)");
    }
}

// 参数校验方法实现 - 使用统一的参数配置类
bool CommunicationJammerModel::isPowerValid(double power_dBm) const {
    return CommunicationJammerParameterConfig::isJammerPowerValid(power_dBm);
}

bool CommunicationJammerModel::isFrequencyValid(double freq_kHz) const {
    return CommunicationJammerParameterConfig::isFrequencyValid(freq_kHz);
}

bool CommunicationJammerModel::isBandwidthValid(double bandwidth_kHz) const {
    return CommunicationJammerParameterConfig::isBandwidthValid(bandwidth_kHz);
}

bool CommunicationJammerModel::isRangeValid(double range_km) const {
    return CommunicationJammerParameterConfig::isRangeValid(range_km);
}

bool CommunicationJammerModel::isDutyCycleValid(double duty) const {
    return CommunicationJammerParameterConfig::isDutyCycleValid(duty);
}

// 干扰机参数设置方法实现
void CommunicationJammerModel::setJammerType(JammerType type) {
    jammerType = type;
}

void CommunicationJammerModel::setJammerStrategy(JammerStrategy strat) {
    strategy = strat;
}

bool CommunicationJammerModel::setJammerPower(double power_dBm) {
    if (!isPowerValid(power_dBm)) {
        return false;
    }
    jammerPower = power_dBm;
    return true;
}

bool CommunicationJammerModel::setJammerFrequency(double freq_kHz) {
    if (!isFrequencyValid(freq_kHz)) {
        return false;
    }
    jammerFrequency = freq_kHz;
    return true;
}

bool CommunicationJammerModel::setJammerBandwidth(double bandwidth_kHz) {
    if (!isBandwidthValid(bandwidth_kHz)) {
        return false;
    }
    jammerBandwidth = bandwidth_kHz;
    return true;
}

bool CommunicationJammerModel::setJammerRange(double range_km) {
    if (!isRangeValid(range_km)) {
        return false;
    }
    jammerRange = range_km;
    return true;
}

// 目标信号参数设置方法实现
bool CommunicationJammerModel::setTargetFrequency(double freq_kHz) {
    if (!isFrequencyValid(freq_kHz)) {
        return false;
    }
    targetFrequency = freq_kHz;
    return true;
}

bool CommunicationJammerModel::setTargetBandwidth(double bandwidth_kHz) {
    if (!isBandwidthValid(bandwidth_kHz)) {
        return false;
    }
    targetBandwidth = bandwidth_kHz;
    return true;
}

bool CommunicationJammerModel::setTargetPower(double power_dBm) {
    if (!CommunicationJammerParameterConfig::isTargetPowerValid(power_dBm)) {
        return false;
    }
    targetPower = power_dBm;
    return true;
}

bool CommunicationJammerModel::setTargetDistance(double distance_km) {
    if (!isRangeValid(distance_km)) {
        return false;
    }
    targetDistance = distance_km;
    return true;
}

// 脉冲干扰参数设置实现
bool CommunicationJammerModel::setPulseWidth(double width_ms) {
    if (!CommunicationJammerParameterConfig::isPulseWidthValid(width_ms)) {
        return false;
    }
    pulseWidth = width_ms;
    return true;
}

bool CommunicationJammerModel::setPulseRepetitionRate(double rate_Hz) {
    if (!CommunicationJammerParameterConfig::isPulseRepetitionRateValid(rate_Hz)) {
        return false;
    }
    pulseRepetitionRate = rate_Hz;
    dutyCycle = (pulseWidth / 1000.0) * rate_Hz; // 自动计算占空比
    if (dutyCycle > 1.0) dutyCycle = 1.0;
    return true;
}

bool CommunicationJammerModel::setDutyCycle(double duty) {
    if (!isDutyCycleValid(duty)) {
        return false;
    }
    dutyCycle = duty;
    return true;
}

// 扫频干扰参数设置实现
bool CommunicationJammerModel::setSweepRate(double rate_MHz_per_s) {
    if (!CommunicationJammerParameterConfig::isSweepRateValid(rate_MHz_per_s)) {
        return false;
    }
    sweepRate = rate_MHz_per_s;
    return true;
}

bool CommunicationJammerModel::setSweepRange(double range_MHz) {
    if (!CommunicationJammerParameterConfig::isSweepRangeValid(range_MHz)) {
        return false;
    }
    sweepRange = range_MHz;
    return true;
}

// 环境参数设置实现
bool CommunicationJammerModel::setPropagationLoss(double loss_dB) {
    if (!CommunicationJammerParameterConfig::isPropagationLossValid(loss_dB)) {
        return false;
    }
    propagationLoss = loss_dB;
    return true;
}

bool CommunicationJammerModel::setAtmosphericLoss(double loss_dB) {
    if (!CommunicationJammerParameterConfig::isAtmosphericLossValid(loss_dB)) {
        return false;
    }
    atmosphericLoss = loss_dB;
    return true;
}

// 参数获取方法实现
JammerType CommunicationJammerModel::getJammerType() const { return jammerType; }
JammerStrategy CommunicationJammerModel::getJammerStrategy() const { return strategy; }
double CommunicationJammerModel::getJammerPower() const { return jammerPower; }
double CommunicationJammerModel::getJammerFrequency() const { return jammerFrequency; }
double CommunicationJammerModel::getJammerBandwidth() const { return jammerBandwidth; }
double CommunicationJammerModel::getJammerRange() const { return jammerRange; }
double CommunicationJammerModel::getTargetFrequency() const { return targetFrequency; }
double CommunicationJammerModel::getTargetBandwidth() const { return targetBandwidth; }
double CommunicationJammerModel::getTargetPower() const { return targetPower; }
double CommunicationJammerModel::getTargetDistance() const { return targetDistance; }
double CommunicationJammerModel::getPulseWidth() const { return pulseWidth; }
double CommunicationJammerModel::getPulseRepetitionRate() const { return pulseRepetitionRate; }
double CommunicationJammerModel::getDutyCycle() const { return dutyCycle; }
double CommunicationJammerModel::getSweepRate() const { return sweepRate; }
double CommunicationJammerModel::getSweepRange() const { return sweepRange; }

// 内部计算方法实现
double CommunicationJammerModel::calculatePropagationLoss(double distance_km, double freq_kHz) const {
    // 自由空间传播损耗公式: L = 20*log10(d) + 20*log10(f) + 32.45
    // d: 距离(km), f: 频率(MHz)
    double freq_MHz = freq_kHz / 1000.0;
    return 20.0 * log10(distance_km) + 20.0 * log10(freq_MHz) + 32.45;
}

double CommunicationJammerModel::calculateFrequencyOverlap() const {
    // 计算干扰频率与目标频率的重叠度
    double jammer_low = jammerFrequency - jammerBandwidth / 2.0;
    double jammer_high = jammerFrequency + jammerBandwidth / 2.0;
    double target_low = targetFrequency - targetBandwidth / 2.0;
    double target_high = targetFrequency + targetBandwidth / 2.0;
    
    double overlap_low = std::max(jammer_low, target_low);
    double overlap_high = std::min(jammer_high, target_high);
    
    if (overlap_high <= overlap_low) {
        return 0.0; // 无重叠
    }
    
    double overlap_bandwidth = overlap_high - overlap_low;
    return overlap_bandwidth / targetBandwidth; // 重叠比例
}

double CommunicationJammerModel::calculateJammerEffectivePower() const {
    // 计算到达目标的有效干扰功率
    double path_loss = calculatePropagationLoss(targetDistance, jammerFrequency);
    return jammerPower - path_loss - atmosphericLoss;
}

// 核心计算方法实现
double CommunicationJammerModel::calculateJammerToSignalRatio() const {
    double effective_jammer_power = calculateJammerEffectivePower();
    return effective_jammer_power - targetPower;
}

double CommunicationJammerModel::calculateJammerEffectiveness() const {
    // 根据干扰类型计算干扰有效性
    switch (jammerType) {
        case JammerType::GAUSSIAN_NOISE:
            return calculateGaussianNoiseEffect();
        case JammerType::NARROWBAND:
            return calculateNarrowbandEffect();
        case JammerType::SWEEP_FREQUENCY:
            return calculateSweepFrequencyEffect();
        case JammerType::PULSE:
            return calculatePulseJammerEffect();
        case JammerType::BARRAGE:
            return calculateBarrageJammerEffect();
        case JammerType::SPOT:
            return calculateSpotJammerEffect();
        default:
            return 0.0;
    }
}

double CommunicationJammerModel::calculateCommunicationDegradation() const {
    double js_ratio = calculateJammerToSignalRatio();
    double effectiveness = calculateJammerEffectiveness();
    
    // 通信性能下降率基于干信比和干扰有效性
    if (js_ratio < 0.0) {
        return 0.0; // 干扰功率不足
    }
    
    double degradation = effectiveness * (1.0 - 1.0 / (1.0 + pow(10.0, js_ratio / 10.0)));
    return std::min(degradation, 1.0);
}

JammerEffectLevel CommunicationJammerModel::evaluateJammerEffect() const {
    double degradation = calculateCommunicationDegradation();
    
    if (degradation < 0.1) {
        return JammerEffectLevel::NO_EFFECT;
    } else if (degradation < 0.3) {
        return JammerEffectLevel::SLIGHT;
    } else if (degradation < 0.6) {
        return JammerEffectLevel::MODERATE;
    } else if (degradation < 0.9) {
        return JammerEffectLevel::SEVERE;
    } else {
        return JammerEffectLevel::COMPLETE_DENIAL;
    }
}

// 特定干扰类型的效果计算实现
double CommunicationJammerModel::calculateGaussianNoiseEffect() const {
    // 高斯白噪声干扰效果主要取决于功率和频率重叠
    double frequency_overlap = calculateFrequencyOverlap();
    double power_factor = std::min(1.0, pow(10.0, calculateJammerToSignalRatio() / 20.0));
    return frequency_overlap * power_factor;
}

double CommunicationJammerModel::calculateNarrowbandEffect() const {
    // 窄带干扰效果取决于频率精确度和功率
    double freq_diff = abs(jammerFrequency - targetFrequency);
    double freq_factor = exp(-freq_diff / targetBandwidth);
    double power_factor = std::min(1.0, pow(10.0, calculateJammerToSignalRatio() / 10.0));
    return freq_factor * power_factor;
}

double CommunicationJammerModel::calculateSweepFrequencyEffect() const {
    // 扫频干扰效果取决于扫频范围覆盖和驻留时间
    double coverage = std::min(1.0, sweepRange * 1000.0 / targetBandwidth);
    double dwell_time = sweepRange / sweepRate; // 扫频周期
    double time_factor = std::min(1.0, targetBandwidth / (sweepRange * 1000.0));
    double power_factor = std::min(1.0, pow(10.0, calculateJammerToSignalRatio() / 15.0));
    return coverage * time_factor * power_factor;
}

double CommunicationJammerModel::calculatePulseJammerEffect() const {
    // 脉冲干扰效果取决于占空比和脉冲功率
    double frequency_overlap = calculateFrequencyOverlap();
    double pulse_power = jammerPower + 10.0 * log10(1.0 / dutyCycle); // 脉冲峰值功率
    double effective_power = pulse_power - calculatePropagationLoss(targetDistance, jammerFrequency);
    double power_factor = std::min(1.0, pow(10.0, (effective_power - targetPower) / 10.0));
    return frequency_overlap * dutyCycle * power_factor;
}

double CommunicationJammerModel::calculateBarrageJammerEffect() const {
    // 阻塞干扰效果主要取决于带宽覆盖和功率
    double bandwidth_ratio = jammerBandwidth / targetBandwidth;
    double coverage = std::min(1.0, bandwidth_ratio);
    double power_factor = std::min(1.0, pow(10.0, calculateJammerToSignalRatio() / 12.0));
    return coverage * power_factor;
}

double CommunicationJammerModel::calculateSpotJammerEffect() const {
    // 点频干扰效果类似窄带干扰但更精确
    return std::min(1.0, calculateNarrowbandEffect() * 1.2); // 点频干扰效果稍好，但不超过1.0
}
/// @brief  计算频谱干扰效果
/// @details 频谱干扰效果 = 干扰功率 - 目标信号功率 - 大气损耗
/// @return 频谱干扰效果(dB)
double CommunicationJammerModel::calculateJammerCoverage() const {
    // 简化的覆盖范围计算，基于干扰功率和最小有效干信比
    double min_js_ratio = 10.0; // 最小有效干信比10dB
    double required_power_at_target = targetPower + min_js_ratio;
    double max_path_loss = jammerPower - required_power_at_target - atmosphericLoss;
    
    // 根据自由空间传播损耗反推最大距离
    double freq_MHz = jammerFrequency / 1000.0;
    double max_distance = pow(10.0, (max_path_loss - 32.45 - 20.0 * log10(freq_MHz)) / 20.0);
    
    return 3.14159 * max_distance * max_distance; // 圆形覆盖面积
}

bool CommunicationJammerModel::isTargetInJammerRange() const {
    return targetDistance <= jammerRange;
}

/// @brief 计算所需干扰功率
/// @details 所需干扰功率 = 目标信号功率 +  desired_js_ratio + 路径损耗 + 大气损耗
/// @return 所需干扰功率(dBm)
double CommunicationJammerModel::calculateRequiredJammerPower(double desired_js_ratio) const {
    double path_loss = calculatePropagationLoss(targetDistance, jammerFrequency);
    return targetPower + desired_js_ratio + path_loss + atmosphericLoss;
}

/// @brief 计算最优干扰频率
/// @details 最优干扰频率通常是目标信号的中心频率
/// @return 最优干扰频率(kHz)
double CommunicationJammerModel::calculateOptimalJammerFrequency() const {
    // 最优干扰频率通常是目标信号的中心频率
    return targetFrequency;
}

/// @brief 计算多干扰机协同效果
/// @details 多干扰机协同效果 = 所有干扰机有效功率的线性组合
/// @return 多干扰机协同效果(dB)
double CommunicationJammerModel::calculateCombinedJammerEffect(
    const std::vector<CommunicationJammerModel>& jammers) const {
    
    double total_jammer_power = 0.0;
    
    for (const auto& jammer : jammers) {
        double jammer_power_linear = pow(10.0, jammer.calculateJammerEffectivePower() / 10.0);
        double effectiveness = jammer.calculateJammerEffectiveness();
        total_jammer_power += jammer_power_linear * effectiveness;
    }
    
    double total_jammer_power_dBm = 10.0 * log10(total_jammer_power);
    double combined_js_ratio = total_jammer_power_dBm - targetPower;
    
    return std::min(1.0, 1.0 - 1.0 / (1.0 + pow(10.0, combined_js_ratio / 10.0)));
}

// 信息输出方法实现
std::string CommunicationJammerModel::getParameterInfo() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    
    oss << "=== 通信干扰模型参数 ===\n";
    
    oss << "干扰类型: ";
    switch (jammerType) {
        case JammerType::GAUSSIAN_NOISE: oss << "高斯白噪声"; break;
        case JammerType::NARROWBAND: oss << "窄带干扰"; break;
        case JammerType::SWEEP_FREQUENCY: oss << "扫频干扰"; break;
        case JammerType::PULSE: oss << "脉冲干扰"; break;
        case JammerType::BARRAGE: oss << "阻塞干扰"; break;
        case JammerType::SPOT: oss << "点频干扰"; break;
    }
    oss << "\n";
    
    oss << "干扰策略: ";
    switch (strategy) {
        case JammerStrategy::CONTINUOUS: oss << "连续干扰"; break;
        case JammerStrategy::INTERMITTENT: oss << "间歇干扰"; break;
        case JammerStrategy::ADAPTIVE: oss << "自适应干扰"; break;
        case JammerStrategy::RANDOM: oss << "随机干扰"; break;
    }
    oss << "\n";
    
    oss << "干扰功率: " << jammerPower << " dBm\n";
    oss << "干扰频率: " << jammerFrequency << " kHz\n";
    oss << "干扰带宽: " << jammerBandwidth << " kHz\n";
    oss << "作用距离: " << jammerRange << " km\n";
    oss << "目标频率: " << targetFrequency << " kHz\n";
    oss << "目标带宽: " << targetBandwidth << " kHz\n";
    oss << "目标功率: " << targetPower << " dBm\n";
    oss << "目标距离: " << targetDistance << " km\n";
    
    if (jammerType == JammerType::PULSE) {
        oss << "脉冲宽度: " << pulseWidth << " ms\n";
        oss << "重复频率: " << pulseRepetitionRate << " Hz\n";
        oss << "占空比: " << dutyCycle << "\n";
    }
    
    if (jammerType == JammerType::SWEEP_FREQUENCY) {
        oss << "扫频速率: " << sweepRate << " MHz/s\n";
        oss << "扫频范围: " << sweepRange << " MHz\n";
    }
    
    return oss.str();
}

std::string CommunicationJammerModel::getJammerEffectInfo() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    
    oss << "=== 干扰效果信息 ===\n";
    oss << "干信比: " << calculateJammerToSignalRatio() << " dB\n";
    oss << "干扰有效性: " << calculateJammerEffectiveness() << "\n";
    oss << "通信性能下降: " << calculateCommunicationDegradation() * 100.0 << "%\n";
    oss << "频率重叠度: " << calculateFrequencyOverlap() * 100.0 << "%\n";
    oss << "有效干扰功率: " << calculateJammerEffectivePower() << " dBm\n";
    oss << "干扰覆盖范围: " << calculateJammerCoverage() << " km²\n";
    oss << "目标在范围内: " << (isTargetInJammerRange() ? "是" : "否") << "\n";
    
    oss << "干扰效果等级: ";
    switch (evaluateJammerEffect()) {
        case JammerEffectLevel::NO_EFFECT: oss << "无效果"; break;
        case JammerEffectLevel::SLIGHT: oss << "轻微影响"; break;
        case JammerEffectLevel::MODERATE: oss << "中等影响"; break;
        case JammerEffectLevel::SEVERE: oss << "严重影响"; break;
        case JammerEffectLevel::COMPLETE_DENIAL: oss << "完全拒止"; break;
    }
    oss << "\n";
    
    return oss.str();
}

std::string CommunicationJammerModel::getRecommendationInfo() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    
    oss << "=== 干扰优化建议 ===\n";
    oss << "最优干扰频率: " << calculateOptimalJammerFrequency() << " kHz\n";
    oss << "建议干扰功率(10dB干信比): " << calculateRequiredJammerPower(10.0) << " dBm\n";
    oss << "建议干扰功率(20dB干信比): " << calculateRequiredJammerPower(20.0) << " dBm\n";
    
    double current_effect = calculateJammerEffectiveness();
    if (current_effect < 0.3) {
        oss << "建议: 增加干扰功率或调整频率以提高干扰效果\n";
    } else if (current_effect > 0.8) {
        oss << "建议: 当前干扰效果良好，可考虑降低功率以节省资源\n";
    } else {
        oss << "建议: 当前干扰效果适中，可根据需要微调参数\n";
    }
    
    return oss.str();
}