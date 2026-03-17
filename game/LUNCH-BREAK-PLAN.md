# Lunch Break Game Plan
## Goal: Basic prototype on Xbox by Monday 2/16/26

**Target:** One playable build running on your Xbox Series X: 2 players, one arena, move + basic attack. No polish—proof of concept only.

**Time available:** Lunch breaks (~30–45 min) Wed–Fri + weekend blocks. Plan assumes ~3 lunch sessions + 2–4 hours over the weekend.

---

## Pre-requisites (do once, before lunch Wed if possible)

- [ ] **Xbox Dev Mode** – On your Series X: enable Developer Mode (Settings → System, or Dev Mode app). Complete activation. Note your console’s IP.
- [ ] **UE5** – Installed and updated. Create a **new project** (Blank or Third Person template; we’ll strip it down). Enable **Xbox support** in project settings or install Xbox One/Series X platform support in Epic Launcher if needed.
- [ ] **PC ↔ Xbox** – Same network (Wi‑Fi or Ethernet). You’ll deploy from PC to Xbox by IP or via “Add device” in Dev Mode.

If any of this isn’t done yet, **Lunch Wed** can be “Dev Mode + UE5 project + first deploy” only.

---

## Day-by-day plan

### Wed 2/11 – Lunch: Project + Xbox target + first deploy

**Goal:** New UE5 project that builds and runs on Xbox.

| Step | Task | Time |
|------|------|------|
| 1 | New UE5 project (Third Person or Blank). Name it (e.g. `CoopCombat`). | 5 min |
| 2 | In **Project Settings → Platforms → Xbox One/Series**: enable Xbox, set dev kit / Dev Mode device if needed. Save. | 5 min |
| 3 | **Package** for Xbox Series X (or “Xbox One” if that’s the only target). Deploy to your console (IP or paired device). | 15–20 min |
| 4 | Launch on Xbox. Confirm: template character moves with controller. | 5 min |

**Done when:** You see the default template (e.g. character on a floor) running on your TV from the Xbox. If packaging fails, use the rest of lunch to fix (driver, SDK path, or Epic docs).

---

### Thu 2/12 – Lunch: Second player + one arena

**Goal:** 2P local in one level. Both players can move. One shared arena.

| Step | Task | Time |
|------|------|------|
| 1 | **Local 2P** – Enable a second gamepad. In UE5: either use “Split Screen” in viewport/player settings or add a second Player Controller that spawns a second pawn. (Blueprint: “Create Player” or C++ equivalent for local P2.) | 15 min |
| 2 | **Two pawns** – Duplicate the default character blueprint (or pawn). P1 controls Pawn A, P2 controls Pawn B. Spawn both in the level. | 10 min |
| 3 | **One arena** – Simple floor + walls (or use a Basic level block). No art; grey box is fine. Both players spawn here. | 5 min |
| 4 | **Build + deploy** to Xbox. Test: two characters, two controllers, both moving in the same arena. | 10 min |

**Done when:** You and your wife can move two characters in one level on the Xbox. Camera: split-screen or shared (your choice for prototype).

---

### Fri 2/13 – Lunch: One attack + “something to hit”

**Goal:** At least one attack input; something that can be “hit” (e.g. dummy or crate with health).

| Step | Task | Time |
|------|------|------|
| 1 | **One attack** – Map a button (e.g. X or RB) to “attack.” In Blueprint: On Press → play a simple montage or apply damage in a sphere/box in front of character. No flick stick yet; one button = one punch/slash. | 15 min |
| 2 | **Target** – Place a simple actor (cube, or character blueprint) with a **Health** component or variable. On overlap/hit from attack, reduce health. When health ≤ 0, destroy or play “defeated.” | 10 min |
| 3 | **Feedback** – Optional: hit sound or tiny camera shake. Skip if time is short. | 5 min |
| 4 | **Build + deploy.** Test on Xbox: both players can move and attack; attacking the target reduces its health / destroys it. | 10 min |

**Done when:** 2P can move, attack with one button, and defeat at least one “enemy” (dummy/crate) on Xbox.

---

### Weekend 2/14–2/15 – Polish prototype for Monday

**Goal:** “Basic prototype” you’re happy to call V0: stable, clear, and runnable on Xbox.

| Priority | Task |
|----------|------|
| 1 | **Stability** – No crashes on start or when both attack. Fix any packaging or input bugs. |
| 2 | **Clarity** – Simple UI: “P1” / “P2” or health bars if you add player health. Optional: “Defeat the dummy to win” text. |
| 3 | **Health for players (optional)** – If time: give each player a health value; something in the arena can damage them. When one dies, “Game Over” and restart. Connects to GDD “over = over.” |
| 4 | **One more deploy** – Final build to Xbox. Playtest together. Note what feels good and what to do next (flick stick, timer, style, etc.). |

**Done when:** You have one build on the Xbox that you can show as “our 2P co-op combat prototype” and a short list of next features.

---

## Monday 2/16 – Prototype “done”

- [ ] Final build on Xbox.
- [ ] 5–10 min playtest with your wife: move, attack, beat the dummy (and each other if you added PvP for fun).
- [ ] Jot down: one thing that felt good, one thing to do next (from GDD V1: flick stick, timer, style grade, etc.).

---

## If you fall behind

- **Wed:** If first deploy doesn’t work, don’t add 2P yet. Get “template on Xbox” solid. Move 2P to Thu or weekend.
- **Thu:** Minimum = 2P moving in one level. Skip pretty arena; use default floor.
- **Fri:** Minimum = one attack that does damage to something. Can be a single crate with 1 HP.
- **Weekend:** Minimum = one stable build on Xbox. Everything else is bonus.

---

## After Monday

- **V1 (next):** Flick stick for attacks, timer, “time or health = over,” one style grade, one real “spot” with one enemy type. See GDD §7.

---

*Plan lives in `life/game/`. Adjust dates if your “next Monday” is different (e.g. 2/13).*
