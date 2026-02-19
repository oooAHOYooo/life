# Ninja (OoT-style) setup

Playable ninja with **Link-from-Ocarina-of-Time** style controls: lock-on (Z-target), strafe, sword combo, charged attack, and **roll/dodge**. Supports **2P** and a **ghost** partner when playing solo. Enemies spawn in **waves** and fight one-at-a-time.

---

## 1. Game mode

- **Edit → Project Settings → Game → Default Game Mode** → set to **NinjaGameMode** (or a Blueprint based on it).
- Or per-level: **Window → World Settings** → **Game Mode Override** → NinjaGameMode.

**NinjaGameMode** uses:
- **Default Pawn:** NinjaCharacter (P1 and P2)
- **Player Controller:** NinjaPlayerController (respawns as NinjaCharacter)
- **Solo:** spawns a **ghost** NinjaCharacter that mirrors your input (same split-screen as 2P)

---

## 2. Playable ninja (NinjaCharacter)

- **Lock-on:** same as Combat (toggle lock-on action). When locked on, you strafe around the target and **roll goes backward**.
- **Move / Look:** stick or WASD + mouse.
- **Combo attack:** light attack string (assign **Combo Attack** and **Charged Attack** input actions in Blueprint or IMC).
- **Charged attack:** hold for heavy hit.
- **Roll/dodge:** assign **Roll** input action (e.g. B / Circle). When **not** locked on, rolls in movement direction; when **locked on**, rolls backward.
- **Double jump:** enabled by default (`Jump Count Max` = 2). Tune in Details or Blueprint.
- **Flips (acrobatics):** backflip, frontflip, sideflip left/right, 360 flip. Assign **Backflip**, **Frontflip**, **Sideflip Left**, **Sideflip Right**, **Flip 360** input actions. You can **punch (combo) and/or kick (Kick montage) during flips** — attacks are not blocked; combine motion freely.
- **Kick:** optional **Kick** action + **Kick Montage**. Works in air and during flips.

Create a **Blueprint** from NinjaCharacter (e.g. `BP_Ninja`) to set:
- Skeletal mesh and animations (combo/charged/roll if you have them)
- **Input actions:** Jump, Move, Look, Combo Attack, Charged Attack, Lock-On, **Roll**, **Backflip**, **Frontflip**, **Sideflip Left**, **Sideflip Right**, **Flip 360**, **Kick** (create Input Actions and assign)
- Combo/charged/kick AnimMontages and section names

---

## 3. 2P partner and ghost

- **Two players:** use **Play → Number of Players: 2**. Each gets a NinjaCharacter; split-screen is set by NinjaGameMode (from CombatGameMode).
- **Solo:** with 1 player, the game mode spawns a **ghost** NinjaCharacter that mirrors your movement, look, lock-on, attacks, and roll. The ghost doesn’t take damage and has no camera (it’s the “2P” slot).

No extra setup: same Default Pawn Class and respawn class handle both.

---

## 4. Wave of enemies

1. Place **CombatWaveSpawner** in the level (Place Actors → search “Combat Wave Spawner”).
2. In Details:
   - **Wave Configs:** add entries. Per wave you can set:
     - **Enemy Class** (or **Enemy Class Pool** for mixed types — each spawn picks randomly from the pool).
     - **Intensity Scale** (>1 = harder: enemies deal more damage, take less, move faster).
     - **Rush Wave:** spawns all enemies in the wave at once (no delay between spawns).
   - **Start Waves on Begin Play:** checked if you want waves to start automatically.
3. In **NinjaGameMode** (or your Blueprint): set **Wave Spawner** to that actor (or leave unset and assign in level).
4. **Engagement:** only one enemy is active at a time (UFC-style). When one dies, the next in the wave engages. Enemies are registered when they spawn.

**NinjaEnemy** is a concrete C++ enemy (lock-on target, life bar, combo/charged attacks via AI). Use it as-is or create a Blueprint (e.g. `BP_NinjaEnemy`) to set mesh, animations, and montages.

---

## 5. Quick checklist

- [ ] Default Game Mode = NinjaGameMode (or BP)
- [ ] Level has at least one **Player Start** (two for 2P spawns)
- [ ] **CombatWaveSpawner** in level, **Enemy Class** = NinjaEnemy (or BP), **Wave Spawner** set on game mode if needed
- [ ] **BP_Ninja** (or default NinjaCharacter) has **Roll** Input Action set and combo/charged montages assigned
- [ ] Optional: create **IA_Roll** Input Action and add to your Input Mapping Context (e.g. Gamepad Face Button Bottom or B)

**Style / acrobatic kills:** Bind to **CombatEnemy → On Enemy Killed With Causer**. If the causer is a NinjaCharacter, call **Consume Last Hit Was During Flip** on it; if true, the killing blow was during a flip — use that to add score or multipliers in Blueprint.

Then **Play**: you control the ninja; with 1 player the ghost mirrors you; waves spawn and engage one at a time. Use double jump, flips, and punch/kick during flips to combine motion.
