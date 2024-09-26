// Dog Fight Game Code By CYM.


#include "Subsystem/ProtobufSubsystem.h"

#include <fstream>

#include "ProtobufLog.h"
#include "GenProto/Config.pb.h"
#include "GenProto/pb_header_v3.pb.h"

void UProtobufSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadConfigs();
}

void UProtobufSubsystem::LoadConfigs()
{
	const FString FilePath = TEXT("F:\\DogFightRe\\Content\\ProtobufData\\Card_cfg.bin");
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
