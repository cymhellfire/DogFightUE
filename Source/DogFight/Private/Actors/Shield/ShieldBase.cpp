// Dog Fight Game Code By CYM.


#include "ShieldBase.h"

#include "DogFightGameModeBase.h"
#include "GameRoundsTimeline.h"
#include "ProjectileBase.h"
#include "ShieldManager.h"
#include "StandardGameMode.h"
#include "StandardGameState.h"

AShieldBase::AShieldBase()
{
	// Create shield mesh component
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->SetCollisionProfileName(FName(TEXT("Shield")));

	// Change the hierarchy
	RootComponent = ShieldMesh;

	BlockType = EShieldBlockType::SBT_NotMine;
}

bool AShieldBase::CheckShouldBlockProjectile(AProjectileBase* TestProjectile)
{
	// Only do block judgement on server side
	if (GetNetMode() == NM_Client)
	{
		return false;
	}

	if (ADogFightGameModeBase* GameModeBase = Cast<ADogFightGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		switch (BlockType)
		{
		case EShieldBlockType::SBT_Enemy:
			return GameModeBase->GetPlayersRelation(OwnerController, TestProjectile->OwnerController) == EPlayerRelation::PR_Enemy;
		case EShieldBlockType::SBT_Ally:
			return GameModeBase->GetPlayersRelation(OwnerController, TestProjectile->OwnerController) == EPlayerRelation::PR_Ally;
		case EShieldBlockType::SBT_NotMine:
			return OwnerController != TestProjectile->OwnerController;
		case EShieldBlockType::SBT_All:
		default:
			return true;
		}
	}

	return true;
}

void AShieldBase::SetAttachActor(AActor* NewParent)
{
	check(NewParent);

	// Check attach socket
	if (SocketName.IsNone())
	{
		SetActorLocation(NewParent->GetActorLocation());
	}
	else
	{
		AttachToActor(NewParent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}

	TargetActor = NewParent;
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(TargetActor))
	{
		StandardModePlayerCharacter->OnCharacterDead.AddDynamic(this, &AShieldBase::OnTargetActorDead);
	}
}

void AShieldBase::SetLifetime(int32 Lifetime)
{
	// Get the lifetime
	AStandardGameState* StandardGameState = GetWorld()->GetGameState<AStandardGameState>();
	if (StandardGameState != nullptr)
	{
		LifetimeQueue = StandardGameState->GetGameRoundsTimeline()->GetLifetime(Lifetime);
	}
}

void AShieldBase::SetSourcePlayerController(AController* NewController)
{
	OwnerController = NewController;
}

void AShieldBase::SetShieldBlockType(EShieldBlockType NewBlockType)
{
	BlockType = NewBlockType;
}

void AShieldBase::BeginPlay()
{
	Super::BeginPlay();

	// Register
	if (GetNetMode() != NM_Client)
	{
		if (ADogFightGameModeBase* GameModeBase = Cast<ADogFightGameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			if (AShieldManager* ShieldManager = GameModeBase->GetShieldManager())
			{
				ShieldManager->RegisterShield(this);
			}

			if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GameModeBase))
			{
				StandardGameMode->OnPlayerRoundEnd.AddDynamic(this, &AShieldBase::OnPlayerRoundEnd);
				StandardGameMode->OnPlayerDead.AddDynamic(this, &AShieldBase::OnPlayerDead);
			}
		}
	}
}

void AShieldBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Unregister
	if (GetNetMode() != NM_Client)
	{
		if (UWorld* MyWorld = GetWorld())
		{
			if (ADogFightGameModeBase* GameModeBase = Cast<ADogFightGameModeBase>(MyWorld->GetAuthGameMode()))
			{
				if (AShieldManager* ShieldManager = GameModeBase->GetShieldManager())
				{
					ShieldManager->UnregisterShield(this);
				}

				if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GameModeBase))
				{
					StandardGameMode->OnPlayerRoundEnd.RemoveDynamic(this, &AShieldBase::OnPlayerRoundEnd);
					StandardGameMode->OnPlayerDead.RemoveDynamic(this, &AShieldBase::OnPlayerDead);
				}
			}
		}
	}
}

void AShieldBase::OnTargetActorDead()
{
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(TargetActor))
	{
		StandardModePlayerCharacter->OnCharacterDead.RemoveDynamic(this, &AShieldBase::OnTargetActorDead);
	}

	Destroy();
}

void AShieldBase::OnPlayerRoundEnd(int32 PlayerId)
{
	// Shorten lifetime
	LifetimeQueue.RemoveSingle(PlayerId);

	if (LifetimeQueue.Num() == 0)
	{
		Destroy();
	}
}

void AShieldBase::OnPlayerDead(int32 PlayerId)
{
	// Remove the dead player from timeline
	LifetimeQueue.RemoveAll([PlayerId] (const int32 Item) {return Item == PlayerId;});
}
