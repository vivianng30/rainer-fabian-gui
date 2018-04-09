/**********************************************************************************************//**
 * \file	VentModeHandler.cpp.
 *
 * Implements the vent mode handler class
 **************************************************************************************************/

#include "StdAfx.h"
#include "VentModeHandler.h"
#include "MVModel.h"
#include "MVViewHandler.h"

CVentModeHandler* CVentModeHandler::theVentModeHandler=0;

/**********************************************************************************************//**
 * Initializes a new instance of the CVentModeHandler class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CVentModeHandler::CVentModeHandler(void)
{
	m_pModel = NULL;
}

/**********************************************************************************************//**
 * Finalizes an instance of the CVentModeHandler class
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

CVentModeHandler::~CVentModeHandler(void)
{
}

/**********************************************************************************************//**
 * Gets the instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	Null if it fails, else the instance.
 **************************************************************************************************/

CVentModeHandler* CVentModeHandler::getInstance()
{
	if(theVentModeHandler == 0)
	{
		theVentModeHandler = new CVentModeHandler;
	}
	return theVentModeHandler;
}

/**********************************************************************************************//**
 * Destroys the instance
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CVentModeHandler::destroyInstance()
{
	if(theVentModeHandler != NULL)
	{
		delete theVentModeHandler;
		theVentModeHandler = NULL;
	}
}

/**********************************************************************************************//**
 * Sets the ippv
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CVentModeHandler::setIPPV()
{
	if(getModel()->getCONFIG()->GetCurMode()!=VM_IPPV)
	{
		getModel()->ResetExhalValvCalMode();

		if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_IPPV)
		{
			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}

			getModel()->getDATAHANDLER()->PARADATA()->SetDataFromMode(VM_PRE_IPPV);
			changeVentMode(VM_IPPV);

			getModel()->getALARMHANDLER()->resetVgVlAutoTurnedOff();

			getModel()->getALARMHANDLER()->deleteAllAlarms(false);

			if(getModel()->IsO2calFlag() && getModel()->IsFlowsensorFlag())
				getModel()->getVIEWHANDLER()->changeViewState(VS_SETUP,VSS_NONE);
		}
		else if(getModel()->getCONFIG()->CurModeIsPresetMode()==true && getModel()->getCONFIG()->GetPrevMode()==VM_IPPV)
		{
			changeVentMode(VM_IPPV,0);

			if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==true && getModel()->getDATAHANDLER()->GetVgarantState()!=VOLUMEGAR_ON)
			{
				getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_ON);
			}
			else if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==false && getModel()->getDATAHANDLER()->GetVgarantState()!=VOLUMEGAR_OFF)
			{
				getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_OFF);
			}

			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		else 
		{
			getModel()->getDATAHANDLER()->PRESET()->SetDataFromMode(VM_IPPV);

			changeVentMode(VM_PRE_IPPV,0);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VENTMODE_HOME);
		}
	}
	else
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->SetFocus();
	}
}

/**********************************************************************************************//**
 * Sets the sippv
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CVentModeHandler::setSIPPV()
{
	if(getModel()->getCONFIG()->GetCurMode()!=VM_SIPPV)
	{
		getModel()->ResetExhalValvCalMode();

		if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIPPV)
		{
			getModel()->getDATAHANDLER()->PARADATA()->SetDataFromMode(VM_PRE_SIPPV);

			changeVentMode(VM_SIPPV);
			
			getModel()->getALARMHANDLER()->resetVgVlAutoTurnedOff();


			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}

			getModel()->getALARMHANDLER()->deleteAllAlarms(false);

			if(getModel()->IsO2calFlag() && getModel()->IsFlowsensorFlag())
				getModel()->getVIEWHANDLER()->changeViewState(VS_SETUP,VSS_NONE);
		}
		else if(getModel()->getCONFIG()->CurModeIsPresetMode()==true && getModel()->getCONFIG()->GetPrevMode()==VM_SIPPV)
		{
			changeVentMode(VM_SIPPV,0);

			if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==true && getModel()->getDATAHANDLER()->GetVgarantState()!=VOLUMEGAR_ON)
			{
				getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_ON);
			}
			else if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==false && getModel()->getDATAHANDLER()->GetVgarantState()!=VOLUMEGAR_OFF)
			{
				getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_OFF);
			}
			
			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		else
		{
			getModel()->getDATAHANDLER()->PRESET()->SetDataFromMode(VM_SIPPV);

			changeVentMode(VM_PRE_SIPPV,0);
			
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VENTMODE_HOME);
		}
	}
	else
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->SetFocus();
	}
}

/**********************************************************************************************//**
 * Sets the simv
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CVentModeHandler::setSIMV()
{
	if(getModel()->getCONFIG()->GetCurMode()!=VM_SIMV)
	{
		getModel()->ResetExhalValvCalMode();

		if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMV)
		{
			//ok
			getModel()->getDATAHANDLER()->PARADATA()->SetDataFromMode(VM_PRE_SIMV);

			changeVentMode(VM_SIMV);
			
			getModel()->getALARMHANDLER()->resetVgVlAutoTurnedOff();


			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}

			getModel()->getALARMHANDLER()->deleteAllAlarms(false);

			if(getModel()->IsO2calFlag() && getModel()->IsFlowsensorFlag())
				getModel()->getVIEWHANDLER()->changeViewState(VS_SETUP,VSS_NONE);
		}
		else if(getModel()->getCONFIG()->CurModeIsPresetMode()==true && getModel()->getCONFIG()->GetPrevMode()==VM_SIMV)
		{
			changeVentMode(VM_SIMV,0);

			if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==true && getModel()->getDATAHANDLER()->GetVgarantState()!=VOLUMEGAR_ON)
			{
				getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_ON);
			}
			else if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==false && getModel()->getDATAHANDLER()->GetVgarantState()!=VOLUMEGAR_OFF)
			{
				getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_OFF);
			}
			
			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		else
		{
			getModel()->getDATAHANDLER()->PRESET()->SetDataFromMode(VM_SIMV);

			changeVentMode(VM_PRE_SIMV,0);
			
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VENTMODE_HOME);
		}
	}
	else
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->SetFocus();
	}
}

/**********************************************************************************************//**
 * Sets the simvpsv
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CVentModeHandler::setSIMVPSV()
{
	if(getModel()->getCONFIG()->GetCurMode()!=VM_SIMVPSV)
	{
		getModel()->ResetExhalValvCalMode();

		if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_SIMVPSV)
		{
			  //ok
			getModel()->getDATAHANDLER()->PARADATA()->SetDataFromMode(VM_PRE_SIMVPSV);

			changeVentMode(VM_SIMVPSV);
			
			getModel()->getALARMHANDLER()->resetVgVlAutoTurnedOff();

			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}

			getModel()->getALARMHANDLER()->deleteAllAlarms(false);

			if(getModel()->IsO2calFlag() && getModel()->IsFlowsensorFlag())
				getModel()->getVIEWHANDLER()->changeViewState(VS_SETUP,VSS_NONE);
		}
		else if(getModel()->getCONFIG()->CurModeIsPresetMode()==true && getModel()->getCONFIG()->GetPrevMode()==VM_SIMVPSV)
		{
			changeVentMode(VM_SIMVPSV,0);

			if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==true && getModel()->getDATAHANDLER()->GetVgarantState()!=VOLUMEGAR_ON)
			{
				getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_ON);
			}
			else if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==false && getModel()->getDATAHANDLER()->GetVgarantState()!=VOLUMEGAR_OFF)
			{
				getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_OFF);
			}
		
			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		else
		{
			getModel()->getDATAHANDLER()->PRESET()->SetDataFromMode(VM_SIMVPSV);

			changeVentMode(VM_PRE_SIMVPSV,0);

			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VENTMODE_HOME);
		}
	}
	else
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->SetFocus();
	}
}

/**********************************************************************************************//**
 * Sets the psv
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CVentModeHandler::setPSV()
{
	if(getModel()->getCONFIG()->GetCurMode()!=VM_PSV)
	{
		getModel()->ResetExhalValvCalMode();

		if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_PSV)
		{
			getModel()->getDATAHANDLER()->PARADATA()->SetDataFromMode(VM_PRE_PSV);

			changeVentMode(VM_PSV);
			
			getModel()->getALARMHANDLER()->resetVgVlAutoTurnedOff();


			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}

			getModel()->getALARMHANDLER()->deleteAllAlarms(false);

			if(getModel()->IsO2calFlag() && getModel()->IsFlowsensorFlag())
				getModel()->getVIEWHANDLER()->changeViewState(VS_SETUP,VSS_NONE);
		}
		else if(getModel()->getCONFIG()->CurModeIsPresetMode()==true && getModel()->getCONFIG()->GetPrevMode()==VM_PSV)
		{
			changeVentMode(VM_PSV,0);

			if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==true && getModel()->getDATAHANDLER()->GetVgarantState()!=VOLUMEGAR_ON)
			{
				getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_ON);
			}
			else if(getModel()->getDATAHANDLER()->IsCurrentModeVGarantStateOn()==false && getModel()->getDATAHANDLER()->GetVgarantState()!=VOLUMEGAR_OFF)
			{
				getModel()->getDATAHANDLER()->ChangeVgarantState(VOLUMEGAR_OFF);
			}
			
			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		else
		{
			getModel()->getDATAHANDLER()->PRESET()->SetDataFromMode(VM_PSV);

			changeVentMode(VM_PRE_PSV,0);
			
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VENTMODE_HOME);

		}
	}
	else
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->SetFocus();
	}
}

/**********************************************************************************************//**
 * Sets the hfo
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CVentModeHandler::setHFO()
{
	if(getModel()->getCONFIG()->GetCurMode()!=VM_HFO)
	{
		getModel()->ResetExhalValvCalMode();

		if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_HFO)
		{
			getModel()->getDATAHANDLER()->PARADATA()->SetDataFromMode(VM_PRE_HFO);

			changeVentMode(VM_HFO);
			
			getModel()->getALARMHANDLER()->resetVgVlAutoTurnedOff();


			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}

			getModel()->getALARMHANDLER()->deleteAllAlarms(false);

			if(getModel()->IsO2calFlag() && getModel()->IsFlowsensorFlag())
				getModel()->getVIEWHANDLER()->changeViewState(VS_SETUP,VSS_NONE);
		}
		else if(getModel()->getCONFIG()->CurModeIsPresetMode()==true && getModel()->getCONFIG()->GetPrevMode()==VM_HFO)
		{
			changeVentMode(VM_HFO,0);
		
			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		else
		{
			getModel()->getDATAHANDLER()->PRESET()->SetDataFromMode(VM_HFO);

			changeVentMode(VM_PRE_HFO,0);
			
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VENTMODE_HOME);

		}
	}
	else
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->SetFocus();
	}
}

/**********************************************************************************************//**
 * Sets the cpap
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CVentModeHandler::setCPAP()
{
	if(getModel()->getCONFIG()->GetCurMode()!=VM_CPAP)
	{
		getModel()->ResetExhalValvCalMode();

		if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_CPAP)
		{
			getModel()->getDATAHANDLER()->PARADATA()->SetDataFromMode(VM_PRE_CPAP);

			changeVentMode(VM_CPAP);
			
			getModel()->getALARMHANDLER()->resetVgVlAutoTurnedOff();


			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF 
				&& getModel()->getDATAHANDLER()->GetPrevFlowSensorState()==FLOWSENSOR_ON)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}

			getModel()->getDATAHANDLER()->checkTriggerTubeDependency();

			getModel()->getALARMHANDLER()->deleteAllAlarms(false);

			if(getModel()->IsO2calFlag() && getModel()->IsFlowsensorFlag())
				getModel()->getVIEWHANDLER()->changeViewState(VS_SETUP,VSS_NONE);
		}
		else if(getModel()->getCONFIG()->CurModeIsPresetMode()==true && getModel()->getCONFIG()->GetPrevMode()==VM_CPAP)
		{
			changeVentMode(VM_CPAP,0);
			
			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF 
				&& getModel()->getDATAHANDLER()->GetPrevFlowSensorState()==FLOWSENSOR_ON)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}

			getModel()->getDATAHANDLER()->checkTriggerTubeDependency();
		}
		else
		{
			getModel()->getDATAHANDLER()->PRESET()->SetDataFromMode(VM_CPAP);

			changeVentMode(VM_PRE_CPAP,0);
			
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VENTMODE_HOME);

		}
	}
	else
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->SetFocus();
	}
}

/**********************************************************************************************//**
 * Sets the ncpap
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CVentModeHandler::setNCPAP()
{
	if(getModel()->getCONFIG()->GetCurMode()!=VM_NCPAP)
	{
		getModel()->ResetExhalValvCalMode();

		if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_NCPAP)
		{
			getModel()->getDATAHANDLER()->PARADATA()->SetDataFromMode(VM_PRE_NCPAP);

			changeVentMode(VM_NCPAP);
			
			if(false==getModel()->getDATAHANDLER()->isNIVTRIGGERAvailable())
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
			}
			else if(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
			}
			else if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF 
				&& getModel()->getDATAHANDLER()->GetPrevFlowSensorState()==FLOWSENSOR_ON 
				&& getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}

			getModel()->getDATAHANDLER()->checkTriggerTubeDependency();

			getModel()->getALARMHANDLER()->deleteAllAlarms(false);
		}
		else if(getModel()->getCONFIG()->CurModeIsPresetMode()==true && getModel()->getCONFIG()->GetPrevMode()==VM_NCPAP)
		{
			changeVentMode(VM_NCPAP,0);

			if(false==getModel()->getDATAHANDLER()->isNIVTRIGGERAvailable())
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
			}
			else if(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
			}
			else if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF 
				&& getModel()->getDATAHANDLER()->GetPrevFlowSensorState()==FLOWSENSOR_ON 
				&& getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
			
			getModel()->getDATAHANDLER()->checkTriggerTubeDependency();
		}
		else
		{
			getModel()->getDATAHANDLER()->PRESET()->SetDataFromMode(VM_NCPAP);

			changeVentMode(VM_PRE_NCPAP,0);
			
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VENTMODE_HOME);

		}
	}
	else
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->SetFocus();
	}
}

/**********************************************************************************************//**
 * Sets the duopap
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CVentModeHandler::setDUOPAP()
{
	if(getModel()->getCONFIG()->GetCurMode()!=VM_DUOPAP)
	{
		getModel()->ResetExhalValvCalMode();

		if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_DUOPAP)
		{
			getModel()->getDATAHANDLER()->PARADATA()->SetDataFromMode(VM_PRE_DUOPAP);

			changeVentMode(VM_DUOPAP);

			if(false==getModel()->getDATAHANDLER()->isNIVTRIGGERAvailable())
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
			}
			else if(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
			}
			else if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF 
				&& getModel()->getDATAHANDLER()->GetPrevFlowSensorState()==FLOWSENSOR_ON 
				&& getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}

			getModel()->getDATAHANDLER()->checkTriggerTubeDependency();

			getModel()->getALARMHANDLER()->deleteAllAlarms(false);

		}
		else if(getModel()->getCONFIG()->CurModeIsPresetMode()==true && getModel()->getCONFIG()->GetPrevMode()==VM_DUOPAP)
		{
			changeVentMode(VM_DUOPAP,0);

			if(false==getModel()->getDATAHANDLER()->isNIVTRIGGERAvailable())
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
			}
			else if(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
			}
			else if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF 
				&& getModel()->getDATAHANDLER()->GetPrevFlowSensorState()==FLOWSENSOR_ON 
				&& getModel()->getDATAHANDLER()->GetTubeSet()!=TUBE_MEDIJET)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}

			getModel()->getDATAHANDLER()->checkTriggerTubeDependency();
		}
		else
		{
			getModel()->getDATAHANDLER()->PRESET()->SetDataFromMode(VM_DUOPAP);

			changeVentMode(VM_PRE_DUOPAP,0);
			
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VENTMODE_HOME);

			getModel()->getDATAHANDLER()->checkPRESETTriggerTubeDependency();
		}
	}
	else
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->SetFocus();
	}
}

/**********************************************************************************************//**
 * Sets the therapie
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CVentModeHandler::setTHERAPIE()
{
	if(getModel()->getCONFIG()->GetCurMode()!=VM_THERAPIE)
	{
		getModel()->ResetExhalValvCalMode();

		if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_THERAPIE)
		{
			getModel()->getDATAHANDLER()->PARADATA()->SetDataFromMode(VM_PRE_THERAPIE);

			changeVentMode(VM_THERAPIE);
			
			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
			}

			getModel()->getALARMHANDLER()->deleteAllAlarms(false);
		}
		else if(getModel()->getCONFIG()->CurModeIsPresetMode()==true && getModel()->getCONFIG()->GetPrevMode()==VM_THERAPIE)
		{
			changeVentMode(VM_THERAPIE,0);
			
			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
			}

		}
		else
		{
			getModel()->getDATAHANDLER()->PRESET()->SetDataFromMode(VM_THERAPIE);

			changeVentMode(VM_PRE_THERAPIE,0);
			
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_VENTMODE_HOME);
		}
	}
	else
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->SetFocus();
	}
}

/**********************************************************************************************//**
 * Gets the model
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CVentModeHandler::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * Change vent mode
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \param	state		   	The state.
 * \param	iSetAlarmSilent	Zero-based index of the set alarm silent.
 * \param	bStopFOT	   	True to stop fot.
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::changeVentMode(eVentMode state, int iSetAlarmSilent, bool bStopFOT)
{
	if(bStopFOT && getModel()->getDATAHANDLER()->isFOTLicenseAvailable() && getModel()->getFOTThread())
	{
		if(getModel()->getFOTThread()->getFOTstate()!=FOT_OFF)
		{
			if(AfxGetApp() != NULL)
				AfxGetApp()->GetMainWnd()->PostMessage(WM_STOP_FOT);
		}
	}
	if(iSetAlarmSilent==1)
	{
		getModel()->getALARMHANDLER()->setAutoSilent();
		getModel()->getALARMHANDLER()->deleteSignaledAlarmlimitStates();
		Sleep(0);
	}

	bool bOldVentModeWasNMODE=activeModeIsNMODE();
	eVentMode stateOld=getModel()->getCONFIG()->GetCurMode();
	getModel()->getCONFIG()->SetCurMode(state);

	getModel()->getALARMHANDLER()->ventModeChanged();

	if(		state != VM_PRE_IPPV
		&&	state != VM_PRE_PSV
		&&	state != VM_PRE_SIMV
		&&	state != VM_PRE_SIMVPSV
		&&	state != VM_PRE_SIPPV
		&&	state != VM_PRE_CPAP
		&&	state != VM_PRE_NCPAP
		&&	state != VM_PRE_DUOPAP
		&&	state != VM_PRE_HFO
		&&	state != VM_PRE_THERAPIE)
	{
		if(getModel()->getDATAHANDLER()->isPSVapnoe()==true)
		{
			if(AfxGetApp())
				AfxGetApp()->GetMainWnd()->PostMessage(WM_PSV_APNOE_END);
		}

		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->PostMessage(WM_DEL_BACKUP);

		getModel()->getSPI()->resetPressureAlarmDelay();

		if(stateOld==VM_SERVICE)
		{
			getModel()->Send_VENT_MODE(state);
		}
		else if(getModel()->getCONFIG()->GetPrevMode()!=state)
		{
			getModel()->Send_VENT_MODE(state);
		}
	}

	//getModel()->getALARMHANDLER()->ventModeChanged();

	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->PostMessage(WM_VENTMODE_CHANGED,iSetAlarmSilent);

	getModel()->notifyVentModeChanged();

	if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
	{
		if(getModel()->getCONFIG()->GetCurMode()==VM_THERAPIE)
		{
			
		}
		else if(getModel()->getCONFIG()->GetCurMode()==VM_CPAP)
		{
			getModel()->getALARMHANDLER()->setAlimitsMinMaxRangeCpap();
		}
		else if(getModel()->getCONFIG()->GetCurMode()==VM_NCPAP)
		{
			getModel()->getALARMHANDLER()->setAlimitsMinMaxRangeNCPAP();
		}
		else if(getModel()->getCONFIG()->GetCurMode()==VM_DUOPAP)
		{
			getModel()->getALARMHANDLER()->setAlimitsMinMaxRangeDUOPAP();
		}
		else//rku PMEANLIMITS ???????????
		{
			getModel()->getALARMHANDLER()->setAlimitsMinMaxRangePressure();
		}
	}

	bool bNewVentModeIsNMODE=activeModeIsNMODE();
	if(bOldVentModeWasNMODE!=bNewVentModeIsNMODE)
	{
		if(AfxGetApp() != NULL)
			AfxGetApp()->GetMainWnd()->PostMessage(WM_SYSTEMSTATE_CHANGED);
	}
	
	if(AfxGetApp())
		AfxGetApp()->GetMainWnd()->SetFocus();

	return true;
}

/**********************************************************************************************//**
 * Change to previous mode
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 **************************************************************************************************/

void CVentModeHandler::changeToPrevMode()
{
	switch(getModel()->getCONFIG()->GetPrevMode())
	{
	case VM_IPPV:
		{
			if(false==changeVentMode(VM_IPPV,0))
				theApp.WriteLog(_T("#HFO:0238"));

			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		break;
	case VM_PSV: 
		{
			if(false==changeVentMode(VM_PSV,0))
				theApp.WriteLog(_T("#HFO:0239"));

			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		break;
	case VM_SIMV:
		{
			if(false==changeVentMode(VM_SIMV,0))
				theApp.WriteLog(_T("#HFO:0240"));

			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		break;
	case VM_SIMVPSV:
		{
			if(false==changeVentMode(VM_SIMVPSV,0))
				theApp.WriteLog(_T("#HFO:0241"));

			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		break;
	case VM_SIPPV:
		{
			if(false==changeVentMode(VM_SIPPV,0))
				theApp.WriteLog(_T("#HFO:0242"));

			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		break;
	case VM_HFO:
		{
			if(false==changeVentMode(VM_HFO,0))
				theApp.WriteLog(_T("#HFO:0243"));

			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		break;
	case VM_CPAP:
		{
			if(false==changeVentMode(VM_CPAP,0))
				theApp.WriteLog(_T("#HFO:0244"));

			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		break;
	case VM_THERAPIE:
		{
			if(getModel()->getDATAHANDLER()->isTHERAPYLicenseAvailable()==true)
			{
				if(false==changeVentMode(VM_THERAPIE,0))
					theApp.WriteLog(_T("#HFO:0245"));

				if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)
				{
					getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
				}
			}
			else
			{
				if(false==changeVentMode(VM_CPAP,0))
					theApp.WriteLog(_T("#HFO:0246"));

				if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
				{
					getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
				}
			}
		}
		break;
	case VM_NCPAP:
		{
			if(getModel()->getDATAHANDLER()->isNMODELicenseAvailable()==true)
			{
				if(false==changeVentMode(VM_NCPAP,0))
					theApp.WriteLog(_T("#HFO:0247"));

				//rku,PRETRIGGER
				/*if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)
				{
					getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
				}*/
			}
			else
			{
				if(false==changeVentMode(VM_IPPV,0))
					theApp.WriteLog(_T("#HFO:0248"));

				if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
				{
					getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
				}

			}
		}
		break;
	case VM_DUOPAP:
		{
			if(getModel()->getDATAHANDLER()->isNMODELicenseAvailable()==true)
			{
				if(false==changeVentMode(VM_DUOPAP,0))
					theApp.WriteLog(_T("#HFO:0249"));
				//rku,PRETRIGGER
				/*if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)
				{
					getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
				}*/
			}
			else
			{
				if(false==changeVentMode(VM_IPPV,0))
					theApp.WriteLog(_T("#HFO:0250"));

				if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
				{
					getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
				}

			}
		}
		break;
	default:
		{
			theApp.WriteLog(_T("#HFO:0251"));
		}
		break;
	}
}

/**********************************************************************************************//**
 * Gets current active mode
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	The current active mode.
 **************************************************************************************************/

eVentMode CVentModeHandler::getCurrentActiveMode()
{
	eVentMode eActive=VM_NONE;
	if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
		eActive=getModel()->getCONFIG()->GetCurMode();
	else
		eActive=getModel()->getCONFIG()->GetPrevMode();
	return eActive;
}

/**********************************************************************************************//**
 * Determines if we can active mode is ncpap
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::activeModeIsNCPAP()
{
	if(getCurrentActiveMode()==VM_NCPAP)
		return true;
	else
		return false;
}

/**********************************************************************************************//**
 * Determines if we can active mode is duopap
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::activeModeIsDUOPAP()
{
	if(getCurrentActiveMode()==VM_DUOPAP)
		return true;
	else
		return false;
}

/**********************************************************************************************//**
 * Determines if we can active mode is therapy
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::activeModeIsTHERAPY()
{
	if(getCurrentActiveMode()==VM_THERAPIE)
		return true;
	else
		return false;
}

/**********************************************************************************************//**
 * Determines if we can active mode is cpap
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::activeModeIsCPAP()
{
	if(getCurrentActiveMode()==VM_CPAP)
		return true;
	else
		return false;
}

/**********************************************************************************************//**
 * Determines if we can active mode is ippv
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::activeModeIsIPPV()
{
	if(getCurrentActiveMode()==VM_IPPV)
		return true;
	else
		return false;
}

/**********************************************************************************************//**
 * Determines if we can active mode is sippv
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::activeModeIsSIPPV()
{
	if(getCurrentActiveMode()==VM_SIPPV)
		return true;
	else
		return false;
}

/**********************************************************************************************//**
 * Determines if we can active mode is simv
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::activeModeIsSIMV()
{
	if(getCurrentActiveMode()==VM_SIMV)
		return true;
	else
		return false;
}

/**********************************************************************************************//**
 * Determines if we can active mode is hfo
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::activeModeIsHFO()
{
	if(getCurrentActiveMode()==VM_HFO)
		return true;
	else
		return false;
}

/**********************************************************************************************//**
 * Determines if we can active mode has psv
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::activeModeHasPSV()
{
	if(		getCurrentActiveMode()==VM_PSV
		||	getCurrentActiveMode()==VM_SIMVPSV)
		return true;
	else
		return false;
}

/**********************************************************************************************//**
 * Determines if we can active mode is view model psv
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::activeModeIsVM_PSV()
{
	if(getCurrentActiveMode()==VM_PSV)
		return true;
	else
		return false;
}

/**********************************************************************************************//**
 * Determines if we can mode is ps vmode
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::modeIsPSVmode()
{
	if(getModel()->getCONFIG()->GetCurMode()==VM_PSV)
	{
		return true;
	}
	else if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_PSV)
	{
		return true;
	}
	else
		return false;
}

/**********************************************************************************************//**
 * Determines if we can view mode is any nmode
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::viewModeIsAnyNMODE()
{
	eVentMode eCurrent=getModel()->getCONFIG()->GetCurMode();
	if(		eCurrent==VM_NCPAP 
		||	eCurrent==VM_DUOPAP
		||	eCurrent==VM_THERAPIE
		||	eCurrent==VM_PRE_NCPAP 
		||	eCurrent==VM_PRE_DUOPAP
		||	eCurrent==VM_PRE_THERAPIE)
	{
		return true;
	}
	else
		return false;
}

/**********************************************************************************************//**
 * Determines if we can active mode is nmode
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::activeModeIsNMODE()
{
	eVentMode eActive=getCurrentActiveMode();
	if(		eActive==VM_NCPAP 
		||	eActive==VM_DUOPAP
		||	eActive==VM_THERAPIE)
		return true;
	else
		return false;
}

/**********************************************************************************************//**
 * Determines if we can current mode is conventional mode
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::curModeIsConventionalMode()
{
	bool bResult = false;

	eVentMode eCurrent=getModel()->getCONFIG()->GetCurMode();
	if(		eCurrent==VM_IPPV
		||	eCurrent==VM_SIPPV
		||	eCurrent==VM_SIMV
		||	eCurrent==VM_SIMVPSV
		||	eCurrent==VM_PSV
		||	eCurrent==VM_PRE_IPPV
		||	eCurrent==VM_PRE_SIPPV
		||	eCurrent==VM_PRE_SIMV
		||	eCurrent==VM_PRE_SIMVPSV
		||	eCurrent==VM_PRE_PSV)
	{
		bResult = true;
	}

	return bResult;
}

/**********************************************************************************************//**
 * Determines if we can current mode is volume support mode
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::curModeIsVolumeSupportMode()
{
	bool bResult = false;

	eVentMode eCurrent=getModel()->getCONFIG()->GetCurMode();
	if(		eCurrent==VM_IPPV
		||	eCurrent==VM_SIPPV
		||	eCurrent==VM_SIMV
		||	eCurrent==VM_SIMVPSV
		||	eCurrent==VM_PSV
		||	eCurrent==VM_HFO
		||	eCurrent==VM_PRE_IPPV
		||	eCurrent==VM_PRE_SIPPV
		||	eCurrent==VM_PRE_SIMV
		||	eCurrent==VM_PRE_SIMVPSV
		||	eCurrent==VM_PRE_PSV
		||	eCurrent==VM_PRE_HFO)
	{
		bResult = true;
	}

	return bResult;
}

/**********************************************************************************************//**
 * Determines if we can current mode is convert trigger mode
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::curModeIsCONVTriggerMode()
{
	bool bResult = false;

	eVentMode eCurrent=getModel()->getCONFIG()->GetCurMode();
	if(		eCurrent==VM_SIPPV
		||	eCurrent==VM_SIMV
		||	eCurrent==VM_SIMVPSV
		||	eCurrent==VM_PSV
		||	eCurrent==VM_PRE_SIPPV
		||	eCurrent==VM_PRE_SIMV
		||	eCurrent==VM_PRE_SIMVPSV
		||	eCurrent==VM_PRE_PSV)
	{
		bResult = true;
	}

	return bResult;
}

/**********************************************************************************************//**
 * Determines if we can current mode is convert trigger preset mode
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::curModeIsCONVTriggerPresetMode()
{
	bool bResult = false;

	eVentMode eCurrent=getModel()->getCONFIG()->GetCurMode();
	if(		eCurrent==VM_PRE_SIPPV
		||	eCurrent==VM_PRE_SIMV
		||	eCurrent==VM_PRE_SIMVPSV
		||	eCurrent==VM_PRE_PSV)
	{
		bResult = true;
	}

	return bResult;
}

/**********************************************************************************************//**
 * Determines if we can active mode is convert trigger
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::activeModeIsCONVTrigger()
{
	bool bResult = false;

	eVentMode eActive=getCurrentActiveMode();
	if(		eActive==VM_SIPPV
		||	eActive==VM_SIMV
		||	eActive==VM_SIMVPSV
		||	eActive==VM_PSV)
	{
		bResult = true;
	}
	return bResult;
}

/**********************************************************************************************//**
 * Determines if we can active mode is nmode trigger
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::activeModeIsNMODETrigger()
{
	bool bResult = false;

	eVentMode eActive=getCurrentActiveMode();
	if(		eActive==VM_DUOPAP
		||	eActive==VM_NCPAP)
	{
		bResult = true;
	}


	return bResult;
}

/**********************************************************************************************//**
 * Determines if we can active mode is trigger
 *
 * \author	Rainer Kühner
 * \date	23.02.2018
 *
 * \return	True if it succeeds, false if it fails.
 **************************************************************************************************/

bool CVentModeHandler::activeModeIsTrigger()
{
	bool bResult = false;

	eVentMode eActive=getCurrentActiveMode();
	if(		eActive==VM_SIPPV
		||	eActive==VM_SIMV
		||	eActive==VM_SIMVPSV
		||	eActive==VM_PSV
		||	eActive==VM_DUOPAP
		||	eActive==VM_NCPAP)
	{
		bResult = true;
	}


	return bResult;
}