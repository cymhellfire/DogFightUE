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

bool UInstructionAddBuff::HandleActorTarget(AActor* Target)
{
	const bool Result = Super::HandleActorTarget(Target);

	if (Result && IsValid(Target))
	{
		if (IsValid(BuffClass))
		{
			// Check compatibility before spawn buff
			if (IGameBuffInterface* BuffCDO = Cast<IGameBuffInterface>(BuffClass->GetDefaultObject()))
			{
				if (BuffCDO->IsCompatibleWith(Target))
				{
					IGameBuffInterface* BuffActor = GetWorld()->SpawnActor<IGameBuffInterface>(BuffClass);
					if (BuffActor != nullptr)
					{
						OnBuffCreated(BuffActor);

						BuffActor->SetSourcePlayerController(GetOwnerCard()->GetOwnerPlayerController());
						BuffActor->SetTargetActor(Target);
						BuffActor->SetLifetime(BuffLifetime);

						// Return true only when buff is created and applied
						return true;
					}
					else
					{
						UE_LOG(LogCustomizableCard, Error, TEXT("Failed to spawn buff actor with class %s"), *BuffClass->GetName());
					}
				}
			}
			else
			{
				UE_LOG(LogCustomizableCard, Error, TEXT("Invalid Buff class %s. (IGameBuffInterface is not implemented.)"), *BuffClass->GetName());
			}
		}
		else
		{
			UE_LOG(LogCustomizableCard, Error, TEXT("No valid buff class specified."));
		}
	}

	return false;
}