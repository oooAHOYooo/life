# CPPd1 Ninja Game Setup Guide

## 🎮 Setting Up 2-Player Support

### Step 1: Configure Game Mode
1. Open your level in Unreal Editor
2. Go to **Edit → Project Settings → Game → Game Mode**
3. Set **Default Game Mode** to your `CombatGameMode` Blueprint (or create one)
4. Or set it in **World Settings** (Window → World Settings)

### Step 2: Set Up Player Spawn Points
1. In your level, add **Player Start** actors (Place Actors → All Classes → Player Start)
2. Place at least 2 Player Start actors for 2-player support
3. The game mode will automatically assign player indices (0 and 1)

### Step 3: Enable Split-Screen (Optional)
1. Go to **Edit → Project Settings → Engine → Game Viewport**
2. Enable **Two Player Splitscreen** or configure as needed
3. Or use **Window → Virtual Cameras** for separate views

### Step 4: Test 2-Player
- **Local Multiplayer**: Add a second player controller in Play settings
- **Network**: Set up as a multiplayer game mode

---

## 👹 Creating Enemies Based on Third Person Character

### Step 1: Create Enemy Blueprint
1. In Content Browser, right-click → **Blueprint Class**
2. Search for `CombatEnemy` (your C++ class)
3. Name it `BP_CombatEnemy` or similar

### Step 2: Set Up Enemy Mesh
1. Open your enemy Blueprint
2. In **Components**, select the **Mesh** component
3. Set **Skeletal Mesh** to the Third Person Character mesh (or your ninja mesh)
4. Set **Animation Blueprint** to Third Person anim blueprint (or create custom)

### Step 3: Configure Enemy Properties
In the enemy Blueprint, set these in **Details**:
- **Max HP**: 3.0 (default) - adjust as needed
- **Melee Damage**: 1.0 - how much damage enemy deals
- **Melee Trace Distance**: 75.0 - attack range
- **Melee Trace Radius**: 50.0 - attack hitbox size
- **Combo Attack Montage**: Assign attack animation
- **Charged Attack Montage**: Assign charged attack animation

### Step 4: Add Lock-On Target Component
The `CombatEnemy` class already has a `LockOnTargetComponent` added in C++!
- It's automatically set up
- You can configure the **Target Bone Or Socket Name** in Blueprint if you want to lock-on to a specific bone
- The **Target Offset** can be adjusted for fine-tuning

### Step 5: Set Up AI Behavior
1. The enemy uses **StateTree** for AI behavior
2. Create a StateTree asset:
   - Right-click → **Artificial Intelligence → State Tree**
   - Name it `ST_CombatEnemy`
3. Configure StateTree tasks:
   - **Chase Player**: Use `CombatStateTreeUtility` tasks
   - **Attack Player**: Use enemy's `DoAIComboAttack()` or `DoAIChargedAttack()`
   - **Patrol**: Add patrol behavior if needed

### Step 6: Assign AI Controller
1. In enemy Blueprint, set **AI Controller Class** to `CombatAIController`
2. Set **Auto Possess AI** to **Placed in World or Spawned**
3. Assign your StateTree to the AI Controller

---

## 🎯 Using Enemy Spawners

### Step 1: Create Enemy Spawner
1. In Content Browser, create Blueprint from `CombatEnemySpawner`
2. Name it `BP_EnemySpawner`

### Step 2: Configure Spawner
1. Place `BP_EnemySpawner` in your level
2. Set **Enemy Class** to your `BP_CombatEnemy`
3. Set **Spawn Count**: How many enemies to spawn
4. Set **Respawn Delay**: Time between enemy spawns
5. **Should Spawn Enemies Immediately**: Check to spawn on level start

### Step 3: Link Spawners (Optional)
- Spawners can activate other spawners when depleted
- Add spawners to **Actors To Activate When Depleted** array
- This creates wave-based enemy spawning

---

## 🏁 Setting Up Goal Zones

### Step 1: Create Goal Zone
1. Create Blueprint from `CPPd1GoalZone`
2. Name it `BP_GoalZone`

### Step 2: Place in Level
1. Place `BP_GoalZone` where players should reach
2. Adjust the **Trigger Box** size to cover the goal area
3. Set **One Win Per Round**: Check if only first player should win

### Step 3: Handle Round End
1. In Blueprint, bind to **On Round Won** event
2. This fires when a player reaches the goal
3. You can:
   - Show victory UI
   - Advance to next round
   - Update scores/honor
   - Reset level for next round

---

## 🎨 Quick Setup Checklist

- [ ] Set Game Mode to `CombatGameMode` (or Blueprint version)
- [ ] Place 2+ Player Start actors in level
- [ ] Create `BP_CombatEnemy` Blueprint with Third Person mesh
- [ ] Assign attack animations to enemy
- [ ] Create StateTree for enemy AI behavior
- [ ] Place `BP_EnemySpawner` actors in level
- [ ] Configure spawner with enemy class
- [ ] Place `BP_GoalZone` for round-winning
- [ ] Test 2-player spawn and gameplay
- [ ] Test lock-on targeting on enemies
- [ ] Test enemy AI chasing and attacking

---

## 💡 Tips

1. **Enemy Mesh**: Use the Third Person Character mesh, or import your own ninja/enemy mesh
2. **Animations**: Reuse Third Person animations, or create custom attack animations
3. **Lock-On**: Enemies automatically have lock-on component - just make sure it's enabled
4. **AI Behavior**: Start simple with chase + attack, then add patrol/patrol behaviors
5. **2-Player Testing**: Use "Number of Players" in Play settings to test locally

---

## 🔧 C++ Classes Reference

- **CPPd1GameModeBase**: Base game mode with 2-player support, respawn, ghost characters
- **CPPd1PlayerState**: Tracks player index (0/1) and honor/score
- **CPPd1GameStateBase**: Tracks game phases and rounds
- **CombatCharacter**: Player character with lock-on, melee combat, health
- **CombatEnemy**: AI enemy with combat capabilities and lock-on target
- **CPPd1GoalZone**: Round-winning trigger volume
- **CPPd1LockOnTargetComponent**: Makes actors lock-on targets
- **CombatEnemySpawner**: Spawns enemies in waves

---

Happy ninja game development! 🥷
