// ParaBtn_LeakCompensation.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ParaBtn_LeakCompensation.h"
#include "globDefs.h"

//global font objects
extern HFONT g_hf7AcuBold;
extern HFONT g_hf11AcuBold;

/**********************************************************************************************//**
 * CParaBtn_LeakCompensation
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CParaBtn_LeakCompensation, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CParaBtn_LeakCompensation class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	btn		   	The button.
 * \param	nOffset	   	The offset.
 * \param	bScrollOver	True to scroll over.
 **************************************************************************************************/

CParaBtn_LeakCompensation::CParaBtn_LeakCompensation(BTN btn, int nOffset,bool bScrollOver):
CParaBtn(btn,nOffset,bScrollOver)
{

}

/**********************************************************************************************//**
 * Finalizes an instance of the CParaBtn_LeakCompensation class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CParaBtn_LeakCompensation::~CParaBtn_LeakCompensation()
{
}


BEGIN_MESSAGE_MAP(CParaBtn_LeakCompensation, CWnd)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CParaBtn_LeakCompensation message handlers
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	pParentWnd	If non-null, the parent window.
 * \param 		  	dwStyle   	The style.
 * \param 		  	v		  	A fVALUE to process.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CParaBtn_LeakCompensation::Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v) 
{
	SIZE sz;
	sz.cx=m_btn.pcBmpUp->Width();
	sz.cy=m_btn.pcBmpUp->Height();
	CRect cRect(m_btn.poPosition,sz);

	//if(CButton::Create(_T("btn"),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW|WS_TABSTOP,cRect,pParentWnd,m_btn.wID))
	if(CButton::Create(_T("btn"),dwStyle,cRect,pParentWnd,m_btn.wID))
	{
		CClientDC dc(this);

		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBm=CreateCompatibleBitmap(dc.m_hDC,m_btn.pcBmpUp->Width(),m_btn.pcBmpUp->Height());
		m_hBmPrev=(HBITMAP)SelectObject(m_hDC,m_hBm);


		m_rcClient.left=m_rcClient.top=0;
		m_rcClient.right=sz.cx;
		m_rcClient.bottom=sz.cy;

		memcpy(&m_v,&v,sizeof(fVALUE));

		return 1;
	}
	return 0;
}

/**********************************************************************************************//**
 * CParaBtn_PCurve message handlers
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	nState	The state.
 **************************************************************************************************/

void CParaBtn_LeakCompensation::Draw(int nState)
{
	if(!m_bDisableText)
	{
		//if(m_hf38Bold )
		{
			HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf31AcuBold);
			int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
			int nTxtColor;
			//int nTxtColorSubBtn;

			switch(nState)
			{
			case BTN_DW_FOCUS:
				nTxtColor=m_btncr.crTxtFocus;
				//nTxtColorSubBtn=m_btncr.crTxtFocus;
				break;
			case BTN_DW_UP:
				nTxtColor=m_btncr.crTxtUp;
				//nTxtColorSubBtn=m_btncr.crTxtUp;
				break;
			case BTN_DW_DW:
				nTxtColor=m_btncr.crTxtDown;
				//nTxtColorSubBtn=m_btncr.crSubTxtDown;
				break;
			case BTN_DW_DIS:
			default:
				nTxtColor=m_btncr.crTxtDisabled;
				//nTxtColorSubBtn=m_btncr.crTxtDisabled;
				break;
			}
			int nPrevTxtColor=SetTextColor(m_hDC,nTxtColor);

			RECT rc;
			memcpy(&rc,&m_rcClient,sizeof(RECT));

			rc.top = 5;
			//rc.bottom = 94;

			if(m_btn.dwFormat&DT_RIGHT)
				rc.right-=m_nOffset;
			else if(!(m_btn.dwFormat&DT_CENTER)) // left
				rc.left+=m_nOffset;

			//TCHAR psz[MAX_PATH];

			//wsprintf(psz,_T("%d"),m_v.iValue);
			

			//test
			//int iT = m_btn.wID;
			/*if(m_v.iValue==0)
				DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_OFF),-1,&rc,m_btn.dwFormat);
			else
				DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat);*/

			//RECT rcName;
			CRect rcName;


			//rcName.top = m_rcClient.top+3;
			rcName.bottom = m_rcClient.bottom;
			rcName.left = m_rcClient.left;
			rcName.right = m_rcClient.right;

			SelectObject(m_hDC,g_hf7AcuBold);

			
			rcName.top = m_rcClient.top+3;
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_LEAKCOMP_TOP),-1,&rcName,DT_TOP|DT_SINGLELINE|DT_CENTER);

			rcName.top = m_rcClient.top+15;
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_LEAKCOMP_MID),-1,&rcName,DT_TOP|DT_SINGLELINE|DT_CENTER);

			rcName.top = m_rcClient.top+27;
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_LEAKCOMP_BOT),-1,&rcName,DT_TOP|DT_SINGLELINE|DT_CENTER);

			SelectObject(m_hDC,g_hf11AcuBold);

			RECT rcUnit;
			rcUnit.top = m_rcClient.top+30;
			rcUnit.bottom = m_rcClient.bottom;
			rcUnit.left = m_rcClient.left;
			rcUnit.right = m_rcClient.right;

			switch((eLeakCompensation)m_v.iValue)
			{
			case LC_LOW:
				{
					DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_SENS_LOW),-1,&rcUnit,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
				}
				break;
			case LC_MIDDLE:
				{
					DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_SENS_MID),-1,&rcUnit,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
				}
				break;
			case LC_HIGH:
				{
					DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_SENS_HIGH),-1,&rcUnit,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
				}
				break;
			default:
				{

				}
				break;
			}

			

			
			m_bEndOfRange=false;

			SelectObject(m_hDC,hPrevFont);
			SetBkMode(m_hDC,nBkMode);
			SetTextColor(m_hDC,nPrevTxtColor);
		}
	}
}

/**********************************************************************************************//**
 * Executes the key up action
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	nChar  	The character.
 * \param	nRepCnt	Number of reps.
 * \param	nFlags 	The flags.
 **************************************************************************************************/

void CParaBtn_LeakCompensation::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//CParaBtn::OnKeyUp(nChar,nRepCnt,nFlags);
	//bool bLimitsOk=true;

	if( m_bLMouseButtonDown )
	{

		//if(nChar==32)	//VK_SPACE
		//{
		//	m_bDepressed = !m_bDepressed;
		//	SetFocus();
		//}
		//CButton::OnKeyUp(nChar,nRepCnt,nFlags);
		/*}
		else
		{*/
		if(nChar==m_kDown)	
		{
			if(m_bDepressed)
			{
				if(m_v.iValue>m_v.iLowerLimit)
				{
					m_v.iValue--;
				}

				if(m_bTextOnly)// notify parent by message
				{
				}
				else
				{
					CClientDC dc(this);
					m_btn.pcBmpDown->Draw(m_hDC);
					Draw(BTN_DW_DW);
					BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
				}
			}
			else
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_FOCUS_VKDOWN);
			}
		}
		else if(nChar==m_kUp)	
		{
			if(m_bDepressed)
			{
				if(m_v.iValue<m_v.iUpperLimit)
				{
					m_v.iValue++;
				}

				if(m_bTextOnly)// notify parent by message
				{
				}
				else
				{
					CClientDC dc(this);
					m_btn.pcBmpDown->Draw(m_hDC);
					Draw(BTN_DW_DW);
					BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
				}

				//WriteCurrentValue();
			}
			else
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_FOCUS_VKUP);
			}
		}
		CParaBtn::OnKeyUp(nChar,nRepCnt,nFlags);
	}
	m_bLMouseButtonDown = false;
}




