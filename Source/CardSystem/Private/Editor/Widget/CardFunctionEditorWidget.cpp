#include "Editor/Widget/CardFunctionEditorWidget.h"

void UCardFunctionEditorWidget::BindWithFunction(TWeakPtr<FEditingCardFunction> InFunction)
{
	if (!InFunction.IsValid() || InFunction == BoundFunction)
	{
		return;
	}

	BoundFunction = InFunction;
}
