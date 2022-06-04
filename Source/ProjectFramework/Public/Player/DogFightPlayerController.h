// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "Common/FrameworkType.h"
#include "GameFramework/PlayerController.h"
#include "DogFightPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTFRAMEWORK_API ADogFightPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	/** Return the client to main menu gracefully. */
	UFUNCTION(Client, Reliable)
	virtual void ClientReturnToMainMenuWithReason(EReturnToMainMenuReason::Type Reason);

	virtual void HandleReturnToMainMenu();
 
	/** Send just before the host start game. */
	UFUNCTION(Client, Reliable)
	virtual void ClientPreStartGame();

	/**
	 * Gather all information and send to server (only for host).
	 * Client use OnRep_PlayerState to send player info.
	 */
	UFUNCTION(Client, Reliable)
	virtual void ClientHostUploadPlayerInfo();

protected:

	/** End and/or destroy current game session. */
	virtual void CleanupSessionOnReturnMain();

	/** Gather player information and send to game server. */
	virtual void GatherPlayerInfo() {};

public:
	virtual void OnRep_PlayerState() override;
};
