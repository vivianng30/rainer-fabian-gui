#include "StdAfx.h"
#include "AlarmLimitList.h"
#include "FabianHFO.h"

CAlarmLimitList::CAlarmLimitList(void)
{
	InitializeCriticalSection(&csAlimitListLock);
	m_count=0;

	for(int i=0;i<NUMALARMLIMITS;i++)
	{
		m_bufAlarmLimitArray[i]=0;
	}
}

CAlarmLimitList::~CAlarmLimitList(void)
{
	//EnterCriticalSection(&csAlimitListLock);
	//CAlarmLimit* pTempClass = NULL;
	//for(int i=0; i < m_count;i++)//search
	//{
	//	pTempClass = &m_bufAlarmLimitArray[i];
	//	//m_pAlarmArray.RemoveAt( iIndex-1 );
	//	if(pTempClass)
	//	{
	//		delete pTempClass;
	//	}
	//	else
	//	{
	//	}
	//}
	//pTempClass = NULL;
	//LeaveCriticalSection(&csAlimitListLock);

	DeleteCriticalSection(&csAlimitListLock);
}
CAlarmLimitPtr CAlarmLimitList::operator[](int i)
{
	if( i < 0 || i >= NUMALARMLIMITS)
	{
		theApp.ReportException(_T("EXCEPTION: CAlarmLimitList NUMALARMLIMITS"));
		exit(1);
	}
	return m_bufAlarmLimitArray[i];
}


int CAlarmLimitList::getCount()
{
	EnterCriticalSection(&csAlimitListLock);
	int iCnt=m_count;
	LeaveCriticalSection(&csAlimitListLock);
	return iCnt;
}
CAlarmLimitPtr CAlarmLimitList::getAlarmLimit(eAlarmLimits enAlarmLimit)
{
	return (enAlarmLimit >= 0 && enAlarmLimit < m_count)? m_bufAlarmLimitArray[enAlarmLimit] : NULL;
}
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

	/*return appendAlarmLimit(	alarmLimit->getAlarmLimit(),
		alarmLimit->getLimitState(),
		alarmLimit->getCurValue(),
		alarmLimit->getMaxValue(),
		alarmLimit->getMinValue());*/
}

//bool CAlarmLimitList::appendAlarmLimit(	const eAlarmLimits&			enAlarmLimit,
//					  eAlarmLimitState eLimitState,
//					  int iCurValue,
//					  int iMax,
//					  int iMin)
//{
//	bool bResult=false;
//	if(searchAlarmLimit(enAlarmLimit)==PSEUDO)//doesn't exist yet
//	{
//		EnterCriticalSection(&csAlimitListLock);
//		if(m_count<NUMALARMS)
//		{
//			m_bufAlarmLimitArray[enAlarmLimit].setAlarmLimit(enAlarmLimit);
//			m_bufAlarmLimitArray[enAlarmLimit].setLimitState(eLimitState);
//			m_bufAlarmLimitArray[enAlarmLimit].setCurValue(iCurValue);
//			m_bufAlarmLimitArray[enAlarmLimit].setMaxValue(iMax);
//			m_bufAlarmLimitArray[enAlarmLimit].setMinValue(iMin);
//			m_count++;
//			bResult=true;
//		}
//		LeaveCriticalSection(&csAlimitListLock);
//	}
//	else
//	{
//		CString szError=_T("");
//		szError.Format(_T("EXCEPTION: CAlarmLimitList append: #%d"),(int)enAlarmLimit);
//		theApp.ReportException(szError);
//	}
//	return bResult;
//}
//*****************************************************************
// Method:    getCurValue
// FullName:  CAlarmLimitList::getCurValue
// Access:    public 
// Returns:   int
// Qualifier: returns -1 if limit doesn't exist in queue,
//			  else the current value
// Parameter: eAlarmLimits enAlarmLimit
//*****************************************************************
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
		theApp.ReportException(szError);
	}
	return iResult;
}
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
		theApp.ReportException(szError);
	}
	return bResult;
}

//*****************************************************************
// Method:    getMaxValue
// FullName:  CAlarmLimitList::getMaxValue
// Access:    public 
// Returns:   int
// Qualifier: returns -1 if limit doesn't exist in queue,
//			  else the maximum value
// Parameter: eAlarmLimits enAlarmLimit
//*****************************************************************
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
		theApp.ReportException(szError);
	}
	return iResult;
}
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
		theApp.ReportException(szError);
	}
	return bResult;
}

//*****************************************************************
// Method:    getMinValue
// FullName:  CAlarmLimitList::getMinValue
// Access:    public 
// Returns:   int
// Qualifier: returns -1 if limit doesn't exist in queue,
//			  else the minimum value
// Parameter: eAlarmLimits enAlarmLimit
//*****************************************************************
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
		theApp.ReportException(szError);
	}
	return iResult;
}
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
		theApp.ReportException(szError);
	}
	return bResult;
}

//*****************************************************************
// Method:    getLimitState
// FullName:  CAlarmLimitList::getLimitState
// Access:    public 
// Returns:   eAlarmLimitState
// Qualifier: returns AL_OFF if limit doesn't exist in queue,
//			  else the current state
// Parameter: eAlarmLimits enAlarmLimit
//*****************************************************************
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
		theApp.ReportException(szError);
	}
	return eResult;
}
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
		theApp.ReportException(szError);
	}
	return bResult;
}
