#include "GameMode/GameStateComponent/GameTimelineComponent.h"

#include "Common/LuaEventDef.h"
#include "GameFramework/PlayerState.h"
#include "GameMode/TopDownStyleGameMode.h"
#include "GameMode/DataStruct/GameTimelineEntry.h"
#include "GameService/GameService.h"
#include "GameService/LuaEventService.h"
#include "Net/UnrealNetwork.h"
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
		// Initialize priority
		InitializeRandomPriorityList(PlayerControllerList.Num());
		// Generate timeline entry for each player
		for (auto PlayerController : PlayerControllerList)
		{
			if (PlayerController.IsValid())
			{
				if (auto PS = PlayerController->GetPlayerState<APlayerState>())
				{
					TSharedPtr<FGameTimelineEntry> NewEntry = MakeShareable(new FGameTimelineEntry(PS->GetPlayerId()));
					NewEntry->SetPriority(GetRandomizedPriority());
					TimelineEntryList.Add(NewEntry);
				}
			}
		}
	}

	// Sort the entries by priority
	TimelineEntryList.Sort(FCompareTimelineEntryByPriority());

	// Record the first player id of a round
	RoundStartPlayerId = TimelineEntryList[0]->GetPlayerId();

	UpdateTimeline();
}

void UGameTimelineComponent::InitializeRandomPriorityList(int32 InPlayerNum)
{
	RandomPriorityList.Empty();
	for (int32 i = InPlayerNum -1; i >= 0; --i)
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

void UGameTimelineComponent::UpdateTimeline()
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UGameTimelineComponent, CurrentTimeline, this);
	CurrentTimeline.Empty();
	for (auto& Entry : TimelineEntryList)
	{
		CurrentTimeline.Add(Entry->GetPlayerId());
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
	if (CurrentTimeline.Num() > 0)
	{
		return CurrentTimeline[0];
	}

	return -1;
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
	const auto CurPlayerId = GetFirstPlayerId();
	if (CurPlayerId >= 0)
	{
		if (CurPlayerId == RoundStartPlayerId)
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
	ensureAlwaysMsgf(InTime > 0, TEXT("Invalid time duration [%s]"), InTime);
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
		if (TimelineEntryList[i].IsValid() && TimelineEntryList[i]->GetPlayerId() == InId)
		{
			TimelineEntryList.RemoveAt(i);
			bRemoved = true;
			break;
		}
	}

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
