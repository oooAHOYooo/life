#include "P2G4WEnemyTrickster.h"

AP2G4WEnemyTrickster::AP2G4WEnemyTrickster()
{
	MaxHealth = 50.f; // Squishier than base
	CurrentHealth = MaxHealth;
}

void AP2G4WEnemyTrickster::AttemptDodge()
{
	// Logic to quickly side-step or roll based on player attack
	// Usually involves launching character or playing root motion montage
}

void AP2G4WEnemyTrickster::TakeDamageFromGod(float DamageAmount, FVector HitLocation)
{
	if (FMath::FRand() < DodgeChance)
	{
		AttemptDodge();
		return; // Avoided damage
	}

	Super::TakeDamageFromGod(DamageAmount, HitLocation);
}
