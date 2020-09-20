// Dog Fight Game Code By CYM.


#include "CardInstructionBase.h"

#include "DelayAction.h"

UCardInstructionBase::UCardInstructionBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstructionName = TEXT("NewInstruction");
	ExecuteType = ECardInstructionExecuteType::Synchronous;
}

bool UCardInstructionBase::Tick(float DeltaSeconds)
{
	// Invoke Execute at the first frame
	Execute();

	// No need to tick after Execute()
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);

	return true;
}

void UCardInstructionBase::BeginDestroy()
{
	Super::BeginDestroy();

	if (GEngine != nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Begin Destroy %s"), *InstructionName));
}

void UCardInstructionBase::ApplyMeta(const FInstructionMetaBase* MetaData)
{
	check(MetaData);

	InstructionName = MetaData->InstructionName;
	ExecuteType = MetaData->ExecuteType;
}

void UCardInstructionBase::BeginExecute()
{
	// Register tick for execution
	TickDelegate = FTickerDelegate::CreateUObject(this, &UCardInstructionBase::Tick);
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);
}

void UCardInstructionBase::Execute()
{
	// Invoke blueprint implemented events if this class is blueprint based.
	if (GetClass()->HasAnyClassFlags(CLASS_CompiledFromBlueprint) || !GetClass()->HasAnyClassFlags(CLASS_Native))
	{
		BeginExecuteBP();
	}
}

void UCardInstructionBase::Finish()
{
	if (bIsFinished)
		return;

	bIsFinished = true;

	// Broadcast the callback
	OnInstructionFinished.Broadcast(this);
}
