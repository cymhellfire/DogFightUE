#include "Editor/EditingCardFunction.h"

#include "Editor/EditingCardCommand.h"

FEditingCardFunction::FEditingCardFunction()
{
}

void FEditingCardFunction::SetName(FString InName)
{
	FunctionName = InName;
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
