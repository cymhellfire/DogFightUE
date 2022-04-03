#pragma once
#include "Common.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"

class FAbstractSyntaxTreeNodeBase;
class FASTIDNode;
class FASTFunctionNode;
class FASTClassNode;

namespace ERegistryEntryType
{
	enum Type
	{
		RET_None			= 0,
		RET_Variable		= 1 << 0,
		RET_Function		= 1 << 1,
		RET_Class			= 1 << 2,
		RET_All				= RET_Variable | RET_Function | RET_Class,
	};
}

struct INGAMESCRIPT_API FRegistryEntry
{
	explicit FRegistryEntry(ERegistryEntryType::Type InEntryType)
	{
		EntryType = InEntryType;
		ValueType = EValueType::VT_None;
		ASTNode = nullptr;
	}

	ERegistryEntryType::Type EntryType;
	EValueType::Type ValueType;
	TSharedPtr<FAbstractSyntaxTreeNodeBase> ASTNode;
};

// A table stores all defined variables in current code scope.
class INGAMESCRIPT_API FRegistry
{
public:
	FRegistry();
	~FRegistry();

	void RegisterNewVariable(FName VariableName, TSharedPtr<FASTIDNode> VariableNode = nullptr);
	void UnRegisterVariable(FName VariableName);

	bool RegisterNewFunction(FName FunctionName, TSharedPtr<FASTFunctionNode> FunctionNode = nullptr);

	bool RegisterNewClass(FName ClassName, TSharedPtr<FASTClassNode> ClassNode = nullptr);

	FRegistryEntry* GetRegistryEntry(FName EntryName, int32 TypeMask = 0);

protected:
	TMap<FName, FRegistryEntry> RegistryMap;
};
