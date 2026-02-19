# Quick Start - Cube Ninja (Ready to Play!)

## ✅ What's Already Set Up

The game mode is **already configured** to use `CubeNinjaCharacter` by default!

- ✅ `NinjaGameMode` uses `CubeNinjaCharacter` as default pawn
- ✅ `NinjaPlayerController` respawns as `CubeNinjaCharacter`
- ✅ All C++ code is ready

## 🚀 Just Compile and Play!

### Step 1: Compile
Press **`Ctrl+Alt+F11`** in UE5

### Step 2: Set Game Mode (One Time)
1. **Edit** → **Project Settings**
2. **Game** → **Default Modes**
3. **Default Game Mode** → Select `NinjaGameMode`

### Step 3: Create Material (One Time)
The cube ninja needs a material or it appears black:

1. **Content Browser** → Right-click → **Material**
2. Name it `M_CubeNinja`
3. Double-click to open
4. Add **Constant3Vector** node
5. Set RGB (e.g., `0.2, 0.5, 0.8` for blue)
6. Connect to **Base Color**
7. **Save** and close

### Step 4: Assign Material to Cube Ninja

**Option A: Create Blueprint (Recommended)**
1. **Content Browser** → Right-click → **Blueprint Class**
2. Search `CubeNinjaCharacter` → Select
3. Name it `BP_CubeNinja`
4. Open it
5. Select `CubeBody` component
6. **Details** → **Cube Material** → Select `M_CubeNinja`
7. **Save**

**Option B: Set Material in C++ Defaults (Advanced)**
- You can set the material in the C++ constructor if you prefer

### Step 5: Press Play! 🎮

That's it! The cube ninja should spawn automatically.

## 🎯 What You'll See

- ✅ Procedural cube ninja (cubes + spheres)
- ✅ Arms/legs swing when moving
- ✅ Double jump works
- ✅ All ninja features (roll, flips, combat)

## ⚙️ Tuning (Optional)

In `BP_CubeNinja` (or C++ defaults):
- **Body Scale**: `0.5` - `2.0` (size)
- **Limb Swing Amount**: `0` - `60` (animation intensity)
- **Limb Swing Speed**: `0.1` - `5.0` (animation speed)

## 🔧 If Cube Ninja Doesn't Appear

1. **Check Material**: Must assign `M_CubeNinja` to `CubeBody` → `Cube Material`
2. **Check Game Mode**: Must be `NinjaGameMode`
3. **Check Compilation**: Make sure `Ctrl+Alt+F11` completed successfully
4. **Check Output Log**: Look for errors

## 📝 Notes

- The cube ninja **replaces** the skeletal mesh (hides it automatically)
- All movement/combat features work the same as regular `NinjaCharacter`
- Material is required or it appears black
- Blueprint is optional but recommended for easy material assignment

---

**That's it! Compile (`Ctrl+Alt+F11`) and play!** 🚀
