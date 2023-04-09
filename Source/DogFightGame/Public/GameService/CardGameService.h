#pragma once

#include "CoreMinimal.h"
#include "GameService/LuaGameService.h"
#include "Card/Card.h"
#include "CardGameService.generated.h"

class AStandardModePlayerCharacter;

UCLASS()
class DOGFIGHTGAME_API UCardGameService : public ULuaGameService
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	UCard* CreateCard(const FString& ScriptPath, UObject* Instigator);

	UFUNCTION(BlueprintImplementableEvent)
	void DestroyCard(UCard* InCard);

	UFUNCTION(BlueprintCallable)
	UClass* GetDefaultCardClass() const;

	virtual FString GetModuleName_Implementation() const override
	{
		return TEXT("DogFight.Services.CardService.CardGameService");
	}

protected:

	UFUNCTION()
	void OnCardFinished(ECardExecutionResult Result, UCard* Card);

	UPROPERTY(Transient)
	UCard* HoldingCard;
};
