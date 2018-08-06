//=============================================================================
/** 
* \class CInterfaceSerial
* 		 
* \brief Interface for serial interface to controller PIC.
* 		 
* \author Rainer Kuehner
* \date 2017-10-23 Creation of file
*
**/
//=============================================================================
#pragma once
#include "interface.h"
#include "globDefs.h"

class CInterfaceSerial : public CInterface
{
	//typedef struct tagMESSAGE
	//{
	//	//bool bWaitForAckn;
	//	CStringW szMessage;
	//}MESSAGE;

public:
	typedef enum
	{
		EEventUnknown  	   = -1,			// Unknown event
		EEventNone  	   = 0,				// Event trigged without cause
		EEventBreak 	   = EV_BREAK,		// A break was detected on input
		EEventCTS   	   = EV_CTS,		// The CTS signal changed state
		EEventDSR   	   = EV_DSR,		// The DSR signal changed state
		EEventError 	   = EV_ERR,		// A line-status error occurred
		EEventRing  	   = EV_RING,		// A ring indicator was detected
		EEventRLSD  	   = EV_RLSD,		// The RLSD signal changed state
		EEventRecv  	   = EV_RXCHAR,		// Data is received on input
		EEventRcvEv 	   = EV_RXFLAG,		// Event character was received on input
		EEventSend		   = EV_TXEMPTY,	// Last character on output was sent
		EEventPrinterError = EV_PERR,		// Printer error occured
		EEventRx80Full	   = EV_RX80FULL,	// Receive buffer is 80 percent full
		EEventProviderEvt1 = EV_EVENT1,		// Provider specific event 1
		EEventProviderEvt2 = EV_EVENT2,		// Provider specific event 2
	} 
	EEvent;

	static CInterfaceSerial* GetInstance();
	static void DestroyInstance();

	bool Init();
	bool Deinit();

	//CString GetSerialVersion();

	BOOL FlushBuffer();
	bool IsAvailable();

	void Send_HARDWARE_CONFIG(WORD wHardwareConfig);

	void Send_MODE_OPTION1(WORD wMode);
	void Send_MODE_OPTION2(WORD wMode);
	void Send_VENT_MODE(eVentMode mode);
	void Send_PARAVAL_PINSP(int val);//newVG
	void Send_PARAVAL_PMAXVG(int val); //newVG
	void Send_PARAVAL_P_PSV(int val);
	void Send_PARAVAL_PEEP(int val);
	void Send_PARAVAL_OXY_RATIO(int val);
	//void Send_PARAVAL_OXY_CORR(int val);
	void Send_PARAVAL_INSP_FLOW(int val);
	void Send_PARAVAL_RISETIME(int val);
	void Send_PARAVAL_EXH_FLOW(int val);
	void Send_PARAVAL_INSP_TIME(int val);
	void Send_PARAVAL_EXH_TIME(int val);
	void Send_PARAVAL_HF_AMPL(int val);
	void Send_PARAVAL_HF_FREQ(int val);
	void Send_PARAVAL_HF_PMITT(int val);

	void Send_PARAVAL_HF_PMEANREC(int val);
	void Send_PARAVAL_HF_FREQ_REC(int val);
	void Send_PARAVAL_HF_ITIME_REC(int val);

	void Send_PARAVAL_APNOE_TIME(int val);

	//void Send_PARAVAL_HF_IERATIO(int val);
	void Send_START_MAN_BREATH();
	void Send_STOP_MAN_BREATH();
	void Send_EXP_VALVE(int val);
	void Send_OPEN_VALVE(int val);
	/*void Send_SET_FG_BIT(int val); //newVG
	void Send_CLEAR_FG_BIT(int val);*/ //newVG
	//void Send_VentilatorRunningMode(/*eRunState state*/);
	void Send_CAL_PRESS_PROX(int val);
	void Send_OXYGEN_CORR(int val);
	void Send_VOLUME_CORR(BYTE val);
	void Send_AUTOFLOW_CORR(BYTE val);
	
	void Send_FOTconv_AMP_FREQ(int val);

	//void CheckM_StatusByte();
	//void CheckM_StatusByteTime();
	//void CheckM_StatusByteATZ();
	BOOL GetM_StatusByte();
	int GetM_CHECKSUM(bool bIgnorCOMstate=false);
	int GetM_CAL_PRESS_SCALE();
	int GetM_CAL_PRESS_OFFSET();
	CStringW GetM_VERSION();

	CStringW GetHFO_VERSION();
	int GetHFO_CHECKSUM();

	CStringW GetBLENDER_VERSION();
	BOOL GetBLENDER_StatusByte();

	int GetM_INSP_FLOW();
	int GetM_EXP_FLOW();
	int GetM_DEMAND_FLOW();

	//newVG
	/*int resetCOMerror(eCOMerror error);
	void setCOMerror(eCOMerror error);*/

	BYTE sendSerialCommand(CStringW data,bool bIgnorCOMstate=false);

	bool IsSerialDisconnection();
	bool IsSerial_TUBE_OCCLUSION();

	//TEST LOCK SERIAL
	//void lockTest();

	void resetErrorCountCheckThread();

private:
	//singleton
	static CInterfaceSerial* theSerialInterface;



protected:
	CInterfaceSerial(void);
	virtual ~CInterfaceSerial(void);




	void StartSerialThread(void);
	void StopSerialThread( void );

	bool ParseControllerCommand(CTlsBlob bl);
	bool CheckStatusBits(CTlsBlob bl);
	int GetCOMValue(CTlsBlob bl);

	//void SetErrorCode(int mode, int cmd);//newVG

	bool CheckBlenderStatusBits(CTlsBlob bl);

	
protected:

	friend UINT CSerialReceiveThread(LPVOID pc);
	CWinThread*	m_pcwtSerialReceiveThread;
	DWORD ReceiveSerialData(void);
	bool m_bDoReceiveThread;
	HANDLE m_hThreadSerialReceive;

	bool doReceiveThread();
	void startReceiveThread();
	void stopReceiveThread();

	friend UINT CSerialSendThread(LPVOID pc);
	CWinThread*	m_pcwtSerialSendThread;
	DWORD SendSerialData(void);
	bool m_bDoSendThread;
	HANDLE m_hThreadSerialSend;

	bool doSendThread();
	void startSendThread();
	void stopSendThread();

	friend UINT CSerialCheckThread(LPVOID pc);
	CWinThread*	m_pcwtSerialCheckThread;
	DWORD CheckSerialData(void);
	bool m_bDoCheckThread;
	CEvent eventCheckPara;
	HANDLE m_hThreadSerialCheck;

	bool doCheckThread();
	void startCheckThread();
	void stopCheckThread();

	//EEvent	m_eEvent;
protected:	
	CString m_szCtrlVersion;

	CTlsComPort m_comPort;

	CList<CStringW, CStringW&>  MSGSend;
	//CStringList MSGSend;
	//CList<MESSAGE*, MESSAGE *> MSGSend;
	CRITICAL_SECTION	  csMSGSend;

	CRITICAL_SECTION	  csReceiveThread;
	CRITICAL_SECTION	  csSendThread;
	CRITICAL_SECTION	  csCheckThread;

	//TEST LOCK SERIAL
	//CRITICAL_SECTION	  csTestLock;

	bool m_bStarted;

	DWORD m_dwLastCheckMstate;
	//DWORD m_dwLastCheckMchecksum;

	int m_iCALPRESSSCALE;
	int m_iCALPRESSOFFSET;
	bool m_bCALPRESSOFFSET;
	bool m_bCALPRESSSCALE;

	int m_iM_INSP_FLOW;
	int m_iM_EXP_FLOW;

	int m_iM_DEMAND_FLOW;

	bool m_bSerialDisconnection;
	/*bool m_bSerialINSPIRATIONTUBE;
	bool m_bSerialEXSPIRATIONTUBE;*/
	bool m_bSerialTUBE_OCCLUSION;

	/*int m_iError_evCOM_VERSION;
	int m_iError_evCOM_CAL_PRESS_OFFSET;
	int m_iError_evCOM_CAL_PRESS_SCALE;
	int m_iError_evCOM_CHECKSUM;
	int m_iError_evCOM_STATUS;
	int m_iError_evCOM_M_INSP_FLOW;
	int m_iError_evCOM_M_EXP_FLOW;
	int m_iError_evCOM_M_DEMAND_FLOW;*/


	//int m_iTestCount;

	BYTE m_iErrorCountCheckThread_Mode;
	BYTE m_iErrorCountCheckThread_VentRunState;
	BYTE m_iErrorCountCheckThread_ITIME;
	BYTE m_iErrorCountCheckThread_ETIME;
	BYTE m_iErrorCountCheckThread_IFLOW;
	BYTE m_iErrorCountCheckThread_RISETIME;
	BYTE m_iErrorCountCheckThread_EFLOW;
	BYTE m_iErrorCountCheckThread_PIP;
	BYTE m_iErrorCountCheckThread_PMAXVG;
	BYTE m_iErrorCountCheckThread_PEEP;
	BYTE m_iErrorCountCheckThread_PPSV;
	BYTE m_iErrorCountCheckThread_OXY;

	BYTE m_iErrorCountCheckThread_HFFREQ;
	BYTE m_iErrorCountCheckThread_PMEAN;
	BYTE m_iErrorCountCheckThread_HFAMPLMAX;

	int m_iErrorCountSendThread;

	//bool m_bResetTestFlag;
};
