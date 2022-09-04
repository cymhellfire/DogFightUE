#include "Editor/CardCommandVisual.h"

void UCardCommandVisual::Initialize(const int32 InId, const FText& InName, const FText& InDesc, const ECardCommandType Type)
{
	ID = InId;
	CommandName = InName;
	CommandDesc = InDesc;
	CommandType = Type;
}
