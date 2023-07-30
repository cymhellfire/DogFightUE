// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActionCharacterInterface.generated.h"

class UCharacterAnimComponent;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UActionCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface that provide functionality to perform actions.
 */
class DOGFIGHTGAME_API IActionCharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, Category="ActionCharacter")
	virtual void PlayActionAnimation(UAnimMontage* InMontage);

	virtual float GetDistanceFrom(AActor* Target);
	virtual float GetDistanceFrom(const FVector& Target);

protected:

	virtual ACharacter* GetCharacter() = 0;
	virtual UCharacterAnimComponent* GetAnimComponent() = 0;
};
