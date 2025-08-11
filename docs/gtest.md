# GoogleTest 测试概览（tests/unit/test_communication_model_api_basic.cpp）

本文档对文件 tests/unit/test_communication_model_api_basic.cpp 中的 GoogleTest 测试夹具与测试用例做简要说明，帮助快速理解测试覆盖的功能点。

## 测试夹具
- 夹具类：CommunicationModelAPIBasicTest（继承 ::testing::Test）
  - SetUp：创建 CommunicationModelAPI 实例（std::unique_ptr）
  - TearDown：释放实例
  - 作用：为同一测试套件内的测试共享一份基础 API 对象，避免重复初始化

## 测试用例简述
- DefaultConstructor
  - 目的：验证默认构造是否成功，以及默认通信场景与默认环境参数
  - 断言要点：
    - api 非空；默认场景为 NORMAL_COMMUNICATION
    - 环境参数 frequency、bandwidth、transmitPower 等与 MathConstants 默认值一致；environmentType 为 OPEN_FIELD

- ScenarioConstructor
  - 目的：验证带场景参数构造函数
  - 断言要点：
    - 以 JAMMED_COMMUNICATION、ANTI_JAM_COMMUNICATION 构造的实例，其场景与传入值一致

- SetScenario
  - 目的：验证场景设置接口
  - 断言要点：
    - setScenario 对 JAMMED/ANTI_JAM/MULTI_USER 返回 true，且 getScenario 与设置值一致

- SetFrequency
  - 目的：验证频率设置与校验
  - 断言要点：
    - 合法值（如 2400、5800）与边界值（FREQUENCY_VALIDATION_MIN/MAX）返回 true 且生效
    - 非法值（如 -100、100000）返回 false

- SetBandwidth
  - 目的：验证带宽设置与校验
  - 断言要点：
    - 合法值（20、100）与边界值（BANDWIDTH_VALIDATION_MIN/MAX）返回 true 且生效
    - 非法值（-10、10000）返回 false

- SetTransmitPower
  - 目的：验证发射功率设置与校验
  - 断言要点：
    - 合法值（30、10）与边界值（POWER_VALIDATION_MIN/MAX）返回 true 且生效
    - 非法值（-200、200）返回 false

- SetDistance
  - 目的：验证距离设置与校验
  - 断言要点：
    - 合法值（1、50）与边界值（DISTANCE_VALIDATION_MIN/MAX）返回 true 且生效
    - 非法值（-1、10000）返回 false

- SetEnvironmentType
  - 目的：验证环境类型设置
  - 断言要点：
    - 设置 URBAN_AREA、MOUNTAINOUS、OPEN_FIELD 后，getEnvironment().environmentType 与之匹配

- SetEnvironment
  - 目的：一次性设置完整环境参数并读取验证
  - 断言要点：
    - setEnvironment 返回 true；随后 getEnvironment 读取到的各字段（频率、带宽、功率、距离、环境类型、温度、湿度、气压）与设置值一致

- SetJammingEnvironment
  - 目的：设置并验证干扰环境参数
  - 断言要点：
    - setJammingEnvironment 返回 true
    - getJammingEnvironment 读取到的 isJammed、jammerType、功率/频率/带宽/距离/密度等与设置值一致

- GetUnderlyingModels
  - 目的：验证底层模型对象可访问
  - 断言要点：
    - getSignalModel、getDistanceModel、getReceiveModel、getJammerModel、getAntiJamModel 均非空

- VersionInfo
  - 目的：验证版本与信息查询接口返回有效内容
  - 断言要点：
    - CommunicationModelAPI::getVersion / ::getBuildInfo 非空
    - api->getModelInfo / api->getCapabilities 非空

## 小结
- 本文件使用 TEST_F 宏基于夹具 CommunicationModelAPIBasicTest 定义多条用例，覆盖了 API 的构造、场景切换、各类环境参数的设置与校验、干扰参数配置、底层模型可用性以及版本/能力信息查询等基础功能。
- 用例风格以“有效值/边界值/无效值”的输入组合进行断言，确保参数校验逻辑稳健。