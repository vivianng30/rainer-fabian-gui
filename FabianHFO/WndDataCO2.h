/**********************************************************************************************//**
 * \file	WndDataCO2.h.
 *
 * Declares the window data co 2 class
 **************************************************************************************************/

#pragma once

#include "MVModel.h"
#include "Bmp.h"
#include "colour.h"
#include "BitmapSlider.h"
#include "KbrdBtn.h"
#include "Diagramm.h"

//global font objects
extern HFONT g_hf14AcuMed;
extern HFONT g_hf43AcuBold;
extern HFONT g_hf17AcuBold;
extern HFONT g_hf10AcuBold;
extern HFONT g_hf11AcuBold;
extern HFONT g_hf7AcuBold;
extern HFONT g_hf8AcuBold;
//extern HFONT g_hf33AcuBold;
//extern HFONT g_hf27AcuBold;

// CWndDataCO2

class CWndDataCO2 : public CWnd
{
	DECLARE_DYNAMIC(CWndDataCO2)

public:
	CWndDataCO2();
	virtual ~CWndDataCO2();

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void Init();
	void Show(bool bShow);

	void UpdateInfoData(bool resetAvailable);
	//void CO2PumpOn();
	void UpdateMessureData(bool bLimits);
	//void UpdateAlarmLimits();

protected:
	void Draw(bool bStatic, bool bLimits);
	void DrawStatic();
	bool DrawLimits();

	void StartCO2DataThread(void);
	void StopCO2DataThread( void );

	CMVModel *getModel();
private:
	CRITICAL_SECTION	csValues;
	CRITICAL_SECTION	csDoThread;

	CMVModel* m_pModel;
	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	HDC		m_hdcStatic;
	HBITMAP m_hbmpStatic;

	CStringW m_csUnit;
	eCO2unit m_eCO2unit;

	CBmp* m_pcLimitHigh_Up;
	CBmp* m_pcLimitLow_Up;

	CBmp* m_pcResetTime_Up;
	CBmp* m_pcResetTime_Dw;

	CBmp* m_pcO2ParaLeft;
	CBmp* m_pcO2ParaRight;


	CKbrdBtn* m_pcResetTime;

	SHORT m_iETCO2;
	//SHORT m_iINSPCO2;
	SHORT m_iRESPRATE;

	DWORD m_dwRemainCO2PumpTime;

	bool m_bCalibrationIsDue;
	bool m_bServiceIsDue;
	bool m_bSystemDateOk;
	bool m_bAutoonPump;

	BYTE m_byExtendedStateBytes;
	BYTE m_byStateBytes;
	BYTE m_byCO2Module;

	bool m_bCO2ValueValid;
	bool m_bETCO2ValueValid;
	bool m_bFreqValuevalid;
	
	static friend UINT CCO2DataThread(LPVOID pc);
	CWinThread*	m_pcwtCO2DataThread;
	DWORD CO2Data(void);
	bool m_bDoThread;
	HANDLE m_hThreadCO2Data;

	bool doThread();
	void startThread();
	void stopThread();

	CBitmapSlider*	m_sliderPassword;
	bool m_bPumpOn;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedResetTimer();

};


