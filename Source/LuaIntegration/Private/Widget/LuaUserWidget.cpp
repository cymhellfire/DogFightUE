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

void ULuaUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

	UnInitialize();
}
