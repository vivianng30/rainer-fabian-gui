//=============================================================================
/** 
* \class CMainFrame
* 		 
* \brief Main frame of MVC.
* 		 
* \author Rainer Kuehner
* \date 2017-10-23 Creation of file
*
* The main frame handles all windows messages, 
* global fonts and values, timer thread, watchdog thread, oxy calibration thread.
*
**/
//=============================================================================

#pragma once

#include "MVModel.h"
#include "colour.h"
#include "MVViewHandler.h"
#include "DIBSectionLite.h"

#define MAXTASKS 10
#define WATCHDOGPRIO 105



class CMainFrame : public CFrameWnd
{
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

public:
	

// Operations
public:
	

	//void DrawHeader(CStringW cs);

	
	

protected:
	HFONT CreateFontHandle(int nPixHeight, TCHAR* pszFacename, LONG lFontWeight, int iOrientation=0);
	void RegisterFFSDISKFonts();
	void RegisterSDCardFonts();
	void UnregisterFFSDISKFonts();
	void UnregisterSDCardFonts();
	WORD CreateAcuFonts(WORD wLanguageID, bool bSetFaceToModel);
	void LoadGlobalAcuFonts(WORD wLanguageID);

	bool CalOxySensor21();
	bool CalOxySensor100();

	bool isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay=0);//used for (dwLastTickCount+DELAY<getTickCount64())

	void CalculateSingleAutoLimit(eAlarmLimitPara para);

	void QuitVentilator();

	void checkSERIAL();

	//void StartThreadWatchdogThread(void);//rkuNEWFIX
	//void StopThreadWatchdogThread( void );//rkuNEWFIX
	
	void stopThreads();
	void stopWatchdog();

	void StartI2CWatchdogThread(void);
	void StopI2CWatchdogThread( void );

	void StartTimerThread(void);
	void StopTimerThread( void );

	void StartOxyCalThread(void);
	void StopOxyCalThread( void );

	void StartSaveTrendUSBThread(void);
	void StopSaveTrendUSBThread(void);

	void StartDelTrendThread(void);
	void StopDelTrendThread( void );

	bool DeleteTrendData(CStringW sFolder);
	bool SaveTrendDataToUSB(UINT trendID);

	void startAcuLink();
	
	CMVModel *getModel();
	
	

// Overrides
public:
	LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

protected: 
	//CRITICAL_SECTION	csThreadWatchdog;
	static CRITICAL_SECTION m_csI2Cinit;
	CMVModel *m_pModel;//WEC2013

	bool m_bExit;//WEC2013

	bool m_bBackup;//WEC2013
	
	DWORD m_dwLastWatchdogMemoryCheck;//WEC2013
	DWORD m_dwLastAutoScreenTimer;//WEC2013
	DWORD m_dwLastMatrixdwn;//WEC2013
	DWORD m_dwSystemsilent;//WEC2013
	DWORD m_dwHourglasstimeout;//WEC2013
	
	bool m_bMatrxDwn_STARTSTOP;//WEC2013
	bool m_bMatrxDwn_OFF;//WEC2013
	bool m_bMatrxDwn_MANBREATH;//WEC2013
	bool m_bMatrxDwn_MANBREATHdisabled;//WEC2013
	bool m_bMatrxDwn_O2Flush;//WEC2013

	
	DWORD m_dwFactorydefaults;//WEC2013
	bool m_bSysSilent;//WEC2013

	bool m_bMatrxDwn_Alarmsilent;//WEC2013
	bool m_bMatrxDwn_Graph;//WEC2013
	bool m_bMatrxDwn_ALimits;//WEC2013
	bool m_bMatrxDwn_Home;//WEC2013
	bool m_bMatrxDwn_Menu;//WEC2013
	bool m_bMatrxDwn_FREE;//WEC2013
	bool m_bNebulizerDwn;//WEC2013

	//bool m_bNebulizerRunning;//WEC2013
	//bool m_bNebulizerDelayRunning;//WEC2013
	BYTE m_iCountCollectFOTTimer;
	BYTE m_iCountFOTimer;
	int m_iO2FlushCount;//WEC2013
	
	int m_iCountDownStart;//WEC2013
	int m_iCountDownAlarmSilent;//WEC2013
	int m_iCountTimeUntilStop;//WEC2013
	int m_iCountTimeUntilOff;//WEC2013

	eVentMode m_eVentModeCalcStarted;//WEC2013

	//static int iTestCount;

	int m_nX;//WEC2013
	int m_nY;//WEC2013
	
	
	TCHAR m_pszFontName[32];//WEC2013
	WORD m_wLanguageID;//WEC2013

	//WEC2013
	typedef enum {
	      CHINESE = 0,
	      JAPANESE,
	      OTHER,
		  NUMFONTCATEGORY
	} fontCategory;

	HFONT m_hf10Norm[NUMFONTCATEGORY];
	HFONT m_hf10Bold[NUMFONTCATEGORY];
	HFONT m_hf11Norm[NUMFONTCATEGORY];
	HFONT m_hf12Norm[NUMFONTCATEGORY];
	HFONT m_hf12Norm90degree[NUMFONTCATEGORY];
	HFONT m_hf12AcuNormNum[NUMFONTCATEGORY];
	HFONT m_hf13Bold[NUMFONTCATEGORY];
	HFONT m_hf13Norm[NUMFONTCATEGORY];
	HFONT m_hf14Norm[NUMFONTCATEGORY];
	HFONT m_hf14Bold[NUMFONTCATEGORY];
	HFONT m_hf14Bold90degree[NUMFONTCATEGORY];
	HFONT m_hf14AcuNormNum[NUMFONTCATEGORY];
	HFONT m_hf15Normal[NUMFONTCATEGORY];
	HFONT m_hf15Bold[NUMFONTCATEGORY];
	HFONT m_hf16Normal[NUMFONTCATEGORY];
	HFONT m_hf16Bold[NUMFONTCATEGORY];
	HFONT m_hf16Bold90degree[NUMFONTCATEGORY];
	HFONT m_hf17Bold[NUMFONTCATEGORY];
	HFONT m_hf18Normal[NUMFONTCATEGORY];
	HFONT m_hf18Bold[NUMFONTCATEGORY];
	HFONT m_hf18BoldNum[NUMFONTCATEGORY];
	HFONT m_hf20Bold[NUMFONTCATEGORY];
	HFONT m_hf20BoldNum[NUMFONTCATEGORY];
	HFONT m_hf21Medium[NUMFONTCATEGORY];
	HFONT m_hf21Bold[NUMFONTCATEGORY];
	HFONT m_hf22Medium[NUMFONTCATEGORY];
	HFONT m_hf22Bold[NUMFONTCATEGORY];
	HFONT m_hf24Bold[NUMFONTCATEGORY];
	HFONT m_hf26Medium[NUMFONTCATEGORY];
	HFONT m_hf28Bold[NUMFONTCATEGORY];
	HFONT m_hf30Bold[NUMFONTCATEGORY];
	HFONT m_hf31AcuBoldNum[NUMFONTCATEGORY];
	HFONT m_hf32Medium[NUMFONTCATEGORY];
	HFONT m_hf33AcuBoldNum[NUMFONTCATEGORY];
	HFONT m_hf34Bold[NUMFONTCATEGORY];
	HFONT m_hf34BoldNum[NUMFONTCATEGORY];
	HFONT m_hf38Bold[NUMFONTCATEGORY];
	HFONT m_hf40Bold[NUMFONTCATEGORY];
	HFONT m_hf50Bold[NUMFONTCATEGORY];
	HFONT m_hf60Bold[NUMFONTCATEGORY];
	HFONT m_hf70Bold[NUMFONTCATEGORY];
	HFONT m_hf70BoldNum[NUMFONTCATEGORY];

	//rkuNEWFIX
	/*friend UINT CThreadWatchdogThread(LPVOID pc);
	CWinThread*	m_pcwtThreadWatchdogThread;
	DWORD ThreadWatchdog(void);
	bool m_bDoThreadWatchdogThread;
	CEvent eventThreadWatchdog;
	HANDLE m_hThreadThreadWatchdog;*/

	//UINT faThreadWatchdog[MAXSIZE_THREADS];
	//UINT faPrevstateThreadWatchdog[MAXSIZE_THREADS];
	
	//Threads
	//WEC2013
	friend UINT CI2CWatchdogThread(LPVOID pc);
	CWinThread*	m_pcwtI2CWatchdogThread;
	DWORD SetI2CWatchdog(void);
	bool m_bDoI2CWatchdogThread;
	CEvent eventI2CWatchdog;
	HANDLE m_hThreadI2CWatchdog;

	//WEC2013
	friend UINT COxyCalThread(LPVOID pc);
	CWinThread*	m_pcwtOxyCalThread;
	DWORD CheckOxyCal(void);
	bool m_bDoOxyCalThread;
	CEvent eventOxyCal;
	HANDLE m_hThreadOxyCal;

	bool m_bO2SensorAutocal;//WEC2013
	bool m_bO2Sensor21manual;//WEC2013
	bool m_bO2Sensor100manual;//WEC2013
	bool m_bCancelOxiCal;//WEC2013

	bool m_bShowCO2PumpOnMsg;//WEC2013


	
	//WEC2013
	friend UINT CTimerThread(LPVOID pc);
	CWinThread*	m_pcwtTimerThread;
	DWORD DoTimerFunctions(void);
	bool m_bDoDoTimerFunctionsThread;
	CEvent eventDoTimerFunctions;
	HANDLE m_hThreadTimerFunctions;

	DWORD m_dwMemory;//WEC2013
	int m_iTestCntMemory;

	//WEC2013
	friend UINT CSaveTrendUSBThread(LPVOID pc);
	CWinThread*	m_pcwtSaveTrendUSBThread;
	DWORD SaveTrendUSB(void);
	bool m_bDoSaveTrendUSBThread;
	CEvent eventSaveTrendUSBfinish;
	HANDLE m_hThreadSaveTrendUSB;

	//WEC2013
	friend UINT CDelTrendThread(LPVOID pc);
	CWinThread*	m_pcwtDelTrendThread;
	DWORD DelTrendData(void);
	bool m_bDoDelTrendThread;
	CEvent eventDelTrend;
	HANDLE m_hThreadDelTrend;

	int m_numtasks;//WEC2013
	struct task_tag
	{
		ULONGLONG tickcount;
		DWORD step;
		eTimeFunc func;
	} task[MAXTASKS];//WEC2013

	CBmp* m_pcLogo;//WEC2013
	
	SHORT m_iOldOxyValue;

	bool m_bStartTrend;//WEC2013
	bool m_bDelayAutoOxyCal;//WEC2013
	bool m_bWD0error;//WEC2013
	bool m_bWDERRORI2C;//WEC2013
	bool m_bForceShutdown;//WEC2013
	bool m_bInitialized;//WEC2013

	bool m_bStartInstaller;
	bool m_bI2Cinitialized;

	bool m_hasFontCreated;
	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	void deletehf(HFONT & xhfont, TCHAR const * const debugTxt);
	void CreateFontHandles(int const xIndex, TCHAR * const xFontName);

	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


