// Dog Fight Game Code By CYM.


#include "BasicArmorBuff.h"
#include "ReceiveDamageComponent.h"
#include "ActorInterfaces.h"

void ABasicArmorBuff::ApplyBuff()
{
	Super::ApplyBuff();

	if (IDamageableActorInterface* DamageableActor = Cast<IDamageableActorInterface>(TargetActor))
	{
		DamageableActor->AddExtraArmor(ExtraArmor);
	}
}

void ABasicArmorBuff::RemoveBuff()
{
	Super::RemoveBuff();

	if (IDamageableActorInterface* DamageableActor = Cast<IDamageableActorInterface>(TargetActor))
	{
		DamageableActor->RemoveExtraArmor(ExtraArmor);
	}
}

void ABasicArmorBuff::InitializeArmor(int32 ArmorValue, int32 DamageCategories, TArray<TSubclassOf<UDamageType>> DesiredTypes)
{
	ExtraArmor = FActorArmor{ArmorValue, DamageCategories, DesiredTypes, this};
}
