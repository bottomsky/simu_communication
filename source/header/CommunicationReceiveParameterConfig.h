#ifndef COMMUNICATION_RECEIVE_PARAMETER_CONFIG_H
#define COMMUNICATION_RECEIVE_PARAMETER_CONFIG_H

#include <string>

/**
 * @file CommunicationReceiveParameterConfig.h
 * @brief 通信接收模型参数配置管理类
 * @details 提供接收模型中各种参数的范围配置和校验功能
 */

/**
 * @brief 参数范围结构体
 */
struct ReceiveParameterRange {
    double minValue;  // 最小值
    double maxValue;  // 最大值
    
    ReceiveParameterRange(double min, double max) : minValue(min), maxValue(max) {}
    
    /**
     * @brief 检查值是否在有效范围内
     * @param value 待检查的值
     * @return 是否有效
     */
    bool isValid(double value) const {
        return value >= minValue && value <= maxValue;
    }
};

/**
 * @brief 通信接收模型参数配置管理类
 * @details 统一管理接收模型中各种参数的有效范围和校验逻辑
 */
class CommunicationReceiveParameterConfig {
private:
    // 静态参数范围配置
    static const ReceiveParameterRange RECEIVE_SENSITIVITY_RANGE;  // 接收灵敏度范围 (dBm)
    static const ReceiveParameterRange NOISE_FIGURE_RANGE;         // 噪声系数范围 (dB)
    static const ReceiveParameterRange SYSTEM_BANDWIDTH_RANGE;     // 系统带宽范围 (kHz)
    static const ReceiveParameterRange AMBIENT_TEMPERATURE_RANGE;  // 环境温度范围 (K)
    static const ReceiveParameterRange ANTENNA_GAIN_RANGE;         // 天线增益范围 (dBi)
    static const ReceiveParameterRange RECEIVED_POWER_RANGE;       // 接收功率范围 (dBm)

public:
    // 参数范围获取方法
    static ReceiveParameterRange getReceiveSensitivityRange();
    static ReceiveParameterRange getNoiseFigureRange();
    static ReceiveParameterRange getSystemBandwidthRange();
    static ReceiveParameterRange getAmbientTemperatureRange();
    static ReceiveParameterRange getAntennaGainRange();
    static ReceiveParameterRange getReceivedPowerRange();

    // 参数校验方法
    /**
     * @brief 校验接收灵敏度是否有效
     * @param sensitivity 接收灵敏度 (dBm)
     * @return 是否有效
     */
    static bool isReceiveSensitivityValid(double sensitivity);

    /**
     * @brief 校验噪声系数是否有效
     * @param noiseFigure 噪声系数 (dB)
     * @return 是否有效
     */
    static bool isNoiseFigureValid(double noiseFigure);

    /**
     * @brief 校验系统带宽是否有效
     * @param bandwidth 系统带宽 (kHz)
     * @return 是否有效
     */
    static bool isSystemBandwidthValid(double bandwidth);

    /**
     * @brief 校验环境温度是否有效
     * @param temperature 环境温度 (K)
     * @return 是否有效
     */
    static bool isAmbientTemperatureValid(double temperature);

    /**
     * @brief 校验天线增益是否有效
     * @param gain 天线增益 (dBi)
     * @return 是否有效
     */
    static bool isAntennaGainValid(double gain);

    /**
     * @brief 校验接收功率是否有效
     * @param power 接收功率 (dBm)
     * @return 是否有效
     */
    static bool isReceivedPowerValid(double power);

    /**
     * @brief 获取所有参数范围信息
     * @return 参数范围信息字符串
     */
    static std::string getParameterRangeInfo();
};

#endif // COMMUNICATION_RECEIVE_PARAMETER_CONFIG_H