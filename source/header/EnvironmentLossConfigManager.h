#ifndef ENVIRONMENT_LOSS_CONFIG_MANAGER_H
#define ENVIRONMENT_LOSS_CONFIG_MANAGER_H

#include "CommunicationDistanceModel.h"
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>

/**
 * @brief 环境损耗配置结构体
 */
struct EnvironmentLossConfig {
    double pathLossExponent;             // 路径损耗指数
    double environmentLoss;              // 环境损耗 (dB)
    double shadowingStdDev;              // 阴影衰落标准差 (dB)
    double frequencyFactor;              // 频率因子
    
    EnvironmentLossConfig(double pathLoss = 2.0, double envLoss = 0.0, 
                         double shadowing = 0.0, double freqFactor = 1.0)
        : pathLossExponent(pathLoss), environmentLoss(envLoss), 
          shadowingStdDev(shadowing), frequencyFactor(freqFactor) {}
};

/**
 * @brief 环境损耗系数配置管理类
 * 
 * 该类负责管理不同环境类型的损耗配置参数，提供配置的获取、设置、
 * 重置等功能。支持运行时动态修改配置，以适应不同的通信环境需求。
 */
class EnvironmentLossConfigManager {
private:
    static std::unordered_map<EnvironmentType, EnvironmentLossConfig> configs_;
    static bool initialized_;
    
    /**
     * @brief 初始化默认配置
     */
    static void initializeDefaultConfigs();
    
public:
    /**
     * @brief 获取指定环境类型的损耗配置
     * @param envType 环境类型
     * @return 环境损耗配置引用
     */
    static const EnvironmentLossConfig& getConfig(EnvironmentType envType);
    
    /**
     * @brief 设置指定环境类型的损耗配置
     * @param envType 环境类型
     * @param config 新的环境损耗配置
     */
    static void setConfig(EnvironmentType envType, const EnvironmentLossConfig& config);
    
    /**
     * @brief 重置所有配置为默认值
     */
    static void resetToDefaults();
    
    /**
     * @brief 获取所有环境类型的配置
     * @return 所有配置的映射表
     */
    static const std::unordered_map<EnvironmentType, EnvironmentLossConfig>& getAllConfigs();
    
    /**
     * @brief 检查指定环境类型是否有配置
     * @param envType 环境类型
     * @return 如果存在配置返回true，否则返回false
     */
    static bool hasConfig(EnvironmentType envType);
    
    /**
     * @brief 获取支持的环境类型数量
     * @return 支持的环境类型数量
     */
    static size_t getConfigCount();
    
    /**
     * @brief 验证配置参数的有效性
     * @param config 要验证的配置
     * @return 如果配置有效返回true，否则返回false
     */
    static bool validateConfig(const EnvironmentLossConfig& config);
    
    /**
     * @brief 获取环境类型的字符串描述
     * @param envType 环境类型
     * @return 环境类型的字符串描述
     */
    static std::string getEnvironmentTypeName(EnvironmentType envType);
    
    /**
     * @brief 从字符串解析环境类型
     * @param name 环境类型名称
     * @return 对应的环境类型，如果解析失败返回OPEN_FIELD
     */
    static EnvironmentType parseEnvironmentType(const std::string& name);
    
    /**
     * @brief 导出配置为JSON格式字符串
     * @return JSON格式的配置字符串
     */
    static std::string exportConfigsToJSON();
    
    /**
     * @brief 从JSON字符串导入配置
     * @param jsonStr JSON格式的配置字符串
     * @return 如果导入成功返回true，否则返回false
     */
    static bool importConfigsFromJSON(const std::string& jsonStr);
    
    /**
     * @brief 保存配置到文件
     * @param filename 文件名
     * @return 如果保存成功返回true，否则返回false
     */
    static bool saveConfigsToFile(const std::string& filename);
    
    /**
     * @brief 从文件加载配置
     * @param filename 文件名
     * @return 如果加载成功返回true，否则返回false
     */
    static bool loadConfigsFromFile(const std::string& filename);
};

#endif // ENVIRONMENT_LOSS_CONFIG_MANAGER_H