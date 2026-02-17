// Copyright Epic Games, Inc. All Rights Reserved.
// CoPlago Enemy – basic Zelda-style enemy: health, chase player, lock-on-able. Call CoPlagoTakeDamage from player attack.
// Add to wave spawner for "one wave at a time" groups.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CoPlagoEnemy.generated.h"

class ACoPlagoEnemyWaveSpawner;

UCLASS()
class CoPlago_API ACoPlagoEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ACoPlagoEnemy();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoPlago")
	float MaxHealth = 3.f;

	UPROPERTY(BlueprintReadOnly, Category = "CoPlago")
	float Health = 3.f;

	/** Chase speed toward player. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoPlago")
	float ChaseSpeed = 250.f;

	/** Distance at which enemy stops moving (avoid overlap). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoPlago")
	float StopDistance = 120.f;

	/** Contact damage to player when overlapping (Zelda-style touch damage). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoPlago")
	float ContactDamage = 1.f;

	/** Seconds between contact damage applications (so it's not per-frame). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoPlago")
	float ContactDamageInterval = 1.f;

	/** Radius to check for player overlap for contact damage. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoPlago")
	float ContactDamageRadius = 100.f;

	/** Called when this enemy dies (e.g. for spawner to track). */
	UFUNCTION(BlueprintCallable, Category = "CoPlago")
	void SetWaveSpawner(ACoPlagoEnemyWaveSpawner* Spawner);

	/** Apply damage; returns true if enemy died. Call from player melee/attack. */
	UFUNCTION(BlueprintCallable, Category = "CoPlago")
	bool CoPlagoTakeDamage(float Damage);

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	/** Override in Blueprint for death VFX/sound. */
	UFUNCTION(BlueprintNativeEvent, Category = "CoPlago")
	void OnEnemyDeath();
	virtual void OnEnemyDeath_Implementation();

protected:
	UPROPERTY()
	TWeakObjectPtr<ACoPlagoEnemyWaveSpawner> WaveSpawner;

	float LastContactDamageTime = -999.f;

	void ChasePlayer(float DeltaTime);
	void TryContactDamage(float CurrentTime);
};
