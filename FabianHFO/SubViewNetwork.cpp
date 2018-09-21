// SubViewNetwork.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "SubViewNetwork.h"
#include "TlsFile.h"
#include "TlsRegistry.h"
//#include "DlgMessageBox.h"

/**********************************************************************************************//**
 * A macro that defines timechange
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define TIMECHANGE	600

/**********************************************************************************************//**
 * A macro that defines color txtbtnup
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNUP				0x00000000

/**********************************************************************************************//**
 * A macro that defines color txtbtndw
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNDW				0x00FFFFFF

/**********************************************************************************************//**
 * A macro that defines color txtbtnfc
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNFC				0x00000000

//global font objects
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf8AcuNorm;
extern HFONT g_hf8AcuBold;
extern HFONT g_hf10AcuBold;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf13AcuBold;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf14AcuMed;
extern HFONT g_hf15AcuMed;
extern HFONT g_hf17AcuBold;
extern HFONT g_hf19AcuMed;
extern HFONT g_hf21AcuBold;
extern HFONT g_hf23AcuBold;
extern HFONT g_hf25AcuMed;
extern HFONT g_hf27AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf33AcuBold;
extern HFONT g_hf43AcuBold;
extern HFONT g_hf53AcuBold;

/**********************************************************************************************//**
 * CSubViewNetwork
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CSubViewNetwork, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CSubViewNetwork class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CSubViewNetwork::CSubViewNetwork()
{
	m_pModel=NULL;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_lX				= 0;
	m_lY				= 0;
	m_lXo				= 0;
	m_lYo				= 0;

	m_pcRegionTopLeCorner=NULL;
	m_pcRegionTopRiCorner=NULL;
	m_pcRegionBotLeCorner=NULL;
	m_pcRegionBotRiCorner=NULL;

	m_pcSmallUp_Up=NULL;
	m_pcSmallUp_Dw=NULL;
	m_pcSmallDw_Up=NULL;
	m_pcSmallDw_Dw=NULL;
	m_pcOkGreen_Up=NULL;
	m_pcOkGreen_Dw=NULL;
	m_pcOkRed_Up=NULL;
	m_pcOkRed_Dw=NULL;

	/*m_pcWave_Up=NULL;
	m_pcWave_Dw=NULL;
	m_pcWave_Fc=NULL;

	m_pcWave1NextUp=NULL;
	m_pcWave1NextDwn=NULL;

	m_pcWave2NextUp=NULL;
	m_pcWave2NextDwn=NULL;*/

	m_uiPort=getModel()->getCONFIG()->GetEthernetPort();
	m_iPDMSProtocol=getModel()->getCONFIG()->GetPDMSprotocol();

	CTlsRegistry regIP(_T("HKLM\\Comm\\dm9ce1\\parms\\tcpip"),true);
	m_szIP=regIP.ReadString(_T("IpAddress"), _T("0.0.0.0"));
	m_szGateway=regIP.ReadString(_T("DefaultGateway"), _T("0.0.0.0"));
	m_szSubnet=regIP.ReadString(_T("Subnetmask"), _T("0.0.0.0"));
	if(regIP.ReadDWORD(_T("EnableDHCP"), 1)==1)
		m_bDHCP=true;
	else
		m_bDHCP=false;

	m_bExit=false;

	
	
}

/**********************************************************************************************//**
 * Finalizes an instance of the CSubViewNetwork class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CSubViewNetwork::~CSubViewNetwork()
{
	delete m_pcRegionTopLeCorner;
	m_pcRegionTopLeCorner=NULL;
	delete m_pcRegionTopRiCorner;
	m_pcRegionTopRiCorner=NULL;
	delete m_pcRegionBotLeCorner;
	m_pcRegionBotLeCorner=NULL;
	delete m_pcRegionBotRiCorner;
	m_pcRegionBotRiCorner=NULL;

	delete m_pcSmallUp_Up;
	m_pcSmallUp_Up=NULL;
	delete m_pcSmallUp_Dw;
	m_pcSmallUp_Dw=NULL;
	delete m_pcSmallDw_Up;
	m_pcSmallDw_Up=NULL;
	delete m_pcSmallDw_Dw;
	m_pcSmallDw_Dw=NULL;
	delete m_pcOkGreen_Up;
	m_pcOkGreen_Up=NULL;
	delete m_pcOkGreen_Dw;
	m_pcOkGreen_Dw=NULL;
	delete m_pcOkRed_Up;
	m_pcOkRed_Up=NULL;
	delete m_pcOkRed_Dw;
	m_pcOkRed_Dw=NULL;

	/*delete m_pcWave_Up;
	m_pcWave_Up=NULL;
	delete m_pcWave_Dw;
	m_pcWave_Dw=NULL;
	delete m_pcWave_Fc;
	m_pcWave_Fc=NULL;

	delete m_pcWave1NextUp;
	m_pcWave1NextUp=NULL;
	delete m_pcWave1NextDwn;
	m_pcWave1NextDwn=NULL;

	delete m_pcWave2NextUp;
	m_pcWave2NextUp=NULL;
	delete m_pcWave2NextDwn;
	m_pcWave2NextDwn=NULL;
	
	delete m_pbtnOk;
	m_pbtnOk=NULL;*/
}


BEGIN_MESSAGE_MAP(CSubViewNetwork, CWnd)
	ON_WM_DESTROY()
	//ON_WM_TIMER()
	/*ON_BN_CLICKED(IDC_BTN_SETUP_WAVE1, &CSubViewNetwork::OnBnClickedWave1)
	ON_BN_CLICKED(IDC_BTN_SETUP_WAVE2, &CSubViewNetwork::OnBnClickedWave2)
	ON_BN_CLICKED(IDC_BTN_SETUP_DT_OK, &CSubViewNetwork::OnBnClickedSetWave)*/
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CSubViewNetwork message handlers
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CSubViewNetwork::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * Creates a window
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

bool CSubViewNetwork::CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
{
	if (!Create(pParentWnd, rc, nID, pContext))
	{
		return false;
	}

	Initialize();

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

BOOL CSubViewNetwork::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext*) 
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

		//CBrush cbrBack(RGB(210,210,255));
		CBrush cbrBack(BACKGND);
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		m_pcSmallUp_Up= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUP1_UPUP);
		m_pcSmallUp_Dw= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUP1_UPDW);
		m_pcSmallDw_Up= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUP1_DWUP);
		m_pcSmallDw_Dw= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUP1_DWDW);

		m_pcOkGreen_Up= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUP_OKGREENUP);
		m_pcOkGreen_Dw= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUP_OKGREENDW);
		m_pcOkRed_Up= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUP_OKREDUP);
		m_pcOkRed_Dw= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_SETUP_OKREDDW);

		/*m_pcWave_Up= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_WAVE_UP);
		m_pcWave_Dw= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_WAVE_DW);
		m_pcWave_Fc= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_WAVE_FC);*/


		m_pcRegionTopLeCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_LE_TOP_CORNER);
		m_pcRegionTopRiCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_RI_TOP_CORNER);
		m_pcRegionBotLeCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_LE_BOT_CORNER);
		m_pcRegionBotRiCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_RI_BOT_CORNER);

		CStringW cs=_T("");
		//BTN btn;

		//btn.wID					= IDC_BTN_SETUP_WAVE1;	
		//btn.poPosition.x		= 370;
		//btn.poPosition.y		= 141;
		//btn.pcBmpUp				= m_pcWave_Up;
		//btn.pcBmpDown			= m_pcWave_Dw;
		//btn.pcBmpFocus			= m_pcWave_Fc;
		//btn.pcBmpDisabled		= m_pcWave_Up;
		//btn.dwFormat			= DT_VCENTER|DT_CENTER;

		//m_pbtnWave1=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
		//m_pbtnWave1->Create(this,g_hf23AcuBold,0);
		//cs = GetWaveString(m_iWave1);
		//m_pbtnWave1->SetText(cs);
		//m_pbtnWave1->ShowWindow(SW_SHOW);
		//m_plStaticBtn.AddTail(m_pbtnWave1);
		//


		//btn.wID					= IDC_BTN_SETUP_WAVE2;	
		//btn.poPosition.x		= 370;
		//btn.poPosition.y		= 243;
		//btn.pcBmpUp				= m_pcWave_Up;
		//btn.pcBmpDown			= m_pcWave_Dw;
		//btn.pcBmpFocus			= m_pcWave_Fc;
		//btn.pcBmpDisabled		= m_pcWave_Up;
		//btn.dwFormat			= DT_VCENTER|DT_CENTER;

		//m_pbtnWave2=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
		//m_pbtnWave2->Create(this,g_hf23AcuBold,0);
		//cs = GetWaveString(m_iWave2);
		//m_pbtnWave2->SetText(cs);
		//m_pbtnWave2->ShowWindow(SW_SHOW);
		//m_plStaticBtn.AddTail(m_pbtnWave2);


		///****************wave1 up/down**************************/
		//btn.wID					= IDC_BTN_WAVE1_NEXTUP;	
		//btn.poPosition.x		= 570;
		//btn.poPosition.y		= 130;
		//btn.pcBmpUp				= m_pcSmallUp_Up;
		//btn.pcBmpDown			= m_pcSmallUp_Dw;
		//btn.pcBmpFocus			= m_pcSmallUp_Up;
		//btn.pcBmpDisabled		= m_pcSmallUp_Up;
		//btn.dwFormat			= DT_VCENTER|DT_CENTER;

		//m_pcWave1NextUp=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
		//m_pcWave1NextUp->Create(this,g_hf21AcuBold,0);
		//m_pcWave1NextUp->SetText(_T(""));
		//m_pcWave1NextUp->ShowWindow(SW_SHOW);

		//btn.wID					= IDC_BTN_WAVE1_NEXTDWN;	
		//btn.poPosition.x		= 570;
		//btn.poPosition.y		= 172;
		//btn.pcBmpUp				= m_pcSmallDw_Up;
		//btn.pcBmpDown			= m_pcSmallDw_Dw;
		//btn.pcBmpFocus			= m_pcSmallDw_Up;
		//btn.pcBmpDisabled		= m_pcSmallDw_Up;
		//btn.dwFormat			= DT_VCENTER|DT_CENTER;

		//m_pcWave1NextDwn=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
		//m_pcWave1NextDwn->Create(this,g_hf21AcuBold,0);
		//m_pcWave1NextDwn->SetText(_T(""));
		//m_pcWave1NextDwn->ShowWindow(SW_SHOW);




		///****************wave2 up/down**************************/
		//btn.wID					= IDC_BTN_WAVE2_NEXTUP;	
		//btn.poPosition.x		= 570;
		//btn.poPosition.y		= 232;
		//btn.pcBmpUp				= m_pcSmallUp_Up;
		//btn.pcBmpDown			= m_pcSmallUp_Dw;
		//btn.pcBmpFocus			= m_pcSmallUp_Up;
		//btn.pcBmpDisabled		= m_pcSmallUp_Up;
		//btn.dwFormat			= DT_VCENTER|DT_CENTER;

		//m_pcWave2NextUp=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
		//m_pcWave2NextUp->Create(this,g_hf21AcuBold,0);
		//m_pcWave2NextUp->SetText(_T(""));
		//m_pcWave2NextUp->ShowWindow(SW_SHOW);

		//btn.wID					= IDC_BTN_WAVE2_NEXTDWN;	
		//btn.poPosition.x		= 570;
		//btn.poPosition.y		= 274;
		//btn.pcBmpUp				= m_pcSmallDw_Up;
		//btn.pcBmpDown			= m_pcSmallDw_Dw;
		//btn.pcBmpFocus			= m_pcSmallDw_Up;
		//btn.pcBmpDisabled		= m_pcSmallDw_Up;
		//btn.dwFormat			= DT_VCENTER|DT_CENTER;

		//m_pcWave2NextDwn=new CDTUpDwnBtn(btn,COLOR_TXTBTNUP);
		//m_pcWave2NextDwn->Create(this,g_hf21AcuBold,0);
		//m_pcWave2NextDwn->SetText(_T(""));
		//m_pcWave2NextDwn->ShowWindow(SW_SHOW);




		//btn.wID					= IDC_BTN_SETUP_DT_OK;	
		//btn.poPosition.x		= 540;
		//btn.poPosition.y		= 375;
		//btn.pcBmpUp				= m_pcOkGreen_Up;
		//btn.pcBmpDown			= m_pcOkGreen_Dw;
		//btn.pcBmpFocus			= m_pcOkGreen_Up;
		//btn.pcBmpDisabled		= m_pcOkGreen_Up;
		//btn.dwFormat			= DT_VCENTER|DT_CENTER;

		//m_pbtnOk=new CBtn(btn,COLOR_TXTBTNUP);
		//m_pbtnOk->Create(this,g_hf21AcuBold,0);
		//m_pbtnOk->SetText(_T(""));
		//m_pbtnOk->ShowWindow(SW_SHOW);


		SelectObject(m_hDC,hbrprev);
		SelectObject(m_hDC,hpenprev);

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
 **************************************************************************************************/

void CSubViewNetwork::Initialize()
{
	/*if(m_iPDMSProtocol!=VUELINK)
	{
		m_pcWave1NextUp->ShowWindow(SW_HIDE);
		m_pcWave1NextDwn->ShowWindow(SW_HIDE);

		m_pcWave2NextUp->ShowWindow(SW_HIDE);
		m_pcWave2NextDwn->ShowWindow(SW_HIDE);

		m_pbtnWave1->ShowWindow(SW_HIDE);
		m_pbtnWave2->ShowWindow(SW_HIDE);

		m_pbtnOk->ShowWindow(SW_HIDE);
	}
	else
	{
		m_pcWave1NextUp->ShowWindow(SW_SHOW);
		m_pcWave1NextDwn->ShowWindow(SW_SHOW);

		m_pcWave2NextUp->ShowWindow(SW_SHOW);
		m_pcWave2NextDwn->ShowWindow(SW_SHOW);

		m_pbtnWave1->ShowWindow(SW_SHOW);
		m_pbtnWave2->ShowWindow(SW_SHOW);

		m_pbtnOk->ShowWindow(SW_SHOW);
	}*/
	

	

}

/**********************************************************************************************//**
 * Shows
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CSubViewNetwork::Show(bool bShow)
{
	if(bShow)
	{
		ShowWindow(SW_SHOW);
		Draw();

		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewNetwork::OnDestroy()
{
	m_bExit=true;

	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);
}

/**********************************************************************************************//**
 * Draws this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewNetwork::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	CBrush cbrBack(BACKGND);
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf14AcuMed);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	CBrush cbrFill;
	cbrFill.CreateSolidBrush(RGB(181,178,181));
	SelectObject(hdcMem,cbrFill);

	Rectangle(hdcMem, 100, 30, 680, 465);
	Rectangle(hdcMem, 120, 10, 660, 30);
	Rectangle(hdcMem, 120, 465, 660, 485);
	if(m_pcRegionTopLeCorner)
		m_pcRegionTopLeCorner->Draw(hdcMem,100,10);
	if(m_pcRegionTopRiCorner)
		m_pcRegionTopRiCorner->Draw(hdcMem,660,10);
	if(m_pcRegionBotLeCorner)
		m_pcRegionBotLeCorner->Draw(hdcMem,100,465);
	if(m_pcRegionBotRiCorner)
		m_pcRegionBotRiCorner->Draw(hdcMem,660,465);


	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	CBrush cbrDarkRound(RGB(140,140,140));
	
	SelectObject(hdcMem,cbrRound);
	RoundRect(hdcMem, 110, 20, 670, 350,20,20);
	SelectObject(hdcMem,cbrDarkRound);
	RoundRect(hdcMem, 110, 20, 670, 50,20,20);

	SetTextColor(hdcMem,RGB(255,255,255));

	rc.left = 130;  
	rc.top = 25;  
	rc.right  = 420;  
	rc.bottom = 500;
	CStringW cs = getModel()->GetLanguageString(IDS_BTN_MNU_PDMS);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	SetTextColor(hdcMem,0x0000000);

	SetTextColor(hdcMem,RGB(255,255,255));

	rc.left = 130;  
	rc.top = 25;  
	rc.right  = 420;  
	rc.bottom = 500;
	cs = getModel()->GetLanguageString(IDS_BTN_MNU_PDMS);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	SetTextColor(hdcMem,0x0000000);

	/****************************************************/

	SetTextColor(hdcMem,0x0000000);

	rc.left = 130;  
	rc.top = 75;  
	rc.right  = 420;  
	rc.bottom = 500;
	cs = getModel()->GetLanguageString(IDS_TXT_PDMS_VERSION);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 300;  
	rc.top = 75;  
	rc.right  = 660;  
	rc.bottom = 500; 

	if(getModel()->getCONFIG()->GetAcuLinkVersion()==ALINKVERS_3)
	{
		DrawText(hdcMem,_T("3.x"),-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}
	else
	{
		DrawText(hdcMem,_T("4.x"),-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}


	rc.left = 130;  
	rc.top = 100;  
	rc.right  = 420;  
	rc.bottom = 500;
	cs = getModel()->GetLanguageString(IDS_TXT_PDMS_PROTOCOL);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 300;  
	rc.top = 100;  
	rc.right  = 660;  
	rc.bottom = 500; 

	if(m_iPDMSProtocol==ACL_NOPDMS)
	{
		cs=getModel()->GetLanguageString(IDS_TXT_DISABLED);
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}
	else if(m_iPDMSProtocol==ACL_SERIAL_ASCII)
	{
		cs=_T("Serial (no wave data)");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}
	else if(m_iPDMSProtocol==ACL_SERIAL_WAVE)
	{
		cs=_T("Serial (with wave data)");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}
	else if(m_iPDMSProtocol==ACL_ETHERNET_WAVE)
	{
		cs=_T("Ethernet (with wave data)");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}
	else if(m_iPDMSProtocol==ACL_SERIAL_IVOI)
	{
		cs=_T("VueLink / IntelliBridge");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}
	else if(m_iPDMSProtocol==ACL_ETHERNET)
	{
		cs=_T("Ethernet (no wave data)");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}
	else if(m_iPDMSProtocol==ACL_TERMINAL_REMOTE)
	{
		cs=_T("Terminal REMOTE");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}
	else if(m_iPDMSProtocol==ACL_TERMINAL_WAVE)
	{
		cs=_T("Terminal WAVE");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}
	else
	{
		cs=_T("-none-");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}

	
	
	/**************version*******************/
	rc.left = 130;  
	rc.top = 125;  
	rc.right  = 420;  
	rc.bottom = 500;
	cs = getModel()->GetLanguageString(IDS_TXT_SOFTVERS);
	cs += _T(":");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 300;  
	rc.top = 125;  
	rc.right  = 660;  
	rc.bottom = 500; 

	cs=getModel()->getAcuLink()->getExeVersion();
	cs+= _T(" / ");
	cs+=getModel()->getAcuLink()->getDllVersion();
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	

	if(m_iPDMSProtocol==ACL_SERIAL_ASCII)
	{
		/**********************interface*********************/
		rc.left = 130;  
		rc.top = 150;  
		rc.right  = 420;  
		rc.bottom = 500;
		cs = getModel()->GetLanguageString(IDS_TXT_PDMS_INTERFACE);
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


		rc.left = 300;  
		rc.top = 150;  
		rc.right  = 660;  
		rc.bottom = 500; 

		cs=getModel()->GetLanguageString(IDS_TXT_PDMS_RS232);
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}
	else if(m_iPDMSProtocol==ACL_SERIAL_WAVE)
	{
		/**********************interface*********************/
		rc.left = 130;  
		rc.top = 150;  
		rc.right  = 420;  
		rc.bottom = 500;
		cs = getModel()->GetLanguageString(IDS_TXT_PDMS_INTERFACE);
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


		rc.left = 300;  
		rc.top = 150;  
		rc.right  = 660;  
		rc.bottom = 500; 

		//cs=getModel()->GetLanguageString(IDS_TXT_PDMS_RS232);
		cs=_T("Serial (with wave data)");
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}
	//else if(m_iPDMSProtocol==ACL_ETHERNET_WAVE)
	//{xxxxxxxxxxxxxxxxxxxxxxxx
	//	/**********************interface*********************/
	//	rc.left = 130;  
	//	rc.top = 150;  
	//	rc.right  = 420;  
	//	rc.bottom = 500;
	//	cs = getModel()->GetLanguageString(IDS_TXT_PDMS_INTERFACE);
	//	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	//	rc.left = 300;  
	//	rc.top = 150;  
	//	rc.right  = 660;  
	//	rc.bottom = 500; 

	//	//cs=getModel()->GetLanguageString(IDS_TXT_PDMS_RS232);
	//	cs=_T("Ethernet (with wave data)");
	//	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	//}
	else if(m_iPDMSProtocol==ACL_ETHERNET)
	{
		/**********************interface*********************/
		rc.left = 130;  
		rc.top = 150;  
		rc.right  = 420;  
		rc.bottom = 500;
		cs = getModel()->GetLanguageString(IDS_TXT_PDMS_INTERFACE);
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


		rc.left = 300;  
		rc.top = 150;  
		rc.right  = 660;  
		rc.bottom = 500; 

		cs=getModel()->GetLanguageString(IDS_TXT_PDMS_RJ45);
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		/**********************IP config******************************/
		rc.left = 130;  
		rc.top = 175;  
		rc.right  = 420;  
		rc.bottom = 500;
		cs = getModel()->GetLanguageString(IDS_TXT_PDMS_IP);
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


		rc.left = 300;  
		rc.top = 175;  
		rc.right  = 660;  
		rc.bottom = 500; 
		DrawText(hdcMem,m_szIP,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


		rc.left = 130;  
		rc.top = 200;  
		rc.right  = 420;  
		rc.bottom = 500;
		cs = getModel()->GetLanguageString(IDS_TXT_PDMS_DEFAULTGATEW);
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


		rc.left = 300;  
		rc.top = 200;  
		rc.right  = 660;  
		rc.bottom = 500; 
		DrawText(hdcMem,m_szGateway,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		rc.left = 130;  
		rc.top = 225;  
		rc.right  = 420;  
		rc.bottom = 500;
		cs = getModel()->GetLanguageString(IDS_TXT_PDMS_SUBNETMASK);
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


		rc.left = 300;  
		rc.top = 225;  
		rc.right  = 660;  
		rc.bottom = 500; 
		DrawText(hdcMem,m_szSubnet,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

		rc.left = 130;  
		rc.top = 250;  
		rc.right  = 420;  
		rc.bottom = 500;
		cs = getModel()->GetLanguageString(IDS_TXT_PDMS_DHCP);
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


		rc.left = 300;  
		rc.top = 250;  
		rc.right  = 660;  
		rc.bottom = 500; 
		if(m_bDHCP)
			cs=getModel()->GetLanguageString(IDS_TXT_ENABLED);
		else
			cs=getModel()->GetLanguageString(IDS_TXT_DISABLED);
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


		//m_uiPort
		rc.left = 130;  
		rc.top = 275;  
		rc.right  = 420;  
		rc.bottom = 500;
		cs = _T("Port:");
		//cs = getModel()->GetLanguageString(IDS_TXT_PDMS_PORT);
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


		rc.left = 300;  
		rc.top = 275;  
		rc.right  = 660;  
		rc.bottom = 500; 
		cs.Format(_T("%d"), m_uiPort);
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}
	else if(m_iPDMSProtocol==ACL_SERIAL_IVOI)
	{
		/**********************interface*********************/
		rc.left = 130;  
		rc.top = 150;  
		rc.right  = 420;  
		rc.bottom = 500;
		cs = getModel()->GetLanguageString(IDS_TXT_PDMS_INTERFACE);
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


		rc.left = 300;  
		rc.top = 150;  
		rc.right  = 660;  
		rc.bottom = 500; 

		cs=getModel()->GetLanguageString(IDS_TXT_PDMS_RS232);
		DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	}

	//dc.BitBlt(0,0,m_lX,m_lY,CDC::FromHandle(hdcMem),0,0,SRCCOPY);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	//rkuNEWFIX
	penLine.DeleteObject();

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}


