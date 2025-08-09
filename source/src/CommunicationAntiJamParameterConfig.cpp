#include "../header/CommunicationAntiJamParameterConfig.h"
#include <sstream>
#include <iomanip>
#include <string>

// 处理增益校验实现
bool CommunicationAntiJamParameterConfig::isProcessingGainValid(double gain) {
    return gain >= MIN_PROCESSING_GAIN && gain <= MAX_PROCESSING_GAIN;
}

// 扩频因子校验实现
bool CommunicationAntiJamParameterConfig::isSpreadingFactorValid(double factor) {
    return factor >= MIN_SPREADING_FACTOR && factor <= MAX_SPREADING_FACTOR;
}

// 跳频/跳时速率校验实现
bool CommunicationAntiJamParameterConfig::isHoppingRateValid(double rate) {
    return rate >= MIN_HOPPING_RATE && rate <= MAX_HOPPING_RATE;
}

// 编码增益校验实现
bool CommunicationAntiJamParameterConfig::isCodingGainValid(double gain) {
    return gain >= MIN_CODING_GAIN && gain <= MAX_CODING_GAIN;
}

// 系统带宽校验实现
bool CommunicationAntiJamParameterConfig::isSystemBandwidthValid(double bandwidth) {
    return bandwidth >= MIN_SYSTEM_BANDWIDTH && bandwidth <= MAX_SYSTEM_BANDWIDTH;
}

// 信号功率校验实现
bool CommunicationAntiJamParameterConfig::isSignalPowerValid(double power) {
    return power >= MIN_SIGNAL_POWER && power <= MAX_SIGNAL_POWER;
}

// 噪声功率校验实现
bool CommunicationAntiJamParameterConfig::isNoisePowerValid(double power) {
    return power >= MIN_NOISE_POWER && power <= MAX_NOISE_POWER;
}

// 干扰电平校验实现
bool CommunicationAntiJamParameterConfig::isInterferenceLevelValid(double level) {
    return level >= MIN_INTERFERENCE_LEVEL && level <= MAX_INTERFERENCE_LEVEL;
}

// 跳频信道数校验实现
bool CommunicationAntiJamParameterConfig::isHoppingChannelsValid(int channels) {
    return channels >= MIN_HOPPING_CHANNELS && channels <= MAX_HOPPING_CHANNELS;
}

// 信道间隔校验实现
bool CommunicationAntiJamParameterConfig::isChannelSpacingValid(double spacing) {
    return spacing >= MIN_CHANNEL_SPACING && spacing <= MAX_CHANNEL_SPACING;
}

// 驻留时间校验实现
bool CommunicationAntiJamParameterConfig::isDwellTimeValid(double time) {
    return time >= MIN_DWELL_TIME && time <= MAX_DWELL_TIME;
}

// 码片速率校验实现
bool CommunicationAntiJamParameterConfig::isChipRateValid(int rate) {
    return rate >= MIN_CHIP_RATE && rate <= MAX_CHIP_RATE;
}

// 序列长度校验实现
bool CommunicationAntiJamParameterConfig::isSequenceLengthValid(double length) {
    return length >= MIN_SEQUENCE_LENGTH && length <= MAX_SEQUENCE_LENGTH;
}

// 自适应速度校验实现
bool CommunicationAntiJamParameterConfig::isAdaptationSpeedValid(double speed) {
    return speed >= MIN_ADAPTATION_SPEED && speed <= MAX_ADAPTATION_SPEED;
}

// 收敛阈值校验实现
bool CommunicationAntiJamParameterConfig::isConvergenceThresholdValid(double threshold) {
    return threshold >= MIN_CONVERGENCE_THRESHOLD && threshold <= MAX_CONVERGENCE_THRESHOLD;
}

// 环境类型校验实现
bool CommunicationAntiJamParameterConfig::isEnvironmentTypeValid(double type) {
    return type >= MIN_ENVIRONMENT_TYPE && type <= MAX_ENVIRONMENT_TYPE;
}

// 干扰机密度校验实现
bool CommunicationAntiJamParameterConfig::isJammerDensityValid(double density) {
    return density >= MIN_JAMMER_DENSITY && density <= MAX_JAMMER_DENSITY;
}

// 参数范围信息获取方法实现
std::string CommunicationAntiJamParameterConfig::getParameterRangeInfo() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    
    oss << "=== 通信抗干扰模型参数范围配置 ===\n";
    oss << "1. 处理增益: " << MIN_PROCESSING_GAIN << " ~ " << MAX_PROCESSING_GAIN << " dB\n";
    oss << "2. 扩频因子: " << MIN_SPREADING_FACTOR << " ~ " << MAX_SPREADING_FACTOR << "\n";
    oss << "3. 跳频/跳时速率: " << MIN_HOPPING_RATE << " ~ " << MAX_HOPPING_RATE << " Hz\n";
    oss << "4. 编码增益: " << MIN_CODING_GAIN << " ~ " << MAX_CODING_GAIN << " dB\n";
    oss << "5. 系统带宽: " << MIN_SYSTEM_BANDWIDTH << " ~ " << MAX_SYSTEM_BANDWIDTH << " MHz\n";
    oss << "6. 信号功率: " << MIN_SIGNAL_POWER << " ~ " << MAX_SIGNAL_POWER << " dBm\n";
    oss << "7. 噪声功率: " << MIN_NOISE_POWER << " ~ " << MAX_NOISE_POWER << " dBm\n";
    oss << "8. 干扰电平: " << MIN_INTERFERENCE_LEVEL << " ~ " << MAX_INTERFERENCE_LEVEL << " dBm\n";
    oss << "9. 跳频信道数: " << MIN_HOPPING_CHANNELS << " ~ " << MAX_HOPPING_CHANNELS << "\n";
    oss << "10. 信道间隔: " << MIN_CHANNEL_SPACING << " ~ " << MAX_CHANNEL_SPACING << " MHz\n";
    oss << "11. 驻留时间: " << MIN_DWELL_TIME << " ~ " << MAX_DWELL_TIME << " ms\n";
    oss << "12. 码片速率: " << MIN_CHIP_RATE << " ~ " << MAX_CHIP_RATE << " Mcps\n";
    oss << "13. 序列长度: " << MIN_SEQUENCE_LENGTH << " ~ " << MAX_SEQUENCE_LENGTH << "\n";
    oss << "14. 自适应速度: " << MIN_ADAPTATION_SPEED << " ~ " << MAX_ADAPTATION_SPEED << "\n";
    oss << "15. 收敛阈值: " << MIN_CONVERGENCE_THRESHOLD << " ~ " << MAX_CONVERGENCE_THRESHOLD << "\n";
    oss << "16. 环境类型: " << MIN_ENVIRONMENT_TYPE << " ~ " << MAX_ENVIRONMENT_TYPE << "\n";
    oss << "17. 干扰机密度: " << MIN_JAMMER_DENSITY << " ~ " << MAX_JAMMER_DENSITY;
    
    return oss.str();
}

std::string CommunicationAntiJamParameterConfig::getProcessingGainRange() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "处理增益范围: " << MIN_PROCESSING_GAIN << " ~ " << MAX_PROCESSING_GAIN << " dB";
    return oss.str();
}

std::string CommunicationAntiJamParameterConfig::getSpreadingFactorRange() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "扩频因子范围: " << MIN_SPREADING_FACTOR << " ~ " << MAX_SPREADING_FACTOR;
    return oss.str();
}

std::string CommunicationAntiJamParameterConfig::getHoppingRateRange() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "跳频/跳时速率范围: " << MIN_HOPPING_RATE << " ~ " << MAX_HOPPING_RATE << " Hz";
    return oss.str();
}

std::string CommunicationAntiJamParameterConfig::getCodingGainRange() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "编码增益范围: " << MIN_CODING_GAIN << " ~ " << MAX_CODING_GAIN << " dB";
    return oss.str();
}

std::string CommunicationAntiJamParameterConfig::getSystemBandwidthRange() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "系统带宽范围: " << MIN_SYSTEM_BANDWIDTH << " ~ " << MAX_SYSTEM_BANDWIDTH << " MHz";
    return oss.str();
}

std::string CommunicationAntiJamParameterConfig::getSignalPowerRange() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "信号功率范围: " << MIN_SIGNAL_POWER << " ~ " << MAX_SIGNAL_POWER << " dBm";
    return oss.str();
}

std::string CommunicationAntiJamParameterConfig::getNoisePowerRange() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "噪声功率范围: " << MIN_NOISE_POWER << " ~ " << MAX_NOISE_POWER << " dBm";
    return oss.str();
}

std::string CommunicationAntiJamParameterConfig::getInterferenceLevelRange() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "干扰电平范围: " << MIN_INTERFERENCE_LEVEL << " ~ " << MAX_INTERFERENCE_LEVEL << " dBm";
    return oss.str();
}

std::string CommunicationAntiJamParameterConfig::getHoppingChannelsRange() {
    std::ostringstream oss;
    oss << "跳频信道数范围: " << MIN_HOPPING_CHANNELS << " ~ " << MAX_HOPPING_CHANNELS;
    return oss.str();
}

std::string CommunicationAntiJamParameterConfig::getChannelSpacingRange() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    oss << "信道间隔范围: " << MIN_CHANNEL_SPACING << " ~ " << MAX_CHANNEL_SPACING << " MHz";
    return oss.str();
}

std::string CommunicationAntiJamParameterConfig::getDwellTimeRange() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    oss << "驻留时间范围: " << MIN_DWELL_TIME << " ~ " << MAX_DWELL_TIME << " ms";
    return oss.str();
}

std::string CommunicationAntiJamParameterConfig::getChipRateRange() {
    std::ostringstream oss;
    oss << "码片速率范围: " << MIN_CHIP_RATE << " ~ " << MAX_CHIP_RATE << " Mcps";
    return oss.str();
}

std::string CommunicationAntiJamParameterConfig::getSequenceLengthRange() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "序列长度范围: " << MIN_SEQUENCE_LENGTH << " ~ " << MAX_SEQUENCE_LENGTH;
    return oss.str();
}

std::string CommunicationAntiJamParameterConfig::getAdaptationSpeedRange() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    oss << "自适应速度范围: " << MIN_ADAPTATION_SPEED << " ~ " << MAX_ADAPTATION_SPEED;
    return oss.str();
}

std::string CommunicationAntiJamParameterConfig::getConvergenceThresholdRange() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(4);
    oss << "收敛阈值范围: " << MIN_CONVERGENCE_THRESHOLD << " ~ " << MAX_CONVERGENCE_THRESHOLD;
    return oss.str();
}

std::string CommunicationAntiJamParameterConfig::getEnvironmentTypeRange() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "环境类型范围: " << MIN_ENVIRONMENT_TYPE << " ~ " << MAX_ENVIRONMENT_TYPE;
    return oss.str();
}

std::string CommunicationAntiJamParameterConfig::getJammerDensityRange() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "干扰机密度范围: " << MIN_JAMMER_DENSITY << " ~ " << MAX_JAMMER_DENSITY;
    return oss.str();
}