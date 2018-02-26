/**********************************************************************************************//**
 * \file	ViewPatData.h.
 *
 * Declares the view pattern data class
 **************************************************************************************************/

#pragma once


#pragma once
#include "MVView.h"
#include "Bmp.h"
#include "WndSetupPatient.h"


class CViewPatData :public CMVView
{
public:
	CViewPatData(int ViewID = 0);
	virtual ~CViewPatData(void);

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

	//void Draw();
	
protected:

	bool CreateWndPatient();
	bool DestroyWndPatient();
	void ShowWndPatient(bool bShow);

protected:
	CWndSetupPatient *m_pcWndPatient;

	


public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
};





