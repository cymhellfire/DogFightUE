#pragma once

#include <fstream>

#include "CoreMinimal.h"
#include "ProtobufLog.h"
#include "GenProto/pb_header_v3.pb.h"
#include "GenProto/xresloader.pb.h"

#define PROTOBUF_DATA_PATH TEXT("ProtobufData")

template<typename ProtoType, typename KeyType, KeyType (ProtoType::*KeyGetter)() const>
class TProtobufReader;

class FProtobufReaderBase
{
public:
	FProtobufReaderBase(const FString& InName)
		: bLoaded(false)
		, ConfigName(InName)
	{}

	virtual ~FProtobufReaderBase()
	{
		DFLog(LogProtobuf, TEXT("Config %s unloaded."), *ConfigName);
	}

	void ReloadConfig()
	{
		if (ConfigName.IsEmpty())
		{
			DFLogE(LogProtobuf, TEXT("Empty config name!"));
			return;
		}
		const FString ContentDir = FPaths::ProjectContentDir();
		const FString FilePath = FString::Printf(TEXT("%s%s/%s.bin"), *ContentDir, PROTOBUF_DATA_PATH, *ConfigName);

		std::fstream ConfigStream;
		ConfigStream.open(*FilePath, std::ios::binary | std::ios::in);
		if (!ConfigStream.is_open())
		{
			DFLogE(LogProtobuf, TEXT("Failed to open config file %s"), *FilePath);
			return;
		}

		DataBlockType DataBlocks;
		if (!DataBlocks.ParseFromIstream(&ConfigStream))
		{
			DFLogE(LogProtobuf, TEXT("Failed to parse config file %s"), *FilePath);
			return;
		}

		if (!HandleLoadedData(DataBlocks))
		{
			DFLogE(LogProtobuf, TEXT("Failed to handle loaded data from %s"), *FilePath);
			return;
		}

		bLoaded = true;
		DFLog(LogProtobuf, TEXT("Config %s loaded"), *FilePath);
	}

	bool IsLoaded() const
	{
		return bLoaded;
	}

	FString GetName() const
	{
		return ConfigName;
	}

	template<typename ProtoType, typename KeyType, KeyType (ProtoType::*KeyGetter)() const>
	TProtobufReader<ProtoType, KeyType, KeyGetter>* Cast()
	{
		return reinterpret_cast<TProtobufReader<ProtoType, KeyType, KeyGetter>*>(this);
	}

protected:
	using DataBlockType = org::xresloader::pb::xresloader_datablocks;

	virtual bool HandleLoadedData(const DataBlockType& DataBlock) = 0;

protected:
	bool bLoaded;
	FString ConfigName;
};

template<typename ProtoType, typename KeyType, KeyType (ProtoType::*KeyGetter)() const>
class TProtobufReader : public FProtobufReaderBase
{
public:
	TProtobufReader(const FString& InName)
		: FProtobufReaderBase(InName)
	{
		ReloadConfig();
	}

	TMap<KeyType, ProtoType> GetAllConfig() const
	{
		return DataMap;
	}

	ProtoType* GetConfig(KeyType Key) const
	{
		if (DataMap.Contains(Key))
		{
			return &(DataMap[Key]);
		}

		return nullptr;
	}

protected:
	virtual bool HandleLoadedData(const DataBlockType& DataBlock) override
	{
		for (int32 i = 0; i < DataBlock.data_block_size(); ++i)
		{
			ProtoType ProtoItem;
			if (ProtoItem.ParseFromString(DataBlock.data_block(i).c_str()))
			{
				KeyType Key = (ProtoItem.*KeyGetter)();
				DataMap.Emplace(Key, ProtoItem);
			}
		}
		return true;
	}

private:
	TMap<KeyType, ProtoType> DataMap;
};
