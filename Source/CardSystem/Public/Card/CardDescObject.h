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

protected:
	UPROPERTY(Transient, Replicated)
	FString CardName;
};
