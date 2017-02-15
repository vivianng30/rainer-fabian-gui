#pragma once
#include "MVView.h"
//#include "WndNumAv.h"
#include "WndNumCPAP_Av.h"
#include "WndNumCPAP_Mech.h"
#include "WndNumAvDUOPAP.h"
#include "WndNumAvNCPAP.h"
#include "WndNumPSV_Av.h"
#include "WndNumPSV_Mech.h"
#include "WndNumSIMVPSV_Av.h"
#include "WndNumSIMVPSV_Mech.h"
#include "WndNumFlowOff_Av.h"
#include "WndNumHFO_Av.h"
#include "WndNumHFO_Mech.h"
//#include "WndNumSavedBreathMech.h"
#include "WndNumIPPV_BtB.h"
#include "WndNumIPPV_Mech.h"
#include "WndNumSIPPV_Av.h"
#include "WndNumSIPPV_Mech.h"
#include "WndNumSIMV_Av.h"
#include "WndNumSIMV_Mech.h"
#include "WndNumFlowOffHFO_Av.h"
#include "WndNumAvTHERAPIE.h"

#include "WndNumConfig.h"

#include "PushBtn.h"

/////////////////////////////////////////////////////////////////////////////
typedef struct tagNUMWNDATTRIB
{
	int			nX;
	int			nY;
	int			nCx;
	int			nCy;
}NUMWNDATTRIB;


class CViewNumericWnd :public CMVView
{
	//Mode of Ventilator
	enum eNumBlock {
		NUMB_NONE			= 0,
		NUMB_IPPV			= 1,
		NUMB_SIPPV			= 2,
		NUMB_SIMV			= 3,
		NUMB_SIMVPSV		= 4,
		NUMB_PSV			= 5,
		NUMB_CPAP			= 6, 
		NUMB_NCPAP			= 7,
		NUMB_DUOPAP			= 8,
		NUMB_HFO			= 9,
		NUMB_THERAPY		= 10,
		NUMB_FLOWOFFCONV	= 11,
		NUMB_FLOWOFFHFO		= 12
	};
public:
	CViewNumericWnd(int ViewID = 0);
	virtual ~CViewNumericWnd(void);

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
	//virtual BOOL PreTranslateMessage(MSG* pMsg);

	//Observer
	virtual void NotifyViewStateChanged();
	virtual void NotifyVentModeChanged();
	virtual void NotifyParaBtnEvent(CMVEvent* pEvent);
	//virtual void NotifyLanguageChanged();
	virtual void NotifyDataChanged();
	virtual void NotifyExspirationDataChanged();
	virtual void NotifyAlarmLimitChanged();
	virtual void NotifyCalculateAlarmLimit();
	virtual void NotifyEvent(CMVEvent* pEvent);

	//void SetOxyHourglass(bool state);

	//void SetFlowData(int valueInsp, int valueExsp);

	//bool isGraphCursor();

protected:
	

	bool createWndNumConfig(UINT ID, NUMERICINI* pbufNumeric);

	void deleteWndNumConfig();
	

	void ShowWndNumIPPV();
	void ShowWndNumSIPPV();
	void ShowWndNumSIMV();
	void ShowWndNumSIMVPSV();
	void ShowWndNumPSV();
	void ShowWndNumCPAP();
	void ShowWndNumDUOPAP();
	void ShowWndNumNCPAP();
	void ShowWndNumHFO();
	void ShowWndNumTHERAPY();
	void ShowWndNumFLOWOFFCONV();
	void ShowWndNumFLOWOFFHFO();

	//void showWndNumConfig();


	void redrawNumerics(bool bData, bool bFrames, bool bText, bool bLimits, bool bFlowmeter);;

	void DrawGraphCursor(int iDiagrmm);
	void DeleteGraphCursor();

	void ShowNextNumericWnd();
	void SetNumericBlock(bool bChangeCurNumBlock);
	/*void ShowNumWnd(int iNumWnd);
	void HideNumWnd(int iNumWnd);*/

	void ShowNumWnd(eNumBlock eBlock,bool bChangeCurNumBlock);
	void HideNumWnd();

	void RedrawAlarmLimits();
	
	void CalculateAlarmLimit();

	void BnMenuNUMERIC();

	

	//void WriteCurrentNumBlock();

protected:
	CRITICAL_SECTION	csNumWnd;
	
	CPushBtn* m_pcMenuNumeric;

	CBmp* m_pcNumeric_Up;
	CBmp* m_pcNumeric_Dw;

	CWndNumConfig* m_pcWndNumConfig;


	NUMWNDATTRIB m_nwa;

	eNumBlock m_eNumBlock;

	int m_iCurrWnd;

	BYTE m_iCountNumericBlock;
	BYTE m_iCurNumericBlock;
	bool m_bExit;
	bool m_bBackState;
	



public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
};
