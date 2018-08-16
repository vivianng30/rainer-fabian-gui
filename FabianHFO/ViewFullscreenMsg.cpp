/**********************************************************************************************//**
 * \file	ViewFullscreenMsg.cpp.
 *
 * Implements the view fullscreen message class
 **************************************************************************************************/

#include "StdAfx.h"
#include "ViewFullscreenMsg.h"
#include "stdafx.h"
#include "FabianHFO.h"
#include "LangAdmin.h"
#include "globDefs.h"
#include "MVViewHandler.h"

/**********************************************************************************************//**
 * A macro that defines color txtbtnup
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNUP				0x00000000

/**********************************************************************************************//**
 * A macro that defines color txtbtndw
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNDW				0x00FFFFFF

/**********************************************************************************************//**
 * A macro that defines color txtbtnfc
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

#define COLOR_TXTBTNFC				0x00000000

//extern CLangAdmin* g_pGlobalLanguageStrings;

/**********************************************************************************************//**
 * Initializes a new instance of the CViewFullscreenMsg class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	iViewID	Identifier for the view.
 **************************************************************************************************/

CViewFullscreenMsg::CViewFullscreenMsg(int iViewID):
CMVView(iViewID)
{
	m_bI2Cerror=false;
	m_bSERIALerror=false;
	m_bDraw=true;
	//m_pcLogo=NULL;
	m_pcCaution=NULL;
	m_pcBack_Up=NULL;
	m_pcBack_Dw=NULL;
	m_pcMenu5=NULL;

	m_eViewState=getModel()->getVIEWHANDLER()->getViewState();
}

/**********************************************************************************************//**
 * Finalizes an instance of the CViewFullscreenMsg class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CViewFullscreenMsg::~CViewFullscreenMsg(void)
{
	//delete m_pcLogo;
	//m_pcLogo=NULL;

	delete m_pcCaution;
	m_pcCaution=NULL;

	delete m_pcMenu5;
	m_pcMenu5=NULL;

	delete m_pcBack_Up;
	m_pcBack_Up=NULL;
	delete m_pcBack_Dw;
	m_pcBack_Dw=NULL;
}



BEGIN_MESSAGE_MAP(CViewFullscreenMsg, CMVView)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * Creates the view
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewFullscreenMsg::CreateView()
{
	//RECT rcLd={0,0,800,580};
	RECT rcLd={0,41,800,600};
	//RECT rcLd={0,0,800,600};

	if (!Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_MESSAGE))
	{
		return false;
	}
	
	Initialize();
	SetTimer(STARTUPTIMER,1000,NULL);

	return true;
}

/**********************************************************************************************//**
 * Creates the Window instance that will be represented by this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param [in,out]	pParentWnd	If non-null, the parent window.
 * \param 		  	rc		  	The rectangle.
 * \param 		  	nID		  	The identifier.
 * \param [in,out]	pContext  	If non-null, the context.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CViewFullscreenMsg::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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

		CBrush cbrBack(BACKGND);
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		//m_pcLogo	= new CBmp(theApp.m_hInstance,m_hDC,IDB_PIC_ACULOGO);
		if(m_eViewState==VS_BATTERY)
			m_pcCaution	= new CBmp(theApp.m_hInstance,m_hDC,IDB_BATTERY_LOAD);
		else if(m_eViewState==VS_SYSFAIL)
			m_pcCaution	= new CBmp(theApp.m_hInstance,m_hDC,IDB_SYSFAIL);

		SelectObject(m_hDC,hbrprev);
		SelectObject(m_hDC, hpenprev);
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
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewFullscreenMsg::Initialize()
{
	bool result=false;

	CClientDC dc(this);

	BTN btn;

	m_pcBack_Up		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUM_BACK_UP);
	m_pcBack_Dw		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_NUM_BACK_DW);

	btn.wID					= IDC_BTN_MENU_NUMERIC;	
	btn.poPosition.x		= 630;
	btn.poPosition.y		= 507;//548;//553;
	btn.pcBmpUp				= m_pcBack_Up;
	btn.pcBmpDown			= m_pcBack_Dw;
	btn.pcBmpFocus			= m_pcBack_Up;
	btn.pcBmpDisabled		= m_pcBack_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcMenu5=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcMenu5->Create(this,g_hf14AcuMed,0);
	m_pcMenu5->SetText(_T(""));

	//if(m_eViewState==VS_BATTERY)
		m_pcMenu5->ShowWindow(SW_SHOW);
	//else
	//	m_pcMenu5->ShowWindow(SW_HIDE);

	//release the Display DC
	//ReleaseDC(pDC);

	return result;
}

/**********************************************************************************************//**
 * Opens this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewFullscreenMsg::Open()
{
}

/**********************************************************************************************//**
 * Closes this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewFullscreenMsg::Close()
{
}

/**********************************************************************************************//**
 * Shows this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewFullscreenMsg::Show()
{
	this->ShowWindow(SW_SHOW);

	Draw();


}

/**********************************************************************************************//**
 * Hides this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewFullscreenMsg::Hide()
{
	this->ShowWindow(SW_HIDE);
}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewFullscreenMsg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewFullscreenMsg::OnDestroy()
{
	KillTimer(STARTUPTIMER);

	if(m_eViewState==VS_SYSFAIL)
	{
		getModel()->getI2C()->DeleteI2CErrorCode();
		getModel()->getDATAHANDLER()->deleteAllCOMError(); //newVG

		//newVG
		if(getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()==AS_ACTIVE)
		{
			getModel()->getALARMHANDLER()->setStateOfAlarm(AL_SysFail_IF_COM, AS_SIGNALED);
			getModel()->setSERIALavailable(TRUE);
		}
		getModel()->getDATAHANDLER()->deleteSPIErrorCode();

		getModel()->setSPIavailability(TRUE);
		if(m_bI2Cerror)
		{
			getModel()->getI2C()->ReinitI2C();
		}
		else
		{
			getModel()->getI2C()->SetI2C_FRAMavailability(TRUE);
			if(getModel()->getI2C()->GetACCUstate())
				getModel()->getI2C()->SetI2C_ACCUavailability(TRUE);
			getModel()->getI2C()->SetI2C_MCPavailability(TRUE);
		}

		getModel()->SetDIOavailability(TRUE);
		getModel()->SetAcuLinkAvailability(TRUE);
	}

	CMVView::OnDestroy();
}

/**********************************************************************************************//**
 * Executes the timer action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	nIDEvent	The identifier event.
 **************************************************************************************************/

void CViewFullscreenMsg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==STARTUPTIMER)
	{
		Draw();
	}

	CMVView::OnTimer(nIDEvent);
}

/**********************************************************************************************//**
 * Draws this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewFullscreenMsg::Draw()
{

	RECT rc={0,0,m_lX,m_lY};
	//RECT rcCl;
	//GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	int bc=SetBkColor(hdcMem,BACKGND);
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);

	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf31AcuBold);

	int tc=SetTextColor(hdcMem,RGB(255,0,0));

	CBrush cbrBack(BACKGND);
	HBRUSH hPrevBrush=(HBRUSH)SelectObject(hdcMem,(HBRUSH)cbrBack);
	HPEN hPrevPen=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));
	//HPEN hPrevPen=(HPEN)SelectObject(hdcMem, (HPEN)penEll);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	CStringW sz = _T("");

	if(m_bDraw)
	{
		if(m_eViewState==VS_BATTERY)
		{
			if(	getModel()->getALARMHANDLER()->ALARM_Accu_Defect->getAlarmState()!=AS_NONE)
			{
				sz = getModel()->GetLanguageString(IDS_TXT_ACCUDEFECT);
			}
			else
			{
				sz = getModel()->GetLanguageString(IDS_TXT_FLATBATT);
			}

		}
		else if(m_eViewState==VS_SYSFAIL)
			sz = getModel()->GetLanguageString(IDS_TXT_SYSFAIL);

		rc.top = 370;
		rc.bottom = m_lY;
		rc.left = 0; 
		rc.right = m_lX;

		DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);
	}

	SetTextColor(hdcMem,RGB(0,0,0));

	SelectObject(hdcMem,g_hf13AcuBold);

	if(m_eViewState==VS_BATTERY)
	{
		if(	getModel()->getALARMHANDLER()->ALARM_Accu_Defect->getAlarmState()!=AS_NONE)
		{
			sz = getModel()->GetLanguageString(IDS_TXT_CHECKACCU);
			rc.top = 480;
			rc.bottom = m_lY;
			rc.left = 0; 
			rc.right = m_lX;
			DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);
		}
		else
		{
			sz = getModel()->GetLanguageString(IDS_TXT_CONNECTPOW);
			rc.top = 480;
			rc.bottom = m_lY;
			rc.left = 0; 
			rc.right = m_lX;
			DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);

			sz =getModel()->GetLanguageString(IDS_TXT_CHARGE);
			rc.top = 500;
			rc.bottom = m_lY;
			rc.left = 0; 
			rc.right = m_lX;
			DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);
		}
	}
	else if(m_eViewState==VS_SYSFAIL)
	{
		sz = getModel()->GetLanguageString(IDS_TXT_SYSFAILCODE);
		rc.top = 420;
		rc.bottom = m_lY;
		rc.left = 0; 
		rc.right = m_lX;
		DrawText(hdcMem,sz,-1,&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);

		SelectObject(hdcMem,g_hf8AcuBold);

		//system alarms
		/*tALARMSTATES alarmState;
		getModel()->getALARMHANDLER()->getAllAlarmStates(&alarmState);*/

		CStringW szAlarm=_T("");

		// AlarmState_RELAIS_DEFECT **********************************************************
		if(getModel()->getALARMHANDLER()->ALARM_SysFail_OUTOFMEMORY->getAlarmState()!=AS_NONE)
		{
			szAlarm+=getModel()->GetLanguageString(IDS_ALARM_OUTOFMEMORY)+_T(", ");
		}

		// AlarmState_RELAIS_DEFECT **********************************************************
		if(getModel()->getALARMHANDLER()->ALARM_SysFail_RELAIS_DEFECT->getAlarmState()!=AS_NONE)
		{
			szAlarm+=getModel()->GetLanguageString(IDS_ALARM_RELAIS_DEFECT)+_T(", ");
			//pDCMem->DrawText(getModel()->GetLanguageString(IDS_ALARM_RELAIS_DEFECT),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		}
		
		// AlarmState_P_PROXIMAL **********************************************************
		//if(alarmState.eSoA_P_PROXIMAL!=AS_NONE)
		//{
		//	//pDCMem->DrawText(getModel()->GetLanguageString(IDS_ALARM_PPROXIMAL),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
		//	szAlarm+=getModel()->GetLanguageString(IDS_ALARM_PPROXIMAL)+_T(", ");
		//}
		
		// AlarmState_P_IN_MIXER **********************************************************
		if(getModel()->getALARMHANDLER()->ALARM_SysFail_P_IN_MIXER->getAlarmState()!=AS_NONE)
		{
			//pDCMem->DrawText(getModel()->GetLanguageString(IDS_ALARM_PIN_MIXER),&rc,DT_TOP|DT_SINGLELINE|DT_LEFT);
			szAlarm+=getModel()->GetLanguageString(IDS_ALARM_PIN_MIXER)+_T(", ");
		}

		

		// AlarmState_VOLTAGE **********************************************************
		if(getModel()->getALARMHANDLER()->ALARM_SysFail_VOLTAGE->getAlarmState()!=AS_NONE)
		{
			CString szTemp=_T("");
			szTemp.Format(_T(" WDST%d"), (int)getModel()->getDATAHANDLER()->GetI2CWatchdogState());

			szAlarm+=getModel()->GetLanguageString(IDS_ALARM_VOLTAGE)+szTemp+_T(", ");

		}

		if(getModel()->getALARMHANDLER()->ALARM_SysFail_ChecksumConPIC->getAlarmState()!=AS_NONE)
		{
			szAlarm+=getModel()->GetLanguageString(IDS_ALARM_CSCONPIC)+_T(", ");
		}

		if(getModel()->getALARMHANDLER()->ALARM_SysFail_ChecksumMonPIC->getAlarmState()!=AS_NONE)
		{
			szAlarm+=getModel()->GetLanguageString(IDS_ALARM_CSMONPIC)+_T(", ");
		}

		if(getModel()->getALARMHANDLER()->ALARM_SysFail_Fan->getAlarmState()!=AS_NONE)
		{
			szAlarm+=getModel()->GetLanguageString(IDS_ALARM_FAN)+_T(", ");
		}


		int iCode=getModel()->getDATAHANDLER()->getCOMErrorCode();//newVG

		if(getModel()->getALARMHANDLER()->ALARM_SysFail_IF_COM->getAlarmState()!=AS_NONE || iCode>0)
		{
			m_bSERIALerror=true;
			szAlarm+=getModel()->GetLanguageString(IDS_ALARM_IF_COM);
			CStringW szTxt=_T("");

			//newVG
			if(iCode & ERRC_COM_INIT)
			{
				szTxt=_T(": ERRC_COM_INIT");
			}
			else if(iCode & ERRC_COM_CHECK_DATA)
			{
				szTxt=_T(": ERRC_COM_CHECK_DATA");
			}
			else if(iCode & ERRC_COM_SEND_DATA)
			{
				szTxt=_T(": ERRC_COM_SEND_DATA");
			}
			else if(iCode & ERRC_COM_READ_MSTATUS)
			{
				szTxt=_T(": ERRC_COM_READ_MSTATUS");
			}
			else if(iCode & ERRC_COM_CO2_INIT)
			{
				szTxt=_T(": ERRC_COM_CO2_INIT");
			}
			else if(iCode & ERRC_COM_CO2_READDATA)
			{
				szTxt=_T(": ERRC_COM_CO2_READDATA");
			}
			else if(iCode & ERRC_COM_CO2_SENDDATA)
			{
				szTxt=_T(": ERRC_COM_CO2_SENDDATA");
			}
			else if(iCode & ERRC_USB_SPO2_INIT)
			{
				szTxt=_T(": ERRC_USB_SPO2_INIT");
			}
			

			szAlarm+=szTxt+_T(", ");
		}

		//iCode=getModel()->getDATAHANDLER()->GetCO2ErrorCode(); //newVG

		/*if(getModel()->IsSERIALavailabel())
			getModel()->SetSERIALavailability(FALSE);*/

		/*if(getModel()->getALARMHANDLER()->alarmState.eSoA_IF_CO2!=AS_NONE || iCode>0)
		{
			szAlarm+=getModel()->GetLanguageString(IDS_ALARM_IF_CO2);
			CStringW szTxt=_T("");

			if(iCode & ERRC_COM_CO2_INIT)
			{
				szTxt=_T(": ERRC_COM_CO2_INIT");
			}
			else if(iCode & ERRC_COM_CO2_SENDDATA)
			{
				szTxt=_T(": ERRC_COM_CO2_SENDDATA");
			}
			else if(iCode & ERRC_COM_CO2_READDATA)
			{
				szTxt=_T(": ERRC_COM_CO2_READDATA");
			}
			

			szAlarm+=szTxt+_T(", ");
		}*/

		/*iCode=getModel()->getDATAHANDLER()->GetSPO2ErrorCode();

		if(alarmState.eSoA_IF_SPO2!=AS_NONE || iCode>0)
		{
			szAlarm+=getModel()->GetLanguageString(IDS_ALARM_IF_SPO2);
			CStringW szTxt=_T("");

			if(iCode & ERRC_USB_SPO2_INIT)
			{
				szTxt=_T(": ERRC_USB_SPO2_INIT");
			}
			else if(iCode & ERRC_USB_SPO2_SENDDATA)
			{
				szTxt=_T(": ERRC_USB_SPO2_SENDDATA");
			}
			else if(iCode & ERRC_USB_SPO2_READDATA)
			{
				szTxt=_T(": ERRC_USB_SPO2_READDATA");
			}


			szAlarm+=szTxt+_T(", ");
		}*/

		iCode=getModel()->getDATAHANDLER()->getSPIErrorCode();

		if(getModel()->getALARMHANDLER()->ALARM_SysFail_IF_SPI->getAlarmState()!=AS_NONE || iCode>0)
		{
			szAlarm+=getModel()->GetLanguageString(IDS_ALARM_IF_SPI);
			CStringW szTxt=_T("");
			

			if(iCode & ERRC_SPI_ALIVE)
			{
				szTxt=_T(": ERRC_SPI_ALIVE");
			}
			else if(iCode & ERRC_SPI_READALIVE)
			{
				szTxt=_T(": ERRC_SPI_READALIVE");
			}
			else if(iCode & ERRC_SPI_INIT)
			{
				szTxt=_T(": ERRC_SPI_INIT");
			}
			else if(iCode & ERRC_SPI_SET_FLOWSENSSTATE)
			{
				szTxt=_T(": ERRC_SPI_SET_FLOWSENSSTATE");
			}
			else if(iCode & ERRC_SPI_READSTATUS)
			{
				szTxt=_T(": ERRC_SPI_READSTATUS");
			}
			else if(iCode & ERRC_SPI_VOLTAGE)
			{
				szTxt=_T(": ERRC_SPI_VOLTAGE");
			}
			else if(iCode & ERRC_SPI_RELAIS)
			{
				szTxt=_T(": ERRC_SPI_RELAIS");
			}
			else if(iCode & ERRC_SPI_MAXPFVBLOCKBUF)
			{
				szTxt=_T(": ERRC_SPI_MAXPFVBLOCKBUF");
			}
			else if(iCode & ERRC_SPI_NULLBLOCKBUF)
			{
				szTxt=_T(": ERRC_SPI_NULLBLOCKBUF");
			}
			else if(iCode & ERRC_SPI_INVALID_HANDLE_VALUE)
			{
				szTxt=_T(": ERRC_SPI_INVALID_HANDLE_VALUE");
			}
			else if(iCode & ERRC_SPI_READMESSUREDATA)
			{
				szTxt=_T(": ERRC_SPI_READDATA");
			}
			else if(iCode & ERRC_SPI_SENDCOMMAND)
			{
				szTxt=_T(": ERRC_SPI_SENDCOMMAND");
			}
			
			szAlarm+=szTxt+_T(", ");
		}

		if(getModel()->getALARMHANDLER()->ALARM_SysFail_IF_I2C->getAlarmState()!=AS_NONE)
		{
			m_bI2Cerror=true;
			szAlarm+=getModel()->GetLanguageString(IDS_ALARM_IF_I2C);
			CStringW szTxt=_T("");
			int iCode=getModel()->getI2C()->GetI2CErrorCode();

			if(iCode & ERRC_I2C_INIT)
			{
				szTxt=_T(": ERRC_I2C_INIT");
			}
			else if(iCode & ERRC_I2C_MCP)
			{
				szTxt=_T(": ERRC_I2C_MCP");
			}
			else if(iCode & ERRC_I2C_FRAM)
			{
				szTxt=_T(": ERRC_I2C_FRAM");
			}
			else if(iCode & ERRC_I2C_ACCU)
			{
				szTxt=_T(": ERRC_I2C_ACCU");
			}

			szAlarm+=szTxt+_T(", ");
		}

		if(getModel()->getALARMHANDLER()->ALARM_SysFail_IF_PIF->getAlarmState()!=AS_NONE)
		{
			szAlarm+=getModel()->GetLanguageString(IDS_ALARM_IF_PIF);
			szAlarm+=_T(", ");
		}

		if(getModel()->getALARMHANDLER()->ALARM_SysFail_IF_DIO->getAlarmState()!=AS_NONE)
		{
			szAlarm+=getModel()->GetLanguageString(IDS_ALARM_IF_DIO);
			szAlarm+=_T(", ");
		}

		/*if(getModel()->getALARMHANDLER()->alarmState.eSoA_IF_ACULINK!=AS_NONE)
		{
			szAlarm+=getModel()->GetLanguageString(IDS_ALARM_IF_TERMINAL);
			szAlarm+=_T(", ");
		}*/

		rc.top = 445;
		rc.bottom = m_lY;
		rc.left = 0; 
		rc.right = m_lX;
		//DrawText(hdcMem,szAlarm,-1,&rc,DT_TOP|DT_SINGLELINE|DT_CENTER);
		DrawText(hdcMem,szAlarm,-1,&rc,DT_TOP|DT_WORDBREAK|DT_CENTER);
	}

	m_bDraw=!m_bDraw;
	
	//m_pcLogo->Draw(hdcMem,450,30);
	if(m_pcCaution)
		m_pcCaution->Draw(hdcMem,250,100);

	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);
	BitBlt(m_hDC, 0, 0, m_lX, m_lY, hdcMem, 0, 0, SRCCOPY);
	
	SelectObject(hdcMem, hPrevBrush);	
	SelectObject(hdcMem, hPrevPen);
	SetTextColor(hdcMem,tc);
	SetBkColor(hdcMem,bc);
	SetBkMode(hdcMem,nBkMode);
	SelectObject(hdcMem,hPrevFont);
	SelectObject(hdcMem,hBmpMemPrev);
	DeleteObject(hBmpMem);

	//DeleteObject(cbrBack);

	DeleteDC(hdcMem);
}

/**********************************************************************************************//**
 * Notifies an event
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param [in,out]	pEvent	If non-null, the event.
 **************************************************************************************************/

void CViewFullscreenMsg::NotifyEvent(CMVEvent* pEvent)
{
	//csView.Lock();

	switch(pEvent->GetET())
	{
	case CMVEvent::ET_ALARM://++++++++++++++++++++++++++++++System-AlarmeP+++++++++++++++++++++++++++++++++++++++++
		{
			CMVEventAlarm* pUIEvent = (CMVEventAlarm*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventAlarm::EV_ALARMSTATE_CHANGED:
				{
					if(m_eViewState==VS_BATTERY)
					{	
						if(		getModel()->getALARMHANDLER()->ALARM_Accu_Empty->getAlarmState()==AS_NONE
							&&	getModel()->getALARMHANDLER()->ALARM_Accu_Defect->getAlarmState()==AS_NONE)
						{
							m_bDraw=true;
							KillTimer(STARTUPTIMER);
						}
						else
							SetTimer(STARTUPTIMER,1000,NULL);
					}
					else if(m_eViewState==VS_SYSFAIL)
					{
						if(getModel()->getALARMHANDLER()->isActiveAlarmType(AT_SYSFAIL)==false)
						{
							m_bDraw=true;
							KillTimer(STARTUPTIMER);
						}
						else
							SetTimer(STARTUPTIMER,1000,NULL);
					}
					try
					{
						Draw();
					}
					catch (...)
					{
						theApp.ReportException(_T("CViewFullscreenMsg::NotifyEvent"));
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	}
	//csView.Unlock();
}

/**********************************************************************************************//**
 * Window proc
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	message	The message.
 * \param	wParam 	The wParam field of the message.
 * \param	lParam 	The lParam field of the message.
 *
 * \return	A LRESULT.
 **************************************************************************************************/

LRESULT CViewFullscreenMsg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_SETVIEWFOCUS:
		{
			//rku cs1
			SetViewFocus();
			return 1;
		}
		break;
	case WM_SETPREVFOCUS:
		{
			//rku cs1
			SetPrevFocus();
			return 1;
		}
		break;
	case WM_SETNEXTFOCUS:
		{
			//rku cs1
			SetNextFocus();
			return 1;
		}
		break;case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_MENU_NUMERIC:
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_NUMERIC);
					return 1;
				}
				break;
			}
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}
