param(
    [switch]$Clean = $false
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest

# Resolve paths
$RootDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$BuildDir = Join-Path $RootDir 'build'
$CMakeListsPath = Join-Path $BuildDir 'CMakeLists.txt'

Write-Host "[Info] RootDir: $RootDir" -ForegroundColor Cyan
Write-Host "[Info] BuildDir: $BuildDir" -ForegroundColor Cyan

if (-not (Test-Path -LiteralPath $BuildDir)) {
    throw "Build 目录不存在: $BuildDir"
}

if (-not (Test-Path -LiteralPath $CMakeListsPath)) {
    throw "未找到 $CMakeListsPath。请确认 build 目录下存在 CMakeLists.txt"
}

# Optional clean: delete everything under build except CMakeLists.txt at build root
if ($Clean) {
    Write-Host "[Clean] 正在清理 $BuildDir 下的文件和目录（保留 CMakeLists.txt）..." -ForegroundColor Yellow
    $entries = Get-ChildItem -LiteralPath $BuildDir -Force
    $toDelete = $entries | Where-Object { $_.Name -ne 'CMakeLists.txt' }
    foreach ($entry in $toDelete) {
        try {
            Remove-Item -LiteralPath $entry.FullName -Recurse -Force -ErrorAction Stop
            Write-Host ("  删除: {0}" -f $entry.FullName) -ForegroundColor DarkYellow
        }
        catch {
            Write-Host ("  警告: 无法删除 {0} - {1}" -f $entry.FullName, $_.Exception.Message) -ForegroundColor Red
            throw
        }
    }
    Write-Host "[Clean] 清理完成。" -ForegroundColor Green
}
else {
    Write-Host "[Info] 跳过清理（默认）。如需清理请加 -Clean 参数。" -ForegroundColor DarkCyan
}

# Configure using the CMakeLists.txt located in build directory
Write-Host "[CMake] 配置阶段：cmake -S $BuildDir -B $BuildDir" -ForegroundColor Cyan
$configure = Start-Process -FilePath "cmake" -ArgumentList @("-S", $BuildDir, "-B", $BuildDir) -NoNewWindow -PassThru -Wait
if ($configure.ExitCode -ne 0) {
    throw "CMake 配置失败，退出码: $($configure.ExitCode)"
}

# Build. For multi-config generators (e.g., Visual Studio), default to Debug.
Write-Host "[CMake] 构建阶段：cmake --build $BuildDir --config Debug" -ForegroundColor Cyan
$build = Start-Process -FilePath "cmake" -ArgumentList @("--build", $BuildDir, "--config", "Debug") -NoNewWindow -PassThru -Wait
if ($build.ExitCode -ne 0) {
    throw "CMake 构建失败，退出码: $($build.ExitCode)"
}

Write-Host "[Done] 构建完成。" -ForegroundColor Green