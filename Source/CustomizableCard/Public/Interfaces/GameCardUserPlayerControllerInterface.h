#pragma once

#include "CoreMinimal.h"
#include "CustomizableCardTypes.h"
#include "GameCardUserPlayerControllerInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UGameCardUserPlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Only PlayerController implemented this interface can use Card correctly.
 */
class IGameCardUserPlayerControllerInterface
{
	GENERATED_BODY()

public:
	/** Get the actual pawn controlling by this PlayerController. */
	virtual APawn* GetActualPawn() = 0;

	/** Broadcast card using message with target information. */
	virtual void BroadcastCardTargetingResult(FText CardName, FText TargetText, ECardInstructionTargetType TargetType) = 0;
};
