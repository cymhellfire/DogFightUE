#pragma once

#include "CoreMinimal.h"
#include "AttributeModifierDescObject.generated.h"

UCLASS(BlueprintType)
class ATTRIBUTESYSTEM_API UAttributeModifierDescObject : public UObject
{
	GENERATED_BODY()
public:

	// Enable network support
	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void SetSourceString(FString InStr);
	virtual void SetEffectString(FString InStr);

	UFUNCTION(BlueprintCallable, Category="AttributeModifierDescObject")
	FString GetSourceString() const { return Source; }
	UFUNCTION(BlueprintCallable, Category="AttributeModifierDescObject")
	FString GetEffectString() const { return Effect; }

	UFUNCTION(BlueprintCallable, Category="AttributeModifierDescObject")
	FString ToString() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AttribtueModifierDesc", Replicated)
	FString Source;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AttributeModifierDesc", Replicated)
	FString Effect;
};
