// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CPPd1.h"
#include "Variant_Combat/CombatPlayerController.h"
#include "Variant_Combat/UI/ControlsOverlay.h"
#include "NinjaPlayerController.generated.h"

/**
 * Player controller for the ninja game. Uses NinjaCharacter for spawn and respawn (P1, P2, ghost).
 * Handles input mode selection and controls overlay.
 */
UCLASS()
class CPPd1_API ANinjaPlayerController : public ACombatPlayerController
{
	GENERATED_BODY()

public:
	ANinjaPlayerController();

	virtual void BeginPlay() override;

	/** Input mode selection widget class */
	UPROPERTY(EditAnywhere, Category = "UI|Input Selection")
	TSubclassOf<class UUserWidget> InputModeSelectionWidgetClass;

	/** Controls overlay widget class */
	UPROPERTY(EditAnywhere, Category = "UI|Controls")
	TSubclassOf<UControlsOverlay> ControlsOverlayWidgetClass;

	/** Get current input mode */
	UFUNCTION(BlueprintCallable, Category = "Input")
	EInputMode GetInputMode() const { return CurrentInputMode; }

	/** Set input mode (called from selection widget) */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetCustomInputMode(EInputMode NewInputMode);

	/** Toggle controls overlay visibility */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ToggleControlsOverlay();

	/** Show/hide controls overlay */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetControlsOverlayVisible(bool bVisible);

protected:
	/** Current input mode */
	UPROPERTY(BlueprintReadOnly, Category = "Input")
	EInputMode CurrentInputMode = EInputMode::Keyboard;

	/** Has player selected input mode? (persist this in save game later) */
	UPROPERTY(BlueprintReadOnly, Category = "Input")
	bool bHasSelectedInputMode = false;

	/** Input mode selection widget */
	UPROPERTY()
	TObjectPtr<UUserWidget> InputModeSelectionWidget;

	/** Controls overlay widget */
	UPROPERTY()
	TObjectPtr<UControlsOverlay> ControlsOverlayWidget;

	/** Keyboard input mapping context */
	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<class UInputMappingContext*> KeyboardMappingContexts;

	/** Gamepad input mapping context */
	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<class UInputMappingContext*> GamepadMappingContexts;

	/** Show input mode selection screen */
	void ShowInputModeSelection();

	/** Called when input mode is selected */
	UFUNCTION()
	void OnInputModeSelected(EInputMode SelectedMode);

	/** Update input mapping contexts based on selected mode */
	void UpdateInputMappings();

	/** Create and show controls overlay */
	void CreateControlsOverlay();

	/** Create HUD dynamically in C++ (no Blueprint required) */
	void CreateDynamicHUD();
};
