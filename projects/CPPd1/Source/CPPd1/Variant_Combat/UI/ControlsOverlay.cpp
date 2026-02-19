// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Combat/UI/ControlsOverlay.h"
#include "Components/Widget.h"

UControlsOverlay::UControlsOverlay(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentInputMode = EInputMode::Keyboard;
}

void UControlsOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	SetInputMode(CurrentInputMode);
}

void UControlsOverlay::SetInputMode(EInputMode NewInputMode)
{
	CurrentInputMode = NewInputMode;

	// Show/hide panels based on input mode
	if (KeyboardControlsPanel)
	{
		KeyboardControlsPanel->SetVisibility(
			NewInputMode == EInputMode::Keyboard ? ESlateVisibility::Visible : ESlateVisibility::Collapsed
		);
	}

	if (GamepadControlsPanel)
	{
		GamepadControlsPanel->SetVisibility(
			NewInputMode == EInputMode::Gamepad ? ESlateVisibility::Visible : ESlateVisibility::Collapsed
		);
	}

	// Call Blueprint event
	OnInputModeChanged(NewInputMode);
}

void UControlsOverlay::SetOverlayVisible(bool bVisible)
{
	SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UControlsOverlay::ToggleOverlay()
{
	ESlateVisibility CurrentVisibility = GetVisibility();
	SetOverlayVisible(CurrentVisibility != ESlateVisibility::Visible);
}
