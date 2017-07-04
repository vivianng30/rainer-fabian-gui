/**********************************************************************************************//**
 * \file	AlarmConditionHandler.h.
 *
 * Declares the alarm condition handler class.
 **************************************************************************************************/

#pragma once
#include "AlarmMgmtHandler.h"
#include "AlarmLimitList.h"

/**********************************************************************************************//**
 * An alarm condition handler.
 *
 * \author	Rainer
 * \date	04.07.2017
 **************************************************************************************************/

class CAlarmConditionHandler 
{
public:

	/**********************************************************************************************//**
	 * Default constructor.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	CAlarmConditionHandler();

	/**********************************************************************************************//**
	 * Destructor.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	virtual ~CAlarmConditionHandler(void);

	/**********************************************************************************************//**
	 * Vent mode changed.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void ventModeChanged();

	/**********************************************************************************************//**
	 * Sets alimit state calculated.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void SetAlimitStateCalculated();

	/**********************************************************************************************//**
	 * Gets alimit state m vmax limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state m vmax limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_MVmaxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state m vmin limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state m vmin limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_MVminLimit();

	/**********************************************************************************************//**
	 * Gets alimit state pi pmax limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state pi pmax limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_PIPmaxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state pi pmin limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state pi pmin limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_PIPminLimit();

	/**********************************************************************************************//**
	 * Gets alimit state pee pmin limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state pee pmin limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_PEEPminLimit();

	/**********************************************************************************************//**
	 * Gets alimit state bp mmax limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state bp mmax limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_BPMmaxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state leakmax limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state leakmax limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_LeakmaxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state apnoe limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state apnoe limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_ApnoeLimit();

	/**********************************************************************************************//**
	 * Gets alimit state dco 2max limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state dco 2max limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_DCO2maxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state dco 2min limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state dco 2min limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_DCO2minLimit();

	/**********************************************************************************************//**
	 * Gets alimit state etco 2max limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state etco 2max limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_ETCO2maxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state etco 2min limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state etco 2min limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_ETCO2minLimit();

	/**********************************************************************************************//**
	 * Gets alimit state fico 2max limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state fico 2max limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_FICO2maxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state fico 2min limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state fico 2min limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_FICO2minLimit();

	/**********************************************************************************************//**
	 * Gets alimit state spo 2max limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state spo 2max limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_SPO2maxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state spo 2min limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state spo 2min limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_SPO2minLimit();

	/**********************************************************************************************//**
	 * Gets alimit state pulse ratemax limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state pulse ratemax limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_PulseRatemaxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state pulse ratemin limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state pulse ratemin limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_PulseRateminLimit();

	/**********************************************************************************************//**
	 * Gets alimit state spo 2 p imin limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state spo 2 p imin limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_SPO2_PIminLimit();

	/**********************************************************************************************//**
	 * Gets alimit state spo 2 SI qmin limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit state spo 2 SI qmin limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_SPO2_SIQminLimit();

	/**********************************************************************************************//**
	 * Sets alimit state apnoe limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_ApnoeLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state apnoe hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateApnoeHF(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state apnoe default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateApnoeDefault(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state bp mmax limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_BPMmaxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state bp mmax hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateBPMmaxHF(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state bp mmax default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateBPMmaxDefault(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state leakmax limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_LeakmaxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state leakmax hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateLeakmaxHF(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state leakmax default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateLeakmaxDefault(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state m vmax limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_MVmaxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state m vmax hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateMVmaxHF(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state m vmax default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateMVmaxDefault(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state m vmin limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_MVminLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state m vmin hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateMVminHF(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state m vmin default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateMVminDefault(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pee pmin limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_PEEPminLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pee pmin ncpap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStatePEEPminNCPAP(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pee pmin cpap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStatePEEPminCpap(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pee pmin duopap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStatePEEPminDUOPAP(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pee pmin default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStatePEEPminDefault(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pi pmin limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_PIPminLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pi pmin default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStatePIPminDefault(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pi pmax limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_PIPmaxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pi pmax hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStatePIPmaxHF(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pi pmax cpap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStatePIPmaxCpap(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pi pmax ncpap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStatePIPmaxNCPAP(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pi pmax duopap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStatePIPmaxDUOPAP(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pi pmax default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStatePIPmaxDefault(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state dco 2max limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_DCO2maxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state dco 2max hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateDCO2maxHF(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state dco 2min limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_DCO2minLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state dco 2min hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateDCO2minHF(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state etco 2max limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_ETCO2maxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state etco 2max hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateETCO2maxHF(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state etco 2max default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateETCO2maxDefault(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state etco 2min limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_ETCO2minLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state etco 2min hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateETCO2minHF(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state etco 2min default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateETCO2minDefault(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state fico 2max limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_FICO2maxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state fico 2max hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateFICO2maxHF(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state fico 2max default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateFICO2maxDefault(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state fico 2min limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_FICO2minLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state fico 2min hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateFICO2minHF(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state fico 2min default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateFICO2minDefault(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state spo 2max limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_SPO2maxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state spo 2max default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateSPO2maxDefault(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state spo 2min limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_SPO2minLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state spo 2min default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateSPO2minDefault(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pulse ratemax limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_PulseRatemaxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pulse ratemax default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStatePulseRatemaxDefault(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pulse ratemin limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_PulseRateminLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pulse ratemin default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStatePulseRateminDefault(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state spo 2 p imin limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_SPO2_PIminLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state spo 2 p imin default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateSPO2_PIminDefault(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state spo 2 SI qmin limit.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_SPO2_SIQminLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state spo 2 SI qmin default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitStateSPO2_SIQminDefault(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit apnoe.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitApnoe(int value);

	/**********************************************************************************************//**
	 * Sets alimit apnoe hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitApnoeHF(int value);

	/**********************************************************************************************//**
	 * Sets alimit apnoe default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitApnoeDefault(int value);

	/**********************************************************************************************//**
	 * Sets alimit bp mmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitBPMmax(int value);

	/**********************************************************************************************//**
	 * Sets alimit bp mmax hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitBPMmaxHF(int value);

	/**********************************************************************************************//**
	 * Sets alimit bp mmax default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitBPMmaxDefault(int value);

	/**********************************************************************************************//**
	 * Sets alimit leakmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitLeakmax(int value);

	/**********************************************************************************************//**
	 * Sets alimit leakmax hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitLeakmaxHF(int value);

	/**********************************************************************************************//**
	 * Sets alimit leakmax default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitLeakmaxDefault(int value);

	/**********************************************************************************************//**
	 * Sets alimit m vmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitMVmax(int value);

	/**********************************************************************************************//**
	 * Sets alimit m vmax hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitMVmaxHF(int value);

	/**********************************************************************************************//**
	 * Sets alimit m vmax default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitMVmaxDefault(int value);

	/**********************************************************************************************//**
	 * Sets alimit m vmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitMVmin(int value);

	/**********************************************************************************************//**
	 * Sets alimit m vmin hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitMVminHF(int value);

	/**********************************************************************************************//**
	 * Sets alimit m vmin default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitMVminDefault(int value);

	/**********************************************************************************************//**
	 * Sets alimit pee pmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPEEPmin(int value);

	/**********************************************************************************************//**
	 * Sets alimit pee pmin cpap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPEEPminCpap(int value);

	/**********************************************************************************************//**
	 * Sets alimit pee pmin ncpap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPEEPminNCPAP(int value);

	/**********************************************************************************************//**
	 * Sets alimit pee pmin duopap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPEEPminDUOPAP(int value);

	/**********************************************************************************************//**
	 * Sets alimit pee pmin default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPEEPminDefault(int value);

	/**********************************************************************************************//**
	 * Sets alimit pi pmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPIPmax(int value);

	/**********************************************************************************************//**
	 * Sets alimit pi pmax hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPIPmaxHF(int value);

	/**********************************************************************************************//**
	 * Sets alimit pi pmax cpap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPIPmaxCpap(int value);

	/**********************************************************************************************//**
	 * Sets alimit pi pmax ncpap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPIPmaxNCPAP(int value);

	/**********************************************************************************************//**
	 * Sets alimit pi pmax duopap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPIPmaxDUOPAP(int value);

	/**********************************************************************************************//**
	 * Sets alimit pi pmax default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPIPmaxDefault(int value);

	/**********************************************************************************************//**
	 * Sets alimit pi pmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPIPmin(int value);

	/**********************************************************************************************//**
	 * Sets alimit pi pmin default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPIPminDefault(int value);

	/**********************************************************************************************//**
	 * Sets alimit dco 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitDCO2max(int value);

	/**********************************************************************************************//**
	 * Sets alimit dco 2max hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitDCO2maxHF(int value);

	/**********************************************************************************************//**
	 * Sets alimit dco 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitDCO2min(int value);

	/**********************************************************************************************//**
	 * Sets alimit dco 2min hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitDCO2minHF(int value);

	/**********************************************************************************************//**
	 * Sets alimit etco 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitETCO2max(int value);

	/**********************************************************************************************//**
	 * Sets alimit etco 2max hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitETCO2maxHF(int value);

	/**********************************************************************************************//**
	 * Sets alimit etco 2max default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitETCO2maxDefault(int value);

	/**********************************************************************************************//**
	 * Sets alimit etco 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitETCO2min(int value);

	/**********************************************************************************************//**
	 * Sets alimit etco 2min hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitETCO2minHF(int value);

	/**********************************************************************************************//**
	 * Sets alimit etco 2min default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitETCO2minDefault(int value);

	/**********************************************************************************************//**
	 * Sets alimit fico 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitFICO2max(int value);

	/**********************************************************************************************//**
	 * Sets alimit fico 2max hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitFICO2maxHF(int value);

	/**********************************************************************************************//**
	 * Sets alimit fico 2max default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitFICO2maxDefault(int value);

	/**********************************************************************************************//**
	 * Sets alimit fico 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitFICO2min(int value);

	/**********************************************************************************************//**
	 * Sets alimit fico 2min hf.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitFICO2minHF(int value);

	/**********************************************************************************************//**
	 * Sets alimit fico 2min default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitFICO2minDefault(int value);

	/**********************************************************************************************//**
	 * Sets alimit spo 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitSPO2max(int value);

	/**********************************************************************************************//**
	 * Sets alimit spo 2max default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitSPO2maxDefault(int value);

	/**********************************************************************************************//**
	 * Sets alimit spo 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitSPO2min(int value);

	/**********************************************************************************************//**
	 * Sets alimit spo 2min default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitSPO2minDefault(int value);

	/**********************************************************************************************//**
	 * Sets alimit pulse ratemax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPulseRatemax(int value);

	/**********************************************************************************************//**
	 * Sets alimit pulse ratemax default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPulseRatemaxDefault(int value);

	/**********************************************************************************************//**
	 * Sets alimit pulse ratemin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPulseRatemin(int value);

	/**********************************************************************************************//**
	 * Sets alimit pulse ratemin default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPulseRateminDefault(int value);

	/**********************************************************************************************//**
	 * Sets alimit spo 2 p imin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitSPO2_PImin(int value);

	/**********************************************************************************************//**
	 * Sets alimit spo 2 p imin default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitSPO2_PIminDefault(int value);

	/**********************************************************************************************//**
	 * Sets alimit spo 2 SI qmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitSPO2_SIQmin(int value);

	/**********************************************************************************************//**
	 * Sets alimit spo 2 SI qmin default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitSPO2_SIQminDefault(int value);

	/**********************************************************************************************//**
	 * Sets alimits minimum maximum range cpap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void setAlimitsMinMaxRangeCpap();

	/**********************************************************************************************//**
	 * Sets alimits minimum maximum range ncpap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void setAlimitsMinMaxRangeNCPAP();

	/**********************************************************************************************//**
	 * Sets alimits minimum maximum range duopap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void setAlimitsMinMaxRangeDUOPAP();

	/**********************************************************************************************//**
	 * Sets alimits minimum maximum range pressure.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void setAlimitsMinMaxRangePressure();

	/**********************************************************************************************//**
	 * Gets alimit m vmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit m vmin.
	 **************************************************************************************************/

	int getAlimitMVmin();

	/**********************************************************************************************//**
	 * Gets alimit m vmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit m vmax.
	 **************************************************************************************************/

	int getAlimitMVmax();

	/**********************************************************************************************//**
	 * Gets alimit pi pmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit pi pmax.
	 **************************************************************************************************/

	int getAlimitPIPmax();

	/**********************************************************************************************//**
	 * Gets alimit pi pmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit pi pmin.
	 **************************************************************************************************/

	int getAlimitPIPmin();

	/**********************************************************************************************//**
	 * Gets alimit pee pmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit pee pmin.
	 **************************************************************************************************/

	int getAlimitPEEPmin();

	/**********************************************************************************************//**
	 * Gets alimit leakmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit leakmax.
	 **************************************************************************************************/

	int getAlimitLeakmax();

	/**********************************************************************************************//**
	 * Gets alimit bp mmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit bp mmax.
	 **************************************************************************************************/

	int getAlimitBPMmax();

	/**********************************************************************************************//**
	 * Gets alimit apnoe.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit apnoe.
	 **************************************************************************************************/

	int getAlimitApnoe();

	/**********************************************************************************************//**
	 * Gets alimit dco 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit dco 2min.
	 **************************************************************************************************/

	int getAlimitDCO2min();

	/**********************************************************************************************//**
	 * Gets alimit dco 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit dco 2max.
	 **************************************************************************************************/

	int getAlimitDCO2max();

	/**********************************************************************************************//**
	 * Gets alimit etco 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit etco 2min.
	 **************************************************************************************************/

	int getAlimitETCO2min();

	/**********************************************************************************************//**
	 * Gets alimit etco 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit etco 2max.
	 **************************************************************************************************/

	int getAlimitETCO2max();

	/**********************************************************************************************//**
	 * Gets alimit fico 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit fico 2min.
	 **************************************************************************************************/

	int getAlimitFICO2min();

	/**********************************************************************************************//**
	 * Gets alimit fico 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit fico 2max.
	 **************************************************************************************************/

	int getAlimitFICO2max();

	/**********************************************************************************************//**
	 * Gets alimit spo 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit spo 2max.
	 **************************************************************************************************/

	int getAlimitSPO2max();

	/**********************************************************************************************//**
	 * Gets alimit spo 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit spo 2min.
	 **************************************************************************************************/

	int getAlimitSPO2min();

	/**********************************************************************************************//**
	 * Gets alimit pulse ratemax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit pulse ratemax.
	 **************************************************************************************************/

	int getAlimitPulseRatemax();

	/**********************************************************************************************//**
	 * Gets alimit pulse ratemin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit pulse ratemin.
	 **************************************************************************************************/

	int getAlimitPulseRatemin();

	/**********************************************************************************************//**
	 * Gets alimit spo 2 p imin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit spo 2 p imin.
	 **************************************************************************************************/

	int getAlimitSPO2_PImin();

	/**********************************************************************************************//**
	 * Gets alimit spo 2 SI qmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit spo 2 SI qmin.
	 **************************************************************************************************/

	int getAlimitSPO2_SIQmin();

	/**********************************************************************************************//**
	 * Sets alimit maximum range pi pmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangePIPmax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range pi pmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangePIPmax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range pi pmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangePIPmin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range pi pmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangePIPmin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range pee pmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangePEEPmin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range pee pmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangePEEPmin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range m vmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeMVmin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range m vmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeMVmin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range m vmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeMVmax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range m vmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeMVmax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range leakmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeLeakmax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range leakmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeLeakmax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range bp mmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeBPMmax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range bp mmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeBPMmax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range apnoe.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeApnoe(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range apnoe.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeApnoe(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range dco 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeDCO2min(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range dco 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeDCO2min(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range dco 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeDCO2max(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range dco 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeDCO2max(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range etco 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeETCO2min(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range etco 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeETCO2min(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range etco 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeETCO2max(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range etco 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeETCO2max(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range fico 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeFICO2min(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range fico 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeFICO2min(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range fico 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeFICO2max(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range fico 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeFICO2max(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range spo 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeSPO2max(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range spo 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeSPO2max(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range spo 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeSPO2min(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range spo 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeSPO2min(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range pulse ratemin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangePulseRatemin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range pulse ratemax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangePulseRatemax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range pulse ratemax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangePulseRatemax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range pulse ratemin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangePulseRatemin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range spo 2 p imin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeSPO2_PImin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range spo 2 p imin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeSPO2_PImin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range spo 2 SI qmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeSPO2_SIQmin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range spo 2 SI qmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeSPO2_SIQmin(int iRangeVal);

	/**********************************************************************************************//**
	 * Gets alimit maximum range pi pmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range pi pmax.
	 **************************************************************************************************/

	int getAlimitMaxRangePIPmax();

	/**********************************************************************************************//**
	 * Gets alimit minimum range pi pmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range pi pmax.
	 **************************************************************************************************/

	int getAlimitMinRangePIPmax();

	/**********************************************************************************************//**
	 * Gets alimit maximum range pi pmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range pi pmin.
	 **************************************************************************************************/

	int getAlimitMaxRangePIPmin();

	/**********************************************************************************************//**
	 * Gets alimit minimum range pi pmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range pi pmin.
	 **************************************************************************************************/

	int getAlimitMinRangePIPmin();

	/**********************************************************************************************//**
	 * Gets alimit maximum range pee pmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range pee pmin.
	 **************************************************************************************************/

	int getAlimitMaxRangePEEPmin();

	/**********************************************************************************************//**
	 * Gets alimit minimum range pee pmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range pee pmin.
	 **************************************************************************************************/

	int getAlimitMinRangePEEPmin();

	/**********************************************************************************************//**
	 * Gets alimit maximum range m vmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range m vmin.
	 **************************************************************************************************/

	int getAlimitMaxRangeMVmin();

	/**********************************************************************************************//**
	 * Gets alimit minimum range m vmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range m vmin.
	 **************************************************************************************************/

	int getAlimitMinRangeMVmin();

	/**********************************************************************************************//**
	 * Gets alimit maximum range m vmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range m vmax.
	 **************************************************************************************************/

	int getAlimitMaxRangeMVmax();

	/**********************************************************************************************//**
	 * Gets alimit minimum range m vmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range m vmax.
	 **************************************************************************************************/

	int getAlimitMinRangeMVmax();

	/**********************************************************************************************//**
	 * Gets alimit maximum range leakmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range leakmax.
	 **************************************************************************************************/

	int getAlimitMaxRangeLeakmax();

	/**********************************************************************************************//**
	 * Gets alimit minimum range leakmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range leakmax.
	 **************************************************************************************************/

	int getAlimitMinRangeLeakmax();

	/**********************************************************************************************//**
	 * Gets alimit maximum range bp mmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range bp mmax.
	 **************************************************************************************************/

	int getAlimitMaxRangeBPMmax();

	/**********************************************************************************************//**
	 * Gets alimit minimum range bp mmax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range bp mmax.
	 **************************************************************************************************/

	int getAlimitMinRangeBPMmax();

	/**********************************************************************************************//**
	 * Gets alimit maximum range apnoe.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range apnoe.
	 **************************************************************************************************/

	int getAlimitMaxRangeApnoe();

	/**********************************************************************************************//**
	 * Gets alimit minimum range apnoe.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range apnoe.
	 **************************************************************************************************/

	int getAlimitMinRangeApnoe();

	/**********************************************************************************************//**
	 * Gets alimit maximum range dco 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range dco 2min.
	 **************************************************************************************************/

	int getAlimitMaxRangeDCO2min();

	/**********************************************************************************************//**
	 * Gets alimit minimum range dco 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range dco 2min.
	 **************************************************************************************************/

	int getAlimitMinRangeDCO2min();

	/**********************************************************************************************//**
	 * Gets alimit maximum range dco 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range dco 2max.
	 **************************************************************************************************/

	int getAlimitMaxRangeDCO2max();

	/**********************************************************************************************//**
	 * Gets alimit minimum range dco 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range dco 2max.
	 **************************************************************************************************/

	int getAlimitMinRangeDCO2max();

	/**********************************************************************************************//**
	 * Gets alimit maximum range etco 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range etco 2min.
	 **************************************************************************************************/

	int getAlimitMaxRangeETCO2min();

	/**********************************************************************************************//**
	 * Gets alimit minimum range etco 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range etco 2min.
	 **************************************************************************************************/

	int getAlimitMinRangeETCO2min();

	/**********************************************************************************************//**
	 * Gets alimit maximum range etco 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range etco 2max.
	 **************************************************************************************************/

	int getAlimitMaxRangeETCO2max();

	/**********************************************************************************************//**
	 * Gets alimit minimum range etco 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range etco 2max.
	 **************************************************************************************************/

	int getAlimitMinRangeETCO2max();

	/**********************************************************************************************//**
	 * Gets alimit maximum range fico 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range fico 2min.
	 **************************************************************************************************/

	int getAlimitMaxRangeFICO2min();

	/**********************************************************************************************//**
	 * Gets alimit minimum range fico 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range fico 2min.
	 **************************************************************************************************/

	int getAlimitMinRangeFICO2min();

	/**********************************************************************************************//**
	 * Gets alimit maximum range fico 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range fico 2max.
	 **************************************************************************************************/

	int getAlimitMaxRangeFICO2max();

	/**********************************************************************************************//**
	 * Gets alimit minimum range fico 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range fico 2max.
	 **************************************************************************************************/

	int getAlimitMinRangeFICO2max();

	/**********************************************************************************************//**
	 * Gets alimit maximum range spo 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range spo 2max.
	 **************************************************************************************************/

	int getAlimitMaxRangeSPO2max();

	/**********************************************************************************************//**
	 * Gets alimit minimum range spo 2max.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range spo 2max.
	 **************************************************************************************************/

	int getAlimitMinRangeSPO2max();

	/**********************************************************************************************//**
	 * Gets alimit maximum range spo 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range spo 2min.
	 **************************************************************************************************/

	int getAlimitMaxRangeSPO2min();

	/**********************************************************************************************//**
	 * Gets alimit minimum range spo 2min.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range spo 2min.
	 **************************************************************************************************/

	int getAlimitMinRangeSPO2min();

	/**********************************************************************************************//**
	 * Gets alimit maximum range pulse ratemin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range pulse ratemin.
	 **************************************************************************************************/

	int getAlimitMaxRangePulseRatemin();

	/**********************************************************************************************//**
	 * Gets alimit minimum range pulse ratemax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range pulse ratemax.
	 **************************************************************************************************/

	int getAlimitMinRangePulseRatemax();

	/**********************************************************************************************//**
	 * Gets alimit maximum range pulse ratemax.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range pulse ratemax.
	 **************************************************************************************************/

	int getAlimitMaxRangePulseRatemax();

	/**********************************************************************************************//**
	 * Gets alimit minimum range pulse ratemin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range pulse ratemin.
	 **************************************************************************************************/

	int getAlimitMinRangePulseRatemin();

	/**********************************************************************************************//**
	 * Gets alimit maximum range spo 2 p imin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range spo 2 p imin.
	 **************************************************************************************************/

	int getAlimitMaxRangeSPO2_PImin();

	/**********************************************************************************************//**
	 * Gets alimit minimum range spo 2 p imin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range spo 2 p imin.
	 **************************************************************************************************/

	int getAlimitMinRangeSPO2_PImin();

	/**********************************************************************************************//**
	 * Gets alimit maximum range spo 2 SI qmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit maximum range spo 2 SI qmin.
	 **************************************************************************************************/

	int getAlimitMaxRangeSPO2_SIQmin();

	/**********************************************************************************************//**
	 * Gets alimit minimum range spo 2 SI qmin.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	The alimit minimum range spo 2 SI qmin.
	 **************************************************************************************************/

	int getAlimitMinRangeSPO2_SIQmin();


protected:

	/**********************************************************************************************//**
	 * Initializes the alarm limit list.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void initAlarmLimitList();

	/**********************************************************************************************//**
	 * Sets alarm limit values spo 2.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void setAlarmLimitValues_SPO2();

	/**********************************************************************************************//**
	 * Sets alarm limit values default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void setAlarmLimitValues_DEFAULT();

	/**********************************************************************************************//**
	 * Sets alarm limit values hfo.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void setAlarmLimitValues_HFO();

	/**********************************************************************************************//**
	 * Sets alarm limit values cpap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void setAlarmLimitValues_CPAP();

	/**********************************************************************************************//**
	 * Sets alarm limit values ncpap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void setAlarmLimitValues_NCPAP();

	/**********************************************************************************************//**
	 * Sets alarm limit values duopap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void setAlarmLimitValues_DUOPAP();
	//void setAlarmLimitValues_THERAPIE();

	/**********************************************************************************************//**
	 * Sets alarm limit ranges default.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void setAlarmLimitRanges_DEFAULT();

	/**********************************************************************************************//**
	 * Sets alarm limit ranges hfo.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void setAlarmLimitRanges_HFO();

	/**********************************************************************************************//**
	 * Sets alarm limit ranges cpap.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void setAlarmLimitRanges_CPAP();

	/**********************************************************************************************//**
	 * Sets alarm limit ranges nmode.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void setAlarmLimitRanges_NMODE();

	/**********************************************************************************************//**
	 * Sets alarm limit ranges etco 2.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void setAlarmLimitRanges_ETCO2();

	/**********************************************************************************************//**
	 * Sets alarm limit ranges spo 2.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 **************************************************************************************************/

	void setAlarmLimitRanges_SPO2();

	/**********************************************************************************************//**
	 * Gets the model.
	 *
	 * \author	Rainer
	 * \date	04.07.2017
	 *
	 * \return	Null if it fails, else the model.
	 **************************************************************************************************/

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
	
};
