// Dog Fight Game Code By CYM.


#include "InstructionAddBuff.h"

#include "CustomizableCard.h"
#include "CardBase.h"
#include "GameBuffInterface.h"

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
