// Dog Fight Game Code By CYM.


#include "StandardModeAIController.h"
#include "DogFight.h"
#include "StandardGameMode.h"
#include "GameFramework/PlayerState.h"
#include "StandardGameMode.h"
#include "StandardPlayerState.h"
#include "StandardModePlayerCharacter.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"

namespace EStandardModeAIControllerState
{
	const FName Idle = FName(TEXT("Idle"));
	const FName UsingCard = FName(TEXT("UsingCard"));
	const FName WaitingOrder = FName(TEXT("WaitingOrder"));
	const FName Dead = FName(TEXT("Dead"));
}

AStandardModeAIController::AStandardModeAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// AI player also need a PlayerState
	bWantsPlayerState = 1;

	FreeMovementRadius = 5000.f;
}

void AStandardModeAIController::InitPlayerState()
{
	Super::InitPlayerState();

	// Set name for AI controller
	APlayerState* MyPlayerState = GetPlayerState<APlayerState>();
	if (MyPlayerState == nullptr)
	{
		UE_LOG(LogDogFight, Error, TEXT("No PlayerState for AIController %s"), *GetName());
		return;
	}

	// Register this controller to current GameMode
	UWorld* World = GetWorld();
	AStandardGameMode* StandardGameMode = World ? Cast<AStandardGameMode>(World->GetAuthGameMode()) : nullptr;
	if (StandardGameMode != nullptr)
	{
		MyPlayerState->SetPlayerId(StandardGameMode->GetAIControllerCount() + 1);
		MyPlayerState->SetPlayerName(FString::Printf(TEXT("AI%d"), MyPlayerState->GetPlayerId()));
		StandardGameMode->RegisterAIController(this);
		StandardGameMode->RegisterAIToTimeline(this);
	}

	// Register delegate
	if (AStandardPlayerState* StandardPlayerState = Cast<AStandardPlayerState>(MyPlayerState))
	{
		StandardPlayerState->OnUsingCardFinished.AddDynamic(this, &AStandardModeAIController::OnCardFinished);
	}

	// Spawn character
	if (World)
	{
		// Get a random PlayerStart as spawn location
		TArray<AActor*> StartPoints;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), StartPoints);
		const FVector StartPoint = StartPoints.Num() > 0 ? StartPoints[FMath::RandRange(0, StartPoints.Num() - 1)]->GetActorLocation() : FVector::ZeroVector;
		
		CharacterPawn = World->SpawnActor<AStandardModePlayerCharacter>(CharacterPawnClass, StartPoint, RootComponent->GetComponentRotation());
		UE_LOG(LogDogFight, Display, TEXT("Spawn AI pawn at %s"), *StartPoint.ToString());
		CharacterPawn->SetUnitName(MyPlayerState->GetPlayerName());
		CharacterPawn->OnCharacterDead.AddDynamic(this, &AStandardModeAIController::OnCharacterPawnDead);
	}
}

void AStandardModeAIController::BeginDestroy()
{
	// Unregister delegate
	if (AStandardPlayerState* StandardPlayerState = GetPlayerState<AStandardPlayerState>())
	{
		StandardPlayerState->OnUsingCardFinished.RemoveDynamic(this, &AStandardModeAIController::OnCardFinished);
	}

	Super::BeginDestroy();
}

void AStandardModeAIController::EnableFreeMovement()
{
	if (CharacterPawn == nullptr)
	{
		UE_LOG(LogDogFight, Error, TEXT("Cannot enable free movement without character pawn."));
		return;
	}
	
	// Get a random point from NavMesh
	FVector TargetPoint;
	UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, CharacterPawn->GetActorLocation(),
		TargetPoint, FreeMovementRadius);

	// Let character pawn move to target location
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(CharacterPawn->GetController(), TargetPoint);
}

void AStandardModeAIController::DisableFreeMovement()
{
	if (CharacterPawn == nullptr)
	{
		UE_LOG(LogDogFight, Error, TEXT("Cannot disable free movement without character pawn."));
		return;
	}

	CharacterPawn->StopMoveImmediately();
}

void AStandardModeAIController::StartAIRound()
{
	if (!IsValid(Blackboard))
	{
		UE_LOG(LogDogFight, Error, TEXT("No BehaviorTree attached to AIController."));
		return;
	}

	Blackboard->SetValueAsEnum(FName(TEXT("AIState")), (uint8)EAIControllerState::ACS_UsingCard);

	SetState(EStandardModeAIControllerState::WaitingOrder);
}

void AStandardModeAIController::StopAIRound()
{
	if (!IsValid(Blackboard))
	{
		UE_LOG(LogDogFight, Error, TEXT("No BehaviorTree attached to AIController."));
		return;
	}
	
	Blackboard->SetValueAsEnum(FName(TEXT("AIState")), (uint8)EAIControllerState::ACS_Idle);

	SetState(EStandardModeAIControllerState::Idle);
}

void AStandardModeAIController::UseRandomCard()
{
	if (AStandardPlayerState* MyPlayerState = GetPlayerState<AStandardPlayerState>())
	{
		const int32 CardCount = MyPlayerState->GetCurrentCardCount();
		if (CardCount > 0)
		{
			const int32 TargetIndex = FMath::RandRange(0, CardCount - 1);
			MyPlayerState->CmdUseCardByIndex(TargetIndex);

			SetState(EStandardModeAIControllerState::UsingCard);
		}
	}
}

void AStandardModeAIController::SetState(FName NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	CurrentState = NewState;
	OnStateChanged(CurrentState);
}

void AStandardModeAIController::RequestActorTarget()
{
	AController* TargetController = nullptr;
	AGameModeBase* GameModeBase = GetWorld()->GetAuthGameMode();
	if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GameModeBase))
	{
		while(true)
		{
			TargetController = StandardGameMode->GetRandomController();
			// Check if selected myself
			if (AStandardPlayerState* TargetPlayerState = TargetController->GetPlayerState<AStandardPlayerState>())
			{
				AStandardPlayerState* MyPlayerState = GetPlayerState<AStandardPlayerState>();
				if (MyPlayerState->GetPlayerId() != TargetPlayerState->GetPlayerId())
				{
					break;
				}
			}
		}
	}

	if (IGameCardUserPlayerControllerInterface* CardUserPlayerController = Cast<IGameCardUserPlayerControllerInterface>(TargetController))
	{
		FCardInstructionTargetInfo NewTargetInfo;
		NewTargetInfo.ActorPtr = CardUserPlayerController->GetActualPawn();
		NewTargetInfo.TargetType = ECardInstructionTargetType::Actor;

		OnCardTargetInfoAcquired.Broadcast(NewTargetInfo);

		// Let character facing target
		CharacterPawn->SetAimingDirection(CardUserPlayerController->GetActualPawn()->GetActorLocation() - CharacterPawn->GetActorLocation());
	}

}

void AStandardModeAIController::RequestPositionTarget()
{
}

void AStandardModeAIController::RequestDirectionTarget()
{
}

APawn* AStandardModeAIController::GetActualPawn()
{
	return Cast<APawn>(CharacterPawn);
}

void AStandardModeAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehaviorTree != nullptr)
	{
		RunBehaviorTree(AIBehaviorTree);
	}
}

void AStandardModeAIController::OnStateChanged(FName NewState)
{
	if (NewState == EStandardModeAIControllerState::Dead)
	{
		BrainComponent->StopLogic(FString(TEXT("Dead")));
	}
}

void AStandardModeAIController::OnCardFinished(bool bPlayerRoundFinished)
{
	SetState(EStandardModeAIControllerState::WaitingOrder);
}

void AStandardModeAIController::OnCharacterPawnDead()
{
	if (AStandardPlayerState* StandardPlayerState = GetPlayerState<AStandardPlayerState>())
	{
		SetState(EStandardModeAIControllerState::Dead);

		StandardPlayerState->SetAlive(false);
		OnAIPlayerDead.Broadcast(StandardPlayerState->GetPlayerId());
	}
}

