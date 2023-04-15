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

	UFUNCTION(BlueprintCallable, Category="CardDescObject")
	void SetCardName(FText InName);

	UFUNCTION(BlueprintCallable, Category="CardDescObject")
	void SetCardDesc(FText InDesc);

	UFUNCTION(BlueprintCallable, Category="CardDescObject")
	void SetCardPicturePath(FString InPath);

	void SetCardInstanceId(int32 InId);

	UFUNCTION(BlueprintCallable, Category="CardDescObject")
	int32 GetCardInstanceId() const
	{
		return CardInstanceId;
	}

protected:
	UPROPERTY(Transient, Replicated)
	FText CardName;

	UPROPERTY(Transient, Replicated)
	FText CardDesc;

	UPROPERTY(Transient, Replicated)
	FString CardPicturePath;

	UPROPERTY(Transient, Replicated)
	int32 CardInstanceId;
};
