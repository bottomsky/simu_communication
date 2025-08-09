#ifndef COMMUNICATION_MODEL_API_H
#define COMMUNICATION_MODEL_API_H

#include "SignalTransmissionModel.h"
#include "CommunicationDistanceModel.h"
#include "CommunicationReceiveModel.h"
#include "CommunicationJammerModel.h"
#include "CommunicationAntiJamModel.h"
#include "EnvironmentLossConfigManager.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <unordered_map>

/**
 * @brief 通信场景类型枚举
 */
enum class CommunicationScenario {
    NORMAL_COMMUNICATION,      // 正常通信
    JAMMED_COMMUNICATION,      // 受干扰通信
    ANTI_JAM_COMMUNICATION,    // 抗干扰通信
    MULTI_USER_COMMUNICATION,  // 多用户通信
    RELAY_COMMUNICATION,       // 中继通信
    MESH_COMMUNICATION         // 网状通信
};

/**
 * @brief 通信质量等级枚举
 */
enum class CommunicationQuality {
    EXCELLENT = 5,             // 优秀
    GOOD = 4,                  // 良好
    FAIR = 3,                  // 一般
    POOR = 2,                  // 较差
    FAILED = 1                 // 失败
};

/**
 * @brief 通信链路状态结构体
 */
struct CommunicationLinkStatus {
    bool isConnected;                    // 是否连接
    double signalStrength;               // 信号强度 (dBm)
    double signalToNoiseRatio;           // 信噪比 (dB)
    double bitErrorRate;                 // 误码率
    double throughput;                   // 吞吐量 (Mbps)
    double latency;                      // 延迟 (ms)
    double packetLossRate;               // 丢包率
    CommunicationQuality quality;        // 通信质量
    std::string statusDescription;       // 状态描述
};

/**
 * @brief 通信性能指标结构体
 */
struct CommunicationPerformance {
    double effectiveRange;               // 有效通信距离 (km)
    double maxDataRate;                  // 最大数据速率 (Mbps)
    double powerEfficiency;              // 功率效率
    double spectralEfficiency;           // 频谱效率
    double reliability;                  // 可靠性 (0-1)
    double availability;                 // 可用性 (0-1)
    double jammerResistance;             // 抗干扰能力 (0-1)
    double interceptionResistance;       // 抗截获能力 (0-1)
};

/**
 * @brief 通信环境参数结构体
 */
struct CommunicationEnvironment {
    double frequency;                    // 工作频率 (MHz)
    double bandwidth;                    // 系统带宽 (MHz)
    double transmitPower;                // 发射功率 (dBm)
    double noisePower;                   // 噪声功率 (dBm)
    double distance;                     // 通信距离 (km)
    EnvironmentType environmentType;     // 环境类型
    double temperature;                  // 环境温度 (°C)
    double humidity;                     // 湿度 (%)
    double atmosphericPressure;          // 大气压力 (kPa)
};

/**
 * @brief 干扰环境参数结构体
 */
struct JammingEnvironment {
    bool isJammed;                       // 是否存在干扰
    JammerType jammerType;               // 干扰类型
    double jammerPower;                  // 干扰功率 (dBm)
    double jammerFrequency;              // 干扰频率 (MHz)
    double jammerBandwidth;              // 干扰带宽 (MHz)
    double jammerDistance;               // 干扰机距离 (km)
    double jammerDensity;                // 干扰机密度 (0-1)
    std::vector<double> jammerFrequencies; // 多个干扰频率
};

/**
 * @brief 通信模型API类
 * 
 * 该类提供了统一的通信系统建模接口，集成了信号传输、通信距离、
 * 接收机、干扰机和抗干扰等多个模型，支持复杂通信场景的分析和优化。
 */
class CommunicationModelAPI {
private:
    // 核心模型实例
    std::unique_ptr<SignalTransmissionModel> signalModel_;
    std::unique_ptr<CommunicationDistanceModel> distanceModel_;
    std::unique_ptr<CommunicationReceiveModel> receiveModel_;
    std::unique_ptr<CommunicationJammerModel> jammerModel_;
    std::unique_ptr<CommunicationAntiJamModel> antiJamModel_;
    
    // 当前通信场景
    CommunicationScenario currentScenario_;
    
    // 环境参数
    CommunicationEnvironment environment_;
    JammingEnvironment jammingEnv_;
    
    // 缓存的计算结果
    mutable bool resultsValid_;
    mutable CommunicationLinkStatus cachedLinkStatus_;
    mutable CommunicationPerformance cachedPerformance_;
    
    // 内部计算方法
    void invalidateCache();
    void updateModelsFromEnvironment();
    double calculateOverallSignalStrength() const;
    double calculateOverallSNR() const;
    double calculateOverallBER() const;
    double calculateThroughput() const;
    double calculateLatency() const;
    double calculatePacketLossRate() const;
    CommunicationQuality assessCommunicationQuality() const;
    
public:
    // 构造函数和析构函数
    CommunicationModelAPI();
    explicit CommunicationModelAPI(CommunicationScenario scenario);
    ~CommunicationModelAPI();
    
    // 禁用拷贝构造和赋值
    CommunicationModelAPI(const CommunicationModelAPI&) = delete;
    CommunicationModelAPI& operator=(const CommunicationModelAPI&) = delete;
    
    // 移动构造和赋值
    CommunicationModelAPI(CommunicationModelAPI&&) = default;
    CommunicationModelAPI& operator=(CommunicationModelAPI&&) = default;
    
    // 场景设置
    bool setScenario(CommunicationScenario scenario);
    CommunicationScenario getScenario() const { return currentScenario_; }
    
    // 环境参数设置
    bool setEnvironment(const CommunicationEnvironment& env);
    bool setJammingEnvironment(const JammingEnvironment& jammingEnv);
    bool setFrequency(double frequency);
    bool setBandwidth(double bandwidth);
    bool setTransmitPower(double power);
    bool setDistance(double distance);
    bool setEnvironmentType(EnvironmentType type);
    
    // 环境参数获取
    CommunicationEnvironment getEnvironment() const { return environment_; }
    JammingEnvironment getJammingEnvironment() const { return jammingEnv_; }
    
    // 核心计算接口
    CommunicationLinkStatus calculateLinkStatus() const;
    CommunicationPerformance calculatePerformance() const;
    double calculateCommunicationRange() const;
    double calculateRequiredPower(double targetRange) const;
    double calculateOptimalFrequency() const;
    double calculateOptimalBandwidth() const;
    
    // 干扰分析接口
    double calculateJammerEffectiveness() const;
    double calculateAntiJamEffectiveness() const;
    double calculateJammerToSignalRatio() const;
    double calculateRequiredAntiJamGain(double targetBER) const;
    std::vector<double> calculateJammerCoverage() const;
    
    // 性能优化接口
    CommunicationEnvironment optimizeForRange(double targetRange) const;
    CommunicationEnvironment optimizeForDataRate(double targetDataRate) const;
    CommunicationEnvironment optimizeForPowerEfficiency() const;
    CommunicationEnvironment optimizeForJammerResistance() const;
    
    // 多场景分析
    std::vector<CommunicationLinkStatus> analyzeMultipleScenarios(
        const std::vector<CommunicationScenario>& scenarios) const;
    std::map<double, CommunicationLinkStatus> analyzeFrequencyRange(
        double startFreq, double endFreq, double step) const;
    std::map<double, CommunicationLinkStatus> analyzePowerRange(
        double startPower, double endPower, double step) const;
    std::map<double, CommunicationLinkStatus> analyzeDistanceRange(
        double startDistance, double endDistance, double step) const;
    
    // 网络拓扑分析
    std::vector<std::vector<double>> calculateLinkMatrix(
        const std::vector<std::pair<double, double>>& nodePositions) const;
    std::vector<int> findOptimalRelayPositions(
        const std::pair<double, double>& source,
        const std::pair<double, double>& destination,
        int maxRelays) const;
    double calculateNetworkConnectivity(
        const std::vector<std::pair<double, double>>& nodePositions) const;
    
    // 实时监控接口
    bool startRealTimeMonitoring();
    bool stopRealTimeMonitoring();
    bool isMonitoring() const;
    CommunicationLinkStatus getCurrentStatus() const;
    std::vector<CommunicationLinkStatus> getStatusHistory(int count = 100) const;
    
    // 预测和仿真
    CommunicationLinkStatus predictFutureStatus(double timeSeconds) const;
    std::vector<CommunicationLinkStatus> simulateTimeVaryingChannel(
        double duration, double timeStep) const;
    std::vector<CommunicationLinkStatus> simulateMobilityScenario(
        const std::vector<std::pair<double, double>>& trajectory,
        double timeStep) const;
    
    // 配置管理
    bool saveConfiguration(const std::string& filename) const;
    bool loadConfiguration(const std::string& filename);
    std::string exportConfigurationToJSON() const;
    bool importConfigurationFromJSON(const std::string& jsonStr);
    
    // 报告生成
    std::string generateDetailedReport() const;
    std::string generatePerformanceReport() const;
    std::string generateJammingAnalysisReport() const;
    std::string generateOptimizationReport() const;
    std::string generateComparisonReport(const CommunicationModelAPI& other) const;
    
    // 可视化数据接口
    std::vector<std::pair<double, double>> getCoverageContour(double threshold = 0.9) const;
    std::vector<std::pair<double, double>> getSignalStrengthMap(
        double xMin, double xMax, double yMin, double yMax, int resolution = 50) const;
    std::vector<std::pair<double, double>> getInterferenceMap(
        double xMin, double xMax, double yMin, double yMax, int resolution = 50) const;
    
    // 校准和验证
    bool calibrateWithMeasurements(const std::vector<std::pair<CommunicationEnvironment, 
                                                              CommunicationLinkStatus>>& measurements);
    double validateAgainstMeasurements(const std::vector<std::pair<CommunicationEnvironment, 
                                                                  CommunicationLinkStatus>>& measurements) const;
    std::string getValidationReport() const;
    
    // 错误处理和诊断
    std::vector<std::string> getLastErrors() const;
    std::vector<std::string> getWarnings() const;
    bool runDiagnostics() const;
    std::string getDiagnosticReport() const;
    
    // 版本和信息
    static std::string getVersion();
    static std::string getBuildInfo();
    std::string getModelInfo() const;
    std::string getCapabilities() const;
    
    // 调试和测试接口
    void enableDebugMode(bool enable = true);
    bool isDebugModeEnabled() const;
    void setLogLevel(int level);
    std::string getDebugInfo() const;
    bool runSelfTest() const;
    
    // 高级功能
    bool enableGPUAcceleration(bool enable = true);
    bool isGPUAccelerationEnabled() const;
    void setThreadCount(int count);
    int getThreadCount() const;
    
    // 插件和扩展接口
    bool loadPlugin(const std::string& pluginPath);
    std::vector<std::string> getLoadedPlugins() const;
    bool unloadPlugin(const std::string& pluginName);
    
    // 获取底层模型的直接访问（高级用户）
    SignalTransmissionModel* getSignalModel() const { return signalModel_.get(); }
    CommunicationDistanceModel* getDistanceModel() const { return distanceModel_.get(); }
    CommunicationReceiveModel* getReceiveModel() const { return receiveModel_.get(); }
    CommunicationJammerModel* getJammerModel() const { return jammerModel_.get(); }
    CommunicationAntiJamModel* getAntiJamModel() const { return antiJamModel_.get(); }
};

// 便利函数
namespace CommunicationModelUtils {
    // 快速计算函数
    double quickCalculateRange(double frequency, double power, EnvironmentType env);
    double quickCalculatePower(double frequency, double range, EnvironmentType env);
    CommunicationQuality quickAssessQuality(double snr, double ber);
    
    // 单位转换函数
    double dBmToWatts(double dBm);
    double wattsTodBm(double watts);
    double dBToLinear(double dB);
    double linearTodB(double linear);
    double frequencyToWavelength(double frequency);
    double wavelengthToFrequency(double wavelength);
    
    // 环境建模辅助函数
    double calculateAtmosphericLoss(double frequency, double distance, double humidity);
    double calculateRainLoss(double frequency, double distance, double rainRate);
    double calculateFoliageLoss(double frequency, double distance, double foliageDensity);
    double calculateUrbanLoss(double frequency, double distance, double buildingDensity);
    
    // 统计分析函数
    double calculateMean(const std::vector<double>& values);
    double calculateStandardDeviation(const std::vector<double>& values);
    double calculatePercentile(const std::vector<double>& values, double percentile);
    std::pair<double, double> calculateConfidenceInterval(const std::vector<double>& values, double confidence);
}

#endif // COMMUNICATION_MODEL_API_H