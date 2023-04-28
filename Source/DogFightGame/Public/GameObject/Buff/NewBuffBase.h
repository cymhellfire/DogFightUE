// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UnLuaInterface.h"
#include "UObject/Object.h"
#include "NewBuffBase.generated.h"

class ATopDownStylePlayerCharacter;

/**
 * 
 */
UCLASS()
class DOGFIGHTGAME_API UNewBuffBase : public UObject, public IUnLuaInterface
{
	GENERATED_BODY()

public:
	UNewBuffBase();

	UFUNCTION(BlueprintCallable, Category="Buff")
	virtual void InitBuff(const FString& InScriptPath);

	virtual FString GetModuleName_Implementation() const override
	{
		return ScriptPath;
	}

	UFUNCTION(BlueprintCallable, Category="Buff")
	void Apply(ATopDownStylePlayerCharacter* InCharacter);

	UFUNCTION(BlueprintCallable, Category="Buff")
	void Remove();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Buff")
	void LoadBuffScript(const FString& InScriptPath);

	UFUNCTION(BlueprintImplementableEvent, Category="Buff")
	void ApplyScript(ATopDownStylePlayerCharacter* InCharacter);

	UFUNCTION(BlueprintImplementableEvent, Category="Buff")
	void RemoveScript(ATopDownStylePlayerCharacter* InCharacter);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff")
	FString ScriptPath;

	UPROPERTY(Transient)
	ATopDownStylePlayerCharacter* AppliedCharacter;
};
