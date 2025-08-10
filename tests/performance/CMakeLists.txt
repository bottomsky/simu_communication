# 性能测试CMakeLists.txt

# 收集所有性能测试源文件
file(GLOB PERFORMANCE_TEST_SOURCES "*.cpp")

# 创建性能测试可执行文件
add_executable(performance_tests ${PERFORMANCE_TEST_SOURCES})

# 链接库
target_link_libraries(performance_tests 
    CommunicationModelLib
    GTest::GTest 
    GTest::Main
)

# 设置输出目录
set_target_properties(performance_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../build
)

# 性能测试特定的编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(performance_tests PRIVATE PERFORMANCE_TEST_OPTIMIZED)
endif()