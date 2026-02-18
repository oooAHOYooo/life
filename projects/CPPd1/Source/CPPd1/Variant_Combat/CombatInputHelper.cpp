// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Combat/CombatInputHelper.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

UEnhancedInputLocalPlayerSubsystem* UCombatInputHelper::GetInputSubsystem(UObject* WorldContextObject, int32 PlayerIndex)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, PlayerIndex))
		{
			if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
			{
				return LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			}
		}
	}
	return nullptr;
}

void UCombatInputHelper::SetupXboxControllerInput(UObject* WorldContextObject, int32 PlayerIndex)
{
	// This is a helper - the actual Input Mapping Contexts need to be created in Blueprint
	// This function can be extended to programmatically add mappings
	// For now, it's a placeholder that can be called from Blueprint
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetInputSubsystem(WorldContextObject, PlayerIndex))
	{
		// Clear existing contexts
		Subsystem->ClearAllMappings();
		
		// Note: Input Mapping Contexts should be set up in Blueprint
		// This helper can be extended to programmatically create them
		UE_LOG(LogTemp, Log, TEXT("Xbox controller input setup called for player %d. Configure Input Mapping Contexts in Blueprint."), PlayerIndex);
	}
}

void UCombatInputHelper::SetupKeyboardMouseInput(UObject* WorldContextObject, int32 PlayerIndex)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetInputSubsystem(WorldContextObject, PlayerIndex))
	{
		// Clear existing contexts
		Subsystem->ClearAllMappings();
		
		UE_LOG(LogTemp, Log, TEXT("Keyboard/Mouse input setup called for player %d. Configure Input Mapping Contexts in Blueprint."), PlayerIndex);
	}
}

void UCombatInputHelper::AutoSetupInput(UObject* WorldContextObject, int32 PlayerIndex)
{
	// Auto-detect: For now, default to Xbox controller
	// Can be extended to detect connected controllers
	SetupXboxControllerInput(WorldContextObject, PlayerIndex);
}
