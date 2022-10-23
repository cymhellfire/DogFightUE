#include "AttributeSystem/Modifier/AttributeModifierBase.h"
#include "AttributeSystem/Attribute/AttributeBase.h"

template <typename T>
FAttributeModifierBase<T>::FAttributeModifierBase(T InFactor)
	: ModifyFactor(InFactor)
{
	bDirty = false;

	ModifiedTarget = nullptr;
}

template <typename T>
FAttributeModifierBase<T>::~FAttributeModifierBase()
{
	if (PreviousModifier.IsValid())
	{
		auto PinnedModifier = PreviousModifier.Pin();
		PinnedModifier->OnModifierDirty.Remove(PreviousModifierDirtyHandle);
	}
}

template <typename T>
void FAttributeModifierBase<T>::Apply(TWeakPtr<FAttributeBase<T>> InAttribute)
{
	if (InAttribute == nullptr)
	{
		return;
	}

	ModifiedTarget = InAttribute;
}

template <typename T>
void FAttributeModifierBase<T>::Remove()
{
	if (!ModifiedTarget.IsValid())
	{
		return;
	}

	MarkAsDirty();
}

template <typename T>
T FAttributeModifierBase<T>::GetOriginValue() const
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
T FAttributeModifierBase<T>::GetModifiedValue()
{
	if (bDirty)
	{
		UpdateModifiedValue();
	}

	return ModifiedValue;
}

template <typename T>
void FAttributeModifierBase<T>::RegisterPreviousModifier(TWeakPtr<FAttributeModifierBase<T>> InModifier)
{
	if (!InModifier.IsValid())
	{
		return;
	}

	ClearPreviousModifier();

	PreviousModifier = InModifier;
	auto PinnedModifier = InModifier.Pin();
	PreviousModifierDirtyHandle = PinnedModifier->OnModifierDirty.AddRaw(this, &FAttributeModifierBase::OnPreviousModifierDirty);

	MarkAsDirty();
}

template <typename T>
void FAttributeModifierBase<T>::ClearPreviousModifier()
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
void FAttributeModifierBase<T>::SetFactor(T InValue)
{
	ModifyFactor = InValue;

	MarkAsDirty();
}

template <typename T>
void FAttributeModifierBase<T>::UpdateModifiedValue()
{
	ModifiedValue = GetOriginValue();

	bDirty = false;
}

template <typename T>
void FAttributeModifierBase<T>::OnPreviousModifierDirty()
{
	// Once previous modifier is marked as dirty, this modifier also need update later
	MarkAsDirty();
}

template <typename T>
void FAttributeModifierNumeric<T>::UpdateModifiedValue()
{
	using Super = FAttributeModifierBase<T>;

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
