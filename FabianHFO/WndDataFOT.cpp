// WndDataFOT.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "WndDataFOT.h"
#include "DlgMessageBox.h"

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTSUBBTNDW			0x00000000
#define COLOR_TXTBTNFC				0x00000000
#define COLOR_TXTBTNDIS				0x00828282

// CWndDataFOT

IMPLEMENT_DYNAMIC(CWndDataFOT, CWnd)

CWndDataFOT::CWndDataFOT(CMVView *parentView)
{
	m_parentView=parentView;
	m_pModel=NULL;

	m_pDlg = NULL;

	m_hdcStatic=NULL;
	m_hbmpStatic=NULL;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;

	m_bHourglassRun=false;
	

	m_pcPara_UpT=NULL;
	m_pcPara_FcT=NULL;
	m_pcPara_DwT=NULL;
	m_pcPara_UpB=NULL;
	m_pcPara_FcB=NULL;
	m_pcPara_DwB=NULL;

	m_pcPara_FOT_Steps=NULL;
	m_pcPara_FOT_PmeanLow=NULL;
	m_pcPara_FOT_PmeanHigh=NULL;
	m_pcPara_FOT_PEEPLow=NULL;
	m_pcPara_FOT_PEEPHigh=NULL;

	//m_pcPara_FOTamplitude=NULL;
	//m_pcPara_FOTfreq=NULL;
	
	m_pcBtnStartStopSeq=NULL;
	m_pcBtnRunSeq=NULL;
	m_pcBtnRepeatSeq=NULL;
	m_pcBtnDecreaseSeq=NULL;

	m_pcStartStopSeq_Dw=NULL;
	m_pcStartStopSeq_Up=NULL;
	m_pcStartStopSeq_Dis=NULL;

	m_pcRunSeq_Dw=NULL;
	m_pcRunSeq_Up=NULL;
	m_pcRunSeq_Dis=NULL;

	m_pcRepeatSeq_Dw=NULL;
	m_pcRepeatSeq_Up=NULL;
	m_pcRepeatSeq_Dis=NULL;

	m_pcDecreaseSeq_Dw=NULL;
	m_pcDecreaseSeq_Up=NULL;
	m_pcDecreaseSeqx_Dis=NULL;

	/*m_pcMenuStartStop=NULL;
	m_pcMenuStartStop_Dw=NULL;
	m_pcMenuStartStop_Up=NULL;
	m_pcMenuStartStop_Dis=NULL;

	m_pcContinueFOTseq=NULL;
	m_pcContinueFOTseq_Dw=NULL;
	m_pcContinueFOTseq_Up=NULL;*/

	m_pcWait=NULL;
	m_pcWait1= NULL;
	m_pcWait2= NULL;
	m_pcWait3= NULL;
	m_pcWait4= NULL;
	m_pcWait5= NULL;
	m_pcWait6= NULL;
	m_pcWait7= NULL;
	m_pcWait8= NULL;
	m_iWaitCount=0;

	if(getModel()->getFOTThread())
		feFOTstate=getModel()->getFOTThread()->getFOTstate();
	else
		feFOTstate=FOT_OFF;
	m_iCountFOTimer=0;
	//m_bFOTdataAvailable=false;

	//m_bPMEANEND_SetKey=false;
}

CWndDataFOT::~CWndDataFOT()
{
}

CMVModel *CWndDataFOT::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

BEGIN_MESSAGE_MAP(CWndDataFOT, CWnd)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STARTSTOP_FOT, &CWndDataFOT::OnBnClickedStartStop)
	ON_BN_CLICKED(IDC_BTN_DECREASE_FOT, &CWndDataFOT::OnBnClickedDecrease)
	ON_BN_CLICKED(IDC_BTN_REPEAT_FOT, &CWndDataFOT::OnBnClickedRepeat)
	ON_BN_CLICKED(IDC_BTN_CONTINUE_FOT, &CWndDataFOT::OnBnClickedContinue)
END_MESSAGE_MAP()



// CWndDataFOT message handlers

BOOL CWndDataFOT::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
{
	bool bRes=false;
	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD/*|WS_VISIBLE*/,rc,pParentWnd,nID))
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

		m_hdcStatic = CreateCompatibleDC(m_hDC);
		m_hbmpStatic = CreateCompatibleBitmap(m_hDC, m_lX, m_lY);
		SelectObject(m_hdcStatic, m_hbmpStatic);
		HPEN hpenprevStat=(HPEN)SelectObject(m_hdcStatic, (HPEN)GetStockObject(NULL_PEN));	
		HBRUSH hbrprevStat=(HBRUSH)SelectObject(m_hdcStatic,cbrBack); 
		Rectangle(m_hdcStatic, 0, 0, m_lX, m_lY);

		m_pcPara_UpT		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_UP_TOP);
		m_pcPara_FcT		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_FC_TOP);
		m_pcPara_DwT		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_DW_TOP);
		m_pcPara_UpB		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_UP_BOT);
		m_pcPara_FcB		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_FC_BOT);
		m_pcPara_DwB		= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_PARA_DW_BOT);

		/*m_pcMenuStartStop_Dw	=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_ONOFF_DW);
		m_pcMenuStartStop_Up	=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_ONOFF_UP);
		m_pcMenuStartStop_Dis	=new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_ONOFF_DIS);


		m_pcContinueFOTseq_Up =new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN160_LG_UP);
		m_pcContinueFOTseq_Dw =new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN160_LG_DW);*/
		



		m_pcStartStopSeq_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_FOT_ONOFF_DW);
		m_pcStartStopSeq_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_FOT_ONOFF_UP);
		m_pcStartStopSeq_Dis= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_FOT_ONOFF_DIS);

		m_pcRunSeq_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_FOT_RUN_DW);
		m_pcRunSeq_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_FOT_RUN_UP);
		m_pcRunSeq_Dis= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_FOT_RUN_DIS);

		m_pcRepeatSeq_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_FOT_REPEAT_DW);
		m_pcRepeatSeq_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_FOT_REPEAT_UP);
		m_pcRepeatSeq_Dis= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_FOT_REPEAT_DIS);

		m_pcDecreaseSeq_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_FOT_DECREASE_DW);
		m_pcDecreaseSeq_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_FOT_DECREASE_UP);
		m_pcDecreaseSeqx_Dis= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_FOT_DECREASE_DIS);


		m_pcWait=NULL;
		m_pcWait1= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY1);
		m_pcWait2= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY2);
		m_pcWait3= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY3);
		m_pcWait4= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY4);
		m_pcWait5= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY5);
		m_pcWait6= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY6);
		m_pcWait7= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY7);
		m_pcWait8= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_WAIT30_GREY8);

		DWORD dwStyleNoTab = WS_CHILD/*|WS_VISIBLE*/|BS_OWNERDRAW|WS_TABSTOP;
		BTN btn;
		fVALUE fv;
		
		if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
		{
			

			//Parameter Button------FOT steps---------------------------------
			btn.wID					= IDC_BTN_PARA_FOT_STEPS;	
			btn.poPosition.x		= 7;
			btn.poPosition.y		= 7;
			btn.pcBmpUp				= m_pcPara_UpT;
			btn.pcBmpDown			= m_pcPara_DwT;
			btn.pcBmpFocus			= m_pcPara_FcT;
			btn.pcBmpDisabled		= m_pcPara_UpT;
			btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSMaxPara();

			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara();
			if(fv.iValue>fv.iUpperLimit)
				fv.iValue=fv.iUpperLimit;
			else if(fv.iValue<fv.iLowerLimit)
				fv.iValue=fv.iLowerLimit;
			m_pcPara_FOT_Steps=new CParaBtn_FOTSTEPS(btn,0,false);
			m_pcPara_FOT_Steps->Create(this,dwStyleNoTab,fv);
			m_pcPara_FOT_Steps->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
			m_pcPara_FOT_Steps->SetNameText(_T("FOT"));
			m_pcPara_FOT_Steps->SetNameNoteText(getModel()->GetLanguageString(IDS_TXT_FOT_STEPS));
			m_pcPara_FOT_Steps->SetUnitText(_T(""));
			m_pcPara_FOT_Steps->ShowWindow(SW_SHOW);


			//Parameter Button------FOT Pmean start---------------------------------
			btn.wID					= IDC_BTN_PARA_FOT_PMEANLOW;	
			btn.poPosition.x		= 102;
			btn.poPosition.y		= 7;
			btn.pcBmpUp				= m_pcPara_UpT;
			btn.pcBmpDown			= m_pcPara_DwT;
			btn.pcBmpFocus			= m_pcPara_FcT;
			btn.pcBmpDisabled		= m_pcPara_UpT;
			btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMinPara();
			
			if(getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara()>1)
			{
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_PMEANENDPara()-getModel()->getCONFIG()->getDiffPmeanFOT();
			}
			else
			{
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxPara();
			}
			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_PMEANSTARTPara();
			if(fv.iValue>fv.iUpperLimit)
			{
				fv.iValue=fv.iUpperLimit;
				getModel()->getDATAHANDLER()->PARADATA()->setFOThfo_PMEANSTARTPara(fv.iValue,true,true);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				fv.iValue=fv.iLowerLimit;
				getModel()->getDATAHANDLER()->PARADATA()->setFOThfo_PMEANSTARTPara(fv.iValue,true,true);
			}
			m_pcPara_FOT_PmeanLow=new CParaBtn_FOTPMEANSTART(btn,0,false);
			m_pcPara_FOT_PmeanLow->Create(this,dwStyleNoTab,fv);
			m_pcPara_FOT_PmeanLow->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
			if(getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara()>1)
			{
				m_pcPara_FOT_PmeanLow->SetNameText(_T("P"));
				m_pcPara_FOT_PmeanLow->SetNameNoteText(_T("mean low"));
			}
			else
			{
				m_pcPara_FOT_PmeanLow->SetNameText(_T("P"));
				m_pcPara_FOT_PmeanLow->SetNameNoteText(_T("mean"));
			}
			if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
			{
				m_pcPara_FOT_PmeanLow->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MBAR));
			}
			else
			{
				m_pcPara_FOT_PmeanLow->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_CMH2O));
			}
			m_pcPara_FOT_PmeanLow->ShowWindow(SW_SHOW);

			if(fv.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxKey())
			{
				m_pcPara_FOT_PmeanLow->SetWarning();
			}
			else
			{
				m_pcPara_FOT_PmeanLow->SetWarning(false);
			}


			//-------------------------------------------------------------
			//Parameter Button------FOT Pmean end---------------------------------
			btn.wID					= IDC_BTN_PARA_FOT_PMEANHIGH;	
			btn.poPosition.x		= 197;
			btn.poPosition.y		= 7;
			btn.pcBmpUp				= m_pcPara_UpT;
			btn.pcBmpDown			= m_pcPara_DwT;
			btn.pcBmpFocus			= m_pcPara_FcT;
			btn.pcBmpDisabled		= m_pcPara_UpT;
			btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_PMEANSTARTPara()+getModel()->getCONFIG()->getDiffPmeanFOT();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxPara();

			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_PMEANENDPara();
			if(fv.iValue>fv.iUpperLimit)
			{
				fv.iValue=fv.iUpperLimit;
				getModel()->getDATAHANDLER()->PARADATA()->setFOThfo_PMEANENDPara(fv.iValue,true,true);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				fv.iValue=fv.iLowerLimit;
				getModel()->getDATAHANDLER()->PARADATA()->setFOThfo_PMEANENDPara(fv.iValue,true,true);
			}
			m_pcPara_FOT_PmeanHigh=new CParaBtn_FOTPMEANEND(btn,0,false);
			m_pcPara_FOT_PmeanHigh->Create(this,dwStyleNoTab,fv);
			m_pcPara_FOT_PmeanHigh->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
			m_pcPara_FOT_PmeanHigh->SetNameText(_T("P"));
			m_pcPara_FOT_PmeanHigh->SetNameNoteText(_T("mean high"));
			if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
			{
				m_pcPara_FOT_PmeanHigh->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MBAR));
			}
			else
			{
				m_pcPara_FOT_PmeanHigh->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_CMH2O));
			}

			if(fv.iValue>getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxKey())
			{
				m_pcPara_FOT_PmeanHigh->SetWarning();
			}
			else
			{
				m_pcPara_FOT_PmeanHigh->SetWarning(false);
			}


			if(getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara()>1)
			{
				m_pcPara_FOT_PmeanHigh->ShowWindow(SW_SHOW);
			}
			else
			{
				m_pcPara_FOT_PmeanHigh->ShowWindow(SW_HIDE);
			}
			
			if(getModel()->getFOTThread())
			{
				if(getModel()->getFOTThread()->getFOTstate()!=FOT_OFF)
				{
					if(m_pcPara_FOT_Steps)
						m_pcPara_FOT_Steps->EnableWindow(FALSE);
					if(m_pcPara_FOT_PmeanLow)
						m_pcPara_FOT_PmeanLow->EnableWindow(FALSE);
					if(m_pcPara_FOT_PmeanHigh)
						m_pcPara_FOT_PmeanHigh->EnableWindow(FALSE);
				}
				else
				{
					if(m_pcPara_FOT_Steps)
						m_pcPara_FOT_Steps->EnableWindow(TRUE);
					if(m_pcPara_FOT_PmeanLow)
						m_pcPara_FOT_PmeanLow->EnableWindow(TRUE);
					if(m_pcPara_FOT_PmeanHigh)
						m_pcPara_FOT_PmeanHigh->EnableWindow(TRUE);
				}
			}
			else
			{
				if(m_pcPara_FOT_Steps)
					m_pcPara_FOT_Steps->EnableWindow(FALSE);
				if(m_pcPara_FOT_PmeanLow)
					m_pcPara_FOT_PmeanLow->EnableWindow(FALSE);
				if(m_pcPara_FOT_PmeanHigh)
					m_pcPara_FOT_PmeanHigh->EnableWindow(FALSE);
			}
		}
		else
		{
			
			
			//Parameter Button------FOT steps---------------------------------
			btn.wID					= IDC_BTN_PARA_FOT_STEPS;	
			btn.poPosition.x		= 7;
			btn.poPosition.y		= 7;
			btn.pcBmpUp				= m_pcPara_UpT;
			btn.pcBmpDown			= m_pcPara_DwT;
			btn.pcBmpFocus			= m_pcPara_FcT;
			btn.pcBmpDisabled		= m_pcPara_UpT;
			btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	
			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSMinPara();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSMaxPara();

			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara();
			if(fv.iValue>fv.iUpperLimit)
				fv.iValue=fv.iUpperLimit;
			else if(fv.iValue<fv.iLowerLimit)
				fv.iValue=fv.iLowerLimit;
			m_pcPara_FOT_Steps=new CParaBtn_FOTSTEPS(btn,0,false);
			m_pcPara_FOT_Steps->Create(this,dwStyleNoTab,fv);
			m_pcPara_FOT_Steps->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
			m_pcPara_FOT_Steps->SetNameText(_T("FOT"));
			m_pcPara_FOT_Steps->SetNameNoteText(_T("steps"));
			m_pcPara_FOT_Steps->SetUnitText(_T(""));
			m_pcPara_FOT_Steps->ShowWindow(SW_SHOW);


			//Parameter Button------FOT Pmean start---------------------------------
			btn.wID					= IDC_BTN_PARA_FOT_PEEPLOW;	
			btn.poPosition.x		= 102;
			btn.poPosition.y		= 7;
			btn.pcBmpUp				= m_pcPara_UpT;
			btn.pcBmpDown			= m_pcPara_DwT;
			btn.pcBmpFocus			= m_pcPara_FcT;
			btn.pcBmpDisabled		= m_pcPara_UpT;
			btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

			WORD iValAMP=(getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_AMPLITUDEPara()+1)/2;
			if(iValAMP%10!=0)
				iValAMP+=5;
			fv.iLowerLimit=iValAMP;
			if(getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara()>1)
			{
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_PEEPENDPara()-getModel()->getCONFIG()->getDiffPEEPFOT();
			}
			else
			{
				fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMaxPara_IPPV();
			}


			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_PEEPSTARTPara();
			if(fv.iValue>fv.iUpperLimit)
			{
				fv.iValue=fv.iUpperLimit;
				getModel()->getDATAHANDLER()->PARADATA()->setFOTconv_PEEPSTARTPara(fv.iValue,true,true);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				fv.iValue=fv.iLowerLimit;
				getModel()->getDATAHANDLER()->PARADATA()->setFOTconv_PEEPSTARTPara(fv.iValue,true,true);
			}

			
			m_pcPara_FOT_PEEPLow=new CParaBtn_FOTPEEPSTART(btn,0,false);
			m_pcPara_FOT_PEEPLow->Create(this,dwStyleNoTab,fv);
			m_pcPara_FOT_PEEPLow->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
			if(getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara()>1)
			{
				m_pcPara_FOT_PEEPLow->SetNameText(_T("PEEP"));
				m_pcPara_FOT_PEEPLow->SetNameNoteText(_T("low"));
			}
			else
			{
				m_pcPara_FOT_PEEPLow->SetNameText(_T("PEEP"));
				m_pcPara_FOT_PEEPLow->SetNameNoteText(_T(""));
			}
			if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
			{
				m_pcPara_FOT_PEEPLow->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MBAR));
			}
			else
			{
				m_pcPara_FOT_PEEPLow->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_CMH2O));
			}
			m_pcPara_FOT_PEEPLow->ShowWindow(SW_SHOW);

			if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentPeepMaxKey())
			{
				m_pcPara_FOT_PEEPLow->SetWarning();
			}
			else
			{
				m_pcPara_FOT_PEEPLow->SetWarning(false);
			}


			//-------------------------------------------------------------
			//Parameter Button------FOT Pmean end---------------------------------
			btn.wID					= IDC_BTN_PARA_FOT_PEEPHIGH;	
			/*btn.poPosition.x		= 102;
			btn.poPosition.y		= 102;
			btn.pcBmpUp				= m_pcPara_UpB;
			btn.pcBmpDown			= m_pcPara_DwB;
			btn.pcBmpFocus			= m_pcPara_FcB;
			btn.pcBmpDisabled		= m_pcPara_UpB;*/
			btn.poPosition.x		= 197;
			btn.poPosition.y		= 7;
			btn.pcBmpUp				= m_pcPara_UpT;
			btn.pcBmpDown			= m_pcPara_DwT;
			btn.pcBmpFocus			= m_pcPara_FcT;
			btn.pcBmpDisabled		= m_pcPara_UpT;
			btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

			fv.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_PEEPSTARTPara()+getModel()->getCONFIG()->getDiffPEEPFOT();
			fv.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMaxPara_IPPV();

			fv.iValue=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_PEEPENDPara();
			if(fv.iValue>fv.iUpperLimit)
			{
				fv.iValue=fv.iUpperLimit;
				getModel()->getDATAHANDLER()->PARADATA()->setFOTconv_PEEPENDPara(fv.iValue,true,true);
			}
			else if(fv.iValue<fv.iLowerLimit)
			{
				fv.iValue=fv.iLowerLimit;
				getModel()->getDATAHANDLER()->PARADATA()->setFOTconv_PEEPENDPara(fv.iValue,true,true);
			}
			m_pcPara_FOT_PEEPHigh=new CParaBtn_FOTPEEPEND(btn,0,false);
			m_pcPara_FOT_PEEPHigh->Create(this,dwStyleNoTab,fv);
			m_pcPara_FOT_PEEPHigh->SetColors(COLOR_TXTBTNUP,COLOR_TXTBTNDW,COLOR_TXTSUBBTNDW,COLOR_TXTBTNFC);
			m_pcPara_FOT_PEEPHigh->SetNameText(_T("PEEP"));
			m_pcPara_FOT_PEEPHigh->SetNameNoteText(_T("high"));
			if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
			{
				m_pcPara_FOT_PEEPHigh->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_MBAR));
			}
			else
			{
				m_pcPara_FOT_PEEPHigh->SetUnitText(getModel()->GetLanguageString(IDS_UNIT_CMH2O));
			}
			if(getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara()>1)
			{
				m_pcPara_FOT_PEEPHigh->ShowWindow(SW_SHOW);
			}
			else
			{
				m_pcPara_FOT_PEEPHigh->ShowWindow(SW_HIDE);
			}

			if(fv.iValue>getModel()->getDATAHANDLER()->GetCurrentPeepMaxKey())
			{
				m_pcPara_FOT_PEEPHigh->SetWarning();
			}
			else
			{
				m_pcPara_FOT_PEEPHigh->SetWarning(false);
			}

			if(getModel()->getFOTThread())
			{
				if(getModel()->getFOTThread()->getFOTstate()!=FOT_OFF)
				{
					if(m_pcPara_FOT_Steps)
						m_pcPara_FOT_Steps->EnableWindow(FALSE);
					if(m_pcPara_FOT_PEEPLow)
						m_pcPara_FOT_PEEPLow->EnableWindow(FALSE);
					if(m_pcPara_FOT_PEEPHigh)
						m_pcPara_FOT_PEEPHigh->EnableWindow(FALSE);
				}
				else
				{
					if(m_pcPara_FOT_Steps)
						m_pcPara_FOT_Steps->EnableWindow(TRUE);
					if(m_pcPara_FOT_PEEPLow)
						m_pcPara_FOT_PEEPLow->EnableWindow(TRUE);
					if(m_pcPara_FOT_PEEPHigh)
						m_pcPara_FOT_PEEPHigh->EnableWindow(TRUE);
				}
			}
			else
			{
				if(m_pcPara_FOT_Steps)
					m_pcPara_FOT_Steps->EnableWindow(FALSE);
				if(m_pcPara_FOT_PEEPLow)
					m_pcPara_FOT_PEEPLow->EnableWindow(FALSE);
				if(m_pcPara_FOT_PEEPHigh)
					m_pcPara_FOT_PEEPHigh->EnableWindow(FALSE);
			}
		}

		
		//Button------decrease FOT steps---------------------------------
		btn.wID					= IDC_BTN_DECREASE_FOT;
		btn.poPosition.x		= 7;
		btn.poPosition.y		= 150;//223;
		btn.pcBmpUp				= m_pcDecreaseSeq_Up;
		btn.pcBmpDown			= m_pcDecreaseSeq_Dw;
		btn.pcBmpFocus			= m_pcDecreaseSeq_Up;
		btn.pcBmpDisabled		= m_pcDecreaseSeqx_Dis;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcBtnDecreaseSeq=new CPushBtn(btn,COLOR_TXTBTNUP,false);
		m_pcBtnDecreaseSeq->Create(this,g_hf9AcuBold,0);
		m_pcBtnDecreaseSeq->SetText(_T(""));
		m_pcBtnDecreaseSeq->ShowWindow(SW_HIDE);



		//Button------continue FOT Start/Stop---------------------------------
		btn.wID					= IDC_BTN_STARTSTOP_FOT;	
		btn.poPosition.x		= 85;
		btn.poPosition.y		= 150;//235;
		btn.pcBmpUp				= m_pcStartStopSeq_Up;
		btn.pcBmpDown			= m_pcStartStopSeq_Dw;
		btn.pcBmpFocus			= m_pcStartStopSeq_Up;
		btn.pcBmpDisabled		= m_pcStartStopSeq_Dis;
		btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

		m_pcBtnStartStopSeq=new CUpDwnBtn(btn,COLOR_TXTBTNUP,false,true);
		m_pcBtnStartStopSeq->Create(this,g_hf14AcuMed,0);
		m_pcBtnStartStopSeq->SetText(_T(""),_T(""));
		m_pcBtnStartStopSeq->ShowWindow(SW_HIDE);
		
		
		

		//Button------repeat last FOT steps---------------------------------
		btn.wID					= IDC_BTN_REPEAT_FOT;
		btn.poPosition.x		= 143;
		btn.poPosition.y		= 150;//223;
		btn.pcBmpUp				= m_pcRepeatSeq_Up;
		btn.pcBmpDown			= m_pcRepeatSeq_Dw;
		btn.pcBmpFocus			= m_pcRepeatSeq_Up;
		btn.pcBmpDisabled		= m_pcRepeatSeq_Dis;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcBtnRepeatSeq=new CPushBtn(btn,COLOR_TXTBTNUP,false);
		m_pcBtnRepeatSeq->Create(this,g_hf9AcuBold,0);
		m_pcBtnRepeatSeq->SetText(_T(""));
		m_pcBtnRepeatSeq->ShowWindow(SW_HIDE);


		//Button------run FOT steps---------------------------------
		btn.wID					= IDC_BTN_CONTINUE_FOT;
		btn.poPosition.x		= 201;
		btn.poPosition.y		= 150;//223;
		btn.pcBmpUp				= m_pcRunSeq_Up;
		btn.pcBmpDown			= m_pcRunSeq_Dw;
		btn.pcBmpFocus			= m_pcRunSeq_Up;
		btn.pcBmpDisabled		= m_pcRunSeq_Dis;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcBtnRunSeq=new CPushBtn(btn,COLOR_TXTBTNUP,false);
		m_pcBtnRunSeq->Create(this,g_hf9AcuBold,0);
		m_pcBtnRunSeq->SetText(_T(""));
		m_pcBtnRunSeq->ShowWindow(SW_HIDE);

		drawFOTmenubar();

		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		SelectObject(m_hdcStatic, hpenprevStat);
		SelectObject(m_hdcStatic,hbrprevStat);
		

		bRes=true;
	}
	else
		bRes=false;

	if(bRes)
		Init();
	

	return bRes;
}

void CWndDataFOT::Init()
{
	/*if(m_bFOTrunning)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_DRAW_FOTSTEP);
	}*/
}

void CWndDataFOT::Show(bool bShow)
{
	if(bShow)
	{
		SetWindowPos(NULL,0,0,m_lX,m_lX,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
		Draw(true);
	}
	else
	{
		SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);
	}
}

void CWndDataFOT::OnDestroy()
{
	CWnd::OnDestroy();

	KillTimer(FOT_HORGLASS_TIMER);
	if(m_pDlg)
	{
		m_pDlg->EndDialog(IDCLOSE);
	}
	m_pDlg = NULL;

	if(m_hdcStatic)
		DeleteDC(m_hdcStatic);

	if(m_hbmpStatic)
		DeleteObject(m_hbmpStatic);
	
	if(m_pcBtnStartStopSeq)
		delete m_pcBtnStartStopSeq;
	m_pcBtnStartStopSeq=NULL;

	if(m_pcBtnRunSeq)
		delete m_pcBtnRunSeq;
	m_pcBtnRunSeq=NULL;

	if(m_pcBtnRepeatSeq)
		delete m_pcBtnRepeatSeq;
	m_pcBtnRepeatSeq=NULL;

	if(m_pcBtnDecreaseSeq)
		delete m_pcBtnDecreaseSeq;
	m_pcBtnDecreaseSeq=NULL;


	if(m_pcStartStopSeq_Dw)
		delete m_pcStartStopSeq_Dw;
	m_pcStartStopSeq_Dw=NULL;
	if(m_pcStartStopSeq_Up)
		delete m_pcStartStopSeq_Up;
	m_pcStartStopSeq_Up=NULL;
	if(m_pcStartStopSeq_Dis)
		delete m_pcStartStopSeq_Dis;
	m_pcStartStopSeq_Dis=NULL;

	if(m_pcRunSeq_Dw)
		delete m_pcRunSeq_Dw;
	m_pcRunSeq_Dw=NULL;
	if(m_pcRunSeq_Up)
		delete m_pcRunSeq_Up;
	m_pcRunSeq_Up=NULL;
	if(m_pcRunSeq_Dis)
		delete m_pcRunSeq_Dis;
	m_pcRunSeq_Dis=NULL;

	if(m_pcRepeatSeq_Dw)
		delete m_pcRepeatSeq_Dw;
	m_pcRepeatSeq_Dw=NULL;
	if(m_pcRepeatSeq_Up)
		delete m_pcRepeatSeq_Up;
	m_pcRepeatSeq_Up=NULL;
	if(m_pcRepeatSeq_Dis)
		delete m_pcRepeatSeq_Dis;
	m_pcRepeatSeq_Dis=NULL;

	if(m_pcDecreaseSeq_Dw)
		delete m_pcDecreaseSeq_Dw;
	m_pcDecreaseSeq_Dw=NULL;
	if(m_pcDecreaseSeq_Up)
		delete m_pcDecreaseSeq_Up;
	m_pcDecreaseSeq_Up=NULL;
	if(m_pcDecreaseSeqx_Dis)
		delete m_pcDecreaseSeqx_Dis;
	m_pcDecreaseSeqx_Dis=NULL;


	

	if(m_pcPara_FOT_Steps)
		delete m_pcPara_FOT_Steps;
	m_pcPara_FOT_Steps=NULL;
	
	if(m_pcPara_FOT_PmeanLow)
		delete m_pcPara_FOT_PmeanLow;
	m_pcPara_FOT_PmeanLow=NULL;
	
	if(m_pcPara_FOT_PmeanHigh)
		delete m_pcPara_FOT_PmeanHigh;
	m_pcPara_FOT_PmeanHigh=NULL;

	if(m_pcPara_FOT_PEEPLow)
		delete m_pcPara_FOT_PEEPLow;
	m_pcPara_FOT_PEEPLow=NULL;

	if(m_pcPara_FOT_PEEPHigh)
		delete m_pcPara_FOT_PEEPHigh;
	m_pcPara_FOT_PEEPHigh=NULL;

	
	if(m_pcPara_UpT)
		delete m_pcPara_UpT;
	m_pcPara_UpT=NULL;
	
	if(m_pcPara_FcT)
		delete m_pcPara_FcT;
	m_pcPara_FcT=NULL;
	
	if(m_pcPara_DwT)
		delete m_pcPara_DwT;
	m_pcPara_DwT=NULL;
	
	if(m_pcPara_UpB)
		delete m_pcPara_UpB;
	m_pcPara_UpB=NULL;
	
	if(m_pcPara_FcB)
		delete m_pcPara_FcB;
	m_pcPara_FcB=NULL;
	
	if(m_pcPara_DwB)
		delete m_pcPara_DwB;
	m_pcPara_DwB=NULL;

	delete m_pcWait;
	m_pcWait=NULL;
	delete m_pcWait1;
	m_pcWait1=NULL;
	delete m_pcWait2;
	m_pcWait2=NULL;
	delete m_pcWait3;
	m_pcWait3=NULL;
	delete m_pcWait4;
	m_pcWait4=NULL;
	delete m_pcWait5;
	m_pcWait5=NULL;
	delete m_pcWait6;
	m_pcWait6=NULL;
	delete m_pcWait7;
	m_pcWait7=NULL;
	delete m_pcWait8;
	m_pcWait8=NULL;

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);
}

void CWndDataFOT::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// Do not call CWnd::OnPaint() for painting messages
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

void CWndDataFOT::drawFOTmenubar()
{
	m_pcBtnStartStopSeq->ShowWindow(SW_SHOW);
	m_pcBtnRunSeq->ShowWindow(SW_SHOW);
	m_pcBtnRepeatSeq->ShowWindow(SW_SHOW);
	m_pcBtnDecreaseSeq->ShowWindow(SW_SHOW);

	bool bDecrease=true;
	BYTE stepsComplete=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara();
	BYTE stepCurrent=getModel()->getFOTThread()->getCurFOTsequence();

	if(true==getModel()->getFOTThread()->isDecreasingSequence())
	{
		bDecrease=false;
	}
	else if(stepsComplete!=0)
	{
		if(		stepsComplete>3
			&&	((stepsComplete+1)/2)<(stepCurrent+1))
		{
 			bDecrease=false;
		}
		else if(stepCurrent==1)
		{
			bDecrease=false;
		}
	}

	if(getModel()->getFOTThread())
	{
		SequenceStatesFOT curSeq=getModel()->getFOTThread()->getFOTstate();

		switch(curSeq)
		{
		case FOT_VENTDELAY:
			{
				m_pcBtnStartStopSeq->EnableWindow(TRUE);
				m_pcBtnStartStopSeq->Depress(true);

				m_pcBtnRunSeq->EnableWindow(TRUE);
				m_pcBtnRepeatSeq->EnableWindow(FALSE);
				if(!bDecrease)
				{
					m_pcBtnDecreaseSeq->EnableWindow(FALSE);
				}
				else if(		false==getModel()->getFOTThread()->isDecreasingSequence() 
					&& getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara()>3)
					m_pcBtnDecreaseSeq->EnableWindow(TRUE);
				else
					m_pcBtnDecreaseSeq->EnableWindow(FALSE);
			}
			break;
		case FOT_STARTSTEP:
		case FOT_WAITCPAP:
		case FOT_WAITVALIDDATA:
		case FOT_ACQUIREDATA:
			{
				m_pcBtnStartStopSeq->EnableWindow(TRUE);
				m_pcBtnStartStopSeq->Depress(true);

				m_pcBtnRunSeq->EnableWindow(FALSE);
				m_pcBtnRepeatSeq->EnableWindow(FALSE);
				m_pcBtnDecreaseSeq->EnableWindow(FALSE);
			}
			break;
		case FOT_RETRY:
			{
				m_pcBtnStartStopSeq->EnableWindow(TRUE);
				m_pcBtnStartStopSeq->Depress(true);

				m_pcBtnRunSeq->EnableWindow(FALSE);
				m_pcBtnRepeatSeq->EnableWindow(TRUE);
				if(!bDecrease)
				{
					m_pcBtnDecreaseSeq->EnableWindow(FALSE);
				}
				else if(		false==getModel()->getFOTThread()->isDecreasingSequence() 
					&& getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara()>3
					&& getModel()->getFOTThread()->getCurFOTsequence()>1)
					m_pcBtnDecreaseSeq->EnableWindow(TRUE);
				else
					m_pcBtnDecreaseSeq->EnableWindow(FALSE);
			}
			break;
		case FOT_WAITCONTINUE:
			{
				m_pcBtnStartStopSeq->Depress(true);
				m_pcBtnStartStopSeq->EnableWindow(TRUE);

				m_pcBtnRunSeq->EnableWindow(TRUE);
				m_pcBtnRepeatSeq->EnableWindow(TRUE);
				if(!bDecrease)
				{
					m_pcBtnDecreaseSeq->EnableWindow(FALSE);
				}
				else if(		false==getModel()->getFOTThread()->isDecreasingSequence() 
					&& getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara()>3)
					m_pcBtnDecreaseSeq->EnableWindow(TRUE);
				else
					m_pcBtnDecreaseSeq->EnableWindow(FALSE);
			}
			break;
		case FOT_OFF:
		default:
			{
				//handle start/stop button alarm dependend
				if(getModel()->getALARMHANDLER()->isFOTrelevantAlarmActive()==false)
				{
					m_pcBtnStartStopSeq->DrawDirectUp();
					m_pcBtnStartStopSeq->EnableWindow(TRUE);
				}
				else
				{
					m_pcBtnStartStopSeq->DrawDirectUp();
					m_pcBtnStartStopSeq->EnableWindow(FALSE);
				}

				m_pcBtnRunSeq->EnableWindow(FALSE);
				m_pcBtnRepeatSeq->EnableWindow(FALSE);
				m_pcBtnDecreaseSeq->EnableWindow(FALSE);
			}
			break;
		}

	}
	else
	{
		m_pcBtnStartStopSeq->DrawDirectUp();
		m_pcBtnStartStopSeq->EnableWindow(FALSE);
		m_pcBtnRunSeq->EnableWindow(FALSE);
		m_pcBtnRepeatSeq->EnableWindow(FALSE);
		m_pcBtnDecreaseSeq->EnableWindow(FALSE);
	}
}


void CWndDataFOT::Draw(bool bStatic)
{
	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CBrush cbrRoundNoData(DARKGREY);
	CBrush cbrRoundData(RGB(0,190,0));
	CBrush cbrBack(BACKGND);
	CBrush cbrLightGrey(LIGHTGREY);
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf8AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	if(bStatic)
		DrawStatic();

	BitBlt(m_hDC, 0, 0, m_lX, m_lY, m_hdcStatic, 0, 0, SRCCOPY);
	BitBlt(hdcMem, 0, 0, m_lX, m_lY,m_hdcStatic , 0, 0, SRCCOPY);
	
	if(getModel()->getFOTThread())
		feFOTstate=getModel()->getFOTThread()->getFOTstate();
	else
		feFOTstate=FOT_OFF;

	CString szERROR=_T("");
	CString csText=_T("");
	CString szUnitPRESSURE=_T("");
	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		szUnitPRESSURE=getModel()->GetLanguageString(IDS_UNIT_MBAR);
	}
	else
	{
		szUnitPRESSURE=getModel()->GetLanguageString(IDS_UNIT_CMH2O);
	}


	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		SelectObject(hdcMem,g_hf14AcuBold);

		if(feFOTstate==FOT_OFF)
		{
			SetTextColor(hdcMem,LIGHTRED);

			if(m_bHourglassRun)
			{
				stopHourglass();
			}

			//hide button continue
			//m_pcBtnRunSeq->ShowWindow(SW_SHOW);

			SelectObject(hdcMem,cbrRoundNoData);
			SelectObject(hdcMem,(HPEN)GetStockObject(NULL_PEN));
			RoundRect(hdcMem, 102, 97, 187, 145,10,10);

			rc.left = 102;  
			rc.top = 97;  
			rc.right  = 187;  
			rc.bottom = 145;

			SelectObject(hdcMem,g_hf17AcuBold);
			DrawText(hdcMem,_T("-")+getModel()->GetLanguageString(IDS_TXT_OFF)+_T("-"),-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
		else
		{
			if(		feFOTstate!=FOT_OFF
				&&	feFOTstate!=FOT_RETRY
				&&	feFOTstate!=FOT_WAITCONTINUE)
			{
				//hide button continue
				m_pcBtnRunSeq->ShowWindow(SW_SHOW);

				if(feFOTstate==FOT_ACQUIREDATA)
				{
					SetTextColor(hdcMem,RGB(0,0,0));
					SelectObject(hdcMem,cbrRoundData);
					if(m_bHourglassRun)
					{
						stopHourglass();
					}
					rc.left = 102;  
					rc.top = 97;  
					rc.right  = 187;  
					rc.bottom = 145;

					csText.Format(_T("%d %s"), m_iCountFOTimer,getModel()->GetLanguageString(IDS_UNIT_SECONDS));
				}
				else if(feFOTstate==FOT_WAITVALIDDATA)
				{
					SetTextColor(hdcMem,LIGHTRED);
					SelectObject(hdcMem,cbrRoundNoData);
					if(!m_bHourglassRun)
					{
						startHourglass();
					}
				}
				else
				{
					SetTextColor(hdcMem,LIGHTRED);

					SelectObject(hdcMem,cbrRoundNoData);
					if(m_bHourglassRun)
					{
						stopHourglass();
					}

					rc.left = 102;  
					rc.top = 97;  
					rc.right  = 187;  
					rc.bottom = 145;

					csText.Format(_T("%d %s"), m_iCountFOTimer,getModel()->GetLanguageString(IDS_UNIT_SECONDS));
				}


				SelectObject(hdcMem,(HPEN)GetStockObject(NULL_PEN));
				RoundRect(hdcMem, 102, 97, 187, 145,10,10);

				DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

				SelectObject(hdcMem,g_hf8AcuBold);
				SetTextColor(hdcMem,RGB(0,0,0));
				
				rc.left = 7;  
				rc.top = 210;
				rc.right  = 143;
				rc.bottom = 275;

				WORD icurFOTHFPmean=0;
				if(getModel()->getFOTThread())
					icurFOTHFPmean=getModel()->getFOTThread()->getCurrentFOTHFPmean();

				//calculate current PIP
				SHORT iCurPIP=icurFOTHFPmean+(getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara()/2);
				
				csText=_T("Pmean now");
				DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

				SelectObject(hdcMem,g_hf21AcuBold);
				csText.Format(_T("%0.1f"), CTlsFloat::Round(((double)icurFOTHFPmean)/10, 1));
				DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

				SelectObject(hdcMem,g_hf7AcuNorm);
				csText.Format(_T("%s"), szUnitPRESSURE);
				DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_BOTTOM|DT_SINGLELINE);

				SelectObject(hdcMem,g_hf8AcuBold);
				rc.left = 143;  
				rc.top = 210;
				rc.right  = 279;
				rc.bottom = 275;

				csText=getModel()->GetLanguageString(IDS_TXT_FOT_PIPNOW);
				//csText=_T("PIPnow");
				DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

				SelectObject(hdcMem,g_hf21AcuBold);
				csText.Format(_T("%d"), iCurPIP/10);
				DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				 
				SelectObject(hdcMem,g_hf7AcuNorm);
				csText.Format(_T("%s"), szUnitPRESSURE);
				DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
				
				
			}
			else
			{
				if(feFOTstate==FOT_ACQUIREDATA)
					SetTextColor(hdcMem,BLACK);
				else
					SetTextColor(hdcMem,LIGHTRED);

				if(m_bHourglassRun)
				{
					stopHourglass();
				}

				SelectObject(hdcMem,cbrRoundNoData);
				SelectObject(hdcMem,(HPEN)GetStockObject(NULL_PEN));
				RoundRect(hdcMem, 102, 97, 187, 145,10,10);

				if(getModel()->getFOTThread())
				{
					if(getModel()->getFOTThread()->getCurFOTsequence()<getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara())
					{
						//show button continue
						BYTE iFOTnextsequence=0;
						
						CString sz1=_T("");
						if(feFOTstate==FOT_RETRY)
						{
							sz1.Format(_T("%s %d/%d"), getModel()->GetLanguageString(IDS_TXT_FOT_STEP),getModel()->getFOTThread()->getCurFOTsequence(),getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara());
							iFOTnextsequence=getModel()->getFOTThread()->getCurFOTsequence();
						}
						else
						{
							sz1.Format(_T("%s %d/%d"), getModel()->GetLanguageString(IDS_TXT_FOT_STEP),getModel()->getFOTThread()->getCurFOTsequence()+1,getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara());
							iFOTnextsequence=getModel()->getFOTThread()->getCurFOTsequence()+1;
						}
						
						
						SetTextColor(hdcMem,RGB(0,0,0));
						SelectObject(hdcMem,g_hf8AcuBold);
						rc.left = 102;  
						rc.top = 97+5;  
						rc.right  = 187;  
						rc.bottom = 145;
						DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_FOT_NEXT),-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
						//DrawText(hdcMem,_T("next"),-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
						
						SelectObject(hdcMem,g_hf8AcuBold);
						rc.left = 102;  
						rc.top = 97;  
						rc.right  = 187;  
						rc.bottom = 145-5;
						DrawText(hdcMem,sz1,-1,&rc,DT_CENTER|DT_BOTTOM|DT_SINGLELINE);

						SelectObject(hdcMem,g_hf8AcuBold);

						if(feFOTstate==FOT_RETRY)
						{
							rc.top = 225;
							rc.bottom = 275;
							rc.left = 7;  
							rc.right  = 279;


							SetTextColor(hdcMem,RGB(255,0,0));
							switch(getModel()->getFOTThread()->getRetryERROR())
							{
							case RETRY_PRESSURE:
								{
									//csText=_T("TARGET PRESSURE not reached");
									csText=getModel()->GetLanguageString(IDS_TXT_FOT_PRESSURE);
								}
								break;
							case RETRY_LEAK:
								{
									csText=getModel()->GetLanguageString(IDS_TXT_FOT_LEAK);
									//csText=_T("LEAK high");
								}
								break;
							case RETRY_FSI:
								{
									csText=getModel()->GetLanguageString(IDS_TXT_FOT_POORQUALITY);
									//csText=getModel()->GetLanguageString(IDS_TXT_FOT_FSI);
									//csText=_T("FSI high");
								}
								break;
							case RETRY_RESISTANCE:
								{
									csText=getModel()->GetLanguageString(IDS_TXT_FOT_POORQUALITY);
									//csText=getModel()->GetLanguageString(IDS_TXT_FOT_RESISTANCE);
									//csText=_T("RESISTANCE low");
								}
								break;
							case RETRY_REACTANCE:
								{
									csText=getModel()->GetLanguageString(IDS_TXT_FOT_POORQUALITY);
									//csText=getModel()->GetLanguageString(IDS_TXT_FOT_REACTANCE);
									//csText=_T("REACTANCE out of range");
								}
								break;
							case RETRY_QUALITY:
								{
									csText=getModel()->GetLanguageString(IDS_TXT_FOT_POORQUALITY);
								}
								break;
							default:
								{
									csText=_T(" ");
								}
								break;
							}
							
							DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

							rc.top = 220;//257;
							//csText=_T("- retry step -");
							csText=getModel()->GetLanguageString(IDS_TXT_FOT_RETRY);
							DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
						}
						else
						{
							//calculate next used values
							WORD iPmeanSTART=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_PMEANSTARTPara();
							WORD icurFOTHFPmean=0;
							WORD iFOTHFPmeanStep=0;
							if(getModel()->getFOTThread())
							{
								icurFOTHFPmean=getModel()->getFOTThread()->getCurrentFOTHFPmean();
								iFOTHFPmeanStep=getModel()->getFOTThread()->getFOTHFPmeanStep();
							}
							
							WORD iSteps=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara();

							WORD curValue=0;
							if(iSteps==1)
							{
								curValue=iPmeanSTART;
							}
							else
							{
								if(iFOTnextsequence==1)
								{
									curValue=icurFOTHFPmean;
								}
								else if(iFOTnextsequence==iSteps)
								{
									curValue=iPmeanSTART;
								}
								else if(iFOTnextsequence<=(iSteps+1)/2)
								{
									curValue=icurFOTHFPmean+iFOTHFPmeanStep;
								}
								else
								{
									curValue=icurFOTHFPmean-iFOTHFPmeanStep;
								}
							}

							
							//calculate next PIP
							SHORT iNextPIP=0;

							if(getModel()->getFOTThread())
								iNextPIP=curValue+(getModel()->getFOTThread()->getOriginHFAMPLPara()/2);

							rc.left = 7;  
							rc.top = 210;
							rc.right  = 143;
							rc.bottom = 275;

							SelectObject(hdcMem,g_hf8AcuBold);
							//csText=_T("Pmean next");
							csText=getModel()->GetLanguageString(IDS_TXT_FOT_PMEANNEXT);
							DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

							SelectObject(hdcMem,g_hf21AcuBold);
							csText.Format(_T("%0.1f"), CTlsFloat::Round(((double)curValue)/10, 1));
							DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

							SelectObject(hdcMem,g_hf7AcuNorm);
							csText.Format(_T("%s"), szUnitPRESSURE);
							DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_BOTTOM|DT_SINGLELINE);

							rc.left = 143;  
							rc.top = 210;
							rc.right  = 279;
							rc.bottom = 275;

							SelectObject(hdcMem,g_hf8AcuBold);
							//csText=_T("PIPnext");
							csText=getModel()->GetLanguageString(IDS_TXT_FOT_PIPNEXT);
							DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

							SelectObject(hdcMem,g_hf21AcuBold);
							csText.Format(_T("%d"), iNextPIP/10);
							DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

							SelectObject(hdcMem,g_hf7AcuNorm);
							csText.Format(_T("%s"), szUnitPRESSURE);
							DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_BOTTOM|DT_SINGLELINE);

						}
					}
					else
					{
						//m_pcBtnRunSeq->ShowWindow(SW_SHOW);

						szERROR.Format(_T("ERROR state %d CurFOTseq %d hfo_STEPS %d\r\n"),(int)feFOTstate,getModel()->getFOTThread()->getCurFOTsequence(),getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara());
						DEBUGMSG(TRUE, (szERROR));
						theApp.getLog()->WriteLine(szERROR);
					}
				}
				/*else
				{
					m_pcBtnRunSeq->ShowWindow(SW_SHOW);
				}*/
			}
		}
	}
	else
	{
		SelectObject(hdcMem,g_hf14AcuBold);

		if(feFOTstate==FOT_OFF)
		{
			SetTextColor(hdcMem,LIGHTRED);

			if(m_bHourglassRun)
			{
				stopHourglass();
			}

			//hide button continue
			//m_pcBtnRunSeq->ShowWindow(SW_SHOW);

			SelectObject(hdcMem,cbrRoundNoData);
			SelectObject(hdcMem,(HPEN)GetStockObject(NULL_PEN));
			RoundRect(hdcMem, 102, 97, 187, 145,10,10);

			rc.left = 102;  
			rc.top = 97;  
			rc.right  = 187;  
			rc.bottom = 145;

			SelectObject(hdcMem,g_hf17AcuBold);
			DrawText(hdcMem,_T("-")+getModel()->GetLanguageString(IDS_TXT_OFF)+_T("-"),-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
		else
		{
			if(		feFOTstate!=FOT_OFF
				&&	feFOTstate!=FOT_RETRY
				&&	feFOTstate!=FOT_WAITCONTINUE)
			{
				if(feFOTstate==FOT_ACQUIREDATA)
				{
					SetTextColor(hdcMem,RGB(0,0,0));
					SelectObject(hdcMem,cbrRoundData);
					if(m_bHourglassRun)
					{
						stopHourglass();
					}
					rc.left = 102;  
					rc.top = 97;  
					rc.right  = 187;  
					rc.bottom = 145;

					csText.Format(_T("%d %s"), m_iCountFOTimer, getModel()->GetLanguageString(IDS_UNIT_SECONDS));
				}
				else if(feFOTstate==FOT_WAITVALIDDATA)
				{
					SetTextColor(hdcMem,LIGHTRED);
					SelectObject(hdcMem,cbrRoundNoData);
					if(!m_bHourglassRun)
					{
						startHourglass();
					}
				}
				else
				{
					SetTextColor(hdcMem,LIGHTRED);

					SelectObject(hdcMem,cbrRoundNoData);
					if(m_bHourglassRun)
					{
						stopHourglass();
					}

					rc.left = 102;  
					rc.top = 97;  
					rc.right  = 187;  
					rc.bottom = 145;

					csText.Format(_T("%d %s"), m_iCountFOTimer,getModel()->GetLanguageString(IDS_UNIT_SECONDS));//todo FOTLANGUAGE
				}

				SelectObject(hdcMem,(HPEN)GetStockObject(NULL_PEN));
				RoundRect(hdcMem, 102, 97, 187, 145,10,10);

				DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

				SelectObject(hdcMem,g_hf8AcuBold);
				SetTextColor(hdcMem,RGB(0,0,0));

				rc.left = 7;  
				rc.top = 210;
				rc.right  = 143;
				rc.bottom = 275;

				WORD icurFOTPEEP=0;
				if(getModel()->getFOTThread())
					icurFOTPEEP=getModel()->getFOTThread()->getCurrentFOTPEEP();
				WORD curPIPValue=0;

				if(true==getModel()->getFOTThread()->isFOToriginVGstateOn())
				{
					curPIPValue=getModel()->getDATAHANDLER()->PARADATA()->GetPmaxVolGPara_IPPV();
					//DEBUGMSG(TRUE, (TEXT("PIPnow1() 1 %d\r\n"),curPIPValue));
				}
				else
				{
					curPIPValue=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPPara_IPPV();
					//DEBUGMSG(TRUE, (TEXT("PIPnow1() 2 %d\r\n"),curPIPValue));
				}
				
				csText=_T("PEEPnow");
				DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

				SelectObject(hdcMem,g_hf21AcuBold);
				csText.Format(_T("%0.1f"), CTlsFloat::Round(((double)icurFOTPEEP)/10, 1));
				DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

				SelectObject(hdcMem,g_hf7AcuNorm);
				csText.Format(_T("%s"), szUnitPRESSURE);
				DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_BOTTOM|DT_SINGLELINE);

				SelectObject(hdcMem,g_hf8AcuBold);
				rc.left = 143;  
				rc.top = 210;
				rc.right  = 279;
				rc.bottom = 275;

				//DEBUGMSG(TRUE, (TEXT("PIPnow1() %d\r\n"),curPIPValue/10));

				//csText=_T("PIPnow");
				csText=getModel()->GetLanguageString(IDS_TXT_FOT_PIPNOW);
				DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

				SelectObject(hdcMem,g_hf21AcuBold);
				csText.Format(_T("%d"), curPIPValue/10);
				//csText.Format(_T("%0.1f"), CTlsFloat::Round(((double)curPIPValue)/10, 1));
				DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

				SelectObject(hdcMem,g_hf7AcuNorm);
				csText.Format(_T("%s"), szUnitPRESSURE);
				DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
			}
			else
			{
				if(feFOTstate==FOT_ACQUIREDATA)
					SetTextColor(hdcMem,BLACK);
				else
					SetTextColor(hdcMem,LIGHTRED);
				
				if(m_bHourglassRun)
				{
					stopHourglass();
				}

				SelectObject(hdcMem,cbrRoundNoData);
				SelectObject(hdcMem,(HPEN)GetStockObject(NULL_PEN));
				RoundRect(hdcMem, 102, 97, 187, 145,10,10);

				if(getModel()->getFOTThread())
				{
					if(getModel()->getFOTThread()->getCurFOTsequence()<getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara())
					{
						//show button continue
						BYTE iFOTnextsequence=0;
						
						CString sz1=_T("");
						if(feFOTstate==FOT_RETRY)
						{
							sz1.Format(_T("%s %d/%d"), getModel()->GetLanguageString(IDS_TXT_FOT_STEP),getModel()->getFOTThread()->getCurFOTsequence(),getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara());
							iFOTnextsequence=getModel()->getFOTThread()->getCurFOTsequence();
						}
						else
						{
							sz1.Format(_T("%s %d/%d"), getModel()->GetLanguageString(IDS_TXT_FOT_STEP),getModel()->getFOTThread()->getCurFOTsequence()+1,getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara());
							iFOTnextsequence=getModel()->getFOTThread()->getCurFOTsequence()+1;
						}
						
						//m_pcBtnRunSeq->ShowWindow(SW_SHOW);

						SetTextColor(hdcMem,RGB(0,0,0));
						SelectObject(hdcMem,g_hf8AcuBold);
						rc.left = 102;  
						rc.top = 97+5;  
						rc.right  = 187;  
						rc.bottom = 145;
						DrawText(hdcMem,getModel()->GetLanguageString(IDS_TXT_FOT_NEXT),-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);//todo FOTLANGUAGE
						
						SelectObject(hdcMem,g_hf8AcuBold);
						rc.left = 102;  
						rc.top = 97;  
						rc.right  = 187;  
						rc.bottom = 145-5;
						DrawText(hdcMem,sz1,-1,&rc,DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
						
						SelectObject(hdcMem,g_hf8AcuBold);

						if(feFOTstate==FOT_RETRY)
						{
							rc.top = 225;
							rc.bottom = 275;
							rc.left = 7;  
							rc.right  = 279;
							

							SetTextColor(hdcMem,RGB(255,0,0));
							switch(getModel()->getFOTThread()->getRetryERROR())
							{
							case RETRY_PRESSURE:
								{
									csText=getModel()->GetLanguageString(IDS_TXT_FOT_PRESSURE);
									//csText=_T("TARGET PRESSURE not reached");
								}
								break;
							case RETRY_LEAK:
								{
									csText=getModel()->GetLanguageString(IDS_TXT_FOT_LEAK);
									//csText=_T("LEAK high");
								}
								break;
							case RETRY_FSI:
								{
									csText=getModel()->GetLanguageString(IDS_TXT_FOT_POORQUALITY);
									//csText=getModel()->GetLanguageString(IDS_TXT_FOT_FSI);
									//csText=_T("FSI high");
								}
								break;
							case RETRY_RESISTANCE:
								{
									csText=getModel()->GetLanguageString(IDS_TXT_FOT_POORQUALITY);
									//csText=getModel()->GetLanguageString(IDS_TXT_FOT_RESISTANCE);
									//csText=_T("RESISTANCE low");
								}
								break;
							case RETRY_REACTANCE:
								{
									csText=getModel()->GetLanguageString(IDS_TXT_FOT_POORQUALITY);
									//csText=getModel()->GetLanguageString(IDS_TXT_FOT_REACTANCE);
									//csText=_T("REACTANCE out of range");
								}
								break;
								case RETRY_QUALITY:
								{
									csText=getModel()->GetLanguageString(IDS_TXT_FOT_POORQUALITY);
								}
								break;
							default:
								{
									csText=_T(" ");
								}
								break;
							}
							DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
							
							rc.top = 220;//257;
							//csText=_T("- retry step -");
							csText=getModel()->GetLanguageString(IDS_TXT_FOT_RETRY);
							DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
						}
						else
						{
							//calculate next used values
							WORD iPEEPSTART=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_PEEPSTARTPara();
							WORD iPEEPEND=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_PEEPENDPara();
							WORD iStepsPARA=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara();
							WORD nextPIPValue=0;
							if(true==getModel()->getFOTThread()->isFOToriginVGstateOn())
							{
								nextPIPValue=getModel()->getDATAHANDLER()->PARADATA()->GetPmaxVolGPara_IPPV();
								//DEBUGMSG(TRUE, (TEXT("PIPnext1() 1 %d\r\n"),nextPIPValue));
							}
							else
							{
								nextPIPValue=getModel()->getDATAHANDLER()->PARADATA()->GetPINSPPara_IPPV();
								//DEBUGMSG(TRUE, (TEXT("PIPnext1() 2 %d\r\n"),nextPIPValue));
							}

							WORD curPEEPValue=0;
							WORD icurFOTPEEP=0;
							WORD iFOTPEEPStep=0;
							SHORT iFOToriginDiffPEEP_PINSP=0;
							if(getModel()->getFOTThread())
							{
								icurFOTPEEP=getModel()->getFOTThread()->getCurrentFOTPEEP();
								iFOTPEEPStep=getModel()->getFOTThread()->getFOTPEEPStep();
								if(true==getModel()->getFOTThread()->isFOToriginVGstateOn())
								{
									iFOToriginDiffPEEP_PINSP=getModel()->getFOTThread()->getFOToriginDiffPEEP_PMAXVG();
									//DEBUGMSG(TRUE, (TEXT("PIPnext1() 3 %d\r\n"),iFOToriginDiffPEEP_PINSP));
								}
								else
								{
									iFOToriginDiffPEEP_PINSP=getModel()->getFOTThread()->getFOToriginDiffPEEP_PINSP();
									//DEBUGMSG(TRUE, (TEXT("PIPnext1() 4 %d\r\n"),iFOToriginDiffPEEP_PINSP));
								}
							}
							
							if(iStepsPARA==1)
							{
								curPEEPValue=iPEEPSTART;
								nextPIPValue=curPEEPValue+iFOToriginDiffPEEP_PINSP;
							}
							else
							{
								if(iFOTnextsequence==1)
								{
									curPEEPValue=icurFOTPEEP;
									if(getModel()->getFOTThread())
									{
										if(true==getModel()->getFOTThread()->isFOToriginVGstateOn())
										{
											nextPIPValue=getModel()->getFOTThread()->getFOToriginPMAXVGPara_IPPV();
											//DEBUGMSG(TRUE, (TEXT("PIPnext1() 5 %d\r\n"),nextPIPValue));
										}
										else
										{
											nextPIPValue=getModel()->getFOTThread()->getFOToriginPINSPPara_IPPV();
											//DEBUGMSG(TRUE, (TEXT("PIPnext1() 6 %d\r\n"),nextPIPValue));
										}
									}
								}
								else if(iFOTnextsequence==iStepsPARA)
								{
									curPEEPValue=iPEEPSTART;
									if(getModel()->getFOTThread())
									{
										if(true==getModel()->getFOTThread()->isFOToriginVGstateOn())
										{
											nextPIPValue=getModel()->getFOTThread()->getFOToriginPMAXVGPara_IPPV();
											//DEBUGMSG(TRUE, (TEXT("PIPnext1() 7 %d\r\n"),nextPIPValue));
										}
										else
										{
											nextPIPValue=getModel()->getFOTThread()->getFOToriginPINSPPara_IPPV();
											//DEBUGMSG(TRUE, (TEXT("PIPnext1() 8 %d\r\n"),nextPIPValue));
										}
									}
								}
								else if(iFOTnextsequence<=(iStepsPARA+1)/2)
								{
									curPEEPValue=icurFOTPEEP+iFOTPEEPStep;
									if(curPEEPValue%10!=0 && iFOToriginDiffPEEP_PINSP%10!=0)
									{
										nextPIPValue=curPEEPValue+iFOToriginDiffPEEP_PINSP;
										//DEBUGMSG(TRUE, (TEXT("PIPnext1() 9 %d\r\n"),nextPIPValue));
									}
									else if(iFOToriginDiffPEEP_PINSP%10!=0)
									{
										nextPIPValue=curPEEPValue+5+iFOToriginDiffPEEP_PINSP;
										//DEBUGMSG(TRUE, (TEXT("PIPnext1() 10 %d\r\n"),nextPIPValue));
									}
									else if(curPEEPValue%10!=0)
									{
										nextPIPValue=curPEEPValue+5+iFOToriginDiffPEEP_PINSP;
										//DEBUGMSG(TRUE, (TEXT("PIPnext1() 11 %d\r\n"),nextPIPValue));
									}
									else
									{
										nextPIPValue=curPEEPValue+iFOToriginDiffPEEP_PINSP;
										//DEBUGMSG(TRUE, (TEXT("PIPnext1() 12 %d\r\n"),nextPIPValue));
									}
								}
								else
								{
									curPEEPValue=icurFOTPEEP-iFOTPEEPStep;
									if(curPEEPValue%10!=0 && iFOToriginDiffPEEP_PINSP%10!=0)
									{
										nextPIPValue=curPEEPValue+iFOToriginDiffPEEP_PINSP;
										//DEBUGMSG(TRUE, (TEXT("PIPnext1() 13 %d\r\n"),nextPIPValue));
									}
									else if(iFOToriginDiffPEEP_PINSP%10!=0)
									{
										nextPIPValue=curPEEPValue+5+iFOToriginDiffPEEP_PINSP;
										//DEBUGMSG(TRUE, (TEXT("PIPnext1() 14 %d\r\n"),nextPIPValue));
									}
									else if(curPEEPValue%10!=0)
									{
										nextPIPValue=curPEEPValue+5+iFOToriginDiffPEEP_PINSP;
										//DEBUGMSG(TRUE, (TEXT("PIPnext1() 15 %d\r\n"),nextPIPValue));
									}
									else
									{
										nextPIPValue=curPEEPValue+iFOToriginDiffPEEP_PINSP;
										//DEBUGMSG(TRUE, (TEXT("PIPnext1() 16 %d\r\n"),nextPIPValue));
									}
								}
							}

							//limit PIPnext to 80 mbar
							if(nextPIPValue>800)
								nextPIPValue=800;

							rc.left = 7;  
							rc.top = 210;
							rc.right  = 143;
							rc.bottom = 275;

							SelectObject(hdcMem,g_hf8AcuBold);
							//csText=_T("PEEPnext");
							csText=getModel()->GetLanguageString(IDS_TXT_FOT_PEEPNEXT);
							DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

							SelectObject(hdcMem,g_hf21AcuBold);
							csText.Format(_T("%0.1f"), CTlsFloat::Round(((double)curPEEPValue)/10, 1));
							DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

							SelectObject(hdcMem,g_hf7AcuNorm);
							csText.Format(_T("%s"), szUnitPRESSURE);
							DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_BOTTOM|DT_SINGLELINE);

							SelectObject(hdcMem,g_hf8AcuBold);
							rc.left = 143;  
							rc.top = 210;
							rc.right  = 279;
							rc.bottom = 275;

							//DEBUGMSG(TRUE, (TEXT("PIPnext1() %d\r\n"),nextPIPValue/10));

							csText=getModel()->GetLanguageString(IDS_TXT_FOT_PIPNEXT);
							//csText=_T("PIPnext");
							DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
							
							SelectObject(hdcMem,g_hf21AcuBold);
							csText.Format(_T("%d"), nextPIPValue/10);
							DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

							SelectObject(hdcMem,g_hf7AcuNorm);
							csText.Format(_T("%s"), szUnitPRESSURE);
							DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
						}
					}
					else
					{
						szERROR.Format(_T("ERROR state %d CurFOTseq %d conv_STEPS %d\r\n"),(int)feFOTstate,getModel()->getFOTThread()->getCurFOTsequence(),getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara());
						DEBUGMSG(TRUE, (szERROR));
						theApp.getLog()->WriteLine(szERROR);
					}
				}
			}
		}
	}

	rc.left = 7;  
	rc.top = 283;  
	rc.right  = 279;  
	rc.bottom = 295;
	//csText=_T("last calc. ");
	csText=getModel()->GetLanguageString(IDS_TXT_FOT_LASTCALC);
	csText+=_T(" ");
	if(getModel()->getFOTThread())
		csText+=getModel()->getFOTThread()->getDateLastSequence();

	SetTextColor(hdcMem,RGB(0,0,0));
	SelectObject(hdcMem,g_hf7AcuNorm);
	DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	
	BitBlt(m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	if(m_bHourglassRun)
		drawHourglass();

	drawMbarStep();
	drawPIPmax();

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);



	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);

}

void CWndDataFOT::drawPIPmax()
{
	/*if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		if(getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara()==1)
		{
			return;
		}
	}
	else
	{
		if(getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara()==1)
		{
			return;
		}
	}*/

	bool bVGon=false;
	if(		true==getModel()->getFOTThread()->isFOToriginVGstateOn() 
		||	true==getModel()->getALARMHANDLER()->isVgarantAutoTurnedOff_FOT()
		||	true==getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
	{
		bVGon=true;
	}

	RECT rcCl,rc;

	rcCl.left = 197;  
	rcCl.top = 92;  
	rcCl.right  = 282;  
	rcCl.bottom = 145;

	rc.left = 0;  
	rc.top = 0;  
	rc.right  = 85;  
	rc.bottom = 53;

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,85,53);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	CBrush brBack(NUMERIC);
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,brBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf7AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	FillRect(hdcMem,&rc,brBack);

	CString szUnitPRESSURE=_T("");
	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		szUnitPRESSURE=getModel()->GetLanguageString(IDS_UNIT_MBAR);
	}
	else
	{
		szUnitPRESSURE=getModel()->GetLanguageString(IDS_UNIT_CMH2O);
	}

	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		fVALUE fv;
		if(m_pcPara_FOT_Steps)
			m_pcPara_FOT_Steps->GetValue(&fv);
		WORD iSteps=fv.iValue;

		if(m_pcPara_FOT_PmeanLow)
			m_pcPara_FOT_PmeanLow->GetValue(&fv);
		WORD iPMEANlow=fv.iValue;

		if(m_pcPara_FOT_PmeanHigh)
			m_pcPara_FOT_PmeanHigh->GetValue(&fv);
		WORD iPMEANhigh=fv.iValue;

		WORD iHFAMPL=0;
		if(bVGon)
		{
			iHFAMPL=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLmaxPara()/2;
		}
		else
		{
			iHFAMPL=getModel()->getFOTThread()->getOriginHFAMPLPara()/2;
		}

		//calculate max PIP
		SHORT iMaxPIP=0;
		if(iSteps==1)
		{
			iMaxPIP=iPMEANlow+iHFAMPL;
		}
		else
		{
			if(m_pcPara_FOT_PmeanHigh)
				m_pcPara_FOT_PmeanHigh->GetValue(&fv);
			WORD iPMEANEND=fv.iValue;

			iMaxPIP=iPMEANEND+iHFAMPL;
		}

		rc.top = 3;  
		rc.bottom = 50;

		//CString csText=_T("PIPmax");
		CString csText=getModel()->GetLanguageString(IDS_TXT_FOT_PIPMAX);
		DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

		SelectObject(hdcMem,g_hf14AcuBold);
		csText.Format(_T("%0.1f"),CTlsFloat::Round(((double)iMaxPIP)/10, 1));
		DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		SelectObject(hdcMem,g_hf7AcuNorm);
		csText.Format(_T("%s"), szUnitPRESSURE);
		DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
	}
	else
	{
		fVALUE fv;
		if(m_pcPara_FOT_Steps)
			m_pcPara_FOT_Steps->GetValue(&fv);
		WORD iSteps=fv.iValue;

		if(m_pcPara_FOT_PEEPLow)
			m_pcPara_FOT_PEEPLow->GetValue(&fv);
		WORD iPEEPlow=fv.iValue;

		if(m_pcPara_FOT_PEEPHigh)
			m_pcPara_FOT_PEEPHigh->GetValue(&fv);
		WORD iPEEhigh=fv.iValue;

		SHORT iMaxPIP=0;
		if(iSteps==1)
		{
			if(bVGon)
			{
				iMaxPIP=iPEEPlow+getModel()->getDATAHANDLER()->PARADATA()->GetPmaxVolGPara_IPPV()-getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_IPPV();
			}
			else
			{
				iMaxPIP=iPEEPlow+getModel()->getDATAHANDLER()->PARADATA()->GetPINSPPara_IPPV()-getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_IPPV();
			}
		}
		else
		{
			if(m_pcPara_FOT_PEEPHigh)
				m_pcPara_FOT_PEEPHigh->GetValue(&fv);
			WORD iPEEPEND=fv.iValue;

			if(bVGon)
			{
				iMaxPIP=iPEEPEND+getModel()->getDATAHANDLER()->PARADATA()->GetPmaxVolGPara_IPPV()-getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_IPPV();
			}
			else
			{
				iMaxPIP=iPEEPEND+getModel()->getDATAHANDLER()->PARADATA()->GetPINSPPara_IPPV()-getModel()->getDATAHANDLER()->PARADATA()->GetPEEPPara_IPPV();
			}
		}

		//limit PIPmax at 80mbar
		if(iMaxPIP>800)
			iMaxPIP=800;

		rc.top = 3;  
		rc.bottom = 50;

		//CString csText=_T("PIPmax");
		CString csText=getModel()->GetLanguageString(IDS_TXT_FOT_PIPMAX);
		DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

		SelectObject(hdcMem,g_hf14AcuBold);
		csText.Format(_T("%d"), iMaxPIP/10);
		
		DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		SelectObject(hdcMem,g_hf7AcuNorm);
		csText.Format(_T("%s"), szUnitPRESSURE);
		DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
	}


	BitBlt(dc.m_hDC,rcCl.left,rcCl.top,rcCl.right,rcCl.bottom,hdcMem,0,0,SRCCOPY);
	BitBlt(m_hDC,rcCl.left,rcCl.top,rcCl.right,rcCl.bottom,hdcMem,0,0,SRCCOPY);

	SetBkMode(hdcMem,nBkMode);

	SetTextColor(hdcMem,tc);
	SelectObject(hdcMem,hPrevFont);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);
	SelectObject(hdcMem,hBmpMemPrev);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}
void CWndDataFOT::drawMbarStep()
{
	RECT rcCl,rc;

	rcCl.left = 7;  
	rcCl.top = 92;  
	rcCl.right  = 92;  
	rcCl.bottom = 145;

	rc.left = 0;  
	rc.top = 0;  
	rc.right  = 85;  
	rc.bottom = 53;

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,85,53);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	CBrush brBack(NUMERIC);
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,brBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf7AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	FillRect(hdcMem,&rc,brBack);

	CString szUnitPRESSURE=_T("");
	if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
	{
		szUnitPRESSURE=getModel()->GetLanguageString(IDS_UNIT_MBAR);
	}
	else
	{
		szUnitPRESSURE=getModel()->GetLanguageString(IDS_UNIT_CMH2O);
	}

	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		fVALUE fv;
		if(m_pcPara_FOT_Steps)
			m_pcPara_FOT_Steps->GetValue(&fv);
		WORD iSteps=fv.iValue;

		if(m_pcPara_FOT_PmeanLow)
			m_pcPara_FOT_PmeanLow->GetValue(&fv);
		WORD iPMEANlow=fv.iValue;

		if(m_pcPara_FOT_PmeanHigh)
			m_pcPara_FOT_PmeanHigh->GetValue(&fv);
		WORD iPMEANhigh=fv.iValue;

		//calculated Pmean step
		WORD iFOTHFPmeanStep=0;
		if(iSteps==1)
		{

		}
		else
		{
			if((iSteps-1)/2>0)
				iFOTHFPmeanStep=(iPMEANhigh-iPMEANlow)/((iSteps-1)/2);

			iFOTHFPmeanStep=(iFOTHFPmeanStep/5)*5;
		}

		rc.top = 3;  
		rc.bottom = 50;

		//CString csText=_T("step");
		CString csText=getModel()->GetLanguageString(IDS_TXT_FOT_STEP);
		DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

		SelectObject(hdcMem,g_hf14AcuBold);
		if(iSteps>1)
		{
			csText.Format(_T("%0.1f"), CTlsFloat::Round(((double)iFOTHFPmeanStep)/10, 1));
		}
		else
		{
			csText=_T("--");
		}
		DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		SelectObject(hdcMem,g_hf7AcuNorm);
		csText.Format(_T("%s"), szUnitPRESSURE);
		DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
	}
	else
	{
		fVALUE fv;
		if(m_pcPara_FOT_Steps)
			m_pcPara_FOT_Steps->GetValue(&fv);
		WORD iSteps=fv.iValue;

		if(m_pcPara_FOT_PEEPLow)
			m_pcPara_FOT_PEEPLow->GetValue(&fv);
		WORD iPEEPlow=fv.iValue;

		if(m_pcPara_FOT_PEEPHigh)
			m_pcPara_FOT_PEEPHigh->GetValue(&fv);
		WORD iPEEhigh=fv.iValue;

		WORD iPEEPStep=0;
		if(iSteps>1)
		{
			if((iSteps-1)/2>0)
				iPEEPStep=(iPEEhigh-iPEEPlow)/((iSteps-1)/2);

			iPEEPStep=(iPEEPStep/5)*5;
		}

		rc.top = 3;  
		rc.bottom = 50;

		//CString csText=_T("step");
		CString csText=getModel()->GetLanguageString(IDS_TXT_FOT_STEP);
		DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

		SelectObject(hdcMem,g_hf14AcuBold);
		if(iSteps>1)
		{
			csText.Format(_T("%0.1f"), CTlsFloat::Round(((double)iPEEPStep)/10, 1));
		}
		else
		{
			csText=_T("--");
		}
		DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		SelectObject(hdcMem,g_hf7AcuNorm);
		csText.Format(_T("%s"), szUnitPRESSURE);
		DrawText(hdcMem,csText,-1,&rc,DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
	}


	BitBlt(dc.m_hDC,rcCl.left,rcCl.top,rcCl.right,rcCl.bottom,hdcMem,0,0,SRCCOPY);
	BitBlt(m_hDC,rcCl.left,rcCl.top,rcCl.right,rcCl.bottom,hdcMem,0,0,SRCCOPY);

	SetBkMode(hdcMem,nBkMode);

	SetTextColor(hdcMem,tc);
	SelectObject(hdcMem,hPrevFont);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);
	SelectObject(hdcMem,hBmpMemPrev);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}

void CWndDataFOT::drawHourglass()
{
	RECT rcCl,rc;

	rcCl.left = 122;  
	rcCl.top = 97;  
	rcCl.right  = 187;  
	rcCl.bottom = 145;

	rc.left = 0;  
	rc.top = 0;  
	rc.right  = 45;  
	rc.bottom = 47;

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,45,47);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	CBrush brBack(DARKGREY);
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,brBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	
	FillRect(hdcMem,&rc,brBack);

	if(m_bHourglassRun)
	{
		//hourglass 
		if(m_iWaitCount>8)
			m_iWaitCount=1;

		switch(m_iWaitCount)
		{
		case 1:
			{
				m_pcWait=m_pcWait1;
			}
			break;
		case 2:
			{
				m_pcWait=m_pcWait2;
			}
			break;
		case 3:
			{
				m_pcWait=m_pcWait3;
			}
			break;
		case 4:
			{
				m_pcWait=m_pcWait4;
			}
			break;
		case 5:
			{
				m_pcWait=m_pcWait5;
			}
			break;
		case 6:
			{
				m_pcWait=m_pcWait6;
			}
			break;
		case 7:
			{
				m_pcWait=m_pcWait7;
			}
			break;
		case 8:
			{
				m_pcWait=m_pcWait8;
			}
			break;
		default:
			{
				m_pcWait=NULL;
			}
			break;
		}

		if(m_pcWait)
			m_pcWait->Draw(hdcMem,7,9);
	}

	BitBlt(dc.m_hDC,rcCl.left,rcCl.top,rcCl.right,rcCl.bottom,hdcMem,0,0,SRCCOPY);
	BitBlt(m_hDC,rcCl.left,rcCl.top,rcCl.right,rcCl.bottom,hdcMem,0,0,SRCCOPY);

	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);
	SelectObject(hdcMem,hBmpMemPrev);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
}

void CWndDataFOT::DrawStatic()
{
	//RECT rc;

	CDC* pDCStatic=CDC::FromHandle(m_hdcStatic);

	CBrush cbrBack(BACKGND);
	CBrush cbrLightGrey(LIGHTGREY);
	CBrush cbrYellow(NUMERIC);
	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));

	HBRUSH hbrprev=(HBRUSH)SelectObject(m_hdcStatic,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(m_hdcStatic, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(m_hdcStatic,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(m_hdcStatic,g_hf8AcuBold);
	int tc=SetTextColor(m_hdcStatic,0x0000000);

	Rectangle(m_hdcStatic, 0, 0, m_lX, m_lY);


	SelectObject(m_hdcStatic,(HPEN)penLine);
	SelectObject(m_hdcStatic,cbrLightGrey);
	
	//RoundRect(m_hdcStatic, 7, 102, 279, 301,20,20);
	RoundRect(m_hdcStatic, 7, 205, 279, 301,20,20);
	
	SelectObject(m_hdcStatic,cbrYellow);
	SelectObject(m_hdcStatic, (HPEN)GetStockObject(NULL_PEN));

	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		if(getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara()>1)
		{
			Rectangle(m_hdcStatic, 7, 92, 92, 145);
			Rectangle(m_hdcStatic, 197, 92, 282, 145);
		}
		else
		{
			Rectangle(m_hdcStatic, 7, 92, 92, 145);
		}
	}
	else
	{
		if(getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara()>1)
		{
			Rectangle(m_hdcStatic, 7, 92, 92, 145);
			Rectangle(m_hdcStatic, 197, 92, 282, 145);
		}
		else
		{
			Rectangle(m_hdcStatic, 7, 92, 92, 145);
		}
	}
	
	
	//rc.left = 20;  
	//rc.top = 108;//210;  
	//rc.right  = 80;  
	//rc.bottom = 240;
	//DrawText(m_hdcStatic,getModel()->GetLanguageString(IDS_MENU_FOT),-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);

	BitBlt(m_hDC, 0, 0, m_lX, m_lY, m_hdcStatic, 0, 0, SRCCOPY);

	SetTextColor(m_hdcStatic,tc);
	SetBkMode(m_hdcStatic,nBkMode);

	SelectObject(m_hdcStatic,hbrprev);
	SelectObject(m_hdcStatic,hpenprev);

	penLine.DeleteObject();

	SelectObject(m_hdcStatic,hPrevFont);
}

void CWndDataFOT::SetViewFocus(int iBtn)
{
	switch(iBtn)
	{
	case IDC_BTN_PARA_FOT_STEPS:
		{
			if(m_pcPara_FOT_Steps)
			{
				m_pcPara_FOT_Steps->SetFocus();
			}

		}
		break;
	case IDC_BTN_PARA_FOT_PMEANLOW:
		{
			if(m_pcPara_FOT_PmeanLow)
			{
				m_pcPara_FOT_PmeanLow->SetFocus();
			}

		}
		break;
	/*case IDC_BTN_PARA_FOTAMP:
		{
			if(m_pcPara_FOTamplitude)
			{
				m_pcPara_FOTamplitude->SetFocus();
			}
		}
		break;
	case IDC_BTN_PARA_FOTFREQ:
		{
			if(m_pcPara_FOTfreq)
			{
				m_pcPara_FOTfreq->SetFocus();
			}
		}
		break;*/
	case IDC_BTN_PARA_FOT_PMEANHIGH:
		{
			if(m_pcPara_FOT_PmeanHigh)
			{
				m_pcPara_FOT_PmeanHigh->SetFocus();
			}

		}
		break;
	case IDC_BTN_PARA_FOT_PEEPLOW:
		{
			if(m_pcPara_FOT_PEEPLow)
			{
				m_pcPara_FOT_PEEPLow->SetFocus();
			}

		}
		break;
	case IDC_BTN_PARA_FOT_PEEPHIGH:
		{
			if(m_pcPara_FOT_PEEPHigh)
			{
				m_pcPara_FOT_PEEPHigh->SetFocus();
			}

		}
		break;
	default:
		{
			//if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
			{
				if(m_pcPara_FOT_Steps)
				{
					m_pcPara_FOT_Steps->SetFocus();
					getModel()->getCONFIG()->setLastSelectedFOTViewBtns(m_pcPara_FOT_Steps->GetDlgCtrlID());
				}
			}
			/*else
			{
				if(m_pcPara_FOTamplitude)
				{
					m_pcPara_FOTamplitude->SetFocus();
					getModel()->getCONFIG()->setLastSelectedFOTViewBtns(m_pcPara_FOTamplitude->GetDlgCtrlID());
				}
			}*/
			

		}
		break;
	}

	if(GetParent())
		GetParent()->PostMessage(WM_SET_PARATIMER);

}

// **************************************************************************
// 
// **************************************************************************
bool CWndDataFOT::SetNextFocus()
{
	CWnd* pFocWnd = CWnd::GetFocus();
	if(pFocWnd==NULL)
		return false;
	int iID=pFocWnd->GetDlgCtrlID();
	CWnd* pWnd = GetNextDlgTabItem(pFocWnd,FALSE);
	if(pWnd==NULL)
		return false;

	int iID2=pWnd->GetDlgCtrlID();

	if(		pFocWnd->GetDlgCtrlID()>pWnd->GetDlgCtrlID()
		||	IDC_BTN_PARA_FOT_PEEPHIGH<pWnd->GetDlgCtrlID())
		return false;

	iID=pWnd->GetDlgCtrlID();
	pFocWnd->CheckDlgButton(pFocWnd->GetDlgCtrlID(),BST_UNCHECKED);

	pWnd->SetFocus();
	getModel()->getCONFIG()->setLastSelectedFOTViewBtns(iID);


	return true;
}
bool CWndDataFOT::SetPrevFocus()
{
	CWnd* pFocWnd = CWnd::GetFocus();
	if(pFocWnd==NULL)
		return false;
	//iID=pFocWnd->GetDlgCtrlID();
	CWnd* pWnd = GetNextDlgTabItem(pFocWnd,TRUE);
	if(pWnd==NULL)
		return false;


	if(pFocWnd->GetDlgCtrlID()<pWnd->GetDlgCtrlID())
		return false;

	int iID=pWnd->GetDlgCtrlID();
	pFocWnd->CheckDlgButton(pFocWnd->GetDlgCtrlID(),BST_UNCHECKED);

	pWnd->SetFocus();
	getModel()->getCONFIG()->setLastSelectedFOTViewBtns(iID);

	return true;
}
void CWndDataFOT::SetSTARTSTOPDepressed()
{
	//m_pcBtnStartStopSeq->Depress(true);
	
	if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_START_FOT);	
}
void CWndDataFOT::SetSTARTSTOPUnpressed()
{
	//m_pcBtnStartStopSeq->DrawDirectUp();
	
	/*if(AfxGetApp() != NULL)
		AfxGetApp()->GetMainWnd()->PostMessage(WM_STOP_FOT);*/
	
	PostMessage(WM_REDRAW_WND);
	//Draw(false);

}
//void CWndDataFOT::SetSTARTSTOPPreset()
//{
//	m_pcMenuStartStop->SetBtnState(CPresetMenuBtn::PRESET);
//}

void CWndDataFOT::showFOTMessage()
{
	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		//CString szStart=_T("This will start FOT - HFO.");
		CString szStart=getModel()->GetLanguageString(IDS_TXT_FOT_STARTHFO);
		if(true==getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
		{
			szStart+=_T(" ");
			szStart+=getModel()->GetLanguageString(IDS_TXT_FOT_VGOFF);
			//szStart+=_T(" VG will be turned off during FOT!");
		}

		if(		getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true 
			&&	getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
		{
			szStart+=_T(" ");
			szStart+=getModel()->GetLanguageString(IDS_TXT_FOT_LUNGRECOFF);
			//szStart+=_T(" Lung recruitment will be turned off!");
		}
		//szStart+=_T(" Continue?");
		szStart+=_T(" ");
		szStart+=getModel()->GetLanguageString(IDS_TXT_CONTINUE);
		szStart+=_T("?");

		CDlgMessageBox box(this,getModel()->GetLanguageString(IDS_TXT_FOTHFO),szStart,MB_YESNO,IDB_MSG_STOP);
		//CDlgMessageBox box(this,_T("FOT - HFO mode"),szStart,MB_YESNO,IDB_MSG_STOP);//todo FOTLANGUAGE
		m_pDlg = &box;
		int iRes=box.DoModal();
		m_pDlg = NULL;

		/*Invalidate(true);
		UpdateWindow();*/

		if(iRes==IDYES)
		{
			if(true==getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
			{
				getModel()->getALARMHANDLER()->setVgarantAutoTurnedOff_FOT();
				getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_PARA_AUTODISABLED);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PARABN_VGARANT);
				Sleep(100);
			}

			if(		getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true 
				&&	getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
			{
				getModel()->getDATAHANDLER()->PARADATA()->SetFreqRecPara(0,true,true);
				Sleep(100);
			}

			/*if(getModel()->getDATAHANDLER()->PARADATA()->GetBackupPara()==0)
			{
				getModel()->getDATAHANDLER()->SetBackupParadata(0,true);
				Sleep(100);
			}*/

			SetSTARTSTOPDepressed();
		}
		else
		{
			SetSTARTSTOPUnpressed();
		}
	}
	else
	{
		
		//CDlgMessageBox box(this,_T("FOT - conventional mode"),_T("This will start FOT - CONVENTIONAL. The ventilation mode will be changed to CPAP while FOT. Continue?"),MB_YESNO,IDB_MSG_STOP);
		CDlgMessageBox box(this,getModel()->GetLanguageString(IDS_TXT_FOTCONV),getModel()->GetLanguageString(IDS_TXT_FOT_STARTCONV),MB_YESNO,IDB_MSG_STOP);
		m_pDlg = &box;
		int iRes=box.DoModal();
		m_pDlg = NULL;

		/*Invalidate(true);
		UpdateWindow();*/

		if(iRes==IDYES)
		{
			SetSTARTSTOPDepressed();
		}
		else
		{
			SetSTARTSTOPUnpressed();
		}
	}
}


// **************************************************************************
// 
// **************************************************************************
LRESULT CWndDataFOT::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_FOT_STEP_CHANGED:
		{
			//Draw(false);
			drawMbarStep();
			drawPIPmax();
		}
		break;
	case WM_FOT_PEEPEND_CHANGED:
		{
			if(m_pcPara_FOT_PEEPLow)
				m_pcPara_FOT_PEEPLow->SetArrowDown(false);
			if(m_pcPara_FOT_Steps)
				m_pcPara_FOT_Steps->SetArrowDown(false);

			//Draw(false);
			drawMbarStep();
			drawPIPmax();
		}
		break;
	case WM_FOT_PEEPSTART_CHANGED:
		{
			if(m_pcPara_FOT_PEEPHigh)
				m_pcPara_FOT_PEEPHigh->SetArrowUp(false);
			if(m_pcPara_FOT_Steps)
				m_pcPara_FOT_Steps->SetArrowDown(false);

			//Draw(false);
			drawMbarStep();
			drawPIPmax();
			return 1;
		}
		break;
	case WM_FOT_PEEPEND_LIMITED:
		{
			if(m_pcPara_FOT_PEEPLow)
				m_pcPara_FOT_PEEPLow->SetArrowDown();
			if(m_pcPara_FOT_Steps)
				m_pcPara_FOT_Steps->SetArrowDown();

			return 1;
		}
		break;
	case WM_FOT_PEEPSTART_LIMITED:
		{
			if(m_pcPara_FOT_PEEPHigh)
				m_pcPara_FOT_PEEPHigh->SetArrowUp();
			if(m_pcPara_FOT_Steps)
				m_pcPara_FOT_Steps->SetArrowDown();

			return 1;
		}
		break;
	case WM_PMEANEND_SETKEY:
		{
			//m_bPMEANEND_SetKey=true;
			if(wParam==TRUE)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);

			return 1;
		}
		break;
	case WM_PMEANSTART_SETKEY:
		{
			//m_bPMEANEND_SetKey=true;
			if(wParam==TRUE)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);

			return 1;
		}
		break;
	case WM_PEEPEND_SETKEY:
		{
			//m_bPMEANEND_SetKey=true;
			if(wParam==TRUE)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);

			return 1;
		}
		break;
	case WM_PEEPSTART_SETKEY:
		{
			//m_bPMEANEND_SetKey=true;
			if(wParam==TRUE)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_CONFIRMRANGE);

			return 1;
		}
		break;
		/*case WM_PMEANEND_CLEARKEY:
		{
			m_bPMEANEND_SetKey=false;
			return 1;
		}
		break;*/
	case WM_FOT_PMEANEND_CHANGED:
		{
			if(m_pcPara_FOT_PmeanLow)
				m_pcPara_FOT_PmeanLow->SetArrowDown(false);
			if(m_pcPara_FOT_Steps)
				m_pcPara_FOT_Steps->SetArrowDown(false);
			
			//Draw(false);
			drawMbarStep();
			drawPIPmax();

			return 1;
		}
		break;
	case WM_FOT_PMEANSTART_CHANGED:
		{
			if(m_pcPara_FOT_PmeanHigh)
				m_pcPara_FOT_PmeanHigh->SetArrowUp(false);
			if(m_pcPara_FOT_Steps)
				m_pcPara_FOT_Steps->SetArrowDown(false);
			
			//Draw(false);
			drawMbarStep();
			drawPIPmax();

			return 1;
		}
		break;
	case WM_FOT_PMEANEND_LIMITED:
		{
			if(m_pcPara_FOT_PmeanLow)
				m_pcPara_FOT_PmeanLow->SetArrowDown();
			if(m_pcPara_FOT_Steps)
				m_pcPara_FOT_Steps->SetArrowDown();
	
			return 1;
		}
		break;
	case WM_FOT_PMEANSTART_LIMITED:
		{
			if(m_pcPara_FOT_PmeanHigh)
				m_pcPara_FOT_PmeanHigh->SetArrowUp();
			if(m_pcPara_FOT_Steps)
				m_pcPara_FOT_Steps->SetArrowDown();

			return 1;
		}
		break;
	case WM_CHECK_FOT_ALARMS:
		{
			
			if(getModel()->getALARMHANDLER()->isFOTrelevantAlarmActive()==true)
			{
				if(getModel()->getFOTThread())
				{
					if(getModel()->getFOTThread()->getFOTstate()!=FOT_OFF)
					{
						if(AfxGetApp() != NULL)
							AfxGetApp()->GetMainWnd()->PostMessage(WM_STOP_FOT);
					}
				}
			}
			drawFOTmenubar();
		}
		break;
	case WM_REDRAW_FOT_STATE:
		{
			Draw(false);
			drawFOTmenubar();
		}
		break;
	case WM_REDRAW_WND:
		{
			Draw(false);
			drawFOTmenubar();
		}
		break;
	case WM_FOTSTEP_CHANGED:
		{
			if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
			{
				if(getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara()>1)
				{
					if(m_pcPara_FOT_PmeanLow)
					{
						m_pcPara_FOT_PmeanLow->SetNameText(_T("P"));
						m_pcPara_FOT_PmeanLow->SetNameNoteText(_T("mean low"),true);
					}
					if(m_pcPara_FOT_PmeanHigh)
						m_pcPara_FOT_PmeanHigh->ShowWindow(SW_SHOW);
				}
				else
				{
					if(m_pcPara_FOT_PmeanLow)
					{
						m_pcPara_FOT_PmeanLow->SetNameText(_T("P"));
						m_pcPara_FOT_PmeanLow->SetNameNoteText(_T("mean"),true);
					}
					if(m_pcPara_FOT_PmeanHigh)
						m_pcPara_FOT_PmeanHigh->ShowWindow(SW_HIDE);
				}
			}
			else
			{
				if(getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara()>1)
				{
					if(m_pcPara_FOT_PEEPLow)
					{
						m_pcPara_FOT_PEEPLow->SetNameText(_T("PEEP"));
						m_pcPara_FOT_PEEPLow->SetNameNoteText(_T("low"),true);
					}
					if(m_pcPara_FOT_PEEPHigh)
						m_pcPara_FOT_PEEPHigh->ShowWindow(SW_SHOW);
				}
				else
				{
					if(m_pcPara_FOT_PEEPLow)
					{
						m_pcPara_FOT_PEEPLow->SetNameText(_T("PEEP"));
						m_pcPara_FOT_PEEPLow->SetNameNoteText(_T(""),true);
					}
					if(m_pcPara_FOT_PEEPHigh)
						m_pcPara_FOT_PEEPHigh->ShowWindow(SW_HIDE);
				}
			}
			Draw(true);
		}
		break;
	//case WM_MENUBUTTONDOWN:
	//	{
	//		switch(wParam)
	//		{
	//		//case IDC_BTN_STARTSTOP_FOT:
	//		//	{
	//		//		if(false==getModel()->getDATAHANDLER()->getFOTrunning())
	//		//		{
	//		//			showFOTMessage();
	//		//			//SetOneButtonPreset_Freshgas(wParam);
	//		//		}
	//		//		else //if(lParam==CPresetMenuBtn::DOWN)
	//		//		{
	//		//			SetSTARTSTOPUnpressed();
	//		//		}
	//		//		return 1;
	//		//	}
	//		//	break;
	//		//case IDC_BTN_CONTINUE_FOT:
	//		//	{
	//		//		if(AfxGetApp())
	//		//			AfxGetApp()->GetMainWnd()->PostMessage(WM_NEXT_FOT_STEP);
	//		//		//m_pcContinueFOTseq->DrawDirectUp();

	//		//		return 1;
	//		//	}
	//		//	break;
	//		default:
	//			{
	//			}
	//			break;
	//		}
	//	}
	//	break;
	//case WM_MENUBUTTONUP:
	//	{
	//		switch(wParam)
	//		{
	//		/*case IDC_BTN_STARTSTOP_FOT:xxx
	//			{
	//				if(getModel()->getFOTThread())
	//				{
	//					if(getModel()->getFOTThread()->getFOTstate()==FOT_OFF)
	//					{
	//						showFOTMessage();
	//					}
	//					else 
	//					{
	//						SetSTARTSTOPUnpressed();
	//					}
	//				}
	//				else
	//				{
	//					SetSTARTSTOPUnpressed();
	//				}
	//				return 1;
	//			}
	//			break;*/
	//		case IDC_BTN_CONTINUE_FOT:xxx
	//			{
	//				if(AfxGetApp())
	//					AfxGetApp()->GetMainWnd()->PostMessage(WM_NEXT_FOT_STEP);

	//				return 1;
	//			}
	//			break;
	//		default:
	//			{
	//			}
	//			break;
	//		}

	//	}
	//	break;
	}
	//#endif

	return CWnd::WindowProc(message, wParam, lParam);
}

//BOOL CWndDataFOT::PreTranslateMessage(MSG* pMsg) 
//{
//	switch(pMsg->message)
//	{
//	/*case WM_LBUTTONDOWN:
//		{
//			if(AfxGetApp())
//				AfxGetApp()->GetMainWnd()->PostMessage(WM_CURRENT_VIEWID,IDC_VIEW_GRAPH,0);
//		}
//		break;*/
//	case WM_KEYDOWN:
//		{
//			if(pMsg->wParam==VK_SPACE)
//			{
//				PostMessage(WM_SET_PARATIMER);
//			}
//			else if(pMsg->wParam==VK_UP)
//			{
//				CWnd* pFocWnd=CWnd::GetFocus();
//				if(pFocWnd!=0)
//				{
//					int iID=pFocWnd->GetDlgCtrlID();
//
//					
//				}
//
//				
//				//return 1;
//			}
//			else if(pMsg->wParam==VK_DOWN)
//			{
//				CWnd* pFocWnd=CWnd::GetFocus();
//				if(pFocWnd!=0)
//				{
//					int iID=pFocWnd->GetDlgCtrlID();
//
//					
//				}
//
//				//return 1;
//			}
//		}
//		break;
//	default:
//		break;
//	}
//	return CWnd::PreTranslateMessage(pMsg);
//}

void CWndDataFOT::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==FOT_HORGLASS_TIMER)
	{
		if(m_iWaitCount>0)
		{
			m_iWaitCount++;
		}
		drawHourglass();
	}

	CWnd::OnTimer(nIDEvent);
}

void CWndDataFOT::setFOTtime(BYTE iCountFOTimer)
{
	m_iCountFOTimer=iCountFOTimer;
	Draw(false);
}

void CWndDataFOT::updateFOTPmeanPara()
{
	fVALUE fvPMEANhigh;
	fvPMEANhigh.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_PMEANSTARTPara()+getModel()->getCONFIG()->getDiffPmeanFOT();
	fvPMEANhigh.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxPara();
	fvPMEANhigh.iValue=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_PMEANENDPara();
	if(fvPMEANhigh.iValue>fvPMEANhigh.iUpperLimit)
	{
		fvPMEANhigh.iValue=fvPMEANhigh.iUpperLimit;
		getModel()->getDATAHANDLER()->PARADATA()->setFOThfo_PMEANENDPara(fvPMEANhigh.iValue,true,true);
	}
	else if(fvPMEANhigh.iValue<fvPMEANhigh.iLowerLimit)
	{
		fvPMEANhigh.iValue=fvPMEANhigh.iLowerLimit;
		getModel()->getDATAHANDLER()->PARADATA()->setFOThfo_PMEANENDPara(fvPMEANhigh.iValue,true,true);
	}
	

	fVALUE fvPMEANlow;
	fvPMEANlow.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMinPara();
	//fvPMEANlow.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_PMEANENDPara()-getModel()->getCONFIG()->getDiffPmeanFOT();
	if(getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara()>1)
	{
		fvPMEANlow.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_PMEANENDPara()-getModel()->getCONFIG()->getDiffPmeanFOT();
	}
	else
	{
		fvPMEANlow.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxPara();
	}
	fvPMEANlow.iValue=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_PMEANSTARTPara();
	if(fvPMEANlow.iValue>fvPMEANlow.iUpperLimit)
	{
		fvPMEANlow.iValue=fvPMEANlow.iUpperLimit;
		getModel()->getDATAHANDLER()->PARADATA()->setFOThfo_PMEANSTARTPara(fvPMEANlow.iValue,true,true);
	}
	else if(fvPMEANlow.iValue<fvPMEANlow.iLowerLimit)
	{
		fvPMEANlow.iValue=fvPMEANlow.iLowerLimit;
		getModel()->getDATAHANDLER()->PARADATA()->setFOThfo_PMEANSTARTPara(fvPMEANlow.iValue,true,true);
	}
	

	if(getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_STEPSPara()>1)
	{
		if(m_pcPara_FOT_PmeanLow)
		{
			m_pcPara_FOT_PmeanLow->SetNameText(_T("P"));
			m_pcPara_FOT_PmeanLow->SetNameNoteText(_T("mean low"));
		}
		if(m_pcPara_FOT_PmeanHigh)
			m_pcPara_FOT_PmeanHigh->ShowWindow(SW_SHOW);
		if(m_pcPara_FOT_PmeanHigh)
			m_pcPara_FOT_PmeanHigh->SetValue(fvPMEANhigh, true);
	}
	else
	{
		if(m_pcPara_FOT_PmeanLow)
		{
			m_pcPara_FOT_PmeanLow->SetNameText(_T("P"));
			m_pcPara_FOT_PmeanLow->SetNameNoteText(_T("mean"));
		}
		if(m_pcPara_FOT_PmeanHigh)
			m_pcPara_FOT_PmeanHigh->ShowWindow(SW_HIDE);
		if(m_pcPara_FOT_PmeanHigh)
			m_pcPara_FOT_PmeanHigh->SetValue(fvPMEANhigh, false);
	}

	if(m_pcPara_FOT_PmeanLow)
		m_pcPara_FOT_PmeanLow->SetValue(fvPMEANlow, true);
	
}

void CWndDataFOT::updateFOTPEEPPara()
{
	fVALUE fvPEEPhigh;
	fvPEEPhigh.iLowerLimit=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_PEEPSTARTPara()+getModel()->getCONFIG()->getDiffPEEPFOT();
	fvPEEPhigh.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMaxPara_IPPV();
	fvPEEPhigh.iValue=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_PEEPENDPara();
	if(fvPEEPhigh.iValue>fvPEEPhigh.iUpperLimit)
	{
		fvPEEPhigh.iValue=fvPEEPhigh.iUpperLimit;
		getModel()->getDATAHANDLER()->PARADATA()->setFOTconv_PEEPENDPara(fvPEEPhigh.iValue,true,true);
	}
	else if(fvPEEPhigh.iValue<fvPEEPhigh.iLowerLimit)
	{
		fvPEEPhigh.iValue=fvPEEPhigh.iLowerLimit;
		getModel()->getDATAHANDLER()->PARADATA()->setFOTconv_PEEPENDPara(fvPEEPhigh.iValue,true,true);
	}


	fVALUE fvPEEPlow;

	WORD iValAMP=(getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_AMPLITUDEPara()+1)/2;
	if(iValAMP%10!=0)
		iValAMP+=5;
	fvPEEPlow.iLowerLimit=iValAMP;
	if(getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara()>1)
	{
		fvPEEPlow.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_PEEPENDPara()-getModel()->getCONFIG()->getDiffPEEPFOT();
	}
	else
	{
		fvPEEPlow.iUpperLimit=getModel()->getDATAHANDLER()->PARADATA()->GetPEEPMaxPara_IPPV();
	}
	
	fvPEEPlow.iValue=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_PEEPSTARTPara();
	if(fvPEEPlow.iValue>fvPEEPlow.iUpperLimit)
	{
		fvPEEPlow.iValue=fvPEEPlow.iUpperLimit;
		getModel()->getDATAHANDLER()->PARADATA()->setFOTconv_PEEPSTARTPara(fvPEEPlow.iValue,true,true);
	}
	else if(fvPEEPlow.iValue<fvPEEPlow.iLowerLimit)
	{
		fvPEEPlow.iValue=fvPEEPlow.iLowerLimit;
		getModel()->getDATAHANDLER()->PARADATA()->setFOTconv_PEEPSTARTPara(fvPEEPlow.iValue,true,true);
	}


	if(getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_STEPSPara()>1)
	{
		if(m_pcPara_FOT_PEEPLow)
		{
			m_pcPara_FOT_PEEPLow->SetNameText(_T("PEEP"));
			m_pcPara_FOT_PEEPLow->SetNameNoteText(_T("low"));
		}
		if(m_pcPara_FOT_PEEPHigh)
			m_pcPara_FOT_PEEPHigh->ShowWindow(SW_SHOW);
		if(m_pcPara_FOT_PEEPHigh)
			m_pcPara_FOT_PEEPHigh->SetValue(fvPEEPhigh, true);
	}
	else
	{
		if(m_pcPara_FOT_PEEPLow)
		{
			m_pcPara_FOT_PEEPLow->SetNameText(_T("PEEP"));
			m_pcPara_FOT_PEEPLow->SetNameNoteText(_T(""));
		}
		if(m_pcPara_FOT_PEEPHigh)
			m_pcPara_FOT_PEEPHigh->ShowWindow(SW_HIDE);
		if(m_pcPara_FOT_PEEPHigh)
			m_pcPara_FOT_PEEPHigh->SetValue(fvPEEPhigh, false);
	}

	if(m_pcPara_FOT_PEEPLow)
		m_pcPara_FOT_PEEPLow->SetValue(fvPEEPlow, true);

}

void CWndDataFOT::setFOTrunning(bool state)
{
	if(m_pcBtnStartStopSeq)
		m_pcBtnStartStopSeq->Depress(state);

	bool bFOTrunning=false;
	
	if(getModel()->getFOTThread())
	{
		if(getModel()->getFOTThread()->getFOTstate()!=FOT_OFF)
			bFOTrunning=true;
	}
	


	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		if(true==bFOTrunning)
		{
			if(m_pcPara_FOT_Steps)
				m_pcPara_FOT_Steps->EnableWindow(FALSE);
			if(m_pcPara_FOT_PmeanLow)
				m_pcPara_FOT_PmeanLow->EnableWindow(FALSE);
			/*if(m_pcPara_FOTamplitude)
				m_pcPara_FOTamplitude->EnableWindow(FALSE);
			if(m_pcPara_FOTfreq)
				m_pcPara_FOTfreq->EnableWindow(FALSE);*/
			if(m_pcPara_FOT_PmeanHigh)
				m_pcPara_FOT_PmeanHigh->EnableWindow(FALSE);
		}
		else
		{
			if(m_pcPara_FOT_Steps)
				m_pcPara_FOT_Steps->EnableWindow(TRUE);
			if(m_pcPara_FOT_PmeanLow)
				m_pcPara_FOT_PmeanLow->EnableWindow(TRUE);
			/*if(m_pcPara_FOTamplitude)
				m_pcPara_FOTamplitude->EnableWindow(TRUE);
			if(m_pcPara_FOTfreq)
				m_pcPara_FOTfreq->EnableWindow(TRUE);*/
			if(m_pcPara_FOT_PmeanHigh)
				m_pcPara_FOT_PmeanHigh->EnableWindow(TRUE);
		}
	}
	else
	{
		if(true==bFOTrunning)
		{
			if(m_pcPara_FOT_Steps)
				m_pcPara_FOT_Steps->EnableWindow(FALSE);
			if(m_pcPara_FOT_PEEPLow)
				m_pcPara_FOT_PEEPLow->EnableWindow(FALSE);
			/*if(m_pcPara_FOTamplitude)
				m_pcPara_FOTamplitude->EnableWindow(FALSE);
			if(m_pcPara_FOTfreq)
				m_pcPara_FOTfreq->EnableWindow(FALSE);*/
			if(m_pcPara_FOT_PEEPHigh)
				m_pcPara_FOT_PEEPHigh->EnableWindow(FALSE);
		}
		else
		{
			if(m_pcPara_FOT_Steps)
				m_pcPara_FOT_Steps->EnableWindow(TRUE);
			if(m_pcPara_FOT_PEEPLow)
				m_pcPara_FOT_PEEPLow->EnableWindow(TRUE);
			/*if(m_pcPara_FOTamplitude)
				m_pcPara_FOTamplitude->EnableWindow(TRUE);
			if(m_pcPara_FOTfreq)
				m_pcPara_FOTfreq->EnableWindow(TRUE);*/
			if(m_pcPara_FOT_PEEPHigh)
				m_pcPara_FOT_PEEPHigh->EnableWindow(TRUE);
		}
	}
	Draw(false);
}

void CWndDataFOT::startHourglass()
{
	m_bHourglassRun=true;
	m_iWaitCount=1;
	SetTimer(FOT_HORGLASS_TIMER, 200, NULL);
}

void CWndDataFOT::stopHourglass()
{
	m_bHourglassRun=false;
	m_iWaitCount=0;
	KillTimer(FOT_HORGLASS_TIMER);
}

void CWndDataFOT::OnBnClickedStartStop()
{
	if(getModel()->getFOTThread())
	{
		if(getModel()->getFOTThread()->getFOTstate()==FOT_OFF)
		{
			showFOTMessage();
		}
		else 
		{
			CDlgMessageBox box(this,_T("FOT"),getModel()->GetLanguageString(IDS_TXT_FOT_STOPFOT),MB_YESNO,IDB_MSG_STOP);
			m_pDlg = &box;
			int iRes=box.DoModal();
			m_pDlg = NULL;

			if(iRes==IDYES)
			{
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_STOP_FOT);
			}
			else
			{
				if(GetParent())
					GetParent()->PostMessage(WM_GRAPH_REDRAW);
			}
		}
	}
	else
	{
		//SetSTARTSTOPUnpressed();
		//m_pcBtnStartStopSeq->DrawDirectUp();

		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_STOP_FOT);

		//Draw(false);
	}
	if(GetParent())
		GetParent()->PostMessage(WM_GRAPH_REDRAW);


	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();
}
void CWndDataFOT::OnBnClickedDecrease()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_DECREASE_FOT_STEP);
}
void CWndDataFOT::OnBnClickedRepeat()
{
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_REPEAT_FOT_STEP);
}
void CWndDataFOT::OnBnClickedContinue()
{
	SequenceStatesFOT curSeq=getModel()->getFOTThread()->getFOTstate();

	if(FOT_VENTDELAY==curSeq)
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_START_FOT_STEP);
	}
	else
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_NEXT_FOT_STEP);
	}
	
}