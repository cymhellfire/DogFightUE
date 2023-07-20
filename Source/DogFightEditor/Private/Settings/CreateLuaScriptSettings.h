// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CreateLuaScriptSettings.generated.h"

UENUM(BlueprintType)
enum class ECreateLuaScriptTabType : uint8
{
	TT_Default		UMETA(DisplayName="Default"),
	TT_Group		UMETA(DisplayName="Group"),
};

UENUM(BlueprintType)
enum class ECreateLuaScriptGroupFolderNameType : uint8
{
	FNT_ScriptName	UMETA(DisplayName="ScriptName"),
	FNT_Fixed		UMETA(DisplayName="Fixed"),
};

enum class ELuaScriptNameTemplateMode : uint8;

USTRUCT(BlueprintType)
struct FCreateLuaScriptTemplateSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateLuaScriptSettings")
	FString TemplateName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateLuaScriptSettings")
	bool bOverrideShortName;

	/**
	 * The name used as prefix or suffix.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateLuaScriptSettings", meta=(EditConditionHides, EditCondition="bOverrideShortName==true"))
	FString ShortName;
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

USTRUCT(BlueprintType)
struct FCreateLuaScriptTabSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateLuaScriptTabSettings", Config)
	FString TabName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateLuaScriptTabSettings", Config)
	ECreateLuaScriptTabType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateLuaScriptTabSettings", Config)
	TArray<FCreateLuaScriptModuleSettings> ModuleList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateLuaScriptTabSettings", Config)
	TArray<FCreateLuaScriptTemplateSettings> TemplateList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateLuaScriptTabSettings", Config)
	ELuaScriptNameTemplateMode DefaultTemplateNameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateLuaScriptTabSettings", meta=(EditConditionHides, EditCondition="Type==ECreateLuaScriptTabType::TT_Group"))
	ECreateLuaScriptGroupFolderNameType FolderNameType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CreateLuaScriptTabSettings", meta=(EditConditionHides, EditCondition="FolderNameType==ECreateLuaScriptGroupFolderNameType::FNT_Fixed"))
	FString OverrideSubfolder;
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
	TArray<FCreateLuaScriptTabSettings> TabList;
};
