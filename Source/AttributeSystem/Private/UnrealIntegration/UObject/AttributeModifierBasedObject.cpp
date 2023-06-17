#include "UnrealIntegration/UObject/AttributeModifierBasedObject.h"

#include "AttributeSystem.h"
#include "AttributeSystem/Attribute/AttributeBase.h"
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
