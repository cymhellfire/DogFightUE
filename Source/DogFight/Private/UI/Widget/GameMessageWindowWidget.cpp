// Dog Fight Game Code By CYM.

#include "GameMessageWindowWidget.h"

#define ST_INGAME_UI_LOC		"/Game/DogFight/Localization/ST_InGameUI.ST_InGameUI"

void UGameMessageWindowWidget::AddGameMessage(const FString& MessageContent, const TArray<FString>& ArgumentList)
{
	FFormatOrderedArguments FormatArgumentValues;
	for (FString Argument : ArgumentList)
	{
		FormatArgumentValues.Add(FFormatArgumentValue(FText::FromString(Argument)));
	}
	
	FText NewMessage = FText::Format(FText::FromStringTable(ST_INGAME_UI_LOC, MessageContent), FormatArgumentValues);

	OnGameMessageReceived(NewMessage);
}
