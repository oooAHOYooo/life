// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPd1GoalZone.h"
#include "CPPd1PlayerState.h"
#include "CPPd1GameStateBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

ACPPd1GoalZone::ACPPd1GoalZone()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	TriggerBox->SetCollisionProfileName(FName("OverlapAllDynamic"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACPPd1GoalZone::OnOverlap);
}

void ACPPd1GoalZone::ResetForNewRound()
{
	bRoundAlreadyWon = false;
}

void ACPPd1GoalZone::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bOneWinPerRound && bRoundAlreadyWon) return;

	ACharacter* Char = Cast<ACharacter>(OtherActor);
	if (!Char) return;

	AController* Controller = Char->GetController();
	if (!Controller) return;

	ACPPd1PlayerState* PS = Controller->GetPlayerState<ACPPd1PlayerState>();
	if (!PS) return;

	bRoundAlreadyWon = true;
	const int32 WinnerIndex = PS->GetPlayerIndex();
	OnRoundWon.Broadcast(WinnerIndex);

	if (UWorld* World = GetWorld())
	{
		if (ACPPd1GameStateBase* GS = World->GetGameState<ACPPd1GameStateBase>())
		{
			GS->SetGamePhase(ECPPd1GamePhase::RoundEnded);
		}
	}
}
