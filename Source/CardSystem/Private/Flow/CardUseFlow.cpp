// Dog Fight Game Code By CYM.


#include "Flow/CardUseFlow.h"

#include "Card/Card.h"
#include "Card/CardDescObject.h"
#include "Card/CardLogic.h"
#include "Common/CardSystemLog.h"
#include "Common/GlobalLog.h"

FCardUseFlowStep::FCardUseFlowStep(UCard* InCard, UCardUseFlow* InFlow)
	: UsingCard(InCard)
	, OwnerFlow(InFlow)
{
	// Get step name
	if (IsValid(InCard))
	{
		if (auto DescObject = InCard->GetDescObject())
		{
			StepName = FString::Printf(TEXT("Using %s"), *DescObject->GetCardName().ToString());
		}
	}
}

void FCardUseFlowStep::Execute(const FCardUseFlowStep* Against)
{
	// Get executing card
	auto CurCard = GetCard();
	if (!IsValid(CurCard))
	{
		Finish();
		return;
	}

	// Try to get target card
	UCard* TargetCard = nullptr;
	if (Against != nullptr)
	{
		TargetCard = Against->GetCard();
	}

	// Execute card against target
	if (IsValid(TargetCard))
	{
		// Update step name
		UpdateStepNameAgainstCard(TargetCard);

		if (auto CardLogic = CurCard->GetCardLogic())
		{
			// Construct target info
			FAcquiredTargetInfo NewTarget;
			NewTarget.Type = ECardTargetType::CTT_Card;
			NewTarget.CardPtr = TargetCard;

			CardLogic->PushTargetInfo(NewTarget);
		}
	}

	DFLog(LogCardSystem, TEXT("%s"), *StepName);

	if (OwnerFlow.IsValid())
	{
		OwnerFlow->ExecuteCard(CurCard);
	}
}

void FCardUseFlowStep::UpdateStepNameAgainstCard(UCard* InCard)
{
	if (IsValid(InCard))
	{
		if (auto DescObject = InCard->GetDescObject())
		{
			StepName = FString::Printf(TEXT("%s against %s"), *StepName, *DescObject->GetCardName().ToString());
		}
	}
}

void FCardUseFlowStep::NotifyCardFinished(UCard* InCard)
{
	// Validate again
	if (InCard == GetCard())
	{
		Finish();
	}
}

void FCardUseFlowStep::Finish()
{
	DFLog(LogCardSystem, TEXT("%s"), *StepName);

	// Trigger callback
	OnCardUseFlowStepComplete.ExecuteIfBound(this);
}

UCardUseFlow::UCardUseFlow()
{
	
}

UCardUseFlow::~UCardUseFlow()
{
	StepStack.Empty();
}

void UCardUseFlow::SetInitialCard(UCard* InCard)
{
	if (ensureAlwaysMsgf(!HasInitialStep(), TEXT("UCardUseFlow cannot initialize twice.")))
	{
		PushStep(InCard);
	}
}

bool UCardUseFlow::HasInitialStep()
{
	// Validate all steps before check
	ValidateAllSteps();

	// Any validate step considered as initial step
	return StepStack.Num() > 0;
}

void UCardUseFlow::PushAgainstStep(UCard* InCard)
{
	if (!IsValid(InCard))
	{
		DFLogE(LogCardSystem, TEXT("Invalid card detected."))
		return;
	}

	PushStep(InCard);
}

void UCardUseFlow::Execute()
{
	// Validate all steps before execute
	ValidateAllSteps();

	if (!HasInitialStep())
	{
		DFLogE(LogCardSystem, TEXT("No available step."))
		return;
	}

	ProcessNextStep();
}

void UCardUseFlow::ValidateAllSteps()
{
	for (int32 i = StepStack.Num() - 1; i >= 0; i--)
	{
		// Remove step that is invalid
		if (!StepStack[i].IsValid())
		{
			StepStack.RemoveAt(i);
		}
	}
}

void UCardUseFlow::Finish()
{
	DFLog(LogCardSystem, TEXT("CardUseFlow finished"));

	// Trigger callback
	OnCardUseFlowComplete.ExecuteIfBound(this);
}

void UCardUseFlow::PushStep(UCard* InCard)
{
	TUniquePtr<FCardUseFlowStep> NewStep = MakeUnique<FCardUseFlowStep>(InCard, this);
	StepStack.Emplace(Forward<TUniquePtr<FCardUseFlowStep>>(NewStep));
}

void UCardUseFlow::ProcessNextStep()
{
	if (StepStack.Num() == 0)
	{
		Finish();
		return;
	}

	// Execute steps from stack top to bottom
	auto& CurStep = StepStack.Last();
	CurStep->OnCardUseFlowStepComplete.BindUObject(this, &UCardUseFlow::OnStepFinished);
	// If stack length larger than 1, use previous as the target
	if (StepStack.Num() > 1)
	{
		CurStep->Execute(StepStack.Last(1).Get());
	}
	else
	{
		CurStep->Execute();
	}
}

void UCardUseFlow::OnStepFinished(FCardUseFlowStep* InStep)
{
	if (InStep)
	{
		InStep->OnCardUseFlowStepComplete.Unbind();
	}

	if (ensureAlwaysMsgf(StepStack.Last().Get() == InStep, TEXT("Step on the stack top should be finished earlier.")))
	{
		// Clear the last step
		StepStack.Pop();

		// Continue the flow
		ProcessNextStep();
	}
}

void UCardUseFlow::ExecuteCard(UCard* InCard)
{
	if (!IsValid(InCard))
	{
		DFLogE(LogCardSystem, TEXT("Invalid card detected."))
		return;
	}

	InCard->OnCardExecutionFinished.AddDynamic(this, &UCardUseFlow::OnCardCompleted);
	InCard->Execute();
}

void UCardUseFlow::OnCardCompleted(ECardExecutionResult Result, UCard* InCard)
{
	if (!IsValid(InCard))
	{
		DFLogE(LogCardSystem, TEXT("Invalid card detected."))
		return;
	}

	// Unregister callback
	InCard->OnCardExecutionFinished.RemoveDynamic(this, &UCardUseFlow::OnCardCompleted);

	if (ensureAlwaysMsgf(HasInitialStep(), TEXT("Empty flow is invalid.")))
	{
		// Validate with step on the stack top
		if (auto CurCard = StepStack.Last()->GetCard())
		{
			if (ensureAlwaysMsgf(CurCard == InCard, TEXT("Listening card should always be the last one.")))
			{
				// Notify the step
				StepStack.Last()->NotifyCardFinished(InCard);
			}
		}
	}
}
