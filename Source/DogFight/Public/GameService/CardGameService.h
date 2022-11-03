#pragma once

#include "CoreMinimal.h"
#include "LuaGameService.h"
#include "Card/Card.h"
#include "CardGameService.generated.h"

UCLASS()
class DOGFIGHT_API UCardGameService : public ULuaGameService
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
		return TEXT("DogFight.Services.CardGameService");
	}

protected:

	UFUNCTION()
	void OnCardFinished(ECardExecutionResult Result, UCard* Card);

protected:
	TWeakObjectPtr<UClass> CardBlueprintClass; 
};
