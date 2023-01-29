#pragma once

#include "UnrealIntegration/UObject/AttributeBasedObject.h"
#include "CardDescObject.generated.h"

/**
 * Card description object that is replicated through network.
 */
UCLASS()
class CARDSYSTEM_API UCardDescObject : public UAttributeBasedObject
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetCardName(FString InName);

	void SetCardInstanceId(int32 InId);

	UFUNCTION(BlueprintCallable, Category="CardDescObject")
	int32 GetCardInstanceId() const
	{
		return CardInstanceId;
	}

protected:
	UPROPERTY(Transient, Replicated)
	FString CardName;

	UPROPERTY(Transient, Replicated)
	int32 CardInstanceId;
};
