// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatFlowSystem.generated.h"

/**
 * Flow/Style meter system inspired by Skate 4
 * Tracks combo chains, style points, and flow state
 */
UCLASS(ClassGroup=(CPPd1), meta=(BlueprintSpawnableComponent))
class UCombatFlowSystem : public UActorComponent
{
	GENERATED_BODY()

public:

	UCombatFlowSystem();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Add flow points (from tricks, combos, style moves) */
	UFUNCTION(BlueprintCallable, Category = "Combat|Flow")
	void AddFlowPoints(float Amount);

	/** Get current flow meter (0.0 to 1.0) */
	UFUNCTION(BlueprintPure, Category = "Combat|Flow")
	float GetFlowMeter() const { return CurrentFlow; }

	/** Get flow percentage (0-100) */
	UFUNCTION(BlueprintPure, Category = "Combat|Flow")
	float GetFlowPercentage() const { return CurrentFlow * 100.0f; }

	/** Check if in flow state (high flow meter) */
	UFUNCTION(BlueprintPure, Category = "Combat|Flow")
	bool IsInFlowState() const { return CurrentFlow >= FlowStateThreshold; }

	/** Get flow multiplier (affects damage, speed, etc.) */
	UFUNCTION(BlueprintPure, Category = "Combat|Flow")
	float GetFlowMultiplier() const;

	/** Get current combo count */
	UFUNCTION(BlueprintPure, Category = "Combat|Flow")
	int32 GetComboCount() const { return ComboCount; }

	/** Increment combo count */
	UFUNCTION(BlueprintCallable, Category = "Combat|Flow")
	void IncrementCombo();

	/** Reset combo count */
	UFUNCTION(BlueprintCallable, Category = "Combat|Flow")
	void ResetCombo();

protected:

	/** Maximum flow */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Flow", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float MaxFlow = 1.0f;

	/** Current flow */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Flow", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float CurrentFlow = 0.0f;

	/** Flow decay rate per second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Flow")
	float FlowDecayRate = 0.1f;

	/** Flow threshold for flow state */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Flow", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float FlowStateThreshold = 0.7f;

	/** Flow multiplier when in flow state */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Flow")
	float FlowStateMultiplier = 1.5f;

	/** Current combo count */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Flow")
	int32 ComboCount = 0;

	/** Time since last combo increment */
	float TimeSinceLastCombo = 0.0f;

	/** Time before combo resets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Flow")
	float ComboResetTime = 3.0f;

	virtual void BeginPlay() override;
};
