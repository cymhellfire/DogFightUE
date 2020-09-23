// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "CardInstructionBase.h"
#include "CustomizableCardTypes.h"
#include "TargetingInstructionBase.generated.h"

class IGameTargetProviderInterface;

/**
 * Instructions can acquire target information for owning card.
 */
UCLASS()
class CUSTOMIZABLECARD_API UTargetingInstructionBase : public UCardInstructionBase
{
	GENERATED_BODY()

public:
	UTargetingInstructionBase(const FObjectInitializer& ObjectInitializer);

	virtual void Execute() override;

protected:

	void RequestTarget();

	UFUNCTION()
	void OnTargetAcquired(FCardInstructionTargetInfo& TargetInfo);

public:
	/** Type of target that required. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Instruction")
	ECardInstructionTargetType TargetType;

	/** The total count of targets. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Instruction")
	int32 TargetCount;

protected:

	IGameTargetProviderInterface* TargetProvider;

	int32 TargetAcquired;
};
