# CPPd1 - Ninja Game Project

## 🎮 Project Overview

A 2-player ninja combat game built in Unreal Engine 5.6 with:
- Wave-based enemy spawning (3, 5, 7 enemies per wave)
- Sequential enemy engagement (UFC-style, one at a time)
- Vertical split-screen (always on, even for solo play)
- Xbox controller support
- Advanced combat systems (tricks, skills, stamina, flow)

## 📁 Project Location

**Source Location:** `C:\Users\agonzalez7\Documents\Unreal Projects\CPPd1`  
**Synced Location:** `C:\Users\agonzalez7\life\projects\CPPd1`

## 🚀 Quick Start

1. **Open Project:**
   - Open `CPPd1.uproject` in Unreal Engine 5.6
   - Right-click → Generate Visual Studio project files (if needed)

2. **Set Game Mode:**
   - Edit → Project Settings → Game → Game Mode → `CombatGameMode`

3. **Add Wave Spawner:**
   - Place `CombatWaveSpawner` actor in your level
   - Configure waves in Details panel

4. **Press Play!**

## 📚 Documentation

All guides are in the project root:
- `QUICK_START_GUIDE.md` - Testing and Xbox deployment
- `NEW_C++_FEATURES.md` - Latest C++ additions
- `WAVE_SYSTEM_GUIDE.md` - Wave spawning setup
- `TUNING_VARIABLES_REFERENCE.md` - All exposed variables
- `SYNC_WITH_LIFE_REPO.md` - Git sync instructions

## 💻 Multi-Computer Workflow

### 🚀 **Automatic Sync** (Recommended)

After making changes in UE5, sync to life repo automatically:

```powershell
# From life repo's CPPd1 folder
cd "C:\Users\agonzalez7\life\projects\CPPd1"

# One-time sync with auto-commit
.\sync-ue5-to-life.ps1 -Commit

# Or use watch mode (auto-syncs on file changes)
.\sync-ue5-to-life.ps1 -Watch -Commit
```

**Or just double-click `sync-now.bat`** for a quick sync!

### Manual Sync (Alternative):
```powershell
cd "C:\Users\agonzalez7\life"
git add projects/CPPd1/
git commit -m "Update CPPd1"
git push
```

### On Another Machine:
```powershell
cd "C:\path\to\life\repo"
git pull
# Open CPPd1.uproject in UE5
# Right-click → Generate Visual Studio project files
# Compile and test
```

## ✅ What's Included

- ✅ All C++ source files (`.cpp`, `.h`)
- ✅ Project files (`.uproject`, `.Build.cs`)
- ✅ Configuration files (`.ini`)
- ✅ Documentation (`.md`)
- ✅ `.gitignore` (excludes build files)

## ❌ What's Excluded (by .gitignore)

- `Binaries/` - Compiled executables
- `Intermediate/` - Build intermediates  
- `Saved/` - Editor saves
- `DerivedDataCache/` - Cached data
- Visual Studio temp files

## 🎯 Key Features

- **Wave System** - Spawn enemies in waves
- **Engagement Manager** - One enemy fights at a time
- **Split-Screen** - Always vertical, even solo
- **Combat HUD** - Health, stamina, flow, wave info
- **Xbox Controllers** - Full gamepad support
- **Tuning Variables** - All exposed in editor

## 📝 Last Updated

2026-02-18 - Initial sync to life repo
