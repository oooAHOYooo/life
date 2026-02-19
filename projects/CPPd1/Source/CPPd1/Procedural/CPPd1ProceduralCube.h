// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CPPd1.h"
#include "GameFramework/Actor.h"
#include "CPPd1ProceduralCube.generated.h"

class UProceduralMeshComponent;

/**
 * Actor that builds a 3D cube from code at runtime (or in editor).
 * Place in level to see a procedural mesh; edit Size to rescale.
 */
UCLASS(BlueprintType, Blueprintable)
class CPPd1_API ACPPd1ProceduralCube : public AActor
{
	GENERATED_BODY()

public:
	ACPPd1ProceduralCube();

	/** Cube edge length (default 100). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural", meta = (ClampMin = "1.0"))
	float Size = 100.f;

	/** Rebuild the procedural mesh (e.g. after changing Size). */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Procedural")
	void RebuildMesh();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, Category = "Procedural")
	TObjectPtr<UProceduralMeshComponent> ProceduralMesh;

	void BuildCubeMesh();
};
