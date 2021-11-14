// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TrialByTargetsWidget.generated.h"

/**
 * 
 */
UCLASS()
class PINGLESTUDIO_API UTrialByTargetsWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	const int* NumberOfDestroyedTargets = nullptr;
	const int* WaveNumber = nullptr;

public:

	void BindNumberOfDestroyedTargets(const int* NumberOfDestroyedTargetsToBind) { NumberOfDestroyedTargetsToBind ? NumberOfDestroyedTargets = NumberOfDestroyedTargetsToBind : NumberOfDestroyedTargets = nullptr; }
	void BindWaveNumber(const int* WaveNumberToBind) { WaveNumberToBind ? WaveNumber = WaveNumberToBind : WaveNumber = nullptr; }
	UFUNCTION(BlueprintCallable)
	int GetNumberOfDestroyedTargets() const { return NumberOfDestroyedTargets ? *NumberOfDestroyedTargets : 0; }
	UFUNCTION(BlueprintCallable)
	int GetWaveNumber() const { return WaveNumber ? *WaveNumber : 0; }
};
