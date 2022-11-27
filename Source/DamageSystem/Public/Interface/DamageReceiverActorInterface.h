#pragma once

#include "DamageReceiverActorInterface.generated.h"

class UDamageReceiverComponent;

UINTERFACE()
class DAMAGESYSTEM_API UDamageReceiverActorInterface : public UInterface
{
	GENERATED_BODY()
};

class DAMAGESYSTEM_API IDamageReceiverActorInterface
{
	GENERATED_BODY()
public:
	virtual UDamageReceiverComponent* GetDamageReceiverComponent() = 0;
};
