// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPd1LockOnTargetComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"

UCPPd1LockOnTargetComponent::UCPPd1LockOnTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FVector UCPPd1LockOnTargetComponent::GetLockOnWorldLocation() const
{
	AActor* Owner = GetOwner();
	if (!Owner) return FVector::ZeroVector;

	FVector BaseLocation = Owner->GetActorLocation();

	if (TargetBoneOrSocketName.IsNone() == false)
	{
		if (USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>())
		{
			if (Mesh->DoesSocketExist(TargetBoneOrSocketName))
			{
				BaseLocation = Mesh->GetSocketLocation(TargetBoneOrSocketName);
			}
			else
			{
				const int32 BoneIndex = Mesh->GetBoneIndex(TargetBoneOrSocketName);
				if (BoneIndex != INDEX_NONE)
				{
					BaseLocation = Mesh->GetBoneLocation(TargetBoneOrSocketName);
				}
			}
		}
	}

	return BaseLocation + Owner->GetActorTransform().TransformVector(TargetOffset);
}

void UCPPd1LockOnTargetComponent::FindLockOnTargetsInRadius(UObject* WorldContextObject, FVector Origin, float Radius, TArray<AActor*>& OutTargets)
{
	OutTargets.Reset();
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!World) return;

	TArray<AActor*> Candidates;
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* A = *It;
		if (A && A->FindComponentByClass<UCPPd1LockOnTargetComponent>())
		{
			const float DistSq = FVector::DistSquared(A->GetActorLocation(), Origin);
			if (DistSq <= Radius * Radius)
			{
				Candidates.Add(A);
			}
		}
	}
	Candidates.Sort([&Origin](const AActor& A, const AActor& B) {
		return FVector::DistSquared(A.GetActorLocation(), Origin) < FVector::DistSquared(B.GetActorLocation(), Origin);
	});
	OutTargets = MoveTemp(Candidates);
}
