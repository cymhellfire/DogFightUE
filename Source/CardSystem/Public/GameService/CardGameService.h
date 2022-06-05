#pragma once

#include "CoreMinimal.h"
#include "UnLuaInterface.h"
#include "GameService/GameService.h"
#include "CardGameService.generated.h"

class UCard;

UCLASS()
class CARDSYSTEM_API UCardGameService : public UGameService, public IUnLuaInterface
{
	GENERATED_BODY()
public:

	void UseCard(FString CardName, UObject* Instigator);

	UFUNCTION(BlueprintImplementableEvent)
	UCard* SpawnAndBindCard(UClass* Class, const FString& ScriptPath, UObject* Instigator);

	UFUNCTION(BlueprintImplementableEvent)
	void DestroyCard(UCard* InCard);

	virtual FString GetModuleName_Implementation() const override
	{
		return TEXT("CardSystem.Services.CardGameService");
	}

protected:
	TWeakObjectPtr<UClass> CardBlueprintClass; 
};
