#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DogFightUtilsFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class ELuaScriptNameTemplateMode : uint8
{
	TM_None,
	TM_Prefix,
	TM_Suffix,
};

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
	ELuaScriptNameTemplateMode TemplateMode;

	UPROPERTY(BlueprintReadWrite)
	bool bOverrideTemplateFolder;

	UPROPERTY(BlueprintReadWrite)
	FString OverrideFolder;
};

UCLASS()
class DOGFIGHT_API UDogFightUtilsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static FString GenerateLuaScriptFolder(const FLuaScriptCreateArgument& InArgument);

	UFUNCTION(BlueprintCallable)
	static FString GenerateLuaScriptPath(const FLuaScriptCreateArgument& InArgument);

	UFUNCTION(BlueprintCallable)
	static FString CreateLuaScriptByTemplate(const FLuaScriptCreateArgument& InArgument);
};
