#include "UnrealIntegration/UObject/AttributeModifierBasedObject.h"

#include "AttributeSystem.h"
#include "AttributeSystem/Attribute/AttributeBase.h"
#include "AttributeSystem/Modifier/AttributeModifier.h"
#include "AttributeSystem/Modifier/AttributeModifierBase.h"
#include "UnrealIntegration/Interface/AttributeCarrierInterface.h"
#include "UnrealIntegration/UObject/AttributeModifierDescObject.h"

UAttributeModifierDescObject* UAttributeModifierBasedObject::GetDescObject(UObject* Instigator)
{
	// Generate if no desc object yet
	if (DescObject == nullptr)
	{
		DescObject = GenerateDescObject(Instigator);
	}

	return DescObject;
}

UAttributeWrapperObjectBase* UAttributeModifierBasedObject::GetModifiedAttributeObject() const
{
	if (Modifier.IsValid())
	{
		if (auto ModifiedAttr = Modifier->GetModifiedAttribute())
		{
			return ModifiedAttr->GetWrapperObject();
		}
	}

	return nullptr;
}

bool UAttributeModifierBasedObject::SetModifierBoolFactor(bool InFactor)
{
	if (Modifier.IsValid() && Modifier->GetDataType() == ADT_Boolean)
	{
		if (auto BoolModifier = StaticCastSharedPtr<FAttributeModifierBoolean>(Modifier))
		{
			BoolModifier->SetFactor(InFactor);
			return true;
		}
	}

	return false;
}

bool UAttributeModifierBasedObject::SetModifierIntegerFactor(int32 InFactor)
{
	if (Modifier.IsValid() && Modifier->GetDataType() == ADT_Integer)
	{
		if (auto IntegerModifier = StaticCastSharedPtr<FAttributeModifierInteger>(Modifier))
		{
			IntegerModifier->SetFactor(InFactor);
			return true;
		}
	}

	return false;
}

bool UAttributeModifierBasedObject::SetModifierFloatFactor(float InFactor)
{
	if (Modifier.IsValid() && Modifier->GetDataType() == ADT_Float)
	{
		if (auto FloatModifier = StaticCastSharedPtr<FAttributeModifierFloat>(Modifier))
		{
			FloatModifier->SetFactor(InFactor);
			return true;
		}
	}

	return false;
}

UAttributeModifierDescObject* UAttributeModifierBasedObject::GenerateDescObject(UObject* Instigator)
{
	if (Instigator == nullptr)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("[AttributeModifierBasedObject] Desc object must have a valid owner."));
		return nullptr;
	}

	TSubclassOf<UAttributeModifierDescObject> DescClass = GetModifierDescClass();
	if (DescClass == nullptr)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("[AttributeModifierBasedObject] No valid desc object class."));
		return nullptr;
	}

	UAttributeModifierDescObject* NewDesc = NewObject<UAttributeModifierDescObject>(Instigator, DescClass, NAME_None, RF_Transient);

	// Initialize the description object
	InitializeDescObject(NewDesc);
	return NewDesc;
}

void UAttributeModifierBasedObject::OnModifierCreated(TSharedPtr<FAttributeModifierBase> InModifier)
{
	Modifier = InModifier;
}

TSubclassOf<UAttributeModifierDescObject> UAttributeModifierBasedObject::GetModifierDescClass()
{
	return UAttributeModifierDescObject::StaticClass();
}

void UAttributeModifierBasedObject::InitializeDescObject(UAttributeModifierDescObject* InDesc)
{
	InDesc->SetSourceString(GetName());
	InDesc->SetEffectString("No Effect");
}
