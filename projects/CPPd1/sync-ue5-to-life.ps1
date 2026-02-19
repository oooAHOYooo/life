# Auto-Sync CPPd1 from UE5 Project to Life Repo
# This script syncs your UE5 project files to the life repo automatically
#
# Usage:
#   .\sync-ue5-to-life.ps1                    # One-time sync
#   .\sync-ue5-to-life.ps1 -Watch              # Watch for changes and auto-sync
#   .\sync-ue5-to-life.ps1 -Commit            # Sync and auto-commit
#   .\sync-ue5-to-life.ps1 -Watch -Commit     # Watch, sync, and auto-commit

param(
    [switch]$Watch = $false,      # Watch for file changes and auto-sync
    [switch]$Commit = $false,     # Auto-commit changes to git
    [switch]$Push = $false        # Auto-push after commit (requires -Commit)
)

# Configuration - Edit these paths if needed
$Ue5ProjectPath = "C:\Users\agonzalez7\Documents\Unreal Projects\CPPd1"
$LifeRepoPath = "C:\Users\agonzalez7\life"
$DestPath = "$LifeRepoPath\projects\CPPd1"

# Directories to exclude from sync (build artifacts)
$ExcludeDirs = @(
    "Binaries",
    "Intermediate", 
    "Saved",
    "DerivedDataCache",
    ".git",
    "Build",
    ".vs",
    ".vscode"
)

# File patterns to exclude
$ExcludeFiles = @(
    "*.sln",
    "*.suo",
    "*.user",
    "*.opensdf",
    "*.sdf",
    "*.db",
    "*.tmp",
    "*.log"
)

function Sync-Project {
    Write-Host "`n🔄 Syncing CPPd1 from UE5 to Life Repo..." -ForegroundColor Cyan
    Write-Host "   Source: $Ue5ProjectPath" -ForegroundColor Gray
    Write-Host "   Dest:   $DestPath" -ForegroundColor Gray
    Write-Host ""
    
    # Validate paths
    if (-not (Test-Path $Ue5ProjectPath)) {
        Write-Host "❌ UE5 project not found: $Ue5ProjectPath" -ForegroundColor Red
        return $false
    }
    
    if (-not (Test-Path $LifeRepoPath)) {
        Write-Host "❌ Life repo not found: $LifeRepoPath" -ForegroundColor Red
        return $false
    }
    
    if (-not (Test-Path "$LifeRepoPath\.git")) {
        Write-Host "❌ Not a Git repository: $LifeRepoPath" -ForegroundColor Red
        return $false
    }
    
    Push-Location $LifeRepoPath
    
    try {
        # Pull latest changes first
        Write-Host "📥 Pulling latest from life repo..." -ForegroundColor Yellow
        $pullOutput = git pull 2>&1
        if ($LASTEXITCODE -ne 0) {
            Write-Host "⚠️  Git pull had issues (might be normal if no remote): $pullOutput" -ForegroundColor Yellow
        }
        
        # Create destination directory if needed
        if (-not (Test-Path $DestPath)) {
            New-Item -ItemType Directory -Path $DestPath -Force | Out-Null
            Write-Host "✅ Created destination directory" -ForegroundColor Green
        }
        
        # Sync files using robocopy (faster and more reliable than PowerShell Copy-Item)
        Write-Host "📋 Syncing files..." -ForegroundColor Yellow
        
        # Build robocopy command with proper exclusions
        $robocopyArgs = @(
            $Ue5ProjectPath,
            $DestPath,
            "/E",           # Include subdirectories
            "/NFL",         # No file list
            "/NDL",         # No directory list
            "/NP",          # No progress
            "/NJH",         # No job header
            "/NJS"          # No job summary
        )
        
        # Add directory exclusions
        foreach ($dir in $ExcludeDirs) {
            $robocopyArgs += "/XD"
            $robocopyArgs += $dir
        }
        
        # Add file exclusions
        foreach ($file in $ExcludeFiles) {
            $robocopyArgs += "/XF"
            $robocopyArgs += $file
        }
        
        # Run robocopy and capture output
        $robocopyOutput = & robocopy @robocopyArgs 2>&1
        $robocopyExit = $LASTEXITCODE
        
        # Robocopy returns 0-7 for success, 8+ for errors
        if ($robocopyExit -ge 8) {
            Write-Host "❌ Robocopy error (exit code: $robocopyExit)" -ForegroundColor Red
            return $false
        }
        
        # Count files changed (robocopy output format)
        $filesChanged = ($robocopyOutput | Select-String "New File|Updated" | Measure-Object).Count
        if ($filesChanged -eq 0) {
            # Try alternative counting method
            $filesChanged = ($robocopyOutput | Select-String "\d+\s+New File|\d+\s+Updated" | Measure-Object).Count
        }
        
        if ($filesChanged -gt 0) {
            Write-Host "✅ Synced $filesChanged file(s)" -ForegroundColor Green
        } else {
            Write-Host "ℹ️  No changes detected" -ForegroundColor Gray
        }
        
        # Git operations
        Write-Host "📝 Updating Git..." -ForegroundColor Yellow
        git add projects/CPPd1/ 2>&1 | Out-Null
        
        $status = git status --short projects/CPPd1/ 2>&1
        if ([string]::IsNullOrEmpty($status)) {
            Write-Host "ℹ️  No changes to commit" -ForegroundColor Gray
            return $true
        }
        
        if ($Commit) {
            $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm"
            $commitMessage = "Update CPPd1 project - $timestamp"
            
            # Check if there are actual changes (not just untracked files)
            $hasChanges = git diff --cached --quiet projects/CPPd1/; $hasChanges = -not $hasChanges
            $hasUntracked = git ls-files --others --exclude-standard projects/CPPd1/ | Measure-Object | Select-Object -ExpandProperty Count
            
            if ($hasChanges -or $hasUntracked -gt 0) {
                git commit -m $commitMessage 2>&1 | Out-Null
                Write-Host "✅ Committed changes: $commitMessage" -ForegroundColor Green
                
                if ($Push) {
                    Write-Host "📤 Pushing to remote..." -ForegroundColor Yellow
                    $pushOutput = git push 2>&1
                    if ($LASTEXITCODE -eq 0) {
                        Write-Host "✅ Pushed to remote!" -ForegroundColor Green
                    } else {
                        Write-Host "⚠️  Push failed: $pushOutput" -ForegroundColor Yellow
                    }
                }
            }
        } else {
            Write-Host "ℹ️  Changes staged. Run with -Commit to commit, or:" -ForegroundColor Gray
            Write-Host "   git commit -m 'Update CPPd1'" -ForegroundColor Gray
        }
        
        Write-Host "`n✅ Sync complete!" -ForegroundColor Green
        return $true
        
    } catch {
        Write-Host "❌ Error: $_" -ForegroundColor Red
        return $false
    } finally {
        Pop-Location
    }
}

function Watch-And-Sync {
    Write-Host "`n👀 Watching for changes in UE5 project..." -ForegroundColor Cyan
    Write-Host "   Press Ctrl+C to stop" -ForegroundColor Gray
    Write-Host ""
    
    $watcher = New-Object System.IO.FileSystemWatcher
    $watcher.Path = $Ue5ProjectPath
    $watcher.IncludeSubdirectories = $true
    $watcher.EnableRaisingEvents = $true
    
    # Filter for source files and docs
    $watcher.Filter = "*.*"
    
    # Debounce timer (wait 2 seconds after last change before syncing)
    $debounceTimer = $null
    $lastChangeTime = Get-Date
    
    $action = {
        $script:lastChangeTime = Get-Date
        
        # Cancel previous timer
        if ($script:debounceTimer) {
            $script:debounceTimer.Dispose()
        }
        
        # Start new debounce timer
        $script:debounceTimer = New-Object System.Timers.Timer
        $script:debounceTimer.Interval = 2000  # 2 seconds
        $script:debounceTimer.AutoReset = $false
        $script:debounceTimer.Add_Elapsed({
            $script:debounceTimer.Stop()
            $script:debounceTimer.Dispose()
            $script:debounceTimer = $null
            
            Write-Host "`n📝 Change detected, syncing..." -ForegroundColor Yellow
            Sync-Project | Out-Null
            Write-Host "👀 Watching... (Ctrl+C to stop)" -ForegroundColor Gray
        })
        $script:debounceTimer.Start()
    }
    
    # Register events
    Register-ObjectEvent -InputObject $watcher -EventName "Changed" -Action $action | Out-Null
    Register-ObjectEvent -InputObject $watcher -EventName "Created" -Action $action | Out-Null
    Register-ObjectEvent -InputObject $watcher -EventName "Deleted" -Action $action | Out-Null
    Register-ObjectEvent -InputObject $watcher -EventName "Renamed" -Action $action | Out-Null
    
    # Initial sync
    Sync-Project | Out-Null
    Write-Host "👀 Watching... (Ctrl+C to stop)" -ForegroundColor Gray
    
    # Keep script running
    try {
        while ($true) {
            Start-Sleep -Seconds 1
        }
    } finally {
        $watcher.EnableRaisingEvents = $false
        $watcher.Dispose()
        if ($debounceTimer) {
            $debounceTimer.Dispose()
        }
        Write-Host "`n👋 Stopped watching" -ForegroundColor Yellow
    }
}

# Main execution
if ($Watch) {
    Watch-And-Sync
} else {
    $success = Sync-Project
    if (-not $success) {
        exit 1
    }
}
