// ParaBtn_ManBreath.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ParaBtn_ManBreath.h"
#include "globDefs.h"

//global font objects
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf31AcuBold;

/**********************************************************************************************//**
 * CParaBtn_ManBreath
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CParaBtn_ManBreath, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CParaBtn_ManBreath class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	btn		   	The button.
 * \param	nOffset	   	The offset.
 * \param	bScrollOver	True to scroll over.
 **************************************************************************************************/

CParaBtn_ManBreath::CParaBtn_ManBreath(BTN btn, int nOffset,bool bScrollOver):
CParaBtn(btn,nOffset,bScrollOver)
{

}

/**********************************************************************************************//**
 * Finalizes an instance of the CParaBtn_ManBreath class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CParaBtn_ManBreath::~CParaBtn_ManBreath()
{
}


BEGIN_MESSAGE_MAP(CParaBtn_ManBreath, CButton)
	//{{AFX_MSG_MAP(CParaBtn_PCurve)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CParaBtn_ManBreath message handlers
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	nState	The state.
 **************************************************************************************************/

void CParaBtn_ManBreath::Draw(int nState)
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

			TCHAR psz[MAX_PATH];

			wsprintf(psz,_T("%d"),m_v.iValue);

			//test
			//int iT = m_btn.wID;
			if(m_v.iValue==0)
				DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_OFF),-1,&rc,m_btn.dwFormat);
			else
				DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat);

			//RECT rcName;
			CRect rcName;


			rcName.top = m_rcClient.top+3;
			rcName.bottom = 27;
			rcName.left = m_rcClient.left+1;
			rcName.right = m_rcClient.right;

			SelectObject(m_hDC,g_hf7AcuBold);

			/*DrawText(m_hDC,_T("Max. time"),-1,&rcName,DT_TOP|DT_SINGLELINE|DT_CENTER);
			DrawText(m_hDC,_T("manual breath"),-1,&rcName,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);*/
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_MANBREATHTIME_TOP),-1,&rcName,DT_TOP|DT_SINGLELINE|DT_CENTER);
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_MANBREATHTIME_BOT),-1,&rcName,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

			//DrawText(m_hDC,m_pszNameText,-1,&rcName,m_btn.dwFormat);


			RECT rcUnit;
			rcUnit.top = m_rcClient.top;
			rcUnit.bottom = m_rcClient.bottom-5;
			rcUnit.left = m_rcClient.left;
			rcUnit.right = m_rcClient.right;

			SelectObject(m_hDC,g_hf7AcuNorm);
			//SetTextColor(m_hDC,RGB(0,0,0));

			if(m_bEndOfRange)
				DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_ENDRANGE),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
			else
				DrawText(m_hDC,m_pszUnitText,-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

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

void CParaBtn_ManBreath::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//CParaBtn::OnKeyUp(nChar,nRepCnt,nFlags);
	//bool bLimitsOk=true;

	if( m_bLMouseButtonDown )
	{

		if(nChar==m_kDown)	
		{
			if(m_bDepressed)
			{
				if(m_v.iValue>m_v.iLowerLimit)
					m_v.iValue--;
				else if(m_bScrollOver)
					m_v.iValue=m_v.iUpperLimit;
				else
					m_bEndOfRange = true;

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
					m_v.iValue++;
				else if(m_bScrollOver)
					m_v.iValue=m_v.iLowerLimit;
				else
					m_bEndOfRange = true;


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


