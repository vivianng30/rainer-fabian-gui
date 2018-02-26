/**********************************************************************************************//**
 * \file	VentModeHandler.h.
 *
 * Declares the vent mode handler class
 **************************************************************************************************/

#pragma once
#include "globDefs.h"

class CMVModel;

class CVentModeHandler
{
public:


	static CVentModeHandler* getInstance();	
	static void destroyInstance();	

	void setIPPV();
	void setSIPPV();
	void setSIMV();
	void setSIMVPSV();
	void setPSV();
	void setHFO();
	void setCPAP();
	void setNCPAP();
	void setDUOPAP();
	void setTHERAPIE();

	bool changeVentMode(eVentMode state=VM_IPPV, int iSetAlarmSilent=1, bool bStopFOT=true);
	void changeToPrevMode();

	eVentMode getCurrentActiveMode();

	bool modeIsPSVmode();
	bool activeModeIsVM_PSV();
	bool activeModeHasPSV();

	bool curModeIsConventionalMode();
	bool activeModeIsCONVTrigger();
	bool activeModeIsNMODETrigger();
	bool activeModeIsTrigger();
	bool activeModeIsHFO();
	bool activeModeIsNMODE();
	bool activeModeIsCPAP();
	bool activeModeIsNCPAP();
	bool activeModeIsDUOPAP();
	bool activeModeIsTHERAPY();
	bool activeModeIsIPPV();
	bool activeModeIsSIPPV();
	bool activeModeIsSIMV();
	bool viewModeIsAnyNMODE();
	bool curModeIsVolumeSupportMode();
	bool curModeIsCONVTriggerMode();
	bool curModeIsCONVTriggerPresetMode();

private:
	//singleton
	static CVentModeHandler* theVentModeHandler;//!< singleton of vent mode handler
protected:
	CVentModeHandler(void);
	~CVentModeHandler(void);

	CMVModel *getModel();

private:

	CMVModel *m_pModel;//!< the model
};
