// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameCardTypes.h"
#include "UObject/NoExportTypes.h"
#include "CardEnhancement.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UCardEnhancement : public UObject
{
	GENERATED_BODY()

public:

	virtual void ApplyEnhancement() {};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="CardEnhancement")
	FCardDescriptionItemInfo GetDescriptionItemInfo();

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="CardEnhancement")
	int32 EnhancementCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CardEnhancement")
	FString EnhancementDescription;

	/** The prefix will be appended to card name. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CardEnhancement")
	FString EnhancementPrefix;
};
