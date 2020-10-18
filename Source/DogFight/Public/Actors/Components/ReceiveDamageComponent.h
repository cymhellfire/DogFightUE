// Dog Fight Game Code By CYM.

#pragma once

#include "DogFight.h"
#include "Components/ActorComponent.h"
#include "DamageStructures.h"
#include "ReceiveDamageComponent.generated.h"

/**
 * Component that contains all damage relevant variables for parent actor.
 */
UCLASS( ClassGroup=(DogFight), meta=(BlueprintSpawnableComponent) )
class DOGFIGHT_API UReceiveDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UReceiveDamageComponent();

	/** Get all extra armors. */
	TArray<FActorArmor> GetExtraArmors() const { return ExtraArmorList; }

public:

	/** The armor to reduce any physical type damage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReceiveDamageComponent", Replicated)
	int32 PhysicalArmor;

	/** The armor to reduce any magical type damage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReceiveDamageComponent", Replicated)
	int32 MagicalArmor;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ReceiveDamageComponent", Replicated)
	TArray<FActorArmor> ExtraArmorList;
};
