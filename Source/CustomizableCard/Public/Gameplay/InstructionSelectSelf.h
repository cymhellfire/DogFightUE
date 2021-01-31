// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/CardInstructionBase.h"
#include "InstructionSelectSelf.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMIZABLECARD_API UInstructionSelectSelf : public UCardInstructionBase
{
	GENERATED_BODY()

public:
	virtual void Execute() override;
};
