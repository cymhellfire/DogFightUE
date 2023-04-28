// Dog Fight Game Code By CYM.


#include "GameService/BuffService.h"

UNewBuffBase* UBuffService::CreateBuff(int32 BuffId)
{
	return CreateBuffById(BuffId);
}
