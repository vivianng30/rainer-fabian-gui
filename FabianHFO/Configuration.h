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
	//virtual void SerializeFile2(CArchive& ar);

	static CConfiguration* GetInstance();
	static void DestroyInstance();

	void Init();
	//void LoadDefaults();

	CStringW GetLanguageNamefromID(WORD iID);
	WORD GetLanguageIDfromName(CStringW szLID);
	WORD GetLanguageID();
	CStringW getLANGUAGE();
	void SetLanguage(CStringW szLID);

	//HARDWARE CONFIGURATION
	BYTE getSystemHWrevision();
	eDevType getDevType();
	eFlowSensorType getFlowSensorType();
	eAVRevision getAVRevision();
	eHFModuleRevision getHFModuleRevision();
	BYTE GetMainBoardVersion();
	void setMainBoardVersion(UINT vers);
	void setTempCO2module(BYTE mod);
	BYTE getTempCO2module();
	BYTE GetCO2module();
	void SetCO2module(BYTE mod);
	BYTE GetPDMSprotocol();
	void SetPDMSprotocol(BYTE prot);
	UINT GetEthernetPort();
	void SetEthernetPort(UINT port);
	void setTempSPO2module(BYTE mod);
	BYTE getTempSPO2module();
	BYTE GetSPO2module();
	void SetSPO2module(BYTE mod, bool bReinit=false);
	bool isSpO2ConfigInProgress();

	eAcuLinkVersion GetAcuLinkVersion();
	void SetAcuLinkVersion(eAcuLinkVersion vers);

	//FABIANHFO CONFIGURATION
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
	/*BYTE GetPDMSwave1();
	void SetPDMSwave1(BYTE wave);
	BYTE GetPDMSwave2();
	void SetPDMSwave2(BYTE wave);*/

	void GraphSetAutoScale(bool state);
	bool GraphIsAutoScale();

	void GraphSetFilled(bool state);
	bool GraphIsFilled();

	void TrendSetAutoScale(bool state);
	bool TrendIsAutoScale();

	void TrendSetFilled(bool state);
	bool TrendIsFilled();

	/*void SetPatDataStartup(bool state);
	bool AskPatDataStartup();*/

	void setTriggerOptionCONV(eTriggereType type);
	eTriggereType getTriggerOptionCONV();

	void setTriggerOptionNMODE(eTriggereType type);
	eTriggereType getTriggerOptionNMODE();


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
	//void SetPlayVlimitSound(bool bState);

	bool GetPlayBackupSound();
	//void SetPlayBackupSound(bool bState);

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


	////parameter values
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

	//SHORT		GetParaDataCPAP();
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
	
	//SHORT		GetParaDataPManual_CPAP();
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

	BYTE		GetParaDataTriggerCONV();
	void		SetParaDataTriggerCONV(BYTE iVal);

	BYTE		GetParaDataTriggerNMODE();
	void		SetParaDataTriggerNMODE(BYTE iVal);

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
	

	


	/*bool IsHFOFeatureAvailable();
	bool IsNMODEFeatureAvailable();
	bool IsCO2FeatureAvailable();
	bool IsVGarantFeatureAvailable();
	bool IsVLimitFeatureAvailable();*/
	
	bool DeaktivateVGarantState();
	bool DeaktivateVLimitState();
	
	
	/*bool IsCO2ModuleEnabled();
	void SetCO2ModuleEnabled(bool state);*/

	eCO2unit GetCO2unit();
	void SetCO2unit(eCO2unit unit);

	SHORT GetCO2BaroPressure();
	void SetCO2BaroPressure(SHORT value);

	/*SHORT GetO2Compensation();
	void SetO2Compensation(BYTE value);*/
	/*BYTE getSpO2SIQlimit();
	void setSpO2SIQlimit(BYTE iSIQval);*/

	bool isFastSATon();
	void setFastSATon(bool bFastSATon);

	eSPO2sensitivitymode getSensitivityMode();
	void setSensitivityMode(eSPO2sensitivitymode mode);

	UINT getSPO2alarmdelay();
	void setSPO2alarmdelay(UINT delay);

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
	bool isNIVTRIGGERREGISTRYenabled();

	BYTE getPRICO_SPO2lowRange();
	void setPRICO_SPO2lowRange(BYTE SPO2low);
	BYTE getPRICO_SPO2highRange();
	void setPRICO_SPO2highRange(BYTE SPO2high);

	BYTE getPRICO_FIO2lowRange();
	void setPRICO_FIO2lowRange(BYTE FIO2low);
	BYTE getPRICO_FIO2highRange();
	void setPRICO_FIO2highRange(BYTE FIO2high);
	//bool isPRICOenabled();

	ePressureUnit GetPressureUnit();
	void SetPressureUnit(ePressureUnit eUnit);

	eVolumeControl GetVolumeControl();
	void SetVolumeControl(eVolumeControl eCtrl);

	eFlowSensorState GetFlowSensorState();
	void SetFlowSensorState(eFlowSensorState state);

	//Current Alarmlimits
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


	//Current Alarmlimits HFO

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

	//NMODE
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

	/*double GetMAXSCALE_CLFIO2_GRAPH();
	void SetMAXSCALE_CLFIO2_GRAPH(double value);
	double GetMAXSCALE_EMG_GRAPH();
	void SetMAXSCALE_EMG_GRAPH(double value);*/
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

	void setLeakCompOff(eLeakCompensation leakComp);
	eLeakCompensation getLeakCompOff();

	void setNextServiceDate(WORD year, WORD month, WORD day);
	COleDateTime getNextServiceDate();

	void GraphSetPrimaryLineDiagramm(int value);
	int GraphGetPrimaryLineDiagramm();


	eAlarmLoudness GetAlarmVolume();
	void SetAlarmVolume(eAlarmLoudness state);

	//SetuView
	//eSetupView GetSetupView();
	//void SetSetupView(eSetupView view);
	int GetLastSelectedSVSettingBtns();
	void SetLastSelectedSVSettingBtns(int btns);

	int GetLastSelectedSVCO2Btns();
	void SetLastSelectedSVCO2Btns(int btns);
	
	int GetLastSelectedSVSPO2Btns();
	void SetLastSelectedSVSPO2Btns(int btns);

	eALimitNumeric getCurALimitNumeric();
	void setCurALimitNumeric(eALimitNumeric numeric);

	//SetuView
	/*eServiceView GetServiceView();
	void SetServiceView(eServiceView view);*/

	//AlarmLimits
	int GetLastSelectedALBtns();
	void SetLastSelectedALBtns(int btns);

	//ParaBtnView
	int GetLastSelectedParaViewBtns();
	void SetLastSelectedParaViewBtns(int btns);

	//FOTBtnView
	int getLastSelectedFOTViewBtns();
	void setLastSelectedFOTViewBtns(int btns);

	void readinFOTventDelaytime();
	WORD getFOTventDelaytime();

	//data of patient
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

	/*bool IsHFOFeatureAvailable();

	bool IsNMODEFeatureAvailable();*/

	void SetLastWrittenTrendFile(UINT type, BYTE count);
	BYTE GetLastWrittenTrendFile(UINT type);

	/*void SetSystemErrors(WORD errors);
	WORD GetSystemErrors();*/



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

	//AmplitudeCorrectionFactor
	void readAmpCorFactor();
	double getAmpCorFactor(BYTE iFreq);

	double getAmpCorFactorCONV_K();
	double getAmpCorFactorCONV_J();
	double getAmpCorFactorHFO_K();
	double getAmpCorFactorHFO_J();

private:
//singleton
	static CConfiguration* theConfig;
protected:
	CConfiguration();
	virtual ~CConfiguration(void);

	

	void LoadSettings();
	
	CMVModel *getModel();
	

private:
	CRITICAL_SECTION	csVentMode;

	CMVModel *m_pModel;

	eRatioIE		m_iParaDataHFIERatio;
	eTubeConfig m_eTubeType;
	eIERelationMode m_eIERelationMode;
	eRefFlowSensor m_eRefFlowSensor;
	eTubeSet m_eTubeSet;
	ePressureUnit m_ePressureUnit;
	eVolumeControl m_eVControl;
	eAutoOxyCal m_eAutoOxyCal;
	eAutoScreenlock m_eAutoScreenlocktime;
	eCO2unit  m_eCO2unit;
	eSPO2averagingtime m_eSPO2averagingTime;
	eSPO2sensitivitymode m_eSPO2SensitivityMode;
	eFlowSensorState m_eFlowSensorState;
	eVentMode m_eCurMode;
	eVentMode m_ePrevMode;
	eALimitNumeric m_eLastALimitNumeric;
	eCurveForm m_eCurveForm;
	eAlarmLoudness m_eAlarmVolume;
	eAcuLinkVersion m_eAcuLinkVersion;

	CStringW m_szLanguageID;
	CStringW m_szKernel;
	CStringW m_szAcuKernel;
	CStringW m_szKernelDate;
	CStringW m_szNetDCUversion;

	COleDateTime m_dtNextServiceDate;

	double dbBufAmpCorFactor[16];
	double dbAmpCorFactorCONV_K;
	double dbAmpCorFactorCONV_J;
	double dbAmpCorFactorHFO_K;
	double dbAmpCorFactorHFO_J;

	UINT m_iEthernetPort;
	UINT m_iTypeTrend1;
	UINT m_iTypeTrend2;
	UINT m_iTypeTrend3;
	UINT m_iTrendSpan;
	UINT m_iPatientAlarmDelay;
	UINT m_iSPO2alarmdelay;
	
	WORD m_iFOTconv_AMPLITUDE;
	BYTE m_iFOTconv_FREQ;
	WORD m_iFOTconv_PEEPSTART;
	WORD m_iFOTconv_PEEPEND;
	BYTE m_iFOTconv_STEPS;
	WORD m_iDiffPEEPFOT;

	WORD m_iFOThfo_AMPLITUDE;
	BYTE m_iFOThfo_FREQ;
	WORD m_iFOThfo_PMEANSTART;
	WORD m_iFOThfo_PMEANEND;
	BYTE m_iFOThfo_STEPS;
	WORD m_iDiffPmeanFOT;

	int m_iVentRange;
	int m_iPrimaryLineDiagramm;
	int m_iManBreathTime;
	int m_iNebulizerTime;
	int m_iPminAlarmDelay;
	int m_iPercentAbortCriterionPSV;
	int m_iGraphSpeedHFO;
	int m_iGraphSpeedNORM;
	int m_iGraphSpeedCLFIO2;
	int m_GraphLastViewState;
	//int m_iMAXSCALE_CLFIO2_GRAPH;
	//int m_iMAXSCALE_EMG_GRAPH;
	int m_iMAXSCALE_SPO2_GRAPH;
	int m_iMAXSCALE_CO2_GRAPH;
	int m_iMAXSCALE_FLOW_GRAPH;
	int m_iMAXSCALE_PRESSURE_GRAPH;
	int m_iMAXSCALE_VOLUME_GRAPH;
	int m_iMAXSCALE_SPO2_HFGRAPH;
	int m_iMAXSCALE_CO2_HFGRAPH;
	int m_iMAXSCALE_FLOW_HFGRAPH;
	int m_iMAXSCALE_VOLUME_HFGRAPH;
	int m_iMAXSCALE_PRESSURE_HFGRAPH;
	int m_iMINSCALE_PRESSURE_HFGRAPH;
	int m_iMAXSCALE_PRESSURE_HFPVLOOP;
	int m_iMINSCALE_PRESSURE_HFPVLOOP;
	int m_iMAXSCALE_VOLUME_HFPVLOOP;
	int m_iMAXSCALE_FLOW_HFVFLOOP;
	int m_iMAXSCALE_VOLUME_HFVFLOOP;
	int m_iMAXSCALE_PRESSURE_PVLOOP;
	int m_iMAXSCALE_VOLUME_PVLOOP;
	int m_iMAXSCALE_VOLUME_VFLOOP;
	int m_iMAXSCALE_FLOW_VFLOOP;
	int m_iTrendPInspMax;
	int m_iTrendPMeanMax;
	int m_iTrendFIO2Max;
	int m_iTrendVteMax;
	int m_iTrendComplianceMax;
	int m_iTrendDCO2Max;
	int m_iTrendMVMax;
	int m_iTrendHFAMPMax;

	int m_iMAXSCALE_FOT_PRESSURE;
	double m_iMINSCALE_FOT_XRS;
	double m_iMAXSCALE_FOT_XRS;

	int m_iTrendRSBIMax;
	int m_iTrendSHAREMVMANDMax;
	int m_iTrendRESISTANCEMax;
	int m_iTrendLEAKMax;
	int m_iTrendSPO2Max;
	int m_iTrendSPO2PIMax;
	int m_iTrendETCO2Max;
	int m_iTrendSPO2PRMax;
	int m_iTrendFREQUENCYMax;


	int m_iFOTViewBtn;
	int m_iSetupView_SettingBtn;
	int m_iSetupView_CO2Btn;
	int m_iSetupView_SPO2Btn;
	int m_iALimit_Btn;
	int m_iParaViewBtn_IPPV;
	int m_iParaViewBtn_PSV;
	int m_iParaViewBtn_SIMV;
	int m_iParaViewBtn_SIMVPSV;
	int m_iParaViewBtn_SIPPV;
	int m_iParaViewBtn_CPAP;
	int m_iParaViewBtn_NCPAP;
	int m_iParaViewBtn_DUOPAP;
	int m_iParaViewBtn_THERAPIE;
	int m_iParaViewBtn_HFO;
	int	m_iPmeanDifference;
	int	m_iPPSVDifference;
	int	m_iParaDataHFFreq;
	int m_iAlarmlimitSPO2max;
	int m_iAlarmlimitStateSPO2max;
	int m_iAlarmlimitSPO2min;
	int m_iAlarmlimitStateSPO2min;
	int m_iAlarmlimitPulseRatemax;
	int m_iAlarmlimitStatePulseRatemax;
	int m_iAlarmlimitPulseRatemin;
	int m_iAlarmlimitStatePulseRatemin;
	int m_iAlarmlimitSPO2_PImin;
	int m_iAlarmlimitStateSPO2_PImin;
	int m_iAlarmlimitSPO2_SIQmin;
	int m_iAlarmlimitStateSPO2_SIQmin;

	int m_iAlarmlimitETCO2max;
	int m_iAlarmlimitStateETCO2max;
	int m_iAlarmlimitETCO2min;
	int m_iAlarmlimitStateETCO2min;

	int m_iAlarmlimitFICO2max;
	int m_iAlarmlimitStateFICO2max;
	int m_iAlarmlimitFICO2min;
	int m_iAlarmlimitStateFICO2min;

	int m_iAlarmlimitMVmax;
	int m_iAlarmlimitStateMVmax;
	int m_iAlarmlimitMVmin;
	int m_iAlarmlimitStateMVmin;
	int m_iAlarmlimitPIPmax;
	int m_iAlarmlimitPIPmin;
	int m_iAlarmlimitStatePIPmax;
	int m_iAlarmlimitStatePIPmin;
	int m_iAlarmlimitPEEPmin;
	int m_iAlarmlimitStatePEEPmin;
	int m_iAlarmlimitBPMmax;
	int m_iAlarmlimitStateBPMmax;
	int m_iAlimitLeakmax;
	int m_iAlarmlimitStateLeakmax;
	int m_iAlarmlimitApnoe;
	int m_iAlarmlimitStateApnoe;

	int m_iAlarmlimitSPO2maxHF;
	int m_iAlarmlimitStateSPO2maxHF;
	int m_iAlarmlimitSPO2minHF;
	int m_iAlarmlimitStateSPO2minHF;
	int m_iAlarmlimitPulseRatemaxHF;
	int m_iAlarmlimitStatePulseRatemaxHF;
	int m_iAlarmlimitPulseRateminHF;
	int m_iAlarmlimitStatePulseRateminHF;
	int m_iAlarmlimitSPO2_PIminHF;
	int m_iAlarmlimitStateSPO2_PIminHF;
	int m_iAlarmlimitSPO2_SIQminHF;
	int m_iAlarmlimitStateSPO2_SIQminHF;

	int m_iAlarmlimitETCO2maxHF;
	int m_iAlarmlimitStateETCO2maxHF;
	int m_iAlarmlimitETCO2minHF;
	int m_iAlarmlimitStateETCO2minHF;

	int m_iAlarmlimitFICO2maxHF;
	int m_iAlarmlimitStateFICO2maxHF;
	int m_iAlarmlimitFICO2minHF;
	int m_iAlarmlimitStateFICO2minHF;

	int m_iAlarmlimitDCO2maxHF;
	int m_iAlarmlimitStateDCO2maxHF;
	int m_iAlarmlimitDCO2minHF;
	int m_iAlarmlimitStateDCO2minHF;
	int m_iAlarmlimitMVmaxHF;
	int m_iAlarmlimitStateMVmaxHF;
	int m_iAlarmlimitMVminHF;
	int m_iAlarmlimitStateMVminHF;
	int m_iAlarmlimitPmaxHF;
	int m_iAlarmlimitStatePmaxHF;
	int m_iAlarmlimitPEEPminHF;
	int m_iAlarmlimitStatePEEPminHF;
	int m_iAlarmlimitBPMmaxHF;
	int m_iAlarmlimitStateBPMmaxHF;
	int m_iAlarmlimitLeakmaxHF;
	int m_iAlarmlimitStateLeakmaxHF;
	int m_iAlarmlimitApnoeHF;
	int m_iAlarmlimitStateApnoeHF;
	int m_iAlarmlimitPmaxCpap;
	int m_iAlarmlimitStatePmaxCpap;
	int m_iAlarmlimitPEEPminCpap;
	int m_iAlarmlimitStatePEEPminCpap;

	int m_iAlarmlimitPmaxDUOPAP;
	int m_iAlarmlimitStatePIPmaxDUOPAP;
	int m_iAlarmlimitPEEPminDUOPAP;
	int m_iAlarmlimitStatePEEPminDUOPAP;

	int m_iAlarmlimitPIPmaxNCPAP;
	int m_iAlarmlimitStatePmaxNCPAP;
	int m_iAlarmlimitPEEPminNCPAP;
	int m_iAlarmlimitStatePEEPminNCPAP;

	int m_iConfigVersion;
	WORD m_wOldConfigVersion;

	
	SHORT		m_iParaDataPEEP_TRIGGER;
	SHORT		m_iParaDataPEEP_IPPV;
	SHORT		m_iParaDataCpap;
	//SHORT		m_iParaDataCpap_NMODE;
	SHORT		m_iParaDataNmode;
	//SHORT		m_iParaDataNmode_NMODE;
	SHORT		m_iParaDataPInsp_TRIGGER;
	SHORT		m_iParaDataPInsp_IPPV;
	SHORT		m_iParaDataPmaxVolG_TRIGGER;
	SHORT		m_iParaDataPmaxVolG_IPPV;
	SHORT		m_iParaDataPPSV;
	SHORT		m_iParaDataPManual_HFO;
	SHORT		m_iParaDataPManual_CPAP;
	SHORT		m_iParaDataPManual_NMODE;
	SHORT		m_iCO2BaroPressure;
	
	WORD		m_iFOTventDelaytime;
	WORD		m_iLanguageID;
	//WORD		m_wHFamp;
	WORD		m_iParaDataITime_IPPV;
	WORD		m_iParaDataITime_TRIGGER;
	WORD		m_iParaDataITime_NMODE;
	WORD		m_iParaDataETIME_IPPV;
	WORD		m_iParaDataETIME_TRIGGER;
	WORD		m_iParaDataETIME_NMODE;
	WORD		m_iParaDataBPM_IPPV;
	WORD		m_iParaDataBPM_TRIGGER;
	WORD		m_iParaDataBPM_NMODE;
	WORD		m_iParaDataIFlow_IPPV;
	WORD		m_iParaDataIFlow_TRIGGER;
	WORD		m_iParaDataHFFlow;
	WORD		m_iParaDataRisetime_IPPV;
	WORD		m_iParaDataRisetime_TRIGGER;
	WORD		m_iParaDataEFlow_IPPV;
	WORD		m_iParaDataEFlow_TRIGGER;
	WORD		m_iParaDataTherapieFlow;
	WORD		m_iParaDataFlowMin;
	WORD		m_iParaDataVLimit_TRIGGER;
	WORD		m_iParaDataVLimit_IPPV;
	WORD		m_iParaDataVGarant_TRIGGER;
	WORD		m_iParaDataVGarant_IPPV;
	WORD		m_iParaDataHFAmpl;
	WORD		m_iParaDataHFAmplmax;
	WORD		m_iParaDataHFITimeRec;
	WORD		m_iParaDataHFFREQ_REC;
	WORD		m_iParaDataHFPMeanRec;
	WORD		m_iParaDataHFPmean;
	WORD		m_iParaDataHFVGarant;

	BYTE	m_iCurNumericBlock_IPPV;
	BYTE	m_iCurNumericBlock_SIPPV;
	BYTE	m_iCurNumericBlock_SIMV;
	BYTE	m_iCurNumericBlock_SIMVPSV;
	BYTE	m_iCurNumericBlock_PSV;
	BYTE	m_iCurNumericBlock_CPAP;
	BYTE	m_iCurNumericBlock_HFO;
	BYTE	m_iCurNumericBlock_NCPAP;
	BYTE	m_iCurNumericBlock_DUOPAP;
	BYTE	m_iCurNumericBlock_THERAPY;
	BYTE	m_iCurNumericBlock_FLOWOFFCONV;
	BYTE	m_iCurNumericBlock_FLOWOFFCPAP;
	BYTE	m_iCurNumericBlock_FLOWOFFHFO;

	BYTE	m_iParaDataTriggerCONV;
	BYTE	m_iParaDataTriggerNMODE;

	BYTE	m_iParaDataBackup;
	BYTE	m_iPDMSProtocol;
	BYTE	m_iCO2module;
	BYTE	m_byCO2ModuleTemp;
	BYTE	m_iSPO2module;
	BYTE	m_bySPO2ModuleTemp;
	BYTE	m_iMainBoard;
	BYTE	m_byFlushTime;
	BYTE	m_iCO2TimePumpAutoOn;
	BYTE	m_iParaDataO2;
	BYTE	m_iParaDataO2Flush;
	BYTE	m_iO2Difference;

	BYTE	m_iPRICO_SPO2lowRange;
	BYTE	m_iPRICO_SPO2highRange;
	BYTE	m_iPRICO_FIO2lowRange;
	BYTE	m_iPRICO_FIO2highRange;

	//BYTE	m_iSPO2_SIQlimit;

	BYTE  m_iSystemHWrevision;
	eDevType  m_eDevType;
	eFlowSensorType  m_eFlowSensorType;
	eAVRevision  m_eAVRevision;
	eHFModuleRevision m_eHFRevision;

	eTriggereType m_eTriggerTypeCONV;
	eTriggereType m_eTriggerTypeNMODE;

	eLeakCompensation m_eLeakCompOff;

	bool m_bEFLOWequalILFOW;
	bool m_bGraphAutoScale;
	bool m_bTrendAutoScale;
	bool m_bFilledGraph;
	bool m_bFilledTrend;
	bool m_bPlayVlimitSound;
	bool m_bPlayBackupSound;
	bool m_bHFOManBreathEnabled;
	bool m_bAutoScreenlockActive;
	bool m_bFastSATon;
	bool m_bUseTveBTB;
	bool m_bUseNeoPed;
	bool m_bBTPSenable;
	bool m_bServicePasswordDisabled;
	bool m_bNursecallActive;
	bool m_bEnableAutolimit;
	bool m_bEnableFactory;
	bool m_bParaDataHFVGarantState;
	bool m_bParaDataVLimitOn_TRIGGER;
	bool m_bParaDataVLimitOn_IPPV;
	bool m_bParaDataVGarantOn_TRIGGER;
	bool m_bParaDataVGarantOn_IPPV;
	bool m_bFreshgasExt;
	bool m_bShowEFlowInSettings;
	bool m_bSPO2registry;
	bool m_bRISETIMEregistry;
	bool m_bCO2registry;
	bool m_bBiasFlow;
	bool m_bSpO2ConfigInProgress;
	bool m_bNIVTRIGGERregistry;
	BOOL m_bPPSVasDeltaPEEPValue;
	

};



