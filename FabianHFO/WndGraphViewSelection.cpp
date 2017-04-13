// WndGraphViewSelection.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndGraphViewSelection.h"
#include "LangAdmin.h"
#include "globDefs.h"
#include "MVViewHandler.h"

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000
// CWndGraphViewSelection

IMPLEMENT_DYNAMIC(CWndGraphViewSelection, CWnd)

CWndGraphViewSelection::CWndGraphViewSelection(CMVView *parentView)
{
	m_bPopupBtn=false;
	m_parentView=parentView;

	m_lX				= 0;
	m_lY				= 0;
	m_lXo				= 0;
	m_lYo				= 0;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_pcMenu_Up=NULL;
	m_pcMenu_Dw=NULL;
	m_pcMenu_Dis=NULL;

	m_pcMenu_Graph_Up=NULL;
	m_pcMenu_Graph_Dw=NULL;
	m_pcMenu_Graph_Dis=NULL;

	m_pcMenu_Loop_Up=NULL;
	m_pcMenu_Loop_Dw=NULL;
	m_pcMenu_Loop_Dis=NULL;

	m_pcMenu_Trend_Up=NULL;
	m_pcMenu_Trend_Dw=NULL;
	m_pcMenu_Trend_Dis=NULL;

	m_pcWaveView=NULL;
	m_pcLoopView=NULL;
	m_pcCO2View=NULL;
	m_pcSPO2View=NULL;
	//m_pcEMGView=NULL;
	//m_pcCLFIO2View=NULL;
	m_pcTrendView=NULL;

	m_pcSelView_Up=NULL;
	m_pcSelView_Dw=NULL;

	m_pcSelView=NULL;

	m_pModel = NULL;
}

CWndGraphViewSelection::~CWndGraphViewSelection()
{
	delete m_pcSelView;
	m_pcSelView=NULL;

	delete m_pcSelView_Up;
	m_pcSelView_Up=NULL;
	delete m_pcSelView_Dw;
	m_pcSelView_Dw=NULL;

	delete m_pcWaveView;
	m_pcWaveView=NULL;
	delete m_pcLoopView;
	m_pcLoopView=NULL;
	delete m_pcCO2View;
	m_pcCO2View=NULL;
	delete m_pcSPO2View;
	m_pcSPO2View=NULL;
	/*delete m_pcEMGView;
	m_pcEMGView=NULL;*/
	delete m_pcTrendView;
	m_pcTrendView=NULL;
	/*delete m_pcCLFIO2View;
	m_pcCLFIO2View=NULL;*/


	delete m_pcMenu_Up;
	m_pcMenu_Up=NULL;
	delete m_pcMenu_Dw;
	m_pcMenu_Dw=NULL;

	delete m_pcMenu_Graph_Up;
	m_pcMenu_Graph_Up=NULL;
	delete m_pcMenu_Graph_Dw;
	m_pcMenu_Graph_Dw=NULL;
	delete m_pcMenu_Graph_Dis;
	m_pcMenu_Graph_Dis=NULL;

	delete m_pcMenu_Loop_Up;
	m_pcMenu_Loop_Up=NULL;
	delete m_pcMenu_Loop_Dw;
	m_pcMenu_Loop_Dw=NULL;
	delete m_pcMenu_Loop_Dis;
	m_pcMenu_Loop_Dis=NULL;

	delete m_pcMenu_Trend_Up;
	m_pcMenu_Trend_Up=NULL;
	delete m_pcMenu_Trend_Dw;
	m_pcMenu_Trend_Dw=NULL;
	delete m_pcMenu_Trend_Dis;
	m_pcMenu_Trend_Dis=NULL;
}

// **************************************************************************
// 
// **************************************************************************
CMVModel *CWndGraphViewSelection::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

BEGIN_MESSAGE_MAP(CWndGraphViewSelection, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CWndGraphViewSelection message handlers
// **************************************************************************
// 
// **************************************************************************
BOOL CWndGraphViewSelection::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
{
	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

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
		Rectangle(m_hDC, 0, 0, m_lX, m_lY);
		CPen* pTempPen = NULL;
		CPen origPen;
		CPen pen(PS_SOLID,1,BLACK);
		
		//CBrush cbrBack(BACKGND);
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));
		//SelectObject(m_hDC, (HPEN)pen);

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		CDC* pDC=CDC::FromHandle(m_hDC);
		pTempPen = (CPen*)pDC->SelectObject(&pen);
		origPen.FromHandle((HPEN)pTempPen);
		pDC->MoveTo(0 , 51);
		pDC->LineTo(159,51);
		pDC->MoveTo(0 , 0);
		pDC->LineTo(0,501);
		pDC->MoveTo(159,0);
		pDC->LineTo(159,501);

		m_pcMenu_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_POPUP_UP);
		m_pcMenu_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_POPUP_DW);
		m_pcMenu_Dis	= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_POPUP_DIS);

		m_pcMenu_Graph_Up=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNPOPUP_WAV_UP);
		m_pcMenu_Graph_Dw=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNPOPUP_WAV_DW);
		m_pcMenu_Graph_Dis=new CBmp(theApp.m_hInstance,dc.m_hDC, IDB_BTNPOPUP_WAV_DIS);

		m_pcMenu_Loop_Up=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNPOPUP_LOOP_UP);
		m_pcMenu_Loop_Dw=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNPOPUP_LOOP_DW);
		m_pcMenu_Loop_Dis=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNPOPUP_LOOP_DIS);

		m_pcMenu_Trend_Up=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNPOPUP_TRD_UP);
		m_pcMenu_Trend_Dw=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNPOPUP_TRD_DW);
		m_pcMenu_Trend_Dis=new CBmp(theApp.m_hInstance,dc.m_hDC, IDB_BTNPOPUP_TRD_DIS);
		

		/*m_pcMenu_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_4MENU_UP);
		m_pcMenu_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_4MENU_DW);*/

		m_pcSelView_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SELMENU_BACK_UP);
		m_pcSelView_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SELMENU_BACK_DW);

		DWORD dwStyleNoTab = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW;
		//DWORD dwStyleNoTab = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW|WS_TABSTOP;

		BTN btn;

		//Menu Buttons
		btn.wID					= IDC_BTN_SELECT_VIEW;	
		btn.poPosition.x		= 0;
		btn.poPosition.y		= 0;
		btn.pcBmpUp				= m_pcSelView_Up;
		btn.pcBmpDown			= m_pcSelView_Dw;
		btn.pcBmpFocus			= m_pcSelView_Up;
		btn.pcBmpDisabled		= m_pcSelView_Up;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		m_pcSelView=new CUpDwnBtn(btn,COLOR_TXTBTNUP,false,false);
		m_pcSelView->Create(this,g_hf14AcuMed,0);
		m_pcSelView->SetText(_T("view"),_T("view"));
		m_pcSelView->ShowWindow(SW_SHOW);

		m_pcSelView->Depress(true);


		btn.wID					= IDC_POPUP_VIEW_WAVE;	
		btn.poPosition.x		= 7;
		btn.poPosition.y		= 60;
		btn.pcBmpUp				= m_pcMenu_Graph_Up;
		btn.pcBmpDown			= m_pcMenu_Graph_Dw;
		btn.pcBmpFocus			= m_pcMenu_Graph_Up;
		btn.pcBmpDisabled		= m_pcMenu_Graph_Dis;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		m_pcWaveView=new CMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcWaveView->Create(this,g_hf14AcuMed,0);
		//m_pcWaveView->SetText(getModel()->GetLanguageString(IDS_MENU_CURVES),getModel()->GetLanguageString(IDS_MENU_CURVES));
		m_pcWaveView->SetText(_T(""),_T(""));
		m_plMenuBtn.AddTail(m_pcWaveView);


		btn.wID					= IDC_POPUP_VIEW_LOOPS;	
		btn.poPosition.x		= 7;
		btn.poPosition.y		= 110;
		btn.pcBmpUp				= m_pcMenu_Loop_Up;
		btn.pcBmpDown			= m_pcMenu_Loop_Dw;
		btn.pcBmpFocus			= m_pcMenu_Loop_Up;
		btn.pcBmpDisabled		= m_pcMenu_Loop_Dis;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		m_pcLoopView=new CMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcLoopView->Create(this,g_hf14AcuMed,0);
		//m_pcLoopView->SetText(getModel()->GetLanguageString(IDS_MENU_LOOPS),getModel()->GetLanguageString(IDS_MENU_LOOPS));
		m_pcLoopView->SetText(_T(""),_T(""));
		m_plMenuBtn.AddTail(m_pcLoopView);

		btn.wID					= IDC_POPUP_VIEW_CO2;	
		btn.poPosition.x		= 7;
		btn.poPosition.y		= 160;
		btn.pcBmpUp				= m_pcMenu_Up;
		btn.pcBmpDown			= m_pcMenu_Dw;
		btn.pcBmpFocus			= m_pcMenu_Up;
		btn.pcBmpDisabled		= m_pcMenu_Dis;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		m_pcCO2View=new CMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcCO2View->Create(this,g_hf14AcuMed,0);
		m_pcCO2View->SetText(getModel()->GetLanguageString(IDS_MENU_CO2),getModel()->GetLanguageString(IDS_MENU_CO2));
		m_plMenuBtn.AddTail(m_pcCO2View);

		btn.wID					= IDC_POPUP_VIEW_SPO2;	
		btn.poPosition.x		= 7;
		btn.poPosition.y		= 210;
		btn.pcBmpUp				= m_pcMenu_Up;
		btn.pcBmpDown			= m_pcMenu_Dw;
		btn.pcBmpFocus			= m_pcMenu_Up;
		btn.pcBmpDisabled		= m_pcMenu_Dis;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		m_pcSPO2View=new CMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcSPO2View->Create(this,g_hf14AcuMed,0);
		m_pcSPO2View->SetText(getModel()->GetLanguageString(IDS_MENU_SPO2),getModel()->GetLanguageString(IDS_MENU_SPO2));
		m_plMenuBtn.AddTail(m_pcSPO2View);


		/*btn.wID					= IDC_POPUP_VIEW_EMG;	
		btn.poPosition.x		= 7;
		btn.poPosition.y		= 260;
		btn.pcBmpUp				= m_pcMenu_Up;
		btn.pcBmpDown			= m_pcMenu_Dw;
		btn.pcBmpFocus			= m_pcMenu_Up;
		btn.pcBmpDisabled		= m_pcMenu_Dis;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		m_pcEMGView=new CMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcEMGView->Create(this,g_hf14AcuMed,0);
		m_pcEMGView->SetText(_T("EMG"),_T("EMG"));
		m_plMenuBtn.AddTail(m_pcEMGView);
		
		btn.wID					= IDC_POPUP_VIEW_CLFIO2;	
		btn.poPosition.x		= 7;
		btn.poPosition.y		= 310;
		btn.pcBmpUp				= m_pcMenu_Up;
		btn.pcBmpDown			= m_pcMenu_Dw;
		btn.pcBmpFocus			= m_pcMenu_Up;
		btn.pcBmpDisabled		= m_pcMenu_Dis;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		m_pcCLFIO2View=new CMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcCLFIO2View->Create(this,g_hf14AcuMed,0);
		m_pcCLFIO2View->SetText(_T("ASOR"),_T("ASOR"));
		m_plMenuBtn.AddTail(m_pcCLFIO2View);*/


		if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
		{
			btn.wID					= IDC_POPUP_VIEW_TREND;	
			btn.poPosition.x		= 7;
			btn.poPosition.y		= 360;
			btn.pcBmpUp				= m_pcMenu_Trend_Up;
			btn.pcBmpDown			= m_pcMenu_Trend_Dw;
			btn.pcBmpFocus			= m_pcMenu_Trend_Up;
			btn.pcBmpDisabled		= m_pcMenu_Trend_Dis;
			btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

			m_pcTrendView=new CMenuBtn(btn,COLOR_TXTBTNUP);
			m_pcTrendView->Create(this,g_hf14AcuMed,0);
			//m_pcTrendView->SetText(getModel()->GetLanguageString(IDS_MENU_TREND),getModel()->GetLanguageString(IDS_MENU_TREND));
			m_pcTrendView->SetText(_T(""),_T(""));
			m_plMenuBtn.AddTail(m_pcTrendView);
		}
		

		pDC->SelectObject(&origPen);
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
void CWndGraphViewSelection::Init()
{
	
}

// **************************************************************************
// 
// **************************************************************************
void CWndGraphViewSelection::SetOneButtonDepressed(int btnID)
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
		szError.Format(_T("EXCEPTION: CWndGraphViewSelection::SetOneButtonDepressed error: #%d"),GetLastError());
		theApp.ReportException(szError);
	}
	
}

// **************************************************************************
// 
// **************************************************************************
void CWndGraphViewSelection::SetAllButtonUnpressed()
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
		szError.Format(_T("EXCEPTION: CWndGraphViewSelection::SetAllButtonUnpressed error: #%d"),GetLastError());
		theApp.ReportException(szError);
	}
}

// **************************************************************************
// 
// **************************************************************************
void CWndGraphViewSelection::Show(BOOL bShow)
{
	if(bShow==true)
	{
		if(m_pcWaveView)
			m_pcWaveView->ShowWindow(SW_SHOW);
		if(m_pcLoopView)
			m_pcLoopView->ShowWindow(SW_SHOW);
		if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
		{
			if(m_pcTrendView)
				m_pcTrendView->ShowWindow(SW_SHOW);
		}
		if(m_pcCO2View)
			m_pcCO2View->ShowWindow(SW_SHOW);

		if(m_pcSPO2View)
			m_pcSPO2View->ShowWindow(SW_SHOW);

//#ifdef INBIOLAB_VERSION
//		if(m_pcEMGView)
//			m_pcEMGView->EnableWindow(TRUE);
//#else
//		if(m_pcEMGView)
//			m_pcEMGView->EnableWindow(FALSE);
//#endif
//		if(m_pcEMGView)
//			m_pcEMGView->ShowWindow(SW_SHOW);

//#ifdef ASOR_VERSION
//		if(m_pcCLFIO2View)
//			m_pcCLFIO2View->EnableWindow(TRUE);
//#else
//		if(m_pcCLFIO2View)
//			m_pcCLFIO2View->EnableWindow(FALSE);
//#endif
//		if(m_pcCLFIO2View)
//			m_pcCLFIO2View->ShowWindow(SW_SHOW);

		if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
		{
			if(m_pcWaveView)
				m_pcWaveView->EnableWindow(FALSE);
			if(m_pcLoopView)
				m_pcLoopView->EnableWindow(FALSE);
			/*if(m_pcCO2View)
				m_pcCO2View->EnableWindow(FALSE);
			if(m_pcSPO2View)
				m_pcSPO2View->EnableWindow(FALSE);
			if(m_pcEMGView)
				m_pcEMGView->EnableWindow(FALSE);
			if(m_pcCLFIO2View)
				m_pcCLFIO2View->EnableWindow(FALSE);*/
		}
		else
		{
			if(m_pcWaveView)
				m_pcWaveView->EnableWindow(TRUE);
			if(m_pcLoopView)
				m_pcLoopView->EnableWindow(TRUE);
			/*if(getModel()->getCONFIG()->GetCO2module()==CO2MODULE_NONE)
			{
				if(m_pcCO2View)
				{
					m_pcCO2View->EnableWindow(FALSE);
				}
			}
			else
			{
				if(m_pcCO2View)
				{
					m_pcCO2View->EnableWindow(TRUE);
				}
			}
			
			if(getModel()->getCONFIG()->GetSPO2module()==SPO2MODULE_MASIMO)
			{
				if(m_pcSPO2View)
				{
					m_pcSPO2View->EnableWindow(FALSE);
				}
			}
			else
			{
				if(m_pcSPO2View)
				{
					m_pcSPO2View->EnableWindow(TRUE);
				}
			}
			
			if(getModel()->getCONFIG()->GetEMGmodule()==EMGMODULE_MASIMO)
			{
			if(m_pcEMGView)
			{
			m_pcEMGView->EnableWindow(FALSE);
			}
			}
			else
			{
			if(m_pcEMGView)
			{
			m_pcEMGView->EnableWindow(TRUE);
			}
			}*/
			
		}

		if(getModel()->getCONFIG()->GetCO2module()==CO2MODULE_NONE)
		{
			if(m_pcCO2View)
			{
				m_pcCO2View->EnableWindow(FALSE);
			}
		}
		else
		{
			if(m_pcCO2View)
			{
				m_pcCO2View->EnableWindow(TRUE);
			}
		}

		if(getModel()->getCONFIG()->GetSPO2module()==SPO2MODULE_NONE)
		{
			if(m_pcSPO2View)
			{
				m_pcSPO2View->EnableWindow(FALSE);
			}
		}
		else
		{
			if(m_pcSPO2View)
			{
				m_pcSPO2View->EnableWindow(TRUE);
			}
		}

		/*if(getModel()->getCONFIG()->GetEMGmodule()==EMGMODULE_NONE)
		{
			if(m_pcEMGView)
			{
				m_pcEMGView->EnableWindow(FALSE);
			}
		}
		else
		{
			if(m_pcEMGView)
			{
				m_pcEMGView->EnableWindow(TRUE);
			}
		}*/

//#ifdef INBIOLAB_VERSION
//		if(m_pcEMGView)
//			m_pcEMGView->EnableWindow(TRUE);
//#else
//		if(m_pcEMGView)
//			m_pcEMGView->EnableWindow(FALSE);
//#endif
//
//#ifdef ASOR_VERSION
//		if(m_pcCLFIO2View)
//			m_pcCLFIO2View->EnableWindow(TRUE);
//#else
//		if(m_pcCLFIO2View)
//			m_pcCLFIO2View->EnableWindow(FALSE);
//#endif

		if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
		{
			if(m_pcTrendView)
				m_pcTrendView->EnableWindow(TRUE);
		}
		else
		{
			if(m_pcTrendView)
				m_pcTrendView->EnableWindow(FALSE);
		}
		 
		 
		/*if(getModel()->getVIEWHANDLER()->getViewSubState()==CMVModel::VSS_GRAPH_CLFIO2GRAPHS)
		{
			SetOneButtonDepressed(IDC_POPUP_VIEW_CLFIO2);
		}
		else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_EMGGRAPHS)
		{
			SetOneButtonDepressed(IDC_POPUP_VIEW_EMG);
		}
		else*/ if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
		{
			SetOneButtonDepressed(IDC_POPUP_VIEW_CO2);
		}
		else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
		{
			SetOneButtonDepressed(IDC_POPUP_VIEW_LOOPS);
		}
		else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
		{
			SetOneButtonDepressed(IDC_POPUP_VIEW_SPO2);
		}
		else
		{
			SetOneButtonDepressed(IDC_POPUP_VIEW_WAVE);
		}
		this->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcWaveView)
			m_pcWaveView->ShowWindow(SW_HIDE);
		if(m_pcLoopView)
			m_pcLoopView->ShowWindow(SW_HIDE);
		if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
		{
			if(m_pcTrendView)
				m_pcTrendView->ShowWindow(SW_HIDE);
		}
		if(m_pcCO2View)
			m_pcCO2View->ShowWindow(SW_HIDE);
		if(m_pcSPO2View)
			m_pcSPO2View->ShowWindow(SW_HIDE);
		/*if(m_pcEMGView)
			m_pcEMGView->ShowWindow(SW_HIDE);
		if(m_pcCLFIO2View)
			m_pcCLFIO2View->ShowWindow(SW_HIDE);*/

		this->ShowWindow(SW_HIDE);
	}

	
}

// **************************************************************************
// 
// **************************************************************************
LRESULT CWndGraphViewSelection::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_MENUBUTTONDOWN:
		{
			m_bPopupBtn=true;
			switch(wParam)
			{
			/*case IDC_BTN_SELECT_VIEW:
				{
					if(m_parentView)
						m_parentView->PostMessage(WM_SELECT_VIEW);
					
					return 1;
				}
				break;*/
			case IDC_POPUP_VIEW_WAVE:
				{
					SetOneButtonDepressed(wParam);

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_WAVE);
					return 1;
				}
				break;
			case IDC_POPUP_VIEW_LOOPS:
				{
					SetOneButtonDepressed(wParam);

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_LOOP);
					return 1;
				}
				break;
			case IDC_POPUP_VIEW_CO2:
				{
					SetOneButtonDepressed(wParam);

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_CO2);
					return 1;
				}
				break;
			case IDC_POPUP_VIEW_SPO2:
				{
					SetOneButtonDepressed(wParam);

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_SPO2);
					return 1;
				}
				break;
			/*case IDC_POPUP_VIEW_EMG:
				{
					SetOneButtonDepressed(wParam);

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_EMG);
					return 1;
				}
				break;*/
			/*case IDC_POPUP_VIEW_CLFIO2:
				{
					SetOneButtonDepressed(wParam);

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_CLFIO2);
					return 1;
				}
				break;*/
			case IDC_POPUP_VIEW_TREND:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_TREND);

					
					return 1;
					
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
void CWndGraphViewSelection::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);


	// CMVView::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
}

//************************************
// Method:    OnDestroy
// FullName:  CWndGraphViewSelection::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndGraphViewSelection::OnDestroy() 
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



//void CWndGraphViewSelection::OnKillFocus(CWnd* pNewWnd)
//{
//	CWnd::OnKillFocus(pNewWnd);
//
//	if(!m_bPopupBtn)
//		m_parentView->PostMessage(WM_VIEWPOPUP_LOSTFOCUS);
//}

//void CWndMenuTrendtype::OnSetFocus(CWnd* pOldWnd)
//{
//	CWnd::OnSetFocus(pOldWnd);
//
//	// TODO: Add your message handler code here
//}


