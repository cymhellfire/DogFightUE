// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UnLuaInterface.h"
#include "UnrealIntegration/UObject/AttributeModifierBasedObject.h"
#include "LuaAttributeModifierObject.generated.h"

UENUM(BlueprintType)
enum class ELuaAttributeModifierType : uint8
{
	AMT_CharacterStatus,
	AMT_CardAttribute,
};

/**
 * Base class of all modifiers bound with lua script.
 */
UCLASS()
class DOGFIGHTGAME_API ULuaAttributeModifierObject : public UAttributeModifierBasedObject, public IUnLuaInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="LuaAttributeModifierObject")
	virtual FText GetEffectString() const;

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.AttributeModifier.AttributeModifierBase";
	}

protected:

	virtual void InitializeDescObject(UAttributeModifierDescObject* InDesc) override;

	UFUNCTION(BlueprintImplementableEvent, Category="LuaAttributeModifierObject")
	void LoadAndInitModifierScript(int32 ConfigId);

	UFUNCTION(BlueprintImplementableEvent, Category="LuaAttributeModifierObject")
	void InitDescObjectScript(UAttributeModifierDescObject* InDesc);

protected:
	UPROPERTY(BlueprintReadOnly, Category="AttributeModifierBaseObject")
	ELuaAttributeModifierType ModifierType;

	FString ModifierScript;
};
