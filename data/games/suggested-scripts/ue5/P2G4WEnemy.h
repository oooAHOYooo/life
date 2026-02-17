// Copyright Epic Games, Inc. All Rights Reserved.
// P2G4W Enemy – basic Zelda-style enemy: health, chase player, lock-on-able. Call P2G4WTakeDamage from player attack.
// Add to wave spawner for "one wave at a time" groups.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P2G4WEnemy.generated.h"

class AP2G4WEnemyWaveSpawner;

UCLASS()
class P2G4W_API AP2G4WEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AP2G4WEnemy();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "P2G4W")
	float MaxHealth = 3.f;

	UPROPERTY(BlueprintReadOnly, Category = "P2G4W")
	float Health = 3.f;

	/** Chase speed toward player. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "P2G4W")
	float ChaseSpeed = 250.f;

	/** Distance at which enemy stops moving (avoid overlap). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "P2G4W")
	float StopDistance = 120.f;

	/** Contact damage to player when overlapping (Zelda-style touch damage). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "P2G4W")
	float ContactDamage = 1.f;

	/** Seconds between contact damage applications (so it's not per-frame). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "P2G4W")
	float ContactDamageInterval = 1.f;

	/** Radius to check for player overlap for contact damage. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "P2G4W")
	float ContactDamageRadius = 100.f;

	/** Called when this enemy dies (e.g. for spawner to track). */
	UFUNCTION(BlueprintCallable, Category = "P2G4W")
	void SetWaveSpawner(AP2G4WEnemyWaveSpawner* Spawner);

	/** Apply damage; returns true if enemy died. Call from player melee/attack. */
	UFUNCTION(BlueprintCallable, Category = "P2G4W")
	bool P2G4WTakeDamage(float Damage);

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	/** Override in Blueprint for death VFX/sound. */
	UFUNCTION(BlueprintNativeEvent, Category = "P2G4W")
	void OnEnemyDeath();
	virtual void OnEnemyDeath_Implementation();

protected:
	UPROPERTY()
	TWeakObjectPtr<AP2G4WEnemyWaveSpawner> WaveSpawner;

	float LastContactDamageTime = -999.f;

	void ChasePlayer(float DeltaTime);
	void TryContactDamage(float CurrentTime);
};
