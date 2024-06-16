// Dog Fight Game Code By CYM.


#include "GameObject/MiniScene/ChildActorRegistryComponent.h"

#include "Common/DogFightGameLog.h"


// Sets default values for this component's properties
UChildActorRegistryComponent::UChildActorRegistryComponent()
{
}

void UChildActorRegistryComponent::AddChildActor(AActor* InActor, const FName& ComponentName)
{
	if (IsValid(InActor))
	{
		if (IsChildActor(InActor))
			return;

		// Find parent component
		if (auto ParentComponent = GetParentComponent(ComponentName))
		{
			InActor->AttachToComponent(ParentComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			ParentComponentMap.Add(InActor, ParentComponent);
			ChildActorList.AddUnique(InActor);
			DFLog(LogDogFightGame, TEXT("Actor %s add to component %s."), *InActor->GetName(), *ComponentName.ToString());
		}
		else
		{
			DFLogW(LogDogFightGame, TEXT("Failed to find parent component %s."), *ComponentName.ToString());
		}
	}
}

void UChildActorRegistryComponent::RemoveChildActor(AActor* InActor)
{
	if (IsValid(InActor))
	{
		if (!IsChildActor(InActor))
			return;

		InActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		if (auto RecordPtr = ParentComponentMap.Find(InActor))
		{
			ParentComponentMap.Remove(InActor);
		}
		ChildActorList.Remove(InActor);
		DFLog(LogDogFightGame, TEXT("Actor %s removed."), *InActor->GetName());
	}
}

void UChildActorRegistryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Destroy all children actors when end play
	DestroyAllChildren();
}

void UChildActorRegistryComponent::DestroyAllChildren()
{
	for (auto Child : ChildActorList)
	{
		if (Child.IsValid())
		{
			Child->Destroy();
		}
	}
	ChildActorList.Empty();
	ParentComponentMap.Empty();
}

USceneComponent* UChildActorRegistryComponent::GetParentComponent(const FName& InName) const
{
	if (auto Owner = GetOwner())
	{
		TInlineComponentArray<USceneComponent*, 32u> CandidateComponents;
		Owner->GetComponents(CandidateComponents);

		// Match by name
		for (auto Candidate : CandidateComponents)
		{
			if (Candidate->GetFName() == InName)
			{
				return Candidate;
			}
		}
	}
	return nullptr;
}
