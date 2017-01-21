// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ChildView.h"
#include <exception>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
	m_brsBkGrnd.DeleteObject();
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CChildView message handlers

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

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
}


BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein, und/oder benutzen Sie den Standard.

	CRect rc;
	GetClientRect(rc);
	pDC->FillRect(&rc, &m_brsBkGrnd);

	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}
