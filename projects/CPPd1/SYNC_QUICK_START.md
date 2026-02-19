# Quick Sync Guide - CPPd1 to Life Repo

## 🚀 Automatic Sync (Easiest)

### Option 1: Double-Click Sync
Just **double-click `sync-now.bat`** - it syncs and commits automatically!

### Option 2: PowerShell Command
```powershell
cd "C:\Users\agonzalez7\life\projects\CPPd1"
.\sync-ue5-to-life.ps1 -Commit
```

### Option 3: Watch Mode (Auto-Sync)
```powershell
cd "C:\Users\agonzalez7\life\projects\CPPd1"
.\sync-ue5-to-life.ps1 -Watch -Commit
```
This watches your UE5 project and automatically syncs whenever you save files!

---

## 📋 What Gets Synced?

✅ **Included:**
- All C++ source files (`.cpp`, `.h`)
- All documentation (`.md` files)
- Project config files (`.ini`, `.uproject`)
- Blueprint source files

❌ **Excluded (build artifacts):**
- `Binaries/` - Compiled executables
- `Intermediate/` - Build intermediates
- `Saved/` - Editor saves
- `DerivedDataCache/` - Cached data
- Visual Studio files (`.sln`, `.suo`, etc.)

---

## 🎯 Usage Examples

### After Working on Procedural Cube Ninja:
```powershell
# Just sync your changes
.\sync-ue5-to-life.ps1 -Commit
```

### Continuous Development:
```powershell
# Start watch mode, then work in UE5
# It will auto-sync whenever you save!
.\sync-ue5-to-life.ps1 -Watch -Commit
```

### Sync and Push to Remote:
```powershell
.\sync-ue5-to-life.ps1 -Commit -Push
```

---

## 🔧 Script Options

| Flag | Description |
|------|-------------|
| `-Commit` | Automatically commit changes to git |
| `-Push` | Push to remote after commit (requires `-Commit`) |
| `-Watch` | Watch for file changes and auto-sync |

---

## 📍 File Locations

- **UE5 Project:** `C:\Users\agonzalez7\Documents\Unreal Projects\CPPd1`
- **Life Repo:** `C:\Users\agonzalez7\life\projects\CPPd1`
- **Sync Script:** `C:\Users\agonzalez7\life\projects\CPPd1\sync-ue5-to-life.ps1`

---

## ❓ Troubleshooting

**Script won't run?**
- Make sure you're in the life repo's CPPd1 folder
- Check that PowerShell execution policy allows scripts: `Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser`

**Files not syncing?**
- Check that the UE5 project path is correct in the script
- Make sure you have write permissions to the life repo

**Git errors?**
- Make sure you're in a git repository
- Check that you have git configured: `git config --global user.name "Your Name"`

---

## 💡 Tips

1. **Use watch mode** when actively developing - it's hands-free!
2. **Run sync before closing UE5** to make sure everything is saved
3. **Check git status** after syncing: `git status` to see what changed
4. **The script uses robocopy** for fast, efficient syncing (only copies changed files)

---

**That's it! Your UE5 project is now automatically synced to your life repo! 🎉**
