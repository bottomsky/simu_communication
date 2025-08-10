#include "../header/CommunicationJammerModel.h"
#include "../header/CommunicationJammerParameterConfig.h"
#include "../header/CommunicationDistanceModel.h"
#include "../header/MathConstants.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

// 构造函数实现
CommunicationJammerModel::CommunicationJammerModel(
    JammerType type, JammerStrategy strat, double power,
    double frequency, double bandwidth, double range)
    : jammerType(type), strategy(strat), propagationLoss(0.0), atmosphericLoss(0.0),
      jammerToSignalRatio(0.0), effectiveJammerPower(0.0),
      pulseWidth(MathConstants::DEFAULT_PULSE_WIDTH), pulseRepetitionRate(MathConstants::DEFAULT_PULSE_REPETITION_RATE), dutyCycle(MathConstants::DEFAULT_DUTY_CYCLE),
      sweepRate(MathConstants::DEFAULT_SWEEP_RATE), sweepRange(MathConstants::DEFAULT_SWEEP_RANGE),
      targetFrequency(MathConstants::DEFAULT_FREQUENCY), targetBandwidth(MathConstants::DEFAULT_TARGET_BANDWIDTH), targetPower(MathConstants::DEFAULT_TARGET_POWER), targetDistance(MathConstants::DEFAULT_DISTANCE) {
    
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
    dutyCycle = (pulseWidth / MathConstants::MS_TO_S_CONVERSION) * rate_Hz; // 自动计算占空比
    if (dutyCycle > MathConstants::MAX_DUTY_CYCLE) dutyCycle = MathConstants::MAX_DUTY_CYCLE;
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

/// @brief 计算传播损耗
/// @details 传播损耗 = 20*log10(d) + 20*log10(f) + 32.45
/// @return 传播损耗(dB)
double CommunicationJammerModel::calculatePropagationLoss(double distance_km, double freq_kHz) const {
    // 使用CommunicationDistanceModel的自由空间路径损耗计算方法
    // 将频率从kHz转换为MHz
    double freq_MHz = freq_kHz / 1000.0;
    return CommunicationDistanceModel::calculateFreeSpacePathLoss(distance_km, freq_MHz);
}

/// @brief 计算频率重叠度
/// @details 计算干扰频率与目标频率的重叠度
/// @return 频率重叠度(0-1)
double CommunicationJammerModel::calculateFrequencyOverlap() const {
    // 计算干扰频率与目标频率的重叠度
    double jammer_low = jammerFrequency - jammerBandwidth / MathConstants::BANDWIDTH_HALF_DIVISOR;
    double jammer_high = jammerFrequency + jammerBandwidth / MathConstants::BANDWIDTH_HALF_DIVISOR;
    double target_low = targetFrequency - targetBandwidth / MathConstants::BANDWIDTH_HALF_DIVISOR;
    double target_high = targetFrequency + targetBandwidth / MathConstants::BANDWIDTH_HALF_DIVISOR;
    
    double overlap_low = std::max(jammer_low, target_low);
    double overlap_high = std::min(jammer_high, target_high);
    
    if (overlap_high <= overlap_low) {
        return 0.0; // 无重叠
    }
    
    double overlap_bandwidth = overlap_high - overlap_low;
    return overlap_bandwidth / targetBandwidth; // 重叠比例
}

/// @brief 计算干扰有效功率
/// @details 计算到达目标的有效干扰功率
/// @return 有效干扰功率(dBm)
double CommunicationJammerModel::calculateJammerEffectivePower() const {
    // 计算到达目标的有效干扰功率
    double path_loss = calculatePropagationLoss(targetDistance, jammerFrequency);
    return jammerPower - path_loss - atmosphericLoss;
}

/// @brief 计算干扰干信比
/// @details 干信比 = 有效干扰功率 - 目标信号功率
/// @return 干信比(dB)
double CommunicationJammerModel::calculateJammingToSignalRatio() const {
    // 计算到达目标的有效干扰功率
    double jammer_path_loss = calculatePropagationLoss(targetDistance, jammerFrequency);
    double effective_jammer_power = jammerPower - jammer_path_loss - atmosphericLoss;
    
    // 计算目标接收的信号功率（需要获取目标信号相关参数）
    double signal_path_loss = calculatePropagationLoss(targetDistance, targetFrequency);
    double received_signal_power = targetPower - signal_path_loss - atmosphericLoss;
    
    // 计算干信比 (dB)：J/S = 有效干扰功率 - 接收信号功率
    return effective_jammer_power - received_signal_power;
}


/// @brief 计算干扰干信比
/// @details 干信比 = 有效干扰功率 - 目标信号功率
/// @return 干信比(dB)
double CommunicationJammerModel::calculateJammerToSignalRatio() const {
    double effective_jammer_power = calculateJammerEffectivePower();
    return effective_jammer_power - targetPower;
}

/// @brief 计算干扰有效性
/// @details 干扰有效性 = 干扰干信比 * 频率重叠度
/// @return 干扰有效性(dB)
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
    
    double degradation = effectiveness * (MathConstants::DEGRADATION_BASE - MathConstants::DEGRADATION_BASE / (MathConstants::DEGRADATION_BASE + std::pow(10.0, js_ratio / MathConstants::LINEAR_TO_DB_MULTIPLIER)));
    return std::min(degradation, MathConstants::MAX_DEGRADATION);
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
    double power_factor = std::min(MathConstants::MAX_POWER_FACTOR, std::pow(10.0, calculateJammerToSignalRatio() / MathConstants::FSPL_DISTANCE_COEFFICIENT));
    return frequency_overlap * power_factor;
}

double CommunicationJammerModel::calculateNarrowbandEffect() const {
    // 窄带干扰效果取决于频率精确度和功率
    double freq_diff = abs(jammerFrequency - targetFrequency);
    double freq_factor = exp(-freq_diff / targetBandwidth);
    double power_factor = std::min(MathConstants::MAX_POWER_FACTOR, std::pow(10.0, calculateJammerToSignalRatio() / MathConstants::LINEAR_TO_DB_MULTIPLIER));
    return freq_factor * power_factor;
}

double CommunicationJammerModel::calculateSweepFrequencyEffect() const {
    // 扫频干扰效果取决于扫频范围覆盖和驻留时间
    double coverage = std::min(MathConstants::MAX_COVERAGE, sweepRange * MathConstants::FREQUENCY_SCALE_FACTOR / targetBandwidth);
    double dwell_time = sweepRange / sweepRate; // 扫频周期
    double time_factor = std::min(MathConstants::MAX_TIME_FACTOR, targetBandwidth / (sweepRange * MathConstants::FREQUENCY_SCALE_FACTOR));
    double power_factor = std::min(MathConstants::MAX_POWER_FACTOR, std::pow(10.0, calculateJammerToSignalRatio() / MathConstants::PULSE_POWER_DIVISOR));
    return coverage * time_factor * power_factor;
}

double CommunicationJammerModel::calculatePulseJammerEffect() const {
    // 脉冲干扰效果取决于占空比和脉冲功率
    double frequency_overlap = calculateFrequencyOverlap();
    double pulse_power = jammerPower + MathConstants::LINEAR_TO_DB_MULTIPLIER * log10(MathConstants::PULSE_POWER_BASE / dutyCycle); // 脉冲峰值功率
    double effective_power = pulse_power - calculatePropagationLoss(targetDistance, jammerFrequency);
    double power_factor = std::min(MathConstants::MAX_POWER_FACTOR, std::pow(10.0, (effective_power - targetPower) / MathConstants::LINEAR_TO_DB_MULTIPLIER));
    return frequency_overlap * dutyCycle * power_factor;
}

double CommunicationJammerModel::calculateBarrageJammerEffect() const {
    // 阻塞干扰效果主要取决于带宽覆盖和功率
    double bandwidth_ratio = jammerBandwidth / targetBandwidth;
    double coverage = std::min(MathConstants::MAX_COVERAGE, bandwidth_ratio);
    double power_factor = std::min(MathConstants::MAX_POWER_FACTOR, std::pow(10.0, calculateJammerToSignalRatio() / MathConstants::BARRAGE_POWER_DIVISOR));
    return coverage * power_factor;
}

double CommunicationJammerModel::calculateSpotJammerEffect() const {
    // 点频干扰效果类似窄带干扰但更精确
    return std::min(MathConstants::MAX_SPOT_EFFECT, calculateNarrowbandEffect() * MathConstants::SPOT_ENHANCEMENT_FACTOR); // 点频干扰效果稍好，但不超过1.0
}

/// @brief 计算干扰覆盖面积
/// @details 基于圆形覆盖范围计算面积
/// @return 覆盖面积(m²)
double CommunicationJammerModel::calculateJammingArea() const {
    double radius = calculateJammingRange();
    if (radius <= 0) {
        return 0.0; // 无有效覆盖
    }
    return MathConstants::PI  * radius * radius; // 圆面积公式
}

/// @brief 计算干扰有效覆盖范围
/// @details 根据干扰机功率、目标最小干信比要求和传播模型，计算圆形覆盖半径
/// @return 干扰覆盖半径(km)，返回-1表示计算失败
double CommunicationJammerModel::calculateJammingRange() const {
    // 1. 参数有效性检查
    if (this->targetFrequency <= 0 || this->jammerFrequency <= 0) {
        return -1.0; // 无效参数返回错误
    }

    // 2. 计算到达目标的有效干扰功率需满足的条件
    // 干信比(J/S)要求：有效干扰功率 - 目标接收功率 ≥ 最小干信比
    // 假设最小干信比为10dB（可以作为参数传入或配置）
    double minRequiredJsRatio = 10.0; // dB
    double requiredJammerPowerAtTarget = this->targetPower + minRequiredJsRatio;

    // 3. 计算最大允许的路径损耗
    // 有效干扰功率 = 干扰机发射功率 - 路径损耗 - 大气损耗
    // 推导得：路径损耗 ≤ 干扰机发射功率 - 大气损耗 - 所需干扰功率
    // 假设大气损耗为2dB（可以作为参数传入或配置）
    double atmosphericLoss = 2.0; // dB
    double maxAllowedPathLoss = this->jammerPower - atmosphericLoss - requiredJammerPowerAtTarget;
    
    if (maxAllowedPathLoss < 0) {
        return 0.0; // 路径损耗为负，说明无有效覆盖范围
    }

    // 4. 使用CommunicationDistanceModel的距离计算方法
    // 频率转换为MHz (jammerFrequency是kHz，需要除以1000)
    double freqMHz = this->jammerFrequency / 1000.0;
    if (freqMHz <= 0) {
        return -1.0; // 无效频率
    }

    // 使用CommunicationDistanceModel的静态方法计算距离
    double distanceKm = CommunicationDistanceModel::calculateDistanceFromPathLoss(maxAllowedPathLoss, freqMHz);
    
    if (distanceKm < 0) {
        return -1.0; // 计算失败
    }
    
    // 确保距离为合理正值
    return distanceKm;
}

bool CommunicationJammerModel::isTargetInJammerRange() const {
    return this->targetDistance <= this->jammerRange;
}

/// @brief 计算所需干扰功率
/// @details 所需干扰功率 = 目标信号功率 +  desired_js_ratio + 路径损耗 + 大气损耗
/// @return 所需干扰功率(dBm)
double CommunicationJammerModel::calculateRequiredJammerPower(double desired_js_ratio) const {
    double path_loss = calculatePropagationLoss(this->targetDistance, this->jammerFrequency);
    double atmosphericLoss = 2.0; // dB - 假设大气损耗为2dB
    return this->targetPower + desired_js_ratio + path_loss + atmosphericLoss;
}

/// @brief 计算最优干扰频率
/// @details 最优干扰频率通常是目标信号的中心频率
/// @return 最优干扰频率(kHz)
double CommunicationJammerModel::calculateOptimalJammerFrequency() const {
    // 最优干扰频率通常是目标信号的中心频率
    return this->targetFrequency;
}

/// @brief 计算多干扰机协同效果
/// @details 多干扰机协同效果 = 所有干扰机有效功率的线性组合
/// @return 多干扰机协同效果(dB)
double CommunicationJammerModel::calculateCombinedJammerEffect(
    const std::vector<CommunicationJammerModel>& jammers) const {
    
    double total_jammer_power = 0.0;
    
    for (const auto& jammer : jammers) {
        double jammer_power_linear = std::pow(10.0, jammer.calculateJammerEffectivePower() / MathConstants::LINEAR_TO_DB_MULTIPLIER);
        double effectiveness = jammer.calculateJammerEffectiveness();
        total_jammer_power += jammer_power_linear * effectiveness;
    }
    
    double total_jammer_power_dBm = MathConstants::LINEAR_TO_DB_MULTIPLIER * log10(total_jammer_power);
    double combined_js_ratio = total_jammer_power_dBm - this->targetPower;
    
    return std::min(MathConstants::MAX_COMBINED_EFFECT, MathConstants::COMBINED_EFFECT_BASE - MathConstants::COMBINED_EFFECT_BASE / (MathConstants::COMBINED_EFFECT_BASE + std::pow(10.0, combined_js_ratio / MathConstants::LINEAR_TO_DB_MULTIPLIER)));
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

/// @brief 计算干扰覆盖范围
/// @details 基于圆形覆盖范围计算面积，返回km²单位
/// @return 覆盖范围(km²)
double CommunicationJammerModel::calculateJammerCoverage() const {
    double radius = calculateJammingRange();
    if (radius <= 0) {
        return 0.0; // 无有效覆盖
    }
    return MathConstants::PI * radius * radius; // 圆面积公式，单位km²
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
    oss << "干扰覆盖范围: " << calculateJammingArea() << " m²\n";
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