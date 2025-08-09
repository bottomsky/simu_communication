#ifndef COMMUNICATION_ANTI_JAM_PARAMETER_CONFIG_H
#define COMMUNICATION_ANTI_JAM_PARAMETER_CONFIG_H

#include <string>

/**
 * @file CommunicationAntiJamParameterConfig.h
 * @brief 通信抗干扰模型参数配置类
 * 
 * 该类负责管理通信抗干扰模型的所有参数范围定义和校验逻辑，
 * 实现参数配置的统一管理和代码的模块化。
 */
class CommunicationAntiJamParameterConfig {
public:
    // 处理增益参数范围 (dB)
    static constexpr double MIN_PROCESSING_GAIN = 0.0;
    static constexpr double MAX_PROCESSING_GAIN = 50.0;
    
    // 扩频因子参数范围
    static constexpr double MIN_SPREADING_FACTOR = 1.0;
    static constexpr double MAX_SPREADING_FACTOR = 100000.0;
    
    // 跳频/跳时速率参数范围 (Hz)
    static constexpr double MIN_HOPPING_RATE = 1.0;
    static constexpr double MAX_HOPPING_RATE = 100000.0;
    
    // 编码增益参数范围 (dB)
    static constexpr double MIN_CODING_GAIN = 0.0;
    static constexpr double MAX_CODING_GAIN = 20.0;
    
    // 系统带宽参数范围 (MHz)
    static constexpr double MIN_SYSTEM_BANDWIDTH = 0.1;
    static constexpr double MAX_SYSTEM_BANDWIDTH = 10000.0;
    
    // 信号功率参数范围 (dBm)
    static constexpr double MIN_SIGNAL_POWER = -150.0;
    static constexpr double MAX_SIGNAL_POWER = 50.0;
    
    // 噪声功率参数范围 (dBm)
    static constexpr double MIN_NOISE_POWER = -150.0;
    static constexpr double MAX_NOISE_POWER = 0.0;
    
    // 干扰电平参数范围 (dBm)
    static constexpr double MIN_INTERFERENCE_LEVEL = -150.0;
    static constexpr double MAX_INTERFERENCE_LEVEL = 50.0;
    
    // 跳频信道数参数范围
    static constexpr int MIN_HOPPING_CHANNELS = 2;
    static constexpr int MAX_HOPPING_CHANNELS = 10000;
    
    // 信道间隔参数范围 (MHz)
    static constexpr double MIN_CHANNEL_SPACING = 0.001;
    static constexpr double MAX_CHANNEL_SPACING = 1000.0;
    
    // 驻留时间参数范围 (ms)
    static constexpr double MIN_DWELL_TIME = 0.001;
    static constexpr double MAX_DWELL_TIME = 1000.0;
    
    // 码片速率参数范围 (Mcps)
    static constexpr int MIN_CHIP_RATE = 1;
    static constexpr int MAX_CHIP_RATE = 1000;
    
    // 序列长度参数范围
    static constexpr double MIN_SEQUENCE_LENGTH = 7.0;
    static constexpr double MAX_SEQUENCE_LENGTH = 1000000.0;
    
    // 自适应速度参数范围
    static constexpr double MIN_ADAPTATION_SPEED = 0.001;
    static constexpr double MAX_ADAPTATION_SPEED = 1.0;
    
    // 收敛阈值参数范围
    static constexpr double MIN_CONVERGENCE_THRESHOLD = 0.0001;
    static constexpr double MAX_CONVERGENCE_THRESHOLD = 0.1;
    
    // 环境类型参数范围 (0-1)
    static constexpr double MIN_ENVIRONMENT_TYPE = 0.0;
    static constexpr double MAX_ENVIRONMENT_TYPE = 1.0;
    
    // 干扰机密度参数范围 (0-1)
    static constexpr double MIN_JAMMER_DENSITY = 0.0;
    static constexpr double MAX_JAMMER_DENSITY = 1.0;
    
    // 参数校验方法
    static bool isProcessingGainValid(double gain);
    static bool isSpreadingFactorValid(double factor);
    static bool isHoppingRateValid(double rate);
    static bool isCodingGainValid(double gain);
    static bool isSystemBandwidthValid(double bandwidth);
    static bool isSignalPowerValid(double power);
    static bool isNoisePowerValid(double power);
    static bool isInterferenceLevelValid(double level);
    static bool isHoppingChannelsValid(int channels);
    static bool isChannelSpacingValid(double spacing);
    static bool isDwellTimeValid(double time);
    static bool isChipRateValid(int rate);
    static bool isSequenceLengthValid(double length);
    static bool isAdaptationSpeedValid(double speed);
    static bool isConvergenceThresholdValid(double threshold);
    static bool isEnvironmentTypeValid(double type);
    static bool isJammerDensityValid(double density);
    
    // 参数范围信息获取方法
    static std::string getParameterRangeInfo();
    static std::string getProcessingGainRange();
    static std::string getSpreadingFactorRange();
    static std::string getHoppingRateRange();
    static std::string getCodingGainRange();
    static std::string getSystemBandwidthRange();
    static std::string getSignalPowerRange();
    static std::string getNoisePowerRange();
    static std::string getInterferenceLevelRange();
    static std::string getHoppingChannelsRange();
    static std::string getChannelSpacingRange();
    static std::string getDwellTimeRange();
    static std::string getChipRateRange();
    static std::string getSequenceLengthRange();
    static std::string getAdaptationSpeedRange();
    static std::string getConvergenceThresholdRange();
    static std::string getEnvironmentTypeRange();
    static std::string getJammerDensityRange();
};

#endif // COMMUNICATION_ANTI_JAM_PARAMETER_CONFIG_H