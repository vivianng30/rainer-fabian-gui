#pragma once
#include "globDefs.h"
#include "AlarmLimit.h"
#include <stdexcept>
#include <iostream>

typedef CAlarmLimit* CAlarmLimitPtr;

#define PSEUDO -1	//Pseudoposition
class CAlarmLimitList
{
private:
	CAlarmLimitPtr m_bufAlarmLimitArray[NUMALARMLIMITS];

	int m_count;
	CRITICAL_SECTION csAlimitListLock;

public:
	CAlarmLimitList(void);
	virtual ~CAlarmLimitList(void);

	CAlarmLimitPtr operator[](int i);

	int getCount();
	CAlarmLimitPtr getAlarmLimit(eAlarmLimits enAlarmLimit);
	int searchAlarmLimit(eAlarmLimits enAlarmLimit);

	bool appendAlarmLimit(CAlarmLimitPtr alarmLimit);

	int					getCurValue(eAlarmLimits enAlarmLimit);
	bool				setCurValue(eAlarmLimits enAlarmLimit,int iVal);

	int					getMaxValue(eAlarmLimits enAlarmLimit);
	bool				setMaxValue(eAlarmLimits enAlarmLimit,int iVal);

	int					getMinValue(eAlarmLimits enAlarmLimit);
	bool				setMinValue(eAlarmLimits enAlarmLimit,int iVal);

	eAlarmLimitState	getLimitState(eAlarmLimits enAlarmLimit);
	bool				setLimitState(eAlarmLimits enAlarmLimit,eAlarmLimitState eState);

protected:
	/*bool appendAlarmLimit(	const eAlarmLimits&			enAlarmLimit,
				eAlarmLimitState eLimitState,
				int iCurValue,
				int iMax,
				int iMin);*/
};
