// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TargetsFactory.generated.h"

class ATarget;
class ATrialByTargets;

/// <summary>
/// Struct for important data
/// </summary>
struct FTrialByTargetsSettings
{
	FVector SphereCenter;
	size_t NumberOfTargetsToSpawn;
	size_t MinDistanceBetweenTargets;
	float RadiusOnWhichTargetsWillSpawn;
	float TargetSize;
};

/// <summary>
/// Enum for choosing algorithm calculation
/// </summary>
UENUM()
enum class EAlgorithmType : uint8
{
	FibonacciSphere UMETA(DisplayName = "FibonacciSphere"),
	RandomSphere UMETA(DisplayName = "RandomSphere"),
	PotentialSphere UMETA(DisplayName = "PotentialSphere")
};

/**
 * 
 */
UCLASS()
class PINGLESTUDIO_API UTargetsFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"))
	static ATarget* SpawnTarget(const UObject* WorldContext, UClass* ClassType, ATrialByTargets* TrialByTargetsActor, const FVector& Location = FVector(0, 0, 0), const FRotator& Rotation = FRotator(0, 0, 0), float TargetSize = 50);
	static ATarget* SpawnTarget(UWorld* WorldContext, UClass* ClassType, ATrialByTargets* TrialByTargetsActor, const FVector& Location = FVector(0, 0, 0), const FRotator& Rotation = FRotator(0, 0, 0), float TargetSize = 50);
	
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"))
	static bool SpawnMultipleTargets(const UObject* WorldContext, UClass* ClassType, ATrialByTargets* TrialByTargetsActor, TArray<ATarget*>& TargetsArray, const TArray<FVector>& Locations, const TArray<FVector>& Rotations, float TargetSize = 50);
	static bool SpawnMultipleTargets(UWorld* WorldContext, UClass* ClassType, ATrialByTargets* TrialByTargetsActor, TArray<ATarget*>& TargetsArray, const TArray<FVector>& Locations, const TArray<FRotator>& Rotations, float TargetSize = 50);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"))
	static bool SpawnTargetsOnSphere(const UObject* WorldContext, UClass* ClassType, ATrialByTargets* TrialByTargetsActor, TArray<ATarget*>& TargetsArray, EAlgorithmType AlgorithmType);
	static bool SpawnTargetsOnSphere(UWorld* WorldContext, UClass* ClassType, ATrialByTargets* TrialByTargetsActor, TArray<ATarget*>& TargetsArray, FTrialByTargetsSettings TrialByTargetsSettings, EAlgorithmType AlgorithmType);

private:

	enum EXYZ
	{
		X,
		Y,
		Z
	};

	static void FibonacciSphere(const FTrialByTargetsSettings& Settings, TArray<FVector>& Locations);
	static void RandomSphere(const FTrialByTargetsSettings& Settings, TArray<FVector>& Locations);
	static void PotentialSphere(const FTrialByTargetsSettings& Settings, TArray<FVector>& Locations);

	static float DeltaE(unsigned long long Index, float DeltaLatticeParameter, float Ei, ::UTargetsFactory::EXYZ Coordinate, TArray<FVector>&
	                    Locations);
	static float E(size_t Index, TArray<FVector>& Locations);
	static float Potential(float R);

	static float GreatArcDistance(const FVector& First, const FVector& Second, const FVector& Center);
};
