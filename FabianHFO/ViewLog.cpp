// ViewLog.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ViewLog.h"
#include "LangAdmin.h"
#include "globDefs.h"




//global font objects
//extern HFONT g_hf15AcuMed;

//extern CLangAdmin* g_pGlobalLanguageStrings;


// **************************************************************************
// 
// **************************************************************************
CViewLog::CViewLog(int iViewID):
CMVView(iViewID)
{
	//getModel()->AttachObserver(this);

	//width = 0;


	//m_pWndMenuCalibration=NULL;
	m_pWndAlarmLog=NULL;
}

// **************************************************************************
// 
// **************************************************************************
CViewLog::~CViewLog(void)
{
	
}


BEGIN_MESSAGE_MAP(CViewLog, CMVView)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// **************************************************************************
// 
// **************************************************************************
bool CViewLog::CreateView()
{
	RECT rcLd={6,220,600,535};

	if (!Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_LOGLIST))
	{
		return false;
	}

	Initialize();

	return true;
}
// **************************************************************************
// 
// **************************************************************************
BOOL CViewLog::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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

		//CBrush cbrBack(RGB(240,240,255));
		CBrush cbrBack(BACKGND);

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
bool CViewLog::Initialize()
{
	CreateWndAlarmLog();

	ShowWndAlarmLog(true);

	/*pListBox = new CListBox();
	pListBox->Create(WS_CHILD|WS_VISIBLE|WS_BORDER | LBS_NOSEL |WS_HSCROLL | WS_VSCROLL , 
		CRect(10,10,560,320), this, 1);

	pListBox->SetFont(CFont::FromHandle(g_hf15AcuMed));*/

	/*pListBox->AddString(_T("Alarm 1"));
	pListBox->AddString(_T("Alarm 2"));
	pListBox->AddString(_T("Alarm 3"));
	pListBox->AddString(_T("Alarm 4"));
	pListBox->AddString(_T("Alarm 5"));
	pListBox->AddString(_T("Alarm 6"));
	pListBox->AddString(_T("Alarm 7"));
	pListBox->AddString(_T("Alarm 8"));
	pListBox->AddString(_T("Alarm 9"));
	pListBox->AddString(_T("Alarm 10"));
	pListBox->AddString(_T("Alarm 11"));
	pListBox->AddString(_T("Alarm 12"));
	pListBox->AddString(_T("Alarm 13"));
	pListBox->AddString(_T("Alarm 14"));
	pListBox->AddString(_T("Alarm 15"));
	pListBox->AddString(_T("Alarm 16666666666666666666666666666666666666666665"));*/

	//CTlsLoglist* rLoglist = getModel()->getDATAHANDLER()->ALARMS->GetAlarmLog();
	//rLoglist.AddMessage(_T("started ******************************************************************************************************"));
	

	//testdata
	//getModel()->getDATAHANDLER()->ALARMS->SetAlarmLog(_T("Druck zu hoch [Pmax-Limit: 16 mbar]"));
	/*getModel()->getDATAHANDLER()->ALARMS->SetAlarmLog(_T("Patientenalarm 2"));
	getModel()->getDATAHANDLER()->ALARMS->SetAlarmLog(_T("Patientenalarm 3"));
	getModel()->getDATAHANDLER()->ALARMS->SetAlarmLog(_T("Patientenalarm 4"));
	getModel()->getDATAHANDLER()->ALARMS->SetAlarmLog(_T("Patientenalarm 5"));
	getModel()->getDATAHANDLER()->ALARMS->SetAlarmLog(_T("Patientenalarm 6"));
	getModel()->getDATAHANDLER()->ALARMS->SetAlarmLog(_T("Patientenalarm 7"));
	getModel()->getDATAHANDLER()->ALARMS->SetAlarmLog(_T("Patientenalarm 8"));
	getModel()->getDATAHANDLER()->ALARMS->SetAlarmLog(_T("Patientenalarm 9"));
	getModel()->getDATAHANDLER()->ALARMS->SetAlarmLog(_T("Patientenalarm 10"));
	getModel()->getDATAHANDLER()->ALARMS->SetAlarmLog(_T("Patientenalarm 11"));
	getModel()->getDATAHANDLER()->ALARMS->SetAlarmLog(_T("Patientenalarm 12"));
	getModel()->getDATAHANDLER()->ALARMS->SetAlarmLog(_T("Patientenalarm 13"));
	getModel()->getDATAHANDLER()->ALARMS->SetAlarmLog(_T("Patientenalarm 14"));
	getModel()->getDATAHANDLER()->ALARMS->SetAlarmLog(_T("Patientenalarm 15"));
	getModel()->getDATAHANDLER()->ALARMS->SetAlarmLog(_T("Patientenalarm 16666666666666666666666666666666666666666665"));*/

	

	/*AddString(_T("05/13/08 15:28: Alarm 1"));
	AddString(_T("05/13/08 15:28: Alarm 2"));
	AddString(_T("05/13/08 15:28: Alarm 3"));
	AddString(_T("05/13/08 15:28: Alarm 4"));
	AddString(_T("05/13/08 15:29: Alarm 5"));
	AddString(_T("05/13/08 15:33: Alarm 6"));
	AddString(_T("05/13/08 15:33: Alarm 7"));
	AddString(_T("05/13/08 15:33: Alarm 8"));
	AddString(_T("05/13/08 15:33: Alarm 9"));
	AddString(_T("05/13/08 15:52: Alarm 10"));
	AddString(_T("05/13/08 15:52: Alarm 11"));
	AddString(_T("05/13/08 15:52: Alarm 12"));
	AddString(_T("05/13/08 15:52: Alarm 13"));
	AddString(_T("05/13/08 15:52: Alarm 14"));
	AddString(_T("05/13/08 15:52: Alarm 15"));
	AddString(_T("05/13/08 15:53: Alarm 16666666666666666666666666666666666666666665"));*/
	
	

	/*int iCount = rLoglist->GetNumMessages();
	for(int idx = 0; idx < iCount; idx++)
		pListBox->AddString(rLoglist->GetMessage(idx));*/
	//m_lbLog.SetCurSel(m_lbLog.GetCount()-1);

	//SetHorizontalExtent(/*CListBox& ListBox*/);

	//rLoglist->AddListener(this);

	return true;
}
LRESULT CViewLog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
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
bool CViewLog::CreateWndAlarmLog()
{
	if(m_pWndAlarmLog==NULL)
	{


		m_pWndAlarmLog = new CWndAlarmLog();
		//RECT rcLd={m_lX,m_lY,m_lX+m_lCx,m_lY+m_lCy};

		RECT rcLd={0,0,580,330};
		//RECT rcLd={0,0,565,485};
		if(m_pWndAlarmLog->CreateWnd(this,rcLd,IDC_VIEW_LOGLIST_LOG))
		{


			return true;
		}


	}
	return false;
}
bool CViewLog::DestroyWndAlarmLog()
{
	if(m_pWndAlarmLog)
	{
		m_pWndAlarmLog->DestroyWindow();
		delete m_pWndAlarmLog;
		m_pWndAlarmLog=NULL;
	}
	return false;
}
void CViewLog::ShowWndAlarmLog(bool show)
{
	m_pWndAlarmLog->Show(show);


}

//http://www.codeproject.com/KB/combobox/logcontrol.aspx


//http://www.codeproject.com/KB/combobox/hscroll_listbox.aspx
//int CViewLog::AddString(LPCTSTR s)
//{
//	int result = pListBox->AddString(s);
//	if(result < 0)
//		return result;
//	updateWidth(s);
//	return result;
//}
//
//
//int CViewLog::InsertString(int i, LPCTSTR s)
//{
//	int result = pListBox->InsertString(i, s);
//	if(result < 0)
//		return result;
//	updateWidth(s);
//	return result;
//}
//
//void CViewLog::updateWidth(LPCTSTR s)
//{
//	//CClientDC dc(this);
//	CDC *pDC = pListBox->GetDC();
//
//	CFont * f = pListBox->GetFont();
//	pDC->SelectObject(f);
//
//	CSize sz = pDC->GetTextExtent(s, _tcslen(s));
//	sz.cx += 3 * ::GetSystemMetrics(SM_CXBORDER);
//	if(sz.cx > width)
//	{ /* extend */
//		width = sz.cx;
//		pListBox->SetHorizontalExtent(width);
//	} /* extend */
//}
//
//void CViewLog::ResetContent()
//{
//	pListBox->ResetContent();
//	width = 0;
//}
//
//int CViewLog::DeleteString(int n)
//{
//	int result = pListBox->DeleteString(n);
//	if(result < 0)
//		return result;
//	//CClientDC dc(this);
//	CDC *pDC = pListBox->GetDC();
//
//	CFont * f = pListBox->GetFont();
//	pDC->SelectObject(f);
//
//	width = 0;
//	for(int i = 0; i < pListBox->GetCount(); i++)
//	{ /* scan strings */
//		CStringW s;
//		pListBox->GetText(i, s);
//		CSize sz = pDC->GetTextExtent(s);
//		sz.cx += 3 * ::GetSystemMetrics(SM_CXBORDER);
//		if(sz.cx > width)
//			width = sz.cx;
//	} /* scan strings */
//	pListBox->SetHorizontalExtent(width);
//	return result;
//}



//void CViewLog::SetHorizontalExtent(/*CListBox& ListBox*/)
//{
//	int nMaxTextWidth = 0;
//
//	CDC *pDC = pListBox->GetDC();
//
//	if (pDC)
//	{
//		CFont *pOldFont = pDC->SelectObject(pListBox->GetFont());
//
//		CStringW Text;
//		const int nItems = pListBox->GetCount();
//
//		for (int i = 0; i < nItems; i++)
//		{
//			pListBox->GetText(i, Text);
//
//			Text += "X";  // otherwise item may be clipped.
//
//			const int nTextWidth = pDC->GetTextExtent(Text).cx;
//
//			if (nTextWidth > nMaxTextWidth)
//			{
//				nMaxTextWidth = nTextWidth;
//			}
//		}
//
//		pDC->SelectObject(pOldFont);
//
//		VERIFY(pListBox->ReleaseDC(pDC) != 0);
//	}
//	else
//	{
//		ASSERT(FALSE);
//	}
//
//	pListBox->SetHorizontalExtent(nMaxTextWidth);
//}


// **************************************************************************
// 
// **************************************************************************
//bool CViewLog::CreateWndMenuCalibration()
//{
//	// **********************************************************************
//	// Init. line diagram
//	if(m_pWndMenuCalibration==NULL && m_lX>-1)
//	{
//
//
//		m_pWndMenuCalibration = new CWndMenuCalibration(this);
//		RECT rcLd={0,550,800,600};
//
//		//RECT rcLd={477,0,640,435};
//		if(m_pWndMenuCalibration->Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_CALMENU))
//		{
//
//			m_pWndMenuCalibration->Init();
//
//
//			return true;
//		}
//
//
//	}
//	return false;
//}

// **************************************************************************
// 
// **************************************************************************
//void CViewLog::ShowWndMenuCalibration(bool bShow)
//{
//	if(m_pWndMenuCalibration)
//	{
//		m_pWndMenuCalibration->Show(bShow);
//	}
//}


// **************************************************************************
// 
// **************************************************************************
void CViewLog::Open()
{
	ShowWndAlarmLog(true);
}


// **************************************************************************
// 
// **************************************************************************
void CViewLog::Close()
{
	
}

// **************************************************************************
// 
// **************************************************************************
void CViewLog::Show()
{
	this->ShowWindow(SW_SHOW);

	ShowWndAlarmLog(true);


}


// **************************************************************************
// 
// **************************************************************************
void CViewLog::Hide()
{
	this->ShowWindow(SW_HIDE);

	ShowWndAlarmLog(false);
}




// **************************************************************************
// 
// **************************************************************************
void CViewLog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}




//************************************
// Method:    OnDestroy
// FullName:  CViewLog::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CViewLog::OnDestroy()
{
	DestroyWndAlarmLog();

	CMVView::OnDestroy();
}

