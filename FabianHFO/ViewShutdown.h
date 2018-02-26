/**********************************************************************************************//**
 * \file	ViewShutdown.h.
 *
 * Declares the view shutdown class
 **************************************************************************************************/

#pragma once
#include "MVView.h"
#include "Bmp.h"

// CViewShutdown

class CViewShutdown : public CMVView
{

public:
	CViewShutdown(int ViewID = 0);
	virtual ~CViewShutdown();

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

protected:

	void Draw();

protected:

	CBmp* m_pcLogo;
	//CBmp* m_pcBaby;
	CBmp* m_pcShutdown;
	CBmp* m_pcAccu;



protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
};


