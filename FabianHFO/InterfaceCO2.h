#pragma once
#include "interface.h"
#include "globDefs.h"
#include "serial.h"
#include "serialEx.h"
#include "ATLComTime.h"

#define CHECKETCO2 1000

extern CEvent g_eventCO2Data;
extern CEvent g_eventETCO2SendData;
extern CEvent g_eventCO2CheckData;

//class CSerialPort : public CSerialEx
//{
//public:
//	int OpenSerialPort(CString sPort, LinkTypes eLinkType);
//	void CloseSerialPort();
//	/*int WriteBuffer();
//	int WriteBuffer(char *buf, DWORD len);
//	void OnEvent (EEvent eEvent, EError eError);
//	WORD CalcChkSum(unsigned char *p, WORD len);
//	WORD AddByteToChkSum(WORD wActChkSum, unsigned char bByte);*/
//	int PerformMsg();
//
//}; // class CSerialPort


//class CInterfaceCO2_CAPNOSTAT;

// CInterfaceCO2

class CInterfaceCO2 : public CInterface, public CSerialEx
{

public:
	

	static CInterfaceCO2* GetInstance(BYTE iCO2module);
	static void DestroyInstance();

	//CMVModel *getModel();

	virtual int Init(BYTE com)=0;
	virtual int Deinit()=0;
	virtual void StartupSequenz()=0;
	virtual void InitializeSequenz()=0;
	//virtual bool ParseCO2Command(BYTE* pData, size_t iLen)=0;
	virtual bool PerformMsg()=0;

	/*virtual void startWriteBuffer()=0;
	bool writeBufferStarted();*/

	virtual void resetStateBytes()=0;
	virtual void setStateBytes(BYTE state)=0;
	virtual BYTE getStateBytes()=0;
	virtual bool isStateOk()=0;

	virtual void setExtendedStateBytes(BYTE state)=0;
	virtual BYTE getExtendedStateBytes()=0;
	virtual bool isExtendedStateOk()=0;

	virtual CString get_ModuleVersion();


	//virtual void OnEvent (EEvent eEvent, EError eError);
	virtual void changeCO2Unit(eCO2unit unitNew,eCO2unit unitOld);
	virtual void CheckModuleState();

	virtual void turnOff();

	virtual void set_GasCompensation(BYTE byO2Compensation=16, BYTE byBalanceGas=0, SHORT iAnestAgent=0);
	virtual void get_GasCompensation();

	virtual void set_BaroPressure(SHORT val=760);
	virtual void get_BaroPressure();

	virtual void set_PatientMode(BYTE val);
	virtual void get_PatientMode();

	virtual void set_MeasurementMode(BYTE val);
	virtual void get_MeasurementMode();

	virtual void set_PumpStateOn(bool bGetExState=true);
	virtual void set_PumpStateOff(bool bGetExState=true);
	//virtual void get_PumpState(){};

	virtual void set_Configuration();

	virtual void set_StandbyMode(bool state);
	virtual bool get_StandbyMode();


	virtual int get_MaxOpHRSCalibration();
	virtual int get_RemainServiceHrs();
	virtual COleDateTime get_CalibrationDateTime();
	virtual void get_LastCalDate();

	virtual bool isPumpOn();
	virtual bool isCO2ValueValid();
	virtual bool isETCO2ValueValid();
	virtual bool isFreqValueValid();
	virtual bool isFICO2ValueValid();
	virtual bool isCalibrationDue();
	virtual bool isServiceDue();
	virtual bool isSystemDateOk();

	virtual void set_restartBreathAlgorithm();

	void retryConnection();

	virtual void zeroCalibration();

	virtual int ShowError (LONG lError, LPCTSTR lptszMessage);
	//virtual void OnEvent (EEvent eEvent, EError eError)=0;
	/*void Deinit();
	void Restart();

	bool Initialize();*/

	void UpdateLastCO2Cmd();
	void CheckLastCO2Cmd();

	//bool IsConnected();

	void send(LPCO2MSG msg);

	void startETCO2checkThread(void);
	void stopETCO2checkThread(void);

	void startETCO2Thread(void);
	void stopETCO2Thread(void);

	
	//void startSendThread(void);

private:
	//singleton
	static CInterfaceCO2* theETCO2Interface;

protected:
	static BYTE m_iCO2module;
	bool m_bStartup;

	bool m_bInit;

	bool m_bStandby;

	static CList<LPCO2MSG, LPCO2MSG>  MSGSendETCO2;
	CRITICAL_SECTION	  csMSGSendETCO2;
	CRITICAL_SECTION	csUpdateDataCO2;
	CRITICAL_SECTION	csErrorDataCO2;

	BYTE m_byStateBytes;
	BYTE m_byExtendedStateBytes;

	DWORD m_dwLastCO2Cmd;

	BYTE m_com;
	BYTE m_iBPM;
	SHORT m_iETCO2;
	SHORT m_iFICO2;

	CString m_szVersion;
	bool m_bVersion;

	bool m_bPumpStateOn;
	bool m_bCO2ValueValid;
	bool m_bETCO2ValueValid;
	bool m_bFreqValuevalid;
	bool m_bFICO2ValueValid;

//public:
//	virtual ~CInterfaceCO2();

protected:
	CInterfaceCO2();
	virtual ~CInterfaceCO2();

	//DWORD m_dwLastCO2send;
	
	friend UINT CETCO2SendThread(LPVOID pc);
	CWinThread*	m_pcwtETCO2SendThread;
	DWORD SendETCO2Data(void);
	bool m_bDoETCO2SendThread;
	HANDLE m_hThreadETCO2Send;

	friend UINT CETCO2InitThread(LPVOID pc);
	CWinThread*	m_pcwtETCO2InitThread;
	DWORD ETCO2Init(void);
	bool m_bDoETCO2InitThread;
	//CEvent eventCO2Time;
	HANDLE m_hThreadETCO2Init;

	friend UINT CETCO2checkThread(LPVOID pc);
	CWinThread*	m_pcwtETCO2checkThread;
	DWORD ETCO2check(void);
	bool m_bDoETCO2checkThread;
	//CEvent g_eventCO2CheckData;
	HANDLE m_hThreadETCO2check;


	
};


