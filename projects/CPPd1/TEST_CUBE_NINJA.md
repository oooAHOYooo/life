# Testing the Cube Ninja Character

## Quick Test Steps

### 1. Compile the Project
- Press `Ctrl+Alt+F11` in UE5
- Wait for compilation to complete
- Check Output Log for any errors

### 2. Create the Blueprint

1. **Content Browser** → Right-click → **Blueprint Class**
2. Search for `CubeNinjaCharacter` → Select it
3. Name it `BP_CubeNinja`
4. Save it in: `Content/Characters/Ninja/`

### 3. Set Up the Blueprint

**Open `BP_CubeNinja`:**

1. In **Components** panel (left), select `CubeBody`
2. In **Details** panel (right):
   - **Body Scale**: `1.0` (or `0.9` for smaller)
   - **Limb Swing Amount**: `25.0` (degrees)
   - **Limb Swing Speed**: `2.0` (Hz)
   - **Cube Material**: Click dropdown → **Create New Material**

### 4. Create a Simple Material

1. **Content Browser** → Right-click → **Material**
2. Name it `M_CubeNinja`
3. Double-click to open Material Editor
4. In Material Graph:
   - Add **Constant3Vector** node
   - Set RGB values (e.g., `0.2, 0.5, 0.8` for blue)
   - Connect to **Base Color** input
   - Click **Save** and close
5. Back in `BP_CubeNinja`:
   - Select `CubeBody` component
   - Set **Cube Material** to `M_CubeNinja`

### 5. Set as Default Pawn

**Option A: Project Settings**
1. **Edit** → **Project Settings**
2. **Game** → **Default Modes**
3. **Default Pawn Class** → Select `BP_CubeNinja`

**Option B: Game Mode**
1. Create Blueprint from `NinjaGameMode` (if needed)
2. Set **Default Pawn Class** to `BP_CubeNinja`

### 6. Set Up Input (Minimal)

The cube ninja inherits from `NinjaCharacter`, so it needs input actions:

1. **Edit** → **Project Settings** → **Input**
2. Create Input Actions:
   - `IA_Jump`
   - `IA_Move` (Vector2D)
   - `IA_Look` (Vector2D)
3. In `BP_CubeNinja`:
   - Select the character root
   - In **Details**, find **Input** section
   - Assign the Input Actions

### 7. Test in Game

1. **Save All** (Ctrl+S)
2. **Press Play** (or Alt+P)
3. You should see:
   - ✅ Cube ninja made of procedural cubes/spheres
   - ✅ Arms/legs swing when moving
   - ✅ Can jump (double jump works)
   - ✅ Can move around

## Troubleshooting

### Crash on Play
- **Check:** Make sure you compiled successfully (`Ctrl+Alt+F11`)
- **Check:** Material is assigned to `CubeBody` → `Cube Material`
- **Check:** Input Actions are set up
- **Try:** Use regular `NinjaCharacter` first to test if issue is cube-specific

### Cube Ninja Not Visible
- **Fix:** Assign a Material to `Cube Material` (it appears black without material)
- **Fix:** Check `Body Scale` is not 0
- **Fix:** Make sure `CubeBody` component is attached

### No Movement
- **Fix:** Set up Input Actions (Jump, Move, Look)
- **Fix:** Check Game Mode is set correctly
- **Fix:** Make sure you're controlling the right pawn

### Limbs Not Swinging
- **Check:** `Limb Swing Amount` > 0
- **Check:** `Limb Swing Speed` > 0
- **Check:** Character is actually moving (not just standing)

## Expected Behavior

✅ **Visual:**
- Blocky ninja made of cubes (spine, chest, limbs) and spheres (head, pelvis, joints)
- Material applied to all parts
- Skeletal mesh is hidden

✅ **Movement:**
- Double jump works
- Roll/dodge works (if input action set)
- Flips work (if input actions set)
- Arms/legs swing when moving

✅ **Combat:**
- Combo attacks work
- Charged attacks work
- Lock-on works

## Code Safety Features

The cube ninja code now includes:
- ✅ Null pointer checks
- ✅ Array bounds validation
- ✅ Component validity checks
- ✅ Safe initialization order

## Next Steps After Testing

1. **Tune Parameters:**
   - Adjust `BodyScale` for size
   - Adjust `LimbSwingAmount` for animation intensity
   - Adjust `LimbSwingSpeed` for animation speed

2. **Create Better Material:**
   - Add normal maps for detail
   - Add roughness/metallic for PBR
   - Add emissive for glow effects

3. **Add to Game Mode:**
   - Set as default pawn for testing
   - Or create spawn system

---

**Ready to test!** Follow the steps above and let me know what you see! 🎮
