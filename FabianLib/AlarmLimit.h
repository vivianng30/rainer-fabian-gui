/**********************************************************************************************//**
 * \file	AlarmLimit.h.
 *
 * Declares the alarm limit class
 **************************************************************************************************/

#pragma once
#include "globDefs.h"

class CAlarmLimit   
{

public:
	CAlarmLimit();
	CAlarmLimit(eAlarmLimits enAlarmLimit,
		eAlarmLimitState eLimitState,
		int iCurValue,
		int iMax,
		int iMin);
	virtual ~CAlarmLimit(void);

	//CAlarmLimit& operator=( const CAlarmLimit& );

	eAlarmLimits		getAlarmLimit();
	void				setAlarmLimit(eAlarmLimits alarmLimit);

	int					getCurValue();
	void				setCurValue(int iVal);

	int					getMaxValue();
	void				setMaxValue(int iVal);

	int					getMinValue();
	void				setMinValue(int iVal);
		
	eAlarmLimitState	getLimitState();
	void				setLimitState(eAlarmLimitState eState);

	//void				setVentRange(UINT ventRange);

private:
	CRITICAL_SECTION csLimitLock;
	eAlarmLimits m_enAlarmLimit;
	eAlarmLimitState m_eLimitState;
	int m_iCurValue;
	int m_iMax;
	int m_iMin;
};
