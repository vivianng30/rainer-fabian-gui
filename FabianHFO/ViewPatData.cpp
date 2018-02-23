// ViewPatData.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ViewPatData.h"
#include "LangAdmin.h"
#include "globDefs.h"
#include "TlsRegistry.h"

/**********************************************************************************************//**
 * Initializes a new instance of the CViewPatData class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	iViewID	Identifier for the view.
 **************************************************************************************************/

CViewPatData::CViewPatData(int iViewID):
CMVView(iViewID)
{
	m_pcWndPatient=NULL;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CViewPatData class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CViewPatData::~CViewPatData(void)
{
}



BEGIN_MESSAGE_MAP(CViewPatData, CMVView)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * Creates the view
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewPatData::CreateView()
{
	//RECT rcLd={5,50,790,535};
	RECT rcLd={5,50,795,595};

	if (!Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_PATDATA))
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

BOOL CViewPatData::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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
		//CBrush cbrBack(RGB(200,200,255));
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

LRESULT CViewPatData::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
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
 * Initializes this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewPatData::Initialize()
{
	bool result=false;

	return result;
}

/**********************************************************************************************//**
 * Opens this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewPatData::Open()
{
}

/**********************************************************************************************//**
 * Closes this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewPatData::Close()
{
}

/**********************************************************************************************//**
 * Shows this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewPatData::Show()
{
	//bool result=false;

	
	CreateWndPatient();
	ShowWndPatient(true);
	

	this->ShowWindow(SW_SHOW);
}

/**********************************************************************************************//**
 * Hides this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewPatData::Hide()
{
	this->ShowWindow(SW_HIDE);
}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewPatData::OnPaint()
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

void CViewPatData::OnDestroy()
{
	CMVView::OnDestroy();
}

/**********************************************************************************************//**
 * Creates window patient
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewPatData::CreateWndPatient()
{
	if(m_pcWndPatient==NULL)
	{


		m_pcWndPatient = new CWndSetupPatient();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,790,545};
		//RECT rcLd={0,0,565,485};
		if(m_pcWndPatient->CreateWnd(this,rcLd,IDC_VIEW_SETUP_PATIENT))
		{


			return true;
		}


	}
	return false;
}

/**********************************************************************************************//**
 * Destroys the window patient
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewPatData::DestroyWndPatient()
{
	if(m_pcWndPatient)
	{
		m_pcWndPatient->DestroyWindow();
		delete m_pcWndPatient;
		m_pcWndPatient=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window patient
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	show	True to show, false to hide.
 **************************************************************************************************/

void CViewPatData::ShowWndPatient(bool show)
{
	m_pcWndPatient->Show(show);

	m_pcWndPatient->SetFocus();
}

