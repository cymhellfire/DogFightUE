#include "GameService/CardGameService.h"

#include "Card/Card.h"
#include "Common/CardSystemConst.h"

void UCardGameService::UseCard(FString CardName, UObject* Instigator)
{
	FString FilePath = FPaths::Combine(CARD_SCRIPT_PATH, CardName);

	// Replace '/' with '.'
	FilePath.ReplaceCharInline('/', '.');

	if (!CardBlueprintClass.IsValid())
	{
		UBlueprint* BlueprintObject = LoadObject<UBlueprint>(NULL, CARD_BLUEPRINT_PATH);
		if (BlueprintObject)
		{
			CardBlueprintClass = BlueprintObject->GeneratedClass;
		}
	}

	if (CardBlueprintClass.IsValid() && !FilePath.IsEmpty())
	{
		UCard* NewCard = SpawnAndBindCard(CardBlueprintClass.Get(), FilePath, Instigator);

		if (NewCard)
		{
			NewCard->Execute();

			DestroyCard(NewCard);
		}
	}
}
