// WndServiceNetwork.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndServiceNetwork.h"
#include "TlsFile.h"
#include "TlsRegistry.h"

// CWndServiceNetwork

IMPLEMENT_DYNAMIC(CWndServiceNetwork, CWnd)

CWndServiceNetwork::CWndServiceNetwork():
CWndService()
{
	if(getModel()->getAcuLink()!=NULL)
		m_bAcuLink=true;
	else
		m_bAcuLink=false;

	m_bAcuLinkLicense=false;

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
}

CWndServiceNetwork::~CWndServiceNetwork()
{
}


BEGIN_MESSAGE_MAP(CWndServiceNetwork, CWnd)
	ON_WM_DESTROY()
	/*ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)
	ON_BN_CLICKED(IDC_BTN_SERVICE_TESTNURSECALL, &CWndServiceSettings::OnBnClickedTestNursecall)*/
END_MESSAGE_MAP()



// CWndServiceNetwork message handlers

// CWndServiceNursecall message handlers
void CWndServiceNetwork::Init()
{
	CClientDC dc(this);

	//m_pcTest_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN160_LG_UP);
	//m_pcTest_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN160_LG_DW);
	//m_pcTest_Dis= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN160_LG_DIS);

	//m_pcNurse		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_PIC_NURSE32);

	//BTN btn;

	//btn.wID					= IDC_BTN_SERVICE_TESTNURSECALL;	
	//btn.poPosition.x		= 200;//;586
	//btn.poPosition.y		= 110;
	//btn.pcBmpUp				= m_pcTest_Up;
	//btn.pcBmpDown			= m_pcTest_Dw;
	//btn.pcBmpFocus			= m_pcTest_Up;
	//btn.pcBmpDisabled		= m_pcTest_Up;
	//btn.dwFormat			= DT_VCENTER|DT_CENTER;
	//m_pcTest=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	//m_pcTest->Create(this,g_hf15AcuMed,0);
	//m_pcTest->SetText(_T("activate alarm"));
	//m_pcTest->ShowWindow(SW_SHOW);



	//m_bNurscallActive=getModel()->getCONFIG()->IsNursecallActive();

	//m_sliderEnable = new CBitmapSlider();
	//m_sliderEnable->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|BS_OWNERDRAW, CRect(200,55,349,100), 
	//	this,IDC_SLD_NURSECALL);
	//m_sliderEnable->SetBitmapChannel( IDB_SLD_CHAN_GREY, NULL );
	//m_sliderEnable->SetBitmapThumb( IDB_SLD_THUMB_GREY, IDB_SLD_THUMB_ACT_GREY);
	//m_sliderEnable->SetRange( 0, 1 );
	//if(m_bNurscallActive)
	//{
	//	m_sliderEnable->SetPos( 1 );
	//	m_pcTest->EnableWindow(TRUE);
	//}
	//else
	//{
	//	m_sliderEnable->SetPos( 0 );
	//	m_pcTest->EnableWindow(FALSE);
	//}
	//m_sliderEnable->SetMargin( 5, 0, 6, 0 );
	//m_sliderEnable->DrawFocusRect( FALSE );
	 
	CStringW szLicenseFile=_T("ML");
	szLicenseFile+=getModel()->GetUniqueID();
	szLicenseFile+=_T(".mlic");

	CStringW szFile=_T("\\FFSDISK\\")+szLicenseFile;

	if(CTlsFile::Exists(szFile))
	{
		m_bAcuLinkLicense=true;
	}
	

	m_pcMenuBack->ShowWindow(SW_SHOW);
}

void CWndServiceNetwork::Draw()
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
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf10AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	//CDC* pDCMem=CDC::FromHandle(hdcMem);
	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	CBrush cbrDarkRound(RGB(140,140,140));

	/**********************acuLink******************************/
	SelectObject(hdcMem,cbrRound);
	RoundRect(hdcMem, 20, 20, 750, 310,20,20);

	SelectObject(hdcMem,cbrDarkRound);

	RoundRect(hdcMem, 20, 20, 750, 50,20,20);

	SetTextColor(hdcMem,RGB(255,255,255));

	rc.left = 40;  
	rc.top = 25;  
	rc.right  = 420;  
	rc.bottom = 500;
	CStringW cs = _T("PDMS:");
	//cs = getModel()->GetLanguageString(IDS_MENU_TREND);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	SetTextColor(hdcMem,0x0000000);

	rc.left = 60;  
	rc.top = 65;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("PDMS:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 180;  
	rc.top = 65;  
	rc.right  = 400;  
	rc.bottom = 500;

	if(m_bAcuLink)
		cs = _T("available");
	else
		cs = _T("not available");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 350;  
	rc.top = 65;  
	rc.right  = 500;  
	rc.bottom = 500;

	cs = _T("Version EXE: ");
	cs+=getModel()->getAcuLink()->getExeVersion();
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 540;  
	rc.top = 65;  
	rc.right  = 750;  
	rc.bottom = 500;

	cs = _T("Version DLL: ");
	cs+=getModel()->getAcuLink()->getDllVersion();
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);




	rc.left = 60;  
	rc.top = 100;  
	rc.right  = 450;  
	rc.bottom = 500;

	cs = _T("Protocol:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 180;  
	rc.top = 100;  
	rc.right  = 700;  
	rc.bottom = 500;

	if(m_bAcuLink)
	{
		if(m_iPDMSProtocol==ACL_SERIAL_ASCII)
			cs = _T("Serial (no wave data)");
		else if(m_iPDMSProtocol==ACL_SERIAL_WAVE)
			cs = _T("Serial (with wave data)");
		else if(m_iPDMSProtocol==ACL_ETHERNET_WAVE)
			cs = _T("Ethernet (with wave data)");
		else if(m_iPDMSProtocol==ACL_ETHERNET)
			cs = _T("Ethernet (no wave data)");
		else if(m_iPDMSProtocol==ACL_SERIAL_IVOI)
			cs = _T("VueLink/IntelliBridge");
		else
			cs = _T("Terminal");
	}
	else
	{
		if(m_iPDMSProtocol==ACL_TERMINAL)
			cs = _T("Terminal");
		else
			cs = _T("--");
	}
	
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	/*rc.left = 350;  
	rc.top = 100;  
	rc.right  = 500;  
	rc.bottom = 500;
	cs = _T("License: ");
	if(m_bAcuLinkLicense)
	{
		cs+=_T("ok");
	}
	else
	{
		cs+=_T("fail");
	}
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);*/




	rc.left = 60;  
	rc.top = 135;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Interface:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 180;  
	rc.top = 135;  
	rc.right  = 400;  
	rc.bottom = 500;

	if(m_bAcuLink)
	{
		if(m_iPDMSProtocol==ACL_SERIAL_ASCII)
			cs = _T("RS-232");
		else if(m_iPDMSProtocol==ACL_SERIAL_WAVE)
			cs = _T("RS-232");
		else if(m_iPDMSProtocol==ACL_ETHERNET_WAVE)
			cs = _T("RJ-45");
		else if(m_iPDMSProtocol==ACL_ETHERNET)
			cs = _T("RJ-45");
		else
			cs = _T("RS-232");
	}
	else
	{
		if(m_iPDMSProtocol==ACL_TERMINAL)
			cs = _T("Terminal");
		else
			cs = _T("--");
	}
	
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 60;  
	rc.top = 170;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("IP address:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 170;  
	rc.top = 170;  
	rc.right  = 800;  
	rc.bottom = 500;
	//cs = _T("---");
	DrawText(hdcMem,m_szIP,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 290;  
	rc.top = 170;  
	rc.right  = 800;  
	rc.bottom = 500;
	cs = _T("Default Gateway:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 420;  
	rc.top = 170;  
	rc.right  = 800;  
	rc.bottom = 500;
	//cs = _T("---");
	DrawText(hdcMem,m_szGateway,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 530;  
	rc.top = 170;  
	rc.right  = 800;  
	rc.bottom = 500;
	cs = _T("Subnetmask:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 635;  
	rc.top = 170;  
	rc.right  = 800;  
	rc.bottom = 500;
	//cs = _T("---");
	DrawText(hdcMem,m_szSubnet,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 60;  
	rc.top = 205;  
	rc.right  = 800;  
	rc.bottom = 500;
	if(m_bDHCP)
	{
		DrawText(hdcMem,_T("DHCP enabled"),-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}
	else
	{
		DrawText(hdcMem,_T("DHCP disabled"),-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);
	}


	rc.left = 60;  
	rc.top = 240;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("License:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 180;  
	rc.top = 240;  
	rc.right  = 400;  
	rc.bottom = 500;

	if(m_bAcuLinkLicense)
		cs = _T("available");
	else
		cs = _T("not available");
	
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	if(false==CTlsFile::Exists(_T("\\FFSDISK\\AcuLink.exe")))
	{
		cs = _T("Installation: #001");
	}
	else if(false==CTlsFile::Exists(_T("\\FFSDISK\\AcuLink_DLL.dll")))
	{
		cs = _T("Installation: #002");
	}
	else if(getModel()->getCONFIG()->GetPDMSprotocol()==ACL_NOPDMS)
	{
		cs = _T("Installation: #003");
	}
	else if(getModel()->getCONFIG()->GetPDMSprotocol()==ACL_TERMINAL)
	{
		cs = _T("Installation: #004");
	}
	else
	{
		cs = _T("Installation: ok");
	}

	rc.left = 350;  
	rc.top = 240;  
	rc.right  = 600;  
	rc.bottom = 500;
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	

	rc.left = 60;  
	rc.top = 275;  
	rc.right  = 280;  
	rc.bottom = 500;
	cs = _T("Port:");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 180;  
	rc.top = 275;  
	rc.right  = 400;  
	rc.bottom = 500;

	cs.Format(_T("%d"), m_uiPort);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	/*rc.left = 180;  
	rc.top = 170;  
	rc.right  = 400;  
	rc.bottom = 500;

	if(m_bAcuLink)
		cs = _T("IP address");
	else
		cs = _T("DHCP");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);*/

	//dc.BitBlt(0,0,m_lX,m_lY,CDC::FromHandle(hdcMem),0,0,SRCCOPY);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	penLine.DeleteObject();

	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}

//************************************
// Method:    OnDestroy
// FullName:  CWndServiceNetwork::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndServiceNetwork::OnDestroy()
{
	CWndService::OnDestroy();
}
