// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Subsystem/LuaGameInstanceSubsystem.h"
#include "Common/GenericMessageDef.h"
#include "PopupMessageSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHTGAME_API UPopupMessageSubsystem : public ULuaGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/**
	 * Add a pending message which will be popped up when specified level is loaded.
	 * @param LevelName			Waiting level until message popping up.
	 * @param InMessage			Message to enqueue.
	 */
	UFUNCTION(BlueprintCallable, Category="PopupMessageSubsystem")
	void AddPendingMessage(const FString& LevelName, const FGenericMessage& InMessage);

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Subsystem.PopupMessageSubsystem";
	}

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="PopupMessageSubsystem")
	void HandleMessagePopup(const FGenericMessage& InMessage);

	void OnPostLoadMapWithWorld(UWorld* World);

protected:
	TArray<FString> PendingKey;

	TMap<FString, TArray<FGenericMessage>> PendingMessageMap;
};
