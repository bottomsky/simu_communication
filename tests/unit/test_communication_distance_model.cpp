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
            50.0,      // 50km最大视距
            EnvironmentType::URBAN_AREA,
            2.0,       // 衰减因子 (URBAN_AREA: 1.5-2.5)
            -100.0,    // -100dBm接收灵敏度
            10.0,      // 10dB链路余量
            20.0       // 20dBm发射功率
        );
    });
}

// 测试构造函数 - 无效参数
TEST_F(CommunicationDistanceModelTest, ConstructorInvalidParameters) {
    // 无效最大视距
    EXPECT_THROW({
        CommunicationDistanceModel model(
            -10.0,     // 负距离
            EnvironmentType::URBAN_AREA,
            2.0,       // 衰减因子 (URBAN_AREA: 1.5-2.5)
            -100.0,
            10.0,
            20.0
        );
    }, std::invalid_argument);

    // 无效接收灵敏度
    EXPECT_THROW({
        CommunicationDistanceModel model(
            50.0,
            EnvironmentType::URBAN_AREA,
            2.0,       // 衰减因子 (URBAN_AREA: 1.5-2.5)
            50.0,      // 正值灵敏度（应该为负）
            10.0,
            20.0
        );
    }, std::invalid_argument);

    // 无效链路余量
    EXPECT_THROW({
        CommunicationDistanceModel model(
            50.0,
            EnvironmentType::URBAN_AREA,
            2.0,       // 衰减因子 (URBAN_AREA: 1.5-2.5)
            -100.0,
            -5.0,      // 负链路余量
            20.0
        );
    }, std::invalid_argument);

    // 无效发射功率
    EXPECT_THROW({
        CommunicationDistanceModel model(
            50.0,
            EnvironmentType::URBAN_AREA,
            2.0,       // 衰减因子 (URBAN_AREA: 1.5-2.5)
            -100.0,
            10.0,
            -50.0      // 超出范围的功率
        );
    }, std::invalid_argument);
}





// 测试参数设置和获取
TEST_F(CommunicationDistanceModelTest, ParameterSettersAndGetters) {
    CommunicationDistanceModel model(
        30.0,      // 最大视距
        EnvironmentType::OPEN_FIELD,
        1.0,       // 衰减因子 (OPEN_FIELD: 0.8-1.2)
        -110.0,    // 接收灵敏度
        15.0,      // 链路余量
        10.0       // 发射功率
    );

    // 测试获取初始参数
    EXPECT_EQ(model.getEnvironmentType(), EnvironmentType::OPEN_FIELD);
    EXPECT_DOUBLE_EQ(model.getMaxLineOfSight(), 30.0);
    EXPECT_DOUBLE_EQ(model.getReceiveSensitivity(), -110.0);
    EXPECT_DOUBLE_EQ(model.getLinkMargin(), 15.0);
    EXPECT_DOUBLE_EQ(model.getTransmitPower(), 10.0);

    // 测试设置环境类型
    model.setEnvironmentType(EnvironmentType::URBAN_AREA);
    EXPECT_EQ(model.getEnvironmentType(), EnvironmentType::URBAN_AREA);

    // 测试设置有效的最大视距
    EXPECT_TRUE(model.setMaxLineOfSight(40.0));
    EXPECT_DOUBLE_EQ(model.getMaxLineOfSight(), 40.0);

    // 测试设置无效的最大视距
    EXPECT_FALSE(model.setMaxLineOfSight(-10.0));
    EXPECT_DOUBLE_EQ(model.getMaxLineOfSight(), 40.0); // 应该保持原值

    // 测试设置有效的接收灵敏度
    EXPECT_TRUE(model.setReceiveSensitivity(-100.0));
    EXPECT_DOUBLE_EQ(model.getReceiveSensitivity(), -100.0);

    // 测试设置无效的接收灵敏度
    EXPECT_FALSE(model.setReceiveSensitivity(10.0));
    EXPECT_DOUBLE_EQ(model.getReceiveSensitivity(), -100.0); // 应该保持原值

    // 测试设置有效的链路余量
    EXPECT_TRUE(model.setLinkMargin(20.0));
    EXPECT_DOUBLE_EQ(model.getLinkMargin(), 20.0);

    // 测试设置无效的链路余量
    EXPECT_FALSE(model.setLinkMargin(-5.0));
    EXPECT_DOUBLE_EQ(model.getLinkMargin(), 20.0); // 应该保持原值

    // 测试设置有效的发射功率
    EXPECT_TRUE(model.setTransmitPower(25.0));
    EXPECT_DOUBLE_EQ(model.getTransmitPower(), 25.0);

    // 测试设置无效的发射功率
    EXPECT_FALSE(model.setTransmitPower(-35.0));
    EXPECT_FALSE(model.setTransmitPower(35.0));
    EXPECT_DOUBLE_EQ(model.getTransmitPower(), 25.0); // 应该保持原值
}

// 测试有效通信距离计算
TEST_F(CommunicationDistanceModelTest, EffectiveCommunicationDistanceCalculation) {
    CommunicationDistanceModel model(
        20.0,      // 20km最大视距
        EnvironmentType::OPEN_FIELD,
        1.0,       // 衰减因子 (OPEN_FIELD: 0.8-1.2)
        -95.0,     // -95dBm接收灵敏度
        10.0,      // 10dB链路余量
        15.0       // 15dBm发射功率
    );

    double distance = model.calculateEffectiveDistance();
    
    // 验证计算结果合理性
    EXPECT_GT(distance, 0.0);
    EXPECT_LE(distance, 20.0); // 不应超过最大视距

    // 测试不同环境下的距离差异
    model.setEnvironmentType(EnvironmentType::URBAN_AREA);
    double urbanDistance = model.calculateEffectiveDistance();
    
    model.setEnvironmentType(EnvironmentType::OPEN_FIELD);
    double freeSpaceDistance = model.calculateEffectiveDistance();
    
    // 自由空间的通信距离应该大于等于城市环境
    EXPECT_GE(freeSpaceDistance, urbanDistance);
}

// 测试频率对通信距离的影响
TEST_F(CommunicationDistanceModelTest, FrequencyImpactOnDistance) {
    CommunicationDistanceModel lowFreqModel(
        15.0,      // 较小的最大视距
        EnvironmentType::OPEN_FIELD,
        0.8,       // 衰减因子 (OPEN_FIELD: 0.8-1.2) - 低频
        -95.0,     // 接收灵敏度
        10.0,      // 链路余量
        15.0       // 发射功率
    );

    CommunicationDistanceModel highFreqModel(
        15.0,      // 较小的最大视距
        EnvironmentType::OPEN_FIELD,
        1.2,       // 衰减因子 (OPEN_FIELD: 0.8-1.2) - 高频
        -95.0,     // 接收灵敏度
        10.0,      // 链路余量
        15.0       // 发射功率
    );

    double lowFreqDistance = lowFreqModel.calculateEffectiveDistance();
    double highFreqDistance = highFreqModel.calculateEffectiveDistance();

    // 低频信号的传播距离通常更远
    EXPECT_GE(lowFreqDistance, highFreqDistance);
}

// 测试功率对通信距离的影响
TEST_F(CommunicationDistanceModelTest, PowerImpactOnDistance) {
    CommunicationDistanceModel lowPowerModel(
        10.0,      // 较小的最大视距，避免达到限制
        EnvironmentType::URBAN_AREA,
        2.0,       // 衰减因子 (URBAN_AREA: 1.5-2.5)
        -90.0,     // 较高的接收灵敏度，减少计算距离
        10.0,      // 链路余量
        5.0        // 5dBm
    );

    CommunicationDistanceModel highPowerModel(
        10.0,      // 较小的最大视距，避免达到限制
        EnvironmentType::URBAN_AREA,
        2.0,       // 衰减因子 (URBAN_AREA: 1.5-2.5)
        -90.0,     // 较高的接收灵敏度，减少计算距离
        10.0,      // 链路余量
        25.0       // 25dBm
    );

    double lowPowerDistance = lowPowerModel.calculateEffectiveDistance();
    double highPowerDistance = highPowerModel.calculateEffectiveDistance();

    // 高功率应该有更远的通信距离
    EXPECT_GE(highPowerDistance, lowPowerDistance);
}

// 测试参数信息字符串生成
TEST_F(CommunicationDistanceModelTest, ParameterInfoGeneration) {
    CommunicationDistanceModel model(
        50.0,      // 最大视距
        EnvironmentType::URBAN_AREA,
        2.0,       // 衰减因子 (URBAN_AREA: 1.5-2.5)
        -100.0,    // 接收灵敏度
        10.0,      // 链路余量
        20.0       // 发射功率
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
            0.5,       // 最小视距
            EnvironmentType::OPEN_FIELD,
            0.8,       // 衰减因子 (OPEN_FIELD: 0.8-1.2)
            -110.0,    // 接收灵敏度
            5.0,       // 最小链路余量
            -30.0      // 最小功率
        );
    });

    // 测试最大参数值
    EXPECT_NO_THROW({
        CommunicationDistanceModel model(
            50.0,      // 最大视距
            EnvironmentType::URBAN_AREA,
            2.5,       // 衰减因子 (URBAN_AREA: 1.5-2.5)
            -90.0,     // 较高的接收灵敏度
            20.0,      // 较大链路余量
            30.0       // 最大功率
        );
    });
}

// 测试计算精度
TEST_F(CommunicationDistanceModelTest, CalculationPrecision) {
    CommunicationDistanceModel model(
        5.0,       // 较小的最大视距
        EnvironmentType::OPEN_FIELD,
        1.0,       // 衰减因子
        -90.0,     // 较高的接收灵敏度
        10.0,      // 链路余量
        10.0       // 较低的发射功率
    );

    // 多次计算应该得到相同结果
    double distance1 = model.calculateEffectiveDistance();
    double distance2 = model.calculateEffectiveDistance();
    double distance3 = model.calculateEffectiveDistance();

    EXPECT_DOUBLE_EQ(distance1, distance2);
    EXPECT_DOUBLE_EQ(distance2, distance3);

    // 结果应该是非负数
    EXPECT_GE(distance1, 0.0);
}

// 性能测试
TEST_F(CommunicationDistanceModelTest, PerformanceTest) {
    auto start = std::chrono::high_resolution_clock::now();
    
    // 创建1000个模型实例并计算距离
    for (int i = 0; i < 1000; ++i) {
        EnvironmentType env = static_cast<EnvironmentType>(i % 3);
        double attenuation;
        switch (env) {
            case EnvironmentType::OPEN_FIELD:
                attenuation = 0.8 + (i % 40) * 0.01; // 0.8-1.2
                break;
            case EnvironmentType::URBAN_AREA:
                attenuation = 1.5 + (i % 100) * 0.01; // 1.5-2.5
                break;
            case EnvironmentType::MOUNTAINOUS:
                attenuation = 2.0 + (i % 100) * 0.01; // 2.0-3.0
                break;
        }
        
        CommunicationDistanceModel model(
            0.5 + (i % 495) * 0.1,              // 最大视距 (0.5-50km范围内)
            env,                                 // 环境类型
            attenuation,                         // 衰减因子
            -110.0 + (i % 20) * 1.0,            // 接收灵敏度 (-110到-90)
            5.0 + (i % 15) * 1.0,               // 链路余量 (5-20)
            -30.0 + (i % 60) * 1.0              // 发射功率 (-30到30)
        );
        
        double distance = model.calculateEffectiveDistance();
        std::string info = model.getParameterInfo();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // 性能要求：1000次计算应在200ms内完成
    EXPECT_LT(duration.count(), 200);
}
