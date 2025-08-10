#include "CommunicationModelCAPI.h"
#include "CommunicationModelAPI.h"
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <exception>

// 内部辅助函数
namespace {
    // 将C++枚举转换为C枚举
    CommunicationScenario ToCppScenario(CommScenario scenario) {
        switch (scenario) {
            case SCENARIO_NORMAL_COMMUNICATION: return CommunicationScenario::NORMAL_COMMUNICATION;
            case SCENARIO_JAMMED_COMMUNICATION: return CommunicationScenario::JAMMED_COMMUNICATION;
            case SCENARIO_ANTI_JAM_COMMUNICATION: return CommunicationScenario::ANTI_JAM_COMMUNICATION;
            case SCENARIO_MULTI_USER_COMMUNICATION: return CommunicationScenario::MULTI_USER_COMMUNICATION;
            case SCENARIO_RELAY_COMMUNICATION: return CommunicationScenario::RELAY_COMMUNICATION;
            case SCENARIO_MESH_COMMUNICATION: return CommunicationScenario::MESH_COMMUNICATION;
            default: return CommunicationScenario::NORMAL_COMMUNICATION;
        }
    }

    CommScenario ToCScenario(CommunicationScenario scenario) {
        switch (scenario) {
            case CommunicationScenario::NORMAL_COMMUNICATION: return SCENARIO_NORMAL_COMMUNICATION;
            case CommunicationScenario::JAMMED_COMMUNICATION: return SCENARIO_JAMMED_COMMUNICATION;
            case CommunicationScenario::ANTI_JAM_COMMUNICATION: return SCENARIO_ANTI_JAM_COMMUNICATION;
            case CommunicationScenario::MULTI_USER_COMMUNICATION: return SCENARIO_MULTI_USER_COMMUNICATION;
            case CommunicationScenario::RELAY_COMMUNICATION: return SCENARIO_RELAY_COMMUNICATION;
            case CommunicationScenario::MESH_COMMUNICATION: return SCENARIO_MESH_COMMUNICATION;
            default: return SCENARIO_NORMAL_COMMUNICATION;
        }
    }

    EnvironmentType ToCppEnvironmentType(CommEnvironmentType envType) {
        switch (envType) {
            case ENV_FREE_SPACE: return EnvironmentType::OPEN_FIELD;
            case ENV_URBAN: return EnvironmentType::URBAN_AREA;
            case ENV_SUBURBAN: return EnvironmentType::URBAN_AREA;  // 郊区映射到城市区域
            case ENV_RURAL: return EnvironmentType::OPEN_FIELD;     // 农村映射到开阔地
            case ENV_INDOOR: return EnvironmentType::URBAN_AREA;    // 室内映射到城市区域
            case ENV_MARITIME: return EnvironmentType::OPEN_FIELD;  // 海上映射到开阔地
            case ENV_MOUNTAINOUS: return EnvironmentType::MOUNTAINOUS;
            default: return EnvironmentType::OPEN_FIELD;
        }
    }

    CommEnvironmentType ToCEnvironmentType(EnvironmentType envType) {
        switch (envType) {
            case EnvironmentType::OPEN_FIELD: return ENV_FREE_SPACE;
            case EnvironmentType::URBAN_AREA: return ENV_URBAN;
            case EnvironmentType::MOUNTAINOUS: return ENV_MOUNTAINOUS;
            default: return ENV_FREE_SPACE;
        }
    }

    JammerType ToCppJammerType(CommJammerType jammerType) {
        switch (jammerType) {
            case JAMMER_GAUSSIAN_NOISE: return JammerType::GAUSSIAN_NOISE;
            case JAMMER_NARROWBAND: return JammerType::NARROWBAND;
            case JAMMER_SWEEP: return JammerType::SWEEP_FREQUENCY;
            case JAMMER_PULSE: return JammerType::PULSE;
            case JAMMER_BARRAGE: return JammerType::BARRAGE;
            case JAMMER_SPOT: return JammerType::SPOT;
            default: return JammerType::GAUSSIAN_NOISE;
        }

    }
    CommJammerType ToCJammerType(JammerType jammerType) {
        switch (jammerType) {
            case JammerType::GAUSSIAN_NOISE: return JAMMER_GAUSSIAN_NOISE;
            case JammerType::NARROWBAND: return JAMMER_NARROWBAND;
            case JammerType::SWEEP_FREQUENCY: return JAMMER_SWEEP;
            case JammerType::PULSE: return JAMMER_PULSE;
            case JammerType::BARRAGE: return JAMMER_BARRAGE;
            case JammerType::SPOT: return JAMMER_SPOT;
            default: return JAMMER_GAUSSIAN_NOISE;
        }
    }

    CommQuality ToCQuality(CommunicationQuality quality) {
        switch (quality) {
            case CommunicationQuality::EXCELLENT: return QUALITY_EXCELLENT;
            case CommunicationQuality::GOOD: return QUALITY_GOOD;
            case CommunicationQuality::FAIR: return QUALITY_FAIR;
            case CommunicationQuality::POOR: return QUALITY_POOR;
            case CommunicationQuality::FAILED: return QUALITY_FAILED;
            default: return QUALITY_FAILED;
        }
    }

    // 结构体转换函数
    CommunicationEnvironment ToCppEnvironment(const CommEnvironment* env) {
        CommunicationEnvironment cppEnv;
        cppEnv.frequency = env->frequency;
        cppEnv.bandwidth = env->bandwidth;
        cppEnv.transmitPower = env->transmitPower;
        cppEnv.noisePower = env->noisePower;
        cppEnv.distance = env->distance;
        cppEnv.environmentType = ToCppEnvironmentType(env->environmentType);
        cppEnv.temperature = env->temperature;
        cppEnv.humidity = env->humidity;
        cppEnv.atmosphericPressure = env->atmosphericPressure;
        return cppEnv;
    }

    void ToCEnvironment(const CommunicationEnvironment& cppEnv, CommEnvironment* env) {
        env->frequency = cppEnv.frequency;
        env->bandwidth = cppEnv.bandwidth;
        env->transmitPower = cppEnv.transmitPower;
        env->noisePower = cppEnv.noisePower;
        env->distance = cppEnv.distance;
        env->environmentType = ToCEnvironmentType(cppEnv.environmentType);
        env->temperature = cppEnv.temperature;
        env->humidity = cppEnv.humidity;
        env->atmosphericPressure = cppEnv.atmosphericPressure;
    }

    JammingEnvironment ToCppJammingEnvironment(const CommJammingEnvironment* jammingEnv) {
        JammingEnvironment cppJammingEnv;
        cppJammingEnv.isJammed = jammingEnv->isJammed != 0;
        cppJammingEnv.jammerType = ToCppJammerType(jammingEnv->jammerType);
        cppJammingEnv.jammerPower = jammingEnv->jammerPower;
        cppJammingEnv.jammerFrequency = jammingEnv->jammerFrequency;
        cppJammingEnv.jammerBandwidth = jammingEnv->jammerBandwidth;
        cppJammingEnv.jammerDistance = jammingEnv->jammerDistance;
        cppJammingEnv.jammerDensity = jammingEnv->jammerDensity;
        
        // 转换频率数组
        if (jammingEnv->jammerFrequencies && jammingEnv->jammerFrequencyCount > 0) {
            cppJammingEnv.jammerFrequencies.resize(jammingEnv->jammerFrequencyCount);
            for (int i = 0; i < jammingEnv->jammerFrequencyCount; ++i) {
                cppJammingEnv.jammerFrequencies[i] = jammingEnv->jammerFrequencies[i];
            }
        }
        
        return cppJammingEnv;
    }

    void ToCJammingEnvironment(const JammingEnvironment& cppJammingEnv, CommJammingEnvironment* jammingEnv) {
        jammingEnv->isJammed = cppJammingEnv.isJammed ? 1 : 0;
        jammingEnv->jammerType = ToCJammerType(cppJammingEnv.jammerType);
        jammingEnv->jammerPower = cppJammingEnv.jammerPower;
        jammingEnv->jammerFrequency = cppJammingEnv.jammerFrequency;
        jammingEnv->jammerBandwidth = cppJammingEnv.jammerBandwidth;
        jammingEnv->jammerDistance = cppJammingEnv.jammerDistance;
        jammingEnv->jammerDensity = cppJammingEnv.jammerDensity;
        
        // 注意：频率数组需要单独分配和管理
        jammingEnv->jammerFrequencyCount = static_cast<int>(cppJammingEnv.jammerFrequencies.size());
    }

    void ToCLinkStatus(const CommunicationLinkStatus& cppStatus, CommLinkStatus* status) {
        status->isConnected = cppStatus.isConnected ? 1 : 0;
        status->signalStrength = cppStatus.signalStrength;
        status->signalToNoiseRatio = cppStatus.signalToNoiseRatio;
        status->bitErrorRate = cppStatus.bitErrorRate;
        status->throughput = cppStatus.throughput;
        status->latency = cppStatus.latency;
        status->packetLossRate = cppStatus.packetLossRate;
        status->quality = ToCQuality(cppStatus.quality);
        
        // 复制状态描述字符串
        strncpy(status->statusDescription, cppStatus.statusDescription.c_str(), 255);
        status->statusDescription[255] = '\0';
    }

    void ToCPerformance(const CommunicationPerformance& cppPerf, CommPerformance* perf) {
        perf->effectiveRange = cppPerf.effectiveRange;
        perf->maxDataRate = cppPerf.maxDataRate;
        perf->powerEfficiency = cppPerf.powerEfficiency;
        perf->spectralEfficiency = cppPerf.spectralEfficiency;
        perf->reliability = cppPerf.reliability;
        perf->availability = cppPerf.availability;
        perf->jammerResistance = cppPerf.jammerResistance;
        perf->interceptionResistance = cppPerf.interceptionResistance;
    }

    // 错误处理宏
    #define SAFE_CALL(call) \
        try { \
            call; \
            return COMM_SUCCESS; \
        } catch (const std::exception& e) { \
            return COMM_ERROR_CALCULATION_FAILED; \
        } catch (...) { \
            return COMM_ERROR_UNKNOWN; \
        }

    #define VALIDATE_HANDLE(handle) \
        if (!handle) return COMM_ERROR_INVALID_HANDLE; \
        CommunicationModelAPI* api = static_cast<CommunicationModelAPI*>(handle);

    #define VALIDATE_POINTER(ptr) \
        if (!ptr) return COMM_ERROR_NULL_POINTER;
}

extern "C" {

// ============================================================================
// 生命周期管理
// ============================================================================

COMMUNICATION_MODEL_API CommModelHandle COMMUNICATION_MODEL_CALL 
CommModel_Create(void) {
    try {
        auto* api = new CommunicationModelAPI();
        return static_cast<CommModelHandle>(api);
    } catch (...) {
        return nullptr;
    }
}

COMMUNICATION_MODEL_API CommModelHandle COMMUNICATION_MODEL_CALL 
CommModel_CreateWithScenario(CommScenario scenario) {
    try {
        auto* api = new CommunicationModelAPI(ToCppScenario(scenario));
        return static_cast<CommModelHandle>(api);
    } catch (...) {
        return nullptr;
    }
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_Destroy(CommModelHandle handle) {
    if (!handle) return COMM_ERROR_INVALID_HANDLE;
    
    try {
        CommunicationModelAPI* api = static_cast<CommunicationModelAPI*>(handle);
        delete api;
        return COMM_SUCCESS;
    } catch (...) {
        return COMM_ERROR_UNKNOWN;
    }
}

// ============================================================================
// 场景设置
// ============================================================================

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_SetScenario(CommModelHandle handle, CommScenario scenario) {
    VALIDATE_HANDLE(handle);
    SAFE_CALL(api->setScenario(ToCppScenario(scenario)));
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_GetScenario(CommModelHandle handle, CommScenario* scenario) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(scenario);
    
    SAFE_CALL(*scenario = ToCScenario(api->getScenario()));
}

// ============================================================================
// 环境参数设置
// ============================================================================

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_SetEnvironment(CommModelHandle handle, const CommEnvironment* env) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(env);
    
    SAFE_CALL(api->setEnvironment(ToCppEnvironment(env)));
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_GetEnvironment(CommModelHandle handle, CommEnvironment* env) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(env);
    
    SAFE_CALL(ToCEnvironment(api->getEnvironment(), env));
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_SetJammingEnvironment(CommModelHandle handle, const CommJammingEnvironment* jammingEnv) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(jammingEnv);
    
    SAFE_CALL(api->setJammingEnvironment(ToCppJammingEnvironment(jammingEnv)));
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_GetJammingEnvironment(CommModelHandle handle, CommJammingEnvironment* jammingEnv) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(jammingEnv);
    
    SAFE_CALL(ToCJammingEnvironment(api->getJammingEnvironment(), jammingEnv));
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_SetFrequency(CommModelHandle handle, double frequency) {
    VALIDATE_HANDLE(handle);
    SAFE_CALL(api->setFrequency(frequency));
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_SetBandwidth(CommModelHandle handle, double bandwidth) {
    VALIDATE_HANDLE(handle);
    SAFE_CALL(api->setBandwidth(bandwidth));
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_SetTransmitPower(CommModelHandle handle, double power) {
    VALIDATE_HANDLE(handle);
    SAFE_CALL(api->setTransmitPower(power));
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_SetDistance(CommModelHandle handle, double distance) {
    VALIDATE_HANDLE(handle);
    SAFE_CALL(api->setDistance(distance));
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_SetEnvironmentType(CommModelHandle handle, CommEnvironmentType envType) {
    VALIDATE_HANDLE(handle);
    SAFE_CALL(api->setEnvironmentType(ToCppEnvironmentType(envType)));
}

// ============================================================================
// 核心计算接口
// ============================================================================

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateLinkStatus(CommModelHandle handle, CommLinkStatus* linkStatus) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(linkStatus);
    
    SAFE_CALL(
        auto cppStatus = api->calculateLinkStatus();
        ToCLinkStatus(cppStatus, linkStatus);
    );
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculatePerformance(CommModelHandle handle, CommPerformance* performance) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(performance);
    
    SAFE_CALL(
        auto cppPerf = api->calculatePerformance();
        ToCPerformance(cppPerf, performance);
    );
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateCommunicationRange(CommModelHandle handle, double* range) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(range);
    
    SAFE_CALL(*range = api->calculateCommunicationRange());
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateRequiredPower(CommModelHandle handle, double targetRange, double* requiredPower) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(requiredPower);
    
    SAFE_CALL(*requiredPower = api->calculateRequiredPower(targetRange));
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateOptimalFrequency(CommModelHandle handle, double* optimalFreq) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(optimalFreq);
    
    SAFE_CALL(*optimalFreq = api->calculateOptimalFrequency());
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateOptimalBandwidth(CommModelHandle handle, double* optimalBandwidth) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(optimalBandwidth);
    
    SAFE_CALL(*optimalBandwidth = api->calculateOptimalBandwidth());
}

// ============================================================================
// 干扰分析接口
// ============================================================================

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateJammerEffectiveness(CommModelHandle handle, double* effectiveness) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(effectiveness);
    
    SAFE_CALL(*effectiveness = api->calculateJammerEffectiveness());
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateAntiJamEffectiveness(CommModelHandle handle, double* effectiveness) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(effectiveness);
    
    SAFE_CALL(*effectiveness = api->calculateAntiJamEffectiveness());
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateJammerToSignalRatio(CommModelHandle handle, double* jsRatio) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(jsRatio);
    
    SAFE_CALL(*jsRatio = api->calculateJammerToSignalRatio());
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateRequiredAntiJamGain(CommModelHandle handle, double targetBER, double* requiredGain) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(requiredGain);
    
    SAFE_CALL(*requiredGain = api->calculateRequiredAntiJamGain(targetBER));
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateJammerCoverage(CommModelHandle handle, DoubleArray* coverage) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(coverage);
    
    SAFE_CALL(
        auto cppCoverage = api->calculateJammerCoverage();
        coverage->count = static_cast<int>(cppCoverage.size());
        if (coverage->count > 0) {
            coverage->values = new double[coverage->count];
            for (int i = 0; i < coverage->count; ++i) {
                coverage->values[i] = cppCoverage[i];
            }
        } else {
            coverage->values = nullptr;
        }
    );
}

// ============================================================================
// 性能优化接口
// ============================================================================

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_OptimizeForRange(CommModelHandle handle, double targetRange, CommEnvironment* optimizedEnv) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(optimizedEnv);
    
    SAFE_CALL(
        auto cppEnv = api->optimizeForRange(targetRange);
        ToCEnvironment(cppEnv, optimizedEnv);
    );
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_OptimizeForDataRate(CommModelHandle handle, double targetDataRate, CommEnvironment* optimizedEnv) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(optimizedEnv);
    
    SAFE_CALL(
        auto cppEnv = api->optimizeForDataRate(targetDataRate);
        ToCEnvironment(cppEnv, optimizedEnv);
    );
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_OptimizeForPowerEfficiency(CommModelHandle handle, CommEnvironment* optimizedEnv) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(optimizedEnv);
    
    SAFE_CALL(
        auto cppEnv = api->optimizeForPowerEfficiency();
        ToCEnvironment(cppEnv, optimizedEnv);
    );
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_OptimizeForJammerResistance(CommModelHandle handle, CommEnvironment* optimizedEnv) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(optimizedEnv);
    
    SAFE_CALL(
        auto cppEnv = api->optimizeForJammerResistance();
        ToCEnvironment(cppEnv, optimizedEnv);
    );
}

// ============================================================================
// 报告生成
// ============================================================================

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_GenerateDetailedReport(CommModelHandle handle, char* report, int bufferSize) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(report);
    
    if (bufferSize <= 0) return COMM_ERROR_INVALID_PARAMETER;
    
    SAFE_CALL(
        std::string cppReport = api->generateDetailedReport();
        strncpy(report, cppReport.c_str(), bufferSize - 1);
        report[bufferSize - 1] = '\0';
    );
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_GeneratePerformanceReport(CommModelHandle handle, char* report, int bufferSize) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(report);
    
    if (bufferSize <= 0) return COMM_ERROR_INVALID_PARAMETER;
    
    SAFE_CALL(
        std::string cppReport = api->generatePerformanceReport();
        strncpy(report, cppReport.c_str(), bufferSize - 1);
        report[bufferSize - 1] = '\0';
    );
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_GenerateJammingAnalysisReport(CommModelHandle handle, char* report, int bufferSize) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(report);
    
    if (bufferSize <= 0) return COMM_ERROR_INVALID_PARAMETER;
    
    SAFE_CALL(
        std::string cppReport = api->generateJammingAnalysisReport();
        strncpy(report, cppReport.c_str(), bufferSize - 1);
        report[bufferSize - 1] = '\0';
    );
}

// ============================================================================
// 配置管理
// ============================================================================

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_SaveConfiguration(CommModelHandle handle, const char* filename) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(filename);
    
    SAFE_CALL(api->saveConfiguration(std::string(filename)));
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_LoadConfiguration(CommModelHandle handle, const char* filename) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(filename);
    
    SAFE_CALL(api->loadConfiguration(std::string(filename)));
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_ExportConfigurationToJSON(CommModelHandle handle, char* jsonStr, int bufferSize) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(jsonStr);
    
    if (bufferSize <= 0) return COMM_ERROR_INVALID_PARAMETER;
    
    SAFE_CALL(
        std::string cppJson = api->exportConfigurationToJSON();
        strncpy(jsonStr, cppJson.c_str(), bufferSize - 1);
        jsonStr[bufferSize - 1] = '\0';
    );
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_ImportConfigurationFromJSON(CommModelHandle handle, const char* jsonStr) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(jsonStr);
    
    SAFE_CALL(api->importConfigurationFromJSON(std::string(jsonStr)));
}

// ============================================================================
// 版本和信息
// ============================================================================

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_GetVersion(char* version, int bufferSize) {
    VALIDATE_POINTER(version);
    
    if (bufferSize <= 0) return COMM_ERROR_INVALID_PARAMETER;
    
    SAFE_CALL(
        std::string cppVersion = CommunicationModelAPI::getVersion();
        strncpy(version, cppVersion.c_str(), bufferSize - 1);
        version[bufferSize - 1] = '\0';
    );
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_GetBuildInfo(char* buildInfo, int bufferSize) {
    VALIDATE_POINTER(buildInfo);
    
    if (bufferSize <= 0) return COMM_ERROR_INVALID_PARAMETER;
    
    SAFE_CALL(
        std::string cppBuildInfo = CommunicationModelAPI::getBuildInfo();
        strncpy(buildInfo, cppBuildInfo.c_str(), bufferSize - 1);
        buildInfo[bufferSize - 1] = '\0';
    );
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_GetModelInfo(CommModelHandle handle, char* modelInfo, int bufferSize) {
    VALIDATE_HANDLE(handle);
    VALIDATE_POINTER(modelInfo);
    
    if (bufferSize <= 0) return COMM_ERROR_INVALID_PARAMETER;
    
    SAFE_CALL(
        std::string cppModelInfo = api->getModelInfo();
        strncpy(modelInfo, cppModelInfo.c_str(), bufferSize - 1);
        modelInfo[bufferSize - 1] = '\0';
    );
}

// ============================================================================
// 内存管理辅助函数
// ============================================================================

COMMUNICATION_MODEL_API DoubleArray COMMUNICATION_MODEL_CALL 
CommModel_AllocateDoubleArray(int count) {
    DoubleArray array;
    if (count > 0) {
        try {
            array.values = new double[count];
            array.count = count;
        } catch (...) {
            array.values = nullptr;
            array.count = 0;
        }
    } else {
        array.values = nullptr;
        array.count = 0;
    }
    return array;
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_FreeDoubleArray(DoubleArray* array) {
    if (!array) return COMM_ERROR_NULL_POINTER;
    
    try {
        if (array->values) {
            delete[] array->values;
            array->values = nullptr;
        }
        array->count = 0;
        return COMM_SUCCESS;
    } catch (...) {
        return COMM_ERROR_UNKNOWN;
    }
}

COMMUNICATION_MODEL_API LinkStatusArray COMMUNICATION_MODEL_CALL 
CommModel_AllocateLinkStatusArray(int count) {
    LinkStatusArray array;
    if (count > 0) {
        try {
            array.statuses = new CommLinkStatus[count];
            array.count = count;
        } catch (...) {
            array.statuses = nullptr;
            array.count = 0;
        }
    } else {
        array.statuses = nullptr;
        array.count = 0;
    }
    return array;
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_FreeLinkStatusArray(LinkStatusArray* array) {
    if (!array) return COMM_ERROR_NULL_POINTER;
    
    try {
        if (array->statuses) {
            delete[] array->statuses;
            array->statuses = nullptr;
        }
        array->count = 0;
        return COMM_SUCCESS;
    } catch (...) {
        return COMM_ERROR_UNKNOWN;
    }
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_AllocateJammerFrequencies(CommJammingEnvironment* jammingEnv, int count) {
    if (!jammingEnv) return COMM_ERROR_NULL_POINTER;
    
    try {
        if (jammingEnv->jammerFrequencies) {
            delete[] jammingEnv->jammerFrequencies;
        }
        
        if (count > 0) {
            jammingEnv->jammerFrequencies = new double[count];
            jammingEnv->jammerFrequencyCount = count;
        } else {
            jammingEnv->jammerFrequencies = nullptr;
            jammingEnv->jammerFrequencyCount = 0;
        }
        
        return COMM_SUCCESS;
    } catch (...) {
        return COMM_ERROR_MEMORY_ALLOCATION;
    }
}

COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_FreeJammerFrequencies(CommJammingEnvironment* jammingEnv) {
    if (!jammingEnv) return COMM_ERROR_NULL_POINTER;
    
    try {
        if (jammingEnv->jammerFrequencies) {
            delete[] jammingEnv->jammerFrequencies;
            jammingEnv->jammerFrequencies = nullptr;
        }
        jammingEnv->jammerFrequencyCount = 0;
        return COMM_SUCCESS;
    } catch (...) {
        return COMM_ERROR_UNKNOWN;
    }
}

} // extern "C"