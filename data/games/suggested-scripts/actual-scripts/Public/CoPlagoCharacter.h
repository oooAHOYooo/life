// Copyright Epic Games, Inc. All Rights Reserved.
// CoPlago Character – 3rd person, Move/Look/Jump, Zelda-style lock-on. Set mesh/anim in Blueprint.
// GDD 6a–6b: core mechanics (movement, camera, lock-on). Enemies/objects with tag CoPlagoLockOnTarget can be locked onto.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CoPlagoCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;

UCLASS()
class CoPlago_API ACoPlagoCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACoPlagoCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	/** Assign in Blueprint (Enhanced Input). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MouseLookAction;

	/** Lock-on (Zelda-style): hold to lock camera and face target. Assign in Blueprint. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* LockOnAction;

	/** Melee attack (e.g. sword). Assign in Blueprint. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* AttackAction;

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Movement (call from Blueprint or C++). */
	UFUNCTION(BlueprintCallable, Category = "CoPlago")
	void DoMove(float Right, float Forward);

	UFUNCTION(BlueprintCallable, Category = "CoPlago")
	void DoLook(float Yaw, float Pitch);

	UFUNCTION(BlueprintCallable, Category = "CoPlago")
	void DoJumpStart();

	UFUNCTION(BlueprintCallable, Category = "CoPlago")
	void DoJumpEnd();

	/** Lock onto nearest valid target (actor with tag CoPlagoLockOnTarget). */
	UFUNCTION(BlueprintCallable, Category = "CoPlago")
	void DoLockOn();

	UFUNCTION(BlueprintCallable, Category = "CoPlago")
	void DoUnlockOn();

	/** Current lock-on target (null if not locked). */
	UPROPERTY(BlueprintReadOnly, Category = "CoPlago")
	AActor* LockOnTarget;

	/** Max distance to consider for lock-on. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoPlago|LockOn")
	float LockOnRange = 2000.f;

	/** How quickly camera/character rotate toward target when locked (degrees per second). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoPlago|LockOn")
	float LockOnRotationRate = 720.f;

	/** Melee: damage applied to enemies in range. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoPlago|Combat")
	float MeleeDamage = 1.f;

	/** Melee: range in front of character. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoPlago|Combat")
	float MeleeRange = 150.f;

	/** Melee: radius for overlap (capsule/box). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoPlago|Combat")
	float MeleeRadius = 80.f;

	/** Perform melee attack: overlap in front, damage CoPlagoEnemy. Call from input or Blueprint. */
	UFUNCTION(BlueprintCallable, Category = "CoPlago")
	void DoMeleeAttack();

	// --- Player health and death ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoPlago|Health")
	float MaxHealth = 3.f;

	UPROPERTY(BlueprintReadOnly, Category = "CoPlago|Health")
	float Health = 3.f;

	/** Seconds before respawn after death. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoPlago|Health")
	float RespawnDelay = 2.f;

	UPROPERTY(BlueprintReadOnly, Category = "CoPlago|Health")
	bool bIsDead = false;

	/** Apply damage to this player. Returns true if player died. */
	UFUNCTION(BlueprintCallable, Category = "CoPlago")
	bool CoPlagoTakeDamage(float Damage);

	/** Override in Blueprint for death VFX/sound. Called when Health reaches 0. */
	UFUNCTION(BlueprintNativeEvent, Category = "CoPlago")
	void OnPlayerDeath();
	virtual void OnPlayerDeath_Implementation();

protected:
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void LockOnPressed(const FInputActionValue& Value);
	UFUNCTION()
	void LockOnReleased(const FInputActionValue& Value);

	UFUNCTION()
	void AttackTriggered(const FInputActionValue& Value);

	/** Find nearest actor with tag CoPlagoLockOnTarget in range and in front. */
	AActor* FindNearestLockOnTarget() const;
};
