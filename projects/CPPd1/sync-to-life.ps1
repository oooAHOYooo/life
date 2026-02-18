# Sync CPPd1 Project to Life Repo
# Edit the path below to point to your life repo

param(
    [string]$LifeRepoPath = ""
)

# If no path provided, try to find it
if ([string]::IsNullOrEmpty($LifeRepoPath)) {
    Write-Host "⚠️  No life repo path provided!"
    Write-Host "Usage: .\sync-to-life.ps1 -LifeRepoPath 'C:\path\to\your\life\repo'"
    Write-Host ""
    Write-Host "Or edit this script and set the path below:"
    exit 1
}

# Verify life repo exists
if (-not (Test-Path $LifeRepoPath)) {
    Write-Host "❌ Life repo path not found: $LifeRepoPath"
    exit 1
}

if (-not (Test-Path "$LifeRepoPath\.git")) {
    Write-Host "❌ Not a Git repository: $LifeRepoPath"
    Write-Host "   Make sure this is your life repo root folder"
    exit 1
}

Write-Host "🔄 Syncing CPPd1 to life repo..."
Write-Host "   Life Repo: $LifeRepoPath"
Write-Host ""

# Source path (this project)
$SourcePath = "C:\Users\agonzalez7\Documents\Unreal Projects\CPPd1"
$DestPath = "$LifeRepoPath\projects\CPPd1"

# Navigate to life repo
Push-Location $LifeRepoPath

try {
    # Pull latest changes
    Write-Host "📥 Pulling latest changes from life repo..."
    git pull

    # Create projects folder if it doesn't exist
    if (-not (Test-Path "projects")) {
        New-Item -ItemType Directory -Path "projects" | Out-Null
        Write-Host "✅ Created projects/ folder"
    }

    # Remove old copy if exists
    if (Test-Path $DestPath) {
        Write-Host "🗑️  Removing old CPPd1 copy..."
        Remove-Item $DestPath -Recurse -Force
    }

    # Copy project (exclude build files)
    Write-Host "📋 Copying CPPd1 project..."
    $ExcludeDirs = @("Binaries", "Intermediate", "Saved", "DerivedDataCache", ".git", "Build")
    $ExcludeFiles = @("*.sln", "*.suo", "*.user", "*.opensdf", "*.sdf")
    
    # Copy everything except excluded
    Get-ChildItem -Path $SourcePath -Recurse | Where-Object {
        $item = $_
        $relativePath = $item.FullName.Substring($SourcePath.Length + 1)
        $shouldExclude = $false
        
        foreach ($exclude in $ExcludeDirs) {
            if ($relativePath -like "$exclude*") {
                $shouldExclude = $true
                break
            }
        }
        
        if (-not $shouldExclude) {
            foreach ($pattern in $ExcludeFiles) {
                if ($item.Name -like $pattern) {
                    $shouldExclude = $true
                    break
                }
            }
        }
        
        -not $shouldExclude
    } | ForEach-Object {
        $destFile = $_.FullName.Replace($SourcePath, $DestPath)
        $destDir = Split-Path $destFile -Parent
        
        if (-not (Test-Path $destDir)) {
            New-Item -ItemType Directory -Path $destDir -Force | Out-Null
        }
        
        Copy-Item $_.FullName -Destination $destFile -Force
    }

    Write-Host "✅ Copy complete!"

    # Add to Git
    Write-Host "📝 Adding to Git..."
    git add projects/CPPd1/

    # Check if there are changes
    $status = git status --short projects/CPPd1/
    if ([string]::IsNullOrEmpty($status)) {
        Write-Host "ℹ️  No changes to commit"
    } else {
        $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm"
        git commit -m "Update CPPd1 project - $timestamp"
        Write-Host "✅ Committed changes"
        
        # Ask to push
        $push = Read-Host "Push to remote? (y/n)"
        if ($push -eq "y" -or $push -eq "Y") {
            git push
            Write-Host "✅ Pushed to remote!"
        } else {
            Write-Host "ℹ️  Changes committed locally. Push manually with: git push"
        }
    }

    Write-Host ""
    Write-Host "✅ Sync complete!"
    Write-Host "   CPPd1 is now in: $DestPath"

} catch {
    Write-Host "❌ Error: $_"
    exit 1
} finally {
    Pop-Location
}
