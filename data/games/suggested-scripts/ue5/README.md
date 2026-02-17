# P2G4W – UE5 C++ scripts (copy-paste)

All scripts are based on the [GDD](../../gdd.md): 2-player setup, core loop hooks, win/lose/round flow (6d), and a 3rd person character (6a–6e).

---

## Files to add to your project

| File | Where in UE5 project |
|------|----------------------|
| `P2G4W.h` | `Source/YourModule/Public/P2G4W.h` |
| `P2G4W.cpp` | `Source/YourModule/Private/P2G4W.cpp` |
| `P2G4WGameModeBase.h` | `Source/YourModule/Public/` |
| `P2G4WGameModeBase.cpp` | `Source/YourModule/Private/` |
| `P2G4WGameStateBase.h` | `Source/YourModule/Public/` |
| `P2G4WGameStateBase.cpp` | `Source/YourModule/Private/` |
| `P2G4WPlayerState.h` | `Source/YourModule/Public/` |
| `P2G4WPlayerState.cpp` | `Source/YourModule/Private/` |
| `P2G4WPlayerController.h` | `Source/YourModule/Public/` |
| `P2G4WPlayerController.cpp` | `Source/YourModule/Private/` |
| `P2G4WCharacter.h` | `Source/YourModule/Public/` |
| `P2G4WCharacter.cpp` | `Source/YourModule/Private/` |
| `P2G4WGoalZone.h` | `Source/YourModule/Public/` |
| `P2G4WGoalZone.cpp` | `Source/YourModule/Private/` |
| `P2G4WLockOnTargetComponent.h` | `Source/YourModule/Public/` |
| `P2G4WLockOnTargetComponent.cpp` | `Source/YourModule/Private/` |
| `P2G4WEnemy.h` | `Source/YourModule/Public/` |
| `P2G4WEnemy.cpp` | `Source/YourModule/Private/` |
| `P2G4WEnemyWaveSpawner.h` | `Source/YourModule/Public/` |
| `P2G4WEnemyWaveSpawner.cpp` | `Source/YourModule/Private/` |

**Module name:** Replace `P2G4W` with your game module name everywhere (includes, `CLASS` macro, and `.Build.cs`) if your project is not named P2G4W.

---

## Build.cs (dependencies)

In `Source/YourModule/YourModule.Build.cs`, ensure you have:

```csharp
PublicDependencyModuleNames.AddRange(new string[]
{
    "Core",
    "CoreUObject",
    "Engine",
    "EnhancedInput",
    "InputCore",
    "GameplayTasks"
});
```

If you already have a third-person template (e.g. ClaCo), you likely have these. Add any missing ones.

---

## Editor setup after paste

1. **Compile** – Build the project (Ctrl+Alt+B or Build in IDE).
2. **Default Game Mode** – Project Settings → Maps & Modes → Default GameMode → set to `P2G4WGameModeBase` (or your Blueprint child).
3. **Player Starts** – In the level, place two **Player Start** actors. Set **Player Start Tag** to `PlayerStart0` and `PlayerStart1` (without space).
4. **Split-screen (optional)** – Project Settings → Maps & Modes → **Use Splitscreen** if you want two cameras.
5. **Input (character)** – Create a Blueprint based on `P2G4WCharacter`. In Class Defaults, assign **Jump Action**, **Move Action**, **Look Action**, **Mouse Look Action**, **Lock On Action**, and **Attack Action** (Enhanced Input). Add an **Input Mapping Context** for Player 0 (e.g. KB+Mouse) and, if needed, a second context for Player 1 (e.g. Gamepad).

6. **Lock-on (Zelda-style)** – Hold Lock On to lock camera and character onto the nearest valid target (enemies or any actor with tag `P2G4WLockOnTarget`). Add **P2G4W Lock On Target Component** to any actor to make it lock-on-able. Release Lock On to return to free camera.

7. **Enemies and waves** – Place **P2G4W Enemy Wave Spawner** in the level. In Details: add waves (Count + Enemy Class; use `P2G4WEnemy` or a Blueprint child), set **Spawn Locations** (or use **Fallback Spawn Offset**). When all enemies in wave N are dead, wave N+1 spawns automatically. Set **Start Waves On Begin Play** to true for “enter room and fight,” or false and call **Start Waves** from Blueprint when the player enters.

8. **Goal Zone (playable prototype)** – In the level, place a **P2G4W Goal Zone** actor (or a Blueprint based on it). Resize the trigger box so both players can reach it. When a character overlaps it, that player wins the round (`OnRoundEnd` is called). From Blueprint (e.g. Game Mode), call **RestartRound()** after a short delay to respawn both and play again.

---

## Playable prototype (first to the goal wins)

After copy-pasting all files and the editor setup above:

1. **Level:** Two **Player Start** (tags `PlayerStart0`, `PlayerStart1`) and one **P2G4W Goal Zone** somewhere in the level (e.g. center or end of a corridor).
2. **Play:** P1 and P2 move (and jump). First player to walk into the goal zone wins the round; the Game Mode's `OnRoundEnd(WinnerPlayerIndex)` runs.
3. **Play again:** In a Blueprint based on `P2G4WGameModeBase`, override **Event On Round End** and use a **Delay** (e.g. 2 seconds) then call **Restart Round**. Both players respawn at their starts and the next round begins.

No extra C++ is required for this loop. You can later replace the goal with collectibles, a finish line, or a different win condition.

---

## Zelda-style lock-on and enemies

- **Lock-on:** Character has **Lock On Action** (hold). When pressed, finds the nearest actor with tag **P2G4WLockOnTarget** in range and in front; camera and character rotate to face it. Movement becomes strafing around the target. Add **P2G4W Lock On Target Component** to enemies, crates, or switches to make them valid targets.
- **Melee attack:** **Attack Action** triggers **DoMeleeAttack()**: a short range sweep in front of the character damages any **P2G4WEnemy** (by default 1 damage, range/radius editable).
- **Enemies:** **P2G4WEnemy** – has Health, chases the player (Player 0), is lock-on-able, and takes damage from **P2G4WTakeDamage** (called by the character’s melee). Override **OnEnemyDeath** in Blueprint for VFX/sound.
- **Waves (one at a time):** **P2G4W Enemy Wave Spawner** – define waves (e.g. wave 0: 3 enemies, wave 1: 5 enemies). Spawns wave 0; when all are dead, spawns wave 1, and so on. Override **OnAllWavesComplete** in Blueprint (e.g. open door, spawn goal). Use one spawner per “room” for Zelda-style clear-the-room flow.

---

## What each class does (GDD mapping)

| Class | GDD | Purpose |
|------|-----|--------|
| **P2G4WGameModeBase** | 6d, 6e | Creates 2 players (`EnsureSecondPlayer`), spawns pawns at PlayerStart0/1 or offset. Round/game over hooks: `OnRoundEnd(WinnerPlayerIndex)`, `OnGameOver()`. |
| **P2G4WGameStateBase** | 6d | Shared state: `GamePhase`, `CurrentRound`, `GetPlayerStateByIndex(0/1)`. |
| **P2G4WPlayerState** | 6e | Per-player: `PlayerIndex` (0/1), `Score`. Use for coop or versus. |
| **P2G4WPlayerController** | 6e | Minimal; `GetP2G4WPlayerIndex()`. Use for UI or camera. |
| **P2G4WCharacter** | 6a, 6b | 3rd person Move/Look/Jump, lock-on, melee, **health/death/respawn** (P2G4WTakeDamage, OnPlayerDeath, RequestRespawn via Game Mode). Assign Lock On and Attack in Blueprint. |
| **P2G4WLockOnTargetComponent** | — | Add to any actor to make it a valid lock-on target (adds tag P2G4WLockOnTarget). |
| **P2G4WEnemy** | — | Basic enemy: health, chase player, **contact damage to player** (with cooldown), lock-on-able, P2G4WTakeDamage. Use in wave spawner or place manually. |
| **P2G4WEnemyWaveSpawner** | — | Zelda-style waves: spawn one wave at a time; when all enemies in the wave are dead, spawn the next. OnAllWavesComplete when done. |
| **P2G4WGoalZone** | 6d | Trigger volume: when a P2G4W character overlaps, that player gets score and the round ends (`OnRoundEnd`). Place in level for "first to the goal" prototype. |
| **RestartRound()** (Game Mode) | 6d | Respawns both players at their starts; call after a round to play again. |
| **RequestRespawn()** (Game Mode) | — | Respawn one player after a delay (e.g. when character dies). Character calls this from P2G4WTakeDamage when Health ≤ 0. |

---

## Scripts checklist (is anything missing?)

See **SCRIPTS-CHECKLIST.md** in this folder for a list of what’s included and what’s optional (HUD, audio, 2P enemy target, game over when both dead, etc.). The current set is enough for a **complete playable loop** (move, lock-on, attack, enemies, waves, player death and respawn, goal zone).

---

## Optional: use your existing character (ClaCo)

If you already use **ClaCoCharacter**:

- In **P2G4WGameModeBase** constructor, set `DefaultPawnClass = AClaCoCharacter::StaticClass();` (and add `#include "ClaCoCharacter.h"`).
- Or create a Blueprint based on `P2G4WCharacter` and assign your mesh/anim; no need to change C++.
