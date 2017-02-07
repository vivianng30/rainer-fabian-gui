#include "StdAfx.h"
#include "Alarm.h"


CAlarm::CAlarm(eAlarm	enAlarm,eAlarmType enType,eAlarmPrio enPriority,eStateOfAlarm	enAlarmState,UINT iDelaySec):
		m_enAlarm(enAlarm),m_enType(enType),m_enPriority(enPriority),m_enAlarmState(enAlarmState),m_iAlarmDelayTimeMiliSec(iDelaySec*1000)
{
	InitializeCriticalSection(&csAlarmLock);
	
	m_dwAlarmTriggerTime=0;
}
CAlarm::CAlarm():
m_enAlarm(AL_Accu_Empty),m_enType(AT_NONE),m_enPriority(AP_NONE),m_enAlarmState(AS_NONE),m_iAlarmDelayTimeMiliSec(0)
{
	InitializeCriticalSection(&csAlarmLock);
	
	m_dwAlarmTriggerTime=0;
}
CAlarm::~CAlarm(void)
{
	DeleteCriticalSection(&csAlarmLock);
}
eAlarmType CAlarm::getAlarmType()
{
	EnterCriticalSection(&csAlarmLock);
	eAlarmType enType=m_enType;
	LeaveCriticalSection(&csAlarmLock);
	return enType;
}
void CAlarm::setAlarmType(eAlarmType type)
{
	EnterCriticalSection(&csAlarmLock);
	m_enType=type;
	LeaveCriticalSection(&csAlarmLock);
}


void CAlarm::setTriggerTimestamp()
{
	EnterCriticalSection(&csAlarmLock);
	m_dwAlarmTriggerTime=GetTickCount();
	LeaveCriticalSection(&csAlarmLock);
}
void CAlarm::resetTriggerTimestamp()
{
	EnterCriticalSection(&csAlarmLock);
	m_dwAlarmTriggerTime=0;
	LeaveCriticalSection(&csAlarmLock);
}

DWORD CAlarm::getTriggerTimestamp()
{
	EnterCriticalSection(&csAlarmLock);
	DWORD trigger=m_dwAlarmTriggerTime;
	LeaveCriticalSection(&csAlarmLock);
	
	return trigger;
}
eAlarm CAlarm::getAlarm()
{
	EnterCriticalSection(&csAlarmLock);
	eAlarm enAlarm=m_enAlarm;
	LeaveCriticalSection(&csAlarmLock);
	return enAlarm;
}
void CAlarm::setALARM(eAlarm alarm)
{
	EnterCriticalSection(&csAlarmLock);
	m_enAlarm=alarm;
	LeaveCriticalSection(&csAlarmLock);
}

eAlarmPrio CAlarm::getAlarmPriority()
{
	EnterCriticalSection(&csAlarmLock);
	eAlarmPrio enPriority=m_enPriority;
	LeaveCriticalSection(&csAlarmLock);
	return enPriority;
}
void CAlarm::setAlarmPriority(eAlarmPrio prio)
{
	EnterCriticalSection(&csAlarmLock);
	m_enPriority=prio;
	LeaveCriticalSection(&csAlarmLock);
}

eStateOfAlarm CAlarm::getAlarmState()
{
	EnterCriticalSection(&csAlarmLock);
	eStateOfAlarm enAlarmState=m_enAlarmState;
	LeaveCriticalSection(&csAlarmLock);
	return enAlarmState;
}
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
void CAlarm::setAlarmDelayTimeSec(UINT delayTimeSec)
{
	EnterCriticalSection(&csAlarmLock);
	m_iAlarmDelayTimeMiliSec=delayTimeSec*1000;
	m_dwAlarmTriggerTime=0;
	LeaveCriticalSection(&csAlarmLock);
}
UINT CAlarm::getAlarmDelayTimeMSec()
{
	EnterCriticalSection(&csAlarmLock);
	UINT delayTime=m_iAlarmDelayTimeMiliSec;
	LeaveCriticalSection(&csAlarmLock);
	return delayTime;
}


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