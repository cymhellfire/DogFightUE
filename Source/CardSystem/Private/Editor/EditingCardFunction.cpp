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

TWeakPtr<FEditingCardCommand> FEditingCardFunction::AddCommand()
{
	TSharedPtr<FEditingCardCommand> NewCommand = MakeShareable(new FEditingCardCommand);

	return NewCommand;
}
