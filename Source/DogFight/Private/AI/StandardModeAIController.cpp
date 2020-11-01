// Dog Fight Game Code By CYM.


#include "StandardModeAIController.h"

#include "AIType.h"
#include "DogFight.h"
#include "StandardGameMode.h"
#include "GameFramework/PlayerState.h"
#include "StandardGameMode.h"
#include "StandardPlayerState.h"
#include "StandardModePlayerController.h"
#include "StandardModePlayerCharacter.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "CardBase.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

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

	// Mark as AI
	MyPlayerState->SetIsABot(true);

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
		CharacterPawn->SetPlayerState(PlayerState);
		CharacterPawn->SetUnitName(MyPlayerState->GetPlayerName());
		CharacterPawn->OnCharacterDead.AddDynamic(this, &AStandardModeAIController::OnCharacterPawnDead);
		CharacterPawn->OnCharacterHealthChanged.AddDynamic(this, &AStandardModeAIController::OnHealthChanged);
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

bool AStandardModeAIController::UseCardByCategoryFlags(int32 CategoryFlags)
{
	// Check if there is card available
	if (AStandardPlayerState* StandardPlayerState = GetPlayerState<AStandardPlayerState>())
	{
		if (StandardPlayerState->GetCurrentCardCount() > 0)
		{
			TArray<ACardBase*> CardInstanceList = StandardPlayerState->GetCardInstanceList();
			bool bFound = false;
			for (int32 Index = 0; Index < CardInstanceList.Num(); ++Index)
			{
				if (CardInstanceList[Index]->IsCardMatchCategoryFlags(CategoryFlags))
				{
					StandardPlayerState->CmdUseCardByIndex(Index);
					bFound = true;
					break;
				}
			}

			if (bFound)
			{
				SetState(EStandardModeAIControllerState::UsingCard);
				return true;
			}
		}
	}

	// Failed to use card will return false
	return false;
}

AStandardModePlayerCharacter* AStandardModeAIController::AcquireTargetPlayerCharacter(int32 TargetFlags)
{
	if (TEST_PLAYER_FLAG(TargetFlags, EFindPlayerFlags::EFP_Self))
	{
		return CharacterPawn;
	}

	TArray<FPlayerRelationStatistic> PlayerRelationStatisticList;
	if (AStandardPlayerState* StandardPlayerState = GetPlayerState<AStandardPlayerState>())
	{
		PlayerRelationStatisticList = StandardPlayerState->GetPlayerRelationStatisticList();
	}
	else
	{
		UE_LOG(LogDogFight, Error, TEXT("Failed to get StandardPlayerState."));
		return nullptr;
	}

	if (TEST_PLAYER_FLAG(TargetFlags, EFindPlayerFlags::EFP_Enemy))
	{
		FilterForEnemyPlayer(PlayerRelationStatisticList);
	}
	else if (TEST_PLAYER_FLAG(TargetFlags, EFindPlayerFlags::EFP_Ally))
	{
		FilterForAllyPlayer(PlayerRelationStatisticList);
	}

	if (TEST_PLAYER_FLAG(TargetFlags, EFindPlayerFlags::EFP_Alive))
	{
		FilterForAlivePlayer(PlayerRelationStatisticList);
	}
	else if (TEST_PLAYER_FLAG(TargetFlags, EFindPlayerFlags::EFP_Dead))
	{
		FilterForDeadPlayer(PlayerRelationStatisticList);
	}

	if (TEST_PLAYER_FLAG(TargetFlags, EFindPlayerFlags::EFP_Human))
	{
		FilterForHumanPlayer(PlayerRelationStatisticList);
	}
	else if (TEST_PLAYER_FLAG(TargetFlags, EFindPlayerFlags::EFP_AI))
	{
		FilterForAIPlayer(PlayerRelationStatisticList);
	}

	if (PlayerRelationStatisticList.Num() > 0)
	{
		FPlayerRelationStatistic const * TargetPlayerStatistic = &PlayerRelationStatisticList[0];
		AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode());
		if (TargetPlayerStatistic->IsAIPlayer)
		{
			AStandardModeAIController* AIController = StandardGameMode->GetAIControllerById(TargetPlayerStatistic->PlayerId);

			UBrainComponent* MyBrainComponent = GetBrainComponent();
			MyBrainComponent->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(FName(TEXT("TargetCharacter")), AIController->GetCharacterPawn());
			return AIController->GetCharacterPawn();
		}
		else
		{
			AStandardModePlayerController* PlayerController = StandardGameMode->GetPlayerControllerById(TargetPlayerStatistic->PlayerId);
			return PlayerController->GetCharacterPawn();
		}
	}

	return nullptr;
}

void AStandardModeAIController::FilterForEnemyPlayer(TArray<FPlayerRelationStatistic>& ResultArray)
{
	// Sort player with ascending order based on RelationPoint
	ResultArray.Sort([](const FPlayerRelationStatistic& PlayerA, const FPlayerRelationStatistic& PlayerB)
	{
		return PlayerA.RelationPoint < PlayerB.RelationPoint;
	});

	// Filter out players do not meet requirement
	for (int32 Index = ResultArray.Num() - 1; Index >= 0; --Index)
	{
		if (ResultArray[Index].RelationPoint > 0)
		{
			ResultArray.RemoveAt(Index);
		}
		else
		{
			// Stop loop since the array is sorted and no need to check other player
			break;
		}
	}
}

void AStandardModeAIController::FilterForAllyPlayer(TArray<FPlayerRelationStatistic>& ResultArray)
{
	// Sort player with descending order based on RelationPoint
	ResultArray.Sort([](const FPlayerRelationStatistic& PlayerA, const FPlayerRelationStatistic& PlayerB)
	{
		return PlayerA.RelationPoint > PlayerB.RelationPoint;
	});

	// Filter out players do not meet requirement
	for (int32 Index = ResultArray.Num() - 1; Index >= 0; --Index)
	{
		if (ResultArray[Index].RelationPoint < 0)
		{
			ResultArray.RemoveAt(Index);
		}
		else
		{
			break;
		}
	}
}

void AStandardModeAIController::FilterForHumanPlayer(TArray<FPlayerRelationStatistic>& ResultArray)
{
	TArray<int32> FilterOutIndex;
	for (int32 Index = ResultArray.Num() - 1; Index >= 0; --Index)
	{
		if (ResultArray[Index].IsAIPlayer)
		{
			FilterOutIndex.Add(Index);
		}
	}

	for (int32 Index : FilterOutIndex)
	{
		ResultArray.RemoveAt(Index);
	}
}

void AStandardModeAIController::FilterForAIPlayer(TArray<FPlayerRelationStatistic>& ResultArray)
{
	TArray<int32> FilterOutIndex;
	for (int32 Index = ResultArray.Num() - 1; Index >= 0; --Index)
	{
		if (!ResultArray[Index].IsAIPlayer)
		{
			FilterOutIndex.Add(Index);
		}
	}

	for (int32 Index : FilterOutIndex)
	{
		ResultArray.RemoveAt(Index);
	}
}

void AStandardModeAIController::FilterForAlivePlayer(TArray<FPlayerRelationStatistic>& ResultArray)
{
	TArray<int32> FilterOutIndex;
	for (int32 Index = ResultArray.Num() - 1; Index >= 0; --Index)
	{
		if (ResultArray[Index].CurrentHealth <= 0)
		{
			FilterOutIndex.Add(Index);
		}
	}

	for (int32 Index : FilterOutIndex)
	{
		ResultArray.RemoveAt(Index);
	}
}

void AStandardModeAIController::FilterForDeadPlayer(TArray<FPlayerRelationStatistic>& ResultArray)
{
	TArray<int32> FilterOutIndex;
	for (int32 Index = ResultArray.Num() - 1; Index >= 0; --Index)
	{
		if (ResultArray[Index].CurrentHealth > 0)
		{
			FilterOutIndex.Add(Index);
		}
	}

	for (int32 Index : FilterOutIndex)
	{
		ResultArray.RemoveAt(Index);
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
	// Use target pawn if it's already specified
	if (TargetCharacterPawn != nullptr)
	{
		FCardInstructionTargetInfo NewTargetInfo;
		NewTargetInfo.ActorPtr = TargetCharacterPawn;
		NewTargetInfo.TargetType = ECardInstructionTargetType::Actor;

		OnCardTargetInfoAcquired.Broadcast(NewTargetInfo);

		// Let character facing target
		const FVector AimingDirection = TargetCharacterPawn->GetActorLocation() - CharacterPawn->GetActorLocation();
		if (AimingDirection.Size() > 0.01f)
		{
			CharacterPawn->SetAimingDirection(AimingDirection);
		}

		return;
	}

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

void AStandardModeAIController::OnHealthChanged(int32 NewHealth)
{
	if (APlayerState* MyPlayerState = GetPlayerState<APlayerState>())
	{
		OnPlayerHealthChanged.Broadcast(MyPlayerState->GetPlayerId(), NewHealth);
	}
}
