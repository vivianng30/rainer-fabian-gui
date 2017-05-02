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

#define MAXALARMTASKS 80

class CMVModel;
class CAlarmConditionHandler;

// CAlarmingHandler
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

	void setSPO2alarmDelay(UINT iDelayTime);

	bool isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay=0);//used for (dwLastTickCount+DELAY<getTickCount64())

protected:
	CAlarmMgmtHandler();
	virtual ~CAlarmMgmtHandler();

	CMVModel *getModel();

	void checkAutoEnablePRICO();
	void setPRICOAutoTurnedOff(bool bAutoTurnOn, eAlarmPrio prioAlarm);
	bool isPRICOAutoTurneOn();
	eAlarmPrio getPRICOAutoAlarmPriority();

	//alarm log
	void setLogAlarm(eAlarm	enAlarm, CStringW szAlarmTxt);
	CStringW getLogTxtAlarm(eAlarm enAlarm);

	//Power Alarm
	bool SetAlarm_AccuEmpty(CStringW alarmTxt);
	bool SetAlarm_AccuDefect(CStringW alarmTxt);
	//System failure
	bool SetAlarm_ChecksumConPIC(CStringW alarmTxt);
	bool SetAlarm_ChecksumMonPIC(CStringW alarmTxt);
	//AL_SysFail_SYSTEMFAIL				= 2,	
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
	//System alarms
	bool SetAlarm_P_IN_O2(CStringW alarmTxt);
	bool SetAlarm_P_IN_AIR(CStringW alarmTxt);
	bool SetAlarm_TUBE_OCCLUSION(CStringW alarmTxt);
	//Disconnection Alarms
	bool SetAlarm_DISCONNECTION(CStringW alarmTxt);
	//Tubus Alarms
	bool SetAlarm_TUBUSBLOCKED(CStringW alarmTxt);
	//Nebulizer Alarms
	/*bool SetAlarm_NEBULIZER_DISCONNECTION(CStringW alarmTxt);
	bool SetAlarm_NEBULIZER_SYSERROR(CStringW alarmTxt);*/
	
	//Sensor Alarms
	bool SetAlarm_FLOW_SENSOR_DEFECT(CStringW alarmTxt);
	bool SetAlarm_FLOW_SENSOR_CLEANING(CStringW alarmTxt);
	bool SetAlarm_FLOW_SENSOR_NOTCONNECTED(CStringW alarmTxt);
	bool SetAlarm_O2_SENSOR_DEFECT(CStringW alarmTxt);
	bool SetAlarm_O2_SENSOR_USED(CStringW alarmTxt);
	bool SetAlarm_O2_VALUE_INCORRECT(CStringW alarmTxt);
	bool SetAlarm_O2_NOT_CALIBRATED(CStringW alarmTxt);
	bool SetAlarm_PRICO_FiO2outOfRange(CStringW alarmTxt);
	//CO2 alarms
	bool SetAlarm_CO2_MODULE_NOTCONNECTED(CStringW alarmTxt);
	bool SetAlarm_CO2_FILTERLINE_NOTCONNECTED(CStringW alarmTxt);
	bool SetAlarm_CO2_CHECKSAMPLINGLINE(CStringW alarmTxt);
	bool SetAlarm_CO2_CHECKAIRWAYADAPTER(CStringW alarmTxt);
	bool SetAlarm_CO2_SENSORFAULTY(CStringW alarmTxt);
	//SPO2 alarms
	bool SetAlarm_SPO2_SENSORFAULTY(CStringW alarmTxt);
	bool SetAlarm_SPO2_CHECKSENSOR(CStringW alarmTxt);
	bool SetAlarm_SPO2_MODULE_NOTCONNECTED(CStringW alarmTxt);
	//Patient alarms
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
	//SysLimit Alarms
	bool SetAlarm_Pinsp_NotReached(CStringW alarmTxt);
	bool SetAlarm_Vlimitted(CStringW alarmTxt);
	bool SetAlarm_Vgaranty(CStringW alarmTxt);
	//akku alarms
	bool SetAlarm_Accu_POWER(CStringW alarmTxt);
	bool SetAlarm_Accu_60(CStringW alarmTxt);
	bool SetAlarm_Accu_30(CStringW alarmTxt);
	bool SetAlarm_Accu_15(CStringW alarmTxt);
public:
	static CAlarmPtr ALARM_NONE;
	static CAlarmPtr ALARM_Accu_Empty;
	static CAlarmPtr ALARM_SysFail_ChecksumConPIC;
	static CAlarmPtr ALARM_SysFail_ChecksumMonPIC;
	static CAlarmPtr ALARM_SysFail_RELAIS_DEFECT;
	static CAlarmPtr ALARM_SysFail_P_IN_MIXER;
	static CAlarmPtr ALARM_SysFail_VOLTAGE;
	static CAlarmPtr ALARM_SysFail_IF_SPI;
	static CAlarmPtr ALARM_SysFail_IF_DIO;
	static CAlarmPtr ALARM_SysFail_IF_COM;
	static CAlarmPtr ALARM_SysFail_IF_I2C;
	static CAlarmPtr ALARM_SysFail_IF_PIF;
	static CAlarmPtr ALARM_SysFail_OUTOFMEMORY;
	static CAlarmPtr ALARM_SysFail_Fan;
	static CAlarmPtr ALARM_SysFail_MIXER_AVCAL;
	static CAlarmPtr ALARM_Accu_Defect;
	static CAlarmPtr ALARM_SysAl_P_IN_O2;
	static CAlarmPtr ALARM_SysAl_P_IN_AIR;
	static CAlarmPtr ALARM_SysAl_TUBE_OCCLUSION;
	static CAlarmPtr ALARM_DISCONNECTION;
	static CAlarmPtr ALARM_TUBUSBLOCKED;
	static CAlarmPtr ALARM_Sens_FLOW_SENSOR_DEFECT;
	static CAlarmPtr ALARM_Sens_FLOW_SENSOR_CLEANING;
	static CAlarmPtr ALARM_Sens_FLOW_SENSOR_NOTCONNECTED;
	static CAlarmPtr ALARM_Sens_O2_SENSOR_DEFECT;
	static CAlarmPtr ALARM_Sens_O2_SENSOR_USED;
	static CAlarmPtr ALARM_Sens_O2_VALUE_INCORRECT;
	static CAlarmPtr ALARM_Sens_O2_NOT_CALIBRATED;
	static CAlarmPtr ALARM_Sens_PRICO_FiO2outOfRange;
	static CAlarmPtr ALARM_Sens_CO2_MODULE_NOTCONNECTED;
	static CAlarmPtr ALARM_Sens_CO2_FILTERLINE_NOTCONNECTED;
	static CAlarmPtr ALARM_Sens_CO2_CHECKSAMPLINGLINE;
	static CAlarmPtr ALARM_Sens_CO2_CHECKAIRWAYADAPTER;
	static CAlarmPtr ALARM_Sens_CO2_SENSORFAULTY;
	static CAlarmPtr ALARM_Sens_SPO2_MODULE_NOTCONNECTED;
	static CAlarmPtr ALARM_Sens_SPO2_SENSORFAULTY;
	static CAlarmPtr ALARM_Sens_SPO2_CHECKSENSOR;
	static CAlarmPtr ALARM_PRICO_FiO2max;
	static CAlarmPtr ALARM_PRICO_FiO2min;
	static CAlarmPtr ALARM_PatAl_MVmax;
	static CAlarmPtr ALARM_PatAl_MVmin;
	static CAlarmPtr ALARM_PatAl_PIPmax;
	static CAlarmPtr ALARM_PatAl_PIPmin;
	static CAlarmPtr ALARM_PatAl_PEEPminLow;
	static CAlarmPtr ALARM_PatAl_PEEPminHigh;
	static CAlarmPtr ALARM_PatAl_BPMmax;
	static CAlarmPtr ALARM_PatAl_Leakmax;
	static CAlarmPtr ALARM_PatAl_Apnoe;
	static CAlarmPtr ALARM_PatAl_DCO2max;
	static CAlarmPtr ALARM_PatAl_DCO2min;
	static CAlarmPtr ALARM_PatAl_ETCO2max;
	static CAlarmPtr ALARM_PatAl_ETCO2min;
	static CAlarmPtr ALARM_PatAl_FICO2max;
	static CAlarmPtr ALARM_PatAl_FICO2min;
	static CAlarmPtr ALARM_PatAl_SPO2max;
	static CAlarmPtr ALARM_PatAl_SPO2min;
	static CAlarmPtr ALARM_PatAl_PulseRatemax;
	static CAlarmPtr ALARM_PatAl_PulseRatemin;
	static CAlarmPtr ALARM_PatAl_SPO2_PImin;
	static CAlarmPtr ALARM_PatAl_SPO2_SIQmin;
	static CAlarmPtr ALARM_SysLimit_Pinsp_NotReached;
	static CAlarmPtr ALARM_SysLimit_Vlimitted;
	static CAlarmPtr ALARM_SysLimit_Vgaranty;
	static CAlarmPtr ALARM_Accu_POWER;
	static CAlarmPtr ALARM_Accu_60;
	static CAlarmPtr ALARM_Accu_30;
	static CAlarmPtr ALARM_Accu_15;

	static CAlarmConditionHandler* ALARMLIMITS;
private:
	//singleton
	static CAlarmMgmtHandler* theAlarmHandler;
	

	CRITICAL_SECTION csAlarmList;
	CRITICAL_SECTION csActiveVentMode;
	CRITICAL_SECTION csActiveAlarm;
	CRITICAL_SECTION csAlarmSilentState;
	CRITICAL_SECTION csPrioActiveAlarm;
	CRITICAL_SECTION csVGVLautoOffState;
	CRITICAL_SECTION csPRICOautoOffState;

	CAlarmList* m_pAlarmlist;

	

	CMVModel *m_pModel;
	static Logfile *m_AlarmLogfile;
	CTlsLoglist * m_AlarmLoglist;

	//thread safe members
	eVentSilentState m_eAlarmSilentState;
	eAlarm m_eActivealarm;
	eAlarmPrio m_ePrioActiveAlarm;

	eVentMode m_eActiveVentMode;

	CStringW m_szPIPmaxAlarmText;
	CStringW m_szPIPminAlarmText;
	CStringW m_szPEEPminLowAlarmText;
	CStringW m_szPEEPminHighAlarmText;
	int m_iPEEPminLowAlarmValue;
	int m_iPEEPminHighAlarmValue;

	DWORD m_dwPminHighValueOk;

	//turn off/on automatic VG and VL
	bool m_bVgarantAutoTurnedOff_FOT;
	bool m_bVgarantAutoTurnedOff_FlowSensor;
	bool m_bVgarantAutoTurnedOff_Tube;

	bool m_bVlimitAutoTurnedOff_FlowSensor;
	bool m_bVlimitAutoTurnedOff_Tube;

	bool m_bPRICOAutoTurnedOff;
	bool m_bPRICOAutoTurnOn;
	eAlarmPrio m_ePRICOAutoAlarmPriority;
	bool m_bStartupSilent;
};


