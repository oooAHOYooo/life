// Copyright Epic Games, Inc. All Rights Reserved.

#include "P2G4WGoalZone.h"
#include "P2G4WCharacter.h"
#include "P2G4WGameModeBase.h"
#include "P2G4WPlayerState.h"
#include "P2G4W.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AP2G4WGoalZone::AP2G4WGoalZone()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AP2G4WGoalZone::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AP2G4WGoalZone::OnBoxBeginOverlap);
}

void AP2G4WGoalZone::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	AP2G4WCharacter* Char = Cast<AP2G4WCharacter>(OtherActor);
	if (!Char) return;

	APlayerState* PS = Char->GetPlayerState();
	AP2G4WPlayerState* P2PS = Cast<AP2G4WPlayerState>(PS);
	if (P2PS)
	{
		P2PS->AddScore(ScoreToAdd);
	}

	if (bEndRoundOnTouch && P2PS)
	{
		AP2G4WGameModeBase* GM = GetWorld() ? Cast<AP2G4WGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())) : nullptr;
		if (GM)
		{
			GM->OnRoundEnd(P2PS->PlayerIndex);
		}
	}
}
