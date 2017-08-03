// Diagramm.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "Diagramm.h"
#include "MVViewHandler.h"

//global font objects
extern HFONT g_hf6AcuNorm;
extern HFONT g_hf7AcuNorm;

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000


// CDiagramm

IMPLEMENT_DYNAMIC(CDiagramm, CWnd)

CDiagramm::CDiagramm()
{
	m_pModel=NULL;

	m_bExit=false;

	ptsLoop=new CPoint[G_MAXPOINTS];
	ptsLine=new CPoint[G_MAXPOINTS];


	m_dXAxisMin=0;
	m_dYAxisMin=0;
	m_dXAxisMax=0;
	m_dYAxisMax=0;

	m_iCharHeight=0;
	m_iCharWidth=0;
	m_iOriginX=0;
	m_iOriginY=0;
	m_dPixelsPerY=0;
	m_dPixelsPerX=0;	
	m_iScrollPosX=0;
	m_iScrollPosY=0;
	
	m_pFunctionParams=NULL;

	m_nID=0;

	m_hDC				= NULL;
	m_hBmp				= NULL;
	m_hdcStatic			= NULL;
	m_hdcGraph			= NULL;
	m_hbmpStatic		= NULL;
	m_hbmpGraph			= NULL;
	m_hBmpPrev			= NULL;

	m_bShowTicks=false;
	m_bShowGrid=false;
	m_bAutofit=false;
	m_bIgnoreAutofit=false;
	m_bYLineAtLeft=false;

	m_iGraphWidth=0;
	m_iGraphHeight=0;

	m_dwLastCheckAutoScaleX=0;
	m_dwLastCheckAutoScaleY=0;
		
	m_pcCursor=NULL;
	/*m_pcGraphMoveup_UP=NULL;
	m_pcGraphMoveup_DW=NULL;
	m_pcGraphMovedown_UP=NULL;
	m_pcGraphMovedown_DW=NULL;*/
	/*m_pcGraphXup=NULL;
	m_pcGraphXdw=NULL;*/

	/*m_hdcLevel1			= NULL;
	m_hbmpLevel1		= NULL;
	m_hdcLevel2			= NULL;
	m_hbmpLevel2		= NULL;*/
	/*m_hdcLevel3			= NULL;
	m_hbmpLevel3		= NULL;*/

	m_bTouchmove=false;

	m_bDrawCursor=false;

	//set default graph position
	m_iGraphX=0;
	m_iGraphY=0;

	m_iDiagrammType=0;
	m_iCurColorScheme=G_DEFAULTSCHEME;
	

	m_bFirstpoint=TRUE;
	m_prevx=0;
	m_prevy=0;

	

	m_bTriggeredBreath=false;
	m_bPreviousLoopBreath=false;
	//m_iCurrentXvalTimeAxis=0;
	m_dXTickSpace=0;
	m_dYTickSpace=0;

	m_dXNumericSpace=0;
	m_dYNumericSpace=0;

	m_dwStart=0;

	m_rmargin=0;
	m_lmargin=0;
	m_tmargin=0;
	m_bmargin=0;

	m_dMaxX=0;
	m_dMinX=0;
	m_dMaxY=0;
	m_dMinY=0;

	/*m_dMaxYcomplete=0;
	m_dMinYcomplete=0;*/

	m_bFREEZE=false;

	m_bSelected = false;

	m_iVolumeLimit=0;
	m_bDrawVolumeLimit=false;

	m_iVolumeGaranty=0;
	m_bDrawVolumeGaranty=false;

	m_iHFOVolumeGarantyHigh=0;
	m_iHFOVolumeGarantyLow=0;
	m_bDrawHFOVolumeGarantyHigh=false;
	m_bDrawHFOVolumeGarantyLow=false;

	m_iTrigThreshold=0;

	m_crPrevLoop3=RGB(210,210,210);
	m_crPrevLoop2=RGB(180,180,180);
	m_crPrevLoop1=RGB(140,140,140);

	//m_crTrigger=RGB(0,220,0);
	m_crTrigger=getModel()->getDATAHANDLER()->getGraphColor_TRIGGER();

	
	m_iXStartPos=0;
	m_iYStartPos=0;

	m_crYTickColor=RGB(0,0,255);
	m_crXTickColor=RGB(0,0,255);
	m_crYLegendTextColor=RGB(0,0,255);
	m_crXLegendTextColor=RGB(0,0,255);
	m_crStaticGraphPenColor=RGB(140,140,140);
	m_crCurrentGraphPenColor=RGB(140,140,140);
	m_iGraphPenSize=1;
	m_crGraphBkColor=WHITE;
	//m_crGraphBkColor=BACKGND;
	//m_crGraphBkColor=RGB(255,255,255);
	m_crGridColor=RGB(225,225,255);
	m_crGraphAutostateColor=RGB(255,255,255);	
	m_crGraphAutostateBkColor=RGB(140,140,140);
	m_crVLimitColor=RGB(255,0,0);

	

}



CDiagramm::~CDiagramm()
{
}

// **************************************************************************
// 
// **************************************************************************
CMVModel *CDiagramm::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}


BEGIN_MESSAGE_MAP(CDiagramm, CWnd)
	//{{AFX_MSG_MAP(CDiagramm)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CDiagramm-Meldungshandler

// **************************************************************************
// 
// **************************************************************************
BOOL CDiagramm::Create(CWnd* pParentWnd, const RECT &rc, UINT nID, CCreateContext* pContext) 
{
	//m_iGraphX=rc.left;
	//m_iGraphY=rc.top;
	m_iGraphX=0;
	m_iGraphY=0;
	//set graphsize (0 or less means default)
	m_iGraphWidth = rc.right-rc.left;
	m_iGraphHeight=rc.bottom-rc.top;



	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD/*|WS_VISIBLE*/,rc,pParentWnd,nID))
	{
		m_nID=nID;
		//m_pParentWnd=pParentWnd;

		//GraphSetAllDefaults();

		CClientDC dc(this);

		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_iGraphWidth,m_iGraphHeight);
		m_hBmpPrev=(HBITMAP)SelectObject(m_hDC,m_hBmp);

	
		CBrush cbrBack;
		cbrBack.CreateSolidBrush(m_crGraphBkColor);

		m_hdcGraph = CreateCompatibleDC(m_hDC);
		m_hbmpGraph = CreateCompatibleBitmap(m_hDC, m_iGraphWidth,m_iGraphHeight);
		SelectObject(m_hdcGraph, m_hbmpGraph);

		// delete background
		HPEN hpenprevGraph=(HPEN)SelectObject(m_hdcGraph, (HPEN)GetStockObject(NULL_PEN));	
		HBRUSH hbrprevGraph=(HBRUSH)SelectObject(m_hdcGraph,cbrBack); 
		Rectangle(m_hdcGraph, 0, 0, m_iGraphWidth,m_iGraphHeight);

		m_hdcStatic = CreateCompatibleDC(m_hDC);
		m_hbmpStatic = CreateCompatibleBitmap(m_hDC, m_iGraphWidth,m_iGraphHeight);
		SelectObject(m_hdcStatic, m_hbmpStatic);

		//// delete background
		HPEN hpenprevStat=(HPEN)SelectObject(m_hdcStatic, (HPEN)GetStockObject(NULL_PEN));	
		HBRUSH hbrprevStat=(HBRUSH)SelectObject(m_hdcStatic,cbrBack); 
		Rectangle(m_hdcStatic, 0, 0, m_iGraphWidth,m_iGraphHeight);


		//m_hdcLevel1 = CreateCompatibleDC(m_hDC);
		//m_hbmpLevel1 = CreateCompatibleBitmap(m_hDC, m_iGraphWidth,m_iGraphHeight);
		//SelectObject(m_hdcLevel1, m_hbmpLevel1);

		////// delete background
		//HPEN hpenprev1=(HPEN)SelectObject(m_hdcLevel1, (HPEN)GetStockObject(NULL_PEN));	
		//HBRUSH hbrprev1=(HBRUSH)SelectObject(m_hdcLevel1,cbrBack); 
		//Rectangle(m_hdcLevel1, 0, 0, m_iGraphWidth,m_iGraphHeight);

		//m_hdcLevel2 = CreateCompatibleDC(m_hDC);
		//m_hbmpLevel2 = CreateCompatibleBitmap(m_hDC, m_iGraphWidth,m_iGraphHeight);
		//SelectObject(m_hdcLevel2, m_hbmpLevel2);

		////// delete background
		//HPEN hpenprev2=(HPEN)SelectObject(m_hdcLevel2, (HPEN)GetStockObject(NULL_PEN));	
		//HBRUSH hbrprev2=(HBRUSH)SelectObject(m_hdcLevel2,cbrBack); 
		//Rectangle(m_hdcLevel2, 0, 0, m_iGraphWidth,m_iGraphHeight);

		//m_hdcLevel3 = CreateCompatibleDC(m_hDC);
		//m_hbmpLevel3 = CreateCompatibleBitmap(m_hDC, m_iGraphWidth,m_iGraphHeight);
		//SelectObject(m_hdcLevel3, m_hbmpLevel3);

		////// delete background
		//SelectObject(m_hdcLevel3, (HPEN)GetStockObject(NULL_PEN));	
		//SelectObject(m_hdcLevel3,cbrBack); 
		//Rectangle(m_hdcLevel3, 0, 0, m_iGraphWidth,m_iGraphHeight);

		HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf7AcuNorm);

		TEXTMETRIC textmetrics;
		GetTextMetrics(m_hDC,&textmetrics);
		m_iCharHeight=textmetrics.tmHeight;
		m_iCharWidth=textmetrics.tmAveCharWidth;
		//CreateGraphFont(_T("arial"),8);


		m_bIgnoreAutofit=false;

		if(m_iDiagrammType==SPO2_GRAPH || m_iDiagrammType==FOT_LOOP)
		{
			m_bAutofit=true;
		}
		else
		{
			m_bAutofit=getModel()->getCONFIG()->GraphIsAutoScale();
		}
		
		if(		m_bAutofit 
			&&	VENT_RUNNING!=getModel()->GetVentRunState())
		{
			IgnoreAutofit(true);
		}
		else if(	m_bAutofit 
			&&	getModel()->getALARMHANDLER()->ALARM_DISCONNECTION->getAlarmState()==AS_ACTIVE)
		{
			IgnoreAutofit(true);
		}
		m_bShowGrid=TRUE;
		m_bShowTicks=TRUE;
		m_bYLineAtLeft=FALSE;
		
		//set the default axis scaling
		SetXAxisScale(G_DEFAULTXMIN,G_DEFAULTXMAX);
		SetYAxisScale(G_DEFAULTYMIN,G_DEFAULTYMAX);
		//set the legend and Title Texts
		//SetDefaultGraphTitle();
		SetDefaultXLegend();
		SetDefaultYLegend();
		//SetDefaultFunctionName();

		//other stuff
		m_pFunctionParams=NULL;

		m_pcCursor		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_GRAPH_CURSOR);
		
		m_dwLastCheckAutoScaleX=GetTickCount();
		m_dwLastCheckAutoScaleY=GetTickCount();
		
		SelectObject(m_hDC,hPrevFont);
		SelectObject(m_hdcGraph, hpenprevGraph);	
		SelectObject(m_hdcGraph,hbrprevGraph); 
		SelectObject(m_hdcStatic, hpenprevStat);	
		SelectObject(m_hdcStatic,hbrprevStat); 
		/*SelectObject(m_hdcLevel1, hpenprev1);	
		SelectObject(m_hdcLevel1,hbrprev1); 
		SelectObject(m_hdcLevel2, hpenprev2);	
		SelectObject(m_hdcLevel2,hbrprev2); */

		return 1;
	}
	else
		return 0;
}

// **************************************************************************
// 
// **************************************************************************
void CDiagramm::Show(BOOL bShow) 
{
	if(m_bExit)
		return;

	if(this->IsWindowVisible() == bShow)
	{
		return;
	}

	if(bShow)
		SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
	else
		SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);
}

// **************************************************************************
// 
// **************************************************************************
void CDiagramm::OnPaint() 
{
	if(m_bFREEZE)
		return;

	CPaintDC dc(this);

	BitBlt(dc.m_hDC, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcStatic, 0, 0, SRCCOPY);
}

//************************************
// Method:    OnDestroy
// FullName:  CDiagramm::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CDiagramm::OnDestroy() 
{
	m_bExit=true;
	
	delete []ptsLoop;
	ptsLoop=NULL;
	
	delete []ptsLine;
	ptsLine=NULL;
	
	delete m_pcCursor;
	m_pcCursor=NULL;

	m_pFunctionParams=NULL;

	CWnd::OnDestroy();

	if(m_hBmp)
	{
		SelectObject(m_hDC,m_hBmpPrev);
		DeleteObject(m_hBmp);
	}

	if(m_hDC)
		DeleteDC(m_hDC);

	if(m_hdcStatic)
		DeleteDC(m_hdcStatic);

	if(m_hbmpStatic)
		DeleteObject(m_hbmpStatic);


	if(m_hdcGraph)
		DeleteDC(m_hdcGraph);

	/*if(m_hdcLevel1)
		DeleteDC(m_hdcLevel1);

	if(m_hbmpLevel1)
		DeleteObject(m_hbmpLevel1);

	if(m_hdcLevel2)
		DeleteDC(m_hdcLevel2);

	if(m_hbmpLevel2)
		DeleteObject(m_hbmpLevel2);*/

	/*if(m_hdcLevel3)
		DeleteDC(m_hdcLevel3);

	if(m_hbmpLevel3)
		DeleteObject(m_hbmpLevel3);*/

	if(m_hbmpGraph)
		DeleteObject(m_hbmpGraph);
}


// **************************************************************************
// 
// **************************************************************************
//int CDiagramm::GetDiagrammType()
//{
//	return m_iDiagrammType;
//}

// **************************************************************************
// 
// **************************************************************************
void CDiagramm::SetDiagrammType(int iType)
{
	if(m_bExit)
		return;

	m_iDiagrammType=iType;

//	int iTe=m_iCharHeight;

	switch(m_iDiagrammType)
	{
	case PRESSURE_HF_GRAPH:
		{
			m_dMaxX=0;
			m_dMinX=0;
			m_dMaxY=G_LOWER_MINSCALE_PRESSUREHF;
			m_dMinY=G_UPPER_MAXSCALE_PRESSUREHF;

			m_rmargin=DIAGRAMM_RIGHTMARGIN_CURVE;
			m_lmargin=DIAGRAMM_LEFTMARGIN;
			m_tmargin=m_iCharHeight;
			m_bmargin=m_iCharHeight;

			m_szXLegendText=_T("[")+getModel()->GetLanguageString(IDS_UNIT_SECONDS)+_T("]");

			if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
			{
				m_szYLegendText=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("]");
			}
			else
			{
				m_szYLegendText=_T("[")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("]");
			}
			
		}
		break;
	case PRESSURE_GRAPH:
		{
			m_dMaxX=0;
			m_dMinX=0;
			m_dMaxY=0;
			
			if(getModel()->getVMODEHANDLER()->activeModeIsNMODE())
			{
				m_dMinY=G_UPPER_MAXSCALE_NMODEPRESSURE;
			}
			else
			{
				m_dMinY=G_UPPER_MAXSCALE_PRESSURE;
			}

			m_rmargin=DIAGRAMM_RIGHTMARGIN_CURVE;
			m_lmargin=DIAGRAMM_LEFTMARGIN;
			m_tmargin=m_iCharHeight;
			m_bmargin=m_iCharHeight;

			m_szXLegendText=_T("[")+getModel()->GetLanguageString(IDS_UNIT_SECONDS)+_T("]");
			
			if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
			{
				m_szYLegendText=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("]");
			}
			else
			{
				m_szYLegendText=_T("[")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("]");
			}
		}
		break;
	case FLOW_HF_GRAPH:
	case FLOW_GRAPH:
		{
			m_dMaxX=(-1)*G_UPPER_MAXSCALE_FLOW;
			m_dMinX=G_UPPER_MAXSCALE_FLOW;
			m_dMaxY=(-1)*G_UPPER_MAXSCALE_FLOW;
			m_dMinY=G_UPPER_MAXSCALE_FLOW;

			m_rmargin=DIAGRAMM_RIGHTMARGIN_CURVE;
			m_lmargin=DIAGRAMM_LEFTMARGIN;
			m_tmargin=m_iCharHeight;
			m_bmargin=m_iCharHeight;

			m_szXLegendText=_T("[")+getModel()->GetLanguageString(IDS_UNIT_SECONDS)+_T("]");
			m_szYLegendText=_T("[")+getModel()->GetLanguageString(IDS_UNIT_LMIN)+_T("]");
		}
		break;
	case VOLUME_HF_GRAPH:
		{
			m_dMaxX=0;
			m_dMinX=0;
			m_dMaxY=0;
			m_dMinY=G_UPPER_MAXSCALE_VOLUME_HF;

			m_rmargin=DIAGRAMM_RIGHTMARGIN_CURVE;
			m_lmargin=DIAGRAMM_LEFTMARGIN;
			m_tmargin=m_iCharHeight;
			m_bmargin=m_iCharHeight;

			m_szXLegendText=_T("[")+getModel()->GetLanguageString(IDS_UNIT_SECONDS)+_T("]");
			m_szYLegendText=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MILLILITER)+_T("]");
		}
		break;
	case VOLUME_GRAPH:
		{
			m_dMaxX=0;
			m_dMinX=0;
			m_dMaxY=0;

			if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
				m_dMinY=G_UPPER_MAXSCALE_VOLUME_NEO;
			else
				m_dMinY=G_UPPER_MAXSCALE_VOLUME_PED;


			m_rmargin=DIAGRAMM_RIGHTMARGIN_CURVE;
			m_lmargin=DIAGRAMM_LEFTMARGIN;
			m_tmargin=m_iCharHeight;
			m_bmargin=m_iCharHeight;

			m_szXLegendText=_T("[")+getModel()->GetLanguageString(IDS_UNIT_SECONDS)+_T("]");
			m_szYLegendText=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MILLILITER)+_T("]");
		}
		break;
	case CO2_HF_GRAPH:
	case CO2_GRAPH:
		{
			m_dMaxX=0;
			m_dMinX=0;
			m_dMaxY=0;
			m_dMinY=G_UPPER_MAXSCALE_CO2;

			m_rmargin=DIAGRAMM_RIGHTMARGIN_CURVE;
			m_lmargin=DIAGRAMM_LEFTMARGIN;
			m_tmargin=m_iCharHeight;
			m_bmargin=m_iCharHeight;

			eCO2unit unit=getModel()->getCONFIG()->GetCO2unit();
			switch(unit)
			{
			case CO2_UNIT_KPA:
				{
					m_szYLegendText=_T("[")+getModel()->GetLanguageString(IDS_UNIT_KPA)+_T("]");
				}
				break;
			case CO2_UNIT_PERCENT:
				{
					m_szYLegendText=_T("[")+getModel()->GetLanguageString(IDS_UNIT_VOLPERCENT)+_T("]");
				}
				break;
			case CO2_UNIT_MMHG:
			default:
				{
					m_szYLegendText=_T("[")+getModel()->GetLanguageString(IDS_UNIT_MMHG)+_T("]");
				}
				break;
			}

			m_szXLegendText=_T("[")+getModel()->GetLanguageString(IDS_UNIT_SECONDS)+_T("]");
			
		}
		break;
	case SPO2_GRAPH:
		{
			m_dMaxX=0;
			m_dMinX=0;
			m_dMaxY=(-1)*G_UPPER_MAXSCALE_SPO2;
			m_dMinY=G_UPPER_MAXSCALE_SPO2;

			m_rmargin=DIAGRAMM_RIGHTMARGIN_CURVE;
			m_lmargin=DIAGRAMM_LEFTMARGIN;
			m_tmargin=m_iCharHeight;
			m_bmargin=m_iCharHeight;
			
			//m_szYLegendText=_T("[")+getModel()->GetLanguageString(IDS_UNIT_PERCENT)+_T("]");
			m_szYLegendText=_T("");
			m_szXLegendText=_T("[")+getModel()->GetLanguageString(IDS_UNIT_SECONDS)+_T("]");

			m_bAutofit=true;
		}
		break;
	case FOT_LOOP:
		{
			//m_dMaxX=G_UPPER_MAXSCALE_PMEAN_FOT;
			//m_dMinX=G_LOWER_MAXSCALE_PMEAN_FOT;
			m_dMaxX=G_UPPER_MAXSCALE_PRESSURE;
			m_dMinX=G_LOWER_MAXSCALE_PRESSURE_FOT;
			m_dMaxY=G_LOWER_MINSCALE_XRS_FOT;//rku, check FOTGRAPH
			m_dMinY=G_UPPER_MAXSCALE_XRS_FOT;

			m_rmargin=DIAGRAMM_RIGHTMARGIN_LOOP;
			m_lmargin=DIAGRAMM_LEFTMARGIN;
			m_tmargin=m_iCharHeight;
			m_bmargin=DIAGRAMM_BOTTOMMARGIN_LOOP;//2*m_iCharHeight;

			//pro, correction
			//m_szYLegendText=_T("[Pmean]Xrs");
			//m_szXLegendText=_T("[Pmean]");
			//m_szYLegendText=_T("Xrs [cmH2O*s/L]");
			if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
			{
				m_szYLegendText=_T("Xrs [")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("/lps]");
			}
			else if (getModel()->getCONFIG()->GetPressureUnit()==PUNIT_CMH2O)
			{
				m_szYLegendText=_T("Xrs [")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("/lps]");
			}

			if(getModel()->getVMODEHANDLER()->activeModeIsHFO() && getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
			{
				m_szXLegendText=_T("Pmean [")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("]");
			}
			else if (getModel()->getVMODEHANDLER()->activeModeIsHFO() && getModel()->getCONFIG()->GetPressureUnit()==PUNIT_CMH2O)
			{
				m_szXLegendText=_T("Pmean [")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("]");
			}
			else if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
			{
				m_szXLegendText=_T("PEEP [")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("]");
			}
			else
			{
				m_szXLegendText=_T("PEEP [")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("]");
			}

			m_bAutofit=true;
		}
		break;
	case PRESSURE_VOLUME_LOOP:
		{
			m_dMaxX=0;
			m_dMinX=G_UPPER_MAXSCALE_PRESSURE;
			m_dMaxY=0;

			if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
				m_dMinY=G_UPPER_MAXSCALE_VOLUME_NEO;
			else
				m_dMinY=G_UPPER_MAXSCALE_VOLUME_PED;

			m_rmargin=DIAGRAMM_RIGHTMARGIN_LOOP;
			m_lmargin=DIAGRAMM_LEFTMARGIN;
			m_tmargin=m_iCharHeight;
			m_bmargin=2*m_iCharHeight;

			if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
			{
				m_szXLegendText=getModel()->GetLanguageString(IDS_GRAPH_PRESSURE)+_T(" [")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("]");
			}
			else
			{
				m_szXLegendText=getModel()->GetLanguageString(IDS_GRAPH_PRESSURE)+_T(" [")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("]");
			}

			
			m_szYLegendText=getModel()->GetLanguageString(IDS_GRAPH_VOLUME)+_T(" [")+getModel()->GetLanguageString(IDS_UNIT_MILLILITER)+_T("]");
			
		}
		break;
	case PRESSURE_VOLUME_HF_LOOP:
		{
			m_dMaxX=G_LOWER_MINSCALE_PRESSUREHF;
			m_dMinX=G_UPPER_MAXSCALE_PRESSUREHF;
			m_dMaxY=0;
			m_dMinY=G_UPPER_MAXSCALE_VOLUME_HF;

			m_rmargin=DIAGRAMM_RIGHTMARGIN_LOOP;
			m_lmargin=DIAGRAMM_LEFTMARGIN;
			m_tmargin=m_iCharHeight;
			m_bmargin=2*m_iCharHeight;

			if(getModel()->getCONFIG()->GetPressureUnit()==PUNIT_MBAR)
			{
				m_szXLegendText=getModel()->GetLanguageString(IDS_GRAPH_PRESSURE)+_T(" [")+getModel()->GetLanguageString(IDS_UNIT_MBAR)+_T("]");
			}
			else
			{
				m_szXLegendText=getModel()->GetLanguageString(IDS_GRAPH_PRESSURE)+_T(" [")+getModel()->GetLanguageString(IDS_UNIT_CMH2O)+_T("]");
			}

			
			m_szYLegendText=getModel()->GetLanguageString(IDS_GRAPH_VOLUME)+_T(" [")+getModel()->GetLanguageString(IDS_UNIT_MILLILITER)+_T("]");

		}
		break;
	case VOLUME_FLOW_LOOP:
		{
			m_dMaxX=0;
			
			if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
				m_dMinX=G_UPPER_MAXSCALE_VOLUME_NEO;
			else
				m_dMinX=G_UPPER_MAXSCALE_VOLUME_PED;

			m_dMaxY=(-1)*G_UPPER_MAXSCALE_FLOW;
			m_dMinY=G_UPPER_MAXSCALE_FLOW;

			m_rmargin=DIAGRAMM_RIGHTMARGIN_LOOP;
			m_lmargin=DIAGRAMM_LEFTMARGIN;
			m_tmargin=m_iCharHeight;
			m_bmargin=2*m_iCharHeight;

			m_szXLegendText=getModel()->GetLanguageString(IDS_GRAPH_VOLUME)+_T(" [")+getModel()->GetLanguageString(IDS_UNIT_MILLILITER)+_T("]");
			m_szYLegendText=getModel()->GetLanguageString(IDS_GRAPH_FLOW)+_T(" [")+getModel()->GetLanguageString(IDS_UNIT_LMIN)+_T("]");

		}
		break;
	case VOLUME_FLOW_HF_LOOP:
		{
			m_dMaxX=0;
			m_dMinX=G_UPPER_MAXSCALE_VOLUME_HF;
			m_dMaxY=(-1)*G_UPPER_MAXSCALE_FLOW;
			m_dMinY=G_UPPER_MAXSCALE_FLOW;

			m_rmargin=DIAGRAMM_RIGHTMARGIN_LOOP;
			m_lmargin=DIAGRAMM_LEFTMARGIN;
			m_tmargin=m_iCharHeight;
			m_bmargin=2*m_iCharHeight;

			m_szXLegendText=getModel()->GetLanguageString(IDS_GRAPH_VOLUME)+_T(" [")+getModel()->GetLanguageString(IDS_UNIT_MILLILITER)+_T("]");
			m_szYLegendText=getModel()->GetLanguageString(IDS_GRAPH_FLOW)+_T(" [")+getModel()->GetLanguageString(IDS_UNIT_LMIN)+_T("]");

		}
		break;
	default:
		{
			m_dMaxX=-100;
			m_dMinX=100;
			m_dMaxY=-100;
			m_dMinY=100;

			m_rmargin=DIAGRAMM_RIGHTMARGIN_CURVE;
			m_lmargin=DIAGRAMM_LEFTMARGIN;
			m_tmargin=m_iCharHeight;
			m_bmargin=m_iCharHeight;

			m_szXLegendText=_T("");
			m_szYLegendText=_T("");
		}
		break;
	}
}

// **************************************************************************
// 	
//				
//				
// **************************************************************************
COLORREF CDiagramm::getCurGraphPenColor()
{
	return m_crCurrentGraphPenColor;
}
// **************************************************************************
// SetColorScheme set the color scheme for the graph	
//				There are 8 colorschemes in total but default colour scheme is the
//				same as as the WHITE colorscheme (which is really grey)
// **************************************************************************
int CDiagramm::SetColorScheme(int Scheme, COLORREF crGraphPenColor, BOOL bRedraw)
{
	if(m_bExit)
		return 0;

	int iPrevColorScheme=m_iCurColorScheme;
	//m_crPrevGraphPenColor=m_crGraphPenColor;

	m_iCurColorScheme=Scheme;
	/*
	This sets up the colors for various graph elements
	*/
	switch (Scheme)
	{
	case G_DEFAULTSCHEME:
		m_crYTickColor=RGB(0,0,255);
		m_crXTickColor=RGB(0,0,255);
		m_crYLegendTextColor=RGB(0,0,255);
		m_crXLegendTextColor=RGB(0,0,255);
		m_crStaticGraphPenColor=crGraphPenColor;
		m_crCurrentGraphPenColor=crGraphPenColor;
		//m_crGraphPenColor=RGB(140,140,140);
		m_iGraphPenSize=2;
		m_crGraphBkColor=WHITE;
		m_crGridColor=RGB(225,225,255);
		m_crGraphAutostateColor=RGB(255,255,255);	
		m_crGraphAutostateBkColor=RGB(140,140,140);
		m_crVLimitColor=RGB(255,0,0);
		break;
	case G_DISABLED:
		m_crYTickColor=RGB(0,0,255);
		m_crXTickColor=RGB(0,0,255);
		m_crYLegendTextColor=RGB(0,0,255);
		m_crXLegendTextColor=RGB(0,0,255);
		m_crStaticGraphPenColor=crGraphPenColor;
		m_crCurrentGraphPenColor=crGraphPenColor;
		//m_crGraphPenColor=RGB(140,140,140);
		m_iGraphPenSize=2;
		m_crGraphBkColor=LIGHTGREY;
		m_crGridColor=RGB(225,225,255);
		m_crGraphAutostateColor=RGB(255,255,255);	
		m_crGraphAutostateBkColor=RGB(140,140,140);
		m_crVLimitColor=RGB(255,0,0);
		break;
	case G_FabianHFOSCHEME_LINE:
		m_crYTickColor=RGB(0,0,255);
		m_crXTickColor=RGB(0,0,255);
		m_crYLegendTextColor=RGB(0,0,255);
		m_crXLegendTextColor=RGB(0,0,255);
		m_crStaticGraphPenColor=crGraphPenColor;
		m_crCurrentGraphPenColor=crGraphPenColor;
		//m_crGraphPenColor=RGB(140,140,140);
		m_iGraphPenSize=1;
		m_crGraphBkColor=WHITE;
		m_crGridColor=RGB(225,225,255);
		m_crGraphAutostateColor=RGB(255,255,255);	
		m_crGraphAutostateBkColor=RGB(140,140,140);
		m_crVLimitColor=RGB(255,0,0);
		break;
	case G_FabianHFOSCHEME_LOOP:
		m_crYTickColor=RGB(0,0,255);
		m_crXTickColor=RGB(0,0,255);
		m_crYLegendTextColor=RGB(0,0,255);
		m_crXLegendTextColor=RGB(0,0,255);
		m_crStaticGraphPenColor=crGraphPenColor;
		m_crCurrentGraphPenColor=crGraphPenColor;
		//m_crGraphPenColor=RGB(0,0,255);
		m_iGraphPenSize=2;
		m_crGraphBkColor=WHITE;
		m_crGridColor=RGB(225,225,255);
		m_crGraphAutostateColor=RGB(255,255,255);	
		m_crGraphAutostateBkColor=RGB(140,140,140);
		m_crVLimitColor=RGB(255,0,0);
		break;
	case G_FOT_LOOPD:
		m_crYTickColor=RGB(0,0,255);
		m_crXTickColor=RGB(0,0,255);
		m_crYLegendTextColor=RGB(0,0,255);
		m_crXLegendTextColor=RGB(0,0,255);
		//m_crStaticGraphPenColor=crGraphPenColor;
		//m_crCurrentGraphPenColor=crGraphPenColor;

		m_crStaticGraphPenColor=crGraphPenColor;
		m_crCurrentGraphPenColor=crGraphPenColor;
		//m_crGraphPenColor=RGB(0,0,255);
		// RGB(140,140,140);
		m_iGraphPenSize=2;
		m_crGraphBkColor=WHITE;
		m_crGridColor=RGB(225,225,255);
		m_crGraphAutostateColor=RGB(255,255,255);	
		m_crGraphAutostateBkColor=RGB(140,140,140);
		m_crVLimitColor=RGB(255,0,0);
		break;
	case G_FabianHFOSCHEME_SAVEDLOOP:
		m_crYTickColor=RGB(0,0,255);
		m_crXTickColor=RGB(0,0,255);
		m_crYLegendTextColor=RGB(0,0,255);
		m_crXLegendTextColor=RGB(0,0,255);
		m_crStaticGraphPenColor=crGraphPenColor;
		m_crCurrentGraphPenColor=crGraphPenColor;
		//m_crGraphPenColor=RGB(0,0,0);
		m_iGraphPenSize=2;
		m_crGraphBkColor=WHITE;
		m_crGridColor=RGB(225,225,255);
		m_crGraphAutostateColor=RGB(255,255,255);	
		m_crGraphAutostateBkColor=RGB(140,140,140);
		m_crVLimitColor=RGB(255,0,0);
		break;
	case G_FabianHFOSCHEME_PREVLOOP1:
		m_crYTickColor=RGB(0,0,255);
		m_crXTickColor=RGB(0,0,255);
		m_crYLegendTextColor=RGB(0,0,255);
		m_crXLegendTextColor=RGB(0,0,255);
		m_crCurrentGraphPenColor=m_crPrevLoop1;
		m_crStaticGraphPenColor=m_crPrevLoop1;
		m_iGraphPenSize=2;
		m_crGraphBkColor=WHITE;
		m_crGridColor=RGB(225,225,255);
		m_crGraphAutostateColor=RGB(255,255,255);	
		m_crGraphAutostateBkColor=RGB(140,140,140);
		m_crVLimitColor=RGB(255,0,0);
		break;
	case G_FabianHFOSCHEME_PREVLOOP2:
		m_crYTickColor=RGB(0,0,255);
		m_crXTickColor=RGB(0,0,255);
		m_crYLegendTextColor=RGB(0,0,255);
		m_crXLegendTextColor=RGB(0,0,255);
		m_crCurrentGraphPenColor=m_crPrevLoop2;
		m_crStaticGraphPenColor=m_crPrevLoop2;
		m_iGraphPenSize=2;
		m_crGraphBkColor=WHITE;
		m_crGridColor=RGB(225,225,255);
		m_crGraphAutostateColor=RGB(255,255,255);	
		m_crGraphAutostateBkColor=RGB(140,140,140);
		m_crVLimitColor=RGB(255,0,0);
		break;
	case G_FabianHFOSCHEME_PREVLOOP3:
		m_crYTickColor=RGB(0,0,255);
		m_crXTickColor=RGB(0,0,255);
		m_crYLegendTextColor=RGB(0,0,255);
		m_crXLegendTextColor=RGB(0,0,255);
		m_crCurrentGraphPenColor=m_crPrevLoop3;
		m_crStaticGraphPenColor=m_crPrevLoop3;
		m_iGraphPenSize=2;
		m_crGraphBkColor=WHITE;
		m_crGridColor=RGB(225,225,255);
		m_crGraphAutostateColor=RGB(255,255,255);	
		m_crGraphAutostateBkColor=RGB(140,140,140);
		m_crVLimitColor=RGB(255,0,0);
		break;
	default:
		break;

	}


	if(bRedraw)
	{
		PaintGraph();
	}

	return iPrevColorScheme;
}





// **************************************************************************
// This sets the min and max values of the x-axis (of the plotting area)
//				It also sets what proportion of the x-range is denoted by one pixel
//				a.ka. PixelsPerX
//				It also caluclates where the x-origin (the x==0) point lies along
//				the x-axis - if x does not pass through zero the x-origin gidline is shown
//				at the top or bottom of the graph.
//				(the x-origin point on screen is stored in the m_iOriginX parameter as 
//				 an offset from the LHS of the (total) graph
//				 26/2/2005 - The autofit parameter is always TRUE as scrolling is not yet
//				 implemented.
// **************************************************************************
void CDiagramm::SetXAxisScale(double min, double max, BOOL bRedraw,bool bRedrawDiagrammData)
{
	if(m_bExit)
		return;

	double minimum=GetNegativeYoffset(max);

	//if(max<0 || min>=minimum)
	{
		switch(m_iDiagrammType)
		{
		case FOT_LOOP:
			{
				if(max<0)
				{
					max=G_UPPER_MAXSCALE_PRESSURE;
					min=m_dYAxisMin;
				}
				//else if(min>minimum)
				//{
				//	//max=m_dYAxisMax;
				//	min=GetNegativeYoffset(m_dYAxisMax);
				//}
			}
			break;
		case PRESSURE_VOLUME_LOOP:
			{
				if(max<0 || min>minimum)
				{
					max=G_UPPER_MAXSCALE_PRESSURE;
					min=GetNegativeYoffset(G_UPPER_MAXSCALE_PRESSURE);
				}
			}
			break;
		case PRESSURE_VOLUME_HF_LOOP:
			{
				if(max<0)
				{
					max=G_UPPER_MAXSCALE_PRESSUREHF;
					min=m_dYAxisMin;
				}
				else if(min>minimum)
				{
					//max=m_dYAxisMax;
					min=GetNegativeYoffset(m_dYAxisMax);
				}
			}
			break;
		case VOLUME_FLOW_LOOP:
			{
				if(max<0 || min>minimum)
				{
					int iUPPER_MAXSCALE_VOLUME=0;
					if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
						iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_NEO;
					else
						iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_PED;


					max=iUPPER_MAXSCALE_VOLUME;
					min=GetNegativeYoffset(iUPPER_MAXSCALE_VOLUME);
				}
				
			}
			break;
		case VOLUME_FLOW_HF_LOOP:
			{
				if(max<0 || min>minimum)
				{
					max=G_UPPER_MAXSCALE_VOLUME_HF;
					min=GetNegativeYoffset(G_UPPER_MAXSCALE_VOLUME_HF);
				}

			}
			break;
		default:
			{

			}
			break;
		}
	}

	//swap min and max if they are the wrong way around
	double temp,scale;
	if (max < min)
	{
		temp =min;
		min=max;
		max=temp;
	}
	//if min and max are the same (especially if they are both zero
	//it can be a problem - so give them a bit of room
	if(min==max)
	{
		max+= 0.1;
	}

	//set the member variables
	m_dXAxisMax=max;
	m_dXAxisMin=min;

	temp=max-min;//the spread of the x-axiz
	scale=(m_iGraphWidth-m_lmargin-m_rmargin)/temp;//calc pixels/x
	m_dPixelsPerX=scale;

	//where would the x-origin be located?
	if( (min < 0) && (max >0))
	{
		m_iOriginX=(int)(fabs(min) * m_dPixelsPerX + m_lmargin);
	}
	else if ( (min <0) && (max <=0))
	{
		m_iOriginX=(m_iGraphWidth)-m_rmargin;
	}
	else if (min >=0 && (max>=0))
	{
		m_iOriginX=m_lmargin;
	}



	switch(m_iDiagrammType)
	{
	case PRESSURE_VOLUME_LOOP:
	case VOLUME_FLOW_LOOP:
	case PRESSURE_VOLUME_HF_LOOP:
	case VOLUME_FLOW_HF_LOOP:
	case FOT_LOOP:
		{
			SetXAxisTickSpace(CalculateXAxisTickGridSpace());
			SetXAxisNumericSpace(CalculateXAxisNumericSpace());
		}
		break;
	default:
		{

		}
		break;
	}

	if(bRedraw)
	{
		//m_bFirstpoint=true;
		PaintGraph();
	}
	if(bRedrawDiagrammData)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_REDRAW_DIAGRAMMDATA,m_nID);//rku APPERR
	}
}


// **************************************************************************
// 
// **************************************************************************
int CDiagramm::GetYPixelsOfDiagramm()
{
	return m_iGraphHeight-(m_bmargin+m_tmargin);
}

// **************************************************************************
// This sets the min and max values of the y-axis (of the plotting area)
//				It also sets what proportion of the grapgh axis pixels represents 1Y
//				(PixelsPerY =plotheightinpixels/yrange)
//				It also caluclates where the y-origin (the y==0) point lies along
//				the y-axis - if y does not pass through zero the y-origin gidline is shown
//				at the left or right of the graph (this can be overridden by using the SetYLineAtLeft()
//				function.
//				(the y-origin point on screen is stored in the m_iOriginY parameter as 
//				an offset from the bottom of the (total) graph
//				The autofit parameter is always TRUE as scrolling is not yet
//				implemented.
// **************************************************************************
void CDiagramm::SetYAxisScale(double min, double max, BOOL bRedraw,bool bRedrawDiagrammData)
{
	if(m_bExit)
		return;

	double minimum=GetNegativeYoffset(max);

	//if(max<0 || min>minimum)
	{
		switch(m_iDiagrammType)
		{
		case PRESSURE_HF_GRAPH:
			{
				if(max<0)
				{
					max=G_UPPER_MAXSCALE_PRESSUREHF;
					min=m_dYAxisMin;
				}
				else if(min>minimum)
				{
					//max=m_dYAxisMax;
					min=GetNegativeYoffset(m_dYAxisMax);
				}
			}
			break;
		case PRESSURE_GRAPH:
			{
				if(max<0 || min>minimum)
				{
					if(getModel()->getVMODEHANDLER()->activeModeIsNMODE())
					{
						max=G_UPPER_MAXSCALE_NMODEPRESSURE;
						min=GetNegativeYoffset(G_UPPER_MAXSCALE_NMODEPRESSURE);
					}
					else
					{
						max=G_UPPER_MAXSCALE_PRESSURE;
						min=GetNegativeYoffset(G_UPPER_MAXSCALE_PRESSURE);
					}
				}
			}
			break;
		case FLOW_HF_GRAPH:
		case FLOW_GRAPH:
			{
				if(max<0)
				{
					max=G_UPPER_MAXSCALE_FLOW;
					min=max*(-1);
				}
			}
			break;
		case VOLUME_GRAPH:
			{
				if(max<0 || min>minimum)
				{
					int iUPPER_MAXSCALE_VOLUME=0;
					if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
						iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_NEO;
					else
						iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_PED;

					max=iUPPER_MAXSCALE_VOLUME;
					min=GetNegativeYoffset(iUPPER_MAXSCALE_VOLUME);
				}
			}
			break;
		case VOLUME_HF_GRAPH:
			{
				if(max<0 || min>minimum)
				{
					max=G_UPPER_MAXSCALE_VOLUME_HF;
					min=GetNegativeYoffset(G_UPPER_MAXSCALE_VOLUME_HF);
				}
			}
			break;
		case CO2_GRAPH:
		case CO2_HF_GRAPH:
			{
				if(max<0 || min>minimum)
				{
					max=G_UPPER_MAXSCALE_CO2;
					min=GetNegativeYoffset(G_UPPER_MAXSCALE_CO2);
				}
			}
			break;
		case SPO2_GRAPH:
			{
				if(max<0)
				{
					max=G_UPPER_MAXSCALE_SPO2;
					min=max*(-1);
				}
			}
			break;
		case FOT_LOOP:
			{
				//if(max!=0 || min<G_LOWER_MINSCALE_XRS_FOT)
				if(max>G_UPPER_MAXSCALE_XRS_FOT || min<G_LOWER_MINSCALE_XRS_FOT)//rku, check FOTGRAPH
				{
					max=G_UPPER_MAXSCALE_XRS_FOT;
					min=G_LOWER_MINSCALE_XRS_FOT;
				}
			}
			break;
		case PRESSURE_VOLUME_LOOP:
			{
				if(max<0 || min>minimum)
				{
					int iUPPER_MAXSCALE_VOLUME=0;
					if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
						iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_NEO;
					else
						iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_PED;

					max=iUPPER_MAXSCALE_VOLUME;
					min=GetNegativeYoffset(iUPPER_MAXSCALE_VOLUME);
				}
			}
			break;
		case PRESSURE_VOLUME_HF_LOOP:
			{
				if(max<0 || min>minimum)
				{
					max=G_UPPER_MAXSCALE_VOLUME_HF;
					min=GetNegativeYoffset(G_UPPER_MAXSCALE_VOLUME_HF);
				}
			}
			break;
		case VOLUME_FLOW_LOOP:
		case VOLUME_FLOW_HF_LOOP:
			{
				if(max<0)
				{
					max=G_UPPER_MAXSCALE_FLOW;
					min=max*(-1);
				}
			}
			break;
		default:
			{

			}
			break;
		}
	}
	

	double temp,scale;
	//swap min and max if they are the wrong way around
	if (max < min)
	{
		temp=min;
		min=max;
		max=temp;
	}

	//if min and max are the same (especially if they are both zero
	//it can be a problem - so give them a bit of room
	if(min==max)
	{
		max+= 0.1;
	}

	//set the member variables
	m_dYAxisMin=min;
	m_dYAxisMax=max;

	//calculate scaling
	CRect dataarea=CalcDataArea();

	temp=max-min;//the spread of the y-axiz
	scale=GetYPixelsOfDiagramm()/temp;//calc pixels/x
	m_dPixelsPerY=scale;

	//where should the Y origin be?

	if (min<0 && max >0)//if Y passes through zero
	{
		m_iOriginY=(int)(fabs(min)*m_dPixelsPerY+m_bmargin);
	}
	//else if(min<0 && max==0)//if Y values are all negative
	//{
	//	m_iOriginY=(int)(fabs(min)*m_dPixelsPerY+m_bmargin);
	//}
	else if(min<0 && max<=0)//if Y values are all negative
	{
		//m_iOriginY=(dataarea.bottom-dataarea.top)+m_bmargin;
		m_iOriginY=fabs(min)*m_dPixelsPerY+m_bmargin;
	}
	else if (min >=0 && max >=0)//if Y values are all positive
	{
		/*m_iOriginY=fabs(min)*m_dPixelsPerY+m_bmargin;

		if(m_iOriginY>m_bmargin)
		m_iOriginY=m_iOriginY*-1;*/
		m_iOriginY=m_bmargin;
	}	

	SetYAxisTickSpace(CalculateYAxisTickGridSpace());
	SetYAxisNumericSpace(CalculateYAxisNumericSpace());

	if(bRedraw)
	{
		PaintGraph();
	}

	if(bRedrawDiagrammData)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_REDRAW_DIAGRAMMDATA,m_nID);//rku APPERR
	}
	
}


// **************************************************************************
// This paints the entire graph on to the holding window's
//				client area;
//				It does it in steps starting from the background and working forward.
//				As the graph is NOT a window object in it's own right, it uses the
//				display context of the holding window. If it has not been given a 
//				pointer to the holding window, it will not paint.
//				Any CGraph routine that paints to the screen, checks the window pointer
//				first.
//				The last thing to be painted is the plotting of the function data (if any)
// **************************************************************************
void CDiagramm::PaintGraph()
{
	if(m_bExit)
		return;

	//m_bFirstpoint=true;

	//here we draw the graph
	//step 1: Draw the surrounding rectangle
	//for the whole graph
	CRect rect;
	CPen penBack,penTick;
	CPen *oldpen=NULL;
	CClientDC dc(this);
	CDC* pDCStatic=CDC::FromHandle(m_hdcStatic);
	//some useful calculations
	UINT Graphbottom=m_iGraphY+m_iGraphHeight;
	UINT Graphright=m_iGraphX+m_iGraphWidth;

	//step 2: color the background
	CBrush brush;
	//brush.CreateSolidBrush(WHITE);
	brush.CreateSolidBrush(m_crGraphBkColor);
	penBack.CreatePen(PS_SOLID,1,BLACK);
	rect.left=m_iGraphX;
	rect.right =rect.left+m_iGraphWidth;
	rect.top=m_iGraphY;
	rect.bottom=rect.top+m_iGraphHeight;
	oldpen=pDCStatic->SelectObject(&penBack);
	CBrush *poldbrush=pDCStatic->SelectObject(&brush);
	pDCStatic->Rectangle(&rect);
	
	////step 3: Draw Grid if required
	if(m_iDiagrammType != FOT_LOOP)
		DrawGrid(pDCStatic);
	//Sleep(0);


	////step 4: Draw Axies
	////draw x-axis
	penTick.CreatePen(PS_SOLID,1,m_crXTickColor);
	pDCStatic->SelectObject(&penTick);

	if(m_iDiagrammType == FOT_LOOP)
	{
		pDCStatic->MoveTo(m_iGraphX+m_lmargin,Graphbottom-m_bmargin);
		pDCStatic->LineTo(Graphright-m_rmargin,Graphbottom-m_bmargin);
	}
	else if(m_iDiagrammType != SPO2_GRAPH)
	{
		pDCStatic->MoveTo(m_iGraphX+m_lmargin,Graphbottom-m_iOriginY);
		pDCStatic->LineTo(Graphright-m_rmargin,Graphbottom-m_iOriginY);
	}
	
	//pDCStatic->SelectObject(oldpen);
	

	////draw the Y Axis
	if(m_iDiagrammType == FOT_LOOP)
	{
		//m_iOriginY=(dataarea.bottom-dataarea.top)+m_bmargin
		//draw the Y Line at the LHS
		/*pDCStatic->MoveTo(m_iGraphX+m_lmargin,m_iGraphY+m_tmargin);
		pDCStatic->LineTo(m_iGraphX+m_lmargin,Graphbottom-m_bmargin);*/

		pDCStatic->MoveTo(m_iGraphX+m_iOriginX,m_iGraphY+m_tmargin);
		pDCStatic->LineTo(m_iGraphX+m_iOriginX,Graphbottom-m_bmargin);
	}
	else if(m_iDiagrammType != SPO2_GRAPH)
	{
		if (!m_bYLineAtLeft)
		{
			//draw the Y Line so that it intercepts
			//the x-line like crosshairs
			pDCStatic->MoveTo(m_iGraphX+m_iOriginX,m_iGraphY+m_tmargin);
			pDCStatic->LineTo(m_iGraphX+m_iOriginX,Graphbottom-m_bmargin);
		}
		else
		{
			//draw the Y Line at the LHS
			pDCStatic->MoveTo(m_iGraphX+m_lmargin,m_iGraphY+m_tmargin);
			pDCStatic->LineTo(m_iGraphX+m_lmargin,Graphbottom-m_bmargin);
		}
	}

	//Sleep(0);

	////step 5: draw ticks if required
	if(m_iDiagrammType != SPO2_GRAPH)
	{
		DrawTicks(pDCStatic);
	}
	
	//Sleep(0);

	////step 6: Write Graph title
	//DrawGraphTitle();

	////step 7: Write Function name
	//DrawFunctionName();

	////step 8: Write x-legend
	DrawXLegend(pDCStatic);
	//Sleep(0);

	////step 9: Write y legend
	////doing this is very similar to doing the graph title or the
	////x-legend but the Y axis is either at the LHS or set to match the
	////x-origin
	if(m_iDiagrammType != SPO2_GRAPH)
	{
		DrawYLegend(pDCStatic);
	}
	
	//Sleep(0);

	////step 10: write the x & y axes values
	DrawXAxisNumbers(pDCStatic);
	if(m_iDiagrammType != SPO2_GRAPH)
	{
		DrawYAxisNumbers(pDCStatic);
	}
	
	//Sleep(0);

	if(m_bAutofit /*&& m_iDiagrammType!=FOT_LOOP*/)
	{
		DrawAutoscaleState(pDCStatic);
		//Sleep(0);
	}

	BitBlt(m_hDC, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcStatic, 0, 0, SRCCOPY);
	BitBlt(dc.m_hDC, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcStatic, 0, 0, SRCCOPY);

	//Sleep(0);
	BitBlt(m_hdcGraph, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcStatic, 0, 0, SRCCOPY);
	
	/*if(getModel()->getVMODEHANDLER()->activeModeIsHFO()==false)
	{
		BitBlt(m_hdcLevel1, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcStatic, 0, 0, SRCCOPY);
		BitBlt(m_hdcLevel2, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcStatic, 0, 0, SRCCOPY);
	}
	else
	{
		BitBlt(m_hdcLevel1, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcStatic, 0, 0, SRCCOPY);
	}
	//Sleep(0);*/
	
	////Step 11
	////draw Function
	//DrawFunction();

	pDCStatic->SelectObject(oldpen);
	pDCStatic->SelectObject(poldbrush);
	penBack.DeleteObject();
	penTick.DeleteObject();
	//DeleteObject(brush);

	if(m_bDrawCursor)
		DrawCursor(m_bDrawCursor);

	
}

// **************************************************************************
// Draws the state "autoscale" on the top right side of the graph
//				if autoscale state is set.
// **************************************************************************
void CDiagramm::DrawAutoscaleState(CDC* pDC)
{
	if(m_bExit)
		return;

	//The graph title is drawn one character line down
	//centered left right between the left and right margins
	CRect rect,dataarea;
	dataarea=CalcDataArea();

	HFONT hPrevFont=(HFONT)pDC->SelectObject(g_hf7AcuNorm);
	CStringW szAuto=_T("  ")+getModel()->GetLanguageString(IDS_TXT_AUTOSCALE)+_T("  ");
	CSize sz1;
	sz1 = pDC->GetTextExtent(szAuto);

	rect.left=m_iGraphX+m_iGraphWidth-sz1.cx-1;
	rect.right=m_iGraphX+m_iGraphWidth-1;
	rect.top=dataarea.top-m_iCharHeight+1;
	rect.bottom=dataarea.top;

	//note we must clear of any old stuff crap fom this area
	CBrush brush;
	brush.CreateSolidBrush(m_crGraphBkColor);
	//brush.CreateSolidBrush(WHITE);
	//or bottom of a given rect
	pDC->FillRect(&rect,&brush);


	pDC->SetBkMode(OPAQUE);
	pDC->SetBkColor(m_crGraphAutostateBkColor);
	pDC->SetTextColor(m_crGraphAutostateColor);
	//pDC->DrawText(_T("   autoscale   "),&rect,DT_RIGHT|DT_END_ELLIPSIS);
	pDC->DrawText(szAuto,&rect,DT_RIGHT|DT_END_ELLIPSIS);


	//cleanup
	pDC->SelectObject(hPrevFont);

	//DeleteObject(brush);
}

// **************************************************************************
// The grid comprises two parts - the rectangle drawn around the plotting area
//				and the vertical&horizontal gridlines.
//				The outline rectangle is always drawn, the drawing of the gidlines
//				is contrlled by the m_bShowGrid member parameter using the ShowGrid function.
//				The grid color is  m_crGridColor.
// **************************************************************************
void CDiagramm::DrawGrid(CDC* pDC)
{
	if(m_bExit)
		return;
	/*
	Always draw the dataarea outline rectangle
	*/

	CRect dataarea=CalcDataArea();//where the graph data is actually drawn

	//need a pen of colour m_crGridColor
	CPen pen;
	CPen *poldpen=NULL;
	pen.CreatePen(PS_SOLID,1,m_crGridColor);

	poldpen=pDC->SelectObject(&pen);

	/* Now check whether the grid itself should
	be shown
	*/
	if(!m_bShowGrid)
	{
		pDC->SelectObject(poldpen);
		pen.DeleteObject();
		return;
	}

	//draw the X-axis gridlines
	//note x-axis grid lines run top tom bottom
	double GridSpacing=CalcXAxisGridAndTicks();
	if(GridSpacing<=0)
	{
		//ERROR todo
		GridSpacing=1;
		theApp.getLog()->WriteLine(_T("#HFO:0015"));
	}
	int n=1; //for the loop
	//draw positive xgrids
	UINT Graphright=m_iGraphX+m_iGraphWidth;
	int iXpixelsRight=Graphright-m_rmargin-m_iOriginX;
	while(GridSpacing*n<iXpixelsRight)
	{
		pDC->MoveTo((int)(m_iOriginX+GridSpacing*n) , (int)dataarea.top);
		pDC->LineTo((int)(m_iOriginX+GridSpacing*n),(int)dataarea.bottom);
		n++;
	}
	//draw negative xgrids
	int iXpixelsLeft=m_iOriginX-m_lmargin;
	n=1;
	while(GridSpacing*n<iXpixelsLeft)
	{
		pDC->MoveTo((int)(m_iOriginX+GridSpacing*n) , (int)dataarea.top);
		pDC->LineTo((int)(m_iOriginX+GridSpacing*n),(int)dataarea.bottom);
		n++;
	}

	//do the Y grid lines
	//note Y gridlines run left - right

	if(m_iDiagrammType != SPO2_GRAPH)
	{
		GridSpacing=CalcYAxisGridAndTicks();
		n=1; //for the loop
		//draw positive ygrids
		UINT Graphbottom=m_iGraphY+m_iGraphHeight;
		int iYpixelsTop=Graphbottom-m_tmargin-m_iOriginY;
		while(GridSpacing*n<iYpixelsTop)
		{
			pDC->MoveTo((int)dataarea.left ,(int)(Graphbottom-m_iOriginY-GridSpacing*n));
			pDC->LineTo((int)dataarea.right,(int)(Graphbottom-m_iOriginY-GridSpacing*n));
			n++;
		}
		//draw negative ygrids
		int iYpixelsBottom=m_iOriginY-m_bmargin;
		n=1;
		while(GridSpacing*n<iYpixelsBottom)
		{
			pDC->MoveTo((int)dataarea.left ,(int)(Graphbottom-m_iOriginY+GridSpacing*n));
			pDC->LineTo((int)dataarea.right,(int)(Graphbottom-m_iOriginY+GridSpacing*n));
			n++;
		}
	}
	

	pDC->SelectObject(poldpen);

	pen.DeleteObject();

}

// **************************************************************************
// DrawTicks does two things - it draws the x & Y axis lines and it also
//				draws the little 'tick' lines.
//				The axis lines are always shown but the 'ticks' are controlled by the
//				m_bShowTicks member (using the ShowTicks function);
//				The length of the ticks are set by the #define in the header file
//				the axis lines and ticks use the same color (m_crXTickColor for
//				the x-axis and m_crYTickColor for the y-axis)
// **************************************************************************
void CDiagramm::DrawTicks(CDC* pDC)
{
	if(m_bExit)
		return;

	//pretty much the same as showing the grid because the ticks and
	//tick align
	if (!m_bShowTicks)
	{
		return;
	}
	CPen xTickPen,yTickPen;
	
	CRect dataarea=CalcDataArea();
	UINT GraphBottom=m_iGraphY+m_iGraphHeight;

	//start with the x-axis
	xTickPen.CreatePen(PS_SOLID,1,m_crXTickColor);
	CPen *poldpen=pDC->SelectObject(&xTickPen);


	double GridSpacing=CalcXAxisGridAndTicks();
	//the ticklines vertically straddle the x-axis
	//two problems though - if the x-line is at or very close to the
	//top or bottom of the dataarea
	UINT xtoptick=((GraphBottom-m_iOriginY)-dataarea.top < G_TICKLENGTH/2)?(GraphBottom-m_iOriginY)-dataarea.top:G_TICKLENGTH/2;
	UINT xbottick=(dataarea.bottom-(GraphBottom-m_iOriginY) <G_TICKLENGTH/2)?dataarea.bottom-(GraphBottom-m_iOriginY):G_TICKLENGTH/2;
	int n=1;
	////draw positive xticks
	UINT Graphright=m_iGraphX+m_iGraphWidth;
	int iXpixelsRight=Graphright-m_rmargin-m_iOriginX;
	if(m_iDiagrammType==FOT_LOOP)
	{
		//tick at 0
		pDC->MoveTo(m_iOriginX,(int)(GraphBottom-m_bmargin-xbottick));
		pDC->LineTo(m_iOriginX,(int)(GraphBottom-m_bmargin+xbottick));

		while(GridSpacing*n<iXpixelsRight)
		{
			
			//loop and do the ticks
			//topticks
			pDC->MoveTo((int)(m_iOriginX+GridSpacing*n),(int)(GraphBottom-m_bmargin-xbottick));
			//bottom ticks
			pDC->LineTo((int)(m_iOriginX+GridSpacing*n),(int)(GraphBottom-m_bmargin+xbottick));
			n++;
		}
		////draw negative xticks
		int iXpixelsLeft=m_iOriginX-m_rmargin;
		n=1;
		while(GridSpacing*n<iXpixelsLeft)
		{
			pDC->MoveTo((int)(m_iOriginX-GridSpacing*n),(int)(GraphBottom-m_iOriginY));
			pDC->LineTo((int)(m_iOriginX-GridSpacing*n),(int)(GraphBottom-m_iOriginY-xbottick));
			//bottom ticks
			pDC->LineTo((int)(m_iOriginX-GridSpacing*n),(int)(GraphBottom-m_iOriginY+xbottick));
			n++;
		}
	}
	else
	{
		while(GridSpacing*n<iXpixelsRight)
		{
			//loop and do the ticks
			//topticks
			pDC->MoveTo((int)(m_iOriginX+GridSpacing*n),(int)(GraphBottom-m_iOriginY-xtoptick));
			//bottom ticks
			pDC->LineTo((int)(m_iOriginX+GridSpacing*n),(int)(GraphBottom-m_iOriginY+xbottick));
			n++;
		}
		////draw negative xticks
		int iXpixelsLeft=m_iOriginX-m_rmargin;
		n=1;
		while(GridSpacing*n<iXpixelsLeft)
		{
			pDC->MoveTo((int)(m_iOriginX-GridSpacing*n),(int)(GraphBottom-m_iOriginY));
			pDC->LineTo((int)(m_iOriginX-GridSpacing*n),(int)(GraphBottom-m_iOriginY-xtoptick));
			//bottom ticks
			pDC->LineTo((int)(m_iOriginX-GridSpacing*n),(int)(GraphBottom-m_iOriginY+xbottick));
			n++;
		}
	}
	



	//now do the y axis ticks	
	//pDC->SelectObject(poldpen);
	//TickPen.Detach();
	yTickPen.CreatePen(PS_SOLID,1,m_crYTickColor);
	pDC->SelectObject(&yTickPen);
	GridSpacing=CalcYAxisGridAndTicks();
	//the tick horizontally straddle the the Y axis
	//some problems though - if the y-axis is at or very close to the
	//left or right of the data area or if the Y-line memeber is set to left
	//handside
	UINT ylefttick=( (m_iGraphX+m_iOriginX)-dataarea.left <G_TICKLENGTH/2)? (m_iGraphX+m_iOriginX)-dataarea.left:G_TICKLENGTH/2;
	UINT yrighttick=( dataarea.right-(m_iGraphX+m_iOriginX) <G_TICKLENGTH/2)? dataarea.right-(m_iGraphX+m_iOriginX):G_TICKLENGTH/2;
	//check for the special case where the y-axis has been forced to the left
	if(m_bYLineAtLeft)
	{
		ylefttick=0;
	}

	int xPoint;
	if(m_bYLineAtLeft)
	{
		xPoint=dataarea.left;
	}
	else
	{
		xPoint=m_iGraphX+m_iOriginX;
	}

	n=1; //for the loop
	//draw positive yticks
	UINT Graphbottom=m_iGraphY+m_iGraphHeight;
	int iYpixelsTop=Graphbottom-m_tmargin-m_iOriginY;
	while(GridSpacing*n<iYpixelsTop)
	{
		pDC->MoveTo(xPoint,Graphbottom-m_iOriginY-(n*GridSpacing));
		//do left side tick
		pDC->LineTo(xPoint-ylefttick,Graphbottom-m_iOriginY-(n*GridSpacing));
		//do rightside tick
		pDC->LineTo(xPoint+yrighttick,Graphbottom-m_iOriginY-(n*GridSpacing));
		n++;
	}
	//draw negative yticks
	int iYpixelsBottom=m_iOriginY-m_bmargin;
	n=1;
	while(GridSpacing*n<iYpixelsBottom)
	{
		pDC->MoveTo(xPoint,Graphbottom-m_iOriginY+(n*GridSpacing));
		//do left side tick
		pDC->LineTo(xPoint-ylefttick,Graphbottom-m_iOriginY+(n*GridSpacing));
		//do rightside tick
		pDC->LineTo(xPoint+yrighttick,(int)(Graphbottom-m_iOriginY+(n*GridSpacing)));
		n++;
	}

	//cleanup
	pDC->SelectObject(poldpen);
	xTickPen.DeleteObject();
	yTickPen.DeleteObject();


}

// **************************************************************************
// 
// **************************************************************************
void CDiagramm::DrawCursor(bool state)
{
	if(m_bExit)
		return;

	if(m_bAutofit)
	{
		m_bDrawCursor=false;
		return;
	}

	m_bDrawCursor=state;

	CClientDC dc(this);

	//CBrush cbrBack(BACKGND);
	CBrush cbrBack(m_crGraphBkColor);
	//CBrush cbrBack(LIGHTGREEN);
	HBRUSH hbrprevClient=(HBRUSH)SelectObject(dc.m_hDC,cbrBack);
	HPEN hpenprevClient=(HPEN)SelectObject(dc.m_hDC, (HPEN)GetStockObject(NULL_PEN));
	

	switch(m_iDiagrammType)
	{
	case SPO2_GRAPH:
	case CO2_GRAPH:
	case CO2_HF_GRAPH:
	case PRESSURE_GRAPH:
	case PRESSURE_HF_GRAPH:
	case FLOW_GRAPH:
	case VOLUME_GRAPH:
	case FLOW_HF_GRAPH:
	case VOLUME_HF_GRAPH:
		{
			if(state)
			{
				m_pcCursor->Draw(dc.m_hDC,DIAGRAMM_WIDTH-31,2);
			}
			else
				Rectangle(dc.m_hDC, DIAGRAMM_WIDTH-31, 2, DIAGRAMM_WIDTH-1, 32);
		}
		break;
	case FOT_LOOP:
	case PRESSURE_VOLUME_HF_LOOP:
	case PRESSURE_VOLUME_LOOP:
	case VOLUME_FLOW_HF_LOOP:
	case VOLUME_FLOW_LOOP:
		{
			
			if(state)
			{
				m_pcCursor->Draw(dc.m_hDC,LOOP_WIDTH-31,2);
			}
			else
			{
				Rectangle(dc.m_hDC, LOOP_WIDTH-31, 1, LOOP_WIDTH-1, 32);
			}
			
		}
		break;
	default:
		{
			
		}
		break;
	}

	SelectObject(dc.m_hDC,hbrprevClient);
	SelectObject(dc.m_hDC,hpenprevClient);
}


// **************************************************************************
// This function calculates the actual plotting area of the graph
//				this is the graph area minus the top,bottom,left & right margins
//				Returns:
//				CRect with the plotting area (in client area co-ords)
// **************************************************************************
CRect CDiagramm::CalcDataArea()
{
	CRect dataarea;
	dataarea.left=m_iGraphX+m_lmargin;
	dataarea.right=m_iGraphX+m_iGraphWidth-m_rmargin;
	dataarea.top=m_iGraphY+m_tmargin;
	dataarea.bottom=m_iGraphY+m_iGraphHeight-m_bmargin;
	return dataarea;
}

// **************************************************************************
// 
// **************************************************************************
//int CDiagramm::GetRightPixelsOfXAxis()
//{
//	UINT Graphright=m_iGraphX+m_iGraphWidth;
//	int iXpixelsRight=Graphright-m_rmargin-m_iOriginX-1;
//
//	return iXpixelsRight;
//}

// **************************************************************************
// This function calculates the actual plotting area of the graph
//				this is the graph area minus the top,bottom,left & right margins
//				Returns:
//				CRect with the plotting area (in client area co-ords)
// **************************************************************************
//int CDiagramm::GetDataAreaWidth()
//{
//	int iWidth = m_iGraphWidth-m_rmargin-m_lmargin-1;
//
//	return iWidth;
//}

// **************************************************************************
// This function calculates the actual plotting area of the graph
//				this is the graph area minus the top,bottom,left & right margins
//				Returns:
//				CRect with the plotting area (in client area co-ords)
// **************************************************************************
//int CDiagramm::GetDataAreaHight()
//{
//	int iHight = m_iGraphHeight-m_bmargin-m_tmargin-1;
//
//	return iHight;
//}

// **************************************************************************
// This calculates the positions of the vertical gridlines of the plot area.
//				This is also used for x-axis ticks as the ticks line up with
//				the gridlines.
//				How many they are is determined by the G_NUMTICKSANDGRID  define
//				in the header file
//				Return:
//				a double denoting the x-asis grid spacing
// **************************************************************************
double CDiagramm::CalcXAxisGridAndTicks()
{
	//the placing of the ticks co-incide with gridlines
	CRect rect=CalcDataArea();

	double result=0;

	if(m_dXTickSpace==0)
	{
		result=((double)rect.right-(double)rect.left)/(double)G_NUMTICKSANDGRID;
	}
	else
	{
		if(m_dXAxisMax<(m_dXAxisMin*(-1)))//sollte zur Zeit nicht vorkommen
		{
			result=(m_iOriginX/(m_dXAxisMin*(-1)))*m_dXTickSpace;
		}
		else
		{
			UINT Graphright=m_iGraphX+m_iGraphWidth;
			int iXpixelsRight=Graphright-m_rmargin-m_iOriginX;

			result=(iXpixelsRight/m_dXAxisMax)*m_dXTickSpace;
		}
	}

	return result;

}

// **************************************************************************
// This calculates the positions of the vertical gridlines of the plot area.
//				This is also used for x-axis ticks as the ticks line up with
//				the gridlines.
//				How many they are is determined by the G_NUMTICKSANDGRID  define
//				in the header file
//				Return:
//				a double denoting the x-asis grid spacing
// **************************************************************************
double CDiagramm::CalcXAxisNumerics()
{
	//the placing of the ticks co-incide with gridlines
	CRect rect=CalcDataArea();

	double result=0;

	if(m_dXNumericSpace==0)
	{
		result=((double)rect.right-(double)rect.left)/(double)G_NUMTICKSANDGRID;
	}
	else
	{
		if(m_dXAxisMax<(m_dXAxisMin*(-1)))//sollte zur Zeit nicht vorkommen
		{
			result=(m_iOriginX/(m_dXAxisMin*(-1)))*m_dXNumericSpace;
		}
		else
		{
			UINT Graphright=m_iGraphX+m_iGraphWidth;
			int iXpixelsRight=Graphright-m_rmargin-m_iOriginX;

			result=(iXpixelsRight/m_dXAxisMax)*m_dXNumericSpace;
		}
	}

	return result;

}

// **************************************************************************
// 
// **************************************************************************
double CDiagramm::CalculateXAxisTickGridSpace()
{
	double result =0;
	double dDiff=m_dXAxisMax-m_dXAxisMin;

	switch(m_iDiagrammType)
	{
	case FOT_LOOP:
		{
			//PRESSURE
			if(dDiff<=10)
				result=1;
			else if(dDiff<=20)
				result=2;
			else if(dDiff<=40)
				result=5;
			else if(dDiff<=100)
				result=10;
			else 
				result=25;
		}
		break;
	case PRESSURE_VOLUME_LOOP:
		{
			//PRESSURE
			if(m_dXAxisMax<=10)
				result=1;
			else if(m_dXAxisMax<=20)
				result=2;
			else if(m_dXAxisMax<=40)
				result=5;
			else 
				result=10;
		}
		break;
	case PRESSURE_VOLUME_HF_LOOP:
		{
			//PRESSURE
			if(dDiff<=10)
				result=1;
			else if(dDiff<=20)
				result=2;
			else if(dDiff<=40)
				result=5;
			else if(dDiff<=100)
				result=10;
			else 
				result=25;
		}
		break;
	case VOLUME_FLOW_LOOP:
	case VOLUME_FLOW_HF_LOOP:
		{
			//VOLUME
			if(m_dXAxisMax<=10)
				result=1;
			else if(m_dXAxisMax<=20)
				result=2;
			/*else if(m_dXAxisMax<=30)
				result=5;*/
			else if(m_dXAxisMax<=40)
				result=5;
			/*else if(m_dXAxisMax<=60)
				result=10;
			else if(m_dXAxisMax<=80)
				result=10;*/
			else if(m_dXAxisMax<=100)
				result=10;
			else if(m_dXAxisMax<=200)
				result=25;
			/*else if(m_dXAxisMax<=300)
				result=50;
			else if(m_dXAxisMax<=400)
				result=50;*/
			else 
				result=50;
		}
		break;
	default:
		{

		}
		break;
	}

	return result;
}

// **************************************************************************
// 
// **************************************************************************
double CDiagramm::CalculateYAxisTickGridSpace()
{
	double result =0;
	double dDiff=m_dYAxisMax-m_dYAxisMin;

	switch(m_iDiagrammType)
	{
	case PRESSURE_GRAPH:
		{
			//PRESSURE
			if(m_dYAxisMax<=10)
				result=1;
			else if(m_dYAxisMax<=20)
				result=2;
			else if(m_dYAxisMax<=40)
				result=5;
			else 
				result=10;
		}
		break;
	case PRESSURE_HF_GRAPH:
		{
			//PRESSURE
			if(dDiff<=10)
				result=1;
			else if(dDiff<=20)
				result=2;
			else if(dDiff<=40)
				result=5;
			else if(dDiff<=100)
				result=10;
			else 
				result=25;
		}
		break;
	case PRESSURE_VOLUME_LOOP:
	case PRESSURE_VOLUME_HF_LOOP:
	case VOLUME_GRAPH:
	case VOLUME_HF_GRAPH:
	case CO2_GRAPH:
	case CO2_HF_GRAPH:
		{
			if(m_dYAxisMax<=10)
				result=1;
			else if(m_dYAxisMax<=20)
				result=2;
			else if(m_dYAxisMax<=40)
				result=5;
			else if(m_dYAxisMax<=100)
				result=10;
			else if(m_dYAxisMax<=200)
				result=25;
			else if(m_dYAxisMax<=400)
				result=50;
			else 
				result=100;
		}
		break;
	case FOT_LOOP://rku, check FOTGRAPH
		{
			double dSpan=abs(m_dYAxisMax-m_dYAxisMin);

			if(dSpan>=300)
				result=50;
			else if(dSpan>=200)
				result=25;
			else if(dSpan>=100)
				result=20;
			else if(dSpan>=40)
				result=10;
			else if(dSpan>=20)
				result=5;
			else 
				result=2;
			
			/*double test=m_dYAxisMax;
			if(m_dYAxisMin>=0)
				int iStop=0;

			if(m_dYAxisMin<=(-300))
				result=50;
			else if(m_dYAxisMin<=(-200))
				result=25;
			else if(m_dYAxisMin<=(-100))
				result=20;
			else if(m_dYAxisMin<=(-40))
				result=10;
			else if(m_dYAxisMin<=(-20))
				result=5;
			else 
				result=2;*/
		}
		break;
	case VOLUME_FLOW_LOOP:
	case VOLUME_FLOW_HF_LOOP:
	case FLOW_GRAPH:
	case FLOW_HF_GRAPH:
		{
			if(m_dYAxisMax<=(10/2))
				result=1;
			else if(m_dYAxisMax<=(20/2))
				result=2;
			else if(m_dYAxisMax<=(40/2))
				result=5;
			else 
				result=10;
		}
		break;
	case SPO2_GRAPH:
		{
			if(m_dYAxisMax<=10)
				result=1;
			else if(m_dYAxisMax<=20)
				result=2;
			else if(m_dYAxisMax<=40)
				result=5;
			else if(m_dYAxisMax<=100)
				result=10;
			else if(m_dYAxisMax<=200)
				result=25;
			else if(m_dYAxisMax<=400)
				result=50;
			else 
				result=100;
		}
		break;
	default:
		{

		}
		break;
	}

	return result;
}


// **************************************************************************
// 
// **************************************************************************
double CDiagramm::CalculateXAxisNumericSpace()
{
	double dDiff=m_dXAxisMax-m_dXAxisMin;
	double result =0;

	switch(m_iDiagrammType)
	{
	case PRESSURE_VOLUME_HF_LOOP:
		{
			if(dDiff<=10)
				result=1;
			else if(dDiff<=20)
				result=2;
			else if(dDiff<=40)
				result=5;
			else if(dDiff<=100)
				result=10;
			else 
				result=25;
		}
		break;
	case PRESSURE_VOLUME_LOOP:
		{
			//PRESSURE
			if(m_dXAxisMax<=10)
				result=2;
			else if(m_dXAxisMax<=20)
				result=4;
			else if(m_dXAxisMax<=30)
				result=5;
			else if(m_dXAxisMax<=60)
				result=10;
			else 
				result=20;
		}
		break;
	case VOLUME_FLOW_LOOP:
	case VOLUME_FLOW_HF_LOOP:
		{
			//VOLUME
			if(m_dXAxisMax<=5)
				result=1;
			else if(m_dXAxisMax<=10)
				result=2;
			else if(m_dXAxisMax<=20)
				result=4;
			else if(m_dXAxisMax<=30)
				result=5;
			/*else if(m_dXAxisMax<=40)
				result=10;*/
			else if(m_dXAxisMax<=60)
				result=10;
			/*else if(m_dXAxisMax<=80)
				result=20;*/
			else if(m_dXAxisMax<=100)
				result=20;
			else if(m_dXAxisMax<=200)
				result=50;
			/*else if(m_dXAxisMax<=300)
				result=100;
			else if(m_dXAxisMax<=400)
				result=100;*/
			else 
				result=100;
		}
		break;
	case FOT_LOOP:
		{
			if(dDiff<=10)
				result=1;
			else if(dDiff<=20)
				result=2;
			else if(dDiff<=40)
				result=5;
			else if(dDiff<=100)
				result=10;
			else 
				result=25;
		}
		break;
	default:
		{

		}
		break;
	}

	return result;
}

// **************************************************************************
// 
// **************************************************************************
double CDiagramm::CalculateYAxisNumericSpace()
{
	double result =0;
	double dDiff=m_dYAxisMax-m_dYAxisMin;

	switch(m_iDiagrammType)
	{
	case PRESSURE_GRAPH:
		{
			//PRESSURE
			if(m_dYAxisMax<=10)
				result=2;
			else if(m_dYAxisMax<=20)
				result=4;
			else if(m_dYAxisMax<=30)
				result=5;
			/*else if(m_dYAxisMax<=40)
			result=10;*/
			else if(m_dYAxisMax<=60)
				result=10;
			/*else if(m_dYAxisMax<=80)
			result=20;
			else if(m_dYAxisMax<=100)
			result=20;*/
			else 
				result=20;
		}
		break;
	case PRESSURE_HF_GRAPH:
		{
			//PRESSURE
			if(dDiff<=10)
				result=2;
			else if(dDiff<=20)
				result=4;
			else if(dDiff<=30)
				result=5;
			/*else if(dDiff<=40)
				result=10;*/
			else if(dDiff<=60)
				result=10;
			else if(dDiff<=100)
				result=20;
			else 
				result=25;
		}
		break;
	case PRESSURE_VOLUME_LOOP:
	case VOLUME_GRAPH:
	case PRESSURE_VOLUME_HF_LOOP:
	case VOLUME_HF_GRAPH:
	case CO2_GRAPH:
	case CO2_HF_GRAPH:
		{
			if(m_dYAxisMax<=5)
				result=1;
			else if(m_dYAxisMax<=10)
				result=2;
			else if(m_dYAxisMax<=20)
				result=4;
			else if(m_dYAxisMax<=30)
				result=5;
			else if(m_dYAxisMax<=60)
				result=10;
			else if(m_dYAxisMax<=100)
				result=20;
			else if(m_dYAxisMax<=200)
				result=50;
			else 
				result=100;
		}
		break;
	case FOT_LOOP://rku, check FOTGRAPH
		{
			double dSpan=abs(m_dYAxisMax-m_dYAxisMin);

			if(dSpan>=300)
				result=50;
			else if(dSpan>=200)
				result=25;
			else if(dSpan>=100)
				result=20;
			else if(dSpan>=40)
				result=10;
			else if(dSpan>=20)
				result=5;
			else 
				result=2;

			/*if(m_dYAxisMin<=(-300))
				result=50;
			else if(m_dYAxisMin<=(-200))
				result=25;
			else if(m_dYAxisMin<=(-100))
				result=20;
			else if(m_dYAxisMin<=(-40))
				result=10;
			else if(m_dYAxisMin<=(-20))
				result=5;
			else 
				result=2;*/
		}
		break;
	case VOLUME_FLOW_LOOP:
	case VOLUME_FLOW_HF_LOOP:
	case FLOW_GRAPH:
	case FLOW_HF_GRAPH:
		{
			if(m_dYAxisMax<=(10/2))
				result=2;
			else if(m_dYAxisMax<=(20/2))
				result=4;
			else if(m_dYAxisMax<=(30/2))
				result=5;
			else if(m_dYAxisMax<=(60/2))
				result=10;
			else 
				result=20;
		}
		break;
	case SPO2_GRAPH:
		{
			if(m_dYAxisMax<=5)
				result=1;
			else if(m_dYAxisMax<=10)
				result=2;
			else if(m_dYAxisMax<=20)
				result=4;
			else if(m_dYAxisMax<=30)
				result=5;
			else if(m_dYAxisMax<=60)
				result=10;
			else if(m_dYAxisMax<=100)
				result=20;
			else if(m_dYAxisMax<=200)
				result=50;
			else 
				result=100;
		}
		break;
	default:
		{

		}
		break;
	}

	return result;
}




// **************************************************************************
// 
// **************************************************************************
void CDiagramm::SetXAxisTickSpace(double dXTickSpace)
{
	m_dXTickSpace=dXTickSpace;
}

// **************************************************************************
// 
// **************************************************************************
double CDiagramm::GetXAxisTickSpace()
{
	return m_dXTickSpace;
}

// **************************************************************************
// 
// **************************************************************************
void CDiagramm::SetXAxisNumericSpace(double dXNumericSpace)
{
	m_dXNumericSpace=dXNumericSpace;
}

// **************************************************************************
// 
// **************************************************************************
double CDiagramm::GetXAxisNumericSpace()
{
	return m_dXNumericSpace;
}


// **************************************************************************
// 
// **************************************************************************
void CDiagramm::SetYAxisTickSpace(double dYTickSpace)
{
	m_dYTickSpace=dYTickSpace;
}

// **************************************************************************
// 
// **************************************************************************
//double CDiagramm::GetYAxisTickSpace()
//{
//	return m_dYTickSpace;
//}

// **************************************************************************
// 
// **************************************************************************
void CDiagramm::SetYAxisNumericSpace(double dYNumericSpace)
{
	m_dYNumericSpace=dYNumericSpace;
}

// **************************************************************************
// 
// **************************************************************************
double CDiagramm::GetYAxisNumericSpace()
{
	return m_dYNumericSpace;
}


// **************************************************************************
// This calculates the positions of the horizontal gridlines of the plot area.
//				This is also used for y-axis ticks as the ticks line up with
//				the gridlines.
//				How many they are is determined by the G_NUMTICKSANDGRID  define
//				in the header file
//				Return:
//				A double denoting the howizontal gridline spacing
// **************************************************************************
double CDiagramm::CalcYAxisGridAndTicks()
{
	////the placing of the ticks co-incide with gridlines
	//CRect rect=CalcDataArea();
	//return ((double)rect.bottom-(double)rect.top)/(double)G_NUMTICKSANDGRID;

	//the placing of the ticks co-incide with gridlines
	CRect rect=CalcDataArea();

	double result=0;

	if(m_dYTickSpace==0)
	{
		result=((double)rect.bottom-(double)rect.top)/(double)G_NUMTICKSANDGRID;
	}
	else
	{
		//if(m_dYAxisMax<(m_dYAxisMin*(-1)))//sollte zur Zeit nicht vorkommen
		//{
		//	//result=(m_iOriginY/(m_dYAxisMin*(-1)))*m_dYTickSpace;
		//	UINT Graphbottom=m_iGraphY+m_iGraphHeight;
		//	int iYpixelsRight=Graphbottom-m_tmargin-m_bmargin;

		//	result=(iYpixelsRight/fabs(m_dYAxisMin))*m_dYTickSpace;
		//}
		//else
		{
			UINT Graphbottom=m_iGraphY+m_iGraphHeight;
			int iYpixelsRight=Graphbottom-m_tmargin-m_iOriginY;

			result=(iYpixelsRight/m_dYAxisMax)*m_dYTickSpace;
		}
	}

	return result;
}

// **************************************************************************
// This calculates the positions of the horizontal gridlines of the plot area.
//				This is also used for y-axis ticks as the ticks line up with
//				the gridlines.
//				How many they are is determined by the G_NUMTICKSANDGRID  define
//				in the header file
//				Return:
//				A double denoting the howizontal gridline spacing
// **************************************************************************
double CDiagramm::CalcYAxisNumerics()
{
	////the placing of the ticks co-incide with gridlines
	//CRect rect=CalcDataArea();
	//return ((double)rect.bottom-(double)rect.top)/(double)G_NUMTICKSANDGRID;

	//the placing of the ticks co-incide with gridlines
	CRect rect=CalcDataArea();

	double result=0;

	if(m_dYNumericSpace==0)
	{
		result=((double)rect.bottom-(double)rect.top)/(double)G_NUMTICKSANDGRID;
	}
	else
	{
		//if(m_dYAxisMax<(m_dYAxisMin*(-1)))//sollte zur Zeit nicht vorkommen
		//{
		//	UINT Graphbottom=m_iGraphY+m_iGraphHeight;
		//	int iYpixelsRight=Graphbottom-m_tmargin-m_bmargin;

		//	result=(iYpixelsRight/fabs(m_dYAxisMin))*m_dYTickSpace;

		//	//result=(m_iOriginY/(m_dYAxisMin*(-1)))*m_dYNumericSpace;
		//}
		//else
		{
			UINT Graphbottom=m_iGraphY+m_iGraphHeight;
			int iYpixelsRight=Graphbottom-m_tmargin-m_iOriginY;

			result=(iYpixelsRight/m_dYAxisMax)*m_dYNumericSpace;
		}
	}

	return result;
}



// **************************************************************************
// The Xaxis legend is drawn below the plotting area
//				below the x-axis scale numbers
// **************************************************************************
void CDiagramm::DrawXLegend(CDC* pDC)
{
	if(m_bExit)
		return;
	//The x legend is drawn  below the data area
	CRect rect,dataarea;

	dataarea=CalcDataArea();

	switch(m_iDiagrammType)
	{
	case PRESSURE_VOLUME_LOOP:
	case PRESSURE_VOLUME_HF_LOOP:
	case VOLUME_FLOW_LOOP:
	case VOLUME_FLOW_HF_LOOP:
	case FOT_LOOP:
		{
			rect.right=m_iGraphX+m_iGraphWidth-4;
			rect.left=rect.right-15*m_iCharWidth;
			rect.top=m_iGraphY+m_iGraphHeight-m_iCharHeight-2;
			rect.bottom=rect.top+m_iCharHeight;
		}
		break;
	default:
		{
			rect.right=m_iGraphX+m_iGraphWidth-4;
			rect.left=rect.right-6*m_iCharWidth;
			rect.top=m_iGraphY+m_iGraphHeight-m_iCharHeight-2;
			rect.bottom=rect.top+m_iCharHeight;
		}
		break;
	}


	//note we must clear of any old stuff crap fom this area
	CBrush brush;
	brush.CreateSolidBrush(m_crGraphBkColor);
	//brush.CreateSolidBrush(RGB(230,230,230));
	pDC->FillRect(&rect,&brush);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_crXLegendTextColor);
	HFONT hPrevFont=(HFONT)pDC->SelectObject(g_hf6AcuNorm);

	switch(m_iDiagrammType)
	{
	case PRESSURE_VOLUME_LOOP:
	case PRESSURE_VOLUME_HF_LOOP:
		{
			pDC->DrawText(m_szXLegendText,&rect,DT_RIGHT|DT_END_ELLIPSIS);
		}
		break;
	case VOLUME_FLOW_LOOP:
	case VOLUME_FLOW_HF_LOOP:
		{
			pDC->DrawText(m_szXLegendText,&rect,DT_RIGHT|DT_END_ELLIPSIS);
		}
		break;
	default:
		{
			pDC->DrawText(m_szXLegendText,&rect,DT_RIGHT|DT_END_ELLIPSIS);
		}
		break;
	}

	//cleanup
	pDC->SelectObject(hPrevFont);

	//DeleteObject(brush);
}

// **************************************************************************
// The Y legend is drawn at the LHS of the plot area between the yaxis max 
//				and yaxis min scale
// **************************************************************************
void CDiagramm::DrawYLegend(CDC* pDC)
{
	if(m_bExit)
		return;
	//The Y legend is drawn down the left hand side just
	//outside the data area
	CRect rect,dataarea;

	dataarea=CalcDataArea();

	HFONT hPrevFont=(HFONT)pDC->SelectObject(g_hf6AcuNorm);
	CSize sz;
	sz = pDC->GetTextExtent(m_szYLegendText);

	//rect.left=m_iOriginX-8*m_iCharWidth;
	//rect.right=rect.left+16*m_iCharWidth;
	rect.left=m_iOriginX+1*m_iCharWidth;
	rect.right=rect.left+15*m_iCharWidth;
	bool bRight=false;
	if(rect.right>=dataarea.right)
	{
		int iDiff=rect.right-dataarea.right;
		rect.left-=iDiff;
		rect.right=dataarea.right-1;
		bRight=true;
	}
	rect.top=dataarea.top-m_iCharHeight-1+2;
	rect.bottom=dataarea.top-1;
	


	//note we must clear of any old stuff crap fom this area
	CBrush brush;
	brush.CreateSolidBrush(m_crGraphBkColor);
	//brush.CreateSolidBrush(m_crGraphPenColor);	//for test rku
	//or bottom of a given rect
	pDC->FillRect(&rect,&brush);


	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_crYLegendTextColor);
	if(bRight)
		pDC->DrawText(m_szYLegendText,&rect,DT_RIGHT|DT_END_ELLIPSIS);
	else
		pDC->DrawText(m_szYLegendText,&rect,DT_LEFT|DT_END_ELLIPSIS);


	//cleanup
	pDC->SelectObject(hPrevFont);

	//DeleteObject(brush);


}

// **************************************************************************
// The X axis scale numbers are drawm directly below the plot area.
//				Three numbers are drawn, min, middle and max scaling
// **************************************************************************
void CDiagramm::DrawXAxisNumbers(CDC* pDC)
{
	if(m_bExit)
		return;
	//
	//HDC hdc = *pDC;
	CStringW astring;
	CRect rect;
	HFONT hPrevFont=(HFONT)pDC->SelectObject(g_hf14AcuNormNum);
	pDC->SetTextColor(m_crXLegendTextColor);
	pDC->SetBkMode(TRANSPARENT);

	CBrush brush;
	brush.CreateSolidBrush(m_crGraphBkColor);
	//brush.CreateSolidBrush(LIGHTGREEN);
	//HPEN hpenprev=(HPEN)SelectObject(hdc, (HPEN)GetStockObject(NULL_PEN));


	CRect dataarea=CalcDataArea();//where the graph data is actually drawn

	double GridSpacing=0;
	double XAxisTickSpace=0;

	switch(m_iDiagrammType)
	{
	case PRESSURE_VOLUME_LOOP:
	case PRESSURE_VOLUME_HF_LOOP:
	case VOLUME_FLOW_LOOP:
	case VOLUME_FLOW_HF_LOOP:
	case FOT_LOOP:
		{
			GridSpacing=CalcXAxisNumerics();
			XAxisTickSpace=GetXAxisNumericSpace();

		}
		break;
	default:
		{
			GridSpacing=CalcXAxisGridAndTicks();
			XAxisTickSpace=GetXAxisTickSpace();
		}
		break;
	}

	/*CString szTest=_T("");
	szTest.Format(_T("%0.2f"), XAxisTickSpace);
	DEBUGMSG(TRUE, (TEXT("XAxisTickSpace =  %s\r\n"),szTest));*/
	
	//the ticklines vertically straddle the x-axis
	//two problems though - if the x-line is at or very close to the
	//top or bottom of the dataarea
	int n=0;
	////draw positive xticks
	UINT Graphright=m_iGraphX+m_iGraphWidth;
	int iXpixelsRight=Graphright-m_rmargin-m_iOriginX;

	switch(m_iDiagrammType)
	{
	case PRESSURE_VOLUME_LOOP:
	case PRESSURE_VOLUME_HF_LOOP:
	case VOLUME_FLOW_LOOP:
	case VOLUME_FLOW_HF_LOOP:
		{
			rect.top=dataarea.bottom-2;
			rect.bottom=rect.top+m_iCharHeight;
			rect.top+=1;
			rect.bottom+=1;
			while(GridSpacing*n<iXpixelsRight)
			{
				rect.left=m_iOriginX+GridSpacing*n-3*m_iCharWidth-2;
				rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters

				if(rect.right>=Graphright)
				{
					rect.right=Graphright-1;
				}
				
				pDC->FillRect(&rect,&brush);

				if(XAxisTickSpace*n<1.0)
					astring.Format(_T("%.2g"),XAxisTickSpace*n);
				else
					astring.Format(_T("%.3g"),XAxisTickSpace*n);
				pDC->DrawText(astring,&rect,DT_NOCLIP|DT_CENTER);
				n++;
			}
		}
		break;
	case FOT_LOOP:
		{
			rect.top=dataarea.bottom;
			rect.bottom=rect.top+m_iCharHeight;
			rect.top+=8;
			rect.bottom+=8;
			while(GridSpacing*n<iXpixelsRight)
			{
				rect.left=m_iOriginX+GridSpacing*n-3*m_iCharWidth-2;
				rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters

				if(rect.right>=Graphright)
				{
					rect.right=Graphright-1;
				}

				pDC->FillRect(&rect,&brush);

				if(XAxisTickSpace*n<1.0)
					astring.Format(_T("%.2g"),XAxisTickSpace*n);
				else
					astring.Format(_T("%.3g"),XAxisTickSpace*n);
				pDC->DrawText(astring,&rect,DT_NOCLIP|DT_CENTER);
				n++;
			}
		}
		break;
	default:
		{
			rect.top=m_iGraphY+m_iGraphHeight-m_iCharHeight-2;
			rect.bottom=rect.top+m_iCharHeight;
			rect.top+=1;
			while(GridSpacing*n<iXpixelsRight)
			{
				rect.left=(LONG)(m_iOriginX+GridSpacing*n-2*m_iCharWidth);
				rect.right=rect.left+4*m_iCharWidth;//allow for 4 characters
				//rect.bottom+=1;

				pDC->FillRect(&rect,&brush);

				if(XAxisTickSpace>=10000)
				{
					if(n==0)
					{
						astring=_T("");
					}
					else if(XAxisTickSpace*n/1000<1.0)
						astring.Format(_T("%.1g"),XAxisTickSpace*n/1000);
					else if(XAxisTickSpace*n/1000<60.0)
					{
						astring.Format(_T("%.2g"),XAxisTickSpace*n/1000);
					}
					else
					{
						astring.Format(_T("%.3g"),XAxisTickSpace*n/1000);
					}
				}
				else
				{
					if(n==0)
					{
						astring=_T("");
					}
					else if(XAxisTickSpace*n/1000<1.0)
						astring.Format(_T("%.1g"),XAxisTickSpace*n/1000);
					else if(XAxisTickSpace*n/1000<60.0)
						astring.Format(_T("%.2g"),XAxisTickSpace*n/1000);
					else
					{
						astring.Format(_T("%.3g"),XAxisTickSpace*n/1000);
					}
				}
				pDC->DrawText(astring,&rect,DT_NOCLIP|DT_CENTER);
				n++;
			}
		}
		break;
	}
	
	////draw negative xticks
	int iXpixelsLeft=m_iOriginX-m_rmargin;
	n=1;

	switch(m_iDiagrammType)
	{
	case PRESSURE_VOLUME_LOOP:
	case PRESSURE_VOLUME_HF_LOOP:
		{
			rect.top=dataarea.bottom;
			rect.bottom=rect.top+m_iCharHeight;
			rect.top+=1;
			rect.bottom+=1;
			while(GridSpacing*n<iXpixelsLeft)
			{
				rect.left=(LONG)(m_iOriginX-GridSpacing*n-3*m_iCharWidth);
				if(rect.left<=0)
					rect.left=1;
				rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters
				
				pDC->FillRect(&rect,&brush);
				if((XAxisTickSpace*n)*(-1)<=-100)
					astring.Format(_T("%.3g"),(XAxisTickSpace*n)*(-1));
				else
					astring.Format(_T("%.2g"),(XAxisTickSpace*n)*(-1));

				pDC->DrawText(astring,&rect,DT_NOCLIP|DT_CENTER);
				n++;
			}
		}
		break;
	case FOT_LOOP:
		{
			rect.top=dataarea.bottom;
			rect.bottom=rect.top+m_iCharHeight;
			rect.top+=8;
			rect.bottom+=8;
			while(GridSpacing*n<iXpixelsLeft)
			{
				rect.left=(LONG)(m_iOriginX-GridSpacing*n-3*m_iCharWidth);
				if(rect.left<=0)
					rect.left=1;
				rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters

				pDC->FillRect(&rect,&brush);
				if((XAxisTickSpace*n)*(-1)<=-100)
					astring.Format(_T("%.3g"),(XAxisTickSpace*n)*(-1));
				else
					astring.Format(_T("%.2g"),(XAxisTickSpace*n)*(-1));

				pDC->DrawText(astring,&rect,DT_NOCLIP|DT_CENTER);
				n++;
			}
		}
		break;
	case VOLUME_FLOW_LOOP:
	case VOLUME_FLOW_HF_LOOP:
		{
			rect.top=m_iGraphY+m_iGraphHeight-1*m_iCharHeight;
			rect.bottom=rect.top+m_iCharHeight;
			rect.top+=1;
			rect.bottom+=1;
			while(GridSpacing*n<iXpixelsLeft)
			{
				rect.left=(LONG)(m_iOriginX-GridSpacing*n-3*m_iCharWidth);
				if(rect.left<=0)
					rect.left=1;
				rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters
				
				pDC->FillRect(&rect,&brush);
				astring.Format(_T("%.2g"),(XAxisTickSpace*n/1000)*(-1));

				pDC->DrawText(astring,&rect,DT_NOCLIP|DT_CENTER);
				n++;
			}
		}
		break;
	default:
		{
			rect.top=m_iGraphY+m_iGraphHeight-m_iCharHeight-1;
			rect.bottom=rect.top+m_iCharHeight;
			while(GridSpacing*n<iXpixelsLeft)
			{
				rect.left=(LONG)(m_iOriginX-GridSpacing*n-3*m_iCharWidth);
				rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters
				
				pDC->FillRect(&rect,&brush);
				astring.Format(_T("%.2g"),(XAxisTickSpace*n/1000)*(-1));

				pDC->DrawText(astring,&rect,DT_NOCLIP|DT_CENTER);
				n++;
			}
		}
		break;
	}



	//cleanup
	pDC->SelectObject(hPrevFont);
	//SelectObject(hdc, hpenprev);

	//DeleteObject(brush);
}

// **************************************************************************
// The Y axis scale is drawn on the LHS of the plot area.
//				Only two numbers are drawn - min and max to allow for the Y-axis
//				legend.
// **************************************************************************
void CDiagramm::DrawYAxisNumbers(CDC* pDC)
{
	if(m_bExit)
		return;

	
	//we will only do two sets of numbers min and max because
	//if we print the halfway point it will cross the yaxis label
	CStringW astring;
	CRect rect;
	HFONT hPrevFont=(HFONT)pDC->SelectObject(g_hf14AcuNormNum);
	pDC->SetTextColor(m_crYLegendTextColor);
	pDC->SetBkMode(TRANSPARENT);

	CBrush brush;
	brush.CreateSolidBrush(m_crGraphBkColor);

	CRect dataarea=CalcDataArea();//where the graph data is actually drawn

	double GridSpacing=CalcYAxisNumerics();
	double YAxisTickSpace=GetYAxisNumericSpace();
	//the ticklines vertically straddle the x-axis
	//two problems though - if the x-line is at or very close to the
	//top or bottom of the dataarea
	int n=0;
	////draw positive yticks
	UINT Graphbottom=m_iGraphY+m_iGraphHeight;
	int iYpixelsTop=Graphbottom-m_tmargin-m_iOriginY;

	rect.left=m_iGraphX+1;
	if(m_iDiagrammType==FOT_LOOP)
	{
		rect.right=m_lmargin;
	}
	else
	{
		rect.right=m_lmargin-2;
	}

	while((GridSpacing*n+(m_iCharHeight/2))<=(iYpixelsTop+(m_iCharHeight/2)))
	{
		////loop and do the ticks
		//do the left side (min) left aligned
		//format and print the number
		rect.top=(LONG)(Graphbottom-m_iOriginY-GridSpacing*n-m_iCharHeight/2);
		rect.bottom=rect.top+m_iCharHeight;

		pDC->FillRect(&rect,&brush);

		if(YAxisTickSpace*n<1.0)
			astring.Format(_T("%.2g"),YAxisTickSpace*n);
		else
			astring.Format(_T("%.3g"),YAxisTickSpace*n);

		pDC->DrawText(astring,&rect,DT_NOCLIP|DT_RIGHT);
		n++;
	}
	//draw negative xticks
	int iYpixelsBottom=m_iOriginY-m_bmargin;
	n=1;

	rect.left=m_iGraphX+1;
	if(m_iDiagrammType==FOT_LOOP)
	{
		rect.right=m_lmargin;
	}
	else
	{
		rect.right=m_lmargin-2;
	}
	while(GridSpacing*n<=iYpixelsBottom)
	{
		rect.top=(LONG)(Graphbottom-m_iOriginY+GridSpacing*n-m_iCharHeight/2);
		rect.bottom=rect.top+m_iCharHeight;

		pDC->FillRect(&rect,&brush);
		//format and print the number
		if(YAxisTickSpace*n<1.0)
			astring.Format(_T("%.2g"),YAxisTickSpace*n*(-1));
		else
			astring.Format(_T("%.3g"),YAxisTickSpace*n*(-1));
		pDC->DrawText(astring,&rect,DT_NOCLIP|DT_RIGHT);

		n++;
	}

	//cleanup
	pDC->SelectObject(hPrevFont);

	//DeleteObject(brush);
}






// **************************************************************************
// 
// **************************************************************************
void CDiagramm::SetDefaultXLegend()
{
	m_szXLegendText="";
}

// **************************************************************************
// 
// **************************************************************************
void CDiagramm::SetDefaultYLegend()
{
	m_szYLegendText="";
}

// **************************************************************************
// Sets the X axis legend text
// **************************************************************************
//void CDiagramm::SetXLegendText(CStringW XText)
//{
//	CDC* pDCStatic=CDC::FromHandle(m_hdcStatic);
//	m_szXLegendText=XText;
//	DrawXLegend(pDCStatic);
//}


// **************************************************************************
// Sets the Y axis legend text
// **************************************************************************
//void CDiagramm::SetYLegendText(CStringW YText)
//{
//	CDC* pDCStatic=CDC::FromHandle(m_hdcStatic);
//	m_szYLegendText=YText;
//	DrawYLegend(pDCStatic);
//}




// **************************************************************************
// The Y axis line can be forced to the LHS of the 
//				plot area using this function
// **************************************************************************
//void CDiagramm::SetYLineAtLeft(BOOL AtLeft)
//{
//	BOOL bprevious=m_bYLineAtLeft;
//	m_bYLineAtLeft=AtLeft;
//	//if there is a change in the Y line position then we will have to
//	//redraw the graph
//	if (m_bYLineAtLeft != bprevious)
//	{
//		PaintGraph();
//	}
//}

// **************************************************************************
// This switches the grid on or off as set by the bShow parameter
//				The graph is repainted to match
// **************************************************************************
//void CDiagramm::ShowGrid(BOOL bShow)
//{
//	//this is public function
//	//show the graph grid if bShow==TRUE
//	//or vice-versa
//	BOOL bprevious=m_bShowGrid;
//	m_bShowGrid=bShow;
//	//if there is a change then repaint
//	if (m_bShowGrid !=bprevious)
//	{
//		PaintGraph();
//	}
//}

// **************************************************************************
// This switches the x and y axis 'ticks' on or off as set by the
//				bShow parameter
// **************************************************************************
//void CDiagramm::ShowTicks(BOOL bShow)
//{
//	//this is public function
//	//show the graph grid if bShow==TRUE
//	//or vice-versa
//	BOOL bprevious=m_bShowTicks;
//	m_bShowTicks=bShow;
//	//if there is a change then repaint
//	if (m_bShowTicks!=bprevious)
//	{
//		PaintGraph();
//	}
//
//}

// **************************************************************************
// ConvertToGraphCoords (double x, double y) will take the	
//				result of some calculation as given by x and y and return where				
//				they should be plotted on the graph.				
//				As we are can only plot a whole pixel the return value is of	
//				type LONG (as apposed to double).						
//				The Y-pixel is in the HIWORD and x-pixel in the LOWORD of the	
//				return.													
//				Note that depending on the scale of the graph, the return coords	
//				of a single pixel could be one of many.						
//				For example: if the x axis is 400 pixels wide, but is scaled to		
//				represent 1000, then each pixel represent 2.5 in the real world.	
//				So to pixel 398 represent the real world values of 995 to 996.5 inc.
// **************************************************************************
LONG CDiagramm::ConvertToGraphCoords(double x, double y)
{

	LONG result =-1;
	//to be plottable on the graph the given x-value must be between
	//x-min and x-max
	if(x < m_dXAxisMin || x > m_dXAxisMax)
	{
		return result;
	}
	if( y <m_dYAxisMin || y > m_dYAxisMax)
	{
		return result;
	}
	//calc the abs difference between Xmin and x;
	double xdif = abs((int)(m_dXAxisMin - x));
	//calc the abs difference between Ymin and y;
	double ydif = abs((int)(m_dYAxisMin - y));

	//find the dataarea
	CRect rect=CalcDataArea();

	int xpos=(int)(rect.left+(xdif*m_dPixelsPerX)); //from left
	int ypos=(int)(rect.bottom-(ydif*m_dPixelsPerY)); //from bottom

	result=MAKELONG(xpos,ypos);	


	return result;
}


// **************************************************************************
// The user fills in a G_FUNCTIONSTRUCT relevant to a function and passes	
//				a pointer to it to this function.				
//				The pointer to this G_FUNCTIONSTRUCT is saved in a member variable
//				This function does some preliminary stuff  and if there are no	
//				obvious problems it then calls the PaintGraph function.			
//
//				Returns:								
//				FALSE if there are no problems
// **************************************************************************
BOOL CDiagramm::DoFunction(G_FUNCTIONSTRUCT_LD *pFunctionParams)
{
	if(m_bExit)
		return FALSE;
	//do some checks first to see if good data has been passed
	if(pFunctionParams->pItems==NULL)
	{
		return FALSE;
	}
	
	if(pFunctionParams)
		m_pFunctionParams=pFunctionParams;
	DrawFunction();
	return TRUE;
}


// **************************************************************************
// This resets the G_FUNCTIONSTRUCT member pointer		
//		The graph is repainteed (cleared)
// **************************************************************************
void CDiagramm::ClearFunction(bool bRedraw)
{
	if(m_bExit)
		return;
	m_bFirstpoint=TRUE;

	if(bRedraw)
	{
		PaintGraph();
	}
}


// **************************************************************************
// This is called from within the PaintGraph routine to draw	
//				the actual function onto the graph.				
//				This functions just switches the FunctionType member specified in the
//				G_FUNCTIONSTRUCT and calls the appropriate routine.
// **************************************************************************
void CDiagramm::DrawFunction()
{
	if(m_bExit)
		return;

	if(!m_pFunctionParams)
	{
		return;
	}

	switch (m_pFunctionParams->FuncType)
	{
	case G_PLOTXYPOINTS:
		DoPlotXYPoints();
		break;
	case G_PLOTSAVEDLOOPPOINTS:
		DoPlotSavedLoopPoints();
		break;
	case G_PLOTXYLOOPPOINTS:
		DoPlotXYLoopPoints();
		break;
	case G_PLOTYPOINTS:
		DoPlotYPoints();
		break;
	default:
		break;
	}
}

// **************************************************************************
// This has many similarities with the other functions - however		
//				there is no need to calculate Y  in the sane way as in the other function
//				as it is given.											
//				a particular x point on the graph is also given - we place the	
//				given y value at the given x point							
//				Other:														
//				a. We will not bother plotting  if x < the graph scale XMin or > XMax
//				b. Only dot, bar and line will be acceptable for the chart type. Any other
//				will default to bar. However be aware that line is only suitable if the	
//				x-values are in sequence.						
//				c. The usual y constraints apply
// **************************************************************************
//void CDiagramm::setChartType(UINT chartType)
//{
//	if(chartType!=m_pFunctionParams->ChartType)
//		m_pFunctionParams->ChartType=chartType;
//}

// **************************************************************************
// This has many similarities with the other functions - however		
//				there is no need to calculate Y  in the sane way as in the other function
//				as it is given.											
//				a particular x point on the graph is also given - we place the	
//				given y value at the given x point							
//				Other:														
//				a. We will not bother plotting  if x < the graph scale XMin or > XMax
//				b. Only dot, bar and line will be acceptable for the chart type. Any other
//				will default to bar. However be aware that line is only suitable if the	
//				x-values are in sequence.						
//				c. The usual y constraints apply
// **************************************************************************
void CDiagramm::DoPlotXYLoopPoints()
{
	if(m_bExit)
		return;

	if(!m_pFunctionParams)
	{
		return;
	}

	LONG result;
	UINT xstart;
	UINT ystart;
	if(m_dPixelsPerX==0 || m_dPixelsPerY==0)
	{
		return;
	}
	if(m_pFunctionParams->dXfactor==0 || m_pFunctionParams->dYfactor==0)
	{
		return;
	}
	double xperpixel= 1/m_dPixelsPerX;
	double yperpixel=1/m_dPixelsPerY;

	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen

	/*UINT Graphbottom=m_iGraphY+m_iGraphHeight;
	UINT Graphright=m_iGraphX+m_iGraphWidth;*/

	switch(m_pFunctionParams->ChartType)
	{
	case G_FOTLOOP:
	case G_LOOPCHART:
		{
			int iPointsCount=0;
			UINT pixelx=0;
			UINT pixely=0;
			bool bNewBreath=false;

			if(!m_bFirstpoint)//begin of graph at x=0
			{
				ptsLoop[iPointsCount].x=m_prevx;
				ptsLoop[iPointsCount].y=m_prevy;
				iPointsCount++;
			}
			
            if(m_pFunctionParams->num_PlotXYItems>G_MAXPOINTS-1)
				m_pFunctionParams->num_PlotXYItems=G_MAXPOINTS-1;

			for (UINT count =0; count < m_pFunctionParams->num_PlotXYItems; count++)
			{
				double x=m_pFunctionParams->pItems[count].pPlotXYItems.x/m_pFunctionParams->dXfactor;
				double y=m_pFunctionParams->pItems[count].pPlotXYItems.y/m_pFunctionParams->dYfactor;

				if(m_pFunctionParams->pItems[count].bNewATZ)
					bNewBreath=true;

				m_bFirstpoint=FALSE;

				if(x>m_dMaxX)
				{
					m_dMaxX=x;
				}
				if(x<m_dMinX)
				{
					m_dMinX=x;
				}
				if(y>m_dMaxY)
				{
					m_dMaxY=y;//
				}
				if(y<m_dMinY)
				{
					m_dMinY=y;
				}

				pixelx= (UINT)(xstart+(x-m_dXAxisMin)/xperpixel);

				y=ConstrainY(y);	

				pixely=(UINT)(ystart-(y-m_dYAxisMin)/yperpixel);

				ptsLoop[iPointsCount].x=pixelx;
				ptsLoop[iPointsCount].y=pixely;
				iPointsCount++;

				if(m_bFREEZE)
					break;

				if(m_bExit)
					return;
			}
			//current point becomes previous point
			m_prevx=pixelx;
			m_prevy=pixely;

			if(iPointsCount>1)
			{
				PlotPointArray(ptsLoop,iPointsCount,true);
			}
			

			if(m_bAutofit && (m_bIgnoreAutofit==false) && bNewBreath)
			{
				CheckAutoScaleXYdecrease();
			}
			else if(m_bAutofit && (m_bIgnoreAutofit==false))
			{
				CheckAutoScaleXYincrease();
			}

			break;
		}
	default:
		{
			break;
		}
	}//SWITCH
}

// **************************************************************************
// This has many similarities with the other functions - however		
//				there is no need to calculate Y  in the sane way as in the other function
//				as it is given.											
//				a particular x point on the graph is also given - we place the	
//				given y value at the given x point							
//				Other:														
//				a. We will not bother plotting  if x < the graph scale XMin or > XMax
//				b. Only dot, bar and line will be acceptable for the chart type. Any other
//				will default to bar. However be aware that line is only suitable if the	
//				x-values are in sequence.						
//				c. The usual y constraints apply
// **************************************************************************
void CDiagramm::DoPlotSavedLoopPoints()
{
	if(m_bExit)
		return;

	if(!m_pFunctionParams)
	{
		return;
	}

	LONG result;
	UINT xstart;
	UINT ystart;
	if(m_dPixelsPerX==0 || m_dPixelsPerY==0)
	{
		return;
	}
	if(m_pFunctionParams->dXfactor==0 || m_pFunctionParams->dYfactor==0)
	{
		return;
	}
	double xperpixel= 1/m_dPixelsPerX;
	double yperpixel=1/m_dPixelsPerY;

	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen

	/*UINT Graphbottom=m_iGraphY+m_iGraphHeight;
	UINT Graphright=m_iGraphX+m_iGraphWidth;*/

	switch(m_pFunctionParams->ChartType)
	{
	case G_FOTLOOP:
	   {
		   //const int iNum = m_pFunctionParams->num_PlotXYItems;
		   int iPointsCount=0;
		   UINT pixelx=0;
		   UINT pixely=0;

		   //bool bNewBreath=false;

		   if(m_pFunctionParams->num_PlotXYItems>G_MAXPOINTS-1)
			   m_pFunctionParams->num_PlotXYItems=G_MAXPOINTS-1;


		   for (UINT count =0; count < m_pFunctionParams->num_PlotXYItems; count++)
		   {
			   double x=m_pFunctionParams->pItems[count].pPlotXYItems.x/m_pFunctionParams->dXfactor;
			   double y=m_pFunctionParams->pItems[count].pPlotXYItems.y/m_pFunctionParams->dYfactor;

			   m_bFirstpoint=FALSE;

			  /* if(x>m_dMaxX)
			   {
				   m_dMaxX=x;
			   }
			   if(x<m_dMinX)
			   {
				   m_dMinX=x;
			   }*/
			  /* if(y>m_dMaxY)
			   {
				   m_dMaxY=y;
			   }*/
			   
			   
			   //if(y<m_dMaxY)//rku, check FOTGRAPH
			   //{
				  // m_dMaxY=y;
			   //}
			   //neu
			   if(y>m_dMaxY)
			   {
				   m_dMaxY=y;
			   }
			   if(y<m_dMinY)
			   {
				   m_dMinY=y;
			   }

			   DEBUGMSG(TRUE, (TEXT("m_dMaxY %d\r\n"),(int)m_dMaxY));
			   DEBUGMSG(TRUE, (TEXT("m_dMinY %d\r\n"),(int)m_dMinY));
			   DEBUGMSG(TRUE, (TEXT("Y %d\r\n"),(int)y));

			   pixelx= (UINT)(xstart+(x-m_dXAxisMin)/xperpixel);

			   y=ConstrainY(y);	
			   pixely=(UINT)(ystart-(y-m_dYAxisMin)/yperpixel);

			   ptsLoop[iPointsCount].x=pixelx;
			   ptsLoop[iPointsCount].y=pixely;
			   iPointsCount++;

			   if(m_bExit)
				   return;

		   }
		   //current point becomes previous point
		   m_prevx=pixelx;
		   m_prevy=pixely;

		   if(iPointsCount>0)
		   {
			   PlotPointArray(ptsLoop,iPointsCount);
		   }

		   //if(m_bAutofit && (m_bIgnoreAutofit==false))
		   {
			   CheckAutoScaleY();
			   //Sleep(0);
		   }
		   break;
	   }
	case G_LOOPCHART:
		{
			//const int iNum = m_pFunctionParams->num_PlotXYItems;
			int iPointsCount=0;
			UINT pixelx=0;
			UINT pixely=0;

			//bool bNewBreath=false;

			if(m_pFunctionParams->num_PlotXYItems>G_MAXPOINTS-1)
				m_pFunctionParams->num_PlotXYItems=G_MAXPOINTS-1;


			for (UINT count =0; count < m_pFunctionParams->num_PlotXYItems; count++)
			{
				double x=m_pFunctionParams->pItems[count].pPlotXYItems.x/m_pFunctionParams->dXfactor;
				double y=m_pFunctionParams->pItems[count].pPlotXYItems.y/m_pFunctionParams->dYfactor;

				m_bFirstpoint=FALSE;

				pixelx= (UINT)(xstart+(x-m_dXAxisMin)/xperpixel);

				y=ConstrainY(y);	
				pixely=(UINT)(ystart-(y-m_dYAxisMin)/yperpixel);

				ptsLoop[iPointsCount].x=pixelx;
				ptsLoop[iPointsCount].y=pixely;
				iPointsCount++;

				if(m_bExit)
					return;

			}
			//current point becomes previous point
			m_prevx=pixelx;
			m_prevy=pixely;

			if(iPointsCount>1)
			{
				PlotPointArray(ptsLoop,iPointsCount);
			}


			break;
		}
	default:
		{
			break;
		}
	}//SWITCH
}

// **************************************************************************
// This has many similarities with the other functions - however		
//				there is no need to calculate Y  in the sane way as in the other function
//				as it is given.											
//				a particular x point on the graph is also given - we place the	
//				given y value at the given x point							
//				Other:														
//				a. We will not bother plotting  if x < the graph scale XMin or > XMax
//				b. Only dot, bar and line will be acceptable for the chart type. Any other
//				will default to bar. However be aware that line is only suitable if the	
//				x-values are in sequence.						
//				c. The usual y constraints apply
// **************************************************************************
void CDiagramm::DoPlotXYPoints()
{
	if(m_bExit)
		return;

	if(!m_pFunctionParams)
	{
		return;
	}

	LONG result;
	UINT xstart;
	UINT ystart;
	if(m_dPixelsPerX==0 || m_dPixelsPerY==0)
	{
		return;
	}
	if(m_pFunctionParams->dXfactor==0 || m_pFunctionParams->dYfactor==0)
	{
		return;
	}
	double xperpixel= 1/m_dPixelsPerX;
	double yperpixel=1/m_dPixelsPerY;

	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen

	/*UINT Graphbottom=m_iGraphY+m_iGraphHeight;
	UINT Graphright=m_iGraphX+m_iGraphWidth;*/

	switch(m_pFunctionParams->ChartType)
	{
	case G_FOTLOOP:
	case G_LOOPCHART:
		{
			//const int iNum = m_pFunctionParams->num_PlotXYItems;

			int iPointsCount=0;
			UINT pixelx=0;
			UINT pixely=0;

			bool bNewBreath=false;

			ptsLine[iPointsCount].x=m_prevx;
			ptsLine[iPointsCount].y=m_prevy;
			iPointsCount++;

			if(m_pFunctionParams->num_PlotXYItems>G_MAXPOINTS-1)
				m_pFunctionParams->num_PlotXYItems=G_MAXPOINTS-1;

			for (UINT count =0; count < m_pFunctionParams->num_PlotXYItems; count++)
			{

				double x=m_pFunctionParams->pItems[count].pPlotXYItems.x/m_pFunctionParams->dXfactor;
				double y=m_pFunctionParams->pItems[count].pPlotXYItems.y/m_pFunctionParams->dYfactor;

				if(m_pFunctionParams->pItems[count].bNewATZ)
					bNewBreath=true;


				if(x>m_dMaxX)
				{
					m_dMaxX=x;
				}
				if(x<m_dMinX)
				{
					m_dMinX=x;
				}
				if(y>m_dMaxY)
				{
					m_dMaxY=y;
				}
				if(y<m_dMinY)
				{
					m_dMinY=y;
				}
				

				//if x is off scale - don't bother
				if ( (x < m_dXAxisMin) || (x > m_dXAxisMax))
				{
					//Sleep(0);
					continue; //NEXT !!!!!!
				}

				pixelx= (UINT)(xstart+(x-m_dXAxisMin)/xperpixel);

				y=ConstrainY(y);	
				pixely=(UINT)(ystart-(y-m_dYAxisMin)/yperpixel);


				ptsLine[iPointsCount].x=pixelx;
				ptsLine[iPointsCount].y=pixely;
				iPointsCount++;

				if(m_bFREEZE)
					break;

				if(m_bExit)
					return;
			}
			//current point becomes previous point
			m_prevx=pixelx;
			m_prevy=pixely;


			PlotPointArray(ptsLine,iPointsCount);

			if(m_bAutofit && (m_bIgnoreAutofit==false) && bNewBreath)
			{
				CheckAutoScaleXYdecrease();
			}
			else if(m_bAutofit && (m_bIgnoreAutofit==false))
			{
				CheckAutoScaleXYincrease();
			}

			break;
		}
	default:
		{
			break;
		}
	}//SWITCH
}




// **************************************************************************
// This has many similarities with the other functions - however		
//				there is no need to calculate Y  in the sane way as in the other function
//				as it is given.											
//				a particular x point on the graph is also given - we place the	
//				given y value at the given x point							
//				Other:														
//				a. We will not bother plotting  if x < the graph scale XMin or > XMax
//				b. Only dot, bar and line will be acceptable for the chart type. Any other
//				will default to bar. However be aware that line is only suitable if the	
//				x-values are in sequence.						
//				c. The usual y constraints apply
// **************************************************************************
void CDiagramm::DoPlotYPoints()
{
	if(m_bExit)
		return;

	if(!m_pFunctionParams)
	{
		return;
	}

	LONG result;
	UINT xstart;
	UINT ystart;
	//double xperpixel= 1/m_dPixelsPerX;
	//double xperpixel= 1;
	double yperpixel=0;

	UINT Graphbottom=0;
	UINT Graphright=0;

	switch(m_pFunctionParams->ChartType)
	{
	case G_LINECHART_REFRESH:
		{
			RefreshLineChart();
			
		}
		break;
	case G_FILLEDCHART_REFRESH:
		{
			RefreshFilledChart();
			
		}
		break;
	case G_LINECHART:
		{
			yperpixel=1/m_dPixelsPerY;

			result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
			xstart=LOWORD(result);//the left hand side of the graph plot area on screen
			ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen

			Graphbottom=m_iGraphY+m_iGraphHeight;
			Graphright=m_iGraphX+m_iGraphWidth;

			int iPointsCount=0;
			
			UINT pixelx=0;
			UINT pixely=0;

			if(m_bFirstpoint)//begin of graph at x=0
			{
				pixelx=1+m_iOriginX;
				ptsLine[iPointsCount].x=pixelx;
				ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
				iPointsCount++;
			}
			else
			{
				ptsLine[iPointsCount].x=m_prevx;
				ptsLine[iPointsCount].y=m_prevy;
				iPointsCount++;
			}

			if(m_pFunctionParams->num_PlotXYItems>G_MAXPOINTS-1)
				m_pFunctionParams->num_PlotXYItems=G_MAXPOINTS-1;

			for (UINT count =0; count < m_pFunctionParams->num_PlotXYItems; count++)
			{
				if(count>=G_MAXPOINTS)
				{
					break;
				}

				double x=m_pFunctionParams->pItems[count].pPlotXYItems.x;
				double y=m_pFunctionParams->pItems[count].pPlotXYItems.y/m_pFunctionParams->dYfactor;

				if(y>m_dMaxY)
				{
					m_dMaxY=y;
				}
				if(y<m_dMinY)
				{
					m_dMinY=y;
				}

			
				if(x==1 && m_bFirstpoint==FALSE)
				{

					if(count>0)
					{
						PlotPointArray(ptsLine,iPointsCount);

						if(m_bAutofit && (m_bIgnoreAutofit==false))
						{
							//werte sind hier immer 0 !!!!!!!!
							CheckAutoScaleY();
						}

						if(getModel()->getVMODEHANDLER()->activeModeIsTrigger())
						{
							if(m_pFunctionParams->pItems[count].bTrig && (m_bTriggeredBreath==false))
							{
								SetTriggeredBreath(true);
							}
							else if((m_pFunctionParams->pItems[count].bTrig==false) 
								//&&	(m_pFunctionParams->pItems[count].bNewATZ==true)
								&&	(m_bTriggeredBreath==true))
							{
								SetTriggeredBreath(false);
							}
						}
						else
							m_bTriggeredBreath=false;
					}

					iPointsCount=0;
				}

				m_bFirstpoint=FALSE;
			
				pixelx= (UINT)(x+m_iOriginX);

				y=ConstrainY(y);	
				pixely=(UINT)(ystart-(y-m_dYAxisMin)/yperpixel);

				ptsLine[iPointsCount].x=pixelx;
				ptsLine[iPointsCount].y=pixely;
				iPointsCount++;

				if(iPointsCount>30)
				{
					PlotPointArray(ptsLine,iPointsCount);

					if(m_bAutofit && (m_bIgnoreAutofit==false))
					{
						CheckAutoScaleY();
					}

					iPointsCount=0;

					ptsLine[iPointsCount].x=pixelx;
					ptsLine[iPointsCount].y=pixely;
					iPointsCount++;

				}

				if(getModel()->getVMODEHANDLER()->activeModeIsTrigger())
				{
					if(		(m_pFunctionParams->pItems[count].bTrig==true) 
						&&	(m_bTriggeredBreath==false))
					{
						PlotPointArray(ptsLine,iPointsCount);

						if(m_bAutofit && (m_bIgnoreAutofit==false))
						{
							CheckAutoScaleY();
						}

						iPointsCount=0;

						ptsLine[iPointsCount].x=pixelx;
						ptsLine[iPointsCount].y=pixely;
						iPointsCount++;

						SetTriggeredBreath(true);
					}
					else if(	(m_pFunctionParams->pItems[count].bTrig==false) 
						//&&	(m_pFunctionParams->pItems[count].bNewATZ==true)
						&&	(m_bTriggeredBreath==true))
					{
						PlotPointArray(ptsLine,iPointsCount);

						if(m_bAutofit && (m_bIgnoreAutofit==false))
						{
							CheckAutoScaleY();
						}

						iPointsCount=0;

						ptsLine[iPointsCount].x=pixelx;
						ptsLine[iPointsCount].y=pixely;
						iPointsCount++;

						SetTriggeredBreath(false);
					}
				}
				else
					m_bTriggeredBreath=false;

				if(m_bExit)
					return;
			}

			//current point becomes previous point
			m_prevx=pixelx;
			m_prevy=pixely;

			PlotPointArray(ptsLine,iPointsCount);

			if(m_bAutofit && (m_bIgnoreAutofit==false))
			{
				CheckAutoScaleY();
			}
			break;
		}
	case G_FILLEDCHART:
		{
			yperpixel=1/m_dPixelsPerY;

			result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
			xstart=LOWORD(result);//the left hand side of the graph plot area on screen
			ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen



			Graphbottom=m_iGraphY+m_iGraphHeight;
			//Graphright=m_iGraphX+m_iGraphWidth;

			int iPointsCount=0;
			//int ites=m_iOriginX;
			UINT pixelx=1+m_iOriginX;
			UINT pixely=0;

			if(m_bFirstpoint)//begin of graph at x=0
			{
				pixelx=1+/*xstart+*/m_iOriginX;
				ptsLine[iPointsCount].x=pixelx;
				ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
				iPointsCount++;
			}
			else
			{
				ptsLine[iPointsCount].x=m_prevx;
				ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
				iPointsCount++;

				ptsLine[iPointsCount].x=m_prevx;
				ptsLine[iPointsCount].y=m_prevy;
				iPointsCount++;
			}

			if(m_pFunctionParams->num_PlotXYItems>G_MAXPOINTS-1)
				m_pFunctionParams->num_PlotXYItems=G_MAXPOINTS-1;

			for (UINT count =0; count < m_pFunctionParams->num_PlotXYItems; count++)
			{
				if(count>=G_MAXPOINTS || iPointsCount>=G_MAXPOINTS)
				{
					break;
				}


				double x=m_pFunctionParams->pItems[count].pPlotXYItems.x;

				if(x==1 && m_bFirstpoint==FALSE)
				{
					/*m_dMaxYcomplete=0;
					m_dMinYcomplete=0;*/
					

					if(count!=0)
					{
						
						ptsLine[iPointsCount].x=pixelx;
						ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
						iPointsCount++;

						PlotPointArray(ptsLine,iPointsCount);

						if(m_bAutofit && (m_bIgnoreAutofit==false))
						{
							//dMaxY + dMinY ist hier immer 0 !!!!!!!!!!!!
							CheckAutoScaleY();
						}

						if(getModel()->getVMODEHANDLER()->activeModeIsTrigger())
						{
							if(m_pFunctionParams->pItems[count].bTrig && (m_bTriggeredBreath==false))
							{
								SetTriggeredBreath(true);
							}
							else if((m_pFunctionParams->pItems[count].bTrig==false) 
								//&&	(m_pFunctionParams->pItems[count].bNewATZ==true)
								&&	(m_bTriggeredBreath==true))
							{
								SetTriggeredBreath(false);
							}
						}
						else
							m_bTriggeredBreath=false;
					}

					iPointsCount=0;

					pixelx= 1+m_iOriginX;
					ptsLine[iPointsCount].x=pixelx;
					ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
					iPointsCount++;
				}

				m_bFirstpoint=FALSE;

				double y=m_pFunctionParams->pItems[count].pPlotXYItems.y/m_pFunctionParams->dYfactor;

				/*if(y>m_dMaxYcomplete)
				{
					m_dMaxYcomplete=y;
				}
				if(y<m_dMinYcomplete)
				{
					m_dMinYcomplete=y;
				}*/

				if(y>m_dMaxY)
				{
					m_dMaxY=y;
				}
				if(y<m_dMinY)
				{
					m_dMinY=y;
				}
				
				pixelx= (UINT)(x+m_iOriginX);

				y=ConstrainY(y);	
				pixely=(UINT)(ystart-(y-m_dYAxisMin)/yperpixel);

				ptsLine[iPointsCount].x=pixelx;
				ptsLine[iPointsCount].y=pixely;
				iPointsCount++;

				if(iPointsCount>30)
				{
					ptsLine[iPointsCount].x=pixelx;
					ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
					iPointsCount++;

					m_prevx=pixelx;
					m_prevy=pixely;

					PlotPointArray(ptsLine,iPointsCount);

					if(m_bAutofit && (m_bIgnoreAutofit==false))
					{
						CheckAutoScaleY();
					}

					iPointsCount=0;

					ptsLine[iPointsCount].x=m_prevx;
					ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
					iPointsCount++;

					ptsLine[iPointsCount].x=m_prevx;
					ptsLine[iPointsCount].y=m_prevy;
					iPointsCount++;

				}

				if(getModel()->getVMODEHANDLER()->activeModeIsTrigger())
				{
					if(		(m_pFunctionParams->pItems[count].bTrig==true) 
						/*&&	(m_pFunctionParams->pItems[count].bNewATZ==true)*/
						&&	(m_bTriggeredBreath==false))
					{
						ptsLine[iPointsCount].x=pixelx;
						ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
						iPointsCount++;

						m_prevx=pixelx;
						m_prevy=pixely;

						PlotPointArray(ptsLine,iPointsCount);

						if(m_bAutofit && (m_bIgnoreAutofit==false))
						{
							CheckAutoScaleY();
						}

						iPointsCount=0;

						ptsLine[iPointsCount].x=m_prevx;
						ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
						iPointsCount++;

						ptsLine[iPointsCount].x=m_prevx;
						ptsLine[iPointsCount].y=m_prevy;
						iPointsCount++;

						SetTriggeredBreath(true);
					}
					else if(	(m_pFunctionParams->pItems[count].bTrig==false) 
						//&&	(m_pFunctionParams->pItems[count].bNewATZ==true)
						&&	(m_bTriggeredBreath==true))
					{
						ptsLine[iPointsCount].x=pixelx;
						ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
						iPointsCount++;

						m_prevx=pixelx;
						m_prevy=pixely;

						PlotPointArray(ptsLine,iPointsCount);

						if(m_bAutofit && (m_bIgnoreAutofit==false))
						{
							CheckAutoScaleY();
						}

						iPointsCount=0;

						ptsLine[iPointsCount].x=m_prevx;
						ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
						iPointsCount++;

						ptsLine[iPointsCount].x=m_prevx;
						ptsLine[iPointsCount].y=m_prevy;
						iPointsCount++;

						SetTriggeredBreath(false);
					}
				}
				else
					m_bTriggeredBreath=false;

				

				if(m_bExit)
					return;
			}
			//current point becomes previous point
			m_prevx=pixelx;
			m_prevy=pixely;

			ptsLine[iPointsCount].x=pixelx;
			ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;

			iPointsCount++;

			PlotPointArray(ptsLine,iPointsCount);

			if(m_bAutofit && (m_bIgnoreAutofit==false))
			{
				CheckAutoScaleY();
			}

			break;
		}
	default:
		{
			break;
		}
	}//SWITCH
}

// **************************************************************************
// 
// **************************************************************************
void CDiagramm::RefreshLineChart()
{
	if(m_bExit)
		return;

	if(!m_pFunctionParams)
	{
		return;
	}

	CClientDC dc(this);
	CDC* pDCGraph=CDC::FromHandle(m_hdcGraph);

	CPen penGreen,penGrey,cursorpen,penTrig,penLimit,penGarant;
	CPen *poldpenGraph=NULL;
	CPen *poldpenDC=NULL;
	//penGreen.CreatePen(PS_SOLID,m_iGraphPenSize,RGB(0,220,0));
	penGreen.CreatePen(PS_SOLID,m_iGraphPenSize,m_crTrigger);
	
	penGrey.CreatePen(PS_SOLID,m_iGraphPenSize,m_crCurrentGraphPenColor);
	//penGrey.CreatePen(PS_SOLID,m_iGraphPenSize,RGB(140,140,140));m_crGraphPenColor
	penTrig.CreatePen(PS_SOLID,m_iGraphPenSize,RGB(0,160,0));
	penLimit.CreatePen(PS_SOLID,1,RGB(255,90,0));
	penGarant.CreatePen(PS_SOLID,1,RGB(255,200,0));
	cursorpen.CreatePen(PS_SOLID,4,RGB(250,250,250));

	poldpenGraph=pDCGraph->SelectObject(&penGrey);
	HBRUSH holdbrush=(HBRUSH)pDCGraph->SelectObject((HBRUSH)GetStockObject(NULL_BRUSH));

	int iXleft=0;
	int iXright=0;
	int iWidth=0;

	double yperpixel=1/m_dPixelsPerY;
	LONG result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
	UINT xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	UINT ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
	UINT Graphbottom=m_iGraphY+m_iGraphHeight;
	UINT Graphright=m_iGraphX+m_iGraphWidth;



	int iPointsCount=0;
	
	UINT pixelx=0;
	UINT pixely=0;

	if(m_bFirstpoint)//begin of graph at x=0
	{
		pixelx=1+m_iOriginX;
		ptsLine[iPointsCount].x=pixelx;
		ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
		iPointsCount++;
	}
	else
	{
		ptsLine[iPointsCount].x=m_prevx;
		ptsLine[iPointsCount].y=m_prevy;
		iPointsCount++;
	}

	//int iT=m_pFunctionParams->num_PlotXYItems;
	if(m_pFunctionParams->num_PlotXYItems>G_MAXPOINTS-1)
		m_pFunctionParams->num_PlotXYItems=G_MAXPOINTS-1;


	for (UINT count =0; count < m_pFunctionParams->num_PlotXYItems; count++)
	{
		if(count>=G_MAXPOINTS)
		{
			//Sleep(0);
			break;
		}

		double x=m_pFunctionParams->pItems[count].pPlotXYItems.x;
		double y=m_pFunctionParams->pItems[count].pPlotXYItems.y/m_pFunctionParams->dYfactor;

		if(y>m_dMaxY)
		{
			m_dMaxY=y;
		}
		if(y<m_dMinY)
		{
			m_dMinY=y;
		}

		if(x==1 && m_bFirstpoint==FALSE)
		{

			if(count>0)
			{
				iXleft=ptsLine[0].x;
				iXright=ptsLine[iPointsCount-1].x;
				iWidth=iXright-iXleft;

				if(iWidth==0)
				{
					BitBlt(m_hdcGraph, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft, 0, SRCCOPY);
				}
				else
				{
					if(iXleft==25)
						BitBlt(m_hdcGraph, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft, 0, SRCCOPY);
					else
						BitBlt(m_hdcGraph, iXleft+1, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft+1, 0, SRCCOPY);
					Sleep(0);
					pDCGraph->Polyline(ptsLine, iPointsCount);
					//PlotPointArray(ptsLine,iPoints);
				}
				Sleep(0);

				if(m_bAutofit && (m_bIgnoreAutofit==false))
				{
					//werte sind hier immer 0 !!!!!!!!
					CheckAutoScaleY();
					Sleep(0);
				}

				if(getModel()->getVMODEHANDLER()->activeModeIsTrigger())
				{
					if(m_pFunctionParams->pItems[count].bTrig && (m_bTriggeredBreath==false))
					{
						m_bTriggeredBreath=true;
						pDCGraph->SelectObject(&penGreen);
					}
					else if((m_pFunctionParams->pItems[count].bTrig==false) 
						&&	(m_bTriggeredBreath==true))
					{
						m_bTriggeredBreath=false;
						pDCGraph->SelectObject(&penGrey);
					}
				}
				else
					m_bTriggeredBreath=false;
			}

			iPointsCount=0;
		}

		m_bFirstpoint=FALSE;

		pixelx= (UINT)(x+m_iOriginX);

		y=ConstrainY(y);	
		pixely=(UINT)(ystart-(y-m_dYAxisMin)/yperpixel);

		ptsLine[iPointsCount].x=pixelx;
		ptsLine[iPointsCount].y=pixely;
		iPointsCount++;

		if(iPointsCount>30)
		{
			iXleft=ptsLine[0].x;
			iXright=ptsLine[iPointsCount-1].x;
			iWidth=iXright-iXleft;

			if(iWidth==0)
			{
				BitBlt(m_hdcGraph, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft, 0, SRCCOPY);
			}
			else
			{
				if(iXleft==25)
					BitBlt(m_hdcGraph, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft, 0, SRCCOPY);
				else
					BitBlt(m_hdcGraph, iXleft+1, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft+1, 0, SRCCOPY);
				Sleep(0);
				pDCGraph->Polyline(ptsLine, iPointsCount);
				//PlotPointArray(ptsLine,iPoints);
			}
			Sleep(0);

			if(m_bAutofit && (m_bIgnoreAutofit==false))
			{
				CheckAutoScaleY();
				Sleep(0);
			}

			iPointsCount=0;

			ptsLine[iPointsCount].x=pixelx;
			ptsLine[iPointsCount].y=pixely;
			iPointsCount++;

		}

		if(getModel()->getVMODEHANDLER()->activeModeIsTrigger())
		{
			if(		(m_pFunctionParams->pItems[count].bTrig==true) 
				&&	(m_bTriggeredBreath==false))
			{
				iXleft=ptsLine[0].x;
				iXright=ptsLine[iPointsCount-1].x;
				iWidth=iXright-iXleft;

				if(iWidth==0)
				{
					BitBlt(m_hdcGraph, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft, 0, SRCCOPY);
				}
				else
				{
					if(iXleft==25)
						BitBlt(m_hdcGraph, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft, 0, SRCCOPY);
					else
						BitBlt(m_hdcGraph, iXleft+1, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft+1, 0, SRCCOPY);
					Sleep(0);
					pDCGraph->Polyline(ptsLine, iPointsCount);
					//PlotPointArray(ptsLine,iPoints);
				}
				Sleep(0);

				if(m_bAutofit && (m_bIgnoreAutofit==false))
				{
					CheckAutoScaleY();
					Sleep(0);
				}

				iPointsCount=0;

				ptsLine[iPointsCount].x=pixelx;
				ptsLine[iPointsCount].y=pixely;
				iPointsCount++;

				m_bTriggeredBreath=true;
				pDCGraph->SelectObject(&penGreen);
			}
			else if(	(m_pFunctionParams->pItems[count].bTrig==false) 
				&&	(m_bTriggeredBreath==true))
			{
				iXleft=ptsLine[0].x;
				iXright=ptsLine[iPointsCount-1].x;
				iWidth=iXright-iXleft;

				if(iWidth==0)
				{
					BitBlt(m_hdcGraph, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft, 0, SRCCOPY);
				}
				else
				{
					if(iXleft==25)
						BitBlt(m_hdcGraph, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft, 0, SRCCOPY);
					else
						BitBlt(m_hdcGraph, iXleft+1, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft+1, 0, SRCCOPY);
					Sleep(0);
					pDCGraph->Polyline(ptsLine, iPointsCount);
					//PlotPointArray(ptsLine,iPoints);
				}
				Sleep(0);

				if(m_bAutofit && (m_bIgnoreAutofit==false))
				{
					CheckAutoScaleY();
					Sleep(0);
				}

				iPointsCount=0;

				ptsLine[iPointsCount].x=pixelx;
				ptsLine[iPointsCount].y=pixely;
				iPointsCount++;

				m_bTriggeredBreath=false;
				pDCGraph->SelectObject(&penGrey);
			}
		}
		else
			m_bTriggeredBreath=false;

		if(m_bExit)
			return;

		Sleep(0);
	}

	//current point becomes previous point
	m_prevx=pixelx;
	m_prevy=pixely;

	iXleft=ptsLine[0].x;
	iXright=ptsLine[iPointsCount-1].x;
	iWidth=iXright-iXleft;

	if(iWidth==0)
	{
		BitBlt(m_hdcGraph, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft, 0, SRCCOPY);
	}
	else
	{
		if(iXleft==25)
			BitBlt(m_hdcGraph, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft, 0, SRCCOPY);
		else
			BitBlt(m_hdcGraph, iXleft+1, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft+1, 0, SRCCOPY);
		Sleep(0);
		pDCGraph->Polyline(ptsLine, iPointsCount);
		//PlotPointArray(ptsLine,iPoints);
	}
	Sleep(0);


	if(m_bAutofit && (m_bIgnoreAutofit==false))
	{
		CheckAutoScaleY();
		Sleep(0);
	}

	int iGraphXleft=1+m_iOriginX;
	int iGraphXright=Graphright;
	int iGraphWidth=iGraphXright-iGraphXleft;

	pDCGraph->SelectObject(&penLimit);

	////Draw the Voloume Limit
	if(m_bDrawVolumeLimit && m_iVolumeLimit>0)
	{
		pDCGraph->MoveTo(iGraphXleft,m_iVolumeLimit);
		pDCGraph->LineTo(iGraphXright,m_iVolumeLimit);
		Sleep(0);
	}

	////Draw the Voloume Garanty
	if(m_bDrawVolumeGaranty && m_iVolumeGaranty>0)
	{
		pDCGraph->SelectObject(&penGarant);
		pDCGraph->MoveTo(iGraphXleft,m_iVolumeGaranty);
		pDCGraph->LineTo(iGraphXright,m_iVolumeGaranty);
		Sleep(0);
	}
	if(m_bDrawHFOVolumeGarantyHigh)
	{
		pDCGraph->SelectObject(&penGarant);
		pDCGraph->MoveTo(iGraphXleft,m_iHFOVolumeGarantyHigh);
		pDCGraph->LineTo(iGraphXright,m_iHFOVolumeGarantyHigh);
		Sleep(0);
	}
	if(m_bDrawHFOVolumeGarantyLow)
	{
		pDCGraph->SelectObject(&penGarant);
		pDCGraph->MoveTo(iGraphXleft,m_iHFOVolumeGarantyLow);
		pDCGraph->LineTo(iGraphXright,m_iHFOVolumeGarantyLow);
		Sleep(0);
	}


	////Draw the TriggerVolume
	if(m_iTrigThreshold>0)
	{
		pDCGraph->SelectObject(&penTrig);
		pDCGraph->MoveTo(iGraphXleft,m_iTrigThreshold);
		pDCGraph->LineTo(iGraphXright,m_iTrigThreshold);
		Sleep(0);
	}


	BitBlt(dc.m_hDC, iGraphXleft, 0, iGraphWidth/*+2*/,m_iGraphHeight, m_hdcGraph,  iGraphXleft, 0, SRCCOPY);
	Sleep(0);

	if(m_bDrawCursor &&
		iXleft>=DIAGRAMM_WIDTH-31)
	{
		DrawCursor(m_bDrawCursor);
	}

	poldpenDC=dc.SelectObject(&cursorpen);
	if(iXleft+iWidth+3<(m_iGraphX+m_iGraphWidth-m_rmargin))
	{
		MoveToEx(dc.m_hDC, iXleft+iWidth+3, m_iGraphHeight-1, NULL);
		LineTo(dc.m_hDC, iXleft+iWidth+3, 1);
		Sleep(0);
	}

	pDCGraph->SelectObject(poldpenGraph);
	pDCGraph->SelectObject(holdbrush);
	dc.SelectObject(poldpenDC);

	penGreen.DeleteObject();
	penGrey.DeleteObject();
	cursorpen.DeleteObject();
	penTrig.DeleteObject();
	penLimit.DeleteObject();
	penGarant.DeleteObject();
}
// **************************************************************************
// 
// **************************************************************************
void CDiagramm::RefreshFilledChart()
{
	if(m_bExit)
		return;

	if(!m_pFunctionParams)
	{
		return;
	}

	CClientDC dc(this);
	CDC* pDCGraph=CDC::FromHandle(m_hdcGraph);

	CPen penGreen,penGrey,cursorpen,penTrig,penLimit,penGarant;
	CPen *poldpenGraph=NULL;
	CPen *poldpenDC=NULL;
	//penGreen.CreatePen(PS_SOLID,m_iGraphPenSize,RGB(0,220,0));
	penGreen.CreatePen(PS_SOLID,m_iGraphPenSize,m_crTrigger);
	//penGrey.CreatePen(PS_SOLID,m_iGraphPenSize,RGB(140,140,140));
	penGrey.CreatePen(PS_SOLID,m_iGraphPenSize,m_crCurrentGraphPenColor);
	cursorpen.CreatePen(PS_SOLID,4,RGB(250,250,250));
	penTrig.CreatePen(PS_SOLID,1,RGB(0,160,0));
	penLimit.CreatePen(PS_SOLID,1,RGB(255,90,0));
	penGarant.CreatePen(PS_SOLID,1,RGB(255,200,0));

	CBrush brushGreen, brushGrey;
	CBrush*poldbrush=NULL;
	//brushGreen.CreateSolidBrush(RGB(0,220,0));
	brushGreen.CreateSolidBrush(m_crTrigger);
	brushGrey.CreateSolidBrush(m_crCurrentGraphPenColor);
	//brushGrey.CreateSolidBrush(RGB(140,140,140));
	poldbrush=pDCGraph->SelectObject(&brushGrey);
	poldpenGraph=pDCGraph->SelectObject(&penGrey);

	

	int iXleft=0;
	int iXright=0;
	int iWidth=0;

	double yperpixel=1/m_dPixelsPerY;
	LONG result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
	UINT xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	UINT ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
	UINT Graphbottom=m_iGraphY+m_iGraphHeight;
	UINT Graphright=m_iGraphX+m_iGraphWidth;

	int iPointsCount=0;
	UINT pixelx=1+m_iOriginX;
	UINT pixely=0;

	int iGraphXleft=1+m_iOriginX;
	int iGraphXright=Graphright;
	int iGraphWidth=iGraphXright-iGraphXleft;

	if(m_bFirstpoint)//begin of graph at x=0
	{
		pixelx=1+/*xstart+*/m_iOriginX;
		ptsLine[iPointsCount].x=pixelx;
		ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
		iPointsCount++;
	}
	else
	{
		ptsLine[iPointsCount].x=m_prevx;
		ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
		iPointsCount++;

		ptsLine[iPointsCount].x=m_prevx;
		ptsLine[iPointsCount].y=m_prevy;
		iPointsCount++;
	}

	if(m_pFunctionParams->num_PlotXYItems>G_MAXPOINTS-1)
		m_pFunctionParams->num_PlotXYItems=G_MAXPOINTS-1;

	Sleep(0);
	BitBlt(m_hdcGraph, iGraphXleft, 0, iGraphWidth,m_iGraphHeight, m_hdcGraph,  iGraphXleft, 0, SRCCOPY);
	Sleep(0);

	for (UINT count =0; count < m_pFunctionParams->num_PlotXYItems; count++)
	{
		if(count>=G_MAXPOINTS)
		{
			Sleep(0);
			break;
		}

		double x=m_pFunctionParams->pItems[count].pPlotXYItems.x;

		if(x==1 && m_bFirstpoint==FALSE)
		{
			if(count!=0)
			{

				ptsLine[iPointsCount].x=pixelx;
				ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
				iPointsCount++;

				iXleft=ptsLine[0].x;
				iXright=ptsLine[iPointsCount-1].x;
				iWidth=iXright-iXleft;

				if(iWidth==0)
				{
					BitBlt(m_hdcGraph, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft, 0, SRCCOPY);
				}
				else
				{
					BitBlt(m_hdcGraph, iXleft+1, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft+1, 0, SRCCOPY);
					Sleep(0);

					pDCGraph->Polygon(ptsLine, iPointsCount);
					//PlotPointArray(ptsLine,iPoints);
				}

				
				Sleep(0);

				if(m_bAutofit && (m_bIgnoreAutofit==false))
				{
					//dMaxY + dMinY ist hier immer 0 !!!!!!!!!!!!
					CheckAutoScaleY();
					Sleep(0);
				}

				if(getModel()->getVMODEHANDLER()->activeModeIsTrigger())
				{
					if(m_pFunctionParams->pItems[count].bTrig && (m_bTriggeredBreath==false))
					{
						m_bTriggeredBreath=true;
						pDCGraph->SelectObject(&brushGreen);
						pDCGraph->SelectObject(&penGreen);
					}
					else if((m_pFunctionParams->pItems[count].bTrig==false) 
						&&	(m_bTriggeredBreath==true))
					{
						m_bTriggeredBreath=false;
						pDCGraph->SelectObject(&brushGrey);
						pDCGraph->SelectObject(&penGrey);
					}
				}
				else
					m_bTriggeredBreath=false;
			}

			iPointsCount=0;

			pixelx= 1+m_iOriginX;
			ptsLine[iPointsCount].x=pixelx;
			ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
			iPointsCount++;
		}

		m_bFirstpoint=FALSE;

		double y=m_pFunctionParams->pItems[count].pPlotXYItems.y/m_pFunctionParams->dYfactor;

		if(y>m_dMaxY)
		{
			m_dMaxY=y;
		}
		if(y<m_dMinY)
		{
			m_dMinY=y;
		}

		pixelx= (UINT)(x+m_iOriginX);

		y=ConstrainY(y);
		Sleep(0);
		pixely=(UINT)(ystart-(y-m_dYAxisMin)/yperpixel);

		ptsLine[iPointsCount].x=pixelx;
		ptsLine[iPointsCount].y=pixely;
		iPointsCount++;
		
		if(iPointsCount>30)
		{
			ptsLine[iPointsCount].x=pixelx;
			ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
			iPointsCount++;

			m_prevx=pixelx;
			m_prevy=pixely;

			iXleft=ptsLine[0].x;
			iXright=ptsLine[iPointsCount-1].x;
			iWidth=iXright-iXleft;

			if(iWidth==0)
			{
				BitBlt(m_hdcGraph, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft, 0, SRCCOPY);
			}
			else
			{
				BitBlt(m_hdcGraph, iXleft+1, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft+1, 0, SRCCOPY);
				Sleep(0);
				pDCGraph->Polygon(ptsLine, iPointsCount);
				//PlotPointArray(ptsLine,iPoints);
			}
			Sleep(0);

			if(m_bAutofit && (m_bIgnoreAutofit==false))
			{
				CheckAutoScaleY();
				Sleep(0);
			}

			iPointsCount=0;

			ptsLine[iPointsCount].x=m_prevx;
			ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
			iPointsCount++;

			ptsLine[iPointsCount].x=m_prevx;
			ptsLine[iPointsCount].y=m_prevy;
			iPointsCount++;

		}

		if(getModel()->getVMODEHANDLER()->activeModeIsTrigger())
		{
			if(		(m_pFunctionParams->pItems[count].bTrig==true) 
				&&	(m_bTriggeredBreath==false))
			{
				ptsLine[iPointsCount].x=pixelx;
				ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
				iPointsCount++;

				m_prevx=pixelx;
				m_prevy=pixely;

				iXleft=ptsLine[0].x;
				iXright=ptsLine[iPointsCount-1].x;
				iWidth=iXright-iXleft;

				if(iWidth==0)
				{
					BitBlt(m_hdcGraph, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft, 0, SRCCOPY);
				}
				else
				{
					BitBlt(m_hdcGraph, iXleft+1, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft+1, 0, SRCCOPY);
					Sleep(0);
					pDCGraph->Polygon(ptsLine, iPointsCount);
					//PlotPointArray(ptsLine,iPoints);
				}
				Sleep(0);

				if(m_bAutofit && (m_bIgnoreAutofit==false))
				{
					CheckAutoScaleY();
					Sleep(0);
				}

				iPointsCount=0;

				ptsLine[iPointsCount].x=m_prevx;
				ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
				iPointsCount++;

				ptsLine[iPointsCount].x=m_prevx;
				ptsLine[iPointsCount].y=m_prevy;
				iPointsCount++;

				m_bTriggeredBreath=true;
				pDCGraph->SelectObject(&brushGreen);
				pDCGraph->SelectObject(&penGreen);
			}
			else if(	(m_pFunctionParams->pItems[count].bTrig==false) 
				//&&	(m_pFunctionParams->pItems[count].bNewATZ==true)
				&&	(m_bTriggeredBreath==true))
			{
				ptsLine[iPointsCount].x=pixelx;
				ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
				iPointsCount++;

				m_prevx=pixelx;
				m_prevy=pixely;

				iXleft=ptsLine[0].x;
				iXright=ptsLine[iPointsCount-1].x;
				iWidth=iXright-iXleft;
				if(iWidth==0)
				{
					BitBlt(m_hdcGraph, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft, 0, SRCCOPY);
				}
				else
				{
					BitBlt(m_hdcGraph, iXleft+1, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft+1, 0, SRCCOPY);
					Sleep(0);
					pDCGraph->Polygon(ptsLine, iPointsCount);
					//PlotPointArray(ptsLine,iPoints);
				}
				Sleep(0);

				if(m_bAutofit && (m_bIgnoreAutofit==false))
				{
					CheckAutoScaleY();
					Sleep(0);
				}

				iPointsCount=0;

				ptsLine[iPointsCount].x=m_prevx;
				ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;
				iPointsCount++;

				ptsLine[iPointsCount].x=m_prevx;
				ptsLine[iPointsCount].y=m_prevy;
				iPointsCount++;

				m_bTriggeredBreath=false;
				pDCGraph->SelectObject(&brushGrey);
				pDCGraph->SelectObject(&penGrey);
			}
		}
		else
			m_bTriggeredBreath=false;

		


		if(m_bExit)
			return;

		Sleep(0);
	}
	//current point becomes previous point
	m_prevx=pixelx;
	m_prevy=pixely;

	ptsLine[iPointsCount].x=pixelx;
	ptsLine[iPointsCount].y=Graphbottom-m_iOriginY;

	iPointsCount++;

	iXleft=ptsLine[0].x;
	iXright=ptsLine[iPointsCount-1].x;
	iWidth=iXright-iXleft;

	
	if(iWidth==0)
	{
		BitBlt(m_hdcGraph, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft, 0, SRCCOPY);
	}
	else
	{
		BitBlt(m_hdcGraph, iXleft+1, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft+1, 0, SRCCOPY);
		Sleep(0);
		pDCGraph->Polygon(ptsLine, iPointsCount);
		//PlotPointArray(ptsLine,iPoints);
	}
	Sleep(0);

	if(m_bAutofit && (m_bIgnoreAutofit==false))
	{
		CheckAutoScaleY();
	}
	Sleep(0);

	

	pDCGraph->SelectObject(&penLimit);

	////Draw the Voloume Limit
	if(m_bDrawVolumeLimit && m_iVolumeLimit>0)
	{
		pDCGraph->MoveTo(iGraphXleft,m_iVolumeLimit);
		pDCGraph->LineTo(iGraphXright,m_iVolumeLimit);
		Sleep(0);
	}

	////Draw the Voloume Garanty
	if(m_bDrawVolumeGaranty && m_iVolumeGaranty>0)
	{
		pDCGraph->SelectObject(&penGarant);
		pDCGraph->MoveTo(iGraphXleft,m_iVolumeGaranty);
		pDCGraph->LineTo(iGraphXright,m_iVolumeGaranty);
		Sleep(0);
	}
	if(m_bDrawHFOVolumeGarantyHigh)
	{
		pDCGraph->SelectObject(&penGarant);
		pDCGraph->MoveTo(iGraphXleft,m_iHFOVolumeGarantyHigh);
		pDCGraph->LineTo(iGraphXright,m_iHFOVolumeGarantyHigh);
		Sleep(0);
	}
	if(m_bDrawHFOVolumeGarantyLow)
	{
		pDCGraph->SelectObject(&penGarant);
		pDCGraph->MoveTo(iGraphXleft,m_iHFOVolumeGarantyLow);
		pDCGraph->LineTo(iGraphXright,m_iHFOVolumeGarantyLow);
		Sleep(0);
	}


	////Draw the TriggerVolume
	if(m_iTrigThreshold>0)
	{
		pDCGraph->SelectObject(&penTrig);
		pDCGraph->MoveTo(iGraphXleft,m_iTrigThreshold);
		pDCGraph->LineTo(iGraphXright,m_iTrigThreshold);
		Sleep(0);
	}

	

	BitBlt(dc.m_hDC, iGraphXleft, 0, iGraphWidth/*+2*/,m_iGraphHeight, m_hdcGraph,  iGraphXleft, 0, SRCCOPY);
	Sleep(0);

	if(m_bDrawCursor &&
		iXleft>=DIAGRAMM_WIDTH-31)
	{
		DrawCursor(m_bDrawCursor);
		Sleep(0);
	}

	poldpenDC=dc.SelectObject(&cursorpen);
	if(iXleft+iWidth+3<(m_iGraphX+m_iGraphWidth-m_rmargin))
	{
		MoveToEx(dc.m_hDC, iXleft+iWidth+3, m_iGraphHeight-1, NULL);
		LineTo(dc.m_hDC, iXleft+iWidth+3, 1);
		Sleep(0);
	}
	

	pDCGraph->SelectObject(poldpenGraph);
	pDCGraph->SelectObject(poldbrush);
	dc.SelectObject(poldpenDC);
	

	penGrey.DeleteObject();
	penGreen.DeleteObject();
	cursorpen.DeleteObject();
	penTrig.DeleteObject();
	penLimit.DeleteObject();
	penGarant.DeleteObject();
	//brushGrey.DeleteObject();//rkuNEWFIX
	//brushGreen.DeleteObject();//rkuNEWFIX

}


void CDiagramm::CheckAutoScaleXYincrease()
{
	if(m_bExit)
		return;

	bool bReset=false;

	//int iID = m_nID;

	if(m_dMaxX>m_dXAxisMax || m_dMinX<m_dXAxisMin)
	{
		IncreaseXScale(true);

		bReset=true;
	}
	

	if(m_dMaxY>m_dYAxisMax || m_dMinY<m_dYAxisMin)
	{
		IncreaseYScale(true);

		bReset=true;
	}
	

	if(bReset)
	{
		switch(m_iDiagrammType)
		{
		case PRESSURE_HF_GRAPH:
			{
				m_dMaxX=0;
				m_dMinX=0;
				m_dMaxY=G_LOWER_MINSCALE_PRESSUREHF;
				m_dMinY=G_UPPER_MAXSCALE_PRESSUREHF;
			}
			break;
		case PRESSURE_GRAPH:
			{
				m_dMaxX=0;
				m_dMinX=0;
				m_dMaxY=0;
				
				if(getModel()->getVMODEHANDLER()->activeModeIsNMODE())
				{
					m_dMinY=G_UPPER_MAXSCALE_NMODEPRESSURE;
				}
				else
				{
					m_dMinY=G_UPPER_MAXSCALE_PRESSURE;
				}
			}
			break;
		case FLOW_HF_GRAPH:
		case FLOW_GRAPH:
			{
				m_dMaxX=(-1)*G_UPPER_MAXSCALE_FLOW;
				m_dMinX=G_UPPER_MAXSCALE_FLOW;
				m_dMaxY=(-1)*G_UPPER_MAXSCALE_FLOW;
				m_dMinY=G_UPPER_MAXSCALE_FLOW;
			}
			break;
		case CO2_GRAPH:
		case CO2_HF_GRAPH:
			{
				m_dMaxX=0;
				m_dMinX=0;
				m_dMaxY=0;
				m_dMinY=G_UPPER_MAXSCALE_CO2;
			}
			break;
		case SPO2_GRAPH:
			{
				m_dMaxX=0;
				m_dMinX=0;
				m_dMaxY=(-1)*G_UPPER_MAXSCALE_SPO2;
				m_dMinY=G_UPPER_MAXSCALE_SPO2;
			}
			break;
		case FOT_LOOP:
			{
				m_dMaxX=G_UPPER_MAXSCALE_PRESSURE;
				m_dMinX=G_LOWER_MAXSCALE_PRESSURE_FOT;
				m_dMaxY=G_LOWER_MINSCALE_XRS_FOT;//rku, check FOTGRAPH
				m_dMinY=G_UPPER_MAXSCALE_XRS_FOT;
			}
			break;
		case VOLUME_HF_GRAPH:
			{
				m_dMaxX=0;
				m_dMinX=0;
				m_dMaxY=0;
				m_dMinY=G_UPPER_MAXSCALE_VOLUME_HF;
			}
			break;
		case VOLUME_GRAPH:
			{
				m_dMaxX=0;
				m_dMinX=0;
				m_dMaxY=0;

				if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
					m_dMinY=G_UPPER_MAXSCALE_VOLUME_NEO;
				else
					m_dMinY=G_UPPER_MAXSCALE_VOLUME_PED;

			}
			break;
		case PRESSURE_VOLUME_LOOP:
			{
				m_dMaxX=0;
				m_dMinX=G_UPPER_MAXSCALE_PRESSURE;
				m_dMaxY=0;

				if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
					m_dMinY=G_UPPER_MAXSCALE_VOLUME_NEO;
				else
					m_dMinY=G_UPPER_MAXSCALE_VOLUME_PED;

			}
			break;
		case PRESSURE_VOLUME_HF_LOOP:
			{
				m_dMaxX=G_LOWER_MINSCALE_PRESSUREHF;
				m_dMinX=G_UPPER_MAXSCALE_PRESSUREHF;
				m_dMaxY=0;
				m_dMinY=G_UPPER_MAXSCALE_VOLUME_HF;
			}
			break;
		case VOLUME_FLOW_LOOP:
			{
				m_dMaxX=0;

				if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
					m_dMinX=G_UPPER_MAXSCALE_VOLUME_NEO;
				else
					m_dMinX=G_UPPER_MAXSCALE_VOLUME_PED;

				m_dMaxY=(-1)*G_UPPER_MAXSCALE_FLOW;
				m_dMinY=G_UPPER_MAXSCALE_FLOW;
			}
			break;
		case VOLUME_FLOW_HF_LOOP:
			{
				m_dMaxX=0;
				m_dMinX=G_UPPER_MAXSCALE_VOLUME_HF;
				m_dMaxY=(-1)*G_UPPER_MAXSCALE_FLOW;
				m_dMinY=G_UPPER_MAXSCALE_FLOW;
			}
			break;
		default:
			{
				m_dMaxX=-100;
				m_dMinX=100;
				m_dMaxY=-100;
				m_dMinY=100;

			}
			break;
		}
	}
	
	
}

void CDiagramm::CheckAutoScaleXYdecrease()
{
	if(m_bExit)
		return;

	bool bReset=false;

	//int iID = m_nID;

	if(m_dMaxX>m_dXAxisMax || m_dMinX<m_dXAxisMin)
	{
		
	}
	else
	{
		if(isSafeTickCountDelayExpired(m_dwLastCheckAutoScaleX, TIMEAUTOSCALE))
		{
			DecreaseXScale(true,true);
			bReset=true;
			m_dwLastCheckAutoScaleX=GetTickCount();
		}
	}

	if(m_dMaxY>m_dYAxisMax || m_dMinY<m_dYAxisMin)
	{
		
	}
	else
	{
		if(isSafeTickCountDelayExpired(m_dwLastCheckAutoScaleY, TIMEAUTOSCALE))
		{
			if(CanDecreaseYScale())
			{
				DecreaseYScaleToNextValue(true);
			}
			bReset=true;
			m_dwLastCheckAutoScaleY=GetTickCount();
		}
	}

	if(bReset)
	{
		switch(m_iDiagrammType)
		{
		case PRESSURE_HF_GRAPH:
			{
				m_dMaxX=0;
				m_dMinX=0;
				m_dMaxY=G_LOWER_MINSCALE_PRESSUREHF;
				m_dMinY=G_UPPER_MAXSCALE_PRESSUREHF;
			}
			break;
		case PRESSURE_GRAPH:
			{
				m_dMaxX=0;
				m_dMinX=0;
				m_dMaxY=0;
				if(getModel()->getVMODEHANDLER()->activeModeIsNMODE())
				{
					m_dMinY=G_UPPER_MAXSCALE_NMODEPRESSURE;
				}
				else
				{
					m_dMinY=G_UPPER_MAXSCALE_PRESSURE;
				}
			}
			break;
		case FLOW_HF_GRAPH:
		case FLOW_GRAPH:
			{
				m_dMaxX=(-1)*G_UPPER_MAXSCALE_FLOW;
				m_dMinX=G_UPPER_MAXSCALE_FLOW;
				m_dMaxY=(-1)*G_UPPER_MAXSCALE_FLOW;
				m_dMinY=G_UPPER_MAXSCALE_FLOW;
			}
			break;
		case CO2_GRAPH:
		case CO2_HF_GRAPH:
			{
				m_dMaxX=0;
				m_dMinX=0;
				m_dMaxY=0;
				m_dMinY=G_UPPER_MAXSCALE_CO2;
			}
			break;
		case SPO2_GRAPH:
			{
				m_dMaxX=0;
				m_dMinX=0;
				m_dMaxY=(-1)*G_UPPER_MAXSCALE_SPO2;
				m_dMinY=G_UPPER_MAXSCALE_SPO2;
				/*m_dMaxY=0;
				m_dMinY=G_UPPER_MAXSCALE_SPO2;*/
			}
			break;
		case FOT_LOOP:
			{
				m_dMaxX=G_UPPER_MAXSCALE_PRESSURE;
				m_dMinX=G_LOWER_MAXSCALE_PRESSURE_FOT;
				m_dMaxY=G_LOWER_MINSCALE_XRS_FOT;//rku, check FOTGRAPH
				m_dMinY=G_UPPER_MAXSCALE_XRS_FOT;
			}
			break;
		case VOLUME_HF_GRAPH:
			{
				m_dMaxX=0;
				m_dMinX=0;
				m_dMaxY=0;
				m_dMinY=G_UPPER_MAXSCALE_VOLUME_HF;
			}
			break;
		case VOLUME_GRAPH:
			{
				m_dMaxX=0;
				m_dMinX=0;
				m_dMaxY=0;

				if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
					m_dMinY=G_UPPER_MAXSCALE_VOLUME_NEO;
				else
					m_dMinY=G_UPPER_MAXSCALE_VOLUME_PED;

			}
			break;
		case PRESSURE_VOLUME_LOOP:
			{
				m_dMaxX=0;
				m_dMinX=G_UPPER_MAXSCALE_PRESSURE;
				m_dMaxY=0;

				if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
					m_dMinY=G_UPPER_MAXSCALE_VOLUME_NEO;
				else
					m_dMinY=G_UPPER_MAXSCALE_VOLUME_PED;
			}
			break;
		case PRESSURE_VOLUME_HF_LOOP:
			{
				m_dMaxX=G_LOWER_MINSCALE_PRESSUREHF;
				m_dMinX=G_UPPER_MAXSCALE_PRESSUREHF;
				m_dMaxY=0;
				m_dMinY=G_UPPER_MAXSCALE_VOLUME_HF;
			}
			break;
		case VOLUME_FLOW_LOOP:
			{
				m_dMaxX=0;

				if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
					m_dMinX=G_UPPER_MAXSCALE_VOLUME_NEO;
				else
					m_dMinX=G_UPPER_MAXSCALE_VOLUME_PED;

				m_dMaxY=(-1)*G_UPPER_MAXSCALE_FLOW;
				m_dMinY=G_UPPER_MAXSCALE_FLOW;
			}
			break;
		case VOLUME_FLOW_HF_LOOP:
			{
				m_dMaxX=0;
				m_dMinX=G_UPPER_MAXSCALE_VOLUME_HF;
				m_dMaxY=(-1)*G_UPPER_MAXSCALE_FLOW;
				m_dMinY=G_UPPER_MAXSCALE_FLOW;
			}
			break;
		default:
			{
				m_dMaxX=-100;
				m_dMinX=100;
				m_dMaxY=-100;
				m_dMinY=100;

			}
			break;
		}
	}
	
}




// **************************************************************************
// 
// **************************************************************************
void CDiagramm::ScaleToNextPossibleXValue(double dMaxX, double dMinX, bool bRedraw,bool bRedrawDiagrammData)
{
	if(m_bExit)
		return;
	switch(m_iDiagrammType)
	{
	case FOT_LOOP:
		{
			double dXAxisMax=GetXAxisMaxPressure(dMaxX);
			if(m_dXAxisMax!=dXAxisMax)
			{
				SetXAxisScale(0, dXAxisMax, bRedraw, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_FOT_PRESSURE(dXAxisMax);
			}
		}
		break;
	case PRESSURE_VOLUME_LOOP:
		{
			double dXAxisMax=GetXAxisMaxPressure(dMaxX);
			if(m_dXAxisMax!=dXAxisMax)
			{
				SetXAxisScale(GetNegativeYoffset(dXAxisMax), dXAxisMax, bRedraw, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_PVLOOP(dXAxisMax);
			}
		}
		break;
	case PRESSURE_VOLUME_HF_LOOP:
		{
			double dXAxisMax=GetXAxisMaxPressure(dMaxX);
			double dXAxisMin=GetXAxisMinPressure(dMinX);
			if(m_dXAxisMax!=dXAxisMax || m_dXAxisMin!=dXAxisMin)
			{
				SetXAxisScale(dXAxisMin, dXAxisMax, bRedraw, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFPVLOOP(dXAxisMax);
				getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFPVLOOP(dXAxisMin);
			}
		}
		break;
	}
}

// **************************************************************************
// 
// **************************************************************************
void CDiagramm::ScaleToNextPossibleYValue(double dMaxY, double dMinY, bool bRedraw,bool bRedrawDiagrammData)
{
	if(m_bExit)
		return;
	switch(m_iDiagrammType)
	{
	case VOLUME_GRAPH: //newVG  sollte nie hier ein???
		{
			double dYAxisMax=GetYAxisMaxPressure(dMaxY);
			if(m_dYAxisMax!=dYAxisMax)
			{
				SetYAxisScale(GetNegativeYoffset(dYAxisMax), dYAxisMax, bRedraw, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_GRAPH(dYAxisMax);
			}
		}
		break;
	case PRESSURE_GRAPH:
		{
			double dYAxisMax=GetYAxisMaxPressure(dMaxY);
			if(m_dYAxisMax!=dYAxisMax)
			{
				SetYAxisScale(GetNegativeYoffset(dYAxisMax), dYAxisMax, bRedraw, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_GRAPH(dYAxisMax);
			}
		}
		break;
	case PRESSURE_HF_GRAPH:
		{
			double dYAxisMax=GetYAxisMaxPressure(dMaxY);
			double dYAxisMin=GetYAxisMinPressure(dMinY);
			if(m_dYAxisMax!=dYAxisMax || m_dYAxisMin!=dYAxisMin)
			{
				SetYAxisScale(dYAxisMin, dYAxisMax, bRedraw, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFGRAPH(dYAxisMax);
				getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFGRAPH(dYAxisMin);
			}
		}
		break;
	case PRESSURE_VOLUME_HF_LOOP:
		{
			double dXAxisMax=GetXAxisMaxPressure(dMaxY);
			double dXAxisMin=GetXAxisMinPressure(dMinY);
			if(m_dXAxisMax!=dXAxisMax || m_dXAxisMin!=dXAxisMin)
			{
				SetXAxisScale(dXAxisMin, dXAxisMax, bRedraw, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFPVLOOP(dXAxisMin);
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFPVLOOP(dXAxisMax);

			}
		}
		break;
	}
}


// **************************************************************************
// 
// **************************************************************************
//void CDiagramm::resetAutoScaleX()
//{
//	m_dMaxX=0;
//	m_dMinX=0;
//}
//void CDiagramm::resetAutoScaleY()
//{
//	m_dMaxY=0;
//	m_dMinY=0;
//}

// **************************************************************************
// 
// **************************************************************************
void CDiagramm::CheckAutoScaleY()
{
	if(m_bExit)
		return;
	if(!m_pFunctionParams)
	{
		return;
	}

	if(false==getModel()->getVMODEHANDLER()->activeModeIsTHERAPY())
	{
		if(m_iDiagrammType==PRESSURE_GRAPH || m_iDiagrammType==PRESSURE_HF_GRAPH)
			return;
	}
	

	if(m_iDiagrammType==FOT_LOOP)
	{
		/*double dTempMaxY=0;
		if(m_dMaxY>=0)
			dTempMaxY=m_dMaxY+10;
		else
			dTempMaxY=m_dMaxY-10;*/

		if(m_dMaxY>=m_dYAxisMax && m_pFunctionParams->yMax>m_dYAxisMax)
		{
			if(isSafeTickCountDelayExpired(m_dwLastCheckAutoScaleY, 100))
			{
				IncreaseYScale(true);

				m_dwLastCheckAutoScaleY=GetTickCount();
				m_dMaxY=G_LOWER_MINSCALE_XRS_FOT;//rku, check FOTGRAPH
				m_dMinY=G_UPPER_MAXSCALE_XRS_FOT;
			}
		}
		else if(m_pFunctionParams->yMin<m_dYAxisMin && m_dMinY<=m_dYAxisMin)//rku, check FOTGRAPH, nothing to change here!
		{
			if(isSafeTickCountDelayExpired(m_dwLastCheckAutoScaleY, 100))
			{
				IncreaseYScale(true);

				m_dwLastCheckAutoScaleY=GetTickCount();
				m_dMaxY=G_LOWER_MINSCALE_XRS_FOT;//rku, check FOTGRAPH
				m_dMinY=G_UPPER_MAXSCALE_XRS_FOT;
			}
		}
		else
		{
			//if(isSafeTickCountDelayExpired(m_dwLastCheckAutoScaleY, TIMEAUTOSCALE))
			{
				m_dwLastCheckAutoScaleY=GetTickCount();

				if(CanDecreaseYScale()) //rku, check FOTGRAPH
				{
					DecreaseYScaleToNextValue(true);
				}
				m_dMaxY=G_LOWER_MINSCALE_XRS_FOT;//rku, check FOTGRAPH
				m_dMinY=G_UPPER_MAXSCALE_XRS_FOT;
			}
		}
	}
	else
	{
		if(m_dMaxY>m_dYAxisMax && m_pFunctionParams->yMax>m_dYAxisMax /*&& m_iDiagrammType!=PRESSURE_GRAPH && m_iDiagrammType!=PRESSURE_HF_GRAPH*/)
		{
			if(isSafeTickCountDelayExpired(m_dwLastCheckAutoScaleY, 100))
			{
				//rkuNEWFIX
				//DEBUGMSG(TRUE, (TEXT("DIAinc1 y%d\r\n"),m_iDiagrammType));
				/*CStringW sz=_T("");
				sz.Format(_T("#DIAinc1 y%d"),m_iDiagrammType);
				theApp.getLog()->WriteLine(sz);*/

				IncreaseYScale(true);

				m_dwLastCheckAutoScaleY=GetTickCount();
				m_dMaxY=0;
				m_dMinY=0;
			}
		}
		else if(m_pFunctionParams->yMax>(m_dYAxisMin*-1) && m_dMinY<m_dYAxisMin /*&& m_iDiagrammType!=PRESSURE_GRAPH && m_iDiagrammType!=PRESSURE_HF_GRAPH*/)
		{
			if(isSafeTickCountDelayExpired(m_dwLastCheckAutoScaleY, 100))
			{
				//rkuNEWFIX
				//DEBUGMSG(TRUE, (TEXT("DIAinc2 y%d\r\n"),m_iDiagrammType));
				/*CStringW sz=_T("");
				sz.Format(_T("#DIAinc2 y%d"),m_iDiagrammType);
				theApp.getLog()->WriteLine(sz);*/

				IncreaseYScale(true);

				m_dwLastCheckAutoScaleY=GetTickCount();
				m_dMaxY=0;
				m_dMinY=0;
			}
		}
		else
		{
			if(isSafeTickCountDelayExpired(m_dwLastCheckAutoScaleY, TIMEAUTOSCALE))
			{
				m_dwLastCheckAutoScaleY=GetTickCount();

				if(CanDecreaseYScale())
				{
					//rkuNEWFIX
					//DEBUGMSG(TRUE, (TEXT("DIAdec1 y%d\r\n"),m_iDiagrammType));
					/*CStringW sz=_T("");
					sz.Format(_T("#DIAdec1 y%d"),m_iDiagrammType);
					theApp.getLog()->WriteLine(sz);*/

					DecreaseYScaleToNextValue(true);
				}
				m_dMaxY=0;
				m_dMinY=0;
			}
		}
	}
	
}

double CDiagramm::GetNegativeYoffset(double dwMaxY)
{
	double result =CTlsFloat::Round((dwMaxY/(GetYPixelsOfDiagramm()-10))*(-10), 1);

	return result;
}



// **************************************************************************
// 
// **************************************************************************
void CDiagramm::IncreaseXScale(bool bRedrawDiagrammData)
{
	if(m_bExit)
		return;

	double dHigherXAxisMax=0;
	double dLowerXAxisMin=0;

	/*double dHigherXAxisMax=GetHigherXAxisMax();

	if(m_dXAxisMax!=dHigherXAxisMax)*/
	{
		switch(m_iDiagrammType)
		{
		case PRESSURE_VOLUME_HF_LOOP:
			{
				if(!m_bAutofit)
				{
					dHigherXAxisMax=GetXAxisMaxPressure(m_dMaxX);
					dLowerXAxisMin=GetXAxisMinPressure(m_dMinX);

					SetXAxisScale(dLowerXAxisMin, dHigherXAxisMax, true);
					getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFPVLOOP(dHigherXAxisMax);
					getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFPVLOOP(dLowerXAxisMin);
				}
				
			}
			break;
		case PRESSURE_VOLUME_LOOP:
			{
				dHigherXAxisMax=GetHigherXAxisMax();

				if(m_dXAxisMax!=dHigherXAxisMax)
				{
					SetXAxisScale(GetNegativeYoffset(dHigherXAxisMax), dHigherXAxisMax, true);
					getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_PVLOOP(dHigherXAxisMax);
				}
				
			}
			break;
		case VOLUME_FLOW_LOOP:
			{
				dHigherXAxisMax=GetHigherXAxisMax();

				if(m_dXAxisMax!=dHigherXAxisMax)
				{
					SetXAxisScale(GetNegativeYoffset(dHigherXAxisMax), dHigherXAxisMax, true);
					getModel()->getCONFIG()->SetMAXSCALE_VOLUME_VFLOOP(dHigherXAxisMax);
				}
				
			}
			break;
		case VOLUME_FLOW_HF_LOOP:
			{
				dHigherXAxisMax=GetHigherXAxisMax();

				if(m_dXAxisMax!=dHigherXAxisMax)
				{
					SetXAxisScale(GetNegativeYoffset(dHigherXAxisMax), dHigherXAxisMax, true);
					getModel()->getCONFIG()->SetMAXSCALE_VOLUME_HFVFLOOP(dHigherXAxisMax);
				}

			}
			break;
		case FOT_LOOP:
			{
				dHigherXAxisMax=GetHigherXAxisMax();

				if(m_dXAxisMax!=dHigherXAxisMax)
				{
					SetXAxisScale(0, dHigherXAxisMax, true);
					getModel()->getCONFIG()->SetMAXSCALE_FOT_PRESSURE(dHigherXAxisMax);
				}
			}
			break;
		case PRESSURE_GRAPH:
		case VOLUME_GRAPH:
		case FLOW_GRAPH:
		case PRESSURE_HF_GRAPH:
		case VOLUME_HF_GRAPH:
		case FLOW_HF_GRAPH:
		case SPO2_GRAPH:
		case CO2_GRAPH:
		case CO2_HF_GRAPH:
		default:
			{

			}
			break;
		}
	}
}


// **************************************************************************
// 
// **************************************************************************
void CDiagramm::IncreaseYScale(bool bRedrawDiagrammData)
{
	if(m_bExit)
		return;
	double dHigherYAxisMax=0;
	double dHigherYAxisMin=0;

	switch(m_iDiagrammType)
	{
	case PRESSURE_GRAPH:
		{
			dHigherYAxisMax=GetHigherYAxisMax();
			if(m_dYAxisMax!=dHigherYAxisMax)
			{
				SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_GRAPH(dHigherYAxisMax);
			}
		}
		break;
	case PRESSURE_HF_GRAPH:
		{
			dHigherYAxisMax=GetHigherYAxisMax();
			if(m_dYAxisMax!=dHigherYAxisMax)
			{
				SetYAxisScale(m_dYAxisMin, dHigherYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFGRAPH(m_dYAxisMin);
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFGRAPH(dHigherYAxisMax);
			}
		}
		break;
	case VOLUME_GRAPH:
		{
			dHigherYAxisMax=GetHigherYAxisMax();
			if(m_dYAxisMax!=dHigherYAxisMax)
			{
				SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_VOLUME_GRAPH(dHigherYAxisMax);
			}
		}
		break;
	case VOLUME_HF_GRAPH:
		{
			dHigherYAxisMax=GetHigherYAxisMax();
			if(m_dYAxisMax!=dHigherYAxisMax)
			{
				SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_VOLUME_HFGRAPH(dHigherYAxisMax);
			}
		}
		break;
	case SPO2_GRAPH:
		{
			dHigherYAxisMax=GetHigherYAxisMax();
			if(m_dYAxisMax!=dHigherYAxisMax)
			{
				SetYAxisScale(dHigherYAxisMax*(-1), dHigherYAxisMax, TRUE, bRedrawDiagrammData);
				//SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, true, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_SPO2_GRAPH(dHigherYAxisMax);
			}
		}
		break;
	case FOT_LOOP:
		{
			dHigherYAxisMax=GetHigherYAxisMax();
			dHigherYAxisMin=GetHigherYAxisMin();
			if(m_dYAxisMax!=dHigherYAxisMax || m_dYAxisMin!=dHigherYAxisMin)
			{
				SetYAxisScale(dHigherYAxisMin, dHigherYAxisMax, TRUE, bRedrawDiagrammData); //rku, check FOTGRAPH
				getModel()->getCONFIG()->SetMINSCALE_FOT_XRS(dHigherYAxisMin);
				getModel()->getCONFIG()->SetMAXSCALE_FOT_XRS(dHigherYAxisMax);
			}
		}
		break;
	case CO2_GRAPH:
		{
			dHigherYAxisMax=GetHigherYAxisMax();
			if(m_dYAxisMax!=dHigherYAxisMax)
			{
				SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_CO2_GRAPH(dHigherYAxisMax);
			}
		}
		break;
	case CO2_HF_GRAPH:
		{
			dHigherYAxisMax=GetHigherYAxisMax();
			if(m_dYAxisMax!=dHigherYAxisMax)
			{
				SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_CO2_HFGRAPH(dHigherYAxisMax);
			}
		}
		break;
	case FLOW_GRAPH:
		{
			dHigherYAxisMax=GetHigherYAxisMax();
			if(m_dYAxisMax!=dHigherYAxisMax)
			{
				SetYAxisScale(dHigherYAxisMax*(-1), dHigherYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_FLOW_GRAPH(dHigherYAxisMax);
			}
		}
		break;
	case FLOW_HF_GRAPH:
		{
			dHigherYAxisMax=GetHigherYAxisMax();
			if(m_dYAxisMax!=dHigherYAxisMax)
			{
				SetYAxisScale(dHigherYAxisMax*(-1), dHigherYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_FLOW_HFGRAPH(dHigherYAxisMax);
			}
		}
		break;
	case PRESSURE_VOLUME_LOOP:
		{
			dHigherYAxisMax=GetHigherYAxisMax();
			if(m_dYAxisMax!=dHigherYAxisMax)
			{
				SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_VOLUME_PVLOOP(dHigherYAxisMax);
			}
		}
		break;
	case PRESSURE_VOLUME_HF_LOOP:
		{
			dHigherYAxisMax=GetHigherYAxisMax();
			if(m_dYAxisMax!=dHigherYAxisMax)
			{
				SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_VOLUME_HFPVLOOP(dHigherYAxisMax);
			}
		}
		break;
	case VOLUME_FLOW_LOOP:
		{
			dHigherYAxisMax=GetHigherYAxisMax();
			if(m_dYAxisMax!=dHigherYAxisMax)
			{
				SetYAxisScale(dHigherYAxisMax*(-1), dHigherYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_FLOW_VFLOOP(dHigherYAxisMax);
			}
		}
		break;
	case VOLUME_FLOW_HF_LOOP:
		{
			dHigherYAxisMax=GetHigherYAxisMax();
			if(m_dYAxisMax!=dHigherYAxisMax)
			{
				SetYAxisScale(dHigherYAxisMax*(-1), dHigherYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_FLOW_HFVFLOOP(dHigherYAxisMax);
			}
		}
		break;
	}
}




// **************************************************************************
// 
// **************************************************************************
double CDiagramm::GetHigherXAxisMax()
{
	double dHigherAxisMax=0;

	if(!m_pFunctionParams)
	{
		return dHigherAxisMax;
	}

	switch(m_iDiagrammType)
	{
	case PRESSURE_VOLUME_LOOP:
	case PRESSURE_VOLUME_HF_LOOP:
		{
			//PRESSURE
			dHigherAxisMax=m_dXAxisMax;
			
			while (m_dMaxX>dHigherAxisMax)
			{
				if(dHigherAxisMax<10)
				{
					dHigherAxisMax=10;
				}
				else if(dHigherAxisMax<20)
				{
					dHigherAxisMax=20;
				}
				else if(dHigherAxisMax<30)
				{
					dHigherAxisMax=30;
				}
				else if(dHigherAxisMax<40)
				{
					dHigherAxisMax=40;
				}
				else if(dHigherAxisMax<60)
				{
					dHigherAxisMax=60;
				}
				else if(dHigherAxisMax<80)
				{
					dHigherAxisMax=80;
				}
				else if(dHigherAxisMax<100)
				{
					dHigherAxisMax=100;
				}
				else
				{
					break;
				}
			}
		}
		break;
	case VOLUME_FLOW_LOOP:
		{
			//VOLUME
			dHigherAxisMax=m_dXAxisMax;
			
			while (m_dMaxX>dHigherAxisMax)
			{
				if(dHigherAxisMax<2)//newVG
				{
					dHigherAxisMax=2;
				}
				else if(dHigherAxisMax<5)
				{
					dHigherAxisMax=5;
				}
				else if(dHigherAxisMax<10)
				{
					dHigherAxisMax=10;
				}
				else if(dHigherAxisMax<20)
				{
					dHigherAxisMax=20;
				}
				else if(dHigherAxisMax<30)
				{
					dHigherAxisMax=30;
				}
				else if(dHigherAxisMax<40)
				{
					dHigherAxisMax=40;
				}
				else if(dHigherAxisMax<60)
				{
					dHigherAxisMax=60;
				}
				else if(dHigherAxisMax<80)
				{
					dHigherAxisMax=80;
				}
				else if(dHigherAxisMax<100)
				{
					dHigherAxisMax=100;
				}
				else if(dHigherAxisMax<200)
				{
					dHigherAxisMax=200;
				}
				else if(dHigherAxisMax<300)
				{
					dHigherAxisMax=300;
				}
				else if(dHigherAxisMax<400)
				{
					dHigherAxisMax=400;
				}
				else if(dHigherAxisMax<600)
				{
					dHigherAxisMax=600;
				}
				else
				{
					break;
				}
			}
		}
		break;
	case VOLUME_FLOW_HF_LOOP:
		{
			//VOLUME
			dHigherAxisMax=m_dXAxisMax;

			while (m_dMaxX>dHigherAxisMax)
			{
				if(dHigherAxisMax<5)
				{
					dHigherAxisMax=5;
				}
				else if(dHigherAxisMax<10)
				{
					dHigherAxisMax=10;
				}
				else if(dHigherAxisMax<20)
				{
					dHigherAxisMax=20;
				}
				else if(dHigherAxisMax<30)
				{
					dHigherAxisMax=30;
				}
				else if(dHigherAxisMax<40)
				{
					dHigherAxisMax=40;
				}
				else if(dHigherAxisMax<50)
				{
					dHigherAxisMax=50;
				}
				else if(dHigherAxisMax<60)
				{
					dHigherAxisMax=60;
				}
				else if(dHigherAxisMax<70)
				{
					dHigherAxisMax=70;
				}
				else if(dHigherAxisMax<80)
				{
					dHigherAxisMax=80;
				}
				else
				{
					break;
				}
			}
		}
		break;
	}

	if(dHigherAxisMax>m_pFunctionParams->xMax || dHigherAxisMax==0)
		dHigherAxisMax=m_pFunctionParams->xMax;

	return dHigherAxisMax;

}

double CDiagramm::GetXAxisMaxPressure(double dMaxX)
{
	double dAxisMax=0;
	double dTemp=0;
	double dValue=dMaxX/G_FACTOR_PRESSURE;


	switch(m_iDiagrammType)
	{
	case FOT_LOOP:
		{
			double d20=(dValue/100)*10;
			dValue=dValue+d20;
			dAxisMax=m_dXAxisMax;

			if(dValue>dAxisMax)
			{
				while (dValue>dAxisMax)
				{
					if(dAxisMax<10)
					{
						dAxisMax=10;
					}
					else if(dAxisMax<20)
					{
						dAxisMax=20;
					}
					else 
					{
						dAxisMax=dAxisMax+10;
					}

					if(dAxisMax>G_UPPER_MAXSCALE_PRESSURE || dAxisMax==0)
					{
						dAxisMax=G_UPPER_MAXSCALE_PRESSURE;
						break;
					}
				}
			}
			else
			{
				while (dValue<dAxisMax)
				{
					if(dAxisMax>=20)
					{
						dTemp=dAxisMax-10;
						if(dValue<dTemp)
							dAxisMax=dTemp;
						else
							break;
					}
					else 
					{
						dAxisMax=10;
						break;
					}
				}
			}
		}
		break;
	case PRESSURE_VOLUME_LOOP:
	case PRESSURE_VOLUME_HF_LOOP:
		{
			double d20=(dValue/100)*10;
			dValue=dValue+d20;
			dAxisMax=m_dXAxisMax;

			if(dValue>dAxisMax)
			{
				while (dValue>dAxisMax)
				{
					if(dAxisMax<10)
					{
						dAxisMax=10;
					}
					else if(dAxisMax<20)
					{
						dAxisMax=20;
					}
					else 
					{
						dAxisMax=dAxisMax+10;
					}

					if(dAxisMax>G_UPPER_MAXSCALE_PRESSUREHF || dAxisMax==0)
					{
						dAxisMax=G_UPPER_MAXSCALE_PRESSUREHF;
						break;
					}
				}
			}
			else
			{
				while (dValue<dAxisMax)
				{
					if(dAxisMax>=20)
					{
						dTemp=dAxisMax-10;
						if(dValue<dTemp)
							dAxisMax=dTemp;
						else
							break;
					}
					else 
					{
						dAxisMax=10;
						break;
					}
				}
			}
		}
		break;
	}

	return dAxisMax;
}


double CDiagramm::GetXAxisMinPressure(double dMinX)
{
	double dAxisMin=0;
	double dTemp=0;
	double dValue=dMinX/G_FACTOR_PRESSURE;
	double d20=0;
	double dbNegXoffset=GetNegativeYoffset(m_dXAxisMax);
	if(dValue==0)
		dValue=dbNegXoffset;
	else
	{
		d20=(dValue/100)*10;
		if(d20>=0)
			dValue=dValue-d20;
		else
			dValue=dValue+d20;
	}
	dAxisMin=m_dXAxisMin;

	if(dAxisMin>=0 || dAxisMin>dbNegXoffset)
	{
		return dbNegXoffset;
	}


	switch(m_iDiagrammType)
	{
	case PRESSURE_VOLUME_LOOP:
	case PRESSURE_VOLUME_HF_LOOP:
		{
			if(dValue<dAxisMin)
			{
				while (dValue<dAxisMin)
				{
					dAxisMin=dAxisMin-10;

					if(dAxisMin<G_LOWER_MINSCALE_PRESSUREHF)
					{
						dAxisMin=G_LOWER_MINSCALE_PRESSUREHF;
						break;
					}
					if(dAxisMin>=0)
					{
						break;
					}
				}


			}
			else
			{
				while (dValue>dAxisMin)
				{
					dTemp=dAxisMin+10;
					if(dValue>dTemp)
						dAxisMin=dTemp;
					else
					{
						break;
					}

					if(dAxisMin>=0)
					{
						break;
					}
				}
			}
		}
		break;
	}

	if(dAxisMin>=0 || dAxisMin>dbNegXoffset)
	{
		dAxisMin=dbNegXoffset;
	}

	return dAxisMin;
}



double CDiagramm::GetYAxisMaxPressure(double dMaxY)
{
	double dTemp=0;
	double dValue=dMaxY/G_FACTOR_PRESSURE;
	
	double d30=(dValue/100)*20;
	dValue=dValue+d30;
	double dAxisMax=m_dYAxisMax;

	switch(m_iDiagrammType)
	{
	case PRESSURE_GRAPH:
		{
			if(dValue>dAxisMax)
			{
				while (dValue>dAxisMax)
				{
					if(dAxisMax<10)
					{
						dAxisMax=10;
					}
					else if(dAxisMax<20)
					{
						dAxisMax=20;
					}
					else 
					{
						dAxisMax=dAxisMax+10;
					}
					
					
					if(getModel()->getVMODEHANDLER()->activeModeIsNMODE())
					{
						if(dAxisMax>G_UPPER_MAXSCALE_NMODEPRESSURE || dAxisMax==0)
						{
							dAxisMax=G_UPPER_MAXSCALE_NMODEPRESSURE;
							break;
						}
					}
					else
					{
						if(dAxisMax>G_UPPER_MAXSCALE_PRESSURE || dAxisMax==0)
						{
							dAxisMax=G_UPPER_MAXSCALE_PRESSURE;
							break;
						}
					}
				}
			}
			else
			{
				while (dValue<dAxisMax)
				{
					if(dAxisMax>=20)
					{
						dTemp=dAxisMax-10;
						if(dValue<dTemp)
							dAxisMax=dTemp;
						else
							break;
					}
					else 
					{
						dAxisMax=G_LOWER_MAXSCALE_PRESSURE;
						break;
					}
				}

				if(dAxisMax<G_LOWER_MAXSCALE_PRESSURE || dAxisMax==0)
					dAxisMax=G_LOWER_MAXSCALE_PRESSURE;
			}
		}
		break;
	case PRESSURE_HF_GRAPH:
		{
			

			if(dValue>dAxisMax)
			{
				while (dValue>dAxisMax)
				{
					if(dAxisMax<10)
					{
						dAxisMax=10;
					}
					else if(dAxisMax<20)
					{
						dAxisMax=20;
					}
					else 
					{
						dAxisMax=dAxisMax+10;
					}

					if(dAxisMax>G_UPPER_MAXSCALE_PRESSUREHF || dAxisMax==0)
					{
						dAxisMax=G_UPPER_MAXSCALE_PRESSUREHF;
						break;
					}
				}
			}
			else
			{
				while (dValue<dAxisMax)
				{
					if(dAxisMax>=20)
					{
						dTemp=dAxisMax-10;
						if(dValue<dTemp)
							dAxisMax=dTemp;
						else
							break;
					}
					else 
					{
						dAxisMax=10;
						break;
					}
				}
			}
		}
		break;
	default:
		{
			
		}
		break;
	}

	return dAxisMax;
}


double CDiagramm::GetYAxisMinPressure(double dMinY)
{
	double dAxisMin=0;
	double dTemp=0;
	double dValue=dMinY/G_FACTOR_PRESSURE;
	double d20=0;
	double dbNegYoffset=GetNegativeYoffset(m_dYAxisMax);
	if(dValue==0)
		dValue=dbNegYoffset;
	else
	{
		d20=(dValue/100)*10;
		if(d20>=0)
			dValue=dValue-d20;
		else
			dValue=dValue+d20;
	}

	dAxisMin=m_dYAxisMin;

	if(dAxisMin>=0 || dAxisMin>dbNegYoffset)
	{
		return dbNegYoffset;
	}
	

	switch(m_iDiagrammType)
	{
	case PRESSURE_HF_GRAPH:
		{
			

			if(dValue<dAxisMin)
			{
				while (dValue<dAxisMin)
				{
					dAxisMin=dAxisMin-10;

					if(dAxisMin<G_LOWER_MINSCALE_PRESSUREHF)
					{
						dAxisMin=G_LOWER_MINSCALE_PRESSUREHF;
						break;
					}
					if(dAxisMin>=0)
					{
						break;
					}
				}
			}
			else
			{
				while (dValue>dAxisMin)
				{
					dTemp=dAxisMin+10;
					if(dValue>dTemp)
						dAxisMin=dTemp;
					else
					{
						break;
					}

					if(dAxisMin>=0)
					{
						break;
					}
				}
			}
		}
		break;
	}

	if(dAxisMin>=0 || dAxisMin>dbNegYoffset)
	{
		dAxisMin=dbNegYoffset;
	}

	return dAxisMin;
}


// **************************************************************************
// 
// **************************************************************************
double CDiagramm::GetHigherYAxisMax()
{
	double dHigherAxisMax=0;

	if(!m_pFunctionParams)
	{
		return dHigherAxisMax;
	}

	double dValue=m_dMaxY;

	/*if((m_dMinY *(-1))>m_dMaxY)
		dValue=m_dMinY *(-1);*/

	switch(m_iDiagrammType)
	{
	case VOLUME_FLOW_LOOP:
	case VOLUME_FLOW_HF_LOOP:
	case FLOW_GRAPH:
	case FLOW_HF_GRAPH:
		{
			if((m_dMinY *(-1))>m_dMaxY)
				dValue=m_dMinY *(-1);

			//FLOW
			dHigherAxisMax=m_dYAxisMax;
			
			while (dValue>dHigherAxisMax)
			{
				if(dHigherAxisMax<5)
				{
					dHigherAxisMax=5;
				}
				else if(dHigherAxisMax<10)
				{
					dHigherAxisMax=10;
				}
				else if(dHigherAxisMax<15)
				{
					dHigherAxisMax=15;
				}
				else if(dHigherAxisMax<20)
				{
					dHigherAxisMax=20;
				}
				else if(dHigherAxisMax<30)
				{
					dHigherAxisMax=30;
				}
				else if(dHigherAxisMax<40)
				{
					dHigherAxisMax=40;
				}
				else if(dHigherAxisMax<50)
				{
					dHigherAxisMax=50;
				}
				else if(dHigherAxisMax<60)
				{
					dHigherAxisMax=60;
				}
				else if(dHigherAxisMax<70)
				{
					dHigherAxisMax=70;
				}
				else if(dHigherAxisMax<80)
				{
					dHigherAxisMax=80;
				}
				else
				{
					break;
				}
			}
		}
		break;
	case PRESSURE_VOLUME_LOOP:
	case VOLUME_GRAPH:
		{
			if((m_dMinY *(-1))>m_dMaxY)
				dValue=m_dMinY *(-1);

			//VOLUME
			dHigherAxisMax=m_dYAxisMax;

			while (dValue>dHigherAxisMax)
			{
				if(dHigherAxisMax<2)//newVG
				{
					dHigherAxisMax=2;
				}
				else if(dHigherAxisMax<5)
				{
					dHigherAxisMax=5;
				}
				else if(dHigherAxisMax<10)
				{
					dHigherAxisMax=10;
				}
				else if(dHigherAxisMax<20)
				{
					dHigherAxisMax=20;
				}
				else if(dHigherAxisMax<30)
				{
					dHigherAxisMax=30;
				}
				else if(dHigherAxisMax<40)
				{
					dHigherAxisMax=40;
				}
				else if(dHigherAxisMax<60)
				{
					dHigherAxisMax=60;
				}
				else if(dHigherAxisMax<80)
				{
					dHigherAxisMax=80;
				}
				else if(dHigherAxisMax<100)
				{
					dHigherAxisMax=100;
				}
				else if(dHigherAxisMax<200)
				{
					dHigherAxisMax=200;
				}
				else if(dHigherAxisMax<300)
				{
					dHigherAxisMax=300;
				}
				else if(dHigherAxisMax<400)
				{
					dHigherAxisMax=400;
				}
				else if(dHigherAxisMax<600)
				{
					dHigherAxisMax=600;
				}
				else
				{
					break;
				}
			}

		}
		break;
	case CO2_GRAPH:
	case CO2_HF_GRAPH:
		{
			if((m_dMinY *(-1))>m_dMaxY)
				dValue=m_dMinY *(-1);

			//VOLUME
			dHigherAxisMax=m_dYAxisMax;

			while (dValue>dHigherAxisMax)
			{
				if(dHigherAxisMax<5)
				{
					dHigherAxisMax=5;
				}
				else if(dHigherAxisMax<10)
				{
					dHigherAxisMax=10;
				}
				else if(dHigherAxisMax<20)
				{
					dHigherAxisMax=20;
				}
				else if(dHigherAxisMax<30)
				{
					dHigherAxisMax=30;
				}
				else if(dHigherAxisMax<40)
				{
					dHigherAxisMax=40;
				}
				else if(dHigherAxisMax<60)
				{
					dHigherAxisMax=60;
				}
				else if(dHigherAxisMax<80)
				{
					dHigherAxisMax=80;
				}
				else if(dHigherAxisMax<100)
				{
					dHigherAxisMax=100;
				}
				else if(dHigherAxisMax<200)
				{
					dHigherAxisMax=200;
				}
				else if(dHigherAxisMax<300)
				{
					dHigherAxisMax=300;
				}
				else if(dHigherAxisMax<400)
				{
					dHigherAxisMax=400;
				}
				else
				{
					break;
				}
			}

		}
		break;
	case SPO2_GRAPH:
		{
			if((m_dMinY *(-1))>m_dMaxY)
				dValue=m_dMinY *(-1);

			dHigherAxisMax=m_dYAxisMax;

			while (dValue>dHigherAxisMax)
			{
				if(dHigherAxisMax<5)
				{
					dHigherAxisMax=5;
				}
				else if(dHigherAxisMax<10)
				{
					dHigherAxisMax=10;
				}
				else if(dHigherAxisMax<15)
				{
					dHigherAxisMax=15;
				}
				else if(dHigherAxisMax<20)
				{
					dHigherAxisMax=20;
				}
				else if(dHigherAxisMax<30)
				{
					dHigherAxisMax=30;
				}
				else if(dHigherAxisMax<40)
				{
					dHigherAxisMax=40;
				}
				else if(dHigherAxisMax<50)
				{
					dHigherAxisMax=50;
				}
				else if(dHigherAxisMax<60)
				{
					dHigherAxisMax=60;
				}
				else if(dHigherAxisMax<70)
				{
					dHigherAxisMax=70;
				}
				else if(dHigherAxisMax<80)
				{
					dHigherAxisMax=80;
				}
				else
				{
					break;
				}
			}

		}
		break;
	case FOT_LOOP:
		{
			double test=m_dMinY;

			if(m_dMaxY>=0)
			{
				double dValue=m_dMaxY;
				dHigherAxisMax=m_dYAxisMax;

				while (dValue>=dHigherAxisMax)
				{
					if(dHigherAxisMax<10)
					{
						dHigherAxisMax=10;
					}
					else if(dHigherAxisMax<20)
					{
						dHigherAxisMax=20;
					}
					else if(dHigherAxisMax<40)
					{
						dHigherAxisMax=40;
					}
					else if(dHigherAxisMax<60)
					{
						dHigherAxisMax=60;
					}
					else if(dHigherAxisMax<80)
					{
						dHigherAxisMax=80;
					}
					else if(dHigherAxisMax<100)
					{
						dHigherAxisMax=100;
						break;
					}
					else//should never occur
					{
						break;
					}
				}
			}
			else
			{
				double dValue=m_dMaxY;
				dHigherAxisMax=m_dYAxisMax;

				while (dValue>dHigherAxisMax)
				{
					if(dHigherAxisMax>-10)
					{
						dHigherAxisMax=0;
					}
					else if(dHigherAxisMax>-20)
					{
						dHigherAxisMax=-10;
					}
					else if(dHigherAxisMax>-40)
					{
						dHigherAxisMax=-20;
					}
					else if(dHigherAxisMax>-60)
					{
						dHigherAxisMax=-40;
					}
					else if(dHigherAxisMax>-80)
					{
						dHigherAxisMax=-60;
					}
					else if(dHigherAxisMax>-100)
					{
						dHigherAxisMax=-80;
					}
					else if(dHigherAxisMax>-200)
					{
						dHigherAxisMax=-100;
					}
					else if(dHigherAxisMax>-300)
					{
						dHigherAxisMax=-200;
					}
					else if(dHigherAxisMax>-400)
					{
						dHigherAxisMax=-300;
					}
					else if(dHigherAxisMax>-500)
					{
						dHigherAxisMax=-400;
						break;
					}
					else
					{
						break;
					}
				}
			}
		}
		break;
	case PRESSURE_VOLUME_HF_LOOP:
	case VOLUME_HF_GRAPH:
		{
			if((m_dMinY *(-1))>m_dMaxY)
				dValue=m_dMinY *(-1);

			//VOLUME
			dHigherAxisMax=m_dYAxisMax;

			while (dValue>dHigherAxisMax)
			{
				if(dHigherAxisMax<2)// newVG
				{
					dHigherAxisMax=2;
				}
				else if(dHigherAxisMax<5)
				{
					dHigherAxisMax=5;
				}
				else if(dHigherAxisMax<10)
				{
					dHigherAxisMax=10;
				}
				else if(dHigherAxisMax<20)
				{
					dHigherAxisMax=20;
				}
				else if(dHigherAxisMax<30)
				{
					dHigherAxisMax=30;
				}
				else if(dHigherAxisMax<40)
				{
					dHigherAxisMax=40;
				}
				else if(dHigherAxisMax<50)
				{
					dHigherAxisMax=50;
				}
				else if(dHigherAxisMax<60)
				{
					dHigherAxisMax=60;
				}
				else if(dHigherAxisMax<70)
				{
					dHigherAxisMax=70;
				}
				else if(dHigherAxisMax<80)
				{
					dHigherAxisMax=80;
				}
				else
				{
					break;
				}
			}

		}
		break;
	case PRESSURE_GRAPH:
	case PRESSURE_HF_GRAPH:
		{
			if((m_dMinY *(-1))>m_dMaxY)
				dValue=m_dMinY *(-1);

			//PRESSURE
			dHigherAxisMax=m_dYAxisMax;

			while (dValue>dHigherAxisMax)
			{
				if(dHigherAxisMax<10)
				{
					dHigherAxisMax=10;
				}
				else if(dHigherAxisMax<20)
				{
					dHigherAxisMax=20;
				}
				else if(dHigherAxisMax<30)
				{
					dHigherAxisMax=30;
				}
				else if(dHigherAxisMax<40)
				{
					dHigherAxisMax=40;
				}
				else if(dHigherAxisMax<60)
				{
					dHigherAxisMax=60;
				}
				else if(dHigherAxisMax<80)
				{
					dHigherAxisMax=80;
				}
				else if(dHigherAxisMax<100)
				{
					dHigherAxisMax=100;
				}
				else
				{
					break;
				}
			}

		}
		break;
	}

	if(m_iDiagrammType==FOT_LOOP)
	{
		if(dHigherAxisMax>m_pFunctionParams->yMax || dHigherAxisMax<m_pFunctionParams->yMin)
			dHigherAxisMax=m_pFunctionParams->yMax;
	}
	else
	{
		if(dHigherAxisMax>m_pFunctionParams->yMax || dHigherAxisMax==0)
			dHigherAxisMax=m_pFunctionParams->yMax;
	}
	

	return dHigherAxisMax;

}

double CDiagramm::GetHigherYAxisMin()
{
	double dHigherAxisMin=0;

	if(!m_pFunctionParams)
	{
		return dHigherAxisMin;
	}

	double dValue=m_dMinY;

	/*if((m_dMinY *(-1))>m_dMaxY)
		dValue=m_dMinY *(-1);*/

	switch(m_iDiagrammType)
	{
	case FOT_LOOP:
		{
			double dValue=m_dMinY;
			dHigherAxisMin=m_dYAxisMin;

			while (dValue<=dHigherAxisMin)
			{
				if(dHigherAxisMin>=100)
				{
					dHigherAxisMin=80;
				}
				else if(dHigherAxisMin>80)
				{
					dHigherAxisMin=80;
				}
				else if(dHigherAxisMin>60)
				{
					dHigherAxisMin=60;
				}
				else if(dHigherAxisMin>40)
				{
					dHigherAxisMin=40;
				}
				else if(dHigherAxisMin>20)
				{
					dHigherAxisMin=20;
				}
				else if(dHigherAxisMin>10)
				{
					dHigherAxisMin=10;
				}
				else if(dHigherAxisMin>0)
				{
					dHigherAxisMin=0;
				}
				else if(dHigherAxisMin>-10)
				{
					dHigherAxisMin=-10;
				}
				else if(dHigherAxisMin>-20)
				{
					dHigherAxisMin=-20;
				}
				else if(dHigherAxisMin>-40)
				{
					dHigherAxisMin=-40;
				}
				else if(dHigherAxisMin>-60)
				{
					dHigherAxisMin=-60;
				}
				else if(dHigherAxisMin>-80)
				{
					dHigherAxisMin=-80;
				}
				else if(dHigherAxisMin>-100)
				{
					dHigherAxisMin=-100;
				}
				else if(dHigherAxisMin>-200)
				{
					dHigherAxisMin=-200;
				}
				else if(dHigherAxisMin>-300)
				{
					dHigherAxisMin=-300;
				}
				else if(dHigherAxisMin>-400)
				{
					dHigherAxisMin=-400;
				}
				else
				{
					dHigherAxisMin=-500;
					break;
				}
			}
		}
		break;
	}

	if(m_iDiagrammType==FOT_LOOP)
	{
		if(dHigherAxisMin<m_pFunctionParams->yMin || dHigherAxisMin>m_pFunctionParams->yMax)
			dHigherAxisMin=m_pFunctionParams->yMin;
	}
	else
	{
		if(dHigherAxisMin<m_pFunctionParams->yMin || dHigherAxisMin>m_pFunctionParams->yMax)
			dHigherAxisMin=m_pFunctionParams->yMin;
	}


	return dHigherAxisMin;

}


// **************************************************************************
// 
// **************************************************************************
bool CDiagramm::IncreaseXScaleToNextValue()
{
	if(m_bExit)
		return false;
	bool bResult=false;
	double dHigherXAxisMax=GetNextHigherXAxisMax();

	if(m_dXAxisMax!=dHigherXAxisMax)
	{
		bResult=true;
		
		switch(m_iDiagrammType)
		{
		case PRESSURE_VOLUME_LOOP:
			{
				SetXAxisScale(GetNegativeYoffset(dHigherXAxisMax), dHigherXAxisMax, true);
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_PVLOOP(dHigherXAxisMax);
			}
			break;
		case VOLUME_FLOW_LOOP:
			{
				SetXAxisScale(GetNegativeYoffset(dHigherXAxisMax), dHigherXAxisMax, true);
				getModel()->getCONFIG()->SetMAXSCALE_VOLUME_VFLOOP(dHigherXAxisMax);
			}
			break;
		case PRESSURE_VOLUME_HF_LOOP:
			{
				SetXAxisScale(m_dXAxisMin, dHigherXAxisMax, true);
				//SetXAxisScale(GetNegativeYoffset(dHigherXAxisMax), dHigherXAxisMax, true);
				getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFPVLOOP(GetNegativeYoffset(dHigherXAxisMax));
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFPVLOOP(dHigherXAxisMax);
			}
			break;
		case VOLUME_FLOW_HF_LOOP:
			{
				SetXAxisScale(GetNegativeYoffset(dHigherXAxisMax), dHigherXAxisMax, true);
				getModel()->getCONFIG()->SetMAXSCALE_VOLUME_HFVFLOOP(dHigherXAxisMax);
			}
			break;
		default:
			break;
		}
	}
	return bResult;
}

// **************************************************************************
// 
// **************************************************************************
bool CDiagramm::IncreaseYScaleToNextValue()
{
	if(m_bExit)
		return false;
	bool bResult=false;
	double dHigherYAxisMax=GetNextHigherYAxisMax();

	if(m_dYAxisMax!=dHigherYAxisMax)
	{
		bResult=true;
		
		switch(m_iDiagrammType)
		{
		case PRESSURE_GRAPH:
			{
				SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, TRUE);
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_GRAPH(dHigherYAxisMax);
			}
			break;
		case PRESSURE_HF_GRAPH:
			{
				//SetYAxisScale(m_dYAxisMax, dHigherYAxisMax, true);
				SetYAxisScale(m_dYAxisMin, dHigherYAxisMax, true);
				getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFGRAPH(m_dYAxisMin);
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFGRAPH(dHigherYAxisMax);
			}
			break;
		case VOLUME_GRAPH:
			{
				SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, TRUE);
				getModel()->getCONFIG()->SetMAXSCALE_VOLUME_GRAPH(dHigherYAxisMax);
			}
			break;
		case VOLUME_HF_GRAPH:
			{
				SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, TRUE);
				getModel()->getCONFIG()->SetMAXSCALE_VOLUME_HFGRAPH(dHigherYAxisMax);
			}
			break;
		case SPO2_GRAPH:
			{
				SetYAxisScale(dHigherYAxisMax*(-1), dHigherYAxisMax, TRUE);
				//SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, true);
				getModel()->getCONFIG()->SetMAXSCALE_SPO2_GRAPH(dHigherYAxisMax);
			}
			break;
		case FOT_LOOP://allways negative
			{
				//SetYAxisScale(dHigherYAxisMax, 0, TRUE);
				SetYAxisScale(dHigherYAxisMax, G_UPPER_MAXSCALE_XRS_FOT, TRUE);//rku, check FOTGRAPH
				getModel()->getCONFIG()->SetMINSCALE_FOT_XRS(dHigherYAxisMax);
			}
			break;
		case CO2_GRAPH:
			{
				SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, TRUE);
				getModel()->getCONFIG()->SetMAXSCALE_CO2_GRAPH(dHigherYAxisMax);
			}
			break;
		case CO2_HF_GRAPH:
			{
				SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, TRUE);
				getModel()->getCONFIG()->SetMAXSCALE_CO2_HFGRAPH(dHigherYAxisMax);
			}
			break;
		case FLOW_GRAPH:
			{
				SetYAxisScale(dHigherYAxisMax*(-1), dHigherYAxisMax, TRUE);
				getModel()->getCONFIG()->SetMAXSCALE_FLOW_GRAPH(dHigherYAxisMax);
			}
			break;
		case FLOW_HF_GRAPH:
			{
				SetYAxisScale(dHigherYAxisMax*(-1), dHigherYAxisMax, TRUE);
				getModel()->getCONFIG()->SetMAXSCALE_FLOW_HFGRAPH(dHigherYAxisMax);
			}
			break;
		case PRESSURE_VOLUME_LOOP:
			{
				SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, TRUE);
				getModel()->getCONFIG()->SetMAXSCALE_VOLUME_PVLOOP(dHigherYAxisMax);
			}
			break;
		case PRESSURE_VOLUME_HF_LOOP:
			{
				SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, TRUE);
				getModel()->getCONFIG()->SetMAXSCALE_VOLUME_HFPVLOOP(dHigherYAxisMax);
			}
			break;
		case VOLUME_FLOW_LOOP:
			{
				SetYAxisScale(dHigherYAxisMax*(-1), dHigherYAxisMax, TRUE);
				getModel()->getCONFIG()->SetMAXSCALE_FLOW_VFLOOP(dHigherYAxisMax);
			}
			break;
		case VOLUME_FLOW_HF_LOOP:
			{
				SetYAxisScale(dHigherYAxisMax*(-1), dHigherYAxisMax, TRUE);
				getModel()->getCONFIG()->SetMAXSCALE_FLOW_HFVFLOOP(dHigherYAxisMax);
			}
			break;
		default:
			break;
		}
	}
	return bResult;
}

// **************************************************************************
// 
// **************************************************************************
bool CDiagramm::DecreaseXScale(bool bTry, bool bRedrawDiagrammData)
{
	if(m_bExit)
		return false;
	bool bRes=false;
	bool bLowerXAxisMax=false;
	double dLowerXAxisMax=0;

	if(!m_pFunctionParams)
	{
		return dLowerXAxisMax;
	}

	switch(m_iDiagrammType)
	{
	case PRESSURE_VOLUME_LOOP:
		{
			if(m_dXAxisMax>=100)
			{
				dLowerXAxisMax=80;
			}
			else if(m_dXAxisMax>=80)
			{
				dLowerXAxisMax=60;
			}
			else if(m_dXAxisMax>=60)
			{
				dLowerXAxisMax=40;
			}
			else if(m_dXAxisMax>=40)
			{
				dLowerXAxisMax=30;
			}
			else if(m_dXAxisMax>=30)
			{
				dLowerXAxisMax=20;
			}
			else if(m_dXAxisMax>=20)
			{
				dLowerXAxisMax=10;
			}
			else
			{
				dLowerXAxisMax=10;
			}

			if(dLowerXAxisMax<m_pFunctionParams->xMin || dLowerXAxisMax==0)
				dLowerXAxisMax=m_pFunctionParams->xMin;

			if(bTry)
			{
				if(		m_dMaxX<dLowerXAxisMax
					&&	m_dXAxisMax>dLowerXAxisMax
					&&	m_dXAxisMax!=dLowerXAxisMax)
				{
					SetXAxisScale(GetNegativeYoffset(dLowerXAxisMax), dLowerXAxisMax, true, bRedrawDiagrammData);
					getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_PVLOOP(dLowerXAxisMax);

					bRes=true;
				}
			}
			else
			{
				if(m_dXAxisMax!=dLowerXAxisMax)
				{
					SetXAxisScale(GetNegativeYoffset(dLowerXAxisMax), dLowerXAxisMax, true, bRedrawDiagrammData);
					getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_PVLOOP(dLowerXAxisMax);

					bRes=true;
				}
			}
			
		}
		break;
	case VOLUME_FLOW_LOOP:
		{
			if(m_dXAxisMax>=600)
			{
				dLowerXAxisMax=400;
			}
			else if(m_dXAxisMax>=400)
			{
				dLowerXAxisMax=300;
			}
			else if(m_dXAxisMax>=300)
			{
				dLowerXAxisMax=200;
			}
			else if(m_dXAxisMax>=200)
			{
				dLowerXAxisMax=100;
			}
			else if(m_dXAxisMax>=100)
			{
				dLowerXAxisMax=80;
			}
			else if(m_dXAxisMax>=80)
			{
				dLowerXAxisMax=60;
			}
			else if(m_dXAxisMax>=60)
			{
				dLowerXAxisMax=40;
			}
			else if(m_dXAxisMax>=40)
			{
				dLowerXAxisMax=30;
			}
			else if(m_dXAxisMax>=30)
			{
				dLowerXAxisMax=20;
			}
			else if(m_dXAxisMax>=20)
			{
				dLowerXAxisMax=10;
			}
			else if(m_dXAxisMax>=10)
			{
				dLowerXAxisMax=5;
			}
			else if(m_dXAxisMax>=5)//newVG
			{
				dLowerXAxisMax=2;
			}
			else
			{
				dLowerXAxisMax=2;
			}

			if(dLowerXAxisMax<m_pFunctionParams->xMin || dLowerXAxisMax==0)
				dLowerXAxisMax=m_pFunctionParams->xMin;

			if(bTry)
			{
				if(		m_dMaxX<dLowerXAxisMax
					&&	m_dXAxisMax>dLowerXAxisMax
					&&	m_dXAxisMax!=dLowerXAxisMax)
				{
					SetXAxisScale(GetNegativeYoffset(dLowerXAxisMax), dLowerXAxisMax, true, bRedrawDiagrammData);
					getModel()->getCONFIG()->SetMAXSCALE_VOLUME_VFLOOP(dLowerXAxisMax);

					bRes=true;
				}
			}
			else
			{
				if(m_dXAxisMax!=dLowerXAxisMax)
				{
					SetXAxisScale(GetNegativeYoffset(dLowerXAxisMax), dLowerXAxisMax, true, bRedrawDiagrammData);
					getModel()->getCONFIG()->SetMAXSCALE_VOLUME_VFLOOP(dLowerXAxisMax);

					bRes=true;
				}
			}
			
		}
		break;
	case VOLUME_FLOW_HF_LOOP:
		{
			if(m_dXAxisMax>=80)
			{
				dLowerXAxisMax=70;
			}
			else if(m_dXAxisMax>=70)
			{
				dLowerXAxisMax=60;
			}
			else if(m_dXAxisMax>=60)
			{
				dLowerXAxisMax=50;
			}
			else if(m_dXAxisMax>=50)
			{
				dLowerXAxisMax=40;
			}
			else if(m_dXAxisMax>=40)
			{
				dLowerXAxisMax=30;
			}
			else if(m_dXAxisMax>=30)
			{
				dLowerXAxisMax=20;
			}
			else if(m_dXAxisMax>=20)
			{
				dLowerXAxisMax=10;
			}
			else if(m_dXAxisMax>=10)
			{
				dLowerXAxisMax=5;
			}
			else if(m_dXAxisMax>=5)
			{
				dLowerXAxisMax=2;
			}
			else
			{
				dLowerXAxisMax=2;
			}

			if(dLowerXAxisMax<m_pFunctionParams->xMin || dLowerXAxisMax==0)
				dLowerXAxisMax=m_pFunctionParams->xMin;

			if(bTry)
			{
				if(		m_dMaxX<dLowerXAxisMax
					&&	m_dXAxisMax>dLowerXAxisMax
					&&	m_dXAxisMax!=dLowerXAxisMax)
				{
					SetXAxisScale(GetNegativeYoffset(dLowerXAxisMax), dLowerXAxisMax, true, bRedrawDiagrammData);
					getModel()->getCONFIG()->SetMAXSCALE_VOLUME_HFVFLOOP(dLowerXAxisMax);

					bRes=true;
				}
			}
			else
			{
				if(m_dXAxisMax!=dLowerXAxisMax)
				{
					SetXAxisScale(GetNegativeYoffset(dLowerXAxisMax), dLowerXAxisMax, true, bRedrawDiagrammData);
					getModel()->getCONFIG()->SetMAXSCALE_VOLUME_HFVFLOOP(dLowerXAxisMax);

					bRes=true;
				}
			}
			
		}
		break;
	case PRESSURE_VOLUME_HF_LOOP:
		{
			if(m_bAutofit)
			{
				//do nothing
			}
			else
			{
				if(m_dXAxisMax>200)
				{
					dLowerXAxisMax=200+m_dXAxisMin;
				}
				else if(m_dXAxisMax>150)
				{
					dLowerXAxisMax=150+m_dXAxisMin;
				}
				else if(m_dXAxisMax>100)
				{
					dLowerXAxisMax=100+m_dXAxisMin;
				}
				else if(m_dXAxisMax>80)
				{
					dLowerXAxisMax=80+m_dXAxisMin;
				}
				else if(m_dXAxisMax>60)
				{
					dLowerXAxisMax=60+m_dXAxisMin;
				}
				else if(m_dXAxisMax>40)
				{
					dLowerXAxisMax=40+m_dXAxisMin;
				}
				else if(m_dXAxisMax>30)
				{
					dLowerXAxisMax=30+m_dXAxisMin;
				}
				else if(m_dXAxisMax>20)
				{
					dLowerXAxisMax=20+m_dXAxisMin;
				}
				else if(m_dXAxisMax>10)
				{
					dLowerXAxisMax=10;
				}
				else
				{
					dLowerXAxisMax=5;
				}

				if(dLowerXAxisMax<m_pFunctionParams->xMin || dLowerXAxisMax==0)
					dLowerXAxisMax=m_pFunctionParams->xMin;


				if(bTry)
				{
					if(		m_dMaxX<dLowerXAxisMax
						&&	m_dXAxisMax>dLowerXAxisMax
						&&	m_dXAxisMax!=dLowerXAxisMax)
					{
						bLowerXAxisMax=true;

						bRes=true;
					}
				}
				else
				{
					if(m_dXAxisMax!=dLowerXAxisMax)
					{
						bLowerXAxisMax=true;

						bRes=true;
					}
				}

				SetXAxisScale(m_dXAxisMin, dLowerXAxisMax, true, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFPVLOOP(m_dXAxisMin);
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFPVLOOP(dLowerXAxisMax);
			}
		}
		break;
	case FOT_LOOP:
		{
			if(m_bAutofit)
			{
				//do nothing
			}
			else
			{
				if(m_dXAxisMax>200)
				{
					dLowerXAxisMax=200+m_dXAxisMin;
				}
				else if(m_dXAxisMax>150)
				{
					dLowerXAxisMax=150+m_dXAxisMin;
				}
				else if(m_dXAxisMax>100)
				{
					dLowerXAxisMax=100+m_dXAxisMin;
				}
				else if(m_dXAxisMax>80)
				{
					dLowerXAxisMax=80+m_dXAxisMin;
				}
				else if(m_dXAxisMax>60)
				{
					dLowerXAxisMax=60+m_dXAxisMin;
				}
				else if(m_dXAxisMax>40)
				{
					dLowerXAxisMax=40+m_dXAxisMin;
				}
				else if(m_dXAxisMax>30)
				{
					dLowerXAxisMax=30+m_dXAxisMin;
				}
				else if(m_dXAxisMax>20)
				{
					dLowerXAxisMax=20+m_dXAxisMin;
				}
				else if(m_dXAxisMax>10)
				{
					dLowerXAxisMax=10;
				}
				else
				{
					dLowerXAxisMax=5;
				}

				if(dLowerXAxisMax<m_pFunctionParams->xMin || dLowerXAxisMax==0)
					dLowerXAxisMax=m_pFunctionParams->xMin;


				if(bTry)
				{
					if(		m_dMaxX<dLowerXAxisMax
						&&	m_dXAxisMax>dLowerXAxisMax
						&&	m_dXAxisMax!=dLowerXAxisMax)
					{
						bLowerXAxisMax=true;

						bRes=true;
					}
				}
				else
				{
					if(m_dXAxisMax!=dLowerXAxisMax)
					{
						bLowerXAxisMax=true;

						bRes=true;
					}
				}

				SetXAxisScale(m_dXAxisMin, dLowerXAxisMax, true, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFPVLOOP(m_dXAxisMin);
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFPVLOOP(dLowerXAxisMax);
			}
		}
		break;
	default:
		{
		}
		break;
	}
	return bRes;
}

// **************************************************************************
// 
// **************************************************************************
bool CDiagramm::DecreaseYScaleToNextValue(bool bRedrawDiagrammData)
{
	if(m_bExit)
		return false;
	bool bResult=false;
	

	switch(m_iDiagrammType)
	{
	case PRESSURE_GRAPH:
		{
			double dLowerYAxisMax=GetNextLowerYAxisMax();

			if(m_dYAxisMax!=dLowerYAxisMax)
			{
				bResult=true;
				SetYAxisScale(GetNegativeYoffset(dLowerYAxisMax), dLowerYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_GRAPH(dLowerYAxisMax);
			}
		}
		break;
	case PRESSURE_HF_GRAPH:
		{
			double dLowerYAxisMax=GetNextLowerYAxisMax();

			if(m_dYAxisMax!=dLowerYAxisMax)
			{
				bResult=true;
				SetYAxisScale(m_dYAxisMin, dLowerYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFGRAPH(m_dYAxisMin);
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFGRAPH(dLowerYAxisMax);
			}
		}
		break;
	case VOLUME_GRAPH:
		{
			double dLowerYAxisMax=GetNextLowerYAxisMax();

			if(m_dYAxisMax!=dLowerYAxisMax)
			{
				bResult=true;
				SetYAxisScale(GetNegativeYoffset(dLowerYAxisMax), dLowerYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_VOLUME_GRAPH(dLowerYAxisMax);
			}
		}
		break;
	case VOLUME_HF_GRAPH:
		{
			double dLowerYAxisMax=GetNextLowerYAxisMax();

			if(m_dYAxisMax!=dLowerYAxisMax)
			{
				bResult=true;
				SetYAxisScale(GetNegativeYoffset(dLowerYAxisMax), dLowerYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_VOLUME_HFGRAPH(dLowerYAxisMax);
			}
		}
		break;
	case SPO2_GRAPH:
		{
			double dLowerYAxisMax=GetNextLowerYAxisMax();

			if(m_dYAxisMax!=dLowerYAxisMax)
			{
				bResult=true;
				SetYAxisScale(dLowerYAxisMax*(-1), dLowerYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_SPO2_GRAPH(dLowerYAxisMax);
			}
		}
		break;
	case FOT_LOOP:
		{
			double dLowerYAxisMax=GetNextLowerYAxisMax();
			double dHigherYAxisMin=GetNextHigherYAxisMin();

			if(m_dYAxisMax!=dLowerYAxisMax || m_dYAxisMin!=dHigherYAxisMin)
			{
				bResult=true;

				if(dHigherYAxisMin<0 && dLowerYAxisMax<0)
					dLowerYAxisMax=0;
				else if(dHigherYAxisMin>0 && dLowerYAxisMax>0)
					dHigherYAxisMin=0;

				SetYAxisScale(dHigherYAxisMin, dLowerYAxisMax, TRUE, bRedrawDiagrammData);//rku, check FOTGRAPH
				getModel()->getCONFIG()->SetMINSCALE_FOT_XRS(dLowerYAxisMax);
				getModel()->getCONFIG()->SetMAXSCALE_FOT_XRS(dHigherYAxisMin);
			}
		}
		break;
	case CO2_GRAPH:
		{
			double dLowerYAxisMax=GetNextLowerYAxisMax();

			if(m_dYAxisMax!=dLowerYAxisMax)
			{
				bResult=true;
				SetYAxisScale(GetNegativeYoffset(dLowerYAxisMax), dLowerYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_CO2_GRAPH(dLowerYAxisMax);
			}
		}
		break;
	case CO2_HF_GRAPH:
		{
			double dLowerYAxisMax=GetNextLowerYAxisMax();

			if(m_dYAxisMax!=dLowerYAxisMax)
			{
				bResult=true;
				SetYAxisScale(GetNegativeYoffset(dLowerYAxisMax), dLowerYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_CO2_HFGRAPH(dLowerYAxisMax);
			}
		}
		break;
	case FLOW_GRAPH:
		{
			double dHigherYAxisMax=GetNextLowerYAxisMax();

			if(m_dYAxisMax!=dHigherYAxisMax)
			{
				bResult=true;
				SetYAxisScale(dHigherYAxisMax*(-1), dHigherYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_FLOW_GRAPH(dHigherYAxisMax);
			}
		}
		break;
	case FLOW_HF_GRAPH:
		{
			double dLowerYAxisMax=GetNextLowerYAxisMax();

			if(m_dYAxisMax!=dLowerYAxisMax)
			{
				bResult=true;
				SetYAxisScale(dLowerYAxisMax*(-1), dLowerYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_FLOW_HFGRAPH(dLowerYAxisMax);
			}
		}
		break;
	case PRESSURE_VOLUME_LOOP:
		{
			double dLowerYAxisMax=GetNextLowerYAxisMax();

			if(m_dYAxisMax!=dLowerYAxisMax)
			{
				bResult=true;
				SetYAxisScale(GetNegativeYoffset(dLowerYAxisMax), dLowerYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_VOLUME_PVLOOP(dLowerYAxisMax);
			}
		}
		break;
	case PRESSURE_VOLUME_HF_LOOP:
		{
			double dLowerYAxisMax=GetNextLowerYAxisMax();

			if(m_dYAxisMax!=dLowerYAxisMax)
			{
				bResult=true;
				SetYAxisScale(GetNegativeYoffset(dLowerYAxisMax), dLowerYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_VOLUME_HFPVLOOP(dLowerYAxisMax);
			}
		}
		break;
	case VOLUME_FLOW_LOOP:
		{
			double dLowerYAxisMax=GetNextLowerYAxisMax();

			if(m_dYAxisMax!=dLowerYAxisMax)
			{
				bResult=true;
				SetYAxisScale(dLowerYAxisMax*(-1), dLowerYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_FLOW_VFLOOP(dLowerYAxisMax);
			}
		}
		break;
	case VOLUME_FLOW_HF_LOOP:
		{
			double dLowerYAxisMax=GetNextLowerYAxisMax();

			if(m_dYAxisMax!=dLowerYAxisMax)
			{
				bResult=true;
				SetYAxisScale(dLowerYAxisMax*(-1), dLowerYAxisMax, TRUE, bRedrawDiagrammData);
				getModel()->getCONFIG()->SetMAXSCALE_FLOW_HFVFLOOP(dLowerYAxisMax);
			}
		}
		break;
	default:
		break;
	}
	return bResult;
}

// **************************************************************************
// 
// **************************************************************************
double CDiagramm::GetNextHigherXAxisMax()
{
	double dHigherAxisMax=0;

	if(!m_pFunctionParams)
	{
		return dHigherAxisMax;
	}

	double dDiff = m_dXAxisMax-m_dXAxisMin;

	switch(m_iDiagrammType)
	{
	case VOLUME_FLOW_LOOP:
		{
			//VOLUME
			if(m_dXAxisMax<2)//newVG
			{
				dHigherAxisMax=2;
			}
			else if(m_dXAxisMax<5)
			{
				dHigherAxisMax=5;
			}
			else if(m_dXAxisMax<10)
			{
				dHigherAxisMax=10;
			}
			else if(m_dXAxisMax<20)
			{
				dHigherAxisMax=20;
			}
			else if(m_dXAxisMax<30)
			{
				dHigherAxisMax=30;
			}
			else if(m_dXAxisMax<40)
			{
				dHigherAxisMax=40;
			}
			else if(m_dXAxisMax<60)
			{
				dHigherAxisMax=60;
			}
			else if(m_dXAxisMax<80)
			{
				dHigherAxisMax=80;
			}
			else if(m_dXAxisMax<100)
			{
				dHigherAxisMax=100;
			}
			else if(m_dXAxisMax<200)
			{
				dHigherAxisMax=200;
			}
			else if(m_dXAxisMax<300)
			{
				dHigherAxisMax=300;
			}
			else if(m_dXAxisMax<400)
			{
				dHigherAxisMax=400;
			}
			else
			{
				dHigherAxisMax=600;
			}
		}
		break;
	case VOLUME_FLOW_HF_LOOP:
		{
			//VOLUME
			if(m_dXAxisMax<5)
			{
				dHigherAxisMax=5;
			}
			else if(m_dXAxisMax<10)
			{
				dHigherAxisMax=10;
			}
			else if(m_dXAxisMax<20)
			{
				dHigherAxisMax=20;
			}
			else if(m_dXAxisMax<30)
			{
				dHigherAxisMax=30;
			}
			else if(m_dXAxisMax<40)
			{
				dHigherAxisMax=40;
			}
			else if(m_dXAxisMax<50)
			{
				dHigherAxisMax=50;
			}
			else if(m_dXAxisMax<60)
			{
				dHigherAxisMax=60;
			}
			else if(m_dXAxisMax<70)
			{
				dHigherAxisMax=70;
			}
			else 
			{
				dHigherAxisMax=80;
			}
		}
		break;
	case PRESSURE_VOLUME_LOOP:
		{
			//PRESSURE
			if(m_dXAxisMax<10)
			{
				dHigherAxisMax=10;
			}
			else if(m_dXAxisMax<20)
			{
				dHigherAxisMax=20;
			}
			else if(m_dXAxisMax<30)
			{
				dHigherAxisMax=30;
			}
			else if(m_dXAxisMax<40)
			{
				dHigherAxisMax=40;
			}
			else if(m_dXAxisMax<60)
			{
				dHigherAxisMax=60;
			}
			else if(m_dXAxisMax<80)
			{
				dHigherAxisMax=80;
			}
			else
			{
				dHigherAxisMax=100;
			}
		}
		break;
	case PRESSURE_VOLUME_HF_LOOP:
		{
			//PRESSURE
			if(dDiff<10)
			{
				dHigherAxisMax=10+m_dXAxisMin;
			}
			else if(dDiff<20)
			{
				dHigherAxisMax=20+m_dXAxisMin;
			}
			else if(dDiff<30)
			{
				dHigherAxisMax=30+m_dXAxisMin;
			}
			else if(dDiff<40)
			{
				dHigherAxisMax=40+m_dXAxisMin;
			}
			else if(dDiff<60)
			{
				dHigherAxisMax=60+m_dXAxisMin;
			}
			else if(dDiff<80)
			{
				dHigherAxisMax=80+m_dXAxisMin;
			}
			else if(dDiff<100)
			{
				dHigherAxisMax=100+m_dXAxisMin;
			}
			else if(dDiff<150)
			{
				dHigherAxisMax=150+m_dXAxisMin;
			}
			else
			{
				dHigherAxisMax=200+m_dXAxisMin;
			}
		}
		break;
	case FOT_LOOP:
		{
			//PRESSURE
			if(dDiff<10)
			{
				dHigherAxisMax=10+m_dXAxisMin;
			}
			else if(dDiff<20)
			{
				dHigherAxisMax=20+m_dXAxisMin;
			}
			else if(dDiff<30)
			{
				dHigherAxisMax=30+m_dXAxisMin;
			}
			else if(dDiff<40)
			{
				dHigherAxisMax=40+m_dXAxisMin;
			}
			else if(dDiff<60)
			{
				dHigherAxisMax=60+m_dXAxisMin;
			}
			else if(dDiff<80)
			{
				dHigherAxisMax=80+m_dXAxisMin;
			}
			else if(dDiff<100)
			{
				dHigherAxisMax=100+m_dXAxisMin;
			}
			else if(dDiff<150)
			{
				dHigherAxisMax=150+m_dXAxisMin;
			}
			else
			{
				dHigherAxisMax=200+m_dXAxisMin;
			}
		}
		break;
	case FLOW_GRAPH:
	case FLOW_HF_GRAPH:
	case VOLUME_GRAPH:
	case VOLUME_HF_GRAPH:
	case PRESSURE_GRAPH:
	case PRESSURE_HF_GRAPH:
	case SPO2_GRAPH:
	case CO2_GRAPH:
	case CO2_HF_GRAPH:
	default:
		{
		}
		break;
	}

	if(dHigherAxisMax>m_pFunctionParams->xMax || dHigherAxisMax==0)
		dHigherAxisMax=m_pFunctionParams->xMax;

	return dHigherAxisMax;

}

// **************************************************************************
// 
// **************************************************************************
double CDiagramm::GetNextHigherYAxisMax()
{
	double dHigherAxisMax=0;

	if(!m_pFunctionParams)
	{
		return dHigherAxisMax;
	}
	double dDiff = m_dYAxisMax-m_dYAxisMin;

	switch(m_iDiagrammType)
	{
	case VOLUME_FLOW_LOOP:
	case VOLUME_FLOW_HF_LOOP:
	case FLOW_GRAPH:
	case FLOW_HF_GRAPH:
		{
			//FLOW
			if(m_dYAxisMax<5)
			{
				dHigherAxisMax=5;
			}
			else if(m_dYAxisMax<10)
			{
				dHigherAxisMax=10;
			}
			else if(m_dYAxisMax<15)
			{
				dHigherAxisMax=15;
			}
			else if(m_dYAxisMax<20)
			{
				dHigherAxisMax=20;
			}
			else if(m_dYAxisMax<30)
			{
				dHigherAxisMax=30;
			}
			else if(m_dYAxisMax<40)
			{
				dHigherAxisMax=40;
			}
			else if(m_dYAxisMax<50)
			{
				dHigherAxisMax=50;
			}
			else if(m_dYAxisMax<60)
			{
				dHigherAxisMax=60;
			}
			else if(m_dYAxisMax<70)
			{
				dHigherAxisMax=70;
			}
			else
			{
				dHigherAxisMax=G_UPPER_MAXSCALE_FLOW;
			}
		}
		break;
	case PRESSURE_GRAPH:
		{
			//PRESSURE
			if(m_dYAxisMax<10)
			{
				dHigherAxisMax=10;
			}
			else if(m_dYAxisMax<20)
			{
				dHigherAxisMax=20;
			}
			else if(m_dYAxisMax<30)
			{
				dHigherAxisMax=30;
			}
			else if(m_dYAxisMax<40)
			{
				dHigherAxisMax=40;
			}
			else if(m_dYAxisMax<60)
			{
				dHigherAxisMax=60;
			}
			else if(m_dYAxisMax<80)
			{
				dHigherAxisMax=80;
			}
			else
			{
				dHigherAxisMax=100;
			}
		}
		break;
	case PRESSURE_HF_GRAPH:
		{
			//PRESSURE
			if(dDiff<10)
			{
				dHigherAxisMax=10+m_dYAxisMin;
			}
			else if(dDiff<20)
			{
				dHigherAxisMax=20+m_dYAxisMin;
			}
			else if(dDiff<30)
			{
				dHigherAxisMax=30+m_dYAxisMin;
			}
			else if(dDiff<40)
			{
				dHigherAxisMax=40+m_dYAxisMin;
			}
			else if(dDiff<60)
			{
				dHigherAxisMax=60+m_dYAxisMin;
			}
			else if(dDiff<80)
			{
				dHigherAxisMax=80+m_dYAxisMin;
			}
			else if(dDiff<100)
			{
				dHigherAxisMax=100+m_dYAxisMin;
			}
			else if(dDiff<150)
			{
				dHigherAxisMax=150+m_dYAxisMin;
			}
			else
			{
				dHigherAxisMax=200+m_dYAxisMin;
			}
		}
		break;
	case VOLUME_GRAPH:
	case PRESSURE_VOLUME_LOOP:
		{
			//VOLUME
			if(m_dYAxisMax<2)//newVG
			{
				dHigherAxisMax=2;
			}
			else if(m_dYAxisMax<10)
			{
				dHigherAxisMax=10;
			}
			else if(m_dYAxisMax<20)
			{
				dHigherAxisMax=20;
			}
			else if(m_dYAxisMax<30)
			{
				dHigherAxisMax=30;
			}
			else if(m_dYAxisMax<40)
			{
				dHigherAxisMax=40;
			}
			else if(m_dYAxisMax<60)
			{
				dHigherAxisMax=60;
			}
			else if(m_dYAxisMax<80)
			{
				dHigherAxisMax=80;
			}
			else if(m_dYAxisMax<100)
			{
				dHigherAxisMax=100;
			}
			else if(m_dYAxisMax<200)
			{
				dHigherAxisMax=200;
			}
			else if(m_dYAxisMax<300)
			{
				dHigherAxisMax=300;
			}
			else if(m_dYAxisMax<400)
			{
				dHigherAxisMax=400;
			}
			else
			{
				dHigherAxisMax=600;
			}
		}
		break;
	case CO2_GRAPH:
	case CO2_HF_GRAPH:
		{
			//VOLUME
			if(m_dYAxisMax<10)
			{
				dHigherAxisMax=10;
			}
			else if(m_dYAxisMax<20)
			{
				dHigherAxisMax=20;
			}
			else if(m_dYAxisMax<30)
			{
				dHigherAxisMax=30;
			}
			else if(m_dYAxisMax<40)
			{
				dHigherAxisMax=40;
			}
			else if(m_dYAxisMax<60)
			{
				dHigherAxisMax=60;
			}
			else if(m_dYAxisMax<80)
			{
				dHigherAxisMax=80;
			}
			else if(m_dYAxisMax<100)
			{
				dHigherAxisMax=100;
			}
			else if(m_dYAxisMax<200)
			{
				dHigherAxisMax=200;
			}
			else if(m_dYAxisMax<300)
			{
				dHigherAxisMax=300;
			}
			else
			{
				dHigherAxisMax=400;
			}
		}
		break;
	case SPO2_GRAPH:
		{
			//FLOW
			if(m_dYAxisMax<5)
			{
				dHigherAxisMax=5;
			}
			else if(m_dYAxisMax<10)
			{
				dHigherAxisMax=10;
			}
			else if(m_dYAxisMax<15)
			{
				dHigherAxisMax=15;
			}
			else if(m_dYAxisMax<20)
			{
				dHigherAxisMax=20;
			}
			else if(m_dYAxisMax<30)
			{
				dHigherAxisMax=30;
			}
			else if(m_dYAxisMax<40)
			{
				dHigherAxisMax=40;
			}
			else if(m_dYAxisMax<50)
			{
				dHigherAxisMax=50;
			}
			else if(m_dYAxisMax<60)
			{
				dHigherAxisMax=60;
			}
			else if(m_dYAxisMax<70)
			{
				dHigherAxisMax=70;
			}
			else
			{
				dHigherAxisMax=G_UPPER_MAXSCALE_SPO2;
			}
		}
		break;
	case FOT_LOOP://:GetNextHigherYAxisMax()
		{
			if(m_dYAxisMax>-10)
			{
				dHigherAxisMax=-10;
			}
			else if(m_dYAxisMax>-20)
			{
				dHigherAxisMax=-20;
			}
			else if(m_dYAxisMax>-40)
			{
				dHigherAxisMax=-40;
			}
			else if(m_dYAxisMax>-60)
			{
				dHigherAxisMax=-60;
			}
			else if(m_dYAxisMax>-80)
			{
				dHigherAxisMax=-80;
			}
			else if(m_dYAxisMax>-100)
			{
				dHigherAxisMax=-100;
			}
			else if(m_dYAxisMax>-200)
			{
				dHigherAxisMax=-200;
			}
			else if(m_dYAxisMax>-300)
			{
				dHigherAxisMax=-300;
			}
			else if(m_dYAxisMax>-400)
			{
				dHigherAxisMax=-400;
			}
			else
			{
				dHigherAxisMax=-500;
			}
		}
		break;
	case PRESSURE_VOLUME_HF_LOOP:
	case VOLUME_HF_GRAPH:
		{
			//VOLUME
			if(m_dYAxisMax<2)//newVG
			{
				dHigherAxisMax=2;
			}
			else if(m_dYAxisMax<5)
			{
				dHigherAxisMax=5;
			}
			else if(m_dYAxisMax<10)
			{
				dHigherAxisMax=10;
			}
			else if(m_dYAxisMax<20)
			{
				dHigherAxisMax=20;
			}
			else if(m_dYAxisMax<30)
			{
				dHigherAxisMax=30;
			}
			else if(m_dYAxisMax<40)
			{
				dHigherAxisMax=40;
			}
			else if(m_dYAxisMax<50)
			{
				dHigherAxisMax=50;
			}
			else if(m_dYAxisMax<60)
			{
				dHigherAxisMax=60;
			}
			else if(m_dYAxisMax<70)
			{
				dHigherAxisMax=70;
			}
			else
			{
				dHigherAxisMax=G_UPPER_MAXSCALE_VOLUME_HF;
			}
		}
		break;
	}

	if(dHigherAxisMax>m_pFunctionParams->yMax || dHigherAxisMax==0)
		dHigherAxisMax=m_pFunctionParams->yMax;

	return dHigherAxisMax;

}



// **************************************************************************
// 
// **************************************************************************
double CDiagramm::GetNextLowerYAxisMax()
{
	double dLowerAxisMax=0;

	if(!m_pFunctionParams)
	{
		return 0;
	}
	double dDiff=m_dYAxisMax-m_dYAxisMin;

	switch(m_iDiagrammType)
	{
	case VOLUME_FLOW_LOOP:
	case VOLUME_FLOW_HF_LOOP:
	case FLOW_GRAPH:
	case FLOW_HF_GRAPH:
		{
			if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-10;
			}
			else if(m_dYAxisMax>10)
			{
				dLowerAxisMax=m_dYAxisMax-5;
			}
			else if(m_dYAxisMax>5)
			{
				dLowerAxisMax=5;
			}
			else
			{
				dLowerAxisMax=2;
			}
		}
		break;
	case VOLUME_GRAPH:
	case PRESSURE_VOLUME_LOOP:
		{
			if(m_dYAxisMax>=600)
			{
				dLowerAxisMax=400;
			}
			else if(m_dYAxisMax>=400)
			{
				dLowerAxisMax=300;
			}
			else if(m_dYAxisMax>=300)
			{
				dLowerAxisMax=200;
			}
			else if(m_dYAxisMax>=200)
			{
				dLowerAxisMax=100;
			}
			else if(m_dYAxisMax>=100)
			{
				dLowerAxisMax=80;
			}
			else if(m_dYAxisMax>=80)
			{
				dLowerAxisMax=60;
			}
			else if(m_dYAxisMax>=60)
			{
				dLowerAxisMax=40;
			}
			else if(m_dYAxisMax>=40)
			{
				dLowerAxisMax=30;
			}
			else if(m_dYAxisMax>=30)
			{
				dLowerAxisMax=20;
			}
			else if(m_dYAxisMax>=20)
			{
				dLowerAxisMax=10;
			}
			else if(m_dYAxisMax>=10)//newVG
			{
				dLowerAxisMax=5;
			}
			else if(m_dYAxisMax>=5)//newVG
			{
				dLowerAxisMax=2;
			}
			else
			{
				dLowerAxisMax=2;
			}
		}
		break;
	case CO2_GRAPH:
	case CO2_HF_GRAPH:
		{
			if(m_dYAxisMax>=400)
			{
				dLowerAxisMax=300;
			}
			else if(m_dYAxisMax>=300)
			{
				dLowerAxisMax=200;
			}
			else if(m_dYAxisMax>=200)
			{
				dLowerAxisMax=100;
			}
			else if(m_dYAxisMax>=100)
			{
				dLowerAxisMax=80;
			}
			else if(m_dYAxisMax>=80)
			{
				dLowerAxisMax=60;
			}
			else if(m_dYAxisMax>=60)
			{
				dLowerAxisMax=40;
			}
			else if(m_dYAxisMax>=40)
			{
				dLowerAxisMax=30;
			}
			else if(m_dYAxisMax>=30)
			{
				dLowerAxisMax=20;
			}
			else if(m_dYAxisMax>=20)
			{
				dLowerAxisMax=10;
			}
			else
			{
				dLowerAxisMax=10;
			}
		}
		break;
	case SPO2_GRAPH:
		{
			if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-10;
			}
			else if(m_dYAxisMax>10)
			{
				dLowerAxisMax=m_dYAxisMax-5;
			}
			else if(m_dYAxisMax>5)
			{
				dLowerAxisMax=5;
			}
			else
			{
				dLowerAxisMax=2;
			}
		}
		break;
	case FOT_LOOP:
		{
			double iTempMaxY=m_dMaxY+10;

			if(iTempMaxY<=(-400))
			{
				dLowerAxisMax=(-400);
			}
			else if(iTempMaxY<=(-300))
			{
				dLowerAxisMax=(-300);
			}
			else if(iTempMaxY<=(-200))
			{
				dLowerAxisMax=(-200);
			}
			else if(iTempMaxY<=(-100))
			{
				dLowerAxisMax=(-100);
			}
			else if(iTempMaxY<=(-80))
			{
				dLowerAxisMax=(-80);
			}
			else if(iTempMaxY<=(-60))
			{
				dLowerAxisMax=(-60);
			}
			else if(iTempMaxY<=(-40))
			{
				dLowerAxisMax=(-40);
			}
			else if(iTempMaxY<=(-20))
			{
				dLowerAxisMax=(-20);
			}
			else if(iTempMaxY<=(-10))
			{
				dLowerAxisMax=(-10);
			}
			else if(iTempMaxY<=0)
			{
				dLowerAxisMax=0;
			}
			else if(iTempMaxY<=10)
			{
				dLowerAxisMax=10;
			}
			else if(iTempMaxY<=20)
			{
				dLowerAxisMax=20;
			}
			else if(iTempMaxY<=40)
			{
				dLowerAxisMax=40;
			}
			else if(iTempMaxY<=60)
			{
				dLowerAxisMax=60;
			}
			else if(iTempMaxY<=80)
			{
				dLowerAxisMax=80;
			}
			else 
			{
				dLowerAxisMax=100;
			}
		}
		break;
	case PRESSURE_VOLUME_HF_LOOP:
	case VOLUME_HF_GRAPH:
		{
			if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-10;
			}
			else if(m_dYAxisMax>=10)
			{
				dLowerAxisMax=5;
			}
			else if(m_dYAxisMax>=5)
			{
				dLowerAxisMax=2;
			}
			else
			{
				dLowerAxisMax=2;
			}
		}
		break;
	case PRESSURE_HF_GRAPH:
		{
			if(dDiff>=200)
			{
				dLowerAxisMax=150+m_dYAxisMin;
			}
			else if(dDiff>=150)
			{
				dLowerAxisMax=100+m_dYAxisMin;
			}
			else if(dDiff>=100)
			{
				dLowerAxisMax=80+m_dYAxisMin;
			}
			else if(dDiff>=80)
			{
				dLowerAxisMax=60+m_dYAxisMin;
			}
			else if(dDiff>=60)
			{
				dLowerAxisMax=40+m_dYAxisMin;
			}
			else if(dDiff>=40)
			{
				dLowerAxisMax=30+m_dYAxisMin;
			}
			else if(dDiff>=30)
			{
				dLowerAxisMax=20+m_dYAxisMin;
			}
			else if(dDiff>=20)
			{
				dLowerAxisMax=10+m_dYAxisMin;
			}
			else
			{
				dLowerAxisMax=10+m_dYAxisMin;
			}
		}
		break;
	case PRESSURE_GRAPH:
		{
			if(m_dYAxisMax>=100)
			{
				dLowerAxisMax=80;
			}
			else if(m_dYAxisMax>=80)
			{
				dLowerAxisMax=60;
			}
			else if(m_dYAxisMax>=60)
			{
				dLowerAxisMax=40;
			}
			else if(m_dYAxisMax>=40)
			{
				dLowerAxisMax=30;
			}
			else if(m_dYAxisMax>=30)
			{
				dLowerAxisMax=20;
			}
			else if(m_dYAxisMax>=20)
			{
				dLowerAxisMax=10;
			}
			else
			{
				dLowerAxisMax=10;
			}
		}
		break;
	}

	if(m_iDiagrammType==FOT_LOOP)
	{
		if(dLowerAxisMax<m_pFunctionParams->yMin || dLowerAxisMax>m_pFunctionParams->yMax)
			dLowerAxisMax=m_pFunctionParams->yMax;
	}
	else
	{
		if(dLowerAxisMax<m_pFunctionParams->yMin || dLowerAxisMax==0)
			dLowerAxisMax=m_pFunctionParams->yMin;
	}
	

	return dLowerAxisMax;

}

double CDiagramm::GetNextHigherYAxisMin()
{
	double dHigherAxisMin=0;

	if(!m_pFunctionParams)
	{
		return 0;
	}

	switch(m_iDiagrammType)
	{
	case FOT_LOOP://allways negative! :GetNextLowerYAxisMax()
		{
			double iTempMinY=m_dMinY-10;

			if(iTempMinY>=100)
			{
				dHigherAxisMin=100;
			}
			else if(iTempMinY>=80)
			{
				dHigherAxisMin=80;
			}
			else if(iTempMinY>=60)
			{
				dHigherAxisMin=60;
			}
			else if(iTempMinY>=40)
			{
				dHigherAxisMin=40;
			}
			else if(iTempMinY>=20)
			{
				dHigherAxisMin=20;
			}
			else if(iTempMinY>=10)
			{
				dHigherAxisMin=10;
			}
			else if(iTempMinY>=0)
			{
				dHigherAxisMin=0;
			}
			else if(iTempMinY>=(-10))
			{
				dHigherAxisMin=(-10);
			}
			else if(iTempMinY>=(-20))
			{
				dHigherAxisMin=(-20);
			}
			else if(iTempMinY>=(-40))
			{
				dHigherAxisMin=(-40);
			}
			else if(iTempMinY>=(-60))
			{
				dHigherAxisMin=(-60);
			}
			else if(iTempMinY>=(-80))
			{
				dHigherAxisMin=(-80);
			}
			else if(iTempMinY>=(-100))
			{
				dHigherAxisMin=(-100);
			}
			else if(iTempMinY>=(-200))
			{
				dHigherAxisMin=(-200);
			}
			else if(iTempMinY>=(-300))
			{
				dHigherAxisMin=(-300);
			}
			else if(iTempMinY>=(-400))
			{
				dHigherAxisMin=(-400);
			}
			else
			{
				dHigherAxisMin=(-500);
			}
		}
		break;
	}

	if(m_iDiagrammType==FOT_LOOP)
	{
		if(dHigherAxisMin>m_pFunctionParams->yMax || dHigherAxisMin<m_pFunctionParams->yMin)
			dHigherAxisMin=m_pFunctionParams->yMin;
	}
	else
	{
		if(dHigherAxisMin>m_pFunctionParams->yMax || dHigherAxisMin<m_pFunctionParams->yMin)
			dHigherAxisMin=m_pFunctionParams->yMin;
	}


	return dHigherAxisMin;

}
// **************************************************************************
// 
// **************************************************************************
//bool CDiagramm::CanDecreaseXScale()
//{
//	bool bRes=false;
//
//	if(!m_pFunctionParams)
//	{
//		return false;
//	}
//	double dLowerAxisMax=0;
//	double dHigherAxisMin=0;
//
//	switch(m_iDiagrammType)
//	{
//	case PRESSURE_VOLUME_LOOP:
//	case VOLUME_FLOW_LOOP:
//	case VOLUME_FLOW_HF_LOOP:
//	//case PRESSURE_HF_GRAPH:
//		{
//			if(m_dXAxisMax>200)
//			{
//				dLowerAxisMax=m_dXAxisMax-200;
//			}
//			else if(m_dXAxisMax>150)
//			{
//				dLowerAxisMax=m_dXAxisMax-150;
//			}
//			else if(m_dXAxisMax>100)
//			{
//				dLowerAxisMax=m_dXAxisMax-100;
//			}
//			else if(m_dXAxisMax>40)
//			{
//				dLowerAxisMax=m_dXAxisMax-20;
//			}
//			else if(m_dXAxisMax>20)
//			{
//				dLowerAxisMax=m_dXAxisMax-10;
//			}
//			else if(m_dXAxisMax>10)
//			{
//				dLowerAxisMax=10;
//			}
//			else
//			{
//				dLowerAxisMax=5;
//			}
//
//			if(dLowerAxisMax<m_pFunctionParams->xMin || dLowerAxisMax==0)
//				dLowerAxisMax=m_pFunctionParams->xMin;
//
//			if(		m_dMaxX<dLowerAxisMax
//				&&	m_dXAxisMax>dLowerAxisMax)
//			{
//				bRes=true;
//			}
//		}
//		break;
//	case PRESSURE_VOLUME_HF_LOOP:
//		{
//			if(m_dXAxisMax>200)
//			{
//				dLowerAxisMax=m_dXAxisMax-200;
//			}
//			else if(m_dXAxisMax>150)
//			{
//				dLowerAxisMax=m_dXAxisMax-150;
//			}
//			else if(m_dXAxisMax>100)
//			{
//				dLowerAxisMax=m_dXAxisMax-100;
//			}
//			else if(m_dXAxisMax>40)
//			{
//				dLowerAxisMax=m_dXAxisMax-20;
//			}
//			else if(m_dXAxisMax>20)
//			{
//				dLowerAxisMax=m_dXAxisMax-10;
//			}
//			else if(m_dXAxisMax>10)
//			{
//				dLowerAxisMax=10;
//			}
//			else
//			{
//				dLowerAxisMax=5;
//			}
//
//			if(dLowerAxisMax<m_pFunctionParams->xMin || dLowerAxisMax==0)
//				dLowerAxisMax=m_pFunctionParams->xMin;
//
//			if(		m_dMaxX<dLowerAxisMax
//				&&	m_dXAxisMax>dLowerAxisMax)
//			{
//				bRes=true;
//			}
//
//
//			dHigherAxisMin=m_dXAxisMin+10;
//
//			if(dHigherAxisMin<m_pFunctionParams->xMin || dHigherAxisMin>=0)
//				dHigherAxisMin=GetNegativeYoffset(m_dXAxisMax);
//
//			if(		m_dMinX>dHigherAxisMin
//				&&	m_dXAxisMax>dHigherAxisMin)
//			{
//				bRes=true;
//			}
//		}
//		break;
//	default:
//		{
//		}
//		break;
//	}
//	return bRes;
//}

bool CDiagramm::CanDecreaseYScale()
{
	bool bRes=false;

	if(!m_pFunctionParams)
	{
		return false;
	}
	double dLowerAxisMax=0;
	double dHigherAxisMin=0;

	int iTempMaxY=(int)(m_dMaxY+((m_dMaxY/100)*10));
	int iTempMinY=0;

	if(m_dMinY<0)
	{
		iTempMinY=(int)(m_dMinY+((m_dMinY/100)*10));
	}
	else
	{
		iTempMinY=(int)(m_dMinY-((m_dMinY/100)*10));
	}


	switch(m_iDiagrammType)
	{
	case VOLUME_FLOW_LOOP:
	case VOLUME_FLOW_HF_LOOP:
	case FLOW_GRAPH:
	case FLOW_HF_GRAPH:
		{
			if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-10;
			}
			else if(m_dYAxisMax>10)
			{
				dLowerAxisMax=m_dYAxisMax-5;
			}
			else if(m_dYAxisMax>5)
			{
				dLowerAxisMax=5;
			}
			else
			{
				dLowerAxisMax=2;
			}

			if(m_dYAxisMin<-20)
			{
				dHigherAxisMin=m_dYAxisMin+10;
			}
			else if(m_dYAxisMin<-10)
			{
				dHigherAxisMin=m_dYAxisMin+5;
			}
			else if(m_dYAxisMin<-5)//FLOWSCALE
			{
				dHigherAxisMin=-5;
			}
			else
			{
				dHigherAxisMin=-2;
			}

			if(dLowerAxisMax<m_pFunctionParams->yMin || dLowerAxisMax==0)
				dLowerAxisMax=m_pFunctionParams->yMin;
			if(dHigherAxisMin>m_pFunctionParams->yMax)
				dHigherAxisMin=m_pFunctionParams->yMax;

			if(		iTempMaxY<dLowerAxisMax
				&&	m_dYAxisMax>dLowerAxisMax
				&&	iTempMinY>dHigherAxisMin
				&&	m_dYAxisMin<dHigherAxisMin)
			{
				bRes=true;
			}

		}
		break;
	case PRESSURE_GRAPH:
	case PRESSURE_HF_GRAPH:
	case VOLUME_GRAPH:
	case VOLUME_HF_GRAPH:
	case PRESSURE_VOLUME_LOOP:
	case PRESSURE_VOLUME_HF_LOOP:
		{
			if(m_dYAxisMax>200)
			{
				dLowerAxisMax=m_dYAxisMax-200;
			}
			else if(m_dYAxisMax>150)
			{
				dLowerAxisMax=m_dYAxisMax-150;
			}
			else if(m_dYAxisMax>100)
			{
				dLowerAxisMax=m_dYAxisMax-100;
			}
			else if(m_dYAxisMax>40)
			{
				dLowerAxisMax=m_dYAxisMax-20;
			}
			else if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-10;
			}
			else if(m_dYAxisMax>10)
			{
				dLowerAxisMax=10;
			}
			else if(m_dYAxisMax>5)//newVG
			{
				dLowerAxisMax=5;
			}
			else if(m_dYAxisMax>2)//newVG
			{
				dLowerAxisMax=2;
			}
			else
			{
				dLowerAxisMax=2;
			}

			if(		iTempMaxY<dLowerAxisMax
				&&	m_dYAxisMax>dLowerAxisMax)
			{
				bRes=true;
			}
		}
		break;
	case CO2_GRAPH:
	case CO2_HF_GRAPH:
		{
			if(m_dYAxisMax>200)
			{
				dLowerAxisMax=m_dYAxisMax-200;
			}
			else if(m_dYAxisMax>150)
			{
				dLowerAxisMax=m_dYAxisMax-150;
			}
			else if(m_dYAxisMax>100)
			{
				dLowerAxisMax=m_dYAxisMax-100;
			}
			else if(m_dYAxisMax>40)
			{
				dLowerAxisMax=m_dYAxisMax-20;
			}
			else if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-10;
			}
			else if(m_dYAxisMax>10)
			{
				dLowerAxisMax=10;
			}

			if(		iTempMaxY<dLowerAxisMax
				&&	m_dYAxisMax>dLowerAxisMax)
			{
				bRes=true;
			}
		}
		break;
	case FOT_LOOP://:CanDecreaseYScale()
		{
			//double dLowerAxisMax=0;
			//double dHigherAxisMin=0;

			//m_dMaxY
			if(m_dYAxisMax>=100)
			{
				dLowerAxisMax=80;
			}
			else if(m_dYAxisMax>=80)
			{
				dLowerAxisMax=60;
			}
			else if(m_dYAxisMax>=60)
			{
				dLowerAxisMax=40;
			}
			else if(m_dYAxisMax>=40)
			{
				dLowerAxisMax=20;
			}
			else if(m_dYAxisMax>=20)
			{
				dLowerAxisMax=10;
			}
			else if(m_dYAxisMax>=10)
			{
				dLowerAxisMax=0;
			}
			else if(m_dYAxisMax>=0)
			{
				dLowerAxisMax=(-10);
			}
			else if(m_dYAxisMax>=(-10))
			{
				dLowerAxisMax=(-20);
			}
			else if(m_dYAxisMax>=(-20))
			{
				dLowerAxisMax=(-40);
			}
			else if(m_dYAxisMax>=(-40))
			{
				dLowerAxisMax=(-60);
			}
			else if(m_dYAxisMax>=(-60))
			{
				dLowerAxisMax=(-80);
			}
			else if(m_dYAxisMax>=(-80))
			{
				dLowerAxisMax=(-100);
			}
			else if(m_dYAxisMax>=(-100))
			{
				dLowerAxisMax=(-200);
			}
			else if(m_dYAxisMax>=(-200))
			{
				dLowerAxisMax=(-300);
			}
			else //if(m_dYAxisMax>=(-300))
			{
				dLowerAxisMax=(-400);
			}
			
			
			if(m_dYAxisMin<=(-500))
			{
				dHigherAxisMin=(-400);
			}
			else if(m_dYAxisMin<=(-400))
			{
				dHigherAxisMin=(-300);
			}
			else if(m_dYAxisMin<=(-300))
			{
				dHigherAxisMin=(-200);
			}
			else if(m_dYAxisMin<=(-200))
			{
				dHigherAxisMin=(-100);
			}
			else if(m_dYAxisMin<=(-100))
			{
				dHigherAxisMin=(-80);
			}
			else if(m_dYAxisMin<=(-80))
			{
				dHigherAxisMin=(-60);
			}
			else if(m_dYAxisMin<=(-60))
			{
				dHigherAxisMin=(-40);
			}
			else if(m_dYAxisMin<=(-40))
			{
				dHigherAxisMin=(-20);
			}
			else if(m_dYAxisMin<=(-20))
			{
				dHigherAxisMin=(-10);
			}
			else if(m_dYAxisMin<=(-10))
			{
				dHigherAxisMin=0;
			}
			else if(m_dYAxisMin<=0)
			{
				dHigherAxisMin=10;
			}
			else if(m_dYAxisMin<=10)
			{
				dHigherAxisMin=20;
			}
			else if(m_dYAxisMin<=20)
			{
				dHigherAxisMin=40;
			}
			else if(m_dYAxisMin<=40)
			{
				dHigherAxisMin=60;
			}
			else if(m_dYAxisMin<=60)
			{
				dHigherAxisMin=80;
			}
			

			iTempMaxY=m_dMaxY+10;
			iTempMinY=m_dMinY-10;

			
			if(m_dMaxY<=0 && m_dMinY<0)
			{
				if(		iTempMaxY<dLowerAxisMax
					&&	m_dYAxisMin<dLowerAxisMax
					&&	m_dYAxisMax!=0)
				{
					bRes=true;
				}
				else if(	iTempMinY>dHigherAxisMin
						&&	m_dYAxisMax>dHigherAxisMin)
				{
					bRes=true;
				}
			}
			else if(m_dMaxY>0 && m_dMinY>=0)
			{
				if(		iTempMaxY<dLowerAxisMax
					&&	m_dYAxisMin<dLowerAxisMax)
				{
					bRes=true;
				}
				else if(	iTempMinY>dHigherAxisMin
					&&	m_dYAxisMax>dHigherAxisMin
					&&	m_dYAxisMin!=0)
				{
					bRes=true;
				}
			}
			else
			{
				if(		iTempMaxY<dLowerAxisMax
					&&	m_dYAxisMin<dLowerAxisMax)
				{
					bRes=true;
				}
				else if(	iTempMinY>dHigherAxisMin
						&&	m_dYAxisMax<dHigherAxisMin)
				{
					bRes=true;
				}
			}
			
		}
		break;
	case SPO2_GRAPH:
		{
			if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-10;
			}
			else if(m_dYAxisMax>10)
			{
				dLowerAxisMax=m_dYAxisMax-5;
			}
			else if(m_dYAxisMax>5)
			{
				dLowerAxisMax=5;
			}
			else
			{
				dLowerAxisMax=2;
			}

			if(m_dYAxisMin<-20)
			{
				dHigherAxisMin=m_dYAxisMin+10;
			}
			else if(m_dYAxisMin<-10)
			{
				dHigherAxisMin=m_dYAxisMin+5;
			}
			else
			{
				dHigherAxisMin=-5;
			}

			if(dLowerAxisMax<m_pFunctionParams->yMin || dLowerAxisMax==0)
				dLowerAxisMax=m_pFunctionParams->yMin;
			if(dHigherAxisMin>m_pFunctionParams->yMax)
				dHigherAxisMin=m_pFunctionParams->yMax;

			if(		iTempMaxY<dLowerAxisMax
				&&	m_dYAxisMax>dLowerAxisMax
				&&	iTempMinY>dHigherAxisMin
				&&	m_dYAxisMin<dHigherAxisMin)
			{
				bRes=true;
			}
		}
		break;
	}
	return bRes;
}

// **************************************************************************
// 
// **************************************************************************
//bool CDiagramm::CanDecreaseYScaleHF(int dMaxY, int dMinY)
//{
//	bool bRes=false;
//
//	if(!m_pFunctionParams)
//	{
//		return false;
//	}
//
//	double dLowerAxisMax=0;
//	double dHigherAxisMin=0;
//
//	int iTempMaxY=dMaxY+((dMaxY/100)*10);
//
//
//	switch(m_iDiagrammType)
//	{
//	case PRESSURE_HF_GRAPH:
//		{
//			if(m_dYAxisMax>200)
//			{
//				dLowerAxisMax=m_dYAxisMax-200;
//			}
//			else if(m_dYAxisMax>150)
//			{
//				dLowerAxisMax=m_dYAxisMax-150;
//			}
//			else if(m_dYAxisMax>100)
//			{
//				dLowerAxisMax=m_dYAxisMax-100;
//			}
//			else if(m_dYAxisMax>40)
//			{
//				dLowerAxisMax=m_dYAxisMax-20;
//			}
//			else if(m_dYAxisMax>20)
//			{
//				dLowerAxisMax=m_dYAxisMax-10;
//			}
//			else if(m_dYAxisMax>10)
//			{
//				dLowerAxisMax=10;
//			}
//			else
//			{
//				dLowerAxisMax=5;
//			}
//
//			if(		iTempMaxY<dLowerAxisMax
//				&&	m_dYAxisMax>dLowerAxisMax)
//			{
//				bRes=true;
//			}
//		}
//		break;
//	}
//	return bRes;
//}







// **************************************************************************
// Each Function e.g. DoSineX,DoPlotXY, etc, calls this function as
//				they calculate each point so that each point can be drawn on the
//				Takes:												
//				UINT x, UNIT y - the graph co-ord of the point just calculeted
//				(current point).										
//				UINT prevx, UINT prevy - the co-ords of the previous point		
//				This routine checks what type of plot (line, dot, or bar) is
//				required and calls the appropriate routine
// **************************************************************************
//void CDiagramm::PlotPoints(UINT x, UINT y, UINT prevx, UINT prevy)
//{
//	//here we check the chart type and plot the points accordingly
//	//we need to constarin the Y values to keep them within the
//	//plot area;
//
//	switch(m_pFunctionParams->ChartType)
//	{
//	case G_LINECHART:
//		{
//			DrawConnectLine(prevx,prevy,x,y);
//			break;
//		}
//	default:
//		{
//			break;
//		}
//	}//SWITCH
//
//	return;
//}

// **************************************************************************
// Each Function e.g. DoSineX,DoPlotXY, etc, calls this function as
//				they calculate each point so that each point can be drawn on the
//				Takes:												
//				UINT x, UNIT y - the graph co-ord of the point just calculeted
//				(current point).										
//				UINT prevx, UINT prevy - the co-ords of the previous point		
//				This routine checks what type of plot (line, dot, or bar) is
//				required and calls the appropriate routine
// **************************************************************************
void CDiagramm::PlotPointArray(CPoint *pts, int iSize, bool copyPlot)
{
	if(m_bExit)
		return;

	if(!m_pFunctionParams)
	{
		return;
	}
	//here we check the chart type and plot the points accordingly
	//we need to constarin the Y values to keep them within the
	//plot area;

	switch(m_pFunctionParams->ChartType)
	{
	case G_LINECHART:
		{
			DrawConnectLine(pts,iSize);
			break;
		}
	case G_LOOPCHART:
		{
			DrawConnectLoop(pts,iSize,copyPlot);
			break;
		}
	case G_FOTLOOP:
		{
			DrawFOTLoop(pts,iSize,copyPlot);
			break;
		}
	case G_FILLEDCHART:
		{
			DrawFilledLine(pts,iSize);
			break;
		}
	default:
		{
			break;
		}
	}//SWITCH

	return;
}


//void CDiagramm::PlotLongPointArray(CPoint *pts, int iSize, bool copyPlot)
//{
//	if(m_bExit)
//		return;
//	//here we check the chart type and plot the points accordingly
//	//we need to constarin the Y values to keep them within the
//	//plot area;
//
//	switch(m_pFunctionParams->ChartType)
//	{
//	case G_LINECHART_REFRESH:
//	case G_LINECHART:
//		{
//			DrawLongConnectLine(pts,iSize);
//			break;
//		}
//	case G_LOOPCHART:
//		{
//			DrawConnectLoop(pts,iSize,copyPlot);
//			break;
//		}
//	case G_FILLEDCHART_REFRESH:
//	case G_FILLEDCHART:
//		{
//			DrawLongFilledLine(pts,iSize);
//			break;
//		}
//	default:
//		{
//			break;
//		}
//	}//SWITCH
//
//	return;
//}

// **************************************************************************
// For the line chat type, this routine draws a line between previous	
//				point (FROM) and current point (TO)
// **************************************************************************
void CDiagramm::DrawConnectLine(CPoint *pts, int iSize)
{
	if(m_bExit)
		return;

	if(!m_pFunctionParams)
	{
		return;
	}
	int iXleft=pts[0].x;
	int iXright=pts[iSize-1].x;
	int iWidth=iXright-iXleft;

	CClientDC dc(this);

	if(iWidth==0)
	{
		BitBlt(m_hdcGraph, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft, 0, SRCCOPY);
		BitBlt(dc.m_hDC, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcGraph,  iXleft, 0, SRCCOPY);
		return;
	}

	CDC* pDCGraph=CDC::FromHandle(m_hdcGraph);

	CPen pen,cursorpen,penTrig,penLimit,penGarant;
	CPen *poldpenGraph=NULL;
	CPen *poldpenDC=NULL;
	pen.CreatePen(PS_SOLID,m_iGraphPenSize,m_crCurrentGraphPenColor);
	penTrig.CreatePen(PS_SOLID,m_iGraphPenSize,RGB(0,160,0));
	penLimit.CreatePen(PS_SOLID,1,RGB(255,90,0));
	penGarant.CreatePen(PS_SOLID,1,RGB(255,200,0));
	cursorpen.CreatePen(PS_SOLID,4,RGB(250,250,250));
	
	poldpenGraph=pDCGraph->SelectObject(&pen);
	HBRUSH holdbrush=(HBRUSH)pDCGraph->SelectObject((HBRUSH)GetStockObject(NULL_BRUSH));

	if(iXleft==25)
		BitBlt(m_hdcGraph, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft, 0, SRCCOPY);
	else
		BitBlt(m_hdcGraph, iXleft+1, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft+1, 0, SRCCOPY);

	pDCGraph->SelectObject(&pen);
	pDCGraph->Polyline(pts, iSize);
	pDCGraph->SelectObject(&penLimit);

	////Draw the Voloume Limit
	if(m_bDrawVolumeLimit && m_iVolumeLimit>0)
	{
		pDCGraph->MoveTo(iXleft,m_iVolumeLimit);
		pDCGraph->LineTo(iXright,m_iVolumeLimit);
	}

	////Draw the Voloume Garanty
	if(m_bDrawVolumeGaranty && m_iVolumeGaranty>0)
	{
		pDCGraph->SelectObject(&penGarant);
		pDCGraph->MoveTo(iXleft,m_iVolumeGaranty);
		pDCGraph->LineTo(iXright,m_iVolumeGaranty);
	}
	if(m_bDrawHFOVolumeGarantyHigh)
	{
		pDCGraph->SelectObject(&penGarant);
		pDCGraph->MoveTo(iXleft,m_iHFOVolumeGarantyHigh);
		pDCGraph->LineTo(iXright,m_iHFOVolumeGarantyHigh);
	}
	if(m_bDrawHFOVolumeGarantyLow)
	{
		pDCGraph->SelectObject(&penGarant);
		pDCGraph->MoveTo(iXleft,m_iHFOVolumeGarantyLow);
		pDCGraph->LineTo(iXright,m_iHFOVolumeGarantyLow);
	}


	////Draw the TriggerVolume
	if(m_iTrigThreshold>0)
	{
		pDCGraph->SelectObject(&penTrig);
		pDCGraph->MoveTo(iXleft,m_iTrigThreshold);
		pDCGraph->LineTo(iXright,m_iTrigThreshold);
	}

	BitBlt(dc.m_hDC, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcGraph,  iXleft, 0, SRCCOPY);

	if(m_bDrawCursor &&
		iXleft>=DIAGRAMM_WIDTH-31)
	{
		DrawCursor(m_bDrawCursor);
	}

	poldpenDC=dc.SelectObject(&cursorpen);
	if(iXleft+iWidth+3<(m_iGraphX+m_iGraphWidth-m_rmargin))
	{
		MoveToEx(dc.m_hDC, iXleft+iWidth+3, m_iGraphHeight-1, NULL);
		LineTo(dc.m_hDC, iXleft+iWidth+3, 1);
	}

	pDCGraph->SelectObject(poldpenGraph);
	pDCGraph->SelectObject(holdbrush);
	dc.SelectObject(poldpenDC);

	pen.DeleteObject();
	cursorpen.DeleteObject();
	penTrig.DeleteObject();
	penLimit.DeleteObject();
	penGarant.DeleteObject();
}



// **************************************************************************
// For the line chat type, this routine draws a line between previous	
//				point (FROM) and current point (TO)
// **************************************************************************
void CDiagramm::DrawConnectLoop(CPoint *pts, int iSize, bool levelPlot)
{
	if(m_bExit)
		return;

	CPen pen,penLevel1,penLevel2;
	CPen *poldpenDC=NULL;
	CPen *poldpen1=NULL;
	CPen  *poldpen2=NULL;
	pen.CreatePen(PS_SOLID,m_iGraphPenSize,m_crCurrentGraphPenColor);
	penLevel1.CreatePen(PS_SOLID,m_iGraphPenSize,m_crPrevLoop2);
	penLevel2.CreatePen(PS_SOLID,m_iGraphPenSize,m_crPrevLoop3);

	CBrush brush;
	CBrush *poldbrushDC=NULL;
	CBrush *poldbrush1=NULL;
	CBrush *poldbrush2=NULL;
	brush.CreateSolidBrush(m_crCurrentGraphPenColor);

	CClientDC dc(this);
	poldpenDC=dc.SelectObject(&pen);
	poldbrushDC=dc.SelectObject(&brush);

	dc.Polyline(pts, iSize);

	

	/*if(getModel()->getVMODEHANDLER()->activeModeIsHFO()==false)
	{
		CDC* pDCLevel1=CDC::FromHandle(m_hdcLevel1);
		poldpen1=pDCLevel1->SelectObject(&penLevel1);
		poldbrush1=pDCLevel1->SelectObject(&brush);

		CDC* pDCLevel2=CDC::FromHandle(m_hdcLevel2);
		poldpen2=pDCLevel2->SelectObject(&penLevel2);
		poldbrush2=pDCLevel2->SelectObject(&brush);

		if(levelPlot)
		{
			pDCLevel1->Polyline(pts, iSize);
			pDCLevel2->Polyline(pts, iSize);
		}

		pDCLevel1->SelectObject(poldpen1);
		pDCLevel1->SelectObject(poldbrush1);
		pDCLevel2->SelectObject(poldpen2);
		pDCLevel2->SelectObject(poldbrush2);
	}
	else
	{
		CDC* pDCLevel1=CDC::FromHandle(m_hdcLevel1);
		poldpen1=pDCLevel1->SelectObject(&penLevel1);
		poldbrush1=pDCLevel1->SelectObject(&brush);

		if(levelPlot)
		{
			pDCLevel1->Polyline(pts, iSize);
		}

		pDCLevel1->SelectObject(poldpen1);
		pDCLevel1->SelectObject(poldbrush1);
	}*/

	dc.SelectObject(poldpenDC);
	dc.SelectObject(poldbrushDC);

	pen.DeleteObject();
	penLevel1.DeleteObject();
	penLevel2.DeleteObject();
	//brush.DeleteObject();//rkuNEWFIX
}

// **************************************************************************
// For the line chat type, this routine draws a line between previous	
//				point (FROM) and current point (TO)
// **************************************************************************
void CDiagramm::DrawFOTLoop(CPoint *pts, int iSize, bool levelPlot)
{
	if(m_bExit)
		return;

	CPen pen, penRect;
	CPen *poldpenDC=NULL;
	CPen *poldpen1=NULL;
	CPen  *poldpen2=NULL;
	pen.CreatePen(PS_SOLID,m_iGraphPenSize,m_crCurrentGraphPenColor);
	penRect.CreatePen(PS_SOLID,1,m_crCurrentGraphPenColor);

	CBrush brush;
	CBrush *poldbrushDC=NULL;
	brush.CreateSolidBrush(m_crGraphBkColor);

	CClientDC dc(this);
	poldpenDC=dc.SelectObject(&penRect);
	poldbrushDC=dc.SelectObject(&brush);


	
	for(int i=0; i<iSize;i++)
	{
		Rectangle(dc.m_hDC, pts[i].x-5, pts[i].y+5, pts[i].x+5,pts[i].y-5);
	}

	dc.SelectObject(&pen);
	dc.Polyline(pts, iSize);


	dc.SelectObject(poldpenDC);
	dc.SelectObject(poldbrushDC);

	pen.DeleteObject();
	penRect.DeleteObject();
	//brush.DeleteObject();//rkuNEWFIX
}

// **************************************************************************
// For the line chat type, this routine draws a line between previous	
//				point (FROM) and current point (TO)
// **************************************************************************
void CDiagramm::DrawFilledLine(CPoint *pts, int iSize)
{
	if(m_bExit)
		return;

	int iXleft=pts[0].x;
	int iXright=pts[iSize-1].x;
	int iWidth=iXright-iXleft;

	CClientDC dc(this);
	
	if(iWidth==0)
	{
		BitBlt(m_hdcGraph, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft, 0, SRCCOPY);
		BitBlt(dc.m_hDC, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcGraph,  iXleft, 0, SRCCOPY);
		return;
	}

	CDC* pDCGraph=CDC::FromHandle(m_hdcGraph);

	CPen *poldpenDC=NULL;
	CPen pen,cursorpen,penTrig,penLimit,penGarant;
	pen.CreatePen(PS_SOLID,m_iGraphPenSize,m_crCurrentGraphPenColor);
	cursorpen.CreatePen(PS_SOLID,4,RGB(250,250,250));
	penTrig.CreatePen(PS_SOLID,1,RGB(0,160,0));
	penLimit.CreatePen(PS_SOLID,1,RGB(255,90,0));
	penGarant.CreatePen(PS_SOLID,1,RGB(255,200,0));

	CBrush brush;
	CBrush *poldbrush=NULL;
	brush.CreateSolidBrush(m_crCurrentGraphPenColor);
	poldbrush=pDCGraph->SelectObject(&brush);
	CPen *poldpenGraph=pDCGraph->SelectObject(&pen);

	BitBlt(m_hdcGraph, iXleft+1, 0, iWidth+2,m_iGraphHeight, m_hdcStatic,  iXleft+1, 0, SRCCOPY);


	pDCGraph->SelectObject(&pen);
	pDCGraph->Polygon(pts, iSize);
	
	pDCGraph->SelectObject(&penLimit);

	////Draw the Voloume Limit
	if(m_bDrawVolumeLimit && m_iVolumeLimit>0)
	{
		pDCGraph->MoveTo(iXleft,m_iVolumeLimit);
		pDCGraph->LineTo(iXright,m_iVolumeLimit);
	}

	////Draw the Voloume Garanty
	if(m_bDrawVolumeGaranty && m_iVolumeGaranty>0)
	{
		pDCGraph->SelectObject(&penGarant);
		pDCGraph->MoveTo(iXleft,m_iVolumeGaranty);
		pDCGraph->LineTo(iXright,m_iVolumeGaranty);
	}
	if(m_bDrawHFOVolumeGarantyHigh)
	{
		pDCGraph->SelectObject(&penGarant);
		pDCGraph->MoveTo(iXleft,m_iHFOVolumeGarantyHigh);
		pDCGraph->LineTo(iXright,m_iHFOVolumeGarantyHigh);
	}
	if(m_bDrawHFOVolumeGarantyLow)
	{
		pDCGraph->SelectObject(&penGarant);
		pDCGraph->MoveTo(iXleft,m_iHFOVolumeGarantyLow);
		pDCGraph->LineTo(iXright,m_iHFOVolumeGarantyLow);
	}

	////Draw the TriggerVolume
	if(m_iTrigThreshold>0)
	{
		pDCGraph->SelectObject(&penTrig);
		pDCGraph->MoveTo(iXleft,m_iTrigThreshold);
		pDCGraph->LineTo(iXright,m_iTrigThreshold);
	}

	

	BitBlt(dc.m_hDC, iXleft, 0, iWidth+2,m_iGraphHeight, m_hdcGraph,  iXleft, 0, SRCCOPY);

	if(m_bDrawCursor &&
		iXleft>=DIAGRAMM_WIDTH-31)
	{
		DrawCursor(m_bDrawCursor);
	}

	poldpenDC=dc.SelectObject(&cursorpen);

	if(iXleft+iWidth+3<(m_iGraphX+m_iGraphWidth-m_rmargin))
	{
		MoveToEx(dc.m_hDC, iXleft+iWidth+3, m_iGraphHeight-1, NULL);
		LineTo(dc.m_hDC, iXleft+iWidth+3, 1);
	}

	pDCGraph->SelectObject(poldpenGraph);
	pDCGraph->SelectObject(poldbrush);
	dc.SelectObject(poldpenDC);

	
	pen.DeleteObject();
	cursorpen.DeleteObject();
	penTrig.DeleteObject();
	penLimit.DeleteObject();
	penGarant.DeleteObject();
	//brush.DeleteObject();//rkuNEWFIX
}

// **************************************************************************
// This function makes sure that a value of Y plot valuestays within
//				stays within the plot area.						
//				Input:												
//				double Y value									
//				Returns:											
//				double y constrained to the plotting area
// **************************************************************************
double CDiagramm::ConstrainY(double y)
{
	if ( (y < m_dYAxisMax) && (y > m_dYAxisMin))
	{
		return y;
	}
	else if (y <= m_dYAxisMin)
	{
		return m_dYAxisMin;
	}
	else if (y >=m_dYAxisMax)
	{
		return m_dYAxisMax;
	}
	return y; //should never get here???
}


// **************************************************************************
// 
// **************************************************************************
//void CDiagramm::SetAutofit(bool state)
//{
//	m_bAutofit=state;
//}

// **************************************************************************
// 
// **************************************************************************
void CDiagramm::IgnoreAutofit(bool state)
{
	if(m_bAutofit)
	{
		if(state==false 
			&& VENT_RUNNING==getModel()->GetVentRunState() 
			&& getModel()->getALARMHANDLER()->ALARM_DISCONNECTION->getAlarmState()!=AS_ACTIVE)
		{
			m_bIgnoreAutofit=state;
		}
		else if(state==true)
		{
			m_bIgnoreAutofit=state;
		}
	}
}


// **************************************************************************
// 
// **************************************************************************
void CDiagramm::SetTriggeredBreath(bool state)
{
	m_bTriggeredBreath=state;

	if(state)
	{
		m_crCurrentGraphPenColor=m_crTrigger;
	}
	else
	{
		m_crCurrentGraphPenColor=m_crStaticGraphPenColor;
	}
}

// **************************************************************************
// 
// **************************************************************************
//void CDiagramm::SetPreviousLoopBreath(bool state)
//{
//	m_bPreviousLoopBreath=state;
//
//	if(state)
//	{
//		m_crCurrentGraphPenColor=RGB(140,140,140);
//	}
//	else
//	{
//		m_crCurrentGraphPenColor=m_crStaticGraphPenColor;
//	}
//}

// **************************************************************************
// 
// **************************************************************************
void CDiagramm::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
}

// **************************************************************************
// 
// **************************************************************************
void CDiagramm::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	if(getModel()->getVIEWHANDLER()->getViewState()==VS_GRAPH)
	{
		m_bSelected = false;
	}
	else if(getModel()->getVIEWHANDLER()->getViewState()==VS_PARA)
	{
		m_bSelected = false;
	}
}



// **************************************************************************
// 
// **************************************************************************
BOOL CDiagramm::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			return 1;
		}
		break;
	//case WM_MOUSEMOVE:
	//	{
	//		switch(pMsg->wParam)
	//		{
	//		case MK_LBUTTON:
	//			{
	//				int xPos = GET_X_LPARAM(pMsg->lParam); 
	//				int yPos = GET_Y_LPARAM(pMsg->lParam);

	//				//if(xPos>m_iXStartPos+10)
	//				//{
	//				//	//TRACE(_T("WM_MOUSEMOVE right xStartPos: %d yStartPos: %d xPos: %d yPos: %d\r"),m_iXStartPos,m_iYStartPos,xPos,yPos);
	//				//	m_iXStartPos = xPos;
	//				//	m_iYStartPos = yPos;

	//				//	/*if(m_iDiagrammType==GRAPH_PRESSURE_HF)
	//				//	{
	//				//		CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_CURSOR_RIGHT);
	//				//		getModel()->Trigger(&event);

	//				//		m_bTouchmove=true;
	//				//	}*/
	//				//}
	//				//else if(xPos<m_iXStartPos-10)
	//				//{
	//				//	//TRACE(_T("WM_MOUSEMOVE left xStartPos: %d yStartPos: %d xPos: %d yPos: %d\r"),m_iXStartPos,m_iYStartPos,xPos,yPos);
	//				//	m_iXStartPos = xPos;
	//				//	m_iYStartPos = yPos;

	//				//	/*if(m_iDiagrammType==GRAPH_PRESSURE_HF)
	//				//	{
	//				//		CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_CURSOR_LEFT);
	//				//		getModel()->Trigger(&event);

	//				//		m_bTouchmove=true;
	//				//	}*/
	//				//}
	//				//else 


	//				//if(yPos>m_iYStartPos+5)
	//				//{
	//				//	int iDiff=yPos-m_iYStartPos;
	//				//	//TRACE(_T("WM_MOUSEMOVE down yStartPos: %d yPos: %d\r"),m_iYStartPos,yPos);
	//				//	//m_iXStartPos = xPos;
	//				//	m_iYStartPos = yPos;

	//				//	int scale=GetYPixelsOfDiagramm()/(m_dYAxisMax-m_dYAxisMin);
	//				//	if(scale==0)
	//				//		scale=1;

	//				//	int iMove=iDiff/scale;
	//				//	
	//				//	if(!m_bAutofit && m_iDiagrammType==GRAPH_PRESSURE_HF)
	//				//	{
	//				//		if((m_dYAxisMin+iMove<0) && iMove!=0)
	//				//		{
	//				//			SetYAxisScale(m_dYAxisMin+iMove, m_dYAxisMax+iMove, true);
	//				//			getModel()->getCONFIG()->GraphSetPressureMaxHF(m_dYAxisMax);
	//				//			getModel()->getCONFIG()->GraphSetPressureMinHF(m_dYAxisMin);

	//				//			m_bTouchmove=true;
	//				//		}
	//				//		else
	//				//		{
	//				//			//limit message

	//				//			m_bTouchmove=true;
	//				//		}
	//				//		
	//				//	}
	//			
	//				//}
	//				//else if(yPos<m_iYStartPos-5)
	//				//{
	//				//	int iDiff=m_iYStartPos-yPos;
	//				//	//TRACE(_T("WM_MOUSEMOVE up yStartPos: %d yPos: %d\r"),m_iYStartPos,yPos);
	//				//	//m_iXStartPos = xPos;
	//				//	m_iYStartPos = yPos;

	//				//	int scale=GetYPixelsOfDiagramm()/(m_dYAxisMax-m_dYAxisMin);
	//				//	if(scale==0)
	//				//		scale=1;
	//				//	int iMove=iDiff/scale;

	//				//	if(!m_bAutofit && m_iDiagrammType==GRAPH_PRESSURE_HF && (m_dYAxisMax+iMove>0))
	//				//	{
	//				//		
	//				//		SetYAxisScale(m_dYAxisMin-iMove, m_dYAxisMax-iMove, true);
	//				//		getModel()->getCONFIG()->GraphSetPressureMaxHF(m_dYAxisMax);
	//				//		getModel()->getCONFIG()->GraphSetPressureMinHF(m_dYAxisMin);

	//				//		m_bTouchmove=true;
	//				//	}

	//				//}
	//				/*else
	//					TRACE(_T("WM_MOUSEMOVE xStartPos: %d yStartPos: %d xPos: %d yPos: %d\r"),m_iXStartPos,m_iYStartPos,xPos,yPos);*/

	//			}
	//			break;
	//		default:
	//			{
	//				int iSt=0;
	//			}
	//			break;
	//		}
	//	}
	//	break;
	//case WM_LBUTTONUP:
	//	{
	//		//int xPos = GET_X_LPARAM(pMsg->lParam); 
	//		//int yPos = GET_Y_LPARAM(pMsg->lParam);

	//		//if((xPos>m_iXStartPos+30) && ((yPos<m_iYStartPos+30)&&(yPos>m_iYStartPos-30)))
	//		//{
	//		//	//TRACE(_T("WM_MOUSEMOVE right xStartPos: %d yStartPos: %d xPos: %d yPos: %d\r"),m_iXStartPos,m_iYStartPos,xPos,yPos);
	//		//	m_iXStartPos = xPos;
	//		//	m_iYStartPos = yPos;

	//		//	CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_CURSOR_RIGHT);
	//		//	getModel()->Trigger(&event);
	//		//}
	//		//else if((xPos<m_iXStartPos-30) && ((yPos<m_iYStartPos+30)&&(yPos>m_iYStartPos-30)))
	//		//{
	//		//	//TRACE(_T("WM_MOUSEMOVE left xStartPos: %d yStartPos: %d xPos: %d yPos: %d\r"),m_iXStartPos,m_iYStartPos,xPos,yPos);
	//		//	m_iXStartPos = xPos;
	//		//	m_iYStartPos = yPos;

	//		//	CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_CURSOR_LEFT);
	//		//	getModel()->Trigger(&event);
	//		//}
	//		//else if((yPos>m_iYStartPos+30) && ((xPos<m_iXStartPos+30)&&(xPos>m_iXStartPos-30)))
	//		//{
	//		//	//TRACE(_T("WM_MOUSEMOVE down xStartPos: %d yStartPos: %d xPos: %d yPos: %d\r"),m_iXStartPos,m_iYStartPos,xPos,yPos);
	//		//	m_iXStartPos = xPos;
	//		//	m_iYStartPos = yPos;

	//		//	CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_CURSOR_DOWN);
	//		//	getModel()->Trigger(&event);
	//		//}
	//		//else if((yPos<m_iYStartPos-30) && ((xPos<m_iXStartPos+30)&&(xPos>m_iXStartPos-30)))
	//		//{
	//		//	//TRACE(_T("WM_MOUSEMOVE up xStartPos: %d yStartPos: %d xPos: %d yPos: %d\r"),m_iXStartPos,m_iYStartPos,xPos,yPos);
	//		//	m_iXStartPos = xPos;
	//		//	m_iYStartPos = yPos;

	//		//	CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_CURSOR_UP);
	//		//	getModel()->Trigger(&event);
	//		//}
	//		/*else
	//			TRACE(_T("WM_MOUSEMOVE xStartPos: %d yStartPos: %d xPos: %d yPos: %d\r"),m_iXStartPos,m_iYStartPos,xPos,yPos);*/

	//		m_iXStartPos = 0;
	//		m_iYStartPos = 0;

	//		/*if(!m_bSelected && !m_bTouchmove)
	//		{
	//			if(getModel()->getVIEWHANDLER()->getViewState()==VS_GRAPH || getModel()->getVIEWHANDLER()->getViewState()==VS_PARA)
	//			{
	//				if(m_bAutofit==false)
	//				{
	//					m_bSelected = true;

	//					GetParent()->PostMessage( WM_GRAPH_SETFOCUS, GetDlgCtrlID(), 0);

	//					SetFocus();
	//				}
	//				else
	//					AfxGetApp()->GetMainWnd()->SetFocus();
	//			}
	//			else
	//				AfxGetApp()->GetMainWnd()->SetFocus();


	//		}
	//		else
	//		{
	//			if(m_bTouchmove)
	//				GetParent()->PostMessage(WM_REDRAW_DIAGRAMM,m_nID);

	//			GetParent()->SetFocus();
	//		}

	//		m_bTouchmove=false;*/

	//	}
	//	break;
	case WM_LBUTTONDOWN:
		{
			m_bTouchmove=false;

			if(!m_bSelected)
			{
				if(getModel()->getVIEWHANDLER()->getViewState()==VS_GRAPH || getModel()->getVIEWHANDLER()->getViewState()==VS_PARA)
				{
					if(m_bAutofit==false)
					{
						/*m_iXStartPos = GET_X_LPARAM(pMsg->lParam);
						m_iYStartPos = GET_Y_LPARAM(pMsg->lParam);*/
						

						m_bSelected = true;
						//int iTest=GetDlgCtrlID();

						if(GetParent())
							GetParent()->PostMessage( WM_GRAPH_SETFOCUS, GetDlgCtrlID(), 0);

						SetFocus();
					}
					else
					{
						if(AfxGetApp())
							AfxGetApp()->GetMainWnd()->SetFocus();
					}
				}
				else
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->SetFocus();
				}


			}
			else
			{
				if(GetParent())
					GetParent()->SetFocus();
			}
		}
		break;
	case WM_KEYDOWN:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				if(GetParent())
					GetParent()->PostMessage( WM_SELECT_NEXTGRAPH);
				
				return 1;
			}
			else if(pMsg->wParam==VK_UP)
			{
				if(GetParent())
					GetParent()->PostMessage(WM_GRAPH_INCREASE_SPEED, GetDlgCtrlID(), 0);
				return 1;
			}
			else if(pMsg->wParam==VK_DOWN)
			{
				if(GetParent())
					GetParent()->PostMessage(WM_GRAPH_DECREASE_SPEED, GetDlgCtrlID(), 0);
				return 1;
			}
			else
			{
				//handle focus change manual, draw next focused button and return true
				//return 1;
			}
		}
		break;
	default:
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}



void CDiagramm::SetFreeze(bool state) 
{
	m_bFREEZE=state;
}

void CDiagramm::SetHFOVolumeGaranty(bool drawState, double iValueHigh, double iValueLow)
{
	//m_bDrawHFOVolumeGaranty=drawState;

	if(drawState==false)
	{	
		m_iHFOVolumeGarantyHigh=0;
		m_iHFOVolumeGarantyLow=0;
		m_bDrawHFOVolumeGarantyHigh=false;
		m_bDrawHFOVolumeGarantyLow=false;
		return;
	}
	else if(iValueHigh==0 )
	{	
		m_iHFOVolumeGarantyHigh=0;
		m_iHFOVolumeGarantyLow=0;
		m_bDrawHFOVolumeGarantyHigh=false;
		m_bDrawHFOVolumeGarantyLow=false;
		return;
	}

	if(m_iDiagrammType!=PRESSURE_GRAPH && m_iDiagrammType!=PRESSURE_HF_GRAPH)
	{	
		m_iHFOVolumeGarantyHigh=0;
		m_iHFOVolumeGarantyLow=0;
		m_bDrawHFOVolumeGarantyHigh=false;
		m_bDrawHFOVolumeGarantyLow=false;
		return;
	}
	


	LONG result;
	UINT xstart;
	UINT ystart;
	double yperpixel=1/m_dPixelsPerY;

	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen

	m_iHFOVolumeGarantyLow=(int)(ystart-(iValueLow-m_dYAxisMin)/yperpixel);
	m_bDrawHFOVolumeGarantyLow=true;

	m_iHFOVolumeGarantyHigh=(int)(ystart-(iValueHigh-m_dYAxisMin)/yperpixel);
	m_bDrawHFOVolumeGarantyHigh=true;

	bool bScaleHigh=false;
	bool bScaleLow=false;

	if(m_dYAxisMin>=iValueLow)
	{
		bScaleLow=true;
	}
	
	if(iValueHigh>=m_dYAxisMax)
	{
		bScaleHigh=true;
	}

	if(m_iHFOVolumeGarantyLow<100)
		int iStop=0;
	else
		int iStop2=0;
	DEBUGMSG(TRUE, (TEXT("high %d %d\r\n"),m_iHFOVolumeGarantyHigh, iValueHigh));
	DEBUGMSG(TRUE, (TEXT("low %d %d\r\n"),m_iHFOVolumeGarantyLow, iValueLow));
	
	if(m_bAutofit)
	{
		if(bScaleLow && bScaleHigh)
			ScaleToNextPossibleYValue((iValueHigh+5)*G_FACTOR_PRESSURE,(iValueLow-5)*G_FACTOR_PRESSURE,true,true);
		else if(bScaleLow)
			ScaleToNextPossibleYValue(m_dYAxisMax*G_FACTOR_PRESSURE,(iValueLow-5)*G_FACTOR_PRESSURE,true,true);
		else if(bScaleHigh)
			ScaleToNextPossibleYValue((iValueHigh+5)*G_FACTOR_PRESSURE,m_dYAxisMin*G_FACTOR_PRESSURE,true,true);
		else if(isSafeTickCountDelayExpired(m_dwLastCheckAutoScaleY, TIMEAUTOSCALE))
		{
			m_dwLastCheckAutoScaleY=GetTickCount();
			double dYAxisMax=GetYAxisMaxPressure(iValueHigh*G_FACTOR_PRESSURE);
			double dYAxisMin=GetYAxisMinPressure(iValueLow*G_FACTOR_PRESSURE);
			if(m_dYAxisMax>dYAxisMax || m_dYAxisMin<dYAxisMin)
			{
				SetYAxisScale(dYAxisMin, dYAxisMax, TRUE,true);
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFGRAPH(dYAxisMax);
				getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFGRAPH(dYAxisMin);
			}
		}
	}
	
}

void CDiagramm::SetVolumeGaranty(bool drawState, double iValue)
{
	m_bDrawVolumeGaranty=drawState;

	if(drawState==false)
	{	
		m_iVolumeGaranty=0;
		m_bDrawVolumeGaranty=false;
		return;
	}
	else if(iValue==0)
	{	
		m_iVolumeGaranty=0;
		m_bDrawVolumeGaranty=false;
		return;
	}

	if(m_iDiagrammType!=PRESSURE_GRAPH && m_iDiagrammType!=PRESSURE_HF_GRAPH)
	{	
		m_iVolumeGaranty=0;
		m_bDrawVolumeGaranty=false;
		return;
	}



	LONG result;
	UINT xstart;
	UINT ystart;
	double yperpixel=1/m_dPixelsPerY;

	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen

	if(		m_dYAxisMin<=iValue 
		&&	iValue<=m_dYAxisMax)
	{
		m_iVolumeGaranty=(int)(ystart-(iValue-m_dYAxisMin)/yperpixel);
	}
	else
	{
		m_iVolumeGaranty=0;
	}
}

void CDiagramm::SetVolumeLimit(bool drawState, double iLimit)
{
	m_bDrawVolumeLimit=drawState;

	if(drawState==false)
	{	
		m_iVolumeLimit=0;
		m_bDrawVolumeLimit=false;
		return;
	}
	else if(iLimit==0)
	{	
		m_iVolumeLimit=0;
		m_bDrawVolumeLimit=false;
		return;
	}

	if(m_iDiagrammType!=VOLUME_GRAPH && m_iDiagrammType!=VOLUME_HF_GRAPH)
	{	
		m_iVolumeLimit=0;
		m_bDrawVolumeLimit=false;
		return;
	}
	

	
	LONG result;
	UINT xstart;
	UINT ystart;
	double yperpixel=1/m_dPixelsPerY;

	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
	
	if(		m_dYAxisMin*10<=iLimit 
		&&	iLimit<=m_dYAxisMax*10)
	{
		m_iVolumeLimit=ystart-(iLimit-(m_dYAxisMin*10))/(yperpixel*10);
	}
	else
	{
		m_iVolumeLimit=0;
	}
}

void CDiagramm::SetTriggerThreshold(double dTrigThreshold)
{
	if(dTrigThreshold==0)
	{	
		m_iTrigThreshold=0;
		return;
	}
	LONG result;
	UINT xstart;
	UINT ystart;
	double yperpixel=1/m_dPixelsPerY;

	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
	if(m_nID==IDC_LINEDIAGRAM_VOLUME)
		dTrigThreshold=dTrigThreshold/10;
	else
		dTrigThreshold=(dTrigThreshold/1000)*6;

	if(		m_dYAxisMin<=dTrigThreshold 
		&&	dTrigThreshold<=m_dYAxisMax)
	{
		m_iTrigThreshold=(int)(ystart-(dTrigThreshold-m_dYAxisMin)/yperpixel);
	}
	else
	{
		m_iTrigThreshold=0;
	}

	
}


void CDiagramm::NextBreath()
{
	if(m_bExit)
		return;

	CClientDC dc(this);
	//else
	/*if(getModel()->getVMODEHANDLER()->activeModeIsHFO()==false)
	{
		BitBlt(dc.m_hDC, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcLevel1, 0, 0, SRCCOPY);
		BitBlt(m_hdcLevel1, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcLevel2, 0, 0, SRCCOPY);
		BitBlt(m_hdcLevel2, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcStatic, 0, 0, SRCCOPY);

	}
	else
	{
		BitBlt(dc.m_hDC, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcLevel1, 0, 0, SRCCOPY);
		BitBlt(m_hdcLevel1, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcStatic, 0, 0, SRCCOPY);
	}*/
	BitBlt(dc.m_hDC, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcStatic, 0, 0, SRCCOPY);
	
	if(m_bDrawCursor)
		DrawCursor(m_bDrawCursor);
}


//void CDiagramm::SetFirstpoint(bool state, UINT prevx)
//{
//	m_bFirstpoint=state;
//	if(state==false)
//		m_prevx=prevx;
//}


//UINT CDiagramm::GetPrevXval()
//{
//	return m_prevx;
//}

//void CDiagramm::BnClickedXup()
//{
//	MoveXup();
//
//	
//}
//void CDiagramm::BnClickedXdown()
//{
//	MoveXdown();
//
//
//}

bool CDiagramm::MoveXup()
{
	if(m_bExit)
		return false;
	int iDiff=(int)(m_dYAxisMax-m_dYAxisMin);
	double dStep=0;

	if(iDiff<20)
	{
		dStep=2;
	}
	else if(iDiff<100)
	{
		dStep=5;
	}
	else
	{
		dStep=10;
	}


	switch(m_iDiagrammType)
	{
	case PRESSURE_HF_GRAPH:
		{
			double minimum=GetNegativeYoffset(m_dYAxisMax-dStep);

			if(m_dYAxisMax-dStep<0)
			{
				SetYAxisScale(m_dYAxisMin, 0, TRUE, true);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
			}
			else if(m_dYAxisMin-dStep>minimum)
			{
				SetYAxisScale(GetNegativeYoffset(m_dYAxisMax), m_dYAxisMax, TRUE, true);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
			}
			else
			{
				SetYAxisScale(m_dYAxisMin-dStep, m_dYAxisMax-dStep, TRUE, true);
			}
			
			getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFGRAPH(m_dYAxisMax);
			getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFGRAPH(m_dYAxisMin);
		}
		break;
	default:
		break;
	}

	return true;
}
bool CDiagramm::MoveXdown()
{
	if(m_bExit)
		return false;

	int iDiff=(int)(m_dYAxisMax-m_dYAxisMin);
	double dStep=0;

	if(iDiff<20)
	{
		dStep=2;
	}
	else if(iDiff<100)
	{
		dStep=5;
	}
	else
	{
		dStep=10;
	}

	

	switch(m_iDiagrammType)
	{
	case PRESSURE_HF_GRAPH:
		{

			double minimum=GetNegativeYoffset(m_dYAxisMax+dStep);

			if(m_dYAxisMax+dStep<0)
			{
				SetYAxisScale(m_dYAxisMin, 0, true, true);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
			}
			else if(m_dYAxisMin+dStep>minimum)
			{
				SetYAxisScale(GetNegativeYoffset(m_dYAxisMax), m_dYAxisMax, TRUE, true);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
			}
			else
			{
				SetYAxisScale(m_dYAxisMin+dStep, m_dYAxisMax+dStep, TRUE, true);
			}
			
			getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFGRAPH(m_dYAxisMax);
			getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFGRAPH(m_dYAxisMin);
		}
		break;
	default:
		break;
	}

	return true;

}
bool CDiagramm::MoveYleft()
{
	if(m_bExit)
		return false;
	int iDiff=(int)(m_dXAxisMax-m_dXAxisMin);
	double dStep=0;

	if(iDiff<20)
	{
		dStep=2;
	}
	else if(iDiff<100)
	{
		dStep=5;
	}
	else
	{
		dStep=10;
	}



	switch(m_iDiagrammType)
	{
	case PRESSURE_VOLUME_HF_LOOP:
		{

			double minimum=GetNegativeYoffset(m_dXAxisMax+dStep);

			if(m_dXAxisMax+dStep<0)
			{
				SetXAxisScale(m_dXAxisMin, 0, true, true);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
			}
			else if(m_dXAxisMin+dStep>minimum)
			{
				SetXAxisScale(GetNegativeYoffset(m_dXAxisMax), m_dXAxisMax, true, true);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
			}
			else
			{
				SetXAxisScale(m_dXAxisMin+dStep, m_dXAxisMax+dStep, true, true);
			}

			getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFPVLOOP(m_dXAxisMax);
			getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFPVLOOP(m_dXAxisMin);
		}
		break;
	default:
		break;
	}

	return true;
	
}
bool CDiagramm::MoveYright()
{
	if(m_bExit)
		return false;
	int iDiff=(int)(m_dXAxisMax-m_dXAxisMin);
	double dStep=0;

	if(iDiff<20)
	{
		dStep=2;
	}
	else if(iDiff<100)
	{
		dStep=5;
	}
	else
	{
		dStep=10;
	}

	switch(m_iDiagrammType)
	{
	case PRESSURE_VOLUME_HF_LOOP:
		{
			double minimum=GetNegativeYoffset(m_dXAxisMax-dStep);

			if(m_dXAxisMax-dStep<0)
			{
				SetXAxisScale(m_dXAxisMin, 0, true, true);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
			}
			else if(m_dXAxisMin-dStep>minimum)
			{
				SetXAxisScale(GetNegativeYoffset(m_dXAxisMax), m_dXAxisMax, true, true);
				if(AfxGetApp() != NULL)
					AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
			}
			else
			{
				SetXAxisScale(m_dXAxisMin-dStep, m_dXAxisMax-dStep, true, true);
			}

			getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFPVLOOP(m_dXAxisMax);
			getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFPVLOOP(m_dXAxisMin);
		}
		break;
	default:
		break;
	}

	return true;

	
}

//rkuTICKCOUNT
bool CDiagramm::isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay)////used to check if old tick count plus delay is still lower than actual tickCount, (dwLastTickCount+DELAY<getTickCount64())
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