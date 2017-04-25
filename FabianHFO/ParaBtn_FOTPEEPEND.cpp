// ParaBtn_FOTPEEPEND.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ParaBtn_FOTPEEPEND.h"
#include "globDefs.h"

extern HFONT g_hf7AcuNorm;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf7AcuBold;

// CParaBtn_FOTPEEPEND


CParaBtn_FOTPEEPEND::CParaBtn_FOTPEEPEND(BTN btn, int nOffset,bool bScrollOver):
CParaBtn(btn,nOffset,bScrollOver)
{

}

CParaBtn_FOTPEEPEND::~CParaBtn_FOTPEEPEND()
{
}


BEGIN_MESSAGE_MAP(CParaBtn_FOTPEEPEND, CButton)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CParaBtn_FOTPEEPEND message handlers
void CParaBtn_FOTPEEPEND::Draw(int nState)
{
	if(!m_bDisableText)
	{
		CDC* pDC=CDC::FromHandle(m_hDC);
		HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf31AcuBoldNum);
		int nBkMode=SetBkMode(m_hDC,TRANSPARENT);
		int nTxtColor;

		CBmp* pcArrow=NULL;

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
			break;
		case BTN_DW_DIS:
		default:
			nTxtColor=m_btncr.crTxtDisabled;
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

		if(m_v.iValue!=0)
		{
			wsprintf(psz,_T("%0.0f"), CTlsFloat::Round(((double)m_v.iValue)/10, 0));
			//wsprintf(psz,_T("%0.1f"), (CTlsFloat::Round( (double)m_v.iValue/5, 0 )*5)/10);
		}
		else
			wsprintf(psz,_T("%d"), 0);

		//test
		//int iT = m_btn.wID;
		DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat);

		CRect rcName;

		SelectObject(m_hDC,g_hf14AcuBold);
		CSize TextSize1 = pDC->GetTextExtent(m_pszNameText);

		SelectObject(m_hDC,g_hf7AcuBold);
		CSize TextSize2 = pDC->GetTextExtent(m_pszNameNoteText);

		SelectObject(m_hDC,g_hf14AcuBold);
		int iDiff=(m_rcClient.right-TextSize1.cx-TextSize2.cx)/2;

		rcName.top = m_rcClient.top;
		rcName.bottom = 30;
		rcName.left = m_rcClient.left;
		rcName.right = rcName.left+iDiff+TextSize1.cx;

		DrawText(m_hDC,m_pszNameText,-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_RIGHT);

		SelectObject(m_hDC,g_hf7AcuBold);
		rcName.bottom = 35;
		rcName.left = rcName.right;
		rcName.right = m_rcClient.right;
		DrawText(m_hDC,m_pszNameNoteText,-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

		RECT rcUnit;
		rcUnit.top = m_rcClient.top;
		rcUnit.bottom = m_rcClient.bottom-5;
		rcUnit.left = m_rcClient.left;
		rcUnit.right = m_rcClient.right;

		SelectObject(m_hDC,g_hf7AcuNorm);
		//SetTextColor(m_hDC,RGB(0,0,0));

		if(m_bEndOfRange)
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_ENDRANGE),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
		else if(m_bLimited)
			DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_LIMITTED),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
		else
		{
			DrawText(m_hDC,m_pszUnitText,-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

			if(m_bDrawWarning && m_bHasFocus)
			{
				if(m_pcWarning_Fc)
					m_pcWarning_Fc->Draw(m_hDC,60,65);
			}
			else if(m_bDrawWarning && m_bDepressed)
			{
				if(m_pcWarning_Dw)
					m_pcWarning_Dw->Draw(m_hDC,60,65);
			}
			else if(m_bDrawWarning)
			{
				if(m_pcWarning_Up)
					m_pcWarning_Up->Draw(m_hDC,60,65);
			}

			if(m_bDrawKey && m_bDepressed)
			{
				if(m_pcKey)
					m_pcKey->Draw(m_hDC,10,65);
			}
			else if(m_bDrawArrowDown && m_bHasFocus)
			{
				pcArrow=m_pcArrowDw_FC;

				if(pcArrow)
					pcArrow->Draw(m_hDC,10,65);
			}
			else if(m_bDrawArrowDown && m_bDepressed)
			{
				pcArrow=m_pcArrowDw_DW;

				if(pcArrow)
					pcArrow->Draw(m_hDC,10,65);
			}
			else if(m_bDrawArrowDown  && !m_bHasFocus && !m_bDepressed)
			{
				pcArrow=m_pcArrowDw_UP;

				if(pcArrow)
					pcArrow->Draw(m_hDC,10,65);
			}
			else if(m_bDrawArrowUp && m_bHasFocus)
			{
				pcArrow=m_pcArrowUp_FC;

				if(pcArrow)
					pcArrow->Draw(m_hDC,10,65);
			}
			else if(m_bDrawArrowUp && m_bDepressed)
			{
				pcArrow=m_pcArrowUp_DW;

				if(pcArrow)
					pcArrow->Draw(m_hDC,10,65);
			}
			else if(m_bDrawArrowUp  && !m_bHasFocus && !m_bDepressed)
			{
				pcArrow=m_pcArrowUp_UP;

				if(pcArrow)
					pcArrow->Draw(m_hDC,10,65);
			}
		}

		m_bEndOfRange=false;
		m_bLimited=false;


		SetTextColor(m_hDC,nPrevTxtColor);
		SetBkMode(m_hDC,nBkMode);
		SelectObject(m_hDC,hPrevFont);
	}
}



void CParaBtn_FOTPEEPEND::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( m_bLMouseButtonDown )
	{
		if(nChar==m_kDown)	
		{
			if(m_bDepressed)
			{
				//if(m_v.iValue>iLowerLimit)
				{
					WORD iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_PEEPSTARTPara()+getModel()->getCONFIG()->getDiffPEEPFOT();
					int dTemp = m_v.iValue-10;
					if(dTemp<iLowerLimit)
					{
						m_bLimited=true;
						if(GetParent())
							GetParent()->PostMessage(WM_FOT_PEEPEND_LIMITED);
					}
					else
					{
						m_v.iValue=dTemp;

						if(dTemp<=getModel()->getDATAHANDLER()->GetCurrentPeepMaxKey())
						{
							m_bKeyValueAccepted=false;
							/*if(m_bDrawKey)
							{
								if(GetParent())
									GetParent()->PostMessage(WM_PMEANEND_CLEARKEY);
							}*/
							m_bDrawKey=false;
							m_bDrawWarning=false;
						}
						m_bWaitConfirm=false;
						m_bKeyBeep=TRUE;

						if(GetParent())
							GetParent()->PostMessage(WM_FOT_PEEPEND_CHANGED);
					}
				}
				/*else if(m_bScrollOver)
					m_v.iValue=m_v.iUpperLimit;
				else
					m_bEndOfRange = true;*/

				CClientDC dc(this);
				m_btn.pcBmpDown->Draw(m_hDC);
				Draw(BTN_DW_DW);
				BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
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
					int iTemp=m_v.iValue+10;

					if(iTemp>getModel()->getDATAHANDLER()->GetCurrentPeepMaxKey())
					{
						if(m_bKeyValueAccepted==false)
						{
							m_bWaitConfirm=true;
							m_bDrawKey=true;
							if(GetParent())
								GetParent()->PostMessage(WM_PEEPEND_SETKEY,m_bKeyBeep);
							m_bKeyBeep=FALSE;
						}
						else
						{
							m_v.iValue=iTemp;
						}
						m_bDrawWarning=true;
					}
					else
					{
						m_v.iValue=iTemp;
					}
				}
				else if(m_bScrollOver)
					m_v.iValue=m_v.iLowerLimit;
				else
					m_bEndOfRange = true;

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
			if(GetParent())
				GetParent()->PostMessage(WM_FOT_PEEPEND_CHANGED);
		}
		else
		{
			if(GetParent())
				GetParent()->PostMessage(WM_FOT_PEEPEND_CHANGED);
		}
		CParaBtn::OnKeyUp(nChar,nRepCnt,nFlags);
	}
	m_bLMouseButtonDown = false;
}




