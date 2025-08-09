#include "EnvironmentLossConfigManager.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cmath>

// 静态成员定义
std::unordered_map<EnvironmentType, EnvironmentLossConfig> EnvironmentLossConfigManager::configs_;
bool EnvironmentLossConfigManager::initialized_ = false;

void EnvironmentLossConfigManager::initializeDefaultConfigs() {
    if (initialized_) return;
    
    // 开阔地区配置
    configs_[EnvironmentType::OPEN_FIELD] = EnvironmentLossConfig(
        2.0,    // 路径损耗指数
        0.0,    // 环境损耗 (dB)
        4.0,    // 阴影衰落标准差 (dB)
        1.0     // 频率因子
    );
    
    // 城市地区配置
    configs_[EnvironmentType::URBAN_AREA] = EnvironmentLossConfig(
        3.0,    // 路径损耗指数
        10.0,   // 环境损耗 (dB)
        8.0,    // 阴影衰落标准差 (dB)
        1.2     // 频率因子
    );
    
    // 山区配置
    configs_[EnvironmentType::MOUNTAINOUS] = EnvironmentLossConfig(
        3.5,    // 路径损耗指数
        15.0,   // 环境损耗 (dB)
        10.0,   // 阴影衰落标准差 (dB)
        1.5     // 频率因子
    );
    
    initialized_ = true;
}

const EnvironmentLossConfig& EnvironmentLossConfigManager::getConfig(EnvironmentType envType) {
    if (!initialized_) {
        initializeDefaultConfigs();
    }
    
    auto it = configs_.find(envType);
    if (it != configs_.end()) {
        return it->second;
    }
    
    // 如果找不到配置，返回开阔地区的默认配置
    return configs_[EnvironmentType::OPEN_FIELD];
}

void EnvironmentLossConfigManager::setConfig(EnvironmentType envType, const EnvironmentLossConfig& config) {
    if (!initialized_) {
        initializeDefaultConfigs();
    }
    
    if (validateConfig(config)) {
        configs_[envType] = config;
    } else {
        throw std::invalid_argument("Invalid environment loss configuration parameters");
    }
}

void EnvironmentLossConfigManager::resetToDefaults() {
    initialized_ = false;
    configs_.clear();
    initializeDefaultConfigs();
}

const std::unordered_map<EnvironmentType, EnvironmentLossConfig>& EnvironmentLossConfigManager::getAllConfigs() {
    if (!initialized_) {
        initializeDefaultConfigs();
    }
    return configs_;
}

bool EnvironmentLossConfigManager::hasConfig(EnvironmentType envType) {
    if (!initialized_) {
        initializeDefaultConfigs();
    }
    return configs_.find(envType) != configs_.end();
}

size_t EnvironmentLossConfigManager::getConfigCount() {
    if (!initialized_) {
        initializeDefaultConfigs();
    }
    return configs_.size();
}
/// @brief 验证环境损耗配置是否有效
/// @param config 环境损耗配置
/// @return 如果配置有效返回true，否则返回false
bool EnvironmentLossConfigManager::validateConfig(const EnvironmentLossConfig& config) {
    // 验证路径损耗指数 (通常在1.5到6之间)
    if (config.pathLossExponent < 1.5 || config.pathLossExponent > 6.0) {
        return false;
    }
    
    // 验证环境损耗 (通常在0到50dB之间)
    if (config.environmentLoss < 0.0 || config.environmentLoss > 50.0) {
        return false;
    }
    
    // 验证阴影衰落标准差 (通常在0到20dB之间)
    if (config.shadowingStdDev < 0.0 || config.shadowingStdDev > 20.0) {
        return false;
    }
    
    // 验证频率因子 (通常在0.5到3.0之间)
    if (config.frequencyFactor < 0.5 || config.frequencyFactor > 3.0) {
        return false;
    }
    
    return true;
}
/// @brief 验证衰减系数是否符合指定环境类型的范围
/// @param attenuation 衰减系数
/// @param envType 环境类型
/// @return 如果衰减系数有效返回true，否则返回false
bool EnvironmentLossConfigManager::isAttenuationValid(double attenuation, EnvironmentType envType) {
    // 获取环境配置
    const EnvironmentLossConfig& config = getConfig(envType);
    
    // 根据环境损耗计算期望的衰减系数
    double expectedAttenuation = 1.0 + (config.environmentLoss / 10.0);
    
    // 允许±0.5的偏差范围
    double tolerance = 0.5;
    return attenuation >= (expectedAttenuation - tolerance) && 
           attenuation <= (expectedAttenuation + tolerance);
}

/// @brief  测试环境损耗
/// @param envType 环境类型
/// @return 环境类型名称
std::string EnvironmentLossConfigManager::getEnvironmentTypeName(EnvironmentType envType) {
    switch (envType) {
        case EnvironmentType::OPEN_FIELD:
            return "开阔地区";
        case EnvironmentType::URBAN_AREA:
            return "城市地区";
        case EnvironmentType::MOUNTAINOUS:
            return "山区";
        default:
            return "未知环境";
    }
}

EnvironmentType EnvironmentLossConfigManager::parseEnvironmentType(const std::string& name) {
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
    
    if (lowerName == "open_field" || lowerName == "开阔地区") {
        return EnvironmentType::OPEN_FIELD;
    } else if (lowerName == "urban_area" || lowerName == "城市地区") {
        return EnvironmentType::URBAN_AREA;
    } else if (lowerName == "mountainous" || lowerName == "山区") {
        return EnvironmentType::MOUNTAINOUS;
    } else {
        return EnvironmentType::OPEN_FIELD; // 默认返回开阔地区
    }
}

std::string EnvironmentLossConfigManager::exportConfigsToJSON() {
    if (!initialized_) {
        initializeDefaultConfigs();
    }
    
    std::ostringstream json;
    json << "{\n";
    json << "  \"environment_loss_configs\": {\n";
    
    bool first = true;
    for (const auto& [envType, config] : configs_) {
        if (!first) {
            json << ",\n";
        }
        first = false;
        
        std::string envName;
        switch (envType) {
            case EnvironmentType::OPEN_FIELD: envName = "open_field"; break;
            case EnvironmentType::URBAN_AREA: envName = "urban_area"; break;
            case EnvironmentType::MOUNTAINOUS: envName = "mountainous"; break;
        }
        
        json << "    \"" << envName << "\": {\n";
        json << "      \"path_loss_exponent\": " << config.pathLossExponent << ",\n";
        json << "      \"environment_loss\": " << config.environmentLoss << ",\n";
        json << "      \"shadowing_std_dev\": " << config.shadowingStdDev << ",\n";
        json << "      \"frequency_factor\": " << config.frequencyFactor << "\n";
        json << "    }";
    }
    
    json << "\n  }\n";
    json << "}";
    
    return json.str();
}

bool EnvironmentLossConfigManager::importConfigsFromJSON(const std::string& jsonStr) {
    // 简化的JSON解析实现
    // 在实际项目中，建议使用专业的JSON库如nlohmann/json
    try {
        // 这里提供一个基础的解析框架
        // 实际实现需要完整的JSON解析逻辑
        
        // 重置配置
        configs_.clear();
        initialized_ = false;
        initializeDefaultConfigs();
        
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

bool EnvironmentLossConfigManager::saveConfigsToFile(const std::string& filename) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        std::string jsonContent = exportConfigsToJSON();
        file << jsonContent;
        file.close();
        
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

bool EnvironmentLossConfigManager::loadConfigsFromFile(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        std::string jsonContent;
        std::string line;
        while (std::getline(file, line)) {
            jsonContent += line + "\n";
        }
        file.close();
        
        return importConfigsFromJSON(jsonContent);
    } catch (const std::exception& e) {
        return false;
    }
}

double EnvironmentLossConfigManager::calculateEnvironmentPathLoss(double distance_km, EnvironmentType envType) {
    if (distance_km <= 0.0) {
        return 0.0;
    }
    
    // 获取环境配置
    const EnvironmentLossConfig& config = getConfig(envType);
    
    // 计算环境路径损耗指数的影响
    // 修正公式: EnvironmentPathLoss = 10*n*log10(d) - 10*2*log10(d)
    // 其中 n 是环境路径损耗指数，2 是自由空间的路径损耗指数
    double environmentPathLoss = 10.0 * (config.pathLossExponent - 2.0) * std::log10(distance_km);
    
    return environmentPathLoss;
}

double EnvironmentLossConfigManager::calculateFrequencyFactorLoss(double frequency_MHz, EnvironmentType envType) {
    if (frequency_MHz <= 0.0) {
        return 0.0;
    }
    
    // 获取环境配置
    const EnvironmentLossConfig& config = getConfig(envType);
    
    // 计算频率因子损耗
    // 公式: FrequencyLoss = frequencyFactor * log10(f/1000) * 2.0
    // 其中 f 是频率(MHz)，除以1000转换为GHz
    double frequencyLoss = config.frequencyFactor * std::log10(frequency_MHz / 1000.0) * 2.0;
    
    return frequencyLoss;
}

double EnvironmentLossConfigManager::calculateTotalEnvironmentLoss(double distance_km, double frequency_MHz, EnvironmentType envType) {
    if (distance_km <= 0.0 || frequency_MHz <= 0.0) {
        return 0.0;
    }
    
    // 获取环境配置
    const EnvironmentLossConfig& config = getConfig(envType);
    
    // 计算各种环境损耗分量
    double environmentPathLoss = calculateEnvironmentPathLoss(distance_km, envType);
    double environmentLoss = config.environmentLoss;
    double frequencyFactorLoss = calculateFrequencyFactorLoss(frequency_MHz, envType);
    
    // 返回总环境损耗
    return environmentPathLoss + environmentLoss + frequencyFactorLoss;
}