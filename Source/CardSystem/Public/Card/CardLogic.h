#pragma once

#include "UnLuaInterface.h"
#include "Common/CardSystemType.h"
#include "CardLogic.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardLogicFinished, ECardLogicFinishType::Type, Result);

/**
 * Card logic class hold the actual card commands that formed the card.
 */
UCLASS(Blueprintable)
class CARDSYSTEM_API UCardLogic : public UObject, public FTickableGameObject, public IUnLuaInterface
{
	GENERATED_BODY()

public:
	UCardLogic();

	void StartLogic(const FString& ScriptPath);

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

protected:

	UFUNCTION(BlueprintImplementableEvent, Category="CardLogic")
	void OnLogicStarted(const FString& ScriptPath);

	UFUNCTION(BlueprintImplementableEvent, Category="CardLogic")
	void TickLogicScript(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category="CardLogic")
	void MarkLogicFinished(ECardLogicFinishType::Type Type);

public:
	UPROPERTY(BlueprintAssignable, Category="CardLogic")
	FOnCardLogicFinished OnCardLogicFinished;

protected:
	uint8 bFinished:1;
};
