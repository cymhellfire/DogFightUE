// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Subsystem/LuaGameInstanceSubsystem.h"
#include "ProtobufSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROTOBUFINTEGRATION_API UProtobufSubsystem : public ULuaGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual FString GetModuleName_Implementation() const override
	{
		return "";
	}

private:
	void LoadConfigs();
};
