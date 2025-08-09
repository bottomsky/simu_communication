#include "CommunicationAntiJamModel.h"
#include "CommunicationAntiJamParameterConfig.h"
#include "MathConstants.h"
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iostream>

// 构造函数
CommunicationAntiJamModel::CommunicationAntiJamModel() 
    : antiJamTechnique_(AntiJamTechnique::FREQUENCY_HOPPING)
    , antiJamStrategy_(AntiJamStrategy::ADAPTIVE)
    , processingGain_(MathConstants::DEFAULT_PROCESSING_GAIN)
    , spreadingFactor_(MathConstants::DEFAULT_SPREADING_FACTOR)
    , hoppingRate_(MathConstants::DEFAULT_HOPPING_RATE)
    , codingGain_(MathConstants::DEFAULT_CODING_GAIN)
    , systemBandwidth_(MathConstants::DEFAULT_SYSTEM_BANDWIDTH)
    , signalPower_(MathConstants::DEFAULT_SIGNAL_POWER)
    , noisePower_(MathConstants::DEFAULT_NOISE_POWER)
    , interferenceLevel_(MathConstants::DEFAULT_INTERFERENCE_LEVEL)
    , hoppingChannels_(MathConstants::DEFAULT_HOPPING_CHANNELS)
    , channelSpacing_(MathConstants::DEFAULT_CHANNEL_SPACING)
    , dwellTime_(MathConstants::DEFAULT_DWELL_TIME)
    , chipRate_(MathConstants::DEFAULT_CHIP_RATE)
    , sequenceLength_(MathConstants::DEFAULT_SEQUENCE_LENGTH)
    , adaptationSpeed_(MathConstants::DEFAULT_ADAPTATION_SPEED)
    , convergenceThreshold_(MathConstants::DEFAULT_CONVERGENCE_THRESHOLD)
    , environmentType_(MathConstants::DEFAULT_ENVIRONMENT_TYPE)
    , jammerDensity_(MathConstants::DEFAULT_JAMMER_DENSITY) {
}

CommunicationAntiJamModel::CommunicationAntiJamModel(AntiJamTechnique technique, 
                                                   AntiJamStrategy strategy,
                                                   double processingGain,
                                                   double spreadingFactor,
                                                   double hoppingRate,
                                                   double codingGain)
    : antiJamTechnique_(technique)
    , antiJamStrategy_(strategy)
    , processingGain_(processingGain)
    , spreadingFactor_(spreadingFactor)
    , hoppingRate_(hoppingRate)
    , codingGain_(codingGain)
    , systemBandwidth_(MathConstants::DEFAULT_SYSTEM_BANDWIDTH)
    , signalPower_(MathConstants::DEFAULT_SIGNAL_POWER)
    , noisePower_(MathConstants::DEFAULT_NOISE_POWER)
    , interferenceLevel_(MathConstants::DEFAULT_INTERFERENCE_LEVEL)
    , hoppingChannels_(MathConstants::DEFAULT_HOPPING_CHANNELS)
    , channelSpacing_(MathConstants::DEFAULT_CHANNEL_SPACING)
    , dwellTime_(MathConstants::DEFAULT_DWELL_TIME)
    , chipRate_(MathConstants::DEFAULT_CHIP_RATE)
    , sequenceLength_(MathConstants::DEFAULT_SEQUENCE_LENGTH)
    , adaptationSpeed_(MathConstants::DEFAULT_ADAPTATION_SPEED)
    , convergenceThreshold_(MathConstants::DEFAULT_CONVERGENCE_THRESHOLD)
    , environmentType_(MathConstants::DEFAULT_ENVIRONMENT_TYPE)
    , jammerDensity_(MathConstants::DEFAULT_JAMMER_DENSITY) {
}

// 参数校验
bool CommunicationAntiJamModel::validateParameters() const {
    if (!CommunicationAntiJamParameterConfig::isProcessingGainValid(processingGain_)) return false;
    if (!CommunicationAntiJamParameterConfig::isSpreadingFactorValid(spreadingFactor_)) return false;
    if (!CommunicationAntiJamParameterConfig::isHoppingRateValid(hoppingRate_)) return false;
    if (!CommunicationAntiJamParameterConfig::isCodingGainValid(codingGain_)) return false;
    if (!CommunicationAntiJamParameterConfig::isSystemBandwidthValid(systemBandwidth_)) return false;
    if (!CommunicationAntiJamParameterConfig::isSignalPowerValid(signalPower_)) return false;
    if (!CommunicationAntiJamParameterConfig::isNoisePowerValid(noisePower_)) return false;
    if (!CommunicationAntiJamParameterConfig::isInterferenceLevelValid(interferenceLevel_)) return false;
    if (!CommunicationAntiJamParameterConfig::isHoppingChannelsValid(hoppingChannels_)) return false;
    if (!CommunicationAntiJamParameterConfig::isChannelSpacingValid(channelSpacing_)) return false;
    if (!CommunicationAntiJamParameterConfig::isDwellTimeValid(dwellTime_)) return false;
    if (!CommunicationAntiJamParameterConfig::isChipRateValid(chipRate_)) return false;
    if (!CommunicationAntiJamParameterConfig::isSequenceLengthValid(sequenceLength_)) return false;
    if (!CommunicationAntiJamParameterConfig::isAdaptationSpeedValid(adaptationSpeed_)) return false;
    if (!CommunicationAntiJamParameterConfig::isConvergenceThresholdValid(convergenceThreshold_)) return false;
    if (!CommunicationAntiJamParameterConfig::isEnvironmentTypeValid(environmentType_)) return false;
    if (!CommunicationAntiJamParameterConfig::isJammerDensityValid(jammerDensity_)) return false;
    return true;
}

// 参数设置方法
bool CommunicationAntiJamModel::setAntiJamTechnique(AntiJamTechnique technique) {
    antiJamTechnique_ = technique;
    return true;
}

bool CommunicationAntiJamModel::setAntiJamStrategy(AntiJamStrategy strategy) {
    antiJamStrategy_ = strategy;
    return true;
}

bool CommunicationAntiJamModel::setProcessingGain(double gain) {
    if (!CommunicationAntiJamParameterConfig::isProcessingGainValid(gain)) return false;
    processingGain_ = gain;
    return true;
}

bool CommunicationAntiJamModel::setSpreadingFactor(double factor) {
    if (!CommunicationAntiJamParameterConfig::isSpreadingFactorValid(factor)) return false;
    spreadingFactor_ = factor;
    return true;
}

bool CommunicationAntiJamModel::setHoppingRate(double rate) {
    if (!CommunicationAntiJamParameterConfig::isHoppingRateValid(rate)) return false;
    hoppingRate_ = rate;
    return true;
}

bool CommunicationAntiJamModel::setCodingGain(double gain) {
    if (!CommunicationAntiJamParameterConfig::isCodingGainValid(gain)) return false;
    codingGain_ = gain;
    return true;
}

bool CommunicationAntiJamModel::setSystemBandwidth(double bandwidth) {
    if (!CommunicationAntiJamParameterConfig::isSystemBandwidthValid(bandwidth)) return false;
    systemBandwidth_ = bandwidth;
    return true;
}

bool CommunicationAntiJamModel::setSignalPower(double power) {
    if (!CommunicationAntiJamParameterConfig::isSignalPowerValid(power)) return false;
    signalPower_ = power;
    return true;
}

bool CommunicationAntiJamModel::setNoisePower(double power) {
    if (!CommunicationAntiJamParameterConfig::isNoisePowerValid(power)) return false;
    noisePower_ = power;
    return true;
}

bool CommunicationAntiJamModel::setInterferenceLevel(double level) {
    if (!CommunicationAntiJamParameterConfig::isInterferenceLevelValid(level)) return false;
    interferenceLevel_ = level;
    return true;
}

bool CommunicationAntiJamModel::setHoppingChannels(int channels) {
    if (!CommunicationAntiJamParameterConfig::isHoppingChannelsValid(channels)) return false;
    hoppingChannels_ = channels;
    return true;
}

bool CommunicationAntiJamModel::setChannelSpacing(double spacing) {
    if (!CommunicationAntiJamParameterConfig::isChannelSpacingValid(spacing)) return false;
    channelSpacing_ = spacing;
    return true;
}

bool CommunicationAntiJamModel::setDwellTime(double time) {
    if (!CommunicationAntiJamParameterConfig::isDwellTimeValid(time)) return false;
    dwellTime_ = time;
    return true;
}

bool CommunicationAntiJamModel::setChipRate(int rate) {
    if (!CommunicationAntiJamParameterConfig::isChipRateValid(rate)) return false;
    chipRate_ = rate;
    return true;
}

bool CommunicationAntiJamModel::setSequenceLength(double length) {
    if (!CommunicationAntiJamParameterConfig::isSequenceLengthValid(length)) return false;
    sequenceLength_ = length;
    return true;
}

bool CommunicationAntiJamModel::setAdaptationSpeed(double speed) {
    if (!CommunicationAntiJamParameterConfig::isAdaptationSpeedValid(speed)) return false;
    adaptationSpeed_ = speed;
    return true;
}

bool CommunicationAntiJamModel::setConvergenceThreshold(double threshold) {
    if (!CommunicationAntiJamParameterConfig::isConvergenceThresholdValid(threshold)) return false;
    convergenceThreshold_ = threshold;
    return true;
}

bool CommunicationAntiJamModel::setEnvironmentType(double type) {
    if (!CommunicationAntiJamParameterConfig::isEnvironmentTypeValid(type)) return false;
    environmentType_ = type;
    return true;
}

bool CommunicationAntiJamModel::setJammerDensity(double density) {
    if (!CommunicationAntiJamParameterConfig::isJammerDensityValid(density)) return false;
    jammerDensity_ = density;
    return true;
}

// 内部计算方法
double CommunicationAntiJamModel::calculateFrequencyHoppingGain() const {
    // 跳频增益 = 10 * log10(跳频信道数)
    return MathConstants::LINEAR_TO_DB_MULTIPLIER * std::log10(static_cast<double>(hoppingChannels_));
}

double CommunicationAntiJamModel::calculateDirectSequenceGain() const {
    // 直接序列扩频增益 = 10 * log10(扩频因子)
    return MathConstants::LINEAR_TO_DB_MULTIPLIER * std::log10(spreadingFactor_);
}

double CommunicationAntiJamModel::calculateTimeHoppingGain() const {
    // 跳时增益基于跳时速率和驻留时间
    double timeSlots = MathConstants::TIME_SLOTS_PER_SECOND / dwellTime_; // 每秒时隙数
    return MathConstants::LINEAR_TO_DB_MULTIPLIER * std::log10(timeSlots);
}

double CommunicationAntiJamModel::calculateAdaptiveFilteringGain() const {
    // 自适应滤波增益基于自适应速度和收敛性能
    double adaptiveGain = MathConstants::ADAPTIVE_GAIN_BASE + MathConstants::ADAPTIVE_GAIN_SPEED_COEFF * adaptationSpeed_ - MathConstants::ADAPTIVE_GAIN_CONVERGENCE_COEFF * convergenceThreshold_;
    return std::max(MathConstants::MIN_ADAPTIVE_GAIN, std::min(MathConstants::MAX_ADAPTIVE_GAIN, adaptiveGain));
}

double CommunicationAntiJamModel::calculateBeamFormingGain() const {
    // 波束成形增益，假设基于系统带宽
    double beamGain = MathConstants::BEAM_GAIN_BASE + MathConstants::BEAM_GAIN_BANDWIDTH_COEFF * systemBandwidth_;
    return std::max(MathConstants::MIN_BEAM_GAIN, std::min(MathConstants::MAX_BEAM_GAIN, beamGain));
}

double CommunicationAntiJamModel::calculateDiversityGain() const {
    // 分集增益，基于环境类型
    double diversityGain = MathConstants::DIVERSITY_GAIN_BASE + MathConstants::DIVERSITY_GAIN_ENV_COEFF * environmentType_;
    return std::max(MathConstants::MIN_DIVERSITY_GAIN, std::min(MathConstants::MAX_DIVERSITY_GAIN, diversityGain));
}

double CommunicationAntiJamModel::calculateErrorCorrectionGain() const {
    // 纠错编码增益
    return codingGain_;
}

double CommunicationAntiJamModel::calculateInterferenceCancellationGain() const {
    // 干扰抵消增益，基于干扰机密度
    double cancellationGain = MathConstants::CANCELLATION_GAIN_MULTIPLIER * (MathConstants::CANCELLATION_GAIN_BASE - jammerDensity_);
    return std::max(MathConstants::MIN_CANCELLATION_GAIN, std::min(MathConstants::MAX_CANCELLATION_GAIN, cancellationGain));
}

double CommunicationAntiJamModel::calculateTotalProcessingGain() const {
    double totalGain = processingGain_;
    
    switch (antiJamTechnique_) {
        case AntiJamTechnique::FREQUENCY_HOPPING:
            totalGain += calculateFrequencyHoppingGain();
            break;
        case AntiJamTechnique::DIRECT_SEQUENCE:
            totalGain += calculateDirectSequenceGain();
            break;
        case AntiJamTechnique::TIME_HOPPING:
            totalGain += calculateTimeHoppingGain();
            break;
        case AntiJamTechnique::HYBRID_SPREAD:
            totalGain += (calculateFrequencyHoppingGain() + calculateDirectSequenceGain()) * MathConstants::HYBRID_SPREAD_FACTOR;
            break;
        case AntiJamTechnique::ADAPTIVE_FILTERING:
            totalGain += calculateAdaptiveFilteringGain();
            break;
        case AntiJamTechnique::BEAM_FORMING:
            totalGain += calculateBeamFormingGain();
            break;
        case AntiJamTechnique::POWER_CONTROL:
            totalGain += MathConstants::POWER_CONTROL_GAIN; // 固定功率控制增益
            break;
        case AntiJamTechnique::ERROR_CORRECTION:
            totalGain += calculateErrorCorrectionGain();
            break;
        case AntiJamTechnique::DIVERSITY_RECEPTION:
            totalGain += calculateDiversityGain();
            break;
        case AntiJamTechnique::INTERFERENCE_CANCELLATION:
            totalGain += calculateInterferenceCancellationGain();
            break;
    }
    
    return totalGain;
}

// 核心计算方法
double CommunicationAntiJamModel::calculateAntiJamGain() const {
    if (!validateParameters()) return 0.0;
    
    double totalGain = calculateTotalProcessingGain();
    
    // 根据抗干扰策略调整增益
    switch (antiJamStrategy_) {
        case AntiJamStrategy::PASSIVE:
            totalGain *= MathConstants::PASSIVE_STRATEGY_FACTOR; // 被动策略效果较低
            break;
        case AntiJamStrategy::ACTIVE:
            totalGain *= MathConstants::ACTIVE_STRATEGY_FACTOR; // 主动策略标准效果
            break;
        case AntiJamStrategy::ADAPTIVE:
            totalGain *= MathConstants::ADAPTIVE_STRATEGY_FACTOR; // 自适应策略效果较好
            break;
        case AntiJamStrategy::COOPERATIVE:
            totalGain *= MathConstants::COOPERATIVE_STRATEGY_FACTOR; // 协作策略效果更好
            break;
        case AntiJamStrategy::COGNITIVE:
            totalGain *= MathConstants::COGNITIVE_STRATEGY_FACTOR; // 认知策略效果最好
            break;
    }
    
    return totalGain;
}

double CommunicationAntiJamModel::calculateJammerResistance() const {
    if (!validateParameters()) return 0.0;
    
    double antiJamGain = calculateAntiJamGain();
    double signalToNoise = signalPower_ - noisePower_;
    double signalToJammer = signalPower_ - interferenceLevel_ + antiJamGain;
    
    // 抗干扰能力基于信干比和抗干扰增益
    double jammerPower = interferenceLevel_;
    double resistance = MathConstants::MAX_RESISTANCE - (jammerPower / (jammerPower + signalPower_));
    resistance = std::max(MathConstants::MIN_RESISTANCE, resistance);
    
    // 考虑抗干扰增益
    resistance *= (MathConstants::RESISTANCE_BASE + antiJamGain / MathConstants::RESISTANCE_GAIN_DIVISOR);
    return std::max(0.0, std::min(1.0, resistance));
}

double CommunicationAntiJamModel::calculateSignalToJammerRatio() const {
    if (!validateParameters()) return MathConstants::MIN_SJR_DB;
    
    double antiJamGain = calculateAntiJamGain();
    double signalToJammer = signalPower_ - interferenceLevel_ + antiJamGain;
    return std::max(MathConstants::MIN_SJR_DB, signalToJammer);
}

double CommunicationAntiJamModel::calculateBitErrorRateWithJamming() const {
    if (!validateParameters()) return 1.0;
    
    double sjr = calculateSignalToJammerRatio();
    double snr = signalPower_ - noisePower_;
    
    // 综合信噪比（考虑噪声和干扰）
    double totalNoise = std::max(noisePower_, interferenceLevel_ - calculateAntiJamGain());
    double effectiveSnr = signalPower_ - totalNoise;
    
    // 简化的误码率计算（BPSK调制）
    double linearSnr = std::pow(MathConstants::LINEAR_TO_DB_BASE, effectiveSnr / MathConstants::LINEAR_TO_DB_MULTIPLIER);
    double ber = MathConstants::BER_COEFFICIENT * std::erfc(std::sqrt(linearSnr));
    
    return std::max(MathConstants::MIN_BER, std::min(MathConstants::MAX_BER, ber));
}

double CommunicationAntiJamModel::calculateThroughputDegradation() const {
    if (!validateParameters()) return 1.0;
    
    double ber = calculateBitErrorRateWithJamming();
    double degradation = MathConstants::MAX_DEGRADATION - std::exp(-MathConstants::DEGRADATION_FACTOR * ber);
    
    return std::max(MathConstants::MIN_DEGRADATION, std::min(MathConstants::MAX_DEGRADATION, degradation));
}

double CommunicationAntiJamModel::calculateDetectionProbability() const {
    if (!validateParameters()) return 1.0;
    
    double sjr = calculateSignalToJammerRatio();
    
    // 检测概率基于信干比
    double detectionProb = MathConstants::MAX_DETECTION_PROB / (MathConstants::MAX_DETECTION_PROB + std::exp(-(sjr + MathConstants::DETECTION_OFFSET) / MathConstants::DETECTION_SCALE));
    
    return std::max(MathConstants::MIN_DETECTION_PROB, std::min(MathConstants::MAX_DETECTION_PROB, detectionProb));
}

double CommunicationAntiJamModel::calculateInterceptionResistance() const {
    if (!validateParameters()) return 0.0;
    
    double antiJamGain = calculateAntiJamGain();
    
    // 抗截获能力主要基于扩频增益和跳频特性
    double resistance = 0.0;
    
    switch (antiJamTechnique_) {
        case AntiJamTechnique::FREQUENCY_HOPPING:
            resistance = MathConstants::FH_BASE_RESISTANCE + MathConstants::FH_RESISTANCE_FACTOR * std::min(MathConstants::MAX_RESISTANCE_FACTOR, hoppingRate_ / MathConstants::FH_RATE_NORMALIZATION);
            break;
        case AntiJamTechnique::DIRECT_SEQUENCE:
            resistance = MathConstants::DS_BASE_RESISTANCE + MathConstants::DS_RESISTANCE_FACTOR * std::min(MathConstants::MAX_RESISTANCE_FACTOR, spreadingFactor_ / MathConstants::DS_FACTOR_NORMALIZATION);
            break;
        case AntiJamTechnique::TIME_HOPPING:
            resistance = MathConstants::TH_BASE_RESISTANCE + MathConstants::TH_RESISTANCE_FACTOR * std::min(MathConstants::MAX_RESISTANCE_FACTOR, MathConstants::MAX_RESISTANCE_FACTOR / dwellTime_);
            break;
        case AntiJamTechnique::HYBRID_SPREAD:
            resistance = MathConstants::HYBRID_RESISTANCE;
            break;
        default:
            resistance = MathConstants::DEFAULT_BASE_RESISTANCE + MathConstants::DEFAULT_RESISTANCE_FACTOR * antiJamGain;
            break;
    }
    
    return std::max(0.0, std::min(1.0, resistance));
}

/// @brief 评估抗干扰效果
/// @details 基于抗干扰能力、抗截获能力和吞吐量维护评估抗干扰效果
/// @return 抗干扰效果等级
AntiJamEffectLevel CommunicationAntiJamModel::evaluateAntiJamEffect() const {
    double effectiveness = calculateProtectionEffectiveness();
    
    if (effectiveness < MathConstants::NO_PROTECTION_THRESHOLD) return AntiJamEffectLevel::NO_PROTECTION;
    else if (effectiveness < MathConstants::LOW_PROTECTION_THRESHOLD) return AntiJamEffectLevel::LOW_PROTECTION;
    else if (effectiveness < MathConstants::MEDIUM_PROTECTION_THRESHOLD) return AntiJamEffectLevel::MEDIUM_PROTECTION;
    else if (effectiveness < MathConstants::HIGH_PROTECTION_THRESHOLD) return AntiJamEffectLevel::HIGH_PROTECTION;
    else return AntiJamEffectLevel::EXCELLENT_PROTECTION;
}

/// @brief 计算抗干扰效果
/// @details 抗干扰效果 = 抗干扰能力 + 抗截获能力 + 吞吐量维护
/// @return 抗干扰效果(0-1)
double CommunicationAntiJamModel::calculateProtectionEffectiveness() const {
    if (!validateParameters()) return 0.0;
    
    double resistance = calculateJammerResistance();
    double interceptionResistance = calculateInterceptionResistance();
    double throughputMaintenance = MathConstants::MAX_THROUGHPUT_MAINTENANCE - calculateThroughputDegradation();
    
    // 综合保护有效性
    double effectiveness = (resistance * MathConstants::RESISTANCE_WEIGHT + interceptionResistance * MathConstants::INTERCEPTION_WEIGHT + throughputMaintenance * MathConstants::THROUGHPUT_WEIGHT);
    
    return std::max(0.0, std::min(1.0, effectiveness));
}

/// @brief  计算抗干扰效果
/// @details 自适应效率 = 自适应速度 * (1 - 收敛阈值)
/// @return 自适应效率(0-1)
double CommunicationAntiJamModel::calculateAdaptationEfficiency() const {
    if (!validateParameters()) return 0.0;
    
    if (antiJamStrategy_ != AntiJamStrategy::ADAPTIVE && 
        antiJamStrategy_ != AntiJamStrategy::COGNITIVE) {
        return 0.0; // 非自适应策略
    }
    
    // 自适应效率基于自适应速度和收敛阈值
    double efficiency = adaptationSpeed_ * (MathConstants::MAX_ADAPTATION_EFFICIENCY - convergenceThreshold_);
    
    return std::max(0.0, std::min(1.0, efficiency));
}

/// @brief 计算资源利用率
/// @details 资源利用率 = (带宽利用率 + 功率利用率 + 处理利用率) / 3
/// @return 资源利用率(0-1)
double CommunicationAntiJamModel::calculateResourceUtilization() const {
    if (!validateParameters()) return 0.0;
    
    double bandwidthUtilization = std::min(MathConstants::MAX_BANDWIDTH_UTILIZATION, systemBandwidth_ / MathConstants::BANDWIDTH_NORMALIZATION);
    double powerUtilization = std::min(MathConstants::MAX_POWER_UTILIZATION, (signalPower_ + MathConstants::POWER_OFFSET) / MathConstants::POWER_NORMALIZATION);
    double processingUtilization = std::min(MathConstants::MAX_PROCESSING_UTILIZATION, processingGain_ / MathConstants::PROCESSING_NORMALIZATION);
    
    return (bandwidthUtilization + powerUtilization + processingUtilization) / MathConstants::UTILIZATION_COMPONENTS;
}

// 特定技术效果计算
double CommunicationAntiJamModel::calculateFrequencyHoppingEffectiveness() const {
    if (antiJamTechnique_ != AntiJamTechnique::FREQUENCY_HOPPING) return 0.0;
    
    double hoppingGain = calculateFrequencyHoppingGain();
    double effectiveness = hoppingGain / MathConstants::HOPPING_GAIN_NORMALIZATION; // 归一化到0-1
    
    return std::max(0.0, std::min(1.0, effectiveness));
}

/// @brief 计算扩频频谱效果
/// @details 扩频频谱效果 = 扩频增益 / 40
/// @return 扩频频谱效果(0-1)
double CommunicationAntiJamModel::calculateSpreadSpectrumEffectiveness() const {
    if (antiJamTechnique_ != AntiJamTechnique::DIRECT_SEQUENCE) return 0.0;
    
    double spreadGain = calculateDirectSequenceGain();
    double effectiveness = spreadGain / MathConstants::SPREAD_GAIN_NORMALIZATION; // 归一化到0-1
    
    return std::max(0.0, std::min(1.0, effectiveness));
}
/// @brief 计算自适应滤波效果
/// @details 自适应滤波效果 = 自适应滤波增益 / 15
/// @return 频谱 Direc Sequence 效果(0-1)
double CommunicationAntiJamModel::calculateAdaptiveFilteringEffectiveness() const {
    if (antiJamTechnique_ != AntiJamTechnique::ADAPTIVE_FILTERING) return 0.0;
    
    double filterGain = calculateAdaptiveFilteringGain();
    double effectiveness = filterGain / MathConstants::FILTER_GAIN_NORMALIZATION; // 归一化到0-1
    
    return std::max(0.0, std::min(1.0, effectiveness));
}
/// @brief 计算波束形成效果
/// @details 波束形成效果 = 波束形成增益 / 20
/// @return 波束形成效果(0-1)
double CommunicationAntiJamModel::calculateBeamFormingEffectiveness() const {
    if (antiJamTechnique_ != AntiJamTechnique::BEAM_FORMING) return 0.0;
    
    double beamGain = calculateBeamFormingGain();
    double effectiveness = beamGain / MathConstants::BEAM_GAIN_NORMALIZATION; // 归一化到0-1
    
    return std::max(0.0, std::min(1.0, effectiveness));
}

/// @brief 计算分集效果
/// @details 分集效果 = 分集增益 / 10
/// @return 分集效果(0-1)
double CommunicationAntiJamModel::calculateDiversityEffectiveness() const {
    if (antiJamTechnique_ != AntiJamTechnique::DIVERSITY_RECEPTION) return 0.0;
    
    double diversityGain = calculateDiversityGain();
    double effectiveness = diversityGain / MathConstants::DIVERSITY_GAIN_NORMALIZATION; // 归一化到0-1
    
    return std::max(0.0, std::min(1.0, effectiveness));
}
/// @brief 计算纠错效果
/// @details 纠错效果 = 纠错增益 / 20
/// @return 纠错效果(0-1)
double CommunicationAntiJamModel::calculateErrorCorrectionEffectiveness() const {
    if (antiJamTechnique_ != AntiJamTechnique::ERROR_CORRECTION) return 0.0;
    
    double codingEffectiveness = codingGain_ / MathConstants::CODING_GAIN_NORMALIZATION; // 归一化到0-1
    
    return std::max(0.0, std::min(1.0, codingEffectiveness));
}

// 最优策略计算
AntiJamTechnique CommunicationAntiJamModel::calculateOptimalTechnique() const {
    std::vector<AntiJamTechnique> techniques = {
        AntiJamTechnique::FREQUENCY_HOPPING,
        AntiJamTechnique::DIRECT_SEQUENCE,
        AntiJamTechnique::TIME_HOPPING,
        AntiJamTechnique::HYBRID_SPREAD,
        AntiJamTechnique::ADAPTIVE_FILTERING,
        AntiJamTechnique::BEAM_FORMING,
        AntiJamTechnique::POWER_CONTROL,
        AntiJamTechnique::ERROR_CORRECTION,
        AntiJamTechnique::DIVERSITY_RECEPTION,
        AntiJamTechnique::INTERFERENCE_CANCELLATION
    };
    
    AntiJamTechnique bestTechnique = antiJamTechnique_;
    double bestEffectiveness = 0.0;
    
    AntiJamTechnique originalTechnique = antiJamTechnique_;
    
    for (auto technique : techniques) {
        const_cast<CommunicationAntiJamModel*>(this)->antiJamTechnique_ = technique;
        double effectiveness = calculateProtectionEffectiveness();
        
        if (effectiveness > bestEffectiveness) {
            bestEffectiveness = effectiveness;
            bestTechnique = technique;
        }
    }
    
    const_cast<CommunicationAntiJamModel*>(this)->antiJamTechnique_ = originalTechnique;
    
    return bestTechnique;
}

double CommunicationAntiJamModel::calculateOptimalProcessingGain() const {
    // 基于当前干扰水平计算最优处理增益
    double interferenceMargin = interferenceLevel_ - noisePower_;
    double optimalGain = interferenceMargin + MathConstants::PROCESSING_GAIN_MARGIN; // 处理增益余量
    
    return std::max(MathConstants::MIN_PROCESSING_GAIN, std::min(MathConstants::MAX_PROCESSING_GAIN, optimalGain));
}

double CommunicationAntiJamModel::calculateOptimalHoppingRate() const {
    if (antiJamTechnique_ != AntiJamTechnique::FREQUENCY_HOPPING) return hoppingRate_;
    
    // 最优跳频速率基于干扰机密度和系统带宽
    double optimalRate = MathConstants::OPTIMAL_HOPPING_BASE * (MathConstants::HOPPING_RATE_BASE + jammerDensity_) * (systemBandwidth_ / MathConstants::HOPPING_BANDWIDTH_DIVISOR);
    
    return std::max(MathConstants::MIN_HOPPING_RATE, std::min(MathConstants::MAX_HOPPING_RATE, optimalRate));
}

int CommunicationAntiJamModel::calculateOptimalHoppingChannels() const {
    if (antiJamTechnique_ != AntiJamTechnique::FREQUENCY_HOPPING) return hoppingChannels_;
    
    // 最优跳频信道数基于系统带宽和信道间隔
    int optimalChannels = static_cast<int>(systemBandwidth_ / channelSpacing_);
    
    return std::max(MathConstants::MIN_HOPPING_CHANNELS, std::min(MathConstants::MAX_HOPPING_CHANNELS, optimalChannels));
}

// 多技术组合效果
double CommunicationAntiJamModel::calculateCombinedTechniqueEffect(const std::vector<AntiJamTechnique>& techniques) const {
    if (techniques.empty()) return 0.0;
    
    double combinedGain = MathConstants::COMBINED_GAIN_INITIAL;
    double synergy = MathConstants::SYNERGY_INITIAL;
    
    for (auto technique : techniques) {
        AntiJamTechnique originalTechnique = antiJamTechnique_;
        const_cast<CommunicationAntiJamModel*>(this)->antiJamTechnique_ = technique;
        
        double gain = calculateAntiJamGain();
        combinedGain += gain * synergy;
        synergy *= MathConstants::SYNERGY_DECAY_FACTOR; // 协同效应递减
        
        const_cast<CommunicationAntiJamModel*>(this)->antiJamTechnique_ = originalTechnique;
    }
    
    return combinedGain;
}

std::vector<AntiJamTechnique> CommunicationAntiJamModel::getRecommendedTechniqueCombination() const {
    std::vector<AntiJamTechnique> recommended;
    
    // 基于环境和威胁特征推荐技术组合
    if (jammerDensity_ > MathConstants::HIGH_THREAT_THRESHOLD) {
        // 高威胁环境
        recommended.push_back(AntiJamTechnique::HYBRID_SPREAD);
        recommended.push_back(AntiJamTechnique::ADAPTIVE_FILTERING);
        recommended.push_back(AntiJamTechnique::INTERFERENCE_CANCELLATION);
    } else if (jammerDensity_ > MathConstants::MEDIUM_THREAT_THRESHOLD) {
        // 中等威胁环境
        recommended.push_back(AntiJamTechnique::FREQUENCY_HOPPING);
        recommended.push_back(AntiJamTechnique::ERROR_CORRECTION);
    } else {
        // 低威胁环境
        recommended.push_back(AntiJamTechnique::DIRECT_SEQUENCE);
        recommended.push_back(AntiJamTechnique::POWER_CONTROL);
    }
    
    return recommended;
}

// 性能预测
double CommunicationAntiJamModel::predictPerformanceUnderJamming(double jammerPower, double jammerBandwidth) const {
    double originalInterference = interferenceLevel_;
    const_cast<CommunicationAntiJamModel*>(this)->interferenceLevel_ = jammerPower;
    
    double performance = MathConstants::MAX_PERFORMANCE - calculateThroughputDegradation();
    
    const_cast<CommunicationAntiJamModel*>(this)->interferenceLevel_ = originalInterference;
    
    return performance;
}

double CommunicationAntiJamModel::calculateRequiredAntiJamGain(double targetBER) const {
    if (targetBER <= MathConstants::MIN_TARGET_BER || targetBER >= MathConstants::MAX_TARGET_BER) return MathConstants::ZERO_GAIN;
    
    // 基于目标误码率计算所需抗干扰增益
    double requiredSnr = -MathConstants::LINEAR_TO_DB_MULTIPLIER * std::log10(MathConstants::BER_MULTIPLIER * targetBER);
    double currentSnr = signalPower_ - std::max(noisePower_, interferenceLevel_);
    double requiredGain = requiredSnr - currentSnr;
    
    return std::max(0.0, requiredGain);
}

double CommunicationAntiJamModel::calculateMaxTolerableJammerPower() const {
    double antiJamGain = calculateAntiJamGain();
    double minRequiredSjr = MathConstants::MIN_REQUIRED_SJR; // 最小所需信干比
    
    double maxJammerPower = signalPower_ + antiJamGain - minRequiredSjr;
    
    return maxJammerPower;
}

// 信息输出方法
std::string CommunicationAntiJamModel::getParameterInfo() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    
    oss << "=== 通信抗干扰模型参数信息 ===" << std::endl;
    
    // 抗干扰技术类型
    oss << "抗干扰技术: ";
    switch (antiJamTechnique_) {
        case AntiJamTechnique::FREQUENCY_HOPPING: oss << "跳频"; break;
        case AntiJamTechnique::DIRECT_SEQUENCE: oss << "直接序列扩频"; break;
        case AntiJamTechnique::TIME_HOPPING: oss << "跳时"; break;
        case AntiJamTechnique::HYBRID_SPREAD: oss << "混合扩频"; break;
        case AntiJamTechnique::ADAPTIVE_FILTERING: oss << "自适应滤波"; break;
        case AntiJamTechnique::BEAM_FORMING: oss << "波束成形"; break;
        case AntiJamTechnique::POWER_CONTROL: oss << "功率控制"; break;
        case AntiJamTechnique::ERROR_CORRECTION: oss << "纠错编码"; break;
        case AntiJamTechnique::DIVERSITY_RECEPTION: oss << "分集接收"; break;
        case AntiJamTechnique::INTERFERENCE_CANCELLATION: oss << "干扰抵消"; break;
    }
    oss << std::endl;
    
    // 抗干扰策略
    oss << "抗干扰策略: ";
    switch (antiJamStrategy_) {
        case AntiJamStrategy::PASSIVE: oss << "被动抗干扰"; break;
        case AntiJamStrategy::ACTIVE: oss << "主动抗干扰"; break;
        case AntiJamStrategy::ADAPTIVE: oss << "自适应抗干扰"; break;
        case AntiJamStrategy::COOPERATIVE: oss << "协作抗干扰"; break;
        case AntiJamStrategy::COGNITIVE: oss << "认知抗干扰"; break;
    }
    oss << std::endl;
    
    oss << "处理增益: " << processingGain_ << " dB" << std::endl;
    oss << "扩频因子: " << spreadingFactor_ << std::endl;
    oss << "跳频速率: " << hoppingRate_ << " Hz" << std::endl;
    oss << "编码增益: " << codingGain_ << " dB" << std::endl;
    oss << "系统带宽: " << systemBandwidth_ << " MHz" << std::endl;
    oss << "信号功率: " << signalPower_ << " dBm" << std::endl;
    oss << "噪声功率: " << noisePower_ << " dBm" << std::endl;
    oss << "干扰电平: " << interferenceLevel_ << " dBm" << std::endl;
    
    if (antiJamTechnique_ == AntiJamTechnique::FREQUENCY_HOPPING) {
        oss << "跳频信道数: " << hoppingChannels_ << std::endl;
        oss << "信道间隔: " << channelSpacing_ << " MHz" << std::endl;
        oss << "驻留时间: " << dwellTime_ << " ms" << std::endl;
    }
    
    if (antiJamTechnique_ == AntiJamTechnique::DIRECT_SEQUENCE) {
        oss << "码片速率: " << chipRate_ << " Mcps" << std::endl;
        oss << "序列长度: " << sequenceLength_ << std::endl;
    }
    
    return oss.str();
}

std::string CommunicationAntiJamModel::getAntiJamEffectInfo() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    
    oss << "=== 抗干扰效果信息 ===" << std::endl;
    oss << "抗干扰增益: " << calculateAntiJamGain() << " dB" << std::endl;
    oss << "抗干扰能力: " << calculateJammerResistance() * MathConstants::PERCENTAGE_MULTIPLIER << "%" << std::endl;
    oss << "信干比: " << calculateSignalToJammerRatio() << " dB" << std::endl;
    oss << "有干扰误码率: " << calculateBitErrorRateWithJamming() << std::endl;
    oss << "吞吐量下降: " << calculateThroughputDegradation() * MathConstants::PERCENTAGE_MULTIPLIER << "%" << std::endl;
    oss << "检测概率: " << calculateDetectionProbability() * MathConstants::PERCENTAGE_MULTIPLIER << "%" << std::endl;
    oss << "抗截获能力: " << calculateInterceptionResistance() * MathConstants::PERCENTAGE_MULTIPLIER << "%" << std::endl;
    oss << "保护有效性: " << calculateProtectionEffectiveness() * MathConstants::PERCENTAGE_MULTIPLIER << "%" << std::endl;
    
    AntiJamEffectLevel level = evaluateAntiJamEffect();
    oss << "抗干扰等级: ";
    switch (level) {
        case AntiJamEffectLevel::NO_PROTECTION: oss << "无保护"; break;
        case AntiJamEffectLevel::LOW_PROTECTION: oss << "低保护"; break;
        case AntiJamEffectLevel::MEDIUM_PROTECTION: oss << "中等保护"; break;
        case AntiJamEffectLevel::HIGH_PROTECTION: oss << "高保护"; break;
        case AntiJamEffectLevel::EXCELLENT_PROTECTION: oss << "优秀保护"; break;
    }
    oss << std::endl;
    
    return oss.str();
}

std::string CommunicationAntiJamModel::getRecommendationInfo() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    
    oss << "=== 抗干扰建议信息 ===" << std::endl;
    
    AntiJamTechnique optimalTech = calculateOptimalTechnique();
    oss << "推荐抗干扰技术: ";
    switch (optimalTech) {
        case AntiJamTechnique::FREQUENCY_HOPPING: oss << "跳频"; break;
        case AntiJamTechnique::DIRECT_SEQUENCE: oss << "直接序列扩频"; break;
        case AntiJamTechnique::TIME_HOPPING: oss << "跳时"; break;
        case AntiJamTechnique::HYBRID_SPREAD: oss << "混合扩频"; break;
        case AntiJamTechnique::ADAPTIVE_FILTERING: oss << "自适应滤波"; break;
        case AntiJamTechnique::BEAM_FORMING: oss << "波束成形"; break;
        case AntiJamTechnique::POWER_CONTROL: oss << "功率控制"; break;
        case AntiJamTechnique::ERROR_CORRECTION: oss << "纠错编码"; break;
        case AntiJamTechnique::DIVERSITY_RECEPTION: oss << "分集接收"; break;
        case AntiJamTechnique::INTERFERENCE_CANCELLATION: oss << "干扰抵消"; break;
    }
    oss << std::endl;
    
    oss << "建议处理增益: " << calculateOptimalProcessingGain() << " dB" << std::endl;
    oss << "建议跳频速率: " << calculateOptimalHoppingRate() << " Hz" << std::endl;
    oss << "建议跳频信道数: " << calculateOptimalHoppingChannels() << std::endl;
    oss << "最大可容忍干扰功率: " << calculateMaxTolerableJammerPower() << " dBm" << std::endl;
    
    auto recommendedCombination = getRecommendedTechniqueCombination();
    if (!recommendedCombination.empty()) {
        oss << "推荐技术组合: ";
        for (size_t i = 0; i < recommendedCombination.size(); ++i) {
            if (i > 0) oss << " + ";
            switch (recommendedCombination[i]) {
                case AntiJamTechnique::FREQUENCY_HOPPING: oss << "跳频"; break;
                case AntiJamTechnique::DIRECT_SEQUENCE: oss << "直扩"; break;
                case AntiJamTechnique::HYBRID_SPREAD: oss << "混合扩频"; break;
                case AntiJamTechnique::ADAPTIVE_FILTERING: oss << "自适应滤波"; break;
                case AntiJamTechnique::ERROR_CORRECTION: oss << "纠错编码"; break;
                case AntiJamTechnique::INTERFERENCE_CANCELLATION: oss << "干扰抵消"; break;
                case AntiJamTechnique::POWER_CONTROL: oss << "功率控制"; break;
                default: oss << "其他"; break;
            }
        }
        oss << std::endl;
    }
    
    return oss.str();
}

std::string CommunicationAntiJamModel::getTechniqueComparisonInfo() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    
    oss << "=== 抗干扰技术对比信息 ===" << std::endl;
    
    std::vector<AntiJamTechnique> techniques = {
        AntiJamTechnique::FREQUENCY_HOPPING,
        AntiJamTechnique::DIRECT_SEQUENCE,
        AntiJamTechnique::ADAPTIVE_FILTERING,
        AntiJamTechnique::BEAM_FORMING,
        AntiJamTechnique::ERROR_CORRECTION
    };
    
    std::vector<std::string> names = {
        "跳频", "直扩", "自适应滤波", "波束成形", "纠错编码"
    };
    
    AntiJamTechnique originalTechnique = antiJamTechnique_;
    
    for (size_t i = 0; i < techniques.size(); ++i) {
        const_cast<CommunicationAntiJamModel*>(this)->antiJamTechnique_ = techniques[i];
        double effectiveness = calculateProtectionEffectiveness();
        oss << names[i] << ": " << effectiveness * MathConstants::PERCENTAGE_MULTIPLIER << "%" << std::endl;
    }
    
    const_cast<CommunicationAntiJamModel*>(this)->antiJamTechnique_ = originalTechnique;
    
    return oss.str();
}

// 调试和测试方法
void CommunicationAntiJamModel::printAllParameters() const {
    std::cout << getParameterInfo() << std::endl;
}

void CommunicationAntiJamModel::printAntiJamAnalysis() const {
    std::cout << getParameterInfo() << std::endl;
    std::cout << getAntiJamEffectInfo() << std::endl;
    std::cout << getRecommendationInfo() << std::endl;
}

bool CommunicationAntiJamModel::runSelfTest() const {
    try {
        // 测试参数校验
        if (!validateParameters()) return false;
        
        // 测试核心计算
        double gain = calculateAntiJamGain();
        if (gain < MathConstants::MIN_GAIN_TEST || gain > MathConstants::MAX_GAIN_TEST) return false;
        
        double resistance = calculateJammerResistance();
        if (resistance < MathConstants::MIN_RESISTANCE_TEST || resistance > MathConstants::MAX_RESISTANCE_TEST) return false;
        
        double sjr = calculateSignalToJammerRatio();
        if (sjr < MathConstants::MIN_SJR_TEST || sjr > MathConstants::MAX_SJR_TEST) return false;
        
        double ber = calculateBitErrorRateWithJamming();
        if (ber < MathConstants::MIN_BER_TEST || ber > MathConstants::MAX_BER_TEST) return false;
        
        // 测试效果评估
        AntiJamEffectLevel level = evaluateAntiJamEffect();
        if (level < AntiJamEffectLevel::NO_PROTECTION || 
            level > AntiJamEffectLevel::EXCELLENT_PROTECTION) return false;
        
        return true;
    } catch (...) {
        return false;
    }
}