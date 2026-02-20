#pragma once

#include "CoreMinimal.h"
#include "P2G4WEnemyBase.h"
#include "P2G4WEnemyBrute.generated.h"

/**
 * 
 */
UCLASS()
class CPPD1_API AP2G4WEnemyBrute : public AP2G4WEnemyBase
{
	GENERATED_BODY()
	
public:
	AP2G4WEnemyBrute();

	// Brute Grab Logic
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AttemptGrab();

	// If hit with sword, armor absorbs it or blocks it
	virtual void TakeDamageFromGod(float DamageAmount, FVector HitLocation) override;

	// Only true damage comes from being wrestled/thrown
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TakeWrestleDamage(float ThrowDamage);

};
