#pragma once
#include "Common.h"
#include "AST/AbstractSyntaxTree.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"

class FScriptLexer;
struct FTokenBase;
struct FRegistryEntry; 
class FAbstractSyntaxTreeNodeBase;
class FRegistry;
class FScriptParser;

enum EParseResult
{
	EPR_EOF,
	EPR_Succeed,
	EPR_FallBack,
	EPR_Failed,
};

/**
 * Cache tokens used in current scope and take different operations when destruct based on listening parse result.
 */
struct FScopeTokenCache
{
	explicit FScopeTokenCache(EParseResult* InResultAddress, TWeakPtr<FScriptParser> InOwner);
	~FScopeTokenCache();

private:
	TWeakPtr<FScriptParser> Owner;
	EParseResult* Result;
};

class INGAMESCRIPT_API FScriptParser : public TSharedFromThis<FScriptParser>
{
public:
	friend class FScriptParserFactory;
	friend FScopeTokenCache;
	~FScriptParser();

	bool Execute();

protected:
	FScriptParser();
	FScriptParser(FString FilePath);

	void CreateLexer(FString FilePath);

	void Initialize();

	/**
	 * Parse the file into program.
	 * @brief StateList
	 */
	TSharedPtr<FAbstractSyntaxTree> Program();

	/**
	 * StateList is a list of common state. (Can be empty.)
	 * @brief [{ CommonStatement }]
	 */
	EParseResult StateList();

	/**
	 * CommonStatement is the basic element in program.\n
	 * @brief ClassDefinition | FunctionDefinition | PrimaryStatement
	 */
	EParseResult CommonStatement();

	/**
	 * ClassDefinition defines a new class.\n
	 * @brief class ID '{' ClassMemberList '}'
	 */
	EParseResult ClassDefinition();

	/**
	 * @brief {FunctionDefinition|VariableInitialization}
	 */
	EParseResult ClassMemberList(TArray<int32>& FuncIndices, TArray<int32>& InitIndices);

	/**
	 * FunctionDefinition defines a function to current scope.\n
	 * @brief function ID '(' FuncArgs ')' '{' StateList '}'
	 */
	EParseResult FunctionDefinition(TArray<int32>& FuncIndices);

	/**
	 * @brief VariableDefinition ['=' ComplexExpression] ';'
	 */
	EParseResult VariableInitialization(TArray<int32>& InitIndices);

	/**
	 * @brief (string|number|bool|ClassID) ID
	 */
	EParseResult VariableDefinition();

	/**
	 * FuncArgs includes all arguments of function.
	 * @brief VariableDefinition {',' VariableDefinition}
	 */
	EParseResult FuncArgs();

	/**
	 * @brief SimpleExpression [',' SimpleExpression]
	 */
	EParseResult FuncParam();

	/**
	 * @brief (AssignStatement | SimpleExpression | ReturnStatement);
	 */
	EParseResult PrimaryStatement();

	/**
	 * @brief return ComplexExpression
	 */
	EParseResult ReturnStatement();

	/**
	 * @brief (VariableDefinition | ID) '=' ComplexExpression
	 */
	EParseResult AssignStatement();

	/**
	 * @brief SimpleExpression {BinaryOperator SimpleExpression}
	 */
	EParseResult ComplexExpression();

	/**
	 * Construct binary expression AST with top three nodes in Stack.
	 */
	bool ConstructBinaryOperatorAST();

	/**
	 * @brief String | Number | Boolean | PrefixExpression
	 */
	EParseResult SimpleExpression();

	/**
	 * PrefixExpression is prefixed with a unary operator.
	 * @brief UnaryOperator SuffixExpression
	 */
	EParseResult PrefixExpression();

	/**
	 * SuffixExpression is always used to represent member/index accessing.
	 * @brief AtomicExpression [{( '.' ID | '[' ComplexExpression ']' | '(' FuncParam ')')}]
	 */
	EParseResult SuffixExpression();

	/**
	 * AtomicExpression is the atomic element in AST.
	 * @brief ID | '(' ComplexExpression ')'
	 */
	EParseResult AtomicExpression();

	/**
	 * Create a new ID node.
	 */
	bool CreateIDNode();

	/**
	 * Find existing registry entry with specified NodeName.
	 * @param NodeName			Name to search with.
	 * @param EntryType			Entry type mask for filtering.
	 */
	FRegistryEntry* FindIDNode(FName NodeName, int32 EntryType);

	/**
	 * Find existing registry entry in given class with specified name.
	 * @param NodeName			Name to search.
	 * @param EntryType			Entry type mask for filtering.
	 * @param ClassNode			Class node to search through.
	 */
	FRegistryEntry* FindIDInClass(FName NodeName, int32 EntryType, TSharedPtr<FASTClassNode> ClassNode);

	/**
	 * Get next token from lexer and push this token into stack.
	 */
	TSharedPtr<FTokenBase> GetNextToken(bool bPushToStack = true);

	/**
	 * Get the token pointed by CurTokenPointer;
	 */
	TSharedPtr<FTokenBase> GetCurrentToken();

	/**
	 * Get next token that is not handled yet. Get next from lexer if stack is empty.
	 */
	TSharedPtr<FTokenBase> GetUnHandledToken();

	/**
	 * Pop specified amount of nodes from stack.
	 */
	bool PopNodes(int32 Count = 1);

	/**
	 * Pop one token from stack.
	 */
	void PopToken(int32 PopCount = 1);

	/**
	 * Move CurTokenPointer backward by given step.
	 * @brief CurTokenPointer - Step
	 * @param Step			Moving step
	 * @return Pointing index after move
	 */
	int32 BackwardTokenPointer(int32 Step = 1)
	{
		CurTokenPointer = FMath::Clamp(CurTokenPointer - Step, 0, TokenStack.Num() - 1);
		return CurTokenPointer;
	}

	/**
	 * Move CurTokenPointer forward by given step.
	 * @brief CurTokenPointer + Step
	 * @param Step			Moving step
	 * @return Pointing index after move
	 */
	int32 ForwardTokenPointer(int32 Step = 1)
	{
		CurTokenPointer = FMath::Clamp(CurTokenPointer + Step, 0, TokenStack.Num() - 1);
		return CurTokenPointer;
	}

	/**
	 * Start cache token from now on.
	 */
	void StartTokenCache();

	/**
	 * Push a token into cache for later recover.
	 */
	void CacheToken(TSharedPtr<FTokenBase> InToken);

	/**
	 * Move all tokens from cache to TokenStack. Tokens will remain the order in cache.
	 */
	void RecoverTokenFromCache();

	/**
	 * Clear all tokens in token cache.
	 */
	void ClearTokenCache();

	/**
	 * Check token with specified symbol type.
	 */
	bool CheckTokenSymbolType(TSharedPtr<FTokenBase> CheckToken, ESingleSymbolType::Type ExpectType) const;

protected:
	TSharedPtr<FScriptLexer> OwningLexer;

	TArray<TSharedPtr<FTokenBase>> TokenStack;
	TArray<TSharedPtr<FAbstractSyntaxTreeNodeBase>> ASTNodeStack;
	TArray<TSharedPtr<FRegistry>> RegistryStack;

	TArray<int32> TokenCacheOrigin;
	TArray<TSharedPtr<FTokenBase>> TokenCache;

	int32 CurTokenPointer;
	bool bEnableTokenCache;
};

class INGAMESCRIPT_API FScriptParserFactory
{
public:
	static TSharedPtr<FScriptParser> CreateScriptParser(FString FilePath);
};
