#pragma once
#include "stdafx.h"
#include "MVView.h"

class CWndWrapper
{
public:
	CWndWrapper(void);
	virtual ~CWndWrapper(void);

	virtual bool showView(bool bRedraw=false)=0;
	virtual BOOL destroyView()=0;
	virtual int GetDlgCtrlID()=0;

	virtual void PostMessage(UINT Msg,WPARAM wParam=0,LPARAM lParam=0);

protected:
	CMVModel *getModel();

	CMVModel* m_pModel;
	CMVView* pView;
};
