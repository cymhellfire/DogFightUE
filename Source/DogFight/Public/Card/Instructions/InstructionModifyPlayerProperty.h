// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Card/Instructions/HandleTargetInstructionBase.h"
#include "Common/LocalizedString.h"

#include "InstructionModifyPlayerProperty.generated.h"

class AStandardPlayerState;

UENUM(BlueprintType)
enum class EModifiablePropertyName : uint8
{
	EMPN_CardGainPerRound		UMETA(DisplayName="Card Gain Per Round"),
	EMPN_CardUseCount			UMETA(DisplayName="Card Use Count"),
	EMPN_MaxCardCount			UMETA(DisplayName="Max Card Count"),
};

UENUM(BlueprintType)
enum class EPropertyModifyMethod : uint8
{
	EPMM_Addition		UMETA(DisplayName="Addition"),
	EPMM_Subtract		UMETA(DisplayName="Subtract"),
	EPMM_Replace		UMETA(DisplayName="Replace"),
};

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UInstructionModifyPlayerProperty : public UHandleTargetInstructionBase
{
	GENERATED_BODY()

protected:
	virtual bool HandleActorTarget(AActor* Target) override;

	void ModifyCardGainPerRound(AStandardPlayerState* PlayerState);
	void ModifyCardUseCount(AStandardPlayerState* PlayerState);
	void ModifyMaxCardCount(AStandardPlayerState* PlayerState);

	int32 CalculateResult(int32 OldValue);

	FText GetOperatorText();

protected:
	/** Property to Modify */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	EModifiablePropertyName PropertyName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	EPropertyModifyMethod ModifyMethod;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FString NewValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FLocalizedString FloatingText;
};
