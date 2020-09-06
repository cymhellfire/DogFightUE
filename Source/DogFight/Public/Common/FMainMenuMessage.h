#pragma once

#include "CoreMinimal.h"
#include "FMainMenuMessage.generated.h"

USTRUCT(BlueprintType)
struct FMainMenuMessage
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FText MessageTitle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FText MessageContent;
};
