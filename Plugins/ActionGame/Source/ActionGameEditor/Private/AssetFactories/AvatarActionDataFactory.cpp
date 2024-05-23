// Dog Fight Game Code By CYM.


#include "AvatarActionDataFactory.h"
#include "DataAsset/AvatarDataAsset.h"

UAvatarActionDataFactory::UAvatarActionDataFactory()
{
	SupportedClass = UAvatarDataAsset::StaticClass();
	bCreateNew = true;
}

UObject* UAvatarActionDataFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
	EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UAvatarDataAsset>(InParent, InClass, InName, Flags, Context);
}
