#include <gtest/gtest.h>
#include "CommunicationDistanceModel.h"
#include <stdexcept>
#include <cmath>
#include <chrono>

class CommunicationDistanceModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 测试前的设置
    }

    void TearDown() override {
        // 测试后的清理
    }
};

// 测试构造函数 - 正常情况
TEST_F(CommunicationDistanceModelTest, ConstructorValidParameters) {
    EXPECT_NO_THROW({
        CommunicationDistanceModel model(
            EnvironmentType::URBAN,
            50.0,      // 50km最大视距
            -100.0,    // -100dBm接收灵敏度
            10.0,      // 10dB链路余量
            20.0       // 20W发射功率
        );
    });
}

// 测试构造函数 - 无效参数
TEST_F(CommunicationDistanceModelTest, ConstructorInvalidParameters) {
    // 无效最大视距
    EXPECT_THROW({
        CommunicationDistanceModel model(
            EnvironmentType::URBAN,
            -10.0,     // 负距离
            -100.0,
            10.0,
            20.0
        );
    }, std::invalid_argument);

    // 无效接收灵敏度
    EXPECT_THROW({
        CommunicationDistanceModel model(
            EnvironmentType::URBAN,
            50.0,
            50.0,      // 正值灵敏度（应该为负）
            10.0,
            20.0
        );
    }, std::invalid_argument);

    // 无效链路余量
    EXPECT_THROW({
        CommunicationDistanceModel model(
            EnvironmentType::URBAN,
            50.0,
            -100.0,
            -5.0,      // 负链路余量
            20.0
        );
    }, std::invalid_argument);

    // 无效发射功率
    EXPECT_THROW({
        CommunicationDistanceModel model(
            EnvironmentType::URBAN,
            50.0,
            -100.0,
            10.0,
            -1.0       // 负功率
        );
    }, std::invalid_argument);
}

// 测试环境衰减系数
TEST_F(CommunicationDistanceModelTest, EnvironmentAttenuationCoefficient) {
    CommunicationDistanceModel model(
        EnvironmentType::URBAN,
        50.0,
        -100.0,
        10.0,
        20.0
    );

    // 测试不同环境的衰减系数
    EXPECT_DOUBLE_EQ(model.getEnvironmentAttenuationCoeff(EnvironmentType::FREE_SPACE), 2.0);
    EXPECT_DOUBLE_EQ(model.getEnvironmentAttenuationCoeff(EnvironmentType::RURAL), 2.5);
    EXPECT_DOUBLE_EQ(model.getEnvironmentAttenuationCoeff(EnvironmentType::SUBURBAN), 3.0);
    EXPECT_DOUBLE_EQ(model.getEnvironmentAttenuationCoeff(EnvironmentType::URBAN), 3.5);
    EXPECT_DOUBLE_EQ(model.getEnvironmentAttenuationCoeff(EnvironmentType::DENSE_URBAN), 4.0);
    EXPECT_DOUBLE_EQ(model.getEnvironmentAttenuationCoeff(EnvironmentType::INDOOR), 4.5);
}

// 测试功率参数范围校验
TEST_F(CommunicationDistanceModelTest, PowerParameterValidation) {
    CommunicationDistanceModel model(
        EnvironmentType::URBAN,
        50.0,
        -100.0,
        10.0,
        20.0
    );

    // 测试有效功率范围
    EXPECT_TRUE(model.isPowerInValidRange(0.1));
    EXPECT_TRUE(model.isPowerInValidRange(50.0));
    EXPECT_TRUE(model.isPowerInValidRange(1000.0));

    // 测试无效功率范围
    EXPECT_FALSE(model.isPowerInValidRange(0.0));
    EXPECT_FALSE(model.isPowerInValidRange(-1.0));
    EXPECT_FALSE(model.isPowerInValidRange(1001.0));
}

// 测试参数设置和获取
TEST_F(CommunicationDistanceModelTest, ParameterSettersAndGetters) {
    CommunicationDistanceModel model(
        EnvironmentType::RURAL,
        30.0,
        -110.0,
        15.0,
        10.0
    );

    // 测试获取初始参数
    EXPECT_EQ(model.getEnvironmentType(), EnvironmentType::RURAL);
    EXPECT_DOUBLE_EQ(model.getMaxLineOfSight(), 30.0);
    EXPECT_DOUBLE_EQ(model.getReceiveSensitivity(), -110.0);
    EXPECT_DOUBLE_EQ(model.getLinkMargin(), 15.0);
    EXPECT_DOUBLE_EQ(model.getTransmitPower(), 10.0);

    // 测试设置环境类型
    model.setEnvironmentType(EnvironmentType::URBAN);
    EXPECT_EQ(model.getEnvironmentType(), EnvironmentType::URBAN);

    // 测试设置有效的最大视距
    EXPECT_TRUE(model.setMaxLineOfSight(60.0));
    EXPECT_DOUBLE_EQ(model.getMaxLineOfSight(), 60.0);

    // 测试设置无效的最大视距
    EXPECT_FALSE(model.setMaxLineOfSight(-10.0));
    EXPECT_DOUBLE_EQ(model.getMaxLineOfSight(), 60.0); // 应该保持原值

    // 测试设置有效的接收灵敏度
    EXPECT_TRUE(model.setReceiveSensitivity(-120.0));
    EXPECT_DOUBLE_EQ(model.getReceiveSensitivity(), -120.0);

    // 测试设置无效的接收灵敏度
    EXPECT_FALSE(model.setReceiveSensitivity(10.0));
    EXPECT_DOUBLE_EQ(model.getReceiveSensitivity(), -120.0); // 应该保持原值

    // 测试设置有效的链路余量
    EXPECT_TRUE(model.setLinkMargin(20.0));
    EXPECT_DOUBLE_EQ(model.getLinkMargin(), 20.0);

    // 测试设置无效的链路余量
    EXPECT_FALSE(model.setLinkMargin(-5.0));
    EXPECT_DOUBLE_EQ(model.getLinkMargin(), 20.0); // 应该保持原值

    // 测试设置有效的发射功率
    EXPECT_TRUE(model.setTransmitPower(50.0));
    EXPECT_DOUBLE_EQ(model.getTransmitPower(), 50.0);

    // 测试设置无效的发射功率
    EXPECT_FALSE(model.setTransmitPower(-1.0));
    EXPECT_FALSE(model.setTransmitPower(1500.0));
    EXPECT_DOUBLE_EQ(model.getTransmitPower(), 50.0); // 应该保持原值
}

// 测试有效通信距离计算
TEST_F(CommunicationDistanceModelTest, EffectiveCommunicationDistanceCalculation) {
    CommunicationDistanceModel model(
        EnvironmentType::FREE_SPACE,
        100.0,     // 100km最大视距
        -100.0,    // -100dBm接收灵敏度
        10.0,      // 10dB链路余量
        20.0,      // 20W发射功率
        100000.0   // 100MHz频率
    );

    double distance = model.calculateEffectiveCommunicationDistance();
    
    // 验证计算结果合理性
    EXPECT_GT(distance, 0.0);
    EXPECT_LE(distance, 100.0); // 不应超过最大视距

    // 测试不同环境下的距离差异
    model.setEnvironmentType(EnvironmentType::URBAN);
    double urbanDistance = model.calculateEffectiveCommunicationDistance();
    
    model.setEnvironmentType(EnvironmentType::FREE_SPACE);
    double freeSpaceDistance = model.calculateEffectiveCommunicationDistance();
    
    // 自由空间的通信距离应该大于城市环境
    EXPECT_GT(freeSpaceDistance, urbanDistance);
}

// 测试频率对通信距离的影响
TEST_F(CommunicationDistanceModelTest, FrequencyImpactOnDistance) {
    CommunicationDistanceModel lowFreqModel(
        EnvironmentType::RURAL,
        50.0,
        -100.0,
        10.0,
        20.0,
        30000.0    // 30MHz (短波)
    );

    CommunicationDistanceModel highFreqModel(
        EnvironmentType::RURAL,
        50.0,
        -100.0,
        10.0,
        20.0,
        1000000.0  // 1GHz (微波)
    );

    double lowFreqDistance = lowFreqModel.calculateEffectiveCommunicationDistance();
    double highFreqDistance = highFreqModel.calculateEffectiveCommunicationDistance();

    // 低频信号的传播距离通常更远
    EXPECT_GT(lowFreqDistance, highFreqDistance);
}

// 测试功率对通信距离的影响
TEST_F(CommunicationDistanceModelTest, PowerImpactOnDistance) {
    CommunicationDistanceModel lowPowerModel(
        EnvironmentType::SUBURBAN,
        50.0,
        -100.0,
        10.0,
        5.0,       // 5W
        100000.0
    );

    CommunicationDistanceModel highPowerModel(
        EnvironmentType::SUBURBAN,
        50.0,
        -100.0,
        10.0,
        50.0,      // 50W
        100000.0
    );

    double lowPowerDistance = lowPowerModel.calculateEffectiveCommunicationDistance();
    double highPowerDistance = highPowerModel.calculateEffectiveCommunicationDistance();

    // 高功率应该有更远的通信距离
    EXPECT_GT(highPowerDistance, lowPowerDistance);
}

// 测试参数信息字符串生成
TEST_F(CommunicationDistanceModelTest, ParameterInfoGeneration) {
    CommunicationDistanceModel model(
        EnvironmentType::URBAN,
        50.0,
        -100.0,
        10.0,
        20.0,
        100000.0
    );

    std::string info = model.getParameterInfo();
    
    // 检查信息字符串包含关键信息
    EXPECT_NE(info.find("通信距离模型参数"), std::string::npos);
    EXPECT_NE(info.find("城市"), std::string::npos);
    EXPECT_NE(info.find("50"), std::string::npos);
    EXPECT_NE(info.find("-100"), std::string::npos);
    EXPECT_NE(info.find("10"), std::string::npos);
    EXPECT_NE(info.find("20"), std::string::npos);
}

// 测试边界条件
TEST_F(CommunicationDistanceModelTest, BoundaryConditions) {
    // 测试最小参数值
    EXPECT_NO_THROW({
        CommunicationDistanceModel model(
            EnvironmentType::FREE_SPACE,
            0.1,       // 最小视距
            -150.0,    // 很低的接收灵敏度
            0.1,       // 最小链路余量
            0.1,       // 最小功率
            1000.0     // 最低频率
        );
    });

    // 测试最大参数值
    EXPECT_NO_THROW({
        CommunicationDistanceModel model(
            EnvironmentType::INDOOR,
            1000.0,    // 最大视距
            -50.0,     // 较高的接收灵敏度
            50.0,      // 较大链路余量
            1000.0,    // 最大功率
            30000000.0 // 最高频率
        );
    });
}

// 测试计算精度
TEST_F(CommunicationDistanceModelTest, CalculationPrecision) {
    CommunicationDistanceModel model(
        EnvironmentType::RURAL,
        50.0,
        -100.0,
        10.0,
        20.0,
        100000.0
    );

    // 多次计算应该得到相同结果
    double distance1 = model.calculateEffectiveCommunicationDistance();
    double distance2 = model.calculateEffectiveCommunicationDistance();
    double distance3 = model.calculateEffectiveCommunicationDistance();

    EXPECT_DOUBLE_EQ(distance1, distance2);
    EXPECT_DOUBLE_EQ(distance2, distance3);

    // 结果应该有合理的精度（不应该是整数）
    EXPECT_NE(std::floor(distance1), distance1);
}

// 性能测试
TEST_F(CommunicationDistanceModelTest, PerformanceTest) {
    auto start = std::chrono::high_resolution_clock::now();
    
    // 创建1000个模型实例并计算距离
    for (int i = 0; i < 1000; ++i) {
        CommunicationDistanceModel model(
            static_cast<EnvironmentType>(i % 6),
            50.0 + i * 0.1,
            -100.0 - i * 0.01,
            10.0 + i * 0.01,
            20.0 + i * 0.1,
            100000.0 + i * 100
        );
        
        double distance = model.calculateEffectiveCommunicationDistance();
        std::string info = model.getParameterInfo();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // 性能要求：1000次计算应在200ms内完成
    EXPECT_LT(duration.count(), 200);
}