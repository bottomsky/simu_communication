#include <iostream>
#include <iomanip>
#include "EnvironmentLossConfigManager.h"

int main() {
    std::cout << "=== 环境损耗配置管理器示例 ===" << std::endl;
    
    // 显示默认配置
    std::cout << "\n1. 默认环境损耗配置:" << std::endl;
    auto configs = EnvironmentLossConfigManager::getAllConfigs();
    for (const auto& [envType, config] : configs) {
        std::string envName;
        switch (envType) {
            case EnvironmentType::OPEN_FIELD: envName = "开阔地区"; break;
            case EnvironmentType::URBAN_AREA: envName = "城市地区"; break;
            case EnvironmentType::MOUNTAINOUS: envName = "山区"; break;
        }
        
        std::cout << "  " << envName << ":" << std::endl;
        std::cout << "    路径损耗指数: " << config.pathLossExponent << std::endl;
        std::cout << "    环境损耗: " << config.environmentLoss << " dB" << std::endl;
        std::cout << "    频率因子: " << config.frequencyFactor << std::endl;
    }
    
    // 获取特定环境的配置
    std::cout << "\n2. 获取特定环境配置:" << std::endl;
    auto urbanConfig = EnvironmentLossConfigManager::getConfig(EnvironmentType::URBAN_AREA);
    std::cout << "  城市地区配置:" << std::endl;
    std::cout << "    路径损耗指数: " << urbanConfig.pathLossExponent << std::endl;
    std::cout << "    环境损耗: " << urbanConfig.environmentLoss << " dB" << std::endl;
    std::cout << "    频率因子: " << urbanConfig.frequencyFactor << std::endl;
    
    // 自定义配置示例
    std::cout << "\n3. 自定义环境损耗配置:" << std::endl;
    
    // 为城市环境设置更高的损耗
    EnvironmentLossConfig customUrbanConfig;
    customUrbanConfig.pathLossExponent = 3.5;
    customUrbanConfig.environmentLoss = 15.0;
    customUrbanConfig.frequencyFactor = 1.8;
    
    EnvironmentLossConfigManager::setConfig(EnvironmentType::URBAN_AREA, customUrbanConfig);
    
    auto updatedUrbanConfig = EnvironmentLossConfigManager::getConfig(EnvironmentType::URBAN_AREA);
    std::cout << "  自定义城市配置后:" << std::endl;
    std::cout << "    路径损耗指数: " << updatedUrbanConfig.pathLossExponent << std::endl;
    std::cout << "    环境损耗: " << updatedUrbanConfig.environmentLoss << " dB" << std::endl;
    std::cout << "    频率因子: " << updatedUrbanConfig.frequencyFactor << std::endl;
    
    // 重置为默认配置
    std::cout << "\n4. 重置为默认配置:" << std::endl;
    EnvironmentLossConfigManager::resetToDefaults();
    
    auto resetUrbanConfig = EnvironmentLossConfigManager::getConfig(EnvironmentType::URBAN_AREA);
    std::cout << "  重置后城市配置:" << std::endl;
    std::cout << "    路径损耗指数: " << resetUrbanConfig.pathLossExponent << std::endl;
    std::cout << "    环境损耗: " << resetUrbanConfig.environmentLoss << " dB" << std::endl;
    std::cout << "    频率因子: " << resetUrbanConfig.frequencyFactor << std::endl;
    
    std::cout << "\n=== 示例完成 ===" << std::endl;
    
    return 0;
}