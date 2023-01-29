#pragma once

#include "GameService/LuaGameService.h"
#include "GameWidgetService.generated.h"

UCLASS()
class DOGFIGHTGAME_API UGameWidgetService : public ULuaGameService
{
	GENERATED_BODY()

public:
	/**
	 * Add widget with specified name to viewport.
	 * @param InWidgetName		Name of widget to add.
	 * @param bSilent			Not display new created widget after creation.
	 * @return			Whether the operation succeed.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	bool AddWidget(const FString& InWidgetName, bool bSilent = false);

	/**
	 * Show widget with specified name in viewport.
	 * @param InWidgetName		Name of widget to show.
	 * @param bCreate			Create widget if no existing instance.
	 * @return			Whether the operation succeed.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	bool ShowWidget(const FString& InWidgetName, bool bCreate = false);

	/**
	 * Remove widget with specified name from viewport and destroy it.
	 * @param InWidgetName		Name of widget to remove.
	 * @return			Whether the operation succeed.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	bool RemoveWidget(const FString& InWidgetName);

	/**
	 * Hide widget with specified name in viewport.
	 * @param InWidgetName		Name of widget to hide.
	 * @return			Whether the operation succeed.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	bool HideWidget(const FString& InWidgetName);

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Services.GameWidgetService";
	}
};
