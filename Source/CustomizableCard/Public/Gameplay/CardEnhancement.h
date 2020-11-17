// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "CustomizableCardTypes.h"
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

	virtual void ApplyEnhancement() {};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="CardEnhancement")
	FCardDescriptionItemInfo GetDescriptionItemInfo();

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="CardEnhancement")
	int32 EnhancementCost;
};
