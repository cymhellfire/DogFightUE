#include "Widget/LuaUserWidget.h"

void ULuaUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!bInitialized)
	{
		bInitialized = true;
		PostInitialized();
	}
}


