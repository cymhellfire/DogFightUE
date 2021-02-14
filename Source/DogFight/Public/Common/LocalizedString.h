#pragma once

#include "CoreMinimal.h"
#include "LocalizedString.generated.h"

#define ST_LOC_PATH			TEXT("/Game/DogFight/Localization/")

USTRUCT(BlueprintType)
struct FLocalizedString
{
	GENERATED_BODY()

	FLocalizedString()
	{
		LocalizeFile = TEXT("ST_UserInterface");
		LocalizeKey = TEXT("MissingString");
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString LocalizeKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString LocalizeFile;

	FText GetLocalizeText() const
	{
		return FText::FromStringTable(FName(*FString::Printf(TEXT("%s%s.%s"), ST_LOC_PATH, *LocalizeFile, *LocalizeFile)), LocalizeKey);
	}

	FText FormatLocalizeText(FFormatOrderedArguments FormatArgumentValues) const
	{
		return FText::Format(GetLocalizeText(), FormatArgumentValues);
	}
};
