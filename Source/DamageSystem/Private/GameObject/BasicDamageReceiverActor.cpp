#include "GameObject/BasicDamageReceiverActor.h"

#include "DamageReceiver/DamageReceiverComponent.h"

ABasicDamageReceiverActor::ABasicDamageReceiverActor()
{
	// Create static mesh component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");

	RootComponent = StaticMeshComponent;

	// Create damage receiver component
	DamageReceiverComponent = CreateDefaultSubobject<UDamageReceiverComponent>("DamageReceiver");
}
