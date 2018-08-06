// WndAlarmLog.cpp: Implementierungsdatei
//




#include "stdafx.h"
#include "FabianHFO.h"
#include "WndAlarmLog.h"

//global font objects
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf8AcuNorm;
extern HFONT g_hf8AcuBold;
//extern HFONT g_hf16Normal;
extern HFONT g_hf10AcuBold;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf13AcuBold;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf15AcuMed;
//extern HFONT g_hf24Normal;
//extern HFONT g_hf24Medium;
extern HFONT g_hf17AcuBold;
extern HFONT g_hf19AcuMed;
extern HFONT g_hf21AcuBold;
extern HFONT g_hf23AcuBold;
extern HFONT g_hf25AcuMed;
extern HFONT g_hf27AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf33AcuBold;
//extern HFONT g_hf42Bold;
extern HFONT g_hf43AcuBold;
extern HFONT g_hf53AcuBold;
//extern HFONT g_hf80Bold;
//extern HFONT g_hf90Bold;

/**********************************************************************************************//**
 * CWndAlarmLog
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CWndAlarmLog, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CWndAlarmLog class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndAlarmLog::CWndAlarmLog()
{
	m_pModel=NULL;

	m_hDC= NULL;
	m_hBmp= NULL;
	m_hBmpPrev= NULL;
	m_lX= 0;
	m_lY= 0;
	m_lXo= 0;
	m_lYo= 0;

	m_bExit=false;

	width = 0;

	m_pListBox = NULL;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CWndAlarmLog class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CWndAlarmLog::~CWndAlarmLog()
{
	try
	{
		if(m_pListBox && ::IsWindow(m_pListBox->m_hWnd))
		{
			m_pListBox->DestroyWindow();
		}
	}
	catch (...)
	{
		delete m_pListBox;
		m_pListBox=NULL;

		theApp.ReportException(_T("CWndAlarmLog::~CWndAlarmLog"));
	}
	
	delete m_pListBox;
	m_pListBox=NULL;
}

/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CWndAlarmLog::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


BEGIN_MESSAGE_MAP(CWndAlarmLog, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//ON_WM_TIMER()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CWndAlarmLog-Meldungshandler
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pParentWnd	If non-null, the parent window.
 * \param 		  	rc		  	The rectangle.
 * \param 		  	nID		  	The identifier.
 * \param [in,out]	pContext  	If non-null, the context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CWndAlarmLog::CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
{
	//RECT rcLd={6,220,600,535};

	if (!Create(pParentWnd,rc,IDC_VIEW_LOGLIST_LOG))
	{
		return false;
	}

	Initialize(rc);

	return true;
}

/**********************************************************************************************//**
 * Creates the Window instance that will be represented by this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pParentWnd	If non-null, the parent window.
 * \param 		  	rc		  	The rectangle.
 * \param 		  	nID		  	The identifier.
 * \param [in,out]	pContext  	If non-null, the context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CWndAlarmLog::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
{


	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD/*|WS_VISIBLE*/,rc,pParentWnd,nID))
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

/**********************************************************************************************//**
 * Initializes this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	rc	The rectangle.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CWndAlarmLog::Initialize(const RECT rc)
{
	/*RECT rcLd={rc.left+10,rc.top+10,rc.right-10,rc.bottom-10};*/

	//RECT rcLd={rc.left+34,rc.top+59,rc.left+295,rc.top+300};

	//pSkinListBox = new CSkinListCtrl();
	//pSkinListBox->Create(WS_CHILD|WS_VISIBLE|/*LBS_OWNERDRAWFIXED|*/ /*LBS_USETABSTOPS |*/LVS_REPORT | LBS_HASSTRINGS|WS_BORDER | LBS_NOSEL /*|WS_HSCROLL */| WS_VSCROLL   , 
	//	rcLd, this, 1);

	////pSkinListBox->EnableScrollBarCtrl(SB_VERT, false);
	////pSkinListBox->EnableScrollBarCtrl(SB_HORZ,false);
	//pSkinListBox->Init();

	//pSkinListBox->SetBkColor(RGB(76,85,118));
	//pSkinListBox->SetTextColor(RGB(222,222,222));

	//pSkinListBox->SetFont(CFont::FromHandle(g_hf7AcuNorm), TRUE);

	//pSkinListBox->InsertColumn(0, _T("BLANK"), LVCFMT_LEFT, 200);
	////pSkinListBox->InsertColumn(1, _T("SONG"), LVCFMT_LEFT, 200);
	///*m_SkinList.InsertColumn(2, "ARTIST", LVCFMT_LEFT, 100);
	//m_SkinList.InsertColumn(3, "GENRE", LVCFMT_LEFT, 100);*/

	//pSkinListBox->SetRedraw(FALSE);


	//pListBox = new CColorListBox();
	//pListBox->SetItemHeight(20);

	//pListBox->Create(WS_CHILD|WS_VISIBLE|LBS_OWNERDRAWFIXED| /*LBS_USETABSTOPS |*/ LBS_HASSTRINGS/*|WS_BORDER*/ | LBS_NOSEL /*|WS_HSCROLL | WS_VSCROLL*/   , 
	//	rcLd, this, 1);








	// create and populate the combo's listbox
	m_pListBox = new CXComboList(this);
	ASSERT(m_pListBox);

	if (m_pListBox)
	{
		CRect rect;
		//GetWindowRect(&rect);
		GetClientRect(&rect);
		//RECT rect={rc.left+10,rc.top+10,rc.right-50,rc.bottom-10};

		//rect.top+=10;
		rect.bottom-=30;
		rect.left+=20;
		//rect.right-=50;

		/*rect.top = rect.bottom;
		rect .bottom = rect.top + 100;*/

		CStringW szClassName = AfxRegisterWndClass(NULL);

		BOOL bSuccess = m_pListBox->CreateEx(0, szClassName, _T(""),
			WS_CHILD | /*WS_POPUP |*/ WS_VISIBLE /*| WS_VSCROLL*/ | WS_BORDER,
			rect,
			this, 0, NULL);

		if (!bSuccess)
		{
		}
		else
		{
			m_pListBox->SetFont(CFont::FromHandle(g_hf13AcuBold), FALSE);

			/*m_pListBox->AddString(_T("Warrigal"));

			m_pListBox->SetCurSel(0);

			m_pListBox->SetActive(m_nSize);*/
		}
	}





	CTlsLoglist* rLoglist = getModel()->getALARMHANDLER()->getAlarmLog();
	//rLoglist.AddMessage(_T("started ******************************************************************************************************"));

	COLORREF bg;
	COLORREF fg;

	int iCount = rLoglist->GetNumMessages();
	CStringW szItem=_T("");
	for(int idx = 0; idx < iCount; idx++)
	{
		szItem=rLoglist->GetMessage(idx);

		//pSkinListBox->InsertItem(idx, szItem);
		////pSkinListBox->SetItemText(idx, 1, szItem);
		///*m_SkinList.SetItemText(i, 2, "Matthew Good");
		//m_SkinList.SetItemText(i, 3, "Rock");*/

		
		
		int iPrio=_ttoi(szItem.Right(1));
		int iPos=szItem.ReverseFind(_T('#')); 
		szItem=szItem.Mid(0, iPos);
		switch(iPrio)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			{
				bg=SYSALARM_COL;
				fg=BLACK;
			}
			break;
		case 8:
			{
				bg=PATALARM_COL;
				fg=BLACK;
			}
			break;
		case 9:
			{
				bg=PATALARM_COL;
				fg=BLACK;
			}
			break;
		case 0:
			{
				bg=PSVBACKUP_COL;
				fg=BLACK;
			}
			break;
		default:
			{
				bg=SYSLIMIT_COL;
				fg=BLACK;
			}
			break;
		}
		

		//pListBox->AddItem(szItem, bg,fg);
		//m_pListBox->AddString(szItem);
		m_pListBox->AddItem(szItem, bg,fg);
		

	}

	//m_pListBox->SetCurSel(0);

	m_pListBox->SetActive();

	//SetHorizontalExtent(/*CListBox& ListBox*/);

	/*pSkinListBox->SetRedraw(TRUE);

	ListView_SetExtendedListViewStyle(pSkinListBox->m_hWnd, 
		LVS_EX_FULLROWSELECT  | LVS_EX_HEADERDRAGDROP);*/

	rLoglist->AddListener(this);

	return true;
}

/**********************************************************************************************//**
 * Shows
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CWndAlarmLog::Show(bool bShow)
{
	if(m_bExit)
		return;

	if(bShow)
	{
		SetWindowPos(NULL,0,0,m_lX,m_lX,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
	}
	else
	{
		SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);
	}
}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndAlarmLog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	// CWnd::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndAlarmLog::OnDestroy()
{
	m_bExit=true;

	getModel()->getALARMHANDLER()->getAlarmLog()->RemoveListener(this);

	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);
}

/**********************************************************************************************//**
 * Sets horizontal extent
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndAlarmLog::SetHorizontalExtent(/*CListBox& ListBox*/)
{
	//int nMaxTextWidth = 0;

	//CDC *pDC = pListBox->GetDC();

	//if (pDC)
	//{
	//	CFont *pOldFont = pDC->SelectObject(pListBox->GetFont());

	//	CStringW Text;
	//	const int nItems = pListBox->GetCount();

	//	for (int i = 0; i < nItems; i++)
	//	{
	//		pListBox->GetText(i, Text);

	//		Text += "X";  // otherwise item may be clipped.

	//		const int nTextWidth = pDC->GetTextExtent(Text).cx;

	//		if (nTextWidth > nMaxTextWidth)
	//		{
	//			nMaxTextWidth = nTextWidth;
	//		}
	//	}

	//	pDC->SelectObject(pOldFont);

	//	VERIFY(pListBox->ReleaseDC(pDC) != 0);
	//}
	//else
	//{
	//	ASSERT(FALSE);
	//}

	//pListBox->SetHorizontalExtent(nMaxTextWidth);


}

/**********************************************************************************************//**
 * Draws this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CWndAlarmLog::Draw()
{
	//RECT rcCl;
	//RECT rc;
	//GetClientRect(&rcCl);

	//CClientDC dc(this);



	////int bc=SetBkColor(m_hDC,BACKGND);
	////int bc=SetBkColor(m_hDC,RGB(181,178,181));

	//int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
	////FillRect(m_hDC,&rcCl,CBrush(BACKGND));

	//rc.left = 50;  
	//rc.top = 50;  
	//rc.right  = 592;  
	//rc.bottom = 400;

	//HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf21AcuBold);


	//int tc=SetTextColor(m_hDC,0x0000000);


	//CStringW cs = _T("Betriebsstunden: 200 h");


	//DrawText(m_hDC,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	//rc.left = 50;  
	//rc.top = 90;  
	//rc.right  = 592;  
	//rc.bottom = 400;


	//cs = _T("Akkulaufzeit: 100 h");


	//DrawText(m_hDC,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);




	////dc.BitBlt(0,0,rcCl.right,rcCl.bottom,CDC::FromHandle(m_hDC),0,0,SRCCOPY);
	////BitBlt(m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);
	//BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);


	//SetTextColor(m_hDC,tc);
	////SetBkColor(m_hDC,bc);
	//SetBkMode(m_hDC,nBkMode);



	//SelectObject(m_hDC,hPrevFont);

}

/**********************************************************************************************//**
 * Adds a message head
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	sMessage	The message.
 **************************************************************************************************/

void CWndAlarmLog::AddMessageHead(CStringW sMessage)
{
	if(m_bExit)
		return;

	if(GetParent())
		GetParent()->PostMessage(WM_SET_ALIMITTIMER);

	if(m_pListBox->GetCount() > 100)
		m_pListBox->DeleteString(100);

	COLORREF bg;
	COLORREF fg;
	CStringW szItem=sMessage;
	int iPrio=_ttoi(szItem.Right(1));
	int iPos=szItem.ReverseFind(_T('#')); 
	szItem=szItem.Mid(0, iPos);
	switch(iPrio)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		{
			bg=SYSALARM_COL;
			fg=BLACK;
		}
		break;
	case 8:
		{
			bg=PATALARM_COL;
			fg=BLACK;
		}
		break;
	case 9:
		{
			bg=PATALARM_COL;
			fg=BLACK;
		}
		break;
	case 0:
		{
			bg=PSVBACKUP_COL;
			fg=BLACK;
		}
		break;
	default:
		{
			bg=SYSLIMIT_COL;
			fg=BLACK;
		}
		break;
	}


	int idx = m_pListBox->AddHead(szItem,bg,fg);

	m_pListBox->SetActive();

	m_pListBox->SetCurSel(idx);
	m_pListBox->InvalidateListBox();

}

/**********************************************************************************************//**
 * Adds a message tail
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	sMessage	The message.
 **************************************************************************************************/

void CWndAlarmLog::AddMessageTail(CStringW sMessage)
{
	if(m_bExit)
		return;

	if(GetParent())
		GetParent()->PostMessage(WM_SET_ALIMITTIMER);

	if(m_pListBox->GetCount() > 100)
		m_pListBox->DeleteString(0);

	COLORREF bg;
	COLORREF fg;
	CStringW szItem=sMessage;
	int iPrio=_ttoi(szItem.Right(1));
	int iPos=szItem.ReverseFind(_T('#')); 
	szItem=szItem.Mid(0, iPos);
	switch(iPrio)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		{
			bg=SYSALARM_COL;
			fg=BLACK;
		}
		break;
	case 8:
		{
			bg=PATALARM_COL;
			fg=BLACK;
		}
		break;
	case 9:
		{
			bg=PATALARM_COL;
			fg=BLACK;
		}
		break;
	case 0:
		{
			bg=PSVBACKUP_COL;
			fg=BLACK;
		}
		break;
	default:
		{
			bg=SYSLIMIT_COL;
			fg=BLACK;
		}
		break;
	}


	m_pListBox->AddHead(szItem, bg,fg);

	m_pListBox->SetActive();

	/*m_pListBox->SetCurSel(idx);
	m_pListBox->InvalidateListBox();*/
}

