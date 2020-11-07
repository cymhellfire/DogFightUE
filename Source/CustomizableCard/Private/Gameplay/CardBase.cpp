// Dog Fight Game Code By CYM.


#include "CardBase.h"
#include "CardInstructionBase.h"
#include "CustomizableCard.h"
#include "InstructionWait.h"

// Sets default values
ACardBase::ACardBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Turn off tick
	PrimaryActorTick.bCanEverTick = false;
}

void ACardBase::Use()
{
	Prepare();

	Execute();
}

void ACardBase::AddInstruction(UCardInstructionBase* NewInstruction)
{
	if (InstructionQueue.Contains(NewInstruction))
		return;

	InstructionQueue.Add(NewInstruction);
	NewInstruction->SetOwnerCard(this);
}

void ACardBase::RemoveInstruction(UCardInstructionBase* TargetInstruction)
{
	if (!InstructionQueue.Contains(TargetInstruction))
	{
		return;
	}

	TargetInstruction->SetOwnerCard(nullptr);
	InstructionQueue.Remove(TargetInstruction);
}

void ACardBase::ClearInstructions()
{
	if (InstructionQueue.Num() > 0)
	{
		for (int i = InstructionQueue.Num() - 1; i >= 0; --i)
		{
			UCardInstructionBase* Instruction = InstructionQueue[i];
			InstructionQueue.Remove(Instruction);
			Instruction->ConditionalBeginDestroy();
		}
	}
}

void ACardBase::PushTargetInfo(FCardInstructionTargetInfo TargetInfo)
{
	TargetInfoList.Push(TargetInfo);
}

FCardInstructionTargetInfo ACardBase::PopTargetInfo()
{
	return TargetInfoList.Pop();
}

FCardInstructionTargetInfo ACardBase::GetTargetInfo(int32 Index)
{
	if (Index >= TargetInfoList.Num())
	{
		return FCardInstructionTargetInfo();
	}

	return TargetInfoList[Index];
}

FCardInstanceDisplayInfo ACardBase::GetCardDisplayInfo_Implementation()
{
	FCardInstanceDisplayInfo Result;
	Result.CardName = CardName;
	Result.CardDescription = CardDescription;

	return Result;
}

void ACardBase::Execute()
{
	// Broadcast delegate
	OnCardStartExecuting.Broadcast();

	// Reset the instruction index
	CurrentInstructionIndex = 0;

	/**
	 * Use loop to iterate the instruction queue.
	 * Asynchronous instructions will be executed in one frame.
	 */
	while(CurrentInstructionIndex < InstructionQueue.Num())
	{
		// If current instruction is synchronous type, we need delay other instructions until it finished.
		if (!ExecuteNextInstruction())
		{
			break;
		}
	}
}

void ACardBase::Finalize()
{
	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("Card %s finished."), *GetName()));
	
	// Broadcast delegate
	OnCardFinished.Broadcast();

	// Destroy this card after finished
	// TODO: Maybe cards and instructions can be pooled
	ClearInstructions();
	Destroy();
}

bool ACardBase::ExecuteNextInstruction()
{
	bool bAsynchronousFlag = false;
	if (CurrentInstructionIndex < InstructionQueue.Num())
	{
		UCardInstructionBase* CardInstruction = InstructionQueue[CurrentInstructionIndex];
		if (IsValid(CardInstruction))
		{
			// Register callback for instruction
			CardInstruction->OnInstructionFinished.AddDynamic(this, &ACardBase::OnInstructionFinished);

			// Check if this instruction is asynchronous type
			bAsynchronousFlag = CardInstruction->ExecuteType == ECardInstructionExecuteType::Asynchronous;

			// Execute the instruction
			CardInstruction->BeginExecute();

			// Increase counter
			ExecutingInstructionCount++;
		}
		else
		{
			UE_LOG(LogCustomizableCard, Error, TEXT("Card [%s] has an invalid instruction"), *GetName());
		}
		
		// Increase the index
		CurrentInstructionIndex++;
	}

	return bAsynchronousFlag;
}

void ACardBase::OnInstructionFinished(UCardInstructionBase* InstructionBase)
{
	// Remove the delegate callback after triggered
	InstructionBase->OnInstructionFinished.RemoveDynamic(this, &ACardBase::OnInstructionFinished);

	// Decrease executing instruction counter
	ExecutingInstructionCount--;

	// Try to execute next instruction when all instruction are finished
	if (ExecutingInstructionCount == 0)
	{
		// Check if current card is finished
		if (CurrentInstructionIndex >= InstructionQueue.Num())
		{
			Finalize();
		}
		else
		{
			ExecuteNextInstruction();
		}
	}
}

