using System;
using System.Collections.Generic;
using System.Linq;

namespace CommunicationModel.WebApi.Contracts
{
    /// <summary>
    /// 通信模型初始化参数校验器
    /// </summary>
    public static class CommunicationModelInitDtoValidator
    {
        /// <summary>
        /// 校验通信模型初始化参数
        /// </summary>
        /// <param name="dto">待校验的参数对象</param>
        /// <returns>校验结果</returns>
        public static ValidationResult Validate(CommunicationModelInitDto dto)
        {
            var result = new ValidationResult();

            if (dto == null)
            {
                result.AddError("", "通信模型初始化参数不能为空");
                return result;
            }

            // 校验场景
            ValidateScenario(dto.Scenario, result);

            // 校验环境参数
            ValidateEnvironment(dto.Environment, result);

            // 校验干扰参数
            ValidateJamming(dto.Jamming, result);

            // 校验抗干扰参数
            ValidateAntiJamming(dto.AntiJamming, result);

            // 校验场景与参数的一致性
            ValidateScenarioConsistency(dto, result);

            return result;
        }

        private static void ValidateScenario(Scenario scenario, ValidationResult result)
        {
            if (!Enum.IsDefined(typeof(Scenario), scenario))
            {
                result.AddError("Scenario", $"无效的通信场景值: {scenario}");
            }
        }

        private static void ValidateEnvironment(EnvironmentDto env, ValidationResult result)
        {
            if (env == null)
            {
                result.AddError("Environment", "环境参数不能为空");
                return;
            }

            // 工作频率 [1, 100000] MHz
            if (env.Frequency <= 0 || env.Frequency > 100000)
            {
                result.AddError("Environment.Frequency", "工作频率必须在 1-100000 MHz 范围内");
            }

            // 系统带宽 (0, 1000] MHz
            if (env.Bandwidth <= 0 || env.Bandwidth > 1000)
            {
                result.AddError("Environment.Bandwidth", "系统带宽必须在 0-1000 MHz 范围内（不包含0）");
            }

            // 发射功率 [-30, 60] dBm
            if (env.TransmitPower < -30 || env.TransmitPower > 60)
            {
                result.AddError("Environment.TransmitPower", "发射功率必须在 -30 到 60 dBm 范围内");
            }

            // 噪声功率 [-150, 0] dBm
            if (env.NoisePower < -150 || env.NoisePower > 0)
            {
                result.AddError("Environment.NoisePower", "噪声功率必须在 -150 到 0 dBm 范围内");
            }

            // 通信距离 [0, 1000] km
            if (env.Distance < 0 || env.Distance > 1000)
            {
                result.AddError("Environment.Distance", "通信距离必须在 0-1000 km 范围内");
            }

            // 环境类型
            if (!Enum.IsDefined(typeof(EnvironmentType), env.EnvironmentType))
            {
                result.AddError("Environment.EnvironmentType", $"无效的环境类型值: {env.EnvironmentType}");
            }

            // 环境温度 [-50, 60] ℃（可选）
            if (env.Temperature != 0 && (env.Temperature < -50 || env.Temperature > 60))
            {
                result.AddWarning("Environment.Temperature", "环境温度建议在 -50 到 60 ℃ 范围内");
            }

            // 湿度 [0, 100] %（可选）
            if (env.Humidity != 0 && (env.Humidity < 0 || env.Humidity > 100))
            {
                result.AddError("Environment.Humidity", "湿度必须在 0-100% 范围内");
            }

            // 大气压力 [80, 120] kPa（可选）
            if (env.AtmosphericPressure != 0 && (env.AtmosphericPressure < 80 || env.AtmosphericPressure > 120))
            {
                result.AddWarning("Environment.AtmosphericPressure", "大气压力建议在 80-120 kPa 范围内");
            }
        }

        private static void ValidateJamming(JammingDto jamming, ValidationResult result)
        {
            if (jamming == null)
            {
                result.AddError("Jamming", "干扰参数不能为空");
                return;
            }

            // 如果存在干扰，则需要校验干扰相关参数
            if (jamming.IsJammed)
            {
                // 干扰类型
                if (!Enum.IsDefined(typeof(JammerType), jamming.JammerType))
                {
                    result.AddError("Jamming.JammerType", $"无效的干扰类型值: {jamming.JammerType}");
                }

                // 干扰功率 [-150, 60] dBm
                if (jamming.JammerPower < -150 || jamming.JammerPower > 60)
                {
                    result.AddError("Jamming.JammerPower", "干扰功率必须在 -150 到 60 dBm 范围内");
                }

                // 干扰频率 [0, 100000] MHz（针对窄带干扰等）
                if (jamming.JammerFrequency > 0 && (jamming.JammerFrequency <= 0 || jamming.JammerFrequency > 100000))
                {
                    result.AddError("Jamming.JammerFrequency", "干扰频率必须在 1-100000 MHz 范围内");
                }

                // 干扰带宽 (0, 1000] MHz（针对宽带干扰等）
                if (jamming.JammerBandwidth > 0 && (jamming.JammerBandwidth <= 0 || jamming.JammerBandwidth > 1000))
                {
                    result.AddError("Jamming.JammerBandwidth", "干扰带宽必须在 0-1000 MHz 范围内（不包含0）");
                }

                // 干扰机距离 [0, 1000] km
                if (jamming.JammerDistance < 0 || jamming.JammerDistance > 1000)
                {
                    result.AddError("Jamming.JammerDistance", "干扰机距离必须在 0-1000 km 范围内");
                }

                // 干扰机密度 [0, 1]
                if (jamming.JammerDensity < 0 || jamming.JammerDensity > 1)
                {
                    result.AddError("Jamming.JammerDensity", "干扰机密度必须在 0-1 范围内");
                }

                // 多个干扰频率校验
                if (jamming.JammerFrequencies != null && jamming.JammerFrequencies.Any())
                {
                    for (int i = 0; i < jamming.JammerFrequencies.Count; i++)
                    {
                        var freq = jamming.JammerFrequencies[i];
                        if (freq <= 0 || freq > 100000)
                        {
                            result.AddError($"Jamming.JammerFrequencies[{i}]", $"干扰频率 #{i + 1} 必须在 1-100000 MHz 范围内");
                        }
                    }

                    // 检查频率数组长度限制
                    if (jamming.JammerFrequencies.Count > 100)
                    {
                        result.AddWarning("Jamming.JammerFrequencies", "干扰频率数组长度过大，建议不超过100个");
                    }
                }
            }
        }

        private static void ValidateAntiJamming(AntiJammingDto antiJamming, ValidationResult result)
        {
            if (antiJamming == null)
            {
                result.AddError("AntiJamming", "抗干扰参数不能为空");
                return;
            }

            // 如果启用抗干扰且设置了目标误码率
            if (antiJamming.Enable && antiJamming.TargetBER.HasValue)
            {
                // 目标误码率 (0, 1)
                if (antiJamming.TargetBER.Value <= 0 || antiJamming.TargetBER.Value >= 1)
                {
                    result.AddError("AntiJamming.TargetBER", "目标误码率必须在 0-1 范围内（不包含0和1）");
                }
            }
        }

        private static void ValidateScenarioConsistency(CommunicationModelInitDto dto, ValidationResult result)
        {
            // 场景与干扰参数的一致性检查
            if (dto.Scenario == Scenario.JammedCommunication || dto.Scenario == Scenario.AntiJamCommunication)
            {
                if (!dto.Jamming.IsJammed)
                {
                    result.AddWarning("Scenario", $"场景 {dto.Scenario} 通常需要设置干扰参数 (Jamming.IsJammed = true)");
                }
            }

            // 抗干扰场景与抗干扰参数的一致性检查
            if (dto.Scenario == Scenario.AntiJamCommunication)
            {
                if (!dto.AntiJamming.Enable)
                {
                    result.AddWarning("Scenario", "抗干扰通信场景建议启用抗干扰策略 (AntiJamming.Enable = true)");
                }
            }

            // 普通通信场景的建议
            if (dto.Scenario == Scenario.NormalCommunication)
            {
                if (dto.Jamming.IsJammed)
                {
                    result.AddWarning("Scenario", "普通通信场景通常不需要设置干扰参数");
                }
                if (dto.AntiJamming.Enable)
                {
                    result.AddWarning("Scenario", "普通通信场景通常不需要启用抗干扰策略");
                }
            }
        }
    }

    /// <summary>
    /// 校验结果
    /// </summary>
    public class ValidationResult
    {
        /// <summary>
        /// 错误列表
        /// </summary>
        public List<ValidationError> Errors { get; } = new List<ValidationError>();

        /// <summary>
        /// 警告列表
        /// </summary>
        public List<ValidationWarning> Warnings { get; } = new List<ValidationWarning>();

        /// <summary>
        /// 是否校验通过（无错误）
        /// </summary>
        public bool IsValid => !Errors.Any();

        /// <summary>
        /// 是否有警告
        /// </summary>
        public bool HasWarnings => Warnings.Any();

        /// <summary>
        /// 添加错误
        /// </summary>
        /// <param name="propertyName">属性名称</param>
        /// <param name="errorMessage">错误信息</param>
        public void AddError(string propertyName, string errorMessage)
        {
            Errors.Add(new ValidationError(propertyName, errorMessage));
        }

        /// <summary>
        /// 添加警告
        /// </summary>
        /// <param name="propertyName">属性名称</param>
        /// <param name="warningMessage">警告信息</param>
        public void AddWarning(string propertyName, string warningMessage)
        {
            Warnings.Add(new ValidationWarning(propertyName, warningMessage));
        }

        /// <summary>
        /// 获取所有错误信息的摘要
        /// </summary>
        /// <returns>错误摘要字符串</returns>
        public string GetErrorSummary()
        {
            if (IsValid)
                return "校验通过";

            return string.Join("; ", Errors.Select(e => $"{e.PropertyName}: {e.Message}"));
        }

        /// <summary>
        /// 获取所有警告信息的摘要
        /// </summary>
        /// <returns>警告摘要字符串</returns>
        public string GetWarningSummary()
        {
            if (!HasWarnings)
                return "";

            return string.Join("; ", Warnings.Select(w => $"{w.PropertyName}: {w.Message}"));
        }
    }

    /// <summary>
    /// 校验错误
    /// </summary>
    public class ValidationError
    {
        /// <summary>
        /// 属性名称
        /// </summary>
        public string PropertyName { get; }

        /// <summary>
        /// 错误信息
        /// </summary>
        public string Message { get; }

        public ValidationError(string propertyName, string message)
        {
            PropertyName = propertyName;
            Message = message;
        }

        public override string ToString()
        {
            return string.IsNullOrEmpty(PropertyName) ? Message : $"{PropertyName}: {Message}";
        }
    }

    /// <summary>
    /// 校验警告
    /// </summary>
    public class ValidationWarning
    {
        /// <summary>
        /// 属性名称
        /// </summary>
        public string PropertyName { get; }

        /// <summary>
        /// 警告信息
        /// </summary>
        public string Message { get; }

        public ValidationWarning(string propertyName, string message)
        {
            PropertyName = propertyName;
            Message = message;
        }

        public override string ToString()
        {
            return string.IsNullOrEmpty(PropertyName) ? Message : $"{PropertyName}: {Message}";
        }
    }
}