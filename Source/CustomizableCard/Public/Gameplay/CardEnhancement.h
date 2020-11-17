// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CardEnhancement.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMIZABLECARD_API UCardEnhancement : public UObject
{
	GENERATED_BODY()

public:

	virtual void ApplyEnhancement();

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="CardEnhancement")
	int32 EnhancementCost;
};
