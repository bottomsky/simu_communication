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

- **🔧 开发语言**: C++11及以上标准
- **🏗️ 构建系统**: CMake跨平台构建
- **📊 数据格式**: JSON格式的配置和测试数据
- **🌐 跨平台**: 支持Windows和Linux平台
- **📚 API接口**: 提供C风格API，支持外部语言调用
- **🧪 测试驱动**: 完整的测试用例和验证数据

## 项目结构

```
signal-transmission-model-cpp/
├── build/                          # 构建目录
│   ├── CMakeLists.txt              # CMake配置文件
│   ├── bin/                        # 可执行文件输出
│   ├── lib/                        # 库文件输出
│   └── obj/                        # 中间文件
├── docs/                           # 技术文档
│   ├── data/                       # 测试数据
│   ├── 通信距离模型.md
│   ├── 通信接收模型.md
│   ├── 通信干扰模型.md
│   └── 通信抗干扰模型.md
├── scripts/                        # 构建和部署脚本
├── source/                         # 源代码
│   ├── header/                     # 头文件
│   └── src/                        # 源文件
├── test/                           # 测试代码
├── .gitignore                      # Git忽略规则
├── README.md                       # 项目说明文档
└── 开发约束文档.md                  # 开发规范文档
```

## 快速开始

### 环境要求

- **编译器**: GCC 7.0+ / MSVC 2017+ / Clang 6.0+
- **构建工具**: CMake 3.10+
- **操作系统**: Windows 10+ / Linux (Ubuntu 18.04+)

### 编译构建

1. **克隆项目**
   ```bash
   git clone <repository-url>
   cd signal-transmission-model-cpp
   ```

2. **创建构建目录**
   ```bash
   mkdir -p build
   cd build
   ```

3. **配置和编译**
   ```bash
   cmake ..
   make  # Linux
   # 或者在Windows上使用Visual Studio
   ```

4. **运行测试**
   ```bash
   ./bin/test_runner
   ```

## 使用示例

### C++ API 使用

```cpp
#include "CommunicationDistanceModel.h"
#include "CommunicationJammerModel.h"

// 创建通信距离模型
CommunicationDistanceModel distanceModel;
distanceModel.setMaxLineOfSight(10.0);
distanceModel.setTransmitPower(20.0);
distanceModel.setEnvironmentType(EnvironmentType::OPEN_FIELD);

// 计算有效通信距离
double effectiveDistance = distanceModel.calculateEffectiveDistance();
std::cout << "有效通信距离: " << effectiveDistance << " km" << std::endl;

// 创建干扰模型
CommunicationJammerModel jammerModel;
jammerModel.setJammerType(JammerType::GAUSSIAN_NOISE);
jammerModel.setJammerPower(15.0);

// 计算干扰效果
double jammerEffect = jammerModel.calculateJammerEffect();
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
- [通信距离模型文档](./docs/通信距离模型.md) - 距离计算模型详解
- [通信干扰模型文档](./docs/通信干扰模型.md) - 干扰仿真模型详解
- [API参考文档](./docs/api/) - 完整的API接口文档

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

## 许可证

本项目采用 [MIT License](LICENSE) 许可证。

## 联系方式

- **项目维护**: 通信对抗仿真团队
- **技术支持**: [技术支持邮箱]
- **问题反馈**: [GitHub Issues](issues)

---

**注意**: 本项目为仿真研究用途，请遵循相关法律法规使用。