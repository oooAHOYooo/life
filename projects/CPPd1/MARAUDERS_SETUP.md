# Marauders & spinning radius spawner

**Lore (GDD):** Two gods defend little people from marauders. **Tricksters** are hard to catch; **Brutes** want to wrestle and ask you to put the weapons down (you can still use the sword; unarmed is more enjoyable). **Win:** clear all waves.

---

## Enemy types (C++)

- **TricksterMarauder** – Slippery, hard to catch. Lower HP, faster movement, slightly lower damage. Use for “trickster” waves.
- **BruteMarauder** – Big, tough, wrestle. High HP, slower, heavier hits. `bPrefersUnarmed` is for UI/audio (“put weapons down”); sword still works.

Both use the same AI as other CombatEnemy (StateTree, combo/charged attacks). Assign **Combo Attack** and **Charged Attack** montages in Blueprints (e.g. `BP_TricksterMarauder`, `BP_BruteMarauder`).

---

## Spinning radius spawner (C++)

**ASpinningRadiusWaveSpawner** – Spawns enemies on a circle around the actor. Each spawn advances the angle so spawn points “spin” around the radius.

1. **Place in level:** Place Actors → search **Spinning Radius Wave Spawner**. Put it where you want the **center** of the circle.
2. **Details:**
   - **Spawn Radius** – Distance from center to spawn (cm). e.g. 400.
   - **Angle Step Per Spawn** – Degrees to advance each spawn (e.g. 45 = 8 positions around the circle).
   - **Spawn Height Offset** – Z offset (e.g. 90 so they spawn at foot height).
   - **Wave Configs** – Same as CombatWaveSpawner. Add waves; for each wave set **Enemy Class** or **Enemy Class Pool**.
3. **Use Tricksters and Brutes:** In **Enemy Class Pool** add `TricksterMarauder` and `BruteMarauder` (or your Blueprint children) for mixed waves; or set **Enemy Class** per wave to one type.
4. **Connect to game mode:** In **NinjaGameMode** (or your game mode Blueprint), set **Wave Spawner** to this Spinning Radius Wave Spawner actor. The game mode will register it and feed spawned enemies to the engagement manager (one-on-one fights).

---

## Quick checklist for tomorrow

- [ ] Build the project (`npm run build-cppd1` or Build in Visual Studio).
- [ ] Open the level; place **Spinning Radius Wave Spawner** where the marauders should emerge from.
- [ ] Set **Spawn Radius**, **Angle Step Per Spawn**, **Wave Configs** (e.g. Wave 1: 2 enemies, Enemy Class Pool = Trickster + Brute).
- [ ] Set game mode **Wave Spawner** to that actor.
- [ ] Create Blueprints from **TricksterMarauder** and **BruteMarauder** if you want to assign meshes/montages; then use those Blueprints in the spawner’s **Enemy Class** / **Enemy Class Pool**.
- [ ] Play: marauders spawn around the circle; clear all waves to win (win condition UI is up to you).

---

## Restoration & NPCs (lore)

- **Per wave:** When a wave is cleared, the duo ninjas are healed by **Heal Per Wave Cleared** (game mode, default 1 HP). This is the gods “restoring” damage done by the marauders.
- **All waves:** When all waves are cleared, both gods are healed to full and **On Gods Defended Realm** is broadcast. Use this in Blueprint to have NPCs congratulate the duo, then play a worship/crowd reaction (e.g. little people gather, celebrate, worship).
- In your game mode Blueprint: bind to **On Gods Defended Realm** and from there run your NPC sequence (dialogue, animation, crowd behavior).
