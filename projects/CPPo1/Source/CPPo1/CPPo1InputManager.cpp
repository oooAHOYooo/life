// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPo1InputManager.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "TimerManager.h"

ACPPo1InputManager::ACPPo1InputManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

ECPPo1InputDevice ACPPo1InputManager::GetInputDeviceType(APlayerController* PlayerController)
{
	if (!PlayerController) return ECPPo1InputDevice::KeyboardMouse;

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
		return ECPPo1InputDevice::Gamepad;
	}

	return ECPPo1InputDevice::KeyboardMouse;
}

bool ACPPo1InputManager::IsUsingGamepad(APlayerController* PlayerController)
{
	return GetInputDeviceType(PlayerController) == ECPPo1InputDevice::Gamepad;
}

bool ACPPo1InputManager::IsUsingKeyboardMouse(APlayerController* PlayerController)
{
	return GetInputDeviceType(PlayerController) == ECPPo1InputDevice::KeyboardMouse;
}

void ACPPo1InputManager::UpdateInputContextForDevice(APlayerController* PlayerController)
{
	if (!PlayerController) return;

	// This would switch input mapping contexts based on device
	// Implementation depends on your input mapping setup
	// You can add/remove different IMCs for gamepad vs keyboard
}

void ACPPo1InputManager::BeginPlay()
{
	Super::BeginPlay();

	// Poll input devices every 0.5 seconds
	GetWorld()->GetTimerManager().SetTimer(InputPollTimer, this, &ACPPo1InputManager::PollInputDevices, 0.5f, true);
}

void ACPPo1InputManager::PollInputDevices()
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
