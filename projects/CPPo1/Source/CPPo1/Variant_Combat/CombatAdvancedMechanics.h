// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatAdvancedMechanics.generated.h"

/** Parry timing window result */
UENUM(BlueprintType)
enum class EParryResult : uint8
{
	Missed,			// Too early/late
	Perfect,		// Perfect timing
	Good,			// Good timing
	Late			// Late but still successful
};

/**
 * Advanced combat mechanics inspired by UFC 5
 * Handles parrying, blocking, counter-attacks, and timing-based mechanics
 */
UCLASS(ClassGroup=(CPPo1), meta=(BlueprintSpawnableComponent))
class UCombatAdvancedMechanics : public UActorComponent
{
	GENERATED_BODY()

public:

	UCombatAdvancedMechanics();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Attempt to parry an incoming attack */
	UFUNCTION(BlueprintCallable, Category = "Combat|Advanced")
	EParryResult AttemptParry();

	/** Start blocking */
	UFUNCTION(BlueprintCallable, Category = "Combat|Advanced")
	void StartBlocking();

	/** Stop blocking */
	UFUNCTION(BlueprintCallable, Category = "Combat|Advanced")
	void StopBlocking();

	/** Check if currently blocking */
	UFUNCTION(BlueprintPure, Category = "Combat|Advanced")
	bool IsBlocking() const { return bIsBlocking; }

	/** Check if in parry window */
	UFUNCTION(BlueprintPure, Category = "Combat|Advanced")
	bool IsInParryWindow() const;

	/** Perform a counter-attack after successful parry */
	UFUNCTION(BlueprintCallable, Category = "Combat|Advanced")
	void PerformCounterAttack();

	/** Get block effectiveness (damage reduction) */
	UFUNCTION(BlueprintPure, Category = "Combat|Advanced")
	float GetBlockEffectiveness() const;

protected:

	/** Is currently blocking */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Advanced")
	bool bIsBlocking = false;

	/** Parry window duration (perfect timing) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Advanced")
	float PerfectParryWindow = 0.1f;

	/** Good parry window duration */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Advanced")
	float GoodParryWindow = 0.2f;

	/** Time since parry input */
	float ParryInputTime = -1.0f;

	/** Block stamina drain per second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Advanced")
	float BlockStaminaDrain = 0.1f;

	/** Base block effectiveness (damage reduction) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Advanced")
	float BaseBlockEffectiveness = 0.5f;

	/** Perfect parry damage multiplier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Advanced")
	float PerfectParryDamageMultiplier = 2.0f;

	/** Counter-attack window after parry */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Advanced")
	float CounterAttackWindow = 1.0f;

	/** Time since last parry */
	float TimeSinceLastParry = -1.0f;

	/** Can perform counter-attack */
	bool bCanCounterAttack = false;

	virtual void BeginPlay() override;
};
