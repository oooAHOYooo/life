// Copyright Epic Games, Inc. All Rights Reserved.
// Add this component to any actor (enemy, crate, switch) to make it a valid lock-on target for CoPlago characters.
// The character finds actors with tag CoPlagoLockOnTarget; this component adds that tag in BeginPlay.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoPlagoLockOnTargetComponent.generated.h"

UCLASS(ClassGroup = (CoPlago), meta = (BlueprintSpawnableComponent))
class CoPlago_API UCoPlagoLockOnTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCoPlagoLockOnTargetComponent();

	virtual void BeginPlay() override;

	/** Optional: offset from actor origin for the lock-on point (e.g. enemy chest). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoPlago")
	FVector LockOnOffset = FVector::ZeroVector;
};
