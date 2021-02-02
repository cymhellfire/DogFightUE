// Dog Fight Game Code By CYM.


#include "Card/Enhancements/InstructionPropertyEnhancement.h"

#include "Card/GameCardTypes.h"
#include "Card/Instructions/CardInstructionBase.h"

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
		UE_LOG(LogGameCards, Error, TEXT("CardEnhancement failed to apply due to invalid target instruction specified."));
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

FString UInstructionPropertyEnhancement::GetLevelModifierText() const
{
	if (PropertyLevelModifier > 0)
	{
		return FString::Printf(TEXT("+%d"), PropertyLevelModifier);
	}
	else if (PropertyLevelModifier < 0)
	{
		return FString::Printf(TEXT("%d"), PropertyLevelModifier);
	}

	return FString(TEXT("Invalid"));
}

FCardDescriptionItemInfo UInstructionPropertyEnhancement::GetDescriptionItemInfo_Implementation()
{
	UClass* DefaultWidgetClass = LoadObject<UClass>(nullptr, TEXT("/Game/DogFight/UI/InGame/CardDisplay/BP_CardDescriptionItemWidget.BP_CardDescriptionItemWidget_C"));
	if (!DefaultWidgetClass)
	{
		UE_LOG(LogInit, Error, TEXT("Failed to load default widget."));
	}
	
	FCardDescriptionItemInfo Result;
	Result.StringValue = EnhancementDescription;
	Result.LocalizeType = ECardDisplayInfoLocType::ILT_CardEnhance;
	Result.ItemWidgetClass = DefaultWidgetClass;

	TArray<FCardDisplayInfoArgument> InfoArguments;
	InfoArguments.Add(FCardDisplayInfoArgument{FString(TEXT("Default")), TargetCardInstruction->InstructionName, ECardDisplayInfoLocType::ILT_Card});
	InfoArguments.Add(FCardDisplayInfoArgument{FString(TEXT("Default")), GetLevelModifierText(), ECardDisplayInfoLocType::ILT_Raw, true});

	Result.Arguments = InfoArguments;

	return Result;
}

bool UInstructionPropertyEnhancement::ApplyEnhanceIfTypeMatch(FProperty* TestProperty)
{
	if (TestProperty == nullptr)
	{
		return false;
	}

	if (FStructProperty* StructProperty = CastField<FStructProperty>(TestProperty))
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
					UE_LOG(LogGameCards, Error, TEXT("Failed get FUpgradableProperty from target instruction."));
				}
			}
			else
			{
				UE_LOG(LogGameCards, Error, TEXT("FUpgradableProperty type mismatch."));
			}
			break;
		default: ;
		}
	}

	return false;
}
