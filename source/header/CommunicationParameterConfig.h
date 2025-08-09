#ifndef COMMUNICATION_PARAMETER_CONFIG_H
#define COMMUNICATION_PARAMETER_CONFIG_H

#include <string>

/**
 * @file CommunicationParameterConfig.h
 * @brief 通信参数配置管理类
 * @details 提供通信模型中各种参数的范围配置和校验功能
 */

/**
 * @brief 通信参数范围配置结构体
 */
struct ParameterRange {
    double minValue;    // 最小值
    double maxValue;    // 最大值
    
    ParameterRange(double min = 0.0, double max = 0.0) 
        : minValue(min), maxValue(max) {}
    
    /**
     * @brief 检查值是否在范围内
     * @param value 要检查的值
     * @return 如果在范围内返回true，否则返回false
     */
    bool isValid(double value) const {
        return value >= minValue && value <= maxValue;
    }
};

/**
 * @brief 通信参数配置管理类
 * @details 统一管理通信模型中各种参数的有效范围和校验逻辑
 */
class CommunicationParameterConfig {
public:
    /**
     * @brief 获取最大视距距离的有效范围
     * @return 最大视距距离范围 (km)
     */
    static ParameterRange getMaxLineOfSightRange();
    
    /**
     * @brief 获取接收灵敏度的有效范围
     * @return 接收灵敏度范围 (dBm)
     */
    static ParameterRange getReceiveSensitivityRange();
    
    /**
     * @brief 获取链路余量的有效范围
     * @return 链路余量范围 (dB)
     */
    static ParameterRange getLinkMarginRange();
    
    /**
     * @brief 获取发射功率的有效范围
     * @return 发射功率范围 (dBm)
     */
    static ParameterRange getTransmitPowerRange();
    
    /**
     * @brief 校验最大视距距离是否有效
     * @param maxLOS 最大视距距离 (km)
     * @return 如果有效返回true，否则返回false
     */
    static bool isMaxLineOfSightValid(double maxLOS);
    
    /**
     * @brief 校验接收灵敏度是否有效
     * @param sensitivity 接收灵敏度 (dBm)
     * @return 如果有效返回true，否则返回false
     */
    static bool isReceiveSensitivityValid(double sensitivity);
    
    /**
     * @brief 校验链路余量是否有效
     * @param margin 链路余量 (dB)
     * @return 如果有效返回true，否则返回false
     */
    static bool isLinkMarginValid(double margin);
    
    /**
     * @brief 校验发射功率是否有效
     * @param power 发射功率 (dBm)
     * @return 如果有效返回true，否则返回false
     */
    static bool isTransmitPowerValid(double power);
    
    /**
     * @brief 获取参数范围的描述信息
     * @return 参数范围描述字符串
     */
    static std::string getParameterRangeInfo();

private:
    // 参数范围常量定义
    static const ParameterRange MAX_LINE_OF_SIGHT_RANGE;    // 最大视距范围 (0.5-50.0 km)
    static const ParameterRange RECEIVE_SENSITIVITY_RANGE;  // 接收灵敏度范围 (-110.0 to -90.0 dBm)
    static const ParameterRange LINK_MARGIN_RANGE;          // 链路余量范围 (5.0-20.0 dB)
    static const ParameterRange TRANSMIT_POWER_RANGE;       // 发射功率范围 (-30.0 to 30.0 dBm)
};

#endif // COMMUNICATION_PARAMETER_CONFIG_H