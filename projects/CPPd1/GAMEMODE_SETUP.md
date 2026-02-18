# Game Mode Setup Guide

## 🎯 Why Game Modes Don't Appear in Dropdown

C++ classes need to be **compiled first** before they appear in UE5 dropdowns. Here's how to fix it:

---

## ✅ Step 1: Compile the Code

1. **In Unreal Editor**, press **Ctrl+Alt+F11** (or go to **Tools → Compile**)
2. Wait for compilation to finish
3. Check the **Output Log** for any errors

---

## ✅ Step 2: Where to Find Game Modes

After compiling, you can set the game mode in **two places**:

### Option A: World Settings (Per-Level)
1. Open your level
2. Go to **Window → World Settings** (or press **Shift+F1**)
3. In **Game Mode** section, click the dropdown next to **GameMode Override**
4. You should see:
   - **CPPd1GameMode** (base game mode with 2-player support)
   - **CombatGameMode** (combat variant with 2-player support)

### Option B: Project Settings (Default for All Levels)
1. Go to **Edit → Project Settings**
2. Navigate to **Game → Game Mode**
3. Set **Default Game Mode** to your game mode class

---

## ✅ Step 3: Create Blueprint (Recommended)

Even though the C++ classes are available, it's better to create **Blueprint classes** so you can:
- Set default pawn classes visually
- Configure settings in the editor
- Make changes without recompiling

### How to Create Blueprint Game Mode:

1. In **Content Browser**, right-click in an empty area
2. Select **Blueprint Class**
3. In the **Pick Parent Class** dialog, search for:
   - `CombatGameMode` (for combat)
   - `CPPd1GameMode` (for base)
4. Name it (e.g., `BP_CombatGameMode`)
5. Open the Blueprint
6. In **Class Defaults**, set:
   - **Default Pawn Class**: Your character Blueprint (e.g., `BP_CombatCharacter`)
   - **Player Controller Class**: `CombatPlayerController` or `CPPd1PlayerController`
   - **HUD Class**: (optional)
   - **Game State Class**: Should auto-set to `CPPd1GameStateBase`
   - **Player State Class**: Should auto-set to `CPPd1PlayerState`

---

## 🔍 Troubleshooting

### Game Mode Still Not Showing?

1. **Check Compilation**:
   - Look for errors in **Output Log**
   - Make sure compilation succeeded (green checkmark)

2. **Refresh Editor**:
   - Close and reopen Unreal Editor
   - Sometimes classes need a refresh to appear

3. **Check Class Names**:
   - Make sure you're searching for the exact class name
   - `CombatGameMode` (not `Combat Game Mode`)

4. **Verify UCLASS Macro**:
   - Classes should have `UCLASS(BlueprintType, Blueprintable)`
   - ✅ `CPPd1GameMode` - Has it
   - ✅ `CombatGameMode` - Has it

### Still Not Working?

Try creating the Blueprint manually:
1. In Content Browser → **Add → Blueprint Class**
2. Search for `GameModeBase` as parent
3. In the Blueprint, go to **File → Reparent Blueprint**
4. Search for `CombatGameMode` or `CPPd1GameMode`
5. This forces UE5 to recognize the C++ class

---

## 📋 Quick Checklist

- [ ] Code compiled successfully (Ctrl+Alt+F11)
- [ ] No compilation errors in Output Log
- [ ] Checked World Settings → Game Mode dropdown
- [ ] Checked Project Settings → Default Game Mode
- [ ] Created Blueprint from C++ class (recommended)
- [ ] Set Default Pawn Class in Blueprint
- [ ] Set Player Controller Class in Blueprint

---

## 🎮 Recommended Setup

**For Combat Game:**
1. Create `BP_CombatGameMode` from `CombatGameMode` C++ class
2. Set **Default Pawn Class** to your `BP_CombatCharacter`
3. Set **Player Controller Class** to `CombatPlayerController`
4. Use this Blueprint in **World Settings** or **Project Settings**

This gives you the best of both worlds:
- C++ functionality (2-player, respawn, etc.)
- Blueprint flexibility (easy tweaking)

---

## 💡 Pro Tip

After creating the Blueprint, you can:
- Expose C++ variables in the Blueprint
- Add Blueprint-only logic
- Override C++ functions in Blueprint
- All without touching C++ code!
