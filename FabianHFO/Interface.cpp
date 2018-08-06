/**********************************************************************************************//**
 * \file	Interface.cpp.
 *
 * Implements the interface class
 **************************************************************************************************/

#include "StdAfx.h"
#include "Interface.h"
#include "MVModel.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CInterface class
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

CInterface::CInterface(void)
{
	m_pModel=NULL;

}

/**********************************************************************************************//**
 * Finalizes an instance of the CInterface class
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

CInterface::~CInterface(void)
{
}

/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CInterface::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * Query if 'oldTickCount' is safe tick count delay expired
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	oldTickCount	Number of old ticks.
 * \param	delay			The delay.
 *
 * \return	True if safe tick count delay expired, false if not.
 **************************************************************************************************/

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









