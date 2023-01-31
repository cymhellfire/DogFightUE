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
	if (!IsValid(LocalizationData))
	{
		return;
	}

	// Get all rows in the table
	TArray<FLocalizationServiceDataRow*> AllRows;
	LocalizationData->GetAllRows("", AllRows);

	// Iterate through the array and load all data
	for (auto Row : AllRows)
	{
		if (Row->DataTable.IsNull())
		{
			continue;
		}

		auto LoadedTable = Row->DataTable.LoadSynchronous();
		if (IsValid(LoadedTable))
		{
			LocalizationPath.Add(Row->DataTable.GetAssetName(), Row->DataTable.ToString());
		}
		else
		{
			UE_LOG(LogLuaIntegration, Error, TEXT("[LocalizationService] Failed to load string table with path: %s"),
				*Row->DataTable->GetPathName());
		}
	}
}

FText ULocalizationService::GetLocalizeString(const FString& InTable, const FString& InKey)
{
	if (LocalizationPath.Contains(InTable))
	{
		return FText::FromStringTable(FName(LocalizationPath[InTable]), InKey);
	}

	return FText();
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
