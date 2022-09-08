#pragma once

#include "CoreMinimal.h"
#include "EditorSelectableListItemBase.h"
#include "EditorTabItemBase.generated.h"

class UButton;

UCLASS(Abstract)
class UEditorTabItemBase : public UEditorSelectableListItemBase
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	virtual void BeginDestroy() override;

protected:
	UFUNCTION()
	virtual void OnCloseTabButtonClicked();
private:
	UPROPERTY(meta=(BindWidget))
	UButton* CloseTabButton;
};
