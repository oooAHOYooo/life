// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPd1GameModeBase.h"
#include "CPPd1GameStateBase.h"
#include "CPPd1PlayerState.h"
#include "Variant_Combat/CombatCharacter.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

ACPPd1GameModeBase::ACPPd1GameModeBase()
{
	GameStateClass = ACPPd1GameStateBase::StaticClass();
	PlayerStateClass = ACPPd1PlayerState::StaticClass();
}

void ACPPd1GameModeBase::RequestRespawn(AController* Controller, APawn* DyingPawn)
{
	if (!Controller || !DyingPawn || !GetWorld()) return;
	FTimerHandle Handle;
	FTimerDelegate Del;
	Del.BindUFunction(this, FName("OnRespawnTimerFired"), DyingPawn);
	GetWorld()->GetTimerManager().SetTimer(Handle, Del, RespawnDelay, false);
	// Keep a reference so the timer isn't cleared when we return (timer is stored in World)
	RespawnTimer = Handle;
}

void ACPPd1GameModeBase::OnRespawnTimerFired(APawn* DyingPawn)
{
	if (DyingPawn)
	{
		DyingPawn->Destroy();
	}
}

void ACPPd1GameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (ACPPd1PlayerState* PS = NewPlayer->GetPlayerState<ACPPd1PlayerState>())
	{
		if (ACPPd1GameStateBase* GS = GetGameState<ACPPd1GameStateBase>())
		{
			const int32 Index = GS->PlayerArray.Num() - 1;
			PS->SetPlayerIndex(FMath::Clamp(Index, 0, 1));
		}
	}
}

void ACPPd1GameModeBase::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);

	// Solo play: spawn a ghost character that mirrors the player (2P co-op by default)
	if (GetNumPlayers() != 1 || !NewPlayer || !DefaultPawnClass) return;

	APawn* MainPawn = NewPlayer->GetPawn();
	ACombatCharacter* MainChar = Cast<ACombatCharacter>(MainPawn);
	if (!MainChar) return;

	UWorld* World = GetWorld();
	if (!World) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ACombatCharacter* Ghost = World->SpawnActor<ACombatCharacter>(DefaultPawnClass, MainPawn->GetActorLocation(), MainPawn->GetActorRotation(), SpawnParams);
	if (!Ghost) return;

	Ghost->bIsGhost = true;
	Ghost->StoredControlRotation = MainPawn->GetActorRotation();
	MainChar->SetGhostCharacter(Ghost);
}

void ACPPd1GameModeBase::EnsureGhostForSoloPlayer(AController* Controller)
{
	if (GetNumPlayers() != 1 || !Controller || !DefaultPawnClass) return;
	APawn* MainPawn = Controller->GetPawn();
	ACombatCharacter* MainChar = Cast<ACombatCharacter>(MainPawn);
	if (!MainChar || MainChar->GetGhostCharacter()) return;

	UWorld* World = GetWorld();
	if (!World) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ACombatCharacter* Ghost = World->SpawnActor<ACombatCharacter>(DefaultPawnClass, MainPawn->GetActorLocation(), MainPawn->GetActorRotation(), SpawnParams);
	if (!Ghost) return;

	Ghost->bIsGhost = true;
	Ghost->StoredControlRotation = MainPawn->GetActorRotation();
	MainChar->SetGhostCharacter(Ghost);
}
