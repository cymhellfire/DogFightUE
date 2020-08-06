// Dog Fight Game Code By CYM.


#include "StandardModePlayerPawnMovement.h"

UStandardModePlayerPawnMovement::UStandardModePlayerPawnMovement(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MaxSpeed = 5000.f;
	Acceleration = 5000.f;
	Deceleration = 5000.f;
}