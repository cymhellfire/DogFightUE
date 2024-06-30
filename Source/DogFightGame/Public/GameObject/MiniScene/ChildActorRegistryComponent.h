// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChildActorRegistryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOGFIGHTGAME_API UChildActorRegistryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UChildActorRegistryComponent();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void AddChildActor(AActor* InActor, const FName& ComponentName);

	void RemoveChildActor(AActor* InActor);

	bool IsChildActor(AActor* InActor) const
	{
		return ChildActorList.Contains(InActor);
	}

private:
	void DestroyAllChildren();

	USceneComponent* GetParentComponent(const FName& InName) const;

private:
	TArray<TWeakObjectPtr<AActor>> ChildActorList;
	TMap<TWeakObjectPtr<AActor>, TWeakObjectPtr<USceneComponent>> ParentComponentMap;
};
