// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActionCharacterInterface.generated.h"

DECLARE_MULTICAST_DELEGATE(FActionCharacterOnReachActionDistance)

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, MinimalAPI)
class UActionCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface that provide functionality to perform actions.
 */
class ACTIONGAME_API IActionCharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Play specified montage.
	 * @param InMontage		Montage asset to play.
	 * @return Length of the action montage.
	 */
	UFUNCTION(BlueprintCallable, Category="ActionCharacter")
	virtual float PlayActionAnimation(UAnimMontage* InMontage) { return 0.f; }

	/**
	 * Play specified montage with action warping activated.
	 * @param InMontage		Montage asset to play.
	 * @param TargetName	Name of target to warping with.
	 * @param TargetPos		Position assign to warping target.
	 * @return Length of the action montage.
	 */
	UFUNCTION(BlueprintCallable, Category="ActionCharacter")
	virtual float PlayActionAnimationWithWarping(UAnimMontage* InMontage, FName TargetName, const FVector& TargetPos) { return 0.f; }

	virtual void MoveToTarget(const FVector& Target, float StopDistance) = 0;

	virtual float GetDistanceFrom(AActor* Target);
	virtual float GetDistanceFrom(const FVector& Target);

	ACharacter* AsCharacter();
protected:

	virtual ACharacter* GetCharacter() = 0;

	/** Trigger when character reach the distance of action. (Need invoke from character class.) */
	//virtual void OnReachActionDistance() = 0;

public:
	/** Trigger when character reach the distance of action. (Need invoke from character class.) */
	FActionCharacterOnReachActionDistance OnReachActionDistance;
};
