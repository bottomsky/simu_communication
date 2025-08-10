#include "header/CommunicationReceiveModel.h"
#include <iostream>
#include <iomanip>

int main() {
    try {
        std::cout << "=== 环境温度默认参数使用示例 ===" << std::endl;
        std::cout << std::fixed << std::setprecision(2);
        
        // 方法一：使用默认环境温度290K
        std::cout << "\n1. 使用默认环境温度290K：" << std::endl;
        CommunicationReceiveModel receiver1;
        std::cout << "默认环境温度: " << receiver1.getAmbientTemperature() << " K" << std::endl;
        
        // 方法二：构造时指定其他参数，环境温度使用默认值
        std::cout << "\n2. 指定部分参数，环境温度使用默认值：" << std::endl;
        CommunicationReceiveModel receiver2(-105.0, 2.5, 20.0, 
            ReceiveModulationType::BPSK, ReceiverType::SUPERHETERODYNE);
        std::cout << "环境温度: " << receiver2.getAmbientTemperature() << " K" << std::endl;
        
        // 方法三：构造时指定环境温度
        std::cout << "\n3. 构造时指定环境温度：" << std::endl;
        CommunicationReceiveModel receiver3(-100.0, 3.0, 25.0, 
            ReceiveModulationType::QPSK, ReceiverType::SUPERHETERODYNE, 
            300.0);  // 指定环境温度为300K
        std::cout << "指定环境温度: " << receiver3.getAmbientTemperature() << " K" << std::endl;
        
        // 方法四：运行时动态设置环境温度
        std::cout << "\n4. 运行时动态设置环境温度：" << std::endl;
        CommunicationReceiveModel receiver4;
        std::cout << "初始环境温度: " << receiver4.getAmbientTemperature() << " K" << std::endl;
        
        bool success = receiver4.setAmbientTemperature(280.0);
        if (success) {
            std::cout << "设置后环境温度: " << receiver4.getAmbientTemperature() << " K" << std::endl;
        }
        
        // 测试不同环境温度对噪声性能的影响
        std::cout << "\n=== 环境温度对噪声性能的影响 ===" << std::endl;
        double test_temperatures[] = {250.0, 290.0, 310.0, 350.0};
        
        for (double temp : test_temperatures) {
            CommunicationReceiveModel receiver(-100.0, 3.0, 25.0, 
                ReceiveModulationType::BPSK, ReceiverType::SUPERHETERODYNE, temp);
            
            receiver.setReceivedPower(-95.0);  // 设置接收功率
            
            double snr = receiver.calculateSignalToNoiseRatio();
            double noise_power = receiver.calculateEffectiveNoisePower();
            double min_detectable = receiver.calculateMinimumDetectablePower();
            
            std::cout << "温度: " << temp << " K" << std::endl;
            std::cout << "  有效噪声功率: " << noise_power << " dBm" << std::endl;
            std::cout << "  信噪比: " << snr << " dB" << std::endl;
            std::cout << "  最小可检测功率: " << min_detectable << " dBm" << std::endl;
            std::cout << std::endl;
        }
        
        // 测试边界值
        std::cout << "=== 环境温度边界值测试 ===" << std::endl;
        CommunicationReceiveModel receiver_test;
        
        // 测试有效温度范围
        double valid_temps[] = {200.0, 290.0, 400.0};
        double invalid_temps[] = {100.0, 500.0};
        
        std::cout << "有效温度测试：" << std::endl;
        for (double temp : valid_temps) {
            bool result = receiver_test.setAmbientTemperature(temp);
            std::cout << "  " << temp << " K: " << (result ? "✓ 有效" : "✗ 无效") << std::endl;
        }
        
        std::cout << "无效温度测试：" << std::endl;
        for (double temp : invalid_temps) {
            bool result = receiver_test.setAmbientTemperature(temp);
            std::cout << "  " << temp << " K: " << (result ? "✓ 有效" : "✗ 无效") << std::endl;
        }
        
        // 显示参数信息
        std::cout << "\n=== 完整参数信息 ===" << std::endl;
        std::cout << receiver1.getParameterInfo() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}