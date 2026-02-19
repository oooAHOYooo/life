// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CubeNinjaBodyComponent.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;

/**
 * A ninja "model" made of interlocking procedural shapes: spheres for head, pelvis, and joints
 * (shoulders, elbows, hands, knees, feet); tiny cubes for spine, chest, and limb segments.
 * Add to a Character; limb segments are driven by simple procedural motion (walk swing).
 */
UCLASS(ClassGroup = (Procedural), meta = (BlueprintSpawnableComponent))
class CPPd1_API UCubeNinjaBodyComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UCubeNinjaBodyComponent();

	/** Overall scale of the cube body (1 = ~180 cm tall blocky ninja). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Ninja", meta = (ClampMin = 0.1f, ClampMax = 3.0f))
	float BodyScale = 1.0f;

	/** Intensity of arm/leg swing when moving (0 = no swing). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Ninja|Motion", meta = (ClampMin = 0.0f, ClampMax = 60.0f, Units = "deg"))
	float LimbSwingAmount = 25.0f;

	/** Speed of walk-cycle swing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Ninja|Motion", meta = (ClampMin = 0.1f, Units = "Hz"))
	float LimbSwingSpeed = 2.0f;

	/** Optional material applied to all cube segments (if null, uses default which may appear black). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Ninja")
	TObjectPtr<UMaterialInterface> CubeMaterial;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Rebuild all cube meshes (e.g. after changing BodyScale). */
	UFUNCTION(BlueprintCallable, Category = "Cube Ninja")
	void RebuildBody();

protected:
	UPROPERTY()
	TArray<TObjectPtr<USceneComponent>> PartPivots;
	UPROPERTY()
	TArray<TObjectPtr<UProceduralMeshComponent>> PartMeshes;

	/** true = sphere (use PartRadii), false = cube (use PartHalfExtents). */
	TArray<bool> PartIsSphere;
	TArray<FVector> PartHalfExtents;
	TArray<float> PartRadii;
	TArray<FVector> PartLocations;
	TArray<FRotator> PartDefaultRotations;
	/** Indices of parts that get limb swing (L_UpperArm, R_UpperArm, L_UpperLeg, R_UpperLeg). */
	TArray<int32> SwingPartIndices;

	float WalkCycleTime = 0.0f;

	void BuildCubeInMesh(UProceduralMeshComponent* Mesh, const FVector& HalfExtents);
	void BuildSphereInMesh(UProceduralMeshComponent* Mesh, float Radius, int32 Segments = 12);
	void UpdateLimbSwing(float DeltaTime);
};
