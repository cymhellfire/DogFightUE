#include "Player/TopDownStylePlayerState.h"

#include "Card/Card.h"
#include "CardSystem/Public/Card/CardDescObject.h"
#include "Common/LuaEventDef.h"
#include "GameService/CardGameService.h"
#include "GameService/GameService.h"
#include "GameService/LuaEventService.h"
#include "Net/UnrealNetwork.h"

void ATopDownStylePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ATopDownStylePlayerState, CardDescObjectList, SharedParams);
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
		LuaEventService->SendEventToLua_OneParam_Int(ELuaEvent::Type::LuaEvent_CardListChanged, GetPlayerId());
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
		UE_LOG(LogTemp, Log, TEXT("[ATopDownStylePlayerState] Finished use card."));
		// Remove card from list
		RemoveCardObject(Card);

		// Destroy the card instance
		if (auto CardGameService = UGameService::GetGameService<UCardGameService>())
		{
			CardGameService->DestroyCard(Card);
		}
	}
}
