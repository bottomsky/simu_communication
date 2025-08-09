#include <gtest/gtest.h>
#include "SignalTransmissionModel.h"
#include <stdexcept>
#include <chrono>

class SignalTransmissionModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 测试前的设置
    }

    void TearDown() override {
        // 测试后的清理
    }
};

// 测试构造函数 - 正常情况
TEST_F(SignalTransmissionModelTest, ConstructorValidParameters) {
    EXPECT_NO_THROW({
        SignalTransmissionModel model(
            FrequencyBand::ULTRA_SHORT_WAVE,
            100000.0,  // 100MHz
            ModulationType::FM,
            25.0,      // 25kHz带宽
            10.0       // 10W功率
        );
    });
}

// 测试构造函数 - 频率超出范围
TEST_F(SignalTransmissionModelTest, ConstructorInvalidFrequency) {
    EXPECT_THROW({
        SignalTransmissionModel model(
            FrequencyBand::SHORT_WAVE,
            100000.0,  // 100MHz，超出短波范围
            ModulationType::AM,
            10.0,
            5.0
        );
    }, std::invalid_argument);
}

// 测试构造函数 - 无效带宽
TEST_F(SignalTransmissionModelTest, ConstructorInvalidBandwidth) {
    EXPECT_THROW({
        SignalTransmissionModel model(
            FrequencyBand::SHORT_WAVE,
            15000.0,   // 15MHz
            ModulationType::AM,
            -5.0,      // 负带宽
            5.0
        );
    }, std::invalid_argument);
}

// 测试构造函数 - 无效功率
TEST_F(SignalTransmissionModelTest, ConstructorInvalidPower) {
    EXPECT_THROW({
        SignalTransmissionModel model(
            FrequencyBand::SHORT_WAVE,
            15000.0,   // 15MHz
            ModulationType::AM,
            10.0,
            -1.0       // 负功率
        );
    }, std::invalid_argument);
}

// 测试频率范围校验
TEST_F(SignalTransmissionModelTest, FrequencyBandValidation) {
    SignalTransmissionModel model(
        FrequencyBand::SHORT_WAVE,
        15000.0,
        ModulationType::AM,
        10.0,
        5.0
    );

    // 短波范围内的频率应该有效
    EXPECT_TRUE(model.isFrequencyInBand(3000.0, FrequencyBand::SHORT_WAVE));
    EXPECT_TRUE(model.isFrequencyInBand(15000.0, FrequencyBand::SHORT_WAVE));
    EXPECT_TRUE(model.isFrequencyInBand(29000.0, FrequencyBand::SHORT_WAVE));

    // 超出短波范围的频率应该无效
    EXPECT_FALSE(model.isFrequencyInBand(1000.0, FrequencyBand::SHORT_WAVE));
    EXPECT_FALSE(model.isFrequencyInBand(35000.0, FrequencyBand::SHORT_WAVE));

    // 超短波范围测试
    EXPECT_TRUE(model.isFrequencyInBand(50000.0, FrequencyBand::ULTRA_SHORT_WAVE));
    EXPECT_TRUE(model.isFrequencyInBand(150000.0, FrequencyBand::ULTRA_SHORT_WAVE));
    EXPECT_FALSE(model.isFrequencyInBand(25000.0, FrequencyBand::ULTRA_SHORT_WAVE));

    // 微波范围测试
    EXPECT_TRUE(model.isFrequencyInBand(1000000.0, FrequencyBand::MICROWAVE));
    EXPECT_TRUE(model.isFrequencyInBand(15000000.0, FrequencyBand::MICROWAVE));
    EXPECT_FALSE(model.isFrequencyInBand(200000.0, FrequencyBand::MICROWAVE));
}

// 测试参数设置和获取
TEST_F(SignalTransmissionModelTest, ParameterSettersAndGetters) {
    SignalTransmissionModel model(
        FrequencyBand::ULTRA_SHORT_WAVE,
        100000.0,
        ModulationType::FM,
        25.0,
        10.0
    );

    // 测试获取初始参数
    EXPECT_EQ(model.getFrequencyBand(), FrequencyBand::ULTRA_SHORT_WAVE);
    EXPECT_DOUBLE_EQ(model.getCenterFrequency(), 100000.0);
    EXPECT_EQ(model.getModulationType(), ModulationType::FM);
    EXPECT_DOUBLE_EQ(model.getSignalBandwidth(), 25.0);
    EXPECT_DOUBLE_EQ(model.getTransmitPower(), 10.0);

    // 测试设置有效的中心频率
    EXPECT_TRUE(model.setCenterFrequency(150000.0));
    EXPECT_DOUBLE_EQ(model.getCenterFrequency(), 150000.0);

    // 测试设置无效的中心频率（超出频段范围）
    EXPECT_FALSE(model.setCenterFrequency(500000.0));
    EXPECT_DOUBLE_EQ(model.getCenterFrequency(), 150000.0); // 应该保持原值

    // 测试设置调制方式
    model.setModulationType(ModulationType::BPSK);
    EXPECT_EQ(model.getModulationType(), ModulationType::BPSK);

    // 测试设置有效带宽
    EXPECT_TRUE(model.setSignalBandwidth(50.0));
    EXPECT_DOUBLE_EQ(model.getSignalBandwidth(), 50.0);

    // 测试设置无效带宽
    EXPECT_FALSE(model.setSignalBandwidth(-10.0));
    EXPECT_DOUBLE_EQ(model.getSignalBandwidth(), 50.0); // 应该保持原值

    // 测试设置有效功率
    EXPECT_TRUE(model.setTransmitPower(20.0));
    EXPECT_DOUBLE_EQ(model.getTransmitPower(), 20.0);

    // 测试设置无效功率
    EXPECT_FALSE(model.setTransmitPower(-5.0));
    EXPECT_FALSE(model.setTransmitPower(150.0)); // 超过100W限制
    EXPECT_DOUBLE_EQ(model.getTransmitPower(), 20.0); // 应该保持原值
}

// 测试频段切换
TEST_F(SignalTransmissionModelTest, FrequencyBandSwitching) {
    SignalTransmissionModel model(
        FrequencyBand::SHORT_WAVE,
        15000.0,  // 15MHz
        ModulationType::AM,
        10.0,
        5.0
    );

    // 切换到超短波频段，原频率不符合，应该自动调整
    model.setFrequencyBand(FrequencyBand::ULTRA_SHORT_WAVE);
    EXPECT_EQ(model.getFrequencyBand(), FrequencyBand::ULTRA_SHORT_WAVE);
    EXPECT_DOUBLE_EQ(model.getCenterFrequency(), 165000.0); // 应该调整到165MHz

    // 切换到微波频段
    model.setFrequencyBand(FrequencyBand::MICROWAVE);
    EXPECT_EQ(model.getFrequencyBand(), FrequencyBand::MICROWAVE);
    EXPECT_DOUBLE_EQ(model.getCenterFrequency(), 15150000.0); // 应该调整到15.15GHz
}

// 测试参数信息字符串生成
TEST_F(SignalTransmissionModelTest, ParameterInfoGeneration) {
    SignalTransmissionModel model(
        FrequencyBand::ULTRA_SHORT_WAVE,
        100000.0,  // 100MHz
        ModulationType::FM,
        25.0,      // 25kHz
        10.0       // 10W
    );

    std::string info = model.getParameterInfo();
    
    // 检查信息字符串包含关键信息
    EXPECT_NE(info.find("信号传输模型参数"), std::string::npos);
    EXPECT_NE(info.find("超短波"), std::string::npos);
    EXPECT_NE(info.find("100000"), std::string::npos);
    EXPECT_NE(info.find("调频"), std::string::npos);
    EXPECT_NE(info.find("25"), std::string::npos);
    EXPECT_NE(info.find("10"), std::string::npos);
}

// 测试边界值
TEST_F(SignalTransmissionModelTest, BoundaryValues) {
    // 测试短波频段边界
    EXPECT_NO_THROW({
        SignalTransmissionModel model1(
            FrequencyBand::SHORT_WAVE,
            1500.0,    // 最小值
            ModulationType::AM,
            0.1,       // 最小带宽
            0.1        // 最小功率
        );
    });

    EXPECT_NO_THROW({
        SignalTransmissionModel model2(
            FrequencyBand::SHORT_WAVE,
            30000.0,   // 最大值
            ModulationType::AM,
            1000.0,    // 较大带宽
            100.0      // 最大功率
        );
    });

    // 测试超出边界的值
    EXPECT_THROW({
        SignalTransmissionModel model3(
            FrequencyBand::SHORT_WAVE,
            1499.0,    // 低于最小值
            ModulationType::AM,
            10.0,
            5.0
        );
    }, std::invalid_argument);
}

// 性能测试
TEST_F(SignalTransmissionModelTest, PerformanceTest) {
    auto start = std::chrono::high_resolution_clock::now();
    
    // 创建1000个模型实例
    for (int i = 0; i < 1000; ++i) {
        SignalTransmissionModel model(
            FrequencyBand::ULTRA_SHORT_WAVE,
            100000.0 + i,
            ModulationType::FM,
            25.0,
            10.0
        );
        
        // 执行一些操作
        model.setCenterFrequency(100000.0 + i * 10);
        model.setSignalBandwidth(25.0 + i * 0.1);
        std::string info = model.getParameterInfo();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // 性能要求：1000次操作应在100ms内完成
    EXPECT_LT(duration.count(), 100);
}