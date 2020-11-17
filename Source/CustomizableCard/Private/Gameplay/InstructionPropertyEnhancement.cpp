// Dog Fight Game Code By CYM.


#include "InstructionPropertyEnhancement.h"

#include "CustomizableCard.h"
#include "CardInstructionBase.h"

void UInstructionPropertyEnhancement::SetTargetInstruction(UCardInstructionBase* NewInstruction)
{
	if (NewInstruction != nullptr)
	{
		TargetCardInstruction = NewInstruction;
	}
}

void UInstructionPropertyEnhancement::ApplyEnhancement()
{
	if (TargetCardInstruction == nullptr)
	{
		UE_LOG(LogCustomizableCard, Error, TEXT("CardEnhancement failed to apply due to invalid target instruction specified."));
		return;
	}

	// Iterate the property list of target instruction to find target
	FProperty* TargetProperty = nullptr;
	UClass* InstructionClass = TargetCardInstruction->GetClass();
	for (TFieldIterator<FProperty> PropertyItr(InstructionClass); PropertyItr; ++PropertyItr)
	{
		FProperty* CurrentProperty = *PropertyItr;
		if (CurrentProperty->GetName() == TargetPropertyName)
		{
			TargetProperty = CurrentProperty;
			break;
		}
	}

	// Apply enhancement to target property
	ApplyEnhanceIfTypeMatch(TargetProperty);
}

bool UInstructionPropertyEnhancement::ApplyEnhanceIfTypeMatch(FProperty* TestProperty)
{
	if (TestProperty == nullptr)
	{
		return false;
	}

	if (FStructProperty* StructProperty = Cast<FStructProperty>(TestProperty))
	{
		switch(TargetPropertyType)
		{
		case EUpgradablePropertyType::Integer:
			if (StructProperty->Struct == TBaseStructure<FUpgradableIntProperty>::Get())
			{
				if (FUpgradableIntProperty* StructAddress = TestProperty->ContainerPtrToValuePtr<FUpgradableIntProperty>(TargetCardInstruction))
				{
					StructAddress->SetLevel(StructAddress->GetCurrentLevel() + PropertyLevelModifier);

					return true;
				}
				else
				{
					UE_LOG(LogCustomizableCard, Error, TEXT("Failed get FUpgradableProperty from target instruction."));
				}
			}
			else
			{
				UE_LOG(LogCustomizableCard, Error, TEXT("FUpgradableProperty type mismatch."));
			}
			break;
		default: ;
		}
	}

	return false;
}
