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

	/**********************************************************************************************//**
	 * Gets the instance.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	Null if it fails, else the instance.
	 **************************************************************************************************/

	static CAlarmMgmtHandler* getInstance();

	/**********************************************************************************************//**
	 * Destroys the instance.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	static void destroyInstance();

	/**********************************************************************************************//**
	 * Initializes this object.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void init();

	/**********************************************************************************************//**
	 * Deinits this object.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void deinit();

	/**********************************************************************************************//**
	 * Initializes the alarm list.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void initAlarmList();

	/**********************************************************************************************//**
	 * Check log alarm date.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void checkLogAlarmDate();

	/**********************************************************************************************//**
	 * Loads the configuration.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void loadConfig();

	/**********************************************************************************************//**
	 * Vent mode changed.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void ventModeChanged();

	/**********************************************************************************************//**
	 * Gets safe active vent mode.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The safe active vent mode.
	 **************************************************************************************************/

	eVentMode getSafeActiveVentMode();

	/**********************************************************************************************//**
	 * Sets state of alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarm  	The alarm.
	 * \param	eState 	The state.
	 * \param	bUpdate	(Optional) True to update.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool setStateOfAlarm(eAlarm alarm, eStateOfAlarm eState, bool bUpdate=true);

	/**********************************************************************************************//**
	 * Gets active alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The active alarm.
	 **************************************************************************************************/

	eAlarm getActiveAlarm();

	/**********************************************************************************************//**
	 * Sets active alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarm	The alarm.
	 **************************************************************************************************/

	void setActiveAlarm(eAlarm alarm);

	/**********************************************************************************************//**
	 * Gets alarm silent state.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alarm silent state.
	 **************************************************************************************************/

	eVentSilentState getAlarmSilentState();

	/**********************************************************************************************//**
	 * Sets alarm silent state.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlarmSilentState(eVentSilentState state);

	/**********************************************************************************************//**
	 * Gets prio active alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The prio active alarm.
	 **************************************************************************************************/

	eAlarmPrio getPrioActiveAlarm();

	/**********************************************************************************************//**
	 * Sets prio active alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	prio	The prio.
	 **************************************************************************************************/

	void setPrioActiveAlarm(eAlarmPrio prio);

	/**********************************************************************************************//**
	 * Query if 'type' is alarm type.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	type	The type.
	 *
	 * \return	True if alarm type, false if not.
	 **************************************************************************************************/

	bool isAlarmType(eAlarmType type);

	/**********************************************************************************************//**
	 * Query if 'type' is active alarm type.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	type	The type.
	 *
	 * \return	True if active alarm type, false if not.
	 **************************************************************************************************/

	bool isActiveAlarmType(eAlarmType type);

	/**********************************************************************************************//**
	 * Query if 'type' is signaled alarm type.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	type	The type.
	 *
	 * \return	True if signaled alarm type, false if not.
	 **************************************************************************************************/

	bool isSignaledAlarmType(eAlarmType type);

	/**********************************************************************************************//**
	 * Query if this object is alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if alarm, false if not.
	 **************************************************************************************************/

	bool isAlarm();

	/**********************************************************************************************//**
	 * Query if this object is active alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if active alarm, false if not.
	 **************************************************************************************************/

	bool isActiveAlarm();

	/**********************************************************************************************//**
	 * Query if this object is signaled alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if signaled alarm, false if not.
	 **************************************************************************************************/

	bool isSignaledAlarm();

	/**********************************************************************************************//**
	 * Deletes the alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarm  	The alarm.
	 * \param	bUpdate	(Optional) True to update.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool deleteAlarm(eAlarm alarm, bool bUpdate=true);

	/**********************************************************************************************//**
	 * Deletes the alarm type.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	type   	(Optional) The type.
	 * \param	bUpdate	(Optional) True to update.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool deleteAlarmType(eAlarmType type=AT_NONE, bool bUpdate=true);

	/**********************************************************************************************//**
	 * Deletes the lower prio alarms described by prio.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	prio	The prio.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool deleteLowerPrioAlarms(eAlarmPrio prio);

	/**********************************************************************************************//**
	 * Deletes the akku alarms.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool deleteAkkuAlarms();

	/**********************************************************************************************//**
	 * Deletes the signaled alarmlimit states.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void deleteSignaledAlarmlimitStates();

	/**********************************************************************************************//**
	 * Deletes the signaled alarms.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void deleteSignaledAlarms();

	/**********************************************************************************************//**
	 * Deletes all alarms described by bDeleteAkkuAlarms.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	bDeleteAkkuAlarms	True to delete the akku alarms.
	 **************************************************************************************************/

	void deleteAllAlarms(bool bDeleteAkkuAlarms);

	/**********************************************************************************************//**
	 * Sets lower prio active alarms signaled.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	prio	The prio.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool setLowerPrioActiveAlarmsSignaled(eAlarmPrio prio);

	/**********************************************************************************************//**
	 * Sets startup silent.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	bState	True to state.
	 **************************************************************************************************/

	void setStartupSilent(bool bState);

	/**********************************************************************************************//**
	 * Gets startup silent.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool getStartupSilent();

	/**********************************************************************************************//**
	 * Sets alarmsilent active.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void setAlarmsilentActive();//aktiviert alle AlarmSilentStates ausser SystemSilent!!!!!

	/**********************************************************************************************//**
	 * Sets system silent active.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void setSystemSilentActive();

	/**********************************************************************************************//**
	 * Sets system silent.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void setSystemSilent();

	/**********************************************************************************************//**
	 * Sets alarm silent.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	bDelSysSilent	(Optional) True to delete the system silent.
	 * \param	bDelAllAlarms	(Optional) True to delete the alarms.
	 **************************************************************************************************/

	void setAlarmSilent(bool bDelSysSilent=false,bool bDelAllAlarms=false);

	/**********************************************************************************************//**
	 * Sets automatic silent.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	bDelSysSilent	(Optional) True to delete the system silent.
	 * \param	bDelSilent   	(Optional) True to delete the silent.
	 * \param	bDelManSilent	(Optional) True to delete the manager silent.
	 **************************************************************************************************/

	void setAutoSilent(bool bDelSysSilent=false, bool bDelSilent=false, bool bDelManSilent=false);

	/**********************************************************************************************//**
	 * Sets manual silent.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	bRefreshTime	(Optional) True to refresh time.
	 **************************************************************************************************/

	void setManualSilent(bool bRefreshTime=true);

	/**********************************************************************************************//**
	 * Sets calibration silent.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void setCalibrationSilent();

	/**********************************************************************************************//**
	 * Query if this object is manual silent state.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if manual silent state, false if not.
	 **************************************************************************************************/

	bool isManualSilentState();

	/**********************************************************************************************//**
	 * Gets alarm text.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarm	The alarm.
	 *
	 * \return	The alarm text.
	 **************************************************************************************************/

	CStringW getAlarmText(eAlarm alarm);

	/**********************************************************************************************//**
	 * Gets alarm priority.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarm	The alarm.
	 *
	 * \return	The alarm prio.
	 **************************************************************************************************/

	eAlarmPrio getAlarmPrio(eAlarm alarm);

	/**********************************************************************************************//**
	 * Gets alarm type.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarm	The alarm.
	 *
	 * \return	The alarm type.
	 **************************************************************************************************/

	eAlarmType getAlarmType(eAlarm alarm);

	/**********************************************************************************************//**
	 * Gets signaled alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The signaled alarm.
	 **************************************************************************************************/

	eAlarm getSignaledAlarm();

	/**********************************************************************************************//**
	 * Sets an alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarm   	The alarm.
	 * \param	alarmTxt	(Optional) The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool setAlarm(eAlarm alarm, CStringW alarmTxt=_T(""));

	/**********************************************************************************************//**
	 * Sets active alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	actAlarm	The act alarm.
	 * \param	alarmTxt	The alarm text.
	 **************************************************************************************************/

	void setActiveAlarm(eAlarm actAlarm, CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Gets the next active alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The next active alarm.
	 **************************************************************************************************/

	eAlarm getNextActiveAlarm();

	/**********************************************************************************************//**
	 * Queries if the prico relevant alarm is active.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if the pric orelevant alarm is active, false if not.
	 **************************************************************************************************/

	bool isPRICOrelevantAlarmActive();

	/**********************************************************************************************//**
	 * Queries if the fot relevant alarm is active.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if the fo trelevant alarm is active, false if not.
	 **************************************************************************************************/

	bool isFOTrelevantAlarmActive();

	/**********************************************************************************************//**
	 * Updates the alarm sound.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void updateAlarmSound();

	/**********************************************************************************************//**
	 * Updates the alarm states silent state dependend.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void updateAlarmStatesSilentStateDependend();

	/**********************************************************************************************//**
	 * Updates the active alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void updateActiveAlarm();

	/**********************************************************************************************//**
	 * Updates the active alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarm	The alarm.
	 * \param	state	The state.
	 **************************************************************************************************/

	void updateActiveAlarm(eAlarm alarm,eStateOfAlarm state);

	/**********************************************************************************************//**
	 * Sets alarm state pdms.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	eAlarmToSet	Set the alarm to belongs to.
	 * \param	state	   	The state.
	 **************************************************************************************************/

	void setAlarmStatePDMS(eAlarm eAlarmToSet, eStateOfAlarm state);

	/**********************************************************************************************//**
	 * Resets the vg vl automatic turned off.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void resetVgVlAutoTurnedOff();

	/**********************************************************************************************//**
	 * Resets the vg vl automatic turned off flow sensor alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void resetVgVlAutoTurnedOff_FlowSensor();

	/**********************************************************************************************//**
	 * Resets the vg vl automatic turned off tube alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void resetVgVlAutoTurnedOff_Tube();

	/**********************************************************************************************//**
	 * Sets vgarant automatic turned off fot alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void setVgarantAutoTurnedOff_FOT();

	/**********************************************************************************************//**
	 * Resets the vg automatic turned off fot alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void resetVgAutoTurnedOff_FOT();

	/**********************************************************************************************//**
	 * Query if this object is vgarant automatic turned off fot alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if vgarant automatic turned off fot, false if not.
	 **************************************************************************************************/

	bool isVgarantAutoTurnedOff_FOT();

	/**********************************************************************************************//**
	 * Sets vgarant automatic turned off flow sensor alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void setVgarantAutoTurnedOff_FlowSensor();

	/**********************************************************************************************//**
	 * Resets the vgarant automatic turned off flow sensor alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void resetVgarantAutoTurnedOff_FlowSensor();

	/**********************************************************************************************//**
	 * Query if this object is vgarant automatic turned off flow sensor alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if vgarant automatic turned off flow sensor, false if not.
	 **************************************************************************************************/

	bool isVgarantAutoTurnedOff_FlowSensor();

	/**********************************************************************************************//**
	 * Sets vlimit automatic turned off flow sensor alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void setVlimitAutoTurnedOff_FlowSensor();

	/**********************************************************************************************//**
	 * Resets the vlimit automatic turned off flow sensor alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void resetVlimitAutoTurnedOff_FlowSensor();

	/**********************************************************************************************//**
	 * Query if this object is vlimit automatic turned off flow sensor alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if vlimit automatic turned off flow sensor, false if not.
	 **************************************************************************************************/

	bool isVlimitAutoTurnedOff_FlowSensor();

	/**********************************************************************************************//**
	 * Sets vgarant automatic turned off tube alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void setVgarantAutoTurnedOff_Tube();

	/**********************************************************************************************//**
	 * Resets the vgarant automatic turned off tube alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void resetVgarantAutoTurnedOff_Tube();

	/**********************************************************************************************//**
	 * Query if this object is vgarant automatic turned off tube alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if vgarant automatic turned off tube, false if not.
	 **************************************************************************************************/

	bool isVgarantAutoTurnedOff_Tube();

	/**********************************************************************************************//**
	 * Sets vlimit automatic turned off tube alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void setVlimitAutoTurnedOff_Tube();

	/**********************************************************************************************//**
	 * Resets the vlimit automatic turned off tube alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void resetVlimitAutoTurnedOff_Tube();

	/**********************************************************************************************//**
	 * Query if this object is vlimit automatic turned off tube alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if vlimit automatic turned off tube, false if not.
	 **************************************************************************************************/

	bool isVlimitAutoTurnedOff_Tube();

	/**********************************************************************************************//**
	 * Check pric oalarms.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void CheckPRICOalarms();

	/**********************************************************************************************//**
	 * Resets the prico automatic turned off.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void resetPRICOAutoTurnedOff();

	/**********************************************************************************************//**
	 * Resets the prico automatic turne on flag.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void resetPRICOAutoTurneOnFlag();

	/**********************************************************************************************//**
	 * Query if this object is prico automatic turned off.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if prico automatic turned off, false if not.
	 **************************************************************************************************/

	bool isPRICOAutoTurnedOff();

	/**********************************************************************************************//**
	 * Sets pip min alarm text.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 **************************************************************************************************/

	void setPIPminAlarmText(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets pip max alarm text.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 **************************************************************************************************/

	void setPIPmaxAlarmText(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets peep min low alarm text.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 **************************************************************************************************/

	void setPEEPminLowAlarmText(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets peep min low alarm value.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setPEEPminLowAlarmValue(int value);

	/**********************************************************************************************//**
	 * Sets peep min high alarm text.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 **************************************************************************************************/

	void setPEEPminHighAlarmText(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets peep min high alarm value.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setPEEPminHighAlarmValue(int value);

	/**********************************************************************************************//**
	 * Gets alimit state mv max limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state mv max limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_MVmaxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state mv min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state mv min limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_MVminLimit();

	/**********************************************************************************************//**
	 * Gets alimit state pip max limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state pip max limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_PIPmaxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state pip min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state pip min limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_PIPminLimit();

	/**********************************************************************************************//**
	 * Gets alimit state peep min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state peep min limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_PEEPminLimit();

	/**********************************************************************************************//**
	 * Gets alimit state bpm max limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state bpm max limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_BPMmaxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state leak max limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state leak max limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_LeakmaxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state apnoe limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state apnoe limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_ApnoeLimit();

	/**********************************************************************************************//**
	 * Gets alimit state map max limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state map max limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_MAPmaxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state map min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state map min limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_MAPminLimit();

	/**********************************************************************************************//**
	 * Gets alimit state dco2 max limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state dco2 vmax limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_DCO2maxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state dco2 min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state dco2 min limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_DCO2minLimit();

	/**********************************************************************************************//**
	 * Gets alimit state etco2 max limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state etco2 max limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_ETCO2maxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state etco2 min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state etco2 min limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_ETCO2minLimit();

	/**********************************************************************************************//**
	 * Gets alimit state fico2 max limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state fico2 max limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_FICO2maxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state fico2 min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state fico2min limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_FICO2minLimit();

	/**********************************************************************************************//**
	 * Gets alimit state spo2 max limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state spo2max limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_SPO2maxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state spo2 min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state spo2min limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_SPO2minLimit();

	/**********************************************************************************************//**
	 * Gets alimit state pulse ratemax limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state pulse ratemax limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_PulseRatemaxLimit();

	/**********************************************************************************************//**
	 * Gets alimit state pulse ratemin limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state pulse ratemin limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_PulseRateminLimit();

	/**********************************************************************************************//**
	 * Gets alimit state spo2 pi min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state spo2 pimin limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_SPO2_PIminLimit();

	/**********************************************************************************************//**
	 * Gets alimit state spo2 SIQ min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit state spo2 SIQmin limit.
	 **************************************************************************************************/

	eAlarmLimitState getAlimitState_SPO2_SIQminLimit();

	/**********************************************************************************************//**
	 * Sets alimit state apnoe limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_ApnoeLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state bpm max limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_BPMmaxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state leak max limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_LeakmaxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state mv max limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_MVmaxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state mv min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_MVminLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state peep min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_PEEPminLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pip max limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_PIPmaxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pip min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_PIPminLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state map max limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_MAPmaxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state map min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_MAPminLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state dco2 max limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_DCO2maxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state dco2 min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_DCO2minLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state etco2 max limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_ETCO2maxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state etco2 min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_ETCO2minLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state fico2 max limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_FICO2maxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state fico2 min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_FICO2minLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state spo2 max limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_SPO2maxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state spo2 min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_SPO2minLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pulse ratemax limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_PulseRatemaxLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state pulse ratemin limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_PulseRateminLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state spo2 pi min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_SPO2_PIminLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit state spo2 SIQ min limit.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	state	The state.
	 **************************************************************************************************/

	void setAlimitState_SPO2_SIQminLimit(eAlarmLimitState state);

	/**********************************************************************************************//**
	 * Sets alimit apnoe.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitApnoe(int value);

	/**********************************************************************************************//**
	 * Sets alimit bpm max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitBPMmax(int value);

	/**********************************************************************************************//**
	 * Sets alimit mv max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitMVmax(int value);

	/**********************************************************************************************//**
	 * Sets alimit mv min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitMVmin(int value);

	/**********************************************************************************************//**
	 * Sets alimit leak max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitLeakmax(int value);

	/**********************************************************************************************//**
	 * Sets alimit peep min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPEEPmin(int value);

	/**********************************************************************************************//**
	 * Sets alimit pip max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPIPmax(int value);

	/**********************************************************************************************//**
	 * Sets alimit pip min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPIPmin(int value);

	/**********************************************************************************************//**
	 * Sets alimit map max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitMAPmax(int value);

	/**********************************************************************************************//**
	 * Sets alimit map min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitMAPmin(int value);

	/**********************************************************************************************//**
	 * Sets alimit dco2 max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitDCO2max(int value);

	/**********************************************************************************************//**
	 * Sets alimit dco2 min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitDCO2min(int value);

	/**********************************************************************************************//**
	 * Sets alimit etco2 max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitETCO2max(int value);

	/**********************************************************************************************//**
	 * Sets alimit etco2 min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitETCO2min(int value);

	/**********************************************************************************************//**
	 * Sets alimit fico2 max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitFICO2max(int value);

	/**********************************************************************************************//**
	 * Sets alimit fico2 min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitFICO2min(int value);

	/**********************************************************************************************//**
	 * Sets alimit spo2 max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitSPO2max(int value);

	/**********************************************************************************************//**
	 * Sets alimit spo2 min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitSPO2min(int value);

	/**********************************************************************************************//**
	 * Sets alimit pulse ratemax.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPulseRatemax(int value);

	/**********************************************************************************************//**
	 * Sets alimit pulse ratemin.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitPulseRatemin(int value);

	/**********************************************************************************************//**
	 * Sets alimit spo2 pi min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitSPO2_PImin(int value);

	/**********************************************************************************************//**
	 * Sets alimit spo2 SIQ min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	value	The value.
	 **************************************************************************************************/

	void setAlimitSPO2_SIQmin(int value);

	/**********************************************************************************************//**
	 * Sets alimits minimum maximum range cpap.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void setAlimitsMinMaxRangeCpap();

	/**********************************************************************************************//**
	 * Sets alimits minimum maximum range ncpap.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void setAlimitsMinMaxRangeNCPAP();

	/**********************************************************************************************//**
	 * Sets alimits minimum maximum range duopap.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void setAlimitsMinMaxRangeDUOPAP();

	/**********************************************************************************************//**
	 * Sets alimits minimum maximum range pressure.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void setAlimitsMinMaxRangePressure();

	/**********************************************************************************************//**
	 * Gets alimit mv min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit mv min.
	 **************************************************************************************************/

	int getAlimitMVmin();

	/**********************************************************************************************//**
	 * Gets alimit mv max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit mv max.
	 **************************************************************************************************/

	int getAlimitMVmax();

	/**********************************************************************************************//**
	 * Gets alimit pip max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit pip max.
	 **************************************************************************************************/

	int getAlimitPIPmax();

	/**********************************************************************************************//**
	 * Gets alimit pip min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit pip min.
	 **************************************************************************************************/

	int getAlimitPIPmin();

	/**********************************************************************************************//**
	 * Gets alimit peep min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit peep min.
	 **************************************************************************************************/

	int getAlimitPEEPmin();

	/**********************************************************************************************//**
	 * Gets alimit leak max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit leak max.
	 **************************************************************************************************/

	int getAlimitLeakmax();

	/**********************************************************************************************//**
	 * Gets alimit bpm max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit bpm max.
	 **************************************************************************************************/

	int getAlimitBPMmax();

	/**********************************************************************************************//**
	 * Gets alimit apnoe.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit apnoe.
	 **************************************************************************************************/

	int getAlimitApnoe();

	/**********************************************************************************************//**
	 * Gets alimit map max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit map max.
	 **************************************************************************************************/

	int getAlimitMAPmax();

	/**********************************************************************************************//**
	 * Gets alimit map min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit map min.
	 **************************************************************************************************/

	int getAlimitMAPmin();

	/**********************************************************************************************//**
	 * Gets alimit dco2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit dco2min.
	 **************************************************************************************************/

	int getAlimitDCO2min();

	/**********************************************************************************************//**
	 * Gets alimit dco2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit dco2max.
	 **************************************************************************************************/

	int getAlimitDCO2max();

	/**********************************************************************************************//**
	 * Gets alimit etco2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit etco2min.
	 **************************************************************************************************/

	int getAlimitETCO2min();

	/**********************************************************************************************//**
	 * Gets alimit etco2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit etco2max.
	 **************************************************************************************************/

	int getAlimitETCO2max();

	/**********************************************************************************************//**
	 * Gets alimit fico2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit fico2min.
	 **************************************************************************************************/

	int getAlimitFICO2min();

	/**********************************************************************************************//**
	 * Gets alimit fico2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit fico2max.
	 **************************************************************************************************/

	int getAlimitFICO2max();

	/**********************************************************************************************//**
	 * Gets alimit spo2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit spo2max.
	 **************************************************************************************************/

	int getAlimitSPO2max();

	/**********************************************************************************************//**
	 * Gets alimit spo2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit spo2min.
	 **************************************************************************************************/

	int getAlimitSPO2min();

	/**********************************************************************************************//**
	 * Gets alimit pulse ratemax.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit pulse ratemax.
	 **************************************************************************************************/

	int getAlimitPulseRatemax();

	/**********************************************************************************************//**
	 * Gets alimit pulse ratemin.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit pulse ratemin.
	 **************************************************************************************************/

	int getAlimitPulseRatemin();

	/**********************************************************************************************//**
	 * Gets alimit spo2 pimin.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit spo2 pimin.
	 **************************************************************************************************/

	int getAlimitSPO2_PImin();

	/**********************************************************************************************//**
	 * Gets alimit spo2 SIQmin.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit spo2 SIQmin.
	 **************************************************************************************************/

	int getAlimitSPO2_SIQmin();

	/**********************************************************************************************//**
	 * Sets alimit maximum range pip max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangePIPmax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range pip max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangePIPmax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range pip min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangePIPmin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range pip min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangePIPmin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range peep min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangePEEPmin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range peep min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangePEEPmin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range mv min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeMVmin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range mv min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeMVmin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range mv max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeMVmax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range mv max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeMVmax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range leakmax.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeLeakmax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range leakmax.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeLeakmax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range bpm max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeBPMmax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range bpm max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeBPMmax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range apnoe.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeApnoe(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range apnoe.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeApnoe(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range map max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeMAPmax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range map max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeMAPmax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range map min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeMAPmin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range map min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeMAPmin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range dco2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeDCO2min(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range dco2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeDCO2min(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range dco2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeDCO2max(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range dco2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeDCO2max(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range etco2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeETCO2min(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range etco2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeETCO2min(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range etco2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeETCO2max(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range etco2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeETCO2max(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range fico2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeFICO2min(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range fico2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeFICO2min(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range fico2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeFICO2max(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range fico2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeFICO2max(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range spo2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeSPO2max(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range spo2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeSPO2max(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range spo2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeSPO2min(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range spo2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeSPO2min(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range pulse ratemin.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangePulseRatemin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range pulse ratemax.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangePulseRatemax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range pulse ratemax.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangePulseRatemax(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range pulse ratemin.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangePulseRatemin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit maximum range spo2 pimin.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMaxRangeSPO2_PImin(int iRangeVal);

	/**********************************************************************************************//**
	 * Sets alimit minimum range spo2 pimin.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	iRangeVal	Zero-based index of the range value.
	 **************************************************************************************************/

	void setAlimitMinRangeSPO2_PImin(int iRangeVal);

	/**********************************************************************************************//**
	 * Gets alimit maximum range pip max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range pip max.
	 **************************************************************************************************/

	int getAlimitMaxRangePIPmax();

	/**********************************************************************************************//**
	 * Gets alimit minimum range pip max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range pip max.
	 **************************************************************************************************/

	int getAlimitMinRangePIPmax();

	/**********************************************************************************************//**
	 * Gets alimit maximum range pip min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range pip min.
	 **************************************************************************************************/

	int getAlimitMaxRangePIPmin();

	/**********************************************************************************************//**
	 * Gets alimit minimum range pip min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range pip min.
	 **************************************************************************************************/

	int getAlimitMinRangePIPmin();

	/**********************************************************************************************//**
	 * Gets alimit maximum range peep min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range peep min.
	 **************************************************************************************************/

	int getAlimitMaxRangePEEPmin();

	/**********************************************************************************************//**
	 * Gets alimit minimum range peep min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range peep min.
	 **************************************************************************************************/

	int getAlimitMinRangePEEPmin();

	/**********************************************************************************************//**
	 * Gets alimit maximum range mv min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range mv min.
	 **************************************************************************************************/

	int getAlimitMaxRangeMVmin();

	/**********************************************************************************************//**
	 * Gets alimit minimum range mv min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range mv min.
	 **************************************************************************************************/

	int getAlimitMinRangeMVmin();

	/**********************************************************************************************//**
	 * Gets alimit maximum range mv max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range mv max.
	 **************************************************************************************************/

	int getAlimitMaxRangeMVmax();

	/**********************************************************************************************//**
	 * Gets alimit minimum range mv max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range mv max.
	 **************************************************************************************************/

	int getAlimitMinRangeMVmax();

	/**********************************************************************************************//**
	 * Gets alimit maximum range leak max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range leak max.
	 **************************************************************************************************/

	int getAlimitMaxRangeLeakmax();

	/**********************************************************************************************//**
	 * Gets alimit minimum range leak max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range leak max.
	 **************************************************************************************************/

	int getAlimitMinRangeLeakmax();

	/**********************************************************************************************//**
	 * Gets alimit maximum range bpmmax.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range bpm max.
	 **************************************************************************************************/

	int getAlimitMaxRangeBPMmax();

	/**********************************************************************************************//**
	 * Gets alimit minimum range bpm max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range bpm max.
	 **************************************************************************************************/

	int getAlimitMinRangeBPMmax();

	/**********************************************************************************************//**
	 * Gets alimit maximum range apnoe.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range apnoe.
	 **************************************************************************************************/

	int getAlimitMaxRangeApnoe();

	/**********************************************************************************************//**
	 * Gets alimit minimum range apnoe.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range apnoe.
	 **************************************************************************************************/

	int getAlimitMinRangeApnoe();

	/**********************************************************************************************//**
	 * Gets alimit maximum range map max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range map max.
	 **************************************************************************************************/

	int getAlimitMaxRangeMAPmax();

	/**********************************************************************************************//**
	 * Gets alimit minimum range map max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range map max.
	 **************************************************************************************************/

	int getAlimitMinRangeMAPmax();

	/**********************************************************************************************//**
	 * Gets alimit maximum range map min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range map min.
	 **************************************************************************************************/

	int getAlimitMaxRangeMAPmin();

	/**********************************************************************************************//**
	 * Gets alimit minimum range map min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range map min.
	 **************************************************************************************************/

	int getAlimitMinRangeMAPmin();

	/**********************************************************************************************//**
	 * Gets alimit maximum range dco2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range dco2min.
	 **************************************************************************************************/

	int getAlimitMaxRangeDCO2min();

	/**********************************************************************************************//**
	 * Gets alimit minimum range dco2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range dco2min.
	 **************************************************************************************************/

	int getAlimitMinRangeDCO2min();

	/**********************************************************************************************//**
	 * Gets alimit maximum range dco2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range dco2max.
	 **************************************************************************************************/

	int getAlimitMaxRangeDCO2max();

	/**********************************************************************************************//**
	 * Gets alimit minimum range dco2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range dco2max.
	 **************************************************************************************************/

	int getAlimitMinRangeDCO2max();

	/**********************************************************************************************//**
	 * Gets alimit maximum range etco2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range etco2min.
	 **************************************************************************************************/

	int getAlimitMaxRangeETCO2min();

	/**********************************************************************************************//**
	 * Gets alimit minimum range etco2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range etco2min.
	 **************************************************************************************************/

	int getAlimitMinRangeETCO2min();

	/**********************************************************************************************//**
	 * Gets alimit maximum range etco2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range etco2max.
	 **************************************************************************************************/

	int getAlimitMaxRangeETCO2max();

	/**********************************************************************************************//**
	 * Gets alimit minimum range etco2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range etco2max.
	 **************************************************************************************************/

	int getAlimitMinRangeETCO2max();

	/**********************************************************************************************//**
	 * Gets alimit maximum range fico2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range fico2min.
	 **************************************************************************************************/

	int getAlimitMaxRangeFICO2min();

	/**********************************************************************************************//**
	 * Gets alimit minimum range fico2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range fico2min.
	 **************************************************************************************************/

	int getAlimitMinRangeFICO2min();

	/**********************************************************************************************//**
	 * Gets alimit maximum range fico2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range fico2max.
	 **************************************************************************************************/

	int getAlimitMaxRangeFICO2max();

	/**********************************************************************************************//**
	 * Gets alimit minimum range fico2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range fico2max.
	 **************************************************************************************************/

	int getAlimitMinRangeFICO2max();

	/**********************************************************************************************//**
	 * Gets alimit maximum range spo2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range spo2max.
	 **************************************************************************************************/

	int getAlimitMaxRangeSPO2max();

	/**********************************************************************************************//**
	 * Gets alimit minimum range spo2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range spo2max.
	 **************************************************************************************************/

	int getAlimitMinRangeSPO2max();

	/**********************************************************************************************//**
	 * Gets alimit maximum range spo2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range spo2min.
	 **************************************************************************************************/

	int getAlimitMaxRangeSPO2min();

	/**********************************************************************************************//**
	 * Gets alimit minimum range spo2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range spo2min.
	 **************************************************************************************************/

	int getAlimitMinRangeSPO2min();

	/**********************************************************************************************//**
	 * Gets alimit maximum range pulse ratemin.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range pulse ratemin.
	 **************************************************************************************************/

	int getAlimitMaxRangePulseRatemin();

	/**********************************************************************************************//**
	 * Gets alimit minimum range pulse ratemax.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range pulse ratemax.
	 **************************************************************************************************/

	int getAlimitMinRangePulseRatemax();

	/**********************************************************************************************//**
	 * Gets alimit maximum range pulse ratemax.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range pulse ratemax.
	 **************************************************************************************************/

	int getAlimitMaxRangePulseRatemax();

	/**********************************************************************************************//**
	 * Gets alimit minimum range pulse ratemin.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range pulse ratemin.
	 **************************************************************************************************/

	int getAlimitMinRangePulseRatemin();

	/**********************************************************************************************//**
	 * Gets alimit maximum range spo2 pimin.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range spo2 pimin.
	 **************************************************************************************************/

	int getAlimitMaxRangeSPO2_PImin();

	/**********************************************************************************************//**
	 * Gets alimit minimum range spo2 pimin.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range spo2 pimin.
	 **************************************************************************************************/

	int getAlimitMinRangeSPO2_PImin();

	/**********************************************************************************************//**
	 * Gets alimit maximum range spo2 SIQ min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit maximum range spo2 SIQ min.
	 **************************************************************************************************/

	int getAlimitMaxRangeSPO2_SIQmin();

	/**********************************************************************************************//**
	 * Gets alimit minimum range spo2 SIQ min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The alimit minimum range spo2 SIQ min.
	 **************************************************************************************************/

	int getAlimitMinRangeSPO2_SIQmin();

	/**********************************************************************************************//**
	 * Sets alimit state calculated.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void SetAlimitStateCalculated();

	/**********************************************************************************************//**
	 * Check limits.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void checkLimits();

	/**********************************************************************************************//**
	 * Check ventilation limits.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void checkVentilationLimits();

	/**********************************************************************************************//**
	 * Check co2 limits.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void checkCO2Limits();

	/**********************************************************************************************//**
	 * Check spo2 limits.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void checkSpO2Limits();

	/**********************************************************************************************//**
	 * Check alarm state bytes.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void CheckAlarmStateBytes();

	/**********************************************************************************************//**
	 * Check flow sensor state.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void CheckFlowSensorState();

	/**********************************************************************************************//**
	 * Query if 'prioAlarm' is active alarm higher or same priority.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	prioAlarm	The prio alarm.
	 *
	 * \return	True if active alarm higher or same priority, false if not.
	 **************************************************************************************************/

	bool isActiveAlarmHigherOrSamePriority(eAlarmPrio prioAlarm);

	/**********************************************************************************************//**
	 * \fn	bool CAlarmMgmtHandler::isActiveAlarmHigherPriority(eAlarmPrio prioAlarm);
	 *
	 * \brief	Query if 'prioAlarm' is active alarm higher priority.
	 *
	 * \author	Rainer
	 * \date	25.08.2017
	 *
	 * \param	prioAlarm	The prio alarm.
	 *
	 * \return	True if active alarm higher priority, false if not.
	 **************************************************************************************************/

	bool isActiveAlarmHigherPriority(eAlarmPrio prioAlarm);

	/**********************************************************************************************//**
	 * Determine if we can set alarm checksum con picture.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm checksum con picture, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_ChecksumConPIC();

	/**********************************************************************************************//**
	 * Determine if we can set alarm disconnection.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm disconnection, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_DISCONNECTION();

	/**********************************************************************************************//**
	 * Determine if we can set alarm tube occlusion.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm tube occlusion, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_TUBE_OCCLUSION();

	/**********************************************************************************************//**
	 * Determine if we can set alarm checksum monitor picture.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm checksum monitor picture, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_ChecksumMonPIC();

	/**********************************************************************************************//**
	 * Determine if we can set alarm relais defect.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm relais defect, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_RELAIS_DEFECT();

	/**********************************************************************************************//**
	 * Determine if we can set alarm mixer avcal.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm mixer avcal, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_MIXER_AVCAL();

	/**********************************************************************************************//**
	 * Determine if we can set alarm pin mixer.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm pin mixer, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_P_IN_MIXER();

	/**********************************************************************************************//**
	 * Determine if we can set alarm voltage.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm voltage, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_VOLTAGE();

	/**********************************************************************************************//**
	 * Determine if we can set alarm interface spi.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm interface spi, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_IF_SPI();

	/**********************************************************************************************//**
	 * Determine if we can set alarm interface dio.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm interface dio, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_IF_DIO();

	/**********************************************************************************************//**
	 * Determine if we can set alarm interface com.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm interface com, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_IF_COM();

	/**********************************************************************************************//**
	 * Determine if we can set alarm interface i2c.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm interface i2c, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_IF_I2C();

	/**********************************************************************************************//**
	 * Determine if we can set alarm interface pif.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm interface pif, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_IF_PIF();

	/**********************************************************************************************//**
	 * Determine if we can set alarm fan.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm fan, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_Fan();

	/**********************************************************************************************//**
	 * Determine if we can set alarm outofmemory.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm outofmemory, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_OUTOFMEMORY();

	/**********************************************************************************************//**
	 * Determine if we can set alarm pin o2.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm pin o2, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_P_IN_O2();

	/**********************************************************************************************//**
	 * Determine if we can set alarm pin air.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm pin air, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_P_IN_AIR();

	/**********************************************************************************************//**
	 * Determine if we can set alarm tubusblocked.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm tubusblocked, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_TUBUSBLOCKED();

	/**********************************************************************************************//**
	 * Determine if we can set alarm flow sensor defect.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm flow sensor defect, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_FLOW_SENSOR_DEFECT();

	/**********************************************************************************************//**
	 * Determine if we can set alarm flow sensor cleaning.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm flow sensor cleaning, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_FLOW_SENSOR_CLEANING();

	/**********************************************************************************************//**
	 * Determine if we can set alarm flow sensor notconnected.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm flow sensor notconnected, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_FLOW_SENSOR_NOTCONNECTED();

	/**********************************************************************************************//**
	 * Determine if we can set alarm o2 sensor defect.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm o2 sensor defect, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_O2_SENSOR_DEFECT();

	/**********************************************************************************************//**
	 * Determine if we can set alarm o2 sensor used.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm o2 sensor used, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_O2_SENSOR_USED();

	/**********************************************************************************************//**
	 * Determine if we can set alarm o2 value incorrect.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm o2 value incorrect, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_O2_VALUE_INCORRECT();

	/**********************************************************************************************//**
	 * Determine if we can set alarm o2 not calibrated.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm o2 not calibrated, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_O2_NOT_CALIBRATED();

	/**********************************************************************************************//**
	 * Determine if we can set alarm prico fio2 out of range.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm prico fio2 out of range, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_PRICO_FiO2outOfRange();

	/**********************************************************************************************//**
	 * Determine if we can set alarm co2 module notconnected.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm co2 module notconnected, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_CO2_MODULE_NOTCONNECTED();

	/**********************************************************************************************//**
	 * Determine if we can set alarm co2 filterline notconnected.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm co2 filterline notconnected, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_CO2_FILTERLINE_NOTCONNECTED();

	/**********************************************************************************************//**
	 * Determine if we can set alarm co2 checksamplingline.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm co2 checksamplingline, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_CO2_CHECKSAMPLINGLINE();

	/**********************************************************************************************//**
	 * Determine if we can set alarm co2 checkairwayadapter.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm co2 checkairwayadapter, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_CO2_CHECKAIRWAYADAPTER();

	/**********************************************************************************************//**
	 * Determine if we can set alarm co2 sensorfaulty.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm co2 sensorfaulty, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_CO2_SENSORFAULTY();

	/**********************************************************************************************//**
	 * Determine if we can set alarm spo2 sensorfaulty.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm spo2 sensorfaulty, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_SPO2_SENSORFAULTY();

	/**********************************************************************************************//**
	 * Determine if we can set alarm spo2 checksensor.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm spo2 checksensor, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_SPO2_CHECKSENSOR();

	/**********************************************************************************************//**
	 * Determine if we can set alarm spo2 module notconnected.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm spo2 module notconnected, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_SPO2_MODULE_NOTCONNECTED();

	/**********************************************************************************************//**
	 * Determine if we can set alarm prico SIQ value.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm prico SIQ value, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_PRICO_SIQvalue();

	/**********************************************************************************************//**
	 * Determine if we can set alarm prico fio2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm prico fio2max, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_PRICO_FiO2max();

	/**********************************************************************************************//**
	 * Determine if we can set alarm prico fio2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm prico fio2min, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_PRICO_FiO2min();

	/**********************************************************************************************//**
	 * Determine if we can set alarm mvmax.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm vmax, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_MVmax();

	/**********************************************************************************************//**
	 * Determine if we can set alarm mvmin.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm vmin, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_MVmin();

	/**********************************************************************************************//**
	 * Determine if we can set alarm pipmax.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm pipmax, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_PIPmax();

	/**********************************************************************************************//**
	 * Determine if we can set alarm pipmin.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm pipmin, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_PIPmin();

	/**********************************************************************************************//**
	 * Determine if we can set alarm peepmin low.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm peepmin low, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_PEEPminLow();

	/**********************************************************************************************//**
	 * Determine if we can set alarm peepmin high.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm peepmin high, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_PEEPminHigh();

	/**********************************************************************************************//**
	 * Determine if we can set alarm bpmmax.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm bpmmax, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_BPMmax();

	/**********************************************************************************************//**
	 * Determine if we can set alarm leakmax.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm leakmax, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_Leakmax();

	/**********************************************************************************************//**
	 * Determine if we can set alarm apnoe.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm apnoe, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_Apnoe();

	/**********************************************************************************************//**
	 * Determine if we can set alarm mapmax.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm mapmax, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_MAPmax();

	/**********************************************************************************************//**
	 * Determine if we can set alarm mapmin.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm mapmin, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_MAPmin();

	/**********************************************************************************************//**
	 * Determine if we can set alarm dco2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm dco2max, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_DCO2max();

	/**********************************************************************************************//**
	 * Determine if we can set alarm dco2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm dco2min, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_DCO2min();

	/**********************************************************************************************//**
	 * Determine if we can set alarm etco2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm etco2max, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_ETCO2max();

	/**********************************************************************************************//**
	 * Determine if we can set alarm etco2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm etco2min, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_ETCO2min();

	/**********************************************************************************************//**
	 * Determine if we can set alarm fico2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm fico2max, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_FICO2max();

	/**********************************************************************************************//**
	 * Determine if we can set alarm fico2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm fico2min, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_FICO2min();

	/**********************************************************************************************//**
	 * Determine if we can set alarm spo2max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm spo2max, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_SPO2max();

	/**********************************************************************************************//**
	 * Determine if we can set alarm spo2min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm spo2min, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_SPO2min();

	/**********************************************************************************************//**
	 * Determine if we can set alarm pulse ratemax.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm pulse ratemax, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_PulseRatemax();

	/**********************************************************************************************//**
	 * Determine if we can set alarm pulse ratemin.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm pulse ratemin, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_PulseRatemin();

	/**********************************************************************************************//**
	 * Determine if we can set alarm spo2 pimin.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm spo 2 p imin, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_SPO2_PImin();

	/**********************************************************************************************//**
	 * Determine if we can set alarm pinsp not reached.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm pinsp not reached, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_Pinsp_NotReached();

	/**********************************************************************************************//**
	 * Determine if we can set alarm vgaranty.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm vgaranty, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_Vgaranty();

	/**********************************************************************************************//**
	 * Determine if we can set alarm vlimitted.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm vlimitted, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_Vlimitted();

	/**********************************************************************************************//**
	 * Determine if we can set alarm accu power.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm accu power, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_Accu_POWER();

	/**********************************************************************************************//**
	 * Determine if we can set alarm accu 60.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm accu 60, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_Accu_60();

	/**********************************************************************************************//**
	 * Determine if we can set alarm accu 30.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm accu 30, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_Accu_30();

	/**********************************************************************************************//**
	 * Determine if we can set alarm accu 15.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm accu 15, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_Accu_15();

	/**********************************************************************************************//**
	 * Determine if we can set alarm accu empty.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm accu empty, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_AccuEmpty();

	/**********************************************************************************************//**
	 * Determine if we can set alarm accu defect.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if we can set alarm accu defect, false if not.
	 **************************************************************************************************/

	bool CanSetAlarm_AccuDefect();

	/**********************************************************************************************//**
	 * Gets alarm log.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	Null if it fails, else the alarm log.
	 **************************************************************************************************/

	CTlsLoglist* getAlarmLog();

	/**********************************************************************************************//**
	 * Sets log text.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	sz	The size.
	 **************************************************************************************************/

	void setLogTxt(CStringW sz);

	/**********************************************************************************************//**
	 * Sets spo2 alarm delay.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	delay	The delay.
	 **************************************************************************************************/

	void setSPO2alarmDelay(eSPO2alarmdelay delay);

	/**********************************************************************************************//**
	 * Query if 'oldTickCount' is safe tick count delay expired.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	oldTickCount	Number of old ticks.
	 * \param	delay			(Optional) The delay.
	 *
	 * \return	True if safe tick count delay expired, false if not.
	 **************************************************************************************************/

	bool isSafeTickCountDelayExpired(DWORD oldTickCount, UINT delay=0);//used for (dwLastTickCount+DELAY<getTickCount64())

protected:

	/**********************************************************************************************//**
	 * Default constructor.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	CAlarmMgmtHandler();

	/**********************************************************************************************//**
	 * Destructor.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	virtual ~CAlarmMgmtHandler();

	/**********************************************************************************************//**
	 * Gets the model.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	Null if it fails, else the model.
	 **************************************************************************************************/

	CMVModel *getModel();

	/**********************************************************************************************//**
	 * Check automatic enable prico.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 **************************************************************************************************/

	void checkAutoEnablePRICO();

	/**********************************************************************************************//**
	 * Sets prico automatic turned off.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	bAutoTurnOn	True to enable, false to disable the automatic turn.
	 * \param	prioAlarm  	The prio alarm.
	 **************************************************************************************************/

	void setPRICOAutoTurnedOff(bool bAutoTurnOn, eAlarmPrio prioAlarm);

	/**********************************************************************************************//**
	 * Query if this object is prico automatic turned on.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if prico automatic turne on, false if not.
	 **************************************************************************************************/

	bool isPRICOAutoTurnedOn();

	/**********************************************************************************************//**
	 * Gets prico automatic alarm priority.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	The prico automatic alarm priority.
	 **************************************************************************************************/

	eAlarmPrio getPRICOAutoAlarmPriority();

	/**********************************************************************************************//**
	 * Sets log alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	enAlarm   	The en alarm.
	 * \param	szAlarmTxt	The alarm text.
	 **************************************************************************************************/

	void setLogAlarm(eAlarm	enAlarm, CStringW szAlarmTxt);

	/**********************************************************************************************//**
	 * Gets log text alarm.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	enAlarm	The en alarm.
	 *
	 * \return	The log text alarm.
	 **************************************************************************************************/

	CStringW getLogTxtAlarm(eAlarm enAlarm);

	/**********************************************************************************************//**
	 * Sets alarm accu empty.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_AccuEmpty(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm accu defect.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_AccuDefect(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm checksum controller PIC.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_ChecksumConPIC(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm checksum monitor PIC.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_ChecksumMonPIC(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm relais defect
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_RELAIS_DEFECT(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm mixer avcal.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_MIXER_AVCAL(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm pin mixer.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_P_IN_MIXER(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm voltage.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_VOLTAGE(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm interface spi.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_IF_SPI(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm interface dio.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_IF_DIO(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm interface com.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_IF_COM(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm interface i2c.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_IF_I2C(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm interface pif.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_IF_PIF(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm out of memory.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_OUTOFMEMORY(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm fan.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_Fan(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm pin O2.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_P_IN_O2(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm pin air.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_P_IN_AIR(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm tube occlusion.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_TUBE_OCCLUSION(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm disconnection.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_DISCONNECTION(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm tubus blocked.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_TUBUSBLOCKED(CStringW alarmTxt);
	//Nebulizer Alarms
	/*bool SetAlarm_NEBULIZER_DISCONNECTION(CStringW alarmTxt);
	bool SetAlarm_NEBULIZER_SYSERROR(CStringW alarmTxt);*/

	/**********************************************************************************************//**
	 * Sets alarm flow sensor defect.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_FLOW_SENSOR_DEFECT(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm flow sensor cleaning.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_FLOW_SENSOR_CLEANING(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm flow sensor notconnected.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_FLOW_SENSOR_NOTCONNECTED(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm o2 sensor defect.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_O2_SENSOR_DEFECT(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm o2 sensor used.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_O2_SENSOR_USED(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm o2 value incorrect.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_O2_VALUE_INCORRECT(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm o2 not calibrated.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_O2_NOT_CALIBRATED(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm prico fio2 out of range.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_PRICO_FiO2outOfRange(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm co2 module notconnected.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_CO2_MODULE_NOTCONNECTED(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm co2 filterline notconnected.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_CO2_FILTERLINE_NOTCONNECTED(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm co2 checksamplingline.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_CO2_CHECKSAMPLINGLINE(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm co2 checkairwayadapter.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_CO2_CHECKAIRWAYADAPTER(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm co2 sensorfaulty.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_CO2_SENSORFAULTY(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm spo2 sensorfaulty.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_SPO2_SENSORFAULTY(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm spo2 checksensor.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_SPO2_CHECKSENSOR(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm spo2 module notconnected.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_SPO2_MODULE_NOTCONNECTED(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm prico siq.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_PRICO_SIQvalue(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm prico fio2 max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_PRICO_FiO2max(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm prico fio2 min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_PRICO_FiO2min(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm mv max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_MVmax(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm mv min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_MVmin(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm pip max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_PIPmax();

	/**********************************************************************************************//**
	 * Sets alarm pip min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_PIPmin();

	/**********************************************************************************************//**
	 * Sets alarm peep min low.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_PEEPminLow();

	/**********************************************************************************************//**
	 * Sets alarm peep min high.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_PEEPminHigh();

	/**********************************************************************************************//**
	 * Sets alarm bpm max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_BPMmax(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm leak max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_Leakmax(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm apnoe.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_Apnoe(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm map max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_MAPmax(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm map min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_MAPmin(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm dco2 max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_DCO2max(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm dco2 min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_DCO2min(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm etco2 max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_ETCO2max(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm etco2 min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_ETCO2min(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm fico2 max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_FICO2max(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm fico2 min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_FICO2min(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm spo2 max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_SPO2max(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm spo2 min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_SPO2min(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm pulse rate max.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_PulseRatemax(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm pulse rate min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_PulseRatemin(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm spo2 pi min.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_SPO2_PImin(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarms Pinsp not reached.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_Pinsp_NotReached(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm vlimitted.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_Vlimitted(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm vgaranty.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_Vgaranty(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * net power alarms.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_Accu_POWER(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm accu 60 minutes.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_Accu_60(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm accu 30 minutes.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool SetAlarm_Accu_30(CStringW alarmTxt);

	/**********************************************************************************************//**
	 * Sets alarm accu 15 minutes.
	 *
	 * \author	Rainer
	 * \date	11.07.2017
	 *
	 * \param	alarmTxt	The alarm text.
	 *
	 * \return	True if it succeeds, false if it fails.
	 **************************************************************************************************/

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


