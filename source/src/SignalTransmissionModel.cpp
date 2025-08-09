#include "SignalTransmissionModel.h"
#include <string>
#include <sstream>

// 频段频率范围校验实现
bool SignalTransmissionModel::isFrequencyInBand(double freqKHz, FrequencyBand band) const {
    switch (band) {
        case FrequencyBand::SHORT_WAVE:
            return (freqKHz >= 1500.0 && freqKHz <= 30000.0);  // 1.5-30MHz
        case FrequencyBand::ULTRA_SHORT_WAVE:
            return (freqKHz >= 30000.0 && freqKHz <= 300000.0);  // 30-300MHz
        case FrequencyBand::MICROWAVE:
            return (freqKHz >= 300000.0 && freqKHz <= 30000000.0);  // 300MHz-30GHz
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
                centerFrequency = 15750.0;  // 15.75MHz
                break;
            case FrequencyBand::ULTRA_SHORT_WAVE:
                centerFrequency = 165000.0;  // 165MHz
                break;
            case FrequencyBand::MICROWAVE:
                centerFrequency = 15150000.0;  // 15.15GHz
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
    if (powerW > 0 && powerW <= 100.0) {  // 限制在0-100W(符合车载/便携设备范围)
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