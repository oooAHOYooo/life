# P2G4W – Scripts checklist (what makes the prototype complete)

Use this list to see what’s included and what you might still add in Blueprint or C++.

---

## Included in this folder (copy-paste into UE5)

| Script | Purpose |
|--------|--------|
| **P2G4W** (module) | Log category, module hook. |
| **P2G4WGameModeBase** | 2 players, spawn at starts, round/restart, **RequestRespawn** for player death. |
| **P2G4WGameStateBase** | Game phase, current round, get player state by index. |
| **P2G4WPlayerState** | Player index, score. |
| **P2G4WPlayerController** | Player index helper. |
| **P2G4WCharacter** | Move, look, jump, **lock-on**, **melee**, **health**, **death**, **respawn** (via Game Mode). |
| **P2G4WLockOnTargetComponent** | Add to any actor to make it lock-on-able. |
| **P2G4WGoalZone** | Trigger: first to touch wins round. |
| **P2G4WEnemy** | Health, chase player, **contact damage to player**, lock-on-able, notifies spawner on death. |
| **P2G4WEnemyWaveSpawner** | Waves (one at a time), spawn when previous wave cleared. |

After copy-paste + editor setup you get:

- 2 players, lock-on, melee, goal zone, waves of enemies that deal contact damage.
- Player can take damage, die, and respawn after a delay.

---

## Optional (Blueprint or extra C++)

| What | Notes |
|------|--------|
| **HUD / UI** | Health bars, score, “Wave 2”, “Player 1 / 2”. Use UMG Widgets; bind to **PlayerState** (score) and **Character** (health) or replicate health to **PlayerState** if you want it in UI from there. |
| **Pause / main menu** | Usually Blueprint (pause, show menu widget). Or minimal C++ on **PlayerController** / **Game Mode**. |
| **Audio** | Footsteps, hit, death, lock-on tick: typically Blueprint or Audio components driven by events (**OnPlayerDeath**, **OnEnemyDeath**, etc.). |
| **Enemy targets P2** | Enemy currently chases Player 0. For 2P: in **P2G4WEnemy::ChasePlayer** use nearest of GetPlayerPawn(0) and GetPlayerPawn(1). |
| **Game over when both dead** | When a character dies you already call **RequestRespawn**. For “both dead = game over” track deaths in **Game State** or **Game Mode** and call **OnGameOver** when both are dead (and optionally disable respawn). |
| **Save / checkpoint** | Use **SaveGame** and load/save **PlayerState** (score) and level/checkpoint; not in this script set. |
| **Camera shake** | On player hit or death: call **PlayerController->ClientPlayCameraShake**. Easiest in Blueprint from **OnPlayerDeath** or a small C++ helper. |

---

## Summary

**You do *not* need more scripts** for a complete loop: movement, lock-on, melee, enemies, waves, player health/death/respawn, and goal-based rounds are all covered. What’s left is mostly **Blueprint and content**: input mapping, meshes, animations, VFX, sound, and HUD bound to the existing C++ (health, score, phase).
