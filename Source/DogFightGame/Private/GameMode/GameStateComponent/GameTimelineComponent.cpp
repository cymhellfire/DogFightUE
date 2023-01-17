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

	UpdateTimeline();
}

void UGameTimelineComponent::InitializeRandomPriorityList(int32 InPlayerNum)
{
	RandomPriorityList.Empty();
	for (int32 i = 0; i < InPlayerNum; ++i)
	{
		RandomPriorityList.Add(i);
	}

	// Randomize the order with Fisher-Yates Shuffle algorithm
	// No need to check the number at 0 position
	for (int i = RandomPriorityList.Num() - 1; i > 0; --i)
	{
		const int32 Temp = RandomPriorityList[i];
		const int32 SwitchTargetIndex = FMath::RandRange(0, i);
		// Switch value
		RandomPriorityList[i] = RandomPriorityList[SwitchTargetIndex];
		RandomPriorityList[SwitchTargetIndex] = Temp;
	}
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

void UGameTimelineComponent::OnRep_CurrentTimeline()
{
	OnGameTimelineChanged.Broadcast();

	// Notify lua side
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_GameTimelineChanged);
	}
}
