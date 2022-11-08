#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DogFightUtilsFunctionLibrary.generated.h"

UCLASS()
class DOGFIGHT_API UDogFightUtilsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static FString CreateLuaScriptByTemplate(FString Template, FString Path, FString ScriptName);
};
