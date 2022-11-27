#pragma once

#define TO_NET_ROLE_STR(Role) \
	(Role == ROLE_Authority ? TEXT("Host") : TEXT("Client"))
