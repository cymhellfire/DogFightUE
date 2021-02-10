#pragma once

#include "CoreMinimal.h"
#include "Card/GameCardTypes.h"
#include "GameTargetProviderInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UGameTargetProviderInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCardTargetInfoAcquiredSignature, FCardInstructionTargetInfo&, TargetInfo);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetActorSelectedSignature, AActor**);
/**
 * Implement this interface to provide target information to customizable cards.
 */
class IGameTargetProviderInterface
{
	GENERATED_BODY()

public:

	virtual FCardTargetInfoAcquiredSignature& GetTargetInfoAcquiredDelegate() = 0;

	virtual FOnTargetActorSelectedSignature& GetOnTargetActorSelectedDelegate() = 0;

	virtual void RequestActorTarget() {};

	virtual void RequestPositionTarget() {};

	virtual void RequestDirectionTarget() {};

	virtual FCardInstructionTargetInfo RequestRandomActorTarget(bool bIgnoreSelf) { return FCardInstructionTargetInfo(); };

	virtual FCardInstructionTargetInfo RequestRandomPositionTarget() { return FCardInstructionTargetInfo(); };

	virtual FCardInstructionTargetInfo RequestRandomDirectionTarget() { return FCardInstructionTargetInfo(); };
};
