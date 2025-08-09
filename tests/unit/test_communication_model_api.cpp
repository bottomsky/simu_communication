#include "../source/header/CommunicationModelAPI.h"
#include "../source/header/CommunicationDistanceModel.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>
#include <chrono>

// 简单的测试宏定义
#define EXPECT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            std::cerr << "FAIL: " << #condition << " at line " << __LINE__ << std::endl; \
            return false; \
        } \
    } while(0)

#define EXPECT_FALSE(condition) \
    do { \
        if (condition) { \
            std::cerr << "FAIL: " << #condition << " should be false at line " << __LINE__ << std::endl; \
            return false; \
        } \
    } while(0)

#define EXPECT_NEAR(actual, expected, tolerance) \
    do { \
        if (std::abs((actual) - (expected)) > (tolerance)) { \
            std::cerr << "FAIL: " << #actual << " = " << (actual) \
                      << ", expected " << (expected) << " ± " << (tolerance) \
                      << " at line " << __LINE__ << std::endl; \
            return false; \
        } \
    } while(0)

#define EXPECT_GT(actual, expected) \
    do { \
        if (!((actual) > (expected))) { \
            std::cerr << "FAIL: " << #actual << " = " << (actual) \
                      << " should be > " << (expected) \
                      << " at line " << __LINE__ << std::endl; \
            return false; \
        } \
    } while(0)

#define EXPECT_LT(actual, expected) \
    do { \
        if (!((actual) < (expected))) { \
            std::cerr << "FAIL: " << #actual << " = " << (actual) \
                      << " should be < " << (expected) \
                      << " at line " << __LINE__ << std::endl; \
            return false; \
        } \
    } while(0)

#define EXPECT_GE(actual, expected) \
    do { \
        if (!((actual) >= (expected))) { \
            std::cerr << "FAIL: " << #actual << " = " << (actual) \
                      << " should be >= " << (expected) \
                      << " at line " << __LINE__ << std::endl; \
            return false; \
        } \
    } while(0)

#define EXPECT_LE(actual, expected) \
    do { \
        if (!((actual) <= (expected))) { \
            std::cerr << "FAIL: " << #actual << " = " << (actual) \
                      << " should be <= " << (expected) \
                      << " at line " << __LINE__ << std::endl; \
            return false; \
        } \
    } while(0)

// 测试构造函数
bool testConstructor() {
    std::cout << "测试构造函数..." << std::endl;
    
    // 默认构造函数
    CommunicationModelAPI api1;
    EXPECT_TRUE(api1.getVersion() == "1.0.0");
    
    // 带场景的构造函数
    CommunicationModelAPI api2(CommunicationScenario::JAMMED_COMMUNICATION);
    
    std::cout << "构造函数测试通过" << std::endl;
    return true;
}

// 测试场景设置
bool testScenarioSetting() {
    std::cout << "测试场景设置..." << std::endl;
    
    CommunicationModelAPI api;
    
    // 测试各种场景设置
    EXPECT_TRUE(api.setScenario(CommunicationScenario::NORMAL_COMMUNICATION));
    EXPECT_TRUE(api.setScenario(CommunicationScenario::JAMMED_COMMUNICATION));
    EXPECT_TRUE(api.setScenario(CommunicationScenario::ANTI_JAM_COMMUNICATION));
    EXPECT_TRUE(api.setScenario(CommunicationScenario::MULTI_USER_COMMUNICATION));
    EXPECT_TRUE(api.setScenario(CommunicationScenario::RELAY_COMMUNICATION));
    EXPECT_TRUE(api.setScenario(CommunicationScenario::MESH_COMMUNICATION));
    
    std::cout << "场景设置测试通过" << std::endl;
    return true;
}

// 测试环境参数设置
bool testEnvironmentSetting() {
    std::cout << "测试环境参数设置..." << std::endl;
    
    CommunicationModelAPI api;
    
    // 测试频率设置
    EXPECT_TRUE(api.setFrequency(2400.0));
    EXPECT_FALSE(api.setFrequency(0.5)); // 无效频率
    EXPECT_FALSE(api.setFrequency(50000.0)); // 无效频率
    
    // 测试带宽设置
    EXPECT_TRUE(api.setBandwidth(100.0));
    EXPECT_FALSE(api.setBandwidth(0.05)); // 无效带宽
    EXPECT_FALSE(api.setBandwidth(20000.0)); // 无效带宽
    
    // 测试功率设置
    EXPECT_TRUE(api.setTransmitPower(30.0));
    EXPECT_FALSE(api.setTransmitPower(-60.0)); // 无效功率
    EXPECT_FALSE(api.setTransmitPower(60.0)); // 无效功率
    
    // 测试距离设置
    EXPECT_TRUE(api.setDistance(10.0));
    EXPECT_FALSE(api.setDistance(0.0)); // 无效距离
    EXPECT_FALSE(api.setDistance(2000.0)); // 无效距离
    
    // 测试环境类型设置
    EXPECT_TRUE(api.setEnvironmentType(EnvironmentType::URBAN_AREA));
    EXPECT_TRUE(api.setEnvironmentType(EnvironmentType::MOUNTAINOUS));
    
    std::cout << "环境参数设置测试通过" << std::endl;
    return true;
}

// 测试干扰环境设置
bool testJammingEnvironmentSetting() {
    std::cout << "测试干扰环境设置..." << std::endl;
    
    CommunicationModelAPI api;
    
    JammingEnvironment jammingEnv;
    jammingEnv.isJammed = true;
    jammingEnv.jammerType = JammerType::GAUSSIAN_NOISE;
    jammingEnv.jammerPower = -80.0;
    jammingEnv.jammerFrequency = 2400.0;
    jammingEnv.jammerBandwidth = 50.0;
    jammingEnv.jammerDistance = 15.0;
    jammingEnv.jammerDensity = 0.2;
    
    EXPECT_TRUE(api.setJammingEnvironment(jammingEnv));
    
    std::cout << "干扰环境设置测试通过" << std::endl;
    return true;
}

// 测试链路状态计算
bool testLinkStatusCalculation() {
    std::cout << "测试链路状态计算..." << std::endl;
    
    CommunicationModelAPI api;
    
    // 设置基本参数
    api.setFrequency(2400.0);
    api.setBandwidth(100.0);
    api.setTransmitPower(30.0);
    api.setDistance(1.0);
    api.setEnvironmentType(EnvironmentType::OPEN_FIELD);
    
    CommunicationLinkStatus status = api.calculateLinkStatus();
    
    // 验证计算结果的合理性
    EXPECT_GT(status.signalStrength, -150.0);
    EXPECT_LT(status.signalStrength, 50.0);
    EXPECT_GT(status.signalToNoiseRatio, -50.0);
    EXPECT_LT(status.signalToNoiseRatio, 100.0);
    EXPECT_GE(status.bitErrorRate, 0.0);
    EXPECT_LE(status.bitErrorRate, 1.0);
    EXPECT_GE(status.throughput, 0.0);
    EXPECT_GT(status.latency, 0.0);
    EXPECT_GE(status.packetLossRate, 0.0);
    EXPECT_LE(status.packetLossRate, 1.0);
    EXPECT_FALSE(status.statusDescription.empty());
    
    std::cout << "链路状态: " << status.statusDescription << std::endl;
    
    std::cout << "链路状态计算测试通过" << std::endl;
    return true;
}

// 测试性能计算
bool testPerformanceCalculation() {
    std::cout << "测试性能计算..." << std::endl;
    
    CommunicationModelAPI api;
    
    // 设置基本参数
    api.setFrequency(2400.0);
    api.setBandwidth(100.0);
    api.setTransmitPower(30.0);
    api.setDistance(1.0);
    api.setEnvironmentType(EnvironmentType::OPEN_FIELD);
    
    CommunicationPerformance performance = api.calculatePerformance();
    
    // 验证性能指标的合理性
    EXPECT_GE(performance.effectiveRange, 0.0);
    EXPECT_GE(performance.maxDataRate, 0.0);
    EXPECT_GE(performance.powerEfficiency, 0.0);
    EXPECT_GE(performance.spectralEfficiency, 0.0);
    EXPECT_GE(performance.reliability, 0.0);
    EXPECT_LE(performance.reliability, 1.0);
    EXPECT_GE(performance.availability, 0.0);
    EXPECT_LE(performance.availability, 1.0);
    EXPECT_GE(performance.jammerResistance, 0.0);
    EXPECT_GE(performance.interceptionResistance, 0.0);
    
    std::cout << "有效距离: " << performance.effectiveRange << " km" << std::endl;
    std::cout << "最大数据速率: " << performance.maxDataRate << " Mbps" << std::endl;
    
    std::cout << "性能计算测试通过" << std::endl;
    return true;
}

// 测试通信距离计算
bool testCommunicationRangeCalculation() {
    std::cout << "测试通信距离计算..." << std::endl;
    
    CommunicationModelAPI api;
    
    // 设置基本参数
    api.setFrequency(2400.0);
    api.setTransmitPower(30.0);
    api.setEnvironmentType(EnvironmentType::OPEN_FIELD);
    
    double range = api.calculateCommunicationRange();
    EXPECT_GT(range, 0.0);
    EXPECT_LT(range, 1000.0); // 合理的距离范围
    
    std::cout << "通信距离: " << range << " km" << std::endl;
    
    std::cout << "通信距离计算测试通过" << std::endl;
    return true;
}

// 测试所需功率计算
bool testRequiredPowerCalculation() {
    std::cout << "测试所需功率计算..." << std::endl;
    
    CommunicationModelAPI api;
    
    // 设置基本参数
    api.setFrequency(2400.0);
    api.setEnvironmentType(EnvironmentType::OPEN_FIELD);
    
    double requiredPower = api.calculateRequiredPower(10.0); // 10km距离
    EXPECT_GT(requiredPower, -50.0);
    EXPECT_LT(requiredPower, 100.0);
    
    std::cout << "10km距离所需功率: " << requiredPower << " dBm" << std::endl;
    
    std::cout << "所需功率计算测试通过" << std::endl;
    return true;
}

// 测试最优参数计算
bool testOptimalParameterCalculation() {
    std::cout << "测试最优参数计算..." << std::endl;
    
    CommunicationModelAPI api;
    
    // 设置基本环境
    api.setEnvironmentType(EnvironmentType::URBAN_AREA);
    
    double optimalFreq = api.calculateOptimalFrequency();
    EXPECT_GT(optimalFreq, 100.0);
    EXPECT_LT(optimalFreq, 30000.0);
    
    double optimalBW = api.calculateOptimalBandwidth();
    EXPECT_GT(optimalBW, 0.1);
    EXPECT_LT(optimalBW, 1000.0);
    
    std::cout << "最优频率: " << optimalFreq << " MHz" << std::endl;
    std::cout << "最优带宽: " << optimalBW << " MHz" << std::endl;
    
    std::cout << "最优参数计算测试通过" << std::endl;
    return true;
}

// 测试干扰分析
bool testJammerAnalysis() {
    std::cout << "测试干扰分析..." << std::endl;
    
    CommunicationModelAPI api;
    
    // 设置干扰环境
    JammingEnvironment jammingEnv;
    jammingEnv.isJammed = true;
    jammingEnv.jammerType = JammerType::GAUSSIAN_NOISE;
    jammingEnv.jammerPower = -70.0;
    jammingEnv.jammerFrequency = 2400.0;
    jammingEnv.jammerBandwidth = 50.0;
    jammingEnv.jammerDistance = 5.0;
    
    api.setJammingEnvironment(jammingEnv);
    api.setScenario(CommunicationScenario::JAMMED_COMMUNICATION);
    
    double jammerEffectiveness = api.calculateJammerEffectiveness();
    EXPECT_GE(jammerEffectiveness, 0.0);
    EXPECT_LE(jammerEffectiveness, 1.0);
    
    double jammerToSignalRatio = api.calculateJammerToSignalRatio();
    EXPECT_GT(jammerToSignalRatio, -200.0);
    EXPECT_LT(jammerToSignalRatio, 100.0);
    
    std::cout << "干扰有效性: " << jammerEffectiveness << std::endl;
    std::cout << "干信比: " << jammerToSignalRatio << " dB" << std::endl;
    
    std::cout << "干扰分析测试通过" << std::endl;
    return true;
}

// 测试抗干扰分析
bool testAntiJamAnalysis() {
    std::cout << "测试抗干扰分析..." << std::endl;
    
    CommunicationModelAPI api;
    
    // 设置抗干扰场景
    api.setScenario(CommunicationScenario::ANTI_JAM_COMMUNICATION);
    
    JammingEnvironment jammingEnv;
    jammingEnv.isJammed = true;
    jammingEnv.jammerType = JammerType::GAUSSIAN_NOISE;
    jammingEnv.jammerPower = -70.0;
    
    api.setJammingEnvironment(jammingEnv);
    
    double antiJamEffectiveness = api.calculateAntiJamEffectiveness();
    EXPECT_GE(antiJamEffectiveness, 0.0);
    EXPECT_LE(antiJamEffectiveness, 1.0);
    
    double requiredGain = api.calculateRequiredAntiJamGain(1e-6);
    EXPECT_GE(requiredGain, 0.0);
    EXPECT_LT(requiredGain, 100.0);
    
    std::cout << "抗干扰有效性: " << antiJamEffectiveness << std::endl;
    std::cout << "所需抗干扰增益: " << requiredGain << " dB" << std::endl;
    
    std::cout << "抗干扰分析测试通过" << std::endl;
    return true;
}

// 测试干扰覆盖计算
bool testJammerCoverageCalculation() {
    std::cout << "测试干扰覆盖计算..." << std::endl;
    
    CommunicationModelAPI api;
    
    // 设置干扰环境
    JammingEnvironment jammingEnv;
    jammingEnv.isJammed = true;
    jammingEnv.jammerType = JammerType::GAUSSIAN_NOISE;
    jammingEnv.jammerPower = -60.0;
    
    api.setJammingEnvironment(jammingEnv);
    
    std::vector<double> coverage = api.calculateJammerCoverage();
    EXPECT_GT(coverage.size(), 0);
    
    // 验证覆盖数据的合理性
    for (double value : coverage) {
        EXPECT_GE(value, 0.0);
        EXPECT_LE(value, 1.0);
    }
    
    std::cout << "干扰覆盖数据点数: " << coverage.size() << std::endl;
    
    std::cout << "干扰覆盖计算测试通过" << std::endl;
    return true;
}

// 测试性能优化
bool testPerformanceOptimization() {
    std::cout << "测试性能优化..." << std::endl;
    
    CommunicationModelAPI api;
    
    // 测试距离优化
    CommunicationEnvironment optimizedForRange = api.optimizeForRange(20.0);
    EXPECT_GT(optimizedForRange.transmitPower, -50.0);
    EXPECT_LT(optimizedForRange.transmitPower, 100.0);
    
    // 测试数据速率优化
    CommunicationEnvironment optimizedForDataRate = api.optimizeForDataRate(50.0);
    EXPECT_GT(optimizedForDataRate.bandwidth, 0.1);
    EXPECT_LT(optimizedForDataRate.bandwidth, 1000.0);
    
    // 测试功率效率优化
    CommunicationEnvironment optimizedForPower = api.optimizeForPowerEfficiency();
    EXPECT_GT(optimizedForPower.transmitPower, -50.0);
    EXPECT_LT(optimizedForPower.transmitPower, 100.0);
    
    // 测试抗干扰优化
    CommunicationEnvironment optimizedForAntiJam = api.optimizeForJammerResistance();
    EXPECT_GT(optimizedForAntiJam.transmitPower, -50.0);
    EXPECT_LT(optimizedForAntiJam.transmitPower, 100.0);
    
    std::cout << "距离优化功率: " << optimizedForRange.transmitPower << " dBm" << std::endl;
    std::cout << "数据速率优化带宽: " << optimizedForDataRate.bandwidth << " MHz" << std::endl;
    
    std::cout << "性能优化测试通过" << std::endl;
    return true;
}

// 测试便利函数
bool testUtilityFunctions() {
    std::cout << "测试便利函数..." << std::endl;
    
    // 测试快速距离计算
    double range = CommunicationDistanceModel::quickCalculateRange(2400.0, 30.0, EnvironmentType::OPEN_FIELD);
    EXPECT_GT(range, 0.0);
    EXPECT_LT(range, 1000.0);
    
    // 测试快速功率计算
    double power = CommunicationModelUtils::quickCalculatePower(2400.0, 10.0, EnvironmentType::URBAN_AREA);
    EXPECT_GT(power, -50.0);
    EXPECT_LT(power, 100.0);
    
    // 测试快速质量评估
    CommunicationQuality quality = CommunicationModelUtils::quickAssessQuality(15.0, 1e-5);
    EXPECT_TRUE(quality == CommunicationQuality::EXCELLENT || 
                quality == CommunicationQuality::GOOD);
    
    // 测试单位转换
    double watts = CommunicationModelUtils::dBmToWatts(30.0);
    EXPECT_NEAR(watts, 1.0, 0.01);
    
    double dBm = CommunicationModelUtils::wattsTodBm(1.0);
    EXPECT_NEAR(dBm, 30.0, 0.01);
    
    double linear = CommunicationModelUtils::dBToLinear(10.0);
    EXPECT_NEAR(linear, 10.0, 0.01);
    
    double dB = CommunicationModelUtils::linearTodB(10.0);
    EXPECT_NEAR(dB, 10.0, 0.01);
    
    // 测试频率波长转换
    double wavelength = CommunicationModelUtils::frequencyToWavelength(2400.0);
    EXPECT_NEAR(wavelength, 0.125, 0.001);
    
    double frequency = CommunicationModelUtils::wavelengthToFrequency(0.125);
    EXPECT_NEAR(frequency, 2400.0, 1.0);
    
    std::cout << "快速距离计算: " << range << " km" << std::endl;
    std::cout << "快速功率计算: " << power << " dBm" << std::endl;
    
    std::cout << "便利函数测试通过" << std::endl;
    return true;
}

// 测试版本信息
bool testVersionInfo() {
    std::cout << "测试版本信息..." << std::endl;
    
    CommunicationModelAPI api;
    
    std::string version = api.getVersion();
    EXPECT_FALSE(version.empty());
    
    std::string buildInfo = api.getBuildInfo();
    EXPECT_FALSE(buildInfo.empty());
    
    std::string modelInfo = api.getModelInfo();
    EXPECT_FALSE(modelInfo.empty());
    
    std::cout << "版本: " << version << std::endl;
    std::cout << "构建信息: " << buildInfo << std::endl;
    std::cout << "模型信息: " << std::endl << modelInfo << std::endl;
    
    std::cout << "版本信息测试通过" << std::endl;
    return true;
}

// 测试不同环境类型的性能
bool testDifferentEnvironments() {
    std::cout << "测试不同环境类型的性能..." << std::endl;
    
    CommunicationModelAPI api;
    api.setFrequency(2400.0);
    api.setBandwidth(100.0);
    api.setTransmitPower(30.0);
    api.setDistance(5.0);
    
    std::vector<EnvironmentType> environments = {
        EnvironmentType::OPEN_FIELD,
        EnvironmentType::URBAN_AREA,
        EnvironmentType::MOUNTAINOUS
    };
    
    std::vector<std::string> envNames = {
        "开阔地区", "城市地区", "山区"
    };
    
    for (size_t i = 0; i < environments.size(); ++i) {
        api.setEnvironmentType(environments[i]);
        CommunicationLinkStatus status = api.calculateLinkStatus();
        
        EXPECT_GT(status.signalStrength, -200.0);
        EXPECT_LT(status.signalStrength, 100.0);
        
        std::cout << envNames[i] << " - 信号强度: " << status.signalStrength 
                  << " dBm, 信噪比: " << status.signalToNoiseRatio << " dB" << std::endl;
    }
    
    std::cout << "不同环境类型测试通过" << std::endl;
    return true;
}

// 性能测试
bool testPerformance() {
    std::cout << "测试性能..." << std::endl;
    
    CommunicationModelAPI api;
    api.setFrequency(2400.0);
    api.setBandwidth(100.0);
    api.setTransmitPower(30.0);
    api.setDistance(1.0);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // 执行多次计算
    for (int i = 0; i < 1000; ++i) {
        api.calculateLinkStatus();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    double avgTime = duration.count() / 1000.0;
    std::cout << "平均计算时间: " << avgTime << " 微秒" << std::endl;
    
    // 性能应该在合理范围内
    EXPECT_LT(avgTime, 10000.0); // 小于10毫秒
    
    std::cout << "性能测试通过" << std::endl;
    return true;
}

// 主测试函数
int main() {
    std::cout << "开始通信模型API测试..." << std::endl;
    
    bool allPassed = true;
    
    allPassed &= testConstructor();
    allPassed &= testScenarioSetting();
    allPassed &= testEnvironmentSetting();
    allPassed &= testJammingEnvironmentSetting();
    allPassed &= testLinkStatusCalculation();
    allPassed &= testPerformanceCalculation();
    allPassed &= testCommunicationRangeCalculation();
    allPassed &= testRequiredPowerCalculation();
    allPassed &= testOptimalParameterCalculation();
    allPassed &= testJammerAnalysis();
    allPassed &= testAntiJamAnalysis();
    allPassed &= testJammerCoverageCalculation();
    allPassed &= testPerformanceOptimization();
    allPassed &= testUtilityFunctions();
    allPassed &= testVersionInfo();
    allPassed &= testDifferentEnvironments();
    allPassed &= testPerformance();
    
    if (allPassed) {
        std::cout << "\n所有测试通过！" << std::endl;
        return 0;
    } else {
        std::cout << "\n部分测试失败！" << std::endl;
        return 1;
    }
}