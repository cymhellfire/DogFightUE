#include "GameService/LocalizationService.h"

#include "UnLuaLegacy.h"
#include "UnLuaModule.h"
#include "Common/LuaIntegrationLog.h"
#include "Internationalization/StringTable.h"

void ULocalizationService::Startup()
{
	Super::Startup();

	// Load all localization string table
	LoadData();

	RegisterLuaFunction();
}

void ULocalizationService::LoadData()
{
	// Load all string tables
	for (auto SoftPtr : LocalizationTables)
	{
		if (SoftPtr.IsNull())
			continue;

		auto LocTable = SoftPtr.IsValid() ? SoftPtr.Get() : SoftPtr.LoadSynchronous();
		if (IsValid(LocTable))
		{
			LocalizationPath.Add(SoftPtr.GetAssetName(), SoftPtr.ToString());
		}
	}
}

void ULocalizationService::RegisterLuaFunction()
{
	lua_State* l = IUnLuaModule::Get().GetEnv(this)->GetMainState();
	lua_register(l, "GetLocalizedString", Lua_LocalizeString);
}

int ULocalizationService::Lua_LocalizeString(lua_State* L)
{
	ULocalizationService* LocalizationService = GetGameServiceBySuperClass<ULocalizationService>();
	if (!LocalizationService)
	{
		return 0;
	}

	int32 n = lua_gettop(L);

	// Get string table name
	FString TableName;
	if (UnLua::IsType(L, 1, UnLua::TType<FString>()))
	{
		TableName = UnLua::Get(L, 1, UnLua::TType<FString>());
	}

	// Get key
	FString KeyName;
	if (UnLua::IsType(L, 2, UnLua::TType<FString>()))
	{
		KeyName = UnLua::Get(L, 2, UnLua::TType<FString>());
	}

	FText LocalizeStr = LocalizationService->GetLocalizeString(TableName, KeyName);
	if (n == 2)
	{
		UnLua::Push(L, LocalizeStr);
		return 1;
	}

	FFormatOrderedArguments FormatArguments;
	for (int32 i = 3; i <= n; ++i)
	{
		if (lua_isnumber(L, i))
		{
			FormatArguments.Add(FFormatArgumentValue(lua_tonumber(L, i)));
		}
		else if (UnLua::IsType(L, i, UnLua::TType<FText>()))
		{
			FormatArguments.Add(FFormatArgumentValue(UnLua::Get(L, i, UnLua::TType<FText>())));
		}
		else if (UnLua::IsType(L, i, UnLua::TType<FName>()))
		{
			FormatArguments.Add(FFormatArgumentValue(FText::FromName(UnLua::Get(L, i, UnLua::TType<FName>()))));
		}
		else if (UnLua::IsType(L, i, UnLua::TType<FString>()))
		{
			FormatArguments.Add(FFormatArgumentValue(FText::FromString(UnLua::Get(L, i, UnLua::TType<FString>()))));
		}
		else
		{
			ensureAlwaysMsgf(0, TEXT("Invalid argument type to localize with."));
		}
	}

	FText FormatStr = FText::Format(LocalizeStr, FormatArguments);
	UnLua::Push(L, FormatStr);
	return 1;
}

FText ULocalizationService::GetLocalizeString(const FString& InTable, const FString& InKey)
{
	if (LocalizationPath.Contains(InTable))
	{
		return FText::FromStringTable(FName(LocalizationPath[InTable]), InKey);
	}

	return FText::FromString(FString::Printf(TEXT("[Missing table %s] %s"), *InTable, *InKey));
}

FText ULocalizationService::GetLocalizeStringWithFormat(const FString& InTable, const FString& InKey,
	const TArray<FString>& ParamList)
{
	if (LocalizationPath.Contains(InTable))
	{
		auto FormatText = GetLocalizeString(InTable, InKey);
		if (!FormatText.IsEmpty())
		{
			FFormatOrderedArguments FormatArguments;
			for (auto& Param : ParamList)
			{
				FormatArguments.Add(FFormatArgumentValue(FText::FromString(Param)));
			}
			return FText::Format(FormatText, FormatArguments);
		}
	}

	return FText();
}
