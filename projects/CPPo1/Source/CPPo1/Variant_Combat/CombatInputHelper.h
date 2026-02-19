// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CPPo1.h"
#include "UObject/NoExportTypes.h"
#include "CombatInputHelper.generated.h"

class UInputMappingContext;
class UInputAction;
class UEnhancedInputLocalPlayerSubsystem;

/**
 * Helper class to quickly set up Xbox controller input for combat
 * Use this in Blueprint or C++ to auto-configure Enhanced Input
 */
UCLASS(BlueprintType, Blueprintable)
class CPPo1_API UCombatInputHelper : public UObject
{
	GENERATED_BODY()

public:
	/** Setup Xbox controller input for a player controller */
	UFUNCTION(BlueprintCallable, Category = "Combat|Input", meta = (WorldContext = "WorldContextObject"))
	static void SetupXboxControllerInput(UObject* WorldContextObject, int32 PlayerIndex = 0);

	/** Setup keyboard/mouse input for a player controller */
	UFUNCTION(BlueprintCallable, Category = "Combat|Input", meta = (WorldContext = "WorldContextObject"))
	static void SetupKeyboardMouseInput(UObject* WorldContextObject, int32 PlayerIndex = 0);

	/** Auto-detect input device and setup accordingly */
	UFUNCTION(BlueprintCallable, Category = "Combat|Input", meta = (WorldContext = "WorldContextObject"))
	static void AutoSetupInput(UObject* WorldContextObject, int32 PlayerIndex = 0);

protected:
	/** Get the input subsystem for a player */
	static UEnhancedInputLocalPlayerSubsystem* GetInputSubsystem(UObject* WorldContextObject, int32 PlayerIndex);
};
