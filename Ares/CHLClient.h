// CHLClient
#pragma once

// Includes
#include "dt\dt_common.h"
#include "dt\dt_recv.h"

class ClientClass {
public:
	BYTE _chPadding[8];
	char* szName;
	RecvTable* rtTable;
	ClientClass* pNextClass;
	int iClassID;
};

class CHLClient {
public:
	ClientClass* GetAllClasses();
}; extern CHLClient* g_pClient;