#ifndef COMMUNICATION_RECEIVE_MODEL_H
#define COMMUNICATION_RECEIVE_MODEL_H

#include <string>
#include <stdexcept>
#include <cmath>

// 调制方式枚举（与SignalTransmissionModel保持一致）
enum class ReceiveModulationType {
    AM,     // 调幅
    FM,     // 调频
    BPSK,   // 二进制相移键控
    QPSK,   // 四相相移键控
    QAM16   // 16进制正交幅度调制
};

// 接收机类型枚举
enum class ReceiverType {
    SUPERHETERODYNE,  // 超外差接收机
    DIRECT_CONVERSION, // 直接变频接收机
    SOFTWARE_DEFINED   // 软件定义无线电
};

class CommunicationReceiveModel {
private:
    // 接收机基本参数
    double receiveSensitivity;    // 接收灵敏度(dBm)
    double noiseFigure;          // 噪声系数(dB)
    double systemBandwidth;      // 系统带宽(kHz)
    ReceiveModulationType modType; // 调制方式
    ReceiverType receiverType;   // 接收机类型
    
    // 环境参数
    double ambientTemperature;   // 环境温度(K)，默认290K
    double antennaGain;         // 接收天线增益(dBi)
    
    // 信号参数
    double receivedPower;       // 接收信号功率(dBm)
    double noiseFloor;          // 噪声底(dBm)
    
    // 常量定义
    static constexpr double BOLTZMANN_CONSTANT = 1.38e-23; // 玻尔兹曼常数(J/K)
    static constexpr double THERMAL_NOISE_DENSITY = -174.0; // 热噪声密度(dBm/Hz)

    // 参数校验方法
    bool isSensitivityValid(double sensitivity_dBm) const;
    bool isNoiseFigureValid(double nf_dB) const;
    bool isBandwidthValid(double bandwidth_kHz) const;
    bool isTemperatureValid(double temp_K) const;
    bool isAntennaGainValid(double gain_dBi) const;

    // 内部计算方法
    double calculateThermalNoise() const;
    double calculateSystemNoise() const;

public:
    // 构造函数，带默认参数
    CommunicationReceiveModel(
        double sensitivity = -100.0,     // 默认接收灵敏度-100dBm
        double nf = 3.0,                // 默认噪声系数3dB
        double bandwidth = 25.0,        // 默认带宽25kHz
        ReceiveModulationType mod = ReceiveModulationType::FM,
        ReceiverType receiver = ReceiverType::SUPERHETERODYNE,
        double temp = 290.0,            // 默认环境温度290K
        double antGain = 0.0            // 默认天线增益0dBi
    );

    // 参数设置方法
    bool setReceiveSensitivity(double sensitivity_dBm);
    bool setNoiseFigure(double nf_dB);
    bool setSystemBandwidth(double bandwidth_kHz);
    void setModulationType(ReceiveModulationType mod);
    void setReceiverType(ReceiverType receiver);
    bool setAmbientTemperature(double temp_K);
    bool setAntennaGain(double gain_dBi);
    
    // 信号设置方法
    bool setReceivedPower(double power_dBm);

    // 参数获取方法
    double getReceiveSensitivity() const;
    double getNoiseFigure() const;
    double getSystemBandwidth() const;
    ReceiveModulationType getModulationType() const;
    ReceiverType getReceiverType() const;
    double getAmbientTemperature() const;
    double getAntennaGain() const;
    double getReceivedPower() const;

    // 核心计算方法
    double calculateSignalToNoiseRatio() const;  // 计算信噪比(dB)
    double calculateBitErrorRate() const;        // 计算误码率
    double calculateEffectiveNoisePower() const; // 计算有效噪声功率(dBm)
    double calculateMinimumDetectablePower() const; // 计算最小可检测功率(dBm)
    
    // 接收性能评估
    bool isSignalDetectable() const;            // 判断信号是否可检测
    bool isSignalDecodable(double required_snr = 10.0) const; // 判断信号是否可解码
    double calculateReceiveMargin() const;       // 计算接收余量(dB)
    
    // 调制方式相关的理论SNR要求
    double getRequiredSNRForBER(double target_ber = 1e-6) const;
    
    // 获取参数信息字符串
    std::string getParameterInfo() const;
    std::string getPerformanceInfo() const;
};

#endif // COMMUNICATION_RECEIVE_MODEL_H