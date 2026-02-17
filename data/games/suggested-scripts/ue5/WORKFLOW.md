# Workflow: Edit Scripts in Life Repo → Sync to UE5 Project

## The Problem
- **Script Computer** (with life repo): Where you edit C++ code in Cursor
- **Home Computer** (with UE5 project): Where you test/play the game
- You want to edit on script computer, sync to home computer, without uploading the entire UE5 project

## The Solution: One-Way Sync (Life Repo → UE5)

The **life repo is the source of truth**. You edit scripts there, then sync only the C++ files to your UE5 project.

---

## Setup (One Time)

### On Script Computer (where you edit)

1. **Edit scripts in the life repo:**
   ```
   C:\Users\agonzalez7\life\data\games\suggested-scripts\ue5\
   ```

2. **Commit and push changes to git:**
   ```bash
   git add data/games/suggested-scripts/ue5/
   git commit -m "Updated P2G4W scripts"
   git push
   ```

### On Home Computer (where UE5 project lives)

1. **Clone/pull the life repo:**
   ```bash
   cd C:\Users\agonzalez7\life
   git pull
   ```

2. **Run the sync script:**
   ```powershell
   cd C:\Users\agonzalez7\life\data\games\suggested-scripts\ue5
   .\sync-to-ue5.ps1
   ```

   This copies files from the life repo to your UE5 project, replacing `P2G4W` with `CoPlago`.

3. **Build in UE5:**
   - Open UE5 project
   - Press `Ctrl+Alt+B` to compile
   - Or use: `UnrealEditor.exe -project="path\to\CoPlago.uproject" -run=Compile`

---

## Daily Workflow

### Option A: Manual Sync (Simple)

**On Script Computer:**
1. Edit `.h` and `.cpp` files in `life/data/games/suggested-scripts/ue5/`
2. Save, commit, push to git

**On Home Computer:**
1. Pull the life repo: `git pull`
2. Run sync script: `.\sync-to-ue5.ps1`
3. Build in UE5

### Option B: Automated Sync (Advanced)

**On Home Computer**, set up a file watcher or scheduled task:

**PowerShell script to watch for changes:**
```powershell
# watch-and-sync.ps1
$lifeRepo = "C:\Users\agonzalez7\life\data\games\suggested-scripts\ue5"
$watcher = New-Object System.IO.FileSystemWatcher
$watcher.Path = $lifeRepo
$watcher.Filter = "*.cpp"
$watcher.Filter = "*.h"
$watcher.IncludeSubdirectories = $false
$watcher.EnableRaisingEvents = $true

Register-ObjectEvent $watcher "Changed" -Action {
    Write-Host "File changed, syncing..."
    & "C:\Users\agonzalez7\life\data\games\suggested-scripts\ue5\sync-to-ue5.ps1"
}
```

Or use a simple batch file you run when ready:
```batch
@echo off
cd C:\Users\agonzalez7\life
git pull
cd data\games\suggested-scripts\ue5
powershell -ExecutionPolicy Bypass -File sync-to-ue5.ps1
echo Sync complete! Now build in UE5.
```

---

## What Gets Synced?

**Only these C++ files:**
- All `.h` files → `Source/CoPlago/Public/`
- All `.cpp` files → `Source/CoPlago/Private/`

**What does NOT get synced:**
- UE5 project files (`.uproject`, `.sln`)
- Content folder (Blueprints, assets)
- Intermediate/Binaries folders
- Config files

**Result:** You're only syncing ~20 small text files (C++ source), not gigabytes of UE5 content.

---

## File Mapping

| Life Repo | UE5 Project | Notes |
|-----------|-------------|-------|
| `P2G4W.h` | `Source/CoPlago/Public/CoPlago.h` | Module name replaced |
| `P2G4W.cpp` | `Source/CoPlago/Private/CoPlago.cpp` | Module name replaced |
| `P2G4WCharacter.h` | `Source/CoPlago/Public/CoPlagoCharacter.h` | Class name replaced |
| `P2G4WCharacter.cpp` | `Source/CoPlago/Private/CoPlagoCharacter.cpp` | Class name replaced |
| ... (all other files) | ... | Same pattern |

The sync script automatically:
- Replaces `P2G4W` with `CoPlago` in filenames
- Replaces `P2G4W` with `CoPlago` in file contents (includes, class names, etc.)

---

## Git Strategy

### Life Repo (Script Computer)
- ✅ **Commit C++ scripts** to git
- ✅ **Version control** your game logic
- ✅ **Share with team** via git

### UE5 Project (Home Computer)
- ❌ **Don't commit** synced C++ files to git (they're generated)
- ✅ **Do commit** Blueprints, assets, level files
- ✅ **Do commit** `.uproject`, `.Build.cs` (project config)

**Why?** The life repo is the source of truth. If you edit in UE5 project, those changes will be overwritten on next sync. Always edit in the life repo.

---

## Troubleshooting

### "Module name mismatch"
If you see errors about `P2G4W` vs `CoPlago`, the sync script didn't replace all instances. Check that the script ran successfully.

### "File not found"
Make sure the sync script paths are correct. Edit `sync-to-ue5.ps1` if your UE5 project is in a different location.

### "Build errors after sync"
1. Close UE5 editor
2. Delete `Intermediate` and `Binaries` folders
3. Right-click `.uproject` → "Generate Visual Studio project files"
4. Build again

### "Changes lost after sync"
You edited files in the UE5 project directly. Always edit in the life repo, then sync.

---

## Quick Reference

**Edit scripts:** `C:\Users\agonzalez7\life\data\games\suggested-scripts\ue5\`

**Sync command:** `.\sync-to-ue5.ps1` (from the ue5 folder)

**UE5 project:** `C:\Users\agonzalez7\Documents\Unreal Projects\CoPlago\`

**Build in UE5:** `Ctrl+Alt+B` or use UnrealBuildTool

---

## Summary

1. **Edit** in life repo (Cursor on script computer)
2. **Commit/push** to git
3. **Pull** on home computer
4. **Sync** using the script
5. **Build** in UE5

This keeps your code lightweight (just text files), version controlled, and synced between computers without uploading the entire UE5 project.
