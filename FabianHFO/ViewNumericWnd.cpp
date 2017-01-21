#include "StdAfx.h"
#include "ViewNumericWnd.h"
#include "MVViewHandler.h"

#define NUMAV_WND_ID			0x0124

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000


// **************************************************************************
// 
// **************************************************************************
CViewNumericWnd::CViewNumericWnd(int ViewID):
CMVView(ViewID)
{
	InitializeCriticalSection(&csNumWnd);

	m_bExit=false;

	m_eNumBlock=NUMB_NONE;
	m_bBackState=false;
	
	m_iCurrWnd=0;


	m_nwa.nX=610;
	m_nwa.nY=41;

	m_nwa.nCx=190;
	//m_nwa.nCy=559;
	m_nwa.nCy=559;

	m_pcWndNumIPPVdefault=NULL;
	m_pcWndNumIPPVdefaultmech=NULL;
	m_pcWndNumSIPPVdefault=NULL;
	m_pcWndNumSIPPVdefaultmech=NULL;
	m_pcWndNumSIMVdefault=NULL;
	m_pcWndNumSIMVdefaultmech=NULL;
	m_pcWndNumCPAPdefault=NULL;
	m_pcWndNumCPAPdefaultmech=NULL;
	m_pcWndNumNCPAPdefault=NULL;
	m_pcWndNumDUOPAPdefault=NULL;
	m_pcWndNumTHERAPIEdefault=NULL;
	m_pcWndNumHFOdefault=NULL;
	m_pcWndNumHFOdefaultmech=NULL;
	m_pcWndNumFlowOff=NULL;
	m_pcWndNumFlowOffHFO=NULL;

	m_pcWndNumPSVdefault=NULL;
	m_pcWndNumPSVdefaultmech=NULL;
	m_pcWndNumSIMVPSVdefault=NULL;
	m_pcWndNumSIMVPSVdefaultmech=NULL;

	m_pcWndNumConfig=NULL;

	m_pcMenuNumeric=NULL;
	m_pcNumeric_Up=NULL;
	m_pcNumeric_Dw=NULL;

	m_iCurNumericBlock=0;
	m_iCountNumericBlock=0;

}

// **************************************************************************
// 
// **************************************************************************
CViewNumericWnd::~CViewNumericWnd(void)
{
	if(m_pcMenuNumeric)
		delete m_pcMenuNumeric;
	m_pcMenuNumeric=NULL;
	if(m_pcNumeric_Up)
		delete m_pcNumeric_Up;
	m_pcNumeric_Up=NULL;
	if(m_pcNumeric_Dw)
		delete m_pcNumeric_Dw;
	m_pcNumeric_Dw=NULL;

	DeleteCriticalSection(&csNumWnd);
}


BEGIN_MESSAGE_MAP(CViewNumericWnd, CMVView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateView()
{
	RECT rcLd={m_nwa.nX,m_nwa.nY,m_nwa.nX+m_nwa.nCx,m_nwa.nY+m_nwa.nCy};

	if (!Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_NUM))
	{
		return false;
	}

	Initialize();

	return true;
}


// **************************************************************************
// 
// **************************************************************************
BOOL CViewNumericWnd::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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
		//CBrush cbrBack(RGB(180,180,255));
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();//rkuNEWFIX

		return 1;
	}
	else
		return 0;
}


// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::Initialize()
{
	CClientDC dc(this);

	BTN btn;

	m_pcNumeric_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUMERIC3_UP);
	m_pcNumeric_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUMERIC3_DW);
	//m_pcNumeric_Fc		= new CBmp(theApp.m_hInstance,hdc,	IDB_BTN_NUMMENU_FC);

	btn.wID					= IDC_BTN_MENU_NUMERIC;	
	btn.poPosition.x		= 15;
	//btn.poPosition.y		= 550;
	btn.poPosition.y		= 509;
	btn.pcBmpUp				= m_pcNumeric_Up;
	btn.pcBmpDown			= m_pcNumeric_Dw;
	btn.pcBmpFocus			= m_pcNumeric_Up;
	btn.pcBmpDisabled		= m_pcNumeric_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuNumeric=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuNumeric->Create(this,g_hf21AcuBold,0);
	//m_pcMenuNumeric->SetText(getModel()->GetLanguageString(IDS_BTN_NUMERICS));
	m_pcMenuNumeric->SetText(_T(""));
	m_pcMenuNumeric->ShowWindow(SW_HIDE);

	//Open();
	Show();

	return true;
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::Open()
{

	Show();
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::Close()
{
	deleteWndNumIPPV();
	deleteWndNumSIPPV();
	deleteWndNumPSV();
	deleteWndNumSIMV();
	deleteWndNumSIMVPSV();
	deleteWndNumCPAP();
	deleteWndNumHFO();
	deleteWndNumFlowOff();
	deleteWndNumFlowOffHFO();
	deleteWndNumTHERAPIE();
	deleteWndNumDUOPAP();
	deleteWndNumNCPAP();
	deleteWndNumConfig();
	
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::Show()
{
	SetNumericBlock(false);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::Hide()
{
	HideNumWnd();
}

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumHFO_Default()
{
	if(m_pcWndNumHFOdefault==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumHFOdefault = new CWndNumHFO_Av();

		RECT rcLd={0,0,210,560};
		if(m_pcWndNumHFOdefault->Create(this,rcLd,IDC_VIEW_NUM_HFO_DEFAULT))
		{
			m_iCurrWnd=IDC_VIEW_NUM_HFO_DEFAULT;

			/*if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_CALCULATE_AUTOALARMLIMITS);*/
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			m_pcWndNumHFOdefault->DrawData(true,true,true,true,false);
		}
	}
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumHFO_DefaultMech()
{
	if(m_pcWndNumHFOdefaultmech==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumHFOdefaultmech = new CWndNumHFO_Mech();

		RECT rcLd={0,0,210,560};
		if(m_pcWndNumHFOdefaultmech->Create(this,rcLd,IDC_VIEW_NUM_HFO_DEFAULTMECH))
		{
			m_iCurrWnd=IDC_VIEW_NUM_HFO_DEFAULTMECH;

			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}
			m_pcWndNumHFOdefaultmech->DrawData(true,true,true,true,false);
		}
	}
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumCPAP_Default()
{
	if(m_pcWndNumCPAPdefault==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumCPAPdefault = new CWndNumCPAP_Av();
		RECT rcLd={0,0,210,560};

		if(m_pcWndNumCPAPdefault->Create(this,rcLd,IDC_VIEW_NUM_CPAP_DEFAULT))
		{
			m_iCurrWnd=IDC_VIEW_NUM_CPAP_DEFAULT;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			m_pcWndNumCPAPdefault->DrawData(true,true,true,true,false);
		}
	}
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumCPAP_DefaultMech()
{
	if(m_pcWndNumCPAPdefaultmech==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumCPAPdefaultmech = new CWndNumCPAP_Mech();
		RECT rcLd={0,0,210,560};

		if(m_pcWndNumCPAPdefaultmech->Create(this,rcLd,IDC_VIEW_NUM_CPAP_DEFAULTMECH))
		{
			m_iCurrWnd=IDC_VIEW_NUM_CPAP_DEFAULTMECH;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}
			m_pcWndNumCPAPdefaultmech->DrawData(true,true,true,true,false);
		}
	}
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumDUOPAP_Default()
{
	if(m_pcWndNumDUOPAPdefault==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumDUOPAPdefault = new CWndNumAvDUOPAP();
		RECT rcLd={0,0,210,560};

		if(m_pcWndNumDUOPAPdefault->Create(this,rcLd,IDC_VIEW_NUM_DUOPAP))
		{
			m_iCurrWnd=IDC_VIEW_NUM_DUOPAP;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			m_pcWndNumDUOPAPdefault->DrawData(true,true,true,true,true);
		}
	}
	return true;
}


// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumFlowOff()
{
	if(m_pcWndNumFlowOff==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumFlowOff = new CWndNumFlowOff_Av();
		RECT rcLd={0,0,210,560};

		if(m_pcWndNumFlowOff->Create(this,rcLd,IDC_VIEW_NUM_FLOWOFF))
		{
			m_iCurrWnd=IDC_VIEW_NUM_FLOWOFF;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPLOW();
			}

			m_pcWndNumFlowOff->DrawData(true,true,true,true,false);
		}
	}
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumFlowOffHFO()
{
	if(m_pcWndNumFlowOffHFO==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumFlowOffHFO = new CWndNumFlowOffHFO_Av();
		RECT rcLd={0,0,210,560};

		if(m_pcWndNumFlowOffHFO->Create(this,rcLd,IDC_VIEW_NUM_FLOWOFF_HFO))
		{
			m_iCurrWnd=IDC_VIEW_NUM_FLOWOFF_HFO;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			m_pcWndNumFlowOffHFO->DrawData(true,true,true,true,false);
		}
	}
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumNCPAP_Default()
{
	if(m_pcWndNumNCPAPdefault==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumNCPAPdefault = new CWndNumAvNCPAP();
		RECT rcLd={0,0,210,560};

		if(m_pcWndNumNCPAPdefault->Create(this,rcLd,IDC_VIEW_NUM_NCPAP))
		{
			m_iCurrWnd=IDC_VIEW_NUM_NCPAP;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			m_pcWndNumNCPAPdefault->DrawData(true,true,true,true,false);
		}
	}
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumTHERAPIE_Default()
{
	if(m_pcWndNumTHERAPIEdefault==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumTHERAPIEdefault = new CWndNumAvTHERAPIE();
		RECT rcLd={0,0,210,560};

		if(m_pcWndNumTHERAPIEdefault->Create(this,rcLd,IDC_VIEW_NUM_THERAPIE))
		{
			m_iCurrWnd=IDC_VIEW_NUM_THERAPIE;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			m_pcWndNumTHERAPIEdefault->DrawData(true,true,true,true,false);
		}
	}
	return true;
}


// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumIPPV_Default()
{
	if(m_pcWndNumIPPVdefault==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumIPPVdefault = new CWndNumIPPV_BtB();
		RECT rcLd={0,0,210,560};

		if(m_pcWndNumIPPVdefault->Create(this,rcLd,IDC_VIEW_NUM_IPPV_DEFAULT))
		{
			m_iCurrWnd=IDC_VIEW_NUM_IPPV_DEFAULT;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPLOW();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}
			
			m_pcWndNumIPPVdefault->DrawData(true,true,true,true,false);
		}
	}
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumIPPV_DefaultMech()
{
	if(m_pcWndNumIPPVdefaultmech==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumIPPVdefaultmech = new CWndNumIPPV_Mech();

		RECT rcLd={0,0,210,560};
		if(m_pcWndNumIPPVdefaultmech->Create(this,rcLd,IDC_VIEW_NUM_IPPV_DEFAULTMECH))
		{
			m_iCurrWnd=IDC_VIEW_NUM_IPPV_DEFAULTMECH;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPLOW();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			m_pcWndNumIPPVdefaultmech->DrawData(true,true,true,true,false);
		}
	}
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumSIPPV_Default()
{
	if(m_pcWndNumSIPPVdefault==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumSIPPVdefault = new CWndNumSIPPV_Av();

		RECT rcLd={0,0,210,560};
		if(m_pcWndNumSIPPVdefault->Create(this,rcLd,IDC_VIEW_NUM_SIPPV_DEFAULT))
		{
			m_iCurrWnd=IDC_VIEW_NUM_SIPPV_DEFAULT;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPLOW();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}
			
			m_pcWndNumSIPPVdefault->DrawData(true,true,true,true,false);
		}
	}
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumSIPPV_DefaultMech()
{
	if(m_pcWndNumSIPPVdefaultmech==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumSIPPVdefaultmech = new CWndNumSIPPV_Mech();

		RECT rcLd={0,0,210,560};
		if(m_pcWndNumSIPPVdefaultmech->Create(this,rcLd,IDC_VIEW_NUM_SIPPV_DEFAULTMECH))
		{
			m_iCurrWnd=IDC_VIEW_NUM_SIPPV_DEFAULTMECH;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPLOW();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			m_pcWndNumSIPPVdefaultmech->DrawData(true,true,true,true,false);
		}
	}
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumPSV_Default()
{
	if(m_pcWndNumPSVdefault==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumPSVdefault = new CWndNumPSV_Av();

		RECT rcLd={0,0,210,560};
		if(m_pcWndNumPSVdefault->Create(this,rcLd,IDC_VIEW_NUM_PSV_DEFAULT))
		{
			m_iCurrWnd=IDC_VIEW_NUM_PSV_DEFAULT;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPLOW();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			m_pcWndNumPSVdefault->DrawData(true,true,true,true,false);
		}
	}
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumPSV_DefaultMech()
{
	if(m_pcWndNumPSVdefaultmech==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumPSVdefaultmech = new CWndNumPSV_Mech();

		RECT rcLd={0,0,210,560};
		if(m_pcWndNumPSVdefaultmech->Create(this,rcLd,IDC_VIEW_NUM_PSV_DEFAULTMECH))
		{
			m_iCurrWnd=IDC_VIEW_NUM_PSV_DEFAULTMECH;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPLOW();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			m_pcWndNumPSVdefaultmech->DrawData(true,true,true,true,false);
		}
	}
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumSIMV_Default()
{
	if(m_pcWndNumSIMVdefault==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumSIMVdefault = new CWndNumSIMV_Av();

		RECT rcLd={0,0,210,560};
		if(m_pcWndNumSIMVdefault->Create(this,rcLd,IDC_VIEW_NUM_SIMV_DEFAULT))
		{
			m_iCurrWnd=IDC_VIEW_NUM_SIMV_DEFAULT;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPLOW();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			m_pcWndNumSIMVdefault->DrawData(true,true,true,true,false);
		}
	}
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumSIMV_DefaultMech()
{
	if(m_pcWndNumSIMVdefaultmech==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumSIMVdefaultmech = new CWndNumSIMV_Mech();

		RECT rcLd={0,0,210,560};
		if(m_pcWndNumSIMVdefaultmech->Create(this,rcLd,IDC_VIEW_NUM_SIMV_DEFAULTMECH))
		{
			m_iCurrWnd=IDC_VIEW_NUM_SIMV_DEFAULTMECH;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPLOW();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			m_pcWndNumSIMVdefaultmech->DrawData(true,true,true,true,false);
		}
	}
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumSIMVPSV_Default()
{
	if(m_pcWndNumSIMVPSVdefault==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumSIMVPSVdefault = new CWndNumSIMVPSV_Av();

		RECT rcLd={0,0,210,560};
		if(m_pcWndNumSIMVPSVdefault->Create(this,rcLd,IDC_VIEW_NUM_SIMVPSV_DEFAULT))
		{
			m_iCurrWnd=IDC_VIEW_NUM_SIMVPSV_DEFAULT;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPLOW();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			m_pcWndNumSIMVPSVdefault->DrawData(true,true,true,true,false);
		}
	}
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::CreateWndNumSIMVPSV_DefaultMech()
{
	if(m_pcWndNumSIMVPSVdefaultmech==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumSIMVPSVdefaultmech = new CWndNumSIMVPSV_Mech();

		RECT rcLd={0,0,210,560};
		if(m_pcWndNumSIMVPSVdefaultmech->Create(this,rcLd,IDC_VIEW_NUM_SIMVPSV_DEFAULTMECH))
		{
			m_iCurrWnd=IDC_VIEW_NUM_SIMVPSV_DEFAULTMECH;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPLOW();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			m_pcWndNumSIMVPSVdefaultmech->DrawData(true,true,true,true,false);
		}
	}
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewNumericWnd::createWndNumConfig(UINT ID, NUMERICINI* pbufNumeric)
{
	if(m_pcWndNumConfig==NULL && m_nwa.nX>-1)
	{
		m_pcWndNumConfig = new CWndNumConfig(pbufNumeric);

		RECT rcLd={0,0,210,560};
		if(m_pcWndNumConfig->Create(this,rcLd,ID))
		{
			/*m_pcWndNumConfig->Invalidate();
			m_pcWndNumConfig->UpdateWindow();*/
			m_pcWndNumConfig->init();
			m_iCurrWnd=ID;

			if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPEEPmin();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPLOW();
			}
			if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
			{
				getModel()->getDATAHANDLER()->calculateAutoAlarmlimitPIPHIGH();
			}

			m_pcWndNumConfig->DrawData(true,true,true,true,false);

		}
	}
	return true;
}


// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumHFO_Default()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumHFOdefault)
	{
		m_pcWndNumHFOdefault->DestroyWindow();
		delete m_pcWndNumHFOdefault;
		m_pcWndNumHFOdefault=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumHFO_DefaultMech()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumHFOdefaultmech)
	{
		m_pcWndNumHFOdefaultmech->DestroyWindow();
		delete m_pcWndNumHFOdefaultmech;
		m_pcWndNumHFOdefaultmech=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}


// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumIPPV_Default()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumIPPVdefault)
	{
		m_pcWndNumIPPVdefault->DestroyWindow();
		delete m_pcWndNumIPPVdefault;
		m_pcWndNumIPPVdefault=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumIPPV_DefaultMech()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumIPPVdefaultmech)
	{
		m_pcWndNumIPPVdefaultmech->DestroyWindow();
		delete m_pcWndNumIPPVdefaultmech;
		m_pcWndNumIPPVdefaultmech=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumSIPPV_Default()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumSIPPVdefault)
	{
		m_pcWndNumSIPPVdefault->DestroyWindow();
		delete m_pcWndNumSIPPVdefault;
		m_pcWndNumSIPPVdefault=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumSIPPV_DefaultMech()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumSIPPVdefaultmech)
	{
		m_pcWndNumSIPPVdefaultmech->DestroyWindow();
		delete m_pcWndNumSIPPVdefaultmech;
		m_pcWndNumSIPPVdefaultmech=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumPSV_Default()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumPSVdefault)
	{
		m_pcWndNumPSVdefault->DestroyWindow();
		delete m_pcWndNumPSVdefault;
		m_pcWndNumPSVdefault=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumPSV_DefaultMech()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumPSVdefaultmech)
	{
		m_pcWndNumPSVdefaultmech->DestroyWindow();
		delete m_pcWndNumPSVdefaultmech;
		m_pcWndNumPSVdefaultmech=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumSIMVPSV_Default()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumSIMVPSVdefault)
	{
		m_pcWndNumSIMVPSVdefault->DestroyWindow();
		delete m_pcWndNumSIMVPSVdefault;
		m_pcWndNumSIMVPSVdefault=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumSIMVPSV_DefaultMech()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumSIMVPSVdefaultmech)
	{
		m_pcWndNumSIMVPSVdefaultmech->DestroyWindow();
		delete m_pcWndNumSIMVPSVdefaultmech;
		m_pcWndNumSIMVPSVdefaultmech=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}


// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumSIMV_Default()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumSIMVdefault)
	{
		m_pcWndNumSIMVdefault->DestroyWindow();
		delete m_pcWndNumSIMVdefault;
		m_pcWndNumSIMVdefault=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumSIMV_DefaultMech()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumSIMVdefaultmech)
	{
		m_pcWndNumSIMVdefaultmech->DestroyWindow();
		delete m_pcWndNumSIMVdefaultmech;
		m_pcWndNumSIMVdefaultmech=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}


// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumCPAP_Default()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumCPAPdefault)
	{
		m_pcWndNumCPAPdefault->DestroyWindow();
		delete m_pcWndNumCPAPdefault;
		m_pcWndNumCPAPdefault=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumCPAP_DefaultMech()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumCPAPdefaultmech)
	{
		m_pcWndNumCPAPdefaultmech->DestroyWindow();
		delete m_pcWndNumCPAPdefaultmech;
		m_pcWndNumCPAPdefaultmech=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumNCPAP_Default()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumNCPAPdefault)
	{
		m_pcWndNumNCPAPdefault->DestroyWindow();
		delete m_pcWndNumNCPAPdefault;
		m_pcWndNumNCPAPdefault=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumTHERAPIE_Default()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumTHERAPIEdefault)
	{
		m_pcWndNumTHERAPIEdefault->DestroyWindow();
		delete m_pcWndNumTHERAPIEdefault;
		m_pcWndNumTHERAPIEdefault=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumDUOPAP_Default()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumDUOPAPdefault)
	{
		m_pcWndNumDUOPAPdefault->DestroyWindow();
		delete m_pcWndNumDUOPAPdefault;
		m_pcWndNumDUOPAPdefault=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumFlowOff()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumFlowOff)
	{
		m_pcWndNumFlowOff->DestroyWindow();
		delete m_pcWndNumFlowOff;
		m_pcWndNumFlowOff=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumFlowOffHFO()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumFlowOffHFO)
	{
		m_pcWndNumFlowOffHFO->DestroyWindow();
		delete m_pcWndNumFlowOffHFO;
		m_pcWndNumFlowOffHFO=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumConfig()
{
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumConfig)
	{
		m_pcWndNumConfig->DestroyWindow();
		delete m_pcWndNumConfig;
		m_pcWndNumConfig=NULL;
	}
	LeaveCriticalSection(&csNumWnd);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumIPPV()
{
	switch(m_iCurrWnd)
	{
	case IDC_VIEW_NUM_IPPV_DEFAULT:
		{
			deleteWndNumIPPV_Default();
			m_iCurrWnd=0;
		}
		break;
	case IDC_VIEW_NUM_IPPV_DEFAULTMECH:
		{
			deleteWndNumIPPV_DefaultMech();
			m_iCurrWnd=0;
		}
		break;
	case IDC_VIEW_NUM_IPPV:
		{
			deleteWndNumConfig();
			m_iCurrWnd=0;
		}
		break;
	}
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumSIPPV()
{
	switch(m_iCurrWnd)
	{
	case IDC_VIEW_NUM_SIPPV_DEFAULT:
		{
			deleteWndNumSIPPV_Default();
			m_iCurrWnd=0;
		}
		break;
	case IDC_VIEW_NUM_SIPPV_DEFAULTMECH:
		{
			deleteWndNumSIPPV_DefaultMech();
			m_iCurrWnd=0;
		}
		break;
	case IDC_VIEW_NUM_SIPPV:
		{
			deleteWndNumConfig();
			m_iCurrWnd=0;
		}
		break;
	}
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumPSV()
{
	switch(m_iCurrWnd)
	{
	case IDC_VIEW_NUM_PSV_DEFAULT:
		{
			deleteWndNumPSV_Default();
			m_iCurrWnd=0;
		}
		break;
	case IDC_VIEW_NUM_PSV_DEFAULTMECH:
		{
			deleteWndNumPSV_DefaultMech();
			m_iCurrWnd=0;
		}
		break;
	case IDC_VIEW_NUM_PSV:
		{
			deleteWndNumConfig();
			m_iCurrWnd=0;
		}
		break;
	}
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumSIMV()
{
	switch(m_iCurrWnd)
	{
	case IDC_VIEW_NUM_SIMV_DEFAULT:
		{
			deleteWndNumSIMV_Default();
			m_iCurrWnd=0;
		}
		break;
	case IDC_VIEW_NUM_SIMV_DEFAULTMECH:
		{
			deleteWndNumSIMV_DefaultMech();
			m_iCurrWnd=0;
		}
		break;
	case IDC_VIEW_NUM_SIMV:
		{
			deleteWndNumConfig();
			m_iCurrWnd=0;
		}
		break;
	}
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumSIMVPSV()
{
	switch(m_iCurrWnd)
	{
	case IDC_VIEW_NUM_SIMVPSV_DEFAULT:
		{
			deleteWndNumSIMVPSV_Default();
			m_iCurrWnd=0;
		}
		break;
	case IDC_VIEW_NUM_SIMVPSV_DEFAULTMECH:
		{
			deleteWndNumSIMVPSV_DefaultMech();
			m_iCurrWnd=0;
		}
		break;
	case IDC_VIEW_NUM_SIMVPSV:
		{
			deleteWndNumConfig();
			m_iCurrWnd=0;
		}
		break;
	}
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumCPAP()
{
	switch(m_iCurrWnd)
	{
	case IDC_VIEW_NUM_CPAP_DEFAULT:
		{
			deleteWndNumCPAP_Default();
			m_iCurrWnd=0;
		}
		break;
	case IDC_VIEW_NUM_CPAP_DEFAULTMECH:
		{
			deleteWndNumCPAP_DefaultMech();
			m_iCurrWnd=0;
		}
		break;
	case IDC_VIEW_NUM_CPAP:
		{
			deleteWndNumConfig();
			m_iCurrWnd=0;
		}
		break;
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumHFO()
{
	switch(m_iCurrWnd)
	{
	case IDC_VIEW_NUM_HFO_DEFAULT:
		{
			deleteWndNumHFO_Default();
			m_iCurrWnd=0;
		}
		break;
	case IDC_VIEW_NUM_HFO_DEFAULTMECH:
		{
			deleteWndNumHFO_DefaultMech();
			m_iCurrWnd=0;
		}
		break;
	case IDC_VIEW_NUM_HFO:
		{
			deleteWndNumConfig();
			m_iCurrWnd=0;
		}
		break;
	}
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumTHERAPIE()
{
	deleteWndNumTHERAPIE_Default();
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumDUOPAP()
{
	deleteWndNumDUOPAP_Default();
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::deleteWndNumNCPAP()
{
	deleteWndNumNCPAP_Default();
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowWndNumFlowOff()
{
	if(m_pcMenuNumeric)
		m_pcMenuNumeric->ShowWindow(SW_HIDE);

	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumFlowOff==NULL)
		CreateWndNumFlowOff();
	else
		m_pcWndNumFlowOff->DrawData(true,true,true,true,false);
	LeaveCriticalSection(&csNumWnd);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowWndNumFlowOffHFO()
{
	if(m_pcMenuNumeric)
		m_pcMenuNumeric->ShowWindow(SW_HIDE);

	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumFlowOffHFO==NULL)
		CreateWndNumFlowOffHFO();
	else
		m_pcWndNumFlowOffHFO->DrawData(true,true,true,true,false);
	LeaveCriticalSection(&csNumWnd);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowWndNumHFO()
{
	if(getModel()->getDATAHANDLER()->getCountNumericHFO()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericHFO[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_HFO,&getModel()->getDATAHANDLER()->m_pbufNumericHFO[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	case DEFAULTBLOCK:
		{
			if(m_pcWndNumHFOdefault==NULL)
				CreateWndNumHFO_Default();
			else
				m_pcWndNumHFOdefault->DrawData(true,true,true,true,false);
		}
		break;
	case DEFAULTMECHANICBLOCK:
		{
			if(m_pcWndNumHFOdefaultmech==NULL)
				CreateWndNumHFO_DefaultMech();
			else
				m_pcWndNumHFOdefaultmech->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}


// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowWndNumIPPV()
{
	if(getModel()->getDATAHANDLER()->getCountNumericIPPV()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericIPPV[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_IPPV,&getModel()->getDATAHANDLER()->m_pbufNumericIPPV[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	case DEFAULTBLOCK:
		{
			if(m_pcWndNumIPPVdefault==NULL)
				CreateWndNumIPPV_Default();
			else
				m_pcWndNumIPPVdefault->DrawData(true,true,true,true,false);
		}
		break;
	case DEFAULTMECHANICBLOCK:
		{
			if(m_pcWndNumIPPVdefaultmech==NULL)
				CreateWndNumIPPV_DefaultMech();
			else
				m_pcWndNumIPPVdefaultmech->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowWndNumSIPPV()
{
	if(getModel()->getDATAHANDLER()->getCountNumericSIPPV()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericSIPPV[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_SIPPV,&getModel()->getDATAHANDLER()->m_pbufNumericSIPPV[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	case DEFAULTBLOCK:
		{
			if(m_pcWndNumSIPPVdefault==NULL)
				CreateWndNumSIPPV_Default();
			else
				m_pcWndNumSIPPVdefault->DrawData(true,true,true,true,false);
		}
		break;
	case DEFAULTMECHANICBLOCK:
		{
			if(m_pcWndNumSIPPVdefaultmech==NULL)
				CreateWndNumSIPPV_DefaultMech();
			else
				m_pcWndNumSIPPVdefaultmech->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowWndNumSIMV()
{
	if(getModel()->getDATAHANDLER()->getCountNumericSIMV()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericSIMV[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_SIMV,&getModel()->getDATAHANDLER()->m_pbufNumericSIMV[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	case DEFAULTBLOCK:
		{
			if(m_pcWndNumSIMVdefault==NULL)
				CreateWndNumSIMV_Default();
			else
				m_pcWndNumSIMVdefault->DrawData(true,true,true,true,false);
		}
		break;
	case DEFAULTMECHANICBLOCK:
		{
			if(m_pcWndNumSIMVdefaultmech==NULL)
				CreateWndNumSIMV_DefaultMech();
			else
				m_pcWndNumSIMVdefaultmech->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowWndNumSIMVPSV()
{
	if(getModel()->getDATAHANDLER()->getCountNumericSIMVPSV()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericSIMVPSV[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_SIMVPSV,&getModel()->getDATAHANDLER()->m_pbufNumericSIMVPSV[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	case DEFAULTBLOCK:
		{
			if(m_pcWndNumSIMVPSVdefault==NULL)
				CreateWndNumSIMVPSV_Default();
			else
				m_pcWndNumSIMVPSVdefault->DrawData(true,true,true,true,false);
		}
		break;
	case DEFAULTMECHANICBLOCK:
		{
			if(m_pcWndNumSIMVPSVdefaultmech==NULL)
				CreateWndNumSIMVPSV_DefaultMech();
			else
				m_pcWndNumSIMVPSVdefaultmech->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowWndNumPSV()
{
	if(getModel()->getDATAHANDLER()->getCountNumericPSV()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericPSV[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_PSV,&getModel()->getDATAHANDLER()->m_pbufNumericPSV[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	case DEFAULTBLOCK:
		{
			if(m_pcWndNumPSVdefault==NULL)
				CreateWndNumPSV_Default();
			else
				m_pcWndNumPSVdefault->DrawData(true,true,true,true,false);
		}
		break;
	case DEFAULTMECHANICBLOCK:
		{
			if(m_pcWndNumPSVdefaultmech==NULL)
				CreateWndNumPSV_DefaultMech();
			else
				m_pcWndNumPSVdefaultmech->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowWndNumCPAP()
{
	if(getModel()->getDATAHANDLER()->getCountNumericCPAP()>1)
	{
		CString szText=_T("");
		szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
		m_pcMenuNumeric->RefreshText(szText);
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuNumeric)
			m_pcMenuNumeric->ShowWindow(SW_HIDE);
	}

	BYTE byBlock=getModel()->getDATAHANDLER()->m_pbufNumericCPAP[m_iCurNumericBlock].SHOW;

	EnterCriticalSection(&csNumWnd);
	switch(byBlock)
	{
	case FALSE:
		{

		}
		break;
	case TRUE:
		{
			if(m_pcWndNumConfig==NULL)
				createWndNumConfig(IDC_VIEW_NUM_CPAP,&getModel()->getDATAHANDLER()->m_pbufNumericCPAP[m_iCurNumericBlock]);
			else
				m_pcWndNumConfig->DrawData(true,true,true,true,false);
		}
		break;
	case DEFAULTBLOCK:
		{
			if(m_pcWndNumCPAPdefault==NULL)
				CreateWndNumCPAP_Default();
			else
				m_pcWndNumCPAPdefault->DrawData(true,true,true,true,false);
		}
		break;
	case DEFAULTMECHANICBLOCK:
		{
			if(m_pcWndNumCPAPdefaultmech==NULL)
				CreateWndNumCPAP_DefaultMech();
			else
				m_pcWndNumCPAPdefaultmech->DrawData(true,true,true,true,false);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowWndNumDUOPAP()
{
	if(m_pcMenuNumeric)
		m_pcMenuNumeric->ShowWindow(SW_HIDE);

	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumDUOPAPdefault==NULL)
		CreateWndNumDUOPAP_Default();
	else
		m_pcWndNumDUOPAPdefault->DrawData(true,true,true,true,true);
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowWndNumNCPAP()
{
	if(m_pcMenuNumeric)
		m_pcMenuNumeric->ShowWindow(SW_HIDE);

	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumNCPAPdefault==NULL)
		CreateWndNumNCPAP_Default();
	else
		m_pcWndNumNCPAPdefault->DrawData(true,true,true,true,true);
	LeaveCriticalSection(&csNumWnd);
}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowWndNumTHERAPIE()
{
	if(m_pcMenuNumeric)
		m_pcMenuNumeric->ShowWindow(SW_HIDE);
	
	EnterCriticalSection(&csNumWnd);
	if(m_pcWndNumTHERAPIEdefault==NULL)
		CreateWndNumTHERAPIE_Default();
	else
		m_pcWndNumTHERAPIEdefault->DrawData(true,true,true,true,true);
	LeaveCriticalSection(&csNumWnd);
}


// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::HideNumWnd()
{
	switch(m_iCurrWnd)
	{
	case IDC_VIEW_NUM_FLOWOFF:
		{
			deleteWndNumFlowOff();
		}
		break;
	case IDC_VIEW_NUM_FLOWOFF_HFO:
		{
			deleteWndNumFlowOffHFO();
		}
		break;
	case IDC_VIEW_NUM_IPPV_DEFAULT:
	case IDC_VIEW_NUM_IPPV_DEFAULTMECH:
	case IDC_VIEW_NUM_IPPV:
		{
			deleteWndNumIPPV();
		}
		break;
	case IDC_VIEW_NUM_SIPPV_DEFAULT:
	case IDC_VIEW_NUM_SIPPV_DEFAULTMECH:
	case IDC_VIEW_NUM_SIPPV:
		{
			deleteWndNumSIPPV();
		}
		break;
	case IDC_VIEW_NUM_SIMV_DEFAULT:
	case IDC_VIEW_NUM_SIMV_DEFAULTMECH:
	case IDC_VIEW_NUM_SIMV:
		{
			deleteWndNumSIMV();
		}
		break;
	case IDC_VIEW_NUM_SIMVPSV_DEFAULT:
	case IDC_VIEW_NUM_SIMVPSV_DEFAULTMECH:
	case IDC_VIEW_NUM_SIMVPSV:
		{
			deleteWndNumSIMVPSV();
		}
		break;
	case IDC_VIEW_NUM_PSV_DEFAULT:
	case IDC_VIEW_NUM_PSV_DEFAULTMECH:
	case IDC_VIEW_NUM_PSV:
		{
			deleteWndNumPSV();
		}
		break;
	case IDC_VIEW_NUM_CPAP_DEFAULT:
	case IDC_VIEW_NUM_CPAP_DEFAULTMECH:
	case IDC_VIEW_NUM_CPAP:
		{
			deleteWndNumCPAP();
		}
		break;
	case IDC_VIEW_NUM_HFO_DEFAULT:
	case IDC_VIEW_NUM_HFO_DEFAULTMECH:
	case IDC_VIEW_NUM_HFO:
		{
			deleteWndNumHFO();
		}
		break;
	case IDC_VIEW_NUM_DUOPAP:
		{
			deleteWndNumDUOPAP();
		}
		break;
	case IDC_VIEW_NUM_NCPAP:
		{
			deleteWndNumNCPAP();
		}
		break;
	case IDC_VIEW_NUM_THERAPIE:
		{
			deleteWndNumTHERAPIE();
		}
		break;
	default:
		break;
	}
}
#define DIV 1024
// #define DIV 1

const char *divisor = "K";
// char *divisor = "";
#define WIDTH 7

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowNumWnd(eNumBlock eBlock,bool bChangeCurNumBlock)
{
	if(m_eNumBlock!=eBlock || bChangeCurNumBlock)
		HideNumWnd();

	m_eNumBlock=eBlock;
	switch(eBlock)
	{
	case NUMB_FLOWOFF: 
		{
			ShowWndNumFlowOff();
		}
		break;
	case NUMB_FLOWOFF_HFO: 
		{
			ShowWndNumFlowOffHFO();
		}
		break;
	case NUMB_IPPV: 
		{
			ShowWndNumIPPV();
		}
		break;
	case NUMB_SIPPV: 
		{
			ShowWndNumSIPPV();
		}
		break;
	case NUMB_SIMV: 
		{
			ShowWndNumSIMV();
		}
		break;
	case NUMB_SIMVPSV: 
		{
			ShowWndNumSIMVPSV();
		}
		break;
	case NUMB_PSV: 
		{
			ShowWndNumPSV();
		}
		break;
	case NUMB_CPAP:
		{
			ShowWndNumCPAP();
		}
		break;
	case NUMB_DUOPAP:
		{
			ShowWndNumDUOPAP();
		}
		break;
	case NUMB_NCPAP: 
		{
			ShowWndNumNCPAP();
		}
		break;
	case NUMB_THERAPIE: 
		{
			ShowWndNumTHERAPIE();
		}
		break;
	case NUMB_HFO: 
		{
			ShowWndNumHFO();
		}
		break;
	default:
		{
		}
		break;
	}

	

	
	if(getModel()->getVIEWHANDLER()->getViewState()==VS_TREND)
	{
		DeleteGraphCursor();
	}

}



// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::SetNumericBlock(bool bChangeCurNumBlock)
{
	eNumBlock eBlock=NUMB_IPPV;

	eVentMode eMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	switch(eMode)
	{
	case VM_IPPV:
		{
			if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{
				eBlock=NUMB_IPPV;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericIPPV();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericIPPV();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericIPPV(m_iCurNumericBlock);
				}
			}
			else
			{
				eBlock=NUMB_FLOWOFF;
			}
		}
		break;
	case VM_SIMV:
		{
			if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{
				eBlock=NUMB_SIMV;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericSIMV();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericSIMV();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericSIMV(m_iCurNumericBlock);
				}
			}
			else
			{
				eBlock=NUMB_FLOWOFF;
			}
			
		}
		break;
	case VM_SIMVPSV:
		{
			if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{
				eBlock=NUMB_SIMVPSV;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericSIMVPSV();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericSIMVPSV();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericSIMVPSV(m_iCurNumericBlock);
				}
			}
			else
			{
				eBlock=NUMB_FLOWOFF;
			}
			
		}
		break;
	case VM_SIPPV:
		{
			if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{
				eBlock=NUMB_SIPPV;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericSIPPV();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericSIPPV();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericSIPPV(m_iCurNumericBlock);
				}
			}
			else
			{
				eBlock=NUMB_FLOWOFF;
			}
		}
		break;
	case VM_PSV:
		{
			if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{
				eBlock=NUMB_PSV;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericPSV();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericPSV();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericPSV(m_iCurNumericBlock);
				}
			}
			else
			{
				eBlock=NUMB_FLOWOFF;
			}
		}
		
		break;
	case VM_CPAP:
		{
			if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{
				eBlock=NUMB_CPAP;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericCPAP();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericCPAP();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericCPAP(m_iCurNumericBlock);
				}
			}
			else
			{
				eBlock=NUMB_FLOWOFF;
			}
			
		}
		break;
	case VM_HFO:
		{
			if(getModel()->getDATAHANDLER()->IsFlowSensorStateOff()==false)
			{
				eBlock=NUMB_HFO;
				m_iCurNumericBlock=getModel()->getCONFIG()->getLastNumericHFO();
				m_iCountNumericBlock=getModel()->getDATAHANDLER()->getCountNumericHFO();

				if(m_iCurNumericBlock>m_iCountNumericBlock-1)
				{
					m_iCurNumericBlock=0;
					getModel()->getCONFIG()->setLastNumericHFO(m_iCurNumericBlock);
				}
			}
			else
			{
				eBlock=NUMB_FLOWOFF_HFO;
			}

		}
		break;
	case VM_NCPAP:
		{
			eBlock=NUMB_NCPAP;
		}
		break;
	case VM_DUOPAP:
		{
			eBlock=NUMB_DUOPAP;
		}
		break;
	case VM_THERAPIE:
		{
			eBlock=NUMB_THERAPIE;
		}
		break;
	default:
		{
			return;
		}
		break;
	}

	

	ShowNumWnd(eBlock,bChangeCurNumBlock);
}


// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::NotifyEvent(CMVEvent* pEvent)
{
	if(m_bExit)
		return;

	switch(pEvent->GetET())
	{
	case CMVEvent::ET_CONTROL:
		{
			CMVEventControl* pCtrlEvent = (CMVEventControl*)pEvent;
			switch(pCtrlEvent->GetEventType())
			{
			case CMVEventControl::EV_CONTROL_FLOWSENSORSTATE:
			case CMVEventControl::EV_CONTROL_O2SENSORSTATE:
				{
					try
					{
						SetNumericBlock(false);
					}
					catch (...)
					{
						CFabianHFOApp::ReportException(_T("EXCEPTION: CViewNumericWnd::NotifyEvent()"));
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
	case CMVEvent::ET_UIEVENT:
		{
			CMVEventUI* pUIEvent = (CMVEventUI*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventUI::EV_BN_NUMERIC:
				{
					ShowNextNumericWnd();
				}
				break;
			//case CMVEventUI::EV_BN_GRAPH_FREEZED:
			//	{
			//		if(		getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS
			//			&&	getModel()->getCONFIG()->GetCurMode()!=VM_CPAP
			//			&&	getModel()->getCONFIG()->GetCurMode()!=VM_NCPAP
			//			&&	getModel()->getCONFIG()->GetCurMode()!=VM_DUOPAP
			//			&&	getModel()->getCONFIG()->GetCurMode()!=VM_THERAPIE)
			//		{
			//			try
			//			{
			//				ShowNumWnd(IDC_VIEW_NUM_BREATH);
			//			}
			//			catch (...)
			//			{
			//				CFabianHFOApp::ReportException(_T("EXCEPTION: CViewNumericWnd::NotifyEvent() EV_BN_GRAPH_FREEZED"));
			//				//AfxMessageBox( _T("EXCEPTION: CViewNumericWnd::NotifyEvent() EV_BN_GRAPH_FREEZED") );
			//				
			//			}
			//			
			//		}

			//	}
			//	break;
			//case CMVEventUI::EV_BN_GRAPH_CONTINUE:
			//	{
			//		if(		getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS
			//			&&	getModel()->getCONFIG()->GetCurMode()!=VM_CPAP
			//			&&	getModel()->getCONFIG()->GetCurMode()!=VM_NCPAP
			//			&&	getModel()->getCONFIG()->GetCurMode()!=VM_DUOPAP
			//			&&	getModel()->getCONFIG()->GetCurMode()!=VM_THERAPIE)
			//		{
			//			try
			//			{
			//				SetNumericBlock();
			//			}
			//			catch (...)
			//			{
			//				CFabianHFOApp::ReportException(_T("EXCEPTION: CViewNumericWnd::NotifyEvent() EV_BN_GRAPH_CONTINUE"));
			//				//AfxMessageBox( _T("EXCEPTION: CViewNumericWnd::NotifyEvent() EV_BN_GRAPH_CONTINUE") );
			//				
			//			}
			//			
			//		}

			//	}
			//	break;
			//case CMVEventUI::EV_BN_GRAPH_SAVE:
			//	{
			//		try
			//		{
			//			ShowNumWnd(IDC_VIEW_NUM_BREATH);
			//		}
			//		catch (...)
			//		{
			//			CFabianHFOApp::ReportException(_T("EXCEPTION: CViewNumericWnd::NotifyEvent() EV_BN_GRAPH_SAVE"));
			//			//AfxMessageBox( _T("EXCEPTION: CViewNumericWnd::NotifyEvent() EV_BN_GRAPH_SAVE") );
			//			
			//		}
			//		
			//	}
			//	break;
			case CMVEventUI::EV_GRAPH_SETCURSOR:
				{
					DrawGraphCursor(pUIEvent->GetIData());
				}
				break;
			case CMVEventUI::EV_GRAPH_DELCURSOR:
				{
					DeleteGraphCursor();
				}
				break;
			default:
				break;
			}
		}
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::ShowNextNumericWnd()
{

}
// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::DrawGraphCursor(int iDiagrmm)
{
	
	//m_bDrawGraphCursor=true;

	bool bMoveXscale=false;
	bool bMoveYscale=false;

	if(getModel()->getVIEWHANDLER()->getViewState()==VS_TREND)
	{

	}
	else
	{
		if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
		{
			switch(iDiagrmm)
			{
			case IDC_LINEDIAGRAM_PRESSURE:
				{
					bMoveXscale=true;
				}
				break;
			case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
				{
					bMoveYscale=true;
				}
				break;
			}
		}
	}


	try
	{
		EnterCriticalSection(&csNumWnd);
		if(m_pcWndNumHFOdefault)
		{
			m_pcWndNumHFOdefault->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumHFOdefaultmech)
		{
			m_pcWndNumHFOdefaultmech->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumDUOPAPdefault)
		{
			m_pcWndNumDUOPAPdefault->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumFlowOff)
		{
			m_pcWndNumFlowOff->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumFlowOffHFO)
		{
			m_pcWndNumFlowOffHFO->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumCPAPdefault)
		{
			m_pcWndNumCPAPdefault->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumCPAPdefaultmech)
		{
			m_pcWndNumCPAPdefaultmech->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumNCPAPdefault)
		{
			m_pcWndNumNCPAPdefault->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumIPPVdefault)
		{
			m_pcWndNumIPPVdefault->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumIPPVdefaultmech)
		{
			m_pcWndNumIPPVdefaultmech->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumSIMVPSVdefaultmech)
		{
			m_pcWndNumSIMVPSVdefaultmech->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumSIMVPSVdefault)
		{
			m_pcWndNumSIMVPSVdefault->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumPSVdefaultmech)
		{
			m_pcWndNumPSVdefaultmech->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumPSVdefault)
		{
			m_pcWndNumPSVdefault->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumSIPPVdefault)
		{
			m_pcWndNumSIPPVdefault->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumSIPPVdefaultmech)
		{
			m_pcWndNumSIPPVdefaultmech->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumSIMVdefault)
		{
			m_pcWndNumSIMVdefault->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumSIMVdefaultmech)
		{
			m_pcWndNumSIMVdefaultmech->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumTHERAPIEdefault)
		{
			m_pcWndNumTHERAPIEdefault->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		else if(m_pcWndNumConfig)
		{
			m_pcWndNumConfig->SetGraphCursor(true, bMoveXscale, bMoveYscale);
		}
		LeaveCriticalSection(&csNumWnd);
	}
	catch (...)
	{
		CFabianHFOApp::ReportException(_T("EXCEPTION: CViewNumericWnd::NotifyEvent() EV_GRAPH_SETCURSOR"));
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::DeleteGraphCursor()
{
	try
	{
		EnterCriticalSection(&csNumWnd);
		if(m_pcWndNumHFOdefault)
		{
			m_pcWndNumHFOdefault->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumHFOdefaultmech)
		{
			m_pcWndNumHFOdefaultmech->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumDUOPAPdefault)
		{
			m_pcWndNumDUOPAPdefault->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumFlowOff)
		{
			m_pcWndNumFlowOff->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumFlowOffHFO)
		{
			m_pcWndNumFlowOffHFO->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumCPAPdefault)
		{
			m_pcWndNumCPAPdefault->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumCPAPdefaultmech)
		{
			m_pcWndNumCPAPdefaultmech->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumNCPAPdefault)
		{
			m_pcWndNumNCPAPdefault->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumIPPVdefault)
		{
			m_pcWndNumIPPVdefault->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumIPPVdefaultmech)
		{
			m_pcWndNumIPPVdefaultmech->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumPSVdefault)
		{
			m_pcWndNumPSVdefault->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumPSVdefaultmech)
		{
			m_pcWndNumPSVdefaultmech->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumSIMVPSVdefault)
		{
			m_pcWndNumSIMVPSVdefault->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumSIMVPSVdefaultmech)
		{
			m_pcWndNumSIMVPSVdefaultmech->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumSIPPVdefault)
		{
			m_pcWndNumSIPPVdefault->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumSIPPVdefaultmech)
		{
			m_pcWndNumSIPPVdefaultmech->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumSIMVdefault)
		{
			m_pcWndNumSIMVdefault->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumSIMVdefaultmech)
		{
			m_pcWndNumSIMVdefaultmech->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumTHERAPIEdefault)
		{
			m_pcWndNumTHERAPIEdefault->SetGraphCursor(false,false,false);
		}
		else if(m_pcWndNumConfig)
		{
			m_pcWndNumConfig->SetGraphCursor(false,false,false);
		}
		LeaveCriticalSection(&csNumWnd);
	}
	catch (...)
	{
		CFabianHFOApp::ReportException(_T("EXCEPTION: CViewNumericWnd::NotifyEvent() EV_GRAPH_DELCURSOR"));
	}
}


// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::NotifyParaBtnEvent(CMVEvent* pEvent)
{
	if(m_bExit)
		return;

	switch(pEvent->GetET())
	{
	case CMVEvent::ET_UIEVENT:
		{
			CMVEventUI* pUIEvent = (CMVEventUI*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventUI::EV_PARABN_PMITT:
			case CMVEventUI::EV_PARABN_IERATIO:
			case CMVEventUI::EV_PARABN_HFFREQREC:
				{
					bool bChanged=false;
					if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
					{
						bChanged=true;
					}
					if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
					{
						bChanged=true;
					}
					if(bChanged)
					{	
						PostMessage(WM_ALIMIT_CHANGED);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_HFAMPL:
				{
					bool bChanged=false;
					if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
					{
						bChanged=true;
					}
					if(true==getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
					{
						bChanged=true;
					}
					else if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
					{
						bChanged=true;
					}
					if(bChanged)
					{	
						PostMessage(WM_ALIMIT_CHANGED);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_PEEP:
				{
					if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
					{
						PostMessage(WM_ALIMIT_CHANGED);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_CPAP:
				{
					bool bLimitChanged=false;
					if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
					{
						//getModel()->GetDataHandler()->CalculateAutoAlarmlimitPEEPmin();xxxxxxxxxx
						bLimitChanged=true;
					}
					if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
					{
						bLimitChanged=true;
					}
					if(bLimitChanged)
					{
						PostMessage(WM_ALIMIT_CHANGED);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_PPSV:
			case CMVEventUI::EV_PARABN_PINSP:
			case CMVEventUI::EV_PARABN_PMAXVOLG:
				{
					bool bLimitChanged=false;
					if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
					{
						bLimitChanged=true;
					}
					if(getModel()->getALARMHANDLER()->getAlimitState_PIPminLimit()==AL_AUTO)
					{
						bLimitChanged=true;
					}
					if(bLimitChanged)
					{
						PostMessage(WM_ALIMIT_CHANGED);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_PDUO:
				{
					if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
					{
						PostMessage(WM_ALIMIT_CHANGED);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_PMAN:
				{
					if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
					{
						PostMessage(WM_ALIMIT_CHANGED);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_FOT:
				{
					bool bChanged=false;
					if(getModel()->getALARMHANDLER()->getAlimitState_PEEPminLimit()==AL_AUTO)
					{
						bChanged=true;
					}
					if(true==getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
					{
						bChanged=true;
					}
					else if(getModel()->getALARMHANDLER()->getAlimitState_PIPmaxLimit()==AL_AUTO)
					{
						bChanged=true;
					}
					if(bChanged)
					{	
						PostMessage(WM_ALIMIT_CHANGED);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_IFLOW:	
			case CMVEventUI::EV_PARABN_EFLOW:
			case CMVEventUI::EV_PARABN_VLIMIT:
			case CMVEventUI::EV_PARABN_TRIGGER:
			case CMVEventUI::EV_PARABN_BPM:
			case CMVEventUI::EV_PARABN_ITIME:
			case CMVEventUI::EV_PARABN_ETIME:
			case CMVEventUI::EV_PARABN_BACKUP:
			case CMVEventUI::EV_PARABN_O2:
			case CMVEventUI::EV_PARABN_MINFLOW:
			case CMVEventUI::EV_PARABN_VGARANT:
			case CMVEventUI::EV_PARABN_O2FLUSH:
			case CMVEventUI::EV_PARABN_HFFREQ:
			//case CMVEventUI::EV_PARABN_HFAMPL:
			//case CMVEventUI::EV_PARABN_IERATIO:
			default:
				break;
			}
		}

	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::NotifyViewStateChanged()
{
	if(m_bExit)
		return;

	switch(getModel()->getVIEWHANDLER()->getViewState())
	{
	case VS_SETUP:
		{
			m_bBackState=true;
		}
		break;
	case VS_PARA:
		{
			if(getModel()->getVIEWHANDLER()->getPrevViewState()==VS_SETUP)
			{
				if(m_bBackState)
				{
					m_bBackState=false;
				}

				break;
			}

			if(m_bBackState)
			{
				m_bBackState=false;
			}

			try
			{
				//SetNumericBlock();
			}
			catch (...)
			{
				CFabianHFOApp::ReportException(_T("EXCEPTION: CViewNumericWnd::NotifyViewStateChanged() VS_PARA"));
			}
		}
		break;
	case VS_GRAPH:
	case VS_TREND:
		{
			try
			{
				//SetNumericBlock();
			}
			catch (...)
			{
				CFabianHFOApp::ReportException(_T("EXCEPTION: CViewNumericWnd::NotifyViewStateChanged() VS_GRAPH"));
			}
			
		}
		break;
	/*case VS_TREND:
		{
			
		}
		break;*/
	default:
		{
			if(m_bBackState)
			{
				m_bBackState=false;
			}
		}
		break;
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::NotifyVentModeChanged()
{
	if(m_bExit)
		return;

	switch(getModel()->getVIEWHANDLER()->getViewState())
	{
	//case CMVModel::VS_NUMERIC:
	//	{
	//	}
	//	break;
	case VS_PARA:
		{
			if(false==getModel()->getCONFIG()->CurModeIsPresetMode())
			{
				try
				{
					SetNumericBlock(false);
				}
				catch (...)
				{
					CFabianHFOApp::ReportException(_T("EXCEPTION: CViewNumericWnd::NotifyVentModeChanged()"));
				}
			}
		}
		break;
	default:
		{
		}
		break;
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::CalculateAlarmLimit()
{
	if(m_bExit)
		return;

	redrawNumerics(true,true,true,true,false);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::NotifyCalculateAlarmLimit()
{
	if(m_bExit)
		return;

	try
	{
		PostMessage(WM_CALCULATE_ALARMLIMIT);
	}
	catch (...)
	{
		CFabianHFOApp::ReportException(_T("EXCEPTION: CViewNumericWnd::NotifyCalculateAlarmLimit()"));
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::RedrawAlarmLimits()
{
	if(m_bExit)
		return;

	redrawNumerics(true,true,true,true,false);
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::NotifyAlarmLimitChanged()
{
	if(m_bExit)
		return;
	
	try
	{
		PostMessage(WM_ALIMIT_CHANGED);
	}
	catch (...)
	{
		CFabianHFOApp::ReportException(_T("EXCEPTION: CViewNumericWnd::NotifyAlarmLimitChanged()"));
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::NotifyDataChanged()
{
	if(m_bExit)
		return;
	
	try
	{
		PostMessage(WM_DATA_CHANGED);
	}
	catch (...)
	{
		CFabianHFOApp::ReportException(_T("EXCEPTION: CViewNumericWnd::NotifyDataChanged()"));
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewNumericWnd::NotifyExspirationDataChanged()
{
	if(m_bExit)
		return;

	try
	{
		PostMessage(WM_EXSPIRATION);
	}
	catch (...)
	{
		CFabianHFOApp::ReportException(_T("EXCEPTION: CViewAlarmLimit::NotifyExspirationDataChanged()"));
	}
}

// **************************************************************************
//
// 
//
// **************************************************************************
LRESULT CViewNumericWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
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
		break;
	case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_MENU_NUMERIC:
				{
					BnMenuNUMERIC();
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
	case WM_CALCULATE_ALARMLIMIT:
		{
			CalculateAlarmLimit();
			return 1;
		}
		break;
	case WM_ALIMIT_CHANGED:
		{
			RedrawAlarmLimits();
			return 1;
		}
		break;
	case WM_EXSPIRATION:
		{
			redrawNumerics(true,false, false, false,false);
			return 1;
		}
		break;
	case WM_DATA_CHANGED:
		{
			redrawNumerics(true,false, false, false,true);
			return 1;
		}
		break;
	default:
		{
		}
		break;
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}

void CViewNumericWnd::redrawNumerics(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter)
{
	EnterCriticalSection(&csNumWnd);
	switch(m_iCurrWnd)
	{
	case IDC_VIEW_NUM_HFO:
	case IDC_VIEW_NUM_CPAP:
	case IDC_VIEW_NUM_PSV:
	case IDC_VIEW_NUM_SIMVPSV:
	case IDC_VIEW_NUM_SIMV:
	case IDC_VIEW_NUM_SIPPV:
	case IDC_VIEW_NUM_IPPV:
		{
			if(m_pcWndNumConfig)
				m_pcWndNumConfig->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	case IDC_VIEW_NUM_IPPV_DEFAULT:
		{
			if(m_pcWndNumIPPVdefault)
				m_pcWndNumIPPVdefault->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	case IDC_VIEW_NUM_IPPV_DEFAULTMECH:
		{
			if(m_pcWndNumIPPVdefaultmech)
				m_pcWndNumIPPVdefaultmech->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	case IDC_VIEW_NUM_SIPPV_DEFAULT:
		{
			if(m_pcWndNumSIPPVdefault)
				m_pcWndNumSIPPVdefault->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	case IDC_VIEW_NUM_SIPPV_DEFAULTMECH:
		{
			if(m_pcWndNumSIPPVdefaultmech)
				m_pcWndNumSIPPVdefaultmech->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	case IDC_VIEW_NUM_SIMV_DEFAULT:
		{
			if(m_pcWndNumSIMVdefault)
				m_pcWndNumSIMVdefault->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	case IDC_VIEW_NUM_SIMV_DEFAULTMECH:
		{
			if(m_pcWndNumSIMVdefaultmech)
				m_pcWndNumSIMVdefaultmech->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	case IDC_VIEW_NUM_SIMVPSV_DEFAULT:
		{
			if(m_pcWndNumSIMVPSVdefault)
				m_pcWndNumSIMVPSVdefault->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	case IDC_VIEW_NUM_SIMVPSV_DEFAULTMECH:
		{
			if(m_pcWndNumSIMVPSVdefaultmech)
				m_pcWndNumSIMVPSVdefaultmech->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	case IDC_VIEW_NUM_PSV_DEFAULT:
		{
			if(m_pcWndNumPSVdefault)
				m_pcWndNumPSVdefault->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	case IDC_VIEW_NUM_PSV_DEFAULTMECH:
		{
			if(m_pcWndNumPSVdefaultmech)
				m_pcWndNumPSVdefaultmech->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	case IDC_VIEW_NUM_CPAP_DEFAULT:
		{
			if(m_pcWndNumCPAPdefault)
				m_pcWndNumCPAPdefault->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	case IDC_VIEW_NUM_CPAP_DEFAULTMECH:
		{
			if(m_pcWndNumCPAPdefaultmech)
				m_pcWndNumCPAPdefaultmech->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	case IDC_VIEW_NUM_HFO_DEFAULT:
		{
			if(m_pcWndNumHFOdefault)
				m_pcWndNumHFOdefault->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	case IDC_VIEW_NUM_HFO_DEFAULTMECH:
		{
			if(m_pcWndNumHFOdefaultmech)
				m_pcWndNumHFOdefaultmech->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	case IDC_VIEW_NUM_NCPAP:
		{
			if(m_pcWndNumNCPAPdefault)
				m_pcWndNumNCPAPdefault->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	case IDC_VIEW_NUM_DUOPAP:
		{
			if(m_pcWndNumDUOPAPdefault)
				m_pcWndNumDUOPAPdefault->DrawData(bData, bFrames, bText, bLimits, true);
		}
		break;
	case IDC_VIEW_NUM_THERAPIE:
		{
			if(m_pcWndNumTHERAPIEdefault)
				m_pcWndNumTHERAPIEdefault->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	case IDC_VIEW_NUM_FLOWOFF:
		{
			if(m_pcWndNumFlowOff)
				m_pcWndNumFlowOff->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	case IDC_VIEW_NUM_FLOWOFF_HFO:
		{
			if(m_pcWndNumFlowOffHFO)
				m_pcWndNumFlowOffHFO->DrawData(bData, bFrames, bText, bLimits, bFlowmeter);
		}
		break;
	}
	LeaveCriticalSection(&csNumWnd);
}


//************************************
// Method:    OnDestroy
// FullName:  CViewNumericWnd::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CViewNumericWnd::OnDestroy()
{
	m_bExit=true;
	
	switch(m_iCurrWnd)
	{
	case IDC_VIEW_NUM_IPPV_DEFAULT:
	case IDC_VIEW_NUM_IPPV_DEFAULTMECH:
	case IDC_VIEW_NUM_IPPV:
		{
			deleteWndNumIPPV();
		}
		break;
	case IDC_VIEW_NUM_SIPPV_DEFAULT:
	case IDC_VIEW_NUM_SIPPV_DEFAULTMECH:
	case IDC_VIEW_NUM_SIPPV:
		{
			deleteWndNumSIPPV();
		}
		break;
	case IDC_VIEW_NUM_SIMV_DEFAULT:
	case IDC_VIEW_NUM_SIMV_DEFAULTMECH:
	case IDC_VIEW_NUM_SIMV:
		{
			deleteWndNumSIMV();
		}
		break;
	case IDC_VIEW_NUM_SIMVPSV_DEFAULT:
	case IDC_VIEW_NUM_SIMVPSV_DEFAULTMECH:
	case IDC_VIEW_NUM_SIMVPSV:
		{
			deleteWndNumSIMVPSV();
		}
		break;
	case IDC_VIEW_NUM_PSV_DEFAULT:
	case IDC_VIEW_NUM_PSV_DEFAULTMECH:
	case IDC_VIEW_NUM_PSV:
		{
			deleteWndNumPSV();
		}
		break;
	case IDC_VIEW_NUM_CPAP_DEFAULT:
	case IDC_VIEW_NUM_CPAP_DEFAULTMECH:
	case IDC_VIEW_NUM_CPAP:
		{
			deleteWndNumCPAP();
		}
		break;
	case IDC_VIEW_NUM_HFO_DEFAULT:
	case IDC_VIEW_NUM_HFO_DEFAULTMECH:
	case IDC_VIEW_NUM_HFO:
		{
			deleteWndNumHFO();
		}
		break;
	case IDC_VIEW_NUM_NCPAP:
		{
			deleteWndNumNCPAP();
		}
		break;
	case IDC_VIEW_NUM_DUOPAP:
		{
			deleteWndNumDUOPAP();
		}
		break;
	case IDC_VIEW_NUM_THERAPIE:
		{
			deleteWndNumTHERAPIE();
		}
		break;
	case IDC_VIEW_NUM_FLOWOFF:
		{
			deleteWndNumFlowOff();
		}
		break;
	case IDC_VIEW_NUM_FLOWOFF_HFO:
		{
			deleteWndNumFlowOffHFO();
		}
		break;
	}

	m_iCurrWnd=0;

	CMVView::OnDestroy();
}


void CViewNumericWnd::BnMenuNUMERIC()
{
	bool bChangeCurNumBlock=false;
	CString szText=_T("");
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_DEL_GRAPHCURSOR);

	switch(m_iCurrWnd)
	{
	case IDC_VIEW_NUM_IPPV_DEFAULT:
	case IDC_VIEW_NUM_IPPV_DEFAULTMECH:
	case IDC_VIEW_NUM_IPPV:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericIPPV())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericIPPV(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_SIPPV_DEFAULT:
	case IDC_VIEW_NUM_SIPPV_DEFAULTMECH:
	case IDC_VIEW_NUM_SIPPV:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericSIPPV())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericSIPPV(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_SIMV_DEFAULT:
	case IDC_VIEW_NUM_SIMV_DEFAULTMECH:
	case IDC_VIEW_NUM_SIMV:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericSIMV())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericSIMV(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_SIMVPSV_DEFAULT:
	case IDC_VIEW_NUM_SIMVPSV_DEFAULTMECH:
	case IDC_VIEW_NUM_SIMVPSV:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericSIMVPSV())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericSIMVPSV(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_PSV_DEFAULT:
	case IDC_VIEW_NUM_PSV_DEFAULTMECH:
	case IDC_VIEW_NUM_PSV:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericPSV())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericPSV(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_CPAP_DEFAULT:
	case IDC_VIEW_NUM_CPAP_DEFAULTMECH:
	case IDC_VIEW_NUM_CPAP:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericCPAP())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericCPAP(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_HFO_DEFAULT:
	case IDC_VIEW_NUM_HFO_DEFAULTMECH:
	case IDC_VIEW_NUM_HFO:
		{
			m_iCurNumericBlock++;
			if(m_iCurNumericBlock>=getModel()->getDATAHANDLER()->getCountNumericHFO())
			{
				m_iCurNumericBlock=0;
			}
			getModel()->getCONFIG()->setLastNumericHFO(m_iCurNumericBlock);
			szText.Format(_T("  %d/%d"),m_iCurNumericBlock+1,m_iCountNumericBlock);
			m_pcMenuNumeric->RefreshText(szText);
			bChangeCurNumBlock=true;
		}
		break;
	case IDC_VIEW_NUM_NCPAP:
		{
			
		}
		break;
	case IDC_VIEW_NUM_DUOPAP:
		{
			
		}
		break;
	case IDC_VIEW_NUM_THERAPIE:
		{
			
		}
		break;
	case IDC_VIEW_NUM_FLOWOFF:
		{
			
		}
		break;
	case IDC_VIEW_NUM_FLOWOFF_HFO:
		{
			
		}
		break;
	}
	SetNumericBlock(bChangeCurNumBlock);

	AfxGetApp()->GetMainWnd()->SetFocus();
}

