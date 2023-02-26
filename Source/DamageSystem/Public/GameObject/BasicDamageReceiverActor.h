#pragma once

#include "CoreMinimal.h"
#include "Interface/DamageReceiverActorInterface.h"
#include "BasicDamageReceiverActor.generated.h"

class UStaticMeshComponent;
class UDamageReceiverComponent;

UCLASS(Blueprintable)
class DAMAGESYSTEM_API ABasicDamageReceiverActor : public AActor, public IDamageReceiverActorInterface
{
	GENERATED_BODY()

public:
	ABasicDamageReceiverActor();

	virtual UDamageReceiverComponent* GetDamageReceiverComponent() override
	{
		return DamageReceiverComponent;
	}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="BasicDamageReceiver", meta=(ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|StaticMesh"))
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="BasicDamageReceiver")
	UDamageReceiverComponent* DamageReceiverComponent;
};
