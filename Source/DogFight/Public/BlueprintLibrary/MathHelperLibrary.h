// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathHelperLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UMathHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	* Find the smallest positive missing number in array that contains both
	* positive and negative integers.
	*/
	UFUNCTION(BlueprintCallable, Category="Math|Dog Fight")
	static int32 GetMinMissingPositiveNumber(TArray<int32> InArray, int32 MinNumber);
};
