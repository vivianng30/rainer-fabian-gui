// ParaBtn_PCurve.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ParaBtn_PCurve.h"
#include "globDefs.h"

//global font objects
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf31AcuBold;

/**********************************************************************************************//**
 * CParaBtn_PCurve
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CParaBtn_PCurve, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CParaBtn_PCurve class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 *
 * \param	btn		   	The button.
 * \param	nOffset	   	The offset.
 * \param	bScrollOver	True to scroll over.
 **************************************************************************************************/

CParaBtn_PCurve::CParaBtn_PCurve(BTN btn, int nOffset,bool bScrollOver):
CParaBtn(btn,nOffset,bScrollOver)
{
	m_pcIFlow= NULL;
	m_pcLinear= NULL;
	m_pcAutoflow= NULL;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CParaBtn_PCurve class
 *
 * \author	Rainer Kühner
 * \date	22.02.2018
 **************************************************************************************************/

CParaBtn_PCurve::~CParaBtn_PCurve()
{
	delete m_pcIFlow;
	m_pcIFlow=NULL;

	delete m_pcLinear;
	m_pcLinear=NULL;

	delete m_pcAutoflow;
	m_pcAutoflow=NULL;
}


BEGIN_MESSAGE_MAP(CParaBtn_PCurve, CButton)
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
 * CParaBtn-Meldungshandler
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

BOOL CParaBtn_PCurve::Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v) 
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

		m_pcIFlow= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_PCTRL_IFLOW);
		m_pcLinear= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_PCTRL_RAMPE);
		m_pcAutoflow= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_PCTRL_AUTOFLOW);


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

void CParaBtn_PCurve::Draw(int nState)
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


			rcName.top = m_rcClient.top+3;
			rcName.bottom = 27;
			rcName.left = m_rcClient.left;
			rcName.right = m_rcClient.right;

			SelectObject(m_hDC,g_hf7AcuBold);

			/*DrawText(m_hDC,_T("Pressure"),-1,&rcName,DT_TOP|DT_SINGLELINE|DT_CENTER);
			DrawText(m_hDC,_T("rise control"),-1,&rcName,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);*/
			//CString szTest=getModel()->GetLanguageString(IDS_PARA_PRESSURERISE_BOT);
			if(getModel()->GetLanguageString(IDS_PARA_PRESSURERISE_BOT)==_T("0"))
			{
				DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_PRESSURERISE_TOP),-1,&rcName,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
			}
			else
			{
				DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_PRESSURERISE_TOP),-1,&rcName,DT_TOP|DT_SINGLELINE|DT_CENTER);
				DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_PRESSURERISE_BOT),-1,&rcName,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

			}
						

			RECT rcUnit;
			rcUnit.top = m_rcClient.top;
			rcUnit.bottom = m_rcClient.bottom-5;
			rcUnit.left = m_rcClient.left;
			rcUnit.right = m_rcClient.right;

			switch((eCurveForm)m_v.iValue)
			{
			case CURVE_IFLOW:
				{
					m_pcIFlow->Draw(m_hDC,23,30);

					if(m_bEndOfRange)
						DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_ENDRANGE),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
					else
					{
						//DrawText(m_hDC,_T("I-Flow"),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
						DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_IFLOW),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
					}

				}
				break;
			case CURVE_LINEAR:
				{
					m_pcLinear->Draw(m_hDC,23,30);

					if(m_bEndOfRange)
						DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_ENDRANGE),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
					else
					{
						DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_RISETIME),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
						//DrawText(m_hDC,_T("RiseTime"),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
					}

				}
				break;
			case CURVE_AUTOFLOW:
				{
					m_pcAutoflow->Draw(m_hDC,23,30);

					if(m_bEndOfRange)
						DrawText(m_hDC,getModel()->GetLanguageString(IDS_TXT_ENDRANGE),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
					else
					{
						DrawText(m_hDC,getModel()->GetLanguageString(IDS_PARA_AUTOFLOW),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
						//DrawText(m_hDC,_T("autoIFlow"),-1,&rcUnit,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);
					}

				}
				break;
			default:
				{

				}
				break;
			}


			

			//SelectObject(m_hDC,g_hf7AcuNorm);
			//SetTextColor(m_hDC,RGB(0,0,0));

			
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

void CParaBtn_PCurve::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
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
					//TODO_AUTOIFLOW
					/*if(false==getModel()->getCONFIG()->isRISETIMEREGISTRYenabled())
					{
						m_v.iValue=CURVE_IFLOW;
					}
					else*/
					{
						m_v.iValue--;
					}
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
					//TODO_AUTOIFLOW
					/*if(false==getModel()->getCONFIG()->isRISETIMEREGISTRYenabled())
					{
						m_v.iValue=CURVE_AUTOFLOW;
					}
					else*/
					{
						m_v.iValue++;
					}
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


