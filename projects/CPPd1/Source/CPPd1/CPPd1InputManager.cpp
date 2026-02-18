// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPd1InputManager.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "TimerManager.h"

ACPPd1InputManager::ACPPd1InputManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

ECPPd1InputDevice ACPPd1InputManager::GetInputDeviceType(APlayerController* PlayerController)
{
	if (!PlayerController) return ECPPd1InputDevice::KeyboardMouse;

	// Check if any gamepad input is active
	if (PlayerController->IsInputKeyDown(EKeys::Gamepad_LeftX) ||
		PlayerController->IsInputKeyDown(EKeys::Gamepad_LeftY) ||
		PlayerController->IsInputKeyDown(EKeys::Gamepad_RightX) ||
		PlayerController->IsInputKeyDown(EKeys::Gamepad_RightY) ||
		PlayerController->IsInputKeyDown(EKeys::Gamepad_LeftTrigger) ||
		PlayerController->IsInputKeyDown(EKeys::Gamepad_RightTrigger) ||
		PlayerController->IsInputKeyDown(EKeys::Gamepad_Special_Left) ||
		PlayerController->IsInputKeyDown(EKeys::Gamepad_FaceButton_Bottom))
	{
		return ECPPd1InputDevice::Gamepad;
	}

	return ECPPd1InputDevice::KeyboardMouse;
}

bool ACPPd1InputManager::IsUsingGamepad(APlayerController* PlayerController)
{
	return GetInputDeviceType(PlayerController) == ECPPd1InputDevice::Gamepad;
}

bool ACPPd1InputManager::IsUsingKeyboardMouse(APlayerController* PlayerController)
{
	return GetInputDeviceType(PlayerController) == ECPPd1InputDevice::KeyboardMouse;
}

void ACPPd1InputManager::UpdateInputContextForDevice(APlayerController* PlayerController)
{
	if (!PlayerController) return;

	// This would switch input mapping contexts based on device
	// Implementation depends on your input mapping setup
	// You can add/remove different IMCs for gamepad vs keyboard
}

void ACPPd1InputManager::BeginPlay()
{
	Super::BeginPlay();

	// Poll input devices every 0.5 seconds
	GetWorld()->GetTimerManager().SetTimer(InputPollTimer, this, &ACPPd1InputManager::PollInputDevices, 0.5f, true);
}

void ACPPd1InputManager::PollInputDevices()
{
	// Update input contexts for all players
	if (UWorld* World = GetWorld())
	{
		for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
		{
			if (APlayerController* PC = It->Get())
			{
				UpdateInputContextForDevice(PC);
			}
		}
	}
}
