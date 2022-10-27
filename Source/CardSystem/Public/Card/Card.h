#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/Modifier/AttributeModifierCommon.h"
#include "Player/CardTargetProviderInterface.h"
#include "Card.generated.h"

class UCardCommand;
class UCardAsyncCommand;
class UCardConcurrentCallbackCommand;
class FAttributeBase;

UENUM()
enum class ECardExecutionResult : uint8
{
	CER_Default,
	CER_Cancelled,
};

UCLASS(Blueprintable)
class CARDSYSTEM_API UCard : public UObject
{
	GENERATED_BODY()
public:
	UCard();

	// ---------------- Card Attribute ---------------------
	UFUNCTION(BlueprintCallable, Category="Card")
	bool CreateAttributeBool(FName InName, bool InValue);

	UFUNCTION(BlueprintCallable, Category="Card")
	bool CreateAttributeInteger(FName InName, int32 InValue);

	UFUNCTION(BlueprintCallable, Category="Card")
	bool CreateAttributeFloat(FName InName, float InValue);

	bool AddAttribute(TSharedPtr<FAttributeBase> InAttribute);

	TWeakPtr<FAttributeBase> GetAttribute(FName InName);

	UFUNCTION(BlueprintCallable, Category="Card")
	bool RemoveAttribute(FName InName);

	UFUNCTION(BlueprintCallable, Category="Card")
	bool GetAttributeBoolValue(FName InName, bool& OutValue);

	UFUNCTION(BlueprintCallable, Category="Card")
	bool GetAttributeIntegerValue(FName InName, int32& OutValue);

	UFUNCTION(BlueprintCallable, Category="Card")
	bool GetAttributeFloatValue(FName InName, float& OutValue);

	UFUNCTION(BlueprintCallable, Category="Card")
	bool CreateModifierForBoolAttribute(FName InName, bool InValue);

	UFUNCTION(BlueprintCallable, Category="Card")
	bool CreateModifierForIntegerAttribute(FName InName, int32 InValue, EModifierOperatorType OpType);

	UFUNCTION(BlueprintCallable, Category="Card")
	bool CreateModifierForFloatAttribute(FName InName, float InValue, EModifierOperatorType OpType);

protected:
	TMap<FName, TSharedPtr<FAttributeBase>> AttributeMap;

public:
	/**
	 * Execute this card logic flow.
	 */
	void Execute();

	void SetOwnerController(AController* InOwner);

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCardExecutedSignature, ECardExecutionResult, Result, UCard*, Card);
	FCardExecutedSignature OnCardExecutionFinished;

	// ---------------- Acquire Targets ---------------------
	void StartAcquireTargets();

	UFUNCTION(BlueprintImplementableEvent)
	void AcquireCardTargetsImplementation();

	UFUNCTION()
	void AcquireTargetBatch(FTargetAcquireSettings Settings, int32 BatchIndex);

	void AcquireTargetBatch_Internal(FTargetAcquireSettings Settings, int32 BatchIndex);

protected:

	void OnTargetBatchFinished(bool bSuccess, int32 BatchIndex);

	void OnAllTargetSet();
	void RecordPointTargets(int32 BatchIndex, TArray<FVector> PointList);
	void RecordDirectionTargets(int32 BatchIndex, TArray<FVector> DirectionList);
	void RecordActorTargets(int32 BatchIndex, TArray<TWeakObjectPtr<AActor>> ActorList);

	void ClearAllTargetInfo();

	// ------------------ Card Cancel -----------------------

	void OnCardCancel();

	// ------------------ Card Logic ------------------------
public:
	void StartCardLogic();

	UFUNCTION(BlueprintImplementableEvent)
	void CardLogicImplementation();

	UFUNCTION()
	void OnAsyncCommandFinished(UCardAsyncCommand* Command, bool bSuccess);

	UFUNCTION()
	void OnConcurrentCallbackCommandFinished(UCardConcurrentCallbackCommand* Command, int32 Result);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCallbackResult(int32 CommandIndex, int32 Result);

	UFUNCTION()
	TArray<FVector> GetPointTargetListByBatch(int32 BatchIndex) const;
	UFUNCTION()
	TArray<FVector> GetDirectionTargetListByBatch(int32 BatchIndex) const;
	UFUNCTION()
	TArray<AActor*> GetActorTargetListByBatch(int32 BatchIndex) const;

	void QueueCommand(UCardCommand* NewCommand);

protected:

	void ConsumeCommand();

	bool CheckCardFinished();

public:

	// ----------------- Card Finished ----------------------

	/** Invoke when card execution finished. */
	UFUNCTION()
	void OnCardFinished();

protected:
	/** The owner controller to handle necessary RPC function call. */
	TWeakObjectPtr<AController> OwnerController;

	// Target acquiring info list
	int32 WaitingTargetBatch;
	TMap<int32, FTargetAcquireSettings> PendingTargetAcquireInfo;
	// Target info variables
	TMap<int32, ECardTargetType> TargetTypeMap;
	TMap<int32, TArray<FVector>> PointTargetMap;
	TMap<int32, TArray<FVector>> DirectionTargetMap;
	TMap<int32, TArray<TWeakObjectPtr<AActor>>> ActorTargetMap;

	// Command
	uint8 bWaitAsyncCommand : 1;
	uint8 bAutoConsume : 1;

	int32 ExecutingIndex;
	int32 WaitingConcurrentCommands;

	UPROPERTY()
	TArray<UCardCommand*> CommandQueue;
};
