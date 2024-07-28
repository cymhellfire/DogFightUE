// Dog Fight Game Code By CYM.


#include "PlayerController/TopDownStyleBotController.h"

#include "Pawn/PlayerCharacter/TopDownStylePlayerCharacter.h"
#include "Player/TopDownStylePlayerState.h"


// Sets default values
ATopDownStyleBotController::ATopDownStyleBotController()
{
}

// Called when the game starts or when spawned
void ATopDownStyleBotController::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ATopDownStyleBotController::SpawnCharacterPawn()
{
	if (!IsValid(CharacterClass))
	{
		return false;
	}

	if (HasAuthority())
	{
		if (CharacterPawn == nullptr)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			CharacterPawn = GetWorld()->SpawnActor<ATopDownStylePlayerCharacter>(CharacterClass, FVector::ZeroVector,
				FRotator::ZeroRotator, SpawnParameters);

			// Establish dependency on character
			if (auto MyPlayerState = GetPlayerState<ATopDownStylePlayerState>())
			{
				MyPlayerState->InitWithCharacter(CharacterPawn);
			}
		}
	}
	return false;
}
