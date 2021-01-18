// Dog Fight Game Code By CYM.

#include "GameMessageWindowWidget.h"
#include "Common/Localization.h"

void UGameMessageWindowWidget::AddGameMessage(FGameMessage Message)
{
	FFormatOrderedArguments FormatArgumentValues;
	for (FText Argument : Message.Arguments)
	{
		FormatArgumentValues.Add(FFormatArgumentValue(Argument));
	}
	
	FText NewMessage = FText::Format(FText::FromStringTable(ST_INGAME_UI_LOC, Message.MessageString), FormatArgumentValues);

	OnGameMessageReceived(Message.Type, NewMessage, Message.Source);
}
