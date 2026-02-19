// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Combat/UI/InputModeSelection.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"

UInputModeSelection::UInputModeSelection(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UInputModeSelection::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInputModeSelection::SelectKeyboard()
{
	OnInputModeSelected.Broadcast(EInputMode::Keyboard);
	SetVisibility(ESlateVisibility::Collapsed);
}

void UInputModeSelection::SelectGamepad()
{
	OnInputModeSelected.Broadcast(EInputMode::Gamepad);
	SetVisibility(ESlateVisibility::Collapsed);
}
