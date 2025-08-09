#include <iostream>
#include <iomanip>
#include "CommunicationModelAPI.h"

int main() {
    std::cout << "=== 环境损耗配置示例 ===" << std::endl;
    
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
    
    // 创建通信模型API实例
    CommunicationModelAPI api;
    
    // 使用默认配置进行通信计算
    api.setFrequency(2400e6);  // 2.4 GHz
    api.setTransmitPower(20.0);  // 20 dBm
    api.setDistance(1000.0);   // 1 km
    api.setEnvironmentType(EnvironmentType::URBAN_AREA);
    
    std::cout << "\n2. 使用默认配置的通信计算:" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    
    for (auto envType : {EnvironmentType::OPEN_FIELD, EnvironmentType::URBAN_AREA, EnvironmentType::MOUNTAINOUS}) {
        api.setEnvironmentType(envType);
        
        std::string envName;
        switch (envType) {
            case EnvironmentType::OPEN_FIELD: envName = "开阔地区"; break;
            case EnvironmentType::URBAN_AREA: envName = "城市地区"; break;
            case EnvironmentType::MOUNTAINOUS: envName = "山区"; break;
        }
        
        auto linkStatus = api.calculateLinkStatus();
        double optimalFreq = api.calculateOptimalFrequency();
        double range = CommunicationModelUtils::quickCalculateRange(2400.0, 20.0, envType);
        
        std::cout << "  " << envName << ":" << std::endl;
        std::cout << "    信号强度: " << linkStatus.signalStrength << " dBm" << std::endl;
        std::cout << "    信噪比: " << linkStatus.signalToNoiseRatio << " dB" << std::endl;
        std::cout << "    最优频率: " << optimalFreq << " MHz" << std::endl;
        std::cout << "    通信范围: " << range << " km" << std::endl;
    }
    
    // 自定义配置示例
    std::cout << "\n3. 自定义环境损耗配置:" << std::endl;
    
    // 为城市环境设置更高的损耗
    EnvironmentLossConfig customUrbanConfig;
    customUrbanConfig.pathLossExponent = 3.5;
    customUrbanConfig.environmentLoss = 15.0;
    customUrbanConfig.frequencyFactor = 1.8;
    
    EnvironmentLossConfigManager::setConfig(EnvironmentType::URBAN_AREA, customUrbanConfig);
    
    api.setEnvironmentType(EnvironmentType::URBAN_AREA);
    auto newLinkStatus = api.calculateLinkStatus();
    double newOptimalFreq = api.calculateOptimalFrequency();
    double newRange = CommunicationModelUtils::quickCalculateRange(2400.0, 20.0, EnvironmentType::URBAN_AREA);
    
    std::cout << "  自定义城市配置后:" << std::endl;
    std::cout << "    信号强度: " << newLinkStatus.signalStrength << " dBm" << std::endl;
    std::cout << "    信噪比: " << newLinkStatus.signalToNoiseRatio << " dB" << std::endl;
    std::cout << "    最优频率: " << newOptimalFreq << " MHz" << std::endl;
    std::cout << "    通信范围: " << newRange << " km" << std::endl;
    
    // 重置为默认配置
    std::cout << "\n4. 重置为默认配置:" << std::endl;
    EnvironmentLossConfigManager::resetToDefaults();
    
    auto resetLinkStatus = api.calculateLinkStatus();
    double resetOptimalFreq = api.calculateOptimalFrequency();
    double resetRange = CommunicationModelUtils::quickCalculateRange(2400.0, 20.0, EnvironmentType::URBAN_AREA);
    
    std::cout << "  重置后城市配置:" << std::endl;
    std::cout << "    信号强度: " << resetLinkStatus.signalStrength << " dBm" << std::endl;
    std::cout << "    信噪比: " << resetLinkStatus.signalToNoiseRatio << " dB" << std::endl;
    std::cout << "    最优频率: " << resetOptimalFreq << " MHz" << std::endl;
    std::cout << "    通信范围: " << resetRange << " km" << std::endl;
    
    std::cout << "\n=== 示例完成 ===" << std::endl;
    
    return 0;
}