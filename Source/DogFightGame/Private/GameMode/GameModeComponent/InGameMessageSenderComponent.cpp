#include "GameMode/GameModeComponent/InGameMessageSenderComponent.h"
#include "GameMode/FreeForAllGameMode.h"
#include "PlayerController/FreeForAllPlayerController.h"
#include "PlayerController/PlayerControllerComponent/InGameMessageReceiverComponent.h"

UInGameMessageSenderComponent::UInGameMessageSenderComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;
}

void UInGameMessageSenderComponent::InitializeComponent()
{
	Super::InitializeComponent();

	ParentGameMode = GetOwner<AFreeForAllGameMode>();
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
