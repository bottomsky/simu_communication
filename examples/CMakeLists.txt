# 示例程序构建配置
cmake_minimum_required(VERSION 3.16)

# 基础使用示例
add_executable(basic_usage_example
    basic_usage_example.cpp
)

target_link_libraries(basic_usage_example
    CommunicationModel
)

# 环境配置示例
add_executable(environment_config_example
    environment_config_example.cpp
)

target_link_libraries(environment_config_example
    CommunicationModel
)

# 添加简化环境配置示例程序
add_executable(simple_environment_config_example
    simple_environment_config_example.cpp
)

target_link_libraries(simple_environment_config_example
    CommunicationModel
)

# 设置示例程序的输出目录
set_target_properties(
    basic_usage_example
    environment_config_example
    simple_environment_config_example
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
)

# 添加自定义目标用于运行示例
add_custom_target(run_examples
    COMMAND echo "=== 基础使用示例 ===" && $<TARGET_FILE:basic_usage_example> && echo "" && echo "=== 环境配置示例 ===" && $<TARGET_FILE:environment_config_example>
    DEPENDS basic_usage_example environment_config_example
    COMMENT "运行所有示例程序"
)