# Cube Ninja model (C++)

A playable ninja made of **interlocking procedural shapes**: **spheres** for the head, pelvis, and joints (shoulders, elbows, hands, knees, feet); **tiny cubes** for the spine, chest, and limb segments. No external mesh; everything is generated in C++.

---

## What’s there

- **UCubeNinjaBodyComponent** – Builds the body from 22 parts:
  - **Spheres:** Head, Pelvis, L/R Shoulder, L/R Elbow, L/R Hand, L/R Knee, L/R Foot.
  - **Tiny cubes:** Spine, Chest, L/R Upper Arm, L/R Lower Arm, L/R Upper Leg, L/R Lower Leg.

  Hierarchy: Pelvis → Spine → Chest → Head; Chest → L/R Shoulder → Upper Arm → Elbow → Lower Arm → Hand; Pelvis → L/R Upper Leg → Knee → Lower Leg → Foot. Spheres and cubes are built with **UProceduralMeshComponent** and interlock at the joints.

- **ACubeNinjaCharacter** – A **NinjaCharacter** that uses this body as its visual:
  - Adds `CubeBody` (a `UCubeNinjaBodyComponent`) attached to the capsule.
  - Hides the skeletal mesh so only the cube body is drawn.
  - Same movement and combat as NinjaCharacter (double jump, flips, punch/kick, waves).

- **Procedural motion:** In **Tick**, the component drives a simple walk cycle: arm and leg cubes swing when the character is moving (speed from `CharacterMovement`). You can tune **Limb Swing Amount** and **Limb Swing Speed** on the component.

---

## Using it as the player character

1. **Game mode**  
   Set **Default Pawn Class** to **CubeNinjaCharacter** (or a Blueprint based on it).  
   Same as for NinjaCharacter: **NinjaGameMode** can be changed to use **CubeNinjaCharacter** as `DefaultPawnClass` if you want the cube ninja by default.

2. **Materials**  
   The procedural cubes use no material by default and can appear black. On your **CubeNinjaCharacter** Blueprint (or in code):
   - Select the **Cube Body** component.
   - In the Details panel, each cube is a child procedural mesh (e.g. under **CN_Pelvis** → **CN_PelvisM**). You can assign a **Material** to each, or in C++ set `PartMeshes[i]->SetMaterial(0, YourMaterial)` in **BeginPlay** after **RebuildBody**.

3. **Scale**  
   **Cube Body → Body Scale** (default 0.9 on CubeNinjaCharacter) controls overall size. Change it in the Blueprint or in the constructor.

4. **Feet alignment**  
   The cube body is offset so the bottom of the pelvis (and the legs) lines up with the capsule bottom; no extra setup needed.

---

## Using the body on another character

Add **Cube Ninja Body** to any Character (e.g. your existing NinjaCharacter Blueprint):

1. Open the Blueprint.
2. **Add Component** → search **Cube Ninja Body**.
3. Attach it to the **Capsule** (or root).
4. Set **Relative Location** Z so the feet are at the capsule bottom (e.g. **-96** if half height is 96).
5. Hide the skeletal mesh if you want only the cube body visible.

The component will tick and drive limb swing from the owner’s velocity when the owner is a **Character** with **CharacterMovement**.

---

## Files

- `Source/CPPd1/Procedural/CubeNinjaBodyComponent.h`
- `Source/CPPd1/Procedural/CubeNinjaBodyComponent.cpp`
- `Source/CPPd1/Variant_Combat/CubeNinjaCharacter.h`
- `Source/CPPd1/Variant_Combat/CubeNinjaCharacter.cpp`

Build the project, then set your game mode’s default pawn to **CubeNinjaCharacter** (or add **Cube Ninja Body** to another character) and assign materials so the cubes are visible.
