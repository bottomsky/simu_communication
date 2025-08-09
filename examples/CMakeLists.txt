# 示例程序构建配置
cmake_minimum_required(VERSION 3.16)

# 基础使用示例
add_executable(basic_usage_example
    basic_usage_example.cpp
)

target_link_libraries(basic_usage_example
    CommunicationModel
)

# 高级功能示例
add_executable(advanced_features_example
    advanced_features_example.cpp
)

target_link_libraries(advanced_features_example
    CommunicationModel
)

# 性能测试示例
add_executable(performance_benchmark
    performance_benchmark.cpp
)

target_link_libraries(performance_benchmark
    CommunicationModel
)

# 参数优化示例
add_executable(parameter_optimization_example
    parameter_optimization_example.cpp
)

target_link_libraries(parameter_optimization_example
    CommunicationModel
)

# 设置示例程序的输出目录
set_target_properties(
    basic_usage_example
    advanced_features_example
    performance_benchmark
    parameter_optimization_example
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
)

# 添加自定义目标用于运行所有示例
add_custom_target(run_examples
    COMMAND echo "=== 基础使用示例 ===" && $<TARGET_FILE:basic_usage_example>
    COMMAND echo "=== 高级功能示例 ===" && $<TARGET_FILE:advanced_features_example>
    COMMAND echo "=== 性能基准测试 ===" && $<TARGET_FILE:performance_benchmark>
    COMMAND echo "=== 参数优化示例 ===" && $<TARGET_FILE:parameter_optimization_example>
    DEPENDS basic_usage_example advanced_features_example performance_benchmark parameter_optimization_example
    COMMENT "运行所有示例程序"
)