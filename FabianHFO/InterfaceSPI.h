//=============================================================================
/** 
* \class CInterfaceSerial
* 		 
* \brief Interface for SPI interface to monitor PIC.
* 		 
* \author Rainer Kuehner
* \date 2017-10-23 Creation of file
*
**/
//=============================================================================
#pragma once
#include "interface.h"
#include "globDefs.h"

#include "nspiio.h"
//#include "DIO.h"


class CInterfaceSPI :
	public CInterface
{
	//typedef struct
	//{
	//	unsigned char *pchCommandBuf;         /* Buffer with command data */
	//	unsigned char *pchWriteBuf;           /* Buffer with data to send */
	//	unsigned char *pchReadBuf;            /* Buffer for received data */
	//	unsigned short wCommandLen;           /* Number of bytes to write only */
	//	unsigned short wDataLen;              /* Number of bytes to read/write */
	//} SPI_DATA, *PSPI_DATA;

	typedef struct tagSPISENDMESSAGE
	{
		BYTE			byteSPICommand[1];
		int				iValue;
	}SPISENDMESSAGE;

public:
	

	static CInterfaceSPI* GetInstance();
	static void DestroyInstance();

	bool Init();

	void StartSPIMonitorThread(void);

	BOOL SendSPICommand(SPISENDMESSAGE* pMessage, bool bIgnoreSPIstate=false);
	
	bool isSPIrunning();

	bool isMechanicalBreath();
	void activateMANBREATH();
	void deactivateMANBREATH();

	void ResetCO2Value();
	void ResetSPO2Value();

	bool NewAlarmData();
	void ResetAlarmData();

	bool NewFlowsensorData();
	void ResetFlowsensorData();

	CStringW GetDriverInfo();

	void Send_HARDWARE_CONFIG(WORD wHardwareConfig);

	void Send_VENT_MODE(eVentMode mode);
	void Send_KOMMANDO(int val);
	void Send_FLOWSENS_CMND(int val);
	void Send_MODE_OPTION1(WORD wMode);
	void Send_MODE_OPTION2(WORD wMode);

	void Send_ALARMKEY_PUSH();

	void Send_ALIVE_BYTE(int val);
	int Read_ALIVE_BYTE();

	void send_Shutdown();

	void Send_PARAVAL_PMAXVG(int val);//newVG
	void Send_PARAVAL_PINSP(int val);//newVG
	void Send_PARAVAL_P_PSV(int val);
	void Send_PARAVAL_PEEP(int val);
	void Send_PARAVAL_OXY_RATIO(int val);
	void Send_PARAVAL_VOLUME_LIMIT(int val);
	void Send_PARAVAL_VOLUME_GARANT(int val);
	void Send_ABORT_CRITERIONPSV(int val);
	void Send_PARAVAL_INSP_FLOW(int val);
	void Send_PARAVAL_EXH_FLOW(int val);
	void Send_PARAVAL_APNOE_TIME(int val);
	void Send_PARAVAL_BACKUP(int val);
	void Send_PARAVAL_INSP_TIME(int val);
	void Send_PARAVAL_EXH_TIME(int val);
	void Send_PARAVAL_HF_AMPL(int val);
	void Send_PARAVAL_HF_FREQ(int val);
	void Send_PARAVAL_HF_PMITT(int val);
	void Send_PARAVAL_HF_PMEANREC(int val);
	void Send_PARAVAL_HF_FREQ_REC(int val);
	void Send_PARAVAL_HF_ITIME_REC(int val);
	void Send_PARAVAL_TRIG_SCHWELLE(int val);
	void Send_FLOW_CORFACTOR_NEO(int val);
	void Send_FLOW_CORFACTOR_PED(int val);
	void Send_PARAVAL_RISETIME(int val);
	
	int GetMessureData();
	int ReadSPIData();

	int Read_ATZ_P_Peak();
	int Read_CHECKSUM();
	int Read_P_PROX_ADC();
	int Read_DRM_1_ADC();
	int Read_DRM_2_ADC();
	int Read_DRM_3_ADC();
	int Read_OXY1_ADC();
	int Read_OXY2_ADC();
	int Read_STATUS();
	WORD Read_VERSION1();
	WORD Read_VERSION2();
	int Read_VCC_24_BLEND_ADC();
	int Read_VCC_5_ADC();
	//int Read_PARA_P_PSV();
	//int Read_PARA_APNOE_TIME();
	//int Read_PARA_BACKUP();
	//int Read_PARA_INSP_TIME();
	//int Read_MODE_OPTION();
	//int Read_PARA_EXH_TIME();
	//int Read_PARA_P_INSP();
	//int Read_PARA_PEEP();
	int Read_VENT_MODE();
	//int Read_PARA_OXY_RATIO();
	//int Read_PARA_VOLUME_LIMIT();
	//int Read_PARA_VOLUME_GARANT();
	//int Read_PARA_FLOW_INSP();
	//int Read_PARA_FLOW_EXH();
	int Read_CAL_PRESS_SCALE();
	int Read_CAL_PRESS_OFFSET();
	//int Read_P_DELTA_MAX();
	//int Read_PARA_HF_AMPLITUDE();
	//int Read_PARA_HF_FREQ();
	//int Read_PARA_HF_MEAN_AIRWAY_PRESS();
	int Read_STATUS_PROXSENSOR();
	int Read_KOMMANDO();
	int Read_FLOWSENS_CMND();
	int Read_STATUS_FLOWSENSOR();
	int Read_DAC_FLOW_INSP();
	int Read_DAC_FLOW_EXH();
	int Read_FLOW_INSP_ADC();
	int Read_FLOW_EXH_ADC();
	//int Read_FLOW();
	int Read_SYST_OFFSET_FLOW_INSP();
	int Read_SYST_OFFSET_FLOW_EXH();
	int Read_SYST_SCALE_FLOW_INSP();
	int Read_SYST_SCALE_FLOW_EXH();
	int Read_FLOWCAL_DEV_A();
	int Read_FLOWCAL_DEV_B();
	int Read_OXY_ACTUAL();
	int Read_OXY_DEFAULT();
	int Read_STATUS_2();
	BYTE Read_AUTOFLOW_CORR();
	//int Read_PARA_TRIG_SCHWELLE();
	int Read_FLOW_CORFACTOR_NEO();
	int Read_FLOW_CORFACTOR_PED();
	//int Read_STATE_VOLGARANTY();
	//int Read_PROXVAL_VOLGARANTY();
	//int Read_V_TRIG();
	//int Read_LEAK();
	//int Read_PERC_ANTEIL_RESP();
	//int Read_C20C();
	//int Read_RESISTANCE();
	//int Read_DATASET_BLOCK();

	BOOL check_ALIVE_BYTE();

	void resetPressureAlarmDelay();

private:
	//singleton
	static CInterfaceSPI* theSPIInterface;

protected:
	CInterfaceSPI(void);
	virtual ~CInterfaceSPI(void);

	void IncrementAlive();

	void StartSPICommunicationThread(void);
	void StopSPICommunicationThread( void );

	void StopSPIMonitorThread( void );

	void checkUpperPressureLimit(eVentMode eActiveVentMode, SHORT iMaxPressureVal);
	void checkLowerPressureLimit(eVentMode eActiveVentMode, SHORT iMinPressureVal);
	
	bool ReadNSPIdata(BYTE* pCommand,WORD wcommandSize, BYTE* pBuffer,WORD wBufSize, WORD wOffset);	
	UINT ReadNSPIPFVblock(BYTE *pBuffer);	
	DWORD ReadNSPIATZblock(BYTE *pBuffer,WORD wSize);	
	DWORD ReadNSPIAVGblock(BYTE *pBuffer,WORD bufSize);

	//for test simulation
	DWORD SimulateNSPIblock(BYTE *pBuffer);

	BOOL WriteNSPIdata(BYTE* command,WORD wcommandSize,WORD wValue, WORD wOffset);	//submit data

protected:

	
	CRITICAL_SECTION	csSPIFile;
	CRITICAL_SECTION	csSPIsendqueue;

	CList<SPISENDMESSAGE*, SPISENDMESSAGE *> SPIsendqueue;
	
	HANDLE m_hSPIFile;

	static friend UINT CSPIMonitorThread(LPVOID pc);
	CWinThread*	m_pcwtSPIMonitorThread;
	bool m_bDoSPIMonitorThread;
	CEvent eventSPITime;
	HANDLE m_hThreadSPIMonitor;
	DWORD SPIMonitorData(void);

	static friend UINT CSPICommunicationThread(LPVOID pc);
	CWinThread*	m_pcwtSPICommunicationThread;
	bool m_bDoSPICommunicationThread;
	HANDLE m_hThreadSPICommunication;
	DWORD SPICommunicationData(void);


	DWORD m_dwWaittimeATZ;
	DWORD m_dwWaittimeDelayPEEP_OG;
	DWORD m_dwWaittimeDelayPEEP_UG;

	UINT m_iReadERROR;

	SHORT m_iLastValSPO2;
	SHORT m_iLastValCO2;

	BYTE m_byAlive;

	bool m_bAlarmData;
	bool m_bFlowsensorData;
	bool m_bManBreathRunning;
	bool m_bSPIrunning;
	bool m_bShowReadSPIDataError;


	SHORT m_iHighPressureValue;
	SHORT m_iLowPressureValue;

	bool m_bMechanicalATZ;
	bool m_bHighPressureAlarmPending;
	bool m_bLowPressureAlarmFlag;
	bool m_bLowPressureAlarmPending;
	

	//need for simulation only
	DWORD m_dwLastSimulatedData;
	float m_iGraphCount;
	int m_i4Count;
	int m_iTestCntSPIbufSize;
	SHORT m_iTempF;
	WORD m_iTempP;
	WORD m_iTempV;
	
	
	//bool m_bAutoflowBit;

	
	//DWORD m_testTickCount;
	//int m_iTest;

	/*BYTE m_iDataSend;
	DWORD m_dwLastTerminalSend;
	DWORD m_dwLastSPIdata;*/
	//BYTE m_iCntAcuLinkData;
	

	
};
