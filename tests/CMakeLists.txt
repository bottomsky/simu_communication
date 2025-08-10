# 测试构建配置
cmake_minimum_required(VERSION 3.16)

# 尝试查找系统安装的Google Test
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    # 如果没有找到GTest，使用FetchContent下载
    include(FetchContent)
    
    # 设置下载选项
    set(FETCHCONTENT_QUIET OFF)
    
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.12.1
        GIT_SHALLOW    TRUE
    )
    
    # 对于Windows: 防止覆盖父项目的编译器/链接器设置
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    
    # 禁用Google Test的安装
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    
    FetchContent_MakeAvailable(googletest)
    
    # 如果仍然失败，创建简单的测试框架
    if(NOT TARGET gtest_main)
        message(WARNING "Google Test下载失败，将创建简单的测试框架")
        
        # 创建简单的测试主函数
        add_library(simple_test_main STATIC)
        target_sources(simple_test_main PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/simple_test_main.cpp
        )
        
        # 定义简单的测试宏
        target_compile_definitions(simple_test_main PUBLIC
            EXPECT_EQ=simple_expect_eq
            EXPECT_TRUE=simple_expect_true
            EXPECT_FALSE=simple_expect_false
            TEST_F=simple_test_f
        )
        
        # 设置别名
        add_library(gtest_main ALIAS simple_test_main)
        add_library(gtest ALIAS simple_test_main)
    endif()
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

add_executable(test_communication_receive
    unit/test_communication_receive_model.cpp
)

target_link_libraries(test_communication_receive
    CommunicationModel
    gtest_main
    gtest
)

# 新增的单元测试
add_executable(test_communication_anti_jam
    unit/test_communication_anti_jam_model.cpp
)

target_link_libraries(test_communication_anti_jam
    CommunicationModel
    gtest_main
    gtest
)

add_executable(test_communication_jammer
    unit/test_communication_jammer_model.cpp
)

target_link_libraries(test_communication_jammer
    CommunicationModel
    gtest_main
    gtest
)

add_executable(test_communication_model_api
    unit/test_communication_model_api.cpp
)

target_link_libraries(test_communication_model_api
    CommunicationModel
    gtest_main
    gtest
)

# 参数配置测试
add_executable(test_anti_jam_parameter_config
    unit/test_anti_jam_parameter_config.cpp
)

target_link_libraries(test_anti_jam_parameter_config
    CommunicationModel
    gtest_main
    gtest
)

add_executable(test_jammer_parameter_config
    unit/test_jammer_parameter_config.cpp
)

target_link_libraries(test_jammer_parameter_config
    CommunicationModel
    gtest_main
    gtest
)

add_executable(test_receive_parameter_config
    unit/test_receive_parameter_config.cpp
)

target_link_libraries(test_receive_parameter_config
    CommunicationModel
    gtest_main
    gtest
)

# 简单测试
add_executable(simple_receive_test
    unit/simple_receive_test.cpp
)

target_link_libraries(simple_receive_test
    CommunicationModel
    gtest_main
    gtest
)

# 常量测试
add_executable(test_constants
    test_constants.cpp
)

target_link_libraries(test_constants
    CommunicationModel
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

# 迁移验证测试
add_executable(test_environment_loss_migration
    integration/test_environment_loss_migration.cpp
)

target_link_libraries(test_environment_loss_migration
    CommunicationModel
    gtest_main
    gtest
)

add_executable(test_path_loss_migration
    integration/test_path_loss_migration.cpp
)

target_link_libraries(test_path_loss_migration
    CommunicationModel
    gtest_main
    gtest
)

add_executable(test_free_space_path_loss_refactor
    integration/test_free_space_path_loss_refactor.cpp
)

target_link_libraries(test_free_space_path_loss_refactor
    CommunicationModel
    gtest_main
    gtest
)

# 注册测试
# 基础单元测试
add_test(NAME SignalTransmissionModelTest COMMAND test_signal_transmission)
add_test(NAME CommunicationDistanceModelTest COMMAND test_communication_distance)
add_test(NAME CommunicationReceiveModelTest COMMAND test_communication_receive)

# 新增单元测试
add_test(NAME CommunicationAntiJamModelTest COMMAND test_communication_anti_jam)
add_test(NAME CommunicationJammerModelTest COMMAND test_communication_jammer)
add_test(NAME CommunicationModelApiTest COMMAND test_communication_model_api)

# 参数配置测试
add_test(NAME AntiJamParameterConfigTest COMMAND test_anti_jam_parameter_config)
add_test(NAME JammerParameterConfigTest COMMAND test_jammer_parameter_config)
add_test(NAME ReceiveParameterConfigTest COMMAND test_receive_parameter_config)

# 简单测试
add_test(NAME SimpleReceiveTest COMMAND simple_receive_test)

# 常量测试
add_test(NAME ConstantsTest COMMAND test_constants)

# 集成测试
add_test(NAME BasicIntegrationTest COMMAND test_integration)

# 迁移验证测试
add_test(NAME EnvironmentLossMigrationTest COMMAND test_environment_loss_migration)
add_test(NAME PathLossMigrationTest COMMAND test_path_loss_migration)
add_test(NAME FreeSpacePathLossRefactorTest COMMAND test_free_space_path_loss_refactor)

# 设置测试属性
# 基础单元测试属性
set_tests_properties(SignalTransmissionModelTest PROPERTIES
    TIMEOUT 30
    LABELS "unit;basic"
)

set_tests_properties(CommunicationDistanceModelTest PROPERTIES
    TIMEOUT 30
    LABELS "unit;basic"
)

set_tests_properties(CommunicationReceiveModelTest PROPERTIES
    TIMEOUT 30
    LABELS "unit;basic"
)

# 新增单元测试属性
set_tests_properties(CommunicationAntiJamModelTest PROPERTIES
    TIMEOUT 30
    LABELS "unit;antijam"
)

set_tests_properties(CommunicationJammerModelTest PROPERTIES
    TIMEOUT 30
    LABELS "unit;jammer"
)

set_tests_properties(CommunicationModelApiTest PROPERTIES
    TIMEOUT 30
    LABELS "unit;api"
)

# 参数配置测试属性
set_tests_properties(AntiJamParameterConfigTest PROPERTIES
    TIMEOUT 30
    LABELS "unit;config;antijam"
)

set_tests_properties(JammerParameterConfigTest PROPERTIES
    TIMEOUT 30
    LABELS "unit;config;jammer"
)

set_tests_properties(ReceiveParameterConfigTest PROPERTIES
    TIMEOUT 30
    LABELS "unit;config;receive"
)

# 简单测试属性
set_tests_properties(SimpleReceiveTest PROPERTIES
    TIMEOUT 30
    LABELS "unit;simple"
)

# 常量测试属性
set_tests_properties(ConstantsTest PROPERTIES
    TIMEOUT 30
    LABELS "unit;constants"
)

# 集成测试属性
set_tests_properties(BasicIntegrationTest PROPERTIES
    TIMEOUT 60
    LABELS "integration"
)

# 迁移验证测试属性
set_tests_properties(EnvironmentLossMigrationTest PROPERTIES
    TIMEOUT 60
    LABELS "integration;migration"
)

set_tests_properties(PathLossMigrationTest PROPERTIES
    TIMEOUT 60
    LABELS "integration;migration"
)

set_tests_properties(FreeSpacePathLossRefactorTest PROPERTIES
    TIMEOUT 60
    LABELS "integration;refactor"
)

# 添加自定义目标用于运行所有测试
add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
    DEPENDS 
        test_signal_transmission 
        test_communication_distance 
        test_communication_receive
        test_communication_anti_jam
        test_communication_jammer
        test_communication_model_api
        test_anti_jam_parameter_config
        test_jammer_parameter_config
        test_receive_parameter_config
        simple_receive_test
        test_constants
        test_integration
        test_environment_loss_migration
        test_path_loss_migration
        test_free_space_path_loss_refactor
    COMMENT "运行所有测试"
)

# 添加自定义目标用于运行单元测试
add_custom_target(run_unit_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose -L unit
    DEPENDS 
        test_signal_transmission 
        test_communication_distance 
        test_communication_receive
        test_communication_anti_jam
        test_communication_jammer
        test_communication_model_api
        test_anti_jam_parameter_config
        test_jammer_parameter_config
        test_receive_parameter_config
        simple_receive_test
        test_constants
    COMMENT "运行单元测试"
)

# 添加自定义目标用于运行集成测试
add_custom_target(run_integration_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose -L integration
    DEPENDS 
        test_integration
        test_environment_loss_migration
        test_path_loss_migration
        test_free_space_path_loss_refactor
    COMMENT "运行集成测试"
)

# 添加自定义目标用于运行配置测试
add_custom_target(run_config_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose -L config
    DEPENDS 
        test_anti_jam_parameter_config
        test_jammer_parameter_config
        test_receive_parameter_config
    COMMENT "运行配置测试"
)

# 添加自定义目标用于运行迁移测试
add_custom_target(run_migration_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose -L migration
    DEPENDS 
        test_environment_loss_migration
        test_path_loss_migration
    COMMENT "运行迁移测试"
)