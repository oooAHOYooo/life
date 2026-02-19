// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CPPd1.h"
#include "Blueprint/UserWidget.h"
#include "ControlsOverlay.generated.h"

/**
 * Input mode enum
 */
UENUM(BlueprintType)
enum class EInputMode : uint8
{
	Keyboard	UMETA(DisplayName = "Keyboard & Mouse"),
	Gamepad		UMETA(DisplayName = "Gamepad")
};

/**
 * Overlay widget showing control instructions for keyboard or gamepad
 */
UCLASS(BlueprintType, Blueprintable)
class CPPd1_API UControlsOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UControlsOverlay(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	/** Set which input mode to display controls for */
	UFUNCTION(BlueprintCallable, Category = "Controls")
	void SetInputMode(EInputMode NewInputMode);

	/** Show/hide the overlay */
	UFUNCTION(BlueprintCallable, Category = "Controls")
	void SetOverlayVisible(bool bVisible);

	/** Toggle overlay visibility */
	UFUNCTION(BlueprintCallable, Category = "Controls")
	void ToggleOverlay();

protected:
	/** Current input mode */
	UPROPERTY(BlueprintReadOnly, Category = "Controls")
	EInputMode CurrentInputMode = EInputMode::Keyboard;

	/** Keyboard controls panel (set in Blueprint) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Controls")
	TObjectPtr<UWidget> KeyboardControlsPanel;

	/** Gamepad controls panel (set in Blueprint) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Controls")
	TObjectPtr<UWidget> GamepadControlsPanel;

	/** Toggle button (optional - for showing/hiding overlay) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Controls")
	TObjectPtr<UWidget> ToggleButton;

	/** Update visibility of control panels based on input mode */
	UFUNCTION(BlueprintImplementableEvent, Category = "Controls")
	void OnInputModeChanged(EInputMode NewInputMode);
};
