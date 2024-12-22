#pragma once

#include "CoreMinimal.h"
#include "GameService/LuaGameService.h"
#include "Card/Card.h"
#include "CardGameService.generated.h"

class AStandardModePlayerCharacter;
class UCard;
class UCardUseFlow;

UCLASS()
class DOGFIGHTGAME_API UCardGameService : public ULuaGameService
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	UCard* CreateCard(int32 CardConfigId, UObject* Instigator);

	UFUNCTION(BlueprintImplementableEvent)
	void DestroyCard(UCard* InCard);

	UFUNCTION(BlueprintCallable)
	UClass* GetDefaultCardClass() const;

	virtual FString GetModuleName_Implementation() const override
	{
		return TEXT("DogFight.Services.CardService.CardGameService");
	}

	/**
	 * Start a card use flow with given card.
	 * @param InCard Card that used to start a new use flow.
	 */
	UFUNCTION(BlueprintCallable, Category=CardGameService)
	void StartCardUseFlow(UCard* InCard);

	/**
	 * Push a new card to current card use flow.
	 * @param InCard New card to push.
	 */
	UFUNCTION(BlueprintCallable, Category=CardGameService)
	void PushCardToUseFlow(UCard* InCard);

	/**
	 * Mark the card use flow as ended and start to execute.
	 */
	UFUNCTION(BlueprintCallable, Category=CardGameService)
	void EndAndExecuteCardUseFlow();

protected:
	void ClearCardUseFlow();

	void OnCardUseCompleted(UCardUseFlow* InFlow);

	UPROPERTY(Transient)
	UCardUseFlow* CurrentCardUseFlow;
};
