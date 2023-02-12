#include "GameService/ProjectileService.h"

#include "Common/DogFightGameLog.h"
#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"
#include "GameObject/Projectile/NewProjectileBase.h"

UProjectileService::UProjectileService()
{
	ActorPoolPosition = FVector(0, 0, -20000);
}

void UProjectileService::Startup()
{
	Super::Startup();

	// Load all projectile config at startup
	LoadConfig();
}

ANewProjectileBase* UProjectileService::SpawnProjectileAtPos(int32 ProjectileId, FVector Pos, FRotator Rot)
{
	auto ConfigPtr = ProjectileConfigMap.Find(ProjectileId);
	if (ConfigPtr == nullptr)
	{
		return nullptr;
	}

	// Get instance from actor pool first
	ANewProjectileBase* Result = GetInstanceById<ANewProjectileBase>(ProjectileId);

	// Create new instance if no one available
	if (Result == nullptr)
	{
		auto Config = *ConfigPtr;
		if (Config.IsValid())
		{
			Result = SpawnNewProjectileInstance(Config.Get());
		}
	}

	// Ensure the projectile placed at the desired position
	if (Result)
	{
		Result->SetActorLocation(Pos);
		Result->SetActorRotation(Rot);
	}

	return Result;
}

TArray<FProjectileUtilsDesc> UProjectileService::GetAllProjectileUtilsDesc() const
{
	TArray<FProjectileUtilsDesc> Result;
	for (auto Config : ProjectileConfigMap)
	{
		if (Config.Value.IsValid())
		{
			FProjectileUtilsDesc NewDesc;
			NewDesc.Id = Config.Value->Id;
			NewDesc.DisplayName = GetDefineNameOfProjectile(NewDesc.Id);

			Result.Emplace(NewDesc);
		}
	}
	return Result;
}

void UProjectileService::LoadConfig()
{
	auto ConfigList = GatherProjectileConfigData();
	for (auto Config : ConfigList)
	{
		if (Config.ProjectileId < 0 || Config.ResourcePath.IsEmpty())
		{
			continue;
		}

		UClass* ProjectileClass = ULuaIntegrationFunctionLibrary::LoadClassByPath(Config.ResourcePath);
		if (ProjectileClass == nullptr)
		{
			UE_LOG(LogDogFightGame, Error, TEXT("[ProjectileService] Failed to load projectile class: %s"), *Config.ResourcePath);
			continue;
		}

		// Create runtime version
		TSharedPtr<FRuntimeProjectileConfig> NewConfig = MakeShareable(new FRuntimeProjectileConfig);
		NewConfig->Id = Config.ProjectileId;
		NewConfig->Class = ProjectileClass;
		NewConfig->ScriptPath = Config.ScriptPath;

		ProjectileConfigMap.Add(Config.ProjectileId, NewConfig);
	}
}

ANewProjectileBase* UProjectileService::SpawnNewProjectileInstance(const FRuntimeProjectileConfig* InConfig)
{
	if (IsValid(InConfig->Class))
	{
		if (auto NewInstance = GetWorld()->SpawnActor<ANewProjectileBase>(InConfig->Class))
		{
			NewInstance->Id = InConfig->Id;
			NewInstance->SetProjectileScriptModule(InConfig->ScriptPath);
			NewInstance->OnProjectileDead.AddDynamic(this, &UProjectileService::OnProjectileDead);
			return NewInstance;
		}
	}

	return nullptr;
}

void UProjectileService::OnProjectileDead(ANewProjectileBase* Projectile)
{
	if (IsValid(Projectile))
	{
		// Reset projectile before reclaim it
		Projectile->Reset();
		ReclaimInstance(Projectile->Id, Projectile);
		Projectile->SetActorLocation(ActorPoolPosition);
	}
}
