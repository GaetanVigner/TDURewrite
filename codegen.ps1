# Generates the recompiled C++ (generated\default) from game\default.xex with the ReXGlue codegen.
# game\ is your own copy of the disc (a folder or a junction to it); nothing of the game is in this repository.
param([string]$Sdk = 'F:\Crackdown\crackdown-xbox360-recompiled\tools\rexglue\win-amd64')
$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot
if (-not (Test-Path (Join-Path $root 'game\default.xex'))) {
    throw 'Missing game\default.xex: extract the disc into game\ (or make game\ a junction to it).'
}
Push-Location $root
try {
    & (Join-Path $Sdk 'bin\rexglue.exe') codegen tdu_manifest.toml
    if ($LASTEXITCODE) { throw "Codegen failed ($LASTEXITCODE)" }
} finally {
    Pop-Location
}
