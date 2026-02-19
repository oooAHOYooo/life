# Workflow: Do the work in Cursor / Claude / Antigravity → UE5 updates

Use **Cursor** (and Claude, Antigravity, etc.) as your main place to edit code and design. The **life repo** is the single source of truth. When you build from here, UE5 picks up the changes—either by hot-reload (Live Coding) or the next time you open the project.

---

## The idea

1. **Edit in Cursor** (or Claude, Antigravity): change C++ under `projects/CPPd1/Source/`, add docs, tweak config. The life repo is the only place you edit.
2. **Build from the repo**: run the build script from Cursor’s terminal (or from your repo root).
3. **UE5 reflects it**:  
   - If the editor is **open** with this project and **Live Coding** is on → compile runs and UE5 hot-reloads.  
   - If the editor is **closed** → next time you open the project and hit Play, the new code is already there.

So: **most work in Cursor/Claude/Antigravity → pipe into UE5 by building → UE5 “automatically” has the latest code** (either via Live Coding or on next open).

---

## One-time setup

### 1. Open the project from the life repo in UE5

- In UE5: **File → Open Project** and choose  
  `…/life/projects/CPPd1/CPPd1.uproject`  
  (where `life` is your repo root).
- Don’t use a copy under “Documents\Unreal Projects” as the main project; the **canonical** project is inside the repo.

### 2. Turn on Live Coding (optional but recommended)

- **Edit → Editor Preferences → General → Live Coding**
- Enable **Enable Live Coding**.
- Then when you build from Cursor, you can leave the editor open and it will hot-reload after a successful compile.

### 3. Tell the build script where UE5 is

**Windows (PowerShell):**

```powershell
$env:UE5_ROOT = "C:\Program Files\Epic Games\UE_5.6"
```

(Use your real install path, e.g. `UE_5.5` if that’s what you have.)

**Mac (bash/zsh):**

```bash
export UE5_ROOT=~/UnrealEngine
```

Put that in your profile or run it once per terminal before building.

---

## Daily workflow

### Option A: Build after you’re done editing (simplest)

1. Edit in Cursor (CPPd1 C++, docs, config).
2. Save.
3. In Cursor’s terminal (from **repo root**):

   ```bash
   npm run build-cppd1
   ```

   (Or run the script directly: `.\projects\CPPd1\Scripts\build-cppd1.ps1` on Windows, `./projects/CPPd1/Scripts/build-cppd1.sh` on Mac.)

4. If UE5 is open with this project and Live Coding is on → it compiles and hot-reloads.  
   If UE5 is closed → next time you open the project, the new code is already built.

### Option B: Auto-build on save (optional)

If you want “save in Cursor → build runs automatically”:

- Run the **watch-and-build** script in a terminal and leave it running while you work (from repo root):

  ```bash
  npm run watch-cppd1
  ```

  It watches `projects/CPPd1/Source/` and runs the platform build when `.cpp` or `.h` files change. Same `UE5_ROOT` (or `UE_ROOT`) env var as above.

---

## What “automatically changes” in UE5

- **C++**: Only after a **compile**. So “automatically” means: you run the build (or the watcher runs it), and then either Live Coding applies the new binary in the open editor, or the next time you open the project you’re already on the new code. UE5 does not compile by itself when you save files in Cursor.
- **Content (Blueprints, maps, assets)**: Edited in the UE5 editor; the life repo can store them if you commit the Content folder. No “pipe from Cursor” for Blueprint logic—that stays in the editor.
- **Config (e.g. DefaultEngine.ini)**: If you edit these in Cursor and save, they’re just files on disk; UE5 will use them the next time the project loads. No build needed.

So the main “pipe” is: **edit in Cursor → run build script → UE5 gets new C++**. Live Coding makes that feel automatic when the editor is open.

---

## Summary

| Where you work | What you do | What happens in UE5 |
|----------------|-------------|----------------------|
| Cursor / Claude / Antigravity | Edit `projects/CPPd1/Source/**` and docs | Nothing until you build |
| Cursor terminal | Run `build-cppd1.ps1` or `build-cppd1.sh` | Compile runs; if Live Coding on, editor hot-reloads |
| Optional: run `watch-and-build.js` | Edit and save C++ | Build runs on save; UE5 hot-reloads if open |
| UE5 editor | Edit Blueprints, levels, assets | Normal; save in editor |

Single source of truth: **life repo** at `projects/CPPd1/`. Open that `.uproject` in UE5 and build from the repo so changes you make in Cursor automatically end up in UE5 after each build.
