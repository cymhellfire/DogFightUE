#include "AST/ASTType.h"

namespace EValueType
{
	TSharedPtr<FValueTypeManager> FValueTypeManager::Instance;

	void FValueTypeManager::Initialize()
	{
		if (!Instance.IsValid())
		{
			Instance = MakeShareable(new FValueTypeManager);
		}
	}

	void FValueTypeManager::Uninitialize()
	{
		if (Instance.IsValid())
		{
			Instance.Reset();
		}
	}

	int32 FValueTypeManager::FindMissingClassIndex() const
	{
		TArray<int32> KeyArray;
		ClassTypeIndexMap.GenerateKeyArray(KeyArray);

		// Mark element visited by negative its value
		int32 Size = KeyArray.Num();
		for (int32 i = 0; i < Size; ++i)
		{
			int32 VisitIndex = FMath::Abs(KeyArray[i]);
			if (VisitIndex < Size && KeyArray[VisitIndex] >= 0)
			{
				KeyArray[VisitIndex] = -KeyArray[VisitIndex];
			}
		}

		// Return first non-negative value index
		for (int32 i = 0; i < Size; ++i)
		{
			if (KeyArray[i] > 0)
			{
				return i;
			}
		}

		return Size;
	}

	int32 FValueTypeManager::RegisterNewClass(FName ClassName)
	{
		// Check existing value
		for (auto Record : ClassTypeIndexMap)
		{
			if (Record.Value == ClassName)
			{
				return Record.Key;
			}
		}

		// Register new class
		int32 ClassIndex = FindMissingClassIndex();
		if (ClassTypeIndexMap.Contains(ClassIndex))
		{
			UE_LOG(LogInGameScript, Error, TEXT("[RegisterNewClass] Class index duplicated."));
			return ClassIndex;
		}
		ClassTypeIndexMap.Add(ClassIndex, ClassName);
		return ClassIndex;
	}

	Type FValueTypeManager::GetValueTypeFromClassName(FName ClassName)
	{
		return static_cast<Type>(RegisterNewClass(ClassName) + VT_Class);
	}

	FName FValueTypeManager::GetClassNameFromValueType(Type ValueType)
	{
		int32 ClassIndex = ValueType - VT_Class;
		if (ValueType < 0)
		{
			UE_LOG(LogInGameScript, Error, TEXT("[GetClassNameFromValueType] Invalid class type."));
			return NAME_None;
		}

		return ClassTypeIndexMap.Contains(ClassIndex) ? ClassTypeIndexMap[ClassIndex] : NAME_None;
	}
}
