//=============================================================================
/** 
* \class CInterfaceSPO2
* 		 
* \brief Interface base class for SPO2 interfaces.
* 		 
* \author Rainer Kuehner
* \date 2017-10-23 Creation of file
*
**/
//=============================================================================
#pragma once
#include "interface.h"
#include "globDefs.h"
#include "serial.h"
#include "serialEx.h"
#include "ATLComTime.h"

#define CHECKSPO2 1000

extern CEvent g_eventSPO2Data;
extern CEvent g_eventSPO2SendData;
extern CEvent g_eventSPO2CheckData;

#define NUM_DATABYTES_MASIMO_SPO2 7


class CInterfaceSPO2 :
	public CInterface, public CSerialEx
{
public:


	static CInterfaceSPO2* GetInstance(BYTE iSPO2module);
	static void DestroyInstance();

	virtual int Init(BYTE com)=0;
	virtual int Deinit(bool bReinit)=0;
	virtual bool OpenCOM()=0;

	//virtual void StartupSequenz()=0;
	virtual void InitializeSequenz()=0;
	virtual bool PerformMsg()=0;
	virtual bool isStateOk()=0;

	virtual WORD get_ProductID();
	virtual WORD get_VersionMCU();
	virtual WORD get_VersionDSP();
	virtual SHORT get_EXCEPTION2();

	virtual void checkModuleState();

	virtual void set_FastSATon(bool bFastSATon);
	virtual void set_SensitivityMode(eSPO2sensitivitymode mode);
	virtual void set_SPO2averagingTime(eSPO2averagingtime avtime);
	virtual void set_SPO2alarmDelay(eSPO2alarmdelay delay);

	
	void send(LPSPO2MSG msg);

	void startSPO2checkThread(void);
	void stopSPO2checkThread(void);

	void startSPO2initThread(void);
	void stopSPO2initThread(void);

	void startSPO2sendThread(void);
	void stopSPO2sendThread(void);

	void updateLastSpO2Cmd();
	void checkLastSpO2Cmd();
	
	bool isSensorConnectedToUSB();
	bool isSensorConnectedToModule();

private:
	//singleton
	static CInterfaceSPO2* theSPO2Interface;

protected:

	static BYTE m_iSPO2module;
	bool m_bStartup;

	static CList<LPSPO2MSG, LPSPO2MSG>  MSGSendSPO2;
	CRITICAL_SECTION	csMSGSendSPO2;
	CRITICAL_SECTION	csUpdateDataSpO2;
	//CRITICAL_SECTION	csUpdateDataCO2;
	//CRITICAL_SECTION	csErrorDataCO2;

protected:
	CInterfaceSPO2(void);
	virtual ~CInterfaceSPO2(void);

	static friend UINT CSPO2SendThread(LPVOID pc);
	CWinThread*	m_pcwtSPO2SendThread;
	DWORD SendSPO2Data(void);
	bool m_bDoSPO2SendThread;
	HANDLE m_hThreadSPO2Send;

	static friend UINT CSPO2InitThread(LPVOID pc);
	CWinThread*	m_pcwtSPO2InitThread;
	DWORD SPO2Init(void);
	bool m_bDoSPO2InitThread;
	HANDLE m_hThreadSPO2Init;
	CEvent g_eventCO2Startup;

	static friend UINT CSPO2checkThread(LPVOID pc);
	CWinThread*	m_pcwtSPO2checkThread;
	DWORD SPO2check(void);
	bool m_bDoSPO2checkThread;

	HANDLE m_hThreadSPO2check;

	DWORD m_dwLastSpO2Cmd;
	
	SHORT m_iSpO2;
	SHORT m_iPulseRate;
	SHORT m_iPerfusionIndex;

	bool m_bSensorConnectedToUSB;
	bool m_bSensorConnectedToModule;

	//bool m_binitialized;
};
