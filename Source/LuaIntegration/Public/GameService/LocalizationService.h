#pragma once

#include "Engine/DataTable.h"
#include "GameService/GameService.h"
#include "LocalizationService.generated.h"

class UStringTable;

USTRUCT(BlueprintType)
struct FLocalizationServiceDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="LocalizationServiceData")
	TSoftObjectPtr<UStringTable> DataTable;
};

UCLASS(Abstract, Blueprintable)
class LUAINTEGRATION_API ULocalizationService : public UGameService
{
	GENERATED_BODY()

public:
	virtual void Startup() override;

	FText GetLocalizeString(const FString& InTable, const FString& InKey);

	FText GetLocalizeStringWithFormat(const FString& InTable, const FString& InKey, const TArray<FString>& ParamList);

protected:
	void LoadData();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LocalizationService")
	UDataTable* LocalizationData;

	TMap<FString, FString> LocalizationPath;
};
