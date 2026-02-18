# Advanced Combat Systems Guide

## 🎮 Overview

This guide covers the advanced combat systems inspired by **Skate 4** (trick-based combos) and **UFC 5** (skill-based fighting). All systems work together to create a deep, skill-based 2-player combat experience.

---

## 🎯 System Components

### 1. **Input Manager** (`CPPd1InputManager`)
- **Purpose**: Detects and manages gamepad vs keyboard/mouse input
- **Features**:
  - Automatic device detection
  - Supports 2 players with different input devices
  - Switches input contexts based on device type

### 2. **Trick System** (`CombatTrickSystem`)
- **Purpose**: Skate 4-style trick/combo system
- **Features**:
  - Directional input sequences (Up, Down, Left, Right, etc.)
  - Button combinations (Light, Heavy, Special)
  - Trick registration and matching
  - Flow point rewards
  - Skill level requirements

### 3. **Skill System** (`CombatSkillSystem`)
- **Purpose**: UFC 5-style skill progression
- **Features**:
  - 6 skill types: Striking, Grappling, Defense, Stamina, Technique, Flow
  - Experience-based leveling (0-100)
  - Affects combat effectiveness
  - Damage multipliers based on skill

### 4. **Stamina System** (`CombatStaminaSystem`)
- **Purpose**: UFC 5-style stamina management
- **Features**:
  - Real-time stamina drain/recovery
  - Exhaustion states
  - Skill-based efficiency
  - Recovery delays

### 5. **Flow System** (`CombatFlowSystem`)
- **Purpose**: Skate 4-style style/flow meter
- **Features**:
  - Flow meter (0-100%)
  - Combo tracking
  - Flow state bonuses
  - Decay over time

### 6. **Advanced Mechanics** (`CombatAdvancedMechanics`)
- **Purpose**: UFC 5-style advanced combat
- **Features**:
  - Parrying with timing windows
  - Blocking with stamina drain
  - Counter-attacks
  - Perfect parry bonuses

---

## 🎮 Setting Up 2-Player with Gamepads

### Step 1: Configure Input Mappings
1. Create **Input Mapping Contexts**:
   - `IMC_Gamepad` - For Xbox controllers
   - `IMC_KeyboardMouse` - For keyboard/mouse
2. Map actions to both contexts:
   - **Move**: Left Stick (Gamepad) / WASD (Keyboard)
   - **Look**: Right Stick (Gamepad) / Mouse
   - **Light Attack**: X (Gamepad) / Left Click (Mouse)
   - **Heavy Attack**: Y (Gamepad) / Right Click (Mouse)
   - **Block/Parry**: Left Trigger (Gamepad) / Shift (Keyboard)
   - **Trick Input**: D-Pad/Right Stick (Gamepad) / Arrow Keys (Keyboard)

### Step 2: Add Input Manager to Game Mode
1. In your `CombatGameMode` Blueprint:
   - Add `CPPd1InputManager` component
   - It will automatically detect and switch input contexts

### Step 3: Test 2-Player
1. **Play Settings** → **Number of Players**: 2
2. Connect 2 Xbox controllers OR 1 controller + keyboard/mouse
3. Each player gets their own input context automatically

---

## 🛹 Setting Up Trick System (Skate 4 Style)

### Step 1: Create Trick Blueprint
1. Create Blueprint from `CombatTrickSystem`
2. Name it `BP_TrickSystem`

### Step 2: Register Tricks
In Blueprint or C++, register tricks:

```cpp
// Example: "Spinning Kick" trick
FCombatTrick SpinningKick;
SpinningKick.TrickName = "SpinningKick";
SpinningKick.InputSequence.Add({ECombatTrickDirection::Right, 1, 0.3f}); // Right + Light
SpinningKick.InputSequence.Add({ECombatTrickDirection::Left, 1, 0.3f});   // Left + Light
SpinningKick.DamageMultiplier = 1.5f;
SpinningKick.FlowPoints = 25.0f;
SpinningKick.RequiredSkillLevel = 10;

TrickSystem->RegisterTrick(SpinningKick);
```

### Step 3: Check Trick Inputs
In your character's input handling:
```cpp
// Get directional input (from stick/d-pad)
ECombatTrickDirection Direction = GetDirectionFromInput();

// Get button input
int32 Button = GetButtonFromInput(); // 0=none, 1=light, 2=heavy, 3=special

// Check trick
FCombatTrickInput Input = {Direction, Button, 0.3f};
TrickSystem->CheckTrickInput(Input, DeltaTime);

// Get matching trick
FCombatTrick Trick = TrickSystem->GetBestMatchingTrick(CurrentSkillLevel);
if (Trick.TrickName != NAME_None)
{
    // Play trick animation
    PlayTrickAnimation(Trick.TrickMontage);
}
```

### Step 4: Trick Examples
- **Light Combo**: Right + Light → Left + Light → Up + Light
- **Heavy Finisher**: Down + Heavy → Up + Heavy
- **Special Move**: Up + Right + Special (requires high skill)

---

## 🥊 Setting Up Skill System (UFC 5 Style)

### Step 1: Add Skill System to Character
1. In your `CombatCharacter` Blueprint:
   - Add `CombatSkillSystem` component
   - It initializes all 6 skill types automatically

### Step 2: Award Experience
```cpp
// When player lands an attack
SkillSystem->AddExperience(ECombatSkillType::Striking, 10.0f);

// When player successfully parries
SkillSystem->AddExperience(ECombatSkillType::Defense, 15.0f);

// When player performs a trick
SkillSystem->AddExperience(ECombatSkillType::Flow, 5.0f);
```

### Step 3: Use Skill Multipliers
```cpp
// Get damage multiplier
float DamageMultiplier = SkillSystem->GetStrikingDamageMultiplier();
float FinalDamage = BaseDamage * DamageMultiplier;

// Get defense effectiveness
float BlockReduction = SkillSystem->GetDefenseEffectiveness();
float ReducedDamage = IncomingDamage * (1.0f - BlockReduction);
```

### Step 4: Skill Progression
- **Striking**: Increases damage output
- **Defense**: Improves blocking/parrying effectiveness
- **Stamina**: Reduces stamina drain, improves recovery
- **Technique**: Improves combo execution windows
- **Flow**: Increases flow meter gains and multipliers

---

## ⚡ Setting Up Stamina System

### Step 1: Add Stamina System
1. Add `CombatStaminaSystem` component to character
2. It automatically integrates with `CombatSkillSystem`

### Step 2: Drain Stamina
```cpp
// When attacking
StaminaSystem->DrainStamina(0.15f); // 15% stamina per attack

// When blocking (automatic in AdvancedMechanics)
// Stamina drains continuously while blocking

// When performing tricks
StaminaSystem->DrainStamina(0.10f * TrickComplexity);
```

### Step 3: Check Stamina
```cpp
// Before allowing action
if (!StaminaSystem->HasStamina(0.15f))
{
    // Can't perform action - show "Exhausted" message
    return;
}

// Check exhaustion state
if (StaminaSystem->IsExhausted())
{
    // Apply exhaustion penalties (slower movement, weaker attacks)
}
```

---

## 🌊 Setting Up Flow System

### Step 1: Add Flow System
1. Add `CombatFlowSystem` component to character

### Step 2: Award Flow Points
```cpp
// When landing a trick
FlowSystem->AddFlowPoints(Trick.FlowPoints);

// When landing a combo
FlowSystem->IncrementCombo(); // Also adds flow points

// When performing style moves
FlowSystem->AddFlowPoints(5.0f);
```

### Step 3: Use Flow Multiplier
```cpp
// When in flow state
if (FlowSystem->IsInFlowState())
{
    float FlowMultiplier = FlowSystem->GetFlowMultiplier(); // 1.5x default
    float EnhancedDamage = BaseDamage * FlowMultiplier;
    float EnhancedSpeed = BaseSpeed * FlowMultiplier;
}
```

---

## 🛡️ Setting Up Advanced Mechanics (Parrying/Blocking)

### Step 1: Add Advanced Mechanics
1. Add `CombatAdvancedMechanics` component to character

### Step 2: Implement Parrying
```cpp
// On parry input
EParryResult Result = AdvancedMechanics->AttemptParry();

if (Result == EParryResult::Perfect)
{
    // Perfect parry - massive damage bonus
    float DamageMultiplier = AdvancedMechanics->PerfectParryDamageMultiplier;
    // Trigger counter-attack window
    AdvancedMechanics->PerformCounterAttack();
}
else if (Result == EParryResult::Good)
{
    // Good parry - small damage reduction
}
```

### Step 3: Implement Blocking
```cpp
// On block input (hold)
AdvancedMechanics->StartBlocking();

// Blocking automatically:
// - Reduces incoming damage
// - Drains stamina
// - Stops if exhausted

// On block release
AdvancedMechanics->StopBlocking();
```

### Step 4: Block Effectiveness
```cpp
// Get damage reduction
float BlockEffectiveness = AdvancedMechanics->GetBlockEffectiveness();
float ReducedDamage = IncomingDamage * (1.0f - BlockEffectiveness);
```

---

## 🔗 Integrating All Systems

### Complete Combat Flow Example:

```cpp
// 1. Check input device
ECPPd1InputDevice Device = InputManager->GetInputDeviceType(PlayerController);

// 2. Get trick input
FCombatTrickInput TrickInput = GetTrickInput(Device);
TrickSystem->CheckTrickInput(TrickInput, DeltaTime);

// 3. Check if trick matches
FCombatTrick Trick = TrickSystem->GetBestMatchingTrick(
    SkillSystem->GetSkillLevel(ECombatSkillType::Technique)
);

// 4. Check stamina
if (StaminaSystem->HasStamina(0.15f))
{
    // 5. Perform trick
    PlayTrickAnimation(Trick.TrickMontage);
    
    // 6. Drain stamina
    StaminaSystem->DrainStamina(0.15f);
    
    // 7. Award experience
    SkillSystem->AddExperience(ECombatSkillType::Striking, 10.0f);
    SkillSystem->AddExperience(ECombatSkillType::Flow, Trick.FlowPoints);
    
    // 8. Add flow points
    FlowSystem->AddFlowPoints(Trick.FlowPoints);
    FlowSystem->IncrementCombo();
    
    // 9. Calculate damage with all multipliers
    float BaseDamage = 10.0f;
    float SkillMultiplier = SkillSystem->GetStrikingDamageMultiplier();
    float FlowMultiplier = FlowSystem->GetFlowMultiplier();
    float TrickMultiplier = Trick.DamageMultiplier;
    
    float FinalDamage = BaseDamage * SkillMultiplier * FlowMultiplier * TrickMultiplier;
    
    // 10. Apply damage
    ApplyDamage(FinalDamage);
}
```

---

## 📊 System Interactions

### Skill → Stamina
- Higher Stamina skill = Less stamina drain
- Better recovery rate

### Skill → Defense
- Higher Defense skill = Better block effectiveness
- Better parry windows

### Flow → Damage
- Higher flow meter = Damage multiplier
- Combo chains increase flow

### Trick → Flow → Skill
- Tricks award flow points
- Flow increases with combos
- Flow skill improves trick effectiveness

### Stamina → Advanced Mechanics
- Blocking drains stamina
- Exhaustion prevents advanced moves
- Perfect parries don't drain stamina

---

## 🎯 Recommended Blueprint Setup

1. **Character Blueprint**:
   - `CombatSkillSystem` component
   - `CombatStaminaSystem` component
   - `CombatFlowSystem` component
   - `CombatAdvancedMechanics` component
   - `CombatTrickSystem` (as object reference)

2. **Game Mode Blueprint**:
   - `CPPd1InputManager` component

3. **Input Actions**:
   - `IA_Move` (Vector2D)
   - `IA_Look` (Vector2D)
   - `IA_LightAttack` (Action)
   - `IA_HeavyAttack` (Action)
   - `IA_Block` (Action)
   - `IA_Parry` (Action)
   - `IA_TrickDirection` (Vector2D) - For trick inputs

---

## 🚀 Next Steps

1. **Compile** all new systems (Ctrl+Alt+F11)
2. **Create Blueprints** from the new components
3. **Set up Input Mappings** for gamepad and keyboard
4. **Register Tricks** in your character Blueprint
5. **Test 2-Player** with gamepads
6. **Balance** stamina drain, flow decay, skill XP rates
7. **Create UI** for stamina bar, flow meter, skill levels

---

## 💡 Pro Tips

- **Trick Complexity**: More complex tricks = more flow points
- **Skill Gates**: Lock advanced tricks behind skill requirements
- **Flow State**: Make flow state feel powerful (speed boost, visual effects)
- **Stamina Management**: Make players choose between offense and defense
- **Parry Timing**: Make perfect parries feel rewarding but risky
- **Combo Chains**: Reward long combo chains with exponential flow gains

Happy fighting! 🥷⚔️
