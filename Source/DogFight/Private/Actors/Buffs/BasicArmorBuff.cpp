// Dog Fight Game Code By CYM.


#include "BasicArmorBuff.h"
#include "ReceiveDamageComponent.h"
#include "ActorInterfaces.h"

void ABasicArmorBuff::ApplyBuff()
{
	Super::ApplyBuff();

	if (IDamageableActorInterface* DamageableActor = Cast<IDamageableActorInterface>(TargetActor))
	{
		DamageableActor->SetPhysicalArmor(DamageableActor->GetPhysicalArmor() + PhysicalArmor);
		DamageableActor->SetMagicalArmor(DamageableActor->GetMagicalArmor() + MagicalArmor);
	}
}

void ABasicArmorBuff::RemoveBuff()
{
	Super::RemoveBuff();

	if (IDamageableActorInterface* DamageableActor = Cast<IDamageableActorInterface>(TargetActor))
	{
		DamageableActor->SetPhysicalArmor(DamageableActor->GetPhysicalArmor() - PhysicalArmor);
		DamageableActor->SetMagicalArmor(DamageableActor->GetMagicalArmor() - MagicalArmor);
	}
}
