using System;
using System.Runtime.InteropServices;
using System.Text;

namespace CommunicationModel
{
    /// <summary>
    /// 通信模型结果枚举
    /// </summary>
    public enum CommResult
    {
        Success = 0,
        ErrorInvalidHandle = -1,
        ErrorNullPointer = -2,
        ErrorInvalidParameter = -3,
        ErrorCalculationFailed = -4,
        ErrorMemoryAllocation = -5,
        ErrorFileOperation = -6,
        ErrorUnknown = -99
    }

    /// <summary>
    /// 通信场景枚举
    /// </summary>
    public enum CommScenario
    {
        NormalCommunication = 0,
        JammedCommunication = 1,
        AntiJamCommunication = 2,
        MultiUserCommunication = 3,
        RelayCommunication = 4,
        MeshCommunication = 5
    }

    /// <summary>
    /// 通信质量枚举
    /// </summary>
    public enum CommQuality
    {
        Excellent = 0,
        Good = 1,
        Fair = 2,
        Poor = 3,
        Failed = 4
    }

    /// <summary>
    /// 环境类型枚举
    /// </summary>
    public enum CommEnvironmentType
    {
        FreeSpace = 0,
        Urban = 1,
        Suburban = 2,
        Rural = 3,
        Indoor = 4,
        Maritime = 5,
        Mountainous = 6
    }

    /// <summary>
    /// 干扰器类型枚举
    /// </summary>
    public enum CommJammerType
    {
        GaussianNoise = 0,
        Narrowband = 1,
        Wideband = 2,
        Pulse = 3,
        Sweep = 4,
        Barrage = 5
    }

    /// <summary>
    /// 通信环境结构体
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct CommEnvironment
    {
        public double Frequency;
        public double Bandwidth;
        public double TransmitPower;
        public double NoisePower;
        public double Distance;
        public CommEnvironmentType EnvironmentType;
        public double Temperature;
        public double Humidity;
        public double AtmosphericPressure;
    }

    /// <summary>
    /// 干扰环境结构体
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct CommJammingEnvironment
    {
        public int IsJammed;
        public CommJammerType JammerType;
        public double JammerPower;
        public double JammerFrequency;
        public double JammerBandwidth;
        public double JammerDistance;
        public double JammerDensity;
        public IntPtr JammerFrequencies;
        public int JammerFrequencyCount;
    }

    /// <summary>
    /// 链路状态结构体
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct CommLinkStatus
    {
        public int IsConnected;
        public double SignalStrength;
        public double SignalToNoiseRatio;
        public double BitErrorRate;
        public double Throughput;
        public double Latency;
        public double PacketLossRate;
        public CommQuality Quality;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
        public string StatusDescription;
    }

    /// <summary>
    /// 性能指标结构体
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct CommPerformance
    {
        public double EffectiveRange;
        public double MaxDataRate;
        public double PowerEfficiency;
        public double SpectralEfficiency;
        public double Reliability;
        public double Availability;
        public double JammerResistance;
        public double InterceptionResistance;
    }

    /// <summary>
    /// 双精度数组结构体
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct DoubleArray
    {
        public IntPtr Values;
        public int Count;
    }

    /// <summary>
    /// 链路状态数组结构体
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct LinkStatusArray
    {
        public IntPtr Statuses;
        public int Count;
    }

    /// <summary>
    /// 通信模型API包装类
    /// </summary>
    public class CommunicationModelAPI : IDisposable
    {
        private IntPtr _handle;
        private bool _disposed = false;

        #region DLL导入声明

        private const string DllName = "CommunicationModelCAPI";

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr CommModel_Create();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr CommModel_CreateWithScenario(CommScenario scenario);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_Destroy(IntPtr handle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_SetScenario(IntPtr handle, CommScenario scenario);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_GetScenario(IntPtr handle, out CommScenario scenario);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_SetEnvironment(IntPtr handle, ref CommEnvironment env);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_GetEnvironment(IntPtr handle, out CommEnvironment env);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_SetJammingEnvironment(IntPtr handle, ref CommJammingEnvironment jammingEnv);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_GetJammingEnvironment(IntPtr handle, out CommJammingEnvironment jammingEnv);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_SetFrequency(IntPtr handle, double frequency);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_SetBandwidth(IntPtr handle, double bandwidth);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_SetTransmitPower(IntPtr handle, double power);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_SetDistance(IntPtr handle, double distance);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_SetEnvironmentType(IntPtr handle, CommEnvironmentType envType);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_CalculateLinkStatus(IntPtr handle, out CommLinkStatus linkStatus);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_CalculatePerformance(IntPtr handle, out CommPerformance performance);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_CalculateCommunicationRange(IntPtr handle, out double range);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_CalculateRequiredPower(IntPtr handle, double targetRange, out double requiredPower);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_CalculateOptimalFrequency(IntPtr handle, out double optimalFreq);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_CalculateOptimalBandwidth(IntPtr handle, out double optimalBandwidth);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_CalculateJammerEffectiveness(IntPtr handle, out double effectiveness);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_CalculateAntiJamEffectiveness(IntPtr handle, out double effectiveness);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_CalculateJammerToSignalRatio(IntPtr handle, out double jsRatio);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_CalculateRequiredAntiJamGain(IntPtr handle, double targetBER, out double requiredGain);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_CalculateJammerCoverage(IntPtr handle, out DoubleArray coverage);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_OptimizeForRange(IntPtr handle, double targetRange, out CommEnvironment optimizedEnv);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_OptimizeForDataRate(IntPtr handle, double targetDataRate, out CommEnvironment optimizedEnv);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_OptimizeForPowerEfficiency(IntPtr handle, out CommEnvironment optimizedEnv);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_OptimizeForJammerResistance(IntPtr handle, out CommEnvironment optimizedEnv);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern CommResult CommModel_GenerateDetailedReport(IntPtr handle, StringBuilder report, int bufferSize);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern CommResult CommModel_GeneratePerformanceReport(IntPtr handle, StringBuilder report, int bufferSize);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern CommResult CommModel_GenerateJammingAnalysisReport(IntPtr handle, StringBuilder report, int bufferSize);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern CommResult CommModel_SaveConfiguration(IntPtr handle, string filename);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern CommResult CommModel_LoadConfiguration(IntPtr handle, string filename);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern CommResult CommModel_ExportConfigurationToJSON(IntPtr handle, StringBuilder jsonStr, int bufferSize);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern CommResult CommModel_ImportConfigurationFromJSON(IntPtr handle, string jsonStr);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern CommResult CommModel_GetVersion(StringBuilder version, int bufferSize);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern CommResult CommModel_GetBuildInfo(StringBuilder buildInfo, int bufferSize);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern CommResult CommModel_GetModelInfo(IntPtr handle, StringBuilder modelInfo, int bufferSize);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern DoubleArray CommModel_AllocateDoubleArray(int count);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_FreeDoubleArray(ref DoubleArray array);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern LinkStatusArray CommModel_AllocateLinkStatusArray(int count);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern CommResult CommModel_FreeLinkStatusArray(ref LinkStatusArray array);

        #endregion

        #region 构造函数和析构函数

        /// <summary>
        /// 默认构造函数
        /// </summary>
        public CommunicationModelAPI()
        {
            _handle = CommModel_Create();
            if (_handle == IntPtr.Zero)
            {
                throw new InvalidOperationException("Failed to create communication model instance");
            }
        }

        /// <summary>
        /// 带场景的构造函数
        /// </summary>
        /// <param name="scenario">通信场景</param>
        public CommunicationModelAPI(CommScenario scenario)
        {
            _handle = CommModel_CreateWithScenario(scenario);
            if (_handle == IntPtr.Zero)
            {
                throw new InvalidOperationException("Failed to create communication model instance");
            }
        }

        /// <summary>
        /// 析构函数
        /// </summary>
        ~CommunicationModelAPI()
        {
            Dispose(false);
        }

        #endregion

        #region 公共方法

        /// <summary>
        /// 设置通信场景
        /// </summary>
        /// <param name="scenario">通信场景</param>
        public void SetScenario(CommScenario scenario)
        {
            CheckDisposed();
            var result = CommModel_SetScenario(_handle, scenario);
            CheckResult(result);
        }

        /// <summary>
        /// 获取通信场景
        /// </summary>
        /// <returns>通信场景</returns>
        public CommScenario GetScenario()
        {
            CheckDisposed();
            var result = CommModel_GetScenario(_handle, out CommScenario scenario);
            CheckResult(result);
            return scenario;
        }

        /// <summary>
        /// 设置通信环境
        /// </summary>
        /// <param name="environment">通信环境</param>
        public void SetEnvironment(CommEnvironment environment)
        {
            CheckDisposed();
            var result = CommModel_SetEnvironment(_handle, ref environment);
            CheckResult(result);
        }

        /// <summary>
        /// 获取通信环境
        /// </summary>
        /// <returns>通信环境</returns>
        public CommEnvironment GetEnvironment()
        {
            CheckDisposed();
            var result = CommModel_GetEnvironment(_handle, out CommEnvironment environment);
            CheckResult(result);
            return environment;
        }

        /// <summary>
        /// 设置干扰环境
        /// </summary>
        /// <param name="jammingEnvironment">干扰环境</param>
        public void SetJammingEnvironment(CommJammingEnvironment jammingEnvironment)
        {
            CheckDisposed();
            var result = CommModel_SetJammingEnvironment(_handle, ref jammingEnvironment);
            CheckResult(result);
        }

        /// <summary>
        /// 获取干扰环境
        /// </summary>
        /// <returns>干扰环境</returns>
        public CommJammingEnvironment GetJammingEnvironment()
        {
            CheckDisposed();
            var result = CommModel_GetJammingEnvironment(_handle, out CommJammingEnvironment jammingEnvironment);
            CheckResult(result);
            return jammingEnvironment;
        }

        /// <summary>
        /// 设置频率
        /// </summary>
        /// <param name="frequency">频率 (Hz)</param>
        public void SetFrequency(double frequency)
        {
            CheckDisposed();
            var result = CommModel_SetFrequency(_handle, frequency);
            CheckResult(result);
        }

        /// <summary>
        /// 设置带宽
        /// </summary>
        /// <param name="bandwidth">带宽 (Hz)</param>
        public void SetBandwidth(double bandwidth)
        {
            CheckDisposed();
            var result = CommModel_SetBandwidth(_handle, bandwidth);
            CheckResult(result);
        }

        /// <summary>
        /// 设置发射功率
        /// </summary>
        /// <param name="power">发射功率 (W)</param>
        public void SetTransmitPower(double power)
        {
            CheckDisposed();
            var result = CommModel_SetTransmitPower(_handle, power);
            CheckResult(result);
        }

        /// <summary>
        /// 设置距离
        /// </summary>
        /// <param name="distance">距离 (m)</param>
        public void SetDistance(double distance)
        {
            CheckDisposed();
            var result = CommModel_SetDistance(_handle, distance);
            CheckResult(result);
        }

        /// <summary>
        /// 设置环境类型
        /// </summary>
        /// <param name="environmentType">环境类型</param>
        public void SetEnvironmentType(CommEnvironmentType environmentType)
        {
            CheckDisposed();
            var result = CommModel_SetEnvironmentType(_handle, environmentType);
            CheckResult(result);
        }

        /// <summary>
        /// 计算链路状态
        /// </summary>
        /// <returns>链路状态</returns>
        public CommLinkStatus CalculateLinkStatus()
        {
            CheckDisposed();
            var result = CommModel_CalculateLinkStatus(_handle, out CommLinkStatus linkStatus);
            CheckResult(result);
            return linkStatus;
        }

        /// <summary>
        /// 计算性能指标
        /// </summary>
        /// <returns>性能指标</returns>
        public CommPerformance CalculatePerformance()
        {
            CheckDisposed();
            var result = CommModel_CalculatePerformance(_handle, out CommPerformance performance);
            CheckResult(result);
            return performance;
        }

        /// <summary>
        /// 计算通信距离
        /// </summary>
        /// <returns>通信距离 (m)</returns>
        public double CalculateCommunicationRange()
        {
            CheckDisposed();
            var result = CommModel_CalculateCommunicationRange(_handle, out double range);
            CheckResult(result);
            return range;
        }

        /// <summary>
        /// 计算所需功率
        /// </summary>
        /// <param name="targetRange">目标距离 (m)</param>
        /// <returns>所需功率 (W)</returns>
        public double CalculateRequiredPower(double targetRange)
        {
            CheckDisposed();
            var result = CommModel_CalculateRequiredPower(_handle, targetRange, out double requiredPower);
            CheckResult(result);
            return requiredPower;
        }

        /// <summary>
        /// 计算最优频率
        /// </summary>
        /// <returns>最优频率 (Hz)</returns>
        public double CalculateOptimalFrequency()
        {
            CheckDisposed();
            var result = CommModel_CalculateOptimalFrequency(_handle, out double optimalFreq);
            CheckResult(result);
            return optimalFreq;
        }

        /// <summary>
        /// 计算最优带宽
        /// </summary>
        /// <returns>最优带宽 (Hz)</returns>
        public double CalculateOptimalBandwidth()
        {
            CheckDisposed();
            var result = CommModel_CalculateOptimalBandwidth(_handle, out double optimalBandwidth);
            CheckResult(result);
            return optimalBandwidth;
        }

        /// <summary>
        /// 计算干扰有效性
        /// </summary>
        /// <returns>干扰有效性</returns>
        public double CalculateJammerEffectiveness()
        {
            CheckDisposed();
            var result = CommModel_CalculateJammerEffectiveness(_handle, out double effectiveness);
            CheckResult(result);
            return effectiveness;
        }

        /// <summary>
        /// 计算抗干扰有效性
        /// </summary>
        /// <returns>抗干扰有效性</returns>
        public double CalculateAntiJamEffectiveness()
        {
            CheckDisposed();
            var result = CommModel_CalculateAntiJamEffectiveness(_handle, out double effectiveness);
            CheckResult(result);
            return effectiveness;
        }

        /// <summary>
        /// 计算干信比
        /// </summary>
        /// <returns>干信比</returns>
        public double CalculateJammerToSignalRatio()
        {
            CheckDisposed();
            var result = CommModel_CalculateJammerToSignalRatio(_handle, out double jsRatio);
            CheckResult(result);
            return jsRatio;
        }

        /// <summary>
        /// 计算所需抗干扰增益
        /// </summary>
        /// <param name="targetBER">目标误码率</param>
        /// <returns>所需抗干扰增益</returns>
        public double CalculateRequiredAntiJamGain(double targetBER)
        {
            CheckDisposed();
            var result = CommModel_CalculateRequiredAntiJamGain(_handle, targetBER, out double requiredGain);
            CheckResult(result);
            return requiredGain;
        }

        /// <summary>
        /// 计算干扰覆盖范围
        /// </summary>
        /// <returns>干扰覆盖范围数组</returns>
        public double[] CalculateJammerCoverage()
        {
            CheckDisposed();
            var result = CommModel_CalculateJammerCoverage(_handle, out DoubleArray coverage);
            CheckResult(result);

            if (coverage.Count == 0 || coverage.Values == IntPtr.Zero)
            {
                return new double[0];
            }

            double[] managedArray = new double[coverage.Count];
            Marshal.Copy(coverage.Values, managedArray, 0, coverage.Count);

            // 释放非托管内存
            CommModel_FreeDoubleArray(ref coverage);

            return managedArray;
        }

        /// <summary>
        /// 针对距离优化
        /// </summary>
        /// <param name="targetRange">目标距离 (m)</param>
        /// <returns>优化后的环境</returns>
        public CommEnvironment OptimizeForRange(double targetRange)
        {
            CheckDisposed();
            var result = CommModel_OptimizeForRange(_handle, targetRange, out CommEnvironment optimizedEnv);
            CheckResult(result);
            return optimizedEnv;
        }

        /// <summary>
        /// 针对数据速率优化
        /// </summary>
        /// <param name="targetDataRate">目标数据速率 (bps)</param>
        /// <returns>优化后的环境</returns>
        public CommEnvironment OptimizeForDataRate(double targetDataRate)
        {
            CheckDisposed();
            var result = CommModel_OptimizeForDataRate(_handle, targetDataRate, out CommEnvironment optimizedEnv);
            CheckResult(result);
            return optimizedEnv;
        }

        /// <summary>
        /// 针对功率效率优化
        /// </summary>
        /// <returns>优化后的环境</returns>
        public CommEnvironment OptimizeForPowerEfficiency()
        {
            CheckDisposed();
            var result = CommModel_OptimizeForPowerEfficiency(_handle, out CommEnvironment optimizedEnv);
            CheckResult(result);
            return optimizedEnv;
        }

        /// <summary>
        /// 针对抗干扰能力优化
        /// </summary>
        /// <returns>优化后的环境</returns>
        public CommEnvironment OptimizeForJammerResistance()
        {
            CheckDisposed();
            var result = CommModel_OptimizeForJammerResistance(_handle, out CommEnvironment optimizedEnv);
            CheckResult(result);
            return optimizedEnv;
        }

        /// <summary>
        /// 生成详细报告
        /// </summary>
        /// <returns>详细报告</returns>
        public string GenerateDetailedReport()
        {
            CheckDisposed();
            var buffer = new StringBuilder(4096);
            var result = CommModel_GenerateDetailedReport(_handle, buffer, buffer.Capacity);
            CheckResult(result);
            return buffer.ToString();
        }

        /// <summary>
        /// 生成性能报告
        /// </summary>
        /// <returns>性能报告</returns>
        public string GeneratePerformanceReport()
        {
            CheckDisposed();
            var buffer = new StringBuilder(4096);
            var result = CommModel_GeneratePerformanceReport(_handle, buffer, buffer.Capacity);
            CheckResult(result);
            return buffer.ToString();
        }

        /// <summary>
        /// 生成干扰分析报告
        /// </summary>
        /// <returns>干扰分析报告</returns>
        public string GenerateJammingAnalysisReport()
        {
            CheckDisposed();
            var buffer = new StringBuilder(4096);
            var result = CommModel_GenerateJammingAnalysisReport(_handle, buffer, buffer.Capacity);
            CheckResult(result);
            return buffer.ToString();
        }

        /// <summary>
        /// 保存配置
        /// </summary>
        /// <param name="filename">文件名</param>
        public void SaveConfiguration(string filename)
        {
            CheckDisposed();
            var result = CommModel_SaveConfiguration(_handle, filename);
            CheckResult(result);
        }

        /// <summary>
        /// 加载配置
        /// </summary>
        /// <param name="filename">文件名</param>
        public void LoadConfiguration(string filename)
        {
            CheckDisposed();
            var result = CommModel_LoadConfiguration(_handle, filename);
            CheckResult(result);
        }

        /// <summary>
        /// 导出配置为JSON
        /// </summary>
        /// <returns>JSON字符串</returns>
        public string ExportConfigurationToJSON()
        {
            CheckDisposed();
            var buffer = new StringBuilder(4096);
            var result = CommModel_ExportConfigurationToJSON(_handle, buffer, buffer.Capacity);
            CheckResult(result);
            return buffer.ToString();
        }

        /// <summary>
        /// 从JSON导入配置
        /// </summary>
        /// <param name="jsonStr">JSON字符串</param>
        public void ImportConfigurationFromJSON(string jsonStr)
        {
            CheckDisposed();
            var result = CommModel_ImportConfigurationFromJSON(_handle, jsonStr);
            CheckResult(result);
        }

        /// <summary>
        /// 获取版本信息
        /// </summary>
        /// <returns>版本信息</returns>
        public static string GetVersion()
        {
            var buffer = new StringBuilder(256);
            var result = CommModel_GetVersion(buffer, buffer.Capacity);
            CheckResult(result);
            return buffer.ToString();
        }

        /// <summary>
        /// 获取构建信息
        /// </summary>
        /// <returns>构建信息</returns>
        public static string GetBuildInfo()
        {
            var buffer = new StringBuilder(512);
            var result = CommModel_GetBuildInfo(buffer, buffer.Capacity);
            CheckResult(result);
            return buffer.ToString();
        }

        /// <summary>
        /// 获取模型信息
        /// </summary>
        /// <returns>模型信息</returns>
        public string GetModelInfo()
        {
            CheckDisposed();
            var buffer = new StringBuilder(512);
            var result = CommModel_GetModelInfo(_handle, buffer, buffer.Capacity);
            CheckResult(result);
            return buffer.ToString();
        }

        #endregion

        #region IDisposable实现

        /// <summary>
        /// 释放资源
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// 释放资源
        /// </summary>
        /// <param name="disposing">是否正在释放</param>
        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (_handle != IntPtr.Zero)
                {
                    CommModel_Destroy(_handle);
                    _handle = IntPtr.Zero;
                }
                _disposed = true;
            }
        }

        #endregion

        #region 私有辅助方法

        /// <summary>
        /// 检查是否已释放
        /// </summary>
        private void CheckDisposed()
        {
            if (_disposed)
            {
                throw new ObjectDisposedException(nameof(CommunicationModelAPI));
            }
        }

        /// <summary>
        /// 检查结果
        /// </summary>
        /// <param name="result">结果</param>
        private static void CheckResult(CommResult result)
        {
            if (result != CommResult.Success)
            {
                throw new CommunicationModelException($"Operation failed with result: {result}");
            }
        }

        #endregion
    }

    /// <summary>
    /// 通信模型异常类
    /// </summary>
    public class CommunicationModelException : Exception
    {
        public CommunicationModelException(string message) : base(message) { }
        public CommunicationModelException(string message, Exception innerException) : base(message, innerException) { }
    }
}