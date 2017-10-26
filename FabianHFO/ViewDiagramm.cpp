// ViewDiagramm.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FabianHFO.h"
#include "ViewDiagramm.h"
#include "globDefs.h"
#include <vector>
#include <algorithm>
#include <functional>
#include "MVViewHandler.h"
// CViewDiagramm
CRITICAL_SECTION CViewDiagramm::csDoThread;
CRITICAL_SECTION CViewDiagramm::csSPIReadInBuffer;
CRITICAL_SECTION CViewDiagramm::csCurrentBreath;
CRITICAL_SECTION CViewDiagramm::csSpeed;
CRITICAL_SECTION CViewDiagramm::csXValue;
CRITICAL_SECTION CViewDiagramm::csVolLimit;
CRITICAL_SECTION CViewDiagramm::csVolGarant;
CRITICAL_SECTION CViewDiagramm::csThreadAccess;
CRITICAL_SECTION CViewDiagramm::csExit;
CRITICAL_SECTION CViewDiagramm::csRedrawDiagram;

CRITICAL_SECTION CViewDiagramm::csDrawDataFOT;
CRITICAL_SECTION CViewDiagramm::csDrawDataSPO2;
CRITICAL_SECTION CViewDiagramm::csDrawDataCO2;
CRITICAL_SECTION CViewDiagramm::csDiagrammPRESSURE;
CRITICAL_SECTION CViewDiagramm::csDiagrammFLOW;
CRITICAL_SECTION CViewDiagramm::csDiagrammVOLUME;
CRITICAL_SECTION CViewDiagramm::csDiagrammLOOP;
CRITICAL_SECTION CViewDiagramm::csDiagrammSPO2;
CRITICAL_SECTION CViewDiagramm::csDiagrammCO2;
CRITICAL_SECTION CViewDiagramm::csDiagrammFOT;
CRITICAL_SECTION CViewDiagramm::csGraphButton;
CRITICAL_SECTION CViewDiagramm::csMenuGraphs;
CRITICAL_SECTION CViewDiagramm::csLPFS_FUNCTIONSTRUCT;

extern CEvent g_eventGraphData;

using namespace std;
// Define a template class vector of int
typedef vector<int, allocator<int> > IntVector ;

//Define an iterator for template class vector of strings
typedef IntVector::iterator IntVectorIt ;

#define COLOR_TXTBTNUP				0x00000000
#define COLOR_TXTBTNDW				0x00FFFFFF
#define COLOR_TXTBTNFC				0x00000000

#define TIME_PARATIMER 45000

PBUFSPI* CViewDiagramm::m_pbufSPIReadInBuffer;
PBUFSPI* CViewDiagramm::m_pbufCurrentBreath;


LPG_FUNCTIONSTRUCT_LD CViewDiagramm::m_lpfsSPO2;
LPG_FUNCTIONSTRUCT_LD CViewDiagramm::m_lpfsCO2;
LPG_FUNCTIONSTRUCT_LD CViewDiagramm::m_lpfsPressure;
LPG_FUNCTIONSTRUCT_LD CViewDiagramm::m_lpfsFlow;
LPG_FUNCTIONSTRUCT_LD CViewDiagramm::m_lpfsVolume;


LPG_PLOTITEMSSTRUCT_LD CViewDiagramm::m_pPlotItemsFOTloop;
LPG_FUNCTIONSTRUCT_LD CViewDiagramm::m_lpfsFOT;

LPG_PLOTITEMSSTRUCT_LD CViewDiagramm::m_pPlotItemsPVloop;
LPG_FUNCTIONSTRUCT_LD CViewDiagramm::m_lpfsPressureVolume;

LPG_PLOTITEMSSTRUCT_LD CViewDiagramm::m_pPlotItemsVFloop;
LPG_FUNCTIONSTRUCT_LD CViewDiagramm::m_lpfsVolumeFlow;

LPG_PLOTITEMSSTRUCT_LD CViewDiagramm::m_pPlotItems;


IMPLEMENT_DYNAMIC(CViewDiagramm, CWnd)

CViewDiagramm::CViewDiagramm(int ViewID):
CMVView(ViewID)
{
	InitializeCriticalSection(&csDoThread);
	InitializeCriticalSection(&csDrawDataFOT);
	InitializeCriticalSection(&csDrawDataSPO2);
	InitializeCriticalSection(&csDrawDataCO2);
	InitializeCriticalSection(&csSPIReadInBuffer);
	InitializeCriticalSection(&csCurrentBreath);
	InitializeCriticalSection(&csXValue);
	InitializeCriticalSection(&csSpeed);
	InitializeCriticalSection(&csVolLimit);
	InitializeCriticalSection(&csVolGarant);
	InitializeCriticalSection(&csThreadAccess);
	InitializeCriticalSection(&csRedrawDiagram);
	InitializeCriticalSection(&csDiagrammPRESSURE);
	InitializeCriticalSection(&csDiagrammFLOW);
	InitializeCriticalSection(&csDiagrammVOLUME);
	InitializeCriticalSection(&csDiagrammLOOP);
	InitializeCriticalSection(&csDiagrammSPO2);
	InitializeCriticalSection(&csDiagrammCO2);
	InitializeCriticalSection(&csDiagrammFOT);
	InitializeCriticalSection(&csGraphButton);
	InitializeCriticalSection(&csMenuGraphs);
	InitializeCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("init csLPFS_FUNCTIONSTRUCT\r\n")));

	InitializeCriticalSection(&csExit);

	m_bExit=false;


	m_pcGraphArrow_Up=NULL;
	m_pcGraphArrow_Fc=NULL;

	m_pcBargraphSIQofSPO2=NULL;

	m_iItemsToMerge=0;
	//m_bGraphThreadFinished=true;
	
	//m_bViewMenu=false;
	m_pcGraph1=NULL;
	//m_bUpdateView=false;
	

	m_dwLastSpeedchange=0;

	m_pPlotItems=NULL;
	m_pPlotItemsFOTloop=NULL;
	m_pPlotItemsPVloop=NULL;
	m_pPlotItemsVFloop=NULL;

	m_lpfsSPO2=NULL;
	m_lpfsCO2=NULL;
	m_lpfsPressure=NULL;
	m_lpfsFlow=NULL;
	m_lpfsVolume=NULL;
	m_lpfsFOT=NULL;
	m_lpfsPressureVolume=NULL;
	m_lpfsVolumeFlow=NULL;

	m_hThreadGraph=INVALID_HANDLE_VALUE;
	m_pcwtGraphThread=NULL;
	m_bDoThread=false;

	m_pWndMenuGraphs=NULL;
	//m_pcWndGraphViewSelection=NULL;

	m_pDataFOT=NULL;
	m_pLoopFOT=NULL;

	m_pDataSPO2=NULL;
	m_pDiagrammSPO2=NULL;
	m_pDataCO2=NULL;
	m_pDiagrammCO2=NULL;
	m_pDiagrammPressure=NULL;
	m_pDiagrammVolume=NULL;
	m_pDiagrammFlow=NULL;
	m_pLoopPressureVolume=NULL;
	m_pLoopVolumeFlow=NULL;

	m_bDataFOTIsActive=false;
	m_bDataSPO2IsActive=false;
	m_bDataCO2IsActive=false;
	m_bLoopFOTIsActive=false;
	m_bGraphSPO2IsActive=false;
	m_bGraphCO2IsActive=false;
	m_bGraphPressureIsActive=false;
	m_bGraphVolumeIsActive=false;
	m_bGraphFlowIsActive=false;
	m_bLoopPressureVolumeIsActive=false;
	m_bLoopVolumeFlowIsActive=false;

	m_iGraphSpeedGraph=getModel()->getCONFIG()->GraphGetActualSpeedGraph();

	m_iCurrentLineDiagramm=getModel()->getCONFIG()->GraphGetPrimaryLineDiagramm();
	m_iCurFocusedWnd=0;
	m_bGraphSelected = false;

	m_iCurrentXvalGraphs=1;
	
	m_bDecreaseGraphSpeed=false;
	m_bIncreaseGraphSpeed=false;

	m_bResetXaxe=true;
	m_bFreeze = false;
	m_bFreezeNextBreath = false;
	m_bDrawSavedLoops=false;
	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
		m_bFilledGraph=false;
	else
		m_bFilledGraph=getModel()->getCONFIG()->GraphIsFilled();

	m_bDrawVolumeLimit=IsVLimitStateOn();
	m_dbValueVolumeLimit=(getModel()->getDATAHANDLER()->GetCurrentVLimitPara())/*/10*/;

	m_bDrawVolumeGaranty=drawVGarantyLine();
	m_dbValueVolumeGaranty=getModel()->getDATAHANDLER()->GetActiveModePMAXVGPara()/10;//newVG

	m_bDrawHFOVolumeGaranty=drawHFOVGarantyLine();
	if(m_bDrawHFOVolumeGaranty)
	{
		CalculateHFOVolGaranty();
	}
	else
	{
		m_dbHFOValueVolumeGarantyHigh=0;
		m_dbHFOValueVolumeGarantyLow=0;
	}
	

	m_iTrigThreshold=getModel()->getDATAHANDLER()->getAVGMessureDataTrigger();

	m_pbufCurrentBreath = new PBUFSPI[G_MAXPOINTS];
	m_iSizeCurrentBreath=0;
	
	m_pbufSPIReadInBuffer = new PBUFSPI[MAXSIZE_SPIREADIN_BUFFER];
	m_iSizeReadInBufferSPI=0;

	m_iPrevXvalPressure=0;
	m_iPrevXvalVolume=0;
	m_iPrevXvalFlow=0;

	m_bUpdateRingBufCopy=false;
	m_iDiagramToRedraw=REDRAW_NONE;


	m_bRefreshLastBreathOfLoops=false;
	m_bResetAllDiagramms=false;
	//m_bResetCurrentXaxes=false;

	//m_bResetCLFIO2Graph=false;
	//m_bResetEMGGraph=false;
	//m_bResetSPO2Graph=false;
	//m_bResetCO2Graph=false;
	//m_bResetPressureGraph=false;
	//m_bResetFlowGraph=false;
	//m_bResetVolumeGraph=false;


	m_pPlotItemsFOTloop=new G_PLOTITEMSSTRUCT_LD[G_MAXFOTSTEPS];
	m_pPlotItemsPVloop=new G_PLOTITEMSSTRUCT_LD[G_MAXPOINTS];
	m_pPlotItemsVFloop=new G_PLOTITEMSSTRUCT_LD[G_MAXPOINTS];

	m_pPlotItems=new G_PLOTITEMSSTRUCT_LD[MAXSIZE_RING_BUFFER];

	

	m_lpfsSPO2 =new G_FUNCTIONSTRUCT_LD;
	memset(m_lpfsSPO2,0,sizeof(G_FUNCTIONSTRUCT_LD));
	m_lpfsSPO2->FuncType=G_PLOTYPOINTS;
	m_lpfsSPO2->xMax=0;
	m_lpfsSPO2->xMin=0;
	m_lpfsSPO2->yMax=G_UPPER_MAXSCALE_SPO2;
	m_lpfsSPO2->yMin=G_LOWER_MAXSCALE_SPO2;
	m_lpfsSPO2->dXfactor=1;
	m_lpfsSPO2->dYfactor=G_FACTOR_SPO2;
	m_lpfsSPO2->pItems=m_pPlotItems;
	m_lpfsSPO2->num_PlotXYItems=0;

	m_lpfsCO2 =new G_FUNCTIONSTRUCT_LD;
	memset(m_lpfsCO2,0,sizeof(G_FUNCTIONSTRUCT_LD));
	m_lpfsCO2->FuncType=G_PLOTYPOINTS;
	m_lpfsCO2->xMax=0;
	m_lpfsCO2->xMin=0;
	m_lpfsCO2->yMax=G_UPPER_MAXSCALE_CO2;
	m_lpfsCO2->yMin=G_LOWER_MAXSCALE_CO2;
	m_lpfsCO2->dXfactor=1;
	m_lpfsCO2->dYfactor=G_FACTOR_CO2;
	m_lpfsCO2->pItems=m_pPlotItems;
	m_lpfsCO2->num_PlotXYItems=0;

	m_lpfsPressure =new G_FUNCTIONSTRUCT_LD;
	memset(m_lpfsPressure,0,sizeof(G_FUNCTIONSTRUCT_LD));
	m_lpfsPressure->FuncType=G_PLOTYPOINTS;
	m_lpfsPressure->xMax=0;
	m_lpfsPressure->xMin=0;
	if(getModel()->getVMODEHANDLER()->activeModeIsNMODE())
	{
		m_lpfsPressure->yMax=G_UPPER_MAXSCALE_NMODEPRESSURE;
	}
	else
	{
		m_lpfsPressure->yMax=G_UPPER_MAXSCALE_PRESSURE;
	}
	m_lpfsPressure->yMin=G_LOWER_MAXSCALE_PRESSURE;
	m_lpfsPressure->dXfactor=1;
	m_lpfsPressure->dYfactor=G_FACTOR_PRESSURE;
	m_lpfsPressure->pItems=m_pPlotItems;
	m_lpfsPressure->num_PlotXYItems=0;

	int iUPPER_MAXSCALE_VOLUME=0;
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
		iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_NEO;
	else
		iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_PED;

	m_lpfsVolume =new G_FUNCTIONSTRUCT_LD;
	memset(m_lpfsVolume,0,sizeof(G_FUNCTIONSTRUCT_LD));
	m_lpfsVolume->FuncType=G_PLOTYPOINTS;
	m_lpfsVolume->xMax=0;
	m_lpfsVolume->xMin=0;
	m_lpfsVolume->yMax=iUPPER_MAXSCALE_VOLUME;
	m_lpfsVolume->yMin=G_LOWER_MAXSCALE_VOLUME;
	m_lpfsVolume->dXfactor=1;
	m_lpfsVolume->dYfactor=G_FACTOR_VOLUME;
	m_lpfsVolume->pItems=m_pPlotItems;
	m_lpfsVolume->num_PlotXYItems=0;


	m_lpfsFlow =new G_FUNCTIONSTRUCT_LD;
	memset(m_lpfsFlow,0,sizeof(G_FUNCTIONSTRUCT_LD));
	m_lpfsFlow->FuncType=G_PLOTYPOINTS;
	m_lpfsFlow->xMax=0;
	m_lpfsFlow->xMin=0;
	m_lpfsFlow->yMax=G_UPPER_MAXSCALE_FLOW;
	m_lpfsFlow->yMin=G_LOWER_MAXSCALE_FLOW;
	m_lpfsFlow->dXfactor=1;
	m_lpfsFlow->dYfactor=G_FACTOR_FLOW;
	m_lpfsFlow->pItems=m_pPlotItems;
	m_lpfsFlow->num_PlotXYItems=0;


	m_lpfsPressureVolume =new G_FUNCTIONSTRUCT_LD;
	memset(m_lpfsPressureVolume,0,sizeof(G_FUNCTIONSTRUCT_LD));
	m_lpfsPressureVolume->FuncType=G_PLOTXYLOOPPOINTS;
	m_lpfsPressureVolume->xMax=G_UPPER_MAXSCALE_PRESSURE;
	m_lpfsPressureVolume->xMin=G_LOWER_MAXSCALE_PRESSURE;
	m_lpfsPressureVolume->yMax=iUPPER_MAXSCALE_VOLUME;
	m_lpfsPressureVolume->yMin=G_LOWER_MAXSCALE_VOLUME;
	m_lpfsPressureVolume->dXfactor=G_FACTOR_PRESSURE;
	m_lpfsPressureVolume->dYfactor=G_FACTOR_VOLUME;
	m_lpfsPressureVolume->ChartType=G_LOOPCHART;
	m_lpfsPressureVolume->pItems=m_pPlotItemsPVloop;
	m_lpfsPressureVolume->num_PlotXYItems=0;


	m_lpfsFOT =new G_FUNCTIONSTRUCT_LD;
	memset(m_lpfsFOT,0,sizeof(G_FUNCTIONSTRUCT_LD));
	m_lpfsFOT->FuncType=G_PLOTSAVEDLOOPPOINTS;//G_PLOTXYLOOPPOINTS;
	//m_lpfsFOT->xMax=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanMaxPara();
	//m_lpfsFOT->xMin=G_LOWER_MAXSCALE_PMEAN_FOT;
	m_lpfsFOT->xMax=G_UPPER_MAXSCALE_PRESSURE;
	m_lpfsFOT->xMin=G_LOWER_MAXSCALE_PRESSURE;
	
	m_lpfsFOT->yMax=G_UPPER_MAXSCALE_XRS_FOT;//G_UPPER_MINSCALE_XRS_FOT; //rku, check FOTGRAPH
	m_lpfsFOT->yMin=G_LOWER_MINSCALE_XRS_FOT;
	m_lpfsFOT->dXfactor=G_FACTOR_FOTxrs;
	m_lpfsFOT->dYfactor=1;
	m_lpfsFOT->ChartType=G_FOTLOOP;
	m_lpfsFOT->pItems=m_pPlotItemsFOTloop;
	m_lpfsFOT->num_PlotXYItems=0;

	





	m_lpfsVolumeFlow =new G_FUNCTIONSTRUCT_LD;
	memset(m_lpfsVolumeFlow,0,sizeof(G_FUNCTIONSTRUCT_LD));
	m_lpfsVolumeFlow->FuncType=G_PLOTXYLOOPPOINTS;
	m_lpfsVolumeFlow->xMax=iUPPER_MAXSCALE_VOLUME;
	m_lpfsVolumeFlow->xMin=G_LOWER_MAXSCALE_VOLUME;
	m_lpfsVolumeFlow->yMax=G_UPPER_MAXSCALE_FLOW;
	m_lpfsVolumeFlow->yMin=G_LOWER_MAXSCALE_FLOW;
	m_lpfsVolumeFlow->dXfactor=G_FACTOR_VOLUME;
	m_lpfsVolumeFlow->dYfactor=G_FACTOR_FLOW;
	m_lpfsVolumeFlow->ChartType=G_LOOPCHART;
	m_lpfsVolumeFlow->pItems=m_pPlotItemsVFloop;
	m_lpfsVolumeFlow->num_PlotXYItems=0;

	m_szGraph1=_T("");
	m_szGraph2=_T("");
	m_szGraph3=_T("");
}

CViewDiagramm::~CViewDiagramm()
{
	EnterCriticalSection(&csSPIReadInBuffer);
	if (m_pbufSPIReadInBuffer != NULL) {
		delete [] m_pbufSPIReadInBuffer;
	}
	m_pbufSPIReadInBuffer=NULL;
	LeaveCriticalSection(&csSPIReadInBuffer);

	EnterCriticalSection(&csCurrentBreath);
	if (m_pbufCurrentBreath != NULL) {
		delete [] m_pbufCurrentBreath;
	}
	m_pbufCurrentBreath=NULL;
	LeaveCriticalSection(&csCurrentBreath);


	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("lock 1\r\n")));
	if(m_pPlotItems !=NULL)
	{
		delete []m_pPlotItems;
	}
	m_pPlotItems=NULL;

	if(m_lpfsSPO2 !=NULL)
	{
		delete m_lpfsSPO2;
	}
	m_lpfsSPO2=NULL;

	if(m_lpfsCO2 !=NULL)
	{
		delete m_lpfsCO2;
	}
	m_lpfsCO2=NULL;

	if(m_lpfsPressure !=NULL)
	{
		delete m_lpfsPressure;
	}
	m_lpfsPressure=NULL;


	if(m_lpfsVolume !=NULL)
	{
		delete m_lpfsVolume;
	}
	m_lpfsVolume=NULL;


	if(m_lpfsFlow !=NULL)
	{
		delete m_lpfsFlow;
	}
	m_lpfsFlow=NULL;


	if(m_pPlotItemsPVloop !=NULL)
	{
		delete []m_pPlotItemsPVloop;
	}
	m_pPlotItemsPVloop=NULL;

	if(m_lpfsPressureVolume !=NULL)
	{
		delete m_lpfsPressureVolume;
	}
	m_lpfsPressureVolume=NULL;


	if(m_pPlotItemsFOTloop !=NULL)
	{
		delete []m_pPlotItemsFOTloop;
	}
	m_pPlotItemsFOTloop=NULL;
	if(m_lpfsFOT !=NULL)
	{
		delete m_lpfsFOT;
	}
	m_lpfsFOT=NULL;

	if(m_pPlotItemsVFloop !=NULL)
	{
		delete []m_pPlotItemsVFloop;
	}
	m_pPlotItemsVFloop=NULL;

	if(m_lpfsVolumeFlow !=NULL)
	{
		delete m_lpfsVolumeFlow;
	}
	m_lpfsVolumeFlow=NULL;
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 1\r\n")));

	EnterCriticalSection(&csGraphButton);
	delete m_pcGraph1;
	m_pcGraph1=NULL;
	LeaveCriticalSection(&csGraphButton);
	
	delete m_pcGraphArrow_Up;
	m_pcGraphArrow_Up=NULL;
	delete m_pcGraphArrow_Fc;
	m_pcGraphArrow_Fc=NULL;

	if(m_pcBargraphSIQofSPO2)
		delete m_pcBargraphSIQofSPO2;
	m_pcBargraphSIQofSPO2=NULL;

	//DEBUGMSG(TRUE, (TEXT("delete csLPFS_FUNCTIONSTRUCT\r\n")));
	DeleteCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	DeleteCriticalSection(&csMenuGraphs);
	DeleteCriticalSection(&csDrawDataFOT);
	DeleteCriticalSection(&csDrawDataSPO2);
	DeleteCriticalSection(&csDrawDataCO2);
	DeleteCriticalSection(&csSPIReadInBuffer);
	DeleteCriticalSection(&csCurrentBreath);
	DeleteCriticalSection(&csXValue);
	DeleteCriticalSection(&csSpeed);
	DeleteCriticalSection(&csVolLimit);
	DeleteCriticalSection(&csVolGarant);
	DeleteCriticalSection(&csThreadAccess);
	DeleteCriticalSection(&csRedrawDiagram);
	DeleteCriticalSection(&csGraphButton);
	DeleteCriticalSection(&csDiagrammPRESSURE);
	DeleteCriticalSection(&csDiagrammFLOW);
	DeleteCriticalSection(&csDiagrammVOLUME);
	DeleteCriticalSection(&csDiagrammLOOP);
	DeleteCriticalSection(&csDiagrammSPO2);
	DeleteCriticalSection(&csDiagrammCO2);
	DeleteCriticalSection(&csDiagrammFOT);
	DeleteCriticalSection(&csDoThread);
	DeleteCriticalSection(&csExit);
}


BEGIN_MESSAGE_MAP(CViewDiagramm, CMVView)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SELGRAPH_1, &CViewDiagramm::OnBnClickedNextGraph)
END_MESSAGE_MAP()



// CViewDiagramm-Meldungshandler

// **************************************************************************
// 
// **************************************************************************
bool CViewDiagramm::CreateView()
{
	//RECT rcLd={0,48,605,212};
	RECT rcLd={0,48,605,542};

	if (!Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_GRAPH))
	{
		return false;
	}
	Initialize();

	Show();
	this->ShowWindow(SW_SHOW);
	
	if(m_bDataCO2IsActive)
	{
		ShowWndDataCO2(true);
	}
	else if(m_bDataSPO2IsActive)
	{
		ShowWndDataSPO2(true);
	}
	else if(m_bDataFOTIsActive)
	{
		ShowWndDataFOT(true);
	}
	
	return true;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CViewDiagramm::Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext) 
{
	m_lX=rc.right-rc.left;
	m_lY=rc.bottom-rc.top;

	CStringW  lpszClassName=AfxRegisterWndClass(NULL); 
	if(CWnd::Create(lpszClassName,NULL,WS_CHILD/*|WS_VISIBLE*/,rc,pParentWnd,nID))
	{
		CClientDC dc(this);
		m_hDC=CreateCompatibleDC(dc.m_hDC);
		m_hBmp=CreateCompatibleBitmap(dc.m_hDC,m_lX,m_lY);
		m_hBmpPrev=(HBITMAP)SelectObject(m_hDC,m_hBmp);

		CBrush cbrBack(BACKGND);
		//CBrush cbrBack(RGB(0,0,255));

		HBRUSH hbrprev=(HBRUSH)SelectObject(m_hDC,cbrBack);
		HPEN hpenprev=(HPEN)SelectObject(m_hDC, (HPEN)GetStockObject(NULL_PEN));

		Rectangle(m_hDC, 0, 0, m_lX, m_lY);

		SelectObject(m_hDC,hbrprev);
		SelectObject(m_hDC,hpenprev);
		//DeleteObject(cbrBack);

		return 1;
	}
	else
		return 0;
}

bool CViewDiagramm::Initialize()
{
	if(isExit())
		return false;

	CDC* pDC = this->GetDC();
	HDC hdc = pDC->GetSafeHdc();

	m_pcGraphArrow_Up		= new CBmp(theApp.m_hInstance,hdc,	IDB_GRAPHBTN_ARW_UP);
	m_pcGraphArrow_Fc		= new CBmp(theApp.m_hInstance,hdc,	IDB_GRAPHBTN_ARW_DW);

	m_pcBargraphSIQofSPO2	= new CBmp(theApp.m_hInstance,hdc,	IDB_FRAME_SIQ);

	BTN btn;

	//Graph Buttons
	btn.wID					= IDC_BTN_SELGRAPH_1;	
	btn.poPosition.x		= 0;
	btn.poPosition.y		= 8;
	btn.pcBmpUp				= m_pcGraphArrow_Up;
	btn.pcBmpDown			= m_pcGraphArrow_Fc;
	btn.pcBmpFocus			= m_pcGraphArrow_Fc;
	btn.pcBmpDisabled		= m_pcGraphArrow_Up;
	btn.dwFormat			= DT_VCENTER|DT_SINGLELINE|DT_CENTER;

	m_pcGraph1=new CSelectGraphBtn(btn,COLOR_TXTBTNUP);
	m_pcGraph1->Create(this,g_hf7AcuBold,0);
	m_pcGraph1->SetText(getModel()->GetLanguageString(IDS_BTN_SELPRESSUREGRAPH));
	m_pcGraph1->ShowWindow(SW_SHOW);


	setTextGraph1(getModel()->GetLanguageString(IDS_BTN_SELPRESSUREGRAPH));
	setTextGraph2(getModel()->GetLanguageString(IDS_BTN_SELFLOWGRAPH));
	setTextGraph3(getModel()->GetLanguageString(IDS_BTN_SELVOLUMEGRAPH));



	//release the Display DC
	ReleaseDC(pDC);


	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewDiagramm::CreateWndDataCO2(int x, int y, int nWidth, int nHeight)
{
	if(isExit())
		return false;

	bool result=false;

	EnterCriticalSection(&csDrawDataCO2);
	if(m_pDataCO2==NULL)
	{
		m_pDataCO2=new CWndDataCO2();
		RECT rcLd={x,y,x+nWidth,y+nHeight};

		if(m_pDataCO2->Create(this,rcLd,IDC_DATAWND_CO2))
		{
			result=true;
		}
	}
	else
	{
		m_pDataCO2->MoveWindow(x,y,nWidth,nHeight);
	}
	LeaveCriticalSection(&csDrawDataCO2);
	
	return result;
}
// **************************************************************************
// 
// **************************************************************************
bool CViewDiagramm::CreateWndGraphCO2(int x, int y, int nWidth, int nHeight)
{
	if(isExit())
		return false;

	bool result=false;

	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		m_bFilledGraph=false;
	}
	else
		m_bFilledGraph=getModel()->getCONFIG()->GraphIsFilled();


	EnterCriticalSection(&csDiagrammCO2);
	if(m_pDiagrammCO2==NULL)
	{
		m_pDiagrammCO2=new CDiagramm();
		RECT rcLd={x,y,x+nWidth,y+nHeight};

		if(m_pDiagrammCO2->Create(this,rcLd,IDC_LINEDIAGRAM_CO2))
		{
			result=true;
			double dYAxisMax=0;
			double dYAxisMin=0;

			if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
			{
				/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
				//DEBUGMSG(TRUE, (TEXT("lock 21\r\n")));
				EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				m_lpfsCO2->FuncType=G_PLOTYPOINTS;
				m_lpfsCO2->xMax=0;
				m_lpfsCO2->xMin=0;
				m_lpfsCO2->yMax=G_UPPER_MAXSCALE_CO2;
				m_lpfsCO2->yMin=G_LOWER_MAXSCALE_CO2;
				m_lpfsCO2->dXfactor=1;
				m_lpfsCO2->dYfactor=G_FACTOR_CO2;
				m_lpfsCO2->pItems=m_pPlotItems;
				m_lpfsCO2->num_PlotXYItems=0;
				m_lpfsCO2->ChartType=G_LINECHART;
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 2\r\n")));

				m_pDiagrammCO2->SetDiagrammType(CO2_HF_GRAPH);

				if(	getModel()->getCONFIG()->GraphIsAutoScale())
				{
					dYAxisMax=G_LOWER_MAXSCALE_CO2;
				}
				else
				{
					dYAxisMax=getModel()->getCONFIG()->GetMAXSCALE_CO2_HFGRAPH();
				}

				dYAxisMin=m_pDiagrammCO2->GetNegativeYoffset(dYAxisMax);
				m_pDiagrammCO2->SetColorScheme(G_FabianHFOSCHEME_LINE,getModel()->getDATAHANDLER()->getGraphColor_ETCO2());
			}
			else
			{
				/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
				//DEBUGMSG(TRUE, (TEXT("lock 3\r\n")));
				EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				m_lpfsCO2->FuncType=G_PLOTYPOINTS;
				m_lpfsCO2->xMax=0;
				m_lpfsCO2->xMin=0;
				m_lpfsCO2->yMax=G_UPPER_MAXSCALE_CO2;
				m_lpfsCO2->yMin=G_LOWER_MAXSCALE_CO2;
				m_lpfsCO2->dXfactor=1;
				m_lpfsCO2->dYfactor=G_FACTOR_CO2;
				m_lpfsCO2->pItems=m_pPlotItems;
				m_lpfsCO2->num_PlotXYItems=0;
				if(m_bFilledGraph)
				{
					m_lpfsCO2->ChartType=G_FILLEDCHART;
				}
				else
				{
					m_lpfsCO2->ChartType=G_LINECHART;
				}
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 3\r\n")));

				m_pDiagrammCO2->SetDiagrammType(CO2_GRAPH);

				if(	getModel()->getCONFIG()->GraphIsAutoScale())
				{
					dYAxisMax=G_LOWER_MAXSCALE_DCO2;
				}
				else
				{
					dYAxisMax=getModel()->getCONFIG()->GetMAXSCALE_CO2_GRAPH();
				}
				dYAxisMin=m_pDiagrammCO2->GetNegativeYoffset(getModel()->getCONFIG()->GetMAXSCALE_CO2_GRAPH());
				m_pDiagrammCO2->SetColorScheme(G_DEFAULTSCHEME,getModel()->getDATAHANDLER()->getGraphColor_ETCO2());
			}
						
			m_pDiagrammCO2->SetXAxisTickSpace(GetXAxisTimeTickSpace());
			m_pDiagrammCO2->SetXAxisScale(0, GetTimeAxisMaxScale());
			m_pDiagrammCO2->SetYAxisScale(dYAxisMin, dYAxisMax,TRUE,false);
		}
	}
	else
	{
		double dYAxisMax=0;
		double dYAxisMin=0;

		
		m_pDiagrammCO2->MoveWindow(x,y,nWidth,nHeight);

		if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
		{
			/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
			//DEBUGMSG(TRUE, (TEXT("lock 4\r\n")));
			EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			m_lpfsCO2->FuncType=G_PLOTYPOINTS;
			m_lpfsCO2->xMax=0;
			m_lpfsCO2->xMin=0;
			m_lpfsCO2->yMax=G_UPPER_MAXSCALE_CO2;
			m_lpfsCO2->yMin=G_LOWER_MAXSCALE_CO2;
			m_lpfsCO2->dXfactor=1;
			m_lpfsCO2->dYfactor=G_FACTOR_CO2;
			m_lpfsCO2->pItems=m_pPlotItems;
			m_lpfsCO2->num_PlotXYItems=0;
			m_lpfsCO2->ChartType=G_LINECHART;
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 4\r\n")));

			m_pDiagrammCO2->SetDiagrammType(CO2_HF_GRAPH);

			if(	getModel()->getCONFIG()->GraphIsAutoScale())
			{
				dYAxisMax=G_LOWER_MAXSCALE_CO2;
			}
			else
			{
				dYAxisMax=getModel()->getCONFIG()->GetMAXSCALE_CO2_HFGRAPH();
			}

			dYAxisMin=m_pDiagrammCO2->GetNegativeYoffset(dYAxisMax);
			m_pDiagrammCO2->SetColorScheme(G_FabianHFOSCHEME_LINE,getModel()->getDATAHANDLER()->getGraphColor_ETCO2());
		}
		else
		{
			/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
			//DEBUGMSG(TRUE, (TEXT("lock 5\r\n")));
			EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			m_lpfsCO2->FuncType=G_PLOTYPOINTS;
			m_lpfsCO2->xMax=0;
			m_lpfsCO2->xMin=0;
			m_lpfsCO2->yMax=G_UPPER_MAXSCALE_CO2;
			m_lpfsCO2->yMin=G_LOWER_MAXSCALE_CO2;
			m_lpfsCO2->dXfactor=1;
			m_lpfsCO2->dYfactor=G_FACTOR_CO2;
			m_lpfsCO2->pItems=m_pPlotItems;
			m_lpfsCO2->num_PlotXYItems=0;
			if(m_bFilledGraph)
			{
				m_lpfsCO2->ChartType=G_FILLEDCHART;
			}
			else
			{
				m_lpfsCO2->ChartType=G_LINECHART;
			}
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 5\r\n")));

			m_pDiagrammCO2->SetDiagrammType(CO2_GRAPH);

			dYAxisMax=getModel()->getCONFIG()->GetMAXSCALE_CO2_GRAPH();
			
			dYAxisMin=m_pDiagrammCO2->GetNegativeYoffset(dYAxisMax);
			m_pDiagrammCO2->SetColorScheme(G_DEFAULTSCHEME,getModel()->getDATAHANDLER()->getGraphColor_ETCO2());
		}

		m_pDiagrammCO2->SetYAxisScale(dYAxisMin, dYAxisMax,TRUE,false);

	}
	LeaveCriticalSection(&csDiagrammCO2);

	return result;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewDiagramm::CreateWndDataFOT(int x, int y, int nWidth, int nHeight)
{
	if(isExit())
		return false;

	bool result=false;

	EnterCriticalSection(&csDrawDataFOT);
	if(m_pDataFOT==NULL)
	{
		m_pDataFOT=new CWndDataFOT(this);
		RECT rcLd={x,y,x+nWidth,y+nHeight};

		if(m_pDataFOT->Create(this,rcLd,IDC_DATAWND_FOT))
		{
			result=true;
		}
	}
	else
	{
		m_pDataFOT->MoveWindow(x,y,nWidth,nHeight);
	}
	LeaveCriticalSection(&csDrawDataFOT);
	
	return result;
}

/**=================================================================================================
 * \fn bool CViewDiagramm::CreateWndGraphFOT(int x, int y, int nWidth, int nHeight)
 *
 * \brief Creates window graph fot.
 *
 * \author Rainer
 * \date 28 Jan 2016
 *
 * \param x		  The x coordinate.
 * \param y		  The y coordinate.
 * \param nWidth  The width.
 * \param nHeight The height.
 *
 * \return true if it succeeds, false if it fails.
 *===============================================================================================**/
bool CViewDiagramm::CreateWndGraphFOT(int x, int y, int nWidth, int nHeight)
{
	if(isExit())
		return false;

	bool result=false;

	m_bFilledGraph=false;
	
	EnterCriticalSection(&csDiagrammFOT);
	if(m_pLoopFOT==NULL)
	{
		m_pLoopFOT=new CDiagramm();
		RECT rcLd={x,y,x+nWidth,y+nHeight};

		if(m_pLoopFOT->Create(this,rcLd,IDC_LOOP_FOT))
		{
			result=true;

			/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
			//DEBUGMSG(TRUE, (TEXT("lock 6\r\n")));
			EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			m_lpfsFOT->FuncType=G_PLOTSAVEDLOOPPOINTS;//G_PLOTXYLOOPPOINTS;
			m_lpfsFOT->xMax=G_UPPER_MAXSCALE_PRESSURE;
			m_lpfsFOT->xMin=G_LOWER_MAXSCALE_PRESSURE;
			m_lpfsFOT->yMax=G_UPPER_MAXSCALE_XRS_FOT;//G_UPPER_MINSCALE_XRS_FOT; //rku, check FOTGRAPH
			m_lpfsFOT->yMin=G_LOWER_MINSCALE_XRS_FOT;
			m_lpfsFOT->dXfactor=G_FACTOR_FOTxrs;
			m_lpfsFOT->dYfactor=1;
			m_lpfsFOT->pItems=m_pPlotItemsFOTloop;
			m_lpfsFOT->num_PlotXYItems=0;
			m_lpfsFOT->ChartType=G_FOTLOOP;
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 6\r\n")));

			m_pLoopFOT->SetDiagrammType(FOT_LOOP);
			m_pLoopFOT->SetColorScheme(G_FOT_LOOPD,RGB(0,0,255));
			m_pLoopFOT->SetXAxisScale(0, getModel()->getCONFIG()->GetMAXSCALE_FOT_PRESSURE());
			//m_pLoopFOT->SetYAxisScale( getModel()->getCONFIG()->GetMINSCALE_FOT_XRS(),0);//rku, check FOTGRAPH
			m_pLoopFOT->SetYAxisScale( getModel()->getCONFIG()->GetMINSCALE_FOT_XRS(),getModel()->getCONFIG()->GetMAXSCALE_FOT_XRS());

			m_pLoopFOT->PaintGraph();
			m_pLoopFOT->Show(true);
		}
	}
	else
	{
		m_pLoopFOT->MoveWindow(x,y,nWidth,nHeight);

		/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
		//DEBUGMSG(TRUE, (TEXT("lock 7\r\n")));
		EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
		m_lpfsFOT->FuncType=G_PLOTSAVEDLOOPPOINTS;
		m_lpfsFOT->xMax=G_UPPER_MAXSCALE_PRESSURE;
		m_lpfsFOT->xMin=G_LOWER_MAXSCALE_PRESSURE;
		m_lpfsFOT->yMax=G_UPPER_MAXSCALE_XRS_FOT;//G_UPPER_MINSCALE_XRS_FOT; //rku, check FOTGRAPH
		m_lpfsFOT->yMin=G_LOWER_MINSCALE_XRS_FOT;
		m_lpfsFOT->dXfactor=G_FACTOR_FOTxrs;
		m_lpfsFOT->dYfactor=1;
		m_lpfsFOT->pItems=m_pPlotItemsFOTloop;
		m_lpfsFOT->num_PlotXYItems=0;
		m_lpfsFOT->ChartType=G_FOTLOOP;
		/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
		LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
		//DEBUGMSG(TRUE, (TEXT("unlock 7\r\n")));

		m_pLoopFOT->SetDiagrammType(FOT_LOOP);
		m_pLoopFOT->SetColorScheme(G_FOT_LOOPD,RGB(0,0,255));
		m_pLoopFOT->SetXAxisScale(0, getModel()->getCONFIG()->GetMAXSCALE_FOT_PRESSURE());
		m_pLoopFOT->SetYAxisScale( getModel()->getCONFIG()->GetMINSCALE_FOT_XRS(),0);

		m_pLoopFOT->PaintGraph();
		m_pLoopFOT->Show(true);

	}
	LeaveCriticalSection(&csDiagrammFOT);

	return result;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewDiagramm::CreateWndDataSPO2(int x, int y, int nWidth, int nHeight)
{
	if(isExit())
		return false;

	bool result=false;

	EnterCriticalSection(&csDrawDataSPO2);
	if(m_pDataSPO2==NULL)
	{
		m_pDataSPO2=new CWndDataSPO2();
		RECT rcLd={x,y,x+nWidth,y+nHeight};

		if(m_pDataSPO2->Create(this,rcLd,IDC_DATAWND_SPO2))
		{
			result=true;
		}
	}
	else
	{
		m_pDataSPO2->MoveWindow(x,y,nWidth,nHeight);
	}
	LeaveCriticalSection(&csDrawDataSPO2);

	
	return result;
}
// **************************************************************************
// 
// **************************************************************************
bool CViewDiagramm::CreateWndGraphSPO2(int x, int y, int nWidth, int nHeight)
{
	if(isExit())
		return false;

	bool result=false;

	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		m_bFilledGraph=false;
	}
	else
		m_bFilledGraph=getModel()->getCONFIG()->GraphIsFilled();

	EnterCriticalSection(&csDiagrammSPO2);
	if(m_pDiagrammSPO2==NULL)
	{
		m_pDiagrammSPO2=new CDiagramm();
		RECT rcLd={x,y,x+nWidth,y+nHeight};

		if(m_pDiagrammSPO2->Create(this,rcLd,IDC_LINEDIAGRAM_SPO2))
		{
			result=true;
			double dYAxisMax=0;
			double dYAxisMin=0;

			/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
			//DEBUGMSG(TRUE, (TEXT("lock 8\r\n")));
			EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			m_lpfsSPO2->FuncType=G_PLOTYPOINTS;
			m_lpfsSPO2->xMax=0;
			m_lpfsSPO2->xMin=0;
			m_lpfsSPO2->yMax=G_UPPER_MAXSCALE_SPO2;
			m_lpfsSPO2->yMin=G_LOWER_MAXSCALE_SPO2;
			m_lpfsSPO2->dXfactor=1;
			m_lpfsSPO2->dYfactor=G_FACTOR_SPO2;
			m_lpfsSPO2->pItems=m_pPlotItems;
			m_lpfsSPO2->num_PlotXYItems=0;
			m_lpfsSPO2->ChartType=G_LINECHART;
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 8\r\n")));
			
			m_pDiagrammSPO2->SetDiagrammType(SPO2_GRAPH);

			dYAxisMax=getModel()->getCONFIG()->GetMAXSCALE_SPO2_GRAPH();
			dYAxisMin=(-1)*dYAxisMax;
			
			m_pDiagrammSPO2->SetColorScheme(G_DEFAULTSCHEME,getModel()->getDATAHANDLER()->getGraphColor_SPO2());
			m_pDiagrammSPO2->SetXAxisTickSpace(GetXAxisTimeTickSpace());
			m_pDiagrammSPO2->SetXAxisScale(0, GetTimeAxisMaxScale());
			m_pDiagrammSPO2->SetYAxisScale(dYAxisMin, dYAxisMax,TRUE,false);
		}
	}
	else
	{
		double dYAxisMax=0;
		double dYAxisMin=0;
		
		m_pDiagrammSPO2->MoveWindow(x,y,nWidth,nHeight);

		/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
		//DEBUGMSG(TRUE, (TEXT("lock 9\r\n")));
		EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
		m_lpfsSPO2->FuncType=G_PLOTYPOINTS;
		m_lpfsSPO2->xMax=0;
		m_lpfsSPO2->xMin=0;
		m_lpfsSPO2->yMax=G_UPPER_MAXSCALE_SPO2;
		m_lpfsSPO2->yMin=G_LOWER_MAXSCALE_SPO2;
		m_lpfsSPO2->dXfactor=1;
		m_lpfsSPO2->dYfactor=G_FACTOR_SPO2;
		m_lpfsSPO2->pItems=m_pPlotItems;
		m_lpfsSPO2->num_PlotXYItems=0;
		m_lpfsSPO2->ChartType=G_LINECHART;
		/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
		LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
		//DEBUGMSG(TRUE, (TEXT("unlock 9\r\n")));
		
		m_pDiagrammSPO2->SetDiagrammType(SPO2_GRAPH);

		dYAxisMin=(-1)*getModel()->getCONFIG()->GetMAXSCALE_SPO2_GRAPH();
		dYAxisMax=getModel()->getCONFIG()->GetMAXSCALE_SPO2_GRAPH();

		m_pDiagrammSPO2->SetColorScheme(G_DEFAULTSCHEME,getModel()->getDATAHANDLER()->getGraphColor_SPO2());

		m_pDiagrammSPO2->SetYAxisScale(dYAxisMin, dYAxisMax,TRUE,false);
	}
	LeaveCriticalSection(&csDiagrammSPO2);

	return result;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewDiagramm::CreateWndGraphPressure(int x, int y, int nWidth, int nHeight)
{
	if(isExit())
		return false;

	bool result=false;

	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		m_bFilledGraph=false;
	}
	else
		m_bFilledGraph=getModel()->getCONFIG()->GraphIsFilled();

	
	EnterCriticalSection(&csDiagrammPRESSURE);
	if(m_pDiagrammPressure==NULL)
	{
		m_pDiagrammPressure=new CDiagramm();
		RECT rcLd={x,y,x+nWidth,y+nHeight};

		if(m_pDiagrammPressure->Create(this,rcLd,IDC_LINEDIAGRAM_PRESSURE))
		{
			result=true;
			double dYAxisMax=0;
			double dYAxisMin=0;

			if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
			{
				/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
				//DEBUGMSG(TRUE, (TEXT("lock 10\r\n")));
				EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				m_lpfsPressure->FuncType=G_PLOTYPOINTS;
				m_lpfsPressure->xMax=0;
				m_lpfsPressure->xMin=0;
				m_lpfsPressure->yMax=G_UPPER_MAXSCALE_PRESSUREHF;
				m_lpfsPressure->yMin=G_LOWER_MAXSCALE_PRESSUREHF;
				m_lpfsPressure->dXfactor=1;
				m_lpfsPressure->dYfactor=G_FACTOR_PRESSURE;
				m_lpfsPressure->pItems=m_pPlotItems;
				m_lpfsPressure->num_PlotXYItems=0;
				m_lpfsPressure->ChartType=G_LINECHART;
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 10\r\n")));

				m_pDiagrammPressure->SetDiagrammType(PRESSURE_HF_GRAPH);

				if(	getModel()->getCONFIG()->GraphIsAutoScale()
					||	getModel()->getDATAHANDLER()->GetExhalValvCalMode()==true)
				{
					int iMax=0;
					int iMin=0;
					if(		getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true 
						&&	getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
					{
						int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
						int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
						int iPmeanrec=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
						iMax=iPmeanrec+(iAmp/2);
						iMin=iPmean-(iAmp/2)-100;
					}
					else
					{
						int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
						int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
						iMax=iPmean+(iAmp/2);
						iMin=iPmean-(iAmp/2);
					}
					
					dYAxisMin=m_pDiagrammPressure->GetYAxisMinPressure(iMin);
					dYAxisMax=m_pDiagrammPressure->GetYAxisMaxPressure(iMax);
					
					getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFGRAPH(dYAxisMin);
					getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFGRAPH(dYAxisMax);
				}
				else
				{
					dYAxisMin=getModel()->getCONFIG()->GetMINSCALE_PRESSURE_HFGRAPH();
					dYAxisMax=getModel()->getCONFIG()->GetMAXSCALE_PRESSURE_HFGRAPH();
				}
				m_pDiagrammPressure->SetColorScheme(G_FabianHFOSCHEME_LINE,getModel()->getDATAHANDLER()->getGraphColor_PRESSURE());
			}
			else
			{
				/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
				//DEBUGMSG(TRUE, (TEXT("lock 11\r\n")));
				EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				m_lpfsPressure->FuncType=G_PLOTYPOINTS;
				m_lpfsPressure->xMax=0;
				m_lpfsPressure->xMin=0;
				if(getModel()->getVMODEHANDLER()->activeModeIsNMODE())
				{
					m_lpfsPressure->yMax=G_UPPER_MAXSCALE_NMODEPRESSURE;
				}
				else
				{
					m_lpfsPressure->yMax=G_UPPER_MAXSCALE_PRESSURE;
				}
				m_lpfsPressure->yMin=G_LOWER_MAXSCALE_PRESSURE;
				m_lpfsPressure->dXfactor=1;
				m_lpfsPressure->dYfactor=G_FACTOR_PRESSURE;
				m_lpfsPressure->pItems=m_pPlotItems;
				m_lpfsPressure->num_PlotXYItems=0;
				if(m_bFilledGraph)
				{
					m_lpfsPressure->ChartType=G_FILLEDCHART;
				}
				else
				{
					m_lpfsPressure->ChartType=G_LINECHART;
				}
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 11\r\n")));

				m_pDiagrammPressure->SetDiagrammType(PRESSURE_GRAPH);

				if(	getModel()->getCONFIG()->GraphIsAutoScale()
					||	getModel()->getDATAHANDLER()->GetExhalValvCalMode()==true)
				{
					int iVal=getModel()->getDATAHANDLER()->GetActiveModePRESSUREPara();//newVG
					dYAxisMax=m_pDiagrammPressure->GetYAxisMaxPressure(iVal);
					dYAxisMin=m_pDiagrammPressure->GetNegativeYoffset(dYAxisMax);
					getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_GRAPH(dYAxisMax);
				}
				else
				{
					dYAxisMax=getModel()->getCONFIG()->GetMAXSCALE_PRESSURE_GRAPH();
					dYAxisMin=m_pDiagrammPressure->GetNegativeYoffset(dYAxisMax);
				}
				m_pDiagrammPressure->SetColorScheme(G_DEFAULTSCHEME,getModel()->getDATAHANDLER()->getGraphColor_PRESSURE());
			}
					
			setDrawVolGaranty(drawVGarantyLine());
			setValueVolGaranty(getModel()->getDATAHANDLER()->GetActiveModePMAXVGPara()/10);//newVG
			//m_pDiagrammPressure->SetVolumeGaranty(getDrawVolGaranty(), getValueVolGaranty());
			setDrawHFOVolGaranty(drawHFOVGarantyLine());
			if(getDrawHFOVolGaranty())
			{
				CalculateHFOVolGaranty();
			}
			else
			{
				setValueVolGarantyHigh(0);
				setValueVolGarantyLow(0);
			}
			//m_pDiagrammPressure->SetHFOVolumeGaranty(getDrawHFOVolGaranty(), getValueVolGarantyHigh(), getValueVolGarantyLow());

			m_pDiagrammPressure->SetXAxisTickSpace(GetXAxisTimeTickSpace());
			m_pDiagrammPressure->SetXAxisScale(0, GetTimeAxisMaxScale());
			m_pDiagrammPressure->SetYAxisScale(dYAxisMin, dYAxisMax,TRUE,false);

			m_pDiagrammPressure->SetVolumeGaranty(getDrawVolGaranty(), getValueVolGaranty());
			m_pDiagrammPressure->SetHFOVolumeGaranty(getDrawHFOVolGaranty(), getValueVolGarantyHigh(), getValueVolGarantyLow());
		}
	}
	else
	{
		double dYAxisMax=0;
		double dYAxisMin=0;
		
		m_pDiagrammPressure->MoveWindow(x,y,nWidth,nHeight);

		if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
		{
			/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
			//DEBUGMSG(TRUE, (TEXT("lock 12\r\n")));
			EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			m_lpfsPressure->FuncType=G_PLOTYPOINTS;
			m_lpfsPressure->xMax=0;
			m_lpfsPressure->xMin=0;
			m_lpfsPressure->yMax=G_UPPER_MAXSCALE_PRESSUREHF;
			m_lpfsPressure->yMin=G_LOWER_MAXSCALE_PRESSUREHF;
			m_lpfsPressure->dXfactor=1;
			m_lpfsPressure->dYfactor=G_FACTOR_PRESSURE;
			m_lpfsPressure->pItems=m_pPlotItems;
			m_lpfsPressure->num_PlotXYItems=0;
			m_lpfsPressure->ChartType=G_LINECHART;
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 12\r\n")));
			
			m_pDiagrammPressure->SetDiagrammType(PRESSURE_HF_GRAPH);

			if(	getModel()->getCONFIG()->GraphIsAutoScale()
			||	getModel()->getDATAHANDLER()->GetExhalValvCalMode()==true)
			{
				int iMax=0;
				int iMin=0;
				if(		getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true 
					&&	getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
				{
					int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
					int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
					int iPmeanrec=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
					iMax=iPmeanrec+(iAmp/2);
					iMin=iPmean-(iAmp/2)-100;
				}
				else
				{
					int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
					int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
					iMax=iPmean+(iAmp/2);
					iMin=iPmean-(iAmp/2);
				}
				
				dYAxisMax=m_pDiagrammPressure->GetYAxisMaxPressure(iMax);
				dYAxisMin=m_pDiagrammPressure->GetYAxisMinPressure(iMin);
				getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFGRAPH(dYAxisMin);
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFGRAPH(dYAxisMax);
			}
			else
			{
				dYAxisMax=getModel()->getCONFIG()->GetMAXSCALE_PRESSURE_HFGRAPH();
				dYAxisMin=getModel()->getCONFIG()->GetMINSCALE_PRESSURE_HFGRAPH();
			}
			m_pDiagrammPressure->SetColorScheme(G_FabianHFOSCHEME_LINE,getModel()->getDATAHANDLER()->getGraphColor_PRESSURE());
		}
		else
		{
			/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
			//DEBUGMSG(TRUE, (TEXT("lock 13\r\n")));
			EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			m_lpfsPressure->FuncType=G_PLOTYPOINTS;
			m_lpfsPressure->xMax=0;
			m_lpfsPressure->xMin=0;
			if(getModel()->getVMODEHANDLER()->activeModeIsNMODE())
			{
				m_lpfsPressure->yMax=G_UPPER_MAXSCALE_NMODEPRESSURE;
			}
			else
			{
				m_lpfsPressure->yMax=G_UPPER_MAXSCALE_PRESSURE;
			}
			m_lpfsPressure->yMin=G_LOWER_MAXSCALE_PRESSURE;
			m_lpfsPressure->dXfactor=1;
			m_lpfsPressure->dYfactor=G_FACTOR_PRESSURE;
			m_lpfsPressure->pItems=m_pPlotItems;
			m_lpfsPressure->num_PlotXYItems=0;
			if(m_bFilledGraph)
			{
				m_lpfsPressure->ChartType=G_FILLEDCHART;
			}
			else
			{
				m_lpfsPressure->ChartType=G_LINECHART;
			}
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 13\r\n")));
			

			m_pDiagrammPressure->SetDiagrammType(PRESSURE_GRAPH);

			if(	getModel()->getCONFIG()->GraphIsAutoScale()
				||	getModel()->getDATAHANDLER()->GetExhalValvCalMode()==true)
			{
				int iVal=getModel()->getDATAHANDLER()->GetActiveModePRESSUREPara();//newVG
				dYAxisMax=m_pDiagrammPressure->GetYAxisMaxPressure(iVal);
				dYAxisMin=m_pDiagrammPressure->GetNegativeYoffset(dYAxisMax);
				getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_GRAPH(dYAxisMax);
			}
			else
			{
				dYAxisMax=getModel()->getCONFIG()->GetMAXSCALE_PRESSURE_GRAPH();
				dYAxisMin=m_pDiagrammPressure->GetNegativeYoffset(dYAxisMax);
			}
			m_pDiagrammPressure->SetColorScheme(G_DEFAULTSCHEME,getModel()->getDATAHANDLER()->getGraphColor_PRESSURE());
		}

		setDrawVolGaranty(drawVGarantyLine());
		setValueVolGaranty(getModel()->getDATAHANDLER()->GetActiveModePMAXVGPara()/10);//newVG
		//m_pDiagrammPressure->SetVolumeGaranty(getDrawVolGaranty(), getValueVolGaranty());
		setDrawHFOVolGaranty(drawHFOVGarantyLine());
		if(getDrawHFOVolGaranty())
		{
			CalculateHFOVolGaranty();
		}
		else
		{
			setValueVolGarantyHigh(0);
			setValueVolGarantyLow(0);
		}
		//m_pDiagrammPressure->SetHFOVolumeGaranty(getDrawHFOVolGaranty(), getValueVolGarantyHigh(), getValueVolGarantyLow());
		
		m_pDiagrammPressure->SetYAxisScale(dYAxisMin, dYAxisMax,TRUE,false);

		m_pDiagrammPressure->SetVolumeGaranty(getDrawVolGaranty(), getValueVolGaranty());
		m_pDiagrammPressure->SetHFOVolumeGaranty(getDrawHFOVolGaranty(), getValueVolGarantyHigh(), getValueVolGarantyLow());

	}
	LeaveCriticalSection(&csDiagrammPRESSURE);

	return result;
}
bool CViewDiagramm::CreateWndGraphVolume(int x, int y, int nWidth, int nHeight)
{
	if(isExit())
		return false;

	bool result=false;

	double dYAxisMax=0;
	double dYAxisMin=0;

	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		m_bFilledGraph=false;
	}
	else
		m_bFilledGraph=getModel()->getCONFIG()->GraphIsFilled();

	EnterCriticalSection(&csDiagrammVOLUME);
	if(m_pDiagrammVolume==NULL)
	{
		m_pDiagrammVolume=new CDiagramm();
		RECT rcLd={x,y,x+nWidth,y+nHeight};
		

		if(m_pDiagrammVolume->Create(this,rcLd,IDC_LINEDIAGRAM_VOLUME))
		{
			result=true;

			if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
			{
				/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
				//DEBUGMSG(TRUE, (TEXT("lock 14\r\n")));
				EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				m_lpfsVolume->FuncType=G_PLOTYPOINTS;
				m_lpfsVolume->xMax=0;
				m_lpfsVolume->xMin=0;
				m_lpfsVolume->yMax=G_UPPER_MAXSCALE_VOLUME_HF;
				m_lpfsVolume->yMin=G_LOWER_MAXSCALE_VOLUME_HF;
				m_lpfsVolume->dXfactor=1;
				m_lpfsVolume->dYfactor=G_FACTOR_VOLUME;
				m_lpfsVolume->pItems=m_pPlotItems;
				m_lpfsVolume->num_PlotXYItems=0;
				m_lpfsVolume->ChartType=G_LINECHART;
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 14\r\n")));
				
				m_pDiagrammVolume->SetDiagrammType(VOLUME_HF_GRAPH);
				if(	getModel()->getCONFIG()->GraphIsAutoScale())
				{
					dYAxisMax=G_LOWER_MAXSCALE_VOLUME;
				}
				else
				{
					dYAxisMax=getModel()->getCONFIG()->GetMAXSCALE_VOLUME_HFGRAPH();
				}
				
				dYAxisMin=m_pDiagrammVolume->GetNegativeYoffset(dYAxisMax);
				m_pDiagrammVolume->SetColorScheme(G_FabianHFOSCHEME_LINE,getModel()->getDATAHANDLER()->getGraphColor_VOLUME());
			}
			else
			{
				int iUPPER_MAXSCALE_VOLUME=0;
				if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
					iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_NEO;
				else
					iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_PED;

				/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
				//DEBUGMSG(TRUE, (TEXT("lock 15\r\n")));
				EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				m_lpfsVolume->FuncType=G_PLOTYPOINTS;
				m_lpfsVolume->xMax=0;
				m_lpfsVolume->xMin=0;
				m_lpfsVolume->yMax=iUPPER_MAXSCALE_VOLUME;
				m_lpfsVolume->yMin=G_LOWER_MAXSCALE_VOLUME;
				m_lpfsVolume->dXfactor=1;
				m_lpfsVolume->dYfactor=G_FACTOR_VOLUME;
				m_lpfsVolume->pItems=m_pPlotItems;
				m_lpfsVolume->num_PlotXYItems=0;
				if(m_bFilledGraph)
				{
					m_lpfsVolume->ChartType=G_FILLEDCHART;
				}
				else
				{
					m_lpfsVolume->ChartType=G_LINECHART;
				}
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 15\r\n")));
				

				m_pDiagrammVolume->SetDiagrammType(VOLUME_GRAPH);
				if(	getModel()->getCONFIG()->GraphIsAutoScale())
				{
					dYAxisMax=G_LOWER_MAXSCALE_VOLUME;
				}
				else
				{
					dYAxisMax=getModel()->getCONFIG()->GetMAXSCALE_VOLUME_GRAPH();
				}
				dYAxisMin=m_pDiagrammVolume->GetNegativeYoffset(dYAxisMax);
				m_pDiagrammVolume->SetColorScheme(G_DEFAULTSCHEME,getModel()->getDATAHANDLER()->getGraphColor_VOLUME());
			}
			
			
			m_pDiagrammVolume->SetXAxisTickSpace(GetXAxisTimeTickSpace());
			setDrawVolLimit(IsVLimitStateOn());
			setValueVolLimit((getModel()->getDATAHANDLER()->GetCurrentVLimitPara())/*/10*/);
			m_pDiagrammVolume->SetVolumeLimit(getDrawVolLimit(), getValueVolLimit());
			if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_VOLUME && getModel()->getVMODEHANDLER()->activeModeIsCONVTrigger())
			{
				m_pDiagrammVolume->SetTriggerThreshold(m_iTrigThreshold);
			}
			else
				m_pDiagrammVolume->SetTriggerThreshold(0);

			m_pDiagrammVolume->SetXAxisScale(0, GetTimeAxisMaxScale());
			m_pDiagrammVolume->SetYAxisScale(dYAxisMin,dYAxisMax,TRUE,false);
		}
	}
	else
	{
		m_pDiagrammVolume->MoveWindow(x,y,nWidth,nHeight);

		if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
		{
			/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
			//DEBUGMSG(TRUE, (TEXT("lock 16\r\n")));
			EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			m_lpfsVolume->FuncType=G_PLOTYPOINTS;
			m_lpfsVolume->xMax=0;
			m_lpfsVolume->xMin=0;
			m_lpfsVolume->yMax=G_UPPER_MAXSCALE_VOLUME_HF;
			m_lpfsVolume->yMin=G_LOWER_MAXSCALE_VOLUME_HF;
			m_lpfsVolume->dXfactor=1;
			m_lpfsVolume->dYfactor=G_FACTOR_VOLUME;
			m_lpfsVolume->pItems=m_pPlotItems;
			m_lpfsVolume->num_PlotXYItems=0;
			m_lpfsVolume->ChartType=G_LINECHART;
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 16\r\n")));

			m_pDiagrammVolume->SetDiagrammType(VOLUME_HF_GRAPH);
			if(	getModel()->getCONFIG()->GraphIsAutoScale())
			{
				dYAxisMax=G_LOWER_MAXSCALE_VOLUME;
			}
			else
			{
				dYAxisMax=getModel()->getCONFIG()->GetMAXSCALE_VOLUME_HFGRAPH();
			}
			dYAxisMin=m_pDiagrammVolume->GetNegativeYoffset(dYAxisMax);

			m_pDiagrammVolume->SetColorScheme(G_FabianHFOSCHEME_LINE,getModel()->getDATAHANDLER()->getGraphColor_VOLUME());
		}
		else
		{
			int iUPPER_MAXSCALE_VOLUME=0;
			if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
				iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_NEO;
			else
				iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_PED;

			/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
			//DEBUGMSG(TRUE, (TEXT("lock 17\r\n")));
			EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			m_lpfsVolume->FuncType=G_PLOTYPOINTS;
			m_lpfsVolume->xMax=0;
			m_lpfsVolume->xMin=0;
			m_lpfsVolume->yMax=iUPPER_MAXSCALE_VOLUME;
			m_lpfsVolume->yMin=G_LOWER_MAXSCALE_VOLUME;
			m_lpfsVolume->dXfactor=1;
			m_lpfsVolume->dYfactor=G_FACTOR_VOLUME;
			m_lpfsVolume->pItems=m_pPlotItems;
			m_lpfsVolume->num_PlotXYItems=0;
			if(m_bFilledGraph)
			{
				m_lpfsVolume->ChartType=G_FILLEDCHART;
			}
			else
			{
				m_lpfsVolume->ChartType=G_LINECHART;
			}
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 17\r\n")));
			
			m_pDiagrammVolume->SetDiagrammType(VOLUME_GRAPH);
			dYAxisMax=getModel()->getCONFIG()->GetMAXSCALE_VOLUME_GRAPH();
			
			dYAxisMin=m_pDiagrammVolume->GetNegativeYoffset(dYAxisMax);
			m_pDiagrammVolume->SetColorScheme(G_DEFAULTSCHEME,getModel()->getDATAHANDLER()->getGraphColor_VOLUME());

		}
		m_pDiagrammVolume->SetYAxisScale(dYAxisMin,dYAxisMax,TRUE,false);
	}
	LeaveCriticalSection(&csDiagrammVOLUME);

	return result;
}
bool CViewDiagramm::CreateWndGraphFlow(int x, int y, int nWidth, int nHeight)
{
	if(isExit())
		return false;

	bool result=false;

	double dYAxisMax=0;
	double dYAxisMin=0;

	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		m_bFilledGraph=false;
	}
	else
		m_bFilledGraph=getModel()->getCONFIG()->GraphIsFilled();

	EnterCriticalSection(&csDiagrammFLOW);
	if(m_pDiagrammFlow==NULL)
	{
		m_pDiagrammFlow=new CDiagramm();
		RECT rcLd={x,y,x+nWidth,y+nHeight};

		if(m_pDiagrammFlow->Create(this,rcLd,IDC_LINEDIAGRAM_FLOW))
		{
			result=true;

			if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
			{
				/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
				//DEBUGMSG(TRUE, (TEXT("lock 18\r\n")));
				EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				m_lpfsFlow->FuncType=G_PLOTYPOINTS;
				m_lpfsFlow->xMax=0;
				m_lpfsFlow->xMin=0;
				m_lpfsFlow->yMax=G_UPPER_MAXSCALE_FLOW;
				m_lpfsFlow->yMin=G_LOWER_MAXSCALE_FLOW;
				m_lpfsFlow->dXfactor=1;
				m_lpfsFlow->dYfactor=G_FACTOR_FLOW;
				m_lpfsFlow->pItems=m_pPlotItems;
				m_lpfsFlow->num_PlotXYItems=0;
				m_lpfsFlow->ChartType=G_LINECHART;
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 18\r\n")));

				m_pDiagrammFlow->SetDiagrammType(FLOW_HF_GRAPH);

				if(	getModel()->getCONFIG()->GraphIsAutoScale())
				{
					dYAxisMin=(-1)*G_LOWER_MAXSCALE_FLOW;
					dYAxisMax=G_LOWER_MAXSCALE_FLOW;
				}
				else
				{
					dYAxisMin=(-1)*getModel()->getCONFIG()->GetMAXSCALE_FLOW_HFGRAPH();
					dYAxisMax=getModel()->getCONFIG()->GetMAXSCALE_FLOW_HFGRAPH();
				}
				
				m_pDiagrammFlow->SetColorScheme(G_FabianHFOSCHEME_LINE,getModel()->getDATAHANDLER()->getGraphColor_FLOW());
			}
			else
			{
				/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
				//DEBUGMSG(TRUE, (TEXT("lock 19\r\n")));
				EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				m_lpfsFlow->FuncType=G_PLOTYPOINTS;
				m_lpfsFlow->xMax=0;
				m_lpfsFlow->xMin=0;
				m_lpfsFlow->yMax=G_UPPER_MAXSCALE_FLOW;
				m_lpfsFlow->yMin=G_LOWER_MAXSCALE_FLOW;
				m_lpfsFlow->dXfactor=1;
				m_lpfsFlow->dYfactor=G_FACTOR_FLOW;
				m_lpfsFlow->pItems=m_pPlotItems;
				m_lpfsFlow->num_PlotXYItems=0;
				if(m_bFilledGraph)
				{
					m_lpfsFlow->ChartType=G_FILLEDCHART;
				}
				else
				{
					m_lpfsFlow->ChartType=G_LINECHART;
				}
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 19\r\n")));
				
				m_pDiagrammFlow->SetDiagrammType(FLOW_GRAPH);

				if(	getModel()->getCONFIG()->GraphIsAutoScale())
				{
					dYAxisMax=G_LOWER_MAXSCALE_FLOW;
					dYAxisMin=(-1)*G_LOWER_MAXSCALE_FLOW;
				}
				else
				{
					dYAxisMin=(-1)*getModel()->getCONFIG()->GetMAXSCALE_FLOW_GRAPH();
					dYAxisMax=getModel()->getCONFIG()->GetMAXSCALE_FLOW_GRAPH();
				}
				
				m_pDiagrammFlow->SetColorScheme(G_DEFAULTSCHEME,getModel()->getDATAHANDLER()->getGraphColor_FLOW());
			}
			
			m_pDiagrammFlow->SetXAxisTickSpace(GetXAxisTimeTickSpace());

			if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_FLOW && getModel()->getVMODEHANDLER()->activeModeIsCONVTrigger())
			{
				m_pDiagrammFlow->SetTriggerThreshold(m_iTrigThreshold);
			}
			else
				m_pDiagrammFlow->SetTriggerThreshold(0);

			m_pDiagrammFlow->SetXAxisScale(0, GetTimeAxisMaxScale());
			m_pDiagrammFlow->SetYAxisScale(dYAxisMin, dYAxisMax,TRUE,false);
		}
	}
	else
	{
		if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
		{
			/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
			//DEBUGMSG(TRUE, (TEXT("lock 20\r\n")));
			EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			m_lpfsFlow->FuncType=G_PLOTYPOINTS;
			m_lpfsFlow->xMax=0;
			m_lpfsFlow->xMin=0;
			m_lpfsFlow->yMax=G_UPPER_MAXSCALE_FLOW;
			m_lpfsFlow->yMin=G_LOWER_MAXSCALE_FLOW;
			m_lpfsFlow->dXfactor=1;
			m_lpfsFlow->dYfactor=G_FACTOR_FLOW;
			m_lpfsFlow->pItems=m_pPlotItems;
			m_lpfsFlow->num_PlotXYItems=0;
			m_lpfsFlow->ChartType=G_LINECHART;
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 20\r\n")));

			m_pDiagrammFlow->SetDiagrammType(FLOW_HF_GRAPH);
			if(	getModel()->getCONFIG()->GraphIsAutoScale())
			{
				dYAxisMin=(-1)*G_LOWER_MAXSCALE_FLOW;
				dYAxisMax=G_LOWER_MAXSCALE_FLOW;
			}
			else
			{
				dYAxisMin=(-1)*getModel()->getCONFIG()->GetMAXSCALE_FLOW_HFGRAPH();
				dYAxisMax=getModel()->getCONFIG()->GetMAXSCALE_FLOW_HFGRAPH();
			}
			
			m_pDiagrammFlow->SetColorScheme(G_FabianHFOSCHEME_LINE,getModel()->getDATAHANDLER()->getGraphColor_FLOW());
		}
		else
		{
			/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
			//DEBUGMSG(TRUE, (TEXT("lock 21\r\n")));
			EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			m_lpfsFlow->FuncType=G_PLOTYPOINTS;
			m_lpfsFlow->xMax=0;
			m_lpfsFlow->xMin=0;
			m_lpfsFlow->yMax=G_UPPER_MAXSCALE_FLOW;
			m_lpfsFlow->yMin=G_LOWER_MAXSCALE_FLOW;
			m_lpfsFlow->dXfactor=1;
			m_lpfsFlow->dYfactor=G_FACTOR_FLOW;
			m_lpfsFlow->pItems=m_pPlotItems;
			m_lpfsFlow->num_PlotXYItems=0;
			if(m_bFilledGraph)
			{
				m_lpfsFlow->ChartType=G_FILLEDCHART;
			}
			else
			{
				m_lpfsFlow->ChartType=G_LINECHART;
			}
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 21\r\n")));

			m_pDiagrammFlow->SetDiagrammType(FLOW_GRAPH);
			
			dYAxisMin=(-1)*getModel()->getCONFIG()->GetMAXSCALE_FLOW_GRAPH();
				dYAxisMax=getModel()->getCONFIG()->GetMAXSCALE_FLOW_GRAPH();
			
			m_pDiagrammFlow->SetColorScheme(G_DEFAULTSCHEME,getModel()->getDATAHANDLER()->getGraphColor_FLOW());
		}
		m_pDiagrammFlow->MoveWindow(x,y,nWidth,nHeight);
		m_pDiagrammFlow->SetYAxisScale(dYAxisMin, dYAxisMax,TRUE,false);
	}
	LeaveCriticalSection(&csDiagrammFLOW);

	return result;
}
bool CViewDiagramm::CreateWndLoopPressureVolume(int x, int y, int nWidth, int nHeight)
{
	if(isExit())
		return false;

	bool result=false;

	EnterCriticalSection(&csDiagrammLOOP);
	if(m_pLoopPressureVolume==NULL)
	{
		m_pLoopPressureVolume=new CDiagramm();
		RECT rcLd={x,y,x+nWidth,y+nHeight};

		if(m_pLoopPressureVolume->Create(this,rcLd,IDC_LOOPDIAGRAM_PRESSUREVOLUME))
		{
			result=true;

			if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
			{
				/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
				//DEBUGMSG(TRUE, (TEXT("lock 22\r\n")));
				EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				m_lpfsPressureVolume->FuncType=G_PLOTXYLOOPPOINTS;
				m_lpfsPressureVolume->xMax=G_UPPER_MAXSCALE_PRESSUREHF;
				m_lpfsPressureVolume->xMin=G_LOWER_MAXSCALE_PRESSUREHF;
				m_lpfsPressureVolume->yMax=G_UPPER_MAXSCALE_VOLUME_HF;
				m_lpfsPressureVolume->yMin=G_LOWER_MAXSCALE_VOLUME_HF;
				m_lpfsPressureVolume->dXfactor=G_FACTOR_PRESSURE;
				m_lpfsPressureVolume->dYfactor=G_FACTOR_VOLUME;
				m_lpfsPressureVolume->ChartType=G_LOOPCHART;
				m_lpfsPressureVolume->pItems=m_pPlotItemsPVloop;
				m_lpfsPressureVolume->num_PlotXYItems=0;
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 22\r\n")));
				
				m_pLoopPressureVolume->SetDiagrammType(PRESSURE_VOLUME_HF_LOOP);

				if(	getModel()->getCONFIG()->GraphIsAutoScale()
					||	getModel()->getDATAHANDLER()->GetExhalValvCalMode()==true)
				{
					if(getModel()->getDATAHANDLER()->IsGraphFreezed())
					{
						int iMax=0;
						int iMin=0;
						if(		getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true 
							&&	getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
						{
							int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
							int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
							int iPmeanrec=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
							iMax=iPmeanrec+(iAmp/2);
							iMin=iPmean-(iAmp/2)-100;
						}
						else
						{
							int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
							int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
							iMax=iPmean+(iAmp/2);
							iMin=iPmean-(iAmp/2);
						}

						double dYAxisMax=m_pLoopPressureVolume->GetXAxisMaxPressure(iMax);
						double dYAxisMin=m_pLoopPressureVolume->GetXAxisMinPressure(iMin);

						m_pLoopPressureVolume->SetYAxisScale(m_pLoopPressureVolume->GetNegativeYoffset(getModel()->getCONFIG()->GetMAXSCALE_VOLUME_HFPVLOOP()), getModel()->getCONFIG()->GetMAXSCALE_VOLUME_HFPVLOOP());
						m_pLoopPressureVolume->SetXAxisScale(dYAxisMin, dYAxisMax);
						getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFPVLOOP(dYAxisMin);
						getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFPVLOOP(dYAxisMax);
					}
					else
					{
						int iMax=0;
						int iMin=0;
						if(		getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true 
							&&	getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
						{
							int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
							int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
							int iPmeanrec=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
							iMax=iPmeanrec+(iAmp/2);
							iMin=iPmean-(iAmp/2)-100;
						}
						else
						{
							int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
							int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
							iMax=iPmean+(iAmp/2);
							iMin=iPmean-(iAmp/2);
						}
						double dYAxisMax=m_pLoopPressureVolume->GetXAxisMaxPressure(iMax);
						double dYAxisMin=m_pLoopPressureVolume->GetXAxisMinPressure(iMin);

						m_pLoopPressureVolume->SetXAxisScale(dYAxisMin, dYAxisMax);
						getModel()->getCONFIG()->SetMINSCALE_PRESSURE_HFPVLOOP(dYAxisMin);
						getModel()->getCONFIG()->SetMAXSCALE_PRESSURE_HFPVLOOP(dYAxisMax);
						
						m_pLoopPressureVolume->SetYAxisScale(m_pLoopPressureVolume->GetNegativeYoffset(G_LOWER_MAXSCALE_VOLUME), G_LOWER_MAXSCALE_VOLUME);
					}					
				}
				else
				{
					m_pLoopPressureVolume->SetYAxisScale(m_pLoopPressureVolume->GetNegativeYoffset(getModel()->getCONFIG()->GetMAXSCALE_VOLUME_HFPVLOOP()), getModel()->getCONFIG()->GetMAXSCALE_VOLUME_HFPVLOOP());
					m_pLoopPressureVolume->SetXAxisScale(getModel()->getCONFIG()->GetMINSCALE_PRESSURE_HFPVLOOP(), getModel()->getCONFIG()->GetMAXSCALE_PRESSURE_HFPVLOOP());
				}
			}
			else
			{
				int iUPPER_MAXSCALE_VOLUME=0;
				if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
					iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_NEO;
				else
					iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_PED;

				/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
				//DEBUGMSG(TRUE, (TEXT("lock 23\r\n")));
				EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				m_lpfsPressureVolume->FuncType=G_PLOTXYLOOPPOINTS;
				m_lpfsPressureVolume->xMax=G_UPPER_MAXSCALE_PRESSURE;
				m_lpfsPressureVolume->xMin=G_LOWER_MAXSCALE_PRESSURE;
				m_lpfsPressureVolume->yMax=iUPPER_MAXSCALE_VOLUME;
				m_lpfsPressureVolume->yMin=G_LOWER_MAXSCALE_VOLUME;
				m_lpfsPressureVolume->dXfactor=G_FACTOR_PRESSURE;
				m_lpfsPressureVolume->dYfactor=G_FACTOR_VOLUME;
				m_lpfsPressureVolume->ChartType=G_LOOPCHART;
				m_lpfsPressureVolume->pItems=m_pPlotItemsPVloop;
				m_lpfsPressureVolume->num_PlotXYItems=0;
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 23\r\n")));

				m_pLoopPressureVolume->SetDiagrammType(PRESSURE_VOLUME_LOOP);

				if(	getModel()->getCONFIG()->GraphIsAutoScale()
					||	getModel()->getDATAHANDLER()->GetExhalValvCalMode()==true)
				{
					if(getModel()->getDATAHANDLER()->IsGraphFreezed())
					{
						m_pLoopPressureVolume->SetYAxisScale(m_pLoopPressureVolume->GetNegativeYoffset(getModel()->getCONFIG()->GetMAXSCALE_VOLUME_PVLOOP()), getModel()->getCONFIG()->GetMAXSCALE_VOLUME_PVLOOP());
						m_pLoopPressureVolume->SetXAxisScale(m_pLoopPressureVolume->GetNegativeYoffset(getModel()->getCONFIG()->GetMAXSCALE_PRESSURE_PVLOOP()), getModel()->getCONFIG()->GetMAXSCALE_PRESSURE_PVLOOP());

					}
					else
					{
						m_pLoopPressureVolume->SetYAxisScale(m_pLoopPressureVolume->GetNegativeYoffset(G_LOWER_MAXSCALE_VOLUME), G_LOWER_MAXSCALE_VOLUME);
						m_pLoopPressureVolume->SetXAxisScale(m_pLoopPressureVolume->GetNegativeYoffset(getModel()->getCONFIG()->GetMAXSCALE_PRESSURE_PVLOOP()), getModel()->getCONFIG()->GetMAXSCALE_PRESSURE_PVLOOP());
					}

				}
				else
				{
					m_pLoopPressureVolume->SetYAxisScale(m_pLoopPressureVolume->GetNegativeYoffset(getModel()->getCONFIG()->GetMAXSCALE_VOLUME_PVLOOP()), getModel()->getCONFIG()->GetMAXSCALE_VOLUME_PVLOOP());
					m_pLoopPressureVolume->SetXAxisScale(m_pLoopPressureVolume->GetNegativeYoffset(getModel()->getCONFIG()->GetMAXSCALE_PRESSURE_PVLOOP()), getModel()->getCONFIG()->GetMAXSCALE_PRESSURE_PVLOOP());

				}
			}
			m_pLoopPressureVolume->SetColorScheme(G_FabianHFOSCHEME_LOOP,getModel()->getDATAHANDLER()->getGraphColor_LOOP());

			m_pLoopPressureVolume->PaintGraph();
			m_pLoopPressureVolume->Show(true);
		}
	}
	LeaveCriticalSection(&csDiagrammLOOP);

	return result;
}
bool CViewDiagramm::CreateWndLoopVolumeFlow(int x, int y, int nWidth, int nHeight)
{
	if(isExit())
		return false;

	bool result=false;

	EnterCriticalSection(&csDiagrammLOOP);
	if(m_pLoopVolumeFlow==NULL)
	{
		m_pLoopVolumeFlow=new CDiagramm();
		RECT rcLd={x,y,x+nWidth,y+nHeight};

		if(m_pLoopVolumeFlow->Create(this,rcLd,IDC_LOOPDIAGRAM_VOLUMEFLOW))
		{
			result=true;

			if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
			{
				/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
				//DEBUGMSG(TRUE, (TEXT("lock 24\r\n")));
				EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				m_lpfsVolumeFlow->FuncType=G_PLOTXYLOOPPOINTS;
				m_lpfsVolumeFlow->xMax=G_UPPER_MAXSCALE_VOLUME_HF;
				m_lpfsVolumeFlow->xMin=G_LOWER_MAXSCALE_VOLUME_HF;
				m_lpfsVolumeFlow->yMax=G_UPPER_MAXSCALE_FLOW;
				m_lpfsVolumeFlow->yMin=G_LOWER_MAXSCALE_FLOW;
				m_lpfsVolumeFlow->dXfactor=G_FACTOR_VOLUME;
				m_lpfsVolumeFlow->dYfactor=G_FACTOR_FLOW;
				m_lpfsVolumeFlow->ChartType=G_LOOPCHART;
				m_lpfsVolumeFlow->pItems=m_pPlotItemsVFloop;
				m_lpfsVolumeFlow->num_PlotXYItems=0;
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 24\r\n")));

				m_pLoopVolumeFlow->SetDiagrammType(VOLUME_FLOW_HF_LOOP);

				if(	getModel()->getCONFIG()->GraphIsAutoScale()
					||	getModel()->getDATAHANDLER()->GetExhalValvCalMode()==true)
				{
					if(getModel()->getDATAHANDLER()->IsGraphFreezed())
					{
						m_pLoopVolumeFlow->SetYAxisScale((-1)*getModel()->getCONFIG()->GetMAXSCALE_FLOW_HFVFLOOP(), getModel()->getCONFIG()->GetMAXSCALE_FLOW_HFVFLOOP());
						m_pLoopVolumeFlow->SetXAxisScale(m_pLoopVolumeFlow->GetNegativeYoffset(getModel()->getCONFIG()->GetMAXSCALE_VOLUME_HFVFLOOP()), getModel()->getCONFIG()->GetMAXSCALE_VOLUME_HFVFLOOP());
					}
					else
					{
						m_pLoopVolumeFlow->SetYAxisScale((-1)*G_LOWER_MAXSCALE_FLOW, G_LOWER_MAXSCALE_FLOW);
						m_pLoopVolumeFlow->SetXAxisScale(m_pLoopVolumeFlow->GetNegativeYoffset(G_LOWER_MAXSCALE_VOLUME), G_LOWER_MAXSCALE_VOLUME);
					}
				}
				else
				{
					m_pLoopVolumeFlow->SetYAxisScale((-1)*getModel()->getCONFIG()->GetMAXSCALE_FLOW_HFVFLOOP(), getModel()->getCONFIG()->GetMAXSCALE_FLOW_HFVFLOOP());
					m_pLoopVolumeFlow->SetXAxisScale(m_pLoopVolumeFlow->GetNegativeYoffset(getModel()->getCONFIG()->GetMAXSCALE_VOLUME_HFVFLOOP()), getModel()->getCONFIG()->GetMAXSCALE_VOLUME_HFVFLOOP());
				}
			}
			else
			{
				int iUPPER_MAXSCALE_VOLUME=0;
				if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
					iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_NEO;
				else
					iUPPER_MAXSCALE_VOLUME=G_UPPER_MAXSCALE_VOLUME_PED;

				/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
				//DEBUGMSG(TRUE, (TEXT("lock 25\r\n")));
				EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				m_lpfsVolumeFlow->FuncType=G_PLOTXYLOOPPOINTS;
				m_lpfsVolumeFlow->xMax=iUPPER_MAXSCALE_VOLUME;
				m_lpfsVolumeFlow->xMin=G_LOWER_MAXSCALE_VOLUME;
				m_lpfsVolumeFlow->yMax=G_UPPER_MAXSCALE_FLOW;
				m_lpfsVolumeFlow->yMin=G_LOWER_MAXSCALE_FLOW;
				m_lpfsVolumeFlow->dXfactor=G_FACTOR_VOLUME;
				m_lpfsVolumeFlow->dYfactor=G_FACTOR_FLOW;
				m_lpfsVolumeFlow->ChartType=G_LOOPCHART;
				m_lpfsVolumeFlow->pItems=m_pPlotItemsVFloop;
				m_lpfsVolumeFlow->num_PlotXYItems=0;
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 25\r\n")));

				m_pLoopVolumeFlow->SetDiagrammType(VOLUME_FLOW_LOOP);

				if(	getModel()->getCONFIG()->GraphIsAutoScale()
					||	getModel()->getDATAHANDLER()->GetExhalValvCalMode()==true)
				{
					if(getModel()->getDATAHANDLER()->IsGraphFreezed())
					{
						m_pLoopVolumeFlow->SetYAxisScale((-1)*getModel()->getCONFIG()->GetMAXSCALE_FLOW_VFLOOP(), getModel()->getCONFIG()->GetMAXSCALE_FLOW_VFLOOP());
						m_pLoopVolumeFlow->SetXAxisScale(m_pLoopVolumeFlow->GetNegativeYoffset(getModel()->getCONFIG()->GetMAXSCALE_VOLUME_VFLOOP()), getModel()->getCONFIG()->GetMAXSCALE_VOLUME_VFLOOP());
					}
					else
					{
						m_pLoopVolumeFlow->SetYAxisScale((-1)*G_LOWER_MAXSCALE_FLOW, G_LOWER_MAXSCALE_FLOW);
						m_pLoopVolumeFlow->SetXAxisScale(m_pLoopVolumeFlow->GetNegativeYoffset(G_LOWER_MAXSCALE_VOLUME), G_LOWER_MAXSCALE_VOLUME);
					}
				}
				else
				{
					m_pLoopVolumeFlow->SetYAxisScale((-1)*getModel()->getCONFIG()->GetMAXSCALE_FLOW_VFLOOP(), getModel()->getCONFIG()->GetMAXSCALE_FLOW_VFLOOP());
					m_pLoopVolumeFlow->SetXAxisScale(m_pLoopVolumeFlow->GetNegativeYoffset(getModel()->getCONFIG()->GetMAXSCALE_VOLUME_VFLOOP()), getModel()->getCONFIG()->GetMAXSCALE_VOLUME_VFLOOP());

				}
			}

			m_pLoopVolumeFlow->SetColorScheme(G_FabianHFOSCHEME_LOOP,getModel()->getDATAHANDLER()->getGraphColor_LOOP());

			m_pLoopVolumeFlow->PaintGraph();
			m_pLoopVolumeFlow->Show(true);
		}
	}
	LeaveCriticalSection(&csDiagrammLOOP);

	return result;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewDiagramm::DestroyWndDataCO2()
{
	m_bDataCO2IsActive=false;

	EnterCriticalSection(&csDrawDataCO2);
	if(m_pDataCO2)
	{
		m_pDataCO2->DestroyWindow();
		delete m_pDataCO2;
		m_pDataCO2=NULL;
	}
	LeaveCriticalSection(&csDrawDataCO2);
	return true;
}
// **************************************************************************
// 
// **************************************************************************
bool CViewDiagramm::DestroyWndDataFOT()
{
	m_bDataFOTIsActive=false;

	EnterCriticalSection(&csDrawDataFOT);
	if(m_pDataFOT)
	{
		m_pDataFOT->DestroyWindow();
		delete m_pDataFOT;
		m_pDataFOT=NULL;
	}
	LeaveCriticalSection(&csDrawDataFOT);
	return true;
}
bool CViewDiagramm::DestroyWndGraphFOT()
{
	m_bLoopFOTIsActive=false;

	EnterCriticalSection(&csDiagrammFOT);
	if(m_pLoopFOT)
	{
		m_pLoopFOT->DestroyWindow();
		delete m_pLoopFOT;
		m_pLoopFOT=NULL;
	}
	EnterCriticalSection(&csDiagrammFOT);

	return true;
}
// **************************************************************************
// 
// **************************************************************************
bool CViewDiagramm::DestroyWndDataSPO2()
{
	m_bDataSPO2IsActive=false;
	
	EnterCriticalSection(&csDrawDataSPO2);
	if(m_pDataSPO2)
	{
		m_pDataSPO2->DestroyWindow();
		delete m_pDataSPO2;
		m_pDataSPO2=NULL;
		
	}
	LeaveCriticalSection(&csDrawDataSPO2);
	return true;
}
bool CViewDiagramm::DestroyWndGraphSPO2()
{
	m_bGraphSPO2IsActive=false;

	EnterCriticalSection(&csDiagrammSPO2);
	if(m_pDiagrammSPO2)
	{
		m_pDiagrammSPO2->DestroyWindow();
		delete m_pDiagrammSPO2;
		m_pDiagrammSPO2=NULL;
	}
	LeaveCriticalSection(&csDiagrammSPO2);
	return true;
}
// **************************************************************************
// 
// **************************************************************************
bool CViewDiagramm::DestroyWndGraphCO2()
{
	m_bGraphCO2IsActive=false;

	EnterCriticalSection(&csDiagrammCO2);
	if(m_pDiagrammCO2)
	{
		m_pDiagrammCO2->DestroyWindow();
		delete m_pDiagrammCO2;
		m_pDiagrammCO2=NULL;
	}
	LeaveCriticalSection(&csDiagrammCO2);
	return true;
}
bool CViewDiagramm::DestroyWndGraphPressure()
{
	m_bGraphPressureIsActive=false;

	EnterCriticalSection(&csDiagrammPRESSURE);
	if(m_pDiagrammPressure)
	{
		m_pDiagrammPressure->DestroyWindow();
		delete m_pDiagrammPressure;
		m_pDiagrammPressure=NULL;
	}
	LeaveCriticalSection(&csDiagrammPRESSURE);
	return true;
}
bool CViewDiagramm::DestroyWndGraphVolume()
{
	m_bGraphVolumeIsActive=false;

	EnterCriticalSection(&csDiagrammVOLUME);
	if (m_pDiagrammVolume)
	{
		m_pDiagrammVolume->DestroyWindow();
		delete m_pDiagrammVolume;
		m_pDiagrammVolume=NULL;
	}
	LeaveCriticalSection(&csDiagrammVOLUME);
	return true;
}
bool CViewDiagramm::DestroyWndGraphFlow()
{
	m_bGraphFlowIsActive=false;

	EnterCriticalSection(&csDiagrammFLOW);
	if (m_pDiagrammFlow)
	{
		m_pDiagrammFlow->DestroyWindow();
		delete m_pDiagrammFlow;
		m_pDiagrammFlow=NULL;
	}
	LeaveCriticalSection(&csDiagrammFLOW);
	return true;
}
bool CViewDiagramm::DestroyWndLoopPressureVolume()
{
	m_bLoopPressureVolumeIsActive=false;

	EnterCriticalSection(&csDiagrammLOOP);
	if (m_pLoopPressureVolume)
	{
		m_pLoopPressureVolume->DestroyWindow();
		delete m_pLoopPressureVolume;
		m_pLoopPressureVolume=NULL;
	}
	LeaveCriticalSection(&csDiagrammLOOP);
	return true;
}
bool CViewDiagramm::DestroyWndLoopVolumeFlow()
{
	m_bLoopVolumeFlowIsActive=false;

	EnterCriticalSection(&csDiagrammLOOP);
	if (m_pLoopVolumeFlow)
	{
		m_pLoopVolumeFlow->DestroyWindow();
		delete m_pLoopVolumeFlow;
		m_pLoopVolumeFlow=NULL;
	}
	LeaveCriticalSection(&csDiagrammLOOP);
	return true;
}



// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::ShowWndDataFOT(bool bShow)
{
	if(isExit())
		return;

	EnterCriticalSection(&csDrawDataFOT);
	if(m_pDataFOT)
	{
		if(bShow)
		{
			m_pDataFOT->Show(true);
		}
		else
		{
			m_pDataFOT->Show(false);
		}
		m_bDataFOTIsActive=bShow;
	}
	LeaveCriticalSection(&csDrawDataFOT);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::ShowWndDataSPO2(bool bShow)
{
	if(isExit())
		return;
	EnterCriticalSection(&csDrawDataSPO2);
	if(m_pDataSPO2)
	{
		if(bShow)
		{
			m_pDataSPO2->Show(true);
		}
		else
		{
			m_pDataSPO2->Show(false);
		}
		m_bDataSPO2IsActive=bShow;
	}
	LeaveCriticalSection(&csDrawDataSPO2);
	
}


// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::ShowWndDataCO2(bool bShow)
{
	if(isExit())
		return;
	EnterCriticalSection(&csDrawDataCO2);
	if(m_pDataCO2)
	{
		if(bShow)
		{
			m_pDataCO2->Show(true);
		}
		else
		{
			m_pDataCO2->Show(false);
		}
		m_bDataCO2IsActive=bShow;
	}
	LeaveCriticalSection(&csDrawDataCO2);
	
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::ShowWndGraphFOT(bool bShow)
{
	if(isExit())
		return;
	EnterCriticalSection(&csDiagrammFOT);
	if(m_pLoopFOT)
	{
		if(bShow)
		{
			m_pLoopFOT->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pLoopFOT->ShowWindow(SW_HIDE);
		}
		m_bLoopFOTIsActive=bShow;
	}
	LeaveCriticalSection(&csDiagrammFOT);
	
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::ShowWndGraphSPO2(bool bShow)
{
	if(isExit())
		return;
	EnterCriticalSection(&csDiagrammSPO2);
	if(m_pDiagrammSPO2)
	{
		if(bShow)
		{
			m_pDiagrammSPO2->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pDiagrammSPO2->ShowWindow(SW_HIDE);
		}
		m_bGraphSPO2IsActive=bShow;
	}
	LeaveCriticalSection(&csDiagrammSPO2);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::ShowWndGraphCO2(bool bShow)
{
	if(isExit())
		return;
	EnterCriticalSection(&csDiagrammCO2);
	if(m_pDiagrammCO2)
	{
		if(bShow)
		{
			m_pDiagrammCO2->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pDiagrammCO2->ShowWindow(SW_HIDE);
		}
		m_bGraphCO2IsActive=bShow;
	}
	LeaveCriticalSection(&csDiagrammCO2);
}
void CViewDiagramm::ShowWndGraphPressure(bool bShow)
{
	if(isExit())
		return;
	EnterCriticalSection(&csDiagrammPRESSURE);
	if(m_pDiagrammPressure)
	{
		if(bShow)
		{
			m_pDiagrammPressure->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pDiagrammPressure->ShowWindow(SW_HIDE);
		}
		m_bGraphPressureIsActive=bShow;
	}
	LeaveCriticalSection(&csDiagrammPRESSURE);
}
void CViewDiagramm::ShowWndGraphVolume(bool bShow)
{
	if(isExit())
		return;
	EnterCriticalSection(&csDiagrammVOLUME);
	if(m_pDiagrammVolume)
	{
		if(bShow)
		{
			m_pDiagrammVolume->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pDiagrammVolume->ShowWindow(SW_HIDE);
		}
		m_bGraphVolumeIsActive=bShow;
	}
	LeaveCriticalSection(&csDiagrammVOLUME);
}
void CViewDiagramm::ShowWndGraphFlow(bool bShow)
{
	if(isExit())
		return;
	EnterCriticalSection(&csDiagrammFLOW);
	if(m_pDiagrammFlow)
	{
		if(bShow)
		{
			m_pDiagrammFlow->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pDiagrammFlow->ShowWindow(SW_HIDE);
		}
		m_bGraphFlowIsActive=bShow;
	}
	LeaveCriticalSection(&csDiagrammFLOW);
}
void CViewDiagramm::ShowWndLoopPressureVolume(bool bShow)
{
	if(isExit())
		return;
	EnterCriticalSection(&csDiagrammLOOP);
	if(m_pLoopPressureVolume)
	{
		if(bShow)
		{
			m_pLoopPressureVolume->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pLoopPressureVolume->ShowWindow(SW_HIDE);
		}
		m_bLoopPressureVolumeIsActive=bShow;
	}
	LeaveCriticalSection(&csDiagrammLOOP);
}
void CViewDiagramm::ShowWndLoopVolumeFlow(bool bShow)
{
	if(isExit())
		return;
	EnterCriticalSection(&csDiagrammLOOP);
	if(m_pLoopVolumeFlow)
	{
		if(bShow)
		{
			m_pLoopVolumeFlow->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pLoopVolumeFlow->ShowWindow(SW_HIDE);
		}
		m_bLoopVolumeFlowIsActive=bShow;
	}
	LeaveCriticalSection(&csDiagrammLOOP);
}

bool CViewDiagramm::IsVLimitStateOn()
{
	bool bRes=false;

	bRes=getModel()->getDATAHANDLER()->IsActiveModeVLimitStateOn();

	return bRes;
}

bool CViewDiagramm::drawHFOVGarantyLine()
{
	bool bRes=false;

	if(getModel()->isMANBREATHrunning())
	{
		bRes=false;
	}
	else if(		getModel()->getCONFIG()->CurModeIsPresetMode() 
		&&	getModel()->getCONFIG()->GetPrevMode()==VM_HFO)
	{
		bRes=getModel()->getDATAHANDLER()->IsPrevVGarantStateOn();
	}
	else if(getModel()->getCONFIG()->GetCurMode()==VM_HFO)
	{
		bRes=getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn();
	}
	else 
	{
		bRes=false;
	}
	return bRes;
}


bool CViewDiagramm::drawVGarantyLine()
{
	bool bRes=false;

	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		bRes=false;
	}
	else if(getModel()->getCONFIG()->CurModeIsPresetMode())
	{
		bRes=getModel()->getDATAHANDLER()->IsPrevVGarantStateOn();
	}
	else 
	{
		bRes=getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn();
	}
	return bRes;
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::StopDiagramm(bool bIgnoreFreeze)
{
	if(!m_bFreeze || bIgnoreFreeze)
		StopGraphThread();
	//DEBUGMSG(TRUE, (TEXT("xxxxxxxxxxxxxxx StopDiagramm\r\n")));
}


// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::Show(bool bNextGraph)
{
	drawView(bNextGraph);
}


// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::drawView(bool bNextGraph)
{
	if(isExit())
		return;
	DrawCursor(0);

	if(!m_bFreeze)
	{
		if(m_pcwtGraphThread!=NULL)
			StopGraphThread();

		getModel()->getDATAHANDLER()->updateCopyDataBuffer();

		EnterCriticalSection(&getModel()->getDATAHANDLER()->csSPIDataBuffer);
		getModel()->getDATAHANDLER()->m_rbufSPI.reset();
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csSPIDataBuffer);

		EnterCriticalSection(&getModel()->getDATAHANDLER()->csCO2DataBuffer);
		getModel()->getDATAHANDLER()->m_rbufCO2.reset();
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCO2DataBuffer);

		EnterCriticalSection(&getModel()->getDATAHANDLER()->csSPO2DataBuffer);
		getModel()->getDATAHANDLER()->m_rbufSPO2.reset();
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csSPO2DataBuffer);

		resetCurrentXtimevalGraphs();
	}

	//Sleep(0); //rku check1

	eViewState curViewState = getModel()->getVIEWHANDLER()->getViewState();

	switch(curViewState)
	{
	case VS_GRAPH:
		{
			drawGraphview(bNextGraph);
		}
		break;
	case VS_ALARM_LIMIT:
	case VS_PARA:
		{

			drawParaview();
		}
		break;
	default:
		{
			//rku check1
			theApp.getLog()->WriteLine(_T("#ERROR: CViewDiagramm::drawView default"));
			EnterCriticalSection(&csGraphButton);
			if(m_pcGraph1)
				m_pcGraph1->ShowWindow(SW_HIDE);
			LeaveCriticalSection(&csGraphButton);

			DestroyWndMenuGraphs();
			DestroyWndGraphPressure();
			DestroyWndGraphVolume();
			DestroyWndGraphFlow();
			DestroyWndLoopPressureVolume();
			DestroyWndLoopVolumeFlow();
			DestroyWndGraphCO2();
			DestroyWndDataCO2();
			DestroyWndGraphSPO2();
			DestroyWndDataSPO2();
			DestroyWndGraphFOT();
			DestroyWndDataFOT();
		}
		break;
	}

	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		m_bFilledGraph=false;
	}
	else
		m_bFilledGraph=getModel()->getCONFIG()->GraphIsFilled();

	//rku check1
	if(!m_bFreeze && !isExit())
	{
		StartGraphThread();
	}
}


// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::drawParaview()
{
	if(isExit())
		return;
	
	if(m_bFreeze)
	{
		m_bFreezeNextBreath=false;
		m_bFreeze=false;
		getModel()->getDATAHANDLER()->SetGraphFreezed(false);
		m_bDrawSavedLoops=false;
	}

	if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
		||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
		||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF
		||	getModel()->getVMODEHANDLER()->activeModeIsDUOPAP()
		||	(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getVMODEHANDLER()->activeModeIsNCPAP()) )
	{
		MoveWindow(0,48,605,164);

		m_iCurrentLineDiagramm=IDC_LINEDIAGRAM_PRESSURE;
		getModel()->getCONFIG()->GraphSetPrimaryLineDiagramm(m_iCurrentLineDiagramm);

		EnterCriticalSection(&csGraphButton);
		if(m_pcGraph1)
			m_pcGraph1->ShowWindow(SW_HIDE);
		LeaveCriticalSection(&csGraphButton);

		DestroyWndGraphPressure();
		DestroyWndGraphVolume();
		DestroyWndGraphFlow();
		DestroyWndLoopPressureVolume();
		DestroyWndLoopVolumeFlow();
		DestroyWndGraphCO2();
		DestroyWndDataCO2();
		DestroyWndGraphSPO2();
		DestroyWndDataSPO2();
		DestroyWndGraphFOT();
		DestroyWndDataFOT();
		
		CreateWndGraphPressure(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
		ShowWndGraphPressure(true);

		if(m_bGraphCO2IsActive)
		{
			setTextGraph2(getModel()->GetLanguageString(IDS_MENU_CO2));
		}

		drawGraphTXT(true, false, false);
		setRedrawDiagram(REDRAW_PRESSUREGRAPH);
	}
	else
	{
		MoveWindow(0,48,605,164);

		EnterCriticalSection(&csGraphButton);
		if(m_pcGraph1)
			m_pcGraph1->ShowWindow(SW_SHOW);
		LeaveCriticalSection(&csGraphButton);

		DestroyWndGraphPressure();
		DestroyWndGraphVolume();
		DestroyWndGraphFlow();
		DestroyWndLoopPressureVolume();
		DestroyWndLoopVolumeFlow();
		DestroyWndGraphCO2();
		DestroyWndDataCO2();
		DestroyWndGraphSPO2();
		DestroyWndDataSPO2();
		DestroyWndGraphFOT();
		DestroyWndDataFOT();

		if(m_iCurrentLineDiagramm==IDC_LINEDIAGRAM_VOLUME)
		{
			EnterCriticalSection(&csGraphButton);
			if(m_pcGraph1)
				m_pcGraph1->RefreshText(getModel()->GetLanguageString(IDS_BTN_SELVOLUMEGRAPH));
			LeaveCriticalSection(&csGraphButton);
			
			CreateWndGraphVolume(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
			ShowWndGraphVolume(true);

			setRedrawDiagram(REDRAW_VOLUMEGRAPH);
		}
		else if(m_iCurrentLineDiagramm==IDC_LINEDIAGRAM_FLOW)
		{
			EnterCriticalSection(&csGraphButton);
			if(m_pcGraph1)
				m_pcGraph1->RefreshText(getModel()->GetLanguageString(IDS_BTN_SELFLOWGRAPH));
			LeaveCriticalSection(&csGraphButton);
			
			CreateWndGraphFlow(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
			ShowWndGraphFlow(true);

			setRedrawDiagram(REDRAW_FLOWGRAPH);
		}
		else
		{
			EnterCriticalSection(&csGraphButton);
			if(m_pcGraph1)
				m_pcGraph1->RefreshText(getModel()->GetLanguageString(IDS_BTN_SELPRESSUREGRAPH));
			LeaveCriticalSection(&csGraphButton);
			
			CreateWndGraphPressure(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
			ShowWndGraphPressure(true);

			setRedrawDiagram(REDRAW_PRESSUREGRAPH);
		}
		drawGraphTXT(false, false, false);
	}

	DestroyWndMenuGraphs();
	UpdateWindow();
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::drawGraphview(bool bNextGraph)
{
	if(isExit())
		return;

	CreateWndMenuGraphs();
	ShowWndMenuGraphs(true);


	if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
	{
		if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF
			||	getModel()->getVMODEHANDLER()->activeModeIsDUOPAP()
			||	(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getVMODEHANDLER()->activeModeIsNCPAP()) )
		{
			DestroyWndGraphPressure();
			DestroyWndGraphVolume();
			DestroyWndGraphFlow();
			DestroyWndLoopPressureVolume();
			DestroyWndLoopVolumeFlow();
			DestroyWndGraphCO2();
			DestroyWndDataCO2();
			DestroyWndGraphSPO2();
			DestroyWndDataSPO2();
			DestroyWndGraphFOT();
			DestroyWndDataFOT();
			
			MoveWindow(0,48,605,488);

			EnterCriticalSection(&csGraphButton);
			if(m_pcGraph1)
				m_pcGraph1->ShowWindow(SW_HIDE);
			LeaveCriticalSection(&csGraphButton);
			
			//setTextGraph2(_T("FOT"));
			drawGraphTXT(true, true, false,true);

			m_iCurrentLineDiagramm=IDC_LINEDIAGRAM_PRESSURE;
			getModel()->getCONFIG()->GraphSetPrimaryLineDiagramm(m_iCurrentLineDiagramm);

			
			CreateWndGraphPressure(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
			ShowWndGraphPressure(true);

			if(!m_bFreeze)//rku DIAG
			{
				setRedrawDiagram(REDRAW_GRAPHS);
			}

			CreateWndGraphFOT(10,173,LOOP_WIDTH,LOOP_HEIGHT);
			ShowWndGraphFOT(true);

			CreateWndDataFOT(315,173,LOOP_WIDTH,LOOP_HEIGHT);
			ShowWndDataFOT(true);

			if(getModel()->getFOTThread())
			{
				if(getModel()->getFOTThread()->getBufSizeFOTdisplay()>0)
				{
					PostMessage(WM_DRAW_FOT_STEP);
				}
			}
			
		}
		else
		{
			DestroyWndGraphPressure();
			DestroyWndGraphVolume();
			DestroyWndGraphFlow();
			if(!bNextGraph)
			{
				DestroyWndLoopPressureVolume();
				DestroyWndLoopVolumeFlow();
				DestroyWndGraphCO2();
				DestroyWndDataCO2();
				DestroyWndGraphSPO2();
				DestroyWndDataSPO2();
				DestroyWndGraphFOT();
				DestroyWndDataFOT();
			}
			

			MoveWindow(0,48,605,488);

			EnterCriticalSection(&csGraphButton);
			if(m_pcGraph1)
				m_pcGraph1->ShowWindow(SW_SHOW);
			LeaveCriticalSection(&csGraphButton);
			
			drawGraphTXT(false, false, false);
		

			if(m_iCurrentLineDiagramm==IDC_LINEDIAGRAM_VOLUME)
			{
				EnterCriticalSection(&csGraphButton);
				if(m_pcGraph1)
					m_pcGraph1->RefreshText(getModel()->GetLanguageString(IDS_BTN_SELVOLUMEGRAPH));
				LeaveCriticalSection(&csGraphButton);
				
				CreateWndGraphVolume(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
				ShowWndGraphVolume(true);

				if(!m_bFreeze)//rku DIAG
				{
					setRedrawDiagram(REDRAW_GRAPHS);
				}
			}
			else if(m_iCurrentLineDiagramm==IDC_LINEDIAGRAM_FLOW)
			{
				EnterCriticalSection(&csGraphButton);
				if(m_pcGraph1)
					m_pcGraph1->RefreshText(getModel()->GetLanguageString(IDS_BTN_SELFLOWGRAPH));
				LeaveCriticalSection(&csGraphButton);
				
				CreateWndGraphFlow(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
				ShowWndGraphFlow(true);

				if(!m_bFreeze)//rku DIAG
				{
					setRedrawDiagram(REDRAW_GRAPHS);
				}
			}
			else
			{
				EnterCriticalSection(&csGraphButton);
				if(m_pcGraph1)
					m_pcGraph1->RefreshText(getModel()->GetLanguageString(IDS_BTN_SELPRESSUREGRAPH));
				LeaveCriticalSection(&csGraphButton);
				
				CreateWndGraphPressure(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
				ShowWndGraphPressure(true);

				if(!m_bFreeze)//rku DIAG
				{
					setRedrawDiagram(REDRAW_GRAPHS);
				}
			}
			if(!bNextGraph)
			{
				CreateWndGraphFOT(10,173,LOOP_WIDTH,LOOP_HEIGHT);
				ShowWndGraphFOT(true);

				CreateWndDataFOT(315,173,LOOP_WIDTH,LOOP_HEIGHT);
				ShowWndDataFOT(true);

				if(getModel()->getFOTThread())
				{
					if(getModel()->getFOTThread()->getBufSizeFOTdisplay()>0)
					{
						PostMessage(WM_DRAW_FOT_STEP);
					}
				}
			}
		}
	}
	else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
	{
		if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF
			||	getModel()->getVMODEHANDLER()->activeModeIsDUOPAP()
			||	(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getVMODEHANDLER()->activeModeIsNCPAP()) )
		{
			DestroyWndGraphPressure();
			DestroyWndGraphVolume();
			DestroyWndGraphFlow();
			DestroyWndLoopPressureVolume();
			DestroyWndLoopVolumeFlow();
			DestroyWndGraphCO2();
			DestroyWndDataCO2();
			DestroyWndGraphSPO2();
			DestroyWndDataSPO2();
			DestroyWndGraphFOT();
			DestroyWndDataFOT();
			

			MoveWindow(0,48,605,488);

			EnterCriticalSection(&csGraphButton);
			if(m_pcGraph1)
				m_pcGraph1->ShowWindow(SW_HIDE);
			LeaveCriticalSection(&csGraphButton);
			
			setTextGraph2(_T("PLETH"));
			drawGraphTXT(true, true, false,true);
			
			m_iCurrentLineDiagramm=IDC_LINEDIAGRAM_PRESSURE;
			getModel()->getCONFIG()->GraphSetPrimaryLineDiagramm(m_iCurrentLineDiagramm);

			
			CreateWndGraphPressure(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
			ShowWndGraphPressure(true);

			if(!m_bFreeze)//rku DIAG
			{
				setRedrawDiagram(REDRAW_GRAPHS);
			}

			CreateWndGraphSPO2(40,169,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
			ShowWndGraphSPO2(true);

			CreateWndDataSPO2(40,331,DIAGRAMM_WIDTH,DIAGRAMM_DATAHEIGHT);
			ShowWndDataSPO2(true);
		}
		else
		{
			DestroyWndGraphPressure();
			DestroyWndGraphVolume();
			DestroyWndGraphFlow();
			if(!bNextGraph)
			{
				DestroyWndLoopPressureVolume();
				DestroyWndLoopVolumeFlow();
				DestroyWndGraphCO2();
				DestroyWndDataCO2();
				DestroyWndGraphSPO2();
				DestroyWndDataSPO2();
				DestroyWndGraphFOT();
				DestroyWndDataFOT();
			}
			
			
			MoveWindow(0,48,605,488);

			EnterCriticalSection(&csGraphButton);
			if(m_pcGraph1)
				m_pcGraph1->ShowWindow(SW_SHOW);
			LeaveCriticalSection(&csGraphButton);
			
			setTextGraph2(_T("PLETH"));
			
			if(!bNextGraph)
				drawGraphTXT(false, true, false,true);
		

			if(m_iCurrentLineDiagramm==IDC_LINEDIAGRAM_VOLUME)
			{
				EnterCriticalSection(&csGraphButton);
				if(m_pcGraph1)
					m_pcGraph1->RefreshText(getModel()->GetLanguageString(IDS_BTN_SELVOLUMEGRAPH));
				LeaveCriticalSection(&csGraphButton);
				
				CreateWndGraphVolume(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
				ShowWndGraphVolume(true);
				if(!m_bFreeze)
				{
					setRedrawDiagram(REDRAW_GRAPHS);
				}
			}
			else if(m_iCurrentLineDiagramm==IDC_LINEDIAGRAM_FLOW)
			{
				EnterCriticalSection(&csGraphButton);
				if(m_pcGraph1)
					m_pcGraph1->RefreshText(getModel()->GetLanguageString(IDS_BTN_SELFLOWGRAPH));
				LeaveCriticalSection(&csGraphButton);
				
				CreateWndGraphFlow(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
				ShowWndGraphFlow(true);
				if(!m_bFreeze)
				{
					setRedrawDiagram(REDRAW_GRAPHS);
				}
			}
			else
			{
				EnterCriticalSection(&csGraphButton);
				if(m_pcGraph1)
					m_pcGraph1->RefreshText(getModel()->GetLanguageString(IDS_BTN_SELPRESSUREGRAPH));
				LeaveCriticalSection(&csGraphButton);
				
				CreateWndGraphPressure(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
				ShowWndGraphPressure(true);
				if(!m_bFreeze)
				{
					setRedrawDiagram(REDRAW_GRAPHS);
				}
			}

			if(!bNextGraph)
			{
				CreateWndGraphSPO2(40,169,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
				ShowWndGraphSPO2(true);

				CreateWndDataSPO2(40,331,DIAGRAMM_WIDTH,DIAGRAMM_DATAHEIGHT);
				ShowWndDataSPO2(true);
			}
			
		}
	}
	else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
	{
		//theApp.getLog()->Write(_T("#19"));
		if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF
			||	getModel()->getVMODEHANDLER()->activeModeIsDUOPAP()
			||	(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getVMODEHANDLER()->activeModeIsNCPAP()) )
		{
			DestroyWndGraphPressure();
			DestroyWndGraphVolume();
			DestroyWndGraphFlow();
			DestroyWndLoopPressureVolume();
			DestroyWndLoopVolumeFlow();
			DestroyWndGraphCO2();
			DestroyWndDataCO2();
			DestroyWndGraphSPO2();
			DestroyWndDataSPO2();
			DestroyWndGraphFOT();
			DestroyWndDataFOT();
			
			MoveWindow(0,48,605,488);

			EnterCriticalSection(&csGraphButton);
			if(m_pcGraph1)
				m_pcGraph1->ShowWindow(SW_HIDE);
			LeaveCriticalSection(&csGraphButton);
			
			setTextGraph2(getModel()->GetLanguageString(IDS_MENU_CO2));
			drawGraphTXT(true, true, false);

			m_iCurrentLineDiagramm=IDC_LINEDIAGRAM_PRESSURE;
			getModel()->getCONFIG()->GraphSetPrimaryLineDiagramm(m_iCurrentLineDiagramm);

			
			CreateWndGraphPressure(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
			ShowWndGraphPressure(true);
			if(!m_bFreeze)
			{
				setRedrawDiagram(REDRAW_GRAPHS);
			}

			CreateWndGraphCO2(40,169,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
			ShowWndGraphCO2(true);

			CreateWndDataCO2(40,331,DIAGRAMM_WIDTH,DIAGRAMM_DATAHEIGHT);
			ShowWndDataCO2(true);
		}
		else
		{
			DestroyWndGraphPressure();
			DestroyWndGraphVolume();
			DestroyWndGraphFlow();
			if(!bNextGraph)
			{
				DestroyWndLoopPressureVolume();
				DestroyWndLoopVolumeFlow();
				DestroyWndGraphCO2();
				DestroyWndDataCO2();
				DestroyWndGraphSPO2();
				DestroyWndDataSPO2();
				DestroyWndGraphFOT();
				DestroyWndDataFOT();
			}
		
			MoveWindow(0,48,605,488);

			EnterCriticalSection(&csGraphButton);
			if(m_pcGraph1)
				m_pcGraph1->ShowWindow(SW_SHOW);
			LeaveCriticalSection(&csGraphButton);
			
			setTextGraph2(getModel()->GetLanguageString(IDS_MENU_CO2));
			drawGraphTXT(false, true, false);
			
			if(m_iCurrentLineDiagramm==IDC_LINEDIAGRAM_VOLUME)
			{
				EnterCriticalSection(&csGraphButton);
				if(m_pcGraph1)
					m_pcGraph1->RefreshText(getModel()->GetLanguageString(IDS_BTN_SELVOLUMEGRAPH));
				LeaveCriticalSection(&csGraphButton);
				
				CreateWndGraphVolume(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
				ShowWndGraphVolume(true);
				if(!m_bFreeze)
				{
					setRedrawDiagram(REDRAW_GRAPHS);
				}
			}
			else if(m_iCurrentLineDiagramm==IDC_LINEDIAGRAM_FLOW)
			{
				EnterCriticalSection(&csGraphButton);
				if(m_pcGraph1)
					m_pcGraph1->RefreshText(getModel()->GetLanguageString(IDS_BTN_SELFLOWGRAPH));
				LeaveCriticalSection(&csGraphButton);
				
				CreateWndGraphFlow(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
				ShowWndGraphFlow(true);
				if(!m_bFreeze)
				{
					setRedrawDiagram(REDRAW_GRAPHS);
				}
			}
			else
			{
				EnterCriticalSection(&csGraphButton);
				if(m_pcGraph1)
					m_pcGraph1->RefreshText(getModel()->GetLanguageString(IDS_BTN_SELPRESSUREGRAPH));
				LeaveCriticalSection(&csGraphButton);
				
				CreateWndGraphPressure(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
				ShowWndGraphPressure(true);
				if(!m_bFreeze)
				{
					setRedrawDiagram(REDRAW_GRAPHS);
				}
			}

			if(!bNextGraph)
			{
				CreateWndGraphCO2(40,169,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
				ShowWndGraphCO2(true);

				CreateWndDataCO2(40,331,DIAGRAMM_WIDTH,DIAGRAMM_DATAHEIGHT);
				ShowWndDataCO2(true);
			}
			
		}
	}
	else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
	{
		if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF
			||	getModel()->getVMODEHANDLER()->activeModeIsNMODETrigger())//rku,PRETRIGGER
		{
			PostMessage(WM_GRAPH_CHANGESUBSTATE);
		}
		else
		{
			DestroyWndGraphPressure();
			DestroyWndGraphVolume();
			DestroyWndGraphFlow();
			if(!bNextGraph)
			{
				DestroyWndLoopPressureVolume();
				DestroyWndLoopVolumeFlow();
				DestroyWndGraphCO2();
				DestroyWndDataCO2();
				DestroyWndGraphSPO2();
				DestroyWndDataSPO2();
				DestroyWndGraphFOT();
				DestroyWndDataFOT();
			}
			

			MoveWindow(0,48,605,488);

			EnterCriticalSection(&csGraphButton);
			if(m_pcGraph1)
				m_pcGraph1->ShowWindow(SW_SHOW);
			LeaveCriticalSection(&csGraphButton);
			
			drawGraphTXT(false, false, false);
			

			if(m_iCurrentLineDiagramm==IDC_LINEDIAGRAM_VOLUME)
			{
				EnterCriticalSection(&csGraphButton);
				if(m_pcGraph1)
					m_pcGraph1->RefreshText(getModel()->GetLanguageString(IDS_BTN_SELVOLUMEGRAPH));
				LeaveCriticalSection(&csGraphButton);
				
				CreateWndGraphVolume(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
				ShowWndGraphVolume(true);
				if(!m_bFreeze)
				{
					setRedrawDiagram(REDRAW_GRAPHS);
				}
			}
			else if(m_iCurrentLineDiagramm==IDC_LINEDIAGRAM_FLOW)
			{
				EnterCriticalSection(&csGraphButton);
				if(m_pcGraph1)
					m_pcGraph1->RefreshText(getModel()->GetLanguageString(IDS_BTN_SELFLOWGRAPH));
				LeaveCriticalSection(&csGraphButton);
				
				CreateWndGraphFlow(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
				ShowWndGraphFlow(true);
				if(!m_bFreeze)
				{
					setRedrawDiagram(REDRAW_GRAPHS);
				}
			}
			else
			{
				EnterCriticalSection(&csGraphButton);
				if(m_pcGraph1)
					m_pcGraph1->RefreshText(getModel()->GetLanguageString(IDS_BTN_SELPRESSUREGRAPH));
				LeaveCriticalSection(&csGraphButton);
				
				CreateWndGraphPressure(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
				ShowWndGraphPressure(true);
				if(!m_bFreeze)
				{
					setRedrawDiagram(REDRAW_GRAPHS);
				}
			}
			
			if(!bNextGraph)
			{
				CreateWndLoopPressureVolume(10,173,LOOP_WIDTH,LOOP_HEIGHT);
				CreateWndLoopVolumeFlow(315,173,LOOP_WIDTH,LOOP_HEIGHT);
				ShowWndLoopPressureVolume(true);
				ShowWndLoopVolumeFlow(true);
			}
		}
	}
	else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SINGLE_LINEGRAPHS
		||	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_NONE)
	{
		if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF
			||	getModel()->getVMODEHANDLER()->activeModeIsDUOPAP()
			||	(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getVMODEHANDLER()->activeModeIsNCPAP()) )
		{
			DestroyWndGraphPressure();
			DestroyWndGraphVolume();
			DestroyWndGraphFlow();
			DestroyWndLoopPressureVolume();
			DestroyWndLoopVolumeFlow();
			DestroyWndGraphCO2();
			DestroyWndDataCO2();
			DestroyWndGraphSPO2();
			DestroyWndDataSPO2();
			DestroyWndGraphFOT();
			DestroyWndDataFOT();

			MoveWindow(0,48,605,164);

			EnterCriticalSection(&csGraphButton);
			if(m_pcGraph1)
				m_pcGraph1->ShowWindow(SW_HIDE);
			LeaveCriticalSection(&csGraphButton);
			
			drawGraphTXT(true, false, false);
		
			m_iCurrentLineDiagramm=IDC_LINEDIAGRAM_PRESSURE;
			getModel()->getCONFIG()->GraphSetPrimaryLineDiagramm(m_iCurrentLineDiagramm);

			CreateWndGraphPressure(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
			ShowWndGraphPressure(true);
			if(!m_bFreeze)
			{
				setRedrawDiagram(REDRAW_GRAPHS);
			}
		}
		else
		{
			DestroyWndGraphPressure();
			DestroyWndGraphVolume();
			DestroyWndGraphFlow();
			if(!bNextGraph)
			{
				DestroyWndLoopPressureVolume();
				DestroyWndLoopVolumeFlow();
				DestroyWndGraphCO2();
				DestroyWndDataCO2();
				DestroyWndGraphSPO2();
				DestroyWndDataSPO2();
				DestroyWndGraphFOT();
				DestroyWndDataFOT();
			}

			MoveWindow(0,48,605,164);

			EnterCriticalSection(&csGraphButton);
			if(m_pcGraph1)
				m_pcGraph1->ShowWindow(SW_SHOW);
			LeaveCriticalSection(&csGraphButton);
			
			drawGraphTXT(false, false, false);

			if(m_iCurrentLineDiagramm==IDC_LINEDIAGRAM_VOLUME)
			{
				EnterCriticalSection(&csGraphButton);
				if(m_pcGraph1)
					m_pcGraph1->RefreshText(getModel()->GetLanguageString(IDS_BTN_SELVOLUMEGRAPH));
				LeaveCriticalSection(&csGraphButton);
				
				CreateWndGraphVolume(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
				ShowWndGraphVolume(true);
				if(!m_bFreeze)
				{
					setRedrawDiagram(REDRAW_GRAPHS);
				}
			}
			else if(m_iCurrentLineDiagramm==IDC_LINEDIAGRAM_FLOW)
			{
				EnterCriticalSection(&csGraphButton);
				if(m_pcGraph1)
					m_pcGraph1->RefreshText(getModel()->GetLanguageString(IDS_BTN_SELFLOWGRAPH));
				LeaveCriticalSection(&csGraphButton);
				
				CreateWndGraphFlow(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
				ShowWndGraphFlow(true);
				if(!m_bFreeze)
				{
					setRedrawDiagram(REDRAW_GRAPHS);
				}
			}
			else
			{
				EnterCriticalSection(&csGraphButton);
				if(m_pcGraph1)
					m_pcGraph1->RefreshText(getModel()->GetLanguageString(IDS_BTN_SELPRESSUREGRAPH));
				LeaveCriticalSection(&csGraphButton);
				
				CreateWndGraphPressure(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
				ShowWndGraphPressure(true);
				if(!m_bFreeze)
				{
					setRedrawDiagram(REDRAW_GRAPHS);
				}
			}
		}
	}
	else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_MULTI_LINEGRAPHS)
	{
		if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
			||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF
			||	getModel()->getVMODEHANDLER()->activeModeIsDUOPAP()
			||	(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getVMODEHANDLER()->activeModeIsNCPAP()) )
		{
			DestroyWndGraphPressure();
			DestroyWndGraphVolume();
			DestroyWndGraphFlow();
			DestroyWndLoopPressureVolume();
			DestroyWndLoopVolumeFlow();
			DestroyWndGraphCO2();
			DestroyWndDataCO2();
			DestroyWndGraphSPO2();
			DestroyWndDataSPO2();
			DestroyWndGraphFOT();
			DestroyWndDataFOT();

			MoveWindow(0,48,605,488);

			EnterCriticalSection(&csGraphButton);
			if(m_pcGraph1)
				m_pcGraph1->ShowWindow(SW_HIDE);
			LeaveCriticalSection(&csGraphButton);
			
			drawGraphTXT(true, false, false);
			
			CreateWndGraphPressure(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
			ShowWndGraphPressure(true);
			if(!m_bFreeze)
			{
				setRedrawDiagram(REDRAW_GRAPHS);
			}
		}
		else
		{
			DestroyWndGraphPressure();
			DestroyWndGraphVolume();
			DestroyWndGraphFlow();
			DestroyWndLoopPressureVolume();
			DestroyWndLoopVolumeFlow();
			DestroyWndGraphCO2();
			DestroyWndDataCO2();
			DestroyWndGraphSPO2();
			DestroyWndDataSPO2();
			DestroyWndGraphFOT();
			DestroyWndDataFOT();

			MoveWindow(0,48,605,488);	

			EnterCriticalSection(&csGraphButton);
			if(m_pcGraph1)
				m_pcGraph1->ShowWindow(SW_HIDE);
			LeaveCriticalSection(&csGraphButton);

			setTextGraph2(getModel()->GetLanguageString(IDS_BTN_SELFLOWGRAPH));
			drawGraphTXT(true, true, true);
			
			CreateWndGraphPressure(40,7,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
			CreateWndGraphFlow(40,169,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
			CreateWndGraphVolume(40,331,DIAGRAMM_WIDTH,DIAGRAMM_HEIGHT);
			
			ShowWndGraphPressure(true);
			ShowWndGraphFlow(true);
			ShowWndGraphVolume(true);

			if(!m_bFreeze)
			{
				setRedrawDiagram(REDRAW_GRAPHS);
			}
		}
	}

	if(m_bFreeze)
	{
		if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
		{
			setRedrawDiagram(REDRAW_ALL);
		}
		else
		{
			setRedrawDiagram(REDRAW_GRAPHS);
		}
	}

	UpdateWindow();
}


void CViewDiagramm::Hide()
{
	ShowWndMenuGraphs(false);

	this->ShowWindow(SW_HIDE);
}

void CViewDiagramm::resetSPO2Diagramm()
{
	EnterCriticalSection(&csDiagrammSPO2);
	if(m_pDiagrammSPO2 && m_bGraphSPO2IsActive)
	{
		m_pDiagrammSPO2->SetFreeze(false);
		m_pDiagrammSPO2->ClearFunction(true);

	}
	LeaveCriticalSection(&csDiagrammSPO2);
}
void CViewDiagramm::resetCO2Diagramm()
{
	EnterCriticalSection(&csDiagrammCO2);
	if(m_pDiagrammCO2 && m_bGraphCO2IsActive)
	{
		m_pDiagrammCO2->SetFreeze(false);
		//m_pDiagrammCO2->PaintGraph();
		m_pDiagrammCO2->ClearFunction(true);

	}
	LeaveCriticalSection(&csDiagrammCO2);
}
void CViewDiagramm::resetPressureDiagramm()
{
	EnterCriticalSection(&csDiagrammPRESSURE);
	if(m_pDiagrammPressure && m_bGraphPressureIsActive)
	{
		m_pDiagrammPressure->SetFreeze(false);
		m_pDiagrammPressure->ClearFunction(true);
	}
	LeaveCriticalSection(&csDiagrammPRESSURE);
}
void CViewDiagramm::resetFlowDiagramm()
{
	EnterCriticalSection(&csDiagrammFLOW);
	if(m_pDiagrammFlow && m_bGraphFlowIsActive)
	{
		m_pDiagrammFlow->SetFreeze(false);
		m_pDiagrammFlow->ClearFunction(true);

	}
	LeaveCriticalSection(&csDiagrammFLOW);
}

void CViewDiagramm::resetVolumeDiagramm()
{
	EnterCriticalSection(&csDiagrammVOLUME);
	if(m_pDiagrammVolume && m_bGraphVolumeIsActive)
	{
		m_pDiagrammVolume->SetFreeze(false);
		m_pDiagrammVolume->ClearFunction(true);
	}
	LeaveCriticalSection(&csDiagrammVOLUME);
}


void CViewDiagramm::resetAllDiagramms()
{
	EnterCriticalSection(&csDiagrammSPO2);
	if(m_pDiagrammSPO2 && m_bGraphSPO2IsActive)
	{
		m_pDiagrammSPO2->SetFreeze(false);
		m_pDiagrammSPO2->ClearFunction(true);
	}
	LeaveCriticalSection(&csDiagrammSPO2);

	EnterCriticalSection(&csDiagrammCO2);
	if(m_pDiagrammCO2 && m_bGraphCO2IsActive)
	{
		m_pDiagrammCO2->SetFreeze(false);
		m_pDiagrammCO2->ClearFunction(true);
	}
	LeaveCriticalSection(&csDiagrammCO2);
	
	EnterCriticalSection(&csDiagrammPRESSURE);
	if(m_pDiagrammPressure && m_bGraphPressureIsActive)
	{
		m_pDiagrammPressure->SetFreeze(false);
		m_pDiagrammPressure->ClearFunction(true);
	}
	LeaveCriticalSection(&csDiagrammPRESSURE);

	EnterCriticalSection(&csDiagrammVOLUME);
	if(m_pDiagrammVolume && m_bGraphVolumeIsActive)
	{
		m_pDiagrammVolume->SetFreeze(false);
		m_pDiagrammVolume->ClearFunction(true);
	}
	LeaveCriticalSection(&csDiagrammVOLUME);

	EnterCriticalSection(&csDiagrammFLOW);
	if(m_pDiagrammFlow && m_bGraphFlowIsActive)
	{
		m_pDiagrammFlow->SetFreeze(false);
		m_pDiagrammFlow->ClearFunction(true);
	}
	LeaveCriticalSection(&csDiagrammFLOW);

	EnterCriticalSection(&csDiagrammLOOP);
	if(m_pLoopPressureVolume && m_bLoopPressureVolumeIsActive)
	{
		m_pLoopPressureVolume->SetFreeze(false);
		m_pLoopPressureVolume->ClearFunction(true);
	}
	LeaveCriticalSection(&csDiagrammLOOP);

	EnterCriticalSection(&csDiagrammLOOP);
	if(m_pLoopVolumeFlow && m_bLoopVolumeFlowIsActive)
	{
		m_pLoopVolumeFlow->SetFreeze(false);
		m_pLoopVolumeFlow->ClearFunction(true);
	}
	LeaveCriticalSection(&csDiagrammLOOP);
}



// **************************************************************************
// 
// **************************************************************************
bool CViewDiagramm::SetNextFocus()
{
	if(getModel()->getVIEWHANDLER()->getViewState()==VS_GRAPH)
	{
		if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
		{
			bool bFOTfocus=false;
			CWnd* pFocWnd=CWnd::GetFocus();
			if(pFocWnd!=0)
			{
				int iID=pFocWnd->GetDlgCtrlID();

				if(iID==IDC_BTN_PARA_FOT_STEPS
					||	iID==IDC_BTN_PARA_FOT_PMEANLOW
					/*||	iID==IDC_BTN_PARA_FOTAMP
					||	iID==IDC_BTN_PARA_FOTFREQ*/
					||	iID==IDC_BTN_PARA_FOT_PMEANHIGH
					||	iID==IDC_BTN_PARA_FOT_PEEPLOW
					||	iID==IDC_BTN_PARA_FOT_PEEPHIGH)
				{
					bFOTfocus=true;
				}
			}
			if(bFOTfocus)
			{
				//rku check1
				if(m_pDataFOT)
					m_pDataFOT->SetNextFocus();
			}
			else
			{
				switch(m_iCurFocusedWnd)
				{
				case IDC_BTN_SELGRAPH_1:
					{
						DrawCursor(IDC_LINEDIAGRAM);
						return true;
					}
					break;
				case IDC_LINEDIAGRAM:
					{
						DrawCursor(IDC_LOOPDIAGRAM_PRESSUREVOLUME);
						return true;
					}
					break;
				case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
					{
						DrawCursor(IDC_LOOPDIAGRAM_VOLUMEFLOW);
						return true;
					}
					break;
				case IDC_LOOPDIAGRAM_VOLUMEFLOW:
					{
						DrawCursor(IDC_BTN_SELGRAPH_1);
						return true;
					}
					break;
				default:
					break;
				}
			}
		}
		else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
		{
			switch(m_iCurFocusedWnd)
			{
			case IDC_BTN_SELGRAPH_1:
				{
					DrawCursor(IDC_LINEDIAGRAM);
					return true;
				}
				break;
			case IDC_LINEDIAGRAM:
				{
					DrawCursor(IDC_LINEDIAGRAM_SPO2);
					return true;
				}
				break;
			case IDC_LINEDIAGRAM_SPO2:
				{
					DrawCursor(IDC_BTN_SELGRAPH_1);
					return true;
				}
				break;
			default:
				break;
			}
		}
		else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
		{
			switch(m_iCurFocusedWnd)
			{
			case IDC_BTN_SELGRAPH_1:
				{
					DrawCursor(IDC_LINEDIAGRAM);
					return true;
				}
				break;
			case IDC_LINEDIAGRAM:
				{
					DrawCursor(IDC_LINEDIAGRAM_CO2);
					return true;
				}
				break;
			case IDC_LINEDIAGRAM_CO2:
				{
					DrawCursor(IDC_BTN_SELGRAPH_1);
					return true;
				}
				break;
			default:
				break;
			}
		}
		else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
		{
			switch(m_iCurFocusedWnd)
			{
			case IDC_BTN_SELGRAPH_1:
				{
					DrawCursor(IDC_LINEDIAGRAM);
					return true;
				}
				break;
			case IDC_LINEDIAGRAM:
				{
					DrawCursor(IDC_LOOPDIAGRAM_PRESSUREVOLUME);
					return true;
				}
				break;
			case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
				{
					DrawCursor(IDC_LOOPDIAGRAM_VOLUMEFLOW);
					return true;
				}
				break;
				case IDC_LOOPDIAGRAM_VOLUMEFLOW:
				{
					DrawCursor(IDC_BTN_SELGRAPH_1);
					return true;
				}
				break;
			default:
				break;
			}
		}//rku check1
		/*else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SINGLE_LINEGRAPHS
			||	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_NONE)
		{
		}*/
		else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_MULTI_LINEGRAPHS)
		{
			switch(m_iCurFocusedWnd)
			{
			case IDC_LINEDIAGRAM_PRESSURE:
				{
					DrawCursor(IDC_LINEDIAGRAM_FLOW);
				}
				break;
			case IDC_LINEDIAGRAM_FLOW:
				{
					DrawCursor(IDC_LINEDIAGRAM_VOLUME);
				}
				break;
			case IDC_LINEDIAGRAM_VOLUME:
				{
					DrawCursor(IDC_LINEDIAGRAM_PRESSURE);
				}
				break;
			default:
				{

				}
				break;
			}
		}
	}
	
	return false;
}
// **************************************************************************
// 
// **************************************************************************
bool CViewDiagramm::SetPrevFocus()
{
	if(getModel()->getVIEWHANDLER()->getViewState()==VS_GRAPH)
	{
		if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
		{
			bool bFOTfocus=false;
			CWnd* pFocWnd=CWnd::GetFocus();
			if(pFocWnd!=0)
			{
				int iID=pFocWnd->GetDlgCtrlID();

				if(iID==IDC_BTN_PARA_FOT_STEPS
					||	iID==IDC_BTN_PARA_FOT_PMEANLOW
					/*||	iID==IDC_BTN_PARA_FOTAMP
					||	iID==IDC_BTN_PARA_FOTFREQ*/
					||	iID==IDC_BTN_PARA_FOT_PMEANHIGH
					||	iID==IDC_BTN_PARA_FOT_PEEPLOW
					||	iID==IDC_BTN_PARA_FOT_PEEPHIGH)
				{
					bFOTfocus=true;
				}
			}
			if(bFOTfocus)
			{
				//rku check1
				if(m_pDataFOT)
					m_pDataFOT->SetPrevFocus();
			}
			else
			{
				switch(m_iCurFocusedWnd)
				{
				case IDC_BTN_SELGRAPH_1:
					{
						DrawCursor(IDC_LOOPDIAGRAM_VOLUMEFLOW);
						return true;
					}
					break;
				case IDC_LINEDIAGRAM:
					{
						DrawCursor(IDC_BTN_SELGRAPH_1);
						return true;
					}
					break;
				case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
					{
						DrawCursor(IDC_LINEDIAGRAM);
						return true;
					}
					break;
				case IDC_LOOPDIAGRAM_VOLUMEFLOW:
					{
						DrawCursor(IDC_LOOPDIAGRAM_PRESSUREVOLUME);
						return true;
					}
					break;
				default:
					break;
				}
			}
			
		}
		else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
		{
			switch(m_iCurFocusedWnd)
			{
			case IDC_BTN_SELGRAPH_1:
				{
					DrawCursor(IDC_LINEDIAGRAM_SPO2);
					return true;
				}
				break;
			case IDC_LINEDIAGRAM:
				{
					DrawCursor(IDC_BTN_SELGRAPH_1);
					return true;
				}
				break;
			case IDC_LINEDIAGRAM_SPO2:
				{
					DrawCursor(IDC_LINEDIAGRAM);
					return true;
				}
				break;
			default:
				break;
			}
		}
		else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
		{
			switch(m_iCurFocusedWnd)
			{
			case IDC_BTN_SELGRAPH_1:
				{
					DrawCursor(IDC_LINEDIAGRAM_CO2);
					return true;
				}
				break;
			case IDC_LINEDIAGRAM:
				{
					DrawCursor(IDC_BTN_SELGRAPH_1);
					return true;
				}
				break;
			case IDC_LINEDIAGRAM_CO2:
				{
					DrawCursor(IDC_LINEDIAGRAM);
					return true;
				}
				break;
			default:
				break;
			}
		}
		else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
		{
			switch(m_iCurFocusedWnd)
			{
			case IDC_BTN_SELGRAPH_1:
				{
					DrawCursor(IDC_LOOPDIAGRAM_VOLUMEFLOW);
					return true;
				}
				break;
			case IDC_LINEDIAGRAM:
				{
					DrawCursor(IDC_BTN_SELGRAPH_1);
					return true;
				}
				break;
			case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
				{
					DrawCursor(IDC_LINEDIAGRAM);
					return true;
				}
				break;
			case IDC_LOOPDIAGRAM_VOLUMEFLOW:
				{
					DrawCursor(IDC_LOOPDIAGRAM_PRESSUREVOLUME);
					return true;
				}
				break;
			default:
				break;
			}
		}//rku check1
		/*else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SINGLE_LINEGRAPHS
			||	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_NONE)
		{
		}*/
		else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_MULTI_LINEGRAPHS)
		{
			switch(m_iCurFocusedWnd)
			{
			case IDC_LINEDIAGRAM_PRESSURE:
				{
					DrawCursor(IDC_LINEDIAGRAM_VOLUME);
				}
				break;
			case IDC_LINEDIAGRAM_FLOW:
				{
					DrawCursor(IDC_LINEDIAGRAM_PRESSURE);
				}
				break;
			case IDC_LINEDIAGRAM_VOLUME:
				{
					DrawCursor(IDC_LINEDIAGRAM_FLOW);
				}
				break;
			default:
				break;
			}
		}
	}
	else if(getModel()->getVIEWHANDLER()->getViewState()==VS_PARA)
	{
		switch(m_iCurFocusedWnd)
		{
		case 1:
			{
				return true;
			}
			break;
		default:
			{

			}
			break;
		}

	}
	return false;
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::SetViewFocus()
{
	if(getModel()->getVIEWHANDLER()->getViewState()==VS_GRAPH)
	{
		if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
		{
			switch(getModel()->getCONFIG()->getLastSelectedFOTViewBtns())
			{
			case IDC_BTN_PARA_FOT_STEPS:
				{
					if(m_pDataFOT)
						m_pDataFOT->SetViewFocus(IDC_BTN_PARA_FOT_STEPS);
				}
				break;
			case IDC_BTN_PARA_FOT_PMEANLOW:
				{
					if(m_pDataFOT)
						m_pDataFOT->SetViewFocus(IDC_BTN_PARA_FOT_PMEANLOW);
				}
				break;
			/*case IDC_BTN_PARA_FOTAMP:
				{
					if(m_pDataFOT)
						m_pDataFOT->SetViewFocus(IDC_BTN_PARA_FOTAMP);
				}
				break;
			case IDC_BTN_PARA_FOTFREQ:
				{
					if(m_pDataFOT)
						m_pDataFOT->SetViewFocus(IDC_BTN_PARA_FOTFREQ);
				}
				break;*/
			case IDC_BTN_PARA_FOT_PMEANHIGH:
				{
					if(m_pDataFOT)
						m_pDataFOT->SetViewFocus(IDC_BTN_PARA_FOT_PMEANHIGH);
				}
				break;
			case IDC_BTN_PARA_FOT_PEEPLOW:
				{
					if(m_pDataFOT)
						m_pDataFOT->SetViewFocus(IDC_BTN_PARA_FOT_PEEPLOW);
				}
				break;
			case IDC_BTN_PARA_FOT_PEEPHIGH:
				{
					if(m_pDataFOT)
						m_pDataFOT->SetViewFocus(IDC_BTN_PARA_FOT_PEEPHIGH);
				}
				break;
			default:
				{
					if(m_pDataFOT)
						m_pDataFOT->SetViewFocus(IDC_BTN_PARA_FOT_STEPS);
				}
				break;
			}
			return;
		}
		/*else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
		{
			PostMessage(WM_SELECT_NEXTGRAPH);
			DrawCursor(IDC_BTN_SELGRAPH_1);
		}
		else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
		{
			PostMessage(WM_SELECT_NEXTGRAPH);
			DrawCursor(IDC_BTN_SELGRAPH_1);
		}
		else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
		{
			PostMessage(WM_SELECT_NEXTGRAPH);
			DrawCursor(IDC_BTN_SELGRAPH_1);
		}
		else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SINGLE_LINEGRAPHS
			||	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_NONE)
		{
		}
		else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_MULTI_LINEGRAPHS)
		{
			this->SetFocus();
			DrawCursor(0);
		}*/
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->SetFocus();
		}
	}
	else if(getModel()->getVIEWHANDLER()->getViewState()==VS_PARA)
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->SetFocus();
	}
	else
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->SetFocus();
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::OnBnClickedNextGraph()
{
	if(m_iCurrentLineDiagramm==IDC_LINEDIAGRAM_PRESSURE)
	{
		m_iCurrentLineDiagramm=IDC_LINEDIAGRAM_VOLUME;
	}
	else if(m_iCurrentLineDiagramm==IDC_LINEDIAGRAM_VOLUME)
	{
		m_iCurrentLineDiagramm=IDC_LINEDIAGRAM_FLOW;
	}
	else
	{
		m_iCurrentLineDiagramm=IDC_LINEDIAGRAM_PRESSURE;
	}

	getModel()->getCONFIG()->GraphSetPrimaryLineDiagramm(m_iCurrentLineDiagramm);
	Show(true);

	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();
}

//************************************
// Method:    OnDestroy
// FullName:  CViewDiagramm::OnDestroy
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/19: checked for correct closing of window
//************************************
void CViewDiagramm::OnDestroy()
{
	setExit();

	KillTimer(GRAPHTIMER);
	StopGraphThread();

	getModel()->getDATAHANDLER()->SetGraphFreezed(false);

	DestroyWndGraphPressure();
	DestroyWndGraphVolume();
	DestroyWndGraphFlow();
	DestroyWndLoopPressureVolume();
	DestroyWndLoopVolumeFlow();
	DestroyWndGraphCO2();
	DestroyWndDataCO2();
	DestroyWndGraphSPO2();
	DestroyWndDataSPO2();
	DestroyWndGraphFOT();
	DestroyWndDataFOT();

	DestroyWndMenuGraphs();

	if(m_pcwtGraphThread!=NULL)
	{
		delete m_pcwtGraphThread;
		m_pcwtGraphThread=NULL;

		if(m_hThreadGraph!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThreadGraph);
			m_hThreadGraph=INVALID_HANDLE_VALUE;
		}
	}
	CMVView::OnDestroy();
}


// **************************************************************************
// 
// **************************************************************************
BOOL CViewDiagramm::OnEraseBkgnd(CDC* pDC)
{
	CBrush cbrBack(BACKGND);

	CRect rc;
	GetClientRect(rc);
	pDC->FillRect(&rc, &cbrBack);

	return TRUE;
}

// **************************************************************************
// 
// **************************************************************************
BOOL CViewDiagramm::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_LBUTTONDOWN:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_CURRENT_VIEWID,IDC_VIEW_GRAPH,0);
		}
		break;
	case WM_KEYDOWN:
		{
			if(pMsg->wParam==VK_SPACE)
			{
				PostMessage(WM_SET_PARATIMER);
			}
			else if(pMsg->wParam==VK_UP)
			{
				bool bFOT=false;
				bool bSPO2=false;
				CWnd* pFocWnd=CWnd::GetFocus();
				if(pFocWnd!=0)
				{
					int iID=pFocWnd->GetDlgCtrlID();

					if(		iID==IDC_BTN_LIMIT_SPO2LOW
						||	iID==IDC_BTN_LIMIT_SPO2HIGH
						||	iID==IDC_BTN_LIMIT_FIO2LOW
						||	iID==IDC_BTN_LIMIT_FIO2HIGH)
					{
						bSPO2=true;
						PostMessage(WM_SET_PARATIMER);
					}
					else if(iID==IDC_BTN_PARA_FOT_STEPS
						||	iID==IDC_BTN_PARA_FOT_PMEANLOW
						/*||	iID==IDC_BTN_PARA_FOTAMP
						||	iID==IDC_BTN_PARA_FOTFREQ*/
						||	iID==IDC_BTN_PARA_FOT_PMEANHIGH
						||	iID==IDC_BTN_PARA_FOT_PEEPLOW
						||	iID==IDC_BTN_PARA_FOT_PEEPHIGH)
					{
						bFOT=true;
						PostMessage(WM_SET_PARATIMER);
					}
				}

				if(!bSPO2 && !bFOT)
				{
					IncreaseGraphSpeed();
					return 1;
				}
			}
			else if(pMsg->wParam==VK_DOWN)
			{
				bool bFOT=false;
				bool bSPO2=false;
				CWnd* pFocWnd=CWnd::GetFocus();
				if(pFocWnd!=0)
				{
					int iID=pFocWnd->GetDlgCtrlID();

					if(		iID==IDC_BTN_LIMIT_SPO2LOW
						||	iID==IDC_BTN_LIMIT_SPO2HIGH
						||	iID==IDC_BTN_LIMIT_FIO2LOW
						||	iID==IDC_BTN_LIMIT_FIO2HIGH)
					{
						bSPO2=true;
						PostMessage(WM_SET_PARATIMER);
					}
					else if(iID==IDC_BTN_PARA_FOT_STEPS
						||	iID==IDC_BTN_PARA_FOT_PMEANLOW
						/*||	iID==IDC_BTN_PARA_FOTAMP
						||	iID==IDC_BTN_PARA_FOTFREQ*/
						||	iID==IDC_BTN_PARA_FOT_PMEANHIGH
						||	iID==IDC_BTN_PARA_FOT_PEEPLOW
						||	iID==IDC_BTN_PARA_FOT_PEEPHIGH)
					{
						bFOT=true;
						PostMessage(WM_SET_PARATIMER);
					}
				}

				if(!bSPO2 && !bFOT)
				{
					DecreaseGraphSpeed();
					return 1;
				}
			}
		}
		break;
	default:
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}

// **************************************************************************
// 
// **************************************************************************
LRESULT CViewDiagramm::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_FOT_UPDATE_PMEAN:
		{
			updateFOTPmeanPara();
		}
		break;
	case WM_FOT_UPDATE_PEEP:
		{
			updateFOTPEEPPara();
		}
		break;
	case WM_FOT_STARTED:
		{
			setFOTrunning(true);
		}
		break;
	case WM_FOT_STOPPED:
		{
			setFOTrunning(false);
		}
		break;
	case WM_DRAW_FOT_TIME:
		{
			setFOTtime((BYTE)wParam);
		}
		break;
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
	case WM_REDRAW_DIAGRAMMDATA:
		{
			RedrawDiagrammData(wParam);
			return 1;
		}
		break;
	case WM_UPDATECO2INFODATA_TRUE:
		{
			UpdateCO2InfoData(true);
			return 1;
		}
		break;
	case WM_UPDATECO2INFODATA_FALSE:
		{
			UpdateCO2InfoData(false);
			return 1;
		}
		break;

	case WM_UPDATELIMITDATA:
		{
			UpdateLimitData();
			return 1;
		}
		break;
	case WM_SETPRICO_TRUE:
		{
			setPRICOrunning(true);
			return 1;
		}
		break;
	case WM_SETPRICO_FALSE:
		{
			setPRICOrunning(false);
			return 1;
		}
		break;

	case WM_DRAW_SIQSPO2:
		{
			drawSIQofSPO2();
			return 1;
		}
		break;
	case WM_DRAW_FIO2VALUE:
		{
			drawMeasuredFiO2Value();
			return 1;
		}
		break;
	case WM_DRAW_FOT_STEP:
		{
			drawFOTsteps();
			this->SetFocus();
			return 1;
		}
		break;
	case WM_SET_PARATIMER:
		{
			SetTimer(PARATIMER, TIME_PARATIMER, NULL);
			if(getModel()->getVIEWHANDLER()->getViewState()==VS_ALARM_LIMIT)
			{
				getModel()->getVIEWHANDLER()->SetALIMITTIMER();
			}
			return 1;
		}
		break;
	case WM_ALIMIT_CHANGED:
		{
			EnterCriticalSection(&csDrawDataCO2);
			if(m_pDataCO2 && m_bDataCO2IsActive)
			{
				m_pDataCO2->UpdateMessureData(true);
			}
			LeaveCriticalSection(&csDrawDataCO2);
			
			EnterCriticalSection(&csDrawDataSPO2);
			if(m_pDataSPO2 && m_bDataSPO2IsActive)
			{
				m_pDataSPO2->UpdateLimitData();
			}
			LeaveCriticalSection(&csDrawDataSPO2);

			return 1;
		}
		break;
	case WM_FREEZE_GRAPHS:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_FREEZED);

			if(		getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS
				&&	getModel()->getCONFIG()->GetCurMode()!=VM_CPAP
				&&	getModel()->getCONFIG()->GetCurMode()!=VM_NCPAP
				&&	getModel()->getCONFIG()->GetCurMode()!=VM_DUOPAP
				&&	getModel()->getCONFIG()->GetCurMode()!=VM_THERAPIE)
			{
				m_bFreezeNextBreath=true;
				m_bFreeze=false;

				m_bRefreshLastBreathOfLoops=true;
			}
			else
			{
				m_bFreezeNextBreath=false;
				m_bFreeze=true;
			}

			m_bDrawSavedLoops=true;

			getModel()->getDATAHANDLER()->SetGraphFreezed(true);

			this->SetFocus();
			return 1;
		}
		break;
	case WM_CONTINUE_GRAPHS:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_CONTINUE);

			m_bResetAllDiagramms=true;
			m_bDrawSavedLoops=false;
			m_bFreezeNextBreath=false;
			m_bFreeze=false;

			setRedrawDiagram(REDRAW_GRAPHS);
			getModel()->getDATAHANDLER()->SetGraphFreezed(false);

			this->SetFocus();
			return 1;
		}
		break;
	case WM_DATA_CHANGED:
		{
			EnterCriticalSection(&csDrawDataCO2);
			if(m_pDataCO2 && m_bDataCO2IsActive)
			{
				m_pDataCO2->UpdateMessureData(false);
			}
			LeaveCriticalSection(&csDrawDataCO2);
			
			EnterCriticalSection(&csDrawDataSPO2);
			if(m_pDataSPO2 && m_bDataSPO2IsActive)
			{
				m_pDataSPO2->UpdateMessureData();
			}
			LeaveCriticalSection(&csDrawDataSPO2);
			
			if(m_bGraphVolumeIsActive)
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsCONVTrigger()==true)
				{
					if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_VOLUME)
					{
						int iTemp=getModel()->getDATAHANDLER()->getAVGMessureDataTrigger();
						if(iTemp!=m_iTrigThreshold)
						{
							m_iTrigThreshold=iTemp;
							EnterCriticalSection(&csDiagrammVOLUME);
							if(m_pDiagrammVolume)
								m_pDiagrammVolume->SetTriggerThreshold(m_iTrigThreshold);
							LeaveCriticalSection(&csDiagrammVOLUME);
						}
					}
					else
					{
						EnterCriticalSection(&csDiagrammVOLUME);
						if(m_pDiagrammVolume)
							m_pDiagrammVolume->SetTriggerThreshold(0);
						LeaveCriticalSection(&csDiagrammVOLUME);
					}
				}
			}

			if(m_bGraphFlowIsActive)
			{
				if(getModel()->getVMODEHANDLER()->activeModeIsCONVTrigger()==true)
				{
					if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_FLOW)
					{
						int iTemp=getModel()->getDATAHANDLER()->getAVGMessureDataTrigger();
						if(iTemp!=m_iTrigThreshold)
						{
							m_iTrigThreshold=iTemp;
							EnterCriticalSection(&csDiagrammFLOW);
							if(m_pDiagrammFlow)
								m_pDiagrammFlow->SetTriggerThreshold(m_iTrigThreshold);
							LeaveCriticalSection(&csDiagrammFLOW);
						}
					}
					else
					{
						EnterCriticalSection(&csDiagrammFLOW);
						if(m_pDiagrammFlow)
							m_pDiagrammFlow->SetTriggerThreshold(0);
						LeaveCriticalSection(&csDiagrammFLOW);
					}
				}
			}

			if(m_bGraphPressureIsActive)
			{
				bool bPrevDrawHFOVolGaranty=getDrawHFOVolGaranty();
				double dbPrevHFOVolumeGarantyHigh=getValueVolGarantyHigh();
				double dbPrevHFOVolumeGarantyLow=getValueVolGarantyLow();

				bool bCalculated=false;
				setDrawHFOVolGaranty(drawHFOVGarantyLine());

				if(getDrawHFOVolGaranty())
				{
					bCalculated=CalculateHFOVolGaranty();
				}
				else
				{
					setValueVolGarantyHigh(0);
					setValueVolGarantyLow(0);
				}

				if(		bPrevDrawHFOVolGaranty!=getDrawHFOVolGaranty()
					||	dbPrevHFOVolumeGarantyHigh!=getValueVolGarantyHigh()
					||	dbPrevHFOVolumeGarantyLow!=getValueVolGarantyLow())
				{
					if(bCalculated)
					{
						EnterCriticalSection(&csDiagrammPRESSURE);
						if(m_pDiagrammPressure)
							m_pDiagrammPressure->SetHFOVolumeGaranty(getDrawHFOVolGaranty(), getValueVolGarantyHigh(), getValueVolGarantyLow());
						LeaveCriticalSection(&csDiagrammPRESSURE);
					}
				}
			}
			return 1;
		}
		break;
	case WM_CHECK_PRICO_ALARMS:
		{
			EnterCriticalSection(&csDrawDataSPO2);
			if(m_pDataSPO2 && m_bDataSPO2IsActive)
			{
				m_pDataSPO2->PostMessage(WM_CHECK_PRICO_ALARMS);
			}
			LeaveCriticalSection(&csDrawDataSPO2);
		}
		break;
	case WM_CHECK_FOT_ALARMS:
		{
			EnterCriticalSection(&csDrawDataFOT);
			if(m_pDataFOT && m_bDataFOTIsActive)
			{
				m_pDataFOT->PostMessage(WM_CHECK_FOT_ALARMS);
			}
			LeaveCriticalSection(&csDrawDataFOT);
		}
		break;
	case WM_REDRAW_FOT_STATE:
		{
			EnterCriticalSection(&csDrawDataFOT);
			if(m_pDataFOT && m_bDataFOTIsActive)
			{
				m_pDataFOT->PostMessage(WM_REDRAW_FOT_STATE);
			}
			LeaveCriticalSection(&csDrawDataFOT);
		}
		break;
	case WM_GRAPH_FLOWSENSORSTATE:
		{
			//rku check1
			EnterCriticalSection(&csMenuGraphs);
			if(m_pWndMenuGraphs)
				m_pWndMenuGraphs->FlowSensorStateChanged();
			LeaveCriticalSection(&csMenuGraphs);
			

			if(		getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF
				||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_MANOFF
				||	getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_AUTOOFF)
			{
				if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_WAVE);
				}
				else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_WAVE);
				}
				else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_WAVE);
				}
				else
				{
					Show();
				}
			}
			else
			{
				Show();
			}
			return 1;
		}
		break;
	case WM_GRAPH_REDRAW:
		{
			Show();

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->SetFocus();

			return 1;
		}
		break;
	case WM_GRAPH_CHANGESUBSTATE:
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_EV_BN_GRAPH_WAVE);

			return 1;
		}
		break;
	case WM_SET_GRAPHTIMER:
		{
			KillTimer(GRAPHTIMER);
			SetTimer(GRAPHTIMER, 5000, NULL);
			return 1;
		}
		break;
	case WM_KILL_GRAPHTIMER:
		{
			KillTimer(GRAPHTIMER);
			return 1;
		}
		break;
	case WM_SELECT_NEXTGRAPH:
		{
			if(getModel()->getVIEWHANDLER()->getViewState()==VS_GRAPH)
			{
				if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
				{
					OnBnClickedNextGraph();
				}
				else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
				{
					OnBnClickedNextGraph();
				}
				else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
				{
					OnBnClickedNextGraph();
				}
				else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
				{
					OnBnClickedNextGraph();
				}
				else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SINGLE_LINEGRAPHS
					||	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_NONE)
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->SetFocus();
				}
				else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_MULTI_LINEGRAPHS)
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
			return 1;
		}
		break;
	case WM_GRAPH_DECREASE_SPEED:
		{
			if(getModel()->isSafeTickCountDelayExpired(m_dwLastSpeedchange, GRAPH_SPEEDCHANGE_DELAY))
			{
				DecreaseGraphSpeed();
				m_dwLastSpeedchange=GetTickCount();
			}
			return 1;
		}
		break;
	case WM_GRAPH_INCREASE_SPEED:
		{
			if(getModel()->isSafeTickCountDelayExpired(m_dwLastSpeedchange, GRAPH_SPEEDCHANGE_DELAY))
			{
				IncreaseGraphSpeed();
				m_dwLastSpeedchange=GetTickCount();
			}
			return 1;
		}
		break;
	case WM_GRAPH_SETCURSOR:
		{
			if(m_iCurFocusedWnd==wParam)
			{
				if(getModel()->getVIEWHANDLER()->getViewState()==VS_GRAPH)
				{
					if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
					{
						switch(wParam)
						{
						/*case IDC_LOOP_FOT:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;*/
						case IDC_LINEDIAGRAM_PRESSURE:
						case IDC_LINEDIAGRAM_FLOW:
						case IDC_LINEDIAGRAM_VOLUME:
							{
								DrawCursor(IDC_LOOP_FOT);
							}
							break;
						default:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						}
					}
					else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
					{
						switch(wParam)
						{
						case IDC_LINEDIAGRAM_SPO2:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						case IDC_LINEDIAGRAM_PRESSURE:
						case IDC_LINEDIAGRAM_FLOW:
						case IDC_LINEDIAGRAM_VOLUME:
							{
								DrawCursor(IDC_LINEDIAGRAM_SPO2);
							}
							break;
						default:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						}
					}
					else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
					{
						switch(wParam)
						{
						case IDC_LINEDIAGRAM_CO2:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						case IDC_LINEDIAGRAM_PRESSURE:
						case IDC_LINEDIAGRAM_FLOW:
						case IDC_LINEDIAGRAM_VOLUME:
							{
								DrawCursor(IDC_LINEDIAGRAM_CO2);
							}
							break;
						default:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						}
					}
					else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
					{
						switch(wParam)
						{
						case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
							{
								DrawCursor(IDC_LOOPDIAGRAM_VOLUMEFLOW);
							}
							break;
						case IDC_LOOPDIAGRAM_VOLUMEFLOW:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						case IDC_LINEDIAGRAM_PRESSURE:
						case IDC_LINEDIAGRAM_FLOW:
						case IDC_LINEDIAGRAM_VOLUME:
							{
								DrawCursor(IDC_LOOPDIAGRAM_PRESSUREVOLUME);
							}
							break;
						default:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						}
					}//rku check1
					/*else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SINGLE_LINEGRAPHS
						||	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_NONE)
					{
					}*/
					else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_MULTI_LINEGRAPHS)
					{
						switch(wParam)
						{
						case IDC_LINEDIAGRAM_PRESSURE:
							{
								DrawCursor(IDC_LINEDIAGRAM_FLOW);
							}
							break;
						case IDC_LINEDIAGRAM_FLOW:
							{
								DrawCursor(IDC_LINEDIAGRAM_VOLUME);
							}
							break;
						case IDC_LINEDIAGRAM_VOLUME:
							{
								DrawCursor(IDC_LINEDIAGRAM_PRESSURE);
							}
							break;
						default:
							{
								DrawCursor(IDC_LINEDIAGRAM_PRESSURE);
							}
							break;
						}
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
				if(getModel()->getVIEWHANDLER()->getViewState()==VS_GRAPH)
				{
					if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
					{
						switch(m_iCurFocusedWnd)
						{
						case 0:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						case IDC_LINEDIAGRAM_SPO2:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						case IDC_LINEDIAGRAM_PRESSURE:
						case IDC_LINEDIAGRAM_FLOW:
						case IDC_LINEDIAGRAM_VOLUME:
							{
								DrawCursor(IDC_LOOP_FOT);
							}
							break;
						case IDC_LINEDIAGRAM:
							{
								DrawCursor(IDC_LOOP_FOT);
							}
							break;
						default:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						}
					}
					else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
					{
						switch(m_iCurFocusedWnd)
						{
						case 0:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						case IDC_LINEDIAGRAM_SPO2:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						case IDC_LINEDIAGRAM_PRESSURE:
						case IDC_LINEDIAGRAM_FLOW:
						case IDC_LINEDIAGRAM_VOLUME:
							{
								DrawCursor(IDC_LINEDIAGRAM_SPO2);
							}
							break;
						case IDC_LINEDIAGRAM:
							{
								DrawCursor(IDC_LINEDIAGRAM_SPO2);
							}
							break;
						default:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						}
					}
					else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
					{
						switch(m_iCurFocusedWnd)
						{
						case 0:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						case IDC_LINEDIAGRAM_CO2:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						case IDC_LINEDIAGRAM_PRESSURE:
						case IDC_LINEDIAGRAM_FLOW:
						case IDC_LINEDIAGRAM_VOLUME:
							{
								DrawCursor(IDC_LINEDIAGRAM_CO2);
							}
							break;
						case IDC_LINEDIAGRAM:
							{
								DrawCursor(IDC_LINEDIAGRAM_CO2);
							}
							break;
						default:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						}
					}
					else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
					{
						switch(m_iCurFocusedWnd)
						{
						case 0:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
							{
								DrawCursor(IDC_LOOPDIAGRAM_VOLUMEFLOW);
							}
							break;
						case IDC_LOOPDIAGRAM_VOLUMEFLOW:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						case IDC_LINEDIAGRAM_PRESSURE:
						case IDC_LINEDIAGRAM_FLOW:
						case IDC_LINEDIAGRAM_VOLUME:
							{
								DrawCursor(IDC_LOOPDIAGRAM_PRESSUREVOLUME);
							}
							break;
						case IDC_LINEDIAGRAM:
							{
								DrawCursor(IDC_LOOPDIAGRAM_PRESSUREVOLUME);
							}
							break;
						default:
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							break;
						}
					}//rku check1
					/*else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SINGLE_LINEGRAPHS
						||	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_NONE)
					{
					}*/
					else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_MULTI_LINEGRAPHS)
					{
						switch(m_iCurFocusedWnd)
						{
						case 0:
							{
								DrawCursor(IDC_LINEDIAGRAM_PRESSURE);
							}
							break;
						case IDC_LINEDIAGRAM_PRESSURE:
							{
								DrawCursor(IDC_LINEDIAGRAM_FLOW);
							}
							break;
						case IDC_LINEDIAGRAM_FLOW:
							{
								DrawCursor(IDC_LINEDIAGRAM_VOLUME);
							}
							break;
						case IDC_LINEDIAGRAM_VOLUME:
							{
								DrawCursor(IDC_LINEDIAGRAM_PRESSURE);
							}
							break;
						default:
							{

							}
							break;
						}
					}
				}
				else
				{
					if(AfxGetApp())
						AfxGetApp()->GetMainWnd()->SetFocus();
				}
			}
			return 1;
		}
		break;
	case WM_GRAPH_SETFOCUS:
		{
			if(getModel()->getVIEWHANDLER()->getViewState()==VS_GRAPH || getModel()->getVIEWHANDLER()->getViewState()==VS_PARA)
			{
				if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
				{
					switch(wParam)
					{
					case IDC_LINEDIAGRAM_PRESSURE:
					case IDC_LINEDIAGRAM_FLOW:
					case IDC_LINEDIAGRAM_VOLUME:
						{
							if(m_iCurFocusedWnd!=wParam)
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							else
							{
								DrawCursor(0);
							}
						}
						break;
					default:
						{
							if(m_iCurFocusedWnd!=wParam)
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							else
							{
								DrawCursor(0);
							}
						}
						break;
					}
				}
				else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
				{
					switch(wParam)
					{
					case IDC_LINEDIAGRAM_SPO2:
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
					case IDC_LINEDIAGRAM_PRESSURE:
					case IDC_LINEDIAGRAM_FLOW:
					case IDC_LINEDIAGRAM_VOLUME:
						{
							if(m_iCurFocusedWnd!=wParam)
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							else
							{
								DrawCursor(0);
							}
						}
						break;
					default:
						{
							if(m_iCurFocusedWnd!=wParam)
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							else
							{
								DrawCursor(0);
							}
						}
						break;
					}
				}
				else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
				{
					switch(wParam)
					{
					case IDC_LINEDIAGRAM_CO2:
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
					case IDC_LINEDIAGRAM_PRESSURE:
					case IDC_LINEDIAGRAM_FLOW:
					case IDC_LINEDIAGRAM_VOLUME:
						{
							if(m_iCurFocusedWnd!=wParam)
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							else
							{
								DrawCursor(0);
							}
						}
						break;
					default:
						{
							if(m_iCurFocusedWnd!=wParam)
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							else
							{
								DrawCursor(0);
							}
						}
						break;
					}
				}
				else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
				{
					switch(wParam)
					{
					case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
					case IDC_LOOPDIAGRAM_VOLUMEFLOW:
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
					case IDC_LINEDIAGRAM_PRESSURE:
					case IDC_LINEDIAGRAM_FLOW:
					case IDC_LINEDIAGRAM_VOLUME:
						{
							if(m_iCurFocusedWnd!=wParam)
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							else
							{
								DrawCursor(0);
							}
						}
						break;
					default:
						{
							if(m_iCurFocusedWnd!=wParam)
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							else
							{
								DrawCursor(0);
							}
						}
						break;
					}
				}
				else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SINGLE_LINEGRAPHS
					||	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_NONE)
				{
					switch(wParam)
					{
					case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
					case IDC_LOOPDIAGRAM_VOLUMEFLOW:
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
					case IDC_LINEDIAGRAM_PRESSURE:
					case IDC_LINEDIAGRAM_FLOW:
					case IDC_LINEDIAGRAM_VOLUME:
						{
							if(m_iCurFocusedWnd!=wParam)
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							else
							{
								DrawCursor(0);
							}
						}
						break;
					default:
						{
							if(m_iCurFocusedWnd!=wParam)
							{
								DrawCursor(IDC_LINEDIAGRAM);
							}
							else
							{
								DrawCursor(0);
							}
						}
						break;
					}
				}
				else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_MULTI_LINEGRAPHS)
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
			}
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->SetFocus();

			return 1;
		}
		break;
	case WM_GRAPH_LOSTFOCUS:
		{
			if(m_bGraphSelected)
				DrawCursor(0);
			return 1;
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

// **************************************************************************
// 
// **************************************************************************
bool CViewDiagramm::CalculateHFOVolGaranty()
{
	bool bRes=true;
	int iHFAMPsoll=getModel()->getDATAHANDLER()->GetCurrentHFAMPLmaxPara();
	int iMessHFAMP=getModel()->getDATAHANDLER()->getAVGMessureDataHFAmpl();
	int iMessPmax=getModel()->getDATAHANDLER()->getAVGMessureDataPINSP();
	int iDiffHFAMP=iHFAMPsoll-iMessHFAMP;

	if(iMessPmax== 0 || iMessHFAMP==0 || iHFAMPsoll==0)
		bRes=false;

	double highVal=(double)(iMessPmax+(iDiffHFAMP/2))/10;
	double lowVal=(double)((iMessPmax+(iDiffHFAMP/2))-iHFAMPsoll)/10;
	setValueVolGarantyHigh(highVal);
	setValueVolGarantyLow(lowVal);

	return bRes;
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::RedrawDiagrammData(int iDiagrammID)
{
	if(false==doThread())
	{
		return;
	}

	switch(iDiagrammID)
	{
	case IDC_LINEDIAGRAM_PRESSURE:
		{
			setDrawHFOVolGaranty(drawHFOVGarantyLine());
			bool bCalculated=false;
			if(getDrawHFOVolGaranty())
			{
				bCalculated=CalculateHFOVolGaranty();
			}
			else
			{
				setValueVolGarantyHigh(0);
				setValueVolGarantyLow(0);
			}

			if(getDrawHFOVolGaranty() && bCalculated)
			{
				EnterCriticalSection(&csDiagrammPRESSURE);
				if(m_pDiagrammPressure)
					m_pDiagrammPressure->SetHFOVolumeGaranty(getDrawHFOVolGaranty(), getValueVolGarantyHigh(), getValueVolGarantyLow());
				LeaveCriticalSection(&csDiagrammPRESSURE);
			}

			setRedrawDiagram(REDRAW_PRESSUREGRAPH);
		}
		break;
	case IDC_LINEDIAGRAM_FLOW:
		{
			setRedrawDiagram(REDRAW_FLOWGRAPH);
		}
		break;
	case IDC_LINEDIAGRAM_VOLUME:
		{
			setRedrawDiagram(REDRAW_VOLUMEGRAPH);
		}
		break;
	case IDC_LINEDIAGRAM_CO2:
		{
			setRedrawDiagram(REDRAW_CO2GRAPH);
		}
		break;
	case IDC_LINEDIAGRAM_SPO2:
		{
			setRedrawDiagram(REDRAW_SPO2GRAPH);
		}
		break;
	case IDC_LOOP_FOT:
		{
			PostMessage(WM_DRAW_FOT_STEP);
		}
		break;
	default:
		{

		}
		break;
	}
}

// **************************************************************************
// 
// **************************************************************************
bool CViewDiagramm::DestroyWndMenuGraphs()
{
	//rku check1
	EnterCriticalSection(&csMenuGraphs);
	if(m_pWndMenuGraphs)
	{
		m_pWndMenuGraphs->DestroyWindow();
		delete m_pWndMenuGraphs;
		m_pWndMenuGraphs=NULL;
	}
	LeaveCriticalSection(&csMenuGraphs);
	
	return true;
}

// **************************************************************************
// 
// **************************************************************************
bool CViewDiagramm::CreateWndMenuGraphs()
{
	if(isExit())
		return false;

	//rku check1
	EnterCriticalSection(&csMenuGraphs);
	if(m_pWndMenuGraphs==NULL && m_lX>-1)
	{
		m_pWndMenuGraphs = new CWndMenuGraphs(this);
		RECT rcLd={0,551,610,600};

		if(m_pWndMenuGraphs->Create(AfxGetApp()->GetMainWnd(),rcLd,IDC_VIEW_GRAPHMENU))
		{
			m_pWndMenuGraphs->Init();
			LeaveCriticalSection(&csMenuGraphs);
			return true;
		}
	}
	LeaveCriticalSection(&csMenuGraphs);
	
	return false;
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::ShowWndMenuGraphs(bool bShow)
{
	if(isExit())
		return;

	//rku check1
	EnterCriticalSection(&csMenuGraphs);
	if(m_pWndMenuGraphs)
	{
		m_pWndMenuGraphs->Show(bShow);
	}
	LeaveCriticalSection(&csMenuGraphs);
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::NotifyVentModeChanged()
{
	bool bResetAll=false;
	if(getModel()->getVMODEHANDLER()->activeModeIsTrigger()==false)
	{
		EnterCriticalSection(&csDiagrammSPO2);
		if(m_pDiagrammSPO2)
		{
			m_pDiagrammSPO2->SetTriggeredBreath(false);
		}
		LeaveCriticalSection(&csDiagrammSPO2);

		EnterCriticalSection(&csDiagrammCO2);
		if(m_pDiagrammCO2)
		{
			m_pDiagrammCO2->SetTriggeredBreath(false);
		}
		LeaveCriticalSection(&csDiagrammCO2);

		EnterCriticalSection(&csDiagrammPRESSURE);
		if(m_pDiagrammPressure)
		{
			m_pDiagrammPressure->SetTriggeredBreath(false);
		}
		LeaveCriticalSection(&csDiagrammPRESSURE);

		EnterCriticalSection(&csDiagrammVOLUME);
		if(m_pDiagrammVolume)
		{
			m_pDiagrammVolume->SetTriggeredBreath(false);
			m_pDiagrammVolume->SetTriggerThreshold(0);
		}
		LeaveCriticalSection(&csDiagrammVOLUME);

		EnterCriticalSection(&csDiagrammFLOW);
		if(m_pDiagrammFlow)
		{
			m_pDiagrammFlow->SetTriggeredBreath(false);
			m_pDiagrammFlow->SetTriggerThreshold(0);
		}
		LeaveCriticalSection(&csDiagrammFLOW);

		EnterCriticalSection(&csDiagrammLOOP);
		if(m_pLoopPressureVolume)
		{
			m_pLoopPressureVolume->SetTriggeredBreath(false);
		}
		LeaveCriticalSection(&csDiagrammLOOP);

		EnterCriticalSection(&csDiagrammLOOP);
		if(m_pLoopVolumeFlow)
		{
			m_pLoopVolumeFlow->SetTriggeredBreath(false);
		}
		LeaveCriticalSection(&csDiagrammLOOP);
		m_iTrigThreshold=0;
	}
	else if(getModel()->getVMODEHANDLER()->activeModeIsCONVTrigger()==true)
	{
		int iTempTrig=getModel()->getDATAHANDLER()->getAVGMessureDataTrigger();

		if(iTempTrig!=m_iTrigThreshold)
		{
			m_iTrigThreshold=iTempTrig;
		}
	}

	bool bPrevDrawVolumeLimit=getDrawVolLimit();
	double dbPrevVolumeLimit=getValueVolLimit();

	setDrawVolLimit(IsVLimitStateOn());
	setValueVolLimit((getModel()->getDATAHANDLER()->GetCurrentVLimitPara()));

	EnterCriticalSection(&csDiagrammVOLUME);
	if(m_pDiagrammVolume && m_bGraphVolumeIsActive)
	{
		if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_VOLUME && getModel()->getVMODEHANDLER()->activeModeIsCONVTrigger())
		{
			m_pDiagrammVolume->SetTriggerThreshold(m_iTrigThreshold);
		}
		else
		{
			m_pDiagrammVolume->SetTriggerThreshold(0);
		}

		if(bPrevDrawVolumeLimit!=getDrawVolLimit() || dbPrevVolumeLimit!=getValueVolLimit())
		{
			m_pDiagrammVolume->SetVolumeLimit(getDrawVolLimit(), getValueVolLimit());
			bResetAll=true;
		}
	}
	LeaveCriticalSection(&csDiagrammVOLUME);

	EnterCriticalSection(&csDiagrammFLOW);
	if(m_pDiagrammFlow && m_bGraphFlowIsActive)
	{
		if(getModel()->getCONFIG()->getTriggerOption_CONV()==TRIGGER_FLOW && getModel()->getVMODEHANDLER()->activeModeIsCONVTrigger())
		{
			m_pDiagrammFlow->SetTriggerThreshold(m_iTrigThreshold);
		}
		else
		{
			m_pDiagrammFlow->SetTriggerThreshold(0);
		}
	}
	LeaveCriticalSection(&csDiagrammFLOW);


	bool bPrevDrawVolumeGaranty=getDrawVolGaranty();
	double dbPrevVolumeGaranty=getValueVolGaranty();

	setDrawVolGaranty(drawVGarantyLine());
	setValueVolGaranty(getModel()->getDATAHANDLER()->GetActiveModePMAXVGPara()/10);//newVG

	EnterCriticalSection(&csDiagrammPRESSURE);
	if(m_pDiagrammPressure && m_bGraphPressureIsActive)
	{
		if(bPrevDrawVolumeGaranty!=getDrawVolGaranty() || dbPrevVolumeGaranty!=getValueVolGaranty())
		{
			m_pDiagrammPressure->SetVolumeGaranty(getDrawVolGaranty(), getValueVolGaranty());
			bResetAll=true;
		}
	}
	LeaveCriticalSection(&csDiagrammPRESSURE);


	bool bRedraw=false;

	if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
	{
		bool bVentmodeChangedToHFO=false;
		bool bVentmodeChangedFromHFO=false;

		if(getModel()->getCONFIG()->GetCurMode()==VM_HFO && getModel()->getCONFIG()->GetPrevMode()!=VM_HFO)
		{
			bVentmodeChangedToHFO=true;
			bRedraw=true;
		}
		else if(getModel()->getCONFIG()->GetCurMode()!=VM_HFO && getModel()->getCONFIG()->GetPrevMode()==VM_HFO)
		{
			bVentmodeChangedFromHFO=true;
			bRedraw=true;
		}
		else if(getModel()->getCONFIG()->GetCurMode()==VM_CPAP && getModel()->getCONFIG()->GetPrevMode()!=VM_CPAP)//newVG
		{
			bRedraw=true;
		}
		else if(getModel()->getCONFIG()->GetCurMode()!=VM_CPAP && getModel()->getCONFIG()->GetPrevMode()==VM_CPAP)//newVG
		{
			bRedraw=true;
		}
		else if(getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP && getModel()->getCONFIG()->GetPrevMode()!=VM_DUOPAP)//newVG
		{
			bRedraw=true;
		}
		else if(getModel()->getCONFIG()->GetCurMode()!=VM_DUOPAP && getModel()->getCONFIG()->GetPrevMode()==VM_DUOPAP)//newVG
		{
			bRedraw=true;
		}
		else if(getModel()->getCONFIG()->GetCurMode()==VM_NCPAP && getModel()->getCONFIG()->GetPrevMode()!=VM_NCPAP)//newVG
		{
			bRedraw=true;
		}
		else if(getModel()->getCONFIG()->GetCurMode()!=VM_NCPAP && getModel()->getCONFIG()->GetPrevMode()==VM_NCPAP)//newVG
		{
			bRedraw=true;
		}
		else if(getModel()->getCONFIG()->GetCurMode()==VM_THERAPIE && getModel()->getCONFIG()->GetPrevMode()!=VM_THERAPIE)//newVG
		{
			bRedraw=true;
		}
		else if(getModel()->getCONFIG()->GetCurMode()!=VM_THERAPIE && getModel()->getCONFIG()->GetPrevMode()==VM_THERAPIE)//newVG
		{
			bRedraw=true;
		}

		if(bVentmodeChangedToHFO)
		{
			setGraphSpeed(getModel()->getCONFIG()->GraphGetActualSpeedGraph());
			UpdateTimeAxis(false);
		}
		else if(bVentmodeChangedFromHFO)
		{
			setGraphSpeed(getModel()->getCONFIG()->GraphGetActualSpeedGraph());
			UpdateTimeAxis(false);
		}

		if(bRedraw)
			Show();
		//DrawView(bRedraw,true);
	}

	if(bResetAll && !bRedraw)
	{
		m_bResetAllDiagramms=true;
	}
	
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::NotifyDataChanged()
{
	PostMessage(WM_DATA_CHANGED);
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::NotifyEvent(CMVEvent* pEvent)
{
	switch(pEvent->GetET())
	{
	case CMVEvent::ET_ALARM:
		{
			CMVEventAlarm* pUIEvent = (CMVEventAlarm*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventAlarm::EV_ALARMSTATE_CHANGED:
				{
					if(getModel()->getALARMHANDLER()->ALARM_DISCONNECTION->getAlarmState()==AS_ACTIVE && getModel()->getCONFIG()->GraphIsAutoScale())
					{
						EnterCriticalSection(&csDiagrammPRESSURE);
						if(m_pDiagrammPressure && m_bGraphPressureIsActive)
						{
							m_pDiagrammPressure->IgnoreAutofit(true);
						}
						LeaveCriticalSection(&csDiagrammPRESSURE);

						EnterCriticalSection(&csDiagrammFLOW);
						if(m_pDiagrammFlow && m_bGraphFlowIsActive)
						{
							m_pDiagrammFlow->IgnoreAutofit(true);
						}
						LeaveCriticalSection(&csDiagrammFLOW);

						EnterCriticalSection(&csDiagrammVOLUME);
						if(m_pDiagrammVolume && m_bGraphVolumeIsActive)
						{
							m_pDiagrammVolume->IgnoreAutofit(true);
						}
						LeaveCriticalSection(&csDiagrammVOLUME);

						EnterCriticalSection(&csDiagrammLOOP);
						if(m_pLoopPressureVolume && m_bLoopPressureVolumeIsActive)
						{
							m_pLoopPressureVolume->IgnoreAutofit(true);
						}
						LeaveCriticalSection(&csDiagrammLOOP);

						EnterCriticalSection(&csDiagrammLOOP);
						if(m_pLoopVolumeFlow && m_bLoopVolumeFlowIsActive)
						{
							m_pLoopVolumeFlow->IgnoreAutofit(true);
						}
						LeaveCriticalSection(&csDiagrammLOOP);
					}
					else if(	getModel()->getALARMHANDLER()->ALARM_DISCONNECTION->getAlarmState()!=AS_ACTIVE 
						&&	getModel()->getCONFIG()->GraphIsAutoScale()
						&&	VENT_RUNNING==getModel()->GetVentRunState())
					{
						EnterCriticalSection(&csDiagrammPRESSURE);			
						if(m_pDiagrammPressure && m_bGraphPressureIsActive)
						{
							m_pDiagrammPressure->IgnoreAutofit(false);
						}
						LeaveCriticalSection(&csDiagrammPRESSURE);

						EnterCriticalSection(&csDiagrammFLOW);			
						if(m_pDiagrammFlow && m_bGraphFlowIsActive)
						{
							m_pDiagrammFlow->IgnoreAutofit(false);
						}
						LeaveCriticalSection(&csDiagrammFLOW);

						EnterCriticalSection(&csDiagrammVOLUME);			
						if(m_pDiagrammVolume && m_bGraphVolumeIsActive)
						{
							m_pDiagrammVolume->IgnoreAutofit(false);
						}
						LeaveCriticalSection(&csDiagrammVOLUME);

						EnterCriticalSection(&csDiagrammLOOP);			
						if(m_pLoopPressureVolume && m_bLoopPressureVolumeIsActive)
						{
							m_pLoopPressureVolume->IgnoreAutofit(false);
						}
						LeaveCriticalSection(&csDiagrammLOOP);

						EnterCriticalSection(&csDiagrammLOOP);
						if(m_pLoopVolumeFlow && m_bLoopVolumeFlowIsActive)
						{
							m_pLoopVolumeFlow->IgnoreAutofit(false);
						}
						LeaveCriticalSection(&csDiagrammLOOP);
					}

					if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true && m_bDataSPO2IsActive)
					{
						PostMessage(WM_CHECK_PRICO_ALARMS);
					}

					if(getModel()->getDATAHANDLER()->isFOTLicenseAvailable()==true && m_bDataFOTIsActive)
					{
						PostMessage(WM_CHECK_FOT_ALARMS);
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
	case CMVEvent::ET_CONTROL:
		{
			CMVEventControl* pCtrlEvent = (CMVEventControl*)pEvent;
			switch(pCtrlEvent->GetEventType())
			{
			/*case CMVEventControl::EV_CONTROL_START_FOT_CONVENTIONAL:
				{
					xxx
				}
				break;
			case CMVEventControl::EV_CONTROL_STOP_FOT_CONVENTIONAL:
				{
					xxx
				}
				break;*/
			case CMVEventControl::EV_CONTROL_STANDBY:
				{
					if(getModel()->getCONFIG()->GraphIsAutoScale())
					{
						EnterCriticalSection(&csDiagrammPRESSURE);
						if(m_pDiagrammPressure && m_bGraphPressureIsActive)
						{
							m_pDiagrammPressure->IgnoreAutofit(true);
						}
						LeaveCriticalSection(&csDiagrammPRESSURE);

						EnterCriticalSection(&csDiagrammFLOW);
						if(m_pDiagrammFlow && m_bGraphFlowIsActive)
						{
							m_pDiagrammFlow->IgnoreAutofit(true);
						}
						LeaveCriticalSection(&csDiagrammFLOW);

						EnterCriticalSection(&csDiagrammVOLUME);
						if(m_pDiagrammVolume && m_bGraphVolumeIsActive)
						{
							m_pDiagrammVolume->IgnoreAutofit(true);
						}
						LeaveCriticalSection(&csDiagrammVOLUME);

						EnterCriticalSection(&csDiagrammCO2);
						if(m_pDiagrammCO2 && m_bGraphCO2IsActive)
						{
							m_pDiagrammCO2->IgnoreAutofit(true);
						}
						LeaveCriticalSection(&csDiagrammCO2);

						EnterCriticalSection(&csDiagrammLOOP);
						if(m_pLoopPressureVolume && m_bLoopPressureVolumeIsActive)
						{
							m_pLoopPressureVolume->IgnoreAutofit(true);
						}
						LeaveCriticalSection(&csDiagrammLOOP);

						EnterCriticalSection(&csDiagrammLOOP);
						if(m_pLoopVolumeFlow && m_bLoopVolumeFlowIsActive)
						{
							m_pLoopVolumeFlow->IgnoreAutofit(true);
						}
						LeaveCriticalSection(&csDiagrammLOOP);
					}

					//SPO2 is allways autoscale
					EnterCriticalSection(&csDiagrammSPO2);
					if(m_pDiagrammSPO2 && m_bGraphSPO2IsActive)
					{
						m_pDiagrammSPO2->IgnoreAutofit(true);
					}
					LeaveCriticalSection(&csDiagrammSPO2);
				}
				break;
			case CMVEventControl::EV_CONTROL_START_VENTILATION:
				{
					if(getModel()->getCONFIG()->GraphIsAutoScale())
					{
						EnterCriticalSection(&csDiagrammPRESSURE);
						if(m_pDiagrammPressure && m_bGraphPressureIsActive)
						{
							m_pDiagrammPressure->IgnoreAutofit(false);
						}
						LeaveCriticalSection(&csDiagrammPRESSURE);

						EnterCriticalSection(&csDiagrammFLOW);
						if(m_pDiagrammFlow && m_bGraphFlowIsActive)
						{
							m_pDiagrammFlow->IgnoreAutofit(false);
						}
						LeaveCriticalSection(&csDiagrammFLOW);

						EnterCriticalSection(&csDiagrammVOLUME);
						if(m_pDiagrammVolume && m_bGraphVolumeIsActive)
						{
							m_pDiagrammVolume->IgnoreAutofit(false);
						}
						LeaveCriticalSection(&csDiagrammVOLUME);

						EnterCriticalSection(&csDiagrammCO2);
						if(m_pDiagrammCO2 && m_bGraphCO2IsActive)
						{
							m_pDiagrammCO2->IgnoreAutofit(false);
						}
						LeaveCriticalSection(&csDiagrammCO2);

						EnterCriticalSection(&csDiagrammLOOP);
						if(m_pLoopPressureVolume && m_bLoopPressureVolumeIsActive)
						{
							m_pLoopPressureVolume->IgnoreAutofit(false);
						}
						LeaveCriticalSection(&csDiagrammLOOP);

						EnterCriticalSection(&csDiagrammLOOP);
						if(m_pLoopVolumeFlow && m_bLoopVolumeFlowIsActive)
						{
							m_pLoopVolumeFlow->IgnoreAutofit(false);
						}
						LeaveCriticalSection(&csDiagrammLOOP);
					}

					//SPO2 is allways autoscale
					EnterCriticalSection(&csDiagrammSPO2);
					if(m_pDiagrammSPO2 && m_bGraphSPO2IsActive)
					{
						m_pDiagrammSPO2->IgnoreAutofit(false);
					}
					LeaveCriticalSection(&csDiagrammSPO2);
				}
				break;
			case CMVEventControl::EV_CONTROL_STOP_VENTILATION:
				{
					if(getModel()->getCONFIG()->GraphIsAutoScale())
					{
						EnterCriticalSection(&csDiagrammPRESSURE);
						if(m_pDiagrammPressure && m_bGraphPressureIsActive)
						{
							m_pDiagrammPressure->IgnoreAutofit(true);
						}
						LeaveCriticalSection(&csDiagrammPRESSURE);

						EnterCriticalSection(&csDiagrammFLOW);
						if(m_pDiagrammFlow && m_bGraphFlowIsActive)
						{
							m_pDiagrammFlow->IgnoreAutofit(true);
						}
						LeaveCriticalSection(&csDiagrammFLOW);

						EnterCriticalSection(&csDiagrammVOLUME);
						if(m_pDiagrammVolume && m_bGraphVolumeIsActive)
						{
							m_pDiagrammVolume->IgnoreAutofit(true);
						}
						LeaveCriticalSection(&csDiagrammVOLUME);

						EnterCriticalSection(&csDiagrammCO2);
						if(m_pDiagrammCO2 && m_bGraphCO2IsActive)
						{
							m_pDiagrammCO2->IgnoreAutofit(true);
						}
						LeaveCriticalSection(&csDiagrammCO2);

						EnterCriticalSection(&csDiagrammLOOP);
						if(m_pLoopPressureVolume && m_bLoopPressureVolumeIsActive)
						{
							m_pLoopPressureVolume->IgnoreAutofit(true);
						}
						LeaveCriticalSection(&csDiagrammLOOP);

						EnterCriticalSection(&csDiagrammLOOP);
						if(m_pLoopVolumeFlow && m_bLoopVolumeFlowIsActive)
						{
							m_pLoopVolumeFlow->IgnoreAutofit(true);
						}
						LeaveCriticalSection(&csDiagrammLOOP);
					}

					//SPO2 is always autoscale
					EnterCriticalSection(&csDiagrammSPO2);
					if(m_pDiagrammSPO2 && m_bGraphSPO2IsActive)
					{
						m_pDiagrammSPO2->IgnoreAutofit(true);
					}
					LeaveCriticalSection(&csDiagrammSPO2);
				}
				break;
			case CMVEventControl::EV_CONTROL_O2SENSORSTATE:
				{
					if(getModel()->getDATAHANDLER()->isPRICOLicenseAvailable()==true && m_bDataSPO2IsActive)
					{
						PostMessage(WM_CHECK_PRICO_ALARMS);
					}
				}
				break;
			case CMVEventControl::EV_CONTROL_FLOWSENSORSTATE:
			case CMVEventControl::EV_CONTROL_TRIGGERSTATE:
				{
					PostMessage( WM_GRAPH_FLOWSENSORSTATE, 0, 0);
				}
				break;
			case CMVEventControl::EV_CONTROL_START_MANBREATH:
				{
					if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
					{
						if(m_bGraphPressureIsActive)
						{
							bool bCalculated=false;
							setDrawHFOVolGaranty(drawHFOVGarantyLine());

							if(getDrawHFOVolGaranty())
							{
								bCalculated=CalculateHFOVolGaranty();
							}
							else
							{
								setValueVolGarantyHigh(0);
								setValueVolGarantyLow(0);
							}

							EnterCriticalSection(&csDiagrammPRESSURE);
							if(m_pDiagrammPressure)
							{
								m_pDiagrammPressure->SetHFOVolumeGaranty(getDrawHFOVolGaranty(), getValueVolGarantyHigh(), getValueVolGarantyLow());
							}
							LeaveCriticalSection(&csDiagrammPRESSURE);
						}
					}

					if(getModel()->getCONFIG()->GraphIsAutoScale())
					{
						if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
						{
							int iPmanual=getModel()->getDATAHANDLER()->PARADATA()->GetPManualHFOPara();
							int iMax=0;
							int iMin=0;
							if(		getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true 
								&&	getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
							{
								int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
								int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
								int iPmeanrec=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
								iMax=iPmeanrec+(iAmp/2);
								iMin=iPmean-(iAmp/2)-100;
							}
							else
							{
								int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
								int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
								iMax=iPmean+(iAmp/2);
								iMin=iPmean-(iAmp/2);
							}

							bool bRescale=false;
							if(iPmanual>iMax)
							{
								iMax=iPmanual;
								bRescale=true;
							}

							if(bRescale)
							{
								EnterCriticalSection(&csDiagrammPRESSURE);
								if(		m_pDiagrammPressure
									&&	m_bGraphPressureIsActive)
								{
									m_pDiagrammPressure->ScaleToNextPossibleYValue(iMax,iMin,true,true);
								}
								LeaveCriticalSection(&csDiagrammPRESSURE);
							}
							
						}
						else//MANBREATH_SCALE
						{
							if(getModel()->getVMODEHANDLER()->activeModeIsCPAP())
							{
								int iPmanual=getModel()->getDATAHANDLER()->GetActiveModePINSPPara();
								EnterCriticalSection(&csDiagrammPRESSURE);
								if(m_pDiagrammPressure &&	m_bGraphPressureIsActive)
								{
									m_pDiagrammPressure->ScaleToNextPossibleYValue(iPmanual,0,true,true);
								}
								LeaveCriticalSection(&csDiagrammPRESSURE);
							}
							else if(getModel()->getVMODEHANDLER()->activeModeIsNCPAP())
							{
								int iPmanual=getModel()->getDATAHANDLER()->GetActiveModePINSPPara();
								EnterCriticalSection(&csDiagrammPRESSURE);
								if(m_pDiagrammPressure &&	m_bGraphPressureIsActive)
								{
									m_pDiagrammPressure->ScaleToNextPossibleYValue(iPmanual,0,true,true);
								}
								LeaveCriticalSection(&csDiagrammPRESSURE);
							}
							else if(getModel()->getVMODEHANDLER()->activeModeIsDUOPAP())
							{
								int iPmanual=getModel()->getDATAHANDLER()->GetActiveModePINSPPara();
								EnterCriticalSection(&csDiagrammPRESSURE);
								if(m_pDiagrammPressure &&	m_bGraphPressureIsActive)
								{
									m_pDiagrammPressure->ScaleToNextPossibleYValue(iPmanual,0,true,true);
								}
								LeaveCriticalSection(&csDiagrammPRESSURE);
							}
							else
							{
								if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn())
								{
									int iPmaxVG=getModel()->getDATAHANDLER()->GetActiveModePMAXVGPara();
									int iPmanual=getModel()->getDATAHANDLER()->GetActiveModePINSPPara();

									bool bRescale=false;
									if(iPmanual>iPmaxVG)
										bRescale=true;

									if(bRescale)
									{
										EnterCriticalSection(&csDiagrammPRESSURE);
										if(m_pDiagrammPressure &&	m_bGraphPressureIsActive)
										{
											m_pDiagrammPressure->ScaleToNextPossibleYValue(iPmanual,0,true,true);
										}
										LeaveCriticalSection(&csDiagrammPRESSURE);
									}
								}
							}
						}
					}
				}	
				break;
			case CMVEventControl::EV_CONTROL_STOP_MANBREATH:
				{
					if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
					{
						if(m_bGraphPressureIsActive)
						{
							if(m_bGraphPressureIsActive)
							{
								bool bCalculated=false;
								setDrawHFOVolGaranty(drawHFOVGarantyLine());

								if(getDrawHFOVolGaranty())
								{
									bCalculated=CalculateHFOVolGaranty();
								}
								else
								{
									setValueVolGarantyHigh(0);
									setValueVolGarantyLow(0);
								}

								EnterCriticalSection(&csDiagrammPRESSURE);
								if(m_pDiagrammPressure)
								{
									m_pDiagrammPressure->SetHFOVolumeGaranty(getDrawHFOVolGaranty(), getValueVolGarantyHigh(), getValueVolGarantyLow());
								}
								LeaveCriticalSection(&csDiagrammPRESSURE);
							}
						}
					}

					if(getModel()->getCONFIG()->GraphIsAutoScale())
					{
						if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
						{
							int iPmanual=getModel()->getDATAHANDLER()->PARADATA()->GetPManualHFOPara();
							int iMax=0;
							int iMin=0;
							if(		getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true 
								&&	getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
							{
								int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
								int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
								int iPmeanrec=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
								iMax=iPmeanrec+(iAmp/2);
								iMin=iPmean-(iAmp/2)-100;
							}
							else
							{
								int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
								int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
								iMax=iPmean+(iAmp/2);
								iMin=iPmean-(iAmp/2);
							}

							bool bRescale=false;
							if(iPmanual>iMax)
							{
								bRescale=true;
							}

							if(bRescale)
							{
								EnterCriticalSection(&csDiagrammPRESSURE);
								if(		m_pDiagrammPressure
									&&	m_bGraphPressureIsActive)
								{
									m_pDiagrammPressure->ScaleToNextPossibleYValue(iMax,iMin,true,true);
								}
								LeaveCriticalSection(&csDiagrammPRESSURE);
							}
							
						}
						else//MANBREATH_SCALE
						{
							if(getModel()->getVMODEHANDLER()->activeModeIsCPAP())
							{
								int iVal=getModel()->getDATAHANDLER()->PARADATA()->GetPManualCPAPPara();

								EnterCriticalSection(&csDiagrammPRESSURE);
								if(m_pDiagrammPressure	&&	m_bGraphPressureIsActive)
								{
									m_pDiagrammPressure->ScaleToNextPossibleYValue(iVal,0,true,true);
								}
								LeaveCriticalSection(&csDiagrammPRESSURE);
							}
							else if(getModel()->getVMODEHANDLER()->activeModeIsNCPAP())
							{
								int iVal=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEPara();

								EnterCriticalSection(&csDiagrammPRESSURE);
								if(m_pDiagrammPressure	&&	m_bGraphPressureIsActive)
								{
									m_pDiagrammPressure->ScaleToNextPossibleYValue(iVal,0,true,true);
								}
								LeaveCriticalSection(&csDiagrammPRESSURE);
							}
							else if(getModel()->getVMODEHANDLER()->activeModeIsDUOPAP())
							{
								int iVal=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEPara();

								EnterCriticalSection(&csDiagrammPRESSURE);
								if(m_pDiagrammPressure)
									m_pDiagrammPressure->ScaleToNextPossibleYValue(iVal,0,true,true);
								LeaveCriticalSection(&csDiagrammPRESSURE);
							}
							else
							{
								if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn())
								{
									int iPmaxVG=getModel()->getDATAHANDLER()->GetActiveModePMAXVGPara();
									
									EnterCriticalSection(&csDiagrammPRESSURE);
									if(m_pDiagrammPressure &&	m_bGraphPressureIsActive)
									{
										m_pDiagrammPressure->ScaleToNextPossibleYValue(iPmaxVG,0,true,true);
									}
									LeaveCriticalSection(&csDiagrammPRESSURE);
								}
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
		}
		break;
	case CMVEvent::ET_MATRIX:
		{
			CMVEventMatrix* pMatEvent = (CMVEventMatrix*)pEvent;
			switch(pMatEvent->GetEventType())
			{
			case CMVEventMatrix::EV_BN_MATRIX_GRAPHS:
				{
					if(getModel()->getCONFIG()->GraphIsAutoScale()==false)
						PostMessage( WM_GRAPH_SETCURSOR, 0, 0);
				}
				break;
			case CMVEventMatrix::EV_BN_MATRIX_CURSOR_MOVEXUP:
				{
					if(m_bGraphSelected && getModel()->getCONFIG()->GraphIsAutoScale()==false)
					{
						PostMessage(WM_SET_GRAPHTIMER);

						if(getModel()->getVIEWHANDLER()->getViewState()==VS_GRAPH || getModel()->getVIEWHANDLER()->getViewState()==VS_PARA)
						{
							switch(m_iCurFocusedWnd)
							{
							case IDC_LINEDIAGRAM_PRESSURE:
								{
									EnterCriticalSection(&csDiagrammPRESSURE);
									if(m_pDiagrammPressure)
									{
										if(m_pDiagrammPressure->MoveXup())
										{
											if(getModel()->getDATAHANDLER()->IsGraphFreezed())
											{
												setRedrawDiagram(REDRAW_PVLOOP);
											}
										}
										else
										{
											if(AfxGetApp())
												AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
										}
									}
									LeaveCriticalSection(&csDiagrammPRESSURE);
								}
								break;
							default:
								{

								}
								break;
							}
						}
						DrawCursor(m_iCurFocusedWnd);
					}
				}
				break;
			case CMVEventMatrix::EV_BN_MATRIX_CURSOR_MOVEXDOWN:
				{
					if(m_bGraphSelected && getModel()->getCONFIG()->GraphIsAutoScale()==false)
					{
						PostMessage(WM_SET_GRAPHTIMER);

						if(getModel()->getVIEWHANDLER()->getViewState()==VS_GRAPH || getModel()->getVIEWHANDLER()->getViewState()==VS_PARA)
						{
							switch(m_iCurFocusedWnd)
							{
							case IDC_LINEDIAGRAM_PRESSURE:
								{
									EnterCriticalSection(&csDiagrammPRESSURE);
									if(m_pDiagrammPressure)
									{
										if(m_pDiagrammPressure->MoveXdown())
										{
											if(getModel()->getDATAHANDLER()->IsGraphFreezed())
											{
												setRedrawDiagram(REDRAW_PVLOOP);
											}
										}
										else
										{
											if(AfxGetApp())
												AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
										}
									}
									LeaveCriticalSection(&csDiagrammPRESSURE);
								}
								break;
							default:
								{
								}
								break;
							}
						}
						DrawCursor(m_iCurFocusedWnd);
					}
				}
				break;
			case CMVEventMatrix::EV_BN_MATRIX_CURSOR_MOVEYLEFT:
				{
					if(m_bGraphSelected && getModel()->getCONFIG()->GraphIsAutoScale()==false)
					{
						PostMessage(WM_SET_GRAPHTIMER);

						if(getModel()->getVIEWHANDLER()->getViewState()==VS_GRAPH || getModel()->getVIEWHANDLER()->getViewState()==VS_PARA)
						{
							switch(m_iCurFocusedWnd)
							{
							case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
								{
									EnterCriticalSection(&csDiagrammLOOP);
									if(m_pLoopPressureVolume)
									{
										if(m_pLoopPressureVolume->MoveYleft())
										{
											if(getModel()->getDATAHANDLER()->IsGraphFreezed())
											{
												setRedrawDiagram(REDRAW_PVLOOP);
											}
										}
										else
										{
											if(AfxGetApp())
												AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
										}
									}
									LeaveCriticalSection(&csDiagrammLOOP);
								}
								break;
							default:
								{
								}
								break;
							}
						}
						DrawCursor(m_iCurFocusedWnd);
					}
				}
				break;
			case CMVEventMatrix::EV_BN_MATRIX_CURSOR_MOVEYRIGHT:
				{
					if(m_bGraphSelected && getModel()->getCONFIG()->GraphIsAutoScale()==false)
					{
						PostMessage(WM_SET_GRAPHTIMER);

						if(getModel()->getVIEWHANDLER()->getViewState()==VS_GRAPH || getModel()->getVIEWHANDLER()->getViewState()==VS_PARA)
						{
							switch(m_iCurFocusedWnd)
							{
							case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
								{
									EnterCriticalSection(&csDiagrammLOOP);
									if(m_pLoopPressureVolume)
									{
										if(m_pLoopPressureVolume->MoveYright())
										{
											if(getModel()->getDATAHANDLER()->IsGraphFreezed())
											{
												setRedrawDiagram(REDRAW_PVLOOP);
											}
										}
										else
										{
											if(AfxGetApp())
												AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
										}
									}
									LeaveCriticalSection(&csDiagrammLOOP);
								}
								break;
							default:
								{
								}
								break;
							}
						}
						DrawCursor(m_iCurFocusedWnd);
					}
				}
				break;
			case CMVEventMatrix::EV_BN_MATRIX_CURSOR_LEFT:
				{
					if(m_bGraphSelected && getModel()->getCONFIG()->GraphIsAutoScale()==false)
					{
						if(getModel()->getVIEWHANDLER()->getViewState()==VS_GRAPH || getModel()->getVIEWHANDLER()->getViewState()==VS_PARA)
						{
							if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LOOP_FOT:
									{
										//increaseXScaleToNextValue_FOTLOOP();
									}
									break;
								case IDC_LINEDIAGRAM_PRESSURE:
								case IDC_LINEDIAGRAM_FLOW:
								case IDC_LINEDIAGRAM_VOLUME:
								case IDC_LINEDIAGRAM:
									{
										DecreaseGraphSpeed();
									}
									break;
								default:
									{
									}
									break;
								}
							}
							else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LINEDIAGRAM_SPO2:
								case IDC_LINEDIAGRAM_PRESSURE:
								case IDC_LINEDIAGRAM_FLOW:
								case IDC_LINEDIAGRAM_VOLUME:
								case IDC_LINEDIAGRAM:
									{
										DecreaseGraphSpeed();
									}
									break;
								default:
									{
									}
									break;
								}
							}
							else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LINEDIAGRAM_CO2:
								case IDC_LINEDIAGRAM_PRESSURE:
								case IDC_LINEDIAGRAM_FLOW:
								case IDC_LINEDIAGRAM_VOLUME:
								case IDC_LINEDIAGRAM:
									{
										DecreaseGraphSpeed();
									}
									break;
								default:
									{
									}
									break;
								}
							}
							else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
									{
										increaseXScaleToNextValue_PRESSUREVOLUME();
									}
									break;
								case IDC_LOOPDIAGRAM_VOLUMEFLOW:
									{
										increaseXScaleToNextValue_VOLUMEFLOW();
									}
									break;
								case IDC_LINEDIAGRAM_PRESSURE:
								case IDC_LINEDIAGRAM_FLOW:
								case IDC_LINEDIAGRAM_VOLUME:
								case IDC_LINEDIAGRAM:
									{
										DecreaseGraphSpeed();
									}
									break;
								default:
									{
									}
									break;
								}
							}
							else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SINGLE_LINEGRAPHS
								||	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_NONE)
							{
								DecreaseGraphSpeed();
							}
							else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_MULTI_LINEGRAPHS)
							{
								DecreaseGraphSpeed();
							}
						}
						else
						{
							DecreaseGraphSpeed();
						}
					}
					else if(getModel()->getCONFIG()->GraphIsAutoScale()==false)
					{
						DecreaseGraphSpeed();
					}

					if(m_bGraphPressureIsActive)
					{
						bool bCalculated=false;
						setDrawHFOVolGaranty(drawHFOVGarantyLine());

						if(getDrawHFOVolGaranty())
						{
							bCalculated=CalculateHFOVolGaranty();
						}
						else
						{
							setValueVolGarantyHigh(0);
							setValueVolGarantyLow(0);
						}

						if(getDrawHFOVolGaranty() && bCalculated)
						{
							EnterCriticalSection(&csDiagrammPRESSURE);
							if(m_pDiagrammPressure)
							{
								m_pDiagrammPressure->SetHFOVolumeGaranty(getDrawHFOVolGaranty(), getValueVolGarantyHigh(), getValueVolGarantyLow());
							}
							LeaveCriticalSection(&csDiagrammPRESSURE);
						}
					}
				}
				break;
			case CMVEventMatrix::EV_BN_MATRIX_CURSOR_RIGHT:
				{
					if(m_bGraphSelected && getModel()->getCONFIG()->GraphIsAutoScale()==false)
					{
						if(getModel()->getVIEWHANDLER()->getViewState()==VS_GRAPH || getModel()->getVIEWHANDLER()->getViewState()==VS_PARA)
						{
							if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LINEDIAGRAM_SPO2:
									{
										//decreaseXScale_FOTLOOP();
									}
									break;
								case IDC_LINEDIAGRAM_PRESSURE:
								case IDC_LINEDIAGRAM_FLOW:
								case IDC_LINEDIAGRAM_VOLUME:
								case IDC_LINEDIAGRAM:
									{
										IncreaseGraphSpeed();
									}
									break;
								default:
									{
									}
									break;
								}
							}
							else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LINEDIAGRAM_SPO2:
								case IDC_LINEDIAGRAM_PRESSURE:
								case IDC_LINEDIAGRAM_FLOW:
								case IDC_LINEDIAGRAM_VOLUME:
								case IDC_LINEDIAGRAM:
									{
										IncreaseGraphSpeed();
									}
									break;
								default:
									{
									}
									break;
								}
							}
							else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LINEDIAGRAM_CO2:
								case IDC_LINEDIAGRAM_PRESSURE:
								case IDC_LINEDIAGRAM_FLOW:
								case IDC_LINEDIAGRAM_VOLUME:
								case IDC_LINEDIAGRAM:
									{
										IncreaseGraphSpeed();
									}
									break;
								default:
									{
									}
									break;
								}
							}
							else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
									{
										decreaseXScale_PRESSUREVOLUME();
									}
									break;
								case IDC_LOOPDIAGRAM_VOLUMEFLOW:
									{
										decreaseXScale_VOLUMEFLOW();
									}
									break;
								case IDC_LINEDIAGRAM_PRESSURE:
								case IDC_LINEDIAGRAM_FLOW:
								case IDC_LINEDIAGRAM_VOLUME:
								case IDC_LINEDIAGRAM:
									{
										IncreaseGraphSpeed();
									}
									break;
								default:
									{
									}
									break;
								}
							}
							else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SINGLE_LINEGRAPHS
								||	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_NONE)
							{
								IncreaseGraphSpeed();
							}
							else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_MULTI_LINEGRAPHS)
							{
								IncreaseGraphSpeed();
							}
						}
						else
						{
							IncreaseGraphSpeed();
						}
					}
					else if(getModel()->getCONFIG()->GraphIsAutoScale()==false)
					{
						IncreaseGraphSpeed();
					}

					if(m_bGraphPressureIsActive)
					{
						bool bCalculated=false;
						setDrawHFOVolGaranty(drawHFOVGarantyLine());

						if(getDrawHFOVolGaranty())
						{
							bCalculated=CalculateHFOVolGaranty();
						}
						else
						{
							setValueVolGarantyHigh(0);
							setValueVolGarantyLow(0);
						}

						if(getDrawHFOVolGaranty() && bCalculated)
						{
							EnterCriticalSection(&csDiagrammPRESSURE);
							if(m_pDiagrammPressure)
							{
								m_pDiagrammPressure->SetHFOVolumeGaranty(getDrawHFOVolGaranty(), getValueVolGarantyHigh(), getValueVolGarantyLow());
							}
							LeaveCriticalSection(&csDiagrammPRESSURE);
						}
					}
				}
				break;
			case CMVEventMatrix::EV_BN_MATRIX_CURSOR_UP:
				{
					if(m_bGraphSelected && getModel()->getCONFIG()->GraphIsAutoScale()==false)
					{
						PostMessage(WM_SET_GRAPHTIMER);

						if(getModel()->getVIEWHANDLER()->getViewState()==VS_GRAPH || getModel()->getVIEWHANDLER()->getViewState()==VS_PARA)
						{
							if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LOOP_FOT:
									{
										//decreaseYScaleToNextValue_FOTLOOP();
									}
									break;
								case IDC_LINEDIAGRAM_PRESSURE:
									{
										decreaseYScaleToNextValue_PRESSURE();
									}
									break;
								case IDC_LINEDIAGRAM_FLOW:
									{
										decreaseYScaleToNextValue_FLOW();
									}
									break;
								case IDC_LINEDIAGRAM_VOLUME:
									{
										decreaseYScaleToNextValue_VOLUME();
									}
									break;
								case IDC_LINEDIAGRAM:
									{
										switch(m_iCurrentLineDiagramm)
										{
										case IDC_LINEDIAGRAM_PRESSURE:
											{
												decreaseYScaleToNextValue_PRESSURE();
											}
											break;
										case IDC_LINEDIAGRAM_FLOW:
											{
												decreaseYScaleToNextValue_FLOW();
											}
											break;
										case IDC_LINEDIAGRAM_VOLUME:
											{
												decreaseYScaleToNextValue_VOLUME();
											}
											break;
										}
									}
									break;
								default:
									{
									}
									break;
								}
							}
							else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LINEDIAGRAM_SPO2:
									{
										decreaseYScaleToNextValue_SPO2();
									}
									break;
								case IDC_LINEDIAGRAM_PRESSURE:
									{
										decreaseYScaleToNextValue_PRESSURE();
									}
									break;
								case IDC_LINEDIAGRAM_FLOW:
									{
										decreaseYScaleToNextValue_FLOW();
									}
									break;
								case IDC_LINEDIAGRAM_VOLUME:
									{
										decreaseYScaleToNextValue_VOLUME();
									}
									break;
								case IDC_LINEDIAGRAM:
									{
										switch(m_iCurrentLineDiagramm)
										{
										case IDC_LINEDIAGRAM_PRESSURE:
											{
												decreaseYScaleToNextValue_PRESSURE();
											}
											break;
										case IDC_LINEDIAGRAM_FLOW:
											{
												decreaseYScaleToNextValue_FLOW();
											}
											break;
										case IDC_LINEDIAGRAM_VOLUME:
											{
												decreaseYScaleToNextValue_VOLUME();
											}
											break;
										}
									}
									break;
								default:
									{
									}
									break;
								}
							}
							else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LINEDIAGRAM_CO2:
									{
										decreaseYScaleToNextValue_CO2();
									}
									break;
								case IDC_LINEDIAGRAM_PRESSURE:
									{
										decreaseYScaleToNextValue_PRESSURE();
									}
									break;
								case IDC_LINEDIAGRAM_FLOW:
									{
										decreaseYScaleToNextValue_FLOW();
									}
									break;
								case IDC_LINEDIAGRAM_VOLUME:
									{
										decreaseYScaleToNextValue_VOLUME();
									}
									break;
								case IDC_LINEDIAGRAM:
									{
										switch(m_iCurrentLineDiagramm)
										{
										case IDC_LINEDIAGRAM_PRESSURE:
											{
												decreaseYScaleToNextValue_PRESSURE();
											}
											break;
										case IDC_LINEDIAGRAM_FLOW:
											{
												decreaseYScaleToNextValue_FLOW();
											}
											break;
										case IDC_LINEDIAGRAM_VOLUME:
											{
												decreaseYScaleToNextValue_VOLUME();
											}
											break;
										}
									}
									break;
								default:
									{
									}
									break;
								}
							}
							else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
									{
										decreaseYScaleToNextValue_PRESSUREVOLUME();
									}
									break;
								case IDC_LOOPDIAGRAM_VOLUMEFLOW:
									{
										decreaseYScaleToNextValue_VOLUMEFLOW();
									}
									break;
								case IDC_LINEDIAGRAM_PRESSURE:
									{
										decreaseYScaleToNextValue_PRESSURE();
									}
									break;
								case IDC_LINEDIAGRAM_FLOW:
									{
										decreaseYScaleToNextValue_FLOW();
									}
									break;
								case IDC_LINEDIAGRAM_VOLUME:
									{
										decreaseYScaleToNextValue_VOLUME();
									}
									break;
								case IDC_LINEDIAGRAM:
									{
										switch(m_iCurrentLineDiagramm)
										{
										case IDC_LINEDIAGRAM_PRESSURE:
											{
												decreaseYScaleToNextValue_PRESSURE();
											}
											break;
										case IDC_LINEDIAGRAM_FLOW:
											{
												decreaseYScaleToNextValue_FLOW();
											}
											break;
										case IDC_LINEDIAGRAM_VOLUME:
											{
												decreaseYScaleToNextValue_VOLUME();
											}
											break;
										}
									}
									break;
								default:
									{
									}
									break;
								}
							}
							else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SINGLE_LINEGRAPHS
								||	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_NONE)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
									{
										decreaseYScaleToNextValue_PRESSUREVOLUME();
									}
									break;
								case IDC_LOOPDIAGRAM_VOLUMEFLOW:
									{
										decreaseYScaleToNextValue_VOLUMEFLOW();
									}
									break;
								case IDC_LINEDIAGRAM_PRESSURE:
									{
										decreaseYScaleToNextValue_PRESSURE();
									}
									break;
								case IDC_LINEDIAGRAM_FLOW:
									{
										decreaseYScaleToNextValue_FLOW();
									}
									break;
								case IDC_LINEDIAGRAM_VOLUME:
									{
										decreaseYScaleToNextValue_VOLUME();
									}
									break;
								case IDC_LINEDIAGRAM:
									{
										switch(m_iCurrentLineDiagramm)
										{
										case IDC_LINEDIAGRAM_PRESSURE:
											{
												decreaseYScaleToNextValue_PRESSURE();
											}
											break;
										case IDC_LINEDIAGRAM_FLOW:
											{
												decreaseYScaleToNextValue_FLOW();
											}
											break;
										case IDC_LINEDIAGRAM_VOLUME:
											{
												decreaseYScaleToNextValue_VOLUME();
											}
											break;
										}
									}
									break;
								default:
									{
									}
									break;
								}
							}
							else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_MULTI_LINEGRAPHS)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LINEDIAGRAM_PRESSURE:
									{
										decreaseYScaleToNextValue_PRESSURE();
									}
									break;
								case IDC_LINEDIAGRAM_FLOW:
									{
										decreaseYScaleToNextValue_FLOW();
									}
									break;
								case IDC_LINEDIAGRAM_VOLUME:
									{
										decreaseYScaleToNextValue_VOLUME();
									}
									break;
								default:
									{
									}
									break;
								}
							}
						}
						DrawCursor(m_iCurFocusedWnd);
					}


					if(m_bGraphPressureIsActive)
					{
						bool bCalculated=false;
						setDrawHFOVolGaranty(drawHFOVGarantyLine());

						if(getDrawHFOVolGaranty())
						{
							bCalculated=CalculateHFOVolGaranty();
						}
						else
						{
							setValueVolGarantyHigh(0);
							setValueVolGarantyLow(0);
						}

						if(getDrawHFOVolGaranty() && bCalculated)
						{
							EnterCriticalSection(&csDiagrammPRESSURE);
							if(m_pDiagrammPressure)
							{
								m_pDiagrammPressure->SetHFOVolumeGaranty(getDrawHFOVolGaranty(), getValueVolGarantyHigh(), getValueVolGarantyLow());
							}
							LeaveCriticalSection(&csDiagrammPRESSURE);
						}
					}
				}
				break;
			case CMVEventMatrix::EV_BN_MATRIX_CURSOR_DOWN:
				{
					if(m_bGraphSelected && getModel()->getCONFIG()->GraphIsAutoScale()==false)
					{
						PostMessage(WM_SET_GRAPHTIMER);

						if(getModel()->getVIEWHANDLER()->getViewState()==VS_GRAPH || getModel()->getVIEWHANDLER()->getViewState()==VS_PARA)
						{
							if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LOOP_FOT:
									{
										//increaseYScaleToNextValue_FOTLOOP();
									}
									break;
								case IDC_LINEDIAGRAM_PRESSURE:
									{
										increaseYScaleToNextValue_PRESSURE();
									}
									break;
								case IDC_LINEDIAGRAM_FLOW:
									{
										increaseYScaleToNextValue_FLOW();
									}
									break;
								case IDC_LINEDIAGRAM_VOLUME:
									{
										increaseYScaleToNextValue_VOLUME();
									}
									break;
								case IDC_LINEDIAGRAM:
									{
										switch(m_iCurrentLineDiagramm)
										{
										case IDC_LINEDIAGRAM_PRESSURE:
											{
												increaseYScaleToNextValue_PRESSURE();
											}
											break;
										case IDC_LINEDIAGRAM_FLOW:
											{
												increaseYScaleToNextValue_FLOW();
											}
											break;
										case IDC_LINEDIAGRAM_VOLUME:
											{
												increaseYScaleToNextValue_VOLUME();
											}
											break;
										}
									}
									break;
								default:
									{
									}
									break;
								}
							}
							else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LINEDIAGRAM_SPO2:
									{
										increaseYScaleToNextValue_SPO2();
									}
									break;
								case IDC_LINEDIAGRAM_PRESSURE:
									{
										increaseYScaleToNextValue_PRESSURE();
									}
									break;
								case IDC_LINEDIAGRAM_FLOW:
									{
										increaseYScaleToNextValue_FLOW();
									}
									break;
								case IDC_LINEDIAGRAM_VOLUME:
									{
										increaseYScaleToNextValue_VOLUME();
									}
									break;
								case IDC_LINEDIAGRAM:
									{
										switch(m_iCurrentLineDiagramm)
										{
										case IDC_LINEDIAGRAM_PRESSURE:
											{
												increaseYScaleToNextValue_PRESSURE();
											}
											break;
										case IDC_LINEDIAGRAM_FLOW:
											{
												increaseYScaleToNextValue_FLOW();
											}
											break;
										case IDC_LINEDIAGRAM_VOLUME:
											{
												increaseYScaleToNextValue_VOLUME();
											}
											break;
										}
									}
									break;
								default:
									{
									}
									break;
								}
							}
							else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LINEDIAGRAM_CO2:
									{
										increaseYScaleToNextValue_CO2();
									}
									break;
								case IDC_LINEDIAGRAM_PRESSURE:
									{
										increaseYScaleToNextValue_PRESSURE();
									}
									break;
								case IDC_LINEDIAGRAM_FLOW:
									{
										increaseYScaleToNextValue_FLOW();
									}
									break;
								case IDC_LINEDIAGRAM_VOLUME:
									{
										increaseYScaleToNextValue_VOLUME();
									}
									break;
								case IDC_LINEDIAGRAM:
									{
										switch(m_iCurrentLineDiagramm)
										{
										case IDC_LINEDIAGRAM_PRESSURE:
											{
												increaseYScaleToNextValue_PRESSURE();
											}
											break;
										case IDC_LINEDIAGRAM_FLOW:
											{
												increaseYScaleToNextValue_FLOW();
											}
											break;
										case IDC_LINEDIAGRAM_VOLUME:
											{
												increaseYScaleToNextValue_VOLUME();
											}
											break;
										}
									}
									break;
								default:
									{
									}
									break;
								}
							}
							else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
									{
										increaseYScaleToNextValue_PRESSUREVOLUME();
									}
									break;
								case IDC_LOOPDIAGRAM_VOLUMEFLOW:
									{
										increaseYScaleToNextValue_VOLUMEFLOW();
									}
									break;
								case IDC_LINEDIAGRAM_PRESSURE:
									{
										increaseYScaleToNextValue_PRESSURE();
									}
									break;
								case IDC_LINEDIAGRAM_FLOW:
									{
										increaseYScaleToNextValue_FLOW();
									}
									break;
								case IDC_LINEDIAGRAM_VOLUME:
									{
										increaseYScaleToNextValue_VOLUME();
									}
									break;
								case IDC_LINEDIAGRAM:
									{
										switch(m_iCurrentLineDiagramm)
										{
										case IDC_LINEDIAGRAM_PRESSURE:
											{
												increaseYScaleToNextValue_PRESSURE();
											}
											break;
										case IDC_LINEDIAGRAM_FLOW:
											{
												increaseYScaleToNextValue_FLOW();
											}
											break;
										case IDC_LINEDIAGRAM_VOLUME:
											{
												increaseYScaleToNextValue_VOLUME();
											}
											break;
										}
									}
									break;
								default:
									{
									}
									break;
								}
							}
							else if(	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SINGLE_LINEGRAPHS
								||	getModel()->getVIEWHANDLER()->getViewSubState()==VSS_NONE)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
									{
										increaseYScaleToNextValue_PRESSUREVOLUME();
									}
									break;
								case IDC_LOOPDIAGRAM_VOLUMEFLOW:
									{
										increaseYScaleToNextValue_VOLUMEFLOW();
									}
									break;
								case IDC_LINEDIAGRAM_PRESSURE:
									{
										increaseYScaleToNextValue_PRESSURE();
									}
									break;
								case IDC_LINEDIAGRAM_FLOW:
									{
										increaseYScaleToNextValue_FLOW();
									}
									break;
								case IDC_LINEDIAGRAM_VOLUME:
									{
										increaseYScaleToNextValue_VOLUME();
									}
									break;
								case IDC_LINEDIAGRAM:
									{
										switch(m_iCurrentLineDiagramm)
										{
										case IDC_LINEDIAGRAM_PRESSURE:
											{
												increaseYScaleToNextValue_PRESSURE();
											}
											break;
										case IDC_LINEDIAGRAM_FLOW:
											{
												increaseYScaleToNextValue_FLOW();
											}
											break;
										case IDC_LINEDIAGRAM_VOLUME:
											{
												increaseYScaleToNextValue_VOLUME();
											}
											break;
										}
									}
									break;
								default:
									{
									}
									break;
								}
							}
							else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_MULTI_LINEGRAPHS)
							{
								switch(m_iCurFocusedWnd)
								{
								case IDC_LINEDIAGRAM_PRESSURE:
									{
										increaseYScaleToNextValue_PRESSURE();
									}
									break;
								case IDC_LINEDIAGRAM_FLOW:
									{
										increaseYScaleToNextValue_FLOW();
									}
									break;
								case IDC_LINEDIAGRAM_VOLUME:
									{
										increaseYScaleToNextValue_VOLUME();
									}
									break;
								default:
									{
									}
									break;
								}
							}
						}
						DrawCursor(m_iCurFocusedWnd);
					}

					if(m_bGraphPressureIsActive)
					{
						bool bCalculated=false;
						setDrawHFOVolGaranty(drawHFOVGarantyLine());

						if(getDrawHFOVolGaranty())
						{
							bCalculated=CalculateHFOVolGaranty();
						}
						else
						{
							setValueVolGarantyHigh(0);
							setValueVolGarantyLow(0);
						}

						if(getDrawHFOVolGaranty() && bCalculated)
						{
							EnterCriticalSection(&csDiagrammPRESSURE);
							if(m_pDiagrammPressure)
							{
								m_pDiagrammPressure->SetHFOVolumeGaranty(getDrawHFOVolGaranty(), getValueVolGarantyHigh(), getValueVolGarantyLow());
							}
							LeaveCriticalSection(&csDiagrammPRESSURE);
						}
					}
				}
				break;
			default:
				break;
			}
		}
	case CMVEvent::ET_UIEVENT:
		{
			CMVEventUI* pUIEvent = (CMVEventUI*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventUI::EV_GRAPH_DELCURSOR:
				{
					if(true==m_bGraphSelected)
					{
						DrawCursor(0);
					}
				}
				break;
			case CMVEventUI::EV_BN_NEXT_GRAPH:
				{
					getModel()->getCONFIG()->GraphSetPrimaryLineDiagramm(m_iCurrentLineDiagramm);
					//DrawView(true,false);
					Show();
				}
				break;
			case CMVEventUI::EV_BN_GRAPH_WAVE:
			case CMVEventUI::EV_BN_GRAPH_LOOP:
			case CMVEventUI::EV_BN_GRAPH_CO2:
			case CMVEventUI::EV_BN_GRAPH_SPO2:
			case CMVEventUI::EV_BN_GRAPH_FOT:
				{
					m_bResetXaxe=false;
				}
				break;
			case CMVEventUI::EV_BN_GRAPH_SAVE:
				{
					try
					{
						SaveLoops();
						SetFocus();
					}
					catch (...)
					{
						theApp.ReportException(_T("CViewDiagramm::NotifyEvent() EV_BN_GRAPH_SAVE"));
					}
				}
				break;
			}
		}
	}
}

// **************************************************************************
// 
// **************************************************************************
//void CViewDiagramm::increaseXScaleToNextValue_FOTLOOP()
//{
//	EnterCriticalSection(&csDiagrammFOT);
//	if(m_pLoopFOT)
//	{
//		/*if(m_pLoopFOT->IncreaseXScaleToNextValue())
//		{
//			if(getModel()->getDATAHANDLER()->IsGraphFreezed())
//			{
//				setRedrawDiagram(REDRAW_FOT);
//			}
//		}
//		else*/
//		{
//			if(AfxGetApp())
//				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
//		}
//	}
//	LeaveCriticalSection(&csDiagrammFOT);
//
//	DrawCursor(m_iCurFocusedWnd);
//	PostMessage(WM_SET_GRAPHTIMER);
//}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::increaseXScaleToNextValue_PRESSUREVOLUME()
{
	EnterCriticalSection(&csDiagrammLOOP);
	if(m_pLoopPressureVolume)
	{
		if(m_pLoopPressureVolume->IncreaseXScaleToNextValue())
		{
			if(getModel()->getDATAHANDLER()->IsGraphFreezed())
			{
				setRedrawDiagram(REDRAW_PVLOOP);
			}
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
	}
	LeaveCriticalSection(&csDiagrammLOOP);

	DrawCursor(m_iCurFocusedWnd);
	PostMessage(WM_SET_GRAPHTIMER);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::increaseXScaleToNextValue_VOLUMEFLOW()
{
	EnterCriticalSection(&csDiagrammLOOP);
	if(m_pLoopVolumeFlow)
	{
		if(m_pLoopVolumeFlow->IncreaseXScaleToNextValue())
		{
			if(getModel()->getDATAHANDLER()->IsGraphFreezed())
			{
				setRedrawDiagram(REDRAW_VFLOOP);
			}
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
	}
	LeaveCriticalSection(&csDiagrammLOOP);

	DrawCursor(m_iCurFocusedWnd);
	PostMessage(WM_SET_GRAPHTIMER);
}
// **************************************************************************
// 
// **************************************************************************
//void CViewDiagramm::decreaseXScale_FOTLOOP()
//{
//	EnterCriticalSection(&csDiagrammFOT);
//	if(m_pLoopFOT)
//	{
//		/*if(m_pLoopFOT->DecreaseXScale(false,false))
//		{
//			if(getModel()->getDATAHANDLER()->IsGraphFreezed())
//			{
//				setRedrawDiagram(REDRAW_FOT);
//			}
//		}
//		else*/
//		{
//			if(AfxGetApp())
//				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
//		}
//	}
//	LeaveCriticalSection(&csDiagrammFOT);
//
//	DrawCursor(m_iCurFocusedWnd);
//	PostMessage(WM_SET_GRAPHTIMER);
//}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::decreaseXScale_PRESSUREVOLUME()
{
	EnterCriticalSection(&csDiagrammLOOP);
	if(m_pLoopPressureVolume)
	{
		if(m_pLoopPressureVolume->DecreaseXScale(false,false))
		{
			if(getModel()->getDATAHANDLER()->IsGraphFreezed())
			{
				setRedrawDiagram(REDRAW_PVLOOP);
			}
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
	}
	LeaveCriticalSection(&csDiagrammLOOP);

	DrawCursor(m_iCurFocusedWnd);
	PostMessage(WM_SET_GRAPHTIMER);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::decreaseXScale_VOLUMEFLOW()
{
	EnterCriticalSection(&csDiagrammLOOP);
	if(m_pLoopVolumeFlow)
	{
		if(m_pLoopVolumeFlow->DecreaseXScale(false,false))
		{
			if(getModel()->getDATAHANDLER()->IsGraphFreezed())
			{
				setRedrawDiagram(REDRAW_VFLOOP);
			}
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
	}
	LeaveCriticalSection(&csDiagrammLOOP);

	DrawCursor(m_iCurFocusedWnd);
	PostMessage(WM_SET_GRAPHTIMER);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::decreaseYScaleToNextValue_PRESSUREVOLUME()
{
	EnterCriticalSection(&csDiagrammLOOP);
	if(m_pLoopPressureVolume)
	{
		if(m_pLoopPressureVolume->DecreaseYScaleToNextValue())
		{
			if(getModel()->getDATAHANDLER()->IsGraphFreezed())
			{
				setRedrawDiagram(REDRAW_PVLOOP);
			}
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
	}
	LeaveCriticalSection(&csDiagrammLOOP);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::decreaseYScaleToNextValue_VOLUMEFLOW()
{
	EnterCriticalSection(&csDiagrammLOOP);
	if(m_pLoopVolumeFlow)
	{
		if(m_pLoopVolumeFlow->DecreaseYScaleToNextValue())
		{
			if(getModel()->getDATAHANDLER()->IsGraphFreezed())
			{
				setRedrawDiagram(REDRAW_VFLOOP);
			}
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
	}
	LeaveCriticalSection(&csDiagrammLOOP);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::decreaseYScaleToNextValue_SPO2()
{
	EnterCriticalSection(&csDiagrammSPO2);
	if(m_pDiagrammSPO2)
	{
		if(m_pDiagrammSPO2->DecreaseYScaleToNextValue())
		{
			setRedrawDiagram(REDRAW_SPO2GRAPH);
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
	}
	LeaveCriticalSection(&csDiagrammSPO2);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::decreaseYScaleToNextValue_CO2()
{
	EnterCriticalSection(&csDiagrammCO2);
	if(m_pDiagrammCO2)
	{
		if(m_pDiagrammCO2->DecreaseYScaleToNextValue())
		{
			setRedrawDiagram(REDRAW_CO2GRAPH);
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
	}
	LeaveCriticalSection(&csDiagrammCO2);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::decreaseYScaleToNextValue_PRESSURE()
{
	EnterCriticalSection(&csDiagrammPRESSURE);
	if(m_pDiagrammPressure)
	{
		if(m_pDiagrammPressure->DecreaseYScaleToNextValue())
		{
			setRedrawDiagram(REDRAW_PRESSUREGRAPH);
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
	}
	LeaveCriticalSection(&csDiagrammPRESSURE);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::decreaseYScaleToNextValue_FLOW()
{
	EnterCriticalSection(&csDiagrammFLOW);
	if(m_pDiagrammFlow)
	{
		if(m_pDiagrammFlow->DecreaseYScaleToNextValue())
		{
			setRedrawDiagram(REDRAW_FLOWGRAPH);
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
	}
	LeaveCriticalSection(&csDiagrammFLOW);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::decreaseYScaleToNextValue_VOLUME()
{
	EnterCriticalSection(&csDiagrammVOLUME);
	if(m_pDiagrammVolume)
	{
		if(m_pDiagrammVolume->DecreaseYScaleToNextValue())
		{
			setRedrawDiagram(REDRAW_VOLUMEGRAPH);
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
	}
	LeaveCriticalSection(&csDiagrammVOLUME);
}

// **************************************************************************
// 
// **************************************************************************
//void CViewDiagramm::decreaseYScaleToNextValue_FOTLOOP()
//{
//	EnterCriticalSection(&csDiagrammFOT);
//	if(m_pLoopFOT)
//	{
//		/*if(m_pLoopFOT->DecreaseYScaleToNextValue())
//		{
//			setRedrawDiagram(REDRAW_VOLUMEGRAPH);
//		}
//		else
//		{
//			if(AfxGetApp())
//				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
//		}*/
//	}
//	LeaveCriticalSection(&csDiagrammFOT);
//}

// **************************************************************************
// 
// **************************************************************************
//void CViewDiagramm::increaseYScaleToNextValue_FOTLOOP()
//{
//	EnterCriticalSection(&csDiagrammFOT);
//	if(m_pLoopFOT)
//	{
//		/*if(m_pLoopFOT->IncreaseYScaleToNextValue())
//		{
//			setRedrawDiagram(REDRAW_SPO2GRAPH);
//		}
//		else
//		{
//			if(AfxGetApp())
//				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
//		}*/
//	}
//	LeaveCriticalSection(&csDiagrammFOT);
//}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::increaseYScaleToNextValue_SPO2()
{
	EnterCriticalSection(&csDiagrammSPO2);
	if(m_pDiagrammSPO2)
	{
		if(m_pDiagrammSPO2->IncreaseYScaleToNextValue())
		{
			setRedrawDiagram(REDRAW_SPO2GRAPH);
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
	}
	LeaveCriticalSection(&csDiagrammSPO2);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::increaseYScaleToNextValue_CO2()
{
	EnterCriticalSection(&csDiagrammCO2);
	if(m_pDiagrammCO2)
	{
		if(m_pDiagrammCO2->IncreaseYScaleToNextValue())
		{
			setRedrawDiagram(REDRAW_CO2GRAPH);
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
	}
	LeaveCriticalSection(&csDiagrammCO2);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::increaseYScaleToNextValue_PRESSUREVOLUME()
{
	EnterCriticalSection(&csDiagrammLOOP);
	if(m_pLoopPressureVolume)
	{
		if(m_pLoopPressureVolume->IncreaseYScaleToNextValue())
		{
			if(getModel()->getDATAHANDLER()->IsGraphFreezed())
			{
				setRedrawDiagram(REDRAW_PVLOOP);
			}
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
	}
	LeaveCriticalSection(&csDiagrammLOOP);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::increaseYScaleToNextValue_VOLUMEFLOW()
{
	EnterCriticalSection(&csDiagrammLOOP);
	if(m_pLoopVolumeFlow)
	{
		if(m_pLoopVolumeFlow->IncreaseYScaleToNextValue())
		{
			if(getModel()->getDATAHANDLER()->IsGraphFreezed())
			{
				setRedrawDiagram(REDRAW_VFLOOP);
			}
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
	}
	LeaveCriticalSection(&csDiagrammLOOP);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::increaseYScaleToNextValue_PRESSURE()
{
	EnterCriticalSection(&csDiagrammPRESSURE);
	if(m_pDiagrammPressure)
	{
		if(m_pDiagrammPressure->IncreaseYScaleToNextValue())
		{
			setRedrawDiagram(REDRAW_PRESSUREGRAPH);
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
	}
	LeaveCriticalSection(&csDiagrammPRESSURE);
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::increaseYScaleToNextValue_FLOW()
{
	EnterCriticalSection(&csDiagrammFLOW);
	if(m_pDiagrammFlow)
	{
		if(m_pDiagrammFlow->IncreaseYScaleToNextValue())
		{
			setRedrawDiagram(REDRAW_FLOWGRAPH);
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
	}
	LeaveCriticalSection(&csDiagrammFLOW);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::increaseYScaleToNextValue_VOLUME()
{
	EnterCriticalSection(&csDiagrammVOLUME);
	if(m_pDiagrammVolume)
	{
		if(m_pDiagrammVolume->IncreaseYScaleToNextValue())
		{
			setRedrawDiagram(REDRAW_VOLUMEGRAPH);
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
	}
	LeaveCriticalSection(&csDiagrammVOLUME);
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::StartGraphThread( void )
{
	//DEBUGMSG(TRUE, (TEXT("StartGraphThread\r\n")));
	EnterCriticalSection(&csDoThread);
	bool bDoThread=m_bDoThread;
	LeaveCriticalSection(&csDoThread);

	if(bDoThread==false)//rku APPERR
	{
		m_pcwtGraphThread=AfxBeginThread(CGraphThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
		if(m_pcwtGraphThread!=NULL)
		{
			EnterCriticalSection(&csDoThread);
			m_bDoThread=true;;
			LeaveCriticalSection(&csDoThread);

			m_hThreadGraph=m_pcwtGraphThread->m_hThread;
			m_pcwtGraphThread->m_bAutoDelete = FALSE; 
			m_pcwtGraphThread->ResumeThread();
		}
	}
}

//************************************
// Method:    StopGraphThread
// FullName:  CViewDiagramm::StopGraphThread
// Access:    protected 
// Returns:   void
// Qualifier:
// Parameter: void
//
// 2015/06/17: checked for correct closing of thread
//************************************
void CViewDiagramm::StopGraphThread( void )
{
	try
	{
		EnterCriticalSection(&csDoThread);
		bool bDoThread=m_bDoThread;
		m_bDoThread=false;;
		LeaveCriticalSection(&csDoThread);

		//DEBUGMSG(TRUE, (TEXT("xxxxxxxxxxxxxxx StopGraphThread1\r\n")));
		if(bDoThread)
		{
			g_eventGraphData.SetEvent();

			if (WaitForSingleObject(m_pcwtGraphThread->m_hThread,3000) == WAIT_TIMEOUT)
			{
				theApp.getLog()->WriteLine(_T("#THR:031a EXCEPTION"));

				if(!TerminateThread(m_pcwtGraphThread,0))
				{
					theApp.getLog()->WriteLine(_T("#THR:031b EXCEPTION"));
				}
			}
		}

		//DEBUGMSG(TRUE, (TEXT("xxxxxxxxxxxxxxx StopGraphThread2\r\n")));

		if(m_pcwtGraphThread!=NULL)
		{
			delete m_pcwtGraphThread;
			m_pcwtGraphThread=NULL;

			if(m_hThreadGraph!=INVALID_HANDLE_VALUE)
			{
				CloseHandle(m_hThreadGraph);
				m_hThreadGraph=INVALID_HANDLE_VALUE;
			}
		}
		
	}
	catch (...)
	{
		theApp.ReportException(_T("CViewDiagramm::StopGraphThread"));

		throw;
	}
	
}

bool CViewDiagramm::doThread()
{
	bool bRes=false;
	EnterCriticalSection(&csDoThread);
	bRes=m_bDoThread;
	LeaveCriticalSection(&csDoThread);
	return bRes;
}
void CViewDiagramm::startThread()
{
	EnterCriticalSection(&csDoThread);
	m_bDoThread=true;
	LeaveCriticalSection(&csDoThread);
}
void CViewDiagramm::stopThread()
{
	EnterCriticalSection(&csDoThread);
	m_bDoThread=false;
	LeaveCriticalSection(&csDoThread);
}

bool CViewDiagramm::isExit()
{
	bool bRes=false;
	EnterCriticalSection(&csExit);
	bRes=m_bExit;
	LeaveCriticalSection(&csExit);
	return bRes;
}
void CViewDiagramm::setExit()
{
	EnterCriticalSection(&csExit);
	m_bExit=true;
	LeaveCriticalSection(&csExit);
}
//************************************
// Method:    CGraphThread
// FullName:  CGraphThread
// Access:    public static 
// Returns:   UINT
// Qualifier:
// Parameter: LPVOID pc
//
// 2015/06/17: checked for correct closing of thread
//************************************
static UINT CGraphThread( LPVOID pc )
{	
	try
	{
		((CViewDiagramm*)pc)->DrawDiagramm();
	}
	catch (CException* e)
	{
		TCHAR   szCause[255];
		e->GetErrorMessage(szCause, 255);

		CString errorStr=_T("");
		errorStr.Format(_T("CGraphThread: %s"),szCause);

		theApp.ReportException(errorStr);

		e->Delete();
	}
	catch(...)
	{
		theApp.ReportException(_T("CGraphThread"));

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_EXCEPTION);
	}

	return TRUE;
}

//************************************
// Method:    DrawDiagramm
// FullName:  CViewDiagramm::DrawDiagramm
// Access:    protected 
// Returns:   void
// Qualifier:
//
// 2015/06/17: checked for correct closing of thread
//************************************
UINT CViewDiagramm::DrawDiagramm()
{
	if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
	{
		m_bFilledGraph=false;
	}
	else
		m_bFilledGraph=getModel()->getCONFIG()->GraphIsFilled();

	bool bDrawDiagramms=false;
	int iNumSPIItems=1;

	int iTempXval=0;
	bool bTempXval=false;

	while(doThread())
	{
		DWORD dw = ::WaitForSingleObject(g_eventGraphData, INFINITE);

		switch(dw)
		{
		case WAIT_OBJECT_0://draw graphs
			{
				if(doThread()==false)
				{
					//TEST EXCEPTION
					//LPG_FUNCTIONSTRUCT_LD m_lpfsTest=NULL;
					//m_lpfsTest->ChartType=G_FILLEDCHART;
					/*try
					{
						LPG_FUNCTIONSTRUCT_LD m_lpfsTest=NULL;
						m_lpfsTest->ChartType=G_FILLEDCHART;
					}
					catch (CException* e)
					{
						TCHAR   szCause[255];
						e->GetErrorMessage(szCause, 255);
						
						CString errorStr=_T("");
						errorStr.Format(_T("CViewDiagramm1: exception %s"),szCause);
						
						theApp.ReportException(errorStr);

						e->Delete();
					}
					catch(...)
					{
						theApp.ReportException(_T("Unknown CViewDiagramm1"));
					}*/
					return 0;
				}

				if(getUpdateRingBufCopy())
				{
					deleteUpdateRingBufCopy();
					getModel()->getDATAHANDLER()->updateCopyDataBuffer();
				}

				if(doThread()==false)
				{
					/*try
					{
						LPG_FUNCTIONSTRUCT_LD m_lpfsTest=NULL;
						m_lpfsTest->ChartType=G_FILLEDCHART;
					}
					catch (CException* e)
					{
						TCHAR   szCause[255];
						e->GetErrorMessage(szCause, 255);

						CString errorStr=_T("");
						errorStr.Format(_T("CViewDiagramm1: exception %s"),szCause);

						theApp.ReportException(errorStr);

						e->Delete();
					}
					catch(...)
					{
						theApp.ReportException(_T("Unknown CViewDiagramm1"));
					}*/
					return 0;
				}

				checkForResetDiagramms();
				checkForScale();

				if(doThread()==false)
				{
					/*try
					{
						LPG_FUNCTIONSTRUCT_LD m_lpfsTest=NULL;
						m_lpfsTest->ChartType=G_FILLEDCHART;
					}
					catch (CException* e)
					{
						TCHAR   szCause[255];
						e->GetErrorMessage(szCause, 255);

						CString errorStr=_T("");
						errorStr.Format(_T("CViewDiagramm1: exception %s"),szCause);

						theApp.ReportException(errorStr);

						e->Delete();
					}
					catch(...)
					{
						theApp.ReportException(_T("Unknown CViewDiagramm1"));
					}*/
					return 0;
				}

				checkForRedraw();

				if(doThread()==false)
				{
					/*try
					{
						LPG_FUNCTIONSTRUCT_LD m_lpfsTest=NULL;
						m_lpfsTest->ChartType=G_FILLEDCHART;
					}
					catch (CException* e)
					{
						TCHAR   szCause[255];
						e->GetErrorMessage(szCause, 255);

						CString errorStr=_T("");
						errorStr.Format(_T("CViewDiagramm1: exception %s"),szCause);

						theApp.ReportException(errorStr);

						e->Delete();
					}
					catch(...)
					{
						theApp.ReportException(_T("Unknown CViewDiagramm1"));
					}*/
					return 0;
				}

				if(m_bFreeze)
				{
					if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
					{
						if(m_bRefreshLastBreathOfLoops)
						{
							m_bRefreshLastBreathOfLoops=false;

							redrawPressureVolumeLoop();
							//Sleep(0);
							redrawVolumeFlowLoop();
							//Sleep(0);
							refreshLastBreathOfPressureVolumeLoop();
							//Sleep(0);
							refreshLastBreathOfVolumeFlowLoop();
							//Sleep(0);
						}
						else if(m_bDrawSavedLoops)
						{
							m_bDrawSavedLoops=false;

							if(getModel()->getDATAHANDLER()->getSizeSavedBreath()>0)
							{
								drawSavedPressureVolumeLoop();
								//Sleep(0);
								drawSavedVolumeFlowLoop();
								//Sleep(0);
								refreshLastBreathOfPressureVolumeLoop();
								//Sleep(0);
								refreshLastBreathOfVolumeFlowLoop();
								//Sleep(0);
							}
						}
					}
				}
				else //if(doThread())	//draw Diagramms
				{
					bDrawDiagramms=false;

					iTempXval=0;
					bTempXval=false;
					
					iNumSPIItems=CalculateSPIItemsToDraw();

					if(iNumSPIItems>0)
						bDrawDiagramms=true;
					
					if(bDrawDiagramms)
					{
						m_iItemsToMerge=GetCurrentItemsToMerge();

						if(m_bGraphPressureIsActive)
						{
							iTempXval=drawPressureGraph(iNumSPIItems);
							bTempXval=true;
						}
						if(m_bGraphVolumeIsActive)
						{
							iTempXval=drawVolumeGraph(iNumSPIItems);
							bTempXval=true;
						}
						if(m_bGraphFlowIsActive)
						{
							iTempXval=drawFlowGraph(iNumSPIItems);
							bTempXval=true;
						}
						if(m_bGraphCO2IsActive)
						{
							iTempXval=drawCO2Graph(iNumSPIItems);
							bTempXval=true;
						}
						if(m_bGraphSPO2IsActive)
						{
							iTempXval=drawSPO2Graph(iNumSPIItems);
							bTempXval=true;
						}
						if(	(m_bLoopPressureVolumeIsActive || m_bLoopVolumeFlowIsActive))
						{
							DrawLoops(iNumSPIItems);
						}
						if(bTempXval)
						{
							setCurrentXtimevalGraphs(iTempXval);
						}
						m_iItemsToMerge=0;
					}
				}
				if(doThread()==false)
				{
					/*try
					{
						LPG_FUNCTIONSTRUCT_LD m_lpfsTest=NULL;
						m_lpfsTest->ChartType=G_FILLEDCHART;
					}
					catch (CException* e)
					{
						TCHAR   szCause[255];
						e->GetErrorMessage(szCause, 255);

						CString errorStr=_T("");
						errorStr.Format(_T("CViewDiagramm1: exception %s"),szCause);

						theApp.ReportException(errorStr);

						e->Delete();
					}
					catch(...)
					{
						theApp.ReportException(_T("Unknown CViewDiagramm1"));
					}*/
					return 0;
				}
			}
			break;
		default:
			{
			}
			break;
		}
		

	}//while(m_bDoGraphThread)

	DEBUGMSG(TRUE, (TEXT("xxxxxxxxxxxxxxx end thread\r\n")));
	return 0;//rku APPERR
}
bool CViewDiagramm::checkForRedraw()
{
	bool bRefresh=false;

	WORD redrawDiagram=getRedrawDiagram();

	if(redrawDiagram==REDRAW_NONE)
		return false;
	else
		resetRedrawDiagram();

	/*if(redrawDiagram & REDRAWBIT_FOT)
	{
	if(m_bLoopFOTIsActive)
	{
	redrawFOTLoop();
	Sleep(0);
	bRefresh=true;
	}
	}*/

	if(redrawDiagram & REDRAWBIT_SPO2GRAPH)
	{
		if(m_bGraphSPO2IsActive)
		{
			redrawSPO2Graph();
			Sleep(0);
			bRefresh=true;
		}
	}

	if(redrawDiagram & REDRAWBIT_CO2GRAPH)
	{
		if(m_bGraphCO2IsActive)
		{
			redrawCO2Graph();
			bRefresh=true;
			Sleep(0);
		}
	}

	if(redrawDiagram & REDRAWBIT_PRESSUREGRAPH)
	{
		if(m_bGraphPressureIsActive)
		{
			//DEBUGMSG(TRUE, (TEXT("REDRAWBIT_PRESSUREGRAPH\r\n")));
			redrawPressureGraph();
			Sleep(0);
			bRefresh=true;
		}
	}

	if(redrawDiagram & REDRAWBIT_FLOWGRAPH)
	{
		if(m_bGraphFlowIsActive)
		{
			//DEBUGMSG(TRUE, (TEXT("REDRAWBIT_FLOWGRAPH\r\n")));
			redrawFlowGraph();
			Sleep(0);
			bRefresh=true;
		}
	}

	if(redrawDiagram & REDRAWBIT_VOLUMEGRAPH)
	{
		if(m_bGraphVolumeIsActive)
		{
			//DEBUGMSG(TRUE, (TEXT("REDRAWBIT_VOLUMEGRAPH\r\n")));
			redrawVolumeGraph();
			Sleep(0);
			bRefresh=true;
		}
	}

	if(redrawDiagram & REDRAWBIT_PVLOOP)
	{
		if(m_bLoopPressureVolumeIsActive)
		{
			redrawPressureVolumeLoop();
			Sleep(0);
			refreshLastBreathOfPressureVolumeLoop();//rku DIAG
			Sleep(0);
			bRefresh=true;
		}
	}

	if(redrawDiagram & REDRAWBIT_VFLOOP)
	{
		if(m_bLoopVolumeFlowIsActive)
		{
			redrawVolumeFlowLoop();
			Sleep(0);
			refreshLastBreathOfVolumeFlowLoop();//rku DIAG
			Sleep(0);
			bRefresh=true;
		}
	}

	if(bRefresh && m_iCurFocusedWnd!=0)
		DrawCursor(m_iCurFocusedWnd);

	return bRefresh;
}
bool CViewDiagramm::checkForScale()
{
	bool bCheck=false;
	if(m_bDecreaseGraphSpeed)
	{
		//DEBUGMSG(TRUE, (TEXT("checkForScale1\r\n")));
		m_bDecreaseGraphSpeed=false;
		UINT iTemp = getGraphSpeed();
		if(iTemp<G_SPEED_49750MS)
		{
			iTemp++;
			setGraphSpeed(iTemp);
			UpdateTimeAxis(true);
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}

		if(m_bGraphSelected && getModel()->getCONFIG()->GraphIsAutoScale()==false)
		{
			PostMessage(WM_SET_GRAPHTIMER);
		}
		bCheck=true;
	}
	else if(m_bIncreaseGraphSpeed)
	{
		//DEBUGMSG(TRUE, (TEXT("checkForScale2\r\n")));
		m_bIncreaseGraphSpeed=false;
		UINT iTemp = getGraphSpeed();
		if(iTemp>G_SPEED_505MS)
		{
			iTemp--;
			setGraphSpeed(iTemp);
			UpdateTimeAxis(true);
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}

		if(m_bGraphSelected && getModel()->getCONFIG()->GraphIsAutoScale()==false)
		{
			PostMessage(WM_SET_GRAPHTIMER);
		}
		bCheck=true;
	}
	return bCheck;
}
bool CViewDiagramm::checkForResetDiagramms()
{
	bool bReset=false;
	if(m_bResetAllDiagramms)
	{
		resetCurrentXtimevalGraphs();
		resetAllDiagramms();
		

		getModel()->getDATAHANDLER()->updateCopyDataBuffer();

		EnterCriticalSection(&getModel()->getDATAHANDLER()->csSPIDataBuffer);
		getModel()->getDATAHANDLER()->m_rbufSPI.reset();
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csSPIDataBuffer);

		EnterCriticalSection(&getModel()->getDATAHANDLER()->csCO2DataBuffer);
		getModel()->getDATAHANDLER()->m_rbufCO2.reset();
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCO2DataBuffer);

		EnterCriticalSection(&getModel()->getDATAHANDLER()->csSPO2DataBuffer);
		getModel()->getDATAHANDLER()->m_rbufSPO2.reset();
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csSPO2DataBuffer);

		resetCurrentXtimevalGraphs();

		m_bResetAllDiagramms=false;


		bReset=true;
	}
	return bReset;
}
// **************************************************************************
// 
// **************************************************************************
UINT CViewDiagramm::CalculateSPIItemsToDraw()
{
	//if(false==doThread() || isExit()) //rku  not necessary here
	//	return 0;
	UINT iNumItems=0;

	/*if(getModel()->getVIEWHANDLER()->getViewSubState()==CMVModel::VSS_GRAPH_CLFIO2GRAPHS)
	{
		return 1;
	}
	else*/
	{
		EnterCriticalSection(&getModel()->getDATAHANDLER()->csSPIDataBuffer);

		UINT iCurSizeOfPlotItemsSPI=getModel()->getDATAHANDLER()->m_rbufSPI.getCount();
		UINT iSizeToReadSPI=0;
		UINT ifactorSPI=0;

		switch(getGraphSpeed())
		{
		case G_SPEED_505MS:
			{
				ifactorSPI=iCurSizeOfPlotItemsSPI/20;
				iSizeToReadSPI=ifactorSPI*20;
				iNumItems=iSizeToReadSPI;
			}
			break;
		case G_SPEED_1000MS:
			{
				ifactorSPI=iCurSizeOfPlotItemsSPI/20;
				iSizeToReadSPI=ifactorSPI*20;
				iNumItems=iSizeToReadSPI/2;
			}
			break;
		case G_SPEED_2487MS:	//es werden 20 Werte zu 4 zusammengefasst
			{
				ifactorSPI=iCurSizeOfPlotItemsSPI/20;
				iSizeToReadSPI=ifactorSPI*20;
				iNumItems=iSizeToReadSPI/5;
			}
			break;
		case G_SPEED_4975MS:
			{
				ifactorSPI=iCurSizeOfPlotItemsSPI/20;
				iSizeToReadSPI=ifactorSPI*20;
				iNumItems=iSizeToReadSPI/10;
			}
			break;
		case G_SPEED_9950MS:
			{
				ifactorSPI=iCurSizeOfPlotItemsSPI/20;
				iSizeToReadSPI=ifactorSPI*20;
				iNumItems=ifactorSPI;
			}
			break;
		case G_SPEED_19900MS:
			{
				ifactorSPI=iCurSizeOfPlotItemsSPI/40;
				iSizeToReadSPI=ifactorSPI*40;
				iNumItems=ifactorSPI;
			}
			break;
		case G_SPEED_29850MS:
			{
				ifactorSPI=iCurSizeOfPlotItemsSPI/60;
				iSizeToReadSPI=ifactorSPI*60;
				iNumItems=ifactorSPI;
			}
			break;
		case G_SPEED_39800MS:
			{
				ifactorSPI=iCurSizeOfPlotItemsSPI/80;
				iSizeToReadSPI=ifactorSPI*80;
				iNumItems=ifactorSPI;
			}
			break;
		case G_SPEED_49750MS:
			{
				ifactorSPI=iCurSizeOfPlotItemsSPI/100;
				iSizeToReadSPI=ifactorSPI*100;
				iNumItems=ifactorSPI;
			}
			break;
		case G_SPEED_99500MS:
			{
				ifactorSPI=iCurSizeOfPlotItemsSPI/200;
				iSizeToReadSPI=ifactorSPI*200;
				iNumItems=ifactorSPI;
			}
			break;
		case G_SPEED_199000MS:
			{
				ifactorSPI=iCurSizeOfPlotItemsSPI/400;
				iSizeToReadSPI=ifactorSPI*400;
				iNumItems=ifactorSPI;
			}
			break;
		case G_SPEED_398000MS:
			{
				ifactorSPI=iCurSizeOfPlotItemsSPI/800;
				iSizeToReadSPI=ifactorSPI*800;
				iNumItems=ifactorSPI;
			}
			break;
		case G_SPEED_796000MS:
			{
				ifactorSPI=iCurSizeOfPlotItemsSPI/1600;
				iSizeToReadSPI=ifactorSPI*1600;
				iNumItems=ifactorSPI;
			}
			break;
		default:
			{
				iNumItems=1;
			}
			break;
		}	

		UINT iCnt=0;

		EnterCriticalSection(&csSPIReadInBuffer);
		for(iCnt=0;iCnt<iSizeToReadSPI;iCnt++)
		{
			m_pbufSPIReadInBuffer[iCnt]=getModel()->getDATAHANDLER()->m_rbufSPI.read();
			getModel()->getDATAHANDLER()->writeCopyDataBuffer(m_pbufSPIReadInBuffer[iCnt]);
		}
		LeaveCriticalSection(&csSPIReadInBuffer);
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csSPIDataBuffer);

		if(iSizeToReadSPI>1600)
		{
			setSizeReadInBufferSPI(100);
			
			/*DEBUGMSG(TRUE, (TEXT("iCurSizeOfPlotItemsSPI %d\r\n"),iCurSizeOfPlotItemsSPI));
			DEBUGMSG(TRUE, (TEXT("iSizeToReadSPI %d\r\n"),iSizeToReadSPI));
			DEBUGMSG(TRUE, (TEXT("iNumItems %d\r\n"),iNumItems));*/
		}
		else
		{
			setSizeReadInBufferSPI(iSizeToReadSPI);
		}
	}

	return iNumItems;
}



// **************************************************************************
// 
// **************************************************************************
int CViewDiagramm::GetCurrentItemsToMerge()
{
	int iItems=0;

	switch(getGraphSpeed())
	{
	case G_SPEED_505MS:
		{
			iItems=1;
		}
		break;
	case G_SPEED_1000MS:
		{
			iItems=2;
		}
		break;
	case G_SPEED_2487MS:	//es werden 20 Werte zu 4 zusammengefasst
		{
			iItems=5;
		}
		break;
	case G_SPEED_4975MS:
		{
			iItems=10;
		}
		break;
	case G_SPEED_9950MS:
		{
			iItems=20;
		}
		break;
	case G_SPEED_19900MS:
		{
			iItems=40;
		}
		break;
	case G_SPEED_29850MS:
		{
			iItems=60;
		}
		break;
	case G_SPEED_39800MS:
		{
			iItems=80;
		}
		break;
	case G_SPEED_49750MS:
		{
			iItems=100;
		}
		break;
	case G_SPEED_99500MS:
		{
			iItems=200;
		}
		break;
	case G_SPEED_199000MS:
		{
			iItems=400;
		}
		break;
	case G_SPEED_398000MS:
		{
			iItems=800;
		}
		break;
	case G_SPEED_796000MS:
		{
			iItems=1600;
		}
		break;
	default:
		{
			iItems=1;
		}
		break;
	}

	return iItems;
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::CopyCurBreathToSavedBreath()
{
	EnterCriticalSection(&getModel()->getDATAHANDLER()->csSavedBreath);
	if(getModel()->getDATAHANDLER()->m_pbufSavedBreath==NULL)
	{
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csSavedBreath);
		return;
	}
	LeaveCriticalSection(&getModel()->getDATAHANDLER()->csSavedBreath);

	getModel()->getDATAHANDLER()->setSizeSavedBreath(0);

	int iSizeCurrentBreath=getSizeCurrentBreath();
	if(iSizeCurrentBreath>=G_MAXPOINTS-1)
	{
		setSizeCurrentBreath(0);
		return;
	}
	EnterCriticalSection(&csCurrentBreath);
	EnterCriticalSection(&csSPIReadInBuffer);

	if(m_pbufCurrentBreath!=NULL && iSizeCurrentBreath>0)
	{
		try
		{
			DEBUGMSG(TRUE, (TEXT("CopyCurBreathToSavedBreath m_iSizeCurrentBreath %d\r\n"),iSizeCurrentBreath));
			getModel()->getDATAHANDLER()->setSizeSavedBreath(iSizeCurrentBreath);
			errno_t err;

			EnterCriticalSection(&getModel()->getDATAHANDLER()->csSavedBreath);
			err = memcpy_s(getModel()->getDATAHANDLER()->m_pbufSavedBreath, sizeof(PBUFSPI)*getModel()->getDATAHANDLER()->getSizeSavedBreath(), m_pbufCurrentBreath, sizeof(PBUFSPI)*iSizeCurrentBreath );    
			
			LeaveCriticalSection(&getModel()->getDATAHANDLER()->csSavedBreath);
	
		}
		catch (...)
		{
			theApp.ReportException(_T("CViewDiagramm::CopyCurBreathToSavedBreath"));

			LeaveCriticalSection(&getModel()->getDATAHANDLER()->csSavedBreath);
		}
		Sleep(0);
	}
	LeaveCriticalSection(&csSPIReadInBuffer);
	LeaveCriticalSection(&csCurrentBreath);
}




// **************************************************************************
// Draws the pressure graph
//				Depending on speed and size of SPI data
// **************************************************************************
int CViewDiagramm::drawPressureGraph(int iNumItems)
{
	int iCurrentXtimeval=getCurrentXtimevalGraphs();

	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	//DEBUGMSG(TRUE, (TEXT("lock 26\r\n")));
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	if(m_pPlotItems !=NULL && m_lpfsPressure !=NULL)
	{
		int iCountBufPointer=0;
		bool bTrigger=false;
		bool bNewBreath=false;
		PBUFSPI _pBufRead;
		LONG minY=0;
		LONG maxY=0;

		int VECTOR_SIZE=m_iItemsToMerge;
		IntVector PressureNumbers(VECTOR_SIZE) ;
		IntVectorIt start, end, locationMax, locationMin ;

		if(iNumItems*VECTOR_SIZE>MAXSIZE_SPIREADIN_BUFFER)
		{
			//ERROR
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 26\r\n")));
			return 0;
		}

		EnterCriticalSection(&csSPIReadInBuffer);

		int iCountNumItems=0;

		for(int n=0;n<iNumItems;n++)
		{
			bTrigger=false;
			bNewBreath=false;

			// Initialize vector Numbers
			for(int iT=0;iT<VECTOR_SIZE;iT++)
			{
				_pBufRead=m_pbufSPIReadInBuffer[iCountBufPointer];

				PressureNumbers[iT] = _pBufRead.iValPressure;

				iCountBufPointer++;

				if(_pBufRead.bTrig)
					bTrigger=true;

				if(_pBufRead.bATZ)
				{
					bNewBreath=true;
				}
			}

			start = PressureNumbers.begin() ;   // location of first
			end = PressureNumbers.end() ;       // one past the location

			locationMin = min_element(start, end, less<int>());
			minY=(*locationMin);

			locationMax = max_element(start, end, less<int>());
			maxY=(*locationMax);

			if(minY!=maxY)
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMin);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>G_MAXPOINTS-1)
				{
					//ERROR
					iCountNumItems=G_MAXPOINTS-1;
				}

				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=false;
				iCountNumItems++;
				if(iCountNumItems>G_MAXPOINTS-1)
				{
					//ERROR
					iCountNumItems=G_MAXPOINTS-1;
				}
			}
			else
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>G_MAXPOINTS-1)
				{
					//ERROR
					iCountNumItems=G_MAXPOINTS-1;
				}
			}
			
			if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
			{
				if(m_bFreezeNextBreath )
				{
					m_bFreeze=true;
				}
			}
			else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
			{
				if(m_bFreezeNextBreath )
				{
					m_bFreeze=true;
				}
			}
			else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
			{
				if(m_bFreezeNextBreath )
				{
					m_bFreeze=true;
				}
			}
			else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
			{
				if(m_bFreezeNextBreath && bNewBreath)
				{
					m_bRefreshLastBreathOfLoops=true;
					m_bFreeze=true;
				}
			}
			else
			{
				if(m_bFreezeNextBreath)
				{
					m_bFreeze=true;
				}
			}

			if(iCurrentXtimeval<DIAGRAMM_DATAAREA)
				iCurrentXtimeval++;
			else
			{
				iCurrentXtimeval=1;
			}
		}
		LeaveCriticalSection(&csSPIReadInBuffer);

		m_lpfsPressure->num_PlotXYItems=iCountNumItems;

		if(m_bFilledGraph)
		{
			m_lpfsPressure->ChartType=G_FILLEDCHART;
		}
		else
		{
			m_lpfsPressure->ChartType=G_LINECHART;
		}

		bool bRedraw=false;
		bool bVg=drawVGarantyLine();
		if(bVg!=getDrawVolGaranty())
		{
			setDrawVolGaranty(bVg);
			bRedraw=true;
		}

		if(bVg)
		{
			double dbTemp=getModel()->getDATAHANDLER()->GetActiveModePMAXVGPara()/10;//newVG

			if(getValueVolGaranty()!=dbTemp)
			{
				setValueVolGaranty(dbTemp);
				bRedraw=true;
			}
		}

		EnterCriticalSection(&csDiagrammPRESSURE);
		if(m_pDiagrammPressure)
		{
			if(bRedraw)
				m_pDiagrammPressure->SetVolumeGaranty(getDrawVolGaranty(), getValueVolGaranty());

			m_pDiagrammPressure->DoFunction(m_lpfsPressure);
		}
		LeaveCriticalSection(&csDiagrammPRESSURE);
	}
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 26\r\n")));
	return iCurrentXtimeval;
}

// **************************************************************************
// Draws the flow graph
//				Depending on speed and size of SPI data
// **************************************************************************
int CViewDiagramm::drawFlowGraph(int iNumItems)
{
	int iCurrentXtimeval=getCurrentXtimevalGraphs();

	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("lock 27\r\n")));
	if(m_pPlotItems !=NULL && m_lpfsFlow !=NULL)
	{
		int iCountBufPointer=0;
		bool bTrigger=false;
		bool bNewBreath=false;
		PBUFSPI _pBufRead;
		LONG minY=0;
		LONG maxY=0;

		int VECTOR_SIZE=m_iItemsToMerge;
		IntVector FlowNumbers(VECTOR_SIZE) ;
		IntVectorIt start, end, locationMax, locationMin ;

		int iCountNumItems=0;

		if(iNumItems*VECTOR_SIZE>MAXSIZE_SPIREADIN_BUFFER)
		{
			//ERROR
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 27\r\n")));
			return 0;
		}

		EnterCriticalSection(&csSPIReadInBuffer);
		for(int n=0;n<iNumItems;n++)
		{
			// Initialize vector Numbers
			for(int iT=0;iT<VECTOR_SIZE;iT++)
			{
				if(iCountBufPointer<0)
				{
					iCountBufPointer=0;
				}
				_pBufRead=m_pbufSPIReadInBuffer[iCountBufPointer];

				FlowNumbers[iT] = _pBufRead.iValFlow;

				iCountBufPointer++;

				if(_pBufRead.bTrig)
					bTrigger=true;

				if(_pBufRead.bATZ)
				{
					bNewBreath=true;
				}
			}

			start = FlowNumbers.begin() ;   // location of first
			end = FlowNumbers.end() ;       // one past the location

			locationMin = min_element(start, end, less<int>());
			minY=(*locationMin);

			locationMax = max_element(start, end, less<int>());
			maxY=(*locationMax);

			if(minY!=maxY)
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMin);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>G_MAXPOINTS-1)
				{
					//ERROR
					iCountNumItems=G_MAXPOINTS-1;
				}

				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=false;
				iCountNumItems++;
				if(iCountNumItems>G_MAXPOINTS-1)
				{
					//ERROR
					iCountNumItems=G_MAXPOINTS-1;
				}
			}
			else
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>G_MAXPOINTS-1)
				{
					//ERROR
					iCountNumItems=G_MAXPOINTS-1;
				}
			}

			if(iCurrentXtimeval<DIAGRAMM_DATAAREA)
				iCurrentXtimeval++;
			else
			{
				iCurrentXtimeval=1;
			}

			if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
			{
				if(m_bFreezeNextBreath)
				{
					m_bFreeze=true;
				}
			}
			else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
			{
				if(m_bFreezeNextBreath)
				{
					m_bFreeze=true;
				}
			}
			else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
			{
				if(m_bFreezeNextBreath)
				{
					m_bFreeze=true;
				}
			}
			else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
			{
				if(m_bFreezeNextBreath && bNewBreath)
				{
					m_bRefreshLastBreathOfLoops=true;
					m_bFreeze=true;
				}
			}
			else
			{
				if(m_bFreezeNextBreath)
				{
					m_bFreeze=true;
				}
			}

			bTrigger=false;
			bNewBreath=false;
		}
		LeaveCriticalSection(&csSPIReadInBuffer);

		m_lpfsFlow->num_PlotXYItems=iCountNumItems;

		if(m_bFilledGraph)
		{
			m_lpfsFlow->ChartType=G_FILLEDCHART;
		}
		else
		{
			m_lpfsFlow->ChartType=G_LINECHART;
		}

		EnterCriticalSection(&csDiagrammFLOW);
		if(m_pDiagrammFlow)
		{
			m_pDiagrammFlow->DoFunction(m_lpfsFlow);
		}
		LeaveCriticalSection(&csDiagrammFLOW);
	}
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 27\r\n")));

	return iCurrentXtimeval;
}
// **************************************************************************
// Draws the volume graph
//				Depending on speed and size of SPI data
// **************************************************************************
int CViewDiagramm::drawVolumeGraph(int iNumItems)
{
	int iCurrentXtimeval=getCurrentXtimevalGraphs();

	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	//DEBUGMSG(TRUE, (TEXT("lock 28\r\n")));
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	if(m_pPlotItems !=NULL && m_lpfsVolume !=NULL)
	{
		int iCountBufPointer=0;
		bool bTrigger=false;
		bool bNewBreath=false;
		PBUFSPI _pBufRead;
		LONG minY=0;
		LONG maxY=0;

		int VECTOR_SIZE=m_iItemsToMerge;
		IntVector VolumeNumbers(VECTOR_SIZE) ;
		IntVectorIt start, end, locationMax, locationMin ;

		int iCountNumItems=0;

		if(iNumItems*VECTOR_SIZE>MAXSIZE_SPIREADIN_BUFFER)
		{
			//ERROR
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 28\r\n")));
			return 0;
		}

		EnterCriticalSection(&csSPIReadInBuffer);
		for(int n=0;n<iNumItems;n++)
		{
			// Initialize vector Numbers
			for(int iT=0;iT<VECTOR_SIZE;iT++)
			{
				if(iCountBufPointer<0)
				{
					//ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					iCountBufPointer=0;
				}
				_pBufRead=m_pbufSPIReadInBuffer[iCountBufPointer];

				VolumeNumbers[iT] = _pBufRead.iValVolume;

				iCountBufPointer++;

				if(_pBufRead.bTrig)
					bTrigger=true;

				if(_pBufRead.bATZ)
				{
					bNewBreath=true;
				}
			}

			start = VolumeNumbers.begin() ;   // location of first
			end = VolumeNumbers.end() ;       // one past the location

			locationMin = min_element(start, end, less<int>());
			minY=(*locationMin);

			locationMax = max_element(start, end, less<int>());
			maxY=(*locationMax);

			if(minY!=maxY)
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMin);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>G_MAXPOINTS-1)
				{
					//ERROR
					iCountNumItems=G_MAXPOINTS-1;
				}

				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=false;
				iCountNumItems++;
				if(iCountNumItems>G_MAXPOINTS-1)
				{
					//ERROR
					iCountNumItems=G_MAXPOINTS-1;
				}
			}
			else
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>G_MAXPOINTS-1)
				{
					//ERROR
					iCountNumItems=G_MAXPOINTS-1;
				}
			}

			if(iCurrentXtimeval<DIAGRAMM_DATAAREA)
				iCurrentXtimeval++;
			else
			{
				iCurrentXtimeval=1;
			}

			if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
			{
				if(m_bFreezeNextBreath)
				{
					m_bFreeze=true;
				}
			}
			else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
			{
				if(m_bFreezeNextBreath)
				{
					m_bFreeze=true;
				}
			}
			else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
			{
				if(m_bFreezeNextBreath)
				{
					m_bFreeze=true;
				}
			}
			else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
			{
				if(m_bFreezeNextBreath && bNewBreath)
				{
					m_bRefreshLastBreathOfLoops=true;
					m_bFreeze=true;
				}
			}
			else
			{
				if(m_bFreezeNextBreath)
				{
					m_bFreeze=true;
				}
			}

			bTrigger=false;
			bNewBreath=false;
		}
		LeaveCriticalSection(&csSPIReadInBuffer);

		Sleep(0);

		m_lpfsVolume->num_PlotXYItems=iCountNumItems;

		if(m_bFilledGraph)
		{
			m_lpfsVolume->ChartType=G_FILLEDCHART;
		}
		else
		{
			m_lpfsVolume->ChartType=G_LINECHART;
		}

		bool bRedraw=false;
		bool bVl=IsVLimitStateOn();

		if(getDrawVolLimit()!=bVl)
		{
			setDrawVolLimit(bVl);
			bRedraw=true;
		}

		if(bVl)
		{
			double dbTemp=(getModel()->getDATAHANDLER()->GetCurrentVLimitPara())/*/10*/;
			if(getValueVolLimit()!=dbTemp)
			{
				setValueVolLimit(dbTemp);
				bRedraw=true;
			}
		}

		EnterCriticalSection(&csDiagrammVOLUME);
		if(m_pDiagrammVolume)
		{
			if(bRedraw)
				m_pDiagrammVolume->SetVolumeLimit(getDrawVolLimit(), getValueVolLimit());

			m_pDiagrammVolume->DoFunction(m_lpfsVolume);
		}
		LeaveCriticalSection(&csDiagrammVOLUME);
	}
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 28\r\n")));
	return iCurrentXtimeval;
}

// **************************************************************************
// Draws the CO2 graph
//				Depending on speed and size of SPI data
// **************************************************************************
int CViewDiagramm::drawCO2Graph(int iNumItems)
{
	int iCurrentXtimeval=getCurrentXtimevalGraphs();

	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	//DEBUGMSG(TRUE, (TEXT("lock 29\r\n")));
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	if(m_pPlotItems !=NULL && m_lpfsCO2 !=NULL)
	{
		int iCountBufPointer=0;
		bool bTrigger=false;
		bool bNewBreath=false;
		LONG minY=0;
		LONG maxY=0;

		int VECTOR_SIZE=m_iItemsToMerge;
		IntVector CO2Numbers(VECTOR_SIZE) ;
		IntVectorIt start, end, locationMax, locationMin ;

		int iCountNumItems=0;

		if(iNumItems*VECTOR_SIZE>MAXSIZE_SPIREADIN_BUFFER)
		{
			//ERROR
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 29\r\n")));
			return 0;
		}

		EnterCriticalSection(&csSPIReadInBuffer);
		for(int n=0;n<iNumItems;n++)
		{
			// Initialize vector Numbers
			for(int iT=0;iT<VECTOR_SIZE;iT++)
			{
				if(iCountBufPointer<0)
				{
					//ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					iCountBufPointer=0;
				}

				CO2Numbers[iT] = m_pbufSPIReadInBuffer[iCountBufPointer].iValCO2;

				if(m_pbufSPIReadInBuffer[iCountBufPointer].bTrig)
					bTrigger=true;

				if(m_pbufSPIReadInBuffer[iCountBufPointer].bATZ)
				{
					bNewBreath=true;
				}

				iCountBufPointer++;
			}

			start = CO2Numbers.begin() ;   // location of first
			end = CO2Numbers.end() ;       // one past the location

			locationMin = min_element(start, end, less<int>());
			minY=(*locationMin);

			locationMax = max_element(start, end, less<int>());
			maxY=(*locationMax);

			if(minY!=maxY)
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMin);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>G_MAXPOINTS-1)
				{
					//ERROR
					iCountNumItems=G_MAXPOINTS-1;
				}

				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=false;
				iCountNumItems++;
				if(iCountNumItems>G_MAXPOINTS-1)
				{
					//ERROR
					iCountNumItems=G_MAXPOINTS-1;
				}
			}
			else
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>G_MAXPOINTS-1)
				{
					//ERROR
					iCountNumItems=G_MAXPOINTS-1;
				}
			}

			if(iCurrentXtimeval<DIAGRAMM_DATAAREA)
				iCurrentXtimeval++;
			else
			{
				iCurrentXtimeval=1;
			}

			if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
			{
				if(m_bFreezeNextBreath)
				{
					m_bFreeze=true;
				}
			}
			else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
			{
				if(m_bFreezeNextBreath)
				{
					m_bFreeze=true;
				}
			}
			else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
			{
				if(m_bFreezeNextBreath)
				{
					m_bFreeze=true;
				}
			}
			else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
			{
				if(m_bFreezeNextBreath && bNewBreath)
				{
					m_bRefreshLastBreathOfLoops=true;
					m_bFreeze=true;
				}
			}
			else
			{
				if(m_bFreezeNextBreath)
				{
					m_bFreeze=true;
				}
			}

			bTrigger=false;
			bNewBreath=false;
		}
		LeaveCriticalSection(&csSPIReadInBuffer);

		m_lpfsCO2->num_PlotXYItems=iCountNumItems;

		if(m_bFilledGraph)
		{
			m_lpfsCO2->ChartType=G_FILLEDCHART;
		}
		else
		{
			m_lpfsCO2->ChartType=G_LINECHART;
		}

		EnterCriticalSection(&csDiagrammCO2);
		if(m_pDiagrammCO2)
		{
			m_pDiagrammCO2->DoFunction(m_lpfsCO2);
		}
		LeaveCriticalSection(&csDiagrammCO2);
	}
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 29\r\n")));
	return iCurrentXtimeval;
}



// **************************************************************************
// Draws the SPO2 graph
//				Depending on speed and size of SPI data
// **************************************************************************
int CViewDiagramm::drawSPO2Graph(int iNumItems)
{
	int iCurrentXtimeval=getCurrentXtimevalGraphs();

	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	//DEBUGMSG(TRUE, (TEXT("lock 30\r\n")));
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	if(m_pPlotItems !=NULL && m_lpfsSPO2 !=NULL)
	{
		int iCountBufPointer=0;
		bool bNewBreath=false;
		LONG minY=0;
		LONG maxY=0;

		int VECTOR_SIZE=m_iItemsToMerge;
		IntVector SPO2Numbers(VECTOR_SIZE) ;
		IntVectorIt start, end, locationMax, locationMin ;

		int iCountNumItems=0;

		if(iNumItems*VECTOR_SIZE>MAXSIZE_SPIREADIN_BUFFER)
		{
			//ERROR
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 30\r\n")));
			return 0;
		}

		EnterCriticalSection(&csSPIReadInBuffer);
		for(int n=0;n<iNumItems;n++)
		{
			// Initialize vector Numbers
			for(int iT=0;iT<VECTOR_SIZE;iT++)
			{
				if(iCountBufPointer<0)
				{
					//ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					iCountBufPointer=0;
				}

				SPO2Numbers[iT] = m_pbufSPIReadInBuffer[iCountBufPointer].iValSPO2;

				if(m_pbufSPIReadInBuffer[iCountBufPointer].bATZ)
				{
					bNewBreath=true;
				}

				iCountBufPointer++;
			}

			start = SPO2Numbers.begin() ;   // location of first
			end = SPO2Numbers.end() ;       // one past the location

			locationMin = min_element(start, end, less<int>());
			minY=(*locationMin);

			locationMax = max_element(start, end, less<int>());
			maxY=(*locationMax);

			if(minY!=maxY)
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMin);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=false;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>G_MAXPOINTS-1)
				{
					//ERROR
					iCountNumItems=G_MAXPOINTS-1;
				}

				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=false;
				m_pPlotItems[iCountNumItems].bNewATZ=false;
				iCountNumItems++;
				if(iCountNumItems>G_MAXPOINTS-1)
				{
					//ERROR
					iCountNumItems=G_MAXPOINTS-1;
				}
			}
			else
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=false;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>G_MAXPOINTS-1)
				{
					//ERROR
					iCountNumItems=G_MAXPOINTS-1;
				}
			}

			if(iCurrentXtimeval<DIAGRAMM_DATAAREA)
				iCurrentXtimeval++;
			else
			{
				iCurrentXtimeval=1;
			}

			if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_FOTGRAPHS)
			{
				if(m_bFreezeNextBreath)
				{
					m_bFreeze=true;
				}
			}
			else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_SPO2GRAPHS)
			{
				if(m_bFreezeNextBreath)
				{
					m_bFreeze=true;
				}
			}
			else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_CO2GRAPHS)
			{
				if(m_bFreezeNextBreath)
				{
					m_bFreeze=true;
				}
			}
			else if(getModel()->getVIEWHANDLER()->getViewSubState()==VSS_GRAPH_LOOPGRAPHS)
			{
				if(m_bFreezeNextBreath && bNewBreath)
				{
					m_bRefreshLastBreathOfLoops=true;
					m_bFreeze=true;
				}
			}
			else
			{
				if(m_bFreezeNextBreath)
				{
					m_bFreeze=true;
				}
			}

			//bTrigger=false;
			bNewBreath=false;
		}
		LeaveCriticalSection(&csSPIReadInBuffer);

		m_lpfsSPO2->num_PlotXYItems=iCountNumItems;

		m_lpfsSPO2->ChartType=G_LINECHART;

		EnterCriticalSection(&csDiagrammSPO2);
		if(m_pDiagrammSPO2)
		{
			m_pDiagrammSPO2->DoFunction(m_lpfsSPO2);
		}
		LeaveCriticalSection(&csDiagrammSPO2);
	}
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 30\r\n")));
	return iCurrentXtimeval;
}


void CViewDiagramm::DrawLoops(int iNumItems)
{
	bool loopPV=false;
	bool loopVF=false;

	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	//DEBUGMSG(TRUE, (TEXT("lock 31\r\n")));
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);

	EnterCriticalSection(&csDiagrammLOOP);
	if(m_pLoopPressureVolume && m_bLoopPressureVolumeIsActive && m_pPlotItemsPVloop !=NULL && m_lpfsPressureVolume !=NULL)
		loopPV=true;
	LeaveCriticalSection(&csDiagrammLOOP);

	EnterCriticalSection(&csDiagrammLOOP);
	if(m_pLoopVolumeFlow && m_bLoopVolumeFlowIsActive && m_pPlotItemsVFloop !=NULL && m_lpfsVolumeFlow !=NULL)
		loopVF=true;
	LeaveCriticalSection(&csDiagrammLOOP);

	int iCountBufPointer=0;
	bool bTrigger=false;
	bool bNewBreath=false;

	PBUFSPI _pBufRead;

	int iNum=getSizeReadInBufferSPI();

	if(loopPV)
	{
		m_lpfsPressureVolume->num_PlotXYItems=iNum;
		m_lpfsPressureVolume->FuncType=G_PLOTXYLOOPPOINTS;
	}
	if(loopVF)
	{
		m_lpfsVolumeFlow->num_PlotXYItems=iNum;
		m_lpfsVolumeFlow->FuncType=G_PLOTXYLOOPPOINTS;
	}

	int n=0;
	int iCountItems=0;
	eViewSubState subState=getModel()->getVIEWHANDLER()->getViewSubState();

	int iSizeCurrentBreath=getSizeCurrentBreath();
	EnterCriticalSection(&csSPIReadInBuffer);
	for(n=0;n<iNum;n++)
	{
		if(iCountBufPointer<0)
		{
			//ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			iCountBufPointer=0;
		}
		_pBufRead=m_pbufSPIReadInBuffer[iCountBufPointer];

		//NEWLOOP
		if(		subState==VSS_GRAPH_FOTGRAPHS
			||	subState==VSS_GRAPH_SPO2GRAPHS
			||	subState==VSS_GRAPH_CO2GRAPHS)
		{
			if(m_bFreezeNextBreath)
			{
				setRedrawDiagram(REDRAW_GRAPHS);
				m_bFreeze=true;
				break;
			}
		}
		else if(subState==VSS_GRAPH_LOOPGRAPHS)
		{
			if(m_bFreezeNextBreath && _pBufRead.bATZ)
			{
				EnterCriticalSection(&csDiagrammLOOP);
				if(m_pLoopPressureVolume)
					m_pLoopPressureVolume->SetFreeze(true);
				LeaveCriticalSection(&csDiagrammLOOP);

				EnterCriticalSection(&csDiagrammLOOP);
				if(m_pLoopVolumeFlow)
					m_pLoopVolumeFlow->SetFreeze(true);
				LeaveCriticalSection(&csDiagrammLOOP);

				setRedrawDiagram(REDRAW_ALL);
				m_bFreeze=true;
				break;
			}
		}
		else
		{
			if(m_bFreezeNextBreath)
			{
				EnterCriticalSection(&csDiagrammLOOP);
				if(m_pLoopPressureVolume)
					m_pLoopPressureVolume->SetFreeze(true);
				LeaveCriticalSection(&csDiagrammLOOP);

				EnterCriticalSection(&csDiagrammLOOP);
				if(m_pLoopVolumeFlow)
					m_pLoopVolumeFlow->SetFreeze(true);
				LeaveCriticalSection(&csDiagrammLOOP);

				m_bFreeze=true;
				break;
			}
		}
		EnterCriticalSection(&csCurrentBreath);

		if(_pBufRead.bATZ)
		{
			if(iSizeCurrentBreath==0)
			{
				m_pbufCurrentBreath[iSizeCurrentBreath]=_pBufRead;
				iSizeCurrentBreath++;
			}
			else
			{
				m_pbufCurrentBreath[iSizeCurrentBreath]=_pBufRead;
				iSizeCurrentBreath++;

				EnterCriticalSection(&csDiagrammLOOP);
				if(m_pLoopPressureVolume)
					m_pLoopPressureVolume->NextBreath();
				LeaveCriticalSection(&csDiagrammLOOP);

				EnterCriticalSection(&csDiagrammLOOP);
				if(m_pLoopVolumeFlow)
					m_pLoopVolumeFlow->NextBreath();
				LeaveCriticalSection(&csDiagrammLOOP);

				iSizeCurrentBreath=0;
				m_pbufCurrentBreath[iSizeCurrentBreath]=_pBufRead;
				iSizeCurrentBreath++;

				//NEWLOOP
				if(iSizeCurrentBreath>G_MAXPOINTS-1)
					iSizeCurrentBreath=0;
			}
		}
		else if(iSizeCurrentBreath>0)
		{
			m_pbufCurrentBreath[iSizeCurrentBreath]=_pBufRead;
			iSizeCurrentBreath++;
			//NEWLOOP
			if(iSizeCurrentBreath>G_MAXPOINTS-1)
				iSizeCurrentBreath=0;
		}

		LeaveCriticalSection(&csCurrentBreath);

		iCountBufPointer++;

		if(_pBufRead.bTrig)
			bTrigger=true;

		if(_pBufRead.bATZ)
			bNewBreath=true;

		if(loopPV)
		{
			m_pPlotItemsPVloop[iCountItems].pPlotXYItems.x=_pBufRead.iValPressure;
			m_pPlotItemsPVloop[iCountItems].pPlotXYItems.y=_pBufRead.iValVolume;
			m_pPlotItemsPVloop[iCountItems].bTrig=bTrigger;
			m_pPlotItemsPVloop[iCountItems].bNewATZ=bNewBreath;
		}
		if(loopVF)
		{
			m_pPlotItemsVFloop[iCountItems].pPlotXYItems.x=_pBufRead.iValVolume;
			m_pPlotItemsVFloop[iCountItems].pPlotXYItems.y=_pBufRead.iValFlow;
			m_pPlotItemsVFloop[iCountItems].bTrig=bTrigger;
			m_pPlotItemsVFloop[iCountItems].bNewATZ=bNewBreath;
		}

		bTrigger=false;
		bNewBreath=false;
		iCountItems++;
		if(iCountItems>MAXSIZE_RING_BUFFER-1)
		{
			//ERROR
			iCountItems=MAXSIZE_RING_BUFFER-1;
			DEBUGMSG(TRUE, (TEXT("G_MAXLOOPPOINTS 6\r\n")));
		}
	}
	LeaveCriticalSection(&csSPIReadInBuffer);

	setSizeCurrentBreath(iSizeCurrentBreath);

	//if(doThread())
	{
		EnterCriticalSection(&csDiagrammLOOP);
		if(m_pLoopPressureVolume && loopPV)
		{
			m_pLoopPressureVolume->DoFunction(m_lpfsPressureVolume);

		}
		LeaveCriticalSection(&csDiagrammLOOP);
		//Sleep(0);
		EnterCriticalSection(&csDiagrammLOOP);
		if(m_pLoopVolumeFlow && loopVF)
		{
			m_pLoopVolumeFlow->DoFunction(m_lpfsVolumeFlow);
		}
		LeaveCriticalSection(&csDiagrammLOOP);
	}
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 31\r\n")));
}



void CViewDiagramm::drawSavedPressureVolumeLoop()
{
	int iSizeSavedBreath=getModel()->getDATAHANDLER()->getSizeSavedBreath();
	if(iSizeSavedBreath==0 || iSizeSavedBreath>G_MAXPOINTS || false==doThread())
		return;

	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	//DEBUGMSG(TRUE, (TEXT("lock 32\r\n")));
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	if(m_pPlotItemsPVloop !=NULL && m_lpfsPressureVolume !=NULL)
	{
		m_lpfsPressureVolume->num_PlotXYItems=iSizeSavedBreath;
		m_lpfsPressureVolume->FuncType=G_PLOTSAVEDLOOPPOINTS;

		EnterCriticalSection(&getModel()->getDATAHANDLER()->csSavedBreath);
		for(int iSav=0;iSav<iSizeSavedBreath;iSav++)
		{
			if(false==doThread())
				break;

			m_pPlotItemsPVloop[iSav].bTrig=getModel()->getDATAHANDLER()->m_pbufSavedBreath[iSav].bTrig;
			m_pPlotItemsPVloop[iSav].bNewATZ=getModel()->getDATAHANDLER()->m_pbufSavedBreath[iSav].bATZ;
			m_pPlotItemsPVloop[iSav].pPlotXYItems.x=getModel()->getDATAHANDLER()->m_pbufSavedBreath[iSav].iValPressure;
			m_pPlotItemsPVloop[iSav].pPlotXYItems.y=getModel()->getDATAHANDLER()->m_pbufSavedBreath[iSav].iValVolume;
		}
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csSavedBreath);

		int iPrevScheme=0;

		EnterCriticalSection(&csDiagrammLOOP);
		if(m_pLoopPressureVolume && m_bLoopPressureVolumeIsActive && doThread())
		{
			COLORREF crPrevGraphPenColor=m_pLoopPressureVolume->getCurGraphPenColor();
			iPrevScheme=m_pLoopPressureVolume->SetColorScheme(G_FabianHFOSCHEME_SAVEDLOOP,getModel()->getDATAHANDLER()->getGraphColor_SAVEDLOOP());

			if(m_pLoopPressureVolume)
				m_pLoopPressureVolume->DoFunction(m_lpfsPressureVolume);
			if(m_pLoopPressureVolume)
				m_pLoopPressureVolume->SetColorScheme(iPrevScheme, crPrevGraphPenColor);
		}
		LeaveCriticalSection(&csDiagrammLOOP);
		//Sleep(0);
	}
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 32\r\n")));
}
void CViewDiagramm::drawSavedVolumeFlowLoop()
{
	int iSizeSavedBreath=getModel()->getDATAHANDLER()->getSizeSavedBreath();
	if(iSizeSavedBreath==0 || iSizeSavedBreath>G_MAXPOINTS || false==doThread())
		return;

	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	//DEBUGMSG(TRUE, (TEXT("lock 33\r\n")));
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	if(m_pPlotItemsVFloop !=NULL && m_lpfsVolumeFlow !=NULL)
	{
		m_lpfsVolumeFlow->num_PlotXYItems=iSizeSavedBreath;
		m_lpfsVolumeFlow->FuncType=G_PLOTSAVEDLOOPPOINTS;

		EnterCriticalSection(&getModel()->getDATAHANDLER()->csSavedBreath);
		for(int iSav=0;iSav<iSizeSavedBreath;iSav++)
		{
			if(false==doThread())
				break;

			m_pPlotItemsVFloop[iSav].bTrig=getModel()->getDATAHANDLER()->m_pbufSavedBreath[iSav].bTrig;
			m_pPlotItemsVFloop[iSav].bNewATZ=getModel()->getDATAHANDLER()->m_pbufSavedBreath[iSav].bATZ;
			m_pPlotItemsVFloop[iSav].pPlotXYItems.x=getModel()->getDATAHANDLER()->m_pbufSavedBreath[iSav].iValVolume;
			m_pPlotItemsVFloop[iSav].pPlotXYItems.y=getModel()->getDATAHANDLER()->m_pbufSavedBreath[iSav].iValFlow;
		}
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csSavedBreath);

		int iPrevScheme=0;

		EnterCriticalSection(&csDiagrammLOOP);
		if(m_pLoopVolumeFlow && m_bLoopVolumeFlowIsActive && doThread())
		{
			COLORREF crPrevGraphPenColor=m_pLoopPressureVolume->getCurGraphPenColor();
			iPrevScheme=m_pLoopVolumeFlow->SetColorScheme(G_FabianHFOSCHEME_SAVEDLOOP,getModel()->getDATAHANDLER()->getGraphColor_SAVEDLOOP());

			if(m_pLoopVolumeFlow)
				m_pLoopVolumeFlow->DoFunction(m_lpfsVolumeFlow);
			if(m_pLoopVolumeFlow)
				m_pLoopVolumeFlow->SetColorScheme(iPrevScheme,crPrevGraphPenColor);
		}
		LeaveCriticalSection(&csDiagrammLOOP);
		//Sleep(0);
	}
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 33\r\n")));
}



// **************************************************************************
// 
// **************************************************************************
int CViewDiagramm::redrawPressureGraph()
{
	int iCurrentXtimeval=getCurrentXtimevalGraphs();

	EnterCriticalSection(&csDiagrammPRESSURE);
	if(m_pDiagrammPressure)
		m_pDiagrammPressure->SetTriggeredBreath(false);
	LeaveCriticalSection(&csDiagrammPRESSURE);

	PBUFSPI _pBufRead;

	EnterCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
	if(getModel()->getDATAHANDLER()->m_rbufCopy.getCount()==0)
	{
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
		return iCurrentXtimeval;
	}

	int iNumItems=DIAGRAMM_DATAAREA;

	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	//DEBUGMSG(TRUE, (TEXT("lock 34\r\n")));
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	if(m_pPlotItems !=NULL && m_lpfsPressure !=NULL)
	{
		bool bTrigger=false;
		bool bNewBreath=false;
		LONG minY=0;
		LONG maxY=0;

		int VECTOR_SIZE=GetCurrentItemsToMerge();
		IntVector PressureNumbers(VECTOR_SIZE) ;
		IntVectorIt start, end, locationMax, locationMin ;

		int iCountNumItems=0;
		int iCountBufPointer=(iNumItems*VECTOR_SIZE)-1;

		if(iNumItems*VECTOR_SIZE>MAXSIZE_RING_BUFFER)//size copy buffer
		{
			//ERROR
			//rkuNEWFIX
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 34\r\n")));
			LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
			return 0;
		}

		for(int n=0;n<iNumItems;n++)
		{
			for(int iT=0;iT<VECTOR_SIZE;iT++)
			{
				//Sleep(0);

				if(iCountBufPointer<0)
				{
					iCountBufPointer=0;
				}
				_pBufRead=getModel()->getDATAHANDLER()->m_rbufCopy[iCountBufPointer];

				PressureNumbers[iT] = _pBufRead.iValPressure;

				iCountBufPointer--;

				if(_pBufRead.bTrig)
					bTrigger=true;

				if(_pBufRead.bATZ)
				{
					bNewBreath=true;
				}
			}

			if(!doThread())//rkuNEWFIX
			{
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 34\r\n")));
				LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
				return 0;
			}

			start = PressureNumbers.begin() ;   // location of first
			end = PressureNumbers.end() ;       // one past the location

			//Sleep(0);

			locationMin = min_element(start, end, less<int>());
			minY=(*locationMin);

			locationMax = max_element(start, end, less<int>());
			maxY=(*locationMax);

			if(minY!=maxY)
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMin);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>MAXSIZE_RING_BUFFER-1)
				{
					//ERROR
					iCountNumItems=MAXSIZE_RING_BUFFER-1;
				}

				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=false;
				iCountNumItems++;
				if(iCountNumItems>MAXSIZE_RING_BUFFER-1)
				{
					//ERROR
					iCountNumItems=MAXSIZE_RING_BUFFER-1;
				}
			}
			else
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				
				iCountNumItems++;
				if(iCountNumItems>MAXSIZE_RING_BUFFER-1)
				{
					//ERROR
					iCountNumItems=MAXSIZE_RING_BUFFER-1;
				}
			}

			if(iCurrentXtimeval<DIAGRAMM_DATAAREA)
				iCurrentXtimeval++;
			else
			{
				iCurrentXtimeval=1;
			}

			bTrigger=false;
			bNewBreath=false;
		}

		//Sleep(0);

		m_lpfsPressure->num_PlotXYItems=iCountNumItems;

		if(m_bFilledGraph)
		{
			m_lpfsPressure->ChartType=G_FILLEDCHART_REFRESH;
		}
		else
		{
			m_lpfsPressure->ChartType=G_LINECHART_REFRESH;
		}

		setDrawVolGaranty(drawVGarantyLine());
		setValueVolGaranty(getModel()->getDATAHANDLER()->GetActiveModePMAXVGPara()/10);//newVG
			
		EnterCriticalSection(&csDiagrammPRESSURE);
		if(m_pDiagrammPressure && m_bGraphPressureIsActive)
		{
			m_pDiagrammPressure->SetVolumeGaranty(getDrawVolGaranty(), getValueVolGaranty());
			//m_pDiagrammPressure->IgnoreAutofit(true);
			m_pDiagrammPressure->DoFunction(m_lpfsPressure);
			//m_pDiagrammPressure->IgnoreAutofit(false);
		}
		LeaveCriticalSection(&csDiagrammPRESSURE);
	}
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 34\r\n")));
	LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);

	return iCurrentXtimeval;
}
// **************************************************************************
// 
// **************************************************************************
int CViewDiagramm::redrawFlowGraph()
{
	int iCurrentXtimeval=getCurrentXtimevalGraphs();

	EnterCriticalSection(&csDiagrammFLOW);
	if(m_pDiagrammFlow)
		m_pDiagrammFlow->SetTriggeredBreath(false);
	LeaveCriticalSection(&csDiagrammFLOW);
	
	PBUFSPI _pBufRead;

	EnterCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
	if(getModel()->getDATAHANDLER()->m_rbufCopy.getSize()==0)
	{
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
		return iCurrentXtimeval;
	}

	int iNumItems=DIAGRAMM_DATAAREA;

	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	//DEBUGMSG(TRUE, (TEXT("lock 35\r\n")));
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	if(m_pPlotItems !=NULL && m_lpfsFlow !=NULL)
	{
		bool bTrigger=false;
		bool bNewBreath=false;
		LONG minY=0;
		LONG maxY=0;

		int VECTOR_SIZE=GetCurrentItemsToMerge();
		IntVector FlowNumbers(VECTOR_SIZE) ;
		IntVectorIt start, end, locationMax, locationMin ;

		int iCountNumItems=0;
		int iCountBufPointer=(iNumItems*VECTOR_SIZE)-1;

		if(iNumItems*VECTOR_SIZE>MAXSIZE_RING_BUFFER)//size copy buffer
		{
			//ERROR
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 35\r\n")));
			LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);//rkuNEWFIX
			return 0;
		}

		for(int n=0;n<iNumItems;n++)
		{
			// Initialize vector Numbers
			for(int iT=0;iT<VECTOR_SIZE;iT++)
			{
				if(iCountBufPointer<0)
				{
					//ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					iCountBufPointer=0;
				}
				_pBufRead=getModel()->getDATAHANDLER()->m_rbufCopy[iCountBufPointer];

				FlowNumbers[iT] = _pBufRead.iValFlow;

				iCountBufPointer--;

				if(_pBufRead.bTrig)
					bTrigger=true;

				if(_pBufRead.bATZ)
				{
					bNewBreath=true;
				}
			}

			if(!doThread())//rkuNEWFIX
			{
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 35\r\n")));
				LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
				return 0;
			}

			start = FlowNumbers.begin() ;   // location of first
			end = FlowNumbers.end() ;       // one past the location

			//Sleep(0);

			locationMin = min_element(start, end, less<int>());
			minY=(*locationMin);

			locationMax = max_element(start, end, less<int>());
			maxY=(*locationMax);

			if(minY!=maxY)
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMin);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>MAXSIZE_RING_BUFFER-1)
				{
					//ERROR
					iCountNumItems=MAXSIZE_RING_BUFFER-1;
				}

				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=false;
				iCountNumItems++;
				if(iCountNumItems>MAXSIZE_RING_BUFFER-1)
				{
					//ERROR
					iCountNumItems=MAXSIZE_RING_BUFFER-1;
				}
			}
			else
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>MAXSIZE_RING_BUFFER-1)
				{
					//ERROR
					iCountNumItems=MAXSIZE_RING_BUFFER-1;
				}
			}

			if(iCurrentXtimeval<DIAGRAMM_DATAAREA)
				iCurrentXtimeval++;
			else
			{
				iCurrentXtimeval=1;
			}

			bTrigger=false;
			bNewBreath=false;
		}

		//Sleep(0);

		m_lpfsFlow->num_PlotXYItems=iCountNumItems;

		if(m_bFilledGraph)
		{
			m_lpfsFlow->ChartType=G_FILLEDCHART_REFRESH;
		}
		else
		{
			m_lpfsFlow->ChartType=G_LINECHART_REFRESH;
		}

		EnterCriticalSection(&csDiagrammFLOW);
		if(m_pDiagrammFlow && m_bGraphFlowIsActive)
		{
			if(getModel()->getVMODEHANDLER()->activeModeIsCONVTrigger()==true)
			{
				if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_FLOW)
				{
					m_iTrigThreshold=getModel()->getDATAHANDLER()->getAVGMessureDataTrigger();
					m_pDiagrammFlow->SetTriggerThreshold(m_iTrigThreshold);
				}
				else
				{
					m_pDiagrammFlow->SetTriggerThreshold(0);
				}
			}

			//m_pDiagrammFlow->IgnoreAutofit(true);
			m_pDiagrammFlow->DoFunction(m_lpfsFlow);
			//m_pDiagrammFlow->IgnoreAutofit(false);
		}
		LeaveCriticalSection(&csDiagrammFLOW);
	}
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 35\r\n")));
	LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);

	return iCurrentXtimeval;
}
// **************************************************************************
// 
// **************************************************************************
int CViewDiagramm::redrawVolumeGraph()
{
	int iCurrentXtimeval=getCurrentXtimevalGraphs();

	EnterCriticalSection(&csDiagrammVOLUME);
	if(m_pDiagrammVolume)
		m_pDiagrammVolume->SetTriggeredBreath(false);
	LeaveCriticalSection(&csDiagrammVOLUME);

	PBUFSPI _pBufRead;

	EnterCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
	if(getModel()->getDATAHANDLER()->m_rbufCopy.getCount()==0)
	{
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
		return iCurrentXtimeval;
	}

	int iNumItems=DIAGRAMM_DATAAREA;

	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	//DEBUGMSG(TRUE, (TEXT("lock 36\r\n")));
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	if(m_pPlotItems !=NULL && m_lpfsVolume !=NULL)
	{
		bool bTrigger=false;
		bool bNewBreath=false;
		LONG minY=0;
		LONG maxY=0;

		int VECTOR_SIZE=GetCurrentItemsToMerge();
		IntVector VolumeNumbers(VECTOR_SIZE) ;
		IntVectorIt start, end, locationMax, locationMin ;

		int iCountNumItems=0;
		int iCountBufPointer=(iNumItems*VECTOR_SIZE)-1;

		if(iNumItems*VECTOR_SIZE>MAXSIZE_RING_BUFFER)//size copy buffer
		{
			//ERROR
			//rkuNEWFIX
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 36\r\n")));
			LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
			return 0;
		}

		for(int n=0;n<iNumItems;n++)
		{
			// Initialize vector Numbers
			for(int iT=0;iT<VECTOR_SIZE;iT++)
			{
				if(iCountBufPointer<0)
				{
					//ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					iCountBufPointer=0;
				}
				_pBufRead=getModel()->getDATAHANDLER()->m_rbufCopy[iCountBufPointer];

				VolumeNumbers[iT] = _pBufRead.iValVolume;

				iCountBufPointer--;

				if(_pBufRead.bTrig)
					bTrigger=true;

				if(_pBufRead.bATZ)
				{
					bNewBreath=true;
				}
			}

			if(!doThread())//rkuNEWFIX
			{
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 36\r\n")));
				LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
				return 0;
			}

			start = VolumeNumbers.begin() ;   // location of first
			end = VolumeNumbers.end() ;       // one past the location

			//Sleep(0);

			locationMin = min_element(start, end, less<int>());
			minY=(*locationMin);

			locationMax = max_element(start, end, less<int>());
			maxY=(*locationMax);

			if(minY!=maxY)
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMin);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>MAXSIZE_RING_BUFFER-1)
				{
					//ERROR
					iCountNumItems=MAXSIZE_RING_BUFFER-1;
				}

				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=false;
				iCountNumItems++;
				if(iCountNumItems>MAXSIZE_RING_BUFFER-1)
				{
					//ERROR
					iCountNumItems=MAXSIZE_RING_BUFFER-1;
				}

			}
			else
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>MAXSIZE_RING_BUFFER-1)
				{
					//ERROR
					iCountNumItems=MAXSIZE_RING_BUFFER-1;
				}
			}

			if(iCurrentXtimeval<DIAGRAMM_DATAAREA)
				iCurrentXtimeval++;
			else
			{
				iCurrentXtimeval=1;
			}

			bTrigger=false;
			bNewBreath=false;
		}

		//Sleep(0);

		m_lpfsVolume->num_PlotXYItems=iCountNumItems;

		if(m_bFilledGraph)
		{
			m_lpfsVolume->ChartType=G_FILLEDCHART_REFRESH;
		}
		else
		{
			m_lpfsVolume->ChartType=G_LINECHART_REFRESH;
		}

		EnterCriticalSection(&csDiagrammVOLUME);
		if(m_pDiagrammVolume && m_bGraphVolumeIsActive)
		{
			if(getModel()->getVMODEHANDLER()->activeModeIsCONVTrigger()==true)
			{
				if(getModel()->getDATAHANDLER()->getTriggerOptionCONV()==TRIGGER_VOLUME)
				{
					m_iTrigThreshold=getModel()->getDATAHANDLER()->getAVGMessureDataTrigger();
					m_pDiagrammVolume->SetTriggerThreshold(m_iTrigThreshold);
				}
				else
				{
					m_pDiagrammVolume->SetTriggerThreshold(0);
				}
			}
			setDrawVolLimit(IsVLimitStateOn());
			setValueVolLimit((getModel()->getDATAHANDLER()->GetCurrentVLimitPara())/*/10*/);
			m_pDiagrammVolume->SetVolumeLimit(getDrawVolLimit(), getValueVolLimit());
			//m_pDiagrammVolume->IgnoreAutofit(true);
			m_pDiagrammVolume->DoFunction(m_lpfsVolume);
			//m_pDiagrammVolume->IgnoreAutofit(false);
		}
		LeaveCriticalSection(&csDiagrammVOLUME);
	}
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 36\r\n")));
	LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);

	return iCurrentXtimeval;
}

// **************************************************************************
// 
// **************************************************************************
int CViewDiagramm::redrawCO2Graph()
{
	int iCurrentXtimeval=getCurrentXtimevalGraphs();

	EnterCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
	if(getModel()->getDATAHANDLER()->m_rbufCopy.getCount()==0)
	{
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
		return iCurrentXtimeval;
	}

	int iNumItems=DIAGRAMM_DATAAREA;

	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	//DEBUGMSG(TRUE, (TEXT("lock 37\r\n")));
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	if(m_pPlotItems !=NULL && m_lpfsCO2 !=NULL)
	{
		bool bTrigger=false;
		bool bNewBreath=false;
		LONG minY=0;
		LONG maxY=0;

		int VECTOR_SIZE=GetCurrentItemsToMerge();
		IntVector CO2Numbers(VECTOR_SIZE) ;
		IntVectorIt start, end, locationMax, locationMin ;

		int iCountNumItems=0;
		int iCountBufPointer=(iNumItems*VECTOR_SIZE)-1;

		if(iNumItems*VECTOR_SIZE>MAXSIZE_RING_BUFFER)//size copy buffer
		{
			//ERROR
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 37\r\n")));
			LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
			return 0;
		}

		for(int n=0;n<iNumItems;n++)
		{
			// Initialize vector Numbers
			for(int iT=0;iT<VECTOR_SIZE;iT++)
			{
				if(iCountBufPointer<0)
				{
					//ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					iCountBufPointer=0;
				}

				CO2Numbers[iT] = getModel()->getDATAHANDLER()->m_rbufCopy[iCountBufPointer].iValCO2;

				if(getModel()->getDATAHANDLER()->m_rbufCopy[iCountBufPointer].bTrig)
					bTrigger=true;

				if(getModel()->getDATAHANDLER()->m_rbufCopy[iCountBufPointer].bATZ)
				{
					bNewBreath=true;
				}

				iCountBufPointer--;
			}

			if(!doThread())//rkuNEWFIX
			{
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 37\r\n")));
				LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
				return 0;
			}

			start = CO2Numbers.begin() ;   // location of first
			end = CO2Numbers.end() ;       // one past the location

			//Sleep(0);

			locationMin = min_element(start, end, less<int>());
			minY=(*locationMin);

			locationMax = max_element(start, end, less<int>());
			maxY=(*locationMax);

			if(minY!=maxY)
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMin);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>MAXSIZE_RING_BUFFER-1)
				{
					//ERROR
					iCountNumItems=MAXSIZE_RING_BUFFER-1;
				}

				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=false;
				iCountNumItems++;
				if(iCountNumItems>MAXSIZE_RING_BUFFER-1)
				{
					//ERROR
					iCountNumItems=MAXSIZE_RING_BUFFER-1;
				}
			}
			else
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>MAXSIZE_RING_BUFFER-1)
				{
					//ERROR
					iCountNumItems=MAXSIZE_RING_BUFFER-1;
				}
			}

			if(iCurrentXtimeval<DIAGRAMM_DATAAREA)
				iCurrentXtimeval++;
			else
			{
				iCurrentXtimeval=1;
			}

			bTrigger=false;
			bNewBreath=false;
		}

		//Sleep(0);

		m_lpfsCO2->num_PlotXYItems=iCountNumItems;

		if(m_bFilledGraph)
		{
			m_lpfsCO2->ChartType=G_FILLEDCHART_REFRESH;
		}
		else
		{
			m_lpfsCO2->ChartType=G_LINECHART_REFRESH;
		}

		EnterCriticalSection(&csDiagrammCO2);
		if(m_pDiagrammCO2 && m_bGraphCO2IsActive)
		{
			//m_pDiagrammCO2->IgnoreAutofit(true);
			m_pDiagrammCO2->DoFunction(m_lpfsCO2);
			//m_pDiagrammCO2->IgnoreAutofit(false);
			
		}
		LeaveCriticalSection(&csDiagrammCO2);
	}
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 37\r\n")));
	LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);

	return iCurrentXtimeval;
}

// **************************************************************************
// 
// **************************************************************************
//int CViewDiagramm::redrawFOTLoop()
//{
//	return 0;
//}
// **************************************************************************
// 
// **************************************************************************
int CViewDiagramm::redrawSPO2Graph()
{
	int iCurrentXtimeval=getCurrentXtimevalGraphs();

	EnterCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);

	if(getModel()->getDATAHANDLER()->m_rbufCopy.getCount()==0)
	{
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
		return iCurrentXtimeval;
	}

	int iNumItems=DIAGRAMM_DATAAREA;

	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	//DEBUGMSG(TRUE, (TEXT("lock 38\r\n")));
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	if(m_pPlotItems !=NULL && m_lpfsSPO2 !=NULL)
	{
		bool bTrigger=false;
		bool bNewBreath=false;
		LONG minY=0;
		LONG maxY=0;

		int VECTOR_SIZE=GetCurrentItemsToMerge();
		IntVector SPO2Numbers(VECTOR_SIZE) ;
		IntVectorIt start, end, locationMax, locationMin ;

		int iCountNumItems=0;
		int iCountBufPointer=(iNumItems*VECTOR_SIZE)-1;

		if(iNumItems*VECTOR_SIZE>MAXSIZE_RING_BUFFER)//size copy buffer
		{
			//ERROR
			/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
			LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
			//DEBUGMSG(TRUE, (TEXT("unlock 38\r\n")));
			LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
			return 0;
		}

		for(int n=0;n<iNumItems;n++)
		{
			for(int iT=0;iT<VECTOR_SIZE;iT++)
			{
				if(iCountBufPointer<0)
				{
					//ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					iCountBufPointer=0;
				}

				SPO2Numbers[iT] = getModel()->getDATAHANDLER()->m_rbufCopy[iCountBufPointer].iValSPO2;

				if(getModel()->getDATAHANDLER()->m_rbufCopy[iCountBufPointer].bATZ)
				{
					bNewBreath=true;
				}

				iCountBufPointer--;
			}

			if(!doThread())//rkuNEWFIX
			{
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 38\r\n")));
				LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
				return 0;
			}

			start = SPO2Numbers.begin() ;   // location of first
			end = SPO2Numbers.end() ;       // one past the location

			//Sleep(0);

			locationMin = min_element(start, end, less<int>());
			minY=(*locationMin);

			locationMax = max_element(start, end, less<int>());
			maxY=(*locationMax);

			if(minY!=maxY)
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMin);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>MAXSIZE_RING_BUFFER-1)
				{
					//ERROR
					iCountNumItems=MAXSIZE_RING_BUFFER-1;
				}

				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=false;
				iCountNumItems++;
				if(iCountNumItems>MAXSIZE_RING_BUFFER-1)
				{
					//ERROR
					iCountNumItems=MAXSIZE_RING_BUFFER-1;
				}
			}
			else
			{
				m_pPlotItems[iCountNumItems].pPlotXYItems.y=(*locationMax);
				m_pPlotItems[iCountNumItems].pPlotXYItems.x=iCurrentXtimeval;
				m_pPlotItems[iCountNumItems].bTrig=bTrigger;
				m_pPlotItems[iCountNumItems].bNewATZ=bNewBreath;
				iCountNumItems++;
				if(iCountNumItems>MAXSIZE_RING_BUFFER-1)
				{
					//ERROR
					iCountNumItems=MAXSIZE_RING_BUFFER-1;
				}
			}

			if(iCurrentXtimeval<DIAGRAMM_DATAAREA)
				iCurrentXtimeval++;
			else
			{
				iCurrentXtimeval=1;
			}
			bNewBreath=false;
		}

		//Sleep(0);

		m_lpfsSPO2->num_PlotXYItems=iCountNumItems;

		m_lpfsSPO2->ChartType=G_LINECHART_REFRESH;

		EnterCriticalSection(&csDiagrammSPO2);
		if(m_pDiagrammSPO2 && m_bGraphSPO2IsActive)
		{
			//m_pDiagrammSPO2->IgnoreAutofit(true);
			m_pDiagrammSPO2->DoFunction(m_lpfsSPO2);
			//m_pDiagrammSPO2->IgnoreAutofit(false);
		}
		LeaveCriticalSection(&csDiagrammSPO2);
	}
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 38\r\n")));
	LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);

	return iCurrentXtimeval;
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::refreshLastBreathOfPressureVolumeLoop()
{
	int iSizeCurrentBreath=getSizeCurrentBreath();
	if(iSizeCurrentBreath==0)
		return;
	else if(iSizeCurrentBreath>G_MAXPOINTS-1)
	{
		//ERROR
		setSizeCurrentBreath(0);
		return;
	}

	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	//DEBUGMSG(TRUE, (TEXT("lock 39\r\n")));
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	if(m_pPlotItemsPVloop !=NULL && m_lpfsPressureVolume !=NULL)
	{
		EnterCriticalSection(&csCurrentBreath);
		int iNumItems=iSizeCurrentBreath;

		m_lpfsPressureVolume->num_PlotXYItems=iNumItems;
		m_lpfsPressureVolume->FuncType=G_PLOTSAVEDLOOPPOINTS;

		for(int iSav=0;iSav<iSizeCurrentBreath;iSav++)
		{
			if(false==doThread())
			{
				LeaveCriticalSection(&csCurrentBreath);
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 39\r\n")));
				return;//rkuNEWFIX
			}
			m_pPlotItemsPVloop[iSav].bTrig=m_pbufCurrentBreath[iSav].bTrig;
			m_pPlotItemsPVloop[iSav].bNewATZ=m_pbufCurrentBreath[iSav].bATZ;
			m_pPlotItemsPVloop[iSav].pPlotXYItems.x=m_pbufCurrentBreath[iSav].iValPressure;
			m_pPlotItemsPVloop[iSav].pPlotXYItems.y=m_pbufCurrentBreath[iSav].iValVolume;
		}

		LeaveCriticalSection(&csCurrentBreath);

		EnterCriticalSection(&csDiagrammLOOP);
		if(m_pLoopPressureVolume && m_bLoopPressureVolumeIsActive && doThread())
		{
			m_pLoopPressureVolume->DoFunction(m_lpfsPressureVolume);
		}
		LeaveCriticalSection(&csDiagrammLOOP);
	}
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 39\r\n")));
}
void CViewDiagramm::refreshLastBreathOfVolumeFlowLoop()
{
	int iSizeCurrentBreath=getSizeCurrentBreath();
	if(iSizeCurrentBreath==0)
		return;
	else if(iSizeCurrentBreath>G_MAXPOINTS-1)
	{
		//ERROR
		setSizeCurrentBreath(0);
		return;
	}

	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	//DEBUGMSG(TRUE, (TEXT("lock 40\r\n")));
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	if(m_pPlotItemsVFloop !=NULL && m_lpfsVolumeFlow !=NULL)
	{
		EnterCriticalSection(&csCurrentBreath);

		int iNumItems=iSizeCurrentBreath;

		m_lpfsVolumeFlow->num_PlotXYItems=iNumItems;
		m_lpfsVolumeFlow->FuncType=G_PLOTSAVEDLOOPPOINTS;

		for(int iSav=0;iSav<iSizeCurrentBreath;iSav++)
		{
			if(false==doThread())
			{
				LeaveCriticalSection(&csCurrentBreath);
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 40\r\n")));
				return;//rkuNEWFIX
			}

			m_pPlotItemsVFloop[iSav].bTrig=m_pbufCurrentBreath[iSav].bTrig;
			m_pPlotItemsVFloop[iSav].bNewATZ=m_pbufCurrentBreath[iSav].bATZ;
			m_pPlotItemsVFloop[iSav].pPlotXYItems.x=m_pbufCurrentBreath[iSav].iValVolume;
			m_pPlotItemsVFloop[iSav].pPlotXYItems.y=m_pbufCurrentBreath[iSav].iValFlow;
		}

		LeaveCriticalSection(&csCurrentBreath);

		EnterCriticalSection(&csDiagrammLOOP);
		if(m_pLoopVolumeFlow && m_bLoopVolumeFlowIsActive && doThread())
		{
			m_pLoopVolumeFlow->DoFunction(m_lpfsVolumeFlow);
		}
		LeaveCriticalSection(&csDiagrammLOOP);
	}
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 40\r\n")));
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::redrawPressureVolumeLoop()
{
	if(m_pLoopPressureVolume==NULL || false==doThread())
		return;

	EnterCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
	int iSize=getModel()->getDATAHANDLER()->m_rbufCopy.getCount();
	if(iSize==0)
	{
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
		return;
	}
	else if(iSize>MAXSIZE_RING_BUFFER)
	{
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
		return;
	}

	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	//DEBUGMSG(TRUE, (TEXT("lock 50\r\n")));
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	if(m_pPlotItemsPVloop !=NULL && m_lpfsPressureVolume !=NULL)
	{
		UINT iBreathCnt=0;
		UINT iBufCnt=1;

		if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
		{
			if(getModel()->getDATAHANDLER()->m_rbufCopy[iBufCnt].bATZ)
				iBreathCnt=1;
		}
		else
		{
			while(iBreathCnt<3 && iBufCnt<iSize)
			{
				if(getModel()->getDATAHANDLER()->m_rbufCopy[iBufCnt].bATZ)
				{
					iBreathCnt++;
					//Sleep(0);
				}

				iBufCnt++;

				if(iBufCnt>((iBreathCnt+1)*G_MAXPOINTS))//per ATZ max G_MAXLOOPPOINTS data points
				{
					/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
					LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
					//DEBUGMSG(TRUE, (TEXT("unlock 50\r\n")));
					LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
					theApp.getLog()->WriteLine(_T("#G_MAXLOOPPOINTS 1"));
					return;
				}
			}
			iBufCnt--;
		}

		if(iBufCnt>18000)//3*G_MAXPOINTS
		{
			//ERROR
			iBufCnt=18000;
		}

		if(iBreathCnt>3)
		{
			theApp.getLog()->WriteLine(_T("#G_MAXLOOPPOINTS 2"));
		}

		m_lpfsPressureVolume->FuncType=G_PLOTSAVEDLOOPPOINTS;

		int iNumItems=0;
		bool bNextBreath=false;
		int iSizeSavedBreath=getModel()->getDATAHANDLER()->getSizeSavedBreath();

		while(iBreathCnt>0)
		{
			iNumItems=0;
			bNextBreath=false;

			if(iBreathCnt==1 && iSizeSavedBreath>0)
				drawSavedPressureVolumeLoop();

			while(iBufCnt>0 && !bNextBreath)
			{
				if(getModel()->getDATAHANDLER()->m_rbufCopy[iBufCnt].bATZ)
				{
					bNextBreath=true;
				}
				else//if(iNextBreath<2)
				{
					m_pPlotItemsPVloop[iNumItems].bTrig=getModel()->getDATAHANDLER()->m_rbufCopy[iBufCnt].bTrig;
					m_pPlotItemsPVloop[iNumItems].bNewATZ=getModel()->getDATAHANDLER()->m_rbufCopy[iBufCnt].bATZ;
					m_pPlotItemsPVloop[iNumItems].pPlotXYItems.x=getModel()->getDATAHANDLER()->m_rbufCopy[iBufCnt].iValPressure;
					m_pPlotItemsPVloop[iNumItems].pPlotXYItems.y=getModel()->getDATAHANDLER()->m_rbufCopy[iBufCnt].iValVolume;

					iBufCnt--;

					iNumItems++;

					if(iNumItems>G_MAXPOINTS-1)
					{
						//ERROR
						/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
						LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
						//DEBUGMSG(TRUE, (TEXT("unlock 50\r\n")));
						LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
						theApp.getLog()->WriteLine(_T("#G_MAXLOOPPOINTS 3"));
						return;
					}
				}
			}
			//Sleep(0);

			m_lpfsPressureVolume->num_PlotXYItems=iNumItems;

			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopPressureVolume && m_bLoopPressureVolumeIsActive && doThread())
			{
				if(iBreathCnt==3)
				{
					m_pLoopPressureVolume->SetColorScheme(G_FabianHFOSCHEME_PREVLOOP2,RGB(180,180,180));
				}
				else if(iBreathCnt==2)
				{
					m_pLoopPressureVolume->SetColorScheme(G_FabianHFOSCHEME_PREVLOOP1,RGB(140,140,140));
				}
				else //if(iBreathCnt==1) -> actual ATZ
				{
					m_pLoopPressureVolume->SetColorScheme(G_FabianHFOSCHEME_LOOP,getModel()->getDATAHANDLER()->getGraphColor_LOOP());
				}

				m_pLoopPressureVolume->DoFunction(m_lpfsPressureVolume);
			}
			LeaveCriticalSection(&csDiagrammLOOP);

			//Sleep(0);
			iBreathCnt--;
		}
	}
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 50\r\n")));
	LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::redrawVolumeFlowLoop()
{
	if(m_pLoopVolumeFlow==NULL || false==doThread())
		return;

	EnterCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
	int iSize=getModel()->getDATAHANDLER()->m_rbufCopy.getCount();
	if(iSize==0)
	{
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
		return;
	}
	else if(iSize>MAXSIZE_RING_BUFFER)
	{
		LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
		return;
	}

	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	//DEBUGMSG(TRUE, (TEXT("lock 51\r\n")));
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	if(m_pPlotItemsVFloop !=NULL && m_lpfsVolumeFlow !=NULL)
	{
		UINT iBreathCnt=0;
		UINT iBufCnt=1;

		if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
		{
			if(getModel()->getDATAHANDLER()->m_rbufCopy[iBufCnt].bATZ)
				iBreathCnt++;
		}
		else
		{
			while(iBreathCnt<3 && iBufCnt<iSize)
			{
				if(getModel()->getDATAHANDLER()->m_rbufCopy[iBufCnt].bATZ)
				{
					iBreathCnt++;
					//Sleep(0);
				}

				iBufCnt++;

				if(iBufCnt>((iBreathCnt+1)*G_MAXPOINTS))//per ATZ max G_MAXLOOPPOINTS data points
				{
					/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
					LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
					//DEBUGMSG(TRUE, (TEXT("unlock 51\r\n")));
					LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
					theApp.getLog()->WriteLine(_T("#G_MAXLOOPPOINTS 1"));
					return;
				}
			}
			iBufCnt--;
		}

		if(iBufCnt>18000)//3*G_MAXPOINTS
		{
			//ERROR
			iBufCnt=18000;
		}

		if(iBreathCnt>3)
		{
			theApp.getLog()->WriteLine(_T("#G_MAXLOOPPOINTS 2"));
		}

		m_lpfsVolumeFlow->FuncType=G_PLOTSAVEDLOOPPOINTS;

		int iNumItems=0;
		bool bNextBreath=false;
		int iSizeSavedBreath=getModel()->getDATAHANDLER()->getSizeSavedBreath();

		while(iBreathCnt>0)
		{
			iNumItems=0;
			bNextBreath=false;

			if(iBreathCnt==1 && iSizeSavedBreath>0)
				drawSavedVolumeFlowLoop();

			while(iBufCnt>0 && !bNextBreath)
			{
				if(getModel()->getDATAHANDLER()->m_rbufCopy[iBufCnt].bATZ)
				{
					bNextBreath=true;
				}
				else//if(iNextBreath<2)
				{
					m_pPlotItemsVFloop[iNumItems].bTrig=getModel()->getDATAHANDLER()->m_rbufCopy[iBufCnt].bTrig;
					m_pPlotItemsVFloop[iNumItems].bNewATZ=getModel()->getDATAHANDLER()->m_rbufCopy[iBufCnt].bATZ;
					m_pPlotItemsVFloop[iNumItems].pPlotXYItems.x=getModel()->getDATAHANDLER()->m_rbufCopy[iBufCnt].iValVolume;
					m_pPlotItemsVFloop[iNumItems].pPlotXYItems.y=getModel()->getDATAHANDLER()->m_rbufCopy[iBufCnt].iValFlow;

					iBufCnt--;

					iNumItems++;

					if(iNumItems>G_MAXPOINTS-1)
					{
						//ERROR
						//iNumItems=G_MAXLOOPPOINTS-1;
						/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
						LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
						//DEBUGMSG(TRUE, (TEXT("unlock 51\r\n")));
						LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
						theApp.getLog()->WriteLine(_T("#G_MAXLOOPPOINTS 3"));
						return;
					}
				}
			}
			//Sleep(0);

			m_lpfsVolumeFlow->num_PlotXYItems=iNumItems;

			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopVolumeFlow && m_bLoopVolumeFlowIsActive && doThread())
			{
				if(iBreathCnt==3)
				{
					m_pLoopVolumeFlow->SetColorScheme(G_FabianHFOSCHEME_PREVLOOP2,RGB(180,180,180));
				}
				else if(iBreathCnt==2)
				{
					m_pLoopVolumeFlow->SetColorScheme(G_FabianHFOSCHEME_PREVLOOP1,RGB(140,140,140));
				}
				else //if(iBreathCnt==1) -> actual ATZ
				{
					m_pLoopVolumeFlow->SetColorScheme(G_FabianHFOSCHEME_LOOP,getModel()->getDATAHANDLER()->getGraphColor_LOOP());
				}

				m_pLoopVolumeFlow->DoFunction(m_lpfsVolumeFlow);
			}
			LeaveCriticalSection(&csDiagrammLOOP);

			//Sleep(0);
			iBreathCnt--;
		}
	}
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 51\r\n")));
	LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
}

void CViewDiagramm::drawFOTsteps()
{
	if(m_pLoopFOT==NULL || false==doThread())
		return;

	int iSize=0;

	if(getModel()->getFOTThread())
		iSize=getModel()->getFOTThread()->getBufSizeFOTdisplay();
	else
		return;

	//DEBUGMSG(TRUE, (TEXT("CViewDiagramm::drawFOTsteps getBufSizeFOTdisplay %d\r\n"),iSize));

	if(iSize==0)
	{
		//LeaveCriticalSection(&getModel()->getDATAHANDLER()->csCopyDataBuffer);
		return;
	}
	
	/** \brief The enter critical section csLPFS_FUNCTIONSTRUCT. */
	//DEBUGMSG(TRUE, (TEXT("lock 52\r\n")));
	EnterCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	if(m_pPlotItemsFOTloop !=NULL && m_lpfsFOT !=NULL)
	{
		m_lpfsFOT->FuncType=G_PLOTSAVEDLOOPPOINTS;

		int iNumItems=0;
		
		for (iNumItems; iNumItems<iSize; iNumItems++)
		{
			m_pPlotItemsFOTloop[iNumItems].bTrig=false;
			m_pPlotItemsFOTloop[iNumItems].bNewATZ=false;
			if(getModel()->getFOTThread())
			{
				m_pPlotItemsFOTloop[iNumItems].pPlotXYItems.x=getModel()->getFOTThread()->m_pbufFOTdisplay[iNumItems].iXValPmean;
				m_pPlotItemsFOTloop[iNumItems].pPlotXYItems.y=getModel()->getFOTThread()->m_pbufFOTdisplay[iNumItems].iYValXRS;
			}

			if(iNumItems>=MAXSIZE_FOT_STEPS)
			{
				//ERROR
				/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
				LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
				//DEBUGMSG(TRUE, (TEXT("unlock 52\r\n")));
				theApp.getLog()->WriteLine(_T("#MAXSIZE_FOT_STEPS"));
				return;
			}
		}

		//Sleep(0);

		m_lpfsFOT->num_PlotXYItems=iNumItems;

		EnterCriticalSection(&csDrawDataFOT);
		if(m_pLoopFOT && m_bLoopFOTIsActive && doThread())
		{
			m_pLoopFOT->DoFunction(m_lpfsFOT);
		}
		LeaveCriticalSection(&csDrawDataFOT);

		//Sleep(0);
	}
	/** \brief The leave critical section csLPFS_FUNCTIONSTRUCT. */
	LeaveCriticalSection(&csLPFS_FUNCTIONSTRUCT);
	//DEBUGMSG(TRUE, (TEXT("unlock 52\r\n")));
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::setCurrentXtimevalGraphs(int iVal)
{
	EnterCriticalSection(&csXValue);
	m_iCurrentXvalGraphs=iVal;
	LeaveCriticalSection(&csXValue);
}
// **************************************************************************
// 
// **************************************************************************
int CViewDiagramm::getCurrentXtimevalGraphs()
{
	EnterCriticalSection(&csXValue);
	int iTemp=m_iCurrentXvalGraphs;
	LeaveCriticalSection(&csXValue);
	return iTemp;
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::resetCurrentXtimevalGraphs()
{
	//DEBUGMSG(TRUE, (TEXT("resetCurrentXtimevalGraphs\r\n")));
	EnterCriticalSection(&csXValue);
	m_iCurrentXvalGraphs=1;
	LeaveCriticalSection(&csXValue);
	
}

// **************************************************************************
// Gets the speed of the graph
//				Output:												
//				UINT m_iGraphSpeed		
//				possible Values:
//				G_SPEED_500MS
//				G_SPEED_1SEC
//				G_SPEED_2750MS
//				G_SPEED_5700MS
//				G_SPEED_10SEC
//				G_SPEED_20SEC
//				G_SPEED_40SEC
//				G_SPEED_60SEC
// **************************************************************************
UINT CViewDiagramm::getGraphSpeed()
{
	EnterCriticalSection(&csSpeed);
	UINT speed=m_iGraphSpeedGraph;
	LeaveCriticalSection(&csSpeed);

	return speed;
}

// **************************************************************************
// Sets the speed of the graph
//				Input:												
//				UINT iSpeed		
//				possible Values:
//				G_SPEED_500MS
//				G_SPEED_1SEC
//				G_SPEED_2750MS
//				G_SPEED_5700MS
//				G_SPEED_10SEC
//				G_SPEED_30SEC
//				G_SPEED_60SEC
// **************************************************************************
void CViewDiagramm::setGraphSpeed(UINT speed)
{
	EnterCriticalSection(&csSpeed);
	m_iGraphSpeedGraph=speed;
	LeaveCriticalSection(&csSpeed);
	getModel()->getCONFIG()->GraphSetActualSpeedGraph(speed);
	SetXAxisTimeTickSpaceOfDiagramms();
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::SetXAxisTimeTickSpaceOfDiagramms()
{
	int iXTickSpace=GetXAxisTimeTickSpace();

	EnterCriticalSection(&csDiagrammSPO2);
	if(m_pDiagrammSPO2 && m_bGraphSPO2IsActive)
	{
		m_pDiagrammSPO2->SetXAxisTickSpace(iXTickSpace);
	}
	LeaveCriticalSection(&csDiagrammSPO2);

	EnterCriticalSection(&csDiagrammCO2);
	if(m_pDiagrammCO2 && m_bGraphCO2IsActive)
	{
		m_pDiagrammCO2->SetXAxisTickSpace(iXTickSpace);
	}
	LeaveCriticalSection(&csDiagrammCO2);

	EnterCriticalSection(&csDiagrammPRESSURE);
	if(m_pDiagrammPressure && m_bGraphPressureIsActive)
	{
		m_pDiagrammPressure->SetXAxisTickSpace(iXTickSpace);
	}
	LeaveCriticalSection(&csDiagrammPRESSURE);

	EnterCriticalSection(&csDiagrammVOLUME);
	if(m_pDiagrammVolume && m_bGraphVolumeIsActive)
	{
		m_pDiagrammVolume->SetXAxisTickSpace(iXTickSpace);
	}
	LeaveCriticalSection(&csDiagrammVOLUME);

	EnterCriticalSection(&csDiagrammFLOW);
	if(m_pDiagrammFlow && m_bGraphFlowIsActive)
	{
		m_pDiagrammFlow->SetXAxisTickSpace(iXTickSpace);
	}
	LeaveCriticalSection(&csDiagrammFLOW);
}

// **************************************************************************
// 
// **************************************************************************
int CViewDiagramm::GetXAxisTimeTickSpace()
{
	int iXTickSpace=1000;

	switch(getGraphSpeed())
	{
	case G_SPEED_505MS:
		{
			iXTickSpace=100;
		}
		break;
	case G_SPEED_1000MS:
		{
			iXTickSpace=200;
		}
		break;
	case G_SPEED_2487MS:	
		{
			iXTickSpace=500;
		}
		break;
	case G_SPEED_4975MS:
		{
			iXTickSpace=1000;
		}
		break;
	case G_SPEED_9950MS:
		{
			iXTickSpace=1000;
		}
		break;
	case G_SPEED_19900MS:
		{
			iXTickSpace=2000;
		}
		break;
	case G_SPEED_29850MS:
		{
			iXTickSpace=5000;
		}
		break;
	case G_SPEED_39800MS:
		{
			iXTickSpace=5000;
		}
		break;
	case G_SPEED_49750MS:
		{
			iXTickSpace=5000;
		}
		break;
	case G_SPEED_99500MS:
		{
			iXTickSpace=10000;
		}
		break;
	case G_SPEED_199000MS:
		{
			iXTickSpace=20000;
		}
		break;
	case G_SPEED_398000MS:
		{
			iXTickSpace=50000;
		}
		break;
	case G_SPEED_796000MS:
		{
			iXTickSpace=100000;
		}
		break;
	default:
		{
		}
		break;
	}
	return iXTickSpace;
}


// **************************************************************************
// Sets the next lower speed of the graph, minimum is 60 sec
//				for the whole dataarea, m_iGraphSpeed == G_SPEED_60SEC (==7)
// **************************************************************************
void CViewDiagramm::DecreaseGraphSpeed()
{
	if(!m_bFreeze)
	{
		m_bDecreaseGraphSpeed=true;
	}
	else
	{
		UINT iTemp = getGraphSpeed();
		if(iTemp<G_SPEED_49750MS)
		{
			iTemp++;
			setGraphSpeed(iTemp);
			UpdateTimeAxis(m_bFreeze);
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}

		DrawCursor(m_iCurFocusedWnd);

		if(m_bGraphSelected && getModel()->getCONFIG()->GraphIsAutoScale()==false)
		{
			PostMessage(WM_SET_GRAPHTIMER);
		}
	}
}
// **************************************************************************
// Sets the next higher speed of the graph, maximum is 500 msec
//				for the whole dataarea, m_iGraphSpeed == G_SPEED_500MS (==0)	
// **************************************************************************
void CViewDiagramm::IncreaseGraphSpeed()
{
	if(!m_bFreeze)
	{
		m_bIncreaseGraphSpeed=true;
	}
	else
	{
		UINT iTemp = getGraphSpeed();
		if(iTemp>G_SPEED_505MS)
		{
			iTemp--;
			setGraphSpeed(iTemp);
			UpdateTimeAxis(m_bFreeze);
		}
		else
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_SCALE_LIMIT);
		}
		DrawCursor(m_iCurFocusedWnd);

		if(m_bGraphSelected && getModel()->getCONFIG()->GraphIsAutoScale()==false)
		{
			PostMessage(WM_SET_GRAPHTIMER);
		}
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::UpdateTimeAxis(bool bRedraw)
{
	int iXmin=0;
	int iXmax=GetTimeAxisMaxScale();

	EnterCriticalSection(&csDiagrammSPO2);
	if(m_pDiagrammSPO2 && m_bGraphSPO2IsActive)
	{
		m_pDiagrammSPO2->SetXAxisScale(iXmin, iXmax,true,false);
		//Sleep(0);
	}
	LeaveCriticalSection(&csDiagrammSPO2);

	EnterCriticalSection(&csDiagrammCO2);
	if(m_pDiagrammCO2 && m_bGraphCO2IsActive)
	{
		m_pDiagrammCO2->SetXAxisScale(iXmin, iXmax,true,false);
		//Sleep(0);
	}
	LeaveCriticalSection(&csDiagrammCO2);

	EnterCriticalSection(&csDiagrammPRESSURE);
	if(m_pDiagrammPressure && m_bGraphPressureIsActive)
	{
		m_pDiagrammPressure->SetXAxisScale(iXmin, iXmax,true,false);
		//Sleep(0);
	}
	LeaveCriticalSection(&csDiagrammPRESSURE);

	EnterCriticalSection(&csDiagrammVOLUME);
	if(m_pDiagrammVolume && m_bGraphVolumeIsActive)
	{
		m_pDiagrammVolume->SetXAxisScale(iXmin, iXmax,true,false);
		//Sleep(0);
	}
	LeaveCriticalSection(&csDiagrammVOLUME);

	EnterCriticalSection(&csDiagrammFLOW);
	if(m_pDiagrammFlow && m_bGraphFlowIsActive)
	{
		m_pDiagrammFlow->SetXAxisScale(iXmin, iXmax,true,false);
		//Sleep(0);
	}
	LeaveCriticalSection(&csDiagrammFLOW);

	if(bRedraw)
	{
		setRedrawDiagram(REDRAW_GRAPHS);
	}
}

// **************************************************************************
// 
// **************************************************************************
int CViewDiagramm::GetTimeAxisMaxScale()
{
	int iXmax=0;

	switch(getGraphSpeed())
	{
	case G_SPEED_505MS:
		{
			iXmax=550;
		}
		break;
	case G_SPEED_1000MS:
		{
			iXmax=1070;
		}
		break;
	case G_SPEED_2487MS:	
		{
			iXmax=2640;
		}
		break;
	case G_SPEED_4975MS:
		{
			iXmax=5285;
		}
		break;
	case G_SPEED_9950MS:
		{
			//iXmax=10560;
			iXmax=10600;
		}
		break;
	case G_SPEED_19900MS:
		{
			iXmax=21170;
		}
		break;
	case G_SPEED_29850MS:
		{
			iXmax=31750;
		}
		break;
	case G_SPEED_39800MS:
		{
			iXmax=42380;
		}
		break;
	case G_SPEED_49750MS:
		{
			iXmax=52930;
		}
		break;
	case G_SPEED_99500MS:
		{
			iXmax=105860;
		}
		break;
	case G_SPEED_199000MS:
		{
			iXmax=211720;
		}
		break;
	case G_SPEED_398000MS:
		{
			iXmax=423440;
		}
		break;
	case G_SPEED_796000MS:
		{
			iXmax=846880;
		}
		break;
	default:
		{
		}
		break;
	}

	return iXmax;
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::DrawCursor(int iDiagramm)
{
	m_bGraphSelected=false;
	
	switch(iDiagramm)
	{
	case IDC_BTN_SELGRAPH_1:
		{
			EnterCriticalSection(&csGraphButton);
			if(m_pcGraph1)
				m_pcGraph1->SetFocus();
			LeaveCriticalSection(&csGraphButton);
			
			m_bGraphSelected=false;
		}
		break;
	case IDC_LINEDIAGRAM:
		{
			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopPressureVolume)
				m_pLoopPressureVolume->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammLOOP);

			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopVolumeFlow)
				m_pLoopVolumeFlow->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammLOOP);

			EnterCriticalSection(&csDiagrammCO2);
			if(m_pDiagrammCO2)
				m_pDiagrammCO2->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammCO2);

			EnterCriticalSection(&csDiagrammSPO2);
			if(m_pDiagrammSPO2)
				m_pDiagrammSPO2->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammSPO2);

			EnterCriticalSection(&csDiagrammPRESSURE);
			if(m_pDiagrammPressure)
			{
				if(m_iCurFocusedWnd==IDC_LINEDIAGRAM_PRESSURE)
				{
					iDiagramm=0;
					m_pDiagrammPressure->DrawCursor(false);
					m_bGraphSelected=false;
				}
				else
				{
					iDiagramm=IDC_LINEDIAGRAM_PRESSURE;
					m_pDiagrammPressure->DrawCursor(true);
					m_bGraphSelected=true;
				}
			}
			LeaveCriticalSection(&csDiagrammPRESSURE);

			EnterCriticalSection(&csDiagrammFLOW);
			if(m_pDiagrammFlow)
			{
				if(m_iCurFocusedWnd==IDC_LINEDIAGRAM_FLOW)
				{
					iDiagramm=0;
					m_pDiagrammFlow->DrawCursor(false);
					m_bGraphSelected=false;
				}
				else
				{
					iDiagramm=IDC_LINEDIAGRAM_FLOW;
					m_pDiagrammFlow->DrawCursor(true);
					m_bGraphSelected=true;
				}
			}
			LeaveCriticalSection(&csDiagrammFLOW);

			EnterCriticalSection(&csDiagrammVOLUME);
			if(m_pDiagrammVolume)
			{
				if(m_iCurFocusedWnd==IDC_LINEDIAGRAM_VOLUME)
				{
					iDiagramm=0;
					m_pDiagrammVolume->DrawCursor(false);
					m_bGraphSelected=false;
				}
				else
				{
					iDiagramm=IDC_LINEDIAGRAM_VOLUME;
					m_pDiagrammVolume->DrawCursor(true);
					m_bGraphSelected=true;
				}
			}
			LeaveCriticalSection(&csDiagrammVOLUME);
		}
		break;
	case IDC_LINEDIAGRAM_SPO2:
		{
			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopPressureVolume)
				m_pLoopPressureVolume->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammLOOP);

			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopVolumeFlow)
				m_pLoopVolumeFlow->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammLOOP);
			
			EnterCriticalSection(&csDiagrammFLOW);
			if(m_pDiagrammFlow)
				m_pDiagrammFlow->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammFLOW);
			
			EnterCriticalSection(&csDiagrammVOLUME);
			if(m_pDiagrammVolume)
				m_pDiagrammVolume->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammVOLUME);
			
			EnterCriticalSection(&csDiagrammPRESSURE);
			if(m_pDiagrammPressure)
				m_pDiagrammPressure->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammPRESSURE);
			
			EnterCriticalSection(&csDiagrammSPO2);
			if(m_pDiagrammSPO2)
			{
				m_pDiagrammSPO2->DrawCursor(true);
				m_bGraphSelected=true;
			}
			LeaveCriticalSection(&csDiagrammSPO2);
		}
		break;
	case IDC_LINEDIAGRAM_CO2:
		{
			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopPressureVolume)
				m_pLoopPressureVolume->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammLOOP);

			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopVolumeFlow)
				m_pLoopVolumeFlow->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammLOOP);
			
			EnterCriticalSection(&csDiagrammFLOW);
			if(m_pDiagrammFlow)
				m_pDiagrammFlow->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammFLOW);
			
			EnterCriticalSection(&csDiagrammVOLUME);
			if(m_pDiagrammVolume)
				m_pDiagrammVolume->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammVOLUME);
			
			EnterCriticalSection(&csDiagrammPRESSURE);
			if(m_pDiagrammPressure)
				m_pDiagrammPressure->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammPRESSURE);
			
			EnterCriticalSection(&csDiagrammCO2);
			if(m_pDiagrammCO2)
			{
				m_pDiagrammCO2->DrawCursor(true);
				m_bGraphSelected=true;
			}
			LeaveCriticalSection(&csDiagrammCO2);
		}
		break;
	case IDC_LINEDIAGRAM_PRESSURE:
		{
			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopPressureVolume)
				m_pLoopPressureVolume->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammLOOP);

			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopVolumeFlow)
				m_pLoopVolumeFlow->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammLOOP);
			
			EnterCriticalSection(&csDiagrammFLOW);
			if(m_pDiagrammFlow)
				m_pDiagrammFlow->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammFLOW);
			
			EnterCriticalSection(&csDiagrammVOLUME);
			if(m_pDiagrammVolume)
				m_pDiagrammVolume->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammVOLUME);
			
			EnterCriticalSection(&csDiagrammPRESSURE);
			if(m_pDiagrammPressure)
			{
				m_pDiagrammPressure->DrawCursor(true);
				m_bGraphSelected=true;
			}
			LeaveCriticalSection(&csDiagrammPRESSURE);
		}
		break;
	case IDC_LINEDIAGRAM_FLOW:
		{
			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopPressureVolume)
				m_pLoopPressureVolume->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammLOOP);

			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopVolumeFlow)
				m_pLoopVolumeFlow->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammLOOP);
			
			EnterCriticalSection(&csDiagrammVOLUME);
			if(m_pDiagrammVolume)
				m_pDiagrammVolume->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammVOLUME);
			
			EnterCriticalSection(&csDiagrammPRESSURE);
			if(m_pDiagrammPressure)
				m_pDiagrammPressure->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammPRESSURE);
			
			EnterCriticalSection(&csDiagrammFLOW);
			if(m_pDiagrammFlow)
			{
				m_pDiagrammFlow->DrawCursor(true);
				m_bGraphSelected=true;
			}
			LeaveCriticalSection(&csDiagrammFLOW);
		}
		break;
	case IDC_LINEDIAGRAM_VOLUME:
		{
			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopPressureVolume)
				m_pLoopPressureVolume->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammLOOP);

			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopVolumeFlow)
				m_pLoopVolumeFlow->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammLOOP);
			
			EnterCriticalSection(&csDiagrammFLOW);
			if(m_pDiagrammFlow)
				m_pDiagrammFlow->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammFLOW);
			
			EnterCriticalSection(&csDiagrammPRESSURE);
			if(m_pDiagrammPressure)
				m_pDiagrammPressure->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammPRESSURE);
			
			EnterCriticalSection(&csDiagrammVOLUME);
			if(m_pDiagrammVolume)
			{
				m_pDiagrammVolume->DrawCursor(true);
				m_bGraphSelected=true;
			}
			LeaveCriticalSection(&csDiagrammVOLUME);
		}
		break;
	case IDC_LOOPDIAGRAM_PRESSUREVOLUME:
		{
			EnterCriticalSection(&csDiagrammFLOW);
			if(m_pDiagrammFlow)
				m_pDiagrammFlow->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammFLOW);
			
			EnterCriticalSection(&csDiagrammPRESSURE);
			if(m_pDiagrammPressure)
				m_pDiagrammPressure->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammPRESSURE);
			
			EnterCriticalSection(&csDiagrammVOLUME);
			if(m_pDiagrammVolume)
				m_pDiagrammVolume->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammVOLUME);
			
			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopVolumeFlow)
				m_pLoopVolumeFlow->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammLOOP);

			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopPressureVolume)
			{
				m_pLoopPressureVolume->DrawCursor(true);
				m_bGraphSelected=true;
			}
			LeaveCriticalSection(&csDiagrammLOOP);
		}
		break;
	case IDC_LOOPDIAGRAM_VOLUMEFLOW:
		{
			EnterCriticalSection(&csDiagrammFLOW);
			if(m_pDiagrammFlow)
				m_pDiagrammFlow->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammFLOW);
			
			EnterCriticalSection(&csDiagrammPRESSURE);
			if(m_pDiagrammPressure)
				m_pDiagrammPressure->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammPRESSURE);
			
			EnterCriticalSection(&csDiagrammVOLUME);
			if(m_pDiagrammVolume)
				m_pDiagrammVolume->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammVOLUME);
			
			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopPressureVolume)
				m_pLoopPressureVolume->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammLOOP);

			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopVolumeFlow)
			{
				m_pLoopVolumeFlow->DrawCursor(true);
				m_bGraphSelected=true;
			}
			LeaveCriticalSection(&csDiagrammLOOP);
		}
		break;
	case 0:
		{
			EnterCriticalSection(&csDiagrammSPO2);
			if(m_pDiagrammSPO2)
				m_pDiagrammSPO2->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammSPO2);
			
			EnterCriticalSection(&csDiagrammCO2);
			if(m_pDiagrammCO2)
				m_pDiagrammCO2->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammCO2);
			
			EnterCriticalSection(&csDiagrammFLOW);
			if(m_pDiagrammFlow)
				m_pDiagrammFlow->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammFLOW);
			
			EnterCriticalSection(&csDiagrammPRESSURE);
			if(m_pDiagrammPressure)
				m_pDiagrammPressure->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammPRESSURE);
			
			EnterCriticalSection(&csDiagrammVOLUME);
			if(m_pDiagrammVolume)
				m_pDiagrammVolume->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammVOLUME);
			
			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopPressureVolume)
				m_pLoopPressureVolume->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammLOOP);

			EnterCriticalSection(&csDiagrammLOOP);
			if(m_pLoopVolumeFlow)
				m_pLoopVolumeFlow->DrawCursor(false);
			LeaveCriticalSection(&csDiagrammLOOP);

			m_bGraphSelected=false;
		}
		break;
	default:
		{

		}
		break;
	}
	
	if(m_bGraphSelected == true)
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SET_GRAPHCURSOR,iDiagramm);
		PostMessage(WM_SET_GRAPHTIMER);
	}
	else if(m_bGraphSelected == true && m_iCurFocusedWnd>0)
	{
		PostMessage(WM_SET_GRAPHTIMER);
	}
	else if(m_bGraphSelected == false && m_iCurFocusedWnd>0)
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_DEL_GRAPHCURSOR);
		iDiagramm=0;	
		PostMessage(WM_KILL_GRAPHTIMER);
	}

	m_iCurFocusedWnd=iDiagramm;
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==PARATIMER)
	{
		KillTimer(PARATIMER);
		EnterCriticalSection(&csDrawDataSPO2);
		if(m_pDataSPO2 && m_bDataSPO2IsActive)
		{
			m_pDataSPO2->SetAllButtonUnpressed();
		} 
		LeaveCriticalSection(&csDrawDataSPO2);
		
	}
	else if(nIDEvent==GRAPHTIMER)
	{
		KillTimer(GRAPHTIMER);
		DrawCursor(0);

		m_iCurFocusedWnd=0;

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->SetFocus();
	}


	CWnd::OnTimer(nIDEvent);
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::SaveLoops()
{
	CopyCurBreathToSavedBreath();
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::NotifyParaBtnEvent(CMVEvent* pEvent)
{
	switch(pEvent->GetET())
	{
	case CMVEvent::ET_UIEVENT:
		{
			CMVEventUI* pUIEvent = (CMVEventUI*)pEvent;
			switch(pUIEvent->GetEventType())
			{
			case CMVEventUI::EV_VOLUMEGARANT_ON:
				{
					//if(!m_bExit)
					{
						bool bCalculated=false;
						setDrawHFOVolGaranty(drawHFOVGarantyLine());

						if(getDrawHFOVolGaranty())
						{
							bCalculated=CalculateHFOVolGaranty();
						}
						else
						{
							setValueVolGarantyHigh(0);
							setValueVolGarantyLow(0);
						}

						EnterCriticalSection(&csDiagrammPRESSURE);
						if(m_pDiagrammPressure && m_bGraphPressureIsActive && bCalculated )
						{
							m_pDiagrammPressure->SetHFOVolumeGaranty(getDrawHFOVolGaranty(), getValueVolGarantyHigh(), getValueVolGarantyLow());

						}
						LeaveCriticalSection(&csDiagrammPRESSURE);

						if(		getModel()->getCONFIG()->CurModeIsPresetMode()==false 
							&&	getModel()->getCONFIG()->GraphIsAutoScale())
						{
							if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
							{
								int iMax=0;
								int iMin=0;

								if(		getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true 
									&&	getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
								{
									int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLmaxPara();
									int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
									int iPmeanrec=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
									iMax=iPmeanrec+(iAmp/2);
									iMin=iPmean-(iAmp/2)-100;
								}
								else
								{
									int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLmaxPara();
									int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
									iMax=iPmean+(iAmp/2);
									iMin=iPmean-(iAmp/2);
								}

								EnterCriticalSection(&csDiagrammPRESSURE);
								if(m_pDiagrammPressure &&	m_bGraphPressureIsActive)
								{
									m_pDiagrammPressure->ScaleToNextPossibleYValue(iMax,iMin,true,true);
								}
								LeaveCriticalSection(&csDiagrammPRESSURE);
							}
							else
							{
								int iVal=getModel()->getDATAHANDLER()->GetActiveModePRESSUREPara();//newVG
								EnterCriticalSection(&csDiagrammPRESSURE);
								if(m_pDiagrammPressure &&	m_bGraphPressureIsActive)
								{
									m_pDiagrammPressure->ScaleToNextPossibleYValue(iVal,0,true,true);
								}
								LeaveCriticalSection(&csDiagrammPRESSURE);

								EnterCriticalSection(&csDiagrammLOOP);
								if(m_pLoopPressureVolume &&	m_bLoopPressureVolumeIsActive)
								{
									m_pLoopPressureVolume->ScaleToNextPossibleXValue(iVal,0,true,true);
								}
								LeaveCriticalSection(&csDiagrammLOOP);
							}
						}
					}
				}
				break;
			case CMVEventUI::EV_VOLUMEGARANT_OFF:
				{
					//if(!m_bExit)
					{
						setDrawHFOVolGaranty(drawHFOVGarantyLine());

						if(getDrawHFOVolGaranty())
						{
							CalculateHFOVolGaranty();
						}
						else
						{
							setValueVolGarantyHigh(0);
							setValueVolGarantyLow(0);
						}


						EnterCriticalSection(&csDiagrammPRESSURE);
						if(m_pDiagrammPressure && m_bGraphPressureIsActive)
						{
							m_pDiagrammPressure->SetHFOVolumeGaranty(getDrawHFOVolGaranty(), getValueVolGarantyHigh(), getValueVolGarantyLow());
						}
						LeaveCriticalSection(&csDiagrammPRESSURE);

						if(		getModel()->getCONFIG()->CurModeIsPresetMode()==false 
							&&	getModel()->getCONFIG()->GraphIsAutoScale())
						{
							if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
							{
								int iMax=0;
								int iMin=0;

								if(		getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true 
									&&	getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
								{
									int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
									int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
									int iPmeanrec=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
									iMax=iPmeanrec+(iAmp/2);
									iMin=iPmean-(iAmp/2)-100;
								}
								else
								{
									int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
									int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
									iMax=iPmean+(iAmp/2);
									iMin=iPmean-(iAmp/2);
								}

								EnterCriticalSection(&csDiagrammPRESSURE);
								if(m_pDiagrammPressure	&&	m_bGraphPressureIsActive)
								{
									m_pDiagrammPressure->ScaleToNextPossibleYValue(iMax,iMin,true,true);
								}
								LeaveCriticalSection(&csDiagrammPRESSURE);
							}
							else
							{
								int iVal=getModel()->getDATAHANDLER()->GetActiveModePRESSUREPara();//newVG
								EnterCriticalSection(&csDiagrammPRESSURE);
								if(m_pDiagrammPressure	&&	m_bGraphPressureIsActive)
								{
									m_pDiagrammPressure->ScaleToNextPossibleYValue(iVal,0,true,true);
								}
								LeaveCriticalSection(&csDiagrammPRESSURE);

								EnterCriticalSection(&csDiagrammLOOP);
								if(m_pLoopPressureVolume &&	m_bLoopPressureVolumeIsActive)
								{
									m_pLoopPressureVolume->ScaleToNextPossibleXValue(iVal,0,true,true);
								}
								LeaveCriticalSection(&csDiagrammLOOP);
							}
						}
					}
				}
				break;
			case CMVEventUI::EV_PARABN_FOT:
				{
					if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
					{
						if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
						{
							int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
							int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
							int iMax=iPmean+(iAmp/2);
							int iMin=iPmean-(iAmp/2);

							EnterCriticalSection(&csDiagrammPRESSURE);
							if(m_pDiagrammPressure	&&	m_bGraphPressureIsActive )
							{
								m_pDiagrammPressure->ScaleToNextPossibleYValue(iMax,iMin,true,true);

							}
							LeaveCriticalSection(&csDiagrammPRESSURE);

							int iMaxXFOT=getModel()->getDATAHANDLER()->PARADATA()->getFOThfo_PMEANENDPara();
							int iMinXFOT=G_LOWER_MAXSCALE_PRESSURE;

							EnterCriticalSection(&csDiagrammFOT);
							if(m_pLoopFOT	&&	m_bLoopFOTIsActive )
							{
								m_pLoopFOT->ScaleToNextPossibleXValue(iMaxXFOT,iMinXFOT,true,true);

							}
							LeaveCriticalSection(&csDiagrammFOT);
						}
						else
						{
							int iVal=getModel()->getDATAHANDLER()->GetActiveModePRESSUREPara();

							EnterCriticalSection(&csDiagrammPRESSURE);
							if(m_pDiagrammPressure	&&	m_bGraphPressureIsActive)
							{
								m_pDiagrammPressure->ScaleToNextPossibleYValue(iVal,0,true,true);
							}
							LeaveCriticalSection(&csDiagrammPRESSURE);

							EnterCriticalSection(&csDiagrammLOOP);
							if(m_pLoopPressureVolume &&	m_bLoopPressureVolumeIsActive)
							{
								m_pLoopPressureVolume->ScaleToNextPossibleXValue(iVal,0,true,true);
							}
							LeaveCriticalSection(&csDiagrammLOOP);

							
							int iMaxXFOT=getModel()->getDATAHANDLER()->PARADATA()->getFOTconv_PEEPENDPara();
							int iMinXFOT=G_LOWER_MAXSCALE_PRESSURE;

							EnterCriticalSection(&csDiagrammFOT);
							if(m_pLoopFOT	&&	m_bLoopFOTIsActive )
							{
								m_pLoopFOT->ScaleToNextPossibleXValue(iMaxXFOT,iMinXFOT,true,true);

							}
							LeaveCriticalSection(&csDiagrammFOT);
						}
						
					}
				}
				break;
			case CMVEventUI::EV_PARABN_HFAMPL:
				{
					if(		getModel()->getCONFIG()->CurModeIsPresetMode()==false 
						&&	getModel()->getCONFIG()->GraphIsAutoScale()
						&& false==getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
					{
						int iMax=0;
						int iMin=0;

						if(		getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true 
							&&	getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
						{
							int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
							int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
							int iPmeanrec=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
							iMax=iPmeanrec+(iAmp/2);
							iMin=iPmean-(iAmp/2)-100;
						}
						else
						{
							int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
							int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
							iMax=iPmean+(iAmp/2);
							iMin=iPmean-(iAmp/2);
						}

						EnterCriticalSection(&csDiagrammPRESSURE);
						if(m_pDiagrammPressure	&&	m_bGraphPressureIsActive)
						{
							m_pDiagrammPressure->ScaleToNextPossibleYValue(iMax,iMin,true,true);
						}
						LeaveCriticalSection(&csDiagrammPRESSURE);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_PMITT:
			case CMVEventUI::EV_PARABN_IERATIO:
			case CMVEventUI::EV_PARABN_HFFREQREC:
				{
					if(		getModel()->getCONFIG()->CurModeIsPresetMode()==false 
						&&	getModel()->getCONFIG()->GraphIsAutoScale())
					{
						int iMax=0;
						int iMin=0;

						if(		getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true 
							&&	getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
						{
							int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
							int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
							int iPmeanrec=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
							iMax=iPmeanrec+(iAmp/2);
							iMin=iPmean-(iAmp/2)-100;
						}
						else
						{
							int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
							int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
							iMax=iPmean+(iAmp/2);
							iMin=iPmean-(iAmp/2);
						}

						EnterCriticalSection(&csDiagrammPRESSURE);
						if(m_pDiagrammPressure	&&	m_bGraphPressureIsActive )
						{
							m_pDiagrammPressure->ScaleToNextPossibleYValue(iMax,iMin,true,true);

						}
						LeaveCriticalSection(&csDiagrammPRESSURE);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_PPSV:
			case CMVEventUI::EV_PARABN_PINSP:
				{
					if(		getModel()->getCONFIG()->CurModeIsPresetMode()==false 
						&&	getModel()->getCONFIG()->GraphIsAutoScale())
					{
						int iVal=getModel()->getDATAHANDLER()->GetActiveModePRESSUREPara();//newVG

						EnterCriticalSection(&csDiagrammPRESSURE);
						if(m_pDiagrammPressure	&&	m_bGraphPressureIsActive)
						{
							m_pDiagrammPressure->ScaleToNextPossibleYValue(iVal,0,true,true);
						}
						LeaveCriticalSection(&csDiagrammPRESSURE);

						EnterCriticalSection(&csDiagrammLOOP);
						if(m_pLoopPressureVolume &&	m_bLoopPressureVolumeIsActive)
						{
							m_pLoopPressureVolume->ScaleToNextPossibleXValue(iVal,0,true,true);
						}
						LeaveCriticalSection(&csDiagrammLOOP);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_PMAXVOLG:
				{
					if(		getModel()->getCONFIG()->CurModeIsPresetMode()==false 
						&&	getModel()->getCONFIG()->GraphIsAutoScale()
						&& true==getModel()->getDATAHANDLER()->IsActiveModeVGarantStateOn())
					{
						if(getModel()->getVMODEHANDLER()->activeModeIsHFO())
						{
							int iMax=0;
							int iMin=0;

							if(		getModel()->getDATAHANDLER()->isLUNGRECLicenseAvailable()==true 
								&&	getModel()->getDATAHANDLER()->PARADATA()->GetFreqRecPara()!=0)
							{
								int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
								int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
								int iPmeanrec=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanRecPara();
								iMax=iPmeanrec+(iAmp/2);
								iMin=iPmean-(iAmp/2)-100;
							}
							else
							{
								int iAmp=getModel()->getDATAHANDLER()->PARADATA()->GetHFAMPLPara();
								int iPmean=getModel()->getDATAHANDLER()->PARADATA()->GetHFPMeanPara();
								iMax=iPmean+(iAmp/2);
								iMin=iPmean-(iAmp/2);
							}

							EnterCriticalSection(&csDiagrammPRESSURE);
							if(m_pDiagrammPressure	&&	m_bGraphPressureIsActive)
							{
								m_pDiagrammPressure->ScaleToNextPossibleYValue(iMax,iMin,true,true);
							}
							LeaveCriticalSection(&csDiagrammPRESSURE);
						}
						else
						{
							int iVal=getModel()->getDATAHANDLER()->GetActiveModePRESSUREPara();//newVG

							EnterCriticalSection(&csDiagrammPRESSURE);
							if(m_pDiagrammPressure	&&	m_bGraphPressureIsActive)
							{
								m_pDiagrammPressure->ScaleToNextPossibleYValue(iVal,0,true,true);
							}
							LeaveCriticalSection(&csDiagrammPRESSURE);

							EnterCriticalSection(&csDiagrammLOOP);
							if(m_pLoopPressureVolume &&	m_bLoopPressureVolumeIsActive)
							{
								m_pLoopPressureVolume->ScaleToNextPossibleXValue(iVal,0,true,true);
							}
							LeaveCriticalSection(&csDiagrammLOOP);
						}
					}
				}
				break;
			case CMVEventUI::EV_PARABN_PDUO:
				{
					if(		getModel()->getCONFIG()->CurModeIsPresetMode()==false 
						&&	m_bGraphPressureIsActive
						&&	getModel()->getCONFIG()->GraphIsAutoScale())
					{
						int iVal=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEPara();

						EnterCriticalSection(&csDiagrammPRESSURE);
						if(m_pDiagrammPressure)
							m_pDiagrammPressure->ScaleToNextPossibleYValue(iVal,0,true,true);
						LeaveCriticalSection(&csDiagrammPRESSURE);
					}
				}
				break;
			case CMVEventUI::EV_PARABN_PMAN:
				{
					if(		getModel()->getCONFIG()->CurModeIsPresetMode()==false 
						&&	getModel()->getCONFIG()->GraphIsAutoScale())
					{
						if(getModel()->getCONFIG()->GetCurMode()==VM_CPAP)
						{
							int iVal=getModel()->getDATAHANDLER()->PARADATA()->GetPManualCPAPPara();

							EnterCriticalSection(&csDiagrammPRESSURE);
							if(m_pDiagrammPressure	&&	m_bGraphPressureIsActive)
							{
								m_pDiagrammPressure->ScaleToNextPossibleYValue(iVal,0,true,true);
							}
							LeaveCriticalSection(&csDiagrammPRESSURE);

							EnterCriticalSection(&csDiagrammLOOP);
							if(m_pLoopPressureVolume &&	m_bLoopPressureVolumeIsActive)
							{
								m_pLoopPressureVolume->ScaleToNextPossibleXValue(iVal,0,true,true);
							}
							LeaveCriticalSection(&csDiagrammLOOP);
						}
						else if(getModel()->getCONFIG()->GetCurMode()==VM_NCPAP
							||	getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP
							||	getModel()->getCONFIG()->GetCurMode()==VM_THERAPIE)
						{
							int iVal=getModel()->getDATAHANDLER()->PARADATA()->GetPManualNMODEPara();

							EnterCriticalSection(&csDiagrammPRESSURE);
							if(m_pDiagrammPressure	&&	m_bGraphPressureIsActive)
							{
								m_pDiagrammPressure->ScaleToNextPossibleYValue(iVal,0,true,true);
							}
							LeaveCriticalSection(&csDiagrammPRESSURE);

						}
					}
				}
				break;
			default:
				break;
			}
		}
	}
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::NotifyAlarmLimitChanged()
{
	PostMessage(WM_ALIMIT_CHANGED);
}

// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::UpdateCO2InfoData(bool resetAvailable)
{
	if(isExit())
		return;

	EnterCriticalSection(&csDrawDataCO2);
	if(m_pDataCO2 && m_bDataCO2IsActive)
	{
		m_pDataCO2->UpdateInfoData(resetAvailable);
	}
	LeaveCriticalSection(&csDrawDataCO2);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::drawMeasuredFiO2Value()
{
	if(isExit())
		return;
	EnterCriticalSection(&csDrawDataSPO2);
	if(m_pDataSPO2 && m_bDataSPO2IsActive)
	{
		m_pDataSPO2->drawMeasuredFiO2Value();
	}
	LeaveCriticalSection(&csDrawDataSPO2);
}
// **************************************************************************
// 
// **************************************************************************
//void CViewDiagramm::setFIO2LOWvalue(BYTE iFIO2LOWvalue)
//{
//	if(isExit())
//		return;
//	EnterCriticalSection(&csDrawDataSPO2);
//	if(m_pDataSPO2 && m_bDataSPO2IsActive)
//	{
//		m_pDataSPO2->setFIO2LOWvalue(iFIO2LOWvalue);	
//	}
//	LeaveCriticalSection(&csDrawDataSPO2);
//}
// **************************************************************************
// 
// **************************************************************************
//void CViewDiagramm::setFIO2HIGHvalue(BYTE iFIO2HIGHvalue)
//{
//	if(isExit())
//		return;
//	EnterCriticalSection(&csDrawDataSPO2);
//	if(m_pDataSPO2 && m_bDataSPO2IsActive)
//	{
//		m_pDataSPO2->setFIO2HIGHvalue(iFIO2HIGHvalue);
//	}
//	LeaveCriticalSection(&csDrawDataSPO2);
//}
// **************************************************************************
// 
// **************************************************************************
//void CViewDiagramm::setSPO2LOWvalue(BYTE iSPO2LOWvalue)
//{
//	if(isExit())
//		return;
//	EnterCriticalSection(&csDrawDataSPO2);
//	if(m_pDataSPO2 && m_bDataSPO2IsActive)
//	{
//		m_pDataSPO2->setSPO2LOWvalue(iSPO2LOWvalue);
//	}
//	LeaveCriticalSection(&csDrawDataSPO2);
//}
// **************************************************************************
// 
// **************************************************************************
//void CViewDiagramm::setSPO2HIGHvalue(BYTE iSPO2HIGHvalue)
//{
//	if(isExit())
//		return;
//	EnterCriticalSection(&csDrawDataSPO2);
//	if(m_pDataSPO2 && m_bDataSPO2IsActive)
//	{
//		m_pDataSPO2->setSPO2HIGHvalue(iSPO2HIGHvalue);
//	}
//	LeaveCriticalSection(&csDrawDataSPO2);
//}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::UpdateLimitData()
{
	if(isExit())
		return;
	EnterCriticalSection(&csDrawDataSPO2);
	if(m_pDataSPO2 && m_bDataSPO2IsActive)
	{
		m_pDataSPO2->UpdateLimitData();
	}
	LeaveCriticalSection(&csDrawDataSPO2);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::setPRICOrunning(bool state)
{
	if(isExit())
		return;

	EnterCriticalSection(&csDrawDataSPO2);
	if(m_pDataSPO2 && m_bDataSPO2IsActive)
	{
		m_pDataSPO2->setPRICOrunning(state);
	}
	LeaveCriticalSection(&csDrawDataSPO2);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::drawSIQofSPO2()
{
	if(isExit() || getModel()->getVIEWHANDLER()->getViewSubState()!=VSS_GRAPH_SPO2GRAPHS)
		return;

	RECT rcCl,rc;

	rcCl.left = 0;  
	rcCl.top = 331;  
	rcCl.right  = 40;  
	rcCl.bottom = 481;

	rc.left = 0;  
	rc.top = 0;  
	rc.right  = 40;  
	rc.bottom = 157;

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,40,157);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);

	CBrush brBack(BACKGND);
	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,brBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf8AcuBold);
	int tc=SetTextColor(hdcMem,BLACK);
	
	FillRect(hdcMem,&rc,brBack);


	if(m_pcBargraphSIQofSPO2)
		m_pcBargraphSIQofSPO2->Draw(hdcMem,5,0);
	
	rc.left = 0;  
	rc.right  = 40;  
	rc.top = 134;  
	rc.bottom = 169;
	DrawText(hdcMem,_T("SIQ"),-1,&rc,DT_CENTER|DT_SINGLELINE|DT_TOP);

	SHORT iSpO2SIQ = getModel()->getDATAHANDLER()->readSpO2SIQ();


	//CDC* pDC=CDC::FromHandle(hdcMem);
	//(siq >=90 % grün, >=30% gelb, <30% rot, rot mit 2 Felder, gelb 4 Felder, grün 1 Feld, immer 15%)
	if(iSpO2SIQ>=90)
	{
		draw15SIQ(hdcMem);
		draw30SIQ(hdcMem);
		draw45SIQ(hdcMem);
		draw60SIQ(hdcMem);
		draw75SIQ(hdcMem);
		draw90SIQ(hdcMem);
		draw100SIQ(hdcMem);
	}
	else if(iSpO2SIQ>=75)
	{
		draw15SIQ(hdcMem);
		draw30SIQ(hdcMem);
		draw45SIQ(hdcMem);
		draw60SIQ(hdcMem);
		draw75SIQ(hdcMem);
		draw90SIQ(hdcMem);
	}
	else if(iSpO2SIQ>=60)
	{
		draw15SIQ(hdcMem);
		draw30SIQ(hdcMem);
		draw45SIQ(hdcMem);
		draw60SIQ(hdcMem);
		draw75SIQ(hdcMem);
	}
	else if(iSpO2SIQ>=45)
	{
		draw15SIQ(hdcMem);
		draw30SIQ(hdcMem);
		draw45SIQ(hdcMem);
		draw60SIQ(hdcMem);
	}
	else if(iSpO2SIQ>=30)
	{
		draw15SIQ(hdcMem);
		draw30SIQ(hdcMem);
		draw45SIQ(hdcMem);
	}
	else if(iSpO2SIQ>=15)
	{
		draw15SIQ(hdcMem);
		draw30SIQ(hdcMem);
	}
	else if(iSpO2SIQ>0)
	{
		draw15SIQ(hdcMem);
	}
	else	
	{
		rc.left = 0;  
		rc.right  = 40;  
		rc.top = 115;  
		rc.bottom = 130;
		DrawText(hdcMem,_T("--"),-1,&rc,DT_CENTER|DT_SINGLELINE|DT_TOP);

		/*rc.left = 0;  
		rc.right  = 40;  
		rc.top = 125;  
		rc.bottom = 130;
		DrawText(hdcMem,_T("--"),-1,&rc,DT_CENTER|DT_SINGLELINE|DT_TOP);*/
	}

	BYTE iSIQvalue=getModel()->getALARMHANDLER()->getAlimitSPO2_SIQmin();
	if(iSIQvalue<=15)
		iSIQvalue=129-iSIQvalue;
	else if(iSIQvalue<=30)
		iSIQvalue=129-2-iSIQvalue;
	else if(iSIQvalue<=45)
		iSIQvalue=129-4-iSIQvalue;
	else if(iSIQvalue<=60)
		iSIQvalue=129-6-iSIQvalue;
	else if(iSIQvalue<=75)
		iSIQvalue=129-8-iSIQvalue;
	else if(iSIQvalue<=90)
		iSIQvalue=129-10-iSIQvalue;
	else 
		iSIQvalue=129-12-iSIQvalue;
	

	CPen penAlarm;
	penAlarm.CreatePen(PS_SOLID,2,RGB(255,100,0));
	SelectObject(hdcMem,(HPEN)penAlarm);

	//MoveToEx(hdcMem, 5, iSIQvalue, NULL);
	MoveToEx(hdcMem, 5, iSIQvalue, NULL);
	LineTo(hdcMem, 13, iSIQvalue);

	BitBlt(dc.m_hDC,rcCl.left,rcCl.top,rcCl.right,rcCl.bottom,hdcMem,0,0,SRCCOPY);
	BitBlt(m_hDC,rcCl.left,rcCl.top,rcCl.right,rcCl.bottom,hdcMem,0,0,SRCCOPY);

	//rkuNEWFIX double entries deleted
	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);
	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);
	SelectObject(hdcMem,hPrevFont);
	SelectObject(hdcMem,hBmpMemPrev);

	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
	brBack.DeleteObject();//rkuNEWFIX
	penAlarm.DeleteObject();//rkuNEWFIX
}

void CViewDiagramm::draw15SIQ(HDC hdc)
{
	//HDC hdc = *pDC;

	CBrush cbrFill;
	cbrFill.CreateSolidBrush(RGB(255,0,0));

	HBRUSH hPrevBrush = (HBRUSH)SelectObject(hdc, cbrFill);	
	HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(NULL_PEN));

	Rectangle(hdc, 14, 114, 25, 129);

	SelectObject(hdc, hPrevBrush);	
	SelectObject(hdc, hPrevPen);
}
void CViewDiagramm::draw30SIQ(HDC hdc)
{
	//HDC hdc = *pDC;

	CBrush cbrFill;
	cbrFill.CreateSolidBrush(RGB(255,0,0));

	HBRUSH hPrevBrush = (HBRUSH)SelectObject(hdc, cbrFill);	
	HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(NULL_PEN));

	Rectangle(hdc, 14, 97, 25, 112);

	SelectObject(hdc, hPrevBrush);	
	SelectObject(hdc, hPrevPen);
}
void CViewDiagramm::draw45SIQ(HDC hdc)
{
	//HDC hdc = *pDC;

	CBrush cbrFill;
	cbrFill.CreateSolidBrush(RGB(255,245,0));

	HBRUSH hPrevBrush = (HBRUSH)SelectObject(hdc, cbrFill);	
	HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(NULL_PEN));

	Rectangle(hdc, 14, 80, 25, 95);

	SelectObject(hdc, hPrevBrush);	
	SelectObject(hdc, hPrevPen);
}
void CViewDiagramm::draw60SIQ(HDC hdc)
{
	//HDC hdc = *pDC;

	CBrush cbrFill;
	cbrFill.CreateSolidBrush(RGB(255,245,0));

	HBRUSH hPrevBrush = (HBRUSH)SelectObject(hdc, cbrFill);	
	HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(NULL_PEN));

	Rectangle(hdc, 14, 63, 25, 78);

	SelectObject(hdc, hPrevBrush);	
	SelectObject(hdc, hPrevPen);
}
void CViewDiagramm::draw75SIQ(HDC hdc)
{
	//HDC hdc = *pDC;

	CBrush cbrFill;
	cbrFill.CreateSolidBrush(RGB(255,245,0));

	HBRUSH hPrevBrush = (HBRUSH)SelectObject(hdc, cbrFill);	
	HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(NULL_PEN));

	Rectangle(hdc, 14, 46, 25, 61);

	SelectObject(hdc, hPrevBrush);	
	SelectObject(hdc, hPrevPen);
}
void CViewDiagramm::draw90SIQ(HDC hdc)
{
	//HDC hdc = *pDC;

	CBrush cbrFill;
	cbrFill.CreateSolidBrush(RGB(255,245,0));

	HBRUSH hPrevBrush = (HBRUSH)SelectObject(hdc, cbrFill);	
	HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(NULL_PEN));

	Rectangle(hdc, 14, 29, 25, 44);

	SelectObject(hdc, hPrevBrush);	
	SelectObject(hdc, hPrevPen);
}
void CViewDiagramm::draw100SIQ(HDC hdc)
{
	//HDC hdc = *pDC;

	CBrush cbrFill;
	cbrFill.CreateSolidBrush(RGB(0,250,0));

	HBRUSH hPrevBrush = (HBRUSH)SelectObject(hdc, cbrFill);	
	HPEN hPrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(NULL_PEN));

	Rectangle(hdc, 14, 17, 25, 27);

	SelectObject(hdc, hPrevBrush);	
	SelectObject(hdc, hPrevPen);
}

void CViewDiagramm::drawGraphTXT(bool bGraph1, bool bGraph2, bool bGraph3, bool bSIQofSPO2)
{
	RECT rcCl;
	RECT rc;
	GetClientRect(&rcCl);

	CClientDC dc(this);
	HDC hdcMem=CreateCompatibleDC(dc.m_hDC);
	HBITMAP hBmpMem=CreateCompatibleBitmap(dc.m_hDC,40,508);
	HBITMAP hBmpMemPrev=(HBITMAP)SelectObject(hdcMem,hBmpMem);



	CBrush cbrBack(BACKGND);
	//CBrush cbrBack(RGB(255,0,0));

	HBRUSH hbrprev=(HBRUSH)SelectObject(hdcMem,cbrBack);
	HPEN hpenprev=(HPEN)SelectObject(hdcMem, (HPEN)GetStockObject(NULL_PEN));	
	int nBkMode=SetBkMode(hdcMem,TRANSPARENT);
	//HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf7AcuBold90degree);
	HFONT hPrevFont=(HFONT)SelectObject(hdcMem,g_hf7AcuBold);
	int tc=SetTextColor(hdcMem,RGB(0,0,0));

	Rectangle(hdcMem, 0, 0, 40, 508);

	CBrush cbrGraph(GRAPHBACKGRND_COL);
	SelectObject(hdcMem,cbrGraph);

	CDC* pDC=CDC::FromHandle(hdcMem);

	int dyFont=pDC->GetTextExtent(_T("|")).cy-1;
	CStringW sz=_T("");
	int iLen = 0;

	if(bGraph1)
	{
		sz=m_szGraph1;
		iLen = sz.GetLength();

		Rectangle(hdcMem, 0, 7, 40, 157);
		rc.left = 0;//15;  
		rc.right  = 40;//100;  
		rc.top = 7;//5;  
		rc.bottom = 157;//147;

		LONG lDiff=rc.bottom-rc.top;

		RECT rc1;
		rc1.left=rc.left;
		rc1.right=rc.right-10;

		int iY = 0;
		if(iLen>0)
		{
			if(iLen%2==0)
			{
				iY = rc.top+(lDiff/2)-(iLen/2)*dyFont;
			}
			else
			{
				if(iLen>1)
				{
					iY = rc.top+(lDiff/2)-((((iLen-1)/2)*dyFont)+dyFont/2);
				}
				else
				{
					iY = rc.top+(lDiff/2)-(dyFont/2);
				}
			}

			for(int i=1; i <= iLen; i++)
			{
				rc1.top=iY;
				rc1.bottom=iY+dyFont;

				CStringW sz1 = sz.Left(1);
				DrawText(hdcMem,sz1,-1,&rc1,DT_VCENTER|DT_SINGLELINE|DT_CENTER);

				sz = sz.Right(iLen-i);
				iY = rc1.bottom;
			}
		}


		//DrawText(hdcMem,m_szGraph1,-1,&rc,DT_LEFT|DT_SINGLELINE|DT_BOTTOM);
		//DrawText(hdcMem,_T("PRESSURE"),-1,&rc,DT_CENTER|DT_SINGLELINE|DT_BOTTOM);

	}
	if(bGraph2)
	{
		sz=m_szGraph2;
		iLen = sz.GetLength();

		Rectangle(hdcMem, 0, 169, 40, 319);
		rc.left = 0;//15;  
		rc.right  = 40;//100;  
		rc.top = 169;//172;  
		rc.bottom = 319;//314;

		LONG lDiff=rc.bottom-rc.top;

		RECT rc1;
		rc1.left=rc.left;
		rc1.right=rc.right-10;

		int iY = 0;
		if(iLen>0)
		{
			if(iLen%2==0)
			{
				iY = rc.top+(lDiff/2)-(iLen/2)*dyFont;
			}
			else
			{
				if(iLen>1)
				{
					iY = rc.top+(lDiff/2)-((((iLen-1)/2)*dyFont)+dyFont/2);
				}
				else
				{
					iY = rc.top+(lDiff/2)-(dyFont/2);
				}
			}

			for(int i=1; i <= iLen; i++)
			{
				rc1.top=iY;
				rc1.bottom=iY+dyFont;

				CStringW sz1 = sz.Left(1);
				DrawText(hdcMem,sz1,-1,&rc1,DT_VCENTER|DT_SINGLELINE|DT_CENTER);

				sz = sz.Right(iLen-i);
				iY = rc1.bottom;
			}
		}
		//DrawText(hdcMem,m_szGraph2,-1,&rc,DT_LEFT|DT_SINGLELINE|DT_BOTTOM);

	}
	if(bGraph3)
	{
		sz=m_szGraph3;
		iLen = sz.GetLength();

		Rectangle(hdcMem, 0, 331, 40, 481);
		rc.left = 0;//15;  
		rc.right  = 40;//100;  
		rc.top = 331;//339;  
		rc.bottom = 481;

		LONG lDiff=rc.bottom-rc.top;

		RECT rc1;
		rc1.left=rc.left;
		rc1.right=rc.right-10;

		int iY = 0;
		if(iLen>0)
		{
			if(iLen%2==0)
			{
				iY = rc.top+(lDiff/2)-(iLen/2)*dyFont;
			}
			else
			{
				if(iLen>1)
				{
					iY = rc.top+(lDiff/2)-((((iLen-1)/2)*dyFont)+dyFont/2);
				}
				else
				{
					iY = rc.top+(lDiff/2)-(dyFont/2);
				}
			}

			for(int i=1; i <= iLen; i++)
			{
				rc1.top=iY;
				rc1.bottom=iY+dyFont;

				CStringW sz1 = sz.Left(1);
				DrawText(hdcMem,sz1,-1,&rc1,DT_VCENTER|DT_SINGLELINE|DT_CENTER);

				sz = sz.Right(iLen-i);
				iY = rc1.bottom;
			}
		}

		//DrawText(hdcMem,m_szGraph3,-1,&rc,DT_LEFT|DT_SINGLELINE|DT_BOTTOM);

	}

	//if(bGraph1)
	//{
	//	Rectangle(hdcMem, 0, 7, 40, 157);
	//	rc.left = 15;  
	//	rc.right  = 100;  
	//	rc.top = 5;  
	//	rc.bottom = 147;
	//	DrawText(hdcMem,m_szGraph1,-1,&rc,DT_LEFT|DT_SINGLELINE|DT_BOTTOM);
	//	//DrawText(hdcMem,_T("PRESSURE"),-1,&rc,DT_CENTER|DT_SINGLELINE|DT_BOTTOM);

	//}
	//if(bGraph2)
	//{
	//	Rectangle(hdcMem, 0, 169, 40, 319);
	//	rc.left = 15;  
	//	rc.right  = 100;  
	//	rc.top = 172;  
	//	rc.bottom = 314;
	//	DrawText(hdcMem,m_szGraph2,-1,&rc,DT_LEFT|DT_SINGLELINE|DT_BOTTOM);

	//}
	//if(bGraph3)
	//{
	//	Rectangle(hdcMem, 0, 331, 40, 481);
	//	rc.left = 15;  
	//	rc.right  = 100;  
	//	rc.top = 339;  
	//	rc.bottom = 481;
	//	DrawText(hdcMem,m_szGraph3,-1,&rc,DT_LEFT|DT_SINGLELINE|DT_BOTTOM);

	//}
	if(bSIQofSPO2)
	{
		if(m_pcBargraphSIQofSPO2)
			m_pcBargraphSIQofSPO2->Draw(hdcMem,5,331);
		SetTextColor(hdcMem,BLACK);
		SelectObject(hdcMem,g_hf8AcuBold);
		rc.left = 0;  
		rc.right  = 40;  
		rc.top = 465;  
		rc.bottom = 500;
		DrawText(hdcMem,_T("SIQ"),-1,&rc,DT_CENTER|DT_SINGLELINE|DT_TOP);

		rc.left = 0;  
		rc.right  = 40;  
		rc.top = 445;  
		rc.bottom = 465;
		DrawText(hdcMem,_T("--"),-1,&rc,DT_CENTER|DT_SINGLELINE|DT_TOP);
	}

	BitBlt(dc.m_hDC,0,0,40, 508,hdcMem,0,0,SRCCOPY);
	BitBlt(m_hDC,0,0,40, 508,hdcMem,0,0,SRCCOPY);

	SetTextColor(hdcMem,tc);
	SetBkMode(hdcMem,nBkMode);

	SelectObject(hdcMem,hbrprev);
	SelectObject(hdcMem,hpenprev);

	//penLine.DeleteObject();

	SelectObject(hdcMem,hBmpMemPrev);
	SelectObject(hdcMem,hPrevFont);
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);

}
void CViewDiagramm::setTextGraph1(CString szText)
{
	m_szGraph1=szText;
}
void CViewDiagramm::setTextGraph2(CString szText)
{
	m_szGraph2=szText;
}
void CViewDiagramm::setTextGraph3(CString szText)
{
	m_szGraph3=szText;
}

void CViewDiagramm::setDrawVolLimit(bool bState)
{
	EnterCriticalSection(&csVolLimit);
	m_bDrawVolumeLimit=bState;
	LeaveCriticalSection(&csVolLimit);
}
bool CViewDiagramm::getDrawVolLimit()
{
	EnterCriticalSection(&csVolLimit);
	bool bVL=m_bDrawVolumeLimit;
	LeaveCriticalSection(&csVolLimit);
	return bVL;
}
void CViewDiagramm::setValueVolLimit(double val)
{
	EnterCriticalSection(&csVolLimit);
	m_dbValueVolumeLimit=val;
	LeaveCriticalSection(&csVolLimit);
}
double CViewDiagramm::getValueVolLimit()
{
	EnterCriticalSection(&csVolLimit);
	double dbVal=m_dbValueVolumeLimit;
	LeaveCriticalSection(&csVolLimit);
	return dbVal;
}

void CViewDiagramm::setDrawVolGaranty(bool bState)
{
	EnterCriticalSection(&csVolGarant);
	m_bDrawVolumeGaranty=bState;
	LeaveCriticalSection(&csVolGarant);
}
bool CViewDiagramm::getDrawVolGaranty()
{
	EnterCriticalSection(&csVolGarant);
	bool bVG=m_bDrawVolumeGaranty;
	LeaveCriticalSection(&csVolGarant);
	return bVG;
}
void CViewDiagramm::setValueVolGaranty(double val)
{
	EnterCriticalSection(&csVolGarant);
	m_dbValueVolumeGaranty=val;
	LeaveCriticalSection(&csVolGarant);
}
double CViewDiagramm::getValueVolGaranty()
{
	EnterCriticalSection(&csVolGarant);
	double dbVal=m_dbValueVolumeGaranty;
	LeaveCriticalSection(&csVolGarant);
	return dbVal;
}


void CViewDiagramm::setDrawHFOVolGaranty(bool bState)
{
	EnterCriticalSection(&csVolGarant);
	m_bDrawHFOVolumeGaranty=bState;
	LeaveCriticalSection(&csVolGarant);
}
bool CViewDiagramm::getDrawHFOVolGaranty()
{
	EnterCriticalSection(&csVolGarant);
	bool bVG=m_bDrawHFOVolumeGaranty;
	LeaveCriticalSection(&csVolGarant);
	return bVG;
}
void CViewDiagramm::setValueVolGarantyHigh(double val)
{
	EnterCriticalSection(&csVolGarant);
	m_dbHFOValueVolumeGarantyHigh=val;
	LeaveCriticalSection(&csVolGarant);
}
double CViewDiagramm::getValueVolGarantyHigh()
{
	EnterCriticalSection(&csVolGarant);
	double dbVal=m_dbHFOValueVolumeGarantyHigh;
	LeaveCriticalSection(&csVolGarant);
	return dbVal;
}

void CViewDiagramm::setValueVolGarantyLow(double val)
{
	EnterCriticalSection(&csVolGarant);
	m_dbHFOValueVolumeGarantyLow=val;
	LeaveCriticalSection(&csVolGarant);
}
double CViewDiagramm::getValueVolGarantyLow()
{
	EnterCriticalSection(&csVolGarant);
	double dbVal=m_dbHFOValueVolumeGarantyLow;
	LeaveCriticalSection(&csVolGarant);
	return dbVal;
}

//void CViewDiagramm::setResetCurrentXaxes()
//{
//	EnterCriticalSection(&csThreadAccess);
//	m_bResetCurrentXaxes=true;
//	LeaveCriticalSection(&csThreadAccess);
//}
//void CViewDiagramm::deleteResetCurrentXaxes()
//{
//	EnterCriticalSection(&csThreadAccess);
//	m_bResetCurrentXaxes=false;
//	LeaveCriticalSection(&csThreadAccess);
//}
//bool CViewDiagramm::getResetCurrentXaxes()
//{
//	EnterCriticalSection(&csThreadAccess);
//	bool bReset=m_bResetCurrentXaxes;
//	LeaveCriticalSection(&csThreadAccess);
//	return bReset;
//}
void CViewDiagramm::setUpdateRingBufCopy()
{
	EnterCriticalSection(&csThreadAccess);
	m_bUpdateRingBufCopy=true;
	LeaveCriticalSection(&csThreadAccess);
}
bool CViewDiagramm::getUpdateRingBufCopy()
{
	EnterCriticalSection(&csThreadAccess);
	bool bState=m_bUpdateRingBufCopy;
	LeaveCriticalSection(&csThreadAccess);
	return bState;
}
void CViewDiagramm::deleteUpdateRingBufCopy()
{
	EnterCriticalSection(&csThreadAccess);
	m_bUpdateRingBufCopy=false;
	LeaveCriticalSection(&csThreadAccess);
}

void CViewDiagramm::setSizeReadInBufferSPI(int iSPIdata)
{
	if(iSPIdata>MAXSIZE_SPIREADIN_BUFFER)
	{
		iSPIdata=MAXSIZE_SPIREADIN_BUFFER;
	}
	EnterCriticalSection(&csThreadAccess);
	m_iSizeReadInBufferSPI=iSPIdata;
	LeaveCriticalSection(&csThreadAccess);
}
int CViewDiagramm::getSizeReadInBufferSPI()
{
	EnterCriticalSection(&csThreadAccess);
	int iSize=m_iSizeReadInBufferSPI;
	LeaveCriticalSection(&csThreadAccess);
	return iSize;
}

void CViewDiagramm::resetRedrawDiagram()
{
	EnterCriticalSection(&csRedrawDiagram);
	m_iDiagramToRedraw = 0;
	LeaveCriticalSection(&csRedrawDiagram);
}
void CViewDiagramm::setRedrawDiagram(WORD diagram)
{
	//DEBUGMSG(TRUE, (TEXT("setRedrawDiagram\r\n")));
	EnterCriticalSection(&csRedrawDiagram);
	m_iDiagramToRedraw = m_iDiagramToRedraw | diagram;
	LeaveCriticalSection(&csRedrawDiagram);
}
WORD CViewDiagramm::getRedrawDiagram()
{
	EnterCriticalSection(&csRedrawDiagram);
	WORD toRedraw=m_iDiagramToRedraw;
	LeaveCriticalSection(&csRedrawDiagram);
	return toRedraw;
}

void CViewDiagramm::setSizeCurrentBreath(int iSizeBreath)
{
	EnterCriticalSection(&csThreadAccess);
	m_iSizeCurrentBreath=iSizeBreath;
	LeaveCriticalSection(&csThreadAccess);
}

int CViewDiagramm::getSizeCurrentBreath()
{
	EnterCriticalSection(&csThreadAccess);
	int iSizeBreath=m_iSizeCurrentBreath;
	LeaveCriticalSection(&csThreadAccess);
	return iSizeBreath;
}

void CViewDiagramm::setFOTtime(BYTE iCountFOTimer)
{
	EnterCriticalSection(&csDrawDataFOT);
	if(m_pDataFOT)
	{
		m_pDataFOT->setFOTtime(iCountFOTimer);
	}
	LeaveCriticalSection(&csDrawDataFOT);
}
//void CViewDiagramm::setCollectdataFOTtime(BYTE iCountFOTimer)
//{
//	EnterCriticalSection(&csDrawDataFOT);
//	if(m_pDataFOT)
//	{
//		m_pDataFOT->setCollectdataFOTtime(iCountFOTimer);
//	}
//	LeaveCriticalSection(&csDrawDataFOT);
//}

void CViewDiagramm::setFOTrunning(bool state)
{
	EnterCriticalSection(&csDrawDataFOT);
	if(m_pDataFOT)
	{
		m_pDataFOT->setFOTrunning(state);
	}
	LeaveCriticalSection(&csDrawDataFOT);
}
// **************************************************************************
// 
// **************************************************************************
void CViewDiagramm::updateFOTPmeanPara()
{
	EnterCriticalSection(&csDrawDataFOT);
	if(m_pDataFOT)
	{
		m_pDataFOT->updateFOTPmeanPara();
	}
	LeaveCriticalSection(&csDrawDataFOT);
}

void CViewDiagramm::updateFOTPEEPPara()
{
	EnterCriticalSection(&csDrawDataFOT);
	if(m_pDataFOT)
	{
		m_pDataFOT->updateFOTPEEPPara();
	}
	LeaveCriticalSection(&csDrawDataFOT);
}