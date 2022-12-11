#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DogFightUtilsFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct FLuaScriptCreateArgument
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString ModuleName;

	UPROPERTY(BlueprintReadWrite)
	FString TemplateName;

	UPROPERTY(BlueprintReadWrite)
	FString Path;

	UPROPERTY(BlueprintReadWrite)
	FString ScriptName;

	UPROPERTY(BlueprintReadWrite)
	bool bUsePrefix;
};

UCLASS()
class DOGFIGHT_API UDogFightUtilsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static FString GenerateLuaScriptPath(const FLuaScriptCreateArgument& InArgument);

	UFUNCTION(BlueprintCallable)
	static FString CreateLuaScriptByTemplate(const FLuaScriptCreateArgument& InArgument);
};
