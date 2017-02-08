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
		NUMB_FLOWOFF		= 11,
		NUMB_FLOWOFF_HFO	= 12
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
	bool CreateWndNumIPPV_Default();
	bool CreateWndNumIPPV_DefaultMech();
	bool CreateWndNumSIPPV_Default();
	bool CreateWndNumSIPPV_DefaultMech();
	bool CreateWndNumPSV_Default();
	bool CreateWndNumPSV_DefaultMech();
	bool CreateWndNumSIMV_Default();
	bool CreateWndNumSIMV_DefaultMech();
	bool CreateWndNumSIMVPSV_Default();
	bool CreateWndNumSIMVPSV_DefaultMech();
	bool CreateWndNumCPAP_Default();
	bool CreateWndNumCPAP_DefaultMech();
	bool CreateWndNumHFO_Default();
	bool CreateWndNumHFO_DefaultMech();
	bool CreateWndNumFlowOff();
	bool CreateWndNumFlowOffHFO();
	bool CreateWndNumTHERAPY_Default();
	bool CreateWndNumDUOPAP_Default();
	bool CreateWndNumNCPAP_Default();

	bool createWndNumConfig(UINT ID, NUMERICINI* pbufNumeric);



	void deleteWndNumIPPV_Default();
	void deleteWndNumIPPV_DefaultMech();
	void deleteWndNumSIPPV_Default();
	void deleteWndNumSIPPV_DefaultMech();
	void deleteWndNumPSV_Default();
	void deleteWndNumPSV_DefaultMech();
	void deleteWndNumSIMV_Default();
	void deleteWndNumSIMV_DefaultMech();
	void deleteWndNumSIMVPSV_Default();
	void deleteWndNumSIMVPSV_DefaultMech();
	void deleteWndNumCPAP_Default();
	void deleteWndNumCPAP_DefaultMech();
	void deleteWndNumHFO_Default();
	void deleteWndNumHFO_DefaultMech();
	void deleteWndNumTHERAPY_Default();
	void deleteWndNumDUOPAP_Default();
	void deleteWndNumNCPAP_Default();
	


	void deleteWndNumIPPV();
	void deleteWndNumSIPPV();
	void deleteWndNumPSV();
	void deleteWndNumSIMV();
	void deleteWndNumSIMVPSV();
	void deleteWndNumCPAP();
	void deleteWndNumHFO();
	void deleteWndNumFlowOff();
	void deleteWndNumFlowOffHFO();
	void deleteWndNumTHERAPY();
	void deleteWndNumDUOPAP();
	void deleteWndNumNCPAP();

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
	void ShowWndNumFlowOff();
	void ShowWndNumFlowOffHFO();
	void ShowWndNumTHERAPY();

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

	CWndNumIPPV_BtB* m_pcWndNumIPPVdefault;
	CWndNumIPPV_Mech* m_pcWndNumIPPVdefaultmech;
	CWndNumSIPPV_Av* m_pcWndNumSIPPVdefault;
	CWndNumSIPPV_Mech* m_pcWndNumSIPPVdefaultmech;
	CWndNumPSV_Av* m_pcWndNumPSVdefault;
	CWndNumPSV_Mech* m_pcWndNumPSVdefaultmech;
	CWndNumSIMVPSV_Av* m_pcWndNumSIMVPSVdefault;
	CWndNumSIMVPSV_Mech* m_pcWndNumSIMVPSVdefaultmech;
	CWndNumSIMV_Av* m_pcWndNumSIMVdefault;
	CWndNumSIMV_Mech* m_pcWndNumSIMVdefaultmech;
	CWndNumCPAP_Av* m_pcWndNumCPAPdefault;
	CWndNumCPAP_Mech* m_pcWndNumCPAPdefaultmech;
	CWndNumAvNCPAP* m_pcWndNumNCPAPdefault;
	CWndNumAvDUOPAP* m_pcWndNumDUOPAPdefault;
	CWndNumAvTHERAPIE* m_pcWndNumTHERAPYdefault;
	CWndNumHFO_Av* m_pcWndNumHFOdefault;
	CWndNumHFO_Mech* m_pcWndNumHFOdefaultmech;
	CWndNumFlowOff_Av* m_pcWndNumFlowOff;
	CWndNumFlowOffHFO_Av* m_pcWndNumFlowOffHFO;

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
