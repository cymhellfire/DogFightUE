#pragma once

#include "CoreMinimal.h"
#include "AttributeWrapperObject.generated.h"

class UAttributeModifierDescObject;

UCLASS()
class ATTRIBUTESYSTEM_API UAttributeWrapperBase : public UObject
{
	GENERATED_BODY()
public:
	// Enable networking
	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddDescObject(UAttributeModifierDescObject* InDescObject);

	void RemoveDescObject(UAttributeModifierDescObject* InDescObject);

protected:
	// Only replicate array content in this class since all description objects are replicated by outer actor.
	UPROPERTY(Replicated, Transient)
	TArray<UAttributeModifierDescObject*> AppliedModifierDesc;
};

UCLASS()
class ATTRIBUTESYSTEM_API UAttributeBooleanWrapper : public UAttributeWrapperBase
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(Replicated, Transient)
	bool BaseValue;

	UPROPERTY(Replicated, Transient)
	bool Value;
};
