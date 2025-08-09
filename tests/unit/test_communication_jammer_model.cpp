#include "CommunicationJammerModel.h"
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
    CommunicationJammerModel jammer;
    EXPECT_TRUE(jammer.getJammerType() == JammerType::GAUSSIAN_NOISE);
    EXPECT_TRUE(jammer.getJammerStrategy() == JammerStrategy::CONTINUOUS);
    EXPECT_NEAR(jammer.getJammerPower(), 30.0, 0.01);
    EXPECT_NEAR(jammer.getJammerFrequency(), 10000.0, 0.01);
    EXPECT_NEAR(jammer.getJammerBandwidth(), 100.0, 0.01);
    EXPECT_NEAR(jammer.getJammerRange(), 50.0, 0.01);
    
    // 测试带参数的构造函数
    CommunicationJammerModel jammer2(JammerType::NARROWBAND, 
                                    JammerStrategy::ADAPTIVE,
                                    40.0, 15000.0, 50.0, 30.0);
    EXPECT_TRUE(jammer2.getJammerType() == JammerType::NARROWBAND);
    EXPECT_TRUE(jammer2.getJammerStrategy() == JammerStrategy::ADAPTIVE);
    EXPECT_NEAR(jammer2.getJammerPower(), 40.0, 0.01);
    EXPECT_NEAR(jammer2.getJammerFrequency(), 15000.0, 0.01);
    EXPECT_NEAR(jammer2.getJammerBandwidth(), 50.0, 0.01);
    EXPECT_NEAR(jammer2.getJammerRange(), 30.0, 0.01);
    
    std::cout << "✓ 构造函数和基本参数设置测试通过" << std::endl;
    return true;
}

// 测试参数设置和校验
bool testParameterValidation() {
    std::cout << "测试: 参数校验..." << std::endl;
    
    CommunicationJammerModel jammer;
    
    // 测试干扰功率范围
    EXPECT_TRUE(jammer.setJammerPower(0.0));
    EXPECT_TRUE(jammer.setJammerPower(-50.0));
    EXPECT_TRUE(jammer.setJammerPower(50.0));
    EXPECT_FALSE(jammer.setJammerPower(-60.0)); // 超出下限
    EXPECT_FALSE(jammer.setJammerPower(60.0));  // 超出上限
    
    // 测试频率范围
    EXPECT_TRUE(jammer.setJammerFrequency(1000.0));
    EXPECT_TRUE(jammer.setJammerFrequency(1000000.0));
    EXPECT_TRUE(jammer.setJammerFrequency(30000000.0));
    EXPECT_FALSE(jammer.setJammerFrequency(0.5)); // 超出下限
    EXPECT_FALSE(jammer.setJammerFrequency(50000000.0)); // 超出上限
    
    // 测试带宽范围
    EXPECT_TRUE(jammer.setJammerBandwidth(0.1));
    EXPECT_TRUE(jammer.setJammerBandwidth(1000.0));
    EXPECT_TRUE(jammer.setJammerBandwidth(10000.0));
    EXPECT_FALSE(jammer.setJammerBandwidth(0.05)); // 超出下限
    EXPECT_FALSE(jammer.setJammerBandwidth(15000.0)); // 超出上限
    
    // 测试作用距离范围
    EXPECT_TRUE(jammer.setJammerRange(0.1));
    EXPECT_TRUE(jammer.setJammerRange(100.0));
    EXPECT_TRUE(jammer.setJammerRange(1000.0));
    EXPECT_FALSE(jammer.setJammerRange(0.05)); // 超出下限
    EXPECT_FALSE(jammer.setJammerRange(1500.0)); // 超出上限
    
    // 测试目标参数设置
    EXPECT_TRUE(jammer.setTargetFrequency(10000.0));
    EXPECT_TRUE(jammer.setTargetBandwidth(25.0));
    EXPECT_TRUE(jammer.setTargetPower(-80.0));
    EXPECT_TRUE(jammer.setTargetDistance(20.0));
    
    std::cout << "✓ 参数校验测试通过" << std::endl;
    return true;
}

// 测试脉冲干扰参数
bool testPulseJammerParameters() {
    std::cout << "测试: 脉冲干扰参数..." << std::endl;
    
    CommunicationJammerModel jammer;
    jammer.setJammerType(JammerType::PULSE);
    
    // 测试脉冲宽度
    EXPECT_TRUE(jammer.setPulseWidth(1.0));
    EXPECT_TRUE(jammer.setPulseWidth(100.0));
    EXPECT_TRUE(jammer.setPulseWidth(1000.0));
    EXPECT_FALSE(jammer.setPulseWidth(0.0)); // 无效值
    EXPECT_FALSE(jammer.setPulseWidth(1500.0)); // 超出上限
    
    // 测试脉冲重复频率
    EXPECT_TRUE(jammer.setPulseRepetitionRate(100.0));
    EXPECT_TRUE(jammer.setPulseRepetitionRate(1000.0));
    EXPECT_TRUE(jammer.setPulseRepetitionRate(10000.0));
    EXPECT_FALSE(jammer.setPulseRepetitionRate(0.0)); // 无效值
    EXPECT_FALSE(jammer.setPulseRepetitionRate(150000.0)); // 超出上限
    
    // 测试占空比
    EXPECT_TRUE(jammer.setDutyCycle(0.0));
    EXPECT_TRUE(jammer.setDutyCycle(0.5));
    EXPECT_TRUE(jammer.setDutyCycle(1.0));
    EXPECT_FALSE(jammer.setDutyCycle(-0.1)); // 超出下限
    EXPECT_FALSE(jammer.setDutyCycle(1.1));  // 超出上限
    
    std::cout << "✓ 脉冲干扰参数测试通过" << std::endl;
    return true;
}

// 测试扫频干扰参数
bool testSweepJammerParameters() {
    std::cout << "测试: 扫频干扰参数..." << std::endl;
    
    CommunicationJammerModel jammer;
    jammer.setJammerType(JammerType::SWEEP_FREQUENCY);
    
    // 测试扫频速率
    EXPECT_TRUE(jammer.setSweepRate(0.1));
    EXPECT_TRUE(jammer.setSweepRate(10.0));
    EXPECT_TRUE(jammer.setSweepRate(1000.0));
    EXPECT_FALSE(jammer.setSweepRate(0.0)); // 无效值
    EXPECT_FALSE(jammer.setSweepRate(1500.0)); // 超出上限
    
    // 测试扫频范围
    EXPECT_TRUE(jammer.setSweepRange(1.0));
    EXPECT_TRUE(jammer.setSweepRange(100.0));
    EXPECT_TRUE(jammer.setSweepRange(10000.0));
    EXPECT_FALSE(jammer.setSweepRange(0.0)); // 无效值
    EXPECT_FALSE(jammer.setSweepRange(15000.0)); // 超出上限
    
    std::cout << "✓ 扫频干扰参数测试通过" << std::endl;
    return true;
}

// 测试干信比计算
bool testJammerToSignalRatioCalculation() {
    std::cout << "测试: 干信比计算..." << std::endl;
    
    CommunicationJammerModel jammer;
    jammer.setJammerPower(30.0);
    jammer.setTargetPower(-80.0);
    jammer.setTargetDistance(10.0);
    jammer.setJammerFrequency(10000.0);
    
    double js_ratio = jammer.calculateJammerToSignalRatio();
    
    // 干信比应该在合理范围内
    EXPECT_GT(js_ratio, -50.0);
    EXPECT_LT(js_ratio, 100.0);
    
    // 测试距离对干信比的影响
    jammer.setTargetDistance(5.0);  // 更近距离
    double js_ratio_near = jammer.calculateJammerToSignalRatio();
    
    jammer.setTargetDistance(20.0); // 更远距离
    double js_ratio_far = jammer.calculateJammerToSignalRatio();
    
    EXPECT_GT(js_ratio_near, js_ratio_far); // 近距离干信比应该更高
    
    std::cout << "✓ 干信比计算测试通过" << std::endl;
    return true;
}

// 测试不同干扰类型的效果计算
bool testDifferentJammerTypeEffects() {
    std::cout << "测试: 不同干扰类型效果..." << std::endl;
    
    std::vector<JammerType> jammerTypes = {
        JammerType::GAUSSIAN_NOISE,
        JammerType::NARROWBAND,
        JammerType::SWEEP_FREQUENCY,
        JammerType::PULSE,
        JammerType::BARRAGE,
        JammerType::SPOT
    };
    
    for (auto jammerType : jammerTypes) {
        CommunicationJammerModel jammer;
        jammer.setJammerType(jammerType);
        jammer.setJammerPower(30.0);
        jammer.setJammerFrequency(10000.0);
        jammer.setTargetFrequency(10000.0);
        jammer.setTargetPower(-80.0);
        jammer.setTargetDistance(10.0);
        
        double effectiveness = jammer.calculateJammerEffectiveness();
        double degradation = jammer.calculateCommunicationDegradation();
        
        // 干扰有效性和性能下降应该在0-1范围内
        EXPECT_GE(effectiveness, 0.0);
        EXPECT_LE(effectiveness, 1.0);
        EXPECT_GE(degradation, 0.0);
        EXPECT_LE(degradation, 1.0);
        
        // 测试干扰效果等级评估
        JammerEffectLevel level = jammer.evaluateJammerEffect();
        EXPECT_TRUE(level >= JammerEffectLevel::NO_EFFECT && 
                   level <= JammerEffectLevel::COMPLETE_DENIAL);
    }
    
    std::cout << "✓ 不同干扰类型效果测试通过" << std::endl;
    return true;
}

// 测试频率重叠对干扰效果的影响
bool testFrequencyOverlapEffect() {
    std::cout << "测试: 频率重叠效果..." << std::endl;
    
    CommunicationJammerModel jammer;
    jammer.setJammerType(JammerType::NARROWBAND);
    jammer.setJammerPower(30.0);
    jammer.setJammerBandwidth(50.0);
    jammer.setTargetBandwidth(25.0);
    jammer.setTargetPower(-80.0);
    jammer.setTargetDistance(10.0);
    
    // 测试完全重叠
    jammer.setJammerFrequency(10000.0);
    jammer.setTargetFrequency(10000.0);
    double effect_overlap = jammer.calculateJammerEffectiveness();
    
    // 测试部分重叠
    jammer.setJammerFrequency(10020.0);
    double effect_partial = jammer.calculateJammerEffectiveness();
    
    // 测试无重叠
    jammer.setJammerFrequency(10100.0);
    double effect_no_overlap = jammer.calculateJammerEffectiveness();
    
    // 重叠度越高，干扰效果应该越好
    EXPECT_GT(effect_overlap, effect_partial);
    EXPECT_GT(effect_partial, effect_no_overlap);
    
    std::cout << "✓ 频率重叠效果测试通过" << std::endl;
    return true;
}

// 测试干扰覆盖范围计算
bool testJammerCoverageCalculation() {
    std::cout << "测试: 干扰覆盖范围计算..." << std::endl;
    
    CommunicationJammerModel jammer;
    jammer.setJammerPower(30.0);
    jammer.setTargetPower(-80.0);
    
    double coverage = jammer.calculateJammerCoverage();
    
    // 覆盖范围应该为正值
    EXPECT_GT(coverage, 0.0);
    
    // 测试功率对覆盖范围的影响
    jammer.setJammerPower(40.0); // 增加功率
    double coverage_high_power = jammer.calculateJammerCoverage();
    
    EXPECT_GT(coverage_high_power, coverage); // 更高功率应该有更大覆盖范围
    
    // 测试目标是否在干扰范围内
    jammer.setJammerRange(50.0);
    jammer.setTargetDistance(30.0);
    EXPECT_TRUE(jammer.isTargetInJammerRange());
    
    jammer.setTargetDistance(60.0);
    EXPECT_FALSE(jammer.isTargetInJammerRange());
    
    std::cout << "✓ 干扰覆盖范围计算测试通过" << std::endl;
    return true;
}

// 测试所需干扰功率计算
bool testRequiredJammerPowerCalculation() {
    std::cout << "测试: 所需干扰功率计算..." << std::endl;
    
    CommunicationJammerModel jammer;
    jammer.setTargetPower(-80.0);
    jammer.setTargetDistance(10.0);
    jammer.setJammerFrequency(10000.0);
    
    double required_power_10dB = jammer.calculateRequiredJammerPower(10.0);
    double required_power_20dB = jammer.calculateRequiredJammerPower(20.0);
    
    // 更高的干信比要求应该需要更高的功率
    EXPECT_GT(required_power_20dB, required_power_10dB);
    
    // 功率值应该在合理范围内
    EXPECT_GT(required_power_10dB, -50.0);
    EXPECT_LT(required_power_10dB, 100.0);
    
    std::cout << "✓ 所需干扰功率计算测试通过" << std::endl;
    return true;
}

// 测试最优干扰频率计算
bool testOptimalJammerFrequencyCalculation() {
    std::cout << "测试: 最优干扰频率计算..." << std::endl;
    
    CommunicationJammerModel jammer;
    jammer.setTargetFrequency(15000.0);
    
    double optimal_freq = jammer.calculateOptimalJammerFrequency();
    
    // 最优频率应该等于目标频率
    EXPECT_NEAR(optimal_freq, 15000.0, 0.01);
    
    std::cout << "✓ 最优干扰频率计算测试通过" << std::endl;
    return true;
}

// 测试信息输出
bool testInformationOutput() {
    std::cout << "测试: 信息输出..." << std::endl;
    
    CommunicationJammerModel jammer(JammerType::PULSE, 
                                   JammerStrategy::ADAPTIVE,
                                   35.0, 12000.0, 75.0, 40.0);
    jammer.setTargetFrequency(12000.0);
    jammer.setTargetPower(-85.0);
    jammer.setTargetDistance(15.0);
    
    std::string paramInfo = jammer.getParameterInfo();
    std::string effectInfo = jammer.getJammerEffectInfo();
    std::string recommendInfo = jammer.getRecommendationInfo();
    
    // 检查信息字符串不为空
    EXPECT_TRUE(!paramInfo.empty());
    EXPECT_TRUE(!effectInfo.empty());
    EXPECT_TRUE(!recommendInfo.empty());
    
    // 检查包含关键信息
    EXPECT_TRUE(paramInfo.find("干扰类型") != std::string::npos);
    EXPECT_TRUE(paramInfo.find("脉冲干扰") != std::string::npos);
    EXPECT_TRUE(paramInfo.find("自适应干扰") != std::string::npos);
    
    EXPECT_TRUE(effectInfo.find("干信比") != std::string::npos);
    EXPECT_TRUE(effectInfo.find("干扰有效性") != std::string::npos);
    
    EXPECT_TRUE(recommendInfo.find("最优干扰频率") != std::string::npos);
    EXPECT_TRUE(recommendInfo.find("建议干扰功率") != std::string::npos);
    
    std::cout << "✓ 信息输出测试通过" << std::endl;
    return true;
}

// 性能测试
bool testPerformance() {
    std::cout << "测试: 性能测试..." << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // 执行大量计算
    for (int i = 0; i < 5000; ++i) {
        CommunicationJammerModel jammer(JammerType::GAUSSIAN_NOISE,
                                       JammerStrategy::CONTINUOUS,
                                       30.0 + i * 0.001, 
                                       10000.0 + i * 0.1, 
                                       100.0, 50.0);
        jammer.setTargetPower(-80.0 + i * 0.001);
        jammer.setTargetDistance(10.0 + i * 0.001);
        
        double js_ratio = jammer.calculateJammerToSignalRatio();
        double effectiveness = jammer.calculateJammerEffectiveness();
        double degradation = jammer.calculateCommunicationDegradation();
        JammerEffectLevel level = jammer.evaluateJammerEffect();
        
        // 避免编译器优化掉计算
        (void)js_ratio; (void)effectiveness; (void)degradation; (void)level;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "5000次计算耗时: " << duration.count() << "ms" << std::endl;
    EXPECT_LT(duration.count(), 1000); // 应该在1秒内完成
    
    std::cout << "✓ 性能测试通过" << std::endl;
    return true;
}

// 运行所有测试
int main() {
    std::cout << "=== 通信干扰模型单元测试 ===" << std::endl;
    
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
        {"脉冲干扰参数", testPulseJammerParameters},
        {"扫频干扰参数", testSweepJammerParameters},
        {"干信比计算", testJammerToSignalRatioCalculation},
        {"不同干扰类型效果", testDifferentJammerTypeEffects},
        {"频率重叠效果", testFrequencyOverlapEffect},
        {"干扰覆盖范围计算", testJammerCoverageCalculation},
        {"所需干扰功率计算", testRequiredJammerPowerCalculation},
        {"最优干扰频率计算", testOptimalJammerFrequencyCalculation},
        {"信息输出", testInformationOutput},
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