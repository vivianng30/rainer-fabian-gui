// SubViewSystem.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "SubViewSystem.h"

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
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

// CSubViewSystem

IMPLEMENT_DYNAMIC(CSubViewSystem, CWnd)

CSubViewSystem::CSubViewSystem()
{
	m_pModel=NULL;
	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;

	getModel()->getDATAHANDLER()->getMainboardData(&m_dataMainboard);
	m_szVersion = _T("fabianHFO ");
	m_szVersion += getModel()->GetMainVersion();
	m_szUniqueID=getModel()->GetUniqueID();
	m_iOpMinBatt=getModel()->getDATAHANDLER()->getOpTimeBatteryMin(false);
	m_iOpMinDev=getModel()->getDATAHANDLER()->getOpTimeDeviceMin(false);
	m_iOpMinHFO=getModel()->getDATAHANDLER()->getOpTimeHFOMin(false);
	//m_iSystemErrors=getModel()->getCONFIG()->GetSystemErrors();
	m_szLangVersion=getModel()->GetLanguageString(IDS_VERSION_LANGUAGE);
	m_iMainVersion=getModel()->getCONFIG()->GetMainBoardVersion();
}

CSubViewSystem::~CSubViewSystem()
{
}


BEGIN_MESSAGE_MAP(CSubViewSystem, CWnd)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CSubViewSystem message handlers

// **************************************************************************
// 
// **************************************************************************
CMVModel *CSubViewSystem::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}
// **************************************************************************
// 
// **************************************************************************
bool CSubViewSystem::CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
{
	if (!Create(pParentWnd, rc, nID, pContext))
	{
		return false;
	}

	Initialize();

	return true;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CSubViewSystem::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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
		CBrush cbrFill;
		cbrFill.CreateSolidBrush(RGB(181,178,181));
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		RECT rcCl;
		GetClientRect(&rcCl);

		rcCl.left = 5; 
		rcCl.right = 795; 
		rcCl.top = 5; 
		rcCl.bottom = 500; 
		FillRect(m_hDC,&rcCl,cbrFill);

		SelectObject(m_hDC,hbrprev);
		SelectObject(m_hDC,hpenprev);

		return 1;
	}
	else
		return 0;
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewSystem::Initialize()
{
	//CClientDC dc(this);


	SetTimer(SERVICETIMER, 30000, NULL);

}

// **************************************************************************
// 
// **************************************************************************
void CSubViewSystem::Show(bool bShow)
{
	if(bShow)
	{
		ShowWindow(SW_SHOW);
		//SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);

		getModel()->getDATAHANDLER()->getMainboardData(&m_dataMainboard);
		m_szVersion = _T("fabianHFO ");
		m_szVersion += getModel()->GetMainVersion();
		m_szUniqueID=getModel()->GetUniqueID();
		m_iOpMinBatt=getModel()->getDATAHANDLER()->getOpTimeBatteryMin(false);
		m_iOpMinDev=getModel()->getDATAHANDLER()->getOpTimeDeviceMin(false);
		m_iOpMinHFO=getModel()->getDATAHANDLER()->getOpTimeHFOMin(false);
		//m_iSystemErrors=getModel()->getCONFIG()->GetSystemErrors();

		Draw();

		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	else
	{
		ShowWindow(SW_HIDE);
		//SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);
	}
}

//************************************
// Method:    OnDestroy
// FullName:  CSubViewSystem::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CSubViewSystem::OnDestroy()
{
	KillTimer(SERVICETIMER);

	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewSystem::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);



	CBrush cbrBack(RGB(181,178,181));
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf21AcuBold);
	HFONT hPrevFontDc=(HFONT)SelectObject(dc.m_hDC,g_hf21AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	//Rectangle(hdcMem, 0, 0, m_lX, m_lY);
	BitBlt(hdcMem, 0, 0, m_lX, m_lY, m_hDC, 0, 0, SRCCOPY);

	//CDC* pDCMem=CDC::FromHandle(hdcMem);


	//CStringW cs = _T("System-Informationen:");
	CStringW cs = getModel()->GetLanguageString(IDS_TXT_SYSINF)+_T(":");
	CSize sz = dc.GetTextExtent(cs);

	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	SelectObject(hdcMem,cbrRound);
	RoundRect(hdcMem, 20, 20, 40+sz.cx, 80,20,20);

	/*rc.left = 20;  
	rc.top = 15;  
	rc.right  = rc.left+sz.cx+20;  
	rc.bottom = 45;
	FillRect(hdcMem,&rc,(HBRUSH)GetStockObject(WHITE_BRUSH));*/

	rc.left = 30;  
	rc.top = 30;  
	rc.right  = 582;  
	rc.bottom = 70;

	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);


	SelectObject(hdcMem,g_hf13AcuBold);

	rc.left = 60;  
	rc.top = 100;  
	rc.right  = 360;  
	rc.bottom = 400;
	//cs = _T("Operating hours device:");
	cs = getModel()->GetLanguageString(IDS_TXT_OPDEV)+_T(":");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//getModel()->getDATAHANDLER()->RefreshOpTime(false);
	//DWORD iOpMinDev=getModel()->getDATAHANDLER()->GetOpTimeDevice();
	int iOpHrsDev = m_iOpMinDev/60;
	if(iOpHrsDev>0)
		m_iOpMinDev=m_iOpMinDev-(iOpHrsDev*60);

	rc.left = 360;  
	rc.top = 100;  
	rc.right  = 800;  
	rc.bottom = 400;
	//cs = _T("200 h");
	cs.Format(_T("%d %s %d %s"),iOpHrsDev,getModel()->GetLanguageString(IDS_UNIT_HRS),m_iOpMinDev,getModel()->GetLanguageString(IDS_UNIT_MIN));
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 60;  
	rc.top = 130;  
	rc.right  = 360;  
	rc.bottom = 400;
	//cs = _T("Operating hours battery:");
	cs = getModel()->GetLanguageString(IDS_TXT_OPACCU)+_T(":");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//DWORD iOpMinBatt=getModel()->getDATAHANDLER()->GetOpTimeBattery();
	int iOpHrsBatt = m_iOpMinBatt/60;
	if(iOpHrsBatt>0)
		m_iOpMinBatt=m_iOpMinBatt-(iOpHrsBatt*60);


	rc.left = 360;  
	rc.top = 130;  
	rc.right  = 800;  
	rc.bottom = 400;
	cs.Format(_T("%d %s %d %s"),iOpHrsBatt,getModel()->GetLanguageString(IDS_UNIT_HRS),m_iOpMinBatt,getModel()->GetLanguageString(IDS_UNIT_MIN));
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);



	rc.left = 60;  
	rc.top = 160;  
	rc.right  = 360;  
	rc.bottom = 400;
	cs = getModel()->GetLanguageString(IDS_TXT_OPHFO)+_T(":");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//DWORD iOpMinBatt=getModel()->getDATAHANDLER()->GetOpTimeBattery();
	int iOpHrsHFO = m_iOpMinHFO/60;
	if(iOpHrsHFO>0)
	m_iOpMinHFO=m_iOpMinHFO-(iOpHrsHFO*60);


	rc.left = 360;  
	rc.top = 160;  
	rc.right  = 800;  
	rc.bottom = 400;
	cs.Format(_T("%d %s %d %s"),iOpHrsHFO,getModel()->GetLanguageString(IDS_UNIT_HRS),m_iOpMinHFO,getModel()->GetLanguageString(IDS_UNIT_MIN));
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);




	rc.left = 60;  
	rc.top = 190;  
	rc.right  = 360;  
	rc.bottom = 400;
	//cs = _T("Device-ID:");
	cs = getModel()->GetLanguageString(IDS_TXT_DEVID)+_T(":");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 360;  
	rc.top = 190;  
	rc.right  = 800;  
	rc.bottom = 400;
	//cs = getModel()->getDATAHANDLER()->GetUniqueID();
	DrawText(hdcMem,m_szUniqueID,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 60;  
	rc.top = 220;  
	rc.right  = 360;  
	rc.bottom = 400;
	//cs = _T("Software version:");
	cs = getModel()->GetLanguageString(IDS_TXT_SOFTVERS)+_T(":");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 360;  
	rc.top = 220;  
	rc.right  = 800;  
	rc.bottom = 400;
	//cs = getModel()->GetVersion();
	DrawText(hdcMem,m_szVersion,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************Monitor PIC**********************************//
	rc.left = 60;  
	rc.top = 250;  
	rc.right  = 360;  
	rc.bottom = 400;
	//cs = _T("Version Monitor PIC:");
	cs = getModel()->GetLanguageString(IDS_TXT_MONPICVERS)+_T(":");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 360;  
	rc.top = 250;  
	rc.right  = 800;  
	rc.bottom = 400;
	cs.Format(_T("%d.%d.%d, Checksum: %X"), 
		m_dataMainboard.m_iMonitorVersion_x, 
		m_dataMainboard.m_iMonitorVersion_y, 
		m_dataMainboard.m_iMonitorVersion_z, 
		m_dataMainboard.m_iMonitorChecksum);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	//**************************Steuer PIC**********************************//
	rc.left = 60;  
	rc.top = 280;  
	rc.right  = 360;  
	rc.bottom = 400;
	//cs = _T("Version Conductor PIC:");
	cs = getModel()->GetLanguageString(IDS_TXT_CONPICVERS)+_T(":");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 360;  
	rc.top = 280;  
	rc.right  = 800;  
	rc.bottom = 400;
	cs.Format(_T("%s, Checksum: %X"), 
		m_dataMainboard.m_szConductorVersion, 
		m_dataMainboard.m_iConductorChecksum);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************Blender PIC**********************************//
	rc.left = 60;  
	rc.top = 310;  
	rc.right  = 360;  
	rc.bottom = 400;
	//cs = _T("Version Blender PIC:");
	cs = getModel()->GetLanguageString(IDS_TXT_BLENDERPICVERS)+_T(":");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 360;  
	rc.top = 310;  
	rc.right  = 800;  
	rc.bottom = 400;
	cs.Format(_T("%s"), 
		m_dataMainboard.m_szBlenderVersion);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	//**************************HFO PIC**********************************//
	rc.left = 60;  
	rc.top = 340;  
	rc.right  = 360;  
	rc.bottom = 400;
	//cs = _T("Version HFO PIC:");
	cs = getModel()->GetLanguageString(IDS_TXT_HFOPICVERS)+_T(":");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 360;  
	rc.top = 340;  
	rc.right  = 800;  
	rc.bottom = 400;
	cs.Format(_T("%s"), 
		m_dataMainboard.m_szHfoVersion);
	/*cs.Format(_T("%s, Checksum: %X"), 
		m_dataMainboard.m_szHfoVersion, 
		m_dataMainboard.m_iHfoChecksum);*/
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************Kernel**********************************//
	rc.left = 60;  
	rc.top = 370;  
	rc.right  = 360;  
	rc.bottom = 450;
	//cs = _T("Version Kernel:");
	cs = getModel()->GetLanguageString(IDS_TXT_KERNELVERS)+_T(":");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 360;  
	rc.top = 370;  
	rc.right  = 800;  
	rc.bottom = 450;
	/*if(getModel()->getCONFIG()->GetKernelAcuVersion()==_T("unknown"))
	{
		cs.Format(_T("%s %s"), getModel()->getCONFIG()->GetKernelVersion(),getModel()->getCONFIG()->GetKernelVersionDate());
	}
	else
	{
		cs.Format(_T("%s %s"), getModel()->getCONFIG()->GetKernelVersion(),getModel()->getCONFIG()->GetKernelAcuVersion());
	}*/
	cs.Format(_T("%s %s %s"), getModel()->getCONFIG()->GetKernelVersion(),getModel()->getCONFIG()->GetKernelVersionDate(), getModel()->getCONFIG()->GetKernelAcuVersion());
	
	//cs=getModel()->getCONFIG()->GetKernelVersion();
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	//**************************Language Version**********************************//
	rc.left = 60;  
	rc.top = 400;  
	rc.right  = 360;  
	rc.bottom = 450;
	cs = getModel()->GetLanguageString(IDS_MENU_LANGUAGE)+_T(":");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 360;  
	rc.top = 400;  
	rc.right  = 800;  
	rc.bottom = 450;
	DrawText(hdcMem,m_szLangVersion,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//**************************Mainboard Version**********************************//
	rc.left = 60;  
	rc.top = 430;  
	rc.right  = 360;  
	rc.bottom = 480;
	cs = getModel()->GetLanguageString(IDS_MENU_MAINBOARD)+_T(":");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 360;  
	rc.top = 430;  
	rc.right  = 800;  
	rc.bottom = 480;
	int iMajor=(m_iMainVersion >> 4) & 0x000F;
	int iMinor=m_iMainVersion & 0x000F;
	cs.Format(_T("%d.%d"), iMajor,iMinor);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);



	//dc.BitBlt(0,0,m_lX,m_lY,CDC::FromHandle(hdcMem),0,0,SRCCOPY);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	penLine.DeleteObject();

	
	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	SelectObject(dc.m_hDC,hPrevFontDc);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);

}
// **************************************************************************
// 
// **************************************************************************
void CSubViewSystem::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==SERVICETIMER)
	{
		getModel()->getDATAHANDLER()->getMainboardData(&m_dataMainboard);
		
		m_iOpMinBatt=getModel()->getDATAHANDLER()->getOpTimeBatteryMin(false);
		m_iOpMinDev=getModel()->getDATAHANDLER()->getOpTimeDeviceMin(false);
		m_iOpMinHFO=getModel()->getDATAHANDLER()->getOpTimeHFOMin(false);

		Draw();

	}

	CWnd::OnTimer(nIDEvent);
}
