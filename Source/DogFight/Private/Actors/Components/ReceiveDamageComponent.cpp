// Dog Fight Game Code By CYM.


#include "Actors/Components/ReceiveDamageComponent.h"

// Sets default values for this component's properties
UReceiveDamageComponent::UReceiveDamageComponent()
{
	// No need to tick every frame
	PrimaryComponentTick.bCanEverTick = false;

}

bool UReceiveDamageComponent::AddExtraArmor(FActorArmor& NewArmor)
{
	if (!ExtraArmorList.Contains(NewArmor))
	{
		ExtraArmorList.Add(NewArmor);
		return true;
	}

	UE_LOG(LogDogFight, Warning, TEXT("Failed to add duplicated armor to actor [%s]."), *GetOwner()->GetName());
	return false;
}


bool UReceiveDamageComponent::RemoveExtraArmor(FActorArmor& TargetArmor)
{
	if (ExtraArmorList.Contains(TargetArmor))
	{
		ExtraArmorList.Remove(TargetArmor);
		return true;
	}

	UE_LOG(LogDogFight, Warning, TEXT("Target armor not found."));
	return false;
}

void UReceiveDamageComponent::AddInvincibleFlags(int32 Flags)
{
	MERGE_FLAGS(InvincibleFlags, Flags);
}

void UReceiveDamageComponent::RemoveInvincibleFlags(int32 Flags)
{
	REMOVE_FLAGS(InvincibleFlags, Flags);
}

void UReceiveDamageComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UReceiveDamageComponent, InvincibleFlags);
	DOREPLIFETIME(UReceiveDamageComponent, PhysicalArmor);
	DOREPLIFETIME(UReceiveDamageComponent, MagicalArmor);
	DOREPLIFETIME(UReceiveDamageComponent, ExtraArmorList);
}
