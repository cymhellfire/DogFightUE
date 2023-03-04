#include "GameService/DamageService.h"

#include "Common/DogFightGameLog.h"
#include "DamageReceiver/DamageReceiverComponent.h"
#include "GameObject/WidgetActor/DamageDisplayActor.h"

void UDamageService::RegisterDamageType(int32 Id, UExtendedDamageInstance* Instance)
{
	if (DamageTypeMap.Contains(Id))
	{
		UE_LOG(LogDogFightGame, Error, TEXT("[DamageService] Duplicated damage config id detected: %d"), Id);
		return;
	}

	DamageTypeMap.Add(Id, Instance);
}

UExtendedDamageInstance* UDamageService::GetDamageInstanceById(int32 InId) const
{
	if (DamageTypeMap.Contains(InId))
	{
		return DamageTypeMap[InId];
	}
	return nullptr;
}

void UDamageService::CreateDamageDisplay(UExtendedDamageInstance* DamageInstance,
	const FExtendedDamageEvent& DamageEvent)
{
	auto DisplayActor = GetInstanceById<ADamageDisplayActor>(0);
	if (DisplayActor == nullptr)
	{
		DisplayActor = Cast<ADamageDisplayActor>(GetWorld()->SpawnActor(ADamageDisplayActor::StaticClass()));
		DisplayActor->SetWidget(CreateDamageWidget());
		DisplayActor->OnDamageDisplayActorDead.AddDynamic(this, &UDamageService::OnDamageDisplayActorDead);
	}

	// Move the actor to damage event location
	auto DamageLoc = IsValid(DamageEvent.ReceiverComponent) ?
		DamageEvent.ReceiverComponent->GetOwner()->GetActorLocation() : FVector::ZeroVector;
	DisplayActor->SetActorLocation(DamageLoc);
	DisplayActor->SetDamageInfo(DamageInstance, DamageEvent);
	// Active instance
	DisplayActor->Activate(2.f);
}

void UDamageService::OnDamageDisplayActorDead(ADamageDisplayActor* Instance)
{
	Instance->Reset();
	ReclaimInstance(0, Instance);
}
