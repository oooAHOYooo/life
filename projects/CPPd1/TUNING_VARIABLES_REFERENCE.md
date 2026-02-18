# Combat Character Tuning Variables Reference

All variables below are **exposed in the Blueprint Inspector** and can be tweaked without recompiling C++ code.

## 📍 How to Access

1. Open your `CombatCharacter` Blueprint
2. Select the character in the viewport or Components panel
3. Look in the **Details** panel
4. Find the **Tuning** categories (they're organized by system)

---

## 🏃 Movement Tuning (`Tuning|Movement`)

| Variable | Default | Description | Units |
|----------|---------|-------------|-------|
| **Movement Speed** | 400.0 | Character walk/run speed | cm/s |
| **Jump Velocity** | 500.0 | How high the character jumps | cm/s |
| **Air Control** | 0.35 | Air control amount (0.0 = none, 1.0 = full) | 0.0-1.0 |
| **Rotation Rate** | 500.0 | How fast character turns | deg/s |

---

## 🛹 Trick System Tuning (`Tuning|Trick System`)

| Variable | Default | Description | Units |
|----------|---------|-------------|-------|
| **Trick Input Gap Time** | 0.5 | Max time between trick inputs to maintain combo | seconds |
| **Base Trick Flow Points** | 10.0 | Base flow points for any trick | points |
| **Complex Trick Multiplier** | 1.5 | Flow multiplier for complex tricks | multiplier |

---

## 🥊 Skill System Tuning (`Tuning|Skill System`)

| Variable | Default | Description | Range |
|----------|---------|-------------|-------|
| **Starting Skill Level** | 0.0 | Starting level for all skills | 0-100 |
| **Skill Experience Multiplier** | 1.0 | XP gain multiplier (2.0 = double XP) | 0.1-10.0 |
| **Base Damage Multiplier** | 0.5 | Damage at skill level 0 | 0.1+ |
| **Max Damage Multiplier** | 2.0 | Damage at skill level 100 | 1.0+ |

---

## ⚡ Stamina System Tuning (`Tuning|Stamina`)

| Variable | Default | Description | Range |
|----------|---------|-------------|-------|
| **Max Stamina** | 1.0 | Maximum stamina (0.0 to 1.0) | 0.0-1.0 |
| **Light Attack Stamina Cost** | 0.10 | Stamina drain per light attack | 0.0-1.0 |
| **Heavy Attack Stamina Cost** | 0.25 | Stamina drain per heavy attack | 0.0-1.0 |
| **Block Stamina Drain Rate** | 0.15 | Stamina drain per second while blocking | 0.0-1.0 per sec |
| **Base Stamina Recovery Rate** | 0.2 | Stamina recovery per second | per second |
| **Stamina Recovery Delay** | 1.0 | Time before stamina starts recovering | seconds |
| **Exhaustion Threshold** | 0.2 | Stamina level for exhaustion (below this = exhausted) | 0.0-1.0 |

---

## 🌊 Flow System Tuning (`Tuning|Flow System`)

| Variable | Default | Description | Range |
|----------|---------|-------------|-------|
| **Max Flow** | 1.0 | Maximum flow meter | 0.0-1.0 |
| **Flow Decay Rate** | 0.1 | Flow decay per second | per second |
| **Flow State Threshold** | 0.7 | Flow level to enter flow state (70%) | 0.0-1.0 |
| **Flow State Multiplier** | 1.5 | Damage/speed multiplier when in flow state | 1.0+ |
| **Flow Points Per Combo** | 5.0 | Flow points awarded per combo hit | points |
| **Combo Reset Time** | 3.0 | Time before combo resets | seconds |

---

## 🛡️ Advanced Mechanics Tuning (`Tuning|Advanced Mechanics`)

| Variable | Default | Description | Range |
|----------|---------|-------------|-------|
| **Perfect Parry Window** | 0.1 | Perfect parry timing window | 0.0-1.0 seconds |
| **Good Parry Window** | 0.2 | Good parry timing window | 0.0-1.0 seconds |
| **Perfect Parry Damage Multiplier** | 2.0 | Damage multiplier for perfect parry counter | 1.0+ |
| **Counter Attack Window** | 1.0 | Time window to perform counter-attack after parry | seconds |
| **Base Block Effectiveness** | 0.5 | Base damage reduction when blocking (50%) | 0.0-1.0 |

---

## ⚔️ General Combat Tuning (`Tuning|Combat`)

| Variable | Default | Description | Range |
|----------|---------|-------------|-------|
| **Global Damage Multiplier** | 1.0 | Multiplies all damage dealt | 0.0+ |
| **Global Defense Multiplier** | 1.0 | Multiplies all damage received | 0.0+ |
| **Invincibility Frames** | 0.5 | Invincibility time after taking damage | seconds |

---

## 🎯 Existing Tuning Variables (Already Exposed)

These were already exposed and remain available:

### Lock-On (`Lock-On`)
- **Lock On Radius**: 1500.0 cm
- **Lock On Rotation Speed**: 360.0 deg/s

### Damage (`Damage`)
- **Max HP**: 5.0
- **Life Bar Color**: (editable color)

### Melee Attack (`Melee Attack`)
- **Melee Trace Distance**: 75.0 cm
- **Melee Trace Radius**: 75.0 cm
- **Melee Damage**: 1.0
- **Melee Knockback Impulse**: 250.0 cm/s
- **Melee Launch Impulse**: 300.0 cm/s
- **Attack Input Cache Time Tolerance**: 1.0 seconds
- **Combo Input Cache Time Tolerance**: 0.45 seconds

### Camera (`Camera`)
- **Death Camera Distance**: 400.0 cm
- **Default Camera Distance**: 100.0 cm

### Respawn (`Respawn`)
- **Respawn Time**: 3.0 seconds

---

## 💡 Tuning Tips

### For Faster Combat:
- Increase **Movement Speed** (500-600)
- Increase **Rotation Rate** (600-800)
- Decrease **Stamina Recovery Delay** (0.5-0.8)
- Increase **Flow Decay Rate** (0.15-0.2)

### For Slower, More Tactical Combat:
- Decrease **Movement Speed** (300-350)
- Decrease **Rotation Rate** (300-400)
- Increase **Stamina Recovery Delay** (1.5-2.0)
- Decrease **Flow Decay Rate** (0.05-0.08)

### For Easier Gameplay:
- Increase **Global Defense Multiplier** (1.5-2.0)
- Increase **Invincibility Frames** (0.8-1.0)
- Increase **Base Block Effectiveness** (0.7-0.9)
- Increase **Perfect Parry Window** (0.15-0.2)

### For Harder Gameplay:
- Decrease **Global Defense Multiplier** (0.5-0.8)
- Decrease **Invincibility Frames** (0.2-0.3)
- Decrease **Base Block Effectiveness** (0.3-0.4)
- Decrease **Perfect Parry Window** (0.05-0.08)

### For More Flow/Style Focus (Skate-like):
- Increase **Flow Points Per Combo** (10-15)
- Increase **Flow State Multiplier** (2.0-2.5)
- Decrease **Flow Decay Rate** (0.05-0.08)
- Increase **Complex Trick Multiplier** (2.0-3.0)

### For More Skill Focus (UFC-like):
- Increase **Skill Experience Multiplier** (1.5-2.0)
- Increase **Max Damage Multiplier** (2.5-3.0)
- Decrease **Base Damage Multiplier** (0.3-0.4)
- Increase **Stamina Recovery Delay** (1.5-2.0)

---

## 🔧 Quick Reference: Common Adjustments

**"Combat feels too fast"**
→ Decrease Movement Speed, Increase Stamina Costs

**"Combat feels too slow"**
→ Increase Movement Speed, Decrease Stamina Recovery Delay

**"Parrying is too hard"**
→ Increase Perfect Parry Window, Increase Good Parry Window

**"Flow meter fills too slow"**
→ Increase Flow Points Per Combo, Decrease Flow Decay Rate

**"Players die too fast"**
→ Increase Global Defense Multiplier, Increase Max HP

**"Players die too slow"**
→ Decrease Global Defense Multiplier, Decrease Max HP

**"Stamina runs out too fast"**
→ Decrease Stamina Costs, Increase Base Stamina Recovery Rate

---

## 📝 Notes

- All variables are **EditAnywhere** - you can change them in Blueprint instances
- Variables with **ClampMin/ClampMax** have enforced ranges
- Variables with **Units** show their measurement unit in the inspector
- Changes take effect **immediately** when playing in editor (no recompile needed)
- For **per-instance** tuning, create different Blueprint variants with different values

---

Happy tuning! 🎮
