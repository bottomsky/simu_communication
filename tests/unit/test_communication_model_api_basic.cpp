#include <gtest/gtest.h>
#include "CommunicationModelAPI.h"
#include "MathConstants.h"
#include <memory>

/**
 * @brief CommunicationModelAPI基础功能测试类
 */
class CommunicationModelAPIBasicTest : public ::testing::Test {
protected:
    void SetUp() override {
        api = std::make_unique<CommunicationModelAPI>();
    }

    void TearDown() override {
        api.reset();
    }

    std::unique_ptr<CommunicationModelAPI> api;
};

/**
 * @brief 测试默认构造函数
 */
TEST_F(CommunicationModelAPIBasicTest, DefaultConstructor) {
    EXPECT_NE(api, nullptr);
    EXPECT_EQ(api->getScenario(), CommunicationScenario::NORMAL_COMMUNICATION);
    
    // 检查默认环境参数
    auto env = api->getEnvironment();
    EXPECT_EQ(env.frequency, MathConstants::DEFAULT_FREQUENCY);
    EXPECT_EQ(env.bandwidth, MathConstants::DEFAULT_BANDWIDTH);
    EXPECT_EQ(env.transmitPower, MathConstants::DEFAULT_TRANSMIT_POWER);
    EXPECT_EQ(env.environmentType, EnvironmentType::OPEN_FIELD);
}

/**
 * @brief 测试带场景参数的构造函数
 */
TEST_F(CommunicationModelAPIBasicTest, ScenarioConstructor) {
    auto jammedApi = std::make_unique<CommunicationModelAPI>(CommunicationScenario::JAMMED_COMMUNICATION);
    EXPECT_EQ(jammedApi->getScenario(), CommunicationScenario::JAMMED_COMMUNICATION);
    
    auto antiJamApi = std::make_unique<CommunicationModelAPI>(CommunicationScenario::ANTI_JAM_COMMUNICATION);
    EXPECT_EQ(antiJamApi->getScenario(), CommunicationScenario::ANTI_JAM_COMMUNICATION);
}

/**
 * @brief 测试场景设置
 */
TEST_F(CommunicationModelAPIBasicTest, SetScenario) {
    EXPECT_TRUE(api->setScenario(CommunicationScenario::JAMMED_COMMUNICATION));
    EXPECT_EQ(api->getScenario(), CommunicationScenario::JAMMED_COMMUNICATION);
    
    EXPECT_TRUE(api->setScenario(CommunicationScenario::ANTI_JAM_COMMUNICATION));
    EXPECT_EQ(api->getScenario(), CommunicationScenario::ANTI_JAM_COMMUNICATION);
    
    EXPECT_TRUE(api->setScenario(CommunicationScenario::MULTI_USER_COMMUNICATION));
    EXPECT_EQ(api->getScenario(), CommunicationScenario::MULTI_USER_COMMUNICATION);
}

/**
 * @brief 测试频率设置
 */
TEST_F(CommunicationModelAPIBasicTest, SetFrequency) {
    // 有效频率范围测试
    EXPECT_TRUE(api->setFrequency(2400.0)); // 2.4 GHz
    EXPECT_EQ(api->getEnvironment().frequency, 2400.0);
    
    EXPECT_TRUE(api->setFrequency(5800.0)); // 5.8 GHz
    EXPECT_EQ(api->getEnvironment().frequency, 5800.0);
    
    // 边界值测试
    EXPECT_TRUE(api->setFrequency(MathConstants::FREQUENCY_VALIDATION_MIN));
    EXPECT_TRUE(api->setFrequency(MathConstants::FREQUENCY_VALIDATION_MAX));
    
    // 无效频率测试
    EXPECT_FALSE(api->setFrequency(-100.0));
    EXPECT_FALSE(api->setFrequency(100000.0));
}

/**
 * @brief 测试带宽设置
 */
TEST_F(CommunicationModelAPIBasicTest, SetBandwidth) {
    // 有效带宽测试
    EXPECT_TRUE(api->setBandwidth(20.0)); // 20 MHz
    EXPECT_EQ(api->getEnvironment().bandwidth, 20.0);
    
    EXPECT_TRUE(api->setBandwidth(100.0)); // 100 MHz
    EXPECT_EQ(api->getEnvironment().bandwidth, 100.0);
    
    // 边界值测试
    EXPECT_TRUE(api->setBandwidth(MathConstants::BANDWIDTH_VALIDATION_MIN));
    EXPECT_TRUE(api->setBandwidth(MathConstants::BANDWIDTH_VALIDATION_MAX));
    
    // 无效带宽测试
    EXPECT_FALSE(api->setBandwidth(-10.0));
    EXPECT_FALSE(api->setBandwidth(10000.0));
}

/**
 * @brief 测试发射功率设置
 */
TEST_F(CommunicationModelAPIBasicTest, SetTransmitPower) {
    // 有效功率测试
    EXPECT_TRUE(api->setTransmitPower(30.0)); // 30 dBm
    EXPECT_EQ(api->getEnvironment().transmitPower, 30.0);
    
    EXPECT_TRUE(api->setTransmitPower(10.0)); // 10 dBm
    EXPECT_EQ(api->getEnvironment().transmitPower, 10.0);
    
    // 边界值测试
    EXPECT_TRUE(api->setTransmitPower(MathConstants::POWER_VALIDATION_MIN));
    EXPECT_TRUE(api->setTransmitPower(MathConstants::POWER_VALIDATION_MAX));
    
    // 无效功率测试
    EXPECT_FALSE(api->setTransmitPower(-200.0));
    EXPECT_FALSE(api->setTransmitPower(200.0));
}

/**
 * @brief 测试距离设置
 */
TEST_F(CommunicationModelAPIBasicTest, SetDistance) {
    // 有效距离测试
    EXPECT_TRUE(api->setDistance(1.0)); // 1 km
    EXPECT_EQ(api->getEnvironment().distance, 1.0);
    
    EXPECT_TRUE(api->setDistance(50.0)); // 50 km
    EXPECT_EQ(api->getEnvironment().distance, 50.0);
    
    // 边界值测试
    EXPECT_TRUE(api->setDistance(MathConstants::DISTANCE_VALIDATION_MIN));
    EXPECT_TRUE(api->setDistance(MathConstants::DISTANCE_VALIDATION_MAX));
    
    // 无效距离测试
    EXPECT_FALSE(api->setDistance(-1.0));
    EXPECT_FALSE(api->setDistance(10000.0));
}

/**
 * @brief 测试环境类型设置
 */
TEST_F(CommunicationModelAPIBasicTest, SetEnvironmentType) {
    EXPECT_TRUE(api->setEnvironmentType(EnvironmentType::URBAN_AREA));
    EXPECT_EQ(api->getEnvironment().environmentType, EnvironmentType::URBAN_AREA);
    
    EXPECT_TRUE(api->setEnvironmentType(EnvironmentType::MOUNTAINOUS));
    EXPECT_EQ(api->getEnvironment().environmentType, EnvironmentType::MOUNTAINOUS);
    
    EXPECT_TRUE(api->setEnvironmentType(EnvironmentType::OPEN_FIELD));
    EXPECT_EQ(api->getEnvironment().environmentType, EnvironmentType::OPEN_FIELD);
}

/**
 * @brief 测试环境参数整体设置
 */
TEST_F(CommunicationModelAPIBasicTest, SetEnvironment) {
    CommunicationEnvironment env;
    env.frequency = 2400.0;
    env.bandwidth = 20.0;
    env.transmitPower = 30.0;
    env.distance = 5.0;
    env.environmentType = EnvironmentType::URBAN_AREA;
    env.temperature = 25.0;
    env.humidity = 60.0;
    env.atmosphericPressure = 1013.25;
    
    EXPECT_TRUE(api->setEnvironment(env));
    
    auto retrievedEnv = api->getEnvironment();
    EXPECT_EQ(retrievedEnv.frequency, 2400.0);
    EXPECT_EQ(retrievedEnv.bandwidth, 20.0);
    EXPECT_EQ(retrievedEnv.transmitPower, 30.0);
    EXPECT_EQ(retrievedEnv.distance, 5.0);
    EXPECT_EQ(retrievedEnv.environmentType, EnvironmentType::URBAN_AREA);
    EXPECT_EQ(retrievedEnv.temperature, 25.0);
    EXPECT_EQ(retrievedEnv.humidity, 60.0);
    EXPECT_EQ(retrievedEnv.atmosphericPressure, 1013.25);
}

/**
 * @brief 测试干扰环境设置
 */
TEST_F(CommunicationModelAPIBasicTest, SetJammingEnvironment) {
    JammingEnvironment jammingEnv;
    jammingEnv.isJammed = true;
    jammingEnv.jammerType = JammerType::GAUSSIAN_NOISE;
    jammingEnv.jammerPower = 20.0;
    jammingEnv.jammerFrequency = 2400.0;
    jammingEnv.jammerBandwidth = 50.0;
    jammingEnv.jammerDistance = 2.0;
    jammingEnv.jammerDensity = 0.5;
    
    EXPECT_TRUE(api->setJammingEnvironment(jammingEnv));
    
    auto retrievedJammingEnv = api->getJammingEnvironment();
    EXPECT_TRUE(retrievedJammingEnv.isJammed);
    EXPECT_EQ(retrievedJammingEnv.jammerType, JammerType::GAUSSIAN_NOISE);
    EXPECT_EQ(retrievedJammingEnv.jammerPower, 20.0);
    EXPECT_EQ(retrievedJammingEnv.jammerFrequency, 2400.0);
    EXPECT_EQ(retrievedJammingEnv.jammerBandwidth, 50.0);
    EXPECT_EQ(retrievedJammingEnv.jammerDistance, 2.0);
    EXPECT_EQ(retrievedJammingEnv.jammerDensity, 0.5);
}

/**
 * @brief 测试底层模型访问
 */
TEST_F(CommunicationModelAPIBasicTest, GetUnderlyingModels) {
    EXPECT_NE(api->getSignalModel(), nullptr);
    EXPECT_NE(api->getDistanceModel(), nullptr);
    EXPECT_NE(api->getReceiveModel(), nullptr);
    EXPECT_NE(api->getJammerModel(), nullptr);
    EXPECT_NE(api->getAntiJamModel(), nullptr);
}

/**
 * @brief 测试版本信息
 */
TEST_F(CommunicationModelAPIBasicTest, VersionInfo) {
    std::string version = CommunicationModelAPI::getVersion();
    EXPECT_FALSE(version.empty());
    
    std::string buildInfo = CommunicationModelAPI::getBuildInfo();
    EXPECT_FALSE(buildInfo.empty());
    
    std::string modelInfo = api->getModelInfo();
    EXPECT_FALSE(modelInfo.empty());
    
    std::string capabilities = api->getCapabilities();
    EXPECT_FALSE(capabilities.empty());
}