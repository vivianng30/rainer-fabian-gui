#pragma once
#include "wndwrapper.h"
#include "ViewParaBtn.h"

class CWndWrapperParaBtn :
	public CWndWrapper
{
public:
	CWndWrapperParaBtn(void);
	virtual ~CWndWrapperParaBtn(void);

	//virtual abstract
	bool showView(bool bRedraw=false);
	BOOL destroyView();
	int GetDlgCtrlID();

	void setPmeanDiff(int iValPmean, bool bSend);
	void setPmeanRecDiff(int iValPmeanRec, bool bSend);
	void setCPAPvalue(int iValCPAP, bool bSend);
	void setPEEPvalue(int iValPEEP, bool bSend);
	void setPmanValue(int iValPMAN);
	void setPpsvValue(int iValPPSV,bool bSend);
	void setPinspValue(int iValPINSP);
	void setPmaxValue(int iValPMAX);

protected:
	CViewParaBtn m_vParaBtn;
};
