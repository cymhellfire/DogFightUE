// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UnLuaInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LuaGameInstanceSubsystem.generated.h"

class ULuaGameInstance;

/**
 * 
 */
UCLASS()
class LUAINTEGRATION_API ULuaGameInstanceSubsystem : public UGameInstanceSubsystem, public IUnLuaInterface
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category="LuaGameInstanceSubsystem")
	ULuaGameInstance* GetLuaGameInstance() const;

	UFUNCTION(BlueprintCallable, Category="LuaGameInstanceSubsystem")
	FString GetSubsystemName() const
	{
		return GetClass()->GetName();
	}

protected:

	UFUNCTION(BlueprintImplementableEvent, Category="LuaGameInstanceSubsystem")
	void ScriptInit();

	UFUNCTION(BlueprintImplementableEvent, Category="LuaGameInstanceSubsystem")
	void ScriptDeinit();
};
