#include "Editor/EditingCardCommand.h"

FEditingCardCommand::FEditingCardCommand()
{
}

void FEditingCardCommand::SetCommandID(int32 InId)
{
	if (InId == CommandID)
	{
		return;
	}

	CommandID = InId;
	OnCommandIdChanged.Broadcast(InId);
}
