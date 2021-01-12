// Dog Fight Game Code By CYM.


#include "Gameplay/InstructionAddBuff.h"

#include "CustomizableCard.h"
#include "Gameplay/CardBase.h"
#include "Interfaces/GameBuffInterface.h"

UInstructionAddBuff::UInstructionAddBuff(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BuffLifetime = 1.f;
}

void UInstructionAddBuff::HandleActorTarget(AActor* Target)
{
	Super::HandleActorTarget(Target);

	if (IsValid(Target))
	{
		if (IsValid(BuffClass))
		{
			IGameBuffInterface* BuffActor = GetWorld()->SpawnActor<IGameBuffInterface>(BuffClass);
			if (BuffActor != nullptr)
			{
				OnBuffCreated(BuffActor);

				BuffActor->SetSourcePlayerController(GetOwnerCard()->GetOwnerPlayerController());
				BuffActor->SetTargetActor(Target);
				BuffActor->SetLifetime(BuffLifetime);
			}
			else
			{
				UE_LOG(LogCustomizableCard, Error, TEXT("Failed to spawn buff actor with class %s"), *BuffClass->GetName());
			}
		}
		else
		{
			UE_LOG(LogCustomizableCard, Error, TEXT("No valid buff class specified."));
		}
	}
}