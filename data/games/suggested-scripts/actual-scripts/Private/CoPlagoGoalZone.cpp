// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoPlagoGoalZone.h"
#include "CoPlagoCharacter.h"
#include "CoPlagoGameModeBase.h"
#include "CoPlagoPlayerState.h"
#include "CoPlago.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ACoPlagoGoalZone::ACoPlagoGoalZone()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void ACoPlagoGoalZone::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACoPlagoGoalZone::OnBoxBeginOverlap);
}

void ACoPlagoGoalZone::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	ACoPlagoCharacter* Char = Cast<ACoPlagoCharacter>(OtherActor);
	if (!Char) return;

	APlayerState* PS = Char->GetPlayerState();
	ACoPlagoPlayerState* P2PS = Cast<ACoPlagoPlayerState>(PS);
	if (P2PS)
	{
		P2PS->AddScore(ScoreToAdd);
	}

	if (bEndRoundOnTouch && P2PS)
	{
		ACoPlagoGameModeBase* GM = GetWorld() ? Cast<ACoPlagoGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())) : nullptr;
		if (GM)
		{
			GM->OnRoundEnd(P2PS->PlayerIndex);
		}
	}
}
