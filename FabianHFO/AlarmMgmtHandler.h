//=============================================================================
//** 
//* \file AlarmMgmtHandler.h
//* \class CAlarmMgmtHandler
//* \brief Alarm handler depending on priority of each alarm.
//* \author Rainer Kuehner
//* \date 2015-06-29 Creation of file
//*
//* The alarm handler (file AlarmMgmtHandler.h) is responsible for alarm priorities
//* and controlling alarm states. System alarms are provided by PICs to the alarm
//* handler, patient alarms are checked against limits by the alarm handler.
//*
//*       Copyright (c) 2015 Ing.Buero Rainer Kuehner
//*                      Illerkirchberg
//*
//**/
//=============================================================================
#pragma once
#include "globDefs.h"
#include "Logfile.h"
#include "TlsLoglist.h"
#include "TlsTime.h"
#include "Alarm.h"
#include "AlarmLimit.h"
#include "AlarmList.h"

/**********************************************************************************************//**
 * A macro that defines maxalarmtasks.
 *
 * \author	Rainer
 * \date	11.07.2017
 **************************************************************************************************/

#define MAXALARMTASKS 80

/**********************************************************************************************//**
 * A data Model for the cmv.
 *
 * \author	Rainer
 * \date	11.07.2017
 **************************************************************************************************/

class CMVModel;

/**********************************************************************************************//**
 * An alarm condition handler.
 *
 * \author	Rainer
 * \date	11.07.2017
 **************************************************************************************************/

class CAlarmConditionHandler;

/**********************************************************************************************//**
 * CAlarmingHandler.
 *
 * \author	Rainer
 * \date	11.07.2017
 **************************************************************************************************/

class CAlarmMgmtHandler
{

public:

	static CAlarmMgmtHandler* getInstance();

	static void destroyInstance();

	void init();

	void deinit();

	void initAlarmList();

	void checkLogAlarmDate();

	void loadConfig();

	void ventModeChanged();

	eVentMode getSafeActiveVentMode();

	bool setStateOfAlarm(eAlarm alarm, eStateOfAlarm eState, bool bUpdate=true);

	eAlarm getActiveAlarm();

	void setActiveAlarm(eAlarm alarm);

	eVentSilentState getAlarmSilentState();

	void setAlarmSilentState(eVentSilentState state);

	eAlarmPrio getPrioActiveAlarm();

	void setPrioActiveAlarm(eAlarmPrio prio);

	bool isAlarmType(eAlarmType type);

	bool isActiveAlarmType(eAlarmType type);

	bool isSignaledAlarmType(eAlarmType type);

	bool isAlarm();

	bool isActiveAlarm();

	bool isSignaledAlarm();

	bool deleteAlarm(eAlarm alarm, bool bUpdate=true);

	bool deleteAlarmType(eAlarmType type=AT_NONE, bool bUpdate=true);

	bool deleteLowerPrioAlarms(eAlarmPrio prio);

	bool deleteAkkuAlarms();

	void deleteSignaledAlarmlimitStates();

	void deleteSignaledAlarms();

	void deleteAllAlarms(bool bDeleteAkkuAlarms);

	bool setLowerPrioActiveAlarmsSignaled(eAlarmPrio prio);

	void setStartupSilent(bool bState);

	bool getStartupSilent();

	void setAlarmsilentActive();//aktiviert alle AlarmSilentStates ausser SystemSilent!!!!!

	void setSystemSilentActive();

	void setSystemSilent();

	void setAlarmSilent(bool bDelSysSilent=false,bool bDelAllAlarms=false);

	void setAutoSilent(bool bDelSysSilent=false, bool bDelSilent=false, bool bDelManSilent=false);

	void setManualSilent(bool bRefreshTime=true);

	void setCalibrationSilent();

	bool isManualSilentState();

	CStringW getAlarmText(eAlarm alarm);

	eAlarmPrio getAlarmPrio(eAlarm alarm);

	eAlarmType getAlarmType(eAlarm alarm);

	eAlarm getSignaledAlarm();

	bool setAlarm(eAlarm alarm, CStringW alarmTxt=_T(""));

	void setActiveAlarm(eAlarm actAlarm, CStringW alarmTxt);

	eAlarm getNextActiveAlarm();

	bool isPRICOrelevantAlarmActive();

	bool isFOTrelevantAlarmActive();

	void updateAlarmSound();

	void updateAlarmStatesSilentStateDependend();

	void updateActiveAlarm();

	void updateActiveAlarm(eAlarm alarm,eStateOfAlarm state);

	void setAlarmStatePDMS(eAlarm eAlarmToSet, eStateOfAlarm state);

	void resetVgVlAutoTurnedOff();

	void resetVgVlAutoTurnedOff_FlowSensor();

	void resetVgVlAutoTurnedOff_Tube();

	void setVgarantAutoTurnedOff_FOT();

	void resetVgAutoTurnedOff_FOT();

	bool isVgarantAutoTurnedOff_FOT();

	void setVgarantAutoTurnedOff_FlowSensor();

	void resetVgarantAutoTurnedOff_FlowSensor();

	bool isVgarantAutoTurnedOff_FlowSensor();

	void setVlimitAutoTurnedOff_FlowSensor();

	void resetVlimitAutoTurnedOff_FlowSensor();

	bool isVlimitAutoTurnedOff_FlowSensor();

	void setVgarantAutoTurnedOff_Tube();

	void resetVgarantAutoTurnedOff_Tube();

	bool isVgarantAutoTurnedOff_Tube();

	void setVlimitAutoTurnedOff_Tube();

	void resetVlimitAutoTurnedOff_Tube();

	bool isVlimitAutoTurnedOff_Tube();

	void CheckPRICOalarms();

	void resetPRICOAutoTurnedOff();

	void resetPRICOAutoTurneOnFlag();

	bool isPRICOAutoTurnedOff();

	void setPIPminAlarmText(CStringW alarmTxt);

	void setPIPmaxAlarmText(CStringW alarmTxt);

	void setPEEPminLowAlarmText(CStringW alarmTxt);

	void setPEEPminLowAlarmValue(int value);

	void setPEEPminHighAlarmText(CStringW alarmTxt);

	void setPEEPminHighAlarmValue(int value);

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
	void setAlimitState_BPMmaxLimit(eAlarmLimitState state);
	void setAlimitState_LeakmaxLimit(eAlarmLimitState state);
	void setAlimitState_MVmaxLimit(eAlarmLimitState state);
	void setAlimitState_MVminLimit(eAlarmLimitState state);
	void setAlimitState_PEEPminLimit(eAlarmLimitState state);
	void setAlimitState_PIPmaxLimit(eAlarmLimitState state);
	void setAlimitState_PIPminLimit(eAlarmLimitState state);
	void setAlimitState_MAPmaxLimit(eAlarmLimitState state);
	void setAlimitState_MAPminLimit(eAlarmLimitState state);
	void setAlimitState_DCO2maxLimit(eAlarmLimitState state);
	void setAlimitState_DCO2minLimit(eAlarmLimitState state);
	void setAlimitState_ETCO2maxLimit(eAlarmLimitState state);
	void setAlimitState_ETCO2minLimit(eAlarmLimitState state);
	void setAlimitState_FICO2maxLimit(eAlarmLimitState state);
	void setAlimitState_FICO2minLimit(eAlarmLimitState state);
	void setAlimitState_SPO2maxLimit(eAlarmLimitState state);
	void setAlimitState_SPO2minLimit(eAlarmLimitState state);
	void setAlimitState_PulseRatemaxLimit(eAlarmLimitState state);
	void setAlimitState_PulseRateminLimit(eAlarmLimitState state);
	void setAlimitState_SPO2_PIminLimit(eAlarmLimitState state);
	void setAlimitState_SPO2_SIQminLimit(eAlarmLimitState state);

	void setAlimitApnoe(int value);
	void setAlimitBPMmax(int value);
	void setAlimitMVmax(int value);
	void setAlimitMVmin(int value);
	void setAlimitLeakmax(int value);
	void setAlimitPEEPmin(int value);
	void setAlimitPIPmax(int value);
	void setAlimitPIPmin(int value);
	void setAlimitMAPmax(int value);
	void setAlimitMAPmin(int value);
	void setAlimitDCO2max(int value);
	void setAlimitDCO2min(int value);
	void setAlimitETCO2max(int value);
	void setAlimitETCO2min(int value);
	void setAlimitFICO2max(int value);
	void setAlimitFICO2min(int value);
	void setAlimitSPO2max(int value);
	void setAlimitSPO2min(int value);
	void setAlimitPulseRatemax(int value);
	void setAlimitPulseRatemin(int value);
	void setAlimitSPO2_PImin(int value);
	void setAlimitSPO2_SIQmin(int value);
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

	void SetAlimitStateCalculated();

	void checkLimits();
	void checkVentilationLimits();
	void checkCO2Limits();
	void checkSpO2Limits();

	void CheckAlarmStateBytes();
	void CheckFlowSensorState();

	bool isActiveAlarmHigherOrSamePriority(eAlarmPrio prioAlarm);

	bool isActiveAlarmHigherPriority(eAlarmPrio prioAlarm);

	bool CanSetAlarm_ChecksumConPIC();
	bool CanSetAlarm_DISCONNECTION();
	bool CanSetAlarm_TUBE_OCCLUSION();
	bool CanSetAlarm_ChecksumMonPIC();
	bool CanSetAlarm_RELAIS_DEFECT();
	bool CanSetAlarm_MIXER_AVCAL();
	bool CanSetAlarm_P_IN_MIXER();
	bool CanSetAlarm_VOLTAGE();
	bool CanSetAlarm_IF_SPI();
	bool CanSetAlarm_IF_DIO();
	bool CanSetAlarm_IF_COM();
	bool CanSetAlarm_IF_I2C();
	bool CanSetAlarm_IF_PIF();
	bool CanSetAlarm_Fan();
	bool CanSetAlarm_OUTOFMEMORY();
	bool CanSetAlarm_P_IN_O2();
	bool CanSetAlarm_P_IN_AIR();
	bool CanSetAlarm_TUBUSBLOCKED();
	bool CanSetAlarm_FLOW_SENSOR_DEFECT();
	bool CanSetAlarm_FLOW_SENSOR_CLEANING();
	bool CanSetAlarm_FLOW_SENSOR_NOTCONNECTED();
	bool CanSetAlarm_O2_SENSOR_DEFECT();
	bool CanSetAlarm_O2_SENSOR_USED();
	bool CanSetAlarm_O2_VALUE_INCORRECT();
	bool CanSetAlarm_O2_NOT_CALIBRATED();
	bool CanSetAlarm_PRICO_FiO2outOfRange();
	bool CanSetAlarm_CO2_MODULE_NOTCONNECTED();
	bool CanSetAlarm_CO2_FILTERLINE_NOTCONNECTED();
	bool CanSetAlarm_CO2_CHECKSAMPLINGLINE();
	bool CanSetAlarm_CO2_CHECKAIRWAYADAPTER();
	bool CanSetAlarm_CO2_SENSORFAULTY();
	bool CanSetAlarm_SPO2_SENSORFAULTY();
	bool CanSetAlarm_SPO2_CHECKSENSOR();
	bool CanSetAlarm_SPO2_MODULE_NOTCONNECTED();
	bool CanSetAlarm_PRICO_SIQvalue();
	bool CanSetAlarm_PRICO_FiO2max();
	bool CanSetAlarm_PRICO_FiO2min();
	bool CanSetAlarm_MVmax();
	bool CanSetAlarm_MVmin();
	bool CanSetAlarm_PIPmax();
	bool CanSetAlarm_PIPmin();
	bool CanSetAlarm_PEEPminLow();
	bool CanSetAlarm_PEEPminHigh();
	bool CanSetAlarm_BPMmax();
	bool CanSetAlarm_Leakmax();
	bool CanSetAlarm_Apnoe();
	bool CanSetAlarm_MAPmax();
	bool CanSetAlarm_MAPmin();
	bool CanSetAlarm_DCO2max();
	bool CanSetAlarm_DCO2min();
	bool CanSetAlarm_ETCO2max();
	bool CanSetAlarm_ETCO2min();
	bool CanSetAlarm_FICO2max();
	bool CanSetAlarm_FICO2min();
	bool CanSetAlarm_SPO2max();
	bool CanSetAlarm_SPO2min();
	bool CanSetAlarm_PulseRatemax();
	bool CanSetAlarm_PulseRatemin();
	bool CanSetAlarm_SPO2_PImin();
	bool CanSetAlarm_Pinsp_NotReached();
	bool CanSetAlarm_Vgaranty();
	bool CanSetAlarm_Vlimitted();
	bool CanSetAlarm_Accu_POWER();
	bool CanSetAlarm_Accu_60();
	bool CanSetAlarm_Accu_30();
	bool CanSetAlarm_Accu_15();
	bool CanSetAlarm_AccuEmpty();
	bool CanSetAlarm_AccuDefect();

	CTlsLoglist* getAlarmLog();
	void setLogTxt(CStringW sz);

	void setSPO2alarmDelay(eSPO2alarmdelay delay);

	bool isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay=0);//used for (dwLastTickCount+DELAY<getTickCount64())

protected:

	CAlarmMgmtHandler();

	virtual ~CAlarmMgmtHandler();

	CMVModel *getModel();

	void checkAutoEnablePRICO();
	void setPRICOAutoTurnedOff(bool bAutoTurnOn, eAlarmPrio prioAlarm);
	bool isPRICOAutoTurnedOn();
	eAlarmPrio getPRICOAutoAlarmPriority();

	void setLogAlarm(eAlarm	enAlarm, CStringW szAlarmTxt);
	CStringW getLogTxtAlarm(eAlarm enAlarm);

	bool SetAlarm_AccuEmpty(CStringW alarmTxt);
	bool SetAlarm_AccuDefect(CStringW alarmTxt);
	bool SetAlarm_ChecksumConPIC(CStringW alarmTxt);
	bool SetAlarm_ChecksumMonPIC(CStringW alarmTxt);
	bool SetAlarm_RELAIS_DEFECT(CStringW alarmTxt);
	bool SetAlarm_MIXER_AVCAL(CStringW alarmTxt);
	bool SetAlarm_P_IN_MIXER(CStringW alarmTxt);
	bool SetAlarm_VOLTAGE(CStringW alarmTxt);
	bool SetAlarm_IF_SPI(CStringW alarmTxt);
	bool SetAlarm_IF_DIO(CStringW alarmTxt);
	bool SetAlarm_IF_COM(CStringW alarmTxt);
	bool SetAlarm_IF_I2C(CStringW alarmTxt);
	bool SetAlarm_IF_PIF(CStringW alarmTxt);
	bool SetAlarm_OUTOFMEMORY(CStringW alarmTxt);
	bool SetAlarm_Fan(CStringW alarmTxt);
	bool SetAlarm_P_IN_O2(CStringW alarmTxt);
	bool SetAlarm_P_IN_AIR(CStringW alarmTxt);
	bool SetAlarm_TUBE_OCCLUSION(CStringW alarmTxt);
	bool SetAlarm_DISCONNECTION(CStringW alarmTxt);
	bool SetAlarm_TUBUSBLOCKED(CStringW alarmTxt);
	bool SetAlarm_FLOW_SENSOR_DEFECT(CStringW alarmTxt);
	bool SetAlarm_FLOW_SENSOR_CLEANING(CStringW alarmTxt);
	bool SetAlarm_FLOW_SENSOR_NOTCONNECTED(CStringW alarmTxt);
	bool SetAlarm_O2_SENSOR_DEFECT(CStringW alarmTxt);
	bool SetAlarm_O2_SENSOR_USED(CStringW alarmTxt);
	bool SetAlarm_O2_VALUE_INCORRECT(CStringW alarmTxt);
	bool SetAlarm_O2_NOT_CALIBRATED(CStringW alarmTxt);
	bool SetAlarm_PRICO_FiO2outOfRange(CStringW alarmTxt);
	bool SetAlarm_CO2_MODULE_NOTCONNECTED(CStringW alarmTxt);
	bool SetAlarm_CO2_FILTERLINE_NOTCONNECTED(CStringW alarmTxt);
	bool SetAlarm_CO2_CHECKSAMPLINGLINE(CStringW alarmTxt);
	bool SetAlarm_CO2_CHECKAIRWAYADAPTER(CStringW alarmTxt);
	bool SetAlarm_CO2_SENSORFAULTY(CStringW alarmTxt);
	bool SetAlarm_SPO2_SENSORFAULTY(CStringW alarmTxt);
	bool SetAlarm_SPO2_CHECKSENSOR(CStringW alarmTxt);
	bool SetAlarm_SPO2_MODULE_NOTCONNECTED(CStringW alarmTxt);
	bool SetAlarm_PRICO_SIQvalue(CStringW alarmTxt);
	bool SetAlarm_PRICO_FiO2max(CStringW alarmTxt);
	bool SetAlarm_PRICO_FiO2min(CStringW alarmTxt);
	bool SetAlarm_MVmax(CStringW alarmTxt);
	bool SetAlarm_MVmin(CStringW alarmTxt);
	bool SetAlarm_PIPmax();
	bool SetAlarm_PIPmin();
	bool SetAlarm_PEEPminLow();
	bool SetAlarm_PEEPminHigh();
	bool SetAlarm_BPMmax(CStringW alarmTxt);
	bool SetAlarm_Leakmax(CStringW alarmTxt);
	bool SetAlarm_Apnoe(CStringW alarmTxt);
	bool SetAlarm_MAPmax(CStringW alarmTxt);
	bool SetAlarm_MAPmin(CStringW alarmTxt);
	bool SetAlarm_DCO2max(CStringW alarmTxt);
	bool SetAlarm_DCO2min(CStringW alarmTxt);
	bool SetAlarm_ETCO2max(CStringW alarmTxt);
	bool SetAlarm_ETCO2min(CStringW alarmTxt);
	bool SetAlarm_FICO2max(CStringW alarmTxt);
	bool SetAlarm_FICO2min(CStringW alarmTxt);
	bool SetAlarm_SPO2max(CStringW alarmTxt);
	bool SetAlarm_SPO2min(CStringW alarmTxt);
	bool SetAlarm_PulseRatemax(CStringW alarmTxt);
	bool SetAlarm_PulseRatemin(CStringW alarmTxt);
	bool SetAlarm_SPO2_PImin(CStringW alarmTxt);
	bool SetAlarm_Pinsp_NotReached(CStringW alarmTxt);
	bool SetAlarm_Vlimitted(CStringW alarmTxt);
	bool SetAlarm_Vgaranty(CStringW alarmTxt);
	bool SetAlarm_Accu_POWER(CStringW alarmTxt);
	bool SetAlarm_Accu_60(CStringW alarmTxt);
	bool SetAlarm_Accu_30(CStringW alarmTxt);
	bool SetAlarm_Accu_15(CStringW alarmTxt);
public:
	static CAlarmPtr ALARM_NONE;	///< The alarm none
	static CAlarmPtr ALARM_Accu_Empty;  ///< The alarm accu empty
	static CAlarmPtr ALARM_SysFail_ChecksumConPIC;  ///< The alarm system fail checksum con picture
	static CAlarmPtr ALARM_SysFail_ChecksumMonPIC;  ///< The alarm system fail checksum monitor picture
	static CAlarmPtr ALARM_SysFail_RELAIS_DEFECT;   ///< The alarm system fail relais defect
	static CAlarmPtr ALARM_SysFail_P_IN_MIXER;  ///< The alarm system fail pin mixer
	static CAlarmPtr ALARM_SysFail_VOLTAGE; ///< The alarm system fail voltage
	static CAlarmPtr ALARM_SysFail_IF_SPI;  ///< The alarm system fail interface spi
	static CAlarmPtr ALARM_SysFail_IF_DIO;  ///< The alarm system fail interface dio
	static CAlarmPtr ALARM_SysFail_IF_COM;  ///< The alarm system fail interface com
	static CAlarmPtr ALARM_SysFail_IF_I2C;  ///< The alarm system fail interface i2c
	static CAlarmPtr ALARM_SysFail_IF_PIF;  ///< The alarm system fail interface pif
	static CAlarmPtr ALARM_SysFail_OUTOFMEMORY; ///< The alarm system fail outofmemory
	static CAlarmPtr ALARM_SysFail_Fan; ///< The alarm system fail fan
	static CAlarmPtr ALARM_SysFail_MIXER_AVCAL; ///< The alarm system fail mixer avcal
	static CAlarmPtr ALARM_Accu_Defect; ///< The alarm accu defect
	static CAlarmPtr ALARM_SysAl_P_IN_O2;   ///< The second alarm system  pin o2
	static CAlarmPtr ALARM_SysAl_P_IN_AIR;  ///< The alarm system pin air
	static CAlarmPtr ALARM_SysAl_TUBE_OCCLUSION;	///< The alarm system tube occlusion
	static CAlarmPtr ALARM_DISCONNECTION;   ///< The alarm disconnection
	static CAlarmPtr ALARM_TUBUSBLOCKED;	///< The alarm tubusblocked
	static CAlarmPtr ALARM_Sens_FLOW_SENSOR_DEFECT; ///< The alarm sensor flow sensor defect
	static CAlarmPtr ALARM_Sens_FLOW_SENSOR_CLEANING;   ///< The alarm sensor flow sensor cleaning
	static CAlarmPtr ALARM_Sens_FLOW_SENSOR_NOTCONNECTED;   ///< The alarm sensor flow sensor notconnected
	static CAlarmPtr ALARM_Sens_O2_SENSOR_DEFECT;   ///< The alarm sensor o2 sensor defect
	static CAlarmPtr ALARM_Sens_O2_SENSOR_USED; ///< The alarm sensor o2 sensor used
	static CAlarmPtr ALARM_Sens_O2_VALUE_INCORRECT; ///< The alarm sensor o2 value incorrect
	static CAlarmPtr ALARM_Sens_O2_NOT_CALIBRATED;  ///< The alarm sensor o2 not calibrated
	static CAlarmPtr ALARM_Sens_PRICO_FiO2outOfRange;   ///< The alarm sensor prico fio2 out of range
	static CAlarmPtr ALARM_Sens_CO2_MODULE_NOTCONNECTED;	///< The alarm sensor co2 module notconnected
	static CAlarmPtr ALARM_Sens_CO2_FILTERLINE_NOTCONNECTED;	///< The alarm sensor co2 filterline notconnected
	static CAlarmPtr ALARM_Sens_CO2_CHECKSAMPLINGLINE;  ///< The alarm sensor co2 checksamplingline
	static CAlarmPtr ALARM_Sens_CO2_CHECKAIRWAYADAPTER; ///< The alarm sensor co2 checkairwayadapter
	static CAlarmPtr ALARM_Sens_CO2_SENSORFAULTY;   ///< The alarm sensor co 2 sensorfaulty
	static CAlarmPtr ALARM_Sens_SPO2_MODULE_NOTCONNECTED;   ///< The alarm sensor spo2 module notconnected
	static CAlarmPtr ALARM_Sens_SPO2_SENSORFAULTY;  ///< The alarm sensor spo2 sensorfaulty
	static CAlarmPtr ALARM_Sens_SPO2_CHECKSENSOR;   ///< The alarm sensor spo2 checksensor
	static CAlarmPtr ALARM_PRICO_FiO2max;   ///< The alarm prico fio2 max
	static CAlarmPtr ALARM_PRICO_FiO2min;   ///< The alarm prico fio2 min
	static CAlarmPtr ALARM_PatAl_MVmax; ///< The alarm mv max
	static CAlarmPtr ALARM_PatAl_MVmin; ///< The alarm mv min
	static CAlarmPtr ALARM_PatAl_PIPmax;	///< The alarm pip max
	static CAlarmPtr ALARM_PatAl_PIPmin;	///< The alarm pip min
	static CAlarmPtr ALARM_PatAl_PEEPminLow;	///< The alarm peep min low
	static CAlarmPtr ALARM_PatAl_PEEPminHigh;   ///< The alarm peep min high
	static CAlarmPtr ALARM_PatAl_BPMmax;	///< The alarm bpm max
	static CAlarmPtr ALARM_PatAl_Leakmax;   ///< The alarm leak max
	static CAlarmPtr ALARM_PatAl_Apnoe; ///< The alarm apnoe
	static CAlarmPtr ALARM_PatAl_MAPmax;	///< The alarm map max
	static CAlarmPtr ALARM_PatAl_MAPmin;	///< The alarm map min
	static CAlarmPtr ALARM_PatAl_DCO2max;   ///< The alarm dco2 max
	static CAlarmPtr ALARM_PatAl_DCO2min;   ///< The alarm dco2 min
	static CAlarmPtr ALARM_PatAl_ETCO2max;  ///< The alarm etco2 max
	static CAlarmPtr ALARM_PatAl_ETCO2min;  ///< The alarm etco2 min
	static CAlarmPtr ALARM_PatAl_FICO2max;  ///< The alarm fico2 max
	static CAlarmPtr ALARM_PatAl_FICO2min;  ///< The alarm fico2 min
	static CAlarmPtr ALARM_PatAl_SPO2max;   ///< The alarm spo2 max
	static CAlarmPtr ALARM_PatAl_SPO2min;   ///< The alarm spo2 min
	static CAlarmPtr ALARM_PatAl_PulseRatemax;  ///< The alarm pulse rate max
	static CAlarmPtr ALARM_PatAl_PulseRatemin;  ///< The alarm pulse rate min
	static CAlarmPtr ALARM_PatAl_SPO2_PImin;	///< The alarm spo2 p min
	static CAlarmPtr ALARM_PatAl_SPO2_SIQmin;   ///< The alarm spo2 SIQmin
	static CAlarmPtr ALARM_SysLimit_Pinsp_NotReached;   ///< The alarm system limit pinsp not reached
	static CAlarmPtr ALARM_SysLimit_Vlimitted;  ///< The alarm system limit vlimitted
	static CAlarmPtr ALARM_SysLimit_Vgaranty;   ///< The alarm system limit vgaranty
	static CAlarmPtr ALARM_Accu_POWER;  ///< The alarm accu power
	static CAlarmPtr ALARM_Accu_60; ///< The alarm accu 60
	static CAlarmPtr ALARM_Accu_30; ///< The alarm accu 30
	static CAlarmPtr ALARM_Accu_15; ///< The alarm accu 15

	static CAlarmConditionHandler* ALARMLIMITS; ///< The alarmlimits
private:
	//singleton
	static CAlarmMgmtHandler* theAlarmHandler;  ///< the alarm handler
	

	CRITICAL_SECTION csAlarmList;   ///< List of create structure alarms
	CRITICAL_SECTION csActiveVentMode;  ///< The create struct active vent mode
	CRITICAL_SECTION csActiveAlarm; ///< The create struct active alarm
	CRITICAL_SECTION csAlarmSilentState;	///< State of the create structure alarm silent
	CRITICAL_SECTION csPrioActiveAlarm; ///< The create struct prio active alarm
	CRITICAL_SECTION csVGVLautoOffState;	///< State of the create structure vgvl auto off
	CRITICAL_SECTION csPRICOautoOffState;   ///< State of the create structure prico auto off

	CAlarmList* m_pAlarmlist;   ///< The alarmlist

	

	CMVModel *m_pModel; ///< The model
	static Logfile *m_AlarmLogfile; ///< The alarm logfile
	CTlsLoglist * m_AlarmLoglist;   ///< The alarm loglist

	//thread safe members
	eVentSilentState m_eAlarmSilentState;   ///< State of the alarm silent
	eAlarm m_eActivealarm;  ///< The activealarm
	eAlarmPrio m_ePrioActiveAlarm;  ///< The prio active alarm

	eVentMode m_eActiveVentMode;	///< The active vent mode

	CStringW m_szPIPmaxAlarmText;   ///< The pipmax alarm text
	CStringW m_szPIPminAlarmText;   ///< The pipmin alarm text
	CStringW m_szPEEPminLowAlarmText;   ///< The peepmin low alarm text
	CStringW m_szPEEPminHighAlarmText;  ///< The peepmin high alarm text
	int m_iPEEPminLowAlarmValue;	///< The peepmin low alarm value
	int m_iPEEPminHighAlarmValue;   ///< The peepmin high alarm value

	DWORD m_dwPminHighValueOk;  ///< The pmin high value ok

	//turn off/on automatic VG and VL
	bool m_bVgarantAutoTurnedOff_FOT;   ///< True to vgarant automatic turned off fot
	bool m_bVgarantAutoTurnedOff_FlowSensor;	///< True to vgarant automatic turned off flow sensor
	bool m_bVgarantAutoTurnedOff_Tube;  ///< True to vgarant automatic turned off tube

	bool m_bVlimitAutoTurnedOff_FlowSensor; ///< True to vlimit automatic turned off flow sensor
	bool m_bVlimitAutoTurnedOff_Tube;   ///< True to vlimit automatic turned off tube

	bool m_bPRICOAutoTurnedOff; ///< True to disable, false to enable the prico automatic turned
	bool m_bPRICOAutoTurnOn;	///< True to enable, false to disable the prico automatic turn
	eAlarmPrio m_ePRICOAutoAlarmPriority;   ///< The prico automatic alarm priority
	bool m_bStartupSilent;  ///< True to startup silent
};


