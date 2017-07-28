/**********************************************************************************************//**
 * \file	Configuration.h.
 *
 * Declares the configuration class.
 **************************************************************************************************/

#pragma once
#include "globDefs.h"
#include "config.h"

//#include "TlsTime.h"

class CTlsTime;
class CMVModel;

class CConfiguration: public CObject
{
public:

	virtual void SerializeFile(CArchive& ar);

	static CConfiguration* GetInstance();

	static void DestroyInstance();

	void Init();

	CStringW GetLanguageNamefromID(WORD iID);
	WORD GetLanguageIDfromName(CStringW szLID);
	WORD GetLanguageID();
	CStringW getLANGUAGE();
	void SetLanguage(CStringW szLID);

	BYTE getSystemHWrevision();
	eDevType getDevType();
	eFlowSensorType getFlowSensorType();
	eAVRevision getAVRevision();
	eHFModuleRevision getHFModuleRevision();
	
	BYTE GetMainBoardVersion();
	void setMainBoardVersion(UINT vers);

	void setTempCO2module(BYTE mod);
	BYTE getTempCO2module();
	BYTE getCO2module();
	void setCO2module(BYTE mod);

	BYTE GetPDMSprotocol();
	void SetPDMSprotocol(BYTE prot);

	UINT GetEthernetPort();
	void SetEthernetPort(UINT port);

	void setTempSPO2module(BYTE mod);
	BYTE getTempSPO2module();
	BYTE getSPO2module();
	void setSPO2module(BYTE mod, bool bReinit=false);
	bool isSpO2ConfigInProgress();

	eAcuLinkVersion GetAcuLinkVersion();
	void SetAcuLinkVersion(eAcuLinkVersion vers);

	int GetVentRange();
	void SetVentRange(int range);
	void checkVentRangeSettings();

	CStringW GetBoardSubType();
	CStringW GetKernelVersion();
	CStringW GetKernelAcuVersion();
	CStringW GetKernelVersionDate();

	void GraphSetLastViewState(int state);
	int GraphGetLastViewState();

	BYTE GetTimePumpAutoOn();
	void SetTimePumpAutoOn(BYTE value);

	void GraphSetAutoScale(bool state);
	bool GraphIsAutoScale();

	void GraphSetFilled(bool state);
	bool GraphIsFilled();

	void TrendSetAutoScale(bool state);
	bool TrendIsAutoScale();
	void TrendSetFilled(bool state);
	bool TrendIsFilled();

	void setTriggerOption_CONV(eTriggereType type);
	eTriggereType getTriggerOption_CONV();

	void setTriggerOption_CPAP(eTriggereType type);
	eTriggereType getTriggerOption_CPAP();

	void setTriggerOption_DUOPAP(eTriggereType type);
	eTriggereType getTriggerOption_DUOPAP();

	void setTriggerOption_NCPAP(eTriggereType type);
	eTriggereType getTriggerOption_NCPAP();

	void SetAutoOxyCal(eAutoOxyCal state);
	eAutoOxyCal GetAutoOxyCal();

	void SetAutoScreenlockTime(eAutoScreenlock state);
	eAutoScreenlock GetAutoScreenlockTime();

	void SetAutoScreenlockActive(bool state);
	bool IsAutoScreenlockActive();

	void SetServicePasswordDisabled(bool state);
	bool IsServicePasswordDisabled();

	void SetNursecallActive(bool state);
	bool IsNursecallActive();

	void SetEnableAutolimit(bool state);
	bool IsEnableAutolimitActive();

	void SetEnableFactory(bool state);
	bool IsEnableFactoryActive();

	bool GetPlayVlimitSound();
	
	bool GetPlayBackupSound();
	
	bool getShowEFlowInSettings();
	void setShowEFlowInSettings(bool bState);

	void SetHFOManBreathEnabled(bool state);
	bool IsHFOManBreathEnabled();

	void SetEFLOWequalILFOW(bool state);
	bool IsEFLOWequalILFOW();

	void SetPercentAbortCriterionPSV(int iVal);
	int GetPercentAbortCriterionPSV();

	void SetFlushTime(BYTE iTime);
	BYTE GetFlushTime();

	void SetManBreathTime(int iTime);
	int GetManBreathTime();

	void setFOTconv_AMPLITUDE(WORD amp);
	WORD getFOTconv_AMPLITUDE();

	void setFOTconv_FREQ(BYTE freq);
	BYTE getFOTconv_FREQ();

	void setFOTconv_STEPS(BYTE steps);
	BYTE getFOTconv_STEPS();

	void setDiffPEEPFOT(WORD diff);
	WORD getDiffPEEPFOT();

	void setFOTconv_PEEPSTART(WORD pmean);
	WORD getFOTconv_PEEPSTART();

	void setFOTconv_PEEPEND(WORD pmean);
	WORD getFOTconv_PEEPEND();

	void setFOThfo_AMPLITUDE(WORD amp);
	WORD getFOThfo_AMPLITUDE();

	void setFOThfo_FREQ(BYTE freq);
	BYTE getFOThfo_FREQ();

	void setFOThfo_STEPS(BYTE steps);
	BYTE getFOThfo_STEPS();

	void setDiffPmeanFOT(WORD diff);
	WORD getDiffPmeanFOT();

	void setFOThfo_PMEANSTART(WORD pmean);
	WORD getFOThfo_PMEANSTART();

	void setFOThfo_PMEANEND(WORD pmean);
	WORD getFOThfo_PMEANEND();

	void SetNebulizerTime(int iTime);
	int GetNebulizerTime();

	int GetCurPminAlarmDelay();
	void SetCurPminAlarmDelay(int iTime);

	int getCurPatientAlarmDelay();
	void setCurPatientAlarmDelay(int iTime);

	eVentMode GetCurMode();
	void SetCurMode(eVentMode state);

	void SetCurPressureRiseCtrl(eCurveForm form);
	eCurveForm GetCurPressureRiseCtrl();

	bool CurModeIsPresetMode();

	eVentMode GetPrevMode();
	void SetPrevMode(eVentMode state);

	WORD		GetParaDataITime_TRIGGER();
	void		SetParaDataITime_TRIGGER(WORD iVal);

	WORD		GetParaDataITime_IPPV();
	void		SetParaDataITime_IPPV(WORD iVal);

	WORD		GetParaDataITime_NMODE();
	void		SetParaDataITime_NMODE(WORD iVal);

	WORD		GetParaDataETIME_TRIGGER();
	void		SetParaDataETIME_TRIGGER(WORD iVal);

	WORD		GetParaDataETIME_IPPV();
	void		SetParaDataETIME_IPPV(WORD iVal);

	WORD		GetParaDataETIME_NMODE();
	void		SetParaDataETIME_NMODE(WORD iVal);

	WORD		GetParaDataBPM_TRIGGER();
	void		SetParaDataBPM_TRIGGER(WORD iVal);

	WORD		GetParaDataBPM_IPPV();
	void		SetParaDataBPM_IPPV(WORD iVal);

	WORD		GetParaDataBPM_NMODE();
	void		SetParaDataBPM_NMODE(WORD iVal);

	WORD		GetParaDataIFlow_TRIGGER();
	void		SetParaDataIFlow_TRIGGER(WORD iVal);

	WORD		GetParaDataIFlow_IPPV();
	void		SetParaDataIFlow_IPPV(WORD iVal);

	WORD		GetParaDataHFFlow();
	void		SetParaDataHFFlow(WORD iVal);

	WORD		GetParaDataRisetime_IPPV();
	void		SetParaDataRisetime_IPPV(WORD iVal);

	WORD		GetParaDataRisetime_TRIGGER();
	void		SetParaDataRisetime_TRIGGER(WORD iVal);

	WORD		GetParaDataTherapieFlow();
	void		SetParaDataTherapieFlow(WORD iVal);

	WORD		GetParaDataEFlow_IPPV();
	void		SetParaDataEFlow_IPPV(WORD iVal);

	WORD		GetParaDataEFlow_TRIGGER();
	void		SetParaDataEFlow_TRIGGER(WORD iVal);

	WORD		GetParaDataFlowMin();
	void		SetParaDataFlowMin(WORD iVal);

	SHORT		GetParaDataPEEP_IPPV();
	void		SetParaDataPEEP_IPPV(SHORT iVal);

	SHORT		GetParaDataPEEP_TRIGGER();
	void		SetParaDataPEEP_TRIGGER(SHORT iVal);

	void		SetParaDataCPAP(SHORT iVal);

	SHORT		GetParaDataCPAP_NMODE();
	void		SetParaDataCPAP_NMODE(SHORT iVal);

	SHORT		GetParaDataPInsp_TRIGGER();
	void		SetParaDataPInsp_TRIGGER(SHORT iVal);

	SHORT		GetParaDataPInsp_IPPV();
	void		SetParaDataPInsp_IPPV(SHORT iVal);

	SHORT		GetParaDataPmaxVolG_TRIGGER();
	void		SetParaDataPmaxVolG_TRIGGER(SHORT iVal);

	SHORT		GetParaDataPmaxVolG_IPPV();
	void		SetParaDataPmaxVolG_IPPV(SHORT iVal);

	SHORT		GetParaDataPpsv();
	void		SetParaDataPpsv(SHORT iVal);

	SHORT		GetParaDataPManual_HFO();
	void		SetParaDataPManual_HFO(SHORT iVal, bool bSendAcuLink=true);//PMAN1
	
	void		SetParaDataPManual_CPAP(SHORT iVal, bool bSendAcuLink=true);//PMAN1

	SHORT		GetParaDataPManual_NMODE();
	void		SetParaDataPManual_NMODE(SHORT iVal);

	BYTE		GetParaDataO2();
	void		SetParaDataO2(BYTE iVal);

	BYTE		GetParaDataO2Flush();
	void		SetParaDataO2Flush(BYTE iVal);

	BYTE		GetO2Difference();
	void		SetO2Difference(BYTE value);

	int			GetPmeanDifference();
	void		SetPmeanDifference(int value);

	int			getPEEP_PpsvDifference();
	void		setPEEP_PpsvDifference(int value);

	WORD		GetParaDataVLimit_TRIGGER();
	WORD		GetParaDataVLimit_IPPV();

	void		SetParaDataVLimit_TRIGGER(WORD iVal);
	void		SetParaDataVLimit_IPPV(WORD iVal);

	bool		GetParaDataVLimitState_TRIGGER();
	bool		GetParaDataVLimitState_IPPV();

	void		SetParaDataVLimitState_TRIGGER(bool bState);
	void		SetParaDataVLimitState_IPPV(bool bState);

	WORD		GetParaDataVGarant_TRIGGER();
	WORD		GetParaDataVGarant_IPPV();

	void		SetParaDataVGarant_TRIGGER(WORD iVal);
	void		SetParaDataVGarant_IPPV(WORD iVal);

	bool		GetParaDataVGarantState_TRIGGER();
	bool		GetParaDataVGarantState_IPPV();

	void		SetParaDataVGarantState_TRIGGER(bool bState);
	void		SetParaDataVGarantState_IPPV(bool bState);

	/*BYTE		GetParaDataTriggerCONV();
	void		SetParaDataTriggerCONV(BYTE iVal);*/

	/*BYTE		GetParaDataTriggerNMODE();
	void		SetParaDataTriggerNMODE(BYTE iVal);*/

	BYTE		GetParaDataTrigger_CONV();
	void		SetParaDataTrigger_CONV(BYTE iVal);

	BYTE		GetParaDataTrigger_CPAP();
	void		SetParaDataTrigger_CPAP(BYTE iVal);

	BYTE		GetParaDataTrigger_NCPAP();
	void		SetParaDataTrigger_NCPAP(BYTE iVal);

	BYTE		GetParaDataTrigger_DUOPAP();
	void		SetParaDataTrigger_DUOPAP(BYTE iVal);



	BYTE		GetParaDataBackup();
	void		SetParaDataBackup(BYTE iVal);

	WORD		GetParaDataHFAmpl();
	void		SetParaDataHFAmpl(WORD iVal);

	WORD		GetParaDataHFAmplmax();
	void		SetParaDataHFAmplmax(WORD iVal);

	BYTE		GetParaDataHFFreq();
	void		SetParaDataHFFreq(BYTE iVal);

	WORD		GetParaDataHFPmean();
	void		SetParaDataHFPmean(WORD iVal);

	WORD		GetParaDataITimeRec();
	void		SetParaDataITimeRec(WORD iVal);

	WORD		GetParaDataFREQ_REC();
	void		SetParaDataFREQ_REC(WORD iVal);

	WORD		GetParaDataHFPMeanRec();
	void		SetParaDataHFPMeanRec(WORD iVal);

	WORD		GetParaDataHFVGarant();
	void		SetParaDataHFVGarant(WORD iVal);

	bool		GetParaDataHFVGarantState();
	void		SetParaDataHFVGarantState(bool bState);

	eRatioIE	GetParaDataHFIERatio();
	void		SetParaDataHFIERatio(eRatioIE iVal);
	
	bool DeaktivateVGarantState();

	bool DeaktivateVLimitState();
	
	eCO2unit GetCO2unit();
	void SetCO2unit(eCO2unit unit);

	SHORT GetCO2BaroPressure();
	void SetCO2BaroPressure(SHORT value);

	bool isFastSATon();
	void setFastSATon(bool bFastSATon);

	eSPO2sensitivitymode getSensitivityMode();
	void setSensitivityMode(eSPO2sensitivitymode mode);

	eSPO2alarmdelay getSPO2alarmDelay();
	void setSPO2alarmDelay(eSPO2alarmdelay delay);

	eSPO2averagingtime getSPO2averagingTime();
	void setSPO2averagingTime(eSPO2averagingtime avtime);

	bool useTveBTB();
	void setUseTveBTB(bool state);

	bool useNeoPed();
	void setUseNeoPed(bool state);

	bool getBTPS();
	void setBTPS(bool state);

	eIERelationMode GetIERelationMode();
	void SetIERelationMode(eIERelationMode mode);

	eRefFlowSensor GetRefFlowSensor();
	void SetRefFlowSensor(eRefFlowSensor mode);

	eTubeConfig GetTubeConfig();
	eTubeSet GetTubeSet();

	void SetTubeSet(eTubeSet tube);

	void setFreshgasExt(bool state);

	bool getFreshgasExt();

	bool isBiasFlowActive();
	bool isSPO2REGISTRYenabled();
	bool isCO2REGISTRYenabled();
	bool isRISETIMEREGISTRYenabled();

	BYTE getPRICO_SPO2lowRange();
	void setPRICO_SPO2lowRange(BYTE SPO2low);

	BYTE getPRICO_SPO2highRange();
	void setPRICO_SPO2highRange(BYTE SPO2high);

	BYTE getPRICO_FIO2lowRange();
	void setPRICO_FIO2lowRange(BYTE FIO2low);

	BYTE getPRICO_FIO2highRange();
	void setPRICO_FIO2highRange(BYTE FIO2high);
	
	ePressureUnit GetPressureUnit();
	void SetPressureUnit(ePressureUnit eUnit);

	eVolumeControl GetVolumeControl();
	void SetVolumeControl(eVolumeControl eCtrl);

	eFlowSensorState GetFlowSensorState();
	void SetFlowSensorState(eFlowSensorState state);

	int GetAlarmlimitETCO2max();
	eAlarmLimitState GetAlarmlimitStateETCO2max();
	void SetAlarmlimitETCO2max(int value);
	void SetAlarmlimitStateETCO2max(eAlarmLimitState state);
	int GetAlarmlimitETCO2min();
	eAlarmLimitState GetAlarmlimitStateETCO2min();
	void SetAlarmlimitETCO2min(int value);
	void SetAlarmlimitStateETCO2min(eAlarmLimitState state);

	int GetAlarmlimitFICO2max();
	eAlarmLimitState GetAlarmlimitStateFICO2max();
	void SetAlarmlimitFICO2max(int value);
	void SetAlarmlimitStateFICO2max(eAlarmLimitState state);
	int GetAlarmlimitFICO2min();
	eAlarmLimitState GetAlarmlimitStateFICO2min();
	void SetAlarmlimitFICO2min(int value);
	void SetAlarmlimitStateFICO2min(eAlarmLimitState state);

	int GetAlarmlimitSPO2max();
	eAlarmLimitState GetAlarmlimitStateSPO2max();
	void SetAlarmlimitSPO2max(int value);
	void SetAlarmlimitStateSPO2max(eAlarmLimitState state);

	int GetAlarmlimitSPO2min();
	eAlarmLimitState GetAlarmlimitStateSPO2min();
	void SetAlarmlimitSPO2min(int value);
	void SetAlarmlimitStateSPO2min(eAlarmLimitState state);

	int GetAlarmlimitPulseRatemax();
	eAlarmLimitState GetAlarmlimitStatePulseRatemax();
	void SetAlarmlimitPulseRatemax(int value);
	void SetAlarmlimitStatePulseRatemax(eAlarmLimitState state);

	int GetAlarmlimitPulseRatemin();
	eAlarmLimitState GetAlarmlimitStatePulseRatemin();
	void SetAlarmlimitPulseRatemin(int value);
	void SetAlarmlimitStatePulseRatemin(eAlarmLimitState state);

	int GetAlarmlimitSPO2_PImin();
	eAlarmLimitState GetAlarmlimitStateSPO2_PImin();
	void SetAlarmlimitSPO2_PImin(int value);
	void SetAlarmlimitStateSPO2_PImin(eAlarmLimitState state);

	int GetAlarmlimitSPO2_SIQmin();
	eAlarmLimitState GetAlarmlimitStateSPO2_SIQmin();
	void SetAlarmlimitSPO2_SIQmin(int value);
	void SetAlarmlimitStateSPO2_SIQmin(eAlarmLimitState state);

	int GetAlarmlimitMVmax();
	eAlarmLimitState GetAlarmlimitStateMVmax();
	void SetAlarmlimitMVmax(int value);
	void SetAlarmlimitStateMVmax(eAlarmLimitState state);

	int GetAlarmlimitMVmin();
	eAlarmLimitState GetAlarmlimitStateMVmin();
	void SetAlarmlimitMVmin(int value);
	void SetAlarmlimitStateMVmin(eAlarmLimitState state);

	int GetAlarmlimitPIPmax();
	eAlarmLimitState GetAlarmlimitStatePIPmax();
	void SetAlarmlimitPIPmax(int value);
	void SetAlarmlimitStatePIPmax(eAlarmLimitState state);

	int GetAlarmlimitPIPmin();
	eAlarmLimitState GetAlarmlimitStatePIPmin();
	void SetAlarmlimitPIPmin(int value);
	void SetAlarmlimitStatePIPmin(eAlarmLimitState state);

	int GetAlarmlimitPEEPmin();
	eAlarmLimitState GetAlarmlimitStatePEEPmin();
	void SetAlarmlimitPEEPmin(int value);
	void SetAlarmlimitStatePEEPmin(eAlarmLimitState state);

	int GetAlarmlimitBPMmax();
	eAlarmLimitState GetAlarmlimitStateBPMmax();
	void SetAlarmlimitBPMmax(int value);
	void SetAlarmlimitStateBPMmax(eAlarmLimitState state);

	int getAlimitLeakmax();
	eAlarmLimitState GetAlarmlimitStateLeakmax();
	void SetAlarmlimitLeakmax(int value);
	void SetAlarmlimitStateLeakmax(eAlarmLimitState state);

	int GetAlarmlimitApnoe();
	eAlarmLimitState GetAlarmlimitStateApnoe();
	void setAlimitApnoe(int value);
	void SetAlarmlimitStateApnoe(eAlarmLimitState state);

	int GetAlarmlimitSPO2maxHF();
	eAlarmLimitState GetAlarmlimitStateSPO2maxHF();
	void SetAlarmlimitSPO2maxHF(int value);
	void SetAlarmlimitStateSPO2maxHF(eAlarmLimitState state);

	int GetAlarmlimitSPO2minHF();
	eAlarmLimitState GetAlarmlimitStateSPO2minHF();
	void SetAlarmlimitSPO2minHF(int value);
	void SetAlarmlimitStateSPO2minHF(eAlarmLimitState state);

	int GetAlarmlimitPulseRatemaxHF();
	eAlarmLimitState GetAlarmlimitStatePulseRatemaxHF();
	void SetAlarmlimitPulseRatemaxHF(int value);
	void SetAlarmlimitStatePulseRatemaxHF(eAlarmLimitState state);

	int GetAlarmlimitPulseRateminHF();
	eAlarmLimitState GetAlarmlimitStatePulseRateminHF();
	void SetAlarmlimitPulseRateminHF(int value);
	void SetAlarmlimitStatePulseRateminHF(eAlarmLimitState state);

	int GetAlarmlimitSPO2_PIminHF();
	eAlarmLimitState GetAlarmlimitStateSPO2_PIminHF();
	void SetAlarmlimitSPO2_PIminHF(int value);
	void SetAlarmlimitStateSPO2_PIminHF(eAlarmLimitState state);

	int GetAlarmlimitSPO2_SIQminHF();
	eAlarmLimitState GetAlarmlimitStateSPO2_SIQminHF();
	void SetAlarmlimitSPO2_SIQminHF(int value);
	void SetAlarmlimitStateSPO2_SIQminHF(eAlarmLimitState state);

	int GetAlarmlimitETCO2maxHF();
	eAlarmLimitState GetAlarmlimitStateETCO2maxHF();
	void SetAlarmlimitETCO2maxHF(int value);
	void SetAlarmlimitStateETCO2maxHF(eAlarmLimitState state);

	int GetAlarmlimitETCO2minHF();
	eAlarmLimitState GetAlarmlimitStateETCO2minHF();
	void SetAlarmlimitETCO2minHF(int value);
	void SetAlarmlimitStateETCO2minHF(eAlarmLimitState state);

	int GetAlarmlimitFICO2maxHF();
	eAlarmLimitState GetAlarmlimitStateFICO2maxHF();
	void SetAlarmlimitFICO2maxHF(int value);
	void SetAlarmlimitStateFICO2maxHF(eAlarmLimitState state);

	int GetAlarmlimitFICO2minHF();
	eAlarmLimitState GetAlarmlimitStateFICO2minHF();
	void SetAlarmlimitFICO2minHF(int value);
	void SetAlarmlimitStateFICO2minHF(eAlarmLimitState state);

	int GetAlarmlimitMAPmaxHF();
	eAlarmLimitState GetAlarmlimitStateMAPmaxHF();
	void SetAlarmlimitMAPmaxHF(int value);
	void SetAlarmlimitStateMAPmaxHF(eAlarmLimitState state);

	int GetAlarmlimitMAPminHF();
	eAlarmLimitState GetAlarmlimitStateMAPminHF();
	void SetAlarmlimitMAPminHF(int value);
	void SetAlarmlimitStateMAPminHF(eAlarmLimitState state);

	int GetAlarmlimitDCO2maxHF();
	eAlarmLimitState GetAlarmlimitStateDCO2maxHF();
	void SetAlarmlimitDCO2maxHF(int value);
	void SetAlarmlimitStateDCO2maxHF(eAlarmLimitState state);

	int GetAlarmlimitDCO2minHF();
	eAlarmLimitState GetAlarmlimitStateDCO2minHF();
	void SetAlarmlimitDCO2minHF(int value);
	void SetAlarmlimitStateDCO2minHF(eAlarmLimitState state);

	int GetAlarmlimitMVmaxHF();
	eAlarmLimitState GetAlarmlimitStateMVmaxHF();
	void SetAlarmlimitMVmaxHF(int value);
	void SetAlarmlimitStateMVmaxHF(eAlarmLimitState state);

	int GetAlarmlimitMVminHF();
	eAlarmLimitState GetAlarmlimitStateMVminHF();
	void SetAlarmlimitMVminHF(int value);
	void SetAlarmlimitStateMVminHF(eAlarmLimitState state);

	
	int GetAlarmlimitPmaxHF();
	eAlarmLimitState GetAlarmlimitStatePmaxHF();
	void SetAlarmlimitPmaxHF(int value);
	void SetAlarmlimitStatePmaxHF(eAlarmLimitState state);

	int GetAlarmlimitPEEPminHF();
	eAlarmLimitState GetAlarmlimitStatePEEPminHF();
	void SetAlarmlimitPEEPminHF(int value);
	void SetAlarmlimitStatePEEPminHF(eAlarmLimitState state);

	int GetAlarmlimitBPMmaxHF();
	eAlarmLimitState GetAlarmlimitStateBPMmaxHF();
	void SetAlarmlimitBPMmaxHF(int value);
	void SetAlarmlimitStateBPMmaxHF(eAlarmLimitState state);

	int GetAlarmlimitLeakmaxHF();
	eAlarmLimitState GetAlarmlimitStateLeakmaxHF();
	void SetAlarmlimitLeakmaxHF(int value);
	void SetAlarmlimitStateLeakmaxHF(eAlarmLimitState state);

	int GetAlarmlimitApnoeHF();
	eAlarmLimitState GetAlarmlimitStateApnoeHF();
	void SetAlarmlimitApnoeHF(int value);
	void SetAlarmlimitStateApnoeHF(eAlarmLimitState state);

	int GetAlarmlimitPmaxCpap();
	eAlarmLimitState GetAlarmlimitStatePmaxCpap();
	void SetAlarmlimitPmaxCpap(int value);
	void SetAlarmlimitStatePmaxCpap(eAlarmLimitState state);

	int GetAlarmlimitPEEPminCpap();
	eAlarmLimitState GetAlarmlimitStatePEEPminCpap();
	void SetAlarmlimitPEEPminCpap(int value);
	void SetAlarmlimitStatePEEPminCpap(eAlarmLimitState state);

	int GetAlarmlimitPmaxDUOPAP();
	eAlarmLimitState GetAlarmlimitStatePmaxDUOPAP();
	void SetAlarmlimitPmaxDUOPAP(int value);
	void SetAlarmlimitStatePmaxDUOPAP(eAlarmLimitState state);

	int GetAlarmlimitPmaxNCPAP();
	eAlarmLimitState GetAlarmlimitStatePmaxNCPAP();
	void SetAlarmlimitPmaxNCPAP(int value);
	void SetAlarmlimitStatePmaxNCPAP(eAlarmLimitState state);

	int GetAlarmlimitPEEPminDUOPAP();
	eAlarmLimitState GetAlarmlimitStatePEEPminDUOPAP();
	void SetAlarmlimitPEEPminDUOPAP(int value);
	void SetAlarmlimitStatePEEPminDUOPAP(eAlarmLimitState state);

	int GetAlarmlimitPEEPminNCPAP();
	eAlarmLimitState GetAlarmlimitStatePEEPminNCPAP();
	void SetAlarmlimitPEEPminNCPAP(int value);
	void SetAlarmlimitStatePEEPminNCPAP(eAlarmLimitState state);

	double GetMAXSCALE_SPO2_GRAPH();
	void SetMAXSCALE_SPO2_GRAPH(double value);

	double GetMAXSCALE_CO2_GRAPH();
	void SetMAXSCALE_CO2_GRAPH(double value);

	double GetMAXSCALE_FLOW_GRAPH();
	void SetMAXSCALE_FLOW_GRAPH(double value);

	double GetMAXSCALE_PRESSURE_GRAPH();
	void SetMAXSCALE_PRESSURE_GRAPH(double value);

	double GetMAXSCALE_VOLUME_GRAPH();
	void SetMAXSCALE_VOLUME_GRAPH(double value);

	double GetMAXSCALE_SPO2_HFGRAPH();
	void SetMAXSCALE_SPO2_HFGRAPH(double value);

	double GetMAXSCALE_CO2_HFGRAPH();
	void SetMAXSCALE_CO2_HFGRAPH(double value);

	double GetMAXSCALE_FLOW_HFGRAPH();
	void SetMAXSCALE_FLOW_HFGRAPH(double value);

	double GetMAXSCALE_VOLUME_HFGRAPH();
	void SetMAXSCALE_VOLUME_HFGRAPH(double value);

	double GetMAXSCALE_PRESSURE_HFGRAPH();
	void SetMAXSCALE_PRESSURE_HFGRAPH(double value);

	double GetMINSCALE_PRESSURE_HFGRAPH();
	void SetMINSCALE_PRESSURE_HFGRAPH(double value);

	double GetMAXSCALE_PRESSURE_HFPVLOOP();
	void SetMAXSCALE_PRESSURE_HFPVLOOP(double value);

	double GetMINSCALE_PRESSURE_HFPVLOOP();
	void SetMINSCALE_PRESSURE_HFPVLOOP(double value);

	double GetMAXSCALE_VOLUME_HFPVLOOP();
	void SetMAXSCALE_VOLUME_HFPVLOOP(double value);

	double GetMAXSCALE_FLOW_HFVFLOOP();
	void SetMAXSCALE_FLOW_HFVFLOOP(double value);

	double GetMAXSCALE_VOLUME_HFVFLOOP();
	void SetMAXSCALE_VOLUME_HFVFLOOP(double value);

	double GetMAXSCALE_PRESSURE_PVLOOP();
	void SetMAXSCALE_PRESSURE_PVLOOP(double value);

	double GetMAXSCALE_VOLUME_PVLOOP();
	void SetMAXSCALE_VOLUME_PVLOOP(double value);

	double GetMAXSCALE_VOLUME_VFLOOP();
	void SetMAXSCALE_VOLUME_VFLOOP(double value);

	double GetMAXSCALE_FLOW_VFLOOP();
	void SetMAXSCALE_FLOW_VFLOOP(double value);

	double GetMAXSCALE_FOT_PRESSURE();
	void SetMAXSCALE_FOT_PRESSURE(double value);

	double GetMINSCALE_FOT_XRS();
	void SetMINSCALE_FOT_XRS(double value);

	double GetMAXSCALE_FOT_XRS();
	void SetMAXSCALE_FOT_XRS(double value);

	void TrendSetPInspMax(int value);
	int TrendGetPInspMax();

	void TrendSetPMeanMax(int value);
	int TrendGetPMeanMax();

	void TrendSetFIO2Max(int value);
	int TrendGetFIO2Max();

	void TrendSetVteMax(int value);
	int TrendGetVteMax();

	void TrendSetComplianceMax(int value);
	int TrendGetComplianceMax();

	void TrendSetDCO2Max(int value);
	int TrendGetDCO2Max();

	void TrendSetMVMax(int value);
	int TrendGetMVMax();

	void TrendSetHFAMPMax(int value);
	int TrendGetHFAMPMax();

	void TrendSetRSBIMax(int value);
	int TrendGetRSBIMax();

	void TrendSetSHAREMVMANDMax(int value);
	int TrendGetSHAREMVMANDMax();

	void TrendSetRESISTANCEMax(int value);
	int TrendGetRESISTANCEMax();

	void TrendSetLEAKMax(int value);
	int TrendGetLEAKMax();

	void TrendSetSPO2Max(int value);
	int TrendGetSPO2Max();

	void TrendSetSPO2PIMax(int value);
	int TrendGetSPO2PIMax();

	void TrendSetETCO2Max(int value);
	int TrendGetETCO2Max();

	void TrendSetSPO2PRMax(int value);
	int TrendGetSPO2PRMax();

	void TrendSetFREQUENCYMax(int value);
	int TrendGetFREQUENCYMax();

	void GraphSetActualSpeedGraph(int value);
	int GraphGetActualSpeedGraph();

	void GraphSetActualSpeedCLFIO2(int value);
	int GraphGetActualSpeedCLFIO2();

	UINT GetTrendSpan();
	void SetTrendSpan(UINT span);

	UINT GetTrendType1();
	void SetTrendType1(UINT type);

	UINT GetTrendType2();
	void SetTrendType2(UINT type);

	UINT GetTrendType3();
	void SetTrendType3(UINT type);

	void setPpsvAsDeltaPEEPValue(BOOL bState);
	BOOL isPpsvAsDeltaPEEPValue();

	void setLeakCompensation(eLeakCompensation leakComp);
	eLeakCompensation getLeakCompensation();

	void setNextServiceDate(WORD year, WORD month, WORD day);
	COleDateTime getNextServiceDate();

	void GraphSetPrimaryLineDiagramm(int value);
	int GraphGetPrimaryLineDiagramm();

	eAlarmLoudness GetAlarmVolume();
	void SetAlarmVolume(eAlarmLoudness state);

	int GetLastSelectedSVSettingBtns();
	void SetLastSelectedSVSettingBtns(int btns);

	int GetLastSelectedSVCO2Btns();
	void SetLastSelectedSVCO2Btns(int btns);

	int GetLastSelectedSVSPO2Btns();
	void SetLastSelectedSVSPO2Btns(int btns);

	eALimitNumeric getCurALimitNumeric();
	void setCurALimitNumeric(eALimitNumeric numeric);

	int GetLastSelectedALBtns();
	void SetLastSelectedALBtns(int btns);

	int GetLastSelectedParaViewBtns();
	void SetLastSelectedParaViewBtns(int btns);

	int getLastSelectedFOTViewBtns();
	void setLastSelectedFOTViewBtns(int btns);

	void readinFOTventDelaytime();
	WORD getFOTventDelaytime();

	CStringW GetPatientName();
	CStringW GetPatientFirstname();

	int GetPatientID();
	int GetPatientWeight();
	CStringW GetPatientBirthday();
	CStringW GetPatientRemark();
	void SetPatientData(CStringW name, CStringW firstName, int persID, int weight, CStringW  szTime, CStringW  szRemark);

	CStringW GetLastCalOxy21();
	void SetLastCalOxy21(CStringW szDateTime);

	CStringW GetLastCalOxy100();
	void SetLastCalOxy100(CStringW szDateTime);

	CStringW GetLastCalFlow();
	void SetLastCalFlow(CStringW szDateTime);

	CStringW GetLastCal0mbar();
	void SetLastCal0mbar(CStringW szDateTime);

	CStringW GetLastCal60mbar();
	void SetLastCal60mbar(CStringW szDateTime);

	DWORD getOperatingMinutesDevice();
	void setOperatingMinutesDevice(DWORD min, bool bFlushReg);

	DWORD getOperatingMinutesBattery();
	void setOperatingMinutesBattery(DWORD min, bool bFlushReg);

	DWORD getOperatingMinutesHFO();
	void setOperatingMinutesHFO(DWORD min, bool bFlushReg);

	void SetLastWrittenTrendFile(UINT type, BYTE count);
	BYTE GetLastWrittenTrendFile(UINT type);

	BYTE getLastNumericIPPV();
	void setLastNumericIPPV(BYTE num);

	BYTE getLastNumericSIPPV();
	void setLastNumericSIPPV(BYTE num);

	BYTE getLastNumericSIMV();
	void setLastNumericSIMV(BYTE num);

	BYTE getLastNumericSIMVPSV();
	void setLastNumericSIMVPSV(BYTE num);

	BYTE getLastNumericPSV();
	void setLastNumericPSV(BYTE num);

	BYTE getLastNumericCPAP();
	void setLastNumericCPAP(BYTE num);

	BYTE getLastNumericHFO();
	void setLastNumericHFO(BYTE num);

	BYTE getLastNumericNCPAP();
	void setLastNumericNCPAP(BYTE num);

	BYTE getLastNumericDUOPAP();
	void setLastNumericDUOPAP(BYTE num);

	BYTE getLastNumericTHERAPY();
	void setLastNumericTHERAPY(BYTE num);

	BYTE getLastNumericFLOWOFFCONV();
	void setLastNumericFLOWOFFCONV(BYTE num);

	BYTE getLastNumericFLOWOFFCPAP();
	void setLastNumericFLOWOFFCPAP(BYTE num);

	BYTE getLastNumericFLOWOFFHFO();
	void setLastNumericFLOWOFFHFO(BYTE num);

	int getConfigVersion();

	void disableNIVTRIGGER();
	COleDateTime GetNIVTRIGGERdemoTimestamp();
	void SetNIVTRIGGERdemoTimestamp(COleDateTime dateTime);

	COleDateTime GetFOTdemoTimestamp();
	void SetFOTdemoTimestamp(COleDateTime dateTime);

	COleDateTime GetPRICOdemoTimestamp();
	void SetPRICOdemoTimestamp(COleDateTime dateTime);

	COleDateTime GetTHERAPYdemoTimestamp();
	void SetTHERAPYdemoTimestamp(COleDateTime dateTime);

	COleDateTime GetTRENDdemoTimestamp();
	void SetTRENDdemoTimestamp(COleDateTime dateTime);

	COleDateTime GetLUNGRECdemoTimestamp();
	void SetLUNGRECdemoTimestamp(COleDateTime dateTime);

	COleDateTime GetVLIMITdemoTimestamp();
	void SetVLIMITdemoTimestamp(COleDateTime dateTime);

	COleDateTime GetVGUARANTYdemoTimestamp();
	void SetVGUARANTYdemoTimestamp(COleDateTime dateTime);

	COleDateTime GetNMODEdemoTimestamp();
	void SetNMODEdemoTimestamp(COleDateTime dateTime);

	COleDateTime GetHFOdemoTimestamp();
	void SetHFOdemoTimestamp(COleDateTime dateTime);

	void readAmpCorFactor();
	double getAmpCorFactor(BYTE iFreq);
	double getAmpCorFactorCONV_K();
	double getAmpCorFactorCONV_J();
	double getAmpCorFactorHFO_K();
	double getAmpCorFactorHFO_J();

private:
//singleton
	static CConfiguration* theConfig;   ///< the configuration
protected:

	CConfiguration();

	virtual ~CConfiguration(void);

	void LoadSettings();

	CMVModel *getModel();
	

private:
	CRITICAL_SECTION	csVentMode; ///< The create struct vent mode

	CMVModel *m_pModel; ///< The model

	eRatioIE		m_iParaDataHFIERatio;   ///< The para data hfie ratio
	eTubeConfig m_eTubeType;	///< Type of the tube
	eIERelationMode m_eIERelationMode;  ///< The IE relation mode
	eRefFlowSensor m_eRefFlowSensor;	///< The reference flow sensor
	eTubeSet m_eTubeSet;	///< Set the tube belongs to
	ePressureUnit m_ePressureUnit;  ///< The pressure unit
	eVolumeControl m_eVControl; ///< The v control
	eAutoOxyCal m_eAutoOxyCal;  ///< The automatic oxy calendar
	eAutoScreenlock m_eAutoScreenlocktime;  ///< The automatic screenlocktime
	eCO2unit  m_eCO2unit;   ///< The co 2unit
	eSPO2averagingtime m_eSPO2averagingTime;	///< The spo 2averaging time
	eSPO2sensitivitymode m_eSPO2SensitivityMode;	///< The spo 2 sensitivity mode
	eFlowSensorState m_eFlowSensorState;	///< State of the flow sensor
	eVentMode m_eCurMode;   ///< The current mode
	eVentMode m_ePrevMode;  ///< The previous mode
	eALimitNumeric m_eLastALimitNumeric;	///< The last a limit numeric
	eCurveForm m_eCurveForm;	///< The curve form
	eAlarmLoudness m_eAlarmVolume;  ///< The alarm volume
	eAcuLinkVersion m_eAcuLinkVersion;  ///< The acu link version

	CStringW m_szLanguageID;	///< Identifier for the language
	CStringW m_szKernel;	///< The kernel
	CStringW m_szAcuKernel; ///< The acu kernel
	CStringW m_szKernelDate;	///< The kernel date
	CStringW m_szNetDCUversion; ///< The net device-context uversion

	COleDateTime m_dtNextServiceDate;   ///< The dt next service date

	double dbBufAmpCorFactor[16];   ///< The database buffer amp cor factor[ 16]
	double dbAmpCorFactorCONV_K;	///< The database amp cor factor convert k
	double dbAmpCorFactorCONV_J;	///< The database amp cor factor convert j
	double dbAmpCorFactorHFO_K; ///< The database amp cor factor hfo k
	double dbAmpCorFactorHFO_J; ///< The database amp cor factor hfo j

	UINT m_iEthernetPort;   ///< The ethernet port
	UINT m_iTypeTrend1; ///< The first type trend
	UINT m_iTypeTrend2; ///< The second type trend
	UINT m_iTypeTrend3; ///< The third type trend
	UINT m_iTrendSpan;  ///< The trend span
	UINT m_iPatientAlarmDelay;  ///< The patient alarm delay
	eSPO2alarmdelay m_eSPO2alarmdelay;  ///< The spo 2alarmdelay
	
	WORD m_iFOTconv_AMPLITUDE;  ///< The fotconv amplitude
	BYTE m_iFOTconv_FREQ;   ///< The fotconv frequency
	WORD m_iFOTconv_PEEPSTART;  ///< The fotconv peepstart
	WORD m_iFOTconv_PEEPEND;	///< The fotconv peepend
	BYTE m_iFOTconv_STEPS;  ///< The fotconv steps
	WORD m_iDiffPEEPFOT;	///< The difference peepfot

	WORD m_iFOThfo_AMPLITUDE;   ///< The fothfo amplitude
	BYTE m_iFOThfo_FREQ;	///< The fothfo frequency
	WORD m_iFOThfo_PMEANSTART;  ///< The fothfo pmeanstart
	WORD m_iFOThfo_PMEANEND;	///< The fothfo pmeanend
	BYTE m_iFOThfo_STEPS;   ///< The fothfo steps
	WORD m_iDiffPmeanFOT;   ///< The difference pmean fot

	int m_iVentRange;   ///< The vent range
	int m_iPrimaryLineDiagramm; ///< The primary line diagramm
	int m_iManBreathTime;   ///< Manager for breath time
	int m_iNebulizerTime;   ///< The nebulizer time
	int m_iPminAlarmDelay;  ///< The pmin alarm delay
	int m_iPercentAbortCriterionPSV;	///< The percent abort criterion psv
	int m_iGraphSpeedHFO;   ///< The graph speed hfo
	int m_iGraphSpeedNORM;  ///< The graph speed normalise
	int m_iGraphSpeedCLFIO2;	///< The second graph speed clfio
	int m_GraphLastViewState;   ///< State of the graph last view
	//int m_iMAXSCALE_CLFIO2_GRAPH;
	//int m_iMAXSCALE_EMG_GRAPH;
	int m_iMAXSCALE_SPO2_GRAPH; ///< The maxscale spo 2 graph
	int m_iMAXSCALE_CO2_GRAPH;  ///< The maxscale co 2 graph
	int m_iMAXSCALE_FLOW_GRAPH; ///< The maxscale flow graph
	int m_iMAXSCALE_PRESSURE_GRAPH; ///< The maxscale pressure graph
	int m_iMAXSCALE_VOLUME_GRAPH;   ///< The maxscale volume graph
	int m_iMAXSCALE_SPO2_HFGRAPH;   ///< The maxscale spo 2 hfgraph
	int m_iMAXSCALE_CO2_HFGRAPH;	///< The maxscale co 2 hfgraph
	int m_iMAXSCALE_FLOW_HFGRAPH;   ///< The maxscale flow hfgraph
	int m_iMAXSCALE_VOLUME_HFGRAPH; ///< The maxscale volume hfgraph
	int m_iMAXSCALE_PRESSURE_HFGRAPH;   ///< The maxscale pressure hfgraph
	int m_iMINSCALE_PRESSURE_HFGRAPH;   ///< The minscale pressure hfgraph
	int m_iMAXSCALE_PRESSURE_HFPVLOOP;  ///< The maxscale pressure hfpvloop
	int m_iMINSCALE_PRESSURE_HFPVLOOP;  ///< The minscale pressure hfpvloop
	int m_iMAXSCALE_VOLUME_HFPVLOOP;	///< The maxscale volume hfpvloop
	int m_iMAXSCALE_FLOW_HFVFLOOP;  ///< The maxscale flow hfvfloop
	int m_iMAXSCALE_VOLUME_HFVFLOOP;	///< The maxscale volume hfvfloop
	int m_iMAXSCALE_PRESSURE_PVLOOP;	///< The maxscale pressure pvloop
	int m_iMAXSCALE_VOLUME_PVLOOP;  ///< The maxscale volume pvloop
	int m_iMAXSCALE_VOLUME_VFLOOP;  ///< The maxscale volume vfloop
	int m_iMAXSCALE_FLOW_VFLOOP;	///< The maxscale flow vfloop
	int m_iTrendPInspMax;   ///< The trend p insp maximum
	int m_iTrendPMeanMax;   ///< The trend p mean maximum
	int m_iTrendFIO2Max;	///< The trend fio 2 maximum
	int m_iTrendVteMax; ///< The trend vte maximum
	int m_iTrendComplianceMax;  ///< The trend compliance maximum
	int m_iTrendDCO2Max;	///< The trend dco 2 maximum
	int m_iTrendMVMax;  ///< The trend mv maximum
	int m_iTrendHFAMPMax;   ///< The trend hfamp maximum

	int m_iMAXSCALE_FOT_PRESSURE;   ///< The maxscale fot pressure
	double m_iMINSCALE_FOT_XRS; ///< The minscale fot xrs
	double m_iMAXSCALE_FOT_XRS; ///< The maxscale fot xrs

	int m_iTrendRSBIMax;	///< The trend rsbi maximum
	int m_iTrendSHAREMVMANDMax; ///< The trend sharemvmand maximum
	int m_iTrendRESISTANCEMax;  ///< The trend resistance maximum
	int m_iTrendLEAKMax;	///< The trend leak maximum
	int m_iTrendSPO2Max;	///< The trend spo2 maximum
	int m_iTrendSPO2PIMax;  ///< The trend spo2 pi maximum
	int m_iTrendETCO2Max;   ///< The trend etco2 maximum
	int m_iTrendSPO2PRMax;  ///< The trend spo2 pr maximum
	int m_iTrendFREQUENCYMax;   ///< The trend frequency maximum


	int m_iFOTViewBtn;  ///< The fot view button
	int m_iSetupView_SettingBtn;	///< The setup view setting button
	int m_iSetupView_CO2Btn;	///< The setup view co 2 button
	int m_iSetupView_SPO2Btn;   ///< The setup view spo 2 button
	int m_iALimit_Btn;  ///< The alarm limit button
	int m_iParaViewBtn_IPPV;	///< The para view button ippv
	int m_iParaViewBtn_PSV; ///< The para view button psv
	int m_iParaViewBtn_SIMV;	///< The para view button simv
	int m_iParaViewBtn_SIMVPSV; ///< The para view button simvpsv
	int m_iParaViewBtn_SIPPV;   ///< The para view button sippv
	int m_iParaViewBtn_CPAP;	///< The para view button cpap
	int m_iParaViewBtn_NCPAP;   ///< The para view button ncpap
	int m_iParaViewBtn_DUOPAP;  ///< The para view button duopap
	int m_iParaViewBtn_THERAPIE;	///< The para view button therapie
	int m_iParaViewBtn_HFO; ///< The para view button hfo
	int	m_iPmeanDifference; ///< The pmean difference
	int	m_iPPSVDifference;  ///< The ppsv difference
	int	m_iParaDataHFFreq;  ///< The para data hf frequency
	int m_iAlarmlimitSPO2max;   ///< The alarmlimit spo 2max
	int m_iAlarmlimitStateSPO2max;  ///< The alarmlimit state spo 2max
	int m_iAlarmlimitSPO2min;   ///< The alarmlimit spo 2min
	int m_iAlarmlimitStateSPO2min;  ///< The alarmlimit state spo 2min
	int m_iAlarmlimitPulseRatemax;  ///< The alarmlimit pulse ratemax
	int m_iAlarmlimitStatePulseRatemax; ///< The alarmlimit state pulse ratemax
	int m_iAlarmlimitPulseRatemin;  ///< The alarmlimit pulse ratemin
	int m_iAlarmlimitStatePulseRatemin; ///< The alarmlimit state pulse ratemin
	int m_iAlarmlimitSPO2_PImin;	///< The alarmlimit spo 2 p imin
	int m_iAlarmlimitStateSPO2_PImin;   ///< The alarmlimit state spo2 pimin
	int m_iAlarmlimitSPO2_SIQmin;   ///< The alarmlimit spo 2 SI qmin
	int m_iAlarmlimitStateSPO2_SIQmin;  ///< The alarmlimit state spo2 SIqmin

	int m_iAlarmlimitETCO2max;  ///< The alarmlimit etco2max
	int m_iAlarmlimitStateETCO2max; ///< The alarmlimit state etco2max
	int m_iAlarmlimitETCO2min;  ///< The alarmlimit etco2min
	int m_iAlarmlimitStateETCO2min; ///< The alarmlimit state etco2min

	int m_iAlarmlimitFICO2max;  ///< The alarmlimit fico2max
	int m_iAlarmlimitStateFICO2max; ///< The alarmlimit state fico2max
	int m_iAlarmlimitFICO2min;  ///< The alarmlimit fico2min
	int m_iAlarmlimitStateFICO2min; ///< The alarmlimit state fico2min

	int m_iAlarmlimitMVmax; ///< The alarmlimit mvmax
	int m_iAlarmlimitStateMVmax;	///< The alarmlimit state mvmax
	int m_iAlarmlimitMVmin; ///< The alarmlimit mvmin
	int m_iAlarmlimitStateMVmin;	///< The alarmlimit state mvmin
	int m_iAlarmlimitPIPmax;	///< The alarmlimit pi pmax
	int m_iAlarmlimitPIPmin;	///< The alarmlimit pi pmin
	int m_iAlarmlimitStatePIPmax;   ///< The alarmlimit state pi pmax
	int m_iAlarmlimitStatePIPmin;   ///< The alarmlimit state pi pmin
	int m_iAlarmlimitPEEPmin;   ///< The alarmlimit pee pmin
	int m_iAlarmlimitStatePEEPmin;  ///< The alarmlimit state pee pmin
	int m_iAlarmlimitBPMmax;	///< The alarmlimit bp mmax
	int m_iAlarmlimitStateBPMmax;   ///< The alarmlimit state bp mmax
	int m_iAlimitLeakmax;   ///< The alimit leakmax
	int m_iAlarmlimitStateLeakmax;  ///< The alarmlimit state leakmax
	int m_iAlarmlimitApnoe; ///< The alarmlimit apnoe
	int m_iAlarmlimitStateApnoe;	///< The alarmlimit state apnoe

	int m_iAlarmlimitSPO2maxHF; ///< The alarmlimit spo2max hf
	int m_iAlarmlimitStateSPO2maxHF;	///< The alarmlimit state spo2max hf
	int m_iAlarmlimitSPO2minHF; ///< The alarmlimit spo2min hf
	int m_iAlarmlimitStateSPO2minHF;	///< The alarmlimit state spo2min hf
	int m_iAlarmlimitPulseRatemaxHF;	///< The alarmlimit pulse ratemax hf
	int m_iAlarmlimitStatePulseRatemaxHF;   ///< The alarmlimit state pulse ratemax hf
	int m_iAlarmlimitPulseRateminHF;	///< The alarmlimit pulse ratemin hf
	int m_iAlarmlimitStatePulseRateminHF;   ///< The alarmlimit state pulse ratemin hf
	int m_iAlarmlimitSPO2_PIminHF;  ///< The alarmlimit spo2 pimin hf
	int m_iAlarmlimitStateSPO2_PIminHF; ///< The alarmlimit state spo2  imin hf
	int m_iAlarmlimitSPO2_SIQminHF; ///< The alarmlimit spo2 SIqmin hf
	int m_iAlarmlimitStateSPO2_SIQminHF;	///< The alarmlimit state spo2 SIqmin hf

	int m_iAlarmlimitETCO2maxHF;	///< The alarmlimit etco2max hf
	int m_iAlarmlimitStateETCO2maxHF;   ///< The alarmlimit state etco2max hf
	int m_iAlarmlimitETCO2minHF;	///< The alarmlimit etco2min hf
	int m_iAlarmlimitStateETCO2minHF;   ///< The alarmlimit state etco2min hf

	int m_iAlarmlimitFICO2maxHF;	///< The alarmlimit fico2max hf
	int m_iAlarmlimitStateFICO2maxHF;   ///< The alarmlimit state fico2max hf
	int m_iAlarmlimitFICO2minHF;	///< The alarmlimit fico2min hf
	int m_iAlarmlimitStateFICO2minHF;   ///< The alarmlimit state fico2min hf

	int m_iAlarmlimitMAPmaxHF;  ///< The alarmlimit ma pmax hf
	int m_iAlarmlimitStateMAPmaxHF; ///< The alarmlimit state ma pmax hf
	int m_iAlarmlimitMAPminHF;  ///< The alarmlimit ma pmin hf
	int m_iAlarmlimitStateMAPminHF; ///< The alarmlimit state ma pmin hf

	int m_iAlarmlimitDCO2maxHF; ///< The alarmlimit dco2max hf
	int m_iAlarmlimitStateDCO2maxHF;	///< The alarmlimit state dco2max hf
	int m_iAlarmlimitDCO2minHF; ///< The alarmlimit dco2min hf
	int m_iAlarmlimitStateDCO2minHF;	///< The alarmlimit state dco2min hf
	int m_iAlarmlimitMVmaxHF;   ///< The alarmlimit mvmax hf
	int m_iAlarmlimitStateMVmaxHF;  ///< The alarmlimit state mvmax hf
	int m_iAlarmlimitMVminHF;   ///< The alarmlimit mvmin hf
	int m_iAlarmlimitStateMVminHF;  ///< The alarmlimit state mvmin hf
	int m_iAlarmlimitPmaxHF;	///< The alarmlimit pmax hf
	int m_iAlarmlimitStatePmaxHF;   ///< The alarmlimit state pmax hf
	int m_iAlarmlimitPEEPminHF; ///< The alarmlimit pee pmin hf
	int m_iAlarmlimitStatePEEPminHF;	///< The alarmlimit state pee pmin hf
	int m_iAlarmlimitBPMmaxHF;  ///< The alarmlimit bp mmax hf
	int m_iAlarmlimitStateBPMmaxHF; ///< The alarmlimit state bp mmax hf
	int m_iAlarmlimitLeakmaxHF; ///< The alarmlimit leakmax hf
	int m_iAlarmlimitStateLeakmaxHF;	///< The alarmlimit state leakmax hf
	int m_iAlarmlimitApnoeHF;   ///< The alarmlimit apnoe hf
	int m_iAlarmlimitStateApnoeHF;  ///< The alarmlimit state apnoe hf
	int m_iAlarmlimitPmaxCpap;  ///< The alarmlimit pmax cpap
	int m_iAlarmlimitStatePmaxCpap; ///< The alarmlimit state pmax cpap
	int m_iAlarmlimitPEEPminCpap;   ///< The alarmlimit pee pmin cpap
	int m_iAlarmlimitStatePEEPminCpap;  ///< The alarmlimit state pee pmin cpap

	int m_iAlarmlimitPmaxDUOPAP;	///< The alarmlimit pmax duopap
	int m_iAlarmlimitStatePIPmaxDUOPAP; ///< The alarmlimit state pi pmax duopap
	int m_iAlarmlimitPEEPminDUOPAP; ///< The alarmlimit pee pmin duopap
	int m_iAlarmlimitStatePEEPminDUOPAP;	///< The alarmlimit state pee pmin duopap

	int m_iAlarmlimitPIPmaxNCPAP;   ///< The alarmlimit pi pmax ncpap
	int m_iAlarmlimitStatePmaxNCPAP;	///< The alarmlimit state pmax ncpap
	int m_iAlarmlimitPEEPminNCPAP;  ///< The alarmlimit pee pmin ncpap
	int m_iAlarmlimitStatePEEPminNCPAP; ///< The alarmlimit state pee pmin ncpap

	int m_iConfigVersion;   ///< The configuration version
	WORD m_wOldConfigVersion;   ///< The old configuration version

	
	SHORT		m_iParaDataPEEP_TRIGGER;	///< The para data peep trigger
	SHORT		m_iParaDataPEEP_IPPV;   ///< The para data peep ippv
	SHORT		m_iParaDataCpap;	///< The para data cpap
	//SHORT		m_iParaDataCpap_NMODE;
	SHORT		m_iParaDataNmode;   ///< The para data nmode
	//SHORT		m_iParaDataNmode_NMODE;
	SHORT		m_iParaDataPInsp_TRIGGER;   ///< The para data p insp trigger
	SHORT		m_iParaDataPInsp_IPPV;  ///< The para data p insp ippv
	SHORT		m_iParaDataPmaxVolG_TRIGGER;	///< The para data pmax volume g trigger
	SHORT		m_iParaDataPmaxVolG_IPPV;   ///< The para data pmax volume g ippv
	SHORT		m_iParaDataPPSV;	///< The para data ppsv
	SHORT		m_iParaDataPManual_HFO; ///< The para data p manual hfo
	SHORT		m_iParaDataPManual_CPAP;	///< The para data p manual cpap
	SHORT		m_iParaDataPManual_NMODE;   ///< The para data p manual nmode
	SHORT		m_iCO2BaroPressure; ///< The co2 baro pressure
	
	WORD		m_iFOTventDelaytime;	///< The fotvent delaytime
	WORD		m_iLanguageID;  ///< Identifier for the language
	//WORD		m_wHFamp;
	WORD		m_iParaDataITime_IPPV;  ///< The para data i time ippv
	WORD		m_iParaDataITime_TRIGGER;   ///< The para data i time trigger
	WORD		m_iParaDataITime_NMODE; ///< The para data i time nmode
	WORD		m_iParaDataETIME_IPPV;  ///< The para data etime ippv
	WORD		m_iParaDataETIME_TRIGGER;   ///< The para data etime trigger
	WORD		m_iParaDataETIME_NMODE; ///< The para data etime nmode
	WORD		m_iParaDataBPM_IPPV;	///< The para data bpm ippv
	WORD		m_iParaDataBPM_TRIGGER; ///< The para data bpm trigger
	WORD		m_iParaDataBPM_NMODE;   ///< The para data bpm nmode
	WORD		m_iParaDataIFlow_IPPV;  ///< The para data i flow ippv
	WORD		m_iParaDataIFlow_TRIGGER;   ///< The para data i flow trigger
	WORD		m_iParaDataHFFlow;  ///< The para data hf flow
	WORD		m_iParaDataRisetime_IPPV;   ///< The para data risetime ippv
	WORD		m_iParaDataRisetime_TRIGGER;	///< The para data risetime trigger
	WORD		m_iParaDataEFlow_IPPV;  ///< The para data e flow ippv
	WORD		m_iParaDataEFlow_TRIGGER;   ///< The para data e flow trigger
	WORD		m_iParaDataTherapieFlow;	///< The para data therapie flow
	WORD		m_iParaDataFlowMin; ///< The para data flow minimum
	WORD		m_iParaDataVLimit_TRIGGER;  ///< The para data v limit trigger
	WORD		m_iParaDataVLimit_IPPV; ///< The para data v limit ippv
	WORD		m_iParaDataVGarant_TRIGGER; ///< The para data v garant trigger
	WORD		m_iParaDataVGarant_IPPV;	///< The para data v garant ippv
	WORD		m_iParaDataHFAmpl;  ///< The para data hf ampl
	WORD		m_iParaDataHFAmplmax;   ///< The para data hf amplmax
	WORD		m_iParaDataHFITimeRec;  ///< The para data hfi time record
	WORD		m_iParaDataHFFREQ_REC;  ///< The para data hffreq record
	WORD		m_iParaDataHFPMeanRec;  ///< The para data hfp mean record
	WORD		m_iParaDataHFPmean; ///< The para data hf pmean
	WORD		m_iParaDataHFVGarant;   ///< The para data hfv garant

	BYTE	m_iCurNumericBlock_IPPV;	///< The current numeric block ippv
	BYTE	m_iCurNumericBlock_SIPPV;   ///< The current numeric block sippv
	BYTE	m_iCurNumericBlock_SIMV;	///< The current numeric block simv
	BYTE	m_iCurNumericBlock_SIMVPSV; ///< The current numeric block simvpsv
	BYTE	m_iCurNumericBlock_PSV; ///< The current numeric block psv
	BYTE	m_iCurNumericBlock_CPAP;	///< The current numeric block cpap
	BYTE	m_iCurNumericBlock_HFO; ///< The current numeric block hfo
	BYTE	m_iCurNumericBlock_NCPAP;   ///< The current numeric block ncpap
	BYTE	m_iCurNumericBlock_DUOPAP;  ///< The current numeric block duopap
	BYTE	m_iCurNumericBlock_THERAPY; ///< The current numeric block therapy
	BYTE	m_iCurNumericBlock_FLOWOFFCONV; ///< The current numeric block flowoffconv
	BYTE	m_iCurNumericBlock_FLOWOFFCPAP; ///< The current numeric block flowoffcpap
	BYTE	m_iCurNumericBlock_FLOWOFFHFO;  ///< The current numeric block flowoffhfo

	//BYTE	m_iParaDataTriggerCONV; ///< The para data trigger convert
	//BYTE	m_iParaDataTriggerNMODE;	///< The para data trigger nmode

	BYTE	m_iParaDataTrigger_CONV;	///< The para data trigger conventional (sippv, simv, simv+psv, psv)
	BYTE	m_iParaDataTrigger_CPAP;	///< The para data trigger cpap
	BYTE	m_iParaDataTrigger_NCPAP;   ///< The para data trigger ncpap
	BYTE	m_iParaDataTrigger_DUOPAP;  ///< The para data trigger duopap

	BYTE	m_iParaDataBackup;  ///< The para data backup
	BYTE	m_iPDMSProtocol;	///< The pdms protocol
	BYTE	m_iCO2module;   ///< The co2module
	BYTE	m_byCO2ModuleTemp;  ///< The by co2 module temporary
	BYTE	m_iSPO2module;  ///< The spo2module
	BYTE	m_bySPO2ModuleTemp; ///< The by spo2 module temporary
	BYTE	m_iMainBoard;   ///< The main board
	BYTE	m_byFlushTime;  ///< The by flush time
	BYTE	m_iCO2TimePumpAutoOn;   ///< The co2 time pump automatic on
	BYTE	m_iParaDataO2;  ///< The second para data o2
	BYTE	m_iParaDataO2Flush; ///< The para data o2 flush
	BYTE	m_iO2Difference;	///< The o2 difference

	BYTE	m_iPRICO_SPO2lowRange;  ///< The prico spo2low range
	BYTE	m_iPRICO_SPO2highRange; ///< The prico spo2high range
	BYTE	m_iPRICO_FIO2lowRange;  ///< The prico fio2low range
	BYTE	m_iPRICO_FIO2highRange; ///< The prico fio2high range

	//BYTE	m_iSPO2_SIQlimit;

	BYTE  m_iSystemHWrevision;  ///< The system h wrevision
	eDevType  m_eDevType;   ///< Type of the development
	eFlowSensorType  m_eFlowSensorType; ///< Type of the flow sensor
	eAVRevision  m_eAVRevision; ///< a v revision
	eHFModuleRevision m_eHFRevision;	///< The hf revision

	eTriggereType m_eTriggerType_CONV;   ///< The trigger type convert
	eTriggereType m_eTriggerType_CPAP;  ///< The trigger type cpap
	eTriggereType m_eTriggerType_DUOPAP;  ///< The trigger type duopap
	eTriggereType m_eTriggerType_NCPAP;  ///< The trigger type ncpap

	eLeakCompensation m_eLeakCompensation;  ///< The leak compensation

	bool m_bEFLOWequalILFOW;	///< True to eflo wequal ilfow
	bool m_bGraphAutoScale; ///< True to graph automatic scale
	bool m_bTrendAutoScale; ///< True to trend automatic scale
	bool m_bFilledGraph;	///< True to filled graph
	bool m_bFilledTrend;	///< True to filled trend
	bool m_bPlayVlimitSound;	///< True to play vlimit sound
	bool m_bPlayBackupSound;	///< True to play backup sound
	bool m_bHFOManBreathEnabled;	///< True to enable, false to disable the hfo manager breath
	bool m_bAutoScreenlockActive;   ///< True to automatically screenlock active
	bool m_bFastSATon;  ///< True to fast sa ton
	bool m_bUseTveBTB;  ///< True to use tve btb
	bool m_bUseNeoPed;  ///< True if use neo ped
	bool m_bBTPSenable; ///< True if btp senable
	bool m_bServicePasswordDisabled;	///< True to disable, false to enable the service password
	bool m_bNursecallActive;	///< True to nursecall active
	bool m_bEnableAutolimit;	///< True to enable, false to disable the autolimit
	bool m_bEnableFactory;  ///< True to enable, false to disable the factory
	bool m_bParaDataHFVGarantState; ///< True to para data hfv garant state
	bool m_bParaDataVLimitOn_TRIGGER;   ///< True to b para data v limit on trigger
	bool m_bParaDataVLimitOn_IPPV;  ///< True to b para data v limit on ippv
	bool m_bParaDataVGarantOn_TRIGGER;  ///< True to b para data v garant on trigger
	bool m_bParaDataVGarantOn_IPPV; ///< True to b para data v garant on ippv
	bool m_bFreshgasExt;	///< True to freshgas extent
	bool m_bShowEFlowInSettings;	///< True to show, false to hide the e flow in settings
	bool m_bSPO2registry;   ///< True to spo2registry
	bool m_bRISETIMEregistry;   ///< True to risetim eregistry
	bool m_bCO2registry;	///< True to co2registry
	bool m_bBiasFlow;   ///< True to bias flow
	bool m_bSpO2ConfigInProgress;   ///< True to spo2 configuration in progress
	BOOL m_bPPSVasDeltaPEEPValue;   ///< True to ppsv as delta peep value
	

};



