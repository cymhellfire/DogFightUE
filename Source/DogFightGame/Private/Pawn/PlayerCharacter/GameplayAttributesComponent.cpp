// Dog Fight Game Code By CYM.


#include "Pawn/PlayerCharacter/GameplayAttributesComponent.h"


// Sets default values for this component's properties
UGameplayAttributesComponent::UGameplayAttributesComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

