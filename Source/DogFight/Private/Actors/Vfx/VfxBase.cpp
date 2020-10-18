// Dog Fight Game Code By CYM.


#include "VfxBase.h"

// Sets default values
AVfxBase::AVfxBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AVfxBase::BeginPlay()
{
	Super::BeginPlay();

	// Set life span
	SetLifeSpan(Lifetime);
}
