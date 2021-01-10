// Dog Fight Game Code By CYM.


#include "OperationHintMessageWidget.h"

#define ST_INGAME_UI_LOC		"/Game/DogFight/Localization/ST_InGameUI.ST_InGameUI"

namespace EOperationHintMessageContent
{
	const FName SelectActorTarget = FName(TEXT("OpHintMsg_SelectActorTarget"));
	const FName SelectPositionTarget = FName(TEXT("OpHintMsg_SelectPositionTarget"));
	const FName SelectDirectionTarget = FName(TEXT("OpHintMsg_SelectDirectionTarget"));
	const FName DiscardCard = FName(TEXT("OpHintMsg_DiscardCard"));
}

void UOperationHintMessageWidget::SetHintMessage(FName& NewMessage)
{
	if (NewMessage == CurrentHintMessage)
		return;

	CurrentHintMessage = NewMessage;

	// Localize message and display
	const FText LocalizedMessage = FText::FromStringTable(ST_INGAME_UI_LOC, NewMessage.ToString());
	OnHintMessageChanged(LocalizedMessage);
}
