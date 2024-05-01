#include "GameObject/GameEffect/GameEffectNiagara.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"

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

void AGameEffectNiagara::SetTarget(AActor* InTarget)
{
	Super::SetTarget(InTarget);

	if (!IsValid(InTarget))
	{
		return;
	}

	if (AttachSocketName.IsNone())
	{
		SetActorLocation(InTarget->GetActorLocation());
	}
	else
	{
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld, false);

		if (auto Character = Cast<ACharacter>(InTarget))
		{
			AttachToComponent(Character->GetMesh(), AttachRules, AttachSocketName);
		}
		else
		{
			AttachToActor(InTarget, AttachRules, AttachSocketName);
		}
	}
}

void AGameEffectNiagara::PlayEffect_Implementation()
{
	Super::PlayEffect_Implementation();

#if WITH_EDITOR
	const bool bIsRuntime = GetWorld()->WorldType == EWorldType::Game || GetWorld()->WorldType == EWorldType::PIE;
#endif

	if (NiagaraComponent)
	{
		if (IsValid(NiagaraComponent->GetAsset()))
		{
			NiagaraComponent->ActivateSystem();
			bNiagaraOn = true;
#if WITH_EDITOR
			if (bIsRuntime)
			{
				NiagaraComponent->OnSystemFinished.AddUniqueDynamic(this, &AGameEffectNiagara::OnNiagaraSystemFinished);
			}
#else
			NiagaraComponent->OnSystemFinished.AddUniqueDynamic(this, &AGameEffectNiagara::OnNiagaraSystemFinished);
#endif
		}
	}

	if (AudioComponent)
	{
		if (IsValid(AudioComponent->Sound))
		{
			AudioComponent->Play();
			bAudioOn = true;
#if WITH_EDITOR
			if (bIsRuntime)
			{
				AudioComponent->OnAudioFinished.AddUniqueDynamic(this, &AGameEffectNiagara::OnAudioFinished);
			}
#else
			AudioComponent->OnAudioFinished.AddUniqueDynamic(this, &AGameEffectNiagara::OnAudioFinished);
#endif
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
