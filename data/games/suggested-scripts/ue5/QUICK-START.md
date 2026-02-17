# Quick Start: Sync & Play Workflow

## The Answer to "Will it auto-update when I hit Play?"

**Short answer:** Almost! You need to **compile** after syncing, but if UE5 is already open, it's just one click.

## Two Scenarios

### Scenario A: UE5 Editor is Already Open (Recommended)

1. **Edit** your C++ files in `suggested-scripts/ue5/`
2. **Run sync:** `node sync-to-ue.js` (or `npm run sync-ue5`)
3. **UE5 detects changes** - you'll see a notification or the "Compile" button lights up
4. **Click "Compile"** in UE5 (or press `Ctrl+Alt+B`)
5. **Hit Play** - your changes are live!

**Time:** ~10-30 seconds (just compile time)

### Scenario B: UE5 Editor is Closed

1. **Edit** your C++ files in `suggested-scripts/ue5/`
2. **Run sync:** `node sync-to-ue.js`
3. **Open UE5 project**
4. **Build** (Ctrl+Alt+B) - this compiles all C++ code
5. **Hit Play** - your changes are live!

**Time:** ~1-2 minutes (full build)

---

## Recommended Workflow (Fastest)

**Keep UE5 editor open while developing:**

1. Open UE5 project once
2. Keep it running in the background
3. Edit C++ in Cursor → Run sync → Click Compile in UE5 → Play

This way you skip the "open project" step and UE5's hot-reload works.

---

## What Happens When You Sync?

The sync script:
1. ✅ Copies C++ files to your UE5 project
2. ✅ Transforms P2G4W → CoPlago automatically
3. ✅ Files are ready to compile

**But:** UE5 doesn't automatically compile. You need to:
- Click "Compile" button (if editor is open)
- Or build the project (if editor is closed)

---

## Hot Reload vs Full Build

- **Hot Reload** (editor open): Compiles only changed files, ~10-30 seconds
- **Full Build** (editor closed): Compiles everything, ~1-2 minutes

**Pro tip:** Keep UE5 open for faster iteration!

---

## Troubleshooting

### "Changes not showing up"
- Make sure you clicked "Compile" in UE5 after syncing
- Check for compile errors in UE5's Output Log
- Try closing and reopening UE5 if hot-reload isn't working

### "UE5 says files are modified outside editor"
- This is normal! Click "Reload" or "Compile"
- UE5 detected the sync script updated the files

### "Build errors"
- Make sure all dependencies are in `.Build.cs` (see README.md)
- Check that module name is correct (CoPlago, not P2G4W)
- Delete `Intermediate` and `Binaries` folders, then rebuild

---

## Summary

**Sync script** = Copies files ✅  
**UE5 Compile** = Makes them runnable ⚙️  
**Play** = Tests your changes 🎮

**Workflow:** Edit → Sync → Compile → Play

Keep UE5 open for the fastest workflow!
