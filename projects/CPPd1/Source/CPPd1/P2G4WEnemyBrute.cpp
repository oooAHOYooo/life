#include "P2G4WEnemyBrute.h"

AP2G4WEnemyBrute::AP2G4WEnemyBrute()
{
	MaxHealth = 200.f; // Beefy
	CurrentHealth = MaxHealth;
}

void AP2G4WEnemyBrute::AttemptGrab()
{
	// Trigger grab animation on player
	// If successful, engage in wrestle minigame
}

void AP2G4WEnemyBrute::TakeDamageFromGod(float DamageAmount, FVector HitLocation)
{
	// Sword does minimal damage to brutes, maybe play a clank sound
	Super::TakeDamageFromGod(DamageAmount * 0.1f, HitLocation);
}

void AP2G4WEnemyBrute::TakeWrestleDamage(float ThrowDamage)
{
	// Wrestling does full damage
	Super::TakeDamageFromGod(ThrowDamage, FVector::ZeroVector);
}
