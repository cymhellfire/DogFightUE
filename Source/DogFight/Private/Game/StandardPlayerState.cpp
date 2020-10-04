// Dog Fight Game Code By CYM.


#include "StandardPlayerState.h"
#include "CardBase.h"

AStandardPlayerState::AStandardPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CardGainPerRounds = 2;
}

void AStandardPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStandardPlayerState, CardInfoList);
}

void AStandardPlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();

	// Trigger the delegate
	OnPlayerNameChanged.Broadcast(GetPlayerName());
}

void AStandardPlayerState::AddCard(ACardBase* Card)
{
	if (GetNetMode() == NM_Client)
		return;

	CardInfoList.Add(Card->GetCardDisplayInfo());
	CardInstanceList.Add(Card);

	// Invoke OnRep function on server side
	OnRep_CardInfoList();
}

void AStandardPlayerState::CmdUseCardByIndex_Implementation(int32 Index)
{
	if (Index >= CardInstanceList.Num())
	{
		UE_LOG(LogDogFight, Error, TEXT("Cannot find card instance with index %d"), Index);
		return;
	}

	// Begin using specified card
	ACardBase* UsingCard = CardInstanceList[Index];
	UsingCard->Use();
	UsingCardIndex = Index;

	// Register to card's delegate
	UsingCard->OnCardFinished.AddDynamic(this, &AStandardPlayerState::OnCardFinished);
}

void AStandardPlayerState::OnCardFinished()
{
	ACardBase* UsingCard = CardInstanceList[UsingCardIndex];
	UsingCard->OnCardFinished.RemoveDynamic(this, &AStandardPlayerState::OnCardFinished);
	CardInstanceList.RemoveAt(UsingCardIndex);
	CardInfoList.RemoveAt(UsingCardIndex);

	UsingCardIndex = -1;

	// Invoke OnRep on server side
	OnRep_CardInfoList();
}

int32 AStandardPlayerState::GetCardGainNumByRound()
{
	return CardGainPerRounds;
}

void AStandardPlayerState::OnRep_CardInfoList()
{
	// Broadcast event
	OnPlayerCardInfoListChanged.Broadcast();
}
