#pragma once

#include "CoreMinimal.h"
#include "UnrealIntegration/Marco/AttributeDataTypeMacro.h"
#include "AttributeWrapperObject.generated.h"

class UAttributeModifierDescObject;

UCLASS()
class ATTRIBUTESYSTEM_API UAttributeWrapperObjectBase : public UObject
{
	GENERATED_BODY()
public:
	// Enable networking
	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddDescObject(UAttributeModifierDescObject* InDescObject);

	void RemoveDescObject(UAttributeModifierDescObject* InDescObject);

	void SetAttributeName(FName InName);

	FName GetAttributeName() const
	{
		return AttributeName;
	}

	virtual FString ToString() const
	{
		return FString::Printf(TEXT("Attribute Name: %s"), *AttributeName.ToString());
	}

protected:
	UFUNCTION()
	void OnRep_AppliedModifierDesc(const TArray<UAttributeModifierDescObject*>& OldList);

public:
	DECLARE_MULTICAST_DELEGATE(FAttributeWrapperObjectDelegate);
	FAttributeWrapperObjectDelegate OnBaseValueChanged;
	FAttributeWrapperObjectDelegate OnValueChanged;

//protected:
	// Only replicate array content in this class since all description objects are replicated by outer actor.
	UPROPERTY(ReplicatedUsing=OnRep_AppliedModifierDesc, Transient)
	TArray<UAttributeModifierDescObject*> AppliedModifierDesc;

protected:
	UPROPERTY(Replicated, Transient)
	FName AttributeName;
};

UCLASS()
class ATTRIBUTESYSTEM_API UAttributeBooleanWrapperObject : public UAttributeWrapperObjectBase
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool GetBaseValue() const
	{
		return BaseValue;
	}

	void SetBaseValue(bool InValue);

	bool GetValue() const
	{
		return Value;
	}

	void SetValue(bool InValue);

	virtual FString ToString() const override
	{
		return FString::Printf(TEXT("%s, Value: %s/%s"), *Super::ToString(),
			BOOL_TO_STR(Value), BOOL_TO_STR(BaseValue));
	}

protected:
	UFUNCTION()
	void OnRep_BaseValue(bool OldValue);

	UFUNCTION()
	void OnRep_Value(bool OldValue);

protected:
	UPROPERTY(ReplicatedUsing=OnRep_BaseValue, Transient)
	bool BaseValue;

	UPROPERTY(ReplicatedUsing=OnRep_Value, Transient)
	bool Value;
};

UCLASS()
class ATTRIBUTESYSTEM_API UAttributeIntegerWrapperObject : public UAttributeWrapperObjectBase
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	int32 GetBaseValue() const
	{
		return BaseValue;
	}

	void SetBaseValue(int32 InValue);

	int32 GetValue() const
	{
		return Value;
	}

	void SetValue(int32 InValue);

	virtual FString ToString() const override
	{
		return FString::Printf(TEXT("%s, Value: %d/%d"), *Super::ToString(), Value, BaseValue);
	}

protected:
	UFUNCTION()
	void OnRep_BaseValue(int32 OldValue);

	UFUNCTION()
	void OnRep_Value(int32 OldValue);

protected:
	UPROPERTY(ReplicatedUsing=OnRep_BaseValue, Transient)
	int32 BaseValue;

	UPROPERTY(ReplicatedUsing=OnRep_Value, Transient)
	int32 Value;
};

UCLASS()
class ATTRIBUTESYSTEM_API UAttributeFloatWrapperObject : public UAttributeWrapperObjectBase
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	float GetBaseValue() const
	{
		return BaseValue;
	}

	void SetBaseValue(float InValue);

	float GetValue() const
	{
		return Value;
	}

	void SetValue(float InValue);

	virtual FString ToString() const override
	{
		return FString::Printf(TEXT("%s, Value: %.3f/%.3f"), *Super::ToString(), Value, BaseValue);
	}

protected:
	UFUNCTION()
	void OnRep_BaseValue(float OldValue);

	UFUNCTION()
	void OnRep_Value(float OldValue);

protected:
	UPROPERTY(ReplicatedUsing=OnRep_BaseValue, Transient)
	float BaseValue;

	UPROPERTY(ReplicatedUsing=OnRep_Value, Transient)
	float Value;
};
