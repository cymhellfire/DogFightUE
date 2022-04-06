#pragma once

#include "Common.h"

namespace EValueType
{
	/**
	 * Manager class for unifying User-Created Class with inner value type.
	 */
	class FValueTypeManager
	{
	public:
		static void Initialize();
		static void Uninitialize();
		static TSharedPtr<FValueTypeManager> GetInstance()
		{
			return Instance;
		}

		Type GetValueTypeFromClassName(FName ClassName);
		FName GetClassNameFromValueType(Type ValueType);

	private:
		int32 FindMissingClassIndex() const;
		int32 RegisterNewClass(FName ClassName);

	private:
		static TSharedPtr<FValueTypeManager> Instance;

		TMap<int32, FName> ClassTypeIndexMap;
	};
}
