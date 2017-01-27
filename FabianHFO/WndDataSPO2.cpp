// WndDataSPO2.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndDataSPO2.h"

extern CEvent g_eventSPO2Data;

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTSUBBTNDW			0x00000000
#define COLOR_TXTBTNFC				0x00000000
#define COLOR_TXTBTNDIS				0x00828282
// CWndDataSPO2

IMPLEMENT_DYNAMIC(CWndDataSPO2, CWnd)

CWndDataSPO2::CWndDataSPO2()
{
	InitializeCriticalSection(&csDraw);
	InitializeCriticalSection(&csDoThread);
	m_pModel=NULL;

	m_bSpO2Out=false;
	m_bSpO2blink=false;

	//m_bFiO2Out=false;
	//m_bFiO2blink=false;


	m_hdcStatic=NULL;
	m_hbmpStatic=NULL;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;

	m_pcLimitBtn_SPO2low=NULL;
	m_pcLimitBtn_SPO2high=NULL;
	m_pcLimitBtn_FIO2low=NULL;
	m_pcLimitBtn_FIO2high=NULL;

	m_pcLimit_LeUp=NULL;
	m_pcLimit_LeDw=NULL;
	m_pcLimit_RiUp=NULL;
	m_pcLimit_RiDw=NULL;

	m_pcPRICO_OnOff=NULL;
	m_pcPRICO_OnOff_Up=NULL;
	m_pcPRICO_OnOff_Dw=NULL;
	m_pcPRICO_OnOff_Dis=NULL;

	m_pcSPO2ParaLeftTop=NULL;
	m_pcSPO2ParaRightTop=NULL;
	m_pcSPO2ParaLeftBot=NULL;
	m_pcSPO2ParaRightBot=NULL;

	m_pcwtSPO2DataThread=NULL;
	m_hThreadSPO2Data=INVALID_HANDLE_VALUE;
	m_bDoThread=false;

	m_iSPO2=0;
	m_iPulseRate=0;
	m_iPerfusionIndex=0;
	m_iEXCEPTIONS2=0;
	m_iFiO2measuredValue=0;
	m_iFIO2HIGHvalue=0;
	m_iFIO2LOWvalue=0;
	m_iSPO2HIGHvalue=0;
	m_iSPO2LOWvalue=0;

	m_iAlimitSPO2max=0;
	m_iAlimitSPO2min=0;

	m_pcLimitHigh_Up=NULL;
	m_pcLimitLow_Up=NULL;

	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));//rkuNEWFIX
	penBlueVal.CreatePen(PS_SOLID,2,RGB(0,0,255));//rkuNEWFIX
}

CWndDataSPO2::~CWndDataSPO2()
{
	
	m_plLimitBtn.RemoveAll();

	if(m_pcLimitBtn_SPO2low)
		delete m_pcLimitBtn_SPO2low;
	m_pcLimitBtn_SPO2low=NULL;
	if(m_pcLimitBtn_SPO2high)
		delete m_pcLimitBtn_SPO2high;
	m_pcLimitBtn_SPO2high=NULL;
	if(m_pcLimitBtn_FIO2low)
		delete m_pcLimitBtn_FIO2low;
	m_pcLimitBtn_FIO2low=NULL;
	if(m_pcLimitBtn_FIO2high)
		delete m_pcLimitBtn_FIO2high;
	m_pcLimitBtn_FIO2high=NULL;


	if(m_pcLimit_LeUp)
		delete m_pcLimit_LeUp;
	m_pcLimit_LeUp=NULL;
	if(m_pcLimit_LeDw)
		delete m_pcLimit_LeDw;
	m_pcLimit_LeDw=NULL;
	if(m_pcLimit_RiUp)
		delete m_pcLimit_RiUp;
	m_pcLimit_RiUp=NULL;
	if(m_pcLimit_RiDw)
		delete m_pcLimit_RiDw;
	m_pcLimit_RiDw=NULL;

	if(m_pcPRICO_OnOff)
		delete m_pcPRICO_OnOff;
	m_pcPRICO_OnOff=NULL;

	if(m_pcPRICO_OnOff_Up)
		delete m_pcPRICO_OnOff_Up;
	m_pcPRICO_OnOff_Up=NULL;
	if(m_pcPRICO_OnOff_Dw)
		delete m_pcPRICO_OnOff_Dw;
	m_pcPRICO_OnOff_Dw=NULL;
	if(m_pcPRICO_OnOff_Dis)
		delete m_pcPRICO_OnOff_Dis;
	m_pcPRICO_OnOff_Dis=NULL;

	if(m_pcLimitHigh_Up)
		delete m_pcLimitHigh_Up;
	m_pcLimitHigh_Up=NULL;

	if(m_pcLimitLow_Up)
		delete m_pcLimitLow_Up;
	m_pcLimitLow_Up=NULL;

	if(m_pcSPO2ParaLeftTop)
		delete m_pcSPO2ParaLeftTop;
	m_pcSPO2ParaLeftTop=NULL;

	if(m_pcSPO2ParaRightTop)
		delete m_pcSPO2ParaRightTop;
	m_pcSPO2ParaRightTop=NULL;


	if(m_pcSPO2ParaLeftBot)
		delete m_pcSPO2ParaLeftBot;
	m_pcSPO2ParaLeftBot=NULL;

	if(m_pcSPO2ParaRightBot)
		delete m_pcSPO2ParaRightBot;
	m_pcSPO2ParaRightBot=NULL;

	penLine.DeleteObject();//rkuNEWFIX
	penBlueVal.DeleteObject();//rkuNEWFIX

	DeleteCriticalSection(&csDraw);
	DeleteCriticalSection(&csDoThread);
}

CMVModel *CWndDataSPO2::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

BEGIN_MESSAGE_MAP(CWndDataSPO2, CWnd)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CWndDataSPO2 message handlers
// **************************************************************************
// 
// **************************************************************************
BOOL CWndDataSPO2::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
{
	bool bRes=false;
	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD/*|WS_VISIBLE*/,rc,pParentWnd,nID))
	{
		CClientDC dc(this);
		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
		m_hBmpPrev=(HBITMAP)SelectObject(m_hDC,m_hBmp);

		//CBrush cbrBack(RGB(255,0,0));
		CBrush cbrBack(BACKGND);

		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		m_hdcStatic = CreateCompatibleDC(m_hDC);
		m_hbmpStatic = CreateCompatibleBitmap(m_hDC, m_lX, m_lY);
		SelectObject(m_hdcStatic, m_hbmpStatic);
		HPEN hpenprevStat=(HPEN)SelectObject(m_hdcStatic, (HPEN)GetStockObject(NULL_PEN));	
		HBRUSH hbrprevStat=(HBRUSH)SelectObject(m_hdcStatic,cbrBack); 
		Rectangle(m_hdcStatic, 0, 0, m_lX, m_lY);


		m_pcSPO2ParaLeftTop= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SPO2PARA_LE_TOP);
		m_pcSPO2ParaRightTop= new CBmp(theApp.m_hInstance,m_hDC, IDB_SPO2PARA_RI_TOP);

		m_pcSPO2ParaLeftBot= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SPO2PARA_LE_BOT);
		m_pcSPO2ParaRightBot= new CBmp(theApp.m_hInstance,m_hDC, IDB_SPO2PARA_RI_BOT);

		m_pcLimitHigh_Up	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_LIMIT_HIGH);
		m_pcLimitLow_Up		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_LIMIT_LOW);

		m_pcPRICO_OnOff_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_ONOFF_UP);
		m_pcPRICO_OnOff_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_ONOFF_DW);
		m_pcPRICO_OnOff_Dis		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_ONOFF_DIS);


		m_pcLimit_LeUp		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PRICOLIMIT_LE_UP);
		m_pcLimit_LeDw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PRICOLIMIT_LE_DW);
		m_pcLimit_RiUp		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PRICOLIMIT_RI_UP);
		m_pcLimit_RiDw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PRICOLIMIT_RI_DW);


		DWORD dwStyleNoTab = WS_CHILD/*|WS_VISIBLE*/|BS_OWNERDRAW|WS_TABSTOP;
		BTN btn;
		fVALUE fv;

		//-------------------------------------------------------------
		//Parameter Button------PRICO---------------------------------
		btn.wID					= IDC_BTN_PRICO_ONOFF;	
		btn.poPosition.x		= 23;
		btn.poPosition.y		= 108;
		btn.pcBmpUp				= m_pcPRICO_OnOff_Up;
		btn.pcBmpDown			= m_pcPRICO_OnOff_Dw;
		btn.pcBmpFocus			= m_pcPRICO_OnOff_Up;
		btn.pcBmpDisabled		= m_pcPRICO_OnOff_Dis;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcPRICO_OnOff=new CUpDwnBtn(btn,COLOR_TXTBTNUP,false,true);
		m_pcPRICO_OnOff->Create(this,g_hf7AcuBold,0);
		m_pcPRICO_OnOff->SetText(_T(""),_T(""));
		
		if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
		{
			m_pcPRICO_OnOff->ShowWindow(SW_SHOW);
			if(getModel()->getSPO2())
			{
				if(		getModel()->getSPO2()->isStateOk() 
					&& getModel()->getDATAHANDLER()->GetO2SensorState()!=OXYSENS_OFF 
					&& false==getModel()->isO2FlushActive())//PRICO04
				m_pcPRICO_OnOff->EnableWindow(TRUE);
			else
				m_pcPRICO_OnOff->EnableWindow(FALSE);
			}
			else
			{
				m_pcPRICO_OnOff->EnableWindow(FALSE);
			}
			

			if(getModel()->getDATAHANDLER()->getPRICOState()==true)
				m_pcPRICO_OnOff->Depress(true);
			else
				m_pcPRICO_OnOff->Depress(false);

		}
		else
		{
			m_pcPRICO_OnOff->ShowWindow(SW_HIDE);
			//m_pcPRICO_OnOff->EnableWindow(FALSE);
		}

		


		//-------------------------------------------------------------
		eAlarmLimitState limitstateSPO2Max = getModel()->getALARMHANDLER()->getAlimitState_SPO2maxLimit();
		eAlarmLimitState limitstateSPO2Min = getModel()->getALARMHANDLER()->getAlimitState_SPO2minLimit();
		int iAlimitSPO2min=0;
		int iAlimitSPO2max=0;
		if(limitstateSPO2Max==AL_OFF || limitstateSPO2Max==AL_CALC)
			iAlimitSPO2max=100;
		else
			iAlimitSPO2max=CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitSPO2max())/10, 0);
			//iAlimitSPO2max=getModel()->getALARMHANDLER()->getAlimitSPO2max()/10;

		if(limitstateSPO2Min==AL_OFF || limitstateSPO2Min==AL_CALC)
			iAlimitSPO2min=0;
		else
			iAlimitSPO2min=CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitSPO2min())/10, 0);
			//iAlimitSPO2min=getModel()->getALARMHANDLER()->getAlimitSPO2min()/10;
		//-------------------------------------------------------------
		//m_pcLimitBtn_SPO2low
		btn.wID					= IDC_BTN_LIMIT_SPO2LOW;	
		btn.poPosition.x		= 175;
		btn.poPosition.y		= 87;
		btn.pcBmpUp				= m_pcLimit_LeUp;
		btn.pcBmpDown			= m_pcLimit_LeDw;
		btn.pcBmpFocus			= m_pcLimit_LeUp;
		btn.pcBmpDisabled		= m_pcLimit_LeUp;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		fv.iLowerLimit=iAlimitSPO2min;
		fv.iUpperLimit=getModel()->getDATAHANDLER()->getPRICO_SPO2highRange();
		fv.iValue=getModel()->getDATAHANDLER()->getPRICO_SPO2lowRange();
		
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->setPRICO_SPO2lowRange(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->setPRICO_SPO2lowRange(fv.iValue);
		}
		
		m_pcLimitBtn_SPO2low=new CLimitBtn_SPO2low(this,btn,0,false);
		m_pcLimitBtn_SPO2low->Create(this,dwStyleNoTab,fv);
		m_pcLimitBtn_SPO2low->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
		if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
		{
			m_pcLimitBtn_SPO2low->ShowWindow(SW_SHOW);
			m_plLimitBtn.AddTail(m_pcLimitBtn_SPO2low);
		}
		else
		{
			m_pcLimitBtn_SPO2low->ShowWindow(SW_HIDE);
		}

		

		//-------------------------------------------------------------
		//m_pcLimitBtn_SPO2high
		btn.wID					= IDC_BTN_LIMIT_SPO2HIGH;	
		btn.poPosition.x		= 490;
		btn.poPosition.y		= 87;
		btn.pcBmpUp				= m_pcLimit_RiUp;
		btn.pcBmpDown			= m_pcLimit_RiDw;
		btn.pcBmpFocus			= m_pcLimit_RiUp;
		btn.pcBmpDisabled		= m_pcLimit_RiUp;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		fv.iLowerLimit=getModel()->getDATAHANDLER()->getPRICO_SPO2lowRange();
		fv.iUpperLimit=iAlimitSPO2max;
		fv.iValue=getModel()->getDATAHANDLER()->getPRICO_SPO2highRange();

		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->setPRICO_SPO2highRange(fv.iValue);
		}
		else if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->setPRICO_SPO2highRange(fv.iValue);
		}

		m_pcLimitBtn_SPO2high=new CLimitBtn_SPO2high(this,btn,0,false);
		m_pcLimitBtn_SPO2high->Create(this,dwStyleNoTab,fv);
		m_pcLimitBtn_SPO2high->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
		if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
		{
			m_pcLimitBtn_SPO2high->ShowWindow(SW_SHOW);
			m_plLimitBtn.AddTail(m_pcLimitBtn_SPO2high);
		}
		else
		{
			m_pcLimitBtn_SPO2high->ShowWindow(SW_HIDE);
		}
		

		//-------------------------------------------------------------
		//m_pcLimitBtn_FIO2low
		btn.wID					= IDC_BTN_LIMIT_FIO2LOW;	
		btn.poPosition.x		= 175;
		btn.poPosition.y		= 123;
		btn.pcBmpUp				= m_pcLimit_LeUp;
		btn.pcBmpDown			= m_pcLimit_LeDw;
		btn.pcBmpFocus			= m_pcLimit_LeUp;
		btn.pcBmpDisabled		= m_pcLimit_LeUp;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		fv.iLowerLimit=21;
		fv.iUpperLimit=getModel()->getDATAHANDLER()->getPRICO_FIO2highRange();
		fv.iValue=getModel()->getDATAHANDLER()->getPRICO_FIO2lowRange();

		//PRICO02
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->setPRICO_FIO2lowRange(fv.iValue);
		}
		//PRICO02
		if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->setPRICO_FIO2lowRange(fv.iValue);
		}

		m_pcLimitBtn_FIO2low=new CLimitBtn_FIO2low(this,btn,0,false);
		m_pcLimitBtn_FIO2low->Create(this,dwStyleNoTab,fv);
		m_pcLimitBtn_FIO2low->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
		if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
		{
			m_pcLimitBtn_FIO2low->ShowWindow(SW_SHOW);
			m_plLimitBtn.AddTail(m_pcLimitBtn_FIO2low);
		}
		else
		{
			m_pcLimitBtn_FIO2low->ShowWindow(SW_HIDE);
		}
		

		//-------------------------------------------------------------
		//m_pcLimitBtn_FIO2high
		btn.wID					= IDC_BTN_LIMIT_FIO2HIGH;	
		btn.poPosition.x		= 490;
		btn.poPosition.y		= 123;
		btn.pcBmpUp				= m_pcLimit_RiUp;
		btn.pcBmpDown			= m_pcLimit_RiDw;
		btn.pcBmpFocus			= m_pcLimit_RiUp;
		btn.pcBmpDisabled		= m_pcLimit_RiUp;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		fv.iLowerLimit=getModel()->getDATAHANDLER()->getPRICO_FIO2lowRange();
		fv.iUpperLimit=100;
		fv.iValue=getModel()->getDATAHANDLER()->getPRICO_FIO2highRange();

		//PRICO02
		if(fv.iValue>fv.iUpperLimit)
		{
			fv.iValue=fv.iUpperLimit;
			getModel()->getDATAHANDLER()->setPRICO_FIO2highRange(fv.iValue);
		}
		//PRICO02
		if(fv.iValue<fv.iLowerLimit)
		{
			fv.iValue=fv.iLowerLimit;
			getModel()->getDATAHANDLER()->setPRICO_FIO2highRange(fv.iValue);
		}

		m_pcLimitBtn_FIO2high=new CLimitBtn_FIO2high(this,btn,0,false);
		m_pcLimitBtn_FIO2high->Create(this,dwStyleNoTab,fv);
		m_pcLimitBtn_FIO2high->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
		if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
		{
			m_pcLimitBtn_FIO2high->ShowWindow(SW_SHOW);
			m_plLimitBtn.AddTail(m_pcLimitBtn_FIO2high);
		}
		else
		{
			m_pcLimitBtn_FIO2high->ShowWindow(SW_HIDE);
		}
		

		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		SelectObject(m_hdcStatic, hpenprevStat);
		SelectObject(m_hdcStatic,hbrprevStat);
		//cbrBack.DeleteObject();//rkuNEWFIX

		bRes=true;
	}
	else
		bRes=false;

	if(bRes)
		Init();


	return bRes;
}

void CWndDataSPO2::Init()
{
	m_iSPO2=getModel()->getDATAHANDLER()->getAVGMessureDataSPO2();
	m_iPulseRate=getModel()->getDATAHANDLER()->getAVGMessureDataSpO2PulseRate();
	m_iPerfusionIndex=getModel()->getDATAHANDLER()->getAVGMessureDataSpO2PerfusionIndex();
	m_iEXCEPTIONS2=0;
	m_bProcessing=false;
	if(getModel()->getSPO2())
	{
		m_iEXCEPTIONS2=getModel()->getSPO2()->get_EXCEPTION2();
		m_bProcessing=getModel()->getSPO2()->isStateOk();
	}
	

	if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
	{
		m_iFiO2measuredValue=getModel()->getDATAHANDLER()->getAppliedFiO2para();//rku O2
		m_iFIO2HIGHvalue=getModel()->getDATAHANDLER()->getPRICO_FIO2highRange();
		m_iFIO2LOWvalue=getModel()->getDATAHANDLER()->getPRICO_FIO2lowRange();

		m_iSPO2HIGHvalue=getModel()->getDATAHANDLER()->getPRICO_SPO2highRange();
		m_iSPO2LOWvalue=getModel()->getDATAHANDLER()->getPRICO_SPO2lowRange();

		eAlarmLimitState limitstateSPO2Max = getModel()->getALARMHANDLER()->getAlimitState_SPO2maxLimit();
		eAlarmLimitState limitstateSPO2Min = getModel()->getALARMHANDLER()->getAlimitState_SPO2minLimit();
		int iAlimitSPO2min=0;
		int iAlimitSPO2max=0;
		if(limitstateSPO2Max==AL_OFF || limitstateSPO2Max==AL_CALC)
			iAlimitSPO2max=100;
		else
			iAlimitSPO2max=CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitSPO2max())/10, 0);
		//iAlimitSPO2max=getModel()->getALARMHANDLER()->getAlimitSPO2max()/10;xxx round

		if(limitstateSPO2Min==AL_OFF || limitstateSPO2Min==AL_CALC)
			iAlimitSPO2min=0;
		else
			iAlimitSPO2min=CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitSPO2min())/10, 0);
		//iAlimitSPO2min=getModel()->getALARMHANDLER()->getAlimitSPO2min()/10;xxx round

		int iLowerLimit=iAlimitSPO2min;
		int iUpperLimit=getModel()->getDATAHANDLER()->getPRICO_SPO2highRange();
		m_pcLimitBtn_SPO2low->SetLimits(iLowerLimit, iUpperLimit);

		iLowerLimit=getModel()->getDATAHANDLER()->getPRICO_SPO2lowRange();
		iUpperLimit=iAlimitSPO2max;
		m_pcLimitBtn_SPO2high->SetLimits(iLowerLimit, iUpperLimit);

		m_iAlimitSPO2max=iAlimitSPO2max;

		m_iAlimitSPO2min=iAlimitSPO2min;
	}
	
	

	StartSPO2DataThread();

	if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
	{
		PostMessage(WM_CHECK_PRICO_ALARMS);
	}
}

void CWndDataSPO2::Show(bool bShow)
{
	if(bShow)
	{
		SetWindowPos(NULL,0,0,m_lX,m_lX,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
		//Draw(true,true);
		//Draw((bool bStatic, bool bValues, bool bLimits, bool bPRICOSpO2, bool bPRICOFiO2);
		Draw(true, true, true, true);
	}
	else
	{
		SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);
	}
}

//************************************
// Method:    OnDestroy
// FullName:  CWndDataSPO2::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of thread
//************************************
void CWndDataSPO2::OnDestroy()
{
	KillTimer(PICORANGETIMER);
	StopSPO2DataThread();

	if(m_pcwtSPO2DataThread!=NULL)
	{
		delete m_pcwtSPO2DataThread;
		m_pcwtSPO2DataThread=NULL;

		if(m_hThreadSPO2Data!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSPO2Data);
			m_hThreadSPO2Data=INVALID_HANDLE_VALUE;
		}
	}

	CWnd::OnDestroy();

	if(m_hdcStatic)
		DeleteDC(m_hdcStatic);

	if(m_hbmpStatic)
		DeleteObject(m_hbmpStatic);

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);
}

void CWndDataSPO2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);

	//DEBUGMSG(TRUE, (TEXT("CWndDataSPO2::OnPaint\r\n")));
}

//void CWndDataSPO2::UpdateAlarmLimits()
//{
//	Draw(false,true);
//}

void CWndDataSPO2::Draw(bool bStatic, bool bValues, bool bPRICOSpO2, bool bPRICOFiO2)
{
	EnterCriticalSection(&csDraw);

	if(bStatic)
	{
		DrawStatic();
	}

	if(bValues)
		DrawValues();

	if(bPRICOSpO2 && getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
		drawPRICOSpO2();

	if(bPRICOFiO2 && getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
		drawPRICOFiO2();

	LeaveCriticalSection(&csDraw);
}

void CWndDataSPO2::DrawStatic()
{
	RECT rc;

	CDC* pDCStatic=CDC::FromHandle(m_hdcStatic);

	CBrush cbrBack(BACKGND);

	CBrush cbrYellow(RGB(252,252,170));
	CBrush cbrDarkGrey(DARKGREY);
	CBrush cbrLightGrey(LIGHTGREY);
	//rkuNEWFIX
	//CPen penLine;
	//penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));

	HBRUSH hbrprev=(HBRUSH)SelectObject(m_hdcStatic,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(m_hdcStatic, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(m_hdcStatic,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(m_hdcStatic,g_hf14AcuMed);
	int tc=SetTextColor(m_hdcStatic,0x0000000);

	Rectangle(m_hdcStatic, 0, 0, m_lX, m_lY);

	m_pcSPO2ParaLeftTop->Draw(m_hdcStatic,0,0);
	m_pcSPO2ParaRightTop->Draw(m_hdcStatic,545,0);
	m_pcSPO2ParaLeftBot->Draw(m_hdcStatic,0,85);
	m_pcSPO2ParaRightBot->Draw(m_hdcStatic,545,85);

	SelectObject(m_hdcStatic,cbrYellow);
	Rectangle(m_hdcStatic, 20, 00, 545, 80);

	SelectObject(m_hdcStatic,cbrDarkGrey);
	Rectangle(m_hdcStatic, 20, 85, 545, DIAGRAMM_DATAHEIGHT);

	SelectObject(m_hdcStatic,cbrBack);
	
	Rectangle(m_hdcStatic, 0, 80, 565, 85);
	Rectangle(m_hdcStatic, 185, 00, 190, 80);
	Rectangle(m_hdcStatic, 375, 00, 380, 80);

	//*******************************PI*****************************//
	rc.left = 205;  
	rc.top = 5;  
	rc.right  = 375;  
	rc.bottom = 200;
	CStringW cs = _T("PI");
	//cs = getModel()->GetLanguageString(IDS_TXT_SENSOR_PUMPONOFF);
	cs = getModel()->GetLanguageString(IDS_PARA_SPO2_PI);
	DrawText(m_hdcStatic,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
		

	//*******************************SPO2*****************************//
	rc.left = 15;  
	rc.top = 5;  
	rc.right  = 185;  
	rc.bottom = 200;
	//cs = getModel()->GetLanguageString(IDS_PARA_ETCO);
	cs = _T("SpO");
	CSize sz = pDCStatic->GetTextExtent(cs);
	cs += _T("    [%]");
	//cs += m_csUnit;
	DrawText(m_hdcStatic,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 15+sz.cx;  
	rc.top = 10;  
	rc.right  = 186;  
	rc.bottom = 200;
	//cs = getModel()->GetLanguageString(IDS_PARA_ETCO2);
	cs = _T("2");
	DrawText(m_hdcStatic,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//*******************************PulseRate*****************************//
	rc.left = 395;//199;  
	rc.top = 5;  
	rc.right  = 565;//364;  
	rc.bottom = 200;
	//cs = _T("Pulse Rate");
	cs = getModel()->GetLanguageString(IDS_PARA_SPO2_PULSERATE);
	sz = pDCStatic->GetTextExtent(cs);
	//cs += _T("   [")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]");
	DrawText(m_hdcStatic,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	
	rc.left = 399+sz.cx;//199;  
	rc.top = 5;  
	rc.right  = 565;//364;  
	rc.bottom = 200;
	//cs = _T("Resp Rate [BPM]");
	cs = _T("   [")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]");
	DrawText(m_hdcStatic,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	//*******************************PRICO*****************************//
	//*******************************PRICO on/off BUTTON*****************************//
	if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
	{
		SelectObject(m_hdcStatic,g_hf8AcuBold);
		SelectObject(m_hdcStatic,(HPEN)penLine);
		SelectObject(m_hdcStatic,cbrLightGrey);
		RoundRect(m_hdcStatic, 10, 90, 100, 152,20,20);

		rc.left = 10;  
		rc.top = 85;  
		rc.right  = 100;  
		rc.bottom = 115;
		DrawText(m_hdcStatic,_T("PRICO"),-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		SelectObject(m_hdcStatic,g_hf14AcuBold);

		rc.left = 120;  
		rc.top = 95;  
		rc.right  = 565;  
		rc.bottom = 152;
		DrawText(m_hdcStatic,_T("SpO2"),-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		rc.left = 120;  
		rc.top = 90;  
		rc.right  = 565;  
		rc.bottom = 147;
		DrawText(m_hdcStatic,_T("FiO2"),-1,&rc,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);

		SelectObject(m_hdcStatic,g_hf10AcuBold);

		rc.left = 120;  
		rc.top = 95;  
		rc.right  = 560;  
		rc.bottom = 152;
		DrawText(m_hdcStatic,_T("%"),-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);

		rc.left = 120;  
		rc.top = 90;  
		rc.right  = 560;  
		rc.bottom = 147;
		DrawText(m_hdcStatic,_T("%"),-1,&rc,DT_RIGHT|DT_BOTTOM|DT_SINGLELINE);

		//SelectObject(m_hdcStatic,(HPEN)penLine);//rkuNEWFIX
		SelectObject(m_hdcStatic,cbrYellow);
		Rectangle(m_hdcStatic, 222, 87, 490, 119);
		Rectangle(m_hdcStatic, 222, 123, 490, 155);
	}
	else 
	{
		//*******************************Sensor state*****************************//
		SelectObject(m_hdcStatic,g_hf10AcuBold);

		cs = getModel()->GetLanguageString(IDS_TXT_SENSOR_STATE);
		
		bool bCheckSensor=false;
		bool bSensorNotConnected=false;
		bool bPulseSearch=false;
		bool bProcessingActive=false;
		bool bDemoMode=false;

		if(getModel()->getSPO2()==false)
		{
			bSensorNotConnected=true;
			cs += _T(" ");
			cs += getModel()->GetLanguageString(IDS_TXT_SPO2_NOCONUSB);
			//cs += _T(" not connected to USB,");
			cs += _T(",");
		}
		else if(getModel()->getSPO2()->isSensorConnectedToUSB()==false)
		{
			bSensorNotConnected=true;
			cs += _T(" ");
			cs += getModel()->GetLanguageString(IDS_TXT_SPO2_NOCONUSB);
			//cs += _T(" not connected to USB,");
			cs += _T(",");
		}
		else
		{
			if(m_iEXCEPTIONS2 & BIT0)	// no sensor connected
			{
				cs += _T(" ");
				bSensorNotConnected=true;
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_NOCONMODULE);
				//cs += _T(" not connected to module,");
				cs += _T(",");
			}

			if(m_iEXCEPTIONS2 & BIT1)	// defective sensor
			{
				bCheckSensor=true;
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_DEFSENS);
				//cs += _T(" defective sensor,");
				cs += _T(",");
			}

			if(m_iEXCEPTIONS2 & BIT2)	// low perfusion
			{
				bCheckSensor=true;
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_LOWPERF);
				//cs += _T(" low perfusion,");
				cs += _T(",");
			}

			if(m_iEXCEPTIONS2 & BIT3)	// pulse search
			{
				bPulseSearch=true;
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_PULSESEARCH);
				//cs += _T(" pulse search,");
				cs += _T(",");
			}

			if(m_iEXCEPTIONS2 & BIT4)	// interference detected
			{
				bCheckSensor=true;
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_INTERFERENCE);
				//cs += _T(" interference detected,");
				cs += _T(",");
			}

			if(m_iEXCEPTIONS2 & BIT5)	// sensor off patient
			{
				bCheckSensor=true;
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_OFFPATIENT);
				//cs += _T(" sensor off patient,");
				cs += _T(",");
			}

			if(m_iEXCEPTIONS2 & BIT6)	// too much ambient light
			{
				bCheckSensor=true;
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_AMBIENTLIGHT);
				//cs += _T(" too much ambient light,");
				cs += _T(",");
			}

			if(m_iEXCEPTIONS2 & BIT7)	// unrecognized sensor
			{
				bCheckSensor=true;
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_UNRECSENS);
				//cs += _T(" unrecognized sensor,");
				cs += _T(",");
			}

			if(m_iEXCEPTIONS2 & BIT10)	// low signal IQ
			{
				bCheckSensor=true;
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_LOWIQ);
				//cs += _T(" low signal IQ,");
				cs += _T(",");
			}

			if(m_iEXCEPTIONS2 & BIT11)	// processing active
			{
				bProcessingActive=true;
				//cs += _T(" processing active,");
			}

			if(m_iEXCEPTIONS2 & BIT12)	// no cable connected
			{
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_NOCABLE);
				//cs += _T(" no cable connected,");
				cs += _T(",");
				bCheckSensor=true;
			}

			if(m_iEXCEPTIONS2 & BIT13)	// no adhesive sensor connected
			{
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_NOADHESIVE);
				//cs += _T(" no adhesive sensor connected,");
				cs += _T(",");
				bCheckSensor=true;
			}

			if(m_iEXCEPTIONS2 & BIT14)	// demo mode
			{
				bDemoMode=true;
				cs += _T(" ");
				cs += getModel()->GetLanguageString(IDS_TXT_SPO2_DEMO);
				//cs += _T(" demo mode,");
				cs += _T(",");
			}
		}

		if(!bSensorNotConnected && !bCheckSensor && !bPulseSearch && !bDemoMode && bProcessingActive)	// processing active
		{
			cs += _T(" ");
			cs += getModel()->GetLanguageString(IDS_TXT_SPO2_ACTIVE);
			//cs += _T(" processing active,");
			cs += _T(",");
		}
		else if(!bSensorNotConnected && !bCheckSensor && !bPulseSearch && !bProcessingActive && !bDemoMode)
		{
			cs += _T(" ");
			cs += getModel()->GetLanguageString(IDS_TXT_CHCKSENSOR);
			//cs += _T(" checking");
			cs += _T(",");
		}
		

		SetTextColor(m_hdcStatic,RGB(0,0,0));
		rc.left = 0;  
		rc.top = 85;  
		rc.right  = 545;  
		rc.bottom = 150;
		DrawText(m_hdcStatic,cs,-1,&rc,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}

	//limits
	//################ SpO2 ##################
	m_pcLimitHigh_Up->Draw(m_hdcStatic,120,35);//right-60, top+2
	m_pcLimitLow_Up->Draw(m_hdcStatic,120,60);//right-60, bottom-16


	//################ PI ##################
	m_pcLimitLow_Up->Draw(m_hdcStatic,310,60);

	//################ PULSERATE ##################
	m_pcLimitHigh_Up->Draw(m_hdcStatic,500,35);//right-60, top+2
	m_pcLimitLow_Up->Draw(m_hdcStatic,500,60);//right-60, bottom-16


	
	BitBlt(m_hDC, 0, 0, m_lX, m_lY, m_hdcStatic, 0, 0, SRCCOPY);

	SetTextColor(m_hdcStatic,tc);
	SetBkMode(m_hdcStatic,nBkMode);

	SelectObject(m_hdcStatic,hbrprev);
	SelectObject(m_hdcStatic,hpenprev);

	//DeleteObject(cbrBack);//rkuNEWFIX
	//DeleteObject(cbrYellow);//rkuNEWFIX
	//DeleteObject(cbrDarkGrey);//rkuNEWFIX
	//DeleteObject(cbrLightGrey);//rkuNEWFIX

	//penLine.DeleteObject();//rkuNEWFIX

	SelectObject(m_hdcStatic,hPrevFont);
}

void CWndDataSPO2::DrawValues()
{
	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	//HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,565,80);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);
	
	/*HDC hdcMem = *pDC;
	HBITMAP hBmpMem=CreateCompatibleBitmap(hdcMem,565,80);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);*/

	BitBlt(hdcMem, 0, 0, 565, 80,m_hdcStatic , 0, 0, SRCCOPY);

	//RECT rcCl;
	RECT rc;
	//GetClientRect(&rcCl);

	CBrush cbrBack(BACKGND);
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf43AcuBold);
	int tc=SetTextColor(hdcMem,RGB(0,0,255));

	
	CStringW cs = _T("");

	rc.left = 15;  
	rc.top = 30;  
	rc.right  = 185;  
	rc.bottom = 200;
	if(m_iSPO2>0)
	{
		cs.Format(_T("%0.0f"), CTlsFloat::Round(((double)m_iSPO2)/10, 0));
		//cs.Format(_T("%d"), m_iSPO2/10);
	}
	else
	{
		cs=_T("--");
	}
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//*******************************PulseRate*****************************//

	rc.left = 395;  
	rc.right  = 565;  
	
	if(m_iPulseRate>0)
	{
		cs.Format(_T("%d"), m_iPulseRate);
	}
	else
	{
		cs=_T("--");
	}

	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	
	rc.left = 205;  
	rc.right  = 375;  

	if(m_iPerfusionIndex>0)
	{
		if(m_iPerfusionIndex>=100)
		{
			cs.Format(_T("%0.1f"), CTlsFloat::Round(((double)m_iPerfusionIndex)/1000, 1));
		}
		else
		{
			cs.Format(_T("%0.2f"),CTlsFloat::Round(((double)m_iPerfusionIndex)/1000, 2));
		}
	}
	else
	{
		cs=_T("--");
	}

	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//CDC* pDC=CDC::FromHandle(hdcMem);
	//DrawLimits(pDC);

	SelectObject(hdcMem,g_hf10AcuBold);
	SetTextColor(hdcMem,RGB(200,0,0));

	TCHAR psz[MAX_PATH];

	//################ SpO2 ##################
	//m_pcLimitHigh_Up->Draw(hdcMem,120,30);//right-60, top+2
	//m_pcLimitLow_Up->Draw(hdcMem,120,60);//right-60, bottom-16
	
	rc.top = 33;
	rc.bottom = 75;
	rc.left=50;
	rc.right=175;
	eAlarmLimitState limitstateSPO2Max = getModel()->getALARMHANDLER()->getAlimitState_SPO2maxLimit();
	eAlarmLimitState limitstateSPO2Min = getModel()->getALARMHANDLER()->getAlimitState_SPO2minLimit();
	if(limitstateSPO2Max == AL_CALC)
	{
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_AUTO),-1,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
	}
	else if(limitstateSPO2Max == AL_OFF)
	{
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_OFF),-1,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
	}
	else if(limitstateSPO2Max != AL_OFF)
	{
		wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitSPO2max())/10, 0));
		DrawText(hdcMem,psz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
	}
	if(limitstateSPO2Min == AL_CALC)
	{
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_AUTO),-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
	}
	else if(limitstateSPO2Min == AL_OFF)
	{
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_OFF),-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
	}
	else if(limitstateSPO2Min != AL_OFF)
	{
		wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitSPO2min())/10, 0));
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
	}

	//################ PI ##################
	//m_pcLimitLow_Up->Draw(hdcMem,310,60);
	//m_pcLimitLow_Up->Draw(m_hdcStatic,300,45);

	rc.top = 33;
	rc.bottom = 75;
	rc.left=290;
	rc.right=365;//325;
	eAlarmLimitState limitstateSPO2_PIMin = getModel()->getALARMHANDLER()->getAlimitState_SPO2_PIminLimit();
	if(limitstateSPO2_PIMin == AL_CALC)
	{
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_AUTO),-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
	}
	else if(limitstateSPO2_PIMin == AL_OFF)
	{
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_OFF),-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
	}
	else if(limitstateSPO2_PIMin != AL_OFF)
	{
		int iPI=getModel()->getALARMHANDLER()->getAlimitSPO2_PImin();
		if(iPI>=1000)
		{
			wsprintf(psz,_T("%d"),iPI/1000);
		}
		else if(iPI>=100)
		{
			wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)iPI)/1000, 1));
		}
		else
		{
			wsprintf(psz,_T("%0.2f"),CTlsFloat::Round(((double)iPI)/1000, 2));
		}

		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);

		/*rc.left=295;
		rc.right=325;

		pDCStatic->DrawText(psz,&rc,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);*/
	}

	//################ PULSERATE ##################
	//m_pcLimitHigh_Up->Draw(hdcMem,500,30);//right-60, top+2
	//m_pcLimitLow_Up->Draw(hdcMem,500,60);//right-60, bottom-16

	rc.top = 33;
	rc.bottom = 75;
	rc.left=416;
	rc.right=555;
	eAlarmLimitState limitstatePULSERATEMax = getModel()->getALARMHANDLER()->getAlimitState_PulseRatemaxLimit();
	eAlarmLimitState limitstatePULSERATEMin = getModel()->getALARMHANDLER()->getAlimitState_PulseRateminLimit();
	if(limitstatePULSERATEMax == AL_CALC)
	{
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_AUTO),-1,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
	}
	else if(limitstatePULSERATEMax == AL_OFF)
	{
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_OFF),-1,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
	}
	else if(limitstatePULSERATEMax != AL_OFF)
	{
		wsprintf(psz,_T("%d"),getModel()->getALARMHANDLER()->getAlimitPulseRatemax());
		DrawText(hdcMem,psz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_RIGHT);
	}
	if(limitstatePULSERATEMin == AL_CALC)
	{
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_AUTO),-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
	}
	else if(limitstatePULSERATEMin == AL_OFF)
	{
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_OFF),-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
	}
	else if(limitstatePULSERATEMin != AL_OFF)
	{
		wsprintf(psz,_T("%d"),getModel()->getALARMHANDLER()->getAlimitPulseRatemin());
		DrawText(hdcMem,psz,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_RIGHT);
	}

	BitBlt(m_hDC,0,0,565, 80,hdcMem,0,0,SRCCOPY);
	BitBlt(dc.m_hDC,0,0,565, 80,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	//DeleteObject(cbrBack);//rkuNEWFIX

	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}



//=============================================================================
/**
 * @brief Draws the bargraph for SpO2
 *
 *   This function draws the bargraph for SpO2 range settings
 *   and the actual SpO2 value into the bargraph
 *   depending on alarm limit of SPO2.
 *
 **/
//=============================================================================
void CWndDataSPO2::drawPRICOSpO2()
{
	//DEBUGMSG(TRUE, (TEXT("drawPRICOSpO2\r\n")));
	RECT rcCl,rc;

	rcCl.left = 222;  
	rcCl.top = 87;  
	rcCl.right  = 490;  
	rcCl.bottom = 119;

	rc.left = 0;  
	rc.top = 0;  
	rc.right  = 268;  
	rc.bottom = 32;

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,268,32);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	//BitBlt(hdcMem, 0, 0, 268,32,m_hdcStatic , 0, 0, SRCCOPY);

	CBrush cbrYellow(RGB(252,252,170));
	//CPen penLine(PS_SOLID,1,RGB(140,140,140)); //rkuNEWFIX
	//CPen penCurVal(PS_SOLID,2,RGB(0,0,255)); //rkuNEWFIX

	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrYellow);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)penLine);	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);

	Rectangle(hdcMem, 0, 0, 268, 32);

	CBrush cbrGeen(RGB(0,190,0));
	SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));
	SelectObject(hdcMem,cbrGeen);

	//get low high range and draw green area depending on alarm lit SPO2
	double iSPO2low=m_iSPO2LOWvalue;
	double iSPO2high=m_iSPO2HIGHvalue;
	double iDiffSPO2=m_iAlimitSPO2max-m_iAlimitSPO2min;
	double iFactor=266/iDiffSPO2;

	int iPosLow=0;
	int iPosHigh=0;
	if(iSPO2low==m_iAlimitSPO2min)
	{
		iPosLow=1;//223;
	}
	else
	{
		iPosLow=(int)(1+((iSPO2low-(double)m_iAlimitSPO2min)*iFactor));
	}

	if(iSPO2high==m_iAlimitSPO2max)
	{
		//iPosHigh=489;
		iPosHigh=267;
	}
	else
	{
		iPosHigh=(int)(1+((iSPO2high-(double)m_iAlimitSPO2min)*iFactor));
	}
	//Rectangle(m_hdcStatic, iPosLow, 124, iPosHigh, 154);
	rc.left = iPosLow;  
	rc.top = 1;
	rc.right  = iPosHigh;  
	rc.bottom = 31;
	Rectangle(hdcMem, iPosLow, 1, iPosHigh, 31);

	

	int iSPO2=CTlsFloat::Round(((double)m_iSPO2)/10, 0);
	int iPos=0;
	//bool bOutside=false;
	if(iSPO2>m_iAlimitSPO2max)
	{
		iPos=266;//488;
		//bOutside=true;
	}
	else if(iSPO2<m_iAlimitSPO2min)
	{
		iPos=1;//488;
		//bOutside=true;
	}
	else if(iSPO2==m_iAlimitSPO2max)
	{
		iPos=266;//488;
	}
	else if(iSPO2==m_iAlimitSPO2min)
	{
		iPos=1;//223;
	}
	else
	{
		iPos=(int)(1+(((double)(iSPO2-m_iAlimitSPO2min)*iFactor)));
		
		//Ellipse(m_hdcStatic,222, 124, 230, 154);
	}

	SelectObject(hdcMem, (HPEN)penBlueVal);

	if(false==m_bSpO2blink && m_bProcessing && iSPO2>0)
	{
		MoveToEx(hdcMem, iPos, 1, NULL);
		LineTo(hdcMem, iPos, 31);
	}

	BitBlt(dc.m_hDC,rcCl.left,rcCl.top,rcCl.right,rcCl.bottom,hdcMem,0,0,SRCCOPY);
	BitBlt(m_hDC,rcCl.left,rcCl.top,rcCl.right,rcCl.bottom,hdcMem,0,0,SRCCOPY);
	
	SetBkMode(hdcMem,nBkMode);

	//DeleteObject(cbrYellow);//rkuNEWFIX
	//DeleteObject(cbrGeen);//rkuNEWFIX
	//penLine.DeleteObject(); //rkuNEWFIX
	//penCurVal.DeleteObject(); //rkuNEWFIX

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	SelectObject(hdcMem,hBmpMemPrev);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}

// **************************************************************************
// 
// **************************************************************************
void CWndDataSPO2::drawPRICOFiO2()
{
	//DEBUGMSG(TRUE, (TEXT("drawPRICOFiO2\r\n")));
	RECT rcCl,rc;

	rcCl.left = 222;  
	rcCl.top = 123;  
	rcCl.right  = 490;  
	rcCl.bottom = 155;

	/*rc.left = 222;  
	rc.top = 123;  
	rc.right  = 490;  
	rc.bottom = 155;*/
	rc.left = 0;  
	rc.top = 0;  
	rc.right  = 268;  
	rc.bottom = 32;

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,268,32);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	//BitBlt(hdcMem, 0, 0, 268,32,m_hdcStatic , 0, 0, SRCCOPY);

	CBrush cbrYellow(RGB(252,252,170));
	//CPen penLine(PS_SOLID,1,RGB(140,140,140)); //rkuNEWFIX
	//CPen penCurVal(PS_SOLID,2,RGB(0,0,255)); //rkuNEWFIX

	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrYellow);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)penLine);	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	
	//FillRect(hdcMem,&rc,cbrYellow);
	Rectangle(hdcMem, 0, 0, 268, 32);
	//Rectangle(m_hdcStatic, 222, 123, 490, 155);

	CBrush cbrGeen(RGB(0,190,0));
	SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));
	SelectObject(hdcMem,cbrGeen);
	
	//get low high range and draw green area
	double iFiO2low=m_iFIO2LOWvalue*10;
	double iFiO2high=m_iFIO2HIGHvalue*10;
	
	int iPosLow=0;
	int iPosHigh=0;
	if(iFiO2low==210)
	{
		iPosLow=1;//223;
	}
	else
	{
		double iDiffFiO2=(iFiO2low-210)/10;
		//iPosLow=(int)(223+(iDiffFiO2*3.3));	
		iPosLow=(int)(1+(iDiffFiO2*3.3));
	}

	if(iFiO2high==1000)
	{
		//iPosHigh=489;
		iPosHigh=267;
	}
	else
	{
		double iDiffFiO2=(iFiO2high-210)/10;
		iPosHigh=(int)(1+(iDiffFiO2*3.3));
	}
	rc.left = iPosLow;  
	rc.top = 1;
	rc.right  = iPosHigh;  
	rc.bottom = 31;
	Rectangle(hdcMem, iPosLow, 1, iPosHigh, 31);

	SelectObject(hdcMem, (HPEN)penBlueVal);  //rkuNEWFIX
	
	int iPos=0;
	if(m_iFiO2measuredValue>=1000)
	{
		iPos=266;
	}
	else if(m_iFiO2measuredValue<=210)
	{
		iPos=1;
	}
	else//if(m_iFiO2measuredValue>210)
	{
		double iDiffFiO2=((double)m_iFiO2measuredValue-210)/10;
		iPos=(int)(1+(iDiffFiO2*3.3));		
	}

	MoveToEx(hdcMem, iPos, 1, NULL);
	LineTo(hdcMem, iPos, 31);

	BitBlt(dc.m_hDC,rcCl.left,rcCl.top,rcCl.right,rcCl.bottom,hdcMem,0,0,SRCCOPY);
	BitBlt(m_hDC,rcCl.left,rcCl.top,rcCl.right,rcCl.bottom,hdcMem,0,0,SRCCOPY);
	
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	//DeleteObject(cbrYellow);//rkuNEWFIX
	//DeleteObject(cbrGeen);//rkuNEWFIX

	//penLine.DeleteObject(); //rkuNEWFIX
	//penCurVal.DeleteObject(); //rkuNEWFIX

	SelectObject(hdcMem,hBmpMemPrev);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}


/***************************************************************************/
//     Purpose: 
//**************************************************************************/
void CWndDataSPO2::StartSPO2DataThread( void )
{
	startThread();

	if(m_pcwtSPO2DataThread!=NULL)
	{
		delete m_pcwtSPO2DataThread;
		m_pcwtSPO2DataThread=NULL;

		if(m_hThreadSPO2Data!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadSPO2Data);
			m_hThreadSPO2Data=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtSPO2DataThread=AfxBeginThread(CSPO2DataThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadSPO2Data=m_pcwtSPO2DataThread->m_hThread;
	m_pcwtSPO2DataThread->m_bAutoDelete = FALSE; 
	m_pcwtSPO2DataThread->ResumeThread();
}
//************************************
// Method:    StopSPO2DataThread
// FullName:  CWndDataSPO2::StopSPO2DataThread
// Access:    protected 
// Returns:   void
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
void CWndDataSPO2::StopSPO2DataThread( void )
{
	if(doThread())
	{
		stopThread();

		g_eventSPO2Data.SetEvent();
		
		if (WaitForSingleObject(m_pcwtSPO2DataThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:035a"));
			if(!TerminateThread(m_pcwtSPO2DataThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:035b"));
			}
		}
	}
}
bool CWndDataSPO2::doThread()
{
	bool bRes=false;
	EnterCriticalSection(&csDoThread);
	bRes=m_bDoThread;
	LeaveCriticalSection(&csDoThread);
	return bRes;
}
void CWndDataSPO2::startThread()
{
	EnterCriticalSection(&csDoThread);
	m_bDoThread=true;
	LeaveCriticalSection(&csDoThread);
}
void CWndDataSPO2::stopThread()
{
	EnterCriticalSection(&csDoThread);
	m_bDoThread=false;
	LeaveCriticalSection(&csDoThread);
}
//************************************
// Method:    CSPO2DataThread
// FullName:  CSPO2DataThread
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID pc
//
// 2015/06/19: checked for correct closing of thread
//************************************
static UINT CSPO2DataThread( LPVOID pc )
{
	((CWndDataSPO2*)pc)->SPO2Data();
	return TRUE;
}

//************************************
// Method:    SPO2Data
// FullName:  CWndDataSPO2::SPO2Data
// Access:    private 
// Returns:   DWORD
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
DWORD CWndDataSPO2::SPO2Data(void) 
{
	CeSetThreadPriority(m_pcwtSPO2DataThread->m_hThread,256);
	
	do
	{
		WaitForSingleObject(g_eventSPO2Data, INFINITE);

		if(doThread())
		{
			bool bDrawValue=false;
			bool bDrawSpO2=false;
			bool bDrawEXCEPTIONS2=false;
			bool bDrawPRICOFiO2=false;
			bool bCheckPricoState=false;

			SHORT iFiO2measuredValue=getModel()->getDATAHANDLER()->getAppliedFiO2para();//rku O2
			if(m_iFiO2measuredValue!=iFiO2measuredValue)
			{
				m_iFiO2measuredValue=iFiO2measuredValue;
				bDrawPRICOFiO2=true;
			}

			SHORT iSPO2=getModel()->getDATAHANDLER()->getAVGMessureDataSPO2();
			if(m_iSPO2!=iSPO2)
			{
				m_iSPO2=iSPO2;
				bDrawSpO2=true;
				bDrawValue=true;
			}

			int iSPO2check=CTlsFloat::Round(((double)m_iSPO2)/10, 0);
			if(iSPO2check>m_iAlimitSPO2max)
			{
				if(false==m_bSpO2Out)
				{
					m_bSpO2Out=true;
					SetTimer(PICORANGETIMER,1000,NULL);
					m_bSpO2blink=true;
				}
			}
			else if(iSPO2check<m_iAlimitSPO2min)
			{
				if(false==m_bSpO2Out)
				{
					m_bSpO2Out=true;
					SetTimer(PICORANGETIMER,1000,NULL);
					m_bSpO2blink=true;
				}
			}
			else if(true==m_bSpO2Out)
			{
				//reset
				m_bSpO2Out=false;
				KillTimer(PICORANGETIMER);
				m_bSpO2blink=false;
			}
			

			SHORT iPulseRate=getModel()->getDATAHANDLER()->getAVGMessureDataSpO2PulseRate();
			if(m_iPulseRate!=iPulseRate)
			{
				m_iPulseRate=iPulseRate;
				bDrawValue=true;
			}

			SHORT iEXCEPTIONS2=0;
			if(getModel()->getSPO2())
			{
				iEXCEPTIONS2=getModel()->getSPO2()->get_EXCEPTION2();
			}
			if(m_iEXCEPTIONS2!=iEXCEPTIONS2)
			{
				m_iEXCEPTIONS2=iEXCEPTIONS2;
				bDrawEXCEPTIONS2=true;
				bDrawValue=true;
			}


			SHORT iPerfusionIndex=getModel()->getDATAHANDLER()->getAVGMessureDataSpO2PerfusionIndex();
			if(m_iPerfusionIndex!=iPerfusionIndex)
			{
				m_iPerfusionIndex=iPerfusionIndex;
				bDrawValue=true;
			}

			if(getModel()->getSPO2())
			{
				if(m_bProcessing!=getModel()->getSPO2()->isStateOk())
				{
					m_bProcessing=getModel()->getSPO2()->isStateOk();
					bCheckPricoState=true;
					bDrawSpO2=true;
					bDrawValue=true;
				}
			}
			
			if(bDrawEXCEPTIONS2)
			{
				//Draw(true,true);
				Draw(true, bDrawValue,  bDrawSpO2, bDrawPRICOFiO2);
			}
			else if(bDrawValue || bDrawSpO2 || bDrawPRICOFiO2)
			{
				//Draw(false,false);
				Draw(false, bDrawValue, bDrawSpO2, bDrawPRICOFiO2);
			}

			if(bCheckPricoState)
			{
				checkPRICOstate();
			}
			
			
		}
	}while(doThread());

	theApp.getLog()->WriteLine(_T("#THR:035"));

	return 0;
}

// **************************************************************************
// 
// **************************************************************************
void CWndDataSPO2::drawMeasuredFiO2Value()
{
	g_eventSPO2Data.SetEvent();
}
// **************************************************************************
// 
// **************************************************************************
void CWndDataSPO2::setFIO2LOWvalue(BYTE iFIO2LOWvalue)
{
	if(m_iFIO2LOWvalue!=iFIO2LOWvalue)
	{
		m_iFIO2LOWvalue=iFIO2LOWvalue;
		Draw(false, false, false, true);
		
		
	}
}
// **************************************************************************
// 
// **************************************************************************
void CWndDataSPO2::setFIO2HIGHvalue(BYTE iFIO2HIGHvalue)
{
	if(m_iFIO2HIGHvalue!=iFIO2HIGHvalue)
	{
		m_iFIO2HIGHvalue=iFIO2HIGHvalue;
		//drawPRICOFiO2();
		Draw(false, false, false, true);

		
	}
}

// **************************************************************************
// 
// **************************************************************************
void CWndDataSPO2::setSPO2LOWvalue(BYTE iSPO2LOWvalue)
{
	if(m_iSPO2LOWvalue!=iSPO2LOWvalue)
	{
		m_iSPO2LOWvalue=iSPO2LOWvalue;
		Draw(false, false, true, false);

		
	}
}
// **************************************************************************
// 
// **************************************************************************
void CWndDataSPO2::setSPO2HIGHvalue(BYTE iSPO2HIGHvalue)
{
	if(m_iSPO2HIGHvalue!=iSPO2HIGHvalue)
	{
		m_iSPO2HIGHvalue=iSPO2HIGHvalue;
		
		Draw(false, false, true, false);
	}
}

// **************************************************************************
// 
// **************************************************************************
void CWndDataSPO2::setPRICOrunning(bool state)
{
	if(state==true)
		m_pcPRICO_OnOff->Depress(true);
	else
		m_pcPRICO_OnOff->Depress(false);
}

void CWndDataSPO2::enablePRICObtn()
{
	if(m_pcPRICO_OnOff)
		m_pcPRICO_OnOff->EnableWindow(TRUE);
}
void CWndDataSPO2::disablePRICObtn()
{
	if(m_pcPRICO_OnOff)
		m_pcPRICO_OnOff->EnableWindow(FALSE);
}
void CWndDataSPO2::checkPRICOstate()
{
	eAlarm curAlarm=getModel()->getALARMHANDLER()->getActiveAlarm();
	bool bPRICOdisable=false;

	if(getModel()->isO2FlushActive())//PRICO04
	{
		bPRICOdisable=true;
	}
	else if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==false)
	{
		bPRICOdisable=true;
	}
	else if(curAlarm!=AL_NONE)
	{
		if(getModel()->getALARMHANDLER()->isPRICOrelevantAlarmActive())
		{
			bPRICOdisable=true;
		}
	}

	

	if(bPRICOdisable)
	{
		disablePRICObtn();
	}
	else if(getModel()->getSPO2()==false)
	{
		disablePRICObtn();
	}
	else if(getModel()->getSPO2()->isStateOk()==false)
	{
		disablePRICObtn();
	}
	else if(getModel()->getDATAHANDLER()->GetO2SensorState()==OXYSENS_OFF)
	{
		disablePRICObtn();
	}
	else
	{
		enablePRICObtn();
	}

	if(getModel()->getSPO2())
	{
		m_bProcessing=getModel()->getSPO2()->isStateOk();
	}
	else
		m_bProcessing=false;
	

	if(GetParent())
		GetParent()->PostMessage(WM_SETVIEWFOCUS);
}


void CWndDataSPO2::UpdateMessureData()
{
	g_eventSPO2Data.SetEvent();
	//Draw(bLimits,bLimits);
}
//void CWndDataSPO2::UpdateSPO2Range()
//{
//	/*m_iSPO2HIGHvalue=getModel()->getDATAHANDLER()->getPRICO_SPO2highRange();
//	m_iSPO2LOWvalue=getModel()->getDATAHANDLER()->getPRICO_SPO2lowRange();
//
//	m_pcLimitBtn_SPO2low->SetParaValue(m_iSPO2LOWvalue);
//	m_pcLimitBtn_SPO2high->SetParaValue(m_iSPO2HIGHvalue);*/
//
//
//	UpdateLimitData();
//}

void CWndDataSPO2::UpdateLimitData()
{
	eAlarmLimitState limitstateSPO2Max = getModel()->getALARMHANDLER()->getAlimitState_SPO2maxLimit();
	eAlarmLimitState limitstateSPO2Min = getModel()->getALARMHANDLER()->getAlimitState_SPO2minLimit();
	int iAlimitSPO2min=0;
	int iAlimitSPO2max=0;
	if(limitstateSPO2Max==AL_OFF || limitstateSPO2Max==AL_CALC)
		iAlimitSPO2max=100;
	else
		iAlimitSPO2max=CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitSPO2max())/10, 0);
		//iAlimitSPO2max=getModel()->getALARMHANDLER()->getAlimitSPO2max()/10;

	if(limitstateSPO2Min==AL_OFF || limitstateSPO2Min==AL_CALC)
		iAlimitSPO2min=0;
	else
		iAlimitSPO2min=CTlsFloat::Round(((double)getModel()->getALARMHANDLER()->getAlimitSPO2min())/10, 0);
		//iAlimitSPO2min=getModel()->getALARMHANDLER()->getAlimitSPO2min()/10;

	int iLowerLimit=iAlimitSPO2min;
	int iUpperLimit=getModel()->getDATAHANDLER()->getPRICO_SPO2highRange();
	m_pcLimitBtn_SPO2low->SetLimits(iLowerLimit, iUpperLimit);

	iLowerLimit=getModel()->getDATAHANDLER()->getPRICO_SPO2lowRange();
	iUpperLimit=iAlimitSPO2max;
	m_pcLimitBtn_SPO2high->SetLimits(iLowerLimit, iUpperLimit);

	m_iAlimitSPO2max=iAlimitSPO2max;
	m_iAlimitSPO2min=iAlimitSPO2min;

	m_iSPO2HIGHvalue=getModel()->getDATAHANDLER()->getPRICO_SPO2highRange();
	m_iSPO2LOWvalue=getModel()->getDATAHANDLER()->getPRICO_SPO2lowRange();

	m_pcLimitBtn_SPO2low->SetParaValue(m_iSPO2LOWvalue);
	m_pcLimitBtn_SPO2high->SetParaValue(m_iSPO2HIGHvalue);

	Draw(false, true, true, false);
	//g_eventSPO2Data.SetEvent();
}

LRESULT CWndDataSPO2::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	int iID = 0;
	switch(message)
	{
	case WM_CHECK_PRICO_ALARMS:
		{
			if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true)
			{
				checkPRICOstate();
			}
		}
		break;
		//case WM_FIO2_CHANGED://PRICO02
		//	{
		//		drawPRICOFiO2();
		//		if(GetParent())
		//			GetParent()->PostMessage(WM_SET_PARATIMER);
		//	}
		//	break;
		//case WM_SPO2_CHANGED://PRICO02
		//	{
		//		drawPRICOSpO2();
		//		if(GetParent())
		//			GetParent()->PostMessage(WM_SET_PARATIMER);
		//	}
		//	break;
	case WM_FIO2LOW_LIMITED://PRICO02
		{
			CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, _T("! PRICO: limmited due to current O2 !"), 3000);
			getModel()->triggerEvent(&event);

			getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
		}
		break;
	case WM_FIO2HIGH_LIMITED://PRICO02
		{
			CMVEventInfotext event(CMVEventInfotext::EV_TIMETEXT, _T("! PRICO: limmited due to current O2 !"), 3000);
			getModel()->triggerEvent(&event);

			getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);
		}
		break;
	case WM_SPO2HIGH_LIMITEDLOW:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SPO2HIGH_LIMITEDLOW);
		}
		break;
	case WM_SPO2HIGH_LIMITEDHIGH:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SPO2HIGH_LIMITEDHIGH);
		}
		break;
	case WM_SPO2LOW_LIMITEDLOW:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SPO2LOW_LIMITEDLOW);
		}
		break;
	case WM_SPO2LOW_LIMITEDHIGH:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SPO2LOW_LIMITEDHIGH);
		}
		break;
	case WM_SET_PARATIMER:
		{
			if(GetParent())
				GetParent()->PostMessage(WM_SET_PARATIMER);
		}
		break;
	case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_PRICO_ONOFF:
				{
					if(getModel()->getDATAHANDLER()->getPRICOState()==true)
					{
						m_pcPRICO_OnOff->Depress(false);
						getModel()->getDATAHANDLER()->setPRICOoff();//PRICO04
					}
					else
					{
						m_pcPRICO_OnOff->Depress(true);
						getModel()->getDATAHANDLER()->setPRICOon();//PRICO04
					}

					if(getModel()->getALARMHANDLER()->isPRICOAutoTurnedOff())//rku AUTOPRICO
					{
						getModel()->getALARMHANDLER()->resetPRICOAutoTurnedOff();
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

// **************************************************************************
// 
// **************************************************************************
//void CWndDataSPO2::SetOneButtonDepressed(int btnID)
//{
//	POSITION pos;
//	try
//	{
//		for( pos = m_plLimitBtn.GetHeadPosition(); pos != NULL; )
//		{
//			CParaBtn* pParaBtn = ( CParaBtn* ) m_plLimitBtn.GetNext( pos );
//			if(pParaBtn->GetBtnId() != btnID)
//			{
//				if(pParaBtn->GetButton())
//				{
//					pParaBtn->DrawDirectUp(false);
//				}
//			}
//		}
//	}
//	catch (...)
//	{
//		CFabianHFOApp::ReportException(_T("EXCEPTION: CWndDataSPO2::SetOneButtonDepressed"));
//	}
//	
//}
bool CWndDataSPO2::isLimitBtnDepressed()
{
	bool bDepressed=false;
	POSITION pos;
	try
	{
		for( pos = m_plLimitBtn.GetHeadPosition(); pos != NULL; )
		{
			CLimitBtn* pParaBtn = ( CLimitBtn* ) m_plLimitBtn.GetNext( pos );
			if(pParaBtn->IsDepressed())
			{
				bDepressed=true;
				break;
			}
		}
	}
	catch (...)
	{
		CFabianHFOApp::ReportException(_T("EXCEPTION: CWndDataSPO2::SetAllButtonUnpressed"));
	}
	return bDepressed;
}

void CWndDataSPO2::SetAllButtonUnpressed()
{
	POSITION pos;
	try
	{
		for( pos = m_plLimitBtn.GetHeadPosition(); pos != NULL; )
		{
			CLimitBtn* pParaBtn = ( CLimitBtn* ) m_plLimitBtn.GetNext( pos );
			if(pParaBtn->IsDepressed())
			{
				pParaBtn->Depress();
				break;
			}
		}
	}
	catch (...)
	{
		CFabianHFOApp::ReportException(_T("EXCEPTION: CWndDataSPO2::SetAllButtonUnpressed"));
	}
}

void CWndDataSPO2::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==PICORANGETIMER)
	{
		bool bSpO2=false;
		if(m_bSpO2Out)
		{
			bSpO2=true;
			m_bSpO2blink=!m_bSpO2blink;
		}

		bool bFiO2=false;
		/*if(m_bFiO2Out)
		{
			bFiO2=true;
			m_bFiO2blink=!m_bFiO2blink;
		}*/
		Draw(false, false, bSpO2, bFiO2);
	}
	
	

	CWnd::OnTimer(nIDEvent);
}