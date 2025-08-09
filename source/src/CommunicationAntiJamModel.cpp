#include "CommunicationAntiJamModel.h"
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iostream>

// 构造函数
CommunicationAntiJamModel::CommunicationAntiJamModel() 
    : antiJamTechnique_(AntiJamTechnique::FREQUENCY_HOPPING)
    , antiJamStrategy_(AntiJamStrategy::ADAPTIVE)
    , processingGain_(20.0)
    , spreadingFactor_(1000.0)
    , hoppingRate_(1000.0)
    , codingGain_(3.0)
    , systemBandwidth_(100.0)
    , signalPower_(-80.0)
    , noisePower_(-100.0)
    , interferenceLevel_(-70.0)
    , hoppingChannels_(100)
    , channelSpacing_(1.0)
    , dwellTime_(1.0)
    , chipRate_(10)
    , sequenceLength_(1023.0)
    , adaptationSpeed_(0.1)
    , convergenceThreshold_(0.01)
    , environmentType_(0.5)
    , jammerDensity_(0.1) {
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
    , systemBandwidth_(100.0)
    , signalPower_(-80.0)
    , noisePower_(-100.0)
    , interferenceLevel_(-70.0)
    , hoppingChannels_(100)
    , channelSpacing_(1.0)
    , dwellTime_(1.0)
    , chipRate_(10)
    , sequenceLength_(1023.0)
    , adaptationSpeed_(0.1)
    , convergenceThreshold_(0.01)
    , environmentType_(0.5)
    , jammerDensity_(0.1) {
}

// 参数校验
bool CommunicationAntiJamModel::validateParameters() const {
    if (processingGain_ < 0.0 || processingGain_ > 50.0) return false;
    if (spreadingFactor_ < 1.0 || spreadingFactor_ > 100000.0) return false;
    if (hoppingRate_ < 1.0 || hoppingRate_ > 100000.0) return false;
    if (codingGain_ < 0.0 || codingGain_ > 20.0) return false;
    if (systemBandwidth_ < 0.1 || systemBandwidth_ > 10000.0) return false;
    if (signalPower_ < -150.0 || signalPower_ > 50.0) return false;
    if (noisePower_ < -150.0 || noisePower_ > 0.0) return false;
    if (interferenceLevel_ < -150.0 || interferenceLevel_ > 50.0) return false;
    if (hoppingChannels_ < 2 || hoppingChannels_ > 10000) return false;
    if (channelSpacing_ < 0.001 || channelSpacing_ > 1000.0) return false;
    if (dwellTime_ < 0.001 || dwellTime_ > 1000.0) return false;
    if (chipRate_ < 1 || chipRate_ > 1000) return false;
    if (sequenceLength_ < 7.0 || sequenceLength_ > 1000000.0) return false;
    if (adaptationSpeed_ < 0.001 || adaptationSpeed_ > 1.0) return false;
    if (convergenceThreshold_ < 0.0001 || convergenceThreshold_ > 0.1) return false;
    if (environmentType_ < 0.0 || environmentType_ > 1.0) return false;
    if (jammerDensity_ < 0.0 || jammerDensity_ > 1.0) return false;
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
    if (gain < 0.0 || gain > 50.0) return false;
    processingGain_ = gain;
    return true;
}

bool CommunicationAntiJamModel::setSpreadingFactor(double factor) {
    if (factor < 1.0 || factor > 100000.0) return false;
    spreadingFactor_ = factor;
    return true;
}

bool CommunicationAntiJamModel::setHoppingRate(double rate) {
    if (rate < 1.0 || rate > 100000.0) return false;
    hoppingRate_ = rate;
    return true;
}

bool CommunicationAntiJamModel::setCodingGain(double gain) {
    if (gain < 0.0 || gain > 20.0) return false;
    codingGain_ = gain;
    return true;
}

bool CommunicationAntiJamModel::setSystemBandwidth(double bandwidth) {
    if (bandwidth < 0.1 || bandwidth > 10000.0) return false;
    systemBandwidth_ = bandwidth;
    return true;
}

bool CommunicationAntiJamModel::setSignalPower(double power) {
    if (power < -150.0 || power > 50.0) return false;
    signalPower_ = power;
    return true;
}

bool CommunicationAntiJamModel::setNoisePower(double power) {
    if (power < -150.0 || power > 0.0) return false;
    noisePower_ = power;
    return true;
}

bool CommunicationAntiJamModel::setInterferenceLevel(double level) {
    if (level < -150.0 || level > 50.0) return false;
    interferenceLevel_ = level;
    return true;
}

bool CommunicationAntiJamModel::setHoppingChannels(int channels) {
    if (channels < 2 || channels > 10000) return false;
    hoppingChannels_ = channels;
    return true;
}

bool CommunicationAntiJamModel::setChannelSpacing(double spacing) {
    if (spacing < 0.001 || spacing > 1000.0) return false;
    channelSpacing_ = spacing;
    return true;
}

bool CommunicationAntiJamModel::setDwellTime(double time) {
    if (time < 0.001 || time > 1000.0) return false;
    dwellTime_ = time;
    return true;
}

bool CommunicationAntiJamModel::setChipRate(int rate) {
    if (rate < 1 || rate > 1000) return false;
    chipRate_ = rate;
    return true;
}

bool CommunicationAntiJamModel::setSequenceLength(double length) {
    if (length < 7.0 || length > 1000000.0) return false;
    sequenceLength_ = length;
    return true;
}

bool CommunicationAntiJamModel::setAdaptationSpeed(double speed) {
    if (speed < 0.001 || speed > 1.0) return false;
    adaptationSpeed_ = speed;
    return true;
}

bool CommunicationAntiJamModel::setConvergenceThreshold(double threshold) {
    if (threshold < 0.0001 || threshold > 0.1) return false;
    convergenceThreshold_ = threshold;
    return true;
}

bool CommunicationAntiJamModel::setEnvironmentType(double type) {
    if (type < 0.0 || type > 1.0) return false;
    environmentType_ = type;
    return true;
}

bool CommunicationAntiJamModel::setJammerDensity(double density) {
    if (density < 0.0 || density > 1.0) return false;
    jammerDensity_ = density;
    return true;
}

// 内部计算方法
double CommunicationAntiJamModel::calculateFrequencyHoppingGain() const {
    // 跳频增益 = 10 * log10(跳频信道数)
    return 10.0 * std::log10(static_cast<double>(hoppingChannels_));
}

double CommunicationAntiJamModel::calculateDirectSequenceGain() const {
    // 直接序列扩频增益 = 10 * log10(扩频因子)
    return 10.0 * std::log10(spreadingFactor_);
}

double CommunicationAntiJamModel::calculateTimeHoppingGain() const {
    // 跳时增益基于跳时速率和驻留时间
    double timeSlots = 1000.0 / dwellTime_; // 每秒时隙数
    return 10.0 * std::log10(timeSlots);
}

double CommunicationAntiJamModel::calculateAdaptiveFilteringGain() const {
    // 自适应滤波增益基于自适应速度和收敛性能
    double adaptiveGain = 5.0 + 10.0 * adaptationSpeed_ - 20.0 * convergenceThreshold_;
    return std::max(0.0, std::min(15.0, adaptiveGain));
}

double CommunicationAntiJamModel::calculateBeamFormingGain() const {
    // 波束成形增益，假设基于系统带宽
    double beamGain = 3.0 + 0.01 * systemBandwidth_;
    return std::max(0.0, std::min(20.0, beamGain));
}

double CommunicationAntiJamModel::calculateDiversityGain() const {
    // 分集增益，基于环境类型
    double diversityGain = 2.0 + 8.0 * environmentType_;
    return std::max(0.0, std::min(10.0, diversityGain));
}

double CommunicationAntiJamModel::calculateErrorCorrectionGain() const {
    // 纠错编码增益
    return codingGain_;
}

double CommunicationAntiJamModel::calculateInterferenceCancellationGain() const {
    // 干扰抵消增益，基于干扰机密度
    double cancellationGain = 10.0 * (1.0 - jammerDensity_);
    return std::max(0.0, std::min(15.0, cancellationGain));
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
            totalGain += (calculateFrequencyHoppingGain() + calculateDirectSequenceGain()) * 0.7;
            break;
        case AntiJamTechnique::ADAPTIVE_FILTERING:
            totalGain += calculateAdaptiveFilteringGain();
            break;
        case AntiJamTechnique::BEAM_FORMING:
            totalGain += calculateBeamFormingGain();
            break;
        case AntiJamTechnique::POWER_CONTROL:
            totalGain += 3.0; // 固定功率控制增益
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
            totalGain *= 0.8; // 被动策略效果较低
            break;
        case AntiJamStrategy::ACTIVE:
            totalGain *= 1.0; // 主动策略标准效果
            break;
        case AntiJamStrategy::ADAPTIVE:
            totalGain *= 1.2; // 自适应策略效果较好
            break;
        case AntiJamStrategy::COOPERATIVE:
            totalGain *= 1.3; // 协作策略效果更好
            break;
        case AntiJamStrategy::COGNITIVE:
            totalGain *= 1.4; // 认知策略效果最好
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
    double resistance = signalToJammer / (signalToNoise + 1.0);
    return std::max(0.0, std::min(1.0, resistance / 20.0));
}

double CommunicationAntiJamModel::calculateSignalToJammerRatio() const {
    if (!validateParameters()) return -100.0;
    
    double antiJamGain = calculateAntiJamGain();
    return signalPower_ - interferenceLevel_ + antiJamGain;
}

double CommunicationAntiJamModel::calculateBitErrorRateWithJamming() const {
    if (!validateParameters()) return 1.0;
    
    double sjr = calculateSignalToJammerRatio();
    double snr = signalPower_ - noisePower_;
    
    // 综合信噪比（考虑噪声和干扰）
    double totalNoise = std::max(noisePower_, interferenceLevel_ - calculateAntiJamGain());
    double effectiveSnr = signalPower_ - totalNoise;
    
    // 简化的误码率计算（BPSK调制）
    double linearSnr = std::pow(10.0, effectiveSnr / 10.0);
    double ber = 0.5 * std::erfc(std::sqrt(linearSnr));
    
    return std::max(1e-10, std::min(0.5, ber));
}

double CommunicationAntiJamModel::calculateThroughputDegradation() const {
    if (!validateParameters()) return 1.0;
    
    double ber = calculateBitErrorRateWithJamming();
    double degradation = 1.0 - std::exp(-10.0 * ber);
    
    return std::max(0.0, std::min(1.0, degradation));
}

double CommunicationAntiJamModel::calculateDetectionProbability() const {
    if (!validateParameters()) return 1.0;
    
    double sjr = calculateSignalToJammerRatio();
    
    // 检测概率基于信干比
    double detectionProb = 1.0 / (1.0 + std::exp(-(sjr + 10.0) / 5.0));
    
    return std::max(0.0, std::min(1.0, detectionProb));
}

double CommunicationAntiJamModel::calculateInterceptionResistance() const {
    if (!validateParameters()) return 0.0;
    
    double antiJamGain = calculateAntiJamGain();
    
    // 抗截获能力主要基于扩频增益和跳频特性
    double resistance = 0.0;
    
    switch (antiJamTechnique_) {
        case AntiJamTechnique::FREQUENCY_HOPPING:
            resistance = 0.8 + 0.2 * std::min(1.0, hoppingRate_ / 10000.0);
            break;
        case AntiJamTechnique::DIRECT_SEQUENCE:
            resistance = 0.7 + 0.3 * std::min(1.0, spreadingFactor_ / 10000.0);
            break;
        case AntiJamTechnique::TIME_HOPPING:
            resistance = 0.6 + 0.4 * std::min(1.0, 1.0 / dwellTime_);
            break;
        case AntiJamTechnique::HYBRID_SPREAD:
            resistance = 0.9;
            break;
        default:
            resistance = 0.3 + 0.02 * antiJamGain;
            break;
    }
    
    return std::max(0.0, std::min(1.0, resistance));
}

// 抗干扰效果评估
AntiJamEffectLevel CommunicationAntiJamModel::evaluateAntiJamEffect() const {
    double effectiveness = calculateProtectionEffectiveness();
    
    if (effectiveness < 0.2) return AntiJamEffectLevel::NO_PROTECTION;
    else if (effectiveness < 0.4) return AntiJamEffectLevel::LOW_PROTECTION;
    else if (effectiveness < 0.6) return AntiJamEffectLevel::MEDIUM_PROTECTION;
    else if (effectiveness < 0.8) return AntiJamEffectLevel::HIGH_PROTECTION;
    else return AntiJamEffectLevel::EXCELLENT_PROTECTION;
}

double CommunicationAntiJamModel::calculateProtectionEffectiveness() const {
    if (!validateParameters()) return 0.0;
    
    double resistance = calculateJammerResistance();
    double interceptionResistance = calculateInterceptionResistance();
    double throughputMaintenance = 1.0 - calculateThroughputDegradation();
    
    // 综合保护有效性
    double effectiveness = (resistance * 0.4 + interceptionResistance * 0.3 + throughputMaintenance * 0.3);
    
    return std::max(0.0, std::min(1.0, effectiveness));
}

double CommunicationAntiJamModel::calculateAdaptationEfficiency() const {
    if (!validateParameters()) return 0.0;
    
    if (antiJamStrategy_ != AntiJamStrategy::ADAPTIVE && 
        antiJamStrategy_ != AntiJamStrategy::COGNITIVE) {
        return 0.0; // 非自适应策略
    }
    
    // 自适应效率基于自适应速度和收敛阈值
    double efficiency = adaptationSpeed_ * (1.0 - convergenceThreshold_);
    
    return std::max(0.0, std::min(1.0, efficiency));
}

double CommunicationAntiJamModel::calculateResourceUtilization() const {
    if (!validateParameters()) return 0.0;
    
    double bandwidthUtilization = std::min(1.0, systemBandwidth_ / 1000.0);
    double powerUtilization = std::min(1.0, (signalPower_ + 100.0) / 150.0);
    double processingUtilization = std::min(1.0, processingGain_ / 50.0);
    
    return (bandwidthUtilization + powerUtilization + processingUtilization) / 3.0;
}

// 特定技术效果计算
double CommunicationAntiJamModel::calculateFrequencyHoppingEffectiveness() const {
    if (antiJamTechnique_ != AntiJamTechnique::FREQUENCY_HOPPING) return 0.0;
    
    double hoppingGain = calculateFrequencyHoppingGain();
    double effectiveness = hoppingGain / 30.0; // 归一化到0-1
    
    return std::max(0.0, std::min(1.0, effectiveness));
}

double CommunicationAntiJamModel::calculateSpreadSpectrumEffectiveness() const {
    if (antiJamTechnique_ != AntiJamTechnique::DIRECT_SEQUENCE) return 0.0;
    
    double spreadGain = calculateDirectSequenceGain();
    double effectiveness = spreadGain / 40.0; // 归一化到0-1
    
    return std::max(0.0, std::min(1.0, effectiveness));
}

double CommunicationAntiJamModel::calculateAdaptiveFilteringEffectiveness() const {
    if (antiJamTechnique_ != AntiJamTechnique::ADAPTIVE_FILTERING) return 0.0;
    
    double filterGain = calculateAdaptiveFilteringGain();
    double effectiveness = filterGain / 15.0; // 归一化到0-1
    
    return std::max(0.0, std::min(1.0, effectiveness));
}

double CommunicationAntiJamModel::calculateBeamFormingEffectiveness() const {
    if (antiJamTechnique_ != AntiJamTechnique::BEAM_FORMING) return 0.0;
    
    double beamGain = calculateBeamFormingGain();
    double effectiveness = beamGain / 20.0; // 归一化到0-1
    
    return std::max(0.0, std::min(1.0, effectiveness));
}

double CommunicationAntiJamModel::calculateDiversityEffectiveness() const {
    if (antiJamTechnique_ != AntiJamTechnique::DIVERSITY_RECEPTION) return 0.0;
    
    double diversityGain = calculateDiversityGain();
    double effectiveness = diversityGain / 10.0; // 归一化到0-1
    
    return std::max(0.0, std::min(1.0, effectiveness));
}

double CommunicationAntiJamModel::calculateErrorCorrectionEffectiveness() const {
    if (antiJamTechnique_ != AntiJamTechnique::ERROR_CORRECTION) return 0.0;
    
    double codingEffectiveness = codingGain_ / 20.0; // 归一化到0-1
    
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
    double optimalGain = interferenceMargin + 10.0; // 10dB余量
    
    return std::max(0.0, std::min(50.0, optimalGain));
}

double CommunicationAntiJamModel::calculateOptimalHoppingRate() const {
    if (antiJamTechnique_ != AntiJamTechnique::FREQUENCY_HOPPING) return hoppingRate_;
    
    // 最优跳频速率基于干扰机密度和系统带宽
    double optimalRate = 1000.0 * (1.0 + jammerDensity_) * (systemBandwidth_ / 100.0);
    
    return std::max(1.0, std::min(100000.0, optimalRate));
}

int CommunicationAntiJamModel::calculateOptimalHoppingChannels() const {
    if (antiJamTechnique_ != AntiJamTechnique::FREQUENCY_HOPPING) return hoppingChannels_;
    
    // 最优跳频信道数基于系统带宽和信道间隔
    int optimalChannels = static_cast<int>(systemBandwidth_ / channelSpacing_);
    
    return std::max(2, std::min(10000, optimalChannels));
}

// 多技术组合效果
double CommunicationAntiJamModel::calculateCombinedTechniqueEffect(const std::vector<AntiJamTechnique>& techniques) const {
    if (techniques.empty()) return 0.0;
    
    double combinedGain = 0.0;
    double synergy = 1.0;
    
    for (auto technique : techniques) {
        AntiJamTechnique originalTechnique = antiJamTechnique_;
        const_cast<CommunicationAntiJamModel*>(this)->antiJamTechnique_ = technique;
        
        double gain = calculateAntiJamGain();
        combinedGain += gain * synergy;
        synergy *= 0.8; // 协同效应递减
        
        const_cast<CommunicationAntiJamModel*>(this)->antiJamTechnique_ = originalTechnique;
    }
    
    return combinedGain;
}

std::vector<AntiJamTechnique> CommunicationAntiJamModel::getRecommendedTechniqueCombination() const {
    std::vector<AntiJamTechnique> recommended;
    
    // 基于环境和威胁特征推荐技术组合
    if (jammerDensity_ > 0.7) {
        // 高威胁环境
        recommended.push_back(AntiJamTechnique::HYBRID_SPREAD);
        recommended.push_back(AntiJamTechnique::ADAPTIVE_FILTERING);
        recommended.push_back(AntiJamTechnique::INTERFERENCE_CANCELLATION);
    } else if (jammerDensity_ > 0.3) {
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
    
    double performance = 1.0 - calculateThroughputDegradation();
    
    const_cast<CommunicationAntiJamModel*>(this)->interferenceLevel_ = originalInterference;
    
    return performance;
}

double CommunicationAntiJamModel::calculateRequiredAntiJamGain(double targetBER) const {
    if (targetBER <= 0.0 || targetBER >= 0.5) return 0.0;
    
    // 基于目标误码率计算所需抗干扰增益
    double requiredSnr = -10.0 * std::log10(2.0 * targetBER);
    double currentSnr = signalPower_ - std::max(noisePower_, interferenceLevel_);
    double requiredGain = requiredSnr - currentSnr;
    
    return std::max(0.0, requiredGain);
}

double CommunicationAntiJamModel::calculateMaxTolerableJammerPower() const {
    double antiJamGain = calculateAntiJamGain();
    double minRequiredSjr = 10.0; // 最小所需信干比
    
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
    oss << "抗干扰能力: " << calculateJammerResistance() * 100 << "%" << std::endl;
    oss << "信干比: " << calculateSignalToJammerRatio() << " dB" << std::endl;
    oss << "有干扰误码率: " << calculateBitErrorRateWithJamming() << std::endl;
    oss << "吞吐量下降: " << calculateThroughputDegradation() * 100 << "%" << std::endl;
    oss << "检测概率: " << calculateDetectionProbability() * 100 << "%" << std::endl;
    oss << "抗截获能力: " << calculateInterceptionResistance() * 100 << "%" << std::endl;
    oss << "保护有效性: " << calculateProtectionEffectiveness() * 100 << "%" << std::endl;
    
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
        oss << names[i] << ": " << effectiveness * 100 << "%" << std::endl;
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
        if (gain < 0.0 || gain > 100.0) return false;
        
        double resistance = calculateJammerResistance();
        if (resistance < 0.0 || resistance > 1.0) return false;
        
        double sjr = calculateSignalToJammerRatio();
        if (sjr < -200.0 || sjr > 200.0) return false;
        
        double ber = calculateBitErrorRateWithJamming();
        if (ber < 0.0 || ber > 1.0) return false;
        
        // 测试效果评估
        AntiJamEffectLevel level = evaluateAntiJamEffect();
        if (level < AntiJamEffectLevel::NO_PROTECTION || 
            level > AntiJamEffectLevel::EXCELLENT_PROTECTION) return false;
        
        return true;
    } catch (...) {
        return false;
    }
}