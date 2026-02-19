// Copyright Epic Games, Inc. All Rights Reserved.

#include "BruteMarauder.h"
#include "GameFramework/CharacterMovementComponent.h"

ABruteMarauder::ABruteMarauder()
{
	MaxHP = 6.0f;
	CurrentHP = MaxHP;
	MeleeDamage = 1.4f;
	MeleeKnockbackImpulse = 280.0f;
	MeleeLaunchImpulse = 200.0f;
	// Slow, wants to wrestle
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
		Move->MaxWalkSpeed = 280.0f;
}
