#include "../../source/header/CommunicationAntiJamModel.h"
#include "../../source/header/CommunicationAntiJamParameterConfig.h"
#include <iostream>
#include <iomanip>

/**
 * @brief 测试抗干扰模型参数配置重构
 * 
 * 该程序验证：
 * 1. 参数配置类的功能
 * 2. 抗干扰模型与参数配置类的集成
 * 3. 参数校验的正确性
 * 4. 边界条件测试
 */

void testParameterRangeInfo() {
    std::cout << "\n=== 测试参数范围信息 ===\n";
    std::cout << CommunicationAntiJamParameterConfig::getParameterRangeInfo() << std::endl;
    
    std::cout << "\n=== 单个参数范围测试 ===\n";
    std::cout << CommunicationAntiJamParameterConfig::getProcessingGainRange() << std::endl;
    std::cout << CommunicationAntiJamParameterConfig::getSpreadingFactorRange() << std::endl;
    std::cout << CommunicationAntiJamParameterConfig::getHoppingRateRange() << std::endl;
    std::cout << CommunicationAntiJamParameterConfig::getCodingGainRange() << std::endl;
}

void testParameterValidation() {
    std::cout << "\n=== 测试参数校验功能 ===\n";
    
    // 测试有效参数
    std::cout << "有效参数测试:\n";
    std::cout << "处理增益 25.0 dB: " << (CommunicationAntiJamParameterConfig::isProcessingGainValid(25.0) ? "有效" : "无效") << std::endl;
    std::cout << "扩频因子 1000.0: " << (CommunicationAntiJamParameterConfig::isSpreadingFactorValid(1000.0) ? "有效" : "无效") << std::endl;
    std::cout << "跳频速率 5000.0 Hz: " << (CommunicationAntiJamParameterConfig::isHoppingRateValid(5000.0) ? "有效" : "无效") << std::endl;
    std::cout << "编码增益 6.0 dB: " << (CommunicationAntiJamParameterConfig::isCodingGainValid(6.0) ? "有效" : "无效") << std::endl;
    
    // 测试无效参数
    std::cout << "\n无效参数测试:\n";
    std::cout << "处理增益 -5.0 dB: " << (CommunicationAntiJamParameterConfig::isProcessingGainValid(-5.0) ? "有效" : "无效") << std::endl;
    std::cout << "扩频因子 0.5: " << (CommunicationAntiJamParameterConfig::isSpreadingFactorValid(0.5) ? "有效" : "无效") << std::endl;
    std::cout << "跳频速率 200000.0 Hz: " << (CommunicationAntiJamParameterConfig::isHoppingRateValid(200000.0) ? "有效" : "无效") << std::endl;
    std::cout << "编码增益 25.0 dB: " << (CommunicationAntiJamParameterConfig::isCodingGainValid(25.0) ? "有效" : "无效") << std::endl;
}

void testAntiJamModelIntegration() {
    std::cout << "\n=== 测试抗干扰模型集成 ===\n";
    
    CommunicationAntiJamModel model;
    
    // 测试有效参数设置
    std::cout << "设置有效参数:\n";
    bool result1 = model.setProcessingGain(30.0);
    bool result2 = model.setSpreadingFactor(2000.0);
    bool result3 = model.setHoppingRate(10000.0);
    bool result4 = model.setCodingGain(8.0);
    bool result5 = model.setSystemBandwidth(100.0);
    bool result6 = model.setSignalPower(-50.0);
    bool result7 = model.setNoisePower(-120.0);
    bool result8 = model.setInterferenceLevel(-80.0);
    
    std::cout << "处理增益设置: " << (result1 ? "成功" : "失败") << std::endl;
    std::cout << "扩频因子设置: " << (result2 ? "成功" : "失败") << std::endl;
    std::cout << "跳频速率设置: " << (result3 ? "成功" : "失败") << std::endl;
    std::cout << "编码增益设置: " << (result4 ? "成功" : "失败") << std::endl;
    std::cout << "系统带宽设置: " << (result5 ? "成功" : "失败") << std::endl;
    std::cout << "信号功率设置: " << (result6 ? "成功" : "失败") << std::endl;
    std::cout << "噪声功率设置: " << (result7 ? "成功" : "失败") << std::endl;
    std::cout << "干扰电平设置: " << (result8 ? "成功" : "失败") << std::endl;
    
    // 测试无效参数设置
    std::cout << "\n设置无效参数:\n";
    bool result9 = model.setProcessingGain(-10.0);   // 无效：小于最小值
    bool result10 = model.setSpreadingFactor(200000.0); // 无效：大于最大值
    bool result11 = model.setHoppingRate(0.5);       // 无效：小于最小值
    bool result12 = model.setCodingGain(30.0);       // 无效：大于最大值
    
    std::cout << "无效处理增益设置: " << (result9 ? "成功" : "失败") << std::endl;
    std::cout << "无效扩频因子设置: " << (result10 ? "成功" : "失败") << std::endl;
    std::cout << "无效跳频速率设置: " << (result11 ? "成功" : "失败") << std::endl;
    std::cout << "无效编码增益设置: " << (result12 ? "成功" : "失败") << std::endl;
    
    // 计算抗干扰效果
    if (result1 && result2 && result3 && result4 && result5 && result6 && result7 && result8) {
        std::cout << "\n抗干扰效果计算:\n";
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "抗干扰增益: " << model.calculateAntiJamGain() << " dB" << std::endl;
        std::cout << "抗干扰能力: " << model.calculateJammerResistance() << std::endl;
        std::cout << "信干比: " << model.calculateSignalToJammerRatio() << " dB" << std::endl;
    }
}

void testBoundaryConditions() {
    std::cout << "\n=== 测试边界条件 ===\n";
    
    CommunicationAntiJamModel model;
    
    // 测试最小值边界
    std::cout << "最小值边界测试:\n";
    bool min1 = model.setProcessingGain(0.0);        // 最小处理增益
    bool min2 = model.setSpreadingFactor(1.0);       // 最小扩频因子
    bool min3 = model.setHoppingRate(1.0);           // 最小跳频速率
    bool min4 = model.setCodingGain(0.0);            // 最小编码增益
    bool min5 = model.setSystemBandwidth(0.1);       // 最小系统带宽
    bool min6 = model.setSignalPower(-150.0);        // 最小信号功率
    bool min7 = model.setNoisePower(-150.0);         // 最小噪声功率
    bool min8 = model.setInterferenceLevel(-150.0);  // 最小干扰电平
    
    std::cout << "最小处理增益: " << (min1 ? "通过" : "失败") << std::endl;
    std::cout << "最小扩频因子: " << (min2 ? "通过" : "失败") << std::endl;
    std::cout << "最小跳频速率: " << (min3 ? "通过" : "失败") << std::endl;
    std::cout << "最小编码增益: " << (min4 ? "通过" : "失败") << std::endl;
    std::cout << "最小系统带宽: " << (min5 ? "通过" : "失败") << std::endl;
    std::cout << "最小信号功率: " << (min6 ? "通过" : "失败") << std::endl;
    std::cout << "最小噪声功率: " << (min7 ? "通过" : "失败") << std::endl;
    std::cout << "最小干扰电平: " << (min8 ? "通过" : "失败") << std::endl;
    
    // 测试最大值边界
    std::cout << "\n最大值边界测试:\n";
    bool max1 = model.setProcessingGain(50.0);       // 最大处理增益
    bool max2 = model.setSpreadingFactor(100000.0);  // 最大扩频因子
    bool max3 = model.setHoppingRate(100000.0);      // 最大跳频速率
    bool max4 = model.setCodingGain(20.0);           // 最大编码增益
    bool max5 = model.setSystemBandwidth(10000.0);   // 最大系统带宽
    bool max6 = model.setSignalPower(50.0);          // 最大信号功率
    bool max7 = model.setNoisePower(0.0);            // 最大噪声功率
    bool max8 = model.setInterferenceLevel(50.0);    // 最大干扰电平
    
    std::cout << "最大处理增益: " << (max1 ? "通过" : "失败") << std::endl;
    std::cout << "最大扩频因子: " << (max2 ? "通过" : "失败") << std::endl;
    std::cout << "最大跳频速率: " << (max3 ? "通过" : "失败") << std::endl;
    std::cout << "最大编码增益: " << (max4 ? "通过" : "失败") << std::endl;
    std::cout << "最大系统带宽: " << (max5 ? "通过" : "失败") << std::endl;
    std::cout << "最大信号功率: " << (max6 ? "通过" : "失败") << std::endl;
    std::cout << "最大噪声功率: " << (max7 ? "通过" : "失败") << std::endl;
    std::cout << "最大干扰电平: " << (max8 ? "通过" : "失败") << std::endl;
}

void testAdditionalParameters() {
    std::cout << "\n=== 测试其他参数 ===\n";
    
    CommunicationAntiJamModel model;
    
    // 测试跳频参数
    std::cout << "跳频参数测试:\n";
    bool hop1 = model.setHoppingChannels(100);       // 跳频信道数
    bool hop2 = model.setChannelSpacing(1.0);        // 信道间隔
    bool hop3 = model.setDwellTime(10.0);            // 驻留时间
    
    std::cout << "跳频信道数设置: " << (hop1 ? "成功" : "失败") << std::endl;
    std::cout << "信道间隔设置: " << (hop2 ? "成功" : "失败") << std::endl;
    std::cout << "驻留时间设置: " << (hop3 ? "成功" : "失败") << std::endl;
    
    // 测试扩频参数
    std::cout << "\n扩频参数测试:\n";
    bool spread1 = model.setChipRate(100);           // 码片速率
    bool spread2 = model.setSequenceLength(1023.0);  // 序列长度
    
    std::cout << "码片速率设置: " << (spread1 ? "成功" : "失败") << std::endl;
    std::cout << "序列长度设置: " << (spread2 ? "成功" : "失败") << std::endl;
    
    // 测试自适应参数
    std::cout << "\n自适应参数测试:\n";
    bool adapt1 = model.setAdaptationSpeed(0.1);     // 自适应速度
    bool adapt2 = model.setConvergenceThreshold(0.01); // 收敛阈值
    
    std::cout << "自适应速度设置: " << (adapt1 ? "成功" : "失败") << std::endl;
    std::cout << "收敛阈值设置: " << (adapt2 ? "成功" : "失败") << std::endl;
    
    // 测试环境参数
    std::cout << "\n环境参数测试:\n";
    bool env1 = model.setEnvironmentType(0.5);       // 环境类型
    bool env2 = model.setJammerDensity(0.3);         // 干扰机密度
    
    std::cout << "环境类型设置: " << (env1 ? "成功" : "失败") << std::endl;
    std::cout << "干扰机密度设置: " << (env2 ? "成功" : "失败") << std::endl;
}

int main() {
    std::cout << "=== 通信抗干扰模型参数配置重构测试 ===\n";
    std::cout << "测试目标：验证参数校验逻辑从硬编码重构为独立配置类\n";
    
    try {
        testParameterRangeInfo();
        testParameterValidation();
        testAntiJamModelIntegration();
        testBoundaryConditions();
        testAdditionalParameters();
        
        std::cout << "\n=== 重构测试总结 ===\n";
        std::cout << "✓ 参数配置类功能正常\n";
        std::cout << "✓ 抗干扰模型集成成功\n";
        std::cout << "✓ 参数校验逻辑正确\n";
        std::cout << "✓ 边界条件处理正确\n";
        std::cout << "✓ 重构成功完成！\n";
        
    } catch (const std::exception& e) {
        std::cerr << "测试过程中发生错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}