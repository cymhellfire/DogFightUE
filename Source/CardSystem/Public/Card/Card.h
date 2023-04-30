#pragma once

#include "CoreMinimal.h"
#include "UnLuaInterface.h"
#include "AttributeSystem/Attribute/AttributeCommon.h"
#include "Common/CardSystemType.h"
#include "Player/CardTargetProviderInterface.h"
#include "Card.generated.h"

class UCardCommand;
class UCardAsyncCommand;
class UCardConcurrentCallbackCommand;
class UCardModifier;
class UCardDescObject;
class UCardLogic;

UENUM()
enum class ECardExecutionResult : uint8
{
	CER_Default,
	CER_Cancelled,
};

UCLASS(Blueprintable)
class CARDSYSTEM_API UCard : public UObject, public IUnLuaInterface
{
	GENERATED_BODY()
public:
	UCard();

	// IUnLuaInterface
	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Card.CardBase";
	}

	// ------------------- Initialize ----------------------
	virtual void InitDescObject();

	UFUNCTION(BlueprintCallable, Category="Card")
	void SetInstanceId(int32 InId);

	UFUNCTION(BlueprintCallable, Category="Card")
	int32 GetInstanceId() const
	{
		return CardInstanceId;
	}

	UFUNCTION(BlueprintCallable, Category="Card")
	void SetOwnerController(AController* InOwner);

	AController* GetOwnerController() const
	{
		return OwnerController.IsValid() ? OwnerController.Get() : nullptr;
	}

	UFUNCTION(BlueprintCallable, Category="Card")
	void SetOwnerPlayerId(int32 PlayerId);

	UFUNCTION(BlueprintCallable, Category="Card")
	int32 GetOwnerPlayerId() const
	{
		return OwnerPlayerId;
	}

	// ------------------- Attribute ----------------------

	UFUNCTION(BlueprintCallable, Category="Card")
	void AddAttribute(const FAttributeCreateArgument& InArgument);

	UFUNCTION(BlueprintCallable, Category="Card")
	bool GetAttributeBoolValue(FName InName, bool& OutValue);

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	bool GetAttributeIntegerValue(FName InName, int32& OutValue);

	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	bool GetAttributeFloatValue(FName InName, float& OutValue);

	UCardDescObject* GetDescObject() const
	{
		return DescObject;
	}

	UFUNCTION(BlueprintCallable, Category="Card")
	void AddModifierObject(UCardModifier* InModifier);

	UFUNCTION(BlueprintCallable, Category="Card")
	void RemoveModifierObject(UCardModifier* InModifier);

	// ---------------- Card Logic ---------------------
	UFUNCTION(BlueprintCallable, Category="Card")
	void SetCardLogicPath(const FString&  InPath);

protected:
	void CreateCardLogic();

	void StartCardLogic();

	UFUNCTION()
	void OnCardLogicFinished(ECardLogicFinishType::Type FinishType);

public:
	/**
	 * Execute this card logic flow.
	 */
	void Execute();

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCardExecutedSignature, ECardExecutionResult, Result, UCard*, Card);
	FCardExecutedSignature OnCardExecutionFinished;

	// ------------------ Card Cancel -----------------------

	void OnCardCancel();

	// ----------------- Card Finished ----------------------

	/** Invoke when card execution finished. */
	UFUNCTION()
	void OnCardFinished();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Card")
	FString LogicScriptPath;

protected:
	UPROPERTY(Transient)
	UCardDescObject* DescObject;

	UPROPERTY(Transient)
	UCardLogic* CardLogic;

	/** Id used to distinguish different cards. */
	int32 CardInstanceId;

	int32 OwnerPlayerId;

	/** The owner controller to handle necessary RPC function call. */
	TWeakObjectPtr<AController> OwnerController;
};
