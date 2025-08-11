# 通信模型测试构建和运行脚本
# 该脚本执行从CMake配置到运行测试的完整流程

param(
    [Alias("c")]
    [switch]$CleanBuild = $false,
    [Alias("t")]
    [switch]$TestOnly = $false,
    [switch]$Help
)

# 显示帮助信息
if ($Help) {
    Write-Host "通信模型测试构建和运行脚本" -ForegroundColor Green
    Write-Host ""
    Write-Host "用法:" -ForegroundColor Yellow
    Write-Host "  .\test_build_run.ps1 [-CleanBuild|-c] [-TestOnly|-t] [-Help]" -ForegroundColor White
    Write-Host ""
    Write-Host "参数:" -ForegroundColor Yellow
    Write-Host "  -CleanBuild, -c    清空build目录缓存后重新构建 (默认: false)" -ForegroundColor White
    Write-Host "  -TestOnly,  -t    仅运行测试，跳过CMake配置与构建步骤" -ForegroundColor White
    Write-Host "  -Help              显示此帮助信息" -ForegroundColor White
    Write-Host "" 
    Write-Host "示例:" -ForegroundColor Yellow
    Write-Host "  .\test_build_run.ps1                # 增量构建" -ForegroundColor White
    Write-Host "  .\test_build_run.ps1 -CleanBuild    # 清空缓存后重新构建" -ForegroundColor White
    Write-Host "  .\test_build_run.ps1 -c             # 清空缓存后重新构建 (简写)" -ForegroundColor White
    Write-Host "  .\test_build_run.ps1 -TestOnly      # 仅运行测试" -ForegroundColor White
    Write-Host "  .\test_build_run.ps1 -t             # 仅运行测试 (简写)" -ForegroundColor White
    exit 0
}

Write-Host "=== 通信模型测试构建和运行脚本 ===" -ForegroundColor Green
Write-Host "开始时间: $(Get-Date)" -ForegroundColor Yellow
Write-Host "清空构建缓存: $CleanBuild" -ForegroundColor Yellow

# 设置错误处理
$ErrorActionPreference = "Stop"

# 确保在build目录中执行
$buildDir = Join-Path $PSScriptRoot "build"

# 根据参数决定是否清除build目录缓存
if ($CleanBuild -and (Test-Path $buildDir)) {
    Write-Host "清除build目录缓存..." -ForegroundColor Yellow
    # 保留CMakeLists.txt和CMakeCache.txt文件，删除其他所有内容
    $cmakeListsFile = Join-Path $buildDir "CMakeLists.txt"
    $cmakeCacheFile = Join-Path $PSScriptRoot "CMakeCache.txt"
    $hasCMakeLists = Test-Path $cmakeListsFile
    $hasCMakeCache = Test-Path $cmakeCacheFile
    
    if ($hasCMakeLists) {
        # 临时备份CMakeLists.txt
        $tempListsFile = Join-Path $env:TEMP "CMakeLists_backup.txt"
        Copy-Item $cmakeListsFile $tempListsFile -Force
    }
    
    # 删除build目录中的所有内容
    Get-ChildItem $buildDir -Force | Remove-Item -Recurse -Force
    
    if ($hasCMakeLists) {
        # 恢复CMakeLists.txt
        Copy-Item $tempListsFile $cmakeListsFile -Force
        Remove-Item $tempListsFile -Force
        Write-Host "已保留CMakeLists.txt文件" -ForegroundColor Green
    }
    
    if ($hasCMakeCache) {
        # 复制CMakeCache.txt到build目录
        $buildCacheFile = Join-Path $buildDir "CMakeCache.txt"
        Copy-Item $cmakeCacheFile $buildCacheFile -Force
        Write-Host "已复制CMakeCache.txt文件到build目录" -ForegroundColor Green
    }
    
    Write-Host "build目录缓存清除完成" -ForegroundColor Green
} elseif ($CleanBuild) {
    Write-Host "build目录不存在，将创建新目录" -ForegroundColor Yellow
}

# 确保build目录存在
if (-not (Test-Path $buildDir)) {
    Write-Host "创建build目录..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Path $buildDir -Force
} elseif (-not $CleanBuild) {
    Write-Host "使用现有build目录进行增量构建..." -ForegroundColor Yellow
}

Write-Host "切换到build目录: $buildDir" -ForegroundColor Yellow
Set-Location $buildDir

function Invoke-CTestColorized {
    param(
        [string]$Configuration = "Release"
    )
    & ctest -C $Configuration --output-on-failure 2>&1 | ForEach-Object {
        $line = $_
        if ($line -match "\[\s*FAILED\s*\]" -or $line -match "^The following tests FAILED" -or $line -match "FAILED TESTS" -or $line -match "Errors while running CTest" -or $line -match "^\s*0% tests passed") {
            Write-Host $line -ForegroundColor Red
        } elseif ($line -match "\[\s*PASSED\s*\]" -or $line -match "100% tests passed" -or $line -match "All tests passed") {
            Write-Host $line -ForegroundColor Green
        } else {
            Write-Host $line
        }
    }
    return $LASTEXITCODE
}

try {
    if ($TestOnly) {
        Write-Host "`n[步骤] 仅运行测试 (跳过配置与构建)..." -ForegroundColor Cyan
        $exitCode = Invoke-CTestColorized -Configuration "Release"
        if ($exitCode -ne 0) {
            Write-Host "测试执行完成，但有测试失败，退出码: $exitCode" -ForegroundColor Red
        } else {
            Write-Host "所有测试通过!" -ForegroundColor Green
        }
        Write-Host "`n=== 脚本执行完成 ===" -ForegroundColor Green
        Write-Host "结束时间: $(Get-Date)" -ForegroundColor Yellow
        return
    }

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
    $exitCode = Invoke-CTestColorized -Configuration "Release"
    if ($exitCode -ne 0) {
        Write-Host "测试执行完成，但有测试失败，退出码: $exitCode" -ForegroundColor Red
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