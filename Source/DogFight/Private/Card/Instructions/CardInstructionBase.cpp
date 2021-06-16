// Dog Fight Game Code By CYM.


#include "Card/Instructions/CardInstructionBase.h"

#include "Actors/Interfaces/GameCardUserPlayerControllerInterface.h"
#include "Card/CardBase.h"
#include "Game/StandardGameMode.h"
#include "GameFramework/PlayerState.h"

UCardInstructionBase::UCardInstructionBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstructionName = TEXT("NewInstruction");
	ExecuteType = ECardInstructionExecuteType::Synchronous;
	CameraFocusType = EInstructionCameraFocusType::ICFT_None;
	bForciblyCameraEventToOwner = false;
}

bool UCardInstructionBase::Tick(float DeltaSeconds)
{
	// Invoke Execute at the first frame
	Execute();

	// No need to tick after Execute()
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);

	return true;
}

void UCardInstructionBase::ApplyMeta(const FInstructionMetaBase* MetaData)
{
	check(MetaData);

	InstructionName = MetaData->InstructionName;
	ExecuteType = MetaData->ExecuteType;
}

void UCardInstructionBase::BeginExecute()
{
	// Register tick for execution
	TickDelegate = FTickerDelegate::CreateUObject(this, &UCardInstructionBase::Tick);
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);

	// Increase counter of owner card
	OwnerCard->IncreaseExecutingInstructionCounter();
}

void UCardInstructionBase::Execute()
{
	// Invoke blueprint implemented events if this class is blueprint based.
	if (GetClass()->HasAnyClassFlags(CLASS_CompiledFromBlueprint) || !GetClass()->HasAnyClassFlags(CLASS_Native))
	{
		BeginExecuteBP();
	}

	// Check to broadcast camera focus event
	if (CameraFocusType == EInstructionCameraFocusType::ICFT_User)
	{
		if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
		{
			if (IGameCardUserPlayerControllerInterface* PlayerController = Cast<IGameCardUserPlayerControllerInterface>(OwnerCard->GetOwnerPlayerController()))
			{
				const FVector CurrentPos = PlayerController->GetActualPawn()->GetActorLocation();
				if (bForciblyCameraEventToOwner)
				{
					StandardGameMode->BroadcastCameraFocusEvent(
						FCameraFocusEvent
						{
							OwnerCard->GetOwnerPlayerController()->GetPlayerState<APlayerState>()->GetPlayerId(),
							CurrentPos.X,
							CurrentPos.Y,
							ECameraFocusEventType::Type::OwnerForced
						});
				}
				else
				{
					StandardGameMode->BroadcastCameraFocusEvent(
						FCameraFocusEvent
						{
							-1,
							CurrentPos.X,
							CurrentPos.Y,
							ECameraFocusEventType::Type::Default
						});
				}
			}
		}
	}
}

void UCardInstructionBase::Finish()
{
	if (bIsFinished)
		return;

	bIsFinished = true;

	// Decrease counter of owner card
	OwnerCard->DecreaseExecutingInstructionCounter();
}

APawn* UCardInstructionBase::GetOwnerControlledPawn() const
{
	AController* PlayerController = GetOwnerCard()->GetOwnerPlayerController();
	if (IGameCardUserPlayerControllerInterface* CardUserController = Cast<IGameCardUserPlayerControllerInterface>(PlayerController))
	{
		return CardUserController->GetActualPawn();
	}

	return nullptr;
}
