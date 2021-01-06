#pragma once

#include "CoreMinimal.h"
#include "ActorInterfaces.generated.h"

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

	/** Set Physical Armor value. */
	virtual void SetPhysicalArmor(int32 NewArmor) {}

	/** Get Physical Armor value. */
	virtual int32 GetPhysicalArmor() const { return 0; }

	/** Set Magical Armor value. */
	virtual void SetMagicalArmor(int32 NewArmor) {}

	/** Get Magical Armor value. */
	virtual int32 GetMagicalArmor() const { return 0; }
};
