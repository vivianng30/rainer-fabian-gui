/**********************************************************************************************//**
 * \file	ThreadPRICO.h.
 *
 * Declares the thread prico class
 **************************************************************************************************/

#pragma once
#include "globDefs.h"
#include "Alarm.h"
//#include "AlarmLimit.h"
#include "AlarmList.h"
//#include "CircularBuffer.h"

#define PRICO_ALPHA 0.05

class CMVModel;
// CThreadPRICO

class CThreadPRICO
{

public:
	static CThreadPRICO* getInstance();	
	static void destroyInstance();

	

	/**
	* @brief Start/Stop thread Closed Loop (PRICO)
	**/
	void startPRICOThread(void);
	void stopPRICOThread( void );

	
	bool isPRICOalgorithmRunning();
	//void setMeasuredOxyValue(SHORT value);
	void setStartupOxyValue(SHORT value);

	SHORT getCalculatedOxyValue();

	//void setMeasuredSpO2Value(const SHORT& pBufData);
	void setMeasuredSpO2Value(SHORT iValue);
	void setHighSpO2PRICOlimit(SHORT value);
	void setLowSpO2PRICOlimit(SHORT value);

	//void checkFiO2range();

	void zeroDelay();
	void decreaseDelay();
	void resetDelay();
	BYTE getDelay();

	/*void setAlarmDisabled();
	void resetAlarmDisabled();
	bool isAlarmDisabled();*/
	//void setAllAlarmStates(tALARMSTATES* pa);
	//void setAlarmStates(CAlarmList* pAlarmlist);

	void resetCircBufSpO2();

protected:
	CThreadPRICO();
	~CThreadPRICO();
	CMVModel *getModel();

	void startPRICOalgorithm();
	void stopPRICOalgorithm();

	SHORT calcFiO2(/*int iAvgSpO2*/);
	//BYTE getSpO2buf(SHORT* piSpO2);
	

	//SHORT getMeasuredOxyValue();
	//SHORT getSettingOxyValue();

	void setCalculatedOxyValue(SHORT value);
	

	//BYTE getCountMeasuredSpO2Buf();
	//SHORT getMeasuredSpO2Value(BYTE iCnt);
	SHORT getHighSpO2PRICOlimit();
	SHORT getLowSpO2PRICOlimit();

	//void resetAlarmStates();

	/*eStateOfAlarm getSoA_SPO2_SENSORFAULTY();
	eStateOfAlarm getSoA_SPO2_CHECKSENSOR();
	eStateOfAlarm getSoA_SPO2_MODULE_NOTCONNECTED();*/
private:
	bool doThread();
	void startThread();
	void stopThread();

private:
	static CThreadPRICO* theThreadPRICO;//!< singleton of ThreadPRICO
	CMVModel *m_pModel;//!< the model

	CRITICAL_SECTION	csPRICOstate;
	//CRITICAL_SECTION	csPRICOactoxy;
	CRITICAL_SECTION	csPRICOcalcFiO2;
	CRITICAL_SECTION	csPRICOactspo2;
	CRITICAL_SECTION	csPRICOSpO2limit;
	CRITICAL_SECTION	csPRICOcntdown;
	CRITICAL_SECTION	csPRICOthread;

	static friend UINT PRICOThread(LPVOID pc);
	CWinThread*	m_pcwtPRICOThread;
	DWORD PRICOData(void);
	bool m_bDoPRICOThread;
	HANDLE m_hThreadPRICO;

	SHORT m_iAvgSpO2;
	SHORT m_iPrevAvgSpO2;
	SHORT m_iPrevSpO2val;
	SHORT m_iCurSpO2val;
	//static CircularBuffer<SHORT> m_piCircBufSpO2;
	//SHORT* m_piSpO2;
	//BYTE m_iCntSpo2val;

	//SHORT m_iMeasuredOxy;
	//SHORT m_iSettingOxy;

	//SHORT m_iMeasuredSpO2;
	SHORT m_iHighSpO2PRICOlimit;
	SHORT m_iLowSpO2PRICOlimit;

	SHORT m_iCalcFiO2;


	bool m_bPRICOrunning;
	bool m_bTimeOut;

	BYTE m_iCntDelay;

	BOOL m_bSPO2checkFlag;

	//bool m_bPRICOdisabledByAlarm;
	
};


