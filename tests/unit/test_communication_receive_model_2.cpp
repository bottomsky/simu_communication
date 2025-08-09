#include "../source/header/CommunicationReceiveModel.h"
#include <iostream>
#include <cassert>
#include <cmath>
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
    
    CommunicationReceiveModel model;
    
    // 验证默认值
    EXPECT_NEAR(model.getReceiveSensitivity(), -100.0, 0.1);
    EXPECT_NEAR(model.getNoiseFigure(), 3.0, 0.1);
    EXPECT_NEAR(model.getSystemBandwidth(), 25.0, 0.1);
    
    std::cout << "构造函数测试通过" << std::endl;
    return true;
}

// 测试参数设置和校验
bool testParameterSetting() {
    std::cout << "测试参数设置和校验..." << std::endl;
    
    CommunicationReceiveModel model;
    
    // 测试有效参数设置
    EXPECT_TRUE(model.setReceiveSensitivity(-90.0));
    EXPECT_NEAR(model.getReceiveSensitivity(), -90.0, 0.1);
    
    EXPECT_TRUE(model.setNoiseFigure(5.0));
    EXPECT_NEAR(model.getNoiseFigure(), 5.0, 0.1);
    
    EXPECT_TRUE(model.setSystemBandwidth(200.0));
    EXPECT_NEAR(model.getSystemBandwidth(), 200.0, 0.1);
    
    // 测试无效参数
    EXPECT_FALSE(model.setReceiveSensitivity(-200.0)); // 过低
    EXPECT_FALSE(model.setReceiveSensitivity(50.0));   // 过高
    EXPECT_FALSE(model.setNoiseFigure(-1.0));          // 负值
    EXPECT_FALSE(model.setNoiseFigure(100.0));         // 过高
    EXPECT_FALSE(model.setSystemBandwidth(0.0));       // 零值
    EXPECT_FALSE(model.setSystemBandwidth(100000.0));  // 过大
    
    std::cout << "参数设置和校验测试通过" << std::endl;
    return true;
}

// 测试信噪比计算
bool testSNRCalculation() {
    std::cout << "测试信噪比计算..." << std::endl;
    
    CommunicationReceiveModel model;
    model.setReceiveSensitivity(-90.0);
    model.setNoiseFigure(3.0);
    model.setSystemBandwidth(100.0);
    model.setReceivedPower(-80.0); // 设置接收信号功率-80dBm
    
    double snr = model.calculateSignalToNoiseRatio();
    
    // SNR应该为正值且合理
    EXPECT_GT(snr, 0.0);
    EXPECT_LT(snr, 50.0);
    
    std::cout << "SNR = " << snr << " dB" << std::endl;
    
    std::cout << "信噪比计算测试通过" << std::endl;
    return true;
}

// 测试误码率计算
bool testBERCalculation() {
    std::cout << "测试误码率计算..." << std::endl;
    
    CommunicationReceiveModel model;
    model.setModulationType(ReceiveModulationType::BPSK);
    model.setReceivedPower(-80.0); // 设置接收信号功率
    
    double ber = model.calculateBitErrorRate();
    
    // BER应该在合理范围内
    EXPECT_GE(ber, 0.0);
    EXPECT_LE(ber, 1.0);
    
    std::cout << "BER = " << ber << std::endl;
    
    std::cout << "误码率计算测试通过" << std::endl;
    return true;
}

// 测试接收性能评估
bool testReceivePerformanceAssessment() {
    std::cout << "测试接收性能评估..." << std::endl;
    
    CommunicationReceiveModel model;
    
    // 测试强信号
    model.setReceivedPower(-70.0);
    EXPECT_TRUE(model.isSignalDetectable());
    EXPECT_TRUE(model.isSignalDecodable());
    
    // 测试弱信号
    model.setReceivedPower(-130.0);
    EXPECT_FALSE(model.isSignalDetectable());
    EXPECT_FALSE(model.isSignalDecodable());
    
    // 测试接收余量
    model.setReceivedPower(-80.0);
    double margin = model.calculateReceiveMargin();
    EXPECT_GT(margin, -50.0); // 应该在合理范围内
    
    std::cout << "接收余量 = " << margin << " dB" << std::endl;
    
    std::cout << "接收性能评估测试通过" << std::endl;
    return true;
}

// 测试所需信噪比计算
bool testRequiredSNRCalculation() {
    std::cout << "测试所需信噪比计算..." << std::endl;
    
    CommunicationReceiveModel model;
    model.setModulationType(ReceiveModulationType::BPSK);
    
    double requiredSNR = model.getRequiredSNRForBER(1e-6); // 目标BER 1e-6
    
    // 所需SNR应该合理
    EXPECT_GT(requiredSNR, 5.0);
    EXPECT_LT(requiredSNR, 20.0);
    
    std::cout << "目标BER 1e-6所需SNR = " << requiredSNR << " dB" << std::endl;
    
    std::cout << "所需信噪比计算测试通过" << std::endl;
    return true;
}

// 测试信息输出
bool testInformationOutput() {
    std::cout << "测试信息输出..." << std::endl;
    
    CommunicationReceiveModel model;
    
    std::string paramInfo = model.getParameterInfo();
    std::string perfInfo = model.getPerformanceInfo();
    
    EXPECT_FALSE(paramInfo.empty());
    EXPECT_FALSE(perfInfo.empty());
    
    std::cout << "参数信息:" << std::endl << paramInfo << std::endl;
    std::cout << "性能信息:" << std::endl << perfInfo << std::endl;
    
    std::cout << "信息输出测试通过" << std::endl;
    return true;
}

// 性能测试
bool testPerformance() {
    std::cout << "测试性能..." << std::endl;
    
    CommunicationReceiveModel model;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // 设置接收功率
    model.setReceivedPower(-80.0);
    
    // 执行多次计算
    for (int i = 0; i < 10000; ++i) {
        model.calculateSignalToNoiseRatio();
        model.calculateBitErrorRate();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "10000次计算耗时: " << duration.count() << " 微秒" << std::endl;
    
    // 性能应该在合理范围内
    EXPECT_LT(duration.count(), 100000); // 小于100毫秒
    
    std::cout << "性能测试通过" << std::endl;
    return true;
}

// 主测试函数
int main() {
    std::cout << "开始通信接收模型测试..." << std::endl;
    
    bool allPassed = true;
    
    allPassed &= testConstructor();
    allPassed &= testParameterSetting();
    allPassed &= testSNRCalculation();
    allPassed &= testBERCalculation();
    allPassed &= testReceivePerformanceAssessment();
    allPassed &= testRequiredSNRCalculation();
    allPassed &= testInformationOutput();
    allPassed &= testPerformance();
    
    if (allPassed) {
        std::cout << "\n所有测试通过！" << std::endl;
        return 0;
    } else {
        std::cout << "\n部分测试失败！" << std::endl;
        return 1;
    }
}