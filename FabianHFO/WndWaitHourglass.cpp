// WndWaitHourglass.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndWaitHourglass.h"

extern HFONT g_hf11AcuBold;

// CWndWaitHourglass

IMPLEMENT_DYNAMIC(CWndWaitHourglass, CWnd)

CWndWaitHourglass::CWndWaitHourglass()
{
	m_pModel=NULL;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;

	m_pcWait=NULL;
	m_pcWait1= NULL;
	m_pcWait2= NULL;
	m_pcWait3= NULL;
	m_pcWait4= NULL;
	m_pcWait5= NULL;
	m_pcWait6= NULL;
	m_pcWait7= NULL;
	m_pcWait8= NULL;
	m_iWaitCount=0;

	m_pcwtWaitThread=NULL;
	m_bDoWaitThread=false;
	m_hThreadWait=INVALID_HANDLE_VALUE;
}

CWndWaitHourglass::~CWndWaitHourglass()
{
	delete m_pcWait;
	m_pcWait=NULL;
	delete m_pcWait1;
	m_pcWait1=NULL;
	delete m_pcWait2;
	m_pcWait2=NULL;
	delete m_pcWait3;
	m_pcWait3=NULL;
	delete m_pcWait4;
	m_pcWait4=NULL;
	delete m_pcWait5;
	m_pcWait5=NULL;
	delete m_pcWait6;
	m_pcWait6=NULL;
	delete m_pcWait7;
	m_pcWait7=NULL;
	delete m_pcWait8;
	m_pcWait8=NULL;
}


BEGIN_MESSAGE_MAP(CWndWaitHourglass, CWnd)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//ON_WM_TIMER()
END_MESSAGE_MAP()



// CWndWaitHourglass message handlers



void CWndWaitHourglass::SetMessage(CStringW szText)
{
	m_szMessage=szText;
}

void CWndWaitHourglass::Show(BOOL bShow)
{
	if(bShow==TRUE)
	{
		this->ShowWindow(SW_SHOW);
		startWaitThread();
	}
	else
	{
		stopWaitThread();
		this->ShowWindow(SW_HIDE);
	}
	UpdateWindow();
	Invalidate();

}

// **************************************************************************
// 
// **************************************************************************
BOOL CWndWaitHourglass::Create(CWnd* pParentWnd, const RECT rc, UINT nID,CString szMessage) 
{
	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	m_szMessage=szMessage;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	
	if(CWnd::CreateEx(WS_EX_TOPMOST|WS_EX_DLGMODALFRAME/*|WS_EX_CLIENTEDGE*/,lpszClassName,_T("Menu"),WS_POPUP/*|WS_VISIBLE*/ ,rc,pParentWnd,nID))
	{
		CClientDC dc(this);

		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
		m_hBmpPrev=(HBITMAP)SelectObject(m_hDC,m_hBmp);

		//CBrush cbrBack(RGB(110,110,255));
		CBrush cbrBack(BACKGND);
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		

		
		m_pcWait=NULL;
		m_pcWait1= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT85_BLUE1);
		m_pcWait2= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT85_BLUE2);
		m_pcWait3= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT85_BLUE3);
		m_pcWait4= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT85_BLUE4);
		m_pcWait5= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT85_BLUE5);
		m_pcWait6= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT85_BLUE6);
		m_pcWait7= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT85_BLUE7);
		m_pcWait8= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT85_BLUE8);
		//m_pcWait9= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_DLG_HOUR9);
		m_iWaitCount=1;

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
void CWndWaitHourglass::startWaitThread()
{
	//KillTimer(HOURGLASSTIMER);

	m_bDoWaitThread=true;

	if(m_pcwtWaitThread!=NULL)
	{
		delete m_pcwtWaitThread;
		m_pcwtWaitThread=NULL;

		if(m_hThreadWait!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadWait);
			m_hThreadWait=INVALID_HANDLE_VALUE;
		}

	}

	m_pcwtWaitThread=AfxBeginThread(CWaitThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadWait=m_pcwtWaitThread->m_hThread;
	m_pcwtWaitThread->m_bAutoDelete = FALSE; 
	m_pcwtWaitThread->ResumeThread();
}

// **************************************************************************
// 
// **************************************************************************
void CWndWaitHourglass::stopWaitThread()
{
	//KillTimer(HOURGLASSTIMER);

	if(m_bDoWaitThread)
	{
		m_bDoWaitThread=false;

		if (WaitForSingleObject(m_pcwtWaitThread->m_hThread,1000) == WAIT_TIMEOUT)
		{
			if(!TerminateThread(m_pcwtWaitThread,0))
			{
				
			}
		}
	}
}

//************************************
// Method:    CWaitThread
// FullName:  CWaitThread
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID pc
//
// 2015/06/19: checked for correct closing of thread
//************************************
static UINT CWaitThread( LPVOID pc )
{
	try
	{
		((CWndWaitHourglass*)pc)->WaitHourglass();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CWaitThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CWaitThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CWndWaitHourglass*)pc)->WaitHourglass();
	return TRUE;
}

//************************************
// Method:    WaitHourglass
// FullName:  CWndWaitHourglass::WaitHourglass
// Access:    private 
// Returns:   DWORD
// Qualifier:
// Parameter: void
//
// 2015/06/19: checked for correct closing of thread
//************************************
DWORD CWndWaitHourglass::WaitHourglass(void) 
{
	CClientDC dc(this);

	HPEN hpenprev=(HPEN)SelectObject(dc.m_hDC,(HPEN)GetStockObject(NULL_PEN));

	RECT rc;
	int tc=SetTextColor(dc.m_hDC,0x0000000);
	HFONT hPrevFont=(HFONT)SelectObject(dc.m_hDC,g_hf11AcuBold);

	do
	{
		//hourglass 
		if(m_iWaitCount>8)
			m_iWaitCount=1;

		switch(m_iWaitCount)
		{
		case 1:
			{
				m_pcWait=m_pcWait1;
			}
			break;
		case 2:
			{
				m_pcWait=m_pcWait2;
			}
			break;
		case 3:
			{
				m_pcWait=m_pcWait3;
			}
			break;
		case 4:
			{
				m_pcWait=m_pcWait4;
			}
			break;
		case 5:
			{
				m_pcWait=m_pcWait5;
			}
			break;
		case 6:
			{
				m_pcWait=m_pcWait6;
			}
			break;
		case 7:
			{
				m_pcWait=m_pcWait7;
			}
			break;
		case 8:
			{
				m_pcWait=m_pcWait8;
			}
			break;
		default:
			{
				m_pcWait=NULL;
			}
			break;
		}

		if(m_pcWait)
			m_pcWait->Draw(dc.m_hDC,110,80);

		rc.left = 0;//450;  
		rc.top = 50;  
		rc.right  = 300;//580;  
		rc.bottom = 80;
		DrawText(dc.m_hDC,m_szMessage,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

		if(m_iWaitCount>0)
		{
			m_iWaitCount++;
		}

		Sleep(200);
	}while(m_bDoWaitThread);

	SelectObject(dc.m_hDC,hpenprev);
	SelectObject(dc.m_hDC,hPrevFont);
	SetTextColor(dc.m_hDC,tc);

	return 0;
}

//************************************
// Method:    OnDestroy
// FullName:  CWndWaitHourglass::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of thread
//************************************
void CWndWaitHourglass::OnDestroy()
{
	stopWaitThread();
	if(m_pcwtWaitThread!=NULL)
	{
		delete m_pcwtWaitThread;
		m_pcwtWaitThread=NULL;
		if(m_hThreadWait!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadWait);
			m_hThreadWait=INVALID_HANDLE_VALUE;
		}
	}

	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);
}


// **************************************************************************
// 
// **************************************************************************
void CWndWaitHourglass::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}
// **************************************************************************
// 
// **************************************************************************
//void CWndWaitHourglass::OnTimer(UINT_PTR nIDEvent)
//{
//	CClientDC dc(this);
//
//	if(nIDEvent==HOURGLASSTIMER)
//	{
//
//		HPEN hpenprev=(HPEN)SelectObject(dc.m_hDC,(HPEN)GetStockObject(NULL_PEN));
//
//		RECT rc;
//		int tc=SetTextColor(dc.m_hDC,0x0000000);
//		HFONT hPrevFont=(HFONT)SelectObject(dc.m_hDC,g_hf11AcuBold);
//
//		//hourglass 
//		if(m_iWaitCount>8)
//			m_iWaitCount=1;
//
//		switch(m_iWaitCount)
//		{
//		case 1:
//			{
//				m_pcWait=m_pcWait1;
//			}
//			break;
//		case 2:
//			{
//				m_pcWait=m_pcWait2;
//			}
//			break;
//		case 3:
//			{
//				m_pcWait=m_pcWait3;
//			}
//			break;
//		case 4:
//			{
//				m_pcWait=m_pcWait4;
//			}
//			break;
//		case 5:
//			{
//				m_pcWait=m_pcWait5;
//			}
//			break;
//		case 6:
//			{
//				m_pcWait=m_pcWait6;
//			}
//			break;
//		case 7:
//			{
//				m_pcWait=m_pcWait7;
//			}
//			break;
//		case 8:
//			{
//				m_pcWait=m_pcWait8;
//			}
//			break;
//		default:
//			{
//				m_pcWait=NULL;
//			}
//			break;
//		}
//
//		if(m_pcWait)
//			m_pcWait->Draw(dc.m_hDC,110,80);
//
//
//		rc.left = 0;//450;  
//		rc.top = 50;  
//		rc.right  = 300;//580;  
//		rc.bottom = 80;
//		//DrawText(dc.m_hDC,_T("... data will be loaded ..."),-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
//		//DrawText(dc.m_hDC,getModel()->GetLanguageString(IDS_TREND_LOADDATA),-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
//		DrawText(dc.m_hDC,m_szMessage,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
//
//		if(m_iWaitCount>0)
//		{
//			m_iWaitCount++;
//		}
//
//		SelectObject(dc.m_hDC,hpenprev);
//		SelectObject(dc.m_hDC,hPrevFont);
//		SetTextColor(dc.m_hDC,tc);
//
//	}
//
//}

// **************************************************************************
// 
// **************************************************************************
CMVModel *CWndWaitHourglass::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}