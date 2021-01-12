// Dog Fight Game Code By CYM.


#include "Pawns/MenuModePawn.h"

// Sets default values
AMenuModePawn::AMenuModePawn()
{
 	// Disable tick since the menu level doesn't need Pawn operations
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

// No actual input binding here since no need
void AMenuModePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
}

