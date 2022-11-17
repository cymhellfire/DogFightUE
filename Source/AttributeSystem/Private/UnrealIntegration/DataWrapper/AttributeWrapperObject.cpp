#include "UnrealIntegration/DataWrapper/AttributeWrapperObject.h"
#include "Net/UnrealNetwork.h"

#define MAKE_SHARED_PARAMS(ParamName) \
	FDoRepLifetimeParams ParamName; \
	ParamName.bIsPushBased = true;

void UAttributeWrapperBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	MAKE_SHARED_PARAMS(SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeWrapperBase, AppliedModifierDesc, SharedParams);
}

void UAttributeWrapperBase::AddDescObject(UAttributeModifierDescObject* InDescObject)
{
	if (!InDescObject)
		return;

	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeWrapperBase, AppliedModifierDesc, this);
	AppliedModifierDesc.AddUnique(InDescObject);
}

void UAttributeWrapperBase::RemoveDescObject(UAttributeModifierDescObject* InDescObject)
{
	if (!InDescObject)
		return;

	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeWrapperBase, AppliedModifierDesc, this);
	AppliedModifierDesc.Remove(InDescObject);
}

void UAttributeBooleanWrapper::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	MAKE_SHARED_PARAMS(SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeBooleanWrapper, BaseValue, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeBooleanWrapper, Value, SharedParams);
}
