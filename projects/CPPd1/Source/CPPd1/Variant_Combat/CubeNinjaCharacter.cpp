// Copyright Epic Games, Inc. All Rights Reserved.

#include "CubeNinjaCharacter.h"
#include "Procedural/CubeNinjaBodyComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

ACubeNinjaCharacter::ACubeNinjaCharacter()
{
	CubeBody = CreateDefaultSubobject<UCubeNinjaBodyComponent>(TEXT("CubeBody"));
	CubeBody->SetupAttachment(RootComponent);
	// Align cube body feet with capsule bottom (pelvis bottom at component origin; offset down by half height)
	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		CubeBody->SetRelativeLocation(FVector(0.f, 0.f, -Capsule->GetScaledCapsuleHalfHeight()));
	}
	CubeBody->BodyScale = 0.9f;
}

void ACubeNinjaCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Use cube body as visual; hide skeletal mesh so we see only the cubes
	if (GetMesh())
	{
		GetMesh()->SetVisibility(false);
		GetMesh()->SetHiddenInGame(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
