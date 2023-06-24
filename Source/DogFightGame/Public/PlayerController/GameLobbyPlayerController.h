// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Player/DogFightPlayerController.h"
#include "GameLobbyPlayerController.generated.h"

UCLASS()
class DOGFIGHTGAME_API AGameLobbyPlayerController : public ADogFightPlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameLobbyPlayerController();

	UFUNCTION(Server, Reliable)
	void ServerMarkPlayerReady(bool bIsReady);
};
