/**********************************************************************************************//**
 * \file	AlarmConditionHandler.cpp.
 *
 * Implements the alarm condition handler class
 **************************************************************************************************/

#include "StdAfx.h"
#include "FabianHFO.h"
#include "MVModel.h"
#include "AlarmConditionHandler.h"

CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_MVMAX=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_MVMIN=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_PIPMAX=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_PIPMIN=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_PEEPMIN=NULL; 
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_FREQMAX=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_LEAKMAX=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_APNOE=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_DCO2MAX=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_DCO2MIN=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_ETCO2MAX=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_ETCO2MIN=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_FICO2MAX=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_FICO2MIN=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_SPO2MAX=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_SPO2MIN=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_PULSERATEMAX=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_PULSERATEMIN=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_SPO2PIMIN=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_SPO2SIQMIN=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_MAPMAX=NULL;
CAlarmLimitPtr CAlarmConditionHandler::ALIMIT_MAPMIN=NULL;

/**********************************************************************************************//**
 * Default constructor
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

CAlarmConditionHandler::CAlarmConditionHandler()
{
	InitializeCriticalSection(&csAlarmLimitList);

	m_pModel=NULL;
	m_pAlarmLimitlist=NULL;
	initAlarmLimitList();
}

/**********************************************************************************************//**
 * Destructor
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

CAlarmConditionHandler::~CAlarmConditionHandler(void)
{
	if(ALIMIT_MVMAX)
		delete ALIMIT_MVMAX;
	ALIMIT_MVMAX=NULL;
	if(ALIMIT_MVMIN)
		delete ALIMIT_MVMIN;
	ALIMIT_MVMIN=NULL;
	if(ALIMIT_PIPMAX)
		delete ALIMIT_PIPMAX;
	ALIMIT_PIPMAX=NULL;
	if(ALIMIT_PIPMIN)
		delete ALIMIT_PIPMIN;
	ALIMIT_PIPMIN=NULL;
	if(ALIMIT_PEEPMIN)
		delete ALIMIT_PEEPMIN;
	ALIMIT_PEEPMIN=NULL; 
	if(ALIMIT_FREQMAX)
		delete ALIMIT_FREQMAX;
	ALIMIT_FREQMAX=NULL;
	if(ALIMIT_LEAKMAX)
		delete ALIMIT_LEAKMAX;
	ALIMIT_LEAKMAX=NULL;
	if(ALIMIT_APNOE)
		delete ALIMIT_APNOE;
	ALIMIT_APNOE=NULL;
	if(ALIMIT_DCO2MAX)
		delete ALIMIT_DCO2MAX;
	ALIMIT_DCO2MAX=NULL;
	if(ALIMIT_DCO2MIN)
		delete ALIMIT_DCO2MIN;
	ALIMIT_DCO2MIN=NULL;
	if(ALIMIT_ETCO2MAX)
		delete ALIMIT_ETCO2MAX;
	ALIMIT_ETCO2MAX=NULL;
	if(ALIMIT_ETCO2MIN)
		delete ALIMIT_ETCO2MIN;
	ALIMIT_ETCO2MIN=NULL;
	if(ALIMIT_FICO2MAX)
		delete ALIMIT_FICO2MAX;
	ALIMIT_FICO2MAX=NULL;
	if(ALIMIT_FICO2MIN)
		delete ALIMIT_FICO2MIN;
	ALIMIT_FICO2MIN=NULL;
	if(ALIMIT_SPO2MAX)
		delete ALIMIT_SPO2MAX;
	ALIMIT_SPO2MAX=NULL;
	if(ALIMIT_SPO2MIN)
		delete ALIMIT_SPO2MIN;
	ALIMIT_SPO2MIN=NULL;
	if(ALIMIT_PULSERATEMAX)
		delete ALIMIT_PULSERATEMAX;
	ALIMIT_PULSERATEMAX=NULL;
	if(ALIMIT_PULSERATEMIN)
		delete ALIMIT_PULSERATEMIN;
	ALIMIT_PULSERATEMIN=NULL;
	if(ALIMIT_SPO2PIMIN)
		delete ALIMIT_SPO2PIMIN;
	ALIMIT_SPO2PIMIN=NULL;
	if(ALIMIT_SPO2SIQMIN)
		delete ALIMIT_SPO2SIQMIN;
	ALIMIT_SPO2SIQMIN=NULL;
	if(ALIMIT_MAPMAX)
		delete ALIMIT_MAPMAX;
	ALIMIT_MAPMAX=NULL;
	if(ALIMIT_MAPMIN)
		delete ALIMIT_MAPMIN;
	ALIMIT_MAPMIN=NULL;

	if(m_pAlarmLimitlist)
	{
		delete m_pAlarmLimitlist;
	}
	m_pAlarmLimitlist=NULL;

	DeleteCriticalSection(&csAlarmLimitList);
}

/**********************************************************************************************//**
 * Gets the model, singleton
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	Null if it fails, else the model.
 **************************************************************************************************/

CMVModel *CAlarmConditionHandler::getModel()
{
	if(m_pModel==NULL)
		m_pModel=CMVModel::GetInstance();
	return m_pModel;
}

/**********************************************************************************************//**
 * Initializes the alarm limit list
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::initAlarmLimitList()
{
	ALIMIT_MVMAX=new CAlarmLimit(LI_MVMAX,AL_OFF,0,0,0);
	ALIMIT_MVMIN=new CAlarmLimit(LI_MVMIN,AL_OFF,0,0,0);
	ALIMIT_PIPMAX=new CAlarmLimit(LI_PIPMAX,AL_OFF,0,0,0);
	ALIMIT_PIPMIN=new CAlarmLimit(LI_PIPMIN,AL_OFF,0,0,0);
	ALIMIT_PEEPMIN=new CAlarmLimit(LI_PMIN,AL_OFF,0,0,0); 
	ALIMIT_FREQMAX=new CAlarmLimit(LI_FREQMAX,AL_OFF,0,0,0);
	ALIMIT_LEAKMAX=new CAlarmLimit(LI_LEAKMAX,AL_OFF,0,0,0);
	ALIMIT_APNOE=new CAlarmLimit(LI_APNOE,AL_OFF,0,0,0);
	ALIMIT_DCO2MAX=new CAlarmLimit(LI_DCO2MAX,AL_OFF,0,0,0);
	ALIMIT_DCO2MIN=new CAlarmLimit(LI_DCO2MIN,AL_OFF,0,0,0);
	ALIMIT_ETCO2MAX=new CAlarmLimit(LI_ETCO2MAX,AL_OFF,0,0,0);
	ALIMIT_ETCO2MIN=new CAlarmLimit(LI_ETCO2MIN,AL_OFF,0,0,0);
	ALIMIT_FICO2MAX=new CAlarmLimit(LI_FICO2MAX,AL_OFF,0,0,0);
	ALIMIT_FICO2MIN=new CAlarmLimit(LI_FICO2MIN,AL_OFF,0,0,0);
	ALIMIT_SPO2MAX=new CAlarmLimit(LI_SPO2MAX,AL_OFF,0,0,0);
	ALIMIT_SPO2MIN=new CAlarmLimit(LI_SPO2MIN,AL_OFF,0,0,0);
	ALIMIT_PULSERATEMAX=new CAlarmLimit(LI_PULSERATEMAX,AL_OFF,0,0,0);
	ALIMIT_PULSERATEMIN=new CAlarmLimit(LI_PULSERATEMIN,AL_OFF,0,0,0);
	ALIMIT_SPO2PIMIN=new CAlarmLimit(LI_SPO2PIMIN,AL_OFF,0,0,0);
	ALIMIT_SPO2SIQMIN=new CAlarmLimit(LI_SPO2SIQMIN,AL_OFF,0,0,0);
	ALIMIT_MAPMAX=new CAlarmLimit(LI_MAPMAX,AL_OFF,0,0,0);
	ALIMIT_MAPMIN=new CAlarmLimit(LI_MAPMIN,AL_OFF,0,0,0);
	

	EnterCriticalSection(&csAlarmLimitList);
	m_pAlarmLimitlist = new CAlarmLimitList();

	if(m_pAlarmLimitlist)
	{
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_MVMAX);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_MVMIN);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_PIPMAX);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_PIPMIN);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_PEEPMIN);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_FREQMAX);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_LEAKMAX);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_APNOE);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_DCO2MAX);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_DCO2MIN);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_ETCO2MAX);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_ETCO2MIN);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_FICO2MAX);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_FICO2MIN);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_SPO2MAX);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_SPO2MIN);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_PULSERATEMAX);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_PULSERATEMIN);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_SPO2PIMIN);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_SPO2SIQMIN);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_MAPMAX);
		m_pAlarmLimitlist->appendAlarmLimit(ALIMIT_MAPMIN);
	}
	LeaveCriticalSection(&csAlarmLimitList);
}

/**********************************************************************************************//**
 * Vent mode changed
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::ventModeChanged()
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();

	if(AL_CALC==getAlimitState_MVmaxLimit())
	{
		setAlimitState_MVmaxLimit(AL_ON);
	}
	if(AL_CALC==getAlimitState_MVminLimit())
	{
		setAlimitState_MVminLimit(AL_ON);
	}
	if(AL_CALC==getAlimitState_PIPmaxLimit())
	{
		setAlimitState_PIPmaxLimit(AL_ON);
	}
	if(AL_CALC==getAlimitState_PEEPminLimit())
	{
		setAlimitState_PEEPminLimit(AL_ON);
	}
	if(AL_CALC==getAlimitState_BPMmaxLimit())
	{
		setAlimitState_BPMmaxLimit(AL_ON);
	}
	if(AL_CALC==getAlimitState_LeakmaxLimit())
	{
		setAlimitState_LeakmaxLimit(AL_ON);
	}
	if(AL_CALC==getAlimitState_ApnoeLimit())
	{
		setAlimitState_ApnoeLimit(AL_ON);
	}
	if(AL_CALC==getAlimitState_DCO2maxLimit())
	{
		setAlimitState_DCO2maxLimit(AL_ON);
	}
	if(AL_CALC==getAlimitState_DCO2minLimit())
	{
		setAlimitState_DCO2minLimit(AL_ON);
	}
	if(AL_CALC==getAlimitState_ETCO2maxLimit())
	{
		setAlimitState_ETCO2maxLimit(AL_ON);
	}
	if(AL_CALC==getAlimitState_ETCO2minLimit())
	{
		setAlimitState_ETCO2minLimit(AL_ON);
	}
	if(AL_CALC==getAlimitState_FICO2maxLimit())
	{
		setAlimitState_FICO2maxLimit(AL_ON);
	}
	if(AL_CALC==getAlimitState_FICO2minLimit())
	{
		setAlimitState_FICO2minLimit(AL_ON);
	}
	if(AL_CALC==getAlimitState_SPO2maxLimit())
	{
		setAlimitState_SPO2maxLimit(AL_ON);
	}
	if(AL_CALC==getAlimitState_SPO2minLimit())
	{
		setAlimitState_SPO2minLimit(AL_ON);
	}
	if(AL_CALC==getAlimitState_PulseRatemaxLimit())
	{
		setAlimitState_PulseRatemaxLimit(AL_ON);
	}
	if(AL_CALC==getAlimitState_PulseRateminLimit())
	{
		setAlimitState_PulseRateminLimit(AL_ON);
	}
	if(AL_CALC==getAlimitState_SPO2_PIminLimit())
	{
		setAlimitState_SPO2_PIminLimit(AL_ON);
	}

	

	if(eActiveVentMode==VM_HFO)
	{
		setAlarmLimitValues_HFO();
		setAlarmLimitRanges_HFO();
	}
	else if(eActiveVentMode==VM_CPAP)
	{
		setAlarmLimitValues_CPAP();
		setAlarmLimitRanges_CPAP();
	}
	else if( eActiveVentMode==VM_DUOPAP)
	{
		setAlarmLimitValues_DUOPAP();
		setAlarmLimitRanges_DUOPAP();
	}
	else if(eActiveVentMode==VM_NCPAP || eActiveVentMode==VM_THERAPIE)
	{
		setAlarmLimitValues_NCPAP();
		setAlarmLimitRanges_NCPAP();
	}
	else
	{
		setAlarmLimitValues_DEFAULT();
		setAlarmLimitRanges_DEFAULT();
	}

	if(getModel()->getCONFIG()->getCO2module()!=CO2MODULE_NONE)
	{
		//setAlarmLimitValues_ETCO2();
		setAlarmLimitRanges_ETCO2();
	}

	if(getModel()->getCONFIG()->getSPO2module()!=SPO2MODULE_NONE)
	{
		setAlarmLimitValues_SPO2();
		setAlarmLimitRanges_SPO2();
	}
}

/**********************************************************************************************//**
 * Sets alarm limit values spo2
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::setAlarmLimitValues_SPO2()
{
	ALIMIT_SPO2MAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2max());
	ALIMIT_SPO2MIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2min());
	ALIMIT_PULSERATEMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPulseRatemax());
	ALIMIT_PULSERATEMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPulseRatemin());
	ALIMIT_SPO2PIMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2_PImin());
	ALIMIT_SPO2SIQMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2_SIQmin());

	ALIMIT_SPO2MAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2max());
	ALIMIT_SPO2MIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2min());
	ALIMIT_PULSERATEMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePulseRatemax());
	ALIMIT_PULSERATEMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePulseRatemin());
	ALIMIT_SPO2PIMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2_PImin());
	ALIMIT_SPO2SIQMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2_SIQmin());
}

//void CAlarmConditionHandler::setAlarmLimitValues_ETCO2()
//{
//
//}
//void CAlarmConditionHandler::setAlarmLimitValues_SPO2()
//{
//
//}

/**********************************************************************************************//**
 * Sets alarm limit values hfo
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::setAlarmLimitValues_HFO()
{
	ALIMIT_MVMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitMVmaxHF());
	ALIMIT_MVMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitMVminHF());
	//ALIMIT_PIPMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPmaxHF());
	//ALIMIT_PIPMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPIPmin());
	ALIMIT_PEEPMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPEEPminHF());
	ALIMIT_FREQMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitBPMmaxHF());
	ALIMIT_LEAKMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitLeakmaxHF());
	ALIMIT_APNOE->setCurValue(getModel()->getCONFIG()->GetAlarmlimitApnoeHF());
	ALIMIT_DCO2MAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitDCO2maxHF());
	ALIMIT_DCO2MIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitDCO2minHF());
	ALIMIT_ETCO2MAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitETCO2maxHF());
	ALIMIT_ETCO2MIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitETCO2minHF());
	ALIMIT_FICO2MAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitFICO2maxHF());
	ALIMIT_FICO2MIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitFICO2minHF());
	ALIMIT_MAPMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitMAPmaxHF());
	ALIMIT_MAPMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitMAPminHF());

	/*ALIMIT_SPO2MAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2maxHF());
	ALIMIT_SPO2MIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2minHF());
	ALIMIT_PULSERATEMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPulseRatemaxHF());
	ALIMIT_PULSERATEMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPulseRateminHF());
	ALIMIT_SPO2PIMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2_PIminHF());
	ALIMIT_SPO2SIQMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2_SIQminHF());*/


	ALIMIT_MVMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateMVmaxHF());
	ALIMIT_MVMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateMVminHF());
	//ALIMIT_PIPMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePmaxHF());
	ALIMIT_PIPMAX->setLimitState(AL_OFF);
	ALIMIT_PIPMIN->setLimitState(AL_OFF);
	ALIMIT_PEEPMIN->setLimitState(AL_OFF);
	ALIMIT_FREQMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateBPMmaxHF());
	ALIMIT_LEAKMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateLeakmaxHF());
	ALIMIT_APNOE->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateApnoeHF());
	ALIMIT_DCO2MAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateDCO2maxHF());
	ALIMIT_DCO2MIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateDCO2minHF());
	ALIMIT_ETCO2MAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateETCO2maxHF());
	ALIMIT_ETCO2MIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateETCO2minHF());
	ALIMIT_FICO2MAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateFICO2maxHF());
	ALIMIT_FICO2MIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateFICO2minHF());
	ALIMIT_MAPMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateMAPmaxHF());
	ALIMIT_MAPMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateMAPminHF());

	/*ALIMIT_SPO2MAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2maxHF());
	ALIMIT_SPO2MIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2minHF());
	ALIMIT_PULSERATEMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePulseRatemaxHF());
	ALIMIT_PULSERATEMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePulseRateminHF());
	ALIMIT_SPO2PIMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2_PIminHF());
	ALIMIT_SPO2SIQMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2_SIQminHF());*/
}

/**********************************************************************************************//**
 * Sets alarm limit values default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::setAlarmLimitValues_DEFAULT()
{
	ALIMIT_MVMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitMVmax());
	ALIMIT_MVMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitMVmin());
	ALIMIT_PIPMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPIPmax());
	ALIMIT_PIPMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPIPmin());
	ALIMIT_PEEPMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPEEPmin());
	ALIMIT_FREQMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitBPMmax());
	ALIMIT_LEAKMAX->setCurValue(getModel()->getCONFIG()->getAlimitLeakmax());
	ALIMIT_APNOE->setCurValue(getModel()->getCONFIG()->GetAlarmlimitApnoe());
	ALIMIT_DCO2MAX->setCurValue(0);
	ALIMIT_DCO2MIN->setCurValue(0);
	ALIMIT_ETCO2MAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitETCO2max());
	ALIMIT_ETCO2MIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitETCO2min());
	ALIMIT_FICO2MAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitFICO2max());
	ALIMIT_FICO2MIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitFICO2min());
	ALIMIT_MAPMAX->setCurValue(0);
	ALIMIT_MAPMIN->setCurValue(0);

	/*ALIMIT_SPO2MAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2max());
	ALIMIT_SPO2MIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2min());
	ALIMIT_PULSERATEMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPulseRatemax());
	ALIMIT_PULSERATEMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPulseRatemin());
	ALIMIT_SPO2PIMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2_PImin());
	ALIMIT_SPO2SIQMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2_SIQmin());*/


	ALIMIT_MVMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateMVmax());
	ALIMIT_MVMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateMVmin());
	ALIMIT_PIPMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePIPmax());
	ALIMIT_PIPMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePIPmin());
	ALIMIT_PEEPMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePEEPmin());
	ALIMIT_FREQMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateBPMmax());
	ALIMIT_LEAKMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateLeakmax());
	ALIMIT_APNOE->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateApnoe());
	ALIMIT_DCO2MAX->setLimitState(AL_OFF);
	ALIMIT_DCO2MIN->setLimitState(AL_OFF);
	ALIMIT_ETCO2MAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateETCO2max());
	ALIMIT_ETCO2MIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateETCO2min());
	ALIMIT_FICO2MAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateFICO2max());
	ALIMIT_FICO2MIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateFICO2min());
	ALIMIT_MAPMAX->setLimitState(AL_OFF);
	ALIMIT_MAPMIN->setLimitState(AL_OFF);
	/*ALIMIT_SPO2MAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2max());
	ALIMIT_SPO2MIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2min());
	ALIMIT_PULSERATEMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePulseRatemax());
	ALIMIT_PULSERATEMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePulseRatemin());
	ALIMIT_SPO2PIMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2_PImin());
	ALIMIT_SPO2SIQMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2_SIQmin());*/
}

/**********************************************************************************************//**
 * Sets alarm limit values cpap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::setAlarmLimitValues_CPAP()
{
	ALIMIT_MVMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitMVmax());
	ALIMIT_MVMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitMVmin());
	ALIMIT_PIPMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPmaxCpap());
	ALIMIT_PEEPMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPEEPminCpap());
	/*ALIMIT_SPO2MAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2max());
	ALIMIT_SPO2MIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2min());
	ALIMIT_PULSERATEMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPulseRatemax());
	ALIMIT_PULSERATEMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPulseRatemin());
	ALIMIT_SPO2PIMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2_PImin());
	ALIMIT_SPO2SIQMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2_SIQmin());*/
	ALIMIT_FREQMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitBPMmax());
	ALIMIT_LEAKMAX->setCurValue(getModel()->getCONFIG()->getAlimitLeakmax());

	ALIMIT_APNOE->setCurValue(getModel()->getCONFIG()->GetAlarmlimitApnoe());//rku,PRETRIGGER
	ALIMIT_APNOE->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateApnoe());//rku,PRETRIGGER

	ALIMIT_FREQMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateBPMmax());
	ALIMIT_LEAKMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateLeakmax());
	
	ALIMIT_MVMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateMVmax());
	ALIMIT_MVMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateMVmin());
	ALIMIT_PIPMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePmaxCpap());
	ALIMIT_PIPMIN->setLimitState(AL_OFF);
	ALIMIT_PEEPMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePEEPminCpap());
	/*ALIMIT_SPO2MAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2max());
	ALIMIT_SPO2MIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2min());
	ALIMIT_PULSERATEMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePulseRatemax());
	ALIMIT_PULSERATEMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePulseRatemin());
	ALIMIT_SPO2PIMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2_PImin());
	ALIMIT_SPO2SIQMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2_SIQmin());*/
}

/**********************************************************************************************//**
 * Sets alarm limit values ncpap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::setAlarmLimitValues_NCPAP()
{
	ALIMIT_PIPMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPmaxNCPAP());
	ALIMIT_PEEPMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPEEPminNCPAP());
	/*ALIMIT_SPO2MAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2max());
	ALIMIT_SPO2MIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2min());
	ALIMIT_PULSERATEMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPulseRatemax());
	ALIMIT_PULSERATEMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPulseRatemin());
	ALIMIT_SPO2PIMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2_PImin());
	ALIMIT_SPO2SIQMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2_SIQmin());*/
	
	ALIMIT_APNOE->setCurValue(getModel()->getCONFIG()->GetAlarmlimitApnoe());//rku,PRETRIGGER
	ALIMIT_APNOE->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateApnoe());//rku,PRETRIGGER

	ALIMIT_FREQMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitBPMmax());
	ALIMIT_FREQMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateBPMmax());

	ALIMIT_PIPMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePmaxNCPAP());
	ALIMIT_PIPMIN->setLimitState(AL_OFF);
	ALIMIT_PEEPMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePEEPminNCPAP());
	/*ALIMIT_SPO2MAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2max());
	ALIMIT_SPO2MIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2min());
	ALIMIT_PULSERATEMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePulseRatemax());
	ALIMIT_PULSERATEMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePulseRatemin());
	ALIMIT_SPO2PIMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2_PImin());
	ALIMIT_SPO2SIQMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2_SIQmin());*/
}

/**********************************************************************************************//**
 * Sets alarm limit values duopap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::setAlarmLimitValues_DUOPAP()
{
	ALIMIT_PIPMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPIPmaxDUOPAP());
	ALIMIT_PIPMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPIPminDUOPAP());
	ALIMIT_PEEPMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPEEPminDUOPAP());
	/*ALIMIT_SPO2MAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2max());
	ALIMIT_SPO2MIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2min());
	ALIMIT_PULSERATEMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPulseRatemax());
	ALIMIT_PULSERATEMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitPulseRatemin());
	ALIMIT_SPO2PIMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2_PImin());
	ALIMIT_SPO2SIQMIN->setCurValue(getModel()->getCONFIG()->GetAlarmlimitSPO2_SIQmin());*/

	ALIMIT_APNOE->setCurValue(getModel()->getCONFIG()->GetAlarmlimitApnoe());//rku,PRETRIGGER
	ALIMIT_APNOE->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateApnoe());//rku,PRETRIGGER

	ALIMIT_FREQMAX->setCurValue(getModel()->getCONFIG()->GetAlarmlimitBPMmax());
	ALIMIT_FREQMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateBPMmax());

	ALIMIT_PIPMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePIPmaxDUOPAP());
	ALIMIT_PIPMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePIPminDUOPAP());
	ALIMIT_PEEPMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePEEPminDUOPAP());
	/*ALIMIT_SPO2MAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2max());
	ALIMIT_SPO2MIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2min());
	ALIMIT_PULSERATEMAX->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePulseRatemax());
	ALIMIT_PULSERATEMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStatePulseRatemin());
	ALIMIT_SPO2PIMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2_PImin());
	ALIMIT_SPO2SIQMIN->setLimitState(getModel()->getCONFIG()->GetAlarmlimitStateSPO2_SIQmin());*/
}

/**********************************************************************************************//**
 * Sets alarm limit ranges default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::setAlarmLimitRanges_DEFAULT()
{
	//DEBUGMSG(TRUE, (TEXT("CAlarmConditionHandler::setAlarmLimitRanges_DEFAULT()\r\n")));
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		//MVmax
		ALIMIT_MVMAX->setMaxValue(MAXRANGE_NEONATAL_MVMAX);

		int iMVmin=ALIMIT_MVMIN->getCurValue();
		if(getModel()->getALARMHANDLER()->getAlimitState_MVminLimit() == AL_OFF)
		{
			ALIMIT_MVMAX->setMinValue(MINRANGE_NEONATAL_MVMIN);
		}
		else if(iMVmin<=260)
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+10);
		}
		else if(iMVmin<=500)
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+20);
		}
		else if(iMVmin<=1000)
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+50);
		}
		else
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+100);
		}

		//MVmin
		ALIMIT_MVMIN->setMinValue(MINRANGE_NEONATAL_MVMIN);

		int iMVmax=ALIMIT_MVMAX->getCurValue();
		if(iMVmax<=260)
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-10);
		}
		else if(iMVmax<=500)
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-20);
		}
		else if(iMVmax<=1000)
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-50);
		}
		else
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-100);
		}

		//rku PIPLOW DUOPAP
		//Pmax
		int iCurPEEP=ALIMIT_PEEPMIN->getCurValue();
		if(iCurPEEP%10!=0)
		{
			iCurPEEP+=5;
		}

		ALIMIT_PIPMAX->setMaxValue(MAXRANGE_NEONATAL_PMAX);
		
		if (ALIMIT_PIPMIN->getLimitState()==AL_OFF)
		{
			DEBUGMSG(TRUE, (TEXT("1 PIPMAX->setMinValue %d\r\n"),iCurPEEP+DIFF_PMAX_PMIN_LIMIT));
			ALIMIT_PIPMAX->setMinValue(iCurPEEP+DIFF_PMAX_PMIN_LIMIT);
		}
		else
		{
			DEBUGMSG(TRUE, (TEXT("2 PIPMAX->setMinValue %d\r\n"),ALIMIT_PIPMIN->getCurValue()+DIFF_PMAX_PMIN_LIMIT));
			ALIMIT_PIPMAX->setMinValue(ALIMIT_PIPMIN->getCurValue()+DIFF_PMAX_PMIN_LIMIT);
		}


		ALIMIT_PIPMIN->setMaxValue(ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT);
		
		//DEBUGMSG(TRUE, (TEXT("ALIMIT_PIPMIN->setMinValue %d\r\n"),iCurPEEP));//rku newPIPOFF
		ALIMIT_PIPMIN->setMinValue(iCurPEEP);

		//PEEP
		if (ALIMIT_PIPMIN->getLimitState()==AL_OFF)//rku newPIPOFF
		{
			//DEBUGMSG(TRUE, (TEXT("ALIMIT_PEEPMIN->setMaxValue %d\r\n"),ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT));
			ALIMIT_PEEPMIN->setMaxValue(ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT);
		}
		else
		{
			//DEBUGMSG(TRUE, (TEXT("ALIMIT_PEEPMIN->setMaxValue %d\r\n"),ALIMIT_PIPMIN->getCurValue()));
			ALIMIT_PEEPMIN->setMaxValue(ALIMIT_PIPMIN->getCurValue());
		}
		ALIMIT_PEEPMIN->setMinValue(MINRANGE_NEONATAL_CPAPPEEPMIN);



		//BPM
		ALIMIT_FREQMAX->setMaxValue(MAXRANGE_NEONATAL_BPMMAX);
		ALIMIT_FREQMAX->setMinValue(MINRANGE_NEONATAL_BPMMIN);

		//Leak
		ALIMIT_LEAKMAX->setMaxValue(MAXRANGE_NEONATAL_LEAKMAX);
		ALIMIT_LEAKMAX->setMinValue(MINRANGE_NEONATAL_LEAKMIN);

		//Apnoe
		ALIMIT_APNOE->setMaxValue(MAXRANGE_NEONATAL_APNOE);
		ALIMIT_APNOE->setMinValue(MINRANGE_NEONATAL_APNOE);
	}
	else
	{
		//PEDIATRIC;
		//MVmax
		ALIMIT_MVMAX->setMaxValue(MAXRANGE_PEDIATRIC_MVMAX);

		int iMVmin=ALIMIT_MVMIN->getCurValue();
		if(getModel()->getALARMHANDLER()->getAlimitState_MVminLimit() == AL_OFF)
		{
			ALIMIT_MVMAX->setMinValue(MINRANGE_PEDIATRIC_MVMIN);
		}
		else if(iMVmin<=260)
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+10);
		}
		else if(iMVmin<=500)
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+20);
		}
		else if(iMVmin<=1000)
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+50);
		}
		else
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+100);
		}

		//MVmin
		ALIMIT_MVMIN->setMinValue(MINRANGE_PEDIATRIC_MVMIN);

		int iMVmax=ALIMIT_MVMAX->getCurValue();
		if(iMVmax<=260)
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-10);
		}
		else if(iMVmax<=500)
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-20);
		}
		else if(iMVmax<=1000)
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-50);
		}
		else
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-100);
		}
		
		//rku PIPLOW DUOPAP
		//Pmax
		int iCurPEEP=ALIMIT_PEEPMIN->getCurValue();
		if(iCurPEEP%10!=0)
		{
			iCurPEEP+=5;
		}

		ALIMIT_PIPMAX->setMaxValue(MAXRANGE_PEDIATRIC_PMAX);

		if (ALIMIT_PIPMIN->getLimitState()==AL_OFF)
		{
			ALIMIT_PIPMAX->setMinValue(iCurPEEP+DIFF_PMAX_PMIN_LIMIT);
		}
		else
		{
			ALIMIT_PIPMAX->setMinValue(ALIMIT_PIPMIN->getCurValue()+DIFF_PMAX_PMIN_LIMIT);
		}


		ALIMIT_PIPMIN->setMaxValue(ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT);

		//DEBUGMSG(TRUE, (TEXT("ALIMIT_PIPMIN->setMinValue %d\r\n"),iCurPEEP));//rku newPIPOFF
		ALIMIT_PIPMIN->setMinValue(iCurPEEP);

		//PEEP
		if (ALIMIT_PIPMIN->getLimitState()==AL_OFF)//rku newPIPOFF
		{
			//DEBUGMSG(TRUE, (TEXT("ALIMIT_PEEPMIN->setMaxValue %d\r\n"),ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT));
			ALIMIT_PEEPMIN->setMaxValue(ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT);
		}
		else
		{
			//DEBUGMSG(TRUE, (TEXT("ALIMIT_PEEPMIN->setMaxValue %d\r\n"),ALIMIT_PIPMIN->getCurValue()));
			ALIMIT_PEEPMIN->setMaxValue(ALIMIT_PIPMIN->getCurValue());
		}
		ALIMIT_PEEPMIN->setMinValue(MINRANGE_PEDIATRIC_CPAPPEEPMIN);
		
		//BPM
		ALIMIT_FREQMAX->setMaxValue(MAXRANGE_PEDIATRIC_BPMMAX);
		ALIMIT_FREQMAX->setMinValue(MINRANGE_PEDIATRIC_BPMMIN);

		//Leak
		ALIMIT_LEAKMAX->setMaxValue(MAXRANGE_PEDIATRIC_LEAKMAX);
		ALIMIT_LEAKMAX->setMinValue(MINRANGE_PEDIATRIC_LEAKMIN);

		//Apnoe
		ALIMIT_APNOE->setMaxValue(MAXRANGE_PEDIATRIC_APNOE);
		ALIMIT_APNOE->setMinValue(MINRANGE_PEDIATRIC_APNOE);
	}
}

/**********************************************************************************************//**
 * Sets alarm limit ranges hfo
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::setAlarmLimitRanges_HFO()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		//Range_NEONATAL;
		//MVmax
		ALIMIT_MVMAX->setMaxValue(MAXRANGE_HF_NEONATAL_MVMAX);

		int iMVmin=ALIMIT_MVMIN->getCurValue();
		if(iMVmin<=260)
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+10);
		}
		else if(iMVmin<=500)
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+20);
		}
		else if(iMVmin<=1000)
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+50);
		}
		else
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+100);
		}

		//MVmin
		ALIMIT_MVMIN->setMinValue(MINRANGE_HF_NEONATAL_MVMIN);

		int iMVmax=ALIMIT_MVMAX->getCurValue();
		if(iMVmax<=260)
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-10);
		}
		else if(iMVmax<=500)
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-20);
		}
		else if(iMVmax<=1000)
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-50);
		}
		else
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-100);
		}
		
		//DCO2max
		ALIMIT_DCO2MAX->setMaxValue(MAXRANGE_HF_NEONATAL_DCO2MAX);

		int iDCO2min=ALIMIT_DCO2MIN->getCurValue();
		if(iDCO2min<=200)
		{
			ALIMIT_DCO2MAX->setMinValue(iDCO2min+1);
		}
		else if(iDCO2min<=500)
		{
			ALIMIT_DCO2MAX->setMinValue(iDCO2min+5);
		}
		else if(iDCO2min<=1000)
		{
			ALIMIT_DCO2MAX->setMinValue(iDCO2min+10);
		}
		else
		{
			ALIMIT_DCO2MAX->setMinValue(iDCO2min+100);
		}

		//DCO2min
		ALIMIT_DCO2MIN->setMinValue(MINRANGE_HF_NEONATAL_DCO2MIN);

		int iDCO2max=ALIMIT_DCO2MAX->getCurValue();
		if(iDCO2max<=200)
		{
			ALIMIT_DCO2MIN->setMaxValue(iDCO2max-1);
		}
		else if(iDCO2max<=500)
		{
			ALIMIT_DCO2MIN->setMaxValue(iDCO2max-5);
		}
		else if(iDCO2max<=1000)
		{
			ALIMIT_DCO2MIN->setMaxValue(iDCO2max-10);
		}
		else
		{
			ALIMIT_DCO2MIN->setMaxValue(iDCO2max-100);
		}


		//MAPmax
		ALIMIT_MAPMAX->setMaxValue(MAXRANGE_HF_NEONATAL_MAPMAX);
		ALIMIT_MAPMAX->setMinValue(ALIMIT_MAPMIN->getCurValue()+DIFF_MAPMAX_MAPMIN_LIMIT);


		//MAPmin
		ALIMIT_MAPMIN->setMaxValue(ALIMIT_MAPMAX->getCurValue()-DIFF_MAPMAX_MAPMIN_LIMIT);
		ALIMIT_MAPMIN->setMinValue(MINRANGE_HF_NEONATAL_MAPMIN);

		

		//Pmax
		ALIMIT_PIPMAX->setMaxValue(MAXRANGE_HF_NEONATAL_PMAX);
		ALIMIT_PIPMAX->setMinValue(ALIMIT_PEEPMIN->getCurValue()+DIFF_PMAX_PMIN_LIMIT);

		//PEEP
		ALIMIT_PEEPMIN->setMaxValue(ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT);
		ALIMIT_PEEPMIN->setMinValue(MINRANGE_HF_NEONATAL_PEEPMIN);
		

		//BPM
		ALIMIT_FREQMAX->setMaxValue(MAXRANGE_HF_NEONATAL_BPMMAX);
		ALIMIT_FREQMAX->setMinValue(MINRANGE_HF_NEONATAL_BPMMIN);

		//Leak
		ALIMIT_LEAKMAX->setMaxValue(MAXRANGE_HF_NEONATAL_LEAKMAX);
		ALIMIT_LEAKMAX->setMinValue(MINRANGE_HF_NEONATAL_LEAKMIN);

		//Apnoe
		ALIMIT_APNOE->setMaxValue(MAXRANGE_HF_NEONATAL_APNOE);
		ALIMIT_APNOE->setMinValue(MINRANGE_HF_NEONATAL_APNOE);
	}
	else
	{
		//PEDIATRIC;
		//MVmax
		ALIMIT_MVMAX->setMaxValue(MAXRANGE_HF_PEDIATRIC_MVMAX);

		int iMVmin=ALIMIT_MVMIN->getCurValue();
		if(iMVmin<=260)
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+10);
		}
		else if(iMVmin<=500)
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+20);
		}
		else if(iMVmin<=1000)
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+50);
		}
		else
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+100);
		}

		//MVmin
		ALIMIT_MVMIN->setMinValue(MINRANGE_HF_PEDIATRIC_MVMIN);

		int iMVmax=ALIMIT_MVMAX->getCurValue();
		if(iMVmax<=260)
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-10);
		}
		else if(iMVmax<=500)
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-20);
		}
		else if(iMVmax<=1000)
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-50);
		}
		else
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-100);
		}
		
		//DCO2max
		ALIMIT_DCO2MAX->setMaxValue(MAXRANGE_HF_PEDIATRIC_DCO2MAX);

		int iDCO2min=ALIMIT_DCO2MIN->getCurValue();
		if(iDCO2min<=200)
		{
			ALIMIT_DCO2MAX->setMinValue(iDCO2min+1);
		}
		else if(iDCO2min<=500)
		{
			ALIMIT_DCO2MAX->setMinValue(iDCO2min+5);
		}
		else if(iDCO2min<=1000)
		{
			ALIMIT_DCO2MAX->setMinValue(iDCO2min+10);
		}
		else
		{
			ALIMIT_DCO2MAX->setMinValue(iDCO2min+100);
		}

		//DCO2min
		ALIMIT_DCO2MIN->setMinValue(MINRANGE_HF_PEDIATRIC_DCO2MIN);

		int iDCO2max=ALIMIT_DCO2MAX->getCurValue();
		if(iDCO2max<=200)
		{
			ALIMIT_DCO2MIN->setMaxValue(iDCO2max-1);
		}
		else if(iDCO2max<=500)
		{
			ALIMIT_DCO2MIN->setMaxValue(iDCO2max-5);
		}
		else if(iDCO2max<=1000)
		{
			ALIMIT_DCO2MIN->setMaxValue(iDCO2max-10);
		}
		else
		{
			ALIMIT_DCO2MIN->setMaxValue(iDCO2max-100);
		}

		//MAPmax
		ALIMIT_MAPMAX->setMaxValue(MAXRANGE_HF_PEDIATRIC_MAPMAX);
		ALIMIT_MAPMAX->setMinValue(ALIMIT_MAPMIN->getCurValue()+DIFF_MAPMAX_MAPMIN_LIMIT);


		//MAPmin
		ALIMIT_MAPMIN->setMaxValue(ALIMIT_MAPMAX->getCurValue()-DIFF_MAPMAX_MAPMIN_LIMIT);
		ALIMIT_MAPMIN->setMinValue(MINRANGE_HF_PEDIATRIC_MAPMIN);


		//Pmax
		ALIMIT_PIPMAX->setMaxValue(MAXRANGE_HF_PEDIATRIC_PMAX);
		ALIMIT_PIPMAX->setMinValue(ALIMIT_PEEPMIN->getCurValue()+DIFF_PMAX_PMIN_LIMIT);

		//PEEP
		ALIMIT_PEEPMIN->setMaxValue(ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT);
		ALIMIT_PEEPMIN->setMinValue(MINRANGE_HF_PEDIATRIC_PEEPMIN);

		//BPM
		ALIMIT_FREQMAX->setMaxValue(MAXRANGE_HF_PEDIATRIC_BPMMAX);
		ALIMIT_FREQMAX->setMinValue(MINRANGE_HF_PEDIATRIC_BPMMIN);

		//Leak
		ALIMIT_LEAKMAX->setMaxValue(MAXRANGE_HF_PEDIATRIC_LEAKMAX);
		ALIMIT_LEAKMAX->setMinValue(MINRANGE_HF_PEDIATRIC_LEAKMIN);

		//Apnoe
		ALIMIT_APNOE->setMaxValue(MAXRANGE_HF_PEDIATRIC_APNOE);
		ALIMIT_APNOE->setMinValue(MINRANGE_HF_PEDIATRIC_APNOE);
	}
}

/**********************************************************************************************//**
 * Sets alarm limit ranges cpap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::setAlarmLimitRanges_CPAP()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		//Range_NEONATAL;
		//MVmax
		ALIMIT_MVMAX->setMaxValue(MAXRANGE_NEONATAL_MVMAX);

		int iMVmin=ALIMIT_MVMIN->getCurValue();
		if(iMVmin<=260)
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+10);
		}
		else if(iMVmin<=500)
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+20);
		}
		else if(iMVmin<=1000)
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+50);
		}
		else
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+100);
		}

		//MVmin
		ALIMIT_MVMIN->setMinValue(MINRANGE_NEONATAL_MVMIN);

		int iMVmax=ALIMIT_MVMAX->getCurValue();
		if(iMVmax<=260)
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-10);
		}
		else if(iMVmax<=500)
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-20);
		}
		else if(iMVmax<=1000)
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-50);
		}
		else
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-100);
		}

		//Pmax
		ALIMIT_PIPMAX->setMaxValue(MAXRANGE_NEONATAL_CPAPPEEPMAX);
		ALIMIT_PIPMAX->setMinValue(ALIMIT_PEEPMIN->getCurValue()+DIFF_PMAX_PMIN_LIMIT);

		//PEEP
		ALIMIT_PEEPMIN->setMaxValue(ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT);
		ALIMIT_PEEPMIN->setMinValue(MINRANGE_NEONATAL_CPAPPEEPMIN);


		//BPM
		ALIMIT_FREQMAX->setMaxValue(MAXRANGE_NEONATAL_BPMMAX);
		ALIMIT_FREQMAX->setMinValue(MINRANGE_NEONATAL_BPMMIN);

		//Leak
		ALIMIT_LEAKMAX->setMaxValue(MAXRANGE_NEONATAL_LEAKMAX);
		ALIMIT_LEAKMAX->setMinValue(MINRANGE_NEONATAL_LEAKMIN);

		//Apnoe
		ALIMIT_APNOE->setMaxValue(MAXRANGE_NEONATAL_APNOE);
		ALIMIT_APNOE->setMinValue(MINRANGE_NEONATAL_APNOE);
	}
	else
	{
		//PEDIATRIC;
		//MVmax
		ALIMIT_MVMAX->setMaxValue(MAXRANGE_PEDIATRIC_MVMAX);

		int iMVmin=ALIMIT_MVMIN->getCurValue();
		if(iMVmin<=260)
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+10);
		}
		else if(iMVmin<=500)
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+20);
		}
		else if(iMVmin<=1000)
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+50);
		}
		else
		{
			ALIMIT_MVMAX->setMinValue(iMVmin+100);
		}

		//MVmin
		ALIMIT_MVMIN->setMinValue(MINRANGE_PEDIATRIC_MVMIN);

		int iMVmax=ALIMIT_MVMAX->getCurValue();
		if(iMVmax<=260)
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-10);
		}
		else if(iMVmax<=500)
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-20);
		}
		else if(iMVmax<=1000)
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-50);
		}
		else
		{
			ALIMIT_MVMIN->setMaxValue(iMVmax-100);
		}

		//Pmax
		ALIMIT_PIPMAX->setMaxValue(MAXRANGE_PEDIATRIC_PMAX);
		ALIMIT_PIPMAX->setMinValue(ALIMIT_PEEPMIN->getCurValue()+DIFF_PMAX_PMIN_LIMIT);

		//PEEP
		ALIMIT_PEEPMIN->setMaxValue(ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT);
		ALIMIT_PEEPMIN->setMinValue(MINRANGE_PEDIATRIC_CPAPPEEPMIN);


		//BPM
		ALIMIT_FREQMAX->setMaxValue(MAXRANGE_PEDIATRIC_BPMMAX);
		ALIMIT_FREQMAX->setMinValue(MINRANGE_PEDIATRIC_BPMMIN);

		//Leak
		ALIMIT_LEAKMAX->setMaxValue(MAXRANGE_PEDIATRIC_LEAKMAX);
		ALIMIT_LEAKMAX->setMinValue(MINRANGE_PEDIATRIC_LEAKMIN);

		//Apnoe
		ALIMIT_APNOE->setMaxValue(MAXRANGE_PEDIATRIC_APNOE);
		ALIMIT_APNOE->setMinValue(MINRANGE_PEDIATRIC_APNOE);
	}
}

/**********************************************************************************************//**
 * Sets alarm limit ranges ncpap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::setAlarmLimitRanges_NCPAP()
{//rku PIPLOW DUOPAP
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		//Pmax
		ALIMIT_PIPMAX->setMaxValue(MAXRANGE_NEONATAL_NMODEMAX);
		ALIMIT_PIPMAX->setMinValue(ALIMIT_PEEPMIN->getCurValue()+DIFF_PMAX_PMIN_LIMIT);

		//PEEP
		ALIMIT_PEEPMIN->setMaxValue(ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT);
		ALIMIT_PEEPMIN->setMinValue(MINRANGE_NEONATAL_NMODEPEEPMIN);

		//Apnoe
		ALIMIT_APNOE->setMaxValue(MAXRANGE_NEONATAL_APNOE);
		ALIMIT_APNOE->setMinValue(MINRANGE_NEONATAL_APNOE);

		//BPM
		ALIMIT_FREQMAX->setMaxValue(MAXRANGE_NEONATAL_BPMMAX);
		ALIMIT_FREQMAX->setMinValue(MINRANGE_NEONATAL_BPMMIN);
	}
	else
	{
		//Pmax
		ALIMIT_PIPMAX->setMaxValue(MAXRANGE_PEDIATRIC_NMODEMAX);
		ALIMIT_PIPMAX->setMinValue(ALIMIT_PEEPMIN->getCurValue()+DIFF_PMAX_PMIN_LIMIT);

		//PEEP
		ALIMIT_PEEPMIN->setMaxValue(ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT);
		ALIMIT_PEEPMIN->setMinValue(MINRANGE_PEDIATRIC_NMODEPEEPMIN);

		//Apnoe
		ALIMIT_APNOE->setMaxValue(MAXRANGE_PEDIATRIC_APNOE);
		ALIMIT_APNOE->setMinValue(MINRANGE_PEDIATRIC_APNOE);

		//BPM
		ALIMIT_FREQMAX->setMaxValue(MAXRANGE_PEDIATRIC_BPMMAX);
		ALIMIT_FREQMAX->setMinValue(MINRANGE_PEDIATRIC_BPMMIN);
	}
}

/**********************************************************************************************//**
 * Sets alarm limit ranges duopap
 *
 * \author	Rainer Kühner
 * \date	05.03.2018
 **************************************************************************************************/

void CAlarmConditionHandler::setAlarmLimitRanges_DUOPAP()//rku PIPLOW DUOPAP
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		//Pmax
		int iCurPEEP=ALIMIT_PEEPMIN->getCurValue();
		if(iCurPEEP%10!=0)
		{
			iCurPEEP+=5;
		}

		ALIMIT_PIPMAX->setMaxValue(MAXRANGE_NEONATAL_NMODEMAX);

		if (ALIMIT_PIPMIN->getLimitState()==AL_OFF)
		{
			DEBUGMSG(TRUE, (TEXT("3 PIPMAX->setMinValue %d\r\n"),iCurPEEP+DIFF_PMAX_PMIN_LIMIT));
			ALIMIT_PIPMAX->setMinValue(iCurPEEP+DIFF_PMAX_PMIN_LIMIT);
		}
		else
		{
			DEBUGMSG(TRUE, (TEXT("4 PIPMAX->setMinValue %d\r\n"),ALIMIT_PIPMIN->getCurValue()+DIFF_PMAX_PMIN_LIMIT));
			ALIMIT_PIPMAX->setMinValue(ALIMIT_PIPMIN->getCurValue()+DIFF_PMAX_PMIN_LIMIT);
		}


		ALIMIT_PIPMIN->setMaxValue(ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT);

		//DEBUGMSG(TRUE, (TEXT("ALIMIT_PIPMIN->setMinValue %d\r\n"),iCurPEEP));//rku newPIPOFF
		ALIMIT_PIPMIN->setMinValue(iCurPEEP);

		//PEEP
		if (ALIMIT_PIPMIN->getLimitState()==AL_OFF)//rku newPIPOFF
		{
			//DEBUGMSG(TRUE, (TEXT("ALIMIT_PEEPMIN->setMaxValue %d\r\n"),ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT));
			ALIMIT_PEEPMIN->setMaxValue(ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT);
		}
		else
		{
			//DEBUGMSG(TRUE, (TEXT("ALIMIT_PEEPMIN->setMaxValue %d\r\n"),ALIMIT_PIPMIN->getCurValue()));
			ALIMIT_PEEPMIN->setMaxValue(ALIMIT_PIPMIN->getCurValue());
		}
		ALIMIT_PEEPMIN->setMinValue(MINRANGE_NEONATAL_NMODEPEEPMIN);




		//Apnoe
		ALIMIT_APNOE->setMaxValue(MAXRANGE_NEONATAL_APNOE);
		ALIMIT_APNOE->setMinValue(MINRANGE_NEONATAL_APNOE);

		//BPM
		ALIMIT_FREQMAX->setMaxValue(MAXRANGE_NEONATAL_BPMMAX);
		ALIMIT_FREQMAX->setMinValue(MINRANGE_NEONATAL_BPMMIN);
	}
	else
	{
		//Pmax
		int iCurPEEP=ALIMIT_PEEPMIN->getCurValue();
		if(iCurPEEP%10!=0)
		{
			iCurPEEP+=5;
		}

		ALIMIT_PIPMAX->setMaxValue(MAXRANGE_PEDIATRIC_NMODEMAX);

		if (ALIMIT_PIPMIN->getLimitState()==AL_OFF)
		{
			ALIMIT_PIPMAX->setMinValue(iCurPEEP+DIFF_PMAX_PMIN_LIMIT);
		}
		else
		{
			ALIMIT_PIPMAX->setMinValue(ALIMIT_PIPMIN->getCurValue()+DIFF_PMAX_PMIN_LIMIT);
		}


		ALIMIT_PIPMIN->setMaxValue(ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT);

		//DEBUGMSG(TRUE, (TEXT("ALIMIT_PIPMIN->setMinValue %d\r\n"),iCurPEEP));//rku newPIPOFF
		ALIMIT_PIPMIN->setMinValue(iCurPEEP);

		//PEEP
		if (ALIMIT_PIPMIN->getLimitState()==AL_OFF)//rku newPIPOFF
		{
			//DEBUGMSG(TRUE, (TEXT("ALIMIT_PEEPMIN->setMaxValue %d\r\n"),ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT));
			ALIMIT_PEEPMIN->setMaxValue(ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT);
		}
		else
		{
			//DEBUGMSG(TRUE, (TEXT("ALIMIT_PEEPMIN->setMaxValue %d\r\n"),ALIMIT_PIPMIN->getCurValue()));
			ALIMIT_PEEPMIN->setMaxValue(ALIMIT_PIPMIN->getCurValue());
		}
		ALIMIT_PEEPMIN->setMinValue(MINRANGE_PEDIATRIC_NMODEPEEPMIN);

		//Apnoe
		ALIMIT_APNOE->setMaxValue(MAXRANGE_PEDIATRIC_APNOE);
		ALIMIT_APNOE->setMinValue(MINRANGE_PEDIATRIC_APNOE);

		//BPM
		ALIMIT_FREQMAX->setMaxValue(MAXRANGE_PEDIATRIC_BPMMAX);
		ALIMIT_FREQMAX->setMinValue(MINRANGE_PEDIATRIC_BPMMIN);
	}
}
/**********************************************************************************************//**
 * Sets alarm limit ranges etco2
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::setAlarmLimitRanges_ETCO2()
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();

	eCO2unit unit=getModel()->getCONFIG()->GetCO2unit();

	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		if(eActiveVentMode==VM_HFO)
		{
			//ETCO2max
			switch(unit)
			{
			case CO2_UNIT_KPA:
				{
					ALIMIT_ETCO2MAX->setMaxValue(MAXRANGE_HF_NEONATAL_ETCO2MAX_KPA);
					ALIMIT_ETCO2MIN->setMinValue(MINRANGE_HF_NEONATAL_ETCO2MIN_KPA);

					ALIMIT_FICO2MAX->setMaxValue(MAXRANGE_HF_NEONATAL_FICO2MAX_KPA);
					ALIMIT_FICO2MIN->setMinValue(MINRANGE_HF_NEONATAL_FICO2MIN_KPA);
				}
				break;
			case CO2_UNIT_PERCENT:
				{
					ALIMIT_ETCO2MAX->setMaxValue(MAXRANGE_HF_NEONATAL_ETCO2MAX_VOL);
					ALIMIT_ETCO2MIN->setMinValue(MINRANGE_HF_NEONATAL_ETCO2MIN_VOL);

					ALIMIT_FICO2MAX->setMaxValue(MAXRANGE_HF_NEONATAL_FICO2MAX_VOL);
					ALIMIT_FICO2MIN->setMinValue(MINRANGE_HF_NEONATAL_FICO2MIN_VOL);
				}
				break;
			case CO2_UNIT_MMHG:
			default:
				{
					ALIMIT_ETCO2MAX->setMaxValue(MAXRANGE_HF_NEONATAL_ETCO2MAX_MMHG);
					ALIMIT_ETCO2MIN->setMinValue(MINRANGE_HF_NEONATAL_ETCO2MIN_MMHG);

					ALIMIT_FICO2MAX->setMaxValue(MAXRANGE_HF_NEONATAL_FICO2MAX_MMHG);
					ALIMIT_FICO2MIN->setMinValue(MINRANGE_HF_NEONATAL_FICO2MIN_MMHG);
				}
				break;
			}
			ALIMIT_ETCO2MAX->setMinValue(ALIMIT_ETCO2MIN->getCurValue()+20);

			//ETCO2min
			ALIMIT_ETCO2MIN->setMaxValue(ALIMIT_ETCO2MAX->getCurValue()-20);

			//FiO2
			ALIMIT_FICO2MAX->setMinValue(ALIMIT_FICO2MIN->getCurValue()+20);
			ALIMIT_FICO2MIN->setMaxValue(ALIMIT_FICO2MAX->getCurValue()-20);
		}
		else
		{
			//ETCO2max
			ALIMIT_ETCO2MAX->setMaxValue(MAXRANGE_NEONATAL_ETCO2MAX);
			ALIMIT_ETCO2MAX->setMinValue(ALIMIT_ETCO2MIN->getCurValue()+20);

			//ETCO2min
			ALIMIT_ETCO2MIN->setMaxValue(ALIMIT_ETCO2MAX->getCurValue()-20);
			ALIMIT_ETCO2MIN->setMinValue(MINRANGE_NEONATAL_ETCO2MIN);


			//FICO2max
			ALIMIT_FICO2MAX->setMaxValue(MAXRANGE_NEONATAL_FICO2MAX);
			ALIMIT_FICO2MAX->setMinValue(ALIMIT_FICO2MIN->getCurValue()+20);

			//FICO2min
			ALIMIT_FICO2MIN->setMaxValue(ALIMIT_FICO2MAX->getCurValue()-20);
			ALIMIT_FICO2MIN->setMinValue(MINRANGE_NEONATAL_FICO2MIN);
		}
	}
	else
	{
		if(eActiveVentMode==VM_HFO)
		{
			//ETCO2max
			switch(unit)
			{
			case CO2_UNIT_KPA:
				{
					ALIMIT_ETCO2MAX->setMaxValue(MAXRANGE_HF_PEDIATRIC_ETCO2MAX_KPA);
					ALIMIT_ETCO2MIN->setMinValue(MINRANGE_HF_PEDIATRIC_ETCO2MIN_KPA);

					ALIMIT_FICO2MAX->setMaxValue(MAXRANGE_HF_PEDIATRIC_FICO2MAX_KPA);
					ALIMIT_FICO2MIN->setMinValue(MINRANGE_HF_PEDIATRIC_FICO2MIN_KPA);
				}
				break;
			case CO2_UNIT_PERCENT:
				{
					ALIMIT_ETCO2MAX->setMaxValue(MAXRANGE_HF_PEDIATRIC_ETCO2MAX_VOL);
					ALIMIT_ETCO2MIN->setMinValue(MINRANGE_HF_PEDIATRIC_ETCO2MIN_VOL);

					ALIMIT_FICO2MAX->setMaxValue(MAXRANGE_HF_PEDIATRIC_FICO2MAX_VOL);
					ALIMIT_FICO2MIN->setMinValue(MINRANGE_HF_PEDIATRIC_FICO2MIN_VOL);
				}
				break;
			case CO2_UNIT_MMHG:
			default:
				{
					ALIMIT_ETCO2MAX->setMaxValue(MAXRANGE_HF_PEDIATRIC_ETCO2MAX_MMHG);
					ALIMIT_ETCO2MIN->setMinValue(MINRANGE_HF_PEDIATRIC_ETCO2MIN_MMHG);

					ALIMIT_FICO2MAX->setMaxValue(MAXRANGE_HF_PEDIATRIC_FICO2MAX_MMHG);
					ALIMIT_FICO2MIN->setMinValue(MINRANGE_HF_PEDIATRIC_FICO2MIN_MMHG);
				}
				break;
			}
			ALIMIT_ETCO2MAX->setMinValue(ALIMIT_ETCO2MIN->getCurValue()+20);

			//ETCO2min
			ALIMIT_ETCO2MIN->setMaxValue(ALIMIT_ETCO2MAX->getCurValue()-20);

			//FICO2
			ALIMIT_FICO2MAX->setMinValue(ALIMIT_FICO2MIN->getCurValue()+20);
			ALIMIT_FICO2MIN->setMaxValue(ALIMIT_FICO2MAX->getCurValue()-20);
		}
		else
		{
			//ETCO2max
			ALIMIT_ETCO2MAX->setMaxValue(MAXRANGE_PEDIATRIC_ETCO2MAX);
			ALIMIT_ETCO2MAX->setMinValue(ALIMIT_ETCO2MIN->getCurValue()+20);

			//ETCO2min
			ALIMIT_ETCO2MIN->setMaxValue(ALIMIT_ETCO2MAX->getCurValue()-20);
			ALIMIT_ETCO2MIN->setMinValue(MINRANGE_PEDIATRIC_ETCO2MIN);


			//ETCO2max
			ALIMIT_FICO2MAX->setMaxValue(MAXRANGE_PEDIATRIC_FICO2MAX);
			ALIMIT_FICO2MAX->setMinValue(ALIMIT_FICO2MIN->getCurValue()+20);

			//ETCO2min
			ALIMIT_FICO2MIN->setMaxValue(ALIMIT_FICO2MAX->getCurValue()-20);
			ALIMIT_FICO2MIN->setMinValue(MINRANGE_PEDIATRIC_FICO2MIN);
		}
	}
}

/**********************************************************************************************//**
 * Sets alarm limit ranges spo2
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::setAlarmLimitRanges_SPO2()
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		if(eActiveVentMode==VM_HFO)
		{
			//SPO2max
			ALIMIT_SPO2MAX->setMaxValue(MAXRANGE_HF_NEONATAL_SPO2MAX);
			ALIMIT_SPO2MAX->setMinValue(ALIMIT_SPO2MIN->getCurValue()+20);

			//SPO2min
			ALIMIT_SPO2MIN->setMaxValue(ALIMIT_SPO2MAX->getCurValue()-20);
			ALIMIT_SPO2MIN->setMinValue(MINRANGE_HF_NEONATAL_SPO2MIN);

			//PulseRatemax
			ALIMIT_PULSERATEMAX->setMaxValue(MAXRANGE_HF_NEONATAL_PULSERATEMAX);
			ALIMIT_PULSERATEMAX->setMinValue(ALIMIT_PULSERATEMIN->getCurValue()+5);

			//PulseRatemin
			ALIMIT_PULSERATEMIN->setMaxValue(ALIMIT_PULSERATEMAX->getCurValue()-5);
			ALIMIT_PULSERATEMIN->setMinValue(MINRANGE_HF_NEONATAL_PULSERATEMIN);

			//SPO2_PImin
			ALIMIT_SPO2PIMIN->setMaxValue(MAXRANGE_HF_NEONATAL_SPO2_PIMIN);
			ALIMIT_SPO2PIMIN->setMinValue(MINRANGE_HF_NEONATAL_SPO2_PIMIN);

			//SPO2_SIQmin
			ALIMIT_SPO2SIQMIN->setMaxValue(MAXRANGE_HF_NEONATAL_SPO2_SIQMIN);
			ALIMIT_SPO2SIQMIN->setMinValue(MINRANGE_HF_NEONATAL_SPO2_SIQMIN);
		}
		else
		{
			//SPO2max
			ALIMIT_SPO2MAX->setMaxValue(MAXRANGE_NEONATAL_SPO2MAX);
			ALIMIT_SPO2MAX->setMinValue(ALIMIT_SPO2MIN->getCurValue()+20);

			//SPO2min
			ALIMIT_SPO2MIN->setMaxValue(ALIMIT_SPO2MAX->getCurValue()-20);
			ALIMIT_SPO2MIN->setMinValue(MINRANGE_NEONATAL_SPO2MIN);

			//PulseRatemax
			ALIMIT_PULSERATEMAX->setMaxValue(MAXRANGE_NEONATAL_PULSERATEMAX);
			ALIMIT_PULSERATEMAX->setMinValue(ALIMIT_PULSERATEMIN->getCurValue()+5);

			//PulseRatemin
			ALIMIT_PULSERATEMIN->setMaxValue(ALIMIT_PULSERATEMAX->getCurValue()-5);
			ALIMIT_PULSERATEMIN->setMinValue(MINRANGE_NEONATAL_PULSERATEMIN);

			//SPO2_PImin
			ALIMIT_SPO2PIMIN->setMaxValue(MAXRANGE_NEONATAL_SPO2_PIMIN);
			ALIMIT_SPO2PIMIN->setMinValue(MINRANGE_NEONATAL_SPO2_PIMIN);

			//SPO2_SIQmin
			ALIMIT_SPO2SIQMIN->setMaxValue(MAXRANGE_NEONATAL_SPO2_SIQMIN);
			ALIMIT_SPO2SIQMIN->setMinValue(MINRANGE_NEONATAL_SPO2_SIQMIN);
		}
	}
	else
	{
		if(eActiveVentMode==VM_HFO)
		{
			//SPO2max
			ALIMIT_SPO2MAX->setMaxValue(MAXRANGE_HF_PEDIATRIC_SPO2MAX);
			ALIMIT_SPO2MAX->setMinValue(ALIMIT_SPO2MIN->getCurValue()+20);

			//SPO2min
			ALIMIT_SPO2MIN->setMaxValue(ALIMIT_SPO2MAX->getCurValue()-20);
			ALIMIT_SPO2MIN->setMinValue(MINRANGE_HF_PEDIATRIC_SPO2MIN);

			//PulseRatemax
			ALIMIT_PULSERATEMAX->setMaxValue(MAXRANGE_HF_PEDIATRIC_PULSERATEMAX);
			ALIMIT_PULSERATEMAX->setMinValue(ALIMIT_PULSERATEMIN->getCurValue()+5);

			//PulseRatemin
			ALIMIT_PULSERATEMIN->setMaxValue(ALIMIT_PULSERATEMAX->getCurValue()-5);
			ALIMIT_PULSERATEMIN->setMinValue(MINRANGE_HF_PEDIATRIC_PULSERATEMIN);

			//SPO2_PImin
			ALIMIT_SPO2PIMIN->setMaxValue(MAXRANGE_HF_PEDIATRIC_SPO2_PIMIN);
			ALIMIT_SPO2PIMIN->setMinValue(MINRANGE_HF_PEDIATRIC_SPO2_PIMIN);

			//SPO2_SIQmin
			ALIMIT_SPO2SIQMIN->setMaxValue(MAXRANGE_HF_PEDIATRIC_SPO2_SIQMIN);
			ALIMIT_SPO2SIQMIN->setMinValue(MINRANGE_HF_PEDIATRIC_SPO2_SIQMIN);
		}
		else
		{
			//SPO2max
			ALIMIT_SPO2MAX->setMaxValue(MAXRANGE_PEDIATRIC_SPO2MAX);
			ALIMIT_SPO2MAX->setMinValue(ALIMIT_SPO2MIN->getCurValue()+20);
			
			//SPO2min
			ALIMIT_SPO2MIN->setMaxValue(ALIMIT_SPO2MAX->getCurValue()-20);
			ALIMIT_SPO2MIN->setMinValue(MINRANGE_PEDIATRIC_SPO2MIN);

			//PulseRatemax
			ALIMIT_PULSERATEMAX->setMaxValue(MAXRANGE_PEDIATRIC_PULSERATEMAX);
			ALIMIT_PULSERATEMAX->setMinValue(ALIMIT_PULSERATEMIN->getCurValue()+5);

			//PulseRatemin
			ALIMIT_PULSERATEMIN->setMaxValue(ALIMIT_PULSERATEMAX->getCurValue()-5);
			ALIMIT_PULSERATEMIN->setMinValue(MINRANGE_PEDIATRIC_PULSERATEMIN);

			//SPO2_PImin
			ALIMIT_SPO2PIMIN->setMaxValue(MAXRANGE_PEDIATRIC_SPO2_PIMIN);
			ALIMIT_SPO2PIMIN->setMinValue(MINRANGE_PEDIATRIC_SPO2_PIMIN);

			//SPO2_SIQmin
			ALIMIT_SPO2SIQMIN->setMaxValue(MAXRANGE_PEDIATRIC_SPO2_SIQMIN);
			ALIMIT_SPO2SIQMIN->setMinValue(MINRANGE_PEDIATRIC_SPO2_SIQMIN);
		}
	}
}

/**********************************************************************************************//**
 * Gets alimit state mvmax limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state mvmax limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_MVmaxLimit()
{
	return ALIMIT_MVMAX->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state mvmin limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state mvmin limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_MVminLimit()
{
	return ALIMIT_MVMIN->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state pipmax limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state pipmax limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_PIPmaxLimit()
{
	return ALIMIT_PIPMAX->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state pipmin limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state pipmin limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_PIPminLimit()
{
	return ALIMIT_PIPMIN->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state peepmin limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state peepmin limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_PEEPminLimit()
{
	return ALIMIT_PEEPMIN->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state bpmmax limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state bpmmax limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_BPMmaxLimit()
{
	return ALIMIT_FREQMAX->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state leakmax limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state leakmax limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_LeakmaxLimit()
{
	return ALIMIT_LEAKMAX->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state apnoe limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state apnoe limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_ApnoeLimit()
{
	return ALIMIT_APNOE->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state mapmax limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state mapmax limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_MAPmaxLimit()
{
	return ALIMIT_MAPMAX->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state mapmin limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state mapmin limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_MAPminLimit()
{
	return ALIMIT_MAPMIN->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state dco2max limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state dco2max limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_DCO2maxLimit()
{
	return ALIMIT_DCO2MAX->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state dco2min limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state dco2min limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_DCO2minLimit()
{
	return ALIMIT_DCO2MIN->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state etco2max limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state etco2max limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_ETCO2maxLimit()
{
	return ALIMIT_ETCO2MAX->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state etco2min limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state etco2min limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_ETCO2minLimit()
{
	return ALIMIT_ETCO2MIN->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state fico2max limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state fico2max limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_FICO2maxLimit()
{
	return ALIMIT_FICO2MAX->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state fico2min limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state fico2min limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_FICO2minLimit()
{
	return ALIMIT_FICO2MIN->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state spo2max limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state spo2max limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_SPO2maxLimit()
{
	return ALIMIT_SPO2MAX->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state spo2min limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state spo2min limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_SPO2minLimit()
{
	return ALIMIT_SPO2MIN->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state pulseratemax limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state pulseratemax limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_PulseRatemaxLimit()
{
	return ALIMIT_PULSERATEMAX->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state pulseratemin limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state pulseratemin limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_PulseRateminLimit()
{
	return ALIMIT_PULSERATEMIN->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state spo2 pimin limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state spo2 pimin limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_SPO2_PIminLimit()
{
	return ALIMIT_SPO2PIMIN->getLimitState();
}

/**********************************************************************************************//**
 * Gets alimit state spo2 SIqmin limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit state spo2 SIqmin limit.
 **************************************************************************************************/

eAlarmLimitState CAlarmConditionHandler::getAlimitState_SPO2_SIQminLimit()
{
	return ALIMIT_SPO2SIQMIN->getLimitState();
}

/**********************************************************************************************//**
 * Sets alimit apnoe
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitApnoe(int value)
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitApnoeHF(value);
	}
	else
	{
		setAlimitApnoeDefault(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit apnoe hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitApnoeHF(int value)
{
	ALIMIT_APNOE->setCurValue(value);

	CStringW sz=_T("");
	sz.Format(_T("ALApnoeHF%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitApnoeHF(value);
	getModel()->Send_PARA_APNOE_TIME(value,true,true);
}

/**********************************************************************************************//**
 * Sets alimit apnoe default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitApnoeDefault(int value)
{
	ALIMIT_APNOE->setCurValue(value);

	CStringW sz=_T("");
	sz.Format(_T("ALApnoe%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->setAlimitApnoe(value);
	getModel()->Send_PARA_APNOE_TIME(value,true,true);
}

/**********************************************************************************************//**
 * Sets alimit bpmmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitBPMmax(int value)
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitBPMmaxHF(value);
	}
	else
	{
		setAlimitBPMmaxDefault(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit bpmmax hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitBPMmaxHF(int value)
{
	ALIMIT_FREQMAX->setCurValue(value);

	CStringW sz=_T("");
	sz.Format(_T("ALBPMmaxHF%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitBPMmaxHF(value);
}

/**********************************************************************************************//**
 * Sets alimit bpmmax default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitBPMmaxDefault(int value)
{
	ALIMIT_FREQMAX->setCurValue(value);

	CStringW sz=_T("");
	sz.Format(_T("ALBPMmax%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitBPMmax(value);
}

/**********************************************************************************************//**
 * Sets alimit leakmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitLeakmax(int value)
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitLeakmaxHF(value);
	}
	else
	{
		setAlimitLeakmaxDefault(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit leakmax hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitLeakmaxHF(int value)
{
	ALIMIT_LEAKMAX->setCurValue(value);

	CStringW sz=_T("");
	sz.Format(_T("ALLeakmaxHF%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitLeakmaxHF(value);
}

/**********************************************************************************************//**
 * Sets alimit leakmax default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitLeakmaxDefault(int value)
{
	ALIMIT_LEAKMAX->setCurValue(value);

	CStringW sz=_T("");
	sz.Format(_T("ALLeakmax%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitLeakmax(value);
}

/**********************************************************************************************//**
 * Sets alimit mvmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMVmax(int value)
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitMVmaxHF(value);
	}
	else
	{
		setAlimitMVmaxDefault(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit mvmax hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMVmaxHF(int value)
{
	ALIMIT_MVMAX->setCurValue(value);

	if(value<=260)
	{
		ALIMIT_MVMIN->setMaxValue(value-10);
	}
	else if(value<=500)
	{
		ALIMIT_MVMIN->setMaxValue(value-20);
	}
	else if(value<=1000)
	{
		ALIMIT_MVMIN->setMaxValue(value-50);
	}
	else
	{
		ALIMIT_MVMIN->setMaxValue(value-100);
	}

	CStringW sz=_T("");
	sz.Format(_T("ALMVmaxHF%d,"), value);
	theApp.getLog()->WriteLine(sz);

	getModel()->getCONFIG()->SetAlarmlimitMVmaxHF(value);
}

/**********************************************************************************************//**
 * Sets alimit mvmax default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMVmaxDefault(int value)
{
	ALIMIT_MVMAX->setCurValue(value);

	if(value<=260)
	{
		ALIMIT_MVMIN->setMaxValue(value-10);
	}
	else if(value<=500)
	{
		ALIMIT_MVMIN->setMaxValue(value-20);
	}
	else if(value<=1000)
	{
		ALIMIT_MVMIN->setMaxValue(value-50);
	}
	else
	{
		ALIMIT_MVMIN->setMaxValue(value-100);
	}

	CStringW sz=_T("");
	sz.Format(_T("ALMVmax%d,"), value);
	theApp.getLog()->WriteLine(sz);

	getModel()->getCONFIG()->SetAlarmlimitMVmax(value);
}

/**********************************************************************************************//**
 * Sets alimit mvmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMVmin(int value)
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitMVminHF(value);
	}
	else
	{
		setAlimitMVminDefault(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit mvmin hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMVminHF(int value)
{
	ALIMIT_MVMIN->setCurValue(value);

	if(value<=40)
	{
		ALIMIT_MVMAX->setMinValue(50);
	}
	else if(value<=260)
	{
		ALIMIT_MVMAX->setMinValue(value+10);
	}
	else if(value<=500)
	{
		ALIMIT_MVMAX->setMinValue(value+20);
	}
	else if(value<=1000)
	{
		ALIMIT_MVMAX->setMinValue(value+50);
	}
	else
	{
		ALIMIT_MVMAX->setMinValue(value+100);
	}

	CStringW sz=_T("");
	sz.Format(_T("ALMVminHF%d,"), value);
	theApp.getLog()->WriteLine(sz);

	getModel()->getCONFIG()->SetAlarmlimitMVminHF(value);
}

/**********************************************************************************************//**
 * Sets alimit mvmin default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMVminDefault(int value)
{
	ALIMIT_MVMIN->setCurValue(value);

	if(getModel()->getALARMHANDLER()->getAlimitState_MVminLimit() == AL_OFF) //pro - added
	{
		ALIMIT_MVMAX->setMinValue(MINRANGE_NEONATAL_MVMIN);
	}
	else if(value<=260)
	{
		ALIMIT_MVMAX->setMinValue(value+10);
	}
	else if(value<=500)
	{
		ALIMIT_MVMAX->setMinValue(value+20);
	}
	else if(value<=1000)
	{
		ALIMIT_MVMAX->setMinValue(value+50);
	}
	else
	{
		ALIMIT_MVMAX->setMinValue(value+100);
	}

	CStringW sz=_T("");
	sz.Format(_T("ALMVmin%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitMVmin(value);
}

/**********************************************************************************************//**
 * Sets alimit peepmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitPEEPmin(int value)
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_THERAPIE)
	{

	}
	//else if(eActiveVentMode==VM_HFO)
	//{
	//	//setAlarmlimitDataPEEPminHF(value);
	//}
	else if(eActiveVentMode==VM_CPAP)
	{
		setAlimitPEEPminCpap(value);
	}
	else if(eActiveVentMode==VM_NCPAP)
	{
		setAlimitPEEPminNCPAP(value);
	}
	else if(eActiveVentMode==VM_DUOPAP)
	{
		setAlimitPEEPminDUOPAP(value);
	}
	else
	{
		setAlimitPEEPminDefault(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit peepmin cpap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitPEEPminCpap(int value)
{
	ALIMIT_PEEPMIN->setCurValue(value);
	ALIMIT_PIPMAX->setMinValue(value+DIFF_PMAX_PMIN_LIMIT);

	CStringW sz=_T("");
	sz.Format(_T("ALPEEPminCPAP%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitPEEPminCpap(value);
}

/**********************************************************************************************//**
 * Sets alimit peepmin ncpap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitPEEPminNCPAP(int value)
{
	ALIMIT_PEEPMIN->setCurValue(value);
	ALIMIT_PIPMAX->setMinValue(value+DIFF_PMAX_PMIN_LIMIT);

	CStringW sz=_T("");
	sz.Format(_T("ALPEEPminNCPAP%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitPEEPminNCPAP(value);
}

/**********************************************************************************************//**
 * Sets alimit peepmin duopap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitPEEPminDUOPAP(int value)
{
	//rku PIPLOW DUOPAP
	ALIMIT_PEEPMIN->setCurValue(value);

	setAlimitsMinMaxRangeDUOPAP();

	CStringW sz=_T("");
	sz.Format(_T("ALPEEPminDUOPAP%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitPEEPminDUOPAP(value);
}

/**********************************************************************************************//**
 * Sets alimit peepmin default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitPEEPminDefault(int value)//rku PIPLOW DUOPAP
{
	ALIMIT_PEEPMIN->setCurValue(value);

	setAlimitsMinMaxRangePressure();

	CStringW sz=_T("");
	sz.Format(_T("ALPEEPmin%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitPEEPmin(value);
}

/**********************************************************************************************//**
 * Sets alimit pipmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitPIPmax(int value)
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_THERAPIE)
	{

	}
	else if(eActiveVentMode==VM_HFO)
	{
		setAlimitPIPmaxHF(value);
	}
	else if(eActiveVentMode==VM_CPAP)
	{
		setAlimitPIPmaxCpap(value);
	}
	else if(eActiveVentMode==VM_NCPAP)
	{
		setAlimitPIPmaxNCPAP(value);
	}
	else if(eActiveVentMode==VM_DUOPAP)
	{
		setAlimitPIPmaxDUOPAP(value);
	}
	else
	{
		setAlimitPIPmaxDefault(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit pipmax hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitPIPmaxHF(int value)
{
	ALIMIT_PIPMAX->setCurValue(value);
	ALIMIT_PEEPMIN->setMaxValue(value-DIFF_PMAX_PMIN_LIMIT);

	CStringW sz=_T("");
	sz.Format(_T("ALPmaxHF%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitPmaxHF(value);
}

/**********************************************************************************************//**
 * Sets alimit pipmax cpap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitPIPmaxCpap(int value)
{
	ALIMIT_PIPMAX->setCurValue(value);
	ALIMIT_PEEPMIN->setMaxValue(value-DIFF_PMAX_PMIN_LIMIT);

	CStringW sz=_T("");
	sz.Format(_T("ALPmaxCPAP%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitPmaxCpap(value);
}

/**********************************************************************************************//**
 * Sets alimit pipmax ncpap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitPIPmaxNCPAP(int value)
{
	ALIMIT_PIPMAX->setCurValue(value);
	ALIMIT_PEEPMIN->setMaxValue(value-DIFF_PMAX_PMIN_LIMIT);

	CStringW sz=_T("");
	sz.Format(_T("ALPmaxNCPAP%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitPmaxNCPAP(value);
}

/**********************************************************************************************//**
 * Sets alimit pipmax duopap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitPIPmaxDUOPAP(int value)
{
	//rku PIPLOW DUOPAP
	ALIMIT_PIPMAX->setCurValue(value);

	setAlimitsMinMaxRangeDUOPAP();

	CStringW sz=_T("");
	sz.Format(_T("ALPIPmax%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitPIPmaxDUOPAP(value);
}

/**********************************************************************************************//**
 * Sets alimit pipmax default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitPIPmaxDefault(int value)//rku PIPLOW DUOPAP
{
	ALIMIT_PIPMAX->setCurValue(value);

	setAlimitsMinMaxRangePressure();

	CStringW sz=_T("");
	sz.Format(_T("ALPIPmax%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitPIPmax(value);
}

/**********************************************************************************************//**
 * Sets alimit pipmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitPIPmin(int value)
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_THERAPIE)
	{

	}
	else if(eActiveVentMode==VM_HFO)
	{
	}
	else if(eActiveVentMode==VM_CPAP)
	{
	}
	else if(eActiveVentMode==VM_NCPAP)
	{
	}
	else if(eActiveVentMode==VM_DUOPAP)
	{
		setAlimitPIPminDUOPAP(value);//rku PIPLOW DUOPAP
	}
	else
	{
		setAlimitPIPminDefault(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit pipmin default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitPIPminDefault(int value)//rku PIPLOW DUOPAP
{
	ALIMIT_PIPMIN->setCurValue(value);

	setAlimitsMinMaxRangePressure();

	CStringW sz=_T("");
	sz.Format(_T("ALPIPmin%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitPIPmin(value);
}

void CAlarmConditionHandler::setAlimitPIPminDUOPAP(int value)//rku PIPLOW DUOPAP
{
	ALIMIT_PIPMIN->setCurValue(value);

	setAlimitsMinMaxRangeDUOPAP();

	CStringW sz=_T("");
	sz.Format(_T("ALPIPmin%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitPIPminDUOPAP(value);
}
/**********************************************************************************************//**
 * Sets alimit mapmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMAPmax(int value)
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitMAPmaxHF(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit mapmax hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMAPmaxHF(int value)
{
	ALIMIT_MAPMAX->setCurValue(value);
	DEBUGMSG(TRUE, (TEXT("ALIMIT_MAPMAX value %d\r\n"),value));

	ALIMIT_MAPMIN->setMaxValue(value-DIFF_MAPMAX_MAPMIN_LIMIT);
	DEBUGMSG(TRUE, (TEXT("ALIMIT_MAPMIN max value %d\r\n"),value-DIFF_MAPMAX_MAPMIN_LIMIT));


	CStringW sz=_T("");
	sz.Format(_T("ALMAPmaxHF%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitMAPmaxHF(value);
}

/**********************************************************************************************//**
 * Sets alimit mapmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMAPmin(int value)
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitMAPminHF(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit mapmin hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMAPminHF(int value)
{
	ALIMIT_MAPMIN->setCurValue(value);
	DEBUGMSG(TRUE, (TEXT("ALIMIT_MAPMIN value %d\r\n"),value));

	ALIMIT_MAPMAX->setMinValue(value+DIFF_MAPMAX_MAPMIN_LIMIT);
	DEBUGMSG(TRUE, (TEXT("ALIMIT_MAPMAX min value %d\r\n"),value-DIFF_MAPMAX_MAPMIN_LIMIT));

	CStringW sz=_T("");
	sz.Format(_T("ALMAPminHF%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitMAPminHF(value);
}

/**********************************************************************************************//**
 * Sets alimit dco2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitDCO2max(int value)
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitDCO2maxHF(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit dco2max hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitDCO2maxHF(int value)
{
	ALIMIT_DCO2MAX->setCurValue(value);

	if(value<=200)
	{
		ALIMIT_DCO2MIN->setMaxValue(value-1);
	}
	else if(value<=500)
	{
		ALIMIT_DCO2MIN->setMaxValue(value-5);
	}
	else if(value<=1000)
	{
		ALIMIT_DCO2MIN->setMaxValue(value-10);
	}
	else
	{
		ALIMIT_DCO2MIN->setMaxValue(value-100);
	}

	CStringW sz=_T("");
	sz.Format(_T("ALDCO2maxHF%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitDCO2maxHF(value);
}

/**********************************************************************************************//**
 * Sets alimit dco2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitDCO2min(int value)
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitDCO2minHF(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit dco2min hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitDCO2minHF(int value)
{
	ALIMIT_DCO2MIN->setCurValue(value);

	if(value<=200)
	{
		ALIMIT_DCO2MAX->setMinValue(value+1);
	}
	else if(value<=500)
	{
		ALIMIT_DCO2MAX->setMinValue(value+5);
	}
	else if(value<=1000)
	{
		ALIMIT_DCO2MAX->setMinValue(value+10);
	}
	else
	{
		ALIMIT_DCO2MAX->setMinValue(value+100);
	}

	CStringW sz=_T("");
	sz.Format(_T("ALDCO2minHF%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitDCO2minHF(value);
}

/**********************************************************************************************//**
 * Sets alimit etco2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitETCO2max(int value)
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitETCO2maxHF(value);
	}
	else
	{
		setAlimitETCO2maxDefault(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit etco2max hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitETCO2maxHF(int value)
{
	ALIMIT_ETCO2MAX->setCurValue(value);
	ALIMIT_ETCO2MIN->setMaxValue(value-20);

	CStringW sz=_T("");
	sz.Format(_T("ALETCO2maxHF%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitETCO2maxHF(value);
}

/**********************************************************************************************//**
 * Sets alimit etco2max default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitETCO2maxDefault(int value)
{
	ALIMIT_ETCO2MAX->setCurValue(value);
	ALIMIT_ETCO2MIN->setMaxValue(value-20);

	CStringW sz=_T("");
	sz.Format(_T("ALETCO2max%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitETCO2max(value);
}

/**********************************************************************************************//**
 * Sets alimit etco2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitETCO2min(int value)
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitETCO2minHF(value);
	}
	else
	{
		setAlimitETCO2minDefault(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit etco2min hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitETCO2minHF(int value)
{
	ALIMIT_ETCO2MIN->setCurValue(value);
	ALIMIT_ETCO2MAX->setMinValue(value+20);

	CStringW sz=_T("");
	sz.Format(_T("ALETCO2minHF%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitETCO2minHF(value);
}

/**********************************************************************************************//**
 * Sets alimit etco2min default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitETCO2minDefault(int value)
{
	ALIMIT_ETCO2MIN->setCurValue(value);
	ALIMIT_ETCO2MAX->setMinValue(value+20);

	CStringW sz=_T("");
	sz.Format(_T("ALETCO2min%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitETCO2min(value);
}

/**********************************************************************************************//**
 * Sets alimit fico2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitFICO2max(int value)
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitFICO2maxHF(value);
	}
	else
	{
		setAlimitFICO2maxDefault(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit fico2max hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitFICO2maxHF(int value)
{
	ALIMIT_FICO2MAX->setCurValue(value);
	ALIMIT_FICO2MIN->setMaxValue(value-20);

	CStringW sz=_T("");
	sz.Format(_T("ALFICO2maxHF%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitFICO2maxHF(value);
}

/**********************************************************************************************//**
 * Sets alimit fico2max default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitFICO2maxDefault(int value)
{
	ALIMIT_FICO2MAX->setCurValue(value);
	ALIMIT_FICO2MIN->setMaxValue(value-20);

	CStringW sz=_T("");
	sz.Format(_T("ALFICO2max%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitFICO2max(value);
}

/**********************************************************************************************//**
 * Sets alimit fico2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitFICO2min(int value)
{
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitFICO2minHF(value);
	}
	else
	{
		setAlimitFICO2minDefault(value);
	}
}

/**********************************************************************************************//**
 * Sets alimit fico2min hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitFICO2minHF(int value)
{
	ALIMIT_FICO2MIN->setCurValue(value);
	ALIMIT_FICO2MAX->setMinValue(value+20);

	CStringW sz=_T("");
	sz.Format(_T("ALFICO2minHF%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitFICO2minHF(value);
}

/**********************************************************************************************//**
 * Sets alimit fico2min default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitFICO2minDefault(int value)
{
	ALIMIT_FICO2MIN->setCurValue(value);
	ALIMIT_FICO2MAX->setMinValue(value+20);

	CStringW sz=_T("");
	sz.Format(_T("ALFICO2min%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitFICO2min(value);
}

/**********************************************************************************************//**
 * Sets alimit spo2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitSPO2max(int value)
{
	setAlimitSPO2maxDefault(value);
}

/**********************************************************************************************//**
 * Sets alimit spo2max default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitSPO2maxDefault(int value)
{
	ALIMIT_SPO2MAX->setCurValue(value);
	ALIMIT_SPO2MIN->setMaxValue(value-20);

	CStringW sz=_T("");
	sz.Format(_T("ALSPO2max%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitSPO2max(value);
}

/**********************************************************************************************//**
 * Sets alimit spo2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitSPO2min(int value)
{
	setAlimitSPO2minDefault(value);
}

/**********************************************************************************************//**
 * Sets alimit spo2min default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitSPO2minDefault(int value)
{
	ALIMIT_SPO2MIN->setCurValue(value);
	ALIMIT_SPO2MAX->setMinValue(value+20);

	CStringW sz=_T("");
	sz.Format(_T("ALSPO2min%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitSPO2min(value);
}

/**********************************************************************************************//**
 * Sets alimit pulseratemax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitPulseRatemax(int value)
{
	setAlimitPulseRatemaxDefault(value);
}

/**********************************************************************************************//**
 * Sets alimit pulseratemax default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitPulseRatemaxDefault(int value)
{
	//DEBUGMSG(TRUE, (TEXT("YYY setAlimitPulseRatemaxDefault %d\r\n"),value));

	ALIMIT_PULSERATEMAX->setCurValue(value);
	ALIMIT_PULSERATEMIN->setMaxValue(value-5);

	CStringW sz=_T("");
	sz.Format(_T("ALPulseRatemax%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitPulseRatemax(value);
}

/**********************************************************************************************//**
 * Sets alimit pulseratemin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitPulseRatemin(int value)
{
	setAlimitPulseRateminDefault(value);
}

/**********************************************************************************************//**
 * Sets alimit pulseratemin default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitPulseRateminDefault(int value)
{
	//DEBUGMSG(TRUE, (TEXT("YYY setAlimitPulseRateminDefault %d\r\n"),value));
	ALIMIT_PULSERATEMIN->setCurValue(value);
	ALIMIT_PULSERATEMAX->setMinValue(value+5);

	CStringW sz=_T("");
	sz.Format(_T("ALPulseRatemin%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitPulseRatemin(value);
}

/**********************************************************************************************//**
 * Sets alimit spo2 pimin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitSPO2_PImin(int value)
{
	setAlimitSPO2_PIminDefault(value);
}

/**********************************************************************************************//**
 * Sets alimit spo2 pimin default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitSPO2_PIminDefault(int value)
{
	ALIMIT_SPO2PIMIN->setCurValue(value);

	CStringW sz=_T("");
	sz.Format(_T("ALSPO2_PImin%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitSPO2_PImin(value);
}

/**********************************************************************************************//**
 * Sets alimit spo2 SIqmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitSPO2_SIQmin(int value)
{
	setAlimitSPO2_SIQminDefault(value);
}

/**********************************************************************************************//**
 * Sets alimit spo2 SIqmin default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	value	The value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitSPO2_SIQminDefault(int value)
{
	ALIMIT_SPO2SIQMIN->setCurValue(value);

	CStringW sz=_T("");
	sz.Format(_T("ALSPO2_SIQmin%d,"), value);
	theApp.getLog()->WriteLine(sz);
	getModel()->getCONFIG()->SetAlarmlimitSPO2_SIQmin(value);
}

/**********************************************************************************************//**
 * Sets alimits minimum maximum range cpap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitsMinMaxRangeCpap()
{
	int iLI_PIPMAX=ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT;
	int iLI_PEEPMIN=ALIMIT_PEEPMIN->getCurValue()+DIFF_PMAX_PMIN_LIMIT;

	setAlimitMaxRangePEEPmin(iLI_PIPMAX);
	setAlimitMinRangePIPmax(iLI_PEEPMIN);
}

/**********************************************************************************************//**
 * Sets alimits minimum maximum range ncpap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitsMinMaxRangeNCPAP()
{
	int iLI_PIPMAX=ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT;
	int iLI_PEEPMIN=ALIMIT_PEEPMIN->getCurValue()+DIFF_PMAX_PMIN_LIMIT;

	setAlimitMaxRangePEEPmin(iLI_PIPMAX);
	setAlimitMinRangePIPmax(iLI_PEEPMIN);
}

/**********************************************************************************************//**
 * Sets alimits minimum maximum range duopap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitsMinMaxRangeDUOPAP()
{
	//rku PIPLOW DUOPAP
	int iCurPEEP=ALIMIT_PEEPMIN->getCurValue();
	if(iCurPEEP%10!=0)
	{
		iCurPEEP+=5;
	}

	//rku PIPLOW DUOPAP
	if (ALIMIT_PIPMIN->getLimitState()==AL_OFF)
	{
		setAlimitMinRangePIPmax(iCurPEEP+DIFF_PMAX_PMIN_LIMIT);
		setAlimitMaxRangePIPmin(ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT);
		setAlimitMinRangePIPmin(iCurPEEP);
		setAlimitMaxRangePEEPmin(ALIMIT_PIPMAX->getCurValue()-DIFF_MAPMAX_MAPMIN_LIMIT);
	}
	else
	{
		setAlimitMinRangePIPmax(ALIMIT_PIPMIN->getCurValue()+DIFF_PMAX_PMIN_LIMIT);
		setAlimitMaxRangePIPmin(ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT);
		setAlimitMinRangePIPmin(iCurPEEP);
		setAlimitMaxRangePEEPmin(ALIMIT_PIPMIN->getCurValue());
	}
}

/**********************************************************************************************//**
 * Sets alimits minimum maximum range pressure
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitsMinMaxRangePressure()
{
	//rku PIPLOW DUOPAP
	int iCurPEEP=ALIMIT_PEEPMIN->getCurValue();
	if(iCurPEEP%10!=0)
	{
		iCurPEEP+=5;
	}

	//rku PIPLOW DUOPAP
	if (ALIMIT_PIPMIN->getLimitState()==AL_OFF)
	{
		setAlimitMinRangePIPmax(iCurPEEP+DIFF_PMAX_PMIN_LIMIT);
		setAlimitMaxRangePIPmin(ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT);
		setAlimitMinRangePIPmin(iCurPEEP);
		setAlimitMaxRangePEEPmin(ALIMIT_PIPMAX->getCurValue()-DIFF_MAPMAX_MAPMIN_LIMIT);
	}
	else
	{
		setAlimitMinRangePIPmax(ALIMIT_PIPMIN->getCurValue()+DIFF_PMAX_PMIN_LIMIT);
		setAlimitMaxRangePIPmin(ALIMIT_PIPMAX->getCurValue()-DIFF_PMAX_PMIN_LIMIT);
		setAlimitMinRangePIPmin(iCurPEEP);
		setAlimitMaxRangePEEPmin(ALIMIT_PIPMIN->getCurValue());
	}
}

/**********************************************************************************************//**
 * Gets alimit mvmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit mvmin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMVmin()
{
	return ALIMIT_MVMIN->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit mvmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit mvmax.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMVmax()
{
	return ALIMIT_MVMAX->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit pipmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit pipmax.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitPIPmax()
{
	return ALIMIT_PIPMAX->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit pipmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit pipmin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitPIPmin()
{
	return ALIMIT_PIPMIN->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit peepmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit peepmin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitPEEPmin()
{
	return ALIMIT_PEEPMIN->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit leakmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit leakmax.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitLeakmax()
{
	return ALIMIT_LEAKMAX->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit bpmmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit bpmmax.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitBPMmax()
{
	return ALIMIT_FREQMAX->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit apnoe
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit apnoe.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitApnoe()
{
	return ALIMIT_APNOE->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit mapmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit mapmax.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMAPmax()
{
	return ALIMIT_MAPMAX->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit mapmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit mapmin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMAPmin()
{
	return ALIMIT_MAPMIN->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit dco2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit dco2min.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitDCO2min()
{
	return ALIMIT_DCO2MIN->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit dco2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit dco2max.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitDCO2max()
{
	return ALIMIT_DCO2MAX->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit etco2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit etco2min.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitETCO2min()
{
	return ALIMIT_ETCO2MIN->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit etco2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit etco2max.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitETCO2max()
{
	return ALIMIT_ETCO2MAX->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit fico2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit fico2min.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitFICO2min()
{
	return ALIMIT_FICO2MIN->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit fico2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit fico2max.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitFICO2max()
{
	return ALIMIT_FICO2MAX->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit spo2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit spo2max.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitSPO2max()
{
	return ALIMIT_SPO2MAX->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit spo2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit spo2min.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitSPO2min()
{
	return ALIMIT_SPO2MIN->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit pulseratemax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit pulseratemax.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitPulseRatemax()
{
	return ALIMIT_PULSERATEMAX->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit pulseratemin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit pulseratemin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitPulseRatemin()
{
	return ALIMIT_PULSERATEMIN->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit spo2 pimin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit spo2 pimin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitSPO2_PImin()
{
	return ALIMIT_SPO2PIMIN->getCurValue();
}

/**********************************************************************************************//**
 * Gets alimit spo2 SIQmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit spo2 SIQmin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitSPO2_SIQmin()
{
	return ALIMIT_SPO2SIQMIN->getCurValue();
}

/**********************************************************************************************//**
 * Sets alimit maximum range pipmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangePIPmax(int iRangeVal)
{
	ALIMIT_PIPMAX->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range pipmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangePIPmax(int iRangeVal)
{
	DEBUGMSG(TRUE, (TEXT("5 PIPMAX->setMinValue %d\r\n"),iRangeVal));
	ALIMIT_PIPMAX->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range pipmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangePIPmin(int iRangeVal)
{
	ALIMIT_PIPMIN->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range pipmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangePIPmin(int iRangeVal)
{
	//DEBUGMSG(TRUE, (TEXT("ALIMIT_PIPMIN->setMinValue %d\r\n"),iRangeVal));//rku newPIPOFF
	ALIMIT_PIPMIN->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range peepmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangePEEPmin(int iRangeVal)
{
	//DEBUGMSG(TRUE, (TEXT("ALIMIT_PEEPMIN->setMaxValue %d\r\n"),iRangeVal));//rku newPIPOFF
	ALIMIT_PEEPMIN->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range peepmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangePEEPmin(int iRangeVal)
{
	ALIMIT_PEEPMIN->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range mvmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangeMVmin(int iRangeVal)
{
	ALIMIT_MVMIN->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range mvmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangeMVmin(int iRangeVal)
{
	ALIMIT_MVMIN->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range mvmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangeMVmax(int iRangeVal)
{
	ALIMIT_MVMAX->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range mvmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangeMVmax(int iRangeVal)
{
	ALIMIT_MVMAX->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range leakmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangeLeakmax(int iRangeVal)
{
	ALIMIT_LEAKMAX->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range leakmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangeLeakmax(int iRangeVal)
{
	ALIMIT_LEAKMAX->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range bpmmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangeBPMmax(int iRangeVal)
{
	ALIMIT_FREQMAX->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range bpmmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangeBPMmax(int iRangeVal)
{
	ALIMIT_FREQMAX->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range apnoe
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangeApnoe(int iRangeVal)
{
	ALIMIT_APNOE->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range apnoe
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangeApnoe(int iRangeVal)
{
	ALIMIT_APNOE->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range mapmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangeMAPmax(int iRangeVal)
{
	ALIMIT_MAPMAX->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range mapmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangeMAPmax(int iRangeVal)
{
	ALIMIT_MAPMAX->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range mapmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangeMAPmin(int iRangeVal)
{
	ALIMIT_MAPMIN->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range mapmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangeMAPmin(int iRangeVal)
{
	ALIMIT_MAPMIN->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range dco2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangeDCO2min(int iRangeVal)
{
	ALIMIT_DCO2MIN->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range dco2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangeDCO2min(int iRangeVal)
{
	ALIMIT_DCO2MIN->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range dco2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangeDCO2max(int iRangeVal)
{
	ALIMIT_DCO2MAX->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range dco2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangeDCO2max(int iRangeVal)
{
	ALIMIT_DCO2MAX->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range etco2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangeETCO2min(int iRangeVal)
{
	ALIMIT_ETCO2MIN->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range etco2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangeETCO2min(int iRangeVal)
{
	ALIMIT_ETCO2MIN->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range etco2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangeETCO2max(int iRangeVal)
{
	ALIMIT_ETCO2MAX->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range etco2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangeETCO2max(int iRangeVal)
{
	ALIMIT_ETCO2MAX->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range fico2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangeFICO2min(int iRangeVal)
{
	ALIMIT_FICO2MIN->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range fico2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangeFICO2min(int iRangeVal)
{
	ALIMIT_FICO2MIN->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range fico2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangeFICO2max(int iRangeVal)
{
	ALIMIT_FICO2MAX->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range fico2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangeFICO2max(int iRangeVal)
{
	ALIMIT_FICO2MAX->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range spo2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangeSPO2max(int iRangeVal)
{
	ALIMIT_SPO2MAX->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range spo2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangeSPO2max(int iRangeVal)
{
	ALIMIT_SPO2MAX->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range spo2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangeSPO2min(int iRangeVal)
{
	ALIMIT_SPO2MIN->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range spo2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangeSPO2min(int iRangeVal)
{
	ALIMIT_SPO2MIN->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range pulseratemin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangePulseRatemin(int iRangeVal)
{
	ALIMIT_PULSERATEMIN->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range pulseratemin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangePulseRatemin(int iRangeVal)
{
	ALIMIT_PULSERATEMIN->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range pulseratemax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangePulseRatemax(int iRangeVal)
{
	ALIMIT_PULSERATEMAX->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range pulseratemax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangePulseRatemax(int iRangeVal)
{
	ALIMIT_PULSERATEMAX->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range spo2 pimin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangeSPO2_PImin(int iRangeVal)
{
	ALIMIT_SPO2PIMIN->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range spo2 pimin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangeSPO2_PImin(int iRangeVal)
{
	ALIMIT_SPO2PIMIN->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit maximum range spo2 SIQmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMaxRangeSPO2_SIQmin(int iRangeVal)
{
	ALIMIT_SPO2SIQMIN->setMaxValue(iRangeVal);
}

/**********************************************************************************************//**
 * Sets alimit minimum range spo2 SIQmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	iRangeVal	Zero-based index of the range value.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitMinRangeSPO2_SIQmin(int iRangeVal)
{
	ALIMIT_SPO2SIQMIN->setMinValue(iRangeVal);
}

/**********************************************************************************************//**
 * Gets alimit maximum range pipmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range pipmax.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangePIPmax()
{
	return ALIMIT_PIPMAX->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range pipmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range pipmax.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangePIPmax()
{
	return ALIMIT_PIPMAX->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range pipmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range pipmin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangePIPmin()
{
	return ALIMIT_PIPMIN->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range pipmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range pipmin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangePIPmin()
{
	return ALIMIT_PIPMIN->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range peepmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range peepmin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangePEEPmin()
{
	return ALIMIT_PEEPMIN->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range peepmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range peepmin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangePEEPmin()
{
	return ALIMIT_PEEPMIN->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range mvmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range mvmin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangeMVmin()
{
	return ALIMIT_MVMIN->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range mvmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range mvmin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangeMVmin()
{
	return ALIMIT_MVMIN->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range mvmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range mvmax.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangeMVmax()
{
	return ALIMIT_MVMAX->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range mvmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range mvmax.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangeMVmax()
{
	return ALIMIT_MVMAX->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range leakmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range leakmax.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangeLeakmax()
{
	return ALIMIT_LEAKMAX->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range leakmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range leakmax.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangeLeakmax()
{
	return ALIMIT_LEAKMAX->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range bpmmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range bpmmax.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangeBPMmax()
{
	return ALIMIT_FREQMAX->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range bpmmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range bpmmax.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangeBPMmax()
{
	return ALIMIT_FREQMAX->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range apnoe
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range apnoe.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangeApnoe()
{
	return ALIMIT_APNOE->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range apnoe
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range apnoe.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangeApnoe()
{
	return ALIMIT_APNOE->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range mapmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range mapmax.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangeMAPmax()
{
	return ALIMIT_MAPMAX->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range mapmax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range mapmax.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangeMAPmax()
{
	return ALIMIT_MAPMAX->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range mapmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range mapmin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangeMAPmin()
{
	return ALIMIT_MAPMIN->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range mapmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range mapmin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangeMAPmin()
{
	return ALIMIT_MAPMIN->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range dco2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range dco2min.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangeDCO2min()
{
	return ALIMIT_DCO2MIN->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range dco2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range dco2min.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangeDCO2min()
{
	return ALIMIT_DCO2MIN->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range dco2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range dco2max.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangeDCO2max()
{
	return ALIMIT_DCO2MAX->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range dco2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range dco2max.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangeDCO2max()
{
	return ALIMIT_DCO2MAX->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range etco2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range etco2min.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangeETCO2min()
{
	return ALIMIT_ETCO2MIN->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range etco2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range etco2min.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangeETCO2min()
{
	return ALIMIT_ETCO2MIN->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range etco2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range etco2max.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangeETCO2max()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		eCO2unit unit=getModel()->getCONFIG()->GetCO2unit();
		switch(unit)
		{
		case CO2_UNIT_KPA:
			{
				return MAXRANGE_HF_NEONATAL_ETCO2MAX_KPA;
			}
			break;
		case CO2_UNIT_PERCENT:
			{
				return MAXRANGE_HF_NEONATAL_ETCO2MAX_VOL;
			}
			break;
		case CO2_UNIT_MMHG:
		default:
			{
				return MAXRANGE_HF_NEONATAL_ETCO2MAX_MMHG;
			}
			break;
		}
	}
	else
	{
		eCO2unit unit=getModel()->getCONFIG()->GetCO2unit();
		switch(unit)
		{
		case CO2_UNIT_KPA:
			{
				return MAXRANGE_HF_NEONATAL_ETCO2MAX_KPA;
			}
			break;
		case CO2_UNIT_PERCENT:
			{
				return MAXRANGE_HF_NEONATAL_ETCO2MAX_VOL;
			}
			break;
		case CO2_UNIT_MMHG:
		default:
			{
				return MAXRANGE_HF_NEONATAL_ETCO2MAX_MMHG;
			}
			break;
		}
	}
}

/**********************************************************************************************//**
 * Gets alimit minimum range etco2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range etco2max.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangeETCO2max()
{
	return ALIMIT_ETCO2MAX->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range fico2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range fico2min.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangeFICO2min()
{
	return ALIMIT_FICO2MIN->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range fico2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range fico2min.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangeFICO2min()
{
	return ALIMIT_FICO2MIN->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range fico2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range fico2max.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangeFICO2max()
{
	if(getModel()->getCONFIG()->GetVentRange()==NEONATAL)
	{
		eCO2unit unit=getModel()->getCONFIG()->GetCO2unit();
		switch(unit)
		{
		case CO2_UNIT_KPA:
			{
				return MAXRANGE_HF_NEONATAL_FICO2MAX_KPA;
			}
			break;
		case CO2_UNIT_PERCENT:
			{
				return MAXRANGE_HF_NEONATAL_FICO2MAX_VOL;
			}
			break;
		case CO2_UNIT_MMHG:
		default:
			{
				return MAXRANGE_HF_NEONATAL_FICO2MAX_MMHG;
			}
			break;
		}
	}
	else
	{
		eCO2unit unit=getModel()->getCONFIG()->GetCO2unit();
		switch(unit)
		{
		case CO2_UNIT_KPA:
			{
				return MAXRANGE_HF_NEONATAL_FICO2MAX_KPA;
			}
			break;
		case CO2_UNIT_PERCENT:
			{
				return MAXRANGE_HF_NEONATAL_FICO2MAX_VOL;
			}
			break;
		case CO2_UNIT_MMHG:
		default:
			{
				return MAXRANGE_HF_NEONATAL_FICO2MAX_MMHG;
			}
			break;
		}
	}

	
}

/**********************************************************************************************//**
 * Gets alimit minimum range fico2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range fico2max.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangeFICO2max()
{
	return ALIMIT_FICO2MAX->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range spo2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range spo2max.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangeSPO2max()
{
	return ALIMIT_SPO2MAX->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range spo2max
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range spo2max.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangeSPO2max()
{
	return ALIMIT_SPO2MAX->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range spo2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range spo2min.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangeSPO2min()
{
	return ALIMIT_SPO2MIN->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range spo2min
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range spo2min.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangeSPO2min()
{
	return ALIMIT_SPO2MIN->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range pulseratemax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range pulseratemax.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangePulseRatemax()
{
	return ALIMIT_PULSERATEMAX->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range pulseratemax
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range pulseratemax.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangePulseRatemax()
{
	return ALIMIT_PULSERATEMAX->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range pulseratemin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range pulseratemin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangePulseRatemin()
{
	return ALIMIT_PULSERATEMIN->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range pulseratemin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range pulseratemin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangePulseRatemin()
{
	return ALIMIT_PULSERATEMIN->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range spo2 pimin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range spo2 pimin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangeSPO2_PImin()
{
	return ALIMIT_SPO2PIMIN->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range spo2 pimin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range spo2 pimin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangeSPO2_PImin()
{
	return ALIMIT_SPO2PIMIN->getMinValue();
}

/**********************************************************************************************//**
 * Gets alimit maximum range spo2 SIQmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit maximum range spo2 SIQmin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMaxRangeSPO2_SIQmin()
{
	return ALIMIT_SPO2SIQMIN->getMaxValue();
}

/**********************************************************************************************//**
 * Gets alimit minimum range spo2 SIQmin
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \return	The alimit minimum range spo2 SIQmin.
 **************************************************************************************************/

int CAlarmConditionHandler::getAlimitMinRangeSPO2_SIQmin()
{
	return ALIMIT_SPO2SIQMIN->getMinValue();
}

/**********************************************************************************************//**
 * Sets alimit state apnoe limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_ApnoeLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALApnea AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALApnea AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALApnea AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALApnea AL_CALC"));
		}
		break;
	}
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitStateApnoeHF(state);
	}
	else
	{
		setAlimitStateApnoeDefault(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state apnoe hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateApnoeHF(eAlarmLimitState state)
{
	ALIMIT_APNOE->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateApnoeHF(state);
}

/**********************************************************************************************//**
 * Sets alimit state apnoe default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateApnoeDefault(eAlarmLimitState state)
{
	ALIMIT_APNOE->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateApnoe(state);
}

/**********************************************************************************************//**
 * Sets alimit state bpmmax limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_BPMmaxLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALBPMmax AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALBPMmax AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALBPMmax AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALBPMmax AL_CALC"));
		}
		break;
	}
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitStateBPMmaxHF(state);
	}
	else
	{
		setAlimitStateBPMmaxDefault(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state bpmmax hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateBPMmaxHF(eAlarmLimitState state)
{
	ALIMIT_FREQMAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateBPMmaxHF(state);
}

/**********************************************************************************************//**
 * Sets alimit state bpmmax default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateBPMmaxDefault(eAlarmLimitState state)
{
	ALIMIT_FREQMAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateBPMmax(state);
}

/**********************************************************************************************//**
 * Sets alimit state leakmax limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_LeakmaxLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALLeakmax AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALLeakmax AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALLeakmax AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALLeakmax AL_CALC"));
		}
		break;
	}
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitStateLeakmaxHF(state);
	}
	else
	{
		setAlimitStateLeakmaxDefault(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state leakmax hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateLeakmaxHF(eAlarmLimitState state)
{
	ALIMIT_LEAKMAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateLeakmaxHF(state);
}

/**********************************************************************************************//**
 * Sets alimit state leakmax default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateLeakmaxDefault(eAlarmLimitState state)
{
	ALIMIT_LEAKMAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateLeakmax(state);
}

/**********************************************************************************************//**
 * Sets alimit state mvmax limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_MVmaxLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALMVmax AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALMVmax AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALMVmax AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALMVmax AL_CALC"));
		}
		break;
	}
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitStateMVmaxHF(state);
	}
	else
	{
		setAlimitStateMVmaxDefault(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state mvmax hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateMVmaxHF(eAlarmLimitState state)
{
	ALIMIT_MVMAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateMVmaxHF(state);
}

/**********************************************************************************************//**
 * Sets alimit state mvmax default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateMVmaxDefault(eAlarmLimitState state)
{
	ALIMIT_MVMAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateMVmax(state);
}

/**********************************************************************************************//**
 * Sets alimit state mvmin limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_MVminLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALMVmin AL_ON"));
			DEBUGMSG(TRUE, (TEXT("ALMVmin AL_ON\r\n")));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALMVmin AL_OFF"));
			DEBUGMSG(TRUE, (TEXT("ALMVmin AL_OFF\r\n")));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALMVmin AL_AUTO"));
			DEBUGMSG(TRUE, (TEXT("ALMVmin AL_AUTO\r\n")));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALMVmin AL_CALC"));
			DEBUGMSG(TRUE, (TEXT("ALMVmin AL_CALC\r\n")));
		}
		break;
	}
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitStateMVminHF(state);
	}
	else
	{
		setAlimitStateMVminDefault(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state mvmin hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateMVminHF(eAlarmLimitState state)
{
	ALIMIT_MVMIN->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateMVminHF(state);
}

/**********************************************************************************************//**
 * Sets alimit state mvmin default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateMVminDefault(eAlarmLimitState state)
{
	ALIMIT_MVMIN->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateMVmin(state);
}

/**********************************************************************************************//**
 * Sets alimit state peepmin limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_PEEPminLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALPmin AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALPmin AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALPmin AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALPmin AL_CALC"));
		}
		break;
	}
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_THERAPIE)
	{

	}
	else if(eActiveVentMode==VM_HFO)
	{
		
	}
	else if(eActiveVentMode==VM_CPAP)
	{
		setAlimitStatePEEPminCpap(state);
	}
	else if(eActiveVentMode==VM_NCPAP)
	{
		setAlimitStatePEEPminNCPAP(state);
	}
	else if(eActiveVentMode==VM_DUOPAP)
	{
		setAlimitStatePEEPminDUOPAP(state);
	}
	else
	{
		setAlimitStatePEEPminDefault(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state peepmin cpap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStatePEEPminCpap(eAlarmLimitState state)
{
	ALIMIT_PEEPMIN->setLimitState(state);
	if(state==AL_OFF || state==AL_ON || state==AL_AUTO)
		getModel()->getCONFIG()->SetAlarmlimitStatePEEPminCpap(state);
}

/**********************************************************************************************//**
 * Sets alimit state peepmin ncpap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStatePEEPminNCPAP(eAlarmLimitState state)
{
	ALIMIT_PEEPMIN->setLimitState(state);
	if(state==AL_OFF || state==AL_ON || state==AL_AUTO)
		getModel()->getCONFIG()->SetAlarmlimitStatePEEPminNCPAP(state);
}

/**********************************************************************************************//**
 * Sets alimit state peepmin duopap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStatePEEPminDUOPAP(eAlarmLimitState state)
{
	ALIMIT_PEEPMIN->setLimitState(state);
	if(state==AL_OFF || state==AL_ON || state==AL_AUTO)
		getModel()->getCONFIG()->SetAlarmlimitStatePEEPminDUOPAP(state);
}

/**********************************************************************************************//**
 * Sets alimit state peepmin default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStatePEEPminDefault(eAlarmLimitState state)
{
	ALIMIT_PEEPMIN->setLimitState(state);
	if(state==AL_OFF || state==AL_ON || state==AL_AUTO)
		getModel()->getCONFIG()->SetAlarmlimitStatePEEPmin(state);
}

/**********************************************************************************************//**
 * Sets alimit state pipmin limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_PIPminLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALPIPmin AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALPIPmin AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALPIPmin AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALPIPmin AL_CALC"));
		}
		break;
	}
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_THERAPIE)
	{

	}
	else if(eActiveVentMode==VM_HFO)
	{
		
	}
	else if(eActiveVentMode==VM_CPAP)
	{
		
	}
	else if(eActiveVentMode==VM_NCPAP)
	{
		
	}
	else if(eActiveVentMode==VM_DUOPAP)
	{
		setAlimitStatePIPminDUOPAP(state);
	}
	else
	{
		setAlimitStatePIPminDefault(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state pipmin default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStatePIPminDefault(eAlarmLimitState state)
{
	ALIMIT_PIPMIN->setLimitState(state);
	if(state==AL_OFF || state==AL_ON || state==AL_AUTO)
		getModel()->getCONFIG()->SetAlarmlimitStatePIPmin(state);
}

/**********************************************************************************************//**
 * Sets alimit state pipmin duopap
 *
 * \author	Rainer Kühner
 * \date	06.03.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStatePIPminDUOPAP(eAlarmLimitState state)//rku PIPLOW DUOPAP
{
	ALIMIT_PIPMIN->setLimitState(state);
	if(state==AL_OFF || state==AL_ON || state==AL_AUTO)
		getModel()->getCONFIG()->SetAlarmlimitStatePIPminDUOPAP(state);
}

/**********************************************************************************************//**
 * Sets alimit state pipmax limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_PIPmaxLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALPmax AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALPmax AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALPmax AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALPIPmax AL_CALC"));
		}
		break;
	}
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_THERAPIE)
	{

	}
	else if(eActiveVentMode==VM_HFO)
	{
		setAlimitStatePIPmaxHF(state);
	}
	else if(eActiveVentMode==VM_CPAP)
	{
		setAlimitStatePIPmaxCpap(state);
	}
	else if(eActiveVentMode==VM_NCPAP)
	{
		setAlimitStatePIPmaxNCPAP(state);
	}
	else if(eActiveVentMode==VM_DUOPAP)
	{
		setAlimitStatePIPmaxDUOPAP(state);
	}
	else
	{
		setAlimitStatePIPmaxDefault(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state pipmax hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStatePIPmaxHF(eAlarmLimitState state)
{
	ALIMIT_PIPMAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON || state==AL_AUTO)
		getModel()->getCONFIG()->SetAlarmlimitStatePmaxHF(state);
}

/**********************************************************************************************//**
 * Sets alimit state pipmax cpap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStatePIPmaxCpap(eAlarmLimitState state)
{
	ALIMIT_PIPMAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON || state==AL_AUTO)
		getModel()->getCONFIG()->SetAlarmlimitStatePmaxCpap(state);
}

/**********************************************************************************************//**
 * Sets alimit state pipmax ncpap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStatePIPmaxNCPAP(eAlarmLimitState state)
{
	ALIMIT_PIPMAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON || state==AL_AUTO)
		getModel()->getCONFIG()->SetAlarmlimitStatePmaxNCPAP(state);
}

/**********************************************************************************************//**
 * Sets alimit state pipmax duopap
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStatePIPmaxDUOPAP(eAlarmLimitState state)
{
	ALIMIT_PIPMAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON || state==AL_AUTO)
		getModel()->getCONFIG()->SetAlarmlimitStatePIPmaxDUOPAP(state);
}

/**********************************************************************************************//**
 * Sets alimit state pipmax default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStatePIPmaxDefault(eAlarmLimitState state)
{
	ALIMIT_PIPMAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON || state==AL_AUTO)
		getModel()->getCONFIG()->SetAlarmlimitStatePIPmax(state);
}

/**********************************************************************************************//**
 * Sets alimit state mapmax limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_MAPmaxLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALMAPmax AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALMAPmax AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALMAPmax AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALMAPmax AL_CALC"));
		}
		break;
	}
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitStateMAPmaxHF(state);
	}

}

/**********************************************************************************************//**
 * Sets alimit state mapmax hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateMAPmaxHF(eAlarmLimitState state)
{
	ALIMIT_MAPMAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON || state==AL_AUTO)
		getModel()->getCONFIG()->SetAlarmlimitStateMAPmaxHF(state);
}

/**********************************************************************************************//**
 * Sets alimit state mapmin limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_MAPminLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALMAPmin AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALMAPmin AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALMAPmin AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALMAPmin AL_CALC"));
		}
		break;
	}
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitStateMAPminHF(state);
	}

}

/**********************************************************************************************//**
 * Sets alimit state mapmin hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateMAPminHF(eAlarmLimitState state)
{
	ALIMIT_MAPMIN->setLimitState(state);
	if(state==AL_OFF || state==AL_ON || state==AL_AUTO)
		getModel()->getCONFIG()->SetAlarmlimitStateMAPminHF(state);
}

/**********************************************************************************************//**
 * Sets alimit state dco2max limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_DCO2maxLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALDCO2max AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALDCO2max AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALDCO2max AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALDCO2max AL_CALC"));
		}
		break;
	}
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitStateDCO2maxHF(state);
	}
	
}

/**********************************************************************************************//**
 * Sets alimit state dco2max hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateDCO2maxHF(eAlarmLimitState state)
{
	ALIMIT_DCO2MAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateDCO2maxHF(state);
}

/**********************************************************************************************//**
 * Sets alimit state dco2min limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_DCO2minLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALDCO2min AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALDCO2min AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALDCO2min AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALDCO2min AL_CALC"));
		}
		break;
	}
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitStateDCO2minHF(state);
	}
	
}

/**********************************************************************************************//**
 * Sets alimit state dco2min hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateDCO2minHF(eAlarmLimitState state)
{
	ALIMIT_DCO2MIN->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateDCO2minHF(state);
}

/**********************************************************************************************//**
 * Sets alimit state etco2max limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_ETCO2maxLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALETCO2max AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALETCO2max AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALETCO2max AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALETCO2max AL_CALC"));
		}
		break;
	}
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitStateETCO2maxHF(state);
	}
	else
	{
		setAlimitStateETCO2maxDefault(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state etco2max hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateETCO2maxHF(eAlarmLimitState state)
{
	ALIMIT_ETCO2MAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateETCO2maxHF(state);
}

/**********************************************************************************************//**
 * Sets alimit state etco2max default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateETCO2maxDefault(eAlarmLimitState state)
{
	ALIMIT_ETCO2MAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateETCO2max(state);
}

/**********************************************************************************************//**
 * Sets alimit state etco2min limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_ETCO2minLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALETCO2min AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALETCO2min AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALETCO2min AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALETCO2min AL_CALC"));
		}
		break;
	}
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitStateETCO2minHF(state);
	}
	else
	{
		setAlimitStateETCO2minDefault(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state etco2min hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateETCO2minHF(eAlarmLimitState state)
{
	ALIMIT_ETCO2MIN->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateETCO2minHF(state);
}

/**********************************************************************************************//**
 * Sets alimit state etco2min default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateETCO2minDefault(eAlarmLimitState state)
{
	ALIMIT_ETCO2MIN->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateETCO2min(state);
}

/**********************************************************************************************//**
 * Sets alimit state fico2max limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_FICO2maxLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALFICO2max AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALFICO2max AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALFICO2max AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALFICO2max AL_CALC"));
		}
		break;
	}
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitStateFICO2maxHF(state);
	}
	else
	{
		setAlimitStateFICO2maxDefault(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state fico2max hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateFICO2maxHF(eAlarmLimitState state)
{
	ALIMIT_FICO2MAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateFICO2maxHF(state);
}

/**********************************************************************************************//**
 * Sets alimit state fico2max default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateFICO2maxDefault(eAlarmLimitState state)
{
	ALIMIT_FICO2MAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateFICO2max(state);
}

/**********************************************************************************************//**
 * Sets alimit state fico2min limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_FICO2minLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALFICO2min AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALFICO2min AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALFICO2min AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALFICO2min AL_CALC"));
		}
		break;
	}
	eVentMode eActiveVentMode=getModel()->getVMODEHANDLER()->getCurrentActiveMode();
	if(eActiveVentMode==VM_HFO)
	{
		setAlimitStateFICO2minHF(state);
	}
	else
	{
		setAlimitStateFICO2minDefault(state);
	}
}

/**********************************************************************************************//**
 * Sets alimit state fico2min hf
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateFICO2minHF(eAlarmLimitState state)
{
	ALIMIT_FICO2MIN->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateFICO2minHF(state);
}

/**********************************************************************************************//**
 * Sets alimit state fico2min default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateFICO2minDefault(eAlarmLimitState state)
{
	ALIMIT_FICO2MIN->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateFICO2min(state);
}

/**********************************************************************************************//**
 * Sets alimit state spo2max limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_SPO2maxLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALSPO2max AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALSPO2max AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALSPO2max AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALSPO2max AL_CALC"));
		}
		break;
	}
	setAlimitStateSPO2maxDefault(state);
}

/**********************************************************************************************//**
 * Sets alimit state spo2max default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateSPO2maxDefault(eAlarmLimitState state)
{
	ALIMIT_SPO2MAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateSPO2max(state);
}

/**********************************************************************************************//**
 * Sets alimit state spo2min limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_SPO2minLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALSPO2min AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALSPO2min AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALSPO2minAL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALSPO2min AL_CALC"));
		}
		break;
	}
	setAlimitStateSPO2minDefault(state);
}

/**********************************************************************************************//**
 * Sets alimit state spo2min default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateSPO2minDefault(eAlarmLimitState state)
{
	ALIMIT_SPO2MIN->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateSPO2min(state);
}

/**********************************************************************************************//**
 * Sets alimit state pulseratemax limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_PulseRatemaxLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALPulseRatemax AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALPulseRatemax AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALPulseRatemax AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALPulseRatemax AL_CALC"));
		}
		break;
	}
	setAlimitStatePulseRatemaxDefault(state);
}

/**********************************************************************************************//**
 * Sets alimit state pulseratemax default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStatePulseRatemaxDefault(eAlarmLimitState state)
{
	ALIMIT_PULSERATEMAX->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStatePulseRatemax(state);
}

/**********************************************************************************************//**
 * Sets alimit state pulseratemin limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_PulseRateminLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALPulseRatemin AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALPulseRatemin AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALPulseRatemin AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALPulseRatemin AL_CALC"));
		}
		break;
	}
	setAlimitStatePulseRateminDefault(state);
}

/**********************************************************************************************//**
 * Sets alimit state pulseratemin default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStatePulseRateminDefault(eAlarmLimitState state)
{
	ALIMIT_PULSERATEMIN->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStatePulseRatemin(state);
}

/**********************************************************************************************//**
 * Sets alimit state spo2 pimin limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_SPO2_PIminLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALSPO2_PImin AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALSPO2_PImin AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALSPO2_PImin AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALSPO2_PImin AL_CALC"));
		}
		break;
	}
	setAlimitStateSPO2_PIminDefault(state);
}

/**********************************************************************************************//**
 * Sets alimit state spo2 pimin default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateSPO2_PIminDefault(eAlarmLimitState state)
{
	ALIMIT_SPO2PIMIN->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateSPO2_PImin(state);
}

/**********************************************************************************************//**
 * Sets alimit state spo2 SIQmin limit
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitState_SPO2_SIQminLimit(eAlarmLimitState state)
{
	switch(state)
	{
	case AL_ON:
		{
			theApp.getLog()->WriteLine(_T("ALSPO2_SIQmin AL_ON"));
		}
		break;
	case AL_OFF:
		{
			theApp.getLog()->WriteLine(_T("ALSPO2_SIQmin AL_OFF"));
		}
		break;
	case AL_AUTO:
		{
			theApp.getLog()->WriteLine(_T("ALSPO2_SIQmin AL_AUTO"));
		}
		break;
	case AL_CALC:
		{
			theApp.getLog()->WriteLine(_T("ALSPO2_SIQmin AL_CALC"));
		}
		break;
	}
	setAlimitStateSPO2_SIQminDefault(state);
}

/**********************************************************************************************//**
 * Sets alimit state spo2 SIQmin default
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 *
 * \param	state	The state.
 **************************************************************************************************/

void CAlarmConditionHandler::setAlimitStateSPO2_SIQminDefault(eAlarmLimitState state)
{
	ALIMIT_SPO2SIQMIN->setLimitState(state);
	if(state==AL_OFF || state==AL_ON)
		getModel()->getCONFIG()->SetAlarmlimitStateSPO2_SIQmin(state);
}

/**********************************************************************************************//**
 * Sets alimit state calculated
 *
 * \author	Rainer Kühner
 * \date	19.02.2018
 **************************************************************************************************/

void CAlarmConditionHandler::SetAlimitStateCalculated()
{
	EnterCriticalSection(&csAlarmLimitList);
	int iCntLimitList=m_pAlarmLimitlist->getCount();
	if(iCntLimitList>0)
	{
		for(int iLimit=0;iLimit<iCntLimitList;iLimit++)
		{
			if(AL_CALC==m_pAlarmLimitlist->getLimitState((eAlarmLimits)iLimit))
				m_pAlarmLimitlist->setLimitState((eAlarmLimits)iLimit,AL_ON);
		}
	}
	LeaveCriticalSection(&csAlarmLimitList);

	getModel()->SetAlimitStateCalculating(false);
}