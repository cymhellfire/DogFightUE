#pragma once

#include "LuaGameService.h"
#include "DelegateHelperService.generated.h"

class UDelegateHelper;

UCLASS()
class LUAINTEGRATION_API UDelegateHelperService : public ULuaGameService
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="DelegateHandleService")
	UDelegateHelper* GetDelegateHelper();

	UFUNCTION(BlueprintCallable, Category="DelegateHandleService")
	void ReleaseDelegateHelper(UDelegateHelper* InHelper);

	virtual FString GetModuleName_Implementation() const override
	{
		return "LuaIntegration.Services.DelegateHelperService";
	}

private:
	UPROPERTY(Transient)
	TArray<UDelegateHelper*> FreeHelpers;

	UPROPERTY(Transient)
	TArray<UDelegateHelper*> UsingHelper;
};
