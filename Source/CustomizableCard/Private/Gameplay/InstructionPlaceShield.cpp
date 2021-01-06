// Dog Fight Game Code By CYM.

#include "InstructionPlaceShield.h"
#include "CardBase.h"
#include "CustomizableCard.h"

UInstructionPlaceShield::UInstructionPlaceShield(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ShieldBlockType = EShieldBlockType::SBT_Enemy;
	ShieldLifetime = 1;
}

void UInstructionPlaceShield::HandleActorTarget(AActor* Target)
{
	Super::HandleActorTarget(Target);

	if (IsValid(Target))
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
			}
			else
			{
				UE_LOG(LogCustomizableCard, Error, TEXT("Failed to spawn shield actor with class %s"), *ShieldClass->GetName());
			}
		}
		else
		{
			UE_LOG(LogCustomizableCard, Error, TEXT("No valid shield class specified."));
		}
	}
}

