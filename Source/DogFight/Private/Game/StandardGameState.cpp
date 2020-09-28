// Dog Fight Game Code By CYM.


#include "StandardGameState.h"

#include "StandardGameMode.h"

AStandardGameState::AStandardGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MiniMapCamera = nullptr;
	RemainingTime = 0;
}

void AStandardGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStandardGameState, RemainingTime);
	DOREPLIFETIME(AStandardGameState, CurrentGamePhase);
	DOREPLIFETIME(AStandardGameState, CountdownContentString);
}

void AStandardGameState::SetCurrentGamePhase(FName NewGamePhase)
{
	if (GetNetMode() == NM_Client)
		return;

	if (NewGamePhase == CurrentGamePhase)
		return;

	CurrentGamePhase = NewGamePhase;

	// Trigger the OnRep function on server side manually
	OnRep_CurrentGamePhase();
}

void AStandardGameState::SetRemainingTime(uint32 Time)
{
	RemainingTime = Time;

	// Invoke OnRep on server side manually
	if (GetNetMode() != NM_Client)
	{
		OnRep_RemainingTime();
	}
}

void AStandardGameState::SetCountdownContentString(FString NewString)
{
	if (GetNetMode() != NM_Client)
	{
		if (CountdownContentString != NewString)
		{
			CountdownContentString = NewString;

			// Invoke OnRep function on server side manually
			OnRep_CountdownContentString();
		}
	}
}

void AStandardGameState::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetNetMode() == NM_Client)
		return;

	// Acquire current game phase as GameState begin play
	if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
	{
		SetCurrentGamePhase(StandardGameMode->GetGamePhase());
	}
}

bool AStandardGameState::ShouldCountdownDisplay() const
{
	return CurrentGamePhase == GamePhase::WaitingForStart || CurrentGamePhase == GamePhase::FreeMoving;
}

void AStandardGameState::OnRep_CurrentGamePhase()
{
	OnGamePhaseChanged.Broadcast(CurrentGamePhase);

	// Update the countdown context string once game phase changed
	if (ShouldCountdownDisplay())
	{
		CountdownContentString = FString::FromInt(RemainingTime);
		OnCountdownContentStringChanged.Broadcast();
	}
}

void AStandardGameState::OnRep_RemainingTime()
{
	if (ShouldCountdownDisplay())
	{
		CountdownContentString = FString::FromInt(RemainingTime);
		OnCountdownContentStringChanged.Broadcast();
	}
}

void AStandardGameState::OnRep_CountdownContentString()
{
	OnCountdownContentStringChanged.Broadcast();
}
