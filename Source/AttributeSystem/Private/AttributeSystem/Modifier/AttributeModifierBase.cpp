#include "AttributeSystem/Modifier/AttributeModifierBase.h"

#include "AttributeSystem.h"
#include "AttributeSystem/Attribute/AttributeBase.h"

template <typename T>
TAttributeModifierBase<T>::TAttributeModifierBase(const FModifierCreateArgument& InArgument, T InValue)
	: ModifyFactor(InValue)
{
	bDirty = false;

	ModifiedTarget = nullptr;
}

template <typename T>
TAttributeModifierBase<T>::~TAttributeModifierBase()
{
	if (PreviousModifier.IsValid())
	{
		auto PinnedModifier = PreviousModifier.Pin();
		PinnedModifier->OnModifierDirty.Remove(PreviousModifierDirtyHandle);
	}
}

template <typename T>
void TAttributeModifierBase<T>::Apply(TWeakPtr<FAttributeBase> InAttribute)
{
	if (!InAttribute.IsValid())
	{
		return;
	}

	// Convert passed in attribute to expected type
	auto PinnedAttribute = InAttribute.Pin();
	TSharedPtr<TAttributeBase<T>> ConvertedAttribute = StaticCastSharedPtr<TAttributeBase<T>>(PinnedAttribute);

	if (ConvertedAttribute.IsValid())
	{
		ModifiedTarget = ConvertedAttribute;
	}
	else
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("[TAttributeModifierBase] AttributeBase type converting failed."));
	}
}

template <typename T>
void TAttributeModifierBase<T>::Remove()
{
	if (!ModifiedTarget.IsValid())
	{
		return;
	}

	MarkAsDirty();
}

template <typename T>
T TAttributeModifierBase<T>::GetOriginValue() const
{
	if (PreviousModifier.IsValid())
	{
		return PreviousModifier.Pin()->GetModifiedValue();
	}

	if (ModifiedTarget.IsValid())
	{
		return ModifiedTarget.Pin()->GetRawValue();
	}

	// Should never be here
	check(0);
	return 0;
}

template <typename T>
T TAttributeModifierBase<T>::GetModifiedValue()
{
	if (bDirty)
	{
		UpdateModifiedValue();
	}

	return ModifiedValue;
}

template <typename T>
void TAttributeModifierBase<T>::RegisterPreviousModifier(TWeakPtr<TAttributeModifierBase<T>> InModifier)
{
	if (!InModifier.IsValid())
	{
		return;
	}

	ClearPreviousModifier();

	PreviousModifier = InModifier;
	auto PinnedModifier = InModifier.Pin();
	PreviousModifierDirtyHandle = PinnedModifier->OnModifierDirty.AddRaw(this, &TAttributeModifierBase::OnPreviousModifierDirty);

	MarkAsDirty();
}

template <typename T>
void TAttributeModifierBase<T>::ClearPreviousModifier()
{
	if (!PreviousModifier.IsValid())
	{
		return;
	}

	// Remove callback
	auto PinnedModifier = PreviousModifier.Pin();
	PinnedModifier->OnModifierDirty.Remove(PreviousModifierDirtyHandle);

	PreviousModifier.Reset();

	MarkAsDirty();
}

template <typename T>
void TAttributeModifierBase<T>::SetFactor(T InValue)
{
	ModifyFactor = InValue;

	MarkAsDirty();
}

template <typename T>
void TAttributeModifierBase<T>::UpdateModifiedValue()
{
	ModifiedValue = GetOriginValue();

	bDirty = false;
}

template <typename T>
void TAttributeModifierBase<T>::OnPreviousModifierDirty()
{
	// Once previous modifier is marked as dirty, this modifier also need update later
	MarkAsDirty();
}

template <typename T>
void TAttributeModifierNumeric<T>::UpdateModifiedValue()
{
	using Super = TAttributeModifierBase<T>;

	const T OriginValue = Super::GetOriginValue();
	switch (OperatorType)
	{
	case EModifierOperatorType::MOT_Equal:
		Super::ModifiedValue = Super::ModifyFactor;
		break;
	case EModifierOperatorType::MOT_Addition:
		Super::ModifiedValue += Super::ModifyFactor;
		break;
	case EModifierOperatorType::MOT_Subtract:
		Super::ModifiedValue -= Super::ModifyFactor;
		break;
	case EModifierOperatorType::MOT_Multiply:
		Super::ModifiedValue *= Super::ModifyFactor;
		break;
	case EModifierOperatorType::MOT_Divide:
		// NOTE: No implicit conversion here
		Super::ModifiedValue /= Super::ModifyFactor;
		break;
	default:
		// Should never be here
		Super::ModifiedValue = OriginValue;
	}

	Super::bDirty = false;
}
