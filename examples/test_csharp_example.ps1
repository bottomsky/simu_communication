# C# 示例测试脚本
# 用于编译、复制动态库并运行 C# 示例项目

param(
    [string]$Configuration = "Debug",
    [switch]$Clean = $false,
    [switch]$Verbose = $false
)

# 设置错误处理
$ErrorActionPreference = "Stop"

# 定义路径
$ProjectRoot = Split-Path -Parent $PSScriptRoot
$CSharpSolutionPath = Join-Path $PSScriptRoot "csharp\CommunicationModelExample.sln"
$CSharpProjectPath = Join-Path $PSScriptRoot "csharp\CommunicationModelExample.csproj"
$CSharpOutputPath = Join-Path $PSScriptRoot "csharp\bin\$Configuration\net6.0"

# 颜色输出函数
function Write-ColorOutput {
    param(
        [string]$Message,
        [string]$Color = "White"
    )
    Write-Host $Message -ForegroundColor $Color
}

function Write-Step {
    param([string]$Message)
    Write-ColorOutput "==> $Message" "Cyan"
}

function Write-Success {
    param([string]$Message)
    Write-ColorOutput "✓ $Message" "Green"
}

function Write-Error {
    param([string]$Message)
    Write-ColorOutput "✗ $Message" "Red"
}

function Write-Warning {
    param([string]$Message)
    Write-ColorOutput "⚠ $Message" "Yellow"
}

# 检查必要工具
function Test-Prerequisites {
    Write-Step "检查必要工具..."
    
    # 检查 dotnet CLI
    try {
        $dotnetVersion = dotnet --version
        Write-Success "找到 .NET CLI 版本: $dotnetVersion"
    }
    catch {
        Write-Error ".NET CLI 未找到，请安装 .NET 6.0 SDK"
        exit 1
    }
    
    # 检查 MSBuild (可选，dotnet CLI 已包含)
    try {
        $msbuildPath = Get-Command "MSBuild.exe" -ErrorAction SilentlyContinue
        if ($msbuildPath) {
            Write-Success "找到 MSBuild: $($msbuildPath.Source)"
        }
    }
    catch {
        Write-Warning "MSBuild 未找到，将使用 dotnet CLI 进行构建"
    }
}

# 检查文件存在性
function Test-Files {
    Write-Step "检查项目文件..."
    
    if (-not (Test-Path $CSharpSolutionPath)) {
        Write-Error "C# 解决方案文件未找到: $CSharpSolutionPath"
        exit 1
    }
    Write-Success "找到 C# 解决方案: $CSharpSolutionPath"
    
    if (-not (Test-Path $CSharpProjectPath)) {
        Write-Error "C# 项目文件未找到: $CSharpProjectPath"
        exit 1
    }
    Write-Success "找到 C# 项目: $CSharpProjectPath"
}

# 清理输出目录
function Clear-Output {
    if ($Clean) {
        Write-Step "清理输出目录..."
        
        $csharpBinPath = Join-Path $PSScriptRoot "csharp\bin"
        $csharpObjPath = Join-Path $PSScriptRoot "csharp\obj"
        
        if (Test-Path $csharpBinPath) {
            Remove-Item $csharpBinPath -Recurse -Force
            Write-Success "已清理: $csharpBinPath"
        }
        
        if (Test-Path $csharpObjPath) {
            Remove-Item $csharpObjPath -Recurse -Force
            Write-Success "已清理: $csharpObjPath"
        }
    }
}

# 编译 C# 项目
function Build-CSharpProject {
    Write-Step "编译 C# 项目..."
    
    try {
        $verbosityLevel = if ($Verbose) { "normal" } else { "minimal" }
        $buildArgs = @(
            "build"
            $CSharpSolutionPath
            "--configuration", $Configuration
            "--verbosity", $verbosityLevel
        )
        
        if ($Verbose) {
            Write-ColorOutput "执行命令: dotnet $($buildArgs -join ' ')" "Gray"
        }
        
        & dotnet @buildArgs
        
        if ($LASTEXITCODE -eq 0) {
            Write-Success "C# 项目编译成功"
        } else {
            Write-Error "C# 项目编译失败，退出码: $LASTEXITCODE"
            exit $LASTEXITCODE
        }
    }
    catch {
        Write-Error "编译过程中发生异常: $($_.Exception.Message)"
        exit 1
    }
}

# 复制动态库
function Copy-DynamicLibraries {
    Write-Step "复制动态库到 C# 输出目录..."
    
    # 确保输出目录存在
    if (-not (Test-Path $CSharpOutputPath)) {
        New-Item -Path $CSharpOutputPath -ItemType Directory -Force | Out-Null
        Write-Success "创建输出目录: $CSharpOutputPath"
    }
    
    # 定义可能的构建输出路径
    $possibleBuildPaths = @(
        (Join-Path $ProjectRoot "build\bin\$Configuration"),
        (Join-Path $ProjectRoot "build\lib\$Configuration"),
        (Join-Path $ProjectRoot "build\bin"),
        (Join-Path $ProjectRoot "build\lib"),
        (Join-Path $ProjectRoot "build\$Configuration\bin"),
        (Join-Path $ProjectRoot "build\$Configuration\lib")
    )
    
    # 主要需要的动态库（C# 调用的核心库）
    $primaryLibrary = "CommunicationModelCAPI.dll"
    
    # 可能的依赖库
    $dependencyLibraries = @(
        "CommunicationModel.dll",
        "CommunicationModelShared.dll"
    )
    
    $copiedCount = 0
    $primaryLibraryFound = $false
    
    # 首先查找主要的 C API 动态库
    Write-ColorOutput "查找主要动态库: $primaryLibrary" "Gray"
    
    foreach ($buildPath in $possibleBuildPaths) {
        $sourcePath = Join-Path $buildPath $primaryLibrary
        
        if (Test-Path $sourcePath) {
            $destPath = Join-Path $CSharpOutputPath $primaryLibrary
            
            try {
                Copy-Item $sourcePath $destPath -Force
                Write-Success "已复制主要库: $primaryLibrary (从 $buildPath)"
                $copiedCount++
                $primaryLibraryFound = $true
                
                # 在同一目录下查找依赖库
                foreach ($depLib in $dependencyLibraries) {
                    $depSourcePath = Join-Path $buildPath $depLib
                    $depDestPath = Join-Path $CSharpOutputPath $depLib
                    
                    if (Test-Path $depSourcePath) {
                        try {
                            Copy-Item $depSourcePath $depDestPath -Force
                            Write-Success "已复制依赖库: $depLib"
                            $copiedCount++
                        }
                        catch {
                            Write-Warning "复制依赖库失败: $depLib - $($_.Exception.Message)"
                        }
                    }
                }
                
                break  # 找到主要库后退出循环
            }
            catch {
                Write-Warning "复制主要库失败: $primaryLibrary - $($_.Exception.Message)"
            }
        }
    }
    
    if (-not $primaryLibraryFound) {
        Write-Error "未找到主要动态库: $primaryLibrary"
        Write-Warning "请确保已经构建了 C++ 项目"
        
        # 列出 build 目录下的文件以帮助调试
        Write-ColorOutput "搜索的路径:" "Gray"
        foreach ($path in $possibleBuildPaths) {
            if (Test-Path $path) {
                Write-ColorOutput "  ✓ $path" "Gray"
                Get-ChildItem $path -File -Filter "*.dll" | 
                    ForEach-Object { Write-ColorOutput "    - $($_.Name)" "Gray" }
            } else {
                Write-ColorOutput "  ✗ $path (不存在)" "Gray"
            }
        }
        
        # 递归搜索整个 build 目录
        $buildDir = Join-Path $ProjectRoot "build"
        if (Test-Path $buildDir) {
            Write-ColorOutput "build 目录下的所有 DLL 文件:" "Gray"
            Get-ChildItem $buildDir -Recurse -File -Filter "*.dll" | 
                ForEach-Object { Write-ColorOutput "  $($_.FullName)" "Gray" }
        }
        
        exit 1
    }
    
    Write-Success "共复制了 $copiedCount 个动态库文件"
}

# 运行 C# 项目
function Run-CSharpProject {
    Write-Step "运行 C# 示例项目..."
    
    try {
        $runArgs = @(
            "run"
            "--project", $CSharpProjectPath
            "--configuration", $Configuration
            "--no-build"  # 不重新构建，使用已编译的版本
        )
        
        if ($Verbose) {
            Write-ColorOutput "执行命令: dotnet $($runArgs -join ' ')" "Gray"
        }
        
        Write-ColorOutput "--- C# 程序输出开始 ---" "Yellow"
        & dotnet @runArgs
        $exitCode = $LASTEXITCODE
        Write-ColorOutput "--- C# 程序输出结束 ---" "Yellow"
        
        if ($exitCode -eq 0) {
            Write-Success "C# 示例程序运行成功"
        } else {
            Write-Error "C# 示例程序运行失败，退出码: $exitCode"
            exit $exitCode
        }
    }
    catch {
        Write-Error "运行过程中发生异常: $($_.Exception.Message)"
        exit 1
    }
}

# 显示帮助信息
function Show-Help {
    Write-ColorOutput @"
C# 示例测试脚本

用法:
    .\test_csharp_example.ps1 [-Configuration <Debug|Release>] [-Clean] [-Verbose]

参数:
    -Configuration  构建配置 (Debug 或 Release，默认: Debug)
    -Clean          清理输出目录后再构建
    -Verbose        显示详细输出

示例:
    .\test_csharp_example.ps1                          # 使用默认设置
    .\test_csharp_example.ps1 -Configuration Release   # 使用 Release 配置
    .\test_csharp_example.ps1 -Clean -Verbose          # 清理并显示详细输出

注意:
    运行此脚本前，请确保已经构建了 C++ 项目，生成了必要的动态库文件。
"@ "Cyan"
}

# 主函数
function Main {
    Write-ColorOutput "=== C# 示例测试脚本 ===" "Magenta"
    Write-ColorOutput "配置: $Configuration" "Gray"
    Write-ColorOutput "项目根目录: $ProjectRoot" "Gray"
    Write-ColorOutput ""
    
    try {
        Test-Prerequisites
        Test-Files
        Clear-Output
        Build-CSharpProject
        Copy-DynamicLibraries
        Run-CSharpProject
        
        Write-ColorOutput ""
        Write-Success "所有步骤完成！C# 示例测试成功。"
    }
    catch {
        Write-Error "脚本执行失败: $($_.Exception.Message)"
        exit 1
    }
}

# 检查是否请求帮助
if ($args -contains "-h" -or $args -contains "--help" -or $args -contains "/?") {
    Show-Help
    exit 0
}

# 执行主函数
Main