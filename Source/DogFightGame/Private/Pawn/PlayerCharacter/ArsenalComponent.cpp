// Dog Fight Game Code By CYM.


#include "Pawn/PlayerCharacter/ArsenalComponent.h"


// Sets default values for this component's properties
UArsenalComponent::UArsenalComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UArsenalComponent::EquipWeapon(EWeaponSlotType Slot, UWeaponBase* InWeapon)
{
	return true;
}


// Called when the game starts
void UArsenalComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
