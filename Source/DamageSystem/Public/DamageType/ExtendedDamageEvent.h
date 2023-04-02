#pragma once

#include "CoreMinimal.h"
#include "ExtendedDamageEvent.generated.h"

class UDamageReceiverComponent;

USTRUCT(BlueprintType)
struct FExtendedDamageEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExtendedDamageEvent")
	float DamageValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExtendedDamageEvent")
	UDamageReceiverComponent* ReceiverComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExtendedDamageEvent")
	AActor* DamageCauser;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExtendedDamageEvent")
	AController* Instigator;
};