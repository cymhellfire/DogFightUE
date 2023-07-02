// Dog Fight Game Code By CYM.


#include "Subsystem/PopupMessageSubsystem.h"

void UPopupMessageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnPostLoadMapWithWorld);
}

void UPopupMessageSubsystem::Deinitialize()
{
	Super::Deinitialize();

	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

void UPopupMessageSubsystem::AddPendingMessage(const FString& LevelName, const FGenericMessage& InMessage)
{
	UE_LOG(LogTemp, Log, TEXT("UPopupMessageSubsystem::AddPendingMessage %s Message: %s"), *LevelName, *InMessage.Content.ToString());

	// Enqueue new message
	auto& MessageQueue = PendingMessageMap.FindOrAdd(LevelName);
	MessageQueue.Add(InMessage);
}

void UPopupMessageSubsystem::OnPostLoadMapWithWorld(UWorld* World)
{
	auto LevelName = World->GetName();

	// Find pending messages
	if (auto MessageQueuePtr = PendingMessageMap.Find(LevelName))
	{
		auto MessageQueue = *MessageQueuePtr;
		for (auto& Message : MessageQueue)
		{
			HandleMessagePopup(Message);
		}

		MessageQueuePtr->Empty();
	}
}
