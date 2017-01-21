#include "StdAfx.h"
#include "WndTrendTimeaxis.h"
#include "FabianHFO.h"
#include "LangAdmin.h"
#include "globDefs.h"




//global font objects
extern HFONT g_hf8AcuBold;

//extern CLangAdmin* g_pGlobalLanguageStrings;



// CWndTrendTimeaxis

IMPLEMENT_DYNAMIC(CWndTrendTimeaxis, CWnd)

CWndTrendTimeaxis::CWndTrendTimeaxis(CMVView *parentView)
{
	m_parentView=parentView;

	m_pcTimeAxis=NULL;

	m_iXStartPos=0;
	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	
	m_lX				= 0;
	m_lY				= 0;
	m_lXo				= 0;
	m_lYo				= 0;

	SYSTEMTIME st;
	GetLocalTime(&st);
	m_dtStartTime=COleDateTime(st);
	m_dtEndTime=m_dtStartTime;
	m_iOffsetMinutes=0;
	m_iTimeSpanMinutes=MAX_TREND_MINUTES;
	m_dFactor=(double)399/(double)MAX_TREND_MINUTES;


	m_pModel = NULL;
}

CWndTrendTimeaxis::~CWndTrendTimeaxis()
{
	delete m_pcTimeAxis;
	m_pcTimeAxis=NULL;
}

// **************************************************************************
// 
// **************************************************************************
CMVModel *CWndTrendTimeaxis::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

BEGIN_MESSAGE_MAP(CWndTrendTimeaxis, CWnd)
	//ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CWndMenuTrendtype message handlers
// **************************************************************************
// 
// **************************************************************************
BOOL CWndTrendTimeaxis::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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

		CBrush cbrBack(RGB(252,252,170));
		
		//CBrush cbrBack(BACKGND);
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

// **************************************************************************
// 
// **************************************************************************
void CWndTrendTimeaxis::Init()
{
	CClientDC dc(this);

	//DWORD dwStyleNoTab = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW;
	//DWORD dwStyleNoTab = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW|WS_TABSTOP;

	m_pcTimeAxis= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_TIMEAXIS);
	

	Draw();


}


// **************************************************************************
// 
// **************************************************************************
void CWndTrendTimeaxis::Show(BOOL bShow)
{
	if(this->IsWindowVisible() == bShow)
	{
		Draw();
		return;
	}

	if(bShow==TRUE)
	{
		this->ShowWindow(SW_SHOW);
		Draw();
	}
	else
		this->ShowWindow(SW_HIDE);
}




//************************************
// Method:    OnDestroy
// FullName:  CWndTrendTimeaxis::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CWndTrendTimeaxis::OnDestroy() 
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

void CWndTrendTimeaxis::SetTimeRange(COleDateTime dtStartTime, COleDateTime dtEndTime,int iOffset, int iTimeSpan)
{
	m_dtStartTime=dtStartTime;
	m_dtEndTime=dtEndTime;

	m_iOffsetMinutes=iOffset;
	m_iTimeSpanMinutes=iTimeSpan;

	Draw();
}

//void CWndTrendTimeaxis::SetComplTimeRange(COleDateTime dtStartTime, COleDateTime dtEndTime)
//{
//	m_dtStartTime=dtStartTime;
//	m_dtEndTime=dtEndTime;
//
//	Draw();
//}
//
//void CWndTrendTimeaxis::SetCurrTimeRange(int iOffset, int iTimeSpan)
//{
//	m_iOffsetMinutes=iOffset;
//	m_iTimeSpanMinutes=iTimeSpan;
//
//	Draw();
//}

void CWndTrendTimeaxis::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	CBrush cbrBack(RGB(252,252,170));
	CBrush cbrRange(BACKGND);
	CBrush cbrTime(RGB(150,150,255));
	//CBrush cbrBack(BACKGND);

	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf8AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	m_pcTimeAxis->Draw(hdcMem,80,15);

	SelectObject(hdcMem,cbrRange);
	SelectObject(hdcMem, (HPEN)GetStockObject(BLACK_PEN));

	double dSpan=m_dFactor*(double)m_iTimeSpanMinutes;
	double dRight=490-(m_dFactor*(double)m_iOffsetMinutes);
	int iLeft=(int)(dRight-dSpan);
	if(iLeft<91)
		iLeft=91;
	int iRight=dRight;
	if(iLeft==iRight)
		iRight++;
	Rectangle(hdcMem, iLeft, rcCl.top+10, iRight, rcCl.bottom-10);

	SelectObject(hdcMem,cbrTime);
	SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));
	if(iLeft+1<iRight-1)
		Rectangle(hdcMem, iLeft+1, rcCl.top+15, iRight-1, rcCl.bottom-15);

	SelectObject(hdcMem,cbrRange);

	CStringW cs = m_dtEndTime.Format(_T("%d.%m."));

	rc.left = 10;  
	rc.top = 7;  
	rc.right  = 555;  
	rc.bottom = 40;
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	cs = m_dtEndTime.Format(_T("%H:%M"));

	rc.left = 10;  
	rc.top = 7;  
	rc.right  = 555;  
	rc.bottom = 40;
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);


	cs = m_dtStartTime.Format(_T("%d.%m."));

	rc.left = 10;  
	rc.top = 7;  
	rc.right  = 555;  
	rc.bottom = 40;
	DrawText(hdcMem,cs,-1,&rc,DT_RIGHT|DT_TOP|DT_SINGLELINE);

	cs = m_dtStartTime.Format(_T("%H:%M"));

	rc.left = 10;  
	rc.top = 7;  
	rc.right  = 555;  
	rc.bottom = 40;
	DrawText(hdcMem,cs,-1,&rc,DT_RIGHT|DT_BOTTOM|DT_SINGLELINE);




	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	//DeleteObject(cbrBack);
	//DeleteObject(cbrRange);
	//DeleteObject(cbrTime);

	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}

// **************************************************************************
// 
// **************************************************************************
BOOL CWndTrendTimeaxis::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	/*case WM_MOUSEMOVE:
		{
			switch(pMsg->wParam)
			{
			case MK_LBUTTON:
				{
					int xPos = GET_X_LPARAM(pMsg->lParam); 
					
					double dSpan=(m_dFactor*(double)m_iTimeSpanMinutes);
					double dRight=490-(m_dFactor*(double)m_iOffsetMinutes);
					int iLeft=(int)(dRight-dSpan);

					if(xPos>dRight)
					{
						m_parentView->PostMessage(WM_TREND_MOVERIGHT);
					}
					else if(xPos<iLeft)
					{
						m_parentView->PostMessage(WM_TREND_MOVELEFT);
					}


					
					
				}
				break;
			default:
				{
					int iSt=0;
				}
				break;
			}
		}
		break;*/
	//case WM_LBUTTONUP:
	//	{
	//		int xPos = GET_X_LPARAM(pMsg->lParam); 
	//		int yPos = GET_Y_LPARAM(pMsg->lParam);

	//		

	//		//m_iXStartPos = 0;
	//		//m_iYStartPos = 0;
	//	}
	//	break;
	case WM_LBUTTONDOWN:
		{
			m_iXStartPos = GET_X_LPARAM(pMsg->lParam);
			//m_iYStartPos = GET_Y_LPARAM(pMsg->lParam);

			if(m_iXStartPos>91 && m_iXStartPos<490)
			{
				double dSpan=m_dFactor*(double)m_iTimeSpanMinutes;
				double dRight=490-(m_dFactor*(double)m_iOffsetMinutes);
				int iLeft=(int)(dRight-dSpan);

				int iOffsetMinutes=m_iXStartPos+(dSpan/2)-91;
				int iPos=((399-iOffsetMinutes)/m_dFactor)+(m_iTimeSpanMinutes/2);
			
				if(m_iXStartPos<iLeft)
				{
					m_parentView->PostMessage(WM_TREND_MOVE,iPos);
				}
				else if(m_iXStartPos>dRight)
				{
					m_parentView->PostMessage(WM_TREND_MOVE,iPos);
				}
			}
		}
		break;
	default:
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}