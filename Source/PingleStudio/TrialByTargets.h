// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetsFactory.h"
#include "GameFramework/Actor.h"
#include "TrialByTargets.generated.h"

class ATarget;
class UTargetsFactory;
class UTrialByTargetsWidget;
struct FTrialByTargetsSettings;
enum class EAlgorithmType : uint8;

UCLASS()
class PINGLESTUDIO_API ATrialByTargets : public AActor
{
	GENERATED_BODY()

public:

	// Sphere radius on surface of which targets will spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trial")
	float SpawningSphereRadius = 2000;

	// Sphere radius multiplier for spawning new wave
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trial")
	float SpawningSphereRadiusMultiplier = 0.05;

	// Starting target size (radius)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trial")
	int StartingTargetSize = 10;

	// Decrease radius step. This is a parameter that will be
	// multiplied by target radius after collision with projectile
	// If this parameter is < 1 then it will decrease radius,
	// otherwise it will increase it. Couldn't be zero or negative
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trial")
	float TargetSizeMultiplier = 0.5;

	// Minimum distance between targets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trial")
	float MinDistanceBetweenTargets = 80;

	// This number of targets will spawn in first wave
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trial")
	int NumberOfTargetsToSpawn = 15;

	// Multiplier for spawning new targets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trial")
	float NumberOfTargetsToSpawnMultiplier = 0.1;

	// When number of destroyed targets get to this number the wave ends
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trial")
	int NumberOfTargetsToDestroyToEndWave = 10;

	// Current number of destroyed targets
	UPROPERTY(BlueprintReadOnly, Category = "Trial")
	int NumberOfDestroyedTargets= 0;

	// Number of active targets
	UPROPERTY(BlueprintReadOnly, Category = "Trial")
	int NumberOfActiveTargets;

	// This property will be incremented with every completed wave 
	UPROPERTY(BlueprintReadOnly, Category = "Trial")
	int WaveNumber = 0;

	// Time in seconds between two end of one wave and the
	// beginning of the another one
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trial")
	float TimeBetweenWaves = 5.0;

	// Array of spawned targets
	UPROPERTY(BlueprintReadOnly, Category = "Trial")
	TArray<ATarget*> TargetsArray;

	// Pointer to UTrialByTargetsWidget UI
	UPROPERTY(BlueprintReadOnly, Category = "Trial")
	UTrialByTargetsWidget* TrialByTargetsWidget = nullptr;

	// Particle system which is spawned after each target death
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trial")
	UParticleSystemComponent* ParticleSystemComponent = nullptr;

	// Enum for selecting algorithm type
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trial")
	EAlgorithmType AlgorithmType = EAlgorithmType::FibonacciSphere;

	// Position at which sphere will be spawned at each wave
	UPROPERTY(BlueprintReadOnly, Category = "Trial")
	FVector SpawnPointForNextWave;
	
public:

	// Sets default values for this actor's properties
	ATrialByTargets();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	// Fills FTrialByTargetsSettings struct with parameters
	FTrialByTargetsSettings GetSettings() const;
	UFUNCTION()
	void StartWave();
	UFUNCTION(BlueprintCallable)
	void EndWave();
	// Called from ATarget class to notify this class about destruction
	void TargetDestroyed(ATarget* Target);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
