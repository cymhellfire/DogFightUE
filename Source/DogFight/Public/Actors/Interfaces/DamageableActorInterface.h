#pragma once

#include "CoreMinimal.h"
#include "DamageableActorInterface.generated.h"

class UReceiveDamageComponent;
struct FDamageStruct;

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

	/** Add invincible flags to target. */
	virtual void AddInvincibleFlags(int32 Flags) {}

	/** Remove invincible flags from target. */
	virtual void RemoveInvincibleFlags(int32 Flags) {}

	/** Add a new armor to this actor. */
	virtual bool AddExtraArmor(struct FActorArmor& NewArmor) { return true; }

	/** Remove specified armor from this actor. */
	virtual bool RemoveExtraArmor(struct FActorArmor& TargetArmor) { return true; }

	/** Set health based on given percentage. */
	virtual void SetHealthPercentage(float NewPercentage) {}

	/** Get if this actor is alive. */
	virtual bool IsAlive() { return false; }

	/** Apply damage to actor. */
	virtual void ApplyDamage(FDamageStruct Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) = 0;
};
