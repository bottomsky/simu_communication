#include <gtest/gtest.h>
#include "SignalTransmissionModel.h"
#include "CommunicationDistanceModel.h"
#include <memory>
#include <chrono>
#include <vector>

class BasicIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 创建测试用的信号传输模型
        signalModel = std::make_unique<SignalTransmissionModel>(
            FrequencyBand::ULTRA_SHORT_WAVE,
            100000.0,  // 100MHz
            ModulationType::FM,
            25.0,      // 25kHz带宽
            20.0       // 20W功率
        );

        // 创建测试用的通信距离模型
        distanceModel = std::make_unique<CommunicationDistanceModel>(
            EnvironmentType::SUBURBAN,
            50.0,      // 50km最大视距
            -100.0,    // -100dBm接收灵敏度
            10.0,      // 10dB链路余量
            20.0,      // 20W发射功率
            100000.0   // 100MHz频率
        );
    }

    void TearDown() override {
        signalModel.reset();
        distanceModel.reset();
    }

    std::unique_ptr<SignalTransmissionModel> signalModel;
    std::unique_ptr<CommunicationDistanceModel> distanceModel;
};

// 测试信号传输模型和通信距离模型的参数一致性
TEST_F(BasicIntegrationTest, ParameterConsistency) {
    // 验证两个模型的功率参数一致
    EXPECT_DOUBLE_EQ(signalModel->getTransmitPower(), distanceModel->getTransmitPower());

    // 修改信号模型的功率，然后同步到距离模型
    EXPECT_TRUE(signalModel->setTransmitPower(30.0));
    EXPECT_TRUE(distanceModel->setTransmitPower(30.0));
    
    EXPECT_DOUBLE_EQ(signalModel->getTransmitPower(), distanceModel->getTransmitPower());
}

// 测试频率变化对通信距离的影响
TEST_F(BasicIntegrationTest, FrequencyImpactOnCommunicationRange) {
    // 记录初始通信距离
    double initialDistance = distanceModel->calculateEffectiveCommunicationDistance();
    
    // 改变信号频率到更高频段
    signalModel->setFrequencyBand(FrequencyBand::MICROWAVE);
    signalModel->setCenterFrequency(1000000.0); // 1GHz
    
    // 创建新的距离模型使用新频率
    CommunicationDistanceModel newDistanceModel(
        EnvironmentType::SUBURBAN,
        50.0,
        -100.0,
        10.0,
        signalModel->getTransmitPower(),
        signalModel->getCenterFrequency()
    );
    
    double newDistance = newDistanceModel.calculateEffectiveCommunicationDistance();
    
    // 高频信号的传播距离通常更短
    EXPECT_LT(newDistance, initialDistance);
}

// 测试功率变化对通信距离的影响
TEST_F(BasicIntegrationTest, PowerImpactOnCommunicationRange) {
    // 记录低功率时的通信距离
    signalModel->setTransmitPower(5.0);
    distanceModel->setTransmitPower(5.0);
    double lowPowerDistance = distanceModel->calculateEffectiveCommunicationDistance();
    
    // 增加功率
    signalModel->setTransmitPower(50.0);
    distanceModel->setTransmitPower(50.0);
    double highPowerDistance = distanceModel->calculateEffectiveCommunicationDistance();
    
    // 高功率应该有更远的通信距离
    EXPECT_GT(highPowerDistance, lowPowerDistance);
    
    // 功率增加10倍，距离应该有显著增加
    double powerRatio = 50.0 / 5.0; // 10倍
    double distanceRatio = highPowerDistance / lowPowerDistance;
    EXPECT_GT(distanceRatio, 1.5); // 距离至少增加50%
}

// 测试不同环境下的通信性能
TEST_F(BasicIntegrationTest, EnvironmentImpactOnCommunication) {
    struct TestCase {
        EnvironmentType env;
        std::string name;
        double expectedRelativeDistance; // 相对于自由空间的距离比例
    };

    std::vector<TestCase> testCases = {
        {EnvironmentType::FREE_SPACE, "自由空间", 1.0},
        {EnvironmentType::RURAL, "农村", 0.8},
        {EnvironmentType::SUBURBAN, "郊区", 0.6},
        {EnvironmentType::URBAN, "城市", 0.4},
        {EnvironmentType::DENSE_URBAN, "密集城市", 0.3},
        {EnvironmentType::INDOOR, "室内", 0.2}
    };

    // 计算自由空间的基准距离
    CommunicationDistanceModel freeSpaceModel(
        EnvironmentType::FREE_SPACE,
        100.0,
        -100.0,
        10.0,
        signalModel->getTransmitPower(),
        signalModel->getCenterFrequency()
    );
    double baselineDistance = freeSpaceModel.calculateEffectiveCommunicationDistance();

    // 测试各种环境
    for (const auto& testCase : testCases) {
        CommunicationDistanceModel envModel(
            testCase.env,
            100.0,
            -100.0,
            10.0,
            signalModel->getTransmitPower(),
            signalModel->getCenterFrequency()
        );
        
        double envDistance = envModel.calculateEffectiveCommunicationDistance();
        double actualRatio = envDistance / baselineDistance;
        
        // 验证距离比例在合理范围内
        EXPECT_LE(actualRatio, 1.0) << "环境: " << testCase.name;
        EXPECT_GE(actualRatio, 0.1) << "环境: " << testCase.name;
        
        // 对于非自由空间环境，距离应该更短
        if (testCase.env != EnvironmentType::FREE_SPACE) {
            EXPECT_LT(actualRatio, 1.0) << "环境: " << testCase.name;
        }
    }
}

// 测试调制方式对系统性能的影响
TEST_F(BasicIntegrationTest, ModulationImpactOnSystem) {
    std::vector<ModulationType> modulations = {
        ModulationType::AM,
        ModulationType::FM,
        ModulationType::PSK,
        ModulationType::QAM,
        ModulationType::OFDM
    };

    for (auto modulation : modulations) {
        signalModel->setModulationType(modulation);
        
        // 验证调制方式设置成功
        EXPECT_EQ(signalModel->getModulationType(), modulation);
        
        // 验证参数信息包含调制方式
        std::string info = signalModel->getParameterInfo();
        EXPECT_FALSE(info.empty());
        
        // 通信距离计算应该仍然正常工作
        double distance = distanceModel->calculateEffectiveCommunicationDistance();
        EXPECT_GT(distance, 0.0);
    }
}

// 测试系统边界条件
TEST_F(BasicIntegrationTest, SystemBoundaryConditions) {
    // 测试最小功率配置
    EXPECT_TRUE(signalModel->setTransmitPower(0.1));
    EXPECT_TRUE(distanceModel->setTransmitPower(0.1));
    
    double minPowerDistance = distanceModel->calculateEffectiveCommunicationDistance();
    EXPECT_GT(minPowerDistance, 0.0);
    
    // 测试最大功率配置
    EXPECT_TRUE(signalModel->setTransmitPower(100.0));
    EXPECT_TRUE(distanceModel->setTransmitPower(100.0));
    
    double maxPowerDistance = distanceModel->calculateEffectiveCommunicationDistance();
    EXPECT_GT(maxPowerDistance, minPowerDistance);
    
    // 测试极端环境条件
    distanceModel->setEnvironmentType(EnvironmentType::INDOOR);
    distanceModel->setReceiveSensitivity(-120.0); // 很低的灵敏度
    distanceModel->setLinkMargin(20.0); // 较大的链路余量
    
    double extremeDistance = distanceModel->calculateEffectiveCommunicationDistance();
    EXPECT_GT(extremeDistance, 0.0);
}

// 测试参数同步和一致性
TEST_F(BasicIntegrationTest, ParameterSynchronization) {
    // 创建一个参数同步函数
    auto syncParameters = [&]() {
        distanceModel->setTransmitPower(signalModel->getTransmitPower());
        // 在实际系统中，这里还会同步频率等其他参数
    };

    // 测试参数变化后的同步
    std::vector<double> testPowers = {1.0, 5.0, 10.0, 25.0, 50.0};
    
    for (double power : testPowers) {
        EXPECT_TRUE(signalModel->setTransmitPower(power));
        syncParameters();
        
        EXPECT_DOUBLE_EQ(signalModel->getTransmitPower(), distanceModel->getTransmitPower());
        
        // 验证同步后系统仍能正常工作
        double distance = distanceModel->calculateEffectiveCommunicationDistance();
        EXPECT_GT(distance, 0.0);
        
        std::string signalInfo = signalModel->getParameterInfo();
        std::string distanceInfo = distanceModel->getParameterInfo();
        EXPECT_FALSE(signalInfo.empty());
        EXPECT_FALSE(distanceInfo.empty());
    }
}

// 测试系统性能和稳定性
TEST_F(BasicIntegrationTest, SystemPerformanceAndStability) {
    auto start = std::chrono::high_resolution_clock::now();
    
    // 执行大量操作测试系统稳定性
    for (int i = 0; i < 100; ++i) {
        // 随机改变参数
        double power = 1.0 + (i % 50);
        double frequency = 50000.0 + i * 1000;
        
        signalModel->setTransmitPower(power);
        if (signalModel->setCenterFrequency(frequency)) {
            distanceModel->setTransmitPower(power);
            
            // 计算通信距离
            double distance = distanceModel->calculateEffectiveCommunicationDistance();
            EXPECT_GT(distance, 0.0);
            EXPECT_LT(distance, 1000.0); // 合理的上限
            
            // 生成参数信息
            std::string info1 = signalModel->getParameterInfo();
            std::string info2 = distanceModel->getParameterInfo();
            EXPECT_FALSE(info1.empty());
            EXPECT_FALSE(info2.empty());
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // 性能要求：100次完整操作应在500ms内完成
    EXPECT_LT(duration.count(), 500);
}

// 测试错误处理和恢复
TEST_F(BasicIntegrationTest, ErrorHandlingAndRecovery) {
    // 记录正常状态
    double normalPower = signalModel->getTransmitPower();
    double normalFreq = signalModel->getCenterFrequency();
    
    // 尝试设置无效参数
    EXPECT_FALSE(signalModel->setTransmitPower(-1.0));
    EXPECT_FALSE(signalModel->setTransmitPower(200.0));
    
    // 验证参数没有被破坏
    EXPECT_DOUBLE_EQ(signalModel->getTransmitPower(), normalPower);
    EXPECT_DOUBLE_EQ(signalModel->getCenterFrequency(), normalFreq);
    
    // 系统应该仍能正常工作
    double distance = distanceModel->calculateEffectiveCommunicationDistance();
    EXPECT_GT(distance, 0.0);
    
    // 尝试无效的频率设置
    EXPECT_FALSE(signalModel->setCenterFrequency(500000.0)); // 超出超短波范围
    EXPECT_DOUBLE_EQ(signalModel->getCenterFrequency(), normalFreq);
}