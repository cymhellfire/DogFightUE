#pragma once

#include "Widget/LuaUserWidget.h"
#include "DamageDisplayWidget.generated.h"

class UExtendedDamageInstance;

UCLASS()
class DOGFIGHTGAME_API UDamageDisplayWidget : public ULuaUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category="DamageDisplay")
	void AddDamageItem(const FDamageDisplayParams& DisplayParams);

	UFUNCTION(BlueprintImplementableEvent, Category="DamageDisplay")
	void OnOwnerActivated();

	UFUNCTION(BlueprintImplementableEvent, Category="DamageDisplay")
	void OnOwnerDead();
};
