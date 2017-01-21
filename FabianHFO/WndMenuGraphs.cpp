// WndMenuGraphs.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndMenuGraphs.h"
#include "LangAdmin.h"
#include "globDefs.h"
#include "MVViewHandler.h"

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000



// CWndMenuGraphs
IMPLEMENT_DYNAMIC(CWndMenuGraphs, CWnd)

CWndMenuGraphs::CWndMenuGraphs(CMVView *parentView)
{
	m_parentView=parentView;

	m_lX				= 0;
	m_lY				= 0;
	m_lXo				= 0;
	m_lYo				= 0;

	//test inbiolab
//#ifdef INBIOLAB_VERSION
//	m_bViewMenu=false;
//#else
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

//#endif

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_pcMenu_Up=NULL;
	m_pcMenu_Dw=NULL;
	

	

	m_pcMenu_Save_Up=NULL;
	m_pcMenu_Save_Dw=NULL;
	m_pcMenu_Save_Dis=NULL;

	m_pcMenu_Freeze_Up=NULL;
	m_pcMenu_Freeze_Dw=NULL;
	m_pcMenu_Freeze_Dis=NULL;

	//test inbiolab
//#ifdef INBIOLAB_VERSION
//	m_pcSelView_Up=NULL;
//	m_pcSelView_Dw=NULL;
//	m_pcSelView=NULL;
//#else
	m_pcMenuWave=NULL;
	m_pcMenuLoops=NULL;
	m_pcMenuTrend=NULL;
	m_pcMenuCO2=NULL;
	m_pcMenuSPO2=NULL;
	m_pcMenuFOT=NULL;
//#endif
	

	m_pcMenuFreeze=NULL;
	m_pcMenuSave=NULL;
	
	
	m_pModel = NULL;

	m_bFREEZE=false;
	
	
}

CWndMenuGraphs::~CWndMenuGraphs()
{

	delete m_pcMenuWave;
	m_pcMenuWave=NULL;
	delete m_pcMenuLoops;
	m_pcMenuLoops=NULL;
	delete m_pcMenuTrend;
	m_pcMenuTrend=NULL;
	delete m_pcMenuCO2;
	m_pcMenuCO2=NULL;
	delete m_pcMenuSPO2;
	m_pcMenuSPO2=NULL;
	delete m_pcMenuFOT;
	m_pcMenuFOT=NULL;
	delete m_pcMenu_Dis;
	m_pcMenu_Dis=NULL;

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
//#endif


	delete m_pcMenuFreeze;
	m_pcMenuFreeze=NULL;
	delete m_pcMenuSave;
	m_pcMenuSave=NULL;



	delete m_pcMenu_Up;
	m_pcMenu_Up=NULL;
	delete m_pcMenu_Dw;
	m_pcMenu_Dw=NULL;






	delete m_pcMenu_Save_Up;
	m_pcMenu_Save_Up=NULL;
	delete m_pcMenu_Save_Dw;
	m_pcMenu_Save_Dw=NULL;
	delete m_pcMenu_Save_Dis;
	m_pcMenu_Save_Dis=NULL;

	delete m_pcMenu_Freeze_Up;
	m_pcMenu_Freeze_Up=NULL;
	delete m_pcMenu_Freeze_Dw;
	m_pcMenu_Freeze_Dw=NULL;
	delete m_pcMenu_Freeze_Dis;
	m_pcMenu_Freeze_Dis=NULL;
}

// **************************************************************************
// 
// **************************************************************************
CMVModel *CWndMenuGraphs::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


BEGIN_MESSAGE_MAP(CWndMenuGraphs, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//ON_WM_TIMER()
END_MESSAGE_MAP()



// CWndMenuGraphs message handlers
// **************************************************************************
// 
// **************************************************************************
BOOL CWndMenuGraphs::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
{
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
void CWndMenuGraphs::Init()
{
	CClientDC dc(this);

	DWORD dwStyleNoTab = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW;

	BTN btn;

	//test inbiolab
#ifdef INBIOLAB_VERSION
	m_pcMenu_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_UP);
	m_pcMenu_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_5MENU_DW);

	m_pcSelView_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SELMENU_UP);
	m_pcSelView_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SELMENU_DW);
#else
	m_pcMenu_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_7MENU_UP);
	m_pcMenu_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_7MENU_DW);
	m_pcMenu_Dis		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_7MENU_DIS);

	m_pcMenu_Graph_Up=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU7_GRAPH_UP);
	m_pcMenu_Graph_Dw=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU7_GRAPH_DWN);
	m_pcMenu_Graph_Dis=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU7_GRAPH_DIS);

	m_pcMenu_Loop_Up=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU7_LOOP_UP);
	m_pcMenu_Loop_Dw=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU7_LOOP_DWN);
	m_pcMenu_Loop_Dis=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU7_LOOP_DIS);

	m_pcMenu_Trend_Up=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU7_TREND_UP);
	m_pcMenu_Trend_Dw=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU7_TREND_DWN);
	m_pcMenu_Trend_Dis=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU7_TREND_DIS);
#endif
	

	

	m_pcMenu_Save_Up=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU7_SAVE_UP);
	m_pcMenu_Save_Dw=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU7_SAVE_DWN);
	m_pcMenu_Save_Dis=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU7_SAVE_DIS);

	m_pcMenu_Freeze_Up=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU7_FREEZE_UP);
	m_pcMenu_Freeze_Dw=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU7_FREEZE_DWN);
	m_pcMenu_Freeze_Dis=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTNMNU7_FREEZE_DIS);


	//test inbiolab
#ifdef INBIOLAB_VERSION
	//Menu Buttons
	btn.wID					= IDC_BTN_SELECT_VIEW;	
	btn.poPosition.x		= 3;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcSelView_Up;
	btn.pcBmpDown			= m_pcSelView_Dw;
	btn.pcBmpFocus			= m_pcSelView_Up;
	btn.pcBmpDisabled		= m_pcSelView_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcSelView=new CUpDwnBtn(btn,COLOR_TXTBTNUP,false,false);
	m_pcSelView->Create(this,g_hf14AcuMed,0);
	m_pcSelView->SetText(_T("view"),_T("view"));
	m_pcSelView->ShowWindow(SW_HIDE);
#else
	//Menu Buttons
	btn.wID					= IDC_BTN_GRAPH_WAVE;	
	btn.poPosition.x		= 5;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcMenu_Graph_Up;
	btn.pcBmpDown			= m_pcMenu_Graph_Dw;
	btn.pcBmpFocus			= m_pcMenu_Graph_Up;
	btn.pcBmpDisabled		= m_pcMenu_Graph_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuWave=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuWave->Create(this,g_hf14AcuMed,0);
	m_pcMenuWave->SetText(_T(""),_T(""));
	m_pcMenuWave->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuWave);

	btn.wID					= IDC_BTN_GRAPH_LOOPS;	
	btn.poPosition.x		= 90;//102;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcMenu_Loop_Up;
	btn.pcBmpDown			= m_pcMenu_Loop_Dw;
	btn.pcBmpFocus			= m_pcMenu_Loop_Up;
	btn.pcBmpDisabled		= m_pcMenu_Loop_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuLoops=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuLoops->Create(this,g_hf14AcuMed,0);
	m_pcMenuLoops->SetText(_T(""),_T(""));
	m_pcMenuLoops->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuLoops);


	btn.wID					= IDC_BTN_GRAPH_CO2;	
	btn.poPosition.x		= 175;//201;//399;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuCO2=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuCO2->Create(this,g_hf14AcuMed,0);
	m_pcMenuCO2->SetText(getModel()->GetLanguageString(IDS_MENU_CO2),getModel()->GetLanguageString(IDS_MENU_CO2));
	m_pcMenuCO2->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuCO2);
	
	btn.wID					= IDC_BTN_GRAPH_SPO2;	
	btn.poPosition.x		= 260;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuSPO2=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuSPO2->Create(this,g_hf14AcuMed,0);
	m_pcMenuSPO2->SetText(getModel()->GetLanguageString(IDS_MENU_SPO2),getModel()->GetLanguageString(IDS_MENU_SPO2));
	m_pcMenuSPO2->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuSPO2);


	btn.wID					= IDC_BTN_GRAPH_FOT;	
	btn.poPosition.x		= 345;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcMenu_Up;
	btn.pcBmpDown			= m_pcMenu_Dw;
	btn.pcBmpFocus			= m_pcMenu_Up;
	btn.pcBmpDisabled		= m_pcMenu_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuFOT=new CMenuBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuFOT->Create(this,g_hf14AcuMed,0);
	m_pcMenuFOT->SetText(getModel()->GetLanguageString(IDS_MENU_FOT),getModel()->GetLanguageString(IDS_MENU_FOT)); //todoFOT implement to excel
	m_pcMenuFOT->ShowWindow(SW_HIDE);
	m_plMenuBtn.AddTail(m_pcMenuFOT);

	if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
	{
		btn.wID					= IDC_BTN_GRAPH_TREND;	
		btn.poPosition.x		= 515;
		btn.poPosition.y		= 0;
		btn.pcBmpUp				= m_pcMenu_Trend_Up;
		btn.pcBmpDown			= m_pcMenu_Trend_Dw;
		btn.pcBmpFocus			= m_pcMenu_Trend_Up;
		btn.pcBmpDisabled		= m_pcMenu_Trend_Dis;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		m_pcMenuTrend=new CMenuBtn(btn,COLOR_TXTBTNUP);
		m_pcMenuTrend->Create(this,g_hf14AcuMed,0);
		m_pcMenuTrend->SetText(_T(""),_T(""));
		m_pcMenuTrend->ShowWindow(SW_HIDE);
		m_plMenuBtn.AddTail(m_pcMenuTrend);
	}

#endif


	btn.wID					= IDC_BTN_GRAPH_FREEZE;	
	btn.poPosition.x		= 430;//345;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcMenu_Freeze_Up;
	btn.pcBmpDown			= m_pcMenu_Freeze_Dw;
	btn.pcBmpFocus			= m_pcMenu_Freeze_Up;
	btn.pcBmpDisabled		= m_pcMenu_Freeze_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuFreeze=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuFreeze->Create(this,g_hf14AcuMed,0);
	m_pcMenuFreeze->SetText(_T(""),_T(""));
	m_pcMenuFreeze->ShowWindow(SW_HIDE);

	btn.wID					= IDC_BTN_GRAPH_SAVE;	
	btn.poPosition.x		= 515;//430;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcMenu_Save_Up;
	btn.pcBmpDown			= m_pcMenu_Save_Dw;
	btn.pcBmpFocus			= m_pcMenu_Save_Up;
	btn.pcBmpDisabled		= m_pcMenu_Save_Dis;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenuSave=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcMenuSave->Create(this,g_hf14AcuMed,0);
	m_pcMenuSave->SetText(_T(""),_T(""));
	m_pcMenuSave->ShowWindow(SW_HIDE);



	//btn.wID					= IDC_BTN_GRAPH_CO2PUMP_ONOFF;	
	//btn.poPosition.x		= 312;//399;
	//btn.poPosition.y		= 0;
	//btn.pcBmpUp				= m_pcMenu_Up;
	//btn.pcBmpDown			= m_pcMenu_Dw;
	//btn.pcBmpFocus			= m_pcMenu_Up;
	//btn.pcBmpDisabled		= m_pcMenu_Dis;
	//btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	//m_pcMenuCO2PumpOnOff=new CUpDwnBtn(btn,COLOR_TXTBTNUP,true);
	//m_pcMenuCO2PumpOnOff->Create(this,g_hf10AcuBold,0);
	//m_pcMenuCO2PumpOnOff->SetText(getModel()->GetLanguageString(IDS_MENU_CO2),getModel()->GetLanguageString(IDS_TXT_CO2_PUMP_OFF),getModel()->GetLanguageString(IDS_MENU_CO2),getModel()->GetLanguageString(IDS_TXT_CO2_PUMP_OFF));
	//m_pcMenuCO2PumpOnOff->ShowWindow(SW_HIDE);


	//btn.wID					= IDC_BTN_GRAPH_CO2PUMP_RESET;	
	//btn.poPosition.x		= 458;//399;
	//btn.poPosition.y		= 0;
	//btn.pcBmpUp				= m_pcMenu_Up;
	//btn.pcBmpDown			= m_pcMenu_Dw;
	//btn.pcBmpFocus			= m_pcMenu_Up;
	//btn.pcBmpDisabled		= m_pcMenu_Dis;
	//btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	//m_pcMenuCO2PumpReset=new CPushBtn(btn,COLOR_TXTBTNUP,true);
	//m_pcMenuCO2PumpReset->Create(this,g_hf10AcuBold,0);
	//m_pcMenuCO2PumpReset->SetText(getModel()->GetLanguageString(IDS_MENU_CO2),getModel()->GetLanguageString(IDS_TXT_CO2_RESTARTTIMER));
	//m_pcMenuCO2PumpReset->ShowWindow(SW_HIDE);



	
	
	

}

// **************************************************************************
// 
// **************************************************************************
//bool CWndMenuGraphs::IsButtonPushed(int nIDButton )
//{
//	bool bPushed=false;
//	switch(nIDButton)
//	{
//	case IDC_BTN_GRAPH_FREEZE:
//		{
//			bPushed=m_pcMenuFreeze->IsDepressed();
//		}
//		break;
//	}
//	return bPushed;
//}

// **************************************************************************
// 
// **************************************************************************
void CWndMenuGraphs::Show(BOOL bShow)
{
//#ifdef INBIOLAB_VERSION
//	if(bShow==TRUE)
//	{
//		if(m_pcSelView)
//			m_pcSelView->ShowWindow(SW_SHOW);
//		
//
//		if(m_pcMenuFreeze)
//			m_pcMenuFreeze->ShowWindow(SW_SHOW);
//		
//		//test inbiolab
//		if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_EMGGRAPHS)
//		{
//			if(m_bFREEZE)
//			{
//				if(m_pcMenuSave)
//					m_pcMenuSave->ShowWindow(SW_SHOW);
//			}
//			else
//			{
//				if(m_pcMenuSave)
//					m_pcMenuSave->ShowWindow(SW_HIDE);
//			}
//		}
//		else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
//		{
//			//if(m_bySPO2Module==SPO2MODULE_MASIMO)
//			//{
//			//	
//			//}
//			//else
//			//{
//			//	/*if(m_pcMenuCO2PumpOnOff)
//			//		m_pcMenuCO2PumpOnOff->ShowWindow(SW_HIDE);
//			//	if(m_pcMenuCO2PumpReset)
//			//		m_pcMenuCO2PumpReset->ShowWindow(SW_HIDE);*/
//			//}
//			
//			
//			if(m_bFREEZE)
//			{
//				if(m_pcMenuSave)
//					m_pcMenuSave->ShowWindow(SW_SHOW);
//			}
//			else
//			{
//				if(m_pcMenuSave)
//					m_pcMenuSave->ShowWindow(SW_HIDE);
//			}
//			
//		}
//		else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
//		{
//			if(m_bFREEZE)
//			{
//				if(m_pcMenuSave)
//					m_pcMenuSave->ShowWindow(SW_SHOW);
//			}
//			else
//			{
//				if(m_pcMenuSave)
//					m_pcMenuSave->ShowWindow(SW_HIDE);
//			}
//		}
//		else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
//		{
//			if(m_bFREEZE)
//			{
//				if(m_pcMenuSave)
//					m_pcMenuSave->ShowWindow(SW_SHOW);
//			}
//			else
//			{
//				if(m_pcMenuSave)
//					m_pcMenuSave->ShowWindow(SW_HIDE);
//			}
//			
//		}
//		else
//		{
//			if(m_pcMenuSave)
//				m_pcMenuSave->ShowWindow(SW_HIDE);
//		}
//
//		this->ShowWindow(SW_SHOW);
//	}
//	else
//	{
//		if(m_pcSelView)
//			m_pcSelView->ShowWindow(SW_HIDE);
//
//		/*if(m_pcMenuCO2PumpOnOff)
//			m_pcMenuCO2PumpOnOff->ShowWindow(SW_HIDE);
//		if(m_pcMenuCO2PumpReset)
//			m_pcMenuCO2PumpReset->ShowWindow(SW_HIDE);*/
//
//		if(m_pcMenuFreeze)
//			m_pcMenuFreeze->ShowWindow(SW_HIDE);
//		if(m_pcMenuSave)
//			m_pcMenuSave->ShowWindow(SW_HIDE);
//		
//		
//		this->ShowWindow(SW_HIDE);
//	}
//#else
	if(bShow==TRUE)
	{
		if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF
			||	getModel()->getVMODEHANDLER()->activeModeIsNMODETrigger())//rku,PRETRIGGER)
		{
			if(m_pcMenuLoops)
				m_pcMenuLoops->EnableWindow(FALSE);
			if(m_pcMenuLoops)
				m_pcMenuLoops->ShowWindow(TRUE);
		}
		else
		{

			if(m_pcMenuLoops)
				m_pcMenuLoops->EnableWindow(TRUE);
			if(m_pcMenuLoops)
				m_pcMenuLoops->ShowWindow(SW_SHOW);
		}

		if(m_pcMenuWave)
			m_pcMenuWave->ShowWindow(SW_SHOW);


		if(getModel()->getCONFIG()->isCO2REGISTRYenabled()==false)
		{
			if(m_pcMenuCO2)
			{
				m_pcMenuCO2->ShowWindow(SW_SHOW);
				m_pcMenuCO2->EnableWindow(FALSE);
			}
		}
		else
		{
			if(getModel()->getCONFIG()->GetCO2module()==CO2MODULE_NONE)
			{
				if(m_pcMenuCO2)
				{
					m_pcMenuCO2->ShowWindow(SW_SHOW);
					m_pcMenuCO2->EnableWindow(FALSE);
				}
			}
			else
			{
				if(m_pcMenuCO2)
				{
					m_pcMenuCO2->ShowWindow(SW_SHOW);
					m_pcMenuCO2->EnableWindow(TRUE);
				}
			}
		}

		if(getModel()->getCONFIG()->isSPO2REGISTRYenabled()==false)
		{
			m_pcMenuSPO2->ShowWindow(SW_SHOW);
			m_pcMenuSPO2->EnableWindow(FALSE);
		}
		else
		{
			if(getModel()->getCONFIG()->GetSPO2module()==SPO2MODULE_NONE)
			{
				if(m_pcMenuSPO2)
				{
					m_pcMenuSPO2->ShowWindow(SW_SHOW);
					m_pcMenuSPO2->EnableWindow(FALSE);
				}
			}
			else
			{
				if(m_pcMenuSPO2)
				{
					m_pcMenuSPO2->ShowWindow(SW_SHOW);
					m_pcMenuSPO2->EnableWindow(TRUE);
				}
			}
		}

		if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
		{
			if(m_pcMenuFOT)
			{
				m_pcMenuFOT->ShowWindow(SW_SHOW);
				m_pcMenuFOT->EnableWindow(FALSE);
			}
		}
		else if(		getModel()->getDATAHANDLER()->isFOTLicenseAvailable()
			&&	false==getModel()->getVMODEHANDLER()->activeModeIsNMODE())
		{
			if(m_pcMenuFOT)
			{
				m_pcMenuFOT->ShowWindow(SW_SHOW);
				m_pcMenuFOT->EnableWindow(TRUE);
			}
		}
		else
		{
			if(m_pcMenuFOT)
			{
				m_pcMenuFOT->ShowWindow(SW_SHOW);
				m_pcMenuFOT->EnableWindow(FALSE);
			}
		}

		
		
		
		if(m_pcMenuFreeze)
			m_pcMenuFreeze->ShowWindow(SW_SHOW);
		
		if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
		{
			SetOneButtonDepressed(IDC_BTN_GRAPH_FOT);
		}
		else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
		{
			SetOneButtonDepressed(IDC_BTN_GRAPH_SPO2);
		}
		else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
		{
			SetOneButtonDepressed(IDC_BTN_GRAPH_CO2);
		}
		else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
		{
			SetOneButtonDepressed(IDC_BTN_GRAPH_LOOPS);
		}
		else
		{
			SetOneButtonDepressed(IDC_BTN_GRAPH_WAVE);
		}

		if(		getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS
			&&	getModel()->getCONFIG()->GetCurMode()!=VM_CPAP
			&&	getModel()->getCONFIG()->GetCurMode()!=VM_NCPAP
			&&	getModel()->getCONFIG()->GetCurMode()!=VM_DUOPAP)
		{
			if(m_bFREEZE)
			{
				if(m_pcMenuSave)
					m_pcMenuSave->ShowWindow(SW_SHOW);

				if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
				{
					if(m_pcMenuTrend)
						m_pcMenuTrend->ShowWindow(SW_HIDE);
				}
			}
			else
			{
				if(m_pcMenuSave)
					m_pcMenuSave->ShowWindow(SW_HIDE);

				if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
				{
					if(m_pcMenuTrend)
						m_pcMenuTrend->ShowWindow(SW_SHOW);
				}
			}
		}
		else
		{
			if(m_pcMenuSave)
				m_pcMenuSave->ShowWindow(SW_HIDE);

			if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
			{
				if(m_pcMenuTrend)
					m_pcMenuTrend->ShowWindow(SW_SHOW);
			}
			else
			{
				if(m_pcMenuTrend)
					m_pcMenuTrend->ShowWindow(SW_HIDE);
			}
		}

		/*if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_MULTI_LINEGRAPHS)
		{
			if(m_pcMenuSave)
				m_pcMenuSave->ShowWindow(SW_HIDE);

			if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
			{
				if(m_pcMenuTrend)
					m_pcMenuTrend->ShowWindow(SW_SHOW);
			}
			else
			{
				if(m_pcMenuTrend)
					m_pcMenuTrend->ShowWindow(SW_HIDE);
			}
		}
		else
		{
			if(m_bFREEZE)
			{
				if(m_pcMenuSave)
					m_pcMenuSave->ShowWindow(SW_SHOW);

				if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
				{
					if(m_pcMenuTrend)
						m_pcMenuTrend->ShowWindow(SW_HIDE);
				}
			}
			else
			{
				if(m_pcMenuSave)
					m_pcMenuSave->ShowWindow(SW_HIDE);

				if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
				{
					if(m_pcMenuTrend)
						m_pcMenuTrend->ShowWindow(SW_SHOW);
				}
			}
			
		}*/

		this->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_pcMenuWave)
			m_pcMenuWave->ShowWindow(SW_HIDE);
		if(m_pcMenuLoops)
			m_pcMenuLoops->ShowWindow(SW_HIDE);
		if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
		{
			if(m_pcMenuTrend)
				m_pcMenuTrend->ShowWindow(SW_HIDE);
		}

		if(m_pcMenuFreeze)
			m_pcMenuFreeze->ShowWindow(SW_HIDE);
		if(m_pcMenuSave)
			m_pcMenuSave->ShowWindow(SW_HIDE);
		
		if(m_pcMenuCO2)
			m_pcMenuCO2->ShowWindow(SW_HIDE);
		if(m_pcMenuSPO2)
			m_pcMenuSPO2->ShowWindow(SW_HIDE);
		if(m_pcMenuFOT)
			m_pcMenuFOT->ShowWindow(SW_HIDE);
	
		this->ShowWindow(SW_HIDE);
	}
//#endif
	
}


// **************************************************************************
// 
// **************************************************************************
LRESULT CWndMenuGraphs::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
//#ifdef INBIOLAB_VERSION
//	switch(message)
//	{
//	case WM_SHOW_VIEWMENUBTN:
//		{
//			m_bViewMenu=false;
//			m_pcSelView->ShowWindow(SW_SHOW);
//
//			if(m_pcSelView->IsDepressed())
//			{
//				m_pcSelView->Depress(false);
//			}
//
//			Invalidate();
//			UpdateWindow();
//			return 1;
//		}
//		break;
//	case WM_HIDE_VIEWMENUBTN:
//		{
//			m_bViewMenu=true;
//			if(m_pcSelView->IsDepressed())
//			{
//				m_pcSelView->Depress(false);
//			}
//
//			m_pcSelView->ShowWindow(SW_HIDE);
//
//			Invalidate();
//			UpdateWindow();
//			return 1;
//		}
//		break;
//	case WM_MENUBUTTONDOWN:
//		{
//			switch(wParam)
//			{
//			case IDC_BTN_SELECT_VIEW:
//				{
//					if(m_parentView)
//						m_parentView->PostMessage(WM_SELECT_VIEW);
//
//					
//					return 1;
//				}
//				break;
//			case IDC_BTN_GRAPH_FREEZE:
//				{
//					m_bFREEZE=!m_bFREEZE;
//
//					if(m_bFREEZE)
//					{
//						if(		getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS
//							&&	getModel()->getCONFIG()->GetCurMode()!=VM_CPAP
//							&&	getModel()->getCONFIG()->GetCurMode()!=VM_NCPAP
//							&&	getModel()->getCONFIG()->GetCurMode()!=VM_DUOPAP)
//						{
//							if(m_pcMenuSave)
//								m_pcMenuSave->ShowWindow(SW_SHOW);
//						}
//						else
//						{
//							if(m_pcMenuSave)
//								m_pcMenuSave->ShowWindow(SW_HIDE);
//						}
//
//						m_parentView->PostMessage(WM_FREEZE_GRAPHS);
//					}
//					else
//					{
//						m_pcMenuSave->ShowWindow(SW_HIDE);
//						m_parentView->PostMessage(WM_CONTINUE_GRAPHS);
//					}
//
//					return 1;
//				}
//				break;
//			case IDC_BTN_GRAPH_SAVE:
//				{
//					if(AfxGetApp())
//						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_SAVE);
//					
//					return 1;
//				}
//				break;
//			default:
//				{
//				}
//				break;
//			}
//		}
//		break;
//	case WM_MENUBUTTONUP:
//		{
//			switch(wParam)
//			{
//			case IDC_BTN_SELECT_VIEW:
//				{
//					if(m_parentView)
//						m_parentView->PostMessage(WM_SELECT_VIEW);
//
//					return 1;
//				}
//				break;
//			case IDC_BTN_GRAPH_WAVE:
//				{
//					return 1;
//				}
//				break;
//			case IDC_BTN_GRAPH_LOOPS:
//				{
//					return 1;
//				}
//				break;
//			case IDC_BTN_GRAPH_CO2:
//				{
//					return 1;
//				}
//				break;
//			case IDC_BTN_GRAPH_TREND:
//				{
//					return 1;
//					
//				}
//				break;
//			case IDC_BTN_GRAPH_FREEZE:
//				{
//					return 1;
//				}
//				break;
//			case IDC_BTN_GRAPH_SAVE:
//				{
//					return 1;
//				}
//				break;
//			default:
//				{
//				}
//				break;
//			}
//
//		}
//		break;
//	}
//#else
	switch(message)
	{
	case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_GRAPH_WAVE:
				{
					SetOneButtonDepressed(wParam);

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_WAVE);
					return 1;
				}
				break;
			case IDC_BTN_GRAPH_LOOPS:
				{
					SetOneButtonDepressed(wParam);

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_LOOP);
					return 1;
				}
				break;
			case IDC_BTN_GRAPH_CO2:
				{
					SetOneButtonDepressed(wParam);

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_CO2);
					return 1;
				}
				break;
			case IDC_BTN_GRAPH_SPO2:
				{
					SetOneButtonDepressed(wParam);

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_SPO2);
					return 1;
				}
				break;
			case IDC_BTN_GRAPH_FOT:
				{
					SetOneButtonDepressed(wParam);

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_FOT);
					return 1;
				}
				break;
			case IDC_BTN_GRAPH_TREND:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_TREND);

					m_bFREEZE=false;

					return 1;
					
				}
				break;
			case IDC_BTN_GRAPH_FREEZE:
				{
					m_bFREEZE=!m_bFREEZE;

					if(m_bFREEZE)
					{
						if(		getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS
							&&	getModel()->getCONFIG()->GetCurMode()!=VM_CPAP
							&&	getModel()->getCONFIG()->GetCurMode()!=VM_NCPAP
							&&	getModel()->getCONFIG()->GetCurMode()!=VM_DUOPAP)
						{
							if(m_pcMenuSave)
								m_pcMenuSave->ShowWindow(SW_SHOW);

							if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
							{
								if(m_pcMenuTrend)
									m_pcMenuTrend->ShowWindow(SW_HIDE);
							}
						}
						else
						{
							if(m_pcMenuSave)
								m_pcMenuSave->ShowWindow(SW_HIDE);

							if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
							{
								if(m_pcMenuTrend)
									m_pcMenuTrend->ShowWindow(SW_SHOW);
							}
						}

						m_parentView->PostMessage(WM_FREEZE_GRAPHS);
					}
					else
					{
						m_pcMenuSave->ShowWindow(SW_HIDE);

						if(getModel()->getDATAHANDLER()->isTRENDLicenseAvailable())
						{
							if(m_pcMenuTrend)
								m_pcMenuTrend->ShowWindow(SW_SHOW);
						}

						m_parentView->PostMessage(WM_CONTINUE_GRAPHS);
					}

					return 1;
				}
				break;
			case IDC_BTN_GRAPH_SAVE:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_SAVE);
					
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
	case WM_MENUBUTTONUP:
		{
			switch(wParam)
			{
			case IDC_BTN_GRAPH_SAVE:
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
	}
//#endif
	
	return CWnd::WindowProc(message, wParam, lParam);
}

// **************************************************************************
// 
// **************************************************************************
void CWndMenuGraphs::SetOneButtonDepressed(int btnID)
{
	POSITION pos;

	try
	{
		for( pos = m_plMenuBtn.GetHeadPosition(); pos != NULL; )
		{
			CMenuBtn* pUDBtn = m_plMenuBtn.GetNext( pos );

			if(pUDBtn->GetBtnId() != btnID)
			{
				pUDBtn->Depress(false);
			}
			else
			{
				pUDBtn->Depress(true);
			}
		}
	}
	catch (...)
	{
		CFabianHFOApp::ReportException(_T("EXCEPTION: CWndMenuGraphs::SetOneButtonDepressed"));
	}
}

// **************************************************************************
// 
// **************************************************************************
void CWndMenuGraphs::SetAllButtonUnpressed()
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
		CFabianHFOApp::ReportException(_T("EXCEPTION: CWndMenuGraphs::SetAllButtonUnpressed"));
	}
	
}

// **************************************************************************
// 
// **************************************************************************
void CWndMenuGraphs::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);

	//test inbiolab
#ifdef INBIOLAB_VERSION
	Draw();
#endif
}

//test inbiolab
// **************************************************************************
// 
// **************************************************************************
#ifdef INBIOLAB_VERSION
void CWndMenuGraphs::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);


	//CBrush cbrBack(RGB(151,158,151));
	CBrush cbrBack(BACKGND);
	CBrush cbrGrey;
	cbrGrey.CreateSolidBrush(RGB(181,178,181));
	//cbrGrey.CreateSolidBrush(RGB(3,230,3));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	rc.left = 5;  
	rc.top = 0;  
	rc.right  = 165;  
	rc.bottom = 49;

	if(m_bViewMenu)
	{
		FillRect(hdcMem,&rc,cbrGrey);

		CPen pen;
		pen.CreatePen(PS_SOLID,1,BLACK);

		CDC* pDC=CDC::FromHandle(hdcMem);
		pDC->SelectObject(&pen);
		pDC->MoveTo(5 , 0);
		pDC->LineTo(5,50);
		pDC->MoveTo(164,0);
		pDC->LineTo(164,50);

		pen.DeleteObject();
	}
	else
	{
		FillRect(hdcMem,&rc,cbrBack);
	}



	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	SelectObject(hdcMem,hBmpMemPrev);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);

}
#endif

//************************************
// Method:    OnDestroy
// FullName:  CWndMenuGraphs::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndMenuGraphs::OnDestroy() 
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

void CWndMenuGraphs::FlowSensorStateChanged()
{
	//test inbiolab
#ifndef INBIOLAB_VERSION
	if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
		||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
		||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
	{
		if(m_pcMenuLoops)
			m_pcMenuLoops->ShowWindow(SW_HIDE);
	}
	else
	{
		if(m_pcMenuLoops)
			m_pcMenuLoops->ShowWindow(SW_SHOW);
	}
#endif
	
}

// **************************************************************************
// 
// **************************************************************************
//void CWndMenuGraphs::OnTimer(UINT_PTR nIDEvent)
//{
//
//}


