// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatStaminaSystem.generated.h"

/**
 * Stamina system inspired by UFC 5
 * Manages stamina drain, recovery, and exhaustion states
 */
UCLASS(ClassGroup=(CPPo1), meta=(BlueprintSpawnableComponent))
class UCombatStaminaSystem : public UActorComponent
{
	GENERATED_BODY()

public:

	UCombatStaminaSystem();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Get current stamina (0.0 to 1.0) */
	UFUNCTION(BlueprintPure, Category = "Combat|Stamina")
	float GetStamina() const { return CurrentStamina; }

	/** Get stamina percentage (0-100) */
	UFUNCTION(BlueprintPure, Category = "Combat|Stamina")
	float GetStaminaPercentage() const { return CurrentStamina * 100.0f; }

	/** Drain stamina */
	UFUNCTION(BlueprintCallable, Category = "Combat|Stamina")
	void DrainStamina(float Amount);

	/** Check if has enough stamina for an action */
	UFUNCTION(BlueprintPure, Category = "Combat|Stamina")
	bool HasStamina(float RequiredAmount) const;

	/** Check if exhausted (stamina too low) */
	UFUNCTION(BlueprintPure, Category = "Combat|Stamina")
	bool IsExhausted() const { return CurrentStamina <= ExhaustionThreshold; }

	/** Get stamina recovery rate multiplier */
	UFUNCTION(BlueprintPure, Category = "Combat|Stamina")
	float GetRecoveryRate() const;

protected:

	/** Maximum stamina */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Stamina", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float MaxStamina = 1.0f;

	/** Current stamina */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Stamina", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float CurrentStamina = 1.0f;

	/** Base recovery rate per second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Stamina")
	float BaseRecoveryRate = 0.2f;

	/** Recovery rate when exhausted (slower) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Stamina")
	float ExhaustedRecoveryRate = 0.05f;

	/** Stamina threshold for exhaustion */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Stamina", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float ExhaustionThreshold = 0.2f;

	/** Time before stamina starts recovering after drain */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Stamina")
	float RecoveryDelay = 1.0f;

	/** Time since last stamina drain */
	float TimeSinceLastDrain = 0.0f;

	/** Stamina efficiency multiplier (from skill system) */
	float StaminaEfficiency = 1.0f;

	virtual void BeginPlay() override;
};
