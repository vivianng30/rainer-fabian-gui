#include "StdAfx.h"
#include "AlarmLimit.h"

CAlarmLimit::CAlarmLimit(eAlarmLimits enAlarmLimit,
						 eAlarmLimitState eLimitState,
						 int iCurValue,
						 int iMax,
						 int iMin):
		m_enAlarmLimit(enAlarmLimit),m_eLimitState(eLimitState),m_iCurValue(iCurValue),m_iMax(iMax),m_iMin(iMin)
{
	InitializeCriticalSection(&csLimitLock);
}
CAlarmLimit::CAlarmLimit():
m_enAlarmLimit(LI_MVMAX),m_eLimitState(AL_OFF),m_iCurValue(0),m_iMax(0),m_iMin(0)
{
	InitializeCriticalSection(&csLimitLock);
}


CAlarmLimit::~CAlarmLimit(void)
{
	DeleteCriticalSection(&csLimitLock);
}
eAlarmLimits CAlarmLimit::getAlarmLimit()
{
	EnterCriticalSection(&csLimitLock);
	eAlarmLimits alarmLimit=m_enAlarmLimit;
	LeaveCriticalSection(&csLimitLock);
	return alarmLimit;
}
void CAlarmLimit::setAlarmLimit(eAlarmLimits alarmLimit)
{
	EnterCriticalSection(&csLimitLock);
	m_enAlarmLimit=alarmLimit;
	LeaveCriticalSection(&csLimitLock);
}
int CAlarmLimit::getCurValue()
{	
	EnterCriticalSection(&csLimitLock);
	int iVal=m_iCurValue;
	LeaveCriticalSection(&csLimitLock);
	return iVal;
}
void CAlarmLimit::setCurValue(int iVal)
{
	EnterCriticalSection(&csLimitLock);
	m_iCurValue=iVal;
	LeaveCriticalSection(&csLimitLock);
}

int CAlarmLimit::getMaxValue()
{
	int iVal=0;

	EnterCriticalSection(&csLimitLock);
	iVal=m_iMax;
	LeaveCriticalSection(&csLimitLock);

	return iVal;
}
void CAlarmLimit::setMaxValue(int iVal)
{
	EnterCriticalSection(&csLimitLock);
	m_iMax=iVal;
	LeaveCriticalSection(&csLimitLock);
}

int CAlarmLimit::getMinValue()
{
	int iVal=0;

	EnterCriticalSection(&csLimitLock);
	iVal=m_iMin;
	LeaveCriticalSection(&csLimitLock);

	return iVal;
}
void CAlarmLimit::setMinValue(int iVal)
{
	EnterCriticalSection(&csLimitLock);
	m_iMin=iVal;
	LeaveCriticalSection(&csLimitLock);
}



		
eAlarmLimitState CAlarmLimit::getLimitState()
{
	EnterCriticalSection(&csLimitLock);
	eAlarmLimitState eState=m_eLimitState;
	LeaveCriticalSection(&csLimitLock);
	return eState;
}
void CAlarmLimit::setLimitState(eAlarmLimitState eState)
{
	EnterCriticalSection(&csLimitLock);
	m_eLimitState=eState;
	LeaveCriticalSection(&csLimitLock);
}
