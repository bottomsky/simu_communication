#include "../header/CommunicationReceiveModel.h"
#include <sstream>
#include <iomanip>

// 构造函数实现
CommunicationReceiveModel::CommunicationReceiveModel(
    double sensitivity, double nf, double bandwidth,
    ReceiveModulationType mod, ReceiverType receiver,
    double temp, double antGain)
    : modType(mod), receiverType(receiver), receivedPower(-120.0), noiseFloor(-120.0) {
    
    if (!setReceiveSensitivity(sensitivity)) {
        throw std::invalid_argument("接收灵敏度超出有效范围(-150至-30dBm)");
    }
    if (!setNoiseFigure(nf)) {
        throw std::invalid_argument("噪声系数超出有效范围(0至20dB)");
    }
    if (!setSystemBandwidth(bandwidth)) {
        throw std::invalid_argument("系统带宽超出有效范围(0.1至10000kHz)");
    }
    if (!setAmbientTemperature(temp)) {
        throw std::invalid_argument("环境温度超出有效范围(200至400K)");
    }
    if (!setAntennaGain(antGain)) {
        throw std::invalid_argument("天线增益超出有效范围(-20至50dBi)");
    }
    
    // 计算初始噪声底
    noiseFloor = calculateSystemNoise();
}

// 参数校验方法实现
bool CommunicationReceiveModel::isSensitivityValid(double sensitivity_dBm) const {
    return sensitivity_dBm >= -150.0 && sensitivity_dBm <= -30.0;
}

bool CommunicationReceiveModel::isNoiseFigureValid(double nf_dB) const {
    return nf_dB >= 0.0 && nf_dB <= 20.0;
}

bool CommunicationReceiveModel::isBandwidthValid(double bandwidth_kHz) const {
    return bandwidth_kHz >= 0.1 && bandwidth_kHz <= 10000.0;
}

bool CommunicationReceiveModel::isTemperatureValid(double temp_K) const {
    return temp_K >= 200.0 && temp_K <= 400.0;
}

bool CommunicationReceiveModel::isAntennaGainValid(double gain_dBi) const {
    return gain_dBi >= -20.0 && gain_dBi <= 50.0;
}

// 参数设置方法实现
bool CommunicationReceiveModel::setReceiveSensitivity(double sensitivity_dBm) {
    if (!isSensitivityValid(sensitivity_dBm)) {
        return false;
    }
    receiveSensitivity = sensitivity_dBm;
    return true;
}

bool CommunicationReceiveModel::setNoiseFigure(double nf_dB) {
    if (!isNoiseFigureValid(nf_dB)) {
        return false;
    }
    noiseFigure = nf_dB;
    noiseFloor = calculateSystemNoise(); // 重新计算噪声底
    return true;
}

bool CommunicationReceiveModel::setSystemBandwidth(double bandwidth_kHz) {
    if (!isBandwidthValid(bandwidth_kHz)) {
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

bool CommunicationReceiveModel::setAmbientTemperature(double temp_K) {
    if (!isTemperatureValid(temp_K)) {
        return false;
    }
    ambientTemperature = temp_K;
    noiseFloor = calculateSystemNoise(); // 重新计算噪声底
    return true;
}

bool CommunicationReceiveModel::setAntennaGain(double gain_dBi) {
    if (!isAntennaGainValid(gain_dBi)) {
        return false;
    }
    antennaGain = gain_dBi;
    return true;
}

bool CommunicationReceiveModel::setReceivedPower(double power_dBm) {
    if (power_dBm < -200.0 || power_dBm > 50.0) {
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

// 内部计算方法实现
double CommunicationReceiveModel::calculateThermalNoise() const {
    // 热噪声功率 = kTB (W)
    // 转换为dBm: 10*log10(kTB*1000)
    double thermal_noise_W = BOLTZMANN_CONSTANT * ambientTemperature * (systemBandwidth * 1000);
    return 10.0 * log10(thermal_noise_W * 1000.0);
}

double CommunicationReceiveModel::calculateSystemNoise() const {
    // 系统噪声 = 热噪声 + 噪声系数
    // 使用标准公式: N = -174 + 10*log10(BW) + NF (dBm)
    double bandwidth_Hz = systemBandwidth * 1000.0;
    return THERMAL_NOISE_DENSITY + 10.0 * log10(bandwidth_Hz) + noiseFigure;
}

// 核心计算方法实现
double CommunicationReceiveModel::calculateSignalToNoiseRatio() const {
    // SNR = 接收信号功率 - 噪声功率 (dB)
    return receivedPower - noiseFloor;
}

double CommunicationReceiveModel::calculateBitErrorRate() const {
    double snr_linear = pow(10.0, calculateSignalToNoiseRatio() / 10.0);
    
    // 根据调制方式计算误码率
    switch (modType) {
        case ReceiveModulationType::BPSK:
            // BER = 0.5 * erfc(sqrt(SNR))
            return 0.5 * erfc(sqrt(snr_linear));
            
        case ReceiveModulationType::QPSK:
            // BER = 0.5 * erfc(sqrt(SNR/2))
            return 0.5 * erfc(sqrt(snr_linear / 2.0));
            
        case ReceiveModulationType::QAM16:
            // 简化的16QAM误码率公式
            return 0.375 * erfc(sqrt(0.4 * snr_linear));
            
        case ReceiveModulationType::FM:
            // FM的误码率与SNR的关系较复杂，使用简化模型
            if (snr_linear < 10.0) {
                return 0.1; // 低SNR时误码率较高
            } else {
                return 1e-6 * exp(-snr_linear / 10.0);
            }
            
        case ReceiveModulationType::AM:
            // AM的误码率模型
            return 0.5 * erfc(sqrt(snr_linear / 4.0));
            
        default:
            return 0.5; // 默认返回50%误码率
    }
}

double CommunicationReceiveModel::calculateEffectiveNoisePower() const {
    return noiseFloor;
}

double CommunicationReceiveModel::calculateMinimumDetectablePower() const {
    // 最小可检测功率 = 噪声底 + 检测门限(通常3dB)
    return noiseFloor + 3.0;
}

// 接收性能评估方法实现
bool CommunicationReceiveModel::isSignalDetectable() const {
    return receivedPower > calculateMinimumDetectablePower();
}

bool CommunicationReceiveModel::isSignalDecodable(double required_snr) const {
    return calculateSignalToNoiseRatio() >= required_snr;
}

double CommunicationReceiveModel::calculateReceiveMargin() const {
    return receivedPower - receiveSensitivity;
}

double CommunicationReceiveModel::getRequiredSNRForBER(double target_ber) const {
    // 根据调制方式返回达到目标误码率所需的SNR
    switch (modType) {
        case ReceiveModulationType::BPSK:
            // 对于BPSK，BER = 1e-6时，SNR ≈ 10.5dB
            if (target_ber <= 1e-6) return 10.5;
            else if (target_ber <= 1e-4) return 8.4;
            else return 6.0;
            
        case ReceiveModulationType::QPSK:
            // 对于QPSK，BER = 1e-6时，SNR ≈ 10.5dB
            if (target_ber <= 1e-6) return 10.5;
            else if (target_ber <= 1e-4) return 8.4;
            else return 6.0;
            
        case ReceiveModulationType::QAM16:
            // 对于16QAM，需要更高的SNR
            if (target_ber <= 1e-6) return 16.0;
            else if (target_ber <= 1e-4) return 12.0;
            else return 9.0;
            
        case ReceiveModulationType::FM:
            // FM需要的SNR相对较低
            if (target_ber <= 1e-6) return 12.0;
            else return 8.0;
            
        case ReceiveModulationType::AM:
            // AM需要中等SNR
            if (target_ber <= 1e-6) return 14.0;
            else return 10.0;
            
        default:
            return 10.0;
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