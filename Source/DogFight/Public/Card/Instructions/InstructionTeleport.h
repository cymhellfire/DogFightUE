// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Card/Instructions/HandleTargetInstructionBase.h"
#include "InstructionTeleport.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UInstructionTeleport : public UHandleTargetInstructionBase
{
	GENERATED_BODY()

public:
	UInstructionTeleport(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void HandlePositionTarget(FVector Position) override;

	UFUNCTION()
	void OnTeleportFinished(class UActorTeleportComponent* Component);
};
