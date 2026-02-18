# CPPd1 Editor Scripts

Scripts here run **inside the Unreal Editor** (they are not standalone). They help automate setup that would otherwise be done by hand.

## Requirements

- **Editor Scripting Utilities** plugin: **Edit → Plugins** → search "Editor Scripting" → enable **Editor Scripting Utilities**.
- Optional: **Python Editor Script Plugin** if you want to run `.py` files from the editor.

## add_two_player_starts.py

Adds **2 Player Start** actors to the **current level** so you don’t have to place them manually for 2-player support.

### How to run

1. Open your level in the editor.
2. Run the script in one of these ways:
   - **Tools → Execute Python Script** and choose `add_two_player_starts.py`, or
   - **Window → Developer Tools → Output Log** → switch to **Cmd** tab and run:
     ```  
     py "c:/path/to/CPPd1/Scripts/add_two_player_starts.py"
     ```
3. **Save the level** (Ctrl+S) so the new Player Starts are kept.

### Options (edit inside the script)

- `PLAYER_0_OFFSET` / `PLAYER_1_OFFSET`: position offset from world origin (Y = left/right, X = forward).
- `BASE_Z`: height above origin.
- `ADD_IF_ALREADY_PRESENT`: if `True`, always adds 2 more Player Starts; if `False`, does nothing when the level already has 2 or more.

### Note

Player index (0 or 1) is assigned by the game mode at runtime based on spawn order; you can rename or move the actors in the level after running the script.
