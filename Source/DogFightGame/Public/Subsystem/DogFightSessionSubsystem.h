// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "CommonSessionSubsystem.h"
#include "DogFightSessionSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHTGAME_API UDogFightSessionSubsystem : public UCommonSessionSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Modified settings value by specified key.
	 * @param Key Name of settings to set.
	 * @param Value Value to apply.
	 */
	UFUNCTION(BlueprintCallable, Category=Session)
	void UpdateSessionSettings(const FString& Key, const FString& Value);

protected:
	virtual void HandlePostLoadMap(UWorld* World) override;

	virtual IOnlineSessionPtr GetCurrentSession();
};
