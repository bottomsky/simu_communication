#include "CommunicationModelAPI.h"
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <chrono>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


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
    environment_.frequency = 10000.0;
    environment_.bandwidth = 100.0;
    environment_.transmitPower = 30.0;
    environment_.noisePower = -100.0;
    environment_.distance = 10.0;
    environment_.environmentType = EnvironmentType::OPEN_FIELD;
    environment_.temperature = 20.0;
    environment_.humidity = 50.0;
    environment_.atmosphericPressure = 101.3;
    
    // 设置默认干扰环境
    jammingEnv_.isJammed = false;
    jammingEnv_.jammerType = JammerType::GAUSSIAN_NOISE;
    jammingEnv_.jammerPower = -90.0;
    jammingEnv_.jammerFrequency = 10000.0;
    jammingEnv_.jammerBandwidth = 50.0;
    jammingEnv_.jammerDistance = 20.0;
    jammingEnv_.jammerDensity = 0.1;
    
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
    if (!signalModel_ || !distanceModel_) return -150.0;
    
    // 基础信号强度
    double baseSignal = environment_.transmitPower;
    
    // 使用距离模型计算总路径损耗（包含所有损耗因子）
    double totalPathLoss = distanceModel_->calculateTotalPathLoss(environment_.distance, environment_.frequency);
    
    return baseSignal - totalPathLoss;
}

double CommunicationModelAPI::calculateOverallSNR() const {
    if (!receiveModel_) return -50.0;
    
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

double CommunicationModelAPI::calculateOverallBER() const {
    double snr = calculateOverallSNR();
    
    // 简化的BPSK误码率计算
    double linearSnr = std::pow(10.0, snr / 10.0);
    double ber = 0.5 * std::erfc(std::sqrt(linearSnr));
    
    return std::max(1e-12, std::min(0.5, ber));
}

double CommunicationModelAPI::calculateThroughput() const {
    double snr = calculateOverallSNR();
    double ber = calculateOverallBER();
    
    // 基于香农定理的理论容量
    double theoreticalCapacity = environment_.bandwidth * std::log2(1.0 + std::pow(10.0, snr / 10.0));
    
    // 考虑误码率的影响
    double efficiency = 1.0 - 10.0 * ber; // 简化的效率模型
    efficiency = std::max(0.0, std::min(1.0, efficiency));
    
    return theoreticalCapacity * efficiency;
}

double CommunicationModelAPI::calculateLatency() const {
    double baseLatency = environment_.distance / 300.0; // 光速传播延迟 (ms)
    
    // 处理延迟
    double processingDelay = 1.0; // 基础处理延迟 1ms
    
    // 重传延迟（基于误码率）
    double ber = calculateOverallBER();
    double retransmissionDelay = ber * 100.0; // 简化模型
    
    return baseLatency + processingDelay + retransmissionDelay;
}

double CommunicationModelAPI::calculatePacketLossRate() const {
    double ber = calculateOverallBER();
    
    // 假设数据包长度为1000字节
    int packetLengthBits = 8000;
    
    // 数据包错误率 = 1 - (1 - BER)^数据包长度
    double packetErrorRate = 1.0 - std::pow(1.0 - ber, packetLengthBits);
    
    return std::max(0.0, std::min(1.0, packetErrorRate));
}

CommunicationQuality CommunicationModelAPI::assessCommunicationQuality() const {
    double snr = calculateOverallSNR();
    double ber = calculateOverallBER();
    double packetLoss = calculatePacketLossRate();
    
    // 综合评估通信质量
    int score = 0;
    
    // SNR评分
    if (snr > 20.0) score += 2;
    else if (snr > 10.0) score += 1;
    else if (snr < 0.0) score -= 1;
    
    // BER评分
    if (ber < 1e-6) score += 2;
    else if (ber < 1e-4) score += 1;
    else if (ber > 1e-2) score -= 1;
    
    // 丢包率评分
    if (packetLoss < 0.01) score += 1;
    else if (packetLoss > 0.1) score -= 1;
    
    // 映射到质量等级
    if (score >= 4) return CommunicationQuality::EXCELLENT;
    else if (score >= 2) return CommunicationQuality::GOOD;
    else if (score >= 0) return CommunicationQuality::FAIR;
    else if (score >= -2) return CommunicationQuality::POOR;
    else return CommunicationQuality::FAILED;
}

// 场景设置
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
    if (frequency < 1.0 || frequency > 30000.0) return false;
    environment_.frequency = frequency;
    updateModelsFromEnvironment();
    return true;
}

bool CommunicationModelAPI::setBandwidth(double bandwidth) {
    if (bandwidth < 0.1 || bandwidth > 10000.0) return false;
    environment_.bandwidth = bandwidth;
    updateModelsFromEnvironment();
    return true;
}

bool CommunicationModelAPI::setTransmitPower(double power) {
    if (power < -50.0 || power > 50.0) return false;
    environment_.transmitPower = power;
    updateModelsFromEnvironment();
    return true;
}

bool CommunicationModelAPI::setDistance(double distance) {
    if (distance < 0.001 || distance > 1000.0) return false;
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
    status.isConnected = (status.signalToNoiseRatio > 0.0 && 
                         status.bitErrorRate < 0.1 && 
                         status.packetLossRate < 0.5);
    
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
    double powerWatts = std::pow(10.0, (environment_.transmitPower - 30.0) / 10.0);
    performance.powerEfficiency = (performance.maxDataRate * 1e6) / powerWatts;
    
    // 频谱效率 (bps/Hz)
    performance.spectralEfficiency = (performance.maxDataRate * 1e6) / (environment_.bandwidth * 1e6);
    
    // 可靠性（基于误码率）
    double ber = calculateOverallBER();
    performance.reliability = 1.0 - std::min(1.0, ber * 1000.0);
    
    // 可用性（基于信噪比）
    double snr = calculateOverallSNR();
    performance.availability = 1.0 / (1.0 + std::exp(-(snr - 5.0) / 2.0));
    
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
    optimized.transmitPower = optimized.noisePower + minSNR + 3.0; // 3dB余量
    
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

// 便利函数实现
namespace CommunicationModelUtils {
    double quickCalculateRange(double frequency, double power, EnvironmentType env) {
        // 从配置获取环境特性
        const EnvironmentLossConfig& config = EnvironmentLossConfigManager::getConfig(env);
        
        double wavelength = 300.0 / frequency; // MHz to meters
        double freeSpaceRef = 20.0 * std::log10(4.0 * M_PI / wavelength);
        
        // 考虑环境损耗和频率因子
        double environmentLoss = config.environmentLoss;
        double frequencyLoss = config.frequencyFactor * std::log10(frequency / 1000.0) * 2.0;
        double totalLoss = environmentLoss + frequencyLoss;
        
        double maxPathLoss = power - (-100.0) - 10.0 - totalLoss; // 假设-100dBm噪声，10dB SNR
        
        return std::pow(10.0, (maxPathLoss - freeSpaceRef) / (10.0 * config.pathLossExponent)) / 1000.0; // km
    }
    
    double quickCalculatePower(double frequency, double range, EnvironmentType env) {
        // 从配置获取环境特性
        const EnvironmentLossConfig& config = EnvironmentLossConfigManager::getConfig(env);
        
        double wavelength = 300.0 / frequency; // MHz to meters
        double freeSpaceRef = 20.0 * std::log10(4.0 * M_PI / wavelength);
        double pathLoss = freeSpaceRef + 10.0 * config.pathLossExponent * std::log10(range * 1000.0);
        
        // 考虑环境损耗和频率因子
        double environmentLoss = config.environmentLoss;
        double frequencyLoss = config.frequencyFactor * std::log10(frequency / 1000.0) * 2.0;
        double totalLoss = environmentLoss + frequencyLoss;
        
        return -100.0 + 10.0 + pathLoss + totalLoss; // 噪声功率 + SNR + 路径损耗 + 环境损耗
    }
    
    CommunicationQuality quickAssessQuality(double snr, double ber) {
        if (snr > 20.0 && ber < 1e-6) return CommunicationQuality::EXCELLENT;
        else if (snr > 10.0 && ber < 1e-4) return CommunicationQuality::GOOD;
        else if (snr > 5.0 && ber < 1e-2) return CommunicationQuality::FAIR;
        else if (snr > 0.0 && ber < 0.1) return CommunicationQuality::POOR;
        else return CommunicationQuality::FAILED;
    }
    
    double dBmToWatts(double dBm) {
        return std::pow(10.0, (dBm - 30.0) / 10.0);
    }
    
    double wattsTodBm(double watts) {
        return 10.0 * std::log10(watts) + 30.0;
    }
    
    double dBToLinear(double dB) {
        return std::pow(10.0, dB / 10.0);
    }
    
    double linearTodB(double linear) {
        return 10.0 * std::log10(linear);
    }
    
    double frequencyToWavelength(double frequency) {
        return 300.0 / frequency; // MHz to meters
    }
    
    double wavelengthToFrequency(double wavelength) {
        return 300.0 / wavelength; // meters to MHz
    }
}