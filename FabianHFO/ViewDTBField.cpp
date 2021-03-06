/**********************************************************************************************//**
 * \file	ViewDTBField.cpp.
 *
 * Implements the view dtb field class
 **************************************************************************************************/

#include "StdAfx.h"
#include "ViewDTBField.h"

//Date-Time-Battery
#include "MVViewHandler.h"
#include "LangAdmin.h"
#include "globDefs.h"

/**********************************************************************************************//**
 * TimerValue
 * #define BATTIMESTART	100
 * #define BATTERYTIME		15000
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define ACCUCHARGE		500

/**********************************************************************************************//**
 * A macro that defines batterytime
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define BATTERYTIME		2000

/**********************************************************************************************//**
 * A macro that defines alarmtime 300 Milliseconds
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define ALARMTIME300MS		300

/**********************************************************************************************//**
 * A macro that defines alarmtime 800 Milliseconds
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define ALARMTIME800MS		800

/**********************************************************************************************//**
 * A macro that defines accustatetime
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define ACCUSTATETIME		30000

/**********************************************************************************************//**
 * A macro that defines alarmtimenurse
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define ALARMTIMENURSE		1000

/**********************************************************************************************//**
 * A macro that defines standbytime
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define STANDBYTIME			1000

/**********************************************************************************************//**
 * A macro that defines psvapnoetime
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define PSVAPNOETIME		1000

/**********************************************************************************************//**
 * A macro that defines alarmlogtime
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define ALARMLOGTIME		1000




//extern CLangAdmin* g_pGlobalLanguageStrings;

/**********************************************************************************************//**
 * Initializes a new instance of the CViewDTBField class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	iViewID	Identifier for the view.
 **************************************************************************************************/

CViewDTBField::CViewDTBField(int iViewID):
CMVView(iViewID)
{
	InitializeCriticalSection(&csInfo);
	
	m_hdcStatusLine=NULL;
	m_hdcInfoLine=NULL;
	m_hdcNumericLine=NULL;
	m_hbmpStatusLine=NULL;
	m_hbmpInfoLine=NULL;
	m_hbmpNumericLine=NULL;

	m_bFlowCalNeeded=false;

	m_bDrawRemainingTime=false;

	m_bNurscallAlarm=false;
	m_bNurscallActive=false;
	m_bDrawNursecallRed=false;

	m_szInfolineText=_T("");
	m_szInfoText=_T("");
	m_szCounterText=_T("");
	
	m_iCounterASilent=-1;
	m_bCountDownASilent=false;
	m_bDrawYellowNumHeader=false;
	//m_bDrawAlarmLogNumHeader=false;

	m_iCounterStartVent=-1;
	m_bCountDownStartVent=false;

	m_iCounterStopVent=-1;
	m_bCountDownStopVent=false;

	m_iCounterTurnOff=-1;
	m_bCountDownTurnOff=false;

	m_szOxyFlushTime=_T("");
	m_bCountDownOxyFlush=false;

	m_bShowPresureVal=false;
	m_iAir=0;
	m_iOxy=0;
	m_iPPROX_Data=0;
	m_iP_MIX_Data=0;

	m_bExtrenalPower=false;
	m_bNetPower=true;
	m_bAccuPower=false;
	m_bOxyCalRunning=false;
	m_bPatData=false;
	m_bExhalValvCalRunning=false;
	m_bActiveHighAlarm=false;
	m_bActiveMediumAlarm=false;
	m_bActiveLowAlarm=false;

	m_bSignalAlarm=false;
	m_szSigaledAlarmText=_T("");

	m_bNebulizerRunning=false;
	m_bDrawNebulizer=false;
	m_bManualBreathRunning=false;
	m_bSTANDBYMode=false;
	m_bSystemASilent=false;
	m_bServiceASilent=false;
	m_bDrawInfoText=false;
	m_bScreenlockActive=false;

	m_bDrawRedInfoline=false;
	m_bDrawOrangeInfoline=false;
	m_bDrawYellowInfoline=false;
	m_bPSVapnoeTimer=false;
	m_bPSVapnoe=false;
	m_bCPAPBackup=false;
	//m_bChangePSVapnoeColor=false;
	m_bAlarmLogTimer=false;

	m_bMaintenanceNeeded=false;
	
	m_pcWarningGrey=NULL;
	m_pcWarningRed=NULL;
	m_pcGreyInfoline=NULL;
	m_pcRedInfoline=NULL;
	m_pcOrangeInfoline=NULL;
	m_pcYellowInfoline=NULL;
	m_pcScreenlockInfoline=NULL;
	m_pcBatState=NULL;
	m_pcGreyNumHeader=NULL;
	m_pcYellowNumHeader=NULL;
	m_pcAlarmOffGrey=NULL;
	m_pcAlarmOffYellow=NULL;
	m_pcAlarmSilentGrey=NULL;
	m_pcAlarmSilentYellow=NULL;

	m_pcAlarmSignaled=NULL;
	//m_pcAlarmLog=NULL;
	m_pcNoManATZ=NULL;
	m_pcPatientData=NULL;
	m_pcNurse=NULL;
	m_pcNurseActive=NULL;

	m_pcPediatric=NULL;
	m_pcNeonatal=NULL;
	//m_pcNebulizer=NULL;
	
	m_pcMaintenance=NULL;

	m_iBattStatMinutes=0;
	m_iBattStatPercents=0;
	m_bACCUcharging=false;
	m_bACCUfull=false;
	m_iCntAccuCharge=0;

	m_pcPRICO=NULL;
	m_pcPRICO1=NULL;
	m_pcPRICO2=NULL;
	m_pcPRICO3=NULL;
	m_pcPRICO4=NULL;
	m_pcPRICO5=NULL;
	m_pcPRICO6=NULL;
	m_pcPRICO7=NULL;
	m_pcPRICO8=NULL;
	m_pcPRICO9=NULL;
	m_pcPRICO10=NULL;
	m_pcPRICO11=NULL;
	m_pcPRICO12=NULL;
	m_pcPRICO13=NULL;
	m_pcPRICO14=NULL;
	m_pcPRICO15=NULL;
	m_pcPRICO16=NULL;
	m_pcPRICO17=NULL;
	m_pcPRICO18=NULL;
	m_pcPRICO19=NULL;
	m_pcPRICO20=NULL;
	m_pcPRICO21=NULL;
	m_pcPRICO22=NULL;
	m_pcPRICO23=NULL;
	m_pcPRICO24=NULL;

	m_pcACCU=NULL;
	m_pcACCU_0=NULL;
	m_pcACCU_20=NULL;
	m_pcACCU_40=NULL;
	m_pcACCU_60=NULL;
	m_pcACCU_80=NULL;
	m_pcACCU_100=NULL;
	/*m_pcACCU_Net=NULL;
	m_pcACCU_Extern=NULL;
	m_pcACCU_Empty=NULL;
	m_pcACCULOAD_0=NULL;*/
	m_pcACCULOAD_20=NULL;
	m_pcACCULOAD_40=NULL;
	m_pcACCULOAD_60=NULL;
	m_pcACCULOAD_80=NULL;
	m_pcACCULOAD_100=NULL;

	m_pcStartHFO_red=NULL;
	m_pcStartHFO_grey=NULL;
	m_pcStartHFO_yellow=NULL;
	m_pcStartHFO_orange=NULL;

	/*CTime time=CTime::GetCurrentTime();
	m_iPrevDay = time.GetDay();*/

	SYSTEMTIME st;
	GetLocalTime(&st);
	COleDateTime dtTime(st);
	m_iPrevDay = dtTime.GetDay();

	m_iPRICOCount=0;
	m_bPRICOrunning=false;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CViewDTBField class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CViewDTBField::~CViewDTBField(void)
{
	//m_bDoDateTime=false;

	DeleteCriticalSection(&csInfo);
}

BEGIN_MESSAGE_MAP(CViewDTBField, CMVView)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * Creates the view
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewDTBField::CreateView()
{
	RECT rcLd3={0,0,800,41};

	if (!Create(AfxGetApp()->GetMainWnd(),rcLd3,IDC_VIEW_DTBFIELD))
	{
		return false;
	}
	
	Initialize();

	DrawLine(true,true,true);

	m_Clock.Run();

	return true;
}

/**********************************************************************************************//**
 * Creates the Window instance that will be represented by this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param [in,out]	pParentWnd	If non-null, the parent window.
 * \param 		  	rc		  	The rectangle.
 * \param 		  	nID		  	The identifier.
 * \param [in,out]	pContext  	If non-null, the context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CViewDTBField::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
{
	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD|WS_VISIBLE,rc,pParentWnd,nID))
	{
		CClientDC dc(this);

		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
		m_hBmpPrev=(HBITMAP)SelectObject(m_hDC,m_hBmp);

		//CBrush cbrBack(RGB(100,100,100));
		CBrush cbrBack(BACKGND);
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);


		//##############Status Line##################################
		m_hdcStatusLine = CreateCompatibleDC(m_hDC);
		m_hbmpStatusLine = CreateCompatibleBitmap(m_hDC, 150,41);
		SelectObject(m_hdcStatusLine, m_hbmpStatusLine);

		// delete background
		HPEN hpenprevStatusLine=(HPEN)SelectObject(m_hdcStatusLine, (HPEN)GetStockObject(NULL_PEN));	
		HBRUSH hbrprevStatusLine=(HBRUSH)SelectObject(m_hdcStatusLine,cbrBack); 
		Rectangle(m_hdcStatusLine, 0, 0, 150,41);

		//##############Info Line##################################
		m_hdcInfoLine = CreateCompatibleDC(m_hDC);
		m_hbmpInfoLine = CreateCompatibleBitmap(m_hDC, 460,41);
		SelectObject(m_hdcInfoLine, m_hbmpInfoLine);

		// delete background
		HPEN hpenprevInfoLine=(HPEN)SelectObject(m_hdcInfoLine, (HPEN)GetStockObject(NULL_PEN));	
		HBRUSH hbrprevInfoLine=(HBRUSH)SelectObject(m_hdcInfoLine,cbrBack); 
		Rectangle(m_hdcInfoLine, 0, 0, 460,41);


		//##############Numeric Line##################################
		m_hdcNumericLine = CreateCompatibleDC(m_hDC);
		m_hbmpNumericLine = CreateCompatibleBitmap(m_hDC, 190,41);
		SelectObject(m_hdcNumericLine, m_hbmpNumericLine);

		// delete background
		HPEN hpenprevNumericLine=(HPEN)SelectObject(m_hdcNumericLine, (HPEN)GetStockObject(NULL_PEN));	
		HBRUSH hbrprevNumericLine=(HBRUSH)SelectObject(m_hdcNumericLine,cbrBack); 
		Rectangle(m_hdcNumericLine, 0, 0, 190,41);


#if defined(DEMO_VERSION)
		m_pcGreyInfoline = new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_GREY_DEMO);
#elif defined(FRANZ_VERSION)
		//m_pcGreyInfoline = new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_GREY_DEMO);
		m_pcGreyInfoline = new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_GREY);
		SetTimer(FRANZTIMER,1000,NULL);
		m_bShowPresureVal=true;
#else
		if(getModel()->getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL_REMOTE)
			m_pcGreyInfoline = new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_GREY_DEMO);
		else
			m_pcGreyInfoline = new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_GREY);
#endif

		
		m_pcBatState		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_DATE_BAT);
		m_pcGreyNumHeader		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_NUMHEADER);

		m_pcBatState->Draw(m_hDC);
		m_pcGreyInfoline->Draw(m_hDC,150,0);
		m_pcGreyNumHeader->Draw(m_hDC,610,0);

		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		

		SelectObject(m_hdcStatusLine, hpenprevStatusLine);	
		SelectObject(m_hdcStatusLine,hbrprevStatusLine); 

		SelectObject(m_hdcInfoLine, hpenprevInfoLine);	
		SelectObject(m_hdcInfoLine,hbrprevInfoLine); 

		SelectObject(m_hdcNumericLine, hpenprevNumericLine);	
		SelectObject(m_hdcNumericLine,hbrprevNumericLine); 

		//cbrBack.DeleteObject();//rkuNEWFIX

		return 1;
	}
	else
		return 0;
}

/**********************************************************************************************//**
 * Initializes this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewDTBField::Initialize()
{
	CDC* pDC=CDC::FromHandle(m_hDC);
	//HDC hdc = *pDC;

	

#if defined(DEMO_VERSION)
	m_pcScreenlockInfoline		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_SCREENLOCK_DEMO);
	m_pcRedInfoline				= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_RED_DEMO);
	m_pcOrangeInfoline			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_ORANGE_DEMO);
	m_pcYellowInfoline			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_YELLOW_DEMO);
#elif defined(FRANZ_VERSION)
	/*m_pcScreenlockInfoline		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_SCREENLOCK_DEMO);
	m_pcRedInfoline				= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_RED_DEMO);
	m_pcOrangeInfoline			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_ORANGE_DEMO);
	m_pcYellowInfoline			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_YELLOW_DEMO);*/
	m_pcScreenlockInfoline		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_SCREENLOCK);
	m_pcRedInfoline				= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_RED);
	m_pcOrangeInfoline			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_ORANGE);
	m_pcYellowInfoline			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_YELLOW);
	m_iAir=getModel()->getSPI()->Read_DRM_1_ADC();
	m_iOxy=getModel()->getSPI()->Read_DRM_2_ADC();
	m_iPPROX_Data=getModel()->getSPI()->Read_P_PROX_ADC();
	m_iP_MIX_Data=getModel()->getSPI()->Read_DRM_3_ADC();
#else
	//if(getModel()->getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL)//FOT-TERM1
	//{
	//	m_pcScreenlockInfoline		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_SCREENLOCK_DEMO);
	//	m_pcRedInfoline				= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_RED_DEMO);
	//	m_pcOrangeInfoline			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_ORANGE_DEMO);
	//	m_pcYellowInfoline			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_YELLOW_DEMO);
	//}
	//else
	{
		m_pcScreenlockInfoline		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_SCREENLOCK);
		m_pcRedInfoline				= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_RED);
		m_pcOrangeInfoline			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_ORANGE);
		m_pcYellowInfoline			= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_INFOLINE_YELLOW);
	}

	
#endif

	m_pcYellowNumHeader		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_NUMHEAD_ASILENT);
	
	m_pcWarningGrey		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_WARNING_GREY_23x20);
	m_pcWarningRed		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_WARNING_RED_23x20);

	m_pcAlarmOffGrey		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_ASILENT_GREY);
	m_pcAlarmOffYellow	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_ASILENT_YELLOW);
	m_pcAlarmSignaled	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_ALARM_SIGNALED);

	m_pcAlarmSilentGrey		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_ALARMSILENT_GREY);
	m_pcAlarmSilentYellow	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_ALARMSILENT_YELLOW);

	m_pcPatientData		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PATIENTDATA);
	m_pcNoManATZ		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_NOMANATZ);
	m_pcPediatric		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PEDIATRIC);
	m_pcNeonatal		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_NEONATAL);
	m_pcNurse		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_NURSE32);
	m_pcNurseActive		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_NURSEALARM);

	//m_pcNebulizer		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NEBUL_INFO);

	m_pcMaintenance = new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_MAINTENANCE);
	
	m_pcPRICO1	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_1);
	m_pcPRICO2	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_2);
	m_pcPRICO3	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_3);
	m_pcPRICO4	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_4);
	m_pcPRICO5	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_5);
	m_pcPRICO6	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_6);
	m_pcPRICO7	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_7);
	m_pcPRICO8	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_8);
	m_pcPRICO9	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_9);
	m_pcPRICO10	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_10);
	m_pcPRICO11	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_11);
	m_pcPRICO12	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_12);
	m_pcPRICO13	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_13);
	m_pcPRICO14	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_14);
	m_pcPRICO15	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_15);
	m_pcPRICO16	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_16);
	m_pcPRICO17	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_17);
	m_pcPRICO18	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_18);
	m_pcPRICO19	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_19);
	m_pcPRICO20	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_20);
	m_pcPRICO21	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_21);
	m_pcPRICO22	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_22);
	m_pcPRICO23	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_23);
	m_pcPRICO24	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_PRICO_24);

	m_pcACCU_0		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_ACCU_0);
	m_pcACCU_20		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_ACCU_20);
	m_pcACCU_40		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_ACCU_40);
	m_pcACCU_60		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_ACCU_60);
	m_pcACCU_80		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_ACCU_80);
	m_pcACCU_100		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_ACCU_100);
	/*m_pcACCU_Net		= new CBmp(theApp.m_hInstance,hdc,	IDB_PIC_ACCU_NET);
	m_pcACCU_Extern		= new CBmp(theApp.m_hInstance,hdc,	IDB_PIC_ACCU_EXTERN);
	m_pcACCU_Empty		= new CBmp(theApp.m_hInstance,hdc,	IDB_PIC_ACCU_EMPTY);
	m_pcACCULOAD_0		= new CBmp(theApp.m_hInstance,hdc,	IDB_PIC_ACCULOAD_0);*/
	m_pcACCULOAD_20		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_ACCULOAD_20);
	m_pcACCULOAD_40		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_ACCULOAD_40);
	m_pcACCULOAD_60		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_ACCULOAD_60);
	m_pcACCULOAD_80		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_ACCULOAD_80);
	m_pcACCULOAD_100		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PIC_ACCULOAD_100);

	m_pcStartHFO_red		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_STARTHFO_ALARM);
	m_pcStartHFO_grey		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_STARTHFO_GREY);
	m_pcStartHFO_yellow		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_STARTHFO_YELLOW);
	m_pcStartHFO_orange		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_DTB_STARTHFO_ORANGE);

	m_pcBatState->Draw(m_hDC);
	m_pcGreyInfoline->Draw(m_hDC,150,0);
	m_pcGreyNumHeader->Draw(m_hDC,610,0);



	m_Clock.Create(_T(""), WS_VISIBLE|WS_CHILD, CRect(760,1,800,41), this , 0);
	m_Clock.ShowDate(false);
	m_Clock.SetBitmaps(IDB_CLOCK_FACE_GREY,false);

	m_bNurscallActive=getModel()->getCONFIG()->IsNursecallActive();
	if(getModel()->getDATAHANDLER()->isNurscallAlarm() 
		&& m_bNurscallActive)
	{
		if(m_bNurscallAlarm==false)
		{
#ifndef SIMULATION_ENTREK //rkuNEWFIX
			SetTimer(NURSECALLTIMER,ALARMTIMENURSE,NULL);
#endif
		}
		m_bNurscallAlarm=true;
	}
	
	CheckAkkuState();


	//StartDateTimeThread();

#ifndef SIMULATION_ENTREK //rkuNEWFIX
	SetTimer(BATTERYTIMER,BATTERYTIME,NULL);
#endif

	return true;
}

/**********************************************************************************************//**
 * Opens this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewDTBField::Open()
{
	Show();
	
}

/**********************************************************************************************//**
 * Closes this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewDTBField::Close()
{
	KillTimer(NURSECALLTIMER);
	KillTimer(BATTERYTIMER);
	KillTimer(ACCUCHARGETIMER);
	KillTimer(INFOTXTTIMER);
	KillTimer(STANDBYTIMER);
	KillTimer(INFOALARMTIMER);
	KillTimer(PSVAPNOETIMER);
	KillTimer(FRANZTIMER);

	//m_bDoDateTime=false;

	if(m_Clock)
	{
		m_Clock.Stop();
		delete m_Clock;
	}

	
	/*m_bSignalTubusDisconAlarm=false;
	m_bSignalPatientAlarm=false;*/
	m_bSignalAlarm=false;
	m_szSigaledAlarmText=_T("");
	/*m_bSignalHighAlarm=false;
	m_bSignalMediumAlarm=false;
	m_bSignalLowAlarm=false;*/
	m_bActiveHighAlarm=false;
	m_bActiveMediumAlarm=false;
	m_bActiveLowAlarm=false;
	m_bSTANDBYMode=false;


	if(m_pcStartHFO_red)
		delete m_pcStartHFO_red;
	m_pcStartHFO_red=NULL;

	if(m_pcStartHFO_grey)
		delete m_pcStartHFO_grey;
	m_pcStartHFO_grey=NULL;

	if(m_pcStartHFO_yellow)
		delete m_pcStartHFO_yellow;
	m_pcStartHFO_yellow=NULL;

	if(m_pcStartHFO_orange)
		delete m_pcStartHFO_orange;
	m_pcStartHFO_orange=NULL;

	if(m_pcScreenlockInfoline)
		delete m_pcScreenlockInfoline;
	m_pcScreenlockInfoline=NULL;

	if(m_pcGreyInfoline)
		delete m_pcGreyInfoline;
	m_pcGreyInfoline=NULL;

	if(m_pcRedInfoline)
		delete m_pcRedInfoline;
	m_pcRedInfoline=NULL;

	if(m_pcOrangeInfoline)
		delete m_pcOrangeInfoline;
	m_pcOrangeInfoline=NULL;

	if(m_pcYellowInfoline)
		delete m_pcYellowInfoline;
	m_pcYellowInfoline=NULL;

	if(m_pcBatState)
		delete m_pcBatState;
	m_pcBatState=NULL;

	if(m_pcGreyNumHeader)
		delete m_pcGreyNumHeader;
	m_pcGreyNumHeader=NULL;

	if(m_pcYellowNumHeader)
		delete m_pcYellowNumHeader;
	m_pcYellowNumHeader=NULL;

	/*if(m_pcAlimitHeader)
		delete m_pcAlimitHeader;
	m_pcAlimitHeader=NULL;*/

	if(m_pcMaintenance)
		delete m_pcMaintenance;
	m_pcMaintenance=NULL;

	if(m_pcWarningRed)
		delete m_pcWarningRed;
	m_pcWarningRed=NULL;

	if(m_pcWarningGrey)
		delete m_pcWarningGrey;
	m_pcWarningGrey=NULL;

	if(m_pcAlarmOffGrey)
		delete m_pcAlarmOffGrey;
	m_pcAlarmOffGrey=NULL;

	if(m_pcAlarmOffYellow)
		delete m_pcAlarmOffYellow;
	m_pcAlarmOffYellow=NULL;

	if(m_pcAlarmSilentGrey)
		delete m_pcAlarmSilentGrey;
	m_pcAlarmSilentGrey=NULL;

	if(m_pcAlarmSilentYellow)
		delete m_pcAlarmSilentYellow;
	m_pcAlarmSilentYellow=NULL;

	if(m_pcAlarmSignaled)
		delete m_pcAlarmSignaled;
	m_pcAlarmSignaled=NULL;

	/*if(m_pcAlarmLog)
		delete m_pcAlarmLog;
	m_pcAlarmLog=NULL;*/

	if(m_pcNoManATZ)
		delete m_pcNoManATZ;
	m_pcNoManATZ=NULL;

	if(m_pcNurse)
		delete m_pcNurse;
	m_pcNurse=NULL;

	if(m_pcNurseActive)
		delete m_pcNurseActive;
	m_pcNurseActive=NULL;

	if(m_pcPatientData)
		delete m_pcPatientData;
	m_pcPatientData=NULL;

	if(m_pcPediatric)
		delete m_pcPediatric;
	m_pcPediatric=NULL;

	if(m_pcNeonatal)
		delete m_pcNeonatal;
	m_pcNeonatal=NULL;

	/*if(m_pcNebulizer)
		delete m_pcNebulizer;
	m_pcNebulizer=NULL;*/

	if(m_pcPRICO)
		delete m_pcPRICO;
	m_pcPRICO=NULL;
	if(m_pcPRICO1)
		delete m_pcPRICO1;
	m_pcPRICO1=NULL;
	if(m_pcPRICO2)
		delete m_pcPRICO2;
	m_pcPRICO2=NULL;
	if(m_pcPRICO3)
		delete m_pcPRICO3;
	m_pcPRICO3=NULL;
	if(m_pcPRICO4)
		delete m_pcPRICO4;
	m_pcPRICO4=NULL;
	if(m_pcPRICO5)
		delete m_pcPRICO5;
	m_pcPRICO5=NULL;
	if(m_pcPRICO6)
		delete m_pcPRICO6;
	m_pcPRICO6=NULL;
	if(m_pcPRICO7)
		delete m_pcPRICO7;
	m_pcPRICO7=NULL;
	if(m_pcPRICO8)
		delete m_pcPRICO8;
	m_pcPRICO8=NULL;
	if(m_pcPRICO9)
		delete m_pcPRICO9;
	m_pcPRICO9=NULL;
	if(m_pcPRICO10)
		delete m_pcPRICO10;
	m_pcPRICO10=NULL;
	if(m_pcPRICO11)
		delete m_pcPRICO11;
	m_pcPRICO11=NULL;
	if(m_pcPRICO12)
		delete m_pcPRICO12;
	m_pcPRICO12=NULL;
	if(m_pcPRICO13)
		delete m_pcPRICO13;
	m_pcPRICO13=NULL;
	if(m_pcPRICO14)
		delete m_pcPRICO14;
	m_pcPRICO14=NULL;
	if(m_pcPRICO15)
		delete m_pcPRICO15;
	m_pcPRICO15=NULL;
	if(m_pcPRICO16)
		delete m_pcPRICO16;
	m_pcPRICO16=NULL;
	if(m_pcPRICO17)
		delete m_pcPRICO17;
	m_pcPRICO17=NULL;
	if(m_pcPRICO18)
		delete m_pcPRICO18;
	m_pcPRICO18=NULL;
	if(m_pcPRICO19)
		delete m_pcPRICO19;
	m_pcPRICO19=NULL;
	if(m_pcPRICO20)
		delete m_pcPRICO20;
	m_pcPRICO20=NULL;
	if(m_pcPRICO21)
		delete m_pcPRICO21;
	m_pcPRICO21=NULL;
	if(m_pcPRICO22)
		delete m_pcPRICO22;
	m_pcPRICO22=NULL;
	if(m_pcPRICO23)
		delete m_pcPRICO23;
	m_pcPRICO23=NULL;
	if(m_pcPRICO24)
		delete m_pcPRICO24;
	m_pcPRICO24=NULL;

	if(m_pcACCU)
		delete m_pcACCU;
	m_pcACCU=NULL;
	if(m_pcACCU_0)
		delete m_pcACCU_0;
	m_pcACCU_0=NULL;
	if(m_pcACCU_20)
		delete m_pcACCU_20;
	m_pcACCU_20=NULL;
	if(m_pcACCU_40)
		delete m_pcACCU_40;
	m_pcACCU_40=NULL;
	if(m_pcACCU_60)
		delete m_pcACCU_60;
	m_pcACCU_60=NULL;
	if(m_pcACCU_80)
		delete m_pcACCU_80;
	m_pcACCU_80=NULL;
	if(m_pcACCU_100)
		delete m_pcACCU_100;
	m_pcACCU_100=NULL;
	/*if(m_pcACCU_Net)
		delete m_pcACCU_Net;
	m_pcACCU_Net=NULL;
	if(m_pcACCU_Extern)
		delete m_pcACCU_Extern;
	m_pcACCU_Extern=NULL;
	if(m_pcACCU_Empty)
		delete m_pcACCU_Empty;
	m_pcACCU_Empty=NULL;

	if(m_pcACCULOAD_0)
		delete m_pcACCULOAD_0;
	m_pcACCULOAD_0=NULL;*/
	if(m_pcACCULOAD_20)
		delete m_pcACCULOAD_20;
	m_pcACCULOAD_20=NULL;
	if(m_pcACCULOAD_40)
		delete m_pcACCULOAD_40;
	m_pcACCULOAD_40=NULL;
	if(m_pcACCULOAD_60)
		delete m_pcACCULOAD_60;
	m_pcACCULOAD_60=NULL;
	if(m_pcACCULOAD_80)
		delete m_pcACCULOAD_80;
	m_pcACCULOAD_80=NULL;
	if(m_pcACCULOAD_100)
		delete m_pcACCULOAD_100;
	m_pcACCULOAD_100=NULL;


	if(m_hdcStatusLine)
		DeleteDC(m_hdcStatusLine);

	if(m_hdcInfoLine)
		DeleteDC(m_hdcInfoLine);

	if(m_hdcNumericLine)
		DeleteDC(m_hdcNumericLine);

	if(m_hbmpStatusLine)
		DeleteObject(m_hbmpStatusLine);

	if(m_hbmpInfoLine)
		DeleteObject(m_hbmpInfoLine);

	if(m_hbmpNumericLine)
		DeleteObject(m_hbmpNumericLine);

	
	
}

/**********************************************************************************************//**
 * Shows this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewDTBField::Show()
{
	this->ShowWindow(SW_SHOW);

	//Draw();
	DrawLine(true,true,true);

	// start the clock
	//m_Clock.Run();
}

/**********************************************************************************************//**
 * Hides this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewDTBField::Hide()
{
	this->ShowWindow(SW_HIDE);
}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewDTBField::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);

}

/**********************************************************************************************//**
 * Executes the timer action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	nIDEvent	The identifier event.
 **************************************************************************************************/

void CViewDTBField::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==PRICOTIMER)
	{
		if(m_iPRICOCount>0)
			m_iPRICOCount++;
		PostMessage(WM_DTB_REFRESH_NUM);
	}
	else if(nIDEvent==FRANZTIMER)
	{
		m_iAir=getModel()->getSPI()->Read_DRM_1_ADC();
		m_iOxy=getModel()->getSPI()->Read_DRM_2_ADC();
		m_iPPROX_Data=getModel()->getSPI()->Read_P_PROX_ADC();
		m_iP_MIX_Data=getModel()->getSPI()->Read_DRM_3_ADC();
		PostMessage(WM_DTB_REFRESH_INFO);
	}
	//else if(nIDEvent==NEBULIZER_DTB_TIMER) //rkuNEWFIX
	//{
	//	m_bDrawNebulizer=!m_bDrawNebulizer;
	//	DrawLine(true,false,false);
	//}
	else if(nIDEvent==ACCUSTATETIMER)
	{
		KillTimer(ACCUSTATETIMER);
		m_bDrawRemainingTime=true;
		DrawLine(true,false,false);


	}
	else if(nIDEvent==NURSECALLTIMER)
	{
		m_bDrawNursecallRed=!m_bDrawNursecallRed;
		DrawLine(true,false,false);
	}
	else if(nIDEvent==ACCUCHARGETIMER)
	{
		m_iCntAccuCharge++;
		if(m_iCntAccuCharge>5)
			m_iCntAccuCharge=0;
		DrawLine(true,false,false);
	}
	else if(nIDEvent==BATTERYTIMER)
	{
		if(CheckAkkuState())
			DrawLine(true,false,false);

		if(CheckDateChanged())
			DrawLine(false,false,false);
	}
	else if(nIDEvent==STANDBYTIMER)
	{
		PostMessage(WM_DTB_REFRESH_INFO);
	}
	else if(nIDEvent==INFOALARMTIMER)
	{
		PostMessage(WM_DTB_REFRESH_INFO);
	}
	else if(nIDEvent==PSVAPNOETIMER)
	{
		PostMessage(WM_DTB_REFRESH_INFO);
	}
	else if(nIDEvent==INFOTXTTIMER)
	{
		KillTimer(INFOTXTTIMER);
		m_bDrawInfoText=false;
		m_szInfoText=_T("");
		PostMessage(WM_DTB_REFRESH_INFO);
	}

	CMVView::OnTimer(nIDEvent);
}

/**********************************************************************************************//**
 * Sets cpap backup
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CViewDTBField::SetCPAPBackup(bool bState)
{
	m_bCPAPBackup=bState;
	setPSVapnoe(bState);
}

/**********************************************************************************************//**
 * Sets maintenance flag
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CViewDTBField::setMaintenanceFlag(bool bState)
{
	bool bRedraw=false;
	if(m_bMaintenanceNeeded!=bState)
	{
		bRedraw=true;
	} 
	m_bMaintenanceNeeded=bState;

	if(bRedraw)
		PostMessage(WM_DTB_REFRESH_INFO);

}

/**********************************************************************************************//**
 * Sets ps vapnoe
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bState	True to state.
 **************************************************************************************************/

void CViewDTBField::setPSVapnoe(bool bState)
{
	if(m_bPSVapnoe==bState)
		return;

	

	if(bState && !m_bPSVapnoe)
	{
		SetTimer(PSVAPNOETIMER,PSVAPNOETIME,NULL);
		m_bPSVapnoe=bState;
		if(!m_bPSVapnoeTimer)
		{
			m_bPSVapnoeTimer=true;
			PostMessage(WM_DTB_REFRESH_INFO);
		}
	}
	else if(!bState && m_bPSVapnoe)
	{
		KillTimer(PSVAPNOETIMER);
		m_bPSVapnoe=bState;
		PostMessage(WM_DTB_REFRESH_INFO);
	}
}

/**********************************************************************************************//**
 * Gets active alarm text
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	The active alarm text.
 **************************************************************************************************/

CStringW CViewDTBField::GetActiveAlarmText()
{
	eAlarm ePrimaryAlarm = getModel()->getALARMHANDLER()->getActiveAlarm();
	CStringW szAlarmText=_T("");

	if(ePrimaryAlarm!=AL_NONE)
	{
		szAlarmText=getModel()->getALARMHANDLER()->getAlarmText(ePrimaryAlarm);
		/*if(		ePrimaryAlarm==AL_DISCONNECTION
			&&	getModel()->getDATAHANDLER()->IsHFOrunning()==false
			&&	getModel()->getDATAHANDLER()->activeModeIsHFO())
		{
			szAlarmText+=_T(" - ");
			szAlarmText+=getModel()->GetLanguageString(IDS_TXT_HFOSTOPPED);
			m_bHFOstopped=true;
		}
		else
			m_bHFOstopped=false;*/
	}
	return szAlarmText;
}

//CStringW CViewDTBField::GetSignalAlarmText()
//{
//	CStringW szAlarmText=_T("");
//
//	if(	getModel()->getALARMHANDLER()->ALARM_DISCONNECTION->getAlarmState()==AS_SIGNALED)
//	{
//		szAlarmText=getModel()->getALARMHANDLER()->GetAlarmText(AL_DISCONNECTION);
//		if(		getModel()->getDATAHANDLER()->IsHFOrunning()==false
//			&&	getModel()->getDATAHANDLER()->activeModeIsHFO())
//		{
//			m_bHFOstopped=true;
//			szAlarmText+=_T(" - ");
//			szAlarmText+=getModel()->GetLanguageString(IDS_TXT_HFOSTOPPED);
//		}
//		else
//			m_bHFOstopped=false;
//	}
//	else if(	getModel()->getALARMHANDLER()->getState_TUBUSBLOCKED()==AS_SIGNALED)
//	{
//		szAlarmText=getModel()->getALARMHANDLER()->GetAlarmText(AL_TUBUSBLOCKED);
//	}
//
//	return szAlarmText;
//}

/**********************************************************************************************//**
 * Notifies the language changed
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewDTBField::NotifyLanguageChanged()
{
	DrawLine(true,true,true);
}

/**********************************************************************************************//**
 * Notifies an event
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param [in,out]	pEvent	If non-null, the event.
 **************************************************************************************************/

void CViewDTBField::NotifyEvent(CMVEvent* pEvent)
{
	switch(pEvent->GetET())
	{
	case CMVEvent::ET_CONTROL:
		{
			CMVEventControl* pCtrlEvent = (CMVEventControl*)pEvent;
			switch(pCtrlEvent->GetEventType())
			{
			case CMVEventControl::EV_CONTROL_SYSTEMSTATE_CHANGED:
				{
					m_bNurscallActive=getModel()->getCONFIG()->IsNursecallActive();
					if(getModel()->getDATAHANDLER()->isNurscallAlarm() 
						&& m_bNurscallActive)
					{
						if(m_bNurscallAlarm==false)
						{
#ifndef SIMULATION_ENTREK //rkuNEWFIX
							SetTimer(NURSECALLTIMER,ALARMTIMENURSE,NULL);
#endif
						}
						m_bNurscallAlarm=true;
					}

					//if(false==getModel()->getVMODEHANDLER()->activeModeIsNMODE())
					{
						m_bFlowCalNeeded = getModel()->getVentilationRangeChanged();

						if(m_bFlowCalNeeded)
							DrawLine(true,true,false);
						else
							DrawLine(true,false,false);
					}
					/*else
					{
						m_bFlowCalNeeded = false;
					}*/
					
				}
				break;
			case CMVEventControl::EV_CONTROL_FLOWSENSOR_CALIBRATED:
				{
					m_bFlowCalNeeded = false;

					PostMessage(WM_DTB_REFRESH_INFO);
				}
				break;
			case CMVEventControl::EV_CONTROL_POWERSTATE_CHANGED:
				{
					CheckAkkuState();
					DrawLine(true,false,false);
				}
				break;
			case CMVEventControl::EV_CONTROL_DEL_MODE_EXHALVALVCAL:
				{
					m_bServiceASilent=false;
					m_bExhalValvCalRunning=false;
					PostMessage(WM_DTB_REFRESH_INFO);
				}
				break;
			case CMVEventControl::EV_CONTROL_SET_MODE_EXHALVALVCAL:
				{
					//m_szInfoText=_T("");
					m_bServiceASilent=true;
					m_bExhalValvCalRunning=true;

					PostMessage(WM_DTB_REFRESH_INFO);
				}
				break;
			case CMVEventControl::EV_CONTROL_STANDBY:
				{
					m_bSTANDBYMode=true;
					m_bCountDownStartVent=false;
					

					//DrawInfoText(getModel()->GetLanguageString(IDS_STANDBY),true);
					SetTimer(STANDBYTIMER,STANDBYTIME,NULL);
				}
				break;
			case CMVEventControl::EV_CONTROL_START_VENTILATION:
				{
					KillTimer(STANDBYTIMER);
					
					m_bSTANDBYMode=false;
					m_bCountDownStartVent=false;
					PostMessage(WM_DTB_REFRESH_INFO);
				}
				break;
			case CMVEventControl::EV_CONTROL_STOP_VENTILATION:
				{
					KillTimer(STANDBYTIMER);
					m_bCountDownStartVent=true;
					m_bSTANDBYMode=false;
					PostMessage(WM_DTB_REFRESH_INFO);
				}
				break;
			case CMVEventControl::EV_CONTROL_START_MANBREATH:
				{
					m_bManualBreathRunning=true;
					PostMessage(WM_DTB_REFRESH_INFO);
				}
				break;
			case CMVEventControl::EV_CONTROL_STOP_MANBREATH:
				{
					m_bManualBreathRunning=false;
					PostMessage(WM_DTB_REFRESH_INFO);
				}
				break;
			case CMVEventControl::EV_CONTROL_START_NEBULIZER:
				{
					SetTimer(NEBULIZER_DTB_TIMER,STANDBYTIME,NULL);
					//m_bNebulizerRunning=getModel()->getDATAHANDLER()->isNebulizerOn();
					//DrawLine(true,false,false);
				}
				break;
			case CMVEventControl::EV_CONTROL_STOP_NEBULIZER:
				{
					KillTimer(NEBULIZER_DTB_TIMER);
					m_bNebulizerRunning=false;
					m_bDrawNebulizer=false;
					//DrawLine(true,false,false);
				}
				break;
			case CMVEventControl::EV_CONTROL_FLOWSENSORSTATE:
			case CMVEventControl::EV_CONTROL_O2SENSORSTATE:
				{
					PostMessage(WM_DTB_REFRESH_INFO);
				}
				break;
			case CMVEventControl::EV_CONTROL_O2CALSTART:
				{
					//DEBUGMSG(TRUE, (TEXT("EV_CONTROL_O2CALSTART\r\n")));
					m_bOxyCalRunning=true;

					if(!m_bPSVapnoeTimer)
						PostMessage(WM_DTB_REFRESH_INFO);
				}
				break;
			case CMVEventControl::EV_CONTROL_O2CALSTOP:
				{
					//DEBUGMSG(TRUE, (TEXT("EV_CONTROL_O2CALSTOP\r\n")));
					m_bOxyCalRunning=false;

					if(!m_bPSVapnoeTimer)
						PostMessage(WM_DTB_REFRESH_INFO);
				}
				break;
			default:
				break;
			}
		}
	case CMVEvent::ET_UIEVENT:
		{
			CMVEventUI* pUIEvent = (CMVEventUI*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventUI::EV_BN_SCREENFREE:
				{
					m_bScreenlockActive=false;
					PostMessage(WM_DTB_REFRESH_INFO);
				}
				break;
			case CMVEventUI::EV_BN_SCREENLOCK:
				{
					m_bScreenlockActive=true;
					PostMessage(WM_DTB_REFRESH_INFO);
				}
				break;
			case CMVEventUI::EV_PATDATA_CHANGED:
				{
					DrawLine(true,false,false);
				}
				break;
			case CMVEventUI::EV_BN_SET_SERVICE_MODE:
				{
					if(getModel()->getALARMHANDLER()->getAlarmSilentState()==ASTATE_SILENT)
					{
						m_bServiceASilent=true;

						PostMessage(WM_DTB_REFRESH_INFO_NUM);
					}
				}
				break;
			case CMVEventUI::EV_BN_ALIMIT4:
				{
					if(!getModel()->getALARMHANDLER()->isAlarm())
					{
						PostMessage(WM_DTB_REFRESH_INFO_NUM);
					}
					
				}
				break;
			default:
				break;
			}
		}
		break;
	case CMVEvent::ET_ALARM:
		{
			CMVEventAlarm* pUIEvent = (CMVEventAlarm*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventAlarm::EV_ALARM_REACTIVATED:
				{
					//CheckAlarm();
					if(getModel()->getALARMHANDLER()->isActiveAlarm())
					{
						eAlarmPrio prio=getModel()->getALARMHANDLER()->getPrioActiveAlarm();
						switch(prio)
						{
						case AP_1:
						case AP_2:
						case AP_3:
						case AP_4:
						case AP_5:
						case AP_6:
						case AP_7:
							{
								m_bActiveHighAlarm=true;
								m_bActiveMediumAlarm=false;
								m_bActiveLowAlarm=false;
								SetTimer(INFOALARMTIMER,ALARMTIME300MS,NULL);
							}
							break;
						case AP_8:
							{
								m_bActiveHighAlarm=false;
								m_bActiveMediumAlarm=true;
								m_bActiveLowAlarm=false;
								SetTimer(INFOALARMTIMER,ALARMTIME800MS,NULL);
							}
							break;
						case AP_9:
						default:
							{
								m_bActiveHighAlarm=false;
								m_bActiveMediumAlarm=false;
								m_bActiveLowAlarm=true;
								SetTimer(INFOALARMTIMER,ALARMTIME800MS,NULL);
							}
							break;

						}

						/*m_bSignalHighAlarm=false;
						m_bSignalMediumAlarm=false;
						m_bSignalLowAlarm=false;*/

						/*m_bSignalTubusDisconAlarm=false;
						m_bSignalPatientAlarm=false;*/
						m_bSignalAlarm=false;
						m_szSigaledAlarmText=_T("");
						
						PostMessage(WM_DTB_REFRESH_INFO_NUM);
					}
					/*else
					{

					}*/
				}
				break;
			case CMVEventAlarm::EV_ALARMSTATE_CHANGED:
				{
					if(getModel()->getDATAHANDLER()->isNurscallAlarm() 
						&& getModel()->getCONFIG()->IsNursecallActive())
					{
						if(m_bNurscallAlarm==false)
						{
#ifndef SIMULATION_ENTREK //rkuNEWFIX
							SetTimer(NURSECALLTIMER,ALARMTIMENURSE,NULL);
#endif
						}
						m_bNurscallAlarm=true;
						m_bNurscallActive=true;
					}
					else if(getModel()->getCONFIG()->IsNursecallActive())
					{
						if(m_bNurscallAlarm==true)
						{
							KillTimer(NURSECALLTIMER);
						}
						m_bNurscallAlarm=false;
						m_bNurscallActive=true;
					}
					else
					{
						KillTimer(NURSECALLTIMER);
						m_bNurscallAlarm=false;
						m_bNurscallActive=false;
					}

					m_bServiceASilent=false;
					m_bSystemASilent=false;
					eVentSilentState stateVentSilent = getModel()->getALARMHANDLER()->getAlarmSilentState();
					switch(stateVentSilent)
					{
					case ASTATE_SILENT:
						{
							m_bServiceASilent=true;
						}
						break;
					case ASTATE_SYSTEM_SILENT:
						{
							m_bSystemASilent=true;
						}
						break;
					}

//checkhere
					if(		getModel()->getALARMHANDLER()->isActiveAlarm()
						&&	stateVentSilent!=ASTATE_SILENT
						&&	stateVentSilent!=ASTATE_SYSTEM_SILENT)
					{
						if(m_bScreenlockActive)
							m_bScreenlockActive=false;

						if(getModel()->getALARMHANDLER()->isAlarmType(AT_POWER))
						{
							m_iBattStatMinutes=getModel()->getDATAHANDLER()->GetBatteryStatMinutes();
							m_iBattStatPercents=getModel()->getDATAHANDLER()->GetBatteryStatPercent();
							if(m_iBattStatPercents>100)
								m_iBattStatPercents=100;
							DrawLine(true,true,true);
						}

						m_bSignalAlarm=false;
						m_szSigaledAlarmText=_T("");

						eAlarmPrio prio=getModel()->getALARMHANDLER()->getPrioActiveAlarm();
						switch(prio)
						{
						case AP_1:
						case AP_2:
						case AP_3:
						case AP_4:
						case AP_5:
						case AP_6:
						case AP_7:
							{
								m_bActiveHighAlarm=true;
								m_bActiveMediumAlarm=false;
								m_bActiveLowAlarm=false;
								SetTimer(INFOALARMTIMER,ALARMTIME300MS,NULL);
							}
							break;
						case AP_8:
							{
								m_bActiveHighAlarm=false;
								m_bActiveMediumAlarm=true;
								m_bActiveLowAlarm=false;
								SetTimer(INFOALARMTIMER,ALARMTIME800MS,NULL);
							}
							break;
						case AP_9:
						default:
							{
								m_bActiveHighAlarm=false;
								m_bActiveMediumAlarm=false;
								m_bActiveLowAlarm=true;
								//SetTimer(INFOALARMTIMER,ALARMTIME800MS,NULL);
								KillTimer(INFOALARMTIMER);
							}
							break;

						}
					}
					else if(getModel()->getALARMHANDLER()->isSignaledAlarm()==true
						&&	stateVentSilent!=ASTATE_SILENT
						&&	stateVentSilent!=ASTATE_SYSTEM_SILENT)
					{
						m_bActiveHighAlarm=false;
						m_bActiveMediumAlarm=false;
						m_bActiveLowAlarm=false;
						

						m_bSignalAlarm=true;

						m_szSigaledAlarmText=getModel()->getALARMHANDLER()->getAlarmText(getModel()->getALARMHANDLER()->getSignaledAlarm());

						KillTimer(INFOALARMTIMER);
					}
					else
					{
						m_bSignalAlarm=false;
						m_szSigaledAlarmText=_T("");

						m_bActiveHighAlarm=false;
						m_bActiveMediumAlarm=false;
						m_bActiveLowAlarm=false;
						KillTimer(INFOALARMTIMER);
					}
					PostMessage(WM_DTB_REFRESH_INFO_NUM);
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case CMVEvent::ET_INFOTEXT:
		{
			CMVEventInfotext* pUIEvent = (CMVEventInfotext*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventInfotext::EV_CLEAR_TEXT:
				{
					m_bDrawInfoText=false;
					m_szInfoText=_T("");
					KillTimer(INFOTXTTIMER);
					PostMessage(WM_DTB_REFRESH_INFO);
				}
				break;
			case CMVEventInfotext::EV_TEXT:
				{
					m_bDrawInfoText=true;
					m_szInfoText=pUIEvent->GetData();
					KillTimer(INFOTXTTIMER);
					DrawLine(false,true,false);
					m_bDrawInfoText=false;
				}
				break;
			case CMVEventInfotext::EV_TIMETEXT:
				{
					m_bDrawInfoText=true;
					m_szInfoText=pUIEvent->GetData();
					int iTime=pUIEvent->GetIData();
					SetTimer(INFOTXTTIMER,iTime,NULL);
					//SetTimer(INFOTXTTIMER,2000,NULL);
					PostMessage(WM_DTB_REFRESH_INFO);

				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case CMVEvent::ET_MATRIX://++++++++++++++++++++++++++++++Buttons MATRIX+++++++++++++++++++++++++++++++++++++++++
		{
			CMVEventMatrix* pUIEvent = (CMVEventMatrix*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventMatrix::EV_BN_MATRIX_SYSALARM_SILENT:
				{
					if(getModel()->getALARMHANDLER()->getAlarmSilentState()==ASTATE_SYSTEM_SILENT)
					{
						m_bSystemASilent=true;
					}
					else
					{
						m_bSystemASilent=false;
					}

					PostMessage(WM_DTB_REFRESH_NUM);
				}
				break;
			case CMVEventMatrix::EV_BN_MATRIX_ALARM_SILENT:
				{
					if(m_bNurscallAlarm && m_bNurscallActive)
					{
						KillTimer(NURSECALLTIMER);
						m_bNurscallAlarm=false;

						DrawLine(true,false,false);
					}
					/*else
					{
						DrawLine(true,false,true);
					}*/

				}
				break;
			default:
				break;
			}
		}
		break;
	}
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewDTBField::OnDestroy()
{
	Close();

	CMVView::OnDestroy();
}

/**********************************************************************************************//**
 * Notifies the vent mode changed
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewDTBField::NotifyVentModeChanged()
{
	//DEBUGMSG(TRUE, (TEXT("CViewDTBField::NotifyVentModeChanged()\r\n")));
	DrawLine(true,true,true);
}

/**********************************************************************************************//**
 * Notifies a para button event
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param [in,out]	pEvent	If non-null, the event.
 **************************************************************************************************/

void CViewDTBField::NotifyParaBtnEvent(CMVEvent* pEvent)
{
	switch(pEvent->GetET())
	{
	case CMVEvent::ET_UIEVENT:
		{
			CMVEventUI* pUIEvent = (CMVEventUI*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventUI::EV_PARABN_VLIMIT:
			case CMVEventUI::EV_PARABN_VGARANT:
				{
					PostMessage(WM_DTB_REFRESH_INFO);
				}
				break;
			default:

				break;
			}
		}

	}
}

/**********************************************************************************************//**
 * Sets time o 2 flush
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	szTime	The time.
 **************************************************************************************************/

void CViewDTBField::SetTimeO2Flush(CStringW szTime)
{
	m_bCountDownOxyFlush=true;
	m_szOxyFlushTime=szTime;
	if(!m_bPSVapnoeTimer)
		PostMessage(WM_DTB_REFRESH_INFO);
}

/**********************************************************************************************//**
 * Stops o 2 flush
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewDTBField::StopO2Flush()
{
	if(m_bCountDownOxyFlush==false)
		return;

	m_bCountDownOxyFlush=false;
	m_szOxyFlushTime=_T("");
	if(!m_bPSVapnoeTimer)
		PostMessage(WM_DTB_REFRESH_INFO);
}

/**********************************************************************************************//**
 * Sets time until start vent
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	iCountDown	Zero-based index of the count down.
 **************************************************************************************************/

void CViewDTBField::SetTimeUntilStartVent(int iCountDown)
{
	if(m_bPSVapnoeTimer)
	{
		KillTimer(PSVAPNOETIMER);
		m_bPSVapnoeTimer=false;
	}
	m_bCountDownStartVent=true;
	m_iCounterStartVent=iCountDown;
	PostMessage(WM_DTB_REFRESH_INFO);
}

/**********************************************************************************************//**
 * Stops time until start vent
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewDTBField::StopTimeUntilStartVent()
{
	m_bCountDownStartVent=false;
	m_iCounterStartVent=-1;
	PostMessage(WM_DTB_REFRESH_INFO);
}

/**********************************************************************************************//**
 * Sets time alarm silent
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	iCountDown	Zero-based index of the count down.
 **************************************************************************************************/

void CViewDTBField::SetTimeAlarmSilent(int iCountDown)
{
	m_bCountDownASilent=true;
	m_iCounterASilent=iCountDown;
	
	PostMessage(WM_DTB_REFRESH_NUM);
}

/**********************************************************************************************//**
 * Stops alarm silent
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewDTBField::StopAlarmSilent()
{
	m_bCountDownASilent=false;
	m_iCounterASilent=-1;
	PostMessage(WM_DTB_REFRESH_NUM);
}

/**********************************************************************************************//**
 * Sets time until stop vent
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	iCountDown	Zero-based index of the count down.
 **************************************************************************************************/

void CViewDTBField::SetTimeUntilStopVent(int iCountDown)
{
	if(m_bPSVapnoeTimer)
	{
		KillTimer(PSVAPNOETIMER);
		m_bPSVapnoeTimer=false;
	}
	m_bCountDownStopVent=true;
	m_iCounterStopVent=iCountDown;
	PostMessage(WM_DTB_REFRESH_INFO_NUM);
}

/**********************************************************************************************//**
 * Stops time until stop vent
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewDTBField::StopTimeUntilStopVent()
{
	m_bCountDownStopVent=false;
	m_iCounterStopVent=-1;
	//m_szHeaderText=_T("");
	PostMessage(WM_DTB_REFRESH_INFO_NUM);
}

/**********************************************************************************************//**
 * Sets time until turn off
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	iCountDown	Zero-based index of the count down.
 **************************************************************************************************/

void CViewDTBField::SetTimeUntilTurnOff(int iCountDown)
{
	if(m_bPSVapnoeTimer)
	{
		KillTimer(PSVAPNOETIMER);
		m_bPSVapnoeTimer=false;
	}
	m_bCountDownTurnOff=true;
	m_iCounterTurnOff=iCountDown;
	PostMessage(WM_DTB_REFRESH_INFO_NUM);
}

/**********************************************************************************************//**
 * Stops time until turn off
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewDTBField::StopTimeUntilTurnOff()
{
	m_bCountDownTurnOff=false;
	m_iCounterTurnOff=-1;
	//m_szHeaderText=_T("");
	PostMessage(WM_DTB_REFRESH_INFO_NUM);
}

/**********************************************************************************************//**
 * Window proc
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	message	The message.
 * \param	wParam 	The wParam field of the message.
 * \param	lParam 	The lParam field of the message.
 *
 * \return	A LRESULT.
 **************************************************************************************************/

LRESULT CViewDTBField::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_SETVIEWFOCUS:
		{
			//rku cs1
			SetViewFocus();
			return 1;
		}
		break;
	case WM_SETPREVFOCUS:
		{
			//rku cs1
			SetPrevFocus();
			return 1;
		}
		break;
	case WM_SETNEXTFOCUS:
		{
			//rku cs1
			SetNextFocus();
			return 1;
		}
		break;case WM_STOPOXIFLUSH:
		{
			StopO2Flush();
			return 1;
		}
		break;
	case WM_STOPALARMSILENT:
		{
			StopAlarmSilent();
			return 1;
		}
		break;
	case WM_DTB_REFRESH_INFO:
		{
			DrawLine(false,true,false);
			return 1;
		}
		break;
	case WM_DTB_REFRESH_NUM:
		{
			DrawLine(false,false,true);
			return 1;
		}
		break;
	case WM_DTB_REFRESH_INFO_NUM:
		{
			DrawLine(false,true,true);
			return 1;
		}
		break;
	case WM_STOPTIMEUNTILSTARTVENT:
		{
			StopTimeUntilStartVent();
			return 1;
		}
		break;
	case WM_STOPTIMEUNTILSTOPVENT:
		{
			StopTimeUntilStopVent();
			return 1;
		}
		break;
	case WM_STOPTIMEUNTILTURNOFF:
		{
			StopTimeUntilTurnOff();
			return 1;
		}
		break;
	case WM_SETCPAPBACKUP_TRUE:
		{
			//rku cs1
			SetCPAPBackup(true);
			return 1;
		}
		break;
	case WM_SETCPAPBACKUP_FALSE:
		{
			//rku cs1
			SetCPAPBackup(false);
			return 1;
		}
		break;
	case WM_SETPSVAPNOE_TRUE:
		{
			//rku cs1
			setPSVapnoe(true);
			return 1;
		}
		break;
	case WM_SETPSVAPNOE_FALSE:
		{
			//rku cs1
			setPSVapnoe(false);
			return 1;
		}
		break;
	case WM_SETMAINTENANCE_TRUE:
		{
			//rku cs1
			setMaintenanceFlag(true);
			return 1;
		}
		break;
	case WM_SETMAINTENANCE_FALSE:
		{
			//rku cs1
			setMaintenanceFlag(false);
			return 1;
		}
		break;
	case WM_SETPRICO_TRUE:
		{
			//rku cs1
			setPRICOrunning(true);
			return 1;
		}
		break;
	case WM_SETPRICO_FALSE:
		{
			//rku cs1
			setPRICOrunning(false);
			return 1;
		}
		break;
	case WM_DATE_CHANGED:
		{
			DrawLine(false,false,true);
			return 1;
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

/**********************************************************************************************//**
 * Sets pric orunning
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CViewDTBField::setPRICOrunning(bool state)
{
	m_bPRICOrunning=state;
	
	if(state)
	{
		m_iPRICOCount=1;
		KillTimer(PRICOTIMER);

		PostMessage(WM_DTB_REFRESH_NUM);
#ifndef SIMULATION_ENTREK //rkuNEWFIX
		SetTimer(PRICOTIMER, 100, NULL);
#endif
	}
	else
	{
		m_iPRICOCount=0;
		KillTimer(PRICOTIMER);

		PostMessage(WM_DTB_REFRESH_NUM);
	}
}

/**********************************************************************************************//**
 * Draw pric oradar
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 **************************************************************************************************/

void CViewDTBField::drawPRICOradar(CDC* pDC)
{
	HDC hdc = *pDC;

	SelectObject(hdc,(HPEN)GetStockObject(NULL_PEN));

	if(m_iPRICOCount>24)
		m_iPRICOCount=1;

	switch(m_iPRICOCount)
	{
	case 1:
		{
			m_pcPRICO=m_pcPRICO1;
		}
		break;
	case 2:
		{
			m_pcPRICO=m_pcPRICO2;
		}
		break;
	case 3:
		{
			m_pcPRICO=m_pcPRICO3;
		}
		break;
	case 4:
		{
			m_pcPRICO=m_pcPRICO4;
		}
		break;
	case 5:
		{
			m_pcPRICO=m_pcPRICO5;
		}
		break;
	case 6:
		{
			m_pcPRICO=m_pcPRICO6;
		}
		break;
	case 7:
		{
			m_pcPRICO=m_pcPRICO7;
		}
		break;
	case 8:
		{
			m_pcPRICO=m_pcPRICO8;
		}
		break;
	case 9:
		{
			m_pcPRICO=m_pcPRICO9;
		}
		break;
	case 10:
		{
			m_pcPRICO=m_pcPRICO10;
		}
		break;
	case 11:
		{
			m_pcPRICO=m_pcPRICO11;
		}
		break;
	case 12:
		{
			m_pcPRICO=m_pcPRICO12;
		}
		break;
	case 13:
		{
			m_pcPRICO=m_pcPRICO13;
		}
		break;
	case 14:
		{
			m_pcPRICO=m_pcPRICO14;
		}
		break;
	case 15:
		{
			m_pcPRICO=m_pcPRICO15;
		}
		break;
	case 16:
		{
			m_pcPRICO=m_pcPRICO16;
		}
		break;
	case 17:
		{
			m_pcPRICO=m_pcPRICO17;
		}
		break;
	case 18:
		{
			m_pcPRICO=m_pcPRICO18;
		}
		break;
	case 19:
		{
			m_pcPRICO=m_pcPRICO19;
		}
		break;
	case 20:
		{
			m_pcPRICO=m_pcPRICO20;
		}
		break;
	case 21:
		{
			m_pcPRICO=m_pcPRICO21;
		}
		break;
	case 22:
		{
			m_pcPRICO=m_pcPRICO22;
		}
		break;
	case 23:
		{
			m_pcPRICO=m_pcPRICO23;
		}
		break;
	case 24:
		{
			m_pcPRICO=m_pcPRICO24;
		}
		break;
	default:
		{
			m_pcPRICO=NULL;
		}
		break;
	}

	if(m_pcPRICO)
		m_pcPRICO->Draw(hdc,125,2);
}

/**********************************************************************************************//**
 * Draw line
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bDrawStateLine  	True to draw state line.
 * \param	bDrawInfoLine   	True to draw information line.
 * \param	bDrawNumericLine	True to draw numeric line.
 **************************************************************************************************/

void CViewDTBField::DrawLine(bool bDrawStateLine,bool bDrawInfoLine,bool bDrawNumericLine)
{
	CClientDC dc(this);

	if(bDrawStateLine)
	{
		DrawStateLine(dc.m_hDC);
	}
	
	Sleep(0);

	if(bDrawInfoLine)
	{
		DrawInfoLine(dc.m_hDC);
	}

	Sleep(0);

	if(bDrawNumericLine)
	{
		DrawNumericLine(dc.m_hDC);
	}

	Sleep(0);

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

/**********************************************************************************************//**
 * Draw numeric line
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	hdc	The hdc.
 **************************************************************************************************/

void CViewDTBField::DrawNumericLine(HDC hdc)
{
	EnterCriticalSection(&csInfo);
	try
	{
		RECT rcCl,rc;

		rcCl.left = 610;  
		rcCl.top = 0;  
		rcCl.right  = 800;  
		rcCl.bottom = 41;

		rc.left = 0;  
		rc.top = 0;  
		rc.right  = 190;  
		rc.bottom = 41;

		HFONT hPrevFont=(HFONT)SelectObject(m_hdcNumericLine,g_hf10AcuBold);
		HPEN hpenprevGraph=(HPEN)SelectObject(m_hdcNumericLine, (HPEN)GetStockObject(NULL_PEN));

		int bc=SetBkColor(m_hdcNumericLine,BACKGND);
		int nBkMode=SetBkMode(m_hdcNumericLine,TRANSPARENT);
		int tc=SetTextColor(m_hdcNumericLine,0x0000000);

		CBrush brBack(BACKGND);
		FillRect(m_hdcNumericLine,&rc,brBack);

		//###################draw numeric line############################################################
		if(m_bCountDownASilent && (m_iCounterASilent%2==0))
		{
			m_bDrawYellowNumHeader=true;
		}
		else
		{
			m_bDrawYellowNumHeader=false;
		}


		if(m_bDrawYellowNumHeader)
		{
			

			if(m_bPRICOrunning)
			{
				if(m_pcGreyNumHeader)
					m_pcGreyNumHeader->Draw(m_hdcNumericLine,0,0);
				CBrush brYellow;
				brYellow.CreateSolidBrush(RGB(255,255,0));

				HBRUSH hPrevBrush = (HBRUSH)SelectObject(m_hdcNumericLine, (HBRUSH)brYellow);	
				HPEN hPrevPen = (HPEN)SelectObject(m_hdcNumericLine, (HPEN)GetStockObject(NULL_PEN));

				rc.left = 0;  
				rc.top = 0;  
				rc.right  = 124;  
				rc.bottom = 41;
				FillRect(m_hdcNumericLine,&rc,brYellow);
				SelectObject(m_hdcNumericLine, hPrevBrush);	
				SelectObject(m_hdcNumericLine, hPrevPen);
			}
			else
			{
				if(m_pcYellowNumHeader)
					m_pcYellowNumHeader->Draw(m_hdcNumericLine,0,0);
			}
		}
		else
		{
			if(m_pcGreyNumHeader)
				m_pcGreyNumHeader->Draw(m_hdcNumericLine,0,0);
		}

		if(getModel()->getVIEWHANDLER()->getViewState()==VS_ALARM_LIMIT)
		{
			if(m_Clock.IsClockRunning())
			{
				m_Clock.Stop();
				m_Clock.ShowWindow(SW_HIDE);
			}
			if(m_bPRICOrunning)
			{
				CDC* pDC=CDC::FromHandle(m_hdcNumericLine);
				drawPRICOradar(pDC);
			}
			else
			{
				SelectObject(m_hdcNumericLine,g_hf9AcuBold);

				rc.left = 35;
				rc.top = 0;  
				rc.right  = 185;
				rc.bottom = 41;
				DrawText(m_hdcNumericLine,getModel()->GetLanguageString(IDS_TXT_ALIMITS),-1,&rc,DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

			}
		}
		else
		{
			CBrush cbrRound;//(RGB(200,200,200));
			cbrRound.CreateSolidBrush(RGB(200,200,200));
			CBrush brGey;//(RGB(176,176,177));
			brGey.CreateSolidBrush(RGB(176,176,177));
			CPen penLine2(PS_SOLID,1,RGB(140,140,140));
			//penLine2.CreatePen(PS_SOLID,1,RGB(140,140,140));
			
			HBRUSH hPrevBrush = (HBRUSH)SelectObject(m_hdcNumericLine, (HBRUSH)cbrRound);	
			HPEN hPrevPen = (HPEN)SelectObject(m_hdcNumericLine, (HPEN)GetStockObject(NULL_PEN));

			rc.left = 115;  
			rc.top = 0;  
			rc.right  = 190;  
			rc.bottom = 41;
			FillRect(m_hdcNumericLine,&rc,brGey);
			

			if(m_bPRICOrunning)
			{
				if(m_Clock.IsClockRunning())
				{
					m_Clock.Stop();
					m_Clock.ShowWindow(SW_HIDE);
				}

				CDC* pDC=CDC::FromHandle(m_hdcNumericLine);
				drawPRICOradar(pDC);
			}
			else //show clock and date
			{
				if(m_Clock.IsClockRunning()==false)
				{
					m_Clock.ShowWindow(SW_SHOW);
					m_Clock.Run();
				}

				SelectObject(m_hdcNumericLine,penLine2);
				//SelectObject(m_hdcNumericLine,cbrRound);
				RoundRect(m_hdcNumericLine, 117, 7, 320, 36,10,10);

				
				SYSTEMTIME st;

				SelectObject(m_hdcNumericLine,g_hf14AcuNormNum);

				GetLocalTime(&st);
				COleDateTime dtTime(st);

				CStringW csDayMonth = dtTime.Format(_T("%d/%m"));
				CStringW csYear = dtTime.Format(_T("%Y"));

				rc.left = 100;  
				rc.top = 10;  
				rc.right  = 149;  
				rc.bottom = 35;

				DrawText(m_hdcNumericLine,csDayMonth,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);
				DrawText(m_hdcNumericLine,csYear,-1,&rc,DT_RIGHT|DT_BOTTOM|DT_SINGLELINE);
			}
			

			SelectObject(m_hdcNumericLine, hPrevBrush);	
			SelectObject(m_hdcNumericLine, hPrevPen);
			//DeleteObject(cbrRound);
			//DeleteObject(brGey);
			//DeleteObject(penLine2);
		}

		SelectObject(m_hdcNumericLine,g_hf15AcuMed);

		if(m_bCountDownTurnOff)
		{
			if(m_pcAlarmOffGrey)
				m_pcAlarmOffGrey->Draw(m_hdcNumericLine,10,5);
		}
		else if(m_bCountDownStopVent)
		{
			if(m_pcAlarmOffGrey)
				m_pcAlarmOffGrey->Draw(m_hdcNumericLine,10,5);
		}
		else if(m_bCountDownStartVent)
		{
			if(m_pcAlarmOffGrey)
				m_pcAlarmOffGrey->Draw(m_hdcNumericLine,10,5);
		}
		else if(m_bSTANDBYMode)
		{
			if(m_pcAlarmOffGrey)
				m_pcAlarmOffGrey->Draw(m_hdcNumericLine,10,5);
		}
		else if(m_bSystemASilent)
		{
			if(m_pcAlarmOffGrey)
				m_pcAlarmOffGrey->Draw(m_hdcNumericLine,10,5);

			rc.left = 35;
			rc.top = 0;  
			rc.right  = 190;
			rc.bottom = 41;
			DrawText(m_hdcNumericLine,getModel()->GetLanguageString(IDS_ALARM_SYSAL),-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}
		else if(m_bServiceASilent)
		{
			if(m_pcAlarmOffGrey)
				m_pcAlarmOffGrey->Draw(m_hdcNumericLine,10,5);
		}
		else if(m_bCountDownASilent)
		{
			if(m_bDrawYellowNumHeader)
			{
				if(m_pcAlarmSilentYellow)
					m_pcAlarmSilentYellow->Draw(m_hdcNumericLine,10,5);
			}
			else
			{
				if(m_pcAlarmSilentGrey)
					m_pcAlarmSilentGrey->Draw(m_hdcNumericLine,10,5);
			}

			if(m_iCounterASilent>=0)
			{
				int iMin=m_iCounterASilent/60;
				int iSec=0;
				if(iMin>0)
				{
					iSec=m_iCounterASilent-(iMin*60);
				}
				else
				{
					iSec=m_iCounterASilent;
				}
				if(iSec<10)
				{
					m_szCounterText.Format(_T("%d:0%d"), iMin,iSec);
				}
				else
				{
					m_szCounterText.Format(_T("%d:%d"), iMin,iSec);
				}

				rc.left = 40;
				rc.top = 0;  
				rc.right  = 190;
				rc.bottom = 41;
				DrawText(m_hdcNumericLine,m_szCounterText,-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			}
		}
		/*else if(m_bSignalPatientAlarm || m_bSignalTubusDisconAlarm)
		{
			if(m_pcAlarmSignaled)
				m_pcAlarmSignaled->Draw(m_hdcNumericLine,10,6);
		}*/

		BitBlt(m_hDC,rcCl.left,rcCl.top,rcCl.right,rcCl.bottom,m_hdcNumericLine,0,0,SRCCOPY);

		SelectObject(m_hdcNumericLine, hpenprevGraph);
		SelectObject(m_hdcNumericLine,hPrevFont);
		SetTextColor(m_hdcNumericLine,tc);
		SetBkColor(m_hdcNumericLine,bc);
		SetBkMode(m_hdcNumericLine,nBkMode);

		//brBack.DeleteObject();//rkuNEWFIX
	}
	catch (...)
	{
		LeaveCriticalSection(&csInfo);
		theApp.ReportException(_T("CViewDTBField::DrawNumericLine"));
	}
	LeaveCriticalSection(&csInfo);


	
}

/**********************************************************************************************//**
 * Draw information line
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	hdc	The hdc.
 **************************************************************************************************/

void CViewDTBField::DrawInfoLine(HDC hdc)
{
	EnterCriticalSection(&csInfo);
	try
	{
		RECT rcCl,rc;

		rcCl.left = 150;  
		rcCl.top = 0;  
		rcCl.right  = 610;//800;  
		rcCl.bottom = 41;

		rc.left = 0;  
		rc.top = 0;  
		rc.right  = 460;  
		rc.bottom = 41;

		HFONT hPrevFont=(HFONT)SelectObject(m_hdcInfoLine,g_hf10AcuBold);
		HPEN hpenprevGraph=(HPEN)SelectObject(m_hdcInfoLine, (HPEN)GetStockObject(NULL_PEN));

		int bc=SetBkColor(m_hdcInfoLine,BACKGND);
		int nBkMode=SetBkMode(m_hdcInfoLine,TRANSPARENT);
		int tc=SetTextColor(m_hdcInfoLine,0x0000000);

		CBrush brBack(BACKGND);
		FillRect(m_hdcInfoLine,&rc,brBack);

		m_szInfolineText=_T("");


		if(m_bScreenlockActive && theApp.IsScreenlock()==false)
		{
			m_bScreenlockActive=false;
		}

		if(getModel()->getDATAHANDLER()->isPSVapnoe() || m_bCPAPBackup)
		{
			m_bPSVapnoe=true;
		}
		else
		{
			m_bPSVapnoe=false;
		}

		//###################draw info line############################################################

		SetTextColor(m_hdcInfoLine,RGB(0,0,0));
		SelectObject(m_hdcInfoLine,g_hf15AcuMed);

		if(m_bShowPresureVal)
		{
			if(m_pcGreyInfoline)
				m_pcGreyInfoline->Draw(m_hdcInfoLine);
			CBrush brOff(RGB(0,0,255));
			CPen penOff(PS_SOLID,1,RGB(0,0,255));

			HBRUSH hPrevBrush = (HBRUSH)SelectObject(m_hdcInfoLine, (HBRUSH)brOff);	
			HPEN hPrevPen = (HPEN)SelectObject(m_hdcInfoLine, (HPEN)penOff);

			SelectObject(m_hdcInfoLine,g_hf11AcuBold);

			rc.left = 100;  
			rc.top = 3;  
			rc.right  = 450;  
			rc.bottom = 38;

			/*int iAir=getModel()->getSPI()->Read_DRM_1_ADC();
			int iOxy=getModel()->getSPI()->Read_DRM_2_ADC();*/

			CString szAir=_T("");
			CString szOxy=_T("");
			CString szPPROX=_T("");
			CString szP_MIX=_T("");

			szAir.Format(_T("P-AIR: %d"), m_iAir);
			szOxy.Format(_T("P-OXY: %d"), m_iOxy);
			szPPROX.Format(_T("PPROX: %d"), m_iPPROX_Data);
			szP_MIX.Format(_T("P-insp: %d"), m_iP_MIX_Data);

			DrawText(m_hdcInfoLine,szAir,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
			DrawText(m_hdcInfoLine,szOxy,-1,&rc,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);

			rc.left = 260;  

			DrawText(m_hdcInfoLine,szPPROX,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
			DrawText(m_hdcInfoLine,szP_MIX,-1,&rc,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);

			SelectObject(m_hdcInfoLine, hPrevBrush);	
			SelectObject(m_hdcInfoLine, hPrevPen);
			//DeleteObject(brOff);//rkuNEWFIX
			//DeleteObject(penOff);//rkuNEWFIX
		}
		else if(m_bCountDownTurnOff)
		{
			if(m_pcGreyInfoline)
				m_pcGreyInfoline->Draw(m_hdcInfoLine);
			CBrush brOff(RGB(0,0,255));
			CPen penOff(PS_SOLID,1,RGB(0,0,255));
			
			HBRUSH hPrevBrush = (HBRUSH)SelectObject(m_hdcInfoLine, (HBRUSH)brOff);	
			HPEN hPrevPen = (HPEN)SelectObject(m_hdcInfoLine, (HPEN)penOff);

			int iDots=5-m_iCounterTurnOff;

			for(int i = 0; i<iDots ; i++)
			{
				int iLeft=190-(i*25)-(i*5);
				int iTop=8;
				int iRight=(190-(i*25))+25-(i*5);
				int iBottom=33;
				Ellipse(m_hdcInfoLine,iLeft,iTop,iRight,iBottom);
			}

			SelectObject(m_hdcInfoLine,g_hf11AcuBold);

			rc.left = 240;  
			rc.top = 3;  
			rc.right  = 450;  
			rc.bottom = 38;
			DrawText(m_hdcInfoLine,getModel()->GetLanguageString(IDS_TXT_RESPOFF1),-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
			DrawText(m_hdcInfoLine,getModel()->GetLanguageString(IDS_TXT_RESPOFF2),-1,&rc,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);

			SelectObject(m_hdcInfoLine, hPrevBrush);	
			SelectObject(m_hdcInfoLine, hPrevPen);
			//DeleteObject(brOff);//rkuNEWFIX
			//DeleteObject(penOff);//rkuNEWFIX
		}
		else if(m_bCountDownStopVent)
		{
			if(m_pcGreyInfoline)
				m_pcGreyInfoline->Draw(m_hdcInfoLine);
			CBrush brBreak(RGB(0,255,0));
			CPen penBreak(PS_SOLID,1,RGB(0,255,0));
			
			HBRUSH hPrevBrush = (HBRUSH)SelectObject(m_hdcInfoLine, (HBRUSH)brBreak);	
			HPEN hPrevPen = (HPEN)SelectObject(m_hdcInfoLine, (HPEN)penBreak);

			int iDots=5-m_iCounterStopVent;

			for(int i = 0; i<iDots ; i++)
			{
				int iLeft=190-(i*25)-(i*5);
				int iTop=8;
				int iRight=(190-(i*25))+25-(i*5);
				int iBottom=33;
				Ellipse(m_hdcInfoLine,iLeft,iTop,iRight,iBottom);
			}

			SelectObject(m_hdcInfoLine,g_hf11AcuBold);

			if(		getModel()->getCONFIG()->GetCurMode()==VM_NCPAP
				||	getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
				||	getModel()->getCONFIG()->GetCurMode()==VM_THERAPIE)
			{
				rc.left = 240;  
				rc.top = 0;  
				rc.right  = 450;  
				rc.bottom = 41;
				DrawText(m_hdcInfoLine,getModel()->GetLanguageString(IDS_STANDBY),-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			}
			else
			{
				rc.left = 240;  
				rc.top = 3;  
				rc.right  = 450;  
				rc.bottom = 38;
				DrawText(m_hdcInfoLine,getModel()->GetLanguageString(IDS_TXT_STOPVENT1),-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
				DrawText(m_hdcInfoLine,getModel()->GetLanguageString(IDS_TXT_STOPVENT2),-1,&rc,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
			}

			SelectObject(m_hdcInfoLine, hPrevBrush);	
			SelectObject(m_hdcInfoLine, hPrevPen);
			//DeleteObject(brBreak);//rkuNEWFIX
			//DeleteObject(penBreak);//rkuNEWFIX
		}
		else if(m_bCountDownStartVent)
		{
			//draw red alarm flash
			rc.left = 0;  
			rc.top = 0;  
			rc.right  = 455;  
			rc.bottom = 41;

			if(m_bPSVapnoeTimer)
			{
				KillTimer(PSVAPNOETIMER);
				m_bPSVapnoeTimer=false;
			}

			if(m_bDrawRedInfoline)
			{
				m_bDrawRedInfoline=false;
				if(m_pcRedInfoline)
					m_pcRedInfoline->Draw(m_hdcInfoLine);
				if(m_pcWarningRed)
					m_pcWarningRed->Draw(m_hdcInfoLine,10,10);
				DrawText(m_hdcInfoLine,getModel()->GetLanguageString(IDS_TXT_RESPIR_STOPPED),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
			}
			else 
			{
				m_bDrawRedInfoline=true;
				if(m_bScreenlockActive)
				{
					if(m_pcScreenlockInfoline)
						m_pcScreenlockInfoline->Draw(m_hdcInfoLine);
					DrawText(m_hdcInfoLine,getModel()->GetLanguageString(IDS_TXT_SCREENLOCK),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);

				}
				else
				{
					if(m_pcGreyInfoline)
						m_pcGreyInfoline->Draw(m_hdcInfoLine);
					if(m_pcWarningGrey)
						m_pcWarningGrey->Draw(m_hdcInfoLine,10,10);
					DrawText(m_hdcInfoLine,getModel()->GetLanguageString(IDS_TXT_RESPIR_STOPPED),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
				}
			}

			SelectObject(m_hdcInfoLine,g_hf15AcuMed);

			if(m_iCounterStartVent>=0)
			{
				int iMin=m_iCounterStartVent/60;
				int iSec=0;
				if(iMin>0)
				{
					iSec=m_iCounterStartVent-(iMin*60);
				}
				else
				{
					iSec=m_iCounterStartVent;
				}
				if(iSec<10)
				{
					m_szCounterText.Format(_T("%d:%d%d"), iMin,0,iSec);
				}
				else
				{
					m_szCounterText.Format(_T("%d:%d"), iMin,iSec);
				}

				rc.left = 38;
				rc.top = 0;  
				rc.right  = 470;  
				rc.bottom = 41;
				DrawText(m_hdcInfoLine,m_szCounterText,-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			}
		}
		else if(m_bSTANDBYMode)
		{
			rc.left = 0;  
			rc.top = 0;  
			rc.right  = 455;  
			rc.bottom = 41;

			if(m_bPSVapnoeTimer)
			{
				KillTimer(PSVAPNOETIMER);
				m_bPSVapnoeTimer=false;
			}

			//draw red alarm flash
			if(m_bDrawRedInfoline)
			{
				m_bDrawRedInfoline=false;
				if(m_pcRedInfoline)
					m_pcRedInfoline->Draw(m_hdcInfoLine);
				if(m_pcWarningRed)
					m_pcWarningRed->Draw(m_hdcInfoLine,10,10);
				DrawText(m_hdcInfoLine,getModel()->GetLanguageString(IDS_STANDBY),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
			}
			else 
			{
				m_bDrawRedInfoline=true;
				if(m_bScreenlockActive)
				{
					if(m_pcScreenlockInfoline)
						m_pcScreenlockInfoline->Draw(m_hdcInfoLine);
					DrawText(m_hdcInfoLine,getModel()->GetLanguageString(IDS_TXT_SCREENLOCK),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
				}
				else 
				{
					if(m_pcGreyInfoline)
						m_pcGreyInfoline->Draw(m_hdcInfoLine);
					if(m_pcWarningGrey)
						m_pcWarningGrey->Draw(m_hdcInfoLine,10,10);
					DrawText(m_hdcInfoLine,getModel()->GetLanguageString(IDS_STANDBY),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
				}
			}
		}
		else if(m_bActiveHighAlarm)
		{
			if(m_bPSVapnoeTimer)
			{
				KillTimer(PSVAPNOETIMER);
				m_bPSVapnoeTimer=false;
			}

			if(m_bScreenlockActive)
				m_bScreenlockActive=false;

			//draw red alarm flash
			if(m_bDrawRedInfoline)
			{
				if(m_pcRedInfoline)
					m_pcRedInfoline->Draw(m_hdcInfoLine);
			}
			else 
			{
				if(m_pcGreyInfoline)
					m_pcGreyInfoline->Draw(m_hdcInfoLine);
			}

			if(m_bDrawRedInfoline)
			{
				m_bDrawRedInfoline=false;
			}
			else 
			{
				m_bDrawRedInfoline=true;
			}

			rc.left = 0;  
			rc.top = 0;  
			rc.right  = 455;  
			rc.bottom = 41;
			DrawText(m_hdcInfoLine,GetActiveAlarmText(),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
		else if(m_bActiveMediumAlarm)
		{
			if(m_bPSVapnoeTimer)
			{
				KillTimer(PSVAPNOETIMER);
				m_bPSVapnoeTimer=false;
			}

			if(m_bScreenlockActive)
				m_bScreenlockActive=false;

			//draw red alarm flash
			if(m_bDrawYellowInfoline)
			{
				if(m_pcYellowInfoline)
					m_pcYellowInfoline->Draw(m_hdcInfoLine);
			}
			else 
			{
				if(m_pcGreyInfoline)
					m_pcGreyInfoline->Draw(m_hdcInfoLine);
			}

			if(m_bDrawYellowInfoline)
			{
				m_bDrawYellowInfoline=false;
			}
			else 
			{
				m_bDrawYellowInfoline=true;
			}

			rc.left = 0;  
			rc.top = 0;  
			rc.right  = 455;  
			rc.bottom = 41;
			DrawText(m_hdcInfoLine,GetActiveAlarmText(),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
		else if(m_bActiveLowAlarm)
		{
			if(m_bPSVapnoeTimer)
			{
				KillTimer(PSVAPNOETIMER);
				m_bPSVapnoeTimer=false;
			}

			if(m_bScreenlockActive)
				m_bScreenlockActive=false;

			if(m_pcYellowInfoline)
				m_pcYellowInfoline->Draw(m_hdcInfoLine);

			rc.left = 0;  
			rc.top = 0;  
			rc.right  = 455;  
			rc.bottom = 41;
			DrawText(m_hdcInfoLine,GetActiveAlarmText(),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);

		}
		else if(m_bFlowCalNeeded && (false == getModel()->getDATAHANDLER()->IsFlowSensorStateOff()))
		{
			rc.left = 0;  
			rc.top = 0;  
			rc.right  = 455;  
			rc.bottom = 41;

			if(m_bScreenlockActive)
			{
				if(m_pcScreenlockInfoline)
					m_pcScreenlockInfoline->Draw(m_hdcInfoLine);
			}
			else
			{
				if(m_pcRedInfoline)
					m_pcRedInfoline->Draw(m_hdcInfoLine);
			}

			//DrawText(m_hdcInfoLine,_T("! flow sensor calibration required !"),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
			DrawText(m_hdcInfoLine,getModel()->GetLanguageString(IDS_TXT_CALREQUIRED),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
		else if(m_bExhalValvCalRunning)
		{
			rc.left = 0;  
			rc.top = 0;  
			rc.right  = 455;  
			rc.bottom = 41;

			if(m_bScreenlockActive)
			{
				if(m_pcScreenlockInfoline)
					m_pcScreenlockInfoline->Draw(m_hdcInfoLine);
			}
			else
			{
				if(m_pcGreyInfoline)
					m_pcGreyInfoline->Draw(m_hdcInfoLine);
			}

			DrawText(m_hdcInfoLine,getModel()->GetLanguageString(IDS_EXHALVALV_CAL),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
		else if(m_bManualBreathRunning)
		{
			rc.left = 0;  
			rc.top = 0;  
			rc.right  = 455;  
			rc.bottom = 41;

			if(m_bScreenlockActive)
			{
				if(m_pcScreenlockInfoline)
					m_pcScreenlockInfoline->Draw(m_hdcInfoLine);
			}
			else
			{
				if(m_pcGreyInfoline)
					m_pcGreyInfoline->Draw(m_hdcInfoLine);
			}

			DrawText(m_hdcInfoLine,getModel()->GetLanguageString(IDS_TXT_MANBREATH_RUN),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
		else if(m_bOxyCalRunning)
		{
			rc.left = 0;  
			rc.top = 0;  
			rc.right  = 455;  
			rc.bottom = 41;

			if(m_bScreenlockActive)
			{
				if(m_pcScreenlockInfoline)
					m_pcScreenlockInfoline->Draw(m_hdcInfoLine);
			}
			else
			{
				if(m_pcGreyInfoline)
					m_pcGreyInfoline->Draw(m_hdcInfoLine);
			}

			DrawText(m_hdcInfoLine,getModel()->GetLanguageString(IDS_TXT_OXYCALRUN),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
		else if(m_bPSVapnoe)
		{

			if(!m_bPSVapnoeTimer)
			{
				m_bPSVapnoeTimer=true;
				SetTimer(PSVAPNOETIMER,PSVAPNOETIME,NULL);
			}
			rc.left = 0;  
			rc.top = 0;  
			rc.right  = 455;  
			rc.bottom = 41;

			if(m_bDrawOrangeInfoline)
			{
				m_bDrawOrangeInfoline=false;
				if(m_pcOrangeInfoline)
					m_pcOrangeInfoline->Draw(m_hdcInfoLine);
			}
			else
			{
				m_bDrawOrangeInfoline=true;
				if(m_bScreenlockActive)
				{
					if(m_pcScreenlockInfoline)
						m_pcScreenlockInfoline->Draw(m_hdcInfoLine);
				}
				else
				{
					if(m_pcGreyInfoline)
						m_pcGreyInfoline->Draw(m_hdcInfoLine);
				}
			}

			if(m_bCountDownOxyFlush)
			{
				CString szTem= m_szOxyFlushTime + _T(" / ");
				if(getModel()->getVMODEHANDLER()->activeModeIsVM_PSV())
				{
					szTem+=getModel()->GetLanguageString(IDS_ALARM_APNEAVENT_ACTIVE);
					//szTem+=_T("Apnoebeatmung aktiv");
				}
				else
				{
					szTem+=getModel()->GetLanguageString(IDS_ALARM_BACKUP_ACTIVE);
				}

				DrawText(m_hdcInfoLine,szTem,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
			}
			else
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsVM_PSV())
				{
					DrawText(m_hdcInfoLine,getModel()->GetLanguageString(IDS_ALARM_APNEAVENT_ACTIVE),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
					//DrawText(m_hdcInfoLine,_T("Apnoebeatmung aktiv"),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
				}
				else
				{
					DrawText(m_hdcInfoLine,getModel()->GetLanguageString(IDS_ALARM_BACKUP_ACTIVE),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
				}
			}
		}
		else if(m_bCountDownOxyFlush)
		{
			rc.left = 0;  
			rc.top = 0;  
			rc.right  = 455;  
			rc.bottom = 41;

			if(m_bScreenlockActive)
			{
				if(m_pcScreenlockInfoline)
					m_pcScreenlockInfoline->Draw(m_hdcInfoLine);
			}
			else
			{
				if(m_pcGreyInfoline)
					m_pcGreyInfoline->Draw(m_hdcInfoLine);
			}

			DrawText(m_hdcInfoLine,m_szOxyFlushTime,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
		else if(m_bScreenlockActive)
		{
			rc.left = 0;  
			rc.top = 0;  
			rc.right  = 455;  
			rc.bottom = 41;

			if(m_pcScreenlockInfoline)
				m_pcScreenlockInfoline->Draw(m_hdcInfoLine);

			DrawText(m_hdcInfoLine,getModel()->GetLanguageString(IDS_TXT_SCREENLOCK),-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
		else if(m_bSignalAlarm && getModel()->getCONFIG()->CurModeIsPresetMode()==false)
		{
			rc.left = 0;  
			rc.top = 0;  
			rc.right  = 455;  
			rc.bottom = 41;

			if(m_pcGreyInfoline)
				m_pcGreyInfoline->Draw(m_hdcInfoLine);

			if(m_pcAlarmSignaled)
				m_pcAlarmSignaled->Draw(m_hdcInfoLine,10,6);
			if(m_pcAlarmSignaled)
				m_pcAlarmSignaled->Draw(m_hdcInfoLine,421,6);

			/*CStringW szAlarmText=_T("");

			if(	getModel()->getALARMHANDLER()->getState_TUBUSBLOCKED()==AS_SIGNALED)
			{
				szAlarmText=getModel()->getALARMHANDLER()->getAlarmText(AL_TUBUSBLOCKED);
			}
			else if(	getModel()->getALARMHANDLER()->ALARM_DISCONNECTION->getAlarmState()==AS_SIGNALED)
			{
				szAlarmText=getModel()->getALARMHANDLER()->getAlarmText(AL_DISCONNECTION);
			}

			DrawText(m_hdcInfoLine,szAlarmText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);*/

//			if(m_szSigaledAlarmText==_T(""))
//			{
				m_szSigaledAlarmText=getModel()->getALARMHANDLER()->getAlarmText(getModel()->getALARMHANDLER()->getSignaledAlarm());
//			}

			DrawText(m_hdcInfoLine,m_szSigaledAlarmText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);

		}
		else
		{
			rc.left = 0;  
			rc.top = 0;  
			rc.right  = 455;  
			rc.bottom = 41;

			if(m_pcGreyInfoline)
				m_pcGreyInfoline->Draw(m_hdcInfoLine);

			SelectObject(m_hdcInfoLine,g_hf15AcuMed);

			SetTextColor(m_hdcInfoLine,0x0000000);

			m_szInfolineText=m_szInfoText;

			if(m_szInfolineText==_T(""))
			{
				if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn())
				{
					if(getModel()->GetLanguageID()==LAN_JAPANESE)
					{
						if(VM_SIMVPSV==getModel()->getCONFIG()->GetCurMode())
						{
							/*m_szInfolineText=getModel()->GetLanguageString(IDS_SIMV);
							m_szInfolineText+=_T("+VG + ");
							m_szInfolineText+=getModel()->GetLanguageString(IDS_PSV);*/
							m_szInfolineText=_T("SIMV");
							m_szInfolineText+=_T("+VG + ");
							m_szInfolineText+=_T("PSV");
						}
						else if(VM_PRE_SIMVPSV==getModel()->getCONFIG()->GetCurMode())
						{
							/*m_szInfolineText=getModel()->GetLanguageString(IDS_SIMV);
							m_szInfolineText+=_T("+VG + ");
							m_szInfolineText+=getModel()->GetLanguageString(IDS_PSV_PRE);*/
							m_szInfolineText=_T("SIMV");
							m_szInfolineText+=_T("+VG + ");
							m_szInfolineText+=_T("PSV プリセット");
						}
						else
						{
							m_szInfolineText=getModel()->getDATAHANDLER()->GetCurrentModeString();
							m_szInfolineText+=_T(" + VG");
						}
					}
					else
					{
						if(VM_SIMVPSV==getModel()->getCONFIG()->GetCurMode())
						{
							m_szInfolineText=getModel()->GetLanguageString(IDS_SIMV);
							m_szInfolineText+=_T("+VG + ");
							m_szInfolineText+=getModel()->GetLanguageString(IDS_PSV);
						}
						else if(VM_PRE_SIMVPSV==getModel()->getCONFIG()->GetCurMode())
						{
							m_szInfolineText=getModel()->GetLanguageString(IDS_SIMV);
							m_szInfolineText+=_T("+VG + ");
							m_szInfolineText+=getModel()->GetLanguageString(IDS_PSV_PRE);
						}
						else
						{
							m_szInfolineText=getModel()->getDATAHANDLER()->GetCurrentModeString();
							m_szInfolineText+=_T(" + VG");
						}
					}

					if(getModel()->getFOTThread())
					{
						if(getModel()->getFOTThread()->isFOTrunning())
							m_szInfolineText+=_T(" + FOT");
					}
					
				}
				else if(getModel()->getDATAHANDLER()->IsCurrentModeVLimitStateOn())
				{
					m_szInfolineText=getModel()->getDATAHANDLER()->GetCurrentModeString();
					m_szInfolineText+=_T(" + VL");

					if(getModel()->getFOTThread())
					{
						if(getModel()->getFOTThread()->isFOTrunning())
							m_szInfolineText+=_T(" + FOT");
					}
				}
				else
				{
					m_szInfolineText=getModel()->getDATAHANDLER()->GetCurrentModeString();

					if(getModel()->getFOTThread())
					{
						/*if(getModel()->getFOTThread()->getFOTstate()!=FOT_OFF)
							m_szInfolineText+=_T(" + FOT");*/
						if(getModel()->getFOTThread()->isFOTrunning())
							m_szInfolineText+=_T(" + FOT");
					}
				}
			}

			if(m_bMaintenanceNeeded)
			{
				if(m_pcMaintenance)
				{
					m_pcMaintenance->Draw(m_hdcInfoLine,15,8);
					m_pcMaintenance->Draw(m_hdcInfoLine,415,8);
				}
			}

			DrawText(m_hdcInfoLine,m_szInfolineText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}

		BitBlt(m_hDC,rcCl.left,rcCl.top,rcCl.right,rcCl.bottom,m_hdcInfoLine,0,0,SRCCOPY);

		SelectObject(m_hdcInfoLine, hpenprevGraph);
		SelectObject(m_hdcInfoLine,hPrevFont);

		SetBkColor(m_hdcInfoLine,bc);
		SetBkMode(m_hdcInfoLine,nBkMode);
		SetTextColor(m_hdcInfoLine,tc);

		//DeleteObject(brBack);
		//brBack.DeleteObject();//rkuNEWFIX
	}
	catch (...)
	{
		LeaveCriticalSection(&csInfo);

		theApp.ReportException(_T("CViewDTBField::DrawInfoLine"));
	}
	LeaveCriticalSection(&csInfo);

	
}

/**********************************************************************************************//**
 * Draw state line
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	hdc	The hdc.
 **************************************************************************************************/

void CViewDTBField::DrawStateLine(HDC hdc)
{
	EnterCriticalSection(&csInfo);
	try
	{
		RECT rcCl,rc;

		rcCl.left = 0;  
		rcCl.top = 0;  
		rcCl.right  = 150;  
		rcCl.bottom = 41;

		HFONT hPrevFont=(HFONT)SelectObject(m_hdcStatusLine,g_hf7AcuNorm);
		CDC* pDCMem=CDC::FromHandle(m_hdcStatusLine);

		int tc=SetTextColor(m_hdcStatusLine,RGB(0,0,0));
		int nBkMode=SetBkMode(m_hdcStatusLine,TRANSPARENT);

		CBrush brBack(BACKGND);

		FillRect(m_hdcStatusLine,&rcCl,brBack);


		if(m_pcBatState)
			m_pcBatState->Draw(m_hdcStatusLine);

		/*m_bNebulizerRunning=getModel()->getDATAHANDLER()->isNebulizerOn();
		if(m_bNebulizerRunning && m_bDrawNebulizer)
		{
			rcCl.left = 0;  
			rcCl.top = 0;  
			rcCl.right  = 75;  
			rcCl.bottom = 41;

			CBrush brYellow(RGB(255,255,0));

			FillRect(m_hdcStatusLine,&rcCl,brYellow);

			if(m_pcNebulizer)
				m_pcNebulizer->Draw(m_hdcStatusLine,7,6);
		}
		else*/
		{
			if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
			{
				if(m_pcNeonatal)
					m_pcNeonatal->Draw(m_hdcStatusLine,1,8);
			}
			else
			{
				if(m_pcPediatric)
					m_pcPediatric->Draw(m_hdcStatusLine,1,8);
			}

			if(getModel()->getDATAHANDLER()->isPatientDataAvailable())
			{
				m_bPatData=true;
				if(m_pcPatientData)
					m_pcPatientData->Draw(m_hdcStatusLine,27,10);
			}
			else
				m_bPatData=false;

			if(getModel()->getCONFIG()->IsHFOManBreathEnabled()==false && getModel()->getVMODEHANDLER()->activeModeIsHFO())
			{
				if(m_pcNoManATZ)
					m_pcNoManATZ->Draw(m_hdcStatusLine,51,3);
				rc.left = 52;  
				rc.top = 28;  
				rc.right  = 78;  
				rc.bottom = 41;
				DrawText(m_hdcStatusLine,getModel()->GetLanguageString(IDS_HFO),-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
			}
		}


		/*if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		{
			if(m_pcNeonatal)
				m_pcNeonatal->Draw(m_hdcStatusLine,1,8);
		}
		else
		{
			if(m_pcPediatric)
				m_pcPediatric->Draw(m_hdcStatusLine,1,8);
		}

		if(getModel()->getDATAHANDLER()->PatientDataAvailable())
		{
			m_bPatData=true;
			if(m_pcPatientData)
				m_pcPatientData->Draw(m_hdcStatusLine,27,10);
		}
		else
			m_bPatData=false;

		if(getModel()->getCONFIG()->IsHFOManBreathEnabled()==false && getModel()->getDATAHANDLER()->activeModeIsHFO())
		{
			if(m_pcNoManATZ)
				m_pcNoManATZ->Draw(m_hdcStatusLine,51,3);
			rc.left = 52;  
			rc.top = 28;  
			rc.right  = 78;  
			rc.bottom = 41;
			DrawText(m_hdcStatusLine,getModel()->GetLanguageString(IDS_HFO),-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
		}*/


		if(m_bNurscallActive && m_bNurscallAlarm)
		{
			if(m_bDrawNursecallRed)
			{
				if(m_pcNurseActive)
					m_pcNurseActive->Draw(m_hdcStatusLine,79,0);
			}
			else
			{
				if(m_pcNurse)
					m_pcNurse->Draw(m_hdcStatusLine,79,0);
			}
		}
		else if(m_bNurscallActive)
		{
			if(m_pcNurse)
				m_pcNurse->Draw(m_hdcStatusLine,79,0);
		}

		if(m_bExtrenalPower)
		{
			m_pcACCU=m_pcACCU_100;
		}
		else if(m_bAccuPower)
		{
			if(m_bACCUfull)
			{
				m_pcACCU=m_pcACCU_100;
			}
			else if(m_iBattStatPercents>90)
			{
				m_pcACCU=m_pcACCU_100;
			}
			else if(70<m_iBattStatPercents && m_iBattStatPercents<=90)
			{
				m_pcACCU=m_pcACCU_80;
			}
			else if(50<m_iBattStatPercents && m_iBattStatPercents<=70)
			{
				m_pcACCU=m_pcACCU_60;
			}
			else if(30<m_iBattStatPercents && m_iBattStatPercents<=50)
			{
				m_pcACCU=m_pcACCU_40;
			}
			else if(10<m_iBattStatPercents && m_iBattStatPercents<=30)
			{
				m_pcACCU=m_pcACCU_20;
			}
			else 
			{
				m_pcACCU=m_pcACCU_0;
			}
		}
		else	//m_bNetPower
		{
			if(m_bACCUfull)
				m_pcACCU=m_pcACCU_100;
			else	//m_bACCUcharging
			{
				switch(m_iCntAccuCharge)
				{
				case 0:
					{
						m_pcACCU=m_pcACCU_0;
					}
					break;
				case 1:
					{
						m_pcACCU=m_pcACCULOAD_20;
					}
					break;
				case 2:
					{
						m_pcACCU=m_pcACCULOAD_40;
					}
					break;
				case 3:
					{
						m_pcACCU=m_pcACCULOAD_60;
					}
					break;
				case 4:
					{
						m_pcACCU=m_pcACCULOAD_80;
					}
					break;
				case 5:
					{
						m_pcACCU=m_pcACCULOAD_100;
					}
					break;
				default:
					{
						m_pcACCU=m_pcACCU_0;
					}
					break;
				}
			}
		}
		SelectObject(m_hdcStatusLine,g_hf14AcuNormNum);

		CStringW cs=_T("");

		if(m_bAccuPower)
		{
			if(m_pcACCU!=NULL)
				m_pcACCU->Draw(m_hdcStatusLine,108,5);
			else
			{
				if(m_pcACCU_0)
					m_pcACCU_0->Draw(m_hdcStatusLine,108,5);
			}
			rc.left = 105;  
			rc.top = 22;  
			rc.right  = 145;  
			rc.bottom = 41;

			if(m_bDrawRemainingTime)
			{
				int iHours=m_iBattStatMinutes/60;
				int iMin=m_iBattStatMinutes-(iHours*60);
				cs.Format(_T("%.2d:%.2d h"),iHours,iMin);

				DrawText(m_hdcStatusLine,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
			}
			else
			{
				cs.Format(_T("%d%s"),m_iBattStatPercents,_T("%"));
				DrawText(m_hdcStatusLine,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
			}
		}
		else
		{
			if(m_pcACCU!=NULL)
				m_pcACCU->Draw(m_hdcStatusLine,108,5);
			else
			{
				if(m_pcACCU_0)
					m_pcACCU_0->Draw(m_hdcStatusLine,108,5);
			}

			rc.left = 105;  
			rc.top = 22;  
			rc.right  = 145;  
			rc.bottom = 41;
			cs.Format(_T("%d%s"),m_iBattStatPercents,_T("%"));
			DrawText(m_hdcStatusLine,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
			CSize sz;
			sz = pDCMem->GetTextExtent(cs);
		}


		BitBlt(m_hDC,rcCl.left,rcCl.top,rcCl.right,rcCl.bottom,m_hdcStatusLine,0,0,SRCCOPY);


		SetTextColor(m_hdcStatusLine,tc);
		SetBkMode(m_hdcStatusLine,nBkMode);

		SelectObject(m_hdcStatusLine,hPrevFont);
		brBack.DeleteObject();

	}
	catch (...)
	{
		LeaveCriticalSection(&csInfo);
		theApp.ReportException(_T("CViewDTBField::DrawStateLine"));
	}
	LeaveCriticalSection(&csInfo);
}

/**********************************************************************************************//**
 * Determines if we can check date changed
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewDTBField::CheckDateChanged()
{
	bool bChanged=false;

	SYSTEMTIME st;
	GetLocalTime(&st);
	COleDateTime dtTime(st);


	if(m_iPrevDay!=dtTime.GetDay())
	{
		m_iPrevDay=dtTime.GetDay();
		bChanged=true;
	}

	return bChanged;
}

/**********************************************************************************************//**
 * Determines if we can check akku state
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewDTBField::CheckAkkuState()
{
	bool bChanged=false;

	int iBattStatMinutes=getModel()->getDATAHANDLER()->GetBatteryStatMinutes();
	if(iBattStatMinutes!=m_iBattStatMinutes)
		bChanged=true;
	m_iBattStatMinutes=iBattStatMinutes;


	int iBattStatPercents=getModel()->getDATAHANDLER()->GetBatteryStatPercent();
	if(iBattStatPercents!=m_iBattStatPercents)
		bChanged=true;
	m_iBattStatPercents=iBattStatPercents;

	if(m_iBattStatPercents>100)
		m_iBattStatPercents=100;

	if(getModel()->getI2C()->IsI2C_ACCUavailability())
	{
		WORD wSTATE=getModel()->getDATAHANDLER()->GetAccuState();
		if(wSTATE & BIT0)
		{
			if(m_bAccuPower==false)
			{
				bChanged=true;
#ifndef SIMULATION_ENTREK //rkuNEWFIX
				SetTimer(ACCUSTATETIMER,ACCUSTATETIME,NULL);
#endif
			}
			m_bAccuPower=true;
		}
		else
		{
			if(m_bAccuPower==true)
			{
				bChanged=true;
				m_bDrawRemainingTime=false;
				KillTimer(ACCUSTATETIMER);
			}
			m_bAccuPower=false;
		}
		
		if(wSTATE & BIT1)
		{
			if(m_bNetPower==false)
				bChanged=true;
			m_bNetPower=true;
		}
		else
		{
			if(m_bNetPower==true)
				bChanged=true;
			m_bNetPower=false;
		}
		if(wSTATE & BIT2)
		{
			if(m_bExtrenalPower==false)
				bChanged=true;
			m_bExtrenalPower=true;
		}
		else
		{
			if(m_bExtrenalPower==true)
				bChanged=true;
			m_bExtrenalPower=false;
		}
		if(wSTATE & BIT3)
		{
			if(m_bACCUcharging==false)
			{
#ifndef SIMULATION_ENTREK //rkuNEWFIX
				SetTimer(ACCUCHARGETIMER,ACCUCHARGE,NULL);
#endif
				m_iCntAccuCharge=0;
				bChanged=true;
			}
			
			m_bACCUcharging=true;

		}
		else
		{
			if(m_bACCUcharging==true)
			{
				bChanged=true;
				KillTimer(ACCUCHARGETIMER);
			}
			m_bACCUcharging=false;
		}
		if(wSTATE & BIT4)
		{
			if(m_bACCUfull==false)
				bChanged=true;
			m_bACCUfull=true;
		}
		else
		{
			if(m_bACCUfull==true)
				bChanged=true;
			m_bACCUfull=false;
		}
	}
	else
	{
		if(getModel()->getDATAHANDLER()->IsAccuSupply())
		{
			if(m_bAccuPower==false)
				bChanged=true;
			m_bNetPower=false;
			m_bAccuPower=true;
			m_bACCUcharging=false;
		}
		else
		{
			if(m_bNetPower==false)
				bChanged=true;
			m_bNetPower=true;
			m_bAccuPower=false;

			if(m_iBattStatPercents>=95)
			{
				m_bACCUfull=true;
				if(m_bACCUcharging==true)
				{
					bChanged=true;
					KillTimer(ACCUCHARGETIMER);
				}
				m_bACCUcharging=false;
			}
			else
			{
				if(m_bACCUcharging==false)
				{
#ifndef SIMULATION_ENTREK //rkuNEWFIX
					SetTimer(ACCUCHARGETIMER,ACCUCHARGE,NULL);
#endif
					m_iCntAccuCharge=0;
					bChanged=true;
				}
				m_bACCUfull=false;
				m_bACCUcharging=true;
			}
		}
	}


	return bChanged;
}

/**********************************************************************************************//**
 * Notifies the view state changed
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewDTBField::NotifyViewStateChanged()
{
	
	if(getModel()->getVIEWHANDLER()->getViewState()==VS_ALARM_LIMIT)
	{
		m_Clock.Stop();
		m_Clock.ShowWindow(SW_HIDE);
		DrawLine(true,true,true);
	}
	else
	{
		if(m_bPRICOrunning)
		{
			if(m_Clock.IsClockRunning())
			{
				m_Clock.Stop();
				m_Clock.ShowWindow(SW_HIDE);
				DrawLine(true,true,true);
			}
		}
		else //show clock and date
		{
			if(m_Clock.IsClockRunning()==false)
			{
				m_Clock.ShowWindow(SW_SHOW);
				m_Clock.Run();
				DrawLine(true,true,true);
			}
		}
		//DrawLine(true,true,true);
	}

}
