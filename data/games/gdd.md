# Game Design Document

**Working title:** P2G4W _(Project 2 Player Game for Wife)_  
**Last updated:** 2026-02-17  
**Status:** Draft  

**Today’s focus:** Core mechanics (sections 6a–6e below). Other sections can stay light for now.

---

## 1. Elevator pitch

_One or two sentences. What is this game? Why would someone play it?_

- You and a partner are two gods defending a realm of little people from marauders. Clear all waves to win.

---

## 2. Vision / high concept

_What’s the big idea? What feeling or experience are you aiming for?_

- Two gods, co-op, protecting the small and helpless. One enemy type is slippery and hard to catch (tricksters); the other wants to wrestle and asks you to put the sword away—fighting them unarmed is more satisfying even though the sword still works.

---

## 3. Core loop

_What does the player do minute-to-minute? (e.g. “Explore → Find clue → Solve puzzle → Unlock new area → Repeat.”)_

1. Defend: marauders spawn (tricksters and/or brutes) and attack.
2. Fight: lock on, dodge, use sword or wrestle; one enemy engages at a time.
3. Clear the wave; next wave starts.
4. Repeat until all waves are cleared (win) or both gods fall (lose).

---

## 4. Design pillars

_3–5 rules that guide every decision. If a feature doesn’t support these, cut it._

1. **Co-op Dependency:** Players must cover each other's blind spots and work together.
2. **Predictable Chaos:** Waves should be readable but overwhelming if ignored.
3. **Satisfying Unarmed Combat:** Beating Brutes with bare hands should feel weighty and punishing.
4. **Distinct Pacing:** Slippery Tricksters vs Heavy Brutes force players to constantly adapt their timing and approach.

---

## 5. Genre & references

- **Genre:** _e.g. puzzle, narrative adventure, roguelike, sim_
- **Reference games:** _What games does this feel like or take inspiration from?_
  - 
  - 
  - 

---

## 6. Core mechanics _(today’s focus)_

### 6a. Mechanic list

_Name each main mechanic and one line on what it does._

| # | Mechanic | One-line description |
|---|----------|------------------------|
| 1 | Movement & Dodge | Position carefully to corral enemies and avoid surrounds. |
| 2 | Weapon Swing (Sword) | Primary attack meant for slippery Tricksters. |
| 3 | Unarmed Wrestle | Grapple and throw mechanics intended for Brutes. |
| 4 | Lock-On | Focus on a single target to manage one-on-one engagements. |
| 5 | Revive/Heal Partner | Bring a fallen god back into the fight. |

---

### 6b. Mechanic deep-dive

_For each mechanic above: what does the player do? What are the rules? What’s the feedback?_

**Mechanic 1: _Movement & Dodge_**
- **Player action:** Left stick/WASD to move, button to dodge/roll.
- **Rules:** Dodge has i-frames but a short cooldown/stamina cost.
- **Feedback:** Distinct woosh sound and visual trail during dodge.
- **Notes:** Essential for avoiding Tricksters' attacks.

**Mechanic 2: _Weapon Swing (Sword)_**
- **Player action:** Attack button. Can combo up to 3 hits.
- **Rules:** Works on all enemies, but Brutes block/absorb it mostly. Tricksters dodge it unless timed right.
- **Feedback:** Sharp metallic slash sounds, hit-stop on connect.
- **Notes:** Need to feel crisp and responsive.

**Mechanic 3: _Unarmed Wrestle_**
- **Player action:** Must put away sword first. Press grapple button near a Brute.
- **Rules:** Only works on Brutes. Puts player in a mini grapple-state. Can throw Brute into other enemies.
- **Feedback:** Heavy thud sounds, camera shake, visual dust impact on throw.
- **Notes:** The highlight of combat vs Brutes.

**Mechanic 4: _Lock-On_**
- **Player action:** Click right-stick/middle-mouse.
- **Rules:** Snaps camera to target, strafe around them. Breaks if they move too far.
- **Feedback:** Subtle UI reticle on the locked enemy.
- **Notes:** Crucial for 1v1 encounters within the wave.

**Mechanic 5: _Revive/Heal Partner_**
- **Player action:** Hold interact button near fallen partner.
- **Rules:** Takes 3 seconds, leaves player vulnerable to attacks.
- **Feedback:** Glowing light, rising sound effect, partner stands up.
- **Notes:** High risk, high reward.

---

### 6c. How mechanics connect

_Which mechanics feed into which? What’s the main “engine” of the game?_

- Mechanic ___ leads to / enables ___
- Mechanic ___ leads to / enables ___
- _Optional: one-line “flow” (e.g. “Move → Collect → Build → Defend”)_

---

### 6d. Win / lose / flow

- **How do you win?** Clear all waves. When the last marauder of the last wave is defeated, the gods have defended the realm.
- **How do you lose / fail?** Both gods are defeated (run over).
- **Session shape:** Single run: waves spawn from a spinning radius in the arena; clear all waves to win.

---

### 6e. 2-player specifics

- **Same screen or separate?** _Couch co-op one device, or two devices?_
- **Roles:** _Same abilities or different (e.g. one builder, one scout)?_
- **Cooperation vs competition:** _Fully coop, fully versus, or hybrid (e.g. shared goal but separate scores)?_
- **What makes it fun for both players at once?** _Why 2P and not 1P?_

---

## 7. Story / setting (if applicable)

_Brief: world, tone, main conflict or premise. “Story-light” or “no story” is fine._

- **Setting:** A realm of little people. You and your partner are two gods guarding it.
- **Tone:** Mythic, co-op, slightly playful (tricksters) and honorable (brutes who want a fair wrestle).
- **Premise / conflict:** Marauders are trying to take over. Two kinds:
  - **Tricksters:** Slippery, hard to catch. They dodge and feint; landing a hit feels earned.
  - **Brutes:** Big, tough, no weapons. They want to wrestle and ask you to put the weapons down. You can still use the sword, but fighting them unarmed is more enjoyable and thematically right.
- **Lore:** The gods defend the little people. Clearing all waves = the realm is safe (for now).
- **Restoration:** The duo can restore damage done by the marauders—to the realm and to themselves. When a wave is cleared, the gods’ wounds mend somewhat; when the realm is fully defended, the land and people heal. The little people feel this and respond.
- **After victory:** The NPCs (little people) congratulate the two gods, then begin to worship them. They gather, celebrate, and honor the pair who restored the realm. 

---

## 8. Audience

_Who is this for? Solo dev hobby project, itch audience, Steam, something else?_

- **Primary audience:** 
- **Platform(s) in mind:** 
- **Rough scope:** _e.g. 2–4 hour experience, endless, 10-level campaign_

---

## 9. Scope / MVP

_What’s the smallest version you’d still call “the game”? What’s in v1 and what’s “later”?_

**Must have for v1 (MVP):**
- 1 Arena map
- Basic player movement, sword attack, and dodge
- Wave Spawner system
- Trickster enemy (Basic dodge AI)
- Brute enemy (Basic charge AI + grapple prompt)
- 2-Player local co-op basic state

**Nice to have / later:**
- Deep wrestling mechanics (suplexes, environment throws)
- Extensive lore and narrative events
- Networked multiplayer
- Complex boss battles

---

## 10. Open questions

_Things to decide as you go._

- 
- 
- 

---

## 11. Notes & links

_Scratch space, references, art docs, tech notes._

- **Base character (UE5):** 3rd person C++ template **ClaCo** — `ClaCoCharacter.cpp` in this folder. Spring-arm camera (400), Enhanced Input (Move, Look, MouseLook, Jump), orient to movement, 500 walk speed. Use as reference; actual project lives in UE5 repo.
- **Tech checklist for 2P:** `tech-notes.md` — what to change for 2-player (second pawn, input, camera, spawn) and tunables to expose.
- **UE5 C++ scripts:** `suggested-scripts/ue5/` — full system (Game Mode, Game State, Player State, Player Controller, Character). Copy into your UE5 project; see `suggested-scripts/ue5/README.md`.
