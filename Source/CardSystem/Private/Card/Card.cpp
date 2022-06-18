#include "Card/Card.h"

#include "CardSystem.h"

UCard::UCard()
{
	WaitingTargetBatch = -1;
}

/**
 * Execution progress can be seperated into two parts:
 * ------------------------------------------------
 * |  Select Targets  |  Card Logic with Targets  |
 * ------------------------------------------------
 */
void UCard::Execute()
{
	StartAcquireTargets();
}

void UCard::SetOwnerController(AController* InOwner)
{
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

void UCard::StartCardLogic()
{
	// Invoke logic implementation
	CardLogicImplementation();
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

void UCard::OnCardFinished()
{
	UE_LOG(LogCardSystem, Log, TEXT("[Card] %s card finished."), *GetName());

	ClearAllTargetInfo();
	// Broadcast finish event
	OnCardExecutionFinished.Broadcast(ECardExecutionResult::CER_Default, this);
}
