// Copyright Epic Games, Inc. All Rights Reserved.
// Add this component to any actor (enemy, crate, switch) to make it a valid lock-on target for P2G4W characters.
// The character finds actors with tag P2G4WLockOnTarget; this component adds that tag in BeginPlay.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P2G4WLockOnTargetComponent.generated.h"

UCLASS(ClassGroup = (P2G4W), meta = (BlueprintSpawnableComponent))
class P2G4W_API UP2G4WLockOnTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UP2G4WLockOnTargetComponent();

	virtual void BeginPlay() override;

	/** Optional: offset from actor origin for the lock-on point (e.g. enemy chest). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "P2G4W")
	FVector LockOnOffset = FVector::ZeroVector;
};
