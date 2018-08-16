/**********************************************************************************************//**
 * \file	WndAlarmLog.h.
 *
 * Declares the window alarm log class
 **************************************************************************************************/

#pragma once
#include "globDefs.h"
#include "MVModel.h"
#include "colour.h"
//#include "ColorListBox.h"
//#include "SkinListCtrl.h"
#include "XComboList.h"

class CWndAlarmLog : public CWnd, public CTlsLoglistListener
{
	DECLARE_DYNAMIC(CWndAlarmLog)

public:
	CWndAlarmLog();
	virtual ~CWndAlarmLog();

	bool CreateWnd(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);

	bool Initialize(const RECT rc);

	void Show(bool bShow);


	virtual void AddMessageHead(CStringW sMessage);
	virtual void AddMessageTail(CStringW sMessage);

protected:
	CMVModel *getModel();

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

	//CListBox* pListBox;
	//CSkinListCtrl* pSkinListBox;

	//CColorListBox* pListBox;

	CXComboList *m_pListBox;

	//CWndMenuCalibration *m_pWndMenuCalibration;

	int width;
	bool m_bExit;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	//afx_msg void OnBnClickedSetDateTime();
};



