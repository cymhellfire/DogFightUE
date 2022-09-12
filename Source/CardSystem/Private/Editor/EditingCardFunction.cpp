#include "Editor/EditingCardFunction.h"

#include "Editor/EditingCardCommand.h"

FEditingCardFunction::FEditingCardFunction()
{
}

void FEditingCardFunction::SetName(FString InName)
{
	if (FunctionName == InName)
		return;

	FunctionName = InName;
	OnEditingFunctionNameChanged.Broadcast(FunctionName);
}

void FEditingCardFunction::SetID(int32 InId)
{
	ID = InId;
}

void FEditingCardFunction::AddCommand(int32 CmdId)
{
	TSharedPtr<FEditingCardCommand> NewCommand = MakeShareable(new FEditingCardCommand);
	NewCommand->SetCommandID(CmdId);

	int32 NewIndex = CommandList.Num();
	NewCommand->SetSelectIndex(NewIndex);
	CommandList.Add(NewCommand);

	// Trigger event
	OnFunctionCommandAdded.Broadcast(NewCommand);
}
