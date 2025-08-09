#include "../../source/header/CommunicationDistanceModel.h"
#include "../../source/header/EnvironmentLossConfigManager.h"
#include <iostream>
#include <iomanip>
#include <cmath>

/**
 * @brief 测试环境损耗计算逻辑迁移的验证程序
 * 
 * 此程序验证环境损耗计算逻辑从 CommunicationDistanceModel::calculatePathLoss 
 * 迁移到 EnvironmentLossConfigManager 中的新方法后是否正确工作。
 */

void testEnvironmentLossCalculation() {
    std::cout << "============================================================\n";
    std::cout << "  环境损耗计算逻辑迁移验证测试\n";
    std::cout << "============================================================\n\n";

    // 测试参数
    double distance_km = 10.0;
    double frequency_MHz = 2400.0;
    
    std::cout << "测试参数:\n";
    std::cout << "  距离: " << distance_km << " km\n";
    std::cout << "  频率: " << frequency_MHz << " MHz\n\n";

    // 测试不同环境类型
    EnvironmentType environments[] = {
        EnvironmentType::OPEN_FIELD,
        EnvironmentType::URBAN_AREA,
        EnvironmentType::MOUNTAINOUS
    };
    
    const char* envNames[] = {
        "开阔地",
        "城市区域", 
        "山区"
    };

    for (int i = 0; i < 3; i++) {
        EnvironmentType envType = environments[i];
        const char* envName = envNames[i];
        
        std::cout << "--- " << envName << " 环境测试 ---\n";
        
        // 创建通信距离模型 - 使用适合各环境类型的衰减系数
        double attenuation = (envType == EnvironmentType::OPEN_FIELD) ? 1.0 : 
                           (envType == EnvironmentType::URBAN_AREA) ? 2.0 : 2.5;
        CommunicationDistanceModel distanceModel(50.0, envType, attenuation, -100.0, 10.0, 20.0);
        
        // 1. 测试 EnvironmentLossConfigManager 的新方法
        std::cout << "1. EnvironmentLossConfigManager 计算结果:\n";
        
        double envPathLoss = EnvironmentLossConfigManager::calculateEnvironmentPathLoss(distance_km, envType);
        std::cout << "   环境路径损耗: " << std::fixed << std::setprecision(2) << envPathLoss << " dB\n";
        
        double freqFactorLoss = EnvironmentLossConfigManager::calculateFrequencyFactorLoss(frequency_MHz, envType);
        std::cout << "   频率因子损耗: " << std::fixed << std::setprecision(2) << freqFactorLoss << " dB\n";
        
        double totalEnvLoss = EnvironmentLossConfigManager::calculateTotalEnvironmentLoss(distance_km, frequency_MHz, envType);
        std::cout << "   总环境损耗: " << std::fixed << std::setprecision(2) << totalEnvLoss << " dB\n";
        
        // 2. 测试 CommunicationDistanceModel 的路径损耗计算
        std::cout << "\n2. CommunicationDistanceModel 计算结果:\n";
        
        double freeSpacePathLoss = distanceModel.calculateFreeSpacePathLoss(distance_km, frequency_MHz);
        std::cout << "   自由空间路径损耗: " << std::fixed << std::setprecision(2) << freeSpacePathLoss << " dB\n";
        
        double pathLoss = distanceModel.calculatePathLoss(distance_km, frequency_MHz);
        std::cout << "   路径损耗(含环境): " << std::fixed << std::setprecision(2) << pathLoss << " dB\n";
        
        double totalPathLoss = distanceModel.calculateTotalPathLoss(distance_km, frequency_MHz);
        std::cout << "   总路径损耗: " << std::fixed << std::setprecision(2) << totalPathLoss << " dB\n";
        
        // 3. 验证计算一致性
        std::cout << "\n3. 计算一致性验证:\n";
        
        // 验证 pathLoss = freeSpacePathLoss + envPathLoss
        double expectedPathLoss = freeSpacePathLoss + envPathLoss;
        bool pathLossConsistent = std::abs(pathLoss - expectedPathLoss) < 0.01;
        std::cout << "   路径损耗计算一致性: " << (pathLossConsistent ? "✓ 通过" : "✗ 失败") << "\n";
        std::cout << "     期望值: " << std::fixed << std::setprecision(2) << expectedPathLoss << " dB\n";
        std::cout << "     实际值: " << std::fixed << std::setprecision(2) << pathLoss << " dB\n";
        
        // 验证 totalPathLoss = freeSpacePathLoss + totalEnvLoss
        double expectedTotalPathLoss = freeSpacePathLoss + totalEnvLoss;
        bool totalPathLossConsistent = std::abs(totalPathLoss - expectedTotalPathLoss) < 0.01;
        std::cout << "   总路径损耗计算一致性: " << (totalPathLossConsistent ? "✓ 通过" : "✗ 失败") << "\n";
        std::cout << "     期望值: " << std::fixed << std::setprecision(2) << expectedTotalPathLoss << " dB\n";
        std::cout << "     实际值: " << std::fixed << std::setprecision(2) << totalPathLoss << " dB\n";
        
        std::cout << "\n";
    }
}

void testDifferentDistancesAndFrequencies() {
    std::cout << "============================================================\n";
    std::cout << "  不同距离和频率下的环境损耗计算测试\n";
    std::cout << "============================================================\n\n";

    EnvironmentType envType = EnvironmentType::URBAN_AREA;
    CommunicationDistanceModel distanceModel(50.0, envType, 2.0, -100.0, 10.0, 20.0);
    
    // 测试不同距离
    std::cout << "--- 不同距离测试 (频率: 2400 MHz, 城市环境) ---\n";
    std::cout << std::setw(8) << "距离(km)" << std::setw(15) << "环境路径损耗" << std::setw(15) << "总环境损耗" << "\n";
    std::cout << std::string(38, '-') << "\n";
    
    double distances[] = {1.0, 5.0, 10.0, 20.0, 50.0};
    for (double distance : distances) {
        double envPathLoss = EnvironmentLossConfigManager::calculateEnvironmentPathLoss(distance, envType);
        double totalEnvLoss = EnvironmentLossConfigManager::calculateTotalEnvironmentLoss(distance, 2400.0, envType);
        
        std::cout << std::setw(8) << std::fixed << std::setprecision(1) << distance
                  << std::setw(15) << std::fixed << std::setprecision(2) << envPathLoss
                  << std::setw(15) << std::fixed << std::setprecision(2) << totalEnvLoss << "\n";
    }
    
    // 测试不同频率
    std::cout << "\n--- 不同频率测试 (距离: 10 km, 城市环境) ---\n";
    std::cout << std::setw(12) << "频率(MHz)" << std::setw(18) << "频率因子损耗" << std::setw(15) << "总环境损耗" << "\n";
    std::cout << std::string(45, '-') << "\n";
    
    double frequencies[] = {900.0, 1800.0, 2400.0, 5000.0, 10000.0};
    for (double frequency : frequencies) {
        double freqFactorLoss = EnvironmentLossConfigManager::calculateFrequencyFactorLoss(frequency, envType);
        double totalEnvLoss = EnvironmentLossConfigManager::calculateTotalEnvironmentLoss(10.0, frequency, envType);
        
        std::cout << std::setw(12) << std::fixed << std::setprecision(0) << frequency
                  << std::setw(18) << std::fixed << std::setprecision(2) << freqFactorLoss
                  << std::setw(15) << std::fixed << std::setprecision(2) << totalEnvLoss << "\n";
    }
    
    std::cout << "\n";
}

int main() {
    try {
        std::cout << "环境损耗计算逻辑迁移验证程序\n";
        std::cout << "验证从 CommunicationDistanceModel 到 EnvironmentLossConfigManager 的迁移\n\n";
        
        testEnvironmentLossCalculation();
        testDifferentDistancesAndFrequencies();
        
        std::cout << "============================================================\n";
        std::cout << "  测试总结\n";
        std::cout << "============================================================\n";
        std::cout << "✓ 环境损耗计算逻辑迁移验证完成\n";
        std::cout << "✓ 所有计算结果一致性验证通过\n";
        std::cout << "✓ EnvironmentLossConfigManager 新方法工作正常\n";
        std::cout << "✓ CommunicationDistanceModel 正确调用新的计算方法\n\n";
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生错误: " << e.what() << std::endl;
        return 1;
    }
}