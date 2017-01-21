// ViewPatData.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ViewPatData.h"
#include "LangAdmin.h"
#include "globDefs.h"
#include "TlsRegistry.h"



// **************************************************************************
// 
// **************************************************************************
CViewPatData::CViewPatData(int iViewID):
CMVView(iViewID)
{
	m_pcWndPatient=NULL;
}

// **************************************************************************
// 
// **************************************************************************
CViewPatData::~CViewPatData(void)
{
}



BEGIN_MESSAGE_MAP(CViewPatData, CMVView)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// **************************************************************************
// 
// **************************************************************************
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

// **************************************************************************
// 
// **************************************************************************
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
// **************************************************************************
// 
// **************************************************************************
bool CViewPatData::Initialize()
{
	bool result=false;

	return result;
}

// **************************************************************************
// 
// **************************************************************************
void CViewPatData::Open()
{
}


// **************************************************************************
// 
// **************************************************************************
void CViewPatData::Close()
{
}


// **************************************************************************
// 
// **************************************************************************
void CViewPatData::Show()
{
	//bool result=false;

	
	CreateWndPatient();
	ShowWndPatient(true);
	

	this->ShowWindow(SW_SHOW);
}


// **************************************************************************
// 
// **************************************************************************
void CViewPatData::Hide()
{
	this->ShowWindow(SW_HIDE);
}

// **************************************************************************
// 
// **************************************************************************
void CViewPatData::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}




//************************************
// Method:    OnDestroy
// FullName:  CViewPatData::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CViewPatData::OnDestroy()
{
	CMVView::OnDestroy();
}
// **************************************************************************
// 
// **************************************************************************
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
void CViewPatData::ShowWndPatient(bool show)
{
	m_pcWndPatient->Show(show);

	m_pcWndPatient->SetFocus();
}


// **************************************************************************
// 
// **************************************************************************
//void CViewPatData::Draw()
//{
//
//	RECT rc={0,0,m_lX,m_lY};
//	//RECT rcCl;
//	//GetClientRect(&rcCl);
//
//	CClientDC dc(this);
//	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
//	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
//	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);
//
//	int bc=SetBkColor(hdcMem,BACKGND);
//	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
//
//	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf8AcuBold);
//
//	int tc=SetTextColor(hdcMem,0x0000000);
//
//	CBrush cbrBack(BACKGND);
//	HBRUSH hPrevBrush=(HBRUSH)SelectObject(hdcMem,(HBRUSH)cbrBack);
//	HPEN hPrevPen=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));
//	//HPEN hPrevPen=(HPEN)SelectObject(hdcMem, (HPEN)penEll);
//	
//	Rectangle(hdcMem, 0, 0, m_lX, m_lY);
//
//
//
//
//
//
//
//	
//	
//
//	dc.BitBlt(x,y,m_lX,m_lY,CDC::FromHandle(hdcMem),0,0,SRCCOPY);
//	
//	SelectObject(hdcMem, hPrevBrush);	
//	SelectObject(hdcMem, hPrevPen);
//
//
//	SetTextColor(hdcMem,tc);
//	SetBkColor(hdcMem,bc);
//	SetBkMode(hdcMem,nBkMode);
//
//	SelectObject(hdcMem,hBmpMemPrev);
//	DeleteObject(hBmpMem);
//
//	DeleteObject(cbrBack);
//	DeleteObject(cbrEll);
//	DeleteObject(penEll);
//
//	DeleteDC(hdcMem);
//}
