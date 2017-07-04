#include "StdAfx.h"
#include "ViewAlarmLimit.h"
#include "LangAdmin.h"
#include "globDefs.h"

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x0000BF00
#define COLOR_TXTSUBBTNDW			0x00000000
#define COLOR_TXTBTNFC				0x00FF5F55

#define TIMEMODECHANGE	45000

//global font objects
//extern HFONT g_hf14AcuBold;
//extern HFONT g_hf10AcuBold;
//extern HFONT g_hf43AcuBold;
//extern HFONT g_hf33AcuBold;
//extern HFONT g_hf8AcuNorm;

//extern CLangAdmin* g_pGlobalLanguageStrings;

// **************************************************************************
// 
// **************************************************************************
CViewAlarmLimit::CViewAlarmLimit(int ViewID):
CMVView(ViewID)
{
	m_hdcStatic=NULL;
	m_hbmpStatic=NULL;

	m_bAlarmSet=false;

	m_eCurVentMode=VM_NONE;

	m_eCurALimitNumeric=ALIMITNUM_VENTILATION;
	
	m_pbtnValueDelay=NULL;
	m_pcAlarmLimit_MVhi=NULL;
	m_pcAlarmLimit_MVlow=NULL;
	m_pcAlarmLimit_PIPmax=NULL;
	m_pcAlarmLimit_PIPmin=NULL;
	m_pcAlarmLimit_PEEP=NULL;
	m_pcAlarmLimit_BPM=NULL;
	m_pcAlarmLimit_Leak=NULL;
	m_pcAlarmLimit_Apnoe=NULL;
	m_pcAlarmLimit_DCO2hi=NULL;
	m_pcAlarmLimit_DCO2lo=NULL;
	m_pcAlarmLimit_ETCO2hi=NULL;
	m_pcAlarmLimit_ETCO2lo=NULL;

	m_pcAlarmLimit_FICO2hi=NULL;
	m_pcAlarmLimit_FICO2lo=NULL;


	m_pcAlarmLimit_SPO2hi=NULL;
	m_pcAlarmLimit_SPO2lo=NULL;
	m_pcAlarmLimit_PulseRatehi=NULL;
	m_pcAlarmLimit_PulseRatelo=NULL;
	m_pcAlarmLimit_SPO2_PIlo=NULL;
	m_pcAlarmLimit_SPO2_SIQ=NULL;
	m_pWndMenuAlarmLimits=NULL;

	m_pcStatic_Up=NULL;
	m_pcStatic_Dw=NULL;
	m_pcStatic_Fc=NULL;

	m_pcNumInfoTop= NULL;
	m_pcNumInfoBot= NULL;
	

	//m_pcNumHeader=NULL;
	m_pcAlarmlimitPara1=NULL;
	m_pcAlarmlimitPara2=NULL;
	m_pcBtnAlarm_Up=NULL;
	m_pcBtnAlarm_Dw=NULL;
	m_pcBtnAlarm_Fc=NULL;

	m_iFocusedWnd=0;
	m_bMenuPressed=false;
	m_iCurPara=0;

	m_iValueDelay=0;

	m_bExit=false;
}

// **************************************************************************
// 
// **************************************************************************
CViewAlarmLimit::~CViewAlarmLimit(void)
{
	/*if(m_pcNumHeader)
		delete m_pcNumHeader;
	m_pcNumHeader=NULL;*/

	

	delete m_pcStatic_Up;
	m_pcStatic_Up=NULL;
	delete m_pcStatic_Dw;
	m_pcStatic_Dw=NULL;
	delete m_pcStatic_Fc;
	m_pcStatic_Fc=NULL;
	delete m_pbtnValueDelay;
	m_pbtnValueDelay=NULL;

	if(m_pcAlarmlimitPara1)
		delete m_pcAlarmlimitPara1;
	m_pcAlarmlimitPara1=NULL;

	if(m_pcAlarmlimitPara2)
		delete m_pcAlarmlimitPara2;
	m_pcAlarmlimitPara2=NULL;

	if(m_pcAlarmLimit_MVhi)
		delete m_pcAlarmLimit_MVhi;
	m_pcAlarmLimit_MVhi=NULL;

	if(m_pcAlarmLimit_MVlow)
		delete m_pcAlarmLimit_MVlow;
	m_pcAlarmLimit_MVlow=NULL;

	if(m_pcAlarmLimit_PIPmax)
		delete m_pcAlarmLimit_PIPmax;
	m_pcAlarmLimit_PIPmax=NULL;

	if(m_pcAlarmLimit_PIPmin)
		delete m_pcAlarmLimit_PIPmin;
	m_pcAlarmLimit_PIPmin=NULL;

	if(m_pcAlarmLimit_PEEP)
		delete m_pcAlarmLimit_PEEP;
	m_pcAlarmLimit_PEEP=NULL;

	if(m_pcAlarmLimit_BPM)
		delete m_pcAlarmLimit_BPM;
	m_pcAlarmLimit_BPM=NULL;

	if(m_pcAlarmLimit_Leak)
		delete m_pcAlarmLimit_Leak;
	m_pcAlarmLimit_Leak=NULL;

	if(m_pcAlarmLimit_Apnoe)
		delete m_pcAlarmLimit_Apnoe;
	m_pcAlarmLimit_Apnoe=NULL;

	if(m_pcAlarmLimit_DCO2hi)
		delete m_pcAlarmLimit_DCO2hi;
	m_pcAlarmLimit_DCO2hi=NULL;

	if(m_pcAlarmLimit_DCO2lo)
		delete m_pcAlarmLimit_DCO2lo;
	m_pcAlarmLimit_DCO2lo=NULL;


	if(m_pcAlarmLimit_ETCO2hi)
		delete m_pcAlarmLimit_ETCO2hi;
	m_pcAlarmLimit_ETCO2hi=NULL;

	if(m_pcAlarmLimit_ETCO2lo)
		delete m_pcAlarmLimit_ETCO2lo;
	m_pcAlarmLimit_ETCO2lo=NULL;

	if(m_pcAlarmLimit_FICO2hi)
		delete m_pcAlarmLimit_FICO2hi;
	m_pcAlarmLimit_FICO2hi=NULL;

	if(m_pcAlarmLimit_FICO2lo)
		delete m_pcAlarmLimit_FICO2lo;
	m_pcAlarmLimit_FICO2lo=NULL;

	if(m_pcAlarmLimit_SPO2hi)
		delete m_pcAlarmLimit_SPO2hi;
	m_pcAlarmLimit_SPO2hi=NULL;

	if(m_pcAlarmLimit_SPO2lo)
		delete m_pcAlarmLimit_SPO2lo;
	m_pcAlarmLimit_SPO2lo=NULL;

	if(m_pcAlarmLimit_PulseRatehi)
		delete m_pcAlarmLimit_PulseRatehi;
	m_pcAlarmLimit_PulseRatehi=NULL;

	if(m_pcAlarmLimit_PulseRatelo)
		delete m_pcAlarmLimit_PulseRatelo;
	m_pcAlarmLimit_PulseRatelo=NULL;

	if(m_pcAlarmLimit_SPO2_PIlo)
		delete m_pcAlarmLimit_SPO2_PIlo;
	m_pcAlarmLimit_SPO2_PIlo=NULL;

	if(m_pcAlarmLimit_SPO2_SIQ)
		delete m_pcAlarmLimit_SPO2_SIQ;
	m_pcAlarmLimit_SPO2_SIQ=NULL;
	
	if(m_pcNumInfoTop)
		delete m_pcNumInfoTop;
	m_pcNumInfoTop=NULL;

	if(m_pcNumInfoBot)
		delete m_pcNumInfoBot;
	m_pcNumInfoBot=NULL;

	//delete m_pcAlarm;
	delete m_pcBtnAlarm_Up;
	m_pcBtnAlarm_Up=NULL;
	delete m_pcBtnAlarm_Dw;
	m_pcBtnAlarm_Dw=NULL;
	delete m_pcBtnAlarm_Fc;
	m_pcBtnAlarm_Fc=NULL;

	if(m_pWndMenuAlarmLimits)
	{
		delete m_pWndMenuAlarmLimits;
		m_pWndMenuAlarmLimits=NULL;
	}
}

BEGIN_MESSAGE_MAP(CViewAlarmLimit, CMVView)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_ALIMIT_DELAY, &CViewAlarmLimit::OnBnClickedValue)
	//ON_WM_SETFOCUS()
END_MESSAGE_MAP()



// **************************************************************************
// 
// **************************************************************************
bool CViewAlarmLimit::CreateView()
{
	RECT rcLd={610,0,800,550};

	if (!Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_ALARMLIMITS))
	{
		return false;
	}

	Initialize();

	//PostMessage(WM_SETSTARTFOCUS);

	return true;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CViewAlarmLimit::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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

		CBrush cbrBack(BACKGND);
		//CBrush cbrBack(RGB(255,0,0));
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		m_hdcStatic = CreateCompatibleDC(m_hDC);
		m_hbmpStatic = CreateCompatibleBitmap(m_hDC, m_lX, m_lY);
		SelectObject(m_hdcStatic, m_hbmpStatic);

		HPEN hpenprevStat=(HPEN)SelectObject(m_hdcStatic, (HPEN)GetStockObject(NULL_PEN));	
		HBRUSH hbrprevStat=(HBRUSH)SelectObject(m_hdcStatic,cbrBack); 
		Rectangle(m_hdcStatic, 0, 0, m_lX, m_lY);

		m_pcAlarmlimitPara1	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUMALIMIT_1);
		m_pcAlarmlimitPara2	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUMALIMIT_2);

		m_pcStatic_Up= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_DELAYVALUE_UP);
		m_pcStatic_Dw= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_DELAYVALUE_DW);
		m_pcStatic_Fc= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_DELAYVALUE_FC);

		SelectObject(m_hdcStatic, hpenprevStat);	
		SelectObject(m_hdcStatic,hbrprevStat);
		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();

		return 1;
	}
	else
		return 0;
}
// **************************************************************************
// 
// **************************************************************************
bool CViewAlarmLimit::Initialize()
{
	bool result = false;

	m_eCurVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();

	m_eCurALimitNumeric=getModel()->getCONFIG()->getCurALimitNumeric();

	checkAlarmState();

	if(m_eCurALimitNumeric==ALIMITNUM_ETCO2 && getModel()->getCONFIG()->GetCO2module()==CO2MODULE_NONE)
	{
		m_eCurALimitNumeric=ALIMITNUM_VENTILATION;
		getModel()->getCONFIG()->setCurALimitNumeric(ALIMITNUM_VENTILATION);
	}
	else if(m_eCurALimitNumeric==ALIMITNUM_SPO2 && getModel()->getCONFIG()->GetSPO2module()==SPO2MODULE_NONE)
	{
		m_eCurALimitNumeric=ALIMITNUM_VENTILATION;
		getModel()->getCONFIG()->setCurALimitNumeric(ALIMITNUM_VENTILATION);
	}

	m_pcBtnAlarm_Up		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_ALIMIT_UP);
	m_pcBtnAlarm_Dw		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_ALIMIT_DW);
	m_pcBtnAlarm_Fc		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_ALIMIT_FC);

	m_pcNumInfoTop		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUM_INFO_TOP);
	m_pcNumInfoBot		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_NUM_INFO_BOT);

	m_iValueDelay=getModel()->getCONFIG()->GetCurPminAlarmDelay();
	
	DrawData(true, true);

	HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf11AcuBold);
	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(m_hDC,RGB(0,0,0));
	DWORD dwStyleNoTab = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW|WS_TABSTOP;

	BTN btn;
	fALVALUE fv;

	//create all buttons

	//-------------------------------------------------------------
	//Button------MV---------------------------------
	btn.wID					= IDC_BTN_ALARM_MV_HI;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 46;
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangeMVmax();
	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangeMVmax();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitMVmax();

	if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
	{
		fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
		getModel()->getALARMHANDLER()->setAlimitMVmax(fv.iCurrentLimit);
	}
	else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
	{
		fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
		getModel()->getALARMHANDLER()->setAlimitMVmax(fv.iCurrentLimit);
	}

	m_pcAlarmLimit_MVhi=new CALimitBtn(btn);
	m_pcAlarmLimit_MVhi->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_MVhi->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_MVhi->SetLimitRange(HIGHLIMIT);
	m_pcAlarmLimit_MVhi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_MVmax->getAlarmState(), false);
	m_pcAlarmLimit_MVhi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_MVmaxLimit());
	m_pcAlarmLimit_MVhi->ShowWindow(SW_HIDE);


	btn.wID					= IDC_BTN_ALARM_MV_LO;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 101;
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangeMVmin();
	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangeMVmin();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitMVmin();

	if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
	{
		fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
		getModel()->getALARMHANDLER()->setAlimitMVmin(fv.iCurrentLimit);
	}
	else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
	{
		fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
		getModel()->getALARMHANDLER()->setAlimitMVmin(fv.iCurrentLimit);
	}

	m_pcAlarmLimit_MVlow=new CALimitBtn(btn);
	m_pcAlarmLimit_MVlow->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_MVlow->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_MVlow->SetLimitRange(LOWLIMIT);
	m_pcAlarmLimit_MVlow->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_MVmin->getAlarmState(), false);
	m_pcAlarmLimit_MVlow->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_MVminLimit());
	m_pcAlarmLimit_MVlow->ShowWindow(SW_HIDE);

	////-------------------------------------------------------------
	////Button------PIPmax---------------------------------
	btn.wID					= IDC_BTN_ALARM_PIPMAX_HI;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 156;
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangePIPmax();
	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmax();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitPIPmax();

	if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
	{
		fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
		getModel()->getALARMHANDLER()->setAlimitPIPmax(fv.iCurrentLimit);
	}
	else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
	{
		fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
		getModel()->getALARMHANDLER()->setAlimitPIPmax(fv.iCurrentLimit);
	}

	m_pcAlarmLimit_PIPmax=new CALimitBtn(btn);
	m_pcAlarmLimit_PIPmax->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_PIPmax->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_PIPmax->SetLimitRange(HIGHLIMIT);
	m_pcAlarmLimit_PIPmax->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_PIPmax->getAlarmState(), false);
	m_pcAlarmLimit_PIPmax->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit());
	m_pcAlarmLimit_PIPmax->ShowWindow(SW_HIDE);


	////-------------------------------------------------------------
	////Button------PIPmin---------------------------------
	btn.wID					= IDC_BTN_ALARM_PIPMIN_LO;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 211;
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	/*fv.iAbsoluteLowerLimit=50;
	fv.iAbsoluteUpperLimit=0;
	fv.iCurrentLimit=10;*/

	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangePIPmin();
	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmin();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitPIPmin();

	if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
	{
		fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
		getModel()->getALARMHANDLER()->setAlimitPIPmin(fv.iCurrentLimit);
	}
	else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
	{
		fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
		getModel()->getALARMHANDLER()->setAlimitPIPmin(fv.iCurrentLimit);
	}

	m_pcAlarmLimit_PIPmin=new CALimitBtn(btn);
	m_pcAlarmLimit_PIPmin->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_PIPmin->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_PIPmin->SetLimitRange(LOWLIMIT);
	m_pcAlarmLimit_PIPmin->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_PIPmin->getAlarmState(), false);
	m_pcAlarmLimit_PIPmin->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit());
	m_pcAlarmLimit_PIPmin->ShowWindow(SW_HIDE);
	
	////-------------------------------------------------------------
	////Button------PEEP---------------------------------

	btn.wID					= IDC_BTN_ALARM_PEEP_LO;	
	if(m_eCurVentMode==VM_NCPAP || m_eCurVentMode==VM_CPAP || m_eCurVentMode==VM_DUOPAP)
	{
		btn.poPosition.x		= 100;
		btn.poPosition.y		= 211;
	}
	else
	{
		btn.poPosition.x		= 100;
		btn.poPosition.y		= 266;
	}
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangePEEPmin();
	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangePEEPmin();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitPEEPmin();

	if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
	{
		fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
		getModel()->getALARMHANDLER()->setAlimitPEEPmin(fv.iCurrentLimit);
	}
	else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
	{
		fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
		getModel()->getALARMHANDLER()->setAlimitPEEPmin(fv.iCurrentLimit);
	}

	m_pcAlarmLimit_PEEP=new CALimitBtn(btn);
	m_pcAlarmLimit_PEEP->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_PEEP->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_PEEP->SetLimitRange(LOWLIMIT);
	m_pcAlarmLimit_PEEP->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_PEEPminLow->getAlarmState(), false);
	m_pcAlarmLimit_PEEP->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit());
	m_pcAlarmLimit_PEEP->ShowWindow(SW_HIDE);

	//IDC_BTN_ALARM_DCO2_HI
	btn.wID					= IDC_BTN_ALARM_DCO2_HI;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 211;
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangeDCO2max();
	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangeDCO2max();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitDCO2max();

	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
			getModel()->getALARMHANDLER()->setAlimitDCO2max(fv.iCurrentLimit);
		}
		else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
			getModel()->getALARMHANDLER()->setAlimitDCO2max(fv.iCurrentLimit);
		}
	}
	
	m_pcAlarmLimit_DCO2hi=new CALimitBtn(btn);
	m_pcAlarmLimit_DCO2hi->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_DCO2hi->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_DCO2hi->SetLimitRange(HIGHLIMIT);
	m_pcAlarmLimit_DCO2hi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_DCO2max->getAlarmState(), false);
	m_pcAlarmLimit_DCO2hi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_DCO2maxLimit());
	m_pcAlarmLimit_DCO2hi->ShowWindow(SW_HIDE);


	btn.wID					= IDC_BTN_ALARM_DCO2_LO;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 266;
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangeDCO2min();
	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangeDCO2min();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitDCO2min();

	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
			getModel()->getALARMHANDLER()->setAlimitDCO2min(fv.iCurrentLimit);
		}
		else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
			getModel()->getALARMHANDLER()->setAlimitDCO2min(fv.iCurrentLimit);
		}
	}
	

	m_pcAlarmLimit_DCO2lo=new CALimitBtn(btn);
	m_pcAlarmLimit_DCO2lo->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_DCO2lo->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_DCO2lo->SetLimitRange(LOWLIMIT);
	m_pcAlarmLimit_DCO2lo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_DCO2min->getAlarmState(), false);
	m_pcAlarmLimit_DCO2lo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_DCO2minLimit());
	m_pcAlarmLimit_DCO2lo->ShowWindow(SW_HIDE);


	////-------------------------------------------------------------
	////Button------BPM---------------------------------
	btn.wID					= IDC_BTN_ALARM_BPM_HI;	
	if(m_eCurVentMode==VM_DUOPAP || m_eCurVentMode==VM_NCPAP)
	{
		/*btn.poPosition.x		= 100;
		btn.poPosition.y		= 321;*/

		btn.poPosition.x		= 100;
		btn.poPosition.y		= 266;
	}
	else if(m_eCurVentMode==VM_CPAP)
	{
		btn.poPosition.x		= 100;
		btn.poPosition.y		= 266;
	}
	else
	{
		btn.poPosition.x		= 100;
		btn.poPosition.y		= 321;
	}
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangeBPMmax();
	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangeBPMmax();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitBPMmax();

	if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
	{
		fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
		getModel()->getALARMHANDLER()->setAlimitBPMmax(fv.iCurrentLimit);
	}
	else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
	{
		fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
		getModel()->getALARMHANDLER()->setAlimitBPMmax(fv.iCurrentLimit);
	}

	m_pcAlarmLimit_BPM=new CALimitBtn(btn);
	m_pcAlarmLimit_BPM->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_BPM->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_BPM->SetLimitRange(HIGHLIMIT);
	m_pcAlarmLimit_BPM->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_BPMmax->getAlarmState(), false);
	m_pcAlarmLimit_BPM->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_BPMmaxLimit());
	m_pcAlarmLimit_BPM->ShowWindow(SW_HIDE);

	////-------------------------------------------------------------
	////Button------Leak---------------------------------
	btn.wID					= IDC_BTN_ALARM_LEAK_HI;	
	if(m_eCurVentMode==VM_CPAP || m_eCurVentMode==VM_HFO)
	{
		btn.poPosition.x		= 100;
		btn.poPosition.y		= 321;
	}
	else
	{
		btn.poPosition.x		= 100;
		btn.poPosition.y		= 376;
	}
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangeLeakmax();
	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangeLeakmax();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitLeakmax();

	if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
	{
		fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
		getModel()->getALARMHANDLER()->setAlimitLeakmax(fv.iCurrentLimit);
	}
	else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
	{
		fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
		getModel()->getALARMHANDLER()->setAlimitLeakmax(fv.iCurrentLimit);
	}

	m_pcAlarmLimit_Leak=new CALimitBtn(btn);
	m_pcAlarmLimit_Leak->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_Leak->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_Leak->SetLimitRange(HIGHLIMIT);
	m_pcAlarmLimit_Leak->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_Leakmax->getAlarmState(), false);
	m_pcAlarmLimit_Leak->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_LeakmaxLimit());
	m_pcAlarmLimit_Leak->ShowWindow(SW_HIDE);


	
	////-------------------------------------------------------------
	////Button------Apnoe---------------------------------
	btn.wID					= IDC_BTN_ALARM_APNOE;	
	if(m_eCurVentMode==VM_DUOPAP || m_eCurVentMode==VM_NCPAP)
	{
		btn.poPosition.x		= 100;
		btn.poPosition.y		= 321;
	}
	else if(m_eCurVentMode==VM_CPAP)
	{
		btn.poPosition.x		= 100;
		btn.poPosition.y		= 376;
	}
	else
	{
		btn.poPosition.x		= 100;
		btn.poPosition.y		= 431;
	}
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangeApnoe();
	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangeApnoe();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitApnoe();
	eAlarmLimitState stateApnoe = getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit();

	if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit && stateApnoe!=AL_OFF)
	{
		fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
		getModel()->getALARMHANDLER()->setAlimitApnoe(fv.iCurrentLimit);
	}
	else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
	{
		fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
		getModel()->getALARMHANDLER()->setAlimitApnoe(fv.iCurrentLimit);
	}


	m_pcAlarmLimit_Apnoe=new CALimitBtn(btn);
	m_pcAlarmLimit_Apnoe->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_Apnoe->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_Apnoe->SetLimitRange(NOLIMIT);
	m_pcAlarmLimit_Apnoe->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_Apnoe->getAlarmState(), false);
	m_pcAlarmLimit_Apnoe->SetAlarmLimitState(stateApnoe);
	m_pcAlarmLimit_Apnoe->ShowWindow(SW_HIDE);

	////-------------------------------------------------------------
	////Button------ETCO2---------------------------------
	btn.wID					= IDC_BTN_ALARM_ETCO2_HI;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 46;//431;
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangeETCO2max();
	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangeETCO2max();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitETCO2max();

	//if(getModel()->getCONFIG()->GetCO2module()!=CO2MODULE_NONE)
	{
		if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
			getModel()->getALARMHANDLER()->setAlimitETCO2max(fv.iCurrentLimit);
		}
		else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
			getModel()->getALARMHANDLER()->setAlimitETCO2max(fv.iCurrentLimit);
		}
	}
	

	m_pcAlarmLimit_ETCO2hi=new CALimitBtn(btn);
	m_pcAlarmLimit_ETCO2hi->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_ETCO2hi->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_ETCO2hi->SetLimitRange(HIGHLIMIT);
	m_pcAlarmLimit_ETCO2hi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_ETCO2max->getAlarmState(), false);
	m_pcAlarmLimit_ETCO2hi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_ETCO2maxLimit());
	m_pcAlarmLimit_ETCO2hi->ShowWindow(SW_HIDE);


	btn.wID					= IDC_BTN_ALARM_ETCO2_LO;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 101;//486;
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangeETCO2min();
	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangeETCO2min();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitETCO2min();

	//if(getModel()->getCONFIG()->GetCO2module()!=CO2MODULE_NONE)
	{
		if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
			getModel()->getALARMHANDLER()->setAlimitETCO2min(fv.iCurrentLimit);
		}
		else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
			getModel()->getALARMHANDLER()->setAlimitETCO2min(fv.iCurrentLimit);
		}
	}
	

	m_pcAlarmLimit_ETCO2lo=new CALimitBtn(btn);
	m_pcAlarmLimit_ETCO2lo->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_ETCO2lo->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_ETCO2lo->SetLimitRange(LOWLIMIT);
	m_pcAlarmLimit_ETCO2lo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_ETCO2min->getAlarmState(), false);
	m_pcAlarmLimit_ETCO2lo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_ETCO2minLimit());
	m_pcAlarmLimit_ETCO2lo->ShowWindow(SW_HIDE);

	////-------------------------------------------------------------
	////Button------FICO2---------------------------------
	btn.wID					= IDC_BTN_ALARM_FICO2_HI;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 156;
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangeFICO2max();
	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangeFICO2max();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitFICO2max();

	//if(getModel()->getCONFIG()->GetCO2module()!=CO2MODULE_NONE)
	{
		if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
			getModel()->getALARMHANDLER()->setAlimitFICO2max(fv.iCurrentLimit);
		}
		else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
			getModel()->getALARMHANDLER()->setAlimitFICO2max(fv.iCurrentLimit);
		}
	}


	m_pcAlarmLimit_FICO2hi=new CALimitBtn(btn);
	m_pcAlarmLimit_FICO2hi->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_FICO2hi->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_FICO2hi->SetLimitRange(HIGHLIMIT);
	m_pcAlarmLimit_FICO2hi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_FICO2max->getAlarmState(), false);
	m_pcAlarmLimit_FICO2hi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_FICO2maxLimit());
	m_pcAlarmLimit_FICO2hi->ShowWindow(SW_HIDE);


	btn.wID					= IDC_BTN_ALARM_FICO2_LO;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 211;
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangeFICO2min();
	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangeFICO2min();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitFICO2min();

	//if(getModel()->getCONFIG()->GetCO2module()!=CO2MODULE_NONE)
	{
		if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
			getModel()->getALARMHANDLER()->setAlimitFICO2min(fv.iCurrentLimit);
		}
		else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
			getModel()->getALARMHANDLER()->setAlimitFICO2min(fv.iCurrentLimit);
		}
	}


	m_pcAlarmLimit_FICO2lo=new CALimitBtn(btn);
	m_pcAlarmLimit_FICO2lo->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_FICO2lo->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_FICO2lo->SetLimitRange(LOWLIMIT);
	m_pcAlarmLimit_FICO2lo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_FICO2min->getAlarmState(), false);
	m_pcAlarmLimit_FICO2lo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_FICO2minLimit());
	m_pcAlarmLimit_FICO2lo->ShowWindow(SW_HIDE);


	////-------------------------------------------------------------
	////Button------SPO2---------------------------------
	btn.wID					= IDC_BTN_ALARM_SPO2_HI;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 46;//431;
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangeSPO2max();
	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangeSPO2max();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitSPO2max();


	//if(getModel()->getCONFIG()->GetSPO2module()!=SPO2MODULE_NONE)
	{
		if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
			getModel()->getALARMHANDLER()->setAlimitSPO2max(fv.iCurrentLimit);
		}
		else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
			getModel()->getALARMHANDLER()->setAlimitSPO2max(fv.iCurrentLimit);
		}
	}


	m_pcAlarmLimit_SPO2hi=new CALimitBtn(btn);
	m_pcAlarmLimit_SPO2hi->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_SPO2hi->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_SPO2hi->SetLimitRange(HIGHLIMIT);
	m_pcAlarmLimit_SPO2hi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2max->getAlarmState(), false);
	m_pcAlarmLimit_SPO2hi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_SPO2maxLimit());
	m_pcAlarmLimit_SPO2hi->ShowWindow(SW_HIDE);

	/***************************SpO2min********************/
	btn.wID					= IDC_BTN_ALARM_SPO2_LO;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 101;//431;
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangeSPO2min();
	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangeSPO2min();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitSPO2min();

	//if(getModel()->getCONFIG()->GetSPO2module()!=SPO2MODULE_NONE)
	{
		if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
			getModel()->getALARMHANDLER()->setAlimitSPO2min(fv.iCurrentLimit);
		}
		else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
			getModel()->getALARMHANDLER()->setAlimitSPO2min(fv.iCurrentLimit);
		}
	}


	m_pcAlarmLimit_SPO2lo=new CALimitBtn(btn);
	m_pcAlarmLimit_SPO2lo->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_SPO2lo->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_SPO2lo->SetLimitRange(LOWLIMIT);
	m_pcAlarmLimit_SPO2lo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2min->getAlarmState(), false);
	m_pcAlarmLimit_SPO2lo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_SPO2minLimit());
	m_pcAlarmLimit_SPO2lo->ShowWindow(SW_HIDE);


	/***************************PulseRatemax********************/
	btn.wID					= IDC_BTN_ALARM_PULSERATE_HI;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 156;//431;
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangePulseRatemax();
	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangePulseRatemax();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitPulseRatemax();

	//if(getModel()->getCONFIG()->GetSPO2module()!=SPO2MODULE_NONE)
	{
		if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
			getModel()->getALARMHANDLER()->setAlimitPulseRatemax(fv.iCurrentLimit);
		}
		else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
			getModel()->getALARMHANDLER()->setAlimitPulseRatemax(fv.iCurrentLimit);
		}
	}


	m_pcAlarmLimit_PulseRatehi=new CALimitBtn(btn);
	m_pcAlarmLimit_PulseRatehi->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_PulseRatehi->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_PulseRatehi->SetLimitRange(HIGHLIMIT);
	m_pcAlarmLimit_PulseRatehi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_PulseRatemax->getAlarmState(), false);
	m_pcAlarmLimit_PulseRatehi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_PulseRatemaxLimit());
	m_pcAlarmLimit_PulseRatehi->ShowWindow(SW_HIDE);

	/***************************PulseRate min********************/
	btn.wID					= IDC_BTN_ALARM_PULSERATE_LO;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 211;//431;
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangePulseRatemin();
	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangePulseRatemin();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitPulseRatemin();

	//if(getModel()->getCONFIG()->GetSPO2module()!=SPO2MODULE_NONE)
	{
		if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
			getModel()->getALARMHANDLER()->setAlimitPulseRatemin(fv.iCurrentLimit);
		}
		else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
			getModel()->getALARMHANDLER()->setAlimitPulseRatemin(fv.iCurrentLimit);
		}
	}


	m_pcAlarmLimit_PulseRatelo=new CALimitBtn(btn);
	m_pcAlarmLimit_PulseRatelo->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_PulseRatelo->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_PulseRatelo->SetLimitRange(LOWLIMIT);
	m_pcAlarmLimit_PulseRatelo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_PulseRatemin->getAlarmState(), false);
	m_pcAlarmLimit_PulseRatelo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_PulseRateminLimit());
	m_pcAlarmLimit_PulseRatelo->ShowWindow(SW_HIDE);


	/***************************SpO2 PI min********************/
	btn.wID					= IDC_BTN_ALARM_SPO2_PI_LO;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 266;//431;
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangeSPO2_PImin();
	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangeSPO2_PImin();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitSPO2_PImin();

	//if(getModel()->getCONFIG()->GetSPO2module()!=SPO2MODULE_NONE)
	{
		if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
			getModel()->getALARMHANDLER()->setAlimitSPO2_PImin(fv.iCurrentLimit);
		}
		else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
		{
			fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
			getModel()->getALARMHANDLER()->setAlimitSPO2_PImin(fv.iCurrentLimit);
		}
	}

	m_pcAlarmLimit_SPO2_PIlo=new CALimitBtn(btn);
	m_pcAlarmLimit_SPO2_PIlo->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_SPO2_PIlo->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_SPO2_PIlo->SetLimitRange(LOWLIMIT);
	m_pcAlarmLimit_SPO2_PIlo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2_PImin->getAlarmState(), false);
	m_pcAlarmLimit_SPO2_PIlo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_SPO2_PIminLimit());
	m_pcAlarmLimit_SPO2_PIlo->ShowWindow(SW_HIDE);


	//m_pcAlarmLimit_SPO2_SIQ
	/***************************SpO2 SIQ********************/
	btn.wID					= IDC_BTN_ALARM_SPO2_SIQ;	
	btn.poPosition.x		= 100;
	btn.poPosition.y		= 321;
	btn.pcBmpUp				= m_pcBtnAlarm_Up;
	btn.pcBmpDown			= m_pcBtnAlarm_Dw;
	btn.pcBmpFocus			= m_pcBtnAlarm_Fc;
	btn.pcBmpDisabled		= m_pcBtnAlarm_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	fv.iAbsoluteUpperLimit=getModel()->getALARMHANDLER()->getAlimitMaxRangeSPO2_SIQmin();
	fv.iAbsoluteLowerLimit=getModel()->getALARMHANDLER()->getAlimitMinRangeSPO2_SIQmin();
	fv.iCurrentLimit=getModel()->getALARMHANDLER()->getAlimitSPO2_SIQmin();
	//fv.iCurrentLimit=getModel()->getCONFIG()->getSpO2SIQlimit();

	if(fv.iCurrentLimit<fv.iAbsoluteLowerLimit)
	{
		fv.iCurrentLimit=fv.iAbsoluteLowerLimit;
		getModel()->getALARMHANDLER()->setAlimitSPO2_SIQmin(fv.iCurrentLimit);
		//getModel()->getALARMHANDLER()->setAlimitState_SPO2_SIQminLimit()
	}
	else if(fv.iCurrentLimit>fv.iAbsoluteUpperLimit)
	{
		fv.iCurrentLimit=fv.iAbsoluteUpperLimit;
		getModel()->getALARMHANDLER()->setAlimitSPO2_SIQmin(fv.iCurrentLimit);
	}

	m_pcAlarmLimit_SPO2_SIQ=new CALimitBtn(btn);
	m_pcAlarmLimit_SPO2_SIQ->Create(this,dwStyleNoTab,fv);
	m_pcAlarmLimit_SPO2_SIQ->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP,COLOR_TXTBTNUP);
	m_pcAlarmLimit_SPO2_SIQ->SetLimitRange(LOWLIMIT);
	m_pcAlarmLimit_SPO2_SIQ->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2_SIQmin->getAlarmState(), false);
	m_pcAlarmLimit_SPO2_SIQ->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_SPO2_SIQminLimit());
	m_pcAlarmLimit_SPO2_SIQ->ShowWindow(SW_HIDE);


	if(m_eCurVentMode==VM_NCPAP || m_eCurVentMode==VM_DUOPAP || m_eCurVentMode==VM_CPAP)
	{
		CStringW strVal;
		strVal.Format(_T("%d"),m_iValueDelay);
		
		//*******************alarm delay Pmin*****************************
		btn.wID					= IDC_BTN_ALIMIT_DELAY;	
		btn.poPosition.x		= 40;
		btn.poPosition.y		= 457;
		//btn.poPosition.x		= 40;
		//btn.poPosition.y		= 347;
		btn.pcBmpUp				= m_pcStatic_Up;
		btn.pcBmpDown			= m_pcStatic_Dw;
		btn.pcBmpFocus			= m_pcStatic_Fc;
		btn.pcBmpDisabled		= m_pcStatic_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnValueDelay=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
		m_pbtnValueDelay->Create(this,g_hf33AcuBold,0,dwStyleNoTab);
		m_pbtnValueDelay->SetText(strVal);
		m_pbtnValueDelay->ShowWindow(SW_HIDE);
	}
	//else if(m_eCurVentMode==VM_CPAP)
	//{
	//	
	//	CStringW strVal;
	//	strVal.Format(_T("%d"),m_iValueDelay);
	//	
	//	//*******************alarm delay Pmin*****************************
	//	btn.wID					= IDC_BTN_ALIMIT_DELAY;	
	//	btn.poPosition.x		= 40;
	//	btn.poPosition.y		= 457;
	//	btn.pcBmpUp				= m_pcStatic_Up;
	//	btn.pcBmpDown			= m_pcStatic_Dw;
	//	btn.pcBmpFocus			= m_pcStatic_Fc;
	//	btn.pcBmpDisabled		= m_pcStatic_Up;
	//	btn.dwFormat			= DT_VCENTER|DT_CENTER;

	//	m_pbtnValueDelay=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
	//	m_pbtnValueDelay->Create(this,g_hf33AcuBold,0,dwStyleNoTab);
	//	m_pbtnValueDelay->SetText(strVal);
	//	m_pbtnValueDelay->ShowWindow(SW_HIDE);
	//}



	//make Buttons visible
	showALimitButtons();
	
	result = CreateWndMenuAlarmLimits();

	SelectObject(m_hDC,hPrevFont);
	SetBkMode(m_hDC,nBkMode);
	SetTextColor(m_hDC,nPrevTxtColor);

	return result;
}






// **************************************************************************
// 
// **************************************************************************
void CViewAlarmLimit::DrawData(bool bFrames, bool bLabel/*, bool bDrawPEEPauto, bool bDrawPMAXauto*/)
{
	if(m_bExit)
		return;

	RECT rc={0,0,m_lX,m_lY};

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	CDC* pDCStatic=CDC::FromHandle(m_hdcStatic);
	
	if(bFrames)
		DrawFrames(pDCStatic);

	if(bLabel)
		DrawLabel(pDCStatic);

	BitBlt(hdcMem, 0, 0, m_lX, m_lY,m_hdcStatic , 0, 0, SRCCOPY);

	if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
	{
		DrawData_etCO2(hdcMem);
	}
	else if (m_eCurALimitNumeric==ALIMITNUM_SPO2)
	{
		DrawData_SpO2(hdcMem);
		
	}
	else //if (m_eCurALimitNumeric==ALIMITNUM_VENTILATION)
	{
		DrawData_Vent(hdcMem);
		
	}

	BitBlt(m_hDC, 0, 0, m_lX, m_lY,hdcMem , 0, 0, SRCCOPY);
	BitBlt(dc.m_hDC, 0, 0, m_lX, m_lY, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem,hBmpMemPrev);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}

// **************************************************************************
// 
// **************************************************************************
void CViewAlarmLimit::DrawFrames(CDC* pDC)
{
	if(m_bExit)
		return;

	HDC hdc = *pDC;
	CBrush cbrBack(BACKGND);
	HBRUSH hPrevBrush=(HBRUSH)SelectObject(hdc,cbrBack);
	HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));

	RECT rcCl;
	GetClientRect(&rcCl);

	FillRect(hdc,&rcCl,cbrBack);

	if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
	{
		if(m_eCurVentMode!=VM_HFO && getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
		{
			if(m_pcAlarmlimitPara1)
				m_pcAlarmlimitPara1->Draw(hdc,0,266);	//BPM
		}

		if(m_pcAlarmlimitPara2)
			m_pcAlarmlimitPara2->Draw(hdc,0,46);	//ETCO2

		if(m_pcAlarmlimitPara2)
			m_pcAlarmlimitPara2->Draw(hdc,0,156);	//FICO2
	}
	else if (m_eCurALimitNumeric==ALIMITNUM_SPO2)
	{
		if(m_pcAlarmlimitPara2)
			m_pcAlarmlimitPara2->Draw(hdc,0,46);	//SpO2

		if(m_pcAlarmlimitPara2)
			m_pcAlarmlimitPara2->Draw(hdc,0,156);	//PulseRate

		if(m_pcAlarmlimitPara1)
			m_pcAlarmlimitPara1->Draw(hdc,0,266);	//PI

		if(m_pcAlarmlimitPara1)
			m_pcAlarmlimitPara1->Draw(hdc,0,321);	//SIQ
	}
	else //if (m_eCurALimitNumeric==ALIMITNUM_VENTILATION)
	{
		if(m_eCurVentMode==VM_THERAPIE)
		{
			HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf11AcuBold);
			int nBkMode=SetBkMode(hdc,TRANSPARENT);
			int nPrevTxtColor=SetTextColor(hdc,RGB(255,0,0));

			if(m_pcNumInfoTop)
				m_pcNumInfoTop->Draw(hdc,0,57);
			if(m_pcNumInfoBot)
				m_pcNumInfoBot->Draw(hdc,0,90);

			RECT rcTh;

			rcTh.left = 0;  
			rcTh.top = 74;  
			rcTh.right  = 200;  
			rcTh.bottom = 90;

			CBrush br3(RGB(176,176,177));
			FillRect(hdc,&rcTh,br3);

			rcTh.left = 0;  
			rcTh.top = 65;  
			rcTh.right  = 190;  
			rcTh.bottom = 102;


			pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_THERAP_NOALARM1),&rcTh,DT_TOP|DT_SINGLELINE|DT_CENTER);
			pDC->DrawText(getModel()->GetLanguageString(IDS_TXT_THERAP_NOALARM2),&rcTh,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
			SetTextColor(hdc,nPrevTxtColor);
			SetBkMode(hdc,nBkMode);
			SelectObject(hdc,hPrevFont);
		}
		else if(m_eCurVentMode==VM_HFO)
		{
			if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{	
				if(m_pcAlarmlimitPara2)
					m_pcAlarmlimitPara2->Draw(hdc,0,46);	//MV
			}

			if(m_pcAlarmlimitPara1)
				m_pcAlarmlimitPara1->Draw(hdc,0,156);	//Pmax

			if(m_pcAlarmlimitPara2)
				m_pcAlarmlimitPara2->Draw(hdc,0,211);	//DCO2

			if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{
				if(m_pcAlarmlimitPara1)
					m_pcAlarmlimitPara1->Draw(hdc,0,321);	//Leak HFO
			}
		}
		else if(m_eCurVentMode==VM_CPAP)
		{
			if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{	
				if(m_pcAlarmlimitPara2)
					m_pcAlarmlimitPara2->Draw(hdc,0,46);	//MV
			}

			if(m_pcAlarmlimitPara2)
				m_pcAlarmlimitPara2->Draw(hdc,0,156); //CPAP

			if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{
				if(m_pcAlarmlimitPara1)
					m_pcAlarmlimitPara1->Draw(hdc,0,266);	//BPM

				if(m_pcAlarmlimitPara1)
					m_pcAlarmlimitPara1->Draw(hdc,0,321);	//Leak
				
			}
			else if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_PRESSURE)
			{
				if(m_pcAlarmlimitPara1)
					m_pcAlarmlimitPara1->Draw(hdc,0,266);	//BPM
			}
			if(		getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET
				||	(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false
				&& getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET))
			{
				if(m_pcAlarmlimitPara1)
					m_pcAlarmlimitPara1->Draw(hdc,0,376);	//Apnoe
			}
			else if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_PRESSURE)
			{
				if(m_pcAlarmlimitPara1)
					m_pcAlarmlimitPara1->Draw(hdc,0,376);	//Apnoe
			}
		}
		else if(m_eCurVentMode==VM_NCPAP)
		{
			if(m_pcAlarmlimitPara2)
				m_pcAlarmlimitPara2->Draw(hdc,0,156); //CPAP

			if(		getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false 
				&&	getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET 
				&&	getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
			{
				if(m_pcAlarmlimitPara1)
					m_pcAlarmlimitPara1->Draw(hdc,0,266);	//BPM

				if(m_pcAlarmlimitPara1)
					m_pcAlarmlimitPara1->Draw(hdc,0,321);	//Apnea
			}
			else if(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
			{
				if(m_pcAlarmlimitPara1)
					m_pcAlarmlimitPara1->Draw(hdc,0,266);	//BPM

				if(m_pcAlarmlimitPara1)
					m_pcAlarmlimitPara1->Draw(hdc,0,321);	//Apnea
			}
		}
		else if(m_eCurVentMode==VM_DUOPAP)
		{
			if(m_pcAlarmlimitPara1)
				m_pcAlarmlimitPara1->Draw(hdc,0,156);	//Pmax

			if(m_pcAlarmlimitPara1)
				m_pcAlarmlimitPara1->Draw(hdc,0,211);	//PEEP

			if(		getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false 
				&&	getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET 
				&&	getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
			{
				if(m_pcAlarmlimitPara1)
					m_pcAlarmlimitPara1->Draw(hdc,0,266);	//BPM

				if(m_pcAlarmlimitPara1)
					m_pcAlarmlimitPara1->Draw(hdc,0,321);	//Apnea
			}
			else if(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
			{
				if(m_pcAlarmlimitPara1)
					m_pcAlarmlimitPara1->Draw(hdc,0,266);	//BPM

				if(m_pcAlarmlimitPara1)
					m_pcAlarmlimitPara1->Draw(hdc,0,321);	//Apnea
			}
		}
		else //if(m_eCurVentMode!=VM_THERAPIE)
		{
			if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{	
				if(m_pcAlarmlimitPara2)
					m_pcAlarmlimitPara2->Draw(hdc,0,46);	//MV
			}

			if(m_pcAlarmlimitPara2)
				m_pcAlarmlimitPara2->Draw(hdc,0,156);	//Pmax

			if(m_eCurVentMode!=VM_HFO)
			{
				if(m_pcAlarmlimitPara1)
					m_pcAlarmlimitPara1->Draw(hdc,0,266);	//PEEP
			}

			if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{
				if(m_pcAlarmlimitPara1)
					m_pcAlarmlimitPara1->Draw(hdc,0,321);	//BPM

				if(m_pcAlarmlimitPara1)
					m_pcAlarmlimitPara1->Draw(hdc,0,376);	//Leak

				if(m_eCurVentMode!=VM_IPPV && m_eCurVentMode!=VM_SIPPV)
				{
					if(m_pcAlarmlimitPara1)
						m_pcAlarmlimitPara1->Draw(hdc,0,431);	//Apnoe
				}
			}
			else if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_PRESSURE)
			{
				if(m_eCurVentMode!=VM_IPPV && m_eCurVentMode!=VM_SIPPV)
				{
					if(m_pcAlarmlimitPara1)
						m_pcAlarmlimitPara1->Draw(hdc,0,431);	//Apnoe
				}

				if(m_eCurVentMode==VM_SIPPV || m_eCurVentMode==VM_SIMV)
				{
					if(m_pcAlarmlimitPara1)
						m_pcAlarmlimitPara1->Draw(hdc,0,321);	//BPM
				}
			}
		}
	}

	SelectObject(hdc, hPrevBrush);	
	SelectObject(hdc, hPrevPen);
}

// **************************************************************************
// 
// **************************************************************************
void CViewAlarmLimit::DrawLabel(CDC* pDC)
{
	if(m_bExit)
		return;

	if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
	{
		drawLabel_ETCO2(pDC);
	}
	else if (m_eCurALimitNumeric==ALIMITNUM_SPO2)
	{
		drawLabel_SPO2(pDC);
	}
	else //if (m_eCurALimitNumeric==ALIMITNUM_VENTILATION)
	{
		drawLabel_VENTILATION(pDC);
	}
}

void CViewAlarmLimit::drawLabel_ETCO2(CDC* pDC)
{
	if(m_bExit)
		return;

	HDC hdc = *pDC;

	RECT rc;

	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf10AcuBold);
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));

	CStringW nameText=_T("");

	rc.left = 10;
	rc.right = 210;


	if(m_eCurVentMode!=VM_HFO && getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
	{
		SelectObject(hdc,g_hf10AcuBold);

		//--------------------BPM-------------------------------------
		rc.top = 266;//294;
		rc.bottom = 354;
		nameText=getModel()->GetLanguageString(IDS_PARA_FREQ);
		DrawText(hdc,nameText,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		CSize sz = pDC->GetTextExtent(nameText);

		SelectObject(hdc,g_hf6AcuNorm);

		rc.top = 267;//295;
		rc.bottom = 354;
		rc.left = 15+sz.cx;
		pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

		rc.left = 10;
		rc.right = 210;
	}

	//--------------------ETCO2-------------------------------------
	SelectObject(hdc,g_hf10AcuBold);
	rc.top = 46;
	rc.bottom = 79;

	nameText=_T("etCO");
	CSize sz = pDC->GetTextExtent(nameText);
	DrawText(hdc,nameText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

	SelectObject(hdc,g_hf7AcuBold);

	rc.top = 46;
	rc.bottom = 81;
	rc.left = 10+sz.cx;
	nameText=_T("2");
	DrawText(hdc,nameText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

	SelectObject(hdc,g_hf6AcuNorm);

	rc.top = 42;
	rc.bottom = 97;
	rc.left = 10;
	CString szUnit=_T("");
	eCO2unit unit=getModel()->getCONFIG()->GetCO2unit();
	switch(unit)
	{
	case CO2_UNIT_KPA:
		{
			szUnit +=_T("[")+getModel()->GetLanguageString(IDS_UNIT_KPA)+_T("]");
		}
		break;
	case CO2_UNIT_PERCENT:
		{
			szUnit +=_T("[")+getModel()->GetLanguageString(IDS_UNIT_VOLPERCENT)+_T("]");
		}
		break;
	case CO2_UNIT_MMHG:
	default:
		{
			szUnit +=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MMHG)+_T("]");
		}
		break;
	}
	pDC->DrawText(szUnit,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	//--------------------FICO2-------------------------------------
	SelectObject(hdc,g_hf10AcuBold);
	rc.top = 156;
	rc.bottom = 189;

	nameText=_T("FiCO");
	sz = pDC->GetTextExtent(nameText);
	DrawText(hdc,nameText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

	SelectObject(hdc,g_hf7AcuBold);

	rc.top = 156;
	rc.bottom = 191;
	rc.left = 10+sz.cx;
	nameText=_T("2");
	DrawText(hdc,nameText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

	SelectObject(hdc,g_hf6AcuNorm);

	rc.top = 152;
	rc.bottom = 207;
	rc.left = 10;

	szUnit=_T("");
	switch(unit)
	{
	case CO2_UNIT_KPA:
		{
			szUnit +=_T("[")+getModel()->GetLanguageString(IDS_UNIT_KPA)+_T("]");
		}
		break;
	case CO2_UNIT_PERCENT:
		{
			szUnit +=_T("[")+getModel()->GetLanguageString(IDS_UNIT_VOLPERCENT)+_T("]");
		}
		break;
	case CO2_UNIT_MMHG:
	default:
		{
			szUnit +=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MMHG)+_T("]");
		}
		break;
	}
	pDC->DrawText(szUnit,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);


	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
}
void CViewAlarmLimit::drawLabel_SPO2(CDC* pDC)
{
	if(m_bExit)
		return;

	HDC hdc = *pDC;

	RECT rc;

	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf10AcuBold);
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));

	CStringW nameText=_T("");

	rc.left = 10;
	rc.right = 210;


	SelectObject(hdc,g_hf10AcuBold);
	rc.top = 46;
	rc.bottom = 79;

	//--------------------SPO2-------------------------------------
	nameText=_T("SpO");
	CSize sz = pDC->GetTextExtent(nameText);
	DrawText(hdc,nameText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

	SelectObject(hdc,g_hf7AcuBold);
	rc.top = 46;
	rc.bottom = 81;
	rc.left = 10+sz.cx;

	nameText=_T("2");
	DrawText(hdc,nameText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

	SelectObject(hdc,g_hf6AcuNorm);
	rc.top = 42;
	rc.bottom = 97;
	rc.left = 10;

	CString szUnit=_T("[")+getModel()->GetLanguageString(IDS_UNIT_PERCENT)+_T("]");
	pDC->DrawText(szUnit,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);


	SelectObject(hdc,g_hf10AcuBold);
	//--------------------PulseRate-------------------------------------
	rc.left = 10;
	rc.right = 210;
	rc.top = 156;
	rc.bottom = 189;

	nameText=_T("PR");
	sz = pDC->GetTextExtent(nameText);
	DrawText(hdc,nameText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

	SelectObject(hdc,g_hf6AcuNorm);
	rc.top = 152;
	rc.bottom = 207;
	rc.left = 10;

	szUnit=_T("[")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]");
	pDC->DrawText(szUnit,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	SelectObject(hdc,g_hf10AcuBold);

	//--------------------PerfusionIndex-------------------------------------
	rc.left = 10;
	rc.right = 210;
	rc.top = 266;
	rc.bottom = 354;

	nameText=_T("PI");
	DrawText(hdc,nameText,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	//--------------------SIQ-------------------------------------
	rc.top = 321;
	rc.bottom = 354;

	nameText=_T("SIQ");
	DrawText(hdc,nameText,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);


	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
}
void CViewAlarmLimit::drawLabel_VENTILATION(CDC* pDC)
{
	if(m_bExit)
		return;

	HDC hdc = *pDC;

	RECT rc;

	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf10AcuBold);
	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));

	CStringW nameText=_T("");

	rc.left = 10;
	rc.right = 210;

	if(m_eCurVentMode==VM_THERAPIE)
	{

	}
	else if(m_eCurVentMode==VM_HFO)
	{
		if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
		{
			//--------------------MV-------------------------------------
			rc.top = 46;
			rc.bottom = 79;
			nameText=getModel()->GetLanguageString(IDS_PARA_MV);
			DrawText(hdc,nameText,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
			CSize sz = pDC->GetTextExtent(nameText);

			SelectObject(hdc,g_hf7AcuBold);

			//--------------------exp-------------------------------------
			rc.top = 46;
			rc.bottom = 81;
			rc.left = 11+sz.cx;
			nameText=getModel()->GetLanguageString(IDS_PARA_EXP);
			DrawText(hdc,nameText,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

			SelectObject(hdc,g_hf6AcuNorm);

			//--------------------unit-------------------------------------
			rc.top = 42;
			rc.bottom = 97;
			rc.left = 10;

			if(getModel()->GetLanguageID()==LAN_CHINESE)
			{
				pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_LMIN)+_T("]"),&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
			}
			else
			{
				pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_LITER)+_T("]"),&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
			}
		}

		rc.left = 10;
		rc.right = 210;

		SelectObject(hdc,g_hf10AcuBold);
		//--------------------P-------------------------------------
		rc.top = 156;
		rc.bottom = 230;
		nameText=getModel()->GetLanguageString(IDS_PARA_PIP);
		DrawText(hdc,nameText,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		CSize sz1 = pDC->GetTextExtent(nameText);

		SelectObject(hdc,g_hf6AcuNorm);

		//--------------------Pmax-------------------------------------
		rc.top = 157;
		rc.bottom = 230;
		rc.left = 15+sz1.cx;
		if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
		{
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else
		{
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}

		SelectObject(hdc,g_hf10AcuBold);
		//--------------------DCO2-------------------------------------
		rc.left = 10;
		rc.right = 210;
		rc.top = 211;
		rc.bottom = 244;
		nameText=_T("DCO");
		DrawText(hdc,nameText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
		CSize sz = pDC->GetTextExtent(nameText);

		SelectObject(hdc,g_hf7AcuBold);

		rc.top = 216;
		rc.bottom = 249;
		rc.left = 10+sz.cx;
		nameText=_T("2");
		DrawText(hdc,nameText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

		SelectObject(hdc,g_hf6AcuNorm);

		rc.top = 233;
		rc.left = 10;
		rc.bottom = 262;

		pDC->DrawText(_T("[ml  /sec]"),&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		rc.bottom = 258;
		rc.left+=16;

		pDC->DrawText(_T("2"),&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		rc.left = 10;
		rc.right = 210;

		if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
		{
			SelectObject(hdc,g_hf10AcuBold);
			//--------------------Leak-------------------------------------
			rc.top = 321;
			rc.bottom = 416;
			nameText=getModel()->GetLanguageString(IDS_PARA_LEAK);
			DrawText(hdc,nameText,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
			CSize sz = pDC->GetTextExtent(nameText);

			SelectObject(hdc,g_hf6AcuNorm);

			rc.top = 322;
			rc.bottom = 416;
			rc.left = 15+sz.cx;
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_PERCENT)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
	}
	else if(m_eCurVentMode==VM_CPAP)
	{
		if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
		{
			//--------------------MV-------------------------------------
			rc.top = 46;
			rc.bottom = 79;
			nameText=getModel()->GetLanguageString(IDS_PARA_MV);
			DrawText(hdc,nameText,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
			CSize sz = pDC->GetTextExtent(nameText);

			SelectObject(hdc,g_hf7AcuBold);

			//--------------------exp-------------------------------------
			rc.top = 46;
			rc.bottom = 81;
			rc.left = 11+sz.cx;
			nameText=getModel()->GetLanguageString(IDS_PARA_EXP);
			DrawText(hdc,nameText,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

			SelectObject(hdc,g_hf6AcuNorm);

			//--------------------unit-------------------------------------
			rc.top = 42;
			rc.bottom = 97;
			rc.left = 10;

			if(getModel()->GetLanguageID()==LAN_CHINESE)
			{
				pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_LMIN)+_T("]"),&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
			}
			else
			{
				pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_LITER)+_T("]"),&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
			}
		}

		SelectObject(hdc,g_hf10AcuBold);

		rc.left = 10;
		rc.right = 210;
		rc.top = 110;
		rc.bottom = 189;
		nameText=getModel()->GetLanguageString(IDS_PARA_CPAP);
		DrawText(hdc,nameText,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		SelectObject(hdc,g_hf6AcuNorm);

		rc.top = 110;
		rc.bottom = 207;
		rc.left = 10;
		if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
		{
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("]"),&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
		}
		else
		{
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("]"),&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
		}

		rc.left = 10;
		rc.right = 210;


		if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
		{
			SelectObject(hdc,g_hf10AcuBold);

			//--------------------BPM-------------------------------------
			rc.top = 266;
			rc.bottom = 354;
			nameText=getModel()->GetLanguageString(IDS_PARA_FREQ);
			DrawText(hdc,nameText,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
			CSize sz = pDC->GetTextExtent(nameText);

			SelectObject(hdc,g_hf6AcuNorm);

			rc.top = 267;
			rc.bottom = 354;
			rc.left = 15+sz.cx;
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

			SelectObject(hdc,g_hf10AcuBold);
			//--------------------Leak-------------------------------------

			rc.top = 321;
			rc.bottom = 416;
			rc.left = 10;
			rc.right = 210;
			nameText=getModel()->GetLanguageString(IDS_PARA_LEAK);
			DrawText(hdc,nameText,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
			sz = pDC->GetTextExtent(nameText);

			SelectObject(hdc,g_hf6AcuNorm);

			rc.top = 322;
			rc.bottom = 416;
			rc.left = 15+sz.cx;
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_PERCENT)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_PRESSURE)
		{
			SelectObject(hdc,g_hf10AcuBold);

			//--------------------BPM-------------------------------------
			rc.top = 266;
			rc.bottom = 354;
			nameText=getModel()->GetLanguageString(IDS_PARA_FREQ);
			DrawText(hdc,nameText,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
			CSize sz = pDC->GetTextExtent(nameText);

			SelectObject(hdc,g_hf6AcuNorm);

			rc.top = 267;
			rc.bottom = 354;
			rc.left = 15+sz.cx;
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}

		if(		getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET
			||	(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false
				&& getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET))
		{
			SelectObject(hdc,g_hf10AcuBold);

			//--------------------Apnoe-------------------------------------
			rc.left = 10;
			rc.right = 210;
			rc.top = 376;
			rc.bottom = 426;

			nameText=getModel()->GetLanguageString(IDS_PARA_APNOE);
			DrawText(hdc,nameText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			CSize sz = pDC->GetTextExtent(nameText);

			SelectObject(hdc,g_hf6AcuNorm);

			rc.top = 376;
			rc.bottom = 426;
			rc.left = 15+sz.cx;
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_SECONDS)+_T("]"),&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
		}
		else if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_PRESSURE)
		{
			SelectObject(hdc,g_hf10AcuBold);

			//--------------------Apnoe-------------------------------------
			rc.left = 10;
			rc.right = 210;
			rc.top = 376;
			rc.bottom = 426;

			nameText=getModel()->GetLanguageString(IDS_PARA_APNOE);
			DrawText(hdc,nameText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			CSize sz = pDC->GetTextExtent(nameText);

			SelectObject(hdc,g_hf6AcuNorm);

			rc.top = 376;
			rc.bottom = 426;
			rc.left = 15+sz.cx;
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_SECONDS)+_T("]"),&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
		}
	}
	else if(m_eCurVentMode==VM_NCPAP)
	{
		SelectObject(hdc,g_hf10AcuBold);

		rc.left = 10;
		rc.right = 210;
		rc.top = 110;
		rc.bottom = 189;

		nameText=getModel()->GetLanguageString(IDS_PARA_CPAP);
		DrawText(hdc,nameText,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		SelectObject(hdc,g_hf6AcuNorm);

		rc.top = 110;
		rc.bottom = 207;
		rc.left = 10;
		if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
		{
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("]"),&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
		}
		else
		{
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("]"),&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
		}

		SelectObject(hdc,g_hf10AcuBold);

		//--------------------Apnoe-------------------------------------
		bool bShowApnea=false;
		if(		getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false 
			&&	getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET 
			&&	getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
		{
			bShowApnea=true;
		}
		else if(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
		{
			bShowApnea=true;
		}

		if(bShowApnea)
		{
			//---------- BPM -----------------------------
			rc.left = 10;
			rc.right = 210;
			rc.top = 266;
			rc.bottom = 316;

			/*rc.top = 321;
			rc.bottom = 416;
			rc.left = 10;
			rc.right = 210;*/
			nameText=getModel()->GetLanguageString(IDS_PARA_FREQ);
			DrawText(hdc,nameText,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
			CSize sz = pDC->GetTextExtent(nameText);

			SelectObject(hdc,g_hf6AcuNorm);

			/*rc.top = 322;
			rc.bottom = 416;
			rc.left = 15+sz.cx;*/

			rc.top = 266;
			rc.bottom = 316;
			rc.left = 15+sz.cx;
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

			SelectObject(hdc,g_hf10AcuBold);

			//-------------Apnea---------------------
			rc.top = 321;
			rc.bottom = 371;
			rc.left = 10;
			rc.right = 210;
			/*rc.left = 10;
			rc.right = 210;
			rc.top = 266;
			rc.bottom = 316;*/

			nameText=getModel()->GetLanguageString(IDS_PARA_APNOE);
			DrawText(hdc,nameText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			sz = pDC->GetTextExtent(nameText);

			SelectObject(hdc,g_hf6AcuNorm);

			/*rc.top = 266;
			rc.bottom = 316;
			rc.left = 15+sz.cx;*/

			rc.top = 322;
			rc.bottom = 372;
			rc.left = 15+sz.cx;
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_SECONDS)+_T("]"),&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

		}
	}
	else if(m_eCurVentMode==VM_DUOPAP)
	{
		SelectObject(hdc,g_hf10AcuBold);
		//--------------------P-------------------------------------
		rc.left = 10;
		rc.right = 210;
		rc.top = 156;
		rc.bottom = 230;

		nameText=getModel()->GetLanguageString(IDS_PARA_PIP);
		DrawText(hdc,nameText,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		CSize sz1 = pDC->GetTextExtent(nameText);

		SelectObject(hdc,g_hf6AcuNorm);

		//--------------------Pmax-------------------------------------
		rc.top = 157;
		rc.bottom = 230;
		rc.left = 15+sz1.cx;

		if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
		{
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else
		{
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}

		SelectObject(hdc,g_hf10AcuBold);
		//--------------------PEEP-------------------------------------
		rc.left = 10;
		rc.right = 210;
		rc.top = 211;
		rc.bottom = 292;

		nameText=getModel()->GetLanguageString(IDS_PARA_PEEP);
		DrawText(hdc,nameText,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		CSize sz3 = pDC->GetTextExtent(nameText);

		SelectObject(hdc,g_hf6AcuNorm);

		rc.top = 212;
		rc.bottom = 292;
		rc.left = 11+sz3.cx;
		if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
		{
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else
		{
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}

		SelectObject(hdc,g_hf10AcuBold);

		//--------------------Apnoe-------------------------------------
		bool bShowApnea=false;
		if(		getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false 
			&&	getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET 
			&&	getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
		{
			bShowApnea=true;
		}
		else if(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
		{
			bShowApnea=true;
		}

		if(bShowApnea)
		{

			//---------- BPM -----------------------------
			rc.left = 10;
			rc.right = 210;
			rc.top = 266;
			rc.bottom = 316;

			/*rc.top = 321;
			rc.bottom = 416;
			rc.left = 10;
			rc.right = 210;*/
			nameText=getModel()->GetLanguageString(IDS_PARA_FREQ);
			DrawText(hdc,nameText,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
			CSize sz = pDC->GetTextExtent(nameText);

			SelectObject(hdc,g_hf6AcuNorm);

			/*rc.top = 322;
			rc.bottom = 416;
			rc.left = 15+sz.cx;*/

			rc.top = 266;
			rc.bottom = 316;
			rc.left = 15+sz.cx;
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

			SelectObject(hdc,g_hf10AcuBold);

			//-------------Apnea---------------------
			rc.top = 321;
			rc.bottom = 371;
			rc.left = 10;
			rc.right = 210;
			/*rc.left = 10;
			rc.right = 210;
			rc.top = 266;
			rc.bottom = 316;*/

			nameText=getModel()->GetLanguageString(IDS_PARA_APNOE);
			DrawText(hdc,nameText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			sz = pDC->GetTextExtent(nameText);

			SelectObject(hdc,g_hf6AcuNorm);

			/*rc.top = 266;
			rc.bottom = 316;
			rc.left = 15+sz.cx;*/

			rc.top = 322;
			rc.bottom = 372;
			rc.left = 15+sz.cx;
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_SECONDS)+_T("]"),&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

		}
		
	}
	else //if(m_eCurVentMode!=VM_THERAPIE)
	{
		if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
		{
			//--------------------MV-------------------------------------
			rc.top = 46;
			rc.bottom = 79;
			nameText=getModel()->GetLanguageString(IDS_PARA_MV);
			DrawText(hdc,nameText,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
			CSize sz = pDC->GetTextExtent(nameText);

			SelectObject(hdc,g_hf7AcuBold);

			//--------------------exp-------------------------------------
			rc.top = 46;
			rc.bottom = 81;
			rc.left = 11+sz.cx;
			nameText=getModel()->GetLanguageString(IDS_PARA_EXP);
			DrawText(hdc,nameText,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

			SelectObject(hdc,g_hf6AcuNorm);

			//--------------------unit-------------------------------------
			rc.top = 42;
			rc.bottom = 97;
			rc.left = 10;

			if(getModel()->GetLanguageID()==LAN_CHINESE)
			{
				pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_LMIN)+_T("]"),&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
			}
			else
			{
				pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_LITER)+_T("]"),&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
			}
		}

		SelectObject(hdc,g_hf10AcuBold);
		//--------------------P-------------------------------------
		rc.left = 10;
		rc.right = 210;
		rc.top = 156;
		rc.bottom = 230;

		nameText=getModel()->GetLanguageString(IDS_PARA_PIP);
		DrawText(hdc,nameText,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		CSize sz1 = pDC->GetTextExtent(nameText);

		SelectObject(hdc,g_hf6AcuNorm);

		//--------------------Pmax-------------------------------------
		rc.top = 157;
		rc.bottom = 230;
		rc.left = 15+sz1.cx;

		if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
		{
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else
		{
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}

		SelectObject(hdc,g_hf10AcuBold);
		//--------------------PEEP-------------------------------------

		rc.left = 10;
		rc.right = 210;
		rc.top = 266;
		rc.bottom = 347;

		nameText=getModel()->GetLanguageString(IDS_PARA_PEEP);
		DrawText(hdc,nameText,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		CSize sz3 = pDC->GetTextExtent(nameText);

		SelectObject(hdc,g_hf6AcuNorm);

		rc.top = 267;
		rc.bottom = 347;
		rc.left = 11+sz3.cx;

		if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
		{
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else
		{
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}

		if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
		{
			SelectObject(hdc,g_hf10AcuBold);

			//--------------------BPM-------------------------------------
			rc.left = 10;
			rc.right = 210;
			rc.top = 321;
			rc.bottom = 409;

			nameText=getModel()->GetLanguageString(IDS_PARA_FREQ);
			DrawText(hdc,nameText,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
			CSize sz = pDC->GetTextExtent(nameText);

			SelectObject(hdc,g_hf6AcuNorm);

			rc.top = 322;
			rc.bottom = 409;
			rc.left = 15+sz.cx;
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

			SelectObject(hdc,g_hf10AcuBold);

			//--------------------Leak-------------------------------------
			rc.left = 10;
			rc.right = 210;
			rc.top = 376;
			rc.bottom = 471;
			nameText=getModel()->GetLanguageString(IDS_PARA_LEAK);
			DrawText(hdc,nameText,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
			sz = pDC->GetTextExtent(nameText);

			SelectObject(hdc,g_hf6AcuNorm);

			rc.top = 377;
			rc.bottom = 471;
			rc.left = 15+sz.cx;
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_PERCENT)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_PRESSURE
			&&	(m_eCurVentMode==VM_SIPPV || m_eCurVentMode==VM_SIMV))
		{
			SelectObject(hdc,g_hf10AcuBold);

			//--------------------BPM-------------------------------------
			rc.left = 10;
			rc.right = 210;
			rc.top = 321;
			rc.bottom = 409;

			nameText=getModel()->GetLanguageString(IDS_PARA_FREQ);
			DrawText(hdc,nameText,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
			CSize sz = pDC->GetTextExtent(nameText);

			SelectObject(hdc,g_hf6AcuNorm);

			rc.top = 322;
			rc.bottom = 409;
			rc.left = 15+sz.cx;
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]"),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}

		if(m_eCurVentMode!=VM_IPPV && m_eCurVentMode!=VM_SIPPV && getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
		{
			SelectObject(hdc,g_hf10AcuBold);

			//--------------------Apnoe-------------------------------------
			rc.top = 431;
			rc.bottom = 481;
			rc.left = 10;
			rc.right = 210;

			nameText=getModel()->GetLanguageString(IDS_PARA_APNOE);
			DrawText(hdc,nameText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			CSize sz = pDC->GetTextExtent(nameText);

			SelectObject(hdc,g_hf6AcuNorm);

			rc.top = 432;
			rc.bottom = 481;
			rc.left = 15+sz.cx;
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_SECONDS)+_T("]"),&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

		}
		else if(m_eCurVentMode!=VM_IPPV && m_eCurVentMode!=VM_SIPPV && getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_PRESSURE)
		{
			SelectObject(hdc,g_hf10AcuBold);

			//--------------------Apnoe-------------------------------------
			rc.top = 431;
			rc.bottom = 481;
			rc.left = 10;
			rc.right = 210;

			nameText=getModel()->GetLanguageString(IDS_PARA_APNOE);
			DrawText(hdc,nameText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			CSize sz = pDC->GetTextExtent(nameText);

			SelectObject(hdc,g_hf6AcuNorm);

			rc.top = 432;
			rc.bottom = 481;
			rc.left = 15+sz.cx;
			pDC->DrawText(_T("[")+getModel()->GetLanguageString(IDS_UNIT_SECONDS)+_T("]"),&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
		}
	}


	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
}
void CViewAlarmLimit::DrawData_etCO2(HDC hdc)
{
	RECT rc={0,0,m_lX,m_lY};
	TCHAR psz[MAX_PATH];

	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf34BoldNum);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));

	rc.left = 15;
	rc.right = 210;

	if(m_eCurVentMode!=VM_HFO && getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
	{
		BYTE iData=0;
		//--------------------BPM-------------------------------------
		rc.top = 280;
		rc.bottom = 315;
		if(getModel()->getETCO2()!=NULL)
		{
			if(getModel()->getETCO2()->isFreqValueValid())
			{
				iData = getModel()->getDATAHANDLER()->getAVGMessureDataBPMco2();
				wsprintf(psz,_T("%d"),iData);
				DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
			}
			else
			{
				DrawText(hdc,_T("--"),-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
			}
		}
		else
		{
			DrawText(hdc,_T("--"),-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
		}
	}

	//--------------------ETCO2-------------------------------------
	rc.top = 102;
	rc.bottom = 163;
	int iData = getModel()->getDATAHANDLER()->getAVGMessureDataETCO2();
	if(getModel()->getETCO2()!=NULL)
	{
		if(iData==2550 || iData==1510)
		{
			DrawText(hdc,_T("--"),-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else if(false==getModel()->getETCO2()->isETCO2ValueValid())
		{
			DrawText(hdc,_T("--"),-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else
		{
			if(iData!=0)
			{
				wsprintf(psz,_T("%0.1f"), (double)iData/10, 1);
			}
			else
				wsprintf(psz,_T("%d"), 0);
			DrawText(hdc,psz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
	}
	else
	{
		DrawText(hdc,_T("--"),-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}
	

	//--------------------FICO2-------------------------------------
	rc.top = 212;
	rc.bottom = 273;
	iData = getModel()->getDATAHANDLER()->getAVGMessureDataFICO2();
	if(getModel()->getETCO2()!=NULL)
	{
		if(iData==2550 || iData==1510)
		{
			DrawText(hdc,_T("--"),-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else if(false==getModel()->getETCO2()->isFICO2ValueValid())
		{
			DrawText(hdc,_T("--"),-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else
		{
			if(iData!=0)
			{
				wsprintf(psz,_T("%0.1f"), (double)iData/10, 1);
			}
			else
				wsprintf(psz,_T("%d"), 0);
			DrawText(hdc,psz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
	}
	else
	{
		DrawText(hdc,_T("--"),-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}
	

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
}
void CViewAlarmLimit::DrawData_SpO2(HDC hdc)
{
	//HDC hdc = *pDC;
	RECT rc={0,0,m_lX,m_lY};
	TCHAR psz[MAX_PATH];

	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf34BoldNum);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));

	rc.left = 15;
	rc.right = 210;

	//--------------------SPO2-------------------------------------
	rc.top = 102;
	rc.bottom = 163;
	int iData = getModel()->getDATAHANDLER()->getAVGMessureDataSPO2();
	
	if(iData!=0)
	{
		wsprintf(psz,_T("%0.0f"), CTlsFloat::Round(((double)iData)/10, 0));
	}
	else
		wsprintf(psz,_T("%d"), 0);
	DrawText(hdc,psz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	//--------------------PR-------------------------------------
	rc.top = 212;//102;
	rc.bottom = 273;//163;
	iData = getModel()->getDATAHANDLER()->getAVGMessureDataSpO2PulseRate();
	
	if(iData!=0)
	{
		wsprintf(psz,_T("%d"), iData, 1);
	}
	else
		wsprintf(psz,_T("%d"), 0);
	DrawText(hdc,psz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	//--------------------PI-------------------------------------
	rc.top = 280;
	rc.bottom = 315;
	iData = getModel()->getDATAHANDLER()->getAVGMessureDataSpO2PerfusionIndex();
	if(iData>=100)
	{
		wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)iData)/1000, 1));
	}
	else
	{
		wsprintf(psz,_T("%0.2f"),CTlsFloat::Round(((double)iData)/1000, 2));
	}

	DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	//--------------------SIQ-------------------------------------
	rc.top = 280;
	rc.bottom = 370;
	iData = getModel()->getDATAHANDLER()->readSpO2SIQ();
	if(iData!=0)
	{
		wsprintf(psz,_T("%d"), iData, 1);
	}
	else
		wsprintf(psz,_T("%d"), 0);

	DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);


	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);
}
void CViewAlarmLimit::DrawData_Vent(HDC hdc)
{
	if(m_eCurVentMode==VM_THERAPIE)
	{
		
	}
	else if(m_eCurVentMode==VM_HFO)
	{
		drawDataVentilation_HFO(hdc);
	}
	else if(m_eCurVentMode==VM_CPAP)
	{
		drawDataVentilation_CPAP(hdc);
	}
	else if(m_eCurVentMode==VM_NCPAP)
	{
		drawDataVentilation_NCPAP(hdc);
	}
	else if(m_eCurVentMode==VM_DUOPAP)
	{
		drawDataVentilation_DUOPAP(hdc);
	}
	else //if(m_eCurVentMode!=VM_THERAPIE)
	{
		drawDataVentilation_CONVENTIONAL(hdc);
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewAlarmLimit::drawDataVentilation_HFO(HDC hdc)
{
	RECT rc={0,0,m_lX,m_lY};
	TCHAR psz[MAX_PATH];

	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf34BoldNum);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));
	HPEN hpenprev=(HPEN)SelectObject(hdc, (HPEN)GetStockObject(NULL_PEN));
	CBrush cbrBack(RGB(200,200,200));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdc,cbrBack);

	rc.left = 15;
	rc.right = 210;

	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
	{

		//--------------------MV-------------------------------------
		rc.top = 102;
		rc.bottom = 163;
		WORD wordData = getModel()->getDATAHANDLER()->getAVGMessureDataMV();
		if(wordData!=0)
		{
			if(wordData>=10000)
				wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)wordData)/1000, 1));
			else
				wsprintf(psz,_T("%0.2f"), CTlsFloat::Round(((double)wordData)/1000, 2));
		}
		else
			wsprintf(psz,_T("%d"), 0);
		DrawText(hdc,psz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}

	//--------------------Pmax-------------------------------------
	rc.left = 15;
	rc.right = 210;
	rc.top = 166;
	rc.bottom = 205;

	wsprintf(psz,_T("%0.0f"), ((double)getModel()->getDATAHANDLER()->getBTBMessureDataPINSP())/10);
	DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	//--------------------DCO2max-------------------------------------
	rc.left = 15;
	rc.right = 210;
	rc.top = 267;//292;
	rc.bottom = 349;

	wsprintf(psz,_T("%d"),getModel()->getDATAHANDLER()->getAVGMessureDataDCO2());
	DrawText(hdc,psz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
	{
		//--------------------Leak-------------------------------------
		rc.left = 15;
		rc.right = 210;
		rc.top = 326;
		rc.bottom = 370;

		BYTE iBPM=getModel()->getDATAHANDLER()->getAVGMessureDataBPM();
		if(iBPM==0)
		{
			wsprintf(psz,_T("%s"), _T("--"));
		}
		else
		{
			wsprintf(psz,_T("%d"),getModel()->getDATAHANDLER()->getAVGMessureDataLeak());
		}
		DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
	}

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);

	SelectObject(hdc,hbrprev);
	SelectObject(hdc,hpenprev);
}

void CViewAlarmLimit::drawDataVentilation_CPAP(HDC hdc)
{
	RECT rc={0,0,m_lX,m_lY};
	TCHAR psz[MAX_PATH];

	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf34BoldNum);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));
	HPEN hpenprev=(HPEN)SelectObject(hdc, (HPEN)GetStockObject(NULL_PEN));
	CBrush cbrBack(RGB(200,200,200));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdc,cbrBack);

	rc.left = 15;
	rc.right = 210;

	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
	{
		//--------------------MV-------------------------------------
		rc.top = 102;
		rc.bottom = 163;
		WORD wordData = getModel()->getDATAHANDLER()->getAVGMessureDataMV();
		if(wordData!=0)
		{
			if(wordData>=10000)
				wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)wordData)/1000, 1));
			else
				wsprintf(psz,_T("%0.2f"), CTlsFloat::Round(((double)wordData)/1000, 2));
		}
		else
			wsprintf(psz,_T("%d"), 0);
		DrawText(hdc,psz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}

	rc.left = 15;
	rc.right = 210;

	rc.top = 210;
	rc.bottom = 249;
	wsprintf(psz,_T("%0.1f"), ((double)getModel()->getDATAHANDLER()->getAVGMessureDataPEEP())/10);
	DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
	{
		//--------------------Leak-------------------------------------
		rc.left = 15;
		rc.right = 210;
		rc.top = 326;
		rc.bottom = 370;

		BYTE iBPM=getModel()->getDATAHANDLER()->getAVGMessureDataBPM();
		if(iBPM==0)
		{
			wsprintf(psz,_T("%s"), _T("--"));
		}
		else
		{
			wsprintf(psz,_T("%d"),getModel()->getDATAHANDLER()->getAVGMessureDataLeak());
		}
		DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		//--------------------BPM-------------------------------------
		rc.top = 280;
		rc.bottom = 315;
		wsprintf(psz,_T("%d"),getModel()->getDATAHANDLER()->getAVGMessureDataBPM());
		DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
	}
	else if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_PRESSURE)
	{
		//--------------------BPM-------------------------------------
		rc.top = 280;
		rc.bottom = 315;
		wsprintf(psz,_T("%d"),getModel()->getDATAHANDLER()->getAVGMessureDataBPM());
		DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
	}

	//delay
	SelectObject(hdc,g_hf8AcuBold);

	Rectangle(hdc, 0, 431, 190, 536);

	rc.left = 0;  
	rc.top = 435;
	rc.right  = 190;  
	rc.bottom = 470;
	DrawText(hdc,getModel()->GetLanguageString(IDS_TXT_PMINDELAY),-1,&rc,DT_CENTER|DT_SINGLELINE|DT_TOP);

	rc.top = 517;  
	rc.bottom = 586;
	CStringW sz=_T("");
	sz.Format(_T("%s: %d - %d %s"),
		getModel()->GetLanguageString(IDS_TXT_RANGE),
		MIN_TIME_PMINDELAY,
		MAX_TIME_PMINDELAY,
		getModel()->GetLanguageString(IDS_UNIT_SECONDS));
	DrawText(hdc,sz,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	SelectObject(hdc,g_hf11AcuBold);

	rc.left = 145;  
	rc.top = 452;  
	rc.right  = 220;  
	rc.bottom = 512;
	DrawText(hdc,getModel()->GetLanguageString(IDS_UNIT_SECONDS),-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	CPen penBlack;
	penBlack.CreatePen(PS_SOLID,2,RGB(0,0,0));
	SelectObject(hdc,(HPEN)penBlack);

	MoveToEx(hdc, 40, 456, NULL);
	LineTo(hdc, 140, 456);

	MoveToEx(hdc, 40, 508, NULL);
	LineTo(hdc, 140, 508);

	penBlack.DeleteObject();

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);

	SelectObject(hdc,hbrprev);
	SelectObject(hdc,hpenprev);
}

void CViewAlarmLimit::drawDataVentilation_NCPAP(HDC hdc)
{
	RECT rc={0,0,m_lX,m_lY};
	TCHAR psz[MAX_PATH];

	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf34BoldNum);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));
	HPEN hpenprev=(HPEN)SelectObject(hdc, (HPEN)GetStockObject(NULL_PEN));
	CBrush cbrBack(RGB(200,200,200));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdc,cbrBack);

	rc.left = 15;
	rc.right = 210;

	rc.left = 15;
	rc.right = 210;

	rc.top = 210;
	rc.bottom = 249;
	SHORT iData = getModel()->getDATAHANDLER()->getAVGMessureDataPEEP();
	wsprintf(psz,_T("%0.1f"), ((double)iData)/10);
	DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	bool bShowBPM=false;
	if(		getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false 
		&&	getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET 
		&&	getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
	{
		bShowBPM=true;
	}
	else if(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
	{
		bShowBPM=true;
	}

	if(bShowBPM)
	{
		//--------------------BPM-------------------------------------
		rc.top = 280;
		rc.bottom = 315;
		wsprintf(psz,_T("%d"),getModel()->getDATAHANDLER()->getAVGMessureDataBPM());
		DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
	}

	//delay
	/*SelectObject(hdc,g_hf8AcuBold);
	Rectangle(hdc, 0, 321, 190, 426);

	rc.left = 0;  
	rc.top = 325;
	rc.right  = 190;  
	rc.bottom = 360;
	DrawText(hdc,getModel()->GetLanguageString(IDS_TXT_PMINDELAY),-1,&rc,DT_CENTER|DT_SINGLELINE|DT_TOP);

	rc.top = 407;  
	rc.bottom = 476;
	CStringW sz=_T("");
	sz.Format(_T("%s: %d - %d %s"),
		getModel()->GetLanguageString(IDS_TXT_RANGE),
		MIN_TIME_PMINDELAY,
		MAX_TIME_PMINDELAY,
		getModel()->GetLanguageString(IDS_UNIT_SECONDS));
	DrawText(hdc,sz,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	SelectObject(hdc,g_hf11AcuBold);

	rc.left = 145;  
	rc.top = 342;  
	rc.right  = 220;  
	rc.bottom = 402;
	DrawText(hdc,getModel()->GetLanguageString(IDS_UNIT_SECONDS),-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	CPen penBlack;
	penBlack.CreatePen(PS_SOLID,2,RGB(0,0,0));
	SelectObject(hdc,(HPEN)penBlack);

	MoveToEx(hdc, 40, 346, NULL);
	LineTo(hdc, 140, 346);

	MoveToEx(hdc, 40, 398, NULL);
	LineTo(hdc, 140, 398);

	penBlack.DeleteObject();*/

	//delay
	SelectObject(hdc,g_hf8AcuBold);

	Rectangle(hdc, 0, 431, 190, 536);

	rc.left = 0;  
	rc.top = 435;
	rc.right  = 190;  
	rc.bottom = 470;
	DrawText(hdc,getModel()->GetLanguageString(IDS_TXT_PMINDELAY),-1,&rc,DT_CENTER|DT_SINGLELINE|DT_TOP);

	rc.top = 517;  
	rc.bottom = 586;
	CStringW sz=_T("");
	sz.Format(_T("%s: %d - %d %s"),
		getModel()->GetLanguageString(IDS_TXT_RANGE),
		MIN_TIME_PMINDELAY,
		MAX_TIME_PMINDELAY,
		getModel()->GetLanguageString(IDS_UNIT_SECONDS));
	DrawText(hdc,sz,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	SelectObject(hdc,g_hf11AcuBold);

	rc.left = 145;  
	rc.top = 452;  
	rc.right  = 220;  
	rc.bottom = 512;
	DrawText(hdc,getModel()->GetLanguageString(IDS_UNIT_SECONDS),-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	CPen penBlack;
	penBlack.CreatePen(PS_SOLID,2,RGB(0,0,0));
	SelectObject(hdc,(HPEN)penBlack);

	MoveToEx(hdc, 40, 456, NULL);
	LineTo(hdc, 140, 456);

	MoveToEx(hdc, 40, 508, NULL);
	LineTo(hdc, 140, 508);

	penBlack.DeleteObject();

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);

	SelectObject(hdc,hbrprev);
	SelectObject(hdc,hpenprev);
}

void CViewAlarmLimit::drawDataVentilation_DUOPAP(HDC hdc)
{
	RECT rc={0,0,m_lX,m_lY};
	TCHAR psz[MAX_PATH];

	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf34BoldNum);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));
	HPEN hpenprev=(HPEN)SelectObject(hdc, (HPEN)GetStockObject(NULL_PEN));
	CBrush cbrBack(RGB(200,200,200));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdc,cbrBack);

	rc.left = 15;
	rc.right = 210;

	//--------------------Pmax-------------------------------------
	rc.left = 15;
	rc.right = 210;
	rc.top = 166;
	rc.bottom = 205;//227

	SHORT iData = getModel()->getDATAHANDLER()->getBTBMessureDataPINSP();
	wsprintf(psz,_T("%0.0f"), ((double)iData)/10);
	DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
	//--------------------PEEP-------------------------------------
	rc.top = 228;
	rc.bottom = 260;//289;
	iData = getModel()->getDATAHANDLER()->getAVGMessureDataPEEP();

	wsprintf(psz,_T("%0.1f"), ((double)iData)/10);
	DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	bool bShowBPM=false;
	if(		getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false 
		&&	getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET 
		&&	getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
	{
		bShowBPM=true;
	}
	else if(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
	{
		bShowBPM=true;
	}

	if(bShowBPM)
	{
		//--------------------BPM-------------------------------------
		rc.top = 280;
		rc.bottom = 315;
		wsprintf(psz,_T("%d"),getModel()->getDATAHANDLER()->getAVGMessureDataBPM());
		DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
	}

	//delay
	/*SelectObject(hdc,g_hf8AcuBold);
	Rectangle(hdc, 0, 321, 190, 426);

	rc.left = 0;  
	rc.top = 325;
	rc.right  = 190;  
	rc.bottom = 360;
	DrawText(hdc,getModel()->GetLanguageString(IDS_TXT_PMINDELAY),-1,&rc,DT_CENTER|DT_SINGLELINE|DT_TOP);

	rc.top = 407;  
	rc.bottom = 476;
	CStringW sz=_T("");
	sz.Format(_T("%s: %d - %d %s"),
		getModel()->GetLanguageString(IDS_TXT_RANGE),
		MIN_TIME_PMINDELAY,
		MAX_TIME_PMINDELAY,
		getModel()->GetLanguageString(IDS_UNIT_SECONDS));
	DrawText(hdc,sz,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	SelectObject(hdc,g_hf11AcuBold);

	rc.left = 145;  
	rc.top = 342;  
	rc.right  = 220;  
	rc.bottom = 402;
	DrawText(hdc,getModel()->GetLanguageString(IDS_UNIT_SECONDS),-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	CPen penBlack;
	penBlack.CreatePen(PS_SOLID,2,RGB(0,0,0));
	SelectObject(hdc,(HPEN)penBlack);

	MoveToEx(hdc, 40, 346, NULL);
	LineTo(hdc, 140, 346);

	MoveToEx(hdc, 40, 398, NULL);
	LineTo(hdc, 140, 398);

	penBlack.DeleteObject();*/


	//delay
	SelectObject(hdc,g_hf8AcuBold);

	Rectangle(hdc, 0, 431, 190, 536);

	rc.left = 0;  
	rc.top = 435;
	rc.right  = 190;  
	rc.bottom = 470;
	DrawText(hdc,getModel()->GetLanguageString(IDS_TXT_PMINDELAY),-1,&rc,DT_CENTER|DT_SINGLELINE|DT_TOP);

	rc.top = 517;  
	rc.bottom = 586;
	CStringW sz=_T("");
	sz.Format(_T("%s: %d - %d %s"),
		getModel()->GetLanguageString(IDS_TXT_RANGE),
		MIN_TIME_PMINDELAY,
		MAX_TIME_PMINDELAY,
		getModel()->GetLanguageString(IDS_UNIT_SECONDS));
	DrawText(hdc,sz,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	SelectObject(hdc,g_hf11AcuBold);

	rc.left = 145;  
	rc.top = 452;  
	rc.right  = 220;  
	rc.bottom = 512;
	DrawText(hdc,getModel()->GetLanguageString(IDS_UNIT_SECONDS),-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	CPen penBlack;
	penBlack.CreatePen(PS_SOLID,2,RGB(0,0,0));
	SelectObject(hdc,(HPEN)penBlack);

	MoveToEx(hdc, 40, 456, NULL);
	LineTo(hdc, 140, 456);

	MoveToEx(hdc, 40, 508, NULL);
	LineTo(hdc, 140, 508);

	penBlack.DeleteObject();
	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);

	SelectObject(hdc,hbrprev);
	SelectObject(hdc,hpenprev);
}

void CViewAlarmLimit::drawDataVentilation_CONVENTIONAL(HDC hdc)
{
	RECT rc={0,0,m_lX,m_lY};
	TCHAR psz[MAX_PATH];

	int nBkMode=SetBkMode(hdc,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdc,g_hf34BoldNum);
	int nPrevTxtColor=SetTextColor(hdc,RGB(0,0,0));
	HPEN hpenprev=(HPEN)SelectObject(hdc, (HPEN)GetStockObject(NULL_PEN));
	CBrush cbrBack(RGB(200,200,200));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdc,cbrBack);

	rc.left = 15;
	rc.right = 210;

	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
	{
		//--------------------MV-------------------------------------
		rc.top = 102;
		rc.bottom = 163;
		WORD wordData = getModel()->getDATAHANDLER()->getAVGMessureDataMV();
		if(wordData!=0)
		{
			if(wordData>=10000)
				wsprintf(psz,_T("%0.1f"), CTlsFloat::Round(((double)wordData)/1000, 1));
			else
				wsprintf(psz,_T("%0.2f"), CTlsFloat::Round(((double)wordData)/1000, 2));
		}
		else
			wsprintf(psz,_T("%d"), 0);
		DrawText(hdc,psz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}

	//--------------------Pmax-------------------------------------
	rc.left = 15;
	rc.right = 210;
	rc.top = 166;
	rc.bottom = 205;//227

	wsprintf(psz,_T("%0.0f"), ((double)getModel()->getDATAHANDLER()->getBTBMessureDataPINSP())/10);
	DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	//--------------------PEEP-------------------------------------
	rc.top = 228+55;
	rc.bottom = 260+55;

	wsprintf(psz,_T("%0.1f"), ((double)getModel()->getDATAHANDLER()->getAVGMessureDataPEEP())/10);
	DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

	if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
	{
		//--------------------Leak-------------------------------------
		rc.left = 15;
		rc.right = 210;
		rc.top = 326+55;
		rc.bottom = 370+55;

		BYTE iBPM=getModel()->getDATAHANDLER()->getAVGMessureDataBPM();
		if(iBPM==0)
		{
			wsprintf(psz,_T("%s"), _T("--"));
		}
		else
		{
			wsprintf(psz,_T("%d"),getModel()->getDATAHANDLER()->getAVGMessureDataLeak());
		}
		DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);

		//--------------------BPM-------------------------------------
		rc.top = 280+55;
		rc.bottom = 315+55;
		wsprintf(psz,_T("%d"),getModel()->getDATAHANDLER()->getAVGMessureDataBPM());
		DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
	}
	else if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_PRESSURE
		&&	(m_eCurVentMode==VM_SIPPV || m_eCurVentMode==VM_SIMV))
	{
		//--------------------BPM-------------------------------------
		rc.top = 280+55;
		rc.bottom = 315+55;
		wsprintf(psz,_T("%d"),getModel()->getDATAHANDLER()->getAVGMessureDataBPM());
		DrawText(hdc,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
	}

	SetTextColor(hdc,nPrevTxtColor);
	SetBkMode(hdc,nBkMode);
	SelectObject(hdc,hPrevFont);

	SelectObject(hdc,hbrprev);
	SelectObject(hdc,hpenprev);
}
// **************************************************************************
// 
// **************************************************************************
bool CViewAlarmLimit::CreateWndMenuAlarmLimits()
{
	// **********************************************************************
	// Init. line diagram
	if(m_pWndMenuAlarmLimits==NULL && m_lX>-1 && !m_bExit)
	{
		m_pWndMenuAlarmLimits = new CWndMenuAlarmLimits(this);
		RECT rcLd={0,551,800,600};

		//RECT rcLd={477,0,640,435};
		if(m_pWndMenuAlarmLimits->Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_ALARMLIMITSMENU))
		{
			m_pWndMenuAlarmLimits->Init();

			return true;
		}


	}
	return false;
}
// **************************************************************************
// 
// **************************************************************************
void CViewAlarmLimit::ShowWndMenuAlarmLimits(bool bShow)
{
	if(m_pWndMenuAlarmLimits)
	{
		m_pWndMenuAlarmLimits->Show(bShow);
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewAlarmLimit::Open()
{
	
}


// **************************************************************************
// 
// **************************************************************************
void CViewAlarmLimit::Close()
{
	
}


// **************************************************************************
// 
// **************************************************************************
void CViewAlarmLimit::Show()
{
	this->ShowWindow(SW_SHOW);

	ShowWndMenuAlarmLimits(true);

	SetViewFocus();
}

// **************************************************************************
// 
// **************************************************************************
void CViewAlarmLimit::Hide()
{
}

// **************************************************************************
// 
// **************************************************************************
void CViewAlarmLimit::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

// **************************************************************************
// 
// **************************************************************************
BOOL CViewAlarmLimit::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_LBUTTONDOWN:
		{
			
			if(!m_bExit)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_CURRENT_VIEWID,IDC_VIEW_ALARMLIMITS,0);
				PostMessage(WM_SET_ALIMITTIMER);
			}
		}
		break;
	case WM_KEYDOWN:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				/*CWnd* pFocWnd=CWnd::GetFocus();
				if(pFocWnd!=0)
				{
					int iID=pFocWnd->GetDlgCtrlID();

					DEBUGMSG(TRUE, (TEXT("CViewAlarmLimit::VK_SPACE() %d\r\n"),iID));

					if(iID==IDC_VIEW_ALARMLIMITS)
						SetViewFocus();
				}*/

				/*int iTest=m_iCurPara;
				DEBUGMSG(TRUE, (TEXT("CViewAlarmLimit::VK_SPACE() %d\r\n"),iTest));*/

				if(!m_bExit)
					PostMessage(WM_SET_ALIMITTIMER);
			}
			else if(pMsg->wParam==VK_UP)
			{
				/*CWnd* pFocWnd=CWnd::GetFocus();
				if(pFocWnd!=0)
				{
					int iID=pFocWnd->GetDlgCtrlID();

					DEBUGMSG(TRUE, (TEXT("CViewAlarmLimit::VK_SPACE() %d\r\n"),iID));

					if(iID==IDC_VIEW_ALARMLIMITS)
						SetViewFocus();
				}*/
				if(!m_bExit)
					PostMessage(WM_SET_ALIMITTIMER);

			}
			else if(pMsg->wParam==VK_DOWN)
			{
				/*CWnd* pFocWnd=CWnd::GetFocus();
				if(pFocWnd!=0)
				{
					int iID=pFocWnd->GetDlgCtrlID();

					DEBUGMSG(TRUE, (TEXT("CViewAlarmLimit::VK_SPACE() %d\r\n"),iID));

					if(iID==IDC_VIEW_ALARMLIMITS)
						SetViewFocus();
				}*/
				if(!m_bExit)
					PostMessage(WM_SET_ALIMITTIMER);

			}
			break;
		}
		break;
	default:
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}

// **************************************************************************
// 
// **************************************************************************
bool CViewAlarmLimit::IsButtonDepressed()
{
	bool bResult = false;

	if(m_pcAlarmLimit_MVhi)
	{
		if(m_pcAlarmLimit_MVhi->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_MVlow)
	{
		if(m_pcAlarmLimit_MVlow->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_PIPmax)
	{
		if(m_pcAlarmLimit_PIPmax->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_PIPmin)
	{
		if(m_pcAlarmLimit_PIPmin->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_PEEP)
	{
		if(m_pcAlarmLimit_PEEP->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_BPM)
	{
		if(m_pcAlarmLimit_BPM->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_Leak)
	{
		if(m_pcAlarmLimit_Leak->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_Apnoe)
	{
		if(m_pcAlarmLimit_Apnoe->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_DCO2hi)
	{
		if(m_pcAlarmLimit_DCO2hi->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_DCO2lo)
	{
		if(m_pcAlarmLimit_DCO2lo->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_ETCO2hi)
	{
		if(m_pcAlarmLimit_ETCO2hi->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_ETCO2lo)
	{
		if(m_pcAlarmLimit_ETCO2lo->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_FICO2hi)
	{
		if(m_pcAlarmLimit_FICO2hi->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_FICO2lo)
	{
		if(m_pcAlarmLimit_FICO2lo->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_SPO2hi)
	{
		if(m_pcAlarmLimit_SPO2hi->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_SPO2lo)
	{
		if(m_pcAlarmLimit_SPO2lo->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_PulseRatehi)
	{
		if(m_pcAlarmLimit_PulseRatehi->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_PulseRatelo)
	{
		if(m_pcAlarmLimit_PulseRatelo->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_SPO2_PIlo)
	{
		if(m_pcAlarmLimit_SPO2_PIlo->IsDepressed())
			bResult=true;
	}

	if(m_pcAlarmLimit_SPO2_SIQ)
	{
		if(m_pcAlarmLimit_SPO2_SIQ->IsDepressed())
			bResult=true;
	}

	return bResult;
}

// **************************************************************************
// 
// **************************************************************************
void CViewAlarmLimit::NotifyEvent(CMVEvent* pEvent)
{
	if(m_bExit)
		return;

	try
	{
		switch(pEvent->GetET())
		{
		case CMVEvent::ET_UIEVENT:
			{
				CMVEventUI* pUIEvent = (CMVEventUI*)pEvent;
				switch(pUIEvent->GetEventType())
				{
					case CMVEventUI::EV_BN_ALIMIT4:
						{
							PostMessage(WM_ALIMIT_ALARM_OFF);
						}
						break;
					case CMVEventUI::EV_BN_ALIMIT5:
						{
							if(m_pWndMenuAlarmLimits)
								m_pWndMenuAlarmLimits->PostMessage(WM_ALIMIT_LOGLIST);
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
				case CMVEventAlarm::EV_ALARMSTATE_CHANGED:
					{
						RefreshAlarmStates();
					}
					break;
				default:
					{

					}
					break;
				}
			}
			break;
		case CMVEvent::ET_CONTROL:
			{
				CMVEventControl* pCtrlEvent = (CMVEventControl*)pEvent;
				switch(pCtrlEvent->GetEventType())
				{
				case CMVEventControl::EV_CONTROL_FLOWSENSORSTATE:
				case CMVEventControl::EV_CONTROL_O2SENSORSTATE:
					{
						RedrawView();

					}
					break;
				case CMVEventControl::EV_CONTROL_STOP_VENTILATION:
					{
						if(m_pcAlarmLimit_MVhi)
							m_pcAlarmLimit_MVhi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_MVmax->getAlarmState());
						if(m_pcAlarmLimit_MVlow)
							m_pcAlarmLimit_MVlow->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_MVmin->getAlarmState());
						if(m_pcAlarmLimit_PIPmax)
							m_pcAlarmLimit_PIPmax->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_PIPmax->getAlarmState());
						if(m_pcAlarmLimit_PIPmin)
							m_pcAlarmLimit_PIPmin->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_PIPmin->getAlarmState());
						if(m_pcAlarmLimit_PEEP)
							m_pcAlarmLimit_PEEP->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_PEEPminLow->getAlarmState());
						if(m_pcAlarmLimit_BPM)
							m_pcAlarmLimit_BPM->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_BPMmax->getAlarmState());
						if(m_pcAlarmLimit_Leak)
							m_pcAlarmLimit_Leak->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_Leakmax->getAlarmState());
						if(m_pcAlarmLimit_Apnoe)
							m_pcAlarmLimit_Apnoe->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_Apnoe->getAlarmState());
						if(m_pcAlarmLimit_DCO2hi)
							m_pcAlarmLimit_DCO2hi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_DCO2max->getAlarmState());
						if(m_pcAlarmLimit_DCO2lo)
							m_pcAlarmLimit_DCO2lo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_DCO2min->getAlarmState());
						
						PostMessage(WM_SET_ALIMITTIMER);
					}
					break;
				default:
					break;
				}
			}
		case CMVEvent::ET_MATRIX://++++++++++++++++++++++++++++++Buttons MATRIX+++++++++++++++++++++++++++++++++++++++++
			{
				CMVEventMatrix* pUIEvent = (CMVEventMatrix*)pEvent;
				switch(pUIEvent->GetEventType())
				{
				case CMVEventMatrix::EV_BN_MATRIX_SYSALARM_SILENT:
				case CMVEventMatrix::EV_BN_MATRIX_ALARM_SILENT:
					{
						if(getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_ACTIVE)
						{
							if(m_pcAlarmLimit_MVhi)
							{
								m_pcAlarmLimit_MVhi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_MVmax->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_MVlow)
							{
								m_pcAlarmLimit_MVlow->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_MVmin->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_PIPmax)
							{
								m_pcAlarmLimit_PIPmax->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_PIPmax->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_PIPmin)
							{
								m_pcAlarmLimit_PIPmin->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_PIPmin->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_PEEP)
							{
								m_pcAlarmLimit_PEEP->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_PEEPminLow->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_BPM)
							{
								m_pcAlarmLimit_BPM->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_BPMmax->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_Leak)
							{
								m_pcAlarmLimit_Leak->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_Leakmax->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_Apnoe)
							{
								m_pcAlarmLimit_Apnoe->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_Apnoe->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_DCO2hi)
							{
								m_pcAlarmLimit_DCO2hi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_DCO2max->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_DCO2lo)
							{
								m_pcAlarmLimit_DCO2lo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_DCO2min->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_ETCO2hi)
							{
								m_pcAlarmLimit_ETCO2hi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_ETCO2max->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_ETCO2lo)
							{
								m_pcAlarmLimit_ETCO2lo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_ETCO2min->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_FICO2hi)
							{
								m_pcAlarmLimit_FICO2hi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_FICO2max->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_FICO2lo)
							{
								m_pcAlarmLimit_FICO2lo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_FICO2min->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_SPO2hi)
							{
								m_pcAlarmLimit_SPO2hi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2max->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_SPO2lo)
							{
								m_pcAlarmLimit_SPO2lo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2min->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_PulseRatehi)
							{
								m_pcAlarmLimit_PulseRatehi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_PulseRatemax->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_PulseRatelo)
							{
								m_pcAlarmLimit_PulseRatelo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_PulseRatemin->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_SPO2_PIlo)
							{
								m_pcAlarmLimit_SPO2_PIlo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2_PImin->getAlarmState(), true);
							}
							if(m_pcAlarmLimit_SPO2_SIQ)
							{
								m_pcAlarmLimit_SPO2_SIQ->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2_SIQmin->getAlarmState(), true);
							}
							PostMessage(WM_SET_ALIMITTIMER);
						}
					}
				default:
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
	catch (...)
	{
		theApp.ReportException(_T("CViewAlarmLimit::NotifyEvent"));
	}
}
void CViewAlarmLimit::NotifyVentModeChanged()
{
	if(m_bExit)
		return;

	try
	{
		if(m_pcAlarmLimit_PEEP)
			m_pcAlarmLimit_PEEP->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePEEPmin(),
			getModel()->getALARMHANDLER()->getAlimitMaxRangePEEPmin());

		if(m_pcAlarmLimit_PIPmax)
			m_pcAlarmLimit_PIPmax->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePIPmax(),
			getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmax());

		if(m_pcAlarmLimit_PIPmin)
			m_pcAlarmLimit_PIPmin->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePIPmin(),
			getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmin());
	}
	catch (...)
	{
		theApp.ReportException(_T("CViewAlarmLimit::NotifyVentModeChanged"));
	}
}

void CViewAlarmLimit::RedrawView()
{
	
	m_eCurVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	
	m_eCurALimitNumeric=getModel()->getCONFIG()->getCurALimitNumeric();

	//DEBUGMSG(TRUE, (TEXT("RedrawView() CurALimitNumeric:%d\r\n"),(int)m_eCurALimitNumeric));
	DrawData(true, true);
	
	//make Buttons visible 
	showALimitButtons();

	if(m_pWndMenuAlarmLimits)
		m_pWndMenuAlarmLimits->PostMessage(WM_ALIMIT_REFRESH);
}

void CViewAlarmLimit::showALimitButtons()
{
	if(m_bExit)
		return;

	if(m_pbtnValueDelay)
	{
		m_pbtnValueDelay->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_MVhi)
	{
		m_pcAlarmLimit_MVhi->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_MVlow)
	{
		m_pcAlarmLimit_MVlow->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_PIPmax)
	{
		m_pcAlarmLimit_PIPmax->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_PIPmin)
	{
		m_pcAlarmLimit_PIPmin->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_PEEP)
	{
		m_pcAlarmLimit_PEEP->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_BPM)
	{
		m_pcAlarmLimit_BPM->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_Leak)
	{
		m_pcAlarmLimit_Leak->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_Apnoe)
	{
		m_pcAlarmLimit_Apnoe->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_DCO2hi)
	{
		m_pcAlarmLimit_DCO2hi->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_DCO2lo)
	{
		m_pcAlarmLimit_DCO2lo->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_ETCO2hi)
	{
		m_pcAlarmLimit_ETCO2hi->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_ETCO2lo)
	{
		m_pcAlarmLimit_ETCO2lo->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_FICO2hi)
	{
		m_pcAlarmLimit_FICO2hi->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_FICO2lo)
	{
		m_pcAlarmLimit_FICO2lo->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_SPO2hi)
	{
		m_pcAlarmLimit_SPO2hi->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_SPO2lo)
	{
		m_pcAlarmLimit_SPO2lo->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_PulseRatehi)
	{
		m_pcAlarmLimit_PulseRatehi->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_PulseRatelo)
	{
		m_pcAlarmLimit_PulseRatelo->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_SPO2_PIlo)
	{
		m_pcAlarmLimit_SPO2_PIlo->ShowWindow(SW_HIDE);
	}
	if(m_pcAlarmLimit_SPO2_SIQ)
	{
		m_pcAlarmLimit_SPO2_SIQ->ShowWindow(SW_HIDE);
	}

	
	UpdateWindow();
	Invalidate();

	/*********************************************************************************/
	if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
	{
		if(m_pbtnValueDelay)
		{
			m_pbtnValueDelay->ShowWindow(SW_HIDE);
		}

		if(m_eCurVentMode!=VM_HFO && getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
		{
			if(m_pcAlarmLimit_BPM)
			{
				m_pcAlarmLimit_BPM->ShowWindow(SW_SHOW);
			}
		}

		if(m_pcAlarmLimit_ETCO2hi)
		{
			m_pcAlarmLimit_ETCO2hi->ShowWindow(SW_SHOW);
		}

		if(m_pcAlarmLimit_ETCO2lo)
		{
			m_pcAlarmLimit_ETCO2lo->ShowWindow(SW_SHOW);
		}

		if(m_pcAlarmLimit_FICO2hi)
		{
			m_pcAlarmLimit_FICO2hi->ShowWindow(SW_SHOW);
		}

		if(m_pcAlarmLimit_FICO2lo)
		{
			m_pcAlarmLimit_FICO2lo->ShowWindow(SW_SHOW);
		}
	}
	else if (m_eCurALimitNumeric==ALIMITNUM_SPO2)
	{
		if(m_pbtnValueDelay)
		{
			m_pbtnValueDelay->ShowWindow(SW_HIDE);
		}

		if(m_pcAlarmLimit_SPO2hi)
		{
			m_pcAlarmLimit_SPO2hi->ShowWindow(SW_SHOW);
		}

		if(m_pcAlarmLimit_SPO2lo)
		{
			m_pcAlarmLimit_SPO2lo->ShowWindow(SW_SHOW);
		}

		if(m_pcAlarmLimit_PulseRatehi)
		{
			m_pcAlarmLimit_PulseRatehi->ShowWindow(SW_SHOW);
		}

		if(m_pcAlarmLimit_PulseRatelo)
		{
			m_pcAlarmLimit_PulseRatelo->ShowWindow(SW_SHOW);
		}

		if(m_pcAlarmLimit_SPO2_PIlo)
		{
			m_pcAlarmLimit_SPO2_PIlo->ShowWindow(SW_SHOW);
		}

		if(m_pcAlarmLimit_SPO2_SIQ)
		{
			m_pcAlarmLimit_SPO2_SIQ->ShowWindow(SW_SHOW);
		}
	}
	else //if (m_eCurALimitNumeric==ALIMITNUM_VENTILATION)
	{
		//-------------------------------------------------------------
		//Button------MV---------------------------------
		if(m_eCurVentMode!=VM_THERAPIE && m_eCurVentMode!=VM_NCPAP && m_eCurVentMode!=VM_DUOPAP && getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
		{
			if(m_pcAlarmLimit_MVhi)
			{
				m_pcAlarmLimit_MVhi->ShowWindow(SW_SHOW);
			}

			if(m_pcAlarmLimit_MVlow)
			{
				m_pcAlarmLimit_MVlow->ShowWindow(SW_SHOW);
			}

		}

		if(m_eCurVentMode!=VM_THERAPIE && m_eCurVentMode!=VM_NCPAP && m_eCurVentMode!=VM_CPAP && m_eCurVentMode!=VM_DUOPAP && m_eCurVentMode!=VM_HFO)
		{
			if(m_pcAlarmLimit_PIPmin)
			{
				m_pcAlarmLimit_PIPmin->ShowWindow(SW_SHOW);
			}
		}

		if(m_eCurVentMode!=VM_THERAPIE)
		{
			if(m_pcAlarmLimit_PIPmax)
			{
				m_pcAlarmLimit_PIPmax->ShowWindow(SW_SHOW);
			}
			

			if(m_eCurVentMode!=VM_HFO)
			{
				if(m_pcAlarmLimit_PEEP)
				{
					m_pcAlarmLimit_PEEP->ShowWindow(SW_SHOW);
				}
			}

			if(m_eCurVentMode==VM_HFO)
			{
				if(m_pcAlarmLimit_DCO2hi)
				{
					m_pcAlarmLimit_DCO2hi->ShowWindow(SW_SHOW);
				}

				if(m_pcAlarmLimit_DCO2lo)
				{
					m_pcAlarmLimit_DCO2lo->ShowWindow(SW_SHOW);
				}
			}
		}


		if(m_eCurVentMode!=VM_THERAPIE && m_eCurVentMode!=VM_NCPAP && m_eCurVentMode!=VM_DUOPAP && getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
		{
			if(m_pcAlarmLimit_Leak)
			{
				m_pcAlarmLimit_Leak->ShowWindow(SW_SHOW);
			}
		}

		if(m_eCurVentMode==VM_IPPV && getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true)
		{
			//do not display
		}
		else if(m_eCurVentMode!=VM_THERAPIE && m_eCurVentMode!=VM_NCPAP && m_eCurVentMode!=VM_DUOPAP && m_eCurVentMode!=VM_HFO && getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
		{
			if(m_pcAlarmLimit_BPM)
			{
				m_pcAlarmLimit_BPM->ShowWindow(SW_SHOW);
			}
		}
		else if(m_eCurVentMode!=VM_THERAPIE && m_eCurVentMode!=VM_NCPAP && m_eCurVentMode!=VM_DUOPAP && m_eCurVentMode!=VM_HFO && getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_PRESSURE)
		{
			if(m_pcAlarmLimit_BPM)
			{
				m_pcAlarmLimit_BPM->ShowWindow(SW_SHOW);
			}
		}
		
		if(m_eCurVentMode==VM_NCPAP || m_eCurVentMode==VM_DUOPAP)//rku,PRETRIGGER
		{
			if(		getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false 
				&&	getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET 
				&&	getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
			{
				if(m_pcAlarmLimit_Apnoe)
				{
					m_pcAlarmLimit_Apnoe->ShowWindow(SW_SHOW);
				}
				if(m_pcAlarmLimit_BPM)
				{
					m_pcAlarmLimit_BPM->ShowWindow(SW_SHOW);
				}
			}
			else if(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getDATAHANDLER()->PARADATA()->GetTriggerNMODEPara()!=MAXRANGE_TRIGGER_NMODE_OFF)
			{
				if(m_pcAlarmLimit_Apnoe)
				{
					m_pcAlarmLimit_Apnoe->ShowWindow(SW_SHOW);
				}
				if(m_pcAlarmLimit_BPM)
				{
					m_pcAlarmLimit_BPM->ShowWindow(SW_SHOW);
				}
			}
		}
		else if(m_eCurVentMode==VM_CPAP && getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET)
		{
			if(m_pcAlarmLimit_Apnoe)
			{
				m_pcAlarmLimit_Apnoe->ShowWindow(SW_SHOW);
			}
		}
		else if(m_eCurVentMode==VM_CPAP && getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET && getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
		{
			if(m_pcAlarmLimit_Apnoe)
			{
				m_pcAlarmLimit_Apnoe->ShowWindow(SW_SHOW);
			}
		}
		else if(	m_eCurVentMode!=VM_IPPV && m_eCurVentMode!=VM_SIPPV && m_eCurVentMode!=VM_THERAPIE && m_eCurVentMode!=VM_HFO 
				&&	getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false )
		{
			if(m_pcAlarmLimit_Apnoe)
			{
				m_pcAlarmLimit_Apnoe->ShowWindow(SW_SHOW);
			}
		}
		else if(	m_eCurVentMode!=VM_IPPV && m_eCurVentMode!=VM_SIPPV && m_eCurVentMode!=VM_THERAPIE && m_eCurVentMode!=VM_HFO 
				&&	getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_PRESSURE)
		{
			if(m_pcAlarmLimit_Apnoe)
			{
				m_pcAlarmLimit_Apnoe->ShowWindow(SW_SHOW);
			}
		}
		

		if(m_eCurVentMode==VM_NCPAP || m_eCurVentMode==VM_DUOPAP || m_eCurVentMode==VM_CPAP)
		{
			if(m_pbtnValueDelay)
			{
				m_pbtnValueDelay->ShowWindow(SW_SHOW);
			}
		}
	}



}

void CViewAlarmLimit::NotifyParaBtnEvent(CMVEvent* pEvent)
{
	if(m_bExit)
		return;

	bool bRefresh=false;
	try
	{
		switch(pEvent->GetET())
		{
		case CMVEvent::ET_UIEVENT:
			{
				CMVEventUI* pUIEvent = (CMVEventUI*)pEvent;
				switch(pUIEvent->GetEventType())
				{
				case CMVEventUI::EV_PARABN_PEEP:
					{
						if(m_pcAlarmLimit_PEEP)
							m_pcAlarmLimit_PEEP->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePEEPmin(),
																getModel()->getALARMHANDLER()->getAlimitMaxRangePEEPmin());

						if(m_pcAlarmLimit_PIPmax)
							m_pcAlarmLimit_PIPmax->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePIPmax(),
							getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmax());

						if(m_pcAlarmLimit_PIPmin)
							m_pcAlarmLimit_PIPmin->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePIPmin(),
																	getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmin());

						if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
						{
							if(m_pcAlarmLimit_PEEP)
								m_pcAlarmLimit_PEEP->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPEEPmin(),true);
							if(m_pcAlarmLimit_PIPmin)
								m_pcAlarmLimit_PIPmin->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPIPmin(),true);
						}
						if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
						{
							if(m_pcAlarmLimit_PIPmax)
								m_pcAlarmLimit_PIPmax->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPIPmax(),true);
						}
						if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
						{
							if(m_pcAlarmLimit_PIPmin)
								m_pcAlarmLimit_PIPmin->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPIPmin(),true);
						}
						bRefresh=true;
					}
					break;
				case CMVEventUI::EV_PARABN_CPAP:
					{
						if(m_pcAlarmLimit_PEEP)
							m_pcAlarmLimit_PEEP->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePEEPmin(),
							getModel()->getALARMHANDLER()->getAlimitMaxRangePEEPmin());

						if(m_pcAlarmLimit_PIPmax)
							m_pcAlarmLimit_PIPmax->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePIPmax(),
							getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmax());

						if(m_pcAlarmLimit_PIPmin)
							m_pcAlarmLimit_PIPmin->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePIPmin(),
							getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmin());

						if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
						{
							if(m_pcAlarmLimit_PEEP)
								m_pcAlarmLimit_PEEP->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPEEPmin(),true);
							if(m_pcAlarmLimit_PIPmin)
								m_pcAlarmLimit_PIPmin->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPIPmin(),true);
						}
						if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
						{
							if(m_pcAlarmLimit_PIPmax)
								m_pcAlarmLimit_PIPmax->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPIPmax(),true);
						}
						if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
						{
							if(m_pcAlarmLimit_PIPmin)
								m_pcAlarmLimit_PIPmin->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPIPmin(),true);
						}
						bRefresh=true;
					}
					break;
				case CMVEventUI::EV_PARABN_PPSV:
				case CMVEventUI::EV_PARABN_PINSP:
				case CMVEventUI::EV_PARABN_PMAXVOLG:
					{
						if(m_pcAlarmLimit_PEEP)
							m_pcAlarmLimit_PEEP->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePEEPmin(),
							getModel()->getALARMHANDLER()->getAlimitMaxRangePEEPmin());

						if(m_pcAlarmLimit_PIPmax)
							m_pcAlarmLimit_PIPmax->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePIPmax(),
							getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmax());

						if(m_pcAlarmLimit_PIPmin)
							m_pcAlarmLimit_PIPmin->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePIPmin(),
							getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmin());

						if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
						{
							if(m_pcAlarmLimit_PIPmax)
								m_pcAlarmLimit_PIPmax->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPIPmax(),true);
						}
						if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
						{
							if(m_pcAlarmLimit_PIPmin)
								m_pcAlarmLimit_PIPmin->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPIPmin(),true);
						}
						bRefresh=true;
					}
					break;
				case CMVEventUI::EV_PARABN_HFAMPL:
				case CMVEventUI::EV_PARABN_PMITT:
				case CMVEventUI::EV_PARABN_IERATIO:
				case CMVEventUI::EV_PARABN_HFFREQREC:
					{
						if(m_pcAlarmLimit_PEEP)
							m_pcAlarmLimit_PEEP->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePEEPmin(),
							getModel()->getALARMHANDLER()->getAlimitMaxRangePEEPmin());

						if(m_pcAlarmLimit_PIPmax)
							m_pcAlarmLimit_PIPmax->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePIPmax(),
							getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmax());

						if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
						{
							if(m_pcAlarmLimit_PIPmax)
								m_pcAlarmLimit_PIPmax->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPIPmax(),true);
						}

						bRefresh=true;
					}
					break;
				case CMVEventUI::EV_PARABN_PDUO:
					{
						if(m_pcAlarmLimit_PEEP)
							m_pcAlarmLimit_PEEP->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePEEPmin(),
							getModel()->getALARMHANDLER()->getAlimitMaxRangePEEPmin());

						if(m_pcAlarmLimit_PIPmax)
							m_pcAlarmLimit_PIPmax->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePIPmax(),
							getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmax());

						if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
						{
							if(m_pcAlarmLimit_PIPmax)
								m_pcAlarmLimit_PIPmax->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPIPmax(),true);
						}

						bRefresh=true;
					}
					break;
				case CMVEventUI::EV_PARABN_PMAN:
					{
						if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
						{
							if(m_pcAlarmLimit_PIPmax)
								m_pcAlarmLimit_PIPmax->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPIPmax(),true);
						}

						bRefresh=true;
					}
					break;
				case CMVEventUI::EV_PARABN_FOT:
					{
						if(m_pcAlarmLimit_PEEP)
							m_pcAlarmLimit_PEEP->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePEEPmin(),
							getModel()->getALARMHANDLER()->getAlimitMaxRangePEEPmin());

						if(m_pcAlarmLimit_PIPmax)
							m_pcAlarmLimit_PIPmax->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePIPmax(),
							getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmax());

						if(m_pcAlarmLimit_PIPmin)
							m_pcAlarmLimit_PIPmin->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePIPmin(),
							getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmin());

						if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
						{
							if(m_pcAlarmLimit_PEEP)
								m_pcAlarmLimit_PEEP->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPEEPmin(),true);
							if(m_pcAlarmLimit_PIPmin)
								m_pcAlarmLimit_PIPmin->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPIPmin(),true);
						}
						if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
						{
							if(m_pcAlarmLimit_PIPmax)
								m_pcAlarmLimit_PIPmax->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPIPmax(),true);
						}
						if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
						{
							if(m_pcAlarmLimit_PIPmin)
								m_pcAlarmLimit_PIPmin->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPIPmin(),true);
						}
						bRefresh=true;
					}
					break;
				default:
					bRefresh=true;
					break;
				}
			}
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CViewAlarmLimit::NotifyParaBtnEvent"));
	}

	if(bRefresh)
		RefreshAlarmStates();
}

bool CViewAlarmLimit::isActicePatientAlarmLimit()
{
	bool bRes=false;

	eAlarm ePrimaryAlarm = getModel()->getALARMHANDLER()->getActiveAlarm();
	switch(ePrimaryAlarm)
	{
	case AL_PatAl_BPMmax:
	case AL_PatAl_MVmax:
	case AL_PatAl_MVmin:
	case AL_PatAl_PIPmax:
	case AL_PatAl_PIPmin:
	case AL_PatAl_PEEPminLow:
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
	case AL_PatAl_SPO2_SIQmin:
		{
			bRes=true;
		}
		break;
	}
	return bRes;
}

// **************************************************************************
// 
// **************************************************************************
void CViewAlarmLimit::RefreshAlarmStates()
{
	//DEBUGMSG(TRUE, (TEXT("RedrawView() RefreshAlarmStates:%d\r\n"),(int)m_eCurALimitNumeric));

	if(m_bExit)
		return;

	bool bPatientAlarmLimit=false;
	
	if(getModel()->getALARMHANDLER()->isActiveAlarmType(AT_PATIENT))
	{
		if(isActicePatientAlarmLimit())
		{
			bPatientAlarmLimit=true;
			KillTimer(ALIMITTIMER);
		}
	}
	
	if(m_pcAlarmLimit_MVhi)
	{
		m_pcAlarmLimit_MVhi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_MVmax->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_MVlow)
	{
		m_pcAlarmLimit_MVlow->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_MVmin->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_PIPmax)
	{
		m_pcAlarmLimit_PIPmax->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_PIPmax->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_PIPmin)
	{
		m_pcAlarmLimit_PIPmin->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_PIPmin->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_PEEP)
	{
		m_pcAlarmLimit_PEEP->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_PEEPminLow->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_BPM)
	{
		m_pcAlarmLimit_BPM->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_BPMmax->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_Leak)
	{
		m_pcAlarmLimit_Leak->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_Leakmax->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_Apnoe)
	{
		m_pcAlarmLimit_Apnoe->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_Apnoe->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_DCO2hi)
	{
		m_pcAlarmLimit_DCO2hi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_DCO2max->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_DCO2lo)
	{
		m_pcAlarmLimit_DCO2lo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_DCO2min->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_ETCO2hi)
	{
		m_pcAlarmLimit_ETCO2hi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_ETCO2max->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_ETCO2lo)
	{
		m_pcAlarmLimit_ETCO2lo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_ETCO2min->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_FICO2hi)
	{
		m_pcAlarmLimit_FICO2hi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_FICO2max->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_FICO2lo)
	{
		m_pcAlarmLimit_FICO2lo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_FICO2min->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_SPO2hi)
	{
		m_pcAlarmLimit_SPO2hi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2max->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_SPO2lo)
	{
		m_pcAlarmLimit_SPO2lo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2min->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_PulseRatehi)
	{
		m_pcAlarmLimit_PulseRatehi->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_PulseRatemax->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_PulseRatelo)
	{
		m_pcAlarmLimit_PulseRatelo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_PulseRatemin->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_SPO2_PIlo)
	{
		m_pcAlarmLimit_SPO2_PIlo->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2_PImin->getAlarmState(), true);
	}
	if(m_pcAlarmLimit_SPO2_SIQ)
	{
		m_pcAlarmLimit_SPO2_SIQ->setAlarm(getModel()->getALARMHANDLER()->ALARM_PatAl_SPO2_SIQmin->getAlarmState(), true);
	}

	
	
	if(bPatientAlarmLimit)
	{
		bool bViewOwnsFocus=true;
		CWnd* pFocWnd = CWnd::GetFocus();
		if(pFocWnd!=NULL)
		{
			int iID=pFocWnd->GetDlgCtrlID();
			if(iID<IDC_BTN_ALARM_MV_HI || iID>IDC_BTN_ALARM_SPO2_PI_LO)
				bViewOwnsFocus=false;
		}

		//eALimitNumeric eTempALimitNumeric=ALIMITNUM_VENTILATION;
		eALimitNumeric eTempALimitNumeric=m_eCurALimitNumeric;
		eAlarm ePrimaryAlarm = getModel()->getALARMHANDLER()->getActiveAlarm();
 
		switch(ePrimaryAlarm)
		{
		case AL_PatAl_BPMmax:
			{
				if(		getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true
					&&	getModel()->getCONFIG()->GetCO2module()!=CO2MODULE_NONE)
				{
					eTempALimitNumeric=ALIMITNUM_ETCO2;
					//DEBUGMSG(TRUE, (TEXT("RefreshAlarmStates() ALIMITNUM_ETCO2\r\n")));
				}
				else
				{
					eTempALimitNumeric=ALIMITNUM_VENTILATION;
					//DEBUGMSG(TRUE, (TEXT("RefreshAlarmStates() ALIMITNUM_VENTILATION\r\n")));
				}
			}
			break;
		case AL_PatAl_MVmax:
		case AL_PatAl_MVmin:
		case AL_PatAl_PIPmax:
		case AL_PatAl_PIPmin:
		case AL_PatAl_PEEPminLow:
		case AL_PatAl_Leakmax:
		case AL_PatAl_Apnoe:
		case AL_PatAl_DCO2max:
		case AL_PatAl_DCO2min:
			{
				eTempALimitNumeric=ALIMITNUM_VENTILATION;
				//DEBUGMSG(TRUE, (TEXT("RefreshAlarmStates() ALIMITNUM_VENTILATION\r\n")));
			}
			break;
		case AL_PatAl_ETCO2max:
		case AL_PatAl_ETCO2min:
		case AL_PatAl_FICO2max:
		case AL_PatAl_FICO2min:
			{
				eTempALimitNumeric=ALIMITNUM_ETCO2;
				//DEBUGMSG(TRUE, (TEXT("RefreshAlarmStates() ALIMITNUM_ETCO2\r\n")));
			}
			break;
		case AL_PatAl_SPO2max:						
		case AL_PatAl_SPO2min:
		case AL_PatAl_PulseRatemax:						
		case AL_PatAl_PulseRatemin:
		case AL_PatAl_SPO2_PImin:
		case AL_PatAl_SPO2_SIQmin:
			{
				eTempALimitNumeric=ALIMITNUM_SPO2;
				//DEBUGMSG(TRUE, (TEXT("RefreshAlarmStates() ALIMITNUM_VENTILATION\r\n")));
			}
			break;
		}

		if(eTempALimitNumeric!=m_eCurALimitNumeric)
		{
			m_eCurALimitNumeric=eTempALimitNumeric;
			getModel()->getCONFIG()->setCurALimitNumeric(m_eCurALimitNumeric);

			DEBUGMSG(TRUE, (TEXT("RefreshAlarmStates set %d\r\n"),(int)eTempALimitNumeric));

			RedrawView();
		}

		switch(ePrimaryAlarm)
		{
		case AL_PatAl_MVmax:
			{
				if(m_pcAlarmLimit_MVhi)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_MVhi->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_MV_HI;
					}
				}
			}
			break;
		case AL_PatAl_MVmin:
			{
				if(m_pcAlarmLimit_MVlow)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_MVlow->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_MV_LO;
					}
				}
			}
			break;
		case AL_PatAl_PIPmax:
			{
				if(m_pcAlarmLimit_PIPmax)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_PIPmax->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_PIPMAX_HI;
					}
				}
			}
			break;
		case AL_PatAl_PIPmin:
			{
				if(m_pcAlarmLimit_PIPmin)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_PIPmin->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_PIPMIN_LO;
					}
				}
			}
			break;
		case AL_PatAl_PEEPminLow:
			{
				if(m_pcAlarmLimit_PEEP)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_PEEP->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_PEEP_LO;
					}
				}
			}
			break;
		case AL_PatAl_BPMmax:
			{
				if(m_pcAlarmLimit_BPM)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_BPM->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_BPM_HI;
					}
				}
			}
			break;
		case AL_PatAl_Leakmax:
			{
				if(m_pcAlarmLimit_Leak)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_Leak->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_LEAK_HI;
					}
				}
			}
			break;
		case AL_PatAl_Apnoe:
			{
				if(m_pcAlarmLimit_Apnoe)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_Apnoe->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_APNOE;
					}
				}
			}
			break;
		case AL_PatAl_DCO2max:
			{
				if(m_pcAlarmLimit_DCO2hi)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_DCO2hi->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_DCO2_HI;
					}
				}
			}
			break;
		case AL_PatAl_DCO2min:
			{
				if(m_pcAlarmLimit_DCO2lo)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_DCO2lo->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_DCO2_LO;
					}
				}
			}
			break;
		case AL_PatAl_ETCO2max:
			{
				if(m_pcAlarmLimit_ETCO2hi)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_ETCO2hi->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_ETCO2_HI;
					}
				}
			}
			break;
		case AL_PatAl_ETCO2min:
			{
				if(m_pcAlarmLimit_ETCO2lo)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_ETCO2lo->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_ETCO2_LO;
					}
				}
			}
			break;
		case AL_PatAl_FICO2max:
			{
				if(m_pcAlarmLimit_FICO2hi)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_FICO2hi->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_FICO2_HI;
					}
				}
			}
			break;
		case AL_PatAl_FICO2min:
			{
				if(m_pcAlarmLimit_FICO2lo)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_FICO2lo->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_FICO2_LO;
					}
				}
			}
			break;
		case AL_PatAl_SPO2max:
			{
				if(m_pcAlarmLimit_SPO2hi)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_SPO2hi->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_SPO2_HI;
					}
				}
			}
			break;
		case AL_PatAl_SPO2min:
			{
				if(m_pcAlarmLimit_SPO2lo)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_SPO2lo->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_SPO2_LO;
					}
				}
			}
			break;
		case AL_PatAl_PulseRatemax:
			{
				if(m_pcAlarmLimit_PulseRatehi)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_PulseRatehi->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_PULSERATE_HI;
					}
				}
			}
			break;
		case AL_PatAl_PulseRatemin:
			{
				if(m_pcAlarmLimit_PulseRatelo)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_PulseRatelo->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_PULSERATE_LO;
					}
				}
			}
			break;
		case AL_PatAl_SPO2_PImin:
			{
				if(m_pcAlarmLimit_SPO2_PIlo)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_SPO2_PIlo->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_SPO2_PI_LO;
					}
				}
			}
			break;
		case AL_PatAl_SPO2_SIQmin:
			{
				if(m_pcAlarmLimit_SPO2_SIQ)
				{
					if(!IsButtonDepressed() && !m_bAlarmSet)
					{
						if(bViewOwnsFocus)
							m_pcAlarmLimit_SPO2_SIQ->SetFocus();
						m_iCurPara=IDC_BTN_ALARM_SPO2_SIQ;
					}
				}
			}
			break;
		}

		m_bAlarmSet=true;
	}
	else
	{
		m_bAlarmSet=false;
		PostMessage(WM_SET_ALIMITTIMER);
	}

}

// **************************************************************************
// 
// **************************************************************************
void CViewAlarmLimit::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==ALIMITTIMER)
	{
		KillTimer(ALIMITTIMER);

		if(		getModel()->getALARMHANDLER()->isAlarmType(AT_PATIENT)==false
			&&	getModel()->getALARMHANDLER()->isAlarmType(AT_DISCON_TUBUS)==false)
		{
			if(!m_bExit)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_NUMERIC);
			}
		}
	}

	CWnd::OnTimer(nIDEvent);
}

//************************************
// Method:    OnDestroy
// FullName:  CViewAlarmLimit::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CViewAlarmLimit::OnDestroy()
{
	m_bExit=true;

	KillTimer(ALIMITTIMER);

	if(m_pWndMenuAlarmLimits)
	{
		m_pWndMenuAlarmLimits->DestroyWindow();
		
	}

	CMVView::OnDestroy();

	if(m_hdcStatic)
		DeleteDC(m_hdcStatic);

	if(m_hbmpStatic)
		DeleteObject(m_hbmpStatic);
}

void CViewAlarmLimit::DrawAutoState()
{
	DrawData(true, true);
}


LRESULT CViewAlarmLimit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	//int iID = 0;
	switch(message)
	{
	case WM_SETVIEWFOCUS:
		{
			//rku cs1
			SetViewFocus();
			return 1;
		}
		break;
	case WM_SETNEXTFOCUS:
		{
			//rku cs1
			SetNextFocus();
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
	case WM_MENUBUTTON_VKUP:
		{
			if(wParam==IDC_BTN_ALIMIT_DELAY)
			{
				if(m_pbtnValueDelay->GetState()==BS_DOWN)
				{
					if(m_iValueDelay<MAX_TIME_PMINDELAY)
					{
						m_iValueDelay+=STEP_TIME_PMINDELAY;

						CStringW strVal;

						strVal.Format(_T("%d"),m_iValueDelay);
						m_pbtnValueDelay->RefreshText(strVal);
					}
					else
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
					}
				}
				else if(m_pbtnValueDelay->GetState()==BS_FOCUSED)
				{
					SetNextFocus();
				}
				else if(m_pbtnValueDelay->GetState()==BS_UP)
				{
					if(!m_bExit)
						SetViewFocus();
				}
			}
		}
		break;
	case WM_MENUBUTTON_VKDOWN:
		{
			if(wParam==IDC_BTN_ALIMIT_DELAY)
			{
				if(m_pbtnValueDelay->GetState()==BS_DOWN)
				{
					if(m_iValueDelay>MIN_TIME_PMINDELAY)
					{
						m_iValueDelay-=STEP_TIME_PMINDELAY;

						CStringW strVal;

						strVal.Format(_T("%d"),m_iValueDelay);
						m_pbtnValueDelay->RefreshText(strVal);
					}
					else
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_ENDRANGE);
					}
				}
				else if(m_pbtnValueDelay->GetState()==BS_FOCUSED)
				{
					SetPrevFocus();
				}
				else if(m_pbtnValueDelay->GetState()==BS_UP)
				{
					if(!m_bExit)
						SetViewFocus();
				}
			}
		}
		break;
	case WM_MENUBUTTONUP:
		{
			switch(wParam)
			{
			case IDC_BTN_ALIMIT_DELAY:
				{
					return 1;
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case WM_EV_ALIMIT_NUMERIC:
		{
			RedrawView();
			return 1;
		}
		break;
	case WM_ALARMBTN_SETFOCUS:
		{
			m_iCurPara=(int)wParam;
			//DEBUGMSG(TRUE, (TEXT("WM_ALARMBTN_SETFOCUS %d\r\n"),m_iCurPara));
			return 1;
		}
		break;
	case WM_PEEP_LO_CHANGED:
		{
			if(!m_bExit)
			{
				if(m_pcAlarmLimit_PIPmax)
					m_pcAlarmLimit_PIPmax->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePIPmax(),
					getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmax());

				if(m_pcAlarmLimit_PIPmin)
					m_pcAlarmLimit_PIPmin->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePIPmin(),
					getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmin());
			}
			return 1;
		}
		break;
	case WM_PMAX_HI_CHANGED:
		{
			if(!m_bExit)
			{
				if(m_pcAlarmLimit_PEEP)
					m_pcAlarmLimit_PEEP->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePEEPmin(),
					getModel()->getALARMHANDLER()->getAlimitMaxRangePEEPmin());
			}
			return 1;
		}
		break;
	case WM_PMAX_LO_CHANGED:
		{
			if(!m_bExit)
			{
				if(m_pcAlarmLimit_PEEP)
					m_pcAlarmLimit_PEEP->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePEEPmin(),
					getModel()->getALARMHANDLER()->getAlimitMaxRangePEEPmin());
			}
			return 1;
		}
		break;
	case WM_SET_ALIMITTIMER:
		{
			KillTimer(ALIMITTIMER);
			SetTimer(ALIMITTIMER, TIMEMODECHANGE, NULL);
			return 1;
		}
		break;
	case WM_SETAUTOSTATE:
		{
			if(!m_bExit)
				DrawAutoState();
			return 1;
		}
		break;
	case WM_SETNOAUTOSTATE:
		{
			if(!m_bExit)
				DrawAutoState();
		}
		break;
	case WM_CALCULATE_ALARMLIMIT:
		{
			if(!m_bExit)
				CalculateAlarmLimit();
			return 1;
		}
		break;
	case WM_ALIMIT_CHANGED:
		{
			if(!m_bExit)
				RedrawAlarmLimits();
			return 1;
		}
		break;
	case WM_EXSPIRATION:
		{
			if(!m_bExit)
				DrawData(false, false);
			return 1;
		}
		break;
	case WM_DATA_CHANGED:
		{
			if(!m_bExit)
				DrawData(false, false);
			return 1;
		}
		break;
	case WM_ALIMIT_AUTOSET_SINGLE:
		{
			switch(m_iCurPara)
			{
			case IDC_BTN_ALARM_MV_HI:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_MVMAX);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_MV_LO:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_MVMIN);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_PIPMAX_HI:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_PIPMAX);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_PIPMIN_LO:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_PIPMIN);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_PEEP_LO:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_PEEP);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_BPM_HI:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_BPM);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_LEAK_HI:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_LEAK);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_APNOE:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_APNOE);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_DCO2_HI:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_DCO2MAX);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_DCO2_LO:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_DCO2MIN);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_ETCO2_HI:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_ETCO2MAX);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_ETCO2_LO:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_ETCO2MIN);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_FICO2_HI:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_FICO2MAX);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_FICO2_LO:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_FICO2MIN);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_SPO2_HI:
				{
					//getModel()->getALARMHANDLER()->setAlimitSPO2max(1000);
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_SPO2MAX);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_SPO2_LO:
				{
					//getModel()->getALARMHANDLER()->setAlimitSPO2min(0);
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_SPO2MIN);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_PULSERATE_HI:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_PULSERATEMAX);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_PULSERATE_LO:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_PULSERATEMIN);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_SPO2_PI_LO:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_SPO2_PIMIN);
					return 1;
				}
				break;
			case IDC_BTN_ALARM_SPO2_SIQ:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_SINGLE_AUTOLIMIT,AP_SPO2_SIQMIN);
					return 1;
				}
				break;
			default:
				break;
			}
		}
		break;
	case WM_ALIMIT_SETFOCUS:
		{
			if(!m_bExit)
				SetViewFocus();
			return 1;
		}
		break;
	case WM_ALIMIT_ALARM_OFF:
		 {
			 if(!m_bExit)
				 RefreshAlarmStates();
			 return 1;
		 }
		 break;
	case WM_ALIMIT_STATE_DCO2_HI_CHANGED:
		{
			if(!m_bExit)
			{
				if(m_pcAlarmLimit_DCO2lo)
				{
					m_pcAlarmLimit_DCO2lo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_DCO2minLimit());
					//m_pcAlarmLimit_DCO2lo->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitDCO2min(),true);
					m_pcAlarmLimit_DCO2lo->SetCurLimit(getModel()->getDATAHANDLER()->calculateAutoAlarmlimitDCO2min(),true);
				}
			}
			return 1;
		}
		break;
	case WM_ALIMIT_STATE_DCO2_LO_CHANGED:
		{
			if(!m_bExit)
			{
				//RedrawAlarmLimits();
				if(m_pcAlarmLimit_DCO2hi)
				{
					m_pcAlarmLimit_DCO2hi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_DCO2maxLimit());
					//m_pcAlarmLimit_DCO2hi->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitDCO2max(),true);
					m_pcAlarmLimit_DCO2hi->SetCurLimit(getModel()->getDATAHANDLER()->calculateAutoAlarmlimitDCO2max(),true);
				}
			}
			return 1;
		}
		break;
	//case WM_ALIMIT_STATE_ETCO2_HI_CHANGED:
	//	{
	//		if(!m_bExit)
	//		{
	//			if(m_pcAlarmLimit_DCO2lo)
	//			{
	//				m_pcAlarmLimit_DCO2lo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_DCO2minLimit());
	//				//m_pcAlarmLimit_DCO2lo->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitDCO2min(),true);
	//				m_pcAlarmLimit_DCO2lo->SetCurLimit(getModel()->getDATAHANDLER()->CalculateAutoAlarmlimitDCO2min(),true);
	//			}
	//		}
	//		return 1;
	//	}
	//	break;
	//case WM_ALIMIT_STATE_ETCO2_LO_CHANGED:
	//	{
	//		if(!m_bExit)
	//		{
	//			//RedrawAlarmLimits();
	//			if(m_pcAlarmLimit_DCO2hi)
	//			{
	//				m_pcAlarmLimit_DCO2hi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_DCO2maxLimit());
	//				//m_pcAlarmLimit_DCO2hi->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitDCO2max(),true);
	//				m_pcAlarmLimit_DCO2hi->SetCurLimit(getModel()->getDATAHANDLER()->CalculateAutoAlarmlimitDCO2max(),true);
	//			}
	//		}
	//		return 1;
	//	}
	//	break;
	//case WM_ALIMIT_STATE_SPO2_HI_CHANGED:
	//	{
	//		if(!m_bExit)
	//		{
	//			if(m_pcAlarmLimit_DCO2lo)
	//			{
	//				m_pcAlarmLimit_DCO2lo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_DCO2minLimit());
	//				//m_pcAlarmLimit_DCO2lo->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitDCO2min(),true);
	//				m_pcAlarmLimit_DCO2lo->SetCurLimit(getModel()->getDATAHANDLER()->CalculateAutoAlarmlimitDCO2min(),true);
	//			}
	//		}
	//		return 1;
	//	}
	//	break;
	//case WM_ALIMIT_STATE_SPO2_LO_CHANGED:
	//	{
	//		if(!m_bExit)
	//		{
	//			//RedrawAlarmLimits();
	//			if(m_pcAlarmLimit_SPO2hi)
	//			{
	//				m_pcAlarmLimit_SPO2hi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_DCO2maxLimit());
	//				//m_pcAlarmLimit_DCO2hi->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitDCO2max(),true);
	//				m_pcAlarmLimit_SPO2hi->SetCurLimit(getModel()->getDATAHANDLER()->CalculateAutoAlarmlimitDCO2max(),true);
	//			}
	//		}
	//		return 1;
	//	}
	//	break;
	//case WM_ALIMIT_STATE_PULSERATE_HI_CHANGED:
	//	{
	//		if(!m_bExit)
	//		{
	//			if(m_pcAlarmLimit_DCO2lo)
	//			{
	//				m_pcAlarmLimit_DCO2lo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_DCO2minLimit());
	//				//m_pcAlarmLimit_DCO2lo->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitDCO2min(),true);
	//				m_pcAlarmLimit_DCO2lo->SetCurLimit(getModel()->getDATAHANDLER()->CalculateAutoAlarmlimitDCO2min(),true);
	//			}
	//		}
	//		return 1;
	//	}
	//	break;
	//case WM_ALIMIT_STATE_PULSERATE_LO_CHANGED:
	//	{
	//		if(!m_bExit)
	//		{
	//			//RedrawAlarmLimits();
	//			if(m_pcAlarmLimit_DCO2hi)
	//			{
	//				m_pcAlarmLimit_DCO2hi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_DCO2maxLimit());
	//				//m_pcAlarmLimit_DCO2hi->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitDCO2max(),true);
	//				m_pcAlarmLimit_DCO2hi->SetCurLimit(getModel()->getDATAHANDLER()->CalculateAutoAlarmlimitDCO2max(),true);
	//			}
	//		}
	//		return 1;
	//	}
	//	break;
	//case WM_ALIMIT_STATE_SPO2_PI_LO_CHANGED:
	//	{
	//		if(!m_bExit)
	//		{
	//			if(m_pcAlarmLimit_SPO2_PIlo)
	//			{
	//				m_pcAlarmLimit_SPO2_PIlo->SetAlarmLimitState(getModel()->getALARMHANDLER()->GetAlimitStateSPO2_PImax());
	//				m_pcAlarmLimit_SPO2_PIlo->SetCurLimit(getModel()->getDATAHANDLER()->CalculateAutoAlarmlimitSPO2_PImax(),true);
	//			}
	//		}
	//		return 1;
	//	}
	//	break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}


void CViewAlarmLimit::NotifyExspirationDataChanged()
{
	if(m_bExit)
		return;

	try
	{
		PostMessage(WM_EXSPIRATION);
	}
	catch (...)
	{
		theApp.ReportException(_T("CViewAlarmLimit::NotifyExspirationDataChanged"));
	}
}

void CViewAlarmLimit::NotifyDataChanged()
{
	if(m_bExit)
		return;

	try
	{
		PostMessage(WM_DATA_CHANGED);
	}
	catch (...)
	{
		theApp.ReportException(_T("CViewAlarmLimit::NotifyDataChanged"));
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewAlarmLimit::RedrawAlarmLimits()
{
	if(m_bExit)
		return;

	if(m_pcAlarmLimit_MVhi)
	{
		m_pcAlarmLimit_MVhi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_MVmaxLimit());
		m_pcAlarmLimit_MVhi->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitMVmax(),true);
	}
	if(m_pcAlarmLimit_MVlow)
	{
		m_pcAlarmLimit_MVlow->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_MVminLimit());
		m_pcAlarmLimit_MVlow->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitMVmin(),true);
	}
	if(m_pcAlarmLimit_PIPmax)
	{
		m_pcAlarmLimit_PIPmax->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePIPmax(),
			getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmax());
		m_pcAlarmLimit_PIPmax->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit());
		m_pcAlarmLimit_PIPmax->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPIPmax(),true);
	}
	if(m_pcAlarmLimit_PIPmin)
	{
		m_pcAlarmLimit_PIPmin->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePIPmin(),
			getModel()->getALARMHANDLER()->getAlimitMaxRangePIPmin());
		m_pcAlarmLimit_PIPmin->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit());
		m_pcAlarmLimit_PIPmin->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPIPmin(),true);
	}
	if(m_pcAlarmLimit_PEEP)
	{
		m_pcAlarmLimit_PEEP->SetAbsoluteLimits(getModel()->getALARMHANDLER()->getAlimitMinRangePEEPmin(),
			getModel()->getALARMHANDLER()->getAlimitMaxRangePEEPmin());
		m_pcAlarmLimit_PEEP->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit());
		m_pcAlarmLimit_PEEP->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPEEPmin(),true);
	}
	if(m_pcAlarmLimit_BPM)
	{
		m_pcAlarmLimit_BPM->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_BPMmaxLimit());
		m_pcAlarmLimit_BPM->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitBPMmax(),true);
	}
	if(m_pcAlarmLimit_Leak)
	{
		m_pcAlarmLimit_Leak->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_LeakmaxLimit());
		m_pcAlarmLimit_Leak->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitLeakmax(),true);
	}
	if(m_pcAlarmLimit_Apnoe)
	{
		m_pcAlarmLimit_Apnoe->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit());
		m_pcAlarmLimit_Apnoe->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitApnoe(),true);
	}
	if(m_pcAlarmLimit_DCO2hi)
	{
		m_pcAlarmLimit_DCO2hi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_DCO2maxLimit());
		m_pcAlarmLimit_DCO2hi->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitDCO2max(),true);
	}
	if(m_pcAlarmLimit_DCO2lo)
	{
		m_pcAlarmLimit_DCO2lo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_DCO2minLimit());
		m_pcAlarmLimit_DCO2lo->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitDCO2min(),true);
	}
	if(m_pcAlarmLimit_ETCO2hi)
	{
		m_pcAlarmLimit_ETCO2hi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_ETCO2maxLimit());
		m_pcAlarmLimit_ETCO2hi->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitETCO2max(),true);
	}
	if(m_pcAlarmLimit_ETCO2lo)
	{
		m_pcAlarmLimit_ETCO2lo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_ETCO2minLimit());
		m_pcAlarmLimit_ETCO2lo->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitETCO2min(),true);
	}

	if(m_pcAlarmLimit_FICO2hi)
	{
		m_pcAlarmLimit_FICO2hi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_FICO2maxLimit());
		m_pcAlarmLimit_FICO2hi->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitFICO2max(),true);
	}
	if(m_pcAlarmLimit_FICO2lo)
	{
		m_pcAlarmLimit_FICO2lo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_FICO2minLimit());
		m_pcAlarmLimit_FICO2lo->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitFICO2min(),true);
	}

	if(m_pcAlarmLimit_SPO2hi)
	{
		m_pcAlarmLimit_SPO2hi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_SPO2maxLimit());
		m_pcAlarmLimit_SPO2hi->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitSPO2max(),true);
	}
	if(m_pcAlarmLimit_SPO2lo)
	{
		m_pcAlarmLimit_SPO2lo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_SPO2minLimit());
		m_pcAlarmLimit_SPO2lo->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitSPO2min(),true);
	}
	if(m_pcAlarmLimit_PulseRatehi)
	{
		m_pcAlarmLimit_PulseRatehi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_PulseRatemaxLimit());
		m_pcAlarmLimit_PulseRatehi->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPulseRatemax(),true);
	}
	if(m_pcAlarmLimit_PulseRatelo)
	{
		m_pcAlarmLimit_PulseRatelo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_PulseRateminLimit());
		m_pcAlarmLimit_PulseRatelo->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitPulseRatemin(),true);
	}
	if(m_pcAlarmLimit_SPO2_PIlo)
	{
		m_pcAlarmLimit_SPO2_PIlo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_SPO2_PIminLimit());
		m_pcAlarmLimit_SPO2_PIlo->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitSPO2_PImin(),true);
	}
	if(m_pcAlarmLimit_SPO2_SIQ)
	{
		m_pcAlarmLimit_SPO2_SIQ->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_SPO2_SIQminLimit());
		m_pcAlarmLimit_SPO2_SIQ->SetCurLimit(getModel()->getALARMHANDLER()->getAlimitSPO2_SIQmin(),true);
	}

	
}

// **************************************************************************
// 
// **************************************************************************
void CViewAlarmLimit::NotifyAlarmLimitChanged()
{
	if(m_bExit)
		return;

	try
	{
		PostMessage(WM_ALIMIT_CHANGED);
	}
	catch (...)
	{
		theApp.ReportException(_T("CViewAlarmLimit::NotifyAlarmLimitChanged"));
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewAlarmLimit::CalculateAlarmLimit()
{
	if(m_bExit)
		return;

	if(m_pcAlarmLimit_MVhi && getModel()->getALARMHANDLER()->getAlimitState_MVmaxLimit()==AL_CALC)
	{
		m_pcAlarmLimit_MVhi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_MVmaxLimit());
		m_pcAlarmLimit_MVhi->Redraw();
	}
	if(m_pcAlarmLimit_MVlow && getModel()->getALARMHANDLER()->getAlimitState_MVminLimit()==AL_CALC)
	{
		m_pcAlarmLimit_MVlow->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_MVminLimit());
		m_pcAlarmLimit_MVlow->Redraw();
	}
	if(m_pcAlarmLimit_PIPmax && getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_CALC)
	{
		m_pcAlarmLimit_PIPmax->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit());
		m_pcAlarmLimit_PIPmax->Redraw();
	}
	if(m_pcAlarmLimit_PIPmin && getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_CALC)
	{
		m_pcAlarmLimit_PIPmin->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit());
		m_pcAlarmLimit_PIPmin->Redraw();
	}
	if(m_pcAlarmLimit_PEEP && getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_CALC)
	{
		m_pcAlarmLimit_PEEP->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit());
		m_pcAlarmLimit_PEEP->Redraw();
	}
	if(m_pcAlarmLimit_BPM && getModel()->getALARMHANDLER()->getAlimitState_BPMmaxLimit()==AL_CALC)
	{
		m_pcAlarmLimit_BPM->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_BPMmaxLimit());
		m_pcAlarmLimit_BPM->Redraw();
	}
	if(m_pcAlarmLimit_Leak && getModel()->getALARMHANDLER()->getAlimitState_LeakmaxLimit()==AL_CALC)
	{
		m_pcAlarmLimit_Leak->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_LeakmaxLimit());
		m_pcAlarmLimit_Leak->Redraw();
	}
	if(m_pcAlarmLimit_Apnoe && getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit()==AL_CALC)
	{
		m_pcAlarmLimit_Apnoe->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_ApnoeLimit());
		m_pcAlarmLimit_Apnoe->Redraw();
	}
	if(m_pcAlarmLimit_DCO2hi && getModel()->getALARMHANDLER()->getAlimitState_DCO2maxLimit()==AL_CALC)
	{
		m_pcAlarmLimit_DCO2hi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_DCO2maxLimit());
		m_pcAlarmLimit_DCO2hi->Redraw();
	}
	if(m_pcAlarmLimit_DCO2lo && getModel()->getALARMHANDLER()->getAlimitState_DCO2minLimit()==AL_CALC)
	{
		m_pcAlarmLimit_DCO2lo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_DCO2minLimit());
		m_pcAlarmLimit_DCO2lo->Redraw();
	}
	if(m_pcAlarmLimit_ETCO2hi && getModel()->getALARMHANDLER()->getAlimitState_ETCO2maxLimit()==AL_CALC)
	{
		m_pcAlarmLimit_ETCO2hi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_ETCO2maxLimit());
		m_pcAlarmLimit_ETCO2hi->Redraw();
	}
	if(m_pcAlarmLimit_ETCO2lo && getModel()->getALARMHANDLER()->getAlimitState_ETCO2minLimit()==AL_CALC)
	{
		m_pcAlarmLimit_ETCO2lo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_ETCO2minLimit());
		m_pcAlarmLimit_ETCO2lo->Redraw();
	}

	if(m_pcAlarmLimit_FICO2hi && getModel()->getALARMHANDLER()->getAlimitState_FICO2maxLimit()==AL_CALC)
	{
		m_pcAlarmLimit_FICO2hi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_FICO2maxLimit());
		m_pcAlarmLimit_FICO2hi->Redraw();
	}
	if(m_pcAlarmLimit_FICO2lo && getModel()->getALARMHANDLER()->getAlimitState_FICO2minLimit()==AL_CALC)
	{
		m_pcAlarmLimit_FICO2lo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_FICO2minLimit());
		m_pcAlarmLimit_FICO2lo->Redraw();
	}

	if(m_pcAlarmLimit_SPO2hi && getModel()->getALARMHANDLER()->getAlimitState_SPO2maxLimit()==AL_CALC)
	{
		m_pcAlarmLimit_SPO2hi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_SPO2maxLimit());
		m_pcAlarmLimit_SPO2hi->Redraw();
	}
	if(m_pcAlarmLimit_SPO2lo && getModel()->getALARMHANDLER()->getAlimitState_SPO2minLimit()==AL_CALC)
	{
		m_pcAlarmLimit_SPO2lo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_SPO2minLimit());
		m_pcAlarmLimit_SPO2lo->Redraw();
	}
	if(m_pcAlarmLimit_PulseRatehi && getModel()->getALARMHANDLER()->getAlimitState_PulseRatemaxLimit()==AL_CALC)
	{
		m_pcAlarmLimit_PulseRatehi->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_PulseRatemaxLimit());
		m_pcAlarmLimit_PulseRatehi->Redraw();
	}
	if(m_pcAlarmLimit_PulseRatelo && getModel()->getALARMHANDLER()->getAlimitState_PulseRateminLimit()==AL_CALC)
	{
		m_pcAlarmLimit_PulseRatelo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_PulseRateminLimit());
		m_pcAlarmLimit_PulseRatelo->Redraw();
	}
	if(m_pcAlarmLimit_SPO2_PIlo && getModel()->getALARMHANDLER()->getAlimitState_SPO2_PIminLimit()==AL_CALC)
	{
		m_pcAlarmLimit_SPO2_PIlo->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_SPO2_PIminLimit());
		m_pcAlarmLimit_SPO2_PIlo->Redraw();
	}
	if(m_pcAlarmLimit_SPO2_SIQ && getModel()->getALARMHANDLER()->getAlimitState_SPO2_SIQminLimit()==AL_CALC)
	{
		m_pcAlarmLimit_SPO2_SIQ->SetAlarmLimitState(getModel()->getALARMHANDLER()->getAlimitState_SPO2_SIQminLimit());
		m_pcAlarmLimit_SPO2_SIQ->Redraw();
	}

}

// **************************************************************************
// 
// **************************************************************************
void CViewAlarmLimit::NotifyCalculateAlarmLimit()
{
	if(m_bExit)
		return;

	try
	{
		PostMessage(WM_CALCULATE_ALARMLIMIT);
	}
	catch (...)
	{
		theApp.ReportException(_T("CViewAlarmLimit::NotifyCalculateAlarmLimit"));
	}
}



//void CViewAlarmLimit::OnKillFocus(CWnd* pNewWnd)
//{
//	//int iTemp = pNewWnd->GetDlgCtrlID();
//
//	CWnd::OnKillFocus(pNewWnd);
//}

void CViewAlarmLimit::checkAlarmState()
{
	if(m_bExit)
		return;

	if(getModel()->getALARMHANDLER()->isActiveAlarm()==true && getModel()->getALARMHANDLER()->getAlarmSilentState()==ASTATE_ACTIVE)
	{
		if(	getModel()->getALARMHANDLER()->isActiveAlarmType(AT_PATIENT)==true)
		{
			eAlarm curAlarm=getModel()->getALARMHANDLER()->getActiveAlarm();

			switch(curAlarm)
			{
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
				{
					m_eCurALimitNumeric=ALIMITNUM_VENTILATION;
					getModel()->getCONFIG()->setCurALimitNumeric(ALIMITNUM_VENTILATION);
				}
				break;
			case AL_PatAl_ETCO2max:  
			case AL_PatAl_ETCO2min:
			case AL_PatAl_FICO2max:  
			case AL_PatAl_FICO2min:
				{
					m_eCurALimitNumeric=ALIMITNUM_ETCO2;
					getModel()->getCONFIG()->setCurALimitNumeric(ALIMITNUM_ETCO2);
				}
				break;
			case AL_PatAl_SPO2max:
			case AL_PatAl_SPO2min:
			case AL_PatAl_PulseRatemax:
			case AL_PatAl_PulseRatemin:
			case AL_PatAl_SPO2_PImin:
				{
					m_eCurALimitNumeric=ALIMITNUM_SPO2;
					getModel()->getCONFIG()->setCurALimitNumeric(ALIMITNUM_SPO2);
				}
				break;
			}
		}
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewAlarmLimit::SetViewFocus()
{
	if(m_bExit)
		return;

	if(getModel()->getALARMHANDLER()->isActiveAlarm()==false || getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_ACTIVE)
		m_iCurPara=getModel()->getCONFIG()->GetLastSelectedALBtns();
	/*else
		getModel()->getCONFIG()->SetLastSelectedALBtns(m_iCurPara);*/

	//DEBUGMSG(TRUE, (TEXT("CViewAlarmLimit::SetViewFocus() %d\r\n"),m_iCurPara));

	bool bFocusLost=false;
	
	switch(m_iCurPara)
	{
	case IDC_BTN_ALIMIT_DELAY:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2hi && m_pcAlarmLimit_SPO2hi->IsWindowVisible())
					m_pcAlarmLimit_SPO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pbtnValueDelay && m_pbtnValueDelay->IsWindowVisible())
					m_pbtnValueDelay->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;
			}
			
		}
		break;
	case IDC_BTN_ALARM_MV_HI:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2hi && m_pcAlarmLimit_SPO2hi->IsWindowVisible())
					m_pcAlarmLimit_SPO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_MVhi && m_pcAlarmLimit_MVhi->IsWindowVisible())
					m_pcAlarmLimit_MVhi->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;
			}
		}
		break;
	case IDC_BTN_ALARM_MV_LO:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2hi && m_pcAlarmLimit_SPO2hi->IsWindowVisible())
					m_pcAlarmLimit_SPO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_MVlow && m_pcAlarmLimit_MVlow->IsWindowVisible())
					m_pcAlarmLimit_MVlow->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;
			}
		}
		break;
	case IDC_BTN_ALARM_PIPMAX_HI:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2hi && m_pcAlarmLimit_SPO2hi->IsWindowVisible())
					m_pcAlarmLimit_SPO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;
			}

		}
		break;
	case IDC_BTN_ALARM_PIPMIN_LO:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2hi && m_pcAlarmLimit_SPO2hi->IsWindowVisible())
					m_pcAlarmLimit_SPO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_PIPmin && m_pcAlarmLimit_PIPmin->IsWindowVisible())
					m_pcAlarmLimit_PIPmin->SetFocus();
				else
					bFocusLost=true;
			}

		}
		break;
	case IDC_BTN_ALARM_PEEP_LO:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2hi && m_pcAlarmLimit_SPO2hi->IsWindowVisible())
					m_pcAlarmLimit_SPO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_PEEP && m_pcAlarmLimit_PEEP->IsWindowVisible())
					m_pcAlarmLimit_PEEP->SetFocus();
				else
					bFocusLost=true;
			}
		}
		break;
	case IDC_BTN_ALARM_BPM_HI:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2hi && m_pcAlarmLimit_SPO2hi->IsWindowVisible())
					m_pcAlarmLimit_SPO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==true && m_pcAlarmLimit_BPM && m_pcAlarmLimit_BPM->IsWindowVisible())
				{
					m_pcAlarmLimit_BPM->SetFocus();
				}
				else if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_BPM && m_pcAlarmLimit_BPM->IsWindowVisible())
					m_pcAlarmLimit_BPM->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;
			}
		}
		break;
	case IDC_BTN_ALARM_LEAK_HI:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2hi && m_pcAlarmLimit_SPO2hi->IsWindowVisible())
					m_pcAlarmLimit_SPO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_Leak && m_pcAlarmLimit_Leak->IsWindowVisible())
					m_pcAlarmLimit_Leak->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;
			}
		}
		break;
	case IDC_BTN_ALARM_APNOE:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2hi && m_pcAlarmLimit_SPO2hi->IsWindowVisible())
					m_pcAlarmLimit_SPO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_Apnoe && m_pcAlarmLimit_Apnoe->IsWindowVisible())
					m_pcAlarmLimit_Apnoe->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;
			}
		}
		break;
	case IDC_BTN_ALARM_DCO2_HI:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2hi && m_pcAlarmLimit_SPO2hi->IsWindowVisible())
					m_pcAlarmLimit_SPO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_DCO2hi && m_pcAlarmLimit_DCO2hi->IsWindowVisible())
					m_pcAlarmLimit_DCO2hi->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;
			}

		}
		break;
	case IDC_BTN_ALARM_DCO2_LO:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2hi && m_pcAlarmLimit_SPO2hi->IsWindowVisible())
					m_pcAlarmLimit_SPO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_DCO2lo && m_pcAlarmLimit_DCO2lo->IsWindowVisible())
					m_pcAlarmLimit_DCO2lo->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;
			}
		}
		break;
	case IDC_BTN_ALARM_ETCO2_HI:
		{

			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2hi && m_pcAlarmLimit_SPO2hi->IsWindowVisible())
					m_pcAlarmLimit_SPO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_MVhi && m_pcAlarmLimit_MVhi->IsWindowVisible())
					m_pcAlarmLimit_MVhi->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;
			}
		}
		break;
	case IDC_BTN_ALARM_ETCO2_LO:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2hi && m_pcAlarmLimit_SPO2hi->IsWindowVisible())
					m_pcAlarmLimit_SPO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2lo && m_pcAlarmLimit_ETCO2lo->IsWindowVisible())
					m_pcAlarmLimit_ETCO2lo->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_MVhi && m_pcAlarmLimit_MVhi->IsWindowVisible())
					m_pcAlarmLimit_MVhi->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;
			}
		}
		break;
	case IDC_BTN_ALARM_FICO2_HI:
		{

			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2hi && m_pcAlarmLimit_SPO2hi->IsWindowVisible())
					m_pcAlarmLimit_SPO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_FICO2hi && m_pcAlarmLimit_FICO2hi->IsWindowVisible())
					m_pcAlarmLimit_FICO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_MVhi && m_pcAlarmLimit_MVhi->IsWindowVisible())
					m_pcAlarmLimit_MVhi->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;
			}
		}
		break;
	case IDC_BTN_ALARM_FICO2_LO:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2hi && m_pcAlarmLimit_SPO2hi->IsWindowVisible())
					m_pcAlarmLimit_SPO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_FICO2lo && m_pcAlarmLimit_FICO2lo->IsWindowVisible())
					m_pcAlarmLimit_FICO2lo->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_MVhi && m_pcAlarmLimit_MVhi->IsWindowVisible())
					m_pcAlarmLimit_MVhi->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;
			}
		}
		break;
	case IDC_BTN_ALARM_SPO2_HI:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2hi && m_pcAlarmLimit_SPO2hi->IsWindowVisible())
					m_pcAlarmLimit_SPO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_MVhi && m_pcAlarmLimit_MVhi->IsWindowVisible())
					m_pcAlarmLimit_MVhi->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;
			}
		}
		break;
	case IDC_BTN_ALARM_SPO2_LO:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2lo && m_pcAlarmLimit_SPO2lo->IsWindowVisible())
					m_pcAlarmLimit_SPO2lo->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_MVhi && m_pcAlarmLimit_MVhi->IsWindowVisible())
					m_pcAlarmLimit_MVhi->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;
			}
		}
		break;
	case IDC_BTN_ALARM_PULSERATE_HI:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_PulseRatehi && m_pcAlarmLimit_PulseRatehi->IsWindowVisible())
					m_pcAlarmLimit_PulseRatehi->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_MVhi && m_pcAlarmLimit_MVhi->IsWindowVisible())
					m_pcAlarmLimit_MVhi->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;
			}
		}
		break;
	case IDC_BTN_ALARM_PULSERATE_LO:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_PulseRatelo && m_pcAlarmLimit_PulseRatelo->IsWindowVisible())
					m_pcAlarmLimit_PulseRatelo->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_MVhi && m_pcAlarmLimit_MVhi->IsWindowVisible())
					m_pcAlarmLimit_MVhi->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;
			}
		}
		break;
	case IDC_BTN_ALARM_SPO2_PI_LO:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2_PIlo && m_pcAlarmLimit_SPO2_PIlo->IsWindowVisible())
					m_pcAlarmLimit_SPO2_PIlo->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_MVhi && m_pcAlarmLimit_MVhi->IsWindowVisible())
					m_pcAlarmLimit_MVhi->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;
				
			}
		}
		break;
	case IDC_BTN_ALARM_SPO2_SIQ:
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2_SIQ && m_pcAlarmLimit_SPO2_SIQ->IsWindowVisible())
					m_pcAlarmLimit_SPO2_SIQ->SetFocus();
				else
					bFocusLost=true;
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
				else
					bFocusLost=true;
			}
			else
			{
				if(m_pcAlarmLimit_MVhi && m_pcAlarmLimit_MVhi->IsWindowVisible())
					m_pcAlarmLimit_MVhi->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
				else
					bFocusLost=true;

			}
		}
		break;
	default:
		{
			
			bFocusLost=true;
		}
		break;
	}

	if(bFocusLost)
	{
		if(getModel()->getALARMHANDLER()->isActiveAlarmType(AT_PATIENT))
		{
			RefreshAlarmStates();
		}
		else
		{
			if(m_eCurALimitNumeric==ALIMITNUM_SPO2)
			{
				if(m_pcAlarmLimit_SPO2lo && m_pcAlarmLimit_SPO2lo->IsWindowVisible())
					m_pcAlarmLimit_SPO2lo->SetFocus();
			}
			else if(m_eCurALimitNumeric==ALIMITNUM_ETCO2)
			{
				if(m_pcAlarmLimit_ETCO2hi && m_pcAlarmLimit_ETCO2hi->IsWindowVisible())
					m_pcAlarmLimit_ETCO2hi->SetFocus();
			}
			else
			{
				if(m_pcAlarmLimit_MVhi && m_pcAlarmLimit_MVhi->IsWindowVisible())
					m_pcAlarmLimit_MVhi->SetFocus();
				else if(m_pcAlarmLimit_PIPmax && m_pcAlarmLimit_PIPmax->IsWindowVisible())
					m_pcAlarmLimit_PIPmax->SetFocus();
			}
		}
	}

	PostMessage(WM_SET_ALIMITTIMER);

}

bool CViewAlarmLimit::SetNextFocus()
{
	//DEBUGMSG(TRUE, (TEXT("CViewAlarmLimit::SetNextFocus()\r\n")));

	if(m_bExit)
		return false;

	CWnd* pFocWnd = CWnd::GetFocus();
	if(pFocWnd==NULL)
		return false;
	CWnd* pWnd = GetNextDlgTabItem(pFocWnd,TRUE);
	if(pWnd==NULL)
		return false;

	if(pFocWnd->GetDlgCtrlID()<pWnd->GetDlgCtrlID())
		return false;

	int iID=pWnd->GetDlgCtrlID();
	pFocWnd->CheckDlgButton(pFocWnd->GetDlgCtrlID(),BST_UNCHECKED);

	pWnd->SetFocus();
	getModel()->getCONFIG()->SetLastSelectedALBtns(iID);

	return true;
}
bool CViewAlarmLimit::SetPrevFocus()
{
	//DEBUGMSG(TRUE, (TEXT("CViewAlarmLimit::SetPrevFocus()\r\n")));

	if(m_bExit)
		return false;

	CWnd* pFocWnd = CWnd::GetFocus();
	if(pFocWnd==NULL)
		return false;
	int iID=pFocWnd->GetDlgCtrlID();
	CWnd* pWnd = GetNextDlgTabItem(pFocWnd,FALSE);
	if(pWnd==NULL)
		return false;

	if(		pFocWnd->GetDlgCtrlID()>pWnd->GetDlgCtrlID()
		||	IDC_BTN_ALIMIT_DELAY<pWnd->GetDlgCtrlID())
		return false;
	
	iID=pWnd->GetDlgCtrlID();
	pFocWnd->CheckDlgButton(pFocWnd->GetDlgCtrlID(),BST_UNCHECKED);

	pWnd->SetFocus();
	getModel()->getCONFIG()->SetLastSelectedALBtns(iID);

	return true;
}

//void CViewAlarmLimit::OnSetFocus(CWnd* pOldWnd)
//{
//	CMVView::OnSetFocus(pOldWnd);
//
//	if(pOldWnd!=0)
//	{
//		int iID=pOldWnd->GetDlgCtrlID();
//
//		DEBUGMSG(TRUE, (TEXT("CViewAlarmLimit::OnSetFocus() %d\r\n"),iID));
//	}
//	//DEBUGMSG(TRUE, (TEXT("CViewAlarmLimit::OnSetFocus() \r\n")));
//}

//void CViewAlarmLimit::OnKillFocus(CWnd* pNewWnd)
//{
//	//int iTemp = pNewWnd->GetDlgCtrlID();
//
//	CWnd::OnKillFocus(pNewWnd);
//}

void CViewAlarmLimit::OnBnClickedValue()
{
	if(m_pbtnValueDelay->GetState()==BS_UP)
		m_pbtnValueDelay->SetState(BS_DOWN);
	else if(m_pbtnValueDelay->GetState()==BS_FOCUSED)
		m_pbtnValueDelay->SetState(BS_DOWN);
	else if(m_pbtnValueDelay->GetState()==BS_DOWN)
	{
		m_pbtnValueDelay->SetState(BS_FOCUSED);
		getModel()->getCONFIG()->SetCurPminAlarmDelay(m_iValueDelay);
	}
}