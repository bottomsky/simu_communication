#include "CommunicationAntiJamModel.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <chrono>

// 简单的测试框架宏
#define EXPECT_TRUE(condition) \
    if (!(condition)) { \
        std::cerr << "FAIL: " << #condition << " at line " << __LINE__ << std::endl; \
        return false; \
    }

#define EXPECT_FALSE(condition) \
    if (condition) { \
        std::cerr << "FAIL: " << #condition << " should be false at line " << __LINE__ << std::endl; \
        return false; \
    }

#define EXPECT_NEAR(actual, expected, tolerance) \
    if (std::abs((actual) - (expected)) > (tolerance)) { \
        std::cerr << "FAIL: " << #actual << " = " << (actual) \
                  << ", expected " << (expected) << " ± " << (tolerance) \
                  << " at line " << __LINE__ << std::endl; \
        return false; \
    }

#define EXPECT_GT(actual, expected) \
    if (!((actual) > (expected))) { \
        std::cerr << "FAIL: " << #actual << " = " << (actual) \
                  << " should be > " << (expected) \
                  << " at line " << __LINE__ << std::endl; \
        return false; \
    }

#define EXPECT_LT(actual, expected) \
    if (!((actual) < (expected))) { \
        std::cerr << "FAIL: " << #actual << " = " << (actual) \
                  << " should be < " << (expected) \
                  << " at line " << __LINE__ << std::endl; \
        return false; \
    }

#define EXPECT_GE(actual, expected) \
    if (!((actual) >= (expected))) { \
        std::cerr << "FAIL: " << #actual << " = " << (actual) \
                  << " should be >= " << (expected) \
                  << " at line " << __LINE__ << std::endl; \
        return false; \
    }

#define EXPECT_LE(actual, expected) \
    if (!((actual) <= (expected))) { \
        std::cerr << "FAIL: " << #actual << " = " << (actual) \
                  << " should be <= " << (expected) \
                  << " at line " << __LINE__ << std::endl; \
        return false; \
    }

// 测试构造函数和基本参数设置
bool testConstructorAndBasicParameters() {
    std::cout << "测试: 构造函数和基本参数设置..." << std::endl;
    
    // 测试默认构造函数
    CommunicationAntiJamModel antiJam;
    EXPECT_TRUE(antiJam.getAntiJamTechnique() == AntiJamTechnique::FREQUENCY_HOPPING);
    EXPECT_TRUE(antiJam.getAntiJamStrategy() == AntiJamStrategy::ADAPTIVE);
    EXPECT_NEAR(antiJam.getProcessingGain(), 20.0, 0.01);
    EXPECT_NEAR(antiJam.getSpreadingFactor(), 1000.0, 0.01);
    EXPECT_NEAR(antiJam.getHoppingRate(), 1000.0, 0.01);
    EXPECT_NEAR(antiJam.getCodingGain(), 3.0, 0.01);
    
    // 测试带参数的构造函数
    CommunicationAntiJamModel antiJam2(AntiJamTechnique::DIRECT_SEQUENCE,
                                       AntiJamStrategy::ACTIVE,
                                       25.0, 2000.0, 2000.0, 5.0);
    EXPECT_TRUE(antiJam2.getAntiJamTechnique() == AntiJamTechnique::DIRECT_SEQUENCE);
    EXPECT_TRUE(antiJam2.getAntiJamStrategy() == AntiJamStrategy::ACTIVE);
    EXPECT_NEAR(antiJam2.getProcessingGain(), 25.0, 0.01);
    EXPECT_NEAR(antiJam2.getSpreadingFactor(), 2000.0, 0.01);
    EXPECT_NEAR(antiJam2.getHoppingRate(), 2000.0, 0.01);
    EXPECT_NEAR(antiJam2.getCodingGain(), 5.0, 0.01);
    
    std::cout << "✓ 构造函数和基本参数设置测试通过" << std::endl;
    return true;
}

// 测试参数设置和校验
bool testParameterValidation() {
    std::cout << "测试: 参数校验..." << std::endl;
    
    CommunicationAntiJamModel antiJam;
    
    // 测试处理增益范围
    EXPECT_TRUE(antiJam.setProcessingGain(0.0));
    EXPECT_TRUE(antiJam.setProcessingGain(25.0));
    EXPECT_TRUE(antiJam.setProcessingGain(50.0));
    EXPECT_FALSE(antiJam.setProcessingGain(-1.0)); // 超出下限
    EXPECT_FALSE(antiJam.setProcessingGain(51.0)); // 超出上限
    
    // 测试扩频因子范围
    EXPECT_TRUE(antiJam.setSpreadingFactor(1.0));
    EXPECT_TRUE(antiJam.setSpreadingFactor(1000.0));
    EXPECT_TRUE(antiJam.setSpreadingFactor(100000.0));
    EXPECT_FALSE(antiJam.setSpreadingFactor(0.5)); // 超出下限
    EXPECT_FALSE(antiJam.setSpreadingFactor(100001.0)); // 超出上限
    
    // 测试跳频速率范围
    EXPECT_TRUE(antiJam.setHoppingRate(1.0));
    EXPECT_TRUE(antiJam.setHoppingRate(1000.0));
    EXPECT_TRUE(antiJam.setHoppingRate(100000.0));
    EXPECT_FALSE(antiJam.setHoppingRate(0.5)); // 超出下限
    EXPECT_FALSE(antiJam.setHoppingRate(100001.0)); // 超出上限
    
    // 测试编码增益范围
    EXPECT_TRUE(antiJam.setCodingGain(0.0));
    EXPECT_TRUE(antiJam.setCodingGain(10.0));
    EXPECT_TRUE(antiJam.setCodingGain(20.0));
    EXPECT_FALSE(antiJam.setCodingGain(-1.0)); // 超出下限
    EXPECT_FALSE(antiJam.setCodingGain(21.0)); // 超出上限
    
    // 测试系统参数
    EXPECT_TRUE(antiJam.setSystemBandwidth(100.0));
    EXPECT_TRUE(antiJam.setSignalPower(-80.0));
    EXPECT_TRUE(antiJam.setNoisePower(-100.0));
    EXPECT_TRUE(antiJam.setInterferenceLevel(-70.0));
    
    std::cout << "✓ 参数校验测试通过" << std::endl;
    return true;
}

// 测试跳频参数
bool testFrequencyHoppingParameters() {
    std::cout << "测试: 跳频参数..." << std::endl;
    
    CommunicationAntiJamModel antiJam;
    antiJam.setAntiJamTechnique(AntiJamTechnique::FREQUENCY_HOPPING);
    
    // 测试跳频信道数
    EXPECT_TRUE(antiJam.setHoppingChannels(2));
    EXPECT_TRUE(antiJam.setHoppingChannels(100));
    EXPECT_TRUE(antiJam.setHoppingChannels(10000));
    EXPECT_FALSE(antiJam.setHoppingChannels(1)); // 超出下限
    EXPECT_FALSE(antiJam.setHoppingChannels(10001)); // 超出上限
    
    // 测试信道间隔
    EXPECT_TRUE(antiJam.setChannelSpacing(0.001));
    EXPECT_TRUE(antiJam.setChannelSpacing(1.0));
    EXPECT_TRUE(antiJam.setChannelSpacing(1000.0));
    EXPECT_FALSE(antiJam.setChannelSpacing(0.0005)); // 超出下限
    EXPECT_FALSE(antiJam.setChannelSpacing(1001.0)); // 超出上限
    
    // 测试驻留时间
    EXPECT_TRUE(antiJam.setDwellTime(0.001));
    EXPECT_TRUE(antiJam.setDwellTime(1.0));
    EXPECT_TRUE(antiJam.setDwellTime(1000.0));
    EXPECT_FALSE(antiJam.setDwellTime(0.0005)); // 超出下限
    EXPECT_FALSE(antiJam.setDwellTime(1001.0)); // 超出上限
    
    std::cout << "✓ 跳频参数测试通过" << std::endl;
    return true;
}

// 测试扩频参数
bool testSpreadSpectrumParameters() {
    std::cout << "测试: 扩频参数..." << std::endl;
    
    CommunicationAntiJamModel antiJam;
    antiJam.setAntiJamTechnique(AntiJamTechnique::DIRECT_SEQUENCE);
    
    // 测试码片速率
    EXPECT_TRUE(antiJam.setChipRate(1));
    EXPECT_TRUE(antiJam.setChipRate(10));
    EXPECT_TRUE(antiJam.setChipRate(1000));
    EXPECT_FALSE(antiJam.setChipRate(0)); // 超出下限
    EXPECT_FALSE(antiJam.setChipRate(1001)); // 超出上限
    
    // 测试序列长度
    EXPECT_TRUE(antiJam.setSequenceLength(7.0));
    EXPECT_TRUE(antiJam.setSequenceLength(1023.0));
    EXPECT_TRUE(antiJam.setSequenceLength(1000000.0));
    EXPECT_FALSE(antiJam.setSequenceLength(6.0)); // 超出下限
    EXPECT_FALSE(antiJam.setSequenceLength(1000001.0)); // 超出上限
    
    std::cout << "✓ 扩频参数测试通过" << std::endl;
    return true;
}

// 测试自适应参数
bool testAdaptiveParameters() {
    std::cout << "测试: 自适应参数..." << std::endl;
    
    CommunicationAntiJamModel antiJam;
    antiJam.setAntiJamStrategy(AntiJamStrategy::ADAPTIVE);
    
    // 测试自适应速度
    EXPECT_TRUE(antiJam.setAdaptationSpeed(0.001));
    EXPECT_TRUE(antiJam.setAdaptationSpeed(0.1));
    EXPECT_TRUE(antiJam.setAdaptationSpeed(1.0));
    EXPECT_FALSE(antiJam.setAdaptationSpeed(0.0005)); // 超出下限
    EXPECT_FALSE(antiJam.setAdaptationSpeed(1.1)); // 超出上限
    
    // 测试收敛阈值
    EXPECT_TRUE(antiJam.setConvergenceThreshold(0.0001));
    EXPECT_TRUE(antiJam.setConvergenceThreshold(0.01));
    EXPECT_TRUE(antiJam.setConvergenceThreshold(0.1));
    EXPECT_FALSE(antiJam.setConvergenceThreshold(0.00005)); // 超出下限
    EXPECT_FALSE(antiJam.setConvergenceThreshold(0.11)); // 超出上限
    
    std::cout << "✓ 自适应参数测试通过" << std::endl;
    return true;
}

// 测试抗干扰增益计算
bool testAntiJamGainCalculation() {
    std::cout << "测试: 抗干扰增益计算..." << std::endl;
    
    CommunicationAntiJamModel antiJam;
    antiJam.setProcessingGain(20.0);
    antiJam.setAntiJamTechnique(AntiJamTechnique::FREQUENCY_HOPPING);
    antiJam.setHoppingChannels(100);
    
    double gain = antiJam.calculateAntiJamGain();
    
    // 抗干扰增益应该在合理范围内
    EXPECT_GT(gain, 0.0);
    EXPECT_LT(gain, 100.0);
    
    // 测试不同技术的增益差异
    antiJam.setAntiJamTechnique(AntiJamTechnique::DIRECT_SEQUENCE);
    antiJam.setSpreadingFactor(1000.0);
    double dsGain = antiJam.calculateAntiJamGain();
    
    antiJam.setAntiJamTechnique(AntiJamTechnique::HYBRID_SPREAD);
    double hybridGain = antiJam.calculateAntiJamGain();
    
    // 混合扩频的增益应该介于跳频和直扩之间
    EXPECT_GT(hybridGain, std::min(gain, dsGain));
    
    std::cout << "✓ 抗干扰增益计算测试通过" << std::endl;
    return true;
}

// 测试抗干扰能力计算
bool testJammerResistanceCalculation() {
    std::cout << "测试: 抗干扰能力计算..." << std::endl;
    
    CommunicationAntiJamModel antiJam;
    antiJam.setSignalPower(-80.0);
    antiJam.setNoisePower(-100.0);
    antiJam.setInterferenceLevel(-70.0);
    antiJam.setProcessingGain(20.0);
    
    double resistance = antiJam.calculateJammerResistance();
    
    // 抗干扰能力应该在0-1范围内
    EXPECT_GE(resistance, 0.0);
    EXPECT_LE(resistance, 1.0);
    
    // 测试增加处理增益对抗干扰能力的影响
    antiJam.setProcessingGain(30.0);
    double higherResistance = antiJam.calculateJammerResistance();
    
    EXPECT_GE(higherResistance, resistance); // 更高的处理增益应该有更好的抗干扰能力
    
    std::cout << "✓ 抗干扰能力计算测试通过" << std::endl;
    return true;
}

// 测试信干比计算
bool testSignalToJammerRatioCalculation() {
    std::cout << "测试: 信干比计算..." << std::endl;
    
    CommunicationAntiJamModel antiJam;
    antiJam.setSignalPower(-80.0);
    antiJam.setInterferenceLevel(-70.0);
    antiJam.setProcessingGain(20.0);
    
    double sjr = antiJam.calculateSignalToJammerRatio();
    
    // 信干比应该在合理范围内
    EXPECT_GT(sjr, -50.0);
    EXPECT_LT(sjr, 50.0);
    
    // 测试干扰电平对信干比的影响
    antiJam.setInterferenceLevel(-60.0); // 更强干扰
    double sjr_strong = antiJam.calculateSignalToJammerRatio();
    
    antiJam.setInterferenceLevel(-80.0); // 更弱干扰
    double sjr_weak = antiJam.calculateSignalToJammerRatio();
    
    EXPECT_GT(sjr_weak, sjr_strong); // 更弱的干扰应该有更高的信干比
    
    std::cout << "✓ 信干比计算测试通过" << std::endl;
    return true;
}

// 测试误码率计算
bool testBitErrorRateCalculation() {
    std::cout << "测试: 误码率计算..." << std::endl;
    
    CommunicationAntiJamModel antiJam;
    antiJam.setSignalPower(-80.0);
    antiJam.setNoisePower(-100.0);
    antiJam.setInterferenceLevel(-70.0);
    antiJam.setProcessingGain(20.0);
    
    double ber = antiJam.calculateBitErrorRateWithJamming();
    
    // 误码率应该在0-1范围内
    EXPECT_GE(ber, 0.0);
    EXPECT_LE(ber, 1.0);
    
    // 测试增加抗干扰增益对误码率的影响
    antiJam.setProcessingGain(30.0);
    double ber_improved = antiJam.calculateBitErrorRateWithJamming();
    
    EXPECT_LE(ber_improved, ber); // 更高的抗干扰增益应该有更低的误码率
    
    std::cout << "✓ 误码率计算测试通过" << std::endl;
    return true;
}

// 测试不同抗干扰技术的效果
bool testDifferentAntiJamTechniques() {
    std::cout << "测试: 不同抗干扰技术效果..." << std::endl;
    
    std::vector<AntiJamTechnique> techniques = {
        AntiJamTechnique::FREQUENCY_HOPPING,
        AntiJamTechnique::DIRECT_SEQUENCE,
        AntiJamTechnique::TIME_HOPPING,
        AntiJamTechnique::HYBRID_SPREAD,
        AntiJamTechnique::ADAPTIVE_FILTERING,
        AntiJamTechnique::BEAM_FORMING,
        AntiJamTechnique::POWER_CONTROL,
        AntiJamTechnique::ERROR_CORRECTION,
        AntiJamTechnique::DIVERSITY_RECEPTION,
        AntiJamTechnique::INTERFERENCE_CANCELLATION
    };
    
    for (auto technique : techniques) {
        CommunicationAntiJamModel antiJam;
        antiJam.setAntiJamTechnique(technique);
        antiJam.setSignalPower(-80.0);
        antiJam.setInterferenceLevel(-70.0);
        antiJam.setProcessingGain(20.0);
        
        double gain = antiJam.calculateAntiJamGain();
        double resistance = antiJam.calculateJammerResistance();
        double effectiveness = antiJam.calculateProtectionEffectiveness();
        
        // 所有技术都应该提供一定的保护
        EXPECT_GT(gain, 0.0);
        EXPECT_GE(resistance, 0.0);
        EXPECT_LE(resistance, 1.0);
        EXPECT_GE(effectiveness, 0.0);
        EXPECT_LE(effectiveness, 1.0);
        
        // 测试抗干扰效果等级评估
        AntiJamEffectLevel level = antiJam.evaluateAntiJamEffect();
        EXPECT_TRUE(level >= AntiJamEffectLevel::NO_PROTECTION && 
                   level <= AntiJamEffectLevel::EXCELLENT_PROTECTION);
    }
    
    std::cout << "✓ 不同抗干扰技术效果测试通过" << std::endl;
    return true;
}

// 测试抗干扰策略的影响
bool testAntiJamStrategyEffect() {
    std::cout << "测试: 抗干扰策略效果..." << std::endl;
    
    std::vector<AntiJamStrategy> strategies = {
        AntiJamStrategy::PASSIVE,
        AntiJamStrategy::ACTIVE,
        AntiJamStrategy::ADAPTIVE,
        AntiJamStrategy::COOPERATIVE,
        AntiJamStrategy::COGNITIVE
    };
    
    CommunicationAntiJamModel antiJam;
    antiJam.setAntiJamTechnique(AntiJamTechnique::FREQUENCY_HOPPING);
    antiJam.setProcessingGain(20.0);
    
    double previousGain = 0.0;
    
    for (auto strategy : strategies) {
        antiJam.setAntiJamStrategy(strategy);
        double gain = antiJam.calculateAntiJamGain();
        
        // 更高级的策略应该有更好的效果
        if (strategy != AntiJamStrategy::PASSIVE) {
            EXPECT_GE(gain, previousGain);
        }
        previousGain = gain;
    }
    
    std::cout << "✓ 抗干扰策略效果测试通过" << std::endl;
    return true;
}

// 测试最优策略计算
bool testOptimalStrategyCalculation() {
    std::cout << "测试: 最优策略计算..." << std::endl;
    
    CommunicationAntiJamModel antiJam;
    antiJam.setSignalPower(-80.0);
    antiJam.setInterferenceLevel(-70.0);
    antiJam.setSystemBandwidth(100.0);
    
    AntiJamTechnique optimalTech = antiJam.calculateOptimalTechnique();
    double optimalGain = antiJam.calculateOptimalProcessingGain();
    double optimalRate = antiJam.calculateOptimalHoppingRate();
    int optimalChannels = antiJam.calculateOptimalHoppingChannels();
    
    // 最优参数应该在合理范围内
    EXPECT_GE(optimalGain, 0.0);
    EXPECT_LE(optimalGain, 50.0);
    EXPECT_GE(optimalRate, 1.0);
    EXPECT_LE(optimalRate, 100000.0);
    EXPECT_GE(optimalChannels, 2);
    EXPECT_LE(optimalChannels, 10000);
    
    std::cout << "✓ 最优策略计算测试通过" << std::endl;
    return true;
}

// 测试多技术组合效果
bool testCombinedTechniqueEffect() {
    std::cout << "测试: 多技术组合效果..." << std::endl;
    
    CommunicationAntiJamModel antiJam;
    
    std::vector<AntiJamTechnique> combination = {
        AntiJamTechnique::FREQUENCY_HOPPING,
        AntiJamTechnique::ERROR_CORRECTION,
        AntiJamTechnique::ADAPTIVE_FILTERING
    };
    
    double combinedEffect = antiJam.calculateCombinedTechniqueEffect(combination);
    
    // 组合效果应该大于单一技术
    EXPECT_GT(combinedEffect, 0.0);
    
    // 测试推荐技术组合
    auto recommended = antiJam.getRecommendedTechniqueCombination();
    EXPECT_TRUE(!recommended.empty());
    
    std::cout << "✓ 多技术组合效果测试通过" << std::endl;
    return true;
}

// 测试性能预测
bool testPerformancePrediction() {
    std::cout << "测试: 性能预测..." << std::endl;
    
    CommunicationAntiJamModel antiJam;
    antiJam.setSignalPower(-80.0);
    antiJam.setProcessingGain(20.0);
    
    double performance1 = antiJam.predictPerformanceUnderJamming(-60.0, 50.0);
    double performance2 = antiJam.predictPerformanceUnderJamming(-70.0, 50.0);
    
    // 性能应该在0-1范围内
    EXPECT_GE(performance1, 0.0);
    EXPECT_LE(performance1, 1.0);
    EXPECT_GE(performance2, 0.0);
    EXPECT_LE(performance2, 1.0);
    
    // 更弱的干扰应该有更好的性能
    EXPECT_GE(performance2, performance1);
    
    // 测试所需抗干扰增益计算
    double requiredGain = antiJam.calculateRequiredAntiJamGain(1e-6);
    EXPECT_GE(requiredGain, 0.0);
    
    // 测试最大可容忍干扰功率
    double maxJammerPower = antiJam.calculateMaxTolerableJammerPower();
    EXPECT_GT(maxJammerPower, -200.0);
    EXPECT_LT(maxJammerPower, 100.0);
    
    std::cout << "✓ 性能预测测试通过" << std::endl;
    return true;
}

// 测试信息输出
bool testInformationOutput() {
    std::cout << "测试: 信息输出..." << std::endl;
    
    CommunicationAntiJamModel antiJam(AntiJamTechnique::HYBRID_SPREAD,
                                     AntiJamStrategy::COGNITIVE,
                                     25.0, 2000.0, 5000.0, 5.0);
    antiJam.setSignalPower(-75.0);
    antiJam.setInterferenceLevel(-65.0);
    
    std::string paramInfo = antiJam.getParameterInfo();
    std::string effectInfo = antiJam.getAntiJamEffectInfo();
    std::string recommendInfo = antiJam.getRecommendationInfo();
    std::string comparisonInfo = antiJam.getTechniqueComparisonInfo();
    
    // 检查信息字符串不为空
    EXPECT_TRUE(!paramInfo.empty());
    EXPECT_TRUE(!effectInfo.empty());
    EXPECT_TRUE(!recommendInfo.empty());
    EXPECT_TRUE(!comparisonInfo.empty());
    
    // 检查包含关键信息
    EXPECT_TRUE(paramInfo.find("抗干扰技术") != std::string::npos);
    EXPECT_TRUE(paramInfo.find("混合扩频") != std::string::npos);
    EXPECT_TRUE(paramInfo.find("认知抗干扰") != std::string::npos);
    
    EXPECT_TRUE(effectInfo.find("抗干扰增益") != std::string::npos);
    EXPECT_TRUE(effectInfo.find("信干比") != std::string::npos);
    EXPECT_TRUE(effectInfo.find("保护有效性") != std::string::npos);
    
    EXPECT_TRUE(recommendInfo.find("推荐抗干扰技术") != std::string::npos);
    EXPECT_TRUE(recommendInfo.find("建议处理增益") != std::string::npos);
    
    std::cout << "✓ 信息输出测试通过" << std::endl;
    return true;
}

// 测试自检功能
bool testSelfTest() {
    std::cout << "测试: 自检功能..." << std::endl;
    
    CommunicationAntiJamModel antiJam;
    
    // 正常参数应该通过自检
    EXPECT_TRUE(antiJam.runSelfTest());
    
    // 设置异常参数
    antiJam.setProcessingGain(100.0); // 超出范围的参数
    // 注意：由于参数校验，这个设置会失败，所以自检仍然应该通过
    EXPECT_TRUE(antiJam.runSelfTest());
    
    std::cout << "✓ 自检功能测试通过" << std::endl;
    return true;
}

// 性能测试
bool testPerformance() {
    std::cout << "测试: 性能测试..." << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // 执行大量计算
    for (int i = 0; i < 3000; ++i) {
        CommunicationAntiJamModel antiJam(AntiJamTechnique::FREQUENCY_HOPPING,
                                         AntiJamStrategy::ADAPTIVE,
                                         20.0 + i * 0.001,
                                         1000.0 + i * 0.1,
                                         1000.0 + i * 0.1,
                                         3.0 + i * 0.0001);
        antiJam.setSignalPower(-80.0 + i * 0.001);
        antiJam.setInterferenceLevel(-70.0 + i * 0.001);
        
        double gain = antiJam.calculateAntiJamGain();
        double resistance = antiJam.calculateJammerResistance();
        double sjr = antiJam.calculateSignalToJammerRatio();
        double ber = antiJam.calculateBitErrorRateWithJamming();
        double effectiveness = antiJam.calculateProtectionEffectiveness();
        AntiJamEffectLevel level = antiJam.evaluateAntiJamEffect();
        
        // 避免编译器优化掉计算
        (void)gain; (void)resistance; (void)sjr; (void)ber; (void)effectiveness; (void)level;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "3000次计算耗时: " << duration.count() << "ms" << std::endl;
    EXPECT_LT(duration.count(), 1500); // 应该在1.5秒内完成
    
    std::cout << "✓ 性能测试通过" << std::endl;
    return true;
}

// 运行所有测试
int main() {
    std::cout << "=== 通信抗干扰模型单元测试 ===" << std::endl;
    
    int passed = 0;
    int total = 0;
    
    // 测试列表
    struct Test {
        const char* name;
        bool (*func)();
    };
    
    Test tests[] = {
        {"构造函数和基本参数", testConstructorAndBasicParameters},
        {"参数校验", testParameterValidation},
        {"跳频参数", testFrequencyHoppingParameters},
        {"扩频参数", testSpreadSpectrumParameters},
        {"自适应参数", testAdaptiveParameters},
        {"抗干扰增益计算", testAntiJamGainCalculation},
        {"抗干扰能力计算", testJammerResistanceCalculation},
        {"信干比计算", testSignalToJammerRatioCalculation},
        {"误码率计算", testBitErrorRateCalculation},
        {"不同抗干扰技术", testDifferentAntiJamTechniques},
        {"抗干扰策略效果", testAntiJamStrategyEffect},
        {"最优策略计算", testOptimalStrategyCalculation},
        {"多技术组合效果", testCombinedTechniqueEffect},
        {"性能预测", testPerformancePrediction},
        {"信息输出", testInformationOutput},
        {"自检功能", testSelfTest},
        {"性能测试", testPerformance}
    };
    
    for (const auto& test : tests) {
        total++;
        std::cout << "\n--- " << test.name << " ---" << std::endl;
        
        try {
            if (test.func()) {
                passed++;
                std::cout << "✓ " << test.name << " 通过" << std::endl;
            } else {
                std::cout << "✗ " << test.name << " 失败" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "✗ " << test.name << " 异常: " << e.what() << std::endl;
        }
    }
    
    std::cout << "\n=== 测试结果 ===" << std::endl;
    std::cout << "通过: " << passed << "/" << total << std::endl;
    std::cout << "成功率: " << std::fixed << std::setprecision(1) 
              << (100.0 * passed / total) << "%" << std::endl;
    
    return (passed == total) ? 0 : 1;
}