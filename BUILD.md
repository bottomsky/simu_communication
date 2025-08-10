# 构建说明

## 使用build目录进行构建

为了保持项目根目录的整洁，所有CMake生成的文件现在都放在`build`目录中。

### 构建步骤

1. **配置项目**（在项目根目录执行）：
   ```bash
   cmake -S . -B build
   ```

2. **编译项目**：
   ```bash
   cmake --build build --config Debug
   ```
   或者
   ```bash
   cmake --build build --config Release
   ```

3. **运行测试**：
   ```bash
   cd build
   ctest --config Debug
   ```

### 目录结构

```
signal-transmission-model-cpp/
├── build/                    # CMake生成的所有文件
│   ├── bin/                 # 可执行文件输出目录
│   ├── lib/                 # 库文件输出目录
│   ├── _deps/               # 依赖项（如GoogleTest）
│   └── *.vcxproj           # Visual Studio项目文件
├── source/                  # 源代码
├── tests/                   # 测试代码
├── examples/                # 示例代码
└── CMakeLists.txt          # CMake配置文件
```

### 清理构建

如果需要完全重新构建，可以删除build目录：
```bash
Remove-Item -Recurse -Force build
mkdir build
cmake -S . -B build
```

### 注意事项

- 所有的构建产物（库文件、可执行文件）都会生成在`build/bin`和`build/lib`目录中
- 项目根目录保持干净，不会有CMake生成的临时文件
- 可以安全地将`build/`目录添加到`.gitignore`中