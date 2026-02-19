// Copyright Epic Games, Inc. All Rights Reserved.

#include "TricksterMarauder.h"
#include "GameFramework/CharacterMovementComponent.h"

ATricksterMarauder::ATricksterMarauder()
{
	MaxHP = 2.0f;
	CurrentHP = MaxHP;
	MeleeDamage = 0.8f;
	MeleeTraceDistance = 60.0f;
	// Fast and hard to catch
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
		Move->MaxWalkSpeed = 520.0f;
}
