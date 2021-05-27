// Dog Fight Game Code By CYM.


#include "Actors/Buffs/CharacterPropertyBuff.h"

#include "Pawns/StandardModePlayerCharacter.h"

void FCharacterPropertyModifier::ApplyToCharacter(AStandardModePlayerCharacter* Target) const
{
	if (AffectProperty == ECharacterProperty::CP_HealthPoint)
	{
		Target->SetMaxHealth(CalculateValue(Target->MaxBaseHealth, ModifyMethod));
	}
	else if (AffectProperty == ECharacterProperty::CP_StrengthPoint)
	{
		Target->SetMaxStrength(CalculateValue(Target->MaxStrength, ModifyMethod));
	}
}

void FCharacterPropertyModifier::RemoveFromCharacter(AStandardModePlayerCharacter* Target) const
{
	if (AffectProperty == ECharacterProperty::CP_HealthPoint)
	{
		Target->SetMaxHealth(CalculateValue(Target->MaxBaseHealth, ModifyMethod, true));
	}
	else if (AffectProperty == ECharacterProperty::CP_StrengthPoint)
	{
		Target->SetMaxStrength(CalculateValue(Target->MaxStrength, ModifyMethod, true));
	}
}

float FCharacterPropertyModifier::CalculateValue(float OrgValue, EPropertyModifyMethod Method, bool bReverse) const
{
	switch(Method)
	{
	case EPropertyModifyMethod::PMM_Add:
		return bReverse ? OrgValue - ModifyValue : OrgValue + ModifyValue;
	case EPropertyModifyMethod::PMM_Subtract: 
		return bReverse ? OrgValue + ModifyValue : OrgValue - ModifyValue;
	case EPropertyModifyMethod::PMM_Multiply:
		return bReverse ? OrgValue / ModifyValue : OrgValue * ModifyValue;
	case EPropertyModifyMethod::PMM_Replace:
	default:
		return ModifyValue;
	}
}

void ACharacterPropertyBuff::ApplyBuff()
{
	Super::ApplyBuff();

	// Apply all modifiers to owner
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(TargetActor))
	{
		for (FCharacterPropertyModifier Modifier : PropertyModifiers)
		{
			Modifier.ApplyToCharacter(StandardModePlayerCharacter);
		}
	}
}


void ACharacterPropertyBuff::RemoveBuff()
{
	// Revert all modifiers
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(TargetActor))
	{
		for (FCharacterPropertyModifier Modifier : PropertyModifiers)
		{
			Modifier.RemoveFromCharacter(StandardModePlayerCharacter);
		}
	}

	Super::RemoveBuff();
}
