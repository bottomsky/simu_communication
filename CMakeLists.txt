cmake_minimum_required(VERSION 3.16)
project(CommunicationCounterModel VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 包含目录
include_directories(source/include)

# 源文件
file(GLOB_RECURSE SOURCES "source/src/*.cpp")
file(GLOB_RECURSE HEADERS "source/include/*.h")

# 创建静态库
add_library(CommunicationModel STATIC ${SOURCES} ${HEADERS})

# 设置库的包含目录
target_include_directories(CommunicationModel PUBLIC source/include)

# 选项：是否构建测试
option(BUILD_TESTS "Build unit tests" ON)

# 选项：是否构建示例
option(BUILD_EXAMPLES "Build examples" ON)

# 如果启用测试，添加测试子目录
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# 如果启用示例，添加示例子目录
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# 安装配置
install(TARGETS CommunicationModel
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY source/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# 生成配置文件
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CommunicationModelConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/CommunicationModelConfig.cmake"
    @ONLY
)

# 打印配置信息
message(STATUS "=== 通信对抗模型构建配置 ===")
message(STATUS "项目版本: ${PROJECT_VERSION}")
message(STATUS "C++标准: ${CMAKE_CXX_STANDARD}")
message(STATUS "构建类型: ${CMAKE_BUILD_TYPE}")
message(STATUS "构建测试: ${BUILD_TESTS}")
message(STATUS "构建示例: ${BUILD_EXAMPLES}")
message(STATUS "输出目录: ${CMAKE_BINARY_DIR}")