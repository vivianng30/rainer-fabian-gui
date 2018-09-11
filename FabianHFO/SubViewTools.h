/**********************************************************************************************//**
 * \file	SubViewTools.h.
 *
 * Declares the sub view tools class
 **************************************************************************************************/

#pragma once
#include "globDefs.h"
#include "MVModel.h"
//#include "PushBtn.h"
#include "KbrdBtn.h"
#include "colour.h"
#include "WndWaitHourglass.h"

// CSubViewTools

extern CEvent eventWaitTrendUSBfinish;

class CSubViewTools : public CWnd
{
	DECLARE_DYNAMIC(CSubViewTools)

public:
	CSubViewTools();
	virtual ~CSubViewTools();
	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	bool CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	void Show(bool bShow);


protected:
	void Initialize();
	void Draw();

	CMVModel *getModel();

	void StartWaitTrendUSBThread(void);
	void StopWaitTrendUSBThread(void);

	bool CreateWndHourglass();
	void ShowWndHourglass(bool bShow);
	bool DestroyWndHourglass();

	static friend UINT CWaitTrendUSBThread(LPVOID pc);
	CWinThread*	m_pcwtWaitTrendUSBThread;
	DWORD WaitTrendUSB(void);
	bool m_bDoWaitTrendUSBThread;
	HANDLE m_hThreadWaitTrendUSB;

private:
	CMVModel* m_pModel;

	// Attributes
	HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;

	CWndWaitHourglass *m_pWndHourglass;

	CBmp* m_pcRegionTopLeCorner;
	CBmp* m_pcRegionTopRiCorner;
	CBmp* m_pcRegionBotLeCorner;
	CBmp* m_pcRegionBotRiCorner;

	CDialog*   m_pDlg;
	MAINBOARD_DATA m_dataMainboard;

	CBmp* m_pcTrendToUSB_Up;
	CBmp* m_pcTrendToUSB_Dw;
	CKbrdBtn* m_pcTrendToUSB;
	CKbrdBtn* m_pcLogToUSB;
	CKbrdBtn* m_pcDevInfoToUSB;

	bool m_bUSBavailable;

	bool m_bExit;

	bool m_bSavingTrendToUSB;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedTrendToUSB();
	afx_msg void OnBnClickedLogToUSB();
	afx_msg void OnBnClickedDevInfoToUSB();
	
};


