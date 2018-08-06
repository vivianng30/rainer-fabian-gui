/**********************************************************************************************//**
 * \file	ViewFullscreenMsg.h.
 *
 * Declares the view fullscreen message class
 **************************************************************************************************/

#pragma once


#pragma once
#include "MVView.h"
#include "Bmp.h"
#include "PushBtn.h"
//#include "WndAlarmLog.h"
//#include "NumBtn.h"


class CViewFullscreenMsg :public CMVView
{
public:
	CViewFullscreenMsg(int ViewID = 0);
	virtual ~CViewFullscreenMsg(void);

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual bool CreateView();
	virtual bool Initialize();
	virtual void Open();
	virtual void Close();

	virtual void Show();
	virtual void Hide();

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	
	//Observer
	virtual void NotifyEvent(CMVEvent* pEvent);
	//virtual void NotifyViewStateChanged();
	//virtual void NotifyEvent(CMVEvent* pEvent);

	/*virtual void SetViewFocus(){SetFocus();}
	virtual bool SetNextFocus(){return false;}
	virtual bool SetPrevFocus(){return false;}*/

protected:

	void Draw();

protected:

	//CBmp* m_pcLogo;
	CBmp* m_pcCaution;

	CPushBtn* m_pcMenu5;
	CBmp* m_pcBack_Up;
	CBmp* m_pcBack_Dw;

	bool m_bDraw;
	bool m_bI2Cerror;
	bool m_bSERIALerror;

	eViewState m_eViewState;
	

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};





