#include "CommunicationReceiveParameterConfig.h"
#include <sstream>

// 静态成员变量定义
const ReceiveParameterRange CommunicationReceiveParameterConfig::RECEIVE_SENSITIVITY_RANGE(-150.0, -30.0);
const ReceiveParameterRange CommunicationReceiveParameterConfig::NOISE_FIGURE_RANGE(0.0, 20.0);
const ReceiveParameterRange CommunicationReceiveParameterConfig::SYSTEM_BANDWIDTH_RANGE(0.1, 10000.0);
const ReceiveParameterRange CommunicationReceiveParameterConfig::AMBIENT_TEMPERATURE_RANGE(200.0, 400.0);
const ReceiveParameterRange CommunicationReceiveParameterConfig::ANTENNA_GAIN_RANGE(-20.0, 50.0);
const ReceiveParameterRange CommunicationReceiveParameterConfig::RECEIVED_POWER_RANGE(-200.0, 50.0);

// 参数范围获取方法实现
ReceiveParameterRange CommunicationReceiveParameterConfig::getReceiveSensitivityRange() {
    return RECEIVE_SENSITIVITY_RANGE;
}

ReceiveParameterRange CommunicationReceiveParameterConfig::getNoiseFigureRange() {
    return NOISE_FIGURE_RANGE;
}

ReceiveParameterRange CommunicationReceiveParameterConfig::getSystemBandwidthRange() {
    return SYSTEM_BANDWIDTH_RANGE;
}

ReceiveParameterRange CommunicationReceiveParameterConfig::getAmbientTemperatureRange() {
    return AMBIENT_TEMPERATURE_RANGE;
}

ReceiveParameterRange CommunicationReceiveParameterConfig::getAntennaGainRange() {
    return ANTENNA_GAIN_RANGE;
}

ReceiveParameterRange CommunicationReceiveParameterConfig::getReceivedPowerRange() {
    return RECEIVED_POWER_RANGE;
}

// 参数校验方法实现
bool CommunicationReceiveParameterConfig::isReceiveSensitivityValid(double sensitivity) {
    return RECEIVE_SENSITIVITY_RANGE.isValid(sensitivity);
}

bool CommunicationReceiveParameterConfig::isNoiseFigureValid(double noiseFigure) {
    return NOISE_FIGURE_RANGE.isValid(noiseFigure);
}

bool CommunicationReceiveParameterConfig::isSystemBandwidthValid(double bandwidth) {
    return SYSTEM_BANDWIDTH_RANGE.isValid(bandwidth);
}

bool CommunicationReceiveParameterConfig::isAmbientTemperatureValid(double temperature) {
    return AMBIENT_TEMPERATURE_RANGE.isValid(temperature);
}

bool CommunicationReceiveParameterConfig::isAntennaGainValid(double gain) {
    return ANTENNA_GAIN_RANGE.isValid(gain);
}

bool CommunicationReceiveParameterConfig::isReceivedPowerValid(double power) {
    return RECEIVED_POWER_RANGE.isValid(power);
}

// 参数范围信息获取方法实现
std::string CommunicationReceiveParameterConfig::getParameterRangeInfo() {
    std::stringstream ss;
    ss << "通信接收模型参数有效范围配置:\n"
       << "  接收灵敏度: " << RECEIVE_SENSITIVITY_RANGE.minValue 
       << " - " << RECEIVE_SENSITIVITY_RANGE.maxValue << " dBm\n"
       << "  噪声系数: " << NOISE_FIGURE_RANGE.minValue 
       << " - " << NOISE_FIGURE_RANGE.maxValue << " dB\n"
       << "  系统带宽: " << SYSTEM_BANDWIDTH_RANGE.minValue 
       << " - " << SYSTEM_BANDWIDTH_RANGE.maxValue << " kHz\n"
       << "  环境温度: " << AMBIENT_TEMPERATURE_RANGE.minValue 
       << " - " << AMBIENT_TEMPERATURE_RANGE.maxValue << " K\n"
       << "  天线增益: " << ANTENNA_GAIN_RANGE.minValue 
       << " - " << ANTENNA_GAIN_RANGE.maxValue << " dBi\n"
       << "  接收功率: " << RECEIVED_POWER_RANGE.minValue 
       << " - " << RECEIVED_POWER_RANGE.maxValue << " dBm";
    return ss.str();
}