// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoPlagoGameModeBase.h"
#include "CoPlagoGameStateBase.h"
#include "CoPlagoPlayerController.h"
#include "CoPlagoPlayerState.h"
#include "CoPlagoCharacter.h"
#include "CoPlago.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "TimerManager.h"

ACoPlagoGameModeBase::ACoPlagoGameModeBase()
{
	GameStateClass = ACoPlagoGameStateBase::StaticClass();
	PlayerControllerClass = ACoPlagoPlayerController::StaticClass();
	PlayerStateClass = ACoPlagoPlayerState::StaticClass();
	DefaultPawnClass = ACoPlagoCharacter::StaticClass();
}

void ACoPlagoGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (bSpawnSecondPlayerOnBeginPlay && NumPlayers >= 2)
	{
		EnsureSecondPlayer();
	}
}

void ACoPlagoGameModeBase::EnsureSecondPlayer()
{
	UWorld* World = GetWorld();
	if (!World || NumPlayers < 2) return;

	APlayerController* PC1 = UGameplayStatics::GetPlayerController(World, 1);
	if (PC1)
	{
		if (!PC1->GetPawn()) RestartPlayer(PC1);
		return;
	}

	PC1 = UGameplayStatics::CreatePlayer(World, 1, true);
	if (PC1) RestartPlayer(PC1);
}

void ACoPlagoGameModeBase::RestartPlayer(AController* NewPlayer)
{
	UWorld* World = GetWorld();
	if (!NewPlayer || !World) return;

	APawn* OldPawn = NewPlayer->GetPawn();
	if (OldPawn)
	{
		NewPlayer->UnPossess();
		OldPawn->Destroy();
	}

	int32 PlayerIndex = (UGameplayStatics::GetPlayerController(World, 1) == NewPlayer) ? 1 : 0;
	AActor* StartSpot = FindPlayerStartByTag(PlayerIndex);
	if (!StartSpot && PlayerIndex == 1)
	{
		APawn* P0 = UGameplayStatics::GetPlayerPawn(World, 0);
		if (P0)
		{
			StartSpot = World->SpawnActor<AActor>(AActor::StaticClass(), P0->GetActorLocation() + SecondPlayerSpawnOffset, FRotator::ZeroRotator);
		}
	}

	FVector SpawnLoc = StartSpot ? StartSpot->GetActorLocation() : FVector::ZeroVector;
	FRotator SpawnRot = StartSpot ? StartSpot->GetActorRotation() : FRotator::ZeroRotator;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APawn* NewPawn = World->SpawnActor<APawn>(DefaultPawnClass, SpawnLoc, SpawnRot, SpawnParams);
	if (NewPawn)
	{
		NewPlayer->Possess(NewPawn);
		ACoPlagoPlayerState* PS = Cast<ACoPlagoPlayerState>(NewPlayer->PlayerState);
		if (PS) PS->SetPlayerIndex(PlayerIndex);
	}
}

AActor* ACoPlagoGameModeBase::FindPlayerStartByTag(int32 PlayerIndex) const
{
	FString Tag = FString::Printf(TEXT("PlayerStart%d"), PlayerIndex);
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		if (It->PlayerStartTag == FName(*Tag)) return *It;
	}
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		return *It;
	}
	return nullptr;
}

void ACoPlagoGameModeBase::OnRoundEnd_Implementation(int32 WinnerPlayerIndex)
{
	ACoPlagoGameStateBase* GS = GetGameState<ACoPlagoGameStateBase>();
	if (GS) GS->SetGamePhase(ECoPlagoGamePhase::RoundEnd);

	CurrentRound++;
	if (MaxRounds > 0 && CurrentRound > MaxRounds)
	{
		OnGameOver();
		return;
	}
}

void ACoPlagoGameModeBase::OnGameOver_Implementation()
{
	ACoPlagoGameStateBase* GS = GetGameState<ACoPlagoGameStateBase>();
	if (GS) GS->SetGamePhase(ECoPlagoGamePhase::GameOver);
}

void ACoPlagoGameModeBase::RequestRespawn(AController* Controller, float Delay)
{
	if (!Controller || !GetWorld()) return;
	PendingRespawnController = Controller;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ACoPlagoGameModeBase::OnRespawnTimer, Delay, false);
}

void ACoPlagoGameModeBase::OnRespawnTimer()
{
	if (AController* C = PendingRespawnController.Get())
		RestartPlayer(C);
	PendingRespawnController.Reset();
}

void ACoPlagoGameModeBase::RestartRound()
{
	UWorld* World = GetWorld();
	if (!World) return;

	ACoPlagoGameStateBase* GS = GetGameState<ACoPlagoGameStateBase>();
	if (GS) GS->SetGamePhase(ECoPlagoGamePhase::Playing);

	for (int32 i = 0; i < NumPlayers; i++)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(World, i);
		if (PC) RestartPlayer(PC);
	}
}
