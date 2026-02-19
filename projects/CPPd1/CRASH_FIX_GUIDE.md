# Crash Fix Guide - Cube Ninja Access Violation

## 🔴 Current Issue

Getting `EXCEPTION_ACCESS_VIOLATION writing address 0x0000000000000458` when using `CubeNinjaCharacter`.

## ✅ Safety Fixes Applied

1. ✅ Null pointer checks in component creation
2. ✅ Array bounds validation
3. ✅ Delayed mesh building (timer-based)
4. ✅ Component validity checks before attachment
5. ✅ Safe initialization order

## 🔧 Quick Fix: Test with Regular Ninja First

If crashes persist, temporarily switch to regular `NinjaCharacter` to test if issue is cube-specific:

### Option 1: Change in C++ (Quick)

Edit `NinjaGameMode.cpp`:

```cpp
// Change this line:
DefaultPawnClass = ACubeNinjaCharacter::StaticClass();

// To this:
DefaultPawnClass = ANinjaCharacter::StaticClass();
```

Then compile (`Ctrl+Alt+F11`).

### Option 2: Override in Blueprint

1. Create Blueprint from `NinjaGameMode`
2. Name it `BP_NinjaGameMode`
3. In Blueprint, set **Default Pawn Class** to `NinjaCharacter` (or `BP_Ninja`)
4. Set Game Mode to `BP_NinjaGameMode` in Project Settings

## 🐛 Debugging Steps

### Step 1: Check Output Log
1. **Window** → **Output Log**
2. Look for errors before the crash
3. Check for "null pointer" or "invalid" messages

### Step 2: Test Regular Ninja
- Switch to `NinjaCharacter` (see above)
- If it works → issue is cube-specific
- If it crashes → issue is elsewhere

### Step 3: Check Material
- Make sure Material is assigned to `CubeBody` → `Cube Material`
- Unassigned material can cause crashes

### Step 4: Check Component Setup
- Open `BP_CubeNinja` Blueprint
- Check `CubeBody` component exists
- Check it's attached to root

## 🔍 Potential Causes

1. **ProceduralMeshComponent not initialized**
   - Fixed: Added null checks and delayed building

2. **Component attachment order**
   - Fixed: Added validity checks before attachment

3. **Material access**
   - Check: Make sure material is assigned

4. **BeginPlay timing**
   - Fixed: Delayed mesh building with timer

## 🛠️ Alternative: Disable Cube Body Temporarily

If you need to test other features:

1. In `CubeNinjaCharacter.cpp`, comment out:
   ```cpp
   // CubeBody = CreateDefaultSubobject<UCubeNinjaBodyComponent>(TEXT("CubeBody"));
   ```

2. Use regular `NinjaCharacter` instead

3. Test other features (movement, combat, etc.)

4. Come back to cube ninja later

## 📝 What to Report

If crash persists, check:
- [ ] Output Log errors (before crash)
- [ ] Does regular `NinjaCharacter` work?
- [ ] Is Material assigned?
- [ ] What were you doing when it crashed? (Play, compile, etc.)

---

**Try compiling with the new safety checks first!** The delayed mesh building should help. 🚀
