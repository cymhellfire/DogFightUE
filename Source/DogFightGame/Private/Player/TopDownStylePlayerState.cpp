#include "Player/TopDownStylePlayerState.h"

#include "Card/Card.h"
#include "CardSystem/Public/Card/CardDescObject.h"
#include "Common/DogFightGameLog.h"
#include "Common/LuaEventDef.h"
#include "Engine/ActorChannel.h"
#include "GameService/CardGameService.h"
#include "GameService/GameService.h"
#include "GameService/LuaEventService.h"
#include "Net/UnrealNetwork.h"
#include "Pawn/PlayerCharacter/TopDownStylePlayerCharacter.h"

void ATopDownStylePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ATopDownStylePlayerState, CardDescObjectList, SharedParams);
}

bool ATopDownStylePlayerState::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool bWriteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (auto DescObject : CardDescObjectList)
	{
		bWriteSomething |= Channel->ReplicateSubobject(DescObject, *Bunch, *RepFlags);
	}

	return bWriteSomething;
}

void ATopDownStylePlayerState::InitWithCharacter(ATopDownStylePlayerCharacter* InCharacter)
{
	if (!IsValid(InCharacter))
	{
		return;
	}

	// Register callback
	InCharacter->OnCharacterDead.AddDynamic(this, &ATopDownStylePlayerState::OnCharacterDead);

	// Record character
	ListeningCharacter = InCharacter;
}

void ATopDownStylePlayerState::ReleaseCharacter(ATopDownStylePlayerCharacter* InCharacter)
{
	if (!IsValid(InCharacter) || ListeningCharacter != InCharacter)
	{
		return;
	}

	// Unregister callback
	InCharacter->OnCharacterDead.RemoveDynamic(this, &ATopDownStylePlayerState::OnCharacterDead);

	// Clear the record
	ListeningCharacter.Reset();
}

bool ATopDownStylePlayerState::IsAlive() const
{
	return CurrentState == ETopDownStylePlayerState::PS_Alive;
}

void ATopDownStylePlayerState::AddCardObject(UCard* InCard)
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
}

void ATopDownStylePlayerState::RemoveCardObject(UCard* InCard)
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
}

UCardDescObject* ATopDownStylePlayerState::GetCardDescObjectByIndex(int32 Index)
{
	if (Index < 0 || Index >= CardDescObjectList.Num())
	{
		return nullptr;
	}

	return CardDescObjectList[Index];
}

void ATopDownStylePlayerState::AddCardDescObject(UCardDescObject* InDescObject)
{
	if (IsValid(InDescObject))
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(ATopDownStylePlayerState, CardDescObjectList, this);
		CardDescObjectList.AddUnique(InDescObject);

		if (HasAuthority())
		{
			OnRep_CardDescObjectList();
		}
	}
}

void ATopDownStylePlayerState::RemoveCardDescObject(UCardDescObject* InDescObject)
{
	if (IsValid(InDescObject))
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(ATopDownStylePlayerState, CardDescObjectList, this);
		CardDescObjectList.Remove(InDescObject);

		if (HasAuthority())
		{
			OnRep_CardDescObjectList();
		}
	}
}

void ATopDownStylePlayerState::OnRep_CardDescObjectList()
{
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_CardListChanged, GetPlayerId());
	}
}

UCard* ATopDownStylePlayerState::GetCardByInstanceId(int32 InId)
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

void ATopDownStylePlayerState::ServerTryToUseCardByInstanceId_Implementation(int32 InId)
{
	if (auto Card = GetCardByInstanceId(InId))
	{
		UE_LOG(LogTemp, Log, TEXT("[ATopDownStylePlayerState] Start use card with id: %d"), InId);
		// Notify server
		ServerBeginUseCard(InId);

		// Callback to notify client that card is started
		ClientBeginUseCard(InId);

		Card->OnCardExecutionFinished.AddDynamic(this, &ATopDownStylePlayerState::OnCardFinished);
		Card->Execute();
	}
}

void ATopDownStylePlayerState::OnCardFinished(ECardExecutionResult Result, UCard* Card)
{
	if (IsValid(Card))
	{
		Card->OnCardExecutionFinished.RemoveDynamic(this, &ATopDownStylePlayerState::OnCardFinished);
	}

	if (Result == ECardExecutionResult::CER_Default)
	{
		UE_LOG(LogDogFightGame, Log, TEXT("[ATopDownStylePlayerState] Finished use card."));
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
		UE_LOG(LogDogFightGame, Log, TEXT("[ATopDownStylePlayerState] Cancelled use card."));

		// Notify server
		ServerCancelCard(Card->GetInstanceId());

		// Notify client
		ClientCancelCard(Card->GetInstanceId());
	}
}

void ATopDownStylePlayerState::OnCharacterDead(ATopDownStylePlayerCharacter* Character)
{
	// Double check
	if (Character != ListeningCharacter)
	{
		return;
	}

	// Change state
	CurrentState = ETopDownStylePlayerState::PS_Dead;
}

void ATopDownStylePlayerState::ClientBeginUseCard_Implementation(int32 InId)
{
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_MyCardBeginUsing, InId);
	}
}

void ATopDownStylePlayerState::ServerBeginUseCard_Implementation(int32 InId)
{
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_PlayerCardBeginUsing, GetPlayerId(), InId);
	}
}

void ATopDownStylePlayerState::ClientCancelCard_Implementation(int32 InId)
{
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_MyCardCancelled, InId);
	}
}

void ATopDownStylePlayerState::ServerCancelCard(int32 InId)
{
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_PlayerCardCancelled, GetPlayerId(), InId);
	}
}

void ATopDownStylePlayerState::ClientCardFinished_Implementation(int32 InId)
{
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_MyCardFinished, InId);
	}
}

void ATopDownStylePlayerState::ServerCardFinished_Implementation(int32 InId)
{
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_PlayerCardFinished, GetPlayerId(), InId);
	}
}
