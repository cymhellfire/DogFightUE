// Dog Fight Game Code By CYM.


#include "Actors/Buffs/BuffBase.h"
#include "Game/StandardGameState.h"
#include "Game/StandardGameMode.h"
#include "Game/GameRoundsTimeline.h"
#include "Actors/Vfx/VfxBase.h"
#include "Game/StandardPlayerState.h"
#include "Pawns/StandardModePlayerCharacter.h"
#include "Player/StandardModePlayerController.h"
#include "AI/StandardModeAIController.h"
#include "Actors/Managers/BuffQueue.h"
#include "Actors/Interfaces/BuffableActorInterface.h"

// Sets default values
ABuffBase::ABuffBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Lifetime = 1;
	BuffEndingDuration = 1.f;
	MaxCountPerTarget = 0;
	bAppliedToTarget = false;
	bPendingEnd = false;
	BuffDurationOnRoundBegin = 0.01f;
	BuffDurationOnRoundEnd = 0.01f;
}

void ABuffBase::SetLifetime(float NewLifetime)
{
	bPermanentBuff = NewLifetime == 0;

	AStandardGameState* StandardGameState = GetWorld()->GetGameState<AStandardGameState>();
	if (StandardGameState != nullptr)
	{
		LifetimeQueue = StandardGameState->GetGameRoundsTimeline()->GetLifetime(NewLifetime);
	}
}

void ABuffBase::SetSourcePlayerController(AController* PlayerController)
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

bool ABuffBase::IsCompatibleWith(AActor* Target)
{
	return CheckBuffCompatibility(Target);
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

void ABuffBase::EndBuff()
{
	if (bEnded)
	{
		return;
	}
	else
	{
		bEnded = true;
	}

	// Remove buff
	RemoveBuff();

	// Unregister delegate
	AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode());
	if (StandardGameMode != nullptr)
	{
		StandardGameMode->OnPlayerRoundEnd.RemoveDynamic(this, &ABuffBase::OnPlayerRoundEnd);
		StandardGameMode->OnPlayerDead.RemoveDynamic(this, &ABuffBase::OnPlayerDead);
	}

	// Start ending timer
	GetWorldTimerManager().SetTimer(BuffEndTimerHandle, this, &ABuffBase::OnBuffEnded, BuffEndingDuration);
}

void ABuffBase::OnTargetPlayerRoundBegin()
{
	GetWorldTimerManager().SetTimer(BuffProcessTimerHandle, this, &ABuffBase::OnBuffProcessEnd, BuffDurationOnRoundBegin);
}

void ABuffBase::OnTargetPlayerRoundEnd()
{
	GetWorldTimerManager().SetTimer(BuffProcessTimerHandle, this, &ABuffBase::OnBuffProcessEnd, BuffDurationOnRoundEnd);
}

void ABuffBase::OnBuffProcessEnd()
{
	GetWorldTimerManager().ClearTimer(BuffProcessTimerHandle);

	OnBuffProcessEndedEvent.Broadcast(this);
}

void ABuffBase::OnBuffEnded()
{
	GetWorldTimerManager().ClearTimer(BuffEndTimerHandle);

	OnBuffEndedEvent.Broadcast(this);

	// Destroy this actor
	Destroy();
}

void ABuffBase::ApplyBuff()
{
	// Spawn Vfx if specified
	if (IsValid(VfxClass))
	{
		VfxActor = GetWorld()->SpawnActor<AVfxBase>(VfxClass);
		// Set the vfx target actor
		VfxActor->SetTargetActor(TargetActor);
		// Set owner
		VfxActor->OwnerController = SourcePlayerController;
	}

	// Register buff to target player
	if (APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		if (IBuffableActorInterface* BuffableActor = Cast<IBuffableActorInterface>(TargetPawn))
		{
			BuffableActor->GetBuffQueue()->RegisterBuff(this);
		}

		// Show floating text
		if (AStandardModePlayerCharacter* PlayerCharacter = Cast<AStandardModePlayerCharacter>(TargetPawn))
		{
			PlayerCharacter->MulticastAddFloatingText(GetBuffStartText());
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

	// Unregister buff from target player
	if (APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		if (IBuffableActorInterface* BuffableActor = Cast<IBuffableActorInterface>(TargetPawn))
		{
			BuffableActor->GetBuffQueue()->UnregisterBuff(this);
		}

		// Show floating text
		if (AStandardModePlayerCharacter* PlayerCharacter = Cast<AStandardModePlayerCharacter>(TargetPawn))
		{
			PlayerCharacter->MulticastAddFloatingText(GetBuffEndText());
		}
	}
}

bool ABuffBase::CheckBuffCompatibility(AActor* TestActor)
{
	if (IBuffableActorInterface* BuffableActor = Cast<IBuffableActorInterface>(TestActor))
	{
		if (UBuffQueue* BuffQueue = BuffableActor->GetBuffQueue())
		{
			if (MaxCountPerTarget == 0)
				return true;

			const int32 CurrentCount = BuffQueue->GetBuffCountOfType(GetClass()->GetFName());
			if (CurrentCount < MaxCountPerTarget)
			{
				return true;
			}
		}
	}

	return false;
}

FText ABuffBase::GetBuffStartText() const
{
	return BuffStartText.GetLocalizeText();
}

FText ABuffBase::GetBuffEndText() const
{
	return BuffEndText.GetLocalizeText();
}

void ABuffBase::RegisterCallbackToCharacter(AActor* InActor)
{
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(InActor))
	{
		StandardModePlayerCharacter->OnCharacterDead.AddDynamic(this, &ABuffBase::OnTargetActorDead);
		if (UBuffQueue* BuffQueue = StandardModePlayerCharacter->GetBuffQueue())
		{
			BuffQueue->RegisterBuff(this);
		}
	}
}

void ABuffBase::UnregisterCallbackFromCharacter()
{
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(TargetActor))
	{
		StandardModePlayerCharacter->OnCharacterDead.RemoveDynamic(this, &ABuffBase::OnTargetActorDead);
		if (UBuffQueue* BuffQueue = StandardModePlayerCharacter->GetBuffQueue())
		{
			BuffQueue->UnregisterBuff(this);
		}
	}
}

void ABuffBase::OnPlayerRoundEnd(int32 PlayerId)
{
	// Shorten lifetime
	LifetimeQueue.RemoveSingle(PlayerId);

	if (LifetimeQueue.Num() == 0 && !bPermanentBuff)
	{
		bPendingEnd = true;
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

	EndBuff();
}
