// Copyright Epic Games, Inc. All Rights Reserved.

#include "NinjaPlayerController.h"
#include "NinjaCharacter.h"
#include "CubeNinjaCharacter.h"
#include "Variant_Combat/UI/InputModeSelection.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "Blueprint/UserWidget.h"

ANinjaPlayerController::ANinjaPlayerController()
{
	// Use CubeNinjaCharacter for spawns and respawns
	CharacterClass = ACubeNinjaCharacter::StaticClass();
	CurrentInputMode = EInputMode::Keyboard;
	bHasSelectedInputMode = false;
}

void ANinjaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Show input mode selection if not already selected
	// TODO: Load from save game to persist selection
	if (!bHasSelectedInputMode && IsLocalPlayerController())
	{
		ShowInputModeSelection();
	}
	else
	{
		// Already selected, just update mappings and show controls
		UpdateInputMappings();
		CreateControlsOverlay();
	}
}

void ANinjaPlayerController::ShowInputModeSelection()
{
	if (!InputModeSelectionWidgetClass || !IsLocalPlayerController())
	{
		return;
	}

	InputModeSelectionWidget = CreateWidget<UUserWidget>(this, InputModeSelectionWidgetClass);
	if (InputModeSelectionWidget)
	{
		InputModeSelectionWidget->AddToPlayerScreen(100); // High z-order

		// Bind to selection event if it's an InputModeSelection widget
		if (UInputModeSelection* SelectionWidget = Cast<UInputModeSelection>(InputModeSelectionWidget))
		{
			SelectionWidget->OnInputModeSelected.AddDynamic(this, &ANinjaPlayerController::OnInputModeSelected);
		}
	}
}

void ANinjaPlayerController::OnInputModeSelected(EInputMode SelectedMode)
{
	SetInputMode(SelectedMode);
	bHasSelectedInputMode = true;
	
	// Hide selection widget
	if (InputModeSelectionWidget)
	{
		InputModeSelectionWidget->RemoveFromParent();
		InputModeSelectionWidget = nullptr;
	}

	// Create and show controls overlay
	CreateControlsOverlay();
}

void ANinjaPlayerController::SetInputMode(EInputMode NewInputMode)
{
	CurrentInputMode = NewInputMode;
	UpdateInputMappings();

	// Update controls overlay if it exists
	if (ControlsOverlayWidget)
	{
		ControlsOverlayWidget->SetInputMode(NewInputMode);
	}
}

void ANinjaPlayerController::UpdateInputMappings()
{
	if (!IsLocalPlayerController())
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// Remove all existing mapping contexts
		// Note: This is a simple approach - you might want to track which contexts are added
		// and remove only those, or use priority-based removal

		// Add keyboard or gamepad contexts based on mode
		TArray<UInputMappingContext*>* ContextsToAdd = nullptr;
		if (CurrentInputMode == EInputMode::Keyboard)
		{
			ContextsToAdd = &KeyboardMappingContexts;
		}
		else
		{
			ContextsToAdd = &GamepadMappingContexts;
		}

		if (ContextsToAdd)
		{
			for (UInputMappingContext* Context : *ContextsToAdd)
			{
				if (Context)
				{
					Subsystem->AddMappingContext(Context, 0);
				}
			}
		}
	}
}

void ANinjaPlayerController::CreateControlsOverlay()
{
	if (!ControlsOverlayWidgetClass || !IsLocalPlayerController())
	{
		return;
	}

	ControlsOverlayWidget = CreateWidget<UControlsOverlay>(this, ControlsOverlayWidgetClass);
	if (ControlsOverlayWidget)
	{
		ControlsOverlayWidget->SetInputMode(CurrentInputMode);
		ControlsOverlayWidget->AddToPlayerScreen(10); // Lower z-order than HUD
		// Start visible - player can toggle it
		ControlsOverlayWidget->SetOverlayVisible(true);
	}
}

void ANinjaPlayerController::ToggleControlsOverlay()
{
	if (ControlsOverlayWidget)
	{
		ControlsOverlayWidget->ToggleOverlay();
	}
}

void ANinjaPlayerController::SetControlsOverlayVisible(bool bVisible)
{
	if (ControlsOverlayWidget)
	{
		ControlsOverlayWidget->SetOverlayVisible(bVisible);
	}
}
