// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetsFactory.h"
#include "Target.h"
#include "TrialByTargets.h"
#include "Log.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Kismet/KismetMathLibrary.h"

ATarget* UTargetsFactory::SpawnTarget(const UObject* WorldContext, UClass* ClassType,
                                      ATrialByTargets* TrialByTargetsActor, const FVector& Location, const FRotator& Rotation, float TargetSize)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		UE_LOG(LogTargets, Error, TEXT("WorldContext is nullptr in UTargetsFactory::SpawnTarget(). GetWorldFromContextObject() returns nullptr"));
		return nullptr;
	}
	return SpawnTarget(World, ClassType, TrialByTargetsActor, Location, Rotation, TargetSize);
}

ATarget* UTargetsFactory::SpawnTarget(UWorld* WorldContext, UClass* ClassType, ATrialByTargets* TrialByTargetsActor, const FVector& Location,
                                      const FRotator& Rotation, float TargetSize)
{
	if (!IsValid(WorldContext))
	{
		UE_LOG(LogTargets, Error, TEXT("WorldContext is nullptr in UTargetsFactory::SpawnTarget()"));
		return nullptr;
	}

	if (!IsValid(ClassType))
	{
		UE_LOG(LogTargets, Error, TEXT("ClassType is nullptr in UTargetsFactory::SpawnTarget()"));
		return nullptr;
	}

	if (!IsValid(TrialByTargetsActor))
	{
		UE_LOG(LogTargets, Error, TEXT("TrialByTargetsActor is nullptr in UTargetsFactory::SpawnTarget()"));
		return nullptr;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Owner = TrialByTargetsActor;

	ATarget* SpawnedTarget = WorldContext->SpawnActor<ATarget>(ClassType, Location, Rotation, Params);
	if (!IsValid(SpawnedTarget))
	{
		UE_LOG(LogTargets, Error, TEXT("Failed to spawn Target"));
		return nullptr;
	}
	SpawnedTarget->SetTargetSize(TargetSize);
	return SpawnedTarget;
}

bool UTargetsFactory::SpawnMultipleTargets(const UObject* WorldContext, UClass* ClassType,
	ATrialByTargets* TrialByTargetsActor, TArray<ATarget*>& TargetsArray, const TArray<FVector>& Locations,
	const TArray<FVector>& Rotations, float TargetSize)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		UE_LOG(LogTargets, Error, TEXT("WorldContext is nullptr in UTargetsFactory::SpawnMultipleTargets(). GetWorldFromContextObject() returns nullptr"));
		return nullptr;
	}
	return SpawnMultipleTargets(World, ClassType, TrialByTargetsActor, TargetsArray, Locations, Rotations, TargetSize);
}

bool UTargetsFactory::SpawnMultipleTargets(UWorld* WorldContext, UClass* ClassType, ATrialByTargets* TrialByTargetsActor,
                                           TArray<ATarget*>& TargetsArray, const TArray<FVector>& Locations, const TArray<FRotator>& Rotations, float TargetSize)
{
	if (!IsValid(WorldContext))
	{
		UE_LOG(LogTargets, Error, TEXT("WorldContext is nullptr in UTargetsFactory::SpawnMultipleTargets()"));
		return false;
	}

	if (!IsValid(ClassType))
	{
		UE_LOG(LogTargets, Error, TEXT("ClassType is nullptr in UTargetsFactory::SpawnMultipleTargets()"));
		return false;
	}

	if (!IsValid(TrialByTargetsActor))
	{
		UE_LOG(LogTargets, Error, TEXT("TrialByTargetsActor is nullptr in UTargetsFactory::SpawnMultipleTargets()"));
		return false;
	}

	size_t NumberOfTargetsToSpawn = Locations.Num();
	if (Locations.Num() != Rotations.Num())
	{
		UE_LOG(LogTargets, Error, TEXT("Number of spawning locations isn't equal to number of rotations in UTargetsFactory::SpawnMultipleTargets(). Minimum quantity will be used"));
		NumberOfTargetsToSpawn = Locations.Num() > Rotations.Num() ? Rotations.Num() : Locations.Num();
	}
	TargetsArray.Reserve(NumberOfTargetsToSpawn);

	for (size_t i = 0; i < NumberOfTargetsToSpawn; i++)
	{
		ATarget* SpawnedTarget = SpawnTarget(WorldContext, ClassType, TrialByTargetsActor, Locations[i], Rotations[i], TargetSize);
		if (!IsValid(SpawnedTarget))
		{
			continue;
		}
		TargetsArray.Add(SpawnedTarget);
	}

	return true;
}

bool UTargetsFactory::SpawnTargetsOnSphere(const UObject* WorldContext, UClass* ClassType,
	ATrialByTargets* TrialByTargetsActor, TArray<ATarget*>& TargetsArray, EAlgorithmType AlgorithmType)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		UE_LOG(LogTargets, Error, TEXT("WorldContext is nullptr in UTargetsFactory::SpawnTargetsOnSphere(). GetWorldFromContextObject() returns nullptr"));
		return nullptr;
	}

	if (!IsValid(TrialByTargetsActor))
	{
		UE_LOG(LogTargets, Error, TEXT("TrialByTargetsActor is nullptr in UTargetsFactory::SpawnTargetsOnSphere()"));
		return false;
	}

	return SpawnTargetsOnSphere(World, ClassType, TrialByTargetsActor, TargetsArray, TrialByTargetsActor->GetSettings(), AlgorithmType);
}

bool UTargetsFactory::SpawnTargetsOnSphere(UWorld* WorldContext, UClass* ClassType,
	ATrialByTargets* TrialByTargetsActor, TArray<ATarget*>& TargetsArray, FTrialByTargetsSettings TrialByTargetsSettings, EAlgorithmType AlgorithmType)
{
	if (!IsValid(WorldContext))
	{
		UE_LOG(LogTargets, Error, TEXT("WorldContext is nullptr in UTargetsFactory::SpawnTargetsOnSphere()"));
		return false;
	}

	if (!IsValid(ClassType))
	{
		UE_LOG(LogTargets, Error, TEXT("ClassType is nullptr in UTargetsFactory::SpawnTargetsOnSphere()"));
		return false;
	}

	if (!IsValid(TrialByTargetsActor))
	{
		UE_LOG(LogTargets, Error, TEXT("TrialByTargetsActor is nullptr in UTargetsFactory::SpawnTargetsOnSphere()"));
		return false;
	}

	TArray<FVector> Locations;
	TArray<FRotator> Rotations;
	Locations.Reserve(TrialByTargetsSettings.NumberOfTargetsToSpawn);
	Rotations.Reserve(TrialByTargetsSettings.NumberOfTargetsToSpawn);
	for (size_t i = 0; i < TrialByTargetsActor->NumberOfTargetsToSpawn; i++)
	{
		Rotations.Add(FRotator(0, 0, 0));
	}

	TargetsArray.Empty();

	// Generate points respective to algorithm
	switch(AlgorithmType)
	{
	case EAlgorithmType::FibonacciSphere:
		FibonacciSphere(TrialByTargetsSettings, Locations);
		break;
	case EAlgorithmType::RandomSphere:
		RandomSphere(TrialByTargetsSettings, Locations);
		break;
	case EAlgorithmType::PotentialSphere:
		PotentialSphere(TrialByTargetsSettings, Locations);
		// Delete all points that are inside or outside sphere
		for (size_t i = 0; i < TrialByTargetsSettings.NumberOfTargetsToSpawn; i++)
		{
			constexpr float DeltaR = 100.0;
			float RealDistanceFromCenter = (Locations[i] - TrialByTargetsSettings.SphereCenter).Size();
			if (RealDistanceFromCenter > TrialByTargetsSettings.RadiusOnWhichTargetsWillSpawn + DeltaR || RealDistanceFromCenter < TrialByTargetsSettings.RadiusOnWhichTargetsWillSpawn - DeltaR)
			{
				Locations.Pop(false);
			}
		}
		Locations.Shrink();
		break;
	}

	TrialByTargetsSettings.NumberOfTargetsToSpawn = Locations.Num();


	SpawnMultipleTargets(WorldContext, ClassType, TrialByTargetsActor, TargetsArray, Locations, Rotations, TrialByTargetsSettings.TargetSize);

	return true;
}

/// <summary>
/// Generates uniformly spread points on a sphere using golden ratio
/// </summary>
/// <param name="Settings">FTrialByTargetsSettings struct with settings</param>
/// <param name="Locations">Array that will be filled with coordinates of spawned points</param>
void UTargetsFactory::FibonacciSphere(const FTrialByTargetsSettings& Settings, TArray<FVector>& Locations)
{
	TArray<FVector> GeneratedPoints;
	Locations.Reserve(Settings.NumberOfTargetsToSpawn);

	const size_t NumberOfGeneratedPoints = 100 + Settings.NumberOfTargetsToSpawn;
	GeneratedPoints.Reserve(NumberOfGeneratedPoints);

	// Golden angle in radians
	const float Phi = PI * (3 - sqrt(5));

	for (size_t i = 0; i < NumberOfGeneratedPoints; i++)
	{
		// Generating points on a unit sphere using golden ratio

		FVector NewPointOnAUnitSphere;
		// Y goes from 1 to -1
		NewPointOnAUnitSphere.Y = 1 - (i / static_cast<float>(NumberOfGeneratedPoints - 1)) * 2;
		// Radius at Y
		const float R = sqrt(1 - NewPointOnAUnitSphere.Y * NewPointOnAUnitSphere.Y);
		// Golden angle increment
		const float Theta = Phi * i;
		NewPointOnAUnitSphere.X = UKismetMathLibrary::Cos(Theta) * R;
		NewPointOnAUnitSphere.Z = UKismetMathLibrary::Sin(Theta) * R;

		// Clamp new point to non-unit sphere (Radius != 1)
		NewPointOnAUnitSphere *= Settings.RadiusOnWhichTargetsWillSpawn;
		// Shift all points to where the sphere center is
		NewPointOnAUnitSphere += Settings.SphereCenter;

		GeneratedPoints.Add(NewPointOnAUnitSphere);
	}

	// Shuffle array
	if (GeneratedPoints.Num() > 0)
	{
		const size_t LastIndex = GeneratedPoints.Num() - 1;
		for (size_t i = 0; i <= LastIndex; ++i)
		{
			const size_t Index = FMath::RandRange(static_cast<int32>(i), static_cast<int32>(LastIndex));
			if (i != Index)
			{
				GeneratedPoints.Swap(i, Index);
			}
		}
	}

	// Select some points to Locations array 
	for (size_t i = 0; i < Settings.NumberOfTargetsToSpawn; i++)
	{
		Locations.Add(GeneratedPoints[i]);
	}
}

/// <summary>
/// Generates random points on a sphere
/// </summary>
/// <param name="Settings">FTrialByTargetsSettings struct with settings</param>
/// <param name="Locations">Array that will be filled with coordinates of spawned points</param>
void UTargetsFactory::RandomSphere(const FTrialByTargetsSettings& Settings, TArray<FVector>& Locations)
{
	Locations.Reserve(Settings.NumberOfTargetsToSpawn);
	for (size_t i = 0; i < Settings.NumberOfTargetsToSpawn; i++)
	{
		FVector VectorFromCenterToSphere = UKismetMathLibrary::RandomUnitVector() * Settings.RadiusOnWhichTargetsWillSpawn;
		FVector SpawnPosition = Settings.SphereCenter + VectorFromCenterToSphere;
		Locations.Add(SpawnPosition);
	}
	
	// Check if there are no two points in one place
	for (size_t i = 0; i < Settings.NumberOfTargetsToSpawn - 1; i++)
	{
		if (Locations[i] == Locations[i + 1])
		{
			UE_LOG(LogTargets, Error, TEXT("Two points are at the same place. Deleting first point"));
			Locations.Pop(false);
		}
	}
	Locations.Shrink();
}

/// <summary>
/// Generates points on a sphere such that every point takes the best position with
///	minimum energy
/// </summary>
/// <param name="Settings">FTrialByTargetsSettings struct with settings</param>
/// <param name="Locations">Array that will be filled with coordinates of spawned points</param>
void UTargetsFactory::PotentialSphere(const FTrialByTargetsSettings& Settings, TArray<FVector>& Locations)
{
	Locations.Reserve(Settings.NumberOfTargetsToSpawn);

	// Fill Locations with random points
	RandomSphere(Settings, Locations);

	TArray<FVector> OldLocations = Locations;

	// For debug purposes
	TArray<FString> Print;
	Print.Reserve(Settings.NumberOfTargetsToSpawn);
	for (size_t i = 0; i < Settings.NumberOfTargetsToSpawn; i++)
	{
		FString String = "i: " + FString::SanitizeFloat(i, 0) + " \t\tLocation: " + Locations[i].ToString();
		Print.Add(String);
	}

	// Energy minimization algorithm from static mechanics
	size_t NumberOfMinimizationIterations = 50;
	for (size_t k = 0; k < NumberOfMinimizationIterations; k++)
	{
		for (size_t i = 0; i < Settings.NumberOfTargetsToSpawn; i++)
		{
			size_t InnerIteration = 0;
			while (InnerIteration < 5)
			{
				float DeltaLatticeParameter = Settings.MinDistanceBetweenTargets / 10.0;
				const float Ei = E(i, Locations);
				FVector DeltaEi = FVector(
					DeltaE(i, DeltaLatticeParameter, Ei, EXYZ::X, Locations),
					DeltaE(i, DeltaLatticeParameter, Ei, EXYZ::Y, Locations),
					DeltaE(i, DeltaLatticeParameter, Ei, EXYZ::Z, Locations));
				if (DeltaEi.Size() < SMALL_NUMBER)
				{
					break;
				}
				if (DeltaEi.Size() > 5)
				{
					UE_LOG(LogTargets, Display, TEXT(""));
				}
				// If shift energy isn't negligible then shift point
				FVector DeltaLatticeShift = FVector::ZeroVector;
				DeltaLatticeShift -= Settings.MinDistanceBetweenTargets * DeltaEi / DeltaEi.Size();
				Locations[i] += DeltaLatticeShift;

				// If over-shifted then step back and lower step
				if (E(i, Locations) >= Ei)
				{
					DeltaLatticeParameter /= 8;
					Locations[i] -= DeltaLatticeShift;
					InnerIteration++;
				}
			}
		}
	}

	// For debug purposes 
	for (size_t i = 0; i < Settings.NumberOfTargetsToSpawn; i++)
	{
		Print[i] += " \t\ti: " + FString::SanitizeFloat(i,0) + " \t\tLocation: " + Locations[i].ToString() + " \t\t Delta: " + FString::SanitizeFloat(FVector(Locations[i]-OldLocations[i]).Size());
		UE_LOG(LogTargets, Display, TEXT("%s"), *Print[i]);
	}
}

/// <summary>
/// Calculates shift energy for one point relative to other by shifting second
/// </summary>
/// <param name="Index"></param>
/// <param name="DeltaLatticeParameter"></param>
/// <param name="Coordinate">enum which corresponds for X,Y or Z coordinate to calculate</param>
/// <param name="Locations">Array with points positions</param>
/// <returns></returns>
float UTargetsFactory::DeltaE(size_t Index, float DeltaLatticeParameter, float Ei, enum EXYZ Coordinate, TArray<FVector>& Locations)
{
	switch (Coordinate)
	{
	case X:
		Locations[Index].X += DeltaLatticeParameter;
		break;
	case Y:
		Locations[Index].Y += DeltaLatticeParameter;
		break;
	case Z:
		Locations[Index].Z += DeltaLatticeParameter;
		break;
	}
	const float DeltaEi = E(Index, Locations) - Ei;
	switch (Coordinate)
	{
	case X:
		Locations[Index].X -= DeltaLatticeParameter;
		break;			   
	case Y:				   
		Locations[Index].Y -= DeltaLatticeParameter;
		break;			   
	case Z:				   
		Locations[Index].Z -= DeltaLatticeParameter;
		break;
	}
	return DeltaEi;
}

/// <summary>
/// Calculates energy for this point relative to others
/// </summary>
/// <param name="Index">Index of point to calculate energy for</param>
/// <param name="Locations">Array with points positions</param>
/// <returns>Total energy for point</returns>
float UTargetsFactory::E(size_t Index, TArray<FVector>& Locations)
{
	float E = 0;
	// Check for Index is not greater then size of the array
	if (Index > Locations.Num())
	{
		UE_LOG(LogTargets, Error, TEXT("Index is out of bounds"));
		return 0;
	}

	for (size_t i = 0; i < Locations.Num(); i++)
	{
		if (Index == i)
		{
			continue;
		}
		E += Potential(FVector(Locations[Index] - Locations[i]).Size());
	}
	
	return E;
}

/// <summary>
/// Calculates potential energy between two points. First point is at 0, second point is at R units away
/// </summary>
/// <param name="R">Position of second point</param>
/// <returns>Potential energy for point</returns>
float UTargetsFactory::Potential(float R)
{
	// Gaussian potential U(r) = exp(-r^2/2/sigma^2)
	//const float Sigma = 100;
	//return exp(-R * R / 2 / Sigma * Sigma);
	
	// Morse potential from wiki U(r) = D * (1 - exp(-a * (R-Re)))^2
	// https://en.wikipedia.org/wiki/Morse_potential
	// U(r) = De * (exp(-2*A*(R-Re) - exp(-2*A*(R-Re)))
	// R - distance between atoms
	// Re - equilibrium distance (lattice parameter)
	// De - well depth
	// A - width of potential (smaller A -> greater well)
	// const float De = 10, A = 0.1, Re = 3000*3;
	// return De * (exp(-2 * A * (R - Re)) - 2 * exp(-A * (R - Re)));

	// Lennard-Jones potential U(r) = 4*Epsilon * ((Sigma/R)^12 - (Sigma/R)^6)
	// https://en.wikipedia.org/wiki/Lennard-Jones_potential
	// Epsilon - the depth of the potential well
	// Sigma - the distance at which the particle-particle potential energy U is zero (size of particle)
	// Has minimum at R = Re = 2^(1/6) * Sigma
	const float Epsilon = -0.00001, Sigma = 80;//Sigma = 80*2 / pow(2, 1 / 6);
	return 4 * Epsilon * (pow(Sigma / R, 12) - pow(Sigma / R, 6));
}

/// <summary>
/// Finds great arc length between two points on a sphere
/// </summary>
/// <param name="First">Position of first point relative to (0,0,0)</param>
/// <param name="Second">Position of second point relative to (0,0,0)</param>
/// <param name="Center">Position of sphere center relative to (0,0,0)</param>
/// <returns>Angle between two points in degrees</returns>
float UTargetsFactory::GreatArcDistance(const FVector& First, const FVector& Second, const FVector& Center)
{
	FVector FirstNormalizedRelativeToCenter = FVector(First - Center).GetSafeNormal();
	FVector SecondNormalizedRelativeToCenter = FVector(Second - Center).GetSafeNormal();

	float DotProduct = UKismetMathLibrary::Dot_VectorVector(FirstNormalizedRelativeToCenter, SecondNormalizedRelativeToCenter);
	float AngleBetweenVectors = UKismetMathLibrary::DegAcos(DotProduct);
	float SphereRadius = FVector(First - Center).Size();

	return SphereRadius * AngleBetweenVectors;
}
