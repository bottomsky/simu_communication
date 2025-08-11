using System;
using CommunicationModel;

namespace CommunicationModelExample
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("=== 通信模型C#调用示例 ===");
            Console.WriteLine();

            try
            {
                // 显示版本信息
                Console.WriteLine($"库版本: {CommunicationModelAPI.GetVersion()}");
                Console.WriteLine($"构建信息: {CommunicationModelAPI.GetBuildInfo()}");
                Console.WriteLine();

                // 创建通信模型实例
                using (var commModel = new CommunicationModelAPI(CommScenario.NormalCommunication))
                {
                    Console.WriteLine($"模型信息: {commModel.GetModelInfo()}");
                    Console.WriteLine();

                    // 基础通信场景测试
                    TestBasicCommunication(commModel);
                    Console.WriteLine();

                    // 干扰场景测试
                    TestJammedCommunication(commModel);
                    Console.WriteLine();

                    // 性能优化测试
                    TestPerformanceOptimization(commModel);
                    Console.WriteLine();

                    // 报告生成测试
                    TestReportGeneration(commModel);
                    Console.WriteLine();

                    // 配置管理测试
                    TestConfigurationManagement(commModel);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"错误: {ex.Message}");
                if (ex.InnerException != null)
                {
                    Console.WriteLine($"内部错误: {ex.InnerException.Message}");
                }
            }

            Console.WriteLine();
            Console.WriteLine("按任意键退出...");
            Console.ReadKey();
        }

        /// <summary>
        /// 测试基础通信功能
        /// </summary>
        /// <param name="commModel">通信模型实例</param>
        static void TestBasicCommunication(CommunicationModelAPI commModel)
        {
            Console.WriteLine("=== 基础通信测试 ===");

            // 设置通信环境
            var environment = new CommEnvironment
            {
                Frequency = 2.4e9,           // 2.4 GHz
                Bandwidth = 20e6,            // 20 MHz
                TransmitPower = 1.0,         // 1 W
                NoisePower = 1e-12,          // -90 dBm
                Distance = 1000.0,           // 1 km
                EnvironmentType = CommEnvironmentType.Urban,
                Temperature = 290.0,         // 290 K
                Humidity = 50.0,             // 50%
                AtmosphericPressure = 101325.0 // 标准大气压
            };

            commModel.SetEnvironment(environment);

            // 计算链路状态
            var linkStatus = commModel.CalculateLinkStatus();
            Console.WriteLine($"链路连接状态: {(linkStatus.IsConnected != 0 ? "已连接" : "未连接")}");
            Console.WriteLine($"信号强度: {linkStatus.SignalStrength:F2} dBm");
            Console.WriteLine($"信噪比: {linkStatus.SignalToNoiseRatio:F2} dB");
            Console.WriteLine($"误码率: {linkStatus.BitErrorRate:E2}");
            Console.WriteLine($"吞吐量: {linkStatus.Throughput / 1e6:F2} Mbps");
            Console.WriteLine($"延迟: {linkStatus.Latency * 1000:F2} ms");
            Console.WriteLine($"丢包率: {linkStatus.PacketLossRate * 100:F2}%");
            Console.WriteLine($"通信质量: {linkStatus.Quality}");
            Console.WriteLine($"状态描述: {linkStatus.StatusDescription}");

            // 计算性能指标
            var performance = commModel.CalculatePerformance();
            Console.WriteLine($"有效通信距离: {performance.EffectiveRange / 1000:F2} km");
            Console.WriteLine($"最大数据速率: {performance.MaxDataRate / 1e6:F2} Mbps");
            Console.WriteLine($"功率效率: {performance.PowerEfficiency:F2}");
            Console.WriteLine($"频谱效率: {performance.SpectralEfficiency:F2} bps/Hz");
            Console.WriteLine($"可靠性: {performance.Reliability * 100:F2}%");
            Console.WriteLine($"可用性: {performance.Availability * 100:F2}%");

            // 计算通信距离和所需功率
            var commRange = commModel.CalculateCommunicationRange();
            Console.WriteLine($"当前配置下的通信距离: {commRange / 1000:F2} km");

            var requiredPower = commModel.CalculateRequiredPower(2000.0); // 2 km
            Console.WriteLine($"2km通信所需功率: {requiredPower:F2} W");

            // 计算最优参数
            var optimalFreq = commModel.CalculateOptimalFrequency();
            var optimalBandwidth = commModel.CalculateOptimalBandwidth();
            Console.WriteLine($"最优频率: {optimalFreq / 1e9:F2} GHz");
            Console.WriteLine($"最优带宽: {optimalBandwidth / 1e6:F2} MHz");
        }

        /// <summary>
        /// 测试干扰通信功能
        /// </summary>
        /// <param name="commModel">通信模型实例</param>
        static void TestJammedCommunication(CommunicationModelAPI commModel)
        {
            Console.WriteLine("=== 干扰通信测试 ===");

            // 切换到干扰场景
            commModel.SetScenario(CommScenario.JammedCommunication);

            // 设置干扰环境
            var jammingEnv = new CommJammingEnvironment
            {
                IsJammed = 1,
                JammerType = CommJammerType.GaussianNoise,
                JammerPower = 10.0,          // 10 W
                JammerFrequency = 2.4e9,     // 2.4 GHz
                JammerBandwidth = 50e6,      // 50 MHz
                JammerDistance = 500.0,      // 500 m
                JammerDensity = 1.0,         // 1 jammer per km²
                JammerFrequencies = IntPtr.Zero,
                JammerFrequencyCount = 0
            };

            commModel.SetJammingEnvironment(jammingEnv);

            // 计算干扰分析
            var jammerEffectiveness = commModel.CalculateJammerEffectiveness();
            Console.WriteLine($"干扰有效性: {jammerEffectiveness * 100:F2}%");

            var jsRatio = commModel.CalculateJammerToSignalRatio();
            Console.WriteLine($"干信比: {jsRatio:F2} dB");

            var requiredAntiJamGain = commModel.CalculateRequiredAntiJamGain(1e-6);
            Console.WriteLine($"达到1e-6误码率所需抗干扰增益: {requiredAntiJamGain:F2} dB");

            // 计算干扰覆盖范围
            var jammerCoverage = commModel.CalculateJammerCoverage();
            if (jammerCoverage.Length > 0)
            {
                Console.WriteLine("干扰覆盖范围:");
                for (int i = 0; i < Math.Min(jammerCoverage.Length, 5); i++)
                {
                    Console.WriteLine($"  距离 {i * 100} m: {jammerCoverage[i] / 1000:F2} km");
                }
            }

            // 切换到抗干扰场景
            commModel.SetScenario(CommScenario.AntiJamCommunication);
            var antiJamEffectiveness = commModel.CalculateAntiJamEffectiveness();
            Console.WriteLine($"抗干扰有效性: {antiJamEffectiveness * 100:F2}%");

            // 重新计算性能
            var performance = commModel.CalculatePerformance();
            Console.WriteLine($"抗干扰模式下的可靠性: {performance.Reliability * 100:F2}%");
            Console.WriteLine($"抗干扰能力: {performance.JammerResistance * 100:F2}%");
            Console.WriteLine($"抗截获能力: {performance.InterceptionResistance * 100:F2}%");
        }

        /// <summary>
        /// 测试性能优化功能
        /// </summary>
        /// <param name="commModel">通信模型实例</param>
        static void TestPerformanceOptimization(CommunicationModelAPI commModel)
        {
            Console.WriteLine("=== 性能优化测试 ===");

            // 切换回正常通信场景
            commModel.SetScenario(CommScenario.NormalCommunication);

            // 针对距离优化
            var rangeOptimizedEnv = commModel.OptimizeForRange(5000.0); // 5 km
            Console.WriteLine("针对5km距离优化:");
            Console.WriteLine($"  优化频率: {rangeOptimizedEnv.Frequency / 1e9:F2} GHz");
            Console.WriteLine($"  优化功率: {rangeOptimizedEnv.TransmitPower:F2} W");
            Console.WriteLine($"  优化带宽: {rangeOptimizedEnv.Bandwidth / 1e6:F2} MHz");

            // 针对数据速率优化
            var dataRateOptimizedEnv = commModel.OptimizeForDataRate(100e6); // 100 Mbps
            Console.WriteLine("针对100Mbps数据速率优化:");
            Console.WriteLine($"  优化频率: {dataRateOptimizedEnv.Frequency / 1e9:F2} GHz");
            Console.WriteLine($"  优化功率: {dataRateOptimizedEnv.TransmitPower:F2} W");
            Console.WriteLine($"  优化带宽: {dataRateOptimizedEnv.Bandwidth / 1e6:F2} MHz");

            // 针对功率效率优化
            var powerOptimizedEnv = commModel.OptimizeForPowerEfficiency();
            Console.WriteLine("针对功率效率优化:");
            Console.WriteLine($"  优化频率: {powerOptimizedEnv.Frequency / 1e9:F2} GHz");
            Console.WriteLine($"  优化功率: {powerOptimizedEnv.TransmitPower:F2} W");
            Console.WriteLine($"  优化带宽: {powerOptimizedEnv.Bandwidth / 1e6:F2} MHz");

            // 针对抗干扰能力优化
            var jammerResistanceOptimizedEnv = commModel.OptimizeForJammerResistance();
            Console.WriteLine("针对抗干扰能力优化:");
            Console.WriteLine($"  优化频率: {jammerResistanceOptimizedEnv.Frequency / 1e9:F2} GHz");
            Console.WriteLine($"  优化功率: {jammerResistanceOptimizedEnv.TransmitPower:F2} W");
            Console.WriteLine($"  优化带宽: {jammerResistanceOptimizedEnv.Bandwidth / 1e6:F2} MHz");
        }

        /// <summary>
        /// 测试报告生成功能
        /// </summary>
        /// <param name="commModel">通信模型实例</param>
        static void TestReportGeneration(CommunicationModelAPI commModel)
        {
            Console.WriteLine("=== 报告生成测试 ===");

            // 生成详细报告
            var detailedReport = commModel.GenerateDetailedReport();
            Console.WriteLine("详细报告 (前200字符):");
            Console.WriteLine(detailedReport.Length > 200 ? 
                detailedReport.Substring(0, 200) + "..." : detailedReport);
            Console.WriteLine();

            // 生成性能报告
            var performanceReport = commModel.GeneratePerformanceReport();
            Console.WriteLine("性能报告 (前200字符):");
            Console.WriteLine(performanceReport.Length > 200 ? 
                performanceReport.Substring(0, 200) + "..." : performanceReport);
            Console.WriteLine();

            // 生成干扰分析报告
            var jammingReport = commModel.GenerateJammingAnalysisReport();
            Console.WriteLine("干扰分析报告 (前200字符):");
            Console.WriteLine(jammingReport.Length > 200 ? 
                jammingReport.Substring(0, 200) + "..." : jammingReport);
        }

        /// <summary>
        /// 测试配置管理功能
        /// </summary>
        /// <param name="commModel">通信模型实例</param>
        static void TestConfigurationManagement(CommunicationModelAPI commModel)
        {
            Console.WriteLine("=== 配置管理测试 ===");

            try
            {
                // 导出配置为JSON
                var jsonConfig = commModel.ExportConfigurationToJSON();
                Console.WriteLine("当前配置JSON (前300字符):");
                Console.WriteLine(jsonConfig.Length > 300 ? 
                    jsonConfig.Substring(0, 300) + "..." : jsonConfig);
                Console.WriteLine();

                // 保存配置到文件
                var configFile = "test_config.json";
                commModel.SaveConfiguration(configFile);
                Console.WriteLine($"配置已保存到文件: {configFile}");

                // 修改一些参数
                commModel.SetFrequency(5.8e9); // 改为5.8 GHz
                commModel.SetTransmitPower(2.0); // 改为2W

                Console.WriteLine("参数修改后:");
                var env = commModel.GetEnvironment();
                Console.WriteLine($"  频率: {env.Frequency / 1e9:F2} GHz");
                Console.WriteLine($"  功率: {env.TransmitPower:F2} W");

                // 从文件加载配置
                commModel.LoadConfiguration(configFile);
                Console.WriteLine("从文件加载配置后:");
                env = commModel.GetEnvironment();
                Console.WriteLine($"  频率: {env.Frequency / 1e9:F2} GHz");
                Console.WriteLine($"  功率: {env.TransmitPower:F2} W");

                // 从JSON导入配置
                commModel.ImportConfigurationFromJSON(jsonConfig);
                Console.WriteLine("从JSON导入配置完成");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"配置管理测试出错: {ex.Message}");
            }
        }
    }
}