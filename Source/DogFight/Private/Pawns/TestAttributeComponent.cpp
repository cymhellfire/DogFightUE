#include "Pawns/TestAttributeComponent.h"

#include "AttributeSystem/Attribute/Attribute.h"
#include "AttributeSystem/Attribute/AttributeBase.h"
#include "Net/UnrealNetwork.h"

UTestAttributeComponent::UTestAttributeComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTestAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(UTestAttributeComponent, TestInteger, SharedParams);
}

void UTestAttributeComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// test code
	const FName NewAttrName = "TestInteger";
	FAttributeCreateArgument NewAttributeArgument;
	NewAttributeArgument.AttrName = NewAttrName;
	NewAttributeArgument.DataType = ADT_Integer;
	NewAttributeArgument.InitIntegerValue = 100;

	if (AddAttribute(NewAttributeArgument))
	{
		auto NewAttribute = GetAttribute(NewAttrName);
		NewAttribute->OnValueChanged.AddUObject(this, &UTestAttributeComponent::OnTestAttributeValueChanged);
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

void UTestAttributeComponent::SetTestInteger(int32 NewValue)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UTestAttributeComponent, TestInteger, this);
	TestInteger = NewValue;
}

void UTestAttributeComponent::OnTestAttributeValueChanged(TSharedPtr<FAttributeBase> Attribute)
{
	// Sync value from attribute system
	if (auto ConvertAttr = StaticCastSharedPtr<FAttributeInteger>(Attribute))
	{
		SetTestInteger(ConvertAttr->GetValue());
	}
}

void UTestAttributeComponent::OnRep_TestInteger(int32 OldInteger)
{
	UE_LOG(LogTemp, Log, TEXT("[TestAttribute] OnRep Old: %d New: %d"), OldInteger, TestInteger);
}
