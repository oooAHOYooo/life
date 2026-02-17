# P2G4W – Suggested Scripts Guide

**For the team:** This guide explains where our game scripts live, how to update them in Cursor, and how to copy them into Unreal Engine 5 (UE5). The repo is the **source of truth** — we edit here, then paste into the UE5 project when ready.

---

## Will this update to the repo?

**Yes.** Everything in this folder lives in the **life** repo. When you:

1. **Edit** the scripts in Cursor (in `data/games/suggested-scripts/`)
2. **Save** and **commit/push** to the repo

…the repo is updated. Anyone (including your junior developer) can **pull** the latest and use the same scripts. There is no separate “script repo” — it’s all here.

---

## What's the difference between .h and .cpp?

In C++ (and UE5):

- **`.h` (header)** — Declarations only: class name, member variables, function **signatures** (name, parameters, return type). No function bodies. Other files `#include` the header to know what the class looks like. One header per class (usually).
- **`.cpp` (implementation)** — Definitions: the **actual code** for each function (the logic). Compiled once. Includes its `.h` and implements the declared functions.

**Rule of thumb:** Change the **interface** (new functions, new member variables) in the `.h`; write the **behavior** (how it works) in the `.cpp`. When you copy into UE5, copy **both** the `.h` and `.cpp` for each class.

---

## What are the “suggested scripts”?

We keep **basic script systems** for **P2G4W** (Project 2 Player Game for Wife) in this repo. They are **reference implementations** you can:

- **Update in Cursor** (with AI help, search, version control)
- **Copy-paste into UE5** when you’re ready to use them in the game project

Right now we have one set:

| System | Folder | What it is |
|--------|--------|------------|
| **UE5 C++** | `ue5/` | Game Mode (2-player spawn, round/restart), Game State, Player State, Player Controller, Character, and Goal Zone (trigger = win round). Copy all for a playable “first to the goal” prototype. |

You do **not** run these scripts inside the life repo. You **copy** them into your Unreal project and compile there.

---

## Workflow: Update in Cursor → Copy into UE5

### Step 1: Edit scripts in Cursor (this repo)

- Open the **life** repo in Cursor.
- Go to **`data/games/suggested-scripts/ue5/`**.
- Edit any `.h` or `.cpp` file (or add new ones).
- Save, commit, and push so the repo is up to date.

### Step 2: Copy into your UE5 project

- In the repo, open **`data/games/suggested-scripts/ue5/`**.
- Copy each file into your UE5 project as in the table below.  
  **Detailed list and paths:** see **`ue5/README.md`** in that same folder.

| In this repo | In your UE5 project |
|--------------|---------------------|
| `P2G4W.h` | `Source/YourModule/Public/P2G4W.h` |
| `P2G4W.cpp` | `Source/YourModule/Private/P2G4W.cpp` |
| `P2G4WGameModeBase.h` | `Source/YourModule/Public/` |
| `P2G4WGameModeBase.cpp` | `Source/YourModule/Private/` |
| … (GameState, PlayerState, PlayerController, Character, GoalZone) | Same: .h → Public, .cpp → Private |

Replace **YourModule** with your actual game module name (e.g. P2G4W or ClaCo).

### Step 3: Build and set up in UE5

1. **Build** the UE5 project (e.g. Ctrl+Alt+B or Build in your IDE).
2. In **Project Settings → Maps & Modes**, set **Default GameMode** to `P2G4WGameModeBase` (or your Blueprint child).
3. In your level, add two **Player Start** actors. Set **Player Start Tag** to `PlayerStart0` and `PlayerStart1`.
4. (Optional) Enable **Use Splitscreen** in Project Settings if you want two cameras.
5. Create a **Blueprint** based on `P2G4WCharacter` and assign your **Enhanced Input** actions (Jump, Move, Look, Mouse Look) in Class Defaults.

Full checklist and Build.cs dependencies are in **`ue5/README.md`**.

---

## For the junior developer (step-by-step)

1. **Get the latest scripts**  
   Pull the **life** repo so you have the latest `data/games/suggested-scripts/ue5/` files.

2. **Open the file list**  
   In the repo, open **`data/games/suggested-scripts/ue5/README.md`**. It has the exact table of which file goes in which UE5 folder (Public vs Private).

3. **Copy files into UE5**  
   For each file in `ue5/`, copy it to the path shown in that README. All `.h` files go to **Public**, all `.cpp` files go to **Private**.

4. **Check Build.cs**  
   Your UE5 module’s `.Build.cs` must list: Core, CoreUObject, Engine, EnhancedInput, InputCore, GameplayTasks. See `ue5/P2G4W.Build.cs.example` for an example.

5. **Compile in UE5**  
   Build the project. Fix any include or module name mismatches (e.g. if your module is not named P2G4W, replace the module name in the code).

6. **Set Game Mode and Player Starts**  
   As in Step 3 above: set default Game Mode to `P2G4WGameModeBase`, add two Player Starts with tags `PlayerStart0` and `PlayerStart1`.

7. **Character input**  
   Use a Blueprint based on `P2G4WCharacter` and assign the four Input Actions in Class Defaults so movement and camera work.

---

## Where everything lives (quick reference)

| In the life repo | Purpose |
|------------------|--------|
| `data/games/gdd.md` | Game Design Document (what we’re building). |
| `data/games/suggested-scripts/README.md` | Index of suggested script sets (right now: UE5). |
| `data/games/suggested-scripts/GUIDE.md` | **This guide** (workflow + copy-paste). |
| `data/games/suggested-scripts/ue5/` | All UE5 C++ files + `ue5/README.md` (file list and UE5 setup). |

After you run **`node compile-all.js`** from the repo root, this guide is also available as **`subpages/p2g4w-ue5-guide.html`** so you can read it in the browser and share the link.

---

## Summary

- **Repo = source of truth.** Edit scripts in Cursor in `data/games/suggested-scripts/`, commit and push; the repo updates.
- **Suggested scripts** = basic systems (currently UE5 C++) you can update here and then **copy-paste** into UE5.
- **Junior dev:** Pull repo → copy from `ue5/` into UE5 (using `ue5/README.md`) → build → set Game Mode and Player Starts → assign input on the character Blueprint.
