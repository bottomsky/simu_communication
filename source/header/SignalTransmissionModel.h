#ifndef SIGNAL_TRANSMISSION_MODEL_H
#define SIGNAL_TRANSMISSION_MODEL_H

#include <string>
#include <stdexcept>

// 工作频段枚举
enum class FrequencyBand {
    SHORT_WAVE,    // 短波 1.5-30MHz
    ULTRA_SHORT_WAVE,  // 超短波 30-300MHz
    MICROWAVE      // 微波 300MHz-30GHz
};

// 调制方式枚举
enum class ModulationType {
    AM,     // 调幅
    FM,     // 调频
    BPSK,   // 二进制相移键控
    QPSK,   // 四相相移键控
    QAM16   // 16进制正交幅度调制
};

class SignalTransmissionModel {
private:
    // 信号传输核心参数
    FrequencyBand band;          // 工作频段
    double centerFrequency;      // 中心频率(kHz)
    ModulationType modulation;   // 调制方式
    double signalBandwidth;      // 信号带宽(kHz)
    double transmitPower;        // 发射功率(W)

public:
    // 构造函数(默认参数为典型值)
    SignalTransmissionModel(
        FrequencyBand band = FrequencyBand::SHORT_WAVE,
        double centerFreqKHz = 10000.0,  // 10MHz
        ModulationType modulation = ModulationType::FM,
        double bandwidthKHz = 25.0,
        double powerW = 10.0
    );

    // 参数设置方法(带合法性校验)
    void setFrequencyBand(FrequencyBand newBand);
    bool setCenterFrequency(double freqKHz);  // 返回值表示设置是否成功
    void setModulationType(ModulationType newModulation);
    bool setSignalBandwidth(double bandwidthKHz);  // 返回值表示设置是否成功
    bool setTransmitPower(double powerW);  // 返回值表示设置是否成功

    // 参数获取方法
    FrequencyBand getFrequencyBand() const;
    double getCenterFrequency() const;     // 返回单位: kHz
    ModulationType getModulationType() const;
    double getSignalBandwidth() const;     // 返回单位: kHz
    double getTransmitPower() const;       // 返回单位: W

    // 打印当前参数配置
    std::string getParameterInfo() const;

    // 频段对应的频率范围校验
    bool isFrequencyInBand(double freqKHz, FrequencyBand bandType) const;
};

#endif // SIGNAL_TRANSMISSION_MODEL_H
