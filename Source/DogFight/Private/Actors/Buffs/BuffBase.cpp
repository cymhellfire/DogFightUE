// Dog Fight Game Code By CYM.


#include "BuffBase.h"
#include "StandardGameState.h"
#include "StandardGameMode.h"
#include "GameRoundsTimeline.h"
#include "VfxBase.h"
#include "StandardModePlayerCharacter.h"

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

void ABuffBase::SetTargetActor(AActor* Target)
{
	TargetActor = Target;
	
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(Target))
	{
		StandardModePlayerCharacter->OnCharacterDead.AddDynamic(this, &ABuffBase::OnTargetActorDead);
	}

	// Apply buff if not yet (Ensure the target is non-null when apply buff)
	if (!bAppliedToTarget)
	{
		ApplyBuff();
		bAppliedToTarget = true;
	}
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

	// Apply buff if target is set
	if (TargetActor != nullptr)
	{
		ApplyBuff();
		bAppliedToTarget = true;
	}
}

void ABuffBase::BuffEnd()
{
	// Remove buff
	RemoveBuff();

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

void ABuffBase::ApplyBuff()
{
	// Spawn Vfx if specified
	if (IsValid(VfxClass))
	{
		VfxActor = GetWorld()->SpawnActor<AVfxBase>(VfxClass);
		// Set the vfx position, attach to socket if specified or just move to target actor location
		if (!VfxSocketName.IsNone())
		{
			VfxActor->AttachToActor(TargetActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale, VfxSocketName);
		}
		else
		{
			VfxActor->SetActorLocation(TargetActor->GetActorLocation());
		}
	}
}

void ABuffBase::RemoveBuff()
{
	// Destroy the vfx actor
	if (VfxActor != nullptr)
	{
		VfxActor->Destroy();
	}
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

void ABuffBase::OnTargetActorDead()
{
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(TargetActor))
	{
		StandardModePlayerCharacter->OnCharacterDead.RemoveDynamic(this, &ABuffBase::OnTargetActorDead);
	}

	BuffEnd();
}
