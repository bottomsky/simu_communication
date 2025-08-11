param(
    [Alias('c')]
    [switch]$Clean = $false,
    
    [ValidateSet('Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel')]
    [string]$Config = 'Debug',
    
    [int]$Jobs = 0
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest

# 日志输出函数
function Write-InfoLog($Message) { Write-Host "[Info] $Message" -ForegroundColor Cyan }
function Write-SuccessLog($Message) { Write-Host "[Success] $Message" -ForegroundColor Green }
function Write-WarningLog($Message) { Write-Host "[Warning] $Message" -ForegroundColor Yellow }
function Write-ErrorLog($Message) { Write-Host "[Error] $Message" -ForegroundColor Red }
function Write-StepLog($Message) { Write-Host "[Step] $Message" -ForegroundColor Magenta }

# Resolve paths
$RootDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$BuildDir = Join-Path $RootDir 'build'
$CMakeListsPath = Join-Path $BuildDir 'CMakeLists.txt'

Write-InfoLog "RootDir: $RootDir"
Write-InfoLog "BuildDir: $BuildDir"
Write-InfoLog "使用配置: $Config"
if ($Jobs -gt 0) { Write-InfoLog "并行编译作业数: $Jobs" }

# 验证路径和依赖
try {
    if (-not (Test-Path -LiteralPath $BuildDir)) {
        Write-ErrorLog "Build 目录不存在: $BuildDir"
        throw "Build 目录验证失败"
    }

    if (-not (Test-Path -LiteralPath $CMakeListsPath)) {
        Write-ErrorLog "未找到 $CMakeListsPath"
        throw "CMakeLists.txt 验证失败，请确认 build 目录下存在 CMakeLists.txt"
    }
    
    # 检查 cmake 是否可用
    try {
        $cmakeVersion = & cmake --version 2>&1
        if ($LASTEXITCODE -ne 0) {
            Write-ErrorLog "CMake 不可用或未安装"
            throw "CMake 依赖检查失败"
        }
        Write-InfoLog "CMake 版本检查通过"
    }
    catch {
        Write-ErrorLog "无法执行 cmake 命令: $($_.Exception.Message)"
        throw "CMake 依赖检查失败"
    }
}
catch {
    Write-ErrorLog "初始化验证失败: $($_.Exception.Message)"
    exit 1
}

# Optional clean: delete everything under build except CMakeLists.txt at build root
if ($Clean) {
    Write-StepLog "正在清理 $BuildDir 下的文件和目录（保留 CMakeLists.txt）..."
    try {
        $entries = Get-ChildItem -LiteralPath $BuildDir -Force
        $toDelete = $entries | Where-Object { $_.Name -ne 'CMakeLists.txt' }
        foreach ($entry in $toDelete) {
            try {
                Remove-Item -LiteralPath $entry.FullName -Recurse -Force -ErrorAction Stop
                Write-WarningLog ("删除: {0}" -f $entry.FullName)
            }
            catch {
                Write-ErrorLog ("无法删除 {0} - {1}" -f $entry.FullName, $_.Exception.Message)
                throw
            }
        }
        Write-SuccessLog "清理完成。"
    }
    catch {
        Write-ErrorLog ("清理阶段失败: {0}" -f $_.Exception.Message)
        throw
    }
}
else {
    Write-InfoLog "跳过清理（默认）。如需清理请加 -Clean 或 -c 参数。"
}

# Configure using the CMakeLists.txt located in build directory
Write-StepLog "配置阶段：cmake -S $BuildDir -B $BuildDir (Config: $Config)"
# 尝试在配置阶段同时传入 CMAKE_BUILD_TYPE，适配单配置生成器（如 Ninja/Makefile）；
# 对于多配置生成器（如 VS）该变量可被忽略。
$configureArgs = @("-S", $BuildDir, "-B", $BuildDir, "-DCMAKE_BUILD_TYPE=$Config")
try {
    $configure = Start-Process -FilePath "cmake" -ArgumentList $configureArgs -NoNewWindow -PassThru -Wait
    if ($configure.ExitCode -ne 0) {
        Write-ErrorLog "CMake 配置失败，退出码: $($configure.ExitCode)"
        throw "CMake 配置阶段失败"
    }
    Write-SuccessLog "配置完成。"
}
catch {
    Write-ErrorLog "配置阶段异常: $($_.Exception.Message)"
    exit 1
}

# Build. Use provided $Config for multi-config generators; -j for parallel if specified
$parallelText = if ($Jobs -gt 0) { " -j $Jobs" } else { "" }
Write-StepLog "构建阶段：cmake --build $BuildDir --config $Config$parallelText"
$buildArgs = @("--build", $BuildDir, "--config", $Config)
if ($Jobs -gt 0) { $buildArgs += @("-j", "$Jobs") }
try {
    $build = Start-Process -FilePath "cmake" -ArgumentList $buildArgs -NoNewWindow -PassThru -Wait
    if ($build.ExitCode -ne 0) {
        Write-ErrorLog "CMake 构建失败，退出码: $($build.ExitCode)"
        throw "CMake 构建阶段失败"
    }
    Write-SuccessLog "构建完成。"
}
catch {
    Write-ErrorLog "构建阶段异常: $($_.Exception.Message)"
    exit 1
}