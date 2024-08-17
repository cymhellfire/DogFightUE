#include "GameMode/GameStateComponent/GameTimelineComponent.h"

#include "Common/CommonMagicNumber.h"
#include "Common/DogFightGameLog.h"
#include "Common/LuaEventDef.h"
#include "GameFramework/PlayerState.h"
#include "GameMode/TopDownStyleGameMode.h"
#include "GameMode/DataStruct/GameTimelineEntry.h"
#include "GameService/GameService.h"
#include "GameService/LuaEventService.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Pawn/PlayerCharacter/TopDownStylePlayerCharacter.h"
#include "PlayerController/TopDownStyleBotController.h"
#include "PlayerController/TopDownStylePlayerController.h"

struct FCompareTimelineEntryByPriority
{
	FORCEINLINE bool operator()( TSharedPtr<FGameTimelineEntry> A, TSharedPtr<FGameTimelineEntry> B ) const
	{
		return A->GetPriority() < B->GetPriority();
	}
};

UGameTimelineComponent::UGameTimelineComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, NextAvailableEntityId(0)
	, CurrentRound(0)
{
	SetIsReplicatedByDefault(true);
}

void UGameTimelineComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(UGameTimelineComponent, CurrentTimeline, SharedParams);
}

void UGameTimelineComponent::InitializeTimeline()
{
	if (auto TopDownGameMode = Cast<ATopDownStyleGameMode>(GetWorld()->GetAuthGameMode()))
	{
		auto PlayerControllerList = TopDownGameMode->GetAllPlayerControllers();
		auto BotControllerList = TopDownGameMode->GetAllBotControllers();
		// Initialize priority
		InitializeRandomPriorityList(PlayerControllerList.Num() + BotControllerList.Num());
		// Generate timeline entry for each player
		for (auto PlayerController : PlayerControllerList)
		{
			if (PlayerController.IsValid())
			{
				TSharedPtr<FGameTimelineEntry> NewEntity = MakeShareable(new FGameTimelineEntry(GetNextAvailableEntityId(), PlayerController->GetCharacterPawn()));
				NewEntity->SetPriority(GetRandomizedPriority());
				NewEntity->SetType(EGameTimelineEntityType::Player);
				TimelineEntryList.Add(NewEntity);
				TimelineEntityMap.Add(NewEntity->GetId(), NewEntity);
				// if (auto PS = PlayerController->GetPlayerState<APlayerState>())
				// {
				// 	TSharedPtr<FGameTimelineEntry> NewEntry = MakeShareable(new FGameTimelineEntry(PS->GetPlayerId()));
				// 	NewEntry->SetPriority(GetRandomizedPriority());
				// 	TimelineEntryList.Add(NewEntry);
				// }
			}
		}

		for (auto BotController : BotControllerList)
		{
			if (BotController.IsValid())
			{
				TSharedPtr<FGameTimelineEntry> NewEntity = MakeShareable(new FGameTimelineEntry(GetNextAvailableEntityId(), BotController->GetCharacterPawn()));
				NewEntity->SetPriority(GetRandomizedPriority());
				NewEntity->SetType(EGameTimelineEntityType::Bot);
				TimelineEntryList.Add(NewEntity);
				TimelineEntityMap.Add(NewEntity->GetId(), NewEntity);
			}
		}
	}

	// Sort the entries by priority
	TimelineEntryList.Sort(FCompareTimelineEntryByPriority());

	// Record the first entity id of a round
	RoundStartEntityId = TimelineEntryList[0]->GetId();

	UpdateTimeline();
}

void UGameTimelineComponent::InitializeRandomPriorityList(int32 InPlayerNum)
{
	RandomPriorityList.Empty();
	//for (int32 i = InPlayerNum -1; i >= 0; --i)
	for (int32 i = 0; i < InPlayerNum; ++i)
	{
		RandomPriorityList.Add(i);
	}

	// Randomize the order with Fisher-Yates Shuffle algorithm
	// No need to check the number at 0 position
	// for (int i = RandomPriorityList.Num() - 1; i > 0; --i)
	// {
	// 	const int32 Temp = RandomPriorityList[i];
	// 	const int32 SwitchTargetIndex = FMath::RandRange(0, i);
	// 	// Switch value
	// 	RandomPriorityList[i] = RandomPriorityList[SwitchTargetIndex];
	// 	RandomPriorityList[SwitchTargetIndex] = Temp;
	// }
}

int32 UGameTimelineComponent::GetRandomizedPriority()
{
	bool bHasPriorityLeft = RandomPriorityList.Num() > 0;
	ensureMsgf(bHasPriorityLeft, TEXT("[UGameTimelineComponent] No priority value left."));
	if (bHasPriorityLeft)
	{
		int32 Result = RandomPriorityList[0];
		RandomPriorityList.RemoveAt(0);
		return Result;
	}

	return 0;
}

int32 UGameTimelineComponent::GetNextAvailableEntityId()
{
	int32 Result = NextAvailableEntityId;

	// Loop the next value
	NextAvailableEntityId++;
	if (NextAvailableEntityId < 0)
	{
		NextAvailableEntityId = 0;
	}

	return Result;
}

void UGameTimelineComponent::UpdateTimeline()
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UGameTimelineComponent, CurrentTimeline, this);
	CurrentTimeline.Empty();
	for (auto& Entry : TimelineEntryList)
	{
		CurrentTimeline.Add(Entry->GetId());
	}

	if (GetOwnerRole() == ROLE_Authority)
	{
		OnRep_CurrentTimeline();
	}
}

TArray<int32> UGameTimelineComponent::GetTimeline() const
{
	return CurrentTimeline;
}

int32 UGameTimelineComponent::GetFirstPlayerId() const
{
	if (auto CurrentEntity = GetCurrentTimelineEntity())
	{
		return CurrentEntity->GetOwnerPlayerId();
	}
	// if (CurrentTimeline.Num() > 0)
	// {
	// 	return CurrentTimeline[0];
	// }

	return GameFlowMagicNumbers::InvalidPlayerId;
}

int32 UGameTimelineComponent::GetCurrentEntityId() const
{
	if (CurrentTimeline.Num() > 0)
	{
		return CurrentTimeline[0];
	}

	DFLogW(LogDogFightGame, TEXT("No entity in timeline."));
	return GameFlowMagicNumbers::InvalidTimelineEntityId;
}

FGameTimelineEntry const* UGameTimelineComponent::GetCurrentTimelineEntity() const
{
	auto CurrentId = GetCurrentEntityId();
	if (CurrentId != -1)
	{
		if (auto Result = TimelineEntityMap.Find(CurrentId))
		{
			TSharedPtr<FGameTimelineEntry> EntityPtr = Result->Pin();
			if (EntityPtr.IsValid())
			{
				return EntityPtr.Get();
			}
		}
	}
	return nullptr;
}

void UGameTimelineComponent::MoveForward()
{
	// Move the first entry to the tail
	auto TempEntry = TimelineEntryList[0];
	TimelineEntryList.RemoveAt(0);
	TimelineEntryList.Add(TempEntry);

	UpdateTimeline();
}

void UGameTimelineComponent::OnRep_CurrentTimeline()
{
	OnGameTimelineChanged.Broadcast();

	// Check and notify new round
	const auto CurEntityId = GetCurrentEntityId();
	if (CurEntityId >= 0)
	{
		if (CurEntityId == RoundStartEntityId)
		{
			CurrentRound ++;
			OnNewRoundStarted.Broadcast();
		}
	}

	// Notify lua side
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_GameTimelineChanged);
	}
}

TArray<int32> UGameTimelineComponent::GetTimerWaitingQueue(int32 InTime)
{
	ensureAlwaysMsgf(InTime > 0, TEXT("Invalid time duration [%d]"), InTime);
	TArray<int32> Result;

	// Just use current timeline and repeat n times
	for (int32 i = 0; i < InTime; ++i)
	{
		Result.Append(CurrentTimeline);
	}

	return Result;
}

void UGameTimelineComponent::RemoveEntryByPlayerId(int32 InId)
{
	bool bRemoved = false;
	// Iterate through the entry list and remove matched one
	for (int32 i = 0; i < TimelineEntryList.Num(); ++i)
	{
		if (TimelineEntryList[i].IsValid() && TimelineEntryList[i]->GetId() == InId)
		{
			TimelineEntryList.RemoveAt(i);
			bRemoved = true;
			break;
		}
	}
	TimelineEntityMap.Remove(InId);

	// Trigger the notify
	if (bRemoved)
	{
		MulticastTimelineEntryRemoved(InId);
	}

	UpdateTimeline();
}

void UGameTimelineComponent::MulticastTimelineEntryRemoved_Implementation(int32 InId)
{
	OnTimelineEntryRemoved.Broadcast(InId);
}

void UGameTimelineComponent::AddTimer(int32 InTime, FTimelineRoundTimerFinishDelegate& InCallback)
{
	TSharedPtr<FTimelineRoundTimer> NewTimer = MakeShareable(new FTimelineRoundTimer());
	NewTimer->InitTimer(this, InTime, InCallback);
	NewTimer->OnTimerExpired.AddUObject(this, &UGameTimelineComponent::OnRoundTimerExpired);
	TimerList.Add(NewTimer);
}

void UGameTimelineComponent::OnRoundTimerExpired(TSharedPtr<FTimelineRoundTimer> InTimer)
{
	if (!InTimer.IsValid())
	{
		return;
	}

	InTimer->OnTimerExpired.RemoveAll(this);
	TimerList.Remove(InTimer);
}
