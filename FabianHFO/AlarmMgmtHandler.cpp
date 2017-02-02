// AlarmingHandler.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "MVModel.h"
#include "MVViewHandler.h"
#include "TlsRegistry.h"
#include "TlsFile.h"
#include "acuLink.h"
#include "AlarmMgmtHandler.h"
#include "AlarmConditionHandler.h"
#include "Alarm.h"


extern CEvent g_eventNewAlarmData;
CAlarmMgmtHandler* CAlarmMgmtHandler::theAlarmHandler=0;
CAlarmConditionHandler* CAlarmMgmtHandler::ALARMLIMITS=0;
Logfile *CAlarmMgmtHandler::m_AlarmLogfile=NULL;

CAlarmPtr CAlarmMgmtHandler::ALARM_NONE=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Accu_Empty=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysFail_ChecksumConPIC=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysFail_ChecksumMonPIC=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysFail_RELAIS_DEFECT=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysFail_P_IN_MIXER=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysFail_VOLTAGE=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysFail_IF_SPI=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysFail_IF_DIO=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysFail_IF_COM=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysFail_IF_I2C=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysFail_IF_PIF=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysFail_OUTOFMEMORY=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysFail_Fan=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysFail_MIXER_AVCAL=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Accu_Defect=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysAl_P_IN_O2=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysAl_P_IN_AIR=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysAl_TUBE_OCCLUSION=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_DISCONNECTION=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_TUBUSBLOCKED=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Sens_FLOW_SENSOR_DEFECT=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Sens_FLOW_SENSOR_CLEANING=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Sens_FLOW_SENSOR_NOTCONNECTED=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Sens_O2_SENSOR_DEFECT=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Sens_O2_SENSOR_USED=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Sens_O2_VALUE_INCORRECT=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Sens_O2_NOT_CALIBRATED=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Sens_PRICO_FiO2outOfRange=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Sens_CO2_MODULE_NOTCONNECTED=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Sens_CO2_FILTERLINE_NOTCONNECTED=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Sens_CO2_CHECKSAMPLINGLINE=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Sens_CO2_CHECKAIRWAYADAPTER=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Sens_CO2_SENSORFAULTY=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Sens_SPO2_MODULE_NOTCONNECTED=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Sens_SPO2_SENSORFAULTY=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Sens_SPO2_CHECKSENSOR=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_SPO2_SIQmin=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PRICO_FiO2max=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PRICO_FiO2min=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_MVmax=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_MVmin=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_PIPmax=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_PIPmin=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_PEEPminLow=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_PEEPminHigh=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_BPMmax=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_Leakmax=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_Apnoe=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_DCO2max=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_DCO2min=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_ETCO2max=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_ETCO2min=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_FICO2max=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_FICO2min=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_SPO2max=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_SPO2min=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_PulseRatemax=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_PulseRatemin=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_SPO2_PImin=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysLimit_Pinsp_NotReached=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysLimit_Vlimitted=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_SysLimit_Vgaranty=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Accu_POWER=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Accu_60=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Accu_30=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_Accu_15=NULL;


/**=================================================================================================
 * \fn CAlarmMgmtHandler::CAlarmMgmtHandler()
 *
 * \brief Initializes a new instance of the CAlarmMgmtHandler class.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *===============================================================================================**/
CAlarmMgmtHandler::CAlarmMgmtHandler()
{
	InitializeCriticalSection(&csPRICOautoOffState);
	InitializeCriticalSection(&csActiveAlarm);
	InitializeCriticalSection(&csAlarmSilentState);
	InitializeCriticalSection(&csPrioActiveAlarm);
	InitializeCriticalSection(&csVGVLautoOffState);
	InitializeCriticalSection(&csActiveVentMode);
	InitializeCriticalSection(&csAlarmList);

	m_AlarmLoglist = new CTlsLoglist(105);

	CStringA aPathLog = "";
	CStringW wPathLog = _T("\\sdcard\\");
	if(CTlsFile::Exists(wPathLog)==true)
	{
		aPathLog = "\\sdcard\\Logs\\alarmlog.txt";
	}
	else
	{
		aPathLog = "\\FFSDISK\\Logs\\alarmlog.txt";
	}

	m_AlarmLogfile = new Logfile(aPathLog);

	m_bVgarantAutoTurnedOff_FOT=false;
	m_bVgarantAutoTurnedOff_FlowSensor=false;
	m_bVgarantAutoTurnedOff_Tube=false;

	m_bVlimitAutoTurnedOff_FlowSensor=false;
	m_bVlimitAutoTurnedOff_Tube=false;

	m_bPRICOAutoTurnedOff=false;
	m_bPRICOAutoTurnOn=false;
	m_ePRICOAutoAlarmPriority=AP_NONE;

	m_eAlarmSilentState=ASTATE_SYSTEM_SILENT;
	m_eActivealarm=AL_NONE;
	m_ePrioActiveAlarm=AP_NONE;
	m_eActiveVentMode=VM_NONE;

	m_bStartupSilent=true;

	m_szPIPminAlarmText=_T("");
	m_szPIPmaxAlarmText=_T("");
	m_szPEEPminLowAlarmText=_T("");
	m_szPEEPminHighAlarmText=_T("");
	m_iPEEPminLowAlarmValue=-999;
	m_iPEEPminHighAlarmValue=-999;

	m_dwPminHighValueOk=GetTickCount();

	m_pModel=NULL;
	
	m_pAlarmlist=NULL;
}

/**=================================================================================================
 * \fn CAlarmMgmtHandler::~CAlarmMgmtHandler()
 *
 * \brief Finalizes an instance of the CAlarmMgmtHandler class.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *===============================================================================================**/
CAlarmMgmtHandler::~CAlarmMgmtHandler()
{
	if(ALARM_NONE)
		delete ALARM_NONE;
	ALARM_NONE=NULL;
	if(ALARM_Accu_Empty)
		delete ALARM_Accu_Empty;
	ALARM_Accu_Empty=NULL;
	if(ALARM_SysFail_ChecksumConPIC)
		delete ALARM_SysFail_ChecksumConPIC;
	ALARM_SysFail_ChecksumConPIC=NULL;
	if(ALARM_SysFail_ChecksumMonPIC)
		delete ALARM_SysFail_ChecksumMonPIC;
	ALARM_SysFail_ChecksumMonPIC=NULL;
	if(ALARM_SysFail_RELAIS_DEFECT)
		delete ALARM_SysFail_RELAIS_DEFECT;
	ALARM_SysFail_RELAIS_DEFECT=NULL;
	if(ALARM_SysFail_P_IN_MIXER)
		delete ALARM_SysFail_P_IN_MIXER;
	ALARM_SysFail_P_IN_MIXER=NULL;
	if(ALARM_SysFail_VOLTAGE)
		delete ALARM_SysFail_VOLTAGE;
	ALARM_SysFail_VOLTAGE=NULL;
	if(ALARM_SysFail_IF_SPI)
		delete ALARM_SysFail_IF_SPI;
	ALARM_SysFail_IF_SPI=NULL;
	if(ALARM_SysFail_IF_DIO)
		delete ALARM_SysFail_IF_DIO;
	ALARM_SysFail_IF_DIO=NULL;
	if(ALARM_SysFail_IF_COM)
		delete ALARM_SysFail_IF_COM;
	ALARM_SysFail_IF_COM=NULL;
	if(ALARM_SysFail_IF_I2C)
		delete ALARM_SysFail_IF_I2C;
	ALARM_SysFail_IF_I2C=NULL;
	if(ALARM_SysFail_IF_PIF)
		delete ALARM_SysFail_IF_PIF;
	ALARM_SysFail_IF_PIF=NULL;
	if(ALARM_SysFail_OUTOFMEMORY)
		delete ALARM_SysFail_OUTOFMEMORY;
	ALARM_SysFail_OUTOFMEMORY=NULL;
	if(ALARM_SysFail_Fan)
		delete ALARM_SysFail_Fan;
	ALARM_SysFail_Fan=NULL;
	if(ALARM_SysFail_MIXER_AVCAL)
		delete ALARM_SysFail_MIXER_AVCAL;
	ALARM_SysFail_MIXER_AVCAL=NULL;
	if(ALARM_Accu_Defect)
		delete ALARM_Accu_Defect;
	ALARM_Accu_Defect=NULL;
	if(ALARM_SysAl_P_IN_O2)
		delete ALARM_SysAl_P_IN_O2;
	ALARM_SysAl_P_IN_O2=NULL;
	if(ALARM_SysAl_P_IN_AIR)
		delete ALARM_SysAl_P_IN_AIR;
	ALARM_SysAl_P_IN_AIR=NULL;
	if(ALARM_SysAl_TUBE_OCCLUSION)
		delete ALARM_SysAl_TUBE_OCCLUSION;
	ALARM_SysAl_TUBE_OCCLUSION=NULL;
	if(ALARM_DISCONNECTION)
		delete ALARM_DISCONNECTION;
	ALARM_DISCONNECTION=NULL;
	if(ALARM_TUBUSBLOCKED)
		delete ALARM_TUBUSBLOCKED;
	ALARM_TUBUSBLOCKED=NULL;
	if(ALARM_Sens_FLOW_SENSOR_DEFECT)
		delete ALARM_Sens_FLOW_SENSOR_DEFECT;
	ALARM_Sens_FLOW_SENSOR_DEFECT=NULL;
	if(ALARM_Sens_FLOW_SENSOR_CLEANING)
		delete ALARM_Sens_FLOW_SENSOR_CLEANING;
	ALARM_Sens_FLOW_SENSOR_CLEANING=NULL;
	if(ALARM_Sens_FLOW_SENSOR_NOTCONNECTED)
		delete ALARM_Sens_FLOW_SENSOR_NOTCONNECTED;
	ALARM_Sens_FLOW_SENSOR_NOTCONNECTED=NULL;
	if(ALARM_Sens_O2_SENSOR_DEFECT)
		delete ALARM_Sens_O2_SENSOR_DEFECT;
	ALARM_Sens_O2_SENSOR_DEFECT=NULL;
	if(ALARM_Sens_O2_SENSOR_USED)
		delete ALARM_Sens_O2_SENSOR_USED;
	ALARM_Sens_O2_SENSOR_USED=NULL;
	if(ALARM_Sens_O2_VALUE_INCORRECT)
		delete ALARM_Sens_O2_VALUE_INCORRECT;
	ALARM_Sens_O2_VALUE_INCORRECT=NULL;
	if(ALARM_Sens_PRICO_FiO2outOfRange)
		delete ALARM_Sens_PRICO_FiO2outOfRange;
	ALARM_Sens_PRICO_FiO2outOfRange=NULL;
	if(ALARM_Sens_O2_NOT_CALIBRATED)
		delete ALARM_Sens_O2_NOT_CALIBRATED;
	ALARM_Sens_O2_NOT_CALIBRATED=NULL;
	if(ALARM_Sens_CO2_MODULE_NOTCONNECTED)
		delete ALARM_Sens_CO2_MODULE_NOTCONNECTED;
	ALARM_Sens_CO2_MODULE_NOTCONNECTED=NULL;
	if(ALARM_Sens_CO2_FILTERLINE_NOTCONNECTED)
		delete ALARM_Sens_CO2_FILTERLINE_NOTCONNECTED;
	ALARM_Sens_CO2_FILTERLINE_NOTCONNECTED=NULL;
	if(ALARM_Sens_CO2_CHECKSAMPLINGLINE)
		delete ALARM_Sens_CO2_CHECKSAMPLINGLINE;
	ALARM_Sens_CO2_CHECKSAMPLINGLINE=NULL;
	if(ALARM_Sens_CO2_CHECKAIRWAYADAPTER)
		delete ALARM_Sens_CO2_CHECKAIRWAYADAPTER;
	ALARM_Sens_CO2_CHECKAIRWAYADAPTER=NULL;
	if(ALARM_Sens_CO2_SENSORFAULTY)
		delete ALARM_Sens_CO2_SENSORFAULTY;
	ALARM_Sens_CO2_SENSORFAULTY=NULL;
	if(ALARM_Sens_SPO2_MODULE_NOTCONNECTED)
		delete ALARM_Sens_SPO2_MODULE_NOTCONNECTED;
	ALARM_Sens_SPO2_MODULE_NOTCONNECTED=NULL;
	if(ALARM_Sens_SPO2_SENSORFAULTY)
		delete ALARM_Sens_SPO2_SENSORFAULTY;
	ALARM_Sens_SPO2_SENSORFAULTY=NULL;
	if(ALARM_Sens_SPO2_CHECKSENSOR)
		delete ALARM_Sens_SPO2_CHECKSENSOR;
	ALARM_Sens_SPO2_CHECKSENSOR=NULL;
	if(ALARM_PatAl_SPO2_SIQmin)
		delete ALARM_PatAl_SPO2_SIQmin;
	ALARM_PatAl_SPO2_SIQmin=NULL;
	if(ALARM_PRICO_FiO2max)
		delete ALARM_PRICO_FiO2max;
	ALARM_PRICO_FiO2max=NULL;
	if(ALARM_PRICO_FiO2min)
		delete ALARM_PRICO_FiO2min;
	ALARM_PRICO_FiO2min=NULL;
	if(ALARM_PatAl_MVmax)
		delete ALARM_PatAl_MVmax;
	ALARM_PatAl_MVmax=NULL;
	if(ALARM_PatAl_MVmin)
		delete ALARM_PatAl_MVmin;
	ALARM_PatAl_MVmin=NULL;
	if(ALARM_PatAl_PIPmax)
		delete ALARM_PatAl_PIPmax;
	ALARM_PatAl_PIPmax=NULL;
	if(ALARM_PatAl_PIPmin)
		delete ALARM_PatAl_PIPmin;
	ALARM_PatAl_PIPmin=NULL;
	if(ALARM_PatAl_PEEPminLow)
		delete ALARM_PatAl_PEEPminLow;
	ALARM_PatAl_PEEPminLow=NULL;
	if(ALARM_PatAl_PEEPminHigh)
		delete ALARM_PatAl_PEEPminHigh;
	ALARM_PatAl_PEEPminHigh=NULL;
	if(ALARM_PatAl_BPMmax)
		delete ALARM_PatAl_BPMmax;
	ALARM_PatAl_BPMmax=NULL;
	if(ALARM_PatAl_Leakmax)
		delete ALARM_PatAl_Leakmax;
	ALARM_PatAl_Leakmax=NULL;
	if(ALARM_PatAl_Apnoe)
		delete ALARM_PatAl_Apnoe;
	ALARM_PatAl_Apnoe=NULL;
	if(ALARM_PatAl_DCO2max)
		delete ALARM_PatAl_DCO2max;
	ALARM_PatAl_DCO2max=NULL;
	if(ALARM_PatAl_DCO2min)
		delete ALARM_PatAl_DCO2min;
	ALARM_PatAl_DCO2min=NULL;
	if(ALARM_PatAl_ETCO2max)
		delete ALARM_PatAl_ETCO2max;
	ALARM_PatAl_ETCO2max=NULL;
	if(ALARM_PatAl_ETCO2min)
		delete ALARM_PatAl_ETCO2min;
	ALARM_PatAl_ETCO2min=NULL;
	if(ALARM_PatAl_FICO2max)
		delete ALARM_PatAl_FICO2max;
	ALARM_PatAl_FICO2max=NULL;
	if(ALARM_PatAl_FICO2min)
		delete ALARM_PatAl_FICO2min;
	ALARM_PatAl_FICO2min=NULL;
	if(ALARM_PatAl_SPO2max)
		delete ALARM_PatAl_SPO2max;
	ALARM_PatAl_SPO2max=NULL;
	if(ALARM_PatAl_SPO2min)
		delete ALARM_PatAl_SPO2min;
	ALARM_PatAl_SPO2min=NULL;
	if(ALARM_PatAl_PulseRatemax)
		delete ALARM_PatAl_PulseRatemax;
	ALARM_PatAl_PulseRatemax=NULL;
	if(ALARM_PatAl_PulseRatemin)
		delete ALARM_PatAl_PulseRatemin;
	ALARM_PatAl_PulseRatemin=NULL;
	if(ALARM_PatAl_SPO2_PImin)
		delete ALARM_PatAl_SPO2_PImin;
	ALARM_PatAl_SPO2_PImin=NULL;
	if(ALARM_SysLimit_Pinsp_NotReached)
		delete ALARM_SysLimit_Pinsp_NotReached;
	ALARM_SysLimit_Pinsp_NotReached=NULL;
	if(ALARM_SysLimit_Vlimitted)
		delete ALARM_SysLimit_Vlimitted;
	ALARM_SysLimit_Vlimitted=NULL;
	if(ALARM_SysLimit_Vgaranty)
		delete ALARM_SysLimit_Vgaranty;
	ALARM_SysLimit_Vgaranty=NULL;
	if(ALARM_Accu_POWER)
		delete ALARM_Accu_POWER;
	ALARM_Accu_POWER=NULL;
	if(ALARM_Accu_60)
		delete ALARM_Accu_60;
	ALARM_Accu_60=NULL;
	if(ALARM_Accu_30)
		delete ALARM_Accu_30;
	ALARM_Accu_30=NULL;
	if(ALARM_Accu_15)
		delete ALARM_Accu_15;
	ALARM_Accu_15=NULL;

	if(m_pAlarmlist)
	{
		delete m_pAlarmlist;
	}
	m_pAlarmlist=NULL;


	

	if(m_AlarmLoglist)
	{
		delete m_AlarmLoglist;
		m_AlarmLoglist=NULL;
	}
	if(m_AlarmLogfile)
	{
		delete m_AlarmLogfile;
		m_AlarmLogfile=NULL;
	}

	DeleteCriticalSection(&csAlarmSilentState);
	DeleteCriticalSection(&csActiveAlarm);
	DeleteCriticalSection(&csPrioActiveAlarm);
	DeleteCriticalSection(&csVGVLautoOffState);
	DeleteCriticalSection(&csActiveVentMode);
	DeleteCriticalSection(&csAlarmList);
	DeleteCriticalSection(&csPRICOautoOffState);
}

/**=================================================================================================
 * \fn CAlarmMgmtHandler* CAlarmMgmtHandler::getInstance()
 *
 * \brief Gets the instance. Singleton
 *
 * \author Rainer
 * \date 23 Nov 2015
 *
 * \return null if it fails, else the instance.
 *===============================================================================================**/
CAlarmMgmtHandler* CAlarmMgmtHandler::getInstance()
{
	if(theAlarmHandler == 0)
	{
		theAlarmHandler = new CAlarmMgmtHandler;
	}
	return theAlarmHandler;
}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::destroyInstance()
 *
 * \brief Destroys the instance.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *===============================================================================================**/
void CAlarmMgmtHandler::destroyInstance()
{
	if(theAlarmHandler != NULL)
	{
		delete theAlarmHandler;
		theAlarmHandler = NULL;
	}
}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::init()
 *
 * \brief Initialises this object.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *===============================================================================================**/
void CAlarmMgmtHandler::init()
{
	initAlarmList();

	int iDelayTime=getModel()->getCONFIG()->getSPO2alarmdelay();
	if(iDelayTime!=0)
		setSPO2alarmDelay(iDelayTime);

	if(ALARMLIMITS==NULL)
		ALARMLIMITS=new CAlarmConditionHandler();
}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::initAlarmList()
 *
 * \brief Initialises the alarm list.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *===============================================================================================**/
void CAlarmMgmtHandler::initAlarmList()
{
	ALARM_NONE = new CAlarm(AL_NONE,AT_NONE,AP_2,AS_NONE,0);
	ALARM_Accu_Empty = new CAlarm(AL_Accu_Empty,AT_POWER,AP_2,AS_NONE,0);
	ALARM_SysFail_ChecksumConPIC = new CAlarm(AL_SysFail_ChecksumConPIC,AT_SYSFAIL,AP_2,AS_NONE,0);
	ALARM_SysFail_ChecksumMonPIC = new CAlarm(AL_SysFail_ChecksumMonPIC,AT_SYSFAIL,AP_2,AS_NONE,0);
	ALARM_SysFail_RELAIS_DEFECT = new CAlarm(AL_SysFail_RELAIS_DEFECT,AT_SYSFAIL,AP_2,AS_NONE,0);
	ALARM_SysFail_P_IN_MIXER = new CAlarm(AL_SysFail_P_IN_MIXER,AT_SYSFAIL,AP_2,AS_NONE,0);
	ALARM_SysFail_VOLTAGE = new CAlarm(AL_SysFail_VOLTAGE,AT_SYSFAIL,AP_2,AS_NONE,0);
	ALARM_SysFail_IF_SPI = new CAlarm(AL_SysFail_IF_SPI,AT_SYSFAIL,AP_1,AS_NONE,0);
	ALARM_SysFail_IF_DIO = new CAlarm(AL_SysFail_IF_DIO,AT_SYSFAIL,AP_1,AS_NONE,0);
	ALARM_SysFail_IF_COM = new CAlarm(AL_SysFail_IF_COM,AT_SYSFAIL,AP_1,AS_NONE,0);
	ALARM_SysFail_IF_I2C = new CAlarm(AL_SysFail_IF_I2C,AT_SYSFAIL,AP_1,AS_NONE,0);
	ALARM_SysFail_IF_PIF = new CAlarm(AL_SysFail_IF_PIF,AT_SYSFAIL,AP_1,AS_NONE,0);
	ALARM_SysFail_OUTOFMEMORY = new CAlarm(AL_SysFail_OUTOFMEMORY,AT_SYSFAIL,AP_2,AS_NONE,0);
	ALARM_SysFail_Fan = new CAlarm(AL_SysFail_Fan,AT_SYSFAIL,AP_1,AS_NONE,0);
	ALARM_SysFail_MIXER_AVCAL = new CAlarm(AL_SysFail_MIXER_AVCAL,AT_SYSALARM,AP_2,AS_NONE,0);
	ALARM_Accu_Defect = new CAlarm(AL_Accu_Defect,AT_POWER,AP_2,AS_NONE,0);
	ALARM_SysAl_P_IN_O2 = new CAlarm(AL_SysAl_P_IN_O2,AT_SYSALARM,AP_2,AS_NONE,0);
	ALARM_SysAl_P_IN_AIR = new CAlarm(AL_SysAl_P_IN_AIR,AT_SYSALARM,AP_2,AS_NONE,0);
	ALARM_SysAl_TUBE_OCCLUSION = new CAlarm(AL_SysAl_TUBE_OCCLUSION,AT_SYSALARM,AP_2,AS_NONE,0);
	ALARM_DISCONNECTION = new CAlarm(AL_DISCONNECTION,AT_DISCON_TUBUS,AP_3,AS_NONE,0); //new alarm prio 08.2016
	ALARM_TUBUSBLOCKED = new CAlarm(AL_TUBUSBLOCKED,AT_DISCON_TUBUS,AP_4,AS_NONE,0);
	ALARM_Sens_FLOW_SENSOR_DEFECT = new CAlarm(AL_Sens_FLOW_SENSOR_DEFECT,AT_SENSOR,AP_5,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_FLOW_SENSOR_CLEANING = new CAlarm(AL_Sens_FLOW_SENSOR_CLEANING,AT_SENSOR,AP_5,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_FLOW_SENSOR_NOTCONNECTED = new CAlarm(AL_Sens_FLOW_SENSOR_NOTCONNECTED,AT_SENSOR,AP_5,AS_SIGNALED,0); //new alarm prio 08.2016
	ALARM_Sens_O2_SENSOR_DEFECT = new CAlarm(AL_Sens_O2_SENSOR_DEFECT,AT_SENSOR,AP_5,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_O2_SENSOR_USED = new CAlarm(AL_Sens_O2_SENSOR_USED,AT_SENSOR,AP_5,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_O2_VALUE_INCORRECT = new CAlarm(AL_Sens_O2_VALUE_INCORRECT,AT_SENSOR,AP_5,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_O2_NOT_CALIBRATED = new CAlarm(AL_Sens_O2_NOT_CALIBRATED,AT_SENSOR,AP_5,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_PRICO_FiO2outOfRange = new CAlarm(AL_Sens_PRICO_FiO2outOfRange,AT_SENSOR,AP_5,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_CO2_MODULE_NOTCONNECTED = new CAlarm(AL_Sens_CO2_MODULE_NOTCONNECTED,AT_CO2,AP_6,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_CO2_FILTERLINE_NOTCONNECTED = new CAlarm(AL_Sens_CO2_FILTERLINE_NOTCONNECTED,AT_CO2,AP_6,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_CO2_CHECKSAMPLINGLINE = new CAlarm(AL_Sens_CO2_CHECKSAMPLINGLINE,AT_CO2,AP_6,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_CO2_CHECKAIRWAYADAPTER = new CAlarm(AL_Sens_CO2_CHECKAIRWAYADAPTER,AT_CO2,AP_6,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_CO2_SENSORFAULTY = new CAlarm(AL_Sens_CO2_SENSORFAULTY,AT_CO2,AP_6,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_SPO2_MODULE_NOTCONNECTED = new CAlarm(AL_Sens_SPO2_MODULE_NOTCONNECTED,AT_SPO2,AP_6,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_SPO2_SENSORFAULTY = new CAlarm(AL_Sens_SPO2_SENSORFAULTY,AT_SPO2,AP_6,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_SPO2_CHECKSENSOR = new CAlarm(AL_Sens_SPO2_CHECKSENSOR,AT_SPO2,AP_6,AS_NONE,0); //new alarm prio 08.2016
	ALARM_PatAl_SPO2_SIQmin = new CAlarm(AL_PatAl_SPO2_SIQmin,AT_PATIENT,AP_7,AS_NONE,0);
	ALARM_PRICO_FiO2max = new CAlarm(AL_PRICO_FiO2max,AT_PATIENT,AP_7,AS_NONE,0);
	ALARM_PRICO_FiO2min = new CAlarm(AL_PRICO_FiO2min,AT_PATIENT,AP_7,AS_NONE,0);
	ALARM_PatAl_MVmax = new CAlarm(AL_PatAl_MVmax,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_MVmin = new CAlarm(AL_PatAl_MVmin,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_PIPmax = new CAlarm(AL_PatAl_PIPmax,AT_PATIENT,AP_7,AS_NONE,0);
	ALARM_PatAl_PIPmin = new CAlarm(AL_PatAl_PIPmin,AT_PATIENT,AP_7,AS_NONE,0);
	ALARM_PatAl_PEEPminLow = new CAlarm(AL_PatAl_PEEPminLow,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_PEEPminHigh = new CAlarm(AL_PatAl_PEEPminHigh,AT_PATIENT,AP_7,AS_NONE,0);
	ALARM_PatAl_BPMmax = new CAlarm(AL_PatAl_BPMmax,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_Leakmax = new CAlarm(AL_PatAl_Leakmax,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_Apnoe = new CAlarm(AL_PatAl_Apnoe,AT_PATIENT,AP_6,AS_NONE,0); //new alarm prio 08.2016
	ALARM_PatAl_DCO2max = new CAlarm(AL_PatAl_DCO2max,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_DCO2min = new CAlarm(AL_PatAl_DCO2min,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_ETCO2max = new CAlarm(AL_PatAl_ETCO2max,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_ETCO2min = new CAlarm(AL_PatAl_ETCO2min,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_FICO2max = new CAlarm(AL_PatAl_FICO2max,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_FICO2min = new CAlarm(AL_PatAl_FICO2min,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_SPO2max = new CAlarm(AL_PatAl_SPO2max,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_SPO2min = new CAlarm(AL_PatAl_SPO2min,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_PulseRatemax = new CAlarm(AL_PatAl_PulseRatemax,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_PulseRatemin = new CAlarm(AL_PatAl_PulseRatemin,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_SPO2_PImin = new CAlarm(AL_PatAl_SPO2_PImin,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_SysLimit_Pinsp_NotReached = new CAlarm(AL_SysLimit_Pinsp_NotReached,AT_SYSLIMIT,AP_9,AS_NONE,0);
	ALARM_SysLimit_Vlimitted = new CAlarm(AL_SysLimit_Vlimitted,AT_SYSLIMIT,AP_9,AS_NONE,0);
	ALARM_SysLimit_Vgaranty = new CAlarm(AL_SysLimit_Vgaranty,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_Accu_POWER = new CAlarm(AL_Accu_POWER,AT_POWER,AP_9,AS_NONE,0);
	ALARM_Accu_60 = new CAlarm(AL_Accu_60,AT_POWER,AP_8,AS_NONE,0);
	ALARM_Accu_30 = new CAlarm(AL_Accu_30,AT_POWER,AP_8,AS_NONE,0);
	ALARM_Accu_15 = new CAlarm(AL_Accu_15,AT_POWER,AP_2,AS_NONE,0);

	EnterCriticalSection(&csAlarmList);
	m_pAlarmlist = new CAlarmList();

	if(m_pAlarmlist)
	{
		m_pAlarmlist->appendAlarm(ALARM_NONE);
		m_pAlarmlist->appendAlarm(ALARM_Accu_Empty);
		m_pAlarmlist->appendAlarm(ALARM_SysFail_ChecksumConPIC);
		m_pAlarmlist->appendAlarm(ALARM_SysFail_ChecksumMonPIC);
		m_pAlarmlist->appendAlarm(ALARM_SysFail_RELAIS_DEFECT);
		m_pAlarmlist->appendAlarm(ALARM_SysFail_P_IN_MIXER);
		m_pAlarmlist->appendAlarm(ALARM_SysFail_VOLTAGE);
		m_pAlarmlist->appendAlarm(ALARM_SysFail_IF_SPI);
		m_pAlarmlist->appendAlarm(ALARM_SysFail_IF_DIO);
		m_pAlarmlist->appendAlarm(ALARM_SysFail_IF_COM);
		m_pAlarmlist->appendAlarm(ALARM_SysFail_IF_I2C);
		m_pAlarmlist->appendAlarm(ALARM_SysFail_IF_PIF);
		m_pAlarmlist->appendAlarm(ALARM_SysFail_OUTOFMEMORY);
		m_pAlarmlist->appendAlarm(ALARM_SysFail_Fan);
		m_pAlarmlist->appendAlarm(ALARM_SysFail_MIXER_AVCAL);
		m_pAlarmlist->appendAlarm(ALARM_Accu_Defect);
		m_pAlarmlist->appendAlarm(ALARM_SysAl_P_IN_O2);
		m_pAlarmlist->appendAlarm(ALARM_SysAl_P_IN_AIR);
		m_pAlarmlist->appendAlarm(ALARM_SysAl_TUBE_OCCLUSION);
		m_pAlarmlist->appendAlarm(ALARM_DISCONNECTION);
		m_pAlarmlist->appendAlarm(ALARM_TUBUSBLOCKED);
		m_pAlarmlist->appendAlarm(ALARM_Sens_FLOW_SENSOR_DEFECT);
		m_pAlarmlist->appendAlarm(ALARM_Sens_FLOW_SENSOR_CLEANING);
		m_pAlarmlist->appendAlarm(ALARM_Sens_FLOW_SENSOR_NOTCONNECTED);
		m_pAlarmlist->appendAlarm(ALARM_Sens_O2_SENSOR_DEFECT);
		m_pAlarmlist->appendAlarm(ALARM_Sens_O2_SENSOR_USED);
		m_pAlarmlist->appendAlarm(ALARM_Sens_O2_VALUE_INCORRECT);
		m_pAlarmlist->appendAlarm(ALARM_Sens_O2_NOT_CALIBRATED);
		m_pAlarmlist->appendAlarm(ALARM_Sens_PRICO_FiO2outOfRange);
		m_pAlarmlist->appendAlarm(ALARM_Sens_CO2_MODULE_NOTCONNECTED);
		m_pAlarmlist->appendAlarm(ALARM_Sens_CO2_FILTERLINE_NOTCONNECTED);
		m_pAlarmlist->appendAlarm(ALARM_Sens_CO2_CHECKSAMPLINGLINE);
		m_pAlarmlist->appendAlarm(ALARM_Sens_CO2_CHECKAIRWAYADAPTER);
		m_pAlarmlist->appendAlarm(ALARM_Sens_CO2_SENSORFAULTY);
		m_pAlarmlist->appendAlarm(ALARM_Sens_SPO2_MODULE_NOTCONNECTED);
		m_pAlarmlist->appendAlarm(ALARM_Sens_SPO2_SENSORFAULTY);
		m_pAlarmlist->appendAlarm(ALARM_Sens_SPO2_CHECKSENSOR);
		m_pAlarmlist->appendAlarm(ALARM_PRICO_FiO2max);
		m_pAlarmlist->appendAlarm(ALARM_PRICO_FiO2min);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_SPO2_SIQmin);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_MVmax);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_MVmin);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_PIPmax);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_PIPmin);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_PEEPminLow);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_PEEPminHigh);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_BPMmax);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_Leakmax);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_Apnoe);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_DCO2max);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_DCO2min);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_ETCO2max);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_ETCO2min);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_FICO2max);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_FICO2min);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_SPO2max);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_SPO2min);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_PulseRatemax);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_PulseRatemin);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_SPO2_PImin);
		m_pAlarmlist->appendAlarm(ALARM_SysLimit_Pinsp_NotReached);
		m_pAlarmlist->appendAlarm(ALARM_SysLimit_Vlimitted);
		m_pAlarmlist->appendAlarm(ALARM_SysLimit_Vgaranty);
		m_pAlarmlist->appendAlarm(ALARM_Accu_POWER);
		m_pAlarmlist->appendAlarm(ALARM_Accu_60);
		m_pAlarmlist->appendAlarm(ALARM_Accu_30);
		m_pAlarmlist->appendAlarm(ALARM_Accu_15);
	}
	LeaveCriticalSection(&csAlarmList);

}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::deinit()
 *
 * \brief Deinits this object.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *===============================================================================================**/
void CAlarmMgmtHandler::deinit()
{
	if(ALARMLIMITS)
	{
		delete ALARMLIMITS;
	}
	ALARMLIMITS=NULL;

	
	if(m_AlarmLogfile)
		m_AlarmLogfile->WriteLine(L"**** finished ****");
}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::setSPO2alarmDelay(UINT iDelayTime)
 *
 * \brief Sets spo2alarm delay.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *
 * \param iDelayTime The delay time.
 *===============================================================================================**/
void CAlarmMgmtHandler::setSPO2alarmDelay(UINT iDelayTime)
{
	ALARM_PatAl_SPO2max->setAlarmDelayTimeSec(iDelayTime);
	ALARM_PatAl_SPO2min->setAlarmDelayTimeSec(iDelayTime);
	ALARM_PatAl_PulseRatemax->setAlarmDelayTimeSec(iDelayTime);
	ALARM_PatAl_PulseRatemin->setAlarmDelayTimeSec(iDelayTime);
	ALARM_PatAl_SPO2_PImin->setAlarmDelayTimeSec(iDelayTime);
	ALARM_PatAl_SPO2_SIQmin->setAlarmDelayTimeSec(iDelayTime);
}

/**=================================================================================================
 * \fn CMVModel *CAlarmMgmtHandler::getModel()
 *
 * \brief Gets the model.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *
 * \return null if it fails, else the model.
 *===============================================================================================**/
CMVModel *CAlarmMgmtHandler::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**=================================================================================================
 * \fn CTlsLoglist * CAlarmMgmtHandler::getAlarmLog()
 *
 * \brief Gets alarm log. Singleton
 *
 * \author Rainer
 * \date 23 Nov 2015
 *
 * \return null if it fails, else the alarm log.
 *===============================================================================================**/
CTlsLoglist * CAlarmMgmtHandler::getAlarmLog()
{
	if(m_AlarmLoglist)
	{
		return m_AlarmLoglist;
	}
	else
	{
		CString szError=_T("");
		szError.Format(_T("EXCEPTION: CAlarmingHandler::getAlarmLog()"));
		theApp.ReportException(szError);

		return 0;
	}
}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::setLogTxt(CStringW sz)
 *
 * \brief Sets log text.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *
 * \param sz The size.
 *===============================================================================================**/
void CAlarmMgmtHandler::setLogTxt(CStringW sz)
{
	if(m_AlarmLogfile)
		m_AlarmLogfile->WriteLine(sz);
}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::setLogAlarm(eAlarm enAlarm, CStringW szAlarmTxt)
 *
 * \brief Writes an alarm into logfile and loglist.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *
 * \param enAlarm    The en alarm.
 * \param szAlarmTxt The alarm text.
 *===============================================================================================**/
void CAlarmMgmtHandler::setLogAlarm(eAlarm	enAlarm, CStringW szAlarmTxt)
{
	CStringW szAlarm=_T("");
	if(m_AlarmLoglist)
	{
		szAlarm=getAlarmText(enAlarm);
		szAlarm+=szAlarmTxt;
		m_AlarmLoglist->AddMessageHead(szAlarm);
	}

	if(m_AlarmLogfile)
	{
		szAlarm=getLogTxtAlarm(enAlarm);
		szAlarm+= _T(" ")+ szAlarmTxt;
		m_AlarmLogfile->WriteLine(szAlarm);
	}
}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::checkLogAlarmDate()
 *
 * \brief Check log alarm date.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *===============================================================================================**/
void CAlarmMgmtHandler::checkLogAlarmDate()
{
	if(m_AlarmLogfile)
	{
		m_AlarmLogfile->CheckDate();
	}
}

/**=================================================================================================
 * \fn CStringW CAlarmMgmtHandler::getLogTxtAlarm(eAlarm enAlarm)
 *
 * \brief Gets log text alarm.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *
 * \param enAlarm The en alarm.
 *
 * \return The log text alarm.
 *===============================================================================================**/
CStringW CAlarmMgmtHandler::getLogTxtAlarm(eAlarm enAlarm)
{
	switch(enAlarm)
	{
	case AL_TUBUSBLOCKED:
		{
			CStringW sz=_T("TUBUSBLOCK");

			if(m_bVgarantAutoTurnedOff_Tube)
			{
				sz+=_T(" / V_GARANT_OFF");
			}
			else if(m_bVlimitAutoTurnedOff_Tube)
			{
				sz+=_T(" / V_LIMIT_OFF");
			}
			return sz;
		}
		break;
	case AL_DISCONNECTION:
		{
			return _T("DISCONNECTION");
		}
		break;
	case AL_SysAl_P_IN_O2:
		{
			return _T("PO2IN");
		}
		break;
	case AL_SysAl_P_IN_AIR:
		{
			return _T("PAIRIN");
		}
		break;
	case AL_Sens_FLOW_SENSOR_DEFECT:
		{
			CStringW sz=_T("FLOWSENSOR_DEFECT");

			if(m_bVgarantAutoTurnedOff_FlowSensor)
			{
				sz+=_T(" / V_GARANT_OFF");
			}
			else if(m_bVlimitAutoTurnedOff_FlowSensor)
			{
				sz+=_T(" / V_LIMIT_OFF");
			}
			return sz;
		}
		break;
	case AL_Sens_FLOW_SENSOR_CLEANING:
		{
			CStringW sz=_T("FLOWSENSOR_CLEAN");

			if(m_bVgarantAutoTurnedOff_FlowSensor)
			{
				sz+=_T(" / V_GARANT_OFF");
			}
			else if(m_bVlimitAutoTurnedOff_FlowSensor)
			{
				sz+=_T(" / V_LIMIT_OFF");
			}

			return sz;
		}
		break;
	case AL_Sens_FLOW_SENSOR_NOTCONNECTED:
		{
			CStringW sz=_T("FLOWSENSOR_NOTCON");

			if(m_bVgarantAutoTurnedOff_FlowSensor)
			{
				sz+=_T(" / V_GARANT_OFF");
			}
			else if(m_bVlimitAutoTurnedOff_FlowSensor)
			{
				sz+=_T(" / V_LIMIT_OFF");
			}
			return sz;
		}
		break;
	case AL_Sens_O2_SENSOR_DEFECT:
		{
			return _T("O2SENSOR_DEFECT");
		}
		break;
	case AL_Sens_O2_SENSOR_USED:
		{
			return _T("O2SENSOR_USED");
		}
		break;
	case AL_Sens_O2_NOT_CALIBRATED:
		{
			return _T("O2CALERROR");
		}
		break;
	case AL_Sens_O2_VALUE_INCORRECT:
		{
			return _T("O2_VALUE_INCORRECT");
		}
		break;
	case AL_Sens_PRICO_FiO2outOfRange:
		{
			return _T("PRICO: O2_VALUE_INCORRECT");
		}
		break;
	case AL_SysFail_ChecksumConPIC:
		{
			return _T("CSCONPIC");
		}
		break;
	case AL_SysFail_ChecksumMonPIC:
		{
			return _T("CSMONPIC");
		}
		break;
	case AL_SysFail_IF_SPI:
		{
			return _T("IF_SPI");
		}
		break;
	case AL_SysFail_IF_DIO:
		{
			return _T("IF_DIO");
		}
		break;
	case AL_SysFail_IF_COM:
		{
			return _T("IF_COM");
		}
		break;
	case AL_SysFail_IF_I2C:
		{
			return _T("IF_I2C");
		}
		break;
	case AL_SysFail_IF_PIF:
		{
			return _T("IF_PIF");
		}
		break;
	/*case AL_SysFail_IF_ACULINK:
		{
			return _T("IF_ACULINK");
		}
		break;*/
	case AL_SysFail_Fan:
		{
			return _T("BLOWER_DEFECT");
		}
		break;
	case AL_SysFail_RELAIS_DEFECT:
		{
			return _T("RELAIS_DEFECT");
		}
		break;
	case AL_SysAl_TUBE_OCCLUSION:
		{
			return _T("TUBE_OCCLUSION");
		}
		break;
	case AL_SysFail_P_IN_MIXER:
		{
			return _T("PIN_BLENDER");
		}
		break;
	case AL_SysFail_MIXER_AVCAL:
		{

			int iBLENDERstate=getModel()->getDATAHANDLER()->GetMSTATUS_BlenderPIC();
			CStringW sz=_T("");
			if(iBLENDERstate & BIT8)//AV calibration error
			{
				sz.Format(_T("AV_CAL_FAIL code#%d"),iBLENDERstate);
			}
			else
			{
				sz.Format(_T("CHECK_BLENDER code#%d"),iBLENDERstate);
			}
			return sz;
		}
		break;
	case AL_SysFail_VOLTAGE:
		{
			return _T("VOLTAGE");
		}
		break;
	case AL_SysFail_OUTOFMEMORY:
		{
			return _T("OUTOFMEMORY");
		}
		break;
	case AL_SysLimit_Pinsp_NotReached:
		{
			return _T("PINSP_NOTREACH");
		}
		break;
	case AL_SysLimit_Vlimitted:
		{
			return _T("VLIMIT");
		}
		break;
	case AL_SysLimit_Vgaranty:
		{
			return _T("VGARANTY");
		}
		break;
	case AL_PatAl_MVmax:
		{
			return _T("MVMAX");
		}
		break;
	case AL_PatAl_MVmin:
		{
			return _T("MVMIN");
		}
		break;
	case AL_PatAl_SPO2_SIQmin:
		{
			return _T("SPO2_SIQ");
		}
		break;
	case AL_PRICO_FiO2max:
		{
			return _T("PRICO:FiO2max");
		}
		break;
	case AL_PRICO_FiO2min:
		{
			return _T("PRICO:FiO2min");
		}
		break;
	case AL_PatAl_PIPmax:
		{
			return _T("PIPMAX");
		}
		break;
	case AL_PatAl_PIPmin:
		{
			return _T("PIPMIN");
		}
		break;
	case AL_PatAl_PEEPminLow:
		{
			return _T("PEEP_LOW");
		}
		break;
	case AL_PatAl_PEEPminHigh:
		{
			return _T("PEEP_HIGH");
		}
		break;
	case AL_PatAl_BPMmax:
		{
			return _T("BPMMAX");
		}
		break;
	case AL_PatAl_Leakmax:
		{
			return _T("LEAKMAX");
		}
		break;
	case AL_PatAl_Apnoe:
		{
			return _T("APNOE");
		}
		break;
	case AL_PatAl_DCO2max:
		{
			return _T("DCO2_HI");
		}
		break;
	case AL_PatAl_DCO2min:
		{
			return _T("DCO2_LO");
		}
		break;
	case AL_PatAl_ETCO2max:
		{
			return _T("ETCO2_HI");
		}
		break;
	case AL_PatAl_ETCO2min:
		{
			return _T("ETCO2_LO");
		}
		break;
	case AL_PatAl_FICO2max:
		{
			return _T("FICO2_HI");
		}
		break;
	case AL_PatAl_FICO2min:
		{
			return _T("FICO2_LO");
		}
		break;
	case AL_PatAl_SPO2max:	
		{
			return _T("SPO2_HI");
		}
		break;					
	case AL_PatAl_SPO2min:
		{
			return _T("SPO2_LO");
		}
		break;
	case AL_PatAl_PulseRatemax:	
		{
			return _T("PULSERATE_HI");
		}
		break;					
	case AL_PatAl_PulseRatemin:
		{
			return _T("PULSERATE_LO");
		}
		break;
	case AL_PatAl_SPO2_PImin:
		{
			return _T("SPO2_PI_LO");
		}
		break;
	case AL_Accu_POWER:
		{
			CStringW szText = _T(" / ");
			int iBattStatMinutes=getModel()->getDATAHANDLER()->GetBatteryStatMinutes();

			bool bAdd=false;
			if(iBattStatMinutes<1)
			{

			}
			else if(iBattStatMinutes<15)
			{
				szText+=getModel()->GetLanguageString(IDS_ALARM_AKKU15);
				bAdd=true;
			}
			else if(iBattStatMinutes<30)
			{
				szText+=getModel()->GetLanguageString(IDS_ALARM_AKKU30);
				bAdd=true;
			}
			else if(iBattStatMinutes<60)
			{
				szText+=getModel()->GetLanguageString(IDS_ALARM_AKKU60);
				bAdd=true;
			}
			if(bAdd)
				return _T("POWERFAIL")+ szText;
			else
				return _T("POWERFAIL");
		}
		break;
	case AL_Accu_60:
		{
			return _T("AKKU60");
		}
		break;
	case AL_Accu_30:
		{
			return _T("AKKU30");
		}
		break;
	case AL_Accu_15:
		{
			return _T("AKKU15");
		}
		break;
	case AL_Accu_Empty:
		{
			return _T("FLATBATTERY");
		}
		break;
	case AL_Accu_Defect:
		{
			return _T("ACCUDEFECT");
		}
		break;
	/*case AL_Nebulizer_Disconnection:
		{
			return _T("NEBUL_DISCON / DISABLED");
		}
		break;
	case AL_Nebulizer_SysError:
		{
			return _T("NEBUL_ERROR / DISABLED");
		}
		break;*/
	case AL_Sens_SPO2_MODULE_NOTCONNECTED:
		{
			return _T("SPO2_MODULE_NOTCONNECTED");
		}
		break;
	case AL_Sens_SPO2_SENSORFAULTY:
		{
			return _T("SPO2_SENSORFAULTY");
		}
		break;
	case AL_Sens_SPO2_CHECKSENSOR:
		{
			return _T("SPO2_CHECKSENSOR");
		}
		break;
	case AL_Sens_CO2_MODULE_NOTCONNECTED:
		{
			return _T("CO2_MODULE_NOTCONNECTED");
		}
		break;
	case AL_Sens_CO2_FILTERLINE_NOTCONNECTED:
		{
			return _T("CO2_FILTERLINE_NOTCONNECTED");
		}
		break;
	case AL_Sens_CO2_CHECKSAMPLINGLINE:
		{
			return _T("CO2_CHECKSAMPLINGLINE");
		}
		break;
	case AL_Sens_CO2_CHECKAIRWAYADAPTER:
		{
			if(getModel()->getCONFIG()->GetCO2module()==CO2MODULE_CAPNOSTAT)
				return _T("CO2_CHECKAIRWAYADAPTER");
			else
				return _T("CO2_OCCLUSION");

		}
		break;
	case AL_Sens_CO2_SENSORFAULTY:
		{
			return _T("CO2_SENSORFAULTY");
		}
		break;
	default:
		{
			return _T("- unknown alarm -");
		}
		break;
	}
}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::loadConfig()
 *
 * \brief Loads the configuration.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *===============================================================================================**/
void CAlarmMgmtHandler::loadConfig()
{
	init();
}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::ventModeChanged()
 *
 * \brief Ventilation mode changed.
 * 		  Notification to reload the alarm limits.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *===============================================================================================**/
void CAlarmMgmtHandler::ventModeChanged()
{
	EnterCriticalSection(&csActiveVentMode);
	m_eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	LeaveCriticalSection(&csActiveVentMode);
	
	if(ALARMLIMITS)
	{
		ALARMLIMITS->ventModeChanged();
	}
}

/**=================================================================================================
 * \fn eVentMode CAlarmMgmtHandler::getSafeActiveVentMode()
 *
 * \brief Gets safe active vent mode. Critical Section.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *
 * \return The safe active vent mode.
 *===============================================================================================**/
eVentMode CAlarmMgmtHandler::getSafeActiveVentMode()
{
	eVentMode mode=VM_NONE;
	EnterCriticalSection(&csActiveVentMode);
	mode=m_eActiveVentMode;
	LeaveCriticalSection(&csActiveVentMode);
	return mode;
}

/**=================================================================================================
 * \fn eAlarm CAlarmMgmtHandler::getActiveAlarm()
 *
 * \brief Gets active alarm. Critical Section.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *
 * \return The active alarm.
 *===============================================================================================**/
eAlarm CAlarmMgmtHandler::getActiveAlarm()
{
	EnterCriticalSection(&csActiveAlarm);
	eAlarm alarm=m_eActivealarm;
	LeaveCriticalSection(&csActiveAlarm);
	return alarm;
}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::setActiveAlarm(eAlarm alarm)
 *
 * \brief Sets active alarm. Critical Section.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *
 * \param alarm The alarm.
 *===============================================================================================**/
void CAlarmMgmtHandler::setActiveAlarm(eAlarm alarm)
{
	EnterCriticalSection(&csActiveAlarm);
	m_eActivealarm=alarm;
	LeaveCriticalSection(&csActiveAlarm);
}

/**=================================================================================================
 * \fn eVentSilentState CAlarmMgmtHandler::getAlarmSilentState()
 *
 * \brief Gets alarm silent state. Critical Section.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *
 * \return The alarm silent state.
 *===============================================================================================**/
eVentSilentState CAlarmMgmtHandler::getAlarmSilentState()
{
	EnterCriticalSection(&csAlarmSilentState);
	eVentSilentState state=m_eAlarmSilentState;
	LeaveCriticalSection(&csAlarmSilentState);
	return state;
}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::setAlarmSilentState(eVentSilentState state)
 *
 * \brief Sets alarm silent state. Critical Section.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *
 * \param state The state.
 *===============================================================================================**/
void CAlarmMgmtHandler::setAlarmSilentState(eVentSilentState state)
{
	EnterCriticalSection(&csAlarmSilentState);
	m_eAlarmSilentState=state;
	LeaveCriticalSection(&csAlarmSilentState);
}

/**=================================================================================================
 * \fn eAlarmPrio CAlarmMgmtHandler::getPrioActiveAlarm()
 *
 * \brief Gets prio active alarm. Critical Section.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *
 * \return The prio active alarm.
 *===============================================================================================**/
eAlarmPrio CAlarmMgmtHandler::getPrioActiveAlarm()
{
	EnterCriticalSection(&csPrioActiveAlarm);
	eAlarmPrio prio=m_ePrioActiveAlarm;
	LeaveCriticalSection(&csPrioActiveAlarm);
	return prio;
}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::setPrioActiveAlarm(eAlarmPrio prio)
 *
 * \brief Sets prio active alarm. Critical Section.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *
 * \param prio The prio.
 *===============================================================================================**/
void CAlarmMgmtHandler::setPrioActiveAlarm(eAlarmPrio prio)
{
	EnterCriticalSection(&csPrioActiveAlarm);
	m_ePrioActiveAlarm=prio;
	LeaveCriticalSection(&csPrioActiveAlarm);
}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::setStartupSilent(bool bState)
 *
 * \brief Sets system to startup silent.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *
 * \param bState true to state.
 *===============================================================================================**/
void CAlarmMgmtHandler::setStartupSilent(bool bState)
{
	if(bState)
	{
		getModel()->SetCountDownAlarmSilent(MANUALSILENT_TIME);

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SET_STARTUPALARM_DELAY);
		//DEBUGMSG(TRUE, (TEXT("setStartupSilent true\r\n")));
		setAlarmSilentState(ASTATE_MANSILENT);

		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_MANUAL_ALARMSILENT_ON);
	}
	else
	{
		//DEBUGMSG(TRUE, (TEXT("setStartupSilent false\r\n")));
		setAlarmSilentState(ASTATE_ACTIVE);
	}

	EnterCriticalSection(&csAlarmSilentState);
	m_bStartupSilent=bState;
	LeaveCriticalSection(&csAlarmSilentState);

	updateAlarmSound();
}

/**=================================================================================================
 * \fn bool CAlarmMgmtHandler::getStartupSilent()
 *
 * \brief Gets startup silent.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *
 * \return true if it succeeds, false if it fails.
 *===============================================================================================**/
bool CAlarmMgmtHandler::getStartupSilent()
{
	EnterCriticalSection(&csAlarmSilentState);
	bool bState=m_bStartupSilent;
	LeaveCriticalSection(&csAlarmSilentState);
	return bState;
}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::setAlarmsilentActive()
 *
 * \brief Sets alarmsilent active.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *===============================================================================================**/
void CAlarmMgmtHandler::setAlarmsilentActive()
{
	setStartupSilent(false);
	getModel()->setActiveAlarmDelay(true);
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_SET_ACTIVEALARM_DELAY);

	if(getAlarmSilentState()!=ASTATE_SYSTEM_SILENT)
	{
		if(getAlarmSilentState()==ASTATE_SILENT && getModel()->isSPIavailabel()==FALSE)
		{
			getModel()->setSPIavailability(TRUE);
		}
		setAlarmSilentState(ASTATE_ACTIVE);
		if(AfxGetApp() != NULL)
		{
			AfxGetApp()->GetMainWnd()->PostMessage(WM_MANUAL_ALARMSILENT_OFF);
			AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
			AfxGetApp()->GetMainWnd()->PostMessage(WM_CHECK_LIMITS);
		}
	}
}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::setSystemSilentActive()
 *
 * \brief Sets system silent active.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *===============================================================================================**/
void CAlarmMgmtHandler::setSystemSilentActive()
{
	setStartupSilent(false);

	theApp.getLog()->WriteLine(_T("*** ASTATE_ACTIVE ***"));

	setAlarmSilentState(ASTATE_ACTIVE);

	getModel()->Send_MODE_OPTION1(true,false);

	if(AfxGetApp()->GetMainWnd())
	{
		AfxGetApp()->GetMainWnd()->PostMessage(WM_MANUAL_ALARMSILENT_OFF);
		AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
		AfxGetApp()->GetMainWnd()->PostMessage(WM_CHECK_LIMITS);
	}

	if(getModel()->isSPIavailabel()==FALSE)
	{
		getModel()->setSPIavailability(TRUE);
	}
}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::setSystemSilent()
 *
 * \brief Sets system silent.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *===============================================================================================**/
void CAlarmMgmtHandler::setSystemSilent()
{
	setStartupSilent(false);

	if(getAlarmSilentState()!=ASTATE_SYSTEM_SILENT)
	{
		setAlarmSilentState(ASTATE_SYSTEM_SILENT);
		deleteAllAlarms(true);

		if(getModel()->getCONFIG()->IsNursecallActive())
		{
			getModel()->getDATAHANDLER()->SetNurscallAlarm(false);
		}

		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SYSALARM_OFF);

		getModel()->Send_MODE_OPTION1(true,false);

		theApp.getLog()->WriteLine(_T("*** ASTATE_SYSTEM_SILENT ***"));
	}
}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::setAlarmSilent(bool bDelSysSilent,bool bDelAllAlarms)
 *
 * \brief Sets alarm silent.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *
 * \param bDelSysSilent true to delete the system silent.
 * \param bDelAllAlarms true to delete the alarms.
 *===============================================================================================**/
void CAlarmMgmtHandler::setAlarmSilent(bool bDelSysSilent,bool bDelAllAlarms)
{
	//DEBUGMSG(TRUE, (TEXT("setAlarmSilent\r\n")));
	if(bDelSysSilent)
	{
		eVentSilentState eOldVentSilentState=getAlarmSilentState();

		setAlarmSilentState(ASTATE_SILENT);

		if(eOldVentSilentState==ASTATE_SYSTEM_SILENT)
		{
			getModel()->Send_MODE_OPTION1(true,false);

			if(getModel()->isSPIavailabel()==FALSE)
			{
				getModel()->setSPIavailability(TRUE);
			}
			theApp.getLog()->WriteLine(_T("*** ASTATE_SILENT ***"));
		}

		if(bDelAllAlarms)
		{
			deleteAllAlarms(false);
			if(getModel()->getCONFIG()->IsNursecallActive())
			{
				getModel()->getDATAHANDLER()->SetNurscallAlarm(false);
			}
		}

		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SERVICE_SILENT);
	}
	else if(getAlarmSilentState()!=ASTATE_SYSTEM_SILENT)
	{
		setAlarmSilentState(ASTATE_SILENT);

		if(bDelAllAlarms)
		{
			deleteAllAlarms(false);
			if(getModel()->getCONFIG()->IsNursecallActive())
			{
				getModel()->getDATAHANDLER()->SetNurscallAlarm(false);
			}
		}

		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SERVICE_SILENT);
	}
}

/**=================================================================================================
 * \fn void CAlarmMgmtHandler::setAutoSilent(bool bDelSysSilent, bool bDelSilent, bool bDelManSilent)
 *
 * \brief Sets automatic silent.
 *
 * \author Rainer
 * \date 23 Nov 2015
 *
 * \param bDelSysSilent true to delete the system silent state.
 * \param bDelSilent    true to delete the silent state.
 * \param bDelManSilent true to delete the manual silent state.
 *===============================================================================================**/
void CAlarmMgmtHandler::setAutoSilent(bool bDelSysSilent, bool bDelSilent, bool bDelManSilent)
{
	//DEBUGMSG(TRUE, (TEXT("setAutoSilent\r\n")));

	if(getAlarmSilentState()==ASTATE_SYSTEM_SILENT)
	{
		if(bDelSysSilent)
		{
			eVentSilentState eOldVentSilentState=getAlarmSilentState();

			setAlarmSilentState(ASTATE_AUTOSILENT);

			if(eOldVentSilentState==ASTATE_SYSTEM_SILENT)
			{
				getModel()->Send_MODE_OPTION1(true,false);

				if(getModel()->isSPIavailabel()==FALSE)
				{
					getModel()->setSPIavailability(TRUE);
				}

				theApp.getLog()->WriteLine(_T("*** ASTATE_AUTOSILENT ***"));
			}

			updateAlarmStatesSilentStateDependend();
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_AUTO_ALARMSILENT_ON);
		}
	}
	else if(getAlarmSilentState()==ASTATE_SILENT)
	{
		if(bDelSilent)
		{
			setAlarmSilentState(ASTATE_AUTOSILENT);

			updateAlarmStatesSilentStateDependend();
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_AUTO_ALARMSILENT_ON);

			if(getModel()->isSPIavailabel()==FALSE)
			{
				getModel()->setSPIavailability(TRUE);
			}
		}
	}
	else if(getAlarmSilentState()==ASTATE_MANSILENT)
	{
		if(bDelManSilent)
		{
			setAlarmSilentState(ASTATE_AUTOSILENT);

			updateAlarmStatesSilentStateDependend();
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_AUTO_ALARMSILENT_ON);
		}
		else if(getModel()->GetCountDownAlarmSilent()<30 && getAlarmSilentState()==ASTATE_MANSILENT)
		{
			setAlarmSilentState(ASTATE_AUTOSILENT);

			updateAlarmStatesSilentStateDependend();
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_AUTO_ALARMSILENT_ON);
		}
	}
	else
	{
		setAlarmSilentState(ASTATE_AUTOSILENT);

		updateAlarmStatesSilentStateDependend();
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_AUTO_ALARMSILENT_ON);
	}
}


void CAlarmMgmtHandler::setManualSilent(bool bRefreshTime)
{
	if(getAlarmSilentState()!=ASTATE_SYSTEM_SILENT)
	{
		if(getAlarmSilentState()!=ASTATE_MANSILENT)
		{
			setAlarmSilentState(ASTATE_MANSILENT);

			updateAlarmStatesSilentStateDependend();
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_MANUAL_ALARMSILENT_ON);
		}
		else
		{
			setAlarmSilentState(ASTATE_ACTIVE);

			updateAlarmStatesSilentStateDependend();
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_MANUAL_ALARMSILENT_OFF);
		}
	}
}

void CAlarmMgmtHandler::setCalibrationSilent()
{
	//DEBUGMSG(TRUE, (TEXT("setSystemSilent\r\n")));

	if(		getAlarmSilentState()!=ASTATE_SYSTEM_SILENT
		&&	getModel()->IsCalibrationViewActive()==true)
	{
		if(getModel()->getCONFIG()->IsNursecallActive())
		{
			getModel()->getDATAHANDLER()->SetNurscallAlarm(false);
		}

		setAlarmSilentState(ASTATE_MANSILENT);

		if(getActiveAlarm()!=AL_NONE)
		{
			deleteLowerPrioAlarms(AP_2);
		}

		updateAlarmStatesSilentStateDependend();
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_MANUAL_ALARMSILENT_ON);
	}
}

bool CAlarmMgmtHandler::isManualSilentState()
{
	if(getAlarmSilentState()==ASTATE_MANSILENT)
	{
		return true;
	}
	else
		return false;
}

CStringW CAlarmMgmtHandler::getAlarmText(eAlarm alarm)
{
	CStringW sz = _T("");

	switch(alarm)
	{
	case AL_TUBUSBLOCKED:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_TUBUSBLOCK);

			if(m_bVgarantAutoTurnedOff_Tube)
			{
				sz+=_T("/")
					+getModel()->GetLanguageString(IDS_PARA_V)
					+_T("-")
					+getModel()->GetLanguageString(IDS_PARA_GARANT)
					+_T(" ")
					+getModel()->GetLanguageString(IDS_TXT_OFF);
			}
			else if(m_bVlimitAutoTurnedOff_Tube)
			{
				sz+=_T("/")
					+getModel()->GetLanguageString(IDS_PARA_V)
					+_T("-")
					+getModel()->GetLanguageString(IDS_PARA_LIMIT)
					+_T(" ")
					+getModel()->GetLanguageString(IDS_TXT_OFF);
			}
			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				if(isPRICOAutoTurnedOff())
				{
					sz+=_T("/")
						+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
				}
			}
		}
		break;
	case AL_DISCONNECTION:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_DISCONNECTION);
			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				if(isPRICOAutoTurnedOff())
				{
					sz+=_T("/")
						+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
				}
			}
		}
		break;
	case AL_SysAl_P_IN_O2:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_PO2IN);
			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				if(isPRICOAutoTurnedOff())
				{
					sz+=_T("/")
						+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
				}
			}
		}
		break;
	case AL_SysAl_P_IN_AIR:
		{
			//sz=_T("Eingangsdruck Luftversorgung");
			sz=getModel()->GetLanguageString(IDS_ALARM_PAIRIN);
			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				if(isPRICOAutoTurnedOff())
				{
					sz+=_T("/")
						+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
				}
			}
		}
		break;
	case AL_Sens_FLOW_SENSOR_DEFECT:
		{
			//sz=_T("Flowsensor defekt");
			sz=getModel()->GetLanguageString(IDS_ALARM_FLOWSENSOR_DEFECT);

			if(m_bVgarantAutoTurnedOff_FlowSensor)
			{
				sz+=_T("/")
					+getModel()->GetLanguageString(IDS_PARA_V)
					+_T("-")
					+getModel()->GetLanguageString(IDS_PARA_GARANT)
					+_T(" ")
					+getModel()->GetLanguageString(IDS_TXT_OFF);
			}
			else if(m_bVlimitAutoTurnedOff_FlowSensor)
			{
				sz+=_T("/")
					+getModel()->GetLanguageString(IDS_PARA_V)
					+_T("-")
					+getModel()->GetLanguageString(IDS_PARA_LIMIT)
					+_T(" ")
					+getModel()->GetLanguageString(IDS_TXT_OFF);
			}

			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				if(isPRICOAutoTurnedOff())
				{
					sz+=_T("/")
						+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
				}
			}
		}
		break;
	case AL_Sens_FLOW_SENSOR_CLEANING:
		{
			//sz=_T("Flowsensor reinigen");
			sz=getModel()->GetLanguageString(IDS_ALARM_FLOWSENSOR_CLEAN);

			if(m_bVgarantAutoTurnedOff_FlowSensor)
			{
				sz+=_T("/")
					+getModel()->GetLanguageString(IDS_PARA_V)
					+_T("-")
					+getModel()->GetLanguageString(IDS_PARA_GARANT)
					+_T(" ")
					+getModel()->GetLanguageString(IDS_TXT_OFF);
			}
			else if(m_bVlimitAutoTurnedOff_FlowSensor)
			{
				sz+=_T("/")
					+getModel()->GetLanguageString(IDS_PARA_V)
					+_T("-")
					+getModel()->GetLanguageString(IDS_PARA_LIMIT)
					+_T(" ")
					+getModel()->GetLanguageString(IDS_TXT_OFF);
			}

			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				if(isPRICOAutoTurnedOff())
				{
					sz+=_T("/")
						+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
				}
			}

		}
		break;
	case AL_Sens_FLOW_SENSOR_NOTCONNECTED:
		{
			//sz=_T("Flowsensor nicht angeschlossen");
			sz=getModel()->GetLanguageString(IDS_ALARM_FLOWSENSOR_NOTCON);

			if(m_bVgarantAutoTurnedOff_FlowSensor)
			{
				sz+=_T("/")
					+getModel()->GetLanguageString(IDS_PARA_V)
					+_T("-")
					+getModel()->GetLanguageString(IDS_PARA_GARANT)
					+_T(" ")
					+getModel()->GetLanguageString(IDS_TXT_OFF);
			}
			else if(m_bVlimitAutoTurnedOff_FlowSensor)
			{
				sz+=_T("/")
					+getModel()->GetLanguageString(IDS_PARA_V)
					+_T("-")
					+getModel()->GetLanguageString(IDS_PARA_LIMIT)
					+_T(" ")
					+getModel()->GetLanguageString(IDS_TXT_OFF);
			}

			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				if(isPRICOAutoTurnedOff())
				{
					sz+=_T("/")
						+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
				}
			}
		}
		break;
	case AL_Sens_O2_SENSOR_DEFECT:
		{
			//sz=_T("Sauerstoffsensor defekt");
			sz=getModel()->GetLanguageString(IDS_ALARM_O2SENSOR_DEFECT);
			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				if(isPRICOAutoTurnedOff())
				{
					sz+=_T("/")
						+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
				}
			}
		}
		break;
	case AL_Sens_O2_SENSOR_USED:
		{
			//sz=_T("Sauerstoffsensor verbraucht");
			sz=getModel()->GetLanguageString(IDS_ALARM_O2SENSOR_USED);
			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				if(isPRICOAutoTurnedOff())
				{
					sz+=_T("/")
						+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
				}
			}
		}
		break;
	case AL_Sens_O2_NOT_CALIBRATED:
		{
			//sz=_T("Sauerstoffsensor nicht kalibriert");
			sz=getModel()->GetLanguageString(IDS_ALARM_O2CALERROR);
			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				if(isPRICOAutoTurnedOff())
				{
					sz+=_T("/")
						+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
				}
			}
		}
		break;
	case AL_Sens_O2_VALUE_INCORRECT:
		{
			//sz=_T("Abweichung Sauerstoffwert");
			sz=getModel()->GetLanguageString(IDS_ALARM_O2VALUE);
			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				if(isPRICOAutoTurnedOff())
				{
					sz+=_T("/")
						+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
				}
			}
		}
		break;
	case AL_Sens_PRICO_FiO2outOfRange:
		{
			sz=_T("PRICO: ");
			sz+=getModel()->GetLanguageString(IDS_ALARM_O2VALUE);
			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				if(isPRICOAutoTurnedOff())
				{
					sz+=_T("/")
						+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
				}
			}
		}
		break;
	case AL_SysFail_ChecksumConPIC:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_CSCONPIC);
		}
		break;
	case AL_SysFail_ChecksumMonPIC:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_CSMONPIC);
		}
		break;
	case AL_SysFail_IF_SPI:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_IF_SPI);
		}
		break;
	case AL_SysFail_IF_DIO:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_IF_DIO);
		}
		break;
	case AL_SysFail_IF_COM:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_IF_COM);
		}
		break;
	case AL_SysFail_IF_I2C:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_IF_I2C);
		}
		break;
	case AL_SysFail_IF_PIF:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_IF_PIF);
		}
		break;
	/*case AL_SysFail_IF_ACULINK:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_IF_TERM);
		}
		break;*/
	case AL_SysFail_Fan:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_FAN);
		}
		break;
	case AL_SysFail_RELAIS_DEFECT:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_RELAIS_DEFECT);
		}
		break;
	case AL_SysAl_TUBE_OCCLUSION:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_TUBE_OCCLUSION);
			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				if(isPRICOAutoTurnedOff())
				{
					sz+=_T("/")
						+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
				}
			}
		}
		break;
	case AL_SysFail_P_IN_MIXER:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_PIN_MIXER);
			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				if(isPRICOAutoTurnedOff())
				{
					sz+=_T("/")
						+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
				}
			}
		}
		break;
	case AL_SysFail_MIXER_AVCAL:
		{

			int iBLENDERstate=getModel()->getDATAHANDLER()->GetMSTATUS_BlenderPIC();
			if(iBLENDERstate & BIT8)//AV calibration error
			{
				sz.Format(_T("%s: code#%d"),getModel()->GetLanguageString(IDS_TXT_AV_CAL_FAIL),iBLENDERstate);
			}
			else
			{
				sz.Format(_T("%s: code#%d"),getModel()->GetLanguageString(IDS_TXT_CHECK_MIXER),iBLENDERstate);
			}
		}
		break;
	//case AL_SysFail_OVERPRESSURE:
	//	{
	//		//sz=_T("Überdruck");
	//		sz=getModel()->GetLanguageString(IDS_ALARM_OVERPRESSURE);
	//	}
	//	break;
	case AL_SysFail_VOLTAGE:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_VOLTAGE);
		}
		break;
	case AL_SysFail_OUTOFMEMORY:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_OUTOFMEMORY);
		}
		break;
	case AL_SysLimit_Pinsp_NotReached:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_PINSP_NOTREACH);
		}
		break;
	case AL_SysLimit_Vlimitted:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_VLIMIT);
		}
		break;
	case AL_SysLimit_Vgaranty:
		{
			if(getModel()->getVGarantyAlarmFlag()==VGAL_LOW)
				sz=getModel()->GetLanguageString(IDS_ALARM_VGARANTY);
			/*else
				sz=getModel()->GetLanguageString(IDS_ALARM_VGARANTY_HI);*/
		}
		break;
	case AL_PatAl_MVmax:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_MVMAX);
		}
		break;
	case AL_PatAl_MVmin:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_MVMIN);
		}
		break;
	case AL_PatAl_SPO2_SIQmin:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_PRICOSIQ);
			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				if(isPRICOAutoTurnedOff())
				{
					sz+=_T("/")
						+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
				}
			}
		}
		break;
	case AL_PRICO_FiO2max:
		{
			//sz=_T("FiO2 at max target");
			sz=getModel()->GetLanguageString(IDS_ALARM_FIO2MAX);
			//int iFiO2=CTlsFloat::Round(((double)getModel()->getDATAHANDLER()->getMessureDataO2())/10, 0);//PRICO04
			//if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			//{
			//	if(isPRICOAutoTurnedOff())
			//	{
			//		sz+=_T("/")
			//			+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
			//	}
			//}
		}
		break;
	case AL_PRICO_FiO2min:
		{
			//sz=_T("Lower minimum FiO2");
			sz=getModel()->GetLanguageString(IDS_ALARM_FIO2MIN);
			//if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			//{
			//	if(isPRICOAutoTurnedOff())
			//	{
			//		sz+=_T("/")
			//			+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
			//	}
			//}
		}
		break;
	case AL_PatAl_PIPmax:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_PMAX);
		}
		break;
	case AL_PatAl_PIPmin:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_PIPMIN);
		}
		break;
	case AL_PatAl_PEEPminLow:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_PMIN);
		}
		break;
	case AL_PatAl_PEEPminHigh:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_PMIN_HIGH);
		}
		break;
	case AL_PatAl_BPMmax:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_BPMMAX);
		}
		break;
	case AL_PatAl_Leakmax:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_LEAKMAX);
		}
		break;
	case AL_PatAl_Apnoe:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_APNOE);
		}
		break;
	case AL_PatAl_DCO2max:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_DCO2_HI);
		}
		break;
	case AL_PatAl_DCO2min:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_DCO2_LO);
		}
		break;
	case AL_PatAl_ETCO2max:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_ETCO2_HI);
		}
		break;
	case AL_PatAl_ETCO2min:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_ETCO2_LO);
		}
		break;
	case AL_PatAl_FICO2max:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_FICO2_HI);
		}
		break;
	case AL_PatAl_FICO2min:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_FICO2_LO);
		}
		break;
	case AL_PatAl_SPO2max:	
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_SPO2_HI);
		}
		break;					
	case AL_PatAl_SPO2min:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_SPO2_LO);
		}
		break;
	case AL_PatAl_PulseRatemax:	
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_PULSERATE_HI);
		}
		break;					
	case AL_PatAl_PulseRatemin:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_PULSERATE_LO);
		}
		break;
	case AL_PatAl_SPO2_PImin:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_SPO2_PI_LO);
		}
		break;
	case AL_Accu_POWER:
		{
			CStringW szText = _T(" / ");
			int iBattStatMinutes=getModel()->getDATAHANDLER()->GetBatteryStatMinutes();
			
			bool bAdd=false;
			if(iBattStatMinutes<1)
			{
				if(ALARM_Accu_Empty->getAlarmState()==AS_NONE)
					setAlarm(AL_Accu_Empty);
			}
			else if(iBattStatMinutes<15)
			{
				szText+=getModel()->GetLanguageString(IDS_ALARM_AKKU15);
				bAdd=true;
			}
			else if(iBattStatMinutes<30)
			{
				szText+=getModel()->GetLanguageString(IDS_ALARM_AKKU30);
				bAdd=true;
			}
			else if(iBattStatMinutes<60)
			{
				szText+=getModel()->GetLanguageString(IDS_ALARM_AKKU60);
				bAdd=true;
			}
			if(bAdd)
				sz=getModel()->GetLanguageString(IDS_ALARM_POWERFAIL) + szText;
			else
				sz=getModel()->GetLanguageString(IDS_ALARM_POWERFAIL);
		}
		break;
	case AL_Accu_60:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_AKKU60);
		}
		break;
	case AL_Accu_30:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_AKKU30);
		}
		break;
	case AL_Accu_15:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_AKKU15);
		}
		break;
	case AL_Accu_Empty:
		{
			sz=getModel()->GetLanguageString(IDS_TXT_FLATBATT);
		}
		break;
	case AL_Accu_Defect:
		{
			sz=getModel()->GetLanguageString(IDS_TXT_ACCUDEFECT);
		}
		break;
	/*case AL_Nebulizer_Disconnection:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_NEBUL_DISCON);
			sz+=_T("/");
			sz+=getModel()->GetLanguageString(IDS_TXT_DISABLED);
		}
		break;
	case AL_Nebulizer_SysError:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_NEBUL_ERROR);
			sz+=_T("/");
			sz+=getModel()->GetLanguageString(IDS_TXT_DISABLED);
		}
		break;*/
	case AL_Sens_SPO2_MODULE_NOTCONNECTED:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_SPO2_MODULE_NOTCONNECTED);
			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				if(isPRICOAutoTurnedOff())
				{
					sz+=_T("/")
						+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
				}
			}
		}
		break;
	case AL_Sens_SPO2_SENSORFAULTY:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_SPO2_SENSORFAULTY);
			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				if(isPRICOAutoTurnedOff())
				{
					sz+=_T("/")
						+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
				}
			}
		}
		break;
	case AL_Sens_SPO2_CHECKSENSOR:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_SPO2_CHECKSENSOR);
			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				//DEBUGMSG(TRUE, (TEXT("TEXT AL_Sens_SPO2_CHECKSENSOR\r\n")));
				if(isPRICOAutoTurnedOff())
				{
					sz+=_T("/")
						+getModel()->GetLanguageString(IDS_TXT_PRICOOFF);
				}
			}
		}
		break;
	case AL_Sens_CO2_MODULE_NOTCONNECTED:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_CO2_MODULE_NOTCONNECTED);
		}
		break;
	case AL_Sens_CO2_FILTERLINE_NOTCONNECTED:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_CO2_FILTERLINE_NOTCONNECTED);
		}
		break;
	case AL_Sens_CO2_CHECKSAMPLINGLINE:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_CO2_CHECKSAMPLINGLINE);
		}
		break;
	case AL_Sens_CO2_CHECKAIRWAYADAPTER:
		{
			if(getModel()->getCONFIG()->GetCO2module()==CO2MODULE_CAPNOSTAT)
				sz=getModel()->GetLanguageString(IDS_ALARM_CO2_CHECKAIRWAYADAPTER);
			else
				sz=getModel()->GetLanguageString(IDS_ALARM_CO2_OCCLUSION);
		}
		break;
	case AL_Sens_CO2_SENSORFAULTY:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_CO2_SENSORFAULTY);
		}
		break;
	default:
		break;
	}
	return sz;
}

eAlarmPrio CAlarmMgmtHandler::getAlarmPrio(eAlarm alarm)
{
	switch(alarm)
	{
	case AL_SysFail_IF_SPI:
	case AL_SysFail_IF_DIO:
	case AL_SysFail_IF_COM:
	case AL_SysFail_IF_I2C:
	case AL_SysFail_IF_PIF:
	//case AL_SysFail_IF_ACULINK:
	case AL_SysFail_Fan:
		{
			return AP_1;
		}
		break;
	case AL_SysFail_MIXER_AVCAL:
	case AL_SysFail_RELAIS_DEFECT:				
// 	case AL_SysFail_P_PROXIMAL:					
 	case AL_SysFail_P_IN_MIXER:					
	//case AL_SysFail_OVERPRESSURE:				
	case AL_SysFail_VOLTAGE:					
	case AL_SysFail_OUTOFMEMORY:					
	//case AL_Accu_POWER:
	/*case AL_Accu_60:
	case AL_Accu_30:*/
	case AL_Accu_15:
	case AL_Accu_Empty:
	case AL_Accu_Defect:
	case AL_SysAl_P_IN_O2:					
	case AL_SysAl_P_IN_AIR:					
	//case AL_SysAl_P_EXSPIRATIONTUBE:			
	//case AL_SysAl_P_INSPIRATIONTUBE:			
	case AL_SysAl_TUBE_OCCLUSION:			
	case AL_SysFail_ChecksumConPIC:
	case AL_SysFail_ChecksumMonPIC:
		{
			return AP_2;
		}
		break;
	/*case AL_Nebulizer_Disconnection:
	case AL_Nebulizer_SysError:*/
	case AL_DISCONNECTION: //new alarm prio 08.2016
		{
			return AP_3;
		}
		break;
	case AL_TUBUSBLOCKED:
		{
			return AP_4;
		}
		break;
	//case AL_SysFail_P_PROXIMAL:
	// new alarm prio 08.2016
	case AL_Sens_FLOW_SENSOR_DEFECT:			
	case AL_Sens_FLOW_SENSOR_CLEANING:		
	case AL_Sens_FLOW_SENSOR_NOTCONNECTED:	
	case AL_Sens_O2_SENSOR_DEFECT:			
	case AL_Sens_O2_SENSOR_USED:				
	case AL_Sens_O2_VALUE_INCORRECT:	
	case AL_Sens_O2_NOT_CALIBRATED:
	case AL_Sens_PRICO_FiO2outOfRange:
		{
			return AP_5;
		}
		break;
	case AL_PatAl_Apnoe: //new alarm prio 08.2016
		{
			return AP_6;
		}
		break;	
	//new alarm prio 08.2016
	case AL_Sens_CO2_MODULE_NOTCONNECTED:
	case AL_Sens_CO2_FILTERLINE_NOTCONNECTED:
	case AL_Sens_CO2_CHECKSAMPLINGLINE:
	case AL_Sens_CO2_CHECKAIRWAYADAPTER:
	case AL_Sens_CO2_SENSORFAULTY:
	case AL_Sens_SPO2_MODULE_NOTCONNECTED:
	case AL_Sens_SPO2_SENSORFAULTY:
	case AL_Sens_SPO2_CHECKSENSOR:
		{
			return AP_6;
		}
		break;
	case AL_PatAl_SPO2_SIQmin:
	case AL_PRICO_FiO2max:
	case AL_PRICO_FiO2min:
	case AL_PatAl_PIPmax:	
	case AL_PatAl_PEEPminHigh:
		{
			return AP_7;
		}
		break;
	case AL_PatAl_PIPmin:
	case AL_PatAl_MVmax:						
	case AL_PatAl_MVmin:						
	case AL_PatAl_PEEPminLow:
	case AL_PatAl_BPMmax:						
	case AL_PatAl_Leakmax:
	case AL_PatAl_DCO2max:						
	case AL_PatAl_DCO2min:
	case AL_PatAl_ETCO2max:						
	case AL_PatAl_ETCO2min:
	case AL_PatAl_FICO2max:						
	case AL_PatAl_FICO2min:
	case AL_PatAl_SPO2max:						
	case AL_PatAl_SPO2min:
	case AL_PatAl_PulseRatemax:						
	case AL_PatAl_PulseRatemin:
	case AL_PatAl_SPO2_PImin:
	case AL_Accu_60:
	case AL_Accu_30:
	case AL_SysLimit_Vgaranty:
		{
			return AP_8;
		}
		break;
	case AL_SysLimit_Vlimitted:	
	case AL_SysLimit_Pinsp_NotReached:
	case AL_Accu_POWER:
		{
			return AP_9;
		}
		break;
	default:
		return AP_NONE;
		break;
	}
}

eAlarmType CAlarmMgmtHandler::getAlarmType(eAlarm alarm)
{
	switch(alarm)
	{
	case AL_Accu_Empty:
	case AL_Accu_Defect:
		{
			return AT_POWER;
		}
		break;
	case AL_SysFail_ChecksumConPIC:
	case AL_SysFail_ChecksumMonPIC:
	case AL_SysFail_IF_SPI:
	case AL_SysFail_IF_DIO:
	case AL_SysFail_IF_COM:
	case AL_SysFail_IF_I2C:
	case AL_SysFail_IF_PIF:
	//case AL_SysFail_IF_ACULINK:
	case AL_SysFail_RELAIS_DEFECT:
	case AL_SysFail_P_IN_MIXER:
		//case AL_SysFail_OVERPRESSURE:
	case AL_SysFail_VOLTAGE:
	case AL_SysFail_OUTOFMEMORY:
	case AL_SysFail_Fan:
		{
			return AT_SYSFAIL;
		}
		break;
	case AL_SysFail_MIXER_AVCAL:
	case AL_SysAl_TUBE_OCCLUSION:
	case AL_SysAl_P_IN_O2:
	case AL_SysAl_P_IN_AIR:
		//case AL_SysAl_P_EXSPIRATIONTUBE:
		//case AL_SysAl_P_INSPIRATIONTUBE:
		{
			return AT_SYSALARM;
		}
		break;
	case AL_DISCONNECTION:
	case AL_TUBUSBLOCKED:
		{
			return AT_DISCON_TUBUS;
		}
		break;
	case AL_Sens_FLOW_SENSOR_DEFECT:
	case AL_Sens_FLOW_SENSOR_CLEANING:
	case AL_Sens_FLOW_SENSOR_NOTCONNECTED:
	case AL_Sens_O2_SENSOR_DEFECT:
	case AL_Sens_O2_SENSOR_USED:
	case AL_Sens_O2_VALUE_INCORRECT:
	case AL_Sens_O2_NOT_CALIBRATED:
	case AL_Sens_PRICO_FiO2outOfRange:
		{
			return AT_SENSOR;
		}
		break;
	case AL_Sens_CO2_MODULE_NOTCONNECTED:
	case AL_Sens_CO2_FILTERLINE_NOTCONNECTED:
	case AL_Sens_CO2_CHECKSAMPLINGLINE:
	case AL_Sens_CO2_CHECKAIRWAYADAPTER:
	case AL_Sens_CO2_SENSORFAULTY:
		{
			return AT_CO2;
		}
		break;
	case AL_Sens_SPO2_MODULE_NOTCONNECTED:
	case AL_Sens_SPO2_SENSORFAULTY:
	case AL_Sens_SPO2_CHECKSENSOR:
		{
			return AT_SPO2;
		}
		break;
	case AL_PatAl_SPO2_SIQmin:
	case AL_PRICO_FiO2max:
	case AL_PRICO_FiO2min:
	case AL_PatAl_MVmax:
	case AL_PatAl_MVmin:
	case AL_PatAl_PIPmax:
	case AL_PatAl_PIPmin:
	case AL_PatAl_PEEPminLow:
	case AL_PatAl_PEEPminHigh:
	case AL_PatAl_BPMmax:
	case AL_PatAl_Leakmax:
	case AL_PatAl_Apnoe:
	case AL_PatAl_DCO2max:						
	case AL_PatAl_DCO2min:
	case AL_PatAl_ETCO2max:						
	case AL_PatAl_ETCO2min:
	case AL_PatAl_FICO2max:						
	case AL_PatAl_FICO2min:
	case AL_PatAl_SPO2max:						
	case AL_PatAl_SPO2min:
	case AL_PatAl_PulseRatemax:						
	case AL_PatAl_PulseRatemin:
	case AL_PatAl_SPO2_PImin:						
	case AL_SysLimit_Vgaranty:
		{
			return AT_PATIENT;
		}
		break;
	case AL_SysLimit_Pinsp_NotReached:
	case AL_SysLimit_Vlimitted:
		{
			return AT_SYSLIMIT;
		}
		break;
	case AL_Accu_POWER:
	case AL_Accu_60:
	case AL_Accu_30:
	case AL_Accu_15:
		{
			return AT_POWER;
		}
		break;
	default:
		return AT_NONE;
		break;
	}
}

eAlarm CAlarmMgmtHandler::getSignaledAlarm()
{
	eAlarm actAlarm=AL_NONE;
	eAlarmPrio prioAlarm=AP_NONE;
	//DWORD dwTimeStamp=GetTickCount();
	DWORD dwTimeStamp=0;

	EnterCriticalSection(&csAlarmList);
	UINT iCntList=m_pAlarmlist->getCount();
	for(UINT i=0;i<iCntList;i++)
	{
		CAlarm* cAlarm = m_pAlarmlist->getAlarm((eAlarm)i);
		if(cAlarm->getAlarmState()==AS_SIGNALED && cAlarm->getAlarmPriority()<prioAlarm)//higher priority?
		{
			actAlarm = cAlarm->getAlarm();
			prioAlarm = cAlarm->getAlarmPriority();
			dwTimeStamp = cAlarm->getTriggerTimestamp();
			//DEBUGMSG(TRUE, (TEXT("getSignaledAlarm1: %d %d %d\r\n"),(int)actAlarm,(int)prioAlarm, (int)dwTimeStamp));
		}
		else if(cAlarm->getAlarmState()==AS_ACTIVE 
			&& cAlarm->getAlarmPriority()==prioAlarm 
			&& cAlarm->getTriggerTimestamp()<dwTimeStamp)//same priority but first triggered
		{
			actAlarm=cAlarm->getAlarm();
			prioAlarm = cAlarm->getAlarmPriority();
			dwTimeStamp = cAlarm->getTriggerTimestamp();
			//DEBUGMSG(TRUE, (TEXT("getSignaledAlarm2: %d %d &d\r\n"),(int)actAlarm,(int)prioAlarm, (int)dwTimeStamp));
		}
	}
	LeaveCriticalSection(&csAlarmList);
	//DEBUGMSG(TRUE, (TEXT("getSignaledAlarm\r\n")));


	return actAlarm;
}



bool CAlarmMgmtHandler::setAlarm(eAlarm alarm, CStringW alarmTxt)
{
	bool bRes=false;
	switch(alarm)
	{
	case AL_TUBUSBLOCKED:
		{
			bRes=SetAlarm_TUBUSBLOCKED(alarmTxt);
		}
		break;
	case AL_DISCONNECTION:
		{
			bRes=SetAlarm_DISCONNECTION(alarmTxt);
		}
		break;
	case AL_SysAl_P_IN_O2:
		{
			bRes=SetAlarm_P_IN_O2(alarmTxt);
		}
		break;
	case AL_SysAl_P_IN_AIR:
		{
			bRes=SetAlarm_P_IN_AIR(alarmTxt);
		}
		break;
	/*case AL_SysAl_P_EXSPIRATIONTUBE:
		{
			bRes=SetAlarm_P_EXSPIRATIONTUBE(alarmTxt);
		}
		break;
	case AL_SysAl_P_INSPIRATIONTUBE:
		{
			bRes=SetAlarm_P_INSPIRATIONTUBE(alarmTxt);
		}
		break;*/
	case AL_Sens_FLOW_SENSOR_DEFECT:
		{
			bRes=SetAlarm_FLOW_SENSOR_DEFECT(alarmTxt);
		}
		break;
	case AL_Sens_FLOW_SENSOR_CLEANING:
		{
			bRes=SetAlarm_FLOW_SENSOR_CLEANING(alarmTxt);
		}
		break;
	case AL_Sens_FLOW_SENSOR_NOTCONNECTED:
		{
			bRes=SetAlarm_FLOW_SENSOR_NOTCONNECTED(alarmTxt);
		}
		break;
	/*case AL_Nebulizer_Disconnection:
		{
			bRes=SetAlarm_NEBULIZER_DISCONNECTION(alarmTxt);
		}
		break;
	case AL_Nebulizer_SysError:
		{
			bRes=SetAlarm_NEBULIZER_SYSERROR(alarmTxt);
		}
		break;*/
	case AL_Sens_O2_SENSOR_DEFECT:
		{
			bRes=SetAlarm_O2_SENSOR_DEFECT(alarmTxt);
		}
		break;
	case AL_Sens_O2_SENSOR_USED:
		{
			bRes=SetAlarm_O2_SENSOR_USED(alarmTxt);
		}
		break;
	case AL_Sens_O2_NOT_CALIBRATED:
		{
			bRes=SetAlarm_O2_NOT_CALIBRATED(alarmTxt);
		}
		break;
	case AL_Sens_O2_VALUE_INCORRECT:
		{
			bRes=SetAlarm_O2_VALUE_INCORRECT(alarmTxt);
		}
		break;
	case AL_Sens_PRICO_FiO2outOfRange:
		{
			bRes=SetAlarm_PRICO_FiO2outOfRange(alarmTxt);
		}
		break;
	case AL_Sens_SPO2_MODULE_NOTCONNECTED:
		{
			//DEBUGMSG(TRUE, (TEXT("AL_Sens_SPO2_MODULE_NOTCONNECTED\r\n")));
			bRes=SetAlarm_SPO2_MODULE_NOTCONNECTED(alarmTxt);
		}
		break;
	case AL_Sens_SPO2_SENSORFAULTY:
		{
			//DEBUGMSG(TRUE, (TEXT("AL_Sens_SPO2_SENSORFAULTY\r\n")));
			bRes=SetAlarm_SPO2_SENSORFAULTY(alarmTxt);
		}
		break;
	case AL_Sens_SPO2_CHECKSENSOR:
		{
			//DEBUGMSG(TRUE, (TEXT("AL_Sens_SPO2_CHECKSENSOR\r\n")));
			bRes=SetAlarm_SPO2_CHECKSENSOR(alarmTxt);
		}
		break;
	case AL_Sens_CO2_MODULE_NOTCONNECTED:
		{
			bRes=SetAlarm_CO2_MODULE_NOTCONNECTED(alarmTxt);
		}
		break;
	case AL_Sens_CO2_FILTERLINE_NOTCONNECTED:
		{
			bRes=SetAlarm_CO2_FILTERLINE_NOTCONNECTED(alarmTxt);
		}
		break;
	case AL_Sens_CO2_CHECKSAMPLINGLINE:
		{
			bRes=SetAlarm_CO2_CHECKSAMPLINGLINE(alarmTxt);
		}
		break;
	case AL_Sens_CO2_CHECKAIRWAYADAPTER:
		{
			bRes=SetAlarm_CO2_CHECKAIRWAYADAPTER(alarmTxt);
		}
		break;
	case AL_Sens_CO2_SENSORFAULTY:
		{
			bRes=SetAlarm_CO2_SENSORFAULTY(alarmTxt);
		}
		break;
	case AL_SysFail_ChecksumConPIC:
		{
			bRes=SetAlarm_ChecksumConPIC(alarmTxt);
		}
		break;
	case AL_SysFail_ChecksumMonPIC:
		{
			bRes=SetAlarm_ChecksumMonPIC(alarmTxt);
		}
		break;
	case AL_SysFail_IF_SPI:
		{
			bRes=SetAlarm_IF_SPI(alarmTxt);
		}
		break;
	case AL_SysFail_IF_DIO:
		{
			bRes=SetAlarm_IF_DIO(alarmTxt);
		}
		break;
	case AL_SysFail_IF_COM:
		{
			 bRes=SetAlarm_IF_COM(alarmTxt);
		}
		break;
	case AL_SysFail_IF_I2C:
		{
			bRes=SetAlarm_IF_I2C(alarmTxt);
		}
		break;
	case AL_SysFail_IF_PIF:
		{
			bRes=SetAlarm_IF_PIF(alarmTxt);
		}
		break;
	/*case AL_SysFail_IF_ACULINK:
		{
			bRes=SetAlarm_IF_ACULINK(alarmTxt);
		}
		break;*/
	case AL_SysFail_Fan:
		{
			bRes=SetAlarm_Fan(alarmTxt);
		}
		break;
	case AL_SysFail_RELAIS_DEFECT:
		{
			bRes=SetAlarm_RELAIS_DEFECT(alarmTxt);
		}
		break;
	case AL_SysFail_MIXER_AVCAL:
		{
			bRes=SetAlarm_MIXER_AVCAL(alarmTxt);
		}
		break;
	case AL_SysAl_TUBE_OCCLUSION:
		{
			bRes=SetAlarm_TUBE_OCCLUSION(alarmTxt);
		}
		break;
	case AL_SysFail_P_IN_MIXER:
		{
			bRes=SetAlarm_P_IN_MIXER(alarmTxt);
		}
		break;
	/*case AL_SysFail_OVERPRESSURE:
		{
			bRes=SetAlarm_OVERPRESSURE(alarmTxt);
		}
		break;*/
	case AL_SysFail_VOLTAGE:
		{
			bRes=SetAlarm_VOLTAGE(alarmTxt);
		}
		break;
	case AL_SysFail_OUTOFMEMORY:
		{
			bRes=SetAlarm_OUTOFMEMORY(alarmTxt);
		}
		break;
	case AL_SysLimit_Pinsp_NotReached:
		{
			bRes=SetAlarm_Pinsp_NotReached(alarmTxt);
		}
		break;
	case AL_SysLimit_Vlimitted:
		{
			bRes=SetAlarm_Vlimitted(alarmTxt);
		}
		break;
	case AL_SysLimit_Vgaranty:
		{
			bRes=SetAlarm_Vgaranty(alarmTxt);
		}
		break;
	case AL_PatAl_MVmax:
		{
			bRes=SetAlarm_MVmax(alarmTxt);
		}
		break;
	case AL_PatAl_MVmin:
		{
			bRes=SetAlarm_MVmin(alarmTxt);
		}
		break;
	case AL_PatAl_SPO2_SIQmin:
		{
			bRes=SetAlarm_PRICO_SIQvalue(alarmTxt);
		}
		break;
	case AL_PRICO_FiO2max:
		{
			bRes=SetAlarm_PRICO_FiO2max(alarmTxt);
		}
		break;
	case AL_PRICO_FiO2min:
		{
			bRes=SetAlarm_PRICO_FiO2min(alarmTxt);
		}
		break;
	case AL_PatAl_PIPmax:
		{
			setPIPmaxAlarmText(alarmTxt);
			bRes=SetAlarm_PIPmax();
		}
		break;
	case AL_PatAl_PIPmin:
		{
			setPIPminAlarmText(alarmTxt);
			bRes=SetAlarm_PIPmin();
		}
		break;
	case AL_PatAl_PEEPminHigh:
		{
			setPEEPminHighAlarmText(alarmTxt);
			bRes=SetAlarm_PEEPminHigh();
		}
		break;
	case AL_PatAl_PEEPminLow:
		{
			setPEEPminLowAlarmText(alarmTxt);
			bRes=SetAlarm_PEEPminLow();
		}
		break;
	case AL_PatAl_BPMmax:
		{
			bRes=SetAlarm_BPMmax(alarmTxt);
		}
		break;
	case AL_PatAl_Leakmax:
		{
			bRes=SetAlarm_Leakmax(alarmTxt);
		}
		break;
	case AL_PatAl_Apnoe:
		{
			bRes=SetAlarm_Apnoe(alarmTxt);
		}
		break;
	case AL_PatAl_DCO2max:
		{
			bRes=SetAlarm_DCO2max(alarmTxt);
		}
		break;
	case AL_PatAl_DCO2min:
		{
			bRes=SetAlarm_DCO2min(alarmTxt);
		}
		break;
	case AL_PatAl_ETCO2max:
		{
			bRes=SetAlarm_ETCO2max(alarmTxt);
		}
		break;
	case AL_PatAl_ETCO2min:
		{
			bRes=SetAlarm_ETCO2min(alarmTxt);
		}
		break;
	case AL_PatAl_FICO2max:
		{
			bRes=SetAlarm_FICO2max(alarmTxt);
		}
		break;
	case AL_PatAl_FICO2min:
		{
			bRes=SetAlarm_FICO2min(alarmTxt);
		}
		break;
	case AL_PatAl_SPO2max:	
		{
			bRes=SetAlarm_SPO2max(alarmTxt);
		}
		break;					
	case AL_PatAl_SPO2min:
		{
			bRes=SetAlarm_SPO2min(alarmTxt);
		}
		break;
	case AL_PatAl_PulseRatemax:
		{
			bRes=SetAlarm_PulseRatemax(alarmTxt);
		}
		break;						
	case AL_PatAl_PulseRatemin:
		{
			bRes=SetAlarm_PulseRatemin(alarmTxt);
		}
		break;
	case AL_PatAl_SPO2_PImin:
		{
			bRes=SetAlarm_SPO2_PImin(alarmTxt);
		}
		break;
	case AL_Accu_POWER:
		{
			bRes=SetAlarm_Accu_POWER(alarmTxt);
		}
		break;
	case AL_Accu_60:
		{
			bRes=SetAlarm_Accu_60(alarmTxt);
		}
		break;
	case AL_Accu_30:
		{
			bRes=SetAlarm_Accu_30(alarmTxt);
		}
		break;
	case AL_Accu_15:
		{
			bRes=SetAlarm_Accu_15(alarmTxt);
		}
		break;
	case AL_Accu_Empty:
		{
			bRes=SetAlarm_AccuEmpty(alarmTxt);
		}
		break;
	case AL_Accu_Defect:
		{
			bRes=SetAlarm_AccuDefect(alarmTxt);
		}
		break;
	default:
		break;
	}
	
	return bRes;
}

bool CAlarmMgmtHandler::isActiveAlarmHigherPriority(eAlarmPrio prioAlarm)
{
	bool bRes=false;
	if(AL_NONE!=getActiveAlarm())//active alarm
	{
		// alarm with higher priority active???
		if(	getPrioActiveAlarm() < prioAlarm )	
		{
			bRes=true;
		}
	}
	return bRes;
}
bool CAlarmMgmtHandler::CanSetAlarm_ChecksumConPIC()
{
	if(ALARM_SysFail_ChecksumConPIC->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//all alarms inactive or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysFail_ChecksumConPIC)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_ChecksumConPIC(CStringW alarmTxt)
{
	if(CanSetAlarm_ChecksumConPIC()==false)
		return false;

	eAlarm alarm=AL_SysFail_ChecksumConPIC;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	setAlarmsilentActive();

	setActiveAlarm(alarm, alarmTxt);
	
	return true;
}

bool CAlarmMgmtHandler::CanSetAlarm_ChecksumMonPIC()
{
	if(ALARM_SysFail_ChecksumMonPIC->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//all alarms inactive or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysFail_ChecksumMonPIC)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_ChecksumMonPIC(CStringW alarmTxt)
{
	if(CanSetAlarm_ChecksumMonPIC()==false)
		return false;

	eAlarm alarm=AL_SysFail_ChecksumMonPIC;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	setAlarmsilentActive();

	setActiveAlarm(alarm, alarmTxt);

	return true;
	

}
bool CAlarmMgmtHandler::CanSetAlarm_RELAIS_DEFECT()
{
	if(ALARM_SysFail_RELAIS_DEFECT->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//all alarms inactive or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysFail_RELAIS_DEFECT)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_RELAIS_DEFECT(CStringW alarmTxt)
{
	if(CanSetAlarm_RELAIS_DEFECT()==false)
		return false;
	
	eAlarm alarm=AL_SysFail_RELAIS_DEFECT;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	setAlarmsilentActive();

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_MIXER_AVCAL()
{
	if(ALARM_SysFail_MIXER_AVCAL->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//all alarms inactive or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysFail_MIXER_AVCAL)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_MIXER_AVCAL(CStringW alarmTxt)
{
	if(CanSetAlarm_MIXER_AVCAL()==false)
		return false;

	eAlarm alarm=AL_SysFail_MIXER_AVCAL;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	setAlarmsilentActive();

	setActiveAlarm(alarm, alarmTxt);

	return true;

}

bool CAlarmMgmtHandler::CanSetAlarm_P_IN_MIXER()
{
	if(ALARM_SysFail_P_IN_MIXER->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//all alarms inactive or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysFail_P_IN_MIXER)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_P_IN_MIXER(CStringW alarmTxt)
{
	if(CanSetAlarm_P_IN_MIXER()==false)
		return false;
	
	eAlarm alarm=AL_SysFail_P_IN_MIXER;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	setAlarmsilentActive();

	setActiveAlarm(alarm, alarmTxt);

	return true;

}

bool CAlarmMgmtHandler::CanSetAlarm_VOLTAGE()
{
	if(ALARM_SysFail_VOLTAGE->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//all alarms inactive or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysFail_VOLTAGE)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_VOLTAGE(CStringW alarmTxt)
{
	if(CanSetAlarm_VOLTAGE()==false)
		return false;

	
	eAlarm alarm=AL_SysFail_VOLTAGE;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	setAlarmsilentActive();

	setActiveAlarm(alarm, alarmTxt);

	CStringW szTemp=_T("");
	szTemp.Format(_T("WDST%d#%d"), (int)getModel()->getDATAHANDLER()->GetI2CWatchdogState(), prioAlarm);
	setLogTxt(szTemp);

	return true;

}
bool CAlarmMgmtHandler::CanSetAlarm_IF_SPI()//newSPI
{
	if(ALARM_SysFail_IF_SPI->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//all alarms inactive or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysFail_IF_SPI)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_IF_SPI(CStringW alarmTxt)//newSPI
{
	if(CanSetAlarm_IF_SPI()==false)
		return false;

	if(getModel()->isSPIavailabel())
		getModel()->setSPIavailability(FALSE);

	eAlarm alarm=AL_SysFail_IF_SPI;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);

	getModel()->setSPIavailability(FALSE);
	deleteLowerPrioAlarms(prioAlarm);
	setAlarmsilentActive();

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_IF_DIO()
{
	if(ALARM_SysFail_IF_DIO->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//all alarms inactive or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysFail_IF_DIO)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_IF_DIO(CStringW alarmTxt)
{
	if(CanSetAlarm_IF_DIO()==false)
		return false;

	eAlarm alarm=AL_SysFail_IF_DIO;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_1

	getModel()->SetDIOavailability(FALSE);
	deleteLowerPrioAlarms(prioAlarm);
	setAlarmsilentActive();

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_IF_COM()
{
	if(ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//all alarms inactive or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysFail_IF_COM)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_IF_COM(CStringW alarmTxt)
{
	if(CanSetAlarm_IF_COM()==false)
		return false;

	eAlarm alarm=AL_SysFail_IF_COM;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_1

	deleteLowerPrioAlarms(prioAlarm);
	setAlarmsilentActive();

	setActiveAlarm(alarm, alarmTxt);

	return true;
}

bool CAlarmMgmtHandler::CanSetAlarm_IF_I2C()
{
	if(ALARM_SysFail_IF_I2C->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//all alarms inactive or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysFail_IF_I2C)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_IF_I2C(CStringW alarmTxt)
{
	if(CanSetAlarm_IF_I2C()==false)
		return false;

	eAlarm alarm=AL_SysFail_IF_I2C;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_1

	getModel()->getI2C()->SetI2C_FRAMavailability(FALSE);
	getModel()->getI2C()->SetI2C_ACCUavailability(FALSE);
	getModel()->getI2C()->SetI2C_MCPavailability(FALSE);
	deleteLowerPrioAlarms(prioAlarm);
	setAlarmsilentActive();
	
	setActiveAlarm(alarm, alarmTxt);
	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_IF_PIF()
{
	if(ALARM_SysFail_IF_PIF->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//all alarms inactive or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysFail_IF_PIF)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_IF_PIF(CStringW alarmTxt)
{
	if(CanSetAlarm_IF_PIF()==false)
		return false;

	eAlarm alarm=AL_SysFail_IF_PIF;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_1

	getModel()->SetPIFavailability(FALSE);
	deleteLowerPrioAlarms(prioAlarm);
	setAlarmsilentActive();

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
//bool CAlarmMgmtHandler::CanSetAlarm_IF_ACULINK()
//{
//	if(m_eAlarmStates.eSoA_IF_ACULINK==AS_ACTIVE)
//		return false;
//
//	eVentSilentState silentState=getAlarmSilentState();
//
//	//all alarms inactive or service mode????
//	if(		silentState==ASTATE_SYSTEM_SILENT
//		||	silentState==ASTATE_SILENT
//		//||	silentState==ASTATE_MANSILENT_SYSFAIL
//		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
//		return false;
//
//	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysFail_IF_ACULINK)))
//		return false;
//	return true;
//}
//bool CAlarmMgmtHandler::SetAlarm_IF_ACULINK(CStringW alarmTxt)
//{
//	if(CanSetAlarm_IF_ACULINK()==false)
//		return false;
//
//	eAlarm alarm=AL_SysFail_IF_ACULINK;
//	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_1
//
//	getModel()->SetAcuLinkAvailability(FALSE);
//	deleteLowerPrioAlarms(prioAlarm);
//	//rku
//	setAlarmsilentActive();
//
//	writeAlarmArray(alarm,AS_ACTIVE);
//	updateInternalAlarmStates();
//	updateActiveAlarm(alarm,AS_ACTIVE);
//	updateAlarmSound();
//
//	CStringW szTemp=_T("");
//	szTemp.Format(_T("#%d"), prioAlarm);
//	alarmTxt+=szTemp;
//	setLogAlarm(alarm, alarmTxt);
//
//	if(AfxGetApp() != NULL)
//		AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
//
//	return true;
//}
bool CAlarmMgmtHandler::CanSetAlarm_Fan()
{
	if(ALARM_SysFail_Fan->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//all alarms inactive or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysFail_Fan)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_Fan(CStringW alarmTxt)
{
	if(CanSetAlarm_Fan()==false)
		return false;

	eAlarm alarm=AL_SysFail_Fan;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_1

	deleteLowerPrioAlarms(prioAlarm);
	setAlarmsilentActive();

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_OUTOFMEMORY()
{
	if(ALARM_SysFail_OUTOFMEMORY->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//all alarms inactive or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysFail_OUTOFMEMORY)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_OUTOFMEMORY(CStringW alarmTxt)
{
	if(CanSetAlarm_OUTOFMEMORY()==false)
		return false;

	eAlarm alarm=AL_SysFail_OUTOFMEMORY;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	setAlarmsilentActive();
	
	setActiveAlarm(alarm, alarmTxt);

	return true;

}
bool CAlarmMgmtHandler::CanSetAlarm_P_IN_O2()
{
	if(ALARM_SysAl_P_IN_O2->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysAl_P_IN_O2)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_P_IN_O2(CStringW alarmTxt)
{
	if(CanSetAlarm_P_IN_O2()==false)
		return false;

	eAlarm alarm=AL_SysAl_P_IN_O2;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_P_IN_AIR()
{
	if(ALARM_SysAl_P_IN_AIR->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysAl_P_IN_AIR)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_P_IN_AIR(CStringW alarmTxt)
{
	if(CanSetAlarm_P_IN_AIR()==false)
		return false;

	eAlarm alarm=AL_SysAl_P_IN_AIR;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_TUBE_OCCLUSION()
{
	if(ALARM_SysAl_TUBE_OCCLUSION->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysAl_TUBE_OCCLUSION)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_TUBE_OCCLUSION(CStringW alarmTxt)
{
	if(CanSetAlarm_TUBE_OCCLUSION()==false)
		return false;

	eAlarm alarm=AL_SysAl_TUBE_OCCLUSION;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, alarmTxt);

	return true;
}

bool CAlarmMgmtHandler::CanSetAlarm_DISCONNECTION()
{
	if(ALARM_DISCONNECTION->getAlarmState()==AS_ACTIVE)
		return false;

	if(getModel()->getCONFIG()->GetCurMode()==VM_THERAPIE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	silentState==ASTATE_AUTOSILENT//20160310FWI
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating()
		||	getStartupSilent()==true)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_DISCONNECTION)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_DISCONNECTION(CStringW alarmTxt)
{
	if(CanSetAlarm_DISCONNECTION()==false)
		return false;

	eAlarm alarm=AL_DISCONNECTION;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_4

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_TUBUSBLOCKED()
{
	//bool bResult=false;
		
	if(ALARM_TUBUSBLOCKED->getAlarmState()==AS_ACTIVE)
		return false;

	if(getModel()->getCONFIG()->GetCurMode()==VM_THERAPIE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	silentState==ASTATE_AUTOSILENT//20160310FWI
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating()
		||	getStartupSilent()==true)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_TUBUSBLOCKED)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_TUBUSBLOCKED(CStringW alarmTxt)
{
	if(CanSetAlarm_TUBUSBLOCKED()==false)
		return false;

	eAlarm alarm=AL_TUBUSBLOCKED;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_3

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, alarmTxt);

	return true;
}

//SetAlarm_NEBULIZER_DISCONNECTION
//bool CAlarmMgmtHandler::CanSetAlarm_NEBULIZER_DISCONNECTION()
//{
//	if(m_eAlarmStates.eSoA_Nebulizer_Disconnection==AS_ACTIVE)
//		return false;
//
//	if(m_eAlarmStates.eSoA_IF_SPI==AS_ACTIVE)
//		return false;
//
//	
//
//	eVentSilentState silentState=getAlarmSilentState();
//
//	//higher alarms silent or service mode????
//	if(		silentState==ASTATE_SYSTEM_SILENT
//		||	silentState==ASTATE_SILENT
//		//||	silentState==ASTATE_MANSILENT_CALIBRATION
//		/*||	silentState==ASTATE_MANSILENT_SYSFAIL
//		||	silentState==ASTATE_MANSILENT_SYSALARM
//		||	silentState==ASTATE_MANSILENT*/
//		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
//		return false;
//
//	eAlarm alarm=AL_Nebulizer_Disconnection;
//	eAlarmPrio prioAlarm = getAlarmPrio(alarm);
//
//	if(AL_NONE!=m_eActiveAlarm)//active alarm
//	{
//		// alarm with higher priority active???
//		if(	m_ePrioActiveAlarm < prioAlarm )	
//		{
//			return false;
//		}
//	}
//	return true;
//}
//bool CAlarmHandler::SetAlarm_NEBULIZER_DISCONNECTION(CStringW alarmTxt)
//{
//	if(CanSetAlarm_NEBULIZER_DISCONNECTION()==false)
//		return false;
//
//	eAlarm alarm=AL_Nebulizer_Disconnection;
//	eAlarmPrio prioAlarm = getAlarmPrio(alarm);
//
//	writeAlarmArray(alarm,AS_ACTIVE);
//	updateInternalAlarmState(alarm,AS_ACTIVE);
//	updateActiveAlarm(alarm,AS_ACTIVE);
//	updateAlarmSound();
//
//
//	CStringW szTemp=_T("");
//	szTemp.Format(_T("#%d"), prioAlarm);
//	alarmTxt+=szTemp;
//	setLogAlarm(alarm, alarmTxt);
//
//	if(AfxGetApp() != NULL)
//		AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
//
//	return true;
//}

//AL_Nebulizer_SysError
//bool CAlarmMgmtHandler::CanSetAlarm_NEBULIZER_SYSERROR()
//{
//	if(m_eAlarmStates.eSoA_Nebulizer_SysError==AS_ACTIVE)
//		return false;
//
//	if(m_eAlarmStates.eSoA_IF_SPI==AS_ACTIVE)
//		return false;
//
//	
//
//	eVentSilentState silentState=getAlarmSilentState();
//
//	//higher alarms silent or service mode????
//	if(		silentState==ASTATE_SYSTEM_SILENT
//		||	silentState==ASTATE_SILENT
//		//||	silentState==ASTATE_MANSILENT_CALIBRATION
//		/*||	silentState==ASTATE_MANSILENT_SYSFAIL
//		||	silentState==ASTATE_MANSILENT_SYSALARM
//		||	silentState==ASTATE_MANSILENT*/
//		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
//		return false;
//
//	eAlarm alarm=AL_Nebulizer_SysError;
//	eAlarmPrio prioAlarm = getAlarmPrio(alarm);
//
//	if(AL_NONE!=m_eActiveAlarm)//active alarm
//	{
//		// alarm with higher priority active???
//		if(	m_ePrioActiveAlarm < prioAlarm )	
//		{
//			return false;
//		}
//	}
//	return true;
//}
//bool CAlarmMgmtHandler::SetAlarm_NEBULIZER_SYSERROR(CStringW alarmTxt)
//{
//	if(CanSetAlarm_NEBULIZER_SYSERROR()==false)
//		return false;
//
//	eAlarm alarm=AL_Nebulizer_SysError;
//	eAlarmPrio prioAlarm = getAlarmPrio(alarm);
//
//	writeAlarmArray(alarm,AS_ACTIVE);
//	updateInternalAlarmState(alarm,AS_ACTIVE);
//	updateActiveAlarm(alarm,AS_ACTIVE);
//	updateAlarmSound();
//
//
//	CStringW szTemp=_T("");
//	szTemp.Format(_T("#%d"), prioAlarm);
//	alarmTxt+=szTemp;
//	setLogAlarm(alarm, alarmTxt);
//
//	if(AfxGetApp() != NULL)
//		AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
//
//	return true;
//}

	


bool CAlarmMgmtHandler::CanSetAlarm_FLOW_SENSOR_DEFECT()
{
	if(ALARM_Sens_FLOW_SENSOR_DEFECT->getAlarmState()==AS_ACTIVE)
		return false;

	if(ALARM_SysFail_IF_SPI->getAlarmState()==AS_ACTIVE)
		return false;

	//FlowSensor off && FlowSensoralarm
	if(	getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
	{
		return false;
	}

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_Sens_FLOW_SENSOR_DEFECT)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_FLOW_SENSOR_DEFECT(CStringW alarmTxt)
{
	if(CanSetAlarm_FLOW_SENSOR_DEFECT()==false)
		return false;

	eAlarm alarm=AL_Sens_FLOW_SENSOR_DEFECT;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_FLOW_SENSOR_CLEANING()
{
	if(ALARM_Sens_FLOW_SENSOR_CLEANING->getAlarmState()==AS_ACTIVE)
		return false;

	if(ALARM_SysFail_IF_SPI->getAlarmState()==AS_ACTIVE)
		return false;

	//FlowSensor off && FlowSensoralarm
	if(	getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
	{
		return false;
	}

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_Sens_FLOW_SENSOR_CLEANING)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_FLOW_SENSOR_CLEANING(CStringW alarmTxt)
{
	if(CanSetAlarm_FLOW_SENSOR_CLEANING()==false)
		return false;

	eAlarm alarm=AL_Sens_FLOW_SENSOR_CLEANING;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_FLOW_SENSOR_NOTCONNECTED()
{
	if(ALARM_Sens_FLOW_SENSOR_NOTCONNECTED->getAlarmState()==AS_ACTIVE)
		return false;

	if(ALARM_SysFail_IF_SPI->getAlarmState()==AS_ACTIVE)
		return false;

	//FlowSensor off && FlowSensoralarm
	if(	getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
	{
		return false;
	}

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_Sens_FLOW_SENSOR_NOTCONNECTED)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_FLOW_SENSOR_NOTCONNECTED(CStringW alarmTxt)
{
	if(CanSetAlarm_FLOW_SENSOR_NOTCONNECTED()==false)
		return false;

	eAlarm alarm=AL_Sens_FLOW_SENSOR_NOTCONNECTED;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_O2_SENSOR_DEFECT()
{
	if(ALARM_Sens_O2_SENSOR_DEFECT->getAlarmState()==AS_ACTIVE)
		return false;

	if(ALARM_SysFail_IF_SPI->getAlarmState()==AS_ACTIVE)
		return false;

	//O2Sensor off && O2Sensoralarm
	if(getModel()->getDATAHANDLER()->GetO2SensorState()==OXYSENS_OFF)
	{
		return false;
	}

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_Sens_O2_SENSOR_DEFECT)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_O2_SENSOR_DEFECT(CStringW alarmTxt)
{
	if(CanSetAlarm_O2_SENSOR_DEFECT()==false)
		return false;

	eAlarm alarm=AL_Sens_O2_SENSOR_DEFECT;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, alarmTxt);
	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_O2_SENSOR_USED()
{
	if(ALARM_Sens_O2_SENSOR_USED->getAlarmState()==AS_ACTIVE)
		return false;

	if(ALARM_SysFail_IF_SPI->getAlarmState()==AS_ACTIVE)
		return false;

	//O2Sensor off && O2Sensoralarm
	if(getModel()->getDATAHANDLER()->GetO2SensorState()==OXYSENS_OFF)
	{
		return false;
	}

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_Sens_O2_SENSOR_USED)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_O2_SENSOR_USED(CStringW alarmTxt)
{
	if(CanSetAlarm_O2_SENSOR_USED()==false)
		return false;

	eAlarm alarm=AL_Sens_O2_SENSOR_USED;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_O2_VALUE_INCORRECT()
{
	if(ALARM_Sens_O2_VALUE_INCORRECT->getAlarmState()==AS_ACTIVE)
		return false;

	if(ALARM_SysFail_IF_SPI->getAlarmState()==AS_ACTIVE)
		return false;

	//O2Sensor off && O2Sensoralarm
	if(getModel()->getDATAHANDLER()->GetO2SensorState()==OXYSENS_OFF)
	{
		return false;
	}

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_Sens_O2_VALUE_INCORRECT)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_O2_VALUE_INCORRECT(CStringW alarmTxt)
{
	if(CanSetAlarm_O2_VALUE_INCORRECT()==false)
		return false;

	eAlarm alarm=AL_Sens_O2_VALUE_INCORRECT;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_O2_NOT_CALIBRATED()
{
	if(ALARM_Sens_O2_NOT_CALIBRATED->getAlarmState()==AS_ACTIVE)
		return false;

	if(ALARM_SysFail_IF_SPI->getAlarmState()==AS_ACTIVE)
		return false;

	//O2Sensor off && O2Sensoralarm
	if(getModel()->getDATAHANDLER()->GetO2SensorState()==OXYSENS_OFF)
	{
		return false;
	}

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_Sens_O2_NOT_CALIBRATED)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_O2_NOT_CALIBRATED(CStringW alarmTxt)
{
	if(CanSetAlarm_O2_NOT_CALIBRATED()==false)
		return false;

	eAlarm alarm=AL_Sens_O2_NOT_CALIBRATED;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_PRICO_FiO2outOfRange()
{
	if(ALARM_Sens_PRICO_FiO2outOfRange->getAlarmState()==AS_ACTIVE)
		return false;

	//O2Sensor off && O2Sensoralarm
	if(getModel()->getDATAHANDLER()->GetO2SensorState()==OXYSENS_OFF)
	{
		return false;
	}

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_Sens_PRICO_FiO2outOfRange)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_PRICO_FiO2outOfRange(CStringW alarmTxt)
{
	if(CanSetAlarm_PRICO_FiO2outOfRange()==false)
		return false;

	eAlarm alarm=AL_Sens_PRICO_FiO2outOfRange;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);

	deleteLowerPrioAlarms(prioAlarm);

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_SPO2_SENSORFAULTY()
{
	if(ALARM_Sens_SPO2_SENSORFAULTY->getAlarmState()==AS_ACTIVE)
		return false;

	if(ALARM_Sens_SPO2_MODULE_NOTCONNECTED->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_Sens_SPO2_SENSORFAULTY)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_SPO2_SENSORFAULTY(CStringW alarmTxt)
{
	if(CanSetAlarm_SPO2_SENSORFAULTY()==false)
		return false;

	eAlarm alarm=AL_Sens_SPO2_SENSORFAULTY;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	if(ALARM_Sens_SPO2_CHECKSENSOR->getAlarmState()!=AS_NONE)
	{
		deleteAlarm(AL_Sens_SPO2_CHECKSENSOR, false);
	}

	deleteLowerPrioAlarms(prioAlarm);

	setActiveAlarm(alarm, alarmTxt);

	return true;
}

bool CAlarmMgmtHandler::CanSetAlarm_SPO2_CHECKSENSOR()
{
	if(ALARM_Sens_SPO2_CHECKSENSOR->getAlarmState()==AS_ACTIVE)
		return false;

	if(ALARM_Sens_SPO2_SENSORFAULTY->getAlarmState()==AS_ACTIVE)
		return false;

	if(ALARM_Sens_SPO2_MODULE_NOTCONNECTED->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_Sens_SPO2_CHECKSENSOR)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_SPO2_CHECKSENSOR(CStringW alarmTxt)
{
	if(CanSetAlarm_SPO2_CHECKSENSOR()==false)
		return false;

	eAlarm alarm=AL_Sens_SPO2_CHECKSENSOR;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);

	setActiveAlarm(alarm, alarmTxt);

	return true;
}

bool CAlarmMgmtHandler::CanSetAlarm_SPO2_MODULE_NOTCONNECTED()
{
	if(ALARM_Sens_SPO2_MODULE_NOTCONNECTED->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_Sens_SPO2_MODULE_NOTCONNECTED)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_SPO2_MODULE_NOTCONNECTED(CStringW alarmTxt)
{
	if(CanSetAlarm_SPO2_MODULE_NOTCONNECTED()==false)
		return false;

	eAlarm alarm=AL_Sens_SPO2_MODULE_NOTCONNECTED;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	if(ALARM_Sens_SPO2_CHECKSENSOR->getAlarmState()!=AS_NONE)
	{
		deleteAlarm(AL_Sens_SPO2_CHECKSENSOR, false);
	}
	else if(ALARM_Sens_SPO2_SENSORFAULTY->getAlarmState()!=AS_NONE)
	{
		deleteAlarm(AL_Sens_SPO2_SENSORFAULTY, false);
	}

	deleteLowerPrioAlarms(prioAlarm);

	setActiveAlarm(alarm, alarmTxt);

	return true;
}

bool CAlarmMgmtHandler::CanSetAlarm_CO2_MODULE_NOTCONNECTED()
{
	if(ALARM_Sens_CO2_MODULE_NOTCONNECTED->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_Sens_CO2_MODULE_NOTCONNECTED)))
		return false;
	return true;
}

bool CAlarmMgmtHandler::SetAlarm_CO2_MODULE_NOTCONNECTED(CStringW alarmTxt)
{
	if(CanSetAlarm_CO2_MODULE_NOTCONNECTED()==false)
		return false;

	eAlarm alarm=AL_Sens_CO2_MODULE_NOTCONNECTED;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	if(ALARM_Sens_CO2_FILTERLINE_NOTCONNECTED->getAlarmState()!=AS_NONE)
	{
		deleteAlarm(AL_Sens_CO2_FILTERLINE_NOTCONNECTED, false);
	}
	else if(ALARM_Sens_CO2_CHECKSAMPLINGLINE->getAlarmState()!=AS_NONE)
	{
		deleteAlarm(AL_Sens_CO2_CHECKSAMPLINGLINE, false);
	}
	else if(ALARM_Sens_CO2_CHECKAIRWAYADAPTER->getAlarmState()!=AS_NONE)
	{
		deleteAlarm(AL_Sens_CO2_CHECKAIRWAYADAPTER, false);
	}
	else if(ALARM_Sens_CO2_SENSORFAULTY->getAlarmState()!=AS_NONE)
	{
		deleteAlarm(AL_Sens_CO2_SENSORFAULTY, false);
	}

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, alarmTxt);

	return true;
}

bool CAlarmMgmtHandler::CanSetAlarm_CO2_FILTERLINE_NOTCONNECTED()
{
	if(ALARM_Sens_CO2_FILTERLINE_NOTCONNECTED->getAlarmState()==AS_ACTIVE)
		return false;

	if(ALARM_Sens_CO2_MODULE_NOTCONNECTED->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_Sens_CO2_FILTERLINE_NOTCONNECTED)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_CO2_FILTERLINE_NOTCONNECTED(CStringW alarmTxt)
{
	if(CanSetAlarm_CO2_FILTERLINE_NOTCONNECTED()==false)
		return false;

	eAlarm alarm=AL_Sens_CO2_FILTERLINE_NOTCONNECTED;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_CO2_CHECKSAMPLINGLINE()
{
	if(ALARM_Sens_CO2_CHECKSAMPLINGLINE->getAlarmState()==AS_ACTIVE)
		return false;

	if(ALARM_Sens_CO2_MODULE_NOTCONNECTED->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_Sens_CO2_CHECKSAMPLINGLINE)))
		return false;
	return true;
}

bool CAlarmMgmtHandler::SetAlarm_CO2_CHECKSAMPLINGLINE(CStringW alarmTxt)
{
	if(CanSetAlarm_CO2_CHECKSAMPLINGLINE()==false)
		return false;

	eAlarm alarm=AL_Sens_CO2_CHECKSAMPLINGLINE;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_CO2_CHECKAIRWAYADAPTER()
{
	if(ALARM_Sens_CO2_CHECKAIRWAYADAPTER->getAlarmState()==AS_ACTIVE)
		return false;

	if(ALARM_Sens_CO2_MODULE_NOTCONNECTED->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_Sens_CO2_CHECKAIRWAYADAPTER)))
		return false;
	return true;
}

bool CAlarmMgmtHandler::SetAlarm_CO2_CHECKAIRWAYADAPTER(CStringW alarmTxt)
{
	if(CanSetAlarm_CO2_CHECKAIRWAYADAPTER()==false)
		return false;

	eAlarm alarm=AL_Sens_CO2_CHECKAIRWAYADAPTER;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, alarmTxt);

	return true;
}

bool CAlarmMgmtHandler::CanSetAlarm_CO2_SENSORFAULTY()
{
	if(ALARM_Sens_CO2_SENSORFAULTY->getAlarmState()==AS_ACTIVE)
		return false;

	if(ALARM_Sens_CO2_MODULE_NOTCONNECTED->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_Sens_CO2_SENSORFAULTY)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_CO2_SENSORFAULTY(CStringW alarmTxt)
{
	if(CanSetAlarm_CO2_SENSORFAULTY()==false)
		return false;

	eAlarm alarm=AL_Sens_CO2_SENSORFAULTY;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, alarmTxt);

	return true;
}


bool CAlarmMgmtHandler::CanSetAlarm_MVmax()
{
	if(ALARM_PatAl_MVmax->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_MVmax)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_MVmax(CStringW alarmTxt)
{
	if(CanSetAlarm_MVmax()==false)
		return false;

	eAlarm alarm=AL_PatAl_MVmax;

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_MVmin()
{
	if(		getModel()->getDATAHANDLER()->IsHFOrunning()==false
		&&	getModel()->getVMODEHANDLER()->activeModeIsHFO())
		return false;

	if(ALARM_PatAl_MVmin->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_MVmin)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_MVmin(CStringW alarmTxt)
{
	if(CanSetAlarm_MVmin()==false)
		return false;

	eAlarm alarm=AL_PatAl_MVmin;

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_PRICO_SIQvalue()
{
	if(ALARM_PatAl_SPO2_SIQmin->getAlarmState()==AS_ACTIVE)
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_SPO2_SIQmin)))
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_PRICO_SIQvalue(CStringW alarmTxt)
{
	if(CanSetAlarm_PRICO_SIQvalue()==false)
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}

	eAlarm alarm=AL_PatAl_SPO2_SIQmin;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);

	deleteLowerPrioAlarms(prioAlarm);

	setActiveAlarm(alarm, m_szPIPmaxAlarmText);

	m_szPIPmaxAlarmText=_T("");

	return true;
}


bool CAlarmMgmtHandler::CanSetAlarm_PRICO_FiO2max()
{
	if(ALARM_PRICO_FiO2max->getAlarmState()==AS_ACTIVE)
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PRICO_FiO2max)))
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_PRICO_FiO2max(CStringW alarmTxt)
{
	if(CanSetAlarm_PRICO_FiO2max()==false)
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}

	eAlarm alarm=AL_PRICO_FiO2max;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);

	deleteLowerPrioAlarms(prioAlarm);

	setActiveAlarm(alarm, m_szPIPmaxAlarmText);

	m_szPIPmaxAlarmText=_T("");

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_PRICO_FiO2min()
{
	if(ALARM_PRICO_FiO2min->getAlarmState()==AS_ACTIVE)
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PRICO_FiO2min)))
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_PRICO_FiO2min(CStringW alarmTxt)
{
	if(CanSetAlarm_PRICO_FiO2min()==false)
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}

	eAlarm alarm=AL_PRICO_FiO2min;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);

	deleteLowerPrioAlarms(prioAlarm);

	setActiveAlarm(alarm, m_szPIPmaxAlarmText);

	m_szPIPmaxAlarmText=_T("");

	return true;
}

void CAlarmMgmtHandler::setPIPmaxAlarmText(CStringW alarmTxt)
{
	m_szPIPmaxAlarmText=alarmTxt;
}
bool CAlarmMgmtHandler::CanSetAlarm_PIPmax()
{
	if(ALARM_PatAl_PIPmax->getAlarmState()==AS_ACTIVE)
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_PIPmax)))
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_PIPmax()
{
	if(CanSetAlarm_PIPmax()==false)
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}

	eAlarm alarm=AL_PatAl_PIPmax;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, m_szPIPmaxAlarmText);

	m_szPIPmaxAlarmText=_T("");

	return true;
}


void CAlarmMgmtHandler::setPIPminAlarmText(CStringW alarmTxt)
{
	m_szPIPminAlarmText=alarmTxt;
}
bool CAlarmMgmtHandler::CanSetAlarm_PIPmin()
{
	if(ALARM_PatAl_PIPmin->getAlarmState()==AS_ACTIVE)
	{
		m_szPIPminAlarmText=_T("");
		return false;
	}

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
	{
		m_szPIPminAlarmText=_T("");
		return false;
	}

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_PIPmin)))
	{
		m_szPIPminAlarmText=_T("");
		return false;
	}
	return true;
}

/**********************************************************************************************//**
 * @fn	bool CAlarmMgmtHandler::SetAlarm_PIPmin()
 *
 * @brief	Sets alarm pi pmin.
 *
 * @author	Rainer Kuehner
 * @date	27.09.2016
 *
 * @return	true if it succeeds, false if it fails.
 **************************************************************************************************/
bool CAlarmMgmtHandler::SetAlarm_PIPmin()
{
	if(CanSetAlarm_PIPmin()==false)
	{
		m_szPIPminAlarmText=_T("");
		return false;
	}

	eAlarm alarm=AL_PatAl_PIPmin;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_6

	deleteLowerPrioAlarms(prioAlarm);

	setActiveAlarm(alarm, m_szPIPminAlarmText);

	m_szPIPminAlarmText=_T("");

	return true;
}
//###############################
void CAlarmMgmtHandler::setPEEPminLowAlarmText(CStringW alarmTxt)
{
	m_szPEEPminLowAlarmText=alarmTxt;
}
void CAlarmMgmtHandler::setPEEPminLowAlarmValue(int value)
{
	m_iPEEPminLowAlarmValue=value;
}
bool CAlarmMgmtHandler::CanSetAlarm_PEEPminLow()
{
	if(ALARM_PatAl_PEEPminLow->getAlarmState()==AS_ACTIVE)
	{
		m_szPEEPminLowAlarmText=_T("");
		m_iPEEPminLowAlarmValue=-999;
		return false;
	}
	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
	{
		m_szPEEPminLowAlarmText=_T("");
		m_iPEEPminLowAlarmValue=-999;
		return false;
	}

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_PEEPminLow)))
	{
		m_szPEEPminLowAlarmText=_T("");
		m_iPEEPminLowAlarmValue=-999;
		return false;
	}
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_PEEPminLow()
{
	if(CanSetAlarm_PEEPminLow()==false)
	{
		m_szPEEPminLowAlarmText=_T("");
		m_iPEEPminLowAlarmValue=-999;
		return false;
	}
	eAlarm alarm=AL_PatAl_PEEPminLow;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_6

	if(m_iPEEPminLowAlarmValue!=-999)
	{
		if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
		{
			m_szPEEPminLowAlarmText.Format(_T("[%0.1f %s]"),CTlsFloat::Round(((double)m_iPEEPminLowAlarmValue)/10, 1),getModel()->GetLanguageString(IDS_UNIT_MBAR));
		}
		else
		{
			m_szPEEPminLowAlarmText.Format(_T("[%0.1f %s]"),CTlsFloat::Round(((double)m_iPEEPminLowAlarmValue)/10, 1),getModel()->GetLanguageString(IDS_UNIT_CMH2O));
		}
	}

	setActiveAlarm(alarm, m_szPEEPminLowAlarmText);

	m_szPEEPminLowAlarmText=_T("");

	return true;
}

void CAlarmMgmtHandler::setPEEPminHighAlarmText(CStringW alarmTxt)
{
	m_szPEEPminHighAlarmText=alarmTxt;
}
void CAlarmMgmtHandler::setPEEPminHighAlarmValue(int value)
{
	m_iPEEPminHighAlarmValue=value;
}
bool CAlarmMgmtHandler::CanSetAlarm_PEEPminHigh()
{
	if(ALARM_PatAl_PEEPminHigh->getAlarmState()==AS_ACTIVE)
	{
		m_szPEEPminHighAlarmText=_T("");
		m_iPEEPminHighAlarmValue=-999;
		return false;
	}
	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
	{
		m_szPEEPminHighAlarmText=_T("");
		m_iPEEPminHighAlarmValue=-999;
		return false;
	}

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_PEEPminHigh)))
	{
		m_szPEEPminHighAlarmText=_T("");
		m_iPEEPminHighAlarmValue=-999;
		return false;
	}
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_PEEPminHigh()
{
	if(CanSetAlarm_PEEPminHigh()==false)
	{
		m_szPEEPminHighAlarmText=_T("");
		m_iPEEPminHighAlarmValue=-999;
		return false;
	}
	eAlarm alarm=AL_PatAl_PEEPminHigh;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_7

	deleteLowerPrioAlarms(prioAlarm);
	
	if(m_iPEEPminHighAlarmValue!=-999)
	{
		if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
		{
			m_szPEEPminHighAlarmText.Format(_T("[%0.1f %s]"),CTlsFloat::Round(((double)m_iPEEPminHighAlarmValue)/10, 1),getModel()->GetLanguageString(IDS_UNIT_MBAR));
		}
		else
		{
			m_szPEEPminHighAlarmText.Format(_T("[%0.1f %s]"),CTlsFloat::Round(((double)m_iPEEPminHighAlarmValue)/10, 1),getModel()->GetLanguageString(IDS_UNIT_CMH2O));
		}
	}

	setActiveAlarm(alarm, m_szPEEPminHighAlarmText);

	m_szPEEPminHighAlarmText=_T("");

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_BPMmax()
{
	if(ALARM_PatAl_BPMmax->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_BPMmax)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_BPMmax(CStringW alarmTxt)
{
	if(CanSetAlarm_BPMmax()==false)
		return false;

	eAlarm alarm=AL_PatAl_BPMmax;

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_Leakmax()
{
	if(ALARM_PatAl_Leakmax->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_Leakmax)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_Leakmax(CStringW alarmTxt)
{
	if(CanSetAlarm_Leakmax()==false)
		return false;

	eAlarm alarm=AL_PatAl_Leakmax;

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_Apnoe()
{
	eVentMode eCurActive=getModel()->getVMODEHANDLER()->getCurrentActiveMode();

	if(VM_HFO==eCurActive)
		return false;
	else if(VM_THERAPIE==eCurActive)
		return false;
	else if(VM_IPPV==eCurActive)
		return false;
	else if(VM_SIPPV==eCurActive)
		return false;

	if(ALARM_PatAl_Apnoe->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_Apnoe)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_Apnoe(CStringW alarmTxt)
{
	if(CanSetAlarm_Apnoe()==false)
		return false;

	eAlarm alarm=AL_PatAl_Apnoe;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_5

	deleteLowerPrioAlarms(prioAlarm);
	
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_DCO2max()
{
	if(ALARM_PatAl_DCO2max->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_DCO2max)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_DCO2max(CStringW alarmTxt)
{
	if(CanSetAlarm_DCO2max()==false)
		return false;

	eAlarm alarm=AL_PatAl_DCO2max;

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_DCO2min()
{
	if(ALARM_PatAl_DCO2min->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_DCO2min)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_DCO2min(CStringW alarmTxt)
{
	if(CanSetAlarm_DCO2min()==false)
		return false;

	eAlarm alarm=AL_PatAl_DCO2min;

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_ETCO2max()
{
	if(getModel()->getETCO2()==NULL)
		return false;

	if(		false==getModel()->getETCO2()->isStateOk()
		||	false==getModel()->getETCO2()->isETCO2ValueValid())
		return false;

	if(ALARM_PatAl_ETCO2max->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_ETCO2max)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_ETCO2max(CStringW alarmTxt)
{
	if(CanSetAlarm_ETCO2max()==false)
		return false;

	eAlarm alarm=AL_PatAl_ETCO2max;

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_ETCO2min()
{
	if(getModel()->getETCO2()==NULL)
		return false;

	if(		false==getModel()->getETCO2()->isStateOk()
		||	false==getModel()->getETCO2()->isETCO2ValueValid())
		return false;

	if(ALARM_PatAl_ETCO2min->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_ETCO2min)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_ETCO2min(CStringW alarmTxt)
{
	if(CanSetAlarm_ETCO2min()==false)
		return false;

	eAlarm alarm=AL_PatAl_ETCO2min;

	setActiveAlarm(alarm, alarmTxt);

	return true;
}

bool CAlarmMgmtHandler::CanSetAlarm_FICO2max()
{
	if(getModel()->getETCO2()==NULL)
		return false;

	if(		false==getModel()->getETCO2()->isStateOk()
		||	false==getModel()->getETCO2()->isFICO2ValueValid())
		return false;

	if(ALARM_PatAl_FICO2max->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_FICO2max)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_FICO2max(CStringW alarmTxt)
{
	if(CanSetAlarm_FICO2max()==false)
		return false;

	eAlarm alarm=AL_PatAl_FICO2max;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_FICO2min()
{
	if(getModel()->getETCO2()==NULL)
		return false;

	if(		false==getModel()->getETCO2()->isStateOk()
		||	false==getModel()->getETCO2()->isFICO2ValueValid())
		return false;

	if(ALARM_PatAl_FICO2min->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_FICO2min)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_FICO2min(CStringW alarmTxt)
{
	if(CanSetAlarm_FICO2min()==false)
		return false;

	eAlarm alarm=AL_PatAl_FICO2min;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}

bool CAlarmMgmtHandler::CanSetAlarm_SPO2max()
{
	if(getModel()->getSPO2()==NULL)
		return false;

	if(false==getModel()->getSPO2()->isStateOk())
		return false;

	if(ALARM_PatAl_SPO2max->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_SPO2max)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_SPO2max(CStringW alarmTxt)
{
	if(CanSetAlarm_SPO2max()==false)
		return false;

	eAlarm alarm=AL_PatAl_SPO2max;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_SPO2min()
{
	if(getModel()->getSPO2()==NULL)
		return false;

	if(false==getModel()->getSPO2()->isStateOk())
		return false;

	if(ALARM_PatAl_SPO2min->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_SPO2min)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_SPO2min(CStringW alarmTxt)
{
	if(CanSetAlarm_SPO2min()==false)
		return false;

	eAlarm alarm=AL_PatAl_SPO2min;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_PulseRatemax()
{
	if(getModel()->getSPO2()==NULL)
		return false;

	if(false==getModel()->getSPO2()->isStateOk())
		return false;

	if(ALARM_PatAl_PulseRatemax->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_PulseRatemax)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_PulseRatemax(CStringW alarmTxt)
{
	if(CanSetAlarm_PulseRatemax()==false)
		return false;

	eAlarm alarm=AL_PatAl_PulseRatemax;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_PulseRatemin()
{
	if(getModel()->getSPO2()==NULL)
		return false;

	if(false==getModel()->getSPO2()->isStateOk())
		return false;

	if(ALARM_PatAl_PulseRatemin->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_PulseRatemin)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_PulseRatemin(CStringW alarmTxt)
{
	if(CanSetAlarm_PulseRatemin()==false)
		return false;

	eAlarm alarm=AL_PatAl_PulseRatemin;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_SPO2_PImin()
{
	if(getModel()->getSPO2()==NULL)
		return false;

	if(false==getModel()->getSPO2()->isStateOk())
		return false;

	if(ALARM_PatAl_SPO2_PImin->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_PatAl_SPO2_PImin)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_SPO2_PImin(CStringW alarmTxt)
{
	if(CanSetAlarm_SPO2_PImin()==false)
		return false;

	eAlarm alarm=AL_PatAl_SPO2_PImin;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_Pinsp_NotReached()
{
	if(ALARM_SysLimit_Pinsp_NotReached->getAlarmState()==AS_ACTIVE)
		return false;

	if(getModel()->getCONFIG()->GetCurMode()==VM_THERAPIE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	silentState==ASTATE_AUTOSILENT//20160310FWI
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysLimit_Pinsp_NotReached)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_Pinsp_NotReached(CStringW alarmTxt)
{
	if(CanSetAlarm_Pinsp_NotReached()==false)
		return false;

	eAlarm alarm=AL_SysLimit_Pinsp_NotReached;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_Vlimitted()
{
	if(ALARM_SysLimit_Vlimitted->getAlarmState()==AS_ACTIVE)
		return false;

	if(getModel()->getCONFIG()->GetCurMode()==VM_THERAPIE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	silentState==ASTATE_AUTOSILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysLimit_Vlimitted)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_Vlimitted(CStringW alarmTxt)
{
	if(CanSetAlarm_Vlimitted()==false)
		return false;

	eAlarm alarm=AL_SysLimit_Vlimitted;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_Vgaranty()
{
	if(ALARM_SysLimit_Vgaranty->getAlarmState()==AS_ACTIVE)
		return false;

	if(getModel()->getCONFIG()->GetCurMode()==VM_THERAPIE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	silentState==ASTATE_AUTOSILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherPriority(getAlarmPrio(AL_SysLimit_Vgaranty)))
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_Vgaranty(CStringW alarmTxt)
{
	if(CanSetAlarm_Vgaranty()==false)
		return false;

	eAlarm alarm=AL_SysLimit_Vgaranty;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_Accu_POWER()
{
	if(ALARM_Accu_POWER->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	return true;
}
bool CAlarmMgmtHandler::SetAlarm_Accu_POWER(CStringW alarmTxt)
{
	if(CanSetAlarm_Accu_POWER()==false)
		return false;

	eAlarm alarm=AL_Accu_POWER;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	setAlarmsilentActive();

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_Accu_60()
{
	if(ALARM_Accu_60->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;

	return true;
}
bool CAlarmMgmtHandler::SetAlarm_Accu_60(CStringW alarmTxt)
{
	if(CanSetAlarm_Accu_60()==false)
		return false;

	eAlarm alarm=AL_Accu_60;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	setAlarmsilentActive();

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_Accu_30()
{
	if(ALARM_Accu_30->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_Accu_30(CStringW alarmTxt)
{
	if(CanSetAlarm_Accu_30()==false)
		return false;

	eAlarm alarm=AL_Accu_30;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	setAlarmsilentActive();

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_Accu_15()
{
	if(ALARM_Accu_15->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_Accu_15(CStringW alarmTxt)
{
	if(CanSetAlarm_Accu_15()==false)
		return false;

	eAlarm alarm=AL_Accu_15;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	setAlarmsilentActive();

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_AccuEmpty()
{
	if(ALARM_Accu_Empty->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_AccuEmpty(CStringW alarmTxt)
{
	if(CanSetAlarm_AccuEmpty()==false)
		return false;

	eAlarm alarm=AL_Accu_Empty;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	deleteAkkuAlarms();
	setAlarmsilentActive();

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::CanSetAlarm_AccuDefect()
{
	if(ALARM_Accu_Defect->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		//||  silentState==ASTATE_MANSILENT_SYSFAIL
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE)
		return false;
	return true;
}
bool CAlarmMgmtHandler::SetAlarm_AccuDefect(CStringW alarmTxt)
{
	if(CanSetAlarm_AccuDefect()==false)
		return false;

	eAlarm alarm=AL_Accu_Defect;
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);//AP_2

	deleteLowerPrioAlarms(prioAlarm);
	setAlarmsilentActive();

	setActiveAlarm(alarm, alarmTxt);

	return true;
}
bool CAlarmMgmtHandler::isPRICOrelevantAlarmActive()//rku AUTOPRICO
{
	bool bRes=false;
	eAlarm actAlarm=getActiveAlarm();
	if(		actAlarm == AL_PatAl_SPO2_SIQmin
		||	actAlarm == AL_DISCONNECTION
		||	actAlarm == AL_TUBUSBLOCKED
		||	actAlarm == AL_Sens_SPO2_MODULE_NOTCONNECTED
		||	actAlarm == AL_Sens_SPO2_SENSORFAULTY
		||	actAlarm == AL_Sens_SPO2_CHECKSENSOR
		||	actAlarm == AL_Sens_O2_SENSOR_DEFECT
		||	actAlarm == AL_Sens_O2_SENSOR_USED
		||	actAlarm == AL_Sens_O2_VALUE_INCORRECT
		||	actAlarm == AL_Sens_O2_NOT_CALIBRATED
		||	actAlarm == AL_Sens_PRICO_FiO2outOfRange
		||	actAlarm == AL_SysAl_P_IN_O2
		||	actAlarm == AL_SysAl_P_IN_AIR
		||	actAlarm == AL_SysFail_P_IN_MIXER
		||	actAlarm == AL_SysAl_TUBE_OCCLUSION
		||	actAlarm == AL_Sens_FLOW_SENSOR_DEFECT
		||	actAlarm == AL_Sens_FLOW_SENSOR_CLEANING
		||	actAlarm == AL_Sens_FLOW_SENSOR_NOTCONNECTED)
	{
		bRes=true;
	}
	return bRes;
}
bool CAlarmMgmtHandler::isFOTrelevantAlarmActive()
{
	bool bRes=false;
	eAlarm actAlarm=getActiveAlarm();
	if(		actAlarm == AL_DISCONNECTION
		||	actAlarm == AL_TUBUSBLOCKED
		||	actAlarm == AL_SysAl_P_IN_O2
		||	actAlarm == AL_SysAl_P_IN_AIR
		||	actAlarm == AL_SysFail_P_IN_MIXER
		||	actAlarm == AL_SysAl_TUBE_OCCLUSION
		||	actAlarm == AL_Sens_FLOW_SENSOR_DEFECT
		||	actAlarm == AL_Sens_FLOW_SENSOR_CLEANING
		||	actAlarm == AL_Sens_FLOW_SENSOR_NOTCONNECTED)
	{
		bRes=true;
	}
	return bRes;
}
void CAlarmMgmtHandler::setActiveAlarm(eAlarm actAlarm, CStringW alarmTxt)//rku AUTOPRICO
{
	EnterCriticalSection(&csAlarmList);
	m_pAlarmlist->getAlarm(actAlarm)->setAlarmState(AS_ACTIVE);
	LeaveCriticalSection(&csAlarmList);

	updateActiveAlarm(actAlarm,AS_ACTIVE);
	updateAlarmSound();

	eAlarmPrio prioAlarm = getAlarmPrio(actAlarm);
	CStringW szTemp=_T("");
	szTemp.Format(_T("#%d"), prioAlarm);
	alarmTxt+=szTemp;
	setLogAlarm(actAlarm, alarmTxt);

	if(getModel()->getDATAHANDLER()->isFOTLicenseAvailable()==true)
	{
		if(getModel()->getFOTThread())
		{
			if(getModel()->getFOTThread()->getFOTstate()!=FOT_OFF)
			{
				if(		getAlarmType(actAlarm)==AT_SYSFAIL
					||	getAlarmType(actAlarm)==AT_SYSALARM
					||	getAlarmType(actAlarm)==AT_DISCON_TUBUS)
				{
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_STOP_FOT);
				}
				else if(actAlarm == AL_Sens_FLOW_SENSOR_DEFECT
					||	actAlarm == AL_Sens_FLOW_SENSOR_CLEANING
					||	actAlarm == AL_Sens_FLOW_SENSOR_NOTCONNECTED)
				{
					//getModel()->getDATAHANDLER()->setFOTrunning(false);
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_STOP_FOT);
				}
			}
		}
	}

	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
}
eAlarm CAlarmMgmtHandler::getNextActiveAlarm()
{
	eAlarm actAlarm=AL_NONE;
	eAlarmPrio prioAlarm=AP_NONE;
	
	DWORD dwTimeStamp=0; //rkuTICKCOUNT

	EnterCriticalSection(&csAlarmList);
	UINT iCntList=m_pAlarmlist->getCount();
	for(UINT i=0;i<iCntList;i++)
	{
		CAlarm* cAlarm = m_pAlarmlist->getAlarm((eAlarm)i);
		if(cAlarm->getAlarmState()==AS_ACTIVE && cAlarm->getAlarmPriority()<prioAlarm)//higher priority?
		{
			actAlarm = cAlarm->getAlarm();
			prioAlarm = cAlarm->getAlarmPriority();
			dwTimeStamp = cAlarm->getTriggerTimestamp();
		}
		else if(cAlarm->getAlarmState()==AS_ACTIVE 
			&& cAlarm->getAlarmPriority()==prioAlarm 
			&& cAlarm->getTriggerTimestamp()<dwTimeStamp)//same priority but first triggered
		{
			actAlarm=cAlarm->getAlarm();
			prioAlarm = cAlarm->getAlarmPriority();
			dwTimeStamp = cAlarm->getTriggerTimestamp();
		}
	}
	LeaveCriticalSection(&csAlarmList);

	return actAlarm;
}

void CAlarmMgmtHandler::updateActiveAlarm()
{
	//DEBUGMSG(TRUE, (TEXT("CAlarmMgmtHandler::updateActiveAlarm()\r\n")));

	eAlarm oldActive=getActiveAlarm();
	eAlarm newActive=getNextActiveAlarm();
	setActiveAlarm(newActive);
	setPrioActiveAlarm(getAlarmPrio(newActive));


	if(getModel()->getAcuLink() && getModel()->getAcuLink()!=NULL)
	{
		setAlarmStatePDMS(oldActive, AS_NONE);
		setAlarmStatePDMS(newActive, AS_ACTIVE);
	}
}

void CAlarmMgmtHandler::updateActiveAlarm(eAlarm alarm,eStateOfAlarm state)
{
	//DEBUGMSG(TRUE, (TEXT("updateActiveAlarm(eAlarm alarm,eStateOfAlarm state) %d\r\n"), alarm));
	
	eAlarm oldActive=getActiveAlarm();
	eAlarmPrio prioAlarm = getAlarmPrio(alarm);

	if(state==AS_ACTIVE && oldActive==AL_NONE)
	{
		setActiveAlarm(alarm);
		setPrioActiveAlarm(getAlarmPrio(alarm));
	}
	else if(state==AS_ACTIVE)
	{
		if(prioAlarm<getPrioActiveAlarm())
		{
			setActiveAlarm(alarm);
			setPrioActiveAlarm(getAlarmPrio(alarm));
		}
	}
	else if((state==AS_SIGNALED || state==AS_NONE) && oldActive==alarm)
	{
		eAlarm newActive=getNextActiveAlarm();
		setActiveAlarm(newActive);
		setPrioActiveAlarm(getAlarmPrio(newActive));
	}

	
	if(getModel()->getAcuLink() && getModel()->getAcuLink()!=NULL)
	{
		setAlarmStatePDMS(oldActive, AS_NONE);
		setAlarmStatePDMS(getActiveAlarm(), AS_ACTIVE);
	}
}

void CAlarmMgmtHandler::CheckPRICOalarms()//rku AUTOPRICO
{
	if(getModel()->getPRICOThread())
	{
		eAlarm actAlarm=getActiveAlarm();
		eAlarmPrio prioAlarm = getPrioActiveAlarm();
		
		if(getModel()->getPRICOThread()->isPRICOalgorithmRunning())
		{
			if(		actAlarm == AL_PatAl_SPO2_SIQmin
				||	actAlarm == AL_DISCONNECTION
				||	actAlarm == AL_TUBUSBLOCKED
				||	actAlarm == AL_Sens_SPO2_CHECKSENSOR
				||	actAlarm == AL_Sens_O2_VALUE_INCORRECT
				||	actAlarm == AL_Sens_PRICO_FiO2outOfRange
				||	actAlarm == AL_SysAl_TUBE_OCCLUSION
				||	actAlarm == AL_Sens_FLOW_SENSOR_DEFECT
				||	actAlarm == AL_Sens_FLOW_SENSOR_CLEANING
				||	actAlarm == AL_Sens_FLOW_SENSOR_NOTCONNECTED)
			{	
				setPRICOAutoTurnedOff(true, prioAlarm);
				getModel()->getDATAHANDLER()->setPRICOoff();
			}
			else if(actAlarm == AL_Sens_SPO2_MODULE_NOTCONNECTED 
				||	actAlarm == AL_Sens_SPO2_SENSORFAULTY
				||	actAlarm == AL_Sens_O2_SENSOR_DEFECT
				||	actAlarm == AL_Sens_O2_SENSOR_USED
				||	actAlarm == AL_Sens_O2_NOT_CALIBRATED
				||	actAlarm == AL_SysAl_P_IN_O2
				||	actAlarm == AL_SysAl_P_IN_AIR
				||	actAlarm == AL_SysFail_P_IN_MIXER)
			{	
				setPRICOAutoTurnedOff(false, prioAlarm);
				getModel()->getDATAHANDLER()->setPRICOoff();
			}
		}

		if(isPRICOAutoTurnedOff())
		{
			checkAutoEnablePRICO();//rku AUTOPRICO
		}
	}
}
void CAlarmMgmtHandler::checkAutoEnablePRICO()//rku AUTOPRICO
{
	eAlarm curActive=getActiveAlarm();
	eAlarmPrio prioActAlarm = getPrioActiveAlarm();

	//DEBUGMSG(TRUE, (TEXT("checkAutoEnablePRICO curActiv\r\n"),curActive));

	if(		curActive != AL_PatAl_SPO2_SIQmin
		&&	curActive != AL_DISCONNECTION
		&&	curActive != AL_TUBUSBLOCKED
		&&	curActive != AL_Sens_SPO2_MODULE_NOTCONNECTED
		&&	curActive != AL_Sens_SPO2_SENSORFAULTY
		&&	curActive != AL_Sens_SPO2_CHECKSENSOR
		&&	curActive != AL_Sens_O2_SENSOR_DEFECT
		&&	curActive != AL_Sens_O2_SENSOR_USED
		&&	curActive != AL_Sens_O2_VALUE_INCORRECT
		&&	curActive != AL_Sens_O2_NOT_CALIBRATED
		&&	curActive != AL_Sens_PRICO_FiO2outOfRange
		&&	curActive != AL_SysAl_P_IN_O2
		&&	curActive != AL_SysAl_P_IN_AIR
		&&	curActive != AL_SysFail_P_IN_MIXER
		&&	curActive != AL_SysAl_TUBE_OCCLUSION
		&&	curActive != AL_Sens_FLOW_SENSOR_DEFECT
		&&	curActive != AL_Sens_FLOW_SENSOR_CLEANING
		&&	curActive != AL_Sens_FLOW_SENSOR_NOTCONNECTED)
	{
		if(curActive==AL_NONE)//no alarm active, check autoenable PRICO
		{
			//DEBUGMSG(TRUE, (TEXT("checkAutoEnablePRICO curActive==AL_NONE\r\n")));
			if(isPRICOAutoTurneOn())
				getModel()->getDATAHANDLER()->setPRICOon();
			resetPRICOAutoTurnedOff();
		}
		else if(prioActAlarm<getPRICOAutoAlarmPriority())//active alarm with higher priority than PRICO alarm, reset autoenable PRICO
		{
			//DEBUGMSG(TRUE, (TEXT("checkAutoEnablePRICO prioActAlarm<getPRICOAutoAlarmPriority\r\n")));
			resetPRICOAutoTurnedOff();
		}
		else //active alarm with lower priority than PRICO alarm, check autoenable PRICO
		{
			//DEBUGMSG(TRUE, (TEXT("checkAutoEnablePRICO else\r\n")));
			if(isPRICOAutoTurneOn())
				getModel()->getDATAHANDLER()->setPRICOon();
			resetPRICOAutoTurnedOff();
		}
	}
	else
	{
		if(		curActive == AL_PatAl_SPO2_SIQmin
			||	curActive == AL_DISCONNECTION
			||	curActive == AL_TUBUSBLOCKED
			||	curActive == AL_Sens_SPO2_CHECKSENSOR
			||	curActive == AL_Sens_O2_VALUE_INCORRECT
			||	curActive == AL_Sens_PRICO_FiO2outOfRange
			||	curActive == AL_SysAl_TUBE_OCCLUSION
			||	curActive == AL_Sens_FLOW_SENSOR_DEFECT
			||	curActive == AL_Sens_FLOW_SENSOR_CLEANING
			||	curActive == AL_Sens_FLOW_SENSOR_NOTCONNECTED)
		{	
			//DEBUGMSG(TRUE, (TEXT("checkAutoEnablePRICO2\r\n")));
			setPRICOAutoTurnedOff(true, prioActAlarm);
			getModel()->getDATAHANDLER()->setPRICOoff();
		}
		else if(curActive == AL_Sens_SPO2_MODULE_NOTCONNECTED 
			||	curActive == AL_Sens_SPO2_SENSORFAULTY
			||	curActive == AL_Sens_O2_SENSOR_DEFECT
			||	curActive == AL_Sens_O2_SENSOR_USED
			||	curActive == AL_Sens_O2_NOT_CALIBRATED
			||	curActive == AL_SysAl_P_IN_O2
			||	curActive == AL_SysAl_P_IN_AIR
			||	curActive == AL_SysFail_P_IN_MIXER)
		{	
			//DEBUGMSG(TRUE, (TEXT("checkAutoEnablePRICO3\r\n")));
			setPRICOAutoTurnedOff(false, prioActAlarm);
			getModel()->getDATAHANDLER()->setPRICOoff();
		}
	}
}

bool CAlarmMgmtHandler::setStateOfAlarm(eAlarm alarm, eStateOfAlarm eState, bool bUpdate)
{
	bool bResult = false; 

	EnterCriticalSection(&csAlarmList);
	m_pAlarmlist->getAlarm(alarm)->setAlarmState(eState);
	if(AS_ACTIVE!=eState)
		m_pAlarmlist->getAlarm(alarm)->resetTriggerTimestamp();
	LeaveCriticalSection(&csAlarmList);

	updateActiveAlarm(alarm,eState);
	updateAlarmSound();



	if(bUpdate)
	{
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
	}

	return bResult;
}



bool CAlarmMgmtHandler::deleteAlarm(eAlarm alarm, bool bUpdate)
{
	//DEBUGMSG(TRUE, (TEXT("CAlarmHandler::deleteAlarm\r\n")));
	bool bResult = false; 

	EnterCriticalSection(&csAlarmList);
	CAlarm* pTempAlarm=m_pAlarmlist->getAlarm(alarm);
	if(pTempAlarm)
	{
		pTempAlarm->setAlarmState(AS_NONE);
	}
	LeaveCriticalSection(&csAlarmList);

	updateActiveAlarm();
	updateAlarmSound();

	if(bUpdate)
	{
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
	}

	return bResult;
}

bool CAlarmMgmtHandler::setLowerPrioActiveAlarmsSignaled(eAlarmPrio prio)
{
	bool bRes=false;

	EnterCriticalSection(&csAlarmList);
	UINT iCntList=m_pAlarmlist->getCount();
	for(UINT i=0;i<iCntList;i++)
	{
		CAlarm* cAlarm = m_pAlarmlist->getAlarm((eAlarm)i);
		if(cAlarm->getAlarmPriority()>prio && cAlarm->getAlarmState()==AS_ACTIVE)	
		{
			cAlarm->setAlarmState(AS_SIGNALED);
			bRes=true;

		}
	}
	LeaveCriticalSection(&csAlarmList);

	return bRes;
}

bool CAlarmMgmtHandler::isAlarm()
{
	bool bResult = false; 

	EnterCriticalSection(&csAlarmList);
	UINT iCntList=m_pAlarmlist->getCount();
	for(UINT i=0;i<iCntList;i++)
	{
		CAlarm* cAlarm = m_pAlarmlist->getAlarm((eAlarm)i);
		if(cAlarm->getAlarmState()==AS_ACTIVE || cAlarm->getAlarmState()==AS_SIGNALED)	
		{
			bResult=true;
			break;
		}
	}
	LeaveCriticalSection(&csAlarmList);

	return bResult;
}

bool CAlarmMgmtHandler::isActiveAlarm()
{
	bool bResult = false; 

	EnterCriticalSection(&csAlarmList);
	UINT iCntList=m_pAlarmlist->getCount();
	for(UINT i=0;i<iCntList;i++)
	{
		CAlarm* cAlarm = m_pAlarmlist->getAlarm((eAlarm)i);
		if(cAlarm->getAlarmState()==AS_ACTIVE)	
		{
			bResult=true;
			break;
		}
	}
	LeaveCriticalSection(&csAlarmList);

	return bResult;
}

bool CAlarmMgmtHandler::isSignaledAlarm()
{
	bool bResult = false; 

	EnterCriticalSection(&csAlarmList);
	UINT iCntList=m_pAlarmlist->getCount();
	for(UINT i=0;i<iCntList;i++)
	{
		CAlarm* cAlarm = m_pAlarmlist->getAlarm((eAlarm)i);
		if(cAlarm->getAlarmState()==AS_SIGNALED)	
		{
			bResult=true;
			break;
		}
	}
	LeaveCriticalSection(&csAlarmList);

	return bResult;
}

bool CAlarmMgmtHandler::isAlarmType(eAlarmType type)
{
	bool bResult = false; 

	EnterCriticalSection(&csAlarmList);
	UINT iCntList=m_pAlarmlist->getCount();
	for(UINT i=0;i<iCntList;i++)
	{
		CAlarm* cAlarm = m_pAlarmlist->getAlarm((eAlarm)i);
		if(cAlarm->getAlarmType()==type)
		{
			if(cAlarm->getAlarmState()!=AS_NONE)	
			{
				bResult=true;
				break;
			}
		}
		
	}
	LeaveCriticalSection(&csAlarmList);


	return bResult;
}


bool CAlarmMgmtHandler::isActiveAlarmType(eAlarmType type)
{
	bool bResult = false; 

	EnterCriticalSection(&csAlarmList);
	UINT iCntList=m_pAlarmlist->getCount();
	for(UINT i=0;i<iCntList;i++)
	{
		CAlarm* cAlarm = m_pAlarmlist->getAlarm((eAlarm)i);
		if(cAlarm->getAlarmType()==type)
		{
			if(cAlarm->getAlarmState()==AS_ACTIVE)	
			{
				bResult=true;
				break;
			}
		}

	}
	LeaveCriticalSection(&csAlarmList);

	return bResult;
}

bool CAlarmMgmtHandler::isSignaledAlarmType(eAlarmType type)
{
	bool bResult = false; 

	EnterCriticalSection(&csAlarmList);
	UINT iCntList=m_pAlarmlist->getCount();
	for(UINT i=0;i<iCntList;i++)
	{
		CAlarm* cAlarm = m_pAlarmlist->getAlarm((eAlarm)i);
		if(cAlarm->getAlarmType()==type)
		{
			if(cAlarm->getAlarmState()==AS_SIGNALED)	
			{
				bResult=true;
				break;
			}
		}

	}
	LeaveCriticalSection(&csAlarmList);

	return bResult;
}

void CAlarmMgmtHandler::deleteSignaledAlarms()
{
	EnterCriticalSection(&csAlarmList);
	UINT iCntList=m_pAlarmlist->getCount();
	for(UINT i=0;i<iCntList;i++)
	{
		CAlarm* cAlarm = m_pAlarmlist->getAlarm((eAlarm)i);
		if(cAlarm->getAlarmState()==AS_SIGNALED)	
		{
			cAlarm->setAlarmState(AS_NONE);
		}

	}
	LeaveCriticalSection(&csAlarmList);
	
	updateActiveAlarm();
	updateAlarmSound();

	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
}

void CAlarmMgmtHandler::deleteSignaledAlarmlimitStates()
{
	EnterCriticalSection(&csAlarmList);
	UINT iCntList=m_pAlarmlist->getCount();
	for(UINT i=0;i<iCntList;i++)
	{
		CAlarm* cAlarm = m_pAlarmlist->getAlarm((eAlarm)i);
		if(cAlarm->getAlarmState()==AS_SIGNALED && cAlarm->getAlarmType()==AT_PATIENT)	
		{
			cAlarm->setAlarmState(AS_NONE);
		}

	}
	LeaveCriticalSection(&csAlarmList);


	updateActiveAlarm();
	updateAlarmSound();

	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
}

bool CAlarmMgmtHandler::deleteAkkuAlarms()
{
	bool bRes=false;
	EnterCriticalSection(&csAlarmList);
	UINT iCntList=m_pAlarmlist->getCount();
	for(UINT i=0;i<iCntList;i++)
	{
		CAlarm* cAlarm = m_pAlarmlist->getAlarm((eAlarm)i);
		if(cAlarm->getAlarmType()==AT_POWER)//delete all Akku alarms == AT_POWER	
		{
			cAlarm->setAlarmState(AS_NONE);
			bRes=true;
		}

	}
	LeaveCriticalSection(&csAlarmList);

	updateActiveAlarm();
	updateAlarmSound();
	return bRes;
}

bool CAlarmMgmtHandler::deleteLowerPrioAlarms(eAlarmPrio prio)
{
	bool bRes=false;

	EnterCriticalSection(&csAlarmList);
	UINT iCntList=m_pAlarmlist->getCount();
	for(UINT i=0;i<iCntList;i++)
	{
		CAlarm* cAlarm = m_pAlarmlist->getAlarm((eAlarm)i);
		if(		(cAlarm->getAlarmPriority()>prio) 
			&&	(cAlarm->getAlarmType()!=AT_POWER)
			&&	(cAlarm->getAlarmState()!=AS_NONE))//do not delete Akku alarms == APRIO_6	
		{
			cAlarm->setAlarmState(AS_NONE);
			bRes=true;
		}

	}
	LeaveCriticalSection(&csAlarmList);

	return bRes;
}

void CAlarmMgmtHandler::deleteAllAlarms(bool bDeleteAkkuAlarms)
{
	eAlarm akkuAlarm=AL_NONE;

	bool bResetCO2state=false;
	if(getModel()->getETCO2()!=NULL)
	{
		if(		ALARM_Sens_CO2_CHECKSAMPLINGLINE->getAlarmState()!=AS_NONE
			||	ALARM_Sens_CO2_CHECKAIRWAYADAPTER->getAlarmState()!=AS_NONE
			||	ALARM_Sens_CO2_SENSORFAULTY->getAlarmState()!=AS_NONE
			||	ALARM_Sens_CO2_MODULE_NOTCONNECTED->getAlarmState()!=AS_NONE
			||	ALARM_Sens_CO2_FILTERLINE_NOTCONNECTED->getAlarmState()!=AS_NONE)
			bResetCO2state=true;
	}
	//resetAlarmDelayComplete();

	EnterCriticalSection(&csAlarmList);
	UINT iCntList=m_pAlarmlist->getCount();
	for(UINT i=0;i<iCntList;i++)
	{
		CAlarm* cAlarm = m_pAlarmlist->getAlarm((eAlarm)i);
		if(!bDeleteAkkuAlarms)
		{
			if(	cAlarm->getAlarmType()==AT_POWER && cAlarm->getAlarmState()==AS_ACTIVE)//Akku alarms == AT_POWER
				akkuAlarm=cAlarm->getAlarm();
		}
		if(cAlarm->getAlarmState()!=AS_NONE)
		{
			cAlarm->setAlarmState(AS_NONE);
		}
	}
	LeaveCriticalSection(&csAlarmList);

	getModel()->getDATAHANDLER()->ResetI2CWatchdogState();

	updateActiveAlarm();
	updateAlarmSound();

	if(!bDeleteAkkuAlarms && akkuAlarm!=AL_NONE)
	{
		switch(akkuAlarm)
		{
		case AL_Accu_Defect:
			{
				setAlarm(AL_Accu_Defect);
			}
			break;
		case AL_Accu_POWER:
			{
				setAlarm(AL_Accu_POWER);
			}
			break;
		case AL_Accu_Empty:
			{
				setAlarm(AL_Accu_Empty);
			}
			break;
		case AL_Accu_60:
			{
				setAlarm(AL_Accu_60);
			}
			break;
		case AL_Accu_30:
			{
				setAlarm(AL_Accu_30);
			}
			break;
		case AL_Accu_15:
			{
				setAlarm(AL_Accu_15);
			}
			break;
		}
	}
	else
	{
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
	}

	if(bResetCO2state && getModel()->getETCO2())
		getModel()->getETCO2()->resetStateBytes();
}

bool CAlarmMgmtHandler::deleteAlarmType(eAlarmType type, bool bUpdate)
{
	bool bResult = false; 
	if(type==AT_NONE)
	{
		if(bUpdate)
		{
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
		}
		return bResult;
	}
	EnterCriticalSection(&csAlarmList);
	UINT iCntList=m_pAlarmlist->getCount();
	for(UINT i=0;i<iCntList;i++)
	{
		CAlarm* cAlarm = m_pAlarmlist->getAlarm((eAlarm)i);
		if(cAlarm->getAlarmType()==type)//delete all Akku alarms == AT_POWER	
		{
			cAlarm->setAlarmState(AS_NONE);
			bResult=true;
		}
	}
	LeaveCriticalSection(&csAlarmList);

	updateActiveAlarm();
	updateAlarmSound();

	if(bUpdate)
	{
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
	}

	return bResult;
}

void CAlarmMgmtHandler::updateAlarmSound()
{
	eAlarm activeAlarm=getActiveAlarm();
	if(getAlarmSilentState()!=ASTATE_ACTIVE)
	{
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_NONE);

		if(getModel()->getCONFIG()->IsNursecallActive())
		{
			getModel()->getDATAHANDLER()->SetNurscallAlarm(false);
		}
	}
	else if(activeAlarm!=AL_NONE)
	{
		switch(getPrioActiveAlarm())
		{
		case AP_1:				
		case AP_2:	
		case AP_3:
		case AP_4:
		case AP_5:
		case AP_6:
		case AP_7:
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_ALARM3);

				if(getModel()->getCONFIG()->IsNursecallActive())
				{
					getModel()->getDATAHANDLER()->SetNurscallAlarm(true);
				}
			}
			break;
		case AP_8:	
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_ALARM2);

				if(getModel()->getCONFIG()->IsNursecallActive())
				{
					getModel()->getDATAHANDLER()->SetNurscallAlarm(true);
				}
			}
			break;
		case AP_9:	
			{
				if(getModel()->getCONFIG()->GetPlayVlimitSound()
					&& activeAlarm == AL_SysLimit_Vlimitted)
				{
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_ALARM1);
				}
				else if(activeAlarm != AL_SysLimit_Vlimitted)
				{
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_ALARM1);
				}

				if(getModel()->getCONFIG()->IsNursecallActive()
					&& activeAlarm != AL_SysLimit_Vlimitted)
				{
					getModel()->getDATAHANDLER()->SetNurscallAlarm(true);
				}
			}
			break;
		default:
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_NONE);
			}
			break;
		}
	}
	else
	{
		if(ALARM_SysLimit_Pinsp_NotReached->getAlarmState()==AS_SIGNALED)
		{
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_ALARM1);

			if(getModel()->getCONFIG()->IsNursecallActive())
			{
				getModel()->getDATAHANDLER()->SetNurscallAlarm(true);
			}
		}
		else if(ALARM_SysLimit_Vlimitted->getAlarmState()==AS_SIGNALED)
		{
			if(getModel()->getCONFIG()->GetPlayVlimitSound())
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_ALARM1);
			}
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_ALARM1);
		}
		else if(ALARM_SysLimit_Vgaranty->getAlarmState()==AS_SIGNALED)
		{
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_ALARM1);

			if(getModel()->getCONFIG()->IsNursecallActive())
			{
				getModel()->getDATAHANDLER()->SetNurscallAlarm(true);
			}
		}
		else
		{
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_NONE);

			if(getModel()->getCONFIG()->IsNursecallActive())
			{
				getModel()->getDATAHANDLER()->SetNurscallAlarm(false);
			}
		}
	}
}
void CAlarmMgmtHandler::updateAlarmStatesSilentStateDependend()
{
	updateActiveAlarm();
	updateAlarmSound();
	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
}


void CAlarmMgmtHandler::setAlarmStatePDMS(eAlarm eAlarmToSet, eStateOfAlarm state)
{
	if(getModel()->getAcuLink())
	{
		switch(eAlarmToSet)
		{
		case AL_PatAl_Apnoe:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_Apnoe,(int)state);
			}
			break;
		case AL_TUBUSBLOCKED:				
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_TUBUSBLOCKED,(int)state);
			}
			break;
		case AL_DISCONNECTION:				
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_DISCONNECTION,(int)state);
			}
			break;
		case AL_SysAl_P_IN_O2:					
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysAl_P_IN_O2,(int)state);
			}
			break;
		case AL_SysAl_P_IN_AIR:					
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysAl_P_IN_AIR,(int)state);
			}
			break;
		/*case AL_SysAl_P_EXSPIRATIONTUBE:			
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysAl_P_EXSPIRATIONTUBE,(int)state);
			}
			break;
		case AL_SysAl_P_INSPIRATIONTUBE:			
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysAl_P_INSPIRATIONTUBE,(int)state);
			}
			break;*/
		case AL_Sens_O2_SENSOR_DEFECT:			
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Sens_O2_SENSOR_DEFECT,(int)state);
			}
			break;
		case AL_Sens_O2_NOT_CALIBRATED:			
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Sens_O2_NOT_CALIBRATED,(int)state);
			}
			break;
		case AL_Sens_O2_SENSOR_USED:				
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Sens_O2_SENSOR_USED,(int)state);
			}
			break;
		case AL_Sens_O2_VALUE_INCORRECT:			
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Sens_O2_VALUE_INCORRECT,(int)state);
			}
			break;
		/*case AL_Sens_PRICO_FiO2outOfRange: todo
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Sens_O2_VALUE_INCORRECT,(int)state);
			}
			break;*/
		case AL_SysFail_RELAIS_DEFECT:				
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysFail_RELAIS_DEFECT,(int)state);
			}
			break;
		case AL_SysFail_MIXER_AVCAL:					
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysFail_MIXER,(int)state);
			}
			break;
		case AL_SysAl_TUBE_OCCLUSION:					
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysAl_TUBE_OCCLUSION,(int)state);
			}
			break;
		case AL_SysFail_P_IN_MIXER:					
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysFail_P_IN_MIXER,(int)state);
			}
			break;
		/*case AL_SysFail_OVERPRESSURE:				
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysFail_OVERPRESSURE,(int)state);
			}
			break;*/
		case AL_SysFail_VOLTAGE:					
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysFail_VOLTAGE,(int)state);
			}
			break;
		case AL_SysFail_OUTOFMEMORY:					
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysFail_OUTOFMEMORY,(int)state);
			}
			break;
		case AL_SysFail_ChecksumConPIC:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysFail_ChecksumConPIC,(int)state);
			}
			break;
		case AL_SysFail_ChecksumMonPIC:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysFail_ChecksumMonPIC,(int)state);
			}
			break;
		case AL_SysFail_IF_SPI:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysFail_IF_SPI,(int)state);
			}
			break;
		case AL_SysFail_IF_DIO:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysFail_IF_DIO,(int)state);
			}
			break;
		case AL_SysFail_IF_COM:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysFail_IF_COM,(int)state);
			}
			break;
		case AL_SysFail_IF_I2C:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysFail_IF_I2C,(int)state);
			}
			break;
		case AL_SysFail_IF_PIF:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysFail_IF_PIF,(int)state);
			}
			break;
		/*case AL_SysFail_IF_ACULINK:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysFail_IF_ACULINK,(int)state);
			}
			break;*/
		case AL_SysFail_Fan:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysFail_Fan,(int)state);
			}
			break;
		case AL_PatAl_MVmax:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_MVmax,(int)state);
			}
			break;
		case AL_PatAl_MVmin:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_MVmin,(int)state);
			}
			break;
		case AL_PatAl_SPO2_SIQmin:
			{
				//getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_Pmax,(int)state);
			}
			break;
		case AL_PRICO_FiO2max:
			{
				//getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_Pmax,(int)state);
			}
			break;
		case AL_PRICO_FiO2min:
			{
				//getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_Pmax,(int)state);
			}
			break;
		case AL_PatAl_PIPmax:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_Pmax,(int)state);
			}
			break;
		case AL_PatAl_PIPmin:						
			{
				//getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_PIPmin,(int)state);
			}
			break;
		case AL_PatAl_PEEPminLow:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_PminLow,(int)state);
			}
			break;
		case AL_PatAl_PEEPminHigh:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_PminHigh,(int)state);
			}
			break;
		case AL_PatAl_BPMmax:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_BPMmax,(int)state);
			}
			break;
		case AL_PatAl_Leakmax:					
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_Leakmax,(int)state);
			}
			break;
		case AL_PatAl_DCO2max:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_DCO2max,(int)state);
			}
			break;
		case AL_PatAl_DCO2min:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_DCO2min,(int)state);
			}
			break;
		case AL_PatAl_ETCO2max:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_ETCO2max,(int)state);
			}
			break;
		case AL_PatAl_ETCO2min:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_ETCO2min,(int)state);
			}
			break;
		case AL_PatAl_FICO2max:						
			{
				//getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_FICO2max,(int)state);
			}
			break;
		case AL_PatAl_FICO2min:						
			{
				//getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_FICO2min,(int)state);
			}
			break;
		case AL_PatAl_SPO2max:						
			{
				//getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_SPO2max,(int)state);
			}
			break;				
		case AL_PatAl_SPO2min:						
			{
				//getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_SPO2min,(int)state);
			}
			break;			
		case AL_PatAl_PulseRatemax:							
			{
				//getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_PulseRatemax,(int)state);
			}
			break;								
		case AL_PatAl_PulseRatemin:						
			{
				//getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_PulseRatemin,(int)state);
			}
			break;			
		case AL_PatAl_SPO2_PImin:						
			{
				//getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_SPO2_PImin,(int)state);
			}
			break;			
		/*case AL_Nebulizer_Disconnection:			
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Nebulizer_Disconnection,(int)state);
			}
			break;
		case AL_Nebulizer_SysError:			
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Nebulizer_SysError,(int)state);
			}
			break;*/
		case AL_Sens_SPO2_MODULE_NOTCONNECTED:
			{
				//getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_SPO2_MODULE_NOTCONNECTED,(int)state);
			}
			break;
		case AL_Sens_SPO2_SENSORFAULTY:
			{
				//getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_SPO2_SENSORFAULTY,(int)state);
			}
			break;
		case AL_Sens_SPO2_CHECKSENSOR:
			{
				//getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_SPO2_CHECKSENSOR,(int)state);
			}
			break;
		case AL_Sens_CO2_MODULE_NOTCONNECTED:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_CO2_MODULE_NOTCONNECTED,(int)state);
			}
			break;
		case AL_Sens_CO2_FILTERLINE_NOTCONNECTED:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_CO2_FILTERLINE_NOTCONNECTED,(int)state);
			}
			break;
		case AL_Sens_CO2_CHECKSAMPLINGLINE:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_CO2_CHECKSAMPLINGLINE,(int)state);
			}
			break;
		case AL_Sens_CO2_CHECKAIRWAYADAPTER:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_CO2_CHECKAIRWAYADAPTER,(int)state);
			}
			break;
		case AL_Sens_CO2_SENSORFAULTY:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_CO2_SENSORFAULTY,(int)state);
			}
			break;
		case AL_Sens_FLOW_SENSOR_DEFECT:			
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Sens_FLOW_SENSOR_DEFECT,(int)state);
			}
			break;
		case AL_Sens_FLOW_SENSOR_CLEANING:		
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Sens_FLOW_SENSOR_CLEANING,(int)state);
			}
			break;
		case AL_Sens_FLOW_SENSOR_NOTCONNECTED:	
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Sens_FLOW_SENSOR_NOTCONNECTED,(int)state);
			}
			break;
		case AL_SysLimit_Vlimitted:					
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysLimit_Vlimitted,(int)state);
			}
			break;
		case AL_SysLimit_Vgaranty:					
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysLimit_Vgaranty,(int)state);
			}
			break;
		case AL_SysLimit_Pinsp_NotReached:	
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_SysLimit_Pinsp_NotReached,(int)state);
			}
			break;
		case AL_Accu_POWER:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Accu_POWER,(int)state);
			}
			break;
		case AL_Accu_60:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Accu_60,(int)state);
			}
			break;
		case AL_Accu_30:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Accu_30,(int)state);
			}
			break;
		case AL_Accu_15:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Accu_15,(int)state);
			}
			break;
		case AL_Accu_Empty:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Accu_Empty,(int)state);
			}
			break;
		case AL_Accu_Defect:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Accu_Defect,(int)state);
			}
			break;
		default:
			break;
		}
	}
	
}

void CAlarmMgmtHandler::resetVgVlAutoTurnedOff()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVgarantAutoTurnedOff_FOT=false;
	m_bVgarantAutoTurnedOff_FlowSensor=false;
	m_bVlimitAutoTurnedOff_FlowSensor=false;
	m_bVgarantAutoTurnedOff_Tube=false;
	m_bVlimitAutoTurnedOff_Tube=false;
	LeaveCriticalSection(&csVGVLautoOffState);
}

void  CAlarmMgmtHandler::resetVgVlAutoTurnedOff_FlowSensor()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVgarantAutoTurnedOff_FlowSensor=false;
	m_bVlimitAutoTurnedOff_FlowSensor=false;
	LeaveCriticalSection(&csVGVLautoOffState);
}
void  CAlarmMgmtHandler::resetVgVlAutoTurnedOff_Tube()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVgarantAutoTurnedOff_Tube=false;
	m_bVlimitAutoTurnedOff_Tube=false;
	LeaveCriticalSection(&csVGVLautoOffState);
}
void CAlarmMgmtHandler::setVgarantAutoTurnedOff_FOT()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVgarantAutoTurnedOff_FOT=true;
	LeaveCriticalSection(&csVGVLautoOffState);

}
void  CAlarmMgmtHandler::resetVgAutoTurnedOff_FOT()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVgarantAutoTurnedOff_FOT=false;
	LeaveCriticalSection(&csVGVLautoOffState);
}
bool CAlarmMgmtHandler::isVgarantAutoTurnedOff_FOT()
{
	EnterCriticalSection(&csVGVLautoOffState);
	bool bState=m_bVgarantAutoTurnedOff_FOT;
	LeaveCriticalSection(&csVGVLautoOffState);
	return bState;
}

void CAlarmMgmtHandler::setVgarantAutoTurnedOff_FlowSensor()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVgarantAutoTurnedOff_FlowSensor=true;
	LeaveCriticalSection(&csVGVLautoOffState);
	
}


void CAlarmMgmtHandler::resetVgarantAutoTurnedOff_FlowSensor()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVgarantAutoTurnedOff_FlowSensor=false;
	LeaveCriticalSection(&csVGVLautoOffState);
}
bool CAlarmMgmtHandler::isVgarantAutoTurnedOff_FlowSensor()
{
	EnterCriticalSection(&csVGVLautoOffState);
	bool bState=m_bVgarantAutoTurnedOff_FlowSensor;
	LeaveCriticalSection(&csVGVLautoOffState);
	return bState;
}
void CAlarmMgmtHandler::setVlimitAutoTurnedOff_FlowSensor()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVlimitAutoTurnedOff_FlowSensor=true;
	LeaveCriticalSection(&csVGVLautoOffState);
}
void CAlarmMgmtHandler::resetVlimitAutoTurnedOff_FlowSensor()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVlimitAutoTurnedOff_FlowSensor=false;
	LeaveCriticalSection(&csVGVLautoOffState);
}
bool CAlarmMgmtHandler::isVlimitAutoTurnedOff_FlowSensor()
{
	EnterCriticalSection(&csVGVLautoOffState);
	bool bState=m_bVlimitAutoTurnedOff_FlowSensor;
	LeaveCriticalSection(&csVGVLautoOffState);
	return bState;
}


void CAlarmMgmtHandler::setVgarantAutoTurnedOff_Tube()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVgarantAutoTurnedOff_Tube=true;
	LeaveCriticalSection(&csVGVLautoOffState);
}
void CAlarmMgmtHandler::resetVgarantAutoTurnedOff_Tube()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVgarantAutoTurnedOff_Tube=false;
	LeaveCriticalSection(&csVGVLautoOffState);
}
bool CAlarmMgmtHandler::isVgarantAutoTurnedOff_Tube()
{
	EnterCriticalSection(&csVGVLautoOffState);
	bool bState=m_bVgarantAutoTurnedOff_Tube;
	LeaveCriticalSection(&csVGVLautoOffState);
	return bState;
}
void CAlarmMgmtHandler::setVlimitAutoTurnedOff_Tube()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVlimitAutoTurnedOff_Tube=true;
	LeaveCriticalSection(&csVGVLautoOffState);
}
void CAlarmMgmtHandler::resetVlimitAutoTurnedOff_Tube()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVlimitAutoTurnedOff_Tube=false;
	LeaveCriticalSection(&csVGVLautoOffState);
}
bool CAlarmMgmtHandler::isVlimitAutoTurnedOff_Tube()
{
	EnterCriticalSection(&csVGVLautoOffState);
	bool bState=m_bVlimitAutoTurnedOff_Tube;
	LeaveCriticalSection(&csVGVLautoOffState);
	return bState;
}

void CAlarmMgmtHandler::setPRICOAutoTurnedOff(bool bAutoTurnOn, eAlarmPrio prioAlarm)
{
	DEBUGMSG(TRUE, (TEXT("CAlarmMgmtHandler::setPRICOAutoTurnedOff()\r\n")));
	
	EnterCriticalSection(&csPRICOautoOffState);
	m_bPRICOAutoTurnedOff=true;
	m_bPRICOAutoTurnOn=bAutoTurnOn;
	m_ePRICOAutoAlarmPriority=prioAlarm;
	LeaveCriticalSection(&csPRICOautoOffState);
}
void CAlarmMgmtHandler::resetPRICOAutoTurnedOff()
{
	theApp.getLog()->WriteLine(_T("resetPRICOAutoTurnedOff"));
	DEBUGMSG(TRUE, (TEXT("CAlarmMgmtHandler::resetPRICOAutoTurnedOff()\r\n")));
	
	EnterCriticalSection(&csPRICOautoOffState);
	m_bPRICOAutoTurnedOff=false;
	m_bPRICOAutoTurnOn=false;
	m_ePRICOAutoAlarmPriority=AP_NONE;
	LeaveCriticalSection(&csPRICOautoOffState);
}
bool CAlarmMgmtHandler::isPRICOAutoTurnedOff()
{
	EnterCriticalSection(&csPRICOautoOffState);
	bool bState=m_bPRICOAutoTurnedOff;
	LeaveCriticalSection(&csPRICOautoOffState);
	return bState;
}
bool CAlarmMgmtHandler::isPRICOAutoTurneOn()
{
	EnterCriticalSection(&csPRICOautoOffState);
	bool bState=m_bPRICOAutoTurnOn;
	LeaveCriticalSection(&csPRICOautoOffState);
	return bState;
}
eAlarmPrio CAlarmMgmtHandler::getPRICOAutoAlarmPriority()
{
	EnterCriticalSection(&csPRICOautoOffState);
	eAlarmPrio ePrio=m_ePRICOAutoAlarmPriority;
	LeaveCriticalSection(&csPRICOautoOffState);
	return ePrio;
}


eAlarmLimitState CAlarmMgmtHandler::getAlimitState_MVmaxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_MVmaxLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_MVminLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_MVminLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_PIPmaxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_PIPmaxLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_PIPminLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_PIPminLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_PEEPminLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_PEEPminLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_BPMmaxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_BPMmaxLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_LeakmaxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_LeakmaxLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_ApnoeLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_ApnoeLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_DCO2maxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_DCO2maxLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_DCO2minLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_DCO2minLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_ETCO2maxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_ETCO2maxLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_ETCO2minLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_ETCO2minLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_FICO2maxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_FICO2maxLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_FICO2minLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_FICO2minLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_SPO2maxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_SPO2maxLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_SPO2minLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_SPO2minLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_PulseRatemaxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_PulseRatemaxLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_PulseRateminLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_PulseRateminLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_SPO2_PIminLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_SPO2_PIminLimit();
	}
	return state;
}
eAlarmLimitState CAlarmMgmtHandler::getAlimitState_SPO2_SIQminLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_SPO2_SIQminLimit();
	}
	return state;
}
//###############################################################
void CAlarmMgmtHandler::setAlimitApnoe(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitApnoe(value);
	}
}
void CAlarmMgmtHandler::setAlimitBPMmax(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitBPMmax(value);
	}
}
void CAlarmMgmtHandler::setAlimitMVmax(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMVmax(value);
	}
}
void CAlarmMgmtHandler::setAlimitMVmin(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMVmin(value);
	}
}
void CAlarmMgmtHandler::setAlimitLeakmax(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitLeakmax(value);
	}
}
void CAlarmMgmtHandler::setAlimitPEEPmin(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitPEEPmin(value);
	}
}
void CAlarmMgmtHandler::setAlimitPIPmax(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitPIPmax(value);
	}
}
void CAlarmMgmtHandler::setAlimitPIPmin(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitPIPmin(value);
	}
}
void CAlarmMgmtHandler::setAlimitDCO2max(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitDCO2max(value);
	}
}
void CAlarmMgmtHandler::setAlimitDCO2min(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitDCO2min(value);
	}
}
void CAlarmMgmtHandler::setAlimitETCO2max(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitETCO2max(value);
	}
}
void CAlarmMgmtHandler::setAlimitETCO2min(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitETCO2min(value);
	}
}
void CAlarmMgmtHandler::setAlimitFICO2max(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitFICO2max(value);
	}
}
void CAlarmMgmtHandler::setAlimitFICO2min(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitFICO2min(value);
	}
}
void CAlarmMgmtHandler::setAlimitSPO2max(int value)
{
	if(value==0)//testZERO
	{
		int iStop=0;
	}
	bool bUpdateLimitData=false;
	if(value>1000)
		value=1000;

	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitSPO2max(value);
	}

	getModel()->getALARMHANDLER()->setAlimitMaxRangeSPO2min(value-20);

	int iAlimitSPO2max=value/10;
	if(iAlimitSPO2max<getModel()->getDATAHANDLER()->getPRICO_SPO2highRange())
	{
		getModel()->getDATAHANDLER()->setPRICO_SPO2highRange(iAlimitSPO2max);
		bUpdateLimitData=true;
	}
	if(iAlimitSPO2max<=getModel()->getDATAHANDLER()->getPRICO_SPO2lowRange())
	{
		getModel()->getDATAHANDLER()->setPRICO_SPO2lowRange(iAlimitSPO2max-1);
		bUpdateLimitData=true;
	}

	if(bUpdateLimitData)
	{
		getModel()->getVIEWHANDLER()->UpdateLimitData();
	}
}
void CAlarmMgmtHandler::setAlimitSPO2min(int value)
{
	bool bUpdateLimitData=false;
	if(ALARMLIMITS)
	{
		DEBUGMSG(TRUE, (TEXT("setAlimitSPO2min %d\r\n"),value));
		ALARMLIMITS->setAlimitSPO2min(value);
	}
	getModel()->getALARMHANDLER()->setAlimitMinRangeSPO2max(value+20);

	int iAlimitSPO2min=value/10;
	if(iAlimitSPO2min>getModel()->getDATAHANDLER()->getPRICO_SPO2lowRange())
	{
		getModel()->getDATAHANDLER()->setPRICO_SPO2lowRange(iAlimitSPO2min);
		bUpdateLimitData=true;
	}
	if(iAlimitSPO2min>=getModel()->getDATAHANDLER()->getPRICO_SPO2highRange())
	{
		getModel()->getDATAHANDLER()->setPRICO_SPO2highRange(iAlimitSPO2min+1);
		bUpdateLimitData=true;
	}
	if(bUpdateLimitData)
	{
		getModel()->getVIEWHANDLER()->UpdateLimitData();
	}
}
void CAlarmMgmtHandler::setAlimitPulseRatemax(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitPulseRatemax(value);
	}
}
void CAlarmMgmtHandler::setAlimitPulseRatemin(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitPulseRatemin(value);
	}
}
void CAlarmMgmtHandler::setAlimitSPO2_PImin(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitSPO2_PImin(value);
	}
}
void CAlarmMgmtHandler::setAlimitSPO2_SIQmin(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitSPO2_SIQmin(value);
	}
}
//##########################################################################
void CAlarmMgmtHandler::setAlimitsMinMaxRangeCpap()
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitsMinMaxRangeCpap();
	}
}
void CAlarmMgmtHandler::setAlimitsMinMaxRangeNCPAP()
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitsMinMaxRangeNCPAP();
	}
}
void CAlarmMgmtHandler::setAlimitsMinMaxRangeDUOPAP()
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitsMinMaxRangeDUOPAP();
	}
}
void CAlarmMgmtHandler::setAlimitsMinMaxRangePressure()
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitsMinMaxRangePressure();
	}
}
//##########################################################################
int CAlarmMgmtHandler::getAlimitMVmin()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitMVmin();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitMVmax()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitMVmax();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitPIPmax()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitPIPmax();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitPIPmin()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitPIPmin();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitPEEPmin()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitPEEPmin();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitLeakmax()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitLeakmax();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitBPMmax()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitBPMmax();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitApnoe()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitApnoe();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitDCO2min()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitDCO2min();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitDCO2max()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitDCO2max();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitETCO2min()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitETCO2min();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitETCO2max()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitETCO2max();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitFICO2min()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitFICO2min();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitFICO2max()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitFICO2max();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitSPO2max()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitSPO2max();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitSPO2min()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitSPO2min();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitPulseRatemax()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitPulseRatemax();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitPulseRatemin()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitPulseRatemin();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitSPO2_PImin()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitSPO2_PImin();
	}
	return iLimit;
}
int CAlarmMgmtHandler::getAlimitSPO2_SIQmin()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitSPO2_SIQmin();
	}
	return iLimit;
}
//###############################################################
void CAlarmMgmtHandler::setAlimitMaxRangePIPmax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangePIPmax(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangePIPmax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangePIPmax(iRangeVal);
	}
}

void CAlarmMgmtHandler::setAlimitMaxRangePIPmin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangePIPmin(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangePIPmin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangePIPmin(iRangeVal);
	}
}

void CAlarmMgmtHandler::setAlimitMaxRangePEEPmin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangePEEPmin(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangePEEPmin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangePEEPmin(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMaxRangeMVmin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeMVmin(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangeMVmin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeMVmin(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMaxRangeMVmax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeMVmax(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangeMVmax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeMVmax(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMaxRangeLeakmax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeLeakmax(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangeLeakmax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeLeakmax(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMaxRangeBPMmax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeBPMmax(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangeBPMmax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeBPMmax(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMaxRangeApnoe(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeApnoe(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangeApnoe(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeApnoe(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMaxRangeDCO2min(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeDCO2min(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangeDCO2min(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeDCO2min(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMaxRangeDCO2max(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeDCO2max(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangeDCO2max(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeDCO2max(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMaxRangeETCO2min(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeETCO2min(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangeETCO2min(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeETCO2min(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMaxRangeETCO2max(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeETCO2max(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangeETCO2max(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeETCO2max(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMaxRangeFICO2min(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeFICO2min(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangeFICO2min(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeFICO2min(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMaxRangeFICO2max(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeFICO2max(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangeFICO2max(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeFICO2max(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMaxRangeSPO2max(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		//DEBUGMSG(TRUE, (TEXT("XXX setAlimitMaxRangeSPO2max %d\r\n"),iRangeVal));
		ALARMLIMITS->setAlimitMaxRangeSPO2max(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangeSPO2max(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		//DEBUGMSG(TRUE, (TEXT("XXX setAlimitMinRangeSPO2max %d\r\n"),iRangeVal));
		ALARMLIMITS->setAlimitMinRangeSPO2max(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMaxRangeSPO2min(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		//DEBUGMSG(TRUE, (TEXT("XXX setAlimitMaxRangeSPO2min %d\r\n"),iRangeVal));
		ALARMLIMITS->setAlimitMaxRangeSPO2min(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangeSPO2min(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		//DEBUGMSG(TRUE, (TEXT("XXX setAlimitMinRangeSPO2min %d\r\n"),iRangeVal));
		ALARMLIMITS->setAlimitMinRangeSPO2min(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMaxRangePulseRatemin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		//DEBUGMSG(TRUE, (TEXT("YYY setAlimitMaxRangePulseRatemin %d\r\n"),iRangeVal));
		ALARMLIMITS->setAlimitMaxRangePulseRatemin(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangePulseRatemax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		//DEBUGMSG(TRUE, (TEXT("YYY setAlimitMinRangePulseRatemax %d\r\n"),iRangeVal));
		ALARMLIMITS->setAlimitMinRangePulseRatemax(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMaxRangePulseRatemax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		//DEBUGMSG(TRUE, (TEXT("YYY setAlimitMaxRangePulseRatemax %d\r\n"),iRangeVal));
		ALARMLIMITS->setAlimitMaxRangePulseRatemax(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangePulseRatemin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		//DEBUGMSG(TRUE, (TEXT("YYY setAlimitMinRangePulseRatemin %d\r\n"),iRangeVal));
		ALARMLIMITS->setAlimitMinRangePulseRatemin(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMaxRangeSPO2_PImin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeSPO2_PImin(iRangeVal);
	}
}
void CAlarmMgmtHandler::setAlimitMinRangeSPO2_PImin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeSPO2_PImin(iRangeVal);
	}
}
//##########################################################
int CAlarmMgmtHandler::getAlimitMaxRangePIPmax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangePIPmax();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangePIPmax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangePIPmax();
	}
	return iRange;
}
//##########################################################
int CAlarmMgmtHandler::getAlimitMaxRangePIPmin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangePIPmin();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangePIPmin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangePIPmin();
	}
	return iRange;
}
//##########################################################
int CAlarmMgmtHandler::getAlimitMaxRangePEEPmin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangePEEPmin();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangePEEPmin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangePEEPmin();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMaxRangeMVmin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeMVmin();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangeMVmin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeMVmin();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMaxRangeMVmax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeMVmax();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangeMVmax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeMVmax();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMaxRangeLeakmax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeLeakmax();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangeLeakmax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeLeakmax();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMaxRangeBPMmax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeBPMmax();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangeBPMmax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeBPMmax();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMaxRangeApnoe()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeApnoe();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangeApnoe()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeApnoe();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMaxRangeDCO2min()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeDCO2min();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangeDCO2min()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeDCO2min();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMaxRangeDCO2max()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeDCO2max();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangeDCO2max()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeDCO2max();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMaxRangeETCO2min()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeETCO2min();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangeETCO2min()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeETCO2min();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMaxRangeETCO2max()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeETCO2max();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangeETCO2max()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeETCO2max();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMaxRangeFICO2min()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeFICO2min();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangeFICO2min()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeFICO2min();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMaxRangeFICO2max()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeFICO2max();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangeFICO2max()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeFICO2max();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMaxRangeSPO2max()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeSPO2max();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangeSPO2max()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeSPO2max();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMaxRangeSPO2min()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeSPO2min();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangeSPO2min()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeSPO2min();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMaxRangePulseRatemin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangePulseRatemin();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangePulseRatemax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangePulseRatemax();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMaxRangePulseRatemax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangePulseRatemax();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangePulseRatemin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangePulseRatemin();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMaxRangeSPO2_PImin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeSPO2_PImin();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangeSPO2_PImin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeSPO2_PImin();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMaxRangeSPO2_SIQmin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeSPO2_SIQmin();
	}
	return iRange;
}
int CAlarmMgmtHandler::getAlimitMinRangeSPO2_SIQmin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeSPO2_SIQmin();
	}
	return iRange;
}

//##################################################################################

//############################################################################
void CAlarmMgmtHandler::setAlimitState_ApnoeLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_ApnoeLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_BPMmaxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_BPMmaxLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_LeakmaxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_LeakmaxLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_MVmaxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_MVmaxLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_MVminLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_MVminLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_PEEPminLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_PEEPminLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_PIPmaxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_PIPmaxLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_PIPminLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_PIPminLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_DCO2maxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_DCO2maxLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_DCO2minLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_DCO2minLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_ETCO2maxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_ETCO2maxLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_ETCO2minLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_ETCO2minLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_FICO2maxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_FICO2maxLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_FICO2minLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_FICO2minLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_SPO2maxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		DEBUGMSG(TRUE, (TEXT("XXX setAlimitState_SPO2maxLimit %d\r\n"),(int)state));
		ALARMLIMITS->setAlimitState_SPO2maxLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_SPO2minLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		DEBUGMSG(TRUE, (TEXT("XXX setAlimitState_SPO2minLimit %d\r\n"),(int)state));
		ALARMLIMITS->setAlimitState_SPO2minLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_PulseRatemaxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_PulseRatemaxLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_PulseRateminLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_PulseRateminLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_SPO2_PIminLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_SPO2_PIminLimit(state);
	}
}
void CAlarmMgmtHandler::setAlimitState_SPO2_SIQminLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_SPO2_SIQminLimit(state);
	}
}
//#################################################################################################
void CAlarmMgmtHandler::SetAlimitStateCalculated()
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->SetAlimitStateCalculated();
	}
}


// **************************************************************************
// 
// **************************************************************************
void CAlarmMgmtHandler::CheckFlowSensorState()
{
	getModel()->CheckFlowsensorData();
}

// **************************************************************************
// 
// **************************************************************************
void CAlarmMgmtHandler::CheckAlarmStateBytes()
{
	getModel()->CheckAlarmData();
	g_eventNewAlarmData.SetEvent();
}

// **************************************************************************
// 
// **************************************************************************
void CAlarmMgmtHandler::checkLimits()
{
	// alarm with higher priority active???
	if(		getPrioActiveAlarm() < AP_7	
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating()
		||	getStartupSilent()==true)
		return;

	
	eVentMode eActiveVentMode=getSafeActiveVentMode();

	if(eActiveVentMode!=VM_THERAPIE)
	{
		checkVentilationLimits();
	}
	
	if(getModel()->getCONFIG()->GetCO2module()!=CO2MODULE_NONE)
	{
		checkCO2Limits();
	}

	if(getModel()->getCONFIG()->GetSPO2module()!=SPO2MODULE_NONE)
	{
		checkSpO2Limits();
	}
}

void CAlarmMgmtHandler::checkVentilationLimits()
{
	eVentMode eActiveVentMode=getSafeActiveVentMode();
	bool bActiveModeIsNMODE=false;

	if(eActiveVentMode==VM_NCPAP || eActiveVentMode==VM_DUOPAP || eActiveVentMode==VM_THERAPIE)
	{
		bActiveModeIsNMODE=true;
	}

	MESSURE_BLOCKDATA MessureData;
	MessureData.m_iPmax=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_P_PEAK);
	MessureData.m_iPmitt=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_P_MEAN);
	MessureData.m_iPEEP=getModel()->getDATAHANDLER()->getMessureDataBTB(ALINK_MSMNT_PEEP);
	MessureData.m_iCompliance=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DYNCOMPL);
	MessureData.m_iC20C=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_C20C);
	MessureData.m_iResistance=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_RESISTANCE);
	MessureData.m_iMV=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_MIN_VOL);
	MessureData.m_iPercent=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_ANTEIL_RESP);
	MessureData.m_iTVI=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVI);
	MessureData.m_iTVE=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE);
	MessureData.m_iTVEresp=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE_RESP);
	MessureData.m_iTVEpat=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE_PATIENT);
	MessureData.m_iBPM=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_FREQ);
	MessureData.m_iBPMco2=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_FREQETCO2);
	MessureData.m_iLeak=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_LEAK);
	MessureData.m_iHFAmpl=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_HFO_AMP);
	MessureData.m_iHFFreq=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_HFO_FREQ);
	MessureData.m_iTVEHFO=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TVE_HFO);
	MessureData.m_iDCO2=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_DCO2);
	MessureData.m_iTrigVol=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_TRIGGER);
	MessureData.m_iITimePSV=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_T_INSP_PSV);

	bool bMVmax_Alarm=false;
	bool bMVmin_Alarm=false;
	bool bPIPmax_Alarm=false;
	bool bPIPmin_Alarm=false;
	bool bPEEPminLow_Alarm=false;
	bool bPEEPminHigh_Alarm=false;
	bool bBPMmax_Alarm=false;
	bool bLeakmax_Alarm=false;	
	bool bDCO2max_Alarm=false;
	bool bDCO2min_Alarm=false;


	eAlarmLimitState stateMVmax = getAlimitState_MVmaxLimit();
	eAlarmLimitState stateMVmin = getAlimitState_MVminLimit();
	eAlarmLimitState statePIPmax = getAlimitState_PIPmaxLimit();
	eAlarmLimitState statePIPmin = getAlimitState_PIPminLimit();
	eAlarmLimitState statePEEPmin = getAlimitState_PEEPminLimit();
	eAlarmLimitState stateLeakmax = getAlimitState_LeakmaxLimit();
	eAlarmLimitState stateBPMmax = getAlimitState_BPMmaxLimit();
	eAlarmLimitState stateDCO2max = getAlimitState_DCO2maxLimit();
	eAlarmLimitState stateDCO2min = getAlimitState_DCO2minLimit();



	if(eActiveVentMode!=VM_CPAP && eActiveVentMode!=VM_NCPAP)
	{
		if(statePIPmax==AL_ON || statePIPmax==AL_AUTO)
		{
			if(	MessureData.m_iPmax > getAlimitPIPmax()
				|| MessureData.m_iPmitt > getAlimitPIPmax())
			{
				bPIPmax_Alarm=true;
			}
		}
	}

	if(eActiveVentMode!=VM_HFO && eActiveVentMode!=VM_CPAP && bActiveModeIsNMODE==false)
	{
		if(statePIPmin==AL_ON)
		{
			if(	MessureData.m_iPmax < getAlimitPIPmin())
			{
				bPIPmin_Alarm=true;
			}
		}
	}

	if(eActiveVentMode!=VM_CPAP && bActiveModeIsNMODE==false)
	{
		if(statePEEPmin==AL_ON || statePEEPmin==AL_AUTO)
		{
			if(	MessureData.m_iPEEP < getAlimitPEEPmin()
				|| MessureData.m_iPmitt < getAlimitPEEPmin())
			{
				bPEEPminLow_Alarm=true;
			}
		}
	}

	if(eActiveVentMode!=VM_HFO && bActiveModeIsNMODE==false)
	{
		if(statePEEPmin==AL_ON || statePEEPmin==AL_AUTO)
		{
			int iPEEPpara=0;
			switch(eActiveVentMode)
			{
			case VM_IPPV:
				{
					iPEEPpara=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_IPPV();
				}
				break;
			case VM_PSV:
			case VM_SIMV:
			case VM_SIMVPSV:
			case VM_SIPPV:
			case VM_HFO:
				{
					iPEEPpara=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_TRIGGER();
				}
				break;
			case VM_CPAP:
				{
					iPEEPpara=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPPara();
				}
				break;
			case VM_DUOPAP:
			case VM_NCPAP:
				{
					iPEEPpara=getModel()->getDATAHANDLER()->PARADATA()->GetCPAPNMODEPara();// may not occure!
				}
				break;
			default:
				{
					iPEEPpara=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_IPPV();
				}
				break;
			}
			if(	MessureData.m_iPEEP > iPEEPpara+40)
			{
				if(isSafeTickCountDelayExpired(m_dwPminHighValueOk, ALARMPEEPHIGHDELAY))
				{
					bPEEPminHigh_Alarm=true;
				}
			}
			else
			{
				m_dwPminHighValueOk=GetTickCount();
			}
		}
		else
		{
			m_dwPminHighValueOk=GetTickCount();
		}
	}
	else
	{
		m_dwPminHighValueOk=GetTickCount();
	}

	bool bCheckBPMinNMODE=false;
	if(		getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false 
		&&	getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET 
		&&	getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
	{
		bCheckBPMinNMODE=true;
	}
	else if(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
	{
		bCheckBPMinNMODE=true;
	}

	if(bActiveModeIsNMODE==false && getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
	{
		if(stateMVmax==AL_ON || stateMVmax==AL_AUTO)
		{
			if(	MessureData.m_iMV > getAlimitMVmax())
			{
				bMVmax_Alarm=true;
			}
		}

		if(stateMVmin==AL_ON || stateMVmin==AL_AUTO)
		{
			if( MessureData.m_iMV < getAlimitMVmin())
			{
				bMVmin_Alarm=true;
			}
		}

		if(eActiveVentMode!=VM_HFO)
		{
			if(stateLeakmax==AL_ON || stateLeakmax==AL_AUTO)
			{
				if(	MessureData.m_iLeak > getAlimitLeakmax())
				{
					bLeakmax_Alarm=true;
				}
			}

			if(stateBPMmax==AL_ON || stateBPMmax==AL_AUTO)
			{
				if(	MessureData.m_iBPM > getAlimitBPMmax())
				{
					bBPMmax_Alarm=true;
				}
			}
		}
		else
		{
			if(stateLeakmax==AL_ON || stateLeakmax==AL_AUTO)
			{
				if(	MessureData.m_iLeak > getAlimitLeakmax())
				{
					bLeakmax_Alarm=true;
				}
			}

			if(stateDCO2max==AL_ON || stateDCO2max==AL_AUTO)
			{
				if(	MessureData.m_iDCO2 > getAlimitDCO2max())
				{
					bDCO2max_Alarm=true;
				}
			}

			if(stateDCO2min==AL_ON || stateDCO2min==AL_AUTO)
			{
				if( MessureData.m_iDCO2 < getAlimitDCO2min())
				{
					bDCO2min_Alarm=true;
				}
			}
		}
	}
	else if(bCheckBPMinNMODE)
	{
		if(stateBPMmax==AL_ON || stateBPMmax==AL_AUTO)
		{
			if(	MessureData.m_iBPM > getAlimitBPMmax())
			{
				bBPMmax_Alarm=true;
			}
		}
	}

	//check current states of alarmlimit alarms -> update
	tALARMSTATES eAlarmStates;
	EnterCriticalSection(&csAlarmList);
	eAlarmStates.eSoA_MVmaxLimit=m_pAlarmlist->getAlarm(AL_PatAl_MVmax)->getAlarmState();
	eAlarmStates.eSoA_MVminLimit=m_pAlarmlist->getAlarm(AL_PatAl_MVmin)->getAlarmState();
	eAlarmStates.eSoA_PIPmaxLimit=m_pAlarmlist->getAlarm(AL_PatAl_PIPmax)->getAlarmState();
	eAlarmStates.eSoA_PIPminLimit=m_pAlarmlist->getAlarm(AL_PatAl_PIPmin)->getAlarmState();
	eAlarmStates.eSoA_PEEPminLowLimit=m_pAlarmlist->getAlarm(AL_PatAl_PEEPminLow)->getAlarmState();
	eAlarmStates.eSoA_PEEPminHighLimit=m_pAlarmlist->getAlarm(AL_PatAl_PEEPminHigh)->getAlarmState();
	eAlarmStates.eSoA_LeakmaxLimit=m_pAlarmlist->getAlarm(AL_PatAl_Leakmax)->getAlarmState();
	eAlarmStates.eSoA_BPMmaxLimit=m_pAlarmlist->getAlarm(AL_PatAl_BPMmax)->getAlarmState();
	eAlarmStates.eSoA_DCO2maxLimit=m_pAlarmlist->getAlarm(AL_PatAl_DCO2max)->getAlarmState();
	eAlarmStates.eSoA_DCO2minLimit=m_pAlarmlist->getAlarm(AL_PatAl_DCO2min)->getAlarmState();
	LeaveCriticalSection(&csAlarmList);

	CStringW sz = _T("");
	if(		bMVmax_Alarm 
		&&	eAlarmStates.eSoA_MVmaxLimit!=AS_ACTIVE 
		&&	getAlarmSilentState()!=ASTATE_AUTOSILENT
		&&	getModel()->isActiveAlarmDelay()==false)
	{
		sz.Format(_T(" [%0.2f]"),CTlsFloat::Round(((double)MessureData.m_iMV)/1000, 2));
		//SetAlarm_MVmax(sz);
		setAlarm(AL_PatAl_MVmax,sz);
	}
	else if(!bMVmax_Alarm && eAlarmStates.eSoA_MVmaxLimit==AS_ACTIVE)
	{
		setStateOfAlarm(AL_PatAl_MVmax,AS_SIGNALED);
	}

	if(		bMVmin_Alarm 
		&&	eAlarmStates.eSoA_MVminLimit!=AS_ACTIVE 
		&&	getAlarmSilentState()!=ASTATE_AUTOSILENT
		&&	getModel()->isActiveAlarmDelay()==false)
	{
		sz.Format(_T(" [%0.2f]"),CTlsFloat::Round(((double)MessureData.m_iMV)/1000, 2));
		//SetAlarm_MVmin(sz);
		setAlarm(AL_PatAl_MVmin,sz);
	}
	else if(!bMVmin_Alarm && eAlarmStates.eSoA_MVminLimit==AS_ACTIVE)
	{
		setStateOfAlarm(AL_PatAl_MVmin,AS_SIGNALED);
	}


	if(		bPIPmax_Alarm 
		&&	eAlarmStates.eSoA_PIPmaxLimit!=AS_ACTIVE 
		&&	getAlarmSilentState()!=ASTATE_AUTOSILENT
		&&	getModel()->isActiveAlarmDelay()==false)
	{
		sz.Format(_T(" [%0.0f]"),CTlsFloat::Round(((double)MessureData.m_iPmax)/10, 0) );

		if(getModel()->IsPatAlarmO2dependendEnabled())
		{
			setAlarm(AL_PatAl_PIPmax,sz);
		}
	}
	else if(!bPIPmax_Alarm && eAlarmStates.eSoA_PIPmaxLimit==AS_ACTIVE)
	{
		if(eActiveVentMode!=VM_CPAP && eActiveVentMode!=VM_NCPAP)
			setStateOfAlarm(AL_PatAl_PIPmax,AS_SIGNALED);
	}

	if(		bPIPmin_Alarm 
		&&	eAlarmStates.eSoA_PIPminLimit!=AS_ACTIVE 
		&&	getAlarmSilentState()!=ASTATE_AUTOSILENT
		&&	getModel()->isActiveAlarmDelay()==false)
	{
		sz.Format(_T(" [%0.0f]"),CTlsFloat::Round(((double)MessureData.m_iPmax)/10, 0) );

		if(getModel()->IsPatAlarmO2dependendEnabled())
		{
			setAlarm(AL_PatAl_PIPmin,sz);
		}
	}
	else if(!bPIPmin_Alarm && eAlarmStates.eSoA_PIPminLimit==AS_ACTIVE)
	{
		if(eActiveVentMode!=VM_HFO && eActiveVentMode!=VM_CPAP && bActiveModeIsNMODE==false)
			setStateOfAlarm(AL_PatAl_PIPmin,AS_SIGNALED);
	}

	if(eActiveVentMode!=VM_CPAP && bActiveModeIsNMODE==false)
	{
		if(		bPEEPminLow_Alarm 
			&&	eAlarmStates.eSoA_PEEPminLowLimit!=AS_ACTIVE 
			&&	getAlarmSilentState()!=ASTATE_AUTOSILENT
			&&	getModel()->isActiveAlarmDelay()==false)
		{
			sz.Format(_T(" [%0.1f]"),CTlsFloat::Round(((double)MessureData.m_iPEEP)/10, 1));
			
			if(getModel()->IsPatAlarmO2dependendEnabled())
			{
				setAlarm(AL_PatAl_PEEPminLow,sz);
			}
		}
		else if(!bPEEPminLow_Alarm && eAlarmStates.eSoA_PEEPminLowLimit==AS_ACTIVE)
		{
			setStateOfAlarm(AL_PatAl_PEEPminLow,AS_SIGNALED);
		}
		
	}

	if(eActiveVentMode!=VM_HFO && bActiveModeIsNMODE==false)
	{
		if(		bPEEPminHigh_Alarm 
			&&	eAlarmStates.eSoA_PEEPminHighLimit!=AS_ACTIVE 
			&&	getAlarmSilentState()!=ASTATE_AUTOSILENT
			&&	getModel()->isActiveAlarmDelay()==false)
		{
			sz.Format(_T(" [%0.1f]"),CTlsFloat::Round(((double)MessureData.m_iPEEP)/10, 1));
			
			if(getModel()->IsPatAlarmO2dependendEnabled())
			{
				setAlarm(AL_PatAl_PEEPminHigh,sz);
			}
		}
		else if(!bPEEPminHigh_Alarm && eAlarmStates.eSoA_PEEPminHighLimit==AS_ACTIVE)
		{
			setStateOfAlarm(AL_PatAl_PEEPminHigh,AS_SIGNALED);
		}

	}
	

	if(		bLeakmax_Alarm 
		&&	eAlarmStates.eSoA_LeakmaxLimit!=AS_ACTIVE 
		&&	getAlarmSilentState()!=ASTATE_AUTOSILENT
		&&	getModel()->isActiveAlarmDelay()==false)
	{
		sz.Format(_T(" [%d]"),MessureData.m_iLeak);
		setAlarm(AL_PatAl_Leakmax,sz);
	}
	else if(!bLeakmax_Alarm && eAlarmStates.eSoA_LeakmaxLimit==AS_ACTIVE)
	{
		setStateOfAlarm(AL_PatAl_Leakmax,AS_SIGNALED);
	}

	if(		bBPMmax_Alarm 
		&&	eAlarmStates.eSoA_BPMmaxLimit!=AS_ACTIVE 
		&&	getAlarmSilentState()!=ASTATE_AUTOSILENT
		&&	getModel()->isActiveAlarmDelay()==false)
	{
		if(bCheckBPMinNMODE)
			sz.Format(_T(" [%d]"),MessureData.m_iBPM);
		else if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
			sz.Format(_T(" [%d]"),MessureData.m_iBPMco2);
		else
			sz.Format(_T(" [%d]"),MessureData.m_iBPM);
		setAlarm(AL_PatAl_BPMmax,sz);
	}
	else if(!bBPMmax_Alarm && eAlarmStates.eSoA_BPMmaxLimit==AS_ACTIVE)
	{
		setStateOfAlarm(AL_PatAl_BPMmax,AS_SIGNALED);
	}

	if(eActiveVentMode==VM_HFO)
	{
		if(		bDCO2max_Alarm 
			&&	eAlarmStates.eSoA_DCO2maxLimit!=AS_ACTIVE 
			&&	getAlarmSilentState()!=ASTATE_AUTOSILENT
			&&	getModel()->isActiveAlarmDelay()==false)
		{
			sz.Format(_T(" [%d]"),MessureData.m_iDCO2);
			setAlarm(AL_PatAl_DCO2max,sz);
		}
		else if(!bDCO2max_Alarm && eAlarmStates.eSoA_DCO2maxLimit==AS_ACTIVE)
		{
			setStateOfAlarm(AL_PatAl_DCO2max,AS_SIGNALED);
		}

		if(		bDCO2min_Alarm 
			&&	eAlarmStates.eSoA_DCO2minLimit!=AS_ACTIVE 
			&&	getAlarmSilentState()!=ASTATE_AUTOSILENT
			&&	getModel()->isActiveAlarmDelay()==false)
		{
			sz.Format(_T(" [%d]"),MessureData.m_iDCO2);
			setAlarm(AL_PatAl_DCO2min,sz);
		}
		else if(!bDCO2min_Alarm && eAlarmStates.eSoA_DCO2minLimit==AS_ACTIVE)
		{
			setStateOfAlarm(AL_PatAl_DCO2min,AS_SIGNALED);
		}
	}
}

void CAlarmMgmtHandler::checkCO2Limits()
{
	if(getModel()->getCONFIG()->GetCO2module()!=CO2MODULE_NONE && getModel()->getETCO2())
	{
		bool bETCO2state=getModel()->getETCO2()->isStateOk();
		bool bCO2ValueValid=getModel()->getETCO2()->isCO2ValueValid();
		bool bETCO2ValueValid=getModel()->getETCO2()->isETCO2ValueValid();
		bool bFICO2ValueValid = getModel()->getETCO2()->isFICO2ValueValid();
		bool bFreqValueValid = getModel()->getETCO2()->isFreqValueValid();

		BYTE iBPMco2=getModel()->getDATAHANDLER()->getMessureDataAVG(ALINK_MSMNT_FREQETCO2);

		bool bETCO2max_Alarm=false;
		bool bETCO2min_Alarm=false;
		bool bFICO2max_Alarm=false;
		bool bFICO2min_Alarm=false;
		bool bBPMmax_Alarm=false;

		bool bCO2valid=false;
		bool bEtCO2valid=false;
		bool bFICO2valid=false;

		CStringW sz = _T("");

		SHORT iETCO2=0;
		SHORT iFICO2=0;
		
		eAlarmLimitState stateETCO2max = getAlimitState_ETCO2maxLimit();
		eAlarmLimitState stateETCO2min = getAlimitState_ETCO2minLimit();
		eAlarmLimitState stateBPMmax = getAlimitState_BPMmaxLimit();

		if(		false==bETCO2state
			||	false==bCO2ValueValid)
			bCO2valid=false;
		else
			bCO2valid=true;

		if(		false==bETCO2state
			||	false==bETCO2ValueValid)
			bEtCO2valid=false;
		else
			bEtCO2valid=true;

		iETCO2=getModel()->getDATAHANDLER()->getAVGMessureDataETCO2();

		if(bCO2valid && bEtCO2valid)
		{
			if(stateETCO2max==AL_ON || stateETCO2max==AL_AUTO)
			{
				if(	iETCO2 > getAlimitETCO2max())
				{
					bETCO2max_Alarm=true;
					//int iTest=getAlimitETCO2max();
				}
			}

			if(stateETCO2min==AL_ON || stateETCO2min==AL_AUTO)
			{
				if( iETCO2 < getAlimitETCO2min())
				{
					bETCO2min_Alarm=true;
				}
			}
		}

		eAlarmLimitState stateFICO2max = getAlimitState_FICO2maxLimit();
		eAlarmLimitState stateFICO2min = getAlimitState_FICO2minLimit();

		if(		false==bETCO2state
			||	false==bFICO2ValueValid)
			bFICO2valid=false;
		else
			bFICO2valid=true;

		iFICO2=getModel()->getDATAHANDLER()->getAVGMessureDataFICO2();

		if(bCO2valid && bFICO2valid)
		{
			if(stateFICO2max==AL_ON || stateFICO2max==AL_AUTO)
			{
				if(	iFICO2 > getAlimitFICO2max())
				{
					bFICO2max_Alarm=true;
					//int iTest=getAlimitETCO2max();
				}
			}

			if(stateFICO2min==AL_ON || stateFICO2min==AL_AUTO)
			{
				if( iFICO2 < getAlimitFICO2min())
				{
					bFICO2min_Alarm=true;
				}
			}
		}

		bool bFreqvalid=false;
		if(		false==bETCO2state
			||	false==bFreqValueValid)
			bFreqvalid=false;
		else
			bFreqvalid=true;

		if(bCO2valid && bFreqvalid && getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
		{
			if(stateBPMmax==AL_ON || stateBPMmax==AL_AUTO)
			{
				if(	iBPMco2 > getAlimitBPMmax())
				{
					bBPMmax_Alarm=true;
				}
			}
		}

		tALARMSTATES eAlarmStates;
		EnterCriticalSection(&csAlarmList);
		eAlarmStates.eSoA_ETCO2maxLimit=m_pAlarmlist->getAlarm(AL_PatAl_ETCO2max)->getAlarmState();
		eAlarmStates.eSoA_ETCO2minLimit=m_pAlarmlist->getAlarm(AL_PatAl_ETCO2min)->getAlarmState();
		eAlarmStates.eSoA_FICO2maxLimit=m_pAlarmlist->getAlarm(AL_PatAl_FICO2max)->getAlarmState();
		eAlarmStates.eSoA_FICO2minLimit=m_pAlarmlist->getAlarm(AL_PatAl_FICO2min)->getAlarmState();
		LeaveCriticalSection(&csAlarmList);

		if(		bETCO2max_Alarm 
			&&	eAlarmStates.eSoA_ETCO2maxLimit!=AS_ACTIVE 
			&&	getAlarmSilentState()!=ASTATE_AUTOSILENT
			&&	getModel()->isActiveAlarmDelay()==false)
		{
			CStringW csUnit = _T("");
			eCO2unit unit=getModel()->getCONFIG()->GetCO2unit();
			switch(unit)
			{
			case CO2_UNIT_KPA:
				{
					csUnit =getModel()->GetLanguageString(IDS_UNIT_KPA);
				}
				break;
			case CO2_UNIT_PERCENT:
				{
					csUnit =getModel()->GetLanguageString(IDS_UNIT_VOLPERCENT);
				}
				break;
			case CO2_UNIT_MMHG:
			default:
				{
					csUnit =getModel()->GetLanguageString(IDS_UNIT_MMHG);
				}
				break;
			}
			sz.Format(_T(" [%0.1f]"), (double)iETCO2/10);
			setAlarm(AL_PatAl_ETCO2max,sz);
		}
		else if(!bETCO2max_Alarm && eAlarmStates.eSoA_ETCO2maxLimit==AS_ACTIVE)
		{
			setStateOfAlarm(AL_PatAl_ETCO2max,AS_SIGNALED);
		}

		if(		bETCO2min_Alarm 
			&&	eAlarmStates.eSoA_ETCO2minLimit!=AS_ACTIVE 
			&&	getAlarmSilentState()!=ASTATE_AUTOSILENT
			&&	getModel()->isActiveAlarmDelay()==false)
		{
			CStringW csUnit = _T("");
			eCO2unit unit=getModel()->getCONFIG()->GetCO2unit();
			switch(unit)
			{
			case CO2_UNIT_KPA:
				{
					csUnit =getModel()->GetLanguageString(IDS_UNIT_KPA);
				}
				break;
			case CO2_UNIT_PERCENT:
				{
					csUnit =getModel()->GetLanguageString(IDS_UNIT_VOLPERCENT);
				}
				break;
			case CO2_UNIT_MMHG:
			default:
				{
					csUnit =getModel()->GetLanguageString(IDS_UNIT_MMHG);
				}
				break;
			}
			sz.Format(_T(" [%0.1f]"),(double)iETCO2/10);
			setAlarm(AL_PatAl_ETCO2min,sz);
		}
		else if(!bETCO2min_Alarm && eAlarmStates.eSoA_ETCO2minLimit==AS_ACTIVE)
		{
			setStateOfAlarm(AL_PatAl_ETCO2min,AS_SIGNALED);
		}

		//#############FICO2
		if(		bFICO2max_Alarm 
			&&	eAlarmStates.eSoA_FICO2maxLimit!=AS_ACTIVE 
			&&	getAlarmSilentState()!=ASTATE_AUTOSILENT
			&&	getModel()->isActiveAlarmDelay()==false)
		{
			CStringW csUnit = _T("");
			eCO2unit unit=getModel()->getCONFIG()->GetCO2unit();
			switch(unit)
			{
			case CO2_UNIT_KPA:
				{
					csUnit =getModel()->GetLanguageString(IDS_UNIT_KPA);
				}
				break;
			case CO2_UNIT_PERCENT:
				{
					csUnit =getModel()->GetLanguageString(IDS_UNIT_VOLPERCENT);
				}
				break;
			case CO2_UNIT_MMHG:
			default:
				{
					csUnit =getModel()->GetLanguageString(IDS_UNIT_MMHG);
				}
				break;
			}
			sz.Format(_T(" [%0.1f]"), (double)iFICO2/10);
			setAlarm(AL_PatAl_FICO2max,sz);
		}
		else if(!bFICO2max_Alarm && eAlarmStates.eSoA_FICO2maxLimit==AS_ACTIVE)
		{
			setStateOfAlarm(AL_PatAl_FICO2max,AS_SIGNALED);
		}

		if(		bFICO2min_Alarm 
			&&	eAlarmStates.eSoA_FICO2minLimit!=AS_ACTIVE 
			&&	getAlarmSilentState()!=ASTATE_AUTOSILENT
			&&	getModel()->isActiveAlarmDelay()==false)
		{
			CStringW csUnit = _T("");
			eCO2unit unit=getModel()->getCONFIG()->GetCO2unit();
			switch(unit)
			{
			case CO2_UNIT_KPA:
				{
					csUnit =getModel()->GetLanguageString(IDS_UNIT_KPA);
				}
				break;
			case CO2_UNIT_PERCENT:
				{
					csUnit =getModel()->GetLanguageString(IDS_UNIT_VOLPERCENT);
				}
				break;
			case CO2_UNIT_MMHG:
			default:
				{
					csUnit =getModel()->GetLanguageString(IDS_UNIT_MMHG);
				}
				break;
			}
			sz.Format(_T(" [%0.1f]"),(double)iFICO2/10);
			setAlarm(AL_PatAl_FICO2min,sz);
		}
		else if(!bFICO2min_Alarm && eAlarmStates.eSoA_FICO2minLimit==AS_ACTIVE)
		{
			setStateOfAlarm(AL_PatAl_FICO2min,AS_SIGNALED);
		}
	}


	
}

void CAlarmMgmtHandler::checkSpO2Limits()
{
	if(getModel()->getSPO2() && getModel()->getSPO2()->isStateOk())
	{
		bool bSPO2max_Alarm=false;
		bool bSPO2min_Alarm=false;
		bool bPulseRatemax_Alarm=false;
		bool bPulseRatemin_Alarm=false;
		bool bSPO2_PImin_Alarm=false;

		CStringW sz = _T("");

		SHORT iSPO2=0;
		SHORT iPulseRate=0;
		SHORT iPerfusionIndex=0;

		eAlarmLimitState stateSPO2max = getAlimitState_SPO2maxLimit();
		eAlarmLimitState stateSPO2min = getAlimitState_SPO2minLimit();
		eAlarmLimitState statePulseRatemax = getAlimitState_PulseRatemaxLimit();
		eAlarmLimitState statePulseRatemin = getAlimitState_PulseRateminLimit();
		eAlarmLimitState stateSPO2_PImin = getAlimitState_SPO2_PIminLimit();


		iSPO2=CTlsFloat::Round(((double)getModel()->getDATAHANDLER()->getAVGMessureDataSPO2())/10, 0)*10;
		iPulseRate=getModel()->getDATAHANDLER()->getAVGMessureDataSpO2PulseRate();
		iPerfusionIndex=getModel()->getDATAHANDLER()->getAVGMessureDataSpO2PerfusionIndex();
		int iDelayTime=getModel()->getCONFIG()->getSPO2alarmdelay();

		if(stateSPO2max==AL_ON || stateSPO2max==AL_AUTO)
		{
			if(	iSPO2 > getAlimitSPO2max())
			{
				DEBUGMSG(TRUE, (TEXT("iSPO2 > getAlimitSPO2max()\r\n")));
				if(AS_ACTIVE==ALARM_PatAl_SPO2max->getAlarmState())
				{
					DEBUGMSG(TRUE, (TEXT("bSPO2max_Alarm=true 3\r\n")));
					bSPO2max_Alarm=true;
				}
				else if(iDelayTime!=0)
				{
					if(ALARM_PatAl_SPO2max->getTriggerTimestamp()==0)
					{
						DEBUGMSG(TRUE, (TEXT("ALARM_PatAl_SPO2max->getTriggerTimestamp()==0\r\n")));
						ALARM_PatAl_SPO2max->setTriggerTimestamp();
					}
					else if(isSafeTickCountDelayExpired(ALARM_PatAl_SPO2max->getTriggerTimestamp(), ALARM_PatAl_SPO2max->getAlarmDelayTimeMSec()))//rkuTICKCOUNT
					{
						DEBUGMSG(TRUE, (TEXT("bSPO2max_Alarm=true 1\r\n")));
						bSPO2max_Alarm=true;
					}
				}
				else
				{
					bSPO2max_Alarm=true;
					DEBUGMSG(TRUE, (TEXT("bSPO2max_Alarm=true 2\r\n")));
				}
			}
			else if(ALARM_PatAl_SPO2max->getTriggerTimestamp()!=0)
			{
				DEBUGMSG(TRUE, (TEXT("SPO2max resetTriggerTimestamp\r\n")));
				ALARM_PatAl_SPO2max->resetTriggerTimestamp();
			}
		}

		if(stateSPO2min==AL_ON || stateSPO2min==AL_AUTO)
		{
			if( iSPO2 < getAlimitSPO2min())
			{
				if(AS_ACTIVE==ALARM_PatAl_SPO2min->getAlarmState())
				{
					bSPO2min_Alarm=true;
				}
				else if(iDelayTime!=0)
				{
					if(ALARM_PatAl_SPO2min->getTriggerTimestamp()==0)
					{
						ALARM_PatAl_SPO2min->setTriggerTimestamp();
					}
					else if(isSafeTickCountDelayExpired(ALARM_PatAl_SPO2min->getTriggerTimestamp(), ALARM_PatAl_SPO2min->getAlarmDelayTimeMSec()))//rkuTICKCOUNT
					{
						bSPO2min_Alarm=true;
					}
				}
				else
				{
					bSPO2min_Alarm=true;
				}
				
			}
			else if(ALARM_PatAl_SPO2min->getTriggerTimestamp()!=0)
			{
				ALARM_PatAl_SPO2min->resetTriggerTimestamp();
			}
		}


		if(statePulseRatemax==AL_ON || statePulseRatemax==AL_AUTO)
		{
			if(	iPulseRate > getAlimitPulseRatemax())
			{
				if(AS_ACTIVE==ALARM_PatAl_PulseRatemax->getAlarmState())
				{
					bPulseRatemax_Alarm=true;
				}
				else if(iDelayTime!=0)
				{
					if(ALARM_PatAl_PulseRatemax->getTriggerTimestamp()==0)
					{
						ALARM_PatAl_PulseRatemax->setTriggerTimestamp();
					}
					else if(isSafeTickCountDelayExpired(ALARM_PatAl_PulseRatemax->getTriggerTimestamp(), ALARM_PatAl_PulseRatemax->getAlarmDelayTimeMSec()))//rkuTICKCOUNT
					{
						bPulseRatemax_Alarm=true;
					}
				}
				else
				{
					bPulseRatemax_Alarm=true;
				}
			}
			else if(ALARM_PatAl_PulseRatemax->getTriggerTimestamp()!=0)
			{
				ALARM_PatAl_PulseRatemax->resetTriggerTimestamp();
			}
		}

		if(statePulseRatemin==AL_ON || statePulseRatemin==AL_AUTO)
		{
			if( iPulseRate < getAlimitPulseRatemin())
			{
				if(AS_ACTIVE==ALARM_PatAl_PulseRatemin->getAlarmState())
				{
					bPulseRatemin_Alarm=true;
				}
				else if(iDelayTime!=0)
				{
					if(ALARM_PatAl_PulseRatemin->getTriggerTimestamp()==0)
					{
						ALARM_PatAl_PulseRatemin->setTriggerTimestamp();
					}
					else if(isSafeTickCountDelayExpired(ALARM_PatAl_PulseRatemin->getTriggerTimestamp(), ALARM_PatAl_PulseRatemin->getAlarmDelayTimeMSec()))//rkuTICKCOUNT
					{
						bPulseRatemin_Alarm=true;
					}
				}
				else
				{
					DEBUGMSG(TRUE, (TEXT("bPulseRatemin_Alarm 2\r\n")));
					bPulseRatemin_Alarm=true;
				}
			}
			else if(ALARM_PatAl_PulseRatemin->getTriggerTimestamp()!=0)
			{
				ALARM_PatAl_PulseRatemin->resetTriggerTimestamp();
			}
		}

		if(stateSPO2_PImin==AL_ON || stateSPO2_PImin==AL_AUTO)
		{
			if( iPerfusionIndex < getAlimitSPO2_PImin())
			{
				if(AS_ACTIVE==ALARM_PatAl_SPO2_PImin->getAlarmState())
				{
					bSPO2_PImin_Alarm=true;
				}
				else if(iDelayTime!=0)
				{
					if(ALARM_PatAl_SPO2_PImin->getTriggerTimestamp()==0)
					{
						ALARM_PatAl_SPO2_PImin->setTriggerTimestamp();
					}
					else if(isSafeTickCountDelayExpired(ALARM_PatAl_SPO2_PImin->getTriggerTimestamp(), ALARM_PatAl_SPO2_PImin->getAlarmDelayTimeMSec()))//rkuTICKCOUNT
					{
						bSPO2_PImin_Alarm=true;
					}
				}
				else
				{
					bSPO2_PImin_Alarm=true;
				}
			}
			else if(ALARM_PatAl_SPO2_PImin->getTriggerTimestamp()!=0)
			{
				ALARM_PatAl_SPO2_PImin->resetTriggerTimestamp();
			}
		}

		

		/*********************high SPO2***************************/
		if(		bSPO2max_Alarm 
			&&	ALARM_PatAl_SPO2max->getAlarmState()!=AS_ACTIVE 
			&&	getAlarmSilentState()!=ASTATE_AUTOSILENT)
		{
			sz.Format(_T(" [%0.1f]"),(double)iSPO2/10);
			setAlarm(AL_PatAl_SPO2max,sz);
		}
		else if(!bSPO2max_Alarm && ALARM_PatAl_SPO2max->getAlarmState()==AS_ACTIVE)
		{
			setStateOfAlarm(AL_PatAl_SPO2max,AS_SIGNALED);
		}

		/*********************low SPO2***************************/
		if(		bSPO2min_Alarm 
			&&	ALARM_PatAl_SPO2min->getAlarmState()!=AS_ACTIVE 
			&&	getAlarmSilentState()!=ASTATE_AUTOSILENT)
		{
			sz.Format(_T(" [%0.1f]"),(double)iSPO2/10);
			setAlarm(AL_PatAl_SPO2min,sz);
		}
		else if(!bSPO2min_Alarm && ALARM_PatAl_SPO2min->getAlarmState()==AS_ACTIVE)
		{
			setStateOfAlarm(AL_PatAl_SPO2min,AS_SIGNALED);
		}

		/*********************high PulseRate***************************/
		if(		bPulseRatemax_Alarm 
			&&	ALARM_PatAl_PulseRatemax->getAlarmState()!=AS_ACTIVE 
			&&	getAlarmSilentState()!=ASTATE_AUTOSILENT)
		{
			sz.Format(_T(" [%0.1f]"),(double)iPulseRate/10);
			setAlarm(AL_PatAl_PulseRatemax,sz);
		}
		else if(!bPulseRatemax_Alarm && ALARM_PatAl_PulseRatemax->getAlarmState()==AS_ACTIVE)
		{
			setStateOfAlarm(AL_PatAl_PulseRatemax,AS_SIGNALED);
		}

		/*********************low PulseRate***************************/
		if(		bPulseRatemin_Alarm 
			&&	ALARM_PatAl_PulseRatemin->getAlarmState()!=AS_ACTIVE 
			&&	getAlarmSilentState()!=ASTATE_AUTOSILENT)
		{
			sz.Format(_T(" [%0.1f]"),(double)iPulseRate/10);
			setAlarm(AL_PatAl_PulseRatemin,sz);
		}
		else if(!bPulseRatemin_Alarm && ALARM_PatAl_PulseRatemin->getAlarmState()==AS_ACTIVE)
		{
			setStateOfAlarm(AL_PatAl_PulseRatemin,AS_SIGNALED);
		}

		/*********************low PerfusionIndex***************************/
		if(		bSPO2_PImin_Alarm 
			&&	ALARM_PatAl_SPO2_PImin->getAlarmState()!=AS_ACTIVE 
			&&	getAlarmSilentState()!=ASTATE_AUTOSILENT)
		{
			sz.Format(_T(" [%0.1f]"),(double)iPerfusionIndex/10);
			setAlarm(AL_PatAl_SPO2_PImin,sz);
		}
		else if(!bSPO2_PImin_Alarm && ALARM_PatAl_SPO2_PImin->getAlarmState()==AS_ACTIVE)
		{
			setStateOfAlarm(AL_PatAl_SPO2_PImin,AS_SIGNALED);
		}


		bool bSIQ_Alarm=false;
		SHORT iSpO2SIQ = getModel()->getDATAHANDLER()->readSpO2SIQ();
		SHORT iSpO2SIQlimit = getModel()->getALARMHANDLER()->getAlimitSPO2_SIQmin();


		if( iSpO2SIQ<iSpO2SIQlimit)
		{
			if(AS_ACTIVE==ALARM_PatAl_SPO2_SIQmin->getAlarmState())
			{
				bSIQ_Alarm=true;
				//DEBUGMSG(TRUE, (TEXT("SIQ alarm1\r\n")));
			}
			else if(iDelayTime!=0)
			{
				if(ALARM_PatAl_SPO2_SIQmin->getTriggerTimestamp()==0)
				{
					ALARM_PatAl_SPO2_SIQmin->setTriggerTimestamp();
				}
				else if(isSafeTickCountDelayExpired(ALARM_PatAl_SPO2_SIQmin->getTriggerTimestamp(), ALARM_PatAl_SPO2_SIQmin->getAlarmDelayTimeMSec()))//rkuTICKCOUNT
				{
					bSIQ_Alarm=true;
				}
			}
			else
			{
				bSIQ_Alarm=true;
				DEBUGMSG(TRUE, (TEXT("SIQ alarm3\r\n")));
			}
		}
		else if(ALARM_PatAl_SPO2_SIQmin->getTriggerTimestamp()!=0)
		{
			ALARM_PatAl_SPO2_SIQmin->resetTriggerTimestamp();
			DEBUGMSG(TRUE, (TEXT("reset SIQ alarm\r\n")));
		}

		if(		bSIQ_Alarm 
			&&	ALARM_PatAl_SPO2_SIQmin->getAlarmState()!=AS_ACTIVE 
			&&	getAlarmSilentState()!=ASTATE_AUTOSILENT)
		{
			/*if(getModel()->getPRICOThread()->isPRICOrunning())
			{
				getModel()->getPRICOThread()->setAlarmDisabled();
				getModel()->getDATAHANDLER()->setPRICOState(false);
			}*/
			sz.Format(_T(" [%d]"),iSpO2SIQ);
			setAlarm(AL_PatAl_SPO2_SIQmin,sz);
			DEBUGMSG(TRUE, (TEXT("set SIQ alarm\r\n")));
		}
		else if(!bSIQ_Alarm && ALARM_PatAl_SPO2_SIQmin->getAlarmState()==AS_ACTIVE)
		{
			setStateOfAlarm(AL_PatAl_SPO2_SIQmin,AS_SIGNALED);
			DEBUGMSG(TRUE, (TEXT("signaled SIQ alarm\r\n")));
		}
	}
}

//rkuTICKCOUNT
bool CAlarmMgmtHandler::isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay)////used to check if old tick count plus delay is still lower than actual tickCount, (dwLastTickCount+DELAY<getTickCount64())
{
	//return Test_isSafeTickCountDelayExpired(oldTickCount, delay);
	bool bExpired=false;
	DWORD curTickCount=GetTickCount();

	if(curTickCount<oldTickCount)//overrun
	{
		DWORD diff=0xFFFFFFFF-oldTickCount;

		if(diff+delay<curTickCount)
		{
			bExpired=true;
		}
	}
	else
	{
		if(oldTickCount+delay<curTickCount)
		{
			bExpired=true;
		}
	}
	return bExpired;
}