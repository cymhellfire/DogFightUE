// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UnLuaInterface.h"
#include "UnrealIntegration/UObject/AttributeModifierBasedObject.h"
#include "LuaAttributeModifierObject.generated.h"

/**
 * Base class of all modifiers bound with lua script.
 */
UCLASS()
class DOGFIGHTGAME_API ULuaAttributeModifierObject : public UAttributeModifierBasedObject, public IUnLuaInterface
{
	GENERATED_BODY()

public:

	void InitModifier(const FString& InScript);

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.AttributeModifier.AttributeModifierBase";
	}

protected:

	virtual void InitializeDescObject(UAttributeModifierDescObject* InDesc) override;

	UFUNCTION(BlueprintImplementableEvent, Category="LuaAttributeModifierObject")
	void LoadAndInitModifierScript(const FString& InScript);

	UFUNCTION(BlueprintImplementableEvent, Category="LuaAttributeModifierObject")
	void InitDescObjectScript(UAttributeModifierDescObject* InDesc);

protected:
	FString ModifierScript;
};
