#pragma once
#include "globDefs.h"

class CMVModel;

class CParaData
{
	typedef struct paraData
	{
		WORD		m_iParaDataITime_TRIGGER;
		WORD		m_iParaDataITime_IPPV;
		WORD		m_iParaDataITime_NMODE;
		WORD		m_iParaDataETime_TRIGGER;
		WORD		m_iParaDataETime_IPPV;
		WORD		m_iParaDataETime_NMODE;
		WORD		m_iParaDataETime_SIMV;
		WORD		m_iParaDataBPM_TRIGGER;
		WORD		m_iParaDataBPM_IPPV;
		WORD		m_iParaDataBPM_SIMV;
		WORD		m_iParaDataBPM_NMODE;
		WORD		m_iParaDataIFlow_TRIGGER;
		WORD		m_iParaDataIFlow_IPPV;
		WORD		m_iParaDataHFFlow;
		WORD		m_iParaDataRisetime_TRIGGER;
		WORD		m_iParaDataRisetime_IPPV;
		WORD		m_iParaDataEFlow_TRIGGER;
		WORD		m_iParaDataEFlow_IPPV;
		WORD		m_iParaDataTherapieFlow;
		WORD		m_iParaDataFlowMin;
		SHORT		m_iParaDataPEEP_TRIGGER;
		SHORT		m_iParaDataPEEP_IPPV;
		SHORT		m_iParaDataPEEP_RISETIME;
		SHORT		m_iParaDataCPAP;
		SHORT		m_iParaDataCPAP_NMODE;
		SHORT		m_iParaDataPInsp_TRIGGER;
		SHORT		m_iParaDataPInsp_IPPV;
		SHORT		m_iParaDataPmaxVolG_TRIGGER;
		SHORT		m_iParaDataPmaxVolG_IPPV;
		SHORT		m_iParaDataPpsv;
		SHORT		m_iParaDataPManual_HFO;
		SHORT		m_iParaDataPManual_CPAP;
		SHORT		m_iParaDataPManual_NMODE;
		BYTE		m_iParaDataO2;
		BYTE		m_iParaDataO2Flush;
		WORD		m_iParaDataVLimit_IPPV;
		WORD		m_iParaDataVLimit_TRIGGER;
		bool		m_bParaDataVLimitOn_IPPV;
		bool		m_bParaDataVLimitOn_TRIGGER;
		WORD		m_iParaDataVGarant_IPPV;
		WORD		m_iParaDataVGarant_TRIGGER;
		bool		m_bParaDataVGarantOn_IPPV;
		bool		m_bParaDataVGarantOn_TRIGGER;
		BYTE		m_iParaDataTriggerCONV;
		BYTE		m_iParaDataTriggerNMODE;
		BYTE		m_iParaDataBackup;
		WORD		m_iParaDataHFAMPL;
		WORD		m_iParaDataHFAMPLmax;
		BYTE		m_iParaDataHFFreq;
		WORD		m_iParaDataHFPmean;
		WORD		m_iParaDataVGarant_HFO;
		bool		m_bParaDataVGarantOn_HFO;
		eRatioIE	m_iParaDataIERatio;
		WORD		m_iParaDataITIME_REC;
		WORD		m_iParaDataFREQ_REC;
		WORD		m_iParaDataPMEAN_REC;
		WORD		m_iParaDataFOTconv_AMPLITUDE;
		BYTE		m_iParaDataFOTconv_FREQ;
		WORD		m_iParaDataFOTconv_PEEPSTART;
		WORD		m_iParaDataFOTconv_PEEPEND;
		BYTE		m_iParaDataFOTconv_STEPS;
		WORD		m_iParaDataFOThfo_AMPLITUDE;
		BYTE		m_iParaDataFOThfo_FREQ;
		WORD		m_iParaDataFOThfo_PMEANSTART;
		WORD		m_iParaDataFOThfo_PMEANEND;
		BYTE		m_iParaDataFOThfo_STEPS;
	}PARA_DATA;
public:
	

	static CParaData* GetInstance();
	static void DestroyInstance();

	virtual void Init();

	void loadLimits();
	void checkIErelation();
	
	CMVModel *getModel();

	void GetParaData(PARA_DATA* pData);
	void SetDataFromMode(eVentMode mode);

	SHORT GetPpsvPara();
	void SetPpsvPara(SHORT value, bool bSend, bool bConfig);
	BYTE GetBackupPara();
	void SetBackupPara(BYTE value, bool bSend, bool bConfig);
	BYTE GetO2FlushPara();
	void SetO2FlushPara(BYTE value, bool bConfig);
	WORD GetBPMPara_TRIGGER();
	WORD GetBPMPara_IPPV();
	void SetBPMPara_TRIGGER(WORD value, bool bSend, bool bConfig);
	void SetBPMPara_IPPV(WORD value, bool bSend, bool bConfig);
	WORD GetBPMNMODEPara();
	void SetBPMNMODEPara(WORD value, bool bSend, bool bConfig);
	WORD GetETIMEPara_TRIGGER();
	void SetETIMEPara_TRIGGER(WORD value, bool bSend, bool bConfig);
	WORD GetETIMEPara_IPPV();
	void SetETIMEPara_IPPV(WORD value, bool bSend, bool bConfig);
	WORD GetETIMENMODEPara();
	void SetETIMENMODEPara(WORD value, bool bSend, bool bConfig);
	BYTE GetHFFreqPara();
	void SetHFFreqPara(BYTE value, bool bSend, bool bConfig);
	WORD GetHFAMPLPara();
	void SetHFAMPLPara(WORD value, bool bSend, bool bConfig);
	WORD GetHFAMPLmaxPara();
	void SetHFAMPLmaxPara(WORD value, bool bSend, bool bConfig);
	WORD GetHFPMeanPara();
	void SetHFPMeanPara(WORD value, bool bSend, bool bConfig);

	WORD GetITimeRecPara();
	void SetITimeRecPara(WORD value, bool bSend, bool bConfig);
	WORD GetFreqRecPara();
	void SetFreqRecPara(WORD value, bool bSend, bool bConfig);
	WORD GetHFPMeanRecPara();
	void SetHFPMeanRecPara(WORD value, bool bSend, bool bConfig);

	eRatioIE GetIERatioParaHFO();
	void SetIERatioParaHFO(eRatioIE ratio, bool bSend, bool bConfig);

	SHORT GetCPAPPara();
	void SetCPAPPara(SHORT value, bool bSend, bool bConfig);
	SHORT GetCPAPNMODEPara();
	void SetCPAPNMODEPara(SHORT value, bool bSend, bool bConfig);

	SHORT GetPManualHFOPara();
	void SetPManualHFOPara(SHORT value, bool bSend, bool bConfig);
	SHORT GetPManualCPAPPara();
	void SetPManualCPAPPara(SHORT value, bool bSend, bool bConfig);
	SHORT GetPManualNMODEPara();
	void SetPManualNMODEPara(SHORT value, bool bSend, bool bConfig);

	SHORT GetPmaxVolGPara_TRIGGER();
	SHORT GetPmaxVolGPara_IPPV();
	void SetPmaxVolGPara_TRIGGER(SHORT value, bool bSend, bool bConfig);
	void SetPmaxVolGPara_IPPV(SHORT value, bool bSend, bool bConfig);
	SHORT GetPINSPPara_TRIGGER();
	SHORT GetPINSPPara_IPPV();
	void SetPINSPPara_TRIGGER(SHORT value, bool bSend, bool bConfig);
	void SetPINSPPara_IPPV(SHORT value, bool bSend, bool bConfig);
	SHORT GetPEEPPara_TRIGGER();
	SHORT GetPEEPPara_IPPV();
	void SetPEEPPara_TRIGGER(SHORT value, bool bSend, bool bConfig);
	void SetPEEPPara_IPPV(SHORT value, bool bSend, bool bConfig);

	BYTE GetO2Para();
	void SetO2Para(BYTE value, bool bSend, bool bConfig);
	BYTE GetTriggerCONVPara();
	void SetTriggerCONVPara(BYTE value, bool bSend, bool bConfig);
	BYTE GetTriggerNMODEPara();
	void SetTriggerNMODEPara(BYTE value, bool bSend, bool bConfig);

	WORD GetEFLOWPara_TRIGGER();
	void SetEFLOWPara_TRIGGER(WORD value, bool bSend, bool bConfig);
	WORD GetEFLOWPara_IPPV();
	void SetEFLOWPara_IPPV(WORD value, bool bSend, bool bConfig);
	WORD GetTherapieFLOWPara();
	void SetTherapieFLOWPara(WORD value, bool bSend, bool bConfig);
	WORD GetFlowminPara();
	void SetFlowminPara(WORD value, bool bSend, bool bConfig);
	WORD GetITimePara_TRIGGER();
	void SetITimePara_TRIGGER(WORD value, bool bSend, bool bConfig);
	WORD GetITimePara_IPPV();
	void SetITimePara_IPPV(WORD value, bool bSend, bool bConfig);
	WORD GetITimeNMODEPara();
	void SetITimeNMODEPara(WORD value, bool bSend, bool bConfig);

	BYTE getByteFOTconv_AMPLITUDEPara();
	WORD getFOTconv_AMPLITUDEPara();
	void setFOTconv_AMPLITUDEPara(WORD ampl, bool bSend, bool bConfig);
	BYTE getByteFOTconv_FREQPara();
	BYTE getFOTconv_FREQPara();
	void setFOTconv_FREQPara(BYTE value, bool bSend, bool bConfig);
	WORD getFOTconv_PEEPSTARTPara();
	void setFOTconv_PEEPSTARTPara(WORD value, bool bSend, bool bConfig);
	WORD getFOTconv_PEEPENDPara();
	void setFOTconv_PEEPENDPara(WORD value, bool bSend, bool bConfig);
	BYTE getFOTconv_STEPSPara();
	void setFOTconv_STEPSPara(BYTE value, bool bSend, bool bConfig);

	WORD getFOThfo_AMPLITUDEPara();
	void setFOThfo_AMPLITUDEPara(WORD ampl, bool bSend, bool bConfig);
	BYTE getFOThfo_FREQPara();
	void setFOThfo_FREQPara(BYTE value, bool bSend, bool bConfig);
	WORD getFOThfo_PMEANSTARTPara();
	void setFOThfo_PMEANSTARTPara(WORD value, bool bSend, bool bConfig);
	WORD getFOThfo_PMEANENDPara();
	void setFOThfo_PMEANENDPara(WORD value, bool bSend, bool bConfig);
	BYTE getFOThfo_STEPSPara();
	void setFOThfo_STEPSPara(BYTE value, bool bSend, bool bConfig);

	void GetMaxParaData(PARA_DATA* pData);
	void SetMaxParaData(PARA_DATA* pData);

	BYTE GetBackupMaxPara();
	SHORT GetCPAPMaxPara();
	SHORT GetCPAPNMODEMaxPara();

	SHORT GetPINSPMaxPara_IPPV();
	SHORT GetPINSPMaxPara_TRIGGER();
	SHORT GetPMAXMaxPara_IPPV();
	SHORT GetPMAXMaxPara_TRIGGER();

	SHORT GetPpsvMaxPara();
	SHORT GetPManualMaxPara();
	SHORT GetPManualNMODEMaxPara();
	BYTE GetO2MaxPara();
	BYTE GetO2FlushMaxPara();
	BYTE GetTriggerCONVMaxPara();
	BYTE GetTriggerNMODEMaxPara();
	WORD GetEFLOWMaxPara_IPPV();
	WORD GetEFLOWMaxPara_TRIGGER();
	WORD GetTherapieFlowMaxPara();
	WORD GetFlowminMaxPara();
	SHORT GetPEEPMaxPara_IPPV();
	SHORT GetPEEPMaxPara_TRIGGER();
	WORD GetBPMMaxPara_IPPV();
	WORD GetBPMMaxPara_TRIGGER();
	WORD GetBPMNMODEMaxPara();
	WORD GetITimeMaxPara_IPPV();
	WORD GetITimeMaxPara_TRIGGER();
	WORD GetITimeNMODEMaxPara();
	WORD GetETIMEMaxPara_IPPV();
	WORD GetETIMEMaxPara_TRIGGER();
	WORD GetETIMENMODEMaxPara();
	WORD GetRisetimeMaxPara_IPPV();
	WORD GetRisetimeMaxPara_TRIGGER();
	WORD GetIFlowMaxPara_IPPV();
	WORD GetIFlowMaxPara_TRIGGER();
	WORD GetHFFlowMaxPara();
	WORD GetVLimitMaxPara_IPPV();
	WORD GetVLimitMaxPara_TRIGGER();
	WORD GetVGarantMaxPara_IPPV();
	WORD GetVGarantMaxPara_TRIGGER();
	WORD GetHFVGarantMaxPara();
	BYTE GetHFFreqMaxPara();
	WORD GetHFAMPLMaxPara();
	WORD GetHFPMeanMaxPara();
	WORD GetITimeRecMaxPara();
	WORD GetFreqRecMaxPara();
	WORD GetHFPMeanRecMaxPara();
	eRatioIE GetIERatioMaxPara();

	WORD getFOTconv_AMPLITUDEMaxPara();
	BYTE getFOTconv_FREQMaxPara();
	BYTE getFOTconv_STEPSMaxPara();

	BYTE getFOThfo_STEPSMaxPara();
	WORD getFOThfo_AMPLITUDEMaxPara();
	BYTE getFOThfo_FREQMaxPara();
	//WORD getFOThfo_PMEANSTARTMaxPara();
	//WORD getFOThfo_PMEANENDMaxPara();
	




	void SetHFAmplMaxParaNEONATAL(WORD iVal);
	void SetHFAmplMaxParaPEDIATRIC(WORD iVal);


	void GetMinParaData(PARA_DATA* pData);
	void SetMinParaData(PARA_DATA* pData);

	BYTE GetBackupMinPara();
	SHORT GetCPAPMinPara();
	SHORT GetCPAPNMODEMinPara();

	SHORT GetPINSPMinPara_IPPV();
	SHORT GetPINSPMinPara_TRIGGER();
	SHORT GetPMAXMinPara_IPPV();
	SHORT GetPMAXMinPara_TRIGGER();

	SHORT GetPpsvMinPara();
	SHORT GetPManualMinPara();
	SHORT GetPManualNMODEMinPara();
	BYTE GetO2MinPara();
	BYTE GetO2FlushMinPara();
	BYTE GetTriggerCONVMinPara();
	BYTE GetTriggerNMODEMinPara();
	WORD GetEFLOWMinPara_IPPV();
	WORD GetEFLOWMinPara_TRIGGER();
	WORD GetTherapieFlowMinPara();
	WORD GetFlowminMinPara();
	SHORT GetPEEPMinPara_TRIGGER();
	SHORT GetPEEPMinPara_IPPV();
	SHORT GetPEEPMinPara_RISETIME();
	WORD GetBPMMinPara_IPPV();
	WORD GetBPMMinPara_TRIGGER();
	WORD GetBPMNMODEMinPara();
	WORD GetITimeMinPara_IPPV();
	WORD GetITimeMinPara_TRIGGER();
	WORD GetITimeNMODEMinPara();
	WORD GetETIMEMinPara_IPPV();
	WORD GetETIMEMinPara_TRIGGER();
	WORD GetETIMENMODEMinPara();
	WORD GetIFlowMinPara_IPPV();
	WORD GetIFlowMinPara_TRIGGER();
	WORD GetHFFlowMinPara();
	WORD GetRisetimeMinPara_IPPV();
	WORD GetRisetimeMinPara_TRIGGER();
	WORD GetVLimitMinPara_IPPV();
	WORD GetVLimitMinPara_TRIGGER();
	WORD GetVGarantMinPara_IPPV();
	WORD GetVGarantMinPara_TRIGGER();
	WORD GetHFVGarantMinPara();
	BYTE GetHFFreqMinPara();
	WORD GetHFAMPLMinPara();
	WORD GetHFPMeanMinPara();
	WORD GetITimeRecMinPara();
	WORD GetFreqRecMinPara();
	WORD GetHFPMeanRecMinPara();
	eRatioIE GetIERatioMinPara();

	WORD getFOTconv_AMPLITUDEMinPara();
	BYTE getFOTconv_FREQMinPara();
	BYTE getFOTconv_STEPSMinPara();

	BYTE getFOThfo_STEPSMinPara();
	WORD getFOThfo_AMPLITUDEMinPara();
	BYTE getFOThfo_FREQMinPara();
	//WORD getFOThfo_PMEANSTARTMinPara();
	//WORD getFOThfo_PMEANENDMinPara();
	
	SHORT GetPManualMaxKey();
	SHORT GetPManualNMODEMaxKey();
	SHORT GetCPAPMaxKey();
	SHORT GetCPAPNMODEMaxKey();
	SHORT GetPInspMaxKey();
	SHORT GetPpsvMaxKey();
	SHORT GetPEEPMaxKey();
	WORD GetHFPMeanMaxKey();
	//WORD GetHFampMaxKey();
	BYTE GetHFfreqMaxKey();
	WORD GetVlimitMaxKey_IPPV();
	WORD GetVlimitMaxKey_TRIGGER();
	WORD GetVgarantMaxKey_IPPV();
	WORD GetVgarantMaxKey_TRIGGER();
	WORD GetHFVgarantMaxKey();
	WORD GetHFPMeanRecMaxKey();
	WORD GetFreqRecMaxKey();
	WORD GetITimeRecMaxKey();
	WORD GetTherapieFlowMaxKey();

	SHORT GetPManualMinKey();
	SHORT GetPManualNMODEMinKey();
	SHORT GetCPAPMinKey();
	SHORT GetCPAPNMODEMinKey();
	SHORT GetPInspMinKey();
	SHORT GetPpsvMinKey();
	SHORT GetPEEPMinKey();
	SHORT GetTherapieFlowMinKey();

	
//
	
	WORD GetRisetimePara_TRIGGER();
	void SetRisetimePara_TRIGGER(WORD value, bool bSend, bool bConfig);
	WORD GetRisetimePara_IPPV();
	void SetRisetimePara_IPPV(WORD value, bool bSend, bool bConfig);

	WORD GetIFlowPara_TRIGGER();
	void SetIFlowPara_TRIGGER(WORD value, bool bSend, bool bConfig);
	WORD GetIFlowPara_IPPV();
	void SetIFlowPara_IPPV(WORD value, bool bSend, bool bConfig);

	WORD GetHFFlowPara();
	void SetHFFlowPara(WORD value, bool bSend, bool bConfig);

	WORD GetVLimitParam_TRIGGER();
	void SetVLimitParam_TRIGGER(WORD val, bool bOn,bool bSend, bool bConfig);
	bool IsVLimitParamOn_TRIGGER();

	WORD GetVLimitParam_IPPV();
	void SetVLimitParam_IPPV(WORD val, bool bOn,bool bSend, bool bConfig);
	bool IsVLimitParamOn_IPPV();

	//void SetVGarantParam_IPPV(WORD val, bool bOn,bool bSend,bool bConfig,bool bAutoOn,bool bAutoOff,bool bPreset=false,bool bChangeState=true);
	void SetVGarantPara_IPPV(WORD value, bool bSend, bool bConfig);
	WORD GetVGarantPara_IPPV();
	void SetVGarantState_IPPV(bool bOn,bool bSend,bool bConfig);
	bool IsVGarantStateOn_IPPV();

	//void SetVGarantParam_TRIGGER(WORD val, bool bOn,bool bSend,bool bConfig,bool bAutoOn,bool bAutoOff,bool bPreset=false,bool bChangeState=true);
	void SetVGarantPara_TRIGGER(WORD value, bool bSend, bool bConfig);
	WORD GetVGarantPara_TRIGGER();
	void SetVGarantState_TRIGGER(bool bOn,bool bSend,bool bConfig);
	bool IsVGarantStateOn_TRIGGER();

	//void SetHFVGarantParam(WORD val, bool bOn,bool bSend,bool bConfig,bool bAutoOn,bool bAutoOff,bool bPreset=false,bool bChangeState=true);
	void SetHFVGarantPara(WORD value, bool bSend, bool bConfig);
	WORD GetHFVGarantPara();
	void SetHFVGarantState(bool bOn,bool bSend,bool bConfig);
	bool IsHFVGarantStateOn();



protected:
	


protected:
	CParaData(void);
	virtual ~CParaData(void);
private:
	//singleton
	static CParaData* theData;
	CMVModel *m_pModel;
protected:

	PARA_DATA		m_data;
	PARA_DATA		m_dataMaxRange_NEONATAL;
	PARA_DATA		m_dataMinRange_NEONATAL;
	PARA_DATA		m_dataMaxRange_PEDIATRIC;
	PARA_DATA		m_dataMinRange_PEDIATRIC;

	PARA_DATA		m_dataKeyMax_NEONATAL;
	PARA_DATA		m_dataKeyMax_PEDIATRIC;
	PARA_DATA		m_dataKeyMin_NEONATAL;
	PARA_DATA		m_dataKeyMin_PEDIATRIC;

	BYTE m_byFOTAmp;
	BYTE m_byFOTFreq;
};
