cmake_minimum_required(VERSION 3.16)
project(CommunicationModelLibrary LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Warnings and common flags
if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Paths (CMakeLists is placed in build/; source lives one level up)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../source/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../source/header)
set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../examples)

# Source grouping
file(GLOB ALL_SRC "${SRC_DIR}/*.cpp")
set(CAPI_SRC "${SRC_DIR}/CommunicationModelCAPI.cpp")
list(REMOVE_ITEM ALL_SRC "${CAPI_SRC}")

# Core libraries (Shared and Static)
add_library(CommunicationModelShared SHARED ${ALL_SRC})
target_include_directories(CommunicationModelShared 
    PUBLIC 
        $<BUILD_INTERFACE:${INC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/CommunicationModel>
)
# Windows下自动导出未显式标记的符号，方便其他C++程序直接链接DLL
if(MSVC)
    set_target_properties(CommunicationModelShared PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
# 提供一个更友好的默认别名，方便外部C++项目引用
add_library(CommunicationModel::Core ALIAS CommunicationModelShared)

add_library(CommunicationModelStatic STATIC ${ALL_SRC})
target_include_directories(CommunicationModelStatic 
    PUBLIC 
        $<BUILD_INTERFACE:${INC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/CommunicationModel>
)
# For static builds, disable dll import/export decorations
target_compile_definitions(CommunicationModelStatic PUBLIC COMMUNICATION_MODEL_STATIC)

# C API shared library, exporting functions with COMMUNICATION_MODEL_EXPORTS
add_library(CommunicationModelCAPI SHARED ${CAPI_SRC})
target_include_directories(CommunicationModelCAPI 
    PUBLIC 
        $<BUILD_INTERFACE:${INC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/CommunicationModel>
)
target_link_libraries(CommunicationModelCAPI PRIVATE CommunicationModelShared)
target_compile_definitions(CommunicationModelCAPI PRIVATE COMMUNICATION_MODEL_EXPORTS)
# 提供一个更友好的别名，方便其他项目引用C API
add_library(CommunicationModel::CAPI ALIAS CommunicationModelCAPI)

# Example executables (use the C++ API from the core library)
add_executable(basic_usage_example ${EXAMPLES_DIR}/basic_usage_example.cpp)
target_include_directories(basic_usage_example PRIVATE ${INC_DIR})
target_link_libraries(basic_usage_example PRIVATE CommunicationModelShared)

add_executable(environment_config_example ${EXAMPLES_DIR}/environment_config_example.cpp)
target_include_directories(environment_config_example PRIVATE ${INC_DIR})
target_link_libraries(environment_config_example PRIVATE CommunicationModelShared)

add_executable(simple_environment_config_example ${EXAMPLES_DIR}/simple_environment_config_example.cpp)
target_include_directories(simple_environment_config_example PRIVATE ${INC_DIR})
target_link_libraries(simple_environment_config_example PRIVATE CommunicationModelShared)

# Helpful grouping in IDEs like VS2022
source_group(TREE ${SRC_DIR} FILES ${ALL_SRC} ${CAPI_SRC})
source_group(TREE ${EXAMPLES_DIR} FILES 
    ${EXAMPLES_DIR}/basic_usage_example.cpp 
    ${EXAMPLES_DIR}/environment_config_example.cpp 
    ${EXAMPLES_DIR}/simple_environment_config_example.cpp)

# 安装规则：安装库与头文件，便于被 Python/C#/C++ 等外部项目使用
include(GNUInstallDirs)

install(TARGETS
    CommunicationModelCAPI
    CommunicationModelShared
    CommunicationModelStatic
    EXPORT CommunicationModelTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# 安装C/C++头文件（包含C API头），外部项目可通过 include/CommunicationModel 进行包含
install(DIRECTORY ${INC_DIR}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/CommunicationModel
        FILES_MATCHING PATTERN "*.h")

# 导出CMake目标，方便外部CMake项目使用 find_package 的替代方式：直接 include 该targets文件
install(EXPORT CommunicationModelTargets
        NAMESPACE CommunicationModel::
        FILE CommunicationModelTargets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CommunicationModel)