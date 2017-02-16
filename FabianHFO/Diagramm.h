#pragma once

#include "MVModel.h"
#include "Bmp.h"
#include "PushBtn.h"
#include "colour.h"

#define DIAGRAMM_WIDTH 565
#define DIAGRAMM_HEIGHT 150
#define DIAGRAMM_DOUBLEHEIGHT 319
#define DIAGRAMM_DATAAREA 530//498
#define DIAGRAMM_DATAHEIGHT 157

#define DIAGRAMM_RIGHTMARGIN_CURVE 10
#define DIAGRAMM_RIGHTMARGIN_LOOP 12
#define DIAGRAMM_LEFTMARGIN 24
#define DIAGRAMM_BOTTOMMARGIN_LOOP 38

#define LOOP_WIDTH 290
#define LOOP_HEIGHT 315


#define TIMEAUTOSCALE 5000

// CDiagramm

class CDiagramm : public CWnd
{
	DECLARE_DYNAMIC(CDiagramm)

public:
	CDiagramm();

	virtual ~CDiagramm();

	virtual BOOL Create(CWnd* pParentWnd, const RECT &rc, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void Show(BOOL bShow) ;
	
	void ClearFunction(bool redraw=false);
	BOOL DoFunction(G_FUNCTIONSTRUCT_LD *pFunctionParams);

	COLORREF getCurGraphPenColor();

	void PaintGraph(void);
	void SetYAxisScale(double min, double max, BOOL bRedraw=FALSE,bool bRedrawDiagrammData=false);
	void SetXAxisScale(double min,double max, BOOL bRedraw=FALSE,bool bRedrawDiagrammData=false);
	int SetColorScheme(int Scheme, COLORREF crGraphPenColor, BOOL bRedraw=FALSE);
	void SetTriggeredBreath(bool state);
	void IgnoreAutofit(bool state);
	void SetXAxisTickSpace(double iXTickSpace);
	void SetDiagrammType(int iType);
	void DrawCursor(bool state);
	
	void ScaleToNextPossibleXValue(double dMaxX, double dMinX, bool bRedraw,bool bRedrawDiagrammData);
	void ScaleToNextPossibleYValue(double dMaxY, double dMinY, bool bRedraw,bool bRedrawDiagrammData);

	bool IncreaseXScaleToNextValue();
	bool IncreaseYScaleToNextValue();
	bool DecreaseXScale(bool bTry, bool bRedrawDiagrammData);
	bool DecreaseYScaleToNextValue(bool bRedrawDiagrammData=false);
	
	double GetXAxisMaxPressure(double dMaxX);
	double GetXAxisMinPressure(double dMinX);
	double GetYAxisMaxPressure(double dMaxY);
	double GetYAxisMinPressure(double dMinY);
	double GetNegativeYoffset(double dwMaxY);
	void SetFreeze(bool state);

	void SetVolumeLimit(bool drawState, double iLimit);
	void SetHFOVolumeGaranty(bool drawState, double iValueHigh, double iValuelow);
	void SetVolumeGaranty(bool drawState, double iValue);

	void SetTriggerThreshold(double dTrigThreshold);
	void NextBreath();

	bool MoveXup();
	bool MoveXdown();
	bool MoveYleft();
	bool MoveYright();

private:
	bool isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay=0);//used for (dwLastTickCount+DELAY<getTickCount64())

	CRect CalcDataArea(void);
	
	void SetYAxisTickSpace(double iYTickSpace);
	void SetXAxisNumericSpace(double iXTickSpace);
	void SetYAxisNumericSpace(double iYTickSpace);

	double CalculateXAxisTickGridSpace();
	double CalculateYAxisTickGridSpace();
	double CalculateXAxisNumericSpace();
	double CalculateYAxisNumericSpace();

	double GetNextHigherXAxisMax();
	double GetNextHigherYAxisMax();

	double GetNextLowerYAxisMax();

	bool CanDecreaseYScale();

	int GetYPixelsOfDiagramm();

	LONG ConvertToGraphCoords(double x, double y);	
	double ConstrainY (double y);
	void DoPlotXYPoints();
	void DoPlotXYLoopPoints();
	void DoPlotSavedLoopPoints();
	void DoPlotYPoints();
	void DrawConnectLine(CPoint *pts, int iSize);
	void DrawConnectLoop(CPoint *pts, int iSize, bool levelPlot);
	void DrawFOTLoop(CPoint *pts, int iSize, bool levelPlot);
	void DrawFilledLine(CPoint *pts, int iSize);

	void PlotPointArray(CPoint *pts, int iSize, bool copyPlot=false);
	void DrawFunction();
	
	void DrawYAxisNumbers(CDC* pDC);
	void DrawXAxisNumbers(CDC* pDC);
	void DrawTicks(CDC* pDC);
	void DrawGrid(CDC* pDC);

	double CalcXAxisGridAndTicks(void);
	double CalcYAxisGridAndTicks(void);
	double CalcXAxisNumerics(void);
	double CalcYAxisNumerics(void);

	double GetXAxisTickSpace();
	//double GetYAxisTickSpace();
	double GetXAxisNumericSpace();
	double GetYAxisNumericSpace();

	void DrawAutoscaleState(CDC* pDC);
	void DrawYLegend(CDC* pDC);
	void DrawXLegend(CDC* pDC);
	void SetDefaultYLegend(void);
	void SetDefaultXLegend(void);

	void CheckAutoScaleXYincrease();
	void CheckAutoScaleXYdecrease();

	void CheckAutoScaleY();

	void IncreaseXScale(bool bRedrawDiagrammData/*=false*/);
	void IncreaseYScale(bool bRedrawDiagrammData/*=false*/);
	
	double GetHigherXAxisMax();
	double GetHigherYAxisMax();
	
	void RefreshLineChart();
	void RefreshFilledChart();

	CMVModel *getModel();

private:

	CMVModel* m_pModel;

	bool m_bExit;

	bool m_bTouchmove;

	UINT m_nID;

	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;

	DWORD m_dwStart;

	CPoint *ptsLoop;
	CPoint *ptsLine;

	CBmp* m_pcCursor;
	
	HDC		m_hdcStatic;
	HDC		m_hdcGraph;
	HBITMAP m_hbmpStatic;
	HBITMAP m_hbmpGraph;

	/*HDC		m_hdcLevel1;
	HBITMAP m_hbmpLevel1;*/
	/*HDC		m_hdcLevel2;
	HBITMAP m_hbmpLevel2;*/

	int m_iDiagrammType;
	int m_iCurColorScheme;

	bool m_bDrawCursor;

	BOOL m_bShowTicks;//Ticks are the little things on the x & y axis
	BOOL m_bShowGrid;
	BOOL m_bAutofit;
	BOOL m_bIgnoreAutofit;
	BOOL m_bYLineAtLeft;
	CStringW m_szYLegendText;
	CStringW m_szXLegendText;
	COLORREF m_crYTickColor;
	COLORREF m_crXTickColor;
	COLORREF m_crYLegendTextColor;
	COLORREF m_crXLegendTextColor;
	COLORREF m_crGraphAutostateColor;
	COLORREF m_crGraphAutostateBkColor;
	COLORREF m_crStaticGraphPenColor;
	COLORREF m_crCurrentGraphPenColor;
	COLORREF m_crGraphBkColor;
	COLORREF m_crGridColor;
	COLORREF m_crVLimitColor;
	COLORREF m_crPrevLoop1;
	COLORREF m_crPrevLoop2;
	COLORREF m_crPrevLoop3;
	COLORREF m_crTrigger;

	int m_iGraphPenSize;//
	UINT m_iGraphWidth;//
	UINT m_iGraphHeight;//
	UINT m_iGraphX;//location of the graph within the window
	UINT m_iGraphY;//location of the graph within the window
	double m_dXAxisMin;// the start value of X
	double m_dYAxisMin;//start value of Y
	double m_dXAxisMax;
	double m_dYAxisMax;

	UINT m_rmargin;
	UINT m_lmargin;
	UINT m_tmargin;
	UINT m_bmargin;

	//Helper calculated values - that is to say that these
	//values are not passed in to the graph
	//they are calculated from other given
	//CFont m_GraphFont;//default font font created from default fontface, & point size
	int m_iCharHeight;//calculated from the font
	int m_iCharWidth;//calculated from the font
	int m_iOriginX;//location of the origin within the graph
	int m_iOriginY;//location of the origin within the graph
	double m_dPixelsPerY;//scaling
	double m_dPixelsPerX;//scaling	
	int m_iScrollPosX;//
	int m_iScrollPosY;//
	//Data related variables
	G_FUNCTIONSTRUCT_LD *m_pFunctionParams;

	BOOL m_bFirstpoint;
	UINT m_prevx;
	UINT m_prevy;

	bool m_bTriggeredBreath;
	bool m_bPreviousLoopBreath;
	bool m_bSelected;

	double m_dXTickSpace;
	double m_dYTickSpace;

	double m_dXNumericSpace;
	double m_dYNumericSpace;

	DWORD m_dwLastCheckAutoScaleX;
	DWORD m_dwLastCheckAutoScaleY;
	
	double m_dMaxX;
	double m_dMinX;
	double m_dMaxY;
	double m_dMinY;

	bool m_bFREEZE;

	int m_iVolumeLimit;
	bool m_bDrawVolumeLimit;

	int m_iVolumeGaranty;
	bool m_bDrawVolumeGaranty;

	int m_iHFOVolumeGarantyHigh;
	int m_iHFOVolumeGarantyLow;
	bool m_bDrawHFOVolumeGarantyHigh;
	bool m_bDrawHFOVolumeGarantyLow;

	int m_iTrigThreshold;

	int m_iXStartPos;
	int m_iYStartPos;
	
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
};


