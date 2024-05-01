#pragma once

#include "lua.hpp"
#include "Engine/DataTable.h"
#include "GameService/GameService.h"
#include "LocalizationService.generated.h"

class UStringTable;

UCLASS(Abstract, Blueprintable)
class LUAINTEGRATION_API ULocalizationService : public UGameService
{
	GENERATED_BODY()

public:
	virtual void Startup() override;

	FText GetLocalizeString(const FString& InTable, const FString& InKey);

	FText GetLocalizeStringWithFormat(const FString& InTable, const FString& InKey, const TArray<FString>& ParamList);

protected:
	void LoadData();

	void RegisterLuaFunction();

	static int Lua_LocalizeString(lua_State* L);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LocalizationService")
	TArray<TSoftObjectPtr<UStringTable>> LocalizationTables; 

	TMap<FString, FString> LocalizationPath;
};