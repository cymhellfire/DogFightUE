// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "Card/GameCardTypes.h"
#include "Card/Instructions/CardInstructionBase.h"
#include "GameFramework/Actor.h"
#include "CardBase.generated.h"

class UCardEnhancement;

/**
 * This is the very basic class for all card classes.
 */
UCLASS(Blueprintable)
class DOGFIGHT_API ACardBase : public AActor
{
	GENERATED_BODY()
	
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCardDelegateNoArgument);

	/** This delegate triggered before card executing. */
	FCardDelegateNoArgument OnCardStartExecuting;
	/** This delegate triggered when card is finished. */
	FCardDelegateNoArgument OnCardFinished;
	/** This delegate triggered when card is cancelled. */
	FCardDelegateNoArgument OnCardCancelled;

	// Sets default values for this actor's properties
	ACardBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Card")
	/** Start using this card. All the phases of this card will be executing in order. */
	virtual void Use();

	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Card")
	void AddInstruction(UCardInstructionBase* NewInstruction);

	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Card")
	void RemoveInstruction(UCardInstructionBase* TargetInstruction);

	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Card")
	void ClearInstructions();

	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Card")
	void AddCardEnhancement(UCardEnhancement* NewCardEnhancement);

	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Card")
	void RemoveCardEnhancement(UCardEnhancement* TargetCardEnhancement);

	/**
	 * Push a target information to list for other instruction usage.
	 */
	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Card")
	void PushTargetInfo(FCardInstructionTargetInfo TargetInfo);

	/**
	 * Get the last target information pushed in and remove it from the list.
	 */
	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Card")
	FCardInstructionTargetInfo PopTargetInfo();

	/**
	 * Get the target information at the specified index.
	 */
	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Card")
	FCardInstructionTargetInfo GetTargetInfo(int32 Index);

	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Card")
	int32 GetTargetInfoCount() const { return TargetInfoList.Num(); }

	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Card")
	FORCEINLINE AController* GetOwnerPlayerController() const { return OwnerPlayerController; }

	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Card")
	void SetOwnerPlayerController(AController* PlayerController)
	{
		OwnerPlayerController = PlayerController;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="CustomizableCard|Card")
	FCardInstanceDisplayInfo GetCardDisplayInfo();

	/** Check if this card matches all given category flags. */
	FORCEINLINE bool IsCardMatchCategoryFlags(int32 CategoryFlags) const
	{
		return TEST_MULTI_CARD_CATEGORY(CardCategories, CategoryFlags);
	}

	/** Increase the ExecutingInstructionCount by 1. */
	void IncreaseExecutingInstructionCounter();

	/** Decrease the ExecutingInstructionCount by 1. */
	void DecreaseExecutingInstructionCounter();

	/** Add a new instruction which is not directly used by card. */
	void AddExtraCardInstruction(UCardInstructionBase* NewInstruction);

	ECardUseMethod GetCardUseMethod() const { return CardUseMethod; }

protected:
	/** Do prepare operations before card executing phase. */
	virtual void Prepare() {};

	/** The execute phase of this card. */
	virtual void Execute();

	/** Do finalize operations when card is finished. */
	virtual void Finalize();

	/**
	 * Execute next CardInstruction.
	 * @return True if this instruction is asynchronous type.
	 */
	virtual bool ExecuteNextInstruction();

	UFUNCTION()
	virtual void OnInstructionFinished();

	UFUNCTION()
	void OnCardFinalizeTimerExpired();

	UFUNCTION(BlueprintCallable, Category="CustomizableCard")
	FUpgradablePropertyDisplayInfo GetIntPropertyDisplayInfo(const FUpgradableIntProperty& Property) const;

	UFUNCTION(BlueprintCallable, Category="CustomizableCard")
	FUpgradablePropertyDisplayInfo GetFloatPropertyDisplayInfo(const FUpgradableFloatProperty& Property) const;

	UFUNCTION(BlueprintCallable, Category="CustomizableCard")
	int32 GetUpgradableIntPropertyValue(const FUpgradableIntProperty& Property) const;

	UFUNCTION(BlueprintCallable, Category="CustomizableCard")
	FString GetUpgradableFloatPropertyValue(const FUpgradableFloatProperty& Property) const;

public:

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CustomizableCard")
	FString CardName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CustomizableCard")
	FString CardDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CustomizableCard")
	TArray<UCardInstructionBase*> InstructionQueue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CustomizableCard")
	TArray<UCardEnhancement*> CardEnhancements;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CustomizableCard")
	TArray<FCardInstructionTargetInfo> TargetInfoList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CustomizableCard", Meta = (Bitmask, BitmaskEnum = "ECardCategoryFlags"))
	int32 CardCategories;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CustomizableCard")
	ECardUseMethod CardUseMethod;

	/**
	 * Delay between last instruction finished and card finalize.
	 * Note: Use this delay to ensure the Game Mode State Machine can switch to pending Game Phase before OnCardFinished
	 *       delegate invoked.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CustomizableCard")
	float CardFinishDelay;

	int32 CurrentInstructionIndex;

	/** Counter for executing instructions. */
	int32 ExecutingInstructionCount;

	AController* OwnerPlayerController;

	FTimerHandle CardFinalizeTimerHandle;

	/** Array of instruction are not directly executed by card. (AnimNotify_Delegate owning instruction and etc.) */
	UPROPERTY()
	TArray<UCardInstructionBase*> ExtraInstructions;
};
