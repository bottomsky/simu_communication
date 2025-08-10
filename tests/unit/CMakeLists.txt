# 单元测试CMakeLists.txt

# 收集所有单元测试源文件
file(GLOB UNIT_TEST_SOURCES "*.cpp")

# 创建单元测试可执行文件
add_executable(unit_tests ${UNIT_TEST_SOURCES})

# 链接库
target_link_libraries(unit_tests 
    CommunicationModelLib
    GTest::GTest 
    GTest::Main
)

# 设置输出目录
set_target_properties(unit_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../build
)