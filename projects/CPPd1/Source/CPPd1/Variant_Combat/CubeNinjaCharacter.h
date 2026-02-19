// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CPPd1.h"
#include "NinjaCharacter.h"
#include "CubeNinjaCharacter.generated.h"

class UCubeNinjaBodyComponent;

/**
 * Ninja character that uses a cube-ninja body (interlocking procedural cubes) as its visual.
 * Same movement and combat as NinjaCharacter; the skeletal mesh is hidden and the cube body is shown.
 */
UCLASS(BlueprintType, Blueprintable)
class CPPd1_API ACubeNinjaCharacter : public ANinjaCharacter
{
	GENERATED_BODY()

public:
	ACubeNinjaCharacter();

	/** The cube-ninja body (ninja made of cubes). Edit Body Scale and limb swing here. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cube Ninja")
	TObjectPtr<UCubeNinjaBodyComponent> CubeBody;

protected:
	virtual void BeginPlay() override;
};
