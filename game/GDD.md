# Game Design Document
## 2P Co-op Combat (Working Title)

**Version:** 0.1  
**Target:** Xbox (Series X), PC. UE5.  
**Goal:** 2-player co-op combat game with style scoring and Skate-like replay.

---

## 1. Core Concept

A 2-player co-op action game where you and a partner fight through themed spots in a Mario-style world. Combat blends **MMA-style** control depth (grapples, staggers, peel for each other) with **Skate-style flick stick** for attacks and **Dragon Ball Z–style** flair (aerial combos, big moves). Runs have **time limits** and **hard fail** (time out or death = over). **Stylish points** and optional **lock-in vs pursue** decisions drive replay: same spots, better grades, “one more try.”

---

## 2. Pillars

- **Co-op first** – Designed for two people on the couch. Peel, set up combos, share the spotlight.
- **Combat feel** – Input that rewards skill: flick stick + MMA layer + weapons + DBZ juice.
- **Short runs, high replay** – One spot = one run. Fail or lock in. Retry instantly. Chase style rank.
- **Friendly world** – Mario-inspired: colorful, themed worlds, clear goals. Tone is fun, not grim.

---

## 3. Combat

### 3.1 Control Philosophy

- **Flick stick (right stick)** – Direction + timing = attack type (slash, launcher, heavy, etc.). Combos by chaining flicks. High skill ceiling, expressive.
- **MMA layer** – Grapples, clinch, transitions, staggers. One player can hold/grapple while the other hits. Stamina or similar resource for big grapples.
- **Weapons** – Swords (and maybe one or two more). Swap per spot or per world. Flick stick applies to armed and unarmed.
- **DBZ layer** – Dash, air combos, one or two “signature” moves per character (big hit or energy-style). Fast movement, readable but flashy.

### 3.2 Win / Lose

- **Time** – Each run has a time limit (e.g. 2–3 min per spot). Time runs out = **run over**. No “who’s ahead.”
- **Health** – Zero health = that player is dead. **Both must survive** (or one death = game over). No respawn mid-run.
- **Over = over** – Restart the spot. No continues. Instant retry.

### 3.3 Style & Replay

- **Stylish points** – Earned from: combo length, move variety, co-op links (your hit into partner’s launcher), parries/perfect dodges, finishers. Grade at end (e.g. D/C/B/A/S).
- **Lock-in vs pursue (optional)** – At a moment (e.g. after a wave), choose: **Lock in** (bank current score, end run safely) or **Pursue** (next wave / harder, more style possible, risk death or time). Co-op decision.
- **Replay** – Same spot, same time limit. Chase higher grade, faster clear, or specific challenges (e.g. “S-rank in under 2 min,” “no damage”).

---

## 4. Co-op Encounters

- **Enemy types** – Themed per world. Examples: brutes (tank + DPS roles), duos (mirror you), grappler + striker (peel for each other), one in air / one on ground (DBZ-style split).
- **Boss per world** – One big fight: weak points, phases, teamwork (tank/flank, interrupt, combo together).
- **Spots** – Each spot = one or a few encounter types. Short, repeatable.

---

## 5. World & Structure

- **World map** – Mario-style. 6–8 themed worlds (grass, desert, sky, lava, ice, etc.). Clear visual identity per world.
- **Fight spots** – Multiple spots per world (courtyard, bridge, arena, rooftop). Travel to spot → fight → optional challenges.
- **Story** – Linear at first: World 1 → 2 → … → final boss. Beating boss unlocks next world.
- **Post-story** – Map open. All spots available. Replay = free play + per-spot challenges (time, rank, no damage, etc.).

---

## 6. Platform & Tech

- **Engine:** Unreal Engine 5.
- **Platforms:** Xbox Series X (primary), other consoles later. PC possible.
- **Input:** Gamepad. 2P local (split-screen or shared screen TBD).
- **Deploy / test:** Xbox Dev Mode on retail Series X; ID@Xbox for store.

---

## 7. Scope Tiers (Future)

- **V0 (Prototype)** – One arena, 2P, move + one or two attacks, basic health. Runs on Xbox. *Target: by next Monday.*
- **V1** – Flick stick attacks, one enemy type, timer, “over when time or health gone,” one style grade. One spot.
- **V2** – Second spot, lock-in/pursue (if fun), 2–3 enemy types, one boss.
- **V3** – World map, 2–3 worlds, per-spot challenges, full replay loop.
- **V4** – More worlds, story beat, polish, store submission.

---

## 8. References

- **Combat feel:** MMA (UFC games), Skate (flick input), DBZ (aerial, big moves), Soulcalibur / For Honor (weapons).
- **Structure:** Mario (world map, themes), Skate (spots, challenges, “one more try”).
- **Co-op:** Designed for couch 2P; peel, combos, shared stakes.

---

*Doc lives in `life/game/`. Update as the design evolves.*
