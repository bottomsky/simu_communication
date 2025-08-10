@echo off
setlocal enabledelayedexpansion

echo ========================================
echo Communication Model Library Windows Build Script
echo ========================================

:: Check if CMake is installed
cmake --version >nul 2>&1
if errorlevel 1 (
    echo Error: CMake not found, please install CMake first
    pause
    exit /b 1
)

:: Set build directories
set BUILD_DIR=build
set INSTALL_DIR=install

:: Clean old build directory
if exist %BUILD_DIR% (
    echo Cleaning old build directory...
    rmdir /s /q %BUILD_DIR%
)

if exist %INSTALL_DIR% (
    echo Cleaning old install directory...
    rmdir /s /q %INSTALL_DIR%
)

:: Create build directory
mkdir %BUILD_DIR%
cd %BUILD_DIR%

echo.
echo ========================================
echo Configuring CMake project...
echo ========================================

:: Configure CMake project
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../%INSTALL_DIR% -DBUILD_TESTS=ON

if errorlevel 1 (
    echo Error: CMake configuration failed
    cd ..
    pause
    exit /b 1
)

echo.
echo ========================================
echo Building project...
echo ========================================

:: Build project
cmake --build . --config Release --parallel

if errorlevel 1 (
    echo Error: Project build failed
    cd ..
    pause
    exit /b 1
)

echo.
echo ========================================
echo Installing library files...
echo ========================================

:: Install library files
cmake --install . --config Release

if errorlevel 1 (
    echo Error: Library installation failed
    cd ..
    pause
    exit /b 1
)

cd ..

echo.
echo ========================================
echo Building C# example project...
echo ========================================

:: Check if .NET SDK is installed
dotnet --version >nul 2>&1
if errorlevel 1 (
    echo Warning: .NET SDK not found, skipping C# example build
    goto :skip_csharp
)

:: Build C# example project
cd examples\csharp
dotnet build -c Release

if errorlevel 1 (
    echo Error: C# example project build failed
    cd ..\..
    pause
    exit /b 1
)

cd ..\..

:skip_csharp

echo.
echo ========================================
echo Build completed successfully!
echo ========================================
echo.
echo Build outputs:
echo   - Static library: %BUILD_DIR%\lib\Release\CommunicationModel.lib
echo   - Dynamic library: %BUILD_DIR%\bin\Release\CommunicationModel.dll
echo   - C API dynamic library: %BUILD_DIR%\bin\Release\CommunicationModelCAPI.dll
echo   - Header files: %INSTALL_DIR%\include\CommunicationModel\
echo   - Test programs: %BUILD_DIR%\tests\Release\
echo.
echo C# example:
echo   - Project: examples\csharp\CommunicationModelExample.csproj
echo   - Executable: examples\csharp\bin\Release\net6.0\CommunicationModelExample.exe
echo.

:: Run tests (optional)
set /p run_tests="Run test programs? (y/n): "
if /i "%run_tests%"=="y" (
    echo.
    echo ========================================
    echo Running test programs...
    echo ========================================
    
    if exist %BUILD_DIR%\tests\Release\test_ambient_temperature.exe (
        echo Running ambient temperature test...
        %BUILD_DIR%\tests\Release\test_ambient_temperature.exe
        echo.
    )
    
    if exist examples\csharp\bin\Release\net6.0\CommunicationModelExample.exe (
        echo Running C# example...
        examples\csharp\bin\Release\net6.0\CommunicationModelExample.exe
        echo.
    )
)

echo Build script execution completed!
pause