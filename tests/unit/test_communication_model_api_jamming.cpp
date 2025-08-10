#include <gtest/gtest.h>
#include "CommunicationModelAPI.h"
#include "MathConstants.h"
#include <memory>

/**
 * @brief CommunicationModelAPI干扰功能测试类
 */
class CommunicationModelAPIJammingTest : public ::testing::Test {
protected:
    void SetUp() override {
        api = std::make_unique<CommunicationModelAPI>();
        
        // 设置基础环境
        CommunicationEnvironment env;
        env.frequency = 2400.0;
        env.bandwidth = 20.0;
        env.transmitPower = 30.0;
        env.noisePower = -90.0;
        env.distance = 5.0;
        env.environmentType = EnvironmentType::OPEN_FIELD;
        api->setEnvironment(env);
    }

    void TearDown() override {
        api.reset();
    }

    std::unique_ptr<CommunicationModelAPI> api;
};

/**
 * @brief 测试干扰场景设置
 */
TEST_F(CommunicationModelAPIJammingTest, JammedScenario) {
    // 设置干扰场景
    EXPECT_TRUE(api->setScenario(CommunicationScenario::JAMMED_COMMUNICATION));
    EXPECT_EQ(api->getScenario(), CommunicationScenario::JAMMED_COMMUNICATION);
    
    // 验证干扰环境已激活
    auto jammingEnv = api->getJammingEnvironment();
    EXPECT_TRUE(jammingEnv.isJammed);
}

/**
 * @brief 测试抗干扰场景设置
 */
TEST_F(CommunicationModelAPIJammingTest, AntiJamScenario) {
    // 设置抗干扰场景
    EXPECT_TRUE(api->setScenario(CommunicationScenario::ANTI_JAM_COMMUNICATION));
    EXPECT_EQ(api->getScenario(), CommunicationScenario::ANTI_JAM_COMMUNICATION);
    
    // 验证干扰环境已激活
    auto jammingEnv = api->getJammingEnvironment();
    EXPECT_TRUE(jammingEnv.isJammed);
}

/**
 * @brief 测试干扰环境配置
 */
TEST_F(CommunicationModelAPIJammingTest, JammingEnvironmentConfiguration) {
    JammingEnvironment jammingEnv;
    jammingEnv.isJammed = true;
    jammingEnv.jammerType = JammerType::GAUSSIAN_NOISE;
    jammingEnv.jammerPower = 25.0;
    jammingEnv.jammerFrequency = 2400.0;
    jammingEnv.jammerBandwidth = 40.0;
    jammingEnv.jammerDistance = 3.0;
    jammingEnv.jammerDensity = 0.7;
    jammingEnv.jammerFrequencies = {2380.0, 2400.0, 2420.0};
    
    EXPECT_TRUE(api->setJammingEnvironment(jammingEnv));
    
    auto retrievedEnv = api->getJammingEnvironment();
    EXPECT_TRUE(retrievedEnv.isJammed);
    EXPECT_EQ(retrievedEnv.jammerType, JammerType::GAUSSIAN_NOISE);
    EXPECT_EQ(retrievedEnv.jammerPower, 25.0);
    EXPECT_EQ(retrievedEnv.jammerFrequency, 2400.0);
    EXPECT_EQ(retrievedEnv.jammerBandwidth, 40.0);
    EXPECT_EQ(retrievedEnv.jammerDistance, 3.0);
    EXPECT_EQ(retrievedEnv.jammerDensity, 0.7);
    EXPECT_EQ(retrievedEnv.jammerFrequencies.size(), 3);
}

/**
 * @brief 测试干扰效果计算
 */
TEST_F(CommunicationModelAPIJammingTest, JammerEffectivenessCalculation) {
    // 设置干扰环境
    JammingEnvironment jammingEnv;
    jammingEnv.isJammed = true;
    jammingEnv.jammerPower = 20.0;
    jammingEnv.jammerDistance = 2.0;
    api->setJammingEnvironment(jammingEnv);
    
    api->setScenario(CommunicationScenario::JAMMED_COMMUNICATION);
    
    double effectiveness = api->calculateJammerEffectiveness();
    EXPECT_GE(effectiveness, 0.0);
    EXPECT_LE(effectiveness, 1.0);
}

/**
 * @brief 测试抗干扰效果计算
 */
TEST_F(CommunicationModelAPIJammingTest, AntiJamEffectivenessCalculation) {
    // 设置干扰环境
    JammingEnvironment jammingEnv;
    jammingEnv.isJammed = true;
    jammingEnv.jammerPower = 20.0;
    api->setJammingEnvironment(jammingEnv);
    
    api->setScenario(CommunicationScenario::ANTI_JAM_COMMUNICATION);
    
    double antiJamEffectiveness = api->calculateAntiJamEffectiveness();
    EXPECT_GE(antiJamEffectiveness, 0.0);
    EXPECT_LE(antiJamEffectiveness, 1.0);
}

/**
 * @brief 测试干扰信号比计算
 */
TEST_F(CommunicationModelAPIJammingTest, JammerToSignalRatioCalculation) {
    // 设置干扰环境
    JammingEnvironment jammingEnv;
    jammingEnv.isJammed = true;
    jammingEnv.jammerPower = 25.0;
    jammingEnv.jammerDistance = 1.0;
    api->setJammingEnvironment(jammingEnv);
    
    api->setScenario(CommunicationScenario::JAMMED_COMMUNICATION);
    
    double jsr = api->calculateJammerToSignalRatio();
    EXPECT_GT(jsr, 0.0);
}

/**
 * @brief 测试所需抗干扰增益计算
 */
TEST_F(CommunicationModelAPIJammingTest, RequiredAntiJamGainCalculation) {
    // 设置干扰环境
    JammingEnvironment jammingEnv;
    jammingEnv.isJammed = true;
    jammingEnv.jammerPower = 30.0;
    api->setJammingEnvironment(jammingEnv);
    
    api->setScenario(CommunicationScenario::ANTI_JAM_COMMUNICATION);
    
    double targetBER = 1e-6;
    double requiredGain = api->calculateRequiredAntiJamGain(targetBER);
    EXPECT_GE(requiredGain, 0.0);
}

/**
 * @brief 测试干扰覆盖范围计算
 */
TEST_F(CommunicationModelAPIJammingTest, JammerCoverageCalculation) {
    // 设置干扰环境
    JammingEnvironment jammingEnv;
    jammingEnv.isJammed = true;
    jammingEnv.jammerPower = 35.0;
    jammingEnv.jammerFrequencies = {2380.0, 2400.0, 2420.0, 2440.0};
    api->setJammingEnvironment(jammingEnv);
    
    auto coverage = api->calculateJammerCoverage();
    EXPECT_FALSE(coverage.empty());
    
    // 验证覆盖范围数据的合理性
    for (double range : coverage) {
        EXPECT_GE(range, 0.0);
        EXPECT_LT(range, 100.0); // 合理的覆盖范围
    }
}

/**
 * @brief 测试干扰对性能的影响
 */
TEST_F(CommunicationModelAPIJammingTest, JammingImpactOnPerformance) {
    // 无干扰情况
    api->setScenario(CommunicationScenario::NORMAL_COMMUNICATION);
    auto normalStatus = api->calculateLinkStatus();
    
    // 有干扰情况
    JammingEnvironment jammingEnv;
    jammingEnv.isJammed = true;
    jammingEnv.jammerPower = 20.0;
    jammingEnv.jammerDistance = 2.0;
    api->setJammingEnvironment(jammingEnv);
    api->setScenario(CommunicationScenario::JAMMED_COMMUNICATION);
    auto jammedStatus = api->calculateLinkStatus();
    
    // 验证干扰降低了性能
    EXPECT_LT(jammedStatus.signalToNoiseRatio, normalStatus.signalToNoiseRatio);
    EXPECT_GT(jammedStatus.bitErrorRate, normalStatus.bitErrorRate);
    EXPECT_LT(jammedStatus.throughput, normalStatus.throughput);
}

/**
 * @brief 测试抗干扰对性能的改善
 */
TEST_F(CommunicationModelAPIJammingTest, AntiJamImprovementOnPerformance) {
    // 设置干扰环境
    JammingEnvironment jammingEnv;
    jammingEnv.isJammed = true;
    jammingEnv.jammerPower = 25.0;
    jammingEnv.jammerDistance = 1.5;
    api->setJammingEnvironment(jammingEnv);
    
    // 仅干扰情况
    api->setScenario(CommunicationScenario::JAMMED_COMMUNICATION);
    auto jammedStatus = api->calculateLinkStatus();
    
    // 抗干扰情况
    api->setScenario(CommunicationScenario::ANTI_JAM_COMMUNICATION);
    auto antiJamStatus = api->calculateLinkStatus();
    
    // 验证抗干扰改善了性能
    EXPECT_GE(antiJamStatus.signalToNoiseRatio, jammedStatus.signalToNoiseRatio);
    EXPECT_LE(antiJamStatus.bitErrorRate, jammedStatus.bitErrorRate);
    EXPECT_GE(antiJamStatus.throughput, jammedStatus.throughput);
}

/**
 * @brief 测试不同干扰类型的影响
 */
TEST_F(CommunicationModelAPIJammingTest, DifferentJammerTypesImpact) {
    std::vector<JammerType> jammerTypes = {
        JammerType::GAUSSIAN_NOISE,
        JammerType::NARROWBAND,
        JammerType::SWEEP_FREQUENCY,
        JammerType::PULSE
    };
    
    std::vector<double> berValues;
    
    for (auto jammerType : jammerTypes) {
        JammingEnvironment jammingEnv;
        jammingEnv.isJammed = true;
        jammingEnv.jammerType = jammerType;
        jammingEnv.jammerPower = 20.0;
        api->setJammingEnvironment(jammingEnv);
        api->setScenario(CommunicationScenario::JAMMED_COMMUNICATION);
        
        auto status = api->calculateLinkStatus();
        berValues.push_back(status.bitErrorRate);
    }
    
    // 验证所有干扰类型都产生了有效的BER值
    for (double ber : berValues) {
        EXPECT_GE(ber, 0.0);
        EXPECT_LE(ber, 1.0);
    }
}

/**
 * @brief 测试干扰功率对效果的影响
 */
TEST_F(CommunicationModelAPIJammingTest, JammerPowerImpact) {
    std::vector<double> jammerPowers = {10.0, 20.0, 30.0, 40.0};
    std::vector<double> berValues;
    
    for (double power : jammerPowers) {
        JammingEnvironment jammingEnv;
        jammingEnv.isJammed = true;
        jammingEnv.jammerPower = power;
        jammingEnv.jammerDistance = 2.0;
        api->setJammingEnvironment(jammingEnv);
        api->setScenario(CommunicationScenario::JAMMED_COMMUNICATION);
        
        auto status = api->calculateLinkStatus();
        berValues.push_back(status.bitErrorRate);
    }
    
    // 验证干扰功率增加导致BER增加
    for (size_t i = 1; i < berValues.size(); ++i) {
        EXPECT_GE(berValues[i], berValues[i-1]);
    }
}

/**
 * @brief 测试干扰距离对效果的影响
 */
TEST_F(CommunicationModelAPIJammingTest, JammerDistanceImpact) {
    std::vector<double> jammerDistances = {0.5, 1.0, 2.0, 5.0};
    std::vector<double> berValues;
    
    for (double distance : jammerDistances) {
        JammingEnvironment jammingEnv;
        jammingEnv.isJammed = true;
        jammingEnv.jammerPower = 25.0;
        jammingEnv.jammerDistance = distance;
        api->setJammingEnvironment(jammingEnv);
        api->setScenario(CommunicationScenario::JAMMED_COMMUNICATION);
        
        auto status = api->calculateLinkStatus();
        berValues.push_back(status.bitErrorRate);
    }
    
    // 验证干扰距离增加导致BER减少（干扰效果减弱）
    for (size_t i = 1; i < berValues.size(); ++i) {
        EXPECT_LE(berValues[i], berValues[i-1]);
    }
}