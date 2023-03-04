#pragma once

#include "CoreMinimal.h"
#include "DamageType/ExtendedDamageInstance.h"
#include "WidgetLocatorComponent.generated.h"

class UDamageReceiverComponent;
class UDamageDisplayWidget;

UCLASS(meta=(BlueprintSpawnableComponent))
class DOGFIGHTGAME_API UWidgetLocatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWidgetLocatorComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetDamageWidget(UDamageDisplayWidget* InWidget);

protected:
	void UpdateWidgetPos();

protected:
	TWeakObjectPtr<UDamageDisplayWidget> DisplayWidget;
};
