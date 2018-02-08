#pragma once
#include "interface.h"
#include "globDefs.h"
#include "serial.h"
#include "serialEx.h"


//typedef struct tagTERMMSG
//{
//	CStringW szMessage;
//}TERMMSG, *LPTERMMSG;

class CInterfaceTerminal : public CInterface, public CSerialEx
{
	//typedef struct tagMESSAGE
	//{
	//	//bool bWaitForAckn;
	//	CStringW szMessage;
	//}MESSAGE;

public:
	

	static CInterfaceTerminal* GetInstance(BYTE iTYPE);
	static void DestroyInstance();

	virtual bool Init()=0;
	virtual bool OpenCOM()=0;


	bool Deinit();
	void send(LPTERMINALMSG msg);
	void setWaveDataActive();
	bool isWaveDataActive();
	bool isMeasurementDataActive();


	virtual void sendMessurementData(){};
	

	//bool PerformMsg();
	//void OnEvent (EEvent eEvent, EError eError);
	
	
	
	

	/*void sendData_Cmd(BYTE command);
	void sendData_BYTE(BYTE command, BYTE val) ;
	void sendData_SHORT(BYTE command, SHORT val);*/

	void sendWaveData(SHORT  iValPressure,SHORT  iValFlow, SHORT  iCO2) ;
	void sendWaveData(SHORT  iValPressure,SHORT  iValFlow,SHORT  iValVolume,SHORT  iValCO2);

	//void sendBuffer(LPTERMINALMSG msg);
protected:
	CInterfaceTerminal(void);
	virtual ~CInterfaceTerminal(void);

	void StartTerminalThread(void);
	void StopTerminalThread( void );

	//void setParam_PINSP(WORD iVal);
	//void setParam_PEEP(WORD iVal);
	//void setParam_PPSV(WORD iVal);
	//void setParam_BPM(WORD iVal);
	//void setParam_HFAmpl(WORD iVal);
	//void setParam_HFAmplMax(WORD iVal);
	//void setParam_HFFreq(WORD iVal);
	//void setParam_FiO2(BYTE iVal);
	//void setParam_IFlow(WORD iVal);
	//void setParam_EFlow(WORD iVal);
	//void setParam_RiseTime(WORD iVal);
	//void setParam_ITime(WORD iVal);
	//void setParam_ETime(WORD iVal);
	//void setParam_HFPMean(WORD iVal);
	//void setParam_HFPMeanRec(WORD iVal);
	//void setParam_VLimit(WORD iVal);
	//void setParam_VGarant(WORD iVal);
	//void setParam_AbortCriterionPSV(BYTE iVal);
	//void setState_VLimit(BYTE iVal);
	//void setState_VGarant(BYTE iVal);
	//void setVent_RunState(BYTE iVal);
	//void setParam_IERatioHFO(BYTE iVal);
	//void set_MANBREATHrunning(BYTE iVal);
	//void set_PressureRiseCtrl(BYTE iVal);
	//void setParam_HFOFreqRec(WORD iVal);
	//void setParam_HFOFlow(WORD iVal);
	//void setLeakCompensation(BYTE iVal);
	//void setParam_TherapieFlow(WORD iVal);
	//void setParam_Trigger(BYTE iVal);
	//void setParam_Flowmin(WORD iVal);
	//void setParam_CPAP(WORD iVal);
	//void setParam_PManual(WORD iVal);
	//void setParam_Backup(BYTE iVal);
	//void setParam_ITimeRec(WORD iVal);
	////void setParam_ETimeRec(WORD iVal);
	//void setParam_O2_FLUSH(BYTE iVal);
	//void setParam_SPO2LOW(BYTE iVal);
	//void setParam_SPO2HIGH(BYTE iVal);
	//void setParam_FIO2LOW(BYTE iVal);
	//void setParam_FIO2HIGH(BYTE iVal);
	//void setState_PRICO(BYTE iVal);

	//void setVentMode(BYTE iVal);
	//void setVentRange(BYTE state);

	//void sendModeOption1();
	//void sendModeOption2();

	//void AddChecksum(short num, BYTE buf[]);

private:
	//Singelton
	static CInterfaceTerminal* theTerminalInterface;

protected:

	friend UINT CTerminalSendThread(LPVOID pc);
	CWinThread*	m_pcwtTerminalSendThread;
	DWORD SendTerminalData(void);
	bool m_bDoTerminalSendThread;
	HANDLE m_hThreadTerminalSend;

protected:	

	CTlsComPort m_comPort;

	static CList<LPTERMINALMSG, LPTERMINALMSG>  MSGSend;

	CRITICAL_SECTION	  csMSGSend;
	CRITICAL_SECTION	  csEnableSend;

	//UINT m_iCntBuf;
	
	/*BYTE	fwReadChkSum;
	BYTE	fwCalcChkSum;
	DWORD	fdwRecvBufLen;
	BYTE	faRecvBuf[256];
	BYTE	faDataBuf[256];
	WORD	fwRecvDataCnt;
	WORD	fwRecvDataBufCnt;
	BYTE	fwRecvDataSize;
	BYTE	fwRecvDataCode;*/

	/*BYTE	faSendDataBuf[256];
	BYTE	fwSendDataSize;*/

	//bool m_bStarted;
	bool m_bSendWaveData;

	/*ReceiverStatesTerminal	feRecvState;*/


	//BYTE m_byDellaceAmp;
	//BYTE m_byDellaceFreq;

	/*bool m_bMEASUREMENTS_BTB;
	bool m_bMEASUREMENTS_AVG;*/

	bool m_bSendContiniousMeasurements;
	

};
