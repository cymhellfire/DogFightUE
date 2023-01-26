#pragma once

#include "UnLuaInterface.h"
#include "GameInstance/DogFightGameInstance.h"
#include "LuaGameInstance.generated.h"

UCLASS()
class LUAINTEGRATION_API ULuaGameInstance : public UDogFightGameInstance, public IUnLuaInterface
{
	GENERATED_BODY()

public:

	virtual FString GetModuleName_Implementation() const override
	{
		return "LuaIntegration.Common.LuaGameInstance";
	}
};
