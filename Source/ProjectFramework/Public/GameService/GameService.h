#pragma once

#include "CoreMinimal.h"
#include "GameInstance/DogFightGameInstance.h"
#include "GameService.generated.h"

UCLASS(Abstract)
class PROJECTFRAMEWORK_API UGameService : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override {}

	virtual void Startup() {}
	virtual void Shutdown() {}

	template <typename T>
	static T* GetGameService()
	{
		if (UWorld* CurrentWorld = GEngine->GetCurrentPlayWorld())
		{
			UDogFightGameInstance* DogFightGameInstance = Cast<UDogFightGameInstance>(CurrentWorld->GetGameInstance());
			if (DogFightGameInstance)
			{
				return Cast<T>(DogFightGameInstance->GetGameService(T::StaticClass()->GetFName()));
			}
		}

		return nullptr;
	}

	virtual TStatId GetStatId() const override
	{
		return Super::GetStatID();
	}
};
