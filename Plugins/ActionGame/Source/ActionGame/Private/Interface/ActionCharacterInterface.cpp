// Dog Fight Game Code By CYM.

#include "Interface/ActionCharacterInterface.h"
#include "GameFramework/Character.h"

float IActionCharacterInterface::GetDistanceFrom(AActor* Target)
{
	if (IsValid(Target))
	{
		if (auto Character = GetCharacter())
		{
			return (Target->GetActorLocation() - Character->GetActorLocation()).Length();
		}
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
