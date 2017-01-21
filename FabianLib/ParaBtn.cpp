// ParaBtn.cpp: Implementierungsdatei
//

#include "stdafx.h"
//#include "../FabianHFO.h"
//#include "../globDefs.h"
#include "globDefs.h"
#include "ParaBtn.h"
//#include "../MVViewHandler.h"
#include "MVViewHandler.h"


// CParaBtn

IMPLEMENT_DYNAMIC(CParaBtn, CButton)

CParaBtn::CParaBtn(BTN &btn, int nOffset,bool bScrollOver)
{
	// Init state
	m_pModel=NULL;

	m_hBm=NULL;
	m_hBmPrev=NULL;

	m_v.iValue=0;
	m_v.iUpperLimit=0;
	m_v.iLowerLimit=0;
	//m_v.tText[0]=0x0000;
	//m_iCountUDP=0;

	m_iPrevValue=0;
	m_bLastParaState=false;

	m_pcBmpFocusDown=NULL;
	m_pcArrowUp_GREY=NULL;
	m_pcArrowUp_RED=NULL;
	m_pcArrowDw_RED=NULL;
	m_pcArrowUp_UP=NULL;
	m_pcArrowUp_FC=NULL;
	m_pcArrowUp_DW=NULL;
	m_pcArrowDw_UP=NULL;
	m_pcArrowDw_FC=NULL;
	m_pcArrowDw_DW=NULL;
	m_pcKey=NULL;
	m_pcWarning_Dis=NULL;
	m_pcWarning_Up=NULL;
	m_pcWarning_Fc=NULL;
	m_pcWarning_Dw=NULL;
	m_pcWarning_Red=NULL;

	//m_pcAlarmArrowUp_RED=NULL;
	//m_pcAlarmArrowDw_RED=NULL;
	m_pcAlarmArrowUp_UP=NULL;
	m_pcAlarmArrowUp_FC=NULL;
	m_pcAlarmArrowUp_DW=NULL;
	m_pcAlarmArrowDw_UP=NULL;
	m_pcAlarmArrowDw_FC=NULL;
	m_pcAlarmArrowDw_DW=NULL;

	m_bTurnedOff=false;
	m_bHasFocus=false;
	m_bDepressed=false;
	m_bLMouseButtonDown=false;
	m_bLButtonDown=false;
	
	m_bLimited=false;
	m_bEndOfRange = false;
	m_bOff=false;
	m_bDrawArrowDown=false;
	m_bDrawArrowUp=false;
	m_bDrawAlarmArrowDown=false;
	m_bDrawAlarmArrowUp=false;

	m_bWaitConfirm=false;
	m_bKeyBeep=TRUE;
	m_bKeyValueAccepted=false;
	m_bDrawKey=false;
	m_bDrawWarning=false;

	//m_bAutoState=false;
	m_bSignaled=false;

	m_dwSpeedDelta=0;
	m_dwPushDelta=0;

	//m_kUp					= VK_DOWN;//up dw revers
	//m_kDown					= VK_UP;
	m_kUp					= VK_UP;
	m_kDown					= VK_DOWN;
	m_kSpace				= VK_SPACE;
	//m_kSpace				= VK_RETURN;

	memcpy(&m_btn,&btn,sizeof(BTN));
	// Default colors
	m_btncr.crTxtUp			= 0x00000000;
	m_btncr.crTxtDown		= 0x00000000;
	m_btncr.crTxtDisabled	= 0x00c0c0c0;;		
	m_btncr.crTxtFocus		= 0x00000000;	
	//
	m_hDC=NULL;
	//
	m_pszValueText[0]=0x0000;
	m_pszNameText[0]=0x0000;
	m_pszNameNoteText[0]=0x0000;
	m_pszUnitText[0]=0x0000;
	

	//m_bButtonIsActive=false;

	m_bTextOnly=false;

	m_nOffset=nOffset;

	m_bDisableText=false;

	m_bScrollOver=bScrollOver;
}

CParaBtn::~CParaBtn()
{
	//getModel()=NULL;

	/*if(m_pcAlarmArrowUp_RED)
		delete m_pcAlarmArrowUp_RED;
	m_pcAlarmArrowUp_RED=NULL;*/
	/*if(m_pcAlarmArrowDw_RED)
		delete m_pcAlarmArrowDw_RED;
	m_pcAlarmArrowDw_RED=NULL;*/
	if(m_pcAlarmArrowUp_UP)
		delete m_pcAlarmArrowUp_UP;
	m_pcAlarmArrowUp_UP=NULL;
	if(m_pcAlarmArrowUp_FC)
		delete m_pcAlarmArrowUp_FC;
	m_pcAlarmArrowUp_FC=NULL;
	if(m_pcAlarmArrowUp_DW)
		delete m_pcAlarmArrowUp_DW;
	m_pcAlarmArrowUp_DW=NULL;
	if(m_pcAlarmArrowDw_UP)
		delete m_pcAlarmArrowDw_UP;
	m_pcAlarmArrowDw_UP=NULL;
	if(m_pcAlarmArrowDw_FC)
		delete m_pcAlarmArrowDw_FC;
	m_pcAlarmArrowDw_FC=NULL;
	if(m_pcAlarmArrowDw_DW)
		delete m_pcAlarmArrowDw_DW;
	m_pcAlarmArrowDw_DW=NULL;

	if(m_pcArrowUp_GREY)
		delete m_pcArrowUp_GREY;
	m_pcArrowUp_GREY=NULL;

	if(m_pcArrowUp_RED)
		delete m_pcArrowUp_RED;
	m_pcArrowUp_RED=NULL;

	if(m_pcArrowDw_RED)
		delete m_pcArrowDw_RED;
	m_pcArrowDw_RED=NULL;

	if(m_pcArrowUp_UP)
		delete m_pcArrowUp_UP;
	m_pcArrowUp_UP=NULL;

	if(m_pcArrowUp_FC)
		delete m_pcArrowUp_FC;
	m_pcArrowUp_FC=NULL;

	if(m_pcArrowUp_DW)
		delete m_pcArrowUp_DW;
	m_pcArrowUp_DW=NULL;

	if(m_pcArrowDw_UP)
		delete m_pcArrowDw_UP;
	m_pcArrowDw_UP=NULL;

	if(m_pcArrowDw_FC)
		delete m_pcArrowDw_FC;
	m_pcArrowDw_FC=NULL;

	if(m_pcArrowDw_DW)
		delete m_pcArrowDw_DW;
	m_pcArrowDw_DW=NULL;

	if(m_pcKey)
		delete m_pcKey;
	m_pcKey=NULL;


	if(m_pcWarning_Dis)
		delete m_pcWarning_Dis;
	m_pcWarning_Dis=NULL;

	if(m_pcWarning_Up)
		delete m_pcWarning_Up;
	m_pcWarning_Up=NULL;
	if(m_pcWarning_Fc)
		delete m_pcWarning_Fc;
	m_pcWarning_Fc=NULL;
	if(m_pcWarning_Dw)
		delete m_pcWarning_Dw;
	m_pcWarning_Dw=NULL;
	if(m_pcWarning_Red)
		delete m_pcWarning_Red;
	m_pcWarning_Red=NULL;



	if(m_hDC)
	{
		SelectObject(m_hDC,m_hBmPrev);
		DeleteObject(m_hBm);
		DeleteDC(m_hDC);
		//CDC::FromHandle(m_hDC)->DeleteDC();
	}
}

// **************************************************************************
// 
// **************************************************************************
CMVModel *CParaBtn::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


BEGIN_MESSAGE_MAP(CParaBtn, CButton)
	//{{AFX_MSG_MAP(CParaBtn)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	/*ON_WM_SETFOCUS()*/
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CParaBtn-Meldungshandler
BOOL CParaBtn::Create(CWnd* pParentWnd,DWORD dwStyle, fVALUE v) 
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

		m_pcArrowUp_GREY	= new CBmp(theApp.m_hInstance,m_hDC,	IDB_ARROWUP_GREY);
		m_pcArrowUp_RED		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_ARROWUP_RED);
		m_pcArrowDw_RED		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_ARROWDW_RED);
		m_pcArrowUp_UP		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_ARROWUP_UP);
		m_pcArrowUp_FC		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_ARROWUP_FC);
		m_pcArrowUp_DW		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_ARROWUP_DW);
		m_pcArrowDw_UP		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_ARROWDW_UP);
		m_pcArrowDw_FC		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_ARROWDW_FC);
		m_pcArrowDw_DW		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_ARROWDW_DW);
		m_pcKey				= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PARA_KEY_DW);
		m_pcWarning_Dis		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PARA_WARN_DIS);
		m_pcWarning_Up		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PARA_WARN_UP);
		m_pcWarning_Fc		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PARA_WARN_FC);
		m_pcWarning_Dw		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PARA_WARN_DW);
		m_pcWarning_Red		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_PARA_WARN_RED);

		//m_pcAlarmArrowUp_RED;
		//m_pcAlarmArrowDw_RED;
		m_pcAlarmArrowUp_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_ALARMARROWUP_UP);
		m_pcAlarmArrowUp_FC= new CBmp(theApp.m_hInstance,m_hDC,	IDB_ALARMARROWUP_FC);
		m_pcAlarmArrowUp_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_ALARMARROWUP_DW);
		m_pcAlarmArrowDw_UP= new CBmp(theApp.m_hInstance,m_hDC,	IDB_ALARMARROWDW_UP);
		m_pcAlarmArrowDw_FC= new CBmp(theApp.m_hInstance,m_hDC,	IDB_ALARMARROWDW_FC);
		m_pcAlarmArrowDw_DW= new CBmp(theApp.m_hInstance,m_hDC,	IDB_ALARMARROWDW_DW);


		memcpy(&m_v,&v,sizeof(fVALUE));

		return 1;
	}
	return 0;
}



void CParaBtn::SetSignaled(bool bSignaled)
{
	m_bSignaled=bSignaled;
}

void CParaBtn::SetBitmaps(CBmp* pcBmpUp,CBmp* pcBmpDown,CBmp* pcBmpDisabled,CBmp* pcBmpFocus,bool bSignaled,bool bRedraw)
{
	m_bSignaled=bSignaled;
	if(m_btn.pcBmpUp!=pcBmpUp)
	{
		m_btn.pcBmpUp=pcBmpUp;
		m_btn.pcBmpDown=pcBmpDown;
		m_btn.pcBmpDisabled=pcBmpDisabled;
		m_btn.pcBmpFocus=pcBmpFocus;
		if(bRedraw)
		{
			Invalidate();
			UpdateWindow();
		}
		
	}
}

void CParaBtn::SetKeyValueAccepted(bool state)
{
	m_bKeyValueAccepted=state;
}

void CParaBtn::SetWarning(bool state)
{
	bool bRefresh = false;

	if(m_bDrawWarning!=state)
		bRefresh = true;

	if(state)
	{
		m_bKeyValueAccepted=true;
		m_bDrawKey=false;
		m_bDrawWarning=true;
		m_bWaitConfirm=false;
		m_bKeyBeep=TRUE;
	}
	else
	{
		m_bKeyValueAccepted=false;
		m_bDrawKey=false;
		m_bWaitConfirm=false;
		m_bDrawWarning=false;
		m_bKeyBeep=TRUE;
	}


	if(bRefresh)
	{
		//RefreshBtn();
		Invalidate();
		UpdateWindow();
	}
	
}

// **************************************************************************
// Operation methods
// **************************************************************************
bool CParaBtn::IsDepressed( void )
{
	// Return the buttons state
	return m_bDepressed;
}

bool CParaBtn::Depress()
{
	// Set the buttons state
	if(m_bDepressed)
	{
		m_v.iValue=m_iPrevValue;
		m_bOff=m_bLastParaState;

		if(m_btn.wID==IDC_BTN_PARA_ITIME)
		{
			getModel()->getDATAHANDLER()->SetCurrentTempITime(m_v.iValue);

			if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
			{
				getModel()->getDATAHANDLER()->CalculateETime_ITimeChanged(m_v.iValue,false);
			}
			else
			{
				getModel()->getDATAHANDLER()->CalculateBPM_ITimeChanged(m_v.iValue,false);
			}

			int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
			if(iIE<100)
			{
				m_bDrawWarning=true;
				m_bKeyValueAccepted=true;
			}
			else
			{
				m_bDrawWarning=false;
				m_bKeyValueAccepted=false;
			}
			if(GetParent())
			{
				//GetParent()->PostMessage(WM_ITIME_CLEARKEY);
				GetParent()->PostMessage(WM_ITIME_CHANGED);
			}
		}
		else if(m_btn.wID==IDC_BTN_PARA_ETIME)
		{
			getModel()->getDATAHANDLER()->SetCurrentTempETime(m_v.iValue);

			getModel()->getDATAHANDLER()->CalculateBPM_ETimeChanged(m_v.iValue,false);

			int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
			if(iIE<100)
			{
				m_bDrawWarning=true;
				m_bKeyValueAccepted=true;
			}
			else
			{
				m_bDrawWarning=false;
				m_bKeyValueAccepted=false;
			}
			if(GetParent())
			{
				//GetParent()->PostMessage(WM_ETIME_CLEARKEY);
				GetParent()->PostMessage(WM_ETIME_CHANGED);
			}
		}
		else if(m_btn.wID==IDC_BTN_PARA_BPM)
		{
			getModel()->getDATAHANDLER()->CalculateETime_BPMChanged(m_v.iValue, false);

			int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
			if(iIE<100)
			{
				m_bDrawWarning=true;
				m_bKeyValueAccepted=true;
			}
			else
			{
				m_bDrawWarning=false;
				m_bKeyValueAccepted=false;
			}
			if(GetParent())
			{
				//GetParent()->PostMessage(WM_BPM_CLEARKEY);
				GetParent()->PostMessage(WM_BPM_CHANGED);
			}
		}
		else if(m_btn.wID==IDC_BTN_PARA_PINSP)
		{
			if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPInspMaxKey())
			{
				m_bDrawWarning=true;
				m_bKeyValueAccepted=true;
			}
			else
			{
				m_bDrawWarning=false;
				m_bKeyValueAccepted=false;
			}
			if(GetParent())
			{
				//GetParent()->PostMessage(WM_PINSP_CLEARKEY);
				GetParent()->PostMessage(WM_PINSP_CHANGED);
			}
		}
		else if(m_btn.wID==IDC_BTN_PARA_VG_PMAXI)
		{
			if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPInspMaxKey())
			{
				m_bDrawWarning=true;
				m_bKeyValueAccepted=true;
			}
			else
			{
				m_bDrawWarning=false;
				m_bKeyValueAccepted=false;
			}
			if(GetParent())
			{
				//GetParent()->PostMessage(WM_PMAX_CLEARKEY);
				GetParent()->PostMessage(WM_PMAXI_CHANGED);
			}
		}
		else if(m_btn.wID==IDC_BTN_PARA_VG_AMPMAXI)
		{
			if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPInspMaxKey())
			{
				m_bDrawWarning=true;
				m_bKeyValueAccepted=true;
			}
			else
			{
				m_bDrawWarning=false;
				m_bKeyValueAccepted=false;
			}
			if(GetParent())
			{
				//GetParent()->PostMessage(WM_PMAX_CLEARKEY);
				GetParent()->PostMessage(WM_AMPMAXI_CHANGED);
			}
		}
		else if(m_btn.wID==IDC_BTN_PARA_PDUO)
		{
			if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPManualMaxKey())
			{
				m_bDrawWarning=true;
				m_bKeyValueAccepted=true;
			}
			else
			{
				m_bDrawWarning=false;
				m_bKeyValueAccepted=false;
			}
			if(GetParent())
			{
				//GetParent()->PostMessage(WM_PDUO_CLEARKEY);
				GetParent()->PostMessage(WM_PDUO_CHANGED);
			}
		}
		else if(m_btn.wID==IDC_BTN_PARA_THERAPYFLOW)
		{
			if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentTherapieFlowMaxKey())
			{
				m_bDrawWarning=true;
				m_bKeyValueAccepted=true;
			}
			else
			{
				m_bDrawWarning=false;
				m_bKeyValueAccepted=false;
			}
			//if(GetParent())
			//{
			//	GetParent()->PostMessage(WM_THERAPYFLOW_CLEARKEY);
			//	//GetParent()->PostMessage(WM_EFLOW_CHANGED);
			//}
		}
		else if(m_btn.wID==IDC_BTN_PARA_PMANUAL)
		{
			if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPManualMaxKey())
			{
				m_bDrawWarning=true;
				m_bKeyValueAccepted=true;
			}
			else
			{
				m_bDrawWarning=false;
				m_bKeyValueAccepted=false;
			}
			if(GetParent())
			{
				//GetParent()->PostMessage(WM_PMAN_CLEARKEY);
				GetParent()->PostMessage(WM_PMAN_CHANGED);
			}
		}
		else if(m_btn.wID==IDC_BTN_PARA_PPSV)
		{
			if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
			{
				if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPpsvMaxKey())
				{
					m_bDrawWarning=true;
					if(GetParent())
						GetParent()->PostMessage(WM_SET_PEEPWARNING);
					m_bKeyValueAccepted=true;
				}
				else
				{
					m_bDrawWarning=false;
					if(GetParent())
						GetParent()->PostMessage(WM_CLEAR_PEEPWARNING);
					m_bKeyValueAccepted=false;
				}
			}
			else
			{
				if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPpsvMaxKey())
				{
					m_bDrawWarning=true;
					if(GetParent())
						GetParent()->PostMessage(WM_SET_PEEPWARNING);
					m_bKeyValueAccepted=true;
				}
				else
				{
					m_bDrawWarning=false;
					if(GetParent())
						GetParent()->PostMessage(WM_CLEAR_PEEPWARNING);
					m_bKeyValueAccepted=false;
				}
			}

			if(GetParent())
			{
				//GetParent()->PostMessage(WM_PPSV_CLEARKEY);
				GetParent()->PostMessage(WM_PPSV_CHANGED);
			}
		}
		else if(m_btn.wID==IDC_BTN_PARA_CPAP)
		{
			if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentCPAPMaxKey())
			{
				m_bDrawWarning=true;
				m_bKeyValueAccepted=true;
			}
			else
			{
				m_bDrawWarning=false;
				m_bKeyValueAccepted=false;
			}
			if(GetParent())
			{
				//GetParent()->PostMessage(WM_CPAP_CLEARKEY);
				GetParent()->PostMessage(WM_CPAP_CHANGED);
			}
		}
		else if(m_btn.wID==IDC_BTN_PARA_PEEP)
		{
			
			if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPeepMaxKey())
			{
				m_bDrawWarning=true;
				m_bKeyValueAccepted=true;
			}
			else if(m_v.iValue<getModel()->getDATAHANDLER()->GetCurrentPeepMinKey())
			{
				m_bDrawWarning=true;
				m_bKeyValueAccepted=true;
			}
			else
			{
				m_bDrawWarning=false;
				m_bKeyValueAccepted=false;
			}

			if(getModel()->getCONFIG()->GetCurMode()==VM_PSV
				||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_PSV
				||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV
				||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMVPSV)
			{
				getModel()->getVIEWHANDLER()->setPEEPvalue(m_v.iValue,false);
			}

			if(GetParent())
			{
				//GetParent()->PostMessage(WM_PEEP_CLEARKEY);
				GetParent()->PostMessage(WM_PEEP_CHANGED,m_v.iValue);
			}
		}
		else if(m_btn.wID==IDC_BTN_PARA_VG_VL)
		{
			if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
				||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
			{
				if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentVgarantMaxKey()
					&& !m_bOff)
				{
					m_bDrawWarning=true;
					m_bKeyValueAccepted=true;
				}
				else
				{
					m_bDrawWarning=false;
					m_bKeyValueAccepted=false;
				}
				/*if(GetParent())
					GetParent()->PostMessage(WM_VGARANT_CLEARKEY);*/

			}
			else if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() 
				&& getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
			{
				if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentVlimitMaxKey()
					&& !m_bOff)
				{
					m_bDrawWarning=true;
					m_bKeyValueAccepted=true;
				}
				else
				{
					m_bDrawWarning=false;
					m_bKeyValueAccepted=false;
				}
				/*if(GetParent())
					GetParent()->PostMessage(WM_VLIMIT_CLEARKEY);*/
			}
			else if(getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
				&& getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
			{
				if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentVgarantMaxKey()
					&& !m_bOff)
				{
					m_bDrawWarning=true;
					m_bKeyValueAccepted=true;
				}
				else
				{
					m_bDrawWarning=false;
					m_bKeyValueAccepted=false;
				}
				/*if(GetParent())
					GetParent()->PostMessage(WM_VGARANT_CLEARKEY);*/
			}
		}
		/*else if(m_btn.wID==IDC_BTN_PARA_HFAMPL)
		{
			if(m_v.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFampMaxKey())
			{
				m_bDrawWarning=true;
				m_bKeyValueAccepted=true;
			}
			else
			{
				m_bDrawWarning=false;
				m_bKeyValueAccepted=false;
			}
			if(GetParent())
				GetParent()->PostMessage(WM_HFAMP_CLEARKEY);
		}*/
		else if(m_btn.wID==IDC_BTN_PARA_HFFREQ)
		{
			/*if(m_v.iValue>getModel()->getDATAHANDLER()->HFO()->GetHFfreqMaxKey())
			{
				m_bDrawWarning=true;
				m_bKeyValueAccepted=true;
			}
			else
			{
				m_bDrawWarning=false;
				m_bKeyValueAccepted=false;
			}
			GetParent()->PostMessage(WM_HFFREQ_CLEARKEY);*/
		}
		else if(m_btn.wID==IDC_BTN_PARA_PMITT)
		{
			if(m_v.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxKey())
			{
				m_bDrawWarning=true;
				m_bKeyValueAccepted=true;
			}
			else
			{
				m_bDrawWarning=false;
				m_bKeyValueAccepted=false;
			}

			if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true)
				getModel()->getVIEWHANDLER()->setPmeanDifference(m_v.iValue,false);

			if(GetParent())
			{
				//GetParent()->PostMessage(WM_PMEAN_CLEARKEY);
				GetParent()->PostMessage(WM_PMEAN_CHANGED,m_v.iValue);
			}

			//PMAN1
			/*if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true)
				getModel()->getVIEWHANDLER()->SetPmeanDifference(m_v.iValue,false);*/
			//if(GetParent())
			//{
			//	//GetParent()->PostMessage(WM_PMEAN_CLEARKEY);//PMAN1
			//	GetParent()->PostMessage(WM_PMEAN_CHANGED,m_v.iValue);
			//}
		}
		else if(m_btn.wID==IDC_BTN_PARA_PMEAN_REC)
		{
			if(m_v.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecMaxKey())
			{
				m_bDrawWarning=true;
				m_bKeyValueAccepted=true;
			}
			else
			{
				m_bDrawWarning=false;
				m_bKeyValueAccepted=false;
			}

			//PMAN1
			if(GetParent())
			{
				//GetParent()->PostMessage(WM_PMEANREC_CLEARKEY);
				GetParent()->PostMessage(WM_PMEANREC_CHANGED);
			}
			getModel()->getVIEWHANDLER()->setPmeanRecDifference(m_v.iValue,true);
			//PMAN1
		}
		else if(m_btn.wID==IDC_BTN_PARA_O2)
		{
			getModel()->getDATAHANDLER()->setO2FlushDiff(m_v.iValue);
			if(GetParent())
				GetParent()->PostMessage(WM_O2_CHANGED,m_v.iValue);
		}
		else if(m_btn.wID==IDC_BTN_PARA_O2FLUSH)
		{
			getModel()->getDATAHANDLER()->setO2Diff(m_v.iValue);
			if(GetParent())
				GetParent()->PostMessage(WM_O2FLUSH_CHANGED);
		}
		/*else if(m_btn.wID==IDC_BTN_PARA_PMITT)
		{
			GetParent()->PostMessage(WM_PMEAN_CHANGED,m_v.iValue);
		}*/
		else if(m_btn.wID==IDC_BTN_PARA_ITIME_REC)
		{
			if(GetParent())
				GetParent()->PostMessage(WM_ITIMEREC_CHANGED);
		}
		else if(m_btn.wID==IDC_BTN_PARA_BPM_REC)
		{
			if(GetParent())
				GetParent()->PostMessage(WM_BPMREC_CHANGED);
		}
		//else if(m_btn.wID==IDC_BTN_PARA_FOTAMP)
		//{
		//	/*if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPInspMaxKey())
		//	{
		//		m_bDrawWarning=true;
		//		m_bKeyValueAccepted=true;
		//	}
		//	else
		//	{
		//		m_bDrawWarning=false;
		//		m_bKeyValueAccepted=false;
		//	}*/
		//	/*if(GetParent())
		//		GetParent()->PostMessage(WM_FOTAMP_CHANGED);*/
		//	//getModel()->getDATAHANDLER()->setCurrentFOT_AMPLITUDEPara(m_v.iValue);
		//}
		//else if(m_btn.wID==IDC_BTN_PARA_FOTFREQ)
		//{
		//	/*if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPInspMaxKey())
		//	{
		//		m_bDrawWarning=true;
		//		m_bKeyValueAccepted=true;
		//	}
		//	else
		//	{
		//		m_bDrawWarning=false;
		//		m_bKeyValueAccepted=false;
		//	}
		//	if(GetParent())
		//	{
		//		GetParent()->PostMessage(WM_PMAX_CLEARKEY);
		//		GetParent()->PostMessage(WM_PMAX_CHANGED);
		//	}*/
		//	//getModel()->getDATAHANDLER()->setCurrentFOT_FREQPara(m_v.iValue);
		//}
		/*else if(IDC_BTN_PARA_FOTSTEPS)
		{

		}
		else if(IDC_BTN_PARA_FOTPMEANLOW)
		{

		}
		else if(IDC_BTN_PARA_FOTPMEANHIGH)
		{

		}*/

		m_bDrawKey=false;
		m_bWaitConfirm=false;
		m_bKeyBeep=TRUE;

		if(GetParent())
			GetParent()->PostMessage(WM_SET_PARATIMER,m_btn.wID);

		WriteCurrentValue();
	}
	else
	{
		m_iPrevValue=m_v.iValue;
		m_bLastParaState=m_bOff;
		if(GetParent())
			GetParent()->PostMessage(WM_SET_PARATIMER);
	}
	m_bDepressed = false;
	m_bTurnedOff=false;

	Invalidate(false);
	return m_bDepressed;
}

void CParaBtn::ClearArrows()
{
	bool bRefresh = false;

	if(true==m_bDrawArrowDown)
		bRefresh = true;
	m_bDrawArrowDown=false;

	if(true==m_bDrawArrowUp)
		bRefresh = true;
	m_bDrawArrowUp=false;

	if(true==m_bDrawAlarmArrowDown)
		bRefresh = true;
	m_bDrawAlarmArrowDown=false;

	if(true==m_bDrawAlarmArrowUp)
		bRefresh = true;
	m_bDrawAlarmArrowUp=false;

	if(bRefresh)
	{
		Invalidate();
		UpdateWindow();
	}
}
void CParaBtn::SetArrowDown(bool state)
{
	bool bRefresh = false;

	//check warning state of PSV button, because it depends on PEEP button
	
	if(state!=m_bDrawArrowDown)
		bRefresh = true;

	if(state)
		m_bDrawArrowDown=true;
	else
		m_bDrawArrowDown=false;

	if(bRefresh)
	{
		//RefreshBtn();
		Invalidate();
		UpdateWindow();
	}
}

void CParaBtn::SetArrowUp(bool state)
{
	//int id=m_btn.wID;
	bool bRefresh = false;
	if(state!=m_bDrawArrowUp)
		bRefresh = true;

	if(state)
		m_bDrawArrowUp=true;
	else
		m_bDrawArrowUp=false;

	if(bRefresh)
	{
		//RefreshBtn();
		Invalidate();
		UpdateWindow();
	}
}

void CParaBtn::SetAlarmArrowDown(bool state)
{
	bool bRefresh = false;

	//check warning state of PSV button, because it depends on PEEP button

	if(state!=m_bDrawAlarmArrowDown)
		bRefresh = true;

	if(state)
		m_bDrawAlarmArrowDown=true;
	else
		m_bDrawAlarmArrowDown=false;

	if(bRefresh)
	{
		//RefreshBtn();
		Invalidate();
		UpdateWindow();
	}
}

void CParaBtn::SetAlarmArrowUp(bool state)
{
	//int id=m_btn.wID;
	bool bRefresh = false;
	if(state!=m_bDrawAlarmArrowUp)
		bRefresh = true;

	if(state)
		m_bDrawAlarmArrowUp=true;
	else
		m_bDrawAlarmArrowUp=false;

	if(bRefresh)
	{
		//RefreshBtn();
		Invalidate();
		UpdateWindow();
	}
}


void CParaBtn::SetColors(COLORREF crTxtUp,COLORREF crTxtDown,COLORREF crSubTxtDown,COLORREF crTxtFocus, COLORREF crDisabel, bool bRefresh)
{
	m_btncr.crTxtUp			= crTxtUp;
	m_btncr.crTxtDown		= crTxtDown;
	m_btncr.crTxtDisabled	= crDisabel;		
	m_btncr.crTxtFocus		= crTxtFocus;
	m_btncr.crSubTxtDown	= crSubTxtDown;

	if(bRefresh)
	{
		Invalidate();
		UpdateWindow();
	}
}

void CParaBtn::SetParaValue(int v)
{
	bool bRedraw=false;
	if(m_v.iValue!=v)
		bRedraw=true;

	m_v.iValue=v;
	if(bRedraw)
	{
		Invalidate();
		UpdateWindow();
	}

}

void CParaBtn::SetValue(fVALUE v, bool bRedraw) 
{
	memcpy(&m_v,&v,sizeof(fVALUE));

	if(bRedraw)
	{
		//Invalidate();
		//UpdateWindow();
		RefreshBtn();
	}
}

void CParaBtn::RefreshBtnState()
{
	CClientDC dc(this);
	bool bStateChanged=false;

	if(		m_btn.wID==IDC_BTN_PARA_ITIME
		||	m_btn.wID==IDC_BTN_PARA_ETIME
		||	m_btn.wID==IDC_BTN_PARA_BPM)
	{
		int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
		if(iIE<100)
		{
			if(m_bKeyValueAccepted==false)
			{
				if(m_bDrawKey==false)
					bStateChanged=true;
				m_bDrawKey=true;
			}
			if(m_bDrawWarning==false)
				bStateChanged=true;
			m_bDrawWarning=true;
		}
		else
		{
			/*switch(m_btn.wID)
			{
			case IDC_BTN_PARA_ETIME:
				{
					if(GetParent())
						GetParent()->PostMessage(WM_ETIME_CLEARKEY);
				}
				break;
			case IDC_BTN_PARA_ITIME:
				{
					if(GetParent())
						GetParent()->PostMessage(WM_ITIME_CLEARKEY);
				}
				break;
			case IDC_BTN_PARA_BPM:
				{
					if(GetParent())
						GetParent()->PostMessage(WM_BPM_CLEARKEY);
				}
				break;
			}*/

			if(m_bDrawKey==true || m_bKeyValueAccepted==true || m_bDrawWarning==true)
				bStateChanged=true;

			m_bKeyValueAccepted=false;
			m_bDrawKey=false;
			m_bDrawWarning=false;

		}
	}

	if(bStateChanged)
	{
		Invalidate();
		UpdateWindow();
	}

	/*Invalidate();
	UpdateWindow();*/
	

	/*m_btn.pcBmpUp->Draw(m_hDC);
	Draw(BTN_DW_UP);
	BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);*/
}

void CParaBtn::RefreshBtn()
{
	CClientDC dc(this);

	if(		m_btn.wID==IDC_BTN_PARA_ITIME
		||	m_btn.wID==IDC_BTN_PARA_ETIME
		||	m_btn.wID==IDC_BTN_PARA_BPM)
	{
		int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
		if(iIE<100)
		{
			if(m_bKeyValueAccepted==false)
			{
				m_bDrawKey=true;
			}
			m_bDrawWarning=true;
		}
		else
		{
			/*switch(m_btn.wID)
			{
			case IDC_BTN_PARA_ETIME:
				{
					if(GetParent())
						GetParent()->PostMessage(WM_ETIME_CLEARKEY);
				}
				break;
			case IDC_BTN_PARA_ITIME:
				{
					if(GetParent())
						GetParent()->PostMessage(WM_ITIME_CLEARKEY);
				}
				break;
			case IDC_BTN_PARA_BPM:
				{
					if(GetParent())
						GetParent()->PostMessage(WM_BPM_CLEARKEY);
				}
				break;
			}*/
			m_bKeyValueAccepted=false;
			m_bDrawKey=false;
			m_bDrawWarning=false;

		}
	}

	Invalidate();
	UpdateWindow();
	

	/*m_btn.pcBmpUp->Draw(m_hDC);
	Draw(BTN_DW_UP);
	BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);*/
}

bool CParaBtn::DisableValueText(bool bDisable)
{
	m_bDisableText=bDisable;
	Invalidate();
	UpdateWindow();
	return true;
}

void CParaBtn::SetValueText(TCHAR* pszText,bool bTextOnly) 
{
	_tcscpy_s(m_pszValueText,_countof(m_pszValueText),pszText);
	m_bTextOnly=bTextOnly;
}
void CParaBtn::SetValueText(CStringW& szText,bool bTextOnly) 
{
	_tcscpy_s(m_pszValueText,_countof(m_pszValueText),szText);
	m_bTextOnly=bTextOnly;
}
void CParaBtn::SetNameText(TCHAR* pszText, bool bRefresh)
{
	_tcscpy_s(m_pszNameText,_countof(m_pszNameText),pszText);
	if(bRefresh)
	{
		RefreshBtn();
	}
}
void CParaBtn::SetNameText(CStringW& szText, bool bRefresh)
{
	_tcscpy_s(m_pszNameText,_countof(m_pszNameText),szText);
	if(bRefresh)
	{
		RefreshBtn();
	}
}
void CParaBtn::SetNameNoteText(TCHAR* pszText, bool bRefresh)
{
	_tcscpy_s(m_pszNameNoteText,_countof(m_pszNameNoteText),pszText);

	if(bRefresh)
	{
		RefreshBtn();
	}
}
void CParaBtn::SetNameNoteText(CStringW& szText, bool bRefresh)
{
	_tcscpy_s(m_pszNameNoteText,_countof(m_pszNameNoteText),szText);

	if(bRefresh)
	{
		RefreshBtn();
	}
}
void CParaBtn::SetUnitText(TCHAR* pszText)
{
	_tcscpy_s(m_pszUnitText,_countof(m_pszUnitText),pszText);
}
void CParaBtn::SetUnitText(CStringW& szText)
{
	_tcscpy_s(m_pszUnitText,_countof(m_pszUnitText),szText);
}

void CParaBtn::RefreshValueText(CStringW& szText,bool bTextOnly) 
{
	SetValueText(szText,bTextOnly);
	Invalidate();
	UpdateWindow();
}

bool CParaBtn::SetLimits(int nLower,int nUpper)
{
	m_v.iLowerLimit=nLower;
	m_v.iUpperLimit=nUpper;
	return true;
}

void CParaBtn::DrawDirectUp(bool bCheckValueChanged)
{
	m_bDepressed=false;
	m_bTurnedOff=false;
	
	CClientDC dc(this);

	m_btn.pcBmpUp->Draw(m_hDC);
	Draw(BTN_DW_UP);
	BitBlt(dc.m_hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
}

// **************************************************************************
// Painting
// **************************************************************************
void CParaBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	//int iID = m_btn.wID;
	if (m_bDepressed && !(lpDrawItemStruct->itemState & ODS_DISABLED))
	{
		// Button down
		if(lpDrawItemStruct->itemState & ODS_FOCUS)	
		{
			m_bHasFocus=false;
			m_btn.pcBmpDown->Draw(m_hDC);
			Draw(BTN_DW_DW);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
		else
		{
			m_bHasFocus=false;
			m_btn.pcBmpUp->Draw(m_hDC);
			Draw(BTN_DW_UP);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
	}
	else if(lpDrawItemStruct->itemState & ODS_DISABLED )
	{
		// Button disabled
		if(!m_bDepressed)//up
		{
			m_btn.pcBmpDisabled->Draw(m_hDC);
			//m_btn.pcBmpUp->Draw(m_hDC);
			Draw(BTN_DW_DIS);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
		else // down
		{
			m_btn.pcBmpDisabled->Draw(m_hDC);
			//m_btn.pcBmpUp->Draw(m_hDC);
			Draw(BTN_DW_DIS);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
	}
	else	
	{
		// Button up
		if(lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			m_bHasFocus=true;
			m_btn.pcBmpFocus->Draw(m_hDC);
			Draw(BTN_DW_FOCUS);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
		else
		{
			m_bHasFocus=false;
			m_btn.pcBmpUp->Draw(m_hDC);
			Draw(BTN_DW_UP);
			BitBlt(lpDrawItemStruct->hDC,0,0,m_rcClient.right,m_rcClient.bottom,m_hDC,0,0,SRCCOPY);
		}
	}	
}

void CParaBtn::Draw(int nState)
{
	if(!m_bDisableText)
	{
		HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf31AcuBoldNum);
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
			break;
		case BTN_DW_DIS:
		default:
			nTxtColor=m_btncr.crTxtDisabled;
			break;
		}
		int nPrevTxtColor=SetTextColor(m_hDC,nTxtColor);
		int nBkMode=SetBkMode(m_hDC,TRANSPARENT);

		RECT rc;
		memcpy(&rc,&m_rcClient,sizeof(RECT));

		rc.top = 5;

		if(m_btn.dwFormat&DT_RIGHT)
			rc.right-=m_nOffset;
		else if(!(m_btn.dwFormat&DT_CENTER)) // left
			rc.left+=m_nOffset;

		TCHAR psz[MAX_PATH];
		wsprintf(psz,_T("%d"),m_v.iValue);
		DrawText(m_hDC,psz,-1,&rc,m_btn.dwFormat);

		rc.top = m_rcClient.top;
		rc.bottom = 30;
		SelectObject(m_hDC,g_hf14AcuBold);
		DrawText(m_hDC,m_pszNameText,-1,&rc,m_btn.dwFormat);

		rc.bottom = m_rcClient.bottom-8;
		SelectObject(m_hDC,g_hf7AcuNorm);
		DrawText(m_hDC,m_pszUnitText,-1,&rc,DT_BOTTOM|DT_SINGLELINE|DT_CENTER);

		SetTextColor(m_hDC,nPrevTxtColor);
		SetBkMode(m_hDC,nBkMode);
		SelectObject(m_hDC,hPrevFont);
	}
}

// **************************************************************************
// 
// **************************************************************************
BOOL CParaBtn::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
		{
			if(GetParent())
				GetParent()->PostMessage(WM_SET_PARATIMER);

			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_CURRENT_VIEWID,IDC_VIEW_PARABTN,0);
		}
		break;
	default:
		{

		}
		break;
	}
	return CButton::PreTranslateMessage(pMsg);
}

// **************************************************************************
// Get the button "value" and state
// **************************************************************************
bool CParaBtn::GetButton(int* pnValue) 
{
	if(pnValue)
	{
		*pnValue=m_v.iValue;
	}
	return m_bDepressed;
}

// **************************************************************************
// 
// **************************************************************************
void CParaBtn::SetUDKeys(WORD kUP, WORD kDown, WORD kSpace)
{
	m_kUp=kUP;
	m_kDown=kDown;
	m_kSpace=kSpace;
}

// **************************************************************************
// 
// **************************************************************************
int CParaBtn::GetBtnId()
{
	return m_btn.wID;
}

// **************************************************************************
// Button is very slow - this code permforms speed up - but
//              use of double click is not possible any more
// **************************************************************************
void CParaBtn::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	DWORD lParam;
	WORD  po[2]={(WORD)point.x,(WORD)point.y};
	memcpy(&lParam,po,sizeof(DWORD));

	SendMessage(WM_LBUTTONDOWN,nFlags,lParam);
	CButton::OnLButtonDblClk(nFlags, point);
}

void CParaBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(!m_bLMouseButtonDown)
	{
		getModel()->getCONFIG()->SetLastSelectedParaViewBtns(m_btn.wID);

		m_bLMouseButtonDown = true;
		CButton::OnLButtonDown(nFlags, point);
	}
}

void CParaBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_bLMouseButtonDown )
	{
		if(ChildWindowFromPoint(point)==FromHandle(this->m_hWnd))
		{
			if(m_bDepressed)
			{
				if(		m_btn.wID==IDC_BTN_PARA_ITIME
					||	m_btn.wID==IDC_BTN_PARA_ETIME
					||	m_btn.wID==IDC_BTN_PARA_BPM)
				{
					int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
					if(iIE<100)
					{
						/*switch(m_btn.wID)
						{
						case IDC_BTN_PARA_ETIME:
							{
								if(GetParent())
									GetParent()->PostMessage(WM_ETIME_CLEARKEY);
							}
							break;
						case IDC_BTN_PARA_ITIME:
							{
								if(GetParent())
									GetParent()->PostMessage(WM_ITIME_CLEARKEY);
							}
							break;
						case IDC_BTN_PARA_BPM:
							{
								if(GetParent())
									GetParent()->PostMessage(WM_BPM_CLEARKEY);
							}
							break;
						}*/
						//m_bKeyValueAccepted=true;
						m_bDrawKey=false;
						m_bDrawWarning=true;
					}
					else
					{
						m_bDrawKey=false;
						m_bKeyValueAccepted=false;
						m_bDrawWarning=false;
					}
					
				}
				else if(m_btn.wID==IDC_BTN_PARA_PINSP)
				{
					if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPInspMaxKey())
					{
						//m_bKeyValueAccepted=true;
						/*if(GetParent())
							GetParent()->PostMessage(WM_PINSP_CLEARKEY);*/
						m_bDrawKey=false;
						m_bDrawWarning=true;
					}
					else
					{
						m_bDrawKey=false;
						m_bKeyValueAccepted=false;
						m_bDrawWarning=false;
					}
				}
				else if(m_btn.wID==IDC_BTN_PARA_PDUO)
				{
					if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPManualMaxKey())
					{
						//m_bKeyValueAccepted=true;
						/*if(GetParent())
							GetParent()->PostMessage(WM_PDUO_CLEARKEY);*/
						m_bDrawKey=false;
						m_bDrawWarning=true;
					}
					else
					{
						m_bDrawKey=false;
						m_bKeyValueAccepted=false;
						m_bDrawWarning=false;
					}
				}
				else if(m_btn.wID==IDC_BTN_PARA_THERAPYFLOW)
				{
					if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentTherapieFlowMaxKey())
					{
						//m_bKeyValueAccepted=true;
						/*if(GetParent())
							GetParent()->PostMessage(WM_THERAPYFLOW_CLEARKEY);*/
						m_bDrawKey=false;
						m_bDrawWarning=true;
					}
					else
					{
						m_bDrawKey=false;
						m_bKeyValueAccepted=false;
						m_bDrawWarning=false;
					}
				}
				else if(m_btn.wID==IDC_BTN_PARA_PMANUAL)
				{
					if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPManualMaxKey())
					{
						//m_bKeyValueAccepted=true;
						/*if(GetParent())
							GetParent()->PostMessage(WM_PMAN_CLEARKEY);*/
						m_bDrawKey=false;
						m_bDrawWarning=true;
					}
					else
					{
						m_bDrawKey=false;
						m_bKeyValueAccepted=false;
						m_bDrawWarning=false;
					}
				}
				else if(m_btn.wID==IDC_BTN_PARA_FOT_PMEANHIGH)
				{
					if(m_v.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxKey())
					{
						//m_bKeyValueAccepted=true;
						/*if(GetParent())
							GetParent()->PostMessage(WM_PMAN_CLEARKEY);*/
						m_bDrawKey=false;
						m_bDrawWarning=true;
					}
					else
					{
						m_bDrawKey=false;
						m_bKeyValueAccepted=false;
						m_bDrawWarning=false;
					}
				}
				else if(m_btn.wID==IDC_BTN_PARA_FOT_PMEANLOW)
				{
					if(m_v.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxKey())
					{
						//m_bKeyValueAccepted=true;
						/*if(GetParent())
							GetParent()->PostMessage(WM_PMAN_CLEARKEY);*/
						m_bDrawKey=false;
						m_bDrawWarning=true;
					}
					else
					{
						m_bDrawKey=false;
						m_bKeyValueAccepted=false;
						m_bDrawWarning=false;
					}
				}
				else if(m_btn.wID==IDC_BTN_PARA_FOT_PMEANHIGH)
				{
					if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPManualMaxKey())
					{
						//m_bKeyValueAccepted=true;
						/*if(GetParent())
							GetParent()->PostMessage(WM_PMAN_CLEARKEY);*/
						m_bDrawKey=false;
						m_bDrawWarning=true;
					}
					else
					{
						m_bDrawKey=false;
						m_bKeyValueAccepted=false;
						m_bDrawWarning=false;
					}
				}
				else if(m_btn.wID==IDC_BTN_PARA_FOT_PMEANLOW)
				{
					if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPManualMaxKey())
					{
						//m_bKeyValueAccepted=true;
						/*if(GetParent())
							GetParent()->PostMessage(WM_PMAN_CLEARKEY);*/
						m_bDrawKey=false;
						m_bDrawWarning=true;
					}
					else
					{
						m_bDrawKey=false;
						m_bKeyValueAccepted=false;
						m_bDrawWarning=false;
					}
				}
				else if(m_btn.wID==IDC_BTN_PARA_PPSV)
				{
					if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPpsvMaxKey())
						{
							//m_bKeyValueAccepted=true;
							/*if(GetParent())
								GetParent()->PostMessage(WM_PPSV_CLEARKEY);*/
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bDrawKey=false;
							m_bKeyValueAccepted=false;
							m_bDrawWarning=false;
						}
					}
					else
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPpsvMaxKey())
						{
							//m_bKeyValueAccepted=true;
							/*if(GetParent())
								GetParent()->PostMessage(WM_PPSV_CLEARKEY);*/
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bDrawKey=false;
							m_bKeyValueAccepted=false;
							m_bDrawWarning=false;
						}
					}

				}
				else if(m_btn.wID==IDC_BTN_PARA_CPAP)
				{
					if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentCPAPMaxKey())
					{
						//m_bKeyValueAccepted=true;
						/*if(GetParent())
							GetParent()->PostMessage(WM_CPAP_CLEARKEY);*/
						m_bDrawKey=false;
						m_bDrawWarning=true;
					}
					else
					{
						m_bDrawKey=false;
						m_bKeyValueAccepted=false;
						m_bDrawWarning=false;
					}
				}
				else if(m_btn.wID==IDC_BTN_PARA_PEEP)
				{
					if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPeepMaxKey())
					{
						//m_bKeyValueAccepted=true;
						/*if(GetParent())
							GetParent()->PostMessage(WM_PEEP_CLEARKEY);*/
						m_bDrawKey=false;
						m_bDrawWarning=true;
					}
					else if(m_v.iValue<getModel()->getDATAHANDLER()->GetCurrentPeepMinKey())
					{
						/*if(GetParent())
							GetParent()->PostMessage(WM_PEEP_CLEARKEY);*/
						m_bDrawKey=false;
						m_bDrawWarning=true;
					}
					else
					{
						m_bDrawKey=false;
						m_bKeyValueAccepted=false;
						m_bDrawWarning=false;
					}
				}
				else if(m_btn.wID==IDC_BTN_PARA_VG_VL)
				{
					if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
						||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentVgarantMaxKey())
						{
							//m_bKeyValueAccepted=true;
							/*if(GetParent())
								GetParent()->PostMessage(WM_VGARANT_CLEARKEY);*/
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bDrawKey=false;
							m_bKeyValueAccepted=false;
							m_bDrawWarning=false;
						}
					}
					else if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() 
						&& getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentVlimitMaxKey())
						{
							//m_bKeyValueAccepted=true;
							/*if(GetParent())
								GetParent()->PostMessage(WM_VLIMIT_CLEARKEY);*/
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bDrawKey=false;
							m_bKeyValueAccepted=false;
							m_bDrawWarning=false;
						}
					}
					else if(getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
						&& getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentVgarantMaxKey())
						{
							//m_bKeyValueAccepted=true;
							/*if(GetParent())
								GetParent()->PostMessage(WM_VGARANT_CLEARKEY);*/
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bDrawKey=false;
							m_bKeyValueAccepted=false;
							m_bDrawWarning=false;
						}
					}
				}
				//else if(m_btn.wID==IDC_BTN_PARA_VLIMIT)
				//{
				//	if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentVlimitMaxKey())
				//	{
				//		//m_bKeyValueAccepted=true;
				//		if(GetParent())
				//			GetParent()->PostMessage(WM_VLIMIT_CLEARKEY);
				//		m_bDrawKey=false;
				//		m_bDrawWarning=true;
				//	}
				//	else
				//	{
				//		m_bDrawKey=false;
				//		m_bKeyValueAccepted=false;
				//		m_bDrawWarning=false;
				//	}
				//}
				//else if(m_btn.wID==IDC_BTN_PARA_VGARANT)
				//{
				//	if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentVgarantMaxKey())
				//	{
				//		//m_bKeyValueAccepted=true;
				//		if(GetParent())
				//			GetParent()->PostMessage(WM_VGARANT_CLEARKEY);
				//		m_bDrawKey=false;
				//		m_bDrawWarning=true;
				//	}
				//	else
				//	{
				//		m_bDrawKey=false;
				//		m_bKeyValueAccepted=false;
				//		m_bDrawWarning=false;
				//	}
				//}
				//else if(m_btn.wID==IDC_BTN_PARA_HFAMPL)
				//{
				//	if(m_v.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFampMaxKey())
				//	{
				//		//m_bKeyValueAccepted=true;
				//		if(GetParent())
				//			GetParent()->PostMessage(WM_HFAMP_CLEARKEY);
				//		m_bDrawKey=false;
				//		m_bDrawWarning=true;
				//	}
				//	else
				//	{
				//		m_bDrawKey=false;
				//		m_bKeyValueAccepted=false;
				//		m_bDrawWarning=false;
				//	}
				//}
				/*else if(m_btn.wID==IDC_BTN_PARA_HFFREQ)
				{
					if(m_v.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFfreqMaxKey())
					{
						//m_bKeyValueAccepted=true;
						if(GetParent())
						GetParent()->PostMessage(WM_HFFREQ_CLEARKEY);
						m_bDrawKey=false;
						m_bDrawWarning=true;
					}
					else
					{
					m_bDrawKey=false;
					m_bKeyValueAccepted=false;
					m_bDrawWarning=false;
					}
				}*/
				else if(m_btn.wID==IDC_BTN_PARA_PMITT)
				{
					if(m_v.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxKey())
					{
						//m_bKeyValueAccepted=true;
						/*if(GetParent())
							GetParent()->PostMessage(WM_PMEAN_CLEARKEY);*/
						m_bDrawKey=false;
						m_bDrawWarning=true;
					}
					else
					{
						m_bDrawKey=false;
						m_bKeyValueAccepted=false;
						m_bDrawWarning=false;
					}
				}
				else if(m_btn.wID==IDC_BTN_PARA_PMEAN_REC)
				{
					if(m_v.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecMaxKey())
					{
						//m_bKeyValueAccepted=true;
						/*if(GetParent())
							GetParent()->PostMessage(WM_PMEANREC_CLEARKEY);*/
						m_bDrawKey=false;
						m_bDrawWarning=true;
					}
					else
					{
						m_bDrawKey=false;
						m_bKeyValueAccepted=false;
						m_bDrawWarning=false;
					}
				}
				else if(m_btn.wID==IDC_BTN_PARA_O2 || m_btn.wID==IDC_BTN_PARA_O2FLUSH)
				{
					getModel()->DisablePatAlarmO2dependend();
					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_DISABLE_PATALARM_O2);
				}
				WriteCurrentValue();
				if(GetParent())
					GetParent()->PostMessage(WM_SET_PARATIMER,m_btn.wID);
			}
			else
			{
				m_iPrevValue=m_v.iValue;
				m_bLastParaState=m_bOff;
				if(GetParent())
					GetParent()->PostMessage(WM_SET_PARATIMER);
			}
			m_bDepressed = !m_bDepressed;

			if(!m_bDepressed)
				m_bTurnedOff=false;
			
		}
		/*SetFocus();*/
		//int iTest = GetBtnId();
		CButton::OnLButtonUp(nFlags, point);
	}
	m_bLMouseButtonDown = false;
}
void CParaBtn::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( !m_bLMouseButtonDown /*&& !m_bAutoState*/)
	{
		m_bLMouseButtonDown = true;
		if(nChar==32 /*&& !m_bAutoState*/)	//VK_SPACE
		{
			if(!isSafeTickCountDelayExpired(m_dwPushDelta, BTN_DELAY))
			{
				return;
			}

			m_dwPushDelta=GetTickCount();

			if(m_bDepressed)
			{
				if(m_bWaitConfirm)
				{
					m_iPrevValue=m_v.iValue;
					m_bLastParaState=m_bOff;

					m_bKeyBeep=TRUE;
					m_bWaitConfirm=false;
					m_bKeyValueAccepted=true;
					m_bDrawKey=false;
					if(		m_btn.wID==IDC_BTN_PARA_ITIME
						||	m_btn.wID==IDC_BTN_PARA_ETIME
						||	m_btn.wID==IDC_BTN_PARA_BPM)
					{
						if(GetParent())
							GetParent()->PostMessage(WM_IE_VALUE_CONFIRMED);
					}
				}
				else
				{
					if(		m_btn.wID==IDC_BTN_PARA_ITIME
					||	m_btn.wID==IDC_BTN_PARA_ETIME
					||	m_btn.wID==IDC_BTN_PARA_BPM)
					{
						int iIE = getModel()->getDATAHANDLER()->GetCurrentIERelation100();
						if(iIE<100)
						{
							/*switch(m_btn.wID)
							{
							case IDC_BTN_PARA_ETIME:
								{
									if(GetParent())
										GetParent()->PostMessage(WM_ETIME_CLEARKEY);
								}
								break;
							case IDC_BTN_PARA_ITIME:
								{
									if(GetParent())
										GetParent()->PostMessage(WM_ITIME_CLEARKEY);
								}
								break;
							case IDC_BTN_PARA_BPM:
								{
									if(GetParent())
										GetParent()->PostMessage(WM_BPM_CLEARKEY);
								}
								break;
							}*/
							m_bKeyValueAccepted=true;
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bKeyValueAccepted=false;
							m_bDrawKey=false;
							m_bDrawWarning=false;
						}
						
					}
					else if(m_btn.wID==IDC_BTN_PARA_PINSP)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPInspMaxKey())
						{
							m_bKeyValueAccepted=true;
							/*if(GetParent())
								GetParent()->PostMessage(WM_PINSP_CLEARKEY);*/
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bKeyValueAccepted=false;
							m_bDrawKey=false;
							m_bDrawWarning=false;
						}
					}
					else if(m_btn.wID==IDC_BTN_PARA_PDUO)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPManualMaxKey())
						{
							m_bKeyValueAccepted=true;
							/*if(GetParent())
								GetParent()->PostMessage(WM_PDUO_CLEARKEY);*/
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bKeyValueAccepted=false;
							m_bDrawKey=false;
							m_bDrawWarning=false;
						}
					}
					else if(m_btn.wID==IDC_BTN_PARA_THERAPYFLOW)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentTherapieFlowMaxKey())
						{
							m_bKeyValueAccepted=true;
							/*if(GetParent())
								GetParent()->PostMessage(WM_THERAPYFLOW_CLEARKEY);*/
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bKeyValueAccepted=false;
							m_bDrawKey=false;
							m_bDrawWarning=false;
						}
					}
					else if(m_btn.wID==IDC_BTN_PARA_PMANUAL)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPManualMaxKey())
						{
							m_bKeyValueAccepted=true;
							/*if(GetParent())
								GetParent()->PostMessage(WM_PMAN_CLEARKEY);*/
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bKeyValueAccepted=false;
							m_bDrawKey=false;
							m_bDrawWarning=false;
						}
					}
					else if(m_btn.wID==IDC_BTN_PARA_FOT_PMEANHIGH)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxKey())
						{
							m_bKeyValueAccepted=true;
							/*if(GetParent())
								GetParent()->PostMessage(WM_PMAN_CLEARKEY);*/
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bKeyValueAccepted=false;
							m_bDrawKey=false;
							m_bDrawWarning=false;
						}
					}
					else if(m_btn.wID==IDC_BTN_PARA_FOT_PMEANLOW)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxKey())
						{
							m_bKeyValueAccepted=true;
							/*if(GetParent())
								GetParent()->PostMessage(WM_PMAN_CLEARKEY);*/
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bKeyValueAccepted=false;
							m_bDrawKey=false;
							m_bDrawWarning=false;
						}
					}
					else if(m_btn.wID==IDC_BTN_PARA_FOT_PEEPHIGH)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPManualMaxKey())
						{
							m_bKeyValueAccepted=true;
							/*if(GetParent())
								GetParent()->PostMessage(WM_PMAN_CLEARKEY);*/
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bKeyValueAccepted=false;
							m_bDrawKey=false;
							m_bDrawWarning=false;
						}
					}
					else if(m_btn.wID==IDC_BTN_PARA_FOT_PEEPLOW)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPManualMaxKey())
						{
							m_bKeyValueAccepted=true;
							/*if(GetParent())
								GetParent()->PostMessage(WM_PMAN_CLEARKEY);*/
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bKeyValueAccepted=false;
							m_bDrawKey=false;
							m_bDrawWarning=false;
						}
					}
					else if(m_btn.wID==IDC_BTN_PARA_PPSV)
					{
						if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
						{
							if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPpsvMaxKey())
							{
								m_bKeyValueAccepted=true;
								/*if(GetParent())
									GetParent()->PostMessage(WM_PPSV_CLEARKEY);*/
								m_bDrawKey=false;
								m_bDrawWarning=true;
							}
							else
							{
								m_bKeyValueAccepted=false;
								m_bDrawKey=false;
								m_bDrawWarning=false;
							}
						}
						else
						{
							if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPpsvMaxKey())
							{
								m_bKeyValueAccepted=true;
								/*if(GetParent())
									GetParent()->PostMessage(WM_PPSV_CLEARKEY);*/
								m_bDrawKey=false;
								m_bDrawWarning=true;
							}
							else
							{
								m_bKeyValueAccepted=false;
								m_bDrawKey=false;
								m_bDrawWarning=false;
							}
						}

					}
					else if(m_btn.wID==IDC_BTN_PARA_CPAP)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentCPAPMaxKey())
						{
							m_bKeyValueAccepted=true;
							/*if(GetParent())
								GetParent()->PostMessage(WM_CPAP_CLEARKEY);*/
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bKeyValueAccepted=false;
							m_bDrawKey=false;
							m_bDrawWarning=false;
						}
					}
					else if(m_btn.wID==IDC_BTN_PARA_PEEP)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentPeepMaxKey())
						{
							m_bKeyValueAccepted=true;
							/*if(GetParent())
								GetParent()->PostMessage(WM_PEEP_CLEARKEY);*/
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else if(m_v.iValue<getModel()->getDATAHANDLER()->GetCurrentPeepMinKey())
						{
							m_bKeyValueAccepted=true;
							/*if(GetParent())
								GetParent()->PostMessage(WM_PEEP_CLEARKEY);*/
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bKeyValueAccepted=false;
							m_bDrawKey=false;
							m_bDrawWarning=false;
						}
						/*if(FALSE==getModel()->getCONFIG()->isPPSVasDeltaPEEPValue())
							getModel()->getVIEWHANDLER()->setPEEPDifference(m_v.iValue,true);*/
					}
					else if(m_btn.wID==IDC_BTN_PARA_VG_VL)
					{
						if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
							||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
						{
							if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentVgarantMaxKey())
							{
								m_bKeyValueAccepted=true;
								/*if(GetParent())
									GetParent()->PostMessage(WM_VGARANT_CLEARKEY);*/
								m_bDrawKey=false;
								m_bDrawWarning=true;
							}
							else
							{
								m_bKeyValueAccepted=false;
								m_bDrawKey=false;
								m_bDrawWarning=false;
							}
						}
						else if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() 
							&& getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
						{
							if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentVlimitMaxKey())
							{
								m_bKeyValueAccepted=true;
								/*if(GetParent())
									GetParent()->PostMessage(WM_VLIMIT_CLEARKEY);*/
								m_bDrawKey=false;
								m_bDrawWarning=true;
							}
							else
							{
								m_bKeyValueAccepted=false;
								m_bDrawKey=false;
								m_bDrawWarning=false;
							}
						}
						else if(getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
							&& getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
						{
							if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentVgarantMaxKey())
							{
								m_bKeyValueAccepted=true;
								/*if(GetParent())
									GetParent()->PostMessage(WM_VGARANT_CLEARKEY);*/
								m_bDrawKey=false;
								m_bDrawWarning=true;
							}
							else
							{
								m_bKeyValueAccepted=false;
								m_bDrawKey=false;
								m_bDrawWarning=false;
							}
						}
					}
					/*else if(m_btn.wID==IDC_BTN_PARA_VLIMIT)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentVlimitMaxKey())
						{
							m_bKeyValueAccepted=true;
							if(GetParent())
								GetParent()->PostMessage(WM_VLIMIT_CLEARKEY);
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bKeyValueAccepted=false;
							m_bDrawKey=false;
							m_bDrawWarning=false;
						}
					}
					else if(m_btn.wID==IDC_BTN_PARA_VGARANT)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->GetCurrentVgarantMaxKey())
						{
							m_bKeyValueAccepted=true;
							if(GetParent())
								GetParent()->PostMessage(WM_VGARANT_CLEARKEY);
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bKeyValueAccepted=false;
							m_bDrawKey=false;
							m_bDrawWarning=false;
						}
					}*/
					/*else if(m_btn.wID==IDC_BTN_PARA_HFAMPL)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFampMaxKey())
						{
							m_bKeyValueAccepted=true;
							if(GetParent())
								GetParent()->PostMessage(WM_HFAMP_CLEARKEY);
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bKeyValueAccepted=false;
							m_bDrawKey=false;
							m_bDrawWarning=false;
						}
					}*/
					/*else if(m_btn.wID==IDC_BTN_PARA_HFFREQ)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->HFO()->GetHFfreqMaxKey())
						{
							m_bKeyValueAccepted=true;
							if(GetParent())
							GetParent()->PostMessage(WM_HFFREQ_CLEARKEY);
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
						m_bKeyValueAccepted=false;
						m_bDrawKey=false;
						m_bDrawWarning=false;
						}
					}*/
					else if(m_btn.wID==IDC_BTN_PARA_PMITT)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxKey())
						{
							m_bKeyValueAccepted=true;
							/*if(GetParent())
								GetParent()->PostMessage(WM_PMEAN_CLEARKEY);*/
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bKeyValueAccepted=false;
							m_bDrawKey=false;
							m_bDrawWarning=false;
						}

						//PMAN1
						if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true)
							getModel()->getVIEWHANDLER()->setPmeanDifference(m_v.iValue,true);
					}
					else if(m_btn.wID==IDC_BTN_PARA_PMEAN_REC)
					{
						if(m_v.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecMaxKey())
						{
							m_bKeyValueAccepted=true;
							/*if(GetParent())
								GetParent()->PostMessage(WM_PMEANREC_CLEARKEY);*/
							m_bDrawKey=false;
							m_bDrawWarning=true;
						}
						else
						{
							m_bKeyValueAccepted=false;
							m_bDrawKey=false;
							m_bDrawWarning=false;
						}
					}
					else if(m_btn.wID==IDC_BTN_PARA_O2 || m_btn.wID==IDC_BTN_PARA_O2FLUSH)
					{
						getModel()->DisablePatAlarmO2dependend();
						if(AfxGetApp() != NULL)
							AfxGetApp()->GetMainWnd()->PostMessage(WM_DISABLE_PATALARM_O2);
					}
					WriteCurrentValue();
					if(GetParent())
						GetParent()->PostMessage(WM_SET_PARATIMER,m_btn.wID);
					m_bDepressed = !m_bDepressed;
				}
			}
			else
			{
				m_iPrevValue=m_v.iValue;
				m_bLastParaState=m_bOff;
				if(GetParent())
					GetParent()->PostMessage(WM_SET_PARATIMER);

				m_bDepressed = !m_bDepressed;
			}
			
			

			if(!m_bDepressed)
				m_bTurnedOff=false;
		}
		CButton::OnKeyDown(nChar,nRepCnt,nFlags);
	}
}

void CParaBtn::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_dwPushDelta=GetTickCount();

	if(m_bDepressed)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_SET_PARATIMER,m_btn.wID);
	}

	switch(getModel()->getCONFIG()->GetCurMode())
	{
	case VM_PRE_IPPV:
	case VM_PRE_PSV:
	case VM_PRE_SIMV:
	case VM_PRE_SIMVPSV:
	case VM_PRE_SIPPV:
	case VM_PRE_CPAP:
	case VM_PRE_DUOPAP:
	case VM_PRE_NCPAP:
	case VM_PRE_HFO:
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_VENTMODE_HOME);
		break;
	default:
		break;
	}
	CButton::OnKeyUp(nChar,nRepCnt,nFlags);
	
}

void CParaBtn::OnKillFocus(CWnd* pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);
	Depress();
}



bool CParaBtn::IsOff()
{
	return m_bOff;
}

void CParaBtn::SetOffState(bool state)
{
	if(m_bOff==state)
		return;
	m_bOff=state;
	Invalidate();
	UpdateWindow();
}

//void CParaBtn::SetAuto(bool bAuto,bool bRedraw)
//{
//	m_bAutoState=bAuto;
//
//	if(bRedraw)
//	{
//		Invalidate();
//		UpdateWindow();
//	}
//}

void CParaBtn::WriteCurrentValue()
{
	bool bStopFOT=true;
	if(theApp.IsScreenlock())
	{
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_KILL_AUTOSCREENLOG_TIMER);
	}

	/*if(true==getModel()->getDATAHANDLER()->getFOTstate())
	{
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_STOP_FOT);
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_TXT_STOP_FOT);
	}*/


	if(m_btn.wID==IDC_BTN_PARA_ITIME)
	{
		if(getModel()->getCONFIG()->GetIERelationMode()==RM_SET_ItoBPM)
		{
			if(false==getModel()->getDATAHANDLER()->CalculateETime_ITimeChanged(m_v.iValue,false))
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0016 %d"),m_v.iValue);
				theApp.getLog()->WriteLine(sz);
				m_v.iValue=getModel()->getDATAHANDLER()->GetCurrentITimePara();

				if(false==getModel()->getDATAHANDLER()->CalculateETime_ITimeChanged(m_v.iValue,false))
				{
					sz.Format(_T("#HFO:0017 %d"),m_v.iValue);
					theApp.getLog()->WriteLine(sz);

					getModel()->getDATAHANDLER()->CheckIErelation(	getModel()->getDATAHANDLER()->GetCurrentITimePara(),
																	getModel()->getDATAHANDLER()->GetCurrentBPMPara(),
																	getModel()->getDATAHANDLER()->GetCurrentETimePara());
				}
			}
		}
		else
		{
			if(false==getModel()->getDATAHANDLER()->CalculateBPM_ITimeChanged(m_v.iValue,false))
			{
				CStringW sz=_T("");
				sz.Format(_T("#HFO:0018 %d"),m_v.iValue);
				theApp.getLog()->WriteLine(sz);
				m_v.iValue=getModel()->getDATAHANDLER()->GetCurrentITimePara();
				
				if(false==getModel()->getDATAHANDLER()->CalculateBPM_ITimeChanged(m_v.iValue,false))
				{
					sz.Format(_T("#HFO:0019 %d"),m_v.iValue);
					theApp.getLog()->WriteLine(sz);

					getModel()->getDATAHANDLER()->CheckIErelation(	getModel()->getDATAHANDLER()->GetCurrentITimePara(),
																	getModel()->getDATAHANDLER()->GetCurrentBPMPara(),
																	getModel()->getDATAHANDLER()->GetCurrentETimePara());
				}
			}
		}
		if(GetParent())
			GetParent()->PostMessage(WM_ITIME_CHANGED);
	}
	else if(m_btn.wID==IDC_BTN_PARA_ETIME)
	{
		if(false==getModel()->getDATAHANDLER()->CalculateBPM_ETimeChanged(m_v.iValue,true))
		{
			CStringW sz=_T("");
			sz.Format(_T("#HFO:0020 %d"),m_v.iValue);
			theApp.getLog()->WriteLine(sz);
			m_v.iValue=getModel()->getDATAHANDLER()->GetCurrentETimePara();

			if(false==getModel()->getDATAHANDLER()->CalculateBPM_ETimeChanged(m_v.iValue,true))
			{
				sz.Format(_T("#HFO:0021 %d"),m_v.iValue);
				theApp.getLog()->WriteLine(sz);

				getModel()->getDATAHANDLER()->CheckIErelation(	getModel()->getDATAHANDLER()->GetCurrentITimePara(),
																getModel()->getDATAHANDLER()->GetCurrentBPMPara(),
																getModel()->getDATAHANDLER()->GetCurrentETimePara());

			}
		}
		if(GetParent())
			GetParent()->PostMessage(WM_ETIME_CHANGED);
	}
	else if(m_btn.wID==IDC_BTN_PARA_BPM)
	{
		if(false==getModel()->getDATAHANDLER()->CalculateETime_BPMChanged(m_v.iValue,false))
		{
			CStringW sz=_T("");
			sz.Format(_T("#HFO:0022 %d"),m_v.iValue);
			theApp.getLog()->WriteLine(sz);
			m_v.iValue=getModel()->getDATAHANDLER()->GetCurrentBPMPara();

			if(false==getModel()->getDATAHANDLER()->CalculateETime_BPMChanged(m_v.iValue,false))
			{
				sz.Format(_T("#HFO:0023 %d"),m_v.iValue);
				theApp.getLog()->WriteLine(sz);

				getModel()->getDATAHANDLER()->CheckIErelation(	getModel()->getDATAHANDLER()->GetCurrentITimePara(),
																getModel()->getDATAHANDLER()->GetCurrentBPMPara(),
																getModel()->getDATAHANDLER()->GetCurrentETimePara());
			}
		}
		if(GetParent())
			GetParent()->PostMessage(WM_BPM_CHANGED);
	}


	switch(m_btn.wID)
	{
	case IDC_BTN_PARA_MANBREATH:
		{
			getModel()->getCONFIG()->SetManBreathTime(m_v.iValue);
			bStopFOT=false;
		}
		break;
	case IDC_BTN_PARA_FLUSHTIME:
		{
			getModel()->getDATAHANDLER()->SetCurO2FlushTime(m_v.iValue);
			bStopFOT=false;
		}
		break;
	case IDC_BTN_PARA_ABORTPSV:
		{
			getModel()->getCONFIG()->SetPercentAbortCriterionPSV(m_v.iValue);
			getModel()->Send_ABORT_CRITERIONPSV(m_v.iValue,false,true);
			bStopFOT=false;
		}
		break;
	case IDC_BTN_PARA_PCURVE:
		{
			getModel()->getCONFIG()->SetCurPressureRiseCtrl((eCurveForm)m_v.iValue);
			getModel()->Send_MODE_OPTION1();
			bStopFOT=false;
		}
		break;
	case IDC_BTN_PARA_RISETIME:
		{
			getModel()->getDATAHANDLER()->SetCurrentRisetimePara(m_v.iValue);
			if(GetParent())
				GetParent()->PostMessage(WM_RISETIME_CHANGED);
		}
		break;
	case IDC_BTN_PARA_HFFLOW:
		{
			getModel()->getDATAHANDLER()->SetCurrentHFFlowPara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_IFLOW:
		{
			getModel()->getDATAHANDLER()->SetCurrentIFlowPara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_EFLOW:
		{
			getModel()->getDATAHANDLER()->SetEFlowParaData(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_THERAPYFLOW:
		{
			getModel()->getDATAHANDLER()->SetCurrentTherapieFlowParaData(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_PEEP:
		{
			getModel()->getDATAHANDLER()->SetCurrentPEEPPara(m_v.iValue);
			if(getModel()->getCONFIG()->GetCurMode()==VM_PSV
				||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_PSV
				||	getModel()->getCONFIG()->GetCurMode()==VM_SIMVPSV
				||	getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMVPSV)
			{
				getModel()->getVIEWHANDLER()->setPEEPvalue(m_v.iValue,true);
			}
		}
		break;
	case IDC_BTN_PARA_PINSP:
		{
			getModel()->getDATAHANDLER()->SetCurrentPINSPPara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_VG_PMAXI:
		{
			getModel()->getDATAHANDLER()->SetCurrentPmaxVolGPara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_VG_AMPMAXI:
		{
			getModel()->getDATAHANDLER()->SetCurrentHFAMPLmaxPara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_PDUO:
		{
			getModel()->getDATAHANDLER()->SetCurrentPManPara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_PMANUAL:
		{
			getModel()->getDATAHANDLER()->SetCurrentPManPara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_PPSV:
		{
			getModel()->getDATAHANDLER()->SetCurrentPpsvPara(m_v.iValue);
			getModel()->getVIEWHANDLER()->setPpsvValue(m_v.iValue,false);
		}
		break;
	case IDC_BTN_PARA_VG_VL:
		{
			if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO 
				||	getModel()->getCONFIG()->GetCurMode()==VM_HFO)
			{
				getModel()->getALARMHANDLER()->resetVgVlAutoTurnedOff();
				getModel()->getDATAHANDLER()->SetCurrentVGarantParamData(m_v.iValue);
			}
			else if(getModel()->getDATAHANDLER()->isVLIMITLicenseAvailable() 
				&& getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VLIMIT)
			{
				getModel()->getALARMHANDLER()->resetVgVlAutoTurnedOff();
				getModel()->getDATAHANDLER()->SetCurrentVLimitParam(m_v.iValue,getModel()->getDATAHANDLER()->IsCurrentModeVLimitStateOn(),true);
			}
			else if(getModel()->getDATAHANDLER()->isVGUARANTLicenseAvailable() 
				&& getModel()->getCONFIG()->GetVolumeControl()==VCTRL_VGARANT)
			{
				getModel()->getALARMHANDLER()->resetVgVlAutoTurnedOff();
				getModel()->getDATAHANDLER()->SetCurrentVGarantParamData(m_v.iValue);
			}
		}
		break;
	case IDC_BTN_PARA_BPM:
		{
			getModel()->getDATAHANDLER()->SetCurrentBPMPara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_ITIME:
		{
			getModel()->getDATAHANDLER()->SetCurrentITimePara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_O2:
		{
			getModel()->getDATAHANDLER()->SetCurrentO2Para(m_v.iValue);
			bStopFOT=false;
		}
		break;
	case IDC_BTN_PARA_O2FLUSH:
		{
			getModel()->getDATAHANDLER()->SetCurrentO2FlushPara(m_v.iValue);
			bStopFOT=false;
		}
		break;
	case IDC_BTN_PARA_HFAMPL:
		{
			getModel()->getDATAHANDLER()->SetCurrentHFAMPLPara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_HFFREQ:
		{
			getModel()->getDATAHANDLER()->SetCurrentHFFreqPara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_IERATIO:
		{
			getModel()->getDATAHANDLER()->SetCurrentIERatioParaHFO((eRatioIE)m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_PMITT:
		{
			getModel()->getDATAHANDLER()->SetCurrentPMeanPara(m_v.iValue);

			if(getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true)
				getModel()->getVIEWHANDLER()->setPmeanDifference(m_v.iValue,true);
		}
		break;
	case IDC_BTN_PARA_ETIME:
		{
			getModel()->getDATAHANDLER()->SetCurrentETimePara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_CPAP:
		{
			getModel()->getDATAHANDLER()->SetCurrentCPAPPara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_TRIGNMODE:
	case IDC_BTN_PARA_TRIGCONV:
		{
			getModel()->getDATAHANDLER()->SetCurrentTriggerPara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_BACKUP:
		{
			getModel()->getDATAHANDLER()->SetCurrentBackupPara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_FLOWMIN:
		{
			getModel()->getDATAHANDLER()->SetFlowMinParadata(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_ITIME_REC:
		{
			getModel()->getDATAHANDLER()->SetCurrentITimeRecPara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_BPM_REC:
		{
			getModel()->getDATAHANDLER()->SetCurrentFreqRecPara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_PMEAN_REC:
		{
			getModel()->getDATAHANDLER()->SetCurrentHFPMeanRecPara(m_v.iValue);
			if(m_v.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecMaxKey())
			{
				//m_bKeyValueAccepted=true;
				/*if(GetParent())
					GetParent()->PostMessage(WM_PMEANREC_CLEARKEY);*/
				m_bDrawKey=false;
				m_bDrawWarning=true;
				m_bKeyValueAccepted=true;
			}
			else
			{
				m_bDrawKey=false;
				m_bKeyValueAccepted=false;
				m_bDrawWarning=false;
			}
		}
		break;
	//case IDC_BTN_PARA_FOTAMP:
	//	{
	//		getModel()->getDATAHANDLER()->setCurrentFOT_AMPLITUDEPara(m_v.iValue);
	//		/*if(GetParent())
	//			GetParent()->PostMessage(WM_FOTAMP_CHANGED);*/
	//	}
	//	break;
	//case IDC_BTN_PARA_FOTFREQ:
	//	{
	//		getModel()->getDATAHANDLER()->setCurrentFOT_FREQPara(m_v.iValue);
	//	}
	//	break;
	case IDC_BTN_PARA_FOT_STEPS:
		{
			getModel()->getDATAHANDLER()->setCurrentFOT_STEPSPara(m_v.iValue);
			if(GetParent())
				GetParent()->PostMessage(WM_FOTSTEP_CHANGED);
		}
		break;
	case IDC_BTN_PARA_FOT_PMEANLOW:
		{
			getModel()->getDATAHANDLER()->setCurrentFOT_PMEANSTARTPara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_FOT_PMEANHIGH:
		{
			getModel()->getDATAHANDLER()->setCurrentFOT_PMEANENDPara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_FOT_PEEPLOW:
		{
			getModel()->getDATAHANDLER()->setCurrentFOT_PEEPSTARTPara(m_v.iValue);
		}
		break;
	case IDC_BTN_PARA_FOT_PEEPHIGH:
		{
			getModel()->getDATAHANDLER()->setCurrentFOT_PEEPENDPara(m_v.iValue);
		}
		break;
	default:
		break;
	}

	if(bStopFOT && getModel()->getDATAHANDLER()->isFOTLicenseAvailable()==true && getModel()->getFOTThread())
	{
		if(getModel()->getFOTThread()->getFOTstate()!=FOT_OFF)
		{
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_STOP_FOT);
		}
	}
}

// **************************************************************************
// Time measurement
// **************************************************************************
//rkuTICKCOUNT
bool CParaBtn::isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay)////used to check if old tick count plus delay is still lower than actual tickCount, (dwLastTickCount+DELAY<getTickCount64())
{
	bool bExpired=false;
	DWORD curTickCount=GetTickCount();

	if(curTickCount<oldTickCount)//overrun
	{
		DWORD diff=0xFFFFFFFF-oldTickCount;

		if(diff+delay<curTickCount)
		{
			bExpired=true;
		}
	}
	else
	{
		if(oldTickCount+delay<curTickCount)
		{
			bExpired=true;
		}
	}
	return bExpired;
}
