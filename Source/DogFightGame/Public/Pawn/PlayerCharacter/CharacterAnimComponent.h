// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterAnimComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOGFIGHTGAME_API UCharacterAnimComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterAnimComponent();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayMontage(UAnimMontage* InMontage);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


	virtual float PlayAnimation(UAnimMontage* InAnimMontage);
};
