// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CardUseFlow.generated.h"

class UCard;
class UCardUseFlow;
class FCardUseFlowStep;

DECLARE_DELEGATE_OneParam(FOnCardUseFlowStepCompleteEvent, FCardUseFlowStep*);

class FCardUseFlowStep
{
	friend UCardUseFlow;
public:
	FCardUseFlowStep(UCard* InCard, UCardUseFlow* InFlow);

	void Execute(const FCardUseFlowStep* Against = nullptr);

	void Finish();

	UCard* GetCard() const
	{
		return UsingCard.Get();
	}

private:
	void UpdateStepNameAgainstCard(UCard* InCard);

	void NotifyCardFinished(UCard* InCard);

public:
	FOnCardUseFlowStepCompleteEvent OnCardUseFlowStepComplete;

private:
	FString StepName;

	TWeakObjectPtr<UCard> UsingCard;
	TWeakObjectPtr<UCardUseFlow> OwnerFlow;
};

DECLARE_DELEGATE_OneParam(FOnCardUseFlowCompleteEvent, UCardUseFlow*);

/**
 * Flow manages using order of card and all cards against it.
 */
UCLASS()
class CARDSYSTEM_API UCardUseFlow : public UObject
{
	GENERATED_BODY()

	friend FCardUseFlowStep;
public:
	UCardUseFlow();

	~UCardUseFlow();

	/**
	 * Set specified card as the initial step of the flow.
	 * @param InCard Card as the initial step.
	 */
	UFUNCTION(BlueprintCallable, Category = CardUseFlow)
	void SetInitialCard(UCard* InCard);

	/**
	 * Check if this flow has initial step.
	 * @return Whether this flow has initial step.
	 */
	UFUNCTION(BlueprintCallable, Category = CardUseFlow)
	bool HasInitialStep();

	/**
	 * Push given card into stack and use it against last step.
	 * @param InCard Card using against step on top of stack.
	 */
	UFUNCTION(BlueprintCallable, Category = CardUseFlow)
	void PushAgainstStep(UCard* InCard);

	UFUNCTION(BlueprintCallable, Category = CardUseFlow)
	void Execute();

private:
	void ValidateAllSteps();

	void Finish();

	void PushStep(UCard* InCard);

	void ProcessNextStep();

	void OnStepFinished(FCardUseFlowStep* InStep);

	void ExecuteCard(UCard* InCard);

	UFUNCTION()
	void OnCardCompleted(ECardExecutionResult Result, UCard* InCard);

public:
	FOnCardUseFlowCompleteEvent OnCardUseFlowComplete;

private:
	TArray<TUniquePtr<FCardUseFlowStep>> StepStack;
};
