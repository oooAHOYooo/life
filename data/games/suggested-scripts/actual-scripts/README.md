# Actual Scripts (CoPlago)

This folder contains the **transformed** C++ scripts ready for your UE5 project.

## What is this?

- **Source:** `suggested-scripts/ue5/` (contains P2G4W templates)
- **Transformed:** `actual-scripts/` (contains CoPlago versions)
- **Destination:** Your UE5 project at `Source/CoPlago/`

## How it works

1. Edit files in `suggested-scripts/ue5/` (with P2G4W names)
2. Run `node sync-to-ue.js` from the `ue5/` folder
3. Script transforms P2G4W → CoPlago and creates this folder
4. Script then syncs to your UE5 project automatically

## Structure

```
actual-scripts/
├── Public/     (header files .h)
└── Private/    (implementation files .cpp)
```

These files are **automatically generated** - don't edit them directly. Edit the source files in `suggested-scripts/ue5/` instead.

## Files

All files have been transformed from `P2G4W*` to `CoPlago*`:
- Module name: `P2G4W` → `CoPlago`
- Class names: `P2G4WCharacter` → `CoPlagoCharacter`
- Includes: `#include "P2G4W.h"` → `#include "CoPlago.h"`
- Log categories: `LogP2G4W` → `LogCoPlago`

## Usage

Just run the sync script - it handles everything:

```bash
cd data/games/suggested-scripts/ue5
node sync-to-ue.js
```

Or from the repo root:
```bash
npm run sync-ue5
```
