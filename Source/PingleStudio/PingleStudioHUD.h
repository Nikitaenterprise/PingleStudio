// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PingleStudioHUD.generated.h"


UCLASS()
class APingleStudioHUD : public AHUD
{
	GENERATED_BODY()

public:
	APingleStudioHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

