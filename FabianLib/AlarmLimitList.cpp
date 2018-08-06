/**********************************************************************************************//**
 * \file	AlarmLimitList.cpp.
 *
 * Implements the alarm limit list class
 **************************************************************************************************/

#include "StdAfx.h"
#include "AlarmLimitList.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CAlarmLimitList class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CAlarmLimitList::CAlarmLimitList(void)
{
	InitializeCriticalSection(&csAlimitListLock);
	m_count=0;

	for(int i=0;i<NUMALARMLIMITS;i++)
	{
		m_bufAlarmLimitArray[i]=0;
	}
}

/**********************************************************************************************//**
 * Finalizes an instance of the CAlarmLimitList class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CAlarmLimitList::~CAlarmLimitList(void)
{
	DeleteCriticalSection(&csAlimitListLock);
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

CAlarmLimitPtr CAlarmLimitList::operator[](int i)
{
	if( i < 0 || i >= NUMALARMLIMITS)
	{
		AfxMessageBox( _T("EXCEPTION: CAlarmLimitList NUMALARMLIMITS") );
		//theApp.ReportException(_T("EXCEPTION: CAlarmLimitList NUMALARMLIMITS"));
		exit(1);
	}
	return m_bufAlarmLimitArray[i];
}

/**********************************************************************************************//**
 * Gets the count
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The count.
 **************************************************************************************************/

int CAlarmLimitList::getCount()
{
	EnterCriticalSection(&csAlimitListLock);
	int iCnt=m_count;
	LeaveCriticalSection(&csAlimitListLock);
	return iCnt;
}

/**********************************************************************************************//**
 * Gets alarm limit
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	enAlarmLimit	The en alarm limit.
 *
 * \return	The alarm limit.
 **************************************************************************************************/

CAlarmLimitPtr CAlarmLimitList::getAlarmLimit(eAlarmLimits enAlarmLimit)
{
	return (enAlarmLimit >= 0 && enAlarmLimit < m_count)? m_bufAlarmLimitArray[enAlarmLimit] : NULL;
}

/**********************************************************************************************//**
 * Searches for the first alarm limit
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	enAlarmLimit	The en alarm limit.
 *
 * \return	The found alarm limit.
 **************************************************************************************************/

int CAlarmLimitList::searchAlarmLimit(eAlarmLimits enAlarmLimit)
{
	EnterCriticalSection(&csAlimitListLock);
	for(int i=0; i < m_count;i++)//search
	{
		if(m_bufAlarmLimitArray[i]->getAlarmLimit()==enAlarmLimit)
		{
			LeaveCriticalSection(&csAlimitListLock);
			return i;//found
		}
	}
	LeaveCriticalSection(&csAlimitListLock);
	return PSEUDO;//not found
}

/**********************************************************************************************//**
 * Appends an alarm limit
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	alarmLimit	The alarm limit.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmLimitList::appendAlarmLimit(CAlarmLimitPtr alarmLimit)
{
	bool bResult=false;
	if(searchAlarmLimit(alarmLimit->getAlarmLimit())==PSEUDO)//doesn't exist yet
	{
		EnterCriticalSection(&csAlimitListLock);
		if(m_count<NUMALARMLIMITS)
		{
			m_bufAlarmLimitArray[alarmLimit->getAlarmLimit()]=alarmLimit;
			m_count++;
			bResult=true;
		}
		LeaveCriticalSection(&csAlimitListLock);
	}
	return bResult;
}

/**********************************************************************************************//**
 * Gets current value
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	enAlarmLimit	The en alarm limit.
 *
 * \return	The current value.
 **************************************************************************************************/

int CAlarmLimitList::getCurValue(eAlarmLimits enAlarmLimit)
{
	int iResult=PSEUDO;
	if(searchAlarmLimit(enAlarmLimit)!=PSEUDO)//found
	{
		EnterCriticalSection(&csAlimitListLock);
		iResult=m_bufAlarmLimitArray[enAlarmLimit]->getCurValue();
		LeaveCriticalSection(&csAlimitListLock);
	}
	else
	{
		CString szError=_T("");
		szError.Format(_T("EXCEPTION: CAlarmLimitList getCurValue: #%d"),(int)enAlarmLimit);
		AfxMessageBox(szError);
		//theApp.ReportException(szError);
	}
	return iResult;
}

/**********************************************************************************************//**
 * Sets current value
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	enAlarmLimit	The en alarm limit.
 * \param	iVal			Zero-based index of the value.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmLimitList::setCurValue(eAlarmLimits enAlarmLimit,int iVal)
{
	bool bResult=false;
	if(searchAlarmLimit(enAlarmLimit)!=PSEUDO)//found
	{
		EnterCriticalSection(&csAlimitListLock);
		m_bufAlarmLimitArray[enAlarmLimit]->setCurValue(iVal);
		LeaveCriticalSection(&csAlimitListLock);
		bResult=true;
	}
	else
	{
		CString szError=_T("");
		szError.Format(_T("EXCEPTION: CAlarmLimitList setCurValue: #%d"),(int)enAlarmLimit);
		AfxMessageBox(szError);
		//theApp.ReportException(szError);
	}
	return bResult;
}

/**********************************************************************************************//**
 * Gets maximum value
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	enAlarmLimit	The en alarm limit.
 *
 * \return	The maximum value.
 **************************************************************************************************/

int CAlarmLimitList::getMaxValue(eAlarmLimits enAlarmLimit)
{
	int iResult=PSEUDO;
	if(searchAlarmLimit(enAlarmLimit)!=PSEUDO)//found
	{
		EnterCriticalSection(&csAlimitListLock);
		iResult=m_bufAlarmLimitArray[enAlarmLimit]->getMaxValue();
		LeaveCriticalSection(&csAlimitListLock);
	}
	else
	{
		CString szError=_T("");
		szError.Format(_T("EXCEPTION: CAlarmLimitList getMaxValue: #%d"),(int)enAlarmLimit);
		AfxMessageBox(szError);
		//theApp.ReportException(szError);
	}
	return iResult;
}

/**********************************************************************************************//**
 * Sets maximum value
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	enAlarmLimit	The en alarm limit.
 * \param	iVal			Zero-based index of the value.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmLimitList::setMaxValue(eAlarmLimits enAlarmLimit,int iVal)
{
	bool bResult=false;
	if(searchAlarmLimit(enAlarmLimit)!=PSEUDO)//found
	{
		EnterCriticalSection(&csAlimitListLock);
		m_bufAlarmLimitArray[enAlarmLimit]->setMaxValue(iVal);
		LeaveCriticalSection(&csAlimitListLock);
		bResult=true;
	}
	else
	{
		CString szError=_T("");
		szError.Format(_T("EXCEPTION: CAlarmLimitList setMaxValue: #%d"),(int)enAlarmLimit);
		AfxMessageBox(szError);
		//theApp.ReportException(szError);
	}
	return bResult;
}

/**********************************************************************************************//**
 * Gets minimum value
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	enAlarmLimit	The en alarm limit.
 *
 * \return	The minimum value.
 **************************************************************************************************/

int CAlarmLimitList::getMinValue(eAlarmLimits enAlarmLimit)
{
	int iResult=PSEUDO;
	if(searchAlarmLimit(enAlarmLimit)!=PSEUDO)//found
	{
		EnterCriticalSection(&csAlimitListLock);
		iResult=m_bufAlarmLimitArray[enAlarmLimit]->getMinValue();
		LeaveCriticalSection(&csAlimitListLock);
	}
	else
	{
		CString szError=_T("");
		szError.Format(_T("EXCEPTION: CAlarmLimitList getMinValue: #%d"),(int)enAlarmLimit);
		AfxMessageBox(szError);
		//theApp.ReportException(szError);
	}
	return iResult;
}

/**********************************************************************************************//**
 * Sets minimum value
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	enAlarmLimit	The en alarm limit.
 * \param	iVal			Zero-based index of the value.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmLimitList::setMinValue(eAlarmLimits enAlarmLimit,int iVal)
{
	bool bResult=false;
	if(searchAlarmLimit(enAlarmLimit)!=PSEUDO)//found
	{
		EnterCriticalSection(&csAlimitListLock);
		m_bufAlarmLimitArray[enAlarmLimit]->setMinValue(iVal);
		LeaveCriticalSection(&csAlimitListLock);
		bResult=true;
	}
	else
	{
		CString szError=_T("");
		szError.Format(_T("EXCEPTION: CAlarmLimitList setMinValue: #%d"),(int)enAlarmLimit);
		AfxMessageBox(szError);
		//theApp.ReportException(szError);
	}
	return bResult;
}

/**********************************************************************************************//**
 * Gets limit state
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	enAlarmLimit	The en alarm limit.
 *
 * \return	The limit state.
 **************************************************************************************************/

eAlarmLimitState CAlarmLimitList::getLimitState(eAlarmLimits enAlarmLimit)
{
	eAlarmLimitState eResult=AL_OFF;
	if(searchAlarmLimit(enAlarmLimit)!=PSEUDO)//found
	{
		EnterCriticalSection(&csAlimitListLock);
		eResult=m_bufAlarmLimitArray[enAlarmLimit]->getLimitState();
		LeaveCriticalSection(&csAlimitListLock);
	}
	else
	{
		CString szError=_T("");
		szError.Format(_T("EXCEPTION: CAlarmLimitList getLimitState: #%d"),(int)enAlarmLimit);
		AfxMessageBox(szError);
		//theApp.ReportException(szError);
	}
	return eResult;
}

/**********************************************************************************************//**
 * Sets limit state
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	enAlarmLimit	The en alarm limit.
 * \param	eState			The state.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmLimitList::setLimitState(eAlarmLimits enAlarmLimit,eAlarmLimitState eState)
{
	bool bResult=false;
	if(searchAlarmLimit(enAlarmLimit)!=PSEUDO)//found
	{
		EnterCriticalSection(&csAlimitListLock);
		m_bufAlarmLimitArray[enAlarmLimit]->setLimitState(eState);
		LeaveCriticalSection(&csAlimitListLock);
		bResult=true;
	}
	else
	{
		CString szError=_T("");
		szError.Format(_T("EXCEPTION: CAlarmLimitList setLimitState: #%d"),(int)enAlarmLimit);
		AfxMessageBox(szError);
		//theApp.ReportException(szError);
	}
	return bResult;
}
