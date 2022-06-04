#pragma once

#include "CoreMinimal.h"
#include "MainMenuMessage.generated.h"

USTRUCT(BlueprintType)
struct PROJECTFRAMEWORK_API FMainMenuMessage
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FText MessageTitle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FText MessageContent;
};
