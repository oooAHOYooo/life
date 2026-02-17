# Sync P2G4W scripts from life repo to Unreal Engine 5 project
# Usage: .\sync-to-ue5.ps1 -Ue5ProjectPath "C:\Users\agonzalez7\Documents\Unreal Projects\CoPlago"

param(
    [Parameter(Mandatory=$false)]
    [string]$Ue5ProjectPath = "C:\Users\agonzalez7\Documents\Unreal Projects\CoPlago",
    
    [Parameter(Mandatory=$false)]
    [string]$SourceModuleName = "P2G4W",
    
    [Parameter(Mandatory=$false)]
    [string]$TargetModuleName = "CoPlago"
)

$ErrorActionPreference = "Stop"

# Get the script directory (where this script lives)
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$SourceDir = $ScriptDir

# Validate paths
if (-not (Test-Path $SourceDir)) {
    Write-Error "Source directory not found: $SourceDir"
    exit 1
}

if (-not (Test-Path $Ue5ProjectPath)) {
    Write-Error "UE5 project path not found: $Ue5ProjectPath"
    exit 1
}

$Ue5SourcePath = Join-Path $Ue5ProjectPath "Source\$TargetModuleName"
$Ue5PublicPath = Join-Path $Ue5SourcePath "Public"
$Ue5PrivatePath = Join-Path $Ue5SourcePath "Private"

# Create directories if they don't exist
if (-not (Test-Path $Ue5PublicPath)) {
    New-Item -ItemType Directory -Path $Ue5PublicPath -Force | Out-Null
    Write-Host "Created directory: $Ue5PublicPath" -ForegroundColor Green
}

if (-not (Test-Path $Ue5PrivatePath)) {
    New-Item -ItemType Directory -Path $Ue5PrivatePath -Force | Out-Null
    Write-Host "Created directory: $Ue5PrivatePath" -ForegroundColor Green
}

# File mapping: source file -> destination (Public or Private)
$FileMappings = @{
    "$SourceModuleName.h" = "Public"
    "$SourceModuleName.cpp" = "Private"
    "$SourceModuleName`GameModeBase.h" = "Public"
    "$SourceModuleName`GameModeBase.cpp" = "Private"
    "$SourceModuleName`GameStateBase.h" = "Public"
    "$SourceModuleName`GameStateBase.cpp" = "Private"
    "$SourceModuleName`PlayerState.h" = "Public"
    "$SourceModuleName`PlayerState.cpp" = "Private"
    "$SourceModuleName`PlayerController.h" = "Public"
    "$SourceModuleName`PlayerController.cpp" = "Private"
    "$SourceModuleName`Character.h" = "Public"
    "$SourceModuleName`Character.cpp" = "Private"
    "$SourceModuleName`GoalZone.h" = "Public"
    "$SourceModuleName`GoalZone.cpp" = "Private"
    "$SourceModuleName`LockOnTargetComponent.h" = "Public"
    "$SourceModuleName`LockOnTargetComponent.cpp" = "Private"
    "$SourceModuleName`Enemy.h" = "Public"
    "$SourceModuleName`Enemy.cpp" = "Private"
    "$SourceModuleName`EnemyWaveSpawner.h" = "Public"
    "$SourceModuleName`EnemyWaveSpawner.cpp" = "Private"
}

Write-Host "`n=== Syncing files from life repo to UE5 project ===" -ForegroundColor Cyan
Write-Host "Source: $SourceDir" -ForegroundColor Gray
Write-Host "Destination: $Ue5SourcePath" -ForegroundColor Gray
Write-Host "Replacing module name: $SourceModuleName -> $TargetModuleName`n" -ForegroundColor Gray

$FilesSynced = 0
$FilesSkipped = 0

foreach ($fileName in $FileMappings.Keys) {
    $sourceFile = Join-Path $SourceDir $fileName
    
    if (-not (Test-Path $sourceFile)) {
        Write-Host "⚠️  Source file not found: $fileName" -ForegroundColor Yellow
        $FilesSkipped++
        continue
    }
    
    $destinationFolder = $FileMappings[$fileName]
    $destPath = if ($destinationFolder -eq "Public") { $Ue5PublicPath } else { $Ue5PrivatePath }
    $destFile = Join-Path $destPath $fileName
    
    # Replace module name in filename
    $destFileName = $fileName -replace $SourceModuleName, $TargetModuleName
    $destFile = Join-Path $destPath $destFileName
    
    # Read source file content
    $content = Get-Content $sourceFile -Raw -Encoding UTF8
    
    # Replace module name in content
    $content = $content -replace $SourceModuleName, $TargetModuleName
    
    # Write to destination
    $content | Set-Content -Path $destFile -Encoding UTF8 -NoNewline
    
    Write-Host "✓ Synced: $fileName -> $destFileName ($destinationFolder)" -ForegroundColor Green
    $FilesSynced++
}

Write-Host "`n=== Sync Complete ===" -ForegroundColor Cyan
Write-Host "Files synced: $FilesSynced" -ForegroundColor Green
if ($FilesSkipped -gt 0) {
    Write-Host "Files skipped: $FilesSkipped" -ForegroundColor Yellow
}

Write-Host "`nNext steps:" -ForegroundColor Cyan
Write-Host "1. Open your UE5 project" -ForegroundColor White
Write-Host "2. Right-click the .uproject file and select 'Generate Visual Studio project files' (if needed)" -ForegroundColor White
Write-Host "3. Build the project in UE5 (Ctrl+Alt+B) or use: UnrealEditor.exe -project=`"$Ue5ProjectPath\CoPlago.uproject`" -run=Compile" -ForegroundColor White
Write-Host "4. The files will be compiled and hot-reloaded in the editor" -ForegroundColor White
