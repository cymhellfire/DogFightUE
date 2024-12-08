// Dog Fight Game Code By CYM.


#include "FunctionLibrary/ActionGameCharacterFunctionLibrary.h"

#include "Components/CapsuleComponent.h"
#include "Pawn/ActionGameCharacter.h"

float UActionGameCharacterFunctionLibrary::GetCollideDistance(AActionGameCharacter* A, AActionGameCharacter* B)
{
	if (!IsValid(A) || !IsValid(B))
	{
		return 0.0f;
	}

	// Get capsule components of two characters
	auto ACollider = A->GetCapsuleComponent();
	auto BCollider = B->GetCapsuleComponent();
	if (ACollider && BCollider)
	{
		return ACollider->GetScaledCapsuleRadius() + BCollider->GetScaledCapsuleRadius();
	}
	return 0.0f;
}
