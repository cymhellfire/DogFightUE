// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionEnqueueWeaponInput.h"
#include "Actors/Interfaces/WeaponCarrierInterface.h"

bool UInstructionEnqueueWeaponInput::HandleActorTarget(AActor* Target)
{
	bool Result = Super::HandleActorTarget(Target);
	if (Result)
	{
		if (IWeaponCarrierInterface* WeaponCarrier = Cast<IWeaponCarrierInterface>(GetOwnerControlledPawn()))
		{
			if (WeaponCarrier->GetCurrentWeapon())
			{
				bAutoFinish = false;

				WeaponCarrier->GetWeaponActionFinishedEvent().AddDynamic(this, &UInstructionEnqueueWeaponInput::OnActionFinished);
				WeaponCarrier->SetWeaponTargetActor(Target);
				WeaponCarrier->EnqueueInput(WeaponInput);
			}
			else
			{
				Result = false;
			}
		}
		else
		{
			Result = false;
		}
	}

	return Result;
}

void UInstructionEnqueueWeaponInput::OnActionFinished(AActor* Carrier)
{
	if (IWeaponCarrierInterface* WeaponCarrier = Cast<IWeaponCarrierInterface>(Carrier))
	{
		WeaponCarrier->GetWeaponActionFinishedEvent().RemoveDynamic(this, &UInstructionEnqueueWeaponInput::OnActionFinished);
	}

	Finish();
}
