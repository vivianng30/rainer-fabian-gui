// ParaBtn_HFFLOW.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ParaBtn_HFFLOW.h"
#include "globDefs.h"

extern HFONT g_hf7AcuNorm;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf8AcuBold;
extern HFONT g_hf23AcuBold;
extern HFONT g_hf17AcuBold;

/**********************************************************************************************//**
 * CParaBtn_HFFLOW
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	btn		   	The button.
 * \param	nOffset	   	The offset.
 * \param	bScrollOver	True to scroll over.
 **************************************************************************************************/

CParaBtn_HFFLOW::CParaBtn_HFFLOW(BTN btn, int nOffset,bool bScrollOver):
CParaBtn(btn,nOffset,bScrollOver)
{
	//m_bOff=true;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CParaBtn_HFFLOW class
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 **************************************************************************************************/

CParaBtn_HFFLOW::~CParaBtn_HFFLOW(void)
{
}


BEGIN_MESSAGE_MAP(CParaBtn_HFFLOW, CButton)
	//{{AFX_MSG_MAP(CParaBtn_VLIMIT)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	/*ON_WM_SETFOCUS()*/
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CParaBtn_HFFLOW message handlers
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	nState	The state.
 **************************************************************************************************/

void CParaBtn_HFFLOW::Draw(int nState)
{
	if(!m_bDisableText)
	{
		//if(g_hf31AcuBold )
		{
			HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf31AcuBoldNum);
			int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
			int nTxtColor;
			int nTxtColorDefault;
			//int nTxtColorSubBtn;

			

			switch(nState)
			{
			case BTN_DW_FOCUS:
				nTxtColor=m_btncr.crTxtFocus;
				nTxtColorDefault=m_btncr.crTxtFocus;
				//nTxtColorSubBtn=m_btncr.crTxtFocus;
				break;
			case BTN_DW_UP:
				nTxtColor=m_btncr.crTxtUp;
				nTxtColorDefault=RGB(0,0,255);
				//nTxtColorSubBtn=m_btncr.crTxtUp;
				break;
			case BTN_DW_DW:
				nTxtColor=m_btncr.crTxtDown;
				nTxtColorDefault=m_btncr.crTxtDown;
				//nTxtColorSubBtn=m_btncr.crSubTxtDown;
				break;
			case BTN_DW_DIS:
			default:
				nTxtColor=m_btncr.crTxtDisabled;
				nTxtColorDefault=m_btncr.crTxtDisabled;
				//nTxtColorSubBtn=m_btncr.crTxtDisabled;
				break;
			}
			int nPrevTxtColor=SetTextColor(m_hDC,nTxtColor);

			RECT rc;
			memcpy(&rc,&m_rcClient,sizeof(RECT));

			rc.top = 5;

			if(m_btn.dwFormat&DT_RIGHT)
				rc.right-=m_nOffset;
			else if(!(m_btn.dwFormat&DT_CENTER)) // left
				rc.left+=m_nOffset;

			TCHAR psz[MAX_PATH];

			/*if(m_v.iValue==0)
			{
				SelectObject(m_hDC,g_hf17AcuBold);
				wsprintf(psz,_T("%s"),getModel()->GetLanguageString(IDS_TXT_DEFAULT));
			}
			else*/
			{
				wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)m_v.iValue)/1000, 0));
			}
			

			//test
			//int iT = m_btn.wID;
			DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat);

			//RECT rcName;
			CRect rcName;

			rcName.top = m_rcClient.top;
			rcName.bottom = 30;
			rcName.left = m_rcClient.left;
			//rcName.right = m_rcClient.right/2-5;
			rcName.right = m_rcClient.right;

			SelectObject(m_hDC,g_hf14AcuBold);

			DrawText(m_hDC,m_pszNameText,-1,&rcName,m_btn.dwFormat);


			/*SelectObject(m_hDC,g_hf14AcuBold);

			CSize TextSize1 = pDC->GetTextExtent(m_pszNameText);

			SelectObject(m_hDC,g_hf8AcuBold);

			CSize TextSize2 = pDC->GetTextExtent(m_pszNameNoteText);

			SelectObject(m_hDC,g_hf14AcuBold);

			int iDiff=(m_rcClient.right-TextSize1.cx-TextSize2.cx)/2;

			rcName.top = m_rcClient.top;
			rcName.bottom = 30;
			rcName.left = m_rcClient.left;
			rcName.right = rcName.left+iDiff+TextSize1.cx;

			DrawText(m_hDC,m_pszNameText,-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_RIGHT);

			SelectObject(m_hDC,g_hf8AcuBold);
			rcName.bottom = 35;
			rcName.left = rcName.right;
			rcName.right = m_rcClient.right;
			DrawText(m_hDC,m_pszNameNoteText,-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_LEFT);*/

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
			{
				if(m_v.iValue==8000)
				{
					rcUnit.top = m_rcClient.top;
					rcUnit.bottom = m_rcClient.bottom-12;

					SelectObject(m_hDC,g_hf7AcuBold);
					SetTextColor(m_hDC,nTxtColorDefault);

					//DrawText(m_hDC,m_pszUnitText,-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
					DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_DEFAULT),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
					
					SelectObject(m_hDC,g_hf7AcuNorm);
					SetTextColor(m_hDC,nTxtColor);

					rcUnit.top = m_rcClient.top;
					rcUnit.bottom = m_rcClient.bottom;
					DrawText(m_hDC,m_pszUnitText,-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
					//DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_DEFAULT),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
				}
				else
				{
					DrawText(m_hDC,m_pszUnitText,-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
				}
				

				if(m_bDrawWarning && m_bHasFocus /*&& !m_bAutoState*/)
				{
					if(m_pcWarning_Fc)
						m_pcWarning_Fc->Draw(m_hDC,60,65);
				}
				else if(m_bDrawWarning && m_bDepressed /*&& !m_bAutoState*/)
				{
					if(m_pcWarning_Dw)
						m_pcWarning_Dw->Draw(m_hDC,60,65);
				}
				else if(m_bDrawWarning && m_bSignaled /*&& !m_bAutoState*/)
				{
					if(m_pcWarning_Red)
						m_pcWarning_Red->Draw(m_hDC,60,65);
				}
				else if(m_bDrawWarning /*&& !m_bAutoState*/)
				{
					if(m_pcWarning_Up)
						m_pcWarning_Up->Draw(m_hDC,60,65);
				}

				if(m_bDrawKey && m_bDepressed /*&& !m_bAutoState*/)
				{
					//if(m_bDepressed)
					{
						if(m_pcKey)
							m_pcKey->Draw(m_hDC,10,65);
					}
				}
			}

			m_bEndOfRange=false;

			SetTextColor(m_hDC,nPrevTxtColor);
			SetBkMode(m_hDC,nBkMode);
			SelectObject(m_hDC,hPrevFont);
		}
	}
}

/**********************************************************************************************//**
 * Executes the key up action
 *
 * \author	Rainer K�hner
 * \date	22.02.2018
 *
 * \param	nChar  	The character.
 * \param	nRepCnt	Number of reps.
 * \param	nFlags 	The flags.
 **************************************************************************************************/

void CParaBtn_HFFLOW::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//CParaBtn::OnKeyUp(nChar,nRepCnt,nFlags);
	if( m_bLMouseButtonDown )
	{
		if(nChar==m_kDown)	
		{
			if(m_bDepressed)
			{
				if(m_v.iValue>m_v.iLowerLimit)
				{
					m_v.iValue=m_v.iValue-1000;


				}
				else if(m_bScrollOver)
					m_v.iValue=m_v.iUpperLimit;
				else
					m_bEndOfRange = true;
				
				
				CClientDC dc(this);
				m_btn.pcBmpDown->Draw(m_hDC);
				Draw(BTN_DW_DW);
				BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);

			}
			else
			{
				if(AfxGetApp()->GetMainWnd())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_FOCUS_VKDOWN);
			}
		}
		else if(nChar==m_kUp)	
		{
			if(m_bDepressed)
			{
				if(m_v.iValue<m_v.iUpperLimit)
				{
					if(m_v.iValue==0)
					{
						m_v.iValue=m_v.iLowerLimit;
					}
					else
						m_v.iValue=m_v.iValue+1000;



					if(m_v.iValue>m_v.iUpperLimit)
					{
						m_v.iValue=m_v.iUpperLimit;
						m_bEndOfRange = true;
					}


				}
				else if(m_bScrollOver)
					m_v.iValue=m_v.iLowerLimit;
				else
				{
					m_bEndOfRange = true;
				}

				CClientDC dc(this);
				m_btn.pcBmpDown->Draw(m_hDC);
				Draw(BTN_DW_DW);
				BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
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


