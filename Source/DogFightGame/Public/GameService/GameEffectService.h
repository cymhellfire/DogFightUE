#pragma once

#include "Engine/DataTable.h"
#include "GameService/LuaGameService.h"
#include "GameEffectService.generated.h"

class UDataTable;
class AGameEffectBase;

USTRUCT(BlueprintType)
struct FGameEffectConfigDataRow : public FTableRowBase
{
	GENERATED_BODY()

	FGameEffectConfigDataRow()
		: EffectId(-1)
		, PreloadCount(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameEffectConfig")
	int32 EffectId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameEffectConfig")
	TSoftClassPtr<AGameEffectBase> EffectResource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameEffectConfig")
	int32 PreloadCount;
};

/**
 * Actor pool for managing all preload instances of one game effect.
 */
struct FGameEffectActorPool
{
public:
	void AddInstance(AGameEffectBase* Instance);

	AGameEffectBase* GetInstance();

protected:
	TArray<TWeakObjectPtr<AGameEffectBase>> GameEffectPool;
};

USTRUCT(BlueprintType)
struct FGameEffectUtilsDesc
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GameEffectUtilsDesc")
	int32 EffectId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GameEffectUtilsDesc")
	FString EffectName;
};

UCLASS(Abstract, Blueprintable)
class DOGFIGHTGAME_API UGameEffectService : public ULuaGameService
{
	GENERATED_BODY()

public:
	virtual void Startup() override;
	virtual void Shutdown() override;

	/**
	 * Add a new effect id for preload when enter new game.
	 * @param EffectId			Id of effect config to preload.
	 */
	void AddPreloadEffect(int32 EffectId);

	/**
	 * Spawn effect with given id at specified position.
	 * @param EffectId			Id of effect to spawn.
	 * @param Pos				Position to place the effect.
	 * @param Rot				Rotation of the effect.
	 */
	UFUNCTION(BlueprintCallable, Category="GameEffectService")
	AGameEffectBase* SpawnEffectAtPos(int32 EffectId, FVector Pos, FRotator Rot);

	/**
	 * Get the game effect description for utility scene.
	 * @return					List of all game effect description struct.
	 */
	UFUNCTION(BlueprintCallable, Category="GameEffectService")
	TArray<FGameEffectUtilsDesc> GetAllEffectUtilDesc() const;

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Services.GameEffectService";
	}

protected:
	void LoadConfigTable();

	void PostLoadMap(UWorld* World);

	void PreloadGameEffects();

	/**
	 * Clear the preload effect caches.
	 */
	void ClearEffectCache();

	AGameEffectBase* SpawnEffectWithConfig(FGameEffectConfigDataRow* Config, const FTransform& SpawnTrans = FTransform::Identity);

	UFUNCTION()
	void OnGameEffectFinished(AGameEffectBase* InEffect);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameEffectService")
	UDataTable* GameEffectTable;

protected:
	TMap<int32, FGameEffectConfigDataRow*> EffectDataMap;

	// ---------------------- Preload section ------------------------------
	TArray<int32> PreloadEffectIdList;
	TMap<int32, TSharedPtr<FGameEffectActorPool>> EffectCacheMap;

	FDelegateHandle PostLoadMapHandle;
};
