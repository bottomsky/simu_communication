#include "CommunicationParameterConfig.h"
#include <sstream>

// 静态成员变量定义
const ParameterRange CommunicationParameterConfig::MAX_LINE_OF_SIGHT_RANGE(0.5, 50.0);
const ParameterRange CommunicationParameterConfig::RECEIVE_SENSITIVITY_RANGE(-110.0, -90.0);
const ParameterRange CommunicationParameterConfig::LINK_MARGIN_RANGE(5.0, 20.0);
const ParameterRange CommunicationParameterConfig::TRANSMIT_POWER_RANGE(-30.0, 30.0);

ParameterRange CommunicationParameterConfig::getMaxLineOfSightRange() {
    return MAX_LINE_OF_SIGHT_RANGE;
}

ParameterRange CommunicationParameterConfig::getReceiveSensitivityRange() {
    return RECEIVE_SENSITIVITY_RANGE;
}

ParameterRange CommunicationParameterConfig::getLinkMarginRange() {
    return LINK_MARGIN_RANGE;
}

ParameterRange CommunicationParameterConfig::getTransmitPowerRange() {
    return TRANSMIT_POWER_RANGE;
}

bool CommunicationParameterConfig::isMaxLineOfSightValid(double maxLOS) {
    return MAX_LINE_OF_SIGHT_RANGE.isValid(maxLOS);
}

bool CommunicationParameterConfig::isReceiveSensitivityValid(double sensitivity) {
    return RECEIVE_SENSITIVITY_RANGE.isValid(sensitivity);
}

bool CommunicationParameterConfig::isLinkMarginValid(double margin) {
    return LINK_MARGIN_RANGE.isValid(margin);
}

bool CommunicationParameterConfig::isTransmitPowerValid(double power) {
    return TRANSMIT_POWER_RANGE.isValid(power);
}

std::string CommunicationParameterConfig::getParameterRangeInfo() {
    std::stringstream ss;
    ss << "通信参数有效范围配置:\n"
       << "  最大视距距离: " << MAX_LINE_OF_SIGHT_RANGE.minValue 
       << " - " << MAX_LINE_OF_SIGHT_RANGE.maxValue << " km\n"
       << "  接收灵敏度: " << RECEIVE_SENSITIVITY_RANGE.minValue 
       << " - " << RECEIVE_SENSITIVITY_RANGE.maxValue << " dBm\n"
       << "  链路余量: " << LINK_MARGIN_RANGE.minValue 
       << " - " << LINK_MARGIN_RANGE.maxValue << " dB\n"
       << "  发射功率: " << TRANSMIT_POWER_RANGE.minValue 
       << " - " << TRANSMIT_POWER_RANGE.maxValue << " dBm";
    return ss.str();
}