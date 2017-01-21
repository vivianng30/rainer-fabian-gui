//=============================================================================
/** 
* \file WndDataSPO2.h
* \class CWndDataSPO2
* \brief Window for SPO2 numeric data.
* \author Rainer Kuehner
* \date 2013-02-05 Creation of file
*
* Displays the numeric measurement data of SPO2 module.
*
*       Copyright (c) 2013 Ing.Buero Rainer Kuehner
*                      Illerkirchberg
*
**/
//=============================================================================
#pragma once

#include "MVModel.h"
#include "Bmp.h"
#include "colour.h"
#include "UpDwnBtn.h"
#include "Diagramm.h"
#include "LimitBtn_FIO2high.h"
#include "LimitBtn_FIO2low.h"
#include "LimitBtn_SPO2low.h"
#include "LimitBtn_SPO2high.h"

extern HFONT g_hf34BoldNum;
extern HFONT g_hf14AcuMed;
extern HFONT g_hf14AcuBold;
extern HFONT g_hf43AcuBold;
extern HFONT g_hf17AcuBold;
extern HFONT g_hf10AcuBold;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf8AcuBold;
//extern HFONT g_hf9AcuBold;

// CWndDataSPO2

class CWndDataSPO2 : public CWnd
{
	DECLARE_DYNAMIC(CWndDataSPO2)

public:
	CWndDataSPO2();
	virtual ~CWndDataSPO2();

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void Init();
	void Show(bool bShow);

	void setSPO2LOWvalue(BYTE iSPO2LOWvalue);
	void setSPO2HIGHvalue(BYTE iSPO2HIGHvalue);
	void setPRICOrunning(bool state);

	void setFIO2LOWvalue(BYTE iFIO2LOWvalue);
	void setFIO2HIGHvalue(BYTE iFIO2HIGHvalue);
	void drawMeasuredFiO2Value();
	void UpdateMessureData();
	void UpdateLimitData();
	//void UpdateSPO2Range();
	//void SetOneButtonDepressed(int btnID);
	void SetAllButtonUnpressed();
	bool isLimitBtnDepressed();
	//void UpdateAlarmLimits();
protected:
	void Draw(bool bStatic, bool bValues, bool bPRICOSpO2, bool bPRICOFiO2);
	void DrawStatic();
	void DrawValues();
	//void DrawLimits(CDC* pDC);
	void drawPRICOSpO2();
	void drawPRICOFiO2();
	void enablePRICObtn();
	void disablePRICObtn();

	void checkPRICOstate();

	void StartSPO2DataThread(void);
	void StopSPO2DataThread( void );

	

	CMVModel *getModel();
private:
	CRITICAL_SECTION	csDraw;
	CRITICAL_SECTION	csDoThread;

	CMVModel* m_pModel;
	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	CPen penLine;//rkuNEWFIX
	CPen penBlueVal;//rkuNEWFIX

	HDC		m_hdcStatic;
	HBITMAP m_hbmpStatic;

	CBmp* m_pcLimitHigh_Up;
	CBmp* m_pcLimitLow_Up;

	CBmp* m_pcSPO2ParaLeftTop;
	CBmp* m_pcSPO2ParaRightTop;
	CBmp* m_pcSPO2ParaLeftBot;
	CBmp* m_pcSPO2ParaRightBot;

	CUpDwnBtn* m_pcPRICO_OnOff;
	CBmp* m_pcPRICO_OnOff_Up;
	CBmp* m_pcPRICO_OnOff_Dw;
	CBmp* m_pcPRICO_OnOff_Dis;

	CList<CLimitBtn* , CLimitBtn *> m_plLimitBtn;

	CLimitBtn_SPO2low* m_pcLimitBtn_SPO2low;
	CLimitBtn_SPO2high* m_pcLimitBtn_SPO2high;
	CLimitBtn_FIO2low* m_pcLimitBtn_FIO2low;
	CLimitBtn_FIO2high* m_pcLimitBtn_FIO2high;

	CBmp* m_pcLimit_LeUp;
	CBmp* m_pcLimit_LeDw;
	CBmp* m_pcLimit_RiUp;
	CBmp* m_pcLimit_RiDw;

	friend UINT CSPO2DataThread(LPVOID pc);
	CWinThread*	m_pcwtSPO2DataThread;
	DWORD SPO2Data(void);
	bool m_bDoThread;
	HANDLE m_hThreadSPO2Data;

	bool doThread();
	void startThread();
	void stopThread();

	SHORT m_iSPO2;
	SHORT m_iPulseRate;
	SHORT m_iPerfusionIndex;
	SHORT m_iEXCEPTIONS2;
	SHORT m_iFiO2measuredValue;
	BYTE m_iFIO2HIGHvalue;
	BYTE m_iFIO2LOWvalue;

	BYTE m_iSPO2HIGHvalue;
	BYTE m_iSPO2LOWvalue;

	int m_iAlimitSPO2max;
	int m_iAlimitSPO2min;

	bool m_bProcessing;
	bool m_bSpO2Out;
	bool m_bSpO2blink;

protected:
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


