/**********************************************************************************************//**
 * \file	ViewLog.h.
 *
 * Declares the view log class
 **************************************************************************************************/

#pragma once
#include "MVView.h"
#include "WndAlarmLog.h"


class CViewLog :public CMVView
{
public:
	CViewLog(int ViewID = 0);
	virtual ~CViewLog(void);

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual bool CreateView();
	virtual bool Initialize();
	virtual void Open();
	virtual void Close();

	virtual void Show();
	virtual void Hide();

	/*virtual void SetViewFocus(){SetFocus();}
	virtual bool SetNextFocus(){return false;}
	virtual bool SetPrevFocus(){return false;}*/
	
	//virtual void Trigger(int btnID=0,int iGroupId=0);
	//virtual void SetFocusTo(int iID);

	//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);

	//Observer
	//virtual void NotifyEvent(CMVEvent* pEvent);

	/*virtual void AddMessage(CStringW sMessage);

	/*int AddString(LPCTSTR s);
	int InsertString(int i, LPCTSTR s);
	void ResetContent();
	int DeleteString(int i);*/

protected:

	bool CreateWndAlarmLog();
	bool DestroyWndAlarmLog();
	void ShowWndAlarmLog(bool bShow);

	//void SetHorizontalExtent(/*CListBox& ListBox*/);

	//void updateWidth(LPCTSTR s);
	

protected:
	/*HDC			m_hDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hBmpPrev;
	LONG		m_lX;
	LONG		m_lY;
	LONG		m_lCx;
	LONG		m_lCy;*/

	//CListBox* pListBox;

	CWndAlarmLog *m_pWndAlarmLog;

	//int width;


public:
	DECLARE_MESSAGE_MAP()
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
};

