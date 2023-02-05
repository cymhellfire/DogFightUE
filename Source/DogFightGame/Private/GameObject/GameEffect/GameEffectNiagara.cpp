#include "GameObject/GameEffect/GameEffectNiagara.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"

AGameEffectNiagara::AGameEffectNiagara(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create particle system component
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("ParticleSystemComponent");
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->SetAutoActivate(false);

	// Create audio component
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->SetAutoActivate(false);
}

void AGameEffectNiagara::PlayEffect()
{
	Super::PlayEffect();

	if (NiagaraComponent)
	{
		if (IsValid(NiagaraComponent->GetAsset()))
		{
			NiagaraComponent->ActivateSystem();
			bNiagaraOn = true;
			NiagaraComponent->OnSystemFinished.AddDynamic(this, &AGameEffectNiagara::OnNiagaraSystemFinished);
		}
	}

	if (AudioComponent)
	{
		if (IsValid(AudioComponent->Sound))
		{
			AudioComponent->Play();
			bAudioOn = true;
			AudioComponent->OnAudioFinished.AddDynamic(this, &AGameEffectNiagara::OnAudioFinished);
		}
	}
}

void AGameEffectNiagara::BeginPlay()
{
	Super::BeginPlay();
}

void AGameEffectNiagara::OnNiagaraSystemFinished(UNiagaraComponent* InComponent)
{
	if (InComponent != NiagaraComponent)
	{
		return;
	}

	NiagaraComponent->OnSystemFinished.RemoveDynamic(this, &AGameEffectNiagara::OnNiagaraSystemFinished);
	bNiagaraOn = false;

	CheckForFinished();
}

void AGameEffectNiagara::OnAudioFinished()
{
	AudioComponent->OnAudioFinished.RemoveDynamic(this, &AGameEffectNiagara::OnAudioFinished);
	bAudioOn = false;

	CheckForFinished();
}

void AGameEffectNiagara::CheckForFinished()
{
	if (bAudioOn || bNiagaraOn)
	{
		return;
	}

	// Broadcast finish event
	OnEffectFinished.Broadcast(this);
}
