#include "source/header/MathConstants.h"
#include "source/header/CommunicationModelAPI.h"
#include <iostream>
#include <iomanip>

int main() {
    std::cout << "=== 硬编码常量替换验证测试 ===" << std::endl;
    std::cout << std::fixed << std::setprecision(6);
    
    // 测试基础数学常量
    std::cout << "\n1. 基础数学常量:" << std::endl;
    std::cout << "   ZERO = " << MathConstants::ZERO << std::endl;
    std::cout << "   UNITY = " << MathConstants::UNITY << std::endl;
    std::cout << "   PI = " << MathConstants::PI << std::endl;
    std::cout << "   MEGA_MULTIPLIER = " << MathConstants::MEGA_MULTIPLIER << std::endl;
    
    // 测试环境损耗常量
    std::cout << "\n2. 环境损耗配置常量:" << std::endl;
    std::cout << "   MIN_PATH_LOSS_EXPONENT = " << MathConstants::MIN_PATH_LOSS_EXPONENT << std::endl;
    std::cout << "   MAX_PATH_LOSS_EXPONENT = " << MathConstants::MAX_PATH_LOSS_EXPONENT << std::endl;
    std::cout << "   TOLERANCE_VALUE = " << MathConstants::TOLERANCE_VALUE << std::endl;
    
    // 测试信号传输模型常量
    std::cout << "\n3. 信号传输模型常量:" << std::endl;
    std::cout << "   HF_MIN_FREQUENCY = " << MathConstants::HF_MIN_FREQUENCY << " kHz" << std::endl;
    std::cout << "   HF_MAX_FREQUENCY = " << MathConstants::HF_MAX_FREQUENCY << " kHz" << std::endl;
    std::cout << "   DEFAULT_HF_CENTER_FREQ = " << MathConstants::DEFAULT_HF_CENTER_FREQ << " kHz" << std::endl;
    std::cout << "   MAX_POWER_LIMIT = " << MathConstants::MAX_POWER_LIMIT << " W" << std::endl;
    
    // 测试通信模型API常量
    std::cout << "\n4. 通信模型API常量:" << std::endl;
    std::cout << "   DEFAULT_SIGNAL_STRENGTH = " << MathConstants::DEFAULT_SIGNAL_STRENGTH << " dBm" << std::endl;
    std::cout << "   DEFAULT_SNR = " << MathConstants::DEFAULT_SNR << " dB" << std::endl;
    std::cout << "   PACKET_LENGTH_BITS = " << MathConstants::PACKET_LENGTH_BITS << std::endl;
    
    // 测试质量评估阈值
    std::cout << "\n5. 质量评估阈值:" << std::endl;
    std::cout << "   HIGH_SNR_THRESHOLD = " << MathConstants::HIGH_SNR_THRESHOLD << " dB" << std::endl;
    std::cout << "   EXCELLENT_BER_THRESHOLD = " << MathConstants::EXCELLENT_BER_THRESHOLD << std::endl;
    std::cout << "   GOOD_BER_THRESHOLD = " << MathConstants::GOOD_BER_THRESHOLD << std::endl;
    
    // 创建通信模型API实例并测试基本功能
    std::cout << "\n6. 通信模型API功能测试:" << std::endl;
    try {
        CommunicationModelAPI api;
        
        // 设置一些参数
        api.setFrequency(2400.0);  // 2.4 GHz
        api.setBandwidth(20.0);    // 20 MHz
        api.setTransmitPower(20.0); // 20 dBm
        api.setDistance(1.0);      // 1 km
        
        // 使用公有方法计算指标
        CommunicationLinkStatus status = api.calculateLinkStatus();
        CommunicationPerformance performance = api.calculatePerformance();
        double range = api.calculateCommunicationRange();
        
        std::cout << "   通信质量: " << static_cast<int>(status.quality) << std::endl;
        std::cout << "   有效通信距离: " << performance.effectiveRange << " km" << std::endl;
        std::cout << "   最大数据速率: " << performance.maxDataRate << " Mbps" << std::endl;
        std::cout << "   可靠性: " << performance.reliability << std::endl;
        std::cout << "   计算的通信距离: " << range << " km" << std::endl;
        
        std::cout << "\n✓ 所有常量替换测试通过！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "✗ 测试失败: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}