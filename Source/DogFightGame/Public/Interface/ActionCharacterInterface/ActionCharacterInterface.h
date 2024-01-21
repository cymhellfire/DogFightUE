// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActionCharacterInterface.generated.h"

class UCharacterAnimComponent;

DECLARE_MULTICAST_DELEGATE(FActionCharacterOnReachActionDistance)

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
	virtual float PlayActionAnimation(UAnimMontage* InMontage);

	UFUNCTION(BlueprintCallable, Category="ActionCharacter")
	virtual float PlayActionAnimationWithWarping(UAnimMontage* InMontage, FName TargetName, const FVector& TargetPos);

	virtual void MoveToTarget(const FVector& Target, float StopDistance) = 0;

	virtual float GetDistanceFrom(AActor* Target);
	virtual float GetDistanceFrom(const FVector& Target);

	ACharacter* AsCharacter();
protected:

	virtual ACharacter* GetCharacter() = 0;
	virtual UCharacterAnimComponent* GetAnimComponent() = 0;

	/** Trigger when character reach the distance of action. (Need invoke from character class.) */
	//virtual void OnReachActionDistance() = 0;

public:
	/** Trigger when character reach the distance of action. (Need invoke from character class.) */
	FActionCharacterOnReachActionDistance OnReachActionDistance;
};
