// ViewTrend.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ViewTrend.h"
#include "TlsFile.h"
#include <vector>
#include <algorithm>
#include <functional>



using namespace std;
/** Define a template class vector of int */
typedef vector<int, allocator<int> > IntVector ;

/** Define an iterator for template class vector of strings */
typedef IntVector::iterator IntVectorIt ;

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



WORD* CViewTrend::m_pBufferTrend1=NULL;
WORD* CViewTrend::m_pBufferTrend2=NULL;
WORD* CViewTrend::m_pBufferTrend3=NULL;

/**********************************************************************************************//**
 * Initializes a new instance of the CViewTrend class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	ViewID	Identifier for the view.
 **************************************************************************************************/

CViewTrend::CViewTrend(int ViewID):
CMVView(ViewID)
{
	InitializeCriticalSection(&csTrends);

	m_bExit=false;
	m_bLoadDataRunning=false;
	m_bFilledGraph=getModel()->getCONFIG()->TrendIsFilled();

	m_pWndHourglass=NULL;

	m_pcTrend1Diagram=NULL;
	m_pcTrend2Diagram=NULL;
	m_pcTrend3Diagram=NULL;
	m_pWndMenuTrend=NULL;
	m_pWndMenuTrendtype=NULL;
	m_pWndTrendTimeaxis=NULL;

	m_pPlotItemsTrend1=NULL;
	m_lpfsTrend1=NULL;

	m_pPlotItemsTrend2=NULL;
	m_lpfsTrend2=NULL;

	m_pPlotItemsTrend3=NULL;
	m_lpfsTrend3=NULL;

	m_pcSelectTrend1=NULL;
	m_pcSelectTrend2=NULL;
	m_pcSelectTrend3=NULL;

	m_pcSelectTrend_Up=NULL;
	m_pcSelectTrend_Dw=NULL;

	m_bWndMenuTrendtype=false;

	m_iCurBtnSelTrend=0;

	m_iCurFocusedWnd=0;
	m_bGraphSelected = false;

	m_iTypeTrend1=0;
	m_iTypeTrend2=0;
	m_iTypeTrend3=0;

	m_pcNumPara2er=NULL;

	m_iCountLockedMoveLeft=0;
	m_iCountLockedMoveRight=0;

	m_iTrendSpan=0;
	
	m_hdcStatic=NULL;
	m_hbmpStatic=NULL;
	m_hBmpPrevStatic=NULL;
	m_hDC=NULL;
	m_hBmp=NULL;
	m_hBmpPrev=NULL;
	m_lX=0;
	m_lY=0;
	m_lXo=0;
	m_lYo=0;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CViewTrend class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CViewTrend::~CViewTrend(void)
{
	EnterCriticalSection(&csTrends);

	if (m_pBufferTrend1 != NULL) {
		delete [] m_pBufferTrend1;
	}
	if (m_pBufferTrend2 != NULL) {
		delete [] m_pBufferTrend2;
	}
	if (m_pBufferTrend3 != NULL) {
		delete [] m_pBufferTrend3;
	}

	m_pBufferTrend1=NULL;
	m_pBufferTrend2=NULL;
	m_pBufferTrend3=NULL;


	if(m_pcNumPara2er)
		delete m_pcNumPara2er;
	m_pcNumPara2er=NULL;

	delete m_pcSelectTrend1;
	m_pcSelectTrend1=NULL;
	delete m_pcSelectTrend2;
	m_pcSelectTrend2=NULL;
	delete m_pcSelectTrend3;
	m_pcSelectTrend3=NULL;

	delete m_pcSelectTrend_Up;
	m_pcSelectTrend_Up=NULL;
	delete m_pcSelectTrend_Dw;
	m_pcSelectTrend_Dw=NULL;


	if(m_pPlotItemsTrend1 !=NULL)
	{
		delete []m_pPlotItemsTrend1;
		m_pPlotItemsTrend1=NULL;
	}
	if(m_lpfsTrend1 !=NULL)
	{
		delete m_lpfsTrend1;
	}

	if(m_pPlotItemsTrend2 !=NULL)
	{
		delete []m_pPlotItemsTrend2;
		m_pPlotItemsTrend2=NULL;
	}
	if(m_lpfsTrend2 !=NULL)
	{
		delete m_lpfsTrend2;
	}

	if(m_pPlotItemsTrend3 !=NULL)
	{
		delete []m_pPlotItemsTrend3;
		m_pPlotItemsTrend3=NULL;
	}
	if(m_lpfsTrend3 !=NULL)
	{
		delete m_lpfsTrend3;
	}

	LeaveCriticalSection(&csTrends);

	DeleteCriticalSection(&csTrends);
}


BEGIN_MESSAGE_MAP(CViewTrend, CMVView)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/**********************************************************************************************//**
 * Creates the view
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewTrend::CreateView()
{
	RECT rcLd={0,48,605,540};
	

	if (!Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_TREND))
	{
		return false;
	}

	Initialize();
	Show();
	PostMessage(WM_LOAD_TREND);
	
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

BOOL CViewTrend::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
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
		//CBrush cbrBack(RGB(200,200,255));
		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		m_hdcStatic = CreateCompatibleDC(m_hDC);
		m_hbmpStatic = CreateCompatibleBitmap(m_hDC, m_lX, m_lY);
		m_hBmpPrevStatic=(HBITMAP)SelectObject(m_hdcStatic, m_hbmpStatic);

		HPEN hpenprevStat=(HPEN)SelectObject(m_hdcStatic, (HPEN)GetStockObject(NULL_PEN));	
		HBRUSH hbrprevStat=(HBRUSH)SelectObject(m_hdcStatic,cbrBack); 
		Rectangle(m_hdcStatic, 0, 0, m_lX, m_lY);

		SelectObject(m_hdcStatic,hpenprevStat);
		SelectObject(m_hdcStatic,hbrprevStat);
		SelectObject(m_hDC,hpenprev);
		SelectObject(m_hDC,hbrprev);
		
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

bool CViewTrend::Initialize()
{
	bool result = false;

	HDC hDC = ::GetDC(m_hWnd);

	m_pcSelectTrend_Up		= new CBmp(theApp.m_hInstance,hDC,	IDB_GRAPHBTN_ARW_UP);
	m_pcSelectTrend_Dw		= new CBmp(theApp.m_hInstance,hDC,	IDB_TRENDBTN_ARW_DW);

	m_pcNumPara2er		= new CBmp(theApp.m_hInstance,hDC,	IDB_NUMPARA_2);

	m_iTrendSpan=getModel()->getCONFIG()->GetTrendSpan();
	m_iTypeTrend1=getModel()->getCONFIG()->GetTrendType1();
	m_iTypeTrend2=getModel()->getCONFIG()->GetTrendType2();
	m_iTypeTrend3=getModel()->getCONFIG()->GetTrendType3();
	
	BTN btn;

	btn.wID					= IDC_BTN_SELTREND_1;	
	btn.poPosition.x		= 0;
	btn.poPosition.y		= 0;
	btn.pcBmpUp				= m_pcSelectTrend_Up;
	btn.pcBmpDown			= m_pcSelectTrend_Dw;
	btn.pcBmpFocus			= m_pcSelectTrend_Dw;
	btn.pcBmpDisabled		= m_pcSelectTrend_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	CStringW szType=GetNameOfTrendType(m_iTypeTrend1);

	m_pcSelectTrend1=new CSelectTrendBtn(btn,COLOR_TXTBTNUP);
	m_pcSelectTrend1->Create(this,g_hf7AcuBold,0);
	m_pcSelectTrend1->SetText(szType,szType);
	m_pcSelectTrend1->ShowWindow(SW_HIDE);
	m_plSelectTrendBtn.AddTail(m_pcSelectTrend1);

	btn.wID					= IDC_BTN_SELTREND_2;	
	btn.poPosition.x		= 0;
	btn.poPosition.y		= 148;
	btn.pcBmpUp				= m_pcSelectTrend_Up;
	btn.pcBmpDown			= m_pcSelectTrend_Dw;
	btn.pcBmpFocus			= m_pcSelectTrend_Dw;
	btn.pcBmpDisabled		= m_pcSelectTrend_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	szType=GetNameOfTrendType(m_iTypeTrend2);

	m_pcSelectTrend2=new CSelectTrendBtn(btn,COLOR_TXTBTNUP);
	m_pcSelectTrend2->Create(this,g_hf7AcuBold,0);
	m_pcSelectTrend2->SetText(szType,szType);
	m_pcSelectTrend2->ShowWindow(SW_HIDE);
	m_plSelectTrendBtn.AddTail(m_pcSelectTrend2);

	btn.wID					= IDC_BTN_SELTREND_3;	
	btn.poPosition.x		= 0;
	btn.poPosition.y		= 296;
	btn.pcBmpUp				= m_pcSelectTrend_Up;
	btn.pcBmpDown			= m_pcSelectTrend_Dw;
	btn.pcBmpFocus			= m_pcSelectTrend_Dw;
	btn.pcBmpDisabled		= m_pcSelectTrend_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	szType=GetNameOfTrendType(m_iTypeTrend3);

	m_pcSelectTrend3=new CSelectTrendBtn(btn,COLOR_TXTBTNUP);
	m_pcSelectTrend3->Create(this,g_hf7AcuBold,0);
	m_pcSelectTrend3->SetText(szType,szType);
	m_pcSelectTrend3->ShowWindow(SW_HIDE);
	m_plSelectTrendBtn.AddTail(m_pcSelectTrend3);

	SYSTEMTIME st;
	GetLocalTime(&st);
	m_dtOffsetTime=COleDateTime(st);
	m_dtStartTime=COleDateTime(st);

	CreateTrend1Diagram(40,4,TREND_WIDTH,TREND_HEIGHT);
	CreateTrend2Diagram(40,152,TREND_WIDTH,TREND_HEIGHT);
	CreateTrend3Diagram(40,300,TREND_WIDTH,TREND_HEIGHT);

	DEBUGMSG(TRUE, (TEXT("CreateWndTrendTimeaxis\r\n")));
	CreateWndTrendTimeaxis();

	COleDateTimeSpan tsMax;
	tsMax.SetDateTimeSpan(0, 0, TREND_5DAYS, 0);
	COleDateTime tmEndTime=m_dtStartTime-tsMax;

	if(m_pWndTrendTimeaxis!=NULL && m_pcTrend1Diagram!=NULL)
	{
		m_pWndTrendTimeaxis->SetTimeRange(m_dtStartTime,tmEndTime,m_pcTrend1Diagram->GetOffsetMinutes(), (int)m_pcTrend1Diagram->GetTrendSpan());
	}

	DEBUGMSG(TRUE, (TEXT("CreateWndMenuTrend\r\n")));
	CreateWndMenuTrend();

	m_pBufferTrend1=new WORD[MAXSIZE_TREND_BUFFER];
	m_pBufferTrend2=new WORD[MAXSIZE_TREND_BUFFER];
	m_pBufferTrend3=new WORD[MAXSIZE_TREND_BUFFER];

	//release the Display DC
	::ReleaseDC(m_hWnd, hDC);

	return result;
}

/**********************************************************************************************//**
 * Draw trends
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewTrend::DrawTrends()
{
	DEBUGMSG(TRUE, (TEXT("DrawTrends\r\n")));

	if(m_bExit)
		return;

	DrawTrend1();
	Sleep(0);
	DrawTrend2();
	Sleep(0);
	DrawTrend3();
	Sleep(0);
}

/**********************************************************************************************//**
 * Loads trend data
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	type 	The type.
 * \param	trend	The trend.
 **************************************************************************************************/

void CViewTrend::loadTrendData(BYTE type, eTrend trend)
{
	Sleep(0);

	//DEBUGMSG(TRUE, (TEXT("loadTrendData\r\n")));

	UINT iFactor=1;
	int nMax=0;
	bool bFileExists=false;
	bool bDeserialized=true;
	CStringW szFile=_T("");
	UINT iTrendBufCount=0;
	int iBufPos=0;
	WORD wFilenum=0;
	WORD wValue=0;
	WORD *pBuffer=NULL;
	CString szTrendFolder=_T("\\sdcard\\");

	if(CTlsFile::Exists(szTrendFolder)==false)
	{
		szTrendFolder=_T("\\FFSDISK\\");
	}

	switch(trend)
	{
	case TREND1:
		{
			pBuffer=m_pBufferTrend1;
		}
		break;
	case TREND2:
		{
			pBuffer=m_pBufferTrend2;
		}
		break;
	case TREND3:
		{
			pBuffer=m_pBufferTrend3;
		}
		break;
	}

	switch(type)
	{
	case TREND_PINSP:
		{
			iFactor=10;
			szTrendFolder+=IDS_TRD_FOLDER_PINSP;
		}
		break;
	case TREND_PMEAN:
		{
			iFactor=1;
			szTrendFolder+=IDS_TRD_FOLDER_PMEAN;
		}
		break;
	case TREND_FIO2:
		{
			iFactor=1;//newtrend
			szTrendFolder+=IDS_TRD_FOLDER_FIO2;
		}
		break;
	case TREND_VTE:
		{
			iFactor=1;
			szTrendFolder+=IDS_TRD_FOLDER_VTE;
		}
		break;
	case TREND_COMPLIANCE:
		{
			iFactor=1;
			szTrendFolder+=IDS_TRD_FOLDER_COMPLIANCE;
		}
		break;
	case TREND_CO2HFO:
		{
			iFactor=1;
			szTrendFolder+=IDS_TRD_FOLDER_CO2HFO;
		}
		break;
	case TREND_MV:
		{
			iFactor=1;
			szTrendFolder+=IDS_TRD_FOLDER_MV;
		}
		break;
	case TREND_HFAMP:
		{
			iFactor=10;
			szTrendFolder+=IDS_TRD_FOLDER_HFAMP;
		}
		break;
	case TREND_RSBI:
		{
			iFactor=1;
			szTrendFolder+=IDS_TRD_FOLDER_RSBI;
		}
		break;
	case TREND_SHAREMVMAND:
		{
			iFactor=1;
			szTrendFolder+=IDS_TRD_FOLDER_SHAREMVMAND;
		}
		break;
	case TREND_RESISTANCE:
		{
			iFactor=1;
			szTrendFolder+=IDS_TRD_FOLDER_RESISTANCE;
		}
		break;
	case TREND_LEAK:
		{
			iFactor=1;
			szTrendFolder+=IDS_TRD_FOLDER_LEAK;
		}
		break;
	case TREND_SPO2:
		{
			iFactor=10;
			szTrendFolder+=IDS_TRD_FOLDER_SPO2;
		}
		break;
	case TREND_SPO2PR:
		{
			iFactor=1;
			szTrendFolder+=IDS_TRD_FOLDER_SPO2PR;
		}
		break;
	case TREND_SPO2PI:
		{
			iFactor=100;
			szTrendFolder+=IDS_TRD_FOLDER_SPO2PI;
		}
		break;
	case TREND_ETCO2:
		{
			iFactor=10;
			szTrendFolder+=IDS_TRD_FOLDER_ETCO2;
		}
		break;
	case TREND_FREQUENCY:
		{
			iFactor=1;
			szTrendFolder+=IDS_TRD_FOLDER_FREQUENCY;
		}
		break;
	}

	//DEBUGMSG(TRUE, (TEXT("LoadTrendData %s\r\n"),szTrendFolder));

	try
	{
		wFilenum=(WORD)getModel()->getCONFIG()->GetLastWrittenTrendFile(type);
		szFile.Format(_T("%s%d%s"),szTrendFolder,wFilenum, IDS_TRD_FILE_ID);
		bFileExists=CTlsFile::Exists(szFile);

		//DEBUGMSG(TRUE, (TEXT("LoadTrendData1 %s\r\n"),szFile));


		while(		bFileExists 
			&&	bDeserialized 
			&&	nMax<MAXTRENDFILES
			&&	iTrendBufCount<MAXSIZE_TREND_BUFFER
			&&	!m_bExit)
		{
			bDeserialized=getModel()->getDATAHANDLER()->DeserializeTempTrend(type, wFilenum);
			iBufPos=getModel()->getDATAHANDLER()->m_cTrendTemporary.GetBufferCount()-1;

			for(iBufPos;iBufPos>=0;iBufPos--)
			{
				if(m_bExit)
				{
					return;
				}

				if(iTrendBufCount<MAXSIZE_TREND_BUFFER)
				{
					wValue=getModel()->getDATAHANDLER()->m_cTrendTemporary.GetBufferValue(iBufPos)/iFactor;
					pBuffer[iTrendBufCount]=wValue;
				}
				else
					iBufPos=-1;
				iTrendBufCount++;

				Sleep(0);
			}

			wFilenum--;
			if(wFilenum<=0)
				wFilenum=MAXTRENDFILES;
			szFile.Format(_T("%s%d%s"),szTrendFolder,wFilenum, IDS_TRD_FILE_ID);
			bFileExists=CTlsFile::Exists(szFile);
			//DEBUGMSG(TRUE, (TEXT("LoadTrendData2 %s\r\n"),szFile));
			nMax++;

			Sleep(0);
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CViewTrend::LoadTrendData EV_ALARMSTATE_CHANGED"));
	}
}

/**********************************************************************************************//**
 * Loads all trend data
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewTrend::loadAllTrendData()
{
	DEBUGMSG(TRUE, (TEXT("loadAllTrendData\r\n")));

	if(m_bExit)
		return;

	EnterCriticalSection(&getModel()->getDATAHANDLER()->csTrend);
	loadTrendData(m_iTypeTrend1,TREND1);
	loadTrendData(m_iTypeTrend2,TREND2);
	loadTrendData(m_iTypeTrend3,TREND3);
	LeaveCriticalSection(&getModel()->getDATAHANDLER()->csTrend);
}

//void CViewTrend::NewTestData()
//{
//	//m_rbufTREND
//	GenerateTestData(m_iTypeTrend1, m_pBufferTrend1);
//	GenerateTestData(m_iTypeTrend2, m_pBufferTrend2);
//	GenerateTestData(m_iTypeTrend3, m_pBufferTrend3);
//}

//void CViewTrend::GenerateTestData(UINT iTypeTrend, WORD* buffer)
//{
//	BYTE byTrendData_Pmean=20;
//	BYTE byTrendData_FiO2=21;
//	BYTE byTrendData_Pinsp=30;
//	WORD wTrendData_Vte=80;
//	WORD wTrendData_Compliance=80;
//	WORD wTrendData_CO2HFO=100;
//	//int iSize=VALUESPERMINUTE*MAX_TREND_MINUTES;
//
//	switch(iTypeTrend)
//	{
//	case TREND_PINSP:
//		{
//			for(int i=0;i<MAXSIZE_TREND_BUFFER;i++)
//			{
//				if(i%TREND_4HOURS==0 && i!=0)
//				{
//					if(byTrendData_Pinsp>2)
//						byTrendData_Pinsp=byTrendData_Pinsp-2;
//				}
//				if(i%TREND_16HOURS==0 && i!=0)
//				{
//					byTrendData_Pinsp=30;
//				}
//				buffer[i]=byTrendData_Pinsp;
//			}
//		}
//		break;
//	case TREND_PMEAN:
//		{
//			for(int i=0;i<MAXSIZE_TREND_BUFFER;i++)
//			{
//				if(i%TREND_4HOURS==0 && i!=0)
//				{
//					byTrendData_Pmean=byTrendData_Pmean-1;
//				}
//				if(i%TREND_16HOURS==0 && i!=0)
//				{
//					byTrendData_Pmean=20;
//				}
//				buffer[i]=byTrendData_Pmean;
//			}
//		}
//		break;
//	case TREND_FIO2:
//		{
//			for(int i=0;i<MAXSIZE_TREND_BUFFER;i++)
//			{
//				if(i%TREND_4HOURS==0 && i!=0)
//				{
//					byTrendData_FiO2=byTrendData_FiO2-1;
//				}
//				if(i%TREND_16HOURS==0 && i!=0)
//				{
//					byTrendData_FiO2=21;
//				}
//				buffer[i]=byTrendData_FiO2;
//			}
//		}
//		break;
//	case TREND_VTE:
//		{
//			for(int i=0;i<MAXSIZE_TREND_BUFFER;i++)
//			{
//				if(i%TREND_4HOURS==0 && i!=0)
//				{
//					wTrendData_Vte=wTrendData_Vte+40;
//					}
//				if(i%TREND_16HOURS==0 && i!=0)
//				{
//					wTrendData_Vte=80;
//				}
//				buffer[i]=wTrendData_Vte;
//			}
//		}
//		break;
//	case TREND_COMPLIANCE:
//		{
//			for(int i=0;i<MAXSIZE_TREND_BUFFER;i++)
//			{
//				if(i%TREND_4HOURS==0 && i!=0)
//				{
//					wTrendData_Compliance=wTrendData_Compliance-10;
//				}
//				if(i%TREND_16HOURS==0 && i!=0)
//				{
//					wTrendData_Compliance=80;
//				}
//				buffer[i]=wTrendData_Compliance;
//			}
//		}
//		break;
//	case TREND_CO2HFO:
//		{
//			for(int i=0;i<MAXSIZE_TREND_BUFFER;i++)
//			{
//				if(i%TREND_4HOURS==0 && i!=0)
//				{
//					wTrendData_CO2HFO=wTrendData_CO2HFO+500;
//				}
//				if(i%TREND_16HOURS==0 && i!=0)
//				{
//					wTrendData_CO2HFO=100;
//				}
//				buffer[i]=wTrendData_CO2HFO;
//			}
//		}
//		break;
//	}
//}

/**********************************************************************************************//**
 * Gets name of trend type
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	type	The type.
 *
 * \return	The name of trend type.
 **************************************************************************************************/

CStringW CViewTrend::GetNameOfTrendType(UINT type) 
{
	switch(type)
	{
	case TREND_PINSP:
		{
			return getModel()->GetLanguageString(IDS_TREND_PINSP);
		}
		break;
	case TREND_PMEAN:
		{
			return getModel()->GetLanguageString(IDS_TREND_PMEAN);
		}
		break;
	case TREND_FIO2:
		{
			return getModel()->GetLanguageString(IDS_TREND_FIO2);
		}
		break;
	case TREND_VTE:
		{
			return getModel()->GetLanguageString(IDS_TREND_VTE);
		}
		break;
	case TREND_COMPLIANCE:
		{
			return getModel()->GetLanguageString(IDS_TREND_COMPL);
		}
		break;
	case TREND_CO2HFO:
		{
			return getModel()->GetLanguageString(IDS_TREND_CO2);
		}
		break;
	case TREND_MV:
		{
			return getModel()->GetLanguageString(IDS_TREND_MV);
		}
		break;
	case TREND_HFAMP:
		{
			return getModel()->GetLanguageString(IDS_TREND_HFAMP);
		}
		break;
	case TREND_RSBI:
		{
			return _T("RSBI");
		}
		break;
	case TREND_SHAREMVMAND:
		{
			return _T("mand% MV");
		}
		break;
	case TREND_RESISTANCE:
		{
			return _T("RESISTANCE");
		}
		break;
	case TREND_LEAK:
		{
			return _T("LEAK");
		}
		break;
	case TREND_SPO2:
		{
			return _T("SPO2");
		}
		break;
	case TREND_SPO2PR:
		{
			return _T("SPO2 PR");
		}
		break;
	case TREND_SPO2PI:
		{
			return _T("SPO2 PI");
		}
		break;
	case TREND_ETCO2:
		{
			return _T("ETCO2");
		}
		break;
	case TREND_FREQUENCY:
		{
			return _T("FREQ");
		}
		break;
	}

	return _T("");
}

/**********************************************************************************************//**
 * Gets maximum scale of trend type
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	type	The type.
 *
 * \return	The maximum scale of trend type.
 **************************************************************************************************/

int CViewTrend::GetMaxScaleOfTrendType(UINT type)
{
	//DEBUGMSG(TRUE, (TEXT("GetMaxScaleOfTrendType\r\n")));
	int iYscale=0;
	switch(type)
	{
	case TREND_PINSP:
		{
			iYscale=getModel()->getCONFIG()->TrendGetPInspMax();
		}
		break;
	case TREND_PMEAN:
		{
			iYscale=getModel()->getCONFIG()->TrendGetPMeanMax();
		}
		break;
	case TREND_FIO2:
		{
			iYscale=getModel()->getCONFIG()->TrendGetFIO2Max();
		}
		break;
	case TREND_VTE:
		{
			iYscale=getModel()->getCONFIG()->TrendGetVteMax();
		}
		break;
	case TREND_COMPLIANCE:
		{
			iYscale=getModel()->getCONFIG()->TrendGetComplianceMax();
		}
		break;
	case TREND_CO2HFO:
		{
			iYscale=getModel()->getCONFIG()->TrendGetDCO2Max();
		}
		break;
	case TREND_MV:
		{
			iYscale=getModel()->getCONFIG()->TrendGetMVMax();
		}
		break;
	case TREND_HFAMP:
		{
			iYscale=getModel()->getCONFIG()->TrendGetHFAMPMax();
		}
		break;
	case TREND_RSBI:
		{
			iYscale=getModel()->getCONFIG()->TrendGetRSBIMax();
		}
		break;
	case TREND_SHAREMVMAND:
		{
			iYscale=getModel()->getCONFIG()->TrendGetSHAREMVMANDMax();
		}
		break;
	case TREND_RESISTANCE:
		{
			iYscale=getModel()->getCONFIG()->TrendGetRESISTANCEMax();
		}
		break;
	case TREND_LEAK:
		{
			iYscale=getModel()->getCONFIG()->TrendGetLEAKMax();
		}
		break;
	case TREND_SPO2:
		{
			iYscale=getModel()->getCONFIG()->TrendGetSPO2Max();
		}
		break;
	case TREND_SPO2PR:
		{
			iYscale=getModel()->getCONFIG()->TrendGetSPO2PRMax();
		}
		break;
	case TREND_SPO2PI:
		{
			iYscale=getModel()->getCONFIG()->TrendGetSPO2PIMax();
		}
		break;
	case TREND_ETCO2:
		{
			iYscale=getModel()->getCONFIG()->TrendGetETCO2Max();
		}
		break;
	case TREND_FREQUENCY:
		{
			iYscale=getModel()->getCONFIG()->TrendGetFREQUENCYMax();
		}
		break;
	}

	return iYscale;
}

/**********************************************************************************************//**
 * Gets unit of trend type
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	type	The type.
 *
 * \return	The unit of trend type.
 **************************************************************************************************/

CStringW CViewTrend::GetUnitOfTrendType(UINT type) 
{
	CStringW szUnit = _T("");
	switch(type)
	{
	case TREND_PINSP:
		{
			if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
			{
				szUnit.Format(_T("%s%s%s"),_T("["),getModel()->GetLanguageString(IDS_UNIT_MBAR),_T("]"));
			}
			else
			{
				szUnit.Format(_T("%s%s%s"),_T("["),getModel()->GetLanguageString(IDS_UNIT_CMH2O),_T("]"));
			}
		}
		break;
	case TREND_PMEAN:
		{
			if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
			{
				szUnit.Format(_T("%s%s%s"),_T("["),getModel()->GetLanguageString(IDS_UNIT_MBAR),_T("]"));
			}
			else
			{
				szUnit.Format(_T("%s%s%s"),_T("["),getModel()->GetLanguageString(IDS_UNIT_CMH2O),_T("]"));
			}
		}
		break;
	case TREND_FIO2:
		{
			szUnit.Format(_T("%s%s%s"),_T("["),getModel()->GetLanguageString(IDS_UNIT_PERCENT),_T("]"));
		}
		break;
	case TREND_VTE:
		{
			szUnit.Format(_T("%s%s%s"),_T("["),getModel()->GetLanguageString(IDS_UNIT_MILLILITER),_T("]"));
		}
		break;
	case TREND_COMPLIANCE:
		{
			if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
			{
				szUnit.Format(_T("%s%s%s"),_T("["),getModel()->GetLanguageString(IDS_UNIT_MLMBAR),_T("]"));
			}
			else
			{
				szUnit.Format(_T("%s%s%s"),_T("["),getModel()->GetLanguageString(IDS_UNIT_MLCMH2O),_T("]"));
			}
			
		}
		break;
	case TREND_CO2HFO:
		{
			szUnit.Format(_T("%s%s%s"),_T("["),_T("ml^2/sec"),_T("]"));
		}
		break;
	case TREND_MV:
		{
			if(getModel()->GetLanguageID()==LAN_CHINESE)
			{
				szUnit.Format(_T("%s%s%s"),_T("["),getModel()->GetLanguageString(IDS_UNIT_LMIN),_T("]"));
			}
			else
			{
				szUnit.Format(_T("%s%s%s"),_T("["),getModel()->GetLanguageString(IDS_UNIT_LITER),_T("]"));
			}
		}
		break;
	case TREND_HFAMP:
		{
			if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
			{
				szUnit.Format(_T("%s%s%s"),_T("["),getModel()->GetLanguageString(IDS_UNIT_MBAR),_T("]"));
			}
			else
			{
				szUnit.Format(_T("%s%s%s"),_T("["),getModel()->GetLanguageString(IDS_UNIT_CMH2O),_T("]"));
			}
		}
		break;
	case TREND_RSBI:
		{
			szUnit.Format(_T("%s%s%s"),_T("["),getModel()->GetLanguageString(IDS_UNIT_LPM),_T("]"));
		}
		break;
	case TREND_SHAREMVMAND:
		{
			szUnit=_T("");
			//szUnit.Format(_T("%s%s%s"),_T("["),getModel()->GetLanguageString(IDS_UNIT_LITER),_T("]"));
		}
		break;
	case TREND_RESISTANCE:
		{
			if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
			{
				szUnit=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MBARLPS)+_T("]");
			}
			else
			{
				szUnit=_T("[")+getModel()->GetLanguageString(IDS_UNIT_CMH2OLPS)+_T("]");
			}
		}
		break;
	case TREND_LEAK:
		{
			szUnit.Format(_T("%s%s%s"),_T("["),getModel()->GetLanguageString(IDS_UNIT_PERCENT),_T("]"));
		}
		break;
	case TREND_SPO2:
		{
			szUnit=_T("[")+getModel()->GetLanguageString(IDS_UNIT_PERCENT)+_T("]");
		}
		break;
	case TREND_SPO2PR:
		{
			szUnit=_T("[")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]");
		}
		break;
	case TREND_SPO2PI:
		{
			szUnit=_T("");
		}
		break;
	case TREND_ETCO2:
		{
			eCO2unit eUnit=getModel()->getCONFIG()->GetCO2unit();
			switch(eUnit)
			{
			case CO2_UNIT_KPA:
				{
					szUnit =_T(" [")+getModel()->GetLanguageString(IDS_UNIT_KPA)+_T("]");
				}
				break;
			case CO2_UNIT_PERCENT:
				{
					szUnit =_T(" [")+getModel()->GetLanguageString(IDS_UNIT_VOLPERCENT)+_T("]");
				}
				break;
			case CO2_UNIT_MMHG:
			default:
				{
					szUnit =_T(" [")+getModel()->GetLanguageString(IDS_UNIT_MMHG)+_T("]");
				}
				break;
			}
		}
		break;
	case TREND_FREQUENCY:
		{
			szUnit=_T("[")+getModel()->GetLanguageString(IDS_UNIT_BPM)+_T("]");
		}
		break;
	}

	return szUnit;
}

/**********************************************************************************************//**
 * Shows
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bRedraw	True to redraw.
 **************************************************************************************************/

void CViewTrend::Show(bool bRedraw)
{
	DEBUGMSG(TRUE, (TEXT("Show\r\n")));
	if(m_bExit)
		return;

	this->ShowWindow(SW_SHOW);

	if(m_pcSelectTrend1)
		m_pcSelectTrend1->ShowWindow(SW_SHOW);
	if(m_pcSelectTrend2)
		m_pcSelectTrend2->ShowWindow(SW_SHOW);
	if(m_pcSelectTrend3)
		m_pcSelectTrend3->ShowWindow(SW_SHOW);

	ShowWndMenuTrend(true);
	ShowWndTrendTimeaxis(true);

	UpdateWindow();

	ShowTrend1Diagram(true);
	ShowTrend2Diagram(true);
	ShowTrend3Diagram(true);
		
	if(!m_bExit && bRedraw)
	{
		PostMessage(WM_LOAD_TREND);
	}
}

/**********************************************************************************************//**
 * Hides this instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewTrend::Hide()
{
	ShowWndMenuTrend(false);
	ShowWndTrendTimeaxis(false);
	ShowTrend1Diagram(false);
	ShowTrend2Diagram(false);
	ShowTrend3Diagram(false);

	this->ShowWindow(SW_HIDE);
}

/**********************************************************************************************//**
 * Destroys the window trend timeaxis
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewTrend::DestroyWndTrendTimeaxis()
{
	if(m_pWndTrendTimeaxis)
	{
		m_pWndTrendTimeaxis->DestroyWindow();
		delete m_pWndTrendTimeaxis;
		m_pWndTrendTimeaxis=NULL;
	}

	return true;
}

/**********************************************************************************************//**
 * Creates window trend timeaxis
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewTrend::CreateWndTrendTimeaxis()
{
	DEBUGMSG(TRUE, (TEXT("CreateWndTrendTimeaxis\r\n")));
	// **********************************************************************
	// Init. line diagram
	if(m_pWndTrendTimeaxis==NULL && m_lX>-1)
	{
		m_pWndTrendTimeaxis = new CWndTrendTimeaxis(this);
		RECT rcLd={5,450,585,492};

		if(m_pWndTrendTimeaxis->Create(this,rcLd,IDC_WND_TRENDAXIS))
		{
			m_pWndTrendTimeaxis->Init();

			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window trend timeaxis
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CViewTrend::ShowWndTrendTimeaxis(bool bShow)
{
	if(m_pWndTrendTimeaxis)
	{
		m_pWndTrendTimeaxis->Show(bShow);
	}
}

/**********************************************************************************************//**
 * Destroys the window menu trend
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewTrend::DestroyWndMenuTrend()
{
	if(m_pWndMenuTrend)
	{
		m_pWndMenuTrend->DestroyWindow();
		delete m_pWndMenuTrend;
		m_pWndMenuTrend=NULL;
	}

	return true;
}

/**********************************************************************************************//**
 * Creates window menu trend
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewTrend::CreateWndMenuTrend()
{
	// **********************************************************************
	// Init. line diagram
	if(m_pWndMenuTrend==NULL && m_lX>-1)
	{
		m_pWndMenuTrend = new CWndMenuTrend(this);
		RECT rcLd={0,550,600,600};

		if(m_pWndMenuTrend->Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_TRENDMENU))
		{
			m_pWndMenuTrend->Init();
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window menu trend
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CViewTrend::ShowWndMenuTrend(bool bShow)
{
	DEBUGMSG(TRUE, (TEXT("ShowWndMenuTrend\r\n")));
	if(m_pWndMenuTrend)
	{
		m_pWndMenuTrend->Show(bShow);
	}
}

/**********************************************************************************************//**
 * Destroys the window menu trendtype
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewTrend::DestroyWndMenuTrendtype()
{
	if(m_pWndMenuTrendtype)
	{
		try
		{
			m_pWndMenuTrendtype->DestroyWindow();
			delete m_pWndMenuTrendtype;
			m_pWndMenuTrendtype=NULL;

			m_bWndMenuTrendtype=false;
		}
		catch (...)
		{
			theApp.ReportException(_T("CViewTrend::DestroyWndMenuTrendtype"));
		}
	}

	return true;
}

/**********************************************************************************************//**
 * Creates window menu trendtype
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bSelTrend	The selected trend.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewTrend::CreateWndMenuTrendtype(UINT bSelTrend)
{
	if(m_pWndMenuTrendtype==NULL && m_lX>-1)
	{
		m_pWndMenuTrendtype = new CWndMenuTrendtype(this);
		
		//RECT rcLd={38,49,350,490};
		RECT rcLd={38,49,501,490};

		if(m_pWndMenuTrendtype->Create(this,rcLd,IDC_POPUP_TRENDTYPE,bSelTrend))
		{
			m_pWndMenuTrendtype->Init();

			m_bWndMenuTrendtype=true;
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window menu trendtype
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bShow	  	True to show, false to hide.
 * \param	iTrendType	Type of the trend.
 * \param	bSelTrend 	The selected trend.
 **************************************************************************************************/

void CViewTrend::ShowWndMenuTrendtype(bool bShow, UINT iTrendType, UINT bSelTrend)
{
	if(m_pWndMenuTrendtype)
	{
		m_pWndMenuTrendtype->Show(bShow,iTrendType,bSelTrend);
	}
}

/**********************************************************************************************//**
 * Deletes the trend 1 diagram
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewTrend::DeleteTrend1Diagram(void)
{
	DEBUGMSG(TRUE, (TEXT("DeleteTrend1Diagram\r\n")));
	if(m_pcTrend1Diagram)
	{
		m_pcTrend1Diagram->DestroyWindow();
		delete m_pcTrend1Diagram;
		m_pcTrend1Diagram=NULL;
	}
}

/**********************************************************************************************//**
 * Shows the trend 1 diagram
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CViewTrend::ShowTrend1Diagram(bool bShow)
{
	DEBUGMSG(TRUE, (TEXT("ShowTrend1Diagram\r\n")));
	if(m_pcTrend1Diagram)
	{
		m_pcTrend1Diagram->Show(bShow);
	}
}

/**********************************************************************************************//**
 * Deletes the trend 2 diagram
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewTrend::DeleteTrend2Diagram(void)
{
	DEBUGMSG(TRUE, (TEXT("DeleteTrend2Diagram\r\n")));
	if(m_pcTrend2Diagram)
	{
		m_pcTrend2Diagram->DestroyWindow();
		delete m_pcTrend2Diagram;
		m_pcTrend2Diagram=NULL;
	}
}

/**********************************************************************************************//**
 * Shows the trend 2 diagram
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CViewTrend::ShowTrend2Diagram(bool bShow)
{
	DEBUGMSG(TRUE, (TEXT("ShowTrend2Diagram\r\n")));
	if(m_pcTrend2Diagram)
	{
		m_pcTrend2Diagram->Show(bShow);
	}
}

/**********************************************************************************************//**
 * Deletes the trend 3 diagram
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewTrend::DeleteTrend3Diagram(void)
{
	DEBUGMSG(TRUE, (TEXT("DeleteTrend3Diagram\r\n")));
	if(m_pcTrend3Diagram)
	{
		m_pcTrend3Diagram->DestroyWindow();
		delete m_pcTrend3Diagram;
		m_pcTrend3Diagram=NULL;
	}
}

/**********************************************************************************************//**
 * Shows the trend 3 diagram
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CViewTrend::ShowTrend3Diagram(bool bShow)
{
	DEBUGMSG(TRUE, (TEXT("ShowTrend3Diagram\r\n")));
	if(m_pcTrend3Diagram)
	{
		m_pcTrend3Diagram->Show(bShow);
	}
}

/**********************************************************************************************//**
 * Notifies an event
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param [in,out]	pEvent	If non-null, the event.
 **************************************************************************************************/

void CViewTrend::NotifyEvent(CMVEvent* pEvent)
{
	if(m_bLoadDataRunning)
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
		return;
	}

	switch(pEvent->GetET())
	{
	case CMVEvent::ET_MATRIX:
		{
			CMVEventMatrix* pUIEvent = (CMVEventMatrix*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventMatrix::EV_BN_MATRIX_TREND:
				{
					if(getModel()->getCONFIG()->TrendIsAutoScale()==false)
						PostMessage( WM_TREND_SETCURSOR, 0, 0);

				}
				break;
			case CMVEventMatrix::EV_BN_MATRIX_CURSOR_LEFT:
				{
				}
				break;
			case CMVEventMatrix::EV_BN_MATRIX_CURSOR_RIGHT:
				{
				}
				break;
			case CMVEventMatrix::EV_BN_MATRIX_CURSOR_UP:
				{
					if(m_bGraphSelected /*&& getModel()->getCONFIG()->TrendIsAutoScale()==false*/)
					{
						switch(m_iCurFocusedWnd)
						{
						case IDC_TREND1_DIAGRAM:
							{
								EnterCriticalSection(&csTrends);
								if(m_pcTrend1Diagram)
								{
									if(m_pcTrend1Diagram->DecreaseYScaleToNextValue())
									{
										int iOldCursorPos=m_pcTrend1Diagram->GetTrendCursorPos();
										int iOldBufferPos=m_pcTrend1Diagram->GetTrendBufferXPos();
										DrawTrend1();
										m_pcTrend1Diagram->SetTrendCursorPos(iOldCursorPos,iOldBufferPos);
									}
									else
									{
										if(AfxGetApp())
											AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
									}
								}
								LeaveCriticalSection(&csTrends);

							}
							break;
						case IDC_TREND2_DIAGRAM:
							{
								EnterCriticalSection(&csTrends);
								if(m_pcTrend2Diagram)
								{
									if(m_pcTrend2Diagram->DecreaseYScaleToNextValue())
									{
										int iOldCursorPos=m_pcTrend2Diagram->GetTrendCursorPos();
										int iOldBufferPos=m_pcTrend2Diagram->GetTrendBufferXPos();
										DrawTrend2();
										m_pcTrend2Diagram->SetTrendCursorPos(iOldCursorPos,iOldBufferPos);
									}
									else
									{
										if(AfxGetApp())
											AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
									}
								}
								LeaveCriticalSection(&csTrends);
							}
							break;
						case IDC_TREND3_DIAGRAM:
							{
								EnterCriticalSection(&csTrends);
								if(m_pcTrend3Diagram)
								{
									if(m_pcTrend3Diagram->DecreaseYScaleToNextValue())
									{
										int iOldCursorPos=m_pcTrend3Diagram->GetTrendCursorPos();
										int iOldBufferPos=m_pcTrend3Diagram->GetTrendBufferXPos();
										DrawTrend3();
										m_pcTrend3Diagram->SetTrendCursorPos(iOldCursorPos,iOldBufferPos);
									}
									else
									{
										if(AfxGetApp())
											AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
									}
								}
								LeaveCriticalSection(&csTrends);
							}
							break;
						default:
							{

							}
							break;
						}
					}
				}
				break;
			case CMVEventMatrix::EV_BN_MATRIX_CURSOR_DOWN:
				{
					if(m_bGraphSelected /*&& getModel()->getCONFIG()->TrendIsAutoScale()==false*/)
					{
						switch(m_iCurFocusedWnd)
						{
						case IDC_TREND1_DIAGRAM:
							{
								EnterCriticalSection(&csTrends);
								if(m_pcTrend1Diagram)
								{
									if(m_pcTrend1Diagram->IncreaseYScaleToNextValue())
									{
										int iOldCursorPos=m_pcTrend1Diagram->GetTrendCursorPos();
										int iOldBufferPos=m_pcTrend1Diagram->GetTrendBufferXPos();
										DrawTrend1();
										m_pcTrend1Diagram->SetTrendCursorPos(iOldCursorPos,iOldBufferPos);
									}
									else
									{
										if(AfxGetApp())
											AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
									}
								}
								LeaveCriticalSection(&csTrends);

							}
							break;
						case IDC_TREND2_DIAGRAM:
							{
								EnterCriticalSection(&csTrends);
								if(m_pcTrend2Diagram)
								{
									if(m_pcTrend2Diagram->IncreaseYScaleToNextValue())
									{
										int iOldCursorPos=m_pcTrend2Diagram->GetTrendCursorPos();
										int iOldBufferPos=m_pcTrend2Diagram->GetTrendBufferXPos();
										DrawTrend2();
										m_pcTrend2Diagram->SetTrendCursorPos(iOldCursorPos,iOldBufferPos);
									}
									else
									{
										if(AfxGetApp())
											AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
									}
								}
								LeaveCriticalSection(&csTrends);
								
							}
							break;
						case IDC_TREND3_DIAGRAM:
							{
								EnterCriticalSection(&csTrends);
								if(m_pcTrend3Diagram)
								{
									if(m_pcTrend3Diagram->IncreaseYScaleToNextValue())
									{
										int iOldCursorPos=m_pcTrend3Diagram->GetTrendCursorPos();
										int iOldBufferPos=m_pcTrend3Diagram->GetTrendBufferXPos();
										DrawTrend3();
										m_pcTrend3Diagram->SetTrendCursorPos(iOldCursorPos,iOldBufferPos);
									}
									else
									{
										if(AfxGetApp())
											AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
									}
								}
								LeaveCriticalSection(&csTrends);
							}
							break;
						default:
							{

							}
							break;
						}
					}
				}
				break;
			}
		}
		break;
	case CMVEvent::ET_UIEVENT:
		{
			CMVEventUI* pUIEvent = (CMVEventUI*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventUI::EV_BN_TREND1:
				{
					SetAllButtonUnpressed();
					if(m_pWndMenuTrendtype)
						DestroyWndMenuTrendtype();

					PostMessage(WM_TREND_DECR_TIMEAXIS);
				}
				break;
			case CMVEventUI::EV_BN_TREND2:
				{
					SetAllButtonUnpressed();
					if(m_pWndMenuTrendtype)
						DestroyWndMenuTrendtype();

					PostMessage(WM_TREND_INCR_TIMEAXIS);
				}
				break;
			case CMVEventUI::EV_BN_TREND3:
				{
					SetAllButtonUnpressed();
					if(m_pWndMenuTrendtype)
						DestroyWndMenuTrendtype();

					PostMessage(WM_TREND_MOVELEFT);
				}
				break;
			case CMVEventUI::EV_BN_TREND4:
				{
					SetAllButtonUnpressed();
					if(m_pWndMenuTrendtype)
						DestroyWndMenuTrendtype();

					PostMessage(WM_TREND_MOVERIGHT);
				}
				break;
			case CMVEventUI::EV_BN_TREND5:
				{
					SetAllButtonUnpressed();
					if(m_pWndMenuTrendtype)
						DestroyWndMenuTrendtype();

					PostMessage(WM_LOAD_TREND);
				}
				break;
			case CMVEventUI::EV_TREND_SETCURSOR:
				{
					
				}
				break;
			case CMVEventUI::EV_TREND_DELCURSOR:
				{
					
				}
				break;
			}
		}
		break;
	}
}

/**********************************************************************************************//**
 * Creates trend 1 diagram
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	x	   	The x coordinate.
 * \param	y	   	The y coordinate.
 * \param	nWidth 	The width.
 * \param	nHeight	The height.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewTrend::CreateTrend1Diagram(int x, int y, int nWidth, int nHeight)
{
	bool result=false;

	if(m_pcTrend1Diagram==NULL)
	{
		m_pcTrend1Diagram=new CTrendDiagramm();
		//RECT rcLd={40,0,600,164};
		RECT rcLd={x,y,x+nWidth,y+nHeight};

		if(m_pcTrend1Diagram->Create(this,rcLd,IDC_TREND1_DIAGRAM,G_FabianHFOSCHEME_LINE))
		{
			m_pcTrend1Diagram->RefreshLocalTime(m_dtStartTime,m_dtOffsetTime);
			m_pcTrend1Diagram->SetTrendType(m_iTypeTrend1);
			int iYscale=GetMaxScaleOfTrendType(m_iTypeTrend1);
			m_pcTrend1Diagram->SetYAxisScale(m_pcTrend1Diagram->GetNegativeYoffset(iYscale), iYscale);
			m_pcTrend1Diagram->SetTrendSpan(m_iTrendSpan);
			m_pcTrend1Diagram->SetYLegendText(GetUnitOfTrendType(m_iTypeTrend1));
			m_pcTrend1Diagram->PaintGraph();
			m_pcTrend1Diagram->Show(true);

			m_pcTrend1Diagram->SetTrendCursorPos(0,0);

			//m_pWndTrendTimeaxis->SetOffsetTime(m_pcTrend1Diagram->GetOffsetTime());

			result=true;
		}
	}
	else
		m_pcTrend1Diagram->MoveWindow(x,y,nWidth,nHeight);

	return result;
}

/**********************************************************************************************//**
 * Creates trend 2 diagram
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	x	   	The x coordinate.
 * \param	y	   	The y coordinate.
 * \param	nWidth 	The width.
 * \param	nHeight	The height.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewTrend::CreateTrend2Diagram(int x, int y, int nWidth, int nHeight)
{
	bool result=false;
	if(m_pcTrend2Diagram==NULL)
	{
		m_pcTrend2Diagram=new CTrendDiagramm();
		//RECT rcLd={40,0,600,164};
		RECT rcLd={x,y,x+nWidth,y+nHeight};

		if(m_pcTrend2Diagram->Create(this,rcLd,IDC_TREND2_DIAGRAM,G_FabianHFOSCHEME_LINE))
		{
			m_pcTrend2Diagram->RefreshLocalTime(m_dtStartTime,m_dtOffsetTime);
			m_pcTrend2Diagram->SetTrendType(m_iTypeTrend2);
			int iYscale=GetMaxScaleOfTrendType(m_iTypeTrend2);
			m_pcTrend2Diagram->SetYAxisScale(m_pcTrend2Diagram->GetNegativeYoffset(iYscale), iYscale);
			m_pcTrend2Diagram->SetTrendSpan(m_iTrendSpan);
			m_pcTrend2Diagram->SetYLegendText(GetUnitOfTrendType(m_iTypeTrend2));
			m_pcTrend2Diagram->PaintGraph();
			m_pcTrend2Diagram->Show(true);

			m_pcTrend2Diagram->SetTrendCursorPos(0,0);

			result=true;
		}
	}
	else
		m_pcTrend2Diagram->MoveWindow(x,y,nWidth,nHeight);
	return result;
}

/**********************************************************************************************//**
 * Creates trend 3 diagram
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	x	   	The x coordinate.
 * \param	y	   	The y coordinate.
 * \param	nWidth 	The width.
 * \param	nHeight	The height.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewTrend::CreateTrend3Diagram(int x, int y, int nWidth, int nHeight)
{
	bool result=false;
	if(m_pcTrend3Diagram==NULL)
	{
		m_pcTrend3Diagram=new CTrendDiagramm();
		//RECT rcLd={40,0,600,164};
		RECT rcLd={x,y,x+nWidth,y+nHeight};

		if(m_pcTrend3Diagram->Create(this,rcLd,IDC_TREND3_DIAGRAM,G_FabianHFOSCHEME_LINE))
		{
			m_pcTrend3Diagram->RefreshLocalTime(m_dtStartTime,m_dtOffsetTime);
			m_pcTrend3Diagram->SetTrendType(m_iTypeTrend3);
			int iYscale=GetMaxScaleOfTrendType(m_iTypeTrend3);
			m_pcTrend3Diagram->SetYAxisScale(m_pcTrend3Diagram->GetNegativeYoffset(iYscale), iYscale);
			m_pcTrend3Diagram->SetTrendSpan(m_iTrendSpan);
			m_pcTrend3Diagram->SetYLegendText(GetUnitOfTrendType(m_iTypeTrend3));
			m_pcTrend3Diagram->PaintGraph();
			m_pcTrend3Diagram->Show(true);

			m_pcTrend3Diagram->SetTrendCursorPos(0,0);

			result=true;
		}
	}
	else
		m_pcTrend3Diagram->MoveWindow(x,y,nWidth,nHeight);
	return result;
}

/**********************************************************************************************//**
 * Executes the destroy action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewTrend::OnDestroy()
{
	m_bExit=true;
	
	m_plSelectTrendBtn.RemoveAll();

	DestroyWndHourglass();
	DeleteTrend1Diagram();
	DeleteTrend2Diagram();
	DeleteTrend3Diagram();

	DestroyWndMenuTrendtype();
	DestroyWndMenuTrend();
	DestroyWndTrendTimeaxis();

	CMVView::OnDestroy();

	if(m_hbmpStatic)
	{
		SelectObject(m_hdcStatic,m_hBmpPrevStatic);
		DeleteObject(m_hbmpStatic);
	}
	if(m_hdcStatic)
		DeleteDC(m_hdcStatic);
}

/**********************************************************************************************//**
 * Translates all messages before they are processed by the main message loop
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param [in,out]	pMsg	If non-null, the message.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

BOOL CViewTrend::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->SetFocus();
				return 1;
			}
			else if(pMsg->wParam==VK_UP)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->SetFocus();
				return 1;
			}
			else if(pMsg->wParam==VK_DOWN)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->SetFocus();
				return 1;
			}
			
			break;
		}
		break;
	default:
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}

/**********************************************************************************************//**
 * Executes the timer action
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	nIDEvent	The identifier event.
 **************************************************************************************************/

void CViewTrend::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==STARTTREND_TIMER)
	{
		DEBUGMSG(TRUE, (TEXT("CViewTrend::OnTimer\r\n")));
		KillTimer(STARTTREND_TIMER);

		PostMessage(WM_LOAD_TREND);
	}

	CWnd::OnTimer(nIDEvent);
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

LRESULT CViewTrend::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{

	switch(message)
	{
	case WM_SETVIEWFOCUS:
		{
			SetViewFocus();
			return 1;
		}
		break;
	case WM_SETNEXTFOCUS:
		{
			SetNextFocus();
			return 1;
		}
		break;
	case WM_SETPREVFOCUS:
		{
			SetPrevFocus();
			return 1;
		}
		break;
	case WM_TREND_CURSOR_RIGHT:
		{
			if(m_bLoadDataRunning)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			else
			{
				EnterCriticalSection(&csTrends);
				if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
				{
					m_pcTrend1Diagram->MoveTrendCursorRight();
					m_pcTrend2Diagram->MoveTrendCursorRight();
					m_pcTrend3Diagram->MoveTrendCursorRight();
				}
				LeaveCriticalSection(&csTrends);

			}
			return 1;
		}
		break;
	case WM_TREND_CURSOR_RIGHTS:
		{
			if(m_bLoadDataRunning)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			else
			{
				EnterCriticalSection(&csTrends);
				if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
				{
					m_pcTrend1Diagram->MoveTrendCursorRight(true);
					m_pcTrend2Diagram->MoveTrendCursorRight(true);
					m_pcTrend3Diagram->MoveTrendCursorRight(true);
					
				}
				LeaveCriticalSection(&csTrends);

			}
			return 1;
		}
		break;
	case WM_TREND_CURSOR_LEFT:
		{
			if(m_bLoadDataRunning)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			else
			{
				EnterCriticalSection(&csTrends);
				if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
				{
					m_pcTrend1Diagram->MoveTrendCursorLeft();
					m_pcTrend2Diagram->MoveTrendCursorLeft();
					m_pcTrend3Diagram->MoveTrendCursorLeft();
					
				}
				LeaveCriticalSection(&csTrends);

			}
			return 1;
		}
		break;
	case WM_TREND_CURSOR_LEFTS:
		{
			if(m_bLoadDataRunning)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			else
			{
				EnterCriticalSection(&csTrends);
				if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
				{
					m_pcTrend1Diagram->MoveTrendCursorLeft(true);
					m_pcTrend2Diagram->MoveTrendCursorLeft(true);
					m_pcTrend3Diagram->MoveTrendCursorLeft(true);
					
				}
				LeaveCriticalSection(&csTrends);

			}
			return 1;
		}
		break;
	case WM_TREND_DRAWDATA:
		{
			DEBUGMSG(TRUE, (TEXT("WM_TREND_DRAWDATA\r\n")));
			EnterCriticalSection(&csTrends);
			if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
			{
				DrawTrends();
				m_pcTrend1Diagram->SetTrendCursorPos(0,0);
				m_pcTrend2Diagram->SetTrendCursorPos(0,0);
				m_pcTrend3Diagram->SetTrendCursorPos(0,0);
				
			}
			LeaveCriticalSection(&csTrends);

			return 1;
		}
		break;
	case WM_MENUBUTTONDOWN:
		{
			
			switch(wParam)
			{
			case IDC_BTN_SELTREND_1:
				{
					if(m_bLoadDataRunning)
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
						SetAllButtonUnpressed();
					}
					else
					{
						EnterCriticalSection(&csTrends);
						if(m_iCurBtnSelTrend!=IDC_BTN_SELTREND_1)
						{
							SetOneButtonDepressed(IDC_BTN_SELTREND_1);

							CreateWndMenuTrendtype(m_iCurBtnSelTrend);
							ShowWndMenuTrendtype(true, m_iTypeTrend1,m_iCurBtnSelTrend);

						}
						else
						{
							DestroyWndMenuTrendtype();
							SetAllButtonUnpressed();

							DrawCursor(m_iCurFocusedWnd);

							UpdateWindow();
							PostMessage(WM_TREND_DRAWDATA);
						}
						LeaveCriticalSection(&csTrends);


					}
				}
				break;
			case IDC_BTN_SELTREND_2:
				{
					if(m_bLoadDataRunning)
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
						SetAllButtonUnpressed();
					}
					else
					{
						EnterCriticalSection(&csTrends);
						if(m_iCurBtnSelTrend!=IDC_BTN_SELTREND_2)
						{
							SetOneButtonDepressed(IDC_BTN_SELTREND_2);

							CreateWndMenuTrendtype(m_iCurBtnSelTrend);
							ShowWndMenuTrendtype(true, m_iTypeTrend2,m_iCurBtnSelTrend);

						}
						else
						{
							DestroyWndMenuTrendtype();
							SetAllButtonUnpressed();

							DrawCursor(m_iCurFocusedWnd);

							UpdateWindow();
							PostMessage(WM_TREND_DRAWDATA);
						}
						LeaveCriticalSection(&csTrends);

					}
				}
				break;
			case IDC_BTN_SELTREND_3:
				{
					if(m_bLoadDataRunning)
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
						SetAllButtonUnpressed();
					}
					else
					{
						EnterCriticalSection(&csTrends);
						if(m_iCurBtnSelTrend!=IDC_BTN_SELTREND_3)
						{
							SetOneButtonDepressed(IDC_BTN_SELTREND_3);

							CreateWndMenuTrendtype(m_iCurBtnSelTrend);
							ShowWndMenuTrendtype(true, m_iTypeTrend3,m_iCurBtnSelTrend);

						}
						else
						{
							DestroyWndMenuTrendtype();
							SetAllButtonUnpressed();

							DrawCursor(m_iCurFocusedWnd);

							UpdateWindow();
							PostMessage(WM_TREND_DRAWDATA);
						}
						LeaveCriticalSection(&csTrends);

					}
				}
				break;
			default:
				{
				}
				break;
			}
			return 1;
		}
		break;
	case WM_LOAD_TREND:
		{
			DEBUGMSG(TRUE, (TEXT("WM_LOAD_TREND\r\n")));
			
			UpdateWindow();
			getModel()->getDATAHANDLER()->LoadingTrendData(true);

			m_pcSelectTrend1->LoadDataRunning(true);
			m_pcSelectTrend2->LoadDataRunning(true);
			m_pcSelectTrend3->LoadDataRunning(true);
			m_bLoadDataRunning=true;

			if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram && !m_bExit)
			{
				CreateWndHourglass();
				ShowWndHourglass(true);

				if(!m_bExit)
					getModel()->getDATAHANDLER()->SerializeAllTrends(false);

				loadAllTrendData();

				getModel()->getDATAHANDLER()->LoadingTrendData(false);

				m_bLoadDataRunning=false;

				if(m_pcSelectTrend1)
					m_pcSelectTrend1->LoadDataRunning(false);
				if(m_pcSelectTrend2)
					m_pcSelectTrend2->LoadDataRunning(false);
				if(m_pcSelectTrend3)
					m_pcSelectTrend3->LoadDataRunning(false);

				ShowWndHourglass(false);
				UpdateWindow();

				PostMessage(WM_START_TREND);
			}
			return 1;
		}
		break;
	case WM_START_TREND:
		{
			DEBUGMSG(TRUE, (TEXT("WM_START_TREND\r\n")));
			if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram && !m_bExit)
			{
				EnterCriticalSection(&csTrends);

				SYSTEMTIME st;
				GetLocalTime(&st);
				m_dtOffsetTime=COleDateTime(st);
				m_dtStartTime=COleDateTime(st);

				m_pcTrend1Diagram->RefreshLocalTime(m_dtStartTime,m_dtOffsetTime);
				m_pcTrend2Diagram->RefreshLocalTime(m_dtStartTime,m_dtOffsetTime);
				m_pcTrend3Diagram->RefreshLocalTime(m_dtStartTime,m_dtOffsetTime);

				if(!m_bExit)
				{
					newTrendType1();
					DrawTrend1();
					m_pcTrend1Diagram->SetTrendCursorPos(0,0);
				}
				
				if(!m_bExit)
				{
					newTrendType2();
					DrawTrend2();
					m_pcTrend2Diagram->SetTrendCursorPos(0,0);
				}
				
				if(!m_bExit)
				{
					newTrendType3();
					DrawTrend3();
					m_pcTrend3Diagram->SetTrendCursorPos(0,0);
				}
				
				if(m_pWndTrendTimeaxis!=NULL && m_pcTrend1Diagram!=NULL)
				{
					COleDateTime tmStartTime=m_pcTrend1Diagram->GetStartTime();
					COleDateTimeSpan tsMax;
					tsMax.SetDateTimeSpan(0, 0, TREND_5DAYS, 0);
					COleDateTime tmEndTime=tmStartTime-tsMax;

					m_pWndTrendTimeaxis->SetTimeRange(tmStartTime,tmEndTime,m_pcTrend1Diagram->GetOffsetMinutes(), m_pcTrend1Diagram->GetTrendSpan());
				}

				LeaveCriticalSection(&csTrends);
			}
			return 1;
		}
		break;
	case WM_REDRAW_DIAGRAMM:
		{
			DEBUGMSG(TRUE, (TEXT("WM_REDRAW_DIAGRAMM\r\n")));
			EnterCriticalSection(&csTrends);

			switch(wParam)
			{
			case IDC_TREND1_DIAGRAM:
				{
					DEBUGMSG(TRUE, (TEXT("WM_REDRAW_DIAGRAMM IDC_TREND1_DIAGRAM\r\n")));
					if(m_pcTrend1Diagram)
					{
						int iPrev=m_pcTrend1Diagram->GetTrendBufferXPos();
						DrawTrend1();
						m_pcTrend1Diagram->SetTrendCursorPos(m_pcTrend1Diagram->GetTrendCursorPos(),iPrev);//newtrend
					}
					
				}
				break;
			case IDC_TREND2_DIAGRAM:
				{
					DEBUGMSG(TRUE, (TEXT("WM_REDRAW_DIAGRAMM IDC_TREND2_DIAGRAM\r\n")));
					if(m_pcTrend2Diagram)
					{
						int iPrev=m_pcTrend2Diagram->GetTrendBufferXPos();
						DrawTrend2();
						m_pcTrend2Diagram->SetTrendCursorPos(m_pcTrend2Diagram->GetTrendCursorPos(),iPrev);
					}
					
				}
				break;
			case IDC_TREND3_DIAGRAM:
				{
					DEBUGMSG(TRUE, (TEXT("WM_REDRAW_DIAGRAMM IDC_TREND3_DIAGRAM\r\n")));
					if(m_pcTrend3Diagram)
					{
						int iPrev=m_pcTrend3Diagram->GetTrendBufferXPos();
						DrawTrend3();
						m_pcTrend3Diagram->SetTrendCursorPos(m_pcTrend3Diagram->GetTrendCursorPos(),iPrev);
					}
				}
				break;
			}
			LeaveCriticalSection(&csTrends);

			return 1;
		}
		break;
	case WM_TREND_POPUP_PINSP:
		{
			newTrendType(TREND_PINSP);
			return 1;
		}
		break;
	case WM_TREND_POPUP_PMEAN:
		{
			newTrendType(TREND_PMEAN);
			return 1;
		}
		break;
	case WM_TREND_POPUP_FIO2:
		{
			newTrendType(TREND_FIO2);
			return 1;
		}
		break;
	case WM_TREND_POPUP_VTE:
		{
			newTrendType(TREND_VTE);
			return 1;
		}
		break;
	case WM_TREND_POPUP_COMPLIANCE:
		{
			newTrendType(TREND_COMPLIANCE);
			return 1;
		}
		break;
	case WM_TREND_POPUP_CO2HFO:
		{
			newTrendType(TREND_CO2HFO);
			return 1;
		}
		break;
	case WM_TREND_POPUP_MV:
		{
			newTrendType(TREND_MV);
			return 1;
		}
		break;
	case WM_TREND_POPUP_HFAMP:
		{
			newTrendType(TREND_HFAMP);
			return 1;
		}
		break;
	case WM_TREND_POPUP_RSBI:
		{
			newTrendType(TREND_RSBI);
			return 1;
		}
		break;
	case WM_TREND_POPUP_SHAREMVMAND:
		{
			newTrendType(TREND_SHAREMVMAND);
			return 1;
		}
		break;
	case WM_TREND_POPUP_RESISTANCE:
		{
			newTrendType(TREND_RESISTANCE);
			return 1;
		}
		break;
	case WM_TREND_POPUP_LEAK:
		{
			newTrendType(TREND_LEAK);
			return 1;
		}
		break;
	case WM_TREND_POPUP_SPO2:
		{
			newTrendType(TREND_SPO2);
			return 1;
		}
		break;
	case WM_TREND_POPUP_SPO2PR:
		{
			newTrendType(TREND_SPO2PR);
			return 1;
		}
		break;
	case WM_TREND_POPUP_SPO2PI:
		{
			newTrendType(TREND_SPO2PI);
			return 1;
		}
		break;
	case WM_TREND_POPUP_ETCO2:
		{
			newTrendType(TREND_ETCO2);
			return 1;
		}
		break;
	case WM_TREND_POPUP_FREQUENCY:
		{
			newTrendType(TREND_FREQUENCY);
			return 1;
		}
		break;
	case WM_TREND_SETCURSOR:
		{
			if(m_iCurFocusedWnd==wParam)
			{
				switch(wParam)
				{
				case IDC_TREND1_DIAGRAM:
					{
						DrawCursor(IDC_TREND2_DIAGRAM);
					}
					break;
				case IDC_TREND2_DIAGRAM:
					{
						DrawCursor(IDC_TREND3_DIAGRAM);
					}
					break;
				case IDC_TREND3_DIAGRAM:
					{
						DrawCursor(IDC_TREND1_DIAGRAM);
					}
					break;
				default:
					{
						DrawCursor(IDC_TREND1_DIAGRAM);
					}
					break;
				}
			}
			else
			{
				switch(m_iCurFocusedWnd)
				{
				case IDC_TREND1_DIAGRAM:
					{
						DrawCursor(IDC_TREND2_DIAGRAM);
					}
					break;
				case IDC_TREND2_DIAGRAM:
					{
						DrawCursor(IDC_TREND3_DIAGRAM);
					}
					break;
				case IDC_TREND3_DIAGRAM:
					{
						DrawCursor(IDC_TREND1_DIAGRAM);
					}
					break;
				default:
					{
						DrawCursor(IDC_TREND1_DIAGRAM);
					}
					break;
				}
			}
			return 1;
		}
		break;
	case WM_TREND_SETFOCUS:
		{
			if(m_bLoadDataRunning)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			else
			{
				switch(wParam)
				{
				case IDC_TREND1_DIAGRAM:
				case IDC_TREND2_DIAGRAM:
				case IDC_TREND3_DIAGRAM:
				default:
					{
						if(m_iCurFocusedWnd!=wParam)
						{
							DrawCursor(wParam);
						}
						else
						{
							DrawCursor(0);
						}
					}
					break;
				}
			}
			return 1;
		}
		break;
	case WM_TREND_LOSTFOCUS:
		{
			if(m_bGraphSelected)
			{
				DrawCursor(0);
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->SetFocus();
			}
			return 1;
		}
		break;
	case WM_TREND_MOVE:
		{
			if(m_bLoadDataRunning)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			else
			{
				EnterCriticalSection(&csTrends);
				int iPos=wParam;
				bool bEnd=false;

				if(m_pcTrend1Diagram)
				{
					if(m_pcTrend1Diagram->MoveToPos(iPos))
					{
						if(m_pcTrend2Diagram)
							m_pcTrend2Diagram->MoveToPos(iPos);
						if(m_pcTrend3Diagram)
							m_pcTrend3Diagram->MoveToPos(iPos);
					}
					else
						bEnd=true;
				}

				if(bEnd)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_SCALELIMIT);
				}
				else
				{
					if(m_pWndTrendTimeaxis!=NULL && m_pcTrend1Diagram!=NULL)
					{
						COleDateTime tmStartTime=m_pcTrend1Diagram->GetStartTime();
						COleDateTimeSpan tsMax;
						tsMax.SetDateTimeSpan(0, 0, TREND_5DAYS, 0);
						COleDateTime tmEndTime=tmStartTime-tsMax;

						m_pWndTrendTimeaxis->SetTimeRange(tmStartTime,tmEndTime,m_pcTrend1Diagram->GetOffsetMinutes(), m_pcTrend1Diagram->GetTrendSpan());
					}
					Sleep(0);
					if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
					{
						DrawTrends();
						m_pcTrend1Diagram->SetTrendCursorPos(0,0);
						m_pcTrend2Diagram->SetTrendCursorPos(0,0);
						m_pcTrend3Diagram->SetTrendCursorPos(0,0);
					}
				}
				LeaveCriticalSection(&csTrends);
				Sleep(0);
				if(m_iCurFocusedWnd==0)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->SetFocus();
				}
				else
					DrawCursor(m_iCurFocusedWnd);
			}
			return 1;
		}
		break;
	case WM_TREND_LEFTRANGE:
		{
			if(m_bLoadDataRunning)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			else
			{
				m_iCountLockedMoveLeft++;
				if(m_iCountLockedMoveLeft==3)
					PostMessage(WM_TREND_MOVELEFT);
			}
			return 1;
		}
		break;
	case WM_TREND_RIGHTRANGE:
		{
			if(m_bLoadDataRunning)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			else
			{
				m_iCountLockedMoveRight++;
				if(m_iCountLockedMoveRight==3)
					PostMessage(WM_TREND_MOVERIGHTRANGE);
			}
			return 1;
		}
		break;
	case WM_TREND_MOVELEFT:
		{
			if(m_bLoadDataRunning)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			else
			{
				EnterCriticalSection(&csTrends);


				m_iCountLockedMoveLeft=0;
				bool bEnd=false;

				if(wParam==0)
					wParam=1;

				if(m_pcTrend1Diagram)
				{
					if(m_pcTrend1Diagram->MoveLeft(wParam))
					{
						if(m_pcTrend2Diagram)
							m_pcTrend2Diagram->MoveLeft((UINT)wParam);
						if(m_pcTrend3Diagram)
							m_pcTrend3Diagram->MoveLeft((UINT)wParam);
					}
					else
						bEnd=true;
				}
				

				if(bEnd)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_SCALELIMIT);
				}
				else
				{
					if(m_pWndTrendTimeaxis!=NULL && m_pcTrend1Diagram!=NULL)
					{
						COleDateTime tmStartTime=m_pcTrend1Diagram->GetStartTime();
						COleDateTimeSpan tsMax;
						tsMax.SetDateTimeSpan(0, 0, TREND_5DAYS, 0);
						COleDateTime tmEndTime=tmStartTime-tsMax;

						m_pWndTrendTimeaxis->SetTimeRange(tmStartTime,tmEndTime,m_pcTrend1Diagram->GetOffsetMinutes(), m_pcTrend1Diagram->GetTrendSpan());
					}
					Sleep(0);
					if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
					{
						DrawTrends();
						m_pcTrend1Diagram->SetTrendCursorPos(0,0);
						m_pcTrend2Diagram->SetTrendCursorPos(0,0);
						m_pcTrend3Diagram->SetTrendCursorPos(0,0);
					}
					
					
				}
				LeaveCriticalSection(&csTrends);
				Sleep(0);
				if(m_iCurFocusedWnd==0)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->SetFocus();
				}
				else
					DrawCursor(m_iCurFocusedWnd);

			}
			return 1;
			
		}
		break;
	case WM_TREND_MOVERIGHT:
		{
			if(m_bLoadDataRunning)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			else
			{
				EnterCriticalSection(&csTrends);


				if(wParam==0)
					wParam=1;

				bool bEnd=false;
				m_iCountLockedMoveRight=0;

				if(m_pcTrend1Diagram)
				{
					if(m_pcTrend1Diagram->MoveRight((UINT)wParam))
					{
						if(m_pcTrend2Diagram)
							m_pcTrend2Diagram->MoveRight((UINT)wParam);
						
						if(m_pcTrend3Diagram)
							m_pcTrend3Diagram->MoveRight((UINT)wParam);
					}
					else
						bEnd=true;
				}

				if(bEnd)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_SCALELIMIT);
				}
				else
				{
					if(m_pWndTrendTimeaxis!=NULL && m_pcTrend1Diagram!=NULL)
					{
						COleDateTime tmStartTime=m_pcTrend1Diagram->GetStartTime();
						COleDateTimeSpan tsMax;
						tsMax.SetDateTimeSpan(0, 0, TREND_5DAYS, 0);
						COleDateTime tmEndTime=tmStartTime-tsMax;

						m_pWndTrendTimeaxis->SetTimeRange(tmStartTime,tmEndTime,m_pcTrend1Diagram->GetOffsetMinutes(), m_pcTrend1Diagram->GetTrendSpan());
					}
					Sleep(0);
					if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
					{
						DrawTrends();
						m_pcTrend1Diagram->SetTrendCursorPos(0,0);
						m_pcTrend2Diagram->SetTrendCursorPos(0,0);
						m_pcTrend3Diagram->SetTrendCursorPos(0,0);
					}
				}
				LeaveCriticalSection(&csTrends);
				Sleep(0);
				if(m_iCurFocusedWnd==0)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->SetFocus();
				}
				else
					DrawCursor(m_iCurFocusedWnd);

			}

			return 1;
		}
		break;
	case WM_TREND_MOVERIGHTRANGE:
		{
			if(m_bLoadDataRunning)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			else
			{
				EnterCriticalSection(&csTrends);
				bool bEnd=false;
				m_iCountLockedMoveRight=0;

				if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
				{
					if(m_pcTrend1Diagram->MoveRight(1))
					{
						if(m_pcTrend2Diagram)
							m_pcTrend2Diagram->MoveRight(1);

						if(m_pcTrend3Diagram)
							m_pcTrend3Diagram->MoveRight(1);
					}
					else
						bEnd=true;
				}

				if(bEnd)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_SCALELIMIT);
				}
				else
				{
					if(m_pWndTrendTimeaxis!=NULL && m_pcTrend1Diagram!=NULL)
					{
						COleDateTime tmStartTime=m_pcTrend1Diagram->GetStartTime();
						COleDateTimeSpan tsMax;
						tsMax.SetDateTimeSpan(0, 0, TREND_5DAYS, 0);
						COleDateTime tmEndTime=tmStartTime-tsMax;

						m_pWndTrendTimeaxis->SetTimeRange(tmStartTime,tmEndTime,m_pcTrend1Diagram->GetOffsetMinutes(), m_pcTrend1Diagram->GetTrendSpan());
					}
					Sleep(0);
					if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
					{
						DrawTrends();
						m_pcTrend1Diagram->SetTrendCursorPos(TREND_LEFTRANGE,0);
						m_pcTrend2Diagram->SetTrendCursorPos(TREND_LEFTRANGE,0);
						m_pcTrend3Diagram->SetTrendCursorPos(TREND_LEFTRANGE,0);
					}
				}
				LeaveCriticalSection(&csTrends);
				Sleep(0);
				if(m_iCurFocusedWnd==0)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->SetFocus();
				}
				else
					DrawCursor(m_iCurFocusedWnd);
			}
			return 1;
		}
		break;
	case WM_TREND_INCR_TIMEAXIS:
		{
			if(m_bLoadDataRunning)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			else
			{
				EnterCriticalSection(&csTrends);
				bool bEnd=false;
				
				if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
				{
					bEnd=IncreaseTimeSpan(); 
				}

				if(bEnd)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_SCALELIMIT);
				}
				else
				{
					if(m_pWndTrendTimeaxis!=NULL && m_pcTrend1Diagram!=NULL)
					{
						COleDateTime tmStartTime=m_pcTrend1Diagram->GetStartTime();
						COleDateTimeSpan tsMax;
						tsMax.SetDateTimeSpan(0, 0, TREND_5DAYS, 0);
						COleDateTime tmEndTime=tmStartTime-tsMax;

						m_pWndTrendTimeaxis->SetTimeRange(tmStartTime,tmEndTime,m_pcTrend1Diagram->GetOffsetMinutes(), m_pcTrend1Diagram->GetTrendSpan());
					}
					Sleep(0);
					if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
					{
						DrawTrends();
						m_pcTrend1Diagram->SetTrendCursorPos(0,0);
						m_pcTrend2Diagram->SetTrendCursorPos(0,0);
						m_pcTrend3Diagram->SetTrendCursorPos(0,0);
					}
				}
				LeaveCriticalSection(&csTrends);
				Sleep(0);
				if(m_iCurFocusedWnd==0)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->SetFocus();
				}
				else
					DrawCursor(m_iCurFocusedWnd);
			}
			return 1;
		}
		break;
	case WM_TREND_DECR_TIMEAXIS:
		{
			if(m_bLoadDataRunning)
			{
				if(AfxGetApp())
					AfxGetApp()->GetMainWnd()->PostMessage(WM_PIF_SIGNAL);
			}
			else
			{
				bool bEnd=false;

				EnterCriticalSection(&csTrends);
				if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
				{
					bEnd=DecreaseTimeSpan();
				}

				if(bEnd)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_TIMETEXT_SCALELIMIT);
				}
				else
				{
					if(m_pWndTrendTimeaxis!=NULL && m_pcTrend1Diagram!=NULL)
					{
						COleDateTime tmStartTime=m_pcTrend1Diagram->GetStartTime();
						COleDateTimeSpan tsMax;
						tsMax.SetDateTimeSpan(0, 0, TREND_5DAYS, 0);
						COleDateTime tmEndTime=tmStartTime-tsMax;

						m_pWndTrendTimeaxis->SetTimeRange(tmStartTime,tmEndTime,m_pcTrend1Diagram->GetOffsetMinutes(), m_pcTrend1Diagram->GetTrendSpan());
					}
					Sleep(0);
					if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
					{
						DrawTrends();
						m_pcTrend1Diagram->SetTrendCursorPos(0,0);
						m_pcTrend2Diagram->SetTrendCursorPos(0,0);
						m_pcTrend3Diagram->SetTrendCursorPos(0,0);
					}
					
				}
				LeaveCriticalSection(&csTrends);
				Sleep(0);
				if(m_iCurFocusedWnd==0)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->SetFocus();
				}
				else
					DrawCursor(m_iCurFocusedWnd);

			}
			return 1;
		}
		break;
	default:
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

/**********************************************************************************************//**
 * Determines if we can increase time span
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewTrend::IncreaseTimeSpan()
{
	bool bEnd=false;
	int iTrendSpan1 = m_pcTrend1Diagram->IncreaseTime();
	if(iTrendSpan1>0)
	{
		m_iTrendSpan = iTrendSpan1;
		
		m_pcTrend2Diagram->SetTrendSpan(iTrendSpan1);
		m_pcTrend2Diagram->ClearFunction();

		m_pcTrend3Diagram->SetTrendSpan(iTrendSpan1);
		m_pcTrend3Diagram->ClearFunction();

		getModel()->getCONFIG()->SetTrendSpan(m_iTrendSpan);
	}
	else
		bEnd=true;

	return bEnd;
}

/**********************************************************************************************//**
 * Determines if we can decrease time span
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewTrend::DecreaseTimeSpan()
{
	bool bEnd=false;
	int iTrendSpan1 = m_pcTrend1Diagram->DecreaseTime();
	if(iTrendSpan1>0)
	{
		m_iTrendSpan = iTrendSpan1;

		m_pcTrend2Diagram->SetTrendSpan(iTrendSpan1);
		m_pcTrend2Diagram->ClearFunction();

		m_pcTrend3Diagram->SetTrendSpan(iTrendSpan1);
		m_pcTrend3Diagram->ClearFunction();

		getModel()->getCONFIG()->SetTrendSpan(m_iTrendSpan);
	}
	else
		bEnd=true;

	return bEnd;
}

/**********************************************************************************************//**
 * Draw cursor
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	iTrend	Zero-based index of the trend.
 **************************************************************************************************/

void CViewTrend::DrawCursor(int iTrend)
{
	if(m_bExit)
		return;

	m_bGraphSelected=false;

	switch(iTrend)
	{
	case IDC_TREND1_DIAGRAM:
		{
			if(m_pcTrend2Diagram)
				m_pcTrend2Diagram->DrawCursor(false);
			if(m_pcTrend3Diagram)
				m_pcTrend3Diagram->DrawCursor(false);

			if(m_pcTrend1Diagram)
			{
				m_pcTrend1Diagram->DrawCursor(true);
				m_pcTrend1Diagram->SetFocus();
				m_bGraphSelected=true;
			}
		}
		break;
	case IDC_TREND2_DIAGRAM:
		{
			if(m_pcTrend1Diagram)
				m_pcTrend1Diagram->DrawCursor(false);
			if(m_pcTrend3Diagram)
				m_pcTrend3Diagram->DrawCursor(false);

			if(m_pcTrend2Diagram)
			{
				m_pcTrend2Diagram->DrawCursor(true);
				m_pcTrend2Diagram->SetFocus();
				m_bGraphSelected=true;
			}
		}
		break;
	case IDC_TREND3_DIAGRAM:
		{
			if(m_pcTrend1Diagram)
				m_pcTrend1Diagram->DrawCursor(false);
			if(m_pcTrend2Diagram)
				m_pcTrend2Diagram->DrawCursor(false);

			if(m_pcTrend3Diagram)
			{
				m_pcTrend3Diagram->DrawCursor(true);
				m_pcTrend3Diagram->SetFocus();
				m_bGraphSelected=true;
			}
		}
		break;
	case 0:
		{
			if(m_pcTrend1Diagram)
				m_pcTrend1Diagram->DrawCursor(false);
			if(m_pcTrend2Diagram)
				m_pcTrend2Diagram->DrawCursor(false);
			if(m_pcTrend3Diagram)
				m_pcTrend3Diagram->DrawCursor(false);

			m_bGraphSelected=false;
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->SetFocus();
		}
		break;
	default:
		{

		}
		break;
	}

	

	m_iCurFocusedWnd=iTrend;

	
}

/**********************************************************************************************//**
 * Sets next focus
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewTrend::SetNextFocus()
{
	return false;
}

/**********************************************************************************************//**
 * Sets previous focus
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewTrend::SetPrevFocus()
{
	return false;
}

/**********************************************************************************************//**
 * Sets view focus
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewTrend::SetViewFocus()
{
	this->SetFocus();
}

/**********************************************************************************************//**
 * Creates a new trend type 1
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewTrend::newTrendType1()
{
	if(m_bExit)
		return;
	if(m_pcTrend1Diagram)
	{
		m_pcTrend1Diagram->SetTrendType(m_iTypeTrend1);
		int iYscale=GetMaxScaleOfTrendType(m_iTypeTrend1);
		m_pcTrend1Diagram->SetYAxisScale(m_pcTrend1Diagram->GetNegativeYoffset(iYscale), iYscale);
		m_pcTrend1Diagram->SetTrendSpan(m_iTrendSpan);
		m_pcTrend1Diagram->SetYLegendText(GetUnitOfTrendType(m_iTypeTrend1));
		m_pcTrend1Diagram->PaintGraph();

		loadTrendData(m_iTypeTrend1, TREND1);
	}

	switch(m_iCurFocusedWnd)
	{
	case IDC_TREND1_DIAGRAM:
		{
			if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
			{
				m_pcTrend1Diagram->SetTrendCursorPos(0,0);
				m_pcTrend2Diagram->SetTrendCursorPos(0,0);
				m_pcTrend3Diagram->SetTrendCursorPos(0,0);
			}
		}
		break;
	case IDC_TREND2_DIAGRAM:
		{
			if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
			{
				m_pcTrend1Diagram->SetTrendCursorPos(m_pcTrend2Diagram->GetTrendCursorPos(),m_pcTrend2Diagram->GetTrendBufferXPos());
				m_pcTrend3Diagram->SetTrendCursorPos(m_pcTrend2Diagram->GetTrendCursorPos(),m_pcTrend2Diagram->GetTrendBufferXPos());
			}
		}
		break;
	case IDC_TREND3_DIAGRAM:
		{
			if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
			{
				m_pcTrend1Diagram->SetTrendCursorPos(m_pcTrend3Diagram->GetTrendCursorPos(),m_pcTrend3Diagram->GetTrendBufferXPos());
				m_pcTrend2Diagram->SetTrendCursorPos(m_pcTrend3Diagram->GetTrendCursorPos(),m_pcTrend3Diagram->GetTrendBufferXPos());
			}
		}
		break;
	default:
		{
			if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
			{
				m_pcTrend1Diagram->SetTrendCursorPos(0,0);
				m_pcTrend2Diagram->SetTrendCursorPos(0,0);
				m_pcTrend3Diagram->SetTrendCursorPos(0,0);
			}
		}
		break;
	}
}

/**********************************************************************************************//**
 * Creates a new trend type 2
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewTrend::newTrendType2()
{
	if(m_bExit)
		return;

	if(m_pcTrend2Diagram)
	{
		m_pcTrend2Diagram->SetTrendType(m_iTypeTrend2);
		int iYscale=GetMaxScaleOfTrendType(m_iTypeTrend2);
		m_pcTrend2Diagram->SetYAxisScale(m_pcTrend2Diagram->GetNegativeYoffset(iYscale), iYscale);
		m_pcTrend2Diagram->SetTrendSpan(m_iTrendSpan);
		m_pcTrend2Diagram->SetYLegendText(GetUnitOfTrendType(m_iTypeTrend2));
		m_pcTrend2Diagram->PaintGraph();

		loadTrendData(m_iTypeTrend2, TREND2);
	}

	switch(m_iCurFocusedWnd)
	{
	case IDC_TREND1_DIAGRAM:
		{
			if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
			{
				m_pcTrend2Diagram->SetTrendCursorPos(m_pcTrend1Diagram->GetTrendCursorPos(),m_pcTrend1Diagram->GetTrendBufferXPos());
				m_pcTrend3Diagram->SetTrendCursorPos(m_pcTrend1Diagram->GetTrendCursorPos(),m_pcTrend1Diagram->GetTrendBufferXPos());
			}
		}
		break;
	case IDC_TREND2_DIAGRAM:
		{
			if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
			{
				m_pcTrend1Diagram->SetTrendCursorPos(0,0);
				m_pcTrend2Diagram->SetTrendCursorPos(0,0);
				m_pcTrend3Diagram->SetTrendCursorPos(0,0);
			}
		}
		break;
	case IDC_TREND3_DIAGRAM:
		{
			if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
			{
				m_pcTrend1Diagram->SetTrendCursorPos(m_pcTrend3Diagram->GetTrendCursorPos(),m_pcTrend3Diagram->GetTrendBufferXPos());
				m_pcTrend2Diagram->SetTrendCursorPos(m_pcTrend3Diagram->GetTrendCursorPos(),m_pcTrend3Diagram->GetTrendBufferXPos());
			}
		}
		break;
	default:
		{
			if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
			{
				m_pcTrend1Diagram->SetTrendCursorPos(0,0);
				m_pcTrend2Diagram->SetTrendCursorPos(0,0);
				m_pcTrend3Diagram->SetTrendCursorPos(0,0);
			}
		}
		break;
	}

}

/**********************************************************************************************//**
 * Creates a new trend type 3
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewTrend::newTrendType3()
{
	if(m_bExit)
		return;

	if(m_pcTrend3Diagram)
	{
		m_pcTrend3Diagram->SetTrendType(m_iTypeTrend3);
		int iYscale=GetMaxScaleOfTrendType(m_iTypeTrend3);
		m_pcTrend3Diagram->SetYAxisScale(m_pcTrend3Diagram->GetNegativeYoffset(iYscale), iYscale);
		m_pcTrend3Diagram->SetTrendSpan(m_iTrendSpan);
		m_pcTrend3Diagram->SetYLegendText(GetUnitOfTrendType(m_iTypeTrend3));
		m_pcTrend3Diagram->PaintGraph();

		loadTrendData(m_iTypeTrend3, TREND3);
	}

	switch(m_iCurFocusedWnd)
	{
	case IDC_TREND1_DIAGRAM:
		{
			if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
			{
				m_pcTrend2Diagram->SetTrendCursorPos(m_pcTrend1Diagram->GetTrendCursorPos(),m_pcTrend1Diagram->GetTrendBufferXPos());
				m_pcTrend3Diagram->SetTrendCursorPos(m_pcTrend1Diagram->GetTrendCursorPos(),m_pcTrend1Diagram->GetTrendBufferXPos());
			}
		}
		break;
	case IDC_TREND2_DIAGRAM:
		{
			if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
			{
				m_pcTrend1Diagram->SetTrendCursorPos(m_pcTrend2Diagram->GetTrendCursorPos(),m_pcTrend2Diagram->GetTrendBufferXPos());
				m_pcTrend3Diagram->SetTrendCursorPos(m_pcTrend2Diagram->GetTrendCursorPos(),m_pcTrend2Diagram->GetTrendBufferXPos());
			}
		}
		break;
	case IDC_TREND3_DIAGRAM:
		{
			if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
			{
				m_pcTrend1Diagram->SetTrendCursorPos(0,0);
				m_pcTrend2Diagram->SetTrendCursorPos(0,0);
				m_pcTrend3Diagram->SetTrendCursorPos(0,0);
			}
		}
		break;
	default:
		{
			if(m_pcTrend1Diagram && m_pcTrend2Diagram && m_pcTrend3Diagram)
			{
				m_pcTrend1Diagram->SetTrendCursorPos(0,0);
				m_pcTrend2Diagram->SetTrendCursorPos(0,0);
				m_pcTrend3Diagram->SetTrendCursorPos(0,0);
			}
		}
		break;
	}
}

/**********************************************************************************************//**
 * Creates a new trend type
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	type	The type.
 **************************************************************************************************/

void CViewTrend::newTrendType(UINT type)
{
	CString szText=_T("");

	switch(type)
	{
	case TREND_PINSP:
		{
			szText=getModel()->GetLanguageString(IDS_TREND_PINSP);
		}
		break;
	case TREND_PMEAN:
		{
			szText=getModel()->GetLanguageString(IDS_TREND_PMEAN);
		}
		break;
	case TREND_FIO2:
		{
			szText=getModel()->GetLanguageString(IDS_TREND_FIO2);
		}
		break;
	case TREND_VTE:
		{
			szText=getModel()->GetLanguageString(IDS_TREND_VTE);
		}
		break;
	case TREND_COMPLIANCE:
		{
			szText=getModel()->GetLanguageString(IDS_TREND_COMPL);
		}
		break;
	case TREND_CO2HFO:
		{
			szText=getModel()->GetLanguageString(IDS_TREND_CO2);
		}
		break;
	case TREND_MV:
		{
			szText=getModel()->GetLanguageString(IDS_TREND_MV);
		}
		break;
	case TREND_HFAMP:
		{
			szText=getModel()->GetLanguageString(IDS_TREND_HFAMP);
		}
		break;
	case TREND_RSBI:
		{
			szText=_T("RSBI");
		}
		break;
	case TREND_SHAREMVMAND:
		{
			szText=_T("mand% MV");
		}
		break;
	case TREND_RESISTANCE:
		{
			szText=_T("RESISTANCE");
		}
		break;
	case TREND_LEAK:
		{
			szText=_T("LEAK");
		}
		break;
	case TREND_SPO2:
		{
			szText=_T("SPO2");
		}
		break;
	case TREND_SPO2PR:
		{
			szText=_T("SPO2 PR");
		}
		break;
	case TREND_SPO2PI:
		{
			szText=_T("SPO2 PI");
		}
		break;
	case TREND_ETCO2:
		{
			szText=_T("ETCO2");
		}
		break;
	case TREND_FREQUENCY:
		{
			szText=_T("FREQ");
		}
		break;
	}

	DestroyWndMenuTrendtype();
	UpdateWindow();

	DEBUGMSG(TRUE, (TEXT("newTrendType %s\r\n"),szText));

	EnterCriticalSection(&csTrends);
	switch(m_iCurBtnSelTrend)
	{
	case IDC_BTN_SELTREND_1:
		{
			DEBUGMSG(TRUE, (TEXT("IDC_BTN_SELTREND_1\r\n")));
			if(m_pcSelectTrend1)
			{
				m_pcSelectTrend1->RefreshText(szText,szText);

				m_iTypeTrend1=type;
				getModel()->getCONFIG()->SetTrendType1(m_iTypeTrend1);
			}
		}
		break;
	case IDC_BTN_SELTREND_2:
		{
			DEBUGMSG(TRUE, (TEXT("IDC_BTN_SELTREND_2\r\n")));
			if(m_pcSelectTrend2)
			{
				m_pcSelectTrend2->RefreshText(szText,szText);

				m_iTypeTrend2=type;
				getModel()->getCONFIG()->SetTrendType2(m_iTypeTrend2);
				
			}
		}
		break;
	case IDC_BTN_SELTREND_3:
		{
			DEBUGMSG(TRUE, (TEXT("IDC_BTN_SELTREND_3\r\n")));
			if(m_pcSelectTrend3)
			{
				m_pcSelectTrend3->RefreshText(szText,szText);

				m_iTypeTrend3=type;
				getModel()->getCONFIG()->SetTrendType3(m_iTypeTrend3);

			}
		}
		break;
	}

	DrawCursor(0);

	LeaveCriticalSection(&csTrends);

	SetAllButtonUnpressed();
	PostMessage(WM_LOAD_TREND);
}

/**********************************************************************************************//**
 * Draw trend 1
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewTrend::DrawTrend1()
{
	if(m_bExit)
		return;

	if(m_pBufferTrend1==NULL)
	{
		CFabianHFOApp::ReportException(_T("CViewTrend::DrawTrend1() m_pBufferTrend1==NULL"));
		return;
	}

	Sleep(0);

	if(m_pPlotItemsTrend1 !=NULL)
	{
		delete []m_pPlotItemsTrend1;
	}
	m_pPlotItemsTrend1=NULL;
	if(m_lpfsTrend1 !=NULL)
	{
		delete m_lpfsTrend1;
	}
	m_lpfsTrend1=NULL;


	int iValuesZumEinlesen=(m_iTrendSpan*VALUESPERMINUTE)+1;
	int iStartPoint=m_pcTrend1Diagram->GetOffsetMinutes()*VALUESPERMINUTE;

	//DEBUGMSG(TRUE, (TEXT("DrawTrend1 %d %d\r\n"),iValuesZumEinlesen,iStartPoint));

	UINT numitems=iValuesZumEinlesen;
	m_pPlotItemsTrend1=new G_PLOTITEMSSTRUCT_TD[numitems];


	bool bPrintEx=true;
	int i=0;
	try
	{
		for(i=0;i<iValuesZumEinlesen;i++)
		{
			if(i+iStartPoint>MAXSIZE_TREND_BUFFER)
			{
				if(bPrintEx)
				{
					CString error=_T("");
					error.Format(_T("CViewTrend::DrawTrend1a() %d %d"), iValuesZumEinlesen,iStartPoint);
					CFabianHFOApp::ReportException(error);

					bPrintEx=false;
				}
				numitems=i-1;
				break;
			}
			else
			{
				m_pPlotItemsTrend1[i].pPlotXYItems.x=i;
				m_pPlotItemsTrend1[i].pPlotXYItems.y=m_pBufferTrend1[i+iStartPoint];
			}
			Sleep(0);
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CViewTrend::DrawTrend1b"));
	}

	m_lpfsTrend1 =new G_FUNCTIONSTRUCT_TD;
	memset(m_lpfsTrend1,0,sizeof(G_FUNCTIONSTRUCT_TD));
	
	switch(m_iTypeTrend1)
	{
	case TREND_PINSP:
		{
			m_lpfsTrend1->yMax=G_UPPER_MAXSCALE_PRESSURE;
			m_lpfsTrend1->yMin=G_LOWER_MAXSCALE_PRESSURE;
		}
		break;
	case TREND_PMEAN:
		{
			m_lpfsTrend1->yMax=G_UPPER_MAXSCALE_PRESSURE*10;
			m_lpfsTrend1->yMin=G_LOWER_MAXSCALE_PRESSURE*10;
			
		}
		break;
	case TREND_FIO2:
		{
			m_lpfsTrend1->yMax=G_UPPER_MAXSCALE_FIO2;
			m_lpfsTrend1->yMin=G_LOWER_MAXSCALE_FIO2;
		}
		break;
	case TREND_VTE:
		{
			m_lpfsTrend1->yMax=G_UPPER_MAXSCALE_VTE*10;
			m_lpfsTrend1->yMin=G_LOWER_MAXSCALE_VTE*10;
		}
		break;
	case TREND_COMPLIANCE:
		{
			m_lpfsTrend1->yMax=G_UPPER_MAXSCALE_COMPL;
			m_lpfsTrend1->yMin=G_LOWER_MAXSCALE_COMPL;
		}
		break;
	case TREND_CO2HFO:
		{
			m_lpfsTrend1->yMax=G_UPPER_MAXSCALE_DCO2;
			m_lpfsTrend1->yMin=G_LOWER_MAXSCALE_DCO2;
		}
		break;
	case TREND_MV:
		{
			m_lpfsTrend1->yMax=G_UPPER_MAXSCALE_MV;
			m_lpfsTrend1->yMin=G_LOWER_MAXSCALE_MV;
		}
		break;
	case TREND_HFAMP:
		{
			m_lpfsTrend1->yMax=G_UPPER_MAXSCALE_HFAMP;
			m_lpfsTrend1->yMin=G_LOWER_MAXSCALE_HFAMP;
		}
		break;
	case TREND_RSBI:
		{
			m_lpfsTrend1->yMax=G_UPPER_MAXSCALE_RSBI;
			m_lpfsTrend1->yMin=G_LOWER_MAXSCALE_RSBI;
		}
		break;
	case TREND_SHAREMVMAND:
		{
			m_lpfsTrend1->yMax=G_UPPER_MAXSCALE_SHAREMVMAND;
			m_lpfsTrend1->yMin=G_LOWER_MAXSCALE_SHAREMVMAND;
		}
		break;
	case TREND_RESISTANCE:
		{
			m_lpfsTrend1->yMax=G_UPPER_MAXSCALE_RESISTANCE;
			m_lpfsTrend1->yMin=G_LOWER_MAXSCALE_RESISTANCE;
		}
		break;
	case TREND_LEAK:
		{
			m_lpfsTrend1->yMax=G_UPPER_MAXSCALE_LEAK;
			m_lpfsTrend1->yMin=G_LOWER_MAXSCALE_LEAK;
		}
		break;
	case TREND_SPO2:
		{
			m_lpfsTrend1->yMax=G_UPPER_MAXSCALE_SPO2;
			m_lpfsTrend1->yMin=G_LOWER_MAXSCALE_SPO2;
		}
		break;
	case TREND_SPO2PR:
		{
			m_lpfsTrend1->yMax=G_UPPER_MAXSCALE_SPO2PR;
			m_lpfsTrend1->yMin=G_LOWER_MAXSCALE_SPO2PR;
		}
		break;
	case TREND_SPO2PI:
		{
			m_lpfsTrend1->yMax=G_UPPER_MAXSCALE_SPO2PI;
			m_lpfsTrend1->yMin=G_LOWER_MAXSCALE_SPO2PI;
		}
		break;
	case TREND_ETCO2:
		{
			m_lpfsTrend1->yMax=G_UPPER_MAXSCALE_CO2;
			m_lpfsTrend1->yMin=G_LOWER_MAXSCALE_CO2;
		}
		break;
	case TREND_FREQUENCY:
		{
			m_lpfsTrend1->yMax=G_UPPER_MAXSCALE_FREQUENCY;
			m_lpfsTrend1->yMin=G_LOWER_MAXSCALE_FREQUENCY;
		}
		break;
	}
	m_lpfsTrend1->dYfactor=1;
	m_lpfsTrend1->dXfactor=1;
	m_lpfsTrend1->xMax=0;
	m_lpfsTrend1->xMin=0;
	m_lpfsTrend1->FuncType=G_PLOTXYPOINTS;
	if(m_bFilledGraph)
		m_lpfsTrend1->ChartType=G_FILLEDCHART;
	else
		m_lpfsTrend1->ChartType=G_LINECHART;
	m_lpfsTrend1->pPlotXYItems=m_pPlotItemsTrend1;
	m_lpfsTrend1->num_PlotXYItems=numitems;

	if(m_pcTrend1Diagram)
	{
		m_pcTrend1Diagram->DoFunction(m_lpfsTrend1);
	}

	//DEBUGMSG(TRUE, (TEXT("end DrawTrend1\r\n")));
}

/**********************************************************************************************//**
 * Draw trend 2
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewTrend::DrawTrend2()
{
	if(m_bExit)
		return;

	if(m_pBufferTrend2==NULL)
	{
		CFabianHFOApp::ReportException(_T("CViewTrend::DrawTrend2() m_pBufferTrend2==NULL"));
		return;
	}

	Sleep(0);

	if(m_pPlotItemsTrend2 !=NULL)
	{
		delete []m_pPlotItemsTrend2;
		m_pPlotItemsTrend2=NULL;
	}
	if(m_lpfsTrend2 !=NULL)
	{
		m_pcTrend2Diagram->ClearFunction();
		delete m_lpfsTrend2;
	}

	
	int iValuesZumEinlesen=(m_iTrendSpan*VALUESPERMINUTE)+1;
	int iStartPoint=m_pcTrend2Diagram->GetOffsetMinutes()*VALUESPERMINUTE;

	//DEBUGMSG(TRUE, (TEXT("DrawTrend2 %d %d\r\n"),iValuesZumEinlesen,iStartPoint));


	UINT numitems=iValuesZumEinlesen;
	m_pPlotItemsTrend2=new G_PLOTITEMSSTRUCT_TD[numitems];

	bool bPrintEx=true;

	try
	{
		for(int i=0;i<iValuesZumEinlesen;i++)
		{
			if(i+iStartPoint>MAXSIZE_TREND_BUFFER)
			{
				if(bPrintEx)
				{
					CString error=_T("");
					error.Format(_T("CViewTrend::DrawTrend2a() %d %d"), iValuesZumEinlesen,iStartPoint);
					CFabianHFOApp::ReportException(error);
					bPrintEx=false;
				}
				numitems=i-1;
				break;
			}
			else
			{
				m_pPlotItemsTrend2[i].pPlotXYItems.x=i;
				m_pPlotItemsTrend2[i].pPlotXYItems.y=m_pBufferTrend2[i+iStartPoint];
			}
			Sleep(0);
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CViewTrend::DrawTrend2b"));
	}


	//change the graph width + height
	//change the x,y position
	//use default width & height (set them to 0)
	m_lpfsTrend2 =new G_FUNCTIONSTRUCT_TD;
	memset(m_lpfsTrend2,0,sizeof(G_FUNCTIONSTRUCT_TD));

	
	switch(m_iTypeTrend2)
	{
	case TREND_PINSP:
		{
			m_lpfsTrend2->yMax=G_UPPER_MAXSCALE_PRESSURE;
			m_lpfsTrend2->yMin=G_LOWER_MAXSCALE_PRESSURE;
		}
		break;
	case TREND_PMEAN:
		{
			m_lpfsTrend2->yMax=G_UPPER_MAXSCALE_PRESSURE*10;
			m_lpfsTrend2->yMin=G_LOWER_MAXSCALE_PRESSURE*10;

		}
		break;
	case TREND_FIO2:
		{
			m_lpfsTrend2->yMax=G_UPPER_MAXSCALE_FIO2;
			m_lpfsTrend2->yMin=G_LOWER_MAXSCALE_FIO2;
		}
		break;
	case TREND_VTE:
		{
			m_lpfsTrend2->yMax=G_UPPER_MAXSCALE_VTE*10;
			m_lpfsTrend2->yMin=G_LOWER_MAXSCALE_VTE*10;
		}
		break;
	case TREND_COMPLIANCE:
		{
			m_lpfsTrend2->yMax=G_UPPER_MAXSCALE_COMPL;
			m_lpfsTrend2->yMin=G_LOWER_MAXSCALE_COMPL;
		}
		break;
	case TREND_CO2HFO:
		{
			m_lpfsTrend2->yMax=G_UPPER_MAXSCALE_DCO2;
			m_lpfsTrend2->yMin=G_LOWER_MAXSCALE_DCO2;
		}
		break;
	case TREND_MV:
		{
			m_lpfsTrend2->yMax=G_UPPER_MAXSCALE_MV;
			m_lpfsTrend2->yMin=G_LOWER_MAXSCALE_MV;
		}
		break;
	case TREND_HFAMP:
		{
			m_lpfsTrend2->yMax=G_UPPER_MAXSCALE_HFAMP;
			m_lpfsTrend2->yMin=G_LOWER_MAXSCALE_HFAMP;
		}
		break;
	case TREND_RSBI:
		{
			m_lpfsTrend2->yMax=G_UPPER_MAXSCALE_RSBI;
			m_lpfsTrend2->yMin=G_LOWER_MAXSCALE_RSBI;
		}
		break;
	case TREND_SHAREMVMAND:
		{
			m_lpfsTrend2->yMax=G_UPPER_MAXSCALE_SHAREMVMAND;
			m_lpfsTrend2->yMin=G_LOWER_MAXSCALE_SHAREMVMAND;
		}
		break;
	case TREND_RESISTANCE:
		{
			m_lpfsTrend2->yMax=G_UPPER_MAXSCALE_RESISTANCE;
			m_lpfsTrend2->yMin=G_LOWER_MAXSCALE_RESISTANCE;
		}
		break;
	case TREND_LEAK:
		{
			m_lpfsTrend2->yMax=G_UPPER_MAXSCALE_LEAK;
			m_lpfsTrend2->yMin=G_LOWER_MAXSCALE_LEAK;
		}
		break;
	case TREND_SPO2:
		{
			m_lpfsTrend2->yMax=G_UPPER_MAXSCALE_SPO2;
			m_lpfsTrend2->yMin=G_LOWER_MAXSCALE_SPO2;
		}
		break;
	case TREND_SPO2PR:
		{
			m_lpfsTrend2->yMax=G_UPPER_MAXSCALE_SPO2PR;
			m_lpfsTrend2->yMin=G_LOWER_MAXSCALE_SPO2PR;
		}
		break;
	case TREND_SPO2PI:
		{
			m_lpfsTrend2->yMax=G_UPPER_MAXSCALE_SPO2PI;
			m_lpfsTrend2->yMin=G_LOWER_MAXSCALE_SPO2PI;
		}
		break;
	case TREND_ETCO2:
		{
			m_lpfsTrend2->yMax=G_UPPER_MAXSCALE_CO2;
			m_lpfsTrend2->yMin=G_LOWER_MAXSCALE_CO2;
		}
		break;
	case TREND_FREQUENCY:
		{
			m_lpfsTrend2->yMax=G_UPPER_MAXSCALE_FREQUENCY;
			m_lpfsTrend2->yMin=G_LOWER_MAXSCALE_FREQUENCY;
		}
		break;
	}
	m_lpfsTrend2->dYfactor=1;
	m_lpfsTrend2->dXfactor=1;
	m_lpfsTrend2->xMax=0;
	m_lpfsTrend2->xMin=0;
	m_lpfsTrend2->FuncType=G_PLOTXYPOINTS;
	if(m_bFilledGraph)
		m_lpfsTrend2->ChartType=G_FILLEDCHART;
	else
		m_lpfsTrend2->ChartType=G_LINECHART;
	m_lpfsTrend2->pPlotXYItems=m_pPlotItemsTrend2;
	m_lpfsTrend2->num_PlotXYItems=numitems;

		
	if(m_pcTrend2Diagram)
	{
		m_pcTrend2Diagram->DoFunction(m_lpfsTrend2);
	}

	//DEBUGMSG(TRUE, (TEXT("end DrawTrend2\r\n")));
	
}

/**********************************************************************************************//**
 * Draw trend 3
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewTrend::DrawTrend3()
{
	if(m_bExit)
		return;

	if(m_pBufferTrend3==NULL)
	{
		CFabianHFOApp::ReportException(_T("CViewTrend::DrawTrend3() m_pBufferTrend3==NULL"));
		return;
	}

	Sleep(0);

	if(m_pPlotItemsTrend3 !=NULL)
	{
		delete []m_pPlotItemsTrend3;
		m_pPlotItemsTrend3=NULL;
	}
	if(m_lpfsTrend3 !=NULL)
	{
		m_pcTrend3Diagram->ClearFunction();
		delete m_lpfsTrend3;
	}

	int iValuesZumEinlesen=(m_iTrendSpan*VALUESPERMINUTE)+1;
	int iStartPoint=m_pcTrend3Diagram->GetOffsetMinutes()*VALUESPERMINUTE;

	//DEBUGMSG(TRUE, (TEXT("DrawTrend3 %d %d\r\n"),iValuesZumEinlesen,iStartPoint));


	UINT numitems=iValuesZumEinlesen;
	m_pPlotItemsTrend3=new G_PLOTITEMSSTRUCT_TD[numitems];

	bool bPrintEx=true;

	try
	{
		for(int i=0;i<iValuesZumEinlesen;i++)
		{
			if(i+iStartPoint>MAXSIZE_TREND_BUFFER)
			{
				if(bPrintEx)
				{
					CString error=_T("");
					error.Format(_T("CViewTrend::DrawTrend3a() %d %d"), iValuesZumEinlesen,iStartPoint);
					CFabianHFOApp::ReportException(error);
					bPrintEx=false;
				}
				numitems=i-1;
				break;
			}
			else
			{
				m_pPlotItemsTrend3[i].pPlotXYItems.x=i;
				m_pPlotItemsTrend3[i].pPlotXYItems.y=m_pBufferTrend3[i+iStartPoint];
			}
			Sleep(0);
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CViewTrend::DrawTrend3b"));
	}

	//change the graph width + height
	//change the x,y position
	//use default width & height (set them to 0)
	m_lpfsTrend3 =new G_FUNCTIONSTRUCT_TD;
	memset(m_lpfsTrend3,0,sizeof(G_FUNCTIONSTRUCT_TD));
	
	switch(m_iTypeTrend3)
	{
	case TREND_PINSP:
		{
			m_lpfsTrend3->yMax=G_UPPER_MAXSCALE_PRESSURE;
			m_lpfsTrend3->yMin=G_LOWER_MAXSCALE_PRESSURE;
		}
		break;
	case TREND_PMEAN:
		{
			m_lpfsTrend3->yMax=G_UPPER_MAXSCALE_PRESSURE*10;
			m_lpfsTrend3->yMin=G_LOWER_MAXSCALE_PRESSURE*10;

		}
		break;
	case TREND_FIO2:
		{
			m_lpfsTrend3->yMax=G_UPPER_MAXSCALE_FIO2;
			m_lpfsTrend3->yMin=G_LOWER_MAXSCALE_FIO2;
		}
		break;
	case TREND_VTE:
		{
			m_lpfsTrend3->yMax=G_UPPER_MAXSCALE_VTE*10;
			m_lpfsTrend3->yMin=G_LOWER_MAXSCALE_VTE*10;
		}
		break;
	case TREND_COMPLIANCE:
		{
			m_lpfsTrend3->yMax=G_UPPER_MAXSCALE_COMPL;
			m_lpfsTrend3->yMin=G_LOWER_MAXSCALE_COMPL;
		}
		break;
	case TREND_CO2HFO:
		{
			m_lpfsTrend3->yMax=G_UPPER_MAXSCALE_DCO2;
			m_lpfsTrend3->yMin=G_LOWER_MAXSCALE_DCO2;
		}
		break;
	case TREND_MV:
		{
			m_lpfsTrend3->yMax=G_UPPER_MAXSCALE_MV;
			m_lpfsTrend3->yMin=G_LOWER_MAXSCALE_MV;
		}
		break;
	case TREND_HFAMP:
		{
			m_lpfsTrend3->yMax=G_UPPER_MAXSCALE_HFAMP;
			m_lpfsTrend3->yMin=G_LOWER_MAXSCALE_HFAMP;
		}
		break;
	case TREND_RSBI:
		{
			m_lpfsTrend3->yMax=G_UPPER_MAXSCALE_RSBI;
			m_lpfsTrend3->yMin=G_LOWER_MAXSCALE_RSBI;
		}
		break;
	case TREND_SHAREMVMAND:
		{
			m_lpfsTrend3->yMax=G_UPPER_MAXSCALE_SHAREMVMAND;
			m_lpfsTrend3->yMin=G_LOWER_MAXSCALE_SHAREMVMAND;
		}
		break;
	case TREND_RESISTANCE:
		{
			m_lpfsTrend3->yMax=G_UPPER_MAXSCALE_RESISTANCE;
			m_lpfsTrend3->yMin=G_LOWER_MAXSCALE_RESISTANCE;
		}
		break;
	case TREND_LEAK:
		{
			m_lpfsTrend3->yMax=G_UPPER_MAXSCALE_LEAK;
			m_lpfsTrend3->yMin=G_LOWER_MAXSCALE_LEAK;
		}
		break;
	case TREND_SPO2:
		{
			m_lpfsTrend3->yMax=G_UPPER_MAXSCALE_SPO2;
			m_lpfsTrend3->yMin=G_LOWER_MAXSCALE_SPO2;
		}
		break;
	case TREND_SPO2PR:
		{
			m_lpfsTrend3->yMax=G_UPPER_MAXSCALE_SPO2PR;
			m_lpfsTrend3->yMin=G_LOWER_MAXSCALE_SPO2PR;
		}
		break;
	case TREND_SPO2PI:
		{
			m_lpfsTrend3->yMax=G_UPPER_MAXSCALE_SPO2PI;
			m_lpfsTrend3->yMin=G_LOWER_MAXSCALE_SPO2PI;
		}
		break;
	case TREND_ETCO2:
		{
			m_lpfsTrend3->yMax=G_UPPER_MAXSCALE_CO2;
			m_lpfsTrend3->yMin=G_LOWER_MAXSCALE_CO2;
		}
		break;
	case TREND_FREQUENCY:
		{
			m_lpfsTrend3->yMax=G_UPPER_MAXSCALE_FREQUENCY;
			m_lpfsTrend3->yMin=G_LOWER_MAXSCALE_FREQUENCY;
		}
		break;
	}
	m_lpfsTrend3->dYfactor=1;
	m_lpfsTrend3->dXfactor=1;
	m_lpfsTrend3->xMax=0;
	m_lpfsTrend3->xMin=0;
	m_lpfsTrend3->FuncType=G_PLOTXYPOINTS;
	if(m_bFilledGraph)
		m_lpfsTrend3->ChartType=G_FILLEDCHART;
	else
		m_lpfsTrend3->ChartType=G_LINECHART;
	m_lpfsTrend3->pPlotXYItems=m_pPlotItemsTrend3;
	m_lpfsTrend3->num_PlotXYItems=numitems;

	if(m_pcTrend3Diagram)
	{
		m_pcTrend3Diagram->DoFunction(m_lpfsTrend3);
	}

	//DEBUGMSG(TRUE, (TEXT("end DrawTrend3\r\n")));
}

/**********************************************************************************************//**
 * Paints this window
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewTrend::OnPaint() 
{
	CPaintDC dc(this);
	BitBlt(dc.m_hDC,0,0,m_lX,m_lY,m_hDC,0,0,SRCCOPY);
}

/**********************************************************************************************//**
 * Creates window hourglass
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewTrend::CreateWndHourglass()
{
	if(m_pWndHourglass==NULL)
	{
		m_pWndHourglass = new CWndWaitHourglass();

		RECT rcLd={250,180,550,380};
		if(m_pWndHourglass->Create(this,rcLd,IDC_VIEW_HOURGLASS,getModel()->GetLanguageString(IDS_TREND_LOADDATA)))
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************************************//**
 * Destroys the window hourglass
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CViewTrend::DestroyWndHourglass()
{
	if(m_pWndHourglass)
	{
		m_pWndHourglass->DestroyWindow();
		delete m_pWndHourglass;
		m_pWndHourglass=NULL;
	}
	return false;
}

/**********************************************************************************************//**
 * Shows the window hourglass
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	bShow	True to show, false to hide.
 **************************************************************************************************/

void CViewTrend::ShowWndHourglass(bool bShow)
{
	if(m_pWndHourglass)
	{
		m_pWndHourglass->Show(bShow);		
	}
}

/**********************************************************************************************//**
 * Sets one button depressed
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	btnID	Identifier for the button.
 **************************************************************************************************/

void CViewTrend::SetOneButtonDepressed(int btnID)
{
	if(m_bLoadDataRunning)
		return;

	if(m_bExit)
		return;

	POSITION pos;

	m_iCurBtnSelTrend=btnID;

	try
	{
		for( pos = m_plSelectTrendBtn.GetHeadPosition(); pos != NULL; )
		{
			CSelectTrendBtn* pUDBtn = m_plSelectTrendBtn.GetNext( pos );

			if(pUDBtn->GetBtnId() != btnID)
			{
				//if(pUDBtn->IsDepressed())
				{
					pUDBtn->Depress(false);
				}
			}
			else
			{
				pUDBtn->Depress(true);
			}
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CViewTrend::SetOneButtonDepressed"));
	}
	
	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);
}

/**********************************************************************************************//**
 * Sets all button unpressed
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CViewTrend::SetAllButtonUnpressed()
{
	if(m_bExit)
		return;

	POSITION pos;

	m_iCurBtnSelTrend=0;

	try
	{
		for( pos = m_plSelectTrendBtn.GetHeadPosition(); pos != NULL; )
		{
			CSelectTrendBtn* pUDBtn = m_plSelectTrendBtn.GetNext( pos );

			//if(pUDBtn->IsDepressed())
			{
				pUDBtn->Depress(false);
			}
		}
	}
	catch (...)
	{
		theApp.ReportException(_T("CViewTrend::SetAllButtonUnpressed"));
	}

	if(GetParent())
		GetParent()->PostMessage(WM_SET_SETUPTIMER);
}