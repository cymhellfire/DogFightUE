#pragma once

#include "CoreMinimal.h"
#include "UnrealIntegration/Marco/AttributeDataTypeMacro.h"
#include "AttributeWrapperObject.generated.h"

class FAttributeBase;
class FAttributeBoolean;
class FAttributeInteger;
class FAttributeFloat;
class UAttributeModifierDescObject;
class IAttributeCarrierInterface;
class FAttributeWrapperObjectHelper;

UCLASS()
class ATTRIBUTESYSTEM_API UAttributeWrapperObjectBase : public UObject
{
	GENERATED_BODY()
	friend FAttributeWrapperObjectHelper;
	friend IAttributeCarrierInterface;
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

	ENetRole GetNetRole() const
	{
		if (auto OuterActor = GetTypedOuter<AActor>())
		{
			return OuterActor->GetLocalRole();
		}
		return ROLE_None;
	}

public:
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
	friend FAttributeWrapperObjectHelper;
	friend IAttributeCarrierInterface;
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool GetBaseValue() const
	{
		return BaseValue;
	}

	bool GetValue() const
	{
		return Value;
	}

	virtual FString ToString() const override
	{
		return FString::Printf(TEXT("%s, Value: %s/%s"), *Super::ToString(),
			BOOL_TO_STR(Value), BOOL_TO_STR(BaseValue));
	}

protected:
	void SetBaseValue(bool InValue);
	void SetValue(bool InValue);

	UFUNCTION()
	void OnRep_BaseValue(bool OldValue);

	UFUNCTION()
	void OnRep_Value(bool OldValue);

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FAttributeBooleanWrapperObjectValueDelegate, UAttributeBooleanWrapperObject*, bool);
	FAttributeBooleanWrapperObjectValueDelegate OnBaseValueChanged;
	FAttributeBooleanWrapperObjectValueDelegate OnValueChanged;

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

	friend FAttributeWrapperObjectHelper;
	friend IAttributeCarrierInterface;
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	int32 GetBaseValue() const
	{
		return BaseValue;
	}

	int32 GetValue() const
	{
		return Value;
	}

	virtual FString ToString() const override
	{
		return FString::Printf(TEXT("%s, Value: %d/%d"), *Super::ToString(), Value, BaseValue);
	}

protected:
	void SetBaseValue(int32 InValue);
	void SetValue(int32 InValue);

	UFUNCTION()
	void OnRep_BaseValue(int32 OldValue);

	UFUNCTION()
	void OnRep_Value(int32 OldValue);

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FAttributeIntegerWrapperObjectValueDelegate, UAttributeIntegerWrapperObject*, int32);
	FAttributeIntegerWrapperObjectValueDelegate OnBaseValueChanged;
	FAttributeIntegerWrapperObjectValueDelegate OnValueChanged;

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
	friend FAttributeWrapperObjectHelper;
	friend IAttributeCarrierInterface;
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	float GetBaseValue() const
	{
		return BaseValue;
	}

	float GetValue() const
	{
		return Value;
	}

	virtual FString ToString() const override
	{
		return FString::Printf(TEXT("%s, Value: %.3f/%.3f"), *Super::ToString(), Value, BaseValue);
	}

protected:
	void SetBaseValue(float InValue);
	void SetValue(float InValue);

	UFUNCTION()
	void OnRep_BaseValue(float OldValue);

	UFUNCTION()
	void OnRep_Value(float OldValue);

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FAttributeFloatWrapperObjectValueDelegate, UAttributeFloatWrapperObject*, float);
	FAttributeFloatWrapperObjectValueDelegate OnBaseValueChanged;
	FAttributeFloatWrapperObjectValueDelegate OnValueChanged;

protected:
	UPROPERTY(ReplicatedUsing=OnRep_BaseValue, Transient)
	float BaseValue;

	UPROPERTY(ReplicatedUsing=OnRep_Value, Transient)
	float Value;
};

class ATTRIBUTESYSTEM_API FAttributeWrapperObjectHelper
{
public:
	/**
	 * @brief Helper function of creating new wrapper object from given boolean attribute.
	 * @param Instigator		The instigator of creating.
	 * @param InAttribute		Attribute that create wrapper object with.
	 * @param InCallback		Callback function which is triggered every time the attribute value has changed.
	 * @return New created wrapper object.
	 */
	static UAttributeBooleanWrapperObject* CreateWrapperObjectForBooleanAttribute(UObject* Instigator, TSharedPtr<FAttributeBoolean> InAttribute,
		const TFunction<void(TSharedPtr<FAttributeBase>)>& InCallback);

	/**
	 * @brief Helper function of creating new wrapper object from given integer attribute.
	 * @param Instigator		The instigator of creating.
	 * @param InAttribute		Attribute that create wrapper object with.
	 * @param InCallback		Callback function which is triggered every time the attribute value has changed.
	 * @return New created wrapper object.
	 */
	static UAttributeIntegerWrapperObject* CreateWrapperObjectForIntegerAttribute(UObject* Instigator, TSharedPtr<FAttributeInteger> InAttribute,
		const TFunction<void(TSharedPtr<FAttributeBase>)>& InCallback);

	/**
	 * @brief Helper function of creating new wrapper object from given float attribute.
	 * @param Instigator		The instigator of creating.
	 * @param InAttribute		Attribute that create wrapper object with.
	 * @param InCallback		Callback function which is triggered every time the attribute value has changed.
	 * @return New created wrapper object.
	 */
	static UAttributeFloatWrapperObject* CreateWrapperObjectForFloatAttribute(UObject* Instigator, TSharedPtr<FAttributeFloat> InAttribute,
		const TFunction<void(TSharedPtr<FAttributeBase>)>& InCallback);
};
