#include "GameObject/GameEffect/GameEffectSequence.h"

#include "DefaultLevelSequenceInstanceData.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"

AGameEffectSequence::AGameEffectSequence(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AGameEffectSequence::PlayEffect()
{
	Super::PlayEffect();

	if (LevelSequence.IsNull())
	{
		return;
	}

	// Skip if sequence is already playing
	if (bPlaying)
	{
		return;
	}

	ALevelSequenceActor* SequenceActor = GetOrCreateSequenceActor();
	if (SequenceActor)
	{
		// Load the sequence here
		ULevelSequence* Sequence = LevelSequence.IsValid() ? LevelSequence.Get() : LevelSequence.LoadSynchronous();
		if (Sequence)
		{
			SequenceActor->SetSequence(Sequence);
			SequenceActor->GetSequencePlayer()->OnFinished.AddDynamic(this, &AGameEffectSequence::OnSequenceFinished);

			// Set myself as the level sequence origin actor
			SequenceActor->bOverrideInstanceData = true;
			if (UDefaultLevelSequenceInstanceData* InstanceData = Cast<UDefaultLevelSequenceInstanceData>(SequenceActor->DefaultInstanceData))
			{
				InstanceData->TransformOriginActor = this;
			}

			// Play the sequence
			SequenceActor->GetSequencePlayer()->Play();

			bPlaying = true;
		}
	}
}

ALevelSequenceActor* AGameEffectSequence::GetOrCreateSequenceActor()
{
	// Spawn new sequence actor if cache invalid
	if (!OwnedSequenceActor.IsValid())
	{
		OwnedSequenceActor = GetWorld()->SpawnActor<ALevelSequenceActor>();
	}

	return OwnedSequenceActor.Get();
}

void AGameEffectSequence::OnSequenceFinished()
{
	if (OwnedSequenceActor.IsValid())
	{
		OwnedSequenceActor->GetSequencePlayer()->OnFinished.RemoveDynamic(this, &AGameEffectSequence::OnSequenceFinished);
	}

	bPlaying = false;

	// Broadcast the finish event
	OnEffectFinished.Broadcast(this);
}
