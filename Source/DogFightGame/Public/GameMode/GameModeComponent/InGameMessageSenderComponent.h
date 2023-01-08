#pragma once

#include "CoreMinimal.h"
#include "InGameMessageSenderComponent.generated.h"

class ATopDownStyleGameMode;

/**
 * Component that handles all in-game messages sending.
 */
UCLASS()
class DOGFIGHTGAME_API UInGameMessageSenderComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UInGameMessageSenderComponent(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeComponent() override;

	void BroadcastTitleMessage(const FText& Message);

protected:
	TWeakObjectPtr<ATopDownStyleGameMode> ParentGameMode;
};
