#include "GameMode/TopDownStyleGameState.h"

#include "GameFramework/PlayerState.h"
#include "GameMode/GameStateComponent/GameTimelineComponent.h"
#include "Net/UnrealNetwork.h"

ATopDownStyleGameState::ATopDownStyleGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create timeline component
	GameTimelineComponent = CreateDefaultSubobject<UGameTimelineComponent>("GameTimeline");

	WorldBounds = FBox(FVector(-2000, -2000, 0.f), FVector(2000, 2000, 0.f));
}

void ATopDownStyleGameState::BeginPlay()
{
	Super::BeginPlay();

	if (GameTimelineComponent)
	{
		GameTimelineComponent->OnGameTimelineChanged.AddDynamic(this, &ATopDownStyleGameState::OnTimelineChanged);
	}
}

void ATopDownStyleGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParam;
	SharedParam.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ATopDownStyleGameState, CurrentPlayerId, SharedParam);
}

void ATopDownStyleGameState::OnTimelineChanged()
{
	auto CurTimeline = GameTimelineComponent->GetTimeline();
	UE_LOG(LogTemp, Warning, TEXT("[TopDownGameState] Current time line: "));
	for (int32 i = 0; i < CurTimeline.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("[TopDownGameState] %d -> %d"), i, CurTimeline[i]);
	}
	UE_LOG(LogTemp, Warning, TEXT("[TopDownGameState] End of time line."));
}

void ATopDownStyleGameState::SetCurrentPlayerId(int32 InId)
{
	if (InId != CurrentPlayerId)
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(ATopDownStyleGameState, CurrentPlayerId, this);
		CurrentPlayerId = InId;

		if (HasAuthority())
		{
			OnRep_CurrentPlayerId();
		}
	}
}

void ATopDownStyleGameState::OnRep_CurrentPlayerId()
{
	
}

APlayerState* ATopDownStyleGameState::GetPlayerStateById(int32 InPlayerId) const
{
	APlayerState* Result = nullptr;
	for (auto PS : PlayerArray)
	{
		if (PS && PS->GetPlayerId() == InPlayerId)
		{
			Result = PS;
			break;
		}
	}

	return Result;
}
