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

FDamageDisplayParams UDamageService::GetDamageDisplayParameters(UExtendedDamageInstance* DamageInstance,
	const FExtendedDamageEvent& DamageEvent)
{
	FDamageDisplayParams Result;
	if (IsValid(DamageInstance))
	{
		Result.DamageId = DamageInstance->Id;
	}
	Result.DamageValue = DamageEvent.DamageValue;
	Result.OccuredLocation = IsValid(DamageEvent.ReceiverComponent) ?
		DamageEvent.ReceiverComponent->GetOwner()->GetActorLocation() : FVector::ZeroVector;

	return Result;
}

bool UDamageService::CreateDamageDisplay(const FDamageDisplayParams& DisplayParams)
{
	// Skip if target location is already used.
	if (CollisionLocation.Contains(DisplayParams.OccuredLocation))
	{
		return false;
	}

	auto DisplayActor = GetInstanceById<ADamageDisplayActor>(0);
	if (DisplayActor == nullptr)
	{
		DisplayActor = Cast<ADamageDisplayActor>(GetWorld()->SpawnActor(ADamageDisplayActor::StaticClass()));
		DisplayActor->SetWidget(CreateDamageWidget());
		DisplayActor->OnDamageDisplayActorDead.AddDynamic(this, &UDamageService::OnDamageDisplayActorDead);
		DisplayActor->OnDamageDisplayActorNoCollision.AddDynamic(this, &UDamageService::OnDamageDisplayActorNoCollision);
	}

	// Move the actor to damage event location
	DisplayActor->SetActorLocation(DisplayParams.OccuredLocation);

	// Record this location
	CollisionLocation.Add(DisplayParams.OccuredLocation);

	// Construct display parameters
	DisplayActor->SetDamageInfo(DisplayParams);
	// Active instance
	DisplayActor->Activate(2.f);
	return true;
}

void UDamageService::OnDamageDisplayActorDead(ADamageDisplayActor* Instance)
{
	Instance->Reset();
	ReclaimInstance(0, Instance);
}

void UDamageService::OnDamageDisplayActorNoCollision(ADamageDisplayActor* Instance)
{
	auto Location = Instance->GetActorLocation();
	if (CollisionLocation.Contains(Location))
	{
		CollisionLocation.Remove(Location);
	}
}
