# P2G4W – Tech notes

Quick checklist for when you’re back in the UE5 project. Base: **ClaCo** 3rd person C++ template (`ClaCoCharacter.cpp`).

---

## Template updates to consider

### 2-player
- [ ] **Second pawn** — Second character class or same `AClaCoCharacter` with Player 1 / Player 2 controller. Use `UGameplayStatics::CreatePlayer` or second PlayerController + Possess.
- [ ] **Input** — Each player needs own mapping. Enhanced Input: separate Input Mapping Context per player or one context with different keys (e.g. WASD vs arrows / gamepad 2). Ensure `AddMappingContext` is per LocalPlayer.
- [ ] **Camera** — Same screen: split-screen (UE5 has built-in) or one shared camera that frames both. Decide in GDD 6e (same screen vs separate).
- [ ] **Spawn** — Two player starts or spawn logic for P1 and P2 so both have a character in the level.

### Feel & tuning (no code required)
- Expose in Blueprint or Editor so you can tweak without recompile:
  - **MaxWalkSpeed** (500)
  - **JumpZVelocity** (500)
  - **CameraBoom->TargetArmLength** (400)
  - **RotationRate** (500 yaw)
  - **AirControl** (0.35)

### Code hygiene (optional)
- [ ] **DoMove / DoLook** — Already good for Blueprint override or a subclass. Keep them; use for P2-specific behavior if needed.
- [ ] **ClaCoCharacter.h** — If you add P2-specific logic, consider a subclass (e.g. `AP2G4WCharacter`) so the template stays reusable.

---

## Reference

- Character source (reference copy): `data/games/ClaCoCharacter.cpp`
- GDD 6e: 2-player specifics (same screen, roles, coop vs versus)
