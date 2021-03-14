// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionEquipWeapon.h"
#include "Actors/Weapons/WeaponBase.h"
#include "Actors/Interfaces/WeaponCarrierInterface.h"

UInstructionEquipWeapon::UInstructionEquipWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoFinish = false;
}

bool UInstructionEquipWeapon::HandleActorTarget(AActor* Target)
{
	IWeaponCarrierInterface* WeaponCarrier = Cast<IWeaponCarrierInterface>(Target);
	const bool bWeaponValid = IsValid(WeaponClass);
	if (WeaponCarrier != nullptr && bWeaponValid)
	{
		// Spawn new weapon for target actor
		UWeaponBase* NewWeapon = NewObject<UWeaponBase>(Target, WeaponClass);
		if (IsValid(NewWeapon))
		{
			WeaponCarrier->GetWeaponEquippedEvent().AddDynamic(this, &UInstructionEquipWeapon::OnWeaponEquipped);
			WeaponCarrier->EquipWeapon(NewWeapon);
			WaitingCount++;
			return true;
		}
	}

	return false;
}

void UInstructionEquipWeapon::OnWeaponEquipped(AActor* CarrierActor)
{
	IWeaponCarrierInterface* Carrier = Cast<IWeaponCarrierInterface>(CarrierActor);
	if (Carrier == nullptr)
	{
		return;
	}

	// Unregister callback
	Carrier->GetWeaponEquippedEvent().RemoveDynamic(this, &UInstructionEquipWeapon::OnWeaponEquipped);

	WaitingCount--;
	// Finish after all carriers equipped weapon
	if (WaitingCount <= 0)
	{
		Finish();
	}
}
