#pragma once

/**
 * Print a log with source file information of given token.
 */
#define LOG_WITH_CHAR_POS(LogLevel, LogContent, InToken) \
	FScriptLexer::LogWithToken(InToken, LogContent, LogLevel)

/**
 * Define a private member variable with getter and setter.
 */
#define DEFINE_CHILD_VARIABLE(VariableType, VariableName) \
	private: \
		VariableType VariableName; \
	public: \
		VariableType Get##VariableName() \
		{ \
			return VariableName; \
		} \
		void Set##VariableName(VariableType NewValue) \
		{ \
			VariableName = NewValue; \
		}

/**
 * Define a protected member variable with getter and setter.
 */
#define DEFINE_CHILD_VARIABLE_PROTECTED(VariableType, VariableName) \
	protected: \
		VariableType VariableName; \
	public: \
		VariableType Get##VariableName() \
		{ \
			return VariableName; \
		} \
		void Set##VariableName(VariableType NewValue) \
		{ \
			VariableName = NewValue; \
		}

/**
 * Define a private member variable with getter only.
 */
#define DEFINE_CHILD_VARIABLE_GETTER(VariableType, VariableName) \
	private: \
		VariableType VariableName; \
	public: \
		VariableType Get##VariableName() \
		{ \
			return VariableName; \
		}

/**
 * Define a protected member variable with getter only.
 */
#define DEFINE_CHILD_VARIABLE_GETTER_PROTECTED(VariableType, VariableName) \
	protected: \
		VariableType VariableName; \
	public: \
		VariableType Get##VariableName() \
		{ \
			return VariableName; \
		}

/**
 * Define a private child variable reference by shared pointer.
 * @param NodeType		Variable type.
 * @param NodeName		Variable name.
 */
#define DEFINE_CHILD_SHARED_PTR(NodeType, NodeName) \
	private: \
		TSharedPtr<NodeType> NodeName; \
	public: \
		TSharedPtr<NodeType> Get##NodeName() \
		{ \
			return NodeName; \
		} \
		\
		void Set##NodeName(TSharedPtr<NodeType> NewNode) \
		{ \
			NodeName = NewNode; \
		}

/**
 * Define a protected child variable reference by shared pointer.
 * @param NodeType		Variable type.
 * @param NodeName		Variable name.
 */
#define DEFINE_CHILD_SHARED_PTR_PROTECTED(NodeType, NodeName) \
	protected: \
		TSharedPtr<NodeType> NodeName; \
	public: \
		TSharedPtr<NodeType> Get##NodeName() \
		{ \
			return NodeName; \
		} \
		\
		void Set##NodeName(TSharedPtr<NodeType> NewNode) \
		{ \
			NodeName = NewNode; \
		}

/**
 * Define a private child member shared pointer with array format.
 * @param NodeType		Variable type.
 * @param NodeName		Variable name.
 */
#define DEFINE_CHILD_SHARED_PTR_ARRAY(NodeType, NodeName) \
	private: \
		TArray<TSharedPtr<NodeType>> NodeName; \
	public: \
		TArray<TSharedPtr<NodeType>> Get##NodeName() \
		{ \
			return NodeName; \
		} \
		void AddTo##NodeName(TSharedPtr<NodeType> NewNode) \
		{ \
			NodeName.Add(NewNode); \
		} \
		void RemoveFrom##NodeName(TSharedPtr<NodeType> RemoveNode) \
		{ \
			NodeName.Remove(RemoveNode); \
		} \
		void Clear##NodeName() \
		{ \
			NodeName.Empty(); \
		}
