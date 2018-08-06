// ViewShutdown.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ViewShutdown.h"
#include "TlsFile.h"

/**********************************************************************************************//**
 * CViewShutdown
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	iViewID	Identifier for the view.
 **************************************************************************************************/

CViewShutdown::CViewShutdown(int iViewID):
CMVView(iViewID)
{
	m_pcLogo=NULL;
	//m_pcBaby=NULL;
	m_pcShutdown=NULL;
	m_pcAccu=NULL;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CViewShutdown class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CViewShutdown::~CViewShutdown()
{
	delete m_pcLogo;
	m_pcLogo=NULL;

	//delete m_pcBaby;
	//m_pcBaby=NULL;


	delete m_pcAccu;
	m_pcAccu=NULL;

	delete m_pcShutdown;
	m_pcShutdown=NULL;
}


BEGIN_MESSAGE_MAP(CViewShutdown, CMVView)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CViewShutdown message handlers
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewShutdown::CreateView()
{
	RECT rcLd={0,0,800,600};

	if (!Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_SHUTDOWN))
	{
		return false;
	}

	//Initialize();

	Draw();

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

BOOL CViewShutdown::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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

		/*m_pcBaby	= new CBmp(theApp.m_hInstance,m_hDC,IDB_PIC_FabianHFO);
		m_pcLogo	= new CBmp(theApp.m_hInstance,m_hDC,IDB_PIC_ACULOGO);*/
		
		CStringW sBMP = _T("\\FFSDISK\\logoFabianHFO.bmp");

		if(CTlsFile::Exists(sBMP))
		{
			m_pcLogo	= new CBmp(dc.m_hDC,sBMP);
		}
		else
		{
			m_pcLogo	= new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_PIC_ACULOGO);
			//m_pcBaby	= new CBmp(theApp.m_hInstance,dc.m_hDC,IDB_PIC_FabianHFO);
		}

		m_pcShutdown	= new CBmp(theApp.m_hInstance,m_hDC,IDB_PIC_SHUTDOWN);
		m_pcAccu	= new CBmp(theApp.m_hInstance,m_hDC,IDB_BATTERY_LOAD);

		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();//rkuNEWFIX

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
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewShutdown::Initialize()
{
	bool result=false;

	SetTimer(STARTUPTIMER,100,NULL);

	return result;
}

/**********************************************************************************************//**
 * Window proc
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	message	The message.
 * \param	wParam 	The wParam field of the message.
 * \param	lParam 	The lParam field of the message.
 *
 * \return	A LRESULT.
 **************************************************************************************************/

LRESULT CViewShutdown::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
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

/**********************************************************************************************//**
 * Opens this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewShutdown::Open()
{
}

/**********************************************************************************************//**
 * Closes this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewShutdown::Close()
{
}

/**********************************************************************************************//**
 * Shows this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewShutdown::Show()
{
	this->ShowWindow(SW_SHOW);
}

/**********************************************************************************************//**
 * Hides this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewShutdown::Hide()
{
	this->ShowWindow(SW_HIDE);
}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewShutdown::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewShutdown::OnDestroy()
{
	KillTimer(STARTUPTIMER);

	CMVView::OnDestroy();
}

/**********************************************************************************************//**
 * Draws this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewShutdown::Draw()
{

	RECT rc={0,0,m_lX,m_lY};
	//RECT rcCl;
	//GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	int bc=SetBkColor(hdcMem,BACKGND);
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);

	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf21AcuBold);

	int tc=SetTextColor(hdcMem,0x0000000);

	CBrush cbrBack(BACKGND);
	HBRUSH hPrevBrush=(HBRUSH)SelectObject(hdcMem,(HBRUSH)cbrBack);
	HPEN hPrevPen=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));
	//HPEN hPrevPen=(HPEN)SelectObject(hdcMem, (HPEN)penEll);
	
	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	
	if(m_pcLogo)
		m_pcLogo->Draw(hdcMem,430,55);
	//if(m_pcBaby)
	//	m_pcBaby->Draw(hdcMem,390,100);
	

	

	

	if(getModel()->IsAccuTurnoff() && !getModel()->IsInstaller())
	{
		m_pcShutdown->Draw(hdcMem,95,380);

		SetTextColor(hdcMem,RGB(255,0,0));
		rc.top = 450;
		rc.bottom = m_lY;
		rc.left = 300; 
		rc.right = 800;
		m_pcAccu->Draw(hdcMem,45,30);
		DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_FLATBATT),-1,&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);
		SetTextColor(hdcMem,0x0000000);
	}

	rc.top = 500;
	rc.bottom = m_lY;
	rc.left = 300; 
	rc.right = 800;
	DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_SHUTDOWN),-1,&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);
	//dc.BitBlt(0,0,m_lX,m_lY,CDC::FromHandle(hdcMem),0,0,SRCCOPY);


	SelectObject(hdcMem, hPrevBrush);	
	SelectObject(hdcMem, hPrevPen);
	SelectObject(hdcMem,hPrevFont);
	SetTextColor(hdcMem,tc);
	SetBkColor(hdcMem,bc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hBmpMemPrev);
	DeleteObject(hBmpMem);

	DeleteDC(hdcMem);
}
