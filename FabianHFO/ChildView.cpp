// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ChildView.h"
#include <exception>

#ifdef _DEBUG

/**********************************************************************************************//**
 * A macro that defines new
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

#define new DEBUG_NEW
#endif

/**********************************************************************************************//**
 * CChildView
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

CChildView::CChildView()
{
}

/**********************************************************************************************//**
 * Finalizes an instance of the CChildView class
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

CChildView::~CChildView()
{
	m_brsBkGrnd.DeleteObject();
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CChildView message handlers
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param [in,out]	cs	The create struct.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	m_brsBkGrnd.CreateSolidBrush(RGB(45,45,45));
	//m_brsBkGrnd.CreateSolidBrush(RGB(220,220,255));

	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
}

/**********************************************************************************************//**
 * Executes the erase bkgnd action
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param [in,out]	pDC	If non-null, the device-context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein, und/oder benutzen Sie den Standard.

	CRect rc;
	GetClientRect(rc);
	pDC->FillRect(&rc, &m_brsBkGrnd);

	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}
