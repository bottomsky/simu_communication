#include "CommunicationDistanceModel.h"
#include "MathConstants.h"
#include <sstream>
#include <cmath>



// 功率参数范围校验实现
bool CommunicationDistanceModel::isPowerValid(double power_dBm) const {
    return CommunicationParameterConfig::isTransmitPowerValid(power_dBm);
}

// 构造函数实现
CommunicationDistanceModel::CommunicationDistanceModel(
    double maxLOS,
    EnvironmentType env,
    double attenuation,
    double sensitivity,
    double margin,
    double txPower
) : maxLineOfSight(maxLOS), envType(env), transmitPower(txPower) {
    // 校验最大视距
    if (!CommunicationParameterConfig::isMaxLineOfSightValid(maxLOS)) {
        ParameterRange range = CommunicationParameterConfig::getMaxLineOfSightRange();
        throw std::invalid_argument("最大视距需在" + std::to_string(range.minValue) + 
                                   "-" + std::to_string(range.maxValue) + "km范围内");
    }
    // 校验衰减系数
    if (!EnvironmentLossConfigManager::isAttenuationValid(attenuation, env)) {
        throw std::invalid_argument("衰减系数不符合当前环境类型范围");
    }
    // 校验接收灵敏度
    if (!CommunicationParameterConfig::isReceiveSensitivityValid(sensitivity)) {
        ParameterRange range = CommunicationParameterConfig::getReceiveSensitivityRange();
        throw std::invalid_argument("接收灵敏度需在" + std::to_string(range.minValue) + 
                                   "至" + std::to_string(range.maxValue) + "dBm范围内");
    }
    // 校验链路余量
    if (!CommunicationParameterConfig::isLinkMarginValid(margin)) {
        ParameterRange range = CommunicationParameterConfig::getLinkMarginRange();
        throw std::invalid_argument("链路余量需在" + std::to_string(range.minValue) + 
                                   "-" + std::to_string(range.maxValue) + "dB范围内");
    }
    // 校验发射功率
    if (!CommunicationParameterConfig::isTransmitPowerValid(txPower)) {
        ParameterRange range = CommunicationParameterConfig::getTransmitPowerRange();
        throw std::invalid_argument("发射功率需在" + std::to_string(range.minValue) + 
                                   "至" + std::to_string(range.maxValue) + "dBm范围内");
    }

    envAttenuation = attenuation;
    receiveSensitivity = sensitivity;
    linkMargin = margin;
}

// 设置最大视距距离实现
bool CommunicationDistanceModel::setMaxLineOfSight(double km) {
    if (CommunicationParameterConfig::isMaxLineOfSightValid(km)) {
        maxLineOfSight = km;
        return true;
    }
    return false;
}

// 设置环境类型实现
void CommunicationDistanceModel::setEnvironmentType(EnvironmentType env) {
    envType = env;
    // 使用 EnvironmentLossConfigManager 获取环境配置
    const EnvironmentLossConfig& config = EnvironmentLossConfigManager::getConfig(env);
    
    // 根据环境损耗配置计算衰减系数
    // 将环境损耗转换为衰减系数 (简化模型：每10dB环境损耗对应1.0衰减系数增量)
    envAttenuation = MathConstants::ENV_ATTENUATION_BASE + (config.environmentLoss / MathConstants::LINEAR_TO_DB_MULTIPLIER);
    
    // 确保衰减系数在合理范围内
    if (envAttenuation < MathConstants::ENV_ATTENUATION_MIN) envAttenuation = MathConstants::ENV_ATTENUATION_MIN;
    if (envAttenuation > MathConstants::ENV_ATTENUATION_MAX) envAttenuation = MathConstants::ENV_ATTENUATION_MAX;
}

// 设置环境衰减系数实现
bool CommunicationDistanceModel::setEnvAttenuation(double attenuation) {
    if (EnvironmentLossConfigManager::isAttenuationValid(attenuation, envType)) {
        envAttenuation = attenuation;
        return true;
    }
    return false;
}

// 设置接收灵敏度实现
bool CommunicationDistanceModel::setReceiveSensitivity(double dBm) {
    if (CommunicationParameterConfig::isReceiveSensitivityValid(dBm)) {
        receiveSensitivity = dBm;
        return true;
    }
    return false;
}

// 设置链路余量实现
bool CommunicationDistanceModel::setLinkMargin(double dB) {
    if (CommunicationParameterConfig::isLinkMarginValid(dB)) {
        linkMargin = dB;
        return true;
    }
    return false;
}

// 设置发射功率实现
bool CommunicationDistanceModel::setTransmitPower(double dBm) {
    if (isPowerValid(dBm)) {
        transmitPower = dBm;
        return true;
    }
    return false;
}

// 获取最大视距距离实现
double CommunicationDistanceModel::getMaxLineOfSight() const {
    return maxLineOfSight;
}

// 获取环境类型实现
EnvironmentType CommunicationDistanceModel::getEnvironmentType() const {
    return envType;
}

// 获取环境衰减系数实现
double CommunicationDistanceModel::getEnvAttenuation() const {
    return envAttenuation;
}

// 获取接收灵敏度实现
double CommunicationDistanceModel::getReceiveSensitivity() const {
    return receiveSensitivity;
}

// 获取链路余量实现
double CommunicationDistanceModel::getLinkMargin() const {
    return linkMargin;
}

// 获取发射功率实现
double CommunicationDistanceModel::getTransmitPower() const {
    return transmitPower;
}

// 计算实际有效通信距离实现
double CommunicationDistanceModel::calculateEffectiveDistance() const {
    // 1. 计算功率受限距离（基于收发功率差）
    double powerDiff = transmitPower - receiveSensitivity - linkMargin;
    if (powerDiff < 0) return 0.0;  // 功率不足，无法通信
    
    // 功率距离因子（简化模型：每增加6dB，距离翻倍）
    double powerDistanceFactor = pow(MathConstants::POWER_DISTANCE_BASE, powerDiff / MathConstants::POWER_DISTANCE_DB_FACTOR);

    // 2. 计算环境受限距离（视距/衰减系数）
    double envLimitedDistance = maxLineOfSight / envAttenuation;

    // 3. 有效距离取两者最小值（功率与环境共同限制）
    return std::min(envLimitedDistance * powerDistanceFactor, maxLineOfSight);
}

// 计算自由空间路径损耗实现
double CommunicationDistanceModel::calculateFreeSpacePathLoss(double distance_km, double frequency_MHz) {
    if (distance_km <= 0.0 || frequency_MHz <= 0.0) {
        return 0.0;
    }
    
    // 自由空间路径损耗公式: FSPL = 20*log10(d) + 20*log10(f) + 32.45
    // 其中 d 为距离(km)，f 为频率(MHz)
    double fspl = MathConstants::FSPL_DISTANCE_COEFFICIENT * std::log10(distance_km) + 
                  MathConstants::FSPL_FREQUENCY_COEFFICIENT * std::log10(frequency_MHz) + 
                  MathConstants::FSPL_CONSTANT;
    return fspl;
}

/// @brief 计算路径损耗对应的距离
/// @param pathLoss_dB 路径损耗(dB)
/// @param frequency_MHz 频率(MHz)
/// @return 距离(km)
double CommunicationDistanceModel::calculateDistanceFromPathLoss(double pathLoss_dB, double frequency_MHz) {
    if (pathLoss_dB < 0.0 || frequency_MHz <= 0.0) {
        return -1.0; // 无效参数返回错误
    }
    
    // 自由空间损耗公式：L = 20log10(d) + 20log10(f) + 32.45
    // 其中：L为损耗(dB)，d为距离(km)，f为频率(MHz)
    // 转换为距离计算：d(km) = 10^[(L - 20log10(f) - 32.45)/20]
    
    // 计算对数项
    double logFreq = std::log10(frequency_MHz);
    if (std::isnan(logFreq) || std::isinf(logFreq)) {
        return -1.0; // 对数计算异常
    }

    // 计算距离（单位：km）
    double distanceKm = std::pow(10.0, (pathLoss_dB - MathConstants::FSPL_CONSTANT - MathConstants::FSPL_DISTANCE_COEFFICIENT * logFreq) / MathConstants::FSPL_DISTANCE_COEFFICIENT);
    
    // 确保距离为合理正值
    return (distanceKm > 0) ? distanceKm : 0.0;
}

// 计算路径损耗实现（包含环境因子）
double CommunicationDistanceModel::calculatePathLoss(double distance_km, double frequency_MHz) const {
    if (distance_km <= 0.0 || frequency_MHz <= 0.0) {
        return 0.0;
    }
    
    // 计算自由空间路径损耗
    double freeSpacePathLoss = CommunicationDistanceModel::calculateFreeSpacePathLoss(distance_km, frequency_MHz);
    
    // 使用EnvironmentLossConfigManager计算环境路径损耗
    double environmentPathLoss = EnvironmentLossConfigManager::calculateEnvironmentPathLoss(distance_km, envType);
    
    return freeSpacePathLoss + environmentPathLoss;
}

// 计算总路径损耗实现（包含所有损耗因子）
double CommunicationDistanceModel::calculateTotalPathLoss(double distance_km, double frequency_MHz) const {
    if (distance_km <= 0.0 || frequency_MHz <= 0.0) {
        return 0.0;
    }
    
    // 计算自由空间路径损耗
    double freeSpacePathLoss = CommunicationDistanceModel::calculateFreeSpacePathLoss(distance_km, frequency_MHz);
    
    // 使用EnvironmentLossConfigManager计算总环境损耗
    double totalEnvironmentLoss = EnvironmentLossConfigManager::calculateTotalEnvironmentLoss(distance_km, frequency_MHz, envType);
    
    return freeSpacePathLoss + totalEnvironmentLoss;
}

// 快速距离计算方法实现（使用当前模型参数）
double CommunicationDistanceModel::quickCalculateRange(double frequency_MHz) const {
    if (frequency_MHz <= 0.0) {
        return 0.0;
    }
    
    // 计算最大允许路径损耗
    double maxPathLoss = transmitPower - receiveSensitivity - linkMargin;
    if (maxPathLoss <= 0.0) {
        return 0.0; // 功率不足，无法通信
    }
    
    // 使用迭代方法求解距离
    double estimatedDistance = MathConstants::INITIAL_DISTANCE_ESTIMATE; // 初始估计距离 1km
    
    // 迭代求解距离（牛顿法的简化版本）
    for (int i = 0; i < MathConstants::MAX_ITERATIONS; i++) {
        // 计算当前距离下的总路径损耗
        double totalCalculatedLoss = calculateTotalPathLoss(estimatedDistance, frequency_MHz);
        
        // 计算误差
        double error = totalCalculatedLoss - maxPathLoss;
        
        // 如果误差足够小，退出迭代
        if (std::abs(error) < MathConstants::CONVERGENCE_TOLERANCE) {
            break;
        }
        
        // 根据误差调整距离估计
        if (error > 0) {
            estimatedDistance *= MathConstants::DISTANCE_DECREASE_FACTOR; // 损耗过大，减小距离
        } else {
            estimatedDistance *= MathConstants::DISTANCE_INCREASE_FACTOR; // 损耗过小，增大距离
        }
        
        // 确保距离在合理范围内
        if (estimatedDistance < MathConstants::MIN_DISTANCE_LIMIT) estimatedDistance = MathConstants::MIN_DISTANCE_LIMIT;
        if (estimatedDistance > maxLineOfSight) estimatedDistance = maxLineOfSight;
    }
    
    return estimatedDistance;
}

// 静态快速距离计算方法实现（向后兼容）
double CommunicationDistanceModel::quickCalculateRange(double frequency_MHz, double power_dBm, EnvironmentType env) {
    // 创建临时模型实例，使用传入的参数
    CommunicationDistanceModel tempModel(
        MathConstants::DEFAULT_MAX_LINE_OF_SIGHT,       // 最大视距50km（在有效范围内，足够大不限制计算）
        env,        // 环境类型
        MathConstants::ENV_ATTENUATION_BASE,        // 默认衰减系数（会被环境类型覆盖）
        MathConstants::DEFAULT_RECEIVE_SENSITIVITY,     // 接收灵敏度-100dBm
        MathConstants::DEFAULT_LINK_MARGIN,       // 链路余量10dB
        power_dBm   // 发射功率
    );
    
    return tempModel.quickCalculateRange(frequency_MHz);
}

// 获取参数信息字符串实现
std::string CommunicationDistanceModel::getParameterInfo() const {
    std::stringstream ss;
    ss << "通信距离模型参数:\n"
       << "  最大视距距离: " << maxLineOfSight << "km\n"
       << "  环境类型: " << (envType == EnvironmentType::OPEN_FIELD ? "开阔地" :
                             (envType == EnvironmentType::URBAN_AREA ? "城市区域" : "山区")) << "\n"
       << "  环境衰减系数: " << envAttenuation << "\n"
       << "  接收灵敏度: " << receiveSensitivity << "dBm\n"
       << "  链路余量: " << linkMargin << "dB\n"
       << "  发射功率: " << transmitPower << "dBm\n"
       << "  有效通信距离: " << calculateEffectiveDistance() << "km";
    return ss.str();
}
