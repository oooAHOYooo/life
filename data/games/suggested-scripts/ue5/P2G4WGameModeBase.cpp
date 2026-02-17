// Copyright Epic Games, Inc. All Rights Reserved.

#include "P2G4WGameModeBase.h"
#include "P2G4WGameStateBase.h"
#include "P2G4WPlayerController.h"
#include "P2G4WPlayerState.h"
#include "P2G4WCharacter.h"
#include "P2G4W.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "TimerManager.h"

AP2G4WGameModeBase::AP2G4WGameModeBase()
{
	GameStateClass = AP2G4WGameStateBase::StaticClass();
	PlayerControllerClass = AP2G4WPlayerController::StaticClass();
	PlayerStateClass = AP2G4WPlayerState::StaticClass();
	DefaultPawnClass = AP2G4WCharacter::StaticClass();
}

void AP2G4WGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (bSpawnSecondPlayerOnBeginPlay && NumPlayers >= 2)
	{
		EnsureSecondPlayer();
	}
}

void AP2G4WGameModeBase::EnsureSecondPlayer()
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

void AP2G4WGameModeBase::RestartPlayer(AController* NewPlayer)
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
		AP2G4WPlayerState* PS = Cast<AP2G4WPlayerState>(NewPlayer->PlayerState);
		if (PS) PS->SetPlayerIndex(PlayerIndex);
	}
}

AActor* AP2G4WGameModeBase::FindPlayerStartByTag(int32 PlayerIndex) const
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

void AP2G4WGameModeBase::OnRoundEnd_Implementation(int32 WinnerPlayerIndex)
{
	AP2G4WGameStateBase* GS = GetGameState<AP2G4WGameStateBase>();
	if (GS) GS->SetGamePhase(EP2G4WGamePhase::RoundEnd);

	CurrentRound++;
	if (MaxRounds > 0 && CurrentRound > MaxRounds)
	{
		OnGameOver();
		return;
	}
}

void AP2G4WGameModeBase::OnGameOver_Implementation()
{
	AP2G4WGameStateBase* GS = GetGameState<AP2G4WGameStateBase>();
	if (GS) GS->SetGamePhase(EP2G4WGamePhase::GameOver);
}

void AP2G4WGameModeBase::RequestRespawn(AController* Controller, float Delay)
{
	if (!Controller || !GetWorld()) return;
	PendingRespawnController = Controller;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AP2G4WGameModeBase::OnRespawnTimer, Delay, false);
}

void AP2G4WGameModeBase::OnRespawnTimer()
{
	if (AController* C = PendingRespawnController.Get())
		RestartPlayer(C);
	PendingRespawnController.Reset();
}

void AP2G4WGameModeBase::RestartRound()
{
	UWorld* World = GetWorld();
	if (!World) return;

	AP2G4WGameStateBase* GS = GetGameState<AP2G4WGameStateBase>();
	if (GS) GS->SetGamePhase(EP2G4WGamePhase::Playing);

	for (int32 i = 0; i < NumPlayers; i++)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(World, i);
		if (PC) RestartPlayer(PC);
	}
}
