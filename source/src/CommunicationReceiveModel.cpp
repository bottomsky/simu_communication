#include "../header/CommunicationReceiveModel.h"
#include <sstream>
#include <iomanip>

// 构造函数实现
CommunicationReceiveModel::CommunicationReceiveModel(
    double sensitivity, double nf, double bandwidth,
    ReceiveModulationType mod, ReceiverType receiver,
    double temp, double antGain)
    : modType(mod), receiverType(receiver), receivedPower(-120.0), noiseFloor(-120.0) {
    
    if (!CommunicationReceiveParameterConfig::isReceiveSensitivityValid(sensitivity)) {
        auto range = CommunicationReceiveParameterConfig::getReceiveSensitivityRange();
        throw std::invalid_argument("接收灵敏度需在" + std::to_string(range.minValue) + "至" + std::to_string(range.maxValue) + "dBm范围内");
    }
    if (!CommunicationReceiveParameterConfig::isNoiseFigureValid(nf)) {
        auto range = CommunicationReceiveParameterConfig::getNoiseFigureRange();
        throw std::invalid_argument("噪声系数需在" + std::to_string(range.minValue) + "至" + std::to_string(range.maxValue) + "dB范围内");
    }
    if (!CommunicationReceiveParameterConfig::isSystemBandwidthValid(bandwidth)) {
        auto range = CommunicationReceiveParameterConfig::getSystemBandwidthRange();
        throw std::invalid_argument("系统带宽需在" + std::to_string(range.minValue) + "至" + std::to_string(range.maxValue) + "kHz范围内");
    }
    if (!CommunicationReceiveParameterConfig::isAmbientTemperatureValid(temp)) {
        auto range = CommunicationReceiveParameterConfig::getAmbientTemperatureRange();
        throw std::invalid_argument("环境温度需在" + std::to_string(range.minValue) + "至" + std::to_string(range.maxValue) + "K范围内");
    }
    if (!CommunicationReceiveParameterConfig::isAntennaGainValid(antGain)) {
        auto range = CommunicationReceiveParameterConfig::getAntennaGainRange();
        throw std::invalid_argument("天线增益需在" + std::to_string(range.minValue) + "至" + std::to_string(range.maxValue) + "dBi范围内");
    }
    
    // 设置参数值
    receiveSensitivity = sensitivity;
    noiseFigure = nf;
    systemBandwidth = bandwidth;
    ambientTemperature = temp;
    antennaGain = antGain;
    
    // 计算初始噪声底
    noiseFloor = calculateSystemNoise();
}

// 参数校验方法实现
bool CommunicationReceiveModel::isSensitivityValid(double sensitivity_dBm) const {
    return CommunicationReceiveParameterConfig::isReceiveSensitivityValid(sensitivity_dBm);
}

bool CommunicationReceiveModel::isNoiseFigureValid(double nf_dB) const {
    return CommunicationReceiveParameterConfig::isNoiseFigureValid(nf_dB);
}

bool CommunicationReceiveModel::isBandwidthValid(double bandwidth_kHz) const {
    return CommunicationReceiveParameterConfig::isSystemBandwidthValid(bandwidth_kHz);
}

bool CommunicationReceiveModel::isTemperatureValid(double temp_K) const {
    return CommunicationReceiveParameterConfig::isAmbientTemperatureValid(temp_K);
}

bool CommunicationReceiveModel::isAntennaGainValid(double gain_dBi) const {
    return CommunicationReceiveParameterConfig::isAntennaGainValid(gain_dBi);
}

// 参数设置方法实现
bool CommunicationReceiveModel::setReceiveSensitivity(double sensitivity_dBm) {
    if (!CommunicationReceiveParameterConfig::isReceiveSensitivityValid(sensitivity_dBm)) {
        return false;
    }
    receiveSensitivity = sensitivity_dBm;
    return true;
}

bool CommunicationReceiveModel::setNoiseFigure(double nf_dB) {
    if (!CommunicationReceiveParameterConfig::isNoiseFigureValid(nf_dB)) {
        return false;
    }
    noiseFigure = nf_dB;
    noiseFloor = calculateSystemNoise(); // 重新计算噪声底
    return true;
}

bool CommunicationReceiveModel::setSystemBandwidth(double bandwidth_kHz) {
    if (!CommunicationReceiveParameterConfig::isSystemBandwidthValid(bandwidth_kHz)) {
        return false;
    }
    systemBandwidth = bandwidth_kHz;
    noiseFloor = calculateSystemNoise(); // 重新计算噪声底
    return true;
}

void CommunicationReceiveModel::setModulationType(ReceiveModulationType mod) {
    modType = mod;
}

void CommunicationReceiveModel::setReceiverType(ReceiverType receiver) {
    receiverType = receiver;
}
/// @brief 设置接收机温度
/// @param temp_K 温度(K)
/// @return 温度是否设置成功
bool CommunicationReceiveModel::setAmbientTemperature(double temp_K) {
    if (!CommunicationReceiveParameterConfig::isAmbientTemperatureValid(temp_K)) {
        return false;
    }
    ambientTemperature = temp_K;
    noiseFloor = calculateSystemNoise(); // 重新计算噪声底
    return true;
}

/// @brief 设置天线增益
/// @param gain_dBi 天线增益(dBi)
/// @return 天线增益是否设置成功
bool CommunicationReceiveModel::setAntennaGain(double gain_dBi) {
    if (!CommunicationReceiveParameterConfig::isAntennaGainValid(gain_dBi)) {
        return false;
    }
    antennaGain = gain_dBi;
    return true;
}

bool CommunicationReceiveModel::setReceivedPower(double power_dBm) {
    if (!CommunicationReceiveParameterConfig::isReceivedPowerValid(power_dBm)) {
        return false;
    }
    receivedPower = power_dBm;
    return true;
}

// 参数获取方法实现
double CommunicationReceiveModel::getReceiveSensitivity() const {
    return receiveSensitivity;
}

double CommunicationReceiveModel::getNoiseFigure() const {
    return noiseFigure;
}

double CommunicationReceiveModel::getSystemBandwidth() const {
    return systemBandwidth;
}

ReceiveModulationType CommunicationReceiveModel::getModulationType() const {
    return modType;
}

ReceiverType CommunicationReceiveModel::getReceiverType() const {
    return receiverType;
}

double CommunicationReceiveModel::getAmbientTemperature() const {
    return ambientTemperature;
}

double CommunicationReceiveModel::getAntennaGain() const {
    return antennaGain;
}

double CommunicationReceiveModel::getReceivedPower() const {
    return receivedPower;
}

/// @brief 计算热噪声功率
/// @return 热噪声功率(dBm)
double CommunicationReceiveModel::calculateThermalNoise() const {
    // 热噪声功率 = kTB (W)
    // 转换为dBm: 10*log10(kTB*1000)
    double thermal_noise_W = BOLTZMANN_CONSTANT * ambientTemperature * (systemBandwidth * 1000);
    return 10.0 * log10(thermal_noise_W * 1000.0);
}

/// @brief 计算系统噪声功率
/// @return 系统噪声功率(dBm)
double CommunicationReceiveModel::calculateSystemNoise() const {
    // 系统噪声 = 热噪声 + 噪声系数
    // 使用标准公式: N = -174 + 10*log10(BW) + NF (dBm)
    double bandwidth_Hz = systemBandwidth * 1000.0;
    return THERMAL_NOISE_DENSITY + 10.0 * log10(bandwidth_Hz) + noiseFigure;
}

/// @brief 计算信噪比
/// @details 信噪比 = 接收信号功率 + 天线增益 - 系统噪声功率 (dB)
/// @return 信噪比(dB)
double CommunicationReceiveModel::calculateSignalToNoiseRatio() const {
    // SNR = 接收信号功率 + 天线增益 - 系统噪声功率 (dB)
    return receivedPower + antennaGain - noiseFloor;
}

/// @brief 计算误码率
/// @details 误码率 = 0.5 * erfc(sqrt(SNR))
/// @return 误码率
double CommunicationReceiveModel::calculateBitErrorRate() const {
    double snr_linear = pow(10.0, calculateSignalToNoiseRatio() / 10.0);
    double ber = 0.0;
    
    // 根据调制方式计算误码率
    switch (modType) {
        case ReceiveModulationType::BPSK:
            // BER = 0.5 * erfc(sqrt(SNR))
            ber = 0.5 * erfc(sqrt(snr_linear));
            break;
            
        case ReceiveModulationType::QPSK:
            // BER = 0.5 * erfc(sqrt(SNR/2))
            ber = 0.5 * erfc(sqrt(snr_linear / 2.0));
            break;
            
        case ReceiveModulationType::QAM16:
            // 简化的16QAM误码率公式
            ber = 0.375 * erfc(sqrt(0.4 * snr_linear));
            break;
            
        case ReceiveModulationType::FM:
            // FM的误码率与SNR的关系较复杂，使用简化模型
            if (snr_linear < 10.0) {
                ber = 0.1; // 低SNR时误码率较高
            } else {
                ber = 1e-6 * exp(-snr_linear / 10.0);
            }
            break;
            
        case ReceiveModulationType::AM:
            // AM的误码率模型
            ber = 0.5 * erfc(sqrt(snr_linear / 4.0));
            break;
            
        default:
            ber = 0.5; // 默认返回50%误码率
            break;
    }
    
    // 确保误码率大于0，但允许在高SNR时继续变化
    // 使用一个与SNR相关的最小值，确保不同SNR产生不同结果
    double snr_db = calculateSignalToNoiseRatio();
    double min_ber = 1e-50 * exp(-snr_db / 100.0); // SNR越高，最小值越小
    return std::max(ber, min_ber);
}

/// @brief 计算有效噪声功率
/// @details 有效噪声功率 = 系统噪声功率 + 天线增益
/// @return 有效噪声功率(dBm)
double CommunicationReceiveModel::calculateEffectiveNoisePower() const {
    return noiseFloor + antennaGain;
}

/// @brief 计算最小可检测功率
/// @details 最小可检测功率 = 噪声底 + 检测门限(通常10dB)
/// @return 最小可检测功率(dBm)
double CommunicationReceiveModel::calculateMinimumDetectablePower() const {
    // 最小可检测功率 = 噪声底 + 检测门限(通常10dB)
    return noiseFloor + 10.0;
}

// 接收性能评估方法实现
/// @brief 检查信号是否可检测
/// @details 信号可检测条件：接收功率大于等于最小可检测功率
/// @return 信号是否可检测
bool CommunicationReceiveModel::isSignalDetectable() const {
    return receivedPower > calculateMinimumDetectablePower();
}

/// @brief 检查信号是否可解码
/// @details 信号可解码条件：接收功率大于等于接收灵敏度
/// @return 信号是否可解码
bool CommunicationReceiveModel::isSignalDecodable(double required_snr) const {
    return calculateSignalToNoiseRatio() >= required_snr;
}

/// @brief 计算接收余量
/// @details 接收余量 = 接收功率 - 接收灵敏度
/// @return 接收余量(dBm)
double CommunicationReceiveModel::calculateReceiveMargin() const {
    return receivedPower - receiveSensitivity;
}

double CommunicationReceiveModel::getRequiredSNRForBER(double target_ber) const {
    // 根据调制方式返回达到目标误码率所需的SNR
    switch (modType) {
        case ReceiveModulationType::BPSK:
            // 对于BPSK，BER = 1e-6时，SNR ≈ 10.5dB
            if (target_ber <= 1e-6) return 10.5;
            else if (target_ber <= 1e-4) return 8.5;
            else if (target_ber <= 1e-2) return 6.0;
            else return 4.0;
            
        case ReceiveModulationType::QPSK:
            // 对于QPSK，BER = 1e-6时，SNR ≈ 10.5dB
            if (target_ber <= 1e-6) return 10.5;
            else if (target_ber <= 1e-4) return 8.5;
            else if (target_ber <= 1e-2) return 6.0;
            else return 4.0;
            
        case ReceiveModulationType::QAM16:
            // 对于16QAM，需要更高的SNR
            if (target_ber <= 1e-6) return 16.0;
            else if (target_ber <= 1e-4) return 12.0;
            else if (target_ber <= 1e-2) return 9.0;
            else return 7.0;
            
        case ReceiveModulationType::FM:
            // FM需要的SNR相对较低
            if (target_ber <= 1e-6) return 12.0;
            else if (target_ber <= 1e-4) return 9.0;
            else if (target_ber <= 1e-2) return 6.0;
            else return 4.0;
            
        case ReceiveModulationType::AM:
            // AM需要中等SNR
            if (target_ber <= 1e-6) return 14.0;
            else if (target_ber <= 1e-4) return 11.0;
            else if (target_ber <= 1e-2) return 8.0;
            else return 6.0;
            
        default:
            if (target_ber <= 1e-6) return 12.0;
            else if (target_ber <= 1e-4) return 9.0;
            else if (target_ber <= 1e-2) return 6.0;
            else return 4.0;
    }
}

// 信息输出方法实现
std::string CommunicationReceiveModel::getParameterInfo() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    
    oss << "=== 通信接收模型参数 ===\n";
    oss << "接收灵敏度: " << receiveSensitivity << " dBm\n";
    oss << "噪声系数: " << noiseFigure << " dB\n";
    oss << "系统带宽: " << systemBandwidth << " kHz\n";
    
    oss << "调制方式: ";
    switch (modType) {
        case ReceiveModulationType::AM: oss << "AM"; break;
        case ReceiveModulationType::FM: oss << "FM"; break;
        case ReceiveModulationType::BPSK: oss << "BPSK"; break;
        case ReceiveModulationType::QPSK: oss << "QPSK"; break;
        case ReceiveModulationType::QAM16: oss << "16QAM"; break;
    }
    oss << "\n";
    
    oss << "接收机类型: ";
    switch (receiverType) {
        case ReceiverType::SUPERHETERODYNE: oss << "超外差"; break;
        case ReceiverType::DIRECT_CONVERSION: oss << "直接变频"; break;
        case ReceiverType::SOFTWARE_DEFINED: oss << "软件定义"; break;
    }
    oss << "\n";
    
    oss << "环境温度: " << ambientTemperature << " K\n";
    oss << "天线增益: " << antennaGain << " dBi\n";
    oss << "噪声底: " << noiseFloor << " dBm\n";
    
    return oss.str();
}

std::string CommunicationReceiveModel::getPerformanceInfo() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    
    oss << "=== 接收性能信息 ===\n";
    oss << "接收信号功率: " << receivedPower << " dBm\n";
    oss << "信噪比: " << calculateSignalToNoiseRatio() << " dB\n";
    oss << "误码率: " << std::scientific << calculateBitErrorRate() << std::fixed << "\n";
    oss << "接收余量: " << calculateReceiveMargin() << " dB\n";
    oss << "最小可检测功率: " << calculateMinimumDetectablePower() << " dBm\n";
    oss << "信号可检测: " << (isSignalDetectable() ? "是" : "否") << "\n";
    oss << "信号可解码: " << (isSignalDecodable() ? "是" : "否") << "\n";
    
    return oss.str();
}