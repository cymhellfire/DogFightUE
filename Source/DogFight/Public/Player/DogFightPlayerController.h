// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "DogFightTypes.h"
#include "GameFramework/PlayerController.h"
#include "DogFightPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API ADogFightPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	/** Return the client to main menu gracefully. */
	UFUNCTION(Client, Reliable)
	virtual void RpcReturnToMainMenuWithReason(EReturnToMainMenuReason::Type Reason);

	virtual void HandleReturnToMainMenu();

protected:

	/** End and/or destroy current game session. */
	virtual void CleanupSessionOnReturnMain();
};
