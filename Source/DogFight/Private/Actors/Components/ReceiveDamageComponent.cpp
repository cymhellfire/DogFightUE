// Dog Fight Game Code By CYM.


#include "ReceiveDamageComponent.h"

// Sets default values for this component's properties
UReceiveDamageComponent::UReceiveDamageComponent()
{
	// No need to tick every frame
	PrimaryComponentTick.bCanEverTick = false;

}

void UReceiveDamageComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UReceiveDamageComponent, PhysicalArmor);
	DOREPLIFETIME(UReceiveDamageComponent, MagicalArmor);
	DOREPLIFETIME(UReceiveDamageComponent, ExtraArmorList);
}
