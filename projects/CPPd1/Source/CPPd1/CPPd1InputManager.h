// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPPd1InputManager.generated.h"

/** Input device type */
UENUM(BlueprintType)
enum class ECPPd1InputDevice : uint8
{
	KeyboardMouse,
	Gamepad
};

/**
 * Manages input device detection and switching between gamepad and keyboard/mouse
 * Supports 2-player with separate input devices
 */
UCLASS(BlueprintType, Blueprintable)
class ACPPd1InputManager : public AActor
{
	GENERATED_BODY()

public:

	ACPPd1InputManager();

	/** Get the input device type for a player controller */
	UFUNCTION(BlueprintCallable, Category = "CPPd1|Input")
	static ECPPd1InputDevice GetInputDeviceType(class APlayerController* PlayerController);

	/** Check if player is using gamepad */
	UFUNCTION(BlueprintPure, Category = "CPPd1|Input")
	static bool IsUsingGamepad(class APlayerController* PlayerController);

	/** Check if player is using keyboard/mouse */
	UFUNCTION(BlueprintPure, Category = "CPPd1|Input")
	static bool IsUsingKeyboardMouse(class APlayerController* PlayerController);

	/** Switch input context based on device type */
	UFUNCTION(BlueprintCallable, Category = "CPPd1|Input")
	static void UpdateInputContextForDevice(class APlayerController* PlayerController);

protected:

	virtual void BeginPlay() override;

	/** Poll input devices periodically */
	FTimerHandle InputPollTimer;

	UFUNCTION()
	void PollInputDevices();
};
