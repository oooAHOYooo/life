# Build CPPd1 from the life repo so UE5 picks up C++ changes.
# Run from repo root or from projects/CPPd1. Uses UE5 install path from env.
#
# Usage:
#   $env:UE5_ROOT = "C:\Program Files\Epic Games\UE_5.6"
#   .\projects\CPPd1\Scripts\build-cppd1.ps1
#
# Or one-shot (PowerShell):
#   $env:UE5_ROOT = "C:\Program Files\Epic Games\UE_5.6"; & "$PSScriptRoot\build-cppd1.ps1"

param(
    [string]$Configuration = "Development",
    [string]$Platform = "Win64"
)

$ErrorActionPreference = "Stop"

# Resolve project root (life repo root)
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$ProjectDir = Resolve-Path (Join-Path $ScriptDir "..")
$RepoRoot = Resolve-Path (Join-Path $ProjectDir "..\..")
$UProject = Join-Path $ProjectDir "CPPd1.uproject"

if (-not (Test-Path $UProject)) {
    Write-Error "UProject not found: $UProject"
    exit 1
}

$UE = $env:UE5_ROOT
if (-not $UE) { $UE = $env:UE_ROOT }
if (-not $UE) {
    # Common default
    $UE = "C:\Program Files\Epic Games\UE_5.6"
}
$BuildBat = Join-Path $UE "Engine\Build\BatchFiles\Build.bat"
if (-not (Test-Path $BuildBat)) {
    Write-Host "Unreal Build Tool not found at: $BuildBat"
    Write-Host "Set UE5_ROOT (or UE_ROOT) to your UE5 Engine folder, e.g.:"
    Write-Host '  $env:UE5_ROOT = "C:\Program Files\Epic Games\UE_5.6"'
    exit 1
}

Write-Host "Building CPPd1Editor $Platform $Configuration"
Write-Host "  Project: $UProject"
Write-Host "  UE:      $UE"
Write-Host ""

& $BuildBat CPPd1Editor $Platform $Configuration "`"$UProject`"" -waitmutex
$exitCode = $LASTEXITCODE
if ($exitCode -eq 0) {
    Write-Host ""
    Write-Host "Build succeeded. If UE5 is open with this project, Live Coding will hot-reload."
} else {
    Write-Host ""
    Write-Host "Build failed with exit code $exitCode"
}
exit $exitCode
