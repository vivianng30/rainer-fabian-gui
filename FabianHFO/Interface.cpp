#include "StdAfx.h"
#include "Interface.h"
#include "MVModel.h"




CInterface::CInterface(void)
{
	m_pModel=NULL;

}

CInterface::~CInterface(void)
{
}

// **************************************************************************
// 
// **************************************************************************
CMVModel *CInterface::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


// **************************************************************************
// 	Time measurement
// **************************************************************************
//rkuTICKCOUNT
bool CInterface::isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay)////used to check if old tick count plus delay is still lower than actual tickCount, (dwLastTickCount+DELAY<getTickCount64())
{
	bool bExpired=false;
	DWORD curTickCount=GetTickCount();

	if(curTickCount<oldTickCount)//overrun
	{
		DWORD diff=0xFFFFFFFF-oldTickCount;

		if(diff+delay<curTickCount)
		{
			bExpired=true;
		}
	}
	else
	{
		if(oldTickCount+delay<curTickCount)
		{
			bExpired=true;
		}
	}
	return bExpired;
}









