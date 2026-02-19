# HUD and Controls Setup Guide

## Overview

The ninja game now has:
1. **Health bar** positioned in the **top-left** corner
2. **Controls overlay** showing keyboard and gamepad controls
3. **Input mode selection** screen on first login

## Step 1: Create Blueprint Widgets

### 1.1 Create NinjaHUD Blueprint

1. In **Content Browser**, create folder: `UI/Ninja`
2. **Right-click** → **User Widget** → **Create Widget Blueprint**
3. In "Pick Parent Class" window:
   - Search: **`NinjaHUD`**
   - Select it
   - Click **Select**
4. Name it: **`WBP_NinjaHUD`**
5. Open `WBP_NinjaHUD`:
   - **Designer** tab
   - Add a **Canvas Panel** (root)
   - Add a **Horizontal Box** or **Border** for health bar container
   - Position it in **top-left** (Anchor: Top-Left, X: 20, Y: 20)
   - Add **Progress Bar** → Name: **`HealthBar`** (must match!)
   - Set **Percent** binding to update from character
   - Add **Text Block** for "Health" label (optional)
   - **Compile** and **Save**

### 1.2 Create ControlsOverlay Blueprint

1. **Right-click** in `UI/Ninja` folder
2. **User Widget** → **Create Widget Blueprint**
3. Parent class: **`ControlsOverlay`**
4. Name it: **`WBP_ControlsOverlay`**
5. Open `WBP_ControlsOverlay`:
   - Add **Canvas Panel** (root)
   - Add **Vertical Box** or **Border** for overlay background
   - Position in **bottom-right** or wherever you want
   - Add **Text Block** for "Controls" title
   - Add **Panel** → Name: **`KeyboardControlsPanel`** (must match!)
     - Inside: Add text blocks for:
       - WASD - Move
       - Mouse - Look
       - Space - Jump
       - Left Click - Combo Attack
       - Right Click - Charged Attack
       - Tab - Lock On
       - Shift - Roll
       - Q/1-5 - Flips/Kick
   - Add **Panel** → Name: **`GamepadControlsPanel`** (must match!)
     - Inside: Add text blocks for:
       - Left Stick - Move
       - Right Stick - Look
       - A - Jump
       - X - Combo Attack
       - Y - Charged Attack
       - Left Bumper - Lock On
       - B - Roll
       - D-Pad/Buttons - Flips/Kick
   - Set **Visibility** of panels based on input mode (use Blueprint logic)
   - **Compile** and **Save**

### 1.3 Create InputModeSelection Blueprint

1. **Right-click** in `UI/Ninja` folder
2. **User Widget** → **Create Widget Blueprint**
3. Parent class: **`InputModeSelection`**
4. Name it: **`WBP_InputModeSelection`**
5. Open `WBP_InputModeSelection`:
   - Add **Canvas Panel** (root)
   - Add **Border** or **Panel** for background (semi-transparent)
   - Add **Text Block** → Name: **`TitleText`** (must match!)
     - Text: "Select Input Mode"
   - Add **Text Block** → Name: **`DescriptionText`** (must match!)
     - Text: "Choose your preferred control method"
   - Add **Button** → Name: **`KeyboardButton`** (must match!)
     - Text: "Keyboard & Mouse"
     - **On Clicked** → Call **`Select Keyboard`** function
   - Add **Button** → Name: **`GamepadButton`** (must match!)
     - Text: "Gamepad"
     - **On Clicked** → Call **`Select Gamepad`** function
   - **Compile** and **Save**

## Step 2: Assign Widgets to Player Controller

1. Open **`BP_NinjaPlayerController`** (or create it from `NinjaPlayerController`)
2. In **Details** panel, find **UI** section:
   - **Input Mode Selection Widget Class** → Select **`WBP_InputModeSelection`**
   - **Controls Overlay Widget Class** → Select **`WBP_ControlsOverlay`**
   - **HUD Widget Class** → Select **`WBP_NinjaHUD`** (inherited from CombatPlayerController)
3. **Compile** and **Save**

## Step 3: Set Up Input Mapping Contexts

### 3.1 Create Keyboard Input Mapping Context

1. **Edit** → **Project Settings** → **Input** → **Input Mapping Contexts**
2. Create new: **`IMC_Keyboard`**
3. Add mappings:
   - `IA_Move` → WASD
   - `IA_Look` → Mouse XY
   - `IA_Jump` → Space
   - `IA_ComboAttack` → Left Mouse Button
   - `IA_ChargedAttack` → Right Mouse Button
   - `IA_LockOn` → Tab
   - `IA_Roll` → Left Shift
   - `IA_Backflip` → 1
   - `IA_Frontflip` → 2
   - `IA_SideflipLeft` → 3
   - `IA_SideflipRight` → 4
   - `IA_Flip360` → 5
   - `IA_Kick` → Q

### 3.2 Create Gamepad Input Mapping Context

1. Create new: **`IMC_Gamepad`**
2. Add mappings:
   - `IA_Move` → Left Stick
   - `IA_Look` → Right Stick
   - `IA_Jump` → Gamepad Face Button Bottom (A)
   - `IA_ComboAttack` → Gamepad Face Button Left (X)
   - `IA_ChargedAttack` → Gamepad Face Button Top (Y)
   - `IA_LockOn` → Gamepad Left Shoulder
   - `IA_Roll` → Gamepad Face Button Right (B)
   - `IA_Backflip` → Gamepad D-Pad Up
   - `IA_Frontflip` → Gamepad D-Pad Down
   - `IA_SideflipLeft` → Gamepad D-Pad Left
   - `IA_SideflipRight` → Gamepad D-Pad Right
   - `IA_Flip360` → Gamepad Right Shoulder
   - `IA_Kick` → Gamepad Left Trigger

### 3.3 Assign to Player Controller

1. Open **`BP_NinjaPlayerController`**
2. In **Details** → **Input** section:
   - **Keyboard Mapping Contexts** → Add **`IMC_Keyboard`**
   - **Gamepad Mapping Contexts** → Add **`IMC_Gamepad`**
3. **Compile** and **Save**

## Step 4: Test

1. **Play** the game
2. You should see the **Input Mode Selection** screen
3. Select **Keyboard** or **Gamepad**
4. The **Controls Overlay** should appear showing the correct controls
5. **Health bar** should be in the **top-left** corner

## Optional: Toggle Controls Overlay

You can add a key binding to toggle the controls overlay:

1. In **`BP_NinjaPlayerController`** or **`BP_Ninja`**
2. Add input action: **`IA_ToggleControls`** → F1 key
3. Bind to **`Toggle Controls Overlay`** function

## Notes

- The input mode selection only shows on **first login** (set `bHasSelectedInputMode = true` after selection)
- To test selection again, set `bHasSelectedInputMode = false` in Blueprint
- Health bar updates automatically from character health
- Controls overlay can be hidden/shown via Blueprint functions
