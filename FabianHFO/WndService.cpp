// WndService.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndService.h"

extern HFONT g_hf14AcuMed;
// CWndService

IMPLEMENT_DYNAMIC(CWndService, CWnd)

CWndService::CWndService()
{
	m_pModel=NULL;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;

	m_pcNumeric_Up=NULL;
	m_pcNumeric_Dw=NULL;
	m_pcMenuBack=NULL;
}

CWndService::~CWndService()
{
	delete m_pcNumeric_Up;
	m_pcNumeric_Up=NULL;

	delete m_pcNumeric_Dw;
	m_pcNumeric_Dw=NULL;

	delete m_pcMenuBack;
	m_pcMenuBack=NULL;
}

// **************************************************************************
// 
// **************************************************************************
CMVModel *CWndService::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


BEGIN_MESSAGE_MAP(CWndService, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CWndService message handlers
// **************************************************************************
// 
// **************************************************************************
BOOL CWndService::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
{
	bool bRes=false;
	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD/*|WS_VISIBLE*/,rc,pParentWnd,nID))
	{
		CClientDC dc(this);
		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
		m_hBmpPrev=(HBITMAP)SelectObject(m_hDC,m_hBmp);

		//CBrush cbrBack(RGB(210,210,255));
		//CBrush cbrBack(BACKGND);
		CBrush cbrBack(RGB(181,178,181));
		/*CBrush cbrFill;
		cbrFill.CreateSolidBrush(RGB(181,178,181));*/
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		m_pcNumeric_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUM_BACK_UP);
		m_pcNumeric_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUM_BACK_DW);

		BTN btn;

		btn.wID					= IDC_BTN_MENU_NUMERIC;	
		btn.poPosition.x		= 610;
		btn.poPosition.y		= 480;
		btn.pcBmpUp				= m_pcNumeric_Up;
		btn.pcBmpDown			= m_pcNumeric_Dw;
		btn.pcBmpFocus			= m_pcNumeric_Up;
		btn.pcBmpDisabled		= m_pcNumeric_Up;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		m_pcMenuBack=new CPushBtn(btn,COLOR_TXTBTNUP,true);
		m_pcMenuBack->Create(this,g_hf14AcuMed,0);
		m_pcMenuBack->SetText(_T(""),_T(""));
		m_pcMenuBack->ShowWindow(SW_HIDE);

		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();//rkuNEWFIX

		bRes=true;
	}
	else
		bRes=false;

	if(bRes)
		Init();

	return bRes;
}

void CWndService::Init()
{

}



void CWndService::Show(bool bShow)
{
	if(bShow)
	{
		SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);

		Draw();
	}
	else
	{
		SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);
	}


}
void CWndService::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	// CWnd::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

//************************************
// Method:    OnDestroy
// FullName:  CWndService::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndService::OnDestroy()
{
	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);
}




void CWndService::Draw()
{
	Invalidate();
	UpdateWindow();
}

// **************************************************************************
// 
// **************************************************************************
LRESULT CWndService::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_MENU_NUMERIC:
				{
					//BnClickedBack();
					if(GetParent())
						GetParent()->PostMessage(WM_EV_BN_SERVICE_BACK);
					return 1;
				}
				break;
			}
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}
//void CWndService::BnClickedBack()
//{
//	if(AfxGetApp())
//		AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_SERVICE_BACK);
//}