// Dog Fight Game Code By CYM.

#include "Card/Instructions/InstructionPlaceShield.h"
#include "Card/CardBase.h"

UInstructionPlaceShield::UInstructionPlaceShield(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ShieldBlockType = EShieldBlockType::SBT_Enemy;
	ShieldLifetime = 1;
}

bool UInstructionPlaceShield::HandleActorTarget(AActor* Target)
{
	const bool Result = Super::HandleActorTarget(Target);

	if (Result && IsValid(Target))
	{
		if (IsValid(ShieldClass))
		{
			IGameShieldInterface* ShieldActor = GetWorld()->SpawnActor<IGameShieldInterface>(ShieldClass);

			if (ShieldActor != nullptr)
			{
				ShieldActor->SetSourcePlayerController(GetOwnerCard()->GetOwnerPlayerController());
				ShieldActor->SetShieldBlockType(ShieldBlockType);
				ShieldActor->SetAttachActor(Target);
				ShieldActor->SetLifetime(ShieldLifetime);

				return true;
			}
			else
			{
				UE_LOG(LogGameCards, Error, TEXT("Failed to spawn shield actor with class %s"), *ShieldClass->GetName());
			}
		}
		else
		{
			UE_LOG(LogGameCards, Error, TEXT("No valid shield class specified."));
		}
	}

	return false;
}

void UInstructionPlaceShield::OnBuffCreated(IGameBuffInterface* BuffActor)
{
	
}

