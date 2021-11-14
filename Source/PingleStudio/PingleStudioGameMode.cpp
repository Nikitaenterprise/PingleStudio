// Copyright Epic Games, Inc. All Rights Reserved.

#include "PingleStudioGameMode.h"
#include "PingleStudioHUD.h"
#include "PingleStudioCharacter.h"
#include "Assets.h"
#include "UObject/ConstructorHelpers.h"

APingleStudioGameMode::APingleStudioGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = APingleStudioHUD::StaticClass();

	static ConstructorHelpers::FObjectFinder<UAssets> AssetsClassFinder(TEXT("/Game/Assets_BP.Assets_BP"));
	Assets = AssetsClassFinder.Object;
}