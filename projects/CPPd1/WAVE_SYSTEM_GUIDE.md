# 🌊 Wave System & UFC-Style Combat Guide

All systems are implemented in **C++** - no Blueprint setup required! Just compile and use.

---

## ✅ What Was Created

### 1. **CombatWaveSpawner** (`CombatWaveSpawner.h/cpp`)
- Spawns enemies in **configurable waves** (3, 5, etc.)
- Each wave has its own enemy count, delay, and spawn interval
- Automatically moves to next wave when current wave is defeated
- Events: `OnWaveStarted`, `OnWaveCompleted`, `OnAllWavesCompleted`

### 2. **CombatEngagementManager** (`CombatEngagementManager.h/cpp`)
- **Sequential enemy engagement** - only **one enemy fights at a time** (UFC-style)
- Automatically queues enemies and enables AI one at a time
- Disables AI for waiting enemies
- Auto-starts next engagement when current enemy dies
- Configurable engagement distance and transition delay

### 3. **CPPd1SplitScreenManager** (`CPPd1SplitScreenManager.h/cpp`)
- **Vertical split-screen** (left/right) by default
- **Always enabled**, even for solo play
- Integrated into `CombatGameMode`

### 4. **CombatGameMode** (Updated)
- Integrates all systems automatically
- Split-screen setup on BeginPlay
- Connects wave spawner to engagement manager
- All configurable via Blueprint or C++

---

## 🎮 How to Use

### Step 1: Compile the Code
Press **Ctrl+Alt+F11** in Unreal Editor to compile.

### Step 2: Set Up Wave Spawner in Level

1. **Place `CombatWaveSpawner` actor** in your level
2. In **Details Panel**, configure waves:
   - **Wave Configs** array:
     - **Wave 0**: Enemy Count = 3, Enemy Class = YourEnemyBlueprint
     - **Wave 1**: Enemy Count = 5, Enemy Class = YourEnemyBlueprint
     - **Wave 2**: Enemy Count = 7, Enemy Class = YourEnemyBlueprint
   - **Start Waves On Begin Play** = true
   - **Spawn Interval** = 1.0s (time between enemy spawns in wave)

### Step 3: Set Up Game Mode

1. **Project Settings → Game → Default Game Mode** = `CombatGameMode`
2. Or create Blueprint from `CombatGameMode`:
   - Set **Wave Spawner** reference to your spawner actor
   - **Enable Split Screen** = true
   - **Force Split Screen For Solo** = true
   - **Use Vertical Split** = true

### Step 4: Test!

- **Solo Play**: Split-screen shows player + ghost (both controllable)
- **2 Players**: Split-screen shows both players
- **Waves**: Enemies spawn in waves (3, then 5, then 7, etc.)
- **Engagement**: Only one enemy fights at a time (UFC-style)

---

## ⚙️ Configuration Variables

### CombatWaveSpawner
- **Wave Configs**: Array of wave configurations
  - `EnemyCount`: Number of enemies (3, 5, 7, etc.)
  - `WaveStartDelay`: Delay before wave starts
  - `SpawnInterval`: Time between enemy spawns
  - `EnemyClass`: Enemy Blueprint to spawn

### CombatEngagementManager
- **Engagement Distance**: How close enemy must be to engage (500cm default)
- **Engagement Transition Delay**: Time before next enemy engages (2s default)

### CombatGameMode
- **Enable Split Screen**: Turn split-screen on/off
- **Use Vertical Split**: true = left/right, false = top/bottom
- **Force Split Screen For Solo**: Always show split-screen (even with 1 player)
- **Num Split Screen Players**: Number of viewports (2 default)

---

## 🎯 How It Works

### Wave Flow:
1. **Wave 1 starts** → Spawns 3 enemies
2. **Enemies register** to Engagement Manager
3. **First enemy engages** (AI enabled)
4. **Other enemies wait** (AI disabled)
5. **First enemy dies** → Second enemy engages
6. **Repeat** until wave is complete
7. **Next wave starts** automatically

### Split-Screen Flow:
1. **Game starts** → Split-screen enabled
2. **Player 1** gets left viewport
3. **Player 2** (or ghost) gets right viewport
4. **Both viewports** active simultaneously
5. **Vertical split** by default (can change to horizontal)

---

## 🔧 Advanced Usage

### Custom Wave Patterns:
```cpp
// In Blueprint or C++:
WaveSpawner->WaveConfigs.Add({
    .EnemyCount = 10,
    .WaveStartDelay = 3.0f,
    .SpawnInterval = 0.5f,
    .EnemyClass = StrongEnemyClass
});
```

### Manual Engagement Control:
```cpp
// Start engagement manually
EngagementManager->StartNextEngagement();

// End current engagement
EngagementManager->EndCurrentEngagement();

// Register enemies manually
EngagementManager->RegisterEnemies(EnemyArray);
```

### Events:
- **OnWaveStarted**: Fired when wave begins
- **OnWaveCompleted**: Fired when wave is defeated
- **OnAllWavesCompleted**: Fired when all waves done
- **OnEngagementStarted**: Fired when enemy starts fighting
- **OnEngagementEnded**: Fired when enemy stops fighting

---

## 📋 Example Setup

### Wave Configuration:
- **Wave 0**: 3 Basic Enemies, 2s delay, 1s interval
- **Wave 1**: 5 Basic Enemies, 3s delay, 0.8s interval
- **Wave 2**: 7 Strong Enemies, 5s delay, 0.5s interval
- **Wave 3**: 10 Elite Enemies, 7s delay, 0.3s interval

### Result:
1. Game starts → 2s delay
2. 3 enemies spawn (1 per second)
3. First enemy engages (UFC-style)
4. When defeated, second enemy engages
5. Repeat until all 3 defeated
6. 3s delay → Next wave starts
7. 5 enemies spawn → Same process
8. Continue until all waves complete

---

## 🎮 Split-Screen Behavior

### Solo Play:
- **Left Viewport**: Player 1
- **Right Viewport**: Ghost character (mirrors player)
- Both controllable with same input

### 2 Players:
- **Left Viewport**: Player 1 (Controller 1)
- **Right Viewport**: Player 2 (Controller 2)
- Independent controls

### Always On:
- Split-screen **always active** (even in menus if configured)
- Can be disabled via `bEnableSplitScreen = false`

---

## 🐛 Troubleshooting

**Enemies not engaging?**
- Check `EngagementManager` is initialized
- Verify enemies are registered via `RegisterEnemies()`
- Check `EngagementDistance` is reasonable

**Split-screen not showing?**
- Verify `bEnableSplitScreen = true` in GameMode
- Check `NumPlayers = 2` in GameMode
- Ensure `bForceSplitScreenForSolo = true` for solo play

**Waves not starting?**
- Check `bStartWavesOnBeginPlay = true`
- Verify `WaveConfigs` array has entries
- Check `EnemyClass` is set in wave configs

---

## 💡 Pro Tips

1. **Adjust Engagement Distance**: Larger = enemies engage from farther away
2. **Tune Transition Delay**: Longer = more time between fights (UFC-style pacing)
3. **Wave Timing**: Use delays to create tension between waves
4. **Enemy Variety**: Different enemy classes per wave for progression
5. **Split-Screen Layout**: Vertical works best for side-by-side combat

---

**Everything is in C++ - just compile and play!** 🎮✨
