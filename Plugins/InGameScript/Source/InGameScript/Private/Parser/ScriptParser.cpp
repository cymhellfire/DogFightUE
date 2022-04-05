#include "Parser/ScriptParser.h"

#include "InGameScript.h"
#include "AST/AbstractSyntaxTree.h"
#include "AST/NodeValueTypeHelper.h"
#include "AST/Registry.h"
#include "Parser/ScriptLexer.h"
#include "AST/ASTErrorCode.h"

FScopeTokenCache::FScopeTokenCache(EParseResult* InResultAddress, TWeakPtr<FScriptParser> InOwner)
{
	Result = InResultAddress;
	Owner = InOwner;

	// Start token cache
	Owner.Pin()->StartTokenCache();
}

FScopeTokenCache::~FScopeTokenCache()
{
	if (!Owner.IsValid())
		return;

	// Take different actions based on result
	switch(*Result)
	{
	case EPR_FallBack:
		Owner.Pin()->RecoverTokenFromCache();
		break;
	case EPR_Succeed:
	case EPR_Failed:
	case EPR_EOF:
	default:
		Owner.Pin()->ClearTokenCache();
	}
}

FScriptParser::FScriptParser()
{
	CurTokenPointer = 0;
	bEnableTokenCache = false;
}

FScriptParser::FScriptParser(FString FilePath)
{
	if (!FilePath.IsEmpty())
	{
		CreateLexer(FilePath);
	}
}

FScriptParser::~FScriptParser()
{
	ASTNodeStack.Empty();
	TokenStack.Empty();
	RegistryStack.Empty();
	TokenCache.Empty();
	UE_LOG(LogInGameScript, Log, TEXT("[ScriptParser] Instance destructed."));
}

void FScriptParser::CreateLexer(FString FilePath)
{
	OwningLexer = FScriptLexerFactory::CreateScriptLexer(FilePath);
}

void FScriptParser::Initialize()
{
	CurTokenPointer = 0;
	bEnableTokenCache = false;
}

bool FScriptParser::Execute()
{
	if (OwningLexer.IsValid())
	{
		// Iterate through all tokens and print info
		// int32 TokenIndex = 0;
		// TSharedPtr<FTokenBase> CurToken = OwningLexer->GetNextToken();
		// while (CurToken.IsValid())
		// {
		// 	// Print token information
		// 	UE_LOG(LogInGameScript, Log, TEXT("[ScriptParser] Token[%d]: %s"), TokenIndex, *CurToken->ToString());
		//
		// 	CurToken = OwningLexer->GetNextToken();
		// 	TokenIndex++;
		// }

		UE_LOG(LogInGameScript, Log, TEXT("[ScriptParser] Start parse."));

		Initialize();
		Program();
		UE_LOG(LogInGameScript, Log, TEXT("[ScriptParser] Parser finished."));

		return true;
	}

	return false;
}

TSharedPtr<FAbstractSyntaxTree> FScriptParser::Program()
{
	// Create file scope registry first
	TSharedPtr<FRegistry> FileRegistry = MakeShareable(new FRegistry);
	RegistryStack.Push(FileRegistry);

	EParseResult Result = StateList();

	// Return nullptr if parser failed
	if (Result != EPR_Succeed)
	{
		return nullptr;
	}

	// Get root node
	TSharedPtr<FASTStateListNode> StateList = StaticCastSharedPtr<FASTStateListNode>(ASTNodeStack.Top());

	if (!StateList.IsValid())
	{
		return nullptr;
	}

	// Initialize Abstract Syntax Tree
	TSharedPtr<FAbstractSyntaxTree> NewAST = MakeShareable(new FAbstractSyntaxTree);
	NewAST->SetRootNode(StateList);

	// Start parse
	return NewAST;
}

EParseResult FScriptParser::StateList()
{
	// Parse loop
	int32 OrgNodeStackNum = ASTNodeStack.Num();
	EParseResult ParseResult;
	TSharedPtr<FTokenBase> CurToken;
	do
	{
		ParseResult = CommonStatement();

		// Check next token for existing loop
		CurToken = GetUnHandledToken();
	}
	while (ParseResult == EPR_Succeed && CurToken.IsValid()
		&& !CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_CloseCurly));

	// Create StateList node
	TSharedPtr<FASTStateListNode> StateListNode = MakeShareable(new FASTStateListNode);
	if (ASTNodeStack.Num() > OrgNodeStackNum)
	{
		for (int32 Index = OrgNodeStackNum; Index < ASTNodeStack.Num(); ++Index)
		{
			TSharedPtr<FASTValueNode> ValueNode = StaticCastSharedPtr<FASTValueNode>(ASTNodeStack[Index]);
			if (ValueNode.IsValid())
			{
				StateListNode->AddToChildNodes(ValueNode);
			}
			else
			{
				UE_LOG(LogInGameScript, Error, TEXT("[ScriptParser] Invalid statement node to add."));
				return EPR_Failed;
			}
		}

		// Pop out all statements
		PopNodes(ASTNodeStack.Num() - OrgNodeStackNum);
	}
	ASTNodeStack.Push(StateListNode);

	return ParseResult;
}

EParseResult FScriptParser::CommonStatement()
{
	// Check the first token
	TSharedPtr<FTokenBase> CurToken = GetUnHandledToken();
	if (!CurToken.IsValid())
	{
		return EPR_Failed;
	}

	EParseResult ParseResult = EPR_Failed;
	if (CurToken->TokenType == ETokenType::TT_Reserved)
	{
		TSharedPtr<FReservedToken> ReservedToken = StaticCastSharedPtr<FReservedToken>(CurToken);
		switch (ReservedToken->ReservedType)
		{
		case EReservedType::RT_Class:
			// Class definition
			ParseResult = ClassDefinition();
			break;
		case EReservedType::RT_Function:
			{
				TArray<int32> Temp;
				// Function definition
				ParseResult = FunctionDefinition(Temp);
			}
			break;
		default:
			// Fallback to PrimaryStatement
			ParseResult = PrimaryStatement();
		}
	}
	else
	{
		// Normal statement
		ParseResult = PrimaryStatement();
	}

	return ParseResult;
}

EParseResult FScriptParser::ClassDefinition()
{
	// Pop class token and create class node
	PopToken();
	TSharedPtr<FASTClassNode> NewClass = MakeShareable(new FASTClassNode);

	// Read class name
	TSharedPtr<FTokenBase> CurToken = GetUnHandledToken();
	if (CurToken->TokenType == ETokenType::TT_ID)
	{
		TSharedPtr<FIDToken> IDToken = StaticCastSharedPtr<FIDToken>(CurToken);
		if (IDToken)
		{
			NewClass->SetClassID(IDToken->IdName);
		}
		else
		{
			LOG_WITH_CHAR_POS(ELogVerbosity::Error, TEXT("[ScriptParser] Invalid ID token."), CurToken);
			return EPR_Failed;
		}
	}
	else
	{
		LOG_WITH_CHAR_POS(ELogVerbosity::Error, TEXT("[ScriptParser] Expect ID token."), CurToken);
		return EPR_Failed;
	}
	PopToken();

	// Register new class
	RegistryStack.Top()->RegisterNewClass(NewClass->GetClassID(), NewClass);

	// Create local registry for new class
	TSharedPtr<FRegistry> NewRegistry = MakeShareable(new FRegistry);
	RegistryStack.Push(NewRegistry);

	// Set registry
	NewClass->SetClassRegistry(NewRegistry);

	// Read class body
	CurToken = GetUnHandledToken();
	if (CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_OpenCurly))
	{
		PopToken();

		TArray<int32> FuncIndices, InitIndices;
		EParseResult Result = ClassMemberList(FuncIndices, InitIndices);
		if (Result == EPR_Failed)
		{
			return EPR_Failed;
		}

		// Check tailing '}'
		CurToken = GetUnHandledToken();
		if (!CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_CloseCurly))
		{
			LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] No tailing '}'", CurToken);
			return EPR_Failed;
		}
		PopToken();

		// Link function nodes to class
		for (int32 FuncIndex : FuncIndices)
		{
			TSharedPtr<FASTFunctionNode> FunctionNode = StaticCastSharedPtr<FASTFunctionNode>(ASTNodeStack[FuncIndex]);
			if (FunctionNode)
			{
				NewClass->AddToFunctionNodeList(FunctionNode);
			}
			else
			{
				UE_LOG(LogInGameScript, Error, TEXT("[ScriptParser] Invalid function node detected."));
				return EPR_Failed;
			}
		}

		// Link initialization nodes to class
		for (int32 InitIndex : InitIndices)
		{
			TSharedPtr<FASTAssignStatementNode> InitializeNode = StaticCastSharedPtr<FASTAssignStatementNode>(ASTNodeStack[InitIndex]);
			if (InitializeNode)
			{
				NewClass->AddToVariableInitialList(InitializeNode);
			}
			else
			{
				UE_LOG(LogInGameScript, Error, TEXT("[ScriptParser] Invalid initialize node detected."));
				return EPR_Failed;
			}
		}

		// Pop out all function nodes and initialization nodes
		PopNodes(FuncIndices.Num() + InitIndices.Num());
	}

	// Pop registry
	RegistryStack.Pop();

	return EPR_Succeed;
}

EParseResult FScriptParser::ClassMemberList(TArray<int32>& FuncIndices, TArray<int32>& InitIndices)
{
	TSharedPtr<FTokenBase> CurToken = GetUnHandledToken();
	while(!CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_CloseCurly))
	{
		if (CurToken->TokenType == ETokenType::TT_Reserved)
		{
			TSharedPtr<FReservedToken> ReservedToken = StaticCastSharedPtr<FReservedToken>(CurToken);
			if (ReservedToken)
			{
				if (ReservedToken->ReservedType == EReservedType::RT_Function)
				{
					if (FunctionDefinition(FuncIndices) == EPR_Failed)
					{
						return EPR_Failed;
					}
				}
				else
				{
					if (VariableInitialization(InitIndices) == EPR_Failed)
					{
						return EPR_Failed;
					}
				}
			}
			else
			{
				LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] Invalid ReservedToken detected.", CurToken);
				return EPR_Failed;
			}
		}
		else
		{
			LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] ReservedToken expected.", CurToken);
			return EPR_Failed;
		}
		CurToken = GetUnHandledToken();
	}

	return EPR_Succeed;
}

EParseResult FScriptParser::FunctionDefinition(TArray<int32>& FuncIndices)
{
	// Pop function token and create function node
	PopToken();
	TSharedPtr<FASTFunctionNode> NewFunc = MakeShareable(new FASTFunctionNode);

	// Read function return value type
	TSharedPtr<FTokenBase> CurToken = GetUnHandledToken();
	if (CurToken->TokenType == ETokenType::TT_Reserved)
	{
		PopToken();

		TSharedPtr<FReservedToken> ValueType = StaticCastSharedPtr<FReservedToken>(CurToken);
		switch(ValueType->ReservedType)
		{
		case EReservedType::RT_Boolean:
			{
				TSharedPtr<FASTBooleanNode> BooleanNode = MakeShareable(new FASTBooleanNode);
				NewFunc->SetReturnValueNode(BooleanNode);
			}
			break;
		case EReservedType::RT_Number:
			{
				TSharedPtr<FASTNumberNode> NumberNode = MakeShareable(new FASTNumberNode);
				NewFunc->SetReturnValueNode(NumberNode);
			}
			break;
		case EReservedType::RT_String:
			{
				TSharedPtr<FASTStringNode> StringNode = MakeShareable(new FASTStringNode);
				NewFunc->SetReturnValueNode(StringNode);
			}
			break;
		case EReservedType::RT_Void:
			{
				TSharedPtr<FASTVoidNode> VoidNode = MakeShareable(new FASTVoidNode);
				NewFunc->SetReturnValueNode(VoidNode);
			}
			break;
		default:
			LOG_WITH_CHAR_POS(ELogVerbosity::Error, TEXT("[InGameScript] Invalid value type token."), CurToken);
			return EPR_Failed;
		}
	}
	else if (CurToken->TokenType == ETokenType::TT_ID)
	{
		TSharedPtr<FIDToken> ValueIDToken = StaticCastSharedPtr<FIDToken>(CurToken);
		FRegistryEntry* IDEntry = FindIDNode(ValueIDToken->IdName, ERegistryEntryType::RET_Class);
		if (IDEntry)
		{
			PopToken();

			TSharedPtr<FASTClassNode> ClassNode = StaticCastSharedPtr<FASTClassNode>(IDEntry->ASTNode);
			TSharedPtr<FASTClassTypeNode> ClassTypeNode = MakeShareable(new FASTClassTypeNode(ClassNode));
			NewFunc->SetReturnValueNode(ClassTypeNode);
		}
		else
		{
			LOG_WITH_CHAR_POS(ELogVerbosity::Error, PARSE_ERROR_UNDEFINED_IDENTIFIER, CurToken);
			return EPR_Failed;
		}
	}
	else
	{
		LOG_WITH_CHAR_POS(ELogVerbosity::Error, TEXT("[InGameScript] Value type token expected."), CurToken);
		return EPR_Failed;
	}

	// Read function name
	CurToken = GetUnHandledToken();
	if (CurToken->TokenType == ETokenType::TT_ID)
	{
		TSharedPtr<FIDToken> IDToken = StaticCastSharedPtr<FIDToken>(CurToken);
		if (IDToken)
		{
			NewFunc->SetFunctionName(IDToken->IdName);
		}
		else
		{
			LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] Invalid ID token detected.", CurToken);
			return EPR_Failed;
		}
		PopToken();
	}

	// Register new function
	RegistryStack.Top()->RegisterNewFunction(NewFunc->GetFunctionName(), NewFunc);

	// Create local registry for new function
	TSharedPtr<FRegistry> NewRegistry = MakeShareable(new FRegistry);
	RegistryStack.Push(NewRegistry);

	// Read function arguments
	CurToken = GetUnHandledToken();
	if (CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_OpenParen))
	{
		PopToken();

		EParseResult Result = FuncArgs();
		if (Result == EPR_Failed)
		{
			return EPR_Failed;
		}

		// Check tailing ')'
		CurToken = GetUnHandledToken();
		if (!CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_CloseParen))
		{
			LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] No tailing ')'.", CurToken);
			return EPR_Failed;
		}
		PopToken();

		// Set argument node if parsed before
		if (Result == EPR_Succeed)
		{
			TSharedPtr<FASTFuncArgsNode> FuncArgsNode = StaticCastSharedPtr<FASTFuncArgsNode>(ASTNodeStack.Top());
			if (FuncArgsNode)
			{
				NewFunc->SetArgsNode(FuncArgsNode);
				PopNodes();
			}
			else
			{
				UE_LOG(LogInGameScript, Error, TEXT("[ScriptParser] No FuncArgNode."));
				return EPR_Failed;
			}
		}
	}
	else
	{
		LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] Expected '('.", CurToken);
		return EPR_Failed;
	}

	// Read function body
	CurToken = GetUnHandledToken();
	if (CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_OpenCurly))
	{
		PopToken();

		int32 OrgNodeStackNum = ASTNodeStack.Num();
		if (StateList() == EPR_Failed)
		{
			return EPR_Failed;
		}

		// Check tailing '}'
		CurToken = GetUnHandledToken();
		if (!CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_CloseCurly))
		{
			LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] No tailing '}'.", CurToken);
			return EPR_Failed;
		}
		PopToken();

		// Get state list node
		TSharedPtr<FASTStateListNode> StateListNode = StaticCastSharedPtr<FASTStateListNode>(ASTNodeStack.Top());
		if (StateListNode.IsValid())
		{
			PopNodes();
		}

		NewFunc->SetStateListNode(StateListNode);
	}

	// Pop registry
	RegistryStack.Pop();

	// Record function index
	FuncIndices.Add(ASTNodeStack.Num());

	// Push function node
	ASTNodeStack.Push(NewFunc);

	return EPR_Succeed;
}

EParseResult FScriptParser::VariableInitialization(TArray<int32>& InitIndices)
{
	EParseResult Result = VariableDefinition();
	if (Result == EPR_Failed)
	{
		LOG_WITH_CHAR_POS(ELogVerbosity::Error, TEXT("[ScriptParser] Expected definition."), GetUnHandledToken());
		return Result;
	}

	// Read assignment operator
	TSharedPtr<FTokenBase> CurToken = GetUnHandledToken();
	if (CurToken->TokenType == ETokenType::TT_BinaryOperator)
	{
		TSharedPtr<FOperatorToken> OperatorToken = StaticCastSharedPtr<FOperatorToken>(CurToken);
		if (OperatorToken->BinaryOperatorType == EBinaryOperatorType::BOT_Assign)
		{
			PopToken();

			// Read the value source
			Result = ComplexExpression();
			if (Result == EPR_Failed)
			{
				return EPR_Failed;
			}

			int32 StackTop = ASTNodeStack.Num() - 1;
			// Get the destination node
			TSharedPtr<FASTIDNode> DestNode = StaticCastSharedPtr<FASTIDNode>(ASTNodeStack[StackTop - 1]);
			TSharedPtr<FASTValueNode> SourceNode = StaticCastSharedPtr<FASTValueNode>(ASTNodeStack[StackTop]);

			if (DestNode == nullptr || SourceNode == nullptr)
			{
				LOG_WITH_CHAR_POS(ELogVerbosity::Error, TEXT("[ScriptParser] Failed to construct assign statement."), GetUnHandledToken());
				return EPR_Failed;
			}

			// Create the initialization statement
			TSharedPtr<FASTAssignStatementNode> InitialStatement = MakeShareable(new FASTAssignStatementNode);
			InitialStatement->SetDestNode(DestNode);
			InitialStatement->SetSourceNode(SourceNode);

			PopNodes(2);
			InitIndices.Add(ASTNodeStack.Num());
			ASTNodeStack.Push(InitialStatement);
		}
		else
		{
			LOG_WITH_CHAR_POS(ELogVerbosity::Error, TEXT("[ScriptParser] Only assign statement support here."), CurToken);
			return EPR_Failed;
		}
	}
	else
	{
		// Pop variable definition node from stack to avoid ID node left after class node constructed
		PopNodes();
	}

	// Check the tailing semicolon
	CurToken = GetUnHandledToken();
	if (!CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_Semicolon))
	{
		LOG_WITH_CHAR_POS(ELogVerbosity::Error, TEXT("[ScriptParser] Expected tailing ';'."), CurToken);
		return EPR_Failed;
	}
	PopToken();

	return Result;
}

EParseResult FScriptParser::VariableDefinition()
{
	EParseResult Result = EPR_Failed;
	FScopeTokenCache ScopeTokenCache(&Result, TWeakPtr<FScriptParser>(SharedThis(this)));
	//StartTokenCache();

	TSharedPtr<FTokenBase> CurToken = GetUnHandledToken();
	if (CurToken->TokenType == ETokenType::TT_Reserved)
	{
		TSharedPtr<FReservedToken> ReservedToken = StaticCastSharedPtr<FReservedToken>(CurToken);
		if (ReservedToken)
		{
			PopToken();

			// Read the ID
			CurToken = GetUnHandledToken();
			if (CurToken->TokenType != ETokenType::TT_ID)
			{
				LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] ID token expected.", CurToken);
				return EPR_Failed;
			}
			if (!CreateIDNode())
			{
				return EPR_Failed;
			}
			TSharedPtr<FASTIDNode> IDNode = StaticCastSharedPtr<FASTIDNode>(ASTNodeStack.Top());

			if (RegistryStack.Num() == 0)
			{
				UE_LOG(LogInGameScript, Error, TEXT("[ScriptParser] No registry currently."));
				return EPR_Failed;
			}

			// Register new variable to registry
			TSharedPtr<FRegistry> CurrentRegistry = RegistryStack.Top();
			switch(ReservedToken->ReservedType)
			{
			case EReservedType::RT_Boolean:
				IDNode->SetValue(MakeShareable(new FASTBooleanNode));
				break;
			case EReservedType::RT_Number:
				IDNode->SetValue(MakeShareable(new FASTNumberNode));
				break;
			case EReservedType::RT_String:
				IDNode->SetValue(MakeShareable(new FASTStringNode));
				break;
			default:
				LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] Unsupported value type detected.", CurToken);
				return EPR_Failed;
			}
			CurrentRegistry->RegisterNewVariable(IDNode->GetID(), IDNode);
		}
		else
		{
			LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] Reserved token expected.", CurToken);
			return EPR_Failed;
		}
	}
	else if (CurToken->TokenType == ETokenType::TT_ID)
	{
		TSharedPtr<FIDToken> ClassIDToken = StaticCastSharedPtr<FIDToken>(CurToken);
		if (ClassIDToken.IsValid())
		{
			PopToken();

			// Validate class ID
			FRegistryEntry* ClassEntry = FindIDNode(ClassIDToken->IdName, ERegistryEntryType::RET_Class);
			if (ClassEntry == nullptr)
			{
				Result = EPR_FallBack;
				//RecoverTokenFromCache();
				return Result;
			}

			TSharedPtr<FASTClassNode> ClassNode = StaticCastSharedPtr<FASTClassNode>(ClassEntry->ASTNode);
			TSharedPtr<FASTClassTypeNode> ClassTypeNode = MakeShareable(new FASTClassTypeNode(ClassNode));

			// Read variable ID
			CurToken = GetUnHandledToken();
			if (CurToken->TokenType != ETokenType::TT_ID)
			{
				Result = EPR_FallBack;
				//RecoverTokenFromCache();
				return Result;
			}

			if (!CreateIDNode())
			{
				return EPR_Failed;
			}

			TSharedPtr<FASTIDNode> IDNode = StaticCastSharedPtr<FASTIDNode>(ASTNodeStack.Top());
			IDNode->SetValue(ClassTypeNode);
			IDNode->SetValueType(ClassTypeNode->GetValueType());

			// Register new variable
			RegistryStack.Top()->RegisterNewVariable(IDNode->GetID(), IDNode);
		}
	}
	else
	{
		//RecoverTokenFromCache();
		return EPR_Failed;
	}

	Result = EPR_Succeed;
	//ClearTokenCache();
	return Result;
}

EParseResult FScriptParser::FuncArgs()
{
	EParseResult Result = VariableDefinition();

	// Handle no parameter cases
	TSharedPtr<FTokenBase> CurToken = GetUnHandledToken();
	if (Result == EPR_Failed)
	{
		if (CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_CloseParen))
		{
			return EPR_FallBack;
		}
		return Result;
	}

	int32 ArgCount = 1; 
	while (CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_Comma))
	{
		PopToken();

		Result = VariableDefinition();
		if (Result == EPR_Failed)
		{
			return EPR_Failed;
		}

		ArgCount++;
		CurToken = GetUnHandledToken();
	}

	int32 StackLen = ASTNodeStack.Num();
	TSharedPtr<FASTFuncArgsNode> FuncArgsNode = MakeShareable(new FASTFuncArgsNode);
	for (int32 i = ArgCount; i > 0; --i)
	{
		TSharedPtr<FASTIDNode> IDNode = StaticCastSharedPtr<FASTIDNode>(ASTNodeStack[StackLen - i]);
		if (IDNode == nullptr)
		{
			UE_LOG(LogInGameScript, Error, TEXT("[ScriptParser] Invalid FuncArg detected. Index %d."), ArgCount - i);
			return EPR_Failed;
		}

		FuncArgsNode->AddToArgsList(IDNode);
	}

	if (PopNodes(ArgCount))
	{
		ASTNodeStack.Push(FuncArgsNode);
	}
	else
	{
		return EPR_Failed;
	}

	return Result;
}

EParseResult FScriptParser::FuncParam()
{
	EParseResult Result = SimpleExpression();
	TSharedPtr<FTokenBase> CurToken = GetUnHandledToken();
	if (Result == EPR_Failed)
	{
		if (CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_CloseParen))
		{
			return EPR_FallBack;
		}
		return Result;
	}

	int32 ParamCount = 1;
	CurToken = GetUnHandledToken();
	while (CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_Comma))
	{
		//OwningLexer->ConsumeCurrentChar();
		PopToken();

		Result = SimpleExpression();
		if (Result == EPR_Failed)
		{
			LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] Expected AtomicExpression.", CurToken);
			return EPR_Failed;
		}

		ParamCount++;
		//NextChar = OwningLexer->GetCurrentChar();
		CurToken = GetUnHandledToken();
	}

	int32 StackLen = ASTNodeStack.Num();
	TSharedPtr<FASTFuncParamNode> FuncParamNode = MakeShareable(new FASTFuncParamNode);
	for (int32 i = ParamCount; i > 0; --i)
	{
		TSharedPtr<FASTValueNode> ParamExpr = StaticCastSharedPtr<FASTValueNode>(ASTNodeStack[StackLen - i]);
		if (ParamExpr == nullptr)
		{
			UE_LOG(LogInGameScript, Error, TEXT("[ScriptParser] Invalid FuncParam detected. Index %d."), ParamCount - i);
			return EPR_Failed;
		}

		FuncParamNode->AddToParamList(ParamExpr);
	}

	// Pop out all params
	if (PopNodes(ParamCount))
	{
		ASTNodeStack.Push(FuncParamNode);
	}
	else
	{
		Result = EPR_Failed;
	}

	return Result;
}

EParseResult FScriptParser::PrimaryStatement()
{
	bool bParsed = false;
	TSharedPtr<FTokenBase> CurToken = GetUnHandledToken();
	if (CurToken->TokenType == ETokenType::TT_Reserved)
	{
		TSharedPtr<FReservedToken> ReservedToken = StaticCastSharedPtr<FReservedToken>(CurToken);
		if (ReservedToken)
		{
			switch(ReservedToken->ReservedType)
			{
			case EReservedType::RT_Return:
				if (ReturnStatement() == EPR_Failed)
				{
					return EPR_Failed;
				}
				bParsed = true;
				break;
			default:;
			}
		}
	}

	EParseResult Result;
	if (!bParsed)
	{
		Result = AssignStatement();
		if (Result == EPR_FallBack)
		{
			Result = SimpleExpression();
		}

		if (Result == EPR_Failed)
		{
			return Result;
		}
	}

	// Check the tailing ';'
	CurToken = GetUnHandledToken();
	if (!CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_Semicolon))
	{
		LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] Missing tailling ';'.", CurToken);
		return EPR_Failed;
	}
	PopToken();

	return EPR_Succeed;
}

EParseResult FScriptParser::ReturnStatement()
{
	TSharedPtr<FTokenBase> CurToken = GetUnHandledToken();
	// Return statement should starts with a 'return'
	if (CurToken->TokenType == ETokenType::TT_Reserved)
	{
		TSharedPtr<FReservedToken> ReservedToken = StaticCastSharedPtr<FReservedToken>(CurToken);
		if (ReservedToken)
		{
			if (ReservedToken->ReservedType != EReservedType::RT_Return)
			{
				LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] 'return'", CurToken);
				return EPR_Failed;
			}
			PopToken();
		}
	}

	// Second part should be a ComplexExpression
	if (ComplexExpression() == EPR_Failed)
	{
		return EPR_Failed;
	}

	TSharedPtr<FASTBinaryOperatorNode> BinaryOperatorNode = StaticCastSharedPtr<FASTBinaryOperatorNode>(ASTNodeStack.Top());
	if (BinaryOperatorNode == nullptr)
	{
		UE_LOG(LogInGameScript, Error, TEXT("[ScriptParser] BinaryOperatorNode expected."));
		return EPR_Failed;
	}
	TSharedPtr<FASTReturnStatementNode> ReturnStatementNode = MakeShareable(new FASTReturnStatementNode);
	ReturnStatementNode->SetChildNode(BinaryOperatorNode);
	ReturnStatementNode->SetValueType(BinaryOperatorNode->GetValueType());

	PopNodes();
	ASTNodeStack.Push(ReturnStatementNode);

	return EPR_Succeed;
}

EParseResult FScriptParser::AssignStatement()
{
	// Enable token cache for fallback
	//StartTokenCache();
	EParseResult Result = EPR_Failed;
	FScopeTokenCache ScopeTokenCache(&Result, TWeakPtr<FScriptParser>(SharedThis(this)));
	// Left part
	TSharedPtr<FTokenBase> CurToken = nullptr;
	Result = VariableDefinition();
	if (Result == EPR_Failed)
	{
		return EPR_Failed;
	}

	if (Result == EPR_FallBack)
	{
		CurToken = GetUnHandledToken();
		if (CurToken->TokenType == ETokenType::TT_ID)
		{
			TSharedPtr<FIDToken> IDToken = StaticCastSharedPtr<FIDToken>(CurToken);
			FRegistryEntry* TargetNode = FindIDNode(IDToken->IdName, ERegistryEntryType::RET_Variable);
			if (TargetNode == nullptr)
			{
				LOG_WITH_CHAR_POS(ELogVerbosity::Error, PARSE_ERROR_UNDEFINED_IDENTIFIER, CurToken);
				Result = EPR_Failed;
				return Result;
			}
			PopToken();

			// Push found ID node into stack for later usage
			ASTNodeStack.Push(TargetNode->ASTNode);
		}
	}

	// Assign symbol
	CurToken = GetUnHandledToken();
	if (CurToken->TokenType == ETokenType::TT_BinaryOperator)
	{
		TSharedPtr<FOperatorToken> OperatorToken = StaticCastSharedPtr<FOperatorToken>(CurToken);
		if (OperatorToken)
		{
			if (OperatorToken->BinaryOperatorType != EBinaryOperatorType::BOT_Assign)
			{
				LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] '==' excepted.", CurToken);
				Result = EPR_Failed;
				return Result;
			}
		}
		else
		{
			LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] Invalid binary operator token.", CurToken);
			Result = EPR_Failed;
			return Result;
		}
	}
	else
	{
		// Pop out one node from stack because it will be rebuilt later
		PopNodes(1);
		Result = EPR_FallBack;
		return Result;
	}
	PopToken();

	// Right parts
	Result = ComplexExpression();
	if (Result == EPR_Failed)
	{
		return EPR_Failed;
	}

	int32 Top = ASTNodeStack.Num() - 1;
	TSharedPtr<FASTIDNode> LeftNode = StaticCastSharedPtr<FASTIDNode>(ASTNodeStack[Top - 1]);
	TSharedPtr<FASTValueNode> RightNode = StaticCastSharedPtr<FASTValueNode>(ASTNodeStack[Top]);

	// Create assign node
	TSharedPtr<FASTAssignStatementNode> AssignStatementNode = MakeShareable(new FASTAssignStatementNode);
	AssignStatementNode->SetSourceNode(RightNode);
	AssignStatementNode->SetDestNode(LeftNode);

	PopNodes(2);
	ASTNodeStack.Push(AssignStatementNode);

	// Turn off and clear cache
	//ClearTokenCache();
	return EPR_Succeed;
}

EParseResult FScriptParser::ComplexExpression()
{
	int32 CurrentOpPriority = 0;
	int32 LastOpPriority = 0;

	// Parse first SimpleExpression and push it into stack
	EParseResult Result = SimpleExpression();
	if (Result != EPR_Succeed)
	{
		return Result;
	}

	// Looping for parsing rest parts of expression
	bool bExpressionEnd = false;
	TArray<int32> LastOpPriorityStack;
	const int32 OrgStackTop = ASTNodeStack.Num();
	while (!bExpressionEnd)
	{
		TSharedPtr<FTokenBase> NextToken = GetUnHandledToken();
		// Stop if next char is semicolon
		if (CheckTokenSymbolType(NextToken, ESingleSymbolType::SST_Semicolon))
		{
			break;
		}

		// Next token should be a binary operator
		if (NextToken->TokenType != ETokenType::TT_BinaryOperator)
		{
			//UE_LOG(LogInGameScript, Error, TEXT("[ScriptParser] Binary operator token expected."));
			break;
		}

		// Get operator priority
		TSharedPtr<FOperatorToken> OpToken = StaticCastSharedPtr<FOperatorToken>(NextToken);
		EBinaryOperatorType::Type OpType = OpToken->BinaryOperatorType;

		// Compare priority with last one
		CurrentOpPriority = EBinaryOperatorType::PriorityTable[OpType];
		while (CurrentOpPriority <= LastOpPriority)
		{
			// Pop nodes from stack and construct AST until CurrentOpPriority > LastOpPriority
			if (!ConstructBinaryOperatorAST())
			{
				//LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] Construct BinaryOperatorExpression failed.", OwningLexer);
				return EPR_Failed;
			}

			// Update last operator priority
			LastOpPriority = LastOpPriorityStack.Pop();
		}
		// Push operator priority into stack and record current one
		LastOpPriorityStack.Push(LastOpPriority);
		LastOpPriority = CurrentOpPriority;

		// Create operator node
		TSharedPtr<FASTBinaryOperatorNode> NewOperatorNode = MakeShareable(new FASTBinaryOperatorNode);
		NewOperatorNode->SetOperatorType(OpType);
		ASTNodeStack.Push(NewOperatorNode);
		PopToken();

		// Following a SimpleExpression
		Result = SimpleExpression();
	}

	//Pop out rest nodes in stack and construct AST
	while (ASTNodeStack.Num() > OrgStackTop)
	{
		if (!ConstructBinaryOperatorAST())
		{
			//LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] Construct BinaryOperatorExpression failed.", OwningLexer);
			return EPR_Failed;
		}
	}

	return Result;
}

bool FScriptParser::ConstructBinaryOperatorAST()
{
	if (ASTNodeStack.Num() >= 3)
	{
		int32 Top = ASTNodeStack.Num() - 1;
		TSharedPtr<FASTSimpleExpressionNode> RightChild = StaticCastSharedPtr<FASTSimpleExpressionNode>(ASTNodeStack[Top]);
		TSharedPtr<FASTBinaryOperatorNode> OperatorNode = StaticCastSharedPtr<FASTBinaryOperatorNode>(ASTNodeStack[Top - 1]);
		TSharedPtr<FASTSimpleExpressionNode> LeftChild = StaticCastSharedPtr<FASTSimpleExpressionNode>(ASTNodeStack[Top - 2]);

		if (RightChild == nullptr || OperatorNode == nullptr || LeftChild == nullptr)
		{
			UE_LOG(LogInGameScript, Error, TEXT("[ScriptParser] Failed to construct expression AST with top three nodes."));
			return false;
		}

		// Save reference to operator node
		TSharedPtr<FAbstractSyntaxTreeNodeBase> OrgOperatorNode = ASTNodeStack[Top - 1];

		// Construct AST based on operator
		OperatorNode->SetLeftChildNode(LeftChild);
		OperatorNode->SetRightChildNode(RightChild);
		FNodeValueTypeHelper::UpdateASTNodeValueType(OperatorNode);

		// Pop out nodes that used to construct AST
		if (PopNodes(3))
		{
			// Push new construct node back to stack
			ASTNodeStack.Push(OrgOperatorNode);
		}
		else
		{
			return false;
		}

		return true;
	}
	else
	{
		UE_LOG(LogInGameScript, Error, TEXT("[ScriptParser] Not enough node in stack to construct AST."));
		return false;
	}
}

EParseResult FScriptParser::SimpleExpression()
{
	TSharedPtr<FTokenBase> CurToken = GetUnHandledToken();
	if (CurToken->TokenType == ETokenType::TT_Value)
	{
		TSharedPtr<FValueToken> ValueToken = StaticCastSharedPtr<FValueToken>(CurToken);
		if (ValueToken)
		{
			switch (ValueToken->ValueType)
			{
			case EValueType::VT_Boolean:
				{
					TSharedPtr<FASTBooleanNode> NewBooleanNode = MakeShareable(new FASTBooleanNode);
					NewBooleanNode->SetValue(ValueToken->BoolValue);
					ASTNodeStack.Push(NewBooleanNode);
					PopToken();
					return EPR_Succeed;
				}
			case EValueType::VT_Number:
				{
					// Create new number node and push to stack
					TSharedPtr<FASTNumberNode> NewNumberNode = MakeShareable(new FASTNumberNode);
					NewNumberNode->SetValue(ValueToken->NumberValue);
					ASTNodeStack.Push(NewNumberNode);
					PopToken();
					return EPR_Succeed;
				}
			case EValueType::VT_String:
				{
					TSharedPtr<FASTStringNode> NewStringNode = MakeShareable(new FASTStringNode);
					NewStringNode->SetValue(ValueToken->StringValue);
					ASTNodeStack.Push(NewStringNode);
					PopToken();
					return EPR_Succeed;
				}
			default:;
			}
			UE_LOG(LogInGameScript, Error, TEXT("[ScriptParser] Unsupported value type detected."));
			return EPR_Failed;
		}
		else
		{
			UE_LOG(LogInGameScript, Error, TEXT("[ScriptParser] Invalid token detected. %s"), *CurToken->ToString());
			return EPR_Failed;
		}
	}

	return PrefixExpression();
}

EParseResult FScriptParser::PrefixExpression()
{
	EParseResult Result = EPR_Failed;
	TSharedPtr<FTokenBase> CurToken = GetUnHandledToken();
	TSharedPtr<FASTUnaryOperatorNode> NewUnaryOpNode = nullptr;
	if (CurToken->TokenType == ETokenType::TT_UnaryOperator)
	{
		TSharedPtr<FOperatorToken> OpToken = StaticCastSharedPtr<FOperatorToken>(CurToken);
		if (OpToken)
		{
			// Create unary operator node
			NewUnaryOpNode = MakeShareable(new FASTUnaryOperatorNode);
			NewUnaryOpNode->SetOperatorType(OpToken->UnaryOperatorType);
			PopToken();
		}
		else
		{
			UE_LOG(LogInGameScript, Error, TEXT("[ScriptParser] Invalid unary operator token detected."));
			return EPR_Failed;
		}
	}

	Result = SuffixExpression();
	if (Result == EPR_Succeed && NewUnaryOpNode.IsValid())
	{
		// Construct Prefix Expression node if there is unary operator created
		TSharedPtr<FASTSuffixExpressionNode> SuffixExpressionNode = StaticCastSharedPtr<FASTSuffixExpressionNode>(ASTNodeStack.Top());
		if (SuffixExpressionNode)
		{
			TSharedPtr<FASTPrefixExpressionNode> NewPrefixExpressionNode = MakeShareable(new FASTPrefixExpressionNode);
			NewPrefixExpressionNode->SetUnaryOperatorNode(NewUnaryOpNode);
			NewPrefixExpressionNode->SetChildExpression(SuffixExpressionNode);
			NewPrefixExpressionNode->SetValueType(SuffixExpressionNode->GetValueType());

			// Pop used nodes and push new created one
			ASTNodeStack.Pop();
			ASTNodeStack.Push(NewPrefixExpressionNode);
		}
		else
		{
			UE_LOG(LogInGameScript, Error, TEXT("[ScriptParser] Invalid suffix expression node detected."));
			return EPR_Failed;
		}
	}

	return Result;
}

EParseResult FScriptParser::SuffixExpression()
{
	EParseResult Result = AtomicExpression();
	if (Result == EPR_Failed)
	{
		return Result;
	}

	// Check next char
	int32 OrgStackTop = ASTNodeStack.Num() - 1;
	TSharedPtr<FTokenBase> CurToken = GetUnHandledToken();
	while (true)
	{
		// Member accessor
		if (CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_Period))
		{
			PopToken();

			CurToken = GetUnHandledToken();
			if (CurToken->TokenType != ETokenType::TT_ID)
			{
				LOG_WITH_CHAR_POS(ELogVerbosity::Error, TEXT("[ScriptParser] Expected ID token here."), CurToken);
				return EPR_Failed;
			}
			TSharedPtr<FIDToken> IDToken = StaticCastSharedPtr<FIDToken>(CurToken);

			// Get outer node
			TSharedPtr<FASTIDNode> ClassIDNode = StaticCastSharedPtr<FASTIDNode>(ASTNodeStack.Top());
			if (!ClassIDNode.IsValid())
			{
				LOG_WITH_CHAR_POS(ELogVerbosity::Error, TEXT("[ScriptParser] Expected class name here."), CurToken);
				return EPR_Failed;
			}
			TSharedPtr<FASTClassTypeNode> ClassTypeNode = StaticCastSharedPtr<FASTClassTypeNode>(ClassIDNode->GetValue());
			TSharedPtr<FASTClassNode> ClassNode = ClassTypeNode->GetClassNode();
			FRegistryEntry* IDEntry = FindIDInClass(IDToken->IdName, ERegistryEntryType::RET_All, ClassNode);
			if (IDEntry == nullptr)
			{
				LOG_WITH_CHAR_POS(ELogVerbosity::Error, PARSE_ERROR_UNDEFINED_IDENTIFIER, CurToken);
				return EPR_Failed;
			}
			PopToken();

			TSharedPtr<FASTSuffixExpressionNode> OwnerNode = StaticCastSharedPtr<FASTSuffixExpressionNode>(ASTNodeStack.Top());

			// Create member accessor
			TSharedPtr<FASTMemberAccessorNode> MemberAccessor = MakeShareable(new FASTMemberAccessorNode);
			MemberAccessor->Initialize(*IDEntry);
			MemberAccessor->SetOwningNode(OwnerNode);
			switch (IDEntry->EntryType)
			{
			case ERegistryEntryType::RET_Variable:
				{
					TSharedPtr<FASTValueNode> MemberValue = StaticCastSharedPtr<FASTValueNode>(IDEntry->ASTNode);
					if (MemberValue.IsValid())
					{
						MemberAccessor->SetValueType(MemberValue->GetValueType());
					}
				} break;
			case ERegistryEntryType::RET_Function:
				{
					TSharedPtr<FASTFunctionNode> FunctionNode = StaticCastSharedPtr<FASTFunctionNode>(IDEntry->ASTNode);
					if (FunctionNode.IsValid())
					{
						TSharedPtr<FASTValueNode> ReturnValueNode = FunctionNode->GetReturnValueNode();
						MemberAccessor->SetValueType(ReturnValueNode->GetValueType());
					}
				} break;
			case ERegistryEntryType::RET_Class:
				{
					TSharedPtr<FASTClassNode> InnerClassNode = StaticCastSharedPtr<FASTClassNode>(IDEntry->ASTNode);
					if (InnerClassNode.IsValid())
					{
						MemberAccessor->SetValueType(EValueType::GetValueTypeFromClassName(InnerClassNode->GetClassID()));
					}
				} break;
			default: ;
			}

			PopNodes();
			ASTNodeStack.Push(MemberAccessor);
		}
		// Function invoker
		else if (CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_OpenParen))
		{
			PopToken();

			Result = FuncParam();
			if (Result == EPR_Failed)
			{
				return EPR_Failed;
			}

			// Check tailing ')'
			CurToken = GetUnHandledToken();
			if (!CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_CloseParen))
			{
				LOG_WITH_CHAR_POS(ELogVerbosity::Error, TEXT("[ScriptParser] Expected ')'."), CurToken);
				return EPR_Failed;
			}
			PopToken();

			// Create function invoker
			TSharedPtr<FASTFunctionInvokerNode> FunctionInvoker = MakeShareable(new FASTFunctionInvokerNode);
			int32 StackTop = ASTNodeStack.Num() - 1;
			TSharedPtr<FASTSuffixExpressionNode> FuncPathNode;
			if (Result == EPR_Succeed)
			{
				TSharedPtr<FASTFuncParamNode> FuncParam = StaticCastSharedPtr<FASTFuncParamNode>(ASTNodeStack.Top());
				FuncPathNode = StaticCastSharedPtr<FASTSuffixExpressionNode>(ASTNodeStack[StackTop - 1]);
				FunctionInvoker->SetFuncParamNode(FuncParam);
				FunctionInvoker->SetFuncPathNode(FuncPathNode);
				PopNodes();
			}
			else
			{
				FuncPathNode = StaticCastSharedPtr<FASTSuffixExpressionNode>(ASTNodeStack[StackTop]);
				FunctionInvoker->SetFuncPathNode(FuncPathNode);
			}
			FunctionInvoker->SetValueType(FuncPathNode->GetValueType());
			PopNodes();

			ASTNodeStack.Push(FunctionInvoker);
		}
		// Index accessor
		else if (CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_OpenBracket))
		{
			PopToken();

			Result = ComplexExpression();
			if (Result == EPR_Failed)
			{
				Result = EPR_Failed;
			}

			// Check tailing ']'
			CurToken = GetUnHandledToken();
			if (!CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_CloseBracket))
			{
				LOG_WITH_CHAR_POS(ELogVerbosity::Error, TEXT("[ScriptParser] Expected ']'."), CurToken);
				return EPR_Failed;
			}
			PopToken();

			int32 StackTop = ASTNodeStack.Num() - 1;
			TSharedPtr<FASTValueNode> ValueNode = StaticCastSharedPtr<FASTValueNode>(ASTNodeStack[StackTop]);
			TSharedPtr<FASTSuffixExpressionNode> OuterNode = StaticCastSharedPtr<FASTSuffixExpressionNode>(ASTNodeStack[StackTop - 1]);

			// Create index accessor
			TSharedPtr<FASTIndexAccessorNode> IndexAccessor = MakeShareable(new FASTIndexAccessorNode);
			IndexAccessor->SetIndexNode(ValueNode);
			IndexAccessor->SetOuterNode(OuterNode);
			IndexAccessor->SetValueType(OuterNode->GetValueType());

			PopNodes(2);
			ASTNodeStack.Push(IndexAccessor);
		}
		else
		{
			break;
		}
		CurToken = GetUnHandledToken();
	}

	return EPR_Succeed;
}

EParseResult FScriptParser::AtomicExpression()
{
	EParseResult Result = EPR_Succeed;
	TSharedPtr<FTokenBase> CurToken = GetUnHandledToken();
	if (CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_OpenParen))
	{
		PopToken();
		Result = ComplexExpression();
		if (Result == EPR_Failed)
		{
			return EPR_Failed;
		}

		// Consume following ')'
		CurToken = GetUnHandledToken();
		if (!CheckTokenSymbolType(CurToken, ESingleSymbolType::SST_CloseParen))
		{
			LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] No tailing ')'.", CurToken);
			return EPR_Failed;
		}
		PopToken();

		// TSharedPtr<FASTValueNode> ValueNode = StaticCastSharedPtr<FASTValueNode>(ASTNodeStack.Top());
		// if (ValueNode == nullptr)
		// {
		// 	UE_LOG(LogInGameScript, Error, TEXT("[ScriptParser] Failed to construct AST. ValueNode expected."));
		// 	return EPR_Failed;
		// }
	}
	else if (CurToken->TokenType == ETokenType::TT_ID)
	{
		TSharedPtr<FIDToken> IDToken = StaticCastSharedPtr<FIDToken>(CurToken);
		FRegistryEntry* FoundEntry = FindIDNode(IDToken->IdName, ERegistryEntryType::RET_All);
		if (FoundEntry == nullptr)
		{
			LOG_WITH_CHAR_POS(ELogVerbosity::Error, PARSE_ERROR_UNDEFINED_IDENTIFIER, CurToken);
			return EPR_Failed;
		}
		PopToken();

		// Push found node into stack
		ASTNodeStack.Push(FoundEntry->ASTNode);
	}
	else
	{
		// if (!CreateIDNode())
		// {
		// 	return EPR_Failed;
		// }
		//LOG_WITH_CHAR_POS(ELogVerbosity::Error, TEXT("[ScriptParser] Unsupported token."), OwningLexer);
		return EPR_Failed;
	}

	return Result;
}

bool FScriptParser::CreateIDNode()
{
	// This should be a ID token
	TSharedPtr<FTokenBase> CurToken = GetUnHandledToken();
	if (CurToken && CurToken->TokenType == ETokenType::TT_ID)
	{
		TSharedPtr<FIDToken> IDToken = StaticCastSharedPtr<FIDToken>(CurToken);
		if (IDToken)
		{
			TSharedPtr<FASTIDNode> NewIDNode = MakeShareable(new FASTIDNode);
			NewIDNode->SetID(IDToken->IdName);
			ASTNodeStack.Push(NewIDNode);
			PopToken();
		}
		else
		{
			LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] Invalid ID token detected.", CurToken);
			return false;
		}
	}
	else
	{
		//LOG_WITH_CHAR_POS(ELogVerbosity::Error, "[ScriptParser] ID token expected.", OwningLexer);
		return false;
	}

	return true;
}

FRegistryEntry* FScriptParser::FindIDNode(FName NodeName, int32 EntryType)
{
	// Search in current registry
	int32 RegistryIndex = RegistryStack.Num() - 1;

	FRegistryEntry* Result;
	do
	{
		TSharedPtr<FRegistry> FindSource = RegistryStack[RegistryIndex];
		Result = FindSource->GetRegistryEntry(NodeName, EntryType);

		// Find in outer registries
		RegistryIndex--;
	}
	while (RegistryIndex >= 0 && Result == nullptr);

	return Result;
}

FRegistryEntry* FScriptParser::FindIDInClass(FName NodeName, int32 EntryType, TSharedPtr<FASTClassNode> ClassNode)
{
	if (!ClassNode.IsValid())
	{
		return nullptr;
	}

	TSharedPtr<FRegistry> ClassRegistry = ClassNode->GetClassRegistry();
	return ClassRegistry->GetRegistryEntry(NodeName, EntryType);
}

TSharedPtr<FTokenBase> FScriptParser::GetNextToken(bool bPushToStack)
{
	if (OwningLexer.IsValid())
	{
		TSharedPtr<FTokenBase> NextToken = OwningLexer->GetNextToken();
		if (NextToken != nullptr)
		{
			if (bPushToStack)
			{
				// Increase CurTokenPointer if it's pointing to last one
				if (CurTokenPointer == TokenStack.Num() - 1)
				{
					CurTokenPointer++;
				}

				// Push into token stack
				TokenStack.Push(NextToken);
			}

			if (bEnableTokenCache)
			{
				CacheToken(NextToken);
			}

			return NextToken;
		}
	}

	return nullptr;
}

TSharedPtr<FTokenBase> FScriptParser::GetCurrentToken()
{
	TSharedPtr<FTokenBase> Result = TokenStack.Num() > CurTokenPointer ? TokenStack[CurTokenPointer] : nullptr;
	if (bEnableTokenCache)
	{
		CacheToken(Result);
	}
	return Result;
}

TSharedPtr<FTokenBase> FScriptParser::GetUnHandledToken()
{
	return TokenStack.Num() > 0 ? GetCurrentToken() : GetNextToken();
}

bool FScriptParser::PopNodes(int32 Count)
{
	if (ASTNodeStack.Num() < Count)
	{
		UE_LOG(LogInGameScript, Error, TEXT("[ScriptParser] No enough nodes in stack. Expected %d."), Count);
		return false;
	}

	for (int32 i = 0; i < Count; ++i)
	{
		ASTNodeStack.Pop();
	}
	return true;
}

void FScriptParser::PopToken(int32 PopCount)
{
	for (int32 i = 0; i < PopCount; ++i)
	{
		if (TokenStack.Num() > 0)
		{
			TokenStack.Pop();

			// Validate CurTokenPointer
			CurTokenPointer = CurTokenPointer >= TokenStack.Num() ? TokenStack.Num() - 1 : CurTokenPointer;
		}
		else
		{
			UE_LOG(LogInGameScript, Warning, TEXT("[ScriptParser] Trying to pop token from empty stack."));
			break;
		}
	}
}

void FScriptParser::StartTokenCache()
{
	bEnableTokenCache = true;

	// Record current cache length
	TokenCacheOrigin.Push(TokenCache.Num());
}

void FScriptParser::CacheToken(TSharedPtr<FTokenBase> InToken)
{
	if (!TokenCache.Contains(InToken))
	{
		// Insert new element at first position to remain the order
		//TokenCache.Insert(InToken, 0);
		TokenCache.Add(InToken);
	}
}

void FScriptParser::RecoverTokenFromCache()
{
	if (TokenCacheOrigin.Num() == 0)
	{
		bEnableTokenCache = false;
		return;
	}

	// Get the origin index of last cache
	int32 Origin = TokenCacheOrigin.Pop();

	// Append caches in range to the end of token stack (and avoid duplicated instances)
	// Reverse order
	for (int32 Index = TokenCache.Num() - 1; Index >= Origin; --Index)
	{
		TokenStack.AddUnique(TokenCache[Index]);
		TokenCache.RemoveAt(Index);
	}

	// Set the pointer to end of stack
	CurTokenPointer = TokenStack.Num() - 1;

	if (TokenCacheOrigin.Num() == 0)
	{
		bEnableTokenCache = false;
	}
}

void FScriptParser::ClearTokenCache()
{
	if (TokenCacheOrigin.Num() == 0)
	{
		return;
	}

	// Get the origin index of last cache
	int32 Origin = TokenCacheOrigin.Pop();

	for (int32 Index = TokenCache.Num() - 1; Index >= Origin; --Index)
	{
		TokenCache.RemoveAt(Index);
	}

	if (TokenCacheOrigin.Num() == 0)
	{
		bEnableTokenCache = false;
	}
}

bool FScriptParser::CheckTokenSymbolType(TSharedPtr<FTokenBase> CheckToken, ESingleSymbolType::Type ExpectType) const
{
	if (CheckToken->TokenType != ETokenType::TT_SingleSymbol)
	{
		return false;
	}

	TSharedPtr<FSingleSymbolToken> SymbolToken = StaticCastSharedPtr<FSingleSymbolToken>(CheckToken);
	if (!SymbolToken.IsValid())
		return false;

	return SymbolToken->SymbolType == ExpectType;
}

TSharedPtr<FScriptParser> FScriptParserFactory::CreateScriptParser(FString FilePath)
{
	return MakeShareable(new FScriptParser(FilePath));
}
