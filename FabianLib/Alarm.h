/**********************************************************************************************//**
 * \file	Alarm.h.
 *
 * Declares the alarm class
 **************************************************************************************************/

#pragma once
#include "globDefs.h"

class CAlarm   
{

public:
	CAlarm();
	CAlarm(eAlarm			enAlarm,
		eAlarmType		enType,
		eAlarmPrio		enPriority,
		eStateOfAlarm	enAlarmState,
		UINT iDelaySec);
	virtual ~CAlarm(void);

	eAlarmType		getAlarmType();
	void			setAlarmType(eAlarmType type);

	eAlarm			getAlarm();
	void			setALARM(eAlarm alarm);

	eAlarmPrio		getAlarmPriority();
	void			setAlarmPriority(eAlarmPrio prio);

	eStateOfAlarm	getAlarmState();
	void			setAlarmState(eStateOfAlarm state);

	UINT			getAlarmDelayTimeMSec();
	void			setAlarmDelayTimeSec(UINT timeSec);

	void			setTriggerTimestamp();
	DWORD			getTriggerTimestamp();
	void			resetTriggerTimestamp();

	bool isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay=0);//used for (dwLastTickCount+DELAY<getTickCount64())
private:
	CRITICAL_SECTION csAlarmLock;

	eAlarmType		m_enType;
	eAlarm			m_enAlarm;
	eAlarmPrio		m_enPriority;
	eStateOfAlarm	m_enAlarmState;
	UINT			m_iAlarmDelayTimeMiliSec;
	DWORD			m_dwAlarmTriggerTime;
	
	
};
