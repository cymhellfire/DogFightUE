// Dog Fight Game Code By CYM.


#include "Actors/Weapons/WeaponActionDirectional.h"
#include "Actors/Weapons/WeaponBase.h"

UWeaponActionDirectional::UWeaponActionDirectional()
	: Super()
{
	ObstacleDetectDistance = 150.f;
	ObstacleDetectProfile = TEXT("Spectator");
}

void UWeaponActionDirectional::PrepareActionMontage()
{
	// Check four directions to decide which montage to play
	ACharacter* OwnerCharacter = OwnerWeapon->GetWeaponOwnerCharacter();
	EActionDirection ActionDirection = AD_Backward;
	FHitResult TestHitResult;
	bool bDirectionDecided = false;
	const FVector SourceLocation = OwnerCharacter->GetActorLocation();
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);

	// Check backward
	const FVector CharacterBackward = OwnerCharacter->GetActorForwardVector() * -1.f;
	if (!GetWorld()->LineTraceSingleByProfile(TestHitResult, SourceLocation, SourceLocation + CharacterBackward * ObstacleDetectDistance, ObstacleDetectProfile, QueryParams))
	{
		bDirectionDecided = true;
	}

	// Check left
	if (!bDirectionDecided)
	{
		const FVector CharacterLeft = OwnerCharacter->GetActorRightVector() * -1.f;
		if (!GetWorld()->LineTraceSingleByProfile(TestHitResult, SourceLocation, SourceLocation + CharacterLeft * ObstacleDetectDistance, ObstacleDetectProfile, QueryParams))
		{
			ActionDirection = AD_Left;
			bDirectionDecided = true;
		}
	}

	// Check right
	if (!bDirectionDecided)
	{
		const FVector CharacterRight = OwnerCharacter->GetActorRightVector();
		if (!GetWorld()->LineTraceSingleByProfile(TestHitResult, SourceLocation, SourceLocation + CharacterRight * ObstacleDetectDistance, ObstacleDetectProfile, QueryParams))
		{
			ActionDirection = AD_Right;
			bDirectionDecided = true;
		}
	}

	// If all other directions are block, use forward instead
	if (!bDirectionDecided)
	{
		ActionDirection = AD_Forward;
	}

	// Play montage based on direction
	switch(ActionDirection)
	{
	case AD_Forward:
		PlayActionMontage(ActionMontageForward);
		break;
	case AD_Backward:
		PlayActionMontage(ActionMontageBackward);
		break;
	case AD_Left:
		PlayActionMontage(ActionMontageLeft);
		break;
	case AD_Right:
		PlayActionMontage(ActionMontageRight);
		break;
	default: ;
	}
}
