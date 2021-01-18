// Dog Fight Game Code By CYM.


#include "OperationHintMessageWidget.h"
#include "Common/Localization.h"

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
