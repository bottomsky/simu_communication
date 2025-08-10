#include "CommunicationModelAPI.h"
#include "MathConstants.h"
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <chrono>


#include <thread>
#include <random>

// 构造函数
CommunicationModelAPI::CommunicationModelAPI() 
    : currentScenario_(CommunicationScenario::NORMAL_COMMUNICATION)
    , resultsValid_(false) {
    
    // 初始化所有模型
    signalModel_ = std::make_unique<SignalTransmissionModel>();
    distanceModel_ = std::make_unique<CommunicationDistanceModel>();
    receiveModel_ = std::make_unique<CommunicationReceiveModel>();
    jammerModel_ = std::make_unique<CommunicationJammerModel>();
    antiJamModel_ = std::make_unique<CommunicationAntiJamModel>();
    
    // 设置默认环境参数
    environment_.frequency = MathConstants::DEFAULT_FREQUENCY;
    environment_.bandwidth = MathConstants::DEFAULT_BANDWIDTH;
    environment_.transmitPower = MathConstants::DEFAULT_TRANSMIT_POWER;
    environment_.noisePower = MathConstants::DEFAULT_NOISE_POWER;
    environment_.distance = MathConstants::DEFAULT_DISTANCE;
    environment_.environmentType = EnvironmentType::OPEN_FIELD;
    environment_.temperature = MathConstants::DEFAULT_TEMPERATURE;
    environment_.humidity = MathConstants::DEFAULT_HUMIDITY;
    environment_.atmosphericPressure = MathConstants::DEFAULT_ATMOSPHERIC_PRESSURE;
    
    // 设置默认干扰环境
    jammingEnv_.isJammed = false;
    jammingEnv_.jammerType = JammerType::GAUSSIAN_NOISE;
    jammingEnv_.jammerPower = MathConstants::DEFAULT_JAMMER_POWER;
    jammingEnv_.jammerFrequency = MathConstants::DEFAULT_FREQUENCY;
    jammingEnv_.jammerBandwidth = MathConstants::DEFAULT_JAMMER_BANDWIDTH;
    jammingEnv_.jammerDistance = MathConstants::DEFAULT_JAMMER_DISTANCE;
    jammingEnv_.jammerDensity = MathConstants::DEFAULT_JAMMER_DENSITY;
    
    updateModelsFromEnvironment();
}

CommunicationModelAPI::CommunicationModelAPI(CommunicationScenario scenario) 
    : CommunicationModelAPI() {
    setScenario(scenario);
}

CommunicationModelAPI::~CommunicationModelAPI() = default;

// 内部方法
void CommunicationModelAPI::invalidateCache() {
    resultsValid_ = false;
}

/// @brief 更新所有模型参数
/// @details 根据当前环境参数更新信号传输模型、通信距离模型、接收模型、干扰模型和抗干扰模型
/// @return 更新是否成功
/// @retval true 更新成功
/// @retval false 更新失败
void CommunicationModelAPI::updateModelsFromEnvironment() {
    if (!signalModel_ || !distanceModel_ || !receiveModel_ || 
        !jammerModel_ || !antiJamModel_) return;
    
    // 更新信号传输模型
    signalModel_->setCenterFrequency(environment_.frequency);
    signalModel_->setSignalBandwidth(environment_.bandwidth);
    signalModel_->setTransmitPower(environment_.transmitPower);
    
    // 更新通信距离模型
    distanceModel_->setEnvironmentType(environment_.environmentType);
    distanceModel_->setTransmitPower(environment_.transmitPower);
    
    // 更新接收模型
    receiveModel_->setSystemBandwidth(environment_.bandwidth);
    receiveModel_->setReceivedPower(environment_.noisePower);
    
    // 更新干扰模型
    if (jammingEnv_.isJammed) {
        jammerModel_->setJammerType(jammingEnv_.jammerType);
        jammerModel_->setJammerPower(jammingEnv_.jammerPower);
        jammerModel_->setJammerFrequency(jammingEnv_.jammerFrequency);
        jammerModel_->setJammerBandwidth(jammingEnv_.jammerBandwidth);
        jammerModel_->setTargetFrequency(environment_.frequency);
        jammerModel_->setTargetDistance(jammingEnv_.jammerDistance);
    }
    
    // 更新抗干扰模型
    antiJamModel_->setSystemBandwidth(environment_.bandwidth);
    antiJamModel_->setSignalPower(environment_.transmitPower);
    antiJamModel_->setNoisePower(environment_.noisePower);
    if (jammingEnv_.isJammed) {
        antiJamModel_->setInterferenceLevel(jammingEnv_.jammerPower);
    }
    
    invalidateCache();
}

/// @brief 计算总信号强度
/// @details 总信号强度 = 发射功率 - 路径损耗 - 环境损耗 - 频率因子
/// @return 总信号强度(dBm)
double CommunicationModelAPI::calculateOverallSignalStrength() const {
    if (!signalModel_ || !distanceModel_) return MathConstants::DEFAULT_SIGNAL_STRENGTH;
    
    // 基础信号强度
    double baseSignal = environment_.transmitPower;
    
    // 使用距离模型计算总路径损耗（包含所有损耗因子）
    double totalPathLoss = distanceModel_->calculateTotalPathLoss(environment_.distance, environment_.frequency);
    
    return baseSignal - totalPathLoss;
}

/// @brief 计算信噪比
/// @details 信噪比 = 总信号强度 - 环境噪声功率
/// @return 信噪比(dB)
double CommunicationModelAPI::calculateOverallSNR() const {
    if (!receiveModel_) return MathConstants::DEFAULT_SNR;
    
    double signalStrength = calculateOverallSignalStrength();
    double snr = signalStrength - environment_.noisePower;
    
    // 考虑干扰的影响
    if (jammingEnv_.isJammed && jammerModel_) {
        double jammerEffect = jammerModel_->calculateJammerToSignalRatio();
        snr -= jammerEffect;
    }
    
    // 考虑抗干扰增益
    if (currentScenario_ == CommunicationScenario::ANTI_JAM_COMMUNICATION && antiJamModel_) {
        double antiJamGain = antiJamModel_->calculateAntiJamGain();
        snr += antiJamGain;
    }
    
    return snr;
}

/// @brief 计算误码率
/// @details 误码率 = 0.5 * erfc(sqrt(10^(信噪比/10)))
/// @return 误码率
double CommunicationModelAPI::calculateOverallBER() const {
    double snr = calculateOverallSNR();
    
    // 简化的BPSK误码率计算
    double linearSnr = std::pow(MathConstants::LINEAR_TO_DB_MULTIPLIER, snr / MathConstants::LINEAR_TO_DB_MULTIPLIER);
    double ber = MathConstants::BER_COEFFICIENT * std::erfc(std::sqrt(linearSnr));
    
    return std::max(MathConstants::MIN_BER, std::min(MathConstants::MAX_BER, ber));
}

/// @brief 计算吞吐量
/// @details 吞吐量 = 带宽 * log2(1 + 10^(信噪比/10)) * (1 - 误码率)
/// @return 吞吐量 (bps)
double CommunicationModelAPI::calculateThroughput() const {
    double snr = calculateOverallSNR();
    double ber = calculateOverallBER();
    
    // 基于香农定理的理论容量
    double theoreticalCapacity = environment_.bandwidth * std::log2(MathConstants::SHANNON_BASE + std::pow(MathConstants::LINEAR_TO_DB_MULTIPLIER, snr / MathConstants::LINEAR_TO_DB_MULTIPLIER));
    
    // 考虑误码率的影响
    double efficiency = MathConstants::EFFICIENCY_BASE - MathConstants::BER_EFFICIENCY_FACTOR * ber; // 简化的效率模型
    efficiency = std::max(MathConstants::MIN_EFFICIENCY, std::min(MathConstants::MAX_EFFICIENCY, efficiency));
    
    return theoreticalCapacity * efficiency;
}

/// @brief 计算延迟
/// @details 延迟 = 距离 / 光速 + 处理延迟 + 重传延迟
/// @return 延迟 (ms)
double CommunicationModelAPI::calculateLatency() const {
    double baseLatency = environment_.distance / MathConstants::SPEED_OF_LIGHT; // 光速传播延迟 (ms)
    
    // 处理延迟
    double processingDelay = MathConstants::DEFAULT_PROCESSING_DELAY; // 基础处理延迟 1ms
    
    // 重传延迟（基于误码率）
    double ber = calculateOverallBER();
    double retransmissionDelay = ber * MathConstants::RETRANSMISSION_DELAY_FACTOR; // 简化模型
    
    return baseLatency + processingDelay + retransmissionDelay;
}

/// @brief 计算丢包率
/// @details 丢包率 = 1 - (1 - BER)^(数据包长度 * 8)
/// @return 丢包率
double CommunicationModelAPI::calculatePacketLossRate() const {
    double ber = calculateOverallBER();
    
    // 假设数据包长度为1000字节
    int packetLengthBits = MathConstants::PACKET_LENGTH_BITS;
    
    // 数据包错误率 = 1 - (1 - BER)^数据包长度
    double packetErrorRate = MathConstants::UNITY - std::pow(MathConstants::UNITY - ber, packetLengthBits);
    
    return std::max(MathConstants::ZERO, std::min(MathConstants::UNITY, packetErrorRate));
}
/// @return 通信质量
CommunicationQuality CommunicationModelAPI::assessCommunicationQuality() const {
    double snr = calculateOverallSNR();
    double ber = calculateOverallBER();
    double packetLoss = calculatePacketLossRate();
    
    // 综合评估通信质量
    int score = 0;
    
    // 信噪比评分
    if (snr > MathConstants::HIGH_SNR_THRESHOLD) score += 2;
    else if (snr > MathConstants::MEDIUM_SNR_THRESHOLD) score += 1;
    else if (snr < MathConstants::LOW_SNR_THRESHOLD) score -= 1;
    
    // 误码率评分
    if (ber < MathConstants::EXCELLENT_BER_THRESHOLD) score += 2;
    else if (ber < MathConstants::GOOD_BER_THRESHOLD) score += 1;
    else if (ber > MathConstants::POOR_BER_THRESHOLD_API) score -= 1;
    
    // 丢包率评分
    if (packetLoss < MathConstants::LOW_PACKET_LOSS_THRESHOLD) score += 1;
    else if (packetLoss > MathConstants::HIGH_PACKET_LOSS_THRESHOLD) score -= 1;
    
    // 根据总分确定质量等级
    if (score >= MathConstants::EXCELLENT_QUALITY_SCORE) return CommunicationQuality::EXCELLENT;
    else if (score >= MathConstants::GOOD_QUALITY_SCORE) return CommunicationQuality::GOOD;
    else if (score >= MathConstants::FAIR_QUALITY_SCORE) return CommunicationQuality::FAIR;
    else if (score >= MathConstants::POOR_QUALITY_SCORE) return CommunicationQuality::POOR;
    else return CommunicationQuality::FAILED;
}

/// @brief 设置通信场景
/// @details 根据场景调整模型参数
/// @param scenario 通信场景
/// @return 设置是否成功
bool CommunicationModelAPI::setScenario(CommunicationScenario scenario) {
    currentScenario_ = scenario;
    
    // 根据场景调整模型参数
    switch (scenario) {
        case CommunicationScenario::NORMAL_COMMUNICATION:
            jammingEnv_.isJammed = false;
            break;
        case CommunicationScenario::JAMMED_COMMUNICATION:
            jammingEnv_.isJammed = true;
            break;
        case CommunicationScenario::ANTI_JAM_COMMUNICATION:
            jammingEnv_.isJammed = true;
            if (antiJamModel_) {
                antiJamModel_->setAntiJamStrategy(AntiJamStrategy::ADAPTIVE);
            }
            break;
        case CommunicationScenario::MULTI_USER_COMMUNICATION:
            // 多用户场景的特殊设置
            break;
        case CommunicationScenario::RELAY_COMMUNICATION:
            // 中继通信的特殊设置
            break;
        case CommunicationScenario::MESH_COMMUNICATION:
            // 网状通信的特殊设置
            break;
    }
    
    updateModelsFromEnvironment();
    return true;
}

// 环境参数设置
bool CommunicationModelAPI::setEnvironment(const CommunicationEnvironment& env) {
    environment_ = env;
    updateModelsFromEnvironment();
    return true;
}

bool CommunicationModelAPI::setJammingEnvironment(const JammingEnvironment& jammingEnv) {
    jammingEnv_ = jammingEnv;
    updateModelsFromEnvironment();
    return true;
}

bool CommunicationModelAPI::setFrequency(double frequency) {
    if (frequency < MathConstants::FREQUENCY_VALIDATION_MIN || frequency > MathConstants::FREQUENCY_VALIDATION_MAX) return false;
    environment_.frequency = frequency;
    updateModelsFromEnvironment();
    return true;
}

bool CommunicationModelAPI::setBandwidth(double bandwidth) {
    if (bandwidth < MathConstants::BANDWIDTH_VALIDATION_MIN || bandwidth > MathConstants::BANDWIDTH_VALIDATION_MAX) return false;
    environment_.bandwidth = bandwidth;
    updateModelsFromEnvironment();
    return true;
}

bool CommunicationModelAPI::setTransmitPower(double power) {
    if (power < MathConstants::POWER_VALIDATION_MIN || power > MathConstants::POWER_VALIDATION_MAX) return false;
    environment_.transmitPower = power;
    updateModelsFromEnvironment();
    return true;
}

bool CommunicationModelAPI::setDistance(double distance) {
    if (distance < MathConstants::DISTANCE_VALIDATION_MIN || distance > MathConstants::DISTANCE_VALIDATION_MAX) return false;
    environment_.distance = distance;
    updateModelsFromEnvironment();
    return true;
}

bool CommunicationModelAPI::setEnvironmentType(EnvironmentType type) {
    environment_.environmentType = type;
    updateModelsFromEnvironment();
    return true;
}

// 核心计算接口
CommunicationLinkStatus CommunicationModelAPI::calculateLinkStatus() const {
    if (resultsValid_) {
        return cachedLinkStatus_;
    }
    
    CommunicationLinkStatus status;
    
    status.signalStrength = calculateOverallSignalStrength();
    status.signalToNoiseRatio = calculateOverallSNR();
    status.bitErrorRate = calculateOverallBER();
    status.throughput = calculateThroughput();
    status.latency = calculateLatency();
    status.packetLossRate = calculatePacketLossRate();
    status.quality = assessCommunicationQuality();
    
    // 判断是否连接
    status.isConnected = (status.signalToNoiseRatio > MathConstants::LOW_SNR_THRESHOLD && 
                         status.bitErrorRate < MathConstants::CONNECTION_BER_THRESHOLD && 
                         status.packetLossRate < MathConstants::CONNECTION_PACKET_LOSS_THRESHOLD);
    
    // 生成状态描述
    std::ostringstream oss;
    oss << "信号强度: " << std::fixed << std::setprecision(1) << status.signalStrength << " dBm, ";
    oss << "信噪比: " << status.signalToNoiseRatio << " dB, ";
    oss << "误码率: " << std::scientific << std::setprecision(2) << status.bitErrorRate << ", ";
    oss << "吞吐量: " << std::fixed << std::setprecision(2) << status.throughput << " Mbps";
    status.statusDescription = oss.str();
    
    // 缓存结果
    cachedLinkStatus_ = status;
    resultsValid_ = true;
    
    return status;
}

CommunicationPerformance CommunicationModelAPI::calculatePerformance() const {
    CommunicationPerformance performance;
    
    // 有效通信距离
    if (distanceModel_) {
        performance.effectiveRange = distanceModel_->calculateEffectiveDistance();
    } else {
        performance.effectiveRange = 0.0;
    }
    
    // 最大数据速率
    performance.maxDataRate = calculateThroughput();
    
    // 功率效率 (bps/W)
    double powerWatts = std::pow(MathConstants::LINEAR_TO_DB_MULTIPLIER, (environment_.transmitPower - MathConstants::POWER_CONVERSION_OFFSET) / MathConstants::LINEAR_TO_DB_MULTIPLIER);
    performance.powerEfficiency = (performance.maxDataRate * MathConstants::MEGA_MULTIPLIER) / powerWatts;
    
    // 频谱效率 (bps/Hz)
    performance.spectralEfficiency = (performance.maxDataRate * MathConstants::MEGA_MULTIPLIER) / (environment_.bandwidth * MathConstants::MEGA_MULTIPLIER);
    
    // 可靠性（基于误码率）
    double ber = calculateOverallBER();
    performance.reliability = MathConstants::UNITY - std::min(MathConstants::UNITY, ber * MathConstants::RELIABILITY_MULTIPLIER);
    
    // 可用性（基于信噪比）
    double snr = calculateOverallSNR();
    performance.availability = MathConstants::UNITY / (MathConstants::UNITY + std::exp(-(snr - MathConstants::AVAILABILITY_SNR_OFFSET) / MathConstants::AVAILABILITY_DIVISOR));
    
    // 抗干扰能力
    if (antiJamModel_) {
        performance.jammerResistance = antiJamModel_->calculateJammerResistance();
    } else {
        performance.jammerResistance = 0.0;
    }
    
    // 抗截获能力
    if (antiJamModel_) {
        performance.interceptionResistance = antiJamModel_->calculateInterceptionResistance();
    } else {
        performance.interceptionResistance = 0.0;
    }
    
    return performance;
}

double CommunicationModelAPI::calculateCommunicationRange() const {
    if (!distanceModel_) return 0.0;
    return distanceModel_->calculateEffectiveDistance();
}

double CommunicationModelAPI::calculateRequiredPower(double targetRange) const {
    if (!distanceModel_) return 0.0;
    
    // 保存当前距离
    double originalDistance = environment_.distance;
    
    // 设置目标距离
    const_cast<CommunicationModelAPI*>(this)->environment_.distance = targetRange;
    const_cast<CommunicationModelAPI*>(this)->updateModelsFromEnvironment();
    
    // 使用CommunicationDistanceModel计算总路径损耗
    double totalPathLoss = distanceModel_->calculateTotalPathLoss(targetRange, environment_.frequency);
    
    double requiredPower = environment_.noisePower + 10.0 + totalPathLoss; // 10dB SNR余量
    
    // 恢复原始距离
    const_cast<CommunicationModelAPI*>(this)->environment_.distance = originalDistance;
    const_cast<CommunicationModelAPI*>(this)->updateModelsFromEnvironment();
    
    return requiredPower;
}

double CommunicationModelAPI::calculateOptimalFrequency() const {
    // 从配置获取环境特性
    const EnvironmentLossConfig& config = EnvironmentLossConfigManager::getConfig(environment_.environmentType);
    
    // 基于环境损耗特性计算最优频率
    // 频率因子越高，选择越低的频率
    double baseFrequency = 2400.0; // 基准频率 2.4 GHz
    double optimalFreq = baseFrequency / config.frequencyFactor;
    
    // 考虑路径损耗指数的影响
    if (config.pathLossExponent > 3.0) {
        optimalFreq *= 0.7; // 高损耗环境选择更低频率
    } else if (config.pathLossExponent < 2.5) {
        optimalFreq *= 1.3; // 低损耗环境可以选择更高频率
    }
    
    // 限制频率范围在合理区间内
    return std::max(400.0, std::min(6000.0, optimalFreq));
}

double CommunicationModelAPI::calculateOptimalBandwidth() const {
    // 基于所需数据速率和频谱效率计算最优带宽
    double targetDataRate = 10.0; // 目标10 Mbps
    double spectralEfficiency = 2.0; // 假设2 bps/Hz
    
    return targetDataRate / spectralEfficiency;
}

// 干扰分析接口
double CommunicationModelAPI::calculateJammerEffectiveness() const {
    if (!jammerModel_ || !jammingEnv_.isJammed) return 0.0;
    return jammerModel_->calculateJammerEffectiveness();
}

double CommunicationModelAPI::calculateAntiJamEffectiveness() const {
    if (!antiJamModel_) return 0.0;
    return antiJamModel_->calculateProtectionEffectiveness();
}

double CommunicationModelAPI::calculateJammerToSignalRatio() const {
    if (!jammerModel_ || !jammingEnv_.isJammed) return -100.0;
    return jammerModel_->calculateJammerToSignalRatio();
}

double CommunicationModelAPI::calculateRequiredAntiJamGain(double targetBER) const {
    if (!antiJamModel_) return 0.0;
    return antiJamModel_->calculateRequiredAntiJamGain(targetBER);
}

std::vector<double> CommunicationModelAPI::calculateJammerCoverage() const {
    std::vector<double> coverage;
    
    if (!jammerModel_ || !jammingEnv_.isJammed) {
        return coverage;
    }
    
    // 计算不同距离下的干扰覆盖
    for (double distance = 0.1; distance <= 100.0; distance += 0.5) {
        jammerModel_->setTargetDistance(distance);
        double effectiveness = jammerModel_->calculateJammerEffectiveness();
        coverage.push_back(effectiveness);
    }
    
    return coverage;
}

// 性能优化接口
CommunicationEnvironment CommunicationModelAPI::optimizeForRange(double targetRange) const {
    CommunicationEnvironment optimized = environment_;
    
    // 计算所需功率
    optimized.transmitPower = calculateRequiredPower(targetRange);
    
    // 选择最优频率
    optimized.frequency = calculateOptimalFrequency();
    
    return optimized;
}

CommunicationEnvironment CommunicationModelAPI::optimizeForDataRate(double targetDataRate) const {
    CommunicationEnvironment optimized = environment_;
    
    // 计算所需带宽
    double spectralEfficiency = 2.0; // 假设值
    optimized.bandwidth = targetDataRate / spectralEfficiency;
    
    // 调整功率以保证信噪比
    double requiredSNR = 10.0 * std::log10(std::pow(2.0, targetDataRate / optimized.bandwidth) - 1.0);
    optimized.transmitPower = optimized.noisePower + requiredSNR + 10.0; // 10dB余量
    
    return optimized;
}

CommunicationEnvironment CommunicationModelAPI::optimizeForPowerEfficiency() const {
    CommunicationEnvironment optimized = environment_;
    
    // 降低功率到最小可接受水平
    double minSNR = 10.0; // 最小信噪比要求
    double minTransmitPower = -45.0; // 最小发射功率限制 (dBm)
    
    // 计算基于SNR的最小功率需求
    double calculatedPower = optimized.noisePower + minSNR + 3.0; // 3dB余量
    
    // 确保功率不低于最小限制
    optimized.transmitPower = std::max(calculatedPower, minTransmitPower);
    
    // 选择功率效率最高的频率
    optimized.frequency = calculateOptimalFrequency();
    
    return optimized;
}

CommunicationEnvironment CommunicationModelAPI::optimizeForJammerResistance() const {
    CommunicationEnvironment optimized = environment_;
    
    if (antiJamModel_) {
        // 增加功率以对抗干扰
        double requiredGain = antiJamModel_->calculateRequiredAntiJamGain(1e-6);
        optimized.transmitPower += requiredGain;
        
        // 选择抗干扰性能最好的频率
        // 基于抗干扰技术计算最优频率
        double baseFreq = optimized.frequency;
        double antiJamGain = antiJamModel_->calculateAntiJamGain();
        
        // 根据抗干扰增益调整频率选择
        if (antiJamGain > 20.0) {
            optimized.frequency = baseFreq * 1.1; // 高抗干扰增益时选择稍高频率
        } else if (antiJamGain < 10.0) {
            optimized.frequency = baseFreq * 0.9; // 低抗干扰增益时选择稍低频率
        }
    }
    
    return optimized;
}

// 版本信息
std::string CommunicationModelAPI::getVersion() {
    return "1.0.0";
}

std::string CommunicationModelAPI::getBuildInfo() {
    return "Built on " __DATE__ " " __TIME__;
}

std::string CommunicationModelAPI::getModelInfo() const {
    std::ostringstream oss;
    oss << "通信模型API v" << getVersion() << std::endl;
    oss << "包含模型: 信号传输、通信距离、接收机、干扰机、抗干扰" << std::endl;
    oss << "当前场景: ";
    
    switch (currentScenario_) {
        case CommunicationScenario::NORMAL_COMMUNICATION:
            oss << "正常通信";
            break;
        case CommunicationScenario::JAMMED_COMMUNICATION:
            oss << "受干扰通信";
            break;
        case CommunicationScenario::ANTI_JAM_COMMUNICATION:
            oss << "抗干扰通信";
            break;
        case CommunicationScenario::MULTI_USER_COMMUNICATION:
            oss << "多用户通信";
            break;
        case CommunicationScenario::RELAY_COMMUNICATION:
            oss << "中继通信";
            break;
        case CommunicationScenario::MESH_COMMUNICATION:
            oss << "网状通信";
            break;
    }
    
    return oss.str();
}

bool CommunicationModelAPI::saveConfiguration(const std::string& filename) const {
    // 保存配置到文件的实现
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "# Communication Model Configuration" << std::endl;
    file << "frequency=" << environment_.frequency << std::endl;
    file << "transmitPower=" << environment_.transmitPower << std::endl;
    file << "bandwidth=" << environment_.bandwidth << std::endl;
    file << "distance=" << environment_.distance << std::endl;
    file << "environmentType=" << static_cast<int>(environment_.environmentType) << std::endl;
    
    file.close();
    return true;
}

bool CommunicationModelAPI::loadConfiguration(const std::string& filename) {
    // 从文件加载配置的实现
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            
            if (key == "frequency") {
                environment_.frequency = std::stod(value);
            } else if (key == "transmitPower") {
                environment_.transmitPower = std::stod(value);
            } else if (key == "bandwidth") {
                environment_.bandwidth = std::stod(value);
            } else if (key == "distance") {
                environment_.distance = std::stod(value);
            } else if (key == "environmentType") {
                environment_.environmentType = static_cast<EnvironmentType>(std::stoi(value));
            }
        }
    }
    
    updateModelsFromEnvironment();
    file.close();
    return true;
}

std::string CommunicationModelAPI::exportConfigurationToJSON() const {
    std::ostringstream oss;
    oss << "{" << std::endl;
    oss << "  \"frequency\": " << environment_.frequency << "," << std::endl;
    oss << "  \"transmitPower\": " << environment_.transmitPower << "," << std::endl;
    oss << "  \"bandwidth\": " << environment_.bandwidth << "," << std::endl;
    oss << "  \"distance\": " << environment_.distance << "," << std::endl;
    oss << "  \"environmentType\": " << static_cast<int>(environment_.environmentType) << std::endl;
    oss << "}" << std::endl;
    return oss.str();
}

bool CommunicationModelAPI::importConfigurationFromJSON(const std::string& jsonString) {
    // 简单的JSON解析实现
    try {
        size_t pos = 0;
        std::string json = jsonString;
        
        // 查找并解析frequency
        pos = json.find("\"frequency\":");
        if (pos != std::string::npos) {
            pos = json.find(":", pos) + 1;
            size_t end = json.find(",", pos);
            if (end == std::string::npos) end = json.find("}", pos);
            std::string value = json.substr(pos, end - pos);
            environment_.frequency = std::stod(value);
        }
        
        // 查找并解析transmitPower
        pos = json.find("\"transmitPower\":");
        if (pos != std::string::npos) {
            pos = json.find(":", pos) + 1;
            size_t end = json.find(",", pos);
            if (end == std::string::npos) end = json.find("}", pos);
            std::string value = json.substr(pos, end - pos);
            environment_.transmitPower = std::stod(value);
        }
        
        // 查找并解析bandwidth
        pos = json.find("\"bandwidth\":");
        if (pos != std::string::npos) {
            pos = json.find(":", pos) + 1;
            size_t end = json.find(",", pos);
            if (end == std::string::npos) end = json.find("}", pos);
            std::string value = json.substr(pos, end - pos);
            environment_.bandwidth = std::stod(value);
        }
        
        // 查找并解析distance
        pos = json.find("\"distance\":");
        if (pos != std::string::npos) {
            pos = json.find(":", pos) + 1;
            size_t end = json.find(",", pos);
            if (end == std::string::npos) end = json.find("}", pos);
            std::string value = json.substr(pos, end - pos);
            environment_.distance = std::stod(value);
        }
        
        // 查找并解析environmentType
        pos = json.find("\"environmentType\":");
        if (pos != std::string::npos) {
            pos = json.find(":", pos) + 1;
            size_t end = json.find(",", pos);
            if (end == std::string::npos) end = json.find("}", pos);
            std::string value = json.substr(pos, end - pos);
            environment_.environmentType = static_cast<EnvironmentType>(std::stoi(value));
        }
        
        updateModelsFromEnvironment();
        return true;
    } catch (...) {
        return false;
    }
}

std::string CommunicationModelAPI::getCapabilities() const {
    std::ostringstream oss;
    oss << "通信模型API功能列表:" << std::endl;
    oss << "- 信号传输建模" << std::endl;
    oss << "- 通信距离计算" << std::endl;
    oss << "- 接收机性能分析" << std::endl;
    oss << "- 干扰效果评估" << std::endl;
    oss << "- 抗干扰技术支持" << std::endl;
    oss << "- 多种环境类型支持" << std::endl;
    oss << "- 性能优化算法" << std::endl;
    oss << "- 网络连通性分析" << std::endl;
    oss << "- 频率范围分析" << std::endl;
    oss << "- 实时性能监控" << std::endl;
    return oss.str();
}

std::string CommunicationModelAPI::generateDetailedReport() const {
    std::ostringstream oss;
    oss << "=== 通信模型详细报告 ===" << std::endl;
    oss << "版本: " << getVersion() << std::endl;
    oss << "构建信息: " << getBuildInfo() << std::endl;
    oss << std::endl;
    
    oss << "当前环境参数:" << std::endl;
    oss << "- 频率: " << environment_.frequency << " MHz" << std::endl;
    oss << "- 带宽: " << environment_.bandwidth << " MHz" << std::endl;
    oss << "- 发射功率: " << environment_.transmitPower << " dBm" << std::endl;
    oss << "- 距离: " << environment_.distance << " km" << std::endl;
    oss << "- 环境类型: " << static_cast<int>(environment_.environmentType) << std::endl;
    oss << std::endl;
    
    auto status = calculateLinkStatus();
    oss << "链路状态:" << std::endl;
    oss << "- 信号强度: " << status.signalStrength << " dBm" << std::endl;
    oss << "- 信噪比: " << status.signalToNoiseRatio << " dB" << std::endl;
    oss << "- 误码率: " << status.bitErrorRate << std::endl;
    oss << "- 吞吐量: " << status.throughput << " bps" << std::endl;
    oss << "- 延迟: " << status.latency << " ms" << std::endl;
    oss << "- 丢包率: " << status.packetLossRate << std::endl;
    
    return oss.str();
}

std::string CommunicationModelAPI::generatePerformanceReport() const {
    std::ostringstream oss;
    oss << "=== 性能分析报告 ===" << std::endl;
    
    auto status = calculateLinkStatus();
    oss << "当前性能指标:" << std::endl;
    oss << "- 信号强度: " << status.signalStrength << " dBm" << std::endl;
    oss << "- 信噪比: " << status.signalToNoiseRatio << " dB" << std::endl;
    oss << "- 误码率: " << status.bitErrorRate << std::endl;
    oss << "- 吞吐量: " << status.throughput << " bps" << std::endl;
    oss << "- 延迟: " << status.latency << " ms" << std::endl;
    oss << "- 丢包率: " << status.packetLossRate << std::endl;
    oss << std::endl;
    
    oss << "性能评估:" << std::endl;
    if (status.signalToNoiseRatio > 20.0) {
        oss << "- 信噪比: 优秀" << std::endl;
    } else if (status.signalToNoiseRatio > 10.0) {
        oss << "- 信噪比: 良好" << std::endl;
    } else {
        oss << "- 信噪比: 需要改善" << std::endl;
    }
    
    if (status.bitErrorRate < 1e-6) {
        oss << "- 误码率: 优秀" << std::endl;
    } else if (status.bitErrorRate < 1e-3) {
        oss << "- 误码率: 良好" << std::endl;
    } else {
        oss << "- 误码率: 需要改善" << std::endl;
    }
    
    return oss.str();
}

std::string CommunicationModelAPI::generateJammingAnalysisReport() const {
    std::ostringstream oss;
    oss << "=== 干扰分析报告 ===" << std::endl;
    
    if (jammingEnv_.isJammed) {
        oss << "干扰状态: 存在干扰" << std::endl;
        oss << "干扰类型: " << static_cast<int>(jammingEnv_.jammerType) << std::endl;
        oss << "干扰功率: " << jammingEnv_.jammerPower << " dBm" << std::endl;
        oss << "干扰频率: " << jammingEnv_.jammerFrequency << " MHz" << std::endl;
        oss << "干扰距离: " << jammingEnv_.jammerDistance << " km" << std::endl;
        oss << std::endl;
        
        double jammerEffect = calculateJammerEffectiveness();
        oss << "干扰效果分析:" << std::endl;
        oss << "- 干扰有效性: " << jammerEffect << std::endl;
        
        double jsr = calculateJammerToSignalRatio();
        oss << "- 干信比: " << jsr << " dB" << std::endl;
        
        if (currentScenario_ == CommunicationScenario::ANTI_JAM_COMMUNICATION) {
            double antiJamEffect = calculateAntiJamEffectiveness();
            oss << "- 抗干扰效果: " << antiJamEffect << std::endl;
        }
    } else {
        oss << "干扰状态: 无干扰" << std::endl;
        oss << "通信环境良好，无需抗干扰措施" << std::endl;
    }
    
    return oss.str();
}