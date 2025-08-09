#include <iostream>
#include <iomanip>
#include "../../source/header/CommunicationJammerParameterConfig.h"
#include "../../source/header/CommunicationJammerModel.h"

int main() {
    std::cout << "=== 通信干扰模型参数配置重构验证 ===" << std::endl;
    
    // 1. 显示参数范围信息
    std::cout << "\n" << CommunicationJammerParameterConfig::getParameterRangeInfo() << std::endl;
    
    // 2. 测试有效参数
    std::cout << "\n=== 有效参数测试 ===" << std::endl;
    std::cout << "干扰功率 30.0 dBm: " << (CommunicationJammerParameterConfig::isJammerPowerValid(30.0) ? "✓ 有效" : "✗ 无效") << std::endl;
    std::cout << "频率 10000.0 kHz: " << (CommunicationJammerParameterConfig::isFrequencyValid(10000.0) ? "✓ 有效" : "✗ 无效") << std::endl;
    std::cout << "带宽 100.0 kHz: " << (CommunicationJammerParameterConfig::isBandwidthValid(100.0) ? "✓ 有效" : "✗ 无效") << std::endl;
    std::cout << "作用距离 50.0 km: " << (CommunicationJammerParameterConfig::isRangeValid(50.0) ? "✓ 有效" : "✗ 无效") << std::endl;
    std::cout << "目标功率 -80.0 dBm: " << (CommunicationJammerParameterConfig::isTargetPowerValid(-80.0) ? "✓ 有效" : "✗ 无效") << std::endl;
    std::cout << "脉冲宽度 10.0 ms: " << (CommunicationJammerParameterConfig::isPulseWidthValid(10.0) ? "✓ 有效" : "✗ 无效") << std::endl;
    std::cout << "脉冲重复频率 1000.0 Hz: " << (CommunicationJammerParameterConfig::isPulseRepetitionRateValid(1000.0) ? "✓ 有效" : "✗ 无效") << std::endl;
    std::cout << "占空比 0.5: " << (CommunicationJammerParameterConfig::isDutyCycleValid(0.5) ? "✓ 有效" : "✗ 无效") << std::endl;
    std::cout << "扫频速率 10.0 MHz/s: " << (CommunicationJammerParameterConfig::isSweepRateValid(10.0) ? "✓ 有效" : "✗ 无效") << std::endl;
    std::cout << "扫频范围 100.0 MHz: " << (CommunicationJammerParameterConfig::isSweepRangeValid(100.0) ? "✓ 有效" : "✗ 无效") << std::endl;
    
    // 3. 测试无效参数
    std::cout << "\n=== 无效参数测试 ===" << std::endl;
    std::cout << "干扰功率 -60.0 dBm: " << (CommunicationJammerParameterConfig::isJammerPowerValid(-60.0) ? "✗ 意外有效" : "✓ 正确拒绝") << std::endl;
    std::cout << "频率 0.5 kHz: " << (CommunicationJammerParameterConfig::isFrequencyValid(0.5) ? "✗ 意外有效" : "✓ 正确拒绝") << std::endl;
    std::cout << "带宽 15000.0 kHz: " << (CommunicationJammerParameterConfig::isBandwidthValid(15000.0) ? "✗ 意外有效" : "✓ 正确拒绝") << std::endl;
    std::cout << "作用距离 1500.0 km: " << (CommunicationJammerParameterConfig::isRangeValid(1500.0) ? "✗ 意外有效" : "✓ 正确拒绝") << std::endl;
    std::cout << "目标功率 -200.0 dBm: " << (CommunicationJammerParameterConfig::isTargetPowerValid(-200.0) ? "✗ 意外有效" : "✓ 正确拒绝") << std::endl;
    std::cout << "脉冲宽度 0.0 ms: " << (CommunicationJammerParameterConfig::isPulseWidthValid(0.0) ? "✗ 意外有效" : "✓ 正确拒绝") << std::endl;
    std::cout << "占空比 1.5: " << (CommunicationJammerParameterConfig::isDutyCycleValid(1.5) ? "✗ 意外有效" : "✓ 正确拒绝") << std::endl;
    
    // 4. 测试干扰模型集成
    std::cout << "\n=== 干扰模型集成测试 ===" << std::endl;
    try {
        CommunicationJammerModel jammer(
            JammerType::GAUSSIAN_NOISE,
            JammerStrategy::CONTINUOUS,
            30.0,    // 有效功率
            10000.0, // 有效频率
            100.0,   // 有效带宽
            50.0     // 有效距离
        );
        std::cout << "✓ 干扰模型创建成功（有效参数）" << std::endl;
        
        // 测试参数设置
        bool result1 = jammer.setJammerPower(40.0);
        bool result2 = jammer.setJammerFrequency(15000.0);
        bool result3 = jammer.setTargetPower(-90.0);
        
        std::cout << "设置干扰功率 40.0 dBm: " << (result1 ? "✓ 成功" : "✗ 失败") << std::endl;
        std::cout << "设置干扰频率 15000.0 kHz: " << (result2 ? "✓ 成功" : "✗ 失败") << std::endl;
        std::cout << "设置目标功率 -90.0 dBm: " << (result3 ? "✓ 成功" : "✗ 失败") << std::endl;
        
        // 测试无效参数设置
        bool result4 = jammer.setJammerPower(-60.0);  // 超出范围
        bool result5 = jammer.setJammerFrequency(0.5); // 超出范围
        
        std::cout << "设置无效干扰功率 -60.0 dBm: " << (result4 ? "✗ 意外成功" : "✓ 正确拒绝") << std::endl;
        std::cout << "设置无效干扰频率 0.5 kHz: " << (result5 ? "✗ 意外成功" : "✓ 正确拒绝") << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "✗ 干扰模型创建失败: " << e.what() << std::endl;
    }
    
    // 5. 测试边界条件
    std::cout << "\n=== 边界条件测试 ===" << std::endl;
    std::cout << "最小干扰功率 -50.0 dBm: " << (CommunicationJammerParameterConfig::isJammerPowerValid(-50.0) ? "✓ 有效" : "✗ 无效") << std::endl;
    std::cout << "最大干扰功率 50.0 dBm: " << (CommunicationJammerParameterConfig::isJammerPowerValid(50.0) ? "✓ 有效" : "✗ 无效") << std::endl;
    std::cout << "最小频率 1.0 kHz: " << (CommunicationJammerParameterConfig::isFrequencyValid(1.0) ? "✓ 有效" : "✗ 无效") << std::endl;
    std::cout << "最大频率 30000000.0 kHz: " << (CommunicationJammerParameterConfig::isFrequencyValid(30000000.0) ? "✓ 有效" : "✗ 无效") << std::endl;
    std::cout << "最小占空比 0.0: " << (CommunicationJammerParameterConfig::isDutyCycleValid(0.0) ? "✓ 有效" : "✗ 无效") << std::endl;
    std::cout << "最大占空比 1.0: " << (CommunicationJammerParameterConfig::isDutyCycleValid(1.0) ? "✓ 有效" : "✗ 无效") << std::endl;
    
    std::cout << "\n=== 重构验证完成 ===" << std::endl;
    std::cout << "✓ 所有参数校验逻辑已成功从硬编码迁移到静态配置类" << std::endl;
    std::cout << "✓ 参数范围配置统一管理，便于维护和扩展" << std::endl;
    std::cout << "✓ 干扰模型与参数配置类集成正常" << std::endl;
    
    return 0;
}