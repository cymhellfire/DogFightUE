#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardFunctionEditorWidget.generated.h"

class FEditingCardFunction;

UCLASS(Abstract)
class UCardFunctionEditorWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void BindWithFunction(TWeakPtr<FEditingCardFunction> InFunction);
private:

	

	TWeakPtr<FEditingCardFunction> BoundFunction;
};
