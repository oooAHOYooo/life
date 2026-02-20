#pragma once

#include "CoreMinimal.h"
#include "P2G4WEnemyBase.h"
#include "P2G4WEnemyTrickster.generated.h"

/**
 * 
 */
UCLASS()
class CPPD1_API AP2G4WEnemyTrickster : public AP2G4WEnemyBase
{
	GENERATED_BODY()
	
public:
	AP2G4WEnemyTrickster();

	// Trickster specific dodge logic
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AttemptDodge();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DodgeChance = 0.5f;

	virtual void TakeDamageFromGod(float DamageAmount, FVector HitLocation) override;
};
