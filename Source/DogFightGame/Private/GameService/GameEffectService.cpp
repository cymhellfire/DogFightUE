#include "GameService/GameEffectService.h"

#include "Common/DogFightGameLog.h"
#include "GameObject/GameEffect/GameEffectBase.h"

void UGameEffectService::Startup()
{
	Super::Startup();

	// Load global data table
	LoadConfigTable();

	// Register callback for load map event
	PostLoadMapHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UGameEffectService::PostLoadMap);
}

void UGameEffectService::Shutdown()
{
	Super::Shutdown();

	// Unregister callback for load map event
	FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(PostLoadMapHandle);
}

AGameEffectBase* UGameEffectService::SpawnEffectAtPos(int32 EffectId, FVector Pos, FRotator Rot)
{
	// Check the config id
	if (!EffectDataMap.Contains(EffectId))
	{
		UE_LOG(LogDogFightGame, Error, TEXT("[GameEffectService] Invalid game effect config id: %d"), EffectId);
		return nullptr;
	}

	// Get instance from existing pool first
	AGameEffectBase* Effect = GetInstanceById<AGameEffectBase>(EffectId);
	if (Effect)
	{
		Effect->SetActorLocation(Pos);
		Effect->SetActorRotation(Rot);
	}
	// Spawn new instance
	else
	{
		Effect = SpawnEffectWithConfig(EffectDataMap[EffectId], FTransform(Rot.Quaternion(), Pos));
	}

	// Play the effect
	if (IsValid(Effect))
	{
		Effect->PlayEffect();
	}

	return Effect;
}

TArray<FGameEffectUtilsDesc> UGameEffectService::GetAllEffectUtilDesc() const
{
	TArray<FGameEffectUtilsDesc> Result;
	for (auto Pair : EffectDataMap)
	{
		if (auto EffectCfg = Pair.Value)
		{
			if (EffectCfg->EffectResource.IsNull())
			{
				continue;
			}

			UClass* EffectClass = EffectCfg->EffectResource.IsValid() ? EffectCfg->EffectResource.Get() :
				EffectCfg->EffectResource.LoadSynchronous();
			if (auto CDO = Cast<AGameEffectBase>(EffectClass->ClassDefaultObject))
			{
				FGameEffectUtilsDesc NewDesc;
				NewDesc.EffectId = Pair.Key;
				NewDesc.EffectName = CDO->EffectName;

				Result.Emplace(NewDesc);
			}
		}
	}

	return Result;
}

void UGameEffectService::AddPreloadEffect(int32 EffectId)
{
	PreloadEffectIdList.AddUnique(EffectId);
}

void UGameEffectService::LoadConfigTable()
{
	if (!IsValid(GameEffectTable))
	{
		UE_LOG(LogDogFightGame, Error, TEXT("[GameEffectService] No game effect data table assgined."));
		return;
	}

	TArray<FGameEffectConfigDataRow*> AllConfigs;
	GameEffectTable->GetAllRows<FGameEffectConfigDataRow>("", AllConfigs);

	// Record all data into map
	for (auto Config : AllConfigs)
	{
		// Skip invalid config
		if (Config->EffectId < 0 || Config->EffectResource.IsNull())
		{
			continue;
		}

		EffectDataMap.Add(Config->EffectId, Config);
	}
}

void UGameEffectService::PostLoadMap(UWorld* World)
{
	// Clear the out of date cache
	ClearEffectCache();

	// Try to preload game effect after map loaded
	PreloadGameEffects();
}

void UGameEffectService::PreloadGameEffects()
{
	if (PreloadEffectIdList.Num() == 0)
	{
		return;
	}

	// Preload all recorded effect
	for (auto EffectId : PreloadEffectIdList)
	{
		if (auto ConfigPtr = EffectDataMap.Find(EffectId))
		{
			auto Config = *ConfigPtr;
			auto PreloadCount = Config->PreloadCount;

			for (int32 i = 0; i < PreloadCount; ++i)
			{
				if (auto NewEffect = SpawnEffectWithConfig(Config))
				{
					// Push new instance to pool
					ReclaimInstance(EffectId, NewEffect);
				}
				else
				{
					// Break the spawn loop if any failure occured
					break;
				}
			}
		}
	}

	UE_LOG(LogDogFightGame, Log, TEXT("[GameEffectService] Game effect preload finished. %d type effects in total."), PreloadEffectIdList.Num());
	PreloadEffectIdList.Empty();
}

void UGameEffectService::ClearEffectCache()
{
	ClearPool();
}

AGameEffectBase* UGameEffectService::SpawnEffectWithConfig(FGameEffectConfigDataRow* Config, const FTransform& SpawnTrans)
{
	if (Config == nullptr)
	{
		return nullptr;
	}

	int32 EffectId = Config->EffectId;
	UClass* EffectClass = Config->EffectResource.IsValid() ? Config->EffectResource.Get() :
		Config->EffectResource.LoadSynchronous();

	AGameEffectBase* Result = nullptr;
	if (IsValid(EffectClass))
	{
		if (auto NewEffect = Cast<AGameEffectBase>(GetWorld()->SpawnActor(EffectClass, &SpawnTrans)))
		{
			NewEffect->EffectId = EffectId;
			// Register finish callback
			NewEffect->OnEffectFinished.AddDynamic(this, &UGameEffectService::OnGameEffectFinished);

			Result = NewEffect;
		}
		else
		{
			UE_LOG(LogDogFightGame, Error, TEXT("[GameEffectService] Failed to spawn effect actor with id: %d"), EffectId);
		}
	}
	else
	{
		UE_LOG(LogDogFightGame, Error, TEXT("[GameEffectService] Invalid effect class for preload. Config Id: %d"), EffectId);
	}

	return Result;
}

void UGameEffectService::OnGameEffectFinished(AGameEffectBase* InEffect)
{
	if (!IsValid(InEffect))
	{
		return;
	}

	// Push the instance back to pool
	ReclaimInstance(InEffect->EffectId, InEffect);
}
