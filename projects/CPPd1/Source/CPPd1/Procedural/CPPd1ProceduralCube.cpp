// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPd1ProceduralCube.h"
#include "ProceduralMeshComponent.h"
#include "Engine/Engine.h"

ACPPd1ProceduralCube::ACPPd1ProceduralCube()
{
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	SetRootComponent(ProceduralMesh);
}

void ACPPd1ProceduralCube::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	BuildCubeMesh();
}

void ACPPd1ProceduralCube::RebuildMesh()
{
	BuildCubeMesh();
}

void ACPPd1ProceduralCube::BuildCubeMesh()
{
	if (!ProceduralMesh) return;

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;

	const float H = Size * 0.5f;
	// 8 corners of a cube (centered at origin)
	FVector V[8] = {
		FVector(-H, -H, -H), FVector( H, -H, -H), FVector( H,  H, -H), FVector(-H,  H, -H), // bottom
		FVector(-H, -H,  H), FVector( H, -H,  H), FVector( H,  H,  H), FVector(-H,  H,  H)  // top
	};

	// 6 faces: front, back, left, right, top, bottom (outward normals)
	auto AddQuad = [&](int32 A, int32 B, int32 C, int32 D, const FVector& N) {
		int32 Base = Vertices.Num();
		Vertices.Add(V[A]); Vertices.Add(V[B]); Vertices.Add(V[C]); Vertices.Add(V[D]);
		Normals.Add(N); Normals.Add(N); Normals.Add(N); Normals.Add(N);
		UV0.Add(FVector2D(0,0)); UV0.Add(FVector2D(1,0)); UV0.Add(FVector2D(1,1)); UV0.Add(FVector2D(0,1));
		Triangles.Add(Base+0); Triangles.Add(Base+1); Triangles.Add(Base+2);
		Triangles.Add(Base+0); Triangles.Add(Base+2); Triangles.Add(Base+3);
	};

	AddQuad(4, 5, 6, 7, FVector( 0,  0,  1)); // front (Z+)
	AddQuad(1, 0, 3, 2, FVector( 0,  0, -1)); // back (Z-)
	AddQuad(0, 4, 7, 3, FVector(-1,  0,  0)); // left (X-)
	AddQuad(5, 1, 2, 6, FVector( 1,  0,  0)); // right (X+)
	AddQuad(7, 6, 2, 3, FVector( 0,  1,  0)); // top (Y+)
	AddQuad(0, 1, 5, 4, FVector( 0, -1,  0)); // bottom (Y-)

	ProceduralMesh->ClearAllMeshSections();
	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	ProceduralMesh->SetMaterial(0, nullptr); // use default material or set in editor
}
