/**********************************************************************************************//**
 * \file	AlarmList.h.
 *
 * Declares the alarm list class
 **************************************************************************************************/

#pragma once
#include "globDefs.h"
#include "Alarm.h"

typedef CAlarm* CAlarmPtr;

#define PSEUDO -1	//Pseudoposition
class CAlarmList
{
private:
	CAlarmPtr m_bufAlarmArray[NUMALARMS];
	int m_count;
	CRITICAL_SECTION csAlarmListLock;

public:
	CAlarmList(void);
	virtual ~CAlarmList(void);

	CAlarmPtr operator[](int i);

	int getCount();
	CAlarmPtr getAlarm(eAlarm enAlarm);
	int searchAlarm(eAlarm enAlarm);

	bool appendAlarm( CAlarmPtr alarm);
};
