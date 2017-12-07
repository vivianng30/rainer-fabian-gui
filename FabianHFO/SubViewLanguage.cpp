// SubViewLanguage.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "SubViewLanguage.h"

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000

#define MAXLANG	7


//global font objects
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf8AcuNorm;
extern HFONT g_hf8AcuBold;
extern HFONT g_hf10AcuBold;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf13AcuBold;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf15AcuMed;
extern HFONT g_hf17AcuBold;
extern HFONT g_hf19AcuMed;
extern HFONT g_hf21AcuBold;
extern HFONT g_hf23AcuBold;
extern HFONT g_hf25AcuMed;
extern HFONT g_hf27AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf33AcuBold;
extern HFONT g_hf43AcuBold;
extern HFONT g_hf53AcuBold;


// CSubViewLanguage

IMPLEMENT_DYNAMIC(CSubViewLanguage, CWnd)

CSubViewLanguage::CSubViewLanguage()
{
	m_bInitialized=false;
	m_pModel=NULL;
	m_pWndHourglass=NULL;
	m_pcwtLoadLanguageThread=NULL;
	m_hThreadLoadLanguage=INVALID_HANDLE_VALUE;
	m_bDoLoadLanguageThread=false;

	m_bExit=false;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_lX				= 0;
	m_lY				= 0;
	m_lXo				= 0;
	m_lYo				= 0;
	m_szLangToLoad=_T("");

	m_pcsLangNames = getModel()->getLANGUAGE()->GetLangFiles();

	m_iCurPosInBlock=0;
	m_iCurBlockPos=0;
	m_iNumLanguages=0;

	m_iCurFocusedPos=0;
	m_iCurSelectedPos=0;

	m_pcUp_Up=NULL;
	m_pcUp_Dw=NULL;
	m_pcDw_Up=NULL;
	m_pcDw_Dw=NULL;
	m_pcNextUp=NULL;
	m_pcNextDwn=NULL;
	m_pcFocused=NULL;
	m_pcNotSel=NULL;
	m_pcSelected=NULL;

	m_pbtnLanguage1=NULL;
	m_pbtnLanguage2=NULL;
	m_pbtnLanguage3=NULL;
	m_pbtnLanguage4=NULL;
	m_pbtnLanguage5=NULL;
	m_pbtnLanguage6=NULL;
	m_pbtnLanguage7=NULL;

	m_dwLastSetupTimer=0;

	m_bSDCARDfont=false;
}

CSubViewLanguage::~CSubViewLanguage()
{
	delete m_pcNextUp;
	m_pcNextUp=NULL;
	delete m_pcNextDwn;
	m_pcNextDwn=NULL;

	delete m_pbtnLanguage1;
	m_pbtnLanguage1=NULL;
	delete m_pbtnLanguage2;
	m_pbtnLanguage2=NULL;
	delete m_pbtnLanguage3;
	m_pbtnLanguage3=NULL;
	delete m_pbtnLanguage4;
	m_pbtnLanguage4=NULL;
	delete m_pbtnLanguage5;
	m_pbtnLanguage5=NULL;
	delete m_pbtnLanguage6;
	m_pbtnLanguage6=NULL;
	delete m_pbtnLanguage7;
	m_pbtnLanguage7=NULL;

	delete m_pcUp_Up;
	m_pcUp_Up=NULL;
	delete m_pcUp_Dw;
	m_pcUp_Dw=NULL;
	delete m_pcDw_Up;
	m_pcDw_Up=NULL;
	delete m_pcDw_Dw;
	m_pcDw_Dw=NULL;

	delete m_pcFocused;
	m_pcFocused=NULL;
	delete m_pcSelected;
	m_pcSelected=NULL;
	delete m_pcNotSel;
	m_pcNotSel=NULL;
}


BEGIN_MESSAGE_MAP(CSubViewLanguage, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SETUP_1, &CSubViewLanguage::OnBnClickedLang1)
	ON_BN_CLICKED(IDC_BTN_SETUP_2, &CSubViewLanguage::OnBnClickedLang2)
	ON_BN_CLICKED(IDC_BTN_SETUP_3, &CSubViewLanguage::OnBnClickedLang3)
	ON_BN_CLICKED(IDC_BTN_SETUP_4, &CSubViewLanguage::OnBnClickedLang4)
	ON_BN_CLICKED(IDC_BTN_SETUP_5, &CSubViewLanguage::OnBnClickedLang5)
	ON_BN_CLICKED(IDC_BTN_SETUP_6, &CSubViewLanguage::OnBnClickedLang6)
	ON_BN_CLICKED(IDC_BTN_SETUP_7, &CSubViewLanguage::OnBnClickedLang7)
	ON_BN_CLICKED(IDC_BTN_SETUP_NEXTUP, &CSubViewLanguage::OnBnClickedNextUp)
	ON_BN_CLICKED(IDC_BTN_SETUP_NEXTDW, &CSubViewLanguage::OnBnClickedNextDw)
END_MESSAGE_MAP()



// CSubViewLanguage message handlers

// **************************************************************************
// 
// **************************************************************************
CMVModel *CSubViewLanguage::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

// **************************************************************************
// 
// **************************************************************************
bool CSubViewLanguage::CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
{
	if (!Create(pParentWnd, rc, nID, pContext))
	{
		return false;
	}

	return true;
}
// **************************************************************************
// 
// **************************************************************************
BOOL CSubViewLanguage::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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

		//CBrush cbrBack(RGB(210,210,255));
		CBrush cbrBack(BACKGND);
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		m_pcUp_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_TOP_UP);
		m_pcUp_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_TOP_DW);
		m_pcDw_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_BOT_UP);
		m_pcDw_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_BOT_DW);

		m_pcNotSel= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_SETLANG_NOT);
		m_pcFocused= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_SETLANG_FOC);
		m_pcSelected= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SETUP_SETLANG_SEL);
		

		BTN btn;

		btn.wID					= IDC_BTN_SETUP_NEXTUP;	
		btn.poPosition.x		= 192;
		btn.poPosition.y		= 30;
		btn.pcBmpUp				= m_pcUp_Up;
		btn.pcBmpDown			= m_pcUp_Dw;
		btn.pcBmpFocus			= m_pcUp_Up;
		btn.pcBmpDisabled		= m_pcUp_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcNextUp=new CBtn(btn,COLOR_TXTBTNUP);
		m_pcNextUp->Create(this,g_hf21AcuBold,0);
		m_pcNextUp->SetText(_T(""));
		m_pcNextUp->ShowWindow(SW_SHOW);

		btn.wID					= IDC_BTN_SETUP_NEXTDW;	
		btn.poPosition.x		= 192;
		btn.poPosition.y		= 396;
		btn.pcBmpUp				= m_pcDw_Up;
		btn.pcBmpDown			= m_pcDw_Dw;
		btn.pcBmpFocus			= m_pcDw_Up;
		btn.pcBmpDisabled		= m_pcDw_Up;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcNextDwn=new CBtn(btn,COLOR_TXTBTNUP);
		m_pcNextDwn->Create(this,g_hf21AcuBold,0);
		m_pcNextDwn->SetText(_T(""));
		m_pcNextDwn->ShowWindow(SW_SHOW);


		btn.wID					= IDC_BTN_SETUP_1;	
		btn.poPosition.x		= 192;
		btn.poPosition.y		= 75;
		btn.pcBmpUp				= m_pcNotSel;
		btn.pcBmpDown			= m_pcSelected;
		btn.pcBmpFocus			= m_pcFocused;
		btn.pcBmpDisabled		= m_pcNotSel;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnLanguage1=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
		m_pbtnLanguage1->Create(this,g_hf21AcuBold,0);
		m_pbtnLanguage1->SetText(_T(""));
		m_pbtnLanguage1->ShowWindow(SW_HIDE);
		m_plLangBtn.AddTail(m_pbtnLanguage1);


		btn.wID					= IDC_BTN_SETUP_2;	
		btn.poPosition.x		= 192;
		btn.poPosition.y		= 121;
		btn.pcBmpUp				= m_pcNotSel;
		btn.pcBmpDown			= m_pcSelected;
		btn.pcBmpFocus			= m_pcFocused;
		btn.pcBmpDisabled		= m_pcNotSel;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnLanguage2=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
		m_pbtnLanguage2->Create(this,g_hf21AcuBold,0);
		m_pbtnLanguage2->SetText(_T(""));
		m_pbtnLanguage2->ShowWindow(SW_HIDE);
		m_plLangBtn.AddTail(m_pbtnLanguage2);

		btn.wID					= IDC_BTN_SETUP_3;	
		btn.poPosition.x		= 192;
		btn.poPosition.y		= 167;
		btn.pcBmpUp				= m_pcNotSel;
		btn.pcBmpDown			= m_pcSelected;
		btn.pcBmpFocus			= m_pcFocused;
		btn.pcBmpDisabled		= m_pcNotSel;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnLanguage3=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
		m_pbtnLanguage3->Create(this,g_hf21AcuBold,0);
		m_pbtnLanguage3->SetText(_T(""));
		m_pbtnLanguage3->ShowWindow(SW_HIDE);
		m_plLangBtn.AddTail(m_pbtnLanguage3);

		btn.wID					= IDC_BTN_SETUP_4;	
		btn.poPosition.x		= 192;
		btn.poPosition.y		= 213;
		btn.pcBmpUp				= m_pcNotSel;
		btn.pcBmpDown			= m_pcSelected;
		btn.pcBmpFocus			= m_pcFocused;
		btn.pcBmpDisabled		= m_pcNotSel;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnLanguage4=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
		m_pbtnLanguage4->Create(this,g_hf21AcuBold,0);
		m_pbtnLanguage4->SetText(_T(""));
		m_pbtnLanguage4->ShowWindow(SW_HIDE);
		m_plLangBtn.AddTail(m_pbtnLanguage4);

		btn.wID					= IDC_BTN_SETUP_5;	
		btn.poPosition.x		= 192;
		btn.poPosition.y		= 259;
		btn.pcBmpUp				= m_pcNotSel;
		btn.pcBmpDown			= m_pcSelected;
		btn.pcBmpFocus			= m_pcFocused;
		btn.pcBmpDisabled		= m_pcNotSel;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnLanguage5=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
		m_pbtnLanguage5->Create(this,g_hf21AcuBold,0);
		m_pbtnLanguage5->SetText(_T(""));
		m_pbtnLanguage5->ShowWindow(SW_HIDE);
		m_plLangBtn.AddTail(m_pbtnLanguage5);

		btn.wID					= IDC_BTN_SETUP_6;	
		btn.poPosition.x		= 192;
		btn.poPosition.y		= 305;
		btn.pcBmpUp				= m_pcNotSel;
		btn.pcBmpDown			= m_pcSelected;
		btn.pcBmpFocus			= m_pcFocused;
		btn.pcBmpDisabled		= m_pcNotSel;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnLanguage6=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
		m_pbtnLanguage6->Create(this,g_hf21AcuBold,0);
		m_pbtnLanguage6->SetText(_T(""));
		m_pbtnLanguage6->ShowWindow(SW_HIDE);
		m_plLangBtn.AddTail(m_pbtnLanguage6);

		btn.wID					= IDC_BTN_SETUP_7;	
		btn.poPosition.x		= 192;
		btn.poPosition.y		= 351;
		btn.pcBmpUp				= m_pcNotSel;
		btn.pcBmpDown			= m_pcSelected;
		btn.pcBmpFocus			= m_pcFocused;
		btn.pcBmpDisabled		= m_pcNotSel;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pbtnLanguage7=new CSelectSetupBtn(btn,COLOR_TXTBTNUP);
		m_pbtnLanguage7->Create(this,g_hf21AcuBold,0);
		m_pbtnLanguage7->SetText(_T(""));
		m_pbtnLanguage7->ShowWindow(SW_HIDE);
		m_plLangBtn.AddTail(m_pbtnLanguage7);

		HANDLE hSearch;
		WIN32_FIND_DATA fileData;
		//BOOL bFinished = false;

		m_bSDCARDfont=false;

		//TCHAR szFileName[255];

		hSearch = FindFirstFile(_T("\\sdcard\\fonts\\*.ttf"), &fileData);

		if (hSearch != INVALID_HANDLE_VALUE)
		{
			m_bSDCARDfont=true;

			FindClose(hSearch);
		}
		else
			hSearch = NULL;


		CStringW cs = _T("");
		POSITION pos;
		size_t j=0;
		size_t jCount=m_plLangBtn.GetCount();

		if(m_pcsLangNames && m_pcsLangNames->GetCount()>0)
		{
			for( pos = m_pcsLangNames->GetHeadPosition(); pos != NULL; )
			{
				if(m_bExit)
				{
					m_bInitialized=true;
					return 0;
				}
				cs = m_pcsLangNames->GetNext(pos) ;
				/*if(cs==LANGFILE_CHINESE && m_bSDCARDfont==false)
					continue;*/

				m_iNumLanguages++;

				CStringW csCurLang=getModel()->getCONFIG()->getLANGUAGE();

				if(csCurLang==cs)
				{
					m_iCurSelectedPos=m_iNumLanguages;
				}

				int iPos=cs.Find(_T('.'));
				CStringW csTemp=cs.Left(iPos);
				//g_pGlobalLanguageStrings->LoadLang(cs, true);

				//CStringW csTemp=getModel()->GetLanguageString(IDS_CURLANGUAGE);

				if(j<jCount)
				{
					CSelectSetupBtn*btn = m_plLangBtn.GetAt(m_plLangBtn.FindIndex(j));

					if(btn!=NULL)
					{
						btn->LoadFlag(csTemp);
						m_plUsedLangBtn.AddTail(btn);

						btn->ShowWindow(SW_SHOW);
						btn->RefreshText(csTemp);
						btn->SetLangString(cs);

						if(getModel()->getCONFIG()->getLANGUAGE()==cs)
						{
							btn->DrawOK(true);
						}

						j++;
					}
				}
			}
		}
		

		m_iCurBlockPos=j;
	
		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		//cbrBack.DeleteObject();//rkuNEWFIX

		m_bInitialized=true;

		if(m_iCurSelectedPos>MAXLANG)
		{
			UINT diff=m_iCurSelectedPos-MAXLANG;
			for(int i=0;i<diff;i++)
				ReorganizeBtns(true);
		}

		

		return 1;
	}
	else
	{
		m_bInitialized=true;

		return 0;
	}
}



void CSubViewLanguage::Show(bool bShow)
{
	if(m_bExit)
		return;

	if(bShow)
	{
		ShowWindow(SW_SHOW);
		//SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);

		Draw();

		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	else
	{

		ShowWindow(SW_HIDE);
		//SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);
	}
	
}
void CSubViewLanguage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein.
	// CWnd::OnPaint() soll zum Zeichnen von Meldungen nicht aufgerufen werden.
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

//************************************
// Method:    OnDestroy
// FullName:  CSubViewLanguage::OnDestroy
// Access:    public 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CSubViewLanguage::OnDestroy()
{
	m_bExit=true;

	BYTE iTimeout=0;
	if(m_bInitialized==false)
	{
		while((iTimeout<50) && !m_bInitialized )
		{
			Sleep(100);
			iTimeout++;
		}
	}

	StopLoadLanguageThread();
	if(m_pcwtLoadLanguageThread!=NULL)
	{
		delete m_pcwtLoadLanguageThread;
		m_pcwtLoadLanguageThread=NULL;

		if(m_hThreadLoadLanguage!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadLoadLanguage);
			m_hThreadLoadLanguage=INVALID_HANDLE_VALUE;
		}
	}

	DestroyWndHourglass();

	m_plLangBtn.RemoveAll();
	m_plUsedLangBtn.RemoveAll();

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
BOOL CSubViewLanguage::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				eBtnState eState = GetCurrentBtnState();

				if(eState==BS_FOCUSED)
					SetOneButtonDepressed(m_iCurPosInBlock);
				else if(eState==BS_DOWN)
					SetOneButtonFocused(m_iCurPosInBlock);
				else
					SetOneButtonFocused(IDC_BTN_SETUP_1);
				return 1;
			}
			else if(pMsg->wParam==VK_DOWN)
			{
				eBtnState eState = GetCurrentBtnState();
				if(		eState==BS_FOCUSED
					||	eState==BS_DOWN)
					SetPrevButtonFocused();

				return 1;
			}
			else if(pMsg->wParam==VK_UP)
			{
				eBtnState eState = GetCurrentBtnState();
				if(		eState==BS_FOCUSED
					||	eState==BS_DOWN)
					SetNextButtonFocused();
				return 1;
			}
			else
			{
				//handle focus change manual, draw next focused button and return true
				//return 1;
			}
			break;
		}
		break;
	default:
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}

void CSubViewLanguage::Draw()
{
	CClientDC dc(this);

	int nBkMode=SetBkMode(m_hDC,TRANSPARENT);


	if(m_pcsLangNames && m_iNumLanguages>7)
	{
		CBrush cbrLightBlue,cbrGreen,cbrDarkBlue;
		cbrLightBlue.CreateSolidBrush(RGB(190,190,255));
		cbrDarkBlue.CreateSolidBrush(RGB(85,95,255));
		cbrGreen.CreateSolidBrush(RGB(0,191,0));
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrLightBlue);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));

		if(m_iNumLanguages%2==0)
		{
			int iSize=m_iNumLanguages/2;
			int iTop=235-12-(iSize*15);
			if(iTop<0)
				iTop=0;
			int iLeft=620;
			int iRight=630;
			int iBottom=iTop+10;
			bool bChange=false;

			for(int i=1;i<=m_iNumLanguages;i++)
			{
				if(m_iCurFocusedPos==i)
				{
					SelectObject(m_hDC, (HBRUSH)cbrDarkBlue);
					bChange=true;
				}
				else if(m_iCurSelectedPos==i)
				{
					SelectObject(m_hDC, (HBRUSH)cbrGreen);
					bChange=true;
				}
				Ellipse(m_hDC,iLeft,iTop,iRight,iBottom);
				iTop=iTop+15;
				iBottom=iTop+10;
				if(bChange)
				{
					SelectObject(m_hDC, (HBRUSH)cbrLightBlue);
					bChange=false;
				}
			}
		}
		else
		{
			int iSize=m_iNumLanguages/2;
			int iTop=235-5-(iSize*15);
			if(iTop<0)
				iTop=0;
			int iLeft=620;
			int iRight=630;
			int iBottom=iTop+10;
			bool bChange=false;

			for(int i=1;i<=m_iNumLanguages;i++)
			{
				if(m_iCurFocusedPos==i)
				{
					SelectObject(m_hDC, (HBRUSH)cbrDarkBlue);
					bChange=true;
				}
				else if(m_iCurSelectedPos==i)
				{
					SelectObject(m_hDC, (HBRUSH)cbrGreen);
					bChange=true;
				}
				Ellipse(m_hDC,iLeft,iTop,iRight,iBottom);
				iTop=iTop+15;
				iBottom=iTop+10;
				if(bChange)
				{
					SelectObject(m_hDC, (HBRUSH)cbrLightBlue);
					bChange=false;
				}
			}
		}


		SelectObject(m_hDC,hbrprev);
		SelectObject(m_hDC,hpenprev);
	}


	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);


	//SetTextColor(m_hDC,tc);
	//SetBkColor(m_hDC,bc);
	SetBkMode(m_hDC,nBkMode);

	//SelectObject(m_hDC,hPrevFont);

	
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewLanguage::SetOneButtonDepressed(int btnID)
{
	POSITION pos;

	for( pos = m_plUsedLangBtn.GetHeadPosition(); pos != NULL; )
	{
		CSelectSetupBtn* pBtn = m_plUsedLangBtn.GetNext( pos );

		//int iT = pBtn->GetBtnId();

		if(pBtn->GetBtnId() != btnID)
		{
			//if(pBtn->GetState()==CPresetMenuBtn::DOWN)
			{
				pBtn->DrawOK(false);
				pBtn->SetState(BS_UP);
				
			}
		}
		else
		{
			pBtn->SetState(BS_DOWN);
			pBtn->DrawOK(true);
			m_iCurPosInBlock=btnID;

			SetLanguage(m_iCurPosInBlock);
			//getModel()->getCONFIG()->SetLastSelectedSVSettingBtns(m_iCurPara);
		}
	}
	SetFocusedPos(0);

	if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
	{
		m_dwLastSetupTimer=GetTickCount();
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	Draw();
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewLanguage::SetSelectedPos(CStringW csLang)
{
	CStringW cs = _T("");
	POSITION pos;
	int iNumLanguages=0;

	if(m_pcsLangNames && m_pcsLangNames->GetCount()>0)
	{
		for( pos = m_pcsLangNames->GetHeadPosition(); pos != NULL; )
		{
			if(m_bExit)
			{
				return;
			}
			cs = m_pcsLangNames->GetNext(pos) ;

			iNumLanguages++;

			if(cs==csLang)
			{
				m_iCurSelectedPos=iNumLanguages;
				return;
			}

		}
	}
}
// **************************************************************************
// 
// **************************************************************************
void CSubViewLanguage::SetFocusedPos(int btnID)
{
	POSITION pos;
	int iCurPos=1;

	if(btnID==0)
	{
		m_iCurFocusedPos=0;
		return;
	}

	for( pos = m_plUsedLangBtn.GetHeadPosition(); pos != NULL; )
	{
		CSelectSetupBtn* pBtn = m_plUsedLangBtn.GetNext( pos );

		if(pBtn->GetBtnId() == btnID)
		{
			CStringW szTemp = pBtn->GetLangString();
			if(szTemp.IsEmpty()==0)
			{
				if(m_pcsLangNames && m_pcsLangNames->GetCount()>0)
				{
					for( pos = m_pcsLangNames->GetHeadPosition(); pos != NULL; )
					{
						if(m_bExit)
						{
							return;
						}
						CStringW cs = m_pcsLangNames->GetNext(pos) ;

						if(cs==szTemp)
						{
							m_iCurFocusedPos=iCurPos;
							return;
						}

						iCurPos++;


					}
				}
			}
		}

	}

}

// **************************************************************************
// 
// **************************************************************************
void CSubViewLanguage::SetOneButtonFocused(int btnID)
{
	POSITION pos;

	for( pos = m_plUsedLangBtn.GetHeadPosition(); pos != NULL; )
	{
		CSelectSetupBtn* pBtn = m_plUsedLangBtn.GetNext( pos );

		//int iT = pBtn->GetBtnId();

		if(pBtn->GetBtnId() != btnID)
		{
			//if(pBtn->GetState()==CPresetMenuBtn::DOWN)
			{
				pBtn->SetState(BS_UP);
			}
		}
		else
		{
			pBtn->SetState(BS_FOCUSED);
			m_iCurPosInBlock=btnID;
			SetFocusedPos(m_iCurPosInBlock);
		}
	}
	if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
	{
		m_dwLastSetupTimer=GetTickCount();
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	Draw();
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewLanguage::SetNextButtonFocused()
{
	POSITION pos;

	pos = m_plUsedLangBtn.GetHeadPosition();

	do {
		CSelectSetupBtn* pBtn  = m_plUsedLangBtn.GetNext(pos);

		//int iT = pBtn->GetBtnId();



		if(m_iCurPosInBlock == pBtn->GetBtnId())
		{
			/*pBtn->SetState(BS_UP);*/

			//iT = pBtn->GetBtnId();

			/*int iTest = m_iCurBlockPos;
			iTest = m_iCurPosInBlock;
			iTest = m_iNumLanguages;*/

			if(pos==NULL)
			{
				if(m_iNumLanguages==m_iCurBlockPos)
				{
					/*pBtn->SetState(BS_UP);

					pos = m_plUsedLangBtn.GetHeadPosition();
					pBtn = m_plUsedLangBtn.GetNext( pos );

					iT = pBtn->GetBtnId();

					pBtn->SetState(BS_FOCUSED);
					m_iCurPosInBlock=pBtn->GetBtnId();*/

					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);

					break;
				}
				else if(m_iNumLanguages>MAXLANG && m_iCurPosInBlock==IDC_BTN_SETUP_7)
				{
					pBtn->SetState(BS_UP);
					ReorganizeBtns(true);
					break;
				}
				else
				{
					pBtn->SetState(BS_UP);

					pos = m_plUsedLangBtn.GetHeadPosition();
					pBtn = m_plUsedLangBtn.GetNext( pos );

					pBtn->SetState(BS_FOCUSED);
					m_iCurPosInBlock=pBtn->GetBtnId();
					SetFocusedPos(m_iCurPosInBlock);
					break;
				}
				
			}
			else
			{
				pBtn->SetState(BS_UP);

				pBtn = m_plUsedLangBtn.GetNext( pos );
				pBtn->SetState(BS_FOCUSED);
				m_iCurPosInBlock=pBtn->GetBtnId();
				SetFocusedPos(m_iCurPosInBlock);
				break;
			}
		}


	} while (pos != NULL);

	if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
	{
		m_dwLastSetupTimer=GetTickCount();
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	Draw();
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewLanguage::SetPrevButtonFocused()
{
	POSITION pos;

	pos = m_plUsedLangBtn.GetTailPosition();

	do {
		CSelectSetupBtn* pBtn  = m_plUsedLangBtn.GetPrev(pos);

		//int iT = pBtn->GetBtnId();

		/*pBtn->SetState(BS_UP);*/

		/*int iTest = m_iCurBlockPos;
		iTest = m_iCurPosInBlock;
		iTest = m_iNumLanguages;*/

		if(m_iCurPosInBlock == pBtn->GetBtnId())
		{
			//iT = pBtn->GetBtnId();
			if(pos == NULL)
			{
				if(MAXLANG==m_iCurBlockPos && m_iCurPosInBlock==IDC_BTN_SETUP_1)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
					break;
				}
				else if(m_iNumLanguages>MAXLANG  && m_iCurBlockPos>MAXLANG)
				{
					pBtn->SetState(BS_UP);
					ReorganizeBtns(false);
					break;
				}
				else
				{
					pBtn->SetState(BS_UP);
					pos = m_plUsedLangBtn.GetTailPosition();
					pBtn = m_plUsedLangBtn.GetPrev( pos );

					pBtn->SetState(BS_FOCUSED);
					m_iCurPosInBlock=pBtn->GetBtnId();
					SetFocusedPos(m_iCurPosInBlock);

					break;
				}

				
			}
			else
			{
				pBtn->SetState(BS_UP);
				pBtn = m_plUsedLangBtn.GetPrev( pos );
				pBtn->SetState(BS_FOCUSED);
				m_iCurPosInBlock=pBtn->GetBtnId();
				SetFocusedPos(m_iCurPosInBlock);

				break;
			}
		}


	} while (pos != NULL);

	if(getModel()->isSafeTickCountDelayExpired(m_dwLastSetupTimer, 1000))
	{
		m_dwLastSetupTimer=GetTickCount();
		if(GetParent())
			GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
	Draw();
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewLanguage::OnBnClickedLang1()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_1);
	/*if(eState==BS_UP)
		SetOneButtonFocused(IDC_BTN_SETUP_1);
	else if(eState==BS_FOCUSED)
		SetOneButtonDepressed(IDC_BTN_SETUP_1);
	else if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_1);*/

	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_1);
	else
		SetOneButtonDepressed(IDC_BTN_SETUP_1);
}
// **************************************************************************
// 
// **************************************************************************
void CSubViewLanguage::OnBnClickedLang2()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_2);
	/*if(eState==BS_UP)
		SetOneButtonFocused(IDC_BTN_SETUP_2);
	else if(eState==BS_FOCUSED)
		SetOneButtonDepressed(IDC_BTN_SETUP_2);
	else if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_2);*/

	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_2);
	else
		SetOneButtonDepressed(IDC_BTN_SETUP_2);
}
// **************************************************************************
// 
// **************************************************************************
void CSubViewLanguage::OnBnClickedLang3()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_3);
	/*if(eState==BS_UP)
		SetOneButtonFocused(IDC_BTN_SETUP_3);
	else if(eState==BS_FOCUSED)
		SetOneButtonDepressed(IDC_BTN_SETUP_3);
	else if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_3);*/

	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_3);
	else
		SetOneButtonDepressed(IDC_BTN_SETUP_3);
}
// **************************************************************************
// 
// **************************************************************************
void CSubViewLanguage::OnBnClickedLang4()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_4);
	/*if(eState==BS_UP)
		SetOneButtonFocused(IDC_BTN_SETUP_4);
	else if(eState==BS_FOCUSED)
		SetOneButtonDepressed(IDC_BTN_SETUP_4);
	else if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_4);*/

	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_4);
	else
		SetOneButtonDepressed(IDC_BTN_SETUP_4);
}
// **************************************************************************
// 
// **************************************************************************
void CSubViewLanguage::OnBnClickedLang5()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_5);
	/*if(eState==BS_UP)
		SetOneButtonFocused(IDC_BTN_SETUP_5);
	else if(eState==BS_FOCUSED)
		SetOneButtonDepressed(IDC_BTN_SETUP_5);
	else if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_5);*/

	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_5);
	else
		SetOneButtonDepressed(IDC_BTN_SETUP_5);
}
// **************************************************************************
// 
// **************************************************************************
void CSubViewLanguage::OnBnClickedLang6()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_6);
	/*if(eState==BS_UP)
		SetOneButtonFocused(IDC_BTN_SETUP_6);
	else if(eState==BS_FOCUSED)
		SetOneButtonDepressed(IDC_BTN_SETUP_6);
	else if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_6);*/

	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_6);
	else
		SetOneButtonDepressed(IDC_BTN_SETUP_6);
}
// **************************************************************************
// 
// **************************************************************************
void CSubViewLanguage::OnBnClickedLang7()
{
	eBtnState eState = GetBtnState(IDC_BTN_SETUP_7);
	/*if(eState==BS_UP)
		SetOneButtonFocused(IDC_BTN_SETUP_7);
	else if(eState==BS_FOCUSED)
		SetOneButtonDepressed(IDC_BTN_SETUP_7);
	else if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_7);*/

	if(eState==BS_DOWN)
		SetOneButtonFocused(IDC_BTN_SETUP_7);
	else
		SetOneButtonDepressed(IDC_BTN_SETUP_7);
}

void CSubViewLanguage::OnBnClickedNextUp()
{
	eBtnState eState = GetCurrentBtnState();

	if(eState==BS_FOCUSED || eState==BS_DOWN)
		SetPrevButtonFocused();
	else
		SetOneButtonFocused(IDC_BTN_SETUP_1);



	
}
void CSubViewLanguage::OnBnClickedNextDw()
{
	eBtnState eState = GetCurrentBtnState();

	if(eState==BS_FOCUSED || eState==BS_DOWN)
		SetNextButtonFocused();
	else
		SetOneButtonFocused(IDC_BTN_SETUP_1);
}

// **************************************************************************
// 
// **************************************************************************
eBtnState CSubViewLanguage::GetCurrentBtnState()
{
	POSITION pos;

	pos = m_plUsedLangBtn.GetHeadPosition();

	do {
		CSelectSetupBtn* pBtn  = m_plUsedLangBtn.GetNext(pos);

		//int iT = pBtn->GetBtnId();

		if(m_iCurPosInBlock == pBtn->GetBtnId())
		{
			return pBtn->GetState();
		}


	} while (pos != NULL);

	return BS_NONE;
}

// **************************************************************************
// 
// **************************************************************************
eBtnState CSubViewLanguage::GetBtnState(int iID)
{
	POSITION pos;

	pos = m_plUsedLangBtn.GetHeadPosition();

	do {
		CSelectSetupBtn* pBtn  = m_plUsedLangBtn.GetNext(pos);

		//int iT = pBtn->GetBtnId();

		if(iID == pBtn->GetBtnId())
		{
			return pBtn->GetState();
		}


	} while (pos != NULL);

	return BS_NONE;
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewLanguage::SetLanguage(int btnID)
{
	POSITION pos;

	pos = m_plUsedLangBtn.GetHeadPosition();

	do {
		CSelectSetupBtn* pBtn  = m_plUsedLangBtn.GetNext(pos);

		//int iT = pBtn->GetBtnId();

		if(btnID == pBtn->GetBtnId())
		{
			CStringW szTemp = pBtn->GetLangString();
			if(szTemp.IsEmpty()==0)
			{
				m_szLangToLoad=szTemp;

				SetSelectedPos(szTemp);

				CreateWndHourglass();
				ShowWndHourglass(true);

				StartLoadLanguageThread();
				eventLoadLanguage.SetEvent();

			}
		}


	} while (pos != NULL);
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewLanguage::ReorganizeBtns(bool forward)
{
	// evtl Häckchen setzen etc  !!!!
	size_t jCount=m_plLangBtn.GetCount();	//should be 7

	if(jCount!=MAXLANG)
		return;

	CStringW cs = _T("");

	//int iTemp = m_iCurBlockPos;

	//size_t jIndex=0;

	if(forward)
	{
		m_plUsedLangBtn.RemoveAll();
		m_iCurBlockPos++;
		size_t index=m_iCurBlockPos-MAXLANG;

		for(size_t i=0;i<MAXLANG;i++)
		{

			
			if(m_pcsLangNames && m_pcsLangNames->GetCount()>0)
				cs = m_pcsLangNames->GetAt(m_pcsLangNames->FindIndex(index));

			int iPos=cs.Find(_T('.'));
			CStringW csTemp=cs.Left(iPos);

			CSelectSetupBtn* btn = m_plLangBtn.GetAt(m_plLangBtn.FindIndex(i));

			if(btn!=NULL)
			{
				btn->LoadFlag(csTemp);
				m_plUsedLangBtn.AddTail(btn);
				btn->ShowWindow(SW_SHOW);
				btn->RefreshText(csTemp);
				btn->SetLangString(cs);

				if(getModel()->getCONFIG()->getLANGUAGE()==cs)
					btn->DrawOK(true);
				else
					btn->DrawOK(false);
			}


			index++;
		}
		SetOneButtonFocused(IDC_BTN_SETUP_7);
	
	}
	else
	{
		m_plUsedLangBtn.RemoveAll();
		m_iCurBlockPos--;

		size_t index=m_iCurBlockPos-MAXLANG;

		for(size_t i=0;i<MAXLANG;i++)
		{
			if(m_pcsLangNames && m_pcsLangNames->GetCount()>0)
				cs = m_pcsLangNames->GetAt(m_pcsLangNames->FindIndex(index));

			int iPos=cs.Find(_T('.'));
			CStringW csTemp=cs.Left(iPos);

			CSelectSetupBtn* btn = m_plLangBtn.GetAt(m_plLangBtn.FindIndex(i));

			if(btn!=NULL)
			{
				btn->LoadFlag(csTemp);
				m_plUsedLangBtn.AddTail(btn);
				btn->ShowWindow(SW_SHOW);
				btn->RefreshText(csTemp);
				btn->SetLangString(cs);

				if(getModel()->getCONFIG()->getLANGUAGE()==cs)
					btn->DrawOK(true);
				else
					btn->DrawOK(false);
			}


			index++;
		}
		SetOneButtonFocused(IDC_BTN_SETUP_1);
	}
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewLanguage::StartLoadLanguageThread( void )
{
	m_bDoLoadLanguageThread=true;
	//m_pcwtLoadLanguageThread=AfxBeginThread(CLoadLanguageThread,this);

	if(m_pcwtLoadLanguageThread!=NULL)
	{
		delete m_pcwtLoadLanguageThread;
		m_pcwtLoadLanguageThread=NULL;

		if(m_hThreadLoadLanguage!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadLoadLanguage);
			m_hThreadLoadLanguage=INVALID_HANDLE_VALUE;
		}
	}

	m_pcwtLoadLanguageThread=AfxBeginThread(CLoadLanguageThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_hThreadLoadLanguage=m_pcwtLoadLanguageThread->m_hThread;
	m_pcwtLoadLanguageThread->m_bAutoDelete = FALSE; 
	m_pcwtLoadLanguageThread->ResumeThread();
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewLanguage::StopLoadLanguageThread( void )
{
	if(m_bDoLoadLanguageThread)
	{
		m_bDoLoadLanguageThread=false;
		eventLoadLanguage.SetEvent();

		if (WaitForSingleObject(m_pcwtLoadLanguageThread->m_hThread,3000) == WAIT_TIMEOUT)
		{
			theApp.getLog()->WriteLine(_T("#THR:029a"));
			if(!TerminateThread(m_pcwtLoadLanguageThread,0))
			{
				theApp.getLog()->WriteLine(_T("#THR:029b"));
				/*int err = GetLastError();
				CStringW temp;
				temp.Format(L"TerminateThread error ConnectionThread [%d]",err);*/
			}
			/*m_pcwtLoadLanguageThread=NULL;*/
		}
	}
	
}
// **************************************************************************
// 
// **************************************************************************
static UINT CLoadLanguageThread( LPVOID pc )
{
	try
	{
		((CSubViewLanguage*)pc)->LoadLanguage();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CLoadLanguageThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CLoadLanguageThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}
	//((CSubViewLanguage*)pc)->LoadLanguage();
	return TRUE;
}
// **************************************************************************
// 
// **************************************************************************
DWORD CSubViewLanguage::LoadLanguage(void) 
{
	/*DWORD dwDiff = 0;
	DWORD dwStart=0;
	DWORD dwEnd=0;*/

	//while(m_bDoDeleteThread)
	{
		if (::WaitForSingleObject(eventLoadLanguage, INFINITE) == WAIT_OBJECT_0 && m_bDoLoadLanguageThread) 
		{
			//dwStart=GetTickCount();
			if(m_szLangToLoad!=_T(""))
			{
								

				DWORD dwResult = getModel()->getLANGUAGE()->LoadLang(m_szLangToLoad,true);
				Sleep(1);
				if(dwResult==100)
				{
					CStringW szLog = _T("#HFO:0214: ");
					szLog+=m_szLangToLoad;
					theApp.getLog()->WriteLine(szLog);
					getModel()->getCONFIG()->SetLanguage(LANGFILE_ENGLISH);

					WORD iIdNew=getModel()->getCONFIG()->GetLanguageIDfromName(m_szLangToLoad);
					getModel()->SetLanguageID(iIdNew);
				}
				else
				{
					getModel()->getCONFIG()->SetLanguage(m_szLangToLoad);

					WORD iIdNew=getModel()->getCONFIG()->GetLanguageIDfromName(m_szLangToLoad);
					getModel()->SetLanguageID(iIdNew);
				}
				
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_LANGUAGE_CHANGED);

				m_szLangToLoad=_T("");
			}
			
			/*dwEnd=GetTickCount();
			dwDiff=dwEnd-dwStart;

			if(1000<dwDiff<2000)
			{
				WORD iCnt=0;
				while(iCnt<5 && m_bDoLoadLanguageThread && !m_bExit)
				{
					Sleep(200);
					iCnt++;
				}
			}*/

			Sleep(200);

		}
		m_bDoLoadLanguageThread=false;
		
		if(!m_bExit)
		{
			ShowWndHourglass(false);
			Draw();
		}
		
		//DestroyWndHourglass();
	}

	//theApp.getLog()->WriteLine(_T("#THR:029"));

	return 0;
}


// **************************************************************************
// 
// **************************************************************************
bool CSubViewLanguage::CreateWndHourglass()
{
	if(m_pWndHourglass==NULL)
	{
		m_pWndHourglass = new CWndWaitHourglass();

		RECT rcLd={250,180,550,380};
		if(m_pWndHourglass->Create(this,rcLd,IDC_VIEW_HOURGLASS,getModel()->GetLanguageString(IDS_TXT_LOADLANG)))
		{
			return true;
		}
	}
	return false;
}
bool CSubViewLanguage::DestroyWndHourglass()
{
	if(m_pWndHourglass)
	{
		//m_pWndHourglass->StopTimer();
		m_pWndHourglass->DestroyWindow();
		delete m_pWndHourglass;
		m_pWndHourglass=NULL;
	}
	return false;
}

void CSubViewLanguage::ShowWndHourglass(bool bShow)
{
	if(m_pWndHourglass)
	{
		if(bShow)
		{
			m_pWndHourglass->SetMessage(getModel()->GetLanguageString(IDS_TXT_LOADLANG));
			//m_pWndHourglass->StartTimer();
		}
		/*else
			m_pWndHourglass->StopTimer();*/
		
		m_pWndHourglass->Show(bShow);
	}
}

// **************************************************************************
// 
// **************************************************************************
void CSubViewLanguage::NotifyLanguageChanged()
{
	try
	{
		m_pbtnLanguage1->ReplaceFont(g_hf21AcuBold);
		m_pbtnLanguage2->ReplaceFont(g_hf21AcuBold);
		m_pbtnLanguage3->ReplaceFont(g_hf21AcuBold);
		m_pbtnLanguage4->ReplaceFont(g_hf21AcuBold);
		m_pbtnLanguage5->ReplaceFont(g_hf21AcuBold);
		m_pbtnLanguage6->ReplaceFont(g_hf21AcuBold);
		m_pbtnLanguage7->ReplaceFont(g_hf21AcuBold);
	}
	catch (...)
	{
		theApp.ReportException(_T("CSubViewLanguage::NotifyLanguageChanged"));
	}

}