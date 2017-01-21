#pragma once

#include "MVModel.h"
#include "Bmp.h"
#include "TrendScaleBtn.h"
#include "colour.h"

#define TREND_WIDTH 565
#define TREND_HEIGHT 140
#define TREND_DATAAREA 498
#define TREND_LEFTRANGE 500

//// Some  defines


//the colors for multiplot graphs
#define G_NUM_MULTIPLOT_COLORS 8
#define G_MULTIPLOT_COLOR_ONE (RGB(192,255,255))
#define G_MULTIPLOT_COLOR_TWO (RGB(0,0,255))
#define G_MULTIPLOT_COLOR_THREE (RGB(255,0,0))
#define G_MULTIPLOT_COLOR_FOUR (RGB(255,255,0))
#define G_MULTIPLOT_COLOR_FIVE (RGB(255,192,192))
#define G_MULTIPLOT_COLOR_SIX (RGB(0,255,0))
#define G_MULTIPLOT_COLOR_SEVEN (RGB(0,255,255))
#define G_MULTIPLOT_COLOR_EIGHT (RGB(255,0,255))







class CTrendDiagramm : public CWnd
{
	DECLARE_DYNAMIC(CTrendDiagramm)

public:
	CTrendDiagramm();
	//CTrendDiagramm(CWnd *pParentWnd,int xPos=0, int yPos=0, int Width =0, int Height=0, UINT colorscheme=G_DEFAULTSCHEME);

	virtual ~CTrendDiagramm();

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, UINT colorscheme, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void RefreshLocalTime(COleDateTime dtStartTime, COleDateTime dtOffsetTime);

	void Show(BOOL bShow) ;

	void ShowTicks(BOOL bShow);
	void ClearFunction();
	BOOL DoFunction(G_FUNCTIONSTRUCT_TD *pFunctionParams);
	void SetFunctionName(CStringW FunctionName);
	void ShowGrid(BOOL bShow);
	//void SetYLineAtLeft(BOOL AtLeft);//rkuTRENDIMPR
	void GraphSetAllDefaults();
	void SetYLegendText(CStringW YText);
	void SetXLegendText(CStringW XText);
	void SetGraphTitle(CStringW GraphTitle);
	void PaintGraph(void);
	void SetYAxisScale(double min, double max, BOOL bRedraw=FALSE);
	void SetColorScheme(int Scheme, BOOL bRedraw=FALSE);

	bool IncreaseYScaleToNextValue();
	bool DecreaseYScaleToNextValue();

	double GetNegativeYoffset(double dwMaxY);
	
	void SetTrendType(int iType);
	int GetTrendType();

	bool MoveToPos(int iPos);
	bool MoveLeft(UINT iSpan);
	bool MoveRight(UINT iSpan);
	int IncreaseTime();
	int DecreaseTime();

	int GetOffsetMinutes();
	COleDateTime GetStartTime();
	double GetTrendSpan();
	void SetTrendSpan(int iTrendSpan);
	double GetXAxisTickSpace();

	double GetPixelsPerMinute();

	void DrawCursor(bool state);

	void DrawTrendCursor(bool bClear);
	void SetTrendCursorPos(int iCurTrendCursorPos, int iPosbufferX);
	int GetTrendCursorPos();
	int GetTrendBufferXPos();
	int GetTrendCursorValue();
	int MoveTrendCursorLeft(bool bFast=false);
	int MoveTrendCursorRight(bool bFast=false);

private:
	int GetMoveSpan();
	int GetNextIncreaseTrendSpan();
	int GetNextDecreaseTrendSpan();

	void DrawFunction();
	void SetXAxisScale(double min,double max, BOOL bRedraw=FALSE);
	LONG ConvertToGraphCoords(double x, double y);	
	double ConstrainY (double y);
	void DoPlotXYPoints();
	void DrawConnectLine(UINT FromX, UINT FromY, UINT ToX, UINT ToY);
	void DrawConnectLine(CPoint *pts, int iSize);
	void DrawFilledLine(UINT FromX, UINT FromY, UINT ToX, UINT ToY);
	void DrawFilledLine(CPoint *pts, int iSize);
	void PlotPointArray(CPoint *pts, int iSize);
	void SetDefaultColorScheme(void);
	void DrawFunctionName(CDC* pDC);
	void SetDefaultFunctionName(void);
	void DrawYAxisNumbers(CDC* pDC);
	void DrawXAxisNumbers(CDC* pDC);
	void DrawTicks(CDC* pDC);
	double CalcYAxisGridAndTicks(void);
	CRect CalcDataArea(void);
	double CalcXAxisGridAndTicks(void);
	void DrawGridNum(CDC* pDC);
	void DrawYLegend(CDC* pDC);
	void DrawXLegend(CDC* pDC);
	void SetDefaultYLegend(void);
	void SetDefaultXLegend(void);
	void SetDefaultGraphTitle(void);
	int GetYPixelsOfDiagramm();
	void DrawAutoscaleState(CDC* pDC);
	int GetDataAreaWidth();
	int GetDataAreaHight();
	double CalcXAxisNumerics();
	double CalculateYAxisTickGridSpace();
	double CalculateYAxisNumericSpace();
	
	
	void SetXAxisNumericSpace(double dXNumericSpace);
	double GetXAxisNumericSpace();
	void SetYAxisTickSpace(double dYTickSpace);
	double GetYAxisTickSpace();
	void SetYAxisNumericSpace(double dYNumericSpace);
	double GetYAxisNumericSpace();
	double CalcYAxisNumerics();
	void CheckAutoScaleY();
	void SetXAxisTickSpace(/*double dXTickSpace*/);

	
	void IncreaseYScale();
	double GetHigherYAxisMax();
	void DecreaseYScale();
	double GetLowerYAxisMax();
	
	double GetNextHigherYAxisMax();
	bool CanDecreaseYScale();
	double GetNextLowerYAxisMax();
	void SetAutofit(bool state);
	void SetFirstpoint(bool state, UINT prevx);
	UINT GetPrevXval();

	CMVModel *getModel();

	//void DrawGraphTitle(CDC* pDC);
	/*UINT CalcRightMargin();
	UINT CalcLeftMargin();
	UINT CalcBottomMargin();
	UINT CalcTopMargin();*/
	//void DrawCursor(bool state);
	//int GetRightPixelsOfXAxis();
	//double CalculateXAxisTickGridSpace();
	//void IgnoreAutofit(bool state);
	//void DoHistogram();//rkuTRENDIMPR
	//COLORREF GetMultiPlotPenColor(UINT PenNumber);//rkuTRENDIMPR
	//void DoMultiPlotXY(void);//rkuTRENDIMPR
	//void DoDeviationPercentAbs(void);//rkuTRENDIMPR	
	//void DoPlotXY();//rkuTRENDIMPR
	//void DoPlotY();//rkuTRENDIMPR
	//void DoPlotPoints();
	//void DoPlotYPoints();//rkuTRENDIMPR
	//void DoLogeX();//rkuTRENDIMPR
	//void DoMXPlusC(void);//rkuTRENDIMPR
	//void DoXCubed(void);//rkuTRENDIMPR
	//void DoXSquared();//rkuTRENDIMPR
	//void DoCosX(void);//rkuTRENDIMPR
	//void DrawBar(UINT x, UINT y);
	//void DrawConnectLinePoints(POINT *pts, int iSize);
	//void PlotPoints(UINT x, UINT y, UINT prevx, UINT prevy);//rkuTRENDIMPR
	//void PlotArray(PPOINT pts, int iSize);
	//void DrawDot (UINT X, UINT Y);//rkuTRENDIMPR
	//void DoSineX();//rkuTRENDIMPR

private:
	CMVModel* m_pModel;

	UINT m_nID;

	int m_iCurTrendCursorPos;

	CPoint *ptsTrend;

	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;

	COleDateTime m_dtStartTime;
	COleDateTime m_dtOffsetTime;
	COleDateTimeSpan m_tsOffsetStartTime;

	DWORD m_dwStart;

	CBmp* m_pcCursor;
	CBmp* m_pcTrendDecrease_UP;	//EV_BN_MATRIX_CURSOR_UP
	CBmp* m_pcTrendDecrease_DW;
	CBmp* m_pcTrendIncrease_UP;
	CBmp* m_pcTrendIncrease_DW;

	CTrendScaleBtn* m_pcGraphDecrease;
	CTrendScaleBtn* m_pcGraphIncrease;

	HDC		m_hdcStatic;
	HDC		m_hdcGraph;
	HBITMAP m_hbmpStatic;
	HBITMAP m_hbmpGraph;

	int m_iCurColorScheme;
	//int m_iTrendType;

	BOOL m_bFirstpoint;
	UINT m_prevx;
	UINT m_prevy;

	bool m_bSelected;
	//bool m_bCursor;

	double m_dXTickSpace;
	double m_dYTickSpace;

	double m_dXNumericSpace;
	double m_dYNumericSpace;

	//DWORD m_dwLastCheckAutoScaleX;
	//DWORD m_dwLastCheckAutoScaleY;

	double m_dMaxX;
	double m_dMinX;
	double m_dMaxY;
	double m_dMinY;


	UINT m_rmargin;
	UINT m_lmargin;
	UINT m_tmargin;
	UINT m_bmargin;

	UINT m_iTrendType;
	CStringW m_szUnit;


	//origin
	CStringW m_szFunctionNameText;
	COLORREF m_crFunctionNameColor;
	BOOL m_bShowTicks;//Ticks are the little things on the x & y axis
	BOOL m_bShowGrid;
	BOOL m_bAutofit;
	BOOL m_bIgnoreAutofit;
	BOOL m_bYLineAtLeft;
	CStringW m_szFontFace;
	CStringW m_szYLegendText;
	CStringW m_szXLegendText;
	CStringW m_szGraphTitle;
	COLORREF m_crYTickColor;
	COLORREF m_crXTickColor;
	COLORREF m_crYLegendTextColor;
	COLORREF m_crXLegendTextColorDate;
	COLORREF m_crXLegendTextColorTime;
	COLORREF m_crGraphTitleColor;
	COLORREF m_crGraphPenColor;
	COLORREF m_crGraphBkColor;
	COLORREF m_crGridColor;
	COLORREF m_crGraphAutostateColor;
	COLORREF m_crGraphAutostateBkColor;
	int m_iGraphPenSize;
	int m_iFontSize;//
	int m_iGraphWidth;//
	int m_iGraphHeight;//
	int m_iGraphX;//location of the fraph within the window
	int m_iGraphY;//location of the graph within the window
	double m_dXAxisMin;// the start value of X
	double m_dYAxisMin;//start value of Y
	double m_dXAxisMax;
	double m_dYAxisMax;
	CWnd *m_pWnd;//parent/owner
	//Helper calculated values - that is to say that these
	//values are not passed in to the graph
	//they are calculated from other given
	CFont m_GraphFont;//default font font created from default fontface, & point size
	int m_iCharHeight;//calculated from the font
	int m_iCharWidth;//calculated from the font
	int m_iOriginX;//location of the origin within the graph
	int m_iOriginY;//location of the origin within the graph
	double m_dPixelsPerY;//scaling
	double m_dPixelsPerX;//scaling	
	int m_iScrollPosX;//
	int m_iScrollPosY;//
	//Data related variables
	G_FUNCTIONSTRUCT_TD *m_pFunctionParams;


	/*short *bufferYvalue;
	short *bufferXvalue;*/
	CPoint *bufferPosValues;
	int m_iCountbuffer;
	int m_iPosbufferX;
	//DWORD m_dwSpeedDelta;

	//bool m_bCursorBtnPushed;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
};

