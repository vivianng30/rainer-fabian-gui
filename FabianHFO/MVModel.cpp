/**********************************************************************************************//**
 * \file	MVModel.cpp.
 *
 * Implements the mv model class
 **************************************************************************************************/

#include "StdAfx.h"
#include "MVModel.h"
//#include "acuLink.h"
#include "MVViewHandler.h"
#include <iphlpapi.h>
#include "TlsFile.h"
#include "TlsRegistry.h"

//#include "acuLinkLib.h"

/**********************************************************************************************//**
 * A macro that defines volgaranty delay
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

#define VOLGARANTY_DELAY 2000

CRITICAL_SECTION CMVModel::m_csO2Flush;
CRITICAL_SECTION CMVModel::m_csSerial;
CRITICAL_SECTION CMVModel::m_csI2C;
//CRITICAL_SECTION CMVModel::m_csI2Cinit;
CRITICAL_SECTION CMVModel::m_csObservers;
CRITICAL_SECTION CMVModel::m_csLangString;
CRITICAL_SECTION CMVModel::m_csTrigger;
CRITICAL_SECTION CMVModel::m_csETCO2;
CRITICAL_SECTION CMVModel::m_csSPO2;
CRITICAL_SECTION CMVModel::m_csVentModeInit;

CMVModel* CMVModel::theModel=0;

CThreadMonitor* CMVModel::MONITORTHR=NULL;
CThreadPRICO* CMVModel::PRICOTHR=NULL;
CThreadFOT* CMVModel::FOTTHR=NULL;
CThreadAlarm* CMVModel::ALARMTHR=NULL;
CThreadFlowsensor* CMVModel::FLOWSENSORTHR=NULL;
CThreadExspiration* CMVModel::EXSPIRATIONTHR=NULL;

CMVViewHandler* CMVModel::VIEWHANDLER=NULL;
CVentModeHandler* CMVModel::MODEHANDLER=NULL;
CDataHandler* CMVModel::DATAHANDLER=NULL;
CInterfaceTerminal* CMVModel::TERMINAL=NULL;
CInterfaceAcuLink* CMVModel::ACULINK=NULL;
CInterfaceCO2* CMVModel::ETCO2=NULL;
CInterfaceSPO2* CMVModel::SPO2=NULL;
CInterfaceSerial* CMVModel::SERIAL=NULL;
CInterfaceSPI* CMVModel::SPI=NULL;
CInterfaceFSBus* CMVModel::PIF=NULL;
CInterfaceDIO* CMVModel::DIO=NULL;
CInterfaceI2C* CMVModel::I2C=NULL;
CSoundPlayer* CMVModel::SOUND=NULL;
CAlarmMgmtHandler*  CMVModel::ALARMHANDLER=NULL;
CConfiguration* CMVModel::CONFIG=NULL;
CLanguage* CMVModel::LANGUAGE=NULL;

/**********************************************************************************************//**
 * A macro that defines malloc
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	x	A void to process.
 **************************************************************************************************/

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))

/**********************************************************************************************//**
 * A macro that defines free
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	x	A void to process.
 **************************************************************************************************/

#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

CList <CModelObserver*, CModelObserver*> CMVModel::m_ObserverList(10);

/**********************************************************************************************//**
 * Notifies the calculate alarm limit
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CModelObserver::NotifyCalculateAlarmLimit(){}

/**********************************************************************************************//**
 * Notifies the alarm limit changed
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CModelObserver::NotifyAlarmLimitChanged(){}

/**********************************************************************************************//**
 * Notifies the data changed
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CModelObserver::NotifyDataChanged(){}

/**********************************************************************************************//**
 * Notifies the exspiration data changed
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CModelObserver::NotifyExspirationDataChanged(){}

/**********************************************************************************************//**
 * Notifies the view state changed
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CModelObserver::NotifyViewStateChanged(){}

/**********************************************************************************************//**
 * Notifies the vent mode changed
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CModelObserver::NotifyVentModeChanged(){}

/**********************************************************************************************//**
 * Notifies the language changed
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CModelObserver::NotifyLanguageChanged(){}

/**********************************************************************************************//**
 * Notifies an event
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param [in,out]	pEvent	If non-null, the event.
 **************************************************************************************************/

void CModelObserver::NotifyEvent(CMVEvent* pEvent){}

/**********************************************************************************************//**
 * Notifies a para button event
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param [in,out]	pEvent	If non-null, the event.
 **************************************************************************************************/

void CModelObserver::NotifyParaBtnEvent(CMVEvent* pEvent){}

/**********************************************************************************************//**
 * Notifies a view focus changed
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iViewID	Identifier for the view.
 **************************************************************************************************/

void CModelObserver::NotifyViewFocusChanged(int iViewID){}

/**********************************************************************************************//**
 * Initializes a new instance of the CMVModel class
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

CMVModel::CMVModel(void)
{
	InitializeCriticalSection(&m_csLangString);
	InitializeCriticalSection(&m_csTrigger);
	InitializeCriticalSection(&m_csObservers);
	InitializeCriticalSection(&m_csI2C);
	InitializeCriticalSection(&m_csSerial);
	InitializeCriticalSection(&m_csO2Flush);
	InitializeCriticalSection(&m_csETCO2);
	InitializeCriticalSection(&m_csSPO2);
	InitializeCriticalSection(&m_csVentModeInit);
	
	m_bReloadLangInProgress=false;

	m_szUniqueID=_T("");

	m_eVentModePrevFOT=VM_NONE;
	m_iBackupPrevFOT=0;

	m_bLoadHospitalSettingsFailed=false;
	m_bVentilationRangeChanged=false;

	m_bVideoRunning=false;

	m_szFontName=_T("arial");
	m_wLanguageID=0;

	m_iOldPPSV=0;
	m_bActiveAlarmDelay=false;

	m_bCalculateAlarmlimitRunning=false;

	m_bO2calFlag=true;
	m_bFlowsensor=true;

	m_bPatAlarmO2dependendEnabled=true;

	m_bAccuTurnOff=false;
	m_bInstaller=false;
	
	m_bCO2inprogress=false;

	
#ifdef MEDKOM_VERSION
	m_szVersion = _T("9.0.0");
	m_szBuildVersion = _T("9.0.0.0");
#else
	m_szVersion = _T("5.0.8");
	m_szBuildVersion = _T("5.0.8.13");
#endif

	CTlsRegistry regWorkState(_T("HKCU\\Software\\FabianHFO"),true);
	regWorkState.WriteString(_T("Version"), m_szBuildVersion);

	//<major release><minor release><patch level><build number>
	CString szVersion = _T("fabianHFO ");
	szVersion+=m_szBuildVersion;
	theApp.WriteLog(szVersion);

	m_bSPO2running=false;
	m_bCalibrationViewActive=false;
	m_dwLastVolGarAlarmSound=0;
	m_bSERIALavailability=FALSE;
	m_bSPIavailability=FALSE;
	m_bDIOavailability=FALSE;
	m_bPIFavailability=FALSE;
	m_bAcuLinkAvailability=FALSE;
	m_bO2FlushActive=false;
	m_bMANBREATHrunning=false;

	m_eVentRunState=VENT_RUNNING;

	m_iCountDownAlarmSilent=0;

	m_bVlimitted=false;
	m_eVGaranty=VGAL_NONE;

	m_bAutoResetFromPreset = false;

	m_bSPIDisconnection=false;
	m_bSPI_TUBE_OCCLUSION=false;

	m_bExit=false;

	m_bVentModeInitialized=false;

	//m_bI2Cinitialized=false;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CMVModel class
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

CMVModel::~CMVModel(void)
{
	if(LANGUAGE)
		LANGUAGE->DestroyInstance();
	LANGUAGE=NULL;

	//DeleteCriticalSection(&m_csI2Cinit);
	DeleteCriticalSection(&m_csVentModeInit);
	DeleteCriticalSection(&m_csETCO2);
	DeleteCriticalSection(&m_csSPO2);
	DeleteCriticalSection(&m_csO2Flush);
	DeleteCriticalSection(&m_csLangString);
	DeleteCriticalSection(&m_csTrigger);
	DeleteCriticalSection(&m_csObservers);
	DeleteCriticalSection(&m_csI2C);
	DeleteCriticalSection(&m_csSerial);
}

/**********************************************************************************************//**
 * Gets the instance
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the instance.
 **************************************************************************************************/

CMVModel* CMVModel::GetInstance()
{
	if(theModel == 0)
	{
		theModel = new CMVModel;
	}
	return theModel;
}

/**********************************************************************************************//**
 * Destroys the instance
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::DestroyInstance()
{
	if(theModel != NULL)
	{
		delete theModel;
		theModel = NULL;
	}
}

/**********************************************************************************************//**
 * Gets alarm thread
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the alarm thread.
 **************************************************************************************************/

CThreadAlarm *CMVModel::getAlarmThread()
{
	if(ALARMTHR==NULL)
	{
		ALARMTHR=CThreadAlarm::getInstance();
	}
	return ALARMTHR;
}

/**********************************************************************************************//**
 * Gets flowsensor thread
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the flowsensor thread.
 **************************************************************************************************/

CThreadFlowsensor *CMVModel::getFlowsensorThread()
{
	if(FLOWSENSORTHR==NULL)
	{
		FLOWSENSORTHR=CThreadFlowsensor::getInstance();
	}
	return FLOWSENSORTHR;
}

/**********************************************************************************************//**
 * Gets exspiration thread
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the exspiration thread.
 **************************************************************************************************/

CThreadExspiration *CMVModel::getExspirationThread()
{
	if(EXSPIRATIONTHR==NULL)
	{
		EXSPIRATIONTHR=CThreadExspiration::getInstance();
	}
	return EXSPIRATIONTHR;
}

/**********************************************************************************************//**
 * Gets prico thread
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the prico thread.
 **************************************************************************************************/

CThreadPRICO *CMVModel::getPRICOThread()
{
	if(PRICOTHR==NULL)
	{
		//DEBUGMSG(TRUE, (TEXT("#########CThreadFOT::getPRICOThread()\r\n")));
		//theApp.WriteLog(_T("#ERROR: PRICOTHR"));
		//PRICOTHR=CThreadPRICO::getInstance();
	}
	return PRICOTHR;
}

/**********************************************************************************************//**
 * Gets fot thread
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the fot thread.
 **************************************************************************************************/

CThreadFOT *CMVModel::getFOTThread()
{
	if(FOTTHR==NULL)
	{
		//int iSTop=0;
		//DEBUGMSG(TRUE, (TEXT("#########CThreadFOT::getInstance()\r\n")));
		//theApp.WriteLog(_T("#ERROR: FOTTHR"));
		//FOTTHR=CThreadFOT::getInstance();
	}
	return FOTTHR;
}

/**********************************************************************************************//**
 * Gets monitor thread
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the monitor thread.
 **************************************************************************************************/

CThreadMonitor *CMVModel::getMonitorThread()
{
	if(MONITORTHR==NULL)
	{
		MONITORTHR=CThreadMonitor::getInstance();
	}
	return MONITORTHR;
}

/**********************************************************************************************//**
 * Vmodehandler, called when the get vmode
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the vmodehandler.
 **************************************************************************************************/

CVentModeHandler *CMVModel::getVMODEHANDLER()
{
	if(MODEHANDLER==NULL)
	{
		MODEHANDLER=CVentModeHandler::getInstance();
	}
	return MODEHANDLER;
}

/**********************************************************************************************//**
 * Datahandler, called when the get data
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the datahandler.
 **************************************************************************************************/

CDataHandler *CMVModel::getDATAHANDLER()
{
	if(DATAHANDLER==NULL)
	{
		DATAHANDLER=CDataHandler::getInstance();
	}
	return DATAHANDLER;
}

/**********************************************************************************************//**
 * Gets the configuration
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the configuration.
 **************************************************************************************************/

CConfiguration *CMVModel::getCONFIG()
{
	if(CONFIG==NULL)
	{
		CONFIG=CConfiguration::GetInstance();
	}
	return CONFIG;
}

/**********************************************************************************************//**
 * Gets the language
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the language.
 **************************************************************************************************/

CLanguage *CMVModel::getLANGUAGE()
{ 
	if(LANGUAGE==NULL)
	{
		LANGUAGE=CLanguage::GetInstance();
	}
	return LANGUAGE;
}

/**********************************************************************************************//**
 * Gets the terminal
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the terminal.
 **************************************************************************************************/

CInterfaceTerminal *CMVModel::getTERMINAL()
{ 
	if(TERMINAL==NULL)
	{
		if(getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL_REMOTE)
		{
			TERMINAL=CInterfaceTerminal::GetInstance(ACL_TERMINAL_REMOTE);
		}
		else if(getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL_WAVE)
		{
			TERMINAL=CInterfaceTerminal::GetInstance(ACL_TERMINAL_WAVE);
		}
	}
	return TERMINAL;
}

/**********************************************************************************************//**
 * Gets aculink
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the aculink.
 **************************************************************************************************/

CInterfaceAcuLink *CMVModel::getAcuLink()
{ 
	if(ACULINK==NULL)
	{
		ACULINK=CInterfaceAcuLink::getInstance(getCONFIG()->GetPDMSprotocol());
	}
	return ACULINK;
}

/**********************************************************************************************//**
 * Gets etco2
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the etco2.
 **************************************************************************************************/

CInterfaceCO2 *CMVModel::getETCO2()
{ 
	EnterCriticalSection(&m_csETCO2);
	if(ETCO2==NULL && getCONFIG()->getCO2module()!=CO2MODULE_NONE)
	{
		ETCO2=CInterfaceCO2::GetInstance(getCONFIG()->getCO2module());
	}
	LeaveCriticalSection(&m_csETCO2);
	
	return ETCO2;
}

/**********************************************************************************************//**
 * Gets spo2
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the spo2.
 **************************************************************************************************/

CInterfaceSPO2 *CMVModel::getSPO2()
{ 
	EnterCriticalSection(&m_csSPO2);
	if(SPO2==NULL && getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
	{
		SPO2=CInterfaceSPO2::GetInstance(getCONFIG()->getSPO2module());
	}
	LeaveCriticalSection(&m_csSPO2);
	
	return SPO2;
}

/**********************************************************************************************//**
 * Gets the serial
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the serial.
 **************************************************************************************************/

CInterfaceSerial *CMVModel::getSERIAL()
{ 
	EnterCriticalSection(&m_csSerial);
	if(SERIAL==NULL)
	{
		SERIAL=CInterfaceSerial::GetInstance();
	}
	LeaveCriticalSection(&m_csSerial);
	
	return SERIAL;
}

/**********************************************************************************************//**
 * Gets the spi
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the spi.
 **************************************************************************************************/

CInterfaceSPI *CMVModel::getSPI()
{
	if(SPI==NULL)
	{
		SPI=CInterfaceSPI::GetInstance();
	}
	return SPI;
}

/**********************************************************************************************//**
 * Gets the pif
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the pif.
 **************************************************************************************************/

CInterfaceFSBus *CMVModel::getPIF()
{ 
	if(PIF==NULL)
	{
		PIF=CInterfaceFSBus::GetInstance();
	}
	return PIF;
}

/**********************************************************************************************//**
 * Gets the dio
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the dio.
 **************************************************************************************************/

CInterfaceDIO *CMVModel::getDIO()
{ 
	if(DIO==NULL /*&& !m_bDIOerror*/)
	{
		DIO=CInterfaceDIO::GetInstance();
	}
	return DIO;
}

/**********************************************************************************************//**
 * Gets i2c
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the i2c.
 **************************************************************************************************/

CInterfaceI2C *CMVModel::getI2C()
{ 
	EnterCriticalSection(&m_csI2C);
	if(I2C==NULL)
	{
		I2C=CInterfaceI2C::GetInstance();
	}
	LeaveCriticalSection(&m_csI2C);
	return I2C;
}

/**********************************************************************************************//**
 * Gets the sound
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the sound.
 **************************************************************************************************/

CSoundPlayer *CMVModel::getSOUND()
{ 
	if(SOUND==NULL)
	{
		SOUND=CSoundPlayer::GetInstance();
	}
	return SOUND;
}

/**********************************************************************************************//**
 * get the alarmhandler
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else a larmhandler.
 **************************************************************************************************/

CAlarmMgmtHandler* CMVModel::getALARMHANDLER()
{ 
	if(ALARMHANDLER==NULL)
	{
		ALARMHANDLER=CAlarmMgmtHandler::getInstance();
	}
	return ALARMHANDLER;
}

/**********************************************************************************************//**
 * Initializes this instance
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	szFontName 	Name of the font.
 * \param	wLanguageID	Identifier for the language.
 **************************************************************************************************/

void CMVModel::Init(CStringW szFontName, WORD wLanguageID)
{
	theApp.WriteLog(_T("#M:001"));
	m_szFontName=szFontName;
	m_wLanguageID=wLanguageID;


	//TEST
	/*if(CTlsFile::Exists(_T("\\FFSDISK\\acuLinkLib.dll")))
	{
		HINSTANCE hLibrary = ::LoadLibrary(_T("acuLinkLib.dll"));
		if (hLibrary != NULL)
		{
			pGetTestAddFunc = (LP_GET_TEST_ADD_DLLFUNC)::GetProcAddress(hLibrary, L"AddTest");

			int iTest=this->pGetTestAddFunc(5,6);
			int iStoip=0;
		}	
	}*/

	GetAdapterInfo();
	checkUniqueID();
	
	theApp.WriteLog(_T("#M:002"));

#ifndef SIMULATION_VERSION
	I2C=CInterfaceI2C::GetInstance();
	if(I2C==NULL)
	{
		theApp.WriteLog(_T("#ERROR: I2C NULL"));
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_I2C);
	}
	else if(I2C->Init(true)==false)
	{
		I2C=NULL;
		theApp.WriteLog(_T("#ERROR: I2C INIT"));
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_I2C);
	}
#endif

	Sleep(50);
	//setI2Cinitialized();
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_START_I2CWATCHDOG);
	//Sleep(0);

	theApp.WriteLog(_T("#M:003"));

	DEBUGMSG(TRUE, (TEXT("#########init interfaceses\r\n")));

	LANGUAGE=CLanguage::GetInstance();
	CONFIG=CConfiguration::GetInstance();

	theApp.WriteLog(_T("#M:004"));

	ALARMHANDLER = CAlarmMgmtHandler::getInstance();

	theApp.WriteLog(_T("#M:005"));

#ifndef SIMULATION_NOSERIAL
	SERIAL=CInterfaceSerial::GetInstance();
#endif

	theApp.WriteLog(_T("#M:006"));

#ifndef SIMULATION_NOSPI
	SPI=CInterfaceSPI::GetInstance();
#endif
	
	theApp.WriteLog(_T("#M:007"));

	PIF=CInterfaceFSBus::GetInstance();
	DIO=CInterfaceDIO::GetInstance();

	theApp.WriteLog(_T("#M:008"));

	DEBUGMSG(TRUE, (TEXT("#########getinstance datahandler\r\n")));
	DATAHANDLER=CDataHandler::getInstance();

	theApp.WriteLog(_T("#M:009"));

	MODEHANDLER=CVentModeHandler::getInstance();
	MONITORTHR=CThreadMonitor::getInstance();
	ALARMTHR=CThreadAlarm::getInstance();
	FLOWSENSORTHR=CThreadFlowsensor::getInstance();
	EXSPIRATIONTHR=CThreadExspiration::getInstance();
	SOUND=CSoundPlayer::GetInstance();	

	theApp.WriteLog(_T("#M:010"));

	//###################################################
	//init phase

	DEBUGMSG(TRUE, (TEXT("#########init I2C\r\n")));

//#ifndef SIMULATION_VERSION
//	if(I2C==NULL)
//	{
//		theApp.WriteLog(_T("#ERROR: I2C NULL"));
//		if(AfxGetApp())
//			AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_I2C);
//	}
//	else if(I2C->Init(true)==false)
//	{
//		I2C=NULL;
//		theApp.WriteLog(_T("#ERROR: I2C INIT"));
//		if(AfxGetApp())
//			AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_I2C);
//	}
//#endif

	DEBUGMSG(TRUE, (TEXT("#########init config\r\n")));
	CONFIG->Init();
	
	theApp.WriteLog(_T("#M:011"));

	DEBUGMSG(TRUE, (TEXT("#########init language\r\n")));
	LANGUAGE->Init();

	
	if(PIF->Init())
	{
		SetPIFavailability(TRUE);
	}
	else
	{
		SetPIFavailability(FALSE);
	}

	SOUND->Init();//after PIF init!

	DEBUGMSG(TRUE, (TEXT("#########init datahandler\r\n")));
	theApp.WriteLog(_T("#M:012"));

	DATAHANDLER->init();

	DEBUGMSG(TRUE, (TEXT("#########init datahandler finished\r\n")));
	theApp.WriteLog(_T("#M:013"));

	ALARMHANDLER->init();

	DEBUGMSG(TRUE, (TEXT("#########init alarmhandler finished\r\n")));
	theApp.WriteLog(_T("#M:014"));

	if(DIO->Init())
	{
		SetDIOavailability(TRUE);
	}
	else
	{
		SetDIOavailability(FALSE);
	}

	DEBUGMSG(TRUE, (TEXT("#########init SPI\r\n")));
	theApp.WriteLog(_T("#M:015"));

#ifndef SIMULATION_NOSPI
	if(SPI!=NULL && SPI->Init())
	{
		setSPIavailability(TRUE);
	}
	else
	{
		getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_INIT);
		setSPIavailability(FALSE);
		theApp.WriteLog(_T("#HFO:0227"));
	}
#endif

	DEBUGMSG(TRUE, (TEXT("#########init acuLink\r\n")));
	theApp.WriteLog(_T("#M:016"));

	initAcuLink();

	DEBUGMSG(TRUE, (TEXT("#########init SERIAL\r\n")));
	theApp.WriteLog(_T("#M:017"));

	initSerialController();

	DEBUGMSG(TRUE, (TEXT("#########datahandler start\r\n")));
	theApp.WriteLog(_T("#M:018"));

	DATAHANDLER->start();//serial must be initialized first, sends down start command to serial PIC (==hardware configuration)
	
	theApp.WriteLog(_T("#M:019"));
	
	if(VIEWHANDLER==NULL)
	{
		VIEWHANDLER=CMVViewHandler::GetInstance();
	}

	theApp.WriteLog(_T("#M:020"));
	DEBUGMSG(TRUE, (TEXT("#########init modules\r\n")));

	initCO2module();

	theApp.WriteLog(_T("#M:021"));

	initSPO2module();

	theApp.WriteLog(_T("#M:022"));

	DEBUGMSG(TRUE, (TEXT("#########init PRICO\r\n")));
	if(getDATAHANDLER()->isPRICOLicenseAvailable()==true)
	{
		theApp.WriteLog(_T("#M:022.1"));
		initPRICOthread();
	}
	if(getDATAHANDLER()->isFOTLicenseAvailable()==true)
	{
		theApp.WriteLog(_T("#M:022.2"));
		getFOTThread()->loadHFO_FOTvalues();
	}
	if(getDATAHANDLER()->isLUNGRECLicenseAvailable()==false && getCONFIG()->GetParaDataFREQ_REC()!=0)
	{
		theApp.WriteLog(_T("#M:022.3"));
		getCONFIG()->SetParaDataFREQ_REC(FACTORY_HFO_FREQREC);
	}
	
	initTerminal();
	theApp.WriteLog(_T("#M:023"));
	
	
#ifdef SIMULATION_NOSPI
	theApp.WriteLog(_T("+++SIMULATION_NOSPI+++"));
#endif
#ifdef SIMULATION_VERSION
	theApp.WriteLog(_T("+++SIMULATION_VERSION+++"));
#endif
#ifdef SIMULATION_NOSERIAL
	theApp.WriteLog(_T("+++SIMULATION_NOSERIAL+++"));
#endif
#ifdef SIMULATION_ENTREK
	theApp.WriteLog(_T("+++SIMULATION_ENTREK+++"));
#endif

	DEBUGMSG(TRUE, (TEXT("#########write versions\r\n")));
	theApp.WriteLog(_T("#M:024"));

	writeMainboardVersionToLog();
	writeVentRangeToLog();
	writeKernelVersionToLog();
	writeConfigVersionToLog();
	writeBIASFlowStateToLog();
	writeLEAKCOMPENSATIONToLog();
	writeALTITUDEToLog();

	theApp.WriteLog(_T("#M:025"));

	DEBUGMSG(TRUE, (TEXT("#########start threads\r\n")));
	startThreads();

	if(getSERIAL())
		getSERIAL()->Send_FOTconv_AMP_FREQ(0);//turn FOT off at startup
	
	DEBUGMSG(TRUE, (TEXT("#########WM STARTUP\r\n")));
	if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_STARTUP);

	theApp.WriteLog(_T("#M:026"));
}

//void CMVModel::setI2Cinitialized()
//{
//	EnterCriticalSection(&m_csI2Cinit);
//	m_bI2Cinitialized=true;
//	LeaveCriticalSection(&m_csI2Cinit);
//}
//bool CMVModel::getI2Cinitialized()
//{
//	bool bTemp=false;
//	EnterCriticalSection(&m_csI2Cinit);
//	bTemp=m_bI2Cinitialized;
//	LeaveCriticalSection(&m_csI2Cinit);
//	return bTemp;
//}

/**********************************************************************************************//**
 * Initializes the terminal
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::initTerminal()
{
	if(getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL_REMOTE)
	{
		TERMINAL=CInterfaceTerminal::GetInstance(ACL_TERMINAL_REMOTE);
		TERMINAL->Init();
	}
	else if(getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL_WAVE)
	{
		TERMINAL=CInterfaceTerminal::GetInstance(ACL_TERMINAL_WAVE);
		TERMINAL->Init();
	}
}

/**********************************************************************************************//**
 * Deinit terminal
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::deinitTerminal()
{
	if(TERMINAL)
	{
		TERMINAL->Deinit();
		TERMINAL->DestroyInstance();
	}
	TERMINAL=NULL;
}

/**********************************************************************************************//**
 * Initializes the prico thread
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::initPRICOthread()
{
	PRICOTHR=CThreadPRICO::getInstance();
}

/**********************************************************************************************//**
 * Deinit prico thread
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::deinitPRICOthread()
{
	bool bPRICOrunning=false;
	if(getPRICOThread())
	{
		if(getPRICOThread()->isPRICOalgorithmRunning())
		{
			bPRICOrunning=true;
		}
	}
	if(bPRICOrunning )
	{
		getDATAHANDLER()->setPRICOoff();
	}
	Sleep(200);

	if(PRICOTHR)
	{
		PRICOTHR->destroyInstance();
		PRICOTHR=NULL;
	}
}

/**********************************************************************************************//**
 * Initializes the fot thread
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::initFOTthread()
{
	FOTTHR=CThreadFOT::getInstance();
}

/**********************************************************************************************//**
 * Deinit fot thread
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::deinitFOTthread()
{
	if(FOTTHR)
	{
		FOTTHR->destroyInstance();
		FOTTHR=NULL;
	}
}

/**********************************************************************************************//**
 * Initializes the serial controller
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::initSerialController()
{
	EnterCriticalSection(&m_csSerial);
	if(SERIAL==NULL)
	{
		setSERIALavailable(FALSE);
	}
	else if(SERIAL->Init())
	{
		setSERIALavailable(TRUE);
	}
	else
	{
		getDATAHANDLER()->setCOMErrorCode(ERRC_COM_INIT);
		setSERIALavailable(FALSE);
	}
	LeaveCriticalSection(&m_csSerial);
}

/**********************************************************************************************//**
 * Starts the threads
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::startThreads()
{
	if(getMonitorThread())
		getMonitorThread()->startMonitorThread();
	if(getAlarmThread())
		getAlarmThread()->startAlarmThread();
	if(getFlowsensorThread())
		getFlowsensorThread()->startFlowsensorThread();
	if(getExspirationThread())
		getExspirationThread()->startExspirationThread();
}

/**********************************************************************************************//**
 * Writes the vent range to log
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::writeVentRangeToLog()
{
	if(getCONFIG()->GetVentRange()==NEONATAL)
		theApp.WriteLog(_T("### NEONATAL MODE"));
	else
		theApp.WriteLog(_T("### PEDIATRIC MODE"));
}

/**********************************************************************************************//**
 * Writes altitude to log
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::writeALTITUDEToLog()
{
	WORD iAltitude=getI2C()->ReadConfigWord(ALTITUDE_16);
	CStringW szAlt=_T("");
	szAlt.Format(_T("### ALTITUDE %d"), iAltitude);
	theApp.WriteLog(szAlt);
}

/**********************************************************************************************//**
 * Writes the leakcompensation to log
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::writeLEAKCOMPENSATIONToLog()
{
	eLeakCompensation eLeakCompOff=getCONFIG()->getLeakCompensation();
	switch(eLeakCompOff)
	{
	/*case LC_OFF:
		{
			theApp.WriteLog(_T("### LeakCompensatino disabled"));
		}
		break;*/
	case LC_LOW:
		{
			theApp.WriteLog(_T("### LeakCompensation low"));
		}
		break;
	case LC_MIDDLE:
		{
			theApp.WriteLog(_T("### LeakCompensation middle"));
		}
		break;
	case LC_HIGH:
		{
			theApp.WriteLog(_T("### LeakCompensation high"));
		}
		break;
	}
}	

/**********************************************************************************************//**
 * Writes the bias flow state to log
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::writeBIASFlowStateToLog()
{
	if(getCONFIG()->isBiasFlowActive())
	{
		theApp.WriteLog(_T("### BiasFlow enabled"));
	}
	else
	{
		theApp.WriteLog(_T("### BiasFlow disabled"));
	}
}

/**********************************************************************************************//**
 * Writes the configuration version to log
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::writeConfigVersionToLog()
{
	CStringW szConfigVersion=_T("");
	szConfigVersion.Format(_T("### Config-Version %d"), getCONFIG()->getConfigVersion());
	theApp.WriteLog(szConfigVersion);
}

/**********************************************************************************************//**
 * Writes the kernel version to log
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::writeKernelVersionToLog()
{
	CString csKernel=_T("");
	csKernel.Format(_T("### Kernel %s %s %s ###"), getCONFIG()->GetKernelVersion(),getCONFIG()->GetKernelAcuVersion(),getCONFIG()->GetKernelVersionDate());
	theApp.WriteLog(csKernel);
	csKernel.Format(_T("### BoardSubType %s###"), getCONFIG()->GetBoardSubType());
	theApp.WriteLog(csKernel);
}
//void CMVModel::writeSerialBuiltInToLog()
//{
//	CTlsRegistry regSerial1(_T("HKLM\\Drivers\\BuiltIn\\Serial1"),true);
//	CStringW szSerial=_T("");
//	szSerial.Format(_T("### Serial1 %s"), regSerial1.ReadString(_T("Dll"), _T("unknown")));
//	theApp.WriteLog(szSerial);
//
//	CTlsRegistry regSerial2(_T("HKLM\\Drivers\\BuiltIn\\Serial2"),true);
//	szSerial.Format(_T("### Serial2 %s"), regSerial2.ReadString(_T("Dll"), _T("unknown")));
//	theApp.WriteLog(szSerial);
//
//	CTlsRegistry regSerial3(_T("HKLM\\Drivers\\BuiltIn\\Serial3"),true);
//	szSerial.Format(_T("### Serial3 %s"), regSerial3.ReadString(_T("Dll"), _T("unknown")));
//	theApp.WriteLog(szSerial);
//}

/**********************************************************************************************//**
 * Writes the mainboard version to log
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::writeMainboardVersionToLog()
{
	BYTE iMainVersion=getCONFIG()->GetMainBoardVersion();
	int iMajor=(iMainVersion >> 4) & 0x000F;
	int iMinor=iMainVersion & 0x000F;
	CString szMB=_T("");
	szMB.Format(_T("***MAINBOARD  %d.%d, "), iMajor,iMinor);
	theApp.WriteLog(szMB);
}

/**********************************************************************************************//**
 * Initializes the aculink
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVModel::initAcuLink()
{
	CStringW szLicenseFile=_T("ML");
	szLicenseFile+=GetUniqueID();
	szLicenseFile+=_T(".mlic");
	CStringW szFileAcuLink=_T("\\FFSDISK\\")+szLicenseFile;

	if(		CTlsFile::Exists(_T("\\FFSDISK\\AcuLink.exe"))
		&&	CTlsFile::Exists(_T("\\FFSDISK\\AcuLink_DLL.dll"))
		&&	CTlsFile::Exists(szFileAcuLink)
		&&  getCONFIG()->GetPDMSprotocol()!=ACL_NOPDMS
		&&  getCONFIG()->GetPDMSprotocol()!=ACL_TERMINAL_REMOTE
		&&  getCONFIG()->GetPDMSprotocol()!=ACL_TERMINAL_WAVE)
	{
		ACULINK=CInterfaceAcuLink::getInstance(getCONFIG()->GetPDMSprotocol());

		if(ACULINK->init())
		{
			SetAcuLinkAvailability(TRUE);
		}
		else
		{
			SetAcuLinkAvailability(FALSE);
		}
		return true;
	}
	else
	{
		theApp.WriteLog(_T("***NO ACULINK***"));
		SetAcuLinkAvailability(FALSE);

		if(false==CTlsFile::Exists(_T("\\FFSDISK\\AcuLink.exe")))
		{
			theApp.WriteLog(_T("***ACULINK=no EXE***"));
		}
		else if(false==CTlsFile::Exists(_T("\\FFSDISK\\AcuLink_DLL.dll")))
		{
			theApp.WriteLog(_T("***ACULINK=no DLL***"));
		}
		else if(false==CTlsFile::Exists(szFileAcuLink))
		{
			theApp.WriteLog(_T("***ACULINK=no license***"));
		}
		else if(getCONFIG()->GetPDMSprotocol()==ACL_NOPDMS)
		{
			theApp.WriteLog(_T("***ACULINK=ACL_NOPDMS***"));
		}
		else if(getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL_REMOTE)
		{
			theApp.WriteLog(_T("***ACULINK=ACL_TERMINAL_REMOTE***"));
		}
		else if(getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL_WAVE)
		{
			theApp.WriteLog(_T("***ACULINK=ACL_TERMINAL_WAVE***"));
		}

		return false;
	}
}

/**********************************************************************************************//**
 * Check unique identifier
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::checkUniqueID()
{
	bool bFalseID=false;
	CString szLeft=_T("");
	if(m_szUniqueID.GetLength()>=5)
		szLeft=m_szUniqueID.Left(5);

	if(m_szUniqueID==_T(""))
		bFalseID=true;
	else if(szLeft!=_T("00055"))
		bFalseID=true;

	if(bFalseID)
	{
		setNetworkAddressAsUniqueID();
	}
	else
	{
		/*CString szUniqueID=_T("");
		szUniqueID=m_szUniqueID;

		CString szDevID=_T("");
		szDevID.Format(_T("##DEVICE-ID: %s##"), szUniqueID);
		theApp.WriteLog(szDevID);*/

		writeUniqueIDtoLog();
	}
}

/**********************************************************************************************//**
 * Writes the uniqueid to log
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::writeUniqueIDtoLog()
{
	if(m_szUniqueID==_T(""))
	{
		theApp.WriteLog(_T("#HFO:0226"));
		m_szUniqueID=_T("-ERROR-");
	}
	else
	{
		CString szDevID=_T("");
		szDevID=m_szUniqueID;
		szDevID.Format(_T("##DEVICE-ID: %s##"), szDevID);
		theApp.WriteLog(szDevID);
	}
	
}

/**********************************************************************************************//**
 * Sets network address as unique identifier
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::setNetworkAddressAsUniqueID()
{
	theApp.WriteLog(_T("#HFO:0225"));
	CTlsRegistry regID(_T("HKLM\\Comm\\dm9ce1\\parms"),true);
	CString szUniqueID=regID.ReadString(_T("NetworkAddress"), _T(""));

	/*CString szLAN=_T("");
	szLAN.Format(_T("##NetworkAddress1 %s##"), szUniqueID);
	theApp.WriteLog(szLAN);*/

	if(szUniqueID!=_T(""))
	{
		int iLen=szUniqueID.GetLength();
		if(iLen<20)
		{
			int iPos=0;
			while(iPos!=-1)
			{
				iPos=szUniqueID.Find(_T('-'));
				if(iPos!=-1)
					szUniqueID.Delete(iPos,1);
			}
		}
		m_szUniqueID=szUniqueID;

		writeUniqueIDtoLog();
		/*szUniqueID=m_szUniqueID;
		szLAN=_T("");
		szLAN.Format(_T("##NetworkAddress2 %s##"), szUniqueID);
		theApp.WriteLog(szLAN);*/
	}

	/*if(m_szUniqueID==_T(""))
	{
		theApp.WriteLog(_T("#HFO:0226"));
		m_szUniqueID=_T("-ERROR-");
	}*/
}

/**********************************************************************************************//**
 * get the Viewhandler
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the viewhandler.
 **************************************************************************************************/

CMVViewHandler *CMVModel::getVIEWHANDLER()
{
	if(VIEWHANDLER==NULL)
	{
		//theApp.WriteLog(_T("#ERROR: VIEWHANDLER"));
		VIEWHANDLER=CMVViewHandler::GetInstance();
	}
	return VIEWHANDLER;
}

/**********************************************************************************************//**
 * Deinits this instance
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::Deinit()
{
	m_ObserverList.RemoveAll();

	if(PRICOTHR)
	{
		PRICOTHR->destroyInstance();
		PRICOTHR=NULL;
	}
	if(FOTTHR)
	{
		FOTTHR->destroyInstance();
		FOTTHR=NULL;
	}

	if(EXSPIRATIONTHR)
		EXSPIRATIONTHR->destroyInstance();
	EXSPIRATIONTHR=NULL;

	if(MONITORTHR)
		MONITORTHR->destroyInstance();
	MONITORTHR=NULL;

	if(ALARMTHR)
		ALARMTHR->destroyInstance();
	ALARMTHR=NULL;

	if(FLOWSENSORTHR)
		FLOWSENSORTHR->destroyInstance();
	FLOWSENSORTHR=NULL;

	if(VIEWHANDLER)
		VIEWHANDLER->DestroyInstance();
	VIEWHANDLER=NULL;

	EnterCriticalSection(&m_csETCO2);
	if(ETCO2)
	{
		ETCO2->Deinit();
		ETCO2->DestroyInstance();
	}
	ETCO2=NULL;
	LeaveCriticalSection(&m_csETCO2);
	
	EnterCriticalSection(&m_csSPO2);
	if(SPO2)
	{
		SPO2->Deinit(false);
		SPO2->DestroyInstance();
	}
	SPO2=NULL;
	LeaveCriticalSection(&m_csSPO2);
	
	EnterCriticalSection(&m_csSerial);
	if(SERIAL)
	{
		SERIAL->Deinit();
		SERIAL->DestroyInstance();
	}
	SERIAL=NULL;
	LeaveCriticalSection(&m_csSerial);
	
	if(SPI)
	{
		SPI->DestroyInstance();
	}
	SPI=NULL;

	if(PIF)
	{
		PIF->DestroyInstance();
	}
	PIF=NULL;

	if(DIO)
	{
		DIO->DestroyInstance();
	}
	DIO=NULL;

	if(I2C)
	{
		I2C->DestroyInstance();
	}
	I2C=NULL;

	if(SOUND)
	{
		SOUND->DestroyInstance();
	}
	SOUND=NULL;

	if(ACULINK)
	{
		ACULINK->destroyInstance();
	}
	ACULINK=NULL;

	if(TERMINAL)
	{
		TERMINAL->Deinit();
		TERMINAL->DestroyInstance();
	}
	TERMINAL=NULL;

	if(ALARMHANDLER)
	{
		ALARMHANDLER->deinit();
		ALARMHANDLER->destroyInstance();
	}
	ALARMHANDLER=NULL;

	if(MODEHANDLER)
		MODEHANDLER->destroyInstance();
	MODEHANDLER=NULL;

	if(DATAHANDLER)
	{
		try
		{
			DATAHANDLER->setExit();
		}
		catch (CException* e)
		{
			TCHAR   szCause[255];
			e->GetErrorMessage(szCause, 255);

			CString errorStr=_T("");
			errorStr.Format(_T("DATAHANDLER->setExit: %s"),szCause);

			theApp.ReportException(errorStr);

			e->Delete();
		}
		catch(...)
		{
			theApp.ReportException(_T("DATAHANDLER->setExit"));
		}
		
		DATAHANDLER->destroyInstance();
	}
	DATAHANDLER=NULL;

	if(CONFIG!=NULL)
	{
		try
		{
			CONFIG->DestroyInstance();
		}
		catch (CException* e)
		{
			TCHAR   szCause[255];
			e->GetErrorMessage(szCause, 255);

			CString errorStr=_T("");
			errorStr.Format(_T("CONFIG->DestroyInstance: %s"),szCause);

			theApp.ReportException(errorStr);

			e->Delete();
		}
		catch(...)
		{
			theApp.ReportException(_T("CONFIG->DestroyInstance"));
		}
	}
	CONFIG=NULL;
}

/**********************************************************************************************//**
 * Initializes the co2module
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::initCO2module()
{
	if(getCONFIG()->getCO2module()!=CO2MODULE_NONE)
	{
		ETCO2=CInterfaceCO2::GetInstance(getCONFIG()->getCO2module());

		if(ETCO2)
		{
			if(getCONFIG()->GetMainBoardVersion()>=MAINBOARD_30)
			{
				if(getCONFIG()->getCO2module()!=CO2MODULE_NONE)
				{
					if(ETCO2->Init(3)==0)
					{
						//ok
					}
					else
					{
						getDATAHANDLER()->setCOMErrorCode(ERRC_COM_CO2_INIT);
						ETCO2=NULL;
					}
				}
			}
			else
			{
				if(		getCONFIG()->getCO2module()!=CO2MODULE_NONE 
					&&  getCONFIG()->GetPDMSprotocol()!=ACL_SERIAL_ASCII
					&&  getCONFIG()->GetPDMSprotocol()!=ACL_SERIAL_WAVE
					&&	getCONFIG()->GetPDMSprotocol()!=ACL_SERIAL_IVOI
					&&	getCONFIG()->GetPDMSprotocol()!=ACL_TERMINAL_REMOTE
					&&	getCONFIG()->GetPDMSprotocol()!=ACL_TERMINAL_WAVE)
				{
					if(ETCO2->Init(1)==0)
					{
						//ok
					}
					else
					{
						getDATAHANDLER()->setCOMErrorCode(ERRC_COM_CO2_INIT);
						ETCO2=NULL;
					}

				}
				else if(	getCONFIG()->getCO2module()!=CO2MODULE_NONE 
					&&  (getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_ASCII
					||	getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_WAVE
					||	getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_IVOI
					||	getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL_REMOTE
					||	getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL_WAVE))
				{
					getCONFIG()->setCO2module(CO2MODULE_NONE);
					ETCO2=NULL;
				}
			}
		}
	}
	
	logCO2module();
}

/**********************************************************************************************//**
 * Logs co2module
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::logCO2module()
{
	switch(getCONFIG()->getCO2module())
	{
	case CO2MODULE_NONE:
		{
			theApp.WriteLog(_T("### CO2MODULE_NONE"));
		}
		break;
	case CO2MODULE_CAPNOSTAT:
		{
			theApp.WriteLog(_T("### CO2MODULE_CAPNOSTAT"));
		}
		break;
	case CO2MODULE_MICROPOD:
		{
			theApp.WriteLog(_T("### CO2MODULE_MICROPOD"));
		}
		break;
	}
}

/**********************************************************************************************//**
 * Deinit co2module
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::DeinitCO2module()
{
	if(ETCO2)
	{
		theApp.WriteLog(_T("#ETCO2 deinit"));
		ETCO2->Deinit();
		ETCO2->DestroyInstance();
		ETCO2=NULL;
	}

} 

/**********************************************************************************************//**
 * Sets co2inprogress
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CMVModel::setCO2inprogress(bool bState)
{
	m_bCO2inprogress=bState;

	if(bState)
	{
		DEBUGMSG(TRUE, (TEXT("setCO2inprogress true\r\n")));
	}
	else
	{
		DEBUGMSG(TRUE, (TEXT("setCO2inprogress false\r\n")));
	}
}

/**********************************************************************************************//**
 * Query if this instance is co2inprogress
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if co 2inprogress, false if not.
 **************************************************************************************************/

bool CMVModel::isCO2inprogress()
{
	return m_bCO2inprogress;
}

/**********************************************************************************************//**
 * Initializes the spo2module
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	bReinit	True to reinitialize.
 **************************************************************************************************/

void CMVModel::initSPO2module(bool bReinit)
{
	if(getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
	{
		if(SPO2==NULL)
			SPO2=CInterfaceSPO2::GetInstance(getCONFIG()->getSPO2module());

		if(SPO2)
		{
			if(SPO2->Init(9)==0)
			{
				m_bSPO2running=true;
			}
			else
			{
				SPO2=NULL;
				m_bSPO2running=false;
				getDATAHANDLER()->setCOMErrorCode(ERRC_USB_SPO2_INIT);
			}
		}
	}
	else
	{
		SPO2=NULL;
		m_bSPO2running=false;
	}
	if(!bReinit)
		logSPO2module();
}

/**********************************************************************************************//**
 * Logs spo2module
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::logSPO2module()
{
	switch(getCONFIG()->getSPO2module())
	{
	case SPO2MODULE_NONE:
		{
			theApp.WriteLog(_T("### SPO2MODULE_NONE"));
		}
		break;
	case SPO2MODULE_MASIMO:
		{
			theApp.WriteLog(_T("### SPO2MODULE_MASIMO"));
			DEBUGMSG(TRUE, (TEXT("### SPO2MODULE_MASIMO\r\n")));
		}
		break;
	}
}

/**********************************************************************************************//**
 * Deinit spo2module
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	bReinit	True to reinitialize.
 **************************************************************************************************/

void CMVModel::DeinitSPO2module(bool bReinit)
{
	m_bSPO2running=false;
	
	if(SPO2)
	{
		SPO2->Deinit(bReinit);
		SPO2->DestroyInstance();
		SPO2=NULL;
	}
}

/**********************************************************************************************//**
 * Query if this instance is spo2running
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if spo 2running, false if not.
 **************************************************************************************************/

bool CMVModel::isSPO2running()
{
	return m_bSPO2running;
}

/**********************************************************************************************//**
 * Gets language identifier
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	The language identifier.
 **************************************************************************************************/

WORD CMVModel::GetLanguageID()
{
	return m_wLanguageID;
}

/**********************************************************************************************//**
 * Sets language identifier
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	id	The identifier.
 **************************************************************************************************/

void CMVModel::SetLanguageID(WORD id)
{
	m_wLanguageID=id;
}

/**********************************************************************************************//**
 * Gets font face
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	The font face.
 **************************************************************************************************/

CStringW CMVModel::GetFontFace()
{
	return m_szFontName;
}

/**********************************************************************************************//**
 * Sets font face
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	name	The name.
 **************************************************************************************************/

void CMVModel::SetFontFace(CStringW name)
{
	m_szFontName=name;
}

/**********************************************************************************************//**
 * Query if this instance is o2cal flag
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if o 2cal flag, false if not.
 **************************************************************************************************/

bool CMVModel::IsO2calFlag()
{
	return m_bO2calFlag;
}

/**********************************************************************************************//**
 * Sets o2cal flag
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::SetO2calFlag()
{
	m_bO2calFlag=true;
}

/**********************************************************************************************//**
 * Deletes the o2cal flag
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::DeleteO2calFlag()
{
	if(		getALARMHANDLER()->ALARM_SysAl_P_IN_O2->getAlarmState()!=AS_ACTIVE
		&&	getALARMHANDLER()->ALARM_SysAl_P_IN_AIR->getAlarmState()!=AS_ACTIVE)
	{
		m_bO2calFlag=false;

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_DELAY_AUTOOXYCAL);
	}	
}

/**********************************************************************************************//**
 * Query if this instance is flowsensor flag
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if flowsensor flag, false if not.
 **************************************************************************************************/

bool CMVModel::IsFlowsensorFlag()
{
	return m_bFlowsensor;
}

/**********************************************************************************************//**
 * Deletes the flowsensor flag
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::DeleteFlowsensorFlag()
{
	m_bFlowsensor=false;
}

/**********************************************************************************************//**
 * Gets main version
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	The main version.
 **************************************************************************************************/

CStringW CMVModel::GetMainVersion()
{
	return m_szVersion;
}

/**********************************************************************************************//**
 * Gets build version
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	The build version.
 **************************************************************************************************/

CStringW CMVModel::GetBuildVersion()
{
	return m_szBuildVersion;
}

/**********************************************************************************************//**
 * Gets vent run state
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	The vent run state.
 **************************************************************************************************/

eRunState CMVModel::GetVentRunState()
{
	return m_eVentRunState;
}

/**********************************************************************************************//**
 * Sets vent run state
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CMVModel::SetVentRunState(eRunState state)
{
	m_eVentRunState=state;

	if(IsVideoRunning() && m_eVentRunState==VENT_RUNNING)
	{
		getVIEWHANDLER()->StopVideo();
	}

	bool bStopPRICO=false;
	
	switch(m_eVentRunState)
	{
	case VENT_STOPPED:
		{
			bStopPRICO=true;
			theApp.WriteLog(L"***VENT_STOPPED***");
			if(getCONFIG()->getCO2module()!=CO2MODULE_NONE && getETCO2()!=NULL)
			{
				getETCO2()->set_StandbyMode(true);
			}

			//turn off o2 flush
			if(true==isO2FlushActive())
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_STOP_O2FLUSH);
			}
		}
		break;
	case VENT_RUNNING:
		{
			theApp.WriteLog(L"***VENT_RUNNING***");
			if(getCONFIG()->getCO2module()!=CO2MODULE_NONE && getETCO2()!=NULL)
			{
				getETCO2()->set_StandbyMode(false);
			}
		}
		break;
	case VENT_STANDBY:
		{
			bStopPRICO=true;
			theApp.WriteLog(L"***VENT_STANDBY***");
			if(getCONFIG()->getCO2module()!=CO2MODULE_NONE && getETCO2()!=NULL)
			{
				getETCO2()->set_StandbyMode(true);
			}

			//turn off o2 flush
			if(true==isO2FlushActive())
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_STOP_O2FLUSH);
			}
		}
		break;
	}

	if(bStopPRICO)
	{
		bool bPRICOrunning=false;
		if(getPRICOThread())
		{
			if(getPRICOThread()->isPRICOalgorithmRunning())
			{
				bPRICOrunning=true;
			}
		}
		if(bPRICOrunning )
		{
			getDATAHANDLER()->setPRICOoff();
		}
	}

	if(getALARMHANDLER()->isPRICOAutoTurnedOff())//rku AUTOPRICO
	{
		getALARMHANDLER()->resetPRICOAutoTurnedOff();
	}
	

}

/**********************************************************************************************//**
 * Attach observer
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param [in,out]	pObserver	If non-null, the observer.
 **************************************************************************************************/

void CMVModel::AttachObserver(CModelObserver* pObserver)
{
	EnterCriticalSection(&m_csObservers);
	m_ObserverList.AddTail(pObserver);
	LeaveCriticalSection(&m_csObservers);
}

/**********************************************************************************************//**
 * Detach observer
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param [in,out]	pObserver	If non-null, the observer.
 **************************************************************************************************/

void CMVModel::DetachObserver(CModelObserver* pObserver)
{
	EnterCriticalSection(&m_csObservers);
	try
	{
		POSITION pos = m_ObserverList.Find(pObserver);
		if(pos)
			m_ObserverList.RemoveAt(pos);
	}
	catch (...)
	{
		theApp.ReportException(_T("CMVModel::DetachObserver"));
	}
	LeaveCriticalSection(&m_csObservers);
}

/**********************************************************************************************//**
 * Trigger information text event
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param [in,out]	pEvent	If non-null, the event.
 **************************************************************************************************/

void CMVModel::triggerInfoTextEvent(CMVEvent* pEvent)
{
	CMVEventInfotext* pUIEvent = (CMVEventInfotext*)pEvent;
	switch(pUIEvent->GetEventType())
	{
	case CMVEventInfotext::EV_CLEAR_TEXT:
	case CMVEventInfotext::EV_TEXT:
	case CMVEventInfotext::EV_TIMETEXT:
		{
			NotifyEvent(pEvent);
		}
		break;
	default:
		{

		}
		break;
	}
}

/**********************************************************************************************//**
 * Trigger control event
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param [in,out]	pEvent	If non-null, the event.
 **************************************************************************************************/

void CMVModel::triggerControlEvent(CMVEvent* pEvent)
{
	CMVEventControl* pCtrlEvent = (CMVEventControl*)pEvent;

	switch(pCtrlEvent->GetEventType())
	{
	case CMVEventControl::EV_CONTROL_SYSFAIL:
		{
			getVIEWHANDLER()->changeViewState(VS_SYSFAIL,VSS_NONE);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_ALARM3);
		}
		break;
	case CMVEventControl::EV_CONTROL_SYSTEMSTATE_CHANGED:
		{
			//check NCPAP/DUOPAP in case of PEDIATRIC -> switch to IPPV
			bool bVentStateChanged=false;
			if(getCONFIG()->GetVentRange()==PEDIATRIC)
			{
				if(getVMODEHANDLER()->activeModeIsNCPAP() || getVMODEHANDLER()->activeModeIsDUOPAP())
				{
					bVentStateChanged=true;

					if(false==getVMODEHANDLER()->changeVentMode(VM_IPPV,0))
						theApp.WriteLog(_T("#HFO:0299"));
				}
			}

			if(false==bVentStateChanged)
			{
				if(getCONFIG()->CurModeIsPresetMode())
					Send_VENT_MODE(getCONFIG()->GetPrevMode());
				else
					Send_VENT_MODE(getCONFIG()->GetCurMode());
			}
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventControl::EV_CONTROL_SET_EFLOWEQUIFLOW:
		{
			//getDATAHANDLER()->SetEFlowParadata_IPPV(getDATAHANDLER()->PARADATA()->GetIFlowPara_IPPV(),true);
			getDATAHANDLER()->SetEFlowParadata_TRIGGER(getDATAHANDLER()->PARADATA()->GetIFlowPara_TRIGGER(),true);
		}
		break;
	case CMVEventControl::EV_CONTROL_FLOWSENSOR_CALIBRATED:
		{
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventControl::EV_CONTROL_POWERSTATE_CHANGED:
		{
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventControl::EV_CONTROL_STARTUP_SUCCESS:
		{
			DEBUGMSG(TRUE, (TEXT("#########STARTUP\r\n")));
			//needs to be called here as first function!!!
			getALARMHANDLER()->ventModeChanged();

			getALARMHANDLER()->setStartupSilent(true);

			getSPI()->StartSPIMonitorThread();

			
			//check all Interfaces
#ifndef SIMULATION_NOSERIAL
			if(isSERIALavailable()==FALSE)
			{
				getALARMHANDLER()->setAlarm(AL_SysFail_IF_COM);
			}
			else if(getDATAHANDLER()->getCOMErrorCode()>0)//newVG
			{
				getALARMHANDLER()->setAlarm(AL_SysFail_IF_COM);

			}
#endif
#ifndef SIMULATION_NOSPI
			int iSPICode=getDATAHANDLER()->getSPIErrorCode();
			if(isSPIavailabel()==false)
			{
				getALARMHANDLER()->setAlarm(AL_SysFail_IF_SPI, _T("SPI: INVALID_HANDLE_VALUE"));
			}
			else if(iSPICode>0)
			{
				getALARMHANDLER()->setAlarm(AL_SysFail_IF_SPI);
			}
#endif
#ifndef SIMULATION_VERSION
			if(getI2C()->IsI2C_FRAMavailability()==false || getI2C()->IsI2C_MCPavailability()==false /*|| IsI2C_ACCUavailability()==false*/)
			{
				getALARMHANDLER()->setAlarm(AL_SysFail_IF_I2C);
			}
			if(IsDIOavailability()==false)
			{
				getALARMHANDLER()->setAlarm(AL_SysFail_IF_DIO);
			}
			if(IsPIFavailability()==false)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_IF_PIF);
			}
#endif
			
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_STARTUP_ACULINK);

			//test entrek
			if(		getCONFIG()->GetCurMode()==VM_NCPAP 
				||	getCONFIG()->GetCurMode()==VM_DUOPAP
				||	getCONFIG()->GetCurMode()==VM_THERAPIE)
			{
				getVIEWHANDLER()->changeViewState(VS_PARA,VSS_GRAPH_SINGLE_LINEGRAPHS);
				DeleteO2calFlag();
				//if(AfxGetApp())
				//	AfxGetApp()->GetMainWnd()->PostMessage(WM_DELAY_AUTOOXYCAL);
			}
			else
			{
				getVIEWHANDLER()->changeViewState(VS_SETUP,VSS_NONE);
			}

			/*if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_START_TRENDRECORD);*/
			
			if(getDATAHANDLER()->IsCurrentModeVGarantStateOn()==true)
			{
				getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_OFF);
			}
			getCONFIG()->DeaktivateVGarantState();	

			if(getDATAHANDLER()->IsCurrentModeVLimitStateOn()==true)
			{
				getDATAHANDLER()->SetCurrentVLimitParam(getDATAHANDLER()->GetCurrentVLimitPara(),false,true);
			}
			getCONFIG()->DeaktivateVLimitState();

			getDATAHANDLER()->SetFlowSensorState(getDATAHANDLER()->GetFlowSensorState());
			Send_VENT_MODE(getCONFIG()->GetCurMode());

			
			
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_START_TRENDRECORD);
			
			isMaintenanceNeeded();
			
			if(getCONFIG()->IsAutoScreenlockActive())
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
			}
#ifndef SIMULATION_VERSION
			if(AfxGetApp())
			{
				AfxGetApp()->GetMainWnd()->PostMessage(WM_START_AUTOOXYCAL);
				AfxGetApp()->GetMainWnd()->PostMessage(WM_CHECK_ALARMS);
				AfxGetApp()->GetMainWnd()->PostMessage(WM_GET_BLENDERVERSION);
			}
#endif
			
#ifdef SIMULATION_NOSPI//SIMULATION_NOSERIAL//SIMULATION_VERSION
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SYSALARM_SILENT);
#endif
		}
		break;
	case CMVEventControl::EV_CONTROL_COM_ERROR:
		{
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventControl::EV_CONTROL_START_FOT_CONVENTIONAL:
		{
			//DEBUGMSG(TRUE, (TEXT("EV_CONTROL_START_FOT_CONVENTIONAL\r\n")));
			if(getCONFIG()->CurModeIsPresetMode()==false)
				m_eVentModePrevFOT=getCONFIG()->GetCurMode();
			else
				m_eVentModePrevFOT=getCONFIG()->GetPrevMode();

			//DEBUGMSG(TRUE, (TEXT("EV_CONTROL_START_FOT_CONVENTIONAL 1\r\n")));
			if(m_eVentModePrevFOT!=VM_CPAP)
			{
				m_iBackupPrevFOT=getDATAHANDLER()->PARADATA()->GetBackupPara();
				//set CPAP backup para to off
				getDATAHANDLER()->SetBackupParadata(0,false);

				//DEBUGMSG(TRUE, (TEXT("EV_CONTROL_START_FOT_CONVENTIONAL 2\r\n")));
				theApp.WriteLog(_T("#FOT: CPAP"));
				if(false==getVMODEHANDLER()->changeVentMode(VM_CPAP,1,false))
					theApp.WriteLog(_T("#HFO:0228"));
			}
			else
			{
				m_iBackupPrevFOT=getDATAHANDLER()->PARADATA()->GetBackupPara();
				//set CPAP backup para to off
				getDATAHANDLER()->SetBackupParadata(0,true);
			}
			//DEBUGMSG(TRUE, (TEXT("EV_CONTROL_START_FOT_CONVENTIONAL 3\r\n")));
			getSPI()->resetPressureAlarmDelay();

			if(getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		break;
	case CMVEventControl::EV_CONTROL_RESTORE_FOT_CONVENTIONAL:
		{
			//DEBUGMSG(TRUE, (TEXT("EV_CONTROL_RESTORE_FOT_CONVENTIONAL\r\n")));
			if(m_eVentModePrevFOT!=VM_NONE)
			{
				if(m_eVentModePrevFOT!=VM_CPAP)
				{
					//set CPAP backup para to previous value
					getDATAHANDLER()->SetBackupParadata(m_iBackupPrevFOT,false);
				}
				else
				{
					//set CPAP backup para to previous value
					getDATAHANDLER()->SetBackupParadata(m_iBackupPrevFOT,true);
					Sleep(100);
				}

				//DEBUGMSG(TRUE, (TEXT("EV_CONTROL_RESTORE_FOT_CONVENTIONAL 1\r\n")));
				if(false==getVMODEHANDLER()->changeVentMode(m_eVentModePrevFOT,1,false))
					theApp.WriteLog(_T("#HFO:0228fot"));
				//DEBUGMSG(TRUE, (TEXT("EV_CONTROL_RESTORE_FOT_CONVENTIONAL 2\r\n")));

				getSPI()->resetPressureAlarmDelay();

				if(getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
				{
					getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
				}

				/*if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_AUTOALARMLIMITS);*/

				m_eVentModePrevFOT=VM_NONE;
			}
		}
		break;
	case CMVEventControl::EV_CONTROL_SET_MODE_EXHALVALVCAL:
		{
			getALARMHANDLER()->setAlarmSilent();
			getDATAHANDLER()->SetExhalValvCalMode(true);
		
			if(false==getVMODEHANDLER()->changeVentMode(VM_IPPV,0))
				theApp.WriteLog(_T("#HFO:0228"));

			getSPI()->resetPressureAlarmDelay();
			
			getALARMHANDLER()->resetVgVlAutoTurnedOff();
			getALARMHANDLER()->resetPRICOAutoTurnedOff();//rku AUTOPRICO

			if(getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}

			getVIEWHANDLER()->changeViewState(VS_PARA,VSS_GRAPH_SINGLE_LINEGRAPHS);

			NotifyEvent(pEvent);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_AUTOALARMLIMITS);
		}
		break;
	case CMVEventControl::EV_CONTROL_DEL_MODE_EXHALVALVCAL:
		{
			getDATAHANDLER()->SetExhalValvCalMode(false);
			NotifyEvent(pEvent);

			Send_MODE_OPTION1();

			if(getCONFIG()->IsEFLOWequalILFOW())
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SET_EFLOWEQUIFLOW);
			}
		}
		break;
	case CMVEventControl::EV_CONTROL_O2CALSTART:
	case CMVEventControl::EV_CONTROL_O2CALSTOP:
		{
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventControl::EV_CONTROL_O2SENSORSTATE:
		{
			if(getDATAHANDLER()->GetO2SensorState()==OXYSENS_OFF)
			{
				if(getALARMHANDLER()->ALARM_SysAl_P_IN_O2->getAlarmState()==AS_ACTIVE)
				{
					getALARMHANDLER()->deleteAlarm(AL_SysAl_P_IN_O2);
					if(IsO2calFlag())
						DeleteO2calFlag();
				}
				if(getALARMHANDLER()->ALARM_Sens_O2_SENSOR_DEFECT->getAlarmState()==AS_ACTIVE)
				{
					getALARMHANDLER()->deleteAlarm(AL_Sens_O2_SENSOR_DEFECT);
				}
				if(getALARMHANDLER()->ALARM_Sens_O2_SENSOR_USED->getAlarmState()==AS_ACTIVE)
				{
					getALARMHANDLER()->deleteAlarm(AL_Sens_O2_SENSOR_USED);
				}
				if(getALARMHANDLER()->ALARM_Sens_O2_VALUE_INCORRECT->getAlarmState()==AS_ACTIVE)
				{
					getALARMHANDLER()->deleteAlarm(AL_Sens_O2_VALUE_INCORRECT);
				}
				if(getALARMHANDLER()->ALARM_Sens_O2_NOT_CALIBRATED->getAlarmState()==AS_ACTIVE)
				{
					getALARMHANDLER()->deleteAlarm(AL_Sens_O2_NOT_CALIBRATED);
				}
				if(getALARMHANDLER()->ALARM_Sens_PRICO_FiO2outOfRange->getAlarmState()==AS_ACTIVE)
				{
					getALARMHANDLER()->deleteAlarm(AL_Sens_PRICO_FiO2outOfRange);
				}
			}
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventControl::EV_CONTROL_TRIGGERSTATE:
		{
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventControl::EV_CONTROL_FLOWSENSORSTATE:
		{
			if(getAcuLink()!=NULL)
			{
				if(		getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
					||	getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
					||	getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
				{
					getAcuLink()->setParaData(ALINK_SETT_STATE_FLOWSENSOR,0);
				}
				else
				{
					getAcuLink()->setParaData(ALINK_SETT_STATE_FLOWSENSOR,1);
				}
			}
			if(getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF)
			{
				theApp.WriteLog(_T("#FlowSensor turned off -> VG/VL turned off"));

				getDATAHANDLER()->PARADATA()->SetVGarantState_IPPV(false,false,true);
				getDATAHANDLER()->PRESET()->SetVGarantState_IPPV(false,false,false);

				getDATAHANDLER()->PARADATA()->SetVGarantState_TRIGGER(false,false,true);
				getDATAHANDLER()->PRESET()->SetVGarantState_TRIGGER(false,false,false);

				getDATAHANDLER()->PARADATA()->SetHFVGarantState(false,false,true);
				getDATAHANDLER()->PRESET()->SetHFVGarantState(false,false,false);

				if(getDATAHANDLER()->IsActiveModeVLimitStateOn())
				{
					getDATAHANDLER()->SetCurrentVLimitParam(getDATAHANDLER()->GetCurrentVLimitPara(),false,false);
				}
			}
			eVentMode activeMode=VM_NONE;
			if(getCONFIG()->CurModeIsPresetMode())
				activeMode=getCONFIG()->GetPrevMode();
			else
				activeMode=getCONFIG()->GetCurMode();
			switch(activeMode)
			{
			case VM_PSV:
				{
					if(getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF)
					{
						if(false==getVMODEHANDLER()->changeVentMode(VM_SIPPV))
							theApp.WriteLog(_T("#HFO:0230"));
						else
						{
							getDATAHANDLER()->checkTriggerTubeDependency();
							Send_VENT_MODE(VM_SIPPV);
						}
					}
				}
				break;
			case VM_SIMVPSV:
				{
					if(getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF)
					{
						if(false==getVMODEHANDLER()->changeVentMode(VM_SIMV))
							theApp.WriteLog(_T("#HFO:0230"));
						else
						{
							getDATAHANDLER()->checkTriggerTubeDependency();
							Send_VENT_MODE(VM_SIMV);
						}
					}
				}
				break;
			default:
				{
					Send_VENT_MODE(activeMode);
				}
				break;
			}

			if(		getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
				||	getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
				||	getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
			{
				if(getALARMHANDLER()->ALARM_Sens_FLOW_SENSOR_DEFECT->getAlarmState()==AS_ACTIVE)
				{
					getALARMHANDLER()->deleteAlarm(AL_Sens_FLOW_SENSOR_DEFECT);
				}
				if(getALARMHANDLER()->ALARM_Sens_FLOW_SENSOR_CLEANING->getAlarmState()==AS_ACTIVE)
				{
					getALARMHANDLER()->deleteAlarm(AL_Sens_FLOW_SENSOR_CLEANING);
				}
				if(getALARMHANDLER()->ALARM_Sens_FLOW_SENSOR_NOTCONNECTED->getAlarmState()==AS_ACTIVE)
				{
					getALARMHANDLER()->deleteAlarm(AL_Sens_FLOW_SENSOR_NOTCONNECTED);
				}
			}
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventControl::EV_CONTROL_STANDBY:
		{
			Send_MODE_OPTION1();
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventControl::EV_CONTROL_START_VENTILATION:
		{
			Send_MODE_OPTION1();
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventControl::EV_CONTROL_START_MANBREATH:
	case CMVEventControl::EV_CONTROL_STOP_MANBREATH:
		{
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventControl::EV_CONTROL_START_NEBULIZER:
		{
			
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventControl::EV_CONTROL_STOP_NEBULIZER:
		{
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventControl::EV_CONTROL_STOP_VENTILATION:
		{
			Send_MODE_OPTION1();
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventControl::EV_CONTROL_SETLASTVENTMODE:		//automatisch von PRESET zur�ck
		{
			eVentMode eMode = getCONFIG()->GetPrevMode();

			m_bAutoResetFromPreset = true;
			
			if(false==getVMODEHANDLER()->changeVentMode(eMode,0))
				theApp.WriteLog(_T("#HFO:0233"));

			m_bAutoResetFromPreset = false;
			
			//check for Flowsensor state
			switch(eMode)
			{
			case VM_IPPV:
			case VM_CPAP:
			case VM_HFO:
				{
					if(getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
					{
						getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
					}
				}
				break;
			case VM_DUOPAP:
			case VM_NCPAP:
				{
					if(getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF && getDATAHANDLER()->GetPrevFlowSensorState()==FLOWSENSOR_ON)
					{
						getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
					}
				}
				break;
			case VM_THERAPIE:
				{
					if(getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)
					{
						getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
					}
				}
			default:
				{
				}
				break;
			}
		}
		break;
	default:
		{
		}
		break;
	}
}

/**********************************************************************************************//**
 * Trigger ui event
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param [in,out]	pEvent	If non-null, the event.
 **************************************************************************************************/

void CMVModel::triggerUIevent(CMVEvent* pEvent)
{
	CMVEventUI* pUIEvent = (CMVEventUI*)pEvent;
			
	switch(pUIEvent->GetEventType())
	{
	case CMVEventUI::EV_BN_IPPV://++++++++++++++++++++++++++++++Menu Buttons+++++++++++++++++++++++++++++++++++++++++
		{
			getVMODEHANDLER()->setIPPV();
		}
		break;
	case CMVEventUI::EV_BN_SIPPV:
		{
			getVMODEHANDLER()->setSIPPV();
		}
		break;
	case CMVEventUI::EV_BN_SIMV:
		{
			getVMODEHANDLER()->setSIMV();
		}
		break;
	case CMVEventUI::EV_BN_SIMVPSV:
		{
			getVMODEHANDLER()->setSIMVPSV();
		}
		break;
	case CMVEventUI::EV_BN_PSV:
		{
			getVMODEHANDLER()->setPSV();
		}
		break;
	case CMVEventUI::EV_BN_HFO:
		{
			getVMODEHANDLER()->setHFO();
		}
		break;
	case CMVEventUI::EV_BN_CPAP:
		{
			getVMODEHANDLER()->setCPAP();
		}
		break;
	case CMVEventUI::EV_BN_NCPAP:
		{
			getVMODEHANDLER()->setNCPAP();
		}
		break;
	case CMVEventUI::EV_BN_THERAPIE:
		{
			getVMODEHANDLER()->setTHERAPIE();
		}
		break;
	case CMVEventUI::EV_BN_DUOPAP:
		{
			getVMODEHANDLER()->setDUOPAP();
		}
		break;
	case CMVEventUI::EV_RELOAD_CONFIG:
		{
			getCONFIG()->Init();
			getDATAHANDLER()->loadConfig();
			getALARMHANDLER()->loadConfig();

			getDATAHANDLER()->checkTriggerTubeDependency();

			Send_VENT_MODE(getCONFIG()->GetCurMode());
			
			NotifyEvent(pEvent);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_RELOADCONFIG);
		}
		break;
	case CMVEventUI::EV_LANGUAGE://++++++++++++++++++++++++++++++Change Language+++++++++++++++++++++++++++++++++++++++++
		{
			ChangeLanguage();
		}
		break;
	case CMVEventUI::EV_ALIMIT_BN_BPM_ON://++++++++++++++++++++++++++++++Alarmlimits On/Off+++++++++++++++++++++++++++++++++++++++++
		{
			getALARMHANDLER()->setAlimitState_BPMmaxLimit(AL_ON);
		}
		break;
	case CMVEventUI::EV_ALIMIT_BN_BPM_OFF:
		{
			getALARMHANDLER()->setAlimitState_BPMmaxLimit(AL_OFF);
		}
		break;
	case CMVEventUI::EV_ALIMIT_BN_LEAK_ON:
		{
			getALARMHANDLER()->setAlimitState_LeakmaxLimit(AL_ON);
		}
		break;
	case CMVEventUI::EV_ALIMIT_BN_LEAK_OFF:
		{
			getALARMHANDLER()->setAlimitState_LeakmaxLimit(AL_OFF);
		}
		break;
	case CMVEventUI::EV_ALIMIT_BN_APNOE_ON:
		{
			getALARMHANDLER()->setAlimitState_ApnoeLimit(AL_ON);
		}
		break;
	case CMVEventUI::EV_ALIMIT_BN_APNOE_OFF:
		{
			getALARMHANDLER()->setAlimitState_ApnoeLimit(AL_OFF);
		}
		break;
	case CMVEventUI::EV_PARABN_IFLOW:	//++++++++++++++++++++++++++++++Para-Buttons Value changed+++++++++++++++++++++++++++++++++++++++++
	case CMVEventUI::EV_PARABN_EFLOW:
	case CMVEventUI::EV_PARABN_VLIMIT:
	case CMVEventUI::EV_PARABN_VLIMIT_ENABLED:
	case CMVEventUI::EV_PARABN_VLIMIT_DISABLED:
	case CMVEventUI::EV_PARABN_TRIGGER:
	case CMVEventUI::EV_PARABN_BPM:
	case CMVEventUI::EV_PARABN_ITIME:
	case CMVEventUI::EV_PARABN_ETIME:
	case CMVEventUI::EV_PARABN_BACKUP:
	case CMVEventUI::EV_PARABN_O2:
	case CMVEventUI::EV_PARABN_MINFLOW:
	case CMVEventUI::EV_PARABN_VGARANT:
	case CMVEventUI::EV_PARABN_O2FLUSH:
		{
			NotifyParaBtnEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_PARABN_HFFREQ:
		{
			if(getDATAHANDLER()->isFOTLicenseAvailable()==true)
			{
				getFOTThread()->loadHFO_FOTvalues();
			}
			NotifyParaBtnEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_PARABN_VLIMIT_AUTO_ON:
		{
			NotifyParaBtnEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_PARABN_VLIMIT_AUTO_OFF:
		{
			fVALUE fv;

			if(getVMODEHANDLER()->activeModeIsIPPV())
			{
				fv.iValue=getDATAHANDLER()->PARADATA()->GetPINSPPara_IPPV();
				fv.iLowerLimit=getDATAHANDLER()->PARADATA()->GetPINSPMinPara_IPPV();
				fv.iUpperLimit=getDATAHANDLER()->PARADATA()->GetPINSPMaxPara_IPPV();
			}
			else
			{
				fv.iValue=getDATAHANDLER()->PARADATA()->GetPINSPPara_TRIGGER();
				fv.iLowerLimit=getDATAHANDLER()->PARADATA()->GetPINSPMinPara_TRIGGER();
				fv.iUpperLimit=getDATAHANDLER()->PARADATA()->GetPINSPMaxPara_TRIGGER();
			}

			int iPEEP=getDATAHANDLER()->GetCurrentPEEPPara();


			if(fv.iValue<iPEEP+20)
			{
				fv.iValue=iPEEP+20;
			}

			if(fv.iValue>fv.iUpperLimit)
			{
				fv.iValue=fv.iUpperLimit;
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				fv.iValue=fv.iLowerLimit;
			}
			if(getVMODEHANDLER()->activeModeIsIPPV())
				getDATAHANDLER()->SetPINSPParadata_IPPV(fv.iValue,true);
			else
				getDATAHANDLER()->SetPINSPParadata_TRIGGER(fv.iValue,true);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_PINSP);
			NotifyParaBtnEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_VOLUMEGARANT_ON:
		{
			NotifyParaBtnEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_VOLUMEGARANT_OFF:
		{
			if(getDATAHANDLER()->GetExhalValvCalMode()!=true)
			{
				NotifyParaBtnEvent(pEvent);
			}
		}
		break;
	case CMVEventUI::EV_VOLUMEGARANT_PRESET_ON:
	case CMVEventUI::EV_VOLUMEGARANT_PRESET_OFF:
	case CMVEventUI::EV_PARABN_VOLUMEGARANT_SELECTED_ON:
	case CMVEventUI::EV_PARABN_VOLUMEGARANT_SELECTED_OFF:
	case CMVEventUI::EV_PARABN_VOLUMEGARANT_PRESET_SELECTED_ON:
	case CMVEventUI::EV_PARABN_VOLUMEGARANT_PRESET_SELECTED_OFF:
	case CMVEventUI::EV_PARABN_VOLUMEGARANT_AUTO_OFF:
	case CMVEventUI::EV_PARABN_VOLUMEGARANT_AUTO_ON:
		{
			NotifyParaBtnEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_PARABN_CPAP:
		{
			if(getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			eVentMode mode=VM_NONE;
			if(getCONFIG()->CurModeIsPresetMode()==true)
			{
				mode=getCONFIG()->GetPrevMode();
			}
			else
			{
				mode=getCONFIG()->GetCurMode();
			}

			if(mode==VM_NCPAP)
			{
				getALARMHANDLER()->setAlimitsMinMaxRangeNCPAP();
			}
			else if(mode==VM_DUOPAP)
			{
				//getALARMHANDLER()->setAlimitsMinMaxRangeDUOPAP();
				getALARMHANDLER()->setAlimitsMinMaxRangePressure();//rku PIPLOW DUOPAP
			}
			else
			{
				getALARMHANDLER()->setAlimitsMinMaxRangeCpap();
			}
			NotifyParaBtnEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_PARABN_PEEP:
		{
			if(getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();

				//PostMessage(WM_ALIMIT_CHANGED);

			}

			//rku PMEANLIMITS ?????????????????
			getALARMHANDLER()->setAlimitsMinMaxRangePressure();
			NotifyParaBtnEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_PARABN_PMAN:
		{
			if(getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}
			//rku PMEANLIMITS ?????????????????
			getALARMHANDLER()->setAlimitsMinMaxRangePressure();
			NotifyParaBtnEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_PARABN_PMAXVOLG:
	case CMVEventUI::EV_PARABN_PINSP:
	case CMVEventUI::EV_PARABN_PPSV:
		{
			if(getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}
			if(getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
			{
				getDATAHANDLER()->calculateAutoAlarmlimitPIPLOW();
			}

			//rku PMEANLIMITS ?????????????????
			getALARMHANDLER()->setAlimitsMinMaxRangePressure();
			NotifyParaBtnEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_PARABN_PDUO:
		{
			if(getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			getALARMHANDLER()->setAlimitsMinMaxRangePressure();
			NotifyParaBtnEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_PARABN_IERATIO:
		{
			if(getALARMHANDLER()->getAlimitState_MAPminLimit()==AL_AUTO)
			{
				getDATAHANDLER()->calculateAutoAlarmlimitMAPmin();
			}
			if(getALARMHANDLER()->getAlimitState_MAPmaxLimit()==AL_AUTO)
			{
				getDATAHANDLER()->calculateAutoAlarmlimitMAPmax();
			}
			//rku PMEANLIMITS ?????????????????
			getALARMHANDLER()->setAlimitsMinMaxRangePressure();

			if(getDATAHANDLER()->isFOTLicenseAvailable()==true)
			{
				getFOTThread()->loadHFO_FOTvalues();
			}

			NotifyParaBtnEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_PARABN_HFFREQREC:
	case CMVEventUI::EV_PARABN_PMITT:
		{
			if(getALARMHANDLER()->getAlimitState_MAPminLimit()==AL_AUTO)
			{
				getDATAHANDLER()->calculateAutoAlarmlimitMAPmin();
			}
			if(getALARMHANDLER()->getAlimitState_MAPmaxLimit()==AL_AUTO)
			{
				getDATAHANDLER()->calculateAutoAlarmlimitMAPmax();
			}
			getALARMHANDLER()->setAlimitsMinMaxRangePressure();

			NotifyParaBtnEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_PARABN_HFAMPL:
		{
			if(getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(true==getDATAHANDLER()->IsActiveModeVGarantStateOn())
			{
				getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}
			else if(getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			if(getDATAHANDLER()->isFOTLicenseAvailable()==true)
			{
				getFOTThread()->loadHFO_FOTvalues();
			}

			//rku PMEANLIMITS ?????????????????
			getALARMHANDLER()->setAlimitsMinMaxRangePressure();
			NotifyParaBtnEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_PARABN_FOT:
		{
			if(getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(true==getDATAHANDLER()->IsActiveModeVGarantStateOn())
			{
				getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}
			else if(getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			//rku PMEANLIMITS ?????????????????
			getALARMHANDLER()->setAlimitsMinMaxRangePressure();
			NotifyParaBtnEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_BN_SETUP1://++++++++++++++++++++++++++++++Menu-Buttons SETUP+++++++++++++++++++++++++++++++++++++++++
	case CMVEventUI::EV_BN_SETUP2:
	case CMVEventUI::EV_BN_SETUP3:
	case CMVEventUI::EV_BN_SETUP4:
	case CMVEventUI::EV_BN_SETUP5:
		{
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_BN_TREND1://++++++++++++++++++++++++++++++Menu-Buttons TRENDS+++++++++++++++++++++++++++++++++++++++++
	case CMVEventUI::EV_BN_TREND2:
	case CMVEventUI::EV_BN_TREND3:
	case CMVEventUI::EV_BN_TREND4:
	case CMVEventUI::EV_BN_TREND5:
		{
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_TREND_SETCURSOR:
		{
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_TREND_DELCURSOR:
		{
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_BN_GRAPH_WAVE://++++++++++++++++++++++++++++++Menu-Buttons GRAPH+++++++++++++++++++++++++++++++++++++++++
		{
			if(getVIEWHANDLER()->getViewState()==VS_GRAPH && getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_MULTI_LINEGRAPHS)
			{
				NotifyEvent(pEvent);
			}
			else
			{
				getVIEWHANDLER()->changeViewState(VS_GRAPH,VSS_GRAPH_MULTI_LINEGRAPHS);
				getCONFIG()->GraphSetLastViewState(VSS_GRAPH_MULTI_LINEGRAPHS);
			}
		}
		break;
	case CMVEventUI::EV_BN_GRAPH_LOOP:
		{
			if(getVIEWHANDLER()->getViewState()==VS_GRAPH && getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
			{
				NotifyEvent(pEvent);
			}
			else
			{
				getVIEWHANDLER()->changeViewState(VS_GRAPH,VSS_GRAPH_LOOPGRAPHS);
				getCONFIG()->GraphSetLastViewState(VSS_GRAPH_LOOPGRAPHS);
			}
		}
		break;
	case CMVEventUI::EV_BN_GRAPH_CO2:
		{
			if(getVIEWHANDLER()->getViewState()==VS_GRAPH && getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
			{
				NotifyEvent(pEvent);
			}
			else
			{
				getVIEWHANDLER()->changeViewState(VS_GRAPH,VSS_GRAPH_CO2GRAPHS);
				getCONFIG()->GraphSetLastViewState(VSS_GRAPH_CO2GRAPHS);
			}
		}
		break;
	case CMVEventUI::EV_BN_GRAPH_SPO2:
		{
			if(getVIEWHANDLER()->getViewState()==VS_GRAPH && getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
			{
				NotifyEvent(pEvent);
			}
			else
			{
				getVIEWHANDLER()->changeViewState(VS_GRAPH,VSS_GRAPH_SPO2GRAPHS);
				getCONFIG()->GraphSetLastViewState(VSS_GRAPH_SPO2GRAPHS);
			}
		}
		break;
	case CMVEventUI::EV_BN_GRAPH_FOT:
		{
			if(getVIEWHANDLER()->getViewState()==VS_GRAPH && getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
			{
				NotifyEvent(pEvent);
			}
			else
			{
				getVIEWHANDLER()->changeViewState(VS_GRAPH,VSS_GRAPH_FOTGRAPHS);
				getCONFIG()->GraphSetLastViewState(VSS_GRAPH_FOTGRAPHS);
			}
		}
		break;
	/*case CMVEventUI::EV_BN_GRAPH_EMG:
		{
			if(getVIEWHANDLER()->getViewState()==VS_GRAPH && getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_EMGGRAPHS)
			{
				NotifyEvent(pEvent);
				break;
			}
			else
			{
				getVIEWHANDLER()->changeViewState(VS_GRAPH,VSS_GRAPH_EMGGRAPHS);
				getCONFIG()->GraphSetLastViewState(VSS_GRAPH_EMGGRAPHS);
			}
		}
		break;*/
	/*case CMVEventUI::EV_BN_GRAPH_CLFIO2:
		{
			if(GetViewState()==VS_GRAPH && GetViewSubState()==CMVModel::VSS_GRAPH_CLFIO2GRAPHS)
			{
				NotifyEvent(pEvent);
				break;
			}
			else
			{
				getVIEWHANDLER()->StopDiagramm();
				NotifyEvent(pEvent);
				ChangeViewState(VS_GRAPH,VSS_GRAPH_CLFIO2GRAPHS);
				getCONFIG()->GraphSetLastViewState(VSS_GRAPH_CLFIO2GRAPHS);
			}
		}
		break;*/
	case CMVEventUI::EV_BN_GRAPH_TREND:
		{
			ResetExhalValvCalMode();

			if(getVIEWHANDLER()->getViewState()==VS_TREND )
				NotifyEvent(pEvent);
			else
				getVIEWHANDLER()->changeViewState(VS_TREND,VSS_NONE);

			if(getDATAHANDLER()->IsGraphFreezed())
				getDATAHANDLER()->SetGraphFreezed(false);
		}
		break;
	/*case CMVEventUI::EV_BN_GRAPH_FREEZED:
	case CMVEventUI::EV_BN_GRAPH_CONTINUE:
		{
			NotifyEvent(pEvent);
		}
		break;*/
	case CMVEventUI::EV_BN_GRAPH_SAVE:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_BN_NEXT_GRAPH:
		{
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_GRAPH_SETCURSOR:
		{
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_GRAPH_DELCURSOR:
		{
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_BN_ALIMIT4:
		{
			getALARMHANDLER()->setLogTxt(_T("deleteSignaledAlarms"));

			getI2C()->DeleteI2CErrorCode();
			getDATAHANDLER()->deleteAllCOMError();//newVG
			getDATAHANDLER()->deleteSPIErrorCode();
			getALARMHANDLER()->deleteSignaledAlarms();
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_BN_ALIMIT5:
		{
			if(getVIEWHANDLER()->getViewSubState()!=VSS_ALIMIT_LOGLIST)
			{
				getVIEWHANDLER()->changeViewState(VS_ALARM_LIMIT,VSS_ALIMIT_LOGLIST);
			}
			else
			{
				//ChangeToNextViewState(VS_ALARM_LIMIT,VSS_NONE);
				getVIEWHANDLER()->changeViewState(VS_ALARM_LIMIT,VSS_NONE);
			}
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_BN_SCREENFREE:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);

			NotifyEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_BN_SCREENLOCK:
		{
			if(		getALARMHANDLER()->isActiveAlarm()==false
				||	getALARMHANDLER()->getAlarmSilentState()!=ASTATE_ACTIVE)
			{
				theApp.SetScreenlock(true);
				if(AfxGetApp())
				{
					AfxGetApp()->GetMainWnd()->PostMessage(WM_KILL_AUTOSCREENLOG_TIMER);
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
				}

				NotifyEvent(pEvent);
			}
		}
		break;
	case CMVEventUI::EV_BN_NUMERIC:
		{
			switch(getVIEWHANDLER()->getViewState())
			{
			case VS_SYSFAIL:
				{
					theApp.WriteLog(_T("$M30$"));
					if(getALARMHANDLER()->isActiveAlarmType(AT_SYSFAIL) && getALARMHANDLER()->getAlarmSilentState()==ASTATE_ACTIVE)
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETFOCUS_PIMARYVIEW);
					}
					else
					{
						getVIEWHANDLER()->changeToPrevViewState();

						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
					}
				}
				break;
			case VS_BATTERY:
				{
					theApp.WriteLog(_T("$M31$"));
					if(		(getALARMHANDLER()->ALARM_Accu_Empty->getAlarmState()==AS_ACTIVE
						||	getALARMHANDLER()->ALARM_Accu_Defect->getAlarmState()==AS_ACTIVE)
						 && getALARMHANDLER()->getAlarmSilentState()==ASTATE_ACTIVE)
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETFOCUS_PIMARYVIEW);
					}
					else
					{
						getALARMHANDLER()->deleteSignaledAlarms();

						theApp.WriteLog(_T("CMVEventUI::EV_BN_NUMERIC -> AL_Accu_Empty"));
						getVIEWHANDLER()->changeViewState(VS_PARA,VSS_GRAPH_SINGLE_LINEGRAPHS);

						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
					}
				}
				break;
			case VS_PATDATA:
				{
					theApp.WriteLog(_T("$M32$"));
					getVIEWHANDLER()->changeViewState(VS_PARA,VSS_GRAPH_SINGLE_LINEGRAPHS);
				}
				break;
			case VS_TREND:
				{
					theApp.WriteLog(_T("$M33$"));
					getVIEWHANDLER()->changeViewState(VS_GRAPH,(eViewSubState)getCONFIG()->GraphGetLastViewState());
				}
				break;
			case VS_ALARM_SYS:
				{
					theApp.WriteLog(_T("$M34$"));
					if(getALARMHANDLER()->isActiveAlarmType(AT_SYSALARM) && getALARMHANDLER()->getAlarmSilentState()==ASTATE_ACTIVE)
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_SETFOCUS_PIMARYVIEW);
					}
					else
					{
						getALARMHANDLER()->setLogTxt(_T("deleteSignaledAlarms VS_ALARM_SYS"));
						getALARMHANDLER()->deleteSignaledAlarms();
						getVIEWHANDLER()->changeToPrevViewState();

						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
					}
				}
				break;
			case VS_GRAPH:
				{
					theApp.WriteLog(_T("$M35$"));
				}
				break;
			case VS_PARA:
				{
					theApp.WriteLog(_T("$M36$"));
				}
				break;
			case VS_SETUP:
				{
					if(false==getReloadLanguageProgress())
					{
						theApp.WriteLog(_T("$M37$"));
						getVIEWHANDLER()->changeToPrevViewState();

						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
					}
				}
				break;
			case VS_ALARM_LIMIT:
				{
					theApp.WriteLog(_T("$M38$"));
					getVIEWHANDLER()->changeToPrevViewState();

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);
				}
				break;
			case VS_SERVICE:
				{
					theApp.WriteLog(_T("$M39$"));
					getDATAHANDLER()->SetVentilBurnRunning(false);

					getALARMHANDLER()->setAutoSilent(false,true,false);
					getVIEWHANDLER()->changeToPrevViewState();
				}
				break;
			default:
				{
				}
				break;
			}
		}
		break;
	case CMVEventUI::EV_BN_SET_SERVICE_MODE://++++++++++++++++++++++++++++++Service+++++++++++++++++++++++++++++++++++++++++
		{
			if(false==getVMODEHANDLER()->changeVentMode(VM_SERVICE,0))
				theApp.WriteLog(_T("#HFO:0235"));
			else
			{
				getALARMHANDLER()->setAlarmSilent();
				getALARMHANDLER()->deleteAllAlarms(false);

				NotifyEvent(pEvent);
			}
		}
		break;
	case CMVEventUI::EV_BN_DEL_SERVICE_MODE:
		{
			getVIEWHANDLER()->changeToPrevViewState();
		}
		break;
	case CMVEventUI::EV_BN_SERVICE_SYSTEM://++++++++++++++++++++++++++++++Menu-Buttons Service+++++++++++++++++++++++++++++++++++++++++
	case CMVEventUI::EV_BN_SERVICE_DISPLAY:
	case CMVEventUI::EV_BN_SERVICE_CALIBR:
	case CMVEventUI::EV_BN_SERVICE_FLOW:
	case CMVEventUI::EV_BN_SERVICE_ACCU:
	case CMVEventUI::EV_BN_SERVICE_VALVES:
	case CMVEventUI::EV_BN_SERVICE_MONITOR:
	case CMVEventUI::EV_BN_SERVICE_IF:
	case CMVEventUI::EV_BN_SERVICE_SETTINGS:
	case CMVEventUI::EV_BN_SERVICE_NETWORK:
	case CMVEventUI::EV_BN_SERVICE_LICENSING:
	case CMVEventUI::EV_BN_SERVICE_MAINTENANCE:
	case CMVEventUI::EV_BN_SERVICE_BACK:
		{
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_BN_MENUBUTTON_OPEN:
	case CMVEventUI::EV_BN_MENUBUTTON_CLOSE:
	case CMVEventUI::EV_BN_MENU_MOVELEFT:
	case CMVEventUI::EV_BN_MENU_MOVERIGHT:
		{
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventUI::EV_PATDATA_CHANGED:
		{
			NotifyEvent(pEvent);
		}
		break;
	default:
		{

		}
		break;
	}
}

/**********************************************************************************************//**
 * Trigger matrix event
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param [in,out]	pEvent	If non-null, the event.
 **************************************************************************************************/

void CMVModel::triggerMatrixEvent(CMVEvent* pEvent)
{
	if(getVIEWHANDLER()->getViewState()==VS_STARTUP || getVIEWHANDLER()->getViewState()==VS_PATDATA)
	{
		CMVEventMatrix* pUIEvent = (CMVEventMatrix*)pEvent;
		
		switch(pUIEvent->GetEventType())
		{
		case CMVEventMatrix::EV_BN_MATRIX_STARTSTOP_DOWN:
			{
				theApp.WriteLog(_T("#HFO:0236"));
			}
			break;
		case CMVEventMatrix::EV_BN_MATRIX_STARTSTOP_UP:
			{
				theApp.WriteLog(_T("#HFO:0237"));
		}
			break;
		default:
			break;
		}
	}

	CMVEventMatrix* pUIEvent = (CMVEventMatrix*)pEvent;
	switch(pUIEvent->GetEventType())
	{
	case CMVEventMatrix::EV_BN_MATRIX_SYSALARM_SILENT:
		{			
			ResetExhalValvCalMode();

			if(getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SYSTEM_SILENT)
			{
				getALARMHANDLER()->setSystemSilent();
			}
			else
			{
				if(getVIEWHANDLER()->getViewState()==VS_SERVICE)
				{
					getALARMHANDLER()->setAlarmSilent(true);
				}
				else
				{
					getALARMHANDLER()->setSystemSilentActive();
				}
			}
			NotifyEvent(pEvent);
		}
		break;
	case CMVEventMatrix::EV_BN_MATRIX_ALARM_SILENT:
		{
			bool bSendBtnPushed=false;
			bool bNursecall=false;
			ResetExhalValvCalMode();

			if(getDATAHANDLER()->isNurscallAlarm() 
				&& getCONFIG()->IsNursecallActive())
			{
				getDATAHANDLER()->SetNurscallAlarm(false);
				 bNursecall=true;
			}

			eAlarm alrm =getALARMHANDLER()->getActiveAlarm();

			if(		alrm==AL_Accu_POWER
				||	alrm==AL_Accu_60
				||	alrm==AL_Accu_30
				||	alrm==AL_Accu_15)
			{
				getALARMHANDLER()->deleteAlarm(alrm);
				NotifyEvent(pEvent);
			}
			else
			{
				if(GetVentRunState()==VENT_RUNNING && getCONFIG()->GetCurMode()!=VM_SERVICE)
				{
					if(getALARMHANDLER()->isSignaledAlarm() && getALARMHANDLER()->isActiveAlarm()==false)
					{
						
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_RESET_ALARMS);
					}
					else if(getALARMHANDLER()->getAlarmSilentState()==ASTATE_ACTIVE)
					{
						bSendBtnPushed=true;
						getALARMHANDLER()->setManualSilent();
						if(getALARMHANDLER()->ALARM_Sens_O2_NOT_CALIBRATED->getAlarmState()!=AS_NONE)
						{
							getALARMHANDLER()->deleteAlarm(AL_Sens_O2_NOT_CALIBRATED);
						}
						if(getALARMHANDLER()->ALARM_Sens_O2_SENSOR_USED->getAlarmState()!=AS_NONE)
						{
							getALARMHANDLER()->deleteAlarm(AL_Sens_O2_SENSOR_USED);
						}
						if(getALARMHANDLER()->ALARM_Sens_PRICO_FiO2outOfRange->getAlarmState()!=AS_NONE)
						{
							getALARMHANDLER()->deleteAlarm(AL_Sens_PRICO_FiO2outOfRange);
						}
						NotifyEvent(pEvent);
					}
					else if(	getALARMHANDLER()->getAlarmSilentState()!=ASTATE_ACTIVE
						&&	getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SILENT
						&&	getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SYSTEM_SILENT)
					{
						if(getALARMHANDLER()->getAlarmSilentState()==ASTATE_AUTOSILENT)
						{
							getALARMHANDLER()->setAlarmsilentActive();
							NotifyEvent(pEvent);
						}
						else if(getALARMHANDLER()->getAlarmSilentState()!=ASTATE_MANSILENT)
						{
							bSendBtnPushed=true;
							getALARMHANDLER()->setManualSilent();

							/*if(getALARMHANDLER()->getState_Nebulizer_Disconnection()!=AS_NONE)
							{
								getALARMHANDLER()->deleteAlarm(AL_Nebulizer_Disconnection);
							}
							if(getALARMHANDLER()->getState_Nebulizer_SysError()!=AS_NONE)
							{
								getALARMHANDLER()->deleteAlarm(AL_Nebulizer_SysError);
							}*/
							NotifyEvent(pEvent);
						}
						else
						{
							getALARMHANDLER()->setAlarmsilentActive();
							NotifyEvent(pEvent);
						}
					}
				}
				else if(bNursecall)
				{
					NotifyEvent(pEvent);
				}
			}
			//if(bSendBtnPushed)
			{
				getSPI()->Send_ALARMKEY_PUSH();
			}
		}
		break;
	case CMVEventMatrix::EV_BN_MATRIX_STARTSTOP_DOWN:
		{
			ResetExhalValvCalMode();

			if(AfxGetApp()->GetMainWnd())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VENT_STARTSTOP_DOWN);
		}
		break;
	case CMVEventMatrix::EV_BN_MATRIX_STARTSTOP_UP:
		{
			ResetExhalValvCalMode();

			if(		getALARMHANDLER()->isActiveAlarm()==false
				||	getALARMHANDLER()->getAlarmSilentState()!=ASTATE_ACTIVE
				||	getALARMHANDLER()->getAlarmType(getALARMHANDLER()->getActiveAlarm())>AT_CO2)
			{
				if(getVIEWHANDLER()->getViewState()==VS_SERVICE)
				{
					getVIEWHANDLER()->changeToPrevViewState();
				}

				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_VENT_STARTSTOP_UP);
			}
		}
		break;
	case CMVEventMatrix::EV_BN_MATRIX_ONOFF_DOWN:
		{
			ResetExhalValvCalMode();

			//TREND
			if(getDATAHANDLER()->isTRENDLicenseAvailable())
			{
				//rkuNEWFIX
				//EnterCriticalSection(&GetDataHandler()->csTrend);
				getDATAHANDLER()->SerializeAllTrends(false);
				//LeaveCriticalSection(&GetDataHandler()->csTrend);
			}

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VENT_OFF_DOWN);
		}
		break;
	case CMVEventMatrix::EV_BN_MATRIX_ONOFF_UP:
		{
			ResetExhalValvCalMode();

			if(getVIEWHANDLER()->getViewState()==VS_SERVICE)
			{
				getVIEWHANDLER()->changeToPrevViewState();
			}

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VENT_OFF_UP);
		}
		break;
	case CMVEventMatrix::EV_BN_MATRIX_HOME:
		{
			if(getVIEWHANDLER()->canChangeViewStateAlarmDependend())
			{
				if(!ResetExhalValvCalMode())
				{
					switch(getVIEWHANDLER()->getViewState())
					{
					case VS_SERVICE:
						{
							getALARMHANDLER()->setAutoSilent(false,true,false);
							getVIEWHANDLER()->changeViewState(VS_PARA,VSS_GRAPH_SINGLE_LINEGRAPHS);
						}
						break;
					case VS_GRAPH:
						{
							getVIEWHANDLER()->changeViewState(VS_PARA,VSS_GRAPH_SINGLE_LINEGRAPHS,true);
						}
						break;
					default:
						{
							getVIEWHANDLER()->changeViewState(VS_PARA,VSS_GRAPH_SINGLE_LINEGRAPHS);
						}
						break;
					}
				}
			}
		}
		break;
	case CMVEventMatrix::EV_BN_MATRIX_MENU:
		{
			ResetExhalValvCalMode();

			if(getVIEWHANDLER()->canChangeViewStateAlarmDependend())
			{
				if(getVIEWHANDLER()->getViewState()==VS_SERVICE)
				{
					getALARMHANDLER()->setAutoSilent(false,true,false);
				}

				getVIEWHANDLER()->changeViewState(VS_SETUP,VSS_NONE);

				if(	getALARMHANDLER()->ALARM_SysLimit_Pinsp_NotReached->getAlarmState()!=AS_NONE)
				{
					getALARMHANDLER()->deleteAlarm(AL_SysLimit_Pinsp_NotReached);
				}
				if(	getALARMHANDLER()->ALARM_SysLimit_Vlimitted->getAlarmState()!=AS_NONE)
				{
					getALARMHANDLER()->deleteAlarm(AL_SysLimit_Vlimitted);
				}
				if(	getALARMHANDLER()->ALARM_SysLimit_Vgaranty->getAlarmState()!=AS_NONE)
				{
					getALARMHANDLER()->deleteAlarm(AL_SysLimit_Vgaranty);
				}
			}
		}
		break;
	case CMVEventMatrix::EV_BN_MATRIX_SERVICE:
		{
			ResetExhalValvCalMode();

			if(getVIEWHANDLER()->canChangeViewStateAlarmDependend())
			{
				getVIEWHANDLER()->changeViewState(VS_SERVICE,VSS_NONE);
				if(AfxGetMainWnd())
					AfxGetMainWnd()->PostMessage(WM_SET_SERVICEMODE);//ruft EV_BN_SET_SERVICE_MODE auf -> wechselt den VentMode nach VM_SERVICE
			}
		}
		break;
	case CMVEventMatrix::EV_BN_MATRIX_ALARMLIMITS:
		{
			if(getVIEWHANDLER()->getViewState()==VS_SERVICE)
			{
				getALARMHANDLER()->setAutoSilent(false,true,false);
			}

			ResetExhalValvCalMode();

			if(getVIEWHANDLER()->canChangeViewStateAlarmDependend())
			{
				/*if(getVIEWHANDLER()->getViewState()==VS_GRAPH)
					getVIEWHANDLER()->StopDiagramm();*/

				getVIEWHANDLER()->changeViewState(VS_ALARM_LIMIT,VSS_NONE);
			}
		}
		break;
	case CMVEventMatrix::EV_BN_MATRIX_TREND:
		{
			ResetExhalValvCalMode();
			
			if(		getALARMHANDLER()->isActiveAlarm()==false
				||	getALARMHANDLER()->getAlarmSilentState()!=ASTATE_ACTIVE)
			{
				if(getVIEWHANDLER()->getViewState()==VS_TREND )
					NotifyEvent(pEvent);
				else
					getVIEWHANDLER()->changeViewState(VS_TREND,VSS_NONE);
			}
		}
		break;
	case CMVEventMatrix::EV_BN_MATRIX_GRAPHS:
		{
			if(getVIEWHANDLER()->getViewState()==VS_SERVICE)
			{
				getALARMHANDLER()->setAutoSilent(false,true,false);
			}

			ResetExhalValvCalMode();

			if(getVIEWHANDLER()->getViewState()==VS_GRAPH && getVIEWHANDLER()->getViewSubState()==(eViewSubState)getCONFIG()->GraphGetLastViewState())
				NotifyEvent(pEvent);
			else
			{
				//getVIEWHANDLER()->StopDiagramm();

				if((getCONFIG()->getTempCO2module()==CO2MODULE_NONE)
					&& (eViewSubState)getCONFIG()->GraphGetLastViewState()==VSS_GRAPH_CO2GRAPHS)
				{
					getVIEWHANDLER()->changeViewState(VS_GRAPH,VSS_GRAPH_MULTI_LINEGRAPHS);
					getCONFIG()->GraphSetLastViewState(VSS_GRAPH_MULTI_LINEGRAPHS);
				}
				else if((getCONFIG()->getTempSPO2module()==SPO2MODULE_NONE)
					&& (eViewSubState)getCONFIG()->GraphGetLastViewState()==VSS_GRAPH_SPO2GRAPHS)
				{
					getVIEWHANDLER()->changeViewState(VS_GRAPH,VSS_GRAPH_MULTI_LINEGRAPHS);
					getCONFIG()->GraphSetLastViewState(VSS_GRAPH_MULTI_LINEGRAPHS);
				}
				else if((getDATAHANDLER()->isFOTLicenseAvailable()==false)
					&& (eViewSubState)getCONFIG()->GraphGetLastViewState()==VSS_GRAPH_FOTGRAPHS)
				{
					getVIEWHANDLER()->changeViewState(VS_GRAPH,VSS_GRAPH_MULTI_LINEGRAPHS);
					getCONFIG()->GraphSetLastViewState(VSS_GRAPH_MULTI_LINEGRAPHS);
				}
				else if((getVMODEHANDLER()->activeModeIsNMODE())
					&& (eViewSubState)getCONFIG()->GraphGetLastViewState()==VSS_GRAPH_FOTGRAPHS)
				{
					getVIEWHANDLER()->changeViewState(VS_GRAPH,VSS_GRAPH_MULTI_LINEGRAPHS);
					getCONFIG()->GraphSetLastViewState(VSS_GRAPH_MULTI_LINEGRAPHS);
				}
				else if(	(	getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
							||	getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
							||	getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
					&& (eViewSubState)getCONFIG()->GraphGetLastViewState()==VSS_GRAPH_FOTGRAPHS)
				{
					getVIEWHANDLER()->changeViewState(VS_GRAPH,VSS_GRAPH_MULTI_LINEGRAPHS);
					getCONFIG()->GraphSetLastViewState(VSS_GRAPH_MULTI_LINEGRAPHS);
				}
				else 
					getVIEWHANDLER()->changeViewState(VS_GRAPH,(eViewSubState)getCONFIG()->GraphGetLastViewState());
			}
		}
		break;
	case CMVEventMatrix::EV_BN_MATRIX_CURSOR_UP:
		{
			ResetExhalValvCalMode();

			if(		getVIEWHANDLER()->getViewState()==VS_GRAPH
				||	getVIEWHANDLER()->getViewState()==VS_TREND
				||	getVIEWHANDLER()->getViewState()==VS_PARA)
				NotifyEvent(pEvent);
		}
		break;
	case CMVEventMatrix::EV_BN_MATRIX_CURSOR_DOWN:
		{
			ResetExhalValvCalMode();

			if(		getVIEWHANDLER()->getViewState()==VS_GRAPH
				||	getVIEWHANDLER()->getViewState()==VS_TREND
				||	getVIEWHANDLER()->getViewState()==VS_PARA)
				NotifyEvent(pEvent);
		}
		break;
	case CMVEventMatrix::EV_BN_MATRIX_CURSOR_LEFT:
		{
			ResetExhalValvCalMode();

			if(getCONFIG()->GraphIsAutoScale()==false)
			{
				NotifyEvent(pEvent);
			}
			else
			{
				getDATAHANDLER()->DecreaseContrast();
			}
		}
		break;
	case CMVEventMatrix::EV_BN_MATRIX_CURSOR_RIGHT:
		{
			ResetExhalValvCalMode();

			if(getCONFIG()->GraphIsAutoScale()==false)
			{
				NotifyEvent(pEvent);
			}
			else
			{
				getDATAHANDLER()->IncreaseContrast();
			}
		}
		break;
	case CMVEventMatrix::EV_BN_MATRIX_CURSOR_MOVEXUP:
	case CMVEventMatrix::EV_BN_MATRIX_CURSOR_MOVEXDOWN:
	case CMVEventMatrix::EV_BN_MATRIX_CURSOR_MOVEYLEFT:
	case CMVEventMatrix::EV_BN_MATRIX_CURSOR_MOVEYRIGHT:
		{
			ResetExhalValvCalMode();

			if(getVIEWHANDLER()->getViewState()!=VS_SERVICE)
			{
				if(		getVIEWHANDLER()->getViewState()==VS_GRAPH
					||	getVIEWHANDLER()->getViewState()==VS_PARA)
					NotifyEvent(pEvent);
			}
		}
		break;
	default:
		{

		}
		break;
	}
}

/**********************************************************************************************//**
 * Trigger alarm event
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param [in,out]	pEvent	If non-null, the event.
 **************************************************************************************************/

void CMVModel::triggerAlarmEvent(CMVEvent* pEvent)
{
	CMVEventAlarm* pUIEvent = (CMVEventAlarm*)pEvent;
	switch(pUIEvent->GetEventType())
	{
	case CMVEventAlarm::EV_ALARM_REACTIVATED:
		{
			NotifyEvent(pEvent);
			getDATAHANDLER()->checkLimits();
		}
		break;
	case CMVEventAlarm::EV_ALARMSTATE_CHANGED:
		{
			eVentSilentState silentState = getALARMHANDLER()->getAlarmSilentState();

			if(getAcuLink()!=NULL)
			{
				if(silentState==ASTATE_ACTIVE)
				{
					getAcuLink()->setAlarmData(ALINK_ALARM_ENABLED,1);
				}
				else
				{
					getAcuLink()->setAlarmData(ALINK_ALARM_ENABLED,0);
				}
			}

			if(silentState!=ASTATE_SYSTEM_SILENT)
			{
				eAlarm curAlarm=getALARMHANDLER()->getActiveAlarm();

				if(curAlarm!=AL_NONE)
				{
					if(theApp.IsScreenlock())
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_KILL_AUTOSCREENLOG_TIMER);
						theApp.SetScreenlock(false);
					}

					eAlarmType type = getALARMHANDLER()->getAlarmType(curAlarm);

					if(		type==AT_SYSFAIL
						&&	getVIEWHANDLER()->getViewState()!=VS_SYSFAIL)
					{
						if(silentState!=ASTATE_MANSILENT)
						{
							//getVIEWHANDLER()->StopDiagramm();

							getVIEWHANDLER()->changeViewState(VS_SYSFAIL,VSS_NONE);
						}
					}
					else if(	getALARMHANDLER()->ALARM_Accu_Empty->getAlarmState()==AS_ACTIVE
						&&	getVIEWHANDLER()->getViewState()!=VS_BATTERY)
					{
						if(		silentState!=ASTATE_MANSILENT
							&&	silentState!=ASTATE_SYSTEM_SILENT
							&&	silentState!=ASTATE_SILENT)
						{
							//getVIEWHANDLER()->StopDiagramm();

							theApp.WriteLog(_T("CMVEventAlarm::EV_ALARMSTATE_CHANGED -> AL_Accu_Empty"));
							getVIEWHANDLER()->changeViewState(VS_BATTERY,VSS_NONE);
						}
					}
					else if(	getALARMHANDLER()->ALARM_Accu_Defect->getAlarmState()==AS_ACTIVE
						&&	getVIEWHANDLER()->getViewState()!=VS_BATTERY)
					{
						if(		silentState!=ASTATE_MANSILENT
							&&	silentState!=ASTATE_SYSTEM_SILENT
							&&	silentState!=ASTATE_SILENT)
						{
							//getVIEWHANDLER()->StopDiagramm();

							theApp.WriteLog(_T("CMVEventAlarm::EV_ALARMSTATE_CHANGED -> AL_Accu_Defect"));
							getVIEWHANDLER()->changeViewState(VS_BATTERY,VSS_NONE);
						}
					}
					else if(	type==AT_SYSALARM 
						&&	VS_ALARM_SYS!=getVIEWHANDLER()->getViewState())
					{
						if(	silentState!=ASTATE_MANSILENT)
						{
							if(curAlarm==AL_SysAl_TUBE_OCCLUSION)
							{
								if(silentState==ASTATE_ACTIVE)
								{
									//getVIEWHANDLER()->StopDiagramm();
									getVIEWHANDLER()->changeViewState(VS_ALARM_SYS,VSS_NONE);
								}
							}
							else
							{
								//getVIEWHANDLER()->StopDiagramm();
								getVIEWHANDLER()->changeViewState(VS_ALARM_SYS,VSS_NONE);
							}
						}
					}
				}
				else if(getCONFIG()->IsAutoScreenlockActive())
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_SET_AUTOSCREENLOG_TIMER);
				}

				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_CHECK_ALARMS);
			}
			NotifyEvent(pEvent);
		}
		break;
	default:
		{

		}
		break;
	}
}

/**********************************************************************************************//**
 * Trigger data event
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param [in,out]	pEvent	If non-null, the event.
 **************************************************************************************************/

void CMVModel::triggerDataEvent(CMVEvent* pEvent)
{
	CStringW eventData = _T("");
	CMVEventData* pUIEvent = (CMVEventData*)pEvent;

	switch(pUIEvent->GetEventType())
	{
	case CMVEventData::EV_DATA_ALARMLIMITS_CHANGED:
		{
			NotifyNewAlarmlimitData();
		}
		break;
	case CMVEventData::EV_DATA_CALCULATE_ALARMLIMITS:
		{
			m_bCalculateAlarmlimitRunning=true;
			NotifyCalculateAlarmlimitData();
		}
		break;
	default:
		{
		}
		break;
	}
}

/**********************************************************************************************//**
 * Trigger event
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param [in,out]	pEvent  	If non-null, the event.
 * \param 		  	btnID   	Identifier for the button.
 * \param 		  	iGroupId	Identifier for the group.
 **************************************************************************************************/

void CMVModel::triggerEvent(CMVEvent* pEvent, int btnID, int iGroupId)
{
	EnterCriticalSection(&m_csTrigger);

	switch(pEvent->GetET())
	{
	case CMVEvent::ET_INFOTEXT:
		{
			triggerInfoTextEvent((CMVEventInfotext*)pEvent);
		}
		break;
	case CMVEvent::ET_CONTROL:
		{
			triggerControlEvent((CMVEventInfotext*)pEvent);
		}
		break;
	case CMVEvent::ET_UIEVENT:
		{
			triggerUIevent((CMVEventUI*)pEvent);
		}
		break;
	case CMVEvent::ET_MATRIX://++++++++++++++++++++++++++++++Buttons MATRIX+++++++++++++++++++++++++++++++++++++++++
		{
			triggerMatrixEvent((CMVEventMatrix*)pEvent);
		}
		break;
	case CMVEvent::ET_ALARM://++++++++++++++++++++++++++++++System-AlarmeP+++++++++++++++++++++++++++++++++++++++++
		{
			triggerAlarmEvent((CMVEventAlarm*)pEvent);
		}
		break;
	case CMVEvent::ET_DATAEVENT://++++++++++++++++++++++++++++++Daten Events+++++++++++++++++++++++++++++++++++++++++
		{
			triggerDataEvent((CMVEventData*)pEvent);
		}
		break;
	}

	LeaveCriticalSection(&m_csTrigger);
	
}

/**********************************************************************************************//**
 * Httoi the given value
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	value	The value.
 *
 * \return	An int.
 **************************************************************************************************/

int CMVModel::_httoi(const TCHAR *value)
{
	struct CHexMap
	{
		TCHAR chr;
		int value;
	};
	const int HexMapL = 16;
	CHexMap HexMap[HexMapL] =
	{
		{'0', 0}, {'1', 1},
		{'2', 2}, {'3', 3},
		{'4', 4}, {'5', 5},
		{'6', 6}, {'7', 7},
		{'8', 8}, {'9', 9},
		{'A', 10}, {'B', 11},
		{'C', 12}, {'D', 13},
		{'E', 14}, {'F', 15}
	};
	TCHAR *mstr = _tcsupr(_tcsdup(value));
	TCHAR *s = mstr;
	int result = 0;
	if (*s == '0' && *(s + 1) == 'X') s += 2;
	bool firsttime = true;
	while (*s != '\0')
	{
		bool found = false;
		for (int i = 0; i < HexMapL; i++)
		{
			if (*s == HexMap[i].chr)
			{
				if (!firsttime) result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found) break;
		s++;
		firsttime = false;
	}
	free(mstr);
	return result;
}

/**********************************************************************************************//**
 * Sets alimit state calculating
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CMVModel::SetAlimitStateCalculating(bool state)
{
	m_bCalculateAlarmlimitRunning=true;
}

/**********************************************************************************************//**
 * Calculates the alarmlimit running
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVModel::CalculateAlarmlimitRunning()
{
	return m_bCalculateAlarmlimitRunning;
}

/**********************************************************************************************//**
 * Change language
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::ChangeLanguage()
{
	EnterCriticalSection(&m_csObservers);

	POSITION		Pos;
	CModelObserver	* pObserver = NULL;

	// Step 1 - Notify all the observers to update themselves
	for( Pos = m_ObserverList.GetHeadPosition(); Pos != NULL; )
	{
		pObserver = ( CModelObserver * ) m_ObserverList.GetNext( Pos );
		if(pObserver)
			pObserver->NotifyLanguageChanged();
		Sleep(0);
	}

	LeaveCriticalSection(&m_csObservers);
}

/**********************************************************************************************//**
 * Notifies the view state changed
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::notifyViewStateChanged()
{
	EnterCriticalSection(&m_csObservers);

	try
	{
		POSITION		Pos;
		CModelObserver	* pObserver = NULL;

		for( Pos = m_ObserverList.GetHeadPosition(); Pos != NULL; )
		{
			pObserver = ( CModelObserver * ) m_ObserverList.GetNext( Pos );
			if(pObserver)
				pObserver->NotifyViewStateChanged();
			Sleep(0);
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CMVModel::ChangeViewState"));
	}

	LeaveCriticalSection(&m_csObservers);
}

/**********************************************************************************************//**
 * Notifies the vent mode changed
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::notifyVentModeChanged()
{
	EnterCriticalSection(&m_csObservers);

	POSITION		Pos;
	CModelObserver	* pObserver = NULL;

	try
	{
		for( Pos = m_ObserverList.GetHeadPosition(); Pos != NULL; )
		{
			pObserver = ( CModelObserver * ) m_ObserverList.GetNext( Pos );
			if(pObserver)
				pObserver->NotifyVentModeChanged();
			Sleep(0);
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CMVModel::VentModeChanged"));
	}
	
	LeaveCriticalSection(&m_csObservers);
}

/**********************************************************************************************//**
 * Notifies an event
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param [in,out]	pEvent	If non-null, the event.
 **************************************************************************************************/

void CMVModel::NotifyEvent(CMVEvent* pEvent)
{
	try
	{
		EnterCriticalSection(&m_csObservers);

		POSITION		Pos;
		CModelObserver	* pObserver = NULL;

		for( Pos = m_ObserverList.GetHeadPosition(); Pos != NULL; )
		{
			pObserver = ( CModelObserver * ) m_ObserverList.GetNext( Pos );
			if(pObserver)
				pObserver->NotifyEvent(pEvent);
			Sleep(0);
		}

		LeaveCriticalSection(&m_csObservers);
	}
	catch (...)
	{
		LeaveCriticalSection(&m_csObservers);

		theApp.ReportException(_T("CMVModel::NotifyEvent"));
	}
	
}

/**********************************************************************************************//**
 * Notifies a para button event
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param [in,out]	pEvent	If non-null, the event.
 **************************************************************************************************/

void CMVModel::NotifyParaBtnEvent(CMVEvent* pEvent)
{
	EnterCriticalSection(&m_csObservers);
	try
	{
		POSITION		Pos;
		CModelObserver	* pObserver = NULL;

		for( Pos = m_ObserverList.GetHeadPosition(); Pos != NULL; )
		{
			pObserver = ( CModelObserver * ) m_ObserverList.GetNext( Pos );
			if(pObserver)
				pObserver->NotifyParaBtnEvent(pEvent);
			Sleep(0);
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CMVModel::NotifyParaBtnEvent"));
	}
	LeaveCriticalSection(&m_csObservers);

}

/**********************************************************************************************//**
 * Notifies the exspiration start
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::NotifyExspirationStart()
{
	try
	{
		EnterCriticalSection(&m_csObservers);

		POSITION		Pos;
		CModelObserver	* pObserver = NULL;

		for( Pos = m_ObserverList.GetHeadPosition(); Pos != NULL; )
		{
			pObserver = ( CModelObserver * ) m_ObserverList.GetNext( Pos );
			if(pObserver)
				pObserver->NotifyExspirationDataChanged();
			Sleep(0);
		}
		LeaveCriticalSection(&m_csObservers);
	}
	catch (...)
	{
		LeaveCriticalSection(&m_csObservers);

		theApp.ReportException(_T("CMVModel::NotifyExspirationData"));
	}
}

/**********************************************************************************************//**
 * Notifies the monitor data
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::NotifyMonitorData()
{
	try
	{
		EnterCriticalSection(&m_csObservers);

		POSITION		Pos;
		CModelObserver	* pObserver = NULL;

		for( Pos = m_ObserverList.GetHeadPosition(); Pos != NULL; )
		{
			pObserver = ( CModelObserver * ) m_ObserverList.GetNext( Pos );
			if(pObserver)
				pObserver->NotifyDataChanged();
			Sleep(0);
		}
		LeaveCriticalSection(&m_csObservers);
	}
	catch (...)
	{
		LeaveCriticalSection(&m_csObservers);

		theApp.ReportException(_T("CMVModel::NotifyMonitorData"));
	}
}

/**********************************************************************************************//**
 * Notifies the new alarmlimit data
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::NotifyNewAlarmlimitData()
{
	try
	{
		EnterCriticalSection(&m_csObservers);

		POSITION		Pos;
		CModelObserver	* pObserver = NULL;

		for( Pos = m_ObserverList.GetHeadPosition(); Pos != NULL; )
		{
			pObserver = ( CModelObserver * ) m_ObserverList.GetNext( Pos );
			if(pObserver)
				pObserver->NotifyAlarmLimitChanged();
			Sleep(0);
		}

		LeaveCriticalSection(&m_csObservers);
	}
	catch (...)
	{
		LeaveCriticalSection(&m_csObservers);

		theApp.ReportException(_T("CMVModel::NotifyNewAlarmlimitData"));
	}
}

/**********************************************************************************************//**
 * Notifies the calculate alarmlimit data
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::NotifyCalculateAlarmlimitData()
{
	try
	{
		EnterCriticalSection(&m_csObservers);

		POSITION		Pos;
		CModelObserver	* pObserver = NULL;

		for( Pos = m_ObserverList.GetHeadPosition(); Pos != NULL; )
		{
			pObserver = ( CModelObserver * ) m_ObserverList.GetNext( Pos );
			if(pObserver)
				pObserver->NotifyCalculateAlarmLimit();
			Sleep(0);
		}

		LeaveCriticalSection(&m_csObservers);
	}
	catch (...)
	{
		LeaveCriticalSection(&m_csObservers);

		theApp.ReportException(_T("CMVModel::NotifyCalculateAlarmlimitData"));
	}
}

/**********************************************************************************************//**
 * Gets language string.
 *
 * @author	Rainer K�hner
 * @date	15.08.2018
 *
 * @param	nID	The identifier.
 *
 * @return	The language string.
 **************************************************************************************************/

CStringW CMVModel::GetLanguageString(int nID) 
{
	CStringW szTemp=_T("");

	EnterCriticalSection(&m_csLangString);
	if(false==m_bReloadLangInProgress)
	{
		szTemp=getLANGUAGE()->GetLanguageString(nID);
	}
	else
	{
		DEBUGMSG(TRUE, (TEXT("### ReloadLangInProgress ###\r\n")));
	}
	LeaveCriticalSection(&m_csLangString);

	return szTemp;
}

/**********************************************************************************************//**
 * Sets reload language progress.
 *
 * @author	Rainer K�hner
 * @date	15.08.2018
 *
 * @param	state	True to state.
 **************************************************************************************************/

void CMVModel::setReloadLanguageProgress(bool state) 
{
	EnterCriticalSection(&m_csLangString);
	m_bReloadLangInProgress=state;
	LeaveCriticalSection(&m_csLangString);
}

/**********************************************************************************************//**
 * Gets reload language progress.
 *
 * @author	Rainer K�hner
 * @date	15.08.2018
 *
 * @return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVModel::getReloadLanguageProgress() 
{
	bool bState=true;

	EnterCriticalSection(&m_csLangString);
	bState=m_bReloadLangInProgress;
	LeaveCriticalSection(&m_csLangString);

	return bState;
}
/**********************************************************************************************//**
 * Notifies a view focus changed
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iViewID	Identifier for the view.
 **************************************************************************************************/

void CMVModel::NotifyViewFocusChanged(int iViewID)
{
	try
	{
		EnterCriticalSection(&m_csObservers);

		POSITION		Pos;
		CModelObserver	* pObserver = NULL;

		// Notify all the observers to update themselves
		for( Pos = m_ObserverList.GetHeadPosition(); Pos != NULL; )
		{
			pObserver = ( CModelObserver * ) m_ObserverList.GetNext( Pos );
			if(pObserver)
				pObserver->NotifyViewFocusChanged(iViewID);
		}
		LeaveCriticalSection(&m_csObservers);
	}
	catch (...)
	{
		LeaveCriticalSection(&m_csObservers);

		theApp.ReportException(_T("CMVModel::NotifyViewFocusChanged"));
	}
}

/**********************************************************************************************//**
 * Check alarm data
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::CheckAlarmData() 
{
	if(isSPIavailabel())
	{
		int iAlarmState=getSPI()->Read_STATUS();
		if(iAlarmState>=0)
		{
			getDATAHANDLER()->SetAlarmStateBytes(iAlarmState);

			getAlarmThread()->setCheckAlarmData(true);
			//m_bCheckAlarmData=true;
		}
	}
}

/**********************************************************************************************//**
 * Check flowsensor data
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::CheckFlowsensorData() 
{
	if(true==isSPIavailabel())
	{
		int iState=getSPI()->Read_STATUS_FLOWSENSOR();
		getDATAHANDLER()->SetFlowsensorStateBytes(iState);

		getFlowsensorThread()->setCheckFlowsensorData(true);
		//m_bCheckFlowsensorData=true;

		g_eventNewFlowsensorData.SetEvent();
	}	
}

/**********************************************************************************************//**
 * Parse flowsensor alarm state bytes
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iState	Zero-based index of the state.
 *
 * \return	An int.
 **************************************************************************************************/

int CMVModel::ParseFlowsensorAlarmStateBytes(int iState)
{
	int iChanged=0;
	BYTE byTemp = iState;

	/*tALARMSTATES alarmState;
	getALARMHANDLER()->getAllAlarmStates(&alarmState);*/


	if(byTemp & BIT0)	// Flow-Sensor aus
	{
		if(getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)
		{
			theApp.WriteLog(_T("#HFO:0252"));
			getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
		}
	}
	else
	{
		if(getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF)
		{
			theApp.WriteLog(_T("#HFO:0253"));
			getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_MANOFF);
		}
		else if(getVMODEHANDLER()->activeModeIsTHERAPY() && getDATAHANDLER()->GetFlowSensorState()!=FLOWSENSOR_AUTOOFF)//rku,PRETRIGGER
		{
			theApp.WriteLog(_T("#HFO:0254"));
			getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
		}
	}

	if(byTemp & BIT1)	// Flow-Sensor nicht angeschlossen
	{
		if(		getALARMHANDLER()->CanSetAlarm_FLOW_SENSOR_NOTCONNECTED()
			&&	getDATAHANDLER()->IsFlowSensorStateOff()==false)
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_FLOW_SENSOR_NOTCONNECTED);
		}
	}
	else
	{
		if(getALARMHANDLER()->ALARM_Sens_FLOW_SENSOR_NOTCONNECTED->getAlarmState()==AS_ACTIVE)
		{
			eVentSilentState state=getALARMHANDLER()->getAlarmSilentState();
			if(		state==ASTATE_AUTOSILENT
				||	state==ASTATE_MANSILENT
				||	state==ASTATE_ACTIVE)
				getALARMHANDLER()->setAutoSilent(false,false,false);
			getALARMHANDLER()->setStateOfAlarm(AL_Sens_FLOW_SENSOR_NOTCONNECTED,AS_SIGNALED);

			if(getALARMHANDLER()->isVgarantAutoTurnedOff_FlowSensor() 
				&& getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{
				if(getDATAHANDLER()->PARADATA()->IsVLimitParamOn_IPPV())
				{
					getDATAHANDLER()->SetVLimitParamdata_IPPV(getDATAHANDLER()->PARADATA()->GetVLimitParam_IPPV(),false,true);
				}
				if(getDATAHANDLER()->PARADATA()->IsVLimitParamOn_TRIGGER())
				{
					getDATAHANDLER()->SetVLimitParamdata_TRIGGER(getDATAHANDLER()->PARADATA()->GetVLimitParam_TRIGGER(),false,true);
				}
				getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_PARA_AUTOENABLED);
			}
			else if(getALARMHANDLER()->isVlimitAutoTurnedOff_FlowSensor()
				&& getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{
				if(getDATAHANDLER()->IsActiveModeVGarantStateOn())
				{
					getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_OFF);
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_VGARANT);
				}

				getDATAHANDLER()->SetVLimitParamdata_IPPV(getDATAHANDLER()->GetLastVLimitParam(),true,true);
				getDATAHANDLER()->SetVLimitParamdata_TRIGGER(getDATAHANDLER()->GetLastVLimitParam(),true,true);

				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_VLIMIT_PARA_AUTOENABLED);
				DEBUGMSG(TRUE, (TEXT("WM_VLIMIT_PARA_AUTOENABLED 3 ###\r\n")));
			}

			getALARMHANDLER()->resetVgVlAutoTurnedOff_FlowSensor();
			
		}
	}

	//if(byTemp & BIT2)	// 
	//{
	//	iChanged=1;
	//}
	//else
	//{
	//	iChanged=1;
	//}

	//if(byTemp & BIT3)	// Kalibrierung fehlgeschlagen
	//{
	//	iChanged=1;
	//}
	//else
	//{
	//	iChanged=1;
	//}

	if(byTemp & BIT4)	// Hitzdraht A defekt
	{
		if(getALARMHANDLER()->CanSetAlarm_FLOW_SENSOR_DEFECT())
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_FLOW_SENSOR_DEFECT);
		}
	}
	else if(byTemp & BIT5)	// Hitzdraht A defekt
	{
		if(getALARMHANDLER()->CanSetAlarm_FLOW_SENSOR_DEFECT())
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_FLOW_SENSOR_DEFECT);
		}
	}
	else /*if((byTemp & BIT4)!=true && (byTemp & BIT5)!=true)*/
	{
		if(getALARMHANDLER()->ALARM_Sens_FLOW_SENSOR_DEFECT->getAlarmState()==AS_ACTIVE)
		{
			eVentSilentState state=getALARMHANDLER()->getAlarmSilentState();
			if(		state==ASTATE_AUTOSILENT
				||	state==ASTATE_MANSILENT
				||	state==ASTATE_ACTIVE)
				getALARMHANDLER()->setAutoSilent(false,false,false);
			getALARMHANDLER()->setStateOfAlarm(AL_Sens_FLOW_SENSOR_DEFECT,AS_SIGNALED);
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);

			if(getALARMHANDLER()->isVgarantAutoTurnedOff_FlowSensor()
				&& getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{
				getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_PARA_AUTOENABLED);
			}
			else if(getALARMHANDLER()->isVlimitAutoTurnedOff_FlowSensor()
				&& getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{

				if(getDATAHANDLER()->IsActiveModeVGarantStateOn())
				{
					getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_OFF);
				}
				getDATAHANDLER()->SetVLimitParamdata_IPPV(getDATAHANDLER()->GetLastVLimitParam(),true,true);
				getDATAHANDLER()->SetVLimitParamdata_TRIGGER(getDATAHANDLER()->GetLastVLimitParam(),true,true);

				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_VLIMIT_PARA_AUTOENABLED);
				DEBUGMSG(TRUE, (TEXT("WM_VLIMIT_PARA_AUTOENABLED 4 ###\r\n")));
			}

			getALARMHANDLER()->resetVgVlAutoTurnedOff_FlowSensor();
		}
	}

	//if(byTemp & BIT5)	// Hitzdraht B defekt
	//{
	//	if(getALARMHANDLER()->CanSetAlarm_FLOW_SENSOR_DEFECT())
	//	{
	//		
	//	}
	//}
	//else
	//{
	//	if(alarmState.eSoA_FLOWSENSOR_DEFECT==AS_ACTIVE)
	//	{
	//		eVentSilentState state=getALARMHANDLER()->GetAlarmSilentState();
	//		if(		state==ASTATE_AUTOSILENT
	//			||	state==ASTATE_MANSILENT
	//			/*||	state==ASTATE_MANSILENT_SENSOR*/
	//			||	state==ASTATE_ACTIVE)
	//			getALARMHANDLER()->SetAutoSilent(false,false,false);
	//		//getALARMHANDLER()->deleteAlarm(AL_Sens_FLOW_SENSOR_DEFECT);
	//		getALARMHANDLER()->setStateOfAlarm(AL_Sens_FLOW_SENSOR_DEFECT,AS_SIGNALED);
	//		//iChanged=1;
	//		
	//	}
	//}

	if(byTemp & BIT6)	// Flow Sensor reinigen
	{
		if(getALARMHANDLER()->CanSetAlarm_FLOW_SENSOR_CLEANING())
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_FLOW_SENSOR_CLEANING);
		}
	}
	else
	{
		if(getALARMHANDLER()->ALARM_Sens_FLOW_SENSOR_CLEANING->getAlarmState()==AS_ACTIVE)
		{
			eVentSilentState state=getALARMHANDLER()->getAlarmSilentState();
			if(		state==ASTATE_AUTOSILENT
				||	state==ASTATE_MANSILENT
				||	state==ASTATE_ACTIVE)
				getALARMHANDLER()->setAutoSilent(false,false,false);
			getALARMHANDLER()->setStateOfAlarm(AL_Sens_FLOW_SENSOR_CLEANING,AS_SIGNALED);
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_ALARMSTATE_CHANGED);

			if(getALARMHANDLER()->isVgarantAutoTurnedOff_FlowSensor()
				&& getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{
				getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_PARA_AUTOENABLED);
			}
			else if(getALARMHANDLER()->isVlimitAutoTurnedOff_FlowSensor()
				&& getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{

				if(getDATAHANDLER()->IsActiveModeVGarantStateOn())
				{
					getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_OFF);
				}

				getDATAHANDLER()->SetVLimitParamdata_IPPV(getDATAHANDLER()->GetLastVLimitParam(),true,true);
				getDATAHANDLER()->SetVLimitParamdata_TRIGGER(getDATAHANDLER()->GetLastVLimitParam(),true,true);

				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_VLIMIT_PARA_AUTOENABLED);
				DEBUGMSG(TRUE, (TEXT("WM_VLIMIT_PARA_AUTOENABLED 5 ###\r\n")));
			}
			getALARMHANDLER()->resetVgVlAutoTurnedOff_FlowSensor();
		}
	}

	//if(byTemp & BIT7)	// System-Kalibrierung fehlgeschlagen
	//{
	//	
	//}
	//else
	//{
	//	
	//}

	return iChanged;
}

/**********************************************************************************************//**
 * Query if this instance is spi disconnection
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if spi disconnection, false if not.
 **************************************************************************************************/

bool CMVModel::IsSPIDisconnection()
{
	return m_bSPIDisconnection;
}

/**********************************************************************************************//**
 * Query if this instance is spi tube occlusion
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if spi tube occlusion, false if not.
 **************************************************************************************************/

bool CMVModel::IsSPI_TUBE_OCCLUSION()
{
	return m_bSPI_TUBE_OCCLUSION;
}

/**********************************************************************************************//**
 * Parse alarm state bytes
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iAlarmState	State of the alarm.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVModel::ParseAlarmStateBytes(int iAlarmState)
{
	SHORT byTemp = iAlarmState;
	bool bRes=false;

	/*tALARMSTATES alarmState;
	getALARMHANDLER()->getAllAlarmStates(&alarmState);*/


	//if(byTemp & BIT0)
	//{
	//	m_bSPI_P_INSPIRATION=true;

	//	if(getALARMHANDLER()->CanSetAlarm_P_INSPIRATIONTUBE())
	//	{
	//		if(AfxGetApp())
	//			AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_P_INSPIRATIONTUBE);
	//	}
	//}
	//else
	//{
	//	if(		(alarmState.eSoA_P_INSPIRATION==AS_ACTIVE)
	//		&&	(getSERIAL()->IsSerial_P_INSPIRATION()==false))
	//	{
	//		bRes=true;

	//		eVentSilentState state=getALARMHANDLER()->getAlarmSilentState();
	//		if(		state==ASTATE_AUTOSILENT
	//			||	state==ASTATE_MANSILENT
	//			//||	state==ASTATE_MANSILENT_SYSALARM
	//			||	state==ASTATE_ACTIVE)
	//			getALARMHANDLER()->setAutoSilent(false,false,false);
	//		getALARMHANDLER()->setStateOfAlarm(AL_SysAl_P_INSPIRATIONTUBE,AS_SIGNALED);
	//	}
	//	m_bSPI_P_INSPIRATION=false;
	//}

	//if(byTemp & BIT1)
	//{
	//	m_bSPI_P_EXSPIRATION=true;

	//	if(getALARMHANDLER()->CanSetAlarm_P_EXSPIRATIONTUBE())
	//	{
	//		if(AfxGetApp())
	//			AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_P_EXSPIRATIONTUBE);
	//	}
	//}
	//else
	//{
	//	if(		(alarmState.eSoA_P_EXSPIRATION==AS_ACTIVE)
	//		&&	(getSERIAL()->IsSerial_P_EXSPIRATION()==false))
	//	{
	//		bRes=true;

	//		eVentSilentState state=getALARMHANDLER()->getAlarmSilentState();
	//		if(		state==ASTATE_AUTOSILENT
	//			||	state==ASTATE_MANSILENT
	//			//||	state==ASTATE_MANSILENT_SYSALARM
	//			||	state==ASTATE_ACTIVE)
	//		{
	//			getALARMHANDLER()->setAutoSilent(false,false,false);
	//		}

	//		getALARMHANDLER()->setStateOfAlarm(AL_SysAl_P_EXSPIRATIONTUBE,AS_SIGNALED);
	//	}
	//	m_bSPI_P_EXSPIRATION=false;
	//}

	if(byTemp & BIT2)
	{
		//DEBUGMSG(TRUE, (TEXT("SPI TUBEOCCLUSIONBIT\r\n")));
		m_bSPI_TUBE_OCCLUSION=true;
		if(getALARMHANDLER()->CanSetAlarm_TUBE_OCCLUSION())
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_TUBE_OCCLUSION);
			//DEBUGMSG(TRUE, (TEXT("WM_SETALARM_TUBE_OCCLUSION SPI ###\r\n")));
		}
	}
	else
	{
		if(		(getALARMHANDLER()->ALARM_SysAl_TUBE_OCCLUSION->getAlarmState()==AS_ACTIVE)
			&&	(getSERIAL()->IsSerial_TUBE_OCCLUSION()==false))
		{
			bRes=true;
			
			/*setActiveAlarmDelay(true);
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SET_ACTIVEALARM_DELAY);

			getALARMHANDLER()->setStateOfAlarm(AL_SysAl_TUBE_OCCLUSION,AS_SIGNALED);*/
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SERIAL_DELALARM_TUBE_OCCLUSION);
			//DEBUGMSG(TRUE, (TEXT("WM_SERIAL_DELALARM_TUBE_OCCLUSION SPI ###\r\n")));
			
		}
		m_bSPI_TUBE_OCCLUSION=false;
	}

	if(byTemp & BIT3)
	{
		if(getALARMHANDLER()->CanSetAlarm_P_IN_AIR())
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_P_IN_AIR);
		}
	}
	else
	{
		if(getALARMHANDLER()->ALARM_SysAl_P_IN_AIR->getAlarmState()==AS_ACTIVE)
		{
			bRes=true;
			
			getALARMHANDLER()->setStateOfAlarm(AL_SysAl_P_IN_AIR,AS_SIGNALED);
			if(IsO2calFlag())
				DeleteO2calFlag();
		}
	}

	if(byTemp & BIT4)
	{
		if(getALARMHANDLER()->CanSetAlarm_P_IN_O2())
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_P_IN_O2);
		}
	}
	else
	{
		if(getALARMHANDLER()->ALARM_SysAl_P_IN_O2->getAlarmState()==AS_ACTIVE)
		{
			bRes=true;
			
			//eVentSilentState state=getALARMHANDLER()->getAlarmSilentState();
			//if(		state==ASTATE_AUTOSILENT
			//	||	state==ASTATE_MANSILENT
			//	//||	state==ASTATE_MANSILENT_SYSALARM
			//	||	state==ASTATE_ACTIVE)
			//	getALARMHANDLER()->setAutoSilent(false,false,false);

			getALARMHANDLER()->setStateOfAlarm(AL_SysAl_P_IN_O2,AS_SIGNALED);
			if(IsO2calFlag())
				DeleteO2calFlag();
		}
	}

	//if(getI2C()->GetACCUstate()==false && IsI2C_ACCUavailability()==FALSE)
	//{
	//	if(byTemp & BIT5)
	//	{
	//		if(true==getDATAHANDLER()->SetAccuSupply(true))
	//		{
	//			//theApp.WriteLog(_T("SetAccuSupply(true)"), false);
	//			if(getALARMHANDLER()->SetAlarm_Accu_POWER(_T("")))
	//			{
	//				bRes=true;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		if(alarmState.eSoA_Akku_0!=AS_NONE)
	//		{
	//			getALARMHANDLER()->deleteAlarm(AL_Accu_POWER);
	//		}
	//		if(true==getDATAHANDLER()->SetAccuSupply(false))
	//		{
	//			//theApp.WriteLog(_T("SetAccuSupply(false)"), false);
	//			bRes=true;
	//		}
	//	}
	//}

	if(byTemp & BIT5)
	{
		if(getALARMHANDLER()->CanSetAlarm_Fan())
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_Fan);
		}
	}
	else
	{
		if(getALARMHANDLER()->ALARM_SysFail_Fan->getAlarmState()==AS_ACTIVE)
		{
			bRes=true;

			eVentSilentState state=getALARMHANDLER()->getAlarmSilentState();
			if(		state==ASTATE_AUTOSILENT
				||	state==ASTATE_MANSILENT
				//||	state==ASTATE_MANSILENT_SYSALARM
				||	state==ASTATE_ACTIVE)
				getALARMHANDLER()->setAutoSilent(false,false,false);

			getALARMHANDLER()->setStateOfAlarm(AL_SysFail_Fan,AS_SIGNALED);
		}
	}
	

	if(byTemp & BIT6)
	{
		m_bSPIDisconnection=true;
		if(getALARMHANDLER()->CanSetAlarm_DISCONNECTION())
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_DISCONNECTION);
		}
	}
	else
	{
		if(		(getALARMHANDLER()->ALARM_DISCONNECTION->getAlarmState()==AS_ACTIVE)
			&&	(getSERIAL()->IsSerialDisconnection()==false))
		{
			bRes=true;

			getALARMHANDLER()->setStateOfAlarm(AL_DISCONNECTION,AS_SIGNALED);
		}

		m_bSPIDisconnection=false;
	}

	if(byTemp & BIT7)
	{
		if(getALARMHANDLER()->CanSetAlarm_O2_VALUE_INCORRECT())
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_O2_VALUE_INCORRECT);
		}
	}
	else
	{
		if(getALARMHANDLER()->ALARM_Sens_O2_VALUE_INCORRECT->getAlarmState()==AS_ACTIVE)
		{
			bRes=true;
			//getALARMHANDLER()->deleteAlarm(AL_Sens_O2_VALUE_INCORRECT);
			getALARMHANDLER()->setStateOfAlarm(AL_Sens_O2_VALUE_INCORRECT,AS_SIGNALED);
		}
	}

	if(byTemp & BIT8)
	{
		if(getALARMHANDLER()->CanSetAlarm_O2_SENSOR_DEFECT())
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_O2_SENSOR_DEFECT);
		}
	}
	else
	{
		if(getALARMHANDLER()->ALARM_Sens_O2_SENSOR_DEFECT->getAlarmState()==AS_ACTIVE)
		{
			bRes=true;
			//getALARMHANDLER()->deleteAlarm(AL_Sens_O2_SENSOR_DEFECT);
			getALARMHANDLER()->setStateOfAlarm(AL_Sens_O2_SENSOR_DEFECT,AS_SIGNALED);
		}
	}


	if(byTemp & BIT9)
	{
		if(getALARMHANDLER()->CanSetAlarm_Vgaranty())
		{
			int iTVEmeas=0;
			int iVGarant=0;
			if(getVMODEHANDLER()->activeModeIsHFO())
			{
				iTVEmeas=getDATAHANDLER()->getAVGMessureDataTVEHFO()*10;
				iVGarant=getDATAHANDLER()->PARADATA()->GetHFVGarantPara()*10;
			}
			else
			{
				if(		getCONFIG()->GetCurMode()==VM_SIMVPSV
					||	(getCONFIG()->CurModeIsPresetMode()==true
					&&	getCONFIG()->GetPrevMode()==VM_SIMVPSV))
				{
					iTVEmeas=getDATAHANDLER()->getMessureDataTVEresp()*10;
				}
				else
				{
					iTVEmeas=getDATAHANDLER()->getMessureDataTVE()*10;
				}
				if(getVMODEHANDLER()->activeModeIsIPPV())
					iVGarant=getDATAHANDLER()->PARADATA()->GetVGarantPara_IPPV()*10;
				else
					iVGarant=getDATAHANDLER()->PARADATA()->GetVGarantPara_TRIGGER()*10;
			}

			if(iTVEmeas>iVGarant)
			{
				m_eVGaranty=VGAL_HIGH;
			}
			else
			{
				m_eVGaranty=VGAL_LOW;
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_Vgaranty);
			}
			
		}
	}
	else
	{
		m_eVGaranty=VGAL_NONE;
		if(getALARMHANDLER()->ALARM_SysLimit_Vgaranty->getAlarmState()!=AS_NONE)
		{
			bRes=true;
			getALARMHANDLER()->deleteAlarm(AL_SysLimit_Vgaranty);
		}
	}

	if(byTemp & BIT10)
	{
		if(getALARMHANDLER()->CanSetAlarm_VOLTAGE())
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_VOLTAGE);
		}
	}
	else
	{
		if(getALARMHANDLER()->ALARM_SysFail_VOLTAGE->getAlarmState()==AS_ACTIVE)
		{
			bRes=true;
			getALARMHANDLER()->setStateOfAlarm(AL_SysFail_VOLTAGE,AS_SIGNALED);
			getDATAHANDLER()->ResetI2CWatchdogState();
		}
	}

	if(byTemp & BIT11)
	{
		if(getALARMHANDLER()->CanSetAlarm_RELAIS_DEFECT())
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_RELAIS_DEFECT);
		}
	}
	else
	{
		if(getALARMHANDLER()->ALARM_SysFail_RELAIS_DEFECT->getAlarmState()==AS_ACTIVE)
		{
			bRes=true;
			getALARMHANDLER()->setStateOfAlarm(AL_SysFail_RELAIS_DEFECT,AS_SIGNALED);
		}
	}

	if(byTemp & BIT12)
	{
		//systemfehler ger�t abschalten
		if(getALARMHANDLER()->CanSetAlarm_ChecksumMonPIC())
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_ChecksumMonPIC);
		}
	}
	else
	{
		if(getALARMHANDLER()->ALARM_SysFail_ChecksumMonPIC->getAlarmState()==AS_ACTIVE)
		{
			bRes=true;
			getALARMHANDLER()->setStateOfAlarm(AL_SysFail_ChecksumMonPIC,AS_SIGNALED);
		}
	}

	if(byTemp & BIT13)
	{
		//DEBUGMSG(TRUE, (TEXT("SPI TUBUSBLOCKED\r\n")));
		if(getALARMHANDLER()->CanSetAlarm_TUBUSBLOCKED())
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_TUBUSBLOCKED);

			//DEBUGMSG(TRUE, (TEXT("SPI TUBUSBLOCKED\r\n")));
		}
	}
	else
	{
		if(getALARMHANDLER()->ALARM_TUBUSBLOCKED->getAlarmState()==AS_ACTIVE)
		{
			bRes=true;

			DEBUGMSG(TRUE, (TEXT("AL_TUBUSBLOCKED,AS_SIGNALED\r\n")));
			getALARMHANDLER()->setStateOfAlarm(AL_TUBUSBLOCKED,AS_SIGNALED);

			if(getALARMHANDLER()->isVgarantAutoTurnedOff_Tube() 
				&& getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{
				if(getDATAHANDLER()->PARADATA()->IsVLimitParamOn_IPPV())
				{
					getDATAHANDLER()->SetVLimitParamdata_IPPV(getDATAHANDLER()->PARADATA()->GetVLimitParam_IPPV(),false,true);
				}
				if(getDATAHANDLER()->PARADATA()->IsVLimitParamOn_TRIGGER())
				{
					getDATAHANDLER()->SetVLimitParamdata_TRIGGER(getDATAHANDLER()->PARADATA()->GetVLimitParam_TRIGGER(),false,true);
				}
				getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_PARA_AUTOENABLED);
			}
			else if(getALARMHANDLER()->isVlimitAutoTurnedOff_Tube()
				&& getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{
				if(getDATAHANDLER()->IsActiveModeVGarantStateOn())
				{
					getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_OFF);
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_VGARANT);
				}

				getDATAHANDLER()->SetVLimitParamdata_IPPV(getDATAHANDLER()->GetLastVLimitParam(),true,true);
				getDATAHANDLER()->SetVLimitParamdata_TRIGGER(getDATAHANDLER()->GetLastVLimitParam(),true,true);

				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_VLIMIT_PARA_AUTOENABLED);
				DEBUGMSG(TRUE, (TEXT("WM_VLIMIT_PARA_AUTOENABLED 6 ###\r\n")));
			}

			getALARMHANDLER()->resetVgVlAutoTurnedOff_Tube();
		}
	}

	if(getVMODEHANDLER()->activeModeIsHFO()==false)
	{
		if(byTemp & BIT14)
		{
			if(		getCONFIG()->GetCurMode()==VM_PSV 
				||	(getCONFIG()->CurModeIsPresetMode()==true && getCONFIG()->GetPrevMode()==VM_PSV))
			{
				/*if(getDATAHANDLER()->isPSVapnoe()==false)
				{
				if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PSV_APNOE);
				}*/
				
			}
			else
			{
				if(		getALARMHANDLER()->ALARM_PatAl_Apnoe->getAlarmState()!=AS_ACTIVE 
					&&	getALARMHANDLER()->CanSetAlarm_Apnoe())
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_Apnoe);
				}
			}
		}
		else
		{
			if(		getCONFIG()->GetCurMode()==VM_PSV 
				||	(getCONFIG()->CurModeIsPresetMode()==true && getCONFIG()->GetPrevMode()==VM_PSV))
			{
				/*if(getALARMHANDLER()->GetStateOfAlarm(AL_SysInfo_BackupActive)==AS_ACTIVE)
				{
					bRes=true;
					getALARMHANDLER()->deleteAlarm(AL_SysInfo_BackupActive);
				}*/
			}
			else
			{
				if(getALARMHANDLER()->ALARM_PatAl_Apnoe->getAlarmState()==AS_ACTIVE)
				{
					bRes=true;
					getALARMHANDLER()->setStateOfAlarm(AL_PatAl_Apnoe,AS_SIGNALED);

					if(getCONFIG()->getCO2module()!=CO2MODULE_NONE && getETCO2()!=NULL)
					{
						getETCO2()->set_restartBreathAlgorithm();
					}
				}
			}
		}
	}
	

	if(byTemp & BIT15)
	{
		m_bVlimitted=true;
		

		if(		IsCalibrationViewActive()==false
			&&	getALARMHANDLER()->CanSetAlarm_Vlimitted())
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SETALARM_Vlimitted);
		}
		else if(	getALARMHANDLER()->ALARM_SysLimit_Vlimitted->getAlarmState()!=AS_NONE
				&&	IsCalibrationViewActive()==true)
		{
			bRes=true;
			getALARMHANDLER()->deleteAlarm(AL_SysLimit_Vlimitted);
		}
	}
	else
	{
		m_bVlimitted=false;
		if(getALARMHANDLER()->ALARM_SysLimit_Vlimitted->getAlarmState()!=AS_NONE)
		{
			bRes=true;
			getALARMHANDLER()->deleteAlarm(AL_SysLimit_Vlimitted);
		}
	}

	

	return bRes;
	
}

/**********************************************************************************************//**
 * Gets vlimitted flag
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVModel::GetVlimittedFlag()
{
	return m_bVlimitted;
}

/**********************************************************************************************//**
 * Gets vgaranty alarm flag
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	The v garanty alarm flag.
 **************************************************************************************************/

eVolumeGuarantyAlarm CMVModel::getVGarantyAlarmFlag()
{
	return m_eVGaranty;
}

/**********************************************************************************************//**
 * Sets seria lavailable
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CMVModel::setSERIALavailable(bool state)
{
	m_bSERIALavailability=state;

	if(state)
	{
		//DEBUGMSG(TRUE, (TEXT("### SetSERIALavailability true ###\r\n")));
	}
	else
	{
		theApp.WriteLog(_T("#HFO:0291"));
		//DEBUGMSG(TRUE, (TEXT("### SetSERIALavailability false ###\r\n")));
	}
}

/**********************************************************************************************//**
 * Query if this instance is seria lavailable
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if seria lavailable, false if not.
 **************************************************************************************************/

bool CMVModel::isSERIALavailable()
{
	return m_bSERIALavailability;
}

/**********************************************************************************************//**
 * Sets sp iavailability
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CMVModel::setSPIavailability(bool state)
{
	if(state!=m_bSPIavailability)
	{
		if(state)
		{
			//theApp.WriteLog(_T("SPI SetSPIavailability true"));
		}
		else
		{
			theApp.WriteLog(_T("#HFO:0255"));
		}
	}
	m_bSPIavailability=state;
}

/**********************************************************************************************//**
 * Query if this instance is sp iavailabel
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if sp iavailabel, false if not.
 **************************************************************************************************/

bool CMVModel::isSPIavailabel()
{
	return m_bSPIavailability;
}

/**********************************************************************************************//**
 * Reinitializes the serial
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVModel::reinitSERIAL()
{
	setSERIALavailable(FALSE);

	theApp.WriteLog(_T("CMVModel::ReinitSERIAL()"));

	EnterCriticalSection(&m_csSerial);
	bool bRes=TRUE;

	if(SERIAL)
	{
		SERIAL->Deinit();
	}

	if(SERIAL==NULL)
	{
		bRes=FALSE;
		getDATAHANDLER()->setCOMErrorCode(ERRC_COM_INIT);
		//SetSERIALavailability(FALSE);
	}
	else if(SERIAL->Init()==false)
	{
		bRes=FALSE;
		getDATAHANDLER()->setCOMErrorCode(ERRC_COM_INIT);
		//SetSERIALavailability(FALSE);
	}
	LeaveCriticalSection(&m_csSerial);

	return bRes;
}

/**********************************************************************************************//**
 * Sets di oavailability
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CMVModel::SetDIOavailability(bool state)
{
	m_bDIOavailability=state;
}

/**********************************************************************************************//**
 * Query if this instance is dio availability
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if di oavailability, false if not.
 **************************************************************************************************/

bool CMVModel::IsDIOavailability()
{
	return m_bDIOavailability;
}

/**********************************************************************************************//**
 * Sets pif availability
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CMVModel::SetPIFavailability(bool state)
{
	m_bPIFavailability=state;
}

/**********************************************************************************************//**
 * Query if this instance is pif availability
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if pi favailability, false if not.
 **************************************************************************************************/

bool CMVModel::IsPIFavailability()
{
	return m_bPIFavailability;
}

/**********************************************************************************************//**
 * Sets aculink availability
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CMVModel::SetAcuLinkAvailability(bool state)
{
	m_bAcuLinkAvailability=state;
}

/**********************************************************************************************//**
 * Query if this instance is acu link availability
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if acu link availability, false if not.
 **************************************************************************************************/

bool CMVModel::IsAcuLinkAvailability()
{
	return m_bAcuLinkAvailability;
}

/**********************************************************************************************//**
 * Resets the exhal valv calibration mode
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVModel::ResetExhalValvCalMode()
{
	bool bRes=false;

	if(getDATAHANDLER()->GetExhalValvCalMode())
	{
		bRes=true;
		getDATAHANDLER()->SetExhalValvCalMode(false);
		getALARMHANDLER()->setAutoSilent(false,true,false);

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_CONTROL_DEL_MODE_EXHALVALVCAL);
	}

	return bRes;
}

/**********************************************************************************************//**
 * Activates the o2 flush
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::activateO2Flush()
{
	EnterCriticalSection(&m_csO2Flush);
	m_bO2FlushActive=true;
	LeaveCriticalSection(&m_csO2Flush);
	

	theApp.WriteLog(_T("activateO2Flush"));

	getVIEWHANDLER()->refreshO2Flush();

	//bool bPRICOrunning=false; //PRICO04
	//if(getDATAHANDLER()->isPRICOLicenseAvailable()==true)
	//{
	//	
	//	if(getPRICOThread()->isPRICOalgorithmRunning())
	//	{
	//		bPRICOrunning=true;
	//	}
	//}

	//if(bPRICOrunning)//PRICO04
	//{
	//	getPRICOThread()->resetCircBufSpO2();
	//}

	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();
}

/**********************************************************************************************//**
 * Deactivate o2 flush
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::deactivateO2Flush()
{
	EnterCriticalSection(&m_csO2Flush);
	m_bO2FlushActive=false;
	LeaveCriticalSection(&m_csO2Flush);

	theApp.WriteLog(_T("deactivateO2Flush"));

	getVIEWHANDLER()->refreshO2Flush();

	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();
}

/**********************************************************************************************//**
 * Queries if the o2 flush is active
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if the o 2 flush is active, false if not.
 **************************************************************************************************/

bool CMVModel::isO2FlushActive()
{
	bool bState=false;
	EnterCriticalSection(&m_csO2Flush);
	bState=m_bO2FlushActive;
	LeaveCriticalSection(&m_csO2Flush);
	return bState;
}

/**********************************************************************************************//**
 * Activates the manbreath
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::activateMANBREATH()
{
	theApp.WriteLog(_T("***MATZ on"));
	getSPI()->activateMANBREATH();

	m_bMANBREATHrunning=true;
	Send_MODE_OPTION1(true,false);
}

/**********************************************************************************************//**
 * Deactivate manbreath
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::deactivateMANBREATH()
{
	theApp.WriteLog(_T("***MATZ off"));
	getSPI()->deactivateMANBREATH();

	if(getVMODEHANDLER()->activeModeIsNMODE() == false)
	{
		setActiveAlarmDelay(true);
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SET_ACTIVEALARM_DELAY);
	}

	m_bMANBREATHrunning=false;
	Send_MODE_OPTION1(true,false);
}

/**********************************************************************************************//**
 * Query if this instance is manbreath running
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if manbreat hrunning, false if not.
 **************************************************************************************************/

bool CMVModel::isMANBREATHrunning()
{
	return m_bMANBREATHrunning;
}

/**********************************************************************************************//**
 * Sets count down alarm silent
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iCount	Number of.
 **************************************************************************************************/

void CMVModel::SetCountDownAlarmSilent(int iCount)
{
	m_iCountDownAlarmSilent=iCount;
}

/**********************************************************************************************//**
 * Gets count down alarm silent
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	The count down alarm silent.
 **************************************************************************************************/

int CMVModel::GetCountDownAlarmSilent()
{
	return m_iCountDownAlarmSilent;
}

/**********************************************************************************************//**
 * Stops count down alarm silent
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::StopCountDownAlarmSilent()
{
	m_iCountDownAlarmSilent=0;
	getVIEWHANDLER()->StopCountDownAlarmSilent();
}

/**********************************************************************************************//**
 * Draw count down alarm silent
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iCountDownAlarmSilent	Zero-based index of the count down alarm silent.
 **************************************************************************************************/

void CMVModel::DrawCountDownAlarmSilent(int iCountDownAlarmSilent)
{
	getVIEWHANDLER()->DrawCountDownAlarmSilent(iCountDownAlarmSilent);
}

/**********************************************************************************************//**
 * Sets active alarm delay
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	bDelay	True to delay.
 **************************************************************************************************/

void CMVModel::setActiveAlarmDelay(bool bDelay)
{
	/*if(bDelay)
	{
		DEBUGMSG(TRUE, (TEXT("CMVModel::setActiveAlarmDelay true\r\n")));
	}
	else
	{
		DEBUGMSG(TRUE, (TEXT("CMVModel::setActiveAlarmDelay false\r\n")));
	}*/
	m_bActiveAlarmDelay=bDelay;
}

/**********************************************************************************************//**
 * Query if this instance is active alarm delay
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if active alarm delay, false if not.
 **************************************************************************************************/

bool CMVModel::isActiveAlarmDelay()
{
	return m_bActiveAlarmDelay;
}

/**********************************************************************************************//**
 * Sets state calibration view
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CMVModel::SetStateCalibrationView(bool state)
{
	m_bCalibrationViewActive=state;
}

/**********************************************************************************************//**
 * Queries if the calibration view is active
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if the calibration view is active, false if not.
 **************************************************************************************************/

bool CMVModel::IsCalibrationViewActive()
{
	return m_bCalibrationViewActive;
}

/**********************************************************************************************//**
 * Starts an installer
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::StartInstaller()
{
	m_bInstaller=true;
}

/**********************************************************************************************//**
 * Query if this instance is installer
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if installer, false if not.
 **************************************************************************************************/

bool CMVModel::IsInstaller()
{
	return m_bInstaller;
}

/**********************************************************************************************//**
 * Sets accu turnoff
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::SetAccuTurnoff()
{
	m_bAccuTurnOff=true;
}

/**********************************************************************************************//**
 * Query if this instance is accu turnoff
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if accu turnoff, false if not.
 **************************************************************************************************/

bool CMVModel::IsAccuTurnoff()
{
	return m_bAccuTurnOff;
}

/**********************************************************************************************//**
 * Enables the pattern alarm o2dependend
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::EnablePatAlarmO2dependend()
{
	m_bPatAlarmO2dependendEnabled=true;
}

/**********************************************************************************************//**
 * Disables the pattern alarm o2dependend
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::DisablePatAlarmO2dependend()
{
	m_bPatAlarmO2dependendEnabled=false;
}

/**********************************************************************************************//**
 * Queries if the pattern alarm o2dependend is enabled
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if the pattern alarm o 2dependend is enabled, false if not.
 **************************************************************************************************/

bool CMVModel::IsPatAlarmO2dependendEnabled()
{
	return m_bPatAlarmO2dependendEnabled;
}

//void CMVModel::SetMainthreadPending(bool bState)
//{
//	m_bMainthreadPending=bState;
//}
//
//bool CMVModel::IsMainthreadPending()
//{
//	return m_bMainthreadPending;
//}

/**********************************************************************************************//**
 * Sets video running
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CMVModel::SetVideoRunning(bool bState)
{
	m_bVideoRunning=bState;
}

/**********************************************************************************************//**
 * Query if this instance is video running
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if video running, false if not.
 **************************************************************************************************/

bool CMVModel::IsVideoRunning()
{
	return m_bVideoRunning;
}

/**********************************************************************************************//**
 * Sends a mode option 1
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	bSPI   	True to spi.
 * \param	bSerial	True to serial.
 *
 * \return	A WORD.
 **************************************************************************************************/

WORD CMVModel::Send_MODE_OPTION1(bool bSPI,bool bSerial)
{
	WORD wMode=0;

	//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION1\r\n")));

	if(bSPI)
		getSPI()->Send_HARDWARE_CONFIG(getDATAHANDLER()->getHWconfig());
	if(bSerial)
		getSERIAL()->Send_HARDWARE_CONFIG(getDATAHANDLER()->getHWconfig());

	if(VENT_RUNNING!=GetVentRunState())
		wMode=setBitOfWord(wMode, STARTSTOPVENT_BIT);

	if(getDATAHANDLER()->IsActiveModeVGarantStateOn()==true)
	{
		wMode=setBitOfWord(wMode, PARAVGARANTY_BIT);
	}

	if(getDATAHANDLER()->IsActiveModeVLimitStateOn()==true)
	{
		wMode=setBitOfWord(wMode, PARAVLIMIT_BIT);
	}

	if(PEDIATRIC==getCONFIG()->GetVentRange())
		wMode=setBitOfWord(wMode, VENTRANGE_BIT);

	if(true==getDATAHANDLER()->IsFlowSensorCalibrating())
		wMode=setBitOfWord(wMode, FLOWCALA_BIT);

	if(true==getCONFIG()->getBTPS())
	{
		wMode=setBitOfWord(wMode, BTPS_BIT);
		//DEBUGMSG(TRUE, (TEXT("send BTPS true\r\n")));
	}
	/*else
	{
		DEBUGMSG(TRUE, (TEXT("send BTPS false\r\n")));
	}*/

	if(true==getDATAHANDLER()->GetExhalValvCalMode())
		wMode=setBitOfWord(wMode, AVABGLEICH_BIT);

	/*if(false==getCONFIG()->GetVolumeTrigger())
		wMode=setBitOfWord(wMode, TRIGGERMODE_BIT);*/

	if(true==getDATAHANDLER()->GetO21SensorCalState())
	{
		//DEBUGMSG(TRUE, (TEXT("OXYCAL21_BIT\r\n")));
		wMode=setBitOfWord(wMode, OXYCAL21_BIT);
	}

	if(true==getDATAHANDLER()->GetO100SensorCalState())
	{
		//DEBUGMSG(TRUE, (TEXT("OXYCAL100_BIT\r\n")));
		wMode=setBitOfWord(wMode, OXYCAL100_BIT);
	}

	eTubeSet eTube=getDATAHANDLER()->GetTubeSet();
	switch(eTube)
	{
	case TUBE_INFANTFLOW:
		{
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION1 TUBE_INFANTFLOW\r\n")));
		}
		break;
	case TUBE_MEDIJET:
		{
			wMode=setBitOfWord(wMode, TUBESET10_BIT);
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION1 TUBE_MEDIJET\r\n")));
		}
		break;
	case TUBE_INFANTFLOW_LP:
		{
			wMode=setBitOfWord(wMode, TUBESET11_BIT);
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION1 TUBE_INFANTFLOW_LP\r\n")));
		}
		break;
	}

	if(getVMODEHANDLER()->activeModeIsHFO())
	{
		eRatioIE ratio = getDATAHANDLER()->GetCurrentIERatioParaHFO();

		switch(ratio)
		{
		case RIE_1_3:
			{
				//DEBUGMSG(TRUE, (TEXT("Send IERATIO 1:3\r\n")));
			}
			break;
		case RIE_1_2:
			{
				wMode=setBitOfWord(wMode, IERATIO12_BIT);
				//DEBUGMSG(TRUE, (TEXT("Send IERATIO 1:2\r\n")));
			}
			break;
		case RIE_1_1:
			{
				wMode=setBitOfWord(wMode, IERATIO13_BIT);
				//DEBUGMSG(TRUE, (TEXT("Send IERATIO 1:1\r\n")));
			}
			break;
		}
	}
	else if(getCONFIG()->GetCurMode()==VM_SERVICE && getCONFIG()->GetPrevMode()==VM_HFO)
	{
		eRatioIE ratio = getDATAHANDLER()->GetCurrentIERatioParaHFO();

		switch(ratio)
		{
		case RIE_1_3:
			{
				//DEBUGMSG(TRUE, (TEXT("Send IERATIO 1:3\r\n")));
			}
			break;
		case RIE_1_2:
			{
				wMode=setBitOfWord(wMode, IERATIO12_BIT);
				//DEBUGMSG(TRUE, (TEXT("Send IERATIO 1:2\r\n")));
			}
			break;
		case RIE_1_1:
			{
				wMode=setBitOfWord(wMode, IERATIO13_BIT);
				//DEBUGMSG(TRUE, (TEXT("Send IERATIO 1:1\r\n")));
			}
			break;
		}
	}

	if(true==getDATAHANDLER()->IsVentilBurnRunning())
		wMode=setBitOfWord(wMode, VENTILBURN14_BIT);

	//Man. ATZ
	if(true==isMANBREATHrunning())
	{
		wMode=setBitOfWord(wMode, MANBREAT15_BIT);
	}

	if(bSPI)
		getSPI()->Send_MODE_OPTION1(wMode);
	if(bSerial)
		getSERIAL()->Send_MODE_OPTION1(wMode);
	
	if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_MODE_OPTION,wMode);

	Sleep(0);
	
	Send_MODE_OPTION2(bSPI,bSerial);

	return wMode;
}

/**********************************************************************************************//**
 * Sends a mode option 2
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	bSPI   	True to spi.
 * \param	bSerial	True to serial.
 *
 * \return	A WORD.
 **************************************************************************************************/

WORD CMVModel::Send_MODE_OPTION2(bool bSPI,bool bSerial)
{
	WORD wMode=0;

	eCurveForm form = getCONFIG()->GetCurPressureRiseCtrl();

	switch(form)
	{
	case CURVE_IFLOW:
		{
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION2 CURVE_IFLOW\r\n")));
		}
		break;
	case CURVE_LINEAR:
		{
			wMode=setBitOfWord(wMode, MODOPT2_CURVEFORM1_BIT);
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION2 CURVE_LINEAR\r\n")));
		}
		break;
	case CURVE_AUTOFLOW: 
		{
			wMode=setBitOfWord(wMode, MODOPT2_CURVEFORM2_BIT);
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION2 CURVE_AUTOFLOW\r\n")));
		}
		break;
	}



	if(getVMODEHANDLER()->activeModeIsHFO())
	{
		if(getDATAHANDLER()->GetCurrentFreqRecPara()!=0)
		{
			wMode=setBitOfWord(wMode, MODOPT2_HFORECRUITMENT_BIT);
		}
	}

	if(getVMODEHANDLER()->activeModeIsHFO())
	{
		if(getDATAHANDLER()->PARADATA()->GetHFFlowPara()!=0)
		{
			wMode=setBitOfWord(wMode, MODOPT2_HFOFLOWMANUAL_BIT);
		}
	}

	/*if(getDATAHANDLER()->isNebulizerOn()==true)
	{
		wMode=setBitOfWord(wMode, MODOPT2_NEBULIZER_BIT);
		
	}*/

	eLeakCompensation eLeakCompOff=getCONFIG()->getLeakCompensation();
	switch(eLeakCompOff)
	{
	//case LC_OFF:
	//	{
	//		//
	//	}
	//	break;
	case LC_LOW:
		{
			wMode=setBitOfWord(wMode, MODOPT2_LEAKCOMPENSATION1_BIT);
		}
		break;
	case LC_MIDDLE:
		{
			wMode=setBitOfWord(wMode, MODOPT2_LEAKCOMPENSATION2_BIT);
		}
		break;
	case LC_HIGH:
		{
			wMode=setBitOfWord(wMode, MODOPT2_LEAKCOMPENSATION1_BIT);
			wMode=setBitOfWord(wMode, MODOPT2_LEAKCOMPENSATION2_BIT);
		}
		break;
	}

	if(getVMODEHANDLER()->activeModeIsHFO())
	{
		if(getCONFIG()->getFreshgasExt()==true)
		{
			wMode=setBitOfWord(wMode, MODOPT2_FRESHGASEXT_BIT);
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION2 FreshgasExt\r\n")));
		}
		else
		{
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION2 FreshgasInt\r\n")));
		}
	}
	else if(getVMODEHANDLER()->activeModeIsNMODE())
	{
		wMode=setBitOfWord(wMode, MODOPT2_FRESHGASEXT_BIT);
	}

	eTriggereType trigger = TRIGGER_FLOW;
	if(		getCONFIG()->GetCurMode()==VM_DUOPAP
		||	getCONFIG()->GetCurMode()==VM_PRE_DUOPAP)
	{
		trigger=getDATAHANDLER()->getTriggerOptionDUOPAP();
	}
	else if(	getCONFIG()->GetCurMode()==VM_NCPAP
		||	getCONFIG()->GetCurMode()==VM_PRE_NCPAP)
	{
		trigger=getDATAHANDLER()->getTriggerOptionNCPAP();
	}
	/*else if(	getCONFIG()->GetCurMode()==VM_CPAP
		||	getCONFIG()->GetCurMode()==VM_PRE_CPAP)
	{
		trigger=getDATAHANDLER()->getTriggerOption_CPAP();
	}*/
	else
	{
		trigger=getDATAHANDLER()->getTriggerOptionCONV();
	}
	switch(trigger)
	{
	case TRIGGER_VOLUME:
		{
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION2 TRIGGER_VOLUME\r\n")));
		}
		break;
	case TRIGGER_FLOW:
		{
			wMode=setBitOfWord(wMode, MODOPT2_TRIGGERFLOW_BIT);
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION2 TRIGGER_FLOW\r\n")));
		}
		break;
	case TRIGGER_PRESSURE:
		{
			wMode=setBitOfWord(wMode, MODOPT2_TRIGGERPRESSURE_BIT);
			//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION2 TRIGGER_PRESSURE\r\n")));
		}
		break;
	}
	
	if(true==getDATAHANDLER()->getFOToscillationState())
	{
		wMode=setBitOfWord(wMode, MODOPT2_FOTRUN_BIT);//BIT9
		//DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION2 FOTRUN_BIT true\r\n")));
	}
	/*else
	{
		DEBUGMSG(TRUE, (TEXT("Send_MODE_OPTION2 FOTRUN_BIT false\r\n")));
	}*/
	

	if(bSPI)
		getSPI()->Send_MODE_OPTION2(wMode);
	if(bSerial)
		getSERIAL()->Send_MODE_OPTION2(wMode);

	if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_MODE_OPTION2,wMode);

	return wMode;
}

/**********************************************************************************************//**
 * Sends a vent mode
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	mode	The mode.
 **************************************************************************************************/

void CMVModel::Send_VENT_MODE(eVentMode mode)
{
	//DEBUGMSG(TRUE, (TEXT("CMVModel::Send_VENT_MODE start\r\n")));
	getSERIAL()->Send_VENT_MODE(mode);
	getSPI()->Send_VENT_MODE(mode);
	
	if(getAcuLink()!=NULL)
	{
		getAcuLink()->setParaData(ALINK_SETT_VENT_MODE,(int)mode);
		getAcuLink()->setParaData(ALINK_SETT_VENTRANGE,getCONFIG()->GetVentRange());
	}
		
	//serial
	eAlarmLimitState stateApnoe = getALARMHANDLER()->getAlimitState_ApnoeLimit();
	int iApnoeLimit=0;

	if(stateApnoe==AL_ON || stateApnoe==AL_AUTO)
	{
		iApnoeLimit=getALARMHANDLER()->getAlimitApnoe();
	}

	int iIFlow=0;
	int iEFlow=0;
	int iRisetime=0;
	int iPEEP=0;
	int iITime=0;
	int iETime=0;
	int iBPM=0;
	int iVLimit=0;
	int iVGarant=0;
	int iPmax=0;//newVG

	if(getVMODEHANDLER()->activeModeIsIPPV())
	{
		iIFlow=(int)getDATAHANDLER()->PARADATA()->GetIFlowPara_IPPV();
		iEFlow=(int)getDATAHANDLER()->PARADATA()->GetEFLOWPara_IPPV();
		iRisetime=(int)getDATAHANDLER()->PARADATA()->GetRisetimePara_IPPV();
		iPEEP=(int)getDATAHANDLER()->PARADATA()->GetPEEPPara_IPPV();
		iITime=(int)getDATAHANDLER()->PARADATA()->GetITimePara_IPPV();
		iETime=(int)getDATAHANDLER()->PARADATA()->GetETIMEPara_IPPV();
		iBPM=(int)getDATAHANDLER()->PARADATA()->GetBPMPara_IPPV();
		iVLimit=(int)getDATAHANDLER()->PARADATA()->GetVLimitParam_IPPV();
		iVGarant=(int)getDATAHANDLER()->PARADATA()->GetVGarantPara_IPPV();
		iPmax=(int)getDATAHANDLER()->PARADATA()->GetPmaxVolGPara_IPPV(); //newVG
	}
	else
	{
		iIFlow=(int)getDATAHANDLER()->PARADATA()->GetIFlowPara_TRIGGER();
		iEFlow=(int)getDATAHANDLER()->PARADATA()->GetEFLOWPara_TRIGGER();
		iRisetime=(int)getDATAHANDLER()->PARADATA()->GetRisetimePara_TRIGGER();
		iPEEP=(int)getDATAHANDLER()->PARADATA()->GetPEEPPara_TRIGGER();
		iITime=(int)getDATAHANDLER()->PARADATA()->GetITimePara_TRIGGER();
		iETime=(int)getDATAHANDLER()->PARADATA()->GetETIMEPara_TRIGGER();
		iBPM=(int)getDATAHANDLER()->PARADATA()->GetBPMPara_TRIGGER();
		iVLimit=(int)getDATAHANDLER()->PARADATA()->GetVLimitParam_TRIGGER();
		iVGarant=(int)getDATAHANDLER()->PARADATA()->GetVGarantPara_TRIGGER();
		iPmax=(int)getDATAHANDLER()->PARADATA()->GetPmaxVolGPara_TRIGGER(); //newVG
	}
	int iTherapieFlow=(int)getDATAHANDLER()->PARADATA()->GetTherapieFLOWPara();
	int iPinsp=(int)getDATAHANDLER()->GetActiveModePINSPPara();//newVG
	

	SHORT iO2=0;
	bool bPRICOrunning=false;
	if(getPRICOThread())
	{
		if(getPRICOThread()->isPRICOalgorithmRunning())
		{
			bPRICOrunning=true;
		}
	}
	if(bPRICOrunning)
	{
		iO2=getPRICOThread()->getCalculatedOxyValue();
	}
	else
	{
		if(isO2FlushActive())
			iO2=(SHORT)getDATAHANDLER()->PARADATA()->GetO2FlushPara();
		else
			iO2=(SHORT)getDATAHANDLER()->PARADATA()->GetO2Para();
	}
	
	
	int iPpsv=(int)getDATAHANDLER()->PARADATA()->GetPpsvPara();
	int iTrigger=0;
	if(getVMODEHANDLER()->activeModeIsNCPAP())
	{
		iTrigger=(int)getDATAHANDLER()->PARADATA()->GetTriggerPara_NCPAP(); 
	}
	else if(getVMODEHANDLER()->activeModeIsDUOPAP())
	{
		iTrigger=(int)getDATAHANDLER()->PARADATA()->GetTriggerPara_DUOPAP(); 
	}
	else if(getVMODEHANDLER()->activeModeIsCPAP())
	{
		iTrigger=(int)getDATAHANDLER()->PARADATA()->GetTriggerPara_CPAP(); 
	}
	else
	{
		iTrigger=(int)getDATAHANDLER()->PARADATA()->GetTriggerPara_CONV();
	}
	int iAbortCriterionPSV=getCONFIG()->GetPercentAbortCriterionPSV();
	int iFlowmin=(int)getDATAHANDLER()->PARADATA()->GetFlowminPara();
	int iCPAP=(int)getDATAHANDLER()->PARADATA()->GetCPAPPara();
	int iPManual=0;
	if(getVMODEHANDLER()->activeModeIsHFO())
		iPManual=(int)getDATAHANDLER()->PARADATA()->GetPManualHFOPara();
	else//CPAP
		iPManual=(int)getDATAHANDLER()->PARADATA()->GetPManualCPAPPara();
	int iBackup=(int)getDATAHANDLER()->PARADATA()->GetBackupPara();
	int iCPAPnmode=(int)getDATAHANDLER()->PARADATA()->GetCPAPNMODEPara();
	int iPManualnmode=(int)getDATAHANDLER()->PARADATA()->GetPManualNMODEPara();
	int iITimeNMODE=(int)getDATAHANDLER()->PARADATA()->GetITimeNMODEPara();
	int iETimeNMODE=(int)getDATAHANDLER()->PARADATA()->GetETIMENMODEPara();
	int iBPMNMODE=(int)getDATAHANDLER()->PARADATA()->GetBPMNMODEPara();
	int iHFAmpl=0;
	if(getDATAHANDLER()->IsActiveModeVGarantStateOn())
		iHFAmpl=(int)getDATAHANDLER()->PARADATA()->GetHFAMPLmaxPara();
	else
		iHFAmpl=(int)getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
	int iHFFreq=(int)getDATAHANDLER()->PARADATA()->GetHFFreqPara();
	int iHFPMean=(int)getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
	int iITimeRec=(int)getDATAHANDLER()->PARADATA()->GetITimeRecPara();
	int iETIMERec=getDATAHANDLER()->GetHF_ETIME_REC(getDATAHANDLER()->PARADATA()->GetFreqRecPara());
	int iFreqRec=getDATAHANDLER()->PARADATA()->GetFreqRecPara();

	if(iETIMERec<2000)
		iETIMERec=2000;
	if(getDATAHANDLER()->isLUNGRECLicenseAvailable()==false && getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
	{
		iETIMERec=2000;
		iFreqRec=0;

		getDATAHANDLER()->PARADATA()->SetFreqRecPara(0,true,true);
	}

	int iHFPMeanRec=(int)getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
	int iHFFlow=(int)getDATAHANDLER()->PARADATA()->GetHFFlowPara();
	int iHFVGarant=(int)getDATAHANDLER()->PARADATA()->GetHFVGarantPara();

	Sleep(0);

	switch(mode)
	{
	case VM_IPPV:
		{
			//serial
			getSERIAL()->Send_PARAVAL_INSP_FLOW(iIFlow);
			getSERIAL()->Send_PARAVAL_RISETIME(iRisetime);
			getSERIAL()->Send_PARAVAL_EXH_FLOW(iEFlow);
			getSERIAL()->Send_PARAVAL_PEEP(iPEEP);
			getSERIAL()->Send_PARAVAL_PINSP(iPinsp);//newVG
			getSERIAL()->Send_PARAVAL_PMAXVG(iPmax);//newVG
			getSERIAL()->Send_PARAVAL_INSP_TIME(iITime);
			getSERIAL()->Send_PARAVAL_EXH_TIME(iETime);
			getSERIAL()->Send_PARAVAL_APNOE_TIME(iApnoeLimit);

			//spi
			getSPI()->Send_PARAVAL_INSP_FLOW(iIFlow);
			getSPI()->Send_PARAVAL_RISETIME(iRisetime);
			getSPI()->Send_PARAVAL_EXH_FLOW(iEFlow);
			getSPI()->Send_PARAVAL_PEEP(iPEEP);
			getSPI()->Send_PARAVAL_PINSP(iPinsp);//newVG
			getSPI()->Send_PARAVAL_PMAXVG(iPmax);//newVG
			getSPI()->Send_PARAVAL_INSP_TIME(iITime);
			getSPI()->Send_PARAVAL_EXH_TIME(iETime);
			getSPI()->Send_PARAVAL_APNOE_TIME(iApnoeLimit);
			getSPI()->Send_PARAVAL_VOLUME_LIMIT(iVLimit);
			getSPI()->Send_PARAVAL_VOLUME_GARANT(iVGarant);

			if(false==bPRICOrunning)//PRICO04
				Send_PARA_OXY_RATIO(iO2, true, true);

			iPpsv=ALINK_NOTVALID;
			iTrigger=ALINK_NOTVALID;
			iAbortCriterionPSV=ALINK_NOTVALID;
			iFlowmin=ALINK_NOTVALID;
			iCPAP=ALINK_NOTVALID;
			iPManual=ALINK_NOTVALID;
			iBackup=ALINK_NOTVALID;
			iCPAPnmode=ALINK_NOTVALID;
			iPManualnmode=ALINK_NOTVALID;
			iITimeNMODE=ALINK_NOTVALID;
			iETimeNMODE=ALINK_NOTVALID;
			iBPMNMODE=ALINK_NOTVALID;
			iHFAmpl=ALINK_NOTVALID;
			iHFFreq=ALINK_NOTVALID;
			iHFPMean=ALINK_NOTVALID;
			iITimeRec=ALINK_NOTVALID;
			iETIMERec=ALINK_NOTVALID;
			iFreqRec=ALINK_NOTVALID;
			iHFPMeanRec=ALINK_NOTVALID;
			iHFFlow=ALINK_NOTVALID;
			iHFVGarant=ALINK_NOTVALID;
		}
		break;
	case VM_PSV:
		{
			//serial
			getSERIAL()->Send_PARAVAL_INSP_FLOW(iIFlow);
			getSERIAL()->Send_PARAVAL_EXH_FLOW(iEFlow);
			getSERIAL()->Send_PARAVAL_RISETIME(iRisetime);
			getSERIAL()->Send_PARAVAL_PEEP(iPEEP);
			getSERIAL()->Send_PARAVAL_PINSP(iPinsp);//newVG
			getSERIAL()->Send_PARAVAL_PMAXVG(iPmax);//newVG
			getSERIAL()->Send_PARAVAL_P_PSV(iPpsv);
			getSERIAL()->Send_PARAVAL_INSP_TIME(iITime);
			getSERIAL()->Send_PARAVAL_EXH_TIME(iETime);
			getSERIAL()->Send_PARAVAL_APNOE_TIME(iApnoeLimit);

			//spi
			getSPI()->Send_PARAVAL_TRIG_SCHWELLE(iTrigger);
			getSPI()->Send_PARAVAL_INSP_FLOW(iIFlow);
			getSPI()->Send_PARAVAL_EXH_FLOW(iEFlow);
			getSPI()->Send_PARAVAL_RISETIME(iRisetime);
			getSPI()->Send_PARAVAL_PEEP(iPEEP);
			getSPI()->Send_PARAVAL_PINSP(iPinsp);//newVG
			getSPI()->Send_PARAVAL_PMAXVG(iPmax);//newVG
			getSPI()->Send_PARAVAL_P_PSV(iPpsv);
			getSPI()->Send_PARAVAL_INSP_TIME(iITime);
			getSPI()->Send_PARAVAL_EXH_TIME(iETime);
			getSPI()->Send_PARAVAL_APNOE_TIME(iApnoeLimit);
			getSPI()->Send_PARAVAL_VOLUME_LIMIT(iVLimit);
			getSPI()->Send_PARAVAL_VOLUME_GARANT(iVGarant);

			getSPI()->Send_ABORT_CRITERIONPSV(iAbortCriterionPSV);

			if(false==bPRICOrunning)//PRICO04
				Send_PARA_OXY_RATIO(iO2, true, true);

			iFlowmin=ALINK_NOTVALID;
			iCPAP=ALINK_NOTVALID;
			iPManual=ALINK_NOTVALID;
			iBackup=ALINK_NOTVALID;
			iCPAPnmode=ALINK_NOTVALID;
			iPManualnmode=ALINK_NOTVALID;
			iITimeNMODE=ALINK_NOTVALID;
			iETimeNMODE=ALINK_NOTVALID;
			iBPMNMODE=ALINK_NOTVALID;
			iHFAmpl=ALINK_NOTVALID;
			iHFFreq=ALINK_NOTVALID;
			iHFPMean=ALINK_NOTVALID;
			iITimeRec=ALINK_NOTVALID;
			iETIMERec=ALINK_NOTVALID;
			iFreqRec=ALINK_NOTVALID;
			iHFPMeanRec=ALINK_NOTVALID;
			iHFFlow=ALINK_NOTVALID;
			iHFVGarant=ALINK_NOTVALID;
		}
		break;
	case VM_SIMV:
		{
			//serial
			getSERIAL()->Send_PARAVAL_INSP_FLOW(iIFlow);
			getSERIAL()->Send_PARAVAL_EXH_FLOW(iEFlow);
			getSERIAL()->Send_PARAVAL_RISETIME(iRisetime);
			getSERIAL()->Send_PARAVAL_PEEP(iPEEP);
			getSERIAL()->Send_PARAVAL_PINSP(iPinsp);//newVG
			getSERIAL()->Send_PARAVAL_PMAXVG(iPmax);//newVG
			getSERIAL()->Send_PARAVAL_INSP_TIME(iITime);
			getSERIAL()->Send_PARAVAL_EXH_TIME(iETime);
			getSERIAL()->Send_PARAVAL_APNOE_TIME(iApnoeLimit);

			//spi
			getSPI()->Send_PARAVAL_TRIG_SCHWELLE(iTrigger);
			getSPI()->Send_PARAVAL_INSP_FLOW(iIFlow);
			getSPI()->Send_PARAVAL_EXH_FLOW(iEFlow);
			getSPI()->Send_PARAVAL_RISETIME(iRisetime);
			getSPI()->Send_PARAVAL_PEEP(iPEEP);
			getSPI()->Send_PARAVAL_PINSP(iPinsp);//newVG
			getSPI()->Send_PARAVAL_PMAXVG(iPmax);//newVG
			getSPI()->Send_PARAVAL_INSP_TIME(iITime);
			getSPI()->Send_PARAVAL_EXH_TIME(iETime);
			getSPI()->Send_PARAVAL_APNOE_TIME(iApnoeLimit);
			getSPI()->Send_PARAVAL_VOLUME_LIMIT(iVLimit);
			getSPI()->Send_PARAVAL_VOLUME_GARANT(iVGarant);

			if(false==bPRICOrunning)//PRICO04
				Send_PARA_OXY_RATIO(iO2, true, true);

			iPpsv=ALINK_NOTVALID;
			iAbortCriterionPSV=ALINK_NOTVALID;
			iFlowmin=ALINK_NOTVALID;
			iCPAP=ALINK_NOTVALID;
			iPManual=ALINK_NOTVALID;
			iBackup=ALINK_NOTVALID;
			iCPAPnmode=ALINK_NOTVALID;
			iPManualnmode=ALINK_NOTVALID;
			iITimeNMODE=ALINK_NOTVALID;
			iETimeNMODE=ALINK_NOTVALID;
			iBPMNMODE=ALINK_NOTVALID;
			iHFAmpl=ALINK_NOTVALID;
			iHFFreq=ALINK_NOTVALID;
			iHFPMean=ALINK_NOTVALID;
			iITimeRec=ALINK_NOTVALID;
			iETIMERec=ALINK_NOTVALID;
			iFreqRec=ALINK_NOTVALID;
			iHFPMeanRec=ALINK_NOTVALID;
			iHFFlow=ALINK_NOTVALID;
			iHFVGarant=ALINK_NOTVALID;
		}
		break;
	case VM_SIMVPSV:
		{
			//serial
			getSERIAL()->Send_PARAVAL_INSP_FLOW(iIFlow);
			getSERIAL()->Send_PARAVAL_EXH_FLOW(iEFlow);
			getSERIAL()->Send_PARAVAL_RISETIME(iRisetime);
			getSERIAL()->Send_PARAVAL_PEEP(iPEEP);
			getSERIAL()->Send_PARAVAL_PINSP(iPinsp);//newVG
			getSERIAL()->Send_PARAVAL_PMAXVG(iPmax);//newVG
			getSERIAL()->Send_PARAVAL_P_PSV(iPpsv);
			getSERIAL()->Send_PARAVAL_INSP_TIME(iITime);
			getSERIAL()->Send_PARAVAL_EXH_TIME(iETime);
			getSERIAL()->Send_PARAVAL_APNOE_TIME(iApnoeLimit);

			//spi
			getSPI()->Send_PARAVAL_TRIG_SCHWELLE(iTrigger);
			getSPI()->Send_PARAVAL_INSP_FLOW(iIFlow);
			getSPI()->Send_PARAVAL_EXH_FLOW(iEFlow);
			getSPI()->Send_PARAVAL_RISETIME(iRisetime);
			getSPI()->Send_PARAVAL_PEEP(iPEEP);
			getSPI()->Send_PARAVAL_PINSP(iPinsp);//newVG
			getSPI()->Send_PARAVAL_PMAXVG(iPmax);//newVG
			getSPI()->Send_PARAVAL_P_PSV(iPpsv);
			getSPI()->Send_PARAVAL_INSP_TIME(iITime);
			getSPI()->Send_PARAVAL_EXH_TIME(iETime);
			getSPI()->Send_PARAVAL_APNOE_TIME(iApnoeLimit);
			getSPI()->Send_PARAVAL_VOLUME_LIMIT(iVLimit);
			getSPI()->Send_PARAVAL_VOLUME_GARANT(iVGarant);

			getSPI()->Send_ABORT_CRITERIONPSV(iAbortCriterionPSV);

			if(false==bPRICOrunning)//PRICO04
				Send_PARA_OXY_RATIO(iO2, true, true);

			iFlowmin=ALINK_NOTVALID;
			iCPAP=ALINK_NOTVALID;
			iPManual=ALINK_NOTVALID;
			iBackup=ALINK_NOTVALID;
			iCPAPnmode=ALINK_NOTVALID;
			iPManualnmode=ALINK_NOTVALID;
			iITimeNMODE=ALINK_NOTVALID;
			iETimeNMODE=ALINK_NOTVALID;
			iBPMNMODE=ALINK_NOTVALID;
			iHFAmpl=ALINK_NOTVALID;
			iHFFreq=ALINK_NOTVALID;
			iHFPMean=ALINK_NOTVALID;
			iITimeRec=ALINK_NOTVALID;
			iETIMERec=ALINK_NOTVALID;
			iFreqRec=ALINK_NOTVALID;
			iHFPMeanRec=ALINK_NOTVALID;
			iHFFlow=ALINK_NOTVALID;
			iHFVGarant=ALINK_NOTVALID;
		}
		break;
	case VM_SIPPV:
		{
			//serial
			getSERIAL()->Send_PARAVAL_INSP_FLOW(iIFlow);
			getSERIAL()->Send_PARAVAL_EXH_FLOW(iEFlow);
			getSERIAL()->Send_PARAVAL_RISETIME(iRisetime);
			getSERIAL()->Send_PARAVAL_PEEP(iPEEP);
			getSERIAL()->Send_PARAVAL_PINSP(iPinsp);//newVG
			getSERIAL()->Send_PARAVAL_PMAXVG(iPmax);//newVG
			getSERIAL()->Send_PARAVAL_INSP_TIME(iITime);
			getSERIAL()->Send_PARAVAL_EXH_TIME(iETime);
			getSERIAL()->Send_PARAVAL_APNOE_TIME(iApnoeLimit);

			//spi
			getSPI()->Send_PARAVAL_TRIG_SCHWELLE(iTrigger);
			getSPI()->Send_PARAVAL_INSP_FLOW(iIFlow);
			getSPI()->Send_PARAVAL_EXH_FLOW(iEFlow);
			getSPI()->Send_PARAVAL_RISETIME(iRisetime);
			getSPI()->Send_PARAVAL_PEEP(iPEEP);
			getSPI()->Send_PARAVAL_PINSP(iPinsp);//newVG
			getSPI()->Send_PARAVAL_PMAXVG(iPmax);//newVG
			getSPI()->Send_PARAVAL_INSP_TIME(iITime);
			getSPI()->Send_PARAVAL_EXH_TIME(iETime);
			getSPI()->Send_PARAVAL_APNOE_TIME(iApnoeLimit);
			getSPI()->Send_PARAVAL_VOLUME_LIMIT(iVLimit);
			getSPI()->Send_PARAVAL_VOLUME_GARANT(iVGarant);

			if(false==bPRICOrunning)//PRICO04
				Send_PARA_OXY_RATIO(iO2, true, true);

			iPpsv=ALINK_NOTVALID;
			iAbortCriterionPSV=ALINK_NOTVALID;
			iFlowmin=ALINK_NOTVALID;
			iCPAP=ALINK_NOTVALID;
			iPManual=ALINK_NOTVALID;
			iBackup=ALINK_NOTVALID;
			iCPAPnmode=ALINK_NOTVALID;
			iPManualnmode=ALINK_NOTVALID;
			iITimeNMODE=ALINK_NOTVALID;
			iETimeNMODE=ALINK_NOTVALID;
			iBPMNMODE=ALINK_NOTVALID;
			iHFAmpl=ALINK_NOTVALID;
			iHFFreq=ALINK_NOTVALID;
			iHFPMean=ALINK_NOTVALID;
			iITimeRec=ALINK_NOTVALID;
			iETIMERec=ALINK_NOTVALID;
			iFreqRec=ALINK_NOTVALID;
			iHFPMeanRec=ALINK_NOTVALID;
			iHFFlow=ALINK_NOTVALID;
			iHFVGarant=ALINK_NOTVALID;
		}
		break;
	case VM_CPAP:
		{
			//serial
			getSERIAL()->Send_PARAVAL_EXH_FLOW(iFlowmin);
			getSERIAL()->Send_PARAVAL_PEEP(iCPAP);
			getSERIAL()->Send_PARAVAL_PINSP(iPManual);
			getSERIAL()->Send_PARAVAL_APNOE_TIME(iApnoeLimit);
			getSERIAL()->Send_PARAVAL_INSP_TIME(500);
			getSERIAL()->Send_PARAVAL_EXH_TIME(500);

			//spi
			getSPI()->Send_PARAVAL_EXH_FLOW(iFlowmin);
			getSPI()->Send_PARAVAL_PEEP(iCPAP);
			getSPI()->Send_PARAVAL_PINSP(iPManual);
			getSPI()->Send_PARAVAL_APNOE_TIME(iApnoeLimit);
			getSPI()->Send_PARAVAL_BACKUP(iBackup);
			getSPI()->Send_PARAVAL_INSP_TIME(500);
			getSPI()->Send_PARAVAL_EXH_TIME(500);
			getSPI()->Send_PARAVAL_TRIG_SCHWELLE(iTrigger);

			if(false==bPRICOrunning)//PRICO04
				Send_PARA_OXY_RATIO(iO2, true, true);

			iIFlow=ALINK_NOTVALID;
			iRisetime=ALINK_NOTVALID;
			iPEEP=iCPAP;
			iPinsp=iPManual;
			iITime=500;
			iETime=500;
			iBPM=60;
			iVLimit=ALINK_NOTVALID;
			iVGarant=ALINK_NOTVALID;
			iPpsv=ALINK_NOTVALID;
			
			iAbortCriterionPSV=ALINK_NOTVALID;
			iEFlow=iFlowmin;
			iCPAPnmode=ALINK_NOTVALID;
			iPManualnmode=ALINK_NOTVALID;
			iITimeNMODE=ALINK_NOTVALID;
			iETimeNMODE=ALINK_NOTVALID;
			iBPMNMODE=ALINK_NOTVALID;
			iHFAmpl=ALINK_NOTVALID;
			iHFFreq=ALINK_NOTVALID;
			iHFPMean=ALINK_NOTVALID;
			iITimeRec=ALINK_NOTVALID;
			iETIMERec=ALINK_NOTVALID;
			iFreqRec=ALINK_NOTVALID;
			iHFPMeanRec=ALINK_NOTVALID;
			iHFFlow=ALINK_NOTVALID;
			iHFVGarant=ALINK_NOTVALID;
		}
		break;
	case VM_DUOPAP:
		{
			//serial
			getSERIAL()->Send_PARAVAL_PEEP(iCPAPnmode);
			getSERIAL()->Send_PARAVAL_PINSP(iPManualnmode);
			getSERIAL()->Send_PARAVAL_INSP_TIME(iITimeNMODE);
			getSERIAL()->Send_PARAVAL_EXH_TIME(iETimeNMODE);
			getSERIAL()->Send_PARAVAL_APNOE_TIME(iApnoeLimit);

			//spi
			getSPI()->Send_PARAVAL_TRIG_SCHWELLE(iTrigger);
			getSPI()->Send_PARAVAL_PEEP(iCPAPnmode);
			getSPI()->Send_PARAVAL_PINSP(iPManualnmode);
			getSPI()->Send_PARAVAL_INSP_TIME(iITimeNMODE);
			getSPI()->Send_PARAVAL_EXH_TIME(iETimeNMODE);
			getSPI()->Send_PARAVAL_APNOE_TIME(iApnoeLimit);

			if(false==bPRICOrunning)//PRICO04
				Send_PARA_OXY_RATIO(iO2, true, true);

			iIFlow=ALINK_NOTVALID;
			iEFlow=ALINK_NOTVALID;
			iRisetime=ALINK_NOTVALID;
			iPEEP=iCPAPnmode;
			iPinsp=iPManualnmode;
			iITime=iITimeNMODE;
			iETime=iETimeNMODE;
			iBPM=iBPMNMODE;
			iVLimit=ALINK_NOTVALID;
			iVGarant=ALINK_NOTVALID;
			iPpsv=ALINK_NOTVALID;
			iAbortCriterionPSV=ALINK_NOTVALID;
			iFlowmin=ALINK_NOTVALID;
			iCPAP=ALINK_NOTVALID;
			iPManual=ALINK_NOTVALID;
			iBackup=ALINK_NOTVALID;
			iHFAmpl=ALINK_NOTVALID;
			iHFFreq=ALINK_NOTVALID;
			iHFPMean=ALINK_NOTVALID;
			iITimeRec=ALINK_NOTVALID;
			iETIMERec=ALINK_NOTVALID;
			iFreqRec=ALINK_NOTVALID;
			iHFPMeanRec=ALINK_NOTVALID;
			iHFFlow=ALINK_NOTVALID;
			iHFVGarant=ALINK_NOTVALID;
		}
		break;
	case VM_NCPAP:
		{
			//serial
			getSERIAL()->Send_PARAVAL_PEEP(iCPAPnmode);
			getSERIAL()->Send_PARAVAL_PINSP(iPManualnmode);
			getSERIAL()->Send_PARAVAL_APNOE_TIME(iApnoeLimit);

			//spi
			getSPI()->Send_PARAVAL_PINSP(iPManualnmode);
			getSPI()->Send_PARAVAL_PEEP(iCPAPnmode);
			getSPI()->Send_PARAVAL_APNOE_TIME(iApnoeLimit);
			getSPI()->Send_PARAVAL_TRIG_SCHWELLE(iTrigger);

			if(false==bPRICOrunning)//PRICO04
				Send_PARA_OXY_RATIO(iO2, true, true);

			iIFlow=ALINK_NOTVALID;
			iEFlow=ALINK_NOTVALID;
			iRisetime=ALINK_NOTVALID;
			iPEEP=iCPAPnmode;
			iPinsp=iPManualnmode;
			iBPM=ALINK_NOTVALID;
			iITime=ALINK_NOTVALID;
			iETime=ALINK_NOTVALID;
			iVLimit=ALINK_NOTVALID;
			iVGarant=ALINK_NOTVALID;
			iPpsv=ALINK_NOTVALID;
			iAbortCriterionPSV=ALINK_NOTVALID;
			iFlowmin=ALINK_NOTVALID;
			iPManual=ALINK_NOTVALID;
			iBackup=ALINK_NOTVALID;
			iITimeNMODE=ALINK_NOTVALID;
			iETimeNMODE=ALINK_NOTVALID;
			iBPMNMODE=ALINK_NOTVALID;
			iHFAmpl=ALINK_NOTVALID;
			iHFFreq=ALINK_NOTVALID;
			iHFPMean=ALINK_NOTVALID;
			iITimeRec=ALINK_NOTVALID;
			iETIMERec=ALINK_NOTVALID;
			iFreqRec=ALINK_NOTVALID;
			iHFPMeanRec=ALINK_NOTVALID;
			iHFFlow=ALINK_NOTVALID;
			iHFVGarant=ALINK_NOTVALID;
		}
		break;
	case VM_THERAPIE:
		{
			//serial
			getSERIAL()->Send_PARAVAL_EXH_FLOW(iTherapieFlow);
			getSERIAL()->Send_PARAVAL_PEEP(iCPAPnmode);
			getSERIAL()->Send_PARAVAL_PINSP(iPManualnmode);
			getSERIAL()->Send_PARAVAL_APNOE_TIME(0);

			//spi
			getSPI()->Send_PARAVAL_EXH_FLOW(iTherapieFlow);
			getSPI()->Send_PARAVAL_PINSP(iPManualnmode);
			getSPI()->Send_PARAVAL_PEEP(iCPAPnmode);
			getSPI()->Send_PARAVAL_APNOE_TIME(0);

			if(false==bPRICOrunning)//PRICO04
				Send_PARA_OXY_RATIO(iO2, true, true);

			iIFlow=ALINK_NOTVALID;
			iEFlow=iTherapieFlow;
			iRisetime=ALINK_NOTVALID;
			iPEEP=iCPAPnmode;
			iPinsp=iPManualnmode;
			iBPM=ALINK_NOTVALID;
			iITime=ALINK_NOTVALID;
			iETime=ALINK_NOTVALID;
			iVLimit=ALINK_NOTVALID;
			iVGarant=ALINK_NOTVALID;
			iPpsv=ALINK_NOTVALID;
			iTrigger=ALINK_NOTVALID;
			iAbortCriterionPSV=ALINK_NOTVALID;
			iFlowmin=ALINK_NOTVALID;
			iPManual=ALINK_NOTVALID;
			iBackup=ALINK_NOTVALID;
			iITimeNMODE=ALINK_NOTVALID;
			iETimeNMODE=ALINK_NOTVALID;
			iBPMNMODE=ALINK_NOTVALID;
			iHFAmpl=ALINK_NOTVALID;
			iHFFreq=ALINK_NOTVALID;
			iHFPMean=ALINK_NOTVALID;
			iITimeRec=ALINK_NOTVALID;
			iETIMERec=ALINK_NOTVALID;
			iFreqRec=ALINK_NOTVALID;
			iHFPMeanRec=ALINK_NOTVALID;
			iHFFlow=ALINK_NOTVALID;
			iHFVGarant=ALINK_NOTVALID;
			iApnoeLimit=ALINK_NOTVALID;
		}
		break;
	case VM_HFO:
		{
			//serial
			getSERIAL()->Send_PARAVAL_PINSP(iPManual);
			getSERIAL()->Send_PARAVAL_HF_AMPL(iHFAmpl);
			getSERIAL()->Send_PARAVAL_HF_FREQ(iHFFreq);
			getSERIAL()->Send_PARAVAL_HF_PMITT(iHFPMean);
			getSERIAL()->Send_PARAVAL_APNOE_TIME(iApnoeLimit);
			getSERIAL()->Send_PARAVAL_INSP_FLOW(iHFFlow);
			getSERIAL()->Send_PARAVAL_EXH_TIME(iETIMERec/1000);
			getSERIAL()->Send_PARAVAL_HF_ITIME_REC(iITimeRec);
			getSERIAL()->Send_PARAVAL_HF_PMEANREC(iHFPMeanRec);

			//spi
			getSPI()->Send_PARAVAL_HF_AMPL(iHFAmpl);
			getSPI()->Send_PARAVAL_HF_FREQ(iHFFreq);
			getSPI()->Send_PARAVAL_PINSP(iPManual);
			getSPI()->Send_PARAVAL_HF_PMITT(iHFPMean);
			getSPI()->Send_PARAVAL_APNOE_TIME(iApnoeLimit);
			getSPI()->Send_PARAVAL_VOLUME_GARANT(iHFVGarant);

			if(false==bPRICOrunning)//PRICO04
				Send_PARA_OXY_RATIO(iO2, true, true);

			iIFlow=iHFFlow;
			iEFlow=ALINK_NOTVALID;
			iRisetime=ALINK_NOTVALID;
			iPEEP=ALINK_NOTVALID;
			iPinsp=iPManual;
			if(getDATAHANDLER()->isLUNGRECLicenseAvailable())
			{
				if(iFreqRec!=0)
				{
					iITime=iITimeRec;
					iETime=iETIMERec/100;
					iBPM=iFreqRec;
				}
				else
				{
					iITime=ALINK_OFF;
					iETime=ALINK_OFF;
					iBPM=ALINK_OFF;
					iHFPMeanRec=ALINK_OFF;
				}
				
			}
			else
			{
				iITime=ALINK_NOTVALID;
				iETime=ALINK_NOTVALID;
				iBPM=ALINK_NOTVALID;
				iHFPMeanRec=ALINK_NOTVALID;
			}
			
			iVLimit=ALINK_NOTVALID;
			iVGarant=iHFVGarant;
			iPpsv=ALINK_NOTVALID;
			iTrigger=ALINK_NOTVALID;
			iAbortCriterionPSV=ALINK_NOTVALID;
			iFlowmin=ALINK_NOTVALID;
			iCPAP=ALINK_NOTVALID;
			iBackup=ALINK_NOTVALID;
			iCPAPnmode=ALINK_NOTVALID;
			iPManualnmode=ALINK_NOTVALID;
			iITimeNMODE=ALINK_NOTVALID;
			iETimeNMODE=ALINK_NOTVALID;
			iBPMNMODE=ALINK_NOTVALID;
		}
		break;
	default:
		{

		}
		break;
	}

	Sleep(0);

	if(getAcuLink()!=NULL)
	{
		if(		getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
			||	getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF)
		{
			getAcuLink()->setParaData(ALINK_SETT_STATE_FLOWSENSOR,0);
		}
		else
		{
			getAcuLink()->setParaData(ALINK_SETT_STATE_FLOWSENSOR,1);
		}
		if(getDATAHANDLER()->GetO2SensorState()==OXYSENS_OFF)
		{
			getAcuLink()->setParaData(ALINK_SETT_STATE_OXYSENSOR,0);
		}
		else
		{
			getAcuLink()->setParaData(ALINK_SETT_STATE_OXYSENSOR,1);
		}
		getAcuLink()->setParaData(ALINK_SETT_EXH_FLOW,iEFlow);

		eCurveForm form = getCONFIG()->GetCurPressureRiseCtrl();

		switch(form)
		{
		case CURVE_IFLOW:
			{
				if(getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_IVOI)
					getAcuLink()->setParaData(ALINK_SETT_INSP_FLOW,iIFlow/10);
				else
					getAcuLink()->setParaData(ALINK_SETT_INSP_FLOW,iIFlow);
				getAcuLink()->setParaData(ALINK_SETT_RISETIME,ALINK_NOTVALID);
			}
			break;
		case CURVE_LINEAR:
			{
				getAcuLink()->setParaData(ALINK_SETT_INSP_FLOW,ALINK_NOTVALID);
				if(getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_IVOI)
					getAcuLink()->setParaData(ALINK_SETT_RISETIME,iRisetime/10);
				else
					getAcuLink()->setParaData(ALINK_SETT_RISETIME,iRisetime);
			}
			break;
		case CURVE_AUTOFLOW:
			{
				getAcuLink()->setParaData(ALINK_SETT_INSP_FLOW,ALINK_NOTVALID);
				getAcuLink()->setParaData(ALINK_SETT_RISETIME,ALINK_NOTVALID);
			}
			break;
		}

		getAcuLink()->setParaData(ALINK_SETT_PMANUAL,iPManual);
		getAcuLink()->setParaData(ALINK_SETT_PDUO,iPManualnmode);
		getAcuLink()->setParaData(ALINK_SETT_PEEP,iPEEP);

		if(getDATAHANDLER()->IsCurrentModeVGarantStateOn())//newVG
			getAcuLink()->setParaData(ALINK_SETT_P_INSP,iPmax);
		else
			getAcuLink()->setParaData(ALINK_SETT_P_INSP,iPinsp);
	
		getAcuLink()->setParaData(ALINK_SETT_INSP_TIME,iITime);
		getAcuLink()->setParaData(ALINK_SETT_FREQ,iBPM);
		getAcuLink()->setParaData(ALINK_SETT_EXH_TIME,iETime);

		getAcuLink()->setParaData(ALINK_SETT_FLUSHTIME,getDATAHANDLER()->GetCurO2FlushTime());
		getAcuLink()->setParaData(ALINK_SETT_O2FLUSH,getDATAHANDLER()->PARADATA()->GetO2FlushPara());

		if(mode!=VM_NCPAP && mode!=VM_CPAP && mode!=VM_HFO && mode!=VM_THERAPIE)
		{
			getDATAHANDLER()->CalculateIERatioParts();
		}
		else
		{
			getAcuLink()->setParaData(ALINK_SETT_IERATIO_IPART,ALINK_NOTVALID);
			getAcuLink()->setParaData(ALINK_SETT_IERATIO_EPART,ALINK_NOTVALID);

		}
		
		// will be send by Send_PARA_OXY_RATIO(iO2, true, true);
		//if(false==bPRICOrunning)//PRICO04
		//	getAcuLink()->setParaData(ALINK_SETT_OXY,iO2);
		

		if(getDATAHANDLER()->IsActiveModeVGarantStateOn()==true)
			getAcuLink()->setParaData(ALINK_SETT_VOLUMEGARANT,iVGarant);
		else
			getAcuLink()->setParaData(ALINK_SETT_VOLUMEGARANT,ALINK_OFF);

		if(getDATAHANDLER()->IsActiveModeVLimitStateOn()==true)
			getAcuLink()->setParaData(ALINK_SETT_VOLUMELIMIT,iVLimit);
		else
			getAcuLink()->setParaData(ALINK_SETT_VOLUMELIMIT,ALINK_OFF);


		getAcuLink()->setParaData(ALINK_SETT_P_PSV,iPpsv);
		getAcuLink()->setParaData(ALINK_SETT_TRIG_SCHWELLE,iTrigger);
		getAcuLink()->setParaData(ALINK_SETT_ABORT_CRITERIONPSV,iAbortCriterionPSV);
		getAcuLink()->setParaData(ALINK_SETT_BACKUP,iBackup);
		getAcuLink()->setParaData(ALINK_SETT_HF_AMPL,iHFAmpl);
		getAcuLink()->setParaData(ALINK_SETT_HF_FREQ,iHFFreq);
		getAcuLink()->setParaData(ALINK_SETT_HF_PMITT,iHFPMean);
		getAcuLink()->setParaData(ALINK_SETT_HF_PMEANREC,iHFPMeanRec);
		getAcuLink()->setParaData(ALINK_SETT_FILTER_VOLGARANTY,ALINK_NOTVALID);

		if(mode!=VM_THERAPIE && mode!=VM_HFO && getDATAHANDLER()->IsFlowSensorStateOff()==false)
		{
			if(getALARMHANDLER()->getAlimitState_ApnoeLimit()!=AL_OFF)
				getAcuLink()->setParaData(ALINK_SETT_APNOE_TIME,iApnoeLimit);
			else
				getAcuLink()->setParaData(ALINK_SETT_APNOE_TIME,ALINK_OFF);
		}
		else
		{
			getAcuLink()->setParaData(ALINK_SETT_APNOE_TIME,ALINK_NOTVALID);
		}
		

		if(mode!=VM_NCPAP && mode!=VM_DUOPAP && mode!=VM_THERAPIE && getDATAHANDLER()->IsFlowSensorStateOff()==false)
		{
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MVMAX,getALARMHANDLER()->getAlimitMVmax());
			//getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MVMIN,getALARMHANDLER()->getAlimitMVmin());

			if(getALARMHANDLER()->getAlimitState_MVminLimit()==AL_OFF)
			{
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MVMIN,ALINK_OFF);
			}
			else
			{
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MVMIN,getALARMHANDLER()->getAlimitMVmin());
			}
		}
		else
		{
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MVMAX,ALINK_NOTVALID);
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MVMIN,ALINK_NOTVALID);
		}
		
		if(mode==VM_HFO)
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PIPMAX,ALINK_NOTVALID);
		else
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PIPMAX,getALARMHANDLER()->getAlimitPIPmax());

		if(mode==VM_NCPAP || mode==VM_DUOPAP || mode==VM_THERAPIE || mode==VM_HFO || mode==VM_CPAP)
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PIPMIN,ALINK_NOTVALID);
		else
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PIPMIN,getALARMHANDLER()->getAlimitPIPmin());
		
		if(mode!=VM_HFO)
		{
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PEEPMIN,getALARMHANDLER()->getAlimitPEEPmin());
		}
		else
		{
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PEEPMIN,ALINK_NOTVALID);
		}

		
		if(mode!=VM_NCPAP && mode!=VM_DUOPAP && mode!=VM_THERAPIE && mode!=VM_HFO && getDATAHANDLER()->IsFlowSensorStateOff()==false)
		{
			if(getALARMHANDLER()->getAlimitState_BPMmaxLimit()!=AL_OFF)
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FREQMAX,getALARMHANDLER()->getAlimitBPMmax());
			else
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FREQMAX,ALINK_OFF);
			
		}
		else
		{
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FREQMAX,ALINK_NOTVALID);
		}

		if(mode!=VM_NCPAP && mode!=VM_DUOPAP && mode!=VM_THERAPIE && getDATAHANDLER()->IsFlowSensorStateOff()==false)
		{
			if(getALARMHANDLER()->getAlimitState_LeakmaxLimit()!=AL_OFF)
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_LEAKMAX,getALARMHANDLER()->getAlimitLeakmax());
			else
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_LEAKMAX,ALINK_OFF);
		}
		else
		{
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_LEAKMAX,ALINK_NOTVALID);
		}
		
		

		if(mode==VM_HFO)
		{
			if(getDATAHANDLER()->IsFlowSensorStateOff()==true)
			{
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MAX,ALINK_OFF);
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MIN,ALINK_OFF);
			}
			else if(getALARMHANDLER()->getAlimitState_DCO2maxLimit()==AL_OFF)
			{
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MAX,ALINK_OFF);
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MIN,ALINK_OFF);
			}
			else
			{
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MAX,getALARMHANDLER()->getAlimitDCO2max());
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MIN,getALARMHANDLER()->getAlimitDCO2min());
			}
			
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MAPMAX,getALARMHANDLER()->getAlimitMAPmax());
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MAPMIN,getALARMHANDLER()->getAlimitMAPmin());
		}
		else
		{
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MAX,ALINK_NOTVALID);
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_DCO2MIN,ALINK_NOTVALID);

			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MAPMAX,ALINK_NOTVALID);
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_MAPMIN,ALINK_NOTVALID);
		}

		if(getCONFIG()->getCO2module()!=CO2MODULE_NONE)
		{
			if(getALARMHANDLER()->getAlimitState_ETCO2maxLimit()==AL_OFF)
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MAX, ALINK_OFF);
			else
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MAX,getALARMHANDLER()->getAlimitETCO2max());

			if(getALARMHANDLER()->getAlimitState_ETCO2minLimit()==AL_OFF)
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MIN, ALINK_OFF);
			else
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MIN,getALARMHANDLER()->getAlimitETCO2min());

			if(getALARMHANDLER()->getAlimitState_FICO2maxLimit()==AL_OFF)
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MAX, ALINK_OFF);
			else
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MAX,getALARMHANDLER()->getAlimitFICO2max());

			if(getALARMHANDLER()->getAlimitState_FICO2minLimit()==AL_OFF)
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MIN, ALINK_OFF);
			else
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MIN,getALARMHANDLER()->getAlimitFICO2min());
		}
		else
		{
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MAX,ALINK_NOTVALID);
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_ETCO2MIN,ALINK_NOTVALID);
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MAX,ALINK_NOTVALID);
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_FICO2MIN,ALINK_NOTVALID);
		}

		if(getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
		{
			if(getPRICOThread())
			{
				if(getPRICOThread()->isPRICOalgorithmRunning())
				{
					getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2LOW, getDATAHANDLER()->getPRICO_SPO2lowRange());		
					getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2HIGH, getDATAHANDLER()->getPRICO_SPO2highRange());
					getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2LOW, getDATAHANDLER()->getPRICO_FIO2lowRange());
					getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2HIGH, getDATAHANDLER()->getPRICO_FIO2highRange());
				}
				else
				{
					getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2LOW, ALINK_NOTVALID);		
					getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2HIGH, ALINK_NOTVALID);
					getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2LOW, ALINK_NOTVALID);
					getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2HIGH, ALINK_NOTVALID);
				}
			}
			else
			{
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2LOW, ALINK_NOTVALID);		
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2HIGH, ALINK_NOTVALID);
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2LOW, ALINK_NOTVALID);
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2HIGH, ALINK_NOTVALID);
			}

			if(getALARMHANDLER()->getAlimitState_SPO2_SIQminLimit()==AL_OFF)
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2SIQMIN, ALINK_OFF);
			else
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2SIQMIN, getALARMHANDLER()->getAlimitSPO2_SIQmin());
			
			if(getALARMHANDLER()->getAlimitState_SPO2maxLimit()==AL_OFF)
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2MAX, ALINK_OFF);
			else
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2MAX, getALARMHANDLER()->getAlimitSPO2max());

			if(getALARMHANDLER()->getAlimitState_SPO2minLimit()==AL_OFF)
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2MIN, ALINK_OFF);
			else
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2MIN, getALARMHANDLER()->getAlimitSPO2min());

			if(getALARMHANDLER()->getAlimitState_SPO2_PIminLimit()==AL_OFF)
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2PIMIN, ALINK_OFF);
			else
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2PIMIN, getALARMHANDLER()->getAlimitSPO2_PImin());

			if(getALARMHANDLER()->getAlimitState_PulseRatemaxLimit()==AL_OFF)
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PULSERATEMAX, ALINK_OFF);
			else
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PULSERATEMAX, getALARMHANDLER()->getAlimitPulseRatemax());

			if(getALARMHANDLER()->getAlimitState_PulseRateminLimit()==AL_OFF)
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PULSERATEMIN, ALINK_OFF);
			else
				getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PULSERATEMIN, getALARMHANDLER()->getAlimitPulseRatemin());
		}
		else
		{
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2LOW, ALINK_NOTVALID);		
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_SPO2HIGH, ALINK_NOTVALID);
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2LOW, ALINK_NOTVALID);
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PRICO_FIO2HIGH, ALINK_NOTVALID);

			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2SIQMIN, ALINK_NOTVALID);
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2MAX, ALINK_NOTVALID);
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2MIN, ALINK_NOTVALID);
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_SPO2PIMIN, ALINK_NOTVALID);
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PULSERATEMAX, ALINK_NOTVALID);
			getAcuLink()->setParaData(ALINK_SETT_ALIMIT_PULSERATEMIN, ALINK_NOTVALID);
		}
	}

	Send_MODE_OPTION1();

	if(isVentModeInitialized()==false)
	{
		setVentModeInitialized();
	}
}

/**********************************************************************************************//**
 * Query if this instance is vent mode initialized
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if vent mode initialized, false if not.
 **************************************************************************************************/

bool CMVModel::isVentModeInitialized()
{
	bool bInitialized=false;

	EnterCriticalSection(&m_csVentModeInit);
	bInitialized=m_bVentModeInitialized;
	LeaveCriticalSection(&m_csVentModeInit);

	return bInitialized;
}

/**********************************************************************************************//**
 * Sets vent mode initialized
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::setVentModeInitialized()
{
	EnterCriticalSection(&m_csVentModeInit);
	m_bVentModeInitialized=true;
	LeaveCriticalSection(&m_csVentModeInit);
}

/**********************************************************************************************//**
 * Sends a para volume limit
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_VOLUME_LIMIT(int iVal, bool bSerial, bool bSPI)
{
	/*if(bSerial)
	{

	}*/

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_VOLUME_LIMIT(iVal);
	}

	if(getAcuLink()!=NULL)
	{
		if(getDATAHANDLER()->IsCurrentModeVLimitStateOn()==true)
			getAcuLink()->setParaData(ALINK_SETT_VOLUMELIMIT,iVal);
		else
			getAcuLink()->setParaData(ALINK_SETT_VOLUMELIMIT,ALINK_OFF);
	}

}

/**********************************************************************************************//**
 * Sends a para volume garant
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_VOLUME_GARANT(int iVal, bool bSerial, bool bSPI)
{
	/*if(bSerial)
	{

	}*/

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_VOLUME_GARANT(iVal);
	}

	if(getAcuLink()!=NULL)
	{
		if(getDATAHANDLER()->IsActiveModeVGarantStateOn()==true)
			getAcuLink()->setParaData(ALINK_SETT_VOLUMEGARANT,iVal);
		else
			getAcuLink()->setParaData(ALINK_SETT_VOLUMEGARANT,ALINK_OFF);
	}
}

/**********************************************************************************************//**
 * Sends a para trig schwelle
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_TRIG_SCHWELLE(int iVal, bool bSerial, bool bSPI)
{
	if(bSerial)
	{

	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_TRIG_SCHWELLE(iVal);
	}

	if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_TRIG_SCHWELLE,iVal);
}

/**********************************************************************************************//**
 * Sends a para risetime
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_RISETIME(int iVal, bool bSerial, bool bSPI)
{
	if(bSerial)
	{
		getSERIAL()->Send_PARAVAL_RISETIME(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_RISETIME(iVal);
	}

	if(getAcuLink()!=NULL)
	{
		eCurveForm form = getCONFIG()->GetCurPressureRiseCtrl();

		switch(form)
		{
		case CURVE_IFLOW:
			{
				getAcuLink()->setParaData(ALINK_SETT_RISETIME,ALINK_NOTVALID);
			}
			break;
		case CURVE_LINEAR:
			{
				if(getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_IVOI)
					getAcuLink()->setParaData(ALINK_SETT_RISETIME,iVal/10);
				else
					getAcuLink()->setParaData(ALINK_SETT_RISETIME,iVal);
			}
			break;
		case CURVE_AUTOFLOW:
			{
				getAcuLink()->setParaData(ALINK_SETT_RISETIME,ALINK_NOTVALID);
			}
			break;
		}
	}
}

/**********************************************************************************************//**
 * Sends a para peep
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_PEEP(int iVal, bool bSerial, bool bSPI)
{
	if(bSerial)
	{
		getSERIAL()->Send_PARAVAL_PEEP(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_PEEP(iVal);
	}

	if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_PEEP,iVal);
}

/**********************************************************************************************//**
 * Sends a para ppsv
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_P_PSV(int iVal, bool bSerial, bool bSPI)
{
	if(bSerial)
	{
		getSERIAL()->Send_PARAVAL_P_PSV(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_P_PSV(iVal);
	}

	if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_P_PSV,iVal);
}

/**********************************************************************************************//**
 * Sends a para pinsp
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_PINSP(int iVal, bool bSerial, bool bSPI)//newVG
{
	if(bSerial)
	{
		getSERIAL()->Send_PARAVAL_PINSP(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_PINSP(iVal);
	}

	if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_P_INSP,iVal);
}

/**********************************************************************************************//**
 * Sends a para pmaxvg
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_PMAXVG(int iVal, bool bSerial, bool bSPI)//newVG
{
	if(bSerial)
	{
		getSERIAL()->Send_PARAVAL_PMAXVG(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_PMAXVG(iVal);
	}

	if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_P_INSP,iVal);
}

/**********************************************************************************************//**
 * Sends a para oxy ratio
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_OXY_RATIO(SHORT iVal, bool bSerial, bool bSPI)
{
	if(bSerial)
	{
		getSERIAL()->Send_PARAVAL_OXY_RATIO(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_OXY_RATIO(iVal);
	}

	if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_OXY,iVal);

	if(getCONFIG()->getCO2module()!=CO2MODULE_NONE && getETCO2()!=NULL)
	{
		getETCO2()->set_GasCompensation(iVal);
	}
}

/**********************************************************************************************//**
 * Sends a para insp time
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_INSP_TIME(int iVal, bool bSerial, bool bSPI)
{
	if(bSerial)
	{
		getSERIAL()->Send_PARAVAL_INSP_TIME(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_INSP_TIME(iVal);
	}

	if(getAcuLink()!=NULL)
	{
		getAcuLink()->setParaData(ALINK_SETT_INSP_TIME,iVal);
	}
}

/**********************************************************************************************//**
 * Sends a para insp flow
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_INSP_FLOW(int iVal, bool bSerial, bool bSPI)
{
	//test fmea
	/*iVal=iVal-1000;
	DEBUGMSG(TRUE, (TEXT("CMVModel::Send_PARA_INSP_FLOW %d\r\n"),iVal));*/
	if(bSerial)
	{
		getSERIAL()->Send_PARAVAL_INSP_FLOW(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_INSP_FLOW(iVal);
	}

	if(getAcuLink()!=NULL)
	{
		eCurveForm form = getCONFIG()->GetCurPressureRiseCtrl();

		switch(form)
		{
		case CURVE_IFLOW:
			{
				if(getCONFIG()->GetPDMSprotocol()==ACL_SERIAL_IVOI)
					getAcuLink()->setParaData(ALINK_SETT_INSP_FLOW,iVal/10);
				else
					getAcuLink()->setParaData(ALINK_SETT_INSP_FLOW,iVal);
			}
			break;
		case CURVE_LINEAR:
			{
				getAcuLink()->setParaData(ALINK_SETT_INSP_FLOW,ALINK_NOTVALID);
			}
			break;
		case CURVE_AUTOFLOW:
			{
				getAcuLink()->setParaData(ALINK_SETT_INSP_FLOW,ALINK_NOTVALID);
			}
			break;
		}
	}
}

/**********************************************************************************************//**
 * Sends a para hf pmitt
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_HF_PMITT(int iVal, bool bSerial, bool bSPI)
{
	if(bSerial)
	{
		getSERIAL()->Send_PARAVAL_HF_PMITT(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_HF_PMITT(iVal);
	}

	if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_HF_PMITT,iVal);
}

/**********************************************************************************************//**
 * Sends a para hf pmeanrec
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_HF_PMEANREC(int iVal, bool bSerial, bool bSPI)
{
	if(bSerial)
	{
		getSERIAL()->Send_PARAVAL_HF_PMEANREC(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_HF_PMEANREC(iVal);
	}

	if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_HF_PMEANREC,iVal);
}

/**********************************************************************************************//**
 * Sends a para hf itime record
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void  CMVModel::Send_PARA_HF_ITIME_REC(int iVal, bool bSerial, bool bSPI)
{
	if(bSerial)
	{
		getSERIAL()->Send_PARAVAL_HF_ITIME_REC(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_HF_ITIME_REC(iVal);
	}

	if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_INSP_TIME,iVal);
}

/**********************************************************************************************//**
 * Sends a para hf frequency record
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void  CMVModel::Send_PARA_HF_FREQ_REC(int iVal, bool bSerial, bool bSPI)
{
	if(bSerial)
	{
		getSERIAL()->Send_PARAVAL_HF_FREQ_REC(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_HF_FREQ_REC(iVal);
	}

	int iETIME=getDATAHANDLER()->GetHF_ETIME_REC(iVal);

	if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_EXH_TIME,iETIME/100);
}

/**********************************************************************************************//**
 * Sends a para hf frequency
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_HF_FREQ(int iVal, bool bSerial, bool bSPI)
{
	if(bSerial)
	{
		getSERIAL()->Send_PARAVAL_HF_FREQ(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_HF_FREQ(iVal);
	}

	if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_HF_FREQ,iVal);
}

/**********************************************************************************************//**
 * Sends a para hf ampl
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_HF_AMPL(int iVal, bool bSerial, bool bSPI)
{
	if(bSerial)
	{
		getSERIAL()->Send_PARAVAL_HF_AMPL(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_HF_AMPL(iVal);
	}

	if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_HF_AMPL,iVal);
}

/**********************************************************************************************//**
 * Sends a para exh time
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_EXH_TIME(int iVal, bool bSerial, bool bSPI)
{
	if(bSerial)
	{
		getSERIAL()->Send_PARAVAL_EXH_TIME(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_EXH_TIME(iVal);
	}

	if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_EXH_TIME,iVal);
}

/**********************************************************************************************//**
 * Sends a para exh flow
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_EXH_FLOW(int iVal, bool bSerial, bool bSPI)
{
	if(bSerial)
	{
		getSERIAL()->Send_PARAVAL_EXH_FLOW(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_EXH_FLOW(iVal);
	}

	if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_EXH_FLOW,iVal);
}

/**********************************************************************************************//**
 * Sends a para backup
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_BACKUP(int iVal, bool bSerial, bool bSPI)
{
	if(bSerial)
	{
		//getSERIAL()->Send_PARAVAL_BACKUP(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_BACKUP(iVal);
	}
	if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_BACKUP,iVal);

}

/**********************************************************************************************//**
 * Sends a para apnoe time
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_PARA_APNOE_TIME(int iVal, bool bSerial, bool bSPI)
{
	if(bSerial)
	{
		getSERIAL()->Send_PARAVAL_APNOE_TIME(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_PARAVAL_APNOE_TIME(iVal);
	}

	if(getAcuLink()!=NULL)
	{
		if(getALARMHANDLER()->getAlimitState_ApnoeLimit()!=AL_OFF)
			getAcuLink()->setParaData(ALINK_SETT_APNOE_TIME,iVal);
		else
			getAcuLink()->setParaData(ALINK_SETT_APNOE_TIME,ALINK_OFF);
	}
}

/**********************************************************************************************//**
 * Sends an oxy corr
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_OXY_CORR(int iVal, bool bSerial, bool bSPI)
{
	if(bSerial)
	{
		getSERIAL()->Send_OXYGEN_CORR(iVal);
	}

	if(bSPI)
	{
		//getSPI()->Send_OXYGEN_CORR(iVal);
	}

	/*if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_xxx,iVal);*/
}

//void CMVModel::Send_FILTER_VOLGARANTY(int iVal, bool bSerial, bool bSPI)
//{xxx
//	if(bSerial)
//	{
//		//getSERIAL()->Send_FILTER_VOLUMEGARANTY(iVal);
//	}
//
//	if(bSPI)
//	{
//		getSPI()->Send_FILTER_VOLUMEGARANTY(iVal);
//	}
//
//	if(getAcuLink()!=NULL)
//		getAcuLink()->setParaData(ALINK_SETT_FILTER_VOLGARANTY,iVal);
//}

/**********************************************************************************************//**
 * Sends an abort criterionpsv
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	iVal   	Zero-based index of the value.
 * \param	bSerial	True to serial.
 * \param	bSPI   	True to spi.
 **************************************************************************************************/

void CMVModel::Send_ABORT_CRITERIONPSV(int iVal, bool bSerial, bool bSPI)
{
	if(bSerial)
	{
		//getSERIAL()->Send_FILTER_VOLUMEGARANTY(iVal);
	}

	if(bSPI)
	{
		getSPI()->Send_ABORT_CRITERIONPSV(iVal);
	}

	if(getAcuLink()!=NULL)
		getAcuLink()->setParaData(ALINK_SETT_ABORT_CRITERIONPSV,iVal);
}

/**********************************************************************************************//**
 * Sets bit of byte
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	x	The x coordinate.
 * \param	n	An int to process.
 *
 * \return	A BYTE.
 **************************************************************************************************/

BYTE CMVModel::setBitOfByte(BYTE x, unsigned int n)
{
	x = x | (1 << n);

	return x;
}

/**********************************************************************************************//**
 * Sets bit of word
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	x	A WORD to process.
 * \param	n	An int to process.
 *
 * \return	A WORD.
 **************************************************************************************************/

WORD CMVModel::setBitOfWord(WORD x, unsigned int n)
{
	x = x | (1 << n);

	return x;
}

/**********************************************************************************************//**
 * Sets a bit
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	x	The x coordinate.
 * \param	n	An int to process.
 *
 * \return	An int.
 **************************************************************************************************/

int CMVModel::SetBit(int x, unsigned int n)
{
	//x = x | (1 << n);
	x = x | n;

	return x;
}

/**********************************************************************************************//**
 * Gets adapter information
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	The adapter information.
 **************************************************************************************************/

int CMVModel::GetAdapterInfo()
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	UINT i;

	ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(sizeof (IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL)
	{
		return 1;
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL)
		{
			return 1;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		CString szName=_T("");
		pAdapter = pAdapterInfo;
		while (pAdapter)
		{
			szName=pAdapter->AdapterName;
			bool bEthernet=false;
			switch (pAdapter->Type)
			{
			case MIB_IF_TYPE_OTHER:
				break;
			case MIB_IF_TYPE_ETHERNET:
				bEthernet=true;
				break;
			case MIB_IF_TYPE_TOKENRING:
				break;
			case MIB_IF_TYPE_FDDI:
				break;
			case MIB_IF_TYPE_PPP:
				break;
			case MIB_IF_TYPE_LOOPBACK:
				break;
			case MIB_IF_TYPE_SLIP:
				break;
			default:
				break;
			}
			if(bEthernet && szName==_T("DM9CE1"))
			{
				CStringW szTemp=_T("");
				for (i = 0; i < pAdapter->AddressLength; i++)
				{
					szTemp.Format(_T("%.2X"), (int) pAdapter->Address[i]);
					m_szUniqueID+=szTemp;
				}
			}

			if(bEthernet && szName==_T("DM9CE1"))
			{
				pAdapter=NULL;
			}
			else
				pAdapter = pAdapter->Next;
		}
	}

	if (pAdapterInfo)
		FREE(pAdapterInfo);

	return 0;
}

/**********************************************************************************************//**
 * Gets unique identifier
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	The unique identifier.
 **************************************************************************************************/

CStringA CMVModel::GetUniqueID()
{
	return m_szUniqueID;
}

/**********************************************************************************************//**
 * Sets psvapnoe
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CMVModel::setPSVapnoe(bool bState)
{
	getDATAHANDLER()->setPSVapnoe(bState);
	getVIEWHANDLER()->setPSVapnoe(bState);
}

/**********************************************************************************************//**
 * Gets load hospital settings failed
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVModel::getLoadHospitalSettingsFailed()
{
	return m_bLoadHospitalSettingsFailed;
}

/**********************************************************************************************//**
 * Sets load hospital settings failed
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	failed	True if failed.
 **************************************************************************************************/

void CMVModel::setLoadHospitalSettingsFailed(bool failed)
{
	m_bLoadHospitalSettingsFailed=failed;
}

/**********************************************************************************************//**
 * Saves the hospital settings
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVModel::SaveHospitalSettings()
{
	//load from xml-file from ffsdiks to fram and reload ( config ...)
	//config

	CStringW szFile1=_T("\\FFSDISK\\hospital.dat");
	CFile archivDatei1;
	if(archivDatei1.Open(szFile1,CFile::modeCreate | CFile::modeWrite)!=0)
	{
		CArchive objektArchiv(&archivDatei1, CArchive::store);

		getCONFIG()->SerializeFile(objektArchiv);

		objektArchiv.Close();
		archivDatei1.Close();
	}
	else
		return false;

	/*CStringW szFile2=_T("\\FFSDISK\\hospital2.dat");
	CFile archivDatei2;
	if(archivDatei2.Open(szFile2,CFile::modeCreate | CFile::modeWrite)!=0)
	{
		CArchive objektArchiv(&archivDatei2, CArchive::store);

		getCONFIG()->SerializeFile2(objektArchiv);

		objektArchiv.Close();
		archivDatei2.Close();
	}
	else
		return false;*/



	return true;
}

/**********************************************************************************************//**
 * Loads hospital settings
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVModel::LoadHospitalSettings()
{
	//load from xml-file from ffsdiks to fram and reload ( config ...)
	//config

	CStringW szFile1=_T("\\FFSDISK\\hospital.dat");
	CFile archivDatei1;
	if(archivDatei1.Open(szFile1,CFile::modeRead)!=0)
	{
		CArchive objektArchiv(&archivDatei1, CArchive::load);

		try
		{
			getCONFIG()->SerializeFile(objektArchiv);

			objektArchiv.Close();
			archivDatei1.Close();
		}
		catch (...)
		{
			theApp.ReportException(_T("CMVModel::LoadHospitalSettings"));

			objektArchiv.Close();
			archivDatei1.Close();
			
			if(CTlsFile::Exists(szFile1))
			{
				CTlsFile::Delete(szFile1);
			}

			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_LOADHOSPITALSETTINGS_FAIL);
		}
		return true;
	}
	else
		return false;

	//CStringW szFile2=_T("\\FFSDISK\\hospital2.dat");
	//CFile archivDatei2;
	//if(archivDatei2.Open(szFile2,CFile::modeRead)!=0)
	//{
	//	CArchive objektArchiv(&archivDatei2, CArchive::load);

	//	try
	//	{
	//		getCONFIG()->SerializeFile2(objektArchiv);

	//		objektArchiv.Close();
	//		archivDatei2.Close();
	//	}
	//	catch (...)
	//	{
	//		objektArchiv.Close();
	//		archivDatei2.Close();
	//		//CFabianHFOApp::ReportException(_T("EXCEPTION: CInterfaceI2C::LoadHospitalSettings"));
	//		//AfxMessageBox( _T("EXCEPTION: CMVModel::VentModeChanged()()") );

	//		if(CTlsFile::Exists(szFile2))
	//		{
	//			CTlsFile::Delete(szFile2);
	//		}

	//		if(AfxGetApp() != NULL)
	//			AfxGetApp()->GetMainWnd()->PostMessage(WM_LOADHOSPITALSETTINGS_FAIL);

	//	}


	//	return true;
	//}
	//else
	//	return false;
}

/**********************************************************************************************//**
 * Sends the test data
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::sendTestData()
{
	//test serial
	//GetTERMINAL()->sendTestData();
}

/**********************************************************************************************//**
 * Sends the terminal measurment data
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::sendTerminalMeasurmentData()
{
	//test serial
	//GetTERMINAL()->sendTestMeasurmentData();
}

/**********************************************************************************************//**
 * Sets ventilation range changed
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CMVModel::setVentilationRangeChanged(bool bState)
{
	m_bVentilationRangeChanged=bState;
}

/**********************************************************************************************//**
 * Gets ventilation range changed
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVModel::getVentilationRangeChanged()
{
	return m_bVentilationRangeChanged ;
}

/**********************************************************************************************//**
 * Query if 'oldTickCount' is safe tick count delay expired
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	oldTickCount	Number of old ticks.
 * \param	delay			The delay.
 *
 * \return	True if safe tick count delay expired, false if not.
 **************************************************************************************************/

bool CMVModel::isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay)////used to check if old tick count plus delay is still lower than actual tickCount, (dwLastTickCount+DELAY<getTickCount64())
{
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

/**********************************************************************************************//**
 * Is maintenance needed
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVModel::isMaintenanceNeeded()
{
	COleDateTime dtNextServiceDate=getCONFIG()->getNextServiceDate();

	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	COleDateTime dtCurrentTime(stTime);
	COleDateTimeSpan dtNoticePeriod;
	dtNoticePeriod.SetDateTimeSpan(30,0,0,0); //pro, new

	if (dtNextServiceDate.GetStatus()==COleDateTime::valid)
	{			
		if (dtCurrentTime >= dtNextServiceDate - dtNoticePeriod)
		{
			getVIEWHANDLER()->setMaintenanceFlag(true); 
		} 
		else
		{
			getVIEWHANDLER()->setMaintenanceFlag(false);
		}
	}
	else
	{
		getVIEWHANDLER()->setMaintenanceFlag(false);
	}
}

// Test FMEA
//void CMVModel::closeInterface()
//{
//	/*if(PIF)
//	{
//		PIF->DestroyInstance();
//		PIF=NULL;
//	}*/
//
//	/*if(I2C)
//	{
//		I2C->DestroyInstance();
//		I2C=NULL;
//	}*/
//
//	/*if(SERIAL)
//	{
//		SERIAL->Deinit();
//		SERIAL->DestroyInstance();
//
//		SERIAL=NULL;
//	}*/
//
//	/*if(SPI)
//	{
//		SPI->DestroyInstance();
//		SPI=NULL;
//	}*/
//
//	/*if(DIO)
//	{
//		m_bDIOerror=false;
//		DIO->DestroyInstance();
//		DIO=NULL;
//	}*/
//}
