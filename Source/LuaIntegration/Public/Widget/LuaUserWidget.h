#pragma once

#include "UnLuaInterface.h"
#include "Blueprint/UserWidget.h"
#include "LuaUserWidget.generated.h"

UCLASS()
class LUAINTEGRATION_API ULuaUserWidget : public UUserWidget, public IUnLuaInterface
{
	GENERATED_BODY()
public:

	// IUnLuaInterface
	virtual FString GetModuleName_Implementation() const override
	{
		return LuaScriptPath;
	}
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, Category="LuaUserWidget")
	void PostInitialized();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LuaUserWidget")
	FString LuaScriptPath;
};
