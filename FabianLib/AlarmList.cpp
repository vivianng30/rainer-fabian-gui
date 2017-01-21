#include "StdAfx.h"
#include "AlarmList.h"
#include "FabianHFO.h"

CAlarmList::CAlarmList(void)
{
	InitializeCriticalSection(&csAlarmListLock);
	m_count=0;

	for(int i=0;i<NUMALARMS;i++)
	{
		m_bufAlarmArray[i]=0;
	}
}

CAlarmList::~CAlarmList(void)
{
	//EnterCriticalSection(&csAlarmListLock);
	//CAlarmPtr pTempAlarmClass = NULL;
	//for(int i=0; i < m_count;i++)//search
	//{
	//	pTempAlarmClass = m_bufAlarmArray[i];
	//	//m_pAlarmArray.RemoveAt( iIndex-1 );
	//	if(pTempAlarmClass)
	//	{
	//		delete pTempAlarmClass;
	//	}
	//	else
	//	{
	//	}
	//}
	//pTempAlarmClass = NULL;
	//LeaveCriticalSection(&csAlarmListLock);

	DeleteCriticalSection(&csAlarmListLock);
}
CAlarmPtr CAlarmList::operator[](int i)
{
	if( i < 0 || i >= NUMALARMS)
	{
		theApp.ReportException(_T("EXCEPTION: CAlarmList NUMALARMLIMITS"));
		exit(1);
	}
	return m_bufAlarmArray[i];
}
int CAlarmList::getCount()
{
	EnterCriticalSection(&csAlarmListLock);
	int iCnt=m_count;
	LeaveCriticalSection(&csAlarmListLock);
	return iCnt;
}
CAlarmPtr CAlarmList::getAlarm(eAlarm alarm)
{
	return (alarm >= 0 && alarm < m_count)? m_bufAlarmArray[alarm] : NULL;
}

int CAlarmList::searchAlarm(eAlarm enAlarm)
{
	EnterCriticalSection(&csAlarmListLock);
	for(int i=0; i < m_count;i++)//search
	{
		if(m_bufAlarmArray[i]->getAlarm()==enAlarm)
		{
			LeaveCriticalSection(&csAlarmListLock);
			return i;//found
		}
	}
	LeaveCriticalSection(&csAlarmListLock);
	return PSEUDO;//not found
}

bool CAlarmList::appendAlarm( CAlarmPtr alarm)
{

	bool bResult=false;
	if(searchAlarm(alarm->getAlarm())==PSEUDO)//doesn't exist yet
	{
		EnterCriticalSection(&csAlarmListLock);
		if(m_count<NUMALARMS)
		{
			m_bufAlarmArray[alarm->getAlarm()]=alarm;
			/*m_bufAlarmArray[enAlarm]->setALARM(enAlarm);
			m_bufAlarmArray[enAlarm]->setAlarmType(enType);
			m_bufAlarmArray[enAlarm]->setAlarmPriority(enPriority);
			m_bufAlarmArray[enAlarm]->setAlarmState(enAlarmState);
			m_bufAlarmArray[enAlarm]->setAlarmDelayTimeSec(iDelaySec);*/
			m_count++;
			bResult=true;
		}
		LeaveCriticalSection(&csAlarmListLock);
	}
	return bResult;


	/*return appendAlarm(	alarm->getAlarm(),
		alarm->getAlarmType(),
		alarm->getAlarmPriority(),
		alarm->getAlarmState(),
		(alarm->getAlarmDelayTimeMSec()/1000));*/
}
//
//bool CAlarmList::appendAlarm(	const eAlarm&			enAlarm,
//								const eAlarmType&		enType,
//								const eAlarmPrio&		enPriority,
//								eStateOfAlarm	enAlarmState,
//								UINT				iDelaySec)
//{
//	bool bResult=false;
//	if(searchAlarm(enAlarm)==PSEUDO)//doesn't exist yet
//	{
//		EnterCriticalSection(&csAlarmListLock);
//		if(m_count<NUMALARMS)
//		{
//			m_bufAlarmArray[enAlarm]->setALARM(enAlarm);
//			m_bufAlarmArray[enAlarm]->setAlarmType(enType);
//			m_bufAlarmArray[enAlarm]->setAlarmPriority(enPriority);
//			m_bufAlarmArray[enAlarm]->setAlarmState(enAlarmState);
//			m_bufAlarmArray[enAlarm]->setAlarmDelayTimeSec(iDelaySec);
//			m_count++;
//			bResult=true;
//		}
//		LeaveCriticalSection(&csAlarmListLock);
//	}
//	return bResult;
//}
