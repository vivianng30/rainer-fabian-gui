//=============================================================================
/** 
* \file MVModel.h
* \brief Model of Model-View-Controller software architecture.
* \author Rainer Kühner
* \date 2013-01-21 Creation of file
*
* Defines all necessary functions and macros for the model and the observer of 
* Model–View–Controller (MVC). MVC is an architecture that separates the  
* representation of information from the user's interaction with it.
*
*       Copyright (c) 2010 Ing.Büro Rainer Kühner
*                      Illerkirchberg
*
**/
//=============================================================================
#pragma once



#include "afxwin.h"
#include "FabianHFO.h"
#include "MVEvent.h"
#include "globDefs.h"
#include "Configuration.h"
#include "DataHandler.h"
#include "TlsBlob.h"
#include "InterfaceCO2.h"
#include "InterfaceSPO2.h"
#include "InterfaceSerial.h"


#include "InterfaceSPI.h"
#include "InterfaceFSBus.h"
#include "InterfaceDIO.h"
#include "InterfaceI2C.h"
#include "InterfaceAcuLink.h"
#include "InterfaceTerminal.h"
#include "InterfaceCOMport.h"
#include "SoundPlayer.h"
#include "Language.h"
//#include "AlarmHandler.h"
#include "AlarmMgmtHandler.h"
#include "VentModeHandler.h"

#include "ThreadMonitor.h"
#include "ThreadPRICO.h"
#include "ThreadAlarm.h"
#include "ThreadFlowsensor.h"
#include "ThreadExspiration.h"
#include "ThreadFOT.h"

typedef INT (CALLBACK* LP_GET_TEST_ADD_DLLFUNC)(INT, INT);

//!< Class declaration of CMVViewHandler.
class CMVViewHandler;

//=============================================================================
/** 
* \file MVModel.h
* \class CModelObserver
* \brief Observer class of Model-View-Controller software architecture.
* \author Rainer Kühner
* \date 2013-01-21 Creation of file
*
* The observer pattern is a software design pattern in which an object, called the subject,
* maintains a list of its dependents, called observers, and notifies them automatically of
* any state changes, usually by calling one of their methods. The Observer pattern is also
* a key part in the familiar Model View Controller (MVC) architectural pattern.
*
*       Copyright (c) 2010 Ing.Büro Rainer Kühner
*                      Illerkirchberg
*
**/
//=============================================================================
class CModelObserver
{
public:
	/** Functions of CModelObserver.       
	*  These functions notify the attached observers about occurred events.       
	**/
	virtual void NotifyCalculateAlarmLimit();
	virtual void NotifyAlarmLimitChanged();
	virtual void NotifyDataChanged();
	virtual void NotifyExspirationDataChanged();
	virtual void NotifyViewStateChanged();
	virtual void NotifyVentModeChanged();
	virtual void NotifyLanguageChanged();
	virtual void NotifyEvent(CMVEvent* pEvent);
	virtual void NotifyParaBtnEvent(CMVEvent* pEvent);
	virtual void NotifyViewFocusChanged(int iViewID);
};

//=============================================================================
/** 
* \file MVModel.h
* \class CMVModel
* \brief Model class of Model-View-Controller software architecture.
* \author Rainer Kuehner
* \date 2013-01-21 Creation of file
*
* The model notifies its associated views and controllers when there has been a
* change in its state. This notification allows the views to produce updated output,
* and the controllers to change the available set of commands. Events and
* notifications are controlling transitions between different states and views.
* This is done by a Publisher-Subscriber model. The model also controls the mode
* states of the application.
*
*       Copyright (c) 2010 Ing.Buero Rainer Kuehner
*                      Illerkirchberg
*
**/
//=============================================================================
class CMVModel
{
	static CMVModel* theModel;//!< a singleton.

	CMVModel(void);		//!< Constructor.
	~CMVModel(void);	//!< Destructor.

public:

	LP_GET_TEST_ADD_DLLFUNC pGetTestAddFunc;

	static CMVModel* GetInstance();	//!< Return the instance of model (singleton).
	static void DestroyInstance();	//!< Destroy the instance of model (singleton).

	void Init(CStringW szFontName, WORD wLanguageID);	//!< Initialisation.
	void Deinit();
	//!< Deinitialisation.


	CVentModeHandler *getVMODEHANDLER(); //!< Return the instance of VentMode handler (singleton).
	CDataHandler *getDATAHANDLER();		//!< Return the instance of data handler (singleton).
	CAlarmMgmtHandler* getALARMHANDLER();	//!< Return the instance of alarm handler (singleton).
	CSoundPlayer *getSOUND();			//!< Return the instance of sound handler (singleton).
	CMVViewHandler *getVIEWHANDLER();	//!< Return the instance of view handler (singleton).
	CConfiguration *getCONFIG();		//!< Return the instance of coniguration  handler (singleton).
	CLanguage *getLANGUAGE();			//!< Return the instance of language handler (singleton).
	
	CInterfaceTerminal *getTERMINAL();	//!< Return the instance of terminal interface (singleton).
	CInterfaceAcuLink *getAcuLink();	//!< Return the instance of PDMS acuLink (singleton).
	CInterfaceSerial *getSERIAL();		//!< Return the instance of serial interface (singleton).
	CInterfaceSPI *getSPI();			//!< Return the instance of SPI interface (singleton).
	CInterfaceFSBus *getPIF();			//!< Return the instance of parallel interface (singleton).
	CInterfaceDIO *getDIO();			//!< Return the instance of digital IO interface (singleton).
	CInterfaceI2C *getI2C();			//!< Return the instance of I2C interface (singleton).
	CInterfaceCO2 *getETCO2();	//!< Return the instance of etCO2 interface (singleton).
	CInterfaceSPO2 *getSPO2();	//!< Return the instance of SPO2 interface (singleton).
	
	//rkuINITIF
	CThreadMonitor *getMonitorThread();
	CThreadPRICO *getPRICOThread();
	CThreadAlarm *getAlarmThread();
	CThreadFlowsensor *getFlowsensorThread();
	CThreadExspiration *getExspirationThread();
	CThreadFOT *getFOTThread();

	bool isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay=0);//used for (dwLastTickCount+DELAY<getTickCount64())

	/**
	* @brief Attache/detach to observer.
	**/
	void AttachObserver(CModelObserver* pObserver);
	void DetachObserver(CModelObserver* pObserver);

	CStringW GetLanguageString(int nID);
	void setReloadLanguageProgress(bool state);
	bool getReloadLanguageProgress();

	void notifyViewStateChanged();

	bool IsSPIDisconnection();
	bool IsSPI_TUBE_OCCLUSION();

	void triggerEvent(CMVEvent* pEvent=0,int btnID=0, int iGroupID=0);

	bool ParseAlarmStateBytes(int iAlarmState);
	int ParseFlowsensorAlarmStateBytes(int iState);

	CStringW GetMainVersion();
	CStringW GetBuildVersion();

	void SetFontFace(CStringW name);
	CStringW GetFontFace();
	WORD GetLanguageID();
	void SetLanguageID(WORD id);

	void SetO2calFlag();
	bool IsO2calFlag();
	void DeleteO2calFlag();
	bool IsFlowsensorFlag();
	void DeleteFlowsensorFlag();
	
	bool GetVlimittedFlag();
	eVolumeGuarantyAlarm getVGarantyAlarmFlag();

	void NotifyMonitorData();
	void NotifyExspirationStart();
	void NotifyViewFocusChanged(int iViewID);

	eRunState GetVentRunState();
	void SetVentRunState(eRunState state);

	bool reinitSERIAL();
	void setSERIALavailable(bool state);
	bool isSERIALavailable();

	//newSPI
	void setSPIavailability(bool state);
	bool isSPIavailabel();

	void SetDIOavailability(bool state);
	bool IsDIOavailability();

	void SetPIFavailability(bool state);
	bool IsPIFavailability();

	void SetAcuLinkAvailability(bool state);
	bool IsAcuLinkAvailability();

	void activateO2Flush();
	void deactivateO2Flush();
	bool isO2FlushActive();

	void activateMANBREATH();
	void deactivateMANBREATH();
	bool isMANBREATHrunning();

	void notifyVentModeChanged();

	bool ResetExhalValvCalMode();

	void CheckAlarmData();
	void CheckFlowsensorData();

	void SetCountDownAlarmSilent(int iCount);
	int GetCountDownAlarmSilent();
	void StopCountDownAlarmSilent();
	void DrawCountDownAlarmSilent(int iCountDownAlarmSilent);

	void setActiveAlarmDelay(bool bDelay);
	bool isActiveAlarmDelay();

	void SetStateCalibrationView(bool state);
	bool IsCalibrationViewActive();

	void SetAccuTurnoff();
	bool IsAccuTurnoff();

	void StartInstaller();
	bool IsInstaller();

	//void Quit();

	void EnablePatAlarmO2dependend();
	void DisablePatAlarmO2dependend();
	bool IsPatAlarmO2dependendEnabled();

	void SetAlimitStateCalculating(bool state);
	bool CalculateAlarmlimitRunning();

	void SetVideoRunning(bool bState);
	bool IsVideoRunning();

	bool isVentModeInitialized();
	void setVentModeInitialized();

	void Send_VENT_MODE(eVentMode mode);
	WORD Send_MODE_OPTION1(bool bSPI=true,bool bSerial=true);
	WORD Send_MODE_OPTION2(bool bSPI=true,bool bSerial=true);
	BYTE setBitOfByte(BYTE x, unsigned int n);
	WORD setBitOfWord(WORD x, unsigned int n);
	int SetBit(int x, unsigned int n);

	void Send_PARA_VOLUME_LIMIT(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_VOLUME_GARANT(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_TRIG_SCHWELLE(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_RISETIME(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_PEEP(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_P_PSV(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_PINSP(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_PMAXVG(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_OXY_RATIO(SHORT iVal, bool bSerial, bool bSPI);
	void Send_PARA_INSP_TIME(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_INSP_FLOW(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_HF_PMITT(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_HF_PMEANREC(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_HF_ITIME_REC(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_HF_FREQ_REC(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_HF_FREQ(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_HF_AMPL(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_EXH_TIME(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_EXH_FLOW(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_BACKUP(int iVal, bool bSerial, bool bSPI);
	void Send_PARA_APNOE_TIME(int iVal, bool bSerial, bool bSPI);
	void Send_OXY_CORR(int iVal, bool bSerial, bool bSPI);
	void Send_ABORT_CRITERIONPSV(int iVal, bool bSerial, bool bSPI);

	CStringA GetUniqueID();

	void setCO2inprogress(bool bState);
	bool isCO2inprogress();
	
	void setPSVapnoe(bool bState);

	void sendTestData();
	void sendTerminalMeasurmentData();

	bool getLoadHospitalSettingsFailed();
	void setLoadHospitalSettingsFailed(bool failed);
	bool SaveHospitalSettings();
	bool LoadHospitalSettings();

	void setVentilationRangeChanged(bool bState);
	bool getVentilationRangeChanged();

	void initTerminal();
	void deinitTerminal();

	void initCO2module();
	void DeinitCO2module();
	void logCO2module();

	void initSPO2module(bool bReinit=false);
	void DeinitSPO2module(bool bReinit=false);
	bool isSPO2running();
	void logSPO2module();

	void isMaintenanceNeeded();

	void writeLEAKCOMPENSATIONToLog();
	void writeALTITUDEToLog();

	void initFOTthread();
	void deinitFOTthread();

	void initPRICOthread();
	void deinitPRICOthread();

	//bool getI2Cinitialized();

	int _httoi(const TCHAR *value);

protected:
	void checkUniqueID();
	void setNetworkAddressAsUniqueID();
	void writeUniqueIDtoLog();
	bool initAcuLink();
	//bool initAcuLink_V3();
	//bool initAcuLink_V4();
	void initSerialController();
	void startThreads();

	void writeVentRangeToLog();
	void writeBIASFlowStateToLog();
	void writeConfigVersionToLog();
	void writeKernelVersionToLog();
	void writeMainboardVersionToLog();
	//void writeSerialBuiltInToLog();

	void triggerInfoTextEvent(CMVEvent* pEvent);
	void triggerControlEvent(CMVEvent* pEvent);
	void triggerUIevent(CMVEvent* pEvent);
	void triggerMatrixEvent(CMVEvent* pEvent);
	void triggerAlarmEvent(CMVEvent* pEvent);
	void triggerDataEvent(CMVEvent* pEvent);

	void InitViews();
	void InitViewHandler();

	int GetAdapterInfo();
	
	void NotifyEvent(CMVEvent* pEvent);
	void NotifyNewAlarmlimitData();
	void NotifyCalculateAlarmlimitData();
	void NotifyParaBtnEvent(CMVEvent* pEvent);

	DWORD GetTimeDiff(DWORD dwStart);

	void ChangeLanguage();

	//void setI2Cinitialized();

private:
	static CRITICAL_SECTION m_csObservers;
	static CRITICAL_SECTION m_csI2C;
	//static CRITICAL_SECTION m_csI2Cinit;
	static CRITICAL_SECTION m_csSerial;
	static CRITICAL_SECTION	m_csLangString;
	static CRITICAL_SECTION	m_csTrigger;
	static CRITICAL_SECTION	m_csO2Flush;
	static CRITICAL_SECTION	m_csETCO2;
	static CRITICAL_SECTION	m_csSPO2;
	static CRITICAL_SECTION	m_csVentModeInit;

	//rkuINITIF
	static CMVViewHandler *VIEWHANDLER;
	static CDataHandler *DATAHANDLER;
	static CVentModeHandler *MODEHANDLER;
	static CAlarmMgmtHandler* ALARMHANDLER;
	static CSoundPlayer *SOUND;
	static CConfiguration *CONFIG;
	static CLanguage *LANGUAGE;

	static CThreadMonitor *MONITORTHR;
	static CThreadPRICO *PRICOTHR;
	static CThreadFOT *FOTTHR;
	static CThreadAlarm *ALARMTHR;
	static CThreadFlowsensor *FLOWSENSORTHR;
	static CThreadExspiration *EXSPIRATIONTHR;
	
	static CInterfaceTerminal *TERMINAL;
	static CInterfaceAcuLink *ACULINK;
	static CInterfaceSerial *SERIAL;
	static CInterfaceSPI *SPI;
	static CInterfaceFSBus *PIF;
	static CInterfaceDIO *DIO;
	static CInterfaceI2C *I2C;
	static CInterfaceCO2 *ETCO2;
	static CInterfaceSPO2 *SPO2;
	
	static CList <CModelObserver*, CModelObserver*> m_ObserverList;

	eVentMode m_eVentModePrevFOT;
	BYTE m_iBackupPrevFOT;
	eVolumeGuarantyAlarm m_eVGaranty;
	eRunState m_eVentRunState;

	CStringA m_szUniqueID;
	CStringW m_szFontName;
	CStringW m_szVersion;
	CStringW m_szBuildVersion;

	int m_iCountDownAlarmSilent;
	int m_iOldPPSV;

	DWORD m_dwLastVolGarAlarmSound;
	WORD m_wLanguageID;
	
	bool m_bMANBREATHrunning;
	bool m_bO2FlushActive;
	bool m_bSERIALavailability;
	bool m_bSPIavailability;
	bool m_bDIOavailability;
	bool m_bPIFavailability;
	bool m_bAcuLinkAvailability;
	bool m_bVlimitted;
	bool m_bCalibrationViewActive;
	bool m_bLoadHospitalSettingsFailed;
	bool m_bActiveAlarmDelay;
	bool m_bAutoResetFromPreset;
	bool m_bSPIDisconnection;
	bool m_bSPI_TUBE_OCCLUSION;
	bool m_bAccuTurnOff;
	bool m_bInstaller;
	bool m_bExit;
	bool m_bPatAlarmO2dependendEnabled;
	bool m_bO2calFlag;
	bool m_bFlowsensor;
	bool m_bCalculateAlarmlimitRunning;
	bool m_bSPO2running;
	bool m_bVentilationRangeChanged;
	bool m_bVideoRunning;
	bool m_bCO2inprogress;
	bool m_bVentModeInitialized;
	bool m_bReloadLangInProgress;

};
