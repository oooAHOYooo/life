// Copyright Epic Games, Inc. All Rights Reserved.

#include "CubeNinjaBodyComponent.h"
#include "ProceduralMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

namespace
{
	enum EPart
	{
		Pelvis, Spine, Chest, Head,
		L_Shoulder, L_UpperArm, L_Elbow, L_LowerArm, L_Hand,
		R_Shoulder, R_UpperArm, R_Elbow, R_LowerArm, R_Hand,
		L_UpperLeg, L_Knee, L_LowerLeg, L_Foot,
		R_UpperLeg, R_Knee, R_LowerLeg, R_Foot,
		NumParts
	};
	// Indices that get swing animation
	const int32 L_UA = 5, R_UA = 10, L_UL = 14, R_UL = 18;
}

UCubeNinjaBodyComponent::UCubeNinjaBodyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	PartIsSphere.SetNum(NumParts);
	PartHalfExtents.SetNum(NumParts);
	PartRadii.SetNum(NumParts);
	PartLocations.SetNum(NumParts);
	PartDefaultRotations.SetNum(NumParts);
	for (int32 i = 0; i < NumParts; ++i)
	{
		PartIsSphere[i] = false;
		PartRadii[i] = 0.f;
		PartDefaultRotations[i] = FRotator::ZeroRotator;
	}

	// ---- Spheres: head, pelvis, shoulders, elbows, hands, knees, feet ----
	PartIsSphere[Pelvis] = PartIsSphere[Head] = true;
	PartIsSphere[L_Shoulder] = PartIsSphere[R_Shoulder] = true;
	PartIsSphere[L_Elbow] = PartIsSphere[R_Elbow] = true;
	PartIsSphere[L_Hand] = PartIsSphere[R_Hand] = true;
	PartIsSphere[L_Knee] = PartIsSphere[R_Knee] = true;
	PartIsSphere[L_Foot] = PartIsSphere[R_Foot] = true;

	PartRadii[Pelvis] = 14.f;
	PartRadii[Head] = 11.f;
	PartRadii[L_Shoulder] = PartRadii[R_Shoulder] = 6.f;
	PartRadii[L_Elbow] = PartRadii[R_Elbow] = 4.f;
	PartRadii[L_Hand] = PartRadii[R_Hand] = 3.5f;
	PartRadii[L_Knee] = PartRadii[R_Knee] = 5.f;
	PartRadii[L_Foot] = PartRadii[R_Foot] = 4.f;

	// ---- Tiny cubes: spine, chest, upper/lower arms, upper/lower legs ----
	PartHalfExtents[Spine]  = FVector(6.f, 5.f, 8.f);
	PartHalfExtents[Chest]  = FVector(8.f, 6.f, 7.f);
	PartHalfExtents[L_UpperArm] = PartHalfExtents[R_UpperArm] = FVector(2.5f, 2.5f, 10.f);
	PartHalfExtents[L_LowerArm] = PartHalfExtents[R_LowerArm] = FVector(2.f, 2.f, 9.f);
	PartHalfExtents[L_UpperLeg] = PartHalfExtents[R_UpperLeg] = FVector(3.f, 3.f, 11.f);
	PartHalfExtents[L_LowerLeg] = PartHalfExtents[R_LowerLeg] = FVector(2.5f, 2.5f, 10.f);
	// Unused for sphere parts but keep valid
	PartHalfExtents[Pelvis] = PartHalfExtents[Head] = FVector(1.f, 1.f, 1.f);
	PartHalfExtents[L_Shoulder] = PartHalfExtents[R_Shoulder] = FVector(1.f, 1.f, 1.f);
	PartHalfExtents[L_Elbow] = PartHalfExtents[R_Elbow] = PartHalfExtents[L_Hand] = PartHalfExtents[R_Hand] = FVector(1.f, 1.f, 1.f);
	PartHalfExtents[L_Knee] = PartHalfExtents[R_Knee] = PartHalfExtents[L_Foot] = PartHalfExtents[R_Foot] = FVector(1.f, 1.f, 1.f);

	// Locations relative to parent (pelvis bottom at origin for root child)
	float Pr = PartRadii[Pelvis];
	PartLocations[Pelvis] = FVector(0.f, 0.f, Pr);

	float Sz = PartHalfExtents[Spine].Z, Cz = PartHalfExtents[Chest].Z, Hr = PartRadii[Head];
	PartLocations[Spine] = FVector(0.f, 0.f, Pr + Sz);
	PartLocations[Chest] = FVector(0.f, 0.f, PartHalfExtents[Spine].Z + Cz);
	PartLocations[Head] = FVector(0.f, 0.f, Cz + Hr);

	float Cx = PartHalfExtents[Chest].X, Sr = PartRadii[L_Shoulder];
	PartLocations[L_Shoulder] = FVector(-(Cx + Sr), 1.f, 0.f);
	PartLocations[R_Shoulder] = FVector(Cx + Sr, 1.f, 0.f);

	float Uz = PartHalfExtents[L_UpperArm].Z, Er = PartRadii[L_Elbow], Lz = PartHalfExtents[L_LowerArm].Z, Hr2 = PartRadii[L_Hand];
	PartLocations[L_UpperArm] = FVector(0.f, 0.f, -Uz - Er);
	PartLocations[L_Elbow] = FVector(0.f, 0.f, -Er - Lz);
	PartLocations[L_LowerArm] = FVector(0.f, 0.f, -Lz - Hr2);
	PartLocations[L_Hand] = FVector(0.f, 0.f, 0.f);
	PartLocations[R_UpperArm] = FVector(0.f, 0.f, -Uz - Er);
	PartLocations[R_Elbow] = FVector(0.f, 0.f, -Er - Lz);
	PartLocations[R_LowerArm] = FVector(0.f, 0.f, -Lz - Hr2);
	PartLocations[R_Hand] = FVector(0.f, 0.f, 0.f);

	PartDefaultRotations[L_UpperArm] = FRotator(0.f, 0.f, 12.f);
	PartDefaultRotations[R_UpperArm] = FRotator(0.f, 0.f, -12.f);

	float HipX = PartRadii[Pelvis] * 0.7f, LUz = PartHalfExtents[L_UpperLeg].Z, Kr = PartRadii[L_Knee], LLz = PartHalfExtents[L_LowerLeg].Z, Fr = PartRadii[L_Foot];
	PartLocations[L_UpperLeg] = FVector(-HipX, 0.f, -PartRadii[Pelvis] - LUz);
	PartLocations[L_Knee] = FVector(0.f, 0.f, -LUz - Kr);
	PartLocations[L_LowerLeg] = FVector(0.f, 0.f, -Kr - LLz);
	PartLocations[L_Foot] = FVector(0.f, 0.f, -LLz - Fr);
	PartLocations[R_UpperLeg] = FVector(HipX, 0.f, -PartRadii[Pelvis] - LUz);
	PartLocations[R_Knee] = FVector(0.f, 0.f, -LUz - Kr);
	PartLocations[R_LowerLeg] = FVector(0.f, 0.f, -Kr - LLz);
	PartLocations[R_Foot] = FVector(0.f, 0.f, -LLz - Fr);

	SwingPartIndices = { L_UA, R_UA, L_UL, R_UL };

	const TCHAR* PivotNames[NumParts] = {
		TEXT("CN_Pelvis"), TEXT("CN_Spine"), TEXT("CN_Chest"), TEXT("CN_Head"),
		TEXT("CN_LSh"), TEXT("CN_LUA"), TEXT("CN_LElbow"), TEXT("CN_LLA"), TEXT("CN_LHand"),
		TEXT("CN_RSh"), TEXT("CN_RUA"), TEXT("CN_RElbow"), TEXT("CN_RLA"), TEXT("CN_RHand"),
		TEXT("CN_LUL"), TEXT("CN_LKnee"), TEXT("CN_LLL"), TEXT("CN_LFoot"),
		TEXT("CN_RUL"), TEXT("CN_RKnee"), TEXT("CN_RLL"), TEXT("CN_RFoot")
	};
	const TCHAR* MeshNames[NumParts] = {
		TEXT("CN_PelvisM"), TEXT("CN_SpineM"), TEXT("CN_ChestM"), TEXT("CN_HeadM"),
		TEXT("CN_LShM"), TEXT("CN_LUAM"), TEXT("CN_LElbowM"), TEXT("CN_LLAM"), TEXT("CN_LHandM"),
		TEXT("CN_RShM"), TEXT("CN_RUAM"), TEXT("CN_RElbowM"), TEXT("CN_RLAM"), TEXT("CN_RHandM"),
		TEXT("CN_LULM"), TEXT("CN_LKneeM"), TEXT("CN_LLLM"), TEXT("CN_LFootM"),
		TEXT("CN_RULM"), TEXT("CN_RKneeM"), TEXT("CN_RLLM"), TEXT("CN_RFootM")
	};

	PartPivots.Reserve(NumParts);
	PartMeshes.Reserve(NumParts);
	for (int32 i = 0; i < NumParts; ++i)
	{
		USceneComponent* Pivot = CreateDefaultSubobject<USceneComponent>(PivotNames[i]);
		if (!Pivot)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create pivot component %d"), i);
			continue;
		}
		
		UProceduralMeshComponent* Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(MeshNames[i]);
		if (!Mesh)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create mesh component %d"), i);
			PartPivots.Add(Pivot);
			PartMeshes.Add(nullptr);
			continue;
		}
		
		Mesh->SetupAttachment(Pivot);
		Mesh->SetRelativeLocation(FVector::Zero());
		Mesh->SetRelativeRotation(FRotator::ZeroRotator);
		PartPivots.Add(Pivot);
		PartMeshes.Add(Mesh);
	}

	auto Attach = [](USceneComponent* Child, USceneComponent* Parent, const FVector& Loc, const FRotator& Rot) {
		if (!IsValid(Child) || !IsValid(Parent))
			return;
		Child->SetupAttachment(Parent);
		Child->SetRelativeLocation(Loc);
		Child->SetRelativeRotation(Rot);
	};

	// Attach components with safety checks - only attach if both parent and child are valid
	if (PartPivots.IsValidIndex(Pelvis) && IsValid(PartPivots[Pelvis]))
	{
		Attach(PartPivots[Pelvis], this, PartLocations[Pelvis], PartDefaultRotations[Pelvis]);
		
		if (PartPivots.IsValidIndex(Spine) && IsValid(PartPivots[Spine]))
		{
			Attach(PartPivots[Spine], PartPivots[Pelvis], PartLocations[Spine], PartDefaultRotations[Spine]);
			
			if (PartPivots.IsValidIndex(Chest) && IsValid(PartPivots[Chest]))
			{
				Attach(PartPivots[Chest], PartPivots[Spine], PartLocations[Chest], PartDefaultRotations[Chest]);
				
				if (PartPivots.IsValidIndex(Head) && IsValid(PartPivots[Head]))
					Attach(PartPivots[Head], PartPivots[Chest], PartLocations[Head], PartDefaultRotations[Head]);
				
				if (PartPivots.IsValidIndex(L_Shoulder) && IsValid(PartPivots[L_Shoulder]))
				{
					Attach(PartPivots[L_Shoulder], PartPivots[Chest], PartLocations[L_Shoulder], PartDefaultRotations[L_Shoulder]);
					if (PartPivots.IsValidIndex(L_UpperArm) && IsValid(PartPivots[L_UpperArm]))
					{
						Attach(PartPivots[L_UpperArm], PartPivots[L_Shoulder], PartLocations[L_UpperArm], PartDefaultRotations[L_UpperArm]);
						if (PartPivots.IsValidIndex(L_Elbow) && IsValid(PartPivots[L_Elbow]))
						{
							Attach(PartPivots[L_Elbow], PartPivots[L_UpperArm], PartLocations[L_Elbow], PartDefaultRotations[L_Elbow]);
							if (PartPivots.IsValidIndex(L_LowerArm) && IsValid(PartPivots[L_LowerArm]))
							{
								Attach(PartPivots[L_LowerArm], PartPivots[L_Elbow], PartLocations[L_LowerArm], PartDefaultRotations[L_LowerArm]);
								if (PartPivots.IsValidIndex(L_Hand) && IsValid(PartPivots[L_Hand]))
									Attach(PartPivots[L_Hand], PartPivots[L_LowerArm], PartLocations[L_Hand], PartDefaultRotations[L_Hand]);
							}
						}
					}
				}
				
				if (PartPivots.IsValidIndex(R_Shoulder) && IsValid(PartPivots[R_Shoulder]))
				{
					Attach(PartPivots[R_Shoulder], PartPivots[Chest], PartLocations[R_Shoulder], PartDefaultRotations[R_Shoulder]);
					if (PartPivots.IsValidIndex(R_UpperArm) && IsValid(PartPivots[R_UpperArm]))
					{
						Attach(PartPivots[R_UpperArm], PartPivots[R_Shoulder], PartLocations[R_UpperArm], PartDefaultRotations[R_UpperArm]);
						if (PartPivots.IsValidIndex(R_Elbow) && IsValid(PartPivots[R_Elbow]))
						{
							Attach(PartPivots[R_Elbow], PartPivots[R_UpperArm], PartLocations[R_Elbow], PartDefaultRotations[R_Elbow]);
							if (PartPivots.IsValidIndex(R_LowerArm) && IsValid(PartPivots[R_LowerArm]))
							{
								Attach(PartPivots[R_LowerArm], PartPivots[R_Elbow], PartLocations[R_LowerArm], PartDefaultRotations[R_LowerArm]);
								if (PartPivots.IsValidIndex(R_Hand) && IsValid(PartPivots[R_Hand]))
									Attach(PartPivots[R_Hand], PartPivots[R_LowerArm], PartLocations[R_Hand], PartDefaultRotations[R_Hand]);
							}
						}
					}
				}
			}
		}
		
		// Legs
		if (PartPivots.IsValidIndex(L_UpperLeg) && IsValid(PartPivots[L_UpperLeg]))
		{
			Attach(PartPivots[L_UpperLeg], PartPivots[Pelvis], PartLocations[L_UpperLeg], PartDefaultRotations[L_UpperLeg]);
			if (PartPivots.IsValidIndex(L_Knee) && IsValid(PartPivots[L_Knee]))
			{
				Attach(PartPivots[L_Knee], PartPivots[L_UpperLeg], PartLocations[L_Knee], PartDefaultRotations[L_Knee]);
				if (PartPivots.IsValidIndex(L_LowerLeg) && IsValid(PartPivots[L_LowerLeg]))
				{
					Attach(PartPivots[L_LowerLeg], PartPivots[L_Knee], PartLocations[L_LowerLeg], PartDefaultRotations[L_LowerLeg]);
					if (PartPivots.IsValidIndex(L_Foot) && IsValid(PartPivots[L_Foot]))
						Attach(PartPivots[L_Foot], PartPivots[L_LowerLeg], PartLocations[L_Foot], PartDefaultRotations[L_Foot]);
				}
			}
		}
		
		if (PartPivots.IsValidIndex(R_UpperLeg) && IsValid(PartPivots[R_UpperLeg]))
		{
			Attach(PartPivots[R_UpperLeg], PartPivots[Pelvis], PartLocations[R_UpperLeg], PartDefaultRotations[R_UpperLeg]);
			if (PartPivots.IsValidIndex(R_Knee) && IsValid(PartPivots[R_Knee]))
			{
				Attach(PartPivots[R_Knee], PartPivots[R_UpperLeg], PartLocations[R_Knee], PartDefaultRotations[R_Knee]);
				if (PartPivots.IsValidIndex(R_LowerLeg) && IsValid(PartPivots[R_LowerLeg]))
				{
					Attach(PartPivots[R_LowerLeg], PartPivots[R_Knee], PartLocations[R_LowerLeg], PartDefaultRotations[R_LowerLeg]);
					if (PartPivots.IsValidIndex(R_Foot) && IsValid(PartPivots[R_Foot]))
						Attach(PartPivots[R_Foot], PartPivots[R_LowerLeg], PartLocations[R_Foot], PartDefaultRotations[R_Foot]);
				}
			}
		}
	}
}

void UCubeNinjaBodyComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Delay mesh building to ensure all components are fully initialized
	// Use a timer to rebuild on next frame
	if (UWorld* World = GetWorld())
	{
		FTimerHandle RebuildTimer;
		World->GetTimerManager().SetTimer(RebuildTimer, this, &UCubeNinjaBodyComponent::RebuildBody, 0.01f, false);
	}
}

void UCubeNinjaBodyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateLimbSwing(DeltaTime);
}

void UCubeNinjaBodyComponent::RebuildBody()
{
	// Safety check
	if (PartMeshes.Num() == 0 || PartPivots.Num() == 0)
		return;

	const float S = BodyScale;
	for (int32 i = 0; i < PartMeshes.Num(); ++i)
	{
		if (!IsValid(PartMeshes[i])) continue;
		if (PartIsSphere.IsValidIndex(i) && PartIsSphere[i] && PartRadii.IsValidIndex(i))
			BuildSphereInMesh(PartMeshes[i], PartRadii[i] * S);
		else if (PartHalfExtents.IsValidIndex(i))
			BuildCubeInMesh(PartMeshes[i], PartHalfExtents[i] * S);
	}
	for (int32 i = 0; i < PartPivots.Num(); ++i)
	{
		if (IsValid(PartPivots[i]) && PartLocations.IsValidIndex(i))
			PartPivots[i]->SetRelativeLocation(PartLocations[i] * S);
	}
}

void UCubeNinjaBodyComponent::BuildCubeInMesh(UProceduralMeshComponent* Mesh, const FVector& HalfExtents)
{
	if (!IsValid(Mesh)) return;
	const float Hx = HalfExtents.X, Hy = HalfExtents.Y, Hz = HalfExtents.Z;
	FVector V[8] = {
		FVector(-Hx, -Hy, -Hz), FVector(Hx, -Hy, -Hz), FVector(Hx, Hy, -Hz), FVector(-Hx, Hy, -Hz),
		FVector(-Hx, -Hy, Hz),  FVector(Hx, -Hy, Hz),  FVector(Hx, Hy, Hz),  FVector(-Hx, Hy, Hz)
	};
	TArray<FVector> Vertices, Normals;
	TArray<int32> Triangles;
	TArray<FVector2D> UV0;
	auto AddQuad = [&](int32 A, int32 B, int32 C, int32 D, const FVector& N) {
		int32 Base = Vertices.Num();
		Vertices.Add(V[A]); Vertices.Add(V[B]); Vertices.Add(V[C]); Vertices.Add(V[D]);
		Normals.Add(N); Normals.Add(N); Normals.Add(N); Normals.Add(N);
		UV0.Add(FVector2D(0,0)); UV0.Add(FVector2D(1,0)); UV0.Add(FVector2D(1,1)); UV0.Add(FVector2D(0,1));
		Triangles.Add(Base+0); Triangles.Add(Base+1); Triangles.Add(Base+2);
		Triangles.Add(Base+0); Triangles.Add(Base+2); Triangles.Add(Base+3);
	};
	AddQuad(4, 5, 6, 7, FVector(0, 0, 1));
	AddQuad(1, 0, 3, 2, FVector(0, 0, -1));
	AddQuad(0, 4, 7, 3, FVector(-1, 0, 0));
	AddQuad(5, 1, 2, 6, FVector(1, 0, 0));
	AddQuad(7, 6, 2, 3, FVector(0, 1, 0));
	AddQuad(0, 1, 5, 4, FVector(0, -1, 0));
	Mesh->ClearAllMeshSections();
	Mesh->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	Mesh->SetMaterial(0, CubeMaterial);
}

void UCubeNinjaBodyComponent::BuildSphereInMesh(UProceduralMeshComponent* Mesh, float Radius, int32 Segments)
{
	if (!IsValid(Mesh) || Radius <= 0.f) return;
	TArray<FVector> Vertices, Normals;
	TArray<int32> Triangles;
	TArray<FVector2D> UV0;
	const int32 RingCount = FMath::Max(2, Segments);
	const int32 SectCount = FMath::Max(3, Segments * 2);
	for (int32 Ring = 0; Ring <= RingCount; ++Ring)
	{
		const float Phi = PI * (float)Ring / (float)RingCount;
		const float Y = -FMath::Cos(Phi);
		const float RingR = FMath::Sin(Phi);
		for (int32 Sect = 0; Sect <= SectCount; ++Sect)
		{
			const float Theta = 2.f * PI * (float)Sect / (float)SectCount;
			const float X = RingR * FMath::Cos(Theta);
			const float Z = RingR * FMath::Sin(Theta);
			FVector N(X, Z, Y);
			N.Normalize();
			Vertices.Add(N * Radius);
			Normals.Add(N);
			UV0.Add(FVector2D((float)Sect / (float)SectCount, (float)Ring / (float)RingCount));
		}
	}
	for (int32 Ring = 0; Ring < RingCount; ++Ring)
		for (int32 Sect = 0; Sect < SectCount; ++Sect)
		{
			const int32 A = Ring * (SectCount + 1) + Sect;
			const int32 B = A + 1;
			const int32 C = A + (SectCount + 1);
			const int32 D = C + 1;
			Triangles.Add(A); Triangles.Add(C); Triangles.Add(B);
			Triangles.Add(B); Triangles.Add(C); Triangles.Add(D);
		}
	Mesh->ClearAllMeshSections();
	Mesh->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	Mesh->SetMaterial(0, CubeMaterial);
}

void UCubeNinjaBodyComponent::UpdateLimbSwing(float DeltaTime)
{
	// Safety checks
	if (!IsValid(this) || !GetOwner())
		return;

	ACharacter* Char = Cast<ACharacter>(GetOwner());
	if (!Char)
		return;

	UCharacterMovementComponent* Movement = Char->GetCharacterMovement();
	const float Speed = Movement ? Movement->Velocity.Size2D() : 0.f;
	const bool bMoving = Speed > 10.f;
	if (bMoving)
		WalkCycleTime += DeltaTime * LimbSwingSpeed * 2.f * PI;
	else
		WalkCycleTime = FMath::FInterpTo(WalkCycleTime, 0.f, DeltaTime, 5.f);
	const float Swing = FMath::Sin(WalkCycleTime) * LimbSwingAmount;
	const float SwingLeg = FMath::Sin(WalkCycleTime + PI) * LimbSwingAmount;

	// Safety check for arrays
	if (!PartPivots.IsValidIndex(0) || PartPivots.Num() == 0)
		return;

	for (int32 Idx : SwingPartIndices)
	{
		if (PartPivots.IsValidIndex(Idx) && IsValid(PartPivots[Idx]))
		{
			FRotator R = PartDefaultRotations.IsValidIndex(Idx) ? PartDefaultRotations[Idx] : FRotator::ZeroRotator;
			if (Idx == L_UA || Idx == R_UA)
				R += FRotator(Idx == L_UA ? Swing : -Swing, 0.f, 0.f);
			else
				R += FRotator(Idx == L_UL ? SwingLeg : -SwingLeg, 0.f, 0.f);
			PartPivots[Idx]->SetRelativeRotation(R);
		}
	}
}
