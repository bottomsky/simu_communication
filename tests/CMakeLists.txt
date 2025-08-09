# 测试构建配置
cmake_minimum_required(VERSION 3.16)

# 查找或下载Google Test
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    # 如果没有找到GTest，使用FetchContent下载
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50f33f9142fd2db563d69b413611e5.zip
    )
    
    # 对于Windows: 防止覆盖父项目的编译器/链接器设置
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# 包含测试头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 基础模块测试
add_executable(test_signal_transmission
    unit/test_signal_transmission_model.cpp
)

target_link_libraries(test_signal_transmission
    CommunicationModel
    gtest_main
    gtest
)

add_executable(test_communication_distance
    unit/test_communication_distance_model.cpp
)

target_link_libraries(test_communication_distance
    CommunicationModel
    gtest_main
    gtest
)

# 集成测试
add_executable(test_integration
    integration/test_basic_integration.cpp
)

target_link_libraries(test_integration
    CommunicationModel
    gtest_main
    gtest
)

# 注册测试
add_test(NAME SignalTransmissionModelTest COMMAND test_signal_transmission)
add_test(NAME CommunicationDistanceModelTest COMMAND test_communication_distance)
add_test(NAME BasicIntegrationTest COMMAND test_integration)

# 设置测试属性
set_tests_properties(SignalTransmissionModelTest PROPERTIES
    TIMEOUT 30
    LABELS "unit;basic"
)

set_tests_properties(CommunicationDistanceModelTest PROPERTIES
    TIMEOUT 30
    LABELS "unit;basic"
)

set_tests_properties(BasicIntegrationTest PROPERTIES
    TIMEOUT 60
    LABELS "integration"
)

# 添加自定义目标用于运行所有测试
add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
    DEPENDS test_signal_transmission test_communication_distance test_integration
    COMMENT "运行所有测试"
)

# 添加自定义目标用于运行单元测试
add_custom_target(run_unit_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose -L unit
    DEPENDS test_signal_transmission test_communication_distance
    COMMENT "运行单元测试"
)

# 添加自定义目标用于运行集成测试
add_custom_target(run_integration_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose -L integration
    DEPENDS test_integration
    COMMENT "运行集成测试"
)