// SubViewDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "SubViewDisplay.h"
#include "DlgMessageBox.h"
#include <commctrl.h>

extern "C" BOOL WINAPI TouchCalibrate( void );

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

//global font objects
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf8AcuNorm;
extern HFONT g_hf8AcuBold;
extern HFONT g_hf10AcuBold;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf13AcuBold;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf15AcuMed;
extern HFONT g_hf17AcuBold;
extern HFONT g_hf19AcuMed;
extern HFONT g_hf21AcuBold;
extern HFONT g_hf23AcuBold;
extern HFONT g_hf25AcuMed;
extern HFONT g_hf27AcuBold;
extern HFONT g_hf31AcuBold;
extern HFONT g_hf33AcuBold;
extern HFONT g_hf43AcuBold;
extern HFONT g_hf53AcuBold;

/**********************************************************************************************//**
 * CSubViewDisplay
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	parameter1	The first parameter.
 * \param	parameter2	The second parameter.
 **************************************************************************************************/

IMPLEMENT_DYNAMIC(CSubViewDisplay, CWnd)

/**********************************************************************************************//**
 * Initializes a new instance of the CSubViewDisplay class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CSubViewDisplay::CSubViewDisplay()
{
	m_pModel=NULL;

	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;

	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;

	m_pcNotFilledGraph=NULL;
	m_pcFilledGraph=NULL;

	m_pcNoautoscale=NULL;

	m_pcScreenlocktime=NULL;
	m_pcScreenlocktime_Up=NULL;
	m_pcScreenlocktime_Dw=NULL;
	m_pcScreenlocktime_Fc=NULL;
	m_pcScreenlocktime_Dis=NULL;

	m_pcNight=NULL;
	m_pcBrightness1=NULL;
	m_pcBrightness2=NULL;
	m_pcBrightness3=NULL;
	m_pcBrightness4=NULL;
	m_pcBrightness5=NULL;

	m_pcIncrContrast_Up=NULL;
	m_pcIncrContrast_Dw=NULL;
	m_pcDecrContrast_Up=NULL;
	m_pcDecrContrast_Dw=NULL;

	m_pcIncrMaxContrast_Up=NULL;
	m_pcIncrMaxContrast_Dw=NULL;
	m_pcDecrMaxContrast_Up=NULL;
	m_pcDecrMaxContrast_Dw=NULL;

	m_pcRegionTopLeCorner=NULL;
	m_pcRegionTopRiCorner=NULL;
	m_pcRegionBotLeCorner=NULL;
	m_pcRegionBotRiCorner=NULL;
	
	m_pcIncrease=NULL;
	m_pcIncreaseMax=NULL;
	m_pcDecrease=NULL;
	m_pcDecreaseMax=NULL;

	/*m_pcCal_Up=NULL;
	m_pcCal_Dw=NULL;*/
	//m_pcCal=NULL;
	m_pcScreenlock=NULL;
	m_pcScreenlock_Up=NULL;
	m_pcScreenlock_Dw=NULL;

	//m_pDlg = NULL;
	

	m_sliderAutolock=NULL;
	m_sliderFilledGraph=NULL;
	m_sliderAutoscaleGraph=NULL;
	m_sliderFilledTrend=NULL;
	m_sliderAutoscaleTrend=NULL;

	m_iContrastParm=0;
	m_bAutoScreenLock=false;
	m_eAutoSreenlock=ASL_60;
	m_bGraphFilled=false;
	m_bGraphAutoscale=false;
	m_bTrendFilled=false;
	m_bTrendAutoscale=false;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CSubViewDisplay class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CSubViewDisplay::~CSubViewDisplay()
{
	delete m_sliderAutolock;
	m_sliderAutolock=NULL;

	delete m_sliderFilledGraph;
	m_sliderFilledGraph=NULL;

	delete m_sliderAutoscaleGraph;
	m_sliderAutoscaleGraph=NULL;

	delete m_sliderFilledTrend;
	m_sliderFilledTrend=NULL;

	delete m_sliderAutoscaleTrend;
	m_sliderAutoscaleTrend=NULL;

	delete m_pcNotFilledGraph;
	delete m_pcFilledGraph;
	m_pcNotFilledGraph=NULL;
	m_pcFilledGraph=NULL;

	delete m_pcNoautoscale;
	m_pcNoautoscale=NULL;

	delete m_pcScreenlocktime;
	m_pcScreenlocktime=NULL;

	delete m_pcScreenlocktime_Up;
	delete m_pcScreenlocktime_Dw;
	delete m_pcScreenlocktime_Fc;
	delete m_pcScreenlocktime_Dis;
	m_pcScreenlocktime_Up=NULL;
	m_pcScreenlocktime_Dw=NULL;
	m_pcScreenlocktime_Fc=NULL;
	m_pcScreenlocktime_Dis=NULL;

	delete m_pcNight;
	delete m_pcBrightness1;
	delete m_pcBrightness2;
	delete m_pcBrightness3;
	delete m_pcBrightness4;
	delete m_pcBrightness5;
	m_pcNight=NULL;
	m_pcBrightness1=NULL;
	m_pcBrightness2=NULL;
	m_pcBrightness3=NULL;
	m_pcBrightness4=NULL;
	m_pcBrightness5=NULL;

	delete m_pcIncrContrast_Up;
	delete m_pcIncrContrast_Dw;
	delete m_pcDecrContrast_Up;
	delete m_pcDecrContrast_Dw;

	delete m_pcIncrMaxContrast_Up;
	delete m_pcIncrMaxContrast_Dw;
	delete m_pcDecrMaxContrast_Up;
	delete m_pcDecrMaxContrast_Dw;

	delete m_pcRegionTopLeCorner;
	m_pcRegionTopLeCorner=NULL;
	delete m_pcRegionTopRiCorner;
	m_pcRegionTopRiCorner=NULL;
	delete m_pcRegionBotLeCorner;
	m_pcRegionBotLeCorner=NULL;
	delete m_pcRegionBotRiCorner;
	m_pcRegionBotRiCorner=NULL;

	delete m_pcIncrease;
	delete m_pcIncreaseMax;
	delete m_pcDecrease;
	delete m_pcDecreaseMax;


	m_pcIncrContrast_Up=NULL;
	m_pcIncrContrast_Dw=NULL;
	m_pcDecrContrast_Up=NULL;
	m_pcDecrContrast_Dw=NULL;

	m_pcIncrMaxContrast_Up=NULL;
	m_pcIncrMaxContrast_Dw=NULL;
	m_pcDecrMaxContrast_Up=NULL;
	m_pcDecrMaxContrast_Dw=NULL;

	m_pcIncrease=NULL;
	m_pcIncreaseMax=NULL;
	m_pcDecrease=NULL;
	m_pcDecreaseMax=NULL;



	delete m_pcScreenlock;
	m_pcScreenlock=NULL;

	delete m_pcScreenlock_Up;
	delete m_pcScreenlock_Dw;
	m_pcScreenlock_Up=NULL;
	m_pcScreenlock_Dw=NULL;
}


BEGIN_MESSAGE_MAP(CSubViewDisplay, CWnd)
	//ON_WM_TIMER()
	ON_WM_DESTROY()
	//ON_BN_CLICKED(IDC_BTN_SERVICE_DISPLCAL, &CSubViewDisplay::OnBnClickedCalibrate)
	ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnMyMessage)
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * CSubViewDisplay message handlers
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CSubViewDisplay::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * Creates a window
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

bool CSubViewDisplay::CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext)
{
	if (!Create(pParentWnd, rc, nID, pContext))
	{
		return false;
	}

	Initialize();

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

BOOL CSubViewDisplay::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext*) 
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

		//CBrush cbrBack(RGB(210,210,255));
		CBrush cbrBack;
		//cbrFill.CreateSolidBrush(RGB(181,178,181));
		cbrBack.CreateSolidBrush(BACKGND);
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));	

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		/*RECT rc;
		rc.left = 180;  
		rc.top = 20;  
		rc.right  = 620;  
		rc.bottom = 450;

		SelectObject(m_hDC,cbrFill);*/


		SelectObject(m_hDC,hbrprev);
		SelectObject(m_hDC,hpenprev);

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
 **************************************************************************************************/

void CSubViewDisplay::Initialize()
{
	CClientDC dc(this);

	m_pcNoautoscale= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_PIC_NOAUTOSCALE);

	m_pcScreenlocktime_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SCREENLOCKTIME_UP);
	m_pcScreenlocktime_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SCREENLOCKTIME_DW);
	m_pcScreenlocktime_Fc= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SCREENLOCKTIME_FC);
	m_pcScreenlocktime_Dis= new CBmp(theApp.m_hInstance,dc.m_hDC, IDB_BTN_SCREENLOCKTIME_DIS);
	
	
	m_pcNight= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_PIC_NIGHT);
	m_pcBrightness1= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_PIC_BRIGHTNESS1);
	m_pcBrightness2= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_PIC_BRIGHTNESS2);
	m_pcBrightness3= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_PIC_BRIGHTNESS3);
	m_pcBrightness4= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_PIC_BRIGHTNESS4);
	m_pcBrightness5= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_PIC_BRIGHTNESS5);

	m_pcNotFilledGraph= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_PIC_NOTFIL_GRAPH);
	m_pcFilledGraph= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_PIC_FILLED_GRAPH);

	m_pcIncrContrast_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_CONTR_UP);
	m_pcIncrContrast_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_CONTR_DW);
	m_pcDecrContrast_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_CONTR_UP);
	m_pcDecrContrast_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_CONTR_DW);

	m_pcIncrMaxContrast_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_INCRMAXCONTR_UP);
	m_pcIncrMaxContrast_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_INCRMAXCONTR_DW);
	m_pcDecrMaxContrast_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_DECRMAXCONTR_UP);
	m_pcDecrMaxContrast_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_DECRMAXCONTR_DW);

	m_pcRegionTopLeCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_LE_TOP_CORNER);
	m_pcRegionTopRiCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_RI_TOP_CORNER);
	m_pcRegionBotLeCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_LE_BOT_CORNER);
	m_pcRegionBotRiCorner		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_SETUP_RI_BOT_CORNER);

	m_pcScreenlock_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SCREENLOCK_UP);
	m_pcScreenlock_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_SCREENLOCK_DW);

	/*m_pcCal_Up= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_CALDISPLAY_UP);
	m_pcCal_Dw= new CBmp(theApp.m_hInstance,dc.m_hDC,	IDB_BTN_CALDISPLAY_DW);*/

	m_eAutoSreenlock=getModel()->getCONFIG()->GetAutoScreenlockTime();
	m_bAutoScreenLock=getModel()->getCONFIG()->IsAutoScreenlockActive();

	CStringW szScreenlocktime=_T("");

	if(m_bAutoScreenLock==false)
		szScreenlocktime=getModel()->GetLanguageString(IDS_TXT_OFF);
	else
	{
		switch(m_eAutoSreenlock)
		{
		case ASL_20:
			{
				szScreenlocktime=_T("20 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS);
			}
			break;
		case ASL_30:
			{
				szScreenlocktime=_T("30 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS);
			}
			break;
		case ASL_40:
			{
				szScreenlocktime=_T("40 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS);
			}
			break;
		case ASL_50:
			{
				szScreenlocktime=_T("50 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS);
			}
			break;
		case ASL_60:
			{
				szScreenlocktime=_T("60 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS);
			}
			break;
		}
	}

	m_bGraphFilled=getModel()->getCONFIG()->GraphIsFilled();
	m_bGraphAutoscale=getModel()->getCONFIG()->GraphIsAutoScale();
	m_bTrendFilled=getModel()->getCONFIG()->TrendIsFilled();
	m_bTrendAutoscale=getModel()->getCONFIG()->TrendIsAutoScale();

	BTN btn;

	btn.wID					= IDC_BTN_CONTRAST_DECRMAX;	
	btn.poPosition.x		= 308;//120;
	btn.poPosition.y		= 25;
	btn.pcBmpUp				= m_pcDecrMaxContrast_Up;
	btn.pcBmpDown			= m_pcDecrMaxContrast_Dw;
	btn.pcBmpFocus			= m_pcDecrMaxContrast_Up;
	btn.pcBmpDisabled		= m_pcDecrMaxContrast_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcDecreaseMax=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcDecreaseMax->Create(this,g_hf33AcuBold,0);
	m_pcDecreaseMax->SetText(_T(""));
	m_pcDecreaseMax->ShowWindow(SW_SHOW);


	btn.wID					= IDC_BTN_CONTRAST_DECR;	
	btn.poPosition.x		= 392;//205;
	btn.poPosition.y		= 25;
	btn.pcBmpUp				= m_pcDecrContrast_Up;
	btn.pcBmpDown			= m_pcDecrContrast_Dw;
	btn.pcBmpFocus			= m_pcDecrContrast_Up;
	btn.pcBmpDisabled		= m_pcDecrContrast_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcDecrease=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcDecrease->Create(this,g_hf33AcuBold,0);
	m_pcDecrease->SetText(_T("-"));
	m_pcDecrease->ShowWindow(SW_SHOW);


	btn.wID					= IDC_BTN_CONTRAST_INCR;	
	btn.poPosition.x		= 455;
	btn.poPosition.y		= 25;
	btn.pcBmpUp				= m_pcIncrContrast_Up;
	btn.pcBmpDown			= m_pcIncrContrast_Dw;
	btn.pcBmpFocus			= m_pcIncrContrast_Up;
	btn.pcBmpDisabled		= m_pcIncrContrast_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcIncrease=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcIncrease->Create(this,g_hf33AcuBold,0);
	m_pcIncrease->SetText(_T("+"));
	m_pcIncrease->ShowWindow(SW_SHOW);


	btn.wID					= IDC_BTN_CONTRAST_INCRMAX;	
	btn.poPosition.x		= 517;
	btn.poPosition.y		= 25;
	btn.pcBmpUp				= m_pcIncrMaxContrast_Up;
	btn.pcBmpDown			= m_pcIncrMaxContrast_Dw;
	btn.pcBmpFocus			= m_pcIncrMaxContrast_Up;
	btn.pcBmpDisabled		= m_pcIncrMaxContrast_Up;
	btn.dwFormat			= DT_VCENTER|DT_CENTER;
	m_pcIncreaseMax=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcIncreaseMax->Create(this,g_hf33AcuBold,0);
	m_pcIncreaseMax->SetText(_T(""));
	m_pcIncreaseMax->ShowWindow(SW_SHOW);


	btn.wID					= IDC_BTN_SCREENLOCK;	
	btn.poPosition.x		= 260;
	btn.poPosition.y		= 96;
	btn.pcBmpUp				= m_pcScreenlock_Up;
	btn.pcBmpDown			= m_pcScreenlock_Dw;
	btn.pcBmpFocus			= m_pcScreenlock_Up;
	btn.pcBmpDisabled		= m_pcScreenlock_Up;
	btn.dwFormat			= DT_VCENTER|DT_LEFT;
	m_pcScreenlock=new CPushBtn(btn,COLOR_TXTBTNUP);
	m_pcScreenlock->Create(this,g_hf15AcuMed,60);
	m_pcScreenlock->SetText(getModel()->GetLanguageString(IDS_BTN_SCREENLOCK));
	m_pcScreenlock->ShowWindow(SW_SHOW);


	btn.wID					= IDC_BTN_SCREENLOCKTIME;	
	btn.poPosition.x		= 595;
	btn.poPosition.y		= 157;
	btn.pcBmpUp				= m_pcScreenlocktime_Up;
	btn.pcBmpDown			= m_pcScreenlocktime_Dw;
	btn.pcBmpFocus			= m_pcScreenlocktime_Fc;
	btn.pcBmpDisabled		= m_pcScreenlocktime_Dis;
	btn.dwFormat			= DT_VCENTER|DT_RIGHT;
	//m_pcScreenlocktime=new CPushBtn(btn,RGB(230,230,255));
	m_pcScreenlocktime=new CPushBtn(btn,RGB(0,0,130));
	m_pcScreenlocktime->Create(this,g_hf8AcuBold,17);
	m_pcScreenlocktime->SetText(szScreenlocktime);
	m_pcScreenlocktime->ShowWindow(SW_SHOW);


	/*btn.wID					= IDC_BTN_SERVICE_DISPLCAL;	
	btn.poPosition.x		= 440;
	btn.poPosition.y		= 96;
	btn.pcBmpUp				= m_pcCal_Up;
	btn.pcBmpDown			= m_pcCal_Dw;
	btn.pcBmpFocus			= m_pcCal_Up;
	btn.pcBmpDisabled		= m_pcCal_Up;
	btn.dwFormat			= DT_VCENTER|DT_LEFT;
	m_pcCal=new CKbrdBtn(btn,COLOR_TXTBTNUP);
	m_pcCal->Create(this,g_hf15AcuMed,60);
	m_pcCal->SetText(getModel()->GetLanguageString(IDS_BTN_CALIBRATION));
	m_pcCal->ShowWindow(SW_HIDE);*/



	m_sliderAutolock = new CBitmapSlider();
	m_sliderAutolock->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|BS_OWNERDRAW, CRect(440,160,589,200), 
		this,IDC_SLD_AUTOLOCK);
	m_sliderAutolock->SetBitmapChannel( IDB_SLD_CHAN_GREY, NULL );
	m_sliderAutolock->SetBitmapThumb( IDB_SLD_THUMB_GREY, IDB_SLD_THUMB_ACT_GREY);
	m_sliderAutolock->SetRange( 0, 1 );
	if(m_bAutoScreenLock)
	{
		m_sliderAutolock->SetPos( 1 );
		if(m_pcScreenlocktime)
			m_pcScreenlocktime->EnableWindow(TRUE);
	}
	else
	{
		m_sliderAutolock->SetPos( 0 );
		if(m_pcScreenlocktime)
			m_pcScreenlocktime->EnableWindow(FALSE);
	}
	m_sliderAutolock->SetMargin( 5, 0, 6, 0 );
	m_sliderAutolock->DrawFocusRect( FALSE );


	m_sliderFilledGraph = new CBitmapSlider();
	m_sliderFilledGraph->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|BS_OWNERDRAW, CRect(440,230,589,270), 
		this,IDC_SLD_FILLGRAPH);
	m_sliderFilledGraph->SetBitmapChannel( IDB_SLD_CHAN_GREY, NULL );
	m_sliderFilledGraph->SetBitmapThumb( IDB_SLD_THUMB_GREY, IDB_SLD_THUMB_ACT_GREY);
	m_sliderFilledGraph->SetRange( 0, 1 );
	if(m_bGraphFilled)
		m_sliderFilledGraph->SetPos( 1 );
	else
		m_sliderFilledGraph->SetPos( 0 );
	m_sliderFilledGraph->SetMargin( 5, 0, 6, 0 );
	m_sliderFilledGraph->DrawFocusRect( FALSE );

	m_sliderAutoscaleGraph = new CBitmapSlider();
	m_sliderAutoscaleGraph->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|BS_OWNERDRAW, CRect(440,290,589,330), 
		this,IDC_SLD_AUTOGRAPH);
	m_sliderAutoscaleGraph->SetBitmapChannel( IDB_SLD_CHAN_GREY, NULL );
	m_sliderAutoscaleGraph->SetBitmapThumb( IDB_SLD_THUMB_GREY, IDB_SLD_THUMB_ACT_GREY);
	m_sliderAutoscaleGraph->SetRange( 0, 1 );
	if(m_bGraphAutoscale)
		m_sliderAutoscaleGraph->SetPos( 1 );
	else
		m_sliderAutoscaleGraph->SetPos( 0 );
	m_sliderAutoscaleGraph->SetMargin( 5, 0, 6, 0 );
	m_sliderAutoscaleGraph->DrawFocusRect( FALSE );

	m_sliderFilledTrend = new CBitmapSlider();
	m_sliderFilledTrend->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|BS_OWNERDRAW, CRect(440,360,589,400), 
		this,IDC_SLD_FILLTREND);
	m_sliderFilledTrend->SetBitmapChannel( IDB_SLD_CHAN_GREY, NULL );
	m_sliderFilledTrend->SetBitmapThumb( IDB_SLD_THUMB_GREY, IDB_SLD_THUMB_ACT_GREY);
	m_sliderFilledTrend->SetRange( 0, 1 );
	if(m_bTrendFilled)
		m_sliderFilledTrend->SetPos( 1 );
	else
		m_sliderFilledTrend->SetPos( 0 );
	m_sliderFilledTrend->SetMargin( 5, 0, 6, 0 );
	m_sliderFilledTrend->DrawFocusRect( FALSE );

	m_sliderAutoscaleTrend = new CBitmapSlider();
	m_sliderAutoscaleTrend->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|BS_OWNERDRAW, CRect(440,420,589,460), 
		this,IDC_SLD_AUTOTREND);
	m_sliderAutoscaleTrend->SetBitmapChannel( IDB_SLD_CHAN_GREY, NULL );
	m_sliderAutoscaleTrend->SetBitmapThumb( IDB_SLD_THUMB_GREY, IDB_SLD_THUMB_ACT_GREY);
	m_sliderAutoscaleTrend->SetRange( 0, 1 );
	if(m_bTrendAutoscale)
		m_sliderAutoscaleTrend->SetPos( 1 );
	else
		m_sliderAutoscaleTrend->SetPos( 0 );
	m_sliderAutoscaleTrend->SetMargin( 5, 0, 6, 0 );
	m_sliderAutoscaleTrend->DrawFocusRect( FALSE );

	
	m_iContrastParm=getModel()->getDATAHANDLER()->GetCurContrastParm();
	
}

/**********************************************************************************************//**
 * Shows
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CSubViewDisplay::Show(bool bShow)
{
	if(bShow)
	{
		ShowWindow(SW_SHOW);
		//SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);

		
		Draw();
	}
	else
	{
		ShowWindow(SW_SHOW);
		//SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);
	}
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewDisplay::OnDestroy()
{
	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}
	if(m_hDC)
		DeleteDC(m_hDC);
}

/**********************************************************************************************//**
 * Draws this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewDisplay::Draw()
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);



	CBrush cbrBack(BACKGND);
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf17AcuBold);
	int tc=SetTextColor(hdcMem,0x0000000);

	Rectangle(hdcMem, 0, 0, m_lX, m_lY);

	CBrush cbrFill;
	cbrFill.CreateSolidBrush(RGB(181,178,181));
	SelectObject(hdcMem,cbrFill);

	Rectangle(hdcMem, 100, 30, 680, 465);
	Rectangle(hdcMem, 120, 10, 660, 30);
	Rectangle(hdcMem, 120, 465, 660, 485);
	if(m_pcRegionTopLeCorner)
		m_pcRegionTopLeCorner->Draw(hdcMem,100,10);
	if(m_pcRegionTopRiCorner)
		m_pcRegionTopRiCorner->Draw(hdcMem,660,10);
	if(m_pcRegionBotLeCorner)
		m_pcRegionBotLeCorner->Draw(hdcMem,100,465);
	if(m_pcRegionBotRiCorner)
		m_pcRegionBotRiCorner->Draw(hdcMem,660,465);


	//m_sliderAutolock->UpdateWindow();

	CPen penLine;
	penLine.CreatePen(PS_SOLID,1,RGB(140,140,140));
	SelectObject(hdcMem,(HPEN)penLine);
	CBrush cbrRound(RGB(200,200,200));
	SelectObject(hdcMem,cbrRound);
	RoundRect(hdcMem, 110, 20, 670, 80,20,20);

	rc.left = 0;  
	rc.left = 130;  
	rc.top = 38;  
	//rc.right  = 800;  
	rc.right  = 450;  
	rc.bottom = 500;
	//cs = _T("Brightness:");
	CStringW cs = getModel()->GetLanguageString(IDS_TXT_BRIGHTNESS);
	//DrawText(hdcMem,cs,-1,&rc,DT_CENTER|DT_TOP|DT_SINGLELINE);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	/*MoveToEx(hdcMem, 112, 80, NULL);
	LineTo(hdcMem, 672, 80);*/

	RoundRect(hdcMem, 110, 90, 670, 210,20,20);

	rc.left = 130;  
	rc.top = 100;  
	rc.right  = 450;  
	rc.bottom = 500;
	cs = getModel()->GetLanguageString(IDS_TXT_TOUCH);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//
	MoveToEx(hdcMem, 260, 150, NULL);
	LineTo(hdcMem, 670, 150);

	

	/*MoveToEx(hdcMem, 112, 200, NULL);
	LineTo(hdcMem, 672, 200);*/

	RoundRect(hdcMem, 110, 220, 670, 340,20,20);

	rc.left = 130;  
	rc.top = 230;  
	rc.right  = 450;  
	rc.bottom = 500;
	cs = getModel()->GetLanguageString(IDS_TXT_GRAPHS);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	//
	MoveToEx(hdcMem, 260, 280, NULL);
	LineTo(hdcMem, 670, 280);


	/*MoveToEx(hdcMem, 112, 320, NULL);
	LineTo(hdcMem, 672, 320);*/

	RoundRect(hdcMem, 110, 350, 670, 470,20,20);

	rc.left = 130;  
	rc.top = 360;  
	rc.right  = 450;  
	rc.bottom = 500;
	cs = getModel()->GetLanguageString(IDS_MENU_TREND);
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	/*MoveToEx(hdcMem, 112, 440, NULL);
	LineTo(hdcMem, 672, 440);*/


	//
	MoveToEx(hdcMem, 260, 410, NULL);
	LineTo(hdcMem, 670, 410);

	SelectObject(hdcMem,g_hf10AcuBold);
	
	rc.left = 260;  
	rc.top = 170;  
	rc.right  = 450;  
	rc.bottom = 500;
	cs = getModel()->GetLanguageString(IDS_TXT_AUTOLOCK);
	//cs = _T("automatische Sperre");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 260;  
	rc.top = 240;  
	rc.right  = 450;  
	rc.bottom = 500;
	cs = getModel()->GetLanguageString(IDS_TXT_FILLED);
	//cs = _T("gefüllt");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 260;  
	rc.top = 295;  
	rc.right  = 450;  
	rc.bottom = 500;
	cs = getModel()->GetLanguageString(IDS_TXT_AUTOSCALE);
	//cs = _T("autoscale");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);


	rc.left = 260;  
	rc.top = 370;  
	rc.right  = 450;  
	rc.bottom = 500;
	cs = getModel()->GetLanguageString(IDS_TXT_FILLED);
	//cs = _T("gefüllt");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	rc.left = 260;  
	rc.top = 425;  
	rc.right  = 450;  
	rc.bottom = 500;
	cs = getModel()->GetLanguageString(IDS_TXT_AUTOSCALE);
	//cs = _T("autoscale");
	DrawText(hdcMem,cs,-1,&rc,DT_LEFT|DT_TOP|DT_SINGLELINE);

	

	CStringW szScreenlocktime=getModel()->GetLanguageString(IDS_TXT_OFF);

	//m_bAutoScreenLock=getModel()->getCONFIG()->IsAutoScreenlockActive();
	m_eAutoSreenlock=getModel()->getCONFIG()->GetAutoScreenlockTime();

	if(m_bAutoScreenLock==false)
		szScreenlocktime=getModel()->GetLanguageString(IDS_TXT_OFF);
	else
	{
		switch(m_eAutoSreenlock)
		{
		case ASL_20:
			{
				szScreenlocktime=_T("20 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS);
			}
			break;
		case ASL_30:
			{
				szScreenlocktime=_T("30 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS);
			}
			break;
		case ASL_40:
			{
				szScreenlocktime=_T("40 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS);
			}
			break;
		case ASL_50:
			{
				szScreenlocktime=_T("50 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS);
			}
			break;
		case ASL_60:
			{
				szScreenlocktime=_T("60 ")+getModel()->GetLanguageString(IDS_UNIT_SECONDS);
			}
			break;
		}
	}
	

	if(m_pcScreenlocktime)
		m_pcScreenlocktime->RefreshText(szScreenlocktime);

	if(m_bAutoScreenLock)
	{
		if(m_pcScreenlocktime)
			m_pcScreenlocktime->EnableWindow(TRUE);
	}
	else
	{
		if(m_pcScreenlocktime)
			m_pcScreenlocktime->EnableWindow(FALSE);
	}

	if(m_bGraphFilled)
	{
		if(m_pcFilledGraph)
			m_pcFilledGraph->Draw(hdcMem,595,232);
	}
	else
	{
		if(m_pcNotFilledGraph)
			m_pcNotFilledGraph->Draw(hdcMem,595,232);
	}

	if(m_bGraphAutoscale)
	{
		//m_pcAutoscale->Draw(hdcMem,606,277);
		SelectObject(hdcMem,g_hf7AcuNorm);
		rc.left=590;
		rc.right=656;
		/*rc.top=278;
		rc.bottom=291;*/
		rc.top=300;
		rc.bottom=313;
		
		CStringW szAuto=_T("  ")+getModel()->GetLanguageString(IDS_TXT_AUTOSCALE)+_T("  ");
		SetBkMode(hdcMem,OPAQUE);
		SetBkColor(hdcMem,RGB(140,140,140));
		SetTextColor(hdcMem,RGB(255,255,255));
		DrawText(hdcMem,szAuto,-1,&rc,DT_RIGHT|DT_END_ELLIPSIS);
	}
	else
	{
		if(m_pcNoautoscale)
			m_pcNoautoscale->Draw(hdcMem,610,287);
	}


	if(m_bTrendFilled)
	{
		if(m_pcFilledGraph)
			m_pcFilledGraph->Draw(hdcMem,595,362);
	}
	else
	{
		if(m_pcNotFilledGraph)
			m_pcNotFilledGraph->Draw(hdcMem,595,362);
	}


	if(m_bTrendAutoscale)
	{
		//m_pcAutoscale->Draw(hdcMem,606,397);
		SelectObject(hdcMem,g_hf7AcuNorm);
		rc.left=590;
		rc.right=656;
		/*rc.top=398;
		rc.bottom=411;*/
		rc.top=430;
		rc.bottom=443;

		CStringW szAuto=_T("  ")+getModel()->GetLanguageString(IDS_TXT_AUTOSCALE)+_T("  ");
		SetBkMode(hdcMem,OPAQUE);
		SetBkColor(hdcMem,RGB(140,140,140));
		SetTextColor(hdcMem,RGB(255,255,255));
		DrawText(hdcMem,szAuto,-1,&rc,DT_RIGHT|DT_END_ELLIPSIS);
	}
	else
	{
		if(m_pcNoautoscale)
			m_pcNoautoscale->Draw(hdcMem,610,417);
	}

	if(m_pcNight)
		m_pcNight->Draw(hdcMem,260,30);

	if(getModel()->getDATAHANDLER()->IsLEDdisplay())
	{
		switch(m_iContrastParm)
		{
		case 103:
			{
				if(m_pcBrightness5)
					m_pcBrightness5->Draw(hdcMem,615,32);

			}
			break;
		case 333:
			{
				if(m_pcBrightness4)
					m_pcBrightness4->Draw(hdcMem,615,32);
			}
			break;
		case 563:
			{
				if(m_pcBrightness3)
					m_pcBrightness3->Draw(hdcMem,615,32);
			}
			break;
		case 793:
			{
				if(m_pcBrightness2)
					m_pcBrightness2->Draw(hdcMem,615,32);
			}
			break;
		case 1023:
			{
				if(m_pcBrightness1)
					m_pcBrightness1->Draw(hdcMem,615,32);
			}
			break;
		default:
			{
				m_iContrastParm=getModel()->getDATAHANDLER()->IncreaseMaxContrast();
				if(m_pcBrightness1)
					m_pcBrightness1->Draw(hdcMem,615,32);
			}
			break;
		}
	}
	else
	{
		switch(m_iContrastParm)
		{
		case 23:
			{
				if(m_pcBrightness1)
					m_pcBrightness1->Draw(hdcMem,615,32);
			}
			break;
		case 273:
			{
				if(m_pcBrightness2)
					m_pcBrightness2->Draw(hdcMem,615,32);
			}
			break;
		case 523:
			{
				if(m_pcBrightness3)
					m_pcBrightness3->Draw(hdcMem,615,32);
			}
			break;
		case 773:
			{
				if(m_pcBrightness4)
					m_pcBrightness4->Draw(hdcMem,615,32);
			}
			break;
		case 1023:
			{
				if(m_pcBrightness5)
					m_pcBrightness5->Draw(hdcMem,615,32);
			}
			break;
		default:
			{
				m_iContrastParm=getModel()->getDATAHANDLER()->IncreaseMaxContrast();
				if(m_pcBrightness1)
					m_pcBrightness1->Draw(hdcMem,615,32);
			}
			break;
		}
	}
	


	//dc.BitBlt(0,0,m_lX,m_lY,CDC::FromHandle(hdcMem),0,0,SRCCOPY);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	penLine.DeleteObject();



	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);

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

LRESULT CSubViewDisplay::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_MENUBUTTONDOWN:
		{
			switch(wParam)
			{
			case IDC_BTN_SCREENLOCKTIME:
				{
					OpenSettingsView();
					return 1;
				}
				break;
			case IDC_BTN_SCREENLOCK:
				{
					//Screenlock();
					return 1;
				}
				break;
			case IDC_BTN_CONTRAST_INCR:
				{
					Increase();
					return 1;
				}
				break;
			case IDC_BTN_CONTRAST_DECR:
				{
					Decrease();
					return 1;
				}
				break;
			case IDC_BTN_CONTRAST_INCRMAX:
				{
					IncreaseMax();
					return 1;
				}
				break;
			case IDC_BTN_CONTRAST_DECRMAX:
				{
					DecreaseMax();
					return 1;
				}
				break;
			default:
				{
					
				}
				break;
			}
		}
		break;
		case WM_MENUBUTTONUP:
		{
			switch(wParam)
			{
			case IDC_BTN_SCREENLOCK:
				{
					Screenlock();
					return 1;
				}
			default:
				{
					
				}
				break;
			}
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

/**********************************************************************************************//**
 * Opens settings view
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewDisplay::OpenSettingsView()
{
	if(GetParent())
		GetParent()->PostMessage(WM_OPEN_SETTING,IDC_BTN_SCREENLOCKTIME);
	/*if(m_pcSubViewSetting==NULL)
	{
		this->Show(false);
		CreateSubViewSetting();
		ShowSubViewSetting(true);
	}
	else
	{
		this->Show(false);
		DestroySubViewSetting();
		CreateSubViewSetting();
		ShowSubViewSetting(true);
	}*/

	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);
}

/**********************************************************************************************//**
 * Increases this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewDisplay::Increase()
{
	m_iContrastParm=getModel()->getDATAHANDLER()->IncreaseContrast();

	Draw();

	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);
}

/**********************************************************************************************//**
 * Decreases this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewDisplay::Decrease()
{
	m_iContrastParm=getModel()->getDATAHANDLER()->DecreaseContrast();

	Draw();
	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);
}

/**********************************************************************************************//**
 * Increase maximum
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewDisplay::IncreaseMax()
{
	m_iContrastParm=getModel()->getDATAHANDLER()->IncreaseMaxContrast();

	Draw();
	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);
}

/**********************************************************************************************//**
 * Decrease maximum
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewDisplay::DecreaseMax()
{
	m_iContrastParm=getModel()->getDATAHANDLER()->DecreaseMaxContrast();

	Draw();
	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);
}

/**********************************************************************************************//**
 * Screenlocks this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CSubViewDisplay::Screenlock()
{
	if(GetParent())
	{
		GetParent()->PostMessage(WM_SCREENLOCK);
		GetParent()->PostMessage(WM_SET_SETUPTIMER);
	}
}

/**********************************************************************************************//**
 * Executes my message action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	wParam	The wParam field of the message.
 * \param	lParam	The lParam field of the message.
 *
 * \return	A LRESULT.
 **************************************************************************************************/

LRESULT CSubViewDisplay::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case IDC_SLD_AUTOLOCK:
		{
			if(lParam==0)
			{
				m_bAutoScreenLock=false;
			}
			else
			{
				m_bAutoScreenLock=true;
			}

			getModel()->getCONFIG()->SetAutoScreenlockActive(m_bAutoScreenLock);

			Draw();

			if(GetParent())
				GetParent()->PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case IDC_SLD_FILLGRAPH:
		{
			if(lParam==0)
			{
				m_bGraphFilled=false;
			}
			else
			{
				m_bGraphFilled=true;
			}
			getModel()->getCONFIG()->GraphSetFilled(m_bGraphFilled);
			Draw();

			if(GetParent())
				GetParent()->PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case IDC_SLD_AUTOGRAPH:
		{
			if(lParam==0)
			{
				m_bGraphAutoscale=false;
			}
			else
			{
				m_bGraphAutoscale=true;
			}
			getModel()->getCONFIG()->GraphSetAutoScale(m_bGraphAutoscale);
			Draw();

			if(GetParent())
				GetParent()->PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case IDC_SLD_FILLTREND:
		{
			if(lParam==0)
			{
				m_bTrendFilled=false;
			}
			else
			{
				m_bTrendFilled=true;
			}
			getModel()->getCONFIG()->TrendSetFilled(m_bTrendFilled);
			Draw();

			if(GetParent())
				GetParent()->PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	case IDC_SLD_AUTOTREND:
		{
			if(lParam==0)
			{
				m_bTrendAutoscale=false;
			}
			else
			{
				m_bTrendAutoscale=true;
			}
			getModel()->getCONFIG()->TrendSetAutoScale(m_bTrendAutoscale);
			Draw();

			if(GetParent())
				GetParent()->PostMessage(WM_SET_SETUPTIMER);
		}
		break;
	}
	return 1;
}

