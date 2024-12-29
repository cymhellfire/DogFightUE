// Dog Fight Game Code By CYM.


#include "Pawn/PlayerCharacter/CharacterInventoryComponent.h"

#include "Card/Card.h"
#include "Card/CardDescObject.h"
#include "Common/DogFightGameLog.h"
#include "Common/LuaEventDef.h"
#include "Engine/ActorChannel.h"
#include "GameService/CardGameService.h"
#include "GameService/GameService.h"
#include "GameService/LuaEventService.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Pawn/PlayerCharacter/TopDownStylePlayerCharacter.h"


// Sets default values for this component's properties
UCharacterInventoryComponent::UCharacterInventoryComponent()
{
	SetIsReplicatedByDefault(true);
}

void UCharacterInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(UCharacterInventoryComponent, CardDescObjectList, SharedParams);
}

bool UCharacterInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool bWriteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (auto DescObject : CardDescObjectList)
	{
		bWriteSomething |= Channel->ReplicateSubobject(DescObject, *Bunch, *RepFlags);

		bWriteSomething |= DescObject->ReplicateModifierDescObjects(Channel, Bunch, RepFlags);
	}

	return bWriteSomething;
}

// Called when the game starts
void UCharacterInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UCharacterInventoryComponent::OnRep_CardDescObjectList()
{
	for (auto DescObject : CardDescObjectList)
	{
		DFLog(LogDogFightGame, TEXT("Card %s synced"), *DescObject->GetName());
	}

	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_CardListChanged, GetPlayerId());
	}
}

void UCharacterInventoryComponent::AddCardObject(UCard* InCard)
{
	if (IsValid(InCard))
	{
		if (CardObjectList.Contains(InCard))
		{
			return;
		}

		CardObjectList.Add(InCard);
		// Set owner player id
		InCard->SetOwnerPlayerId(GetPlayerId());
		// Add desc object as well
		AddCardDescObject(InCard->GetDescObject());
	}
	else
	{
		DFLogE(LogDogFightGame, TEXT("Invalid card to add."))
	}
}

void UCharacterInventoryComponent::RemoveCardObject(UCard* InCard)
{
	if (IsValid(InCard))
	{
		if (!CardObjectList.Contains(InCard))
		{
			return;
		}

		// Remove desc object
		RemoveCardDescObject(InCard->GetDescObject());
		CardObjectList.Remove(InCard);
	}
	else
	{
		DFLogE(LogDogFightGame, TEXT("Invalid card to remove."))
	}
}

void UCharacterInventoryComponent::AddCardDescObject(UCardDescObject* InDescObject)
{
	if (IsValid(InDescObject))
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(UCharacterInventoryComponent, CardDescObjectList, this);
		CardDescObjectList.AddUnique(InDescObject);

		// Use owner actor to replicate
		InDescObject->SetReplicator(GetOwner());

		if (GetOwnerRole() == ROLE_Authority)
		{
			OnRep_CardDescObjectList();
		}
	}
}

void UCharacterInventoryComponent::RemoveCardDescObject(UCardDescObject* InDescObject)
{
	if (IsValid(InDescObject))
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(UCharacterInventoryComponent, CardDescObjectList, this);
		CardDescObjectList.Remove(InDescObject);

		if (GetOwnerRole() == ROLE_Authority)
		{
			OnRep_CardDescObjectList();
		}
	}
}

int32 UCharacterInventoryComponent::GetPlayerId() const
{
	if (ATopDownStylePlayerCharacter* Character = Cast<ATopDownStylePlayerCharacter>(GetOwner()))
	{
		return Character->GetOwnerPlayerId();
	}
	return -1;
}

int32 UCharacterInventoryComponent::GetTimelineEntityId() const
{
	if (ATopDownStylePlayerCharacter* Character = Cast<ATopDownStylePlayerCharacter>(GetOwner()))
	{
		return Character->GetEntityId();
	}
	return -1;
}

UCardDescObject* UCharacterInventoryComponent::GetCardDescObjectByIndex(int32 Index)
{
	if (Index < 0 || Index >= CardDescObjectList.Num())
	{
		return nullptr;
	}

	return CardDescObjectList[Index];
}

UCard* UCharacterInventoryComponent::GetCardByInstanceId(int32 InId)
{
	for (auto Card : CardObjectList)
	{
		if (Card->GetInstanceId() == InId)
		{
			return Card;
		}
	}

	return nullptr;
}

void UCharacterInventoryComponent::ServerTryToUseCardByInstanceId_Implementation(int32 InId)
{
	if (auto Card = GetCardByInstanceId(InId))
	{
		DFLog(LogDogFightGame, TEXT("Start use card with id: %d"), InId);
		// Notify server
		// ServerBeginUseCard(InId);
		ServerBeginAcquireTarget(InId);

		// Callback to notify client that card is started
		// ClientBeginUseCard(InId);
		ClientBeginAcquireTarget(InId);

		Card->OnCardAcquiredTarget.AddDynamic(this, &UCharacterInventoryComponent::OnCardAcquiredTarget);
		Card->OnCardExecutionFinished.AddDynamic(this, &UCharacterInventoryComponent::OnCardFinished);
		// Card->Execute();

		Card->SelectTarget();
	}
}

void UCharacterInventoryComponent::OnCardAcquiredTarget(ECardExecutionResult Result, UCard* Card)
{
	if (IsValid(Card))
	{
		Card->OnCardAcquiredTarget.RemoveDynamic(this, &UCharacterInventoryComponent::OnCardAcquiredTarget);
	}

	if (Result == ECardExecutionResult::CER_Default)
	{
		DFLog(LogDogFightGame, TEXT("OnCardAcquiredTarget called"));

		// Notify server
		ServerAcquiredTarget(Card->GetInstanceId());

		// Notify client
		ClientAcquiredTarget(Card->GetInstanceId());
	}
	else if (Result == ECardExecutionResult::CER_Cancelled)
	{
		DFLog(LogDogFightGame, TEXT("OnCardAcquiredTarget Cancelled"));

		// Notify server
		ServerCancelCard(Card->GetInstanceId());

		// Notify client
		ClientCancelCard(Card->GetInstanceId());
	}
}

void UCharacterInventoryComponent::OnCardFinished(ECardExecutionResult Result, UCard* Card)
{
	if (IsValid(Card))
	{
		Card->OnCardExecutionFinished.RemoveDynamic(this, &UCharacterInventoryComponent::OnCardFinished);
	}

	if (Result == ECardExecutionResult::CER_Default)
	{
		DFLog(LogDogFightGame, TEXT("Finished use card."));
		// Remove card from list first
		RemoveCardObject(Card);

		// Notify server
		ServerCardFinished(Card->GetInstanceId());

		// Notify client
		ClientCardFinished(Card->GetInstanceId());

		// Destroy the card instance
		if (auto CardGameService = UGameService::GetGameService<UCardGameService>())
		{
			CardGameService->DestroyCard(Card);
		}
	}
	else if (Result == ECardExecutionResult::CER_Cancelled)
	{
		DFLog(LogDogFightGame, TEXT("Cancelled use card."));

		// Notify server
		ServerCancelCard(Card->GetInstanceId());

		// Notify client
		ClientCancelCard(Card->GetInstanceId());
	}
}

void UCharacterInventoryComponent::ClientBeginAcquireTarget_Implementation(int32 InId)
{
	SEND_LUA_EVENT(ELuaEvent::LuaEvent_MyCardStartAcquireTarget, InId);
}

void UCharacterInventoryComponent::ServerBeginAcquireTarget_Implementation(int32 InId)
{
	SEND_LUA_EVENT(ELuaEvent::LuaEvent_PlayerCardStartAcquireTarget, GetTimelineEntityId(), InId);
}

void UCharacterInventoryComponent::ClientAcquiredTarget_Implementation(int32 InId)
{
	SEND_LUA_EVENT(ELuaEvent::LuaEvent_MyCardAcquiredTarget, InId);
}

void UCharacterInventoryComponent::ServerAcquiredTarget_Implementation(int32 InId)
{
	SEND_LUA_EVENT(ELuaEvent::LuaEvent_PlayerCardAcquiredTarget, GetTimelineEntityId(), InId);
}

void UCharacterInventoryComponent::ClientBeginUseCard_Implementation(int32 InId)
{
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_MyCardBeginUsing, InId);
	}
}

void UCharacterInventoryComponent::ServerBeginUseCard_Implementation(int32 InId)
{
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_PlayerCardBeginUsing, GetTimelineEntityId(), InId);
	}
}

void UCharacterInventoryComponent::ClientCancelCard_Implementation(int32 InId)
{
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_MyCardCancelled, InId);
	}
}

void UCharacterInventoryComponent::ServerCancelCard(int32 InId)
{
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_PlayerCardCancelled, GetTimelineEntityId(), InId);
	}
}

void UCharacterInventoryComponent::ClientCardFinished_Implementation(int32 InId)
{
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_MyCardFinished, InId);
	}
}

void UCharacterInventoryComponent::ServerCardFinished_Implementation(int32 InId)
{
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_PlayerCardFinished, GetTimelineEntityId(), InId);
	}
}
