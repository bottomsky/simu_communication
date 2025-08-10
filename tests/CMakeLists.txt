cmake_minimum_required(VERSION 3.16)

# 设置项目名称
project(CommunicationModelTests)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Google Test
find_package(GTest REQUIRED)

# 包含头文件目录
include_directories(${CMAKE_SOURCE_DIR}/source/header)
include_directories(${CMAKE_SOURCE_DIR}/source/src)

# 设置构建输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

# 添加源文件
file(GLOB_RECURSE SOURCE_FILES 
    "${CMAKE_SOURCE_DIR}/source/src/*.cpp"
)

# 移除main.cpp如果存在
list(FILTER SOURCE_FILES EXCLUDE REGEX ".*main\\.cpp$")

# 创建静态库
add_library(CommunicationModelLib STATIC ${SOURCE_FILES})

# 单元测试
add_subdirectory(unit)

# 集成测试
add_subdirectory(integration)

# 性能测试
add_subdirectory(performance)

# 启用测试
enable_testing()

# 添加所有测试
add_test(NAME UnitTests COMMAND unit_tests)
add_test(NAME IntegrationTests COMMAND integration_tests)
add_test(NAME PerformanceTests COMMAND performance_tests)

# 设置测试属性
set_tests_properties(UnitTests PROPERTIES TIMEOUT 60)
set_tests_properties(IntegrationTests PROPERTIES TIMEOUT 120)
set_tests_properties(PerformanceTests PROPERTIES TIMEOUT 300)