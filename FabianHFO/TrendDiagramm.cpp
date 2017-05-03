// Diagramm.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "TrendDiagramm.h"
#include <math.h>

//global font objects
extern HFONT g_hf3AcuNorm;
extern HFONT g_hf4AcuNorm;
extern HFONT g_hf5AcuNorm;
extern HFONT g_hf7AcuNorm;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf14AcuNormNum;

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000

IMPLEMENT_DYNAMIC(CTrendDiagramm, CWnd)

CTrendDiagramm::CTrendDiagramm()
{
	m_pModel=NULL;

	//m_bCursorBtnPushed=false;

	m_iGraphPenSize=0;
	m_iFontSize=0;//
	m_dXAxisMin=0;// the start value of X
	m_dYAxisMin=0;//start value of Y
	m_dXAxisMax=0;
	m_dYAxisMax=0;
	m_pWnd=NULL;//parent/owner
	m_iCharHeight=0;//calculated from the font
	m_iCharWidth=0;//calculated from the font
	m_iOriginX=0;//location of the origin within the graph
	m_iOriginY=0;//location of the origin within the graph
	m_dPixelsPerY=0;//scaling
	m_dPixelsPerX=0;//scaling	
	m_iScrollPosX=0;//
	m_iScrollPosY=0;//
	m_pFunctionParams=NULL;

	m_nID=0;

	bufferPosValues=new CPoint[TREND_DATAAREA];
	m_iCountbuffer=0;
	m_iPosbufferX=0;

	bufferPosValues[m_iPosbufferX].x=0;
	bufferPosValues[m_iPosbufferX].y=0;

	m_iCurTrendCursorPos=0;

	m_pcCursor=NULL;
	m_pcTrendDecrease_UP=NULL;	//EV_BN_MATRIX_CURSOR_UP
	m_pcTrendDecrease_DW=NULL;
	m_pcTrendIncrease_UP=NULL;
	m_pcTrendIncrease_DW=NULL;

	m_pcGraphDecrease=NULL;
	m_pcGraphIncrease=NULL;

	ptsTrend=new CPoint[MAXSIZE_TREND_BUFFER];
	//ptsTrend=new CPoint[TREND_DATAAREA];

	//m_tsStartPoint.SetDateTimeSpan(0, 0, 0, 0);	//today

	m_hDC				= NULL;
	m_hBmp				= NULL;
	m_hBmpPrev			= NULL;
	m_hdcStatic			= NULL;
	m_hdcGraph			= NULL;
	m_hbmpStatic		= NULL;
	m_hbmpGraph		= NULL;

	m_iTrendType=0;
	m_crYTickColor=0;
	m_crXTickColor=0;
	m_crYLegendTextColor=0;
	m_crXLegendTextColorDate=0;
	m_crXLegendTextColorTime=0;
	m_crGraphTitleColor=0;
	m_crGraphPenColor=0;
	m_crGraphBkColor=0;
	m_crGridColor=0;
	m_crGraphAutostateColor=0;
	m_crGraphAutostateBkColor=0;
	m_crFunctionNameColor=0;
	m_bShowTicks=false;
	m_bShowGrid=false;
	m_bAutofit=false;
	m_bIgnoreAutofit=false;
	m_bYLineAtLeft=false;

	m_iGraphWidth=0;
	m_iGraphHeight=0;

	//set default graph position
	m_iGraphX=0;
	m_iGraphY=0;

	m_iCurColorScheme=G_DEFAULTSCHEME;

	m_bFirstpoint=TRUE;
	m_prevx=0;
	m_prevy=0;

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

	m_bSelected = false;
	m_tsOffsetStartTime.SetDateTimeSpan(0,0,0,0);
}

CTrendDiagramm::~CTrendDiagramm()
{

}
// **************************************************************************
// 
// **************************************************************************
CMVModel *CTrendDiagramm::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}



BEGIN_MESSAGE_MAP(CTrendDiagramm, CWnd)
	//{{AFX_MSG_MAP(CTrendDiagramm)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CTrendDiagramm-Meldungshandler

// **************************************************************************
// 
// **************************************************************************
BOOL CTrendDiagramm::Create(CWnd* pParentWnd, const RECT rc, UINT nID, UINT colorscheme, CCreateContext* pContext) 
{
	GraphSetAllDefaults();

	//setup default values
	m_pWnd=pParentWnd;
	SetColorScheme(colorscheme);
	//set graph position 
	//int Width=rc.right-rc.left;
	//int Height=rc.bottom-rc.top;

	//m_iGraphX=rc.left;
	//m_iGraphY=rc.top;
	m_iGraphX=0;
	m_iGraphY=0;
	//set graphsize (0 or less means default)
	//m_iGraphWidth = Width < G_MINGRAPHWIDTH?G_MINGRAPHWIDTH:Width;
	//m_iGraphHeight=Height < G_MINGRAPHHEIGHT ?G_MINGRAPHHEIGHT:Height;
	m_iGraphWidth = rc.right-rc.left;
	m_iGraphHeight=rc.bottom-rc.top;


	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD/*|WS_VISIBLE*/,rc,pParentWnd,nID))
	{
		m_nID=nID;

		//dtStartDate=COleDateTime::GetCurrentTime();

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





		HFONT hPrevFont=(HFONT)SelectObject(m_hDC,g_hf7AcuNorm);

		TEXTMETRIC textmetrics;
		GetTextMetrics(m_hDC,&textmetrics);
		m_iCharHeight=textmetrics.tmHeight;
		m_iCharWidth=textmetrics.tmAveCharWidth;

		m_rmargin=42;
		m_lmargin=24;
		m_tmargin=m_iCharHeight;
		m_bmargin=m_iCharHeight;

		//CreateGraphFont(_T("arial"),8);

		//G_FabianHFOSCHEME default
		//m_crYTickColor=RGB(0,0,255);
		//m_crXTickColor=RGB(0,0,255);
		//m_crYLegendTextColor=RGB(0,0,255);
		//m_crXLegendTextColor=RGB(0,0,255);
		//m_crGraphTitleColor=RGB(128,255,128);
		//m_crGraphPenColor=RGB(140,140,140);
		//m_crGraphBkColor=BACKGND;
		//m_crFunctionNameColor=RGB(255,255,192);
		//m_crGridColor=RGB(210,210,255);
		////m_crGraphAutostateColor=RGB(255,255,255);	
		////m_crGraphAutostateBkColor=RGB(140,140,140);
		//m_iGraphPenSize=1;


		m_bIgnoreAutofit=false;
		m_bAutofit=getModel()->getCONFIG()->TrendIsAutoScale();
		m_bShowGrid=TRUE;
		m_bShowTicks=TRUE;
		m_bYLineAtLeft=FALSE;

		////set the default axis scaling
		//SetXAxisScale(G_DEFAULTXMIN,G_DEFAULTXMAX);
		//SetYAxisScale(G_DEFAULTYMIN,G_DEFAULTYMAX);
		////set the legend and Title Texts
		////SetDefaultGraphTitle();
		//SetDefaultXLegend();
		//SetDefaultYLegend();
		////SetDefaultFunctionName();

		//other stuff
		m_pFunctionParams=NULL;

		m_pcCursor		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_GRAPH_CURSOR);

		m_pcTrendDecrease_UP		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_DECREASE_UP);
		m_pcTrendDecrease_DW		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_DECREASE_DW);
		m_pcTrendIncrease_UP		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_INCREASE_UP);
		m_pcTrendIncrease_DW		= new CBmp(theApp.m_hInstance,m_hDC,	IDB_BTN_INCREASE_DW);


		BTN btn;

		//CBtn* m_pcGraphXup;
		btn.wID					= IDC_CURSORBTN_UP;	
		btn.poPosition.x		= TREND_WIDTH-37;
		btn.poPosition.y		= 15;
		btn.pcBmpUp				= m_pcTrendDecrease_UP;
		btn.pcBmpDown			= m_pcTrendDecrease_DW;
		btn.pcBmpFocus			= m_pcTrendDecrease_UP;
		btn.pcBmpDisabled		= m_pcTrendDecrease_UP;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcGraphDecrease=new CTrendScaleBtn(btn,COLOR_TXTBTNUP);
		m_pcGraphDecrease->Create(this,g_hf7AcuNorm,0);
		m_pcGraphDecrease->SetText(_T(""));
		m_pcGraphDecrease->ShowWindow(SW_HIDE);

		//CBtn* m_pcGraphXdw;
		btn.wID					= IDC_CURSORBTN_DOWN;	
		btn.poPosition.x		= TREND_WIDTH-37;
		btn.poPosition.y		= 75;
		btn.pcBmpUp				= m_pcTrendIncrease_UP;
		btn.pcBmpDown			= m_pcTrendIncrease_DW;
		btn.pcBmpFocus			= m_pcTrendIncrease_UP;
		btn.pcBmpDisabled		= m_pcTrendIncrease_UP;
		btn.dwFormat			= DT_VCENTER|DT_CENTER;

		m_pcGraphIncrease=new CTrendScaleBtn(btn,COLOR_TXTBTNUP);
		m_pcGraphIncrease->Create(this,g_hf7AcuNorm,0);
		m_pcGraphIncrease->SetText(_T(""));
		m_pcGraphIncrease->ShowWindow(SW_HIDE);

		SelectObject(m_hDC,hPrevFont);

		SelectObject(m_hdcGraph, hpenprevGraph);	
		SelectObject(m_hdcGraph,hbrprevGraph); 
		SelectObject(m_hdcStatic, hpenprevStat);	
		SelectObject(m_hdcStatic,hbrprevStat); 

		return 1;
	}
	else
		return 0;
}

// **************************************************************************
// 
// **************************************************************************
void CTrendDiagramm::OnPaint() 
{
	CPaintDC dc(this);

	BitBlt(dc.m_hDC, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcStatic, 0, 0, SRCCOPY);
	//BitBlt(dc.m_hDC, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcGraph, 0, 0, SRCCOPY);

	//ClearFunction();

}

//************************************
// Method:    OnDestroy
// FullName:  CTrendDiagramm::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CTrendDiagramm::OnDestroy() 
{
	delete m_pcGraphDecrease;
	m_pcGraphDecrease=NULL;

	delete m_pcGraphIncrease;
	m_pcGraphIncrease=NULL;

	delete m_pcTrendDecrease_UP;
	m_pcTrendDecrease_UP=NULL;

	delete m_pcTrendDecrease_DW;
	m_pcTrendDecrease_DW=NULL;

	delete m_pcTrendIncrease_UP;
	m_pcTrendIncrease_UP=NULL;

	delete m_pcTrendIncrease_DW;
	m_pcTrendIncrease_DW=NULL;


	delete m_pcCursor;
	m_pcCursor=NULL;

	delete m_pFunctionParams;
	m_pFunctionParams=NULL;

	delete [] ptsTrend;
	delete [] bufferPosValues;
	
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



	if(m_hbmpGraph)
		DeleteObject(m_hbmpGraph);
}

// **************************************************************************
// 
// **************************************************************************
void CTrendDiagramm::Show(BOOL bShow) 
{
	if(this->IsWindowVisible() == bShow)
	{
		return;
	}

	if(bShow)
	{
		SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
	}
	else
		SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_HIDEWINDOW);
}

// **************************************************************************
// 
// **************************************************************************
int CTrendDiagramm::GetTrendType()
{
	return m_iTrendType;
}

// **************************************************************************
// 
// **************************************************************************
void CTrendDiagramm::SetTrendType(int iType)
{
	m_iTrendType=iType;
}

// **************************************************************************
// This function sets all the member variables to their default values
// creates the default font, etc....
// Because there are so may member variables, attempting to make
// constructors to cover all possibilites will be a pain - so all
// constructors should call this
// function first, then overwrite the default values as required.
// **************************************************************************
void CTrendDiagramm::GraphSetAllDefaults()
{
	SetDefaultColorScheme();
	m_bAutofit=TRUE;
	m_bShowGrid=TRUE;
	m_bShowTicks=TRUE;
	m_bYLineAtLeft=FALSE;
	//set default graph position
	m_iGraphX=0;
	m_iGraphY=0;
	//set default graph size
	//SetGraphSizePos(0, 0,G_MINGRAPHWIDTH,G_MINGRAPHHEIGHT);
	//set the axis scaling
	SetXAxisScale(G_DEFAULTXMIN,G_DEFAULTXMAX);
	SetYAxisScale(G_DEFAULTYMIN,G_DEFAULTYMAX);
	//set the legend and Title Texts
	SetDefaultGraphTitle();
	SetDefaultXLegend();
	SetDefaultYLegend();
	SetDefaultFunctionName();

	//other stuff
	m_pFunctionParams=NULL;
}

/////////////////// SetColorScheme ////////////////////////////////////////////
/*																			///
SetColorScheme set the color scheme for the graph						///
There are 8 colorschemes in total but default colour scheme is the		///
same as as the WHITE colorscheme (which is really grey)					///
*/																			///
///////////////////////////////////////////////////////////////////////////////
void CTrendDiagramm::SetColorScheme(int Scheme, BOOL bRedraw)
{
	/*
	This sets up the colors for various graph elements
	*/
	m_iCurColorScheme=Scheme;
	switch (Scheme)
	{
	case G_DEFAULTSCHEME:
	case G_WHITESCHEME:
		m_crYTickColor=RGB(0,0,0);
		m_crXTickColor=RGB(0,0,0);
		m_crYLegendTextColor=RGB(0,0,0);
		m_crXLegendTextColorDate=RGB(0,0,0);
		m_crXLegendTextColorTime=RGB(100,100,100);
		m_crGraphTitleColor=RGB(0,0,0);
		m_crGraphPenColor=RGB(0,0,0);
		m_crGraphBkColor=RGB(192,192,192);
		m_crFunctionNameColor=RGB(0,0,0);
		m_crGridColor=RGB(220,220,220);	
		m_iGraphPenSize=1;
		m_crGraphAutostateColor=RGB(255,255,255);	
		m_crGraphAutostateBkColor=RGB(140,140,140);
		break;
	case G_REDSCHEME:
		m_crYTickColor=RGB(255,255,255);
		m_crXTickColor=RGB(255,255,255);
		m_crYLegendTextColor=RGB(255,200,200);
		m_crXLegendTextColorDate=RGB(255,150,150);
		m_crXLegendTextColorTime=RGB(255,200,200);
		m_crGraphTitleColor=RGB(255,255,255);
		m_crGraphPenColor=RGB(255,255,0);
		m_crGraphBkColor=RGB(128,0,0);
		m_crFunctionNameColor=RGB(255,0,0);
		m_crGridColor=RGB(200,0,0);	
		m_iGraphPenSize=1;
		m_crGraphAutostateColor=RGB(255,255,255);	
		m_crGraphAutostateBkColor=RGB(140,140,140);
		break;
	case G_BLUESCHEME:
		m_crYTickColor=RGB(255,255,0);
		m_crXTickColor=RGB(255,255,0);
		m_crYLegendTextColor=RGB(200,200,255);
		m_crXLegendTextColorDate=RGB(150,150,255);
		m_crXLegendTextColorTime=RGB(200,200,255);
		m_crGraphTitleColor=RGB(255,255,255);
		m_crGraphPenColor=RGB(0,255,255);
		m_crGraphBkColor=RGB(0,0,128);
		m_crFunctionNameColor=RGB(0,0,255);
		m_crGridColor=RGB(0,0,200);	
		m_iGraphPenSize=1;
		m_crGraphAutostateColor=RGB(255,255,255);	
		m_crGraphAutostateBkColor=RGB(140,140,140);
		break;
	case G_GREENSCHEME:
		m_crYTickColor=RGB(255,255,255);
		m_crXTickColor=RGB(255,255,255);
		m_crYLegendTextColor=RGB(200,255,200);
		m_crXLegendTextColorDate=RGB(150,255,150);
		m_crXLegendTextColorTime=RGB(200,255,200);
		m_crGraphTitleColor=RGB(0,255,255);
		m_crGraphPenColor=RGB(255,255,255);
		m_crGraphBkColor=RGB(0,128,0);
		m_crFunctionNameColor=RGB(255,0,255);
		m_crGridColor=RGB(0,200,0);	
		m_iGraphPenSize=1;
		m_crGraphAutostateColor=RGB(255,255,255);	
		m_crGraphAutostateBkColor=RGB(140,140,140);
		break;
	case G_CYANSCHEME:
		m_crYTickColor=RGB(255,0,255);
		m_crXTickColor=RGB(255,0,255);
		m_crYLegendTextColor=RGB(200,255,200);
		m_crXLegendTextColorDate=RGB(150,255,150);
		m_crXLegendTextColorTime=RGB(200,255,200);
		m_crGraphTitleColor=RGB(192,192,192);
		m_crGraphPenColor=RGB(255,255,0);
		m_crGraphBkColor=RGB(0,128,128);
		m_crFunctionNameColor=RGB(255,255,255);
		m_crGridColor=RGB(0,64,64);	
		m_iGraphPenSize=1;
		m_crGraphAutostateColor=RGB(255,255,255);	
		m_crGraphAutostateBkColor=RGB(140,140,140);
		break;
	case G_YELLOWSCHEME:
		m_crYTickColor=RGB(0,255,255);
		m_crXTickColor=RGB(0,255,255);
		m_crYLegendTextColor=RGB(200,200,0);
		m_crXLegendTextColorDate=RGB(255,255,0);
		m_crXLegendTextColorTime=RGB(255,255,50);
		m_crGraphTitleColor=RGB(192,192,192);
		m_crGraphPenColor=RGB(192,192,192);
		m_crGraphBkColor=RGB(128,128,0);
		m_crFunctionNameColor=RGB(255,255,255);
		m_crGridColor=RGB(0,64,64);	
		m_iGraphPenSize=1;
		m_crGraphAutostateColor=RGB(255,255,255);	
		m_crGraphAutostateBkColor=RGB(140,140,140);
		break;
	case G_MAGENTASCHEME:
		m_crYTickColor=RGB(192,192,255);
		m_crXTickColor=RGB(100,100,255);
		m_crYLegendTextColor=RGB(255,0,0);
		m_crXLegendTextColorDate=RGB(255,128,128);
		m_crXLegendTextColorTime=RGB(255,170,170);
		m_crGraphTitleColor=RGB(128,255,128);
		m_crGraphPenColor=RGB(255,255,255);
		m_crGraphBkColor=RGB(128,0,128);
		m_crFunctionNameColor=RGB(255,255,192);
		m_crGridColor=RGB(192,0,192);
		m_iGraphPenSize=1;
		m_crGraphAutostateColor=RGB(255,255,255);	
		m_crGraphAutostateBkColor=RGB(140,140,140);
		break;
	case G_FabianHFOSCHEME_LINE:
		m_crYTickColor=RGB(0,0,255);
		m_crXTickColor=RGB(0,0,255);
		m_crYLegendTextColor=RGB(0,0,255);
		m_crXLegendTextColorDate=RGB(0,0,0);
		m_crXLegendTextColorTime=RGB(0,0,255);
		m_crGraphTitleColor=RGB(0,0,255);
		m_crGraphPenColor=RGB(140,140,140);
		m_crGraphBkColor=WHITE;
		m_crFunctionNameColor=RGB(0,0,0);
		m_crGridColor=RGB(210,210,255);
		m_iGraphPenSize=1;
		m_crGraphAutostateColor=RGB(255,255,255);	
		m_crGraphAutostateBkColor=RGB(140,140,140);
		break;
	default:
		break;

	}
	if(bRedraw)
	{
		PaintGraph();
	}
}






////////////////////////// CalcTopMargin ///////////////////////////////////
/*
Returns the spacing between the top adge of the graph and the top of the	
plotting area.
*/
////////////////////////////////////////////////////////////////////////////
//UINT CTrendDiagramm::CalcTopMargin()
//{
//	/*the top margin comprises the following
//	1* blank line + 1*Line (graph Title) + 
//	1*blank line +1*Line (graph function);
//	This margin is fixed
//	*/
//	return 4*m_iCharHeight;
//}


/////////////////////  CalcBottommargin  //////////////////////////////////
/*
Returns the space between the bottom of the grapgh and the bottom
of the actual plotting area
*/
//////////////////////////////////////////////////////////////////////////
//UINT CTrendDiagramm::CalcBottomMargin()
//{
//	/* The bottom margin comprises the following
//	1*blank line +1*Line(x axis title) + 1* line(x-values)+1*line(ticks);
//	Mind you this is a 'minimum margin' - required below the x-axis
//	There might be a lot of space below this depending on the x-axis scale
//	*/
//	return 3*m_iCharHeight;
//}

/////////////////////// CalcLeftmargin ////////////////////////////////////
/*
Returns the margin between the left of the graph and the left of the
plotiing area
*/
////////////////////////////////////////////////////////////////////////////
//UINT CTrendDiagramm::CalcLeftMargin()
//{
//	/*The Left margin comprise the following;
//	1*blank line + 5*lines (for the yaxis values)+1*line (yticks);
//	This margin is a 'minimum' value - there may be more space to the 
//	left of this depending on negative scaling of the x-axis
//	*/
//	return 13*m_iCharWidth;
//}

///////////////////////// CalcRightMargin ////////////////////////////////////
/*
Returns the space between the right side of the plotting area and the right
sode of the graph
*/
//////////////////////////////////////////////////////////////////////////////
//UINT CTrendDiagramm::CalcRightMargin()
//{
//	/*This margin is simple - there is a blank line
//	on the right side of width:
//	*/
//	return m_iCharWidth;
//}

// **************************************************************************
// This sets the min and max values of the x-axis (of the plotting area)
//				It also sets what proportion of the x-range is denoted by one pixel
//				a.ka. PixelsPerX
//				It also caluclates where the x-origin (the x==0) point lies along
//				the x-axis - if x does not pass through zero the x-origin gidline is shown
//				at the top or bottom of the graph.
//				(the x-origin point on screen is stored in the m_iOriginX parameter as 
//				 an offset from the LHS of the (total) graph
// **************************************************************************
void CTrendDiagramm::SetXAxisScale(double min, double max, BOOL bRedraw)
{
	//DEBUGMSG(TRUE, (TEXT("SetXAxisScale ID %d min %d max %d\r\n"),m_nID,min,max));
	
	//swap min and max if they are the wrong way around
	double temp;
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
	//
	temp=max-min;//the spread of the x-axiz
	m_dPixelsPerX=(m_iGraphWidth-m_lmargin-m_rmargin)/temp;//calc pixels/x
	//m_dPixelsPerX=scale;

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

	

	SetXAxisTickSpace();

	if(bRedraw)
	{
		PaintGraph();
	}
}

double CTrendDiagramm::GetPixelsPerMinute()
{
	return m_dPixelsPerX;
}

// **************************************************************************
// 
// **************************************************************************
int CTrendDiagramm::GetYPixelsOfDiagramm()
{
	return m_iGraphHeight-(m_bmargin+m_tmargin);
}

/////////////////////// SetYAxisScale /////////////////////////////////////////
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
// **************************************************************************
void CTrendDiagramm::SetYAxisScale(double min, double max, BOOL bRedraw)
{
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
	else if(min<0 && max<=0)//if Y values are all negative
	{
		m_iOriginY=(dataarea.bottom-dataarea.top)+m_bmargin;
	}
	else if (min >=0 && max >=0)//if Y values are all positive
	{
		m_iOriginY=m_bmargin;
	}	

	SetYAxisTickSpace(CalculateYAxisTickGridSpace());
	SetYAxisNumericSpace(CalculateYAxisNumericSpace());

	if(bRedraw)
	{
		PaintGraph();
	}		
}


//////////////////////// PaintGraph //////////////////////////////////////
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
void CTrendDiagramm::PaintGraph()
{
	//here we draw the graph
	//step 1: Draw the surrounding rectangle
	//for the whole graph
	CRect rect;
	CPen penBack,penTick, *oldpen;
	CClientDC dc(this);
	CDC* pDCStatic=CDC::FromHandle(m_hdcStatic);

	//some useful calculations
	UINT Graphbottom=m_iGraphY+m_iGraphHeight;
	UINT Graphright=m_iGraphX+m_iGraphWidth;

	//step 2: color the background
	CBrush brush,*poldbrush;
	brush.CreateSolidBrush(m_crGraphBkColor);
	penBack.CreatePen(PS_SOLID,1,BLACK);
	rect.left=m_iGraphX;
	rect.right =rect.left+m_iGraphWidth;
	rect.top=m_iGraphY;
	rect.bottom=rect.top+m_iGraphHeight;
	oldpen=pDCStatic->SelectObject(&penBack);
	poldbrush=pDCStatic->SelectObject(&brush);
	pDCStatic->Rectangle(&rect);
	
	

	//step 3: Draw Grid if required
	DrawGridNum(pDCStatic);

	//step 4: Draw Axies
	//draw x-axis
	penTick.CreatePen(PS_SOLID,1,m_crXTickColor);
	pDCStatic->SelectObject(&penTick);
	pDCStatic->MoveTo(m_iGraphX+m_lmargin,Graphbottom-m_iOriginY);
	pDCStatic->LineTo(Graphright-m_rmargin,Graphbottom-m_iOriginY);

	//draw the Y Axis
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
	
	//step 5: draw ticks if required
	//DrawTicks(pDCStatic);


	////step 6: Write Graph title
	//DrawGraphTitle();

	////step 7: Write Function name
	//DrawFunctionName();

	//step 8: Write x-legend
	//DrawXLegend(pDCStatic);

	//step 9: Write y legend
	//doing this is very similar to doing the graph title or the
	//x-legend but the Y axis is either at the LHS or set to match the
	//x-origin
	DrawYLegend(pDCStatic);

	//step 10: write the x & y axes values
	//DrawXAxisNumbers(pDCStatic);
	DrawYAxisNumbers(pDCStatic);

	//Step 11
	if(m_bAutofit)
		DrawAutoscaleState(pDCStatic);

	BitBlt(m_hDC, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcStatic, 0, 0, SRCCOPY);
	BitBlt(dc.m_hDC, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcStatic, 0, 0, SRCCOPY);

	BitBlt(m_hdcGraph, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcStatic, 0, 0, SRCCOPY);

	pDCStatic->SelectObject(oldpen);
	pDCStatic->SelectObject(poldbrush);
	penBack.DeleteObject();
	penTick.DeleteObject();
	//DeleteObject(brush);
}

// **************************************************************************
// Draws the state "autoscale" on the top right side of the graph
//				if autoscale state is set.
// **************************************************************************
void CTrendDiagramm::DrawAutoscaleState(CDC* pDC)
{
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

//////////////////// DrawGrid ////////////////////////////////////////////////
// **************************************************************************
// The grid comprises two parts - the rectangle drawn around the plotting area
//				and the vertical&horizontal gridlines.
//				The outline rectangle is always drawn, the drawing of the gidlines
//				is contrlled by the m_bShowGrid member parameter using the ShowGrid function.
//				The grid color is  m_crGridColor.
// **************************************************************************
void CTrendDiagramm::DrawGridNum(CDC* pDC)
{

	/*
	Always draw the dataarea outline rectangle
	*/
	HFONT hPrevFont=(HFONT)pDC->SelectObject(g_hf7AcuNorm);
	CRect dataarea=CalcDataArea();;//where the graph data is actually drawn

	//need a pen of colour m_crGridColor
	CPen penSolid,penDash, *poldpen;
	penSolid.CreatePen(PS_SOLID,1,m_crGridColor);
	penDash.CreatePen(PS_DASH,1,m_crGridColor);

	

	//to make a rectangle outline we have to use a polyline
	//need an array of points
	//a recatangle comprises 5 point
	CPoint points[5];
	//topleft
	points[0].x=dataarea.left;
	points[0].y=dataarea.top;
	//topright
	points[1].x=dataarea.right;
	points[1].y=dataarea.top;
	//bottomright
	points[2].x=dataarea.right;
	points[2].y=dataarea.bottom;
	//leftbottom
	points[3].x=dataarea.left;
	points[3].y=dataarea.bottom;
	//back to topleft
	points[4].x=dataarea.left;
	points[4].y=dataarea.top;

	poldpen=pDC->SelectObject(&penSolid);
	pDC->Polyline(points,5);//draw the outline rectangle

	/* Now check whether the grid itself should
	be shown
	*/
	if(!m_bShowGrid)
	{
		pDC->SelectObject(poldpen);
		penSolid.DeleteObject();
		penDash.DeleteObject();
		return;
	}

	pDC->SetTextColor(m_crXLegendTextColorDate);
	pDC->SetBkMode(TRANSPARENT);


	CBrush brush;
	brush.CreateSolidBrush(m_crGraphBkColor);

	//draw the X-axis gridlines
	//note x-axis grid lines run top tom bottom
	double GridSpacing=CalcXAxisGridAndTicks();
	//int n=1; //for the loop
	//draw positive xgrids + negative xgrids
	//UINT Graphright=m_iGraphX+m_iGraphWidth;
	//UINT Graphright=m_iGraphX+m_iGraphWidth;
	//int iXpixels=dataarea.right-dataarea.left;

	int i=0;
	CStringW szDate = _T("");
	CRect rect;
	int iTrendSpan=(int)m_dXAxisMax;

	/*SYSTEMTIME st;
	GetLocalTime(&st);
	m_dtStartTime=COleDateTime(st);*/

	COleDateTime tmBeginGrid;
	tmBeginGrid.SetDateTime(m_dtStartTime.GetYear(), m_dtStartTime.GetMonth(), m_dtStartTime.GetDay(), m_dtStartTime.GetHour(),m_dtStartTime.GetMinute(),0);
	COleDateTimeSpan tsOffset;
	COleDateTimeSpan tsGrid;
	double dXGridOffset=0;
	int iOffsetMin=0;
	int iOffsetHours=0;
	//int iOffsetDays=0;

	//m_tsOffsetStartTime.SetDateTimeSpan(1, 16, 0, 0);
	tmBeginGrid=tmBeginGrid-m_tsOffsetStartTime;
	m_dtOffsetTime=tmBeginGrid;

	//DEBUGMSG(TRUE, (TEXT("DrawGridNum ID %d iTrendSpan %d\r\n"),m_nID,iTrendSpan));

	switch(iTrendSpan)
	{
	case TREND_30MIN:
		{
			tsGrid.SetDateTimeSpan(0, 0, TREND_TICKMIN_30MIN, 0);
			iOffsetMin=tmBeginGrid.GetMinute();
			iOffsetMin=iOffsetMin%TREND_TICKMIN_30MIN;
			tsOffset.SetDateTimeSpan(0, 0, iOffsetMin, 0);
			tmBeginGrid=tmBeginGrid-tsOffset;
			dXGridOffset=(double)iOffsetMin*(GridSpacing/m_dXTickSpace);

			while(dataarea.left<(dataarea.right-i*GridSpacing-dXGridOffset))
			{
				if(tmBeginGrid.GetHour()==0 && tmBeginGrid.GetMinute()==0)
				{
					pDC->SetTextColor(m_crXLegendTextColorDate);
					pDC->SelectObject(&penSolid);
					szDate = tmBeginGrid.Format(_T("%d.%m."));
				}
				else
				{
					pDC->SetTextColor(m_crXLegendTextColorTime);
					pDC->SelectObject(&penDash);
					szDate = tmBeginGrid.Format(_T("%H:%M"));
				}

				pDC->MoveTo((int)(dataarea.right-i*GridSpacing-dXGridOffset), (int)dataarea.top);
				pDC->LineTo((int)(dataarea.right-i*GridSpacing-dXGridOffset),(int)dataarea.bottom);

				rect.left=dataarea.right-i*GridSpacing-3*m_iCharWidth-dXGridOffset;
				rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters
				/*rect.top=m_iGraphY+m_iGraphHeight-1*m_iCharHeight;
				rect.bottom=rect.top+m_iCharHeight;*/
				rect.top=m_iGraphY+m_iGraphHeight-m_iCharHeight;
				rect.bottom=m_iGraphHeight-1;
				rect.top+=1;
				//rect.bottom+=1;

				pDC->FillRect(&rect,&brush);
				//pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_CENTER);
				pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_VCENTER|DT_BOTTOM);

				tmBeginGrid=tmBeginGrid-tsGrid;

				i++;
			}
		}
		break;
	case TREND_1HOUR:
		{
			tsGrid.SetDateTimeSpan(0, 0, TREND_TICKMIN_1HOUR, 0);
			iOffsetMin=tmBeginGrid.GetMinute();
			iOffsetMin=iOffsetMin%TREND_TICKMIN_1HOUR;
			tsOffset.SetDateTimeSpan(0, 0, iOffsetMin, 0);
			tmBeginGrid=tmBeginGrid-tsOffset;
			dXGridOffset=(double)iOffsetMin*(GridSpacing/m_dXTickSpace);

			while(dataarea.left<(dataarea.right-i*GridSpacing-dXGridOffset))
			{
				if(tmBeginGrid.GetHour()==0 && tmBeginGrid.GetMinute()==0)
				{
					pDC->SetTextColor(m_crXLegendTextColorDate);
					pDC->SelectObject(&penSolid);
					szDate = tmBeginGrid.Format(_T("%d.%m."));
				}
				else
				{
					pDC->SetTextColor(m_crXLegendTextColorTime);
					pDC->SelectObject(&penDash);
					szDate = tmBeginGrid.Format(_T("%H:%M"));
				}

				pDC->MoveTo((int)(dataarea.right-i*GridSpacing-dXGridOffset), (int)dataarea.top);
				pDC->LineTo((int)(dataarea.right-i*GridSpacing-dXGridOffset),(int)dataarea.bottom);

				rect.left=dataarea.right-i*GridSpacing-3*m_iCharWidth-dXGridOffset;
				rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters
				/*rect.top=m_iGraphY+m_iGraphHeight-1*m_iCharHeight;
				rect.bottom=rect.top+m_iCharHeight;*/
				rect.top=m_iGraphY+m_iGraphHeight-m_iCharHeight;
				rect.bottom=m_iGraphHeight-1;
				rect.top+=1;
				//rect.bottom+=1;

				pDC->FillRect(&rect,&brush);
				//pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_CENTER);
				pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_VCENTER|DT_BOTTOM);

				tmBeginGrid=tmBeginGrid-tsGrid;

				i++;
			}
		}
		break;
	case TREND_2HOURS:
		{
			tsGrid.SetDateTimeSpan(0, 0, TREND_TICKMIN_2HOURS, 0);
			iOffsetMin=tmBeginGrid.GetMinute();
			iOffsetMin=iOffsetMin%TREND_TICKMIN_2HOURS;
			tsOffset.SetDateTimeSpan(0, 0, iOffsetMin, 0);
			tmBeginGrid=tmBeginGrid-tsOffset;
			dXGridOffset=(double)iOffsetMin*(GridSpacing/m_dXTickSpace);

			while(dataarea.left<(dataarea.right-i*GridSpacing-dXGridOffset))
			{
				if(tmBeginGrid.GetHour()==0 && tmBeginGrid.GetMinute()==0)
				{
					pDC->SetTextColor(m_crXLegendTextColorDate);
					pDC->SelectObject(&penSolid);
					szDate = tmBeginGrid.Format(_T("%d.%m."));
				}
				else
				{
					pDC->SetTextColor(m_crXLegendTextColorTime);
					pDC->SelectObject(&penDash);
					szDate = tmBeginGrid.Format(_T("%H:%M"));
				}

				pDC->MoveTo((int)(dataarea.right-i*GridSpacing-dXGridOffset), (int)dataarea.top);
				pDC->LineTo((int)(dataarea.right-i*GridSpacing-dXGridOffset),(int)dataarea.bottom);
				
				rect.left=dataarea.right-i*GridSpacing-3*m_iCharWidth-dXGridOffset;
				rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters
				/*rect.top=m_iGraphY+m_iGraphHeight-1*m_iCharHeight;
				rect.bottom=rect.top+m_iCharHeight;*/
				rect.top=m_iGraphY+m_iGraphHeight-m_iCharHeight;
				rect.bottom=m_iGraphHeight-1;
				rect.top+=1;
				//rect.bottom+=1;

				pDC->FillRect(&rect,&brush);
				//pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_CENTER);
				pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_VCENTER|DT_BOTTOM);

				tmBeginGrid=tmBeginGrid-tsGrid;

				i++;
			}
		}
		break;
	case TREND_4HOURS:
		{
			tsGrid.SetDateTimeSpan(0, 0, TREND_TICKMIN_4HOURS, 0);
			iOffsetMin=tmBeginGrid.GetMinute();
			iOffsetMin=iOffsetMin%TREND_TICKMIN_4HOURS;
			tsOffset.SetDateTimeSpan(0, 0, iOffsetMin, 0);
			tmBeginGrid=tmBeginGrid-tsOffset;
			dXGridOffset=(double)iOffsetMin*(GridSpacing/m_dXTickSpace);

			while(dataarea.left<(dataarea.right-i*GridSpacing-dXGridOffset))
			{
				if(tmBeginGrid.GetHour()==0 && tmBeginGrid.GetMinute()==0)
				{
					pDC->SetTextColor(m_crXLegendTextColorDate);
					pDC->SelectObject(&penSolid);
					szDate = tmBeginGrid.Format(_T("%d.%m."));
				}
				else
				{
					pDC->SetTextColor(m_crXLegendTextColorTime);
					pDC->SelectObject(&penDash);
					szDate = tmBeginGrid.Format(_T("%H:%M"));
				}

				pDC->MoveTo((int)(dataarea.right-i*GridSpacing-dXGridOffset), (int)dataarea.top);
				pDC->LineTo((int)(dataarea.right-i*GridSpacing-dXGridOffset),(int)dataarea.bottom);

				rect.left=dataarea.right-i*GridSpacing-3*m_iCharWidth-dXGridOffset;
				rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters
				/*rect.top=m_iGraphY+m_iGraphHeight-1*m_iCharHeight;
				rect.bottom=rect.top+m_iCharHeight;*/
				rect.top=m_iGraphY+m_iGraphHeight-m_iCharHeight;
				rect.bottom=m_iGraphHeight-1;
				rect.top+=1;
				//rect.bottom+=1;

				pDC->FillRect(&rect,&brush);
				//pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_CENTER);
				pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_VCENTER|DT_BOTTOM);

				tmBeginGrid=tmBeginGrid-tsGrid;

				i++;
			}
		}
		break;
	case TREND_8HOURS:
		{
			tsGrid.SetDateTimeSpan(0, 0, TREND_TICKMIN_8HOURS, 0);
			iOffsetMin=tmBeginGrid.GetMinute();
			iOffsetMin=iOffsetMin%TREND_TICKMIN_8HOURS;
			tsOffset.SetDateTimeSpan(0, 0, iOffsetMin, 0);
			tmBeginGrid=tmBeginGrid-tsOffset;
			dXGridOffset=(double)iOffsetMin*(GridSpacing/m_dXTickSpace);

			while(dataarea.left<(dataarea.right-i*GridSpacing-dXGridOffset))
			{
				if(tmBeginGrid.GetHour()==0 && tmBeginGrid.GetMinute()==0)
				{
					pDC->SetTextColor(m_crXLegendTextColorDate);
					pDC->SelectObject(&penSolid);
					szDate = tmBeginGrid.Format(_T("%d.%m."));
				}
				else
				{
					pDC->SetTextColor(m_crXLegendTextColorTime);
					pDC->SelectObject(&penDash);
					szDate = tmBeginGrid.Format(_T("%H:%M"));
				}

				pDC->MoveTo((int)(dataarea.right-i*GridSpacing-dXGridOffset), (int)dataarea.top);
				pDC->LineTo((int)(dataarea.right-i*GridSpacing-dXGridOffset),(int)dataarea.bottom);

				rect.left=dataarea.right-i*GridSpacing-3*m_iCharWidth-dXGridOffset;
				rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters
				/*rect.top=m_iGraphY+m_iGraphHeight-1*m_iCharHeight;
				rect.bottom=rect.top+m_iCharHeight;*/
				rect.top=m_iGraphY+m_iGraphHeight-m_iCharHeight;
				rect.bottom=m_iGraphHeight-1;
				rect.top+=1;
				//rect.bottom+=1;

				pDC->FillRect(&rect,&brush);
				//pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_CENTER);
				pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_VCENTER|DT_BOTTOM);

				tmBeginGrid=tmBeginGrid-tsGrid;

				i++;
			}

			
		}
		break;
	case TREND_16HOURS:
		{
			tsGrid.SetDateTimeSpan(0, 0, TREND_TICKMIN_16HOURS, 0);
			iOffsetMin=tmBeginGrid.GetMinute();
			iOffsetMin=iOffsetMin%TREND_TICKMIN_16HOURS;
			iOffsetHours=tmBeginGrid.GetHour();
			iOffsetHours=iOffsetHours%2;
			tsOffset.SetDateTimeSpan(0, iOffsetHours, iOffsetMin, 0);
			tmBeginGrid=tmBeginGrid-tsOffset;
			dXGridOffset=(double)(iOffsetMin+(iOffsetHours*60))*(GridSpacing/m_dXTickSpace);

			while(dataarea.left<(dataarea.right-i*GridSpacing-dXGridOffset))
			{
				if(tmBeginGrid.GetHour()==0)
				{
					pDC->SetTextColor(m_crXLegendTextColorDate);
					pDC->SelectObject(&penSolid);
					szDate = tmBeginGrid.Format(_T("%d.%m."));
				}
				else
				{
					pDC->SetTextColor(m_crXLegendTextColorTime);
					pDC->SelectObject(&penDash);
					szDate = tmBeginGrid.Format(_T("%H:%M"));
				}

				pDC->MoveTo((int)(dataarea.right-i*GridSpacing-dXGridOffset), (int)dataarea.top);
				pDC->LineTo((int)(dataarea.right-i*GridSpacing-dXGridOffset),(int)dataarea.bottom);

				rect.left=dataarea.right-i*GridSpacing-3*m_iCharWidth-dXGridOffset;
				rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters
				/*rect.top=m_iGraphY+m_iGraphHeight-1*m_iCharHeight;
				rect.bottom=rect.top+m_iCharHeight;*/
				rect.top=m_iGraphY+m_iGraphHeight-m_iCharHeight;
				rect.bottom=m_iGraphHeight-1;
				rect.top+=1;
				//rect.bottom+=1;

				pDC->FillRect(&rect,&brush);
				//pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_CENTER);
				pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_VCENTER|DT_BOTTOM);

				tmBeginGrid=tmBeginGrid-tsGrid;

				i++;
			}
		}
		break;
	case TREND_1DAY4HOURS:
		{
			tsGrid.SetDateTimeSpan(0, 0, TREND_TICKMIN_1DAY4HOURS, 0);
			iOffsetMin=tmBeginGrid.GetMinute();
			iOffsetMin=iOffsetMin%TREND_TICKMIN_1DAY4HOURS;
			iOffsetHours=tmBeginGrid.GetHour();
			iOffsetHours=iOffsetHours%4;
			tsOffset.SetDateTimeSpan(0, iOffsetHours, iOffsetMin, 0);
			tmBeginGrid=tmBeginGrid-tsOffset;
			dXGridOffset=(double)(iOffsetMin+(iOffsetHours*60))*(GridSpacing/m_dXTickSpace);

			while(dataarea.left<(dataarea.right-i*GridSpacing-dXGridOffset))
			{
				if(tmBeginGrid.GetHour()==0)
				{
					pDC->SetTextColor(m_crXLegendTextColorDate);
					pDC->SelectObject(&penSolid);
					szDate = tmBeginGrid.Format(_T("%d.%m."));
				}
				else
				{
					pDC->SetTextColor(m_crXLegendTextColorTime);
					pDC->SelectObject(&penDash);
					szDate = tmBeginGrid.Format(_T("%H:%M"));
				}

				pDC->MoveTo((int)(dataarea.right-i*GridSpacing-dXGridOffset), (int)dataarea.top);
				pDC->LineTo((int)(dataarea.right-i*GridSpacing-dXGridOffset),(int)dataarea.bottom);

				rect.left=dataarea.right-i*GridSpacing-3*m_iCharWidth-dXGridOffset;
				rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters
				/*rect.top=m_iGraphY+m_iGraphHeight-1*m_iCharHeight;
				rect.bottom=rect.top+m_iCharHeight;*/
				rect.top=m_iGraphY+m_iGraphHeight-m_iCharHeight;
				rect.bottom=m_iGraphHeight-1;
				rect.top+=1;
				//rect.bottom+=1;

				pDC->FillRect(&rect,&brush);
				//pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_CENTER);
				pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_VCENTER|DT_BOTTOM);

				tmBeginGrid=tmBeginGrid-tsGrid;

				i++;
			}
		}
		break;
	case TREND_3DAYS:
		{
			tsGrid.SetDateTimeSpan(0, 0, TREND_TICKMIN_3DAYS, 0);
			iOffsetMin=tmBeginGrid.GetMinute();
			iOffsetMin=iOffsetMin%TREND_TICKMIN_3DAYS;
			iOffsetHours=tmBeginGrid.GetHour();
			iOffsetHours=iOffsetHours%12;
			tsOffset.SetDateTimeSpan(0, iOffsetHours, iOffsetMin, 0);
			tmBeginGrid=tmBeginGrid-tsOffset;
			dXGridOffset=(double)(iOffsetMin+(iOffsetHours*60))*(GridSpacing/m_dXTickSpace);

			while(dataarea.left<(dataarea.right-i*GridSpacing-dXGridOffset))
			{
				if(tmBeginGrid.GetHour()==0)
				{
					pDC->SetTextColor(m_crXLegendTextColorDate);
					pDC->SelectObject(&penSolid);
					szDate = tmBeginGrid.Format(_T("%d.%m."));
				}
				else
				{
					/*pDC->SetTextColor(m_crXLegendTextColorTime);
					pDC->SelectObject(&penDash);
					szDate = tmBeginGrid.Format(_T("%H:%M"));*/
					pDC->SelectObject(&penDash);
					szDate = _T("");
				}

				pDC->MoveTo((int)(dataarea.right-i*GridSpacing-dXGridOffset), (int)dataarea.top);
				pDC->LineTo((int)(dataarea.right-i*GridSpacing-dXGridOffset),(int)dataarea.bottom);

				rect.left=dataarea.right-i*GridSpacing-3*m_iCharWidth-dXGridOffset;
				rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters
				/*rect.top=m_iGraphY+m_iGraphHeight-1*m_iCharHeight;
				rect.bottom=rect.top+m_iCharHeight;*/
				rect.top=m_iGraphY+m_iGraphHeight-m_iCharHeight;
				rect.bottom=m_iGraphHeight-1;
				rect.top+=1;
				//rect.bottom+=1;

				pDC->FillRect(&rect,&brush);
				//pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_CENTER);
				pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_VCENTER|DT_BOTTOM);

				tmBeginGrid=tmBeginGrid-tsGrid;

				i++;
			}
		}
		break;
	case TREND_5DAYS:
		{
			tsGrid.SetDateTimeSpan(0, 0, TREND_TICKMIN_5DAYS, 0);
			iOffsetMin=tmBeginGrid.GetMinute();
			iOffsetMin=iOffsetMin%TREND_TICKMIN_5DAYS;
			iOffsetHours=tmBeginGrid.GetHour();
			iOffsetHours=iOffsetHours%12;
			tsOffset.SetDateTimeSpan(0, iOffsetHours, iOffsetMin, 0);
			tmBeginGrid=tmBeginGrid-tsOffset;
			dXGridOffset=(double)(iOffsetMin+(iOffsetHours*60))*(GridSpacing/m_dXTickSpace);

			while(dataarea.left<(dataarea.right-i*GridSpacing-dXGridOffset))
			{
				if(tmBeginGrid.GetHour()==0)
				{
					pDC->SelectObject(&penSolid);
					szDate = tmBeginGrid.Format(_T("%d.%m."));
				}
				else
				{
					pDC->SelectObject(&penDash);
					szDate = _T("");
				}

				pDC->MoveTo((int)(dataarea.right-i*GridSpacing-dXGridOffset), (int)dataarea.top);
				pDC->LineTo((int)(dataarea.right-i*GridSpacing-dXGridOffset),(int)dataarea.bottom);

				rect.left=dataarea.right-i*GridSpacing-3*m_iCharWidth-dXGridOffset;
				rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters
				/*rect.top=m_iGraphY+m_iGraphHeight-1*m_iCharHeight;
				rect.bottom=rect.top+m_iCharHeight;*/
				rect.top=m_iGraphY+m_iGraphHeight-m_iCharHeight;
				rect.bottom=m_iGraphHeight-1;
				rect.top+=1;
				//rect.bottom+=1;

				pDC->FillRect(&rect,&brush);
				//pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_CENTER);
				pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_VCENTER|DT_BOTTOM);

				tmBeginGrid=tmBeginGrid-tsGrid;

				i++;
			}
		}
		break;
	//case TREND_10DAYS:
	//	{
	//		tsGrid.SetDateTimeSpan(0, 0, TREND_TICKMIN_10DAYS, 0);
	//		iOffsetMin=tmBeginGrid.GetMinute();
	//		iOffsetHours=tmBeginGrid.GetHour();
	//		tsOffset.SetDateTimeSpan(0, iOffsetHours, iOffsetMin, 0);
	//		tmBeginGrid=tmBeginGrid-tsOffset;
	//		dXGridOffset=(double)(iOffsetMin+(iOffsetHours*60))*(GridSpacing/m_dXTickSpace);

	//		while(dataarea.left<(dataarea.right-i*GridSpacing-dXGridOffset))
	//		{

	//			pDC->MoveTo(dataarea.right-i*GridSpacing-dXGridOffset, dataarea.top);
	//			pDC->LineTo(dataarea.right-i*GridSpacing-dXGridOffset,dataarea.bottom);

	//			szDate = tmBeginGrid.Format(_T("%d.%m."));

	//			rect.left=dataarea.right-i*GridSpacing-3*m_iCharWidth-dXGridOffset;
	//			rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters
	//			/*rect.top=m_iGraphY+m_iGraphHeight-1*m_iCharHeight;
	//			rect.bottom=rect.top+m_iCharHeight;*/
	//			rect.top=m_iGraphY+m_iGraphHeight-m_iCharHeight;
	//			rect.bottom=m_iGraphHeight-1;
	//			rect.top+=1;
	//			//rect.bottom+=1;

	//			pDC->FillRect(&rect,&brush);
	//			//pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_CENTER);
	//			pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_VCENTER|DT_BOTTOM);

	//			tmBeginGrid=tmBeginGrid-tsGrid;

	//			i++;
	//		}
	//	}
	//	break;
	//case TREND_20DAYS:
	//	{
	//		tsGrid.SetDateTimeSpan(0, 0, TREND_TICKMIN_20DAYS, 0);
	//		iOffsetMin=tmBeginGrid.GetMinute();
	//		iOffsetHours=tmBeginGrid.GetHour();
	//		tsOffset.SetDateTimeSpan(0, iOffsetHours, iOffsetMin, 0);
	//		tmBeginGrid=tmBeginGrid-tsOffset;
	//		dXGridOffset=(double)(iOffsetMin+(iOffsetHours*60))*(GridSpacing/m_dXTickSpace);

	//		while(dataarea.left<(dataarea.right-i*GridSpacing-dXGridOffset))
	//		{

	//			pDC->MoveTo(dataarea.right-i*GridSpacing-dXGridOffset, dataarea.top);
	//			pDC->LineTo(dataarea.right-i*GridSpacing-dXGridOffset,dataarea.bottom);

	//			szDate = tmBeginGrid.Format(_T("%d.%m."));

	//			rect.left=dataarea.right-i*GridSpacing-3*m_iCharWidth-dXGridOffset;
	//			rect.top=m_iGraphY+m_iGraphHeight-1*m_iCharHeight;
	//			rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters
	//			rect.bottom=rect.top+m_iCharHeight;
	//			rect.top+=1;
	//			rect.bottom+=1;

	//			pDC->FillRect(&rect,&brush);
	//			pDC->DrawText(szDate,&rect,DT_NOCLIP|DT_CENTER);

	//			tmBeginGrid=tmBeginGrid-tsGrid;

	//			i++;
	//		}
	//	}
	//	break;
	default:
		break;
	}


	pDC->SetTextColor(m_crXLegendTextColorDate);
	pDC->SelectObject(&penSolid);
	
	

	//do the Y grid lines
	//note Y gridlines run left - right
	GridSpacing=CalcYAxisGridAndTicks();
	int n=1; //for the loop
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

	pDC->SelectObject(poldpen);
	pDC->SelectObject(hPrevFont);

	penDash.DeleteObject();
	penSolid.DeleteObject();
	//DeleteObject(brush);
}

////////////////////// DrawTicks ///////////////////////////////////////////
// **************************************************************************
// DrawTicks does two things - it draws the x & Y axis lines and it also
//				draws the little 'tick' lines.
//				The axis lines are always shown but the 'ticks' are controlled by the
//				m_bShowTicks member (using the ShowTicks function);
//				The length of the ticks are set by the #define in the header file
//				the axis lines and ticks use the same color (m_crXTickColor for
//				the x-axis and m_crYTickColor for the y-axis)
// **************************************************************************
void CTrendDiagramm::DrawTicks(CDC* pDC)
{
	//pretty much the same as showing the grid because the ticks and
	//tick align
	if (!m_bShowTicks)
	{
		return;
	}
	CPen xTickPen,yTickPen, *poldpen;

	CRect dataarea=CalcDataArea();
	UINT GraphBottom=m_iGraphY+m_iGraphHeight;

	//start with the x-axis
	xTickPen.CreatePen(PS_SOLID,1,m_crXTickColor);
	poldpen=pDC->SelectObject(&xTickPen);


	double GridSpacing=CalcXAxisGridAndTicks();
	//the ticklines vertically straddle the x-axis
	//two problems though - if the x-line is at or very close to the
	//top or bottom of the dataarea
	UINT xtoptick=((GraphBottom-m_iOriginY)-dataarea.top < G_TICKLENGTH/2)?(GraphBottom-m_iOriginY)-dataarea.top:G_TICKLENGTH/2;
	UINT xbottick=(dataarea.bottom-(GraphBottom-m_iOriginY) <G_TICKLENGTH/2)?dataarea.bottom-(GraphBottom-m_iOriginY):G_TICKLENGTH/2;
	////draw positive xticks
	//UINT Graphright=m_iGraphX+m_iGraphWidth;
	//int iXpixelsRight=Graphright-m_rmargin-m_iOriginX;

	//while(GridSpacing*n<iXpixelsRight)
	//{
	//	//loop and do the ticks
	//	//topticks
	//	pDC->MoveTo(m_iOriginX+GridSpacing*n,GraphBottom-m_iOriginY);
	//	pDC->LineTo(m_iOriginX+GridSpacing*n,GraphBottom-m_iOriginY-xtoptick);
	//	//bottom ticks
	//	pDC->LineTo(m_iOriginX+GridSpacing*n,GraphBottom-m_iOriginY+xbottick);
	//	n++;
	//}

	int iXpixels=dataarea.right-dataarea.left;
	/*while(GridSpacing*n<iXpixelsRight)
	{
	pDC->MoveTo(m_iOriginX+GridSpacing*n , dataarea.top);
	pDC->LineTo(m_iOriginX+GridSpacing*n,dataarea.bottom);
	n++;
	}*/
	int n=iXpixels/GridSpacing;
	int i=1;
	while(i<=n)
	{
		//loop and do the ticks
		//topticks
		//pDC->MoveTo(dataarea.right-i*GridSpacing , GraphBottom-m_iOriginY);
		pDC->MoveTo(dataarea.right-i*GridSpacing,GraphBottom-m_iOriginY-xtoptick);
		//pDC->LineTo(dataarea.right-i*GridSpacing,GraphBottom-m_iOriginY-xtoptick);
		//bottom ticks
		pDC->LineTo(dataarea.right-i*GridSpacing,GraphBottom-m_iOriginY+xbottick);
		i++;
	}




	////draw negative xticks
	//int iXpixelsLeft=m_iOriginX-m_rmargin;
	//n=1;
	//while(GridSpacing*n<iXpixelsLeft)
	//{
	//	pDC->MoveTo(m_iOriginX-GridSpacing*n,GraphBottom-m_iOriginY);
	//	pDC->LineTo(m_iOriginX-GridSpacing*n,GraphBottom-m_iOriginY-xtoptick);
	//	//bottom ticks
	//	pDC->LineTo(m_iOriginX-GridSpacing*n,GraphBottom-m_iOriginY+xbottick);
	//	n++;
	//}



	//now do the y axis ticks	
	yTickPen.CreatePen(PS_SOLID,1,m_crYTickColor);
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

	int x=0;
	if(m_bYLineAtLeft)
	{
		x=dataarea.left;
	}
	else
	{
		x=m_iGraphX+m_iOriginX;
	}

	n=1; //for the loop
	//draw positive yticks
	UINT Graphbottom=m_iGraphY+m_iGraphHeight;
	int iYpixelsTop=Graphbottom-m_tmargin-m_iOriginY;
	while(GridSpacing*n<iYpixelsTop)
	{
		pDC->MoveTo(x,Graphbottom-m_iOriginY-(n*GridSpacing));
		//do left side tick
		pDC->LineTo(x-ylefttick,Graphbottom-m_iOriginY-(n*GridSpacing));
		//do rightside tick
		pDC->LineTo(x+yrighttick,Graphbottom-m_iOriginY-(n*GridSpacing));
		n++;
	}
	//draw negative yticks
	int iYpixelsBottom=m_iOriginY-m_bmargin;
	n=1;
	while(GridSpacing*n<iYpixelsBottom)
	{
		pDC->MoveTo(x,Graphbottom-m_iOriginY+(n*GridSpacing));
		//do left side tick
		pDC->LineTo(x-ylefttick,Graphbottom-m_iOriginY+(n*GridSpacing));
		//do rightside tick
		pDC->LineTo(x+yrighttick,Graphbottom-m_iOriginY+(n*GridSpacing));
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
void CTrendDiagramm::DrawTrendCursor(bool bClear)
{
	CClientDC dc(this);

	CPen cursorpen,*poldpen;
	cursorpen.CreatePen(PS_SOLID,2,RGB(60,60,120));
	HBRUSH holdbrush;
	poldpen=dc.SelectObject(&cursorpen);
	//poldbrush=pDCGraph->SelectObject(&brush);
	holdbrush=(HBRUSH)dc.SelectObject((HBRUSH)GetStockObject(NULL_BRUSH));

	int iPos = m_lmargin+TREND_DATAAREA-m_iCurTrendCursorPos;//m_iGraphWidth-m_rmargin-m_lmargin
	int iPosRectLeft=iPos;
	
	if(bClear)
	{
		BitBlt(dc.m_hDC, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcGraph, 0, 0, SRCCOPY);
		if(iPos<m_lmargin+50)
			iPosRectLeft=m_lmargin;
		else
			iPosRectLeft=iPos-50;
	}
	else if(iPos<m_lmargin+50)
	{
		BitBlt(dc.m_hDC, m_lmargin, 0, 80,m_iGraphHeight, m_hdcGraph, m_lmargin, 0, SRCCOPY);
		iPosRectLeft=m_lmargin;
	}
	else
	{
		BitBlt(dc.m_hDC, iPos-70, 0, 80,m_iGraphHeight, m_hdcGraph, iPos-70, 0, SRCCOPY);
		iPosRectLeft=iPos-50;
	}
	

	MoveToEx(dc.m_hDC, iPos, m_iGraphHeight, NULL);
	LineTo(dc.m_hDC, iPos, 0);

	HFONT hPrevFont=(HFONT)SelectObject(dc.m_hDC,g_hf7AcuBold);
	int nPrevTxtColor=SetTextColor(dc.m_hDC,RGB(0,0,0));
	RECT rc;
	
	dc.SelectObject((HBRUSH)GetStockObject(WHITE_BRUSH));
	rc.top = 15;
	rc.bottom = 35;
	rc.left = iPosRectLeft;
	rc.right = rc.left+50;
	Rectangle(dc.m_hDC,rc.left,rc.top,rc.right,rc.bottom);
	rc.left = iPosRectLeft;
	rc.right = rc.left+47;
	TCHAR psz[MAX_PATH];
	int iVal=GetTrendCursorValue();

	if(m_iTrendType==TREND_MV)
	{
		if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
			wsprintf(psz,_T("%0.2f"), CTlsFloat::Round(((double)iVal)/1000, 2));
		else
			wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)iVal)/1000, 1));
	}
	else if(m_iTrendType==TREND_VTE || m_iTrendType==TREND_PMEAN || m_iTrendType==TREND_RSBI)
	{
		wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)iVal)/10, 1));
	}
	else if(m_iTrendType==TREND_COMPLIANCE)
	{
		wsprintf(psz,_T("%0.2f"),CTlsFloat::Round(((double)iVal)/100, 2));
	}
	else if(m_iTrendType==TREND_RESISTANCE || m_iTrendType==TREND_FIO2)//newtrend
	{
		wsprintf(psz,_T("%0.0f"),CTlsFloat::Round(((double)iVal)/10, 0));
	}
	/*else if(m_iTrendType==TREND_SPO2)
	{
		wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)iVal)/10, 1));
	}*/
	else if(m_iTrendType==TREND_SPO2PI)
	{
		wsprintf(psz,_T("%0.1f"),CTlsFloat::Round(((double)iVal)/10, 1));
	}
	else
		wsprintf(psz,_T("%d"), iVal);

	DrawText(dc.m_hDC,psz,-1,&rc,DT_VCENTER|DT_SINGLELINE|DT_RIGHT);
	SelectObject(dc.m_hDC,hPrevFont);
	SetTextColor(dc.m_hDC,nPrevTxtColor);

	dc.SelectObject(poldpen);
	dc.SelectObject(holdbrush);
	cursorpen.DeleteObject();
}
void CTrendDiagramm::SetTrendCursorPos(int iCurTrendCursorPos, int iPosbufferX)
{
	//DEBUGMSG(TRUE, (TEXT("SetTrendCursorPos %d %d\r\n"),iCurTrendCursorPos,iPosbufferX));

	if(iCurTrendCursorPos==TREND_LEFTRANGE && iPosbufferX==0)
	{
		m_iPosbufferX=m_iCountbuffer-1;
		m_iCurTrendCursorPos=bufferPosValues[m_iPosbufferX].x;
	}
	else
	{
		m_iCurTrendCursorPos=iCurTrendCursorPos;
		m_iPosbufferX=iPosbufferX;
	}
	

	DrawTrendCursor(true);
}

int CTrendDiagramm::GetTrendCursorPos()
{
	return m_iCurTrendCursorPos;
}
int CTrendDiagramm::GetTrendBufferXPos()
{
	return m_iPosbufferX;
}

int CTrendDiagramm::GetTrendCursorValue()
{
	WORD val=bufferPosValues[m_iPosbufferX].y;
	
	return val;
}

int CTrendDiagramm::MoveTrendCursorLeft(bool bFast)
{
	
	if(bFast)
	{
		if(m_iPosbufferX<m_iCountbuffer-5)
		{
			if(m_lmargin+TREND_DATAAREA-bufferPosValues[m_iPosbufferX+5].x>m_lmargin)
			{
				m_iPosbufferX=m_iPosbufferX+5;
				m_iCurTrendCursorPos=bufferPosValues[m_iPosbufferX].x;
				DrawTrendCursor(false);
			}
		}
		else
		{
			if(GetParent())
				GetParent()->PostMessage(WM_TREND_LEFTRANGE);
		}
	}
	else
	{
		/*int iTest1=m_iPosbufferX;
		int iTest2=bufferPosValues[m_iPosbufferX].x;
		int iTest3=bufferPosValues[m_iPosbufferX+1].x;
		int iTest4=m_iCurTrendCursorPos;
		int iTest5=m_lmargin+TREND_DATAAREA-bufferPosValues[m_iPosbufferX+1].x;
		int iTest6=m_lmargin;*/
		if(m_iPosbufferX<m_iCountbuffer-1)
		{
			if(m_lmargin+TREND_DATAAREA-bufferPosValues[m_iPosbufferX+1].x>m_lmargin)
			{
				m_iPosbufferX++;
				m_iCurTrendCursorPos=bufferPosValues[m_iPosbufferX].x;
				DrawTrendCursor(false);
			}
		}
		else
		{
			if(GetParent())
				GetParent()->PostMessage(WM_TREND_LEFTRANGE);
		}
		
	}
	
	/*DrawTrendCursor();*/
	return GetTrendCursorValue();
}

int CTrendDiagramm::MoveTrendCursorRight(bool bFast)
{
	if(bFast)
	{
		if(m_iPosbufferX>4)
		{
			if(bufferPosValues[m_iPosbufferX-5].x>=0)
			{
				m_iPosbufferX=m_iPosbufferX-5;
				m_iCurTrendCursorPos=bufferPosValues[m_iPosbufferX].x;
				DrawTrendCursor(false);
			}
		}
		else
		{
			if(GetParent())
				GetParent()->PostMessage(WM_TREND_RIGHTRANGE,m_nID);
		}
	}
	else
	{
		/*int iTest1=m_iPosbufferX;
		int iTest2=bufferPosValues[m_iPosbufferX].x;
		int iTest3=0;
		if(m_iPosbufferX>0)
			iTest3=bufferPosValues[m_iPosbufferX-1].x;
		int iTest4=m_iCurTrendCursorPos;*/

		if(m_iPosbufferX>0)
		{
			if(bufferPosValues[m_iPosbufferX-1].x>=0)
			{
				m_iPosbufferX--;
				m_iCurTrendCursorPos=bufferPosValues[m_iPosbufferX].x;
				DrawTrendCursor(false);
			}
		}
		else
		{
			if(GetParent())
				GetParent()->PostMessage(WM_TREND_RIGHTRANGE,m_nID);
		}
	}
	
	/*DrawTrendCursor();*/
	return GetTrendCursorValue();
}

// **************************************************************************
// 
// **************************************************************************
void CTrendDiagramm::DrawCursor(bool state)
{
	if(!m_bAutofit)
	{
		if(state)
		{
			m_pcGraphDecrease->ShowWindow(SW_SHOW);
			m_pcGraphIncrease->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pcGraphDecrease->ShowWindow(SW_HIDE);
			m_pcGraphIncrease->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		//CDC* pDCGraph=CDC::FromHandle(m_hdcGraph);
		//CClientDC dc(this);

		//CBrush cbrBack(BACKGND);
		//HBRUSH hbrprev=(HBRUSH)SelectObject(m_hdcGraph,cbrBack);
		//HPEN hpenprev=(HPEN)SelectObject(m_hdcGraph, (HPEN)GetStockObject(NULL_PEN));

		//if(state)
		//	m_pcCursor->Draw(m_hdcGraph,TREND_WIDTH-30,60);
		//else
		//	Rectangle(m_hdcGraph, TREND_WIDTH-30, 60, TREND_WIDTH, 90);

		//BitBlt(dc.m_hDC, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcGraph, 0, 0, SRCCOPY);

		//DrawTrendCursor(false);

		//SelectObject(m_hdcGraph,hbrprev);
		//SelectObject(m_hdcGraph,hpenprev);
		////DeleteObject(cbrBack);

		CClientDC dc(this);

		//CBrush cbrBack(BACKGND);
		CBrush cbrBack(m_crGraphBkColor);
		//CBrush cbrBack(LIGHTGREEN);
		HBRUSH hbrprevClient=(HBRUSH)SelectObject(dc.m_hDC,cbrBack);
		HPEN hpenprevClient=(HPEN)SelectObject(dc.m_hDC, (HPEN)GetStockObject(NULL_PEN));

		if(state)
		{
			m_pcCursor->Draw(dc.m_hDC,TREND_WIDTH-31,60);
		}
		else
		{
			Rectangle(dc.m_hDC, TREND_WIDTH-31, 60, TREND_WIDTH-1, 90);
		}

		//if(state)
		//	m_pcCursor->Draw(m_hdcGraph,TREND_WIDTH-30,60);
		//else
		//	Rectangle(m_hdcGraph, TREND_WIDTH-30, 60, TREND_WIDTH, 90);

		SelectObject(dc.m_hDC,hbrprevClient);
		SelectObject(dc.m_hDC,hpenprevClient);
	}
	
	
}


///////////// CalcDataArea ////////////////////////////////////////////////////
// **************************************************************************
// This function calculates the actual plotting area of the graph
//				this is the graph area minus the top,bottom,left & right margins
//				Returns:
//				CRect with the plotting area (in client area co-ords)
// **************************************************************************
CRect CTrendDiagramm::CalcDataArea()
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
//int CTrendDiagramm::GetRightPixelsOfXAxis()
//{
//	UINT Graphright=m_iGraphX+m_iGraphWidth;
//	int iXpixelsRight=Graphright-m_rmargin-m_iOriginX;
//
//	return iXpixelsRight;
//}

// **************************************************************************
// This function calculates the actual plotting area of the graph
//				this is the graph area minus the top,bottom,left & right margins
//				Returns:
//				CRect with the plotting area (in client area co-ords)
// **************************************************************************
int CTrendDiagramm::GetDataAreaWidth()
{
	int iWidth = m_iGraphWidth-m_rmargin-m_lmargin;

	return iWidth;
}

// **************************************************************************
// This function calculates the actual plotting area of the graph
//				this is the graph area minus the top,bottom,left & right margins
//				Returns:
//				CRect with the plotting area (in client area co-ords)
// **************************************************************************
int CTrendDiagramm::GetDataAreaHight()
{
	int iHight = m_iGraphHeight-m_bmargin-m_tmargin;

	return iHight;
}


///////////////////// CalcXAxisGridAndTicks ////////////////////////////////
// **************************************************************************
//  This calculates the positions of the vertical gridlines of the plot area.
//				This is also used for x-axis ticks as the ticks line up with
//				the gridlines.
//				How many they are is determined by the G_NUMTICKSANDGRID  define
//				in the header file
//				Return:
//				a double denoting the x-asis grid spacing
// **************************************************************************
double CTrendDiagramm::CalcXAxisGridAndTicks()
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

	//DEBUGMSG(TRUE, (TEXT("CalcXAxisGridAndTicks ID %d result %d\r\n"),m_nID,result));

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
double CTrendDiagramm::CalcXAxisNumerics()
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

	DEBUGMSG(TRUE, (TEXT("CalcXAxisNumerics ID %d result %d\r\n"),m_nID,result));

	return result;

}


// **************************************************************************
// 
// **************************************************************************
double CTrendDiagramm::CalculateYAxisTickGridSpace()
{
	double result =0;

	switch(m_iTrendType)
	{
	case TREND_PINSP:
	case TREND_PMEAN:
	case TREND_FIO2:
	case TREND_VTE:
	case TREND_COMPLIANCE:
	case TREND_CO2HFO:
	case TREND_MV:
	case TREND_HFAMP:
	case TREND_RSBI:
	case TREND_SHAREMVMAND:
	case TREND_RESISTANCE:
	case TREND_LEAK:
	case TREND_SPO2:
	case TREND_SPO2PR:
	case TREND_SPO2PI:
	case TREND_ETCO2:
	case TREND_FREQUENCY:
	default:
		{
			if(m_dYAxisMax<10)
				result=1;
			else if(m_dYAxisMax<20)
				result=2;
			else if(m_dYAxisMax<30)
				result=5;
			else if(m_dYAxisMax<40)
				result=5;
			else if(m_dYAxisMax<50)
				result=5;
			else if(m_dYAxisMax<60)
				result=10;
			/*else if(m_dYAxisMax<80)
				result=10;*/
			else if(m_dYAxisMax<=100)
				result=20;
			/*else if(m_dYAxisMax<200)
				result=20;*/
			else if(m_dYAxisMax<500)
				result=50;
			else if(m_dYAxisMax<1000)
				result=100;
			else if(m_dYAxisMax<2000)
				result=250;
			else if(m_dYAxisMax<5000)
				result=500;
			else 
				result=1000;

			
		}
		break;
	}

	return result;
}
// **************************************************************************
// 
// **************************************************************************
double CTrendDiagramm::CalculateYAxisNumericSpace()
{
	double result =0;

	switch(m_iTrendType)
	{
	case TREND_PINSP:
	case TREND_PMEAN:
	case TREND_FIO2:
	case TREND_VTE:
	case TREND_COMPLIANCE:
	case TREND_CO2HFO:
	case TREND_MV:
	case TREND_HFAMP:
	case TREND_RSBI:
	case TREND_SHAREMVMAND:
	case TREND_RESISTANCE:
	case TREND_LEAK:
	case TREND_SPO2:
	case TREND_SPO2PR:
	case TREND_SPO2PI:
	case TREND_ETCO2:
	case TREND_FREQUENCY:
	default:
		{
			if(m_dYAxisMax<10)
				result=1;
			else if(m_dYAxisMax<20)
				result=2;
			else if(m_dYAxisMax<40)
				result=5;
			else if(m_dYAxisMax<60)
				result=10;
			/*else if(m_dYAxisMax<80)
				result=20;*/
			else if(m_dYAxisMax<=100)
				result=20;
			/*else if(m_dYAxisMax<200)
				result=20;*/
			else if(m_dYAxisMax<400)
				result=50;
			else if(m_dYAxisMax<600)
				result=100;
			else if(m_dYAxisMax<800)
				result=200;
			else if(m_dYAxisMax<1000)
				result=200;
			else if(m_dYAxisMax<2000)
				result=250;
			else if(m_dYAxisMax<4000)
				result=500;
			else if(m_dYAxisMax<6000)
				result=1000;
			else 
				result=2000;
		}
		break;
	}

	return result;
}
// **************************************************************************
// 
// **************************************************************************
void CTrendDiagramm::SetXAxisTickSpace()
{
	int iTimeSpan=(int)m_dXAxisMax;

	switch(iTimeSpan)
	{
	/*case TREND_20DAYS:
		{
			m_dXTickSpace=TREND_TICKMIN_20DAYS;
		}
		break;*/
	/*case TREND_10DAYS:
		{
			m_dXTickSpace=TREND_TICKMIN_10DAYS;
		}
		break;*/
	case TREND_5DAYS:
		{
			m_dXTickSpace=TREND_TICKMIN_5DAYS;
		}
		break;
	case TREND_3DAYS:
		{
			m_dXTickSpace=TREND_TICKMIN_3DAYS;
		}
		break;
	case TREND_1DAY4HOURS:
		{
			m_dXTickSpace=TREND_TICKMIN_1DAY4HOURS;
		}
		break;
	case TREND_16HOURS:
		{
			m_dXTickSpace=TREND_TICKMIN_16HOURS;
		}
		break;
	case TREND_8HOURS:
		{
			m_dXTickSpace=TREND_TICKMIN_8HOURS;
		}
		break;
	case TREND_4HOURS:
		{
			m_dXTickSpace=TREND_TICKMIN_4HOURS;	
		}
		break;
	case TREND_2HOURS:
		{
			m_dXTickSpace=TREND_TICKMIN_2HOURS;	
		}
		break;
	case TREND_1HOUR:
		{
			m_dXTickSpace=TREND_TICKMIN_1HOUR;	
		}
		break;
	case TREND_30MIN:
		{
			m_dXTickSpace=TREND_TICKMIN_30MIN;	
		}
		break;
	default:
		break;
	}

	//DEBUGMSG(TRUE, (TEXT("SetXAxisTickSpace ID %d m_dXTickSpace %d\r\n"),m_nID,m_dXTickSpace));
	//m_dXTickSpace=dXTickSpace;
}

// **************************************************************************
// 
// **************************************************************************
double CTrendDiagramm::GetXAxisTickSpace()
{
	return m_dXTickSpace;
}
// **************************************************************************
// 
// **************************************************************************
void CTrendDiagramm::SetXAxisNumericSpace(double dXNumericSpace)
{
	DEBUGMSG(TRUE, (TEXT("SetXAxisNumericSpace ID %d dXNumericSpace %d\r\n"),m_nID,dXNumericSpace));
	m_dXNumericSpace=dXNumericSpace;
}

// **************************************************************************
// 
// **************************************************************************
double CTrendDiagramm::GetXAxisNumericSpace()
{
	return m_dXNumericSpace;
}
// **************************************************************************
// 
// **************************************************************************
void CTrendDiagramm::SetYAxisTickSpace(double dYTickSpace)
{
	m_dYTickSpace=dYTickSpace;
}

// **************************************************************************
// 
// **************************************************************************
double CTrendDiagramm::GetYAxisTickSpace()
{
	return m_dYTickSpace;
}
// **************************************************************************
// 
// **************************************************************************
void CTrendDiagramm::SetYAxisNumericSpace(double dYNumericSpace)
{
	m_dYNumericSpace=dYNumericSpace;
}

// **************************************************************************
// 
// **************************************************************************
double CTrendDiagramm::GetYAxisNumericSpace()
{
	return m_dYNumericSpace;
}




///////////////////  CalcYAxisGridAndTicks  //////////////////////////////////
// **************************************************************************
// This calculates the positions of the horizontal gridlines of the plot area.
//				This is also used for y-axis ticks as the ticks line up with
//				the gridlines.
//				How many they are is determined by the G_NUMTICKSANDGRID  define
//				in the header file
//				Return:
//				A double denoting the howizontal gridline spacing
// **************************************************************************
double CTrendDiagramm::CalcYAxisGridAndTicks()
{
	//the placing of the ticks co-incide with gridlines
	CRect rect=CalcDataArea();

	double result=0;

	if(m_dYTickSpace==0)
	{
		result=((double)rect.bottom-(double)rect.top)/(double)G_NUMTICKSANDGRID;
	}
	else
	{
		if(m_dYAxisMax<(m_dYAxisMin*(-1)))//sollte zur Zeit nicht vorkommen
		{
			result=(m_iOriginY/(m_dYAxisMin*(-1)))*m_dYTickSpace;
		}
		else
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
double CTrendDiagramm::CalcYAxisNumerics()
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
		if(m_dYAxisMax<(m_dYAxisMin*(-1)))//sollte zur Zeit nicht vorkommen
		{
			result=(m_iOriginY/(m_dYAxisMin*(-1)))*m_dYNumericSpace;
		}
		else
		{
			UINT Graphbottom=m_iGraphY+m_iGraphHeight;
			int iYpixelsRight=Graphbottom-m_tmargin-m_iOriginY;

			result=(iYpixelsRight/m_dYAxisMax)*m_dYNumericSpace;
		}
	}

	return result;
}





///////////////////// DrawGraphTitle ////////////////////////////////////
/*
This draws the Graph title string in the color m_crGraphTitleColor
This title is centred abobe the plotting area.
*/
////////////////////////////////////////////////////////////////////////
//void CTrendDiagramm::DrawGraphTitle(CDC* pDC)
//{
//	//The graph title is drawn one character line down
//	//centered left right between the left and right margins
//	UINT lmargin=CalcLeftMargin();
//	UINT rmargin=CalcRightMargin();
//	CRect rect;
//	rect.left=m_iGraphX+lmargin;
//	rect.top=m_iGraphY+m_iCharHeight;
//	rect.right=m_iGraphX+m_iGraphWidth-rmargin;
//	rect.bottom=rect.top+m_iCharHeight;
//
//	//draw the title using the specified colorscheme
//	//using the graph font
//	CDC *pdc=m_pWnd->GetDC();
//	CFont *poldfont;
//
//	//note we must clear of any old stuff crap fom this area
//	CBrush brush;
//	brush.CreateSolidBrush(m_crGraphBkColor);
//	//or bottom of a given rect
//	pdc->FillRect(&rect,&brush);
//
//	pdc->SetBkMode(TRANSPARENT);
//	pdc->SetTextColor(m_crGraphTitleColor);
//	poldfont=pdc->SelectObject(&m_GraphFont);
//	pdc->DrawText(m_szGraphTitle,&rect,DT_CENTER|DT_END_ELLIPSIS);
//
//	//cleanup
//	pdc->SelectObject(poldfont);
//	m_pWnd->ReleaseDC(pdc);
//}

///////////////////// DrawXLegend //////////////////////////////////////////////
// **************************************************************************
// The Xaxis legend is drawn below the plotting area
//				below the x-axis scale numbers
// **************************************************************************
void CTrendDiagramm::DrawXLegend(CDC* pDC)
{
	//The x legend is drawn  below the data area
	CRect rect,dataarea;

	dataarea=CalcDataArea();

	rect.left=dataarea.right+1;
	rect.right=dataarea.right+6*m_iCharWidth+1;
	rect.top=m_iGraphY+m_iGraphHeight-1*m_iCharHeight;
	rect.bottom=rect.top+m_iCharHeight;


	//note we must clear of any old stuff crap fom this area
	CBrush brush;
	brush.CreateSolidBrush(m_crGraphBkColor);
	//brush.CreateSolidBrush(m_crGraphPenColor);	//for test rku
	rect.InflateRect(0,0,1,1);//bcause fillrect does not go right to the right
	//or bottom of a given rect
	pDC->FillRect(&rect,&brush);
	rect.DeflateRect(0,0,1,1);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_crXLegendTextColorDate);
	HFONT hPrevFont=(HFONT)pDC->SelectObject(g_hf7AcuNorm);

	pDC->DrawText(m_szXLegendText,&rect,DT_CENTER|DT_END_ELLIPSIS);

	//cleanup
	pDC->SelectObject(hPrevFont);

	//DeleteObject(brush);
}

//////////////////////// DrawYlegend //////////////////////////////////////////
// **************************************************************************
// The Y legend is drawn at the LHS of the plot area between the yaxis max 
//				and yaxis min scale
// **************************************************************************
void CTrendDiagramm::DrawYLegend(CDC* pDC)
{
	//The Y legend is drawn down the left hand side just
	//outside the data area
	CRect rect,dataarea;

	dataarea=CalcDataArea();

	/*rect.left=m_iOriginX-8*m_iCharWidth;;
	rect.right=rect.left+16*m_iCharWidth;
	rect.top=dataarea.top-m_iCharHeight-1;
	rect.bottom=dataarea.top-1;*/

	rect.left=m_iOriginX+1*m_iCharWidth;
	rect.right=rect.left+10*m_iCharWidth;
	//rect.top=dataarea.top-m_iCharHeight-1;
	rect.top=1;
	rect.bottom=dataarea.top-1;



	//note we must clear of any old stuff crap fom this area
	CBrush brush;
	brush.CreateSolidBrush(m_crGraphBkColor);
	//brush.CreateSolidBrush(m_crGraphPenColor);	//for test rku
	//or bottom of a given rect
	pDC->FillRect(&rect,&brush);


	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_crYLegendTextColor);
	HFONT hPrevFont=(HFONT)pDC->SelectObject(g_hf7AcuNorm);
	pDC->DrawText(m_szYLegendText,&rect,DT_LEFT|DT_END_ELLIPSIS);


	//cleanup
	pDC->SelectObject(hPrevFont);

	//DeleteObject(brush);
}

///////////////////// DrawXAxisNumbers //////////////////////////////////////
// **************************************************************************
// The X axis scale numbers are drawm directly below the plot area.
//				Three numbers are drawn, min, middle and max scaling
// **************************************************************************
void CTrendDiagramm::DrawXAxisNumbers(CDC* pDC)
{
	CStringW astring;
	CRect rect;
	HFONT hPrevFont=(HFONT)pDC->SelectObject(g_hf14AcuNormNum);
	pDC->SetTextColor(m_crXLegendTextColorDate);
	pDC->SetBkMode(TRANSPARENT);

	CBrush brush;
	brush.CreateSolidBrush(m_crGraphBkColor);
	//brush.CreateSolidBrush(m_crXLegendTextColor);

	//int iTimeSpan=(int)m_dXAxisMax;

	CRect dataarea=CalcDataArea();//where the graph data is actually drawn
	
	int XAxisTickSpace=(int)GetXAxisTickSpace();

	//the ticklines vertically straddle the x-axis
	//two problems though - if the x-line is at or very close to the
	//top or bottom of the dataarea
	//int n=0;
	//UINT Graphright=m_iGraphX+m_iGraphWidth;

	//int iXpixels=dataarea.right-dataarea.left;
	//n=iXpixels/GridSpacing;

	//int i=1;
	//int i=0;

	switch(XAxisTickSpace)
	{
	//case TREND_TICKMIN_20DAYS:
	//	{
	//		//CStringW szDate = _T("");
	//		////COleDateTime dtStartDate=COleDateTime::GetCurrentTime();
	//		////szDate = dtDate.Format(_T("%y%m%d%H%M%S"));
	//		//szDate = dtStartDate.Format(_T("%m%d%H%M%S"));
	//		////COleDateTimeSpan ts;

	//		//while(i<=n)
	//		//{
	//		//	//format and print the number
	//		//	//astring.Format(_T("%.4g"),m_dXAxisMin);
	//		//	rect.left=dataarea.right-i*GridSpacing-3*m_iCharWidth;
	//		//	rect.top=m_iGraphY+m_iGraphHeight-1*m_iCharHeight;
	//		//	rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters
	//		//	rect.bottom=rect.top+m_iCharHeight;
	//		//	rect.top+=1;
	//		//	rect.bottom+=1;

	//		//	pDC->FillRect(&rect,&brush);

	//		//	/*if(XAxisTickSpace*n/1000<1.0)
	//		//		astring.Format(_T("%.1g"),XAxisTickSpace*n/1000);
	//		//	else
	//		//		astring.Format(_T("%.2g"),XAxisTickSpace*n/1000);*/

	//		//	astring.Format(_T("%d"),i);

	//		//	pDC->DrawText(astring,&rect,DT_NOCLIP|DT_CENTER);
	//		//	i++;
	//		//}
	//	}
	//	break;
	/*case TREND_TICKMIN_10DAYS:
		{

		}
		break;*/
	case TREND_TICKMIN_5DAYS:
	//case TREND_TICKMIN_3DAYS:
		{

		}
		break;
	case TREND_TICKMIN_1DAY4HOURS:
		{

		}
		break;
	case TREND_TICKMIN_16HOURS:
		{

		}
		break;
	case TREND_TICKMIN_8HOURS:
		{

		}
		break;
	case TREND_TICKMIN_4HOURS:
		{

		}
		break;
	case TREND_TICKMIN_2HOURS:
		{

		}
		break;
	case TREND_TICKMIN_1HOUR:
		{

		}
		break;
	case TREND_TICKMIN_30MIN:
		{

		}
		break;
	default:
		break;
	}



	////draw negative xticks
	//int iXpixelsLeft=m_iOriginX-m_rmargin;
	//n=1;
	//while(GridSpacing*n<iXpixelsLeft)
	//{
	//	rect.left=m_iOriginX-GridSpacing*n-3*m_iCharWidth;
	//	rect.top=m_iGraphY+m_iGraphHeight-1*m_iCharHeight;
	//	rect.right=rect.left+6*m_iCharWidth;//allow for 6 characters
	//	rect.bottom=rect.top+m_iCharHeight;
	//	rect.top+=1;
	//	rect.bottom+=1;

	//	pDC->FillRect(&rect,&brush);
	//	//format and print the number
	//	//astring.Format(_T("%.4g"),m_dXAxisMin);
	//	/*if(XAxisTickSpace*n/1000<1.0)
	//	astring.Format(_T("%.1g"),XAxisTickSpace*n/1000);
	//	else*/
	//	astring.Format(_T("%.2g"),(XAxisTickSpace*n/1000)*(-1));
	//	pDC->DrawText(astring,&rect,DT_NOCLIP|DT_CENTER);

	//	n++;
	//}

	//cleanup
	pDC->SelectObject(hPrevFont);

	//DeleteObject(brush);
}

///////////////////// DrawYAxisNumbers //////////////////////////////////////
// **************************************************************************
// The Y axis scale is drawn on the LHS of the plot area.
//				Only two numbers are drawn - min and max to allow for the Y-axis
//				legend.
// **************************************************************************
void CTrendDiagramm::DrawYAxisNumbers(CDC* pDC)
{
	CStringW astring;
	CRect rect;
	HFONT hPrevFont=(HFONT)pDC->SelectObject(g_hf5AcuNorm);
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

	

	while((GridSpacing*n+(m_iCharHeight/2))<iYpixelsTop)
	{
		////loop and do the ticks

		//do the left side (min) left aligned

		//format and print the number
		rect.left=m_iGraphX+1;
		rect.top=Graphbottom-m_iOriginY-GridSpacing*n-m_iCharHeight/2;
		rect.right=m_lmargin-2;//
		rect.bottom=rect.top+m_iCharHeight;

		pDC->FillRect(&rect,&brush);

		if(m_iTrendType==TREND_MV)
		{
			astring.Format(_T("%.2f"),CTlsFloat::Round(((double)(YAxisTickSpace*n))/1000, 2));
		}
		/*else if(m_iTrendType==TREND_SPO2)
		{
			astring.Format(_T("%.1f"),CTlsFloat::Round(((double)(YAxisTickSpace*n))/100, 1));
		}*/
		else if(m_iTrendType==TREND_SPO2PI)
		{
			if(YAxisTickSpace*n<1.0)
			{
				astring.Format(_T("%.2g"),CTlsFloat::Round(((double)(YAxisTickSpace*n))/10, 0));
			}
			else
			{
				astring.Format(_T("%.4g"),CTlsFloat::Round(((double)(YAxisTickSpace*n))/10, 0));
			}
		}
		else if(m_iTrendType==TREND_VTE || m_iTrendType==TREND_PMEAN || m_iTrendType==TREND_RSBI)
		{
			if(YAxisTickSpace*n<1.0)
			{
				astring.Format(_T("%.2g"),CTlsFloat::Round(((double)(YAxisTickSpace*n))/10, 0));
			}
			else
			{
				astring.Format(_T("%.4g"),CTlsFloat::Round(((double)(YAxisTickSpace*n))/10, 0));
			}
		}
		else if(m_iTrendType==TREND_RESISTANCE)
		{
			if(YAxisTickSpace*n<1.0)
			{
				astring.Format(_T("%.2g"),CTlsFloat::Round(((double)(YAxisTickSpace*n))/10, 2));
			}
			else if(YAxisTickSpace*n<10)//newtrend
			{
				astring.Format(_T("%.2g"),CTlsFloat::Round(((double)(YAxisTickSpace*n))/10, 2));
			}
			else 
			{
				astring.Format(_T("%.4g"),CTlsFloat::Round(((double)(YAxisTickSpace*n))/10, 0));
			}
		}
		else if(m_iTrendType==TREND_COMPLIANCE)
		{
			if(YAxisTickSpace*n<1.0)
			{
				astring.Format(_T("%.2g"),CTlsFloat::Round(((double)(YAxisTickSpace*n))/100, 1));
			}
			else
			{
				astring.Format(_T("%.4g"),CTlsFloat::Round(((double)(YAxisTickSpace*n))/100, 1));
			}
		}
		else if(m_iTrendType==TREND_FIO2)//newtrend
		{
			if(YAxisTickSpace*n<1.0)
			{
				astring.Format(_T("%.2g"),CTlsFloat::Round(((double)(YAxisTickSpace*n))/10, 0));
			}
			else
			{
				astring.Format(_T("%.4g"),CTlsFloat::Round(((double)(YAxisTickSpace*n))/10, 0));
			}
		}
		else
		{
			if(YAxisTickSpace*n<1.0)
				astring.Format(_T("%.2g"),YAxisTickSpace*n);
			else
				astring.Format(_T("%.4g"),YAxisTickSpace*n);
		}

		pDC->DrawText(astring,&rect,DT_NOCLIP|DT_RIGHT);

		n++;
	}
	//draw negative xticks
	int iYpixelsBottom=m_iOriginY-m_bmargin;
	n=1;
	while(GridSpacing*n<iYpixelsBottom)
	{
		rect.left=m_iGraphX+1;
		rect.top=Graphbottom-m_iOriginY+GridSpacing*n-m_iCharHeight/2;
		rect.right=m_lmargin-2;//
		rect.bottom=rect.top+m_iCharHeight;

		pDC->FillRect(&rect,&brush);
		//format and print the number
		
		if(m_iTrendType==TREND_MV)//newtrend
		{
			astring.Format(_T("%.2f"),CTlsFloat::Round(((double)(YAxisTickSpace*n*(-1)))/1000, 2));
		}
		else if(m_iTrendType==TREND_COMPLIANCE)//newtrend
		{
			if(YAxisTickSpace*n<1.0)
			{
				astring.Format(_T("%.2g"),CTlsFloat::Round(((double)(YAxisTickSpace*n*(-1)))/100, 1));
			}
			else
			{
				astring.Format(_T("%.4g"),CTlsFloat::Round(((double)(YAxisTickSpace*n*(-1)))/100, 1));
			}
		}
		else if(m_iTrendType==TREND_FIO2)//newtrend
		{
			if(YAxisTickSpace*n<1.0)
			{
				astring.Format(_T("%.2g"),CTlsFloat::Round(((double)(YAxisTickSpace*n*(-1)))/10, 0));
			}
			else
			{
				astring.Format(_T("%.4g"),CTlsFloat::Round(((double)(YAxisTickSpace*n*(-1)))/10, 0));
			}
		}
		else if(m_iTrendType==TREND_RESISTANCE)//newtrend
		{
			if(YAxisTickSpace*n<1.0)
			{
				astring.Format(_T("%.2g"),CTlsFloat::Round(((double)(YAxisTickSpace*n*(-1)))/10, 2));
			}
			else if(YAxisTickSpace*n<10)//newtrend
			{
				astring.Format(_T("%.2g"),CTlsFloat::Round(((double)(YAxisTickSpace*n*(-1)))/10, 2));
			}
			else 
			{
				astring.Format(_T("%.4g"),CTlsFloat::Round(((double)(YAxisTickSpace*n*(-1)))/10, 0));
			}
		}
		else if(m_iTrendType==TREND_VTE || m_iTrendType==TREND_PMEAN || m_iTrendType==TREND_RSBI)//newtrend
		{
			if(YAxisTickSpace*n<1.0)
			{
				astring.Format(_T("%.2g"),CTlsFloat::Round(((double)(YAxisTickSpace*n*(-1)))/10, 0));
			}
			else
			{
				astring.Format(_T("%.4g"),CTlsFloat::Round(((double)(YAxisTickSpace*n*(-1)))/10, 0));
			}
		}
		else
		{
			if(YAxisTickSpace*n<1.0)
				astring.Format(_T("%.2g"),YAxisTickSpace*n*(-1));
			else
				astring.Format(_T("%.4g"),YAxisTickSpace*n*(-1));
		}
		
		pDC->DrawText(astring,&rect,DT_NOCLIP|DT_RIGHT);

		n++;
	}

	//cleanup
	pDC->SelectObject(hPrevFont);
}


////////////////// DrawFunctionName /////////////////////////////////////
/*
The function Name is drawn just above the plot area and below the Graph Title;
Usually the Function Name is autoset - but it's just another text string
which can be change programitcally.
*/
////////////////////////////////////////////////////////////////////////////
//void CTrendDiagramm::DrawFunctionName(CDC* pDC)
//{
//	//The function name is drawn just above the dataarea
//	//centered, using it's own color
//	CRect dataarea=CalcDataArea();
//	CRect rect;
//	rect.top=dataarea.top-m_iCharHeight;
//	rect.bottom=dataarea.top;
//	rect.left=dataarea.left;
//	rect.right=dataarea.right;
//
//	//move up a pixel so as not to get too close to the 
//	//dataarea top
//	rect.bottom-=2;
//	rect.top-=2;
//
//	CBrush brush;
//	brush.CreateSolidBrush(m_crGraphBkColor);
//	CFont *poldfont;
//
//	CDC *pdc=m_pWnd->GetDC();
//	pdc->FillRect(&rect,&brush);
//
//	poldfont=pdc->SelectObject(&m_GraphFont);
//	pdc->SetTextColor(m_crFunctionNameColor);
//	pdc->SetBkMode(TRANSPARENT);
//	pdc->DrawText(m_szFunctionNameText,&rect,DT_CENTER|DT_END_ELLIPSIS);
//
//	//cleanup
//	pdc->SelectObject(poldfont);
//	m_pWnd->ReleaseDC(pdc);
//}

////////////////// SetdefaultColorScheme  //////////////////////////////////
/*
sets the colours of various bits back to the default colours
*/
///////////////////////////////////////////////////////////////////////////
void CTrendDiagramm::SetDefaultColorScheme()
{
	SetColorScheme(G_DEFAULTSCHEME);
}


//////////////////////  SetDefaultGraphTitle //////////////////////////////
/*
Sets the default graph title
*/
///////////////////////////////////////////////////////////////////////////
void CTrendDiagramm::SetDefaultGraphTitle()
{
	m_szGraphTitle=_T("Graph Title goes here");
}


//////////// SetDefaultXLegend/////////////////////////////////////////////
/*

*/
///////////////////////////////////////////////////////////////////////////
void CTrendDiagramm::SetDefaultXLegend()
{
	m_szXLegendText=_T("X Axis Legend Text here");
}

///////////////// SetDefaultYLegend ///////////////////////////////////////
/*

*/
///////////////////////////////////////////////////////////////////////////
void CTrendDiagramm::SetDefaultYLegend()
{
	m_szYLegendText=_T("Y Axis Legend text  here");
}



////////////////////// SetDefaultFunctionname  //////////////////////////////
/*
*/
/////////////////////////////////////////////////////////////////////////////
void CTrendDiagramm::SetDefaultFunctionName()
{
	m_szFunctionNameText=_T("Function Name (or extra text)");
}

///////////////////////// SetGraphTitle /////////////////////////////////////
/*
This sets the GraphTitle
Takes:
CStringW
*/
/////////////////////////////////////////////////////////////////////////////
void CTrendDiagramm::SetGraphTitle(CStringW GraphTitle)
{
	//CDC* pDCStatic=CDC::FromHandle(m_hdcStatic);
	m_szGraphTitle=GraphTitle;
	//DrawGraphTitle(pDCStatic);

}


//////////////////////// SetXLegendText  ///////////////////////////////////
// **************************************************************************
// Sets the X axis legend text
// **************************************************************************
void CTrendDiagramm::SetXLegendText(CStringW XText)
{
	CDC* pDCStatic=CDC::FromHandle(m_hdcStatic);
	m_szXLegendText=XText;
	DrawXLegend(pDCStatic);
}


////////////////////// SetYLegendText /////////////////////////////////////
// **************************************************************************
// Sets the Y axis legend text
// **************************************************************************
void CTrendDiagramm::SetYLegendText(CStringW YText)
{
	CDC* pDCStatic=CDC::FromHandle(m_hdcStatic);
	m_szYLegendText=YText;
	DrawYLegend(pDCStatic);
}


////////////////////////////// SetFunctionName /////////////////////////////
/*
Sets the Function name text string
*/
////////////////////////////////////////////////////////////////////////////
void CTrendDiagramm::SetFunctionName(CStringW FunctionName)
{
	//CDC* pDCStatic=CDC::FromHandle(m_hdcStatic);
	m_szFunctionNameText=FunctionName;
	//DrawFunctionName(pDCStatic);
}

//////////////////// SetYLineAtLeft ////////////////////////////////////////
// **************************************************************************
// The Y axis line can be forced to the LHS of the 
//				plot area using this function
// **************************************************************************
//void CTrendDiagramm::SetYLineAtLeft(BOOL AtLeft)
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

////////////////////////// ShowGrid /////////////////////////////////////////
/*
This switches the grid on or off as set by the bShow parameter
The graph is repainted to match
*/
/////////////////////////////////////////////////////////////////////////////
void CTrendDiagramm::ShowGrid(BOOL bShow)
{
	//this is public function
	//show the graph grid if bShow==TRUE
	//or vice-versa
	BOOL bprevious=m_bShowGrid;
	m_bShowGrid=bShow;
	//if there is a change then repaint
	if (m_bShowGrid !=bprevious)
	{
		PaintGraph();
	}
}

/////////////////// ShowTicks ////////////////////////////////////////////
// **************************************************************************
// This switches the x and y axis 'ticks' on or off as set by the
//				bShow parameter
// **************************************************************************
void CTrendDiagramm::ShowTicks(BOOL bShow)
{
	//this is public function
	//show the graph grid if bShow==TRUE
	//or vice-versa
	BOOL bprevious=m_bShowTicks;
	m_bShowTicks=bShow;
	//if there is a change then repaint
	if (m_bShowTicks!=bprevious)
	{
		PaintGraph();
	}

}

////////////////////// ConvertToGraphCoords ///////////////////////////////////
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
LONG CTrendDiagramm::ConvertToGraphCoords(double x, double y)
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
	//double x1 = x*m_dPixelsPerX;
	//double y1 = y*m_dPixelsPerY;

	//calc the abs difference between Xmin and x;
	double xdif = abs(m_dXAxisMin - x);
	//calc the abs difference between Ymin and y;
	double ydif = abs(m_dYAxisMin - y);

	//find the dataarea
	CRect rect=CalcDataArea();

	int xpos=rect.left+(xdif*m_dPixelsPerX); //from left
	int ypos=rect.bottom-(ydif*m_dPixelsPerY); //from bottom

	result=MAKELONG(xpos,ypos);	

	return result;
}


///////////////////////////// DoFunction //////////////////////////////////////
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
BOOL CTrendDiagramm::DoFunction(G_FUNCTIONSTRUCT_TD *pFunctionParams)
{
	if( (pFunctionParams->pPlotXYItems==NULL) && (pFunctionParams->FuncType==G_PLOTXY))
	{
		return FALSE;
	}
	


	m_pFunctionParams=pFunctionParams;
	DrawFunction();
	return TRUE;
}


///////////////////////// ClearFunction ////////////////////////////////
// **************************************************************************
// his resets the G_FUNCTIONSTRUCT member pointer		
//		The graph is repainteed (cleared)
// **************************************************************************
void CTrendDiagramm::ClearFunction()
{
	////m_pFunctionParams=NULL; //reset the pointer

	////Clear the graph
	//m_bFirstpoint=TRUE;

	//if(bRedraw)
	//{
	//	PaintGraph();
	//}

	m_pFunctionParams=NULL; //reset the pointer
	//Clear the graph
	PaintGraph();
}


/////////////////////////////// DrawFunction  /////////////////////////////////
// **************************************************************************
// This is called from within the PaintGraph routine to draw	
//				the actual function onto the graph.				
//				This functions just switches the FunctionType member specified in the
//				G_FUNCTIONSTRUCT and calls the appropriate routine.
// **************************************************************************
void CTrendDiagramm::DrawFunction()
{
	if(!m_pFunctionParams)
	{
		return;
	}

	switch (m_pFunctionParams->FuncType)
	{
	/*case G_PLOTXY:
		DoPlotXY();
		break;*/
	case G_PLOTXYPOINTS:
		DoPlotXYPoints();
		break;
	/*case G_PLOTYPOINTS:
		DoPlotYPoints();
		break;*/
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
void CTrendDiagramm::DoPlotXYPoints()
{
	double xperpixel= VALUESPERMINUTE/m_dPixelsPerX;
	double yperpixel=1/m_dPixelsPerY;

	LONG result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
	UINT xstart=LOWORD(result);//the left hand side of the graph plot area on screen
	UINT ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen

	//rkuTRENDIMPR
	UINT pixelx=0;
	UINT pixely=0;
	UINT bufX=0;
	UINT x=0;
	double y=0;
	double yBufferVal=0;

	int iCountAverage=1;
	double iAverageY=0;

	//test
	int oldX=-1;
	int iOldCountbuffer=0;
	m_iCountbuffer=0;
	m_iPosbufferX=0;
	//int oldY=0;

	//rkuTRENDIMPR
	/*bool bTrendtYpe=false;
	if(m_iTrendType==TREND_PMEAN)
		bTrendtYpe=true;*/

	switch(m_pFunctionParams->ChartType)
	{
	case G_LINECHART:
		{
			int iPointsCount=0;

			for (UINT count =0; count < m_pFunctionParams->num_PlotXYItems; count++)
			{

				//rkuTRENDIMPR
				x=m_pFunctionParams->pPlotXYItems[count].pPlotXYItems.x;
				bufX=(x-m_dXAxisMin)/xperpixel;
				y=m_pFunctionParams->pPlotXYItems[count].pPlotXYItems.y;				
				
				//if x is off scale - don't bother
				if ( (x < m_dXAxisMin) || (x > m_dXAxisMax*TREND_DATAAREA))
				{
					continue; //NEXT !!!!!!
				}

				if(bufX<TREND_DATAAREA)
				{
					if(oldX==bufX)
					{
						yBufferVal+=y;
						iCountAverage++;

						if(y>m_dMaxY)
						{
							m_dMaxY=y;
						}
						if(y<m_dMinY)
						{
							m_dMinY=y;
						}

						//rkuNEWFIX
						/*pixelx= xstart+TREND_DATAAREA-bufX;
						if(pixelx<xstart)
							break;*/

						y=ConstrainY(y);
						iAverageY+=y;
						y=iAverageY/iCountAverage;
						pixely=ystart-(y-m_dYAxisMin)/yperpixel;

						//rkuNEWFIX
						//ptsTrend[iPointsCount].x=pixelx;
						ptsTrend[iPointsCount].y=pixely;

						bufferPosValues[iOldCountbuffer].y=yBufferVal/iCountAverage;
						//rkuNEWFIX
						//bufferPosValues[iOldCountbuffer].x=bufX;
					}
					else //if(oldX!=bufX)
					{
						yBufferVal=y;

						iCountAverage=1;
						
						if(y>m_dMaxY)
						{
							m_dMaxY=y;
						}
						if(y<m_dMinY)
						{
							m_dMinY=y;
						}

						pixelx= xstart+TREND_DATAAREA-bufX;
						y=ConstrainY(y);
						iAverageY=y;
						pixely=ystart-(y-m_dYAxisMin)/yperpixel;


						if(pixelx<xstart)
							break;

						ptsTrend[iPointsCount].x=pixelx;
						ptsTrend[iPointsCount].y=pixely;

						//DEBUGMSG(TRUE, (TEXT("2 x: %d y: %d\r"),pixelx,pixely));


						iPointsCount++;

						bufferPosValues[m_iCountbuffer].y=yBufferVal;
						bufferPosValues[m_iCountbuffer].x=bufX;
						iOldCountbuffer=m_iCountbuffer;
						m_iCountbuffer++;
					}
				}
				else
				{
					break;
				}
				oldX=bufX;				
			}

			Sleep(0);
			DEBUGMSG(TRUE, (TEXT("TREND: PlotPointArray %d\r\n"),iPointsCount));
			PlotPointArray(ptsTrend,iPointsCount);
			Sleep(0);

			if(m_bAutofit)
			{
				CheckAutoScaleY();
			}

			break;
		}
	case G_FILLEDCHART:
		{
			int iPointsCount=0;

			ptsTrend[0].x=xstart+TREND_DATAAREA;
			ptsTrend[0].y=ystart-(0-m_dYAxisMin)/yperpixel;

			iPointsCount++;

			for (UINT count =0; count < m_pFunctionParams->num_PlotXYItems; count++)
			{

				//rkuTRENDIMPR
				x=m_pFunctionParams->pPlotXYItems[count].pPlotXYItems.x;
				bufX=(x-m_dXAxisMin)/xperpixel;
				y=m_pFunctionParams->pPlotXYItems[count].pPlotXYItems.y;

				//if x is off scale - don't bother
				if ( (x < m_dXAxisMin) || (x > m_dXAxisMax*TREND_DATAAREA))
				{
					continue; //NEXT !!!!!!
				}

				if(bufX<TREND_DATAAREA)
				{
					if(oldX==bufX)
					{
						yBufferVal+=y;

						iCountAverage++;
						if(y>m_dMaxY)
						{
							m_dMaxY=y;
						}
						if(y<m_dMinY)
						{
							m_dMinY=y;
						}

						//rkuNEWFIX
						/*pixelx= xstart+TREND_DATAAREA-bufX;
						if(pixelx<xstart)
							break;*/

						y=ConstrainY(y);
						iAverageY+=y;
						y=iAverageY/iCountAverage;
						pixely=ystart-(y-m_dYAxisMin)/yperpixel;

						//rkuNEWFIX
						//ptsTrend[iPointsCount].x=pixelx;
						ptsTrend[iPointsCount].y=pixely;

						bufferPosValues[iOldCountbuffer].y=yBufferVal/iCountAverage;
						//rkuNEWFIX
						//bufferPosValues[iOldCountbuffer].x=bufX;
					}
					else //if(oldX!=bufX)
					{
						yBufferVal=y;

						iCountAverage=1;

						if(y>m_dMaxY)
						{
							m_dMaxY=y;
						}
						if(y<m_dMinY)
						{
							m_dMinY=y;
						}

						pixelx= xstart+TREND_DATAAREA-bufX;
						y=ConstrainY(y);
						iAverageY=y;
						pixely=ystart-(y-m_dYAxisMin)/yperpixel;


						if(pixelx<xstart)
							break;

						ptsTrend[iPointsCount].x=pixelx;
						ptsTrend[iPointsCount].y=pixely;

						//DEBUGMSG(TRUE, (TEXT("2 x: %d y: %d\r"),pixelx,pixely));

						iPointsCount++;

						bufferPosValues[m_iCountbuffer].y=yBufferVal;
						bufferPosValues[m_iCountbuffer].x=bufX;
						iOldCountbuffer=m_iCountbuffer;
						m_iCountbuffer++;
					}
				}
				else
				{
					break;
				}
				oldX=bufX;
			}

			ptsTrend[iPointsCount].x=pixelx;
			ptsTrend[iPointsCount].y=ystart-(0-m_dYAxisMin)/yperpixel;

			iPointsCount++;

			PlotPointArray(ptsTrend,iPointsCount);

			if(m_bAutofit)
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
//void CTrendDiagramm::DoPlotYPoints()
//{
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	//double xperpixel= 1/m_dPixelsPerX;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
//
//
//
//	/*UINT Graphbottom=m_iGraphY+m_iGraphHeight;
//	UINT Graphright=m_iGraphX+m_iGraphWidth;*/
//
//	switch(m_pFunctionParams->ChartType)
//	{
//	case G_LINECHART:
//		{
//			int iPointsCount=0;
//
//			for (UINT count =0; count < m_pFunctionParams->num_PlotXYItems; count++)
//			{
//				
//				double y=m_pFunctionParams->pPlotXYItems[count].pPlotXYItems.y;
//
//				UINT pixelx= xstart+(count+1-m_dXAxisMin);
//
//				y=ConstrainY(y);	
//				UINT pixely=ystart-(y-m_dYAxisMin)/yperpixel;
//
//				ptsTrend[count].x=pixelx;
//				ptsTrend[count].y=pixely;
//
//				iPointsCount++;
//			}
//
//			PlotPointArray(ptsTrend,iPointsCount);
//
//
//			break;
//		}
//	case G_FILLEDCHART:
//		{
//			
//			int iPointsCount=0;
//
//			ptsTrend[0].x=xstart+1-m_dXAxisMin;
//			ptsTrend[0].y=ystart-(0-m_dYAxisMin)/yperpixel;
//
//			iPointsCount++;
//
//			UINT count;
//
//			for (count =0; count < m_pFunctionParams->num_PlotXYItems; count++)
//			{
//
//				double y=m_pFunctionParams->pPlotXYItems[count].pPlotXYItems.y;
//
//				UINT pixelx= xstart+(count+1-m_dXAxisMin);
//
//				y=ConstrainY(y);	
//				UINT pixely=ystart-(y-m_dYAxisMin)/yperpixel;
//
//				ptsTrend[count+1].x=pixelx;
//				ptsTrend[count+1].y=pixely;
//
//				iPointsCount++;
//			}
//
//			ptsTrend[count+1].x=xstart+(count-m_dXAxisMin);
//			ptsTrend[count+1].y=ystart-(0-m_dYAxisMin)/yperpixel;
//
//			iPointsCount++;
//
//			PlotPointArray(ptsTrend,iPointsCount);
//
//
//			break;
//		}
//	default:
//		{
//			break;
//		}
//	}//SWITCH
//}

////////////////////// DoSineX ////////////////////////////////////////////////
/*																			///
This draws the function ASinX											///
*/																			///
///////////////////////////////////////////////////////////////////////////////
//void CTrendDiagramm::DoSineX()
//{
//	//we loop round from the given xstart to xend
//	//plotting y=sinx
//	UINT prevx=0;
//	UINT prevy=0;
//	BOOL firstpoint=TRUE;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	double xperpixel= 1/m_dPixelsPerX;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//the left hand side of the graph plot area on screen
//
//	//how many steps (pixels) in the x-direction
//	UINT xspan=(m_dXAxisMax - m_dXAxisMin)*m_dPixelsPerX;
//
//	UINT step;
//	if(m_pFunctionParams->ChartType==G_BARCHART)
//	{
//		step=G_BARCHARTWIDTH;
//	}
//	else
//	{
//		step=1;
//	}
//
//	for (UINT count =0; count < xspan; count+=step)
//	{
//		//do the function
//		double y=m_pFunctionParams->Const_1*sin(m_dXAxisMin+count*xperpixel);
//
//		UINT pixelx=xstart+count;
//
//		y=ConstrainY(y);
//		UINT pixely=ystart-((y-m_dYAxisMin)/yperpixel);
//
//
//
//		if(firstpoint)
//		{
//			prevx=pixelx;
//			prevy=pixely;
//			firstpoint=FALSE;
//		}
//		PlotPoints(pixelx,pixely,prevx,prevy);
//		//current point becomes previous point
//		prevx=pixelx;
//		prevy=pixely;
//
//	}
//
//}

//////////////////// DoCosX ////////////////////////////////////////////////////
/*																			////
Draws the function CosX													////
*/																			////
////////////////////////////////////////////////////////////////////////////////
//void CTrendDiagramm::DoCosX()
//{
//	//we loop round from the given xstart to xend
//	UINT prevx=0;
//	UINT prevy=0;
//	BOOL firstpoint=TRUE;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	double xperpixel= 1/m_dPixelsPerX;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//the left hand side of the graph plot area on screen
//
//	//how many steps (pixels) in the x-direction
//	UINT xspan=(m_dXAxisMax - m_dXAxisMin)*m_dPixelsPerX;
//
//	UINT step;
//	if(m_pFunctionParams->ChartType==G_BARCHART)
//	{
//		step=G_BARCHARTWIDTH;
//	}
//	else
//	{
//		step=1;
//	}
//
//	for (UINT count =0; count < xspan; count+=step)
//	{
//		//do the function
//		double y=m_pFunctionParams->Const_1*cos(m_dXAxisMin+count*xperpixel);
//
//		UINT pixelx=xstart+count;
//
//		y=ConstrainY(y);
//		UINT pixely=ystart-((y-m_dYAxisMin)/yperpixel);
//
//		if(firstpoint)
//		{
//			prevx=pixelx;
//			prevy=pixely;
//			firstpoint=FALSE;
//		}
//		PlotPoints(pixelx,pixely,prevx,prevy);
//		//current point becomes previous point
//		prevx=pixelx;
//		prevy=pixely;
//	}
//}

/////////////////// DoXSquared  ///////////////////////////////////////////////////
/*																				///
*/																				///
///////////////////////////////////////////////////////////////////////////////////
//void CTrendDiagramm::DoXSquared()
//{
//	//we loop round from the given xstart to xend
//	UINT prevx=0;
//	UINT prevy=0;
//	BOOL firstpoint=TRUE;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	double xperpixel= 1/m_dPixelsPerX;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//the left hand side of the graph plot area on screen
//
//	//how many steps (pixels) in the x-direction
//	UINT xspan=(m_dXAxisMax -m_dXAxisMin)*m_dPixelsPerX;
//
//	UINT step;
//	if(m_pFunctionParams->ChartType==G_BARCHART)
//	{
//		step=G_BARCHARTWIDTH;
//	}
//	else
//	{
//		step=1;
//	}
//
//	for (UINT count =0; count < xspan; count+=step)
//	{
//		//do the function
//		double y=pow( (m_dXAxisMin+count*xperpixel),2);
//
//		UINT pixelx=xstart+count;
//		y=ConstrainY(y);//keep y within the plottable y scale
//
//		UINT pixely=ystart-((y-m_dYAxisMin)/yperpixel);
//
//		if(firstpoint)
//		{
//			prevx=pixelx;
//			prevy=pixely;
//			firstpoint=FALSE;
//		}
//		PlotPoints(pixelx,pixely,prevx,prevy);
//		//current point becomes previous point
//		prevx=pixelx;
//		prevy=pixely;
//
//	}
//}

/////////////////// DoXCubed   /////////////////////////////////////////////////
/*																			////
*/																			////
////////////////////////////////////////////////////////////////////////////////
//void CTrendDiagramm::DoXCubed()
//{
//	//we loop round from the given xstart to xend
//	UINT prevx=0;
//	UINT prevy=0;
//	BOOL firstpoint=TRUE;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	double xperpixel= 1/m_dPixelsPerX;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
//
//	//how many steps (pixels) in the x-direction
//	UINT xspan=(m_dXAxisMax - m_dXAxisMin)*m_dPixelsPerX;
//
//	UINT step;
//	if(m_pFunctionParams->ChartType==G_BARCHART)
//	{
//		step=G_BARCHARTWIDTH;
//	}
//	else
//	{
//		step=1;
//	}
//
//	for (UINT count =0; count < xspan; count+=step)
//	{
//		//do the function
//		double y=pow( (m_dXAxisMin+count*xperpixel),3.00);
//
//		UINT pixelx=xstart+count;
//
//		y=ConstrainY(y);	
//		UINT pixely=ystart-((y-m_dYAxisMin)/yperpixel);
//
//		if(firstpoint)
//		{
//			prevx=pixelx;
//			prevy=pixely;
//			firstpoint=FALSE;
//		}
//		PlotPoints(pixelx,pixely,prevx,prevy);
//		//current point becomes previous point
//		prevx=pixelx;
//		prevy=pixely;
//
//	}
//}

//////////////////// DoMXPlusC ////////////////////////////////////////////////
/*																			///	
*/																			///
///////////////////////////////////////////////////////////////////////////////
//void CTrendDiagramm::DoMXPlusC()
//{
//	UINT prevx=0;
//	UINT prevy=0;
//	BOOL firstpoint=TRUE;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	double xperpixel= 1/m_dPixelsPerX;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
//
//	//how many steps are they (pixels) in the x-direction
//	UINT xspan=(m_dXAxisMax - m_dXAxisMin)*m_dPixelsPerX;
//
//	UINT step;
//	if(m_pFunctionParams->ChartType==G_BARCHART)
//	{
//		step=G_BARCHARTWIDTH;
//	}
//	else
//	{
//		step=1;
//	}
//
//	for (UINT count =0; count < xspan; count+=step)
//	{
//		//do the function
//		double y=m_pFunctionParams->Const_1*( m_dXAxisMin+(count*xperpixel))+m_pFunctionParams->Const_2;
//
//		UINT pixelx=xstart+count;
//
//		y=ConstrainY(y);	
//		UINT pixely=ystart-((y-m_dYAxisMin)/yperpixel);
//
//		if(firstpoint)
//		{
//			prevx=pixelx;
//			prevy=pixely;
//			firstpoint=FALSE;
//		}
//		PlotPoints(pixelx,pixely,prevx,prevy);
//		//current point becomes previous point
//		prevx=pixelx;
//		prevy=pixely;
//
//	}
//}


////////////////////////// DoLogeX  ///////////////////////////////////////////////
/*																				///
*/																				///
///////////////////////////////////////////////////////////////////////////////////
//void CTrendDiagramm::DoLogeX()
//{
//	UINT prevx=0;
//	UINT prevy=0;
//	BOOL firstpoint=TRUE;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	double xperpixel= 1/m_dPixelsPerX;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
//
//	//how many steps are they (pixels) in the x-direction
//	UINT xspan=(m_dXAxisMax - m_dXAxisMin)*m_dPixelsPerX;
//
//	UINT step;
//	if(m_pFunctionParams->ChartType==G_BARCHART)
//	{
//		step=G_BARCHARTWIDTH;
//	}
//	else
//	{
//		step=1;
//	}
//
//	for (UINT count =0; count < xspan; count+=step)
//	{
//		//do the function
//		double y=log( m_dXAxisMin+(count*xperpixel));
//
//		UINT pixelx=xstart+count;
//
//		y=ConstrainY(y);	
//		UINT pixely=ystart-(y-m_dYAxisMin)/yperpixel;
//
//		if(firstpoint)
//		{
//			prevx=pixelx;
//			prevy=pixely;
//			firstpoint=FALSE;
//		}
//		PlotPoints(pixelx,pixely,prevx,prevy);
//		//current point becomes previous point
//		prevx=pixelx;
//		prevy=pixely;
//
//	}
//}


////////////////////////  DoPlotXY ////////////////////////////////////////////////
/*																				///	
This has many similarities with the other functions - however				///
there is no need to calculate Y  in the sane way as in the other function	///
as it is given.																///
a particular x point on the graph is also given - we place the				/// 
given y value at the given x point											///
Other:																		///
a. We will not bother plotting  if x < the graph scale XMin or > XMax		///
b. Only dot, bar and line will be acceptable for the chart type. Any other	///
will default to bar. However be aware that line is only suitable if the		///
x-values are in sequence.													///
c. The usual y constraints apply											///
*/																				///																			////
///////////////////////////////////////////////////////////////////////////////////
//void CTrendDiagramm::DoPlotXY()
//{
//	UINT prevx=0;
//	UINT prevy=0;
//	BOOL firstpoint=TRUE;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	double xperpixel= VALUESPERMINUTE/m_dPixelsPerX;
//	//double xperpixel= 1;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
//
//	for (UINT count =0; count < m_pFunctionParams->num_PlotXYItems; count++)
//	{
//
//		double x=m_pFunctionParams->pPlotXYItems[count].pPlotXYItems.x;
//		double y=m_pFunctionParams->pPlotXYItems[count].pPlotXYItems.y;
//
//		//if x is off scale - don't bother
//		if ( (x < m_dXAxisMin) || (x > m_dXAxisMax*VALUESPERMINUTE))
//		{
//			continue; //NEXT !!!!!!
//		}
//
//		//UINT pixelx= xstart+(x-m_dXAxisMin)/xperpixel;
//		UINT pixelx= xstart+498-(x-m_dXAxisMin)/xperpixel;
//
//
//		y=ConstrainY(y);	
//		UINT pixely=ystart-(y-m_dYAxisMin)/yperpixel;
//
//		if(firstpoint)
//		{
//			prevx=pixelx;
//			prevy=pixely;
//			firstpoint=FALSE;
//		}
//		PlotPoints(pixelx,pixely,prevx,prevy);
//		//current point becomes previous point
//		prevx=pixelx;
//		prevy=pixely;
//
//	}
//}


////////////////////////  DoPlotXY ////////////////////////////////////////////////
/*																				///	
This has many similarities with the other functions - however				///
there is no need to calculate Y  in the sane way as in the other function	///
as it is given.																///
a particular x point on the graph is also given - we place the				/// 
given y value at the given x point											///
Other:																		///
a. We will not bother plotting  if x < the graph scale XMin or > XMax		///
b. Only dot, bar and line will be acceptable for the chart type. Any other	///
will default to bar. However be aware that line is only suitable if the		///
x-values are in sequence.													///
c. The usual y constraints apply											///
*/																				///																			////
///////////////////////////////////////////////////////////////////////////////////
//void CTrendDiagramm::DoPlotY()
//{
//	UINT prevx=0;
//	UINT prevy=0;
//	BOOL firstpoint=TRUE;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	//double xperpixel= 1/m_dPixelsPerX;
//	double xperpixel= 1;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
//
//	for (UINT count =0; count < m_pFunctionParams->num_PlotXYItems; count++)
//	{
//
//		double x=m_pFunctionParams->pPlotXYItems[count].pPlotXYItems.x;
//		double y=m_pFunctionParams->pPlotXYItems[count].pPlotXYItems.y;
//
//		//if x is off scale - don't bother
//		if ( (x < m_dXAxisMin) || (x > m_dXAxisMax))
//		{
//			continue; //NEXT !!!!!!
//		}
//
//		UINT pixelx= xstart+(x-m_dXAxisMin)/xperpixel;
//
//		y=ConstrainY(y);	
//		UINT pixely=ystart-(y-m_dYAxisMin)/yperpixel;
//
//		if(firstpoint)
//		{
//			prevx=pixelx;
//			prevy=pixely;
//			firstpoint=FALSE;
//		}
//		PlotPoints(pixelx,pixely,prevx,prevy);
//		//current point becomes previous point
//		prevx=pixelx;
//		prevy=pixely;
//
//	}
//}


/////////////////////// DoDeviationPercentAbs /////////////////////////////////////
/*																				///
There are two types of deviation graphs, namely percent and absolute		///
but hey both end up here to be sorted out									///
*/																				///
///////////////////////////////////////////////////////////////////////////////////
//void CTrendDiagramm::DoDeviationPercentAbs()
//{
//	///* 
//	//No X valuess are supplied. The x-axis represents
//	//samples really
//	//a. We will not bother plotting  if x < the graph scale XMin or > XMax
//	//c. The usual y constraints apply
//	//*/
//	//UINT prevx=0;
//	//UINT prevy=0;
//	//BOOL firstpoint=TRUE;
//
//	//LONG result;
//	//UINT xstart;
//	//UINT ystart;
//	//double yperpixel=1/m_dPixelsPerY;
//
//	//result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
//	//xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	//ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
//
//	///*
//	//We need to know when we will run off the right hand side
//	//*/
//	//CRect dataarea=CalcDataArea();
//
//	//for (UINT count =0; count < m_pFunctionParams->num_PlotXYItems*2; count+=2)
//	//{
//
//	//	double yval;//will hold percent or absolute devaition
//	//	double yset=m_pFunctionParams->pPlotXYItems[count];
//	//	double yactual=m_pFunctionParams->pPlotXYItems[count+1];
//	//	if( m_pFunctionParams->FuncType==G_PLOTDEV_PERCENT)
//	//	{
//	//		yval=(yactual-yset)/yset*100; //percentage of setpoint
//	//	}
//	//	else
//	//	{
//	//		yval=(yactual-yset); //difference
//	//	}
//
//
//	//	//if we reach the right hand side then we can stop
//
//	//	UINT pixelx= xstart+( (count/2)*m_dPixelsPerX);
//	//	if (pixelx > dataarea.right )
//	//	{
//	//		return; //no need to go any further
//	//	}
//
//
//	//	yval=ConstrainY(yval);	
//	//	UINT pixely=ystart-(yval-m_dYAxisMin)/yperpixel;
//
//	//	if(firstpoint)
//	//	{
//	//		prevx=pixelx;
//	//		prevy=pixely;
//	//		firstpoint=FALSE;
//	//	}
//	//	PlotPoints(pixelx,pixely,prevx,prevy);
//	//	//current point becomes previous point
//	//	prevx=pixelx;
//	//	prevy=pixely;
//
//	//}
//}


///////////////////// DoMultiPlotXY  //////////////////////////////////////////////
/*	This has many similarities with the PlotXY functions - it plots several		///
graphs on the																///
same scale and axis.														///
It loops (number of graphs) times - drawing each graph in it's				///
entirety each time.															///
Other:																		///
a. We will not bother plotting  if x < the graph scale XMin or > XMax		///
b. Dot, bar, and line will be acceptable for the chart type.				///
c. The usual y constraints apply											///
///
The number of graphs required are givenn in the								///
Const_1 fiend of the G_FUNCTIONSTRUCT										///
*/																				///
///////////////////////////////////////////////////////////////////////////////////
//void CTrendDiagramm::DoMultiPlotXY()
//{
//	UINT prevx=0;
//	UINT prevy=0;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	//double xperpixel= 1/m_dPixelsPerX;
//	double xperpixel= 1;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
//
//	//how many graphs?
//	UINT num_graphs=m_pFunctionParams->Const_1;
//	//how may plot items per graph
//	UINT num_plotitems=m_pFunctionParams->num_PlotXYItems;
//	//save the default pen color
//	COLORREF savedpencolor=m_crGraphPenColor;
//	UINT PenCount=0;
//
//	for( long countgraphs=0;countgraphs < 
//		num_graphs*num_plotitems*2;countgraphs+=(long)num_plotitems*2)
//	{
//
//		//set the pencolor for the current graph
//		m_crGraphPenColor=GetMultiPlotPenColor(PenCount);
//
//		BOOL firstpoint=TRUE;//first time round each graph loop
//
//		for (UINT count =0; count < num_plotitems; count++)
//		{
//
//			double x=(m_pFunctionParams->pPlotXYItems+countgraphs)[count].pPlotXYItems.x;
//			double y=(m_pFunctionParams->pPlotXYItems+countgraphs)[count].pPlotXYItems.y;
//
//			//if x is off scale - don't bother
//			if ( (x < m_dXAxisMin) || (x > m_dXAxisMax))
//			{
//				continue; //NEXT !!!!!!
//			}
//
//			UINT pixelx= xstart+(x-m_dXAxisMin)/xperpixel;
//
//			y=ConstrainY(y);	
//			UINT pixely=ystart-(y-m_dYAxisMin)/yperpixel;
//
//			if(firstpoint)
//			{
//				prevx=pixelx;
//				prevy=pixely;
//				firstpoint=FALSE;
//			}
//			PlotPoints(pixelx,pixely,prevx,prevy);
//			//current point becomes previous point
//			prevx=pixelx;
//			prevy=pixely;
//
//		}//end this graph
//
//		PenCount++; //next pen
//	}//all graphs
//
//	//restore the graph pencolor
//	m_crGraphPenColor=savedpencolor;
//}

///////////////    DoHistogram ////////////////////////////////////////////////////
/*																				///
The user can request two types of histogram, namely							///
one where the frequency of occurrence is displayed as anumber value,		///
the other where the frequency of occurrence is displayed as a percentage	///
of the of the quantity of dataitems											///
Either way they both end up here to be plotted.								///
The usersupplies the usual data pairs.										///
However, the first values of the pair(usually the x value)					///
is not used by this function. The user can use this as a sequence			///
number if they wish.														///
We use the send value of each pair -usually the y value.					///
//For the histogram - we need to calulate									///
//a. The mean value															///
//b. The Root of the Mean of the Squares;									///
///
To disply the data we, we divide the graph(dataarea) by the width			///
of a bar to see how many we can fit in. We then divide the min-max			///
span of the data into the same amount of slices.							///
We then check each data item to see which slice it falls into.				///
*/																				///
///////////////////////////////////////////////////////////////////////////////////
//void CTrendDiagramm::DoHistogram()
//{
//
//	//UINT prevx=0;
//	//UINT prevy=0;
//	//BOOL firstpoint=TRUE;
//
//	//LONG result;
//	//UINT xstart;
//	//UINT ystart;
//
//	//result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
//	//xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	//ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
//
//	////we will autoscale the x-axis to match the min and max value in the data;
//	//UINT count;
//	//double minval,maxval;
//	//minval=maxval=m_pFunctionParams->pPlotXYItems[1];
//	//for (count =0; count < m_pFunctionParams->num_PlotXYItems*2; count+=2)
//	//{
//	//	if(m_pFunctionParams->pPlotXYItems[count+1] < minval)
//	//	{
//	//		minval=m_pFunctionParams->pPlotXYItems[count+1];
//	//	}
//	//	else if (m_pFunctionParams->pPlotXYItems[count+1] > maxval)
//	//	{
//	//		maxval=m_pFunctionParams->pPlotXYItems[count+1];
//	//	}
//	//}
//	////we should have just found the min and max values of the data
//	////scale graph accordingly
//	//SetXAxisScale(minval,maxval);
//	//DrawXAxisNumbers();
//
//	////find how many bars can fit into the praph plot width
//	//CRect dataarearect=CalcDataArea();
//	//UINT num_strips= (dataarearect.right-dataarearect.left)/G_BARCHARTWIDTH;
//
//	////find out how much a strip is 'worth'
//	//double strip_val= (maxval-minval)/num_strips;
//	////count the frequency of the data
//	//double *pCounts=new double[num_strips];
//	//memset(pCounts,0,num_strips*sizeof(double));
//	//for(count=0;count < m_pFunctionParams->num_PlotXYItems*2;count +=2)
//	//{
//	//	double value=m_pFunctionParams->pPlotXYItems[count+1];
//	//	//see which strip the current value falls into
//	//	for(UINT stripcount=0; stripcount<num_strips;stripcount++)
//	//	{
//
//	//		if( (value >=minval+(stripcount*strip_val)) && 
//	//			(value <= minval+(stripcount*strip_val)+strip_val))	
//	//		{
//	//			pCounts[stripcount]++;
//	//			break;
//	//		}
//	//	}
//
//	//}
//	///*
//	//Chack if we are showing the histogram frequencies as an absolute
//	//occurrence figure or as a percentage occurrence
//	//*/
//	//if(m_pFunctionParams->FuncType==G_HISTOGRAM_ABS)
//	//{
//
//	//	//find the maximum frequency of occurence and scale the 
//	//	//y axis
//	//	int max_freqcurrent=pCounts[0]; //preset a start point
//	//	for(count=1;count < num_strips; count++)
//	//	{
//	//		if (pCounts[count] > max_freqcurrent)
//	//		{
//	//			max_freqcurrent=pCounts[count];
//	//		}
//	//	}
//
//	//	SetYAxisScale(0,max_freqcurrent+1);
//
//	//}
//	//else
//	//{
//	//	//we are showing as a percentage
//	//	//change the ocurrences from an absolute occurence figure to % occurrence
//
//	//	for(count=0;count < num_strips; count++)
//	//	{
//	//		pCounts[count] =( (pCounts[count]/ ((double)m_pFunctionParams->num_PlotXYItems))*100);
//	//	}
//
//	//	//find the maximum frequency of occurence and scale the 
//	//	//y axis
//	//	int max_freqcurrent=pCounts[0]; //preset a start point
//	//	for(count=1;count < num_strips; count++)
//	//	{
//	//		if (pCounts[count] > max_freqcurrent)
//	//		{
//	//			max_freqcurrent=pCounts[count];
//	//		}
//	//	}
//
//
//	//	SetYAxisScale(0,max_freqcurrent+1);
//	//}
//
//	////in both cases
//	//DrawYAxisNumbers();
//
//	////we have changed the x & y axis scaling so recalculate
//	////the yperpixel and/or xperpixel as required
//	//double yperpixel=1/m_dPixelsPerY;	
//
//	//for (count =0; count < num_strips; count++)
//	//{		
//	//	UINT pixelx= xstart+(count*G_BARCHARTWIDTH);;
//
//	//	pCounts[count]=ConstrainY(pCounts[count]);	
//	//	UINT pixely=ystart-(pCounts[count]/yperpixel);
//
//	//	if(firstpoint)
//	//	{
//	//		prevx=pixelx;
//	//		prevy=pixely;
//	//		firstpoint=FALSE;
//	//	}
//	//	PlotPoints(pixelx,pixely,prevx,prevy);
//	//	//current point becomes previous point
//	//	prevx=pixelx;
//	//	prevy=pixely;
//
//	//}
//	////cleanup
//	//delete []pCounts;
//}


// **************************************************************************
// 
// **************************************************************************
void CTrendDiagramm::CheckAutoScaleY()
{
	bool bRedraw=false;

	if(m_dMaxY>m_dYAxisMax && m_pFunctionParams->yMax>m_dYAxisMax)
	{
		IncreaseYScale();

		m_dMaxY=0;
		m_dMinY=0;

		bRedraw=true;
	}
	else if(m_pFunctionParams->yMin<m_dYAxisMin && m_dMinY<m_dYAxisMin)
	{
		IncreaseYScale();

		m_dMaxY=0;
		m_dMinY=0;

		bRedraw=true;
	}
	else
	{
		if(CanDecreaseYScale())
		{
			DecreaseYScale();

			bRedraw=true;
		}

		m_dMaxY=0;
		m_dMinY=0;
	}

	if(bRedraw)
	{
		if(GetParent())
			GetParent()->PostMessage(WM_REDRAW_DIAGRAMM,m_nID);
	}
}

double CTrendDiagramm::GetNegativeYoffset(double dwMaxY)
{
	double result=(dwMaxY/(GetYPixelsOfDiagramm()-10));
	result=result*(-10);

	return result;
}

// **************************************************************************
// 
// **************************************************************************
void CTrendDiagramm::IncreaseYScale()
{
	double dHigherYAxisMax=GetHigherYAxisMax();

	if(m_dYAxisMax!=dHigherYAxisMax)
	{
		SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, true);

		switch(m_iTrendType)
		{
		case TREND_PINSP:
			{
				getModel()->getCONFIG()->TrendSetPInspMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_PMEAN:
			{
				getModel()->getCONFIG()->TrendSetPMeanMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_FIO2:
			{
				getModel()->getCONFIG()->TrendSetFIO2Max((int)dHigherYAxisMax);
			}
			break;
		case TREND_VTE:
			{
				getModel()->getCONFIG()->TrendSetVteMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_COMPLIANCE:
			{
				getModel()->getCONFIG()->TrendSetComplianceMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_CO2HFO:
			{
				getModel()->getCONFIG()->TrendSetDCO2Max((int)dHigherYAxisMax);
			}
			break;
		case TREND_MV:
			{
				getModel()->getCONFIG()->TrendSetMVMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_HFAMP:
			{
				getModel()->getCONFIG()->TrendSetHFAMPMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_RSBI:
			{
				getModel()->getCONFIG()->TrendSetRSBIMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_SHAREMVMAND:
			{
				getModel()->getCONFIG()->TrendSetSHAREMVMANDMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_RESISTANCE:
			{
				getModel()->getCONFIG()->TrendSetRESISTANCEMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_LEAK:
			{
				getModel()->getCONFIG()->TrendSetLEAKMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_SPO2:
			{
				getModel()->getCONFIG()->TrendSetSPO2Max((int)dHigherYAxisMax);
			}
			break;
		case TREND_SPO2PR:
			{
				getModel()->getCONFIG()->TrendSetSPO2PRMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_SPO2PI:
			{
				getModel()->getCONFIG()->TrendSetSPO2PIMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_ETCO2:
			{
				getModel()->getCONFIG()->TrendSetETCO2Max((int)dHigherYAxisMax);
			}
			break;
		case TREND_FREQUENCY:
			{
				getModel()->getCONFIG()->TrendSetFREQUENCYMax((int)dHigherYAxisMax);
			}
			break;
		}
	}
}
// **************************************************************************
// 
// **************************************************************************
double CTrendDiagramm::GetHigherYAxisMax()
{
	double dValue=m_dMaxY;

	if((m_dMinY *(-1))>m_dMaxY)
		dValue=m_dMinY *(-1);
	double dHigherAxisMax=0;

	switch(m_iTrendType)
	{
	case TREND_FIO2://newtrend
		{
			dHigherAxisMax=m_dYAxisMax;

			while (dValue>dHigherAxisMax)
			{
				if(dHigherAxisMax<100)
				{
					dHigherAxisMax=100;
				}
				else if(dHigherAxisMax<200)
				{
					dHigherAxisMax=200;
				}
				else if(dHigherAxisMax<400)
				{
					dHigherAxisMax=dHigherAxisMax+100;
				}
				else
				{
					dHigherAxisMax=dHigherAxisMax+200;
				}
			}
		}
		break;
	case TREND_VTE:
		{
			dHigherAxisMax=m_dYAxisMax;

			while (dValue>dHigherAxisMax)
			{
				if(dHigherAxisMax<10)
				{
					dHigherAxisMax=10;
				}
				else if(dHigherAxisMax<25)
				{
					dHigherAxisMax=25;
				}
				else if(dHigherAxisMax<100)
				{
					dHigherAxisMax=dHigherAxisMax+25;
				}
				else /*if(dHigherAxisMax<600)*/
				{
					dHigherAxisMax=dHigherAxisMax+100;
				}
			}
		}
		break;
	case TREND_COMPLIANCE:
		{
			dHigherAxisMax=m_dYAxisMax;

			while (dValue>dHigherAxisMax)
			{
				if(dHigherAxisMax<100)
				{
					dHigherAxisMax=100;
				}
				else if(dHigherAxisMax<250)
				{
					dHigherAxisMax=250;
				}
				else /*if(dHigherAxisMax<100)*/
				{
					dHigherAxisMax=dHigherAxisMax+250;
				}
			}
		}
		break;
	case TREND_PINSP:
	case TREND_PMEAN:
	case TREND_CO2HFO:
	case TREND_MV:
	case TREND_HFAMP:
	case TREND_RSBI: 
	case TREND_RESISTANCE:
	//case TREND_SPO2PR:
		{
			dHigherAxisMax=m_dYAxisMax;

			while (dValue>dHigherAxisMax)
			{
				if(dHigherAxisMax<10)
				{
					dHigherAxisMax=10;
				}
				else if(dHigherAxisMax<25)
				{
					dHigherAxisMax=25;
				}
				else if(dHigherAxisMax<100)
				{
					dHigherAxisMax=dHigherAxisMax+25;
				}
				else if(dHigherAxisMax<500)
				{
					dHigherAxisMax=dHigherAxisMax+100;
				}
				else if(dHigherAxisMax<1000)
				{
					dHigherAxisMax=dHigherAxisMax+250;
				}
				else if(dHigherAxisMax<5000)
				{
					dHigherAxisMax=dHigherAxisMax+1000;
				}
				else
				{
					dHigherAxisMax=dHigherAxisMax+2500;
				}
			}
		}
		break;
	case TREND_ETCO2:
		{
			dHigherAxisMax=m_dYAxisMax;

			while (dValue>dHigherAxisMax)
			{
				if(dHigherAxisMax<10)
				{
					dHigherAxisMax=10;
				}
				else if(dHigherAxisMax<20)
				{
					dHigherAxisMax=dHigherAxisMax+10;
				}
				else 
				{
					dHigherAxisMax=dHigherAxisMax+20;
				}
			}
		}
		break;
	case TREND_SPO2:
		{
			dHigherAxisMax=m_dYAxisMax;

			while (dValue>dHigherAxisMax)
			{
				if(dHigherAxisMax<10)
				{
					dHigherAxisMax=10;
				}
				else if(dHigherAxisMax<20)
				{
					dHigherAxisMax=dHigherAxisMax+10;
				}
				else 
				{
					dHigherAxisMax=dHigherAxisMax+20;
				}
			}
		}
		break;
	case TREND_SPO2PR:
		{
			dHigherAxisMax=m_dYAxisMax;

			while (dValue>dHigherAxisMax)
			{
				if(dHigherAxisMax<10)
				{
					dHigherAxisMax=10;
				}
				else if(dHigherAxisMax<20)
				{
					dHigherAxisMax=dHigherAxisMax+10;
				}
				else 
				{
					dHigherAxisMax=dHigherAxisMax+20;
				}
			}
		}
		break;
	case TREND_SPO2PI:
		{
			dHigherAxisMax=m_dYAxisMax;

			while (dValue>dHigherAxisMax)
			{
				if(dHigherAxisMax<50)
				{
					dHigherAxisMax=50;
				}
				else if(dHigherAxisMax<100)
				{
					dHigherAxisMax=100;
				}
				else 
				{
					dHigherAxisMax=dHigherAxisMax+100;
				}
			}
		}
		break;
	case TREND_LEAK:
		{
			dHigherAxisMax=m_dYAxisMax;

			while (dValue>dHigherAxisMax)
			{
				if(dHigherAxisMax<10)
				{
					dHigherAxisMax=10;
				}
				else if(dHigherAxisMax<20)
				{
					dHigherAxisMax=dHigherAxisMax+10;
				}
				else 
				{
					dHigherAxisMax=dHigherAxisMax+20;
				}
			}
		}
		break;
	case TREND_SHAREMVMAND:
		{
			dHigherAxisMax=m_dYAxisMax;

			while (dValue>dHigherAxisMax)
			{
				if(dHigherAxisMax<20)
				{
					dHigherAxisMax=20;
				}
				else if(dHigherAxisMax<100)
				{
					dHigherAxisMax=dHigherAxisMax+20;
				}
				else
				{
					dHigherAxisMax=m_pFunctionParams->yMax;
				}
			}
		}
		break;
	case TREND_FREQUENCY:
		{
			dHigherAxisMax=m_dYAxisMax;

			while (dValue>dHigherAxisMax)
			{
				if(dHigherAxisMax<10)
				{
					dHigherAxisMax=10;
				}
				else if(dHigherAxisMax<20)
				{
					dHigherAxisMax=dHigherAxisMax+10;
				}
				else 
				{
					dHigherAxisMax=dHigherAxisMax+20;
				}
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
bool CTrendDiagramm::IncreaseYScaleToNextValue()
{
	bool bResult=false;
	double dHigherYAxisMax=GetNextHigherYAxisMax();

	if(m_dYAxisMax!=dHigherYAxisMax)
	{
		bResult=true;

		SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, true);
		switch(m_iTrendType)
		{
		case TREND_PINSP:
			{
				getModel()->getCONFIG()->TrendSetPInspMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_PMEAN:
			{
				getModel()->getCONFIG()->TrendSetPMeanMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_FIO2:
			{
				getModel()->getCONFIG()->TrendSetFIO2Max((int)dHigherYAxisMax);
			}
			break;
		case TREND_VTE:
			{
				getModel()->getCONFIG()->TrendSetVteMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_COMPLIANCE:
			{
				getModel()->getCONFIG()->TrendSetComplianceMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_CO2HFO:
			{
				getModel()->getCONFIG()->TrendSetDCO2Max((int)dHigherYAxisMax);
			}
			break;
		case TREND_MV:
			{
				getModel()->getCONFIG()->TrendSetMVMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_HFAMP:
			{
				getModel()->getCONFIG()->TrendSetHFAMPMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_RSBI:
			{
				getModel()->getCONFIG()->TrendSetRSBIMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_SHAREMVMAND:
			{
				getModel()->getCONFIG()->TrendSetSHAREMVMANDMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_RESISTANCE:
			{
				getModel()->getCONFIG()->TrendSetRESISTANCEMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_LEAK:
			{
				getModel()->getCONFIG()->TrendSetLEAKMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_SPO2:
			{
				getModel()->getCONFIG()->TrendSetSPO2Max((int)dHigherYAxisMax);
			}
			break;
		case TREND_SPO2PR:
			{
				getModel()->getCONFIG()->TrendSetSPO2PRMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_SPO2PI:
			{
				getModel()->getCONFIG()->TrendSetSPO2PIMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_ETCO2:
			{
				getModel()->getCONFIG()->TrendSetETCO2Max((int)dHigherYAxisMax);
			}
			break;
		case TREND_FREQUENCY:
			{
				getModel()->getCONFIG()->TrendSetFREQUENCYMax((int)dHigherYAxisMax);
			}
			break;
		}
	}
	return bResult;
}

// **************************************************************************
// 
// **************************************************************************
void CTrendDiagramm::DecreaseYScale()
{
	double dHigherYAxisMax=GetLowerYAxisMax();

	if(m_dYAxisMax!=dHigherYAxisMax)
	{
		SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, true);

		switch(m_iTrendType)
		{
		case TREND_PINSP:
			{
				getModel()->getCONFIG()->TrendSetPInspMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_PMEAN:
			{
				getModel()->getCONFIG()->TrendSetPMeanMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_FIO2:
			{
				getModel()->getCONFIG()->TrendSetFIO2Max((int)dHigherYAxisMax);
			}
			break;
		case TREND_VTE:
			{
				getModel()->getCONFIG()->TrendSetVteMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_COMPLIANCE:
			{
				getModel()->getCONFIG()->TrendSetComplianceMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_CO2HFO:
			{
				getModel()->getCONFIG()->TrendSetDCO2Max((int)dHigherYAxisMax);
			}
			break;
		case TREND_MV:
			{
				getModel()->getCONFIG()->TrendSetMVMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_HFAMP:
			{
				getModel()->getCONFIG()->TrendSetHFAMPMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_RSBI:
			{
				getModel()->getCONFIG()->TrendSetRSBIMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_SHAREMVMAND:
			{
				getModel()->getCONFIG()->TrendSetSHAREMVMANDMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_RESISTANCE:
			{
				getModel()->getCONFIG()->TrendSetRESISTANCEMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_LEAK:
			{
				getModel()->getCONFIG()->TrendSetLEAKMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_SPO2:
			{
				getModel()->getCONFIG()->TrendSetSPO2Max((int)dHigherYAxisMax);
			}
			break;
		case TREND_SPO2PR:
			{
				getModel()->getCONFIG()->TrendSetSPO2PRMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_SPO2PI:
			{
				getModel()->getCONFIG()->TrendSetSPO2PIMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_ETCO2:
			{
				getModel()->getCONFIG()->TrendSetETCO2Max((int)dHigherYAxisMax);
			}
			break;
		case TREND_FREQUENCY:
			{
				getModel()->getCONFIG()->TrendSetFREQUENCYMax((int)dHigherYAxisMax);
			}
			break;
		}
	}
}
// **************************************************************************
// 
// **************************************************************************
double CTrendDiagramm::GetLowerYAxisMax()
{
	double dValue=m_dMaxY;

	if((m_dMinY *(-1))>m_dMaxY)
		dValue=m_dMinY *(-1);
	double dLowerAxisMax=0;
	double dTemp=0;

	switch(m_iTrendType)
	{
	case TREND_FIO2://newtrend
		{
			dLowerAxisMax=m_dYAxisMax;

			while (dValue<dLowerAxisMax)
			{
				if(dLowerAxisMax>400)
				{
					dLowerAxisMax=dLowerAxisMax-200;
				}
				else if(dLowerAxisMax>200)
				{
					dLowerAxisMax=dLowerAxisMax-100;
				}
				else if(dLowerAxisMax>100)
				{
					dLowerAxisMax=100;
				}
				else
				{
					dTemp=50;
					if(dValue<dTemp)
						dLowerAxisMax=dTemp;

					break;
				}
			}

		}
		break;
	case TREND_VTE:
		{
			dLowerAxisMax=m_dYAxisMax;

			while (dValue<dLowerAxisMax)
			{
				/*if(dLowerAxisMax>1000)
				{
					dLowerAxisMax=dLowerAxisMax-100;
				}
				else if(dLowerAxisMax>500)
				{
					dLowerAxisMax=dLowerAxisMax-250;
				}
				else*/ if(dLowerAxisMax>100)
				{
					dLowerAxisMax=dLowerAxisMax-100;
				}
				else if(dLowerAxisMax>25)
				{
					dLowerAxisMax=dLowerAxisMax-25;
				}
				else if(dLowerAxisMax>10)
				{
					dLowerAxisMax=10;
				}
				else
				{
					dTemp=5;
					if(dValue<dTemp)
						dLowerAxisMax=dTemp;

					break;
				}
			}

		}
		break;
	case TREND_COMPLIANCE:
		{
			dLowerAxisMax=m_dYAxisMax;

			while (dValue<dLowerAxisMax)
			{
				if(dLowerAxisMax>1000)
				{
					dLowerAxisMax=dLowerAxisMax-200;
				}
				else if(dLowerAxisMax>250)
				{
					dLowerAxisMax=dLowerAxisMax-250;
				}
				else if(dLowerAxisMax>100)
				{
					dLowerAxisMax=100;
				}
				else
				{
					dTemp=50;
					if(dValue<dTemp)
						dLowerAxisMax=dTemp;

					break;
				}
			}
		}
		break;
	case TREND_PINSP:
	case TREND_PMEAN:
	case TREND_CO2HFO:
	case TREND_HFAMP:
	case TREND_RSBI: 
	case TREND_RESISTANCE:
	//case TREND_SPO2PR:
		{
			dLowerAxisMax=m_dYAxisMax;

			while (dValue<dLowerAxisMax)
			{
				//dTemp=dLowerAxisMax;
				if(dLowerAxisMax>5000)
				{
					dLowerAxisMax=dLowerAxisMax-2500;
				}
				else if(dLowerAxisMax>1000)
				{
					dLowerAxisMax=dLowerAxisMax-1000;
				}
				else if(dLowerAxisMax>500)
				{
					dLowerAxisMax=dLowerAxisMax-250;
				}
				else if(dLowerAxisMax>100)
				{
					dLowerAxisMax=dLowerAxisMax-100;
				}
				else if(dLowerAxisMax>25)
				{
					dLowerAxisMax=dLowerAxisMax-25;
				}
				else if(dLowerAxisMax>10)
				{
					dLowerAxisMax=10;
				}
				else
				{
					dTemp=5;
					if(dValue<dTemp)
						dLowerAxisMax=dTemp;

					break;
				}
			}
		}
		break;
	case TREND_MV:
		{
			dLowerAxisMax=m_dYAxisMax;

			while (dValue<dLowerAxisMax)
			{
				//dTemp=dLowerAxisMax;
				if(dLowerAxisMax>5000)
				{
					dLowerAxisMax=dLowerAxisMax-2500;
				}
				else if(dLowerAxisMax>1000)
				{
					dLowerAxisMax=dLowerAxisMax-1000;
				}
				else if(dLowerAxisMax>500)
				{
					dLowerAxisMax=dLowerAxisMax-250;
				}
				/*else if(dLowerAxisMax>100)
				{
					dLowerAxisMax=dLowerAxisMax-100;
				}
				else if(dLowerAxisMax>50)
				{
					dLowerAxisMax=dLowerAxisMax-25;
				}
				else if(dLowerAxisMax>25)
				{
					dLowerAxisMax=dLowerAxisMax-25;
				}
				else if(dLowerAxisMax>10)
				{
					dLowerAxisMax=10;
				}*/
				else//newtrend
				{
					dTemp=100;
					if(dValue<dTemp)
						dLowerAxisMax=dTemp;

					break;
				}
			}
		}
		break;
	case TREND_ETCO2:
		{
			dLowerAxisMax=m_dYAxisMax;

			while (dValue<dLowerAxisMax)
			{
				if(dLowerAxisMax>20)
				{
					dLowerAxisMax=dLowerAxisMax-20;
				}
				else if(dLowerAxisMax>10)
				{
					dLowerAxisMax=dLowerAxisMax-10;
				}
				else
				{
					dTemp=10;
					if(dValue<dTemp)
						dLowerAxisMax=dTemp;

					break;
				}
			}
		}
		break;
	case TREND_SPO2:
		{
			dLowerAxisMax=m_dYAxisMax;

			while (dValue<dLowerAxisMax)
			{
				if(dLowerAxisMax>20)
				{
					dLowerAxisMax=dLowerAxisMax-20;
				}
				else if(dLowerAxisMax>10)
				{
					dLowerAxisMax=dLowerAxisMax-10;
				}
				else
				{
					dTemp=10;
					if(dValue<dTemp)
						dLowerAxisMax=dTemp;

					break;
				}
			}
		}
		break;
	case TREND_SPO2PR:
		{
			dLowerAxisMax=m_dYAxisMax;

			while (dValue<dLowerAxisMax)
			{
				if(dLowerAxisMax>20)
				{
					dLowerAxisMax=dLowerAxisMax-20;
				}
				else if(dLowerAxisMax>10)
				{
					dLowerAxisMax=dLowerAxisMax-10;
				}
				else
				{
					dTemp=10;
					if(dValue<dTemp)
						dLowerAxisMax=dTemp;

					break;
				}
			}
		}
		break;
	case TREND_SPO2PI:
		{
			dLowerAxisMax=m_dYAxisMax;

			while (dValue<dLowerAxisMax)
			{
				if(dLowerAxisMax>100)
				{
					dLowerAxisMax=dLowerAxisMax-100;
				}
				else
				{
					dTemp=50;
					if(dValue<dTemp)
						dLowerAxisMax=dTemp;

					break;
				}
			}
		}
		break;
	case TREND_LEAK:
		{
			dLowerAxisMax=m_dYAxisMax;

			while (dValue<dLowerAxisMax)
			{
				if(dLowerAxisMax>20)
				{
					dLowerAxisMax=dLowerAxisMax-20;
				}
				else if(dLowerAxisMax>10)
				{
					dLowerAxisMax=dLowerAxisMax-10;
				}
				else
				{
					dTemp=5;
					if(dValue<dTemp)
						dLowerAxisMax=dTemp;

					break;
				}
			}
		}
		break;
	case TREND_SHAREMVMAND:
		{
			dLowerAxisMax=m_dYAxisMax;

			while (dValue<dLowerAxisMax)
			{
				if(dLowerAxisMax>20)
				{
					dLowerAxisMax=dLowerAxisMax-20;
				}
				else
				{
					dTemp=10;
					if(dValue<dTemp)
						dLowerAxisMax=dTemp;

					break;
				}
			}
		}
		break;
	case TREND_FREQUENCY:
		{
			dLowerAxisMax=m_dYAxisMax;

			while (dValue<dLowerAxisMax)
			{
				if(dLowerAxisMax>20)
				{
					dLowerAxisMax=dLowerAxisMax-20;
				}
				else if(dLowerAxisMax>10)
				{
					dLowerAxisMax=dLowerAxisMax-10;
				}
				else
				{
					dTemp=10;
					if(dValue<dTemp)
						dLowerAxisMax=dTemp;

					break;
				}
			}
		}
		break;
	}

	if(dLowerAxisMax>m_pFunctionParams->yMax || dLowerAxisMax==0)
		dLowerAxisMax=m_pFunctionParams->yMax;

	return dLowerAxisMax;

}

// **************************************************************************
// 
// **************************************************************************
bool CTrendDiagramm::DecreaseYScaleToNextValue()
{
	bool bResult=false;
	double dHigherYAxisMax=GetNextLowerYAxisMax();

	if(m_dYAxisMax!=dHigherYAxisMax)
	{
		bResult=true;
		SetYAxisScale(GetNegativeYoffset(dHigherYAxisMax), dHigherYAxisMax, true);
		switch(m_iTrendType)
		{
		case TREND_PINSP:
			{
				getModel()->getCONFIG()->TrendSetPInspMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_PMEAN:
			{
				getModel()->getCONFIG()->TrendSetPMeanMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_FIO2:
			{
				getModel()->getCONFIG()->TrendSetFIO2Max((int)dHigherYAxisMax);
			}
			break;
		case TREND_VTE:
			{
				getModel()->getCONFIG()->TrendSetVteMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_COMPLIANCE:
			{
				getModel()->getCONFIG()->TrendSetComplianceMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_CO2HFO:
			{
				getModel()->getCONFIG()->TrendSetDCO2Max((int)dHigherYAxisMax);
			}
			break;
		case TREND_MV:
			{
				getModel()->getCONFIG()->TrendSetMVMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_HFAMP:
			{
				getModel()->getCONFIG()->TrendSetHFAMPMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_RSBI:
			{
				getModel()->getCONFIG()->TrendSetRSBIMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_SHAREMVMAND:
			{
				getModel()->getCONFIG()->TrendSetSHAREMVMANDMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_RESISTANCE:
			{
				getModel()->getCONFIG()->TrendSetRESISTANCEMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_LEAK:
			{
				getModel()->getCONFIG()->TrendSetLEAKMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_SPO2:
			{
				getModel()->getCONFIG()->TrendSetSPO2Max((int)dHigherYAxisMax);
			}
			break;
		case TREND_SPO2PR:
			{
				getModel()->getCONFIG()->TrendSetSPO2PRMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_SPO2PI:
			{
				getModel()->getCONFIG()->TrendSetSPO2PIMax((int)dHigherYAxisMax);
			}
			break;
		case TREND_ETCO2:
			{
				getModel()->getCONFIG()->TrendSetETCO2Max((int)dHigherYAxisMax);
			}
			break;
		case TREND_FREQUENCY:
			{
				getModel()->getCONFIG()->TrendSetFREQUENCYMax((int)dHigherYAxisMax);
			}
			break;
		}
	}
	return bResult;
}
// **************************************************************************
// 
// **************************************************************************
double CTrendDiagramm::GetNextHigherYAxisMax()
{
	double dHigherAxisMax=0;

	switch(m_iTrendType)
	{
	case TREND_FIO2://newtrend
		{
			if(m_dYAxisMax<100)
			{
				dHigherAxisMax=100;
			}
			else if(m_dYAxisMax<200)
			{
				dHigherAxisMax=200;
			}
			else if(m_dYAxisMax<400)
			{
				dHigherAxisMax=m_dYAxisMax+100;
			}
			else
			{
				dHigherAxisMax=m_dYAxisMax+200;
			}
		}
		break;
	case TREND_VTE://newtrend
		{
			if(m_dYAxisMax<10)
			{
				dHigherAxisMax=10;
			}
			else if(m_dYAxisMax<25)
			{
				dHigherAxisMax=25;
			}
			else if(m_dYAxisMax<100)
			{
				dHigherAxisMax=m_dYAxisMax+25;
			}
			else if(m_dYAxisMax<500)
			{
				dHigherAxisMax=m_dYAxisMax+100;
			}
			else if(m_dYAxisMax<1000)
			{
				dHigherAxisMax=m_dYAxisMax+250;
			}
			else
			{
				dHigherAxisMax=m_dYAxisMax+500;
			}
		}
		break;
	case TREND_COMPLIANCE:
		{
			if(m_dYAxisMax<100)
			{
				dHigherAxisMax=100;
			}
			else if(m_dYAxisMax<250)
			{
				dHigherAxisMax=250;
			}
			else if(m_dYAxisMax<1000)
			{
				dHigherAxisMax=m_dYAxisMax+250;
			}
		}
		break;
	case TREND_PMEAN:
	case TREND_CO2HFO:
	case TREND_PINSP:
	case TREND_MV:
	case TREND_HFAMP:
	case TREND_RSBI: 
	case TREND_RESISTANCE:
	//case TREND_SPO2PR:
		{
			if(m_dYAxisMax<10)
			{
				dHigherAxisMax=10;
			}
			else if(m_dYAxisMax<25)
			{
				dHigherAxisMax=25;
			}
			else if(m_dYAxisMax<100)
			{
				dHigherAxisMax=m_dYAxisMax+25;
			}
			else if(m_dYAxisMax<500)
			{
				dHigherAxisMax=m_dYAxisMax+100;
			}
			else if(m_dYAxisMax<1000)
			{
				dHigherAxisMax=m_dYAxisMax+250;
			}
			else if(m_dYAxisMax<5000)
			{
				dHigherAxisMax=m_dYAxisMax+1000;
			}
			else
			{
				dHigherAxisMax=m_dYAxisMax+2500;
			}
		}
		break;
	case TREND_ETCO2:
		{
			if(m_dYAxisMax<10)
			{
				dHigherAxisMax=10;
			}
			else if(m_dYAxisMax<20)
			{
				dHigherAxisMax=m_dYAxisMax+10;
			}
			else
			{
				dHigherAxisMax=m_dYAxisMax+20;
			}
		}
		break;
	case TREND_SPO2:
		{
			if(m_dYAxisMax<10)
			{
				dHigherAxisMax=10;
			}
			else if(m_dYAxisMax<20)
			{
				dHigherAxisMax=m_dYAxisMax+10;
			}
			else
			{
				dHigherAxisMax=m_dYAxisMax+20;
			}
		}
		break;
	case TREND_SPO2PR:
		{
			if(m_dYAxisMax<10)
			{
				dHigherAxisMax=10;
			}
			else if(m_dYAxisMax<20)
			{
				dHigherAxisMax=m_dYAxisMax+10;
			}
			else
			{
				dHigherAxisMax=m_dYAxisMax+20;
			}
		}
		break;
	case TREND_SPO2PI:
		{
			if(m_dYAxisMax<50)
			{
				dHigherAxisMax=50;
			}
			else if(m_dYAxisMax<100)
			{
				dHigherAxisMax=100;
			}
			else
			{
				dHigherAxisMax=m_dYAxisMax+100;
			}
		}
		break;
	case TREND_LEAK:
		{
			if(m_dYAxisMax<10)
			{
				dHigherAxisMax=10;
			}
			else if(m_dYAxisMax<20)
			{
				dHigherAxisMax=m_dYAxisMax+10;
			}
			else
			{
				dHigherAxisMax=m_dYAxisMax+20;
			}
		}
		break;
	case TREND_SHAREMVMAND:
		{
			if(m_dYAxisMax<20)
			{
				dHigherAxisMax=20;
			}
			else if(m_dYAxisMax<100)
			{
				dHigherAxisMax=m_dYAxisMax+20;
			}
			else
			{
				dHigherAxisMax=m_pFunctionParams->yMax;;
			}
		}
		break;
	case TREND_FREQUENCY:
		{
			if(m_dYAxisMax<10)
			{
				dHigherAxisMax=10;
			}
			else if(m_dYAxisMax<20)
			{
				dHigherAxisMax=m_dYAxisMax+10;
			}
			else
			{
				dHigherAxisMax=m_dYAxisMax+20;
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
bool CTrendDiagramm::CanDecreaseYScale()
{
	bool bRes=false;
	double dLowerAxisMax=0;
	//double dHigherAxisMin=0;


	switch(m_iTrendType)
	{
	case TREND_FIO2://newtrend
		{
			if(m_dYAxisMax>400)
			{
				dLowerAxisMax=m_dYAxisMax-200;
			}
			else if(m_dYAxisMax>200)
			{
				dLowerAxisMax=m_dYAxisMax-100;
			}
			else if(m_dYAxisMax>100)
			{
				dLowerAxisMax=100;
			}
			else
			{
				dLowerAxisMax=50;
			}
		}
		break;
	case TREND_VTE:
		{
			/*if(m_dYAxisMax>1000)
			{
				dLowerAxisMax=m_dYAxisMax-100;
			}
			else if(m_dYAxisMax>500)
			{
				dLowerAxisMax=m_dYAxisMax-250;
			}
			else*/ if(m_dYAxisMax>100)
			{
				dLowerAxisMax=m_dYAxisMax-100;
			}
			else if(m_dYAxisMax>25)
			{
				dLowerAxisMax=m_dYAxisMax-25;
			}
			else if(m_dYAxisMax>10)
			{
				dLowerAxisMax=10;
			}
			else
			{
				dLowerAxisMax=5;
			}
		}
		break;
	case TREND_COMPLIANCE:
		{
			if(m_dYAxisMax>1000)
			{
				dLowerAxisMax=m_dYAxisMax-200;
			}
			else if(m_dYAxisMax>250)
			{
				dLowerAxisMax=m_dYAxisMax-250;
			}
			else if(m_dYAxisMax>100)
			{
				dLowerAxisMax=100;
			}
			else
			{
				dLowerAxisMax=50;
			}

		}
		break;
	case TREND_PINSP:
	case TREND_PMEAN:
	case TREND_CO2HFO:
	case TREND_HFAMP:
	case TREND_RSBI: 
	case TREND_RESISTANCE:
	//case TREND_SPO2PR:
		{
			if(m_dYAxisMax>5000)
			{
				dLowerAxisMax=m_dYAxisMax-2500;
			}
			else if(m_dYAxisMax>1000)
			{
				dLowerAxisMax=m_dYAxisMax-1000;
			}
			else if(m_dYAxisMax>500)
			{
				dLowerAxisMax=m_dYAxisMax-250;
			}
			else if(m_dYAxisMax>100)
			{
				dLowerAxisMax=m_dYAxisMax-100;
			}
			else if(m_dYAxisMax>25)
			{
				dLowerAxisMax=m_dYAxisMax-25;
			}
			else if(m_dYAxisMax>10)
			{
				dLowerAxisMax=10;
			}
			else
			{
				dLowerAxisMax=5;
			}
		}
		break;
	case TREND_MV:
		{
			if(m_dYAxisMax>5000)
			{
				dLowerAxisMax=m_dYAxisMax-2500;
			}
			else if(m_dYAxisMax>1000)
			{
				dLowerAxisMax=m_dYAxisMax-1000;
			}
			else if(m_dYAxisMax>500)
			{
				dLowerAxisMax=m_dYAxisMax-250;
			}
			/*else if(m_dYAxisMax>100)
			{
				dLowerAxisMax=m_dYAxisMax-100;
			}
			else if(m_dYAxisMax>50)
			{
				dLowerAxisMax=m_dYAxisMax-25;
			}
			else if(m_dYAxisMax>10)
			{
				dLowerAxisMax=10;
			}*/
			else//newtrend
			{
				dLowerAxisMax=100;
			}
		}
		break;
	case TREND_ETCO2:
		{
			if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-20;
			}
			else
			{
				dLowerAxisMax=10;
			}
		}
		break;
	case TREND_SPO2:
		{
			if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-20;
			}
			else
			{
				dLowerAxisMax=10;
			}
		}
		break;
	case TREND_SPO2PR:
		{
			if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-20;
			}
			else
			{
				dLowerAxisMax=10;
			}
		}
		break;
	case TREND_SPO2PI:
		{
			if(m_dYAxisMax>100)
			{
				dLowerAxisMax=m_dYAxisMax-100;
			}
			else
			{
				dLowerAxisMax=50;
			}
		}
		break;
	case TREND_LEAK:
		{
			if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-20;
			}
			else if(m_dYAxisMax>10)
			{
				dLowerAxisMax=m_dYAxisMax-10;
			}
			else
			{
				dLowerAxisMax=5;
			}
		}
		break;
	case TREND_SHAREMVMAND:
		{
			if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-20;
			}
			else
			{
				dLowerAxisMax=10;
			}
		}
		break;
	case TREND_FREQUENCY:
		{
			if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-20;
			}
			else
			{
				dLowerAxisMax=10;
			}
		}
		break;
	}

	if(		m_dMaxY<dLowerAxisMax
		&&	m_dYAxisMax>dLowerAxisMax
		&&	m_dMaxY!=0
		&& m_pFunctionParams->yMin < dLowerAxisMax)//newtrend
	{
		bRes=true;
	}
	return bRes;
}
// **************************************************************************
// 
// **************************************************************************
double CTrendDiagramm::GetNextLowerYAxisMax()
{
	double dLowerAxisMax=0;

	switch(m_iTrendType)
	{
	case TREND_FIO2://newtrend
		{
			if(m_dYAxisMax>400)
			{
				dLowerAxisMax=m_dYAxisMax-200;
			}
			else if(m_dYAxisMax>200)
			{
				dLowerAxisMax=m_dYAxisMax-100;
			}
			else if(m_dYAxisMax>100)
			{
				dLowerAxisMax=100;
			}
			else
			{
				dLowerAxisMax=50;
			}
		}
		break;
	case TREND_VTE://newtrend
		{
			if(m_dYAxisMax>1000)
			{
				dLowerAxisMax=m_dYAxisMax-500;
			}
			else if(m_dYAxisMax>500)
			{
				dLowerAxisMax=m_dYAxisMax-250;
			}
			else if(m_dYAxisMax>100)
			{
				dLowerAxisMax=m_dYAxisMax-100;
			}
			else if(m_dYAxisMax>25)
			{
				dLowerAxisMax=m_dYAxisMax-25;
			}
			else if(m_dYAxisMax>10)
			{
				dLowerAxisMax=10;
			}
			else
			{
				dLowerAxisMax=5;
			}
		}
		break;
	case TREND_COMPLIANCE:
		{
			if(m_dYAxisMax>1000)
			{
				dLowerAxisMax=m_dYAxisMax-200;
			}
			else if(m_dYAxisMax>250)
			{
				dLowerAxisMax=m_dYAxisMax-250;
			}
			else if(m_dYAxisMax>100)
			{
				dLowerAxisMax=100;
			}
			else
			{
				dLowerAxisMax=50;
			}
		}
		break;
	case TREND_PINSP:
	case TREND_PMEAN:
	case TREND_CO2HFO:
	case TREND_MV:
	case TREND_HFAMP:
	case TREND_RSBI: 
	case TREND_RESISTANCE:
	//case TREND_SPO2PR:
		{
			if(m_dYAxisMax>5000)
			{
				dLowerAxisMax=m_dYAxisMax-2500;
			}
			else if(m_dYAxisMax>1000)
			{
				dLowerAxisMax=m_dYAxisMax-1000;
			}
			else if(m_dYAxisMax>500)
			{
				dLowerAxisMax=m_dYAxisMax-250;
			}
			else if(m_dYAxisMax>100)
			{
				dLowerAxisMax=m_dYAxisMax-100;
			}
			else if(m_dYAxisMax>25)
			{
				dLowerAxisMax=m_dYAxisMax-25;
			}
			else if(m_dYAxisMax>10)
			{
				dLowerAxisMax=10;
			}
			else
			{
				dLowerAxisMax=5;
			}
		}
		break;
	case TREND_ETCO2:
		{
			if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-20;
			}
			else
			{
				dLowerAxisMax=10;
			}
		}
		break;
	case TREND_SPO2:
		{
			if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-20;
			}
			else
			{
				dLowerAxisMax=10;
			}
		}
		break;
	case TREND_SPO2PR:
		{
			if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-20;
			}
			else
			{
				dLowerAxisMax=10;
			}
		}
		break;
	case TREND_SPO2PI:
		{
			if(m_dYAxisMax>100)
			{
				dLowerAxisMax=m_dYAxisMax-100;
			}
			else
			{
				dLowerAxisMax=50;
			}
		}
		break;
	case TREND_LEAK:
		{
			if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-20;
			}
			else if(m_dYAxisMax>10)
			{
				dLowerAxisMax=m_dYAxisMax-10;
			}
			else
			{
				dLowerAxisMax=5;
			}
		}
		break;
	case TREND_SHAREMVMAND:
		{
			if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-20;
			}
			else
			{
				dLowerAxisMax=10;
			}
		}
		break;
	case TREND_FREQUENCY:
		{
			if(m_dYAxisMax>20)
			{
				dLowerAxisMax=m_dYAxisMax-20;
			}
			else
			{
				dLowerAxisMax=10;
			}
		}
		break;
	}

	if(dLowerAxisMax<m_pFunctionParams->yMin || dLowerAxisMax==0)
		dLowerAxisMax=m_pFunctionParams->yMin;

	return dLowerAxisMax;

}



///////////////// PlotPoints  /////////////////////////////////////////////
/*																		///
Each Function e.g. DoSineX,DoPlotXY, etc, calls this function as	///
they calculate each point so that each point can be drawn on the	///
Takes:																///
UINT x, UNIT y - the graph co-ord of the point just calculeted		///
(current point).													///
UINT prevx, UINT prevy - the co-ords of the previous point			///
This routine checks what type of plot (line, dot, or bar) is		///
required and calls the appropriate routine							///
*/																		///
///////////////////////////////////////////////////////////////////////////
//void CTrendDiagramm::PlotPoints(UINT x, UINT y, UINT prevx, UINT prevy)
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
//	case G_FILLEDCHART:
//		{
//			//DrawFilledLine(prevx,prevy,x,y);
//			break;
//		}
//	/*case G_BARCHART:
//		{
//			DrawBar(x,y);
//			break;
//		}*/
//	case G_DOTCHART:
//	default:
//		{
//			DrawDot(x,y);
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
void CTrendDiagramm::PlotPointArray(CPoint *pts, int iSize)
{
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
	case G_FILLEDCHART:
		{
			DrawFilledLine(pts,iSize);
			break;
		}
	default:
		{
			//DrawDot(x,y);
			break;
		}
	}//SWITCH

	return;
}




///////////////////// DrawDot /////////////////////////////////////////////
/*																		///
For the Dot chart type - this routine plots the pixel at the given	///
point																///
*/																		///
///////////////////////////////////////////////////////////////////////////
//void CTrendDiagramm::DrawDot(UINT X, UINT Y)
//{
//	CClientDC dc(this);
//	
//	dc.SetPixel(X,Y,m_crGraphPenColor);
//}


//////////////////////////// DrawConnectLine //////////////////////////////////
/*																			///
For the line chat type, this routine draws a line between previous		///
point (FROM) and current point (TO)										///
*/																			///
///////////////////////////////////////////////////////////////////////////////
void CTrendDiagramm::DrawConnectLine(UINT FromX, UINT FromY, UINT ToX, UINT ToY)
{
	//draws a connecting line between to pixels
	//using the graphpen color
	// 
	CPen pen, *poldpen;
	pen.CreatePen(PS_SOLID,m_iGraphPenSize,m_crGraphPenColor);
	CClientDC dc(this);
	poldpen=dc.SelectObject(&pen);

	//BitBlt(dc.m_hDC, ToX, 0, 2,m_iGraphHeight, m_hdcStatic,  ToX, 0, SRCCOPY);

	dc.MoveTo(FromX,FromY);
	dc.LineTo(ToX,ToY);

	dc.SelectObject(poldpen);

	pen.DeleteObject();
}

// **************************************************************************
// For the line chat type, this routine draws a line between previous	
//				point (FROM) and current point (TO)
// **************************************************************************
void CTrendDiagramm::DrawConnectLine(CPoint *pts, int iSize)
{
	CClientDC dc(this);
	CDC* pDCGraph=CDC::FromHandle(m_hdcGraph);
	CPen pen, *poldpen;
	pen.CreatePen(PS_SOLID,2,m_crGraphPenColor);
	//pen.CreatePen(PS_SOLID,m_iGraphPenSize,m_crGraphPenColor);
	HBRUSH holdbrush;

	poldpen=pDCGraph->SelectObject(&pen);
	holdbrush=(HBRUSH)pDCGraph->SelectObject((HBRUSH)GetStockObject(NULL_BRUSH));

	BitBlt(m_hdcGraph, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcStatic, 0, 0, SRCCOPY);

	pDCGraph->Polyline(pts, iSize);

	BitBlt(dc.m_hDC, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcGraph, 0, 0, SRCCOPY);

	pDCGraph->SelectObject(poldpen);
	pDCGraph->SelectObject(holdbrush);

	pen.DeleteObject();
}

// **************************************************************************
// For the line chat type, this routine draws a line between previous	
//				point (FROM) and current point (TO)
// **************************************************************************
void CTrendDiagramm::DrawFilledLine(CPoint *pts, int iSize)
{
	CPen pen, *poldpen;
	pen.CreatePen(PS_SOLID,m_iGraphPenSize,m_crGraphPenColor);

	CBrush brush, *poldbrush;
	brush.CreateSolidBrush(m_crGraphPenColor);

	CClientDC dc(this);
	CDC* pDCGraph=CDC::FromHandle(m_hdcGraph);

	poldbrush=pDCGraph->SelectObject(&brush);

	BitBlt(m_hdcGraph, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcStatic, 0, 0, SRCCOPY);

	poldpen=pDCGraph->SelectObject(&pen);

	pDCGraph->Polygon(pts, iSize);

	BitBlt(dc.m_hDC, 0, 0, m_iGraphWidth,m_iGraphHeight, m_hdcGraph, 0, 0, SRCCOPY);

	pDCGraph->SelectObject(poldpen);
	pDCGraph->SelectObject(poldbrush);

	pen.DeleteObject();
}


//////////////// ConstrainY  //////////////////////////////////////////////
/*																		///
This function makes sure that a value of Y plot valuestays within	///
stays within the plot area.											///
Input:																///
double Y value														///
Returns:															///
double y constrained to the plotting area							///
*/																		///
///////////////////////////////////////////////////////////////////////////
double CTrendDiagramm::ConstrainY(double y)
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

//////////////  GetMultiPlotPenColor //////////////////////////////////////////
/*																			///
For multiply xy graphs we cycle through a series of pens for each graph	///
Input:																	///
UINT PenNumber (synonymous with GraphNumber)							///
Returns:																///
COLORREF of next pen in the cycle										///
*/																			///
///////////////////////////////////////////////////////////////////////////////
//COLORREF CTrendDiagramm::GetMultiPlotPenColor(UINT PenNumber)
//{
//	//keep the pen within range
//	PenNumber=PenNumber%G_NUM_MULTIPLOT_COLORS;
//	switch(PenNumber)
//	{
//	case 0:
//		return G_MULTIPLOT_COLOR_ONE;
//		break;
//	case 1:
//		return G_MULTIPLOT_COLOR_TWO;
//		break;
//	case 2:
//		return G_MULTIPLOT_COLOR_THREE;
//		break;
//	case 3:
//		return G_MULTIPLOT_COLOR_FOUR;
//		break;
//	case 4:
//		return G_MULTIPLOT_COLOR_FIVE;
//		break;
//	case 5:
//		return G_MULTIPLOT_COLOR_SIX;
//		break;
//	case 6:
//		return G_MULTIPLOT_COLOR_SEVEN;
//		break;
//	case 7:
//		return G_MULTIPLOT_COLOR_EIGHT;
//		break;
//	default:
//		return G_MULTIPLOT_COLOR_ONE;
//	}
//
//}
// **************************************************************************
// 
// **************************************************************************
void CTrendDiagramm::SetAutofit(bool state)
{
	m_bAutofit=state;
}

// **************************************************************************
// 
// **************************************************************************
void CTrendDiagramm::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

}

// **************************************************************************
// 
// **************************************************************************
void CTrendDiagramm::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	//if(m_bAutofit==false)
	{
		m_bSelected = false;

	}
}

// **************************************************************************
// 
// **************************************************************************
LRESULT CTrendDiagramm::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_MENUBUTTONDOWN:
		{

			switch(wParam)
			{
			case IDC_CURSORBTN_UP:
				{
					//DEBUGMSG(TRUE, (TEXT("WM_MENUBUTTONDOWN IDC_CURSORBTN_UP\r\n")));

					//m_bCursorBtnPushed=true;
					/*CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_CURSOR_UP);
					getModel()->Trigger(&event);*/

					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_CURSOR_UP);

					//SetTimer(TRENDUP, 800, NULL);

					return 1;
				}
				break;
			case IDC_CURSORBTN_DOWN:
				{
					//DEBUGMSG(TRUE, (TEXT("WM_MENUBUTTONDOWN IDC_CURSORBTN_DOWN\r\n")));

					//m_bCursorBtnPushed=true;

					/*CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_CURSOR_DOWN);
					getModel()->Trigger(&event);*/

					if(AfxGetApp() != NULL)
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_CURSOR_DOWN);

					//SetTimer(TRENDDOWN, 800, NULL);

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
	/*case WM_MENUBUTTONUP:
		{
			switch(wParam)
			{
			case IDC_CURSORBTN_UP:
				{
					m_bCursorBtnPushed=false;

					KillTimer(TRENDUP);
					return 1;
				}
				break;
			case IDC_CURSORBTN_DOWN:
				{
					m_bCursorBtnPushed=false;

					KillTimer(TRENDDOWN);
					return 1;
				}
				break;
			default:
				{

				}
				break;
			}

		}
		break;*/
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

// **************************************************************************
// 
// **************************************************************************
//void CTrendDiagramm::OnTimer(UINT_PTR nIDEvent)
//{
//	if(nIDEvent==TRENDUP)
//	{
//		if(AfxGetApp() != NULL)
//			AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_CURSOR_UP);
//	}
//	else if(nIDEvent==TRENDDOWN)
//	{
//		if(AfxGetApp() != NULL)
//			AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_CURSOR_DOWN);
//	}
//
//	CWnd::OnTimer(nIDEvent);
//}

// **************************************************************************
// 
// **************************************************************************
BOOL CTrendDiagramm::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	//case WM_MOUSEMOVE:
	//	{
	//		switch(pMsg->wParam)
	//		{
	//		case MK_LBUTTON:
	//			{
	//				int xPos = GET_X_LPARAM(pMsg->lParam); 
	//				int yPos = GET_Y_LPARAM(pMsg->lParam);

	//				DEBUGMSG(TRUE, (TEXT("WM_MOUSEMOVE CTrendDiagramm xPos: %d yPos: %d\r"),xPos,yPos));

	//				if(m_bCursorBtnPushed)
	//				{
	//					DEBUGMSG(TRUE, (TEXT("WM_MOUSEMOVE m_bCursorBtnPushed\r\n")));
	//				}

	//				//if(xPos>m_iXStartPos+10)
	//				//{
	//				//	//TRACE(_T("WM_MOUSEMOVE right xStartPos: %d yStartPos: %d xPos: %d yPos: %d\r"),m_iXStartPos,m_iYStartPos,xPos,yPos);
	//				//	m_iXStartPos = xPos;
	//				//	m_iYStartPos = yPos;

	//				//	/*if(m_iDiagrammType==GRAPH_PRESSURE_HF)
	//				//	{
	//				//		CMVEventMatrix event(CMVEventMatrix::EV_BN_MATRIX_CURSOR_RIGHT);
	//				//		GetModel()->Trigger(&event);

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
	//				//		GetModel()->Trigger(&event);

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
	//				//			GetModel()->getCONFIG()->GraphSetPressureMaxHF(m_dYAxisMax);
	//				//			GetModel()->getCONFIG()->GraphSetPressureMinHF(m_dYAxisMin);

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
	//				//		GetModel()->getCONFIG()->GraphSetPressureMaxHF(m_dYAxisMax);
	//				//		GetModel()->getCONFIG()->GraphSetPressureMinHF(m_dYAxisMin);

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
	case WM_LBUTTONDOWN:
		{
			if(!m_bSelected)
			{
				//if(m_bAutofit==false)
				{
					m_bSelected = true;
					if(GetParent())
						GetParent()->PostMessage( WM_TREND_SETFOCUS, GetDlgCtrlID(), 0);
					//SetFocus();
				}
				
			}
			else
			{
				if(GetParent())
					GetParent()->PostMessage( WM_TREND_SETFOCUS, GetDlgCtrlID(), 0);
			}
		}
		break;
	case WM_KEYDOWN:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				return 1;
			}
			else if(pMsg->wParam==VK_UP)
			{
				if(GetParent())
					GetParent()->PostMessage(WM_TREND_CURSOR_RIGHT, GetDlgCtrlID(), 0);
				return 1;
			}
			else if(pMsg->wParam==VK_DOWN)
			{
				
				if(GetParent())
					GetParent()->PostMessage(WM_TREND_CURSOR_LEFT, GetDlgCtrlID(), 0);
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


void CTrendDiagramm::SetFirstpoint(bool state, UINT prevx)
{
	m_bFirstpoint=state;
	if(state==false)
		m_prevx=prevx;
}


UINT CTrendDiagramm::GetPrevXval()
{
	//int iX=m_prevx;
	return m_prevx;
}

int CTrendDiagramm::GetOffsetMinutes()
{
	return (int)m_tsOffsetStartTime.GetTotalMinutes();
}


//COleDateTime CTrendDiagramm::GetOffsetTime()
//{
//	return m_dtOffsetTime;
//}

COleDateTime CTrendDiagramm::GetStartTime()
{
	return m_dtStartTime;
}

int CTrendDiagramm::GetMoveSpan()
{
	int iRes=0;
	int iTrendSpan=(int)GetTrendSpan();
	int iTickSpace=(int)GetXAxisTickSpace();

	try
	{
		if(iTickSpace!=0)
			iRes=((iTrendSpan/iTickSpace)-1)*iTickSpace;
	}
	catch (...)
	{
		theApp.ReportErrorException(_T("CTrendDiagramm::GetMoveSpan"));
	}
	return iRes;
}

bool CTrendDiagramm::MoveToPos(int iPos)
{
	//int iCurOffsetStartTime=m_tsOffsetStartTime.GetTotalMinutes();
	int iTrendSpan=(int)GetTrendSpan();
	//int iMoveSpan=GetMoveSpan();
	//int iTickSpace=GetXAxisTickSpace();
	int iNewOffsetStartTime=0;

	if(iPos+(iTrendSpan/2)>=MAX_TREND_MINUTES)//left
	{
		//move to left end
		iNewOffsetStartTime=MAX_TREND_MINUTES-iTrendSpan;
		m_tsOffsetStartTime.SetDateTimeSpan(0,0,iNewOffsetStartTime,0);	//move to the end
	}
	else if(iPos-(iTrendSpan/2)<0)//right
	{
		//move to right end
		m_tsOffsetStartTime.SetDateTimeSpan(0,0,0,0);	//move to the beginning
	}
	else
	{
		iNewOffsetStartTime=iPos-(iTrendSpan/2);
		m_tsOffsetStartTime.SetDateTimeSpan(0,0,iNewOffsetStartTime,0);	//move to pos
	}

	ClearFunction();


	return true;
}

bool CTrendDiagramm::MoveLeft(UINT iSpan)
{
	int iCurOffsetStartTime=(int)m_tsOffsetStartTime.GetTotalMinutes();
	int iTrendSpan=(int)GetTrendSpan();
	//int iTickSpace=GetXAxisTickSpace();
	int iMoveSpan=GetMoveSpan()*iSpan;
	int iNewOffsetStartTime=0;

	if(iCurOffsetStartTime+iTrendSpan>=MAX_TREND_MINUTES)
	{
		return false;	//allready at the end
	}
	else if(iCurOffsetStartTime+iTrendSpan+iMoveSpan>MAX_TREND_MINUTES)
	{
		iMoveSpan=MAX_TREND_MINUTES-(iCurOffsetStartTime+iTrendSpan);
		iNewOffsetStartTime=iMoveSpan+iCurOffsetStartTime;

		m_tsOffsetStartTime.SetDateTimeSpan(0,0,iNewOffsetStartTime,0);	//move to the end
	}
	else
	{
		iNewOffsetStartTime=iMoveSpan+iCurOffsetStartTime;
		m_tsOffsetStartTime.SetDateTimeSpan(0,0,iNewOffsetStartTime,0);	//move time span
	}

	ClearFunction();


	return true;
}


bool CTrendDiagramm::MoveRight(UINT iSpan)
{
	int iCurOffsetStartTime=(int)m_tsOffsetStartTime.GetTotalMinutes();
	//int iTrendSpan=(int)GetTrendSpan();
	//int iTickSpace=GetXAxisTickSpace();
	int iMoveSpan=GetMoveSpan()*iSpan;
	int iNewOffsetStartTime=0;

	if(iCurOffsetStartTime==0)
	{
		return false;	//allready at the beginning (now)
	}
	else if(iCurOffsetStartTime-iMoveSpan<0)
	{
		m_tsOffsetStartTime.SetDateTimeSpan(0,0,0,0);	//move to the beginning
	}
	else
	{
		iNewOffsetStartTime=iCurOffsetStartTime-iMoveSpan;
		m_tsOffsetStartTime.SetDateTimeSpan(0,0,iNewOffsetStartTime,0);	//move time span
	}

	ClearFunction();


	return true;
}
int CTrendDiagramm::IncreaseTime()
{
	//int iTrendSpan=(int)GetTrendSpan();

	int iTrendSpan=GetNextIncreaseTrendSpan();
	if(iTrendSpan==0)
		return iTrendSpan;

	SetTrendSpan(iTrendSpan);

	ClearFunction();

	return iTrendSpan;

}
int CTrendDiagramm::DecreaseTime()
{
	//int iTrendSpan=(int)GetTrendSpan();

	int iTrendSpan=GetNextDecreaseTrendSpan();
	if(iTrendSpan==0)
		return iTrendSpan;

	SetTrendSpan(iTrendSpan);

	ClearFunction();

	return iTrendSpan;
}

int CTrendDiagramm::GetNextDecreaseTrendSpan()
{
	int iTrendSpan=(int)GetTrendSpan();

	switch(iTrendSpan)
	{
	case TREND_30MIN:
		iTrendSpan=TREND_1HOUR;
		break;
	case TREND_1HOUR:
		iTrendSpan=TREND_2HOURS;
		break;
	case TREND_2HOURS:
		iTrendSpan=TREND_4HOURS;
		break;
	case TREND_4HOURS:
		iTrendSpan=TREND_8HOURS;
		break;
	case TREND_8HOURS:
		iTrendSpan=TREND_16HOURS;
		break;
	case TREND_16HOURS:
		iTrendSpan=TREND_1DAY4HOURS;
		break;
	case TREND_1DAY4HOURS:
		iTrendSpan=TREND_3DAYS;
		break;
	case TREND_3DAYS:
		iTrendSpan=TREND_5DAYS;
		break;
	case TREND_5DAYS:
		//iTrendSpan=TREND_10DAYS;
		iTrendSpan=0;
		break;
	//case TREND_10DAYS:
	//	//iTrendSpan=TREND_20DAYS;
	//	iTrendSpan=0;
	//	break;
	//case TREND_20DAYS:
	default:
		iTrendSpan=0;
		break;
	}

	return iTrendSpan;
}

int CTrendDiagramm::GetNextIncreaseTrendSpan()
{
	int iTrendSpan=(int)GetTrendSpan();

	switch(iTrendSpan)
	{
	/*case TREND_20DAYS:
		iTrendSpan=TREND_10DAYS;
		break;*/
	/*case TREND_10DAYS:
		iTrendSpan=TREND_5DAYS;
		break;*/
	case TREND_5DAYS:
		iTrendSpan=TREND_3DAYS;
		break;
	case TREND_3DAYS:
		iTrendSpan=TREND_1DAY4HOURS;
		break;
	case TREND_1DAY4HOURS:
		iTrendSpan=TREND_16HOURS;
		break;
	case TREND_16HOURS:
		iTrendSpan=TREND_8HOURS;
		break;
	case TREND_8HOURS:
		iTrendSpan=TREND_4HOURS;
		break;
	case TREND_4HOURS:
		iTrendSpan=TREND_2HOURS;
		break;
	case TREND_2HOURS:
		iTrendSpan=TREND_1HOUR;
		break;
	case TREND_1HOUR:
		iTrendSpan=TREND_30MIN;
		break;
	case TREND_30MIN:
		iTrendSpan=0;
		break;
	default:
		iTrendSpan=0;
		break;
	}

	return iTrendSpan;
}

double CTrendDiagramm::GetTrendSpan()
{
	return m_dXAxisMax;
}

void CTrendDiagramm::SetTrendSpan(int iTrendSpan)
{
	SetXAxisScale(0, iTrendSpan);
	
	int iCurOffsetStartTime=(int)m_tsOffsetStartTime.GetTotalMinutes();

	if(iCurOffsetStartTime+iTrendSpan>=MAX_TREND_MINUTES)
		m_tsOffsetStartTime.SetDateTimeSpan(0,0,(MAX_TREND_MINUTES-iTrendSpan),0);

}

void CTrendDiagramm::RefreshLocalTime(COleDateTime dtStartTime, COleDateTime dtOffsetTime)
{
	m_dtOffsetTime=dtOffsetTime;
	m_dtStartTime=dtStartTime;
}

/***************************************************************************/
//     Purpose: This has many similarities with the PlotXY functions - it plots several
//				graphs on the same scale and axis.											
//				It loops (number of graphs) times - drawing each graph in it's	
//				entirety each time.									
//				Other:														
//				a. We will not bother plotting  if x < the graph scale XMin or > XMax	
//				b. Dot, bar, and line will be acceptable for the chart type.	
//				c. The usual y constraints apply								
//
//				The number of graphs required are givenn in the					
//				Const_1 fiend of the G_FUNCTIONSTRUCT
//**************************************************************************/
//void CDiagramm::DoMultiPlotXY()
//{
//	UINT prevx=0;
//	UINT prevy=0;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	double xperpixel= 1/m_dPixelsPerX;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
//
//	//how many graphs?
//	UINT num_graphs=m_pFunctionParams->Const_1;
//	//how may plot items per graph
//	UINT num_plotitems=m_pFunctionParams->num_PlotXYItems;
//	//save the default pen color
//	COLORREF savedpencolor=m_crGraphPenColor;
//	UINT PenCount=0;
//
//	for( long countgraphs=0;countgraphs < 
//		num_graphs*num_plotitems;countgraphs+=(long)num_plotitems)
//	{
//
//		//set the pencolor for the current graph
//		m_crGraphPenColor=GetMultiPlotPenColor(PenCount);
//
//		BOOL firstpoint=TRUE;//first time round each graph loop
//
//		for (UINT count =0; count < num_plotitems; count++)
//		{
//
//			double x=(m_pFunctionParams->pPlotXYItems+countgraphs)[count].pPlotXYItems->x;
//			double y=(m_pFunctionParams->pPlotXYItems+countgraphs)[count].pPlotXYItems->y;
//
//			//if x is off scale - don't bother
//			if ( (x < m_dXAxisMin) || (x > m_dXAxisMax))
//			{
//				continue; //NEXT !!!!!!
//			}
//
//			UINT pixelx= xstart+(x-m_dXAxisMin)/xperpixel;
//
//			y=ConstrainY(y);	
//			UINT pixely=ystart-(y-m_dYAxisMin)/yperpixel;
//
//			if(firstpoint)
//			{
//				prevx=pixelx;
//				prevy=pixely;
//				firstpoint=FALSE;
//			}
//			PlotPoints(pixelx,pixely,prevx,prevy);
//			//current point becomes previous point
//			prevx=pixelx;
//			prevy=pixely;
//
//		}//end this graph
//
//		PenCount++; //next pen
//	}//all graphs
//
//	//restore the graph pencolor
//	m_crGraphPenColor=savedpencolor;
//}

/***************************************************************************/
//     Purpose: There are two types of deviation graphs, namely percent and absolute
//				but hey both end up here to be sorted out
//**************************************************************************/
//void CDiagramm::DoDeviationPercentAbs()
//{
//	/* 
//	No X valuess are supplied. The x-axis represents
//	samples really
//	a. We will not bother plotting  if x < the graph scale XMin or > XMax
//	c. The usual y constraints apply
//	*/
//	UINT prevx=0;
//	UINT prevy=0;
//	BOOL firstpoint=TRUE;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
//
//	/*
//	We need to know when we will run off the right hand side
//	*/
//	CRect dataarea=CalcDataArea();
//
//	for (UINT count =0; count < m_pFunctionParams->num_PlotXYItems; count++)
//	{
//
//		double yval;//will hold percent or absolute devaition
//		double yset=m_pFunctionParams->pPlotXYItems[count].pPlotXYItems->x;
//		double yactual=m_pFunctionParams->pPlotXYItems[count].pPlotXYItems->y;
//		if( m_pFunctionParams->FuncType==G_PLOTDEV_PERCENT)
//		{
//			yval=(yactual-yset)/yset*100; //percentage of setpoint
//		}
//		else
//		{
//			yval=(yactual-yset); //difference
//		}
//
//
//		//if we reach the right hand side then we can stop
//
//		UINT pixelx= xstart+( (count/2)*m_dPixelsPerX);
//		if (pixelx > dataarea.right )
//		{
//			return; //no need to go any further
//		}
//
//
//		yval=ConstrainY(yval);	
//		UINT pixely=ystart-(yval-m_dYAxisMin)/yperpixel;
//
//		if(firstpoint)
//		{
//			prevx=pixelx;
//			prevy=pixely;
//			firstpoint=FALSE;
//		}
//		PlotPoints(pixelx,pixely,prevx,prevy);
//		//current point becomes previous point
//		prevx=pixelx;
//		prevy=pixely;
//
//	}
//}

/***************************************************************************/
//     Purpose: 
//**************************************************************************/
//void CDiagramm::DoLogeX()
//{
//	UINT prevx=0;
//	UINT prevy=0;
//	BOOL firstpoint=TRUE;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	double xperpixel= 1/m_dPixelsPerX;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
//
//	//how many steps are they (pixels) in the x-direction
//	UINT xspan=(m_dXAxisMax - m_dXAxisMin)*m_dPixelsPerX;
//
//	UINT step;
//	if(m_pFunctionParams->ChartType==G_BARCHART)
//	{
//		step=G_BARCHARTWIDTH;
//	}
//	else
//	{
//		step=1;
//	}
//
//	for (UINT count =0; count < xspan; count+=step)
//	{
//		//do the function
//		double y=log( m_dXAxisMin+(count*xperpixel));
//
//		UINT pixelx=xstart+count;
//
//		y=ConstrainY(y);	
//		UINT pixely=ystart-(y-m_dYAxisMin)/yperpixel;
//
//		if(firstpoint)
//		{
//			prevx=pixelx;
//			prevy=pixely;
//			firstpoint=FALSE;
//		}
//		PlotPoints(pixelx,pixely,prevx,prevy);
//		//current point becomes previous point
//		prevx=pixelx;
//		prevy=pixely;
//
//	}
//}


/***************************************************************************/
//     Purpose: This has many similarities with the other functions - however		
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
//**************************************************************************/
//void CDiagramm::DoPlotXY()
//{
//	//UINT prevx=0;
//	//UINT prevy=0;
//	//BOOL firstpoint=TRUE;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	double xperpixel= 1/m_dPixelsPerX;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
//
//	bool bChangeColor=false;
//	if(m_pModel->getDATAHANDLER()->CurModeIsTriggerMode())
//	{
//		if(m_pFunctionParams->bTrig && m_pFunctionParams->bATZ && !m_bTriggeredBreath)
//		{
//			bChangeColor=true;
//		}
//		else if(!m_pFunctionParams->bTrig && m_pFunctionParams->bATZ && m_bTriggeredBreath)
//		{
//			bChangeColor=true;
//		}
//	}
//	
//
//	for (UINT count =0; count < m_pFunctionParams->num_PlotXYItems; count++)
//	{
//
//		double x=m_pFunctionParams->pPlotXYItems[count].pPlotXYItems->x;
//		double y=m_pFunctionParams->pPlotXYItems[count].pPlotXYItems->y;
//
//		if(m_pFunctionParams->iValATZ==count && bChangeColor)
//			SetTriggeredBreath(!m_bTriggeredBreath);
//
//		//if x is off scale - don't bother
//		if ( (x < m_dXAxisMin) || (x > m_dXAxisMax))
//		{
//			continue; //NEXT !!!!!!
//		}
//
//		UINT pixelx= xstart+(x-m_dXAxisMin)/xperpixel;
//
//		y=ConstrainY(y);	
//		UINT pixely=ystart-(y-m_dYAxisMin)/yperpixel;
//
//		if(m_bFirstpoint)
//		{
//			m_prevx=pixelx;
//			m_prevy=pixely;
//			m_bFirstpoint=FALSE;
//		}
//
//		PlotPoints(pixelx,pixely,m_prevx,m_prevy);
//		//current point becomes previous point
//		m_prevx=pixelx;
//		m_prevy=pixely;
//
//	}
//}

/***************************************************************************/
//     Purpose: This draws the function ASinX
//**************************************************************************/
//void CDiagramm::DoSineX()
//{
//	//we loop round from the given xstart to xend
//	//plotting y=sinx
//	UINT prevx=0;
//	UINT prevy=0;
//	BOOL firstpoint=TRUE;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	double xperpixel= 1/m_dPixelsPerX;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//the left hand side of the graph plot area on screen
//
//	//how many steps (pixels) in the x-direction
//	UINT xspan=(m_dXAxisMax - m_dXAxisMin)*m_dPixelsPerX;
//
//	UINT step;
//	if(m_pFunctionParams->ChartType==G_BARCHART)
//	{
//		step=G_BARCHARTWIDTH;
//	}
//	else
//	{
//		step=1;
//	}
//
//	for (UINT count =0; count < xspan; count+=step)
//	{
//		//do the function
//		double y=m_pFunctionParams->Const_1*sin(m_dXAxisMin+count*xperpixel);
//
//		UINT pixelx=xstart+count;
//
//		y=ConstrainY(y);
//		UINT pixely=ystart-((y-m_dYAxisMin)/yperpixel);
//
//
//
//		if(firstpoint)
//		{
//			prevx=pixelx;
//			prevy=pixely;
//			firstpoint=FALSE;
//		}
//		PlotPoints(pixelx,pixely,prevx,prevy);
//		//current point becomes previous point
//		prevx=pixelx;
//		prevy=pixely;
//
//	}
//
//}

/***************************************************************************/
//     Purpose: Draws the function CosX	
//**************************************************************************/
//void CDiagramm::DoCosX()
//{
//	//we loop round from the given xstart to xend
//	UINT prevx=0;
//	UINT prevy=0;
//	BOOL firstpoint=TRUE;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	double xperpixel= 1/m_dPixelsPerX;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//the left hand side of the graph plot area on screen
//
//	//how many steps (pixels) in the x-direction
//	UINT xspan=(m_dXAxisMax - m_dXAxisMin)*m_dPixelsPerX;
//
//	UINT step;
//	if(m_pFunctionParams->ChartType==G_BARCHART)
//	{
//		step=G_BARCHARTWIDTH;
//	}
//	else
//	{
//		step=1;
//	}
//
//	for (UINT count =0; count < xspan; count+=step)
//	{
//		//do the function
//		double y=m_pFunctionParams->Const_1*cos(m_dXAxisMin+count*xperpixel);
//
//		UINT pixelx=xstart+count;
//
//		y=ConstrainY(y);
//		UINT pixely=ystart-((y-m_dYAxisMin)/yperpixel);
//
//		if(firstpoint)
//		{
//			prevx=pixelx;
//			prevy=pixely;
//			firstpoint=FALSE;
//		}
//		PlotPoints(pixelx,pixely,prevx,prevy);
//		//current point becomes previous point
//		prevx=pixelx;
//		prevy=pixely;
//	}
//}

/***************************************************************************/
//     Purpose: 
//**************************************************************************/
//void CDiagramm::DoXSquared()
//{
//	//we loop round from the given xstart to xend
//	UINT prevx=0;
//	UINT prevy=0;
//	BOOL firstpoint=TRUE;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	double xperpixel= 1/m_dPixelsPerX;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//the left hand side of the graph plot area on screen
//
//	//how many steps (pixels) in the x-direction
//	UINT xspan=(m_dXAxisMax -m_dXAxisMin)*m_dPixelsPerX;
//
//	UINT step;
//	if(m_pFunctionParams->ChartType==G_BARCHART)
//	{
//		step=G_BARCHARTWIDTH;
//	}
//	else
//	{
//		step=1;
//	}
//
//	for (UINT count =0; count < xspan; count+=step)
//	{
//		//do the function
//		double y=pow( (m_dXAxisMin+count*xperpixel),2);
//
//		UINT pixelx=xstart+count;
//		y=ConstrainY(y);//keep y within the plottable y scale
//
//		UINT pixely=ystart-((y-m_dYAxisMin)/yperpixel);
//
//		if(firstpoint)
//		{
//			prevx=pixelx;
//			prevy=pixely;
//			firstpoint=FALSE;
//		}
//		PlotPoints(pixelx,pixely,prevx,prevy);
//		//current point becomes previous point
//		prevx=pixelx;
//		prevy=pixely;
//
//	}
//}

/***************************************************************************/
//     Purpose: 
//**************************************************************************/
//void CDiagramm::DoXCubed()
//{
//	//we loop round from the given xstart to xend
//	UINT prevx=0;
//	UINT prevy=0;
//	BOOL firstpoint=TRUE;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	double xperpixel= 1/m_dPixelsPerX;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
//
//	//how many steps (pixels) in the x-direction
//	UINT xspan=(m_dXAxisMax - m_dXAxisMin)*m_dPixelsPerX;
//
//	UINT step;
//	if(m_pFunctionParams->ChartType==G_BARCHART)
//	{
//		step=G_BARCHARTWIDTH;
//	}
//	else
//	{
//		step=1;
//	}
//
//	for (UINT count =0; count < xspan; count+=step)
//	{
//		//do the function
//		double y=pow( (m_dXAxisMin+count*xperpixel),3.00);
//
//		UINT pixelx=xstart+count;
//
//		y=ConstrainY(y);	
//		UINT pixely=ystart-((y-m_dYAxisMin)/yperpixel);
//
//		if(firstpoint)
//		{
//			prevx=pixelx;
//			prevy=pixely;
//			firstpoint=FALSE;
//		}
//		PlotPoints(pixelx,pixely,prevx,prevy);
//		//current point becomes previous point
//		prevx=pixelx;
//		prevy=pixely;
//
//	}
//}

/***************************************************************************/
//     Purpose: 
//**************************************************************************/
//void CDiagramm::DoMXPlusC()
//{
//	UINT prevx=0;
//	UINT prevy=0;
//	BOOL firstpoint=TRUE;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//	double xperpixel= 1/m_dPixelsPerX;
//	double yperpixel=1/m_dPixelsPerY;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);//
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
//
//	//how many steps are they (pixels) in the x-direction
//	UINT xspan=(m_dXAxisMax - m_dXAxisMin)*m_dPixelsPerX;
//
//	UINT step;
//	if(m_pFunctionParams->ChartType==G_BARCHART)
//	{
//		step=G_BARCHARTWIDTH;
//	}
//	else
//	{
//		step=1;
//	}
//
//	for (UINT count =0; count < xspan; count+=step)
//	{
//		//do the function
//		double y=m_pFunctionParams->Const_1*( m_dXAxisMin+(count*xperpixel))+m_pFunctionParams->Const_2;
//
//		UINT pixelx=xstart+count;
//
//		y=ConstrainY(y);	
//		UINT pixely=ystart-((y-m_dYAxisMin)/yperpixel);
//
//		if(firstpoint)
//		{
//			prevx=pixelx;
//			prevy=pixely;
//			firstpoint=FALSE;
//		}
//		PlotPoints(pixelx,pixely,prevx,prevy);
//		//current point becomes previous point
//		prevx=pixelx;
//		prevy=pixely;
//
//	}
//}

/***************************************************************************/
//     Purpose: For the Dot chart type - this routine plots the pixel 
//				at the given point
//**************************************************************************/
//void CDiagramm::DrawDot(UINT X, UINT Y)
//{
//	CClientDC dc(this);
//	dc.SetPixel(X,Y,m_crGraphPenColor);
//}

/***************************************************************************/
//     Purpose: This draws the bar of a bar chart				
//				The lefthand side of the bar is at point xy.			
//				It also calcultes whether it is a 'positive' bar or a 'negative 'bar'
//				Some manipulation of the barwidths maybe required depending on what
//				function is being drawn
//**************************************************************************/
//void CDiagramm::DrawBar(UINT x, UINT y)
//{
//	//here we create a vertical rect
//	//and fill it with the current pen color
//	//we only use the current point - the width
//	//of the barchart is added to make rect
//
//	//for plotxy, dev% an dev (abs) barcharts, we have to adjust the width
//	//because for these funtions the user provides a set of xpoint for the
//	//plot. These points may be unevenly spread and may cause overlapping
//	//bars.
//	//we do however know howmuch one unit of x is worth in pixels so we
//	//use that as our barwidth
//
//	UINT barwidth;
//	if	(		(m_pFunctionParams->FuncType==G_PLOTXY)
//		|| (m_pFunctionParams->FuncType==G_PLOTDEV_PERCENT)
//		|| (m_pFunctionParams->FuncType==G_PLOTDEV_ABS)
//		)
//	{
//		if( m_dPixelsPerX >1)
//		{
//			barwidth=m_dPixelsPerX;
//		}
//		else
//		{
//			barwidth=1;
//		}//
//	}
//	else //Standard bars for everybody else
//	{
//		barwidth=G_BARCHARTWIDTH;
//	}
//
//	CRect rect,dataarea;
//	dataarea=CalcDataArea();
//	//remember when we create the bar we must not step over the graph RHS
//	rect.left=x;
//	rect.right= ((x+barwidth)<dataarea.right)? x+barwidth :dataarea.right;
//	//the tricky bit
//	//the top/bottom of the bar depends on whether the bar
//	//is a negative value or positive value
//
//	//note we have need of the Y origin here
//	//the Yorigin is an off-set from the bottom of the entire graph bottom
//	//and therefore includes the bottom margin -
//	// - not from the bottom of the plot area as is our  pixel co-ords
//	UINT Yorg=m_iOriginY-m_bmargin;
//
//
//	if(y < dataarea.bottom-Yorg)
//	{
//		//it's a 'positive' value
//		rect.top=y;
//		rect.bottom=dataarea.bottom-Yorg;
//
//	}
//	else
//	{
//		rect.top=dataarea.bottom-Yorg;
//		rect.bottom=y;
//		// it's a negative value
//	}
//
//	CPen pen, *poldpen;
//	CBrush brush;	
//
//	CClientDC dc(this);
//
//	brush.CreateSolidBrush(m_crGraphPenColor);
//	pen.CreatePen(PS_SOLID,1,m_crGraphPenColor);
//	poldpen=dc.SelectObject(&pen);
//
//	dc.Rectangle(&rect);
//
//	dc.SelectObject(poldpen);
//}



/***************************************************************************/
//     Purpose: The user can request two types of histogram, namely			
//				one where the frequency of occurrence is displayed as anumber value,
//				the other where the frequency of occurrence is displayed as a percentage
//				of the of the quantity of dataitems								
//				Either way they both end up here to be plotted.					
//				The usersupplies the usual data pairs.							
//				However, the first values of the pair(usually the x value)			
//				is not used by this function. The user can use this as a sequence	
//				number if they wish.												
//				We use the send value of each pair -usually the y value.			
//				//For the histogram - we need to calulate						
//				//a. The mean value													
//				//b. The Root of the Mean of the Squares;							
//
//				To disply the data we, we divide the graph(dataarea) by the width	
//				of a bar to see how many we can fit in. We then divide the min-max	
//				span of the data into the same amount of slices.				
//				We then check each data item to see which slice it falls into.
//**************************************************************************/
//void CDiagramm::DoHistogram()
//{
//
//	UINT prevx=0;
//	UINT prevy=0;
//	BOOL firstpoint=TRUE;
//
//	LONG result;
//	UINT xstart;
//	UINT ystart;
//
//	result=ConvertToGraphCoords(m_dXAxisMin,m_dYAxisMin);
//	xstart=LOWORD(result);//the left hand side of the graph plot area on screen
//	ystart=HIWORD(result);//shouldbe the bottom of the graph plot area on screen
//
//	//we will autoscale the x-axis to match the min and max value in the data;
//	UINT count;
//	double minval,maxval;
//	minval=maxval=m_pFunctionParams->pPlotXYItems[1];
//	for (count =0; count < m_pFunctionParams->num_PlotXYItems*2; count+=2)
//	{
//		if(m_pFunctionParams->pPlotXYItems[count+1] < minval)
//		{
//			minval=m_pFunctionParams->pPlotXYItems[count+1];
//		}
//		else if (m_pFunctionParams->pPlotXYItems[count+1] > maxval)
//		{
//			maxval=m_pFunctionParams->pPlotXYItems[count+1];
//		}
//	}
//	//we should have just found the min and max values of the data
//	//scale graph accordingly
//	SetXAxisScale(minval,maxval);
//	DrawXAxisNumbers();
//
//	//find how many bars can fit into the praph plot width
//	CRect dataarearect=CalcDataArea();
//	UINT num_strips= (dataarearect.right-dataarearect.left)/G_BARCHARTWIDTH;
//
//	//find out how much a strip is 'worth'
//	double strip_val= (maxval-minval)/num_strips;
//	//count the frequency of the data
//	double *pCounts=new double[num_strips];
//	memset(pCounts,0,num_strips*sizeof(double));
//	for(count=0;count < m_pFunctionParams->num_PlotXYItems*2;count +=2)
//	{
//		double value=m_pFunctionParams->pPlotXYItems[count+1];
//		//see which strip the current value falls into
//		for(UINT stripcount=0; stripcount<num_strips;stripcount++)
//		{
//
//			if( (value >=minval+(stripcount*strip_val)) && 
//				(value <= minval+(stripcount*strip_val)+strip_val))	
//			{
//				pCounts[stripcount]++;
//				break;
//			}
//		}
//
//	}
//	/*
//	Chack if we are showing the histogram frequencies as an absolute
//	occurrence figure or as a percentage occurrence
//	*/
//	if(m_pFunctionParams->FuncType==G_HISTOGRAM_ABS)
//	{
//
//		//find the maximum frequency of occurence and scale the 
//		//y axis
//		int max_freqcurrent=pCounts[0]; //preset a start point
//		for(count=1;count < num_strips; count++)
//		{
//			if (pCounts[count] > max_freqcurrent)
//			{
//				max_freqcurrent=pCounts[count];
//			}
//		}
//
//		SetYAxisScale(0,max_freqcurrent+1);
//
//	}
//	else
//	{
//		//we are showing as a percentage
//		//change the ocurrences from an absolute occurence figure to % occurrence
//
//		for(count=0;count < num_strips; count++)
//		{
//			pCounts[count] =( (pCounts[count]/ ((double)m_pFunctionParams->num_PlotXYItems))*100);
//		}
//
//		//find the maximum frequency of occurence and scale the 
//		//y axis
//		int max_freqcurrent=pCounts[0]; //preset a start point
//		for(count=1;count < num_strips; count++)
//		{
//			if (pCounts[count] > max_freqcurrent)
//			{
//				max_freqcurrent=pCounts[count];
//			}
//		}
//
//
//		SetYAxisScale(0,max_freqcurrent+1);
//	}
//
//	//in both cases
//	DrawYAxisNumbers();
//
//	//we have changed the x & y axis scaling so recalculate
//	//the yperpixel and/or xperpixel as required
//	double yperpixel=1/m_dPixelsPerY;	
//
//	for (count =0; count < num_strips; count++)
//	{		
//		UINT pixelx= xstart+(count*G_BARCHARTWIDTH);;
//
//		pCounts[count]=ConstrainY(pCounts[count]);	
//		UINT pixely=ystart-(pCounts[count]/yperpixel);
//
//		if(firstpoint)
//		{
//			prevx=pixelx;
//			prevy=pixely;
//			firstpoint=FALSE;
//		}
//		PlotPoints(pixelx,pixely,prevx,prevy);
//		//current point becomes previous point
//		prevx=pixelx;
//		prevy=pixely;
//
//	}
//	//cleanup
//	delete []pCounts;
//}
