#include "CommunicationReceiveModel.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <vector>

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

// 测试构造函数和基本参数设置
bool testConstructorAndBasicParameters() {
    std::cout << "测试: 构造函数和基本参数设置..." << std::endl;
    
    // 测试默认构造函数
    CommunicationReceiveModel model;
    EXPECT_NEAR(model.getReceiveSensitivity(), -100.0, 0.01);
    EXPECT_NEAR(model.getNoiseFigure(), 3.0, 0.01);
    EXPECT_NEAR(model.getSystemBandwidth(), 25.0, 0.01);
    EXPECT_TRUE(model.getModulationType() == ReceiveModulationType::FM);
    EXPECT_TRUE(model.getReceiverType() == ReceiverType::SUPERHETERODYNE);
    
    // 测试带参数的构造函数
    CommunicationReceiveModel model2(-90.0, 5.0, 50.0, 
                                   ReceiveModulationType::BPSK, 
                                   ReceiverType::DIRECT_CONVERSION);
    EXPECT_NEAR(model2.getReceiveSensitivity(), -90.0, 0.01);
    EXPECT_NEAR(model2.getNoiseFigure(), 5.0, 0.01);
    EXPECT_NEAR(model2.getSystemBandwidth(), 50.0, 0.01);
    EXPECT_TRUE(model2.getModulationType() == ReceiveModulationType::BPSK);
    EXPECT_TRUE(model2.getReceiverType() == ReceiverType::DIRECT_CONVERSION);
    
    std::cout << "✓ 构造函数和基本参数设置测试通过" << std::endl;
    return true;
}

// 测试参数设置和校验
bool testParameterValidation() {
    std::cout << "测试: 参数校验..." << std::endl;
    
    CommunicationReceiveModel model;
    
    // 测试接收灵敏度范围
    EXPECT_TRUE(model.setReceiveSensitivity(-100.0));
    EXPECT_TRUE(model.setReceiveSensitivity(-150.0));
    EXPECT_TRUE(model.setReceiveSensitivity(-30.0));
    EXPECT_FALSE(model.setReceiveSensitivity(-160.0)); // 超出下限
    EXPECT_FALSE(model.setReceiveSensitivity(-20.0));  // 超出上限
    
    // 测试噪声系数范围
    EXPECT_TRUE(model.setNoiseFigure(0.0));
    EXPECT_TRUE(model.setNoiseFigure(10.0));
    EXPECT_TRUE(model.setNoiseFigure(20.0));
    EXPECT_FALSE(model.setNoiseFigure(-1.0)); // 超出下限
    EXPECT_FALSE(model.setNoiseFigure(25.0)); // 超出上限
    
    // 测试带宽范围
    EXPECT_TRUE(model.setSystemBandwidth(0.1));
    EXPECT_TRUE(model.setSystemBandwidth(1000.0));
    EXPECT_TRUE(model.setSystemBandwidth(10000.0));
    EXPECT_FALSE(model.setSystemBandwidth(0.05)); // 超出下限
    EXPECT_FALSE(model.setSystemBandwidth(15000.0)); // 超出上限
    
    // 测试温度范围
    EXPECT_TRUE(model.setAmbientTemperature(290.0));
    EXPECT_TRUE(model.setAmbientTemperature(200.0));
    EXPECT_TRUE(model.setAmbientTemperature(400.0));
    EXPECT_FALSE(model.setAmbientTemperature(150.0)); // 超出下限
    EXPECT_FALSE(model.setAmbientTemperature(450.0)); // 超出上限
    
    // 测试天线增益范围
    EXPECT_TRUE(model.setAntennaGain(0.0));
    EXPECT_TRUE(model.setAntennaGain(-20.0));
    EXPECT_TRUE(model.setAntennaGain(50.0));
    EXPECT_FALSE(model.setAntennaGain(-25.0)); // 超出下限
    EXPECT_FALSE(model.setAntennaGain(55.0));  // 超出上限
    
    std::cout << "✓ 参数校验测试通过" << std::endl;
    return true;
}

// 测试信噪比计算
bool testSNRCalculation() {
    std::cout << "测试: 信噪比计算..." << std::endl;
    
    CommunicationReceiveModel model(-100.0, 3.0, 25.0);
    
    // 设置接收功率并测试SNR计算
    model.setReceivedPower(-80.0);
    double snr = model.calculateSignalToNoiseRatio();
    
    // 验证SNR计算的合理性
    EXPECT_GT(snr, 0.0); // SNR应该为正值
    EXPECT_LT(snr, 50.0); // SNR不应该过大
    
    // 测试不同接收功率下的SNR变化
    model.setReceivedPower(-90.0);
    double snr1 = model.calculateSignalToNoiseRatio();
    
    model.setReceivedPower(-70.0);
    double snr2 = model.calculateSignalToNoiseRatio();
    
    EXPECT_GT(snr2, snr1); // 更高的接收功率应该有更高的SNR
    
    std::cout << "✓ 信噪比计算测试通过" << std::endl;
    return true;
}

// 测试误码率计算
bool testBERCalculation() {
    std::cout << "测试: 误码率计算..." << std::endl;
    
    CommunicationReceiveModel model(-100.0, 3.0, 25.0);
    
    // 测试不同调制方式的误码率
    std::vector<ReceiveModulationType> modTypes = {
        ReceiveModulationType::BPSK,
        ReceiveModulationType::QPSK,
        ReceiveModulationType::QAM16,
        ReceiveModulationType::FM,
        ReceiveModulationType::AM
    };
    
    for (auto modType : modTypes) {
        model.setModulationType(modType);
        model.setReceivedPower(-80.0);
        
        double ber = model.calculateBitErrorRate();
        
        // 误码率应该在合理范围内
        EXPECT_GT(ber, 0.0);
        EXPECT_LT(ber, 1.0);
        
        // 测试SNR提高时误码率应该降低
        model.setReceivedPower(-70.0);
        double ber_high_snr = model.calculateBitErrorRate();
        
        EXPECT_LT(ber_high_snr, ber); // 高SNR时误码率应该更低
    }
    
    std::cout << "✓ 误码率计算测试通过" << std::endl;
    return true;
}

// 测试接收性能评估
bool testReceivePerformanceEvaluation() {
    std::cout << "测试: 接收性能评估..." << std::endl;
    
    CommunicationReceiveModel model(-100.0, 3.0, 25.0);
    
    // 测试信号检测能力
    model.setReceivedPower(-120.0); // 低功率
    EXPECT_FALSE(model.isSignalDetectable());
    
    model.setReceivedPower(-80.0); // 高功率
    EXPECT_TRUE(model.isSignalDetectable());
    
    // 测试信号解码能力
    model.setReceivedPower(-90.0);
    bool decodable_low_req = model.isSignalDecodable(5.0); // 低要求
    bool decodable_high_req = model.isSignalDecodable(20.0); // 高要求
    
    // 低要求应该更容易满足
    if (decodable_high_req) {
        EXPECT_TRUE(decodable_low_req);
    }
    
    // 测试接收余量计算
    double margin = model.calculateReceiveMargin();
    EXPECT_GT(margin, -50.0); // 余量应该在合理范围内
    EXPECT_LT(margin, 50.0);
    
    std::cout << "✓ 接收性能评估测试通过" << std::endl;
    return true;
}

// 测试所需SNR计算
bool testRequiredSNRCalculation() {
    std::cout << "测试: 所需SNR计算..." << std::endl;
    
    CommunicationReceiveModel model;
    
    std::vector<ReceiveModulationType> modTypes = {
        ReceiveModulationType::BPSK,
        ReceiveModulationType::QPSK,
        ReceiveModulationType::QAM16,
        ReceiveModulationType::FM,
        ReceiveModulationType::AM
    };
    
    for (auto modType : modTypes) {
        model.setModulationType(modType);
        
        double snr_1e6 = model.getRequiredSNRForBER(1e-6);
        double snr_1e4 = model.getRequiredSNRForBER(1e-4);
        double snr_1e2 = model.getRequiredSNRForBER(1e-2);
        
        // 更低的误码率要求应该需要更高的SNR
        EXPECT_GT(snr_1e6, snr_1e4);
        EXPECT_GT(snr_1e4, snr_1e2);
        
        // SNR值应该在合理范围内
        EXPECT_GT(snr_1e6, 0.0);
        EXPECT_LT(snr_1e6, 30.0);
    }
    
    std::cout << "✓ 所需SNR计算测试通过" << std::endl;
    return true;
}

// 测试信息输出
bool testInformationOutput() {
    std::cout << "测试: 信息输出..." << std::endl;
    
    CommunicationReceiveModel model(-90.0, 5.0, 50.0, 
                                   ReceiveModulationType::QPSK, 
                                   ReceiverType::SOFTWARE_DEFINED);
    model.setReceivedPower(-70.0);
    
    std::string paramInfo = model.getParameterInfo();
    std::string perfInfo = model.getPerformanceInfo();
    
    // 检查信息字符串不为空
    EXPECT_TRUE(!paramInfo.empty());
    EXPECT_TRUE(!perfInfo.empty());
    
    // 检查包含关键信息
    EXPECT_TRUE(paramInfo.find("接收灵敏度") != std::string::npos);
    EXPECT_TRUE(paramInfo.find("噪声系数") != std::string::npos);
    EXPECT_TRUE(paramInfo.find("QPSK") != std::string::npos);
    
    EXPECT_TRUE(perfInfo.find("信噪比") != std::string::npos);
    EXPECT_TRUE(perfInfo.find("误码率") != std::string::npos);
    
    std::cout << "✓ 信息输出测试通过" << std::endl;
    return true;
}

// 性能测试
bool testPerformance() {
    std::cout << "测试: 性能测试..." << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // 执行大量计算
    for (int i = 0; i < 10000; ++i) {
        CommunicationReceiveModel model(-100.0 + i * 0.001, 3.0, 25.0);
        model.setReceivedPower(-80.0 + i * 0.001);
        
        double snr = model.calculateSignalToNoiseRatio();
        double ber = model.calculateBitErrorRate();
        bool detectable = model.isSignalDetectable();
        
        // 避免编译器优化掉计算
        (void)snr; (void)ber; (void)detectable;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "10000次计算耗时: " << duration.count() << "ms" << std::endl;
    EXPECT_LT(duration.count(), 1000); // 应该在1秒内完成
    
    std::cout << "✓ 性能测试通过" << std::endl;
    return true;
}

// 运行所有测试
int main() {
    std::cout << "=== 通信接收模型单元测试 ===" << std::endl;
    
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
        {"信噪比计算", testSNRCalculation},
        {"误码率计算", testBERCalculation},
        {"接收性能评估", testReceivePerformanceEvaluation},
        {"所需SNR计算", testRequiredSNRCalculation},
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