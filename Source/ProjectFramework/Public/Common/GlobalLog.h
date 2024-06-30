#pragma once

#define VA_ARGS(...)	, ##__VA_ARGS__

#define DFLog(CategoryName, InFormat, ...)	\
	UE_LOG(CategoryName, Log, TEXT("[%s:%d] %s"), *FString(__FUNCTION__), __LINE__, *FString::Printf(InFormat VA_ARGS(__VA_ARGS__)))

#define DFLogW(CategoryName, InFormat, ...)	\
	UE_LOG(CategoryName, Warning, TEXT("[%s:%d] %s"), *FString(__FUNCTION__), __LINE__, *FString::Printf(InFormat VA_ARGS(__VA_ARGS__)))

#define DFLogE(CategoryName, InFormat, ...)	\
	UE_LOG(CategoryName, Error, TEXT("[%s:%d] %s"), *FString(__FUNCTION__), __LINE__, *FString::Printf(InFormat VA_ARGS(__VA_ARGS__)))
