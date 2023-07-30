// Dog Fight Game Code By CYM.

#include "Interface/ActionCharacterInterface/ActionCharacterInterface.h"
#include "GameFramework/Character.h"

void IActionCharacterInterface::PlayActionAnimation(UAnimMontage* InMontage)
{
	if (auto AnimComponent = GetAnimComponent())
	{
		
	}
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
