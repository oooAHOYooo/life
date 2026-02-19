// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPPo1LockOnTargetComponent.generated.h"

/**
 * Add this component to any actor to make it a valid lock-on target (e.g. enemies, dummies).
 * Ninja-style focus target for the player.
 */
UCLASS(ClassGroup = (CPPo1), meta = (BlueprintSpawnableComponent))
class UCPPo1LockOnTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UCPPo1LockOnTargetComponent();

	/** Optional socket or bone name on the owner's mesh to aim at. If empty, use actor location. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPo1|Lock-On")
	FName TargetBoneOrSocketName;

	/** World offset from actor location (or bone) for the lock-on point. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPo1|Lock-On")
	FVector TargetOffset = FVector::ZeroVector;

	/** Get world location that lock-on should aim at (e.g. for camera or character rotation). */
	UFUNCTION(BlueprintCallable, Category = "CPPo1|Lock-On")
	FVector GetLockOnWorldLocation() const;

	/** Find all actors with a lock-on target component within radius of Origin. Sorted by distance (nearest first). */
	UFUNCTION(BlueprintCallable, Category = "CPPo1|Lock-On", meta = (WorldContext = "WorldContextObject"))
	static void FindLockOnTargetsInRadius(UObject* WorldContextObject, FVector Origin, float Radius, TArray<AActor*>& OutTargets);
};
