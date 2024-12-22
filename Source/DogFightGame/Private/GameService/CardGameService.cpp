#include "GameService/CardGameService.h"

#include "Common/DogFightGameLog.h"
#include "Common/LuaEventDef.h"
#include "Flow/CardUseFlow.h"
#include "GameService/LuaEventService.h"

UClass* UCardGameService::GetDefaultCardClass() const
{
	return UCard::StaticClass();
}

void UCardGameService::StartCardUseFlow(UCard* InCard)
{
	if (!IsValid(InCard))
	{
		DFLogE(LogDogFightGame, TEXT("Invalid card detected."));
		return;
	}

	if (IsValid(CurrentCardUseFlow))
	{
		DFLogE(LogDogFightGame, TEXT("Use card flow must be unique."));
		return;
	}

	DFLog(LogDogFightGame, TEXT("Use card flow created with card %d"), InCard->GetInstanceId());
	CurrentCardUseFlow = NewObject<UCardUseFlow>();
	CurrentCardUseFlow->SetInitialCard(InCard);
}

void UCardGameService::PushCardToUseFlow(UCard* InCard)
{
	if (!IsValid(InCard))
	{
		DFLogE(LogDogFightGame, TEXT("Invalid card detected."));
		return;
	}

	if (!IsValid(CurrentCardUseFlow))
	{
		DFLogE(LogDogFightGame, TEXT("Card use flow not set."));
		return;
	}

	DFLog(LogDogFightGame, TEXT("Pushing card %s to use flow."), *InCard->GetName());
	CurrentCardUseFlow->PushAgainstStep(InCard);
}

void UCardGameService::EndAndExecuteCardUseFlow()
{
	if (!IsValid(CurrentCardUseFlow))
	{
		DFLogE(LogDogFightGame, TEXT("Card use flow not set."));
		return;
	}

	DFLog(LogDogFightGame, TEXT("Card use flow started."));
	CurrentCardUseFlow->OnCardUseFlowComplete.BindUObject(this, &UCardGameService::OnCardUseCompleted);
	CurrentCardUseFlow->Execute();
}

void UCardGameService::ClearCardUseFlow()
{
	if (IsValid(CurrentCardUseFlow))
	{
		CurrentCardUseFlow->ConditionalBeginDestroy();
		CurrentCardUseFlow = nullptr;
	}
}

void UCardGameService::OnCardUseCompleted(UCardUseFlow* InFlow)
{
	if (InFlow != CurrentCardUseFlow)
	{
		DFLogE(LogDogFightGame, TEXT("Card use flow mismatch."));
		return;
	}

	DFLog(LogDogFightGame, TEXT("Card use flow completed."));
	InFlow->OnCardUseFlowComplete.Unbind();

	// Destroy card flow
	ClearCardUseFlow();

	// Send Event
	SEND_LUA_EVENT(ELuaEvent::LuaEvent_CardUseFlowFinished)
}
