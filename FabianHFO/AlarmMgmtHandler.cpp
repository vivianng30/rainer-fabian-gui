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
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_MAPmax=NULL;
CAlarmPtr CAlarmMgmtHandler::ALARM_PatAl_MAPmin=NULL;
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


/**********************************************************************************************//**
 * Initializes a new instance of the CAlarmMgmtHandler class
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Finalizes an instance of the CAlarmMgmtHandler class
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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
	if(ALARM_PatAl_MAPmax)
		delete ALARM_PatAl_MAPmax;
	ALARM_PatAl_MAPmax=NULL;
	if(ALARM_PatAl_MAPmin)
		delete ALARM_PatAl_MAPmin;
	ALARM_PatAl_MAPmin=NULL;
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


/**********************************************************************************************//**
 * Gets the instance
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	Null if it fails, else the instance.
 **************************************************************************************************/

CAlarmMgmtHandler* CAlarmMgmtHandler::getInstance()
{
	if(theAlarmHandler == 0)
	{
		theAlarmHandler = new CAlarmMgmtHandler;
	}
	return theAlarmHandler;
}


/**********************************************************************************************//**
 * Destroys the instance
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::destroyInstance()
{
	if(theAlarmHandler != NULL)
	{
		delete theAlarmHandler;
		theAlarmHandler = NULL;
	}
}

/**********************************************************************************************//**
 * Initializes this instance
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::init()
{
	initAlarmList();

	eSPO2alarmdelay iDelayTime=getModel()->getCONFIG()->getSPO2alarmDelay();
	
	setSPO2alarmDelay(iDelayTime);

	if(ALARMLIMITS==NULL)
		ALARMLIMITS=new CAlarmConditionHandler();
}

/**********************************************************************************************//**
 * Initializes the alarm list
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::initAlarmList()
{
	ALARM_SysFail_IF_SPI = new CAlarm(AL_SysFail_IF_SPI,AT_SYSFAIL,AP_1,AS_NONE,0);
	ALARM_SysFail_IF_DIO = new CAlarm(AL_SysFail_IF_DIO,AT_SYSFAIL,AP_1,AS_NONE,0);
	ALARM_SysFail_IF_COM = new CAlarm(AL_SysFail_IF_COM,AT_SYSFAIL,AP_1,AS_NONE,0);
	ALARM_SysFail_IF_I2C = new CAlarm(AL_SysFail_IF_I2C,AT_SYSFAIL,AP_1,AS_NONE,0);
	ALARM_SysFail_IF_PIF = new CAlarm(AL_SysFail_IF_PIF,AT_SYSFAIL,AP_1,AS_NONE,0);
	ALARM_SysFail_Fan = new CAlarm(AL_SysFail_Fan,AT_SYSFAIL,AP_1,AS_NONE,0);

	ALARM_Accu_15 = new CAlarm(AL_Accu_15,AT_POWER,AP_2,AS_NONE,0);
	ALARM_NONE = new CAlarm(AL_NONE,AT_NONE,AP_2,AS_NONE,0);
	ALARM_Accu_Empty = new CAlarm(AL_Accu_Empty,AT_POWER,AP_2,AS_NONE,0);
	ALARM_SysFail_ChecksumConPIC = new CAlarm(AL_SysFail_ChecksumConPIC,AT_SYSFAIL,AP_2,AS_NONE,0);
	ALARM_SysFail_ChecksumMonPIC = new CAlarm(AL_SysFail_ChecksumMonPIC,AT_SYSFAIL,AP_2,AS_NONE,0);
	ALARM_SysFail_RELAIS_DEFECT = new CAlarm(AL_SysFail_RELAIS_DEFECT,AT_SYSFAIL,AP_2,AS_NONE,0);
	ALARM_SysFail_P_IN_MIXER = new CAlarm(AL_SysFail_P_IN_MIXER,AT_SYSFAIL,AP_2,AS_NONE,0);
	ALARM_SysFail_VOLTAGE = new CAlarm(AL_SysFail_VOLTAGE,AT_SYSFAIL,AP_2,AS_NONE,0);
	ALARM_SysFail_OUTOFMEMORY = new CAlarm(AL_SysFail_OUTOFMEMORY,AT_SYSFAIL,AP_2,AS_NONE,0);
	ALARM_SysFail_MIXER_AVCAL = new CAlarm(AL_SysFail_MIXER_AVCAL,AT_SYSALARM,AP_2,AS_NONE,0);
	ALARM_Accu_Defect = new CAlarm(AL_Accu_Defect,AT_POWER,AP_2,AS_NONE,0);
	ALARM_SysAl_P_IN_O2 = new CAlarm(AL_SysAl_P_IN_O2,AT_SYSALARM,AP_2,AS_NONE,0);
	ALARM_SysAl_P_IN_AIR = new CAlarm(AL_SysAl_P_IN_AIR,AT_SYSALARM,AP_2,AS_NONE,0);
	ALARM_SysAl_TUBE_OCCLUSION = new CAlarm(AL_SysAl_TUBE_OCCLUSION,AT_SYSALARM,AP_2,AS_NONE,0);

	ALARM_DISCONNECTION = new CAlarm(AL_DISCONNECTION,AT_DISCON_TUBUS,AP_3,AS_NONE,0); //new alarm prio 08.2016
	ALARM_TUBUSBLOCKED = new CAlarm(AL_TUBUSBLOCKED,AT_DISCON_TUBUS,AP_3,AS_NONE,0);

	ALARM_Sens_FLOW_SENSOR_DEFECT = new CAlarm(AL_Sens_FLOW_SENSOR_DEFECT,AT_SENSOR,AP_4,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_FLOW_SENSOR_CLEANING = new CAlarm(AL_Sens_FLOW_SENSOR_CLEANING,AT_SENSOR,AP_4,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_FLOW_SENSOR_NOTCONNECTED = new CAlarm(AL_Sens_FLOW_SENSOR_NOTCONNECTED,AT_SENSOR,AP_4,AS_SIGNALED,0); //new alarm prio 08.2016
	ALARM_Sens_O2_SENSOR_DEFECT = new CAlarm(AL_Sens_O2_SENSOR_DEFECT,AT_SENSOR,AP_4,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_O2_SENSOR_USED = new CAlarm(AL_Sens_O2_SENSOR_USED,AT_SENSOR,AP_4,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_O2_VALUE_INCORRECT = new CAlarm(AL_Sens_O2_VALUE_INCORRECT,AT_SENSOR,AP_4,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_O2_NOT_CALIBRATED = new CAlarm(AL_Sens_O2_NOT_CALIBRATED,AT_SENSOR,AP_4,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_PRICO_FiO2outOfRange = new CAlarm(AL_Sens_PRICO_FiO2outOfRange,AT_SENSOR,AP_4,AS_NONE,0); //new alarm prio 08.2016
	
	ALARM_Sens_CO2_MODULE_NOTCONNECTED = new CAlarm(AL_Sens_CO2_MODULE_NOTCONNECTED,AT_CO2,AP_5,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_CO2_FILTERLINE_NOTCONNECTED = new CAlarm(AL_Sens_CO2_FILTERLINE_NOTCONNECTED,AT_CO2,AP_5,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_CO2_CHECKSAMPLINGLINE = new CAlarm(AL_Sens_CO2_CHECKSAMPLINGLINE,AT_CO2,AP_5,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_CO2_CHECKAIRWAYADAPTER = new CAlarm(AL_Sens_CO2_CHECKAIRWAYADAPTER,AT_CO2,AP_5,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_CO2_SENSORFAULTY = new CAlarm(AL_Sens_CO2_SENSORFAULTY,AT_CO2,AP_5,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_SPO2_MODULE_NOTCONNECTED = new CAlarm(AL_Sens_SPO2_MODULE_NOTCONNECTED,AT_SPO2,AP_5,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_SPO2_SENSORFAULTY = new CAlarm(AL_Sens_SPO2_SENSORFAULTY,AT_SPO2,AP_5,AS_NONE,0); //new alarm prio 08.2016
	ALARM_Sens_SPO2_CHECKSENSOR = new CAlarm(AL_Sens_SPO2_CHECKSENSOR,AT_SPO2,AP_5,AS_NONE,0); //new alarm prio 08.2016

	ALARM_PatAl_Apnoe = new CAlarm(AL_PatAl_Apnoe,AT_PATIENT,AP_6,AS_NONE,0); //new alarm prio 08.2016
	
	ALARM_PatAl_SPO2_SIQmin = new CAlarm(AL_PatAl_SPO2_SIQmin,AT_PATIENT,AP_7,AS_NONE,0);
	ALARM_PRICO_FiO2max = new CAlarm(AL_PRICO_FiO2max,AT_PATIENT,AP_7,AS_NONE,0);
	ALARM_PRICO_FiO2min = new CAlarm(AL_PRICO_FiO2min,AT_PATIENT,AP_7,AS_NONE,0);
	ALARM_PatAl_PIPmax = new CAlarm(AL_PatAl_PIPmax,AT_PATIENT,AP_7,AS_NONE,0);
	ALARM_PatAl_PEEPminHigh = new CAlarm(AL_PatAl_PEEPminHigh,AT_PATIENT,AP_7,AS_NONE,0);

	ALARM_PatAl_PIPmin = new CAlarm(AL_PatAl_PIPmin,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_MVmax = new CAlarm(AL_PatAl_MVmax,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_MVmin = new CAlarm(AL_PatAl_MVmin,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_PEEPminLow = new CAlarm(AL_PatAl_PEEPminLow,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_BPMmax = new CAlarm(AL_PatAl_BPMmax,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_Leakmax = new CAlarm(AL_PatAl_Leakmax,AT_PATIENT,AP_8,AS_NONE,0);
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
	ALARM_PatAl_MAPmax = new CAlarm(AL_PatAl_MAPmax,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_PatAl_MAPmin = new CAlarm(AL_PatAl_MAPmin,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_SysLimit_Pinsp_NotReached = new CAlarm(AL_SysLimit_Pinsp_NotReached,AT_SYSLIMIT,AP_9,AS_NONE,0);
	ALARM_SysLimit_Vlimitted = new CAlarm(AL_SysLimit_Vlimitted,AT_SYSLIMIT,AP_9,AS_NONE,0);
	ALARM_SysLimit_Vgaranty = new CAlarm(AL_SysLimit_Vgaranty,AT_PATIENT,AP_8,AS_NONE,0);
	ALARM_Accu_POWER = new CAlarm(AL_Accu_POWER,AT_POWER,AP_9,AS_NONE,0);
	ALARM_Accu_60 = new CAlarm(AL_Accu_60,AT_POWER,AP_8,AS_NONE,0);
	ALARM_Accu_30 = new CAlarm(AL_Accu_30,AT_POWER,AP_8,AS_NONE,0);

	

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
		m_pAlarmlist->appendAlarm(ALARM_PatAl_MAPmax);
		m_pAlarmlist->appendAlarm(ALARM_PatAl_MAPmin);
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

/**********************************************************************************************//**
 * Deinits this instance
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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


/**********************************************************************************************//**
 * Sets spo 2alarm delay
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	delay	The delay.
 **************************************************************************************************/

void CAlarmMgmtHandler::setSPO2alarmDelay(eSPO2alarmdelay delay)
{
	UINT iDelayTime=0;
	switch(delay)
	{
	case SPO2_ADELAY_0:
		{
			iDelayTime=0;
		}
		break;
	case SPO2_ADELAY_5:
		{
			iDelayTime=5;
		}
		break;
	case SPO2_ADELAY_10:
		{
			iDelayTime=10;
		}
		break;
	case SPO2_ADELAY_15:
		{
			iDelayTime=15;
		}
		break;
	default:
		{
			iDelayTime=10;
		}
		break;
	}
	ALARM_PatAl_SPO2max->setAlarmDelayTimeSec(iDelayTime);
	ALARM_PatAl_SPO2min->setAlarmDelayTimeSec(iDelayTime);
	ALARM_PatAl_PulseRatemax->setAlarmDelayTimeSec(iDelayTime);
	ALARM_PatAl_PulseRatemin->setAlarmDelayTimeSec(iDelayTime);
	ALARM_PatAl_SPO2_PImin->setAlarmDelayTimeSec(iDelayTime);
	ALARM_PatAl_SPO2_SIQmin->setAlarmDelayTimeSec(iDelayTime);
}


/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CAlarmMgmtHandler::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


/**********************************************************************************************//**
 * Gets alarm log
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	Null if it fails, else the alarm log.
 **************************************************************************************************/

CTlsLoglist * CAlarmMgmtHandler::getAlarmLog()
{
	if(m_AlarmLoglist)
	{
		return m_AlarmLoglist;
	}
	else
	{
		theApp.ReportException(_T("CAlarmingHandler::getAlarmLog()"));

		return 0;
	}
}


/**********************************************************************************************//**
 * Sets log text
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	sz	The size.
 **************************************************************************************************/

void CAlarmMgmtHandler::setLogTxt(CStringW sz)
{
	if(m_AlarmLogfile)
		m_AlarmLogfile->WriteLine(sz);
}


/**********************************************************************************************//**
 * Sets log alarm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	enAlarm   	The en alarm.
 * \param	szAlarmTxt	The alarm text.
 **************************************************************************************************/

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
		szAlarm.Format(_T("%s%s|%d"),getLogTxtAlarm(enAlarm),szAlarmTxt,(int)getModel()->getVMODEHANDLER()->getCurrentActiveMode());
		/*szAlarm=getLogTxtAlarm(enAlarm);
		szAlarm+= _T(" ")+ szAlarmTxt;*/
		m_AlarmLogfile->WriteLine(szAlarm);
	}
}


/**********************************************************************************************//**
 * Check log alarm date
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::checkLogAlarmDate()
{
	if(m_AlarmLogfile)
	{
		m_AlarmLogfile->CheckDate();
	}
}


/**********************************************************************************************//**
 * Gets log text alarm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	enAlarm	The en alarm.
 *
 * \return	The log text alarm.
 **************************************************************************************************/

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
			return _T("FAN_DEFECT");
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
	case AL_PatAl_MAPmax:
		{
			return _T("MAP_HI");
		}
		break;
	case AL_PatAl_MAPmin:
		{
			return _T("MAP_LO");
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
			if(getModel()->getCONFIG()->getCO2module()==CO2MODULE_CAPNOSTAT)
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

/**********************************************************************************************//**
 * Loads the configuration
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::loadConfig()
{
	init();
}


/**********************************************************************************************//**
 * Vent mode changed
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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


/**********************************************************************************************//**
 * Gets safe active vent mode
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The safe active vent mode.
 **************************************************************************************************/

eVentMode CAlarmMgmtHandler::getSafeActiveVentMode()
{
	eVentMode mode=VM_NONE;
	EnterCriticalSection(&csActiveVentMode);
	mode=m_eActiveVentMode;
	LeaveCriticalSection(&csActiveVentMode);
	return mode;
}


/**********************************************************************************************//**
 * Gets active alarm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The active alarm.
 **************************************************************************************************/

eAlarm CAlarmMgmtHandler::getActiveAlarm()
{
	EnterCriticalSection(&csActiveAlarm);
	eAlarm alarm=m_eActivealarm;
	LeaveCriticalSection(&csActiveAlarm);
	return alarm;
}

/**********************************************************************************************//**
 * Sets active alarm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarm	The alarm.
 **************************************************************************************************/

void CAlarmMgmtHandler::setActiveAlarm(eAlarm alarm)
{
	EnterCriticalSection(&csActiveAlarm);
	m_eActivealarm=alarm;
	LeaveCriticalSection(&csActiveAlarm);
}


/**********************************************************************************************//**
 * Gets alarm silent state
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alarm silent state.
 **************************************************************************************************/

eVentSilentState CAlarmMgmtHandler::getAlarmSilentState()
{
	EnterCriticalSection(&csAlarmSilentState);
	eVentSilentState state=m_eAlarmSilentState;
	LeaveCriticalSection(&csAlarmSilentState);
	return state;
}


/**********************************************************************************************//**
 * Sets alarm silent state
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlarmSilentState(eVentSilentState state)
{
	EnterCriticalSection(&csAlarmSilentState);
	m_eAlarmSilentState=state;
	LeaveCriticalSection(&csAlarmSilentState);
}


/**********************************************************************************************//**
 * Gets priority active alarm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The priority active alarm.
 **************************************************************************************************/

eAlarmPrio CAlarmMgmtHandler::getPrioActiveAlarm()
{
	EnterCriticalSection(&csPrioActiveAlarm);
	eAlarmPrio prio=m_ePrioActiveAlarm;
	LeaveCriticalSection(&csPrioActiveAlarm);
	return prio;
}


/**********************************************************************************************//**
 * Sets priority active alarm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	prio	The priority.
 **************************************************************************************************/

void CAlarmMgmtHandler::setPrioActiveAlarm(eAlarmPrio prio)
{
	EnterCriticalSection(&csPrioActiveAlarm);
	m_ePrioActiveAlarm=prio;
	LeaveCriticalSection(&csPrioActiveAlarm);
}

/**********************************************************************************************//**
 * Sets startup silent
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Gets startup silent
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::getStartupSilent()
{
	EnterCriticalSection(&csAlarmSilentState);
	bool bState=m_bStartupSilent;
	LeaveCriticalSection(&csAlarmSilentState);
	return bState;
}


/**********************************************************************************************//**
 * Sets alarmsilent active
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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


/**********************************************************************************************//**
 * Sets system silent active
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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


/**********************************************************************************************//**
 * Sets system silent
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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


/**********************************************************************************************//**
 * Sets alarm silent
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	bDelSysSilent	True to delete the system silent.
 * \param	bDelAllAlarms	True to delete the alarms.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlarmSilent(bool bDelSysSilent,bool bDelAllAlarms)
{
	DEBUGMSG(TRUE, (TEXT("setAlarmSilent\r\n")));
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


/**********************************************************************************************//**
 * Sets automatic silent
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	bDelSysSilent	True to delete the system silent.
 * \param	bDelSilent   	True to delete the silent.
 * \param	bDelManSilent	True to delete the manager silent.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets manual silent
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	bRefreshTime	True to refresh time.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets calibration silent
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::setCalibrationSilent()
{
	DEBUGMSG(TRUE, (TEXT("setSystemSilent\r\n")));

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

/**********************************************************************************************//**
 * Query if this instance is manual silent state
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if manual silent state, false if not.
 **************************************************************************************************/

bool CAlarmMgmtHandler::isManualSilentState()
{
	if(getAlarmSilentState()==ASTATE_MANSILENT)
	{
		return true;
	}
	else
		return false;
}

/**********************************************************************************************//**
 * Gets alarm text
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarm	The alarm.
 *
 * \return	The alarm text.
 **************************************************************************************************/

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
			if(getModel()->getVMODEHANDLER()->activeModeIsCPAP() || getModel()->getVMODEHANDLER()->activeModeIsNCPAP())
				sz=getModel()->GetLanguageString(IDS_ALARM_PMAX_CPAP);
			else
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
	case AL_PatAl_MAPmax:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_MAP_HI);
		}
		break;
	case AL_PatAl_MAPmin:
		{
			sz=getModel()->GetLanguageString(IDS_ALARM_MAP_LO);
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
			if(getModel()->getCONFIG()->getCO2module()==CO2MODULE_CAPNOSTAT)
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

/**********************************************************************************************//**
 * Gets alarm priority
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarm	The alarm.
 *
 * \return	The alarm priority.
 **************************************************************************************************/

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
 	case AL_SysFail_P_IN_MIXER:					
	case AL_SysFail_VOLTAGE:					
	case AL_SysFail_OUTOFMEMORY:					
	case AL_Accu_15:
	case AL_Accu_Empty:
	case AL_Accu_Defect:
	case AL_SysAl_P_IN_O2:					
	case AL_SysAl_P_IN_AIR:					
	case AL_SysAl_TUBE_OCCLUSION:			
	case AL_SysFail_ChecksumConPIC:
	case AL_SysFail_ChecksumMonPIC:
		{
			return AP_2;
		}
		break;
	/*case AL_Nebulizer_Disconnection:
	case AL_Nebulizer_SysError:*/
	case AL_DISCONNECTION:
	case AL_TUBUSBLOCKED:
		{
			return AP_3;
		}
		break;
	//case AL_TUBUSBLOCKED:
	case AL_Sens_FLOW_SENSOR_DEFECT:			
	case AL_Sens_FLOW_SENSOR_CLEANING:		
	case AL_Sens_FLOW_SENSOR_NOTCONNECTED:	
	case AL_Sens_O2_SENSOR_DEFECT:			
	case AL_Sens_O2_SENSOR_USED:				
	case AL_Sens_O2_VALUE_INCORRECT:	
	case AL_Sens_O2_NOT_CALIBRATED:
	case AL_Sens_PRICO_FiO2outOfRange:
		{
			return AP_4;
		}
		break;
	//case AL_SysFail_P_PROXIMAL:
	// new alarm prio 08.2016
	/*case AL_Sens_FLOW_SENSOR_DEFECT:			
	case AL_Sens_FLOW_SENSOR_CLEANING:		
	case AL_Sens_FLOW_SENSOR_NOTCONNECTED:	
	case AL_Sens_O2_SENSOR_DEFECT:			
	case AL_Sens_O2_SENSOR_USED:				
	case AL_Sens_O2_VALUE_INCORRECT:	
	case AL_Sens_O2_NOT_CALIBRATED:
	case AL_Sens_PRICO_FiO2outOfRange:*/
	case AL_Sens_CO2_MODULE_NOTCONNECTED:
	case AL_Sens_CO2_FILTERLINE_NOTCONNECTED:
	case AL_Sens_CO2_CHECKSAMPLINGLINE:
	case AL_Sens_CO2_CHECKAIRWAYADAPTER:
	case AL_Sens_CO2_SENSORFAULTY:
	case AL_Sens_SPO2_MODULE_NOTCONNECTED:
	case AL_Sens_SPO2_SENSORFAULTY:
	case AL_Sens_SPO2_CHECKSENSOR:
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
	/*case AL_Sens_CO2_MODULE_NOTCONNECTED:
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
		break;*/
	case AL_PatAl_SPO2_SIQmin:
	case AL_PRICO_FiO2max:
	case AL_PRICO_FiO2min:
	case AL_PatAl_PIPmax:	
	case AL_PatAl_PEEPminHigh:
	case AL_PatAl_MAPmax:						
	case AL_PatAl_MAPmin:
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

/**********************************************************************************************//**
 * Gets alarm type
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarm	The alarm.
 *
 * \return	The alarm type.
 **************************************************************************************************/

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
	case AL_PatAl_MAPmax:						
	case AL_PatAl_MAPmin:
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

/**********************************************************************************************//**
 * Gets signaled alarm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The signaled alarm.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets an alarm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarm   	The alarm.
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::setAlarm(eAlarm alarm, CStringW alarmTxt)
{
	//DEBUGMSG(TRUE, (TEXT("setAlarm\r\n")));

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
	case AL_PatAl_MAPmax:
		{
			bRes=SetAlarm_MAPmax(alarmTxt);
		}
		break;
	case AL_PatAl_MAPmin:
		{
			bRes=SetAlarm_MAPmin(alarmTxt);
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

/**********************************************************************************************//**
 * Query if 'prioAlarm' is active alarm higher or same priority
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	prioAlarm	The prio alarm.
 *
 * \return	True if active alarm higher or same priority, false if not.
 **************************************************************************************************/

bool CAlarmMgmtHandler::isActiveAlarmHigherOrSamePriority(eAlarmPrio prioAlarm)
{
	bool bRes=false;
	if(AL_NONE!=getActiveAlarm())//active alarm
	{
		// alarm with higher or same priority active???
		if(	getPrioActiveAlarm() <= prioAlarm )	
		{
			bRes=true;
		}
	}
	return bRes;
}

/**********************************************************************************************//**
 * Query if 'prioAlarm' is active alarm higher priority
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	prioAlarm	The prio alarm.
 *
 * \return	True if active alarm higher priority, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm checksum Controller PIC
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm checksum Controller PIC, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_SysFail_ChecksumConPIC)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm checksum Controller PIC
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm checksum monitor PIC
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm checksum monitor PIC, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_SysFail_ChecksumMonPIC)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm checksum monitor PIC
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm relais defect
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm relais defect, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_SysFail_RELAIS_DEFECT)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm relais defect
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm mixer avcal
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm mixer avcal, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_SysFail_MIXER_AVCAL)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm mixer avcal
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm pressure in mixer
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm pressure in mixer, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_SysFail_P_IN_MIXER)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm pressure in mixer
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm voltage
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm voltage, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets alarm voltage
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm interface spi
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm interface spi, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_SysFail_IF_SPI)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm interface spi
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm interface dio
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm interface dio, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_SysFail_IF_DIO)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm interface dio
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm interface com
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm interface com, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_SysFail_IF_COM)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm interface com
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm interface i2c
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm interface i2c, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_SysFail_IF_I2C)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm interface i2c
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm interface pif
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm interface pif, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_SysFail_IF_PIF)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm interface pif
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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
//	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_SysFail_IF_ACULINK)))
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

/**********************************************************************************************//**
 * Determine if we can set alarm fan
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm fan, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_SysFail_Fan)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm fan
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm outofmemory
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm outofmemory, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets alarm outofmemory
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm Pin o2
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm Pin o2, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets alarm Pin o2
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm Pin air
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm Pin air, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets alarm Pin air
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm tube occlusion
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm tube occlusion, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_SysAl_TUBE_OCCLUSION)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm tube occlusion
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm disconnection
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm disconnection, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_DISCONNECTION)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm disconnection
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm tubusblocked
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm tubusblocked, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_TUBUSBLOCKED)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm tubusblocked
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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



/**********************************************************************************************//**
 * Determine if we can set alarm flow sensor defect
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm flow sensor defect, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_Sens_FLOW_SENSOR_DEFECT)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm flow sensor defect
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm flow sensor cleaning
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm flow sensor cleaning, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_Sens_FLOW_SENSOR_CLEANING)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm flow sensor cleaning
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm flow sensor notconnected
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm flow sensor notconnected, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_Sens_FLOW_SENSOR_NOTCONNECTED)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm flow sensor notconnected
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm o2 sensor defect
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm o2 sensor defect, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_Sens_O2_SENSOR_DEFECT)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm o2 sensor defect
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm o2 sensor used
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm o2 sensor used, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_Sens_O2_SENSOR_USED)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm o2 sensor used
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm o2 value incorrect
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm o2 value incorrect, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_Sens_O2_VALUE_INCORRECT)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm o2 value incorrect
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm o2 not calibrated
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm o2 not calibrated, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_Sens_O2_NOT_CALIBRATED)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm o2 not calibrated
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm prico fio2out of range
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm prico fio2out of range, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_Sens_PRICO_FiO2outOfRange)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm prico fio2out of range
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm spo2 sensorfaulty
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm spo2 sensorfaulty, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_Sens_SPO2_SENSORFAULTY)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm spo2 sensorfaulty
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm spo2 checksensor
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm spo2 checksensor, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_Sens_SPO2_CHECKSENSOR)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm spo2 checksensor
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm spo2 module notconnected
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm spo2 module notconnected, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_Sens_SPO2_MODULE_NOTCONNECTED)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm spo2 module notconnected
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm co2 module notconnected
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm co2 module notconnected, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_Sens_CO2_MODULE_NOTCONNECTED)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm co2 module notconnected
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm co2 filterline notconnected
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm co2 filterline notconnected, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_Sens_CO2_FILTERLINE_NOTCONNECTED)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm co2 filterline notconnected
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm co2 checksamplingline
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm co2 checksamplingline, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_Sens_CO2_CHECKSAMPLINGLINE)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm co2 checksamplingline
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm co2 checkairwayadapter
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm co2 checkairwayadapter, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_Sens_CO2_CHECKAIRWAYADAPTER)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm co2 checkairwayadapter
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm co2 sensorfaulty
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm co2 sensorfaulty, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_Sens_CO2_SENSORFAULTY)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm co2 sensorfaulty
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm MVmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm MVmax, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_MVmax)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm MVmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_MVmax(CStringW alarmTxt)
{
	if(CanSetAlarm_MVmax()==false)
		return false;

	eAlarm alarm=AL_PatAl_MVmax;

	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm MVmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm MVmin, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_MVmin)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm MVmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_MVmin(CStringW alarmTxt)
{
	if(CanSetAlarm_MVmin()==false)
		return false;

	eAlarm alarm=AL_PatAl_MVmin;

	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm prico SIQvalue
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm prico SIQvalue, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_SPO2_SIQmin)))
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}
	return true;
}

/**********************************************************************************************//**
 * Sets alarm prico SIQvalue
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm prico fio2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm prico fio2max, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PRICO_FiO2max)))
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}
	return true;
}

/**********************************************************************************************//**
 * Sets alarm prico fio2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm prico fio2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm prico fio2min, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PRICO_FiO2min)))
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}
	return true;
}

/**********************************************************************************************//**
 * Sets alarm prico fio2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets pi pmax alarm text
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 **************************************************************************************************/

void CAlarmMgmtHandler::setPIPmaxAlarmText(CStringW alarmTxt)
{
	m_szPIPmaxAlarmText=alarmTxt;
}

/**********************************************************************************************//**
 * Determine if we can set alarm PIPmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm PIPmax, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_PIPmax)))
	{
		m_szPIPmaxAlarmText=_T("");
		return false;
	}
	return true;
}

/**********************************************************************************************//**
 * Sets alarm PIPmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets PIPmin alarm text
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 **************************************************************************************************/

void CAlarmMgmtHandler::setPIPminAlarmText(CStringW alarmTxt)
{
	m_szPIPminAlarmText=alarmTxt;
}

/**********************************************************************************************//**
 * Determine if we can set alarm PIPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm PIPmin, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_PIPmin)))
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
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if it succeeds, false if it fails.
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

/**********************************************************************************************//**
 * Sets PEEPmin low alarm text
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 **************************************************************************************************/

void CAlarmMgmtHandler::setPEEPminLowAlarmText(CStringW alarmTxt)
{
	m_szPEEPminLowAlarmText=alarmTxt;
}

/**********************************************************************************************//**
 * Sets PEEPmin low alarm value
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setPEEPminLowAlarmValue(int value)
{
	m_iPEEPminLowAlarmValue=value;
}

/**********************************************************************************************//**
 * Determine if we can set alarm PEEPmin low
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm PEEPmin low, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_PEEPminLow)))
	{
		m_szPEEPminLowAlarmText=_T("");
		m_iPEEPminLowAlarmValue=-999;
		return false;
	}
	return true;
}

/**********************************************************************************************//**
 * Sets alarm PEEPmin low
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets PEEPmin high alarm text
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 **************************************************************************************************/

void CAlarmMgmtHandler::setPEEPminHighAlarmText(CStringW alarmTxt)
{
	m_szPEEPminHighAlarmText=alarmTxt;
}

/**********************************************************************************************//**
 * Sets PEEPmin high alarm value
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setPEEPminHighAlarmValue(int value)
{
	m_iPEEPminHighAlarmValue=value;
}

/**********************************************************************************************//**
 * Determine if we can set alarm PEEPmin high
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm PEEPmin high, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_PEEPminHigh)))
	{
		m_szPEEPminHighAlarmText=_T("");
		m_iPEEPminHighAlarmValue=-999;
		return false;
	}
	return true;
}

/**********************************************************************************************//**
 * Sets alarm PEEPmin high
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm bpmmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm bpmmax, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_BPMmax)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm bpmmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_BPMmax(CStringW alarmTxt)
{
	if(CanSetAlarm_BPMmax()==false)
		return false;

	eAlarm alarm=AL_PatAl_BPMmax;

	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm leakmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm leakmax, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_Leakmax)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm leakmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_Leakmax(CStringW alarmTxt)
{
	if(CanSetAlarm_Leakmax()==false)
		return false;

	eAlarm alarm=AL_PatAl_Leakmax;

	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm apnoe
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm apnoe, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_Apnoe)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm apnoe
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm MAPmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm MAPmax, false if not.
 **************************************************************************************************/

bool CAlarmMgmtHandler::CanSetAlarm_MAPmax()
{
	if(ALARM_PatAl_MAPmax->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_MAPmax)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm MAPmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_MAPmax(CStringW alarmTxt)
{
	if(CanSetAlarm_MAPmax()==false)
		return false;

	eAlarm alarm=AL_PatAl_MAPmax;

	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm MAPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm MAPmin, false if not.
 **************************************************************************************************/

bool CAlarmMgmtHandler::CanSetAlarm_MAPmin()
{
	if(ALARM_PatAl_MAPmin->getAlarmState()==AS_ACTIVE)
		return false;

	eVentSilentState silentState=getAlarmSilentState();

	//higher alarms silent or service mode????
	if(		silentState==ASTATE_SYSTEM_SILENT
		||	silentState==ASTATE_SILENT
		||	getModel()->getCONFIG()->GetCurMode()==VM_SERVICE
		||	getModel()->getDATAHANDLER()->IsFlowSensorCalibrating())
		return false;

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_MAPmin)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm MAPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_MAPmin(CStringW alarmTxt)
{
	if(CanSetAlarm_MAPmin()==false)
		return false;

	eAlarm alarm=AL_PatAl_MAPmin;

	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm dco2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm dco2max, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_DCO2max)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm dco2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_DCO2max(CStringW alarmTxt)
{
	if(CanSetAlarm_DCO2max()==false)
		return false;

	eAlarm alarm=AL_PatAl_DCO2max;

	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm dco2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm dco2min, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_DCO2min)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm dco2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_DCO2min(CStringW alarmTxt)
{
	if(CanSetAlarm_DCO2min()==false)
		return false;

	eAlarm alarm=AL_PatAl_DCO2min;

	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm etco2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm etco2max, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_ETCO2max)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm etco2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_ETCO2max(CStringW alarmTxt)
{
	if(CanSetAlarm_ETCO2max()==false)
		return false;

	eAlarm alarm=AL_PatAl_ETCO2max;

	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm etco2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm etco2min, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_ETCO2min)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm etco2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_ETCO2min(CStringW alarmTxt)
{
	if(CanSetAlarm_ETCO2min()==false)
		return false;

	eAlarm alarm=AL_PatAl_ETCO2min;

	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm fico2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm fico2max, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_FICO2max)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm fico2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_FICO2max(CStringW alarmTxt)
{
	if(CanSetAlarm_FICO2max()==false)
		return false;

	eAlarm alarm=AL_PatAl_FICO2max;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm fico2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm fico2min, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_FICO2min)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm fico2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_FICO2min(CStringW alarmTxt)
{
	if(CanSetAlarm_FICO2min()==false)
		return false;

	eAlarm alarm=AL_PatAl_FICO2min;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm spo2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm spo2max, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_SPO2max)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm spo2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_SPO2max(CStringW alarmTxt)
{
	if(CanSetAlarm_SPO2max()==false)
		return false;

	eAlarm alarm=AL_PatAl_SPO2max;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm spo2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm spo2min, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_SPO2min)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm spo2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_SPO2min(CStringW alarmTxt)
{
	if(CanSetAlarm_SPO2min()==false)
		return false;

	eAlarm alarm=AL_PatAl_SPO2min;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm pulseratemax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm pulseratemax, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_PulseRatemax)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm pulseratemax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_PulseRatemax(CStringW alarmTxt)
{
	if(CanSetAlarm_PulseRatemax()==false)
		return false;

	eAlarm alarm=AL_PatAl_PulseRatemax;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm pulseratemin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm pulseratemin, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_PulseRatemin)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm pulseratemin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_PulseRatemin(CStringW alarmTxt)
{
	if(CanSetAlarm_PulseRatemin()==false)
		return false;

	eAlarm alarm=AL_PatAl_PulseRatemin;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm spo2 PImin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm spo2 PImin, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_PatAl_SPO2_PImin)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm spo2 PImin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_SPO2_PImin(CStringW alarmTxt)
{
	if(CanSetAlarm_SPO2_PImin()==false)
		return false;

	eAlarm alarm=AL_PatAl_SPO2_PImin;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm Pinsp not reached
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm Pinsp not reached, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_SysLimit_Pinsp_NotReached)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm Pinsp not reached
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_Pinsp_NotReached(CStringW alarmTxt)
{
	if(CanSetAlarm_Pinsp_NotReached()==false)
		return false;

	eAlarm alarm=AL_SysLimit_Pinsp_NotReached;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm vlimitted
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm vlimitted, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_SysLimit_Vlimitted)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm vlimitted
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_Vlimitted(CStringW alarmTxt)
{
	if(CanSetAlarm_Vlimitted()==false)
		return false;

	eAlarm alarm=AL_SysLimit_Vlimitted;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm vgaranty
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm vgaranty, false if not.
 **************************************************************************************************/

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

	if(isActiveAlarmHigherOrSamePriority(getAlarmPrio(AL_SysLimit_Vgaranty)))
		return false;
	return true;
}

/**********************************************************************************************//**
 * Sets alarm vgaranty
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CAlarmMgmtHandler::SetAlarm_Vgaranty(CStringW alarmTxt)
{
	if(CanSetAlarm_Vgaranty()==false)
		return false;

	eAlarm alarm=AL_SysLimit_Vgaranty;
	setActiveAlarm(alarm, alarmTxt);

	return true;
}

/**********************************************************************************************//**
 * Determine if we can set alarm accu power
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm accu power, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets alarm accu power
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm accu60
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm accu60, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets alarm accu60
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm accu30
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm accu30, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets alarm accu30
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm accu15
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm accu15, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets alarm accu15
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm accu empty
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm accu empty, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets alarm accu empty
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Determine if we can set alarm accu defect
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if we can set alarm accu defect, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets alarm accu defect
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarmTxt	The alarm text.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Queries if the pric orelevant alarm is active
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if the pric orelevant alarm is active, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Queries if the fo trelevant alarm is active
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if the fo trelevant alarm is active, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets active alarm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	actAlarm	The act alarm.
 * \param	alarmTxt	The alarm text.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Gets the next active alarm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The next active alarm.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Updates the active alarm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Updates the active alarm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarm	The alarm.
 * \param	state	The state.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Check prico alarms
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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
		else if(isPRICOAutoTurnedOn()
			&&	(actAlarm == AL_Sens_SPO2_MODULE_NOTCONNECTED 
			||	actAlarm == AL_Sens_SPO2_SENSORFAULTY
			||	actAlarm == AL_Sens_O2_SENSOR_DEFECT
			||	actAlarm == AL_Sens_O2_SENSOR_USED
			||	actAlarm == AL_Sens_O2_NOT_CALIBRATED
			||	actAlarm == AL_SysAl_P_IN_O2
			||	actAlarm == AL_SysAl_P_IN_AIR
			||	actAlarm == AL_SysFail_P_IN_MIXER))
		{
			resetPRICOAutoTurneOnFlag();
		}


		if(isPRICOAutoTurnedOff())
		{
			checkAutoEnablePRICO();//rku AUTOPRICO
		}
	}
}

/**********************************************************************************************//**
 * Check automatic enable prico
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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
			if(isPRICOAutoTurnedOn())
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
			if(isPRICOAutoTurnedOn())
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

/**********************************************************************************************//**
 * Sets state of alarm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarm  	The alarm.
 * \param	eState 	The state.
 * \param	bUpdate	True to update.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Deletes the alarm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	alarm  	The alarm.
 * \param	bUpdate	True to update.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets lower prio active alarms signaled
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	prio	The prio.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Query if this instance is alarm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if alarm, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Query if this instance is active alarm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if active alarm, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Query if this instance is signaled alarm
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if signaled alarm, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Query if 'type' is alarm type
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	type	The type.
 *
 * \return	True if alarm type, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Query if 'type' is active alarm type
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	type	The type.
 *
 * \return	True if active alarm type, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Query if 'type' is signaled alarm type
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	type	The type.
 *
 * \return	True if signaled alarm type, false if not.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Deletes the signaled alarms
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Deletes the signaled alarmlimit states
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Deletes the akku alarms
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Deletes the lower priority alarms described by priority
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	prio	The prio.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Deletes all alarms described by bDeleteAkkuAlarms
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	bDeleteAkkuAlarms	True to delete the akku alarms.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Deletes the alarm type
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	type   	The type.
 * \param	bUpdate	True to update.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Updates the alarm sound
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Updates the alarm states silent state dependend
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::updateAlarmStatesSilentStateDependend()
{
	updateActiveAlarm();
	updateAlarmSound();
	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
}

/**********************************************************************************************//**
 * Sets alarm state pdms
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	eAlarmToSet	Set the alarm to belongs to.
 * \param	state	   	The state.
 **************************************************************************************************/

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
		case AL_Sens_PRICO_FiO2outOfRange:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Sens_PRICO_FiO2outOfRange,(int)state);
			}
			break;
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
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_SPO2_SIQmin,(int)state);
			}
			break;
		case AL_PRICO_FiO2max:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PRICO_FiO2max,(int)state);
			}
			break;
		case AL_PRICO_FiO2min:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PRICO_FiO2min,(int)state);
			}
			break;
		case AL_PatAl_PIPmax:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_PIPmax,(int)state);
			}
			break;
		case AL_PatAl_PIPmin:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_PIPmin,(int)state);
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
		case AL_PatAl_MAPmax:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_PMEANmax,(int)state);
			}
			break;
		case AL_PatAl_MAPmin:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_PMEANmin,(int)state);
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
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_FICO2max,(int)state);
			}
			break;
		case AL_PatAl_FICO2min:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_FICO2min,(int)state);
			}
			break;
		case AL_PatAl_SPO2max:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_SPO2max,(int)state);
			}
			break;				
		case AL_PatAl_SPO2min:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_SPO2min,(int)state);
			}
			break;			
		case AL_PatAl_PulseRatemax:							
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_PulseRatemax,(int)state);
			}
			break;								
		case AL_PatAl_PulseRatemin:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_PulseRatemin,(int)state);
			}
			break;			
		case AL_PatAl_SPO2_PImin:						
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_PatAl_SPO2_PImin,(int)state);
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
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Sens_SPO2_MODULE_NOTCONNECTED,(int)state);
			}
			break;
		case AL_Sens_SPO2_SENSORFAULTY:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Sens_SPO2_SENSORFAULTY,(int)state);
			}
			break;
		case AL_Sens_SPO2_CHECKSENSOR:
			{
				getModel()->getAcuLink()->setAlarmData(ALINK_ALARM_Sens_SPO2_CHECKSENSOR,(int)state);
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

/**********************************************************************************************//**
 * Resets the vg vl automatic turned off
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Resets the vg vl automatic turned off flow sensor
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void  CAlarmMgmtHandler::resetVgVlAutoTurnedOff_FlowSensor()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVgarantAutoTurnedOff_FlowSensor=false;
	m_bVlimitAutoTurnedOff_FlowSensor=false;
	LeaveCriticalSection(&csVGVLautoOffState);
}

/**********************************************************************************************//**
 * Resets the vg vl automatic turned off tube
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void  CAlarmMgmtHandler::resetVgVlAutoTurnedOff_Tube()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVgarantAutoTurnedOff_Tube=false;
	m_bVlimitAutoTurnedOff_Tube=false;
	LeaveCriticalSection(&csVGVLautoOffState);
}

/**********************************************************************************************//**
 * Sets vgarant automatic turned off fot
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::setVgarantAutoTurnedOff_FOT()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVgarantAutoTurnedOff_FOT=true;
	LeaveCriticalSection(&csVGVLautoOffState);

}

/**********************************************************************************************//**
 * Resets the vg automatic turned off fot
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void  CAlarmMgmtHandler::resetVgAutoTurnedOff_FOT()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVgarantAutoTurnedOff_FOT=false;
	LeaveCriticalSection(&csVGVLautoOffState);
}

/**********************************************************************************************//**
 * Query if this instance is vgarant automatic turned off fot
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if vgarant automatic turned off fot, false if not.
 **************************************************************************************************/

bool CAlarmMgmtHandler::isVgarantAutoTurnedOff_FOT()
{
	EnterCriticalSection(&csVGVLautoOffState);
	bool bState=m_bVgarantAutoTurnedOff_FOT;
	LeaveCriticalSection(&csVGVLautoOffState);
	return bState;
}

/**********************************************************************************************//**
 * Sets vgarant automatic turned off flow sensor
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::setVgarantAutoTurnedOff_FlowSensor()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVgarantAutoTurnedOff_FlowSensor=true;
	LeaveCriticalSection(&csVGVLautoOffState);
	
}

/**********************************************************************************************//**
 * Resets the vgarant automatic turned off flow sensor
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::resetVgarantAutoTurnedOff_FlowSensor()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVgarantAutoTurnedOff_FlowSensor=false;
	LeaveCriticalSection(&csVGVLautoOffState);
}

/**********************************************************************************************//**
 * Query if this instance is vgarant automatic turned off flow sensor
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if vgarant automatic turned off flow sensor, false if not.
 **************************************************************************************************/

bool CAlarmMgmtHandler::isVgarantAutoTurnedOff_FlowSensor()
{
	EnterCriticalSection(&csVGVLautoOffState);
	bool bState=m_bVgarantAutoTurnedOff_FlowSensor;
	LeaveCriticalSection(&csVGVLautoOffState);
	return bState;
}

/**********************************************************************************************//**
 * Sets vlimit automatic turned off flow sensor
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::setVlimitAutoTurnedOff_FlowSensor()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVlimitAutoTurnedOff_FlowSensor=true;
	LeaveCriticalSection(&csVGVLautoOffState);
}

/**********************************************************************************************//**
 * Resets the vlimit automatic turned off flow sensor
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::resetVlimitAutoTurnedOff_FlowSensor()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVlimitAutoTurnedOff_FlowSensor=false;
	LeaveCriticalSection(&csVGVLautoOffState);
}

/**********************************************************************************************//**
 * Query if this instance is vlimit automatic turned off flow sensor
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if vlimit automatic turned off flow sensor, false if not.
 **************************************************************************************************/

bool CAlarmMgmtHandler::isVlimitAutoTurnedOff_FlowSensor()
{
	EnterCriticalSection(&csVGVLautoOffState);
	bool bState=m_bVlimitAutoTurnedOff_FlowSensor;
	LeaveCriticalSection(&csVGVLautoOffState);
	return bState;
}

/**********************************************************************************************//**
 * Sets vgarant automatic turned off tube
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::setVgarantAutoTurnedOff_Tube()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVgarantAutoTurnedOff_Tube=true;
	LeaveCriticalSection(&csVGVLautoOffState);
}

/**********************************************************************************************//**
 * Resets the vgarant automatic turned off tube
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::resetVgarantAutoTurnedOff_Tube()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVgarantAutoTurnedOff_Tube=false;
	LeaveCriticalSection(&csVGVLautoOffState);
}

/**********************************************************************************************//**
 * Query if this instance is vgarant automatic turned off tube
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if vgarant automatic turned off tube, false if not.
 **************************************************************************************************/

bool CAlarmMgmtHandler::isVgarantAutoTurnedOff_Tube()
{
	EnterCriticalSection(&csVGVLautoOffState);
	bool bState=m_bVgarantAutoTurnedOff_Tube;
	LeaveCriticalSection(&csVGVLautoOffState);
	return bState;
}

/**********************************************************************************************//**
 * Sets vlimit automatic turned off tube
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::setVlimitAutoTurnedOff_Tube()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVlimitAutoTurnedOff_Tube=true;
	LeaveCriticalSection(&csVGVLautoOffState);
}

/**********************************************************************************************//**
 * Resets the vlimit automatic turned off tube
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::resetVlimitAutoTurnedOff_Tube()
{
	EnterCriticalSection(&csVGVLautoOffState);
	m_bVlimitAutoTurnedOff_Tube=false;
	LeaveCriticalSection(&csVGVLautoOffState);
}

/**********************************************************************************************//**
 * Query if this instance is vlimit automatic turned off tube
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if vlimit automatic turned off tube, false if not.
 **************************************************************************************************/

bool CAlarmMgmtHandler::isVlimitAutoTurnedOff_Tube()
{
	EnterCriticalSection(&csVGVLautoOffState);
	bool bState=m_bVlimitAutoTurnedOff_Tube;
	LeaveCriticalSection(&csVGVLautoOffState);
	return bState;
}

/**********************************************************************************************//**
 * Sets prico automatic turned off
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	bAutoTurnOn	True to enable, false to disable the automatic turn.
 * \param	prioAlarm  	The prio alarm.
 **************************************************************************************************/

void CAlarmMgmtHandler::setPRICOAutoTurnedOff(bool bAutoTurnOn, eAlarmPrio prioAlarm)
{
	DEBUGMSG(TRUE, (TEXT("CAlarmMgmtHandler::setPRICOAutoTurnedOff()\r\n")));
	
	EnterCriticalSection(&csPRICOautoOffState);
	m_bPRICOAutoTurnedOff=true;
	m_bPRICOAutoTurnOn=bAutoTurnOn;
	m_ePRICOAutoAlarmPriority=prioAlarm;
	LeaveCriticalSection(&csPRICOautoOffState);
}

/**********************************************************************************************//**
 * Resets the prico automatic turned off
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::resetPRICOAutoTurnedOff()
{
	//theApp.getLog()->WriteLine(_T("resetPRICOAutoTurnedOff"));
	DEBUGMSG(TRUE, (TEXT("CAlarmMgmtHandler::resetPRICOAutoTurnedOff()\r\n")));
	
	EnterCriticalSection(&csPRICOautoOffState);
	m_bPRICOAutoTurnedOff=false;
	m_bPRICOAutoTurnOn=false;
	m_ePRICOAutoAlarmPriority=AP_NONE;
	LeaveCriticalSection(&csPRICOautoOffState);
}

/**********************************************************************************************//**
 * Resets the prico automatic turne on flag
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::resetPRICOAutoTurneOnFlag()
{
	//theApp.getLog()->WriteLine(_T("resetPRICOAutoTurnedOff"));
	DEBUGMSG(TRUE, (TEXT("CAlarmMgmtHandler::resetPRICOAutoTurnedOff()\r\n")));

	EnterCriticalSection(&csPRICOautoOffState);
	m_bPRICOAutoTurnOn=false;
	LeaveCriticalSection(&csPRICOautoOffState);
}

/**********************************************************************************************//**
 * Query if this instance is prico automatic turned off
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if prico automatic turned off, false if not.
 **************************************************************************************************/

bool CAlarmMgmtHandler::isPRICOAutoTurnedOff()
{
	EnterCriticalSection(&csPRICOautoOffState);
	bool bState=m_bPRICOAutoTurnedOff;
	LeaveCriticalSection(&csPRICOautoOffState);
	return bState;
}

/**********************************************************************************************//**
 * Query if this instance is prico automatic turned on
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	True if prico automatic turned on, false if not.
 **************************************************************************************************/

bool CAlarmMgmtHandler::isPRICOAutoTurnedOn()
{
	EnterCriticalSection(&csPRICOautoOffState);
	bool bState=m_bPRICOAutoTurnOn;
	LeaveCriticalSection(&csPRICOautoOffState);
	return bState;
}

/**********************************************************************************************//**
 * Gets prico automatic alarm priority
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The prico automatic alarm priority.
 **************************************************************************************************/

eAlarmPrio CAlarmMgmtHandler::getPRICOAutoAlarmPriority()
{
	EnterCriticalSection(&csPRICOautoOffState);
	eAlarmPrio ePrio=m_ePRICOAutoAlarmPriority;
	LeaveCriticalSection(&csPRICOautoOffState);
	return ePrio;
}

/**********************************************************************************************//**
 * Gets alimit state MVmax limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state MVmax limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_MVmaxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_MVmaxLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state MVmin limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state mVmin limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_MVminLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_MVminLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state PIPmax limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state PIPmax limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_PIPmaxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_PIPmaxLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state PIPmin limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state PIPmin limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_PIPminLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_PIPminLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state PEEPmin limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state PEEPmin limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_PEEPminLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_PEEPminLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state bpmmax limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state bpmmax limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_BPMmaxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_BPMmaxLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state leakmax limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state leakmax limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_LeakmaxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_LeakmaxLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state apnoe limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state apnoe limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_ApnoeLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_ApnoeLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state MAPmax limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state MAPmax limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_MAPmaxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_MAPmaxLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state MAPmin limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state MAPmin limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_MAPminLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_MAPminLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state dco2max limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state dco2max limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_DCO2maxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_DCO2maxLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state dco2min limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state dco2min limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_DCO2minLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_DCO2minLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state etco2max limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state etco2max limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_ETCO2maxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_ETCO2maxLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state etco2min limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state etco2min limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_ETCO2minLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_ETCO2minLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state fico2max limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state fico2max limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_FICO2maxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_FICO2maxLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state fico2min limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state fico2min limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_FICO2minLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_FICO2minLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state spo2max limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state spo2max limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_SPO2maxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_SPO2maxLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state spo2min limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state spo2min limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_SPO2minLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_SPO2minLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state pulseratemax limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state pulseratemax limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_PulseRatemaxLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_PulseRatemaxLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state pulseratemin limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state pulseratemin limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_PulseRateminLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_PulseRateminLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state spo2 PImin limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state spo2 PImin limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_SPO2_PIminLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_SPO2_PIminLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Gets alimit state spo2 SIQmin limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit state spo2 SIQmin limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmMgmtHandler::getAlimitState_SPO2_SIQminLimit()
{
	eAlarmLimitState state=AL_OFF;
	if(ALARMLIMITS)
	{
		state=ALARMLIMITS->getAlimitState_SPO2_SIQminLimit();
	}
	return state;
}

/**********************************************************************************************//**
 * Sets alimit apnoe
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitApnoe(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitApnoe(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit bpmmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitBPMmax(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitBPMmax(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit MVmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMVmax(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMVmax(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit MVmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMVmin(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMVmin(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit leakmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitLeakmax(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitLeakmax(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit PEEPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitPEEPmin(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitPEEPmin(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit PIPmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitPIPmax(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitPIPmax(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit PIPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitPIPmin(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitPIPmin(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit MAPmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMAPmax(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMAPmax(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit MAPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMAPmin(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMAPmin(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit dco2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitDCO2max(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitDCO2max(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit dco2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitDCO2min(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitDCO2min(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit etco2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitETCO2max(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitETCO2max(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit etco2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitETCO2min(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitETCO2min(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit fico2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitFICO2max(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitFICO2max(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit fico2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitFICO2min(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitFICO2min(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit spo2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets alimit spo2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets alimit pulseratemax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitPulseRatemax(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitPulseRatemax(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit pulseratemin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitPulseRatemin(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitPulseRatemin(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit spo2 PImin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitSPO2_PImin(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitSPO2_PImin(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit spo2 SIQmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitSPO2_SIQmin(int value)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitSPO2_SIQmin(value);
	}
}

/**********************************************************************************************//**
 * Sets alimits minimum maximum range cpap
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitsMinMaxRangeCpap()
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitsMinMaxRangeCpap();
	}
}

/**********************************************************************************************//**
 * Sets alimits minimum maximum range ncpap
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitsMinMaxRangeNCPAP()
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitsMinMaxRangeNCPAP();
	}
}

/**********************************************************************************************//**
 * Sets alimits minimum maximum range duopap
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitsMinMaxRangeDUOPAP()
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitsMinMaxRangeDUOPAP();
	}
}

/**********************************************************************************************//**
 * Sets alimits minimum maximum range pressure
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitsMinMaxRangePressure()
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitsMinMaxRangePressure();
	}
}

/**********************************************************************************************//**
 * Gets alimit MVmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit MVmin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMVmin()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitMVmin();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit MVmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit MVmax.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMVmax()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitMVmax();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit PIPmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit PIPmax.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitPIPmax()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitPIPmax();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit PIPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit PIPmin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitPIPmin()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitPIPmin();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit PEEPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit PEEPmin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitPEEPmin()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitPEEPmin();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit leakmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit leakmax.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitLeakmax()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitLeakmax();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit bpmmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit bpmmax.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitBPMmax()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitBPMmax();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit apnoe
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit apnoe.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitApnoe()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitApnoe();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit MAPmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit MAPmax.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMAPmax()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitMAPmax();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit MAPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit MAPmin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMAPmin()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitMAPmin();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit dco2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit dco2min.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitDCO2min()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitDCO2min();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit dco2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit dco2max.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitDCO2max()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitDCO2max();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit etco2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit etco2min.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitETCO2min()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitETCO2min();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit etco2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit etco2max.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitETCO2max()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitETCO2max();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit fico2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit fico2min.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitFICO2min()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitFICO2min();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit fico2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit fico2max.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitFICO2max()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitFICO2max();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit spo2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit spo2max.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitSPO2max()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitSPO2max();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit spo2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit spo2min.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitSPO2min()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitSPO2min();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit pulseratemax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit pulseratemax.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitPulseRatemax()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitPulseRatemax();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit pulseratemin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit pulseratemin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitPulseRatemin()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitPulseRatemin();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit spo2 PImin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit spo2 PImin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitSPO2_PImin()
{
	int iLimit=0;
	if(ALARMLIMITS)
	{
		iLimit=ALARMLIMITS->getAlimitSPO2_PImin();
	}
	return iLimit;
}

/**********************************************************************************************//**
 * Gets alimit spo2 SIQmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit spo2 SIQmin.
 **************************************************************************************************/

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

/**********************************************************************************************//**
 * Sets alimit maximum range PIPmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangePIPmax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangePIPmax(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range PIPmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangePIPmax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangePIPmax(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range PIPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangePIPmin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangePIPmin(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range PIPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangePIPmin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangePIPmin(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range PEEPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangePEEPmin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangePEEPmin(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range PEEPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangePEEPmin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangePEEPmin(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range MVmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangeMVmin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeMVmin(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range MVmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangeMVmin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeMVmin(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range MVmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangeMVmax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeMVmax(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range MVmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangeMVmax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeMVmax(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range leakmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangeLeakmax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeLeakmax(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range leakmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangeLeakmax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeLeakmax(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range bpmmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangeBPMmax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeBPMmax(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range bpmmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangeBPMmax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeBPMmax(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range apnoe
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangeApnoe(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeApnoe(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range apnoe
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangeApnoe(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeApnoe(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range MAPmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangeMAPmax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeMAPmax(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range MAPmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangeMAPmax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeMAPmax(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range MAPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangeMAPmin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeMAPmin(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range MAPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangeMAPmin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeMAPmin(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range dco2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangeDCO2min(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeDCO2min(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range dco2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangeDCO2min(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeDCO2min(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range dco2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangeDCO2max(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeDCO2max(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range dco2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangeDCO2max(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeDCO2max(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range etco2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangeETCO2min(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeETCO2min(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range etco2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangeETCO2min(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeETCO2min(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range etco2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangeETCO2max(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeETCO2max(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range etco2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangeETCO2max(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeETCO2max(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range fico2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangeFICO2min(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeFICO2min(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range fico2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangeFICO2min(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeFICO2min(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range fico2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangeFICO2max(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeFICO2max(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range fico2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangeFICO2max(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeFICO2max(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range spo2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangeSPO2max(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		//DEBUGMSG(TRUE, (TEXT("XXX setAlimitMaxRangeSPO2max %d\r\n"),iRangeVal));
		ALARMLIMITS->setAlimitMaxRangeSPO2max(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range spo2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangeSPO2max(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		//DEBUGMSG(TRUE, (TEXT("XXX setAlimitMinRangeSPO2max %d\r\n"),iRangeVal));
		ALARMLIMITS->setAlimitMinRangeSPO2max(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range spo2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangeSPO2min(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		//DEBUGMSG(TRUE, (TEXT("XXX setAlimitMaxRangeSPO2min %d\r\n"),iRangeVal));
		ALARMLIMITS->setAlimitMaxRangeSPO2min(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range spo2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangeSPO2min(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		//DEBUGMSG(TRUE, (TEXT("XXX setAlimitMinRangeSPO2min %d\r\n"),iRangeVal));
		ALARMLIMITS->setAlimitMinRangeSPO2min(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range pulseratemin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangePulseRatemin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		//DEBUGMSG(TRUE, (TEXT("YYY setAlimitMaxRangePulseRatemin %d\r\n"),iRangeVal));
		ALARMLIMITS->setAlimitMaxRangePulseRatemin(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range pulseratemax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangePulseRatemax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		//DEBUGMSG(TRUE, (TEXT("YYY setAlimitMinRangePulseRatemax %d\r\n"),iRangeVal));
		ALARMLIMITS->setAlimitMinRangePulseRatemax(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range pulseratemax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangePulseRatemax(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		//DEBUGMSG(TRUE, (TEXT("YYY setAlimitMaxRangePulseRatemax %d\r\n"),iRangeVal));
		ALARMLIMITS->setAlimitMaxRangePulseRatemax(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range pulseratemin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangePulseRatemin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		//DEBUGMSG(TRUE, (TEXT("YYY setAlimitMinRangePulseRatemin %d\r\n"),iRangeVal));
		ALARMLIMITS->setAlimitMinRangePulseRatemin(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit maximum range spo2 PImin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMaxRangeSPO2_PImin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMaxRangeSPO2_PImin(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Sets alimit minimum range spo2 PImin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitMinRangeSPO2_PImin(int iRangeVal)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitMinRangeSPO2_PImin(iRangeVal);
	}
}

/**********************************************************************************************//**
 * Gets alimit maximum range PIPmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range PIPmax.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangePIPmax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangePIPmax();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range PIPmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range PIPmax.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangePIPmax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangePIPmax();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range PIPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range PIPmin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangePIPmin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangePIPmin();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range PIPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range PIPmin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangePIPmin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangePIPmin();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range PEEPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range PEEPmin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangePEEPmin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangePEEPmin();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range PEEPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range PEEPmin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangePEEPmin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangePEEPmin();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range MVmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range MVmin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangeMVmin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeMVmin();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range MVmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range MVmin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangeMVmin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeMVmin();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range MVmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range MVmax.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangeMVmax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeMVmax();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range MVmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range MVmax.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangeMVmax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeMVmax();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range leakmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range leakmax.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangeLeakmax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeLeakmax();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range leakmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range leakmax.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangeLeakmax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeLeakmax();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range bpmmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range bpmmax.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangeBPMmax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeBPMmax();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range bpmmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range bpmmax.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangeBPMmax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeBPMmax();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range apnoe
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range apnoe.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangeApnoe()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeApnoe();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range apnoe
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range apnoe.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangeApnoe()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeApnoe();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range MAPmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range MAPmax.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangeMAPmax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeMAPmax();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range MAPmax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range MAPmax.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangeMAPmax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeMAPmax();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range MAPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range MAPmin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangeMAPmin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeMAPmin();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range MAPmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range MAPmin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangeMAPmin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeMAPmin();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range dco2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range dco2min.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangeDCO2min()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeDCO2min();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range dco2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range dco2min.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangeDCO2min()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeDCO2min();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range dco2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range dco2max.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangeDCO2max()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeDCO2max();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range dco2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range dco2max.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangeDCO2max()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeDCO2max();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range etco2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range etco2min.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangeETCO2min()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeETCO2min();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range etco2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range etco2min.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangeETCO2min()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeETCO2min();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range etco2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range etco2max.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangeETCO2max()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeETCO2max();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range etco2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range etco2max.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangeETCO2max()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeETCO2max();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range fico2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range fico2min.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangeFICO2min()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeFICO2min();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range fico2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range fico2min.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangeFICO2min()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeFICO2min();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range fico2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range fico2max.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangeFICO2max()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeFICO2max();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range fico2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range fico2max.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangeFICO2max()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeFICO2max();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range spo2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range spo2max.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangeSPO2max()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeSPO2max();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range spo2max
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range spo2max.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangeSPO2max()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeSPO2max();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range spo2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range spo2min.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangeSPO2min()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeSPO2min();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range spo2min
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range spo2min.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangeSPO2min()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeSPO2min();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range pulseratemin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range pulseratemin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangePulseRatemin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangePulseRatemin();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range pulseratemax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range pulseratemax.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangePulseRatemax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangePulseRatemax();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range pulseratemax
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range pulseratemax.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangePulseRatemax()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangePulseRatemax();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range pulseratemin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range pulseratemin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangePulseRatemin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangePulseRatemin();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range spo2 PImin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range spo2 PImin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangeSPO2_PImin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeSPO2_PImin();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range spo2 PImin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range spo2 PImin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangeSPO2_PImin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeSPO2_PImin();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit maximum range spo2 SIQmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit maximum range spo2 SIQmin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMaxRangeSPO2_SIQmin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMaxRangeSPO2_SIQmin();
	}
	return iRange;
}

/**********************************************************************************************//**
 * Gets alimit minimum range spo2 SIQmin
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \return	The alimit minimum range spo2 SIQmin.
 **************************************************************************************************/

int CAlarmMgmtHandler::getAlimitMinRangeSPO2_SIQmin()
{
	int iRange=0;
	if(ALARMLIMITS)
	{
		iRange=ALARMLIMITS->getAlimitMinRangeSPO2_SIQmin();
	}
	return iRange;
}


/**********************************************************************************************//**
 * Sets alimit state apnoe limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_ApnoeLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_ApnoeLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state bpmmax limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_BPMmaxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_BPMmaxLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state leakmax limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_LeakmaxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_LeakmaxLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state MVmax limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_MVmaxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_MVmaxLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state MVmin limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_MVminLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_MVminLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state PEEPmin limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_PEEPminLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_PEEPminLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state PIPmax limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_PIPmaxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_PIPmaxLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state PIPmin limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_PIPminLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_PIPminLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state MAPmax limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_MAPmaxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_MAPmaxLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state MAPmin limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_MAPminLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_MAPminLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state dco2max limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_DCO2maxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_DCO2maxLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state dco2min limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_DCO2minLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_DCO2minLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state etco2max limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_ETCO2maxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_ETCO2maxLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state etco2min limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_ETCO2minLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_ETCO2minLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state fico2max limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_FICO2maxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_FICO2maxLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state fico2min limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_FICO2minLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_FICO2minLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state spo2max limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_SPO2maxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		DEBUGMSG(TRUE, (TEXT("XXX setAlimitState_SPO2maxLimit %d\r\n"),(int)state));
		ALARMLIMITS->setAlimitState_SPO2maxLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state spo2min limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_SPO2minLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		DEBUGMSG(TRUE, (TEXT("XXX setAlimitState_SPO2minLimit %d\r\n"),(int)state));
		ALARMLIMITS->setAlimitState_SPO2minLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state pulseratemax limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_PulseRatemaxLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_PulseRatemaxLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state pulseratemin limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_PulseRateminLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_PulseRateminLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state spo2 PImin limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_SPO2_PIminLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_SPO2_PIminLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state spo2 SIQmin limit
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmMgmtHandler::setAlimitState_SPO2_SIQminLimit(eAlarmLimitState state)
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->setAlimitState_SPO2_SIQminLimit(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state calculated
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::SetAlimitStateCalculated()
{
	if(ALARMLIMITS)
	{
		ALARMLIMITS->SetAlimitStateCalculated();
	}
}

/**********************************************************************************************//**
 * Check flow sensor state
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::CheckFlowSensorState()
{
	getModel()->CheckFlowsensorData();
}

/**********************************************************************************************//**
 * Check alarm state bytes
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::CheckAlarmStateBytes()
{
	getModel()->CheckAlarmData();
	g_eventNewAlarmData.SetEvent();
}

/**********************************************************************************************//**
 * Check limits
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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
	
	if(getModel()->getCONFIG()->getCO2module()!=CO2MODULE_NONE)
	{
		checkCO2Limits();
	}

	if(getModel()->getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
	{
		checkSpO2Limits();
	}
}

/**********************************************************************************************//**
 * Check ventilation limits
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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
	bool bMAPmax_Alarm=false;
	bool bMAPmin_Alarm=false;


	eAlarmLimitState stateMVmax = getAlimitState_MVmaxLimit();
	eAlarmLimitState stateMVmin = getAlimitState_MVminLimit();
	eAlarmLimitState statePIPmax = getAlimitState_PIPmaxLimit();
	eAlarmLimitState statePIPmin = getAlimitState_PIPminLimit();
	eAlarmLimitState statePEEPmin = getAlimitState_PEEPminLimit();
	eAlarmLimitState stateLeakmax = getAlimitState_LeakmaxLimit();
	eAlarmLimitState stateBPMmax = getAlimitState_BPMmaxLimit();
	eAlarmLimitState stateDCO2max = getAlimitState_DCO2maxLimit();
	eAlarmLimitState stateDCO2min = getAlimitState_DCO2minLimit();
	eAlarmLimitState stateMAPmax = getAlimitState_MAPmaxLimit();
	eAlarmLimitState stateMAPmin = getAlimitState_MAPminLimit();



	if(eActiveVentMode!=VM_CPAP && eActiveVentMode!=VM_NCPAP && eActiveVentMode!=VM_HFO)
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

	if(eActiveVentMode!=VM_HFO && eActiveVentMode!=VM_CPAP && eActiveVentMode!=VM_NCPAP && eActiveVentMode!=VM_THERAPIE)//rku PIPLOW DUOPAP
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

//eActiveVentMode==VM_NCPAP
	bool bCheckBPMinNCPAP=false;
	if(		getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false 
		&&	getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET 
		&&	getModel()->getDATAHANDLER()->PARADATA()->GetTriggerPara_NCPAP()!=MAXRANGE_TRIGGER_OFF
		&&	eActiveVentMode==VM_NCPAP)
	{
		bCheckBPMinNCPAP=true;
	}
	else if(	getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET 
			&&	getModel()->getDATAHANDLER()->PARADATA()->GetTriggerPara_NCPAP()!=MAXRANGE_TRIGGER_OFF
			&&	eActiveVentMode==VM_NCPAP)
	{
		bCheckBPMinNCPAP=true;
	}

	//eActiveVentMode==VM_NCPAP
	bool bCheckBPMinDUOPAP=false;
	if(		getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false 
		&&	getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET 
		&&	getModel()->getDATAHANDLER()->PARADATA()->GetTriggerPara_DUOPAP()!=MAXRANGE_TRIGGER_OFF
		&&	eActiveVentMode==VM_DUOPAP)
	{
		bCheckBPMinDUOPAP=true;
	}
	else if(	getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET 
		&&	getModel()->getDATAHANDLER()->PARADATA()->GetTriggerPara_DUOPAP()!=MAXRANGE_TRIGGER_OFF
		&&	eActiveVentMode==VM_DUOPAP)
	{
		bCheckBPMinDUOPAP=true;
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

			if(stateMAPmax==AL_ON || stateMAPmax==AL_AUTO)
			{
				if(	MessureData.m_iPmitt > getAlimitMAPmax())
				{
					bMAPmax_Alarm=true;
				}
			}

			if(stateMAPmin==AL_ON || stateMAPmin==AL_AUTO)
			{
				if( MessureData.m_iPmitt < getAlimitMAPmin())
				{
					bMAPmin_Alarm=true;
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
	else if(eActiveVentMode==VM_HFO
		&& getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
	{
		if(stateMAPmax==AL_ON || stateMAPmax==AL_AUTO)
		{
			if(	MessureData.m_iPmitt > getAlimitMAPmax())
			{
				bMAPmax_Alarm=true;
			}
		}

		if(stateMAPmin==AL_ON || stateMAPmin==AL_AUTO)
		{
			if( MessureData.m_iPmitt < getAlimitMAPmin())
			{
				bMAPmin_Alarm=true;
			}
		}
	}
	else if((eActiveVentMode==VM_SIMV || eActiveVentMode==VM_SIPPV ||eActiveVentMode==VM_CPAP)
			&& getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
	{
		if(stateBPMmax==AL_ON || stateBPMmax==AL_AUTO)
		{
			if(	MessureData.m_iBPM > getAlimitBPMmax())
			{
				bBPMmax_Alarm=true;
			}
		}
	}
	else if(bCheckBPMinNCPAP || bCheckBPMinDUOPAP)
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
	eAlarmStates.eSoA_MAPmaxLimit=m_pAlarmlist->getAlarm(AL_PatAl_MAPmax)->getAlarmState();
	eAlarmStates.eSoA_MAPminLimit=m_pAlarmlist->getAlarm(AL_PatAl_MAPmin)->getAlarmState();
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
		if(eActiveVentMode!=VM_HFO && eActiveVentMode!=VM_CPAP && eActiveVentMode!=VM_NCPAP && eActiveVentMode!=VM_THERAPIE)//rku PIPLOW DUOPAP
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
		if(bCheckBPMinNCPAP || bCheckBPMinDUOPAP)
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
		if(		bMAPmax_Alarm 
			&&	eAlarmStates.eSoA_MAPmaxLimit!=AS_ACTIVE 
			&&	getAlarmSilentState()!=ASTATE_AUTOSILENT
			&&	getModel()->isActiveAlarmDelay()==false)
		{
			sz.Format(_T(" [%0.1f]"),CTlsFloat::Round(((double)MessureData.m_iPmitt)/10, 1));
			setAlarm(AL_PatAl_MAPmax,sz);
		}
		else if(!bMAPmax_Alarm && eAlarmStates.eSoA_MAPmaxLimit==AS_ACTIVE)
		{
			setStateOfAlarm(AL_PatAl_MAPmax,AS_SIGNALED);
		}

		if(		bMAPmin_Alarm 
			&&	eAlarmStates.eSoA_MAPminLimit!=AS_ACTIVE 
			&&	getAlarmSilentState()!=ASTATE_AUTOSILENT
			&&	getModel()->isActiveAlarmDelay()==false)
		{
			sz.Format(_T(" [%0.1f]"),CTlsFloat::Round(((double)MessureData.m_iPmitt)/10, 1));
			setAlarm(AL_PatAl_MAPmin,sz);
		}
		else if(!bMAPmin_Alarm && eAlarmStates.eSoA_MAPminLimit==AS_ACTIVE)
		{
			setStateOfAlarm(AL_PatAl_MAPmin,AS_SIGNALED);
		}


		/********************************************/
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

/**********************************************************************************************//**
 * Check co2 limits
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

void CAlarmMgmtHandler::checkCO2Limits()
{
	if(getModel()->getCONFIG()->getCO2module()!=CO2MODULE_NONE && getModel()->getETCO2())
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

/**********************************************************************************************//**
 * Check spo2 limits
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 **************************************************************************************************/

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
		eSPO2alarmdelay eDelayTime=getModel()->getCONFIG()->getSPO2alarmDelay();
		UINT iDelayTime=0;
		switch(eDelayTime)
		{
		case SPO2_ADELAY_0:
			{
				iDelayTime=0;
			}
			break;
		case SPO2_ADELAY_5:
			{
				iDelayTime=5;
			}
			break;
		case SPO2_ADELAY_10:
			{
				iDelayTime=10;
			}
			break;
		case SPO2_ADELAY_15:
			{
				iDelayTime=15;
			}
			break;
		default:
			{
				iDelayTime=10;
			}
			break;
		}

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

/**********************************************************************************************//**
 * Query if 'oldTickCount' is safe tick count delay expired
 *
 * \author	Rainer Kühner
 * \date	21.02.2018
 *
 * \param	oldTickCount	Number of old ticks.
 * \param	delay			The delay.
 *
 * \return	True if safe tick count delay expired, false if not.
 **************************************************************************************************/

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