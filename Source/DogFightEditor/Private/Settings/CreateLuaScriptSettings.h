// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CreateLuaScriptSettings.generated.h"

USTRUCT(BlueprintType)
struct FCreateLuaScriptTemplateSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateLuaScriptSettings")
	FString TemplateName;
};

USTRUCT(BlueprintType)
struct FCreateLuaScriptModuleSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateLuaScriptSettings")
	FString ModuleName;

	/**
	 * Whether override the output sub folder. (Use same folder as the ModuleName if false)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateLuaScriptSettings")
	bool bOverridePath;

	/**
	 * Override path when create new script.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateLuaScriptSettings", meta=(EditConditionHides, EditCondition="bOverridePath==true"))
	FString OverridePath;
};

/**
 * Settings of create lua scripts window.
 */
UCLASS(Config=Game, DefaultConfig)
class DOGFIGHTEDITOR_API UCreateLuaScriptSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateLuaScriptSetting", Config)
	TArray<FCreateLuaScriptModuleSettings> ModuleList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateLuaScriptSetting", Config)
	TArray<FCreateLuaScriptTemplateSettings> TemplateList;
};
