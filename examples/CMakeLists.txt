# 示例程序构建配置
cmake_minimum_required(VERSION 3.16)

# 基础使用示例
add_executable(basic_usage_example
    basic_usage_example.cpp
)

target_link_libraries(basic_usage_example
    CommunicationModel
)

# 设置示例程序的输出目录
set_target_properties(
    basic_usage_example
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
)

# 添加自定义目标用于运行基础示例
add_custom_target(run_examples
    COMMAND echo "=== 基础使用示例 ===" && $<TARGET_FILE:basic_usage_example>
    DEPENDS basic_usage_example
    COMMENT "运行基础示例程序"
)