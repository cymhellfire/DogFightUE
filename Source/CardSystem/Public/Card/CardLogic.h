#pragma once

#include "UnLuaInterface.h"
#include "Common/CardSystemType.h"
#include "Player/CardTargetProviderInterface.h"
#include "CardLogic.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardTargetAcquired, ECardTargetAcquireType::Type, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardLogicFinished, ECardLogicFinishType::Type, Result);

class UCard;
class UCardDescObject;

UENUM(BlueprintType)
enum class ECardLogicStateType : uint8
{
	ECLST_None,
	ECLST_AcquireTarget,
	ECLST_Logic,
};

/**
 * Card logic class hold the actual card commands that formed the card.
 */
UCLASS(Blueprintable)
class CARDSYSTEM_API UCardLogic : public UObject, public FTickableGameObject, public IUnLuaInterface
{
	GENERATED_BODY()

public:
	UCardLogic();

	void InitLogic(UCard* InCard, int32 LogicId);

	void StartTargetSelect();

	void StartLogic();

	void TickLogic(float DeltaTime);

	virtual FString GetModuleName_Implementation() const override
	{
		return "Card.CardLogic.CardLogic";
	}

	// Tickable Game Object
	virtual TStatId GetStatId() const override
	{
		return Super::GetStatID();
	}

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="CardLogic")
	UCard* GetOwnerCard() const;

	/**
	 * Setup card description from card logic script.
	 * @param InDescObject			Card description object.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="CardLogic")
	void SetupCardDescObject(UCardDescObject* InDescObject);

	/**
	 * Update card description from card logic script.
	 * @param InDescObject			Card description object.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="CardLogic")
	void UpdateCardDescObject(UCardDescObject* InDescObject);

	/**
	 * Push a target to card logic.
	 * @param InTarget				Target info push to logic.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="CardLogic")
	void PushTargetInfo(const FAcquiredTargetInfo& InTarget);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="CardLogic")
	void LoadAndInitLogicScript(int32 CardLogicId);

	UFUNCTION(BlueprintImplementableEvent, Category="CardLogic")
	void OnSelectTargetStarted();

	UFUNCTION(BlueprintImplementableEvent, Category="CardLogic")
	void TickSelectTargetScript(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category="CardLogic")
	void MarkTargetAcquired(ECardTargetAcquireType::Type Type);

	UFUNCTION(BlueprintImplementableEvent, Category="CardLogic")
	void OnLogicStarted();

	UFUNCTION(BlueprintImplementableEvent, Category="CardLogic")
	void TickLogicScript(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category="CardLogic")
	void MarkLogicFinished(ECardLogicFinishType::Type Type);

public:
	UPROPERTY(BlueprintAssignable, Category="CardLogic")
	FOnCardTargetAcquired OnCardTargetAcquired;

	UPROPERTY(BlueprintAssignable, Category="CardLogic")
	FOnCardLogicFinished OnCardLogicFinished;

protected:
	uint8 bFinished:1;

	ECardLogicStateType CurState;

	TWeakObjectPtr<UCard> OwnerCard;
};
