#include "StdAfx.h"
#include "VentModeHandler.h"
#include "MVModel.h"
#include "MVViewHandler.h"

CVentModeHandler* CVentModeHandler::theVentModeHandler=0;

CVentModeHandler::CVentModeHandler(void)
{
	m_pModel = NULL;
}

CVentModeHandler::~CVentModeHandler(void)
{
}

//=============================================================================
/**
 * @brief Get the instance of vent mode handler (singleton).
 *
 * @return the instance of data handler
 *
 **/
//=============================================================================
CVentModeHandler* CVentModeHandler::getInstance()
{
	if(theVentModeHandler == 0)
	{
		theVentModeHandler = new CVentModeHandler;
	}
	return theVentModeHandler;
}

//=============================================================================
/**
 * @brief Destroys the instance of vent mode handler (singleton).
 **/
//=============================================================================
void CVentModeHandler::destroyInstance()
{
	if(theVentModeHandler != NULL)
	{
		delete theVentModeHandler;
		theVentModeHandler = NULL;
	}
}

//************************************
// Method:    setIPPV
// FullName:  CVentModeHandler::setIPPV
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
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

//************************************
// Method:    setSIPPV
// FullName:  CVentModeHandler::setSIPPV
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
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
//************************************
// Method:    setSIMV
// FullName:  CVentModeHandler::setSIMV
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
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
//************************************
// Method:    setSIMVPSV
// FullName:  CVentModeHandler::setSIMVPSV
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
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
//************************************
// Method:    setPSV
// FullName:  CVentModeHandler::setPSV
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
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
//************************************
// Method:    setHFO
// FullName:  CVentModeHandler::setHFO
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
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
//************************************
// Method:    setCPAP
// FullName:  CVentModeHandler::setCPAP
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
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
//************************************
// Method:    setNCPAP
// FullName:  CVentModeHandler::setNCPAP
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void CVentModeHandler::setNCPAP()
{
	if(getModel()->getCONFIG()->GetCurMode()!=VM_NCPAP)
	{
		getModel()->ResetExhalValvCalMode();

		if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_NCPAP)
		{
			getModel()->getDATAHANDLER()->PARADATA()->SetDataFromMode(VM_PRE_NCPAP);

			changeVentMode(VM_NCPAP);
			
			if(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)
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

			if(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)
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
//************************************
// Method:    setDUOPAP
// FullName:  CVentModeHandler::setDUOPAP
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void CVentModeHandler::setDUOPAP()
{
	if(getModel()->getCONFIG()->GetCurMode()!=VM_DUOPAP)
	{
		getModel()->ResetExhalValvCalMode();

		if(getModel()->getCONFIG()->GetCurMode()==VM_PRE_DUOPAP)
		{
			getModel()->getDATAHANDLER()->PARADATA()->SetDataFromMode(VM_PRE_DUOPAP);

			changeVentMode(VM_DUOPAP);

			if(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)
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

			if(getModel()->getDATAHANDLER()->GetTubeSet()==TUBE_MEDIJET && getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)
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

		}
	}
	else
	{
		if(AfxGetApp())
			AfxGetApp()->GetMainWnd()->SetFocus();
	}
}

//************************************
// Method:    setTHERAPIE
// FullName:  CVentModeHandler::setTHERAPIE
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
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
//=============================================================================
/**
 * @brief Get the instance of the model (singleton).
 *
 * @return the instance of the model
 *
 **/
//=============================================================================
CMVModel *CVentModeHandler::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

//************************************
// Method:    changeVentMode
// FullName:  CVentModeHandler::changeVentMode
// Access:    public 
// Returns:   bool
// Qualifier:
// Parameter: eVentMode state
// Parameter: int iSetAlarmSilent
//************************************
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

	getModel()->getALARMHANDLER()->ventModeChanged();

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

//************************************
// Method:    changeToPrevMode
// FullName:  CVentModeHandler::changeToPrevMode
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void CVentModeHandler::changeToPrevMode()
{
	switch(getModel()->getCONFIG()->GetPrevMode())
	{
	case VM_IPPV:
		{
			if(false==changeVentMode(VM_IPPV,0))
				theApp.getLog()->WriteLine(_T("#HFO:0238"));

			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		break;
	case VM_PSV: 
		{
			if(false==changeVentMode(VM_PSV,0))
				theApp.getLog()->WriteLine(_T("#HFO:0239"));

			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		break;
	case VM_SIMV:
		{
			if(false==changeVentMode(VM_SIMV,0))
				theApp.getLog()->WriteLine(_T("#HFO:0240"));

			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		break;
	case VM_SIMVPSV:
		{
			if(false==changeVentMode(VM_SIMVPSV,0))
				theApp.getLog()->WriteLine(_T("#HFO:0241"));

			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		break;
	case VM_SIPPV:
		{
			if(false==changeVentMode(VM_SIPPV,0))
				theApp.getLog()->WriteLine(_T("#HFO:0242"));

			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		break;
	case VM_HFO:
		{
			if(false==changeVentMode(VM_HFO,0))
				theApp.getLog()->WriteLine(_T("#HFO:0243"));

			if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
			{
				getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
			}
		}
		break;
	case VM_CPAP:
		{
			if(false==changeVentMode(VM_CPAP,0))
				theApp.getLog()->WriteLine(_T("#HFO:0244"));

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
					theApp.getLog()->WriteLine(_T("#HFO:0245"));

				if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)
				{
					getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
				}
			}
			else
			{
				if(false==changeVentMode(VM_CPAP,0))
					theApp.getLog()->WriteLine(_T("#HFO:0246"));

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
					theApp.getLog()->WriteLine(_T("#HFO:0247"));

				//rku,PRETRIGGER
				/*if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)
				{
					getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
				}*/
			}
			else
			{
				if(false==changeVentMode(VM_IPPV,0))
					theApp.getLog()->WriteLine(_T("#HFO:0248"));

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
					theApp.getLog()->WriteLine(_T("#HFO:0249"));
				//rku,PRETRIGGER
				/*if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_ON)
				{
					getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_OFF);
				}*/
			}
			else
			{
				if(false==changeVentMode(VM_IPPV,0))
					theApp.getLog()->WriteLine(_T("#HFO:0250"));

				if(getModel()->getDATAHANDLER()->GetFlowSensorState()==FLOWSENSOR_OFF)
				{
					getModel()->getDATAHANDLER()->SetFlowSensorState(FLOWSENSOR_ON);
				}

			}
		}
		break;
	default:
		{
			theApp.getLog()->WriteLine(_T("#HFO:0251"));
		}
		break;
	}
}

eVentMode CVentModeHandler::getCurrentActiveMode()
{
	eVentMode eActive=VM_NONE;
	if(getModel()->getCONFIG()->CurModeIsPresetMode()==false)
		eActive=getModel()->getCONFIG()->GetCurMode();
	else
		eActive=getModel()->getCONFIG()->GetPrevMode();
	return eActive;
}
bool CVentModeHandler::activeModeIsNCPAP()
{
	if(getCurrentActiveMode()==VM_NCPAP)
		return true;
	else
		return false;
}
bool CVentModeHandler::activeModeIsDUOPAP()
{
	if(getCurrentActiveMode()==VM_DUOPAP)
		return true;
	else
		return false;
}
bool CVentModeHandler::activeModeIsTHERAPY()
{
	if(getCurrentActiveMode()==VM_THERAPIE)
		return true;
	else
		return false;
}
bool CVentModeHandler::activeModeIsCPAP()
{
	if(getCurrentActiveMode()==VM_CPAP)
		return true;
	else
		return false;
}
bool CVentModeHandler::activeModeIsIPPV()
{
	if(getCurrentActiveMode()==VM_IPPV)
		return true;
	else
		return false;
}

bool CVentModeHandler::activeModeIsHFO()
{
	if(getCurrentActiveMode()==VM_HFO)
		return true;
	else
		return false;
}
bool CVentModeHandler::activeModeHasPSV()
{
	if(		getCurrentActiveMode()==VM_PSV
		||	getCurrentActiveMode()==VM_SIMVPSV)
		return true;
	else
		return false;
}
bool CVentModeHandler::activeModeIsVM_PSV()
{
	if(getCurrentActiveMode()==VM_PSV)
		return true;
	else
		return false;
}
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