// Fill out your copyright notice in the Description page of Project Settings.


#include "TrialByTargets.h"

#include "Assets.h"
#include "PingleStudioGameMode.h"
#include "Target.h"
#include "Kismet/KismetMathLibrary.h"
#include "TargetsFactory.h"
#include "TrialByTargetsWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Log.h"

// Sets default values
ATrialByTargets::ATrialByTargets()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetVisibility(false);
}

// Called when the game starts or when spawned
void ATrialByTargets::BeginPlay()
{
	Super::BeginPlay();

	ParticleSystemComponent->Deactivate();

	const APingleStudioGameMode* GameMode = Cast<APingleStudioGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GameMode))
	{
		UE_LOG(LogTargets, Error, TEXT("APingleStudioGameMode is nullptr in ATrialByTargets::BeginPlay()"));
		return;
	}

	if (!IsValid(GameMode->Assets))
	{
		UE_LOG(LogTargets, Error, TEXT("UAssets is nullptr in ATrialByTargets::BeginPlay()"));
		return;
	}

	// Bind data for widget and add widget to viewport
	TSubclassOf<UTrialByTargetsWidget> Subclass = GameMode->Assets->GetTrialByTargetsWidgetClass();
	UTrialByTargetsWidget* TrialByTargetsWidgetTest = CreateWidget<UTrialByTargetsWidget>(GetWorld(), Subclass);
	if(!IsValid(TrialByTargetsWidgetTest))
	{
		UE_LOG(LogTargets, Error, TEXT("UTrialByTargetsWidget is nullptr in ATrialByTargets::BeginPlay()"));
	}
	else
	{
		TrialByTargetsWidget = TrialByTargetsWidgetTest;
		TrialByTargetsWidget->AddToViewport();
		TrialByTargetsWidget->SetVisibility(ESlateVisibility::Visible);
		TrialByTargetsWidget->BindNumberOfDestroyedTargets(&NumberOfDestroyedTargets);
		TrialByTargetsWidget->BindWaveNumber(&WaveNumber);
	}

	SpawnPointForNextWave = GetOwner()->GetActorLocation();
	StartWave();
}

// Called every frame
void ATrialByTargets::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (NumberOfDestroyedTargets>=NumberOfTargetsToDestroyToEndWave)
	{
		EndWave();
		
		// Set timer to start new wave
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("StartWave"));
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, TimeBetweenWaves, false);
	}

}

void ATrialByTargets::Destroyed()
{
	Super::Destroyed();
}

FTrialByTargetsSettings ATrialByTargets::GetSettings() const
{
	FTrialByTargetsSettings Settings;
	Settings.NumberOfTargetsToSpawn = NumberOfTargetsToSpawn * (1 + WaveNumber * NumberOfTargetsToSpawnMultiplier);
	Settings.RadiusOnWhichTargetsWillSpawn = SpawningSphereRadius * (1 + WaveNumber * SpawningSphereRadiusMultiplier);
	Settings.TargetSize = StartingTargetSize + (1 + WaveNumber * TargetSizeMultiplier);
	Settings.SphereCenter = SpawnPointForNextWave;
	Settings.MinDistanceBetweenTargets = 2 * Settings.TargetSize + MinDistanceBetweenTargets;
	return Settings;
}

void ATrialByTargets::StartWave()
{
	const APingleStudioGameMode* GameMode = Cast<APingleStudioGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GameMode))
	{
		UE_LOG(LogTargets, Error, TEXT("APingleStudioGameMode is nullptr in ATrialByTargets::StartWave()"));
		return;
	}

	if (!IsValid(GameMode->Assets))
	{
		UE_LOG(LogTargets, Error, TEXT("UAssets is nullptr in ATrialByTargets::StartWave()"));
		return;
	}

	UClass* TargetClass = GameMode->Assets->GetTargetClass();
	if (!IsValid(TargetClass))
	{
		UE_LOG(LogTargets, Error, TEXT("TargetClass is nullptr in ATrialByTargets::StartWave()"));
		return;
	}

	WaveNumber++;
	
	UTargetsFactory::SpawnTargetsOnSphere(
		GetWorld(), 
		TargetClass,
		this,
		TargetsArray,
		GetSettings(),
		AlgorithmType);
	NumberOfActiveTargets = TargetsArray.Num();
}

void ATrialByTargets::EndWave()
{
	for (const auto& Target : TargetsArray)
	{
		Target->Destroy();
	}
	TargetsArray.Empty();
	NumberOfActiveTargets = 0;
	NumberOfDestroyedTargets = 0;

	// Set new spawn point
	if (IsValid(GetOwner()))
	{
		SpawnPointForNextWave = GetOwner()->GetActorLocation();
	}
}

void ATrialByTargets::TargetDestroyed(ATarget* Target)
{
	if (TargetsArray.Find(Target) == INDEX_NONE)
	{
		UE_LOG(LogTargets, Error, TEXT("ATrialByTargets has no such actor %s in it. Cant't remove it"), *Target->GetName());
		return;
	}
	TargetsArray.Remove(Target);
	NumberOfActiveTargets--;
	NumberOfDestroyedTargets++;

	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		ParticleSystemComponent->Template,
		Target->GetActorLocation(),
		Target->GetActorRotation(),
		FVector(1, 1, 1));
}

