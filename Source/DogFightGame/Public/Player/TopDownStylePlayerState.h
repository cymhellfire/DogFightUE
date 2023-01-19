#pragma once

#include "GameFramework/PlayerState.h"
#include "TopDownStylePlayerState.generated.h"

class UCard;
class UCardDescObject;

UCLASS()
class DOGFIGHTGAME_API ATopDownStylePlayerState : public APlayerState
{
	GENERATED_BODY()
public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Add a new card to this player state.
	 * @param InCard			New card that added to this player state.
	 */
	void AddCardObject(UCard* InCard);

	/**
	 * Remove specified card from this player state.
	 * @param InCard			The card to be removed.
	 */
	void RemoveCardObject(UCard* InCard);

	/**
	 * Get the total card count of this player.
	 * @return			Card count.
	 */
	int32 GetCardNum() const
	{
		return CardDescObjectList.Num();
	}

	/**
	 * Get the card desc object by specified index.
	 * @param Index				The index of card description to obtain.
	 * @return					Corresponding card desc object.
	 */
	UCardDescObject* GetCardDescObjectByIndex(int32 Index);

	/**
	 * Get all card desc objects this player holds.
	 * @return				List of all card desc object.
	 */
	UFUNCTION(BlueprintCallable, Category="TopDownStylePlayerState")
	TArray<UCardDescObject*> GetAllCardDescObject() const
	{
		return CardDescObjectList;
	}

protected:
	void AddCardDescObject(UCardDescObject* InDescObject);

	void RemoveCardDescObject(UCardDescObject* InDescObject);

	UFUNCTION()
	void OnRep_CardDescObjectList();

protected:
	UPROPERTY(Transient)
	TArray<UCard*> CardObjectList;

	UPROPERTY(Transient, ReplicatedUsing=OnRep_CardDescObjectList)
	TArray<UCardDescObject*> CardDescObjectList;
};
