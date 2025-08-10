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
   - 支持多种调制解调方式

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
- **🏗️ 构建系统**: CMake 3.16+ 跨平台构建
- **📊 数据格式**: JSON格式的配置和测试数据
- **🌐 跨平台**: 支持Windows和Linux平台
- **📚 API接口**: 提供C风格API，支持外部语言调用
- **🧪 测试驱动**: 完整的单元测试、集成测试和验证数据
- **🔍 测试框架**: 集成Google Test框架
- **📈 代码质量**: 严格的编译警告和代码规范
- **🎯 模块化设计**: 清晰的模块分离和接口设计

## 项目结构

```
signal-transmission-model-cpp/
├── build/                          # 构建目录
│   ├── CMakeLists.txt              # 主CMake配置文件
│   ├── bin/                        # 可执行文件输出
│   ├── lib/                        # 库文件输出
│   └── [构建生成文件]               # 编译生成的中间文件
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

2. **进入构建目录**
   ```bash
   cd build
   ```

3. **配置项目**
   ```bash
   # 配置项目（CMakeLists.txt 位于 build 目录）
   cmake -S . -B . -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON
   ```

4. **编译项目**
   ```bash
   # Linux/macOS
   cmake --build . --config Release
   
   # Windows (Visual Studio)
   cmake --build . --config Release
   ```

5. **运行测试**
   ```bash
   # 运行所有测试
   ctest -C Release --output-on-failure
   
   # 或运行特定测试
   ./bin/Release/test_constants.exe
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

通信接收模型用于分析接收机性能，包括信噪比、误码率等关键指标。

```cpp
#include "CommunicationReceiveModel.h"

// 创建接收模型
CommunicationReceiveModel receiveModel(
    -100.0,                              // 接收灵敏度 -100dBm
    3.0,                                // 噪声系数 3dB
    25.0,                               // 系统带宽 25kHz
    ReceiveModulationType::QPSK,        // QPSK调制
    ReceiverType::SUPERHETERODYNE,      // 超外差接收机
    290.0,                              // 环境温度 290K
    2.0                                 // 天线增益 2dBi
);

// 设置接收信号功率
receiveModel.setReceivedPower(-85.0);   // 接收功率 -85dBm

// 计算信噪比
double snr = receiveModel.calculateSignalToNoiseRatio();
std::cout << "信噪比: " << snr << " dB" << std::endl;

// 计算误码率
double ber = receiveModel.calculateBitErrorRate();
std::cout << "误码率: " << ber << std::endl;

// 判断信号是否可检测和解码
bool detectable = receiveModel.isSignalDetectable();
bool decodable = receiveModel.isSignalDecodable(12.0); // 要求12dB SNR
std::cout << "信号可检测: " << (detectable ? "是" : "否") << std::endl;
std::cout << "信号可解码: " << (decodable ? "是" : "否") << std::endl;

// 计算接收余量
double margin = receiveModel.calculateReceiveMargin();
std::cout << "接收余量: " << margin << " dB" << std::endl;
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

### 外部语言调用 (Python示例)

```python
import ctypes

# 加载动态库
lib = ctypes.CDLL('./lib/CommunicationModel.dll')  # Windows
# lib = ctypes.CDLL('./lib/libCommunicationModel.so')  # Linux

# 调用C API
result = lib.calculate_communication_distance(
    ctypes.c_double(10.0),  # maxLineOfSight
    ctypes.c_double(20.0),  # transmitPower
    ctypes.c_int(1)         # environmentType
)
print(f"通信距离: {result} km")
```

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

## 许可证

本项目采用 [MIT License](LICENSE) 许可证。

## 联系方式

- **项目维护**: 通信对抗仿真团队
- **技术支持**: [技术支持邮箱]
- **问题反馈**: [GitHub Issues](issues)

---

**注意**: 本项目为仿真研究用途，请遵循相关法律法规使用。