#include "AssetFactory/WarheadFactory.h"
#include "GameObject/Warhead/WarheadBase.h"

UWarheadFactory::UWarheadFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UWarheadBase::StaticClass();
}

UObject* UWarheadFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                           UObject* Context, FFeedbackContext* Warn)
{
	UWarheadBase* NewWarhead = NewObject<UWarheadBase>(InParent, UWarheadBase::StaticClass(), InName, Flags);
	return NewWarhead;
}
