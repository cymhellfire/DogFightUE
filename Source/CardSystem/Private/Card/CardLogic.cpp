#include "Card/CardLogic.h"
#include "Card/Card.h"

UCardLogic::UCardLogic()
{
	bFinished = false;
	CurState = ECardLogicStateType::ECLST_None;
}

void UCardLogic::Tick(float DeltaTime)
{
	TickLogic(DeltaTime);
}

void UCardLogic::InitLogic(UCard* InCard, int32 LogicId)
{
	OwnerCard = InCard;
	LoadAndInitLogicScript(LogicId);
}

void UCardLogic::StartTargetSelect()
{
	CurState = ECardLogicStateType::ECLST_AcquireTarget;

	OnSelectTargetStarted();
}

void UCardLogic::MarkTargetAcquired(ECardTargetAcquireType::Type Type)
{
	OnCardTargetAcquired.Broadcast(Type);
}

void UCardLogic::StartLogic()
{
	CurState = ECardLogicStateType::ECLST_Logic;

	OnLogicStarted();
}

void UCardLogic::TickLogic(float DeltaTime)
{
	if (bFinished)
		return;

	switch (CurState)
	{
	case ECardLogicStateType::ECLST_AcquireTarget:
		TickSelectTargetScript(DeltaTime);
		break;
	case ECardLogicStateType::ECLST_Logic:
		TickLogicScript(DeltaTime);
		break;
	case ECardLogicStateType::ECLST_None:
	default:
		break;
	}
}

void UCardLogic::MarkLogicFinished(ECardLogicFinishType::Type Type)
{
	bFinished = true;
	CurState = ECardLogicStateType::ECLST_None;
	OnCardLogicFinished.Broadcast(Type);
}

UCard* UCardLogic::GetOwnerCard() const
{
	return OwnerCard.IsValid() ? OwnerCard.Get() : nullptr;
}
