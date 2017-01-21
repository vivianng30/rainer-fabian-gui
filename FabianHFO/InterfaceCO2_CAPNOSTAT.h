#pragma once
#include "InterfaceCO2.h"

typedef enum
{
	CO2_CAPNOSTAT_CMD,
	CO2_CAPNOSTAT_NBF,
	CO2_CAPNOSTAT_DATA,
	CO2_CAPNOSTAT_CHKSUM,
} ReceiverStatesCapnostat;

class CInterfaceCO2_CAPNOSTAT: public CInterfaceCO2
{
public:
	CInterfaceCO2_CAPNOSTAT();
	~CInterfaceCO2_CAPNOSTAT();

	
	int Init(BYTE com);
	int Deinit();
	void StartupSequenz();
	void InitializeSequenz();
	bool PerformMsg();
	//bool ParseCO2Command(BYTE* pData, size_t iLen);
	void OnEvent (EEvent eEvent, EError eError);
	void changeCO2Unit(eCO2unit unitNew,eCO2unit unitOld);
	void CheckModuleState();

	//void startWriteBuffer();
	//CMVModel *getModel();

	void set_GasCompensation(BYTE byO2Compensation=16, BYTE byBalanceGas=0, SHORT iAnestAgent=0);
	void get_GasCompensation();

	void set_BaroPressure(SHORT val=760);
	void get_BaroPressure();

	void set_Configuration();

	void set_StandbyMode(bool state);
	bool get_StandbyMode();

	//CString get_ModuleVersion();

	void zeroCalibration();

	//void retryConnection();
	void set_restartBreathAlgorithm();

	void resetStateBytes();
	void setStateBytes(BYTE state);
	BYTE getStateBytes();
	bool isStateOk();


	void getModuleVersion();

	void setExtendedStateBytes(BYTE state);
	BYTE getExtendedStateBytes();
	bool isExtendedStateOk();

	bool isPumpOn();
	bool isCO2ValueValid();
	bool isETCO2ValueValid();
	bool isFreqValueValid();
	bool isFICO2ValueValid();

protected:

	void doInterpolationWave(SHORT iTempfaWaveBuf);

	DWORD SendCO2Command(	BYTE CO2_command=0, 
		BYTE Anzahlbytes=0, 
		BYTE byte0=0, 
		BYTE byte1=0, 
		BYTE byte2=0, 
		BYTE byte3=0, 
		BYTE byte4=0);
	void AddChecksum(short num, BYTE buf[]);
	BOOL IsChecksumValid(BYTE *lpBuf, short num);
	BYTE GetChecksum(BYTE Anzahlbytes, BYTE *buf);

	void Send_StopContinousMode();
	void Send_CO2WaveformDataMode();
	void Send_CapnostatZero();

	void Read_BaroPressure();
	void Send_BaroPressure(SHORT val=760);

	void Read_GasTemperature();
	void Send_GasTemperature(SHORT val=350);

	void Read_ETCO2TimePeriod();
	void Send_ETCO2TimePeriod(BYTE val=10);

	void Read_NoBreathDetectedTimeout();
	void Send_NoBreathDetectedTimeout(BYTE val=20);

	void Read_CurrentCO2Units();
	void Send_CurrentCO2Units(BYTE val=0);

	void Read_SleepMode();
	void Send_SleepMode(BYTE val=0);

	void Read_ZeroGasType();
	void Send_ZeroGasType(BYTE val=1);

	void Read_GasCompensation();
	void Send_GasCompensation(BYTE byO2Compensation=16, BYTE byBalanceGas=0, SHORT iAnestAgent=0);

	bool ValidateMessage(BYTE* pData, size_t iLen);
	//int ShowError (LONG lError, LPCTSTR lptszMessage);
protected:
	/*BYTE m_byPrioritizedCO2Status;*/

	ReceiverStatesCapnostat	feRecvState;
	//SHORT	faetCO2Interpolation[10];

	BYTE	faRecvBuf[256];
	BYTE	faDataBuf[256];

	BYTE m_byConfigBuffer[MAXCAPNOSTATMODULECONFIG];

	DWORD			fdwRecvBufLen;



	SHORT	m_cLastCO2WaveVal;

	WORD m_cfgGasTemp;
	WORD	fwDataCnt;
	
	BYTE m_cfgPEAKPICK;			
	BYTE m_cfgCO2UNITS;	
	BYTE m_cfgNOBREATHDETECTED;
	BYTE m_cfgOPERATINGMODE;
	BYTE m_cfgZEROGASTYPE;
	BYTE m_cfgGasCompensation;

	BYTE m_iConfigBufferCnt;
	BYTE			fwDataSize;
	BYTE			fwDataCode;

	char			fwCalcChkSum;
	char			fwReadChkSum;


};
