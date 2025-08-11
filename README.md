# 通信对抗模型仿真系统

## 项目简介

本项目是一个基于C++开发的通信对抗模型仿真系统，旨在构建完整的通信系统对抗仿真平台。系统通过四个核心仿真模型，全面模拟通信系统在复杂电磁环境下的性能表现，为通信系统设计、优化和对抗策略研究提供强有力的仿真支持。

## 核心功能

### 🎯 四大仿真模型

1. **通信距离模型** - Communication Distance Model
   - 计算不同环境条件下的有效通信距离
   - 考虑发射功率、接收灵敏度、环境衰减等因素
   - 支持多种地形和传播环境建模

2. **通信接收模型** - Communication Receive Model
   - 模拟接收端信号处理过程
   - 计算信噪比、误码率等关键性能指标
   - 可调整检测门限设置
   - 智能信号解码判断 (支持无参和有参两种方式)
   - 环境温度动态调整 (默认290K)
   - 支持多种调制解调方式 (BPSK/QPSK/16QAM/FM/AM)
   - 完整的性能指标输出

3. **通信干扰模型** - Communication Jammer Model
   - 仿真各种干扰类型对通信系统的影响
   - 支持高斯白噪声、窄带干扰、扫频干扰等
   - 评估干扰效果和系统抗干扰能力

4. **通信抗干扰模型** - Communication Anti-Jam Model
   - 实现多种抗干扰技术仿真
   - 支持跳频、扩频、自适应滤波等技术
   - 评估抗干扰策略的有效性

## 技术特性

- **🔧 开发语言**: C++17标准
- **🏗️ 构建系统**: CMake 3.16+ 跨平台构建，支持Visual Studio 2022和GCC
- **📦 库架构**: 提供三种库形式
  - **CommunicationModelShared**: C++核心动态库
  - **CommunicationModelStatic**: C++静态库
  - **CommunicationModelCAPI**: C风格API动态库（支持多语言调用）
- **🌐 跨平台**: 支持Windows (VS2022) 和Linux (GCC)
- **🔗 多语言支持**: 
  - **C++**: 直接使用核心库
  - **C#**: 通过P/Invoke调用C API
  - **Python**: 通过ctypes加载动态库
  - **Java**: 通过JNA调用原生库
- **📊 数据格式**: JSON格式的配置和测试数据
- **🧪 测试体系**: 完整的单元测试、集成测试和验证数据
- **🔍 测试框架**: 集成Google Test框架
- **📈 代码质量**: 严格的编译警告和代码规范
- **🎯 模块化设计**: 清晰的模块分离和接口设计
- **🔧 智能检测**: 支持可调整检测门限和智能信号解码判断
- **⚙️ 参数优化**: 提供默认参数配置，支持快速开始和精细调优
- **📊 性能分析**: 全面的性能指标计算，包括SNR、BER、噪声功率等
- **🔄 向后兼容**: 新功能保持与现有代码的完全兼容性
- **📦 安装支持**: 标准化安装规则，支持CMake目标导出

## 项目结构

```
signal-transmission-model-cpp/
├── build/                          # 构建目录（版本库仅保留 CMakeLists.txt）
│   ├── CMakeLists.txt              # 主CMake配置文件（位于 build/）
│   ├── bin/                        # 可执行文件输出（被 .gitignore 忽略）
│   ├── lib/                        # 库文件输出（被 .gitignore 忽略）
│   └── [构建生成文件]               # 编译中间文件（被 .gitignore 忽略）
├── docs/                           # 技术文档
│   ├── data/                       # 测试数据
│   ├── 基础通信能力模型参数设计.md
│   ├── 通信距离模型.md
│   ├── 通信接收模型.md
│   ├── 通信干扰模型.md
│   └── 通信抗干扰模型.md
├── examples/                       # 示例代码
│   ├── CMakeLists.txt              # 示例构建配置
│   ├── basic_usage_example.cpp     # 基础使用示例
│   ├── environment_config_example.cpp  # 环境配置示例
│   └── simple_environment_config_example.cpp
├── scripts/                        # 构建和部署脚本
├── source/                         # 源代码
│   ├── header/                     # 头文件
│   │   ├── CommunicationModelAPI.h
│   │   ├── MathConstants.h
│   │   ├── [其他模型头文件]
│   └── src/                        # 源文件
│       ├── CommunicationModelAPI.cpp
│       ├── [各模型实现文件]
├── tests/                          # 测试代码
│   ├── CMakeLists.txt              # 测试构建配置
│   ├── unit/                       # 单元测试
│   ├── integration/                # 集成测试
│   ├── test_constants.cpp          # 常量测试
│   └── [其他测试文件]
├── .gitignore                      # Git忽略规则
├── README.md                       # 项目说明文档
├── 开发约束文档.md                  # 开发规范文档
├── 开发计划.md                     # 开发计划文档
└── 测试总结报告.md                  # 测试总结报告
```

## 快速开始

### 环境要求

- **编译器**: GCC 7.0+ / MSVC 2017+ / Clang 6.0+ (支持C++17)
- **构建工具**: CMake 3.16+
- **测试框架**: Google Test (自动下载)
- **操作系统**: Windows 10+ / Linux (Ubuntu 18.04+)
- **Python**: 3.6+ (可选，用于脚本工具)

### 编译构建

1. **克隆项目**
   ```bash
   git clone <repository-url>
   cd signal-transmission-model-cpp
   ```

2. **构建与编译（推荐脚本）**
   - Windows（VS2022，多配置生成器）
     ```powershell
     # 在项目根目录执行
     .\build_from_build_cmakelists.ps1 -Config Debug -Jobs 8 -Clean
     .\build_from_build_cmakelists.ps1 -Config Release -Jobs 8
     ```
     说明：
     - `-Config` 指定构建类型（Debug/Release/RelWithDebInfo/MinSizeRel）。
     - `-Jobs` 指定并行编译的任务数（传给 cmake --build 的 -j）。
     - `-Clean` 可选，先清理 build 目录后再重新生成。

   - Linux（GCC，单配置生成器）
     ```bash
     # 在项目根目录执行
     cmake -S build -B build -DCMAKE_BUILD_TYPE=Release
     cmake --build build -j 8
     ```

3. **手动使用CMake（可选）**
   - 多配置生成器（如 VS2022）：忽略 CMAKE_BUILD_TYPE，使用 --config 控制
     ```powershell
     cmake -S build -B build
     cmake --build build --config Release -j 8
     ```
   - 单配置生成器（如 Ninja/Unix Makefiles）：使用 CMAKE_BUILD_TYPE 控制
     ```bash
     cmake -S build -B build -DCMAKE_BUILD_TYPE=Debug
     cmake --build build -j 8
     ```

4. **运行测试**
   ```bash
   # 运行所有测试（如已集成）
   ctest -C Release --output-on-failure
   ```

## 使用示例

### C++ API 使用

#### 1. 通信距离模型 (CommunicationDistanceModel)

通信距离模型用于计算不同环境下的有效通信距离和路径损耗。

```cpp
#include "CommunicationDistanceModel.h"

// 创建通信距离模型
CommunicationDistanceModel distanceModel(
    10.0,                           // 最大视距距离 10km
    EnvironmentType::OPEN_FIELD,    // 开阔地环境
    1.0,                           // 环境衰减系数
    -100.0,                        // 接收灵敏度 -100dBm
    10.0,                          // 链路余量 10dB
    20.0                           // 发射功率 20dBm
);

// 设置参数
distanceModel.setMaxLineOfSight(15.0);
distanceModel.setTransmitPower(30.0);
distanceModel.setEnvironmentType(EnvironmentType::URBAN);

// 计算有效通信距离
double effectiveDistance = distanceModel.calculateEffectiveDistance();
std::cout << "有效通信距离: " << effectiveDistance << " km" << std::endl;

// 计算路径损耗
double pathLoss = distanceModel.calculatePathLoss(5.0, 100.0); // 5km距离，100MHz频率
std::cout << "路径损耗: " << pathLoss << " dB" << std::endl;

// 快速距离计算
double quickRange = distanceModel.quickCalculateRange(150.0); // 150MHz频率
std::cout << "快速计算距离: " << quickRange << " km" << std::endl;
```

#### 2. 通信接收模型 (CommunicationReceiveModel)

通信接收模型用于分析接收机性能，包括信噪比、误码率等关键指标。支持可调整检测门限和智能信号解码判断。

```cpp
#include "CommunicationReceiveModel.h"

// 创建接收模型 - 使用默认参数
CommunicationReceiveModel receiveModel;

// 或者指定完整参数
CommunicationReceiveModel receiveModel2(
    -100.0,                              // 接收灵敏度 -100dBm
    3.0,                                // 噪声系数 3dB
    25.0,                               // 系统带宽 25kHz
    ReceiveModulationType::QPSK,        // QPSK调制
    ReceiverType::SUPERHETERODYNE,      // 超外差接收机
    290.0,                              // 环境温度 290K (默认值)
    2.0                                 // 天线增益 2dBi
);

// 设置接收信号功率
receiveModel.setReceivedPower(-85.0);   // 接收功率 -85dBm

// 基础性能计算
double snr = receiveModel.calculateSignalToNoiseRatio();
double ber = receiveModel.calculateBitErrorRate();
double noisePower = receiveModel.calculateEffectiveNoisePower();
double minDetectable = receiveModel.calculateMinimumDetectablePower();

std::cout << "信噪比: " << snr << " dB" << std::endl;
std::cout << "误码率: " << ber << std::endl;
std::cout << "有效噪声功率: " << noisePower << " dBm" << std::endl;
std::cout << "最小可检测功率: " << minDetectable << " dBm" << std::endl;

// 检测门限设置 (新功能)
receiveModel.setDetectionThreshold(15.0);  // 设置检测门限为15dB
double threshold = receiveModel.getDetectionThreshold();
std::cout << "当前检测门限: " << threshold << " dB" << std::endl;

// 环境温度动态调整
receiveModel.setAmbientTemperature(310.0); // 设置环境温度为310K
double temp = receiveModel.getAmbientTemperature();
std::cout << "当前环境温度: " << temp << " K" << std::endl;

// 信号检测和解码判断 (增强功能)
bool detectable = receiveModel.isSignalDetectable();
std::cout << "信号可检测: " << (detectable ? "是" : "否") << std::endl;

// 信号解码判断 - 支持两种方式
// 方式1: 智能判断 (根据调制方式自动选择1e-6误码率对应的SNR要求)
bool decodableAuto = receiveModel.isSignalDecodable();
std::cout << "信号可解码(智能): " << (decodableAuto ? "是" : "否") << std::endl;

// 方式2: 手动指定SNR要求
bool decodableManual = receiveModel.isSignalDecodable(12.0); // 要求12dB SNR
std::cout << "信号可解码(12dB): " << (decodableManual ? "是" : "否") << std::endl;

// 获取调制方式对应的理论SNR要求
double requiredSNR = receiveModel.getRequiredSNRForBER(1e-6); // 1e-6误码率
std::cout << "1e-6误码率所需SNR: " << requiredSNR << " dB" << std::endl;

// 计算接收余量和检测余量
double receiveMargin = receiveModel.calculateReceiveMargin();
std::cout << "接收余量: " << receiveMargin << " dB" << std::endl;

// 获取完整参数和性能信息
std::cout << "\n=== 参数信息 ===" << std::endl;
std::cout << receiveModel.getParameterInfo() << std::endl;

std::cout << "\n=== 性能信息 ===" << std::endl;
std::cout << receiveModel.getPerformanceInfo() << std::endl;

// 不同调制方式的性能比较
std::vector<ReceiveModulationType> modTypes = {
    ReceiveModulationType::BPSK,
    ReceiveModulationType::QPSK,
    ReceiveModulationType::QAM16,
    ReceiveModulationType::FM,
    ReceiveModulationType::AM
};

std::cout << "\n=== 调制方式性能比较 ===" << std::endl;
for (auto modType : modTypes) {
    receiveModel.setModulationType(modType);
    double snrReq = receiveModel.getRequiredSNRForBER(1e-6);
    bool canDecode = receiveModel.isSignalDecodable();
    
    std::string modName;
    switch(modType) {
        case ReceiveModulationType::BPSK: modName = "BPSK"; break;
        case ReceiveModulationType::QPSK: modName = "QPSK"; break;
        case ReceiveModulationType::QAM16: modName = "16QAM"; break;
        case ReceiveModulationType::FM: modName = "FM"; break;
        case ReceiveModulationType::AM: modName = "AM"; break;
    }
    
    std::cout << modName << ": SNR要求=" << snrReq << "dB, 可解码=" 
              << (canDecode ? "是" : "否") << std::endl;
}
```

#### 3. 通信干扰模型 (CommunicationJammerModel)

通信干扰模型用于仿真各种干扰类型对通信系统的影响。

```cpp
#include "CommunicationJammerModel.h"

// 创建干扰模型
CommunicationJammerModel jammerModel(
    JammerType::GAUSSIAN_NOISE,     // 高斯白噪声干扰
    JammerStrategy::CONTINUOUS,    // 连续干扰策略
    30.0,                          // 干扰功率 30dBm
    10000.0,                       // 干扰频率 10MHz
    100.0,                         // 干扰带宽 100kHz
    50.0                           // 干扰作用距离 50km
);

// 设置目标信号参数
jammerModel.setTargetFrequency(10050.0);    // 目标频率 10.05MHz
jammerModel.setTargetBandwidth(25.0);       // 目标带宽 25kHz
jammerModel.setTargetPower(20.0);           // 目标功率 20dBm
jammerModel.setTargetDistance(30.0);        // 目标距离 30km

// 计算干信比
double jsRatio = jammerModel.calculateJammerToSignalRatio();
std::cout << "干信比: " << jsRatio << " dB" << std::endl;

// 计算干扰有效性
double effectiveness = jammerModel.calculateJammerEffectiveness();
std::cout << "干扰有效性: " << effectiveness << std::endl;

// 评估干扰效果等级
JammerEffectLevel effectLevel = jammerModel.evaluateJammerEffect();
std::cout << "干扰效果等级: " << static_cast<int>(effectLevel) << std::endl;

// 计算干扰覆盖范围
double jammingRange = jammerModel.calculateJammingRange();
double jammingArea = jammerModel.calculateJammingArea();
std::cout << "干扰覆盖范围: " << jammingRange << " km" << std::endl;
std::cout << "干扰覆盖面积: " << jammingArea << " m²" << std::endl;

// 脉冲干扰特殊设置
jammerModel.setJammerType(JammerType::PULSE);
jammerModel.setPulseWidth(1.0);              // 脉冲宽度 1ms
jammerModel.setPulseRepetitionRate(1000.0);  // 重复频率 1kHz
jammerModel.setDutyCycle(0.1);               // 占空比 10%
```

#### 4. 通信抗干扰模型 (CommunicationAntiJamModel)

通信抗干扰模型用于评估和优化通信系统的抗干扰能力。

```cpp
#include "CommunicationAntiJamModel.h"

// 创建抗干扰模型
CommunicationAntiJamModel antiJamModel(
    AntiJamTechnique::FREQUENCY_HOPPING,  // 跳频技术
    AntiJamStrategy::ADAPTIVE,            // 自适应策略
    20.0,                                // 处理增益 20dB
    1000.0,                              // 扩频因子 1000
    1000.0,                              // 跳频速率 1kHz
    3.0                                  // 编码增益 3dB
);

// 设置系统参数
antiJamModel.setSystemBandwidth(10.0);       // 系统带宽 10MHz
antiJamModel.setSignalPower(20.0);           // 信号功率 20dBm
antiJamModel.setNoisePower(-110.0);          // 噪声功率 -110dBm
antiJamModel.setInterferenceLevel(-80.0);    // 干扰电平 -80dBm

// 设置跳频参数
antiJamModel.setHoppingChannels(100);        // 跳频信道数 100
antiJamModel.setChannelSpacing(0.1);         // 信道间隔 0.1MHz
antiJamModel.setDwellTime(1.0);              // 驻留时间 1ms

// 计算抗干扰增益
double antiJamGain = antiJamModel.calculateAntiJamGain();
std::cout << "抗干扰增益: " << antiJamGain << " dB" << std::endl;

// 计算抗干扰能力
double resistance = antiJamModel.calculateJammerResistance();
std::cout << "抗干扰能力: " << resistance << std::endl;

// 计算信干比
double sjr = antiJamModel.calculateSignalToJammerRatio();
std::cout << "信干比: " << sjr << " dB" << std::endl;

// 计算有干扰时的误码率
double berWithJam = antiJamModel.calculateBitErrorRateWithJamming();
std::cout << "有干扰时误码率: " << berWithJam << std::endl;

// 评估抗干扰效果
AntiJamEffectLevel protectionLevel = antiJamModel.evaluateAntiJamEffect();
std::cout << "抗干扰保护等级: " << static_cast<int>(protectionLevel) << std::endl;

// 计算最优抗干扰技术
AntiJamTechnique optimalTech = antiJamModel.calculateOptimalTechnique();
double optimalGain = antiJamModel.calculateOptimalProcessingGain();
std::cout << "最优处理增益: " << optimalGain << " dB" << std::endl;

// 预测干扰下的性能
double performance = antiJamModel.predictPerformanceUnderJamming(25.0, 200.0);
std::cout << "干扰下性能预测: " << performance << std::endl;
```

#### 5. 综合应用示例

```cpp
#include "CommunicationModelAPI.h"

// 使用统一API进行综合分析
CommunicationModelAPI api;

// 设置通信环境
CommunicationEnvironment env;
env.frequency = 100.0;           // 100MHz
env.transmitPower = 30.0;        // 30dBm
env.bandwidth = 25.0;            // 25kHz
env.environmentType = EnvironmentType::SUBURBAN;

api.setEnvironment(env);

// 设置干扰环境
JammingEnvironment jamEnv;
jamEnv.isJammed = true;
jamEnv.jammerType = JammerType::NARROWBAND;
jamEnv.jammerPower = 25.0;       // 25dBm
jamEnv.jammerDistance = 20.0;    // 20km

api.setJammingEnvironment(jamEnv);

// 计算通信链路状态
CommunicationLinkStatus linkStatus = api.calculateLinkStatus();
std::cout << "链路质量: " << linkStatus.linkQuality << std::endl;
std::cout << "通信可用性: " << linkStatus.availability << std::endl;

// 计算通信性能
CommunicationPerformance performance = api.calculatePerformance();
std::cout << "数据传输速率: " << performance.dataRate << " bps" << std::endl;
std::cout << "误码率: " << performance.bitErrorRate << std::endl;
std::cout << "延迟: " << performance.latency << " ms" << std::endl;
```

### 外部语言调用

- Python（ctypes）
  ```python
  import ctypes, os, sys
  base = os.path.join('build', 'bin') if os.name == 'nt' else os.path.join('build', 'lib')
  libname = 'CommunicationModelCAPI.dll' if os.name == 'nt' else 'libCommunicationModelCAPI.so'
  lib = ctypes.CDLL(os.path.join(base, libname))
  
  # 示例：获取版本信息
  buf = ctypes.create_string_buffer(128)
  rc = lib.CommModel_GetVersion(buf, len(buf))
  if rc == 0:
      print('Version:', buf.value.decode())
  else:
      print('GetVersion failed, rc=', rc)
  ```

- C#（P/Invoke）
  - 参见 examples/csharp，示例工程以 DllImport("CommunicationModelCAPI") 方式引用
  - 运行前确保 CommunicationModelCAPI.dll 位于输出目录，可从 build/bin 或 build/lib 复制

- C++（链接核心库）
  - 引用目标 CommunicationModel::Core 或 CommunicationModelShared
  - 头文件从 include/CommunicationModel（安装后）或 source/header（源码构建）获取

## 测试体系

### 测试分类

1. **单元测试** (Unit Tests)
   - 各模型组件的独立功能测试
   - 参数配置类的验证测试
   - API接口的基础功能测试

2. **集成测试** (Integration Tests)
   - 多模块协同工作测试
   - 端到端功能验证
   - 性能基准测试

3. **常量测试** (Constants Tests)
   - 数学常量精度验证
   - 物理参数一致性检查
   - 配置参数有效性测试

### 测试命令

```bash
# 运行所有测试
ctest -C Release

# 运行特定类型的测试
cmake --build . --target run_unit_tests      # 单元测试
cmake --build . --target run_integration_tests  # 集成测试
cmake --build . --target run_config_tests    # 配置测试

# 运行单个测试
./bin/Release/test_constants.exe
./bin/Release/test_communication_distance.exe
```

### 测试覆盖率

当前测试覆盖情况：
- **单元测试**: 11个测试用例
- **集成测试**: 4个测试用例  
- **总体通过率**: 93% (14/15 测试通过)
- **代码覆盖率**: > 80%

## 技术参数

### 支持的频率范围
- **短波**: 1.5MHz - 30MHz
- **超短波**: 30MHz - 300MHz  
- **微波**: 300MHz - 30GHz

### 支持的调制方式
- AM (调幅)
- FM (调频)
- BPSK (二进制相移键控)
- QPSK (四进制相移键控)
- 16QAM (16进制正交幅度调制)

### 干扰类型
- 高斯白噪声干扰
- 窄带干扰
- 扫频干扰
- 脉冲干扰

### 抗干扰技术
- FHSS (跳频扩频)
- DSSS (直接序列扩频)
- 自适应滤波
- 功率控制

## 性能指标

- **计算精度**: 6位有效数字
- **单次计算时间**: < 100ms
- **内存使用**: < 100MB
- **并发支持**: 多线程安全
- **测试覆盖率**: > 80%

## 文档资源

- [开发约束文档](./开发约束文档.md) - 项目开发规范和约束
- [开发计划文档](./开发计划.md) - 项目开发计划和里程碑
- [测试总结报告](./测试总结报告.md) - 测试结果和质量报告
- [通信距离模型文档](./docs/通信距离模型.md) - 距离计算模型详解
- [通信接收模型文档](./docs/通信接收模型.md) - 接收模型详解
- [通信干扰模型文档](./docs/通信干扰模型.md) - 干扰仿真模型详解
- [通信抗干扰模型文档](./docs/通信抗干扰模型.md) - 抗干扰模型详解
- [基础通信能力模型参数设计](./docs/基础通信能力模型参数设计.md) - 参数设计文档

## 贡献指南

1. Fork 本项目
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m '[模块名] 添加新特性'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 创建 Pull Request

## 版本历史

- **v1.0.0** - 初始版本，包含四大核心模型
- **v1.1.0** - 添加跨平台API支持
- **v1.2.0** - 完善构建系统和测试框架
- **v1.3.0** - 重构项目结构，优化构建流程
  - 将CMakeLists.txt移动到build目录
  - 整理测试文件到tests目录
  - 添加常量验证测试
  - 完善示例代码和文档
- **v1.4.0** - 通信接收模型功能增强
  - 添加可调整检测门限功能
  - 实现智能信号解码判断 (支持无参和有参重载)
  - 优化环境温度默认参数设置 (默认290K)
  - 增强性能分析和参数信息输出
  - 提供多种调制方式性能比较功能
  - 保持完全向后兼容性
- **v1.5.0** - 跨语言与安装支持
  - 新增 C API 动态库目标 CommunicationModelCAPI，支持 Python/C#/Java 调用
  - Windows 下为核心动态库开启自动导出符号（WINDOWS_EXPORT_ALL_SYMBOLS）
  - 增加 CMake 安装规则与目标导出（install(EXPORT ...)），便于外部项目复用
  - 提供 CommunicationModel::Core 与 CommunicationModel::CAPI 别名
  - 新增和优化构建脚本 build_from_build_cmakelists.ps1（支持 -Config/-Jobs/-Clean、彩色日志、错误码检查）
  - 规范 .gitignore：忽略 build 目录下所有内容但保留 build/CMakeLists.txt

## 许可证

本项目采用 [MIT License](LICENSE) 许可证。

## 联系方式

- **项目维护**: 通信对抗仿真团队
- **技术支持**: [技术支持邮箱]
- **问题反馈**: [GitHub Issues](issues)

---

**注意**: 本项目为仿真研究用途，请遵循相关法律法规使用。