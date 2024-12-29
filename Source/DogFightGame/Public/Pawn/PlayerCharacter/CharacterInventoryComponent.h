// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Card/Card.h"
#include "Components/ActorComponent.h"
#include "CharacterInventoryComponent.generated.h"

class UCardDescObject;

/**
 * Component that hold all items for owner actor.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOGFIGHTGAME_API UCharacterInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	/**
	 * Add a new card to this actor.
	 * @param		InCard New card that added to this actor.
	 */
	void AddCardObject(UCard* InCard);

	/**
	 * Remove specified card from this actor.
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
	 * Get all card desc objects this actor holds.
	 * @return				List of all card desc object.
	 */
	UFUNCTION(BlueprintCallable, Category="CharacterInventory")
	TArray<UCardDescObject*> GetAllCardDescObjects() const
	{
		return CardDescObjectList;
	}

	/**
	 * Get card by specified instance id.
	 * @param InId			Instance id that search card with.
	 * @return				Card with given instance id.
	 */
	UFUNCTION(BlueprintCallable, Category="CharacterInventory")
	UCard* GetCardByInstanceId(int32 InId);

	/**
	 * Try to start using process on card with given instance id.
	 * @param InId			Instance id of card want to use.
	 */
	UFUNCTION(Server, Reliable)
	void ServerTryToUseCardByInstanceId(int32 InId);

	/**
	 * Notify client side a card is start acquiring target.
	 * @param InId			Instance id of card to use.
	 */
	UFUNCTION(Client, Reliable)
	void ClientBeginAcquireTarget(int32 InId);

	/**
	 * Notify server side a card is start acquiring target.
	 * @param InId			Instance id of card to use.
	 */
	UFUNCTION(Server, Reliable)
	void ServerBeginAcquireTarget(int32 InId);

	/**
	 * Notify client side a card acquired target.
	 * @param InId			Instance id of card to use.
	 */
	UFUNCTION(Client, Reliable)
	void ClientAcquiredTarget(int32 InId);

	/**
	 * Notify server side a card acquired target.
	 * @param InId			Instance id of card to use.
	 */
	UFUNCTION(Server, Reliable)
	void ServerAcquiredTarget(int32 InId);

	/**
	 * Notify client side a card is started using.
	 * @param InId			Instance id of card is using.
	 */
	UFUNCTION(Client, Reliable)
	void ClientBeginUseCard(int32 InId);

	/**
	 * Notify server side a card is started using.
	 * @param InId			Instance id of card to use.
	 */
	UFUNCTION(Server, Reliable)
	void ServerBeginUseCard(int32 InId);

	/**
	 * Notify client side a card is cancelled.
	 * @param InId			Instance id of card cancelled.
	 */
	UFUNCTION(Client, Reliable)
	void ClientCancelCard(int32 InId);

	/**
	 * Notify server side a card is cancelled.
	 * @param InId			Instance id of card cancelled.
	 */
	void ServerCancelCard(int32 InId);

	/**
	 * Notify client side a card is finished.
	 * @param InId			Instance id of card finished.
	 */
	UFUNCTION(Client, Reliable)
	void ClientCardFinished(int32 InId);

	/**
	 * Notify server side a card is finished.
	 * @param InId			Instance id of card finished.
	 */
	UFUNCTION(Server, Reliable)
	void ServerCardFinished(int32 InId);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_CardDescObjectList();

	UFUNCTION()
	void OnCardAcquiredTarget(ECardExecutionResult Result, UCard* Card);

	UFUNCTION()
	void OnCardFinished(ECardExecutionResult Result, UCard* Card);

	void AddCardDescObject(UCardDescObject* InDescObject);

	void RemoveCardDescObject(UCardDescObject* InDescObject);

	int32 GetPlayerId() const;

	int32 GetTimelineEntityId() const;

private:
	UPROPERTY(Transient)
	TArray<UCard*> CardObjectList;

	UPROPERTY(Transient, ReplicatedUsing=OnRep_CardDescObjectList)
	TArray<UCardDescObject*> CardDescObjectList;
};
