#ifndef COMMUNICATION_MODEL_EXPORT_H
#define COMMUNICATION_MODEL_EXPORT_H

// 跨平台导出宏定义
#ifdef COMMUNICATION_MODEL_STATIC
    // 静态库构建：不使用任何导入/导出修饰符
    #define COMMUNICATION_MODEL_API
    #define COMMUNICATION_MODEL_CALL
#elif defined(_WIN32)
    #ifdef COMMUNICATION_MODEL_EXPORTS
        #define COMMUNICATION_MODEL_API __declspec(dllexport)
    #else
        #define COMMUNICATION_MODEL_API __declspec(dllimport)
    #endif
    #define COMMUNICATION_MODEL_CALL __stdcall
#else
    #ifdef COMMUNICATION_MODEL_EXPORTS
        #define COMMUNICATION_MODEL_API __attribute__((visibility("default")))
    #else
        #define COMMUNICATION_MODEL_API
    #endif
    #define COMMUNICATION_MODEL_CALL
#endif

// C语言兼容性
#ifdef __cplusplus
extern "C" {
#endif

// 错误代码定义
typedef enum {
    COMM_SUCCESS = 0,
    COMM_ERROR_INVALID_PARAMETER = -1,
    COMM_ERROR_NULL_POINTER = -2,
    COMM_ERROR_MEMORY_ALLOCATION = -3,
    COMM_ERROR_INVALID_HANDLE = -4,
    COMM_ERROR_CALCULATION_FAILED = -5,
    COMM_ERROR_NOT_INITIALIZED = -6,
    COMM_ERROR_UNSUPPORTED_OPERATION = -7,
    COMM_ERROR_FILE_IO = -8,
    COMM_ERROR_UNKNOWN = -999
} CommResult;

// 句柄类型定义
typedef void* CommModelHandle;

// 枚举类型的C风格定义
typedef enum {
    SCENARIO_NORMAL_COMMUNICATION = 0,
    SCENARIO_JAMMED_COMMUNICATION = 1,
    SCENARIO_ANTI_JAM_COMMUNICATION = 2,
    SCENARIO_MULTI_USER_COMMUNICATION = 3,
    SCENARIO_RELAY_COMMUNICATION = 4,
    SCENARIO_MESH_COMMUNICATION = 5
} CommScenario;

typedef enum {
    QUALITY_EXCELLENT = 5,
    QUALITY_GOOD = 4,
    QUALITY_FAIR = 3,
    QUALITY_POOR = 2,
    QUALITY_FAILED = 1
} CommQuality;

typedef enum {
    ENV_FREE_SPACE = 0,
    ENV_URBAN = 1,
    ENV_SUBURBAN = 2,
    ENV_RURAL = 3,
    ENV_INDOOR = 4,
    ENV_MARITIME = 5,
    ENV_MOUNTAINOUS = 6
} CommEnvironmentType;

typedef enum {
    JAMMER_GAUSSIAN_NOISE = 0, // 高斯白噪声干扰
    JAMMER_NARROWBAND = 1,
    JAMMER_SWEEP = 2,
    JAMMER_PULSE = 3,
    JAMMER_BARRAGE = 4,
    JAMMER_SPOT = 5

} CommJammerType;

// 结构体的C风格定义
typedef struct {
    int isConnected;                    // 是否连接 (0/1)
    double signalStrength;              // 信号强度 (dBm)
    double signalToNoiseRatio;          // 信噪比 (dB)
    double bitErrorRate;                // 误码率
    double throughput;                  // 吞吐量 (Mbps)
    double latency;                     // 延迟 (ms)
    double packetLossRate;              // 丢包率
    CommQuality quality;                // 通信质量
    char statusDescription[256];        // 状态描述
} CommLinkStatus;

typedef struct {
    double effectiveRange;              // 有效通信距离 (km)
    double maxDataRate;                 // 最大数据速率 (Mbps)
    double powerEfficiency;             // 功率效率
    double spectralEfficiency;          // 频谱效率
    double reliability;                 // 可靠性 (0-1)
    double availability;                // 可用性 (0-1)
    double jammerResistance;            // 抗干扰能力 (0-1)
    double interceptionResistance;      // 抗截获能力 (0-1)
} CommPerformance;

typedef struct {
    double frequency;                   // 工作频率 (MHz)
    double bandwidth;                   // 系统带宽 (MHz)
    double transmitPower;               // 发射功率 (dBm)
    double noisePower;                  // 噪声功率 (dBm)
    double distance;                    // 通信距离 (km)
    CommEnvironmentType environmentType; // 环境类型
    double temperature;                 // 环境温度 (°C)
    double humidity;                    // 湿度 (%)
    double atmosphericPressure;         // 大气压力 (kPa)
} CommEnvironment;

typedef struct {
    int isJammed;                       // 是否存在干扰 (0/1)
    CommJammerType jammerType;          // 干扰类型
    double jammerPower;                 // 干扰功率 (dBm)
    double jammerFrequency;             // 干扰频率 (MHz)
    double jammerBandwidth;             // 干扰带宽 (MHz)
    double jammerDistance;              // 干扰机距离 (km)
    double jammerDensity;               // 干扰机密度 (0-1)
    double* jammerFrequencies;          // 多个干扰频率数组
    int jammerFrequencyCount;           // 干扰频率数组长度
} CommJammingEnvironment;

// 数组结构体定义
typedef struct {
    double* values;
    int count;
} DoubleArray;

typedef struct {
    CommLinkStatus* statuses;
    int count;
} LinkStatusArray;

#ifdef __cplusplus
}
#endif

#endif // COMMUNICATION_MODEL_EXPORT_H