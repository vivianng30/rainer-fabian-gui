/**********************************************************************************************//**
 * \file	AlarmList.cpp.
 *
 * Implements the alarm list class
 **************************************************************************************************/

#include "StdAfx.h"
#include "AlarmList.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CAlarmList class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CAlarmList::CAlarmList(void)
{
	InitializeCriticalSection(&csAlarmListLock);
	m_count=0;

	for(int i=0;i<NUMALARMS;i++)
	{
		m_bufAlarmArray[i]=0;
	}
}

/**********************************************************************************************//**
 * Finalizes an instance of the CAlarmList class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CAlarmList::~CAlarmList(void)
{
	DeleteCriticalSection(&csAlarmListLock);
}

/**********************************************************************************************//**
 * Array indexer operator
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	i	Zero-based index of the.
 *
 * \return	The indexed value.
 **************************************************************************************************/

CAlarmPtr CAlarmList::operator[](int i)
{
	if( i < 0 || i >= NUMALARMS)
	{
		AfxMessageBox( _T("EXCEPTION: CAlarmList NUMALARMLIMITS") );
		//theApp.ReportException(_T("EXCEPTION: CAlarmList NUMALARMLIMITS"));
		exit(1);
	}
	return m_bufAlarmArray[i];
}

/**********************************************************************************************//**
 * Gets the count
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The count.
 **************************************************************************************************/

int CAlarmList::getCount()
{
	EnterCriticalSection(&csAlarmListLock);
	int iCnt=m_count;
	LeaveCriticalSection(&csAlarmListLock);
	return iCnt;
}

/**********************************************************************************************//**
 * Gets an alarm
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	alarm	The alarm.
 *
 * \return	The alarm.
 **************************************************************************************************/

CAlarmPtr CAlarmList::getAlarm(eAlarm alarm)
{
	return (alarm >= 0 && alarm < m_count)? m_bufAlarmArray[alarm] : NULL;
}

/**********************************************************************************************//**
 * Searches for the first alarm
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	enAlarm	The en alarm.
 *
 * \return	The found alarm.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Appends an alarm
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	alarm	The alarm.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmList::appendAlarm( CAlarmPtr alarm)
{
	bool bResult=false;
	if(searchAlarm(alarm->getAlarm())==PSEUDO)//doesn't exist yet
	{
		EnterCriticalSection(&csAlarmListLock);
		if(m_count<NUMALARMS)
		{
			m_bufAlarmArray[alarm->getAlarm()]=alarm;
			m_count++;
			bResult=true;
		}
		LeaveCriticalSection(&csAlarmListLock);
	}
	return bResult;
}
