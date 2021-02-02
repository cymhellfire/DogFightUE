// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionAddBuff.h"

#include "Card/CardBase.h"
#include "Actors/Interfaces/GameBuffInterface.h"

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
						UE_LOG(LogGameCards, Error, TEXT("Failed to spawn buff actor with class %s"), *BuffClass->GetName());
					}
				}
			}
			else
			{
				UE_LOG(LogGameCards, Error, TEXT("Invalid Buff class %s. (IGameBuffInterface is not implemented.)"), *BuffClass->GetName());
			}
		}
		else
		{
			UE_LOG(LogGameCards, Error, TEXT("No valid buff class specified."));
		}
	}

	return false;
}