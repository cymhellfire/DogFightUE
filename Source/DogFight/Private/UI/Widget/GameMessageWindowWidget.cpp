// Dog Fight Game Code By CYM.

#include "GameMessageWindowWidget.h"

#define ST_INGAME_UI_LOC		"/Game/DogFight/Localization/ST_InGameUI.ST_InGameUI"

void UGameMessageWindowWidget::AddGameMessage(FGameMessage Message)
{
	FFormatOrderedArguments FormatArgumentValues;
	for (FString Argument : Message.Arguments)
	{
		FormatArgumentValues.Add(FFormatArgumentValue(FText::FromString(Argument)));
	}
	
	FText NewMessage = FText::Format(FText::FromStringTable(ST_INGAME_UI_LOC, Message.MessageString), FormatArgumentValues);

	OnGameMessageReceived(Message.Type, NewMessage, Message.Source);
}
