#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardFunctionTabWidget.generated.h"

class UButton;
class UListView;
class UCardCommandEditingData;
class FEditingCardFunction;
class FEditingCardCommand;

UCLASS(Abstract)
class CARDSYSTEM_API UCardFunctionTabWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	virtual void BeginDestroy() override;

	void InitializeWithFunction(TWeakPtr<FEditingCardFunction> InFunction);

protected:

	void BindFunctionListener(FEditingCardFunction* InFunction);
	void ClearFunctionListener(FEditingCardFunction* InFunction);

	void OnCommandIdConfirmed(int32 CmdId);
	void OnFunctionCommandAdded(TWeakPtr<FEditingCardCommand> InCommand);

	UFUNCTION()
	void OnAddCommandButtonClicked();

private:
	UPROPERTY(meta=(BindWidget))
	UButton* AddCommandButton;

	UPROPERTY(meta=(BindWidget))
	UListView* FunctionCommandListView;

	UPROPERTY(Transient)
	TArray<UCardCommandEditingData*> CommandEditingDataList;

	TWeakPtr<FEditingCardFunction> EditingFunction;

	FDelegateHandle FunctionCommandAddedHandle;
};
