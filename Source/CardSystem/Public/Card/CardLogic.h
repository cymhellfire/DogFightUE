#pragma once

#include "UnLuaInterface.h"
#include "Common/CardSystemType.h"
#include "CardLogic.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardLogicFinished, ECardLogicFinishType::Type, Result);

class UCard;
class UCardDescObject;

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

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="CardLogic")
	void LoadAndInitLogicScript(int32 CardLogicId);

	UFUNCTION(BlueprintImplementableEvent, Category="CardLogic")
	void OnLogicStarted();

	UFUNCTION(BlueprintImplementableEvent, Category="CardLogic")
	void TickLogicScript(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category="CardLogic")
	void MarkLogicFinished(ECardLogicFinishType::Type Type);

public:
	UPROPERTY(BlueprintAssignable, Category="CardLogic")
	FOnCardLogicFinished OnCardLogicFinished;

protected:
	uint8 bFinished:1;

	TWeakObjectPtr<UCard> OwnerCard;
};
