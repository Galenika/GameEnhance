// IClientEntityList
#pragma once

class IClientEntityList {
public:
	virtual void PAD0() = 0;
	virtual void PAD1() = 0;
	virtual void PAD2() = 0;
	virtual void* GetClientEntity(int iIndex) = 0;
	virtual void* GetClientEntityFromHandle(void* pHandle) = 0;
	virtual int NumberOfEntities(bool bNonNetworkable) = 0;
	virtual int GetHighestEntityIndex() = 0;
	virtual void SetMaxEntities(int iMax) = 0;
	virtual void GetMaxEntities() = 0;
}; extern IClientEntityList* g_pIEntList;
