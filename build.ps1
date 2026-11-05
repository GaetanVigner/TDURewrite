# Builds tdu.exe (Release) into out\build with Visual Studio's Clang, against the ReXGlue SDK install given by -Sdk.
param(
    [string]$Sdk = 'F:\Crackdown\crackdown-xbox360-recompiled\tools\rexglue\win-amd64',
    [string]$Config = 'Release',
    [string]$BuildDir = 'out\build'
)
$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
$vs = "$(& $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Llvm.Clang -property installationPath)".Trim()
$llvm = Join-Path $vs 'VC\Tools\Llvm\x64\bin'
$cmake = Join-Path $vs 'Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
$ninja = Join-Path $vs 'Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe'

# The MSVC environment (headers, libraries) for clang++.
$envLines = cmd /c "`"$(Join-Path $vs 'Common7\Tools\VsDevCmd.bat')`" -arch=amd64 -host_arch=amd64 >nul && set"
foreach ($line in $envLines) {
    if ($line -match '^([^=]+)=(.*)$') { Set-Item -LiteralPath "Env:$($Matches[1])" -Value $Matches[2] }
}

$build = Join-Path $root $BuildDir
& $cmake -S $root -B $build -G Ninja "-DCMAKE_BUILD_TYPE=$Config" `
    "-DCMAKE_CXX_COMPILER:FILEPATH=$(Join-Path $llvm 'clang++.exe')" `
    "-DCMAKE_C_COMPILER:FILEPATH=$(Join-Path $llvm 'clang.exe')" `
    "-DCMAKE_LINKER:FILEPATH=$(Join-Path $llvm 'lld-link.exe')" `
    "-DCMAKE_MAKE_PROGRAM:FILEPATH=$ninja" "-DCMAKE_PREFIX_PATH=$Sdk"
if ($LASTEXITCODE) { throw "CMake configure failed ($LASTEXITCODE)" }
& $cmake --build $build
if ($LASTEXITCODE) { throw "Build failed ($LASTEXITCODE)" }
# The runtime next to the executable.
Copy-Item (Join-Path $Sdk 'bin\rexruntime.dll'), (Join-Path $Sdk 'bin\rexgpu-xenos.dll') $build -Force
Write-Host "Built $(Join-Path $build 'tdu.exe')"
