#include "SignalTransmissionModel.h"
#include "MathConstants.h"
#include <string>
#include <sstream>

// 频段频率范围校验实现
bool SignalTransmissionModel::isFrequencyInBand(double freqKHz, FrequencyBand bandType) const {
    switch (bandType) {
        case FrequencyBand::SHORT_WAVE:
            return (freqKHz >= MathConstants::HF_MIN_FREQUENCY && freqKHz <= MathConstants::HF_MAX_FREQUENCY);  // 1.5-30MHz
        case FrequencyBand::ULTRA_SHORT_WAVE:
            return (freqKHz >= MathConstants::VHF_MIN_FREQUENCY && freqKHz <= MathConstants::VHF_MAX_FREQUENCY);  // 30-300MHz
        case FrequencyBand::MICROWAVE:
            return (freqKHz >= MathConstants::UHF_MIN_FREQUENCY && freqKHz <= MathConstants::UHF_MAX_FREQUENCY);  // 300MHz-30GHz
        default:
            return false;
    }
}

// 构造函数实现
SignalTransmissionModel::SignalTransmissionModel(
    FrequencyBand band,
    double centerFreqKHz,
    ModulationType modulation,
    double bandwidthKHz,
    double powerW
) : band(band), modulation(modulation) {
    // 初始化参数(带校验)
    if (!isFrequencyInBand(centerFreqKHz, band)) {
        throw std::invalid_argument("中心频率超出所选频段范围");
    }
    centerFrequency = centerFreqKHz;

    if (bandwidthKHz <= 0) {
        throw std::invalid_argument("信号带宽必须为正数");
    }
    signalBandwidth = bandwidthKHz;

    if (powerW <= 0) {
        throw std::invalid_argument("发射功率必须为正数");
    }
    transmitPower = powerW;
}

// 参数设置方法实现
void SignalTransmissionModel::setFrequencyBand(FrequencyBand newBand) {
    // 切换频段时需重新校验中心频率
    double currentFreq = centerFrequency;
    band = newBand;
    if (!isFrequencyInBand(currentFreq, newBand)) {
        // 若原频率不符合新频段，自动调整到新频段中间值
        switch (newBand) {
            case FrequencyBand::SHORT_WAVE:
            centerFrequency = MathConstants::DEFAULT_HF_CENTER_FREQ;  // 15.75MHz
            break;
        case FrequencyBand::ULTRA_SHORT_WAVE:
            centerFrequency = MathConstants::DEFAULT_VHF_CENTER_FREQ;  // 165MHz
            break;
        case FrequencyBand::MICROWAVE:
            centerFrequency = MathConstants::DEFAULT_UHF_CENTER_FREQ;  // 15.15GHz
            break;
        }
    }
}

bool SignalTransmissionModel::setCenterFrequency(double freqKHz) {
    if (isFrequencyInBand(freqKHz, band)) {
        centerFrequency = freqKHz;
        return true;
    }
    return false;
}

void SignalTransmissionModel::setModulationType(ModulationType newModulation) {
    modulation = newModulation;
}

bool SignalTransmissionModel::setSignalBandwidth(double bandwidthKHz) {
    if (bandwidthKHz > 0) {
        signalBandwidth = bandwidthKHz;
        return true;
    }
    return false;
}

bool SignalTransmissionModel::setTransmitPower(double powerW) {
    if (powerW > 0 && powerW <= MathConstants::MAX_POWER_LIMIT) {  // 限制在0-100W(符合车载/便携设备范围)
        transmitPower = powerW;
        return true;
    }
    return false;
}

// 参数获取方法实现
FrequencyBand SignalTransmissionModel::getFrequencyBand() const {
    return band;
}

double SignalTransmissionModel::getCenterFrequency() const {
    return centerFrequency;
}

ModulationType SignalTransmissionModel::getModulationType() const {
    return modulation;
}

double SignalTransmissionModel::getSignalBandwidth() const {
    return signalBandwidth;
}

double SignalTransmissionModel::getTransmitPower() const {
    return transmitPower;
}

// 参数信息字符串生成
std::string SignalTransmissionModel::getParameterInfo() const {
    std::stringstream ss;
    ss << "信号传输模型参数:\n";
    
    // 频段信息
    ss << "  工作频段: ";
    switch (band) {
        case FrequencyBand::SHORT_WAVE:
            ss << "短波 (1.5-30MHz)";
            break;
        case FrequencyBand::ULTRA_SHORT_WAVE:
            ss << "超短波 (30-300MHz)";
            break;
        case FrequencyBand::MICROWAVE:
            ss << "微波 (300MHz-30GHz)";
            break;
    }
    ss << "\n";
    
    // 中心频率
    ss << "  中心频率: " << centerFrequency << " kHz";
    if (centerFrequency >= MathConstants::FREQUENCY_DISPLAY_THRESHOLD_MHZ) {
            ss << " (" << (centerFrequency / MathConstants::FREQUENCY_DISPLAY_THRESHOLD_MHZ) << " GHz)";
        } else if (centerFrequency >= MathConstants::FREQUENCY_DISPLAY_THRESHOLD_KHZ) {
            ss << " (" << (centerFrequency / MathConstants::FREQUENCY_DISPLAY_THRESHOLD_KHZ) << " MHz)";
        }
    ss << "\n";
    
    // 调制方式
    ss << "  调制方式: ";
    switch (modulation) {
        case ModulationType::AM:
            ss << "调幅 (AM)";
            break;
        case ModulationType::FM:
            ss << "调频 (FM)";
            break;
        case ModulationType::BPSK:
            ss << "二进制相移键控 (BPSK)";
            break;
        case ModulationType::QPSK:
            ss << "四相相移键控 (QPSK)";
            break;
        case ModulationType::QAM16:
            ss << "16进制正交幅度调制 (QAM16)";
            break;
    }
    ss << "\n";
    
    // 信号带宽
    ss << "  信号带宽: " << signalBandwidth << " kHz";
    if (signalBandwidth >= MathConstants::BANDWIDTH_DISPLAY_THRESHOLD) {
            ss << " (" << (signalBandwidth / MathConstants::BANDWIDTH_DISPLAY_THRESHOLD) << " MHz)";
        }
    ss << "\n";
    
    // 发射功率
    ss << "  发射功率: " << transmitPower << " W";
    if (transmitPower >= MathConstants::POWER_DISPLAY_THRESHOLD) {
            ss << " (" << (transmitPower / MathConstants::POWER_DISPLAY_THRESHOLD) << " kW)";
        }
    ss << "\n";
    
    return ss.str();
}