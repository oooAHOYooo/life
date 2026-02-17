# Game Design Document

**Working title:** P2G4W _(Project 2 Player Game for Wife)_  
**Last updated:** 2026-02-17  
**Status:** Draft  

**Today’s focus:** Core mechanics (sections 6a–6e below). Other sections can stay light for now.

---

## 1. Elevator pitch

_One or two sentences. What is this game? Why would someone play it?_

- 

---

## 2. Vision / high concept

_What’s the big idea? What feeling or experience are you aiming for?_

- 

---

## 3. Core loop

_What does the player do minute-to-minute? (e.g. “Explore → Find clue → Solve puzzle → Unlock new area → Repeat.”)_

1. 
2. 
3. 
_(repeat)_

---

## 4. Design pillars

_3–5 rules that guide every decision. If a feature doesn’t support these, cut it._

1. 
2. 
3. 
4. 
5. 

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
| 1 |          |                        |
| 2 |          |                        |
| 3 |          |                        |
| 4 |          |                        |
| 5 |          |                        |

---

### 6b. Mechanic deep-dive

_For each mechanic above: what does the player do? What are the rules? What’s the feedback?_

**Mechanic 1: _[name]_**
- **Player action:** _What does the player actually do (input, decision)?_
- **Rules:** _When does it work / not work? Limits?_
- **Feedback:** _What do they see/hear/feel when they use it?_
- **Notes:**

**Mechanic 2: _[name]_**
- **Player action:**
- **Rules:**
- **Feedback:**
- **Notes:**

**Mechanic 3: _[name]_**
- **Player action:**
- **Rules:**
- **Feedback:**
- **Notes:**

**Mechanic 4: _[name]_**
- **Player action:**
- **Rules:**
- **Feedback:**
- **Notes:**

**Mechanic 5: _[name]_**
- **Player action:**
- **Rules:**
- **Feedback:**
- **Notes:**

---

### 6c. How mechanics connect

_Which mechanics feed into which? What’s the main “engine” of the game?_

- Mechanic ___ leads to / enables ___
- Mechanic ___ leads to / enables ___
- _Optional: one-line “flow” (e.g. “Move → Collect → Build → Defend”)_

---

### 6d. Win / lose / flow

- **How do you win?** _Or: is there no “win,” just a session or endless loop?_
- **How do you lose / fail?** _Or: no lose state?_
- **Session shape:** _Single round, best of 3, timed run, open-ended, etc._

---

### 6e. 2-player specifics

- **Same screen or separate?** _Couch co-op one device, or two devices?_
- **Roles:** _Same abilities or different (e.g. one builder, one scout)?_
- **Cooperation vs competition:** _Fully coop, fully versus, or hybrid (e.g. shared goal but separate scores)?_
- **What makes it fun for both players at once?** _Why 2P and not 1P?_

---

## 7. Story / setting (if applicable)

_Brief: world, tone, main conflict or premise. “Story-light” or “no story” is fine._

- **Setting:** 
- **Tone:** 
- **Premise / conflict:** 

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
- 
- 
- 

**Nice to have / later:**
- 
- 

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
