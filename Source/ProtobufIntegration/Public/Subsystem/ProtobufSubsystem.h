// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Subsystem/LuaGameInstanceSubsystem.h"
#include "ProtobufSubsystem.generated.h"

class FProtobufReaderBase;

/**
 * 
 */
UCLASS()
class PROTOBUFINTEGRATION_API UProtobufSubsystem : public ULuaGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual FString GetModuleName_Implementation() const override
	{
		return "";
	}

private:
	void LoadConfigs();

	void LoadByReader();

private:
	TMap<FString, TSharedPtr<FProtobufReaderBase>> ProtobufReaderMap;
};
