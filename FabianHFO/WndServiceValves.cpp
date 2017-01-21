// WndServiceValves.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndServiceValves.h"


// CWndServiceValves

IMPLEMENT_DYNAMIC(CWndServiceValves, CWnd)

CWndServiceValves::CWndServiceValves():
CWndService()
{
	

	m_pcVentilBurn=NULL;
	m_pcMenu_UP=NULL;
	m_pcMenu_DW=NULL;
	m_bVentilBurnRunning=false;

	m_iFlow=0;
	m_iPaw=0;
}

CWndServiceValves::~CWndServiceValves()
{
	delete m_pcVentilBurn;
	delete m_pcMenu_UP;
	delete m_pcMenu_DW;

	m_pcVentilBurn=NULL;
	m_pcMenu_UP=NULL;
	m_pcMenu_DW=NULL;

}


BEGIN_MESSAGE_MAP(CWndServiceValves, CWnd)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV10, &CWndServiceValves::OnBnClickedVentilBurn)
	/*ON_BN_CLICKED(IDC_BTN_SERVICE_VALV0, &CWndServiceValves::OnBnClickedValv0)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV1, &CWndServiceValves::OnBnClickedValv1)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV2, &CWndServiceValves::OnBnClickedValv2)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV3, &CWndServiceValves::OnBnClickedValv3)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV8, &CWndServiceValves::OnBnClickedValv8)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV9, &CWndServiceValves::OnBnClickedValv9)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV10, &CWndServiceValves::OnBnClickedValv10)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV11, &CWndServiceValves::OnBnClickedValv11)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV12, &CWndServiceValves::OnBnClickedValv12)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV13, &CWndServiceValves::OnBnClickedValv13)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV14, &CWndServiceValves::OnBnClickedValv14)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV15, &CWndServiceValves::OnBnClickedValv15)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV16, &CWndServiceValves::OnBnClickedValv16)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV17, &CWndServiceValves::OnBnClickedValv17)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV18, &CWndServiceValves::OnBnClickedValv18)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV19, &CWndServiceValves::OnBnClickedValv19)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV20, &CWndServiceValves::OnBnClickedValv20)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV21, &CWndServiceValves::OnBnClickedValv21)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV22, &CWndServiceValves::OnBnClickedValv22)
	ON_BN_CLICKED(IDC_BTN_SERVICE_VALV23, &CWndServiceValves::OnBnClickedValv23)
	ON_BN_CLICKED(IDC_BTN_VALV_OFF, &CWndServiceValves::OnBnClickedValvOff)
	ON_BN_CLICKED(IDC_BTN_EXPVALV_OPEN, &CWndServiceValves::OnBnClickedExpValveOpen)
	ON_BN_CLICKED(IDC_BTN_EXPVALV_CLOSE, &CWndServiceValves::OnBnClickedExpValveClose)
	ON_BN_CLICKED(IDC_BTN_OPVALV_OPEN, &CWndServiceValves::OnBnClickedOpValveOpen)
	ON_BN_CLICKED(IDC_BTN_OPVALV_CLOSE, &CWndServiceValves::OnBnClickedOpValveClose)*/
END_MESSAGE_MAP()



// CWndServiceValves message handlers
void CWndServiceValves::Init()
{
	CClientDC dc(this);

	/*m_pcValve_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VALV_UP);
	m_pcValve_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VALV_DW);

	m_pcValveOff_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VALVOFF_UP);
	m_pcValveOff_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_VALVOFF_DW);

	m_pcFg_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_FG_UP);
	m_pcFg_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_FG_DW);

	m_pcOpen_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_OPENCLOSE_UP);
	m_pcOpen_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_OPENCLOSE_DW);*/

	m_pcMenu_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SELMENU_UP);
	m_pcMenu_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SELMENU_DW);
	

	BTN btn;

	
	btn.wID					= IDC_BTN_SERVICE_VALV10;	
	btn.poPosition.x		= 250;
	btn.poPosition.y		= 100;
	btn.pcBmpUp				= m_pcMenu_UP;
	btn.pcBmpDown			= m_pcMenu_DW;
	btn.pcBmpFocus			= m_pcMenu_UP;
	btn.pcBmpDisabled		= m_pcMenu_UP;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcVentilBurn=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcVentilBurn->Create(this,g_hf15AcuMed,0);

	if(getModel()->getDATAHANDLER()->IsVentilBurnRunning())
	{
		m_pcVentilBurn->SetText(_T("OFF"),_T("OFF"));
		m_bVentilBurnRunning=true;
	}
	else
	{
		m_pcVentilBurn->SetText(_T("ON"),_T("ON"));
		m_bVentilBurnRunning=false;
	}
	
	
	

	////O2
	//btn.wID					= IDC_BTN_SERVICE_VALV10;	
	//btn.poPosition.x		= 535;
	//btn.poPosition.y		= 105;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve10=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve10->Create(this,g_hf15AcuMed,0);
	//m_pcValve10->SetText(_T("10"),_T("10"));

	//btn.wID					= IDC_BTN_SERVICE_VALV11;	
	//btn.poPosition.x		= 490;
	//btn.poPosition.y		= 105;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve11=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve11->Create(this,g_hf15AcuMed,0);
	//m_pcValve11->SetText(_T("11"),_T("11"));

	//btn.wID					= IDC_BTN_SERVICE_VALV12;	
	//btn.poPosition.x		= 445;
	//btn.poPosition.y		= 105;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve12=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve12->Create(this,g_hf15AcuMed,0);
	//m_pcValve12->SetText(_T("12"),_T("12"));


	//btn.wID					= IDC_BTN_SERVICE_VALV13;	
	//btn.poPosition.x		= 400;
	//btn.poPosition.y		= 105;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve13=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve13->Create(this,g_hf15AcuMed,0);
	//m_pcValve13->SetText(_T("13"),_T("13"));

	//btn.wID					= IDC_BTN_SERVICE_VALV14;	
	//btn.poPosition.x		= 355;
	//btn.poPosition.y		= 105;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve14=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve14->Create(this,g_hf15AcuMed,0);
	//m_pcValve14->SetText(_T("14"),_T("14"));

	//btn.wID					= IDC_BTN_SERVICE_VALV15;	
	//btn.poPosition.x		= 310;
	//btn.poPosition.y		= 105;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve15=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve15->Create(this,g_hf15AcuMed,0);
	//m_pcValve15->SetText(_T("15"),_T("15"));


	//btn.wID					= IDC_BTN_SERVICE_VALV0;	
	//btn.poPosition.x		= 265;
	//btn.poPosition.y		= 105;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve0=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve0->Create(this,g_hf15AcuMed,0);
	//m_pcValve0->SetText(_T("0"),_T("0"));

	//btn.wID					= IDC_BTN_SERVICE_VALV1;	
	//btn.poPosition.x		= 220;
	//btn.poPosition.y		= 105;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve1=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve1->Create(this,g_hf15AcuMed,0);
	//m_pcValve1->SetText(_T("1"),_T("1"));

	//btn.wID					= IDC_BTN_SERVICE_VALV2;	
	//btn.poPosition.x		= 175;
	//btn.poPosition.y		= 105;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve2=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve2->Create(this,g_hf15AcuMed,0);
	//m_pcValve2->SetText(_T("2"),_T("2"));


	//btn.wID					= IDC_BTN_SERVICE_VALV3;	
	//btn.poPosition.x		= 130;
	//btn.poPosition.y		= 105;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve3=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve3->Create(this,g_hf15AcuMed,0);
	//m_pcValve3->SetText(_T("3"),_T("3"));


	////AIR
	//btn.wID					= IDC_BTN_SERVICE_VALV16;	
	//btn.poPosition.x		= 535;
	//btn.poPosition.y		= 230;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve16=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve16->Create(this,g_hf15AcuMed,0);
	//m_pcValve16->SetText(_T("16"),_T("16"));

	//btn.wID					= IDC_BTN_SERVICE_VALV17;	
	//btn.poPosition.x		= 490;
	//btn.poPosition.y		= 230;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve17=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve17->Create(this,g_hf15AcuMed,0);
	//m_pcValve17->SetText(_T("7"),_T("7"));

	//btn.wID					= IDC_BTN_SERVICE_VALV18;	
	//btn.poPosition.x		= 445;
	//btn.poPosition.y		= 230;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve18=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve18->Create(this,g_hf15AcuMed,0);
	//m_pcValve18->SetText(_T("18"),_T("18"));


	//btn.wID					= IDC_BTN_SERVICE_VALV19;	
	//btn.poPosition.x		= 400;
	//btn.poPosition.y		= 230;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve19=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve19->Create(this,g_hf15AcuMed,0);
	//m_pcValve19->SetText(_T("19"),_T("19"));

	//btn.wID					= IDC_BTN_SERVICE_VALV20;	
	//btn.poPosition.x		= 355;
	//btn.poPosition.y		= 230;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve20=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve20->Create(this,g_hf15AcuMed,0);
	//m_pcValve20->SetText(_T("20"),_T("20"));

	//btn.wID					= IDC_BTN_SERVICE_VALV21;	
	//btn.poPosition.x		= 310;
	//btn.poPosition.y		= 230;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve21=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve21->Create(this,g_hf15AcuMed,0);
	//m_pcValve21->SetText(_T("21"),_T("21"));


	//btn.wID					= IDC_BTN_SERVICE_VALV22;	
	//btn.poPosition.x		= 265;
	//btn.poPosition.y		= 230;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve22=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve22->Create(this,g_hf15AcuMed,0);
	//m_pcValve22->SetText(_T("22"),_T("22"));

	//btn.wID					= IDC_BTN_SERVICE_VALV23;	
	//btn.poPosition.x		= 220;
	//btn.poPosition.y		= 230;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve23=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve23->Create(this,g_hf15AcuMed,0);
	//m_pcValve23->SetText(_T("23"),_T("23"));

	//btn.wID					= IDC_BTN_SERVICE_VALV8;	
	//btn.poPosition.x		= 175;
	//btn.poPosition.y		= 230;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve8=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve8->Create(this,g_hf15AcuMed,0);
	//m_pcValve8->SetText(_T("8"),_T("8"));


	//btn.wID					= IDC_BTN_SERVICE_VALV9;	
	//btn.poPosition.x		= 130;
	//btn.poPosition.y		= 230;
	//btn.pcBmpUp				= m_pcValve_Up;
	//btn.pcBmpDown			= m_pcValve_Dw;
	//btn.pcBmpFocus			= m_pcValve_Up;
	//btn.pcBmpDisabled		= m_pcValve_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValve9=new CUpDwnBtn(btn,COLOR_TXTBTNUP);
	//m_pcValve9->Create(this,g_hf15AcuMed,0);
	//m_pcValve9->SetText(_T("9"),_T("9"));

	//

	//btn.wID					= IDC_BTN_VALV_OFF;	
	//btn.poPosition.x		= 10;//540;
	//btn.poPosition.y		= 65;
	//btn.pcBmpUp				= m_pcValveOff_Up;
	//btn.pcBmpDown			= m_pcValveOff_Dw;
	//btn.pcBmpFocus			= m_pcValveOff_Up;
	//btn.pcBmpDisabled		= m_pcValveOff_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcValveOff=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	//m_pcValveOff->Create(this,g_hf15AcuMed,0);
	//m_pcValveOff->SetText(_T("off"));


	//VALUE v;

	//v.nLowerLimit=0;
	//v.nUpperLimit=511;

	//v.nValue=0;
	//v.tText[0]=0;

	//btn.wID					= IDC_BTN_EXPVALVE;	
	//btn.poPosition.x		= 140;
	//btn.poPosition.y		= 340;
	//btn.pcBmpUp				= m_pcFg_Up;
	//btn.pcBmpDown			= m_pcFg_Dw;
	//btn.pcBmpFocus			= m_pcFg_Up;
	//btn.pcBmpDisabled		= m_pcFg_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcExpValve=new CUDBtn(btn,COLOR_TXTBTNUP,false);
	//m_pcExpValve->Create(this,g_hf7AcuNorm,g_hf27AcuBold,v);
	//m_pcExpValve->SetText(_T("ExpValve"),false);

	//btn.wID					= IDC_BTN_OPENVALVE;	
	//btn.poPosition.x		= 140;
	//btn.poPosition.y		= 430;
	//btn.pcBmpUp				= m_pcFg_Up;
	//btn.pcBmpDown			= m_pcFg_Dw;
	//btn.pcBmpFocus			= m_pcFg_Up;
	//btn.pcBmpDisabled		= m_pcFg_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcOpenValve=new CUDBtn(btn,COLOR_TXTBTNUP,false);
	//m_pcOpenValve->Create(this,g_hf7AcuNorm,g_hf27AcuBold,v);
	//m_pcOpenValve->SetText(_T("OpenValve"),false);

	//

	//
	//btn.wID					= IDC_BTN_EXPVALV_OPEN;	
	//btn.poPosition.x		= 240;
	//btn.poPosition.y		= 340;
	//btn.pcBmpUp				= m_pcOpen_Up;
	//btn.pcBmpDown			= m_pcOpen_Dw;
	//btn.pcBmpFocus			= m_pcOpen_Up;
	//btn.pcBmpDisabled		= m_pcOpen_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcExpValveOpen=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	//m_pcExpValveOpen->Create(this,g_hf15AcuMed,0);
	//m_pcExpValveOpen->SetText(_T("open"));

	//btn.wID					= IDC_BTN_EXPVALV_CLOSE;	
	//btn.poPosition.x		= 240;
	//btn.poPosition.y		= 380;
	//btn.pcBmpUp				= m_pcOpen_Up;
	//btn.pcBmpDown			= m_pcOpen_Dw;
	//btn.pcBmpFocus			= m_pcOpen_Up;
	//btn.pcBmpDisabled		= m_pcOpen_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcExpValveClose=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	//m_pcExpValveClose->Create(this,g_hf15AcuMed,0);
	//m_pcExpValveClose->SetText(_T("close"));

	//btn.wID					= IDC_BTN_OPVALV_OPEN;	
	//btn.poPosition.x		= 240;
	//btn.poPosition.y		= 430;
	//btn.pcBmpUp				= m_pcOpen_Up;
	//btn.pcBmpDown			= m_pcOpen_Dw;
	//btn.pcBmpFocus			= m_pcOpen_Up;
	//btn.pcBmpDisabled		= m_pcOpen_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcOpValveOpen=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	//m_pcOpValveOpen->Create(this,g_hf15AcuMed,0);
	//m_pcOpValveOpen->SetText(_T("open"));

	//btn.wID					= IDC_BTN_OPVALV_CLOSE;	
	//btn.poPosition.x		= 240;
	//btn.poPosition.y		= 470;
	//btn.pcBmpUp				= m_pcOpen_Up;
	//btn.pcBmpDown			= m_pcOpen_Dw;
	//btn.pcBmpFocus			= m_pcOpen_Up;
	//btn.pcBmpDisabled		= m_pcOpen_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcOpValveClose=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	//m_pcOpValveClose->Create(this,g_hf15AcuMed,0);
	//m_pcOpValveClose->SetText(_T("close"));

	//ClearFgBit(32);


	m_pcMenuBack->ShowWindow(SW_SHOW);

	SetTimer(SERVICETIMER, 500, NULL);

}

void CWndServiceValves::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);


	CBrush cbrBack(RGB(181,178,181));
	CBrush cbrWhite(RGB(255,255,255));
	CBrush cbrDark(RGB(140,140,140));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf10AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	rc.left = 100;  
	rc.top = 100;  
	rc.right  = 280;  
	rc.bottom = 200;
	CStringW cs = _T("Ventil Burn In:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	//**************************Valves**********************************//
	/*rc.left = 30;  
	rc.top = 20;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Valve control:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	

	rc.left = 60;  
	rc.top = 65;  
	rc.right  = 580;  
	rc.bottom = 90;
	FillRect(hdcMem,&rc,cbrDark);

	rc.left = 70;  
	rc.top = 70;  
	rc.right  = 330;  
	rc.bottom = 500;
	cs = _T("Oxy:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 60;  
	rc.top = 190;  
	rc.right  = 580;  
	rc.bottom = 215;
	FillRect(hdcMem,&rc,cbrDark);

	rc.left = 70;  
	rc.top = 195;  
	rc.right  = 330;  
	rc.bottom = 500;
	cs = _T("Air:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	SelectObject(hdcMem,g_hf7AcuNorm);*/


	
	/****************************OXY************************************/
	/*rc.left = 80;  
	rc.top = 120;  
	rc.right  = 330;  
	rc.bottom = 500;
	cs = _T("Valve");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 80;  
	rc.top = 160;  
	rc.right  = 330;  
	rc.bottom = 500;
	cs = _T("Func.");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 130;  
	rc.top = 150;  
	rc.right  = 165;  
	rc.bottom = 500;
	cs = _T("Byp.");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 175;  
	rc.top = 150;  
	rc.right  = 210;  
	rc.bottom = 500;
	cs = _T("16.0");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 220;  
	rc.top = 150;  
	rc.right  = 255;  
	rc.bottom = 500;
	cs = _T("8.0");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 265;  
	rc.top = 150;  
	rc.right  = 300;  
	rc.bottom = 500;
	cs = _T("4.0");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 310;  
	rc.top = 150;  
	rc.right  = 345;  
	rc.bottom = 500;
	cs = _T("2.0");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 355;  
	rc.top = 150;  
	rc.right  = 390;  
	rc.bottom = 500;
	cs = _T("1.0");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 400;  
	rc.top = 150;  
	rc.right  = 435;  
	rc.bottom = 500;
	cs = _T("0.5");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 445;  
	rc.top = 150;  
	rc.right  = 480;  
	rc.bottom = 500;
	cs = _T("0.25");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 490;  
	rc.top = 150;  
	rc.right  = 525;  
	rc.bottom = 500;
	cs = _T("0.125");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 535;  
	rc.top = 150;  
	rc.right  = 570;  
	rc.bottom = 500;
	cs = _T("0.063");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);*/


	////////////////////////////////////////////////////////////////
	/*rc.left = 130;  
	rc.top = 170;  
	rc.right  = 165;  
	rc.bottom = 500;
	cs = _T("Calib.");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 175;  
	rc.top = 170;  
	rc.right  = 210;  
	rc.bottom = 500;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 220;  
	rc.top = 170;  
	rc.right  = 255;  
	rc.bottom = 500;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 265;  
	rc.top = 170;  
	rc.right  = 300;  
	rc.bottom = 500;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 310;  
	rc.top = 170;  
	rc.right  = 345;  
	rc.bottom = 500;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 355;  
	rc.top = 170;  
	rc.right  = 390;  
	rc.bottom = 500;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 400;  
	rc.top = 170;  
	rc.right  = 435;  
	rc.bottom = 500;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 445;  
	rc.top = 170;  
	rc.right  = 480;  
	rc.bottom = 500;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 490;  
	rc.top = 170;  
	rc.right  = 525;  
	rc.bottom = 500;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 535;  
	rc.top = 170;  
	rc.right  = 570;  
	rc.bottom = 500;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);*/


	/****************************AIR************************************/
	/*SelectObject(hdcMem,g_hf7AcuNorm);

	rc.left = 80;  
	rc.top = 240;  
	rc.right  = 330;  
	rc.bottom = 500;
	cs = _T("Valve");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 80;  
	rc.top = 285;  
	rc.right  = 330;  
	rc.bottom = 500;
	cs = _T("Func.");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 130;  
	rc.top = 275;  
	rc.right  = 165;  
	rc.bottom = 500;
	cs = _T("Byp.");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 175;  
	rc.top = 275;  
	rc.right  = 210;  
	rc.bottom = 500;
	cs = _T("16.0");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 220;  
	rc.top = 275;  
	rc.right  = 255;  
	rc.bottom = 500;
	cs = _T("8.0");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 265;  
	rc.top = 275;  
	rc.right  = 300;  
	rc.bottom = 500;
	cs = _T("4.0");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 310;  
	rc.top = 275;  
	rc.right  = 345;  
	rc.bottom = 500;
	cs = _T("2.0");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 355;  
	rc.top = 275;  
	rc.right  = 390;  
	rc.bottom = 500;
	cs = _T("1.0");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 400;  
	rc.top = 275;  
	rc.right  = 435;  
	rc.bottom = 500;
	cs = _T("0.5");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 445;  
	rc.top = 275;  
	rc.right  = 480;  
	rc.bottom = 500;
	cs = _T("0.25");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 490;  
	rc.top = 275;  
	rc.right  = 525;  
	rc.bottom = 500;
	cs = _T("0.125");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 535;  
	rc.top = 275;  
	rc.right  = 570;  
	rc.bottom = 500;
	cs = _T("0.063");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);*/


	////////////////////////////////////////////////////////////////
	/*rc.left = 130;  
	rc.top = 295;  
	rc.right  = 165;  
	rc.bottom = 500;
	cs = _T("Calib.");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 175;  
	rc.top = 295;  
	rc.right  = 210;  
	rc.bottom = 500;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 220;  
	rc.top = 295;  
	rc.right  = 255;  
	rc.bottom = 500;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 265;  
	rc.top = 295;  
	rc.right  = 300;  
	rc.bottom = 500;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 310;  
	rc.top = 295;  
	rc.right  = 345;  
	rc.bottom = 500;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 355;  
	rc.top = 295;  
	rc.right  = 390;  
	rc.bottom = 500;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 400;  
	rc.top = 295;  
	rc.right  = 435;  
	rc.bottom = 500;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 445;  
	rc.top = 295;  
	rc.right  = 480;  
	rc.bottom = 500;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 490;  
	rc.top = 295;  
	rc.right  = 525;  
	rc.bottom = 500;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	rc.left = 535;  
	rc.top = 295;  
	rc.right  = 570;  
	rc.bottom = 500;
	cs = _T("l/min");
	DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);*/


	rc.left = 340;  
	rc.top = 350;  
	rc.right  = 520;  
	rc.bottom = 400;
	FillRect(hdcMem,&rc,cbrWhite);

	rc.left = 340;  
	rc.top = 440;  
	rc.right  = 520;  
	rc.bottom = 490;
	FillRect(hdcMem,&rc,cbrWhite);

	SelectObject(hdcMem,g_hf10AcuBold);

	rc.left = 350;  
	rc.top = 360;  
	rc.right  = 520;  
	rc.bottom = 500;
	cs = _T("Flow");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	
	rc.left = 350;  
	rc.top = 450;  
	rc.right  = 520;  
	rc.bottom = 500;
	cs = _T("Paw");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);



	SelectObject(hdcMem,g_hf7AcuNorm);

	rc.left = 350;  
	rc.top = 375;  
	rc.right  = 520;  
	rc.bottom = 500;
	cs = _T("[l/min]");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 350;  
	rc.top = 465;  
	rc.right  = 520;  
	rc.bottom = 500;
	cs = _T("[mbar]");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	SelectObject(hdcMem,g_hf27AcuBold);

	rc.left = 350;  
	rc.top = 360;  
	rc.right  = 510;  
	rc.bottom = 500;
	//int m_iFlow;
	//cs = _T("-0.002");
	cs.Format(_T("%0.3f"), CTlsFloat::Round(((double)m_iFlow)/G_FACTOR_FLOW, 3));//Achtung, tatsächlicher Meß-Wert ist um Faktor 2 größer
	DrawText(hdcMem,cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);


	rc.left = 350;  
	rc.top = 450;  
	rc.right  = 510;  
	rc.bottom = 500;
	cs.Format(_T("%0.1f"), CTlsFloat::Round(((double)m_iPaw)/10, 1));
	//cs = _T("4.8");
	//int m_iPaw;
	DrawText(hdcMem,cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);

	



	//dc.BitBlt(0,0,m_lX,m_lY,CDC::FromHandle(hdcMem),0,0,SRCCOPY);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	
	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}

//************************************
// Method:    OnDestroy
// FullName:  CWndServiceValves::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndServiceValves::OnDestroy()
{
	KillTimer(SERVICETIMER);

	CWndService::OnDestroy();
}

void CWndServiceValves::OnBnClickedVentilBurn()
{
	if(m_bVentilBurnRunning)
	{
		m_bVentilBurnRunning=false;
		getModel()->getDATAHANDLER()->SetVentilBurnRunning(false);
		getModel()->Send_MODE_OPTION1(false,true);
		m_pcVentilBurn->RefreshText(_T("ON"),_T("ON"));
	}
	else
	{
		m_bVentilBurnRunning=true;
		getModel()->getDATAHANDLER()->SetVentilBurnRunning(true);
		getModel()->Send_MODE_OPTION1(false,true);
		m_pcVentilBurn->RefreshText(_T("OFF"),_T("OFF"));
	}
}


//void CWndServiceValves::OnBnClickedValv0()
//{
//	if(m_pcValve0->IsDepressed())
//		SetFgBit(0);
//	else
//		ClearFgBit(0);
//}
//void CWndServiceValves::OnBnClickedValv1()
//{
//	if(m_pcValve1->IsDepressed())
//		SetFgBit(1);
//	else
//		ClearFgBit(1);
//}
//void CWndServiceValves::OnBnClickedValv2()
//{
//	if(m_pcValve2->IsDepressed())
//		SetFgBit(2);
//	else
//		ClearFgBit(2);
//}
//void CWndServiceValves::OnBnClickedValv3()
//{
//	if(m_pcValve3->IsDepressed())
//		SetFgBit(3);
//	else
//		ClearFgBit(3);
//}
//void CWndServiceValves::OnBnClickedValv8()
//{
//	if(m_pcValve8->IsDepressed())
//		SetFgBit(8);
//	else
//		ClearFgBit(8);
//}
//void CWndServiceValves::OnBnClickedValv9()
//{
//	if(m_pcValve9->IsDepressed())
//		SetFgBit(9);
//	else
//		ClearFgBit(9);
//}
//void CWndServiceValves::OnBnClickedValv10()
//{
//	if(m_pcValve10->IsDepressed())
//		SetFgBit(10);
//	else
//		ClearFgBit(10);
//}
//void CWndServiceValves::OnBnClickedValv11()
//{
//	if(m_pcValve11->IsDepressed())
//		SetFgBit(11);
//	else
//		ClearFgBit(11);
//}
//void CWndServiceValves::OnBnClickedValv12()
//{
//	if(m_pcValve12->IsDepressed())
//		SetFgBit(12);
//	else
//		ClearFgBit(12);
//}
//void CWndServiceValves::OnBnClickedValv13()
//{
//	if(m_pcValve13->IsDepressed())
//		SetFgBit(13);
//	else
//		ClearFgBit(13);
//}
//void CWndServiceValves::OnBnClickedValv14()
//{
//	if(m_pcValve14->IsDepressed())
//		SetFgBit(14);
//	else
//		ClearFgBit(14);
//}
//void CWndServiceValves::OnBnClickedValv15()
//{
//	if(m_pcValve15->IsDepressed())
//		SetFgBit(15);
//	else
//		ClearFgBit(15);
//}
//void CWndServiceValves::OnBnClickedValv16()
//{
//	if(m_pcValve16->IsDepressed())
//		SetFgBit(16);
//	else
//		ClearFgBit(16);
//}
//void CWndServiceValves::OnBnClickedValv17()
//{
//	if(m_pcValve17->IsDepressed())
//		SetFgBit(17);
//	else
//		ClearFgBit(17);
//}
//void CWndServiceValves::OnBnClickedValv18()
//{
//	if(m_pcValve18->IsDepressed())
//		SetFgBit(18);
//	else
//		ClearFgBit(18);
//}
//void CWndServiceValves::OnBnClickedValv19()
//{
//	if(m_pcValve19->IsDepressed())
//		SetFgBit(19);
//	else
//		ClearFgBit(19);
//}
//void CWndServiceValves::OnBnClickedValv20()
//{
//	if(m_pcValve20->IsDepressed())
//		SetFgBit(20);
//	else
//		ClearFgBit(20);
//}
//void CWndServiceValves::OnBnClickedValv21()
//{
//	if(m_pcValve21->IsDepressed())
//		SetFgBit(21);
//	else
//		ClearFgBit(21);
//}
//void CWndServiceValves::OnBnClickedValv22()
//{
//	if(m_pcValve22->IsDepressed())
//		SetFgBit(22);
//	else
//		ClearFgBit(22);
//}
//void CWndServiceValves::OnBnClickedValv23()
//{
//	if(m_pcValve23->IsDepressed())
//		SetFgBit(23);
//	else
//		ClearFgBit(23);
//
//}
//void CWndServiceValves::OnBnClickedValvOff()
//{
//	if(m_pcValve0->IsDepressed())
//		m_pcValve0->DrawDirectUp();
//	if(m_pcValve1->IsDepressed())
//		m_pcValve1->DrawDirectUp();
//	if(m_pcValve2->IsDepressed())
//		m_pcValve2->DrawDirectUp();
//	if(m_pcValve3->IsDepressed())
//		m_pcValve3->DrawDirectUp();
//	if(m_pcValve8->IsDepressed())
//		m_pcValve8->DrawDirectUp();
//	if(m_pcValve9->IsDepressed())
//		m_pcValve9->DrawDirectUp();
//	if(m_pcValve10->IsDepressed())
//		m_pcValve10->DrawDirectUp();
//	if(m_pcValve11->IsDepressed())
//		m_pcValve11->DrawDirectUp();
//	if(m_pcValve12->IsDepressed())
//		m_pcValve12->DrawDirectUp();
//	if(m_pcValve13->IsDepressed())
//		m_pcValve13->DrawDirectUp();
//	if(m_pcValve14->IsDepressed())
//		m_pcValve14->DrawDirectUp();
//	if(m_pcValve15->IsDepressed())
//		m_pcValve15->DrawDirectUp();
//	if(m_pcValve16->IsDepressed())
//		m_pcValve16->DrawDirectUp();
//	if(m_pcValve17->IsDepressed())
//		m_pcValve17->DrawDirectUp();
//	if(m_pcValve18->IsDepressed())
//		m_pcValve18->DrawDirectUp();
//	if(m_pcValve19->IsDepressed())
//		m_pcValve19->DrawDirectUp();
//	if(m_pcValve20->IsDepressed())
//		m_pcValve20->DrawDirectUp();
//	if(m_pcValve21->IsDepressed())
//		m_pcValve21->DrawDirectUp();
//	if(m_pcValve22->IsDepressed())
//		m_pcValve22->DrawDirectUp();
//	if(m_pcValve23->IsDepressed())
//		m_pcValve23->DrawDirectUp();
//	ClearFgBit(32);
//}
//void CWndServiceValves::OnBnClickedExpValveOpen()
//{
//	VALUE v;
//	v.nValue=0;
//	v.nLowerLimit=0;
//	v.nUpperLimit=511;
//	v.tText[0]=0;
//	m_pcExpValve->SetValue(v, true);
//	m_pcExpValve->SetFocus();
//	getModel()->getSERIAL()->Send_EXP_VALVE(0);
//}
//void CWndServiceValves::OnBnClickedExpValveClose()
//{
//	VALUE v;
//	v.nValue=511;
//	v.nLowerLimit=0;
//	v.nUpperLimit=511;
//	v.tText[0]=0;
//	m_pcExpValve->SetValue(v, true);
//	m_pcExpValve->SetFocus();
//	getModel()->getSERIAL()->Send_EXP_VALVE(511);
//}
//void CWndServiceValves::OnBnClickedOpValveOpen()
//{
//	VALUE v;
//	v.nValue=0;
//	v.nLowerLimit=0;
//	v.nUpperLimit=511;
//	v.tText[0]=0;
//	m_pcOpenValve->SetValue(v, true);
//	m_pcOpenValve->SetFocus();
//	getModel()->getSERIAL()->Send_OPEN_VALVE(0);
//}
//void CWndServiceValves::OnBnClickedOpValveClose()
//{
//	
//	VALUE v;
//	v.nValue=511;
//	v.nLowerLimit=0;
//	v.nUpperLimit=511;
//	v.tText[0]=0;
//	m_pcOpenValve->SetValue(v, true);
//	m_pcOpenValve->SetFocus();
//	getModel()->getSERIAL()->Send_OPEN_VALVE(511);
//}

LRESULT CWndServiceValves::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_UPDATE_UDBTN:
		{
			switch(lParam)
			{
			case IDC_BTN_OPENVALVE:
				{
					int iVal=wParam;
					getModel()->getSERIAL()->Send_OPEN_VALVE(iVal);
					return 1;
				}
				break;
			case IDC_BTN_EXPVALVE:
				{
					int iVal=wParam;
					getModel()->getSERIAL()->Send_EXP_VALVE(iVal);
					return 1;
				}
				break;
			}
			
		}
		break;
	}
	return CWndService::WindowProc(message, wParam, lParam);
}


// **************************************************************************
// 
// **************************************************************************
void CWndServiceValves::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==SERVICETIMER)
	{
		m_iFlow=getModel()->getDATAHANDLER()->getMessureDataFLOW();
		m_iPaw=getModel()->getDATAHANDLER()->getMessureDataPRESSURE();

		Draw();

	}

	CWnd::OnTimer(nIDEvent);
}

