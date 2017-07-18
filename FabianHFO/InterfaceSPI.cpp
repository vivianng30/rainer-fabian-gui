#include "StdAfx.h"
#include "InterfaceSPI.h"
#include "MVModel.h"
#include "acuLink.h"
#include "MVViewHandler.h"

#define WAITACKN 2

//The pin number is the sum of the bit number and the port number multiplied by 8. 
//For example to modify Pin 11 of connector J5, look at the table in the NetDCU device driver document. 
//It shows that this is bit 2 of port 1. So the pin number is 2 + 1*8 = 10.
//   BYTE pin = 10;
//   DeviceIoControl(hDev, IOCTL_DIO_SET_PIN, &pin, 1, NULL, 0, NULL, NULL);


//#define ACKNPINBIT 4
//#define ACKNPINPORT 0

#define CDPINBIT 4
//#define CDPINPORT 0
//#define CDPINBIT 8
//#define CDPINPORT 1

#define MAXPFVBLOCKBUFSIZE 210
#define MAXNUMBLOCKS 30

#define BLOCKBUFSIZE 42
//#define ATZBLOCKBUFSIZE 40
//#define AVGBLOCKBUFSIZE 40

#define INSPIRATIONBIT		(1 << 0)
#define TRIGGEREDATZBIT		(1 << 1)
#define ALARMCHANGEDBIT		(1 << 2)
#define FLOWSENSCHANGEDBIT	(1 << 3)
#define MECHATZBIT			(1 << 4)
#define AUTOFLOWBIT			(1 << 5)


#define TIMEOUT_MONITORDATA 1000
#define SPI_DELAY			200

//Minimum & Maximum of messurement data
#define	MesData_P_Peak_Min		0
#define	MesData_P_Peak_Max		1500
#define	MesData_P_Mean_Min		-500
#define	MesData_P_Mean_Max		1500
#define	MesData_PEEP_Min		-500
#define	MesData_PEEP_Max		1500
#define	MesData_T_insp_PSV_Min	0
#define	MesData_T_insp_PSV_Max	5000
#define	MesData_Dyn_Compl_Min	0
#define	MesData_Dyn_Compl_Max	50000
#define	MesData_C20C_Min		0
#define	MesData_C20C_Max		999
#define	MesData_Resistance_Min	0
#define	MesData_Resistance_Max	9990
#define	MesData_Min_Vol_Min		0
#define	MesData_Min_Vol_Max		20000
#define	MesData_Perc_Resp_Min	0
#define	MesData_Perc_Resp_Max	100
#define	MesData_TVE_Min			0
#define	MesData_TVE_Max			6000
#define	MesData_TVE_Resp_Min	0
#define	MesData_TVE_Resp_Max	6000
#define	MesData_TVE_Pat_Min		0
#define	MesData_TVE_Pat_Max		6000
#define	MesData_Leak_Min		0
#define	MesData_Leak_Max		100
#define	MesData_Freq_Min		0
#define	MesData_Freq_Max		250
#define	MesData_Trig_Value_Min	0
#define	MesData_Trig_Value_Max	7500
#define	MesData_TVI_Min			0
#define	MesData_TVI_Max			6000
#define	MesData_HFO_Amp_Min		0
#define	MesData_HFO_Amp_Max		2000
#define	MesData_TVE_HFO_Min		0
#define	MesData_TVE_HFO_Max		2000
#define	MesData_DCO2_Min		0
#define	MesData_DCO2_Max		10000
#define	MesData_HFO_Freq_Min	0
#define	MesData_HFO_Freq_Max	20

CInterfaceSPI* CInterfaceSPI::theSPIInterface=0;



CInterfaceSPI::CInterfaceSPI(void)
{

	InitializeCriticalSection(&csSPIFile);
	InitializeCriticalSection(&csSPIsendqueue);

	m_dwWaittimeATZ=0;
	m_dwWaittimeDelayPEEP_OG=0;
	m_dwWaittimeDelayPEEP_UG=0;
	m_dwLastSimulatedData=0;

	m_iReadERROR=0;

	m_iLastValSPO2=0;
	m_iLastValCO2=0;

	m_byAlive=0;

	m_bAlarmData=false;
	m_bFlowsensorData=false;
	m_bManBreathRunning=false;
	m_bSPIrunning=false;
	m_bShowReadSPIDataError=true;

	m_bSPIrunning=false;
	m_bShowReadSPIDataError=true;
	m_byAlive=1;
	m_bDoSPIMonitorThread=false;
	m_bDoSPICommunicationThread=false;
	m_bAlarmData=false;
	m_bFlowsensorData=false;
	m_iReadERROR=0;
	m_iLastValCO2=0;
	m_iLastValSPO2=0;
	
	m_hSPIFile=INVALID_HANDLE_VALUE;

	//simulation 
	m_i4Count=0;
	m_iTempP=0;
	m_iTempF=0;
	m_iTempV=0;

	m_iTestCntSPIbufSize=18;
	m_dwLastSimulatedData=0;
	
	m_bManBreathRunning=false;

	m_dwWaittimeATZ=0;

	m_bMechanicalATZ=false;

	m_bHighPressureAlarmPending=false;
	m_dwWaittimeDelayPEEP_OG=0;
	m_iHighPressureValue=0;

	m_bLowPressureAlarmFlag=false;
	m_bLowPressureAlarmPending=false;
	m_dwWaittimeDelayPEEP_UG=0;
	m_iLowPressureValue=0;

	m_pcwtSPIMonitorThread=NULL;
	m_pcwtSPICommunicationThread=NULL;

	m_hThreadSPIMonitor=INVALID_HANDLE_VALUE;
	m_hThreadSPICommunication=INVALID_HANDLE_VALUE;

	m_iGraphCount=0;

	m_dwWaittimeATZ=0;
	m_dwWaittimeDelayPEEP_OG=0;
	m_dwWaittimeDelayPEEP_UG=0;
	m_dwLastSimulatedData=0;

	m_iReadERROR=0;

	m_iLastValSPO2=0;
	m_iLastValCO2=0;

	m_byAlive=0;

	m_bAlarmData=false;
	m_bFlowsensorData=false;
	m_bManBreathRunning=false;
	m_bSPIrunning=false;
	m_bShowReadSPIDataError=false;
}

CInterfaceSPI::~CInterfaceSPI(void)
{
	StopSPIMonitorThread();

	if(m_pcwtSPIMonitorThread!=NULL)
	{
		delete m_pcwtSPIMonitorThread;
		m_pcwtSPIMonitorThread=NULL;
		if(m_hThreadSPIMonitor!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSPIMonitor);
			m_hThreadSPIMonitor=INVALID_HANDLE_VALUE;
		}

	}

	StopSPICommunicationThread();

	if(m_pcwtSPICommunicationThread!=NULL)
	{
		delete m_pcwtSPICommunicationThread;
		m_pcwtSPICommunicationThread=NULL;

		if(m_hThreadSPICommunication!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSPICommunication);
			m_hThreadSPICommunication=INVALID_HANDLE_VALUE;
		}
	}

	if(m_hSPIFile!=INVALID_HANDLE_VALUE)
		CloseHandle(m_hSPIFile);

	while (!SPIsendqueue.IsEmpty())
	{
		SPISENDMESSAGE* message = SPIsendqueue.RemoveHead();
		if(message)
		{
			delete message;
		}
	}

	DeleteCriticalSection(&csSPIFile);
	DeleteCriticalSection(&csSPIsendqueue);

}

// **************************************************************************
// 
// **************************************************************************
CInterfaceSPI* CInterfaceSPI::GetInstance()
{
	if(theSPIInterface == 0)
	{
		theSPIInterface = new CInterfaceSPI;
		//theSPIInterface->Init();
	}
	return theSPIInterface;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::DestroyInstance()
{
	

	if(theSPIInterface != NULL)
	{
		delete theSPIInterface;
		theSPIInterface = NULL;
	}
}

// **************************************************************************
// 
// **************************************************************************
bool CInterfaceSPI::Init()
{
	m_hSPIFile = CreateFile(_T("SPI1:"),GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ, NULL,OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, 0);

	if(m_hSPIFile==INVALID_HANDLE_VALUE)
	{
		theApp.getLog()->WriteLine(_T("#HFO:0125"));
		return false;
		
	}

	StartSPICommunicationThread();

	//DRIVER_INFO cInfo;
	//if (!DeviceIoControl(hDevice, IOCTL_DRIVER_GETINFO, NULL, 0,
	//	&cInfo, sizeof(cInfo), NULL, NULL))
	//{
	//	cInfo.wVerMajor = 1; /* Command failed: this is V1.x */
	//	cInfo.wVerMinor = 0;
	//}
	//printf("NSPI driver V%d.%d", cInfo.wVerMajor, cInfo.wVerMinor);

	//Send_MODE_OPTION();

	return true;
}


// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::StartSPIMonitorThread( void )
{
	m_bDoSPIMonitorThread=true;
	//m_pcwtSPIMonitorThread=AfxBeginThread(CSPIMonitorThread,this);

	if(m_pcwtSPIMonitorThread!=NULL)
	{
		delete m_pcwtSPIMonitorThread;
		m_pcwtSPIMonitorThread=NULL;

		if(m_hThreadSPIMonitor!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSPIMonitor);
			m_hThreadSPIMonitor=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtSPIMonitorThread=AfxBeginThread(CSPIMonitorThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadSPIMonitor=m_pcwtSPIMonitorThread->m_hThread;
	m_pcwtSPIMonitorThread->m_bAutoDelete = FALSE; 
	m_pcwtSPIMonitorThread->ResumeThread();
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::StopSPIMonitorThread( void )
{
	if(m_bDoSPIMonitorThread)
	{
		m_bDoSPIMonitorThread=false;
		eventSPITime.SetEvent();
		//WaitForSingleObject(m_pcwtSPIMonitorThread->m_hThread,1000);

		if (WaitForSingleObject(m_pcwtSPIMonitorThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:013a"));
			if(!TerminateThread(m_pcwtSPIMonitorThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:013b"));
			}
		}
	}
	
}
// **************************************************************************
// 
// **************************************************************************
static UINT CSPIMonitorThread( LPVOID pc )
{
	try
	{
		((CInterfaceSPI*)pc)->SPIMonitorData();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CSPIMonitorThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CSPIMonitorThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}

	//((CInterfaceSPI*)pc)->SPIMonitorData();
	return TRUE;
}


// **************************************************************************
// 
// **************************************************************************
DWORD CInterfaceSPI::SPIMonitorData(void) 
{
	CeSetThreadPriority(m_pcwtSPIMonitorThread->m_hThread,130);

	bool bMonitorData=false;
	bool bAlarmData=false;
	bool bFlowsensorData=false;
	PBUFSPI _pBufData;
	_pBufData.bATZ=false;
	_pBufData.bTrig=false;
	_pBufData.iValPressure=0;
	_pBufData.iValFlow=0;
	_pBufData.iValVolume=0;
	_pBufData.iValCO2=0;
	_pBufData.iValSPO2=0;
	PBUFSPO2 _pSPO2BufData;
	_pSPO2BufData.iValSPO2=0;
	
	BYTE byStatus;
	BYTE byLoProx;
	BYTE byHiProx;
	BYTE byLoFlow;
	BYTE byHiFlow;
	BYTE byLoVolume;
	BYTE byHiVolume;

	DWORD dwResult=0;
	BYTE command[1];
	UINT bufSize=0;
	BYTE byBuffer[MAXPFVBLOCKBUFSIZE];

	m_bShowReadSPIDataError=true;

	SHORT iRes=0;

	int iPressureAv=0;
	int iFlowAv=0;	//Achtung, tatsächlicher Meß-Wert ist um Faktor 2 größer
	int iVolumeAv=0;
	int iCountValues=0;

#ifdef SIMULATION_ENTREK
	DWORD WAIT=500;
	Sleep(30000);
#else
	DWORD WAIT=19;
#endif
	
	DWORD dwWait=0;
	DWORD dwDiff = 0;
	DWORD dwStart=0;
	DWORD dwEnd=0;

	int iCountSPIERROR=0;
	int iCountDIOERROR=0;
	bool bRetryReadStatus=false;
	bool bRetryReadStatusFlowsensor=false;

	bool bHFOmode=false;
	UINT iCntTerminal=1;
	UINT iCntFOTdata=1;
	UINT iCntSpO2=0;
	WORD iCntThreadWatchdog=0;

	DWORD dwLastGetMessureData=GetTickCount();
	DWORD dwLastInspirationBit=GetTickCount();
	DWORD dwLastTriggerATZbit=GetTickCount();
	DWORD dwLastAlarmchangebit=GetTickCount();
	DWORD dwLastExspirationData=GetTickCount();
	DWORD dwLastMonitorData=GetTickCount();
	DWORD dwLastNewFlowsensorData=GetTickCount();

	BYTE iCntAcuLinkData=1;

	bool bCheckFlowsensDataTimeout=false;
	bool bCheckAlarmchangeTimeout=false;
	bool bBackup=false;
	bool bExspiration=false;
	bool bInspiration=false;
	bool bTriggeredBreath=false;
	bool bAutoflowBit=false;

	m_bSPIrunning=true;
	
	WORD iCnt=0;
	while(iCnt<20 && m_bDoSPIMonitorThread)
	{
		Sleep(100);
		iCnt++;
	}

	do
	{
		bMonitorData=false;
		bAlarmData=false;
		bFlowsensorData=false;

		dwResult=0;
		bufSize=0;

		dwEnd=GetTickCount();

		if(dwEnd>=dwStart)
			dwDiff=dwEnd-dwStart;
		else
			dwDiff=0;

		if(dwDiff<WAIT)
		{
			dwWait=WAIT-dwDiff;
		}
		else
		{
			dwWait=0;
		}

		DWORD dw = ::WaitForSingleObject(eventSPITime, dwWait);

		if(!m_bDoSPIMonitorThread)
			continue;

		dwStart = GetTickCount();

		if(dw!=WAIT_OBJECT_0)
		{
			if(m_hSPIFile!=INVALID_HANDLE_VALUE)
			{
				if(getModel()->isSPIavailabel()==FALSE)
				{
					for(int j=0;j<20;j++)
					{
						bExspiration=false;
						bInspiration=false;
						_pBufData.bATZ=false;
						_pBufData.bTrig=false;
						_pBufData.iValPressure=0;
						_pBufData.iValFlow=0;
						_pBufData.iValVolume=0;
						_pBufData.iValCO2=0;
						_pBufData.iValSPO2=0;
						if(bBackup)
						{
							bBackup=false;
							if(AfxGetApp())
								AfxGetApp()->GetMainWnd()->PostMessage(WM_DEL_BACKUP);
						}

						getModel()->getDATAHANDLER()->writeDataBuffer(_pBufData);

					}

					g_eventGraphData.SetEvent();
					continue;
				}

				if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
				{
					bHFOmode=true;
				}
				else
				{
					if(		getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
						&&	getModel()->getDATAHANDLER()->IsHFOrunning())
					{
						bHFOmode=true;
					}
					else
					{
						bHFOmode=false;
					}
				}

#ifdef SIMULATION_ENTREK
				EnterCriticalSection(&csSPIFile);
				bufSize=SimulateNSPIblock(byBuffer);
				LeaveCriticalSection(&csSPIFile);
				

				{	
					SHORT iMinPressureVal=1500;
					SHORT iMaxPressureVal=0;
					bool bResetPminAlarm=false;
					
					// ein Block besteht aus 7 Bytes: Status-Byte,LowByte Pressure,HiByte Pressure,LowByte FLow,HiByte Flow,LowByte Volume,HiByte Volume,
					// max 30 Blöcke
					// -> max 210 Bytes
					for(UINT i=0;i<bufSize;/*i++*/)
					{
						//bExspirationData=false;
						bMonitorData=false;
						bAlarmData=false;
						bFlowsensorData=false;

						//Status-Byte
						byStatus=byBuffer[i];
						i++;

						if(byStatus & INSPIRATIONBIT)//new Breath und triggered Breath, BIT 0 und 1
						{
							if(bInspiration==false)
							{
								if(isSafeTickCountDelayExpired(dwLastInspirationBit, 100))
								{
									dwLastInspirationBit=GetTickCount();
									bInspiration=true;
									_pBufData.bATZ=true;
									if(m_bMechanicalATZ==false && bBackup)
									{
										bBackup=false;
										if(	getModel()->getCONFIG()->GetCurMode()==VM_CPAP
											||
											(getModel()->getCONFIG()->CurModeIsPresetMode() && getModel()->getCONFIG()->GetPrevMode()==VM_CPAP))
										{
											if(AfxGetApp())
												AfxGetApp()->GetMainWnd()->PostMessage(WM_DEL_BACKUP);
										}
									}
								}
							}
							else
							{
								_pBufData.bATZ=false;
							}
						}
						else
						{
							if(bInspiration)
							{
								if(isSafeTickCountDelayExpired(dwLastExspirationData, 200))
								{
									dwLastExspirationData=GetTickCount();
									Read_ATZ_P_Peak();
									g_eventExspiration.SetEvent();
								}
							}
							bInspiration=false;
							_pBufData.bATZ=false;
						}
						
						if(byStatus & TRIGGEREDATZBIT)//triggered Breath
						{
							if(bTriggeredBreath==false && isSafeTickCountDelayExpired(dwLastTriggerATZbit,SPI_DELAY))
							{
								dwLastTriggerATZbit=GetTickCount();
								if(AfxGetApp())
									AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGERED_BREATH);
							}
							_pBufData.bTrig=true;
							bTriggeredBreath=true;
						}
						else
						{
							bTriggeredBreath=false;
							_pBufData.bTrig=false;
						}

						if(byStatus & ALARMCHANGEDBIT)//Alarm status changed
						{
							if(isSafeTickCountDelayExpired(dwLastAlarmchangebit, SPI_DELAY))
							{
								dwLastAlarmchangebit=GetTickCount();
								m_bAlarmData=true;
								bCheckAlarmchangeTimeout=false;
							}
							else
							{
								bCheckAlarmchangeTimeout=true;
							}
						}
						else if(bCheckAlarmchangeTimeout && isSafeTickCountDelayExpired(dwLastAlarmchangebit, SPI_DELAY))
						{
							dwLastAlarmchangebit=GetTickCount();
							bCheckAlarmchangeTimeout=false;
							m_bAlarmData=true;
						}

						if(byStatus & FLOWSENSCHANGEDBIT)//Alarm status changed
						{
							if(isSafeTickCountDelayExpired(dwLastNewFlowsensorData, SPI_DELAY))
							{
								dwLastNewFlowsensorData=GetTickCount();
								m_bFlowsensorData=true;
								bCheckFlowsensDataTimeout=false;
							}
							else
							{
								bCheckFlowsensDataTimeout=true;
							}
						}
						else if(bCheckFlowsensDataTimeout && isSafeTickCountDelayExpired(dwLastNewFlowsensorData, SPI_DELAY))
						{
							dwLastNewFlowsensorData=GetTickCount();
							bCheckFlowsensDataTimeout=false;
							m_bFlowsensorData=true;
						}

						if(byStatus & MECHATZBIT)//
						{
							bBackup=true;

							if(		m_bMechanicalATZ==false
								/*&&	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON*/)
							{
								if(	getModel()->getCONFIG()->GetCurMode()==VM_CPAP
									||
									(getModel()->getCONFIG()->CurModeIsPresetMode() && getModel()->getCONFIG()->GetPrevMode()==VM_CPAP))
								{
									if(AfxGetApp())
										AfxGetApp()->GetMainWnd()->PostMessage(WM_NEW_MECHATZ);
								}
							}
							if(m_bMechanicalATZ==false)
							{
								//DEBUGMSG(TRUE, (TEXT("MechATZ started\r\n")));
								m_bMechanicalATZ=true;
								m_dwWaittimeATZ=0;

								if(m_bLowPressureAlarmFlag)
								{
									m_bLowPressureAlarmFlag=false;
									//DEBUGMSG(TRUE, (TEXT("RESET Flag MechATZ\r\n")));
								}
								else
								{
									bResetPminAlarm=true;

									if(m_bLowPressureAlarmPending)
									{
										//DEBUGMSG(TRUE, (TEXT("RESET delay MechATZ\r\n")));
										m_bLowPressureAlarmPending=false;
										m_dwWaittimeDelayPEEP_UG=0;
									}
								}
							}
						}
						else
						{
							if(m_bMechanicalATZ)
							{
								//DEBUGMSG(TRUE, (TEXT("MechATZ finished\r\n")));
								m_dwWaittimeATZ=GetTickCount();
								m_bMechanicalATZ=false;
							}
						}

						if(byStatus & AUTOFLOWBIT)//
						{
							if(bAutoflowBit==false)
							{
								if(AfxGetApp())
									AfxGetApp()->GetMainWnd()->PostMessage(WM_CHECK_AUTOFLOW);
							}
							bAutoflowBit=true;
						}
						else
						{
							if(bAutoflowBit)
								bAutoflowBit=false;
						}

						byLoProx=byBuffer[i];
						i++;
						byHiProx=byBuffer[i];
						i++;
						iRes=MAKEWORD(byLoProx,byHiProx);
						_pBufData.iValPressure=iRes;

						iCountValues++;
						iPressureAv+=iRes;

						if(_pBufData.iValPressure<iMinPressureVal)
							iMinPressureVal=_pBufData.iValPressure;
						else if(_pBufData.iValPressure>iMaxPressureVal)
							iMaxPressureVal=_pBufData.iValPressure;

						byLoFlow=byBuffer[i];
						i++;
						byHiFlow=byBuffer[i];
						i++;
						iRes=MAKEWORD(byLoFlow,byHiFlow);
						

						_pBufData.iValFlow=iRes;	//Achtung, tatsächlicher Meß-Wert ist um Faktor 2 größer
						iFlowAv+=iRes;

						byLoVolume=byBuffer[i];
						i++;
						byHiVolume=byBuffer[i];
						i++;
						iRes=MAKEWORD(byLoVolume,byHiVolume);
						_pBufData.iValVolume=iRes;
						iVolumeAv+=iRes;
						
						if(getModel()->getFOTThread())
						{
							if(getModel()->getFOTThread()->isAcquireFOTData())
							{
								if(iCntFOTdata>=FOT_SAMPLE_DATA)//250Hz 
								{
									getModel()->getFOTThread()->writeFOTventdataBuffer(_pBufData.iValPressure/10, ((double)_pBufData.iValFlow)/30000);////Achtung, tatsächlicher Meß-Wert ist um Faktor 2 größer, + transfer to LITER/sec

									iCntFOTdata=0;
								}
								iCntFOTdata++;
							}
						}
						
						
						if(getModel()->getCONFIG()->getCO2module()!=CO2MODULE_NONE)//MicroPod: 20hz == every 50ms, Capnostat: 100hz == every 10ms
						{
							if(getModel()->getETCO2())
							{
								EnterCriticalSection(&getModel()->getDATAHANDLER()->csCO2DataBuffer);
								UINT iCurSizeOfPlotItemsCO2=getModel()->getDATAHANDLER()->m_rbufCO2.getCount();
								if(iCurSizeOfPlotItemsCO2>0)
								{
									m_iLastValCO2=getModel()->getDATAHANDLER()->m_rbufCO2.read();

									if(iCurSizeOfPlotItemsCO2>500)
									{
										//DEBUGMSG(TRUE, (TEXT("wave 500\r\n")));
										m_iLastValCO2=getModel()->getDATAHANDLER()->m_rbufCO2.read();
									}
								}
								LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCO2DataBuffer);

								_pBufData.iValCO2=m_iLastValCO2;
							}
						}

						if(getModel()->getCONFIG()->getSPO2module()==SPO2MODULE_MASIMO)	//62,5 hz = alle 16ms ein Wert
						{
							if(getModel()->getSPO2())
							{
								EnterCriticalSection(&getModel()->getDATAHANDLER()->csSPO2DataBuffer);
								if(getModel()->getDATAHANDLER()->m_rbufSPO2.getCount()>0)
								{
									_pSPO2BufData=getModel()->getDATAHANDLER()->m_rbufSPO2.read();
									m_iLastValSPO2=_pSPO2BufData.iValSPO2;
								}
								LeaveCriticalSection(&getModel()->getDATAHANDLER()->csSPO2DataBuffer);

								_pBufData.iValSPO2=m_iLastValSPO2;
							}
						}

						if(getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_IVOI
								||getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_WAVE
								|| getModel()->getCONFIG()->GetPDMSprotocol()==ACL_ETHERNET_WAVE)
						{
							if(getModel()->getAcuLink())
							{
								if(iCntAcuLinkData>=8)//125Hz
								{
									iCntAcuLinkData=0;

									SHORT tempFlow=_pBufData.iValFlow;
									if(tempFlow>16000)
										tempFlow=16000;
									else if(tempFlow<-16000)
										tempFlow=-16000;
									if(getModel()->getCONFIG()->GetAcuLinkVersion()==ALINKVERS_3)
									{
										getModel()->getAcuLink()->setGraphDataV3(_pBufData.iValVolume,_pBufData.iValPressure,tempFlow*ALINK_FACTOR_FLOW,_pBufData.iValCO2);
									}
									else
									{
										getModel()->getAcuLink()->setGraphDataV4(_pBufData.iValVolume,_pBufData.iValPressure,tempFlow*ALINK_FACTOR_FLOW,_pBufData.iValCO2,_pBufData.iValSPO2);
									}	
								}
								iCntAcuLinkData++;
							}
							
						}
						else if(getModel()->getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL)
						{
							if(getModel()->getTERMINAL())
							{
								if(getModel()->getTERMINAL()->isWaveDataActive())
								{
									if(iCntTerminal>=4)//250Hz 
									{
										getModel()->getTERMINAL()->sendWaveData(_pBufData.iValPressure,_pBufData.iValFlow*ALINK_FACTOR_FLOW,_pBufData.iValCO2);
										iCntTerminal=0;
									}
									iCntTerminal++;
								}
							}
						}
						
						//write data into data buffer
						getModel()->getDATAHANDLER()->writeDataBuffer(_pBufData);

						if(getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SYSTEM_SILENT)
						{
							if(m_bAlarmData || bRetryReadStatus)//Alarm status changed
							{
								if(getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SILENT)
								{
									//wenn AlarmStateChanged-Flag -> Alarm-Daten holen
									int iAlarmState=Read_STATUS();
									if(iAlarmState>=0)
									{
										getModel()->getDATAHANDLER()->SetAlarmStateBytes(iAlarmState);
										bAlarmData=true;
										bRetryReadStatus=false;
									}
									else
									{
										bAlarmData=false;
										bRetryReadStatus=true;
									}
								}
							}

							if(m_bFlowsensorData || bRetryReadStatusFlowsensor)//flow sensor status changed
							{
								int iState=Read_STATUS_FLOWSENSOR();
								if(iState>=0)
								{
									getModel()->getDATAHANDLER()->SetFlowsensorStateBytes(iState);
									bFlowsensorData=true;
									bRetryReadStatusFlowsensor=false;
								}
								else
								{
									bFlowsensorData=false;
									bRetryReadStatusFlowsensor=true;
								}
							}
						}


						if(_pBufData.bATZ==true)
						{
							int iRes=0;
							if(isSafeTickCountDelayExpired(dwLastGetMessureData, 200))
							{
								iRes=GetMessureData();
								//ullLastGetMessureData=getTickCount64();
								dwLastGetMessureData=GetTickCount();
							}

							if(iRes!=0 && m_bShowReadSPIDataError)
							{
								if(iRes==888)
								{
									theApp.getLog()->WriteLine(_T("#ERROR: GetMessureDataError888"));
								}
								else if(iRes==999)
								{
									theApp.getLog()->WriteLine(_T("#ERROR: GetMessureDataError999"));
								}
								m_bShowReadSPIDataError=false;
							}

							if(isSafeTickCountDelayExpired(dwLastMonitorData, TIMEOUT_MONITORDATA))
							{
								bMonitorData=true;
								dwLastMonitorData=GetTickCount();
							}

							if(getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
							{
								if(AfxGetApp())
									AfxGetApp()->GetMainWnd()->PostMessage(WM_NEW_BREATH);
							}
						}
						else if(isSafeTickCountDelayExpired(dwLastGetMessureData, TIMEOUT_MONITORDATA))
						{
							//DEBUGMSG(TRUE, (TEXT("GetMessureData time\r\n")));
							int iRes=0;
							if(isSafeTickCountDelayExpired(dwLastGetMessureData, 200))
							{
								iRes=GetMessureData();
								//ullLastGetMessureData=getTickCount64();
								dwLastGetMessureData=GetTickCount();
							}

							if(iRes!=0 && m_bShowReadSPIDataError)
							{
								if(iRes==888)
								{
									theApp.getLog()->WriteLine(_T("#ERROR: GetMessureDataError888"));
								}
								else if(iRes==999)
								{
									theApp.getLog()->WriteLine(_T("#ERROR: GetMessureDataError999"));
								}
								m_bShowReadSPIDataError=false;
							}

							bMonitorData=true;
							dwLastMonitorData=GetTickCount();
						}

						if(bMonitorData)
							g_eventNewMonitorData.SetEvent();

						if(bAlarmData)
							g_eventNewAlarmData.SetEvent();

						if(bFlowsensorData)
							g_eventNewFlowsensorData.SetEvent();
					}

					if(iCountValues>0)
					{
						getModel()->getDATAHANDLER()->setMessureDataPRESSURE(iPressureAv/iCountValues);
						getModel()->getDATAHANDLER()->setMessureDataFLOW(iFlowAv/iCountValues);
						getModel()->getDATAHANDLER()->setMessureDataVOLUME(iVolumeAv/iCountValues);
					}

					iPressureAv=0;
					iFlowAv=0;
					iVolumeAv=0;
					iCountValues=0;


					g_eventGraphData.SetEvent();

					
					//checking pressure alarm limits
					if(		getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SYSTEM_SILENT
						&&	getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SILENT)
					{
						eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();

						if(eActiveVentMode==VM_CPAP || eActiveVentMode==VM_NCPAP)
						{
							checkUpperPressureLimit(eActiveVentMode, iMaxPressureVal);
						}

						if(		eActiveVentMode==VM_CPAP
							||	eActiveVentMode==VM_NCPAP
							||	eActiveVentMode==VM_DUOPAP)
						{
							if(bResetPminAlarm && getModel()->getALARMHANDLER()->ALARM_PatAl_PEEPminLow->getAlarmState()==AS_ACTIVE)
							{
								if(AfxGetApp())
									AfxGetApp()->GetMainWnd()->PostMessage(WM_SETSIGNALEDALARM_Pmin);
							}

							if(m_bMechanicalATZ==false)
								checkLowerPressureLimit(eActiveVentMode, iMinPressureVal);
						}
					}
				}
#else

				EnterCriticalSection(&csSPIFile);
				//PVF-Daten holen, Block
				command[0] = M_PFV_DATASET_BLOCK;	//M_CHECKSUM *2 +0 (read) = 18;

				dwResult=getModel()->getDIO()->WritePin (CDPINBIT);
				if(dwResult==0)
				{
					if(iCountDIOERROR>=5)
					{
						iCountDIOERROR=0;
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_DIO);
						continue;
					}
					else
						iCountDIOERROR++;
					LeaveCriticalSection(&csSPIFile);
					continue;
				}

				//#############################################################
				//send  block command 
				if(!DeviceIoControl(m_hSPIFile, IOCTL_NSPI_SEND, command, 1,NULL, 0, NULL, NULL))
				{
					if(iCountSPIERROR>=5)
					{
						if(getModel()->isSPIavailabel())
						{
							getModel()->setSPIavailability(FALSE);
							getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_SENDCOMMAND);
							if(AfxGetApp())
								AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
						}
						iCountSPIERROR=0;
					}
					iCountSPIERROR++;

					dwResult=getModel()->getDIO()->ClearPin (CDPINBIT);
					if(dwResult==0)
					{
						if(iCountDIOERROR>=5)
						{
							iCountDIOERROR=0;
							if(AfxGetApp())
								AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_DIO);
						}
						else
							iCountDIOERROR++;
					}
					LeaveCriticalSection(&csSPIFile);
					continue;
				}

				dwResult=getModel()->getDIO()->ClearPin (CDPINBIT);
				if(dwResult==0)
				{
					if(iCountDIOERROR>=5)
					{
						iCountDIOERROR=0;
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_DIO);
						continue;
					}
					else
						iCountDIOERROR++;
					LeaveCriticalSection(&csSPIFile);
					continue;
				}

				//#############################################################
				//-> read blocks

				bufSize=ReadNSPIPFVblock(byBuffer);
				LeaveCriticalSection(&csSPIFile);

				if(bufSize>MAXPFVBLOCKBUFSIZE)
				{
					if(iCountSPIERROR>=5)
					{
						iCountSPIERROR=0;
						if(getModel()->isSPIavailabel())
						{
							getModel()->setSPIavailability(FALSE);
							getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_MAXPFVBLOCKBUF);
							if(AfxGetApp())
								AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
						}
					}
					iCountSPIERROR++;

					for(int j=0;j<20;j++)
					{
						bExspiration=false;
						bInspiration=false;
						_pBufData.bATZ=false;
						_pBufData.bTrig=false;
						_pBufData.iValPressure=0;
						_pBufData.iValFlow=0;
						_pBufData.iValVolume=0;
						_pBufData.iValCO2=0;
						_pBufData.iValSPO2=0;
						if(bBackup)
						{
							bBackup=false;
							if(AfxGetApp())
								AfxGetApp()->GetMainWnd()->PostMessage(WM_DEL_BACKUP);
						}
						getModel()->getDATAHANDLER()->writeDataBuffer(_pBufData);
					}
					g_eventGraphData.SetEvent();
				}
				else if(bufSize==0)
				{
					//rku, NewThreading
					if(iCountSPIERROR>=5)
					{
						if(getModel()->isSPIavailabel())
						{
							getModel()->setSPIavailability(FALSE);
							getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_NULLBLOCKBUF);
							if(AfxGetApp())
								AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
						}
						iCountSPIERROR=0;

					}
					iCountSPIERROR++;

					for(int j=0;j<20;j++)
					{
						bExspiration=false;
						bInspiration=false;
						_pBufData.bATZ=false;
						_pBufData.bTrig=false;
						_pBufData.iValPressure=0;
						_pBufData.iValFlow=0;
						_pBufData.iValVolume=0;
						_pBufData.iValCO2=0;
						_pBufData.iValSPO2=0;
						bBackup=false;
						getModel()->getDATAHANDLER()->writeDataBuffer(_pBufData);
					}
					g_eventGraphData.SetEvent();
				}
				else
				{	
					if(iCountSPIERROR>0)
						iCountSPIERROR=0;
					if(iCountDIOERROR>0)
						iCountDIOERROR=0;
					
					SHORT iMinPressureVal=1500;
					SHORT iMaxPressureVal=0;
					bool bResetPminAlarm=false;
					
					// ein Block besteht aus 7 Bytes: Status-Byte,LowByte Pressure,HiByte Pressure,LowByte FLow,HiByte Flow,LowByte Volume,HiByte Volume,
					// max 30 Blöcke
					// -> max 210 Bytes
					for(UINT i=0;i<bufSize;/*i++*/)
					{
						//bExspirationData=false;
						bMonitorData=false;
						bAlarmData=false;
						bFlowsensorData=false;

						//Status-Byte
						byStatus=byBuffer[i];
						i++;

						if(byStatus & INSPIRATIONBIT)//new Breath und triggered Breath, BIT 0 und 1
						{
							if(bInspiration==false)
							{
								if(isSafeTickCountDelayExpired(dwLastInspirationBit, 100))
								{
									dwLastInspirationBit=GetTickCount();
									bInspiration=true;
									_pBufData.bATZ=true;
									if(m_bMechanicalATZ==false && bBackup)
									{
										bBackup=false;
										if(	getModel()->getCONFIG()->GetCurMode()==VM_CPAP
											||
											(getModel()->getCONFIG()->CurModeIsPresetMode() && getModel()->getCONFIG()->GetPrevMode()==VM_CPAP))
										{
											if(AfxGetApp())
												AfxGetApp()->GetMainWnd()->PostMessage(WM_DEL_BACKUP);
										}
									}
								}
							}
							else
							{
								_pBufData.bATZ=false;
							}
						}
						else
						{
							if(bInspiration)
							{
								if(isSafeTickCountDelayExpired(dwLastExspirationData, 200))
								{
									dwLastExspirationData=GetTickCount();
									Read_ATZ_P_Peak();
									g_eventExspiration.SetEvent();
								}
							}
							bInspiration=false;
							_pBufData.bATZ=false;
						}
						//Sicherung wenn HFO Aufgrund hohem Volumen einrastet und hängt
						/*if(bHFOmode && (ullLastInspirationBit+700<getTickCount64()))
						{
							ullLastInspirationBit=getTickCount64();
							bInspiration=true;
							_pBufData.bATZ=true;
							if(bBackup)
							{
								bBackup=false;
								if(AfxGetApp())
									AfxGetApp()->GetMainWnd()->PostMessage(WM_DEL_BACKUP);
							}
						}*/
						
						if(byStatus & TRIGGEREDATZBIT)//triggered Breath
						{
							if(bTriggeredBreath==false && isSafeTickCountDelayExpired(dwLastTriggerATZbit,SPI_DELAY))
							{
								dwLastTriggerATZbit=GetTickCount();
								if(AfxGetApp())
									AfxGetApp()->GetMainWnd()->PostMessage(WM_TRIGGERED_BREATH);
							}
							_pBufData.bTrig=true;
							bTriggeredBreath=true;
						}
						else
						{
							bTriggeredBreath=false;
							_pBufData.bTrig=false;
						}

						if(byStatus & ALARMCHANGEDBIT)//Alarm status changed
						{
							if(isSafeTickCountDelayExpired(dwLastAlarmchangebit, SPI_DELAY))
							{
								dwLastAlarmchangebit=GetTickCount();
								m_bAlarmData=true;
								bCheckAlarmchangeTimeout=false;
							}
							else
							{
								bCheckAlarmchangeTimeout=true;
							}
						}
						else if(bCheckAlarmchangeTimeout && isSafeTickCountDelayExpired(dwLastAlarmchangebit, SPI_DELAY))
						{
							dwLastAlarmchangebit=GetTickCount();
							bCheckAlarmchangeTimeout=false;
							m_bAlarmData=true;
						}

						if(byStatus & FLOWSENSCHANGEDBIT)//Alarm status changed
						{
							if(isSafeTickCountDelayExpired(dwLastNewFlowsensorData, SPI_DELAY))
							{
								dwLastNewFlowsensorData=GetTickCount();
								m_bFlowsensorData=true;
								bCheckFlowsensDataTimeout=false;
							}
							else
							{
								bCheckFlowsensDataTimeout=true;
							}
						}
						else if(bCheckFlowsensDataTimeout && isSafeTickCountDelayExpired(dwLastNewFlowsensorData, SPI_DELAY))
						{
							dwLastNewFlowsensorData=GetTickCount();
							bCheckFlowsensDataTimeout=false;
							m_bFlowsensorData=true;
						}

						if(byStatus & MECHATZBIT)//
						{
							bBackup=true;

							if(		m_bMechanicalATZ==false
								/*&&	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON*/)
							{
								if(	getModel()->getCONFIG()->GetCurMode()==VM_CPAP
									||
									(getModel()->getCONFIG()->CurModeIsPresetMode() && getModel()->getCONFIG()->GetPrevMode()==VM_CPAP))
								{
									if(AfxGetApp())
										AfxGetApp()->GetMainWnd()->PostMessage(WM_NEW_MECHATZ);
								}
							}
							if(m_bMechanicalATZ==false)
							{
								//DEBUGMSG(TRUE, (TEXT("MechATZ started\r\n")));
								m_bMechanicalATZ=true;
								m_dwWaittimeATZ=0;

								if(m_bLowPressureAlarmFlag)
								{
									m_bLowPressureAlarmFlag=false;
									//DEBUGMSG(TRUE, (TEXT("RESET Flag MechATZ\r\n")));
								}
								else
								{
									bResetPminAlarm=true;

									if(m_bLowPressureAlarmPending)
									{
										//DEBUGMSG(TRUE, (TEXT("RESET delay MechATZ\r\n")));
										m_bLowPressureAlarmPending=false;
										m_dwWaittimeDelayPEEP_UG=0;
									}
								}
							}
						}
						else
						{
							if(m_bMechanicalATZ)
							{
								//DEBUGMSG(TRUE, (TEXT("MechATZ finished\r\n")));
								m_dwWaittimeATZ=GetTickCount();
								m_bMechanicalATZ=false;
							}
						}

						if(byStatus & AUTOFLOWBIT)//
						{
							if(bAutoflowBit==false)
							{
								if(AfxGetApp())
									AfxGetApp()->GetMainWnd()->PostMessage(WM_CHECK_AUTOFLOW);
							}
							bAutoflowBit=true;
						}
						else
						{
							if(bAutoflowBit)
								bAutoflowBit=false;
						}

						byLoProx=byBuffer[i];
						i++;
						byHiProx=byBuffer[i];
						i++;
						iRes=MAKEWORD(byLoProx,byHiProx);
						
						//AmplitudeCorrectionFactor
						if(false==getModel()->getDATAHANDLER()->getFOToscillationState() && bHFOmode)
						{
							double iPmitt=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
							double iAmpCorFactor = getModel()->getDATAHANDLER()->getAmpCorFactor(getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara());
							if(iAmpCorFactor==0)
								iAmpCorFactor=1;

							iRes=(SHORT)(((double)iRes-iPmitt)*iAmpCorFactor)+iPmitt;
						}
						/*else if(bHFOmode)
						{
							double iPmitt=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
							double iAmpCorFactor = getModel()->getDATAHANDLER()->getAmpCorFactor(getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara());
							if(iAmpCorFactor==0)
								iAmpCorFactor=1;

							iRes=(SHORT)(((double)iRes-iPmitt)*iAmpCorFactor)+iPmitt;
						}*/
						
						_pBufData.iValPressure=iRes;

						iCountValues++;
						iPressureAv+=iRes;

						if(_pBufData.iValPressure<iMinPressureVal)
							iMinPressureVal=_pBufData.iValPressure;
						else if(_pBufData.iValPressure>iMaxPressureVal)
							iMaxPressureVal=_pBufData.iValPressure;

						byLoFlow=byBuffer[i];
						i++;
						byHiFlow=byBuffer[i];
						i++;
						iRes=MAKEWORD(byLoFlow,byHiFlow);
						/*short sTest=MAKEWORD(byLoFlow,byHiFlow);
						int iTest=MAKEWORD(byLoFlow,byHiFlow);*/

						//iRes=-30000;
						_pBufData.iValFlow=iRes;	//Achtung, tatsächlicher Meß-Wert ist um Faktor 2 größer
						iFlowAv+=iRes;

						byLoVolume=byBuffer[i];
						i++;
						byHiVolume=byBuffer[i];
						i++;
						iRes=MAKEWORD(byLoVolume,byHiVolume);
						_pBufData.iValVolume=iRes;
						iVolumeAv+=iRes;
						
						if(getModel()->getFOTThread())
						{
							if(getModel()->getFOTThread()->isAcquireFOTData())
							{
								if(iCntFOTdata>=FOT_SAMPLE_DATA)//250Hz 
								{
									getModel()->getFOTThread()->writeFOTventdataBuffer(_pBufData.iValPressure/10, ((double)_pBufData.iValFlow)/30000);////Achtung, tatsächlicher Meß-Wert ist um Faktor 2 größer, + transfer to LITER/sec

									iCntFOTdata=0;
								}
								iCntFOTdata++;
							}
						}
						
						
						if(getModel()->getCONFIG()->getCO2module()!=CO2MODULE_NONE)//MicroPod: 20hz == every 50ms, Capnostat: 100hz == every 10ms
						{
							if(getModel()->getETCO2())
							{
								EnterCriticalSection(&getModel()->getDATAHANDLER()->csCO2DataBuffer);
								UINT iCurSizeOfPlotItemsCO2=getModel()->getDATAHANDLER()->m_rbufCO2.getCount();
								if(iCurSizeOfPlotItemsCO2>0)
								{
									m_iLastValCO2=getModel()->getDATAHANDLER()->m_rbufCO2.read();

									if(iCurSizeOfPlotItemsCO2>500)
									{
										//DEBUGMSG(TRUE, (TEXT("wave 500\r\n")));
										m_iLastValCO2=getModel()->getDATAHANDLER()->m_rbufCO2.read();
									}
								}
								LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCO2DataBuffer);

								_pBufData.iValCO2=m_iLastValCO2;
							}
						}

						if(getModel()->getCONFIG()->getSPO2module()==SPO2MODULE_MASIMO)	//62,5 hz = alle 16ms ein Wert
						{
							if(getModel()->getSPO2())
							{
								EnterCriticalSection(&getModel()->getDATAHANDLER()->csSPO2DataBuffer);
								if(getModel()->getDATAHANDLER()->m_rbufSPO2.getCount()>0)
								{
									_pSPO2BufData=getModel()->getDATAHANDLER()->m_rbufSPO2.read();
									m_iLastValSPO2=_pSPO2BufData.iValSPO2;
								}
								LeaveCriticalSection(&getModel()->getDATAHANDLER()->csSPO2DataBuffer);

								_pBufData.iValSPO2=m_iLastValSPO2;
							}
						}

						if(getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_IVOI
								||getModel()->getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_WAVE
								|| getModel()->getCONFIG()->GetPDMSprotocol()==ACL_ETHERNET_WAVE)
						{
							if(getModel()->getAcuLink())
							{
								if(iCntAcuLinkData>=8)//125Hz
								{
									iCntAcuLinkData=0;

									SHORT tempFlow=_pBufData.iValFlow;
									if(tempFlow>16000)
										tempFlow=16000;
									else if(tempFlow<-16000)
										tempFlow=-16000;
									if(getModel()->getCONFIG()->GetAcuLinkVersion()==ALINKVERS_3)
									{
										getModel()->getAcuLink()->setGraphDataV3(_pBufData.iValVolume,_pBufData.iValPressure,tempFlow*ALINK_FACTOR_FLOW,_pBufData.iValCO2);
									}
									else
									{
										getModel()->getAcuLink()->setGraphDataV4(_pBufData.iValVolume,_pBufData.iValPressure,tempFlow*ALINK_FACTOR_FLOW,_pBufData.iValCO2,_pBufData.iValSPO2);
									}	
								}
								iCntAcuLinkData++;
							}
							
						}
						else if(getModel()->getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL)
						{
							if(getModel()->getTERMINAL())
							{
								if(getModel()->getTERMINAL()->isWaveDataActive())
								{
									if(iCntTerminal>=4)//250Hz 
									{
										getModel()->getTERMINAL()->sendWaveData(_pBufData.iValPressure,_pBufData.iValFlow*ALINK_FACTOR_FLOW,_pBufData.iValCO2);
										iCntTerminal=0;
									}
									iCntTerminal++;
								}
							}
						}
						
						//write data into data buffer
						getModel()->getDATAHANDLER()->writeDataBuffer(_pBufData);

						if(getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SYSTEM_SILENT)
						{
							if(m_bAlarmData || bRetryReadStatus)//Alarm status changed
							{
								if(getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SILENT)
								{
									//wenn AlarmStateChanged-Flag -> Alarm-Daten holen
									int iAlarmState=Read_STATUS();
									if(iAlarmState>=0)
									{
										getModel()->getDATAHANDLER()->SetAlarmStateBytes(iAlarmState);
										bAlarmData=true;
										bRetryReadStatus=false;
									}
									else
									{
										bAlarmData=false;
										bRetryReadStatus=true;
									}
								}
							}

							if(m_bFlowsensorData || bRetryReadStatusFlowsensor)//flow sensor status changed
							{
								int iState=Read_STATUS_FLOWSENSOR();
								if(iState>=0)
								{
									getModel()->getDATAHANDLER()->SetFlowsensorStateBytes(iState);
									bFlowsensorData=true;
									bRetryReadStatusFlowsensor=false;
								}
								else
								{
									bFlowsensorData=false;
									bRetryReadStatusFlowsensor=true;
								}
							}
						}

						if(_pBufData.bATZ==true)
						{
							int iRes=0;
							if(isSafeTickCountDelayExpired(dwLastGetMessureData, 200))
							{
								iRes=GetMessureData();
								dwLastGetMessureData=GetTickCount();
							}

							if(iRes!=0 && m_bShowReadSPIDataError)
							{
								if(iRes==888)
								{
									theApp.getLog()->WriteLine(_T("#ERROR: GetMessureDataError888"));
								}
								else if(iRes==999)
								{
									theApp.getLog()->WriteLine(_T("#ERROR: GetMessureDataError999"));
								}
								m_bShowReadSPIDataError=false;
							}

							if(isSafeTickCountDelayExpired(dwLastMonitorData, TIMEOUT_MONITORDATA))
							{
								bMonitorData=true;
								dwLastMonitorData=GetTickCount();
							}

							if(getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
							{
								if(AfxGetApp())
									AfxGetApp()->GetMainWnd()->PostMessage(WM_NEW_BREATH);
							}
						}
						else if(isSafeTickCountDelayExpired(dwLastGetMessureData, TIMEOUT_MONITORDATA))
						{
							//DEBUGMSG(TRUE, (TEXT("GetMessureData time\r\n")));
							int iRes=0;
							if(isSafeTickCountDelayExpired(dwLastGetMessureData, 200))
							{
								iRes=GetMessureData();
								dwLastGetMessureData=GetTickCount();
							}

							if(iRes!=0 && m_bShowReadSPIDataError)
							{
								if(iRes==888)
								{
									theApp.getLog()->WriteLine(_T("#ERROR: GetMessureDataError888"));
								}
								else if(iRes==999)
								{
									theApp.getLog()->WriteLine(_T("#ERROR: GetMessureDataError999"));
								}
								m_bShowReadSPIDataError=false;
							}

							bMonitorData=true;
							dwLastMonitorData=GetTickCount();
						}

						if(bMonitorData)
							g_eventNewMonitorData.SetEvent();

						if(bAlarmData)
							g_eventNewAlarmData.SetEvent();

						if(bFlowsensorData)
							g_eventNewFlowsensorData.SetEvent();
					}

					if(iCountValues>0)
					{
						getModel()->getDATAHANDLER()->setMessureDataPRESSURE(iPressureAv/iCountValues);
						getModel()->getDATAHANDLER()->setMessureDataFLOW(iFlowAv/iCountValues);
						getModel()->getDATAHANDLER()->setMessureDataVOLUME(iVolumeAv/iCountValues);
					}

					iPressureAv=0;
					iFlowAv=0;
					iVolumeAv=0;
					iCountValues=0;

//#ifndef SIMULATION_ENTREK
					g_eventGraphData.SetEvent();
//#endif
					
					//checking pressure alarm limits
					if(		getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SYSTEM_SILENT
						&&	getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SILENT)
					{
						eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();

						if(eActiveVentMode==VM_CPAP || eActiveVentMode==VM_NCPAP)
						{
							checkUpperPressureLimit(eActiveVentMode, iMaxPressureVal);
						}

						if(		eActiveVentMode==VM_CPAP
							||	eActiveVentMode==VM_NCPAP
							||	eActiveVentMode==VM_DUOPAP)
						{
							if(bResetPminAlarm && getModel()->getALARMHANDLER()->ALARM_PatAl_PEEPminLow->getAlarmState()==AS_ACTIVE)
							{
								if(AfxGetApp())
									AfxGetApp()->GetMainWnd()->PostMessage(WM_SETSIGNALEDALARM_Pmin);
							}

							if(m_bMechanicalATZ==false)
								checkLowerPressureLimit(eActiveVentMode, iMinPressureVal);
						}
					}
				}
#endif
			}
			else
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_INVALID_HANDLE_VALUE);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0126"));
				m_bDoSPIMonitorThread=false;	
			}
		}
		
	}while(m_bDoSPIMonitorThread);

	m_bSPIrunning=false;
	theApp.getLog()->WriteLine(_T("#THR:013"));

	return 0;
}

void CInterfaceSPI::ResetSPO2Value()
{
	EnterCriticalSection(&getModel()->getDATAHANDLER()->csSPO2DataBuffer);
	m_iLastValSPO2=0;
	LeaveCriticalSection(&getModel()->getDATAHANDLER()->csSPO2DataBuffer);
}

void CInterfaceSPI::ResetCO2Value()
{
	EnterCriticalSection(&getModel()->getDATAHANDLER()->csCO2DataBuffer);
	m_iLastValCO2=0;
	LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCO2DataBuffer);
}

bool CInterfaceSPI::isMechanicalBreath()
{
	return m_bMechanicalATZ;
}

void CInterfaceSPI::checkUpperPressureLimit(eVentMode eActiveVentMode, SHORT iMaxPressureVal)
{
	eAlarmLimitState statePmax = getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit();

	SHORT iCurrentTemp=0;
	SHORT iAlimitPmaxTemp=0;

	if(statePmax==AL_ON || statePmax==AL_AUTO)
	{
		//iMaxPressureVal=iMaxPressureVal/10;

		bool bCheckHighPEEP=false;
		if(eActiveVentMode==VM_CPAP)
		{
			if(m_dwWaittimeATZ==0 || isSafeTickCountDelayExpired(m_dwWaittimeATZ, BREATH_ALARMDELAY_CPAP))
			{
				bCheckHighPEEP=true;
				m_dwWaittimeATZ=0;
			}
		}
		else// if(eActiveVentMode==VM_NCPAP)
		{
			if(m_dwWaittimeATZ==0 || isSafeTickCountDelayExpired(m_dwWaittimeATZ, BREATH_ALARMDELAY_NCPAP))
			{
				bCheckHighPEEP=true;
				m_dwWaittimeATZ=0;
			}
		}

		if(		bCheckHighPEEP 
			&&	m_bManBreathRunning==false 
			&&	m_bMechanicalATZ==false)
		{
			//check current states of alarmlimit alarms -> update
			eStateOfAlarm eSoA_PmaxLimit=getModel()->getALARMHANDLER()->ALARM_PatAl_PIPmax->getAlarmState();

			//check PEEP OG
			bool bPmax_Alarm=false;
			SHORT iAlimitPmax=getModel()->getALARMHANDLER()->getAlimitPIPmax()/*/10*/;

			iAlimitPmaxTemp=iAlimitPmax;
			if(iMaxPressureVal > iAlimitPmax)
			{
				bPmax_Alarm=true;

				if(m_bHighPressureAlarmPending==false)
				{
					//m_iHighPressureValue=iMaxPressureVal/**10*/;
					m_bHighPressureAlarmPending=true;
					m_dwWaittimeDelayPEEP_OG=GetTickCount();

					//DEBUGMSG(TRUE, (TEXT("start m_dwWaittimeDelayPEEP_OG\r\n")));
				}
				else	//check if pending time has  
				{
					//set AlarmDelay timer upper...... if PEEP+25mbar->0,3sec, else 5sec
					SHORT iCurrentCPAPpara=0;
					if(eActiveVentMode==VM_CPAP)
					{
						iCurrentCPAPpara=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPPara()/*/10*/;
					}
					else// if(eActiveVentMode==VM_NCPAP)
					{
						iCurrentCPAPpara=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEPara()/*/10*/;
					}

					iCurrentTemp=iCurrentCPAPpara;

					SHORT iWaitTime=0;
					/*if(iMaxPressureVal >= 250+iCurrentCPAPpara)
					{
						iWaitTime=300;
					}
					else*/
					{
						iWaitTime=5000;
					}

					if(m_dwWaittimeDelayPEEP_OG==0 || isSafeTickCountDelayExpired(m_dwWaittimeDelayPEEP_OG, 5000))
					{
						m_bHighPressureAlarmPending=false;
						//DEBUGMSG(TRUE, (TEXT("SET bPmax_Alarm\r\n")));
						m_iHighPressureValue=iMaxPressureVal;
					}
				}
				
				
			}
			else if(m_bHighPressureAlarmPending==true) //max pressure is in limit
			{
				//DEBUGMSG(TRUE, (TEXT("RESET m_dwWaittimeDelayPEEP_OG 2\r\n")));
				m_bHighPressureAlarmPending=false;

				//kill alarm delay Timer OG
				m_dwWaittimeDelayPEEP_OG=0;
			}

			CStringW sz = _T("");

			if(bPmax_Alarm && m_bHighPressureAlarmPending==false && eSoA_PmaxLimit!=AS_ACTIVE && getModel()->getALARMHANDLER()->getPrioActiveAlarm() >= AP_7)
			{
				if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
				{
					sz.Format(_T("%s [%0.1f %s]"),getModel()->GetLanguageString(IDS_ALARM_PMAX),CTlsFloat::Round(((double)m_iHighPressureValue)/10, 1),getModel()->GetLanguageString(IDS_UNIT_MBAR));
				}
				else
				{
					sz.Format(_T("%s [%0.1f %s]"),getModel()->GetLanguageString(IDS_ALARM_PMAX),CTlsFloat::Round(((double)m_iHighPressureValue)/10, 1),getModel()->GetLanguageString(IDS_UNIT_CMH2O));
				}

				if(getModel()->IsPatAlarmO2dependendEnabled()) // in case of new O2 setting there could be disruption in pressure signal, so there is a delay of 200ms
				{
					m_dwWaittimeDelayPEEP_OG=0;
					getModel()->getALARMHANDLER()->setPIPmaxAlarmText(sz);
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_PIPmax);
				}
			}
			else if(!bPmax_Alarm && eSoA_PmaxLimit==AS_ACTIVE)
			{
				int iStop=iCurrentTemp;
				iStop=iAlimitPmaxTemp;
				//DEBUGMSG(TRUE, (TEXT("WM_DELALARM_Pmax\r\n")));
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_DELALARM_PIPmax);
			}
		}
		
	}
	else if(statePmax!=AL_ON && statePmax!=AL_AUTO && m_bHighPressureAlarmPending==true)
	{
		m_bHighPressureAlarmPending=false;
		m_dwWaittimeDelayPEEP_OG=0;
	}
}

void CInterfaceSPI::checkLowerPressureLimit(eVentMode eActiveVentMode, SHORT iMinPressureVal)
{
	eAlarmLimitState statePEEPmin = getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit();

	if(statePEEPmin==AL_ON || statePEEPmin==AL_AUTO)
	{
		//check current states of alarmlimit alarms -> update
		eStateOfAlarm eSoA_PEEPminLowLimit=getModel()->getALARMHANDLER()->ALARM_PatAl_PEEPminLow->getAlarmState();

		//check PEEP UG
		bool bPmin_Alarm=false;
		SHORT iAlimitPmin=getModel()->getALARMHANDLER()->getAlimitPEEPmin();

		if(eActiveVentMode==VM_NCPAP || eActiveVentMode==VM_CPAP)
		{
			bool bCheckLowPEEP=false;
			if(m_dwWaittimeATZ==0 || isSafeTickCountDelayExpired(m_dwWaittimeATZ,BREATH_ALARMDELAY_NCPAP))
			{
				bCheckLowPEEP=true;
				m_dwWaittimeATZ=0;
			}

			if(		bCheckLowPEEP 
				&&	m_bManBreathRunning==false 
				&&	m_bMechanicalATZ==false)
			{
				if(iMinPressureVal < iAlimitPmin /*&& m_bInspiration==false*/)
				{
					bPmin_Alarm=true;

					int iPminAlarmDelay=getModel()->getCONFIG()->GetCurPminAlarmDelay()*1000;

					if(m_bLowPressureAlarmPending==false)
					{
						m_bLowPressureAlarmFlag=true;
						m_bLowPressureAlarmPending=true;

						m_dwWaittimeDelayPEEP_UG=GetTickCount();

						m_iLowPressureValue=iMinPressureVal;//*10;
					}
					else if((m_dwWaittimeDelayPEEP_UG>0) && isSafeTickCountDelayExpired(m_dwWaittimeDelayPEEP_UG, iPminAlarmDelay))
					{
						m_bLowPressureAlarmPending=false;
						m_dwWaittimeDelayPEEP_UG=0;
					}
					
				}
				else //if(m_bLowPressureAlarmPending==true) //
				{
					//DEBUGMSG(TRUE, (TEXT("RESET m_dwWaittimeDelayPEEP_UG 2\r\n")));
					if(m_bLowPressureAlarmPending==true)
					{
						m_bLowPressureAlarmPending=false;

						//kill alarm delay Timer OG
						m_dwWaittimeDelayPEEP_UG=0;
					}

					if(!bPmin_Alarm && eSoA_PEEPminLowLimit==AS_ACTIVE)
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETSIGNALEDALARM_Pmin);
					}
				}
			}
		}
		else
		{
			if(iMinPressureVal < iAlimitPmin && m_bMechanicalATZ==false)
			{
				bPmin_Alarm=true;

				m_bLowPressureAlarmFlag=true;
				//DEBUGMSG(TRUE, (TEXT("set flag\r\n")));

				int iPminAlarmDelay=getModel()->getCONFIG()->GetCurPminAlarmDelay()*1000;

				if(m_bLowPressureAlarmPending==false)
				{
					m_bLowPressureAlarmFlag=true;
					m_bLowPressureAlarmPending=true;

					m_dwWaittimeDelayPEEP_UG=GetTickCount();

					m_iLowPressureValue=iMinPressureVal;//*10;
				}
				else if((m_dwWaittimeDelayPEEP_UG>0) && isSafeTickCountDelayExpired(m_dwWaittimeDelayPEEP_UG, iPminAlarmDelay))
				{
					m_bLowPressureAlarmPending=false;
					m_dwWaittimeDelayPEEP_UG=0;
				}
			}
		}
		

		CStringW sz = _T("");

		if(bPmin_Alarm && m_bLowPressureAlarmPending==false && eSoA_PEEPminLowLimit!=AS_ACTIVE && getModel()->getALARMHANDLER()->getPrioActiveAlarm() >= AP_7)
		{
			if(getModel()->IsPatAlarmO2dependendEnabled())
			{
				m_bLowPressureAlarmPending=false;
				m_dwWaittimeDelayPEEP_UG=0;
				getModel()->getALARMHANDLER()->setPEEPminLowAlarmValue(m_iLowPressureValue);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SetAlarm_PminLow);
			}
		}
	}
	else
	{
		m_bLowPressureAlarmFlag=false;
		m_bLowPressureAlarmPending=false;

		m_dwWaittimeDelayPEEP_UG=0;
	}
}

void CInterfaceSPI::resetPressureAlarmDelay()
{
	m_bHighPressureAlarmPending=false;
	m_dwWaittimeDelayPEEP_OG=0;

	m_bLowPressureAlarmFlag=false;
	m_bLowPressureAlarmPending=false;
	m_dwWaittimeDelayPEEP_UG=0;
}



// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::StartSPICommunicationThread( void )
{
	m_bDoSPICommunicationThread=true;
	//m_pcwtSPICommunicationThread=AfxBeginThread(CSPICommunicationThread,this);

	if(m_pcwtSPICommunicationThread!=NULL)
	{
		delete m_pcwtSPICommunicationThread;
		m_pcwtSPICommunicationThread=NULL;

		if(m_hThreadSPICommunication!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSPICommunication);
			m_hThreadSPICommunication=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtSPICommunicationThread=AfxBeginThread(CSPICommunicationThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadSPICommunication=m_pcwtSPICommunicationThread->m_hThread;
	m_pcwtSPICommunicationThread->m_bAutoDelete = FALSE; 
	m_pcwtSPICommunicationThread->ResumeThread();

}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::StopSPICommunicationThread( void )
{
	if(m_bDoSPICommunicationThread)
	{
		m_bDoSPICommunicationThread=false;
		//WaitForSingleObject(m_pcwtSPICommunicationThread->m_hThread,1000);

		if (WaitForSingleObject(m_pcwtSPICommunicationThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:014a"));
			if(!TerminateThread(m_pcwtSPICommunicationThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:014b"));
			}
		}
	}
	
}
// **************************************************************************
// 
// **************************************************************************
static UINT CSPICommunicationThread( LPVOID pc )
{
	try
	{
		((CInterfaceSPI*)pc)->SPICommunicationData();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CSPICommunicationThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CSPICommunicationThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CInterfaceSPI*)pc)->SPICommunicationData();
	return TRUE;
}


// **************************************************************************
// 
// **************************************************************************
DWORD CInterfaceSPI::SPICommunicationData(void) 
{
	CeSetThreadPriority(m_pcwtSPICommunicationThread->m_hThread,130);
	
	BOOL bResult=0;

	/*if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_THR_START,THR_SPISEND);*/

	do
	{
		while(true && m_bDoSPICommunicationThread)
		{
			EnterCriticalSection(&csSPIsendqueue);

			if(SPIsendqueue.IsEmpty())
			{
				LeaveCriticalSection(&csSPIsendqueue);
				Sleep(5);

				//if(iCntThreadWatchdog>=100)//each 500 millisec //rkuNEWFIX
				//{
				//	//DEBUGMSG(TRUE, (TEXT("THR_SPISEND1\r\n")));
				//	/*if(AfxGetApp())
				//		AfxGetApp()->GetMainWnd()->PostMessage(WM_THR_UPDATE,THR_SPISEND);*/
				//	iCntThreadWatchdog=0;
				//}
				//iCntThreadWatchdog++;
				// 
				break;
			}

			SPISENDMESSAGE* message = SPIsendqueue.RemoveHead();

			if(message)
			{
				bResult= WriteNSPIdata(message->byteSPICommand,1,message->iValue,0);

				delete message;
			}

			LeaveCriticalSection(&csSPIsendqueue);

			Sleep(1);

			//if(iCntThreadWatchdog>=50)//1Hz - each second
			//{
			//	//DEBUGMSG(TRUE, (TEXT("THR_SPISEND2\r\n")));
			//	/*if(AfxGetApp())
			//		AfxGetApp()->GetMainWnd()->PostMessage(WM_THR_UPDATE,THR_SPISEND);*/
			//	iCntThreadWatchdog=0;
			//}
			//iCntThreadWatchdog++;
		}

		Sleep(20);
	}while(m_bDoSPICommunicationThread);

	/*if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_THR_STOP,THR_SPISEND);*/

	theApp.getLog()->WriteLine(_T("#THR:014"));

	return 0;
}

// **************************************************************************
// 
// **************************************************************************
//WORD CInterfaceSPI::ReadNSPIdata(BYTE* pCommand,WORD wcommandSize, WORD wOffset)	//monitor data
//{
//	WORD wRes=0;
//
//	if(m_hSPIFile!=INVALID_HANDLE_VALUE)
//	{
//		BOOL bRes=false;
//		DWORD dwResult=0;
//		//BYTE chPin = ACKNPINBIT;
//		//BYTE oldState=0;
//		BYTE bufferLo[1];
//		BYTE bufferHi[1];
//
//
//		//get old acknowledge state
//		//m_oldAckState=m_pcDIO->ReadPin(ACKNPINPORT,ACKNPINBIT);
//		/*dwResult=m_pcDIO->ReadPin(&oldState,ACKNPINPORT,ACKNPINBIT);
//		if(dwResult==0)
//		{
//			ERRORMSG(1,(_T("DIO: Error while read pin data, error %d\n\r"), GetLastError()));
//			return wRes;
//		}*/
//
//		/*dwResult=m_pcDIO->WritePin (&setCommandState,CDPINPORT,CDPINBIT);
//		if(dwResult==0)
//		{
//			ERRORMSG(1,(_T("DIO: Error while write pin data, error %d\n\r"), GetLastError()));
//			return bRes;
//		}*/
//
//
//		//#############################################################
//		//send  M_[message]
//		bRes=DeviceIoControl(m_hSPIFile, IOCTL_NSPI_SEND, pCommand, wcommandSize,NULL, 0, NULL, NULL);
//
//		if(!bRes)
//		{
//			return wRes;
//		}
//		//if(!IsAcknowledge(/*oldState*/))
//		//{
//		//	//kein Acknowledge
//		//	return wRes;
//		//}
//
//		/*dwResult=m_pcDIO->WritePin (&delCommandState,CDPINPORT,CDPINBIT);
//		if(dwResult==0)
//		{
//			ERRORMSG(1,(_T("DIO: Error while write pin data, error %d\n\r"), GetLastError()));
//			return bRes;
//		}*/
//
//		//#############################################################
//		//-> read low
//		//oldState=m_pcDIO->ReadPin(ACKNPINPORT,ACKNPINBIT);
//		//dwResult=m_pcDIO->ReadPin(&oldState,ACKNPINPORT,ACKNPINBIT);
//		bufferLo[0]=0;
//
//		bRes=DeviceIoControl(m_hSPIFile, IOCTL_NSPI_RECEIVE, NULL, 0,bufferLo, 1, NULL, NULL);
//		if(!bRes)
//		{
//			return wRes;
//		}
//		//if(!IsAcknowledge(/*oldState*/))
//		//{
//		//	//kein Acknowledge
//		//	return wRes;
//		//}
//
//		//#############################################################
//		//-> read hi
//		bufferHi[0]=0;
//
//		bRes=DeviceIoControl(m_hSPIFile, IOCTL_NSPI_RECEIVE, NULL, 0,bufferHi, 1, NULL, NULL);
//		if(!bRes)
//		{
//			return wRes;
//		}
//		
//		wRes=MAKEWORD(bufferLo,bufferHi);
//
//	}
//
//	return wRes;
//}


// **************************************************************************
// 
// **************************************************************************
bool CInterfaceSPI::ReadNSPIdata(BYTE* pCommand,WORD wcommandSize, BYTE* pBuffer,WORD wBufSize, WORD wOffset)	//monitor data
{
	bool bResult=false;
	DWORD dwIO=0;

	if(m_hSPIFile!=INVALID_HANDLE_VALUE)
	{

		bResult=true;
		EnterCriticalSection(&csSPIFile);

		dwIO=getModel()->getDIO()->WritePin (CDPINBIT);
		if(dwIO==0)
		{
			theApp.getLog()->WriteLine(_T("#HFO:0127"));
		}

		//#############################################################
		//send  M_[message]
		if(!DeviceIoControl(m_hSPIFile, IOCTL_NSPI_SEND, pCommand, wcommandSize,NULL, 0, NULL, NULL))
		{
			bResult=false;
			theApp.getLog()->WriteLine(_T("#HFO:0129"));
		}

		dwIO=getModel()->getDIO()->ClearPin (CDPINBIT);
		if(dwIO==0)
		{
			theApp.getLog()->WriteLine(_T("#HFO:0128"));
		}

		//#############################################################
		//-> read 
		if(!DeviceIoControl(m_hSPIFile, IOCTL_NSPI_RECEIVE, NULL, 0,pBuffer, wBufSize, NULL, NULL))
		{
			bResult=false;
			theApp.getLog()->WriteLine(_T("#HFO:0130"));
		}

		LeaveCriticalSection(&csSPIFile);

	}
	else if(m_hSPIFile==INVALID_HANDLE_VALUE && getModel()->isSPIavailabel())
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
	}

	return bResult;
}


// **************************************************************************
// 
// **************************************************************************
UINT CInterfaceSPI::ReadNSPIPFVblock(BYTE *pBuffer)	//monitor data;
{
	UINT bufSize=0;

	if(m_hSPIFile!=INVALID_HANDLE_VALUE)
	{
		BYTE bufferBlocksize[1];
		bufferBlocksize[0]=0;


		if(!DeviceIoControl(m_hSPIFile, IOCTL_NSPI_RECEIVE, NULL, 0,bufferBlocksize, 1, NULL, NULL))
		{
			theApp.writeLogError(GetLastError(), _T("SPI: Error while receive block data"));
		}

		// ein Block besteht aus 7 Bytes: Status-Byte,LowByte Pressure,HiByte Pressure,LowByte FLow,HiByte Flow,LowByte Volume,HiByte Volume,
		// max 30 Blöcke
		// -> max 210 Bytes
		
		/*if(m_iTest==0)
		{
			m_testTickCount=GetTickCount();
		}
		m_iTest+=bufferBlocksize[0];

		if(m_testTickCount+30000<GetTickCount())
		{
			m_testTickCount=0;
			DEBUGMSG(TRUE, (TEXT("data %d\r\n"),m_iTest));
			m_iTest=0;
		}*/
		/*if(iTest!=16 && iTest!=24)
		{
			m_iTest=iTest;
			DEBUGMSG(TRUE, (TEXT("%d\r\n"),m_iTest));
		}*/
		if(bufferBlocksize[0]>MAXNUMBLOCKS || bufferBlocksize[0]==1)
			bufferBlocksize[0]=0;
		
		bufSize=bufferBlocksize[0]*7;
		

		if(bufSize>0)
		{
			if(!DeviceIoControl(m_hSPIFile, IOCTL_NSPI_RECEIVE, NULL, 0,pBuffer, bufSize, NULL, NULL))
			{
				//todo
				//LogError(_T("SPI: Error while receive block data"));
			}
		}
		
	}
	else if(m_hSPIFile==INVALID_HANDLE_VALUE && getModel()->isSPIavailabel())
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
	}

	return bufSize;
}

const float PI						=3.1416f;
// **************************************************************************
// 
// **************************************************************************
DWORD CInterfaceSPI::SimulateNSPIblock(BYTE *pBuffer)	//monitor data;
{
	DWORD bufSize=0;
	PBUFSPI _pBufDate;
	bool bNewBreath=false;
	int iCount=0;
	BYTE byStatus=0;
	bool bTriggeredBreath=false;
	SHORT iVal=0;

	//if(m_hSPIFile!=INVALID_HANDLE_VALUE)
	{
		BYTE bufferBlocksize[1];
		bufferBlocksize[0]=0;
//#ifndef MEDKOM_VERSION
//		if(m_hSPIFile!=INVALID_HANDLE_VALUE)
//		{
//			if(!DeviceIoControl(m_hSPIFile, IOCTL_NSPI_RECEIVE, NULL, 0,bufferBlocksize, 1, NULL, NULL))
//			{
//				theApp.writeLogError(GetLastError(), _T("SPI: Error while receive block data#2"));
//				//LogError(_T("SPI: Error while receive block data"));
//			}
//		}
//#else
		m_iTestCntSPIbufSize=20;
//#endif
		



		bufferBlocksize[0]=m_iTestCntSPIbufSize+1;

		if(bufferBlocksize[0]>MAXNUMBLOCKS)
			bufferBlocksize[0]=0;
		
		bufSize=bufferBlocksize[0]*7;

		if(bufSize>MAXPFVBLOCKBUFSIZE)
			bufSize=0;

		int iBufCounter=0;
		iCount++;
		//int iPlus=0;

		//float w=(float)(m_nPhi*sin((PI*nStep)/(DATAPOINTS-1)));

		//for(int i=0; i<nCnt; i++)	
		//{
		//	if(m_nTrg)
		//	{
		//		//g_pData[i].y=i/3;
		//		g_pData[i].y=(LONG)((m_lda.nCy)/2+((2*m_lda.nCy)/5)*sin((w*TWOPIDEC*(float)i)));
		//	}
		//	else
		//	{
		//		//g_pData[i].y=i/3;
		//		g_pData[i].y=(LONG)((m_lda.nCy)/2+((2*m_lda.nCy)/5)*cos((w*TWOPIDEC*(float)i)));
		//	}
		//}	
		//nCnt++;
		//if(nCnt>=DATAPOINTS)
		//	nCnt=1;
		
		//if(!bCosGraph)
		//{
		//	//y = 50;
		//	iYval = (int)(((sin((double)iGraphCount/15))*60)+50);
		//	if(iGraphCount>=538)
		//		bCosGraph = true;
		//}
		//else
		//{
		//	//y = 80;
		//	iYval = (int)(((cos((double)iGraphCount/10))*60)+50);
		//	if(iGraphCount>=538)
		//		bCosGraph = false;
		//}

		for(int i=0; i<bufferBlocksize[0]; i++)
		{
			//if(m_bHighSimulation)
			//{
			//	m_iTempP=300;
			//	m_iTempF=5000;
			//	m_iTempV=400;

			//	/*m_iTempP=m_iTempP+1;
			//	m_iTempF=m_iTempF+10;
			//	if(m_i4Count==0)
			//		m_iTempV=m_iTempV+1;
			//	else if(m_i4Count==1)
			//		m_iTempV=m_iTempV+2;
			//	else if(m_i4Count==2)
			//		m_iTempV=m_iTempV+3;
			//	else if(m_i4Count==3)
			//		m_iTempV=m_iTempV+4;*/
			//}
			//else
			//{
			//	m_iTempP=50;
			//	m_iTempF=-5000;
			//	m_iTempV=100;

			//	//int iT=m_iTempF;

			//	/*m_iTempP=0;
			//	m_iTempF=m_iTempF+10;
			//	m_iTempV=0;*/
			//}

			//Test-Störungen für Autoscale
			/*if(m_iLineCount != 0 && m_iLineCount%40000 == 0)
			{
				m_iTempP=1000;
			}

			if(m_iLineCount != 0 && m_iLineCount%60000 == 0)
			{
				m_iTempF=45000;
			}

			if(m_iLineCount != 0 && m_iLineCount%70000 == 0)
			{
				m_iTempV=3000;
			}*/

			//m_iTempP = (int)(((sin(m_iGraphCount/60))*60)+100);


			//rad = (m_iGraphCount * PI) / 180;

			_pBufDate.bTrig=false;
			_pBufDate.bATZ=false;

			m_iTempP = ((int)(((sin((m_iGraphCount* PI)/180))*120)+150));
			//m_iTempF = (m_iTempP*50)-7000;
			m_iTempV = m_iTempP;

			if(m_iTempF>=50000)
				m_iTempF=(SHORT)-50000;
			m_iTempF=m_iTempF+50;
			
			m_iGraphCount++;
			if(m_iGraphCount>360)
			{
				m_iGraphCount=0;
				//bCosGraph = true;

				m_i4Count++;
			}

			if(m_i4Count>3)
			{
				m_i4Count=0;
				_pBufDate.bATZ=true;
				bNewBreath=true;
			}

			
			_pBufDate.iValPressure=m_iTempP;
			_pBufDate.iValVolume=m_iTempV;
			_pBufDate.iValFlow=m_iTempF;
			

			//m_iLineCount++;
			//if(m_iLineCount>100000)
			//{
			//	m_iLineCount=1;
			//}

			//if(m_iLineCount%300 == 0)
			//{
			//	if(m_bHighSimulation)
			//	{
			//		//m_iTempF=-3000;
			//		m_bHighSimulation=false;
			//		_pBufDate.bATZ=false;
			//	}
			//	else
			//	{
			//		//m_iTempF=0;
			//		m_bHighSimulation=true;

			//		m_i4Count++;
			//		if(m_i4Count>3)
			//			m_i4Count=0;

			//		_pBufDate.bATZ=true;
			//		bNewBreath=true;
			//		m_iBreathcount++;

			//		if(m_iBreathcount>=4)
			//		{
			//			m_iBreathcount=0;
			//			bTriggeredBreath=true;
			//			_pBufDate.bTrig=true;
			//		}
			//		else
			//			bTriggeredBreath=false;

			//		
			//	}
			//}

			//byStatus=0;//rücksetzen

			if(bTriggeredBreath)
			{
				//byStatus=0x3;

				byStatus=byStatus | INSPIRATIONBIT;
				//int iTest=byStatus;
				byStatus=byStatus | TRIGGEREDATZBIT;
			}
			else if(bNewBreath)
			{
				//byStatus=0x1;
				byStatus=byStatus | INSPIRATIONBIT;
			}
			else
			{
				byStatus=0x0;
			}

			pBuffer[iBufCounter]=byStatus;
			
			iBufCounter++;

			iVal=_pBufDate.iValPressure;

			pBuffer[iBufCounter]=LOBYTE(iVal);
			iBufCounter++;
			pBuffer[iBufCounter]=HIBYTE(iVal);
			iBufCounter++;
			
			iVal=_pBufDate.iValFlow;

			pBuffer[iBufCounter]=LOBYTE(iVal);
			iBufCounter++;
			pBuffer[iBufCounter]=HIBYTE(iVal);
			iBufCounter++;

			iVal=_pBufDate.iValVolume;

			pBuffer[iBufCounter]=LOBYTE(iVal);
			iBufCounter++;
			pBuffer[iBufCounter]=HIBYTE(iVal);
			iBufCounter++;
		}
	}

	return bufSize;
}

// **************************************************************************
// 
// **************************************************************************
DWORD CInterfaceSPI::ReadNSPIAVGblock(BYTE *pBuffer,WORD bufSize)	//monitor data;
{
	DWORD dwResult=0;

	if(m_hSPIFile!=INVALID_HANDLE_VALUE)
	{
		EnterCriticalSection(&csSPIFile);

		BYTE command[1];
		command[0] = M_AVG_DATASET_BLOCK;	

		dwResult=getModel()->getDIO()->WritePin (CDPINBIT);
		if(dwResult==0)
		{
			//LogError(_T("DIO: Error while write pin data"));
		}

		//#############################################################
		//send  M_[message]
		if(!DeviceIoControl(m_hSPIFile, IOCTL_NSPI_SEND, command, 1,NULL, 0, NULL, NULL))
		{
			dwResult=0;
			theApp.writeLogError(GetLastError(), _T("SPI: Error while send data"));
			//LogError(_T("SPI: Error while send data"));
		}

		dwResult=getModel()->getDIO()->ClearPin (CDPINBIT);
		if(dwResult==0)
		{
			//LogError(_T("DIO: Error while clear pin data"));
		}

		//#############################################################
		//-> read 
		if(!DeviceIoControl(m_hSPIFile, IOCTL_NSPI_RECEIVE, NULL, 0,pBuffer, bufSize, NULL, NULL))
		{
			dwResult=0;
			theApp.writeLogError(GetLastError(), _T("SPI: Error while receive ATZ block data"));
			//LogError(_T("SPI: Error while receive ATZ block data"));
		}

		LeaveCriticalSection(&csSPIFile);

	}
	else if(m_hSPIFile==INVALID_HANDLE_VALUE && getModel()->isSPIavailabel())
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
	}

	return dwResult;

}

// **************************************************************************
// 
// **************************************************************************
DWORD CInterfaceSPI::ReadNSPIATZblock(BYTE *pBuffer,WORD bufSize)	//monitor data;
{
	DWORD dwResult=0;

	if(m_hSPIFile!=INVALID_HANDLE_VALUE)
	{
		EnterCriticalSection(&csSPIFile);

		BYTE command[1];
		command[0] = M_ATZ_DATASET_BLOCK;	

		dwResult=getModel()->getDIO()->WritePin (CDPINBIT);
		if(dwResult==0)
		{
			//LogError(_T("DIO: Error while write pin data"));
		}

		//#############################################################
		//send  M_[message]
		if(!DeviceIoControl(m_hSPIFile, IOCTL_NSPI_SEND, command, 1,NULL, 0, NULL, NULL))
		{
			dwResult=0;
			theApp.writeLogError(GetLastError(), _T("SPI: Error while send data#2"));
			//LogError(_T("SPI: Error while send data"));
		}

		dwResult=getModel()->getDIO()->ClearPin (CDPINBIT);
		if(dwResult==0)
		{
			theApp.writeLogError(GetLastError(), _T("DIO: Error while clear pin data"));
			//LogError(_T("DIO: Error while clear pin data"));
		}

		//#############################################################
		//-> read 
		if(!DeviceIoControl(m_hSPIFile, IOCTL_NSPI_RECEIVE, NULL, 0,pBuffer, bufSize, NULL, NULL))
		{
			dwResult=0;
			theApp.writeLogError(GetLastError(), _T("SPI: Error while receive ATZ block data"));
			//LogError(_T("SPI: Error while receive ATZ block data"));
		}

	

		LeaveCriticalSection(&csSPIFile);

	}
	else if(m_hSPIFile==INVALID_HANDLE_VALUE && getModel()->isSPIavailabel())
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
	}

	return dwResult;

}

// **************************************************************************
// 
// **************************************************************************
//BYTE CInterfaceSPI::setBitOfByte(BYTE x, unsigned int n)
//{
//	x = x | (1 << n);
//
//	return x;
//}
//
//WORD CInterfaceSPI::setBitOfWord(WORD x, unsigned int n)
//{
//	x = x | (1 << n);
//
//	return x;
//}
//
//int CInterfaceSPI::SetBit(int x, unsigned int n)
//{
//	//x = x | (1 << n);
//	x = x | n;
//
//	return x;
//}

// **************************************************************************
// 
// **************************************************************************
BOOL CInterfaceSPI::check_ALIVE_BYTE()
{
	BOOL bRes=TRUE;

	IncrementAlive();

	Send_ALIVE_BYTE(m_byAlive);
	Sleep(50);
	BYTE byTempAlive=Read_ALIVE_BYTE();
	if(byTempAlive!=m_byAlive)
	{
		IncrementAlive();
		Send_ALIVE_BYTE(m_byAlive);
		Sleep(50);
		byTempAlive=Read_ALIVE_BYTE();
		if(byTempAlive!=m_byAlive)
		{
			bRes=FALSE;
		}
	}
	return bRes;
}

void CInterfaceSPI::IncrementAlive()
{
	if(m_byAlive<254)
		m_byAlive++;
	else
		m_byAlive=1;
}
// **************************************************************************
// 
// **************************************************************************
BOOL CInterfaceSPI::WriteNSPIdata(BYTE* pCommand,WORD wcommandSize,WORD wValue, WORD wOffset)	//submit data
{
	BOOL bRes=true;

	if(m_hSPIFile!=INVALID_HANDLE_VALUE)
	{
		EnterCriticalSection(&csSPIFile);

		BYTE data[2];

		DWORD dwResult=getModel()->getDIO()->WritePin (CDPINBIT);
		if(dwResult==0)
		{
			//LogError(_T("DIO: Error while write pin data"));
			bRes=false;
		}

		//command[0] = 43;
		if(!DeviceIoControl(m_hSPIFile, IOCTL_NSPI_SEND, pCommand, wcommandSize,NULL, 0, NULL, NULL))
		{
			theApp.writeLogError(GetLastError(), _T("SPI: Error while send data#3"));
			//LogError(_T("SPI: Error while send data"));
			bRes=false;
		}

		dwResult=getModel()->getDIO()->ClearPin (CDPINBIT);
		if(dwResult==0)
		{
			theApp.writeLogError(GetLastError(), _T("DIO: Error while clear pin data#2"));
			//LogError(_T("DIO: Error while clear pin data"));
			bRes=false;
		}

	
		////BYTE bufferLo=LOBYTE(wValue);
		////BYTE bufferHi=HIBYTE(wValue);
		data[0] = LOBYTE(wValue);
		data[1] = HIBYTE(wValue);
		
		if(!DeviceIoControl(m_hSPIFile, IOCTL_NSPI_SEND, data, 2,NULL, 0, NULL, NULL))
		{
			theApp.writeLogError(GetLastError(), _T("SPI: Error while write data"));
			//LogError(_T("SPI: Error while write data"));
			bRes=false;
		}

		LeaveCriticalSection(&csSPIFile);

	}
	else if(m_hSPIFile==INVALID_HANDLE_VALUE && getModel()->isSPIavailabel())
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
	}

	return bRes;
}



// **************************************************************************
// 
// **************************************************************************
//bool CInterfaceSPI::WriteNSPI(BYTE* pData,WORD wSize, WORD wOffset)
//{
//	if(m_hSPIFile==INVALID_HANDLE_VALUE)
//		return false;
//
//	//Protect(false);
//
//	//SPI_DATA sd;
//	//BYTE command[3];
//
//	/*command[0] = MEM_WREN;
//	sd.pchCommandBuf = command;
//	sd.wCommandLen = 1;
//	sd.pchReadBuf = NULL;
//	sd.pchWriteBuf = NULL;
//	sd.wDataLen = 0;
//	DeviceIoControl(m_hSPIFile, IOCTL_SPI_TRANSFER, (PBYTE)&sd, sizeof(sd),NULL, 0, NULL, NULL);
//
//	command[0] = MEM_WRITE;
//	command[1] = wOffset / MEM_BLOCK_SIZE;
//	command[2] = wOffset % MEM_BLOCK_SIZE;
//
//	sd.wCommandLen = 3;
//	sd.pchReadBuf = NULL;
//	sd.pchWriteBuf = (BYTE*)pData;
//	sd.wDataLen = wSize;
//	DeviceIoControl(m_hSPIFile, IOCTL_SPI_TRANSFER, (PBYTE)&sd, sizeof(sd), NULL, 0, NULL, NULL);*/
//
//	//Protect(true);
//	return true;
//}

// **************************************************************************
// 
// **************************************************************************
//bool CInterfaceSPI::ReadNSPI(BYTE* pData, WORD wSize, WORD wOffset)
//{
//	if(m_hSPIFile==INVALID_HANDLE_VALUE)
//		return false;
//
//	//SPI_DATA sd;
//	//BYTE command[3];
//	/*command[0] = MEM_READ;
//	command[1] = wOffset / MEM_BLOCK_SIZE;
//	command[2] = wOffset % MEM_BLOCK_SIZE;
//	sd.pchCommandBuf = command;
//	sd.wCommandLen = 3;
//	sd.pchReadBuf = (BYTE*)pData;
//	sd.pchWriteBuf = NULL;
//	sd.wDataLen = wSize;
//	DeviceIoControl(m_hSPIFile, IOCTL_SPI_TRANSFER, (PBYTE)&sd, sizeof(sd),NULL, 0, NULL, NULL);*/
//
//	return true;
//}



// **************************************************************************
// 
// **************************************************************************
//bool CInterfaceSPI::IsNewATZ()
//{
//	return m_bNewATZ;
//}


// **************************************************************************
// 
// **************************************************************************
//bool CInterfaceSPI::NewMonitorData()
//{
//	return m_bMonitorData;
//}
//void CInterfaceSPI::ResetMonitorData()
//{
//	m_bMonitorData=false;
//	m_bNewATZ=false;
//}
// **************************************************************************
// 
// **************************************************************************
//bool CInterfaceSPI::IsTriggeredBreath()
//{
//	return m_bTriggeredBreath;
//}
//void CInterfaceSPI::ResetTriggeredBreath()
//{
//	m_bTriggeredBreath=false;
//}
// **************************************************************************
// 
// **************************************************************************
bool CInterfaceSPI::NewAlarmData()
{
	return m_bAlarmData;
}

void CInterfaceSPI::ResetAlarmData()
{
	m_bAlarmData=false;
}

// **************************************************************************
// 
// **************************************************************************
bool CInterfaceSPI::NewFlowsensorData()
{
	return m_bFlowsensorData;
}

void CInterfaceSPI::ResetFlowsensorData()
{
	m_bFlowsensorData=false;
}


// **************************************************************************
// 
// **************************************************************************
BOOL CInterfaceSPI::SendSPICommand(SPISENDMESSAGE* pMessage, bool bIgnoreSPIstate) 
{
	
	if(m_hSPIFile!=INVALID_HANDLE_VALUE && getModel()->isSPIavailabel())
	{
		EnterCriticalSection(&csSPIsendqueue);

		SPIsendqueue.AddTail(pMessage);

		LeaveCriticalSection(&csSPIsendqueue);
	}
	else if(m_hSPIFile!=INVALID_HANDLE_VALUE && bIgnoreSPIstate)
	{
		EnterCriticalSection(&csSPIsendqueue);

		SPIsendqueue.AddTail(pMessage);

		LeaveCriticalSection(&csSPIsendqueue);
	}
	else if(m_hSPIFile==INVALID_HANDLE_VALUE && getModel()->isSPIavailabel())
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
	}

	Sleep(0);

	return 0;
}


// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_ALARMKEY_PUSH()
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_ALARMKEY_PUSHED;
	pMessage->iValue=1;

	SendSPICommand(pMessage);
#endif

}


// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_VENT_MODE(eVentMode mode)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_VENT_MODE;
	pMessage->iValue=(int)mode;

	SendSPICommand(pMessage);
#endif


	

}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_PINSP(int val)//newVG
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_P_INSP;
	pMessage->iValue=val;

	SendSPICommand(pMessage);
#endif

}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_PMAXVG(int val)//newVG
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_PMAXVG;
	pMessage->iValue=val;

	SendSPICommand(pMessage);
#endif

}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_P_PSV(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_P_PSV;
	pMessage->iValue=val;
	SendSPICommand(pMessage);
#endif

	
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_PEEP(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_PEEP;
	pMessage->iValue=val;

	SendSPICommand(pMessage);
#endif

}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_OXY_RATIO(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_OXY_RATIO;
	pMessage->iValue=val;

	SendSPICommand(pMessage);
#endif

}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_TRIG_SCHWELLE(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_TRIG_SCHWELLE;
	pMessage->iValue=val;

	SendSPICommand(pMessage);

	//DEBUGMSG(TRUE, (TEXT("Send_PARAVAL_TRIG_SCHWELLE %d\r\n"),val));
#endif

}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_RISETIME(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_RISETIME;
	pMessage->iValue=val;

	SendSPICommand(pMessage);

	//DEBUGMSG(TRUE, (TEXT("Send_PARAVAL_TRIG_SCHWELLE %d\r\n"),val));
#endif

}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_FLOW_CORFACTOR_NEO(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_FLOW_CORFACTOR_NEO;
	pMessage->iValue=val;

	SendSPICommand(pMessage);
#endif
}

void CInterfaceSPI::Send_FLOW_CORFACTOR_PED(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_FLOW_CORFACTOR_PED;
	pMessage->iValue=val;

	SendSPICommand(pMessage);
#endif
}



// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_INSP_FLOW(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_FLOW_INSP;
	//pMessage->byteSPICommand[0]=0x35;
	pMessage->iValue=val;

	SendSPICommand(pMessage);
#endif
	
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_EXH_FLOW(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_FLOW_EXH;
	pMessage->iValue=val;

	SendSPICommand(pMessage);
#endif
	
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_APNOE_TIME(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_APNOE_TIME;
	pMessage->iValue=val;

	SendSPICommand(pMessage);
#endif
	
}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_BACKUP(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_BACKUP;
	pMessage->iValue=val;

	SendSPICommand(pMessage);
#endif
}


// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_INSP_TIME(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_INSP_TIME;
	pMessage->iValue=val;

	SendSPICommand(pMessage);
#endif
	
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_HARDWARE_CONFIG(WORD wHardwareConfig)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_HW_CONFIG;
	pMessage->iValue=wHardwareConfig;

	SendSPICommand(pMessage);
#endif
	
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_MODE_OPTION1(WORD wMode)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_MODE_OPTION;

	pMessage->iValue=wMode;

	SendSPICommand(pMessage);
#endif

}


// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_EXH_TIME(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_EXH_TIME;
	pMessage->iValue=val;

	SendSPICommand(pMessage);
#endif

}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_HF_AMPL(int val)
{
#ifndef SIMULATION_NOSPI
	//AmplitudeCorrectionFactor
	/*if(false==getModel()->getDATAHANDLER()->getFOToscillationState())
	{
		double iAmpCorFactor = getModel()->getDATAHANDLER()->getAmpCorFactor(getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara());
		if(iAmpCorFactor==0)
			iAmpCorFactor=1;
		val=(SHORT)(double)val/iAmpCorFactor;
	}
	else*/
	{
		double iAmpCorFactor = getModel()->getDATAHANDLER()->getAmpCorFactor(getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara());
		if(iAmpCorFactor==0)
			iAmpCorFactor=1;
		val=(SHORT)(double)val/iAmpCorFactor;
	}
	

	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_HF_AMPLITUDE;
	pMessage->iValue=val;

	SendSPICommand(pMessage);
#endif

}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_HF_FREQ(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_HF_FREQ;
	pMessage->iValue=val;

	SendSPICommand(pMessage);
#endif

}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_HF_PMITT(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_HF_MEAN_AIRWAY_PRESS;
	pMessage->iValue=val;

	SendSPICommand(pMessage);
#endif

}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_HF_PMEANREC(int val)
{
	/*SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_HF_MEAN_AIRWAY_PRESS;
	pMessage->iValue=val;

	SendSPICommand(pMessage);*/
}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_HF_FREQ_REC(int val)
{
	/*SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_HF_MEAN_AIRWAY_PRESS;
	pMessage->iValue=val;

	SendSPICommand(pMessage);*/
}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_HF_ITIME_REC(int val)
{
	/*SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_HF_MEAN_AIRWAY_PRESS;
	pMessage->iValue=val;

	SendSPICommand(pMessage);*/
}

// **************************************************************************
// 
// **************************************************************************
//void CInterfaceSPI::Send_PARAVAL_HF_IERATIO(int val)
//{
//	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
//	pMessage->byteSPICommand[0]=S_PARA_HF_IE_RATIO;
//	pMessage->iValue=val;
//
//	SendSPICommand(pMessage);
//}


// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_KOMMANDO(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_KOMMANDO;

	pMessage->iValue=val;

	SendSPICommand(pMessage);
#endif
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_FLOWSENS_CMND(int val)
{
	//Send_KOMMANDO(val);

#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_FLOWSENS_CMND;

	pMessage->iValue=val;

	SendSPICommand(pMessage);
#endif
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_VOLUME_LIMIT(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_VOLUME_LIMIT;
	pMessage->iValue=val;

	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("VL%d"),val);
	theApp.getLog()->WriteLine(psz);

	SendSPICommand(pMessage);
#endif

}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_PARAVAL_VOLUME_GARANT(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_PARA_VOLUME_GARANT;
	pMessage->iValue=val;

	TCHAR psz[MAX_PATH];
	wsprintf(psz,_T("VG%d"),val);
	theApp.getLog()->WriteLine(psz);

	SendSPICommand(pMessage);
#endif

}



// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_ABORT_CRITERIONPSV(int val)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_ABORTCRITERION_PSV;
	pMessage->iValue=val;

	SendSPICommand(pMessage);
#endif

}


// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::GetMessureData()
{
	int iResult=0;
		
	

	/*if(m_bDemoMode)
	{
		iResult=ReadDemoData();
	}
	else*/
	{
		iResult=ReadSPIData();
	}

	

	if(		getModel()->getCONFIG()->GetCurMode()!=VM_SERVICE 
	&&	getModel()->getVIEWHANDLER()->getViewState()!=VS_STARTUP)
		getModel()->getDATAHANDLER()->checkLimits();

	return iResult;
}

// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::ReadSPIData()
{
	int iResult=0;
	int iBuf=0;
	SHORT iRes=0;
	BYTE byLo;
	BYTE byHi;
	CStringW szError=_T("");
	
	bool bHFOmode=false;
	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		bHFOmode=true;
	}

	BYTE byBuffer[BLOCKBUFSIZE];
	if(ReadNSPIAVGblock(byBuffer,BLOCKBUFSIZE))
	{
		//1. Byte: M_AVG_P_Peak
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);

		//AmplitudeCorrectionFactor
		if(false==getModel()->getDATAHANDLER()->getFOToscillationState() && bHFOmode)
		{
			double iPmitt=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
			double iAmpCorFactor = getModel()->getDATAHANDLER()->getAmpCorFactor(getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara());
			if(iAmpCorFactor==0)
				iAmpCorFactor=1;
			iRes=(SHORT)(((double)iRes-iPmitt)*iAmpCorFactor)+iPmitt;
		}
		/*else if(bHFOmode)
		{
			double iPmitt=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
			double iAmpCorFactor = getModel()->getDATAHANDLER()->getAmpCorFactor(getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara());
			if(iAmpCorFactor==0)
				iAmpCorFactor=1;
			iRes=(SHORT)(((double)iRes-iPmitt)*iAmpCorFactor)+iPmitt;
		}*/
		
		if(		(iRes<MesData_P_Peak_Min)
			||	(iRes>MesData_P_Peak_Max))
		{
			//ERRORxxx
			iResult=getModel()->SetBit(iResult, 1);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0131 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}
			
			if(iRes<MesData_P_Peak_Min)
				iRes=MesData_P_Peak_Min;
			else if(iRes>MesData_P_Peak_Max)
				iRes=MesData_P_Peak_Max;
		}
		//MessureDataAVG.m_iPmax=iRes;//IPPV 190
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_P_PEAK,iRes);


		//2. Byte: M_AVG_P_Mean
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_P_Mean_Min)
			||	(iRes>MesData_P_Mean_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 2);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0132 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_P_Mean_Min)
				iRes=MesData_P_Mean_Min;
			else if(iRes>MesData_P_Mean_Max)
				iRes=MesData_P_Mean_Max;
		}
		else if(iRes>-6 && iRes<0)
		{
			iRes=0;
		}
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_P_MEAN,iRes);
		//DEBUGMSG(TRUE, (TEXT("P_MEAN %d\r\n"),iRes));

		//3. Byte: M_AVG_PEEP
		byLo=byBuffer[iBuf];
		//byLo=246;
		iBuf++;
		byHi=byBuffer[iBuf];
		//byHi=255;
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_PEEP_Min)
			||	(iRes>MesData_PEEP_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 3);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0133 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_PEEP_Min)
				iRes=MesData_PEEP_Min;
			else if(iRes>MesData_PEEP_Max)
				iRes=MesData_PEEP_Max;
		}
		else if(iRes>-6 && iRes<0)
		{
			iRes=0;
		}
		//MessureDataAVG.m_iPEEP=iRes;//IPPV 48
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_PEEP,iRes);


		//4. Byte: M_AVG_T_insp_PSV
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_T_insp_PSV_Min)
			||	(iRes>MesData_T_insp_PSV_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 4);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0134 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_T_insp_PSV_Min)
				iRes=MesData_T_insp_PSV_Min;
			else if(iRes>MesData_T_insp_PSV_Max)
				iRes=MesData_T_insp_PSV_Max;
		}
		//MessureDataAVG.m_iITimePSV=(WORD)iRes;//IPPV 0
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_T_INSP_PSV,iRes);



		//5. Byte: M_AVG_Dyn_Compl
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_Dyn_Compl_Min)
			||	(iRes>MesData_Dyn_Compl_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 5);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0135 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_Dyn_Compl_Min)
				iRes=MesData_Dyn_Compl_Min;
			else if(iRes>MesData_Dyn_Compl_Max)
				iRes=MesData_Dyn_Compl_Max;
		}
		//MessureDataAVG.m_iCompliance=(WORD)iRes;//IPPV 10
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_DYNCOMPL,iRes);

		//6. Byte: M_AVG_C20/C
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);

		if(		(iRes<MesData_C20C_Min)
			||	(iRes>MesData_C20C_Max))
		{
			iResult=getModel()->SetBit(iResult, 6);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0136 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_C20C_Min)
				iRes=MesData_C20C_Min;
			else if(iRes>MesData_C20C_Max)
				iRes=MesData_C20C_Max;

		}
		//MessureDataAVG.m_iC20C=(WORD)iRes;//IPPV 27
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_C20C,iRes);


		//7. Byte: M_AVG_Resistance
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);

		//test
		//iRes=MesData_Resistance_Max;

		if(		(iRes<MesData_Resistance_Min)
			||	(iRes>MesData_Resistance_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 7);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0137 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_Resistance_Min)
				iRes=MesData_Resistance_Min;
			else if(iRes>MesData_Resistance_Max)
				iRes=MesData_Resistance_Max;

		}
		//MessureDataAVG.m_iResistance=(WORD)iRes;//IPPV 2350
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_RESISTANCE,iRes);


		//8. Byte: M_AVG_Min_Vol
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_Min_Vol_Min)
			||	(iRes>MesData_Min_Vol_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 8);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0138 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_Min_Vol_Min)
				iRes=MesData_Min_Vol_Min;
			else if(iRes>MesData_Min_Vol_Max)
				iRes=MesData_Min_Vol_Max;
		}
		//MessureDataAVG.m_iMV=(WORD)iRes;//IPPV 814
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_MIN_VOL,iRes);


		//9. Byte: M_AVG_%Anteil_Resp
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_Perc_Resp_Min)
			||	(iRes>MesData_Perc_Resp_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 9);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0139 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_Perc_Resp_Min)
				iRes=MesData_Perc_Resp_Min;
			else if(iRes>MesData_Perc_Resp_Max)
				iRes=MesData_Perc_Resp_Max;
		}
		//MessureDataAVG.m_iPercent=(BYTE)iRes;//IPPV 100
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_ANTEIL_RESP,iRes);


		//10. Byte: M_AVG_TVE
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_TVE_Min)
			||	(iRes>MesData_TVE_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 10);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0140 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_TVE_Min)
				iRes=MesData_TVE_Min;
			else if(iRes>MesData_TVE_Max)
				iRes=MesData_TVE_Max;
		}
		//MessureDataAVG.m_iTVE=(WORD)iRes;//IPPV 151
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_TVE,iRes);


		//11. Byte: M_AVG_TVE_Respirator
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_TVE_Resp_Min)
			||	(iRes>MesData_TVE_Resp_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 11);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0141 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_TVE_Resp_Min)
				iRes=MesData_TVE_Resp_Min;
			else if(iRes>MesData_TVE_Resp_Max)
				iRes=MesData_TVE_Resp_Max;
		}
		//MessureDataAVG.m_iTVEresp=(WORD)iRes;//IPPV 151
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_TVE_RESP,iRes);


		//12. Byte: M_AVG_TVE_Patient
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_TVE_Pat_Min)
			||	(iRes>MesData_TVE_Pat_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 12);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0142 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_TVE_Pat_Min)
				iRes=MesData_TVE_Pat_Min;
			else if(iRes>MesData_TVE_Pat_Max)
				iRes=MesData_TVE_Pat_Max;
		}
		//MessureDataAVG.m_iTVEpat=(WORD)iRes;//IPPV 0
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_TVE_PATIENT,iRes);


		//13. Byte: M_AVG_Leak
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_Leak_Min)
			||	(iRes>MesData_Leak_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 13);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0143 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_Leak_Min)
				iRes=MesData_Leak_Min;
			else if(iRes>MesData_Leak_Max)
				iRes=MesData_Leak_Max;
		}
		//MessureDataAVG.m_iLeak=(BYTE)iRes;//IPPV 0
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_LEAK,iRes);


		//14. Byte: M_AVG_Freq
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_Freq_Min)
			||	(iRes>MesData_Freq_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 14);
			//iResult=iRes;

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0144 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_Freq_Min)
				iRes=MesData_Freq_Min;
			else if(iRes>MesData_Freq_Max)
				iRes=MesData_Freq_Max;
		}
		//MessureDataAVG.m_iBPM=(BYTE)iRes;//IPPV 52
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_FREQ,iRes);


		//15. Byte: M_AVG_Trig_Value
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_Trig_Value_Min)
			||	(iRes>MesData_Trig_Value_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 15);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0145 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_Trig_Value_Min)
				iRes=MesData_Trig_Value_Min;
			else if(iRes>MesData_Trig_Value_Max)
				iRes=MesData_Trig_Value_Max;
		}
		//MessureDataAVG.m_iTrigVol=(WORD)iRes;//IPPV 0
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_TRIGGER,iRes);

		//16. Byte: M_AVG_TVI
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_TVI_Min)
			||	(iRes>MesData_TVI_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 16);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0146 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_TVI_Min)
				iRes=MesData_TVI_Min;
			else if(iRes>MesData_TVI_Max)
				iRes=MesData_TVI_Max;
		}
		//MessureDataAVG.m_iTVI=(WORD)iRes;//IPPV 153
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_TVI,iRes);



		//17. Byte: M_AVG_HFO_Amp
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);

		//AmplitudeCorrectionFactor
		if(false==getModel()->getDATAHANDLER()->getFOToscillationState() && bHFOmode)
		{
			double iAmpCorFactor = getModel()->getDATAHANDLER()->getAmpCorFactor(getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara());
			if(iAmpCorFactor==0)
				iAmpCorFactor=1;
			iRes=(SHORT)(double)iRes*iAmpCorFactor;
		}
		/*else if(bHFOmode)
		{
			double iAmpCorFactor = getModel()->getDATAHANDLER()->getAmpCorFactor(getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara());
			if(iAmpCorFactor==0)
				iAmpCorFactor=1;
			iRes=(SHORT)(double)iRes*iAmpCorFactor;
		}*/
		

		if(		(iRes<MesData_HFO_Amp_Min)
			||	(iRes>MesData_HFO_Amp_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 17);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0147 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_HFO_Amp_Min)
				iRes=MesData_HFO_Amp_Min;
			else if(iRes>MesData_HFO_Amp_Max)
				iRes=MesData_HFO_Amp_Max;
		}
		//MessureDataAVG.m_iHFAmpl=(WORD)iRes;//IPPV 0
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_HFO_AMP,iRes);


		//18. Byte: M_AVG_TVE_HFO
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_TVE_HFO_Min)
			||	(iRes>MesData_TVE_HFO_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 18);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0148 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_TVE_HFO_Min)
				iRes=MesData_TVE_HFO_Min;
			else if(iRes>MesData_TVE_HFO_Max)
				iRes=MesData_TVE_HFO_Max;
		}
		//MessureDataAVG.m_iTVEHFO=(WORD)iRes;//IPPV 0
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_TVE_HFO,iRes);


		//19. Byte: M_AVG_DCO2
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_DCO2_Min)
			||	(iRes>MesData_DCO2_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 19);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0149 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_DCO2_Min)
				iRes=MesData_DCO2_Min;
			else if(iRes>MesData_DCO2_Max)
				iRes=MesData_DCO2_Max;
		}
		//MessureDataAVG.m_iDCO2=(WORD)iRes;//IPPV 0
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_DCO2,iRes);


		//20. Byte: M_AVG_HFO_Freq
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;

		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_HFO_Freq_Min)
			||	(iRes>MesData_HFO_Freq_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 20);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0150 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_HFO_Freq_Min)
				iRes=MesData_HFO_Freq_Min;
			else if(iRes>MesData_HFO_Freq_Max)
				iRes=MesData_HFO_Freq_Max;
		}
		//MessureDataAVG.m_iHFFreq=(BYTE)iRes;//IPPV 0
		getModel()->getDATAHANDLER()->setMessureDataAVG(ALINK_MSMNT_HFO_FREQ,iRes);



		//21. Byte: M_AVG_VOL_CORR
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;

		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<0)
			||	(iRes>255))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 21);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0151 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<0)
				iRes=0;
			else if(iRes>255)
				iRes=255;
		}

		getModel()->getDATAHANDLER()->setMessureDataVolumeCorrection((BYTE)iRes);//IPPV 0

		//getModel()->getDATAHANDLER()->setBlockMessureDataAVG(&MessureDataAVG);
		getModel()->getDATAHANDLER()->setMessureDataAVGasTrendData();
	}
	else
	{
		if(m_bShowReadSPIDataError)
		{
			iResult=888;
			theApp.getLog()->WriteLine(_T("#ERROR: ReadNSPIAVGblock"));
		}
		
	}


	iBuf=0;
	iRes=0;

	//MESSURE_BLOCKDATA MessureDataBTB;
	if(ReadNSPIATZblock(byBuffer,BLOCKBUFSIZE))
	{
		//1. Byte: M_ATZ_P_Peak
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);

		//AmplitudeCorrectionFactor
		if(false==getModel()->getDATAHANDLER()->getFOToscillationState() && bHFOmode)
		{
			double iPmitt=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
			double iAmpCorFactor = getModel()->getDATAHANDLER()->getAmpCorFactor(getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara());
			if(iAmpCorFactor==0)
				iAmpCorFactor=1;
			iRes=(SHORT)(((double)iRes-iPmitt)*iAmpCorFactor)+iPmitt;
		}
		/*else if(bHFOmode)
		{
			double iPmitt=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
			double iAmpCorFactor = getModel()->getDATAHANDLER()->getAmpCorFactor(getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara());
			if(iAmpCorFactor==0)
				iAmpCorFactor=1;
			iRes=(SHORT)(((double)iRes-iPmitt)*iAmpCorFactor)+iPmitt;
		}*/
		

		if(		(iRes<MesData_P_Peak_Min)
			||	(iRes>MesData_P_Peak_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 1);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0152 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_P_Peak_Min)
				iRes=MesData_P_Peak_Min;
			else if(iRes>MesData_P_Peak_Max)
				iRes=MesData_P_Peak_Max;
		}
		//MessureDataBTB.m_iPmax=iRes;//IPPV 198
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_P_PEAK,iRes);


		//2. Byte: M_ATZ_P_Mean
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_P_Mean_Min)
			||	(iRes>MesData_P_Mean_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 2);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0153 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_P_Mean_Min)
				iRes=MesData_P_Mean_Min;
			else if(iRes>MesData_P_Mean_Max)
				iRes=MesData_P_Mean_Max;
		}
		else if(iRes>-6 && iRes<0)
		{
			iRes=0;
		}

		
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_P_MEAN,iRes);


		//3. Byte: M_ATZ_PEEP

		//byLo=246;
		//iBuf++;
		////byHi=byBuffer[iBuf];
		//byHi=255;
		//iBuf++;

		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;

		iRes=MAKEWORD(byLo,byHi);
		//test
		if(		(iRes<MesData_PEEP_Min)
			||	(iRes>MesData_PEEP_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 3);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0154 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_PEEP_Min)
				iRes=MesData_PEEP_Min;
			else if(iRes>MesData_PEEP_Max)
				iRes=MesData_PEEP_Max;
		}
		else if(iRes>-6 && iRes<0)
		{
			iRes=0;
		}

		//MessureDataBTB.m_iPEEP=iRes;//IPPV 48
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_PEEP,iRes);

		

		//4. Byte: M_ATZ_T_insp_PSVdebug
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_T_insp_PSV_Min)
			||	(iRes>MesData_T_insp_PSV_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 4);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0155 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_T_insp_PSV_Min)
				iRes=MesData_T_insp_PSV_Min;
			else if(iRes>MesData_T_insp_PSV_Max)
				iRes=MesData_T_insp_PSV_Max;
		}
		//MessureDataBTB.m_iITimePSV=(WORD)iRes;//IPPV 0
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_T_INSP_PSV,iRes);



		//5. Byte: M_ATZ_Dyn_Compl
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_Dyn_Compl_Min)
			||	(iRes>MesData_Dyn_Compl_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 5);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0156 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_Dyn_Compl_Min)
				iRes=MesData_Dyn_Compl_Min;
			else if(iRes>MesData_Dyn_Compl_Max)
				iRes=MesData_Dyn_Compl_Max;

		}
		//MessureDataBTB.m_iCompliance=(WORD)iRes;//IPPV 10
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_DYNCOMPL,iRes);


		//6. Byte: M_ATZ_C20/C
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);

		
		if(		(iRes<MesData_C20C_Min)
			||	(iRes>MesData_C20C_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 6);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0157 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_C20C_Min)
				iRes=MesData_C20C_Min;
			else if(iRes>MesData_C20C_Max)
				iRes=MesData_C20C_Max;
		}
		//MessureDataBTB.m_iC20C=(WORD)iRes;//IPPV 26
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_C20C,iRes);


		//7. Byte: M_ATZ_Resistance
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_Resistance_Min)
			||	(iRes>MesData_Resistance_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 7);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0158 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_Resistance_Min)
				iRes=MesData_Resistance_Min;
			else if(iRes>MesData_Resistance_Max)
				iRes=MesData_Resistance_Max;
		}
		//MessureDataBTB.m_iResistance=(WORD)iRes;//IPPV 2410
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_RESISTANCE,iRes);


		//8. Byte: M_ATZ_Min_Vol
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_Min_Vol_Min)
			||	(iRes>MesData_Min_Vol_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 8);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0159 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_Min_Vol_Min)
				iRes=MesData_Min_Vol_Min;
			else if(iRes>MesData_Min_Vol_Max)
				iRes=MesData_Min_Vol_Max;
		}
		//MessureDataBTB.m_iMV=(WORD)iRes;//IPPV 832
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_MIN_VOL,iRes);


		//9. Byte: M_ATZ_%Anteil_Resp
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_Perc_Resp_Min)
			||	(iRes>MesData_Perc_Resp_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 9);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0160 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_Perc_Resp_Min)
				iRes=MesData_Perc_Resp_Min;
			else if(iRes>MesData_Perc_Resp_Max)
				iRes=MesData_Perc_Resp_Max;
		}
		//MessureDataBTB.m_iPercent=(BYTE)iRes;//IPPV 100
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_ANTEIL_RESP,iRes);


		//10. Byte: M_ATZ_TVE
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_TVE_Min)
			||	(iRes>MesData_TVE_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 10);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0161 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_TVE_Min)
				iRes=MesData_TVE_Min;
			else if(iRes>MesData_TVE_Max)
				iRes=MesData_TVE_Max;
		}
		//MessureDataBTB.m_iTVE=(WORD)iRes;//IPPV 157
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_TVE,iRes);


		//11. Byte: M_ATZ_TVE_Respirator
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_TVE_Resp_Min)
			||	(iRes>MesData_TVE_Resp_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 11);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0162 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_TVE_Resp_Min)
				iRes=MesData_TVE_Resp_Min;
			else if(iRes>MesData_TVE_Resp_Max)
				iRes=MesData_TVE_Resp_Max;
		}
		//MessureDataBTB.m_iTVEresp=(WORD)iRes;//IPPV 0
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_TVE_RESP,iRes);


		//12. Byte: M_ATZ_TVE_Patient
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_TVE_Pat_Min)
			||	(iRes>MesData_TVE_Pat_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 12);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0163 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_TVE_Pat_Min)
				iRes=MesData_TVE_Pat_Min;
			else if(iRes>MesData_TVE_Pat_Max)
				iRes=MesData_TVE_Pat_Max;
		}
		//MessureDataBTB.m_iTVEpat=(WORD)iRes;//IPPV 0
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_TVE_PATIENT,iRes);


		//13. Byte: M_ATZ_Leak
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_Leak_Min)
			||	(iRes>MesData_Leak_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 13);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0164 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_Leak_Min)
				iRes=MesData_Leak_Min;
			else if(iRes>MesData_Leak_Max)
				iRes=MesData_Leak_Max;
		}
		//MessureDataBTB.m_iLeak=(BYTE)iRes;//IPPV 0
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_LEAK,iRes);


		//14. Byte: M_ATZ_Freq
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_Freq_Min)
			||	(iRes>MesData_Freq_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 14);
			//iResult=iRes;

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0165 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_Freq_Min)
				iRes=MesData_Freq_Min;
			else if(iRes>MesData_Freq_Max)
				iRes=MesData_Freq_Max;
		}
		//MessureDataBTB.m_iBPM=(BYTE)iRes;//IPPV 52
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_FREQ,iRes);


		//15. Byte: M_ATZ_Trig_Value
		//if(getModel()->getDATAHANDLER()->activeModeIsTriggerMode()==true)
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_Trig_Value_Min)
			||	(iRes>MesData_Trig_Value_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 15);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0166 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_Trig_Value_Min)
				iRes=MesData_Trig_Value_Min;
			else if(iRes>MesData_Trig_Value_Max)
				iRes=MesData_Trig_Value_Max;
		}
		//MessureDataBTB.m_iTrigVol=(WORD)iRes;//IPPV 0
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_TRIGGER,iRes);

		//16. Byte: M_ATZ_TVI
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_TVI_Min)
			||	(iRes>MesData_TVI_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 16);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0167 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_TVI_Min)
				iRes=MesData_TVI_Min;
			else if(iRes>MesData_TVI_Max)
				iRes=MesData_TVI_Max;
		}
		//MessureDataBTB.m_iTVI=(WORD)iRes;//IPPV 157
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_TVI,iRes);



		//17. Byte: M_ATZ_HFO_Amp
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);

		//AmplitudeCorrectionFactor
		if(false==getModel()->getDATAHANDLER()->getFOToscillationState() && bHFOmode)
		{
			double iAmpCorFactor = getModel()->getDATAHANDLER()->getAmpCorFactor(getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara());
			if(iAmpCorFactor==0)
				iAmpCorFactor=1;
			iRes=(SHORT)(double)iRes*iAmpCorFactor;
		}
		/*else if(bHFOmode)
		{
			double iAmpCorFactor = getModel()->getDATAHANDLER()->getAmpCorFactor(getModel()->getDATAHANDLER()->PARADATA()->GetHFFreqPara());
			if(iAmpCorFactor==0)
				iAmpCorFactor=1;
			iRes=(SHORT)(double)iRes*iAmpCorFactor;
		}*/
		

		if(		(iRes<MesData_HFO_Amp_Min)
			||	(iRes>MesData_HFO_Amp_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 17);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0168 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_HFO_Amp_Min)
				iRes=MesData_HFO_Amp_Min;
			else if(iRes>MesData_HFO_Amp_Max)
				iRes=MesData_HFO_Amp_Max;
		}
		//MessureDataBTB.m_iHFAmpl=(WORD)iRes;//IPPV 0
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_HFO_AMP,iRes);


		//18. Byte: M_ATZ_TVE_HFO
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_TVE_HFO_Min)
			||	(iRes>MesData_TVE_HFO_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 18);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0169 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_TVE_HFO_Min)
				iRes=MesData_TVE_HFO_Min;
			else if(iRes>MesData_TVE_HFO_Max)
				iRes=MesData_TVE_HFO_Max;
		}
		//MessureDataBTB.m_iTVEHFO=(WORD)iRes;//IPPV 0
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_TVE_HFO,iRes);


		//19. Byte: M_ATZ_DCO2
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_DCO2_Min)
			||	(iRes>MesData_DCO2_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 19);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0170 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_DCO2_Min)
				iRes=MesData_DCO2_Min;
			else if(iRes>MesData_DCO2_Max)
				iRes=MesData_DCO2_Max;
		}
		//MessureDataBTB.m_iDCO2=(WORD)iRes;//IPPV 0
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_DCO2,iRes);


		//20. Byte: M_ATZ_HFO_Freq
		byLo=byBuffer[iBuf];
		iBuf++;
		byHi=byBuffer[iBuf];
		iBuf++;
		iRes=MAKEWORD(byLo,byHi);
		if(		(iRes<MesData_HFO_Freq_Min)
			||	(iRes>MesData_HFO_Freq_Max))
		{
			//ERROR
			iResult=getModel()->SetBit(iResult, 20);

			if(m_bShowReadSPIDataError)
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0171 %d#"),iRes);
				theApp.getLog()->WriteLine(sz);
			}

			if(iRes<MesData_HFO_Freq_Min)
				iRes=MesData_HFO_Freq_Min;
			else if(iRes>MesData_HFO_Freq_Max)
				iRes=MesData_HFO_Freq_Max;
		}
		//MessureDataBTB.m_iHFFreq=(BYTE)iRes;//IPPV 0
		getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_HFO_FREQ,iRes);

		//getModel()->getDATAHANDLER()->setBlockMessureDataBTB(&MessureDataBTB);
	}
	else
	{
		if(m_bShowReadSPIDataError)
		{
			iResult=999;
			theApp.getLog()->WriteLine(_T("#ERROR: ReadNSPIATZblock"));
		}
		
	}

	return iResult;
}

//int CInterfaceSPI::ReadDemoData()
//{
//	SHORT iRes=0;
//	int iResult=0;
//	MESSURE_BLOCKDATA MessureDataAVG;
//
//	//1. Byte: M_AVG_P_Peak
//	MessureDataAVG.m_iPmax=iRes;
//	//2. Byte: M_AVG_P_Mean
//	MessureDataAVG.m_iPmitt=iRes;
//	//3. Byte: M_AVG_PEEP
//	MessureDataAVG.m_iPEEP=iRes;
//	//4. Byte: M_AVG_T_insp_PSV
//	MessureDataAVG.m_iITimePSV=(WORD)iRes;
//	//5. Byte: M_AVG_Dyn_Compl
//	MessureDataAVG.m_iCompliance=(WORD)iRes;
//	//6. Byte: M_AVG_C20/C
//	MessureDataAVG.m_iC20C=(WORD)iRes;
//	//7. Byte: M_AVG_Resistance
//	MessureDataAVG.m_iResistance=(WORD)iRes;
//	//8. Byte: M_AVG_Min_Vol
//	MessureDataAVG.m_iMV=(WORD)iRes;
//	//9. Byte: M_AVG_%Anteil_Resp
//	MessureDataAVG.m_iPercent=(BYTE)iRes;
//	//10. Byte: M_AVG_TVE
//	MessureDataAVG.m_iTVE=(WORD)iRes;
//	//11. Byte: M_AVG_TVE_Respirator
//	MessureDataAVG.m_iTVEresp=(WORD)iRes;
//	//12. Byte: M_AVG_TVE_Patient
//	MessureDataAVG.m_iTVEpat=(WORD)iRes;
//	//13. Byte: M_AVG_Leak
//	MessureDataAVG.m_iLeak=(BYTE)iRes;
//	//14. Byte: M_AVG_Freq
//	MessureDataAVG.m_iBPM=(BYTE)iRes;
//	//15. Byte: M_AVG_Trig_Value
//	MessureDataAVG.m_iTrigVol=(WORD)iRes;
//	//16. Byte: M_AVG_TVI
//	MessureDataAVG.m_iTVI=(WORD)iRes;
//	//17. Byte: M_AVG_HFO_Amp
//	MessureDataAVG.m_iHFAmpl=(WORD)iRes;
//	//18. Byte: M_AVG_TVE_HFO
//	MessureDataAVG.m_iTVEHFO=(WORD)iRes;
//	//19. Byte: M_AVG_DCO2
//	MessureDataAVG.m_iDCO2=(WORD)iRes;
//	//20. Byte: M_AVG_HFO_Freq
//	MessureDataAVG.m_iHFFreq=(BYTE)iRes;
//	//21. Byte: M_AVG_VOL_CORR
//	getModel()->getDATAHANDLER()->setMessureDataVolumeCorrection((BYTE)iRes);
//
//	getModel()->getDATAHANDLER()->setBlockMessureDataAVG(&MessureDataAVG);
//
//
//
//	MESSURE_BLOCKDATA MessureDataBTB;
//	
//	//1. Byte: M_ATZ_P_Peak
//	MessureDataBTB.m_iPmax=iRes;
//	//2. Byte: M_ATZ_P_Mean
//	MessureDataBTB.m_iPmitt=iRes;
//	//3. Byte: M_ATZ_PEEP
//	MessureDataBTB.m_iPEEP=iRes;
//	//4. Byte: M_ATZ_T_insp_PSV
//	MessureDataBTB.m_iITimePSV=(WORD)iRes;
//	//5. Byte: M_ATZ_Dyn_Compl
//	MessureDataBTB.m_iCompliance=(WORD)iRes;
//	//6. Byte: M_ATZ_C20/C
//	MessureDataBTB.m_iC20C=(WORD)iRes;
//	//7. Byte: M_ATZ_Resistance
//	MessureDataBTB.m_iResistance=(WORD)iRes;
//	//8. Byte: M_ATZ_Min_Vol
//	MessureDataBTB.m_iMV=(WORD)iRes;
//	//9. Byte: M_ATZ_%Anteil_Resp
//	MessureDataBTB.m_iPercent=(BYTE)iRes;
//	//10. Byte: M_ATZ_TVE
//	MessureDataBTB.m_iTVE=(WORD)iRes;
//	//11. Byte: M_ATZ_TVE_Respirator
//	MessureDataBTB.m_iTVEresp=(WORD)iRes;
//	//12. Byte: M_ATZ_TVE_Patient
//	MessureDataBTB.m_iTVEpat=(WORD)iRes;
//	//13. Byte: M_ATZ_Leak
//	MessureDataBTB.m_iLeak=(BYTE)iRes;
//	//14. Byte: M_ATZ_Freq
//	MessureDataBTB.m_iBPM=(BYTE)iRes;
//	//15. Byte: M_ATZ_Trig_Value
//	MessureDataBTB.m_iTrigVol=(WORD)iRes;
//	//16. Byte: M_ATZ_TVI
//	MessureDataBTB.m_iTVI=(WORD)iRes;
//	//17. Byte: M_ATZ_HFO_Amp
//	MessureDataBTB.m_iHFAmpl=(WORD)iRes;
//	//18. Byte: M_ATZ_TVE_HFO
//	MessureDataBTB.m_iTVEHFO=(WORD)iRes;
//	//19. Byte: M_ATZ_DCO2
//	MessureDataBTB.m_iDCO2=(WORD)iRes;
//	//20. Byte: M_ATZ_HFO_Freq
//	MessureDataBTB.m_iHFFreq=(BYTE)iRes;
//
//	getModel()->getDATAHANDLER()->setBlockMessureDataBTB(&MessureDataBTB);
//
//	return iResult;
//
//}


// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_P_PROX_ADC()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_P_PROX_ADC;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0172"));
			}

		}
	}


	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_DRM_1_ADC()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_DRM_1_ADC;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0173"));
			}

		}
	}

	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_DRM_2_ADC()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_DRM_2_ADC;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				//getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: ERRC_SPI_READDATA Read_DRM_2_ADC"));
				theApp.getLog()->WriteLine(_T("#HFO:0174"));
			}

		}
	}

	
	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_DRM_3_ADC()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_DRM_3_ADC;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				//getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: ERRC_SPI_READDATA Read_DRM_3_ADC"));
				theApp.getLog()->WriteLine(_T("#HFO:0175"));
			}

		}
	}


	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_DMS_ADC()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_DMS_ADC;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_DMS_ADC"));
//			}
//
//		}
//	}
//
//
//	return iResult;
//}
// **************************************************************************
// //newVG
// **************************************************************************
//int CInterfaceSPI::Read_Batt_stat()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_Batt_stat;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				if(AfxGetApp())
//					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
//				theApp.getLog()->WriteLine(_T("#HFO:0176"));
//			}
//
//		}
//	}
//	return iResult;
//}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_OXY1_ADC()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_OXY1_ADC;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0177"));
			}

		}
	}


	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_OXY2_ADC()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_OXY2_ADC;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				//getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: ERRC_SPI_READDATA Read_OXY2_ADC"));
				theApp.getLog()->WriteLine(_T("#HFO:0178"));
			}

		}
	}

	return iResult;
}

// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_STATUS()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_STATUS;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				//getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: ERRC_SPI_READDATA Read_STATUS"));
				theApp.getLog()->WriteLine(_T("#HFO:0179"));
			}
		}
	}
	else
	{
		if(getModel()->isSPIavailabel()==FALSE)
			theApp.getLog()->WriteLine(_T("#HFO:0180"));
		else
			theApp.getLog()->WriteLine(_T("#HFO:0181"));
	}

	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_ATZ_P_Peak()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_ATZ_P_Peak;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
			getModel()->getDATAHANDLER()->setMessureDataBTB(ALINK_MSMNT_P_PEAK,iResult);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0182"));
			}

		}
	}

	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_CHECKSUM()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_CHECKSUM;	//M_CHECKSUM *2 +0 (read) = 18;
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0183"));
			}

		}
	}

	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
WORD CInterfaceSPI::Read_VERSION1()
{
	WORD iResult=0xFF;
	BYTE command[1];
	command[0] = M_VERSION1;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0184"));
			}

		}
	}


	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
WORD CInterfaceSPI::Read_VERSION2()
{
	WORD iResult=0xFF;
	BYTE command[1];
	command[0] = M_VERSION2;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0185"));
			}

		}
	}

	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_VCC_24_BLEND_ADC()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_VCC_24_BLEND_ADC;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0186"));
			}
		}
	}

	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_VCC_5_ADC()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_VCC_5_ADC;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0187"));
			}
		}
	}
	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_PARA_P_PSV()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_PARA_P_PSV;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_P_PSV"));
//			}
//		}
//	}
//
//
//	return iResult;
//}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_PARA_APNOE_TIME()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_PARA_APNOE_TIME;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_APNOE_TIME"));
//			}
//		}
//	}
//
//
//	return iResult;
//}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_PARA_BACKUP()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_PARA_BACKUP;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_BACKUP"));
//			}
//		}
//	}
//
//	return iResult;
//}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_PARA_INSP_TIME()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_PARA_INSP_TIME;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_INSP_TIME"));
//			}
//		}
//	}
//
//	return iResult;
//}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_MODE_OPTION()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_MODE_OPTION;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_MODE_OPTION"));
//			}
//		}
//	}
//
//
//	return iResult;
//}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_PARA_EXH_TIME()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_PARA_EXH_TIME;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_EXP_TIME"));
//			}
//		}
//	}
//
//	return iResult;
//}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_PARA_P_INSP()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_PARA_P_INSP;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_P_INSP"));
//			}
//		}
//	}
//
//	return iResult;
//}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_PARA_PEEP()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_PARA_PEEP;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_PEEP"));
//			}
//		}
//	}
//	return iResult;
//}

// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_VENT_MODE()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_VENT_MODE;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0188"));
			}
		}
	}
	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_PARA_OXY_RATIO()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_PARA_OXY_RATIO;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_OXY_RATIO"));
//			}
//		}
//	}
//
//	return iResult;
//}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_PARA_VOLUME_LIMIT()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_PARA_VOLUME_LIMIT;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_PARA_VOLUME_LIMIT"));
//			}
//		}
//	}
//
//	return iResult;
//}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_PARA_VOLUME_GARANT()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_PARA_VOLUME_GARANT;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_PARA_VOLUME_GARANT"));
//			}
//		}
//	}
//
//	return iResult;
//}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_PARA_FLOW_INSP()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_PARA_FLOW_INSP;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_PARA_FLOW_INSP"));
//			}
//		}
//	}
//
//	return iResult;
//}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_PARA_FLOW_EXH()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_PARA_FLOW_EXH;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_PARA_FLOW_EXH"));
//			}
//		}
//	}
//
//	return iResult;
//}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_CAL_PRESS_SCALE()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_CAL_PRESS_SCALE;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0189"));
			}
		}
	}
	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_CAL_PRESS_OFFSET()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_CAL_PRESS_OFFSET;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0190"));
			}
		}
	}

	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_P_DELTA_MAX()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_P_DELTA_MAX;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_P_DELTA_MAX"));
//			}
//		}
//	}
//
//	return iResult;
//}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_PARA_HF_AMPLITUDE()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_PARA_HF_AMPLITUDE;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_PARA_HF_AMPLITUDE"));
//			}
//		}
//	}
//
//	return iResult;
//}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_PARA_HF_FREQ()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_PARA_HF_FREQ;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_PARA_HF_FREQ"));
//			}
//		}
//	}
//
//
//	return iResult;
//}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_PARA_HF_MEAN_AIRWAY_PRESS()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_PARA_HF_MEAN_AIRWAY_PRESS;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_PARA_HF_MEAN_AIRWAY_PRESS"));
//			}
//		}
//	}
//
//	return iResult;
//}

// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_KOMMANDO()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_KOMMANDO;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0191"));
			}
		}
	}


	return iResult;
}

// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_FLOWSENS_CMND()
{
	int iResult=-1;
	//iResult=Read_KOMMANDO();

	BYTE command[1];
	command[0] = M_FLOWSENS_CMND;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0192"));
			}
		}
	}

	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_STATUS_PROXSENSOR()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_STATUS_PROXSENSOR;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0193"));
			}
		}
	}


	return iResult;

}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_STATUS_FLOWSENSOR()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_STATUS_FLOWSENSOR;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0194"));
			}
		}
	}
	return iResult;

}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_DAC_FLOW_INSP()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_DAC_FLOW_INSP;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0195"));
			}
		}
	}


	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_DAC_FLOW_EXH()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_DAC_FLOW_EXH;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0196"));
			}
		}
	}
	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_FLOW_INSP_ADC()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_FLOW_INSP_ADC;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0197"));
			}
		}
	}


	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_FLOW_EXH_ADC()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_FLOW_EXH_ADC;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0198"));
			}
		}
	}

	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_FLOW()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_FLOW;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_FLOW"));
//			}
//		}
//	}
//
//
//	return iResult;
//}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_SYST_OFFSET_FLOW_INSP()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_SYST_OFFSET_FLOW_INSP;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0199"));
			}
		}
	}


	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_SYST_OFFSET_FLOW_EXH()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_SYST_OFFSET_FLOW_EXH;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0200"));
			}
		}
	}

	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_SYST_SCALE_FLOW_INSP()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_SYST_SCALE_FLOW_INSP;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0201"));
			}
		}
	}


	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_SYST_SCALE_FLOW_EXH()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_SYST_SCALE_FLOW_EXH;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0202"));
			}
		}
	}


	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_FLOWCAL_DEV_A()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_FLOWCAL_DEV_A;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0203"));
			}
		}
	}

	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_FLOWCAL_DEV_B()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_FLOWCAL_DEV_B;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0204"));
			}
		}
	}


	return iResult;
}

// **************************************************************************
// 
// **************************************************************************
BYTE CInterfaceSPI::Read_AUTOFLOW_CORR()
{
	BYTE iResult=0;
	BYTE command[1];
	command[0] = M_AUTOFLOW_CORR;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=0;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0205"));
			}
		}
	}


	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_OXY_ACTUAL()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_OXY_ACTUAL;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0206"));
			}
		}
	}


	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_OXY_DEFAULT()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_OXY_DEFAULT;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0207"));
			}
		}
	}

	return iResult;
}

// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_STATUS_2()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_STATUS_2;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0208"));
			}
		}
	}


	return iResult;

}
// **************************************************************************
// 
// **************************************************************************
//int CInterfaceSPI::Read_PARA_TRIG_SCHWELLE()
//{
//	int iResult=-1;
//	BYTE command[1];
//	command[0] = M_PARA_TRIG_SCHWELLE;	
//	BYTE byBuffer[2];
//
//	if(getModel()->IsSPIavailabel())
//	{
//		if(ReadNSPIdata(command,1,byBuffer,2,0))
//		{
//			m_iReadERROR=0;
//			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
//		}
//		else
//		{
//			iResult=-1;
//			m_iReadERROR++;
//			if(m_iReadERROR>=5)
//			{
//				getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READDATA);
//				getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T("SPI: Error ERRC_SPI_READDATA"));
//				LogError(_T("SPI: Error while Read_PARA_TRIG_SCHWELLE"));
//			}
//		}
//	}
//
//
//	return iResult;
//}


// **************************************************************************
// 
// **************************************************************************
int CInterfaceSPI::Read_FLOW_CORFACTOR_NEO()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_FLOW_CORFACTOR_NEO;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0209"));
			}
		}
	}

	return iResult;
}

int CInterfaceSPI::Read_FLOW_CORFACTOR_PED()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_FLOW_CORFACTOR_PED;	
	BYTE byBuffer[2];

	if(getModel()->isSPIavailabel())
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			m_iReadERROR++;
			if(m_iReadERROR>=5)
			{
				getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READDATA);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
				theApp.getLog()->WriteLine(_T("#HFO:0209"));
			}
		}
	}

	return iResult;
}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::send_Shutdown()
{
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_KOMMANDO;
	pMessage->iValue=99;

	SendSPICommand(pMessage);
}
// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_ALIVE_BYTE(int val)
{
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_ALIVE;
	pMessage->iValue=val;

	SendSPICommand(pMessage,true);
}
int CInterfaceSPI::Read_ALIVE_BYTE()
{
	int iResult=-1;
	BYTE command[1];
	command[0] = M_ALIVE;	
	BYTE byBuffer[2];

	//if(m_hSPIFile!=INVALID_HANDLE_VALUE)
	{
		if(ReadNSPIdata(command,1,byBuffer,2,0))
		{
			m_iReadERROR=0;
			iResult=MAKEWORD(byBuffer[0],byBuffer[1]);
		}
		else
		{
			iResult=-1;
			//newSPI
			//m_iReadERROR++;
			//if(m_iReadERROR>=5)
			//{
			//	if(getModel()->IsSPIavailabel()==TRUE)
			//	{
			//		getModel()->getDATAHANDLER()->SetSPIErrorCode(ERRC_SPI_READALIVE);
			//		if(AfxGetApp())
			//			AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_SPI);
			//		//getModel()->getALARMHANDLER()->SetAlarm_IF_SPI(_T(""));
			//		//LogError(_T("SPI: Error while Read_ALIVE_BYTE"));
			//	}

			//}
		}
	}
	return iResult;
}

// **************************************************************************
// 
// **************************************************************************
void CInterfaceSPI::Send_MODE_OPTION2(WORD wMode)
{
#ifndef SIMULATION_NOSPI
	SPISENDMESSAGE* pMessage = new SPISENDMESSAGE();
	pMessage->byteSPICommand[0]=S_MODE_OPTION2;

	pMessage->iValue=wMode;

	SendSPICommand(pMessage);
#endif

}



// **************************************************************************
// 
// **************************************************************************
CStringW CInterfaceSPI::GetDriverInfo()
{
	CStringW szDrive;
	DRIVER_INFO cInfo;
	//if (!DeviceIoControl(hDevice, IOCTL_DRIVER_GETINFO, NULL, 0,
	//	&cInfo, sizeof(cInfo), NULL, NULL))
	//{
	//	cInfo.wVerMajor = 1; /* Command failed: this is V1.x */
	//	cInfo.wVerMinor = 0;
	//}
	//printf("NSPI driver V%d.%d", cInfo.wVerMajor, cInfo.wVerMinor);

	EnterCriticalSection(&csSPIFile);

	if(!DeviceIoControl(m_hSPIFile, IOCTL_DRIVER_GETINFO, NULL, 0,&cInfo, sizeof(cInfo), NULL, NULL))
	{
			cInfo.wVerMajor = 1; /* Command failed: this is V1.x */
			cInfo.wVerMinor = 0;
	}

	LeaveCriticalSection(&csSPIFile);

	szDrive.Format(_T("V%d.%d"), cInfo.wVerMajor,cInfo.wVerMinor);

	return szDrive;

}

//************************************
// Method:    activateMANBREATH
// FullName:  CInterfaceSPI::activateMANBREATH
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void CInterfaceSPI::activateMANBREATH()
{
	m_bManBreathRunning=true;
}

//************************************
// Method:    deactivateMANBREATH
// FullName:  CInterfaceSPI::deactivateMANBREATH
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void CInterfaceSPI::deactivateMANBREATH()
{
	if(m_bManBreathRunning==true)
	{
		m_dwWaittimeATZ=GetTickCount();
	}

	m_bManBreathRunning=false;
}

bool CInterfaceSPI::isSPIrunning()
{
	return m_bSPIrunning;
}