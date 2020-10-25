// Dog Fight Game Code By CYM.


#include "GameTitleMessageWidget.h"

#define ST_INGAME_UI_LOC		"/Game/DogFight/Localization/ST_InGameUI.ST_InGameUI"

void UGameTitleMessageWidget::DisplayTitleMessage(FGameTitleMessage TitleMessage)
{
	FFormatOrderedArguments FormatArgumentValues;
	for (FString Argument : TitleMessage.Arguments)
	{
		FormatArgumentValues.Add(FFormatArgumentValue(FText::FromString(Argument)));
	}

	const FText NewMessage = FText::Format(FText::FromStringTable(ST_INGAME_UI_LOC, TitleMessage.MessageString), FormatArgumentValues);

	OnTitleMessageReceived(NewMessage);
}
