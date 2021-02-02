// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "Card/CardEnhancement.h"
#include "InstructionPropertyEnhancement.generated.h"

class UCardInstructionBase;

UENUM(BlueprintType)
enum class EUpgradablePropertyType : uint8
{
	Integer,
};

/**
 * 
 */
UCLASS(Blueprintable)
class DOGFIGHT_API UInstructionPropertyEnhancement : public UCardEnhancement
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="CardEnhancement")
	void SetTargetInstruction(UCardInstructionBase* NewInstruction);

	UFUNCTION(BlueprintCallable, Category="CardEnhancement")
	virtual void ApplyEnhancement() override;

	UFUNCTION(BlueprintCallable, Category="CardEnhancement")
	UCardInstructionBase* GetTargetInstruction() const { return TargetCardInstruction; }

	UFUNCTION(BlueprintCallable, Category="CardEnhancement")
	FString GetLevelModifierText() const;

	virtual FCardDescriptionItemInfo GetDescriptionItemInfo_Implementation() override;
protected:

	bool ApplyEnhanceIfTypeMatch(FProperty* TestProperty);

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CardEnhancement")
	FString TargetPropertyName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CardEnhancement")
	EUpgradablePropertyType TargetPropertyType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CardEnhancement")
	int32 PropertyLevelModifier;

protected:

	UCardInstructionBase* TargetCardInstruction;
};
