// SelectSetupBtn.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "SelectSetupBtn.h"
#include "FabianHFO.h"
#include "globDefs.h"

/**********************************************************************************************//**
 * CSelectSetupBtn
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CSelectSetupBtn, CButton)

/**********************************************************************************************//**
 * Initializes a new instance of the CSelectSetupBtn class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	btn   	The button.
 * \param 		  	cr	  	The carriage return.
 * \param 		  	bValue	True to value.
 **************************************************************************************************/

CSelectSetupBtn::CSelectSetupBtn(BTN &btn,COLORREF cr,bool bValue)
{
	// Init state
	m_pModel=NULL;

	m_pcOkFc=NULL;
	m_pcOkUp=NULL;
	m_pcOkDw=NULL;

	//m_pcKeyFc=NULL;
	//m_pcKeyUp=NULL;
	m_pcKeyDw=NULL;

	m_pcWarningFc=NULL;
	m_pcWarningUp=NULL;
	m_pcWarningDw=NULL;

	m_btnState=BS_UP;

	m_pbmpFlag=NULL;

	m_nXOffset=0;

	memcpy(&m_btn,&btn,sizeof(BTN));
	// Default colors
	m_btncr.crTxtUp			= cr;
	m_btncr.crTxtDown		= cr;
	m_btncr.crTxtDisabled	= 0x00c0c0c0;		
	m_btncr.crTxtFocus		= cr;	
	//
	m_hDC=NULL;
	m_hBm=NULL;
	m_hBmPrev=NULL;
	//
	m_pszText[0]=0x0000;
	m_pszValueText[0]=0x0000;
	//
	m_hFont=NULL;

	m_nNbr=-1;

	m_bDepressed=false;
	m_bLMouseButtonDown=false;
	m_bLButtonDown=false;

	m_kUp					= VK_UP;//up dw revers
	m_kDown					= VK_DOWN;
	m_kSpace				= VK_SPACE;

	m_bValue = bValue;
	m_bDrawOK=false;
	m_bDrawKey=false;
	m_bDrawWarning=false;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CSelectSetupBtn class
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

CSelectSetupBtn::~CSelectSetupBtn()
{
	delete m_pbmpFlag;
	m_pbmpFlag=NULL;
	delete m_pcOkFc;
	m_pcOkFc=NULL;
	delete m_pcOkUp;
	m_pcOkUp=NULL;
	delete m_pcOkDw;
	m_pcOkDw=NULL;

	//delete m_pcKeyFc;
	//delete m_pcKeyUp;
	delete m_pcKeyDw;

	//m_pcKeyFc=NULL;
	//m_pcKeyUp=NULL;
	m_pcKeyDw=NULL;

	delete m_pcWarningFc;
	delete m_pcWarningUp;
	delete m_pcWarningDw;

	m_pcWarningFc=NULL;
	m_pcWarningUp=NULL;
	m_pcWarningDw=NULL;

	if(m_hDC)
	{
		SelectObject(m_hDC,m_hBmPrev);
		DeleteObject(m_hBm);	 
		DeleteDC(m_hDC);
	}

	
}

/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CSelectSetupBtn::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

BEGIN_MESSAGE_MAP(CSelectSetupBtn, CButton)
	//{{AFX_MSG_MAP(CSelectSetupBtn)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CSelectSetupBtn-Meldungshandler
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pParentWnd	If non-null, the parent window.
 * \param 		  	hFont	  	The font.
 * \param 		  	nXOffset  	The x coordinate offset.
 * \param 		  	dwStyle   	The style.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CSelectSetupBtn::Create(CWnd* pParentWnd, HFONT hFont, int nXOffset, DWORD	dwStyle) 
{
	SIZE sz;
	sz.cx=m_btn.pcBmpUp->Width();
	sz.cy=m_btn.pcBmpUp->Height();
	CRect cRect(m_btn.poPosition,sz);

	//if(CButton::Create(_T("btn"),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW/*|WS_TABSTOP*/,cRect,pParentWnd,m_btn.wID))
	if(CButton::Create(_T("btn"),dwStyle,cRect,pParentWnd,m_btn.wID))
	{
		CClientDC dc(this);
		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBm=CreateCompatibleBitmap(dc.m_hDC,m_btn.pcBmpUp->Width(),m_btn.pcBmpUp->Height());
		m_hBmPrev=(HBITMAP)SelectObject(m_hDC,m_hBm);

		m_pcOkFc= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_OK_FC);
		m_pcOkUp= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_OK_UP);
		m_pcOkDw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_OK_DW);

		//m_pcKeyFc=NULL;
		//m_pcKeyUp=NULL;
		m_pcKeyDw	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PARA_KEY_DW);

		m_pcWarningFc= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PARA_WARN_FC);
		m_pcWarningUp= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PARA_WARN_UP);
		m_pcWarningDw= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PARA_WARN_DW);

		m_rcClient.left=m_rcClient.top=0;
		m_rcClient.right=sz.cx;
		m_rcClient.bottom=sz.cy;

		m_hFont=hFont;
		m_nXOffset=nXOffset;
		return 1;
	}
	return 0;
}

//bool CSelectSetupBtn::IsDepressed( void )
//{
//	// Return the buttons state
//	return m_bDepressed;
//}
//
//bool CSelectSetupBtn::Depress(bool bDown)
//{
//	// Set the buttons state
//	if( bDown != m_bDepressed )
//	{
//		m_bDepressed = bDown;
//		Invalidate(false);
//		return !m_bDepressed;
//	}
//	return m_bDepressed;
//}

/**********************************************************************************************//**
 * Loads a flag
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	csFlag	The create struct flag.
 **************************************************************************************************/

void CSelectSetupBtn::LoadFlag(CStringW csFlag)
{
	CStringW cs;
	cs.LoadString(IDS_LANGPATH);

	WIN32_FIND_DATA fd;
	CStringW csSearch=cs+csFlag;
	csSearch+=_T(".bmp");
	HANDLE hFind=FindFirstFile(csSearch,&fd);
	if(hFind==INVALID_HANDLE_VALUE)
	{
		
		csSearch=cs;
		csSearch+=_T("dummy.bmp");
		hFind=FindFirstFile(csSearch,&fd);
	}
	/*else
	{
		CStringW szTest=fd.cFileName;
	}*/

	if(m_pbmpFlag)
		delete m_pbmpFlag;

	m_pbmpFlag = new CBmp(m_hDC,csSearch);

	if(hFind!=INVALID_HANDLE_VALUE)
		FindClose(hFind);
}

/**********************************************************************************************//**
 * Draw ok
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CSelectSetupBtn::DrawOK(bool state)
{
	bool bRedraw=false;

	if(state!=m_bDrawOK)
		bRedraw = true;

	m_bDrawOK=state;

	if(bRedraw)
		Redraw();
}

/**********************************************************************************************//**
 * Draw key
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CSelectSetupBtn::DrawKey(bool state)
{
	bool bRedraw=false;

	if(state!=m_bDrawKey)
		bRedraw = true;

	m_bDrawKey=state;

	if(bRedraw)
		Redraw();
}

/**********************************************************************************************//**
 * Draw warning
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	state	True to state.
 **************************************************************************************************/

void CSelectSetupBtn::DrawWarning(bool state)
{
	bool bRedraw=false;

	if(state!=m_bDrawWarning)
		bRedraw = true;

	m_bDrawWarning=state;

	if(bRedraw)
		Redraw();
}

/**********************************************************************************************//**
 * Redraws this instance
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CSelectSetupBtn::Redraw() 
{
	Invalidate();
	UpdateWindow();
}

/**********************************************************************************************//**
 * Draw direct up
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CSelectSetupBtn::DrawDirectUp()
{
	CClientDC dc(this);

	m_btnState=BS_UP;
	m_bDepressed=false;

	m_btn.pcBmpUp->Draw(m_hDC);
	Draw(BTN_DW_UP);
	BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
}

/**********************************************************************************************//**
 * Draw direct down
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 **************************************************************************************************/

void CSelectSetupBtn::DrawDirectDown()
{

	CClientDC dc(this);

	m_btnState=BS_DOWN;
	m_bDepressed=true;

	//SetFocus();

	m_btn.pcBmpUp->Draw(m_hDC);
	Draw(BTN_DW_DW);
	BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
}

/**********************************************************************************************//**
 * Replace font
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	hFont	The font.
 **************************************************************************************************/

void CSelectSetupBtn::ReplaceFont(HFONT hFont) 
{
	m_hFont=hFont;
	Invalidate();
	UpdateWindow();
}

/**********************************************************************************************//**
 * Sets a text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pszText	If non-null, the text.
 * \param 		  	nNbr   	Number of.
 **************************************************************************************************/

void CSelectSetupBtn::SetText(TCHAR* pszText, int nNbr) 
{
	_tcscpy_s(m_pszText,_countof(m_pszText),pszText);
	m_nNbr=nNbr;
}

/**********************************************************************************************//**
 * Sets a text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	szText	The text.
 * \param 		  	nNbr  	Number of.
 **************************************************************************************************/

void CSelectSetupBtn::SetText(const CStringW& szText, int nNbr) 
{
	_tcscpy_s(m_pszText,_countof(m_pszText),szText);
	m_nNbr=nNbr;
}

/**********************************************************************************************//**
 * Sets value text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pszText	If non-null, the text.
 * \param 		  	nNbr   	Number of.
 **************************************************************************************************/

void CSelectSetupBtn::SetValueText(TCHAR* pszText, int nNbr) 
{
	_tcscpy_s(m_pszValueText,_countof(m_pszValueText),pszText);
	m_nNbr=nNbr;
}

/**********************************************************************************************//**
 * Sets value text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	szText	The text.
 * \param 		  	nNbr  	Number of.
 **************************************************************************************************/

void CSelectSetupBtn::SetValueText(const CStringW& szText, int nNbr) 
{
	_tcscpy_s(m_pszValueText,_countof(m_pszValueText),szText);
	m_nNbr=nNbr;
}

/**********************************************************************************************//**
 * Refresh text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	szText	The text.
 * \param 		  	nNbr  	Number of.
 **************************************************************************************************/

void CSelectSetupBtn::RefreshText(const CStringW& szText, int nNbr) 
{
	SetText(szText,nNbr);
	Invalidate();
	UpdateWindow();
} 

/**********************************************************************************************//**
 * Refresh text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pszText	If non-null, the text.
 * \param 		  	nNbr   	Number of.
 **************************************************************************************************/

void CSelectSetupBtn::RefreshText(TCHAR* pszText, int nNbr) 
{
	SetText(pszText,nNbr);
	Invalidate();
	UpdateWindow();
} 

/**********************************************************************************************//**
 * Refresh value text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	szText	The text.
 * \param 		  	nNbr  	Number of.
 **************************************************************************************************/

void CSelectSetupBtn::RefreshValueText(const CStringW& szText, int nNbr) 
{
	SetValueText(szText,nNbr);
	Invalidate();
	UpdateWindow();
} 

/**********************************************************************************************//**
 * Refresh value text
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pszText	If non-null, the text.
 * \param 		  	nNbr   	Number of.
 **************************************************************************************************/

void CSelectSetupBtn::RefreshValueText(TCHAR* pszText, int nNbr) 
{
	SetValueText(pszText,nNbr);
	Invalidate();
	UpdateWindow();
} 

/**********************************************************************************************//**
 * Sets a character
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	t	A TCHAR to process.
 **************************************************************************************************/

void CSelectSetupBtn::SetChar(TCHAR t) 
{
	m_pszText[0]=t;
	m_pszText[1]=0x0000;
}

/**********************************************************************************************//**
 * Draw item
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	lpDrawItemStruct	The draw item structure.
 **************************************************************************************************/

void CSelectSetupBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (lpDrawItemStruct->itemState & ODS_DISABLED)
	{
		m_btn.pcBmpDisabled->Draw(m_hDC);
		Draw(BTN_DW_UP);
		//Draw(BTN_DW_DIS);
		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	}
	else if(m_btnState==BS_UP)
	{
		m_btn.pcBmpUp->Draw(m_hDC);
		Draw(BTN_DW_UP);
		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	}
	else if(m_btnState==BS_FOCUSED)
	{
		m_btn.pcBmpFocus->Draw(m_hDC);
		Draw(BTN_DW_FOCUS);
		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	}
	else
	{
		m_btn.pcBmpDown->Draw(m_hDC);
		Draw(BTN_DW_DW);
		BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
	}


}

/**********************************************************************************************//**
 * Draws
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nState	The state.
 **************************************************************************************************/

void CSelectSetupBtn::Draw(int nState/*,TCHAR* psz*/)
{
	if(m_hFont)
	{
		HFONT hPrevFont=(HFONT)SelectObject(m_hDC,m_hFont);
		int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
		int nTxtColor;

		switch(nState)
		{
		case BTN_DW_FOCUS:
			nTxtColor=m_btncr.crTxtFocus;
			break;
		case BTN_DW_UP:
			nTxtColor=m_btncr.crTxtUp;
			break;
		case BTN_DW_DW:
			nTxtColor=m_btncr.crTxtDown;
			/*if(m_btn.wID==IDC_BTN_MENU_NUMERIC)
			{
			m_pViewHandler->BtnPushed(m_btn.wID,0);
			}*/
			break;
		case BTN_DW_DIS:
		default:
			nTxtColor=m_btncr.crTxtDisabled;
			break;
		}

		int nPrevTxtColor=SetTextColor(m_hDC,nTxtColor);

		RECT rc;
		memcpy(&rc,&m_rcClient,sizeof(RECT));

		/*rc.left = rc.left+20;
		rc.right = rc.right-20;*/

		if(m_btn.dwFormat&DT_RIGHT)
			rc.right-=m_nXOffset;
		else //if(!(m_btn.dwFormat&DT_CENTER)) // left
			rc.left+=m_nXOffset;

				


		if(m_bValue)
		{
			rc.left = rc.left+10;
			rc.right = rc.right-20;

			DrawText(m_hDC,m_pszText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

			SetTextColor(m_hDC,RGB(0,0,130));
			DrawText(m_hDC,m_pszValueText,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_RIGHT);
		}
		else
		{
			DrawText(m_hDC,m_pszText,-1,&rc,m_btn.dwFormat);
		}

		

		

		if(m_nNbr!=-1)
		{
			rc.left=0;
			rc.right=m_btn.pcBmpUp->Height(); // quaudrat!
			TCHAR pszNbr[8];
			wsprintf(pszNbr,_T("%d"),m_nNbr);
			DrawText(m_hDC,pszNbr,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_CENTER);	
		}


		if(m_bDrawOK)
		{
			CBmp* pcOK=NULL;

			switch(GetState())
			{
			case BS_UP:
				{
					pcOK=m_pcOkUp;
				}
				break;
			case BS_FOCUSED:
				{
					pcOK=m_pcOkFc;
				}
				break;
			case BS_DOWN:
				{
					pcOK=m_pcOkDw;
				}
				break;
			}

			if(pcOK)
				pcOK->Draw(m_hDC,20,2);
		}

		if(m_bDrawKey)
		{
			CBmp* pcKey=NULL;

			switch(GetState())
			{
			/*case BS_UP:
				{
					pcKey=m_pcKeyUp;
				}
				break;
			case BS_FOCUSED:
				{
					pcKey=m_pcKeyFc;
				}
				break;*/
			case BS_DOWN:
				{
					pcKey=m_pcKeyDw;
				}
				break;
			}

			if(pcKey)
				pcKey->Draw(m_hDC,5,40);
		}

		if(m_bDrawWarning)
		{
			CBmp* pcWarning=NULL;

			switch(GetState())
			{
			case BS_UP:
				{
					pcWarning=m_pcWarningUp;
				}
				break;
			case BS_FOCUSED:
				{
					pcWarning=m_pcWarningFc;
				}
				break;
			case BS_DOWN:
				{
					pcWarning=m_pcWarningDw;
				}
				break;
			}

			if(pcWarning)
				pcWarning->Draw(m_hDC,80,40);
		}



		if(m_pbmpFlag!=NULL)
		{
			m_pbmpFlag->Draw(m_hDC,350,7);
		}

		SetTextColor(m_hDC,nPrevTxtColor);
		SetBkMode(m_hDC,nBkMode);
		SelectObject(m_hDC,hPrevFont);
	}
}

/**********************************************************************************************//**
 * Sets the bitmaps
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pcBmpUp		 	If non-null, the PC bitmap up.
 * \param [in,out]	pcBmpDown	 	If non-null, the PC bitmap down.
 * \param [in,out]	pcBmpDisabled	If non-null, the PC bitmap disabled.
 * \param [in,out]	pcBmpFocus   	If non-null, the PC bitmap focus.
 **************************************************************************************************/

void CSelectSetupBtn::SetBitmaps(CBmp* pcBmpUp,CBmp* pcBmpDown,CBmp* pcBmpDisabled,CBmp* pcBmpFocus)
{
	if(m_btn.pcBmpUp!=pcBmpUp)
	{
		m_btn.pcBmpUp=pcBmpUp;
		m_btn.pcBmpDown=pcBmpDown;
		m_btn.pcBmpDisabled=pcBmpDisabled;
		m_btn.pcBmpFocus=pcBmpFocus;
		Invalidate();
		UpdateWindow();
	}
}

/**********************************************************************************************//**
 * Executes the l button double clock action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	nFlags	The flags.
 * \param	point 	The point.
 **************************************************************************************************/

void CSelectSetupBtn::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	DWORD lParam;
	WORD  po[2]={(WORD)point.x,(WORD)point.y};
	memcpy(&lParam,po,sizeof(DWORD));

	SendMessage(WM_LBUTTONDOWN,nFlags,lParam);
	CButton::OnLButtonDblClk(nFlags, point);
}

/**********************************************************************************************//**
 * Gets button identifier
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The button identifier.
 **************************************************************************************************/

int CSelectSetupBtn::GetBtnId()
{
	return m_btn.wID;
}

/**********************************************************************************************//**
 * Sets a state
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CSelectSetupBtn::SetState(eBtnState state)
{
	if(m_btnState != state)
	{
		m_btnState = state;
		if(state==BS_DOWN)
			m_bDepressed=true;
		else
			m_bDepressed=false;
		Invalidate(false);
		UpdateWindow();
	}
	
}

/**********************************************************************************************//**
 * Gets the state
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The state.
 **************************************************************************************************/

eBtnState CSelectSetupBtn::GetState()
{
	return m_btnState;
}

/**********************************************************************************************//**
 * Translates all messages before they are processed by the main message loop
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pMsg	If non-null, the message.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CSelectSetupBtn::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_LBUTTONDOWN:
		{
			if(GetParent())
				GetParent()->PostMessage(WM_MENUBUTTONDOWN,m_btn.wID,0);
		}
		break;
	case WM_KEYDOWN:
		if(pMsg->wParam==VK_SPACE)
		{
			if(GetParent())
				GetParent()->PostMessage(WM_MENUBUTTONUP,m_btn.wID,0);
			//return 1;
		}
		else if(pMsg->wParam==VK_UP)
		{
			if(GetParent())
				GetParent()->PostMessage(WM_MENUBUTTON_VKUP,m_btn.wID,0);
		}
		else if(pMsg->wParam==VK_DOWN)
		{
			if(GetParent())
				GetParent()->PostMessage(WM_MENUBUTTON_VKDOWN,m_btn.wID,0);
		}
		else
		{
			//handle focus change manual, draw next focused button and return true
			//return 1;
		}
		break;
	default:
		{

		}
		break;
	}
	return CButton::PreTranslateMessage(pMsg);
}

/**********************************************************************************************//**
 * Sets language string
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param	szLang	The language.
 **************************************************************************************************/

void CSelectSetupBtn::SetLangString(CStringW szLang)
{
	m_szLanguageString=szLang;
}

/**********************************************************************************************//**
 * Gets language string
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \return	The language string.
 **************************************************************************************************/

CStringW CSelectSetupBtn::GetLangString()
{
	return m_szLanguageString;
}

/**********************************************************************************************//**
 * Executes the set focus action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pOldWnd	If non-null, the old window.
 **************************************************************************************************/

void CSelectSetupBtn::OnSetFocus(CWnd* pOldWnd)
{
	CButton::OnSetFocus(pOldWnd);

	if(GetDlgCtrlID()==IDC_BTN_ALIMIT_DELAY)
	{
		m_btnState=BS_FOCUSED;
		GetParent()->PostMessage( WM_ALARMBTN_SETFOCUS, GetDlgCtrlID(), 0);
		Redraw();
	}
}

/**********************************************************************************************//**
 * Executes the kill focus action
 *
 * \author	Rainer Kühner
 * \date	26.02.2018
 *
 * \param [in,out]	pNewWnd	If non-null, the new window.
 **************************************************************************************************/

void CSelectSetupBtn::OnKillFocus(CWnd* pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);

	if(GetDlgCtrlID()==IDC_BTN_ALIMIT_DELAY)
	{
		m_btnState=BS_UP;
		Redraw();
		
	}
}