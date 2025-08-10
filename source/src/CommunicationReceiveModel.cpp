#include "../header/CommunicationReceiveModel.h"
#include "../header/MathConstants.h"
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>

// 构造函数实现
CommunicationReceiveModel::CommunicationReceiveModel(
    double sensitivity, double nf, double bandwidth,
    ReceiveModulationType mod, ReceiverType receiver,
    double temp, double antGain)
    : modType(mod), receiverType(receiver), receivedPower(-120.0), noiseFloor(-120.0), detectionThreshold(10.0) {
    
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

bool CommunicationReceiveModel::isDetectionThresholdValid(double threshold_dB) const {
    // 检测门限通常在0-30dB范围内，过低会增加虚警率，过高会降低检测概率
    return (threshold_dB >= 0.0 && threshold_dB <= 30.0);
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

bool CommunicationReceiveModel::setDetectionThreshold(double threshold_dB) {
    if (!isDetectionThresholdValid(threshold_dB)) {
        return false;
    }
    detectionThreshold = threshold_dB;
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

double CommunicationReceiveModel::getDetectionThreshold() const {
    return detectionThreshold;
}

/// @brief 计算热噪声功率
/// @return 热噪声功率(dBm)
double CommunicationReceiveModel::calculateThermalNoise() const {
    // 热噪声功率 = kTB (W)
    // 转换为dBm: 10*log10(kTB*1000)
    double thermal_noise_W = MathConstants::BOLTZMANN_CONSTANT * ambientTemperature * (systemBandwidth * 1000);
    return 10.0 * log10(thermal_noise_W * 1000.0);
}

/// @brief 计算系统噪声功率
/// @return 系统噪声功率(dBm)
double CommunicationReceiveModel::calculateSystemNoise() const {
    // 修正：系统噪声 = 热噪声（含温度影响） + 噪声系数
    // 复用calculateThermalNoise()的结果，确保温度被正确纳入计算
    return calculateThermalNoise() + noiseFigure;
}

/// @brief 计算误码率（优化版）
/// @details 采用更精确的理论公式，尤其是FM和AM调制
/// @return 误码率
double CommunicationReceiveModel::calculateBitErrorRate() const {
    double snr_db = calculateSignalToNoiseRatio();
    if (snr_db < -10.0) { // SNR过低时直接返回高误码率
        return 0.5;
    }
    double snr_linear = pow(10.0, snr_db / 10.0);
    double ber = 0.0;
    
    // 根据调制方式计算误码率（采用更精确的理论模型）
    switch (modType) {
        case ReceiveModulationType::BPSK:
            // AWGN信道下BPSK理论公式：0.5*erfc(sqrt(SNR))
            ber = 0.5 * erfc(sqrt(snr_linear));
            break;
            
        case ReceiveModulationType::QPSK:
            // QPSK等价于两个正交BPSK，BER公式与BPSK相同（SNR需折算）
            ber = 0.5 * erfc(sqrt(snr_linear / 2.0));
            break;
            
        case ReceiveModulationType::QAM16:
            // 16QAM精确公式（考虑Gray编码）
            if (snr_linear < 1e-3) {
                ber = 0.5; // 极低端保护
            } else {
                double term1 = 3.0 / 8.0 * erfc(sqrt(0.4 * snr_linear));
                double term2 = 2.0 / 8.0 * erfc(sqrt(1.2 * snr_linear));
                double term3 = 3.0 / 8.0 * erfc(sqrt(2.0 * snr_linear));
                ber = term1 + term2 + term3;
            }
            break;
            
        case ReceiveModulationType::FM:
            // 调频（FM）相干解调理论公式（假设频偏比β=5，语音通信典型值）
            // 参考：BER ≈ (1/(2*sqrt(π*SNR))) * exp(-SNR/2) （大SNR近似）
            // 低SNR时使用更精确的积分近似
            if (snr_linear < 2.0) {
                // 低SNR区域：考虑鉴频器噪声特性
                ber = 0.5 * exp(-0.5 * snr_linear);
            } else {
                // 高SNR区域：相干解调近似
                ber = (1.0 / (2.0 * sqrt(MathConstants::PI * snr_linear))) * exp(-snr_linear / 2.0);
            }
            break;
            
        case ReceiveModulationType::AM:
            // 调幅（AM）包络检波理论公式（假设调制深度m=1）
            // 参考：BER = 0.5 * [1 - sqrt(SNR/(SNR + 2))] （大SNR近似）
            if (snr_linear < 0.1) {
                ber = 0.5; // 极低端保护
            } else {
                double denominator = snr_linear + 2.0;
                ber = 0.5 * (1.0 - sqrt(snr_linear / denominator));
            }
            break;
            
        default:
            ber = 0.5; // 未知调制方式默认50%误码率
            break;
    }
    
    // 限制误码率范围（避免数值下溢或异常）
    const double min_ber = 1e-20;  // 理论最小误码率下限
    const double max_ber = 0.5;    // 最大可能误码率
    return std::clamp(ber, min_ber, max_ber);
}


/// @brief 计算信噪比
/// @details 信噪比 = 接收信号功率 - 系统噪声功率 (dB)
/// @return 信噪比(dB)
double CommunicationReceiveModel::calculateSignalToNoiseRatio() const {
    // 修正：移除重复的天线增益叠加，接收功率已包含天线增益的影响
    return receivedPower - noiseFloor;
}

/// @brief 计算有效噪声功率
/// @details 有效噪声功率 = 系统噪声功率 + 天线增益
/// @return 有效噪声功率(dBm)
double CommunicationReceiveModel::calculateEffectiveNoisePower() const {
    return noiseFloor + antennaGain;
}

/// @brief 计算最小可检测功率
/// @details 最小可检测功率 = 噪声底 + 检测门限(可调整)
/// @return 最小可检测功率(dBm)
double CommunicationReceiveModel::calculateMinimumDetectablePower() const {
    // 最小可检测功率 = 噪声底 + 检测门限(可通过setDetectionThreshold调整)
    return noiseFloor + detectionThreshold;
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
bool CommunicationReceiveModel::isSignalDecodable(double required_snr ) const {
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
    oss << "检测门限: " << detectionThreshold << " dB\n";
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