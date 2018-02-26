/**********************************************************************************************//**
 * \file	WndServiceMenu.h.
 *
 * Declares the window service menu class
 **************************************************************************************************/

#pragma once
#include "globDefs.h"
#include "colour.h"
#include "WndService.h"
#include "MenuBtn.h"
// CWndServiceMenu

class CWndServiceMenu : public CWndService
{
	DECLARE_DYNAMIC(CWndServiceMenu)

public:
	CWndServiceMenu();
	virtual ~CWndServiceMenu();

	void Init();


	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void SetOneButtonDepressed(int btnID);
	void SetAllButtonUnpressed();
	void SetOneButtonFocused(int btnID);
	void SetNextButtonFocused();
	void SetPrevButtonFocused();

	
	
protected:
	//void Draw();

private:
	
	// Attributes
	/*HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lXo;
	LONG		m_lYo;*/

	CMenuBtn* m_pcMenuSystem;
	CMenuBtn* m_pcMenuCalibration;
	CMenuBtn* m_pcMenuFlowboard;
	CMenuBtn* m_pcMenuMonitorPIC;
	CMenuBtn* m_pcMenuAccuboard;
	CMenuBtn* m_pcMenuValves;
	CMenuBtn* m_pcMenuDisplay;
	CMenuBtn* m_pcMenuSettings;
	CMenuBtn* m_pcMenuInterfaces;
	CMenuBtn* m_pcMenuNetwork;
	CMenuBtn* m_pcMenuLicensing;
	CMenuBtn* m_pcMenuMaintenance;

	CMenuBtn* m_pcMenuBACK;
	
	

	CBmp* m_pcMenu_UP;
	CBmp* m_pcMenu_DW;
	CBmp* m_pcMenu_FC;
	CBmp* m_pcMenu_DIS;

	CBmp* m_pcMenu_BACK_UP;
	CBmp* m_pcMenu_BACK_DW;
	//CBmp* m_pcMenu_EXIT_FC;

	CList<CMenuBtn* , CMenuBtn *> m_plMenuBtn;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedSystem();
	afx_msg void OnBnClickedCalibration();
	afx_msg void OnBnClickedFlowboard();
	afx_msg void OnBnClickedMonitorPIC();
	afx_msg void OnBnClickedAccuboard();
	afx_msg void OnBnClickedValves();
	afx_msg void OnBnClickedDisplay();
	afx_msg void OnBnClickedSettings();
	afx_msg void OnBnClickedInterfaces();
	afx_msg void OnBnClickedNetwork();
	afx_msg void OnBnClickedLicensing();
	afx_msg void OnBnClickedMaintenance();
	afx_msg void OnBnClickedBACK();
	afx_msg void OnPaint();
};

