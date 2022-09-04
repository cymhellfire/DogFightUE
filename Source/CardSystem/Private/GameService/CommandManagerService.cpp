#include "GameService/CommandManagerService.h"

#include "Card/CardCommand.h"
#include "Editor/CardCommandVisual.h"
#include "Editor/DataTable/CardCommandTableRow.h"

#define ST_CARD_COMMAND			"/Game/DogFight/Localization/ST_Card_Command.ST_Card_Command"

void UCommandManagerService::Startup()
{
	InitializeCommandList();
}

void UCommandManagerService::InitializeCommandList()
{
	// Iterate through all command classes
	// for (TObjectIterator<UClass> It; It; ++It)
	// {
	// 	if (It->IsChildOf(UCardCommand::StaticClass()) && !It->HasAnyClassFlags(CLASS_Abstract))
	// 	{
	// 		UCardCommand* CDO = Cast<UCardCommand>(It->ClassDefaultObject);
	// 		if (CDO)
	// 		{
	// 			UCardCommandVisual* NewVisual = NewObject<UCardCommandVisual>(this);
	// 			FText Name = FText::FromStringTable(ST_CARD_COMMAND, CDO->GetNameString());
	// 			FText Desc = FText::FromStringTable(ST_CARD_COMMAND, CDO->GetDescString());
	// 			// FText Name = FText::FromString(CDO->GetNameString());
	// 			// FText Desc = FText::FromString(CDO->GetDescString());
	//
	// 			NewVisual->Initialize(Name, Desc, CDO->GetCommandType());
	//
	// 			CommandVisualItemList.Add(NewVisual);
	// 		}
	// 	}
	// }

	// Read in all command data in table
	if (IsValid(CommandDataTable))
	{
		TArray<FCardCommandTableRow*> TableData;
		CommandDataTable->GetAllRows<FCardCommandTableRow>(TEXT(""), TableData);

		for (auto Data : TableData)
		{
			// Skip invalid data
			if (!IsValid(Data->CommandClass))
			{
				continue;
			}

			UCardCommand* CDO = Cast<UCardCommand>(Data->CommandClass->ClassDefaultObject);
			if (CDO)
			{
				UCardCommandVisual* NewVisual = NewObject<UCardCommandVisual>(this);
				FText Name = FText::FromStringTable(ST_CARD_COMMAND, CDO->GetNameString());
				FText Desc = FText::FromStringTable(ST_CARD_COMMAND, CDO->GetDescString());
				NewVisual->Initialize(Data->ID, Name, Desc, CDO->GetCommandType());

				CommandVisualItemMap.Add(Data->ID, NewVisual);
			}
		}
	}
}
