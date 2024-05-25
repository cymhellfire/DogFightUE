#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_CLASS(LogDogFightGame, Log, All);

#define VA_ARGS(...)	, ##__VA_ARGS__

#define DFLog(InFormat, ...)	\
	UE_LOG(LogDogFightGame, Log, TEXT("[%s:%d] %s"), *FString(__FUNCTION__), __LINE__, *FString::Printf(InFormat VA_ARGS(__VA_ARGS__)))

#define DFLogW(InFormat, ...)	\
	UE_LOG(LogDogFightGame, Warning, TEXT("[%s:%d] %s"), *FString(__FUNCTION__), __LINE__, *FString::Printf(InFormat VA_ARGS(__VA_ARGS__)))

#define DFLogE(InFormat, ...)	\
	UE_LOG(LogDogFightGame, Error, TEXT("[%s:%d] %s"), *FString(__FUNCTION__), __LINE__, *FString::Printf(InFormat VA_ARGS(__VA_ARGS__)))