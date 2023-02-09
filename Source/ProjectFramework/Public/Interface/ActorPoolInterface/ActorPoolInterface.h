#pragma once

#include "CoreMinimal.h"
#include "ActorPoolInterface.generated.h"

struct FActorPoolEntry
{
public:
	void AddInstance(AActor* Instance);

	template<typename T>
	T* GetInstance()
	{
		if (ActorPool.Num() > 0)
		{
			// Stack workflow
			auto Result = ActorPool.Pop();
			while (!Result.IsValid() && ActorPool.Num() > 0)
			{
				Result = ActorPool.Pop();
			}
			if (Result.IsValid())
			{
				// Active the instance before give away
				Result->SetActorHiddenInGame(false);
				return Cast<T>(Result.Get());
			}
		}

		return nullptr;
	}

private:
	TArray<TWeakObjectPtr<AActor>> ActorPool;
};

UINTERFACE()
class PROJECTFRAMEWORK_API UActorPoolInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTFRAMEWORK_API IActorPoolInterface
{
	GENERATED_BODY()

public:
	template<typename T>
	T* GetInstanceById(int32 InId)
	{
		if (auto ActorPoolPtr = ActorPoolMap.Find(InId))
		{
			auto ActorPool = *ActorPoolPtr;
			if (auto Instance = ActorPool->GetInstance<T>())
			{
				return Instance;
			}
		}

		return nullptr;
	}

	void ReclaimInstance(int32 InId, AActor* Instance);

	void ClearPool();

protected:
	TMap<int32, TSharedPtr<FActorPoolEntry>> ActorPoolMap;
};
