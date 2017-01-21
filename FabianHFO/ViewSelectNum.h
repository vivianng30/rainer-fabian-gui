#pragma once
#include "MVView.h"
#include "WndMenuSelectNum.h"

class CViewSelectNum :public CMVView
{
public:
	CViewSelectNum(int ViewID = 0);
	virtual ~CViewSelectNum(void);

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual bool CreateView();
	virtual bool Initialize();
	virtual void Open();
	virtual void Close();

	virtual void Show();
	virtual void Hide();


	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual void SetViewFocus();
	virtual bool SetNextFocus();
	virtual bool SetPrevFocus();

	//Observer
	//virtual void NotifyEvent(CMVEvent* pEvent);
	//virtual void NotifyLanguageChanged();
	//virtual void NotifyDataChanged();
	//virtual void NotifyVentModeChanged();
	//virtual void NotifyViewStateChanged();


protected:
	bool CreateWndMenuSelNum();
	bool DestroyWndMenuSelNum();
	void ShowWndMenuSelNum(bool bShow);

protected:
	CWndMenuSelectNum *m_pWndMenuSelNum;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


