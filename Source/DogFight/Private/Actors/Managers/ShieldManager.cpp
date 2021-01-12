// Dog Fight Game Code By CYM.


#include "Actors/Managers/ShieldManager.h"
#include "Actors/Shield/ShieldBase.h"

// Sets default values
AShieldManager::AShieldManager()
{
 	// Disable tick
	PrimaryActorTick.bCanEverTick = false;

}

void AShieldManager::RegisterShield(AShieldBase* NewShield)
{
	if (ShieldList.Contains(NewShield))
		return;

	UE_LOG(LogInit, Log, TEXT("Register shield %s"), *NewShield->GetName());
	ShieldList.Add(NewShield);
	OnShieldRegistered.Broadcast(NewShield);
}

void AShieldManager::UnregisterShield(AShieldBase* Shield)
{
	if (!ShieldList.Contains(Shield))
		return;

	UE_LOG(LogInit, Log, TEXT("Unregister shield %s"), *Shield->GetName());
	ShieldList.Remove(Shield);
	OnShieldUnregistered.Broadcast(Shield);
}

