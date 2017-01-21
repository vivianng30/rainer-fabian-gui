#pragma once
#include "MVView.h"
#include "UpDwnBtn.h"
//#include "Library/SelectGraphBtn.h"
#include "ParaBtn.h"
#include "ParaBtn_RISETIME.h"
//#include "ParaBtn_PLATEAU.h"
#include "ParaBtn_PINSP.h"
#include "ParaBtn_PMITT.h"
#include "ParaBtn_PPSV.h"
#include "ParaBtn_PDUO.h"
#include "ParaBtn_VLIMIT.h"
#include "ParaBtn_IFLOW.h"
//#include "ParaBtn_EFLOW.h"
#include "ParaBtn_PEEP.h"
#include "ParaBtn_TRIGGER.h"
#include "ParaBtn_TRIGGERNMODE.h"
#include "ParaBtn_BPM.h"
#include "ParaBtn_HFAMPL.h"
#include "ParaBtn_HFFREQ.h"
#include "ParaBtn_ITIME.h"
#include "ParaBtn_ETIME.h"
#include "ParaBtn_O2.h"
#include "ParaBtn_VGARANT.h"
#include "ParaBtn_BACKUP.h"
#include "ParaBtn_CPAP.h"
#include "ParaBtn_FLOWMIN.h"
#include "WndMenuVentState.h"
#include "ParaBtn_O2Flush.h"
#include "ParaBtn_PMANUAL.h"
#include "ParaBtn_IERatio.h"
#include "ParaBtn_PMEAN_REC.h"
#include "ParaBtn_ITIME_REC.h"
#include "ParaBtn_FREQ_REC.h"
#include "ParaBtn_HFFLOW.h"
#include "ParaBtn_FLOW.h"
#include "ParaBtn_PMAX.h"
#include "ParaBtn_AMPMAX.h"
#include "SubParaBtn.h"
#include "WndParaSettings.h"
//#include "CustomSlider.h"

//TEST
//#include "UDBtn.h"

//#include "OCArray.h"

class CViewParaBtn :public CMVView
{
public:
	CViewParaBtn(int ViewID = 0);
	virtual ~CViewParaBtn(void);

	virtual BOOL Create(CWnd* pParentWnd, const RECT rc, UINT nID, CCreateContext* pContext = NULL);

	virtual bool CreateView();
	virtual bool Initialize();
	virtual void Open();
	virtual void Close();

	virtual void Show();
	virtual void Hide();

	virtual void SetViewFocus();
	virtual bool SetNextFocus();
	virtual bool SetPrevFocus();

	void ShowParaSettings(bool bState);
	void ShowParaBtn(bool bVentModeChanged=false);
	void HideParaBtn();
	void Draw();
	
	//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//Observer
	virtual void NotifyVentModeChanged();
	virtual void NotifyParaBtnEvent(CMVEvent* pEvent);
	virtual void NotifyEvent(CMVEvent* pEvent);
	virtual void NotifyViewFocusChanged(int iViewID);
	virtual void NotifyDataChanged();

	void refreshO2Flush();
	void SetO2FlushTime(int iO2FlushTime);

	void SetTriggeredBreath();
	void SetOxyHourglass(bool state);

	void setPSVapnoe(bool bState);
	void setPRICOrunning(bool state);
	

	void setOxyDiffParaBTN(int iValOxy);//PRICO03
	//void setOxyPPRICO(int iValOxy);//PRICO04
	
	//void setPmeanHFO(int iValPmean, bool bSendPmeanDif, bool bSendPmanual);//PMAN1
	//void setPmanualHFO(int iValPmean, bool bSendPmanual);//PMAN1
	void setPmeanDiff(int iValPmean, bool bSend);//PMAN1
	void setPmeanRecDiff(int iValPmeanRec, bool bSend);//PMAN1

	void setPEEPvalue(int iValPEEP, bool bSend);
	void setPpsvValue(int iValPPSV, bool bSend);

	//void setPMAXVG(SHORT iValPMAXVG);//newVG

	SHORT getPpsvDisplayedValue();
	SHORT getPEEPdisplayedValue();

	SHORT getITimeDisplayedValue();
protected:

	void SetOneButtonDepressed(int btnID);
	void SetAllButtonUnpressed();

	bool CreateWndMenuVentState();
	void ShowWndMenuVentState(bool state);

	bool CreateWndParaSettings();
	void ShowWndParaSettings();
	void DestroyWndParaSettings();

	void load_IPPVmode();
	void show_IPPVmode();
	void load_PRE_IPPVmode();
	void show_PRE_IPPVmode();

	void load_SIPPVmode();
	void show_SIPPVmode();
	void load_PRE_SIPPVmode();
	void show_PRE_SIPPVmode();

	void load_SIMVmode();
	void show_SIMVmode();
	void load_PRE_SIMVmode();
	void show_PRE_SIMVmode();

	void load_PSVmode();
	void show_PSVmode();
	void load_PRE_PSVmode();
	void show_PRE_PSVmode();

	void load_SIMVPSVmode();
	void show_SIMVPSVmode();
	void load_PRE_SIMVPSVmode();
	void show_PRE_SIMVPSVmode();

	void load_CPAPmode();
	void show_CPAPmode();
	void load_PRE_CPAPmode();
	void show_PRE_CPAPmode();

	void load_NCPAPmode();
	void show_NCPAPmode();
	void load_PRE_NCPAPmode();
	void show_PRE_NCPAPmode();

	void load_DUOPAPmode();
	void show_DUOPAPmode();
	void load_PRE_DUOPAPmode();
	void show_PRE_DUOPAPmode();

	void load_THERAPYmode();
	void show_THERAPYmode();
	void load_PRE_THERAPYmode();
	void show_PRE_THERAPYmode();

	void load_HFOmode();
	void show_HFOmode();
	void load_PRE_HFOmode();
	void show_PRE_HFOmode();

	void load_FIO2FLUSHpara(bool bPRESET);
	void load_FIO2para(bool bPRESET);
	void load_IFLOWpara(bool bTrigger,bool bPRESET);
	void load_RISETIMEpara(bool bTrigger,bool bPRESET);
	void load_PEEPpara(bool bTrigger,bool bPRESET);
	void load_PPSVpara(bool bSIMV, bool bPRESET);
	void load_PMAXpara(bool bTrigger,bool bPRESET);
	void load_HF_PMAXpara(bool bPRESET);
	void load_PINSPpara(bool bTrigger,bool bPRESET);
	void load_VLIMITpara(bool bTrigger,bool bPRESET);
	void load_VGUARANTpara(bool bTrigger,bool bPRESET);
	void load_HF_VGUARANTpara(bool bPRESET);
	void load_ETIMEpara(bool bTrigger,bool bPRESET);
	void load_ETIMENMODEpara(bool bPRESET);
	void load_BPMpara(bool bTrigger,bool bPRESET);
	void load_BPMNMODEpara(bool bPRESET);
	void load_ITIMEpara(bool bTrigger,bool bPRESET);
	void load_ITIMENMODEpara(bool bPRESET);
	void load_ITIMERECpara(bool bPRESET);
	void load_TRIGGERCONVpara(bool bPRESET);
	void load_TRIGGERNMODEpara(bool bPRESET);
	void load_FLOWMINpara(bool bPRESET);
	void load_CPAPpara(bool bPRESET);
	void load_BACKUPpara(bool bPRESET);
	void load_NCPAP_CPAPpara(bool bPRESET);
	void load_DUOPAP_CPAPpara(bool bPRESET);
	void load_PMANUALpara(bool bNMODE,bool bPRESET);
	void load_HF_PMANUALpara(bool bPRESET);
	void load_PDUOpara(bool bPRESET);
	void load_FLOWpara(bool bPRESET);
	void load_HF_PMITTpara(bool bPRESET);
	void load_HFAMPLpara(bool bPRESET);
	void load_HFFREQpara(bool bPRESET);
	void load_HFFLOWpara(bool bPRESET);
	void load_HF_PMEANRECpara(bool bPRESET);
	void load_HF_IERatiopara(bool bPRESET);

protected:
	CRITICAL_SECTION	csDraw;

	bool m_bExit;
	
	CBmp* m_pcParaGroup_UpT;
	CBmp* m_pcParaGroup_FcT;
	CBmp* m_pcParaGroup_DwT;
	CBmp* m_pcParaGroup_SigT;
	CBmp* m_pcParaGroup_UpB;
	CBmp* m_pcParaGroup_FcB;
	CBmp* m_pcParaGroup_DwB;
	CBmp* m_pcParaGroup_SigB;

	//VG_VL_NEW
	CUpDwnBtn* m_pcPara_OnOff;
	CBmp* m_pcPara_OnOff_Up;
	CBmp* m_pcPara_OnOff_Dw;
	//CBmp* m_pcPara_OnOff_Fc;

	CBmp* m_pcPara_UpT;
	CBmp* m_pcPara_DisT;
	CBmp* m_pcPara_FcT;
	CBmp* m_pcPara_DwT;
	CBmp* m_pcPara_UpB;
	CBmp* m_pcPara_DisB;
	CBmp* m_pcPara_FcB;
	CBmp* m_pcPara_DwB;
	CBmp* m_pcPara_SigT;
	CBmp* m_pcPara_SigB;
	CBmp* m_pcSubPara_Up;
	CBmp* m_pcSubPara_Dw;
	CBmp* m_pcSubPara_Sig;
	CBmp* m_pcSubPara_Trig;

	CBmp* m_pcParaMenu_Up;
	CBmp* m_pcParaMenu_Dw;

	CBmp* m_pcLungRec;
	//CBmp* m_pcVGorange;
	//CBmp* m_pcVLorange;

	//CBmp* m_pcPara_DUMMY;

	CUpDwnBtn* m_pcParaMenu;
	//CSelectGraphBtn* m_pcParaMenu;

	CWndParaSettings *m_pcWndParaSettings;

	
	
	CParaBtn_RISETIME* m_pcPara_RISETIME;//IDC_BTN_PARA_RISETIME
	
	CParaBtn_HFFLOW* m_pcPara_HFFLOW;
	//CParaBtn_PLATEAU* m_pcPara_Plateau;
	CParaBtn_IFLOW* m_pcPara_IFLOW;
	//CParaBtn_EFLOW* m_pcPara_EFLOW;
	CParaBtn_PEEP* m_pcPara_PEEP;
	CParaBtn_PINSP* m_pcPara_PINSP;
	CParaBtn_PDUO* m_pcPara_PDUO;
	CParaBtn_PMANUAL* m_pcPara_PMANUAL;
	CParaBtn_VLIMIT* m_pcPara_VLIMIT;
	CParaBtn_TRIGGER* m_pcPara_TRIGGERCONV;
	CParaBtn_TRIGGERNMODE* m_pcPara_TRIGGERNMODE;
	CParaBtn_BPM* m_pcPara_BPM;
	CParaBtn_ITIME* m_pcPara_ITIME;
	CParaBtn_ETIME* m_pcPara_ETIME;
	CParaBtn_O2* m_pcPara_O2;
	CParaBtn_VGARANT* m_pcPara_VGARANT;
	CParaBtn_BACKUP* m_pcPara_BACKUP;
	CParaBtn_CPAP* m_pcPara_CPAP;
	CParaBtn_FLOWMIN* m_pcPara_FLOWMIN;
	CParaBtn_FLOW* m_pcPara_FLOW;
	CParaBtn_O2Flush* m_pcPara_O2Flush;
	CParaBtn_PMITT* m_pcPara_PMITT;
	CParaBtn_PPSV* m_pcPara_PPSV;
	CParaBtn_HFAMPL* m_pcPara_HFAMPL;
	CParaBtn_HFFREQ* m_pcPara_HFFREQ;
	CParaBtn_IERatio* m_pcPara_IERatio;
	CParaBtn_ITIME_REC* m_pcPara_ITIME_REC;
	//CParaBtn_FREQ_REC* m_pcPara_FREQ_REC;
	CParaBtn_PMEAN_REC* m_pcPara_PMEAN_REC;
	CParaBtn_PMAX* m_pcPara_PMAXI;
	CParaBtn_AMPMAX* m_pcPara_AMPMAXI;

	CSubParaBtn* m_pcSubPara_TrigThreshold;
	CSubParaBtn* m_pcSubPara_IE;
	CSubParaBtn* m_pcSubPara_DemandFlow;
	CSubParaBtn* m_pcSubPara_ETime;
	CSubParaBtn* m_pcSubPara_PSVTIME;
	CSubParaBtn* m_pcSubPara_BPM;
	CSubParaBtn* m_pcSubPara_O2act;
	CSubParaBtn* m_pcSubPara_O2Flush;
	CSubParaBtn* m_pcSubPara_LEAKCOMP;
	CSubParaBtn* m_pcSubPara_BIASFLOW;

	CList<CParaBtn* , CParaBtn *> m_plParaBtn;
	//CPtrList


	CWndMenuVentState *m_pWndMenuVentState;

	//bool m_bBPM_SetKey;
	//bool m_bCPAP_SetKey;
	//bool m_bETIME_SetKey;
	//bool m_bITIME_SetKey;
	//bool m_bPDUO_SetKey;
	//bool m_bPEEP_SetKey;
	//bool m_bPINSP_SetKey;
	//bool m_bPMAX_SetKey;
	//bool m_bPPSV_SetKey;
	////bool m_bPMan_SetKey;
	//bool m_bPMEAN_SetKey;
	//bool m_bHFamp_SetKey;
	//bool m_bHFfreq_SetKey;
	//bool m_bVLIMIT_SetKey;
	//bool m_bVGARANT_SetKey;
	//bool m_bPMEANREC_SetKey;
	//bool m_bTHERAPYFLOW_SetKey;

	bool m_bSettingsActive;

	bool m_bPsvApnoe;

	bool m_bViewPraBtnShown;

	int m_iOldPPSV;
	int m_iOldPINSP;
	
public:
	DECLARE_MESSAGE_MAP()
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnDestroy(); 
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
