#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "SignalTransmissionModel.h"
#include "CommunicationDistanceModel.h"

void printSeparator(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}

void demonstrateSignalTransmissionModel() {
    printSeparator("信号传输模型演示");
    
    try {
        // 创建一个超短波FM信号传输模型
        SignalTransmissionModel signalModel(
            FrequencyBand::ULTRA_SHORT_WAVE,
            100000.0,  // 100MHz
            ModulationType::FM,
            25.0,      // 25kHz带宽
            20.0       // 20W功率
        );
        
        std::cout << "✓ 成功创建信号传输模型" << std::endl;
        std::cout << signalModel.getParameterInfo() << std::endl;
        
        // 演示参数修改
        std::cout << "\n--- 修改信号参数 ---" << std::endl;
        
        // 修改调制方式
        signalModel.setModulationType(ModulationType::BPSK);
        std::cout << "✓ 调制方式已改为BPSK" << std::endl;
        
        // 修改功率
        if (signalModel.setTransmitPower(50.0)) {
            std::cout << "✓ 发射功率已调整为50W" << std::endl;
        }
        
        // 修改带宽
        if (signalModel.setSignalBandwidth(50.0)) {
            std::cout << "✓ 信号带宽已调整为50kHz" << std::endl;
        }
        
        std::cout << "\n--- 修改后的参数 ---" << std::endl;
        std::cout << signalModel.getParameterInfo() << std::endl;
        
        // 演示频段切换
        std::cout << "\n--- 频段切换演示 ---" << std::endl;
        std::cout << "当前中心频率: " << signalModel.getCenterFrequency() << " kHz" << std::endl;
        
        signalModel.setFrequencyBand(FrequencyBand::MICROWAVE);
        std::cout << "切换到微波频段后，中心频率自动调整为: " 
                  << signalModel.getCenterFrequency() << " kHz" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ 错误: " << e.what() << std::endl;
    }
}

void demonstrateCommunicationDistanceModel() {
    printSeparator("通信距离模型演示");
    
    try {
        // 创建通信距离模型
        CommunicationDistanceModel distanceModel(
            50.0,      // 50km最大视距
            EnvironmentType::URBAN_AREA,
            2.0,       // 环境衰减系数
            -100.0,    // -100dBm接收灵敏度
            10.0,      // 10dB链路余量
            20.0       // 20dBm发射功率
        );
        
        std::cout << "✓ 成功创建通信距离模型" << std::endl;
        std::cout << distanceModel.getParameterInfo() << std::endl;
        
        // 计算有效通信距离
        double distance = distanceModel.calculateEffectiveDistance();
        std::cout << "\n--- 通信距离计算结果 ---" << std::endl;
        std::cout << "有效通信距离: " << std::fixed << std::setprecision(2) 
                  << distance << " km" << std::endl;
        
        // 演示不同环境下的通信距离
        std::cout << "\n--- 不同环境下的通信距离对比 ---" << std::endl;
        
        struct EnvironmentTest {
            EnvironmentType env;
            std::string name;
        };
        
        std::vector<EnvironmentTest> environments = {
            {EnvironmentType::OPEN_FIELD, "开阔地"},
            {EnvironmentType::URBAN_AREA, "城市区域"},
            {EnvironmentType::MOUNTAINOUS, "山区"}
        };
        
        for (const auto& env : environments) {
            distanceModel.setEnvironmentType(env.env);
            double envDistance = distanceModel.calculateEffectiveDistance();
            std::cout << std::setw(12) << env.name << ": " 
                      << std::fixed << std::setprecision(2) 
                      << envDistance << " km" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "❌ 错误: " << e.what() << std::endl;
    }
}

void demonstrateIntegratedUsage() {
    printSeparator("集成使用演示");
    
    try {
        // 创建信号传输模型
        SignalTransmissionModel signalModel(
            FrequencyBand::ULTRA_SHORT_WAVE,
            150000.0,  // 150MHz
            ModulationType::FM,
            25.0,
            30.0
        );
        
        // 创建对应的通信距离模型
        CommunicationDistanceModel distanceModel(
            30.0,      // 最大视距
            EnvironmentType::OPEN_FIELD,
            1.0,       // 环境衰减系数
            -110.0,    // 接收灵敏度
            15.0,      // 链路余量
            30.0       // 发射功率(dBm)
        );
        
        std::cout << "✓ 创建了配套的信号传输和通信距离模型" << std::endl;
        
        // 演示参数同步
        std::cout << "\n--- 参数同步演示 ---" << std::endl;
        std::cout << "初始配置:" << std::endl;
        std::cout << "  信号功率: " << signalModel.getTransmitPower() << " W" << std::endl;
        std::cout << "  通信距离: " << std::fixed << std::setprecision(2) 
                  << distanceModel.calculateEffectiveDistance() << " km" << std::endl;
        
        // 修改信号功率并同步
        signalModel.setTransmitPower(60.0);
        distanceModel.setTransmitPower(35.0);  // 对应的dBm值
        
        std::cout << "\n功率调整后:" << std::endl;
        std::cout << "  信号功率: " << signalModel.getTransmitPower() << " W" << std::endl;
        std::cout << "  通信距离: " << std::fixed << std::setprecision(2) 
                  << distanceModel.calculateEffectiveDistance() << " km" << std::endl;
        
        // 演示频率对通信距离的影响
        std::cout << "\n--- 频率影响演示 ---" << std::endl;
        
        std::vector<double> frequencies = {50000.0, 100000.0, 200000.0};
        std::vector<std::string> freqNames = {"50MHz", "100MHz", "200MHz"};
        
        for (size_t i = 0; i < frequencies.size(); ++i) {
            if (signalModel.setCenterFrequency(frequencies[i])) {
                CommunicationDistanceModel tempModel(
                    30.0,      // 最大视距
                    EnvironmentType::OPEN_FIELD,
                    1.0,       // 环境衰减系数
                    -110.0,    // 接收灵敏度
                    15.0,      // 链路余量
                    30.0       // 发射功率(dBm)
                );
                
                double dist = tempModel.calculateEffectiveDistance();
                std::cout << freqNames[i] << " 频率下的通信距离: " 
                          << std::fixed << std::setprecision(2) << dist << " km" << std::endl;
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "❌ 错误: " << e.what() << std::endl;
    }
}

void demonstrateErrorHandling() {
    printSeparator("错误处理演示");
    
    std::cout << "演示各种错误情况的处理..." << std::endl;
    
    // 1. 构造函数参数错误
    std::cout << "\n1. 构造函数参数错误:" << std::endl;
    try {
        SignalTransmissionModel invalidModel(
            FrequencyBand::SHORT_WAVE,
            500000.0,  // 频率超出短波范围
            ModulationType::AM,
            10.0,
            5.0
        );
        std::cout << "❌ 应该抛出异常但没有" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✓ 正确捕获异常: " << e.what() << std::endl;
    }
    
    // 2. 参数设置错误
    std::cout << "\n2. 参数设置错误:" << std::endl;
    try {
        SignalTransmissionModel model(
            FrequencyBand::ULTRA_SHORT_WAVE,
            100000.0,
            ModulationType::FM,
            25.0,
            20.0
        );
        
        // 尝试设置无效功率
        bool result = model.setTransmitPower(-10.0);
        std::cout << "设置负功率结果: " << (result ? "成功" : "失败") << std::endl;
        std::cout << "当前功率: " << model.getTransmitPower() << " W (应该保持原值)" << std::endl;
        
        // 尝试设置超出范围的频率
        result = model.setCenterFrequency(500000.0);
        std::cout << "设置超范围频率结果: " << (result ? "成功" : "失败") << std::endl;
        std::cout << "当前频率: " << model.getCenterFrequency() << " kHz (应该保持原值)" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "异常: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "通信对抗模型基础使用示例" << std::endl;
    std::cout << "版本: 1.0.0" << std::endl;
    std::cout << "构建时间: " << __DATE__ << " " << __TIME__ << std::endl;
    
    // 演示各个功能模块
    demonstrateSignalTransmissionModel();
    demonstrateCommunicationDistanceModel();
    demonstrateIntegratedUsage();
    demonstrateErrorHandling();
    
    printSeparator("演示完成");
    std::cout << "✓ 所有基础功能演示完成" << std::endl;
    std::cout << "✓ 系统运行正常" << std::endl;
    
    return 0;
}