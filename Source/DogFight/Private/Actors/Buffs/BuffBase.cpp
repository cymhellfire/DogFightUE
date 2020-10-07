// Dog Fight Game Code By CYM.


#include "BuffBase.h"
#include "StandardGameState.h"
#include "StandardGameMode.h"
#include "GameRoundsTimeline.h"

// Sets default values
ABuffBase::ABuffBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Lifetime = 1;
}

void ABuffBase::SetLifetime(float NewLifetime)
{
	
}

void ABuffBase::SetSourcePlayerController(APlayerController* PlayerController)
{
	SourcePlayerController = PlayerController;
}

// Called when the game starts or when spawned
void ABuffBase::BeginPlay()
{
	Super::BeginPlay();

	// Get the lifetime
	AStandardGameState* StandardGameState = GetWorld()->GetGameState<AStandardGameState>();
	if (StandardGameState != nullptr)
	{
		LifetimeQueue = StandardGameState->GetGameRoundsTimeline()->GetLifetime(Lifetime);
	}

	// Register delegate
	AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode());
	if (StandardGameMode != nullptr)
	{
		StandardGameMode->OnPlayerRoundEnd.AddDynamic(this, &ABuffBase::OnPlayerRoundEnd);
		StandardGameMode->OnPlayerDead.AddDynamic(this, &ABuffBase::OnPlayerDead);
	}
}

void ABuffBase::BuffEnd()
{
	// Unregister delegate
	AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode());
	if (StandardGameMode != nullptr)
	{
		StandardGameMode->OnPlayerRoundEnd.RemoveDynamic(this, &ABuffBase::OnPlayerRoundEnd);
		StandardGameMode->OnPlayerDead.RemoveDynamic(this, &ABuffBase::OnPlayerDead);
	}

	// Destroy this actor
	Destroy();
}

void ABuffBase::OnPlayerRoundEnd(int32 PlayerId)
{
	// Shorten lifetime
	LifetimeQueue.RemoveSingle(PlayerId);

	if (LifetimeQueue.Num() == 0)
	{
		BuffEnd();
	}
}

void ABuffBase::OnPlayerDead(int32 PlayerId)
{
	// Remove the dead player from timeline
	LifetimeQueue.RemoveAll([PlayerId] (const int32 Item){return Item == PlayerId;});
}
