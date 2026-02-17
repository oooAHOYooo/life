// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoPlagoLockOnTargetComponent.h"

static const FName CoPlagoLockOnTag(TEXT("CoPlagoLockOnTarget"));

UCoPlagoLockOnTargetComponent::UCoPlagoLockOnTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCoPlagoLockOnTargetComponent::BeginPlay()
{
	Super::BeginPlay();
	if (AActor* Owner = GetOwner())
	{
		Owner->Tags.AddUnique(CoPlagoLockOnTag);
	}
}
