// Dog Fight Game Code By CYM.


#include "Subsystem/ProtobufSubsystem.h"

#include <fstream>

#include "ProtobufLog.h"
#include "GenProto/Config.pb.h"
#include "GenProto/pb_header_v3.pb.h"
#include "Utils/ProtobufReader.h"

void UProtobufSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// LoadConfigs();

	LoadByReader();
}

void UProtobufSubsystem::Deinitialize()
{
	Super::Deinitialize();

	// Clear all loaded configs
	ProtobufReaderMap.Empty();
}

void UProtobufSubsystem::LoadConfigs()
{
	FString ContentDir = FPaths::ProjectContentDir();
	// const FString FilePath = TEXT("F:\\DogFightRe\\Content\\ProtobufData\\Card_cfg.bin");
	const FString FilePath = ContentDir / TEXT("ProtobufData\\Card_cfg.bin");
	std::fstream fin;
	fin.open(*FilePath, std::ios::in | std::ios::binary);
	if (!fin.is_open())
	{
		DFLogE(LogProtobuf, TEXT("Failed to open file %s"), *FilePath);
		return;
	}

	org::xresloader::pb::xresloader_datablocks DataWrapper;
	if (!DataWrapper.ParseFromIstream(&fin))
	{
		DFLogE(LogProtobuf, TEXT("Parse file %s failed"), *FilePath);
		return;
	}

	for (int32 i = 0; i < DataWrapper.data_block_size(); ++i)
	{
		Card_cfg CardCfg;
		if (CardCfg.ParseFromString(DataWrapper.data_block(i)))
		{
			DFLog(LogProtobuf, TEXT("Parsing %s"), *FString(DataWrapper.data_block(i).c_str()));

			DFLog(LogProtobuf, TEXT("Cfg[%u] Name[%s] CostType[%d]"), CardCfg.id(), *FString(UTF8_TO_TCHAR(CardCfg.name().c_str())), CardCfg.costtype());
		}
	}
}

#define READER_TYPE_NAME(ConfigName, KeyName, KeyType)		TProtobufReader<ConfigName, KeyType, &##ConfigName##::##KeyName>

#define DECLARE_LOAD_CONFIG(ConfigName, KeyName, KeyType)	\
	{	\
		TSharedPtr<READER_TYPE_NAME(ConfigName, KeyName, KeyType)> Reader = MakeShareable(new READER_TYPE_NAME(ConfigName, KeyName, KeyType)(TEXT(#ConfigName)));	\
		if (Reader->IsLoaded())	\
		{	\
			ProtobufReaderMap.Add(TEXT(#ConfigName), Reader);	\
		}	\
	}

void UProtobufSubsystem::LoadByReader()
{
	const FString ConfigName(TEXT("Card_cfg"));
	// TSharedPtr<TProtobufReader<Card_cfg, uint32_t, &Card_cfg::id>> Reader = MakeShareable(new TProtobufReader<Card_cfg, uint32_t, &Card_cfg::id>(ConfigName));
	//
	// if (Reader->IsLoaded())
	// {
	// 	for (auto& Record : Reader->GetAllConfig())
	// 	{
	// 		Card_cfg& Cfg = Record.Value;
	// 		DFLog(LogProtobuf, TEXT("Key %d: Name[%s] CostType[%d]"), Record.Key, *FString(UTF8_TO_TCHAR(Cfg.name().c_str())), Cfg.costtype());
	// 	}
	//
	// 	ProtobufReaderMap.Add(ConfigName, Reader);
	// }

	DECLARE_LOAD_CONFIG(Card_cfg, id, uint32_t)

	// Test code
	if (auto CardCfgReaderPtr = ProtobufReaderMap.Find(ConfigName))
	{
		auto CardCfgReader = *CardCfgReaderPtr;
		if (CardCfgReader.IsValid())
		{
			// if (auto CastedReader = CardCfgReader->Cast<Card_cfg, uint32_t, &Card_cfg::id>())
			if (auto CastedReader = reinterpret_cast<READER_TYPE_NAME(Card_cfg, id, uint32_t)*>(CardCfgReader.Get()))
			{
				for (auto& Record : CastedReader->GetAllConfig())
				{
					Card_cfg& Cfg = Record.Value;
					DFLog(LogProtobuf, TEXT("Key %d: Name[%s] CostType[%d]"), Record.Key, *FString(UTF8_TO_TCHAR(Cfg.name().c_str())), Cfg.costtype());
				}
			}
		}
	}

	DFLog(LogProtobuf, TEXT("All config loaded"));
}
