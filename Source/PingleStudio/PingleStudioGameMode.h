// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PingleStudioGameMode.generated.h"

class UAssets;
UCLASS(minimalapi)
class APingleStudioGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	UAssets* Assets;

public:
	APingleStudioGameMode();

};



