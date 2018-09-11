#pragma once
#include "interface.h"
#include "globDefs.h"
#include "serial.h"
#include "serialEx.h"
#include "ATLComTime.h"

#define CHECKCOM 1000

extern CEvent g_eventCOMIFData;
extern CEvent g_eventCOMIFSendData;
extern CEvent g_eventCOMIFCheckData;

//#define NUM_DATABYTES_MASIMO_SPO2 7


class CInterfaceCOMport :
	public CInterface, public CSerialEx
{
public:

	static CInterfaceCOMport* GetInstance(BYTE iCOMmodule);
	static void DestroyInstance();

	virtual int Init(BYTE com)=0;
	virtual int Deinit()=0;
	virtual bool OpenCOM()=0;

	virtual void StartupSequenz()=0;
	virtual void InitializeSequenz()=0;
	virtual bool PerformMsg()=0;
	virtual bool isStateOk()=0;


	virtual void checkModuleState();

	


	void send(LPCOMMSG msg);

	void startCOMcheckThread(void);
	void stopCOMcheckThread(void);

	void startCOMThread(void);
	void stopCOMThread(void);

	void updateLastCOMCmd();
	void checkLastCOMCmd();

	bool isConnectedToCOM();
	bool isTrigger();
	

	//bool isSensorConnectedToModule();

private:
	//singleton
	static CInterfaceCOMport* theCOMInterface;

protected:

	static BYTE m_iCOMmodule;
	bool m_bStartup;

	bool m_bTrigger;
	SHORT	m_cLastWaveVal;


	static CList<LPCOMMSG, LPCOMMSG>  MSGSendCOM;
	CRITICAL_SECTION	csMSGSendCOM;
	CRITICAL_SECTION	csUpdateDataCOM;
	//CRITICAL_SECTION	csUpdateDataCO2;
	//CRITICAL_SECTION	csErrorDataCO2;

protected:
	CInterfaceCOMport(void);
	virtual ~CInterfaceCOMport(void);

	static friend UINT CCOMSendThread(LPVOID pc);
	CWinThread*	m_pcwtCOMSendThread;
	DWORD SendCOMData(void);
	bool m_bDoCOMSendThread;
	HANDLE m_hThreadCOMSend;

	static friend UINT CCOMInitThread(LPVOID pc);
	CWinThread*	m_pcwtCOMInitThread;
	DWORD COMInit(void);
	bool m_bDoCOMInitThread;
	HANDLE m_hThreadCOMInit;

	static friend UINT CCOMcheckThread(LPVOID pc);
	CWinThread*	m_pcwtCOMcheckThread;
	DWORD COMcheck(void);
	bool m_bDoCOMcheckThread;

	HANDLE m_hThreadCOMcheck;

	DWORD m_dwLastCOMCmd;
	bool m_bConnectedToCOM;
	
};
