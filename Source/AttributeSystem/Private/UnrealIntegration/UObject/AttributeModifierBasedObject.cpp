#include "UnrealIntegration/UObject/AttributeModifierBasedObject.h"

#include "AttributeSystem.h"
#include "UnrealIntegration/UObject/AttributeModifierDescObject.h"

bool UAttributeModifierBasedObject::CreateModifier(const FModifierCreateArgument& InArgument)
{
	return IAttributeModifierCarrierInterface::CreateModifier(InArgument);
}

UAttributeModifierDescObject* UAttributeModifierBasedObject::GetDescObject(UObject* Instigator)
{
	// Generate if no desc object yet
	if (DescObject == nullptr)
	{
		DescObject = GenerateDescObject(Instigator);
	}

	return DescObject;
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
