# Step-by-Step: Bring CPPd1 Cube Ninja into UE5

## ✅ Prerequisites (Already Done!)

- ✅ All C++ code is in Git
- ✅ All compilation fixes applied
- ✅ Game mode configured to use CubeNinjaCharacter
- ✅ Safety checks added

## 🚀 Step-by-Step Setup

### Step 1: Open Your UE5 Project

1. Open **Unreal Engine 5.6**
2. Open your project: `C:\Users\agonzalez7\Documents\Unreal Projects\CPPd1\CPPd1.uproject`
3. If prompted to rebuild, click **Yes**

### Step 2: Sync Latest Code (If Needed)

If you made changes on another machine:

```powershell
# Open PowerShell in your life repo
cd "C:\Users\agonzalez7\life"
git pull

# Sync to UE5 project
robocopy "C:\Users\agonzalez7\life\projects\CPPd1\Source" "C:\Users\agonzalez7\Documents\Unreal Projects\CPPd1\Source" /E /NFL /NDL /NP /XO
```

### Step 3: Compile the Project

1. In UE5, press **`Ctrl+Alt+F11`**
2. Wait for compilation to complete
3. Check **Output Log** (bottom panel) for any errors
4. Should see: **"Compilation successful"** or **"Live coding ready"**

### Step 4: Set Game Mode (One-Time Setup)

1. **Edit** → **Project Settings**
2. Click **Game** in left sidebar
3. Under **Default Modes**:
   - **Default Game Mode** → Click dropdown → Select **`NinjaGameMode`**
4. Close Project Settings

### Step 5: Create Material for Cube Ninja (One-Time Setup)

The cube ninja needs a material or it appears black:

1. In **Content Browser**, navigate to where you want the material
2. **Right-click** in empty space → **Material**
3. Name it: **`M_CubeNinja`**
4. **Double-click** to open Material Editor
5. In Material Graph:
   - **Right-click** → Search **"Constant3Vector"** → Add it
   - Set RGB values (e.g., `0.2, 0.5, 0.8` for blue, or `1.0, 0.0, 0.0` for red)
   - **Drag** from Constant3Vector output → Connect to **Base Color** input
6. Click **Save** (top toolbar)
7. Close Material Editor

### Step 6: Create Cube Ninja Blueprint (One-Time Setup)

1. In **Content Browser**, create folder structure:
   - **Right-click** → **New Folder** → Name: `Characters`
   - Inside `Characters`, create: `Ninja` folder
   - Inside `Ninja`, create: `Materials` folder

2. Create Blueprint:
   - **Right-click** in `Characters/Ninja/` folder
   - **Blueprint Class**
   - In "Pick Parent Class" window:
     - Search: **`CubeNinjaCharacter`**
     - Select it
     - Click **Select**
   - Name it: **`BP_CubeNinja`**

3. Open `BP_CubeNinja`:
   - **Double-click** `BP_CubeNinja` to open
   - In **Components** panel (left), click **`CubeBody`**
   - In **Details** panel (right), find **Cube Ninja** section:
     - **Cube Material** → Click dropdown → Select **`M_CubeNinja`**
   - Click **Compile** (top toolbar)
   - Click **Save** (top toolbar)
   - Close Blueprint Editor

### Step 7: Set Up Input Actions (If Not Already Done)

The cube ninja needs input actions to move:

1. **Edit** → **Project Settings** → **Input**
2. Under **Input Actions**, create:
   - **`IA_Jump`** (Action, bool)
   - **`IA_Move`** (Action, Vector2D)
   - **`IA_Look`** (Action, Vector2D)
   - **`IA_ComboAttack`** (Action, bool)
   - **`IA_ChargedAttack`** (Action, bool)
   - **`IA_LockOn`** (Action, bool)
   - **`IA_Roll`** (Action, bool)

3. Under **Input Mappings**, create **Input Mapping Context**:
   - Name: **`IMC_Default`**
   - Add mappings:
     - `IA_Jump` → Space bar
     - `IA_Move` → WASD
     - `IA_Look` → Mouse XY
     - `IA_ComboAttack` → Left Mouse Button
     - `IA_ChargedAttack` → Right Mouse Button (Hold)
     - `IA_LockOn` → Tab
     - `IA_Roll` → Left Shift

### Step 8: Assign Input to Cube Ninja (Optional)

If you want to set input in the Blueprint:

1. Open **`BP_CubeNinja`**
2. Select root component (character)
3. In **Details** → **Input** section:
   - Assign the Input Actions you created
4. **Save** and close

**Note:** Input can also be set in C++ or via Input Mapping Context in Player Controller.

### Step 9: Press Play! 🎮

1. Click **Play** button (top toolbar) or press **Alt+P**
2. You should see:
   - ✅ Cube ninja spawns automatically
   - ✅ Made of procedural cubes and spheres
   - ✅ Can move with WASD
   - ✅ Can jump with Space
   - ✅ Arms/legs swing when moving

## 🎯 Quick Checklist

- [ ] Opened UE5 project
- [ ] Compiled (`Ctrl+Alt+F11`)
- [ ] Set Game Mode to `NinjaGameMode`
- [ ] Created `M_CubeNinja` material
- [ ] Created `BP_CubeNinja` Blueprint
- [ ] Assigned material to `CubeBody` → `Cube Material`
- [ ] Set up Input Actions (or use existing)
- [ ] Pressed Play

## 🔧 Troubleshooting

### Compilation Errors
- Check **Output Log** for specific errors
- Make sure all files synced correctly
- Try: **File** → **Refresh Visual Studio Project**

### Cube Ninja Not Visible
- **Check:** Material assigned to `CubeBody` → `Cube Material`
- **Check:** `Body Scale` is not 0
- **Check:** Game Mode is `NinjaGameMode`

### No Movement
- **Check:** Input Actions are created
- **Check:** Input Mapping Context is set up
- **Check:** Player Controller is `NinjaPlayerController`

### Crash on Play
- **Check:** Material is assigned (required!)
- **Check:** Compilation was successful
- **Check:** Output Log for crash details

## 📝 What's Already Configured

✅ **Game Mode:** `NinjaGameMode` uses `CubeNinjaCharacter`  
✅ **Player Controller:** `NinjaPlayerController` respawns as `CubeNinjaCharacter`  
✅ **All C++ Code:** Compiled and ready  
✅ **Safety Checks:** Null pointer protection added  

## 🎮 After Setup

Once everything works:
- Tune `Body Scale`, `Limb Swing Amount`, `Limb Swing Speed` in Blueprint
- Create better materials (normal maps, PBR, etc.)
- Add wave spawner for enemies
- Test with Xbox controllers

---

**You're ready! Follow the steps above and you'll have a working cube ninja! 🚀**
