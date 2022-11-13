#include "AttributeSystem/Attribute/AttributeBase.h"
#include "AttributeSystem/Modifier/AttributeModifierBase.h"

template <typename T>
T TAttributeBase<T>::GetValue() const
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
void TAttributeBase<T>::AddModifier(TSharedPtr<FAttributeModifierBase> InModifier)
{
	if (!InModifier.IsValid())
	{
		return;
	}

	// Convert to expected type
	TSharedPtr<TAttributeModifierBase<T>> ConvertedModifier = StaticCastSharedPtr<TAttributeModifierBase<T>>(InModifier);

	if (ConvertedModifier.IsValid() && !ModifierList.Contains(ConvertedModifier))
	{
		if (ConvertedModifier->Apply(SharedThis(this)))
		{
			// Record the previous modifier
			auto LastModifier = GetLastModifier();
			if (LastModifier.IsValid())
			{
				ConvertedModifier->RegisterPreviousModifier(LastModifier);
			}
			ModifierList.Add(ConvertedModifier);

			// Trigger delegate
			OnValueChanged.Broadcast(SharedThis(this));
		}
	}
}

template <typename T>
void TAttributeBase<T>::RemoveModifier(TWeakPtr<FAttributeModifierBase> InModifier)
{
	if (!InModifier.IsValid())
	{
		return;
	}

	// Convert to expected type
	auto PinnedModifier = InModifier.Pin();
	TSharedPtr<TAttributeModifierBase<T>> ConvertedModifier = StaticCastSharedPtr<TAttributeModifierBase<T>>(PinnedModifier);
	if (!ConvertedModifier.IsValid() || !ModifierList.Contains(ConvertedModifier))
	{
		return;
	}

	int32 Index = ModifierList.IndexOfByKey(ConvertedModifier);
	ensure(Index != INDEX_NONE);

	// Mark modifier as dirty to notify those modifiers behind it
	ConvertedModifier->Remove();

	if (Index == 0)
	{
		// Make the second modifier as the head if possible
		if (ModifierList.Num() > 1)
		{
			TSharedPtr<TAttributeModifierBase<T>> NewHead = ModifierList[1];
			// Clear the previous modifier
			NewHead->ClearPreviousModifier();
		}
	}
	else if (Index < ModifierList.Num() - 1)
	{
		// Handle the removal if modifier is in the middle
		TSharedPtr<TAttributeModifierBase<T>> PreviousModifier = ModifierList[Index - 1];
		TSharedPtr<TAttributeModifierBase<T>> NextModifier = ModifierList[Index + 1];

		NextModifier->RegisterPreviousModifier(PreviousModifier);
	}
	// Nothing to do if remove the tailing modifier

	ModifierList.Remove(ConvertedModifier);

	// Trigger delegate
	OnValueChanged.Broadcast(SharedThis(this));
}
