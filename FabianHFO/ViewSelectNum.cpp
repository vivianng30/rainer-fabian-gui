// ViewSelectNum.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ViewSelectNum.h"
#include "LangAdmin.h"
#include "globDefs.h"


CViewSelectNum::CViewSelectNum(int iViewID):
CMVView(iViewID)
{
	m_pWndMenuSelNum=NULL;

}

CViewSelectNum::~CViewSelectNum(void)
{
}



BEGIN_MESSAGE_MAP(CViewSelectNum, CMVView)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// **************************************************************************
// 
// **************************************************************************
bool CViewSelectNum::CreateView()
{
	//RECT rcLd={5,50,570,535};
	RECT rcLd={5,50,795,535};

	if (!Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_SELECTNUM))
	{
		return false;
	}

	Initialize();

	return true;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CViewSelectNum::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext*) 
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
		HPEN hpenprev = (HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);

		return 1;
	}
	else
		return 0;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewSelectNum::Initialize()
{
	bool result=CreateWndMenuSelNum();

	PostMessage(WM_SET_SETUPTIMER);

	return result;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewSelectNum::CreateWndMenuSelNum()
{
	if(m_pWndMenuSelNum==NULL && m_lX>-1)
	{
		m_pWndMenuSelNum = new CWndMenuSelectNum(this);
		RECT rcLd={0,551,800,600};

		//RECT rcLd={477,0,640,435};
		if(m_pWndMenuSelNum->Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_SELNUMMENU))
		{
			m_pWndMenuSelNum->Init();

			return true;
		}
	}
	return false;
}
bool CViewSelectNum::DestroyWndMenuSelNum()
{
	if(m_pWndMenuSelNum)
	{
		m_pWndMenuSelNum->DestroyWindow();
		delete m_pWndMenuSelNum;
		m_pWndMenuSelNum=NULL;
	}
	return false;
}

void CViewSelectNum::ShowWndMenuSelNum(bool bShow)
{
	if(m_pWndMenuSelNum)
	{
		m_pWndMenuSelNum->Show(bShow);
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewSelectNum::Open()
{
	PostMessage(WM_SET_SETUPTIMER);
}


// **************************************************************************
// 
// **************************************************************************
void CViewSelectNum::Close()
{

}

// **************************************************************************
// 
// **************************************************************************
void CViewSelectNum::Show()
{
	//bool result=false;

	ShowWndMenuSelNum(true);



	this->ShowWindow(SW_SHOW);

	PostMessage(WM_SET_SETUPTIMER);
}


// **************************************************************************
// 
// **************************************************************************
void CViewSelectNum::Hide()
{
	this->ShowWindow(SW_HIDE);

	ShowWndMenuSelNum(false);
}

// **************************************************************************
// 
// **************************************************************************
void CViewSelectNum::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);


	// CMVView::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
}

// **************************************************************************
// 
// **************************************************************************
void CViewSelectNum::SetViewFocus()
{
	this->SetFocus();

	PostMessage(WM_SET_SETUPTIMER);

}

// **************************************************************************
// 
// **************************************************************************
//void CViewSelectNum::NotifyEvent(CMVEvent* pEvent)
//{
//	switch(pEvent->GetET())
//	{
//	case CMVEvent::ET_UIEVENT:
//		{
//			CMVEventUI* pUIEvent = (CMVEventUI*)pEvent;
//			switch(pUIEvent->GetEventType())
//			{
//			/*case CMVEventUI::EV_BN_SETUP1:
//				{
//					
//				}
//				break;
//			case CMVEventUI::EV_BN_SETUP2:
//				{
//					
//				}
//				break;
//			case CMVEventUI::EV_BN_SETUP3:
//				{
//					
//				}
//				break;
//			case CMVEventUI::EV_BN_SETUP4:
//				{
//					
//				}
//				break;
//			case CMVEventUI::EV_BN_SETUP5:
//				{
//					
//				}
//				break;*/
//			default:
//				break;
//			}
//
//			//PostMessage(WM_SET_SETUPTIMER);
//		}
//	}
//}


//************************************
// Method:    OnDestroy
// FullName:  CViewSelectNum::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CViewSelectNum::OnDestroy()
{
	KillTimer(SETUPTIMER);

	DestroyWndMenuSelNum();
	
	CMVView::OnDestroy();
}

// **************************************************************************
// 
// **************************************************************************
bool CViewSelectNum::SetPrevFocus()
{
	return true;
}

bool CViewSelectNum::SetNextFocus()
{
	return true;
}

LRESULT CViewSelectNum::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_SET_SETUPTIMER:
		{
			KillTimer(SETUPTIMER);
			SetTimer(SETUPTIMER, 45000, NULL);

			return 1;
		}
		break;
	case WM_KILL_SETUPTIMER:
		{
			KillTimer(SETUPTIMER);

			return 1;
		}
		break;
	default:
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

// **************************************************************************
// 
// **************************************************************************
void CViewSelectNum::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==SETUPTIMER)
	{
		KillTimer(SETUPTIMER);
		
		/*if(CheckAlarmState()==false)*/
		{
			if(AfxGetApp())
			{
				AfxGetApp()->GetMainWnd()->SetFocus();
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_NUMERIC);
			}
			
		}
	}

	CWnd::OnTimer(nIDEvent);
}

