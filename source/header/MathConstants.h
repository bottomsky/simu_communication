#ifndef MATH_CONSTANTS_H
#define MATH_CONSTANTS_H

/**
 * @file MathConstants.h
 * @brief 数学常量和计算系数定义
 * @details 统一管理项目中使用的数学常量、物理常量和计算系数
 * @author Signal Transmission Model Team
 * @date 2024
 */

namespace MathConstants {

    // ==================== 基础数学常量 ====================
    
    /// @brief 零值常量
    constexpr double ZERO = 0.0;
    
    /// @brief 单位值常量
    constexpr double UNITY = 1.0;
    
    /// @brief 圆周率 π
    constexpr double PI = 3.14159265358979323846;
    
    /// @brief 自然对数底数 e
    constexpr double E = 2.71828182845904523536;
    
    /// @brief 2的平方根
    constexpr double SQRT_2 = 1.41421356237309504880;
    
    /// @brief 10的自然对数
    constexpr double LN_10 = 2.30258509299404568402;

    // ==================== 物理常量 ====================
    
    /// @brief 光速 (m/s)
    constexpr double SPEED_OF_LIGHT = 299792458.0;
    
    /// @brief 光速 (km/s) - 用于延迟计算
    constexpr double SPEED_OF_LIGHT_KM_S = 299792.458;
    
    /// @brief 光速 (m/ms) - 用于毫秒级延迟计算
    constexpr double SPEED_OF_LIGHT_M_MS = 299792458.0 / 1000.0;
    
    /// @brief 频率-波长转换常数 (MHz·m)
    /// @details wavelength(m) = FREQ_WAVELENGTH_CONSTANT / frequency(MHz)
    constexpr double FREQ_WAVELENGTH_CONSTANT = 300.0;

    // ==================== 自由空间路径损耗常量 ====================
    
    /// @brief 自由空间路径损耗公式常数 32.45
    /// @details FSPL(dB) = 20*log10(d_km) + 20*log10(f_MHz) + FSPL_CONSTANT
    constexpr double FSPL_CONSTANT = 32.45;
    
    /// @brief 自由空间路径损耗距离系数 20.0
    constexpr double FSPL_DISTANCE_COEFFICIENT = 20.0;
    
    /// @brief 自由空间路径损耗频率系数 20.0
    constexpr double FSPL_FREQUENCY_COEFFICIENT = 20.0;

    // ==================== 单位转换常量 ====================
    
    /// @brief 兆倍数转换常量 1e6
    constexpr double MEGA_MULTIPLIER = 1e6;
    
    /// @brief dBm到瓦特转换偏移量 30.0
    /// @details P_watts = 10^((P_dBm - DBM_TO_WATTS_OFFSET) / 10)
    constexpr double DBM_TO_WATTS_OFFSET = 30.0;
    
    /// @brief dB到线性转换除数 10.0
    /// @details linear = 10^(dB / DB_TO_LINEAR_DIVISOR)
    constexpr double DB_TO_LINEAR_DIVISOR = 10.0;
    
    /// @brief 线性到dB转换乘数 10.0
    /// @details dB = LINEAR_TO_DB_MULTIPLIER * log10(linear)
    constexpr double LINEAR_TO_DB_MULTIPLIER = 10.0;

    // ==================== 环境损耗计算系数 ====================
    
    /// @brief 环境损耗到衰减系数转换除数 10.0
    /// @details attenuation = 1.0 + (environmentLoss / ENV_LOSS_TO_ATTENUATION_DIVISOR)
    constexpr double ENV_LOSS_TO_ATTENUATION_DIVISOR = 10.0;
    
    /// @brief 自由空间路径损耗指数基准值 2.0
    /// @details 用于计算环境路径损耗: (pathLossExponent - FREE_SPACE_EXPONENT)
    constexpr double FREE_SPACE_EXPONENT = 2.0;
    
    /// @brief 频率因子损耗乘数 2.0
    /// @details frequencyLoss = frequencyFactor * log10(freq/1000) * FREQ_FACTOR_MULTIPLIER
    constexpr double FREQ_FACTOR_MULTIPLIER = 2.0;

    // ==================== 大气损耗模型系数 ====================
    
    /// @brief 氧气吸收系数1 (低频段) 0.0067
    constexpr double OXYGEN_ABSORPTION_COEFF_1 = 0.0067;
    
    /// @brief 氧气吸收系数2 (高频段) 0.067
    constexpr double OXYGEN_ABSORPTION_COEFF_2 = 0.067;
    
    /// @brief 水蒸气吸收系数 0.05
    constexpr double WATER_VAPOR_ABSORPTION_COEFF = 0.05;
    
    /// @brief 水蒸气密度转换系数 0.1
    constexpr double WATER_VAPOR_DENSITY_FACTOR = 0.1;

    // ==================== 雨衰模型系数 ====================
    
    /// @brief 雨衰系数K值 (低频段)
    constexpr double RAIN_K_LOW_FREQ = 0.0001;
    
    /// @brief 雨衰系数K值 (中频段基数)
    constexpr double RAIN_K_MID_FREQ_BASE = 0.001;
    
    /// @brief 雨衰系数K值 (高频段基数)
    constexpr double RAIN_K_HIGH_FREQ_BASE = 0.01;
    
    /// @brief 雨衰指数α值 (低频段)
    constexpr double RAIN_ALPHA_LOW_FREQ = 0.5;
    
    /// @brief 雨衰指数α值 (中频段)
    constexpr double RAIN_ALPHA_MID_FREQ = 1.0;
    
    /// @brief 雨衰指数α值 (高频段)
    constexpr double RAIN_ALPHA_HIGH_FREQ = 1.2;

    // ==================== 植被损耗模型系数 ====================
    
    /// @brief 植被损耗基础系数 0.2
    constexpr double FOLIAGE_LOSS_BASE_COEFF = 0.2;
    
    /// @brief 植被损耗频率指数 0.3
    constexpr double FOLIAGE_FREQ_EXPONENT = 0.3;

    // ==================== 城市环境损耗系数 ====================
    
    /// @brief 城市环境修正系数 3.2
    constexpr double URBAN_CORRECTION_COEFF = 3.2;
    
    /// @brief 城市环境修正偏移 -4.97
    constexpr double URBAN_CORRECTION_OFFSET = -4.97;
    
    /// @brief 城市建筑密度因子 10.0
    constexpr double URBAN_DENSITY_FACTOR = 10.0;
    
    /// @brief 城市天线高度假设值 30.0 (米)
    constexpr double URBAN_ANTENNA_HEIGHT = 30.0;
    
    /// @brief 城市天线高度系数 11.75
    constexpr double URBAN_ANTENNA_HEIGHT_COEFF = 11.75;

    // ==================== 信号质量评估阈值 ====================
    
    /// @brief 优秀信号质量SNR阈值 20.0 dB
    constexpr double EXCELLENT_SNR_THRESHOLD = 20.0;
    
    /// @brief 良好信号质量SNR阈值 10.0 dB
    constexpr double GOOD_SNR_THRESHOLD = 10.0;
    
    /// @brief 一般信号质量SNR阈值 5.0 dB
    constexpr double FAIR_SNR_THRESHOLD = 5.0;
    
    /// @brief 优秀信号质量BER阈值 1e-6
    constexpr double EXCELLENT_BER_THRESHOLD = 1e-6;
    
    /// @brief 良好信号质量BER阈值 1e-4
    constexpr double GOOD_BER_THRESHOLD = 1e-4;
    
    /// @brief 一般信号质量BER阈值 1e-2
    constexpr double FAIR_BER_THRESHOLD = 1e-2;
    
    /// @brief 较差信号质量BER阈值 0.1
    constexpr double POOR_BER_THRESHOLD = 0.1;

    // ==================== 抗干扰模型系数 ====================
    
    /// @brief 默认处理增益 20.0 dB
    constexpr double DEFAULT_PROCESSING_GAIN = 20.0;
    
    /// @brief 最大编码增益 20.0 dB
    constexpr double MAX_CODING_GAIN = 20.0;
    
    /// @brief 自适应增益基础值 5.0 dB
    constexpr double ADAPTIVE_GAIN_BASE = 5.0;
    
    /// @brief 自适应增益速度系数 10.0
    constexpr double ADAPTIVE_GAIN_SPEED_COEFF = 10.0;
    
    /// @brief 自适应增益收敛系数 20.0
    constexpr double ADAPTIVE_GAIN_CONVERGENCE_COEFF = 20.0;

    // ==================== 干扰模型系数 ====================
    
    /// @brief 最小有效干信比 10.0 dB
    constexpr double MIN_EFFECTIVE_JS_RATIO = 10.0;
    
    /// @brief 干扰功率建议值1 10.0 dB
    constexpr double JAMMER_POWER_SUGGESTION_1 = 10.0;
    
    /// @brief 干扰功率建议值2 20.0 dB
    constexpr double JAMMER_POWER_SUGGESTION_2 = 20.0;

    // ==================== 默认参数值 ====================
    
    /// @brief 默认工作频率 2400.0 MHz
    constexpr double DEFAULT_FREQUENCY = 2400.0;
    
    /// @brief 默认最大视距 10.0 km
    constexpr double DEFAULT_MAX_LINE_OF_SIGHT = 10.0;
    
    /// @brief 默认链路余量 10.0 dB
    constexpr double DEFAULT_LINK_MARGIN = 10.0;
    
    /// @brief 默认发射功率 20.0 dBm
    constexpr double DEFAULT_TRANSMIT_POWER = 20.0;
    
    /// @brief 默认接收灵敏度 -100.0 dBm
    constexpr double DEFAULT_RECEIVE_SENSITIVITY = -100.0;
    
    /// @brief 默认噪声系数 3.0 dB
    constexpr double DEFAULT_NOISE_FIGURE = 3.0;
    
    /// @brief 默认环境衰减系数 1.0
    constexpr double DEFAULT_ENV_ATTENUATION = 1.0;

    // ==================== 计算精度和容差 ====================
    
    /// @brief 数值计算精度 1e-6
    constexpr double CALCULATION_PRECISION = 1e-6;
    
    /// @brief 衰减系数验证容差 ±0.5
    constexpr double ATTENUATION_TOLERANCE = 0.5;
    
    /// @brief 置信区间t值 (95%置信度)
    constexpr double CONFIDENCE_T_VALUE_95 = 1.96;
    
    /// @brief 置信区间t值 (99%置信度)
    constexpr double CONFIDENCE_T_VALUE_99 = 2.58;
    
    /// @brief 置信区间t值 (90%置信度)
    constexpr double CONFIDENCE_T_VALUE_90 = 1.64;

    // ==================== 抗截获阻力常量 ====================
    
    /// @brief 跳频基础阻力 0.8
    constexpr double FH_BASE_RESISTANCE = 0.8;
    
    /// @brief 跳频阻力因子 0.2
    constexpr double FH_RESISTANCE_FACTOR = 0.2;
    
    /// @brief 跳频速率归一化因子 10000.0
    constexpr double FH_RATE_NORMALIZATION = 10000.0;
    
    /// @brief 直接序列基础阻力 0.7
    constexpr double DS_BASE_RESISTANCE = 0.7;
    
    /// @brief 直接序列阻力因子 0.3
    constexpr double DS_RESISTANCE_FACTOR = 0.3;
    
    /// @brief 直接序列因子归一化 10000.0
    constexpr double DS_FACTOR_NORMALIZATION = 10000.0;
    
    /// @brief 时间跳频基础阻力 0.6
    constexpr double TH_BASE_RESISTANCE = 0.6;
    
    /// @brief 时间跳频阻力因子 0.4
    constexpr double TH_RESISTANCE_FACTOR = 0.4;
    
    /// @brief 混合扩频阻力 0.9
    constexpr double HYBRID_RESISTANCE = 0.9;
    
    /// @brief 默认基础阻力 0.3
    constexpr double DEFAULT_BASE_RESISTANCE = 0.3;
    
    /// @brief 默认阻力因子 0.02
    constexpr double DEFAULT_RESISTANCE_FACTOR = 0.02;
    
    /// @brief 最大阻力因子 1.0
    constexpr double MAX_RESISTANCE_FACTOR = 1.0;

    // ==================== 误码率计算常量 ====================
    
    /// @brief 线性到dB转换基数 10.0
    constexpr double LINEAR_TO_DB_BASE = 10.0;
    
    /// @brief 误码率系数 0.5
    constexpr double BER_COEFFICIENT = 0.5;
    
    /// @brief 最小误码率 1e-10
    constexpr double MIN_BER = 1e-10;
    
    /// @brief 最大误码率 0.5
    constexpr double MAX_BER = 0.5;

    // ==================== 吞吐量退化常量 ====================
    
    /// @brief 最大退化值 1.0
    constexpr double MAX_DEGRADATION = 1.0;
    
    /// @brief 最小退化值 0.0
    constexpr double MIN_DEGRADATION = 0.0;
    
    /// @brief 退化因子 10.0
    constexpr double DEGRADATION_FACTOR = 10.0;

    // ==================== 检测概率常量 ====================
    
    /// @brief 最大检测概率 1.0
    constexpr double MAX_DETECTION_PROB = 1.0;
    
    /// @brief 最小检测概率 0.0
    constexpr double MIN_DETECTION_PROB = 0.0;
    
    /// @brief 检测偏移量 10.0
    constexpr double DETECTION_OFFSET = 10.0;
    
    /// @brief 检测尺度因子 5.0
    constexpr double DETECTION_SCALE = 5.0;

    // ==================== 自测试范围常量 ====================
    
    /// @brief 增益测试最小值 -50.0
    constexpr double MIN_GAIN_TEST = -50.0;
    
    /// @brief 增益测试最大值 100.0
    constexpr double MAX_GAIN_TEST = 100.0;
    
    /// @brief 阻力测试最小值 0.0
    constexpr double MIN_RESISTANCE_TEST = 0.0;
    
    /// @brief 阻力测试最大值 1.0
    constexpr double MAX_RESISTANCE_TEST = 1.0;
    
    /// @brief 信干比测试最小值 -100.0
    constexpr double MIN_SJR_TEST = -100.0;
    
    /// @brief 信干比测试最大值 100.0
    constexpr double MAX_SJR_TEST = 100.0;
    
    /// @brief 误码率测试最小值 0.0
    constexpr double MIN_BER_TEST = 0.0;
    
    /// @brief 误码率测试最大值 1.0
    constexpr double MAX_BER_TEST = 1.0;

    // ==================== 抗干扰策略系数 ====================
    
    /// @brief 被动策略因子 0.7
    constexpr double PASSIVE_STRATEGY_FACTOR = 0.7;
    
    /// @brief 主动策略因子 1.0
    constexpr double ACTIVE_STRATEGY_FACTOR = 1.0;
    
    /// @brief 自适应策略因子 1.2
    constexpr double ADAPTIVE_STRATEGY_FACTOR = 1.2;
    
    /// @brief 协作策略因子 1.3
    constexpr double COOPERATIVE_STRATEGY_FACTOR = 1.3;
    
    /// @brief 认知策略因子 1.5
    constexpr double COGNITIVE_STRATEGY_FACTOR = 1.5;

    // ==================== 抗干扰阻力计算常量 ====================
    
    /// @brief 最大阻力值 1.0
    constexpr double MAX_RESISTANCE = 1.0;
    
    /// @brief 最小阻力值 0.0
    constexpr double MIN_RESISTANCE = 0.0;
    
    /// @brief 阻力基础值 1.0
    constexpr double RESISTANCE_BASE = 1.0;
    
    /// @brief 阻力增益除数 20.0
    constexpr double RESISTANCE_GAIN_DIVISOR = 20.0;
    
    /// @brief 最小信干比 -100.0 dB
    constexpr double MIN_SJR_DB = -100.0;

    // ==================== 保护有效性权重 ====================
    
    /// @brief 阻力权重 0.4
    constexpr double RESISTANCE_WEIGHT = 0.4;
    
    /// @brief 截获阻力权重 0.3
    constexpr double INTERCEPTION_WEIGHT = 0.3;
    
    /// @brief 吞吐量权重 0.3
    constexpr double THROUGHPUT_WEIGHT = 0.3;
    
    /// @brief 最大吞吐量维护 1.0
    constexpr double MAX_THROUGHPUT_MAINTENANCE = 1.0;

    // ==================== 保护等级阈值 ====================
    
    /// @brief 无保护阈值 0.2
    constexpr double NO_PROTECTION_THRESHOLD = 0.2;
    
    /// @brief 低保护阈值 0.4
    constexpr double LOW_PROTECTION_THRESHOLD = 0.4;
    
    /// @brief 中等保护阈值 0.6
    constexpr double MEDIUM_PROTECTION_THRESHOLD = 0.6;
    
    /// @brief 高保护阈值 0.8
    constexpr double HIGH_PROTECTION_THRESHOLD = 0.8;

    // ==================== 自适应效率常量 ====================
    
    /// @brief 最大自适应效率 1.0
    constexpr double MAX_ADAPTATION_EFFICIENCY = 1.0;

    // ==================== 资源利用率常量 ====================
    
    /// @brief 最大带宽利用率 1.0
    constexpr double MAX_BANDWIDTH_UTILIZATION = 1.0;
    
    /// @brief 带宽归一化因子 100.0
    constexpr double BANDWIDTH_NORMALIZATION = 100.0;
    
    /// @brief 最大功率利用率 1.0
    constexpr double MAX_POWER_UTILIZATION = 1.0;
    
    /// @brief 功率偏移量 50.0
    constexpr double POWER_OFFSET = 50.0;
    
    /// @brief 功率归一化因子 100.0
    constexpr double POWER_NORMALIZATION = 100.0;
    
    /// @brief 最大处理利用率 1.0
    constexpr double MAX_PROCESSING_UTILIZATION = 1.0;
    
    /// @brief 处理归一化因子 50.0
    constexpr double PROCESSING_NORMALIZATION = 50.0;
    
    /// @brief 利用率组件数 3.0
    constexpr double UTILIZATION_COMPONENTS = 3.0;

    // ==================== 增益归一化常量 ====================
    
    /// @brief 跳频增益归一化 30.0
    constexpr double HOPPING_GAIN_NORMALIZATION = 30.0;
    
    /// @brief 扩频增益归一化 40.0
    constexpr double SPREAD_GAIN_NORMALIZATION = 40.0;
    
    /// @brief 滤波增益归一化 15.0
    constexpr double FILTER_GAIN_NORMALIZATION = 15.0;
    
    /// @brief 波束增益归一化 20.0
    constexpr double BEAM_GAIN_NORMALIZATION = 20.0;
    
    /// @brief 分集增益归一化 10.0
    constexpr double DIVERSITY_GAIN_NORMALIZATION = 10.0;
    
    /// @brief 编码增益归一化 20.0
    constexpr double CODING_GAIN_NORMALIZATION = 20.0;

    // ==================== 最优计算常量 ====================
    
    /// @brief 处理增益余量 10.0 dB
    constexpr double PROCESSING_GAIN_MARGIN = 10.0;
    
    /// @brief 最小处理增益 0.0 dB
    constexpr double MIN_PROCESSING_GAIN = 0.0;
    
    /// @brief 最大处理增益 50.0 dB
    constexpr double MAX_PROCESSING_GAIN = 50.0;
    
    /// @brief 最优跳频基础值 1000.0
    constexpr double OPTIMAL_HOPPING_BASE = 1000.0;
    
    /// @brief 跳频速率基础值 1.0
    constexpr double HOPPING_RATE_BASE = 1.0;
    
    /// @brief 跳频带宽除数 10.0
    constexpr double HOPPING_BANDWIDTH_DIVISOR = 10.0;
    
    /// @brief 最小跳频速率 100.0 Hz
    constexpr double MIN_HOPPING_RATE = 100.0;
    
    /// @brief 最大跳频速率 10000.0 Hz
    constexpr double MAX_HOPPING_RATE = 10000.0;
    
    /// @brief 最小跳频信道数 10
    constexpr int MIN_HOPPING_CHANNELS = 10;
    
    /// @brief 最大跳频信道数 1000
    constexpr int MAX_HOPPING_CHANNELS = 1000;

    // ==================== 组合技术效果常量 ====================
    
    /// @brief 组合增益初始值 0.0
    constexpr double COMBINED_GAIN_INITIAL = 0.0;
    
    /// @brief 协同效应初始值 1.0
    constexpr double SYNERGY_INITIAL = 1.0;
    
    /// @brief 协同效应衰减因子 0.8
    constexpr double SYNERGY_DECAY_FACTOR = 0.8;

    // ==================== 威胁等级阈值 ====================
    
    /// @brief 高威胁阈值 0.7
    constexpr double HIGH_THREAT_THRESHOLD = 0.7;
    
    /// @brief 中等威胁阈值 0.4
    constexpr double MEDIUM_THREAT_THRESHOLD = 0.4;

    // ==================== 性能预测常量 ====================
    
    /// @brief 最大性能值 1.0
    constexpr double MAX_PERFORMANCE = 1.0;
    
    /// @brief 最小目标误码率 1e-10
    constexpr double MIN_TARGET_BER = 1e-10;
    
    /// @brief 最大目标误码率 0.5
    constexpr double MAX_TARGET_BER = 0.5;
    
    /// @brief 零增益 0.0
    constexpr double ZERO_GAIN = 0.0;
    
    /// @brief 误码率乘数 2.0
    constexpr double BER_MULTIPLIER = 2.0;
    
    /// @brief 最小所需信干比 10.0 dB
    constexpr double MIN_REQUIRED_SJR = 10.0;

    // ==================== 显示格式常量 ====================
    
    /// @brief 百分比乘数 100.0
    constexpr double PERCENTAGE_MULTIPLIER = 100.0;

    // ==================== 环境损耗配置常量 ====================
    
    /// @brief 路径损耗指数最小值 1.5
    constexpr double MIN_PATH_LOSS_EXPONENT = 1.5;
    
    /// @brief 路径损耗指数最大值 6.0
    constexpr double MAX_PATH_LOSS_EXPONENT = 6.0;
    
    /// @brief 环境损耗最小值 0.0 dB
    constexpr double MIN_ENVIRONMENT_LOSS = 0.0;
    
    /// @brief 环境损耗最大值 50.0 dB
    constexpr double MAX_ENVIRONMENT_LOSS = 50.0;
    
    /// @brief 阴影衰落标准差最小值 0.0 dB
    constexpr double MIN_SHADOWING_STD_DEV = 0.0;
    
    /// @brief 阴影衰落标准差最大值 20.0 dB
    constexpr double MAX_SHADOWING_STD_DEV = 20.0;
    
    /// @brief 频率因子最小值 0.5
    constexpr double MIN_FREQUENCY_FACTOR = 0.5;
    
    /// @brief 频率因子最大值 3.0
    constexpr double MAX_FREQUENCY_FACTOR = 3.0;
    
    /// @brief 环境损耗除数 10.0
    constexpr double ENVIRONMENT_LOSS_DIVISOR = 10.0;
    
    /// @brief 容差值 0.5
    constexpr double TOLERANCE_VALUE = 0.5;
    
    /// @brief 自由空间路径损耗指数 2.0
    constexpr double FREE_SPACE_PATH_LOSS_EXPONENT = 2.0;
    
    /// @brief 频率转换因子 1000.0 (MHz to GHz)
    constexpr double FREQUENCY_CONVERSION_FACTOR = 1000.0;

    // ==================== 信号传输模型常量 ====================
    
    /// @brief 短波频率最小值 1500.0 kHz
    constexpr double HF_MIN_FREQUENCY = 1500.0;
    
    /// @brief 短波频率最大值 30000.0 kHz
    constexpr double HF_MAX_FREQUENCY = 30000.0;
    
    /// @brief 超短波频率最小值 30000.0 kHz
    constexpr double VHF_MIN_FREQUENCY = 30000.0;
    
    /// @brief 超短波频率最大值 300000.0 kHz
    constexpr double VHF_MAX_FREQUENCY = 300000.0;
    
    /// @brief 微波频率最小值 300000.0 kHz
    constexpr double UHF_MIN_FREQUENCY = 300000.0;
    
    /// @brief 微波频率最大值 30000000.0 kHz
    constexpr double UHF_MAX_FREQUENCY = 30000000.0;
    
    /// @brief 默认短波中心频率 15750.0 kHz
    constexpr double DEFAULT_HF_CENTER_FREQ = 15750.0;
    
    /// @brief 默认超短波中心频率 165000.0 kHz
    constexpr double DEFAULT_VHF_CENTER_FREQ = 165000.0;
    
    /// @brief 默认微波中心频率 15150000.0 kHz
    constexpr double DEFAULT_UHF_CENTER_FREQ = 15150000.0;
    
    /// @brief 最大功率限制 100.0 W
    constexpr double MAX_POWER_LIMIT = 100.0;
    
    /// @brief 频率显示阈值 1000000 (1MHz)
    constexpr double FREQUENCY_DISPLAY_THRESHOLD_MHZ = 1000000.0;
    
    /// @brief 频率显示阈值 1000 (1kHz)
    constexpr double FREQUENCY_DISPLAY_THRESHOLD_KHZ = 1000.0;
    
    /// @brief 带宽显示阈值 1000 kHz
    constexpr double BANDWIDTH_DISPLAY_THRESHOLD = 1000.0;
    
    /// @brief 功率显示阈值 1000 W
    constexpr double POWER_DISPLAY_THRESHOLD = 1000.0;

    // ==================== 通信模型API常量 ====================
    
    /// @brief 默认信号强度 -150.0 dBm
    constexpr double DEFAULT_SIGNAL_STRENGTH = -150.0;
    
    /// @brief 默认信噪比 -50.0 dB
    constexpr double DEFAULT_SNR = -50.0;
    
    /// @brief 数据包长度位数 8000
    constexpr int PACKET_LENGTH_BITS = 8000;
    
    /// @brief 高信噪比阈值 20.0 dB
    constexpr double HIGH_SNR_THRESHOLD = 20.0;
    
    /// @brief 中等信噪比阈值 10.0 dB
    constexpr double MEDIUM_SNR_THRESHOLD = 10.0;
    
    /// @brief 低信噪比阈值 0.0 dB
    constexpr double LOW_SNR_THRESHOLD = 0.0;
    

    
    /// @brief 较差误码率阈值 1e-2
    constexpr double POOR_BER_THRESHOLD_API = 1e-2;
    
    /// @brief 低丢包率阈值 0.01
    constexpr double LOW_PACKET_LOSS_THRESHOLD = 0.01;
    
    /// @brief 高丢包率阈值 0.1
    constexpr double HIGH_PACKET_LOSS_THRESHOLD = 0.1;
    
    /// @brief 优秀质量分数 4
    constexpr int EXCELLENT_QUALITY_SCORE = 4;
    
    /// @brief 良好质量分数 2
    constexpr int GOOD_QUALITY_SCORE = 2;
    
    /// @brief 一般质量分数 0
    constexpr int FAIR_QUALITY_SCORE = 0;
    
    /// @brief 较差质量分数 -2
    constexpr int POOR_QUALITY_SCORE = -2;
    
    /// @brief 频率验证最小值 1.0 MHz
    constexpr double FREQUENCY_VALIDATION_MIN = 1.0;
    
    /// @brief 频率验证最大值 30000.0 MHz
    constexpr double FREQUENCY_VALIDATION_MAX = 30000.0;
    
    /// @brief 带宽验证最小值 0.1 MHz
    constexpr double BANDWIDTH_VALIDATION_MIN = 0.1;
    
    /// @brief 带宽验证最大值 10000.0 MHz
    constexpr double BANDWIDTH_VALIDATION_MAX = 10000.0;
    
    /// @brief 功率验证最小值 -50.0 dBm
    constexpr double POWER_VALIDATION_MIN = -50.0;
    
    /// @brief 功率验证最大值 50.0 dBm
    constexpr double POWER_VALIDATION_MAX = 50.0;
    
    /// @brief 距离验证最小值 0.001 km
    constexpr double DISTANCE_VALIDATION_MIN = 0.001;
    
    /// @brief 距离验证最大值 1000.0 km
    constexpr double DISTANCE_VALIDATION_MAX = 1000.0;
    
    /// @brief 连接状态误码率阈值 0.1
    constexpr double CONNECTION_BER_THRESHOLD = 0.1;
    
    /// @brief 连接状态丢包率阈值 0.5
    constexpr double CONNECTION_PACKET_LOSS_THRESHOLD = 0.5;
    
    /// @brief 功率转换偏移 30.0 dB
    constexpr double POWER_CONVERSION_OFFSET = 30.0;
    
    /// @brief 可靠性乘数 1000.0
    constexpr double RELIABILITY_MULTIPLIER = 1000.0;
    
    /// @brief 可用性信噪比偏移 5.0 dB
    constexpr double AVAILABILITY_SNR_OFFSET = 5.0;
    
    /// @brief 可用性除数 2.0
    constexpr double AVAILABILITY_DIVISOR = 2.0;
    
    /// @brief 最大效率 1.0
    constexpr double MAX_EFFICIENCY = 1.0;
    
    /// @brief 默认处理延迟 0.001 秒
    constexpr double DEFAULT_PROCESSING_DELAY = 0.001;
    
    /// @brief 重传延迟因子 2.0
    constexpr double RETRANSMISSION_DELAY_FACTOR = 2.0;
    
    /// @brief 信噪比余量 10.0 dB
    constexpr double SNR_MARGIN = 10.0;
    
    /// @brief 基准频率 2400.0 MHz
    constexpr double BASE_FREQUENCY = 2400.0;
    
    /// @brief 高损耗环境频率因子 0.7
    constexpr double HIGH_LOSS_FREQ_FACTOR = 0.7;
    
    /// @brief 高损耗环境阈值 3.0
    constexpr double HIGH_LOSS_THRESHOLD = 3.0;
    
    /// @brief 低损耗环境频率因子 1.3
    constexpr double LOW_LOSS_FREQ_FACTOR = 1.3;
    
    /// @brief 低损耗环境阈值 2.5
    constexpr double LOW_LOSS_THRESHOLD = 2.5;
    
    /// @brief 最优频率最小值 400.0 MHz
    constexpr double OPTIMAL_FREQ_MIN = 400.0;
    
    /// @brief 最优频率最大值 6000.0 MHz
    constexpr double OPTIMAL_FREQ_MAX = 6000.0;
    
    /// @brief 目标数据速率 10.0 Mbps
    constexpr double TARGET_DATA_RATE = 10.0;
    
    /// @brief 频谱效率 2.0 bps/Hz
    constexpr double SPECTRAL_EFFICIENCY = 2.0;
    
    /// @brief 默认干扰电平 -100.0 dBm
    constexpr double DEFAULT_INTERFERENCE_LEVEL = -100.0;
    
    /// @brief 距离步长 0.5 km
    constexpr double DISTANCE_STEP = 0.5;
    
    /// @brief 距离范围最小值 0.1 km
    constexpr double DISTANCE_RANGE_MIN = 0.1;
    
    /// @brief 距离范围最大值 100.0 km
    constexpr double DISTANCE_RANGE_MAX = 100.0;
    
    /// @brief 最小信噪比要求 10.0 dB
    constexpr double MIN_SNR_REQUIREMENT = 10.0;
    
    /// @brief 最小发射功率限制 -45.0 dBm
    constexpr double MIN_TRANSMIT_POWER_LIMIT = -45.0;
    
    /// @brief 功率余量 3.0 dB
    constexpr double POWER_MARGIN = 3.0;
    
    /// @brief 高抗干扰增益阈值 20.0 dB
    constexpr double HIGH_ANTI_JAM_GAIN_THRESHOLD = 20.0;
    
    /// @brief 高频率因子 1.1
    constexpr double HIGH_FREQUENCY_FACTOR = 1.1;
    
    /// @brief 低抗干扰增益阈值 10.0 dB
    constexpr double LOW_ANTI_JAM_GAIN_THRESHOLD = 10.0;
    
    /// @brief 低频率因子 0.9
    constexpr double LOW_FREQUENCY_FACTOR = 0.9;

    // ==================== 效率相关常量 ====================
    
    /// @brief 效率基础值 0.9
    constexpr double EFFICIENCY_BASE = 0.9;
    
    /// @brief 误码率效率因子 0.5
    constexpr double BER_EFFICIENCY_FACTOR = 0.5;
    
    /// @brief 最小效率 0.1
    constexpr double MIN_EFFICIENCY = 0.1;
    
    /// @brief 香农基础值 1.0
    constexpr double SHANNON_BASE = 1.0;

    // ==================== 距离模型计算常量 ====================
    
    /// @brief 功率距离基数 2.0
    constexpr double POWER_DISTANCE_BASE = 2.0;
    
    /// @brief 功率距离dB因子 6.0
    constexpr double POWER_DISTANCE_DB_FACTOR = 6.0;
    
    /// @brief 环境衰减基础值 1.0
    constexpr double ENV_ATTENUATION_BASE = 1.0;
    

    
    /// @brief 环境衰减最小值 0.5
    constexpr double ENV_ATTENUATION_MIN = 0.5;
    
    /// @brief 环境衰减最大值 5.0
    constexpr double ENV_ATTENUATION_MAX = 5.0;
    

    
    /// @brief 初始距离估计 1.0 km
    constexpr double INITIAL_DISTANCE_ESTIMATE = 1.0;
    
    /// @brief 最大迭代次数 100
    constexpr int MAX_ITERATIONS = 100;
    
    /// @brief 收敛容差 0.1
    constexpr double CONVERGENCE_TOLERANCE = 0.1;
    
    /// @brief 距离减小因子 0.9
    constexpr double DISTANCE_DECREASE_FACTOR = 0.9;
    
    /// @brief 距离增大因子 1.1
    constexpr double DISTANCE_INCREASE_FACTOR = 1.1;
    
    /// @brief 最小距离限制 0.001 km
    constexpr double MIN_DISTANCE_LIMIT = 0.001;

    // ==================== 抗干扰模型默认参数常量 ====================
    
    /// @brief 默认扩频因子 1000.0
    constexpr double DEFAULT_SPREADING_FACTOR = 1000.0;
    
    /// @brief 默认跳频信道数 100
    constexpr int DEFAULT_HOPPING_CHANNELS = 100;
    
    /// @brief 默认信道间隔 25.0 kHz
    constexpr double DEFAULT_CHANNEL_SPACING = 25.0;
    
    /// @brief 默认驻留时间 0.01 秒
    constexpr double DEFAULT_DWELL_TIME = 0.01;
    
    /// @brief 默认码片速率 10000000.0 cps
    constexpr double DEFAULT_CHIP_RATE = 10000000.0;
    
    /// @brief 默认序列长度 1023
    constexpr int DEFAULT_SEQUENCE_LENGTH = 1023;
    
    /// @brief 默认自适应速度 0.1
    constexpr double DEFAULT_ADAPTATION_SPEED = 0.1;
    
    /// @brief 默认收敛阈值 0.01
    constexpr double DEFAULT_CONVERGENCE_THRESHOLD = 0.01;
    
    /// @brief 默认环境类型 1 (开阔地)
    constexpr int DEFAULT_ENVIRONMENT_TYPE = 1;
    
    /// @brief 默认干扰机密度 0.1
    constexpr double DEFAULT_JAMMER_DENSITY = 0.1;
    
    /// @brief 默认干扰功率 20.0 dBm
    constexpr double DEFAULT_JAMMER_POWER = 20.0;
    
    /// @brief 默认干扰带宽 10.0 MHz
    constexpr double DEFAULT_JAMMER_BANDWIDTH = 10.0;
    
    /// @brief 默认干扰距离 5.0 km
    constexpr double DEFAULT_JAMMER_DISTANCE = 5.0;
    
    /// @brief 默认温度 20.0 摄氏度
    constexpr double DEFAULT_TEMPERATURE = 20.0;
    
    /// @brief 默认湿度 50.0 %
    constexpr double DEFAULT_HUMIDITY = 50.0;
    
    /// @brief 默认大气压力 101.325 kPa
    constexpr double DEFAULT_ATMOSPHERIC_PRESSURE = 101.325;
    
    /// @brief 默认带宽 10.0 MHz
    constexpr double DEFAULT_BANDWIDTH = 10.0;
    
    /// @brief 默认噪声功率 -100.0 dBm
    constexpr double DEFAULT_NOISE_POWER = -100.0;
    
    /// @brief 默认距离 5.0 km
    constexpr double DEFAULT_DISTANCE = 5.0;
    
    /// @brief 默认目标带宽 10.0 MHz
    constexpr double DEFAULT_TARGET_BANDWIDTH = 10.0;
    
    /// @brief 默认目标功率 30.0 dBm
    constexpr double DEFAULT_TARGET_POWER = 30.0;
    
    /// @brief 默认系统带宽 10.0 MHz
    constexpr double DEFAULT_SYSTEM_BANDWIDTH = 10.0;
    
    /// @brief 默认信号功率 30.0 dBm
    constexpr double DEFAULT_SIGNAL_POWER = 30.0;
    
    /// @brief 默认编码增益 3.0 dB
    constexpr double DEFAULT_CODING_GAIN = 3.0;
    
    /// @brief 默认跳频速率 1000.0 hops/s
    constexpr double DEFAULT_HOPPING_RATE = 1000.0;

    // ==================== 抗干扰模型计算常量 ====================
    
    /// @brief 每秒时隙数 1000.0
    constexpr double TIME_SLOTS_PER_SECOND = 1000.0;
    
    /// @brief 最小自适应增益 0.0 dB
    constexpr double MIN_ADAPTIVE_GAIN = 0.0;
    
    /// @brief 最大自适应增益 30.0 dB
    constexpr double MAX_ADAPTIVE_GAIN = 30.0;
    
    /// @brief 波束增益基础值 5.0
    constexpr double BEAM_GAIN_BASE = 5.0;
    
    /// @brief 波束增益带宽系数 0.5
    constexpr double BEAM_GAIN_BANDWIDTH_COEFF = 0.5;
    
    /// @brief 最小波束增益 0.0 dB
    constexpr double MIN_BEAM_GAIN = 0.0;
    
    /// @brief 最大波束增益 20.0 dB
    constexpr double MAX_BEAM_GAIN = 20.0;
    
    /// @brief 分集增益基础值 3.0
    constexpr double DIVERSITY_GAIN_BASE = 3.0;
    
    /// @brief 分集增益环境系数 2.0
    constexpr double DIVERSITY_GAIN_ENV_COEFF = 2.0;
    
    /// @brief 最小分集增益 0.0 dB
    constexpr double MIN_DIVERSITY_GAIN = 0.0;
    
    /// @brief 最大分集增益 10.0 dB
    constexpr double MAX_DIVERSITY_GAIN = 10.0;
    
    /// @brief 抵消增益乘数 0.5
    constexpr double CANCELLATION_GAIN_MULTIPLIER = 0.5;
    
    /// @brief 抵消增益基础值 1.0
    constexpr double CANCELLATION_GAIN_BASE = 1.0;
    
    /// @brief 最小抵消增益 0.0 dB
    constexpr double MIN_CANCELLATION_GAIN = 0.0;
    
    /// @brief 最大抵消增益 15.0 dB
    constexpr double MAX_CANCELLATION_GAIN = 15.0;
    
    /// @brief 混合扩频因子 0.8
    constexpr double HYBRID_SPREAD_FACTOR = 0.8;
    
    /// @brief 功率控制增益 3.0 dB
    constexpr double POWER_CONTROL_GAIN = 3.0;
    

    

    


    // ==================== 通信模型工具常量 ====================
    
    
    
    
    
    
    

    
    // ==================== 干扰机模型常量 ====================
    
    /// @brief 默认脉冲宽度 0.001 秒
    constexpr double DEFAULT_PULSE_WIDTH = 0.001;
    
    /// @brief 默认脉冲重复频率 1000.0 Hz
    constexpr double DEFAULT_PULSE_REPETITION_RATE = 1000.0;
    
    /// @brief 默认占空比 0.1
    constexpr double DEFAULT_DUTY_CYCLE = 0.1;
    
    /// @brief 默认扫频速率 1000.0 Hz/s
    constexpr double DEFAULT_SWEEP_RATE = 1000.0;
    
    /// @brief 默认扫频范围 10.0 MHz
    constexpr double DEFAULT_SWEEP_RANGE = 10.0;
    
    /// @brief 毫秒到秒转换 1000.0
    constexpr double MS_TO_S_CONVERSION = 1000.0;
    
    /// @brief 最大占空比 1.0
    constexpr double MAX_DUTY_CYCLE = 1.0;
    
    /// @brief 带宽一半除数 2.0
    constexpr double BANDWIDTH_HALF_DIVISOR = 2.0;
    
    /// @brief 退化基数 1.0
    constexpr double DEGRADATION_BASE = 1.0;
    
    /// @brief 最大功率因子 10.0
    constexpr double MAX_POWER_FACTOR = 10.0;
    
    /// @brief 最大覆盖 1.0
    constexpr double MAX_COVERAGE = 1.0;
    
    /// @brief 频率尺度因子 1.0
    constexpr double FREQUENCY_SCALE_FACTOR = 1.0;
    
    /// @brief 最大时间因子 1.0
    constexpr double MAX_TIME_FACTOR = 1.0;
    
    /// @brief 脉冲功率除数 20.0
    constexpr double PULSE_POWER_DIVISOR = 20.0;
    
    /// @brief 脉冲功率基数 1.0
    constexpr double PULSE_POWER_BASE = 1.0;
    
    /// @brief 弹幕功率除数 10.0
    constexpr double BARRAGE_POWER_DIVISOR = 10.0;
    
    /// @brief 最大点频效果 1.0
    constexpr double MAX_SPOT_EFFECT = 1.0;
    
    /// @brief 点频增强因子 1.1
    constexpr double SPOT_ENHANCEMENT_FACTOR = 1.1;
    
    /// @brief Hz到MHz转换 1000000.0
    constexpr double HZ_TO_MHZ_CONVERSION = 1000000.0;
    
    /// @brief 最大组合效果 1.0
    constexpr double MAX_COMBINED_EFFECT = 1.0;
    
    /// @brief 组合效果基数 1.0
    constexpr double COMBINED_EFFECT_BASE = 1.0;


} // namespace MathConstants

#endif // MATH_CONSTANTS_H