// ViewStartup.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ViewStartup.h"
#include "LangAdmin.h"
#include "globDefs.h"
#include "TlsRegistry.h"
#include "TlsFile.h"

#define CDPINBIT 4

// **************************************************************************
// 
// **************************************************************************
CViewStartup::CViewStartup(int iViewID):
CMVView(iViewID)
{
	m_bERROR=false;
	m_bWatchdog=false;
	m_bStartupCheck=true;
	m_iCountStartup=0;
	m_bSerialChecksumError=false;
	m_bCom_ERROR=true;
	m_iChecksum=0;
	m_bSPI_ERROR=true;
	m_bSPIChecksumError=false;
	m_bRelais_ERROR=false;
	m_bVoltage_ERROR=false;
	m_bI2C_ERROR=true;
	m_bPIF_ERROR=true;
	m_bDIO_ERROR=true;
	m_byAlarmState=0;

	m_pcLogo=NULL;
	//m_pcBaby=NULL;
	//m_pcWarning=NULL;

	m_dwFactorydefaults=0;
}

// **************************************************************************
// 
// **************************************************************************
CViewStartup::~CViewStartup(void)
{
	if(m_pcLogo)
		delete m_pcLogo;
	m_pcLogo=NULL;

	/*if(m_pcBaby)
		delete m_pcBaby;
	m_pcBaby=NULL;*/

	/*if(m_pcWarning)
		delete m_pcWarning;
	m_pcWarning=NULL;*/
}



BEGIN_MESSAGE_MAP(CViewStartup, CMVView)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// **************************************************************************
// 
// **************************************************************************
bool CViewStartup::CreateView()
{
	RECT rcLd={0,0,800,600};

	if (!Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_STARTUP))
	{
		return false;
	}

	Initialize();

	return true;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CViewStartup::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		CStringW sBMP = _T("\\FFSDISK\\logoFabianHFO.bmp");

		if(CTlsFile::Exists(sBMP))
		{
			m_pcLogo	= new CBmp(m_hDC,sBMP);
		}
		else
		{
			m_pcLogo	= new CBmp(theApp.m_hInstance,m_hDC,IDB_PIC_ACULOGO);
			//m_pcBaby	= new CBmp(theApp.m_hInstance,m_hDC,IDB_PIC_FabianHFO);
		}

		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();//rkuNEWFIX

		return 1;
	}
	else
		return 0;
}
LRESULT CViewStartup::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
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
	}
	return CWnd::WindowProc(message, wParam, lParam);
}
// **************************************************************************
// 
// **************************************************************************
bool CViewStartup::Initialize()
{
	bool result=false;
	m_szSWVers = _T("fabianHFO ");
	m_szSWVers += getModel()->GetMainVersion();

	SetTimer(STARTUPTIMER,100,NULL);

	return result;
}

// **************************************************************************
// 
// **************************************************************************
void CViewStartup::Open()
{
}

// **************************************************************************
// 
// **************************************************************************
void CViewStartup::Close()
{
}

// **************************************************************************
// 
// **************************************************************************
void CViewStartup::Show()
{
	this->ShowWindow(SW_SHOW);
}

// **************************************************************************
// 
// **************************************************************************
void CViewStartup::Hide()
{
	this->ShowWindow(SW_HIDE);
}

// **************************************************************************
// 
// **************************************************************************
void CViewStartup::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}




//************************************
// Method:    OnDestroy
// FullName:  CViewStartup::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CViewStartup::OnDestroy()
{
	KillTimer(STARTUPTIMER);

	CMVView::OnDestroy();
}

// **************************************************************************
// 
// **************************************************************************
void CViewStartup::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==STARTUPTIMER)
	{
		KillTimer(STARTUPTIMER);
				
		Draw();

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_STARTUP_SUCCESS);

	}

	CMVView::OnTimer(nIDEvent);
}

// **************************************************************************
// 
// **************************************************************************
void CViewStartup::Draw()
{

	RECT rc={0,0,m_lX,m_lY};

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	int bc=SetBkColor(hdcMem,BACKGND);
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);

	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf17AcuBold);

	int tc=SetTextColor(hdcMem,0x0000000);

	CBrush cbrBack(BACKGND);
	HBRUSH hPrevBrush=(HBRUSH)SelectObject(hdcMem,(HBRUSH)cbrBack);
	HPEN hPrevPen=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));
	
	Rectangle(hdcMem, 0, 0, m_lX, m_lY);
	
	CBrush cbrEll(RGB(0,0,255));
	CPen penEll(PS_SOLID,1,RGB(0,0,255));
	SelectObject(hdcMem,(HBRUSH)cbrEll);
	SelectObject(hdcMem, (HPEN)penEll);

	int iTop=430;
	int iBottom=450;

	if(m_pcLogo)
		m_pcLogo->Draw(hdcMem,430,55);
	/*if(m_pcBaby)
		m_pcBaby->Draw(hdcMem,390,100);*/

	rc.top = 250;
	rc.bottom = m_lY;
	rc.left = 10; 
	rc.right = 380;

	SetTextColor(hdcMem,0x0000000);
	SelectObject(hdcMem,g_hf8AcuBold);

	int iCnt=0;
	int iLeft=50+(iCnt*20)+(iCnt*5);
	int iRight=(50+(iCnt*20))+20+(iCnt*5);

	Ellipse(hdcMem,iLeft,iTop,iRight,iBottom);

	iCnt++;

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	CStringW sz = getModel()->GetLanguageString(IDS_TXT_SOFTVERS)+_T(": ");
	sz+=m_szSWVers;

	rc.top = 400;
	rc.bottom = m_lY;
	rc.left = 450; 
	rc.right = m_lX;

	DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	SelectObject(hdcMem,g_hf17AcuBold);

	rc.top = 50;
	rc.bottom = m_lY;
	rc.left = 50; 
	rc.right = m_lX;
	CStringW szDev =getModel()->GetLanguageString(IDS_TXT_DEVID);

	szDev+=getModel()->GetUniqueID();
	DrawText(hdcMem,szDev,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	rc.top = 400;
	rc.bottom = m_lY;
	rc.left = 50; 
	rc.right = m_lX;

	DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_SELFTEST),-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

	//Sleep(500);
	Sleep(100);
	
	iLeft=50+(iCnt*20)+(iCnt*5);
	iRight=(50+(iCnt*20))+20+(iCnt*5);

	Ellipse(hdcMem,iLeft,iTop,iRight,iBottom);

	iCnt++;

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	if(m_bWatchdog)
	{
		SetTextColor(hdcMem,RGB(255,0,0));

		rc.top = 480;
		rc.bottom = m_lY;
		rc.left = 50; 
		rc.right = m_lX;

		DrawText(hdcMem,_T("!!! FATAL SYSTEM ERROR !!!"),-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

		rc.top = 520;
		rc.bottom = m_lY;
		rc.left = 50; 
		rc.right = m_lX;

		DrawText(hdcMem,_T("- please restart the system -"),-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);

		theApp.getLog()->WriteLine(_T("!!! FATAL SYSTEM ERROR !!!"));
	}

	SetTextColor(hdcMem,RGB(0,0,0));
	SelectObject(hdcMem,g_hf8AcuBold);
	
	rc.top = 435;
	rc.bottom = m_lY;
	rc.left = 450; 
	rc.right = m_lX;

	if(getModel()->getSERIAL()->IsAvailable())
	{
		if(getModel()->getSERIAL()->GetM_StatusByte())
		{
			m_bCom_ERROR=false;
		}
		else
		{
			m_bERROR=true;
			m_bCom_ERROR=true;
			DEBUGMSG(TRUE, (TEXT("STARTUP COM ERROR\r\n")));
			theApp.getLog()->WriteLine(_T("#HFO:0268"));
			getModel()->getDATAHANDLER()->setCOMErrorCode(ERRC_COM_READ_MSTATUS); //newVG
		}
	}
	else
	{
		m_bERROR=true;
		m_bCom_ERROR=true;
	}


	if(getModel()->getI2C()->IsI2C_FRAMavailability())
	{
		m_bI2C_ERROR=false;
	}
	else
	{
		m_bI2C_ERROR=true;
	}

	if(getModel()->getI2C()->IsI2C_MCPavailability() && m_bI2C_ERROR==false)
	{
		m_bI2C_ERROR=false;
	}
	else
	{
		m_bI2C_ERROR=true;
	}

	BYTE data[1];
	DWORD dwBytesToWrite=1;
	data[0] = 0;
	int iBytesWritten=getModel()->getPIF()->SendData(data, dwBytesToWrite);
	if(iBytesWritten==dwBytesToWrite)
	{
		m_bPIF_ERROR=false;
	}
	else
	{
		m_bPIF_ERROR=true;
		theApp.getLog()->WriteLine(_T("#HFO:0269"));
		getModel()->SetPIFavailability(FALSE);
	}


	DWORD dwResult=getModel()->getDIO()->ClearPin (CDPINBIT);
	
	if(dwResult==0)
	{
		m_bDIO_ERROR=true;
		getModel()->SetDIOavailability(FALSE);
		theApp.getLog()->WriteLine(_T("#HFO:0270"));
	}
	else
	{
		m_bDIO_ERROR=false;
		getModel()->SetDIOavailability(TRUE);
	}

	
	//Sleep(500);
	Sleep(100);

	m_byAlarmState=getModel()->getSPI()->Read_STATUS();
	if(m_byAlarmState<0)
	{
		theApp.getLog()->WriteLine(_T("#HFO:0271"));
		Sleep(500);
		m_byAlarmState=getModel()->getSPI()->Read_STATUS();
	}

	if(m_byAlarmState<0)
	{
		m_bERROR=true;
		m_bSPI_ERROR=true;

#ifndef SIMULATION_NOSPI
		theApp.getLog()->WriteLine(_T("#HFO:0272"));
		getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_READSTATUS, true);
#endif
		
	}
	else
	{
		m_bSPI_ERROR=false;

		SHORT byTemp = m_byAlarmState;

		if(byTemp & BIT5)
		{
			getModel()->getDATAHANDLER()->SetAccuSupply(true);
		}
		else
		{
			getModel()->getDATAHANDLER()->SetAccuSupply(false);
		}
	}

	

	if(m_byAlarmState & BIT10 && !m_bSPI_ERROR)
	{
		m_bERROR=true;
		m_bVoltage_ERROR=true;
		getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_VOLTAGE, true);
	}
	if(m_byAlarmState & BIT11 && !m_bSPI_ERROR)
	{
		m_bERROR=true;
		m_bRelais_ERROR=true;
		getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_RELAIS, true);
	}
	if(m_byAlarmState & BIT12 && !m_bSPI_ERROR)
	{
		m_bERROR=true;
		m_bSPIChecksumError=true;
		getModel()->getDATAHANDLER()->setSPIErrorCode(ERRC_SPI_CHECKSUM, true);
	}

	if(m_bCom_ERROR)
	{
		//sz=_T("COM-ERROR");
		sz=getModel()->GetLanguageString(IDS_COM_ERROR);
		SetTextColor(hdcMem,RGB(255,0,0));
		DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}
	else
	{
		sz=_T("COM ok");
		//sz=getModel()->GetLanguageString(IDS_COM_ERROR);
		SetTextColor(hdcMem,RGB(0,0,0));
		DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}

	//Sleep(500);
	Sleep(100);

	iLeft=50+(iCnt*20)+(iCnt*5);
	iRight=(50+(iCnt*20))+20+(iCnt*5);

	Ellipse(hdcMem,iLeft,iTop,iRight,iBottom);

	iCnt++;

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);
	
	//Sleep(500);
	Sleep(100);

	iLeft=50+(iCnt*20)+(iCnt*5);
	iRight=(50+(iCnt*20))+20+(iCnt*5);

	Ellipse(hdcMem,iLeft,iTop,iRight,iBottom);

	iCnt++;

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);
	
	rc.top = 450;
	rc.bottom = m_lY;
	rc.left = 450; 
	rc.right = m_lX;


	/*if(!m_bI2C_ERROR && !m_bPIF_ERROR && !m_bDIO_ERROR && !m_bSPI_ERROR)
	{
	sz=_T("Interfaces ok");
	SetTextColor(hdcMem,RGB(0,0,0));
	DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
	}
	else*/
	{
		if(m_bI2C_ERROR)
		{
			sz=_T("I2C-ERROR");
			SetTextColor(hdcMem,RGB(255,0,0));
			DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else
		{
			sz=_T("I2C ok");
			SetTextColor(hdcMem,RGB(0,0,0));
			DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}

		//Sleep(500);
		Sleep(100);

		iLeft=50+(iCnt*20)+(iCnt*5);
		iRight=(50+(iCnt*20))+20+(iCnt*5);

		Ellipse(hdcMem,iLeft,iTop,iRight,iBottom);

		iCnt++;

		BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);
		
		rc.top = 465;
		rc.bottom = m_lY;
		rc.left = 450; 
		rc.right = m_lX;

		if(m_bPIF_ERROR)
		{
			sz=_T("PIF-ERROR");
			SetTextColor(hdcMem,RGB(255,0,0));
			DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else
		{
			sz=_T("PIF ok");
			SetTextColor(hdcMem,RGB(0,0,0));
			DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}

		Sleep(500);
		//Sleep(100);

		iLeft=50+(iCnt*20)+(iCnt*5);
		iRight=(50+(iCnt*20))+20+(iCnt*5);

		Ellipse(hdcMem,iLeft,iTop,iRight,iBottom);

		iCnt++;

		BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

		rc.top = 480;
		rc.bottom = m_lY;
		rc.left = 450; 
		rc.right = m_lX;

		if(m_bDIO_ERROR)
		{
			sz=_T("DIO-ERROR");
			SetTextColor(hdcMem,RGB(255,0,0));
			DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else
		{
			sz=_T("DIO ok");
			SetTextColor(hdcMem,RGB(0,0,0));
			DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}

		Sleep(500);
		//Sleep(100);

		iLeft=50+(iCnt*20)+(iCnt*5);
		iRight=(50+(iCnt*20))+20+(iCnt*5);

		Ellipse(hdcMem,iLeft,iTop,iRight,iBottom);
		
		getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);

		iCnt++;

		BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);
		
		rc.top = 495;
		rc.bottom = m_lY;
		rc.left = 450; 
		rc.right = m_lX;

		if(m_bSPI_ERROR)
		{
			sz=_T("SPI-ERROR");
			SetTextColor(hdcMem,RGB(255,0,0));
			DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else
		{
			sz=_T("SPI ok");
			SetTextColor(hdcMem,RGB(0,0,0));
			DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}

	}

	if(!m_bSPI_ERROR)
	{
		rc.top = 510;
		rc.bottom = m_lY;
		rc.left = 450; 
		rc.right = m_lX;

		/*if(!m_bVoltage_ERROR && !m_bRelais_ERROR && !m_bSPIChecksumError)
		{
		sz=_T("SPI state ok");
		SetTextColor(hdcMem,RGB(0,0,0));
		DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		else*/
		{
			if(m_bVoltage_ERROR)
			{
				sz=_T("Voltage-ERROR");
				SetTextColor(hdcMem,RGB(255,0,0));
				DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
			}
			else
			{
				sz=_T("Voltage ok");
				SetTextColor(hdcMem,RGB(0,0,0));
				DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
			}

			rc.top = 525;
			rc.bottom = m_lY;
			rc.left = 450; 
			rc.right = m_lX;

			if(m_bRelais_ERROR)
			{
				sz=_T("Relais-ERROR");
				SetTextColor(hdcMem,RGB(255,0,0));
				DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
			}
			else
			{
				sz=_T("Relais ok");
				SetTextColor(hdcMem,RGB(0,0,0));
				DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
			}

			rc.top = 540;
			rc.bottom = m_lY;
			rc.left = 450; 
			rc.right = m_lX;

			if(m_bSPIChecksumError)
			{
				sz=_T("SPIChks-ERROR");
				SetTextColor(hdcMem,RGB(255,0,0));
				DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
			}
			else
			{
				sz=_T("SPIChks ok");
				SetTextColor(hdcMem,RGB(0,0,0));
				DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
			}
		}
	}

	Sleep(300);
	//Sleep(100);

	iLeft=50+(iCnt*20)+(iCnt*5);
	iRight=(50+(iCnt*20))+20+(iCnt*5);

	Ellipse(hdcMem,iLeft,iTop,iRight,iBottom);
	
	getModel()->getSOUND()->SetPIFSound(PIF_SIGNAL);

	iCnt++;

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	Sleep(500);

	SelectObject(hdcMem,hPrevFont);
	SelectObject(hdcMem, hPrevBrush);	
	SelectObject(hdcMem, hPrevPen);

	SetTextColor(hdcMem,tc);
	SetBkColor(hdcMem,bc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hBmpMemPrev);
	DeleteObject(hBmpMem);

	//DeleteObject(cbrBack);
	//DeleteObject(cbrEll);
	//penEll.DeleteObject();//rkuNEWFIX

	DeleteDC(hdcMem);
}


