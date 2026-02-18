# Git Setup Guide - Combining with Your Life Repo

## Quick Setup Options

### Option 1: Initialize Git Here and Push to Your Life Repo

```powershell
# Navigate to project
cd "C:\Users\agonzalez7\Documents\Unreal Projects\CPPd1"

# Initialize git (if not already)
git init

# Add remote to your life repo
git remote add origin <YOUR_LIFE_REPO_URL>

# Add all files
git add .

# Commit
git commit -m "Initial commit: CPPd1 ninja game with wave spawning, split-screen, and combat systems"

# Push to your life repo
git push -u origin main
```

### Option 2: Add This Project as a Subdirectory to Your Life Repo

```powershell
# Navigate to your life repo
cd "C:\path\to\your\life\repo"

# Copy this project folder into your repo
# Then:
git add CPPd1/
git commit -m "Add CPPd1 ninja game project"
git push
```

### Option 3: Use Git Submodule (Best for Keeping Projects Separate)

```powershell
# In your life repo
cd "C:\path\to\your\life\repo"

# First, initialize git in CPPd1 if not already
cd "C:\Users\agonzalez7\Documents\Unreal Projects\CPPd1"
git init
git add .
git commit -m "Initial commit"

# Then in your life repo
cd "C:\path\to\your\life\repo"
git submodule add "C:\Users\agonzalez7\Documents\Unreal Projects\CPPd1" CPPd1
git commit -m "Add CPPd1 as submodule"
git push
```

## What's Included in .gitignore

The `.gitignore` file I created excludes:
- ✅ `Binaries/` - Compiled executables
- ✅ `Intermediate/` - Build intermediates
- ✅ `Saved/` - Editor saves
- ✅ `DerivedDataCache/` - Cached data
- ✅ Visual Studio files (`.sln`, `.suo`, etc.)
- ✅ Sample code folder

**Included:**
- ✅ All source code (`.cpp`, `.h`)
- ✅ Build files (`.Build.cs`)
- ✅ Project files (`.uproject`)
- ✅ Config files (`.ini`, `.json`)
- ✅ Documentation (`.md`)

## Important Notes

1. **Don't commit large binary files** - Unreal assets (`.uasset`, `.umap`) are excluded by default
2. **Keep source code** - All C++ source is included
3. **Multi-computer workflow** - Each developer needs to:
   - Pull the repo
   - Generate project files (right-click `.uproject` → Generate Visual Studio project files)
   - Compile in UE5

## Recommended Workflow

```powershell
# Daily workflow
git pull                    # Get latest changes
# Work on code...
git add .
git commit -m "Description of changes"
git push                    # Share with team
```

## If Combining with Existing Life Repo

If your life repo already has a different structure, you can:

1. **Create a `projects/` folder** in your life repo
2. **Move CPPd1 into it**: `projects/CPPd1/`
3. **Commit**: `git add projects/CPPd1/ && git commit -m "Add CPPd1 project"`

This keeps all your projects organized in one repo!
