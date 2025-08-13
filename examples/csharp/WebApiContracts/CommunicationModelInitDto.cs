using System.Collections.Generic;

namespace CommunicationModel.WebApi.Contracts
{
    /// <summary>
    /// 通信模型初始化请求（用于WebAPI传入）
    /// 将环境、干扰/抗干扰、距离等模型所需基础参数统一封装
    /// </summary>
    public class CommunicationModelInitDto
    {
        /// <summary>
        /// 通信场景（必填）。
        /// 取值范围：参见 <see cref="Scenario"/> 枚举；默认值：NormalCommunication。
        /// </summary>
        public Scenario Scenario { get; set; } = Scenario.NormalCommunication;

        /// <summary>
        /// 环境与链路基础参数（必填）。
        /// </summary>
        public EnvironmentDto Environment { get; set; } = new EnvironmentDto();

        /// <summary>
        /// 干扰环境参数（可选）。当存在干扰时（IsJammed=true）建议填写其余字段。
        /// </summary>
        public JammingDto Jamming { get; set; } = new JammingDto();

        /// <summary>
        /// 抗干扰选项（可选）。当场景为 AntiJamCommunication 时建议启用。
        /// </summary>
        public AntiJammingDto AntiJamming { get; set; } = new AntiJammingDto();
    }

    /// <summary>
    /// 场景定义（保持与 C API CommScenario 一致）
    /// </summary>
    public enum Scenario
    {
        /// <summary>
        /// 普通通信
        /// </summary>
        NormalCommunication = 0,
        /// <summary>
        /// 存在干扰的通信
        /// </summary>
        JammedCommunication = 1,
        /// <summary>
        /// 抗干扰通信
        /// </summary>
        AntiJamCommunication = 2,
        /// <summary>
        /// 多用户通信
        /// </summary>
        MultiUserCommunication = 3,
        /// <summary>
        /// 中继通信
        /// </summary>
        RelayCommunication = 4,
        /// <summary>
        /// Mesh 通信
        /// </summary>
        MeshCommunication = 5
    }

    /// <summary>
    /// 环境类型（保持与 C API CommEnvironmentType 一致）
    /// </summary>
    public enum EnvironmentType
    {
        /// <summary>自由空间</summary>
        FreeSpace = 0,
        /// <summary>城市</summary>
        Urban = 1,
        /// <summary>郊区</summary>
        Suburban = 2,
        /// <summary>农村</summary>
        Rural = 3,
        /// <summary>室内</summary>
        Indoor = 4,
        /// <summary>海上/近海</summary>
        Maritime = 5,
        /// <summary>山区</summary>
        Mountainous = 6
    }

    /// <summary>
    /// 干扰类型（保持与 C API CommJammerType 一致）
    /// </summary>
    public enum JammerType
    {
        /// <summary>高斯白噪声</summary>
        GaussianNoise = 0,
        /// <summary>窄带干扰</summary>
        Narrowband = 1,
        /// <summary>扫频干扰</summary>
        Sweep = 2,
        /// <summary>脉冲干扰</summary>
        Pulse = 3,
        /// <summary>压制式宽带干扰</summary>
        Barrage = 4,
        /// <summary>点频干扰</summary>
        Spot = 5
    }

    /// <summary>
    /// 环境与链路基础参数（对应 C API CommEnvironment）
    /// 单位约定：
    /// - 频率/带宽/干扰频率/干扰带宽：MHz
    /// - 功率：dBm
    /// - 距离：km
    /// - 温度：℃
    /// - 湿度：%
    /// - 压力：kPa
    /// </summary>
    public class EnvironmentDto
    {
        /// <summary>
        /// 工作频率 (MHz)（必填）。取值范围：1-100000。
        /// </summary>
        public double Frequency { get; set; }

        /// <summary>
        /// 系统带宽 (MHz)（必填）。取值范围：(0, 1000]。
        /// </summary>
        public double Bandwidth { get; set; }

        /// <summary>
        /// 发射功率 (dBm)（必填）。取值范围：-30 至 60。
        /// </summary>
        public double TransmitPower { get; set; }

        /// <summary>
        /// 噪声功率 (dBm)（必填）。取值范围：-150 至 0。
        /// </summary>
        public double NoisePower { get; set; }

        /// <summary>
        /// 通信距离 (km)（必填）。取值范围：0-1000。
        /// </summary>
        public double Distance { get; set; }

        /// <summary>
        /// 环境类型（必填）。
        /// </summary>
        public EnvironmentType EnvironmentType { get; set; } = EnvironmentType.FreeSpace;

        /// <summary>
        /// 环境温度 (℃)（可选）。推荐范围：-50 至 60。
        /// </summary>
        public double Temperature { get; set; }

        /// <summary>
        /// 湿度 (%)（可选）。取值范围：0-100。
        /// </summary>
        public double Humidity { get; set; }

        /// <summary>
        /// 大气压力 (kPa)（可选）。推荐范围：80-120。
        /// </summary>
        public double AtmosphericPressure { get; set; }
    }

    /// <summary>
    /// 干扰环境参数（对应 C API CommJammingEnvironment）
    /// </summary>
    public class JammingDto
    {
        /// <summary>
        /// 是否存在干扰（必填）。
        /// </summary>
        public bool IsJammed { get; set; }

        /// <summary>
        /// 干扰类型（可选，IsJammed=true 时建议填写）。
        /// </summary>
        public JammerType JammerType { get; set; } = JammerType.GaussianNoise;

        /// <summary>
        /// 干扰功率 (dBm)（可选）。典型范围：-150 至 60。
        /// </summary>
        public double JammerPower { get; set; }

        /// <summary>
        /// 干扰频率 (MHz)（可选）。范围：1-100000（窄带/点频干扰常用）。
        /// </summary>
        public double JammerFrequency { get; set; }

        /// <summary>
        /// 干扰带宽 (MHz)（可选）。范围：(0, 1000]（宽带/压制/扫频干扰常用）。
        /// </summary>
        public double JammerBandwidth { get; set; }

        /// <summary>
        /// 干扰机距离 (km)（可选）。范围：0-1000。
        /// </summary>
        public double JammerDistance { get; set; }

        /// <summary>
        /// 干扰机密度 (0-1)（可选）。
        /// </summary>
        public double JammerDensity { get; set; }

        /// <summary>
        /// 多个干扰频率 (MHz)（可选）。每个值范围：1-100000；建议总数不超过 100。
        /// </summary>
        public List<double>? JammerFrequencies { get; set; }
    }

    /// <summary>
    /// 抗干扰配置（当前模型基础版本只需要开关与可选目标指标）
    /// </summary>
    public class AntiJammingDto
    {
        /// <summary>
        /// 是否启用抗干扰策略（必填）。
        /// </summary>
        public bool Enable { get; set; }

        /// <summary>
        /// 目标误码率 (0-1，不含0与1)（可选）。
        /// </summary>
        public double? TargetBER { get; set; }
    }
}