#include "GameMode/TopDownStyleGameState.h"

#include "GameMode/GameStateComponent/GameTimelineComponent.h"

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
