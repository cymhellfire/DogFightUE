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
	if (IsValid(Target))
	{
		if (IsValid(BuffClass))
		{
			IGameBuffInterface* BuffActor = GetWorld()->SpawnActor<IGameBuffInterface>(BuffClass);
			if (BuffActor != nullptr)
			{
				BuffActor->SetTargetActor(Target);
				BuffActor->SetLifetime(BuffLifetime);
				BuffActor->SetSourcePlayerController(GetOwnerCard()->GetOwnerPlayerController());
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