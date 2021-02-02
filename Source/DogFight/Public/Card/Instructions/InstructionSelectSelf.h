// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Card/Instructions/CardInstructionBase.h"
#include "InstructionSelectSelf.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UInstructionSelectSelf : public UCardInstructionBase
{
	GENERATED_BODY()

public:
	virtual void Execute() override;
};
