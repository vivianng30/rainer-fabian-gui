/**********************************************************************************************//**
* \class CInterfaceCO2_MICROPOD
* 		 
* \brief Base class for CO2 MICROPOD interface.
* 		 
* \author Rainer Kuehner
* \date 2018-02-26 Creation of file
*
* Implements the object as singleton.
*
 **************************************************************************************************/
#pragma once
#include "InterfaceCO2.h"


typedef enum
{
	CO2_MICROPOD_HEADER,
	CO2_MICROPOD_DATA_SIZE,
	CO2_MICROPOD_DATA_SIZE1,
	CO2_MICROPOD_CODE,
	CO2_MICROPOD_CODE1,
	CO2_MICROPOD_DATA,
	CO2_MICROPOD_DATA1,
	CO2_MICROPOD_CHKSUM,
	CO2_MICROPOD_CHKSUM1,
} ReceiverStatesMicropod;


class CInterfaceCO2_MICROPOD: public CInterfaceCO2
{
public:
	CInterfaceCO2_MICROPOD(void);
	~CInterfaceCO2_MICROPOD(void);

	int Init(BYTE com);
	int Deinit();

	void turnOff();

	void StartupSequenz();
	void InitializeSequenz();
	bool PerformMsg();
	//bool ParseCO2Command(BYTE* pData, size_t iLen);
	void OnEvent (EEvent eEvent, EError eError);
	void changeCO2Unit(eCO2unit unitNew,eCO2unit unitOld);
	void CheckModuleState();

	void set_GasCompensation(BYTE byO2Compensation=16, BYTE byBalanceGas=0, SHORT iAnestAgent=0);
	void get_GasCompensation();

	void set_BaroPressure(SHORT val=760);
	void get_BaroPressure();

	void set_PatientMode(BYTE val);
	void get_PatientMode();

	void set_MeasurementMode(BYTE val);
	void get_MeasurementMode();

	void set_PumpStateOn(bool bGetExState=true);
	void set_PumpStateOff(bool bGetExState=true);

	void set_Configuration();

	void set_StandbyMode(bool state);
	bool get_StandbyMode();

	//CString get_ModuleVersion();

	//void retryConnection();

	void set_restartBreathAlgorithm();

	int get_MaxOpHRSCalibration();
	COleDateTime get_CalibrationDateTime();
	int get_RemainServiceHrs();

	void get_LastCalDate();

	bool isPumpOn();
	bool isCO2ValueValid();
	bool isETCO2ValueValid();
	bool isFreqValueValid();
	bool isFICO2ValueValid();
	bool isCalibrationDue();
	bool isServiceDue();
	bool isSystemDateOk();

	//void startWriteBuffer();
	

	void resetStateBytes();
	void setStateBytes(BYTE state);
	BYTE getStateBytes();
	bool isStateOk();

	void setExtendedStateBytes(BYTE state);
	BYTE getExtendedStateBytes();
	bool isExtendedStateOk();

	void getModuleSetup();
	void setModuleSetup(BYTE byte0, 
		BYTE byte1, 
		BYTE byte2, 
		BYTE byte3);

	void getModuleConfig(BYTE byte0);
	void setModuleConfig(BYTE byte0, WORD value);

	void getOperatingMode();
	void setOperatingMode(BYTE byte0);

	void getModuleVersion();
	void getBreathMessage();

	void getExtendedState();

	void getRemainTimeToCal();
	
	void getServiceHrs();
protected:
	DWORD SendCO2Command(	BYTE CO2_command=0, 
		BYTE Anzahlbytes=0, 
		BYTE byte0=0, 
		BYTE byte1=0, 
		BYTE byte2=0, 
		BYTE byte3=0, 
		BYTE byte4=0, 
		BYTE byte5=0, 
		BYTE byte6=0);


	void doInterpolationWave(SHORT iTempfaWaveBuf);

	//bool ValidateMessage(BYTE* pData, size_t iLen);
	//int ShowError (LONG lError, LPCTSTR lptszMessage);

protected:
	CRITICAL_SECTION	csLastCalCheck;

	//TEST statemachine
	BYTE	faRecvBuf[256];
	BYTE	faDataBuf[256];
	WORD	fwDataCnt;
	DWORD			fdwRecvBufLen;
	ReceiverStatesMicropod	feRecvState;
	BYTE			fwCalcChkSum;
	BYTE			fwDataSize;
	BYTE			fwDataCode;
	BYTE			fwReadChkSum;

	DWORD m_dwLastCalCheck;

	UINT m_iAmbientPressure;
	COleDateTime m_dtCalibrationDateTime;
	int m_iMaxOpHRSCalibration;
	bool m_bCalibrationIsDue;
	int m_iRemainServiceHrs;
	bool m_bServiceIsDue;
	bool m_bSystemDateOk;

	SHORT	m_cLastCO2WaveVal;
	//SHORT	faetCO2Interpolation[50];

	BYTE m_byCmdBuffer[1001];
	UINT m_countCmdBuf;

	bool m_bCmdHeader;

	UINT m_iCmdLength;
	BYTE m_checksumCalc;
	BYTE m_crcReadIn;

	BYTE byBreak;

	UINT m_iCmdIdx;
	BYTE m_byLength;

	//module configuration
	BYTE m_cfgBTPS;				
	BYTE m_cfgPEAKPICK;			
	BYTE m_cfgDISTINGBREATHMSG;	
	BYTE m_cfgCO2UNITS;			
	BYTE m_cfgPERIODICBREATHMSG;	
	BYTE m_cfgWAVEMEASINTERVAL;	
	BYTE m_cfgAUTOSTANDBY;			
	BYTE m_cfgPATIENTMODE;			
	BYTE m_cfgCO2RANGE;			
	BYTE m_cfgMEASUREMODE;	
	BYTE m_cfgCONTCO2NUM;
	BYTE m_cfgCONTPERCSPONT;
	BYTE m_cfgOPERATINGMODE;

	BYTE m_byConfigBuffer[MAXMICROPODMODULECONFIG];
	BYTE m_iConfigBufferCnt;

	

	//int m_iTest;

	
	//SHORT m_iTestValue;
	
};



