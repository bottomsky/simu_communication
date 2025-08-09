#include "CommunicationDistanceModel.h"
#include <sstream>
#include <cmath>



// 功率参数范围校验实现
bool CommunicationDistanceModel::isPowerValid(double power_dBm) const {
    return power_dBm >= -30.0 && power_dBm <= 30.0;  // 覆盖常见设备功率范围
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
    if (maxLOS < 0.5 || maxLOS > 50.0) {
        throw std::invalid_argument("最大视距需在0.5-50km范围内");
    }
    // 校验衰减系数
    if (!EnvironmentLossConfigManager::isAttenuationValid(attenuation, env)) {
        throw std::invalid_argument("衰减系数不符合当前环境类型范围");
    }
    // 校验接收灵敏度
    if (sensitivity < -110.0 || sensitivity > -90.0) {
        throw std::invalid_argument("接收灵敏度需在-110至-90dBm范围内");
    }
    // 校验链路余量
    if (margin < 5.0 || margin > 20.0) {
        throw std::invalid_argument("链路余量需在5-20dB范围内");
    }
    // 校验发射功率
    if (!isPowerValid(txPower)) {
        throw std::invalid_argument("发射功率需在-30至30dBm范围内");
    }

    envAttenuation = attenuation;
    receiveSensitivity = sensitivity;
    linkMargin = margin;
}

// 设置最大视距距离实现
bool CommunicationDistanceModel::setMaxLineOfSight(double km) {
    if (km >= 0.5 && km <= 50.0) {
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
    envAttenuation = 1.0 + (config.environmentLoss / 10.0);
    
    // 确保衰减系数在合理范围内
    if (envAttenuation < 0.5) envAttenuation = 0.5;
    if (envAttenuation > 5.0) envAttenuation = 5.0;
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
    if (dBm >= -110.0 && dBm <= -90.0) {
        receiveSensitivity = dBm;
        return true;
    }
    return false;
}

// 设置链路余量实现
bool CommunicationDistanceModel::setLinkMargin(double dB) {
    if (dB >= 5.0 && dB <= 20.0) {
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
    double powerDistanceFactor = pow(2.0, powerDiff / 6.0);

    // 2. 计算环境受限距离（视距/衰减系数）
    double envLimitedDistance = maxLineOfSight / envAttenuation;

    // 3. 有效距离取两者最小值（功率与环境共同限制）
    return std::min(envLimitedDistance * powerDistanceFactor, maxLineOfSight);
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
