#ifndef COMMUNICATION_DISTANCE_MODEL_H
#define COMMUNICATION_DISTANCE_MODEL_H

#include <string>
#include <stdexcept>
#include "EnvironmentLossConfigManager.h"

class CommunicationDistanceModel {
private:
    double maxLineOfSight;      // 最大视距距离(km)
    EnvironmentType envType;    // 环境类型
    double envAttenuation;      // 环境衰减系数
    double receiveSensitivity;  // 接收灵敏度(dBm)
    double linkMargin;          // 链路余量(dB)
    double transmitPower;       // 发射功率(dBm)

    // 校验衰减系数是否符合当前环境类型的范围
    bool isAttenuationValid(double attenuation, EnvironmentType env) const;

    // 功率参数范围校验
    bool isPowerValid(double power_dBm) const;

public:
    // 构造函数，带默认参数和初始化校验
    CommunicationDistanceModel(
        double maxLOS = 10.0,  // 默认最大视距10km
        EnvironmentType env = EnvironmentType::OPEN_FIELD,
        double attenuation = 1.0,
        double sensitivity = -100.0,  // 默认接收灵敏度-100dBm
        double margin = 10.0,         // 默认链路余量10dB
        double txPower = 20.0         // 默认发射功率20dBm
    );

    // 设置最大视距距离，返回设置是否成功
    bool setMaxLineOfSight(double km);

    // 设置环境类型，同时自动调整衰减系数为对应环境默认值
    void setEnvironmentType(EnvironmentType env);

    // 设置环境衰减系数，返回设置是否成功
    bool setEnvAttenuation(double attenuation);

    // 设置接收灵敏度，返回设置是否成功
    bool setReceiveSensitivity(double dBm);

    // 设置链路余量，返回设置是否成功
    bool setLinkMargin(double dB);

    // 设置发射功率，返回设置是否成功
    bool setTransmitPower(double dBm);

    // 获取最大视距距离
    double getMaxLineOfSight() const;

    // 获取环境类型
    EnvironmentType getEnvironmentType() const;

    // 获取环境衰减系数
    double getEnvAttenuation() const;

    // 获取接收灵敏度
    double getReceiveSensitivity() const;

    // 获取链路余量
    double getLinkMargin() const;

    // 获取发射功率
    double getTransmitPower() const;

    // 计算实际有效通信距离
    double calculateEffectiveDistance() const;

    // 获取参数信息字符串
    std::string getParameterInfo() const;
};

#endif // COMMUNICATION_DISTANCE_MODEL_H
