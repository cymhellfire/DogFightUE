#include "Card/Card.h"

#include "CardSystem.h"
#include "Card/CardConcurrentCallbackCommand.h"
#include "Card/CardAsyncCommand.h"
#include "Card/CardCommand.h"
#include "Card/CardDescObject.h"
#include "Card/CardLogic.h"
#include "CardModifier/CardModifier.h"

UCard::UCard()
{
	WaitingTargetBatch = -1;
	bWaitAsyncCommand = false;
	bAutoConsume = false;
	ExecutingIndex = 0;
	WaitingConcurrentCommands = 0;
}

void UCard::PostInitProperties()
{
	UObject::PostInitProperties();

	// Start initialize card class
	Initialize();
}

void UCard::Initialize()
{
	// Create description object
	DescObject = NewObject<UCardDescObject>(GetOuter(), NAME_None, RF_Transient);

	// Invoke blueprint side implementation
	BP_Initialize();

	// Invoke the description gathering
	FString CardName;
	FString CardDesc;
	BP_GetCardBasicDesc(CardName, CardDesc);
	if (DescObject)
	{
		DescObject->SetCardName(CardName);
	}
}

void UCard::SetInstanceId(int32 InId)
{
	if (InId == CardInstanceId)
	{
		return;
	}

	CardInstanceId = InId;
	// Also sync to desc object
	if (DescObject)
	{
		DescObject->SetCardInstanceId(InId);
	}
}

void UCard::AddAttribute(const FAttributeCreateArgument& InArgument)
{
	if (DescObject)
	{
		DescObject->AddAttribute(InArgument);
	}
}

bool UCard::GetAttributeBoolValue(FName InName, bool& OutValue)
{
	if (DescObject)
	{
		return DescObject->GetAttributeBoolValue(InName, OutValue);
	}

	return false;
}

bool UCard::GetAttributeIntegerValue(FName InName, int32& OutValue)
{
	if (DescObject)
	{
		return DescObject->GetAttributeIntegerValue(InName, OutValue);
	}

	return false;
}

bool UCard::GetAttributeFloatValue(FName InName, float& OutValue)
{
	if (DescObject)
	{
		return DescObject->GetAttributeFloatValue(InName, OutValue);
	}

	return false;
}

void UCard::AddModifierObject(UCardModifier* InModifier)
{
	if (DescObject)
	{
		DescObject->AddModifierObject(InModifier);
	}
}

void UCard::RemoveModifierObject(UCardModifier* InModifier)
{
	if (DescObject)
	{
		DescObject->RemoveModifierObject(InModifier);
	}
}

void UCard::CreateCardLogic()
{
	CardLogic = NewObject<UCardLogic>(this, TEXT("CardLogic"), RF_Transient);
	if (IsValid(CardLogic))
	{
		CardLogic->OnCardLogicFinished.AddDynamic(this, &UCard::OnCardLogicFinished);
		CardLogic->InitLogic(this);
		// Start the logic
		CardLogic->StartLogic(LogicScriptPath);
	}
}

/**
 * Execution progress can be seperated into two parts:
 * ------------------------------------------------
 * |  Select Targets  |  Card Logic with Targets  |
 * ------------------------------------------------
 */
void UCard::Execute()
{
	if (!LogicScriptPath.IsEmpty())
	{
		CreateCardLogic();
	}
	else
	{
		StartAcquireTargets();
	}
}

void UCard::SetOwnerController(AController* InOwner)
{
	if (!IsValid(InOwner))
	{
		UE_LOG(LogCardSystem, Error, TEXT("[Card] Owner controller cannot be nullptr."));
		return;
	}

	// Check if this controller implemented specified interfaces
	if (InOwner->Implements<UCardTargetProviderInterface>())
	{
		OwnerController = InOwner;
	}
	else
	{
		UE_LOG(LogCardSystem, Error, TEXT("[Card] Owner controller must implement UCardTargetProviderInterface."));
	}
}

void UCard::SetOwnerPlayerId(int32 PlayerId)
{
	OwnerPlayerId = PlayerId;
}

void UCard::StartAcquireTargets()
{
	AcquireCardTargetsImplementation();
}

/**
 * Target batches are acquired one by one. Multiple acquiring will be queued.
 */
void UCard::AcquireTargetBatch(FTargetAcquireSettings Settings, int32 BatchIndex)
{
	// Queue acquiring info if there is one already processing
	if (WaitingTargetBatch != -1)
	{
		if (!PendingTargetAcquireInfo.Contains(BatchIndex))
		{
			PendingTargetAcquireInfo.Add(BatchIndex, Settings);
		}
		else
		{
			UE_LOG(LogCardSystem, Warning, TEXT("[Card] Duplicated target batch [%d] acquire detected. This acquiring will be ignored."), BatchIndex);
		}
		return;
	}

	AcquireTargetBatch_Internal(Settings, BatchIndex);
}

void UCard::AcquireTargetBatch_Internal(FTargetAcquireSettings Settings, int32 BatchIndex)
{
	if (OwnerController.IsValid())
	{
		ICardTargetProviderInterface* TargetProvider = Cast<ICardTargetProviderInterface>(OwnerController.Get());
		if (TargetProvider)
		{
			TargetProvider->StartAcquireTargets(Settings, [this, BatchIndex](bool bSuccess, TArray<FAcquiredTargetInfo> TargetInfoList)
			{
				if (bSuccess && TargetInfoList.Num() > 0)
				{
					// Get the target type of this batch (Only one type target in a batch at same time).
					ECardTargetType TargetType = TargetInfoList[0].Type;

					switch(TargetType)
					{
					case ECardTargetType::CTT_Point:
						{
							TArray<FVector> FinalTarget;
							for (auto TargetInfo : TargetInfoList)
							{
								FinalTarget.Add(TargetInfo.Vector);
							}
							RecordPointTargets(BatchIndex, FinalTarget);
						}
						break;
					case ECardTargetType::CTT_Direction:
						{
							TArray<FVector> FinalTarget;
							for (auto TargetInfo : TargetInfoList)
							{
								FinalTarget.Add(TargetInfo.Vector);
							}
							RecordDirectionTargets(BatchIndex, FinalTarget);
						}
						break;
					case ECardTargetType::CTT_Actor:
						{
							TArray<TWeakObjectPtr<AActor>> FinalTarget;
							for (auto TargetInfo : TargetInfoList)
							{
								FinalTarget.Add(TargetInfo.ActorPtr);
							}
							RecordActorTargets(BatchIndex, FinalTarget);
						}
						break;
					default:
						break;
					}
				}

				OnTargetBatchFinished(bSuccess, BatchIndex);
			});

			// Record batch index to waiting list
			WaitingTargetBatch = BatchIndex;
		}
	}
}

void UCard::OnTargetBatchFinished(bool bSuccess, int32 BatchIndex)
{
	if (BatchIndex != WaitingTargetBatch)
	{
		UE_LOG(LogCardSystem, Error, TEXT("[Card] Target batch mismatched, card cancelled. Expecting [%d] but return [%d]."), WaitingTargetBatch, BatchIndex);
		OnCardCancel();
		return;
	}

	// Any target batch returns with bSuccess false can be considered as card cancelled
	if (!bSuccess)
	{
		OnCardCancel();
		return;
	}

	// Reset waiting batch to allow next acquiring
	WaitingTargetBatch = -1;

	// Continue to select next target batch
	if (PendingTargetAcquireInfo.Num() > 0)
	{
		int32 NextBatch = PendingTargetAcquireInfo.begin().Key();
		FTargetAcquireSettings NextSetting = PendingTargetAcquireInfo.begin().Value();
		PendingTargetAcquireInfo.Remove(NextBatch);

		AcquireTargetBatch_Internal(NextSetting, NextBatch);
	}
	else
	{
		OnAllTargetSet();
	}
}

void UCard::OnAllTargetSet()
{
	// Stop here if no targets recorded
	if (TargetTypeMap.Num() == 0)
	{
		OnCardCancel();
		return;
	}

	// Continue to card logic
	StartCardLogic();
}

void UCard::RecordPointTargets(int32 BatchIndex, TArray<FVector> PointList)
{
	TargetTypeMap.Add(BatchIndex, ECardTargetType::CTT_Point);
	PointTargetMap.Add(BatchIndex, PointList);
}

void UCard::RecordDirectionTargets(int32 BatchIndex, TArray<FVector> DirectionList)
{
	TargetTypeMap.Add(BatchIndex, ECardTargetType::CTT_Direction);
	DirectionTargetMap.Add(BatchIndex, DirectionList);
}

void UCard::RecordActorTargets(int32 BatchIndex, TArray<TWeakObjectPtr<AActor>> ActorList)
{
	TargetTypeMap.Add(BatchIndex, ECardTargetType::CTT_Actor);
	ActorTargetMap.Add(BatchIndex, ActorList);
}

void UCard::ClearAllTargetInfo()
{
	WaitingTargetBatch = -1;
	PendingTargetAcquireInfo.Empty();
	TargetTypeMap.Empty();
	PointTargetMap.Empty();
	DirectionTargetMap.Empty();
	ActorTargetMap.Empty();
}

void UCard::OnCardCancel()
{
	UE_LOG(LogCardSystem, Log, TEXT("[Card] %s card cancelled."), *GetName());

	ClearAllTargetInfo();
	// Broadcast cancel event
	OnCardExecutionFinished.Broadcast(ECardExecutionResult::CER_Cancelled, this);
}

/**
 * Instructions in card logic can be synchronous or asynchronous. So all the instructions will be queued and executed
 * one by one. CardLogicImplementation() function will be overwritten by Lua and executed synchronously. All function
 * call in it should be a wrapper version and created a queued command. 
 */
void UCard::StartCardLogic()
{
	// Invoke logic implementation
	CardLogicImplementation();
}

void UCard::OnAsyncCommandFinished(UCardAsyncCommand* Command, bool bSuccess)
{
	// Unregister callback
	Command->OnCommandFinished.RemoveDynamic(this, &UCard::OnAsyncCommandFinished);

	// Unblock commands
	bWaitAsyncCommand = false;

	// Enable auto consume when resume from async command
	bAutoConsume = true;

	// Continue to execute left commands
	ConsumeCommand();
}

void UCard::OnConcurrentCallbackCommandFinished(UCardConcurrentCallbackCommand* Command, int32 Result)
{
	Command->OnCommandFinished.RemoveDynamic(this, &UCard::UCard::OnConcurrentCallbackCommandFinished);

	WaitingConcurrentCommands--;

	int32 CommandIndex = CommandQueue.Find(Command);

	// Notify lua side
	OnCallbackResult(CommandIndex, Result);

	// If all waiting command are finished check card finish
	if (WaitingConcurrentCommands <= 0)
	{
		CheckCardFinished();
	}
}

TArray<FVector> UCard::GetPointTargetListByBatch(int32 BatchIndex) const
{
	if (TargetTypeMap.Contains(BatchIndex) && TargetTypeMap[BatchIndex] == ECardTargetType::CTT_Point)
	{
		return PointTargetMap[BatchIndex];
	}

	UE_LOG(LogCardSystem, Error, TEXT("[Card] Batch Index [%d] is not recorded as Point type."), BatchIndex);
	return TArray<FVector>();
}

TArray<FVector> UCard::GetDirectionTargetListByBatch(int32 BatchIndex) const
{
	if (TargetTypeMap.Contains(BatchIndex) && TargetTypeMap[BatchIndex] == ECardTargetType::CTT_Direction)
	{
		return DirectionTargetMap[BatchIndex];
	}

	UE_LOG(LogCardSystem, Error, TEXT("[Card] Batch Index [%d] is not recorded as Direction type."), BatchIndex);
	return TArray<FVector>();
}

TArray<AActor*> UCard::GetActorTargetListByBatch(int32 BatchIndex) const
{
	if (TargetTypeMap.Contains(BatchIndex) && TargetTypeMap[BatchIndex] == ECardTargetType::CTT_Actor)
	{
		TArray<AActor*> Result;
		for (auto TargetInfo : ActorTargetMap[BatchIndex])
		{
			if (TargetInfo.IsValid())
			{
				Result.Add(TargetInfo.Get());
			}
		}
		return Result;
	}

	UE_LOG(LogCardSystem, Error, TEXT("[Card] Batch Index [%d] is not recorded as Actor type."), BatchIndex);
	return TArray<AActor*>();
}

/**
 * Synchronous commands will be executed immediately once be queued until any asynchronous command arrived.
 * Asynchronous command causes all commands adds after it holding in the queue until the command is finished.
 */
void UCard::QueueCommand(UCardCommand* NewCommand)
{
	CommandQueue.AddUnique(NewCommand);

	// Disable auto consume when enqueue new command
	bAutoConsume = false;

	// Try to execute commands after enqueue
	ConsumeCommand();
}

/**
 * Execute next command in queue.
 */
void UCard::ConsumeCommand()
{
	// Stop if any asynchronous command is executing
	if (bWaitAsyncCommand)
		return;

	if (CheckCardFinished())
	{
		return;
	}

	UCardCommand* NextCommand = CommandQueue[ExecutingIndex];
	NextCommand->Run();
	ExecutingIndex++;

	// Block commands after any asynchronous command
	if (NextCommand->GetExecuteType() == ECardCommandExecuteType::CCET_Asynchronous)
	{
		bWaitAsyncCommand = true;

		// Register finish callback
		UCardAsyncCommand* AsyncCommand = Cast<UCardAsyncCommand>(NextCommand);
		if (AsyncCommand)
		{
			AsyncCommand->OnCommandFinished.AddDynamic(this, &UCard::OnAsyncCommandFinished);
		}
		else
		{
			UE_LOG(LogCardSystem, Error, TEXT("[Card] Command %s has Asynchronous type but cannot be converted to AsyncCommand."),
				*NextCommand->GetClass()->GetName());
		}
	}
	else if (NextCommand->GetExecuteType() == ECardCommandExecuteType::CCET_ConcurrentCallback)
	{
		WaitingConcurrentCommands++;

		// Register callback
		UCardConcurrentCallbackCommand* ConcurrentCallbackCommand = Cast<UCardConcurrentCallbackCommand>(NextCommand);
		if (ConcurrentCallbackCommand)
		{
			ConcurrentCallbackCommand->OnCommandFinished.AddDynamic(this, &UCard::UCard::OnConcurrentCallbackCommandFinished);
		}
		else
		{
			UE_LOG(LogCardSystem, Error, TEXT("[Card] Command %s has ConcurrentCallback type but cannot be converted to ConcurrentCallbackCommand."),
				*NextCommand->GetClass()->GetName());
		}
	}

	// Auto consume next one
	if (bAutoConsume)
	{
		ConsumeCommand();
	}
}

bool UCard::CheckCardFinished()
{
	// Check if command queue is finished
	if (ExecutingIndex >= CommandQueue.Num())
	{
		// Check if any concurrent command is not finished
		// Return true although there might some concurrent commands executing, since this command also affect the
		// command queue iterating. Return true to avoid the ExecutingIndex getting out of range.
		if (WaitingConcurrentCommands == 0)
		{
			OnCardFinished();
		}
		return true;
	}

	return false;
}

void UCard::OnCardLogicFinished(ECardLogicFinishType::Type FinishType)
{
	if (FinishType == ECardLogicFinishType::Success)
	{
		OnCardFinished();
	}
	else
	{
		OnCardCancel();
	}
}

void UCard::OnCardFinished()
{
	UE_LOG(LogCardSystem, Log, TEXT("[Card] %s card finished."), *GetName());

	ClearAllTargetInfo();
	// Broadcast finish event
	OnCardExecutionFinished.Broadcast(ECardExecutionResult::CER_Default, this);
}
