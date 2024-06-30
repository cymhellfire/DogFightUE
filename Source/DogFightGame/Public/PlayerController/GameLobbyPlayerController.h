// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Player/DogFightPlayerController.h"
#include "Subsystem/InGameStatistics/GameLobbyPlayerInfo.h"
#include "GameLobbyPlayerController.generated.h"

UCLASS()
class DOGFIGHTGAME_API AGameLobbyPlayerController : public ADogFightPlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameLobbyPlayerController();

	UFUNCTION(Server, Reliable)
	void ServerSetPlayerInfo(const FGameLobbyPlayerInfo& PlayerInfo);

	UFUNCTION(Server, Reliable)
	void ServerMarkPlayerReady(bool bIsReady);

	UFUNCTION(Server, Reliable)
	void ServerSelectAvatarId(const FGameLobbyPlayerAvatarId& AvatarId);

	virtual void ClientReturnToMainMenuWithReason2_Implementation(EReturnToMainMenuReason::Type Reason) override;

	virtual void ClientPreStartGame_Implementation() override;

	virtual void HandleReturnToMainMenu2() override;

protected:

	virtual void GatherPlayerInfo() override;
};
