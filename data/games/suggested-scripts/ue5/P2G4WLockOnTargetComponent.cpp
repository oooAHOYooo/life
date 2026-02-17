// Copyright Epic Games, Inc. All Rights Reserved.

#include "P2G4WLockOnTargetComponent.h"

static const FName P2G4WLockOnTag(TEXT("P2G4WLockOnTarget"));

UP2G4WLockOnTargetComponent::UP2G4WLockOnTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UP2G4WLockOnTargetComponent::BeginPlay()
{
	Super::BeginPlay();
	if (AActor* Owner = GetOwner())
	{
		Owner->Tags.AddUnique(P2G4WLockOnTag);
	}
}
