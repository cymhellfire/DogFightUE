// Dog Fight Game Code By CYM.

#include "Interface/ActionCharacterInterface/ActionCharacterInterface.h"
#include "GameFramework/Character.h"
#include "Pawn/PlayerCharacter/CharacterAnimComponent.h"

float IActionCharacterInterface::PlayActionAnimation(UAnimMontage* InMontage)
{
	if (auto AnimComponent = GetAnimComponent())
	{
		return AnimComponent->PlayAnimation(InMontage);
	}

	return 0.f;
}

float IActionCharacterInterface::PlayActionAnimationWithWarping(UAnimMontage* InMontage, FName TargetName,
	const FVector& TargetPos)
{
	if (auto AnimComponent = GetAnimComponent())
	{
		return AnimComponent->PlayAnimationWithWarping(InMontage, TargetName, TargetPos);
	}
	return 0.f;
}

float IActionCharacterInterface::GetDistanceFrom(AActor* Target)
{
	if (auto Character = GetCharacter())
	{
		return (Target->GetActorLocation() - Character->GetActorLocation()).Length();
	}
	return 0.f;
}

float IActionCharacterInterface::GetDistanceFrom(const FVector& Target)
{
	if (auto Character = GetCharacter())
	{
		return (Target - Character->GetActorLocation()).Length();
	}
	return 0.f;
}

ACharacter* IActionCharacterInterface::AsCharacter()
{
	return GetCharacter();
}
