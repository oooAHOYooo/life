# 🎮 Quick Setup Guide - Xbox Controllers & Game Modes

This guide will help you set up your game with Xbox controllers and Enhanced Input in just a few minutes.

---

## ✅ Step 1: Set Up Game Mode (2 minutes)

### Option A: Use C++ Game Mode Directly
1. **Edit → Project Settings**
2. **Game → Game Mode**
3. Set **Default Game Mode** to `CombatGameMode`
4. Done! ✅

### Option B: Create Blueprint Game Mode (Recommended)
1. **Content Browser → Right-click → Blueprint Class**
2. Search for `CombatGameMode` as parent
3. Name it `BP_CombatGameMode`
4. Open the Blueprint
5. In **Class Defaults**, set:
   - **Default Pawn Class**: Your character Blueprint (create one if needed)
   - **Player Controller Class**: `CombatPlayerController`
   - **Game State Class**: `CPPd1GameStateBase` (auto-set)
   - **Player State Class**: `CPPd1PlayerState` (auto-set)
6. Use `BP_CombatGameMode` in **Project Settings → Default Game Mode**

---

## ✅ Step 2: Set Up Enhanced Input for Xbox Controllers (5 minutes)

### 2.1: Create Missing Input Actions

You already have some, but you need combat actions:

1. **Content Browser → Content/Input/Actions** (or create this folder)
2. **Right-click → Input → Input Action**
3. Create these Input Actions:
   - `IA_ComboAttack` (Value Type: Digital)
   - `IA_ChargedAttack` (Value Type: Digital)
   - `IA_LockOn` (Value Type: Digital)

### 2.2: Set Up Input Mapping Context for Gamepad

1. **Content Browser → Content/Input**
2. **Right-click → Input → Input Mapping Context**
3. Name it `IMC_Gamepad`
4. Open `IMC_Gamepad`
5. **Add mappings** (click the + button for each):

#### Movement & Camera:
- **IA_Move** → **Gamepad Left Thumbstick** (2D Vector)
- **IA_Look** → **Gamepad Right Thumbstick** (2D Vector)
- **IA_Jump** → **Gamepad Face Button Bottom** (A button)

#### Combat Actions:
- **IA_ComboAttack** → **Gamepad Face Button Right** (X button)
- **IA_ChargedAttack** → **Gamepad Right Trigger** (RT)
- **IA_LockOn** → **Gamepad Right Shoulder** (RB)

### 2.3: Set Up Input Mapping Context for Keyboard/Mouse

1. Open your existing `IMC_Default` (or create new `IMC_KeyboardMouse`)
2. **Add mappings**:

#### Movement & Camera:
- **IA_Move** → **WASD** (2D Vector)
- **IA_MouseLook** → **Mouse XY** (2D Vector)
- **IA_Jump** → **Space Bar**

#### Combat Actions:
- **IA_ComboAttack** → **Left Mouse Button**
- **IA_ChargedAttack** → **Right Mouse Button** (Hold)
- **IA_LockOn** → **Middle Mouse Button** (or Tab key)

### 2.4: Assign Input Mapping Contexts to Player Controller

1. **Content Browser → Right-click → Blueprint Class**
2. Search for `CombatPlayerController` as parent
3. Name it `BP_CombatPlayerController`
4. Open the Blueprint
5. In **Class Defaults**, find **Input → Input Mappings**:
   - **Default Mapping Contexts**: Add both `IMC_Gamepad` and `IMC_KeyboardMouse`
   - **Mobile Excluded Mapping Contexts**: Add `IMC_MouseLook` (for keyboard/mouse)

6. **Compile** and **Save**

### 2.5: Update Your Character Blueprint

1. Open your `CombatCharacter` Blueprint (or create one from `CombatCharacter` C++ class)
2. In **Class Defaults**, find **Input** section:
   - **Jump Action**: `IA_Jump`
   - **Move Action**: `IA_Move`
   - **Look Action**: `IA_Look`
   - **Mouse Look Action**: `IA_MouseLook`
   - **Combo Attack Action**: `IA_ComboAttack`
   - **Charged Attack Action**: `IA_ChargedAttack`
   - **Lock On Action**: `IA_LockOn`

3. **Compile** and **Save**

---

## ✅ Step 3: Test Multi-Controller Setup (2 minutes)

### Enable Multiple Controllers:
1. **Edit → Project Settings**
2. **Engine → Input**
3. Check **Always Show Touch Interface** = **False** (if you see it)
4. **Number of Players** = **2** (for local multiplayer)

### Test in Play:
1. **Play** button → **Number of Players: 2**
2. Connect **2 Xbox controllers** (or 1 controller + keyboard)
3. Both players should spawn and be controllable!

---

## 📋 Complete Setup Checklist

- [ ] **Game Mode Set Up**
  - [ ] Created `BP_CombatGameMode` from `CombatGameMode`
  - [ ] Set Default Pawn Class in Blueprint
  - [ ] Set Player Controller Class in Blueprint
  - [ ] Set as Default Game Mode in Project Settings

- [ ] **Input Actions Created**
  - [ ] `IA_ComboAttack`
  - [ ] `IA_ChargedAttack`
  - [ ] `IA_LockOn`
  - [ ] (Existing: `IA_Move`, `IA_Look`, `IA_MouseLook`, `IA_Jump`)

- [ ] **Input Mapping Contexts Created**
  - [ ] `IMC_Gamepad` (Xbox controller mappings)
  - [ ] `IMC_KeyboardMouse` (or updated `IMC_Default`)

- [ ] **Player Controller Blueprint**
  - [ ] Created `BP_CombatPlayerController`
  - [ ] Assigned Input Mapping Contexts
  - [ ] Compiled and Saved

- [ ] **Character Blueprint**
  - [ ] Created `BP_CombatCharacter` from `CombatCharacter`
  - [ ] Assigned all Input Actions
  - [ ] Compiled and Saved

- [ ] **Testing**
  - [ ] Tested with 1 Xbox controller
  - [ ] Tested with keyboard/mouse
  - [ ] Tested with 2 controllers (local multiplayer)

---

## 🎮 Xbox Controller Button Reference

| Button | Unreal Name | Common Use |
|--------|-------------|------------|
| **A** | `Gamepad Face Button Bottom` | Jump |
| **B** | `Gamepad Face Button Right` | Cancel/Back |
| **X** | `Gamepad Face Button Left` | Combo Attack |
| **Y** | `Gamepad Face Button Top` | Special Action |
| **Left Stick** | `Gamepad Left Thumbstick` | Move |
| **Right Stick** | `Gamepad Right Thumbstick` | Look/Camera |
| **Left Trigger** | `Gamepad Left Trigger` | Block/Parry |
| **Right Trigger** | `Gamepad Right Trigger` | Charged Attack |
| **Left Shoulder** | `Gamepad Left Shoulder` | Lock-On Cycle |
| **Right Shoulder** | `Gamepad Right Shoulder` | Lock-On |
| **D-Pad** | `Gamepad Directional Pad` | Menu Navigation |
| **Start** | `Gamepad Special Right` | Pause Menu |
| **Back/Select** | `Gamepad Special Left` | Menu |

---

## 🚀 Quick Start Template

If you want to skip manual setup, here's the **fastest path**:

1. **Create Blueprint Game Mode**:
   - `BP_CombatGameMode` from `CombatGameMode`
   - Set Default Pawn to your character Blueprint
   - Set Player Controller to `BP_CombatPlayerController`

2. **Create Input Actions** (3 actions):
   - `IA_ComboAttack`, `IA_ChargedAttack`, `IA_LockOn`

3. **Create Input Mapping Context**:
   - `IMC_Gamepad` with all mappings (see Step 2.2)
   - Update `IMC_Default` with keyboard mappings (see Step 2.3)

4. **Create Player Controller Blueprint**:
   - `BP_CombatPlayerController` from `CombatPlayerController`
   - Add both IMCs to Default Mapping Contexts

5. **Update Character Blueprint**:
   - Assign all Input Actions

6. **Test!** 🎮

---

## 💡 Pro Tips

### Auto-Switch Between Gamepad and Keyboard:
The Enhanced Input system automatically detects which device is being used. You can have both IMCs active at the same time - Unreal will use whichever device the player is actively using.

### Multiple Players with Different Devices:
- **Player 1**: Xbox Controller → Uses `IMC_Gamepad`
- **Player 2**: Keyboard/Mouse → Uses `IMC_KeyboardMouse`
- Both work simultaneously! 🎉

### Save Your Setup:
Once everything is working, **save your Blueprints** and they'll be ready for future projects or sharing with your team.

---

## 🐛 Troubleshooting

**Controllers not working?**
- Check Windows Game Controller settings (Win+R → `joy.cpl`)
- Make sure controllers are recognized by Windows
- Try restarting Unreal Editor

**Input Actions not triggering?**
- Verify Input Actions are assigned in Character Blueprint
- Check Input Mapping Context is added to Player Controller
- Make sure IMC priority is set correctly (0 = highest)

**Can't see game modes in dropdown?**
- Compile C++ code first (Ctrl+Alt+F11)
- Refresh editor (close and reopen)
- Check for compilation errors

---

**You're all set!** 🎮✨
