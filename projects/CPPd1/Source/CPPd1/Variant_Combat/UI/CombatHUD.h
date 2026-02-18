// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CPPd1.h"
#include "Blueprint/UserWidget.h"
#include "CombatHUD.generated.h"

class ACombatCharacter;
class UCombatStaminaSystem;
class UCombatFlowSystem;
class UProgressBar;
class UTextBlock;

/**
 * Main HUD widget for combat - displays health, stamina, flow, wave info
 * Create a Blueprint from this to design the visual layout
 */
UCLASS(BlueprintType, Blueprintable)
class CPPd1_API UCombatHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UCombatHUD(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** Set the character to display stats for */
	UFUNCTION(BlueprintCallable, Category = "Combat|HUD")
	void SetCharacter(ACombatCharacter* Character);

	/** Update health bar (0.0 to 1.0) */
	UFUNCTION(BlueprintCallable, Category = "Combat|HUD")
	void UpdateHealth(float HealthPercent);

	/** Update stamina bar (0.0 to 1.0) */
	UFUNCTION(BlueprintCallable, Category = "Combat|HUD")
	void UpdateStamina(float StaminaPercent);

	/** Update flow meter (0.0 to 1.0) */
	UFUNCTION(BlueprintCallable, Category = "Combat|HUD")
	void UpdateFlow(float FlowPercent);

	/** Update wave info */
	UFUNCTION(BlueprintCallable, Category = "Combat|HUD")
	void UpdateWaveInfo(int32 CurrentWave, int32 EnemiesRemaining);

	/** Show/hide wave info */
	UFUNCTION(BlueprintCallable, Category = "Combat|HUD")
	void SetWaveInfoVisible(bool bVisible);

protected:
	/** The character we're displaying stats for */
	UPROPERTY(BlueprintReadOnly, Category = "Combat|HUD")
	TObjectPtr<ACombatCharacter> TargetCharacter;

	/** Health bar widget (set in Blueprint) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Combat|HUD")
	TObjectPtr<UProgressBar> HealthBar;

	/** Stamina bar widget (set in Blueprint) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Combat|HUD")
	TObjectPtr<UProgressBar> StaminaBar;

	/** Flow meter bar widget (set in Blueprint) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Combat|HUD")
	TObjectPtr<UProgressBar> FlowBar;

	/** Wave number text (set in Blueprint) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Combat|HUD")
	TObjectPtr<UTextBlock> WaveNumberText;

	/** Enemies remaining text (set in Blueprint) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Combat|HUD")
	TObjectPtr<UTextBlock> EnemiesRemainingText;

	/** Wave info panel (set in Blueprint) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Combat|HUD")
	TObjectPtr<UWidget> WaveInfoPanel;

	/** Auto-update from character components */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|HUD")
	bool bAutoUpdate = true;

	/** Update rate (how often to refresh from character) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|HUD", meta = (ClampMin = 0.0f))
	float UpdateRate = 0.1f; // Update 10 times per second

	float LastUpdateTime = 0.0f;

	void UpdateFromCharacter();
};
