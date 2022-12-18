#pragma once

#include "Player/DogFightPlayerController.h"
#include "FreeForAllPlayerController.generated.h"

UCLASS()
class DOGFIGHTGAME_API AFreeForAllPlayerController : public ADogFightPlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

protected:
	UFUNCTION(Server, Reliable)
	void ServerMarkPlayerReady();

	UFUNCTION()
	void OnTimerExpired();

protected:
	FTimerHandle RandomTimer;
};
