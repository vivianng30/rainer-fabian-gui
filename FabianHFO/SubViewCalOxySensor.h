/**********************************************************************************************//**
 * \file	SubViewCalOxySensor.h.
 *
 * Declares the sub view calendar oxy sensor class
 **************************************************************************************************/

#pragma once
#include "PushBtn.h"
#include "Bmp.h"
#include "MVEvent.h"
#include "MVModel.h"
#include "colour.h"

// CSubViewCalOxySensor

class CSubViewCalOxySensor : public CWnd
{
	DECLARE_DYNAMIC(CSubViewCalOxySensor)

public:
	CSubViewCalOxySensor();
	virtual ~CSubViewCalOxySensor();

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	bool CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void Init();
	void Show(BOOL bShow);

	void ShowOxyBtns(bool disable);
	void EnableOnOFF(BOOL state);

	void DrawO2Sensor(bool bStatic=false, bool bText=false);
	void SetO2SensorText(CStringW& pszTextTop,CStringW& pszTextBot,bool bTwoLines);
	void SetO2SensorValue(int val);
	void RefreshO2SensorValue(int val);

	void SetCalState(eOxySensCalibrationstate state=OS_UNKNOWN,bool bRedraw=false);
	bool CheckOxySensorState(bool bRedraw=false, bool bCalibrating=false);
	void SetCalibration21Ok(bool bCalibrated);
	void SetCalibration100Ok(bool bCalibrated);
	void SetCalRunning(eOxySensorCalRunState state);

	void UpdateCalTime(bool bRedraw=false);

	void StartO21SensorCheckRoutine();
	void StopO21SensorCheckRoutine();

	void StartO100SensorCheckRoutine();
	void StopO100SensorCheckRoutine();

	void StartCheckOxyThread( void );
	void StopCheckOxyThread( void );


	
protected:
	void DrawStatic();
	void DrawCalText();
	CMVModel *getModel();

	static friend UINT CCheckOxyThread(LPVOID pc);
	CWinThread*	m_pcwtCheckOxyThread;
	DWORD CheckOxySensor(void);
	bool m_bDoCheckThread;
	HANDLE m_hThreadCheckOxy;

private:
	CMVModel* m_pModel;
	CRITICAL_SECTION csOxy;

	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	CBmp* m_pcRegionTopLeCorner;
	CBmp* m_pcRegionTopRiCorner;
	CBmp* m_pcRegionBotLeCorner;
	CBmp* m_pcRegionBotRiCorner;

	CBmp* m_pcMenu_Up;
	CBmp* m_pcMenu_Dw;
	CBmp* m_pcMenu_Dis;
	CPushBtn* m_pcO21Cal;
	CPushBtn* m_pcO100Cal;
	CPushBtn* m_pcOxyOnOff;

	HDC		m_hdcStatic;
	HDC		m_hdcTmp;
	HBITMAP m_hbmpStatic;
	HBITMAP m_hbmpTmp;

	TCHAR m_pszTextO2SensorTop[MAX_PATH];
	TCHAR m_pszTextO2SensorBot[MAX_PATH];
	bool m_bTwoLines;
	int m_iO2Value;

	CBmp* m_pcExclamation;
	CBmp* m_pcOk;
	CBmp* m_pcWait;
	CBmp* m_pcWait1;
	CBmp* m_pcWait2;
	CBmp* m_pcWait3;
	CBmp* m_pcWait4;
	CBmp* m_pcWait5;
	CBmp* m_pcWait6;
	CBmp* m_pcWait7;
	CBmp* m_pcWait8;

	int m_iWaitCount;
	bool m_bCalRunning;

	eOxySensCalibrationstate m_eOxyCalState;

	CStringW m_szLast21Cal;
	CStringW m_szLast100Cal;



	bool m_bExit;

	bool m_bCheckO21;
	bool m_bCheckO100;

	bool m_bO21Cal;
	bool m_bO100Cal;
	bool m_bCheckActive;

	bool m_bDoO21Check;
	bool m_bDoO100Check;

	bool m_bDoO21Cal;
	bool m_bDoO100Cal;

	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
};


