#include "Game/GameWorkflow/StandardGameMode/StandardGameModePhaseBase.h"

#include "DogFight.h"
#include "Game/StandardGameMode.h"

UStandardGameModePhaseBase::UStandardGameModePhaseBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ParentStandardGameMode = nullptr;
}

void UStandardGameModePhaseBase::SetOwnerGameMode(AGameModeBase* InGameMode)
{
	Super::SetOwnerGameMode(InGameMode);

	if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(InGameMode))
	{
		ParentStandardGameMode = StandardGameMode;
	}
}

bool UStandardGameModePhaseBase::StartPhase()
{
	if (!Super::StartPhase())
	{
		return false;
	}

	const bool Result = IsValid(ParentStandardGameMode);
	if (!Result)
	{
		UE_LOG(LogDogFight, Error, TEXT("[GamePhase] Game phase %s does not have valid parent Game Mode."), *GamePhaseName.ToString());
	}

	return Result;
}

void UStandardGameModePhaseBase::EndPhase()
{
	Super::EndPhase();
}

void UStandardGameModePhaseBase::InterruptPhase()
{
	Super::InterruptPhase();
}

void UStandardGameModePhaseBase::ResumePhase()
{
	Super::ResumePhase();
}
