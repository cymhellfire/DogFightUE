// Copyright Epic Games, Inc. All Rights Reserved.

#include "DogFightGameMode.h"
#include "DogFightPlayerController.h"
#include "DogFightCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADogFightGameMode::ADogFightGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ADogFightPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}