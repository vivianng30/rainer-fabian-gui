/**********************************************************************************************//**
 * \file	ThreadAlarm.h.
 *
 * Declares the thread alarm class
 **************************************************************************************************/

#pragma once


class CMVModel;
// CThreadAlarm

class CThreadAlarm
{

public:
	static CThreadAlarm* getInstance();	
	static void destroyInstance();

	

	/**
	* @brief Start/Stop thread for alarm handling.
	**/
	void startAlarmThread(void);
	void stopAlarmThread( void );

	void setCheckAlarmData(bool bState);

protected:
	CThreadAlarm();
	~CThreadAlarm();
	CMVModel *getModel();
private:
	static CThreadAlarm* theThreadAlarm;//!< singleton of CThreadAlarm
	CMVModel *m_pModel;//!< the model

	friend UINT AlarmThread(LPVOID pc);
	CWinThread*	m_pcwtAlarmThread;
	DWORD AlarmData(void);
	bool m_bDoAlarmThread;
	bool m_bCheckAlarmData;
	HANDLE m_hThreadAlarm;
};
