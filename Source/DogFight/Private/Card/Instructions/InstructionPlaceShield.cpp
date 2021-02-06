// Dog Fight Game Code By CYM.

#include "Card/Instructions/InstructionPlaceShield.h"


#include "Actors/Buffs/Buff_Shield.h"
#include "Actors/Shield/ShieldBase.h"
#include "Card/CardBase.h"

UInstructionPlaceShield::UInstructionPlaceShield(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ShieldBlockType = EShieldBlockType::SBT_Enemy;
	ShieldLifetime = 1;
}

bool UInstructionPlaceShield::HandleActorTarget(AActor* Target)
{
	CurrentTargetActor = Target;

	return Super::HandleActorTarget(Target);
}

void UInstructionPlaceShield::OnBuffCreated(IGameBuffInterface* BuffActor)
{
	if (!IsValid(CurrentTargetActor))
		return;

	if (IsValid(ShieldClass))
	{
		AActor* NewActor = GetWorld()->SpawnActor<AActor>(ShieldClass);
		ABuff_Shield* ShieldBuff = Cast<ABuff_Shield>(BuffActor);

		if (AShieldBase* ShieldActor = Cast<AShieldBase>(NewActor))
		{
			ShieldActor->SetSourcePlayerController(GetOwnerCard()->GetOwnerPlayerController());
			ShieldActor->SetShieldBlockType(ShieldBlockType);
			ShieldActor->SetAttachActor(CurrentTargetActor);
			ShieldActor->SetLifetime(ShieldLifetime);

			ShieldBuff->SetShieldActor(ShieldActor);
		}
		else
		{
			NewActor->Destroy();
			UE_LOG(LogGameCards, Error, TEXT("Failed to spawn shield actor with class %s"), *ShieldClass->GetName());
		}
	}
	else
	{
		UE_LOG(LogGameCards, Error, TEXT("No valid shield class specified."));
	}
}

