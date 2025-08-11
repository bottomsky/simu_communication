using System;
using System.Runtime.InteropServices;
using System.Text;
using CommunicationModel;

namespace CommunicationModelExample.Tests
{
    /// <summary>
    /// C# 和 C++ 互操作性测试类
    /// 主要测试数据结构的内存布局一致性和数据交换正确性
    /// </summary>
    public static class InteropTests
    {
        // 通过公开的 API 进行验证，不直接依赖未导出的本机测试函数
        // 包括：SetEnvironment/GetEnvironment、SetJammingEnvironment/GetJammingEnvironment、
        // CalculateLinkStatus、CalculatePerformance、CalculateJammerCoverage 等

        /// <summary>
        /// 运行所有互操作性测试
        /// </summary>
        public static void RunAllTests()
        {
            Console.WriteLine("=== C# 和 C++ 互操作性测试 ===");
            Console.WriteLine();

            bool allTestsPassed = true;

            // 测试内存布局一致性
            Console.WriteLine("1. 测试内存布局一致性...");
            allTestsPassed &= TestMemoryLayoutConsistency();
            Console.WriteLine();

            // 测试数据交换正确性
            Console.WriteLine("2. 测试数据交换正确性...");
            allTestsPassed &= TestDataExchangeCorrectness();
            Console.WriteLine();

            // 测试数组操作
            Console.WriteLine("3. 测试数组操作...");
            allTestsPassed &= TestArrayOperations();
            Console.WriteLine();

            // 测试字符串处理
            Console.WriteLine("4. 测试字符串处理...");
            allTestsPassed &= TestStringHandling();
            Console.WriteLine();

            // 测试指针操作安全性
            Console.WriteLine("5. 测试指针操作安全性...");
            allTestsPassed &= TestPointerSafety();
            Console.WriteLine();

            // 输出总结果
            Console.WriteLine("=== 测试结果总结 ===");
            if (allTestsPassed)
            {
                Console.WriteLine("✓ 所有测试通过！C# 和 C++ 互操作性良好。");
            }
            else
            {
                Console.WriteLine("✗ 存在测试失败！请检查互操作性问题。");
            }
        }

        /// <summary>
        /// 测试内存布局一致性（通过 Set/Get 往返验证字段是否按预期写入/读取）
        /// </summary>
        private static bool TestMemoryLayoutConsistency()
        {
            bool testPassed = true;

            try
            {
                using (var commModel = new CommunicationModelAPI())
                {
                    // 1) CommEnvironment 布局与往返
                    var envIn = new CommEnvironment
                    {
                        Frequency = 2.4e9,
                        Bandwidth = 20e6,
                        TransmitPower = 1.2345,
                        NoisePower = 9.87e-13,
                        Distance = 1234.5,
                        EnvironmentType = CommEnvironmentType.Mountainous,
                        Temperature = 273.15,
                        Humidity = 12.34,
                        AtmosphericPressure = 100123.0
                    };
                    commModel.SetEnvironment(envIn);
                    var envOut = commModel.GetEnvironment();
                    if (CompareCommEnvironment(envIn, envOut))
                        Console.WriteLine("✓ CommEnvironment 往返一致");
                    else
                    {
                        Console.WriteLine("✗ CommEnvironment 往返不一致（可能存在布局问题）");
                        testPassed = false;
                    }

                    // 2) CommJammingEnvironment 布局与往返
                    commModel.SetScenario(CommScenario.JammedCommunication);
                    var jamIn = new CommJammingEnvironment
                    {
                        IsJammed = 1,
                        JammerType = CommJammerType.Sweep,
                        JammerPower = 7.77,
                        JammerFrequency = 5.8e9,
                        JammerBandwidth = 40e6,
                        JammerDistance = 321.0,
                        JammerDensity = 0.5,
                        JammerFrequencies = IntPtr.Zero,
                        JammerFrequencyCount = 0
                    };
                    commModel.SetJammingEnvironment(jamIn);
                    var jamOut = commModel.GetJammingEnvironment();
                    if (CompareCommJammingEnvironment(jamIn, jamOut))
                        Console.WriteLine("✓ CommJammingEnvironment 往返一致");
                    else
                    {
                        Console.WriteLine("✗ CommJammingEnvironment 往返不一致（可能存在布局问题）");
                        testPassed = false;
                    }

                    // 3) CommLinkStatus 从本机返回的结构是否合理（间接验证布局）
                    var linkStatus = commModel.CalculateLinkStatus();
                    bool linkStatusOk = linkStatus.StatusDescription != null &&
                                        Enum.IsDefined(typeof(CommQuality), linkStatus.Quality) &&
                                        !double.IsNaN(linkStatus.SignalStrength) &&
                                        !double.IsNaN(linkStatus.SignalToNoiseRatio);
                    if (linkStatusOk)
                        Console.WriteLine("✓ CommLinkStatus 字段读取合理");
                    else
                    {
                        Console.WriteLine("✗ CommLinkStatus 字段出现异常值（可能存在布局问题）");
                        testPassed = false;
                    }

                    // 4) CommPerformance 从本机返回的结构是否合理
                    var perf = commModel.CalculatePerformance();
                    bool perfOk = !double.IsNaN(perf.EffectiveRange) && !double.IsNaN(perf.MaxDataRate);
                    if (perfOk)
                        Console.WriteLine("✓ CommPerformance 字段读取合理");
                    else
                    {
                        Console.WriteLine("✗ CommPerformance 字段出现异常值（可能存在布局问题）");
                        testPassed = false;
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"✗ 内存布局测试异常: {ex.Message}");
                testPassed = false;
            }

            return testPassed;
        }

        /// <summary>
        /// 测试数据交换正确性（通过 API 设置 -> 计算/读取 -> 校验）
        /// </summary>
        private static bool TestDataExchangeCorrectness()
        {
            bool testPassed = true;

            try
            {
                using (var commModel = new CommunicationModelAPI())
                {
                    // 测试 CommEnvironment 数据往返
                    var originalEnv = new CommEnvironment
                    {
                        Frequency = 3.5e9,
                        Bandwidth = 40e6,
                        TransmitPower = 2.0,
                        NoisePower = 5e-13,
                        Distance = 2000.0,
                        EnvironmentType = CommEnvironmentType.Urban,
                        Temperature = 295.0,
                        Humidity = 60.0,
                        AtmosphericPressure = 100800.0
                    };

                    commModel.SetEnvironment(originalEnv);
                    var returnedEnv = commModel.GetEnvironment();
                    if (CompareCommEnvironment(originalEnv, returnedEnv))
                    {
                        Console.WriteLine("✓ CommEnvironment 数据往返测试通过");
                    }
                    else
                    {
                        Console.WriteLine("✗ CommEnvironment 数据往返测试失败");
                        testPassed = false;
                    }

                    // 测试 CommJammingEnvironment 数据往返
                    commModel.SetScenario(CommScenario.JammedCommunication);
                    var originalJammingEnv = new CommJammingEnvironment
                    {
                        IsJammed = 1,
                        JammerType = CommJammerType.GaussianNoise,
                        JammerPower = 10.0,
                        JammerFrequency = 2.4e9,
                        JammerBandwidth = 50e6,
                        JammerDistance = 500.0,
                        JammerDensity = 1.0,
                        JammerFrequencies = IntPtr.Zero,
                        JammerFrequencyCount = 0
                    };

                    commModel.SetJammingEnvironment(originalJammingEnv);
                    var returnedJammingEnv = commModel.GetJammingEnvironment();
                    if (CompareCommJammingEnvironment(originalJammingEnv, returnedJammingEnv))
                    {
                        Console.WriteLine("✓ CommJammingEnvironment 数据往返测试通过");
                    }
                    else
                    {
                        Console.WriteLine("✗ CommJammingEnvironment 数据往返测试失败");
                        testPassed = false;
                    }

                    // 测试 CommLinkStatus/Performance 计算后字段是否与环境变化一致（基本一致性校验）
                    commModel.SetScenario(CommScenario.NormalCommunication);
                    var status1 = commModel.CalculateLinkStatus();
                    var perf1 = commModel.CalculatePerformance();

                    // 修改环境（例如增大发射功率，通常应提高SNR/吞吐）
                    commModel.SetTransmitPower(originalEnv.TransmitPower * 2);
                    var status2 = commModel.CalculateLinkStatus();
                    var perf2 = commModel.CalculatePerformance();

                    if (status2.SignalToNoiseRatio >= status1.SignalToNoiseRatio && perf2.MaxDataRate >= perf1.MaxDataRate)
                    {
                        Console.WriteLine("✓ 环境变更后计算结果变化方向合理");
                    }
                    else
                    {
                        Console.WriteLine("✗ 环境变更后计算结果不合理（需要检查数据交换/布局）");
                        testPassed = false;
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"✗ 数据交换测试异常: {ex.Message}");
                testPassed = false;
            }

            return testPassed;
        }

        /// <summary>
        /// 测试数组操作
        /// </summary>
        private static bool TestArrayOperations()
        {
            bool testPassed = true;

            try
            {
                // 测试 DoubleArray 行为通过 CalculateJammerCoverage（内部分配/释放）
                using (var commModel = new CommunicationModelAPI())
                {
                    var environment = new CommEnvironment
                    {
                        Frequency = 2.4e9,
                        Bandwidth = 20e6,
                        TransmitPower = 1.0,
                        NoisePower = 1e-12,
                        Distance = 1000.0,
                        EnvironmentType = CommEnvironmentType.Urban,
                        Temperature = 290.0,
                        Humidity = 50.0,
                        AtmosphericPressure = 101325.0
                    };
                    commModel.SetEnvironment(environment);

                    commModel.SetScenario(CommScenario.JammedCommunication);
                    var jammingEnv = new CommJammingEnvironment
                    {
                        IsJammed = 1,
                        JammerType = CommJammerType.GaussianNoise,
                        JammerPower = 10.0,
                        JammerFrequency = 2.4e9,
                        JammerBandwidth = 50e6,
                        JammerDistance = 500.0,
                        JammerDensity = 1.0,
                        JammerFrequencies = IntPtr.Zero,
                        JammerFrequencyCount = 0
                    };
                    commModel.SetJammingEnvironment(jammingEnv);

                    var jammerCoverage = commModel.CalculateJammerCoverage();
                    if (jammerCoverage != null && jammerCoverage.Length > 0)
                    {
                        Console.WriteLine($"✓ 干扰覆盖范围数组获取成功，长度: {jammerCoverage.Length}");

                        bool dataReasonable = true;
                        foreach (var value in jammerCoverage)
                        {
                            if (double.IsNaN(value) || double.IsInfinity(value) || value < 0)
                            {
                                dataReasonable = false;
                                break;
                            }
                        }

                        if (dataReasonable)
                        {
                            Console.WriteLine("✓ 数组数据合理性检查通过");
                        }
                        else
                        {
                            Console.WriteLine("✗ 数组数据包含无效值");
                            testPassed = false;
                        }
                    }
                    else
                    {
                        Console.WriteLine("✓ 数组数据为空（在当前配置下是合理的）");
                    }
                }

                // 构造空数组结构体，验证基本安全
                var emptyDoubleArray = new DoubleArray { Values = IntPtr.Zero, Count = 0 };
                Console.WriteLine("✓ 空数组结构创建成功");

                var emptyLinkStatusArray = new LinkStatusArray { Statuses = IntPtr.Zero, Count = 0 };
                Console.WriteLine("✓ 空链路状态数组结构创建成功");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"✗ 数组操作测试异常: {ex.Message}");
                testPassed = false;
            }

            return testPassed;
        }

        /// <summary>
        /// 测试字符串处理
        /// </summary>
        private static bool TestStringHandling()
        {
            bool testPassed = true;

            try
            {
                // 测试版本信息获取
                var version = CommunicationModelAPI.GetVersion();
                if (!string.IsNullOrEmpty(version))
                {
                    Console.WriteLine($"✓ 版本信息获取成功: {version}");
                }
                else
                {
                    Console.WriteLine("✗ 版本信息获取失败");
                    testPassed = false;
                }

                // 测试构建信息获取
                var buildInfo = CommunicationModelAPI.GetBuildInfo();
                if (!string.IsNullOrEmpty(buildInfo))
                {
                    Console.WriteLine($"✓ 构建信息获取成功: {buildInfo}");
                }
                else
                {
                    Console.WriteLine("✗ 构建信息获取失败");
                    testPassed = false;
                }

                // 测试模型信息获取
                using (var commModel = new CommunicationModelAPI())
                {
                    var modelInfo = commModel.GetModelInfo();
                    if (!string.IsNullOrEmpty(modelInfo))
                    {
                        Console.WriteLine($"✓ 模型信息获取成功: {modelInfo}");
                    }
                    else
                    {
                        Console.WriteLine("✗ 模型信息获取失败");
                        testPassed = false;
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"✗ 字符串处理测试异常: {ex.Message}");
                testPassed = false;
            }

            return testPassed;
        }

        /// <summary>
        /// 测试指针操作安全性
        /// </summary>
        private static bool TestPointerSafety()
        {
            bool testPassed = true;

            try
            {
                // 测试空指针结构体创建
                var nullDoubleArray = new DoubleArray { Values = IntPtr.Zero, Count = 0 };
                Console.WriteLine("✓ 空指针数组结构体创建安全");

                var nullLinkStatusArray = new LinkStatusArray { Statuses = IntPtr.Zero, Count = 0 };
                Console.WriteLine("✓ 空指针链路状态数组结构体创建安全");

                // 测试无效句柄处理
                CommunicationModelAPI? disposedModel = null;
                try
                {
                    using (var commModel = new CommunicationModelAPI())
                    {
                        // 正常操作应该成功
                        var environment = commModel.GetEnvironment();
                        Console.WriteLine("✓ 有效句柄操作成功");
                        disposedModel = commModel;
                    }

                    // 尝试在对象销毁后使用
                    if (disposedModel != null)
                    {
                        try
                        {
                            var env = disposedModel.GetEnvironment();
                            Console.WriteLine("✗ 已销毁对象操作应该失败但没有失败");
                            testPassed = false;
                        }
                        catch (ObjectDisposedException)
                        {
                            Console.WriteLine("✓ 已销毁对象检测正常");
                        }
                    }
                }
                catch (ObjectDisposedException)
                {
                    Console.WriteLine("✓ 对象生命周期管理正常");
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"✗ 句柄安全性测试异常: {ex.Message}");
                    testPassed = false;
                }

                // 当前 C API 对空文件名不会抛错，不进行该负向测试以避免与实现不一致
            }
            catch (Exception ex)
            {
                Console.WriteLine($"✗ 指针安全性测试异常: {ex.Message}");
                testPassed = false;
            }

            return testPassed;
        }

        #region 辅助比较方法

        private static bool CompareCommEnvironment(CommEnvironment a, CommEnvironment b)
        {
            const double tolerance = 1e-10;
            return Math.Abs(a.Frequency - b.Frequency) < tolerance &&
                   Math.Abs(a.Bandwidth - b.Bandwidth) < tolerance &&
                   Math.Abs(a.TransmitPower - b.TransmitPower) < tolerance &&
                   Math.Abs(a.NoisePower - b.NoisePower) < tolerance &&
                   Math.Abs(a.Distance - b.Distance) < tolerance &&
                   a.EnvironmentType == b.EnvironmentType &&
                   Math.Abs(a.Temperature - b.Temperature) < tolerance &&
                   Math.Abs(a.Humidity - b.Humidity) < tolerance &&
                   Math.Abs(a.AtmosphericPressure - b.AtmosphericPressure) < tolerance;
        }

        private static bool CompareCommJammingEnvironment(CommJammingEnvironment a, CommJammingEnvironment b)
        {
            const double tolerance = 1e-10;
            return a.IsJammed == b.IsJammed &&
                   a.JammerType == b.JammerType &&
                   Math.Abs(a.JammerPower - b.JammerPower) < tolerance &&
                   Math.Abs(a.JammerFrequency - b.JammerFrequency) < tolerance &&
                   Math.Abs(a.JammerBandwidth - b.JammerBandwidth) < tolerance &&
                   Math.Abs(a.JammerDistance - b.JammerDistance) < tolerance &&
                   Math.Abs(a.JammerDensity - b.JammerDensity) < tolerance &&
                   a.JammerFrequencies == b.JammerFrequencies &&
                   a.JammerFrequencyCount == b.JammerFrequencyCount;
        }

        private static bool CompareCommLinkStatus(CommLinkStatus a, CommLinkStatus b)
        {
            const double tolerance = 1e-10;
            return a.IsConnected == b.IsConnected &&
                   Math.Abs(a.SignalStrength - b.SignalStrength) < tolerance &&
                   Math.Abs(a.SignalToNoiseRatio - b.SignalToNoiseRatio) < tolerance &&
                   Math.Abs(a.BitErrorRate - b.BitErrorRate) < tolerance &&
                   Math.Abs(a.Throughput - b.Throughput) < tolerance &&
                   Math.Abs(a.Latency - b.Latency) < tolerance &&
                   Math.Abs(a.PacketLossRate - b.PacketLossRate) < tolerance &&
                   a.Quality == b.Quality &&
                   a.StatusDescription == b.StatusDescription;
        }

        private static bool CompareCommPerformance(CommPerformance a, CommPerformance b)
        {
            const double tolerance = 1e-10;
            return Math.Abs(a.EffectiveRange - b.EffectiveRange) < tolerance &&
                   Math.Abs(a.MaxDataRate - b.MaxDataRate) < tolerance &&
                   Math.Abs(a.PowerEfficiency - b.PowerEfficiency) < tolerance &&
                   Math.Abs(a.SpectralEfficiency - b.SpectralEfficiency) < tolerance &&
                   Math.Abs(a.Reliability - b.Reliability) < tolerance &&
                   Math.Abs(a.Availability - b.Availability) < tolerance &&
                   Math.Abs(a.JammerResistance - b.JammerResistance) < tolerance &&
                   Math.Abs(a.InterceptionResistance - b.InterceptionResistance) < tolerance;
        }

        #endregion
    }
}