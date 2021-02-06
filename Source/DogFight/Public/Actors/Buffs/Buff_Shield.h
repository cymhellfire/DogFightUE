#pragma once

#include "CoreMinimal.h"
#include "BuffBase.h"
#include "Buff_Shield.generated.h"

class AShieldBase;

UCLASS()
class ABuff_Shield : public ABuffBase
{
	GENERATED_BODY()

public:

	void SetShieldActor(AShieldBase* NewShield);

protected:
	virtual void RemoveBuff() override;

protected:
	AShieldBase* ShieldActor;
};
