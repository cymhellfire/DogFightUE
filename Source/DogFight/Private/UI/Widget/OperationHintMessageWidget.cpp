// Dog Fight Game Code By CYM.


#include "OperationHintMessageWidget.h"
#include "Common/Localization.h"

namespace EOperationHintMessageContent
{
	const FName SelectActorTarget = FName(TEXT("OpHintMsg_SelectActorTarget"));
	const FName SelectPositionTarget = FName(TEXT("OpHintMsg_SelectPositionTarget"));
	const FName SelectDirectionTarget = FName(TEXT("OpHintMsg_SelectDirectionTarget"));
	const FName DiscardCard = FName(TEXT("OpHintMsg_DiscardCard"));
	const FName ResponseToCard = FName(TEXT("OpHintMsg_ResponseToCard"));
}

void UOperationHintMessageWidget::SetHintMessage(FName& NewMessage, TArray<FText> Arguments)
{
	if (NewMessage == CurrentHintMessage)
		return;

	CurrentHintMessage = NewMessage;

	const FFormatOrderedArguments OrderedArguments(Arguments);

	// Localize message and display
	const FText LocalizedMessage = FText::Format(FText::FromStringTable(ST_INGAME_UI_LOC, NewMessage.ToString()), OrderedArguments);
	OnHintMessageChanged(LocalizedMessage);
}
