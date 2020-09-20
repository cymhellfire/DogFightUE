#pragma once

#include "CoreMinimal.h"
#include "CustomizableCardTypes.h"
#include "GameTargetProviderInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UGameTargetProviderInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCardTargetInfoAcquiredSignature, FCardInstructionTargetInfo&, TargetInfo);

/**
 * Implement this interface to provide target information to customizable cards.
 */
class IGameTargetProviderInterface
{
	GENERATED_BODY()

public:

	virtual FCardTargetInfoAcquiredSignature& GetTargetInfoAcquiredDelegate() = 0;

	virtual void RequestActorTarget() {};

	virtual void RequestPositionTarget() {};

	virtual void RequestDirectionTarget() {};
};
