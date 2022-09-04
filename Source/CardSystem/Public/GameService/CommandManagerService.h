#pragma once

#include "Engine/DataTable.h"
#include "GameService/GameService.h"
#include "CommandManagerService.generated.h"

class UCardCommandVisual;

UCLASS(Abstract, Blueprintable)
class CARDSYSTEM_API UCommandManagerService : public UGameService
{
	GENERATED_BODY()
public:

	virtual void Startup() override;

	TArray<UCardCommandVisual*> GetAllCommandVisualItems() const
	{
		TArray<UCardCommandVisual*> Result;
		CommandVisualItemMap.GenerateValueArray(Result);
		return Result;
	}

	UCardCommandVisual* GetCommandVisual(int32 Id) const
	{
		if (CommandVisualItemMap.Contains(Id))
		{
			return CommandVisualItemMap[Id];
		}

		return nullptr;
	}

protected:

	void InitializeCommandList();

private:

	UPROPERTY(EditDefaultsOnly, Category="CommandManagerService", meta=(RowType="CardCommandTableRow"))
	UDataTable* CommandDataTable;

	UPROPERTY(Transient)
	TMap<int32, UCardCommandVisual*> CommandVisualItemMap;

};
