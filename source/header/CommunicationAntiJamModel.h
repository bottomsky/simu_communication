#ifndef COMMUNICATION_ANTI_JAM_MODEL_H
#define COMMUNICATION_ANTI_JAM_MODEL_H

#include <string>
#include <vector>
#include <map>

/**
 * @brief 抗干扰技术类型枚举
 */
enum class AntiJamTechnique {
    FREQUENCY_HOPPING,      // 跳频
    DIRECT_SEQUENCE,        // 直接序列扩频
    TIME_HOPPING,          // 跳时
    HYBRID_SPREAD,         // 混合扩频
    ADAPTIVE_FILTERING,    // 自适应滤波
    BEAM_FORMING,          // 波束成形
    POWER_CONTROL,         // 功率控制
    ERROR_CORRECTION,      // 纠错编码
    DIVERSITY_RECEPTION,   // 分集接收
    INTERFERENCE_CANCELLATION  // 干扰抵消
};

/**
 * @brief 抗干扰策略枚举
 */
enum class AntiJamStrategy {
    PASSIVE,               // 被动抗干扰
    ACTIVE,                // 主动抗干扰
    ADAPTIVE,              // 自适应抗干扰
    COOPERATIVE,           // 协作抗干扰
    COGNITIVE              // 认知抗干扰
};

/**
 * @brief 抗干扰效果等级枚举
 */
enum class AntiJamEffectLevel {
    NO_PROTECTION = 0,     // 无保护
    LOW_PROTECTION = 1,    // 低保护
    MEDIUM_PROTECTION = 2, // 中等保护
    HIGH_PROTECTION = 3,   // 高保护
    EXCELLENT_PROTECTION = 4  // 优秀保护
};

/**
 * @brief 通信抗干扰模型类
 * 
 * 该类实现了通信系统的抗干扰能力建模，包括各种抗干扰技术的效果评估、
 * 抗干扰性能计算、最优抗干扰策略选择等功能。
 */
class CommunicationAntiJamModel {
private:
    // 基本抗干扰参数
    AntiJamTechnique antiJamTechnique_;      // 抗干扰技术类型
    AntiJamStrategy antiJamStrategy_;        // 抗干扰策略
    double processingGain_;                  // 处理增益 (dB)
    double spreadingFactor_;                 // 扩频因子
    double hoppingRate_;                     // 跳频/跳时速率 (Hz)
    double codingGain_;                      // 编码增益 (dB)
    
    // 系统参数
    double systemBandwidth_;                 // 系统带宽 (MHz)
    double signalPower_;                     // 信号功率 (dBm)
    double noisePower_;                      // 噪声功率 (dBm)
    double interferenceLevel_;               // 干扰电平 (dBm)
    
    // 跳频特有参数
    int hoppingChannels_;                    // 跳频信道数
    double channelSpacing_;                  // 信道间隔 (MHz)
    double dwellTime_;                       // 驻留时间 (ms)
    
    // 扩频特有参数
    int chipRate_;                           // 码片速率 (Mcps)
    double sequenceLength_;                  // 序列长度
    
    // 自适应参数
    double adaptationSpeed_;                 // 自适应速度
    double convergenceThreshold_;            // 收敛阈值
    
    // 环境参数
    double environmentType_;                 // 环境类型 (0-1)
    double jammerDensity_;                   // 干扰机密度
    
    // 内部计算方法
    bool validateParameters() const;
    double calculateFrequencyHoppingGain() const;
    double calculateDirectSequenceGain() const;
    double calculateTimeHoppingGain() const;
    double calculateAdaptiveFilteringGain() const;
    double calculateBeamFormingGain() const;
    double calculateDiversityGain() const;
    double calculateErrorCorrectionGain() const;
    double calculateInterferenceCancellationGain() const;
    double calculateTotalProcessingGain() const;
    
public:
    // 构造函数
    CommunicationAntiJamModel();
    CommunicationAntiJamModel(AntiJamTechnique technique, 
                             AntiJamStrategy strategy,
                             double processingGain = 20.0,
                             double spreadingFactor = 1000.0,
                             double hoppingRate = 1000.0,
                             double codingGain = 3.0);
    
    // 参数设置方法
    bool setAntiJamTechnique(AntiJamTechnique technique);
    bool setAntiJamStrategy(AntiJamStrategy strategy);
    bool setProcessingGain(double gain);
    bool setSpreadingFactor(double factor);
    bool setHoppingRate(double rate);
    bool setCodingGain(double gain);
    bool setSystemBandwidth(double bandwidth);
    bool setSignalPower(double power);
    bool setNoisePower(double power);
    bool setInterferenceLevel(double level);
    
    // 跳频参数设置
    bool setHoppingChannels(int channels);
    bool setChannelSpacing(double spacing);
    bool setDwellTime(double time);
    
    // 扩频参数设置
    bool setChipRate(int rate);
    bool setSequenceLength(double length);
    
    // 自适应参数设置
    bool setAdaptationSpeed(double speed);
    bool setConvergenceThreshold(double threshold);
    
    // 环境参数设置
    bool setEnvironmentType(double type);
    bool setJammerDensity(double density);
    
    // 参数获取方法
    AntiJamTechnique getAntiJamTechnique() const { return antiJamTechnique_; }
    AntiJamStrategy getAntiJamStrategy() const { return antiJamStrategy_; }
    double getProcessingGain() const { return processingGain_; }
    double getSpreadingFactor() const { return spreadingFactor_; }
    double getHoppingRate() const { return hoppingRate_; }
    double getCodingGain() const { return codingGain_; }
    double getSystemBandwidth() const { return systemBandwidth_; }
    double getSignalPower() const { return signalPower_; }
    double getNoisePower() const { return noisePower_; }
    double getInterferenceLevel() const { return interferenceLevel_; }
    int getHoppingChannels() const { return hoppingChannels_; }
    double getChannelSpacing() const { return channelSpacing_; }
    double getDwellTime() const { return dwellTime_; }
    int getChipRate() const { return chipRate_; }
    double getSequenceLength() const { return sequenceLength_; }
    double getAdaptationSpeed() const { return adaptationSpeed_; }
    double getConvergenceThreshold() const { return convergenceThreshold_; }
    double getEnvironmentType() const { return environmentType_; }
    double getJammerDensity() const { return jammerDensity_; }
    
    // 核心计算方法
    double calculateAntiJamGain() const;                    // 计算抗干扰增益
    double calculateJammerResistance() const;               // 计算抗干扰能力
    double calculateSignalToJammerRatio() const;            // 计算信干比
    double calculateBitErrorRateWithJamming() const;        // 计算有干扰时误码率
    double calculateThroughputDegradation() const;          // 计算吞吐量下降
    double calculateDetectionProbability() const;           // 计算检测概率
    double calculateInterceptionResistance() const;         // 计算抗截获能力
    
    // 抗干扰效果评估
    AntiJamEffectLevel evaluateAntiJamEffect() const;       // 评估抗干扰效果等级
    double calculateProtectionEffectiveness() const;        // 计算保护有效性
    double calculateAdaptationEfficiency() const;           // 计算自适应效率
    double calculateResourceUtilization() const;            // 计算资源利用率
    
    // 特定技术效果计算
    double calculateFrequencyHoppingEffectiveness() const;  // 跳频效果
    double calculateSpreadSpectrumEffectiveness() const;    // 扩频效果
    double calculateAdaptiveFilteringEffectiveness() const; // 自适应滤波效果
    double calculateBeamFormingEffectiveness() const;       // 波束成形效果
    double calculateDiversityEffectiveness() const;         // 分集效果
    double calculateErrorCorrectionEffectiveness() const;   // 纠错效果
    
    // 最优策略计算
    AntiJamTechnique calculateOptimalTechnique() const;     // 计算最优抗干扰技术
    double calculateOptimalProcessingGain() const;          // 计算最优处理增益
    double calculateOptimalHoppingRate() const;             // 计算最优跳频速率
    int calculateOptimalHoppingChannels() const;            // 计算最优跳频信道数
    
    // 多技术组合效果
    double calculateCombinedTechniqueEffect(const std::vector<AntiJamTechnique>& techniques) const;
    std::vector<AntiJamTechnique> getRecommendedTechniqueCombination() const;
    
    // 性能预测
    double predictPerformanceUnderJamming(double jammerPower, double jammerBandwidth) const;
    double calculateRequiredAntiJamGain(double targetBER) const;
    double calculateMaxTolerableJammerPower() const;
    
    // 信息输出方法
    std::string getParameterInfo() const;                   // 获取参数信息
    std::string getAntiJamEffectInfo() const;              // 获取抗干扰效果信息
    std::string getRecommendationInfo() const;             // 获取建议信息
    std::string getTechniqueComparisonInfo() const;        // 获取技术对比信息
    
    // 调试和测试方法
    void printAllParameters() const;
    void printAntiJamAnalysis() const;
    bool runSelfTest() const;
};

#endif // COMMUNICATION_ANTI_JAM_MODEL_H