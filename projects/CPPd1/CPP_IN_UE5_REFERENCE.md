# C++ in UE5: What Auto-Works & Building 3D in Code

Quick reference for CPPd1: which C++ types show up in the editor automatically, and how to create 3D geometry in code.

---

## 1. C++ files that “automatically work” in UE5

Any C++ class in your **CPPd1** module (under `Source/CPPd1/`) that uses UE’s reflection macros is picked up by the editor after a successful compile. You don’t register files by hand.

### Rules

- **Same module:** `.h` / `.cpp` live under `Source/CPPd1/` (or a subfolder; add the folder to `PublicIncludePaths` in `CPPd1.Build.cs` if you use a new folder like `Procedural/`).
- **Reflection:** Use `UCLASS()`, `UPROPERTY()`, `UFUNCTION()` (and optionally `USTRUCT()`, `UENUM()`) so the class appears in the editor and Blueprints.
- **Base class:** Inherit from an engine type (`AActor`, `UActorComponent`, `AGameModeBase`, `UObject`, etc.).

### Types you can add (they’ll just work)

| Kind | Base class | Use |
|------|------------|-----|
| **Placeable actor** | `AActor` | Drag into level (e.g. `ACPPd1ProceduralCube`). |
| **Component** | `UActorComponent`, `USceneComponent` | Add to actors in C++ or Blueprint. |
| **Character** | `ACharacter` | Player or AI pawn. |
| **Game mode** | `AGameModeBase` | PIE / packaged game. |
| **Game state** | `AGameStateBase` | Replicated / shared state. |
| **Player state** | `APlayerState` | Per-player data. |
| **Player controller** | `APlayerController` | Input and UI. |
| **Subsystem** | `UWorldSubsystem`, `UGameInstanceSubsystem` | Global services. |
| **Data asset** | `UDataAsset` | Design-time data. |
| **Interface** | `UInterface` (with `UINTERFACE`) | “Can do X” contracts. |
| **Struct** | `USTRUCT()` | Blueprint / replication structs. |
| **Enum** | `UENUM()` | Dropdowns and flags. |

Example for a new actor: create `MyNewActor.h` / `MyNewActor.cpp` with `UCLASS(BlueprintType, Blueprintable)`, inherit from `AActor`, put them in `Source/CPPd1/` (or a subfolder with include path), compile → class appears in **Place Actors** and in **Blueprint Parent Class**.

---

## 2. Building 3D objects with code

Yes — you can create 3D geometry entirely in C++.

### Options

| Approach | Use |
|----------|-----|
| **Procedural Mesh Component** | Full control: vertices, triangles, normals, UVs. Good for cubes, grids, terrain, custom shapes. |
| **Spawn `UStaticMesh`** | Use existing assets; set transform, scale, material at runtime. |
| **Instanced Static Mesh** | Many copies of one mesh (e.g. foliage, bullets) with one draw call. |
| **Spline + Spline Mesh** | Shape a path in code; mesh follows the spline. |
| **Geometry Script (UE5)** | Higher-level procedural geometry; can be used from C++ where exposed. |

### Example in this project: procedural cube

- **Class:** `ACPPd1ProceduralCube` in `Source/CPPd1/Procedural/`.
- **What it does:** Uses `UProceduralMeshComponent` to build a cube from vertices and triangles in `BuildCubeMesh()`.
- **Usage:** Place the actor in the level (search “ProceduralCube” in Place Actors). Change **Size** in the Details panel; use **Rebuild Mesh** (or move the actor) to refresh.
- **Dependency:** **ProceduralMeshComponent** plugin is enabled in the project and in `CPPd1.Build.cs`.

To go further:

- Add more sections in `ProceduralMesh` for multiple materials.
- Generate other shapes (sphere, cylinder, grid, heightfield) by filling `Vertices` / `Triangles` / `Normals` / `UV0`.
- Assign a material: `ProceduralMesh->SetMaterial(0, YourMaterial)`.

---

## 3. Where to put new C++ files

- **General / shared:** `Source/CPPd1/` (e.g. `CPPd1ProceduralCube` could live here; we use `Procedural/` to group).
- **Variant-specific:** `Source/CPPd1/Variant_Combat/`, `Variant_Platforming/`, `Variant_SideScrolling/` (and subfolders already in `PublicIncludePaths`).
- **New folder:** Add to `PublicIncludePaths` in `CPPd1.Build.cs`, e.g. `"CPPd1/Procedural"`.

No need to list each `.cpp` in the build; Unreal Build Tool discovers them in the module.
