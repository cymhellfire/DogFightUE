#pragma once

#include "CoreMinimal.h"
#include "GameService/LuaGameService.h"
#include "CardModifierGameService.generated.h"

class UCardModifier;

UCLASS()
class DOGFIGHT_API UCardModifierGameService : public ULuaGameService
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent)
	UCardModifier* CreateCardModifier(const FString& ScriptPath, UObject* Instigator);

	UFUNCTION(BlueprintCallable)
	UClass* GetDefaultModifierClass();

	virtual FString GetModuleName_Implementation() const override
	{
		return TEXT("DogFight.Services.CardModifierGameService");
	}

protected:
	TWeakObjectPtr<UClass> BlueprintClass;
};
