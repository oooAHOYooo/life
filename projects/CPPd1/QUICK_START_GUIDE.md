# Quick Start Guide - CPPd1 Ninja Game

## ✅ Compilation Complete!

Your project has successfully compiled. Follow these steps to test and deploy:

---

## 🎮 Testing in UE5 (Right Now)

### Step 1: Open Project in UE5
1. Open Unreal Engine 5.6
2. Open your project: `CPPd1.uproject`
3. Wait for shader compilation to finish

### Step 2: Set Up Game Mode
1. **Edit → Project Settings → Game → Game Mode**
   - Set **Default GameMode** to `CombatGameMode`
   - Or set it per-level in **World Settings** (Window → World Settings)

### Step 3: Create/Open a Level
1. Create a new level or open an existing one
2. Make sure you have:
   - A player start (or two for split-screen)
   - Some ground/floor geometry

### Step 4: Add Wave Spawner
1. In the **Place Actors** panel, search for `CombatWaveSpawner`
2. Drag it into your level
3. Select it and in the **Details** panel:
   - Set **Enemy Class** (create a Blueprint from `CombatEnemy` if needed)
   - Configure **Wave Configs**:
     - Wave 1: 3 enemies, 2s spawn interval
     - Wave 2: 5 enemies, 2s spawn interval
   - Enable **Start Waves On Begin Play**

### Step 5: Test Play
1. Press **Play** (or PIE - Play In Editor)
2. You should see:
   - Split-screen (vertical, even with 1 player)
   - Enemies spawning in waves
   - Only one enemy engages at a time (UFC-style)

---

## 🎮 Xbox Deployment (By Friday)

### Prerequisites
1. **Xbox Developer Account** (ID@Xbox or Creator Program)
2. **Xbox Development Kit (XDK)** installed
3. **Xbox console** in Developer Mode

### Step 1: Configure for Xbox
1. **Edit → Project Settings → Platforms → Xbox Series X|S**
   - Set **Target Platform** to Xbox Series X or S
   - Configure **Xbox Live** settings if needed
   - Set **Build Configuration** to Development or Shipping

### Step 2: Build for Xbox
1. **File → Package Project → Xbox Series X|S**
2. Or use **Build → Build Solution** in Visual Studio with Xbox target

### Step 3: Deploy to Xbox
1. Connect Xbox to same network
2. Use **Unreal Frontend** or **Xbox Dev Portal**
3. Deploy the `.xex` file to your console

### Step 4: Test on Xbox
1. Launch from Xbox dashboard
2. Test with 2 Xbox controllers
3. Verify split-screen works correctly

### Important Xbox Considerations:
- **Input**: Xbox controllers should work automatically with Enhanced Input
- **Performance**: Test frame rate, especially with split-screen
- **Memory**: Monitor memory usage (Xbox Series X has 16GB, Series S has 10GB)
- **Certification**: If submitting to Microsoft Store, follow Xbox certification requirements

---

## 🔄 Combining with Your Life Repo (Git Setup)

### Option 1: Add This Project to Existing Repo (Recommended)

```bash
# Navigate to your life repo
cd "C:\path\to\your\life\repo"

# Add this project as a subdirectory
# Copy the entire CPPd1 folder into your repo
# Then:

git add CPPd1/
git commit -m "Add CPPd1 ninja game project"
git push
```

### Option 2: Merge Two Repos

```bash
# In your life repo
cd "C:\path\to\your\life\repo"

# Add this repo as a remote
git remote add cppd1 "C:\Users\agonzalez7\Documents\Unreal Projects\CPPd1"

# Fetch and merge
git fetch cppd1
git merge cppd1/main --allow-unrelated-histories

# Or create a submodule
git submodule add "C:\Users\agonzalez7\Documents\Unreal Projects\CPPd1" CPPd1
```

### Option 3: Initialize Git in This Project (If Not Already)

```bash
cd "C:\Users\agonzalez7\Documents\Unreal Projects\CPPd1"

# Initialize git (if not already)
git init

# Create .gitignore for Unreal projects
# (I'll create this for you)

# Add and commit
git add .
git commit -m "Initial commit: CPPd1 ninja game with wave spawning and split-screen"
```

### Recommended .gitignore for Unreal Projects

I'll create a proper `.gitignore` file for you that excludes:
- `Binaries/`
- `Intermediate/`
- `Saved/`
- `DerivedDataCache/`
- `*.sln`, `*.suo`, etc.

---

## 📋 Quick Checklist

### Before Testing:
- [ ] Game Mode set to `CombatGameMode`
- [ ] `CombatWaveSpawner` placed in level
- [ ] Enemy class assigned to wave spawner
- [ ] Player start(s) in level

### Before Xbox Deployment:
- [ ] Xbox Developer Account active
- [ ] XDK installed
- [ ] Xbox in Developer Mode
- [ ] Project configured for Xbox platform
- [ ] Tested with 2 controllers locally first

### Before Committing to Git:
- [ ] `.gitignore` configured
- [ ] No sensitive data (API keys, etc.)
- [ ] Large binary files excluded
- [ ] Commit message descriptive

---

## 🐛 Troubleshooting

### Split-Screen Not Working?
- Check `bForceSplitScreenForSolo` is `true` in GameMode
- Verify `SplitScreenManager` component exists
- Check console for errors

### Enemies Not Spawning?
- Verify `CombatWaveSpawner` is in level
- Check `Enemy Class` is assigned
- Look at `Wave Configs` array has entries
- Check Output Log for errors

### Xbox Build Fails?
- Verify XDK is installed
- Check Xbox platform settings
- Ensure all dependencies are included
- Check for Xbox-specific API usage

---

## 📞 Next Steps

1. **Test in UE5** - Get basic gameplay working
2. **Tune Wave System** - Adjust enemy counts, spawn rates
3. **Polish Combat** - Fine-tune the engagement system
4. **Xbox Testing** - Deploy and test on hardware
5. **Git Integration** - Commit to your life repo

Good luck with your Friday deadline! 🚀
