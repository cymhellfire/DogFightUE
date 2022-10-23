#include "AttributeSystem/Attribute/AttributeBase.h"
#include "AttributeSystem/Modifier/AttributeModifierBase.h"

template <typename T>
T FAttributeBase<T>::GetValue() const
{
	// Get the modified value if any modifier available
	auto LastModifier = GetLastModifier();
	if (LastModifier.IsValid())
	{
		auto PinnedModifier = LastModifier.Pin();
		return PinnedModifier->GetModifiedValue();
	}

	return GetRawValue();
}

template <typename T>
void FAttributeBase<T>::AddModifier(TSharedPtr<FAttributeModifierBase<T>> InModifier)
{
	if (InModifier == nullptr || ModifierList.Contains(InModifier))
	{
		return;
	}

	InModifier->Apply(SharedThis(this));
	// Record the previous modifier
	auto LastModifier = GetLastModifier();
	if (LastModifier.IsValid())
	{
		InModifier->RegisterPreviousModifier(LastModifier);
	}
	ModifierList.Add(InModifier);
}

template <typename T>
void FAttributeBase<T>::RemoveModifier(TSharedPtr<FAttributeModifierBase<T>> InModifier)
{
	if (InModifier == nullptr || !ModifierList.Contains(InModifier))
	{
		return;
	}

	int32 Index = ModifierList.IndexOfByKey(InModifier);
	ensure(Index != INDEX_NONE);

	// Mark modifier as dirty to notify those modifiers behind it
	InModifier->MarkAsDirty();

	if (Index == 0)
	{
		// Make the second modifier as the head if possible
		if (ModifierList.Num() > 2)
		{
			TSharedPtr<FAttributeModifierBase<T>> NewHead = ModifierList[1];
			// Clear the previous modifier
			NewHead->ClearPreviousModifier();
		}
	}
	else if (Index < ModifierList.Num() - 1)
	{
		// Handle the removal if modifier is in the middle
		TSharedPtr<FAttributeModifierBase<T>> PreviousModifier = ModifierList[Index - 1];
		TSharedPtr<FAttributeModifierBase<T>> NextModifier = ModifierList[Index + 1];

		NextModifier->RegisterPreviousModifier(PreviousModifier);
	}
	// Nothing to do if remove the tailing modifier

	ModifierList.Remove(InModifier);
}
