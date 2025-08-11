# 通信模型动态库使用指南

本指南介绍如何将通信模型封装为动态库，并在C#等其他语言中调用。

## 目录结构

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
│       ├── CommunicationModelWrapper.cs     # C#包装类
│       ├── Program.cs                       # C#示例程序
│       └── CommunicationModelExample.csproj # C#项目文件
├── cmake/
│   └── CommunicationModelConfig.cmake       # CMake配置文件
├── CMakeLists.txt                           # 主CMake文件
├── build_windows.bat                        # Windows构建脚本
├── build_linux.sh                          # Linux构建脚本
└── DYNAMIC_LIBRARY_GUIDE.md               # 本文档
```

## 功能特性

### 跨平台支持
- **Windows**: 生成 `.dll` 动态库
- **Linux**: 生成 `.so` 共享库
- **统一的C API接口**: 兼容不同平台的调用约定

### 完整的API封装
- **生命周期管理**: 模型创建、销毁
- **参数设置**: 环境参数、干扰参数配置
- **核心计算**: 链路状态、性能指标计算
- **干扰分析**: 干扰有效性、抗干扰能力分析
- **性能优化**: 针对不同目标的参数优化
- **报告生成**: 详细报告、性能报告、干扰分析报告
- **配置管理**: 配置保存/加载、JSON导入/导出

### 内存管理
- **自动内存管理**: 提供内存分配和释放函数
- **安全的数组操作**: 防止内存泄漏
- **错误处理**: 完善的错误代码和异常处理

## 构建说明

### Windows构建

#### 前置要求
- Visual Studio 2019/2022 (包含C++工具)
- CMake 3.16+
- .NET 6.0 SDK (可选，用于C#示例)

#### 构建步骤
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

#### 输出文件
```
build/
├── bin/Release/
│   ├── CommunicationModel.dll           # C++ API动态库
│   └── CommunicationModelCAPI.dll       # C API动态库
└── lib/Release/
    └── CommunicationModel.lib           # 静态库

install/
└── include/CommunicationModel/          # 头文件
```

### Linux构建

#### 前置要求
- GCC 7+ 或 Clang 6+
- CMake 3.16+
- .NET 6.0 SDK (可选，用于C#示例)

#### 构建步骤
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

#### 输出文件
```
build/
└── lib/
    ├── libCommunicationModel.so         # C++ API动态库
    ├── libCommunicationModel.a          # 静态库
    └── libCommunicationModelCAPI.so     # C API动态库

install/
└── include/CommunicationModel/          # 头文件
```

## C#调用示例

### 1. 基础使用

```csharp
using CommunicationModel;

// 创建模型实例
using var commModel = new CommunicationModelAPI(CommScenario.NormalCommunication);

// 设置通信环境
var environment = new CommEnvironment
{
    Frequency = 2.4e9,           // 2.4 GHz
    Bandwidth = 20e6,            // 20 MHz
    TransmitPower = 1.0,         // 1 W
    Distance = 1000.0,           // 1 km
    EnvironmentType = CommEnvironmentType.Urban
};
commModel.SetEnvironment(environment);

// 计算链路状态
var linkStatus = commModel.CalculateLinkStatus();
Console.WriteLine($"信噪比: {linkStatus.SignalToNoiseRatio:F2} dB");
Console.WriteLine($"误码率: {linkStatus.BitErrorRate:E2}");
```

### 2. 干扰分析

```csharp
// 切换到干扰场景
commModel.SetScenario(CommScenario.JammedCommunication);

// 设置干扰环境
var jammingEnv = new CommJammingEnvironment
{
    IsJammed = 1,
    JammerType = CommJammerType.Wideband,
    JammerPower = 10.0,          // 10 W
    JammerDistance = 500.0       // 500 m
};
commModel.SetJammingEnvironment(jammingEnv);

// 分析干扰效果
var jammerEffectiveness = commModel.CalculateJammerEffectiveness();
var jsRatio = commModel.CalculateJammerToSignalRatio();
Console.WriteLine($"干扰有效性: {jammerEffectiveness * 100:F2}%");
Console.WriteLine($"干信比: {jsRatio:F2} dB");
```

### 3. 性能优化

```csharp
// 针对5km距离优化
var optimizedEnv = commModel.OptimizeForRange(5000.0);
Console.WriteLine($"优化频率: {optimizedEnv.Frequency / 1e9:F2} GHz");
Console.WriteLine($"优化功率: {optimizedEnv.TransmitPower:F2} W");

// 针对100Mbps数据速率优化
var dataRateOptimized = commModel.OptimizeForDataRate(100e6);
Console.WriteLine($"优化带宽: {dataRateOptimized.Bandwidth / 1e6:F2} MHz");
```

### 4. 报告生成

```csharp
// 生成详细报告
var detailedReport = commModel.GenerateDetailedReport();
Console.WriteLine(detailedReport);

// 生成性能报告
var performanceReport = commModel.GeneratePerformanceReport();
Console.WriteLine(performanceReport);

// 生成干扰分析报告
var jammingReport = commModel.GenerateJammingAnalysisReport();
Console.WriteLine(jammingReport);
```

### 5. 配置管理

```csharp
// 导出配置为JSON
var jsonConfig = commModel.ExportConfigurationToJSON();
Console.WriteLine(jsonConfig);

// 保存配置到文件
commModel.SaveConfiguration("config.json");

// 从文件加载配置
commModel.LoadConfiguration("config.json");

// 从JSON导入配置
commModel.ImportConfigurationFromJSON(jsonConfig);
```

## 其他语言调用

### Python调用示例

```python
import ctypes
from ctypes import Structure, c_double, c_int, c_char_p, POINTER

# 加载动态库
if platform.system() == "Windows":
    lib = ctypes.CDLL("./CommunicationModelCAPI.dll")
else:
    lib = ctypes.CDLL("./libCommunicationModelCAPI.so")

# 定义结构体
class CommEnvironment(Structure):
    _fields_ = [
        ("frequency", c_double),
        ("bandwidth", c_double),
        ("transmitPower", c_double),
        ("noisePower", c_double),
        ("distance", c_double),
        ("environmentType", c_int),
        ("temperature", c_double),
        ("humidity", c_double),
        ("atmosphericPressure", c_double)
    ]

# 定义函数原型
lib.CommModel_Create.restype = ctypes.c_void_p
lib.CommModel_SetEnvironment.argtypes = [ctypes.c_void_p, POINTER(CommEnvironment)]
lib.CommModel_CalculateCommunicationRange.argtypes = [ctypes.c_void_p, POINTER(c_double)]

# 使用示例
handle = lib.CommModel_Create()
env = CommEnvironment()
env.frequency = 2.4e9
env.bandwidth = 20e6
env.transmitPower = 1.0
env.distance = 1000.0

lib.CommModel_SetEnvironment(handle, ctypes.byref(env))

range_result = c_double()
lib.CommModel_CalculateCommunicationRange(handle, ctypes.byref(range_result))
print(f"通信距离: {range_result.value / 1000:.2f} km")

lib.CommModel_Destroy(handle)
```

### Java调用示例 (JNA)

```java
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;

public interface CommunicationModelAPI extends Library {
    CommunicationModelAPI INSTANCE = Native.load("CommunicationModelCAPI", CommunicationModelAPI.class);
    
    Pointer CommModel_Create();
    int CommModel_SetEnvironment(Pointer handle, CommEnvironment env);
    int CommModel_CalculateCommunicationRange(Pointer handle, DoubleByReference range);
    int CommModel_Destroy(Pointer handle);
    
    class CommEnvironment extends Structure {
        public double frequency;
        public double bandwidth;
        public double transmitPower;
        public double noisePower;
        public double distance;
        public int environmentType;
        public double temperature;
        public double humidity;
        public double atmosphericPressure;
    }
}

// 使用示例
Pointer handle = CommunicationModelAPI.INSTANCE.CommModel_Create();
CommEnvironment env = new CommEnvironment();
env.frequency = 2.4e9;
env.bandwidth = 20e6;
env.transmitPower = 1.0;
env.distance = 1000.0;

CommunicationModelAPI.INSTANCE.CommModel_SetEnvironment(handle, env);

DoubleByReference range = new DoubleByReference();
CommunicationModelAPI.INSTANCE.CommModel_CalculateCommunicationRange(handle, range);
System.out.println("通信距离: " + (range.getValue() / 1000) + " km");

CommunicationModelAPI.INSTANCE.CommModel_Destroy(handle);
```

## 错误处理

### 错误代码
```c
typedef enum {
    COMM_SUCCESS = 0,                    // 成功
    COMM_ERROR_INVALID_HANDLE = -1,      // 无效句柄
    COMM_ERROR_NULL_POINTER = -2,        // 空指针
    COMM_ERROR_INVALID_PARAMETER = -3,   // 无效参数
    COMM_ERROR_CALCULATION_FAILED = -4,  // 计算失败
    COMM_ERROR_MEMORY_ALLOCATION = -5,   // 内存分配失败
    COMM_ERROR_FILE_OPERATION = -6,      // 文件操作失败
    COMM_ERROR_UNKNOWN = -99             // 未知错误
} CommResult;
```

### C#异常处理
```csharp
try
{
    var linkStatus = commModel.CalculateLinkStatus();
    // 处理结果
}
catch (CommunicationModelException ex)
{
    Console.WriteLine($"通信模型错误: {ex.Message}");
}
catch (ObjectDisposedException ex)
{
    Console.WriteLine("模型实例已被释放");
}
```

## 性能考虑

### 内存管理
- 及时释放动态分配的数组
- 使用 `using` 语句确保资源释放
- 避免频繁创建/销毁模型实例

### 线程安全
- 每个线程使用独立的模型实例
- 避免多线程共享同一个句柄
- 必要时使用锁机制保护共享资源

### 性能优化
- 批量设置参数减少函数调用
- 缓存计算结果避免重复计算
- 使用适当的数据类型减少转换开销

## 故障排除

### 常见问题

1. **动态库加载失败**
   - 检查库文件路径是否正确
   - 确认库文件与程序架构匹配（x64/x86）
   - Windows: 检查Visual C++运行时是否安装
   - Linux: 检查LD_LIBRARY_PATH环境变量

2. **函数调用失败**
   - 检查函数参数类型和调用约定
   - 确认句柄有效性
   - 查看错误代码确定具体问题

3. **内存泄漏**
   - 确保每个Create对应一个Destroy
   - 及时释放分配的数组
   - 使用内存检测工具验证

4. **计算结果异常**
   - 检查输入参数范围和有效性
   - 确认环境参数设置正确
   - 查看详细报告了解计算过程

### 调试技巧

1. **启用详细日志**
   ```csharp
   // 生成详细报告查看内部状态
   var report = commModel.GenerateDetailedReport();
   Console.WriteLine(report);
   ```

2. **参数验证**
   ```csharp
   // 获取当前环境参数
   var env = commModel.GetEnvironment();
   Console.WriteLine($"当前频率: {env.Frequency}");
   ```

3. **分步测试**
   ```csharp
   // 逐步设置参数并验证
   commModel.SetFrequency(2.4e9);
   commModel.SetBandwidth(20e6);
   var range = commModel.CalculateCommunicationRange();
   ```

## 版本兼容性

- **API版本**: 1.4.0
- **最低.NET版本**: .NET 6.0
- **支持的平台**: Windows x64, Linux x64
- **编译器要求**: C++17标准

## 许可证

本项目采用MIT许可证，详见LICENSE文件。

## 技术支持

如有问题或建议，请通过以下方式联系：
- 提交Issue到项目仓库
- 发送邮件到技术支持邮箱
- 查看项目Wiki获取更多信息