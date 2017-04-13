#include "StdAfx.h"
#include "WndMenuTrendtype.h"
#include "FabianHFO.h"
#include "LangAdmin.h"
#include "globDefs.h"



#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000


//global font objects
//extern HFONT g_hf15AcuMed;

//extern CLangAdmin* g_pGlobalLanguageStrings;



// CWndMenuTrendtype

IMPLEMENT_DYNAMIC(CWndMenuTrendtype, CWnd)

CWndMenuTrendtype::CWndMenuTrendtype(CMVView *parentView)
{
	m_bPopupBtn=false;
	m_parentView=parentView;

	m_iCurBtnSelTrend=IDC_BTN_SELTREND_1;

	m_lX				= 0;
	m_lY				= 0;
	m_lXo				= 0;
	m_lYo				= 0;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;

	m_pcMenu_Up=NULL;
	m_pcMenu_Dw=NULL;
	
	m_pcTrendPINSP=NULL;
	m_pcTrendPMEAN=NULL;
	m_pcTrendFIO2=NULL;
	m_pcTrendVTE=NULL;
	m_pcTrendCOMPLIANCE=NULL;
	m_pcTrendCO2HFO=NULL;
	m_pcTrendMV=NULL;
	m_pcTrendHFAMP=NULL;
	m_pcTrendRSBI=NULL;
	m_pcTrendSHAREMVMAND=NULL;
	m_pcTrendRESISTANCE=NULL;
	m_pcTrendLEAK=NULL;
	m_pcTrendSPO2=NULL;
	m_pcTrendSPO2PI=NULL;
	m_pcTrendSPO2PR=NULL;
	m_pcTrendETCO2=NULL;
	m_pcTrendFREQUENCY=NULL;

	m_pModel = NULL;
}

CWndMenuTrendtype::~CWndMenuTrendtype()
{
	delete m_pcTrendPINSP;
	m_pcTrendPINSP=NULL;
	delete m_pcTrendPMEAN;
	m_pcTrendPMEAN=NULL;
	delete m_pcTrendFIO2;
	m_pcTrendFIO2=NULL;
	delete m_pcTrendVTE;
	m_pcTrendVTE=NULL;
	delete m_pcTrendCOMPLIANCE;
	m_pcTrendCOMPLIANCE=NULL;
	delete m_pcTrendCO2HFO;
	m_pcTrendCO2HFO=NULL;
	delete m_pcTrendMV;
	m_pcTrendMV=NULL;
	delete m_pcTrendHFAMP;
	m_pcTrendHFAMP=NULL;


	delete m_pcTrendRSBI;
	m_pcTrendRSBI=NULL;
	delete m_pcTrendSHAREMVMAND;
	m_pcTrendSHAREMVMAND=NULL;
	delete m_pcTrendRESISTANCE;
	m_pcTrendRESISTANCE=NULL;
	delete m_pcTrendLEAK;
	m_pcTrendLEAK=NULL;
	delete m_pcTrendSPO2;
	m_pcTrendSPO2=NULL;
	delete m_pcTrendSPO2PI;
	m_pcTrendSPO2PI=NULL;
	delete m_pcTrendETCO2;
	m_pcTrendETCO2=NULL;
	delete m_pcTrendSPO2PR;
	m_pcTrendSPO2PR=NULL;
	delete m_pcTrendFREQUENCY;
	m_pcTrendFREQUENCY=NULL;

	delete m_pcMenu_Up;
	m_pcMenu_Up=NULL;
	delete m_pcMenu_Dw;
	m_pcMenu_Dw=NULL;
}

// **************************************************************************
// 
// **************************************************************************
CMVModel *CWndMenuTrendtype::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


BEGIN_MESSAGE_MAP(CWndMenuTrendtype, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//ON_WM_KILLFOCUS()
	//ON_WM_SETFOCUS()
END_MESSAGE_MAP()



// CWndMenuTrendtype message handlers
// **************************************************************************
// 
// **************************************************************************
BOOL CWndMenuTrendtype::Create(CWnd* pParentWnd, const RECT rc, UINT nID, UINT bSelTrend) 
{
	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	m_iCurBtnSelTrend=bSelTrend;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::CreateEx(WS_EX_TOPMOST/*|WS_EX_DLGMODALFRAME*//*|WS_EX_CLIENTEDGE*/,lpszClassName,_T("Menu"),WS_POPUP/*|WS_VISIBLE*/ ,rc,pParentWnd,nID))
	{
		CClientDC dc(this);

		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
		m_hBmpPrev=(HBITMAP)SelectObject(m_hDC,m_hBmp);

		CBrush cbrBack(RGB(181,178,181));
		//CBrush cbrBack(RGB(3,230,3));
		//CBrush cbrBack(BACKGND);
		
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);
		CPen* pTempPen = NULL;
		//CPen origPen;
		CPen pen(PS_SOLID,1,BLACK);

		CDC* pDC=CDC::FromHandle(m_hDC);
		pTempPen = (CPen*)pDC->SelectObject(&pen);

		//origPen.FromHandle((HPEN)pTempPen);

		pDC->MoveTo(0 , 0);
		//pDC->LineTo(161,0);
		//pDC->LineTo(161,440);
		pDC->LineTo(462,0);
		pDC->LineTo(462,440);
		pDC->LineTo(0,440);

		switch(m_iCurBtnSelTrend)
		{
		case IDC_BTN_SELTREND_1:
			{
				pDC->MoveTo(0 , 144);
				pDC->LineTo(0,441);
			}
			break;
		case IDC_BTN_SELTREND_2:
			{
				pDC->MoveTo(0 , 0);
				pDC->LineTo(0,149);
				pDC->MoveTo(0 , 292);
				pDC->LineTo(0,441);
			}
			break;
		case IDC_BTN_SELTREND_3:
			{
				pDC->MoveTo(0 , 0);
				pDC->LineTo(0,296);
			}
			break;
		}
		pDC->SelectObject(&pTempPen);
		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();
		//pen.DeleteObject();

		return 1;
	}
	else
		return 0;
}


// **************************************************************************
// 
// **************************************************************************
void CWndMenuTrendtype::Init()
{
	CClientDC dc(this);

	DWORD dwStyleNoTab = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW;

	BTN btn;

	m_pcMenu_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_POPUP_UP);
	m_pcMenu_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_POPUP_DW);

	
	//Menu Buttons
	btn.wID					= IDC_POPUP_TREND_PINSP;	
	btn.poPosition.x		= 9;
	btn.poPosition.y		= 9;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcTrendPINSP=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcTrendPINSP->Create(this,g_hf14AcuMed,0);
	m_pcTrendPINSP->SetText(getModel()->GetLanguageString(IDS_TREND_PINSP),getModel()->GetLanguageString(IDS_TREND_PINSP));
	m_plMenuBtn.AddTail(m_pcTrendPINSP);


	btn.wID					= IDC_POPUP_TREND_PMEAN;	
	btn.poPosition.x		= 9;
	btn.poPosition.y		= 62;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcTrendPMEAN=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcTrendPMEAN->Create(this,g_hf14AcuMed,0);
	m_pcTrendPMEAN->SetText(getModel()->GetLanguageString(IDS_TREND_PMEAN),getModel()->GetLanguageString(IDS_TREND_PMEAN));
	m_plMenuBtn.AddTail(m_pcTrendPMEAN);

	btn.wID					= IDC_POPUP_TREND_FIO2;	
	btn.poPosition.x		= 9;
	btn.poPosition.y		= 115;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcTrendFIO2=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcTrendFIO2->Create(this,g_hf14AcuMed,0);
	m_pcTrendFIO2->SetText(getModel()->GetLanguageString(IDS_TREND_FIO2),getModel()->GetLanguageString(IDS_TREND_FIO2));
	m_plMenuBtn.AddTail(m_pcTrendFIO2);

	btn.wID					= IDC_POPUP_TREND_VTE;	
	btn.poPosition.x		= 9;
	btn.poPosition.y		= 168;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcTrendVTE=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcTrendVTE->Create(this,g_hf14AcuMed,0);
	m_pcTrendVTE->SetText(getModel()->GetLanguageString(IDS_TREND_VTE),getModel()->GetLanguageString(IDS_TREND_VTE));
	m_plMenuBtn.AddTail(m_pcTrendVTE);


	btn.wID					= IDC_POPUP_TREND_COMPLIANCE;	
	btn.poPosition.x		= 9;
	btn.poPosition.y		= 221;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcTrendCOMPLIANCE=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcTrendCOMPLIANCE->Create(this,g_hf14AcuMed,0);
	m_pcTrendCOMPLIANCE->SetText(getModel()->GetLanguageString(IDS_TREND_COMPL),getModel()->GetLanguageString(IDS_TREND_COMPL));
	m_plMenuBtn.AddTail(m_pcTrendCOMPLIANCE);

	btn.wID					= IDC_POPUP_TREND_CO2HFO;	
	btn.poPosition.x		= 9;
	btn.poPosition.y		= 274;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcTrendCO2HFO=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcTrendCO2HFO->Create(this,g_hf14AcuMed,0);
	m_pcTrendCO2HFO->SetText(getModel()->GetLanguageString(IDS_TREND_CO2),getModel()->GetLanguageString(IDS_TREND_CO2));
	m_plMenuBtn.AddTail(m_pcTrendCO2HFO);

	btn.wID					= IDC_POPUP_TREND_MV;	
	btn.poPosition.x		= 9;
	btn.poPosition.y		= 327;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcTrendMV=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcTrendMV->Create(this,g_hf14AcuMed,0);
	m_pcTrendMV->SetText(getModel()->GetLanguageString(IDS_TREND_MV),getModel()->GetLanguageString(IDS_TREND_MV));
	m_plMenuBtn.AddTail(m_pcTrendMV);


	btn.wID					= IDC_POPUP_TREND_HFAMP;	
	btn.poPosition.x		= 9;
	btn.poPosition.y		= 380;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcTrendHFAMP=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcTrendHFAMP->Create(this,g_hf14AcuMed,0);
	m_pcTrendHFAMP->SetText(getModel()->GetLanguageString(IDS_TREND_HFAMP),getModel()->GetLanguageString(IDS_TREND_HFAMP));
	m_plMenuBtn.AddTail(m_pcTrendHFAMP);


	//m_pcTrendRSBI=NULL;
	btn.wID					= IDC_POPUP_TREND_RSBI;	
	btn.poPosition.x		= 160;
	btn.poPosition.y		= 9;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcTrendRSBI=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcTrendRSBI->Create(this,g_hf14AcuMed,0);
	m_pcTrendRSBI->SetText(_T("RSBI"),_T("RSBI"));
	m_plMenuBtn.AddTail(m_pcTrendRSBI);

	//m_pcTrendSHAREMVMAND=NULL;
	btn.wID					= IDC_POPUP_TREND_SHAREMVMAND;	
	btn.poPosition.x		= 160;
	btn.poPosition.y		= 62;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcTrendSHAREMVMAND=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcTrendSHAREMVMAND->Create(this,g_hf14AcuMed,0);
	m_pcTrendSHAREMVMAND->SetText(_T("% MVmand"),_T("% MVmand"));
	m_plMenuBtn.AddTail(m_pcTrendSHAREMVMAND);

	//m_pcTrendRESISTANCE=NULL;
	btn.wID					= IDC_POPUP_TREND_RESISTANCE;	
	btn.poPosition.x		= 160;
	btn.poPosition.y		= 115;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcTrendRESISTANCE=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcTrendRESISTANCE->Create(this,g_hf14AcuMed,0);
	m_pcTrendRESISTANCE->SetText(_T("RESISTANCE"),_T("RESISTANCE"));
	m_plMenuBtn.AddTail(m_pcTrendRESISTANCE);

	//m_pcTrendLEAK=NULL;
	btn.wID					= IDC_POPUP_TREND_LEAK;	
	btn.poPosition.x		= 160;
	btn.poPosition.y		= 168;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcTrendLEAK=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcTrendLEAK->Create(this,g_hf14AcuMed,0);
	m_pcTrendLEAK->SetText(_T("LEAK"),_T("LEAK"));
	m_plMenuBtn.AddTail(m_pcTrendLEAK);

	//m_pcTrendSPO2=NULL;
	btn.wID					= IDC_POPUP_TREND_SPO2;	
	btn.poPosition.x		= 160;
	btn.poPosition.y		= 221;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcTrendSPO2=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcTrendSPO2->Create(this,g_hf14AcuMed,0);
	m_pcTrendSPO2->SetText(_T("SPO2"),_T("SPO2"));
	m_plMenuBtn.AddTail(m_pcTrendSPO2);

	btn.wID					= IDC_POPUP_TREND_SPO2PI;	
	btn.poPosition.x		= 160;
	btn.poPosition.y		= 274;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcTrendSPO2PI=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcTrendSPO2PI->Create(this,g_hf14AcuMed,0);
	m_pcTrendSPO2PI->SetText(_T("SPO2 PI"),_T("SPO2 PI"));
	m_plMenuBtn.AddTail(m_pcTrendSPO2PI);


	btn.wID					= IDC_POPUP_TREND_SPO2PR;	
	btn.poPosition.x		= 160;
	btn.poPosition.y		= 327;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcTrendSPO2PR=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcTrendSPO2PR->Create(this,g_hf14AcuMed,0);
	m_pcTrendSPO2PR->SetText(_T("SPO2 PR"),_T("SPO2 PR"));
	m_plMenuBtn.AddTail(m_pcTrendSPO2PR);


	//m_pcTrendETCO2=NULL;
	btn.wID					= IDC_POPUP_TREND_ETCO2;	
	btn.poPosition.x		= 160;
	btn.poPosition.y		= 380;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcTrendETCO2=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcTrendETCO2->Create(this,g_hf14AcuMed,0);
	m_pcTrendETCO2->SetText(_T("ETCO2"),_T("ETCO2"));
	m_plMenuBtn.AddTail(m_pcTrendETCO2);
	

	btn.wID					= IDC_POPUP_TREND_FREQUENCY;	
	btn.poPosition.x		= 311;
	btn.poPosition.y		= 9;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcTrendFREQUENCY=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcTrendFREQUENCY->Create(this,g_hf14AcuMed,0);
	m_pcTrendFREQUENCY->SetText(_T("FREQ"),_T("FREQ"));
	m_plMenuBtn.AddTail(m_pcTrendFREQUENCY);
	//ShowMenuBtn();

	//Draw();

}

// **************************************************************************
// 
// **************************************************************************
void CWndMenuTrendtype::SetOneButtonDepressed(int btnID)
{
	POSITION pos;

	try
	{
		for( pos = m_plMenuBtn.GetHeadPosition(); pos != NULL; )
		{
			CMenuBtn* pUDBtn = m_plMenuBtn.GetNext( pos );

			if(pUDBtn->GetBtnId() != btnID)
			{
				if(pUDBtn->IsDepressed())
				{
					pUDBtn->Depress(false);
				}
			}
			else
			{
				pUDBtn->Depress(true);
			}
		}
	}
	catch (...)
	{
		CString szError=_T("");
		szError.Format(_T("EXCEPTION: CWndMenuTrendtype::SetOneButtonDepressed error: #%d"),GetLastError());
		theApp.ReportException(szError);
	}
	
}

// **************************************************************************
// 
// **************************************************************************
void CWndMenuTrendtype::SetAllButtonUnpressed()
{
	POSITION pos;

	try
	{
		for( pos = m_plMenuBtn.GetHeadPosition(); pos != NULL; )
		{
			CMenuBtn* pUDBtn = m_plMenuBtn.GetNext( pos );

			if(pUDBtn->IsDepressed())
			{
				pUDBtn->Depress(false);
			}
		}
	}
	catch (...)
	{
		CString szError=_T("");
		szError.Format(_T("EXCEPTION: CWndMenuTrendtype::SetAllButtonUnpressed error: #%d"),GetLastError());
		theApp.ReportException(szError);
	}
	
}

// **************************************************************************
// 
// **************************************************************************
void CWndMenuTrendtype::Show(BOOL bShow, UINT iTrendType, UINT bSelTrend)
{
	m_iCurBtnSelTrend=bSelTrend;

	if(bShow==TRUE)
		this->ShowWindow(SW_SHOW);
	else
		this->ShowWindow(SW_HIDE);

	switch(iTrendType)
	{
	case TREND_PINSP:
		{
			SetOneButtonDepressed(IDC_POPUP_TREND_PINSP);
		}
		break;
	case TREND_PMEAN:
		{
			SetOneButtonDepressed(IDC_POPUP_TREND_PMEAN);
		}
		break;
	case TREND_FIO2:
		{
			SetOneButtonDepressed(IDC_POPUP_TREND_FIO2);
		}
		break;
	case TREND_VTE:
		{
			SetOneButtonDepressed(IDC_POPUP_TREND_VTE);
		}
		break;
	case TREND_COMPLIANCE:
		{
			SetOneButtonDepressed(IDC_POPUP_TREND_COMPLIANCE);
		}
		break;
	case TREND_CO2HFO:
		{
			SetOneButtonDepressed(IDC_POPUP_TREND_CO2HFO);
		}
		break;
	case TREND_MV:
		{
			SetOneButtonDepressed(IDC_POPUP_TREND_MV);
		}
		break;
	case TREND_HFAMP:
		{
			SetOneButtonDepressed(IDC_POPUP_TREND_HFAMP);
		}
		break;
	case TREND_RSBI:
		{
			SetOneButtonDepressed(IDC_POPUP_TREND_RSBI);
		}
		break;
	case TREND_SHAREMVMAND:
		{
			SetOneButtonDepressed(IDC_POPUP_TREND_SHAREMVMAND);
		}
		break;
	case TREND_RESISTANCE:
		{
			SetOneButtonDepressed(IDC_POPUP_TREND_RESISTANCE);
		}
		break;
	case TREND_LEAK:
		{
			SetOneButtonDepressed(IDC_POPUP_TREND_LEAK);
		}
		break;
	case TREND_SPO2:
		{
			SetOneButtonDepressed(IDC_POPUP_TREND_SPO2);
		}
		break;
	case TREND_SPO2PR:
		{
			SetOneButtonDepressed(IDC_POPUP_TREND_SPO2PR);
		}
		break;
	case TREND_SPO2PI:
		{
			SetOneButtonDepressed(IDC_POPUP_TREND_SPO2PI);
		}
		break;
	case TREND_ETCO2:
		{
			SetOneButtonDepressed(IDC_POPUP_TREND_ETCO2);
		}
		break;
	case TREND_FREQUENCY:
		{
			SetOneButtonDepressed(IDC_POPUP_TREND_FREQUENCY);
		}
		break;
	}

	Draw();
}

void CWndMenuTrendtype::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	//CBrush cbrBack(RGB(3,230,3));
	CBrush cbrBack(RGB(181,178,181));
	//CBrush cbrBack(BACKGND);
	//CBrush cbrGrey;
	//cbrGrey.CreateSolidBrush(RGB(181,178,181));
	//cbrGrey.CreateSolidBrush(RGB(3,230,3));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	rc.left = 5;  
	rc.top = 0;  
	//rc.right  = 165;  
	rc.right  = 315;  
	rc.bottom = 49;

	CPen* pTempPen = NULL;
	CPen origPen;
	CPen pen(PS_SOLID,1,BLACK);
	

	CDC* pDC=CDC::FromHandle(hdcMem);
	pTempPen = (CPen*)pDC->SelectObject(&pen);

	pDC->MoveTo(0 , 0);
	//pDC->LineTo(161,0);
	//pDC->LineTo(161,440);
	pDC->LineTo(311,0);
	pDC->LineTo(311,440);
	pDC->LineTo(0,440);

	switch(m_iCurBtnSelTrend)
	{
	case IDC_BTN_SELTREND_1:
		{
			pDC->MoveTo(0 , 144);
			pDC->LineTo(0,441);
		}
		break;
	case IDC_BTN_SELTREND_2:
		{
			pDC->MoveTo(0 , 0);
			pDC->LineTo(0,149);
			pDC->MoveTo(0 , 292);
			pDC->LineTo(0,441);
		}
		break;
	case IDC_BTN_SELTREND_3:
		{
			pDC->MoveTo(0 , 0);
			pDC->LineTo(0,296);
		}
		break;
	}

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);
	
	pDC->SelectObject(&origPen);

	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	SelectObject(hdcMem,hBmpMemPrev);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);

}

// **************************************************************************
// 
// **************************************************************************
void CWndMenuTrendtype::ShowMenuBtn()
{
	/*m_pcMenu1->ShowWindow(SW_SHOW);
	m_pcMenu2->ShowWindow(SW_SHOW);
	m_pcMenu3->ShowWindow(SW_SHOW);
	m_pcMenu4->ShowWindow(SW_SHOW);
	m_pcMenu5->ShowWindow(SW_SHOW);*/

}



// **************************************************************************
// 
// **************************************************************************
void CWndMenuTrendtype::HideMenuBtn()
{
	/*m_pcMenu1->ShowWindow(SW_HIDE);
	m_pcMenu2->ShowWindow(SW_HIDE);
	m_pcMenu3->ShowWindow(SW_HIDE);
	m_pcMenu4->ShowWindow(SW_HIDE);
	m_pcMenu5->ShowWindow(SW_HIDE);*/
}
// **************************************************************************
// 
// **************************************************************************
LRESULT CWndMenuTrendtype::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_MENUBUTTONDOWN:
		{
			m_bPopupBtn=true;
			switch(wParam)
			{
			case IDC_POPUP_TREND_PINSP:
				{
					m_parentView->PostMessage(WM_TREND_POPUP_PINSP);
					return 1;
				}
				break;
			case IDC_POPUP_TREND_PMEAN:
				{
					m_parentView->PostMessage(WM_TREND_POPUP_PMEAN);
					return 1;
				}
				break;
			case IDC_POPUP_TREND_FIO2:
				{
					m_parentView->PostMessage(WM_TREND_POPUP_FIO2);
					return 1;
				}
				break;
			case IDC_POPUP_TREND_VTE:
				{
					m_parentView->PostMessage(WM_TREND_POPUP_VTE);
					return 1;
				}
				break;
			case IDC_POPUP_TREND_COMPLIANCE:
				{
					m_parentView->PostMessage(WM_TREND_POPUP_COMPLIANCE);
					return 1;
				}
				break;
			case IDC_POPUP_TREND_CO2HFO:
				{
					m_parentView->PostMessage(WM_TREND_POPUP_CO2HFO);
					return 1;
				}
				break;
			case IDC_POPUP_TREND_MV:
				{
					m_parentView->PostMessage(WM_TREND_POPUP_MV);
					return 1;
				}
				break;
			case IDC_POPUP_TREND_HFAMP:
				{
					m_parentView->PostMessage(WM_TREND_POPUP_HFAMP);
					return 1;
				}
				break;
			case IDC_POPUP_TREND_RSBI:
				{
					m_parentView->PostMessage(WM_TREND_POPUP_RSBI);
					return 1;
				}
				break;
			case IDC_POPUP_TREND_SHAREMVMAND:
				{
					m_parentView->PostMessage(WM_TREND_POPUP_SHAREMVMAND);
					return 1;
				}
				break;
			case IDC_POPUP_TREND_RESISTANCE:
				{
					m_parentView->PostMessage(WM_TREND_POPUP_RESISTANCE);
					return 1;
				}
				break;
			case IDC_POPUP_TREND_LEAK:
				{
					m_parentView->PostMessage(WM_TREND_POPUP_LEAK);
					return 1;
				}
				break;
			case IDC_POPUP_TREND_SPO2:
				{
					m_parentView->PostMessage(WM_TREND_POPUP_SPO2);
					return 1;
				}
				break;
			case IDC_POPUP_TREND_SPO2PR:
				{
					m_parentView->PostMessage(WM_TREND_POPUP_SPO2PR);
					return 1;
				}
				break;
			case IDC_POPUP_TREND_SPO2PI:
				{
					m_parentView->PostMessage(WM_TREND_POPUP_SPO2PI);
					return 1;
				}
				break;
			case IDC_POPUP_TREND_ETCO2:
				{
					m_parentView->PostMessage(WM_TREND_POPUP_ETCO2);
					return 1;
				}
				break;
			case IDC_POPUP_TREND_FREQUENCY:
				{
					m_parentView->PostMessage(WM_TREND_POPUP_FREQUENCY);
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
	}
	return CWnd::WindowProc(message, wParam, lParam);
}




// **************************************************************************
// 
// **************************************************************************
void CWndMenuTrendtype::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);


	// CMVView::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
}

//************************************
// Method:    OnDestroy
// FullName:  CWndMenuTrendtype::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndMenuTrendtype::OnDestroy() 
{
	m_plMenuBtn.RemoveAll();

	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);
}


