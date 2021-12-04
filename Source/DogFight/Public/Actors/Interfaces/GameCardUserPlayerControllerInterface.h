#pragma once

#include "CoreMinimal.h"
#include "Card/GameCardTypes.h"
#include "GameCardUserPlayerControllerInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, meta=(CannotImplementInterfaceInBlueprint))
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
	UFUNCTION(BlueprintCallable, Category="GameCardUserPlayerControllerInterface")
	/** Get the actual pawn controlling by this PlayerController. */
	virtual APawn* GetActualPawn() const = 0;

	/** Broadcast card using message with target information. */
	virtual void BroadcastCardTargetingResult(FText CardName, FText TargetText, ECardInstructionTargetType TargetType) = 0;
};
