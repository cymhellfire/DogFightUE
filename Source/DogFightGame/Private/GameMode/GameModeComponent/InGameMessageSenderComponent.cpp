#include "GameMode/GameModeComponent/InGameMessageSenderComponent.h"
#include "GameMode/TopDownStyleGameMode.h"
#include "PlayerController/TopDownStylePlayerController.h"
#include "PlayerController/PlayerControllerComponent/InGameMessageReceiverComponent.h"

UInGameMessageSenderComponent::UInGameMessageSenderComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;
}

void UInGameMessageSenderComponent::InitializeComponent()
{
	Super::InitializeComponent();

	ParentGameMode = GetOwner<ATopDownStyleGameMode>();
}

void UInGameMessageSenderComponent::BroadcastTitleMessage(const FText& Message)
{
	if (ParentGameMode.IsValid())
	{
		auto AllPC = ParentGameMode->GetAllPlayerControllers();
		for (auto PCPtr : AllPC)
		{
			if (PCPtr.IsValid())
			{
				if (auto Receiver = PCPtr->GetInGameMessageReceiverComponent())
				{
					Receiver->ReceiveMessage(Message);
				}
			}
		}
	}
}

void UInGameMessageSenderComponent::BroadcastInGameChatMessage(const FInGameChatMessage& ChatMessage)
{
	if (ParentGameMode.IsValid())
	{
		auto AllPC = ParentGameMode->GetAllPlayerControllers();
		for (auto PCPtr : AllPC)
		{
			if (PCPtr.IsValid())
			{
				if (auto Receiver = PCPtr->GetInGameMessageReceiverComponent())
				{
					Receiver->ReceiveInGameChatMessage(ChatMessage);
				}
			}
		}
	}
}
