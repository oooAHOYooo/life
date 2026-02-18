# Syncing CPPd1 with Your Life Repo

## 📍 Where Your .md Files Are

All documentation files are in the **root** of your CPPd1 project:
```
C:\Users\agonzalez7\Documents\Unreal Projects\CPPd1\
├── NEW_C++_FEATURES.md
├── QUICK_START_GUIDE.md
├── GIT_SETUP.md
├── WAVE_SYSTEM_GUIDE.md
├── QUICK_SETUP_GUIDE.md
├── GAMEMODE_SETUP.md
├── TUNING_VARIABLES_REFERENCE.md
├── ADVANCED_SYSTEMS_GUIDE.md
├── SETUP_GUIDE.md
└── SYNC_WITH_LIFE_REPO.md (this file)
```

---

## 🔄 Sync Process - Choose Your Method

### Method 1: Add CPPd1 as Subdirectory to Life Repo (Recommended)

**Best for:** Keeping all projects organized in one repo

```powershell
# Step 1: Navigate to your life repo
cd "C:\path\to\your\life\repo"

# Step 2: Copy the entire CPPd1 folder into your life repo
# (Or move it if you want it there permanently)
# Example: Copy-Item "C:\Users\agonzalez7\Documents\Unreal Projects\CPPd1" -Destination ".\projects\CPPd1" -Recurse

# Step 3: Add to Git
git add projects/CPPd1/  # or wherever you put it
git commit -m "Add CPPd1 ninja game project"
git push
```

**Result:** Your life repo now contains CPPd1 as a subdirectory.

---

### Method 2: Initialize Git Here and Push to Life Repo

**Best for:** Keeping CPPd1 separate but synced

```powershell
# Step 1: Navigate to CPPd1 project
cd "C:\Users\agonzalez7\Documents\Unreal Projects\CPPd1"

# Step 2: Initialize Git (if not already)
git init

# Step 3: Add all files (respects .gitignore)
git add .

# Step 4: First commit
git commit -m "Initial commit: CPPd1 ninja game with all systems"

# Step 5: Add your life repo as remote
# Replace <LIFE_REPO_URL> with your actual repo URL
git remote add origin <LIFE_REPO_URL>

# Step 6: Push to life repo
git push -u origin main
# Or if your life repo uses 'master':
# git push -u origin master
```

**Result:** CPPd1 is now a branch/folder in your life repo.

---

### Method 3: Use Git Submodule (Advanced)

**Best for:** Keeping projects separate but linked

```powershell
# Step 1: Initialize Git in CPPd1
cd "C:\Users\agonzalez7\Documents\Unreal Projects\CPPd1"
git init
git add .
git commit -m "Initial commit"

# Step 2: In your life repo, add as submodule
cd "C:\path\to\your\life\repo"
git submodule add "C:\Users\agonzalez7\Documents\Unreal Projects\CPPd1" projects/CPPd1
git commit -m "Add CPPd1 as submodule"
git push
```

**Result:** Life repo references CPPd1 as a submodule (keeps them separate but linked).

---

## 🎯 Recommended: Method 1 (Subdirectory)

**Why:**
- ✅ Simplest to manage
- ✅ All code in one place
- ✅ Easy to navigate
- ✅ Single repo to backup

**Steps:**
1. Find your life repo location
2. Create a `projects/` or `games/` folder in it
3. Copy CPPd1 into that folder
4. `git add`, `git commit`, `git push`

---

## 📋 Quick Sync Script

Save this as `sync-to-life.ps1` in your CPPd1 folder:

```powershell
# Configure this path
$LifeRepoPath = "C:\path\to\your\life\repo"

# Navigate to life repo
cd $LifeRepoPath

# Pull latest changes
git pull

# Copy CPPd1 (adjust source path if needed)
$SourcePath = "C:\Users\agonzalez7\Documents\Unreal Projects\CPPd1"
$DestPath = "$LifeRepoPath\projects\CPPd1"

# Remove old copy
if (Test-Path $DestPath) {
    Remove-Item $DestPath -Recurse -Force
}

# Copy new version
Copy-Item $SourcePath -Destination $DestPath -Recurse -Exclude @(".git", "Binaries", "Intermediate", "Saved", "DerivedDataCache")

# Add and commit
cd $LifeRepoPath
git add projects/CPPd1/
git commit -m "Update CPPd1 project - $(Get-Date -Format 'yyyy-MM-dd HH:mm')"
git push

Write-Host "✅ Synced to life repo!"
```

---

## 🔍 Find Your Life Repo

If you're not sure where your life repo is, run:

```powershell
# Search for .git folders (repos)
Get-ChildItem -Path "C:\Users\agonzalez7" -Filter ".git" -Directory -Recurse -ErrorAction SilentlyContinue | Select-Object FullName
```

Or tell me the path and I'll help you set it up!

---

## ⚠️ Important Notes

1. **.gitignore is already set up** - It excludes:
   - `Binaries/`, `Intermediate/`, `Saved/` (build files)
   - Visual Studio files
   - Keeps all source code (`.cpp`, `.h`)

2. **Multi-computer workflow:**
   - Pull from life repo
   - Right-click `.uproject` → Generate Visual Studio files
   - Compile in UE5

3. **Don't commit:**
   - Large binary files (already in .gitignore)
   - Personal API keys
   - Temporary files

---

## 🚀 Quick Start

**Tell me your life repo path and I'll create a custom sync script for you!**

Or use Method 1 manually:
1. Copy CPPd1 folder into your life repo
2. `git add projects/CPPd1/`
3. `git commit -m "Add CPPd1 project"`
4. `git push`

Done! 🎉
