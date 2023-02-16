#include "Interface/ActorPoolInterface/ActorPoolInterface.h"

void FActorPoolEntry::AddInstance(AActor* Instance)
{
	if (!IsValid(Instance))
	{
		return;
	}

	// Make all instance passed in inactive
	Instance->SetActorHiddenInGame(true);

	ActorPool.Push(Instance);
}

void IActorPoolInterface::ReclaimInstance(int32 InId, AActor* Instance)
{
	auto ActorPool = ActorPoolMap.FindRef(InId);
	if (ActorPool.IsValid())
	{
		ActorPool->AddInstance(Instance);
	}
	else
	{
		// Create new actor pool
		TSharedPtr<FActorPoolEntry> NewActorPool = MakeShareable(new FActorPoolEntry);
		NewActorPool->AddInstance(Instance);
		ActorPoolMap.Add(InId, NewActorPool);
	}
}

void IActorPoolInterface::ClearPool()
{
	ActorPoolMap.Empty();
}
