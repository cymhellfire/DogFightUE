#pragma once

#include "Widget/LuaUserWidget.h"
#include "PlayerCharacterStateWidget.generated.h"

UCLASS()
class UPlayerCharacterStateWidget : public ULuaUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category="PlayerCharacterStateWidget")
	void OnActive();

	UFUNCTION(BlueprintImplementableEvent, Category="PlayerCharacterStateWidget")
	void OnHealthChanged(int32 CurHealth, int32 MaxHealth);
};
