#include "DamageCalculator/DamageCalculatorBase.h"
#include "DamageSytemCommon.h"
#include "DamageReceiver/DamageReceiverComponent.h"
#include "DamageType/ExtendedDamageInstance.h"
#include "Interface/DamageReceiverActorInterface.h"
#include "Kismet/GameplayStatics.h"
#include "DamageType/ExtendedDamageType.h"

void UDamageCalculatorBase::ApplyDamage(AActor* DamagedActor, float BaseDamage, UExtendedDamageInstance* DamageInstance,
	AActor* DamageCauser, AController* Instigator)
{
	if (!IsValid(DamageInstance))
	{
		return;
	}

	// Get the receiver component
	TOptional<FExtendedDamageEvent> DamageEvent;
	if (IDamageReceiverActorInterface* DamageReceiver = Cast<IDamageReceiverActorInterface>(DamagedActor))
	{
		auto ReceiverComponent = DamageReceiver->GetDamageReceiverComponent();
		if (ReceiverComponent->IsInvincible())
		{
			return;
		}
		DamageEvent = FExtendedDamageEvent();
		DamageEvent->DamageValue = BaseDamage;
		DamageEvent->ReceiverComponent = ReceiverComponent;
		DamageEvent->DamageCauser = DamageCauser;
		DamageEvent->Instigator = Instigator;
	}

	if (DamageEvent.IsSet())
	{
		DamageInstance->PreApplyToComponent(DamageEvent.GetValue());

		DamageInstance->ApplyToComponent(DamageEvent.GetValue());
	}

	// Origin unreal behavior
	UGameplayStatics::ApplyDamage(DamagedActor, BaseDamage, Instigator, DamageCauser, DamageInstance->DamageType);

	if (DamageEvent.IsSet())
	{
		DamageInstance->PostApplyToComponent(DamageEvent.GetValue());

		// Broadcast damage event
		OnDamageEventOccured.Broadcast(DamageInstance, DamageEvent.GetValue());
	}
}
