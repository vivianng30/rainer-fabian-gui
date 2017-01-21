// MVView.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "MVView.h"




// CMVView


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

// **************************************************************************
// 
// **************************************************************************
BOOL CMVView::Create(CWnd* pParentWnd, const RECT &rc, UINT nID, CCreateContext* pContext) 
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

void CMVView::NotifyDataChanged()
{

}
void CMVView::Open()
{

}
void CMVView::Close()
{

}
void CMVView::Show()
{

}
void CMVView::Show(bool bRedraw)
{

}
void CMVView::Hide()
{
}
void CMVView::SetViewFocus()
{
	SetFocus();
}
bool CMVView::SetNextFocus()
{
	return false;
}
bool CMVView::SetPrevFocus()
{
	return false;
}

void CMVView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);

	// TODO: Add your message handler code here

	// Do not call CWnd::OnPaint() for painting messages
}


// **************************************************************************
// 
// **************************************************************************
CMVModel *CMVView::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


// CMVView message handlers

bool CMVView::CreateView()
{
	return true;
}
bool CMVView::Initialize()
{
	return true;
}

//************************************
// Method:    OnDestroy
// FullName:  CMVView::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CMVView::OnDestroy()
{
	getModel()->DetachObserver(this);


	__super::OnDestroy();
}
// **************************************************************************
// 
// **************************************************************************
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