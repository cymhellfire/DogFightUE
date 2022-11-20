#include "Pawns/TestAttributeComponent.h"

#include "AttributeSystem/Attribute/Attribute.h"
#include "AttributeSystem/Attribute/AttributeBase.h"
#include "Net/UnrealNetwork.h"
#include "Pawns/TestAttributeModifier.h"
#include "UnrealIntegration/UObject/AttributeModifierDescObject.h"

UTestAttributeComponent::UTestAttributeComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTestAttributeComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UTestAttributeComponent::InitializeAttributes()
{
	Super::InitializeAttributes();

	// test code
	const FName AttrName = "TestInteger";
	FAttributeCreateArgument CreateArgument;
	CreateArgument.AttrName = AttrName;
	CreateArgument.DataType = ADT_Integer;
	CreateArgument.InitIntegerValue = 100;

	if (AddAttribute(CreateArgument))
	{
		if (auto TestAttr = GetAttribute(AttrName))
		{
			TestAttr->OnValueChanged.AddUObject(this, &UTestAttributeComponent::OnTestAttributeValueChanged);
		}
	}
}

void UTestAttributeComponent::ChangeTestAttribute(int32 NewValue)
{
	auto TestAttr = GetAttribute("TestInteger");
	if (auto ConvertAttr = StaticCastSharedPtr<FAttributeInteger>(TestAttr))
	{
		ConvertAttr->SetRawValue(NewValue);
	}
}

void UTestAttributeComponent::AddTestModifier()
{
	// Create test modifier
	auto NewModifier = NewObject<UTestAttributeModifier>(this, NAME_None, RF_Transient);
	AddModifierObject(NewModifier);
}

void UTestAttributeComponent::RemoveTestModifier()
{
	// Remove from list head
	if (ModifierObjectList.Num() > 0)
	{
		RemoveModifierObject(ModifierObjectList[0]);
	}
}

void UTestAttributeComponent::OnTestAttributeValueChanged(TSharedPtr<FAttributeBase> Attribute)
{
	// Sync value from attribute system
	if (auto ConvertAttr = StaticCastSharedPtr<FAttributeInteger>(Attribute))
	{
		auto Wrapper = GetIntegerAttributeWrapperByName(Attribute->GetName());
		if (Wrapper)
		{
			UE_LOG(LogTemp, Log, TEXT("[TestComponent] Value changed: %s"), *Wrapper->ToString());
		}
	}
}
