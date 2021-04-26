// Dog Fight Game Code By CYM.


#include "Game/StandardModeDamageCalculator.h"
#include "Actors/Interfaces/DamageableActorInterface.h"
#include "DogFight.h"
#include "Actors/Components/ReceiveDamageComponent.h"
#include "Game/DogFightDamageType.h"

float UStandardModeDamageCalculator::CalculateActualDamage(AActor* DamageTaker, float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Check and get ReceiveDamageComponent
	if (IDamageableActorInterface* DamageableActor = Cast<IDamageableActorInterface>(DamageTaker))
	{
		if (UReceiveDamageComponent* ReceiveDamageComponent = DamageableActor->GetDamageReceiveComponent())
		{
			// Erase damage if target is invincible
			if (ReceiveDamageComponent->IsInvincible())
				return 0;

			// Check the damage type
			UDogFightDamageType* DogFightDamageType = Cast<UDogFightDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject());
			if (DogFightDamageType != nullptr)
			{
				return HandleActorArmor(ReceiveDamageComponent, Damage, DogFightDamageType->DamageCategoryFlag, DamageEvent);
			}
			else
			{
				UE_LOG(LogDogFight, Error, TEXT("Failed to get default object of %s class."), *DamageEvent.DamageTypeClass->GetName());
			}
		}
	}

	// Return the original damage if no ReceiveDamageComponent found
	return Damage;
}

float UStandardModeDamageCalculator::HandleActorArmor(const UReceiveDamageComponent* ReceiveComponent, float Damage, int32 DamageCategory, FDamageEvent const& DamageEvent)
{
	float FinalDamage = Damage;

	// Test with physical armor
	if (TEST_DAMAGE_CATEGORY(DamageCategory, EDogFightDamageCategory::Damage_Physical))
	{
		FinalDamage -= ReceiveComponent->PhysicalArmor;
	}
	// Test with magical armor
	if (TEST_DAMAGE_CATEGORY(DamageCategory, EDogFightDamageCategory::Damage_Magical))
	{
		FinalDamage -= ReceiveComponent->MagicalArmor;
	}

	// Test with extra armor
	for(FActorArmor Armor : ReceiveComponent->GetExtraArmors())
	{
		if (Armor.IsAgainstDamageType(DamageEvent.DamageTypeClass))
		{
			FinalDamage -= Armor.ArmorValue;
		}
	}

	// Clamp the damage value
	FinalDamage = FMath::Clamp(FinalDamage, 0.f, FinalDamage);

	return FinalDamage;
}
