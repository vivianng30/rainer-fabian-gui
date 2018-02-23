// ParaBtn_O2Flush.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ParaBtn_O2Flush.h"
#include "globDefs.h"

extern HFONT g_hf7AcuNorm;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf8AcuBold;
extern HFONT g_hf23AcuBold;

/**********************************************************************************************//**
 * Initializes a new instance of the CParaBtn_O2Flush class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param [in,out]	btn		   	The button.
 * \param 		  	nOffset	   	The offset.
 * \param 		  	bScrollOver	True to scroll over.
 **************************************************************************************************/

CParaBtn_O2Flush::CParaBtn_O2Flush(BTN &btn, int nOffset,bool bScrollOver):
CParaBtn(btn,nOffset,bScrollOver)
{
	m_bOff=false;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CParaBtn_O2Flush class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CParaBtn_O2Flush::~CParaBtn_O2Flush(void)
{

}

BEGIN_MESSAGE_MAP(CParaBtn_O2Flush, CButton)
	//{{AFX_MSG_MAP(CParaBtn_O2Flush)
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
 * Check value
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	iO2Value	Zero-based index of the o 2 value.
 **************************************************************************************************/

void CParaBtn_O2Flush::CheckValue(int iO2Value)
{
	if(m_bOff)
		return;
	int iTemp=iO2Value+getModel()->getDATAHANDLER()->getO2Diff();

	if(iTemp<=m_v.iUpperLimit)
	{
		m_v.iValue=iTemp;

		RefreshBtn();
		WriteCurrentValue();
	}
}

/**********************************************************************************************//**
 * Draws
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	nState	The state.
 **************************************************************************************************/

void CParaBtn_O2Flush::Draw(int nState)
{
	if(!m_bDisableText)
	{
		//if(g_hf31AcuBold )
		{
			CDC* pDC=CDC::FromHandle(m_hDC);
			HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf31AcuBoldNum);
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

			if(m_bOff)
			{
				SelectObject(m_hDC,g_hf23AcuBold);
				wsprintf(psz,_T("%s"),getModel()->GetLanguageString(IDS_TXT_OFF));
			}
			else
			{
				wsprintf(psz,_T("%d"),m_v.iValue);
			}

			DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat);

			CRect rcName;

			rcName.top = m_rcClient.top;
			rcName.bottom = 30;
			rcName.left = m_rcClient.left;
			rcName.right = m_rcClient.right;

			SelectObject(m_hDC,g_hf14AcuBold);

			CSize szO2 = pDC->GetTextExtent(m_pszNameText);
			CSize szFlush = pDC->GetTextExtent(m_pszNameNoteText);
			LONG szComplete=szO2.cx + szFlush.cx;
			LONG paraBtnX=rcName.right-rcName.left;

			CString name(m_pszNameText);
			if(szComplete>paraBtnX)
			{
				if(name==_T("O2"))
				{
					DrawText(m_hDC,_T("O"),-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
					rcName.left=rcName.left+4+pDC->GetTextExtent(_T("O")).cx;
					SelectObject(m_hDC,g_hf8AcuBold);
					rcName.bottom = 40;
					DrawText(m_hDC,_T("2"),-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

					rcName.bottom = 30;
					rcName.left=rcName.left+3+pDC->GetTextExtent(_T("2")).cx;
					SelectObject(m_hDC,g_hf14AcuBold);
					DrawText(m_hDC,m_pszNameNoteText,-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
				}
				else
				{
					DrawText(m_hDC,m_pszNameText,-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
					rcName.left=rcName.left+1+szO2.cx;
					DrawText(m_hDC,m_pszNameNoteText,-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
				}
			}
			else
			{
				rcName.left=((paraBtnX-szComplete)/2);

				if(name==_T("O2"))
				{
					DrawText(m_hDC,_T("O"),-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
					rcName.left=rcName.left+4+pDC->GetTextExtent(_T("0")).cx;
					SelectObject(m_hDC,g_hf8AcuBold);
					rcName.bottom = 40;
					DrawText(m_hDC,_T("2"),-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_LEFT);

					rcName.bottom = 30;
					rcName.left=rcName.left+1+pDC->GetTextExtent(_T("2")).cx;
					SelectObject(m_hDC,g_hf14AcuBold);
					DrawText(m_hDC,m_pszNameNoteText,-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
				}
				else
				{
					DrawText(m_hDC,m_pszNameText,-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
					rcName.left=rcName.left+1+szO2.cx;
					DrawText(m_hDC,m_pszNameNoteText,-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_LEFT);
				}
			}

			

			/*rcName.top = m_rcClient.top;
			rcName.bottom = 40;
			rcName.left = m_rcClient.left+20;
			rcName.right = m_rcClient.right;

			SelectObject(m_hDC,g_hf8AcuBold);

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
			else if(m_bLimited)
				DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_LIMITTED),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
			else if(!m_bOff)
				DrawText(m_hDC,m_pszUnitText,-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

			m_bEndOfRange=false;
			m_bLimited=false;
			

			SetTextColor(m_hDC,nPrevTxtColor);
			SetBkMode(m_hDC,nBkMode);
			SelectObject(m_hDC,hPrevFont);
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

void CParaBtn_O2Flush::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//CParaBtn::OnKeyUp(nChar,nRepCnt,nFlags);
	//bool bLimitsOk=true;

	if( m_bLMouseButtonDown )
	{

		if(nChar==m_kDown)	
		{
			if(m_bDepressed)
			{
				//if(!m_bOff)
				{
					

					if(m_v.iValue>m_v.iLowerLimit)
					{
						int dTemp = m_v.iValue;

						dTemp--;

						if(dTemp-(MAXDIFF_O2_FLUSH-1)<=getModel()->getDATAHANDLER()->GetCurrentO2Para())
						{
							m_bLimited=true;
							if(GetParent())
								GetParent()->PostMessage(WM_O2FLUSH_LIMITED);
						}
						else
						{
							m_v.iValue=dTemp;
							if(GetParent())
								GetParent()->PostMessage(WM_O2FLUSH_CHANGED);
							getModel()->getDATAHANDLER()->setO2Diff(m_v.iValue);
						}
					}
					else if(m_bScrollOver)
						m_v.iValue=m_v.iUpperLimit;
					else
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

					if(GetParent())
						GetParent()->PostMessage(WM_O2FLUSH_CHANGED);
					getModel()->getDATAHANDLER()->setO2Diff(m_v.iValue);
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
		}
		else
		{
			if(GetParent())
				GetParent()->PostMessage(WM_O2FLUSH_CHANGED);
			getModel()->getDATAHANDLER()->setO2Diff(m_v.iValue);
		}

		CParaBtn::OnKeyUp(nChar,nRepCnt,nFlags);
	}
	m_bLMouseButtonDown = false;
}
