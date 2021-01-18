// Dog Fight Game Code By CYM.


#include "Actors/Buffs/BasicArmorBuff.h"
#include "Actors/Components/ReceiveDamageComponent.h"
#include "Actors/ActorInterfaces.h"
#include "Common/Localization.h"

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

FText ABasicArmorBuff::GetBuffStartText() const
{
	FFormatOrderedArguments FormatArgumentValues;
	FormatArgumentValues.Add(ExtraArmor.GetArmorEffectText());

	return FText::Format(FText::FromStringTable(ST_BUFF_LOC, TEXT("FloatText_ArmorBegin")), FormatArgumentValues);
}

FText ABasicArmorBuff::GetBuffEndText() const
{
	FFormatOrderedArguments FormatArgumentValues;
	FormatArgumentValues.Add(ExtraArmor.GetArmorEffectText());

	return FText::Format(FText::FromStringTable(ST_BUFF_LOC, TEXT("FloatText_ArmorEnd")), FormatArgumentValues);
}
