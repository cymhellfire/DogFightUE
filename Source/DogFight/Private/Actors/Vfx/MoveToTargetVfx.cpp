// Dog Fight Game Code By CYM.


#include "Actors/Vfx/MoveToTargetVfx.h"
#include "NiagaraComponent.h"

AMoveToTargetVfx::AMoveToTargetVfx()
	: Super()
{
	// Create the niagara system component
	NiagaraParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraSystem"));
	NiagaraParticleSystem->SetupAttachment(RootComponent);

	// Active after set target
	NiagaraParticleSystem->bAutoActivate = false;
}

void AMoveToTargetVfx::SetTargetLocation(FVector NewTarget)
{
	NiagaraParticleSystem->SetVectorParameter(TEXT("TargetPos"), NewTarget);
	NiagaraParticleSystem->Activate();
}
