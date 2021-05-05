// Dog Fight Game Code By CYM.


#include "BlueprintLibrary/DogFightGameplayLibrary.h"

#include "DogFight.h"
#include "Actors/Components/ActorTeleportComponent.h"

void UDogFightGameplayLibrary::TeleportActor(const AActor* Actor, const FVector& DestPos)
{
	checkf(IsValid(Actor), TEXT("Invalid actor to teleport."));

	// Check teleport component
	if (UActorTeleportComponent* TeleportComponent = Cast<UActorTeleportComponent>(Actor->GetComponentByClass(UActorTeleportComponent::StaticClass())))
	{
		TeleportComponent->MulticastStartTeleport(DestPos);
	}
	else
	{
		UE_LOG(LogDogFight, Error, TEXT("Cannot teleport actor [%s] without UActorTeleportComponent."), *Actor->GetName());
	}
}
