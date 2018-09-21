// MVView.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "MVView.h"

/**********************************************************************************************//**
 * CMVView
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	ViewID	Identifier for the view.
 **************************************************************************************************/

CMVView::CMVView(int ViewID):
m_iViewID(ViewID)
{
	m_pModel = NULL;
	
	m_kUp					= VK_UP;
	m_kDown					= VK_DOWN;
	m_kSpace				= VK_SPACE;

	getModel()->AttachObserver(this);
	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;
	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CMVView class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CMVView::~CMVView()
{
	//getModel()->DetachObserver(this);

	
	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);
	//m_pModel = NULL;


}

BEGIN_MESSAGE_MAP(CMVView, CWnd)
	ON_WM_PAINT()
	//ON_WM_ERASEBKGND()
	//ON_WM_DESTROY()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

//BOOL CMVView::PreCreateWindow(CREATESTRUCT& cs) 
//{
//	if (!CWnd::PreCreateWindow(cs))
//		return FALSE;
//
//	//m_brsBkGrnd.CreateSolidBrush(RGB(220,220,255));
//
//	cs.style &= ~WS_BORDER;
//	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
//		/*::LoadCursor(NULL, IDC_ARROW)*/NULL, reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);
//
//	return TRUE;
//}

/**********************************************************************************************//**
 * Creates the Window instance that will be represented by this instance
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pParentWnd	If non-null, the parent window.
 * \param 		  	rc		  	The rectangle.
 * \param 		  	nID		  	The identifier.
 * \param [in,out]	pContext  	If non-null, the context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CMVView::Create(CWnd* pParentWnd, const RECT &rc, UINT nID, CCreateContext*) 
{
	//csView.Lock();

	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD/*|WS_VISIBLE*/,rc,pParentWnd,nID))
	{
		CClientDC dc(this);
		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
		m_hBmpPrev=(HBITMAP)SelectObject(m_hDC,m_hBmp);

		CBrush cbrBack(RGB(45,45,45));
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		
		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();

		return 1;
	}
	else
	{
		//csView.Unlock();
		return 0;
	}
}

/**********************************************************************************************//**
 * Notifies the data changed
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVView::NotifyDataChanged()
{

}

/**********************************************************************************************//**
 * Opens this instance
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVView::Open()
{

}

/**********************************************************************************************//**
 * Closes this instance
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVView::Close()
{

}

/**********************************************************************************************//**
 * Shows this instance
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVView::Show()
{

}

/**********************************************************************************************//**
 * Shows
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	bRedraw	True to redraw.
 **************************************************************************************************/

void CMVView::Show(bool)
{

}

/**********************************************************************************************//**
 * Hides this instance
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVView::Hide()
{
}

/**********************************************************************************************//**
 * Sets view focus
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVView::SetViewFocus()
{
	SetFocus();
}

/**********************************************************************************************//**
 * Sets next focus
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVView::SetNextFocus()
{
	return false;
}

/**********************************************************************************************//**
 * Sets previous focus
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVView::SetPrevFocus()
{
	return false;
}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);

	// TODO: Add your message handler code here

	// Do not call CWnd::OnPaint() for painting messages
}

/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CMVView::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * CMVView message handlers
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVView::CreateView()
{
	return true;
}

/**********************************************************************************************//**
 * Initializes this instance
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CMVView::Initialize()
{
	return true;
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

void CMVView::OnDestroy()
{
	getModel()->DetachObserver(this);


	__super::OnDestroy();
}

/**********************************************************************************************//**
 * Window proc
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	message	The message.
 * \param	wParam 	The wParam field of the message.
 * \param	lParam 	The lParam field of the message.
 *
 * \return	A LRESULT.
 **************************************************************************************************/

LRESULT CMVView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
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
	case WM_SETNEXTFOCUS:
		{
			//rku cs1
			SetNextFocus();
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
	}
	return CWnd::WindowProc(message, wParam, lParam);
}