#ifndef COMMUNICATION_MODEL_C_API_H
#define COMMUNICATION_MODEL_C_API_H

#include "CommunicationModelExport.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// 生命周期管理
// ============================================================================

/**
 * @brief 创建通信模型实例
 * @return 模型句柄，失败返回NULL
 */
COMMUNICATION_MODEL_API CommModelHandle COMMUNICATION_MODEL_CALL 
CommModel_Create(void);

/**
 * @brief 创建带场景的通信模型实例
 * @param scenario 通信场景
 * @return 模型句柄，失败返回NULL
 */
COMMUNICATION_MODEL_API CommModelHandle COMMUNICATION_MODEL_CALL 
CommModel_CreateWithScenario(CommScenario scenario);

/**
 * @brief 销毁通信模型实例
 * @param handle 模型句柄
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_Destroy(CommModelHandle handle);

// ============================================================================
// 场景设置
// ============================================================================

/**
 * @brief 设置通信场景
 * @param handle 模型句柄
 * @param scenario 通信场景
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_SetScenario(CommModelHandle handle, CommScenario scenario);

/**
 * @brief 获取当前通信场景
 * @param handle 模型句柄
 * @param scenario 输出参数，当前场景
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_GetScenario(CommModelHandle handle, CommScenario* scenario);

// ============================================================================
// 环境参数设置
// ============================================================================

/**
 * @brief 设置通信环境参数
 * @param handle 模型句柄
 * @param env 环境参数
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_SetEnvironment(CommModelHandle handle, const CommEnvironment* env);

/**
 * @brief 获取通信环境参数
 * @param handle 模型句柄
 * @param env 输出参数，环境参数
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_GetEnvironment(CommModelHandle handle, CommEnvironment* env);

/**
 * @brief 设置干扰环境参数
 * @param handle 模型句柄
 * @param jammingEnv 干扰环境参数
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_SetJammingEnvironment(CommModelHandle handle, const CommJammingEnvironment* jammingEnv);

/**
 * @brief 获取干扰环境参数
 * @param handle 模型句柄
 * @param jammingEnv 输出参数，干扰环境参数
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_GetJammingEnvironment(CommModelHandle handle, CommJammingEnvironment* jammingEnv);

/**
 * @brief 设置工作频率
 * @param handle 模型句柄
 * @param frequency 频率 (MHz)
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_SetFrequency(CommModelHandle handle, double frequency);

/**
 * @brief 设置系统带宽
 * @param handle 模型句柄
 * @param bandwidth 带宽 (MHz)
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_SetBandwidth(CommModelHandle handle, double bandwidth);

/**
 * @brief 设置发射功率
 * @param handle 模型句柄
 * @param power 功率 (dBm)
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_SetTransmitPower(CommModelHandle handle, double power);

/**
 * @brief 设置通信距离
 * @param handle 模型句柄
 * @param distance 距离 (km)
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_SetDistance(CommModelHandle handle, double distance);

/**
 * @brief 设置环境类型
 * @param handle 模型句柄
 * @param envType 环境类型
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_SetEnvironmentType(CommModelHandle handle, CommEnvironmentType envType);

// ============================================================================
// 核心计算接口
// ============================================================================

/**
 * @brief 计算链路状态
 * @param handle 模型句柄
 * @param linkStatus 输出参数，链路状态
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateLinkStatus(CommModelHandle handle, CommLinkStatus* linkStatus);

/**
 * @brief 计算性能指标
 * @param handle 模型句柄
 * @param performance 输出参数，性能指标
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculatePerformance(CommModelHandle handle, CommPerformance* performance);

/**
 * @brief 计算通信距离
 * @param handle 模型句柄
 * @param range 输出参数，通信距离 (km)
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateCommunicationRange(CommModelHandle handle, double* range);

/**
 * @brief 计算所需功率
 * @param handle 模型句柄
 * @param targetRange 目标距离 (km)
 * @param requiredPower 输出参数，所需功率 (dBm)
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateRequiredPower(CommModelHandle handle, double targetRange, double* requiredPower);

/**
 * @brief 计算最优频率
 * @param handle 模型句柄
 * @param optimalFreq 输出参数，最优频率 (MHz)
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateOptimalFrequency(CommModelHandle handle, double* optimalFreq);

/**
 * @brief 计算最优带宽
 * @param handle 模型句柄
 * @param optimalBandwidth 输出参数，最优带宽 (MHz)
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateOptimalBandwidth(CommModelHandle handle, double* optimalBandwidth);

// ============================================================================
// 干扰分析接口
// ============================================================================

/**
 * @brief 计算干扰有效性
 * @param handle 模型句柄
 * @param effectiveness 输出参数，干扰有效性
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateJammerEffectiveness(CommModelHandle handle, double* effectiveness);

/**
 * @brief 计算抗干扰有效性
 * @param handle 模型句柄
 * @param effectiveness 输出参数，抗干扰有效性
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateAntiJamEffectiveness(CommModelHandle handle, double* effectiveness);

/**
 * @brief 计算干信比
 * @param handle 模型句柄
 * @param jsRatio 输出参数，干信比 (dB)
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateJammerToSignalRatio(CommModelHandle handle, double* jsRatio);

/**
 * @brief 计算所需抗干扰增益
 * @param handle 模型句柄
 * @param targetBER 目标误码率
 * @param requiredGain 输出参数，所需增益 (dB)
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateRequiredAntiJamGain(CommModelHandle handle, double targetBER, double* requiredGain);

/**
 * @brief 计算干扰覆盖范围
 * @param handle 模型句柄
 * @param coverage 输出参数，覆盖范围数组
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_CalculateJammerCoverage(CommModelHandle handle, DoubleArray* coverage);

// ============================================================================
// 性能优化接口
// ============================================================================

/**
 * @brief 针对距离优化
 * @param handle 模型句柄
 * @param targetRange 目标距离 (km)
 * @param optimizedEnv 输出参数，优化后的环境参数
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_OptimizeForRange(CommModelHandle handle, double targetRange, CommEnvironment* optimizedEnv);

/**
 * @brief 针对数据速率优化
 * @param handle 模型句柄
 * @param targetDataRate 目标数据速率 (Mbps)
 * @param optimizedEnv 输出参数，优化后的环境参数
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_OptimizeForDataRate(CommModelHandle handle, double targetDataRate, CommEnvironment* optimizedEnv);

/**
 * @brief 针对功率效率优化
 * @param handle 模型句柄
 * @param optimizedEnv 输出参数，优化后的环境参数
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_OptimizeForPowerEfficiency(CommModelHandle handle, CommEnvironment* optimizedEnv);

/**
 * @brief 针对抗干扰能力优化
 * @param handle 模型句柄
 * @param optimizedEnv 输出参数，优化后的环境参数
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_OptimizeForJammerResistance(CommModelHandle handle, CommEnvironment* optimizedEnv);

// ============================================================================
// 多场景分析
// ============================================================================

/**
 * @brief 分析多个场景
 * @param handle 模型句柄
 * @param scenarios 场景数组
 * @param scenarioCount 场景数量
 * @param results 输出参数，分析结果数组
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_AnalyzeMultipleScenarios(CommModelHandle handle, 
                                   const CommScenario* scenarios, 
                                   int scenarioCount,
                                   LinkStatusArray* results);

/**
 * @brief 分析频率范围
 * @param handle 模型句柄
 * @param startFreq 起始频率 (MHz)
 * @param endFreq 结束频率 (MHz)
 * @param step 步长 (MHz)
 * @param frequencies 输出参数，频率数组
 * @param results 输出参数，结果数组
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_AnalyzeFrequencyRange(CommModelHandle handle, 
                                double startFreq, 
                                double endFreq, 
                                double step,
                                DoubleArray* frequencies,
                                LinkStatusArray* results);

/**
 * @brief 分析功率范围
 * @param handle 模型句柄
 * @param startPower 起始功率 (dBm)
 * @param endPower 结束功率 (dBm)
 * @param step 步长 (dBm)
 * @param powers 输出参数，功率数组
 * @param results 输出参数，结果数组
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_AnalyzePowerRange(CommModelHandle handle, 
                            double startPower, 
                            double endPower, 
                            double step,
                            DoubleArray* powers,
                            LinkStatusArray* results);

/**
 * @brief 分析距离范围
 * @param handle 模型句柄
 * @param startDistance 起始距离 (km)
 * @param endDistance 结束距离 (km)
 * @param step 步长 (km)
 * @param distances 输出参数，距离数组
 * @param results 输出参数，结果数组
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_AnalyzeDistanceRange(CommModelHandle handle, 
                               double startDistance, 
                               double endDistance, 
                               double step,
                               DoubleArray* distances,
                               LinkStatusArray* results);

// ============================================================================
// 报告生成
// ============================================================================

/**
 * @brief 生成详细报告
 * @param handle 模型句柄
 * @param report 输出参数，报告字符串缓冲区
 * @param bufferSize 缓冲区大小
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_GenerateDetailedReport(CommModelHandle handle, char* report, int bufferSize);

/**
 * @brief 生成性能报告
 * @param handle 模型句柄
 * @param report 输出参数，报告字符串缓冲区
 * @param bufferSize 缓冲区大小
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_GeneratePerformanceReport(CommModelHandle handle, char* report, int bufferSize);

/**
 * @brief 生成干扰分析报告
 * @param handle 模型句柄
 * @param report 输出参数，报告字符串缓冲区
 * @param bufferSize 缓冲区大小
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_GenerateJammingAnalysisReport(CommModelHandle handle, char* report, int bufferSize);

// ============================================================================
// 配置管理
// ============================================================================

/**
 * @brief 保存配置到文件
 * @param handle 模型句柄
 * @param filename 文件名
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_SaveConfiguration(CommModelHandle handle, const char* filename);

/**
 * @brief 从文件加载配置
 * @param handle 模型句柄
 * @param filename 文件名
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_LoadConfiguration(CommModelHandle handle, const char* filename);

/**
 * @brief 导出配置为JSON
 * @param handle 模型句柄
 * @param jsonStr 输出参数，JSON字符串缓冲区
 * @param bufferSize 缓冲区大小
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_ExportConfigurationToJSON(CommModelHandle handle, char* jsonStr, int bufferSize);

/**
 * @brief 从JSON导入配置
 * @param handle 模型句柄
 * @param jsonStr JSON字符串
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_ImportConfigurationFromJSON(CommModelHandle handle, const char* jsonStr);

// ============================================================================
// 版本和信息
// ============================================================================

/**
 * @brief 获取版本信息
 * @param version 输出参数，版本字符串缓冲区
 * @param bufferSize 缓冲区大小
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_GetVersion(char* version, int bufferSize);

/**
 * @brief 获取构建信息
 * @param buildInfo 输出参数，构建信息字符串缓冲区
 * @param bufferSize 缓冲区大小
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_GetBuildInfo(char* buildInfo, int bufferSize);

/**
 * @brief 获取模型信息
 * @param handle 模型句柄
 * @param modelInfo 输出参数，模型信息字符串缓冲区
 * @param bufferSize 缓冲区大小
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_GetModelInfo(CommModelHandle handle, char* modelInfo, int bufferSize);

// ============================================================================
// 内存管理辅助函数
// ============================================================================

/**
 * @brief 分配双精度数组
 * @param count 数组大小
 * @return 数组结构体
 */
COMMUNICATION_MODEL_API DoubleArray COMMUNICATION_MODEL_CALL 
CommModel_AllocateDoubleArray(int count);

/**
 * @brief 释放双精度数组
 * @param array 数组结构体
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_FreeDoubleArray(DoubleArray* array);

/**
 * @brief 分配链路状态数组
 * @param count 数组大小
 * @return 数组结构体
 */
COMMUNICATION_MODEL_API LinkStatusArray COMMUNICATION_MODEL_CALL 
CommModel_AllocateLinkStatusArray(int count);

/**
 * @brief 释放链路状态数组
 * @param array 数组结构体
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_FreeLinkStatusArray(LinkStatusArray* array);

/**
 * @brief 分配干扰频率数组
 * @param jammingEnv 干扰环境结构体
 * @param count 数组大小
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_AllocateJammerFrequencies(CommJammingEnvironment* jammingEnv, int count);

/**
 * @brief 释放干扰频率数组
 * @param jammingEnv 干扰环境结构体
 * @return 操作结果
 */
COMMUNICATION_MODEL_API CommResult COMMUNICATION_MODEL_CALL 
CommModel_FreeJammerFrequencies(CommJammingEnvironment* jammingEnv);

#ifdef __cplusplus
}
#endif

#endif // COMMUNICATION_MODEL_C_API_H