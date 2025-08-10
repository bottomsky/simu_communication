#include "header/CommunicationReceiveModel.h"
#include <iostream>
#include <iomanip>

int main() {
    try {
        // 创建接收模型实例
        CommunicationReceiveModel receiver(
            -100.0,  // 接收灵敏度 -100dBm
            3.0,     // 噪声系数 3dB
            25.0,    // 系统带宽 25kHz
            ReceiveModulationType::BPSK,
            ReceiverType::SUPERHETERODYNE,
            290.0,   // 环境温度 290K
            0.0      // 天线增益 0dBi
        );
        
        // 设置接收功率
        receiver.setReceivedPower(-95.0);
        
        std::cout << "=== 检测门限可调整功能演示 ===" << std::endl;
        std::cout << std::fixed << std::setprecision(2);
        
        // 测试不同的检测门限值
        double thresholds[] = {5.0, 10.0, 15.0, 20.0};
        
        for (double threshold : thresholds) {
            std::cout << "\n--- 检测门限: " << threshold << " dB ---" << std::endl;
            
            // 设置检测门限
            if (receiver.setDetectionThreshold(threshold)) {
                std::cout << "检测门限设置成功: " << receiver.getDetectionThreshold() << " dB" << std::endl;
                
                // 计算相关参数
                double minDetectablePower = receiver.calculateMinimumDetectablePower();
                bool isDetectable = receiver.isSignalDetectable();
                
                std::cout << "最小可检测功率: " << minDetectablePower << " dBm" << std::endl;
                std::cout << "当前接收功率: " << receiver.getReceivedPower() << " dBm" << std::endl;
                std::cout << "信号可检测: " << (isDetectable ? "是" : "否") << std::endl;
                
                if (isDetectable) {
                    double margin = receiver.getReceivedPower() - minDetectablePower;
                    std::cout << "检测余量: " << margin << " dB" << std::endl;
                }
            } else {
                std::cout << "检测门限设置失败！" << std::endl;
            }
        }
        
        // 测试边界值
        std::cout << "\n=== 边界值测试 ===" << std::endl;
        
        // 测试过低的门限值
        if (!receiver.setDetectionThreshold(-1.0)) {
            std::cout << "✓ 正确拒绝了过低的检测门限 (-1.0 dB)" << std::endl;
        }
        
        // 测试过高的门限值
        if (!receiver.setDetectionThreshold(35.0)) {
            std::cout << "✓ 正确拒绝了过高的检测门限 (35.0 dB)" << std::endl;
        }
        
        // 测试边界值
        if (receiver.setDetectionThreshold(0.0)) {
            std::cout << "✓ 成功设置最小检测门限 (0.0 dB)" << std::endl;
        }
        
        if (receiver.setDetectionThreshold(30.0)) {
            std::cout << "✓ 成功设置最大检测门限 (30.0 dB)" << std::endl;
        }
        
        // 显示完整参数信息
        std::cout << "\n" << receiver.getParameterInfo() << std::endl;
        std::cout << receiver.getPerformanceInfo() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}