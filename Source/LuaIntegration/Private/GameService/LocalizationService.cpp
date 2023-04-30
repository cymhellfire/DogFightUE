#include "GameService/LocalizationService.h"

#include "Common/LuaIntegrationLog.h"
#include "Internationalization/StringTable.h"

void ULocalizationService::Startup()
{
	Super::Startup();

	// Load all localization string table
	LoadData();
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
