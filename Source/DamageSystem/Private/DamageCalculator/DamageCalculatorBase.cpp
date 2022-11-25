#include "DamageCalculator/DamageCalculatorBase.h"
#include "DamageSytemCommon.h"
#include "DamageReceiver/DamageReceiverComponent.h"
#include "DamageType/ExtendedDamageInstance.h"
#include "Interface/DamageReceiverActorInterface.h"
#include "Kismet/GameplayStatics.h"
#include "DamageType/ExtendedDamageType.h"

void UDamageCalculatorBase::ApplyDamage(AActor* DamagedActor, float BaseDamage, FName DamageTypeName, AActor* DamageCauser, AController* Instigator)
{
	auto DamageInstance = GetDamageInstanceByName(DamageTypeName);
	if (!DamageInstance)
	{
		UE_LOG(LogDamageSystem, Error, TEXT("[DamageCalculatorBase] Cannot find damage instance with name %s."), *DamageTypeName.ToString());
		return;
	}

	// Get the receiver component
	TOptional<FExtendedDamageEvent> DamageEvent;
	if (IDamageReceiverActorInterface* DamageReceiver = Cast<IDamageReceiverActorInterface>(DamagedActor))
	{
		auto ReceiverComponent = DamageReceiver->GetDamageReceiverComponent();
		DamageEvent = FExtendedDamageEvent();
		DamageEvent->DamageValue = BaseDamage;
		DamageEvent->ReceiverComponent = ReceiverComponent;
		DamageEvent->DamageCauser = DamageCauser;
		DamageEvent->Instigator = Instigator;
	}

	if (DamageEvent.IsSet())
	{
		DamageInstance->PreApplyToComponent(DamageEvent.GetValue());
	}

	// Origin unreal behavior
	UGameplayStatics::ApplyDamage(DamagedActor, BaseDamage, Instigator, DamageCauser, DamageInstance->DamageType);

	if (DamageEvent.IsSet())
	{
		DamageInstance->PostApplyToComponent(DamageEvent.GetValue());
	}
}

UExtendedDamageInstance* UDamageCalculatorBase::GetDamageInstanceByName(FName InName) const
{
	auto Result = DamageInstanceTable.Find(InName);
	return Result ? *Result : nullptr;
}
