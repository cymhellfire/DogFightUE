// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "BuffCommonDef.h"
#include "UnLuaInterface.h"
#include "UObject/Object.h"
#include "NewBuffBase.generated.h"

class ATopDownStylePlayerCharacter;

DECLARE_MULTICAST_DELEGATE(FBuffDoCheckFinishedSignature);

/**
 * 
 */
UCLASS()
class DOGFIGHTGAME_API UNewBuffBase : public UObject, public IUnLuaInterface
{
	GENERATED_BODY()

public:
	UNewBuffBase();

	/**
	 * Initialize this buff with given script.
	 * @param InScriptPath		Path of logic script.
	 */
	UFUNCTION(BlueprintCallable, Category="Buff")
	virtual void InitBuff(const FString& InScriptPath);

	/**
	 * Mark this buff is finish and remove from applied character.
	 */
	UFUNCTION(BlueprintCallable, Category="Buff")
	virtual void Finish();

	virtual FString GetModuleName_Implementation() const override
	{
		return ScriptPath;
	}

	/**
	 * Apply this buff to given character.
	 * @param InCharacter		Character that this buff apply to.
	 */
	void Apply(ATopDownStylePlayerCharacter* InCharacter);

	/**
	 * Remove this buff from current applied character.
	 */
	void Remove();

	/**
	 * Transfer this buff 
	 * @param NewCharacter		New character instance this buff will transfer to.
	 */
	UFUNCTION(BlueprintCallable, Category="Buff")
	void TransferTo(ATopDownStylePlayerCharacter* NewCharacter);

	UFUNCTION(BlueprintCallable, Category="Buff")
	void DoBuffCheck(TEnumAsByte<EBuffCheckType::Type> InCheckType);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Buff")
	void LoadBuffScript(const FString& InScriptPath);

	UFUNCTION(BlueprintImplementableEvent, Category="Buff")
	void OnBuffInitScript();

	UFUNCTION(BlueprintImplementableEvent, Category="Buff")
	void ApplyScript(ATopDownStylePlayerCharacter* InCharacter);

	UFUNCTION(BlueprintImplementableEvent, Category="Buff")
	void RemoveScript(ATopDownStylePlayerCharacter* InCharacter);

	UFUNCTION(BlueprintImplementableEvent, Category="Buff")
	void TransferToScript(ATopDownStylePlayerCharacter* NewCharacter);

	UFUNCTION(BlueprintImplementableEvent, Category="Buff")
	void OnBuffFinishScript();

	void DelayFinishExpired();

	UFUNCTION(BlueprintImplementableEvent, Category="Buff")
	void OnDoBuffCheckScript(EBuffCheckType::Type InCheckType);

	/**
	 * Finish the do check progress.
	 */
	UFUNCTION(BlueprintCallable, Category="Buff")
	void FinishDoCheck();

public:
	FBuffDoCheckFinishedSignature OnDoCheckFinished;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff")
	FString ScriptPath;

	UPROPERTY(Transient)
	ATopDownStylePlayerCharacter* AppliedCharacter;

	FTimerHandle DelayFinishTimer;
};
