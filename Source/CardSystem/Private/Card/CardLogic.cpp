#include "Card/CardLogic.h"
#include "Card/Card.h"

UCardLogic::UCardLogic()
{
	bFinished = false;
}

void UCardLogic::Tick(float DeltaTime)
{
	TickLogic(DeltaTime);
}

void UCardLogic::InitLogic(UCard* InCard)
{
	OwnerCard = InCard;
}

void UCardLogic::StartLogic(const FString& ScriptPath)
{
	OnLogicStarted(ScriptPath);
}

void UCardLogic::TickLogic(float DeltaTime)
{
	if (bFinished)
		return;

	TickLogicScript(DeltaTime);
}

void UCardLogic::MarkLogicFinished(ECardLogicFinishType::Type Type)
{
	bFinished = true;
	OnCardLogicFinished.Broadcast(Type);
}

UCard* UCardLogic::GetOwnerCard() const
{
	return OwnerCard.IsValid() ? OwnerCard.Get() : nullptr;
}
