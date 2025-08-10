#!/bin/bash

echo "========================================"
echo "通信模型库 Linux 构建脚本"
echo "========================================"

# 检查CMake是否安装
if ! command -v cmake &> /dev/null; then
    echo "错误: 未找到CMake，请先安装CMake"
    echo "Ubuntu/Debian: sudo apt-get install cmake"
    echo "CentOS/RHEL: sudo yum install cmake"
    echo "Fedora: sudo dnf install cmake"
    exit 1
fi

# 检查编译器是否安装
if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
    echo "错误: 未找到C++编译器，请先安装g++或clang++"
    echo "Ubuntu/Debian: sudo apt-get install build-essential"
    echo "CentOS/RHEL: sudo yum groupinstall 'Development Tools'"
    echo "Fedora: sudo dnf groupinstall 'Development Tools'"
    exit 1
fi

# 设置构建目录
BUILD_DIR="build"
INSTALL_DIR="install"

# 清理旧的构建目录
if [ -d "$BUILD_DIR" ]; then
    echo "清理旧的构建目录..."
    rm -rf "$BUILD_DIR"
fi

# 创建构建目录
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo ""
echo "========================================"
echo "配置CMake项目..."
echo "========================================"

# 配置CMake项目
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="../$INSTALL_DIR" \
    -DBUILD_TESTS=ON \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON

if [ $? -ne 0 ]; then
    echo "错误: CMake配置失败"
    cd ..
    exit 1
fi

echo ""
echo "========================================"
echo "构建项目..."
echo "========================================"

# 获取CPU核心数用于并行构建
NPROC=$(nproc 2>/dev/null || echo 4)

# 构建项目
cmake --build . --parallel $NPROC

if [ $? -ne 0 ]; then
    echo "错误: 项目构建失败"
    cd ..
    exit 1
fi

echo ""
echo "========================================"
echo "安装库文件..."
echo "========================================"

# 安装库文件
cmake --install .

if [ $? -ne 0 ]; then
    echo "错误: 库文件安装失败"
    cd ..
    exit 1
fi

cd ..

echo ""
echo "========================================"
echo "构建C#示例项目..."
echo "========================================"

# 检查.NET SDK是否安装
if ! command -v dotnet &> /dev/null; then
    echo "警告: 未找到.NET SDK，跳过C#示例构建"
    echo "安装.NET SDK: https://dotnet.microsoft.com/download"
else
    # 构建C#示例项目
    cd examples/csharp
    dotnet build -c Release

    if [ $? -ne 0 ]; then
        echo "错误: C#示例项目构建失败"
        cd ../..
        exit 1
    fi

    cd ../..
fi

echo ""
echo "========================================"
echo "构建完成！"
echo "========================================"
echo ""
echo "构建输出:"
echo "  - 静态库: $BUILD_DIR/lib/libCommunicationModel.a"
echo "  - 动态库: $BUILD_DIR/lib/libCommunicationModel.so"
echo "  - C API动态库: $BUILD_DIR/lib/libCommunicationModelCAPI.so"
echo "  - 头文件: $INSTALL_DIR/include/CommunicationModel/"
echo "  - 测试程序: $BUILD_DIR/tests/"
echo ""
echo "C#示例:"
echo "  - 项目: examples/csharp/CommunicationModelExample.csproj"
echo "  - 可执行文件: examples/csharp/bin/Release/net6.0/CommunicationModelExample"
echo ""

# 设置库路径
export LD_LIBRARY_PATH="$(pwd)/$BUILD_DIR/lib:$LD_LIBRARY_PATH"

# 运行测试（可选）
read -p "是否运行测试程序? (y/n): " run_tests
if [[ "$run_tests" =~ ^[Yy]$ ]]; then
    echo ""
    echo "========================================"
    echo "运行测试程序..."
    echo "========================================"
    
    if [ -f "$BUILD_DIR/tests/test_ambient_temperature" ]; then
        echo "运行环境温度测试..."
        ./$BUILD_DIR/tests/test_ambient_temperature
        echo ""
    fi
    
    if [ -f "examples/csharp/bin/Release/net6.0/CommunicationModelExample" ]; then
        echo "运行C#示例..."
        cd examples/csharp/bin/Release/net6.0
        ./CommunicationModelExample
        cd ../../../../..
        echo ""
    fi
fi

echo "构建脚本执行完成！"