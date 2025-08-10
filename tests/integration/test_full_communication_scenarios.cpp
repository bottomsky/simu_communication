#include <gtest/gtest.h>
#include "CommunicationModelAPI.h"
#include "MathConstants.h"
#include <memory>
#include <vector>
#include <map>

/**
 * @brief 完整通信场景集成测试类
 */
class FullCommunicationScenariosTest : public ::testing::Test {
protected:
    void SetUp() override {
        api = std::make_unique<CommunicationModelAPI>();
    }

    void TearDown() override {
        api.reset();
    }

    std::unique_ptr<CommunicationModelAPI> api;
    
    // 辅助方法：设置标准环境
    void setupStandardEnvironment() {
        CommunicationEnvironment env;
        env.frequency = 2400.0;
        env.bandwidth = 20.0;
        env.transmitPower = 30.0;
        env.noisePower = -90.0;
        env.distance = 5.0;
        env.environmentType = EnvironmentType::OPEN_FIELD;
        env.temperature = 20.0;
        env.humidity = 50.0;
        env.atmosphericPressure = 1013.25;
        api->setEnvironment(env);
    }
    
    // 辅助方法：设置城市环境
    void setupUrbanEnvironment() {
        CommunicationEnvironment env;
        env.frequency = 1800.0;
        env.bandwidth = 10.0;
        env.transmitPower = 25.0;
        env.noisePower = -85.0;
        env.distance = 2.0;
        env.environmentType = EnvironmentType::URBAN;
        env.temperature = 25.0;
        env.humidity = 65.0;
        env.atmosphericPressure = 1010.0;
        api->setEnvironment(env);
    }
};

/**
 * @brief 测试正常通信场景的完整流程
 */
TEST_F(FullCommunicationScenariosTest, NormalCommunicationScenario) {
    setupStandardEnvironment();
    
    // 设置正常通信场景
    EXPECT_TRUE(api->setScenario(CommunicationScenario::NORMAL_COMMUNICATION));
    
    // 计算链路状态
    auto linkStatus = api->calculateLinkStatus();
    EXPECT_TRUE(linkStatus.isConnected);
    EXPECT_GT(linkStatus.signalStrength, -100.0);
    EXPECT_GT(linkStatus.signalToNoiseRatio, 10.0);
    EXPECT_LT(linkStatus.bitErrorRate, 1e-3);
    EXPECT_GT(linkStatus.throughput, 1.0);
    
    // 计算性能指标
    auto performance = api->calculatePerformance();
    EXPECT_GT(performance.effectiveRange, 1.0);
    EXPECT_GT(performance.maxDataRate, 1.0);
    EXPECT_GT(performance.powerEfficiency, 0.0);
    EXPECT_GT(performance.spectralEfficiency, 0.0);
    EXPECT_GT(performance.reliability, 0.8);
    EXPECT_GT(performance.availability, 0.9);
    
    // 验证通信距离计算
    double commRange = api->calculateCommunicationRange();
    EXPECT_GT(commRange, 5.0);
    
    // 验证状态描述不为空
    EXPECT_FALSE(linkStatus.statusDescription.empty());
}

/**
 * @brief 测试干扰通信场景的完整流程
 */
TEST_F(FullCommunicationScenariosTest, JammedCommunicationScenario) {
    setupStandardEnvironment();
    
    // 设置干扰环境
    JammingEnvironment jammingEnv;
    jammingEnv.isJammed = true;
    jammingEnv.jammerType = JammerType::GAUSSIAN_NOISE;
    jammingEnv.jammerPower = 20.0;
    jammingEnv.jammerFrequency = 2400.0;
    jammingEnv.jammerBandwidth = 30.0;
    jammingEnv.jammerDistance = 3.0;
    jammingEnv.jammerDensity = 0.5;
    api->setJammingEnvironment(jammingEnv);
    
    // 设置干扰通信场景
    EXPECT_TRUE(api->setScenario(CommunicationScenario::JAMMED_COMMUNICATION));
    
    // 计算链路状态
    auto linkStatus = api->calculateLinkStatus();
    EXPECT_GT(linkStatus.bitErrorRate, 1e-6); // 干扰应该增加误码率
    
    // 计算干扰相关指标
    double jammerEffectiveness = api->calculateJammerEffectiveness();
    EXPECT_GE(jammerEffectiveness, 0.0);
    EXPECT_LE(jammerEffectiveness, 1.0);
    
    double jsr = api->calculateJammerToSignalRatio();
    EXPECT_GT(jsr, 0.0);
    
    auto jammerCoverage = api->calculateJammerCoverage();
    EXPECT_FALSE(jammerCoverage.empty());
    
    // 验证性能下降
    auto performance = api->calculatePerformance();
    EXPECT_LT(performance.reliability, 0.9); // 可靠性应该下降
}

/**
 * @brief 测试抗干扰通信场景的完整流程
 */
TEST_F(FullCommunicationScenariosTest, AntiJamCommunicationScenario) {
    setupStandardEnvironment();
    
    // 设置强干扰环境
    JammingEnvironment jammingEnv;
    jammingEnv.isJammed = true;
    jammingEnv.jammerType = JammerType::TONE_JAMMER;
    jammingEnv.jammerPower = 30.0;
    jammingEnv.jammerFrequency = 2400.0;
    jammingEnv.jammerBandwidth = 40.0;
    jammingEnv.jammerDistance = 2.0;
    jammingEnv.jammerDensity = 0.8;
    api->setJammingEnvironment(jammingEnv);
    
    // 设置抗干扰通信场景
    EXPECT_TRUE(api->setScenario(CommunicationScenario::ANTI_JAM_COMMUNICATION));
    
    // 计算链路状态
    auto linkStatus = api->calculateLinkStatus();
    
    // 计算抗干扰相关指标
    double antiJamEffectiveness = api->calculateAntiJamEffectiveness();
    EXPECT_GE(antiJamEffectiveness, 0.0);
    EXPECT_LE(antiJamEffectiveness, 1.0);
    
    double requiredGain = api->calculateRequiredAntiJamGain(1e-6);
    EXPECT_GE(requiredGain, 0.0);
    
    // 验证抗干扰性能
    auto performance = api->calculatePerformance();
    EXPECT_GT(performance.jammerResistance, 0.0);
    EXPECT_GT(performance.interceptionResistance, 0.0);
}

/**
 * @brief 测试多用户通信场景
 */
TEST_F(FullCommunicationScenariosTest, MultiUserCommunicationScenario) {
    setupUrbanEnvironment();
    
    // 设置多用户通信场景
    EXPECT_TRUE(api->setScenario(CommunicationScenario::MULTI_USER_COMMUNICATION));
    
    // 计算基本性能
    auto linkStatus = api->calculateLinkStatus();
    auto performance = api->calculatePerformance();
    
    // 验证多用户场景的特殊性能指标
    EXPECT_GT(performance.spectralEfficiency, 0.0);
    EXPECT_GT(performance.powerEfficiency, 0.0);
    
    // 模拟多个用户位置的网络分析
    std::vector<std::pair<double, double>> nodePositions = {
        {0.0, 0.0},   // 基站
        {1.0, 0.0},   // 用户1
        {0.0, 1.0},   // 用户2
        {1.0, 1.0},   // 用户3
        {0.5, 0.5}    // 用户4
    };
    
    auto linkMatrix = api->calculateLinkMatrix(nodePositions);
    EXPECT_EQ(linkMatrix.size(), nodePositions.size());
    
    double connectivity = api->calculateNetworkConnectivity(nodePositions);
    EXPECT_GE(connectivity, 0.0);
    EXPECT_LE(connectivity, 1.0);
}

/**
 * @brief 测试中继通信场景
 */
TEST_F(FullCommunicationScenariosTest, RelayCommunicationScenario) {
    setupStandardEnvironment();
    
    // 设置较大的通信距离，需要中继
    api->setDistance(20.0);
    
    // 设置中继通信场景
    EXPECT_TRUE(api->setScenario(CommunicationScenario::RELAY_COMMUNICATION));
    
    // 计算性能
    auto performance = api->calculatePerformance();
    
    // 寻找最优中继位置
    std::pair<double, double> source = {0.0, 0.0};
    std::pair<double, double> destination = {20.0, 0.0};
    int maxRelays = 2;
    
    auto relayPositions = api->findOptimalRelayPositions(source, destination, maxRelays);
    EXPECT_LE(relayPositions.size(), static_cast<size_t>(maxRelays));
    
    // 验证中继通信的有效性
    EXPECT_GT(performance.effectiveRange, 15.0);
}

/**
 * @brief 测试网状通信场景
 */
TEST_F(FullCommunicationScenariosTest, MeshCommunicationScenario) {
    setupUrbanEnvironment();
    
    // 设置网状通信场景
    EXPECT_TRUE(api->setScenario(CommunicationScenario::MESH_COMMUNICATION));
    
    // 创建网状网络节点
    std::vector<std::pair<double, double>> meshNodes = {
        {0.0, 0.0}, {2.0, 0.0}, {4.0, 0.0},
        {0.0, 2.0}, {2.0, 2.0}, {4.0, 2.0},
        {0.0, 4.0}, {2.0, 4.0}, {4.0, 4.0}
    };
    
    // 计算网络连接矩阵
    auto linkMatrix = api->calculateLinkMatrix(meshNodes);
    EXPECT_EQ(linkMatrix.size(), meshNodes.size());
    
    // 验证每个节点都有连接矩阵数据
    for (const auto& row : linkMatrix) {
        EXPECT_EQ(row.size(), meshNodes.size());
        for (double linkQuality : row) {
            EXPECT_GE(linkQuality, 0.0);
            EXPECT_LE(linkQuality, 1.0);
        }
    }
    
    // 计算网络连通性
    double connectivity = api->calculateNetworkConnectivity(meshNodes);
    EXPECT_GE(connectivity, 0.0);
    EXPECT_LE(connectivity, 1.0);
}

/**
 * @brief 测试多场景分析功能
 */
TEST_F(FullCommunicationScenariosTest, MultiScenarioAnalysis) {
    setupStandardEnvironment();
    
    // 定义多个场景
    std::vector<CommunicationScenario> scenarios = {
        CommunicationScenario::NORMAL_COMMUNICATION,
        CommunicationScenario::JAMMED_COMMUNICATION,
        CommunicationScenario::ANTI_JAM_COMMUNICATION,
        CommunicationScenario::MULTI_USER_COMMUNICATION
    };
    
    // 分析多个场景
    auto results = api->analyzeMultipleScenarios(scenarios);
    EXPECT_EQ(results.size(), scenarios.size());
    
    // 验证每个场景都有有效结果
    for (const auto& result : results) {
        EXPECT_GT(result.signalStrength, -200.0);
        EXPECT_LT(result.signalStrength, 100.0);
        EXPECT_GE(result.bitErrorRate, 0.0);
        EXPECT_LE(result.bitErrorRate, 1.0);
        EXPECT_GE(result.throughput, 0.0);
        EXPECT_FALSE(result.statusDescription.empty());
    }
}

/**
 * @brief 测试频率范围分析
 */
TEST_F(FullCommunicationScenariosTest, FrequencyRangeAnalysis) {
    setupStandardEnvironment();
    
    // 分析频率范围
    double startFreq = 2000.0;
    double endFreq = 3000.0;
    double step = 200.0;
    
    auto freqResults = api->analyzeFrequencyRange(startFreq, endFreq, step);
    EXPECT_FALSE(freqResults.empty());
    
    // 验证频率点数量正确
    int expectedPoints = static_cast<int>((endFreq - startFreq) / step) + 1;
    EXPECT_EQ(freqResults.size(), static_cast<size_t>(expectedPoints));
    
    // 验证每个频率点都有有效结果
    for (const auto& [freq, status] : freqResults) {
        EXPECT_GE(freq, startFreq);
        EXPECT_LE(freq, endFreq);
        EXPECT_GT(status.signalStrength, -200.0);
        EXPECT_GE(status.bitErrorRate, 0.0);
    }
}

/**
 * @brief 测试功率范围分析
 */
TEST_F(FullCommunicationScenariosTest, PowerRangeAnalysis) {
    setupStandardEnvironment();
    
    // 分析功率范围
    double startPower = 10.0;
    double endPower = 40.0;
    double step = 5.0;
    
    auto powerResults = api->analyzePowerRange(startPower, endPower, step);
    EXPECT_FALSE(powerResults.empty());
    
    // 验证功率增加时性能改善
    std::vector<double> throughputs;
    for (const auto& [power, status] : powerResults) {
        EXPECT_GE(power, startPower);
        EXPECT_LE(power, endPower);
        throughputs.push_back(status.throughput);
    }
    
    // 验证吞吐量随功率增加而增加（总体趋势）
    EXPECT_GT(throughputs.back(), throughputs.front());
}

/**
 * @brief 测试距离范围分析
 */
TEST_F(FullCommunicationScenariosTest, DistanceRangeAnalysis) {
    setupStandardEnvironment();
    
    // 分析距离范围
    double startDistance = 1.0;
    double endDistance = 10.0;
    double step = 1.0;
    
    auto distanceResults = api->analyzeDistanceRange(startDistance, endDistance, step);
    EXPECT_FALSE(distanceResults.empty());
    
    // 验证距离增加时信号强度下降
    std::vector<double> signalStrengths;
    for (const auto& [distance, status] : distanceResults) {
        EXPECT_GE(distance, startDistance);
        EXPECT_LE(distance, endDistance);
        signalStrengths.push_back(status.signalStrength);
    }
    
    // 验证信号强度随距离增加而减小
    EXPECT_LT(signalStrengths.back(), signalStrengths.front());
}

/**
 * @brief 测试环境变化对通信的影响
 */
TEST_F(FullCommunicationScenariosTest, EnvironmentalImpactAnalysis) {
    // 测试不同环境类型
    std::vector<EnvironmentType> envTypes = {
        EnvironmentType::OPEN_FIELD,
        EnvironmentType::SUBURBAN,
        EnvironmentType::URBAN,
        EnvironmentType::INDOOR
    };
    
    std::vector<double> ranges;
    
    for (auto envType : envTypes) {
        setupStandardEnvironment();
        api->setEnvironmentType(envType);
        
        double range = api->calculateCommunicationRange();
        ranges.push_back(range);
    }
    
    // 验证开阔地通信距离最远，室内最近
    EXPECT_GE(ranges[0], ranges[1]); // OPEN_FIELD >= SUBURBAN
    EXPECT_GE(ranges[1], ranges[2]); // SUBURBAN >= URBAN
    EXPECT_GE(ranges[2], ranges[3]); // URBAN >= INDOOR
}

/**
 * @brief 测试完整的优化流程
 */
TEST_F(FullCommunicationScenariosTest, OptimizationWorkflow) {
    setupStandardEnvironment();
    
    // 优化通信距离
    double targetRange = 15.0;
    auto rangeOptEnv = api->optimizeForRange(targetRange);
    EXPECT_GT(rangeOptEnv.transmitPower, 20.0);
    
    // 优化数据速率
    double targetDataRate = 50.0; // Mbps
    auto dataRateOptEnv = api->optimizeForDataRate(targetDataRate);
    EXPECT_GT(dataRateOptEnv.bandwidth, 10.0);
    
    // 优化功率效率
    auto powerEffOptEnv = api->optimizeForPowerEfficiency();
    EXPECT_LE(powerEffOptEnv.transmitPower, 35.0);
    
    // 优化抗干扰能力
    auto jammerResOptEnv = api->optimizeForJammerResistance();
    EXPECT_GT(jammerResOptEnv.bandwidth, 15.0);
}

/**
 * @brief 测试场景切换的一致性
 */
TEST_F(FullCommunicationScenariosTest, ScenarioSwitchingConsistency) {
    setupStandardEnvironment();
    
    // 记录初始状态
    auto initialStatus = api->calculateLinkStatus();
    
    // 切换到不同场景再切换回来
    api->setScenario(CommunicationScenario::JAMMED_COMMUNICATION);
    api->setScenario(CommunicationScenario::ANTI_JAM_COMMUNICATION);
    api->setScenario(CommunicationScenario::NORMAL_COMMUNICATION);
    
    // 验证状态一致性
    auto finalStatus = api->calculateLinkStatus();
    EXPECT_DOUBLE_EQ(initialStatus.signalStrength, finalStatus.signalStrength);
    EXPECT_DOUBLE_EQ(initialStatus.signalToNoiseRatio, finalStatus.signalToNoiseRatio);
}