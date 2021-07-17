// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionSelectPartialPlayer.h"

#include "Actors/Interfaces/GameTargetProviderInterface.h"
#include "Card/CardBase.h"
#include "Game/StandardGameMode.h"
#include "Game/StandardGameState.h"

UInstructionSelectPartialPlayer::UInstructionSelectPartialPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SelectRatio = 0.5f;
	SelectCountCapMethod = EPartialSelectCapMethod::Floor;
}

void UInstructionSelectPartialPlayer::Execute()
{
	// Calculate target count
	float TargetCountFloat = 0.f;
	if (AStandardGameState* StandardGameState = GetWorld()->GetGameState<AStandardGameState>())
	{
		TargetCountFloat = StandardGameState->GetAlivePlayerCount() * SelectRatio;
	}

	int32 TargetCount = 0;
	if (SelectCountCapMethod == EPartialSelectCapMethod::Ceil)
	{
		TargetCount = FMath::CeilToInt(TargetCountFloat);
	}
	else
	{
		TargetCount = FMath::Floor(TargetCountFloat);
	}

	// Generate random targets with given count
	GeneratePlayerTarget(TargetCount);

	Finish();
}

void UInstructionSelectPartialPlayer::GeneratePlayerTarget(int32 TargetCount)
{
	AController* PlayerController = GetOwnerCard()->GetOwnerPlayerController();
	IGameTargetProviderInterface* Provider = Cast<IGameTargetProviderInterface>(PlayerController);
	if (Provider != nullptr)
	{
		while (GetOwnerCard()->GetTargetInfoCount() < TargetCount)
		{
			GetOwnerCard()->PushUniqueTargetInfo(Provider->RequestRandomActorTarget(false));
		}
	}
}
