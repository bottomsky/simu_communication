#include "CommunicationReceiveParameterConfig.h"
#include "CommunicationReceiveModel.h"
#include <iostream>
#include <iomanip>

int main() {
    std::cout << "=== 通信接收模型参数配置测试 ===" << std::endl;
    
    // 1. 测试参数范围信息获取
    std::cout << "\n1. 参数范围配置:" << std::endl;
    std::cout << CommunicationReceiveParameterConfig::getParameterRangeInfo() << std::endl;
    
    // 2. 测试各项参数校验
    std::cout << "\n2. 参数校验测试:" << std::endl;
    
    // 接收灵敏度校验
    std::cout << "接收灵敏度校验:" << std::endl;
    std::cout << "  -100.0 dBm: " << (CommunicationReceiveParameterConfig::isReceiveSensitivityValid(-100.0) ? "有效" : "无效") << std::endl;
    std::cout << "  -160.0 dBm: " << (CommunicationReceiveParameterConfig::isReceiveSensitivityValid(-160.0) ? "有效" : "无效") << std::endl;
    std::cout << "  -20.0 dBm: " << (CommunicationReceiveParameterConfig::isReceiveSensitivityValid(-20.0) ? "有效" : "无效") << std::endl;
    
    // 噪声系数校验
    std::cout << "噪声系数校验:" << std::endl;
    std::cout << "  3.0 dB: " << (CommunicationReceiveParameterConfig::isNoiseFigureValid(3.0) ? "有效" : "无效") << std::endl;
    std::cout << "  -1.0 dB: " << (CommunicationReceiveParameterConfig::isNoiseFigureValid(-1.0) ? "有效" : "无效") << std::endl;
    std::cout << "  25.0 dB: " << (CommunicationReceiveParameterConfig::isNoiseFigureValid(25.0) ? "有效" : "无效") << std::endl;
    
    // 系统带宽校验
    std::cout << "系统带宽校验:" << std::endl;
    std::cout << "  25.0 MHz: " << (CommunicationReceiveParameterConfig::isSystemBandwidthValid(25.0) ? "有效" : "无效") << std::endl;
    std::cout << "  0.05 MHz: " << (CommunicationReceiveParameterConfig::isSystemBandwidthValid(0.05) ? "有效" : "无效") << std::endl;
    std::cout << "  15000.0 MHz: " << (CommunicationReceiveParameterConfig::isSystemBandwidthValid(15000.0) ? "有效" : "无效") << std::endl;
    
    // 环境温度校验
    std::cout << "环境温度校验:" << std::endl;
    std::cout << "  290.0 K: " << (CommunicationReceiveParameterConfig::isAmbientTemperatureValid(290.0) ? "有效" : "无效") << std::endl;
    std::cout << "  150.0 K: " << (CommunicationReceiveParameterConfig::isAmbientTemperatureValid(150.0) ? "有效" : "无效") << std::endl;
    std::cout << "  450.0 K: " << (CommunicationReceiveParameterConfig::isAmbientTemperatureValid(450.0) ? "有效" : "无效") << std::endl;
    
    // 天线增益校验
    std::cout << "天线增益校验:" << std::endl;
    std::cout << "  10.0 dBi: " << (CommunicationReceiveParameterConfig::isAntennaGainValid(10.0) ? "有效" : "无效") << std::endl;
    std::cout << "  -25.0 dBi: " << (CommunicationReceiveParameterConfig::isAntennaGainValid(-25.0) ? "有效" : "无效") << std::endl;
    std::cout << "  55.0 dBi: " << (CommunicationReceiveParameterConfig::isAntennaGainValid(55.0) ? "有效" : "无效") << std::endl;
    
    // 接收功率校验
    std::cout << "接收功率校验:" << std::endl;
    std::cout << "  -80.0 dBm: " << (CommunicationReceiveParameterConfig::isReceivedPowerValid(-80.0) ? "有效" : "无效") << std::endl;
    std::cout << "  -200.0 dBm: " << (CommunicationReceiveParameterConfig::isReceivedPowerValid(-200.0) ? "有效" : "无效") << std::endl;
    std::cout << "  50.0 dBm: " << (CommunicationReceiveParameterConfig::isReceivedPowerValid(50.0) ? "有效" : "无效") << std::endl;
    
    // 3. 测试 CommunicationReceiveModel 集成
    std::cout << "\n3. CommunicationReceiveModel 集成测试:" << std::endl;
    
    try {
        // 使用有效参数创建模型
        CommunicationReceiveModel model(-100.0, 3.0, 25.0);
        std::cout << "✓ 使用有效参数创建模型成功" << std::endl;
        
        // 测试参数设置
        bool result1 = model.setReceiveSensitivity(-90.0);
        bool result2 = model.setNoiseFigure(5.0);
        bool result3 = model.setSystemBandwidth(50.0);
        
        std::cout << "✓ 参数设置测试: 灵敏度=" << (result1 ? "成功" : "失败") 
                  << ", 噪声系数=" << (result2 ? "成功" : "失败")
                  << ", 带宽=" << (result3 ? "成功" : "失败") << std::endl;
        
        // 测试无效参数
        bool invalid1 = model.setReceiveSensitivity(-160.0); // 超出范围
        bool invalid2 = model.setNoiseFigure(-1.0);          // 超出范围
        bool invalid3 = model.setSystemBandwidth(15000.0);   // 超出范围
        
        std::cout << "✓ 无效参数测试: 灵敏度=" << (invalid1 ? "意外成功" : "正确拒绝") 
                  << ", 噪声系数=" << (invalid2 ? "意外成功" : "正确拒绝")
                  << ", 带宽=" << (invalid3 ? "意外成功" : "正确拒绝") << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "✗ 模型创建失败: " << e.what() << std::endl;
    }
    
    std::cout << "\n=== 测试完成 ===" << std::endl;
    return 0;
}