#include "Dumping.h"
/*
#include "Dumping.h"

#define DUMPIDTOFILE

void Dump::DumpClassIds()
{
#ifdef DUMPIDTOFILE
	Utilities::EnableLogFile("ClassID.txt");
#endif
	ClientClass* cClass = Interfaces::Client->GetAllClasses();
	while (cClass)
	{
		Utilities::Log("%s = %d,", cClass->m_pNetworkName, cClass->m_ClassID);
		cClass = cClass->m_pNext;
	}
}
*/

void Dump::DumpClassIds()
{
	Utilities::EnableLogFile("ClassID - RedHeadPro.txt");
	Utilities::Log("---------------------------- Class IDs CSGO Poop ----------------------------");
	Utilities::Log("enum eClientClass {");
	for (ClientClass *pClass = Interfaces::Client->GetAllClasses(); pClass; pClass = pClass->m_pNext)
		Utilities::Log("\t%s = %i,", pClass->m_pNetworkName, pClass->m_ClassID);
	Utilities::Log("}");

}
