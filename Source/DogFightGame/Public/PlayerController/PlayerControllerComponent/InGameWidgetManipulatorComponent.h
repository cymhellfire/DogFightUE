#pragma once
#include "Common/DamageDisplayParams.h"

#include "InGameWidgetManipulatorComponent.generated.h"

UCLASS()
class DOGFIGHTGAME_API UInGameWidgetManipulatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInGameWidgetManipulatorComponent(const FObjectInitializer& ObjectInitializer);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Notify client side create a new widget with given name.
	 * @param InWidgetName			Name of widget to create.
	 * @param bSilent				New widget will not be shown after created.
	 */
	UFUNCTION(Client, Reliable)
	void ClientAddInGameWidget(const FString& InWidgetName, bool bSilent = false);

	/**
	 * Notify client side show the widget with given name.
	 * @param InWidgetName			Name of widget to show.
	 * @param bCreateIfNoWidget		New widget will be created if no matched widget exists.
	 */
	UFUNCTION(Client, Reliable)
	void ClientShowInGameWidget(const FString& InWidgetName, bool bCreateIfNoWidget = false);

	/**
	 * Notify client side remove the widget with given name from viewport.
	 * @param InWidgetName			Name of widget to remove.
	 */
	UFUNCTION(Client, Reliable)
	void ClientRemoveInGameWidget(const FString& InWidgetName);

	/**
	 * Notify client side hide the widget with given name.
	 * @param InWidgetName			Name of widget to hide.
	 */
	UFUNCTION(Client, Reliable)
	void ClientHideInGameWidget(const FString& InWidgetName);

	/**
	 * Notify client side display a damage widget with given parameters.
	 */
	UFUNCTION(Client, Reliable)
	void ClientShowDamageWidget(const FDamageDisplayParams& DamageDisplayParams);

private:
	void ProcessDamageDisplayQueue();

	TArray<FDamageDisplayParams> DamageDisplayQueue;
};
