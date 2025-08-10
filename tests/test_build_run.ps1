# 通信模型测试构建和运行脚本
# 该脚本执行从CMake配置到运行测试的完整流程

Write-Host "=== 通信模型测试构建和运行脚本 ===" -ForegroundColor Green
Write-Host "开始时间: $(Get-Date)" -ForegroundColor Yellow

# 设置错误处理
$ErrorActionPreference = "Stop"

# 确保在build目录中执行
$buildDir = Join-Path $PSScriptRoot "build"
if (-not (Test-Path $buildDir)) {
    Write-Host "创建build目录..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Path $buildDir -Force
}

Write-Host "切换到build目录: $buildDir" -ForegroundColor Yellow
Set-Location $buildDir

try {
    # 步骤1: CMake配置
    Write-Host "`n[步骤1] 配置CMake项目..." -ForegroundColor Cyan
    cmake . -G "Visual Studio 17 2022"
    if ($LASTEXITCODE -ne 0) {
        throw "CMake配置失败，退出码: $LASTEXITCODE"
    }
    Write-Host "CMake配置成功!" -ForegroundColor Green

    # 步骤2: 构建项目
    Write-Host "`n[步骤2] 构建项目 (Release配置)..." -ForegroundColor Cyan
    cmake --build . --config Release
    if ($LASTEXITCODE -ne 0) {
        throw "项目构建失败，退出码: $LASTEXITCODE"
    }
    Write-Host "项目构建成功!" -ForegroundColor Green

    # 步骤3: 运行测试
    Write-Host "`n[步骤3] 运行测试..." -ForegroundColor Cyan
    ctest -C Release --output-on-failure
    if ($LASTEXITCODE -ne 0) {
        Write-Host "测试执行完成，但有测试失败，退出码: $LASTEXITCODE" -ForegroundColor Yellow
    } else {
        Write-Host "所有测试通过!" -ForegroundColor Green
    }

    Write-Host "`n=== 脚本执行完成 ===" -ForegroundColor Green
    Write-Host "结束时间: $(Get-Date)" -ForegroundColor Yellow

} catch {
    Write-Host "`n错误: $_" -ForegroundColor Red
    Write-Host "脚本执行失败，请检查错误信息" -ForegroundColor Red
    exit 1
}