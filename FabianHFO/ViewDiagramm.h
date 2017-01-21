#pragma once
#include "MVView.h"
#include "Diagramm.h"
#include "WndDataCO2.h"
#include "WndDataSPO2.h"
#include "WndDataFOT.h"
//#include "WndDataCLFIO2.h"
#include "WndMenuGraphs.h"
#include "SelectGraphBtn.h"
//#include "WndGraphViewSelection.h"


class CViewDiagramm :public CMVView
{
	DECLARE_DYNAMIC(CViewDiagramm)


public:
	CViewDiagramm(int ViewID = 0);
	virtual ~CViewDiagramm(void);

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual bool CreateView();
	virtual bool Initialize();

	virtual void Show(bool bNextGraph=false);
	virtual void Hide();


	virtual void SetViewFocus();
	virtual bool SetNextFocus();
	virtual bool SetPrevFocus();

	void StopDiagramm();

	//Observer
	virtual void NotifyVentModeChanged();
	//virtual void NotifyViewStateChanged();
	virtual void NotifyEvent(CMVEvent* pEvent);
	virtual void NotifyParaBtnEvent(CMVEvent* pEvent);
	virtual void NotifyDataChanged();
	virtual void NotifyAlarmLimitChanged();

	//TCHAR m_pszFontName[32];
	//HFONT CreateFontHandle(CDC* pDC, int nPixHeight, TCHAR* pszFacename, LONG lFontWeight, int iOrientation);

	void setPRICOrunning(bool state);
	void drawSIQofSPO2();
	void drawMeasuredFiO2Value();
	/*void setFIO2LOWvalue(BYTE iFIO2LOWvalue);
	void setFIO2HIGHvalue(BYTE iFIO2HIGHvalue);

	void setSPO2LOWvalue(BYTE iSPO2LOWvalue);
	void setSPO2HIGHvalue(BYTE iSPO2HIGHvalue);*/
	void UpdateLimitData();


protected:
	void StartGraphThread(void);
	void StopGraphThread( void );
	UINT DrawDiagramm();//rku APPERR

	void drawGraphTXT(bool bGraph1, bool bGraph2, bool bGraph3, bool bSIQofSPO2=false);
	void setTextGraph1(CString szText);
	void setTextGraph2(CString szText);
	void setTextGraph3(CString szText);

	void draw15SIQ(HDC hdc);
	void draw30SIQ(HDC hdc);
	void draw45SIQ(HDC hdc);
	void draw60SIQ(HDC hdc);
	void draw75SIQ(HDC hdc);
	void draw90SIQ(HDC hdc);
	void draw100SIQ(HDC hdc);

	bool CreateWndMenuGraphs();
	bool DestroyWndMenuGraphs();
	void ShowWndMenuGraphs(bool bShow);

	bool CreateWndDataFOT(int x, int y, int nWidth, int nHeight);
	bool CreateWndGraphFOT(int x, int y, int nWidth, int nHeight);
	bool CreateWndDataSPO2(int x, int y, int nWidth, int nHeight);
	bool CreateWndGraphSPO2(int x, int y, int nWidth, int nHeight);
	bool CreateWndDataCO2(int x, int y, int nWidth, int nHeight);
	bool CreateWndGraphCO2(int x, int y, int nWidth, int nHeight);
	bool CreateWndGraphPressure(int x, int y, int nWidth, int nHeight);
	bool CreateWndGraphVolume(int x, int y, int nWidth, int nHeight);
	bool CreateWndGraphFlow(int x, int y, int nWidth, int nHeight);
	bool CreateWndLoopPressureVolume(int x, int y, int nWidth, int nHeight);
	bool CreateWndLoopVolumeFlow(int x, int y, int nWidth, int nHeight);

	bool DestroyWndDataFOT();
	bool DestroyWndGraphFOT();
	bool DestroyWndDataSPO2();
	bool DestroyWndGraphSPO2();
	bool DestroyWndDataCO2();
	bool DestroyWndGraphCO2();
	bool DestroyWndGraphPressure();
	bool DestroyWndGraphVolume();
	bool DestroyWndGraphFlow();
	bool DestroyWndLoopPressureVolume();
	bool DestroyWndLoopVolumeFlow();

	void ShowWndDataFOT(bool bShow);
	void ShowWndGraphFOT(bool bShow);
	void ShowWndDataSPO2(bool bShow);
	void ShowWndGraphSPO2(bool bShow);
	void ShowWndDataCO2(bool bShow);
	void ShowWndGraphCO2(bool bShow);
	void ShowWndGraphPressure(bool bShow);
	void ShowWndGraphVolume(bool bShow);
	void ShowWndGraphFlow(bool bShow);
	void ShowWndLoopPressureVolume(bool bShow);
	void ShowWndLoopVolumeFlow(bool bShow);

	/*void ShowGraphViewSelection(bool bState);
	bool CreateWndGraphViewSelection();
	void ShowWndGraphViewSelection();
	void DestroyWndGraphViewSelection();*/

	void drawView(bool bNextGraph=false);
	void drawGraphview(bool bNextGraph=false);
	void drawParaview();

	//void increaseYScaleToNextValue_FOTLOOP();
	void increaseYScaleToNextValue_VOLUME();
	void increaseYScaleToNextValue_FLOW();
	void increaseYScaleToNextValue_PRESSURE();
	void increaseYScaleToNextValue_VOLUMEFLOW();
	void increaseYScaleToNextValue_PRESSUREVOLUME();
	void increaseYScaleToNextValue_CO2();
	void increaseYScaleToNextValue_SPO2();

	//void decreaseYScaleToNextValue_FOTLOOP();
	void decreaseYScaleToNextValue_VOLUME();
	void decreaseYScaleToNextValue_FLOW();
	void decreaseYScaleToNextValue_PRESSURE();
	void decreaseYScaleToNextValue_SPO2();
	void decreaseYScaleToNextValue_CO2();
	void decreaseYScaleToNextValue_VOLUMEFLOW();
	void decreaseYScaleToNextValue_PRESSUREVOLUME();

	void decreaseXScale_VOLUMEFLOW();
	void decreaseXScale_PRESSUREVOLUME();
	//void decreaseXScale_FOTLOOP();

	//void increaseXScaleToNextValue_FOTLOOP();
	void increaseXScaleToNextValue_VOLUMEFLOW();
	void increaseXScaleToNextValue_PRESSUREVOLUME();

	void setGraphSpeed(UINT iSpeed);
	UINT getGraphSpeed();
	
	void IncreaseGraphSpeed();
	void DecreaseGraphSpeed();



	void SetXAxisTimeTickSpaceOfDiagramms();
	int GetXAxisTimeTickSpace();
	
	void UpdateTimeAxis(bool bRedraw);
	int GetTimeAxisMaxScale();

	

	int GetCurrentItemsToMerge();

	void CopyCurBreathToSavedBreath();

	void drawFOTsteps();
	int drawSPO2Graph(int iNumItems);
	int drawCO2Graph(int iNumItems);
	//int DrawGraphs(int iNumItems);
	int drawPressureGraph(int iNumItems);
	int drawFlowGraph(int iNumItems);
	int drawVolumeGraph(int iNumItems);

	void DrawLoops(int iNumItems);
	//void drawPressureVolumeLoop(int iNumItems);
	//void drawVolumeFlowLoop(int iNumItems);

	//void DrawSavedLoops();
	void drawSavedPressureVolumeLoop();
	void drawSavedVolumeFlowLoop();

	
	int redrawPressureGraph();
	int redrawFlowGraph();
	int redrawVolumeGraph();
	int redrawCO2Graph();
	int redrawSPO2Graph();
	//int redrawFOTLoop();
	void redrawPressureVolumeLoop();
	void redrawVolumeFlowLoop();

	//void refreshLastBreathOfLoops();
	void refreshLastBreathOfPressureVolumeLoop();
	void refreshLastBreathOfVolumeFlowLoop();


	void DrawCursor(int iDiagramm);

	void setCurrentXtimevalGraphs(int iVal);
	int getCurrentXtimevalGraphs();
	void resetCurrentXtimevalGraphs();
	/*int IncreaseCurrentXtimevalGraphs();
	int DecreaseCurrentXtimevalGraphs();*/

	/*void SetCurrentXtimevalCLFIO2(int iVal);
	int GetCurrentXtimevalCLFIO2();
	void ResetCurrentXtimevalCLFIO2();
	int IncreaseCurrentXtimevalCLFIO2();
	int DecreaseCurrentXtimevalCLFIO2();*/

	//void resetAllXtimeval();

	void resetAllDiagramms();
	void resetSPO2Diagramm();
	void resetCO2Diagramm();
	void resetPressureDiagramm();
	void resetFlowDiagramm();
	void resetVolumeDiagramm();

	void SaveLoops();

	bool checkForRedraw();
	bool checkForScale();
	bool checkForResetDiagramms();
	UINT CalculateSPIItemsToDraw();
	
	bool drawHFOVGarantyLine();
	bool drawVGarantyLine();
	bool IsVLimitStateOn();
	//void setPSVapnoe(bool bState);

	void setDrawVolLimit(bool bState);
	bool getDrawVolLimit();
	void setValueVolLimit(double val);
	double getValueVolLimit();

	void setDrawVolGaranty(bool bState);
	bool getDrawVolGaranty();
	void setValueVolGaranty(double val);
	double getValueVolGaranty();

	void setDrawHFOVolGaranty(bool bState);
	bool getDrawHFOVolGaranty();
	void setValueVolGarantyHigh(double val);
	double getValueVolGarantyHigh();
	void setValueVolGarantyLow(double val);
	double getValueVolGarantyLow();
	bool CalculateHFOVolGaranty();

	/*void setResetCurrentXaxes();
	void deleteResetCurrentXaxes();
	bool getResetCurrentXaxes();*/

	void deleteUpdateRingBufCopy();
	bool getUpdateRingBufCopy();
	void setUpdateRingBufCopy();

	void setSizeReadInBufferSPI(int iSPIdata);
	int getSizeReadInBufferSPI();

	void resetRedrawDiagram();
	void setRedrawDiagram(WORD diagram);
	WORD getRedrawDiagram();

	void setSizeCurrentBreath(int iSizeBreath);
	int getSizeCurrentBreath();

	//void setCollectdataFOTtime(BYTE iCountFOTimer);
	void setFOTtime(BYTE iCountFOTimer);
	void setFOTrunning(bool state);
	void updateFOTPmeanPara();
	void updateFOTPEEPPara();
public:
	void RedrawDiagrammData(int iDiagrammID);

	void UpdateCO2InfoData(bool resetAvailable);
protected:
	static CRITICAL_SECTION	csSPIReadInBuffer;
	static CRITICAL_SECTION	csCurrentBreath;
	static CRITICAL_SECTION	csSpeed;
	static CRITICAL_SECTION	csXValue;
	static CRITICAL_SECTION	csVolLimit;
	static CRITICAL_SECTION	csVolGarant;
	static CRITICAL_SECTION	csThreadAccess;
	static CRITICAL_SECTION	csExit;
	static CRITICAL_SECTION	csRedrawDiagram;

	static CRITICAL_SECTION	csDoThread;

	DWORD m_dwLastSpeedchange;

	CSelectGraphBtn* m_pcGraph1;

	CBmp* m_pcGraphArrow_Up;
	CBmp* m_pcGraphArrow_Fc;

	CBmp* m_pcBargraphSIQofSPO2;

	CString m_szGraph1;
	CString m_szGraph2;
	CString m_szGraph3;

	CWndMenuGraphs *m_pWndMenuGraphs;
	//CWndGraphViewSelection *m_pcWndGraphViewSelection;

	static CRITICAL_SECTION	csDrawDataFOT;
	static CRITICAL_SECTION	csDrawDataSPO2;
	static CRITICAL_SECTION	csDrawDataCO2;
	CWndDataFOT *m_pDataFOT;
	CWndDataCO2 *m_pDataCO2;
	CWndDataSPO2 *m_pDataSPO2;

	static CRITICAL_SECTION	csDiagrammPRESSURE;
	static CRITICAL_SECTION	csDiagrammFLOW;
	static CRITICAL_SECTION	csDiagrammVOLUME;
	static CRITICAL_SECTION	csDiagrammLOOP;
	static CRITICAL_SECTION	csDiagrammSPO2;
	static CRITICAL_SECTION	csDiagrammCO2;
	static CRITICAL_SECTION	csDiagrammFOT;
	CDiagramm *m_pLoopFOT;
	CDiagramm *m_pDiagrammCO2;
	CDiagramm *m_pDiagrammSPO2;
	CDiagramm *m_pDiagrammPressure;
	CDiagramm *m_pDiagrammVolume;
	CDiagramm *m_pDiagrammFlow;
	CDiagramm *m_pLoopPressureVolume;
	CDiagramm *m_pLoopVolumeFlow;


	UINT m_iItemsToMerge;

	int m_iCurrentLineDiagramm;
	int m_iCurFocusedWnd;
	bool m_bGraphSelected;

	bool m_bDataFOTIsActive;
	bool m_bLoopFOTIsActive;
	bool m_bDataSPO2IsActive;
	bool m_bGraphSPO2IsActive;
	bool m_bDataCO2IsActive;
	bool m_bGraphCO2IsActive;
	bool m_bGraphPressureIsActive;
	bool m_bGraphVolumeIsActive;
	bool m_bGraphFlowIsActive;
	bool m_bLoopPressureVolumeIsActive;
	bool m_bLoopVolumeFlowIsActive;

	UINT m_iPrevXvalPressure;
	UINT m_iPrevXvalVolume;
	UINT m_iPrevXvalFlow;

	static LPG_PLOTITEMSSTRUCT_LD m_pPlotItemsFOTloop;
	static LPG_FUNCTIONSTRUCT_LD m_lpfsFOT;

	static LPG_FUNCTIONSTRUCT_LD m_lpfsSPO2;
	static LPG_FUNCTIONSTRUCT_LD m_lpfsCO2;
	static LPG_FUNCTIONSTRUCT_LD m_lpfsPressure;
	static LPG_FUNCTIONSTRUCT_LD m_lpfsFlow;
	static LPG_FUNCTIONSTRUCT_LD m_lpfsVolume;

	static LPG_PLOTITEMSSTRUCT_LD m_pPlotItemsPVloop;
	static LPG_FUNCTIONSTRUCT_LD m_lpfsPressureVolume;

	static LPG_PLOTITEMSSTRUCT_LD m_pPlotItemsVFloop;
	static LPG_FUNCTIONSTRUCT_LD m_lpfsVolumeFlow;

	static LPG_PLOTITEMSSTRUCT_LD m_pPlotItems;

	friend UINT CGraphThread(LPVOID pc);
	CWinThread*	m_pcwtGraphThread;
	bool m_bDoThread;
	HANDLE m_hThreadGraph;

	bool doThread();
	void startThread();
	void stopThread();

	bool m_bExit;
	bool isExit();
	void setExit();

	UINT m_iGraphSpeedGraph;

	int m_iCurrentXvalGraphs;

	bool m_bDrawVolumeLimit;
	double m_dbValueVolumeLimit;

	bool m_bDrawVolumeGaranty;
	double m_dbValueVolumeGaranty;

	bool m_bDrawHFOVolumeGaranty;
	double m_dbHFOValueVolumeGarantyHigh;
	double m_dbHFOValueVolumeGarantyLow;

	int m_iTrigThreshold;

	bool m_bResetXaxe;
	bool m_bFreeze;
	bool m_bFreezeNextBreath;
	bool m_bDrawSavedLoops;

	bool m_bFilledGraph;

	static PBUFSPI* m_pbufSPIReadInBuffer;
	int m_iSizeReadInBufferSPI;

	static CircularBuffer<PBUFSPI> m_cRingbuf;

	static PBUFSPI* m_pbufCurrentBreath;
	int m_iSizeCurrentBreath;
	
	bool m_bDecreaseGraphSpeed;
	bool m_bIncreaseGraphSpeed;

	WORD m_iDiagramToRedraw;

	bool m_bRefreshLastBreathOfLoops;
	bool m_bUpdateRingBufCopy;

	bool m_bResetAllDiagramms;


public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg void OnSetFocus(CWnd* pOldWnd);
	//afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnBnClickedNextGraph();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};



