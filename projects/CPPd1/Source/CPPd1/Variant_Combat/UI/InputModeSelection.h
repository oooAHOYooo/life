// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CPPd1.h"
#include "Blueprint/UserWidget.h"
#include "ControlsOverlay.h"
#include "InputModeSelection.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputModeSelected, EInputMode, SelectedMode);

/**
 * Widget for selecting input mode (keyboard or gamepad) on first login
 */
UCLASS(BlueprintType, Blueprintable)
class CPPd1_API UInputModeSelection : public UUserWidget
{
	GENERATED_BODY()

public:
	UInputModeSelection(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	/** Called when player selects keyboard input */
	UFUNCTION(BlueprintCallable, Category = "Input Selection")
	void SelectKeyboard();

	/** Called when player selects gamepad input */
	UFUNCTION(BlueprintCallable, Category = "Input Selection")
	void SelectGamepad();

	/** Event fired when input mode is selected */
	UPROPERTY(BlueprintAssignable, Category = "Input Selection")
	FOnInputModeSelected OnInputModeSelected;

protected:
	/** Keyboard button (set in Blueprint) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Input Selection")
	TObjectPtr<UWidget> KeyboardButton;

	/** Gamepad button (set in Blueprint) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Input Selection")
	TObjectPtr<UWidget> GamepadButton;

	/** Title text (set in Blueprint) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Input Selection")
	TObjectPtr<class UTextBlock> TitleText;

	/** Description text (set in Blueprint) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Input Selection")
	TObjectPtr<class UTextBlock> DescriptionText;
};
