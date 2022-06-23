#include "Card/CardCommandLibrary.h"

#include "CardSystem.h"
#include "Card/CardCommand.h"
#include "GameService/CardGameService.h"


template <typename T>
void UCardCommandLibrary::CreateCommand(UCard* Owner, void* CreateParams)
{
	// Card command must have a valid owner
	if (!IsValid(Owner))
	{
		UE_LOG(LogCardSystem, Error, TEXT("[CardCommandLibrary] Invalid owner card specified for new command %s."), *T::StaticClass()->GetName());
		return;
	}

	UCardCommand* NewCommand = NewObject<UCardCommand>(Owner, T::StaticClass());
	// Initialize command with params
	if (!NewCommand->Initialize(CreateParams))
	{
		UE_LOG(LogCardSystem, Error, TEXT("[CardCommandLibrary] Failed to initialize new command %s."), *T::StaticClass()->GetName());
		NewCommand->ConditionalBeginDestroy();
		return;
	}

	Owner->QueueCommand(NewCommand);
}

