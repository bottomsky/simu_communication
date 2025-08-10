# 集成测试CMakeLists.txt

# 收集所有集成测试源文件
file(GLOB INTEGRATION_TEST_SOURCES "*.cpp")

# 创建集成测试可执行文件
add_executable(integration_tests ${INTEGRATION_TEST_SOURCES})

# 链接库
target_link_libraries(integration_tests 
    CommunicationModelLib
    GTest::GTest 
    GTest::Main
)

# 设置输出目录
set_target_properties(integration_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../build
)