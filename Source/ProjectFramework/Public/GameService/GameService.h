#pragma once

#include "CoreMinimal.h"
#include "GameInstance/DogFightGameInstance.h"
#include "GameService.generated.h"

UCLASS(Abstract, BlueprintType)
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
		if (UWorld* CurrentWorld = GetActiveWorld())
		{
			UDogFightGameInstance* DogFightGameInstance = Cast<UDogFightGameInstance>(CurrentWorld->GetGameInstance());
			if (DogFightGameInstance)
			{
				return Cast<T>(DogFightGameInstance->GetGameService(T::StaticClass()->GetFName()));
			}
		}

		return nullptr;
	}

	template<typename T>
	static T* GetGameServiceBySuperClass()
	{
		if (UWorld* CurrentWorld = GetActiveWorld())
		{
			UDogFightGameInstance* DogFightGameInstance = Cast<UDogFightGameInstance>(CurrentWorld->GetGameInstance());
			if (DogFightGameInstance)
			{
				return Cast<T>(DogFightGameInstance->GetGameServiceBySuperClass(T::StaticClass()));
			}
		}

		return nullptr;
	}

	static UWorld* GetActiveWorld()
	{
#if WITH_EDITOR
		// Use GWorld here to ensure simulating multiplayer game locally can work
		UWorld* CurWorld = nullptr;
		if (GWorld)
		{
			CurWorld = GWorld->GetWorld();
		}
		return CurWorld;
#else
		const FWorldContext* Context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		if (Context)
		{
			return Context->World();
		}

		return nullptr;
#endif
	}

	virtual TStatId GetStatId() const override
	{
		return Super::GetStatID();
	}
};
