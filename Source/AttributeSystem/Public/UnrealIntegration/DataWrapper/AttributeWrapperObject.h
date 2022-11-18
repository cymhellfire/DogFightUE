#pragma once

#include "CoreMinimal.h"
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

public:
	DECLARE_MULTICAST_DELEGATE(FAttributeWrapperObjectDelegate);
	FAttributeWrapperObjectDelegate OnBaseValueChanged;
	FAttributeWrapperObjectDelegate OnValueChanged;

//protected:
	// Only replicate array content in this class since all description objects are replicated by outer actor.
	UPROPERTY(Replicated, Transient)
	TArray<UAttributeModifierDescObject*> AppliedModifierDesc;
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

protected:
	UFUNCTION()
	void OnRep_BaseValue();

	UFUNCTION()
	void OnRep_Value();

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

protected:
	UFUNCTION()
	void OnRep_BaseValue();

	UFUNCTION()
	void OnRep_Value();

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

protected:
	UFUNCTION()
	void OnRep_BaseValue();

	UFUNCTION()
	void OnRep_Value();

protected:
	UPROPERTY(ReplicatedUsing=OnRep_BaseValue, Transient)
	float BaseValue;

	UPROPERTY(ReplicatedUsing=OnRep_Value, Transient)
	float Value;
};
