// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPo1GoalZone.h"
#include "CPPo1PlayerState.h"
#include "CPPo1GameStateBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

ACPPo1GoalZone::ACPPo1GoalZone()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	TriggerBox->SetCollisionProfileName(FName("OverlapAllDynamic"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACPPo1GoalZone::OnOverlap);
}

void ACPPo1GoalZone::ResetForNewRound()
{
	bRoundAlreadyWon = false;
}

void ACPPo1GoalZone::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bOneWinPerRound && bRoundAlreadyWon) return;

	ACharacter* Char = Cast<ACharacter>(OtherActor);
	if (!Char) return;

	AController* Controller = Char->GetController();
	if (!Controller) return;

	ACPPo1PlayerState* PS = Controller->GetPlayerState<ACPPo1PlayerState>();
	if (!PS) return;

	bRoundAlreadyWon = true;
	const int32 WinnerIndex = PS->GetPlayerIndex();
	OnRoundWon.Broadcast(WinnerIndex);

	if (UWorld* World = GetWorld())
	{
		if (ACPPo1GameStateBase* GS = World->GetGameState<ACPPo1GameStateBase>())
		{
			GS->SetGamePhase(ECPPo1GamePhase::RoundEnded);
		}
	}
}
