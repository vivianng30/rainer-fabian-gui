/**********************************************************************************************//**
 * \file	Alarm.cpp.
 *
 * Implements the alarm class
 **************************************************************************************************/

#include "StdAfx.h"
#include "Alarm.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CAlarm class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	enAlarm			The en alarm.
 * \param	enType			Type of the en.
 * \param	enPriority  	The en priority.
 * \param	enAlarmState	State of the en alarm.
 * \param	iDelaySec   	Zero-based index of the delay security.
 **************************************************************************************************/

CAlarm::CAlarm(eAlarm	enAlarm,eAlarmType enType,eAlarmPrio enPriority,eStateOfAlarm	enAlarmState,UINT iDelaySec):
		m_enAlarm(enAlarm),m_enType(enType),m_enPriority(enPriority),m_enAlarmState(enAlarmState),m_iAlarmDelayTimeMiliSec(iDelaySec*1000)
{
	InitializeCriticalSection(&csAlarmLock);
	
	m_dwAlarmTriggerTime=0;
}

/**********************************************************************************************//**
 * Initializes a new instance of the CAlarm class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CAlarm::CAlarm():
m_enAlarm(AL_Accu_Empty),m_enType(AT_NONE),m_enPriority(AP_NONE),m_enAlarmState(AS_NONE),m_iAlarmDelayTimeMiliSec(0)
{
	InitializeCriticalSection(&csAlarmLock);
	
	m_dwAlarmTriggerTime=0;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CAlarm class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CAlarm::~CAlarm(void)
{
	DeleteCriticalSection(&csAlarmLock);
}

/**********************************************************************************************//**
 * Gets alarm type
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The alarm type.
 **************************************************************************************************/

eAlarmType CAlarm::getAlarmType()
{
	EnterCriticalSection(&csAlarmLock);
	eAlarmType enType=m_enType;
	LeaveCriticalSection(&csAlarmLock);
	return enType;
}

/**********************************************************************************************//**
 * Sets alarm type
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	type	The type.
 **************************************************************************************************/

void CAlarm::setAlarmType(eAlarmType type)
{
	EnterCriticalSection(&csAlarmLock);
	m_enType=type;
	LeaveCriticalSection(&csAlarmLock);
}

/**********************************************************************************************//**
 * Sets trigger timestamp
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CAlarm::setTriggerTimestamp()
{
	EnterCriticalSection(&csAlarmLock);
	m_dwAlarmTriggerTime=GetTickCount();
	LeaveCriticalSection(&csAlarmLock);
}

/**********************************************************************************************//**
 * Resets the trigger timestamp
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CAlarm::resetTriggerTimestamp()
{
	EnterCriticalSection(&csAlarmLock);
	m_dwAlarmTriggerTime=0;
	LeaveCriticalSection(&csAlarmLock);
}

/**********************************************************************************************//**
 * Gets trigger timestamp
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The trigger timestamp.
 **************************************************************************************************/

DWORD CAlarm::getTriggerTimestamp()
{
	EnterCriticalSection(&csAlarmLock);
	DWORD trigger=m_dwAlarmTriggerTime;
	LeaveCriticalSection(&csAlarmLock);
	
	return trigger;
}

/**********************************************************************************************//**
 * Gets the alarm
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The alarm.
 **************************************************************************************************/

eAlarm CAlarm::getAlarm()
{
	EnterCriticalSection(&csAlarmLock);
	eAlarm enAlarm=m_enAlarm;
	LeaveCriticalSection(&csAlarmLock);
	return enAlarm;
}

/**********************************************************************************************//**
 * Sets a larm
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	alarm	The alarm.
 **************************************************************************************************/

void CAlarm::setALARM(eAlarm alarm)
{
	EnterCriticalSection(&csAlarmLock);
	m_enAlarm=alarm;
	LeaveCriticalSection(&csAlarmLock);
}

/**********************************************************************************************//**
 * Gets alarm priority
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The alarm priority.
 **************************************************************************************************/

eAlarmPrio CAlarm::getAlarmPriority()
{
	EnterCriticalSection(&csAlarmLock);
	eAlarmPrio enPriority=m_enPriority;
	LeaveCriticalSection(&csAlarmLock);
	return enPriority;
}

/**********************************************************************************************//**
 * Sets alarm priority
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	prio	The prio.
 **************************************************************************************************/

void CAlarm::setAlarmPriority(eAlarmPrio prio)
{
	EnterCriticalSection(&csAlarmLock);
	m_enPriority=prio;
	LeaveCriticalSection(&csAlarmLock);
}

/**********************************************************************************************//**
 * Gets alarm state
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The alarm state.
 **************************************************************************************************/

eStateOfAlarm CAlarm::getAlarmState()
{
	EnterCriticalSection(&csAlarmLock);
	eStateOfAlarm enAlarmState=m_enAlarmState;
	LeaveCriticalSection(&csAlarmLock);
	return enAlarmState;
}

/**********************************************************************************************//**
 * Sets alarm state
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarm::setAlarmState(eStateOfAlarm state)
{
	EnterCriticalSection(&csAlarmLock);
	m_enAlarmState=state;
	LeaveCriticalSection(&csAlarmLock);
	
	if(AS_ACTIVE==state) 
	{
		setTriggerTimestamp();
	}
	else
	{
		resetTriggerTimestamp();
	}
}

/**********************************************************************************************//**
 * Sets alarm delay time security
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	delayTimeSec	The delay time security.
 **************************************************************************************************/

void CAlarm::setAlarmDelayTimeSec(UINT delayTimeSec)
{
	EnterCriticalSection(&csAlarmLock);
	m_iAlarmDelayTimeMiliSec=delayTimeSec*1000;
	m_dwAlarmTriggerTime=0;
	LeaveCriticalSection(&csAlarmLock);
}

/**********************************************************************************************//**
 * Gets alarm delay time m security
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The alarm delay time m security.
 **************************************************************************************************/

UINT CAlarm::getAlarmDelayTimeMSec()
{
	EnterCriticalSection(&csAlarmLock);
	UINT delayTime=m_iAlarmDelayTimeMiliSec;
	LeaveCriticalSection(&csAlarmLock);
	return delayTime;
}

/**********************************************************************************************//**
 * Query if 'oldTickCount' is safe tick count delay expired
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	oldTickCount	Number of old ticks.
 * \param	delay			The delay.
 *
 * \return	True if safe tick count delay expired, false if not.
 **************************************************************************************************/

bool CAlarm::isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay)////used to check if old tick count plus delay is still lower than actual tickCount, (dwLastTickCount+DELAY<getTickCount64())
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