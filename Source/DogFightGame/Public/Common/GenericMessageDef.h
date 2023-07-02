#pragma once

#include "GenericMessageDef.generated.h"

USTRUCT(BlueprintType)
struct FGenericMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="GenericMessage")
	FText Title;

	UPROPERTY(BlueprintReadWrite, Category="GenericMessage")
	FText Content;
};
