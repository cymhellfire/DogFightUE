#pragma once

#include "CoreMinimal.h"
#include "DamageableActorInterface.generated.h"

class UReceiveDamageComponent;

UINTERFACE(MinimalAPI, Blueprintable)
class UDamageableActorInterface : public UInterface
{
	GENERATED_BODY()
};

class IDamageableActorInterface
{
	GENERATED_BODY()

public:

	/** Get the damage receive component of this actor. */
	virtual UReceiveDamageComponent* GetDamageReceiveComponent() { return nullptr; }

	/** Change target invincible state. */
	virtual void SetInvincible(bool bActive) {}

	/** Add a new armor to this actor. */
	virtual bool AddExtraArmor(struct FActorArmor& NewArmor) { return true; }

	/** Remove specified armor from this actor. */
	virtual bool RemoveExtraArmor(struct FActorArmor& TargetArmor) { return true; }

	/** Set health based on given percentage. */
	virtual void SetHealthPercentage(float NewPercentage) {}
};
