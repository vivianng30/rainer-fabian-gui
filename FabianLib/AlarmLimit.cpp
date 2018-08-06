/**********************************************************************************************//**
 * \file	AlarmLimit.cpp.
 *
 * Implements the alarm limit class
 **************************************************************************************************/

#include "StdAfx.h"
#include "AlarmLimit.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CAlarmLimit class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	enAlarmLimit	The en alarm limit.
 * \param	eLimitState 	State of the limit.
 * \param	iCurValue   	The current value.
 * \param	iMax			Zero-based index of the maximum.
 * \param	iMin			Zero-based index of the minimum.
 **************************************************************************************************/

CAlarmLimit::CAlarmLimit(eAlarmLimits enAlarmLimit,
						 eAlarmLimitState eLimitState,
						 int iCurValue,
						 int iMax,
						 int iMin):
		m_enAlarmLimit(enAlarmLimit),m_eLimitState(eLimitState),m_iCurValue(iCurValue),m_iMax(iMax),m_iMin(iMin)
{
	InitializeCriticalSection(&csLimitLock);
}

/**********************************************************************************************//**
 * Initializes a new instance of the CAlarmLimit class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CAlarmLimit::CAlarmLimit():
m_enAlarmLimit(LI_MVMAX),m_eLimitState(AL_OFF),m_iCurValue(0),m_iMax(0),m_iMin(0)
{
	InitializeCriticalSection(&csLimitLock);
}

/**********************************************************************************************//**
 * Finalizes an instance of the CAlarmLimit class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CAlarmLimit::~CAlarmLimit(void)
{
	DeleteCriticalSection(&csLimitLock);
}

/**********************************************************************************************//**
 * Gets alarm limit
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The alarm limit.
 **************************************************************************************************/

eAlarmLimits CAlarmLimit::getAlarmLimit()
{
	EnterCriticalSection(&csLimitLock);
	eAlarmLimits alarmLimit=m_enAlarmLimit;
	LeaveCriticalSection(&csLimitLock);
	return alarmLimit;
}

/**********************************************************************************************//**
 * Sets alarm limit
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	alarmLimit	The alarm limit.
 **************************************************************************************************/

void CAlarmLimit::setAlarmLimit(eAlarmLimits alarmLimit)
{
	EnterCriticalSection(&csLimitLock);
	m_enAlarmLimit=alarmLimit;
	LeaveCriticalSection(&csLimitLock);
}

/**********************************************************************************************//**
 * Gets current value
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The current value.
 **************************************************************************************************/

int CAlarmLimit::getCurValue()
{	
	EnterCriticalSection(&csLimitLock);
	int iVal=m_iCurValue;
	LeaveCriticalSection(&csLimitLock);
	return iVal;
}

/**********************************************************************************************//**
 * Sets current value
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CAlarmLimit::setCurValue(int iVal)
{
	EnterCriticalSection(&csLimitLock);
	m_iCurValue=iVal;
	LeaveCriticalSection(&csLimitLock);
}

/**********************************************************************************************//**
 * Gets maximum value
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The maximum value.
 **************************************************************************************************/

int CAlarmLimit::getMaxValue()
{
	int iVal=0;

	EnterCriticalSection(&csLimitLock);
	iVal=m_iMax;
	LeaveCriticalSection(&csLimitLock);

	return iVal;
}

/**********************************************************************************************//**
 * Sets maximum value
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CAlarmLimit::setMaxValue(int iVal)
{
	EnterCriticalSection(&csLimitLock);
	m_iMax=iVal;
	LeaveCriticalSection(&csLimitLock);
}

/**********************************************************************************************//**
 * Gets minimum value
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The minimum value.
 **************************************************************************************************/

int CAlarmLimit::getMinValue()
{
	int iVal=0;

	EnterCriticalSection(&csLimitLock);
	iVal=m_iMin;
	LeaveCriticalSection(&csLimitLock);

	return iVal;
}

/**********************************************************************************************//**
 * Sets minimum value
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	iVal	Zero-based index of the value.
 **************************************************************************************************/

void CAlarmLimit::setMinValue(int iVal)
{
	EnterCriticalSection(&csLimitLock);
	m_iMin=iVal;
	LeaveCriticalSection(&csLimitLock);
}

/**********************************************************************************************//**
 * Gets limit state
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The limit state.
 **************************************************************************************************/

eAlarmLimitState CAlarmLimit::getLimitState()
{
	EnterCriticalSection(&csLimitLock);
	eAlarmLimitState eState=m_eLimitState;
	LeaveCriticalSection(&csLimitLock);
	return eState;
}

/**********************************************************************************************//**
 * Sets limit state
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	eState	The state.
 **************************************************************************************************/

void CAlarmLimit::setLimitState(eAlarmLimitState eState)
{
	EnterCriticalSection(&csLimitLock);
	m_eLimitState=eState;
	LeaveCriticalSection(&csLimitLock);
}
