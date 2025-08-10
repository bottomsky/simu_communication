#include <gtest/gtest.h>
#include "CommunicationModelAPI.h"
#include "MathConstants.h"
#include <memory>
#include <cmath>

/**
 * @brief CommunicationModelAPI计算功能测试类
 */
class CommunicationModelAPICalculationsTest : public ::testing::Test {
protected:
    void SetUp() override {
        api = std::make_unique<CommunicationModelAPI>();
        
        // 设置标准测试环境
        CommunicationEnvironment env;
        env.frequency = 2400.0;      // 2.4 GHz
        env.bandwidth = 20.0;        // 20 MHz
        env.transmitPower = 30.0;    // 30 dBm
        env.noisePower = -90.0;      // -90 dBm
        env.distance = 1.0;          // 1 km
        env.environmentType = EnvironmentType::OPEN_FIELD;
        env.temperature = 20.0;
        env.humidity = 50.0;
        env.atmosphericPressure = 1013.25;
        
        api->setEnvironment(env);
    }

    void TearDown() override {
        api.reset();
    }

    std::unique_ptr<CommunicationModelAPI> api;
};

/**
 * @brief 测试链路状态计算
 */
TEST_F(CommunicationModelAPICalculationsTest, CalculateLinkStatus) {
    auto status = api->calculateLinkStatus();
    
    // 验证基本字段存在
    EXPECT_GT(status.signalStrength, -200.0);
    EXPECT_LT(status.signalStrength, 100.0);
    
    EXPECT_GT(status.signalToNoiseRatio, -50.0);
    EXPECT_LT(status.signalToNoiseRatio, 100.0);
    
    EXPECT_GE(status.bitErrorRate, 0.0);
    EXPECT_LE(status.bitErrorRate, 1.0);
    
    EXPECT_GE(status.throughput, 0.0);
    
    EXPECT_GE(status.latency, 0.0);
    
    EXPECT_GE(status.packetLossRate, 0.0);
    EXPECT_LE(status.packetLossRate, 1.0);
    
    EXPECT_NE(status.quality, CommunicationQuality::FAILED);
    
    EXPECT_FALSE(status.statusDescription.empty());
}

/**
 * @brief 测试性能计算
 */
TEST_F(CommunicationModelAPICalculationsTest, CalculatePerformance) {
    auto performance = api->calculatePerformance();
    
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
}

/**
 * @brief 测试通信距离计算
 */
TEST_F(CommunicationModelAPICalculationsTest, CalculateCommunicationRange) {
    double range = api->calculateCommunicationRange();
    EXPECT_GE(range, 0.0);
    EXPECT_LT(range, 1000.0); // 合理的最大通信距离
}

/**
 * @brief 测试所需功率计算
 */
TEST_F(CommunicationModelAPICalculationsTest, CalculateRequiredPower) {
    double targetRange = 5.0; // 5 km
    double requiredPower = api->calculateRequiredPower(targetRange);
    
    EXPECT_GT(requiredPower, 0.0);
    EXPECT_LT(requiredPower, 100.0); // 合理的功率范围
}

/**
 * @brief 测试最优频率计算
 */
TEST_F(CommunicationModelAPICalculationsTest, CalculateOptimalFrequency) {
    double optimalFreq = api->calculateOptimalFrequency();
    EXPECT_GT(optimalFreq, 0.0);
    EXPECT_LT(optimalFreq, 100000.0); // 合理的频率范围
}

/**
 * @brief 测试最优带宽计算
 */
TEST_F(CommunicationModelAPICalculationsTest, CalculateOptimalBandwidth) {
    double optimalBandwidth = api->calculateOptimalBandwidth();
    EXPECT_GT(optimalBandwidth, 0.0);
    EXPECT_LT(optimalBandwidth, 1000.0); // 合理的带宽范围
}

/**
 * @brief 测试距离对信号强度的影响
 */
TEST_F(CommunicationModelAPICalculationsTest, DistanceImpactOnSignalStrength) {
    // 测试不同距离下的信号强度
    std::vector<double> distances = {0.1, 0.5, 1.0, 2.0, 5.0, 10.0};
    std::vector<double> signalStrengths;
    
    for (double distance : distances) {
        api->setDistance(distance);
        auto status = api->calculateLinkStatus();
        signalStrengths.push_back(status.signalStrength);
    }
    
    // 验证信号强度随距离增加而减小
    for (size_t i = 1; i < signalStrengths.size(); ++i) {
        EXPECT_LT(signalStrengths[i], signalStrengths[i-1]);
    }
}

/**
 * @brief 测试功率对性能的影响
 */
TEST_F(CommunicationModelAPICalculationsTest, PowerImpactOnPerformance) {
    std::vector<double> powers = {10.0, 20.0, 30.0, 40.0};
    std::vector<double> throughputs;
    
    for (double power : powers) {
        api->setTransmitPower(power);
        auto status = api->calculateLinkStatus();
        throughputs.push_back(status.throughput);
    }
    
    // 验证吞吐量随功率增加而增加
    for (size_t i = 1; i < throughputs.size(); ++i) {
        EXPECT_GE(throughputs[i], throughputs[i-1]);
    }
}

/**
 * @brief 测试频率对性能的影响
 */
TEST_F(CommunicationModelAPICalculationsTest, FrequencyImpactOnPerformance) {
    std::vector<double> frequencies = {900.0, 1800.0, 2400.0, 5800.0};
    std::vector<CommunicationLinkStatus> statuses;
    
    for (double freq : frequencies) {
        api->setFrequency(freq);
        statuses.push_back(api->calculateLinkStatus());
    }
    
    // 验证所有频率都能产生有效结果
    for (const auto& status : statuses) {
        EXPECT_GT(status.signalStrength, -200.0);
        EXPECT_GT(status.throughput, 0.0);
    }
}

/**
 * @brief 测试环境类型对性能的影响
 */
TEST_F(CommunicationModelAPICalculationsTest, EnvironmentTypeImpactOnPerformance) {
    std::vector<EnvironmentType> envTypes = {
        EnvironmentType::OPEN_FIELD,
        EnvironmentType::URBAN_AREA,
        EnvironmentType::MOUNTAINOUS
    };
    
    std::vector<double> signalStrengths;
    
    for (auto envType : envTypes) {
        api->setEnvironmentType(envType);
        auto status = api->calculateLinkStatus();
        signalStrengths.push_back(status.signalStrength);
    }
    
    // 验证开阔地信号强度最好，山区最差
    EXPECT_GE(signalStrengths[0], signalStrengths[1]); // OPEN_FIELD >= URBAN_AREA
    EXPECT_GE(signalStrengths[1], signalStrengths[2]); // URBAN_AREA >= MOUNTAINOUS
}

/**
 * @brief 测试缓存机制
 */
TEST_F(CommunicationModelAPICalculationsTest, CachingMechanism) {
    // 第一次计算
    auto status1 = api->calculateLinkStatus();
    
    // 第二次计算（应该使用缓存）
    auto status2 = api->calculateLinkStatus();
    
    // 验证结果一致
    EXPECT_EQ(status1.signalStrength, status2.signalStrength);
    EXPECT_EQ(status1.signalToNoiseRatio, status2.signalToNoiseRatio);
    EXPECT_EQ(status1.bitErrorRate, status2.bitErrorRate);
    
    // 修改参数后缓存应该失效
    api->setFrequency(5800.0);
    auto status3 = api->calculateLinkStatus();
    
    // 结果应该不同
    EXPECT_NE(status1.signalStrength, status3.signalStrength);
}

/**
 * @brief 测试边界条件
 */
TEST_F(CommunicationModelAPICalculationsTest, BoundaryConditions) {
    // 测试极小距离
    api->setDistance(0.001); // 1m
    auto statusNear = api->calculateLinkStatus();
    EXPECT_TRUE(statusNear.isConnected);
    EXPECT_GT(statusNear.signalStrength, -50.0);
    
    // 测试极大距离
    api->setDistance(100.0); // 100km
    auto statusFar = api->calculateLinkStatus();
    EXPECT_LT(statusFar.signalStrength, statusNear.signalStrength);
    
    // 测试极低功率
    api->setDistance(1.0);
    api->setTransmitPower(-10.0); // -10 dBm
    auto statusLowPower = api->calculateLinkStatus();
    EXPECT_LT(statusLowPower.signalStrength, statusNear.signalStrength);
    
    // 测试极高功率
    api->setTransmitPower(50.0); // 50 dBm
    auto statusHighPower = api->calculateLinkStatus();
    EXPECT_GT(statusHighPower.signalStrength, statusLowPower.signalStrength);
}

/**
 * @brief 测试数值稳定性
 */
TEST_F(CommunicationModelAPICalculationsTest, NumericalStability) {
    // 多次计算相同配置，验证结果稳定性
    std::vector<double> results;
    
    for (int i = 0; i < 10; ++i) {
        auto status = api->calculateLinkStatus();
        results.push_back(status.signalStrength);
    }
    
    // 验证所有结果相同
    for (size_t i = 1; i < results.size(); ++i) {
        EXPECT_DOUBLE_EQ(results[0], results[i]);
    }
}

/**
 * @brief 测试质量评估逻辑
 */
TEST_F(CommunicationModelAPICalculationsTest, QualityAssessment) {
    // 设置优秀条件
    api->setDistance(0.1);
    api->setTransmitPower(40.0);
    auto excellentStatus = api->calculateLinkStatus();
    EXPECT_TRUE(excellentStatus.quality == CommunicationQuality::EXCELLENT || 
                excellentStatus.quality == CommunicationQuality::GOOD);
    
    // 设置较差条件
    api->setDistance(50.0);
    api->setTransmitPower(0.0);
    auto poorStatus = api->calculateLinkStatus();
    EXPECT_TRUE(poorStatus.quality == CommunicationQuality::POOR || 
                poorStatus.quality == CommunicationQuality::FAILED ||
                poorStatus.quality == CommunicationQuality::FAIR);
}