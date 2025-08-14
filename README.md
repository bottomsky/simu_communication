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
  - **C#**: 通过P/Invoke调用C API，提供NuGet包管理（包含完整依赖链）
  - **Python**: 通过ctypes加载动态库
  - **Java**: 通过JNA调用原生库
- **🌐 Web API 集成**: 
  - **RESTful API**: 完整的数据传输对象(DTO)定义
  - **数据校验**: 智能参数验证和错误提示
  - **JSON 序列化**: 标准化的API请求/响应格式
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
- **跨平台支持**:
  - **Windows**: 生成 `.dll` 动态库
  - **Linux**: 生成 `.so` 共享库
  - **统一的C API接口**: 兼容不同平台的调用约定
- **完整的API封装**:
  - **生命周期管理**: 模型创建、销毁
  - **参数设置**: 环境参数、干扰参数配置
  - **核心计算**: 链路状态、性能指标计算
  - **干扰分析**: 干扰有效性、抗干扰能力分析
  - **性能优化**: 针对不同目标的参数优化
  - **报告生成**: 详细报告、性能报告、干扰分析报告
  - **配置管理**: 配置保存/加载、JSON导入/导出
- **内存管理**:
  - **自动内存管理**: 提供内存分配和释放函数
  - **安全的数组操作**: 防止内存泄漏
  - **错误处理**: 完善的错误代码和异常处理

## 项目结构

```
signal-transmission-model-cpp/
├── source/
│   ├── header/
│   │   ├── CommunicationModelAPI.h          # 原始C++ API
│   │   ├── CommunicationModelExport.h       # 跨平台导出宏定义
│   │   └── CommunicationModelCAPI.h         # C风格API头文件
│   └── src/
│       └── CommunicationModelCAPI.cpp       # C风格API实现
├── examples/
│   └── csharp/
│       ├── CommunicationModelWrapper/       # C# NuGet包封装
│       │   ├── CommunicationModelWrapper.cs     # C#包装类（P/Invoke 封装）
│       │   ├── CommunicationModelWrapper.csproj # NuGet包项目文件
│       │   └── CommunicationModelWrapper.targets # MSBuild目标文件（自动复制原生库）
│       ├── WebApiContracts/                 # Web API 数据传输对象
│       │   ├── CommunicationModelInitDto.cs     # 通信模型初始化DTO定义
│       │   ├── CommunicationModelInitDtoValidator.cs # 数据校验器
│       │   └── CommunicationModelInit.sample.json   # 示例JSON文件
│       ├── Program.cs                       # C#示例程序（支持 --interop-test 入口）
│       ├── Tests/
│       │   └── InteropTests.cs              # C#/C++ 互操作与内存布局一致性测试
│       ├── CommunicationModelExample.csproj # C#项目文件
│       └── CommunicationModelExample.sln    # Visual Studio解决方案文件
├── cmake/
│   └── CommunicationModelConfig.cmake       # CMake配置文件
├── CMakeLists.txt                           # 主CMake文件
├── build_windows.bat                        # Windows构建脚本
├── build_linux.sh                           # Linux构建脚本
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
- **.NET 6.0 SDK** (可选，用于C#示例)

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
   - Windows（手动）
     ```batch
     # 运行构建脚本
     build_windows.bat

     # 或手动构建
     mkdir build
     cd build
     cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
     cmake --build . --config Release --parallel
     cmake --install . --config Release
     ```
     输出文件：
     ```
     build/
     ├── bin/Release/
     │   ├── CommunicationModelShared.dll     # C++ 核心动态库
     │   └── CommunicationModelCAPI.dll       # C API动态库（依赖 CommunicationModelShared.dll）
     └── lib/Release/
         └── CommunicationModel.lib           # 静态库

     install/
     └── include/CommunicationModel/          # 头文件
     ```
   - Linux（GCC，单配置生成器）
     ```bash
     # 在项目根目录执行
     cmake -S build -B build -DCMAKE_BUILD_TYPE=Release
     cmake --build build -j 8
     ```
   - Linux（手动）
     ```bash
     # 运行构建脚本
     ./build_linux.sh

     # 或手动构建
     mkdir build
     cd build
     cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON
     cmake --build . --parallel $(nproc)
     cmake --install .
     ```
     输出文件：
      ```
      build/
      └── lib/
          ├── libCommunicationModelShared.so   # C++ 核心动态库
          ├── libCommunicationModel.a          # 静态库
          └── libCommunicationModelCAPI.so     # C API动态库（依赖 libCommunicationModelShared.so）

      install/
      └── include/CommunicationModel/          # 头文件
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

5. **.NET 与 Web API 快速开始（可选）**
   - DTO 与校验器位置：examples/csharp/WebApiContracts/
   - 反序列化示例（System.Text.Json）：
     ```csharp
     using System.Text.Json;
     using CommunicationModel.WebApi.Contracts;
     var json = File.ReadAllText("examples/csharp/WebApiContracts/CommunicationModelInit.sample.json");
     var options = new JsonSerializerOptions { PropertyNameCaseInsensitive = true };
     var dto = JsonSerializer.Deserialize<CommunicationModelInitDto>(json, options)!;
     var result = CommunicationModelInitDtoValidator.Validate(dto);
     if (!result.IsValid)
     {
         Console.WriteLine("参数校验失败:");
         foreach (var e in result.Errors) Console.WriteLine($"- {e.Message}");
         return;
     }
     Console.WriteLine("参数校验通过");
     ```

### NuGet 包与原生 DLL 说明（重要）

- C# 包装器 NuGet（examples/csharp/CommunicationModelWrapper）会在 `runtimes/win-x64/native/` 中包含并分发以下原生文件：
  - CommunicationModelCAPI.dll / .pdb
  - CommunicationModelShared.dll / .pdb
- 包内置的 MSBuild .targets（build 与 buildTransitive）会在以下阶段自动复制上述原生 DLL 到输出/发布目录：
  - 编译/构建阶段（AfterTargets: Compile;Build;ResolveAssemblyReferences）
  - 发布阶段（AfterTargets: Publish）
- 使用前置条件：打包前请先构建 C++ 产物，确保 `build/bin/$(Configuration)/` 下存在上述 DLL/PDB（Debug 或 Release）。
- 常见问题：若运行时报“找不到指定的模块”，通常是依赖 DLL 未在运行目录，或 CPU 架构不一致。此包已包含 Shared 依赖并自动拷贝，确保：
  - 你的项目为 x64（PlatformTarget = x64 / RuntimeIdentifier = win-x64）；
  - 使用对应配置的包（Debug/Release）。

## 使用示例

### 基于 DTO 的 C# 初始化与调用（示例）

```csharp
using System.Text.Json;
using CommunicationModel;
using CommunicationModel.WebApi.Contracts;

var json = File.ReadAllText("examples/csharp/WebApiContracts/CommunicationModelInit.sample.json");
var dto = JsonSerializer.Deserialize<CommunicationModelInitDto>(json)!;
var vr = CommunicationModelInitDtoValidator.Validate(dto);
if (!vr.IsValid)
{
    Console.WriteLine("DTO 参数校验未通过");
    foreach (var e in vr.Errors) Console.WriteLine($"- {e.Message}");
    return;
}

using var api = new CommunicationModelAPI();

// 映射 DTO -> C API（示例，不同项目中可抽出到 Mapper）
var env = new CommEnvironment
{
    EnvironmentType = (CommEnvironmentType)dto.Environment.EnvironmentType,
    Frequency = dto.Environment.Frequency,
    Bandwidth = dto.Environment.Bandwidth,
    Distance = dto.Environment.Distance,
    TransmitPower = dto.Environment.TransmitPower,
    NoisePower = dto.Environment.NoisePower,
    Temperature = dto.Environment.Temperature,
    Humidity = dto.Environment.Humidity,
    AtmosphericPressure = dto.Environment.AtmosphericPressure
};
api.SetEnvironment(env);

if (dto.Jamming?.IsJammed == true)
{
    var jam = new CommJammingEnvironment
    {
        IsJammed = 1,
        JammerType = (CommJammerType)dto.Jamming.JammerType,
        JammerPower = dto.Jamming.JammerPower,
        JammerFrequency = dto.Jamming.JammerFrequency,
        JammerBandwidth = dto.Jamming.JammerBandwidth,
        JammerDistance = dto.Jamming.JammerDistance,
        JammerDensity = dto.Jamming.JammerDensity,
        JammerFrequencies = IntPtr.Zero,
        JammerFrequencyCount = 0
    };
    api.SetJammingEnvironment(jam);
}

// 设置场景（抗干扰场景通过 Scenario 控制）
api.SetScenario((CommScenario)dto.Scenario);

// 示例调用：获取性能与链路状态
var perf = api.CalculatePerformance();
var status = api.CalculateLinkStatus();
Console.WriteLine($"有效距离: {perf.EffectiveRange:F2} km, SNR: {status.SignalToNoiseRatio:F2} dB, BER: {status.BitErrorRate:E2}");
```

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
std- BPSK (二进制相移键控)
- QPSK (四进制相移键控)
- 16QAM (16进制正交幅度调制)

### 外部语言调用示例

#### C# 调用示例

C# 示例位于 `examples/csharp/` 目录，使用 P/Invoke 调用 C API。

**基础使用示例：**

C# 示例位于 `examples/csharp/` 目录，包含完整的 P/Invoke 包装器和互操作性测试。

```csharp
using System;
using CommunicationModel;

// 创建模型实例
using var api = new CommunicationModelAPI();

// 获取版本信息
var version = CommunicationModelAPI.GetVersion();
Console.WriteLine($"模型版本: {version}");

// 获取构建信息
var build = CommunicationModelAPI.GetBuildInfo();
Console.WriteLine($"构建信息: {build}");
```

**干扰分析示例：**

```csharp
using var api = new CommunicationModelAPI();

// 设置干扰环境
var jammingEnv = new CommJammingEnvironment
{
    IsJammed = 1,
    JammerType = CommJammerType.GaussianNoise,
    JammerPower = 30.0,
    JammerFrequency = 2.4e9,
    JammerBandwidth = 50e6,
    JammerDistance = 500.0,
    JammerDensity = 1.0,
    JammerFrequencies = IntPtr.Zero,
    JammerFrequencyCount = 0
};

api.SetJammingEnvironment(jammingEnv);

// 计算干扰效果
double effectiveness = api.CalculateJammerEffectiveness();
Console.WriteLine($"干扰有效性: {effectiveness:P2}");

double jsRatio = api.CalculateJammerToSignalRatio();
Console.WriteLine($"干信比: {jsRatio:F2} dB");
```

**性能优化示例：**

```csharp
using var api = new CommunicationModelAPI();

// 针对距离优化
var rangeOptimizedEnv = api.OptimizeForRange(5000.0); // 5 km
Console.WriteLine($"距离优化结果 - 频率: {rangeOptimizedEnv.Frequency / 1e9:F2} GHz");

// 针对数据速率优化
var dataRateOptimizedEnv = api.OptimizeForDataRate(100e6); // 100 Mbps
Console.WriteLine($"数据速率优化结果 - 带宽: {dataRateOptimizedEnv.Bandwidth / 1e6:F2} MHz");

// 针对功率效率优化
var powerOptimizedEnv = api.OptimizeForPowerEfficiency();
Console.WriteLine($"功率效率优化结果 - 功率: {powerOptimizedEnv.TransmitPower:F2} W");
```

**报告生成示例：**

```csharp
using var api = new CommunicationModelAPI();

// 生成详细报告
var detailedReport = api.GenerateDetailedReport();
Console.WriteLine($"详细报告:\n{detailedReport}");

// 生成性能报告
var performanceReport = api.GeneratePerformanceReport();
Console.WriteLine($"性能报告:\n{performanceReport}");

// 生成干扰分析报告
var jammingReport = api.GenerateJammingAnalysisReport();
Console.WriteLine($"干扰分析报告:\n{jammingReport}");
```

**配置管理示例：**

```csharp
using var api = new CommunicationModelAPI();

try
{
    // 保存当前配置
    api.SaveConfiguration("config.json");
    Console.WriteLine("配置保存成功");

    // 加载配置
    api.LoadConfiguration("config.json");
    Console.WriteLine("配置加载成功");

    // JSON 导入导出
    string jsonConfig = api.ExportConfigurationToJSON();
    Console.WriteLine($"配置 JSON: {jsonConfig}");
    
    api.ImportConfigurationFromJSON(jsonConfig);
    Console.WriteLine("JSON 配置导入成功");
}
catch (CommunicationModelException ex)
{
    Console.WriteLine($"配置操作失败: {ex.Message}");
}
```

**互操作性测试 (Interop Testing):**

项目包含一套全面的 C#/C++ 互操作性测试，用于验证内存布局一致性、数据类型映射和 P/Invoke 调用的正确性。

- **测试文件**: `examples/csharp/Tests/InteropTests.cs`
- **运行方式**:
  1. 编译 C# 示例项目。
  2. 在项目根目录的 `build/bin/` 目录下找到 `CommunicationModelExample.exe`。
  3. 从命令行运行，并附加 `--interop-test` 参数：
     ```shell
     cd build/bin/
     ./CommunicationModelExample.exe --interop-test
     ```
- **预期输出**: 测试将执行并报告所有测试用例（如内存布局、数据交换、数组操作等）是否通过。


#### Python 调用示例

使用 ctypes 加载动态库：

```python
import ctypes
import os

# 加载库
lib_path = os.path.join('build', 'bin', 'CommunicationModelCAPI.dll')  # Windows
# lib_path = os.path.join('build', 'lib', 'libCommunicationModelCAPI.so')  # Linux
lib = ctypes.CDLL(lib_path)

# 创建模型
model = lib.CommModel_Create()
if not model:
    print("模型创建失败")

# 获取版本
version_buf = ctypes.create_string_buffer(128)
lib.CommModel_GetVersion(version_buf, 128)
print(f"版本: {version_buf.value.decode()}")

# 销毁模型
lib.CommModel_Destroy(model)
```

#### Java 调用示例

使用 JNA (Java Native Access) 调用：

```java
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.PointerByReference;

public interface CommModelLib extends Library {
    CommModelLib INSTANCE = Native.load("CommunicationModelCAPI", CommModelLib.class);
    
    Pointer CommModel_Create();
    void CommModel_Destroy(Pointer model);
    int CommModel_GetVersion(byte[] version, int bufferSize);
}

// 使用
Pointer model = CommModelLib.INSTANCE.CommModel_Create();
if (model == null) {
    System.out.println("模型创建失败");
    return;
}

byte[] versionBuf = new byte[128];
CommModelLib.INSTANCE.CommModel_GetVersion(versionBuf, 128);
String version = new String(versionBuf).trim();
System.out.println("版本: " + version);

CommModelLib.INSTANCE.CommModel_Destroy(model);
```

## 错误处理

### 错误代码

- 0: 成功
- -1: 无效参数
- -2: 内存分配失败
- -3: 文件操作失败
- -4: 计算错误
- -5: 配置无效

### C# 异常处理

在 C# 包装类中，所有函数都会检查返回值，如果不为0则抛出异常：

```csharp
if (rc != 0) {
    throw new CommunicationModelException($"操作失败，错误码: {rc}");
}
```

## 性能考虑

- **内存管理**: 使用提供的 Alloc/Free 函数管理内存，避免泄漏
- **线程安全**: API 支持多线程，但需确保单个模型实例的线程安全
- **性能优化**: 对于批量计算，使用批量接口减少开销；启用并行计算选项

## 故障排除

### 常见问题

- **库加载失败**: 检查库路径、依赖库（MSVCRuntime等）。若提示“找不到指定的模块”：
  - 确认 CommunicationModelCAPI 与 CommunicationModelShared DLL 是否同目录；
  - 使用 dumpbin /dependents 或 Dependencies 工具检查依赖链；
  - 确认工程为 x64，并与 DLL 架构一致；
  - 使用本仓库提供的 NuGet 包（已包含 Shared 依赖与自动拷贝 .targets）可避免此问题。
- **符号未导出**: 确保使用 Release 构建，检查导出宏
- **内存泄漏**: 始终匹配 Alloc/Free 调用
- **跨平台问题**: 注意路径分隔符、编码格式

### 调试技巧

- 启用详细日志：CommModel_SetLogLevel(model, LOG_DEBUG)
- 检查返回值：所有函数返回错误码
- 使用 Valgrind/Memory Profiler 检查内存问题
- 验证输入参数范围

## 版本兼容性

- 当前版本: v1.5.0
- 向后兼容: 是（API 保持稳定）
- 支持平台: Windows 10+, Ubuntu 20.04+

## 许可证

MIT License - 详见 LICENSE 文件

## 技术支持

- GitHub Issues
- 文档: DYNAMIC_LIBRARY_GUIDE.md (已合并到 README)
- 联系: support@example.com

---

**注意**: 本项目为仿真研究用途，请遵循相关法律法规使用。