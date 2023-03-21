#pragma once

#include "CoreMinimal.h"
#include "UnLuaInterface.h"
#include "DelegateHelper.generated.h"

UCLASS()
class LUAINTEGRATION_API UDelegateHelper : public UObject, public IUnLuaInterface
{
	GENERATED_BODY()

public:
	void Reset();

	// Unlua interface
	virtual FString GetModuleName_Implementation() const override
	{
		return "LuaIntegration.Common.DelegateHelper";
	}

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="DelegateHelper")
	void OnReset();
};
