/**********************************************************************************************//**
 * \class	CAlarmConditionHandler
 *
 * \brief	An alarm condition handler.
 * The alarm condition handler handles states and limits of the alarms limits.
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 * 			
 **************************************************************************************************/

#pragma once
#include "AlarmMgmtHandler.h"
#include "AlarmLimitList.h"

class CAlarmConditionHandler 
{
public:

	CAlarmConditionHandler();

	virtual ~CAlarmConditionHandler(void);

	void ventModeChanged();

	void SetAlimitStateCalculated();

	eAlarmLimitState getAlimitState_MVmaxLimit();
	eAlarmLimitState getAlimitState_MVminLimit();
	eAlarmLimitState getAlimitState_PIPmaxLimit();
	eAlarmLimitState getAlimitState_PIPminLimit();
	eAlarmLimitState getAlimitState_PEEPminLimit();
	eAlarmLimitState getAlimitState_BPMmaxLimit();
	eAlarmLimitState getAlimitState_LeakmaxLimit();
	eAlarmLimitState getAlimitState_ApnoeLimit();
	eAlarmLimitState getAlimitState_MAPmaxLimit();
	eAlarmLimitState getAlimitState_MAPminLimit();
	eAlarmLimitState getAlimitState_DCO2maxLimit();
	eAlarmLimitState getAlimitState_DCO2minLimit();
	eAlarmLimitState getAlimitState_ETCO2maxLimit();
	eAlarmLimitState getAlimitState_ETCO2minLimit();
	eAlarmLimitState getAlimitState_FICO2maxLimit();
	eAlarmLimitState getAlimitState_FICO2minLimit();
	eAlarmLimitState getAlimitState_SPO2maxLimit();
	eAlarmLimitState getAlimitState_SPO2minLimit();
	eAlarmLimitState getAlimitState_PulseRatemaxLimit();
	eAlarmLimitState getAlimitState_PulseRateminLimit();
	eAlarmLimitState getAlimitState_SPO2_PIminLimit();
	eAlarmLimitState getAlimitState_SPO2_SIQminLimit();

	void setAlimitState_ApnoeLimit(eAlarmLimitState state);
	void setAlimitStateApnoeHF(eAlarmLimitState state);
	void setAlimitStateApnoeDefault(eAlarmLimitState state);

	void setAlimitState_BPMmaxLimit(eAlarmLimitState state);
	void setAlimitStateBPMmaxHF(eAlarmLimitState state);
	void setAlimitStateBPMmaxDefault(eAlarmLimitState state);

	void setAlimitState_LeakmaxLimit(eAlarmLimitState state);
	void setAlimitStateLeakmaxHF(eAlarmLimitState state);
	void setAlimitStateLeakmaxDefault(eAlarmLimitState state);

	void setAlimitState_MVmaxLimit(eAlarmLimitState state);
	void setAlimitStateMVmaxHF(eAlarmLimitState state);
	void setAlimitStateMVmaxDefault(eAlarmLimitState state);

	void setAlimitState_MVminLimit(eAlarmLimitState state);
	void setAlimitStateMVminHF(eAlarmLimitState state);
	void setAlimitStateMVminDefault(eAlarmLimitState state);

	void setAlimitState_PEEPminLimit(eAlarmLimitState state);
	void setAlimitStatePEEPminNCPAP(eAlarmLimitState state);
	void setAlimitStatePEEPminCpap(eAlarmLimitState state);
	void setAlimitStatePEEPminDUOPAP(eAlarmLimitState state);
	void setAlimitStatePEEPminDefault(eAlarmLimitState state);

	void setAlimitState_PIPminLimit(eAlarmLimitState state);
	void setAlimitStatePIPminDefault(eAlarmLimitState state);
	void setAlimitStatePIPminDUOPAP(eAlarmLimitState state);

	void setAlimitState_PIPmaxLimit(eAlarmLimitState state);
	void setAlimitStatePIPmaxHF(eAlarmLimitState state);
	void setAlimitStatePIPmaxCpap(eAlarmLimitState state);
	void setAlimitStatePIPmaxNCPAP(eAlarmLimitState state);
	void setAlimitStatePIPmaxDUOPAP(eAlarmLimitState state);
	void setAlimitStatePIPmaxDefault(eAlarmLimitState state);

	void setAlimitState_MAPmaxLimit(eAlarmLimitState state);
	void setAlimitStateMAPmaxHF(eAlarmLimitState state);
	void setAlimitState_MAPminLimit(eAlarmLimitState state);
	void setAlimitStateMAPminHF(eAlarmLimitState state);

	void setAlimitState_DCO2maxLimit(eAlarmLimitState state);
	void setAlimitStateDCO2maxHF(eAlarmLimitState state);
	void setAlimitState_DCO2minLimit(eAlarmLimitState state);
	void setAlimitStateDCO2minHF(eAlarmLimitState state);

	void setAlimitState_ETCO2maxLimit(eAlarmLimitState state);
	void setAlimitStateETCO2maxHF(eAlarmLimitState state);
	void setAlimitStateETCO2maxDefault(eAlarmLimitState state);
	void setAlimitState_ETCO2minLimit(eAlarmLimitState state);
	void setAlimitStateETCO2minHF(eAlarmLimitState state);
	void setAlimitStateETCO2minDefault(eAlarmLimitState state);

	void setAlimitState_FICO2maxLimit(eAlarmLimitState state);
	void setAlimitStateFICO2maxHF(eAlarmLimitState state);
	void setAlimitStateFICO2maxDefault(eAlarmLimitState state);
	void setAlimitState_FICO2minLimit(eAlarmLimitState state);
	void setAlimitStateFICO2minHF(eAlarmLimitState state);
	void setAlimitStateFICO2minDefault(eAlarmLimitState state);

	void setAlimitState_SPO2maxLimit(eAlarmLimitState state);
	void setAlimitStateSPO2maxDefault(eAlarmLimitState state);
	void setAlimitState_SPO2minLimit(eAlarmLimitState state);
	void setAlimitStateSPO2minDefault(eAlarmLimitState state);

	void setAlimitState_PulseRatemaxLimit(eAlarmLimitState state);
	void setAlimitStatePulseRatemaxDefault(eAlarmLimitState state);
	void setAlimitState_PulseRateminLimit(eAlarmLimitState state);
	void setAlimitStatePulseRateminDefault(eAlarmLimitState state);

	void setAlimitState_SPO2_PIminLimit(eAlarmLimitState state);
	void setAlimitStateSPO2_PIminDefault(eAlarmLimitState state);
	void setAlimitState_SPO2_SIQminLimit(eAlarmLimitState state);
	void setAlimitStateSPO2_SIQminDefault(eAlarmLimitState state);

	void setAlimitApnoe(int value);
	void setAlimitApnoeHF(int value);
	void setAlimitApnoeDefault(int value);

	void setAlimitBPMmax(int value);
	void setAlimitBPMmaxHF(int value);
	void setAlimitBPMmaxDefault(int value);

	void setAlimitLeakmax(int value);
	void setAlimitLeakmaxHF(int value);
	void setAlimitLeakmaxDefault(int value);

	void setAlimitMVmax(int value);
	void setAlimitMVmaxHF(int value);
	void setAlimitMVmaxDefault(int value);

	void setAlimitMVmin(int value);
	void setAlimitMVminHF(int value);
	void setAlimitMVminDefault(int value);

	void setAlimitPEEPmin(int value);
	void setAlimitPEEPminCpap(int value);
	void setAlimitPEEPminNCPAP(int value);
	void setAlimitPEEPminDUOPAP(int value);
	void setAlimitPEEPminDefault(int value);

	void setAlimitPIPmax(int value);
	void setAlimitPIPmaxHF(int value);
	void setAlimitPIPmaxCpap(int value);
	void setAlimitPIPmaxNCPAP(int value);
	void setAlimitPIPmaxDUOPAP(int value);
	void setAlimitPIPmaxDefault(int value);

	void setAlimitPIPmin(int value);
	void setAlimitPIPminDUOPAP(int value);
	void setAlimitPIPminDefault(int value);

	void setAlimitMAPmax(int value);
	void setAlimitMAPmaxHF(int value);
	void setAlimitMAPmin(int value);
	void setAlimitMAPminHF(int value);

	void setAlimitDCO2max(int value);
	void setAlimitDCO2maxHF(int value);
	void setAlimitDCO2min(int value);
	void setAlimitDCO2minHF(int value);

	void setAlimitETCO2max(int value);
	void setAlimitETCO2maxHF(int value);
	void setAlimitETCO2maxDefault(int value);
	void setAlimitETCO2min(int value);
	void setAlimitETCO2minHF(int value);
	void setAlimitETCO2minDefault(int value);

	void setAlimitFICO2max(int value);
	void setAlimitFICO2maxHF(int value);
	void setAlimitFICO2maxDefault(int value);
	void setAlimitFICO2min(int value);
	void setAlimitFICO2minHF(int value);
	void setAlimitFICO2minDefault(int value);

	void setAlimitSPO2max(int value);
	void setAlimitSPO2maxDefault(int value);
	void setAlimitSPO2min(int value);
	void setAlimitSPO2minDefault(int value);

	void setAlimitPulseRatemax(int value);
	void setAlimitPulseRatemaxDefault(int value);
	void setAlimitPulseRatemin(int value);
	void setAlimitPulseRateminDefault(int value);

	void setAlimitSPO2_PImin(int value);
	void setAlimitSPO2_PIminDefault(int value);
	void setAlimitSPO2_SIQmin(int value);
	void setAlimitSPO2_SIQminDefault(int value);

	void setAlimitsMinMaxRangeCpap();
	void setAlimitsMinMaxRangeNCPAP();
	void setAlimitsMinMaxRangeDUOPAP();
	void setAlimitsMinMaxRangePressure();

	int getAlimitMVmin();
	int getAlimitMVmax();
	int getAlimitPIPmax();
	int getAlimitPIPmin();
	int getAlimitPEEPmin();
	int getAlimitLeakmax();
	int getAlimitBPMmax();
	int getAlimitApnoe();
	int getAlimitMAPmax();
	int getAlimitMAPmin();
	int getAlimitDCO2min();
	int getAlimitDCO2max();
	int getAlimitETCO2min();
	int getAlimitETCO2max();
	int getAlimitFICO2min();
	int getAlimitFICO2max();
	int getAlimitSPO2max();
	int getAlimitSPO2min();
	int getAlimitPulseRatemax();
	int getAlimitPulseRatemin();
	int getAlimitSPO2_PImin();
	int getAlimitSPO2_SIQmin();

	void setAlimitMaxRangePIPmax(int iRangeVal);
	void setAlimitMinRangePIPmax(int iRangeVal);
	void setAlimitMaxRangePIPmin(int iRangeVal);
	void setAlimitMinRangePIPmin(int iRangeVal);
	void setAlimitMaxRangePEEPmin(int iRangeVal);
	void setAlimitMinRangePEEPmin(int iRangeVal);
	void setAlimitMaxRangeMVmin(int iRangeVal);
	void setAlimitMinRangeMVmin(int iRangeVal);
	void setAlimitMaxRangeMVmax(int iRangeVal);
	void setAlimitMinRangeMVmax(int iRangeVal);
	void setAlimitMaxRangeLeakmax(int iRangeVal);
	void setAlimitMinRangeLeakmax(int iRangeVal);
	void setAlimitMaxRangeBPMmax(int iRangeVal);
	void setAlimitMinRangeBPMmax(int iRangeVal);
	void setAlimitMaxRangeApnoe(int iRangeVal);
	void setAlimitMinRangeApnoe(int iRangeVal);
	void setAlimitMaxRangeMAPmax(int iRangeVal);
	void setAlimitMinRangeMAPmax(int iRangeVal);
	void setAlimitMaxRangeMAPmin(int iRangeVal);
	void setAlimitMinRangeMAPmin(int iRangeVal);
	void setAlimitMaxRangeDCO2min(int iRangeVal);
	void setAlimitMinRangeDCO2min(int iRangeVal);
	void setAlimitMaxRangeDCO2max(int iRangeVal);
	void setAlimitMinRangeDCO2max(int iRangeVal);
	void setAlimitMaxRangeETCO2min(int iRangeVal);
	void setAlimitMinRangeETCO2min(int iRangeVal);
	void setAlimitMaxRangeETCO2max(int iRangeVal);
	void setAlimitMinRangeETCO2max(int iRangeVal);
	void setAlimitMaxRangeFICO2min(int iRangeVal);
	void setAlimitMinRangeFICO2min(int iRangeVal);
	void setAlimitMaxRangeFICO2max(int iRangeVal);
	void setAlimitMinRangeFICO2max(int iRangeVal);
	void setAlimitMaxRangeSPO2max(int iRangeVal);
	void setAlimitMinRangeSPO2max(int iRangeVal);
	void setAlimitMaxRangeSPO2min(int iRangeVal);
	void setAlimitMinRangeSPO2min(int iRangeVal);
	void setAlimitMaxRangePulseRatemin(int iRangeVal);
	void setAlimitMinRangePulseRatemax(int iRangeVal);
	void setAlimitMaxRangePulseRatemax(int iRangeVal);
	void setAlimitMinRangePulseRatemin(int iRangeVal);
	void setAlimitMaxRangeSPO2_PImin(int iRangeVal);
	void setAlimitMinRangeSPO2_PImin(int iRangeVal);
	void setAlimitMaxRangeSPO2_SIQmin(int iRangeVal);
	void setAlimitMinRangeSPO2_SIQmin(int iRangeVal);

	int getAlimitMaxRangePIPmax();
	int getAlimitMinRangePIPmax();
	int getAlimitMaxRangePIPmin();
	int getAlimitMinRangePIPmin();
	int getAlimitMaxRangePEEPmin();
	int getAlimitMinRangePEEPmin();
	int getAlimitMaxRangeMVmin();
	int getAlimitMinRangeMVmin();
	int getAlimitMaxRangeMVmax();
	int getAlimitMinRangeMVmax();
	int getAlimitMaxRangeLeakmax();
	int getAlimitMinRangeLeakmax();
	int getAlimitMaxRangeBPMmax();
	int getAlimitMinRangeBPMmax();
	int getAlimitMaxRangeApnoe();
	int getAlimitMinRangeApnoe();
	int getAlimitMaxRangeMAPmax();
	int getAlimitMinRangeMAPmax();
	int getAlimitMaxRangeMAPmin();
	int getAlimitMinRangeMAPmin();
	int getAlimitMaxRangeDCO2min();
	int getAlimitMinRangeDCO2min();
	int getAlimitMaxRangeDCO2max();
	int getAlimitMinRangeDCO2max();
	int getAlimitMaxRangeETCO2min();
	int getAlimitMinRangeETCO2min();
	int getAlimitMaxRangeETCO2max();
	int getAlimitMinRangeETCO2max();
	int getAlimitMaxRangeFICO2min();
	int getAlimitMinRangeFICO2min();
	int getAlimitMaxRangeFICO2max();
	int getAlimitMinRangeFICO2max();
	int getAlimitMaxRangeSPO2max();
	int getAlimitMinRangeSPO2max();
	int getAlimitMaxRangeSPO2min();
	int getAlimitMinRangeSPO2min();
	int getAlimitMaxRangePulseRatemin();
	int getAlimitMinRangePulseRatemax();
	int getAlimitMaxRangePulseRatemax();
	int getAlimitMinRangePulseRatemin();
	int getAlimitMaxRangeSPO2_PImin();
	int getAlimitMinRangeSPO2_PImin();
	int getAlimitMaxRangeSPO2_SIQmin();
	int getAlimitMinRangeSPO2_SIQmin();

protected:

	void initAlarmLimitList();

	void setAlarmLimitValues_SPO2();
	void setAlarmLimitValues_DEFAULT();
	void setAlarmLimitValues_HFO();
	void setAlarmLimitValues_CPAP();
	void setAlarmLimitValues_NCPAP();
	void setAlarmLimitValues_DUOPAP();
	//void setAlarmLimitValues_THERAPIE();
	void setAlarmLimitRanges_DEFAULT();
	void setAlarmLimitRanges_HFO();
	void setAlarmLimitRanges_CPAP();
	void setAlarmLimitRanges_DUOPAP();//rku PIPLOW DUOPAP
	void setAlarmLimitRanges_NCPAP();//rku PIPLOW DUOPAP
	void setAlarmLimitRanges_ETCO2();
	void setAlarmLimitRanges_SPO2();

	CMVModel *getModel();

private: 
	CAlarmLimitList* m_pAlarmLimitlist; ///< The alarm limitlist
	CRITICAL_SECTION csAlarmLimitList;  ///< List of create structure alarm limits

	CMVModel *m_pModel; ///< The model

	static CAlarmLimitPtr ALIMIT_MVMAX; ///< The alimit mvmax
	static CAlarmLimitPtr ALIMIT_MVMIN; ///< The alimit mvmin
	static CAlarmLimitPtr ALIMIT_PIPMAX;	///< The alimit pipmax
	static CAlarmLimitPtr ALIMIT_PIPMIN;	///< The alimit pipmin
	static CAlarmLimitPtr ALIMIT_PEEPMIN;   ///< The alimit peepmin
	static CAlarmLimitPtr ALIMIT_FREQMAX;   ///< The alimit freqmax
	static CAlarmLimitPtr ALIMIT_LEAKMAX;   ///< The alimit leakmax
	static CAlarmLimitPtr ALIMIT_APNOE; ///< The alimit apnoe
	static CAlarmLimitPtr ALIMIT_DCO2MAX;   ///< The alimit dco 2 maximum
	static CAlarmLimitPtr ALIMIT_DCO2MIN;   ///< The alimit dco 2 minimum
	static CAlarmLimitPtr ALIMIT_ETCO2MAX;  ///< The alimit etco 2 maximum
	static CAlarmLimitPtr ALIMIT_ETCO2MIN;  ///< The alimit etco 2 minimum
	static CAlarmLimitPtr ALIMIT_FICO2MAX;  ///< The alimit fico 2 maximum
	static CAlarmLimitPtr ALIMIT_FICO2MIN;  ///< The alimit fico 2 minimum
	static CAlarmLimitPtr ALIMIT_SPO2MAX;   ///< The alimit spo 2 maximum
	static CAlarmLimitPtr ALIMIT_SPO2MIN;   ///< The alimit spo 2 minimum
	static CAlarmLimitPtr ALIMIT_PULSERATEMAX;  ///< The alimit pulseratemax
	static CAlarmLimitPtr ALIMIT_PULSERATEMIN;  ///< The alimit pulseratemin
	static CAlarmLimitPtr ALIMIT_SPO2PIMIN; ///< The alimit spo 2 pimin
	static CAlarmLimitPtr ALIMIT_SPO2SIQMIN;	///< The alimit spo 2 siqmin
	static CAlarmLimitPtr ALIMIT_MAPMAX;   ///< The alimit MAP maximum (mean airway pressure)
	static CAlarmLimitPtr ALIMIT_MAPMIN;   ///< The alimit MAP minimum (mean airway pressure)
};
