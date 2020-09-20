// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "CardInstructionBase.h"
#include "GameFramework/Actor.h"
#include "CustomizableCardTypes.h"
#include "CardBase.generated.h"

/**
 * This is the very basic class for all card classes.
 */
UCLASS()
class CUSTOMIZABLECARD_API ACardBase : public AActor
{
	GENERATED_BODY()
	
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCardStartExecuting);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCardFinished);

	/** This delegate triggered before card executing. */
	FOnCardStartExecuting OnCardStartExecuting;
	/** This delegate triggered when card is finished. */
	FOnCardFinished OnCardFinished;

	// Sets default values for this actor's properties
	ACardBase();

	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Card")
	/** Start using this card. All the phases of this card will be executing in order. */
	virtual void Use();

	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Card")
	void AddInstruction(UCardInstructionBase* NewInstruction);

	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Card")
	void RemoveInstruction(UCardInstructionBase* TargetInstruction);

	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Card")
	void ClearInstructions();

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
	FORCEINLINE APlayerController* GetOwnerPlayerController() const { return OwnerPlayerController; }

	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Card")
	void SetOwnerPlayerController(APlayerController* PlayerController)
	{
		OwnerPlayerController = PlayerController;
	}

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
	virtual void OnInstructionFinished(UCardInstructionBase* InstructionBase);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CustomizableCard")
	TArray<UCardInstructionBase*> InstructionQueue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CustomizableCard")
	TArray<FCardInstructionTargetInfo> TargetInfoList;

	int32 CurrentInstructionIndex;

	/** Counter for executing instructions. */
	int32 ExecutingInstructionCount;

	APlayerController* OwnerPlayerController;
};
