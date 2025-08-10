#include "header/CommunicationReceiveModel.h"
#include <iostream>
#include <iomanip>

int main() {
    try {
        std::cout << "=== isSignalDecodable 方法重载功能测试 ===" << std::endl;
        std::cout << std::fixed << std::setprecision(2);
        
        // 测试不同调制方式
        ReceiveModulationType modTypes[] = {
            ReceiveModulationType::BPSK,
            ReceiveModulationType::QPSK,
            ReceiveModulationType::QAM16,
            ReceiveModulationType::FM,
            ReceiveModulationType::AM
        };
        
        std::string modNames[] = {"BPSK", "QPSK", "16QAM", "FM", "AM"};
        
        for (int i = 0; i < 5; i++) {
            std::cout << "\n--- 调制方式: " << modNames[i] << " ---" << std::endl;
            
            // 创建接收模型实例
            CommunicationReceiveModel receiver(
                -100.0,  // 接收灵敏度 -100dBm
                3.0,     // 噪声系数 3dB
                25.0,    // 系统带宽 25kHz
                modTypes[i],
                ReceiverType::SUPERHETERODYNE,
                290.0,   // 环境温度 290K
                0.0      // 天线增益 0dBi
            );
            
            // 获取该调制方式在1e-6误码率下的SNR要求
            double required_snr_1e6 = receiver.getRequiredSNRForBER(1e-6);
            std::cout << "1e-6误码率所需SNR: " << required_snr_1e6 << " dB" << std::endl;
            
            // 测试不同的接收功率
            double test_powers[] = {-90.0, -95.0, -100.0, -105.0, -110.0};
            
            for (double power : test_powers) {
                receiver.setReceivedPower(power);
                double current_snr = receiver.calculateSignalToNoiseRatio();
                
                // 测试两种调用方式
                bool decodable_auto = receiver.isSignalDecodable();  // 无参版本（自动选择SNR要求）
                bool decodable_manual = receiver.isSignalDecodable(required_snr_1e6);  // 有参版本（手动指定SNR要求）
                bool decodable_10db = receiver.isSignalDecodable(10.0);  // 有参版本（固定10dB要求）
                
                std::cout << "  功率: " << power << " dBm, SNR: " << current_snr << " dB" << std::endl;
                std::cout << "    无参版本(自动): " << (decodable_auto ? "可解码" : "不可解码") << std::endl;
                std::cout << "    有参版本(1e-6): " << (decodable_manual ? "可解码" : "不可解码") << std::endl;
                std::cout << "    有参版本(10dB): " << (decodable_10db ? "可解码" : "不可解码") << std::endl;
                
                // 验证无参版本和手动指定1e-6误码率SNR的版本结果应该一致
                if (decodable_auto == decodable_manual) {
                    std::cout << "    ✓ 无参版本与1e-6误码率版本结果一致" << std::endl;
                } else {
                    std::cout << "    ✗ 无参版本与1e-6误码率版本结果不一致！" << std::endl;
                }
            }
        }
        
        // 测试边界情况
        std::cout << "\n=== 边界情况测试 ===" << std::endl;
        CommunicationReceiveModel receiver(
            -100.0, 3.0, 25.0, ReceiveModulationType::BPSK,
            ReceiverType::SUPERHETERODYNE, 290.0, 0.0
        );
        
        // 设置一个刚好满足BPSK 1e-6误码率要求的功率
        double required_snr = receiver.getRequiredSNRForBER(1e-6);
        double noise_floor = receiver.calculateEffectiveNoisePower();
        double target_power = noise_floor + required_snr;
        
        receiver.setReceivedPower(target_power);
        
        std::cout << "噪声底: " << noise_floor << " dBm" << std::endl;
        std::cout << "BPSK 1e-6误码率所需SNR: " << required_snr << " dB" << std::endl;
        std::cout << "目标接收功率: " << target_power << " dBm" << std::endl;
        std::cout << "实际SNR: " << receiver.calculateSignalToNoiseRatio() << " dB" << std::endl;
        std::cout << "无参版本结果: " << (receiver.isSignalDecodable() ? "可解码" : "不可解码") << std::endl;
        std::cout << "有参版本结果: " << (receiver.isSignalDecodable(required_snr) ? "可解码" : "不可解码") << std::endl;
        
        // 显示完整性能信息
        std::cout << "\n" << receiver.getPerformanceInfo() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}