// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "Common/LocalizedString.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FrameworkLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTFRAMEWORK_API UFrameworkLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="DogFight|Framework")
	static FText GetTextFromLocalizedString(const FLocalizedString& LocalizedString);

	UFUNCTION(BlueprintCallable, Category="DogFight|Framework", meta=(WorldContext="WorldContextObject"))
	static void LoadGameMap(const UObject* WorldContextObject, const FString& MapPath);

	UFUNCTION(BlueprintCallable, Category="DogFight|Framework", meta=(WorldContext="WorldContextObject"))
	static bool IsCurrentMap(const UObject* WorldContextObject, const FString& MapPath);
};
