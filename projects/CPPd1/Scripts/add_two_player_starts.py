# add_two_player_starts.py
# Run this script INSIDE the Unreal Editor (with Editor Scripting Utilities / Python enabled).
# It adds 2 Player Start actors to the current level so you don't have to place them by hand.
#
# How to run:
#   1. Enable: Edit -> Plugins -> search "Python" -> enable "Editor Scripting Utilities" (and "Python Editor Script Plugin" if needed).
#   2. Open your level.
#   3. Window -> Developer Tools -> Output Log (or run from Tools -> Execute Python Script).
#   4. In Output Log, switch to "Cmd" and run: py "path/to/add_two_player_starts.py"
#      Or: File -> Execute Python Script and select this file.
#
# Options (edit below):
#   - SPAWN_OFFSET: distance in front of world origin for each start (X = forward, Y = left/right).
#   - If the level already has 2+ Player Starts, the script can skip or add extras; set ADD_IF_ALREADY_PRESENT to True to always add 2 more.

import unreal

# --- Config (edit these) ---
# Offset from (0,0,100) for each player. Player 0 = -Y, Player 1 = +Y so they're side by side.
PLAYER_0_OFFSET = unreal.Vector(0.0, -200.0, 0.0)   # 200 cm left
PLAYER_1_OFFSET = unreal.Vector(0.0,  200.0, 0.0)   # 200 cm right
BASE_Z = 100.0  # height above origin
# If True, always add 2 new Player Starts. If False, only add if current level has fewer than 2.
ADD_IF_ALREADY_PRESENT = False

def main():
    editor_subsystem = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem)
    if not editor_subsystem:
        unreal.log_error("UnrealEditorSubsystem not found. Run this script from the Editor.")
        return

    world = editor_subsystem.get_editor_world()
    if not world:
        unreal.log_error("No editor world. Open a level first.")
        return

    # Count existing Player Starts
    existing = unreal.GameplayStatics.get_all_actors_of_class(world, unreal.PlayerStart)
    count = len(existing)
    if count >= 2 and not ADD_IF_ALREADY_PRESENT:
        unreal.log("Level already has {} Player Start(s). Skipping. Set ADD_IF_ALREADY_PRESENT=True to add more.".format(count))
        return

    # Spawn into current editor level (Editor Scripting Utilities plugin)
    editor_level_lib = unreal.EditorLevelLibrary()
    base = unreal.Vector(0.0, 0.0, BASE_Z)
    spawns = [
        ("PlayerStart_0", unreal.Vector(
            base.x + PLAYER_0_OFFSET.x,
            base.y + PLAYER_0_OFFSET.y,
            base.z + PLAYER_0_OFFSET.z
        )),
        ("PlayerStart_1", unreal.Vector(
            base.x + PLAYER_1_OFFSET.x,
            base.y + PLAYER_1_OFFSET.y,
            base.z + PLAYER_1_OFFSET.z
        )),
    ]

    for name, location in spawns:
        actor = editor_level_lib.spawn_actor_from_class(unreal.PlayerStart, location, unreal.Rotator(0.0, 0.0, 0.0))
        if actor:
            actor.set_actor_label(name)
            unreal.log("Spawned: {} at {}".format(name, location))
        else:
            unreal.log_error("Failed to spawn {}".format(name))

    unreal.log("Done. Save the level (Ctrl+S) to keep the new Player Starts.")

if __name__ == "__main__":
    main()
