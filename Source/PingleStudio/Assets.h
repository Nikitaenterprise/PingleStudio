// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Assets.generated.h"

class ATrialByTargets;
class ATarget;
class UTrialByTargetsWidget;
/**
 * 
 */
UCLASS(BlueprintType)
class PINGLESTUDIO_API UAssets : public UDataAsset
{
	GENERATED_BODY()

private:

	// This property represents which Target class to spawn
	UPROPERTY(EditAnywhere, Category = "Classes", meta = (AllowPrivateAcces = "true"))
	TSubclassOf<ATarget> TargetClass;
	// This property represents which TrialByTargets class to spawn
	UPROPERTY(EditAnywhere, Category = "Classes", meta = (AllowPrivateAcces = "true"))
	TSubclassOf<ATrialByTargets> TrialByTargetsClass;
	// This property represents which widget should be used for TrialByTargets UI
	UPROPERTY(EditAnywhere, Category = "Classes", meta = (AllowPrivateAcces = "true"))
	TSubclassOf<UTrialByTargetsWidget> TrialByTargetsWidgetClass;

public:

	// Returns copy of TSubclassOf<ATarget> TargetClass
	UFUNCTION(BlueprintCallable)
	TSubclassOf<ATarget> GetTargetClass() const { return TargetClass; }
	// Returns copy of TSubclassOf<ATrialByTargets> TrialByTargetsClass
	UFUNCTION(BlueprintCallable)
	TSubclassOf<ATrialByTargets> GetTrialByTargetsClass() const { return TrialByTargetsClass; }
	// Returns copy of TSubclassOf<UTrialByTargetsWidget> TrialByTargetsWidgetClass
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UTrialByTargetsWidget> GetTrialByTargetsWidgetClass() const { return TrialByTargetsWidgetClass; }
};
